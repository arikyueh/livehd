
Α
ΑΑ
ICache
clock" 
reset

io_req_valid

io_req_bits_addr
'
io_s1_paddr
 

io_s1_kill


io_s2_kill

io_resp_ready

io_resp_valid

io_resp_bits_data
"
io_resp_bits_datablock
@
io_invalidate

io_mem_0_a_ready

io_mem_0_a_valid
"
io_mem_0_a_bits_opcode
!
io_mem_0_a_bits_param
 
io_mem_0_a_bits_size
"
io_mem_0_a_bits_source
#
io_mem_0_a_bits_address
  
io_mem_0_a_bits_mask
 
io_mem_0_a_bits_data
@
io_mem_0_b_ready

io_mem_0_b_valid
"
io_mem_0_b_bits_opcode
!
io_mem_0_b_bits_param
 
io_mem_0_b_bits_size
"
io_mem_0_b_bits_source
#
io_mem_0_b_bits_address
  
io_mem_0_b_bits_mask
 
io_mem_0_b_bits_data
@
io_mem_0_c_ready

io_mem_0_c_valid
"
io_mem_0_c_bits_opcode
!
io_mem_0_c_bits_param
 
io_mem_0_c_bits_size
"
io_mem_0_c_bits_source
#
io_mem_0_c_bits_address
  
io_mem_0_c_bits_data
@!
io_mem_0_c_bits_error

io_mem_0_d_ready

io_mem_0_d_valid
"
io_mem_0_d_bits_opcode
!
io_mem_0_d_bits_param
 
io_mem_0_d_bits_size
"
io_mem_0_d_bits_source
 
io_mem_0_d_bits_sink
#
io_mem_0_d_bits_addr_lo
 
io_mem_0_d_bits_data
@!
io_mem_0_d_bits_error

io_mem_0_e_ready

io_mem_0_e_valid
 
