//------------------------------------------------------------------------------
// driver.cpp
// Entry point for the primary driver program.
//
// File is under the MIT license; see LICENSE for details
//------------------------------------------------------------------------------

#include <fstream>
#include <iostream>

#include "lnast_visitor.hpp"

#include "slang/compilation/Compilation.h"
#include "slang/diagnostics/DiagnosticEngine.h"
#include "slang/diagnostics/TextDiagnosticClient.h"
#include "slang/parsing/Preprocessor.h"
#include "slang/symbols/ASTSerializer.h"
#include "slang/symbols/CompilationUnitSymbols.h"
#include "slang/syntax/SyntaxPrinter.h"
#include "slang/syntax/SyntaxTree.h"
#include "slang/text/Json.h"
#include "slang/text/SourceManager.h"
#include "slang/util/CommandLine.h"
#include "slang/util/OS.h"
#include "slang/util/String.h"
#include "slang/util/Version.h"

using namespace slang;

void writeToFile(string_view fileName, string_view contents);

bool runPreprocessor(SourceManager& sourceManager, const Bag& options,
                     const std::vector<SourceBuffer>& buffers, bool includeComments,
                     bool includeDirectives) {
    BumpAllocator alloc;
    Diagnostics diagnostics;
    Preprocessor preprocessor(sourceManager, alloc, diagnostics, options);

    for (auto it = buffers.rbegin(); it != buffers.rend(); it++)
        preprocessor.pushSource(*it);

    SyntaxPrinter output;
    output.setIncludeComments(includeComments);
    output.setIncludeDirectives(includeDirectives);

    while (true) {
        slang::Token token = preprocessor.next();
        output.print(token);
        if (token.kind == TokenKind::EndOfFile)
            break;
    }

    // Only print diagnostics if actual errors occurred.
    for (auto& diag : diagnostics) {
        if (diag.isError()) {
            OS::print("{}", DiagnosticEngine::reportAll(sourceManager, diagnostics));
            return false;
        }
    }

    OS::print("{}\n", output.str());
    return true;
}

void printMacros(SourceManager& sourceManager, const Bag& options,
                 const std::vector<SourceBuffer>& buffers) {
    BumpAllocator alloc;
    Diagnostics diagnostics;
    Preprocessor preprocessor(sourceManager, alloc, diagnostics, options);

    for (auto it = buffers.rbegin(); it != buffers.rend(); it++)
        preprocessor.pushSource(*it);

    while (true) {
        slang::Token token = preprocessor.next();
        if (token.kind == TokenKind::EndOfFile)
            break;
    }

    for (auto macro : preprocessor.getDefinedMacros()) {
        SyntaxPrinter printer;
        printer.setIncludeComments(false);
        printer.setIncludeTrivia(false);
        printer.print(macro->name);

        printer.setIncludeTrivia(true);
        if (macro->formalArguments)
            printer.print(*macro->formalArguments);
        printer.print(macro->body);

        OS::print("{}\n", printer.str());
    }
}

bool runCompiler(SourceManager& sourceManager, const Bag& options,
                 const std::vector<SourceBuffer>& buffers,
                 const std::vector<std::string>& warningOptions, uint32_t errorLimit,
                 bool singleUnit, bool onlyParse, bool showColors,
                 const optional<std::string>& astJsonFile) {

    Compilation compilation(options);
    if (singleUnit) {
        compilation.addSyntaxTree(SyntaxTree::fromBuffers(buffers, sourceManager, options));
    }
    else {
        for (const SourceBuffer& buffer : buffers)
            compilation.addSyntaxTree(SyntaxTree::fromBuffer(buffer, sourceManager, options));
    }

    DiagnosticEngine diagEngine(sourceManager);
    Diagnostics optionDiags = diagEngine.setWarningOptions(warningOptions);
    Diagnostics pragmaDiags = diagEngine.setMappingsFromPragmas();
    diagEngine.setErrorLimit(errorLimit);

    auto client = std::make_shared<TextDiagnosticClient>();
    client->setColorsEnabled(showColors);
    diagEngine.addClient(client);

    for (auto& diag : optionDiags)
        diagEngine.issue(diag);

    for (auto& diag : pragmaDiags)
        diagEngine.issue(diag);

    if (onlyParse) {
        for (auto& diag : compilation.getParseDiagnostics())
            diagEngine.issue(diag);
    }
    else {
        for (auto& diag : compilation.getAllDiagnostics())
            diagEngine.issue(diag);
    }

    OS::print("{}", client->getString());

    if (astJsonFile) {
        JsonWriter writer;
        writer.setPrettyPrint(true);

        ASTSerializer serializer(compilation, writer);
        serializer.serialize(compilation.getRoot());

        writeToFile(*astJsonFile, writer.view());
    }

    Lnast_visitor lnast_visitor(compilation, 0, 0);

    compilation.getRoot().visit(lnast_visitor);

    return diagEngine.getNumErrors() == 0;
}

template<typename TArgs>
int driverMain(int argc, TArgs argv) try {
    CommandLine cmdLine;

    // General
    optional<bool> showHelp;
    optional<bool> showVersion;
    cmdLine.add("-h,--help", showHelp, "Display available options");
    cmdLine.add("-v,--version", showVersion, "Display version information and exit");

    // Output control
    optional<bool> onlyPreprocess;
    optional<bool> onlyParse;
    optional<bool> onlyMacros;
    cmdLine.add("-E,--preprocess", onlyPreprocess,
                "Only run the preprocessor (and print preprocessed files to stdout)");
    cmdLine.add("--macros-only", onlyMacros, "Print a list of found macros and exit");
    cmdLine.add("--parse-only", onlyParse,
                "Stop after parsing input files, don't perform elaboration or type checking");

    // Include paths
    std::vector<std::string> includeDirs;
    std::vector<std::string> includeSystemDirs;
    cmdLine.add("-I,--include-directory", includeDirs, "Additional include search paths", "<dir>");
    cmdLine.add("--isystem", includeSystemDirs, "Additional system include search paths", "<dir>");

    // Preprocessor
    optional<bool> includeComments;
    optional<bool> includeDirectives;
    optional<uint32_t> maxIncludeDepth;
    std::vector<std::string> defines;
    std::vector<std::string> undefines;
    cmdLine.add("-D,--define-macro", defines,
                "Define <macro> to <value> (or 1 if <value> ommitted) in all source files",
                "<macro>=<value>");
    cmdLine.add("-U,--undefine-macro", undefines,
                "Undefine macro name at the start of all source files", "<macro>");
    cmdLine.add("--comments", includeComments, "Include comments in preprocessed output (with -E)");
    cmdLine.add("--directives", includeDirectives,
                "Include compiler directives in preprocessed output (with -E)");
    cmdLine.add("--max-include-depth", maxIncludeDepth,
                "Maximum depth of nested include files allowed", "<depth>");

    // Parsing
    optional<uint32_t> maxParseDepth;
    optional<uint32_t> maxLexerErrors;
    cmdLine.add("--max-parse-depth", maxParseDepth,
                "Maximum depth of nested language constructs allowed", "<depth>");
    cmdLine.add("--max-lexer-errors", maxLexerErrors,
                "Maximum number of errors that can occur during lexing before the rest of the file "
                "is skipped",
                "<count>");

    // JSON dumping
    optional<std::string> astJsonFile;
    cmdLine.add("--ast-json", astJsonFile,
                "Dump the compiled AST in JSON format to the specified file, or '-' for stdout",
                "<file>");

    // Compilation
    optional<uint32_t> maxInstanceDepth;
    optional<uint32_t> maxGenerateSteps;
    optional<uint32_t> maxConstexprDepth;
    optional<uint32_t> maxConstexprSteps;
    optional<uint32_t> maxConstexprBacktrace;
    cmdLine.add("--max-hierarchy-depth", maxInstanceDepth, "Maximum depth of the design hierarchy",
                "<depth>");
    cmdLine.add("--max-generate-steps", maxGenerateSteps,
                "Maximum number of steps that can occur during generate block "
                "evaluation before giving up",
                "<steps>");
    cmdLine.add("--max-constexpr-depth", maxConstexprDepth,
                "Maximum depth of a constant evaluation call stack", "<depth>");
    cmdLine.add("--max-constexpr-steps", maxConstexprSteps,
                "Maximum number of steps that can occur during constant "
                "evaluation before giving up",
                "<steps>");
    cmdLine.add("--constexpr-backtrace-limit", maxConstexprBacktrace,
                "Maximum number of frames to show when printing a constant evaluation "
                "backtrace; the rest will be abbreviated",
                "<limit>");

    // Diagnostics control
    optional<bool> colorDiags;
    optional<uint32_t> errorLimit;
    std::vector<std::string> warningOptions;
    cmdLine.add("-W", warningOptions, "Control the specified warning", "<warning>");
    cmdLine.add("--color-diagnostics", colorDiags,
                "Always print diagnostics in color."
                "If this option is unset, colors will be enabled if a color-capable "
                "terminal is detected.");
    cmdLine.add("--error-limit", errorLimit,
                "Limit on the number of errors that will be printed. Setting this to zero will "
                "disable the limit.",
                "<limit>");

    // File list
    optional<bool> singleUnit;
    std::vector<std::string> sourceFiles;
    cmdLine.add("--single-unit", singleUnit, "Treat all input files as a single compilation unit");
    cmdLine.setPositional(sourceFiles, "files");

    if (!cmdLine.parse(argc, argv)) {
        for (auto& err : cmdLine.getErrors())
            OS::print("{}\n", err);
        return 1;
    }

    if (showHelp == true) {
        OS::print("{}", cmdLine.getHelpText("slang SystemVerilog compiler"));
        return 0;
    }

    if (showVersion == true) {
        OS::print("slang version {}.{}.{}\n", VersionInfo::getMajor(), VersionInfo::getMinor(),
                  VersionInfo::getRevision());
        return 0;
    }

    bool anyErrors = false;
    SourceManager sourceManager;
    for (const std::string& dir : includeDirs) {
        try {
            sourceManager.addUserDirectory(string_view(dir));
        }
        catch (const std::exception&) {
            OS::print("error: include directory '{}' does not exist\n", dir);
            anyErrors = true;
        }
    }

    for (const std::string& dir : includeSystemDirs) {
        try {
            sourceManager.addSystemDirectory(string_view(dir));
        }
        catch (const std::exception&) {
            OS::print("error: include directory '{}' does not exist\n", dir);
            anyErrors = true;
        }
    }

    PreprocessorOptions ppoptions;
    ppoptions.predefines = defines;
    ppoptions.undefines = undefines;
    ppoptions.predefineSource = "<command-line>";
    if (maxIncludeDepth.has_value())
        ppoptions.maxIncludeDepth = *maxIncludeDepth;

    LexerOptions loptions;
    if (maxLexerErrors.has_value())
        loptions.maxErrors = *maxLexerErrors;

    ParserOptions poptions;
    if (maxParseDepth.has_value())
        poptions.maxRecursionDepth = *maxParseDepth;

    CompilationOptions coptions;
    if (maxInstanceDepth.has_value())
        coptions.maxInstanceDepth = *maxInstanceDepth;
    if (maxGenerateSteps.has_value())
        coptions.maxGenerateSteps = *maxGenerateSteps;
    if (maxConstexprDepth.has_value())
        coptions.maxConstexprDepth = *maxConstexprDepth;
    if (maxConstexprSteps.has_value())
        coptions.maxConstexprSteps = *maxConstexprSteps;
    if (maxConstexprBacktrace.has_value())
        coptions.maxConstexprBacktrace = *maxConstexprBacktrace;
    if (errorLimit.has_value())
        coptions.errorLimit = *errorLimit * 2;

    Bag options;
    options.set(ppoptions);
    options.set(loptions);
    options.set(poptions);
    options.set(coptions);

    std::vector<SourceBuffer> buffers;
    for (const std::string& file : sourceFiles) {
        SourceBuffer buffer = sourceManager.readSource(file);
        if (!buffer) {
            OS::print("error: no such file or directory: '{}'\n", file);
            anyErrors = true;
            continue;
        }

        buffers.push_back(buffer);
    }

    if (anyErrors)
        return 2;

    if (buffers.empty()) {
        OS::print("error: no input files\n");
        return 3;
    }

    if (onlyParse.has_value() + onlyPreprocess.has_value() + onlyMacros.has_value() > 1) {
        OS::print("Can only specify one of --preprocess, --macros-only, --parse-only");
        return 4;
    }

    bool showColors;
    if (colorDiags)
        showColors = *colorDiags;
    else
        showColors = OS::fileSupportsColors(stdout);

    try {
        if (onlyPreprocess == true) {
            anyErrors = !runPreprocessor(sourceManager, options, buffers, includeComments == true,
                                         includeDirectives == true);
        }
        else if (onlyMacros == true) {
            printMacros(sourceManager, options, buffers);
        }
        else {
            anyErrors = !runCompiler(sourceManager, options, buffers, warningOptions,
                                     errorLimit.value_or(20), singleUnit == true, onlyParse == true,
                                     showColors, astJsonFile);
        }
    }
    catch (const std::exception& e) {
        OS::print("internal compiler error: {}\n", e.what());
        return 4;
    }

    return anyErrors ? 1 : 0;
}
catch (const std::exception& e) {
    OS::print("{}\n", e.what());
    return 5;
}

template<typename Stream, typename String>
void writeToFile(Stream& os, string_view fileName, String contents) {
    os.write(contents.data(), contents.size());
    os.flush();
    if (!os)
        throw std::runtime_error(fmt::format("Unable to write AST to '{}'", fileName));
}

void writeToFile(string_view fileName, string_view contents) {
    if (fileName == "-") {
        writeToFile(std::cout, "stdout", contents);
    }
    else {
        std::ofstream file{ std::string(fileName) };
        writeToFile(file, fileName, contents);
    }
}

int main(int argc, char** argv) {
    return driverMain(argc, argv);
}
