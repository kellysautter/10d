yacc -v -V qstats.y -p qq -D tzvspmaa.h -o tzvmpyaa.c -l tzvspmaa.y

Rules:
    (0)  $accept:                     application $end
    (1)  application:                 operation_definition
    (2)  application:                 application operation_definition
    (3)  application:                 error
    (4)  $4:
    (5)  operation_definition:        operation_declaration parameter_definition_list operation_body $4 end_clause
    (6)  $6:
    (7)  operation_definition:        operation_declaration operation_body $6 end_clause
    (8)  operation_declaration:       operation_type qOPERATION symbol
    (9)  operation_declaration:       operation_type qOPERATION error
   (10)  operation_type:              qLOCAL
   (11)  operation_type:              qGLOBAL
   (12)  operation_type:              qDIALOG
   (13)  operation_type:              qDOMAIN
   (14)  $14:
   (15)  operation_type:              qOBJECT $14 qCONSTRAINT
   (16)  $16:
   (17)  operation_type:              qENTITY $16 qCONSTRAINT
   (18)  operation_type:              qTRANSFORMATION
   (19)  $19:
   (20)  operation_type:              qDERIVED $19 qATTRIBUTE
   (21)  operation_type:              qSHORTCUTKEY
   (22)  operation_type:              error
   (23)  $23:
   (24)  parameter_definition_list:   qLPAREN $23 parameter_declaration_list qRPAREN
   (25)  $25:
   (26)  parameter_definition_list:   qLPAREN $25 qRPAREN
   (27)  parameter_declaration_list:  parameter_declaration
   (28)  parameter_declaration_list:  parameter_declaration_list qCOMMA parameter_declaration
   (29)  $29:
   (30)  parameter_declaration_list:  parameter_declaration_list error $29 parameter_declaration
   (31)  parameter_declaration:       local_declaration
   (32)  parameter_declaration:       pointer_indicator local_declaration
   (33)  parameter_declaration:       view_declaration
   (34)  parameter_declaration:       pointer_indicator view_declaration
   (35)  operation_body:              declaration_list
   (36)  operation_body:              statement_list
   (37)  operation_body:              declaration_list statement_list
   (38)  operation_body:              statement_list declaration_list
   (39)  operation_body:
   (40)  end_clause:                  qEND
   (41)  declaration_list:            declaration
   (42)  declaration_list:            declaration_list declaration
   (43)  declaration:                 local_declaration
   (44)  declaration:                 view_declaration
   (45)  local_declaration:           data_type symbol
   (46)  local_declaration:           data_type error
   (47)  data_type:                   qTINTEGER
   (48)  data_type:                   qTDECIMAL
   (49)  data_type:                   qTSTRING dimension
   (50)  data_type:                   qTSTRING
   (51)  data_type:                   qTENTITYNAME
   (52)  data_type:                   qTATTRIBUTENAME
   (53)  data_type:                   qTSHORT
   (54)  data_type:                   qTCHARACTER
   (55)  pointer_indicator:           qRETURNED
   (56)  $56:
   (57)  dimension:                   qLPAREN qCONSTANT $56 qRPAREN
   (58)  $58:
   (59)  dimension:                   qLPAREN $58 qRPAREN
   (60)  view_declaration:            view_name view_type
   (61)  view_declaration:            view_name
   (62)  view_declaration:            view_name error
   (63)  view_name:                   qTVIEW symbol
   (64)  view_type:                   qBASED qON qLOD symbol
   (65)  view_type:                   qREGISTERED qAS symbol
   (66)  statement_list:              multiple_statements
   (67)  statement_list:              exit_statement
   (68)  statement_list:              multiple_statements exit_statement
   (69)  multiple_statements:         statement
   (70)  multiple_statements:         multiple_statements statement
   (71)  statement:                   conditional_statement
   (72)  statement:                   loop_statement
   (73)  statement:                   selection_statement
   (74)  statement:                   action_statement
   (75)  statement:                   assignment_statement
   (76)  conditional_statement:       if_phrase end_clause
   (77)  conditional_statement:       if_phrase else_phrase end_clause
   (78)  conditional_statement:       if_phrase error
   (79)  loop_statement:              loop_phrase until_phrase
   (80)  loop_statement:              loop_phrase statement_list until_phrase
   (81)  loop_statement:              loop_phrase while_clause end_clause
   (82)  loop_statement:              loop_phrase while_clause statement_list end_clause
   (83)  loop_statement:              loop_phrase while_clause error
   (84)  loop_statement:              loop_phrase while_clause statement_list error
   (85)  until_phrase:                qUNTIL compound_condition
   (86)  until_phrase:                qUNTIL multiple_condition
   (87)  until_phrase:                qUNTIL error
   (88)  selection_statement:         case_phrase of_list end_clause
   (89)  $89:
   (90)  selection_statement:         case_phrase otherwise $89 end_clause
   (91)  $91:
   (92)  selection_statement:         case_phrase of_list otherwise $91 end_clause
   (93)  selection_statement:         case_phrase of_list otherwise error
   (94)  selection_statement:         case_phrase of_list error
   (95)  selection_statement:         case_phrase otherwise error
   (96)  selection_statement:         case_phrase error
   (97)  $97:
   (98)  selection_statement:         case_phrase otherwise of_list $97 end_clause
   (99)  $99:
  (100)  selection_statement:         case_phrase $99 end_clause
  (101)  action_statement:            set_action
  (102)  action_statement:            for_action
  (103)  action_statement:            activate_action
  (104)  action_statement:            commit_action
  (105)  action_statement:            create_action
  (106)  action_statement:            delete_action
  (107)  action_statement:            get_action
  (108)  action_statement:            name_action
  (109)  action_statement:            include_action
  (110)  action_statement:            exclude_action
  (111)  assignment_statement:        operation_call
  (112)  $112:
  (113)  assignment_statement:        left_element qEQUAL $112 r_expression
  (114)  exit_statement:              return_value
  (115)  if_phrase:                   qIF compound_condition
  (116)  if_phrase:                   qIF multiple_condition
  (117)  $117:
  (118)  if_phrase:                   qIF compound_condition $117 statement_list
  (119)  $119:
  (120)  if_phrase:                   qIF multiple_condition $119 statement_list
  (121)  $121:
  (122)  if_phrase:                   qIF error $121 statement_list
  (123)  else_phrase:                 qELSE
  (124)  $124:
  (125)  else_phrase:                 qELSE $124 statement_list
  (126)  loop_phrase:                 qLOOP
  (127)  compound_condition:          condition
  (128)  compound_condition:          condition conditional_join compound_condition
  (129)  compound_condition:          condition conditional_join multiple_condition
  (130)  multiple_condition:          lparen compound_condition rparen
  (131)  multiple_condition:          lparen compound_condition rparen conditional_join multiple_condition
  (132)  multiple_condition:          lparen compound_condition rparen conditional_join compound_condition
  (133)  multiple_condition:          lparen multiple_condition rparen
  (134)  multiple_condition:          lparen multiple_condition rparen conditional_join multiple_condition
  (135)  multiple_condition:          lparen multiple_condition rparen conditional_join compound_condition
  (136)  multiple_condition:          negation_op multiple_condition
  (137)  while_clause:                while_keyword compound_condition
  (138)  while_clause:                while_keyword multiple_condition
  (139)  while_clause:                while_keyword error
  (140)  case_phrase:                 qCASE r_expression
  (141)  of_list:                     of_list of_clause statement_list
  (142)  of_list:                     of_list of_clause
  (143)  of_list:                     of_clause statement_list
  (144)  of_list:                     of_clause
  (145)  $145:
  (146)  otherwise:                   qOTHERWISE qCOLON $145 statement_list
  (147)  otherwise:                   qOTHERWISE qCOLON
  (148)  set_action:                  qSET qCURSOR fnpl_phrase cursor_target
  (149)  set_action:                  qSET qCURSOR fnpl_phrase cursor_target where_clause
  (150)  commit_action:               qCOMMIT symbol
  (151)  get_action:                  qGET qTVIEW view_var task_level qUSING symbol
  (152)  get_action:                  qGET qTVIEW view_var task_level
  (153)  get_action:                  qGET qTVIEW view_var
  (154)  name_action:                 qNAME qTVIEW name_var task_level qUSING symbol
  (155)  name_action:                 qNAME qTVIEW name_var task_level
  (156)  name_action:                 qNAME qTVIEW name_var
  (157)  $157:
  (158)  include_action:              qINCLUDE qENTITYQUALIFIER $157 source_eq position
  (159)  source_eq:                   qFROM qENTITYQUALIFIER
  (160)  position:                    qBEFORE
  (161)  position:                    qAFTER
  (162)  position:
  (163)  $163:
  (164)  exclude_action:              qEXCLUDE qENTITYQUALIFIER $163 exclude_position
  (165)  exclude_position:            qBEFORE
  (166)  exclude_position:            qNONE
  (167)  exclude_position:            qAFTER
  (168)  exclude_position:
  (169)  $169:
  (170)  name_var:                    symbol $169 qCONSTANT
  (171)  $171:
  (172)  name_var:                    symbol $171 symbol
  (173)  $173:
  (174)  view_var:                    symbol $173 qNAMED view_qual
  (175)  view_qual:                   qCONSTANT
  (176)  view_qual:                   symbol
  (177)  task_level:                  qAT qLEVEL qSUBTASK
  (178)  task_level:                  qAT qLEVEL qTASK
  (179)  task_level:                  qAT qLEVEL qAPPLICATION
  (180)  task_level:                  qAT qLEVEL qSYSTEM
  (181)  task_level:                  qAT qLEVEL qANY
  (182)  $182:
  (183)  for_action:                  for_entry statement_list $182 end_clause
  (184)  $184:
  (185)  for_action:                  for_entry $184 end_clause
  (186)  for_entry:                   qualified_for cursor_target where_clause
  (187)  for_entry:                   qualified_for cursor_target
  (188)  $188:
  (189)  qualified_for:               qFOR $188 qEACH
  (190)  $190:
  (191)  qualified_for:               qFOR $190 symbol
  (192)  $192:
  (193)  qualified_for:               qFOR $192 qCONSTANT
  (194)  qualified_for:               qFOR
  (195)  $195:
  (196)  cursor_target:               qENTITYQUALIFIER $195 scope
  (197)  cursor_target:               qENTITYQUALIFIER
  (198)  scope:                     qWITHIN qENTITYQUALIFIER
  (199)  scope:                     qWITHIN qOBJECT
  (200)  $200:
  (201)  activate_action:             activate_phrase where_clause $200 restricting_phrase
  (202)  $202:
  (203)  activate_action:             activate_phrase where_clause $202
  (204)  activate_action:             activate_phrase restricting_phrase
  (205)  activate_action:             activate_phrase qEMPTY
  (206)  activate_action:             activate_phrase
  (207)  $207:
  (208)  activate_phrase:             qACTIVATE symbol $207 zdefine
  (209)  $209:
  (210)  activate_phrase:             qACTIVATE symbol $209
  (211)  zdefine:                     qSINGLE
  (212)  zdefine:                     qMULTIPLE
  (213)  zdefine:                     qROOTONLY
  (214)  zdefine:                     qROOTONLYMULTIPLE
  (215)  zdefine:                     qCONTINUE
  (216)  zdefine:                     qROOTONLYCONTINUE
  (217)  zdefine:                     qSINGLEFORUPDATE
  (218)  restricting_phrase:          restricting_phrase restricting_clause
  (219)  restricting_phrase:          restricting_clause
  (220)  $220:
  (221)  restricting_clause:          qRESTRICTING qENTITYQUALIFIER $220 qTO attribute_where_phrase
  (222)  $222:
  (223)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $222 qAFTER
  (224)  $224:
  (225)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $224 qBEFORE
  (226)  $226:
  (227)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $226 qFIRST
  (228)  $228:
  (229)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $228 qLAST
  (230)  create_action:               qCREATE qENTITY qENTITYQUALIFIER
  (231)  $231:
  (232)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $231 qFIRST
  (233)  $233:
  (234)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $233 qLAST
  (235)  $235:
  (236)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $235 qPREVIOUS
  (237)  $237:
  (238)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $237 qNEXT
  (239)  $239:
  (240)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $239 qNONE
  (241)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER
  (242)  $242:
  (243)  operation_call:              symbol $242 parameter_value_list
  (244)  $244:
  (245)  left_element:                symbol $244 offset
  (246)  left_element:                symbol
  (247)  left_element:                qATTRIBUTEQUALIFIER
  (248)  $248:
  (249)  r_expression:                r_expression qADDITION $248 r_expression
  (250)  $250:
  (251)  r_expression:                r_expression qMINUS $250 r_expression
  (252)  $252:
  (253)  r_expression:                r_expression qMULTIPLICATION $252 r_expression
  (254)  $254:
  (255)  r_expression:                r_expression qDIVISION $254 r_expression
  (256)  r_expression:                lparen r_expression rparen
  (257)  $257:
  (258)  r_expression:                qMINUS $257 r_expression
  (259)  r_expression:                simple_expression
  (260)  return_value:                qRETURN r_expression
  (261)  return_value:                qRETURN
  (262)  $262:
  (263)  condition:                   r_expression qCONDOPERATOR $262 r_expression
  (264)  $264:
  (265)  condition:                   r_expression qEQUAL $264 r_expression
  (266)  condition:                   exist_condition
  (267)  condition:                   r_expression qCONDOPERATOR error
  (268)  $268:
  (269)  exist_condition:             qENTITYQUALIFIER $268 exist_phrase
  (270)  conditional_join:            qJOIN
  (271)  lparen:                     qLPAREN
  (272)  rparen:                     qRPAREN
  (273)  negation_op:                 qNOT
  (274)  while_keyword:               qWHILE
  (275)  $275:
  (276)  of_clause:                   qOF aconstant $275 qCOLON
  (277)  $277:
  (278)  of_clause:                   qOF qDEFINE $277 qCOLON
  (279)  fnpl_phrase:                 qFIRST
  (280)  fnpl_phrase:                 qNEXT
  (281)  fnpl_phrase:                 qPREVIOUS
  (282)  fnpl_phrase:                 qLAST
  (283)  $283:
  (284)  where_clause:                qWHERE $283 attribute_where_phrase
  (285)  $285:
  (286)  attribute_where_phrase:      attribute_where_phrase qJOIN $285 attribute_where_phrase
  (287)  $287:
  (288)  attribute_where_phrase:      qLPAREN $287 attribute_where_phrase qRPAREN
  (289)  attribute_where_phrase:      attribute_op_phrase
  (290)  $290:
  (291)  attribute_op_phrase:         qATTRIBUTEQUALIFIER $290 where_op where_value
  (292)  $292:
  (293)  attribute_op_phrase:         qATTRIBUTEQUALIFIER $292 is_null_phrase
  (294)  $294:
  (295)  attribute_op_phrase:         qENTITYQUALIFIER $294 exist_phrase
  (296)  $296:
  (297)  attribute_op_phrase:         qENTITYQUALIFIER $296 is_keyword qENTITYQUALIFIER
  (298)  where_op:                    qCONDOPERATOR
  (299)  where_op:                    qEQUAL
  (300)  where_op:                    qLIKE
  (301)  exist_phrase:                qEXISTS
  (302)  exist_phrase:                qDOES qNOT qEXIST
  (303)  $303:
  (304)  is_null_phrase:              is_keyword qNOT $303 qNULL
  (305)  is_null_phrase:              is_keyword qNULL
  (306)  is_keyword:                  qIS
  (307)  where_value:                 symbol
  (308)  where_value:                 aconstant
  (309)  where_value:                 qATTRIBUTEQUALIFIER
  (310)  $310:
  (311)  aconstant:                   qMINUS $310 qCONSTANT
  (312)  aconstant:                   qCONSTANT
  (313)  $313:
  (314)  parameter_value_list:        openoper $313 value_list closeoper
  (315)  $315:
  (316)  parameter_value_list:        openoper $315 closeoper
  (317)  openoper:                    qLPAREN
  (318)  closeoper:                   qRPAREN
  (319)  simple_expression:           value
  (320)  simple_expression:           operation_call
  (321)  value:                     symbol
  (322)  $322:
  (323)  value:                     symbol $322 offset
  (324)  value:                     qDEFINE
  (325)  value:                     qATTRIBUTEQUALIFIER
  (326)  value:                     qCONSTANT
  (327)  value:                     error
  (328)  value_list:                  r_expression
  (329)  $329:
  (330)  value_list:                  r_expression qCOMMA $329 value_list
  (331)  $331:
  (332)  offset:                     qLBRACE $331 index_and_length qRBRACE
  (333)  $333:
  (334)  index_and_length:            r_expression qCOLON $333 r_expression
  (335)  index_and_length:            r_expression
  (336)  symbol:                     qVARIABLE

State 0
         $accept:                    .application $end

   error               shift 12
   qENTITY             shift 4
   qDERIVED            shift 2
   qTRANSFORMATION     shift 3
   qSHORTCUTKEY        shift 1
   qGLOBAL             shift 8
   qLOCAL              shift 9
   qOBJECT             shift 5
   qDIALOG             shift 7
   qDOMAIN             shift 6
   .                   error

   operation_type             goto 10
   operation_declaration      goto 11
   operation_definition       goto 13
   application                goto 14

State 1
   (21)  operation_type:              qSHORTCUTKEY.

   .                   reduce (21)

State 2
   (19)  $19:                    .
         operation_type:              qDERIVED.$19 qATTRIBUTE

   .                   reduce (19)

   $19                    goto 15

State 3
   (18)  operation_type:              qTRANSFORMATION.

   .                   reduce (18)

State 4
   (16)  $16:                    .
         operation_type:              qENTITY.$16 qCONSTRAINT

   .                   reduce (16)

   $16                    goto 16

State 5
   (14)  $14:                    .
         operation_type:              qOBJECT.$14 qCONSTRAINT

   .                   reduce (14)

   $14                    goto 17

State 6
   (13)  operation_type:              qDOMAIN.

   .                   reduce (13)

State 7
   (12)  operation_type:              qDIALOG.

   .                   reduce (12)

State 8
   (11)  operation_type:              qGLOBAL.

   .                   reduce (11)

State 9
   (10)  operation_type:              qLOCAL.

   .                   reduce (10)

State 10
         operation_declaration:       operation_type.qOPERATION symbol
         operation_declaration:       operation_type.qOPERATION error

   qOPERATION          shift 18
   .                   error


State 11
         operation_definition:        operation_declaration.parameter_definition_list operation_body $4 end_clause
         operation_definition:        operation_declaration.operation_body $6 end_clause
   (39)  operation_body:             .   [ qEND ]

   qACTIVATE           shift 25
   qTATTRIBUTENAME     shift 67
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qEND                reduce (39)
   qTENTITYNAME        shift 68
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qVARIABLE           shift 19
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   qLPAREN             shift 78
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62
   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 73
   local_declaration          goto 74
   declaration                goto 75
   statement_list             goto 76
   declaration_list           goto 77
   operation_body             goto 79
   parameter_definition_list  goto 80

State 12
    (3)  ITEM and lookahead TOO BIG
   (22)  operation_type:              error.   [ qOPERATION ]

   qOPERATION          reduce (22)
   .                   reduce (3)

State 13
    (1)  application:                 operation_definition.

   .                   reduce (1)

State 14
         $accept:                     application.$end
         application:                 application.operation_definition

   $end                accept
   error               shift 81
   qENTITY             shift 4
   qDERIVED            shift 2
   qTRANSFORMATION     shift 3
   qSHORTCUTKEY        shift 1
   qGLOBAL             shift 8
   qLOCAL              shift 9
   qOBJECT             shift 5
   qDIALOG             shift 7
   qDOMAIN             shift 6
   .                   error

   operation_type             goto 10
   operation_declaration      goto 11
   operation_definition       goto 82

State 15
         operation_type:              qDERIVED $19.qATTRIBUTE

   qATTRIBUTE          shift 83
   .                   error


State 16
         operation_type:              qENTITY $16.qCONSTRAINT

   qCONSTRAINT         shift 84
   .                   error


State 17
         operation_type:              qOBJECT $14.qCONSTRAINT

   qCONSTRAINT         shift 85
   .                   error


State 18
         operation_declaration:       operation_type qOPERATION.symbol
         operation_declaration:       operation_type qOPERATION.error

   error               shift 86
   qVARIABLE           shift 19
   .                   error

   symbol                    goto 87

State 19
  (336)  symbol:                     qVARIABLE.

   .                   reduce (336)

State 20
         return_value:                qRETURN.r_expression
  (261)  ITEM and lookahead TOO BIG

   error               shift 88
   qTATTRIBUTENAME     reduce (261)
   qCONSTANT           shift 89
   qELSE               reduce (261)
   qEND                reduce (261)
   qTENTITYNAME        reduce (261)
   qOF                 reduce (261)
   qTDECIMAL           reduce (261)
   qTINTEGER           reduce (261)
   qTSHORT             reduce (261)
   qTSTRING            reduce (261)
   qUNTIL              reduce (261)
   qVARIABLE           shift 19
   qTVIEW              reduce (261)
   qOTHERWISE          reduce (261)
   qTCHARACTER         reduce (261)
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 98
   symbol                    goto 99

State 21
  (247)  left_element:                qATTRIBUTEQUALIFIER.

   .                   reduce (247)

State 22
  (242)  $242:                    .   [ qLPAREN ]
         operation_call:              symbol.$242 parameter_value_list
  (244)  $244:                    .   [ qLBRACE ]
         left_element:                symbol.$244 offset
  (246)  left_element:                symbol.   [ qEQUAL ]

   qEQUAL              reduce (246)
   qLPAREN             reduce (242)
   qLBRACE             reduce (244)
   .                   error

   $244                    goto 100
   $242                    goto 101

State 23
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER $231 qFIRST
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER $233 qLAST
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER $235 qPREVIOUS
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER $237 qNEXT
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER $239 qNONE
         delete_action:               qDELETE.qENTITY qENTITYQUALIFIER

   qENTITY             shift 102
   .                   error


State 24
         create_action:               qCREATE.qENTITY qENTITYQUALIFIER $222 qAFTER
         create_action:               qCREATE.qENTITY qENTITYQUALIFIER $224 qBEFORE
         create_action:               qCREATE.qENTITY qENTITYQUALIFIER $226 qFIRST
         create_action:               qCREATE.qENTITY qENTITYQUALIFIER $228 qLAST
         create_action:               qCREATE.qENTITY qENTITYQUALIFIER

   qENTITY             shift 103
   .                   error


State 25
         activate_phrase:             qACTIVATE.symbol $207 zdefine
         activate_phrase:             qACTIVATE.symbol $209

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 104

State 26
         activate_action:             activate_phrase.where_clause $200 restricting_phrase
         activate_action:             activate_phrase.where_clause $202
         activate_action:             activate_phrase.restricting_phrase
         activate_action:             activate_phrase.qEMPTY
  (206)  ITEM and lookahead TOO BIG

   qRESTRICTING        shift 106
   qWHERE              shift 105
   qEMPTY              shift 108
   .                   reduce (206)

   restricting_clause         goto 107
   restricting_phrase         goto 109
   where_clause               goto 110

State 27
  (188)  $188:                    .   [ qEACH ]
         qualified_for:               qFOR.$188 qEACH
  (190)  $190:                    .   [ qVARIABLE ]
         qualified_for:               qFOR.$190 symbol
  (192)  $192:                    .   [ qCONSTANT ]
         qualified_for:               qFOR.$192 qCONSTANT
  (194)  qualified_for:               qFOR.   [ qENTITYQUALIFIER ]

   qEACH               reduce (188)
   qCONSTANT           reduce (192)
   qVARIABLE           reduce (190)
   qENTITYQUALIFIER    reduce (194)
   .                   error

   $192                    goto 111
   $190                    goto 112
   $188                    goto 113

State 28
         for_entry:                   qualified_for.cursor_target where_clause
         for_entry:                   qualified_for.cursor_target

   qENTITYQUALIFIER    shift 114
   .                   error

   cursor_target              goto 115

State 29
         for_action:                  for_entry.statement_list $182 end_clause
  (184)  $184:                    .   [ qEND ]
         for_action:                  for_entry.$184 end_clause

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qEND                reduce (184)
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   $184                    goto 116
   statement_list             goto 117
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 30
         exclude_action:              qEXCLUDE.qENTITYQUALIFIER $163 exclude_position

   qENTITYQUALIFIER    shift 118
   .                   error


State 31
         include_action:              qINCLUDE.qENTITYQUALIFIER $157 source_eq position

   qENTITYQUALIFIER    shift 119
   .                   error


State 32
         name_action:                 qNAME.qTVIEW name_var task_level qUSING symbol
         name_action:                 qNAME.qTVIEW name_var task_level
         name_action:                 qNAME.qTVIEW name_var

   qTVIEW              shift 120
   .                   error


State 33
         get_action:                  qGET.qTVIEW view_var task_level qUSING symbol
         get_action:                  qGET.qTVIEW view_var task_level
         get_action:                  qGET.qTVIEW view_var

   qTVIEW              shift 121
   .                   error


State 34
         commit_action:               qCOMMIT.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 122

State 35
         set_action:                  qSET.qCURSOR fnpl_phrase cursor_target
         set_action:                  qSET.qCURSOR fnpl_phrase cursor_target where_clause

   qCURSOR             shift 123
   .                   error


State 36
         case_phrase:                 qCASE.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   symbol                    goto 99
   r_expression               goto 124

State 37
  (126)  loop_phrase:                 qLOOP.

   .                   reduce (126)

State 38
         if_phrase:                   qIF.compound_condition
         if_phrase:                   qIF.multiple_condition
         if_phrase:                   qIF.compound_condition $117 statement_list
         if_phrase:                   qIF.multiple_condition $119 statement_list
         if_phrase:                   qIF.error $121 statement_list

   error               shift 132
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   lparen                    goto 130
   condition                  goto 131
   multiple_condition         goto 133
   compound_condition         goto 134

State 39
  (114)  exit_statement:              return_value.

   .                   reduce (114)

State 40
         assignment_statement:        left_element.qEQUAL $112 r_expression

   qEQUAL              shift 135
   .                   error


State 41
  (111)  assignment_statement:        operation_call.

   .                   reduce (111)

State 42
  (110)  action_statement:            exclude_action.

   .                   reduce (110)

State 43
  (109)  action_statement:            include_action.

   .                   reduce (109)

State 44
  (108)  action_statement:            name_action.

   .                   reduce (108)

State 45
  (107)  action_statement:            get_action.

   .                   reduce (107)

State 46
  (106)  action_statement:            delete_action.

   .                   reduce (106)

State 47
  (105)  action_statement:            create_action.

   .                   reduce (105)

State 48
  (104)  action_statement:            commit_action.

   .                   reduce (104)

State 49
  (103)  action_statement:            activate_action.

   .                   reduce (103)

State 50
  (102)  action_statement:            for_action.

   .                   reduce (102)

State 51
  (101)  action_statement:            set_action.

   .                   reduce (101)

State 52
         selection_statement:         case_phrase.of_list end_clause
         selection_statement:         case_phrase.otherwise $89 end_clause
         selection_statement:         case_phrase.of_list otherwise $91 end_clause
         selection_statement:         case_phrase.of_list otherwise error
         selection_statement:         case_phrase.of_list error
         selection_statement:         case_phrase.otherwise error
         selection_statement:         case_phrase.error
         selection_statement:         case_phrase.otherwise of_list $97 end_clause
   (99)  $99:                    .   [ qEND ]
         selection_statement:         case_phrase.$99 end_clause

   error               shift 140
   qEND                reduce (99)
   qOF                 shift 136
   qOTHERWISE          shift 137
   .                   error

   of_clause                  goto 138
   $99                    goto 139
   otherwise                  goto 141
   of_list                    goto 142

State 53
         loop_statement:              loop_phrase.until_phrase
         loop_statement:              loop_phrase.statement_list until_phrase
         loop_statement:              loop_phrase.while_clause end_clause
         loop_statement:              loop_phrase.while_clause statement_list end_clause
         loop_statement:              loop_phrase.while_clause error
         loop_statement:              loop_phrase.while_clause statement_list error

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qUNTIL              shift 145
   qVARIABLE           shift 19
   qWHILE              shift 143
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   while_keyword              goto 144
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   while_clause               goto 146
   statement_list             goto 147
   until_phrase               goto 148
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 54
         conditional_statement:       if_phrase.end_clause
         conditional_statement:       if_phrase.else_phrase end_clause
         conditional_statement:       if_phrase.error

   error               shift 150
   qELSE               shift 149
   qEND                shift 153
   .                   error

   else_phrase                goto 151
   end_clause                 goto 152

State 55
   (75)  statement:                   assignment_statement.

   .                   reduce (75)

State 56
   (74)  statement:                   action_statement.

   .                   reduce (74)

State 57
   (73)  statement:                   selection_statement.

   .                   reduce (73)

State 58
   (72)  statement:                   loop_statement.

   .                   reduce (72)

State 59
   (71)  statement:                   conditional_statement.

   .                   reduce (71)

State 60
   (69)  multiple_statements:         statement.

   .                   reduce (69)

State 61
   (67)  statement_list:              exit_statement.

   .                   reduce (67)

State 62
   (66)  ITEM and lookahead TOO BIG
         statement_list:              multiple_statements.exit_statement
         multiple_statements:         multiple_statements.statement

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   reduce (66)

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 154
   exit_statement             goto 155

State 63
         view_name:                   qTVIEW.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 156

State 64
         view_declaration:            view_name.view_type
   (61)  ITEM and lookahead TOO BIG
         view_declaration:            view_name.error

   error               shift 159
   qACTIVATE           reduce (61)
   qTATTRIBUTENAME     reduce (61)
   qBASED              shift 158
   qCASE               reduce (61)
   qCREATE             reduce (61)
   qCOMMIT             reduce (61)
   qDELETE             reduce (61)
   qEND                reduce (61)
   qTENTITYNAME        reduce (61)
   qFOR                reduce (61)
   qIF                 reduce (61)
   qLOOP               reduce (61)
   qRETURN             reduce (61)
   qSET                reduce (61)
   qTDECIMAL           reduce (61)
   qTINTEGER           reduce (61)
   qTSHORT             reduce (61)
   qTSTRING            reduce (61)
   qVARIABLE           reduce (61)
   qTVIEW              reduce (61)
   qTCHARACTER         reduce (61)
   qREGISTERED         shift 157
   qGET                reduce (61)
   qNAME               reduce (61)
   qINCLUDE            reduce (61)
   qEXCLUDE            reduce (61)
   qATTRIBUTEQUALIFIER reduce (61)
   qCOMMA              reduce (61)
   qRPAREN             reduce (61)
   .                   error

   view_type                  goto 160

State 65
   (54)  data_type:                   qTCHARACTER.

   .                   reduce (54)

State 66
   (53)  data_type:                   qTSHORT.

   .                   reduce (53)

State 67
   (52)  data_type:                   qTATTRIBUTENAME.

   .                   reduce (52)

State 68
   (51)  data_type:                   qTENTITYNAME.

   .                   reduce (51)

State 69
         data_type:                   qTSTRING.dimension
   (50)  data_type:                   qTSTRING.   [ error qVARIABLE ]

   qLPAREN             shift 161
   .                   reduce (50)

   dimension                  goto 162

State 70
   (48)  data_type:                   qTDECIMAL.

   .                   reduce (48)

State 71
   (47)  data_type:                   qTINTEGER.

   .                   reduce (47)

State 72
         local_declaration:           data_type.symbol
         local_declaration:           data_type.error

   error               shift 163
   qVARIABLE           shift 19
   .                   error

   symbol                    goto 164

State 73
   (44)  declaration:                 view_declaration.

   .                   reduce (44)

State 74
   (43)  declaration:                 local_declaration.

   .                   reduce (43)

State 75
   (41)  declaration_list:            declaration.

   .                   reduce (41)

State 76
   (36)  operation_body:              statement_list.   [ qEND ]
         operation_body:              statement_list.declaration_list

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   .                   reduce (36)

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 73
   local_declaration          goto 74
   declaration                goto 75
   declaration_list           goto 165

State 77
   (35)  operation_body:              declaration_list.   [ qEND ]
         operation_body:              declaration_list.statement_list
         declaration_list:            declaration_list.declaration

   qACTIVATE           shift 25
   qTATTRIBUTENAME     shift 67
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qTENTITYNAME        shift 68
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qVARIABLE           shift 19
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   reduce (35)

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62
   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 73
   local_declaration          goto 74
   declaration                goto 166
   statement_list             goto 167

State 78
   (23)  ITEM and lookahead TOO BIG
         parameter_definition_list:   qLPAREN.$23 parameter_declaration_list qRPAREN
   (25)  $25:                    .   [ qRPAREN ]
         parameter_definition_list:   qLPAREN.$25 qRPAREN

   qRPAREN             reduce (25)
   .                   reduce (23)

   $25                    goto 168
   $23                    goto 169

State 79
    (6)  $6:                    .
         operation_definition:        operation_declaration operation_body.$6 end_clause

   .                   reduce (6)

   $6                    goto 170

State 80
         operation_definition:        operation_declaration parameter_definition_list.operation_body $4 end_clause
   (39)  operation_body:             .   [ qEND ]

   qACTIVATE           shift 25
   qTATTRIBUTENAME     shift 67
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qEND                reduce (39)
   qTENTITYNAME        shift 68
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qVARIABLE           shift 19
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62
   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 73
   local_declaration          goto 74
   declaration                goto 75
   statement_list             goto 76
   declaration_list           goto 77
   operation_body             goto 171

State 81
   (22)  operation_type:              error.

   .                   reduce (22)

State 82
    (2)  application:                 application operation_definition.

   .                   reduce (2)

State 83
   (20)  operation_type:              qDERIVED $19 qATTRIBUTE.

   .                   reduce (20)

State 84
   (17)  operation_type:              qENTITY $16 qCONSTRAINT.

   .                   reduce (17)

State 85
   (15)  operation_type:              qOBJECT $14 qCONSTRAINT.

   .                   reduce (15)

State 86
    (9)  operation_declaration:       operation_type qOPERATION error.

   .                   reduce (9)

State 87
    (8)  operation_declaration:       operation_type qOPERATION symbol.

   .                   reduce (8)

State 88
  (327)  value:                     error.

   .                   reduce (327)

State 89
  (326)  value:                     qCONSTANT.

   .                   reduce (326)

State 90
  (325)  value:                     qATTRIBUTEQUALIFIER.

   .                   reduce (325)

State 91
  (324)  value:                     qDEFINE.

   .                   reduce (324)

State 92
  (320)  simple_expression:           operation_call.

   .                   reduce (320)

State 93
  (319)  simple_expression:           value.

   .                   reduce (319)

State 94
  (271)  lparen:                     qLPAREN.

   .                   reduce (271)

State 95
  (259)  r_expression:                simple_expression.

   .                   reduce (259)

State 96
  (257)  $257:                    .
         r_expression:                qMINUS.$257 r_expression

   .                   reduce (257)

   $257                    goto 172

State 97
         r_expression:                lparen.r_expression rparen

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 173
   symbol                    goto 99

State 98
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (260)  ITEM and lookahead TOO BIG

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (260)


State 99
  (242)  $242:                    .   [ qLPAREN ]
         operation_call:              symbol.$242 parameter_value_list
  (321)  ITEM and lookahead TOO BIG
  (322)  $322:                    .   [ qLBRACE ]
         value:                     symbol.$322 offset

   qLPAREN             reduce (242)
   qLBRACE             reduce (322)
   .                   reduce (321)

   $322                    goto 178
   $242                    goto 101

State 100
         left_element:                symbol $244.offset

   qLBRACE             shift 179
   .                   error

   offset                    goto 180

State 101
         operation_call:              symbol $242.parameter_value_list

   qLPAREN             shift 181
   .                   error

   openoper                   goto 182
   parameter_value_list       goto 183

State 102
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER $231 qFIRST
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER $233 qLAST
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER $235 qPREVIOUS
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER $237 qNEXT
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER $239 qNONE
         delete_action:               qDELETE qENTITY.qENTITYQUALIFIER

   qENTITYQUALIFIER    shift 184
   .                   error


State 103
         create_action:               qCREATE qENTITY.qENTITYQUALIFIER $222 qAFTER
         create_action:               qCREATE qENTITY.qENTITYQUALIFIER $224 qBEFORE
         create_action:               qCREATE qENTITY.qENTITYQUALIFIER $226 qFIRST
         create_action:               qCREATE qENTITY.qENTITYQUALIFIER $228 qLAST
         create_action:               qCREATE qENTITY.qENTITYQUALIFIER

   qENTITYQUALIFIER    shift 185
   .                   error


State 104
  (207)  ITEM and lookahead TOO BIG
         activate_phrase:             qACTIVATE symbol.$207 zdefine
  (209)  ITEM and lookahead TOO BIG
         activate_phrase:             qACTIVATE symbol.$209

   qMULTIPLE           reduce (207)
   qROOTONLY           reduce (207)
   qSINGLE             reduce (207)
   qROOTONLYMULTIPLE   reduce (207)
   qCONTINUE           reduce (207)
   qROOTONLYCONTINUE   reduce (207)
   qSINGLEFORUPDATE    reduce (207)
   .                   reduce (209)

   $209                    goto 186
   $207                    goto 187

State 105
  (283)  $283:                    .
         where_clause:                qWHERE.$283 attribute_where_phrase

   .                   reduce (283)

   $283                    goto 188

State 106
         restricting_clause:          qRESTRICTING.qENTITYQUALIFIER $220 qTO attribute_where_phrase

   qENTITYQUALIFIER    shift 189
   .                   error


State 107
  (219)  restricting_phrase:          restricting_clause.

   .                   reduce (219)

State 108
  (205)  activate_action:             activate_phrase qEMPTY.

   .                   reduce (205)

State 109
  (204)  ITEM and lookahead TOO BIG
         restricting_phrase:          restricting_phrase.restricting_clause

   qRESTRICTING        shift 106
   .                   reduce (204)

   restricting_clause         goto 190

State 110
  (200)  $200:                    .   [ qRESTRICTING ]
         activate_action:             activate_phrase where_clause.$200 restricting_phrase
  (202)  ITEM and lookahead TOO BIG
         activate_action:             activate_phrase where_clause.$202

   qRESTRICTING        reduce (200)
   .                   reduce (202)

   $202                    goto 191
   $200                    goto 192

State 111
         qualified_for:               qFOR $192.qCONSTANT

   qCONSTANT           shift 193
   .                   error


State 112
         qualified_for:               qFOR $190.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 194

State 113
         qualified_for:               qFOR $188.qEACH

   qEACH               shift 195
   .                   error


State 114
  (195)  $195:                    .   [ qWITHIN ]
         cursor_target:               qENTITYQUALIFIER.$195 scope
  (197)  ITEM and lookahead TOO BIG

   qWITHIN             reduce (195)
   .                   reduce (197)

   $195                    goto 196

State 115
         for_entry:                   qualified_for cursor_target.where_clause
  (187)  ITEM and lookahead TOO BIG

   qWHERE              shift 105
   .                   reduce (187)

   where_clause               goto 197

State 116
         for_action:                  for_entry $184.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 198

State 117
  (182)  $182:                    .
         for_action:                  for_entry statement_list.$182 end_clause

   .                   reduce (182)

   $182                    goto 199

State 118
  (163)  $163:                    .
         exclude_action:              qEXCLUDE qENTITYQUALIFIER.$163 exclude_position

   .                   reduce (163)

   $163                    goto 200

State 119
  (157)  $157:                    .
         include_action:              qINCLUDE qENTITYQUALIFIER.$157 source_eq position

   .                   reduce (157)

   $157                    goto 201

State 120
         name_action:                 qNAME qTVIEW.name_var task_level qUSING symbol
         name_action:                 qNAME qTVIEW.name_var task_level
         name_action:                 qNAME qTVIEW.name_var

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 202
   name_var                   goto 203

State 121
         get_action:                  qGET qTVIEW.view_var task_level qUSING symbol
         get_action:                  qGET qTVIEW.view_var task_level
         get_action:                  qGET qTVIEW.view_var

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 204
   view_var                   goto 205

State 122
  (150)  commit_action:               qCOMMIT symbol.

   .                   reduce (150)

State 123
         set_action:                  qSET qCURSOR.fnpl_phrase cursor_target
         set_action:                  qSET qCURSOR.fnpl_phrase cursor_target where_clause

   qFIRST              shift 209
   qLAST               shift 206
   qNEXT               shift 208
   qPREVIOUS           shift 207
   .                   error

   fnpl_phrase                goto 210

State 124
  (140)  case_phrase:                 qCASE r_expression.   [ error qEND qOF qOTHERWISE ]
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (140)


State 125
  (273)  negation_op:                 qNOT.

   .                   reduce (273)

State 126
  (268)  $268:                    .
         exist_condition:             qENTITYQUALIFIER.$268 exist_phrase

   .                   reduce (268)

   $268                    goto 211

State 127
  (266)  condition:                   exist_condition.

   .                   reduce (266)

State 128
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
         condition:                   r_expression.qCONDOPERATOR $262 r_expression
         condition:                   r_expression.qEQUAL $264 r_expression
         condition:                   r_expression.qCONDOPERATOR error

   qCONDOPERATOR       shift 213
   qEQUAL              shift 212
   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   error


State 129
         multiple_condition:          negation_op.multiple_condition

   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   multiple_condition         goto 214
   negation_op                goto 129
   lparen                    goto 215

State 130
         multiple_condition:          lparen.compound_condition rparen
         multiple_condition:          lparen.compound_condition rparen conditional_join multiple_condition
         multiple_condition:          lparen.compound_condition rparen conditional_join compound_condition
         multiple_condition:          lparen.multiple_condition rparen
         multiple_condition:          lparen.multiple_condition rparen conditional_join multiple_condition
         multiple_condition:          lparen.multiple_condition rparen conditional_join compound_condition
         r_expression:                lparen.r_expression rparen

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 216
   symbol                    goto 99
   negation_op                goto 129
   multiple_condition         goto 217
   compound_condition         goto 218
   lparen                    goto 130
   condition                  goto 131

State 131
  (127)  ITEM and lookahead TOO BIG
         compound_condition:          condition.conditional_join compound_condition
         compound_condition:          condition.conditional_join multiple_condition

   qJOIN               shift 219
   .                   reduce (127)

   conditional_join           goto 220

State 132
  (121)  ITEM and lookahead TOO BIG
         if_phrase:                   qIF error.$121 statement_list
  (327)  ITEM and lookahead TOO BIG

   qCONDOPERATOR       reduce (327)
   qEQUAL              reduce (327)
   qADDITION           reduce (327)
   qMINUS              reduce (327)
   qMULTIPLICATION     reduce (327)
   qDIVISION           reduce (327)
   .                   reduce (121)

   $121                    goto 221

State 133
  (116)  if_phrase:                   qIF multiple_condition.   [ error qELSE qEND ]
  (119)  ITEM and lookahead TOO BIG
         if_phrase:                   qIF multiple_condition.$119 statement_list

   error               reduce (116)
   qELSE               reduce (116)
   qEND                reduce (116)
   .                   reduce (119)

   $119                    goto 222

State 134
  (115)  if_phrase:                   qIF compound_condition.   [ error qELSE qEND ]
  (117)  ITEM and lookahead TOO BIG
         if_phrase:                   qIF compound_condition.$117 statement_list

   error               reduce (115)
   qELSE               reduce (115)
   qEND                reduce (115)
   .                   reduce (117)

   $117                    goto 223

State 135
  (112)  $112:                    .
         assignment_statement:        left_element qEQUAL.$112 r_expression

   .                   reduce (112)

   $112                    goto 224

State 136
         of_clause:                   qOF.aconstant $275 qCOLON
         of_clause:                   qOF.qDEFINE $277 qCOLON

   qCONSTANT           shift 225
   qDEFINE             shift 227
   qMINUS              shift 226
   .                   error

   aconstant                  goto 228

State 137
         otherwise:                   qOTHERWISE.qCOLON $145 statement_list
         otherwise:                   qOTHERWISE.qCOLON

   qCOLON              shift 229
   .                   error


State 138
         of_list:                     of_clause.statement_list
  (144)  of_list:                     of_clause.   [ error qEND qOF qOTHERWISE ]

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   reduce (144)

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 230
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 139
         selection_statement:         case_phrase $99.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 231

State 140
   (96)  selection_statement:         case_phrase error.

   .                   reduce (96)

State 141
   (89)  $89:                    .   [ qEND ]
         selection_statement:         case_phrase otherwise.$89 end_clause
         selection_statement:         case_phrase otherwise.error
         selection_statement:         case_phrase otherwise.of_list $97 end_clause

   error               shift 233
   qEND                reduce (89)
   qOF                 shift 136
   .                   error

   of_clause                  goto 138
   of_list                    goto 232
   $89                    goto 234

State 142
         selection_statement:         case_phrase of_list.end_clause
         selection_statement:         case_phrase of_list.otherwise $91 end_clause
         selection_statement:         case_phrase of_list.otherwise error
         selection_statement:         case_phrase of_list.error
         of_list:                     of_list.of_clause statement_list
         of_list:                     of_list.of_clause

   error               shift 236
   qEND                shift 153
   qOF                 shift 136
   qOTHERWISE          shift 137
   .                   error

   of_clause                  goto 235
   otherwise                  goto 237
   end_clause                 goto 238

State 143
  (274)  while_keyword:               qWHILE.

   .                   reduce (274)

State 144
         while_clause:                while_keyword.compound_condition
         while_clause:                while_keyword.multiple_condition
         while_clause:                while_keyword.error

   error               shift 239
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   multiple_condition         goto 240
   compound_condition         goto 241
   negation_op                goto 129
   lparen                    goto 130
   condition                  goto 131

State 145
         until_phrase:                qUNTIL.compound_condition
         until_phrase:                qUNTIL.multiple_condition
         until_phrase:                qUNTIL.error

   error               shift 242
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   lparen                    goto 130
   condition                  goto 131
   multiple_condition         goto 243
   compound_condition         goto 244

State 146
         loop_statement:              loop_phrase while_clause.end_clause
         loop_statement:              loop_phrase while_clause.statement_list end_clause
         loop_statement:              loop_phrase while_clause.error
         loop_statement:              loop_phrase while_clause.statement_list error

   error               shift 245
   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qEND                shift 153
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   statement_list             goto 246
   end_clause                 goto 247
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 147
         loop_statement:              loop_phrase statement_list.until_phrase

   qUNTIL              shift 145
   .                   error

   until_phrase               goto 248

State 148
   (79)  loop_statement:              loop_phrase until_phrase.

   .                   reduce (79)

State 149
  (123)  else_phrase:                 qELSE.   [ qEND ]
  (124)  ITEM and lookahead TOO BIG
         else_phrase:                 qELSE.$124 statement_list

   qEND                reduce (123)
   .                   reduce (124)

   $124                    goto 249

State 150
   (78)  conditional_statement:       if_phrase error.

   .                   reduce (78)

State 151
         conditional_statement:       if_phrase else_phrase.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 250

State 152
   (76)  conditional_statement:       if_phrase end_clause.

   .                   reduce (76)

State 153
   (40)  end_clause:                  qEND.

   .                   reduce (40)

State 154
   (70)  multiple_statements:         multiple_statements statement.

   .                   reduce (70)

State 155
   (68)  statement_list:              multiple_statements exit_statement.

   .                   reduce (68)

State 156
   (63)  view_name:                   qTVIEW symbol.

   .                   reduce (63)

State 157
         view_type:                   qREGISTERED.qAS symbol

   qAS                 shift 251
   .                   error


State 158
         view_type:                   qBASED.qON qLOD symbol

   qON                 shift 252
   .                   error


State 159
   (62)  view_declaration:            view_name error.

   .                   reduce (62)

State 160
   (60)  view_declaration:            view_name view_type.

   .                   reduce (60)

State 161
         dimension:                   qLPAREN.qCONSTANT $56 qRPAREN
   (58)  $58:                    .   [ qRPAREN ]
         dimension:                   qLPAREN.$58 qRPAREN

   qCONSTANT           shift 254
   qRPAREN             reduce (58)
   .                   error

   $58                    goto 253

State 162
   (49)  data_type:                   qTSTRING dimension.

   .                   reduce (49)

State 163
   (46)  local_declaration:           data_type error.

   .                   reduce (46)

State 164
   (45)  local_declaration:           data_type symbol.

   .                   reduce (45)

State 165
   (38)  operation_body:              statement_list declaration_list.   [ qEND ]
         declaration_list:            declaration_list.declaration

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   .                   reduce (38)

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 73
   local_declaration          goto 74
   declaration                goto 166

State 166
   (42)  declaration_list:            declaration_list declaration.

   .                   reduce (42)

State 167
   (37)  operation_body:              declaration_list statement_list.

   .                   reduce (37)

State 168
         parameter_definition_list:   qLPAREN $25.qRPAREN

   qRPAREN             shift 255
   .                   error


State 169
         parameter_definition_list:   qLPAREN $23.parameter_declaration_list qRPAREN

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qRETURNED           shift 256
   .                   error

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 257
   pointer_indicator          goto 258
   local_declaration          goto 259
   parameter_declaration      goto 260
   parameter_declaration_list goto 261

State 170
         operation_definition:        operation_declaration operation_body $6.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 262

State 171
    (4)  $4:                    .
         operation_definition:        operation_declaration parameter_definition_list operation_body.$4 end_clause

   .                   reduce (4)

   $4                    goto 263

State 172
         r_expression:                qMINUS $257.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 264
   symbol                    goto 99

State 173
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
         r_expression:                lparen r_expression.rparen

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   qRPAREN             shift 265
   .                   error

   rparen                    goto 266

State 174
  (254)  $254:                    .
         r_expression:                r_expression qDIVISION.$254 r_expression

   .                   reduce (254)

   $254                    goto 267

State 175
  (252)  $252:                    .
         r_expression:                r_expression qMULTIPLICATION.$252 r_expression

   .                   reduce (252)

   $252                    goto 268

State 176
  (250)  $250:                    .
         r_expression:                r_expression qMINUS.$250 r_expression

   .                   reduce (250)

   $250                    goto 269

State 177
  (248)  $248:                    .
         r_expression:                r_expression qADDITION.$248 r_expression

   .                   reduce (248)

   $248                    goto 270

State 178
         value:                     symbol $322.offset

   qLBRACE             shift 179
   .                   error

   offset                    goto 271

State 179
  (331)  $331:                    .
         offset:                     qLBRACE.$331 index_and_length qRBRACE

   .                   reduce (331)

   $331                    goto 272

State 180
  (245)  left_element:                symbol $244 offset.

   .                   reduce (245)

State 181
  (317)  openoper:                    qLPAREN.

   .                   reduce (317)

State 182
  (313)  ITEM and lookahead TOO BIG
         parameter_value_list:        openoper.$313 value_list closeoper
  (315)  $315:                    .   [ qRPAREN ]
         parameter_value_list:        openoper.$315 closeoper

   qRPAREN             reduce (315)
   .                   reduce (313)

   $315                    goto 273
   $313                    goto 274

State 183
  (243)  operation_call:              symbol $242 parameter_value_list.

   .                   reduce (243)

State 184
  (231)  $231:                    .   [ qFIRST ]
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER.$231 qFIRST
  (233)  $233:                    .   [ qLAST ]
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER.$233 qLAST
  (235)  $235:                    .   [ qPREVIOUS ]
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER.$235 qPREVIOUS
  (237)  $237:                    .   [ qNEXT ]
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER.$237 qNEXT
  (239)  $239:                    .   [ qNONE ]
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER.$239 qNONE
  (241)  ITEM and lookahead TOO BIG

   qFIRST              reduce (231)
   qLAST               reduce (233)
   qNEXT               reduce (237)
   qPREVIOUS           reduce (235)
   qNONE               reduce (239)
   .                   reduce (241)

   $239                    goto 275
   $237                    goto 276
   $235                    goto 277
   $233                    goto 278
   $231                    goto 279

State 185
  (222)  $222:                    .   [ qAFTER ]
         create_action:               qCREATE qENTITY qENTITYQUALIFIER.$222 qAFTER
  (224)  $224:                    .   [ qBEFORE ]
         create_action:               qCREATE qENTITY qENTITYQUALIFIER.$224 qBEFORE
  (226)  $226:                    .   [ qFIRST ]
         create_action:               qCREATE qENTITY qENTITYQUALIFIER.$226 qFIRST
  (228)  $228:                    .   [ qLAST ]
         create_action:               qCREATE qENTITY qENTITYQUALIFIER.$228 qLAST
  (230)  ITEM and lookahead TOO BIG

   qAFTER              reduce (222)
   qBEFORE             reduce (224)
   qFIRST              reduce (226)
   qLAST               reduce (228)
   .                   reduce (230)

   $228                    goto 280
   $226                    goto 281
   $224                    goto 282
   $222                    goto 283

State 186
  (210)  activate_phrase:             qACTIVATE symbol $209.

   .                   reduce (210)

State 187
         activate_phrase:             qACTIVATE symbol $207.zdefine

   qMULTIPLE           shift 289
   qROOTONLY           shift 288
   qSINGLE             shift 290
   qROOTONLYMULTIPLE   shift 287
   qCONTINUE           shift 286
   qROOTONLYCONTINUE   shift 285
   qSINGLEFORUPDATE    shift 284
   .                   error

   zdefine                    goto 291

State 188
         where_clause:                qWHERE $283.attribute_where_phrase

   qATTRIBUTEQUALIFIER shift 293
   qENTITYQUALIFIER    shift 292
   qLPAREN             shift 295
   .                   error

   attribute_op_phrase        goto 294
   attribute_where_phrase     goto 296

State 189
  (220)  $220:                    .
         restricting_clause:          qRESTRICTING qENTITYQUALIFIER.$220 qTO attribute_where_phrase

   .                   reduce (220)

   $220                    goto 297

State 190
  (218)  restricting_phrase:          restricting_phrase restricting_clause.

   .                   reduce (218)

State 191
  (203)  activate_action:             activate_phrase where_clause $202.

   .                   reduce (203)

State 192
         activate_action:             activate_phrase where_clause $200.restricting_phrase

   qRESTRICTING        shift 106
   .                   error

   restricting_clause         goto 107
   restricting_phrase         goto 298

State 193
  (193)  qualified_for:               qFOR $192 qCONSTANT.

   .                   reduce (193)

State 194
  (191)  qualified_for:               qFOR $190 symbol.

   .                   reduce (191)

State 195
  (189)  qualified_for:               qFOR $188 qEACH.

   .                   reduce (189)

State 196
         cursor_target:               qENTITYQUALIFIER $195.scope

   qWITHIN             shift 299
   .                   error

   scope                    goto 300

State 197
  (186)  for_entry:                   qualified_for cursor_target where_clause.

   .                   reduce (186)

State 198
  (185)  for_action:                  for_entry $184 end_clause.

   .                   reduce (185)

State 199
         for_action:                  for_entry statement_list $182.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 301

State 200
         exclude_action:              qEXCLUDE qENTITYQUALIFIER $163.exclude_position
  (168)  ITEM and lookahead TOO BIG

   qAFTER              shift 302
   qBEFORE             shift 304
   qNONE               shift 303
   .                   reduce (168)

   exclude_position           goto 305

State 201
         include_action:              qINCLUDE qENTITYQUALIFIER $157.source_eq position

   qFROM               shift 306
   .                   error

   source_eq                  goto 307

State 202
  (169)  $169:                    .   [ qCONSTANT ]
         name_var:                    symbol.$169 qCONSTANT
  (171)  $171:                    .   [ qVARIABLE ]
         name_var:                    symbol.$171 symbol

   qCONSTANT           reduce (169)
   qVARIABLE           reduce (171)
   .                   error

   $171                    goto 308
   $169                    goto 309

State 203
         name_action:                 qNAME qTVIEW name_var.task_level qUSING symbol
         name_action:                 qNAME qTVIEW name_var.task_level
  (156)  ITEM and lookahead TOO BIG

   qAT                 shift 310
   .                   reduce (156)

   task_level                 goto 311

State 204
  (173)  $173:                    .
         view_var:                    symbol.$173 qNAMED view_qual

   .                   reduce (173)

   $173                    goto 312

State 205
         get_action:                  qGET qTVIEW view_var.task_level qUSING symbol
         get_action:                  qGET qTVIEW view_var.task_level
  (153)  ITEM and lookahead TOO BIG

   qAT                 shift 310
   .                   reduce (153)

   task_level                 goto 313

State 206
  (282)  fnpl_phrase:                 qLAST.

   .                   reduce (282)

State 207
  (281)  fnpl_phrase:                 qPREVIOUS.

   .                   reduce (281)

State 208
  (280)  fnpl_phrase:                 qNEXT.

   .                   reduce (280)

State 209
  (279)  fnpl_phrase:                 qFIRST.

   .                   reduce (279)

State 210
         set_action:                  qSET qCURSOR fnpl_phrase.cursor_target
         set_action:                  qSET qCURSOR fnpl_phrase.cursor_target where_clause

   qENTITYQUALIFIER    shift 114
   .                   error

   cursor_target              goto 314

State 211
         exist_condition:             qENTITYQUALIFIER $268.exist_phrase

   qEXISTS             shift 316
   qDOES               shift 315
   .                   error

   exist_phrase               goto 317

State 212
  (264)  $264:                    .
         condition:                   r_expression qEQUAL.$264 r_expression

   .                   reduce (264)

   $264                    goto 318

State 213
  (262)  ITEM and lookahead TOO BIG
         condition:                   r_expression qCONDOPERATOR.$262 r_expression
         condition:                   r_expression qCONDOPERATOR.error

   error               shift 319
   qCONSTANT           reduce (262)
   qVARIABLE           reduce (262)
   qDEFINE             reduce (262)
   qATTRIBUTEQUALIFIER reduce (262)
   qMINUS              reduce (262)
   qLPAREN             reduce (262)
   .                   error

   $262                    goto 320

State 214
  (136)  multiple_condition:          negation_op multiple_condition.

   .                   reduce (136)

State 215
         multiple_condition:          lparen.compound_condition rparen
         multiple_condition:          lparen.compound_condition rparen conditional_join multiple_condition
         multiple_condition:          lparen.compound_condition rparen conditional_join compound_condition
         multiple_condition:          lparen.multiple_condition rparen
         multiple_condition:          lparen.multiple_condition rparen conditional_join multiple_condition
         multiple_condition:          lparen.multiple_condition rparen conditional_join compound_condition

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   multiple_condition         goto 217
   compound_condition         goto 218
   lparen                    goto 130
   condition                  goto 131

State 216
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
         r_expression:                lparen r_expression.rparen
         condition:                   r_expression.qCONDOPERATOR $262 r_expression
         condition:                   r_expression.qEQUAL $264 r_expression
         condition:                   r_expression.qCONDOPERATOR error

   qCONDOPERATOR       shift 213
   qEQUAL              shift 212
   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   qRPAREN             shift 265
   .                   error

   rparen                    goto 266

State 217
         multiple_condition:          lparen multiple_condition.rparen
         multiple_condition:          lparen multiple_condition.rparen conditional_join multiple_condition
         multiple_condition:          lparen multiple_condition.rparen conditional_join compound_condition

   qRPAREN             shift 265
   .                   error

   rparen                    goto 321

State 218
         multiple_condition:          lparen compound_condition.rparen
         multiple_condition:          lparen compound_condition.rparen conditional_join multiple_condition
         multiple_condition:          lparen compound_condition.rparen conditional_join compound_condition

   qRPAREN             shift 265
   .                   error

   rparen                    goto 322

State 219
  (270)  conditional_join:            qJOIN.

   .                   reduce (270)

State 220
         compound_condition:          condition conditional_join.compound_condition
         compound_condition:          condition conditional_join.multiple_condition

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   lparen                    goto 130
   multiple_condition         goto 323
   compound_condition         goto 324
   condition                  goto 131

State 221
         if_phrase:                   qIF error $121.statement_list

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 325
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 222
         if_phrase:                   qIF multiple_condition $119.statement_list

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 326
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 223
         if_phrase:                   qIF compound_condition $117.statement_list

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 327
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 224
         assignment_statement:        left_element qEQUAL $112.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   symbol                    goto 99
   r_expression               goto 328

State 225
  (312)  aconstant:                   qCONSTANT.

   .                   reduce (312)

State 226
  (310)  $310:                    .
         aconstant:                   qMINUS.$310 qCONSTANT

   .                   reduce (310)

   $310                    goto 329

State 227
  (277)  $277:                    .
         of_clause:                   qOF qDEFINE.$277 qCOLON

   .                   reduce (277)

   $277                    goto 330

State 228
  (275)  $275:                    .
         of_clause:                   qOF aconstant.$275 qCOLON

   .                   reduce (275)

   $275                    goto 331

State 229
  (145)  ITEM and lookahead TOO BIG
         otherwise:                   qOTHERWISE qCOLON.$145 statement_list
  (147)  otherwise:                   qOTHERWISE qCOLON.   [ error qEND qOF ]

   error               reduce (147)
   qEND                reduce (147)
   qOF                 reduce (147)
   .                   reduce (145)

   $145                    goto 332

State 230
  (143)  of_list:                     of_clause statement_list.

   .                   reduce (143)

State 231
  (100)  selection_statement:         case_phrase $99 end_clause.

   .                   reduce (100)

State 232
   (97)  $97:                    .   [ qEND ]
         selection_statement:         case_phrase otherwise of_list.$97 end_clause
         of_list:                     of_list.of_clause statement_list
         of_list:                     of_list.of_clause

   qEND                reduce (97)
   qOF                 shift 136
   .                   error

   of_clause                  goto 235
   $97                    goto 333

State 233
   (95)  selection_statement:         case_phrase otherwise error.

   .                   reduce (95)

State 234
         selection_statement:         case_phrase otherwise $89.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 334

State 235
         of_list:                     of_list of_clause.statement_list
  (142)  of_list:                     of_list of_clause.   [ error qEND qOF qOTHERWISE ]

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   reduce (142)

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 335
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 236
   (94)  selection_statement:         case_phrase of_list error.

   .                   reduce (94)

State 237
   (91)  $91:                    .   [ qEND ]
         selection_statement:         case_phrase of_list otherwise.$91 end_clause
         selection_statement:         case_phrase of_list otherwise.error

   error               shift 336
   qEND                reduce (91)
   .                   error

   $91                    goto 337

State 238
   (88)  selection_statement:         case_phrase of_list end_clause.

   .                   reduce (88)

State 239
  (139)  ITEM and lookahead TOO BIG
  (327)  ITEM and lookahead TOO BIG

   qCONDOPERATOR       reduce (327)
   qEQUAL              reduce (327)
   qADDITION           reduce (327)
   qMINUS              reduce (327)
   qMULTIPLICATION     reduce (327)
   qDIVISION           reduce (327)
   .                   reduce (139)

State 240
  (138)  while_clause:                while_keyword multiple_condition.

   .                   reduce (138)

State 241
  (137)  while_clause:                while_keyword compound_condition.

   .                   reduce (137)

State 242
   (87)  ITEM and lookahead TOO BIG
  (327)  ITEM and lookahead TOO BIG

   qCONDOPERATOR       reduce (327)
   qEQUAL              reduce (327)
   qADDITION           reduce (327)
   qMINUS              reduce (327)
   qMULTIPLICATION     reduce (327)
   qDIVISION           reduce (327)
   .                   reduce (87)

State 243
   (86)  until_phrase:                qUNTIL multiple_condition.

   .                   reduce (86)

State 244
   (85)  until_phrase:                qUNTIL compound_condition.

   .                   reduce (85)

State 245
   (83)  loop_statement:              loop_phrase while_clause error.

   .                   reduce (83)

State 246
         loop_statement:              loop_phrase while_clause statement_list.end_clause
         loop_statement:              loop_phrase while_clause statement_list.error

   error               shift 338
   qEND                shift 153
   .                   error

   end_clause                 goto 339

State 247
   (81)  loop_statement:              loop_phrase while_clause end_clause.

   .                   reduce (81)

State 248
   (80)  loop_statement:              loop_phrase statement_list until_phrase.

   .                   reduce (80)

State 249
         else_phrase:                 qELSE $124.statement_list

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 340
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 250
   (77)  conditional_statement:       if_phrase else_phrase end_clause.

   .                   reduce (77)

State 251
         view_type:                   qREGISTERED qAS.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 341

State 252
         view_type:                   qBASED qON.qLOD symbol

   qLOD                shift 342
   .                   error


State 253
         dimension:                   qLPAREN $58.qRPAREN

   qRPAREN             shift 343
   .                   error


State 254
   (56)  $56:                    .
         dimension:                   qLPAREN qCONSTANT.$56 qRPAREN

   .                   reduce (56)

   $56                    goto 344

State 255
   (26)  parameter_definition_list:   qLPAREN $25 qRPAREN.

   .                   reduce (26)

State 256
   (55)  pointer_indicator:           qRETURNED.

   .                   reduce (55)

State 257
   (33)  parameter_declaration:       view_declaration.

   .                   reduce (33)

State 258
         parameter_declaration:       pointer_indicator.local_declaration
         parameter_declaration:       pointer_indicator.view_declaration

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   .                   error

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 345
   local_declaration          goto 346

State 259
   (31)  parameter_declaration:       local_declaration.

   .                   reduce (31)

State 260
   (27)  parameter_declaration_list:  parameter_declaration.

   .                   reduce (27)

State 261
         parameter_definition_list:   qLPAREN $23 parameter_declaration_list.qRPAREN
         parameter_declaration_list:  parameter_declaration_list.qCOMMA parameter_declaration
         parameter_declaration_list:  parameter_declaration_list.error $29 parameter_declaration

   error               shift 347
   qCOMMA              shift 348
   qRPAREN             shift 349
   .                   error


State 262
    (7)  operation_definition:        operation_declaration operation_body $6 end_clause.

   .                   reduce (7)

State 263
         operation_definition:        operation_declaration parameter_definition_list operation_body $4.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 350

State 264
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (258)  ITEM and lookahead TOO BIG

   .                   reduce (258)


State 265
  (272)  rparen:                     qRPAREN.

   .                   reduce (272)

State 266
  (256)  r_expression:                lparen r_expression rparen.

   .                   reduce (256)

State 267
         r_expression:                r_expression qDIVISION $254.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 351
   symbol                    goto 99

State 268
         r_expression:                r_expression qMULTIPLICATION $252.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 352
   symbol                    goto 99

State 269
         r_expression:                r_expression qMINUS $250.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 353
   symbol                    goto 99

State 270
         r_expression:                r_expression qADDITION $248.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 354
   symbol                    goto 99

State 271
  (323)  value:                     symbol $322 offset.

   .                   reduce (323)

State 272
         offset:                     qLBRACE $331.index_and_length qRBRACE

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   index_and_length           goto 355
   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 356
   symbol                    goto 99

State 273
         parameter_value_list:        openoper $315.closeoper

   qRPAREN             shift 357
   .                   error

   closeoper                  goto 358

State 274
         parameter_value_list:        openoper $313.value_list closeoper

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   value_list                 goto 359
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 360
   symbol                    goto 99

State 275
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER $239.qNONE

   qNONE               shift 361
   .                   error


State 276
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER $237.qNEXT

   qNEXT               shift 362
   .                   error


State 277
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER $235.qPREVIOUS

   qPREVIOUS           shift 363
   .                   error


State 278
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER $233.qLAST

   qLAST               shift 364
   .                   error


State 279
         delete_action:               qDELETE qENTITY qENTITYQUALIFIER $231.qFIRST

   qFIRST              shift 365
   .                   error


State 280
         create_action:               qCREATE qENTITY qENTITYQUALIFIER $228.qLAST

   qLAST               shift 366
   .                   error


State 281
         create_action:               qCREATE qENTITY qENTITYQUALIFIER $226.qFIRST

   qFIRST              shift 367
   .                   error


State 282
         create_action:               qCREATE qENTITY qENTITYQUALIFIER $224.qBEFORE

   qBEFORE             shift 368
   .                   error


State 283
         create_action:               qCREATE qENTITY qENTITYQUALIFIER $222.qAFTER

   qAFTER              shift 369
   .                   error


State 284
  (217)  zdefine:                     qSINGLEFORUPDATE.

   .                   reduce (217)

State 285
  (216)  zdefine:                     qROOTONLYCONTINUE.

   .                   reduce (216)

State 286
  (215)  zdefine:                     qCONTINUE.

   .                   reduce (215)

State 287
  (214)  zdefine:                     qROOTONLYMULTIPLE.

   .                   reduce (214)

State 288
  (213)  zdefine:                     qROOTONLY.

   .                   reduce (213)

State 289
  (212)  zdefine:                     qMULTIPLE.

   .                   reduce (212)

State 290
  (211)  zdefine:                     qSINGLE.

   .                   reduce (211)

State 291
  (208)  activate_phrase:             qACTIVATE symbol $207 zdefine.

   .                   reduce (208)

State 292
  (294)  $294:                    .   [ qEXISTS qDOES ]
         attribute_op_phrase:         qENTITYQUALIFIER.$294 exist_phrase
  (296)  $296:                    .   [ qIS ]
         attribute_op_phrase:         qENTITYQUALIFIER.$296 is_keyword qENTITYQUALIFIER

   qEXISTS             reduce (294)
   qDOES               reduce (294)
   qIS                 reduce (296)
   .                   error

   $296                    goto 370
   $294                    goto 371

State 293
  (290)  $290:                    .   [ qCONDOPERATOR qLIKE qEQUAL ]
         attribute_op_phrase:         qATTRIBUTEQUALIFIER.$290 where_op where_value
  (292)  $292:                    .   [ qIS ]
         attribute_op_phrase:         qATTRIBUTEQUALIFIER.$292 is_null_phrase

   qCONDOPERATOR       reduce (290)
   qLIKE               reduce (290)
   qIS                 reduce (292)
   qEQUAL              reduce (290)
   .                   error

   $292                    goto 372
   $290                    goto 373

State 294
  (289)  attribute_where_phrase:      attribute_op_phrase.

   .                   reduce (289)

State 295
  (287)  $287:                    .
         attribute_where_phrase:      qLPAREN.$287 attribute_where_phrase qRPAREN

   .                   reduce (287)

   $287                    goto 374

State 296
  (284)  ITEM and lookahead TOO BIG
         attribute_where_phrase:      attribute_where_phrase.qJOIN $285 attribute_where_phrase

   qJOIN               shift 375
   .                   reduce (284)


State 297
         restricting_clause:          qRESTRICTING qENTITYQUALIFIER $220.qTO attribute_where_phrase

   qTO                 shift 376
   .                   error


State 298
  (201)  ITEM and lookahead TOO BIG
         restricting_phrase:          restricting_phrase.restricting_clause

   qRESTRICTING        shift 106
   .                   reduce (201)

   restricting_clause         goto 190

State 299
         scope:                     qWITHIN.qENTITYQUALIFIER
         scope:                     qWITHIN.qOBJECT

   qOBJECT             shift 377
   qENTITYQUALIFIER    shift 378
   .                   error


State 300
  (196)  cursor_target:               qENTITYQUALIFIER $195 scope.

   .                   reduce (196)

State 301
  (183)  for_action:                  for_entry statement_list $182 end_clause.

   .                   reduce (183)

State 302
  (167)  exclude_position:            qAFTER.

   .                   reduce (167)

State 303
  (166)  exclude_position:            qNONE.

   .                   reduce (166)

State 304
  (165)  exclude_position:            qBEFORE.

   .                   reduce (165)

State 305
  (164)  exclude_action:              qEXCLUDE qENTITYQUALIFIER $163 exclude_position.

   .                   reduce (164)

State 306
         source_eq:                   qFROM.qENTITYQUALIFIER

   qENTITYQUALIFIER    shift 379
   .                   error


State 307
         include_action:              qINCLUDE qENTITYQUALIFIER $157 source_eq.position
  (162)  ITEM and lookahead TOO BIG

   qAFTER              shift 380
   qBEFORE             shift 381
   .                   reduce (162)

   position                   goto 382

State 308
         name_var:                    symbol $171.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 383

State 309
         name_var:                    symbol $169.qCONSTANT

   qCONSTANT           shift 384
   .                   error


State 310
         task_level:                  qAT.qLEVEL qSUBTASK
         task_level:                  qAT.qLEVEL qTASK
         task_level:                  qAT.qLEVEL qAPPLICATION
         task_level:                  qAT.qLEVEL qSYSTEM
         task_level:                  qAT.qLEVEL qANY

   qLEVEL              shift 385
   .                   error


State 311
         name_action:                 qNAME qTVIEW name_var task_level.qUSING symbol
  (155)  ITEM and lookahead TOO BIG

   qUSING              shift 386
   .                   reduce (155)


State 312
         view_var:                    symbol $173.qNAMED view_qual

   qNAMED              shift 387
   .                   error


State 313
         get_action:                  qGET qTVIEW view_var task_level.qUSING symbol
  (152)  ITEM and lookahead TOO BIG

   qUSING              shift 388
   .                   reduce (152)


State 314
  (148)  ITEM and lookahead TOO BIG
         set_action:                  qSET qCURSOR fnpl_phrase cursor_target.where_clause

   qWHERE              shift 105
   .                   reduce (148)

   where_clause               goto 389

State 315
         exist_phrase:                qDOES.qNOT qEXIST

   qNOT                shift 390
   .                   error


State 316
  (301)  exist_phrase:                qEXISTS.

   .                   reduce (301)

State 317
  (269)  exist_condition:             qENTITYQUALIFIER $268 exist_phrase.

   .                   reduce (269)

State 318
         condition:                   r_expression qEQUAL $264.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 391
   symbol                    goto 99

State 319
  (267)  condition:                   r_expression qCONDOPERATOR error.

   .                   reduce (267)

State 320
         condition:                   r_expression qCONDOPERATOR $262.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 392
   symbol                    goto 99

State 321
  (133)  ITEM and lookahead TOO BIG
         multiple_condition:          lparen multiple_condition rparen.conditional_join multiple_condition
         multiple_condition:          lparen multiple_condition rparen.conditional_join compound_condition

   qJOIN               shift 219
   .                   reduce (133)

   conditional_join           goto 393

State 322
  (130)  ITEM and lookahead TOO BIG
         multiple_condition:          lparen compound_condition rparen.conditional_join multiple_condition
         multiple_condition:          lparen compound_condition rparen.conditional_join compound_condition

   qJOIN               shift 219
   .                   reduce (130)

   conditional_join           goto 394

State 323
  (129)  compound_condition:          condition conditional_join multiple_condition.

   .                   reduce (129)

State 324
  (128)  compound_condition:          condition conditional_join compound_condition.

   .                   reduce (128)

State 325
  (122)  if_phrase:                   qIF error $121 statement_list.

   .                   reduce (122)

State 326
  (120)  if_phrase:                   qIF multiple_condition $119 statement_list.

   .                   reduce (120)

State 327
  (118)  if_phrase:                   qIF compound_condition $117 statement_list.

   .                   reduce (118)

State 328
  (113)  ITEM and lookahead TOO BIG
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (113)


State 329
         aconstant:                   qMINUS $310.qCONSTANT

   qCONSTANT           shift 395
   .                   error


State 330
         of_clause:                   qOF qDEFINE $277.qCOLON

   qCOLON              shift 396
   .                   error


State 331
         of_clause:                   qOF aconstant $275.qCOLON

   qCOLON              shift 397
   .                   error


State 332
         otherwise:                   qOTHERWISE qCOLON $145.statement_list

   qACTIVATE           shift 25
   qCASE               shift 36
   qCREATE             shift 24
   qCOMMIT             shift 34
   qDELETE             shift 23
   qFOR                shift 27
   qIF                 shift 38
   qLOOP               shift 37
   qRETURN             shift 20
   qSET                shift 35
   qVARIABLE           shift 19
   qGET                shift 33
   qNAME               shift 32
   qINCLUDE            shift 31
   qEXCLUDE            shift 30
   qATTRIBUTEQUALIFIER shift 21
   .                   error

   symbol                    goto 22
   activate_phrase            goto 26
   qualified_for              goto 28
   for_entry                  goto 29
   statement_list             goto 398
   return_value               goto 39
   left_element               goto 40
   operation_call             goto 41
   exclude_action             goto 42
   include_action             goto 43
   name_action                goto 44
   get_action                 goto 45
   delete_action              goto 46
   create_action              goto 47
   commit_action              goto 48
   activate_action            goto 49
   for_action                 goto 50
   set_action                 goto 51
   case_phrase                goto 52
   loop_phrase                goto 53
   if_phrase                  goto 54
   assignment_statement       goto 55
   action_statement           goto 56
   selection_statement        goto 57
   loop_statement             goto 58
   conditional_statement      goto 59
   statement                  goto 60
   exit_statement             goto 61
   multiple_statements        goto 62

State 333
         selection_statement:         case_phrase otherwise of_list $97.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 399

State 334
   (90)  selection_statement:         case_phrase otherwise $89 end_clause.

   .                   reduce (90)

State 335
  (141)  of_list:                     of_list of_clause statement_list.

   .                   reduce (141)

State 336
   (93)  selection_statement:         case_phrase of_list otherwise error.

   .                   reduce (93)

State 337
         selection_statement:         case_phrase of_list otherwise $91.end_clause

   qEND                shift 153
   .                   error

   end_clause                 goto 400

State 338
   (84)  loop_statement:              loop_phrase while_clause statement_list error.

   .                   reduce (84)

State 339
   (82)  loop_statement:              loop_phrase while_clause statement_list end_clause.

   .                   reduce (82)

State 340
  (125)  else_phrase:                 qELSE $124 statement_list.

   .                   reduce (125)

State 341
   (65)  view_type:                   qREGISTERED qAS symbol.

   .                   reduce (65)

State 342
         view_type:                   qBASED qON qLOD.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 401

State 343
   (59)  dimension:                   qLPAREN $58 qRPAREN.

   .                   reduce (59)

State 344
         dimension:                   qLPAREN qCONSTANT $56.qRPAREN

   qRPAREN             shift 402
   .                   error


State 345
   (34)  parameter_declaration:       pointer_indicator view_declaration.

   .                   reduce (34)

State 346
   (32)  parameter_declaration:       pointer_indicator local_declaration.

   .                   reduce (32)

State 347
   (29)  $29:                    .
         parameter_declaration_list:  parameter_declaration_list error.$29 parameter_declaration

   .                   reduce (29)

   $29                    goto 403

State 348
         parameter_declaration_list:  parameter_declaration_list qCOMMA.parameter_declaration

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qRETURNED           shift 256
   .                   error

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 257
   pointer_indicator          goto 258
   local_declaration          goto 259
   parameter_declaration      goto 404

State 349
   (24)  parameter_definition_list:   qLPAREN $23 parameter_declaration_list qRPAREN.

   .                   reduce (24)

State 350
    (5)  operation_definition:        operation_declaration parameter_definition_list operation_body $4 end_clause.

   .                   reduce (5)

State 351
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (255)  ITEM and lookahead TOO BIG

   .                   reduce (255)


State 352
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
  (253)  ITEM and lookahead TOO BIG
         r_expression:                r_expression.qDIVISION $254 r_expression

   .                   reduce (253)


State 353
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
  (251)  ITEM and lookahead TOO BIG
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression

   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (251)


State 354
         r_expression:                r_expression.qADDITION $248 r_expression
  (249)  ITEM and lookahead TOO BIG
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression

   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (249)


State 355
         offset:                     qLBRACE $331 index_and_length.qRBRACE

   qRBRACE             shift 405
   .                   error


State 356
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
         index_and_length:            r_expression.qCOLON $333 r_expression
  (335)  index_and_length:            r_expression.   [ qRBRACE ]

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   qCOLON              shift 406
   .                   reduce (335)


State 357
  (318)  closeoper:                   qRPAREN.

   .                   reduce (318)

State 358
  (316)  parameter_value_list:        openoper $315 closeoper.

   .                   reduce (316)

State 359
         parameter_value_list:        openoper $313 value_list.closeoper

   qRPAREN             shift 357
   .                   error

   closeoper                  goto 407

State 360
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (328)  value_list:                  r_expression.   [ qRPAREN ]
         value_list:                  r_expression.qCOMMA $329 value_list

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   qCOMMA              shift 408
   .                   reduce (328)


State 361
  (240)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $239 qNONE.

   .                   reduce (240)

State 362
  (238)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $237 qNEXT.

   .                   reduce (238)

State 363
  (236)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $235 qPREVIOUS.

   .                   reduce (236)

State 364
  (234)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $233 qLAST.

   .                   reduce (234)

State 365
  (232)  delete_action:               qDELETE qENTITY qENTITYQUALIFIER $231 qFIRST.

   .                   reduce (232)

State 366
  (229)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $228 qLAST.

   .                   reduce (229)

State 367
  (227)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $226 qFIRST.

   .                   reduce (227)

State 368
  (225)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $224 qBEFORE.

   .                   reduce (225)

State 369
  (223)  create_action:               qCREATE qENTITY qENTITYQUALIFIER $222 qAFTER.

   .                   reduce (223)

State 370
         attribute_op_phrase:         qENTITYQUALIFIER $296.is_keyword qENTITYQUALIFIER

   qIS                 shift 409
   .                   error

   is_keyword                 goto 410

State 371
         attribute_op_phrase:         qENTITYQUALIFIER $294.exist_phrase

   qEXISTS             shift 316
   qDOES               shift 315
   .                   error

   exist_phrase               goto 411

State 372
         attribute_op_phrase:         qATTRIBUTEQUALIFIER $292.is_null_phrase

   qIS                 shift 409
   .                   error

   is_keyword                 goto 412
   is_null_phrase             goto 413

State 373
         attribute_op_phrase:         qATTRIBUTEQUALIFIER $290.where_op where_value

   qCONDOPERATOR       shift 416
   qLIKE               shift 414
   qEQUAL              shift 415
   .                   error

   where_op                   goto 417

State 374
         attribute_where_phrase:      qLPAREN $287.attribute_where_phrase qRPAREN

   qATTRIBUTEQUALIFIER shift 293
   qENTITYQUALIFIER    shift 292
   qLPAREN             shift 295
   .                   error

   attribute_op_phrase        goto 294
   attribute_where_phrase     goto 418

State 375
  (285)  $285:                    .
         attribute_where_phrase:      attribute_where_phrase qJOIN.$285 attribute_where_phrase

   .                   reduce (285)

   $285                    goto 419

State 376
         restricting_clause:          qRESTRICTING qENTITYQUALIFIER $220 qTO.attribute_where_phrase

   qATTRIBUTEQUALIFIER shift 293
   qENTITYQUALIFIER    shift 292
   qLPAREN             shift 295
   .                   error

   attribute_op_phrase        goto 294
   attribute_where_phrase     goto 420

State 377
  (199)  scope:                     qWITHIN qOBJECT.

   .                   reduce (199)

State 378
  (198)  scope:                     qWITHIN qENTITYQUALIFIER.

   .                   reduce (198)

State 379
  (159)  source_eq:                   qFROM qENTITYQUALIFIER.

   .                   reduce (159)

State 380
  (161)  position:                    qAFTER.

   .                   reduce (161)

State 381
  (160)  position:                    qBEFORE.

   .                   reduce (160)

State 382
  (158)  include_action:              qINCLUDE qENTITYQUALIFIER $157 source_eq position.

   .                   reduce (158)

State 383
  (172)  name_var:                    symbol $171 symbol.

   .                   reduce (172)

State 384
  (170)  name_var:                    symbol $169 qCONSTANT.

   .                   reduce (170)

State 385
         task_level:                  qAT qLEVEL.qSUBTASK
         task_level:                  qAT qLEVEL.qTASK
         task_level:                  qAT qLEVEL.qAPPLICATION
         task_level:                  qAT qLEVEL.qSYSTEM
         task_level:                  qAT qLEVEL.qANY

   qSUBTASK            shift 425
   qTASK               shift 424
   qAPPLICATION        shift 423
   qSYSTEM             shift 422
   qANY                shift 421
   .                   error


State 386
         name_action:                 qNAME qTVIEW name_var task_level qUSING.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 426

State 387
         view_var:                    symbol $173 qNAMED.view_qual

   qCONSTANT           shift 428
   qVARIABLE           shift 19
   .                   error

   symbol                    goto 427
   view_qual                  goto 429

State 388
         get_action:                  qGET qTVIEW view_var task_level qUSING.symbol

   qVARIABLE           shift 19
   .                   error

   symbol                    goto 430

State 389
  (149)  set_action:                  qSET qCURSOR fnpl_phrase cursor_target where_clause.

   .                   reduce (149)

State 390
         exist_phrase:                qDOES qNOT.qEXIST

   qEXIST              shift 431
   .                   error


State 391
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (265)  ITEM and lookahead TOO BIG

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (265)


State 392
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (263)  ITEM and lookahead TOO BIG

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (263)


State 393
         multiple_condition:          lparen multiple_condition rparen conditional_join.multiple_condition
         multiple_condition:          lparen multiple_condition rparen conditional_join.compound_condition

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   compound_condition         goto 432
   multiple_condition         goto 433
   lparen                    goto 130
   condition                  goto 131

State 394
         multiple_condition:          lparen compound_condition rparen conditional_join.multiple_condition
         multiple_condition:          lparen compound_condition rparen conditional_join.compound_condition

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qENTITYQUALIFIER    shift 126
   qMINUS              shift 96
   qNOT                shift 125
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   exist_condition            goto 127
   simple_expression          goto 95
   r_expression               goto 128
   symbol                    goto 99
   negation_op                goto 129
   compound_condition         goto 434
   multiple_condition         goto 435
   lparen                    goto 130
   condition                  goto 131

State 395
  (311)  aconstant:                   qMINUS $310 qCONSTANT.

   .                   reduce (311)

State 396
  (278)  of_clause:                   qOF qDEFINE $277 qCOLON.

   .                   reduce (278)

State 397
  (276)  of_clause:                   qOF aconstant $275 qCOLON.

   .                   reduce (276)

State 398
  (146)  otherwise:                   qOTHERWISE qCOLON $145 statement_list.

   .                   reduce (146)

State 399
   (98)  selection_statement:         case_phrase otherwise of_list $97 end_clause.

   .                   reduce (98)

State 400
   (92)  selection_statement:         case_phrase of_list otherwise $91 end_clause.

   .                   reduce (92)

State 401
   (64)  view_type:                   qBASED qON qLOD symbol.

   .                   reduce (64)

State 402
   (57)  dimension:                   qLPAREN qCONSTANT $56 qRPAREN.

   .                   reduce (57)

State 403
         parameter_declaration_list:  parameter_declaration_list error $29.parameter_declaration

   qTATTRIBUTENAME     shift 67
   qTENTITYNAME        shift 68
   qTDECIMAL           shift 70
   qTINTEGER           shift 71
   qTSHORT             shift 66
   qTSTRING            shift 69
   qTVIEW              shift 63
   qTCHARACTER         shift 65
   qRETURNED           shift 256
   .                   error

   view_name                  goto 64
   data_type                  goto 72
   view_declaration           goto 257
   pointer_indicator          goto 258
   local_declaration          goto 259
   parameter_declaration      goto 436

State 404
   (28)  parameter_declaration_list:  parameter_declaration_list qCOMMA parameter_declaration.

   .                   reduce (28)

State 405
  (332)  offset:                     qLBRACE $331 index_and_length qRBRACE.

   .                   reduce (332)

State 406
  (333)  $333:                    .
         index_and_length:            r_expression qCOLON.$333 r_expression

   .                   reduce (333)

   $333                    goto 437

State 407
  (314)  parameter_value_list:        openoper $313 value_list closeoper.

   .                   reduce (314)

State 408
  (329)  $329:                    .
         value_list:                  r_expression qCOMMA.$329 value_list

   .                   reduce (329)

   $329                    goto 438

State 409
  (306)  is_keyword:                  qIS.

   .                   reduce (306)

State 410
         attribute_op_phrase:         qENTITYQUALIFIER $296 is_keyword.qENTITYQUALIFIER

   qENTITYQUALIFIER    shift 439
   .                   error


State 411
  (295)  attribute_op_phrase:         qENTITYQUALIFIER $294 exist_phrase.

   .                   reduce (295)

State 412
         is_null_phrase:              is_keyword.qNOT $303 qNULL
         is_null_phrase:              is_keyword.qNULL

   qNULL               shift 440
   qNOT                shift 441
   .                   error


State 413
  (293)  attribute_op_phrase:         qATTRIBUTEQUALIFIER $292 is_null_phrase.

   .                   reduce (293)

State 414
  (300)  where_op:                    qLIKE.

   .                   reduce (300)

State 415
  (299)  where_op:                    qEQUAL.

   .                   reduce (299)

State 416
  (298)  where_op:                    qCONDOPERATOR.

   .                   reduce (298)

State 417
         attribute_op_phrase:         qATTRIBUTEQUALIFIER $290 where_op.where_value

   qCONSTANT           shift 225
   qVARIABLE           shift 19
   qATTRIBUTEQUALIFIER shift 442
   qMINUS              shift 226
   .                   error

   aconstant                  goto 443
   symbol                    goto 444
   where_value                goto 445

State 418
         attribute_where_phrase:      attribute_where_phrase.qJOIN $285 attribute_where_phrase
         attribute_where_phrase:      qLPAREN $287 attribute_where_phrase.qRPAREN

   qJOIN               shift 375
   qRPAREN             shift 446
   .                   error


State 419
         attribute_where_phrase:      attribute_where_phrase qJOIN $285.attribute_where_phrase

   qATTRIBUTEQUALIFIER shift 293
   qENTITYQUALIFIER    shift 292
   qLPAREN             shift 295
   .                   error

   attribute_op_phrase        goto 294
   attribute_where_phrase     goto 447

State 420
  (221)  ITEM and lookahead TOO BIG
         attribute_where_phrase:      attribute_where_phrase.qJOIN $285 attribute_where_phrase

   qJOIN               shift 375
   .                   reduce (221)


State 421
  (181)  task_level:                  qAT qLEVEL qANY.

   .                   reduce (181)

State 422
  (180)  task_level:                  qAT qLEVEL qSYSTEM.

   .                   reduce (180)

State 423
  (179)  task_level:                  qAT qLEVEL qAPPLICATION.

   .                   reduce (179)

State 424
  (178)  task_level:                  qAT qLEVEL qTASK.

   .                   reduce (178)

State 425
  (177)  task_level:                  qAT qLEVEL qSUBTASK.

   .                   reduce (177)

State 426
  (154)  name_action:                 qNAME qTVIEW name_var task_level qUSING symbol.

   .                   reduce (154)

State 427
  (176)  view_qual:                   symbol.

   .                   reduce (176)

State 428
  (175)  view_qual:                   qCONSTANT.

   .                   reduce (175)

State 429
  (174)  view_var:                    symbol $173 qNAMED view_qual.

   .                   reduce (174)

State 430
  (151)  get_action:                  qGET qTVIEW view_var task_level qUSING symbol.

   .                   reduce (151)

State 431
  (302)  exist_phrase:                qDOES qNOT qEXIST.

   .                   reduce (302)

State 432
  (135)  multiple_condition:          lparen multiple_condition rparen conditional_join compound_condition.

   .                   reduce (135)

State 433
  (134)  multiple_condition:          lparen multiple_condition rparen conditional_join multiple_condition.

   .                   reduce (134)

State 434
  (132)  multiple_condition:          lparen compound_condition rparen conditional_join compound_condition.

   .                   reduce (132)

State 435
  (131)  multiple_condition:          lparen compound_condition rparen conditional_join multiple_condition.

   .                   reduce (131)

State 436
   (30)  parameter_declaration_list:  parameter_declaration_list error $29 parameter_declaration.

   .                   reduce (30)

State 437
         index_and_length:            r_expression qCOLON $333.r_expression

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 448
   symbol                    goto 99

State 438
         value_list:                  r_expression qCOMMA $329.value_list

   error               shift 88
   qCONSTANT           shift 89
   qVARIABLE           shift 19
   qDEFINE             shift 91
   qATTRIBUTEQUALIFIER shift 90
   qMINUS              shift 96
   qLPAREN             shift 94
   .                   error

   value_list                 goto 449
   operation_call             goto 92
   value                    goto 93
   simple_expression          goto 95
   lparen                    goto 97
   r_expression               goto 360
   symbol                    goto 99

State 439
  (297)  attribute_op_phrase:         qENTITYQUALIFIER $296 is_keyword qENTITYQUALIFIER.

   .                   reduce (297)

State 440
  (305)  is_null_phrase:              is_keyword qNULL.

   .                   reduce (305)

State 441
  (303)  $303:                    .
         is_null_phrase:              is_keyword qNOT.$303 qNULL

   .                   reduce (303)

   $303                    goto 450

State 442
  (309)  where_value:                 qATTRIBUTEQUALIFIER.

   .                   reduce (309)

State 443
  (308)  where_value:                 aconstant.

   .                   reduce (308)

State 444
  (307)  where_value:                 symbol.

   .                   reduce (307)

State 445
  (291)  attribute_op_phrase:         qATTRIBUTEQUALIFIER $290 where_op where_value.

   .                   reduce (291)

State 446
  (288)  attribute_where_phrase:      qLPAREN $287 attribute_where_phrase qRPAREN.

   .                   reduce (288)

State 447
         attribute_where_phrase:      attribute_where_phrase.qJOIN $285 attribute_where_phrase
  (286)  ITEM and lookahead TOO BIG

   .                   reduce (286)


State 448
         r_expression:                r_expression.qADDITION $248 r_expression
         r_expression:                r_expression.qMINUS $250 r_expression
         r_expression:                r_expression.qMULTIPLICATION $252 r_expression
         r_expression:                r_expression.qDIVISION $254 r_expression
  (334)  index_and_length:            r_expression qCOLON $333 r_expression.   [ qRBRACE ]

   qADDITION           shift 177
   qMINUS              shift 176
   qMULTIPLICATION     shift 175
   qDIVISION           shift 174
   .                   reduce (334)


State 449
  (330)  value_list:                  r_expression qCOMMA $329 value_list.

   .                   reduce (330)

State 450
         is_null_phrase:              is_keyword qNOT $303.qNULL

   qNULL               shift 451
   .                   error


State 451
  (304)  is_null_phrase:              is_keyword qNOT $303 qNULL.

   .                   reduce (304)


337 rules, 141 tokens, 166 variables, 452 states
Memory: max = 168K
States: 58 wasted, 1123 resets
Items: 2991, 0 kernel, (6,0) per state, maxival=957 (30 w/s)
Lalr: 101 calls, 2634 recurs, (255 trans, 6687 epred)
Actions: 715 entries, gotos: 458 entries
Exceptions: 33 states, 310 entries
List merging: 16%, Simple state elim: 16%
Error default elim: 5%
Optimizer: in 2389, out 1720
Size of tables: 4872 bytes
0 seconds, final mem = 36K