io_mem_0_e_bits_sink
J1
tag_array_0
 (2	tag_rdata:_T_328J
@ICache.scala 97:25J1
tag_array_1
 (2	tag_rdata:_T_328J
@ICache.scala 97:25J1
tag_array_2
 (2	tag_rdata:_T_328J
@ICache.scala 97:25J1
tag_array_3
 (2	tag_rdata:_T_328J
@ICache.scala 97:25D*
_T_550
@ (2_T_566:_T_556J
 ICache.scala 132:28D*
_T_572
@ (2_T_588:_T_578J
 ICache.scala 132:28D*
_T_594
@ (2_T_610:_T_600J
 ICache.scala 132:28D*
_T_616
@ (2_T_632:_T_622J
 ICache.scala 132:28K2
state
	

clock"	

0*	

stateICache.scala 67:18W>
invalidated
	

clock"	

0*

invalidatedICache.scala 68:24F2-
stall$R"

io_resp_ready	

0ICache.scala 69:15W>
refill_addr
 	

clock"	

0*

refill_addrICache.scala 71:24Q8
s1_valid
	

clock"	

0*


s1_validICache.scala 74:21D2+
_T_221!R


io_s1_kill	

0ICache.scala 75:31A2(
_T_222R


s1_valid


_T_221ICache.scala 75:28?2&
_T_223R	

state	

0ICache.scala 75:52B2)
	out_validR


_T_222


_T_223ICache.scala 75:43C2*
s1_idx R

io_s1_paddr
11
6ICache.scala 76:27D2+
s1_tag!R

io_s1_paddr
31
12ICache.scala 77:27H2.
_T_436$R"

io_invalidate	

0ICache.scala 122:17S9
vb_array
	

clock"	

0*


vb_arrayICache.scala 104:21D2*
_T_438 R

io_s1_paddr
11
6ICache.scala 122:68=2'
_T_439R	

0


_T_438Cat.scala 30:58B2(
_T_440R


vb_array


_T_439ICache.scala 122:43>2$
_T_441R


_T_440
0
0ICache.scala 122:43>2$
_T_442R


_T_441
0
0ICache.scala 122:97@2&
_T_443R


_T_436


_T_442ICache.scala 122:32XB
s1_dout_valid
	

clock"	

0*

s1_dout_validReg.scala 14:44]2C
_T_4509R7(:&
:


tag_array_0	tag_rdatadata
19
0ICache.scala 125:32@2&
_T_451R


_T_450


s1_tagICache.scala 125:46J4
_T_453
	

clock"	

0*


_T_453Reg.scala 34:16Q27
_T_454-2+


s1_dout_valid


_T_451


_T_453Package.scala 27:4262
s1_tag_match_0


_T_454ICache.scala 116:26H2.
_T_455$R"


_T_443

s1_tag_match_0ICache.scala 126:2842
s1_tag_hit_0


_T_455ICache.scala 117:24H2.
_T_462$R"

io_invalidate	

0ICache.scala 122:17D2*
_T_464 R

io_s1_paddr
11
6ICache.scala 122:68=2'
_T_465R	

1


_T_464Cat.scala 30:58B2(
_T_466R


vb_array


_T_465ICache.scala 122:43>2$
_T_467R


_T_466
0
0ICache.scala 122:43>2$
_T_468R


_T_467
0
0ICache.scala 122:97@2&
_T_469R


_T_462


_T_468ICache.scala 122:32]2C
_T_4769R7(:&
:


tag_array_1	tag_rdatadata
19
0ICache.scala 125:32@2&
_T_477R


_T_476


s1_tagICache.scala 125:46J4
_T_479
	

clock"	

0*


_T_479Reg.scala 34:16Q27
_T_480-2+


s1_dout_valid


_T_477


_T_479Package.scala 27:4262
s1_tag_match_1


_T_480ICache.scala 116:26H2.
_T_481$R"


_T_469

s1_tag_match_1ICache.scala 126:2842
s1_tag_hit_1


_T_481ICache.scala 117:24L22
_T_539(R&

s1_tag_hit_0

s1_tag_hit_1ICache.scala 129:44H2.
_T_488$R"

io_invalidate	

0ICache.scala 122:17D2*
_T_490 R

io_s1_paddr
11
6ICache.scala 122:68=2'
_T_491R	

2


_T_490Cat.scala 30:58B2(
_T_492R


vb_array


_T_491ICache.scala 122:43>2$
_T_493R


_T_492
0
0ICache.scala 122:43>2$
_T_494R


_T_493
0
0ICache.scala 122:97@2&
_T_495R


_T_488


_T_494ICache.scala 122:32]2C
_T_5029R7(:&
:


tag_array_2	tag_rdatadata
19
0ICache.scala 125:32@2&
_T_503R


_T_502


s1_tagICache.scala 125:46J4
_T_505
	

clock"	

0*


_T_505Reg.scala 34:16Q27
_T_506-2+


s1_dout_valid


_T_503


_T_505Package.scala 27:4262
s1_tag_match_2


_T_506ICache.scala 116:26H2.
_T_507$R"


_T_495

s1_tag_match_2ICache.scala 126:2842
s1_tag_hit_2


_T_507ICache.scala 117:24F2,
_T_540"R 


_T_539

s1_tag_hit_2ICache.scala 129:44H2.
_T_514$R"

io_invalidate	

0ICache.scala 122:17D2*
_T_516 R

io_s1_paddr
11
6ICache.scala 122:68=2'
_T_517R	

3


_T_516Cat.scala 30:58B2(
_T_518R


vb_array


_T_517ICache.scala 122:43>2$
_T_519R


_T_518
0
0ICache.scala 122:43>2$
_T_520R


_T_519
0
0ICache.scala 122:97@2&
_T_521R


_T_514


_T_520ICache.scala 122:32]2C
_T_5289R7(:&
:


tag_array_3	tag_rdatadata
19
0ICache.scala 125:32@2&
_T_529R


_T_528


s1_tagICache.scala 125:46J4
_T_531
	

clock"	

0*


_T_531Reg.scala 34:16Q27
_T_532-2+


s1_dout_valid


_T_529


_T_531Package.scala 27:4262
s1_tag_match_3


_T_532ICache.scala 116:26H2.
_T_533$R"


_T_521

s1_tag_match_3ICache.scala 126:2842
s1_tag_hit_3


_T_533ICache.scala 117:24F2,
_T_541"R 


_T_540

s1_tag_hit_3ICache.scala 129:44>2(
_T_446R	

0	

0Ecc.scala 14:27J4
_T_448
	

clock"	

0*


_T_448Reg.scala 34:16Q27
_T_449-2+


s1_dout_valid


_T_446


_T_448Package.scala 27:42>2(
_T_458R	

0	

0Ecc.scala 14:27@2&
_T_459R


_T_449


_T_458ICache.scala 127:51@2&
_T_460R


_T_443


_T_459ICache.scala 127:3062
s1_disparity_0


_T_460ICache.scala 112:26>2(
_T_472R	

0	

0Ecc.scala 14:27J4
_T_474
	

clock"	

0*


_T_474Reg.scala 34:16Q27
_T_475-2+


s1_dout_valid


_T_472


_T_474Package.scala 27:42>2(
_T_484R	

0	

0Ecc.scala 14:27@2&
_T_485R


_T_475


_T_484ICache.scala 127:51@2&
_T_486R


_T_469


_T_485ICache.scala 127:3062
s1_disparity_1


_T_486ICache.scala 112:26P26
_T_542,R*

s1_disparity_0

s1_disparity_1ICache.scala 129:78>2(
_T_498R	

0	

0Ecc.scala 14:27J4
_T_500
	

clock"	

0*


_T_500Reg.scala 34:16Q27
_T_501-2+


s1_dout_valid


_T_498


_T_500Package.scala 27:42>2(
_T_510R	

0	

0Ecc.scala 14:27@2&
_T_511R


_T_501


_T_510ICache.scala 127:51@2&
_T_512R


_T_495


_T_511ICache.scala 127:3062
s1_disparity_2


_T_512ICache.scala 112:26H2.
_T_543$R"


_T_542

s1_disparity_2ICache.scala 129:78>2(
_T_524R	

0	

0Ecc.scala 14:27J4
_T_526
	

clock"	

0*


_T_526Reg.scala 34:16Q27
_T_527-2+


s1_dout_valid


_T_524


_T_526Package.scala 27:42>2(
_T_536R	

0	

0Ecc.scala 14:27@2&
_T_537R


_T_527


_T_536ICache.scala 127:51@2&
_T_538R


_T_521


_T_537ICache.scala 127:3062
s1_disparity_3


_T_538ICache.scala 112:26H2.
_T_544$R"


_T_543

s1_disparity_3ICache.scala 129:78A2'
_T_546R


_T_544	

0ICache.scala 129:52@2&
_T_547R


_T_541


_T_546ICache.scala 129:4952
s1_any_tag_hit


_T_547ICache.scala 72:28J21
s1_hit'R%

	out_valid

s1_any_tag_hitICache.scala 78:26H2/
_T_225%R#

s1_any_tag_hit	

0ICache.scala 79:30C2*
s1_missR

	out_valid


_T_225ICache.scala 79:27?2&
_T_226R	

state	

0ICache.scala 81:40E2,
_T_227"R 

io_req_valid


_T_226ICache.scala 81:31A2(
_T_228R

	out_valid	

stallICache.scala 81:67@2'
_T_230R


_T_228	

0ICache.scala 81:55A2(
s0_validR


_T_227


_T_230ICache.scala 81:52A2(
_T_231R

	out_valid	

stallICache.scala 84:37A2(
_T_232R


s0_valid


_T_231ICache.scala 84:24?2&
_T_233R	

state	

0ICache.scala 86:26@2'
_T_234R
	
s1_miss


_T_233ICache.scala 86:17S2:
_GEN_002.



_T_234

io_s1_paddr

refill_addrICache.scala 86:39H2/

refill_tag!R

refill_addr
31
12ICache.scala 89:31G2.

refill_idx R

refill_addr
11
6ICache.scala 90:31V2:
_T_2350R.

io_mem_0_d_ready

io_mem_0_d_validDecoupled.scala 30:37=2#
_T_237RR

255package.scala 19:64N24
_T_238*R(



_T_237

io_mem_0_d_bits_sizepackage.scala 19:71>2$
_T_239R


_T_238
7
0package.scala 19:7642
_T_240R


_T_239package.scala 19:4082
_T_241R	


_T_240
3Edges.scala 198:59L24
_T_242*R(

io_mem_0_d_bits_opcode
0
0Edges.scala 90:36J21
_T_244'2%



_T_242


_T_241	

0Edges.scala 199:14J4
_T_246
	

clock"	

0*


_T_246Reg.scala 26:44@2'
_T_248R


_T_246	

1Edges.scala 208:2832
_T_249R


_T_248Edges.scala 208:2882
_T_250R


_T_249
1Edges.scala 208:28@2'
_T_252R


_T_246	

0Edges.scala 209:25@2'
_T_254R


_T_246	

1Edges.scala 210:25@2'
_T_256R


_T_244	

0Edges.scala 210:47?2&
_T_257R


_T_254


_T_256Edges.scala 210:37D2+
refill_doneR


_T_257


_T_235Edges.scala 211:2232
_T_258R


_T_250Edges.scala 212:27C2*

refill_cntR


_T_244


_T_258Edges.scala 212:25I20
_T_259&2$



_T_252


_T_244


_T_250Edges.scala 214:21I20
_GEN_1&2$



_T_235


_T_259


_T_246Edges.scala 213:17K4
_T_262
	

clock"	

0*


_T_262LFSR.scala 22:19;2$
_T_263R


_T_262
0
0LFSR.scala 23:40;2$
_T_264R


_T_262
2
2LFSR.scala 23:48=2&
_T_265R


_T_263


_T_264LFSR.scala 23:43;2$
_T_266R


_T_262
3
3LFSR.scala 23:56=2&
_T_267R


_T_265


_T_266LFSR.scala 23:51;2$
_T_268R


_T_262
5
5LFSR.scala 23:64=2&
_T_269R


_T_267


_T_268LFSR.scala 23:59<2%
_T_270R


_T_262
15
1LFSR.scala 23:73<2&
_T_271R


_T_269


_T_270Cat.scala 30:58H21
_GEN_2'2%

	
s1_miss


_T_271


_T_262LFSR.scala 23:22?2&
repl_wayR


_T_262
1
0ICache.scala 95:56H2/
_T_282%R#

io_req_bits_addr
11
6ICache.scala 98:42E2,
_T_284"R 

refill_done	

0ICache.scala 98:70A2(
_T_285R


_T_284


s0_validICache.scala 98:83+2$
_GEN_3"



_T_285


_T_282
 2
_T_287


_GEN_3
 .2'
_T_289R


_T_287	

0
 +2$
_T_290R


_T_289
5
0
 922
_GEN_4(2&



_T_285	

1	

0
 +2$
_GEN_5"



_T_285


_T_290
 *2#
_GEN_6"



_T_285	

clock
 C2)
_T_312R


repl_way	

0ICache.scala 101:84C2)
_T_314R


repl_way	

1ICache.scala 101:84C2)
_T_316R


repl_way	

2ICache.scala 101:84C2)
_T_318R


repl_way	

3ICache.scala 101:8402
_T_321_0


_T_312ICache.scala 101:7442
_T_304_0


refill_tagICache.scala 101:48/2(
_GEN_7"



_T_321_0


_T_304_0
 ;24
_GEN_8*2(



_T_321_0	

1	

0
 02
_T_321_1


_T_314ICache.scala 101:7442
_T_304_1


refill_tagICache.scala 101:48/2(
_GEN_9"



_T_321_1


_T_304_1
 <25
_GEN_10*2(



_T_321_1	

1	

0
 02
_T_321_2


_T_316ICache.scala 101:7442
_T_304_2


refill_tagICache.scala 101:4802)
_GEN_11"



_T_321_2


_T_304_2
 <25
_GEN_12*2(



_T_321_2	

1	

0
 02
_T_321_3


_T_318ICache.scala 101:7442
_T_304_3


refill_tagICache.scala 101:4802)
_GEN_13"



_T_321_3


_T_304_3
 <25
_GEN_14*2(



_T_321_3	

1	

0
 G2.
_GEN_15#"!


refill_done


refill_idxICache.scala 99:22B2)
_GEN_16"


refill_done	

clockICache.scala 99:22Q28
_GEN_17-2+


refill_done	

1	

0ICache.scala 99:22C2*
_GEN_18"


refill_done


_GEN_8ICache.scala 99:22D2+
_GEN_19 "


refill_done
	
_GEN_10ICache.scala 99:22D2+
_GEN_20 "


refill_done
	
_GEN_12ICache.scala 99:22D2+
_GEN_21 "


refill_done
	
_GEN_14ICache.scala 99:22C2*
_GEN_22"


refill_done


_GEN_7ICache.scala 99:22C2*
_GEN_23"


refill_done


_GEN_9ICache.scala 99:22D2+
_GEN_24 "


refill_done
	
_GEN_11ICache.scala 99:22D2+
_GEN_25 "


refill_done
	
_GEN_13ICache.scala 99:22F2,
_T_342"R 

invalidated	

0ICache.scala 105:24E2+
_T_343!R

refill_done


_T_342ICache.scala 105:21B2,
_T_344"R 


repl_way


refill_idxCat.scala 30:58A2'
_T_347R
	

1


_T_344ICache.scala 106:32B2(
_T_348R


vb_array


_T_347ICache.scala 106:3262
_T_349R


vb_arrayICache.scala 106:32@2&
_T_350R


_T_349


_T_347ICache.scala 106:3242
_T_351R


_T_350ICache.scala 106:32K21
_T_352'2%
	

1


_T_348


_T_351ICache.scala 106:32M23
_GEN_26(2&



_T_343


_T_352


vb_arrayICache.scala 105:38T2:
_GEN_27/2-


io_invalidate	

0
	
_GEN_26ICache.scala 108:24X2>
_GEN_28321


io_invalidate	

1

invalidatedICache.scala 108:24J20
_T_364&R$


s1_valid

s1_disparity_0ICache.scala 114:20=2'
_T_366R	

0


s1_idxCat.scala 30:58A2'
_T_369R
	

1


_T_366ICache.scala 114:69B2(
_T_370R


vb_array


_T_369ICache.scala 114:6962
_T_371R


vb_arrayICache.scala 114:69@2&
_T_372R


_T_371


_T_369ICache.scala 114:6942
_T_373R


_T_372ICache.scala 114:69K21
_T_374'2%
	

0


_T_370


_T_373ICache.scala 114:69L22
_GEN_29'2%



_T_364


_T_374
	
_GEN_27ICache.scala 114:40J20
_T_375&R$


s1_valid

s1_disparity_1ICache.scala 114:20=2'
_T_377R	

1


s1_idxCat.scala 30:58A2'
_T_380R
	

1


_T_377ICache.scala 114:69B2(
_T_381R


vb_array


_T_380ICache.scala 114:6962
_T_382R


vb_arrayICache.scala 114:69@2&
_T_383R


_T_382


_T_380ICache.scala 114:6942
_T_384R


_T_383ICache.scala 114:69K21
_T_385'2%
	

0


_T_381


_T_384ICache.scala 114:69L22
_GEN_30'2%



_T_375


_T_385
	
_GEN_29ICache.scala 114:40J20
_T_386&R$


s1_valid

s1_disparity_2ICache.scala 114:20=2'
_T_388R	

2


s1_idxCat.scala 30:58A2'
_T_391R
	

1


_T_388ICache.scala 114:69B2(
_T_392R


vb_array


_T_391ICache.scala 114:6962
_T_393R


vb_arrayICache.scala 114:69@2&
_T_394R


_T_393


_T_391ICache.scala 114:6942
_T_395R


_T_394ICache.scala 114:69K21
_T_396'2%
	

0


_T_392


_T_395ICache.scala 114:69L22
_GEN_31'2%



_T_386


_T_396
	
_GEN_30ICache.scala 114:40J20
_T_397&R$


s1_valid

s1_disparity_3ICache.scala 114:20=2'
_T_399R	

3


s1_idxCat.scala 30:58A2'
_T_402R
	

1


_T_399ICache.scala 114:69B2(
_T_403R


vb_array


_T_402ICache.scala 114:6962
_T_404R


vb_arrayICache.scala 114:69@2&
_T_405R


_T_404


_T_402ICache.scala 114:6942
_T_406R


_T_405ICache.scala 114:69K21
_T_407'2%
	

0


_T_403


_T_406ICache.scala 114:69L22
_GEN_32'2%



_T_397


_T_407
	
_GEN_31ICache.scala 114:40N28
_GEN_33-2+


s1_dout_valid


_T_446


_T_448Reg.scala 35:19N28
_GEN_34-2+


s1_dout_valid


_T_451


_T_453Reg.scala 35:19N28
_GEN_35-2+


s1_dout_valid


_T_472


_T_474Reg.scala 35:19N28
_GEN_36-2+


s1_dout_valid


_T_477


_T_479Reg.scala 35:19N28
_GEN_37-2+


s1_dout_valid


_T_498


_T_500Reg.scala 35:19N28
_GEN_38-2+


s1_dout_valid


_T_503


_T_505Reg.scala 35:19N28
_GEN_39-2+


s1_dout_valid


_T_524


_T_526Reg.scala 35:19N28
_GEN_40-2+


s1_dout_valid


_T_529


_T_531Reg.scala 35:19C2)
_T_552R


repl_way	

0ICache.scala 133:42J20
_T_553&R$

io_mem_0_d_valid


_T_552ICache.scala 133:30=2#
_T_554R


refill_idx
3ICache.scala 136:36D2*
_T_555 R


_T_554


refill_cntICache.scala 136:63?2%
_GEN_41"



_T_553


_T_555ICache.scala 134:16>2$
_GEN_42"



_T_553	

clockICache.scala 134:16M23
_GEN_43(2&



_T_553	

1	

0ICache.scala 134:16@2&
_GEN_44"



_T_553	

1ICache.scala 134:16M23
_GEN_45("&



_T_553

io_mem_0_d_bits_dataICache.scala 134:16I2/
_T_557%R#

io_req_bits_addr
11
3ICache.scala 138:28A2'
_T_559R


_T_553	

0ICache.scala 139:45B2(
_T_560R


_T_559


s0_validICache.scala 139:50,2%
_GEN_46"



_T_560


_T_557
 2
_T_562
	
_GEN_46
 .2'
_T_564R


_T_562	

0	
 +2$
_T_565R


_T_564
8
0
 :23
_GEN_47(2&



_T_560	

1	

0
 ,2%
_GEN_48"



_T_560


_T_565
 +2$
_GEN_49"



_T_560	

clock
 J4
_T_568
@	

clock"	

0*


_T_568Reg.scala 34:16d2N
_GEN_50C2A


s1_dout_valid :
:



_T_550_T_566data


_T_568Reg.scala 35:19g2M
_T_569C2A


s1_dout_valid :
:



_T_550_T_566data


_T_568Package.scala 27:42C2)
_T_574R


repl_way	

1ICache.scala 133:42J20
_T_575&R$

io_mem_0_d_valid


_T_574ICache.scala 133:30=2#
_T_576R


refill_idx
3ICache.scala 136:36D2*
_T_577 R


_T_576


refill_cntICache.scala 136:63?2%
_GEN_51"



_T_575


_T_577ICache.scala 134:16>2$
_GEN_52"



_T_575	

clockICache.scala 134:16M23
_GEN_53(2&



_T_575	

1	

0ICache.scala 134:16@2&
_GEN_54"



_T_575	

1ICache.scala 134:16M23
_GEN_55("&



_T_575

io_mem_0_d_bits_dataICache.scala 134:16I2/
_T_579%R#

io_req_bits_addr
11
3ICache.scala 138:28A2'
_T_581R


_T_575	

0ICache.scala 139:45B2(
_T_582R


_T_581


s0_validICache.scala 139:50,2%
_GEN_56"



_T_582


_T_579
 2
_T_584
	
_GEN_56
 .2'
_T_586R


_T_584	

0	
 +2$
_T_587R


_T_586
8
0
 :23
_GEN_57(2&



_T_582	

1	

0
 ,2%
_GEN_58"



_T_582


_T_587
 +2$
_GEN_59"



_T_582	

clock
 J4
_T_590
@	

clock"	

0*


_T_590Reg.scala 34:16d2N
_GEN_60C2A


s1_dout_valid :
:



_T_572_T_588data


_T_590Reg.scala 35:19g2M
_T_591C2A


s1_dout_valid :
:



_T_572_T_588data


_T_590Package.scala 27:42C2)
_T_596R


repl_way	

2ICache.scala 133:42J20
_T_597&R$

io_mem_0_d_valid


_T_596ICache.scala 133:30=2#
_T_598R


refill_idx
3ICache.scala 136:36D2*
_T_599 R


_T_598


refill_cntICache.scala 136:63?2%
_GEN_61"



_T_597


_T_599ICache.scala 134:16>2$
_GEN_62"



_T_597	

clockICache.scala 134:16M23
_GEN_63(2&



_T_597	

1	

0ICache.scala 134:16@2&
_GEN_64"



_T_597	

1ICache.scala 134:16M23
_GEN_65("&



_T_597

io_mem_0_d_bits_dataICache.scala 134:16I2/
_T_601%R#

io_req_bits_addr
11
3ICache.scala 138:28A2'
_T_603R


_T_597	

0ICache.scala 139:45B2(
_T_604R


_T_603


s0_validICache.scala 139:50,2%
_GEN_66"



_T_604


_T_601
 2
_T_606
	
_GEN_66
 .2'
_T_608R


_T_606	

0	
 +2$
_T_609R


_T_608
8
0
 :23
_GEN_67(2&



_T_604	

1	

0
 ,2%
_GEN_68"



_T_604


_T_609
 +2$
_GEN_69"



_T_604	

clock
 J4
_T_612
@	

clock"	

0*


_T_612Reg.scala 34:16d2N
_GEN_70C2A


s1_dout_valid :
:



_T_594_T_610data


_T_612Reg.scala 35:19g2M
_T_613C2A


s1_dout_valid :
:



_T_594_T_610data


_T_612Package.scala 27:42C2)
_T_618R


repl_way	

3ICache.scala 133:42J20
_T_619&R$

io_mem_0_d_valid


_T_618ICache.scala 133:30=2#
_T_620R


refill_idx
3ICache.scala 136:36D2*
_T_621 R


_T_620


refill_cntICache.scala 136:63?2%
_GEN_71"



_T_619


_T_621ICache.scala 134:16>2$
_GEN_72"



_T_619	

clockICache.scala 134:16M23
_GEN_73(2&



_T_619	

1	

0ICache.scala 134:16@2&
_GEN_74"



_T_619	

1ICache.scala 134:16M23
_GEN_75("&



_T_619

io_mem_0_d_bits_dataICache.scala 134:16I2/
_T_623%R#

io_req_bits_addr
11
3ICache.scala 138:28A2'
_T_625R


_T_619	

0ICache.scala 139:45B2(
_T_626R


_T_625


s0_validICache.scala 139:50,2%
_GEN_76"



_T_626


_T_623
 2
_T_628
	
_GEN_76
 .2'
_T_630R


_T_628	

0	
 +2$
_T_631R


_T_630
8
0
 :23
_GEN_77(2&



_T_626	

1	

0
 ,2%
_GEN_78"



_T_626


_T_631
 +2$
_GEN_79"



_T_626	

clock
 J4
_T_634
@	

clock"	

0*


_T_634Reg.scala 34:16d2N
_GEN_80C2A


s1_dout_valid :
:



_T_616_T_632data


_T_634Reg.scala 35:19g2M
_T_635C2A


s1_dout_valid :
:



_T_616_T_632data


_T_634Package.scala 27:42@2&
_T_638R	

stall	

0ICache.scala 148:51J4
_T_639
	

clock"	

0*


_T_639Reg.scala 26:44G21
_GEN_81&2$



_T_638


s1_hit


_T_639Reg.scala 43:19@2&
_T_641R	

stall	

0ICache.scala 149:46N8
_T_654_0
	

clock"	

0*


_T_654_0Reg.scala 34:16N8
_T_654_1
	

clock"	

0*


_T_654_1Reg.scala 34:16N8
_T_654_2
	

clock"	

0*


_T_654_2Reg.scala 34:16N8
_T_654_3
	

clock"	

0*


_T_654_3Reg.scala 34:16O29
_GEN_82.2,



_T_641

s1_tag_hit_0


_T_654_0Reg.scala 35:19O29
_GEN_83.2,



_T_641

s1_tag_hit_1


_T_654_1Reg.scala 35:19O29
_GEN_84.2,



_T_641

s1_tag_hit_2


_T_654_2Reg.scala 35:19O29
_GEN_85.2,



_T_641

s1_tag_hit_3


_T_654_3Reg.scala 35:19@2&
_T_672R	

stall	

0ICache.scala 150:40N8
_T_685_0
@	

clock"	

0*


_T_685_0Reg.scala 34:16N8
_T_685_1
@	

clock"	

0*


_T_685_1Reg.scala 34:16N8
_T_685_2
@	

clock"	

0*


_T_685_2Reg.scala 34:16N8
_T_685_3
@	

clock"	

0*


_T_685_3Reg.scala 34:1612
	s1_dout_0


_T_569ICache.scala 118:21L26
_GEN_86+2)



_T_672

	s1_dout_0


_T_685_0Reg.scala 35:1912
	s1_dout_1


_T_591ICache.scala 118:21L26
_GEN_87+2)



_T_672

	s1_dout_1


_T_685_1Reg.scala 35:1912
	s1_dout_2


_T_613ICache.scala 118:21L26
_GEN_88+2)



_T_672

	s1_dout_2


_T_685_2Reg.scala 35:1912
	s1_dout_3


_T_635ICache.scala 118:21L26
_GEN_89+2)



_T_672

	s1_dout_3


_T_685_3Reg.scala 35:19K25
_T_703+2)



_T_654_0


_T_685_0	

0Mux.scala 19:72K25
_T_705+2)



_T_654_1


_T_685_1	

0Mux.scala 19:72K25
_T_707+2)



_T_654_2


_T_685_2	

0Mux.scala 19:72K25
_T_709+2)



_T_654_3


_T_685_3	

0Mux.scala 19:72<2&
_T_711R


_T_703


_T_705Mux.scala 19:72<2&
_T_712R


_T_711


_T_707Mux.scala 19:72<2&
_T_713R


_T_712


_T_709Mux.scala 19:72@2&
_T_716R	

state	

1ICache.scala 154:27E2+
_T_718!R


io_s2_kill	

0ICache.scala 154:44@2&
_T_719R


_T_716


_T_718ICache.scala 154:41>2$
_T_721R	

refill_addr
6ICache.scala 157:4692
_T_722R


_T_721
6ICache.scala 157:63E2(
_T_726R	

0	

6Parameters.scala 63:32E2(
_T_728R	

6	

6Parameters.scala 63:42C2&
_T_729R


_T_726


_T_728Parameters.scala 63:37E2'
_T_730R	

0


_T_729Parameters.scala 132:31E2'
_T_732R


_T_722	

0Parameters.scala 117:3182
_T_733R


_T_732Parameters.scala 117:49S25
_T_735+R)


_T_733R

	536870912Parameters.scala 117:5282
_T_736R


_T_735Parameters.scala 117:52K2-
_T_738#R!


_T_736R	

0Parameters.scala 117:67D2&
_T_739R


_T_730


_T_738Parameters.scala 132:56E2(
_T_742R	

0	

6Parameters.scala 63:32E2(
_T_744R	

6	

8Parameters.scala 63:42C2&
_T_745R


_T_742


_T_744Parameters.scala 63:37E2'
_T_746R	

0


_T_745Parameters.scala 132:31M2/
_T_748%R#


_T_722

	536870912Parameters.scala 117:3182
_T_749R


_T_748Parameters.scala 117:49S25
_T_751+R)


_T_749R

	536870912Parameters.scala 117:5282
_T_752R


_T_751Parameters.scala 117:52K2-
_T_754#R!


_T_752R	

0Parameters.scala 117:67D2&
_T_755R


_T_746


_T_754Parameters.scala 132:56E2'
_T_757R	

0


_T_739Parameters.scala 134:30D2&
_T_758R


_T_757


_T_755Parameters.scala 134:30A2(
_T_779R
	

1	

2OneHot.scala 49:12=2$
_T_780R


_T_779
2
0OneHot.scala 49:37B2(
_T_782R	

6	

3package.scala 41:21>2$
_T_784R


_T_780
2
2package.scala 44:26>2$
_T_785R


_T_722
2
2package.scala 45:26A2'
_T_787R


_T_785	

0package.scala 46:20A2'
_T_788R	

1


_T_787package.scala 49:27@2&
_T_789R


_T_784


_T_788package.scala 50:38@2&
_T_790R


_T_782


_T_789package.scala 50:29A2'
_T_791R	

1


_T_785package.scala 49:27@2&
_T_792R


_T_784


_T_791package.scala 50:38@2&
_T_793R


_T_782


_T_792package.scala 50:29>2$
_T_794R


_T_780
1
1package.scala 44:26>2$
_T_795R


_T_722
1
1package.scala 45:26A2'
_T_797R


_T_795	

0package.scala 46:20@2&
_T_798R


_T_788


_T_797package.scala 49:27@2&
_T_799R


_T_794


_T_798package.scala 50:38@2&
_T_800R


_T_790


_T_799package.scala 50:29@2&
_T_801R


_T_788


_T_795package.scala 49:27@2&
_T_802R


_T_794


_T_801package.scala 50:38@2&
_T_803R


_T_790


_T_802package.scala 50:29@2&
_T_804R


_T_791


_T_797package.scala 49:27@2&
_T_805R


_T_794


_T_804package.scala 50:38@2&
_T_806R


_T_793


_T_805package.scala 50:29@2&
_T_807R


_T_791


_T_795package.scala 49:27@2&
_T_808R


_T_794


_T_807package.scala 50:38@2&
_T_809R


_T_793


_T_808package.scala 50:29>2$
_T_810R


_T_780
0
0package.scala 44:26>2$
_T_811R


_T_722
0
0package.scala 45:26A2'
_T_813R


_T_811	

0package.scala 46:20@2&
_T_814R


_T_798


_T_813package.scala 49:27@2&
_T_815R


_T_810


_T_814package.scala 50:38@2&
_T_816R


_T_800


_T_815package.scala 50:29@2&
_T_817R


_T_798


_T_811package.scala 49:27@2&
_T_818R


_T_810


_T_817package.scala 50:38@2&
_T_819R


_T_800


_T_818package.scala 50:29@2&
_T_820R


_T_801


_T_813package.scala 49:27@2&
_T_821R


_T_810


_T_820package.scala 50:38@2&
_T_822R


_T_803


_T_821package.scala 50:29@2&
_T_823R


_T_801


_T_811package.scala 49:27@2&
_T_824R


_T_810


_T_823package.scala 50:38@2&
_T_825R


_T_803


_T_824package.scala 50:29@2&
_T_826R


_T_804


_T_813package.scala 49:27@2&
_T_827R


_T_810


_T_826package.scala 50:38@2&
_T_828R


_T_806


_T_827package.scala 50:29@2&
_T_829R


_T_804


_T_811package.scala 49:27@2&
_T_830R


_T_810


_T_829package.scala 50:38@2&
_T_831R


_T_806


_T_830package.scala 50:29@2&
_T_832R


_T_807


_T_813package.scala 49:27@2&
_T_833R


_T_810


_T_832package.scala 50:38@2&
_T_834R


_T_809


_T_833package.scala 50:29@2&
_T_835R


_T_807


_T_811package.scala 49:27@2&
_T_836R


_T_810


_T_835package.scala 50:38@2&
_T_837R


_T_809


_T_836package.scala 50:29<2&
_T_838R


_T_819


_T_816Cat.scala 30:58<2&
_T_839R


_T_825


_T_822Cat.scala 30:58<2&
_T_840R


_T_839


_T_838Cat.scala 30:58<2&
_T_841R


_T_831


_T_828Cat.scala 30:58<2&
_T_842R


_T_837


_T_834Cat.scala 30:58<2&
_T_843R


_T_842


_T_841Cat.scala 30:58<2&
_T_844R


_T_843


_T_840Cat.scala 30:58D2&
_T_848R	

0	

stateConditional.scala 29:28L22
_GEN_90'2%

	
s1_miss	

1	

stateICache.scala 165:22O21
_GEN_91&2$



_T_848
	
_GEN_90	

stateConditional.scala 29:59Q23
_GEN_92(2&



_T_848	

0
	
_GEN_28Conditional.scala 29:59D2&
_T_850R	

1	

stateConditional.scala 29:28W2=
_GEN_93220


io_mem_0_a_ready	

2
	
_GEN_91ICache.scala 169:29Q27
_GEN_94,2*



io_s2_kill	

0
	
_GEN_93ICache.scala 170:25Q23
_GEN_95(2&



_T_850
	
_GEN_94
	
_GEN_91Conditional.scala 29:59D2&
_T_851R	

2	

stateConditional.scala 29:28W2=
_GEN_96220


io_mem_0_d_valid	

3
	
_GEN_95ICache.scala 173:29Q23
_GEN_97(2&



_T_851
	
_GEN_96
	
_GEN_95Conditional.scala 29:59D2&
_T_852R	

3	

stateConditional.scala 29:28R28
_GEN_98-2+


refill_done	

0
	
_GEN_97ICache.scala 176:26Q23
_GEN_99(2&



_T_852
	
_GEN_98
	
_GEN_97Conditional.scala 29:59*2
_T_715


_T_713Mux.scala 19:7252
_T_767_opcode	

4Edges.scala 342:17?2&
_T_767_paramR	

0
3Edges.scala 342:17>2%
_T_767_sizeR	

6
4Edges.scala 342:1752
_T_767_source	

0Edges.scala 342:1752
_T_767_address


_T_722Edges.scala 342:1722
_T_767_mask


_T_844Edges.scala 342:17?2&
_T_767_dataR	

0
64Edges.scala 342:179z


io_resp_valid


_T_639ICache.scala 152:21


io_resp_bits_data
 Bz(


io_resp_bits_datablock


_T_715ICache.scala 151:30<z"


io_mem_0_a_valid


_T_719ICache.scala 154:18Iz/


io_mem_0_a_bits_opcode

_T_767_opcodeICache.scala 155:17Gz-


io_mem_0_a_bits_param

_T_767_paramICache.scala 155:17Ez+


io_mem_0_a_bits_size

_T_767_sizeICache.scala 155:17Iz/


io_mem_0_a_bits_source

_T_767_sourceICache.scala 155:17Kz1


io_mem_0_a_bits_address

_T_767_addressICache.scala 155:17Ez+


io_mem_0_a_bits_mask

_T_767_maskICache.scala 155:17Ez+


io_mem_0_a_bits_data

_T_767_dataICache.scala 155:17


io_mem_0_b_ready
 =z#


io_mem_0_c_valid	

0ICache.scala 159:18$


io_mem_0_c_bits_opcode
 #


io_mem_0_c_bits_param
 "


io_mem_0_c_bits_size
 $


io_mem_0_c_bits_source
 %


io_mem_0_c_bits_address
 "


io_mem_0_c_bits_data
 #


io_mem_0_c_bits_error
 <z#


io_mem_0_d_ready	

1ICache.scala 92:18=z#


io_mem_0_e_valid	

0ICache.scala 160:18"


io_mem_0_e_bits_sink
 Mz4
	

state'2%
	

reset	

0
	
_GEN_99ICache.scala 67:18%z


invalidated
	
_GEN_92
 $z


refill_addr


_GEN_0
 Oz6



s1_valid&2$
	

reset	

0


_T_232ICache.scala 74:21Jz4



_T_246&2$
	

reset	

0


_GEN_1Reg.scala 26:44Kz4



_T_262&2$
	

reset	

1


_GEN_2LFSR.scala 22:19=z6
(:&
:


tag_array_0	tag_rdataaddr


_GEN_5
 =z6
(:&
:


tag_array_1	tag_rdataaddr


_GEN_5
 =z6
(:&
:


tag_array_2	tag_rdataaddr


_GEN_5
 =z6
(:&
:


tag_array_3	tag_rdataaddr


_GEN_5
 ;z4
&:$
:


tag_array_0	tag_rdataen


_GEN_4
 ;z4
&:$
:


tag_array_1	tag_rdataen


_GEN_4
 ;z4
&:$
:


tag_array_2	tag_rdataen


_GEN_4
 ;z4
&:$
:


tag_array_3	tag_rdataen


_GEN_4
 <z5
':%
:


tag_array_0	tag_rdataclk


_GEN_6
 <z5
':%
:


tag_array_1	tag_rdataclk


_GEN_6
 <z5
':%
:


tag_array_2	tag_rdataclk


_GEN_6
 <z5
':%
:


tag_array_3	tag_rdataclk


_GEN_6
 ;z4
%:#
:


tag_array_0_T_328addr
	
_GEN_15
 ;z4
%:#
:


tag_array_1_T_328addr
	
_GEN_15
 ;z4
%:#
:


tag_array_2_T_328addr
	
_GEN_15
 ;z4
%:#
:


tag_array_3_T_328addr
	
_GEN_15
 9z2
#:!
:


tag_array_0_T_328en
	
_GEN_17
 9z2
#:!
:


tag_array_1_T_328en
	
_GEN_17
 9z2
#:!
:


tag_array_2_T_328en
	
_GEN_17
 9z2
#:!
:


tag_array_3_T_328en
	
_GEN_17
 :z3
$:"
:


tag_array_0_T_328clk
	
_GEN_16
 :z3
$:"
:


tag_array_1_T_328clk
	
_GEN_16
 :z3
$:"
:


tag_array_2_T_328clk
	
_GEN_16
 :z3
$:"
:


tag_array_3_T_328clk
	
_GEN_16
 ;z4
%:#
:


tag_array_0_T_328data
	
_GEN_22
 ;z4
%:#
:


tag_array_1_T_328data
	
_GEN_23
 ;z4
%:#
:


tag_array_2_T_328data
	
_GEN_24
 ;z4
%:#
:


tag_array_3_T_328data
	
_GEN_25
 ;z4
%:#
:


tag_array_0_T_328mask
	
_GEN_18
 ;z4
%:#
:


tag_array_1_T_328mask
	
_GEN_19
 ;z4
%:#
:


tag_array_2_T_328mask
	
_GEN_20
 ;z4
%:#
:


tag_array_3_T_328mask
	
_GEN_21
 Rz8



vb_array(2&
	

reset


0
	
_GEN_32ICache.scala 104:217z!


s1_dout_valid


s0_validReg.scala 14:44 z



_T_448
	
_GEN_33
  z



_T_453
	
_GEN_34
  z



_T_474
	
_GEN_35
  z



_T_479
	
_GEN_36
  z



_T_500
	
_GEN_37
  z



_T_505
	
_GEN_38
  z



_T_526
	
_GEN_39
  z



_T_531
	
_GEN_40
 6z/
 :
:



_T_550_T_566addr
	
_GEN_48
 4z-
:
:



_T_550_T_566en
	
_GEN_47
 5z.
:
:



_T_550_T_566clk
	
_GEN_49
 6z/
 :
:



_T_550_T_556addr
	
_GEN_41
 4z-
:
:



_T_550_T_556en
	
_GEN_43
 5z.
:
:



_T_550_T_556clk
	
_GEN_42
 6z/
 :
:



_T_550_T_556data
	
_GEN_45
 6z/
 :
:



_T_550_T_556mask
	
_GEN_44
  z



_T_568
	
_GEN_50
 6z/
 :
:



_T_572_T_588addr
	
_GEN_58
 4z-
:
:



_T_572_T_588en
	
_GEN_57
 5z.
:
:



_T_572_T_588clk
	
_GEN_59
 6z/
 :
:



_T_572_T_578addr
	
_GEN_51
 4z-
:
:



_T_572_T_578en
	
_GEN_53
 5z.
:
:



_T_572_T_578clk
	
_GEN_52
 6z/
 :
:



_T_572_T_578data
	
_GEN_55
 6z/
 :
:



_T_572_T_578mask
	
_GEN_54
  z



_T_590
	
_GEN_60
 6z/
 :
:



_T_594_T_610addr
	
_GEN_68
 4z-
:
:



_T_594_T_610en
	
_GEN_67
 5z.
:
:



_T_594_T_610clk
	
_GEN_69
 6z/
 :
:



_T_594_T_600addr
	
_GEN_61
 4z-
:
:



_T_594_T_600en
	
_GEN_63
 5z.
:
:



_T_594_T_600clk
	
_GEN_62
 6z/
 :
:



_T_594_T_600data
	
_GEN_65
 6z/
 :
:



_T_594_T_600mask
	
_GEN_64
  z



_T_612
	
_GEN_70
 6z/
 :
:



_T_616_T_632addr
	
_GEN_78
 4z-
:
:



_T_616_T_632en
	
_GEN_77
 5z.
:
:



_T_616_T_632clk
	
_GEN_79
 6z/
 :
:



_T_616_T_622addr
	
_GEN_71
 4z-
:
:



_T_616_T_622en
	
_GEN_73
 5z.
:
:



_T_616_T_622clk
	
_GEN_72
 6z/
 :
:



_T_616_T_622data
	
_GEN_75
 6z/
 :
:



_T_616_T_622mask
	
_GEN_74
  z



_T_634
	
_GEN_80
 Kz5



_T_639'2%
	

reset	

0
	
_GEN_81Reg.scala 26:44"z



_T_654_0
	
_GEN_82
 "z



_T_654_1
	
_GEN_83
 "z



_T_654_2
	
_GEN_84
 "z



_T_654_3
	
_GEN_85
 "z



_T_685_0
	
_GEN_86
 "z



_T_685_1
	
_GEN_87
 "z



_T_685_2
	
_GEN_88
 "z



_T_685_3
	
_GEN_89
 
ICache