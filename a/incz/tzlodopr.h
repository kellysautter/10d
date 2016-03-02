/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzlodops.h -  Zeidon application operation prototypes
// DESCRIPTION:   This C file contains global and LOD operation prototypes
//                for the Zeidon application.
//
// **************************************************************************
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// **************************************************************************
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.08.29  DGC
   Added an argument to ParseSource().
2002.08.20 RR
   add defines for zeidon online manuals
2002.05.08 HH
   added qq_reset().
2002.02.19 HH
   added TZVSRTOO.
2001.10.12 HH
   tzvmi.. functions handle return code from operation.
2001.09.26 HH
   changes for WHERE with parens.
2001.09.19 HH
   added NewCreateOperationCallUsingAQ, NewAddGetOrSetAttributeWithString,
   NewAddGetOrSetAttribute.
2001.08.27 HH
   added PrintXPG.
2001.08.10 HH
   R55612, change prototype SetPointerValues, WRKS system.
2001.07.13 RG
   added operation: VerifyDir
2001.06.24 BL
   add new function oTZCMCPRO_DeleteMetaWithCluster,
   oTZCMCPRO_DeleteFileWithCluster and oTZCMCPRO_DeleteDependentsWithCluster
   Delete function oTZCMREPO_InitBaseSampleShadow
2001.02.25 BL
   change prototype of LOD_DialogListBuildControl for revise ERD Cross
   Reference
2001.02.15 HH
   add new function oTZTENVRO_SwitchForeignKeys
2001.02.13 HH
   Parser/Grammar change.
2000.01.02 HH
   added GetVMLConstChar.
2000.11.20 HH
   change prototype of oTZWDLGSO_WndEventName (compiler warning).
2000.09.21 HH
   added FormatString.
2000.09.17 HH
   do not show generated operations in list, AddOperation parameter
2000.09.14  HH
   removed GetOperationData.
2000.08.08  HH  Z2000
   Added #define eQCOMPLEXEXPR and #define eQSTRINGCONCATPAREN
2000.08.08  HH  Z2000
   Added NewAddExpressionEntryToString.
2000.08.08  RG  Z2000
   Removed static function of TZWDLGSO.
   Changed parameters of the last external function of TZWDLGSO
2000.07.1 BL
   Added defines for TE-Tool
2000.07.18  HH
   removed definition for RereadStatement.
2000.06.22  BL
   Added defines for PPE-Tool
2000.02.23  BL,HH
   remove CheckDuplicateName, defined in ZEIDONOP.H
2000.02.23  BL
   Added zwTZCMLPLO_LoadRefreshListForCPLR
2000.02.14  HH
   remove fnOptNumExpr, fnOptTextExpr,
   CreateSourceZeidonStrOpS,CreateSourceZeidonStrOp,
   CreateSourceZeidonStrOpWithID,CreateSourceZeidonStrOpsWithID
   CreateTargetZeidonStrOp, CreateTargetZeidonStrOpS,
   CreateTargetZeidonStrOpWithID, CreateTargetZeidonStrOpSWithID,
   ExtractOperationCallWithView
2000.01.10   BL  Z10 Repository
   Insert new Parameter in oTZCMLPLO_CopyFileToServerFile for Viewcluster
1999.10.06   BL  Z10
   insert #define zTOOL_BROWSER 15 for AboutBox in the
   Object Browser

from here old version

22.04.1997 DonC
   Added operation AddGetVariableFromAttribute.

13.05.1997 DonC
   Added szActionText parameter to CreateButtonAct... operations so that
   Cancel and Close actions would be named ESC and OK and Exit actions would
   be named Alt-F4.

13.11.1997 DonC
   Added SyntaxError message 548.

16.11.1997 DonC
   Added define of qTDATE.

15.02.1998 DonC
   Added SyntaxError messages 549 AND 550.

1998.07.14 DGC
   Removed SourceFileClose().

17.07.1998  HH
   Modified GetOperationParmListFromSource, added parameter sParmListExt.
   added function prototype for NewAddExpressionEntryToView.
   added nFlags to sQExprStruct.

16.10.1998  HH
   prototype GetOperationParmListPM changed (TB363).

26.10.1998  HH
   prototype CaseExpression added (TB369).

17.12.1998  HH
   added NewGenerateTempVariable.

30.12.1998  HH
   remove AnalyzeAssignmentStatement, add GermanUmlautHack.

16.02.1999  HH
   SyntaxError message eQINVALIDSYNTAXINWHERE.

1999.04.19  RS
   Commentlines required for Rose.

*/

// Don't remove the following commentlines. They are required for Rose.
//#include <kzoep1aa.h> //required for Rose

#ifndef TZLODOPR_H
#define TZLODOPR_H

#ifndef RC_INVOKED
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#endif

#define  KZMETA_INCL
#define  KZSYSSVC_INCL
#include <kzoengaa.h>
#include <zdrvropr.h>

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS  5 // remove "This function or variable may be unsafe." warnings
#endif

// #pragma message("In TZLODOPR.H ===> " VAR_NAME_VALUE(_CRT_SECURE_NO_WARNINGS))

#ifdef __cplusplus
extern "C"
{
#endif


#define STOP IssueError( lpGlobalSubtask, 0, 0, "Stopped" );
#define STOPS(s) IssueError( lpGlobalSubtask, 0, 0, s );


#define  BUFSIZE  32765   /*  leave room for EOF and terminating null */
#define  MAXPATTERN 256   /*  max # characters in find string         */
#define  ASCIISIZE  256   /*  # characters in extended ASCII set      */
#define  MAXLINE    512   /*  max length of line (from CRLF to CRLF)  */
#define  MAXSTRING 1024

/* Convert Characters to Dlg units. */
#define  mCCX( a ) ( ((a) << 4 ) / gnDlgUnitCharX )
#define  mCCY( a ) ( ((a) << 6 ) / gnDlgUnitCharY )

//typedef long far *zPLONG;

/*  Arbitrarily large constant for FAST/SLOW loop of Boyer-Moore.     */
#define  LARGE   999999

/* Zeidon application message defines */
#define zTOOL_SYSADM   1
#define zTOOL_WKSADM   2
#define zTOOL_DOMAIN   3
#define zTOOL_ERD      4
#define zTOOL_DTE      5
#define zTOOL_LOD      6
#define zTOOL_DIALOG   7
#define zTOOL_UIS      8
#define zTOOL_PENV     9
#define zTOOL_VOR     10
#define zTOOL_GO      11
#define zTOOL_HDR     12
#define zTOOL_DEBUG   13
#define zTOOL_REPORT  14
#define zTOOL_BROWSER 15

#define  zENAME_WINDOW           3000
#define  zENAME_ER_ENTITY        3001
#define  zENAME_ER_ATTRIBUTE     3002
#define  zENAME_ER_RELATIONSHIP  3003
#define  zENAME_LOD_ENTITY       3004
#define  zENAME_LOD_ATTRIBUTE    3005
#define  zENAME_LOD_RELATIONSHIP 3006
#define  zENAME_DOMAIN_CONTEXT   3007
#define  zENAME_ER_IDENTIFIER    3008
#define  zENAME_DLG_MENU         3009
#define  zENAME_PPE_WINDOWSTYLE  3010
#define  zENAME_PPE_COLOROPTION  3011
#define  zENAME_PPE_FONTOPTION   3012
#define  zENAME_PPE_STYLEEVENT   3013
#define  zENAME_PPE_CTRLEVENT    3014
#define  zENAME_PPE_CTRLMAPDEF   3015
#define  zENAME_PPE_CTRLFONT     3016
#define  zENAME_PPE_CTRLCOLOR    3018
#define  zENAME_DTE_DATASOURCE   3019
#define  zENAME_DTE_TABLE        3020
#define  zENAME_DTE_KEY          3021
#define  zENAME_DOMAIN_TABLEITEM 3022

#define zTOOLMSG_SAVE      100
#define zTOOLMSG_DELETE    101
#define zTOOLMSG_REMOVE    102
#define zTOOLMSG_INACTIVE  103

#define zXC_NOT_LOADED              0x00010000
#define zXC_OPERATION_NOT_SELECTED  0x00010000
#define zXC_MODULE_NOT_SELECTED     0x00010000
#define zXC_OPERATION_EXECUTED      0x00000000
#define zXC_STEP_EXECUTED           0x00010000
#define zXC_OPERATION_SELECTED      0x00010000
#define zXC_SETUP_DEBUGGER          0x00020000
#define zXC_OPERATION_STAGED        0x00030000

/* Zeidon Online Manual Defines */
#define  SYSTEM_HELP     "SystemHelp"
#define  BASICS_MANUAL   "BasicsManual"
#define  USERS_GUIDE     "UsersGuide"
#define  PROG_REF        "VMLReference"
#define  OPER_REF        "OperationReference"

// Defines for Action entity SB_SubAction values
#define zSubActionAccept               1
#define zSubActionCancel               2
#define zSubActionDelete               4
#define zSubActionExclude              8
#define zSubActionCreateEntity         16
#define zSubActionCreateTemporalEntity 32
#define zSubActionCreateTemporalVer    64

// valid ParseAction Parms
#define zPARSE_ACTION_FULL           2
#define zPARSE_ACTION_INCREMENTAL    1
#define zPARSE_ACTION_OPERATION_ONLY 1

// valid ParseLimit Parms
#define zPARSE_LIMIT_SIGNATURES      1
#define zPARSE_LIMIT_OPERATION       2
#define zPARSE_LIMIT_SOURCE          3

// valid ParseSource Parms
#define zPARSE_SOURCE_FILE           1
#define zPARSE_SOURCE_BUFFER         2
#define zPARSE_SOURCE_XPG            3

// valid ParseSource lControl values.
#define zPARSE_DONOTWRITEXPG         0x00000001

#define qTPOINTER 1200

//typedef zSHORT   (FAR PASCAL *FARPROC)();
#ifndef __OS2__
typedef zCHAR    (FAR PASCAL *FARPROCC)();
typedef zSHORT   (FAR PASCAL *FARPROCS)();
typedef zLONG    (FAR PASCAL *FARPROCL)();
typedef zDECIMAL (FAR PASCAL *FARPROCLD)();
typedef zPVOID   (FAR PASCAL *FARPROCP)(); // used for all pointers and zVIEW
#endif
#define ORDINALID(k)   (zPCHAR)((zLONG)((zSHORT)(k)))


#define qAFTER                1002
#define qEACH                 1003
#define qTATTRIBUTENAME       1004
#define qBEFORE               1005
#define qBASED                1006
#define qCASE                 1007
#define qCONDOPERATOR         1009
#define qCONSTANT             1010
#define qCREATE               1011
#define qCURSOR               1012
#define qCOMMIT               1013
#define qDELETE               1014
#define qELSE                 1015
#define qEND                  1016
#define qENTITY               1017
#define qTENTITYNAME          1018
#define qTDATE                1019
#define qFIRST                1020
#define qFOR                  1021
#define qIF                   1022
#define qJOIN                 1024
#define qLAST                 1025
#define qACTIVATE             1026
#define qLOOP                 1027
#define qLOD                  1028
#define qFORIF                1029
#define qNEXT                 1030
#define qOF                   1032
#define qOPERATION            1033
#define qOPERATOR             1034
#define qPREVIOUS             1035
#define qRETURN               1036
#define qRETURNEND            1037
#define qSET                  1038
#define qTDECIMAL             1039
#define qTINTEGER             1040
#define qTSHORT               1041
#define qTSTRING              1042
#define qUNTIL                1043
#define qVARIABLE             1044
#define qTVIEW                1045
#define qWHERE                1046
#define qWHILE                1047
#define qWITHIN               1048
#define qOTHERWISE            1049
#define qON                   1050
#define qAND                  1051
#define qDEFINE               1060
#define qBREAK                1059
#define qRETURNED             1058
#define qDOMAIN               1057
#define qDIALOG               1056
#define qOBJECT               1055
#define qLOCAL                1054
#define qGLOBAL               1053
#define qOR                   1052
#define qRESTRICTING          1061
#define qTO                   1062
#define qEMPTY                1067
#define qLIKE                 1068
#define qMULTIPLE             1063
#define qROOTONLY             1064
#define qSINGLE               1065
#define qROOTONLYMULTIPLE     1066
#define qEXISTS               1069
#define qEXIST                1070
#define qDOES                 1071
#define qTCHARACTER           1072
#define qIS                   1073
#define qNULL                 1074
#define qATTRIBUTE            1075
#define qDERIVED              1076
#define qCONSTRAINT           1077
#define qTRANSFORMATION       1078
#define qSHORTCUTKEY          1079
#define qREGISTERED           1080
#define qAS                   1081
#define qGET                  1082
#define qAT                   1083
#define qLEVEL                1084
#define qSUBTASK              1085
#define qTASK                 1086
#define qAPPLICATION          1087
#define qSYSTEM               1088
#define qANY                  1089
#define qNAME                 1090
#define qUSING                1091
#define qINCLUDE              1092
#define qEXCLUDE              1093
#define qFROM                 1094
#define qNONE                 1095
#define qNAMED                1096
#define qTUNKNOWN             1100
#define qTVOID                1101
#define qCONTINUE             1102
#define qROOTONLYCONTINUE     1103
#define qSINGLEFORUPDATE      1104
#define qATTRIBUTEQUALIFIER   2001
#define qENTITYQUALIFIER      2002
#define qRESERVEDWORD         2003
#define qUSERDEFINED          2004
#define qDELIMITER            2005
#define qEXPRESSION           2006
#define qENDFUNC              2007
#define qOPERATIONCALL        2008
#define qNUMERIC              2009
#define qTEXT                 2010
#define qEQUAL                3001
#define qMINUS                3002
#define qADDITION             3003
#define qMULTIPLICATION       3004
#define qDIVISION             3005
#define qCONDEQUAL            3006
#define qLESSTHANEQUAL        3007
#define qLESSTHAN             3008
#define qGREATERTHANEQUAL     3009
#define qGREATERTHAN          3010
#define qNOTEQUAL             3011
#define qNOT                  3012
#define qCOMMA                3013
#define qLPAREN               3014
#define qRPAREN               3015
#define qLBRACE               3016
#define qRBRACE               3017
#define qOPENOPER             3018
#define qCLOSEOPER            3019
#define qSEMICOLON            3020
#define qCOLON                3021
#define qNOTEXISTS            3022
#define qNOTNULL              3023
#define qPERIOD               3024
#define qUNKNOWNTOKEN         3031
#define qUNARYMINUS           3032
#define qLOCALOPERATION       4001
#define qGLOBALOPERATION      4002
#define qOBJECTOPERATION      4003
#define qDIALOGOPERATION      4004
#define qDOMAINOPERATION      4005
#define qZEIDONOPERATION      4006
#define qCONTROLOPERATION     4007
#define qBOTHVAR              5001
#define qCONCATOPERATOR       5002

typedef struct sQTokenStruct
{
   zLONG lLastTokenClass;
   zLONG lLastToken;
   zLONG lLastLineNo;
   zCHAR szLastToken[ MAXSTRING ];

   zLONG lTokenClass;
   zLONG lToken;
   zLONG lLineNo;
   zCHAR szToken[ MAXSTRING ];

} sQTokenStruct;

typedef struct fQExprStruct  // full expression structure
{
zLONG lStructSize;

   zLONG lElementClass;
   zLONG lDataClass;
   zLONG lDataType;
   zLONG lOffset;
   zLONG lLevel;
   zLONG lCount;
   zLONG lFlags; // *+*
   zCHAR szText[ MAXSTRING + 1 ];

unsigned char CheckByte;
} fQExprStruct;

typedef struct sQExprStruct  // must be in sync with ExprNode Entity of TZVSEXPO.LOD
{                            // to check, use: qstruct tzvsexpo.xod -otzvsexpo.hst
   zLONG lElementClass;
   zLONG lDataClass;
   zLONG lDataType;
   zLONG lOffset;
   zLONG lLevel;
   zLONG lCount;
   zLONG lFlags; // *+*
// definitions for lFlags
#define  POINTER_BIT  0x00000001   // future use
#define  UNSIGNED_BIT 0x00000002
#define  SKIP_BIT     0x00010000
#define  QUALVIEW_BIT 0x00020000
#define  RETVIEW_BIT  0x00040000
#define  MUTABLE_BIT  0x00100000
#define  TESTVIEW_BIT 0x00200000
// for nParmType use the next 3 bits , mask = 0x0000001C
#define  NPARM_TYPE_CHAR    0x00000000
#define  NPARM_TYPE_LONG    0x00000004
#define  NPARM_TYPE_SHORT   0x00000008
#define  NPARM_TYPE_DECIMAL 0x0000000C
#define  NPARM_TYPE_VIEW    0x00000010
#define  NPARM_TYPE_DATE    0x00000014
#define  NPARM_TYPE_VOID    0x00000018
#define  NPARM_TYPE_MASK    0x0000001C

   zCHAR szText[ MAXSTRING ];
} sQExprStruct;

typedef struct sQOperStackStruct
{
   zLONG lOffset;
   zLONG lLevel;
} sQOperStackStruct;

typedef sQExprStruct * LPQEXPRSTRUCT;

#define  aQADDOPERATIONBYNAME              100
#define  aQSETLASTDATATYPEINTEGER          103
#define  aQSETLASTDATATYPEDECIMAL          104
#define  aQSETLASTDATATYPESTRING           105
#define  aQSETLASTDATATYPEENTITYNAME       106
#define  aQSETLASTDATATYPEATTRIBUTENAME    107
#define  aQDECLARELOCALVARIABLE            108
#define  aQADDVIEWVARIABLE                 109
#define  aQLOADDEFASINSTANCE               111
#define  aQSETSTRINGDEFINEDLENGTH          113
#define  aQINSERTIFSTMT                    114
#define  aQINSERTELSESTMT                  115
#define  aQINSERTENDSTMT                   116
#define  aQINSERTUNTILSTMT                 117
#define  aQSETNEXTINLOOP                   118
#define  aQINSERTWHILESTMT                 119
#define  aQINSERTLOOPSTMT                  120
#define  aQDELETELOOPSTMT                  121
#define  aQINSERTFORNOWHERE                122
#define  aQINSERTFORWITHWHERE              123
#define  aQDELETELOOPFORSTMT               124
#define  aQSETLOOPFORALLINDICATOR          125
#define  aQSETLOOPFORVARIABLEINDICATOR     126
#define  aQSETLOOPFORCONSTANTINDICATOR     127
#define  aQSETLOOPFORNEXTINDICATOR         128
#define  aQINSERTCASESTMT                  129
#define  aQSETCURSORNOWHERE                130
#define  aQSETCURSORWITHWHERE              131
#define  aQADDCREATESTMTAFTER              132
#define  aQADDCREATESTMTBEFORE             133
#define  aQADDDELETESTMT                   135
#define  aQVERIFYLOADOBJECT                136
#define  aQADDLOADSTMT                     137
#define  aQINSERTOPERATIONCALL             138
#define  aQADDASSIGNMENTOPERATOR           139
#define  aQINSERTEXPRESSIONSTMT            140
#define  aQADDOPERATIONNAMETOEXPR          141
#define  aQADDOCEATOEXPRASTARGET           142
#define  aQADDNOTOPERATORTOEXPR            143
#define  aQADDVARIABLEASTARGET             144
#define  aQADDCONDOPERATORTOEXPR           145
#define  aQADDCONDEQUALTOEXPR              146
#define  aQCHECKSTOREDOCEA                 147
#define  aQADDJOINTOEXPR                   148
#define  aQADDLPARENTOEXPR                 149
#define  aQADDRPARENTOEXPR                 150
#define  aQADDVARIABLEASCASEQUALIFIER      151
#define  aQADDAQASCASEQUALIFIER            152
#define  aQINSERTOFSTMT                    153
#define  aQSETFIRSTCURSOR                  154
#define  aQSETNEXTCURSOR                   155
#define  aQSETPREVIOUSCURSOR               156
#define  aQSETLASTCURSOR                   157
#define  aQSETUPWHERE                      158
#define  aQADDWHEREJOIN                    159
#define  aQINCREMENTWHEREINDEX             160
#define  aQADDWHERECONDOPER                161
#define  aQADDWHERECONDEQUAL               162
#define  aQADDWHEREVARIABLE                163
#define  aQADDWHERECONSTANT                164
#define  aQADDOPERATORTOEXPR               167
#define  aQADDVARIABLETOEXPR               168
#define  aQADDAQTOEXPR                     169
#define  aQSAVELASTVARIABLE                170
#define  aQSETORIGIN                       171
#define  aQADDCONSTANTTOEXPR               172
#define  aQMARKLASTVARASPARM               173
#define  aQADDRETURNSTATEMENT              174
#define  aQADDLBRACETOEXPR                 175
#define  aQADDRBRACETOEXPR                 176
#define  aQADDCOMMATOEXPR                  177
#define  aQINSERTOTHERWISESTMT             178
#define  aQSETCURSORCONTROL                179
#define  aQSETSCOPE                        180
#define  aQSETCURSORLASTCONTROL            181
#define  aQSETEACHFLAG                     182
#define  aQSETNOEACHFLAG                   183
#define  aQSETVARIABLEFLAG                 184
#define  aQSETCONSTANTFLAG                 185
#define  aQADDFORNOWHERE                   186
#define  aQADDFORWITHWHERE                 187
#define  aQADDWHERELEFTAQ                  188
#define  aQADDWHERERIGHTAQ                 189
#define  aQADDVARIABLEOFFSETTOEXPR         190
#define  aQADDVARIABLEOFFSETASTARGET       191
#define  aQADDCOLONTOEXPR                  193
#define  aQADDOPENOPERTOEXPR               194
#define  aQADDCLOSEOPERTOEXPR              195
#define  aQSETNEXT                         196
#define  aQADDLONEDEFAULT                  197
#define  aQEXTRAENDNEEDED                  198
#define  aQSETOPERATIONTYPE                199
#define  aQMARKLASTVARASPARMPTR            200
#define  aQADDACTIVATEWHERE                201
#define  aQADDACTIVATESTMT                 202
#define  aQVERIFYACTIVATEOBJECT            203
#define  aQSETRESTRICTION                  204
#define  aQSAVEACTIVATESINGLE              205
#define  aQSAVEACTIVATEMULTIPLE            206
#define  aQSAVEACTIVATEROOTONLY            207
#define  aQADDACTIVATESTMTNOWHERE          208
#define  aQADDRESTRICTACTIVATEWHERE        209
#define  aQSAVEACTIVATEROOTONLYMULTIPLE    210
#define  aQINSERTOPERATIONENDSTMT          211
#define  aQBEGINPARMLIST                   212
#define  aQENDPARMLIST                     213
#define  aQADDACTIVATESTMTEMPTY            214
#define  aQADDWHERELIKE                    215
#define  aQADDWHERELEFTEQ                  216
#define  aQADDWHEREEXISTS                  217
#define  aQADDWHERENOTEXIST                218
#define  aQSETLASTDATATYPESHORT            219
#define  aQSETLASTDATATYPECHARACTER        220
#define  aQADDWHEREIS                      221
#define  aQADDWHERERIGHTEQ                 222
//unused aQCONFIRMNOT                      223
#define  aQADDWHEREISNOTNULL               224
#define  aQADDWHEREISNULL                  225
#define  aQTURNOFFTEXTSAVE                 226
#define  aQLOADDEFFROMVOR                  227
#define  aQSAVEGETVIEWNAME                 228
#define  aQADDGETVIEW                      229
#define  aQADDGETVIEWATLEVEL               230
#define  aQADDGETVIEWATLEVELU              231
#define  aQADDGETVIEWATLEVELFROMLEVEL      232
#define  aQSAVENAMEVIEWNAME                233
#define  aQSAVENAMEVIEWCONSTANT            234
#define  aQSAVENAMEVIEWVARIABLE            235
#define  aQADDNAMEVIEW                     236
#define  aQADDNAMEVIEWWITHVIEW             237
#define  aQSETINCLUDETARGET                238
#define  aQSETINCLUDESOURCE                239
#define  aQADDINCLUDE                      240
#define  aQADDINCLUDEBEFORE                241
#define  aQADDEXCLUDE                      242
#define  aQADDEXCLUDEBEFORE                243
#define  aQADDEXCLUDENONE                  244
#define  aQADDCHECKEXIST                   245
#define  aQADDWHERELEFTEQCOND              246
#define  aQSAVEGETVIEWID                   247
#define  aQSAVEGETVIEWCONSTANT             248
#define  aQADDCOMMIT                       249
#define  aQADDDEFINETOEXPR                 250
#define  aQADDCREATESTMTFIRST              251
#define  aQADDCREATESTMTLAST               252
#define  aQADDDELETESTMTFIRST              253
#define  aQADDDELETESTMTLAST               254
#define  aQADDDELETESTMTPREVIOUS           255
#define  aQADDDELETESTMTNONE               256
#define  aQADDDELETESTMTNEXT               257
#define  aQSAVEACTIVATECONTINUE            258
#define  aQSAVEACTIVATEROOTONLYCONTINUE    259
#define  aQSAVEACTIVATESINGLEFORUPDATE     260
#define  aQADDUNARYOPERATOR                261
#define  aQSETVARIABLETEXT                 262
#define  aQSETOBJECTSCOPE                  263
#define  aQSETWHERELEFTPAREN               264
#define  aQSETWHERERIGHTPAREN              265

#define  eQOPERATIONALREADYDEFINED         501
#define  eQNOOPERATIONNAMEGIVEN            502
#define  eQNOOPERATIONSDEFINED             503
#define  eQVARIABLEALREADYDEFINED          504
#define  eQNOVARIABLENAMEGIVEN             505
#define  eQVARIABLEUNDEFINED               506
#define  eQVIEWUNDEFINED                   507
#define  eQVIEWNOTDEFINEDASVIEW            508
#define  eQENTITYUNDEFINEDASVAR            509
#define  eQENTITYNOTDEFINEDINOJECT         510
#define  eQATTRIBUTEUNDEFINEDASVAR         511
#define  eQATTRIBUTENOTDEFINEDINOJECT      512
#define  eQPARAMETERSNOTDELIMITED          513
#define  eQDECLMUSTPRECEEDSTMTS            514
#define  eQNODIMENSIONFORSTRING            515
#define  eQNOVIEWNAMEGIVEN                 516
#define  eQNOVIEWDEFGIVEN                  517
#define  eQSTMTSCANTFOLLOWRETURN           518
#define  eQINVALIDFODPHRASE                519
#define  eQNOCONDITIONAFTERIF              520
#define  eQNOENDFORIFSTMT                  521
#define  eQUNTILNEEDSCONDITION             522
#define  eQWHILENEEDSCONDITION             523
#define  eQWHILECLAUSENEVERENDED           524
#define  eQCASENEEDSAVALUETOTEST           525
#define  eQCASENEVERENDED                  526
#define  eQOFCANTFOLLOWOTHERWISE           527
#define  eQINVALIDCONDEXPRESSION           528
#define  eQINCOMPATIBLETYPES               529
#define  eQOPERATIONTYPENOTGIVEN           530
#define  eQLODCOULDNTBELOADED              531
#define  eQBADTOKENWHEREVALUEEXPECTED      532
#define  eQDEFINECANTBEINDEXED             533
#define  eQONLYSTRINGSCANBEINDEXED         534
#define  eQDEFINEPUTWHEREVARIABLEMUSTBE    535
#define  eQTOOFEWPARAMETERS                536
#define  eQTOOMANYPARAMETERS               537
#define  eQINVALIDSYNTAX                   538
#define  eQUNKNOWNOPERATION                539
#define  eQVARIABLENOTSTRINGTYPE           540
#define  eQATTRIBUTEQUALIFIERUNDEFINED     541
#define  eQVORCOULDNTBELOADED              542
#define  eQINVALIDVIEWDECLARATION          543
#define  eQINVALIDTARGETSTRING             544
#define  eQNOVIEWINPARMLIST                545
#define  eQINVALIDARITHMETICEXPRINIF       546
#define  eQTOOMANYEXPRSINIFSTATEMENT       547
#define  eQINVALIDSTRINGCONSTANT           548
#define  eQINVALIDLEFTWHEREAQ              549
#define  eQINVALIDSYNTAXINWHERE            550
#define  eQSTRINGCONCATPAREN               551
#define  eQCOMPLEXEXPR                     552
#define  eQINVALIDFUNCTIONPARAM            553
#define  eQINVALIDFUNCTIONDEF              554
#define  eQINVALIDCONSTANTDEF              555
#define  eQINVALIDUNARYOPERATOR            556
#define  eMULTIPLERETURNARGUMENTS          557
#define  eQUALIFYINGVIEWUNDEFINED          558

#define  dKZPARSEFROMFILE         0
#define  dKZPARSEFROMPTR          1

#define  VIEWUNDEFINED           -1
#define  VIEWNOTDEFINEDASVIEW    -2
#define  INVALIDENTITY           -3
#define  INVALIDATTRIBUTE        -4

#define  VALIDVIEWUNKNOWN           1
#define  VALIDVIEW                  2
#define  VALIDENTITYUNKNOWN         3
#define  VALIDENTITY                4
#define  VALIDATTRIBUTEUNKNOWN      5
#define  VALIDATTRIBUTE             6
#define  VALIDVIEWTEMPORARY         7
#define  qNOEACH                  100

#define  qENTITYISUNKNOWN                1
#define  qENTITYISCONSTANT               2
#define  qATTRIBUTEISUNKNOWN             3
#define  qATTRIBUTEISCONSTANT            4
#define  qENTITYANDATTRIBUTEAREUNKNOWN   5
#define  qENTITYANDATTRIBUTEARECONSTANT  6

#define zPARSE_ACTION_FULL           2
#define zPARSE_ACTION_INCREMENTAL    1
#define zPARSE_ACTION_OPERATION_ONLY 1

#define zPARSE_LIMIT_SIGNATURES 1
#define zPARSE_LIMIT_OPERATION  2
#define zPARSE_LIMIT_SOURCE     3

#define zPARSE_SOURCE_FILE   1
#define zPARSE_SOURCE_BUFFER 2
#define zPARSE_SOURCE_XPG    3

#define qTYPEEXACTMATCH        2
#define qCLASSEXACTMATCH       1
#define qSETTARGETTOSOURCE     0
#define qNOMATCHONTYPE        -1

#define zCOMP_TYPE_MSVC15 1
#define zCOMP_TYPE_IBMC20 2

#define PARM_LIST_SIZE 20

#define WHERE_ARRAY_SIZE 30

/////////////////////////////////////////////////////////////////////////////
//
// global and LOD operation function prototypes
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
//  tzlooprs.c
//
/////////////////////////////////////////////////////////////////////////////

// type defs

// global operations
zOPER_EXPORT zSHORT OPERATION
GetExpressionStruct( sQExprStruct *psExprStruct,
                     zVIEW  vSourceView,
                     zPCHAR pchEntity );

zOPER_EXPORT zSHORT OPERATION
FindCompilerType( zVIEW  vTaskLPLR );

zOPER_EXPORT zSHORT OPERATION
CreateMakefileForAllTargets( zVIEW  vSubtask,
                             zVIEW  vSrcTaskLPLR,
                             zPCHAR pchCompilerSpec,
                             zBOOL  bBuildAllTargets,
                             zPCHAR pchReturnCommand,
                             zLONG  lMaxCommandLth,
                             zCPCHAR cpcGenLang );
zOPER_EXPORT zSHORT OPERATION
CreateMakefileForTarget( zPCHAR pchTargetName,
                         zVIEW  vSubtask,
                         zVIEW  vSrcTaskLPLR,
                         zPCHAR pchCompilerSpec,
                         zBOOL  bBuildAllTargets,
                         zPCHAR pchReturnCommand,
                         zLONG  lMaxCommandLth,
                         zCPCHAR cpcGenLang );
zOPER_EXPORT zSHORT OPERATION
DeleteOperationFromSourceFile( zVIEW  vSubtask,
                               zPCHAR pchOperName,
                               zPCHAR pchFileName );

zOPER_EXPORT zSHORT OPERATION
LOD_DialogListSub( zVIEW  ViewToWindow,
                   zPCHAR pchSuffix,
                   zVIEW  vTaskLPLR,
                   zVIEW  vXferLOD,
                   zVIEW  vERD );

zOPER_EXPORT zSHORT OPERATION
LOD_DialogListActivateMeta( zVIEW  ViewToWindow,
                            zVIEW  vTaskLPLR,
                            zPVIEW vMeta,
                            zPCHAR pchType,
                            zPCHAR pchMetaName,
                            zPCHAR pchSuffix );

zOPER_EXPORT zSHORT OPERATION
LOD_DialogListBuildControl( zVIEW  ViewToWindow,
                            zVIEW  vDialog,
                            zVIEW  vXferLOD );

zOPER_EXPORT zSHORT OPERATION
FindTextInAttribute( zPCHAR pchTextValue,
                     zVIEW  vView,
                     zPCHAR pchEntityName,
                     zPCHAR pchAttributeName );

zOPER_EXPORT zSHORT OPERATION
GetVMLConstChar( void );

zOPER_EXPORT zBOOL OPERATION
VerifyDir( zVIEW  vSubtask,
           zPCHAR pchDir,
           zPCHAR pchControlName,
           zBOOL  bCreate );

zOPER_EXPORT zSHORT OPERATION
fnSetupEventList( zVIEW vSubtask, zVIEW vDialog );

zOPER_EXPORT zSHORT OPERATION
fnSetupWndEventList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
fnCreateEmptyActionView( zVIEW vSubtask, zVIEW vDialog );

/////////////////////////////////////////////////////////////////////////////
//
//  tz__prfo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZ__PRFO_CommitProfile( zVIEW  vTZ__PRFO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzlodopr.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
IssueToolMsg( zVIEW   vSubtask,
              zSHORT  nStdMsg,
              zSHORT  nTool,
              zSHORT  nObject,
              zVIEW   vObject,
              zCPCHAR cpcEntityName,
              zCPCHAR cpcAttributeName,
              zCPCHAR cpcContextName );
zOPER_EXPORT zSHORT OPERATION
IssueToolAbout( zVIEW   vSubtask,
                zSHORT  nTool,
                zCPCHAR cpcFile,
                zCPCHAR cpcDate,
                zCPCHAR cpcTime );
zOPER_EXPORT zSHORT OPERATION
zgSysGetTimestamp( zPCHAR pchTimestamp );
zOPER_EXPORT zSHORT OPERATION
zgSysGetTimestampFromFFB( zPCHAR pchTimestamp, HFILE hFile );
zOPER_EXPORT zLONG OPERATION
SourceFileOpenForRead( zVIEW lpTaskView, zPCHAR pchFileName );

//DGC 7.14.98
//zOPER_EXPORT zSHORT OPERATION
//SourceFileClose( zLONG hFile );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadcs2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCS2O_DS_EntityRelink( zVIEW  vDSR,
                           zVIEW  vLOD );
zOPER_EXPORT zSHORT OPERATION
oTZADCS2O_DialogSpecEntityCopy( zVIEW  SourceLPLR,
                                zVIEW  OldDS,
                                zVIEW  OldDS_C,
                                zVIEW  NewDS,
                                zVIEW  NewDS_C,
                                zVIEW  vPE,
                                zVIEW  MapLOD );
zOPER_EXPORT zSHORT OPERATION
oTZADCS2O_DialogSpecGetVOR_LOD( zVIEW  OldDS,
                                zPVIEW VOR,
                                zPVIEW vLOD,
                                zPCHAR pchLOD_Name,
                                zPCHAR pchViewName,
                                zPCHAR pchOrigViewEntityName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadcsdo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DeriveKey( zVIEW  vTZADCSDO,
                     LPVIEWENTITY lpViewEntity,
                     LPVIEWATTRIB lpViewAttrib,
                     zSHORT nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DerivedAttr( zVIEW  vTZADCSDO,
                       LPVIEWENTITY lpViewEntity,
                       LPVIEWATTRIB lpViewAttrib,
                       zSHORT nMsg );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw1aw.c
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignGroupboxL( zVIEW    vSubtask,
                               zVIEW    vUSSubEnt,
                               zVIEW    vNewDialog,
                               zVIEW    vDefaults,
                               zPCHAR   pchMarginType,
                               zPCHAR   cListType );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateButtonActOper( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zVIEW    vNewDialogRoot,
                               zVIEW    vDefaults,
                               zPCHAR   pchButtonText,
                               zPCHAR   pchActionText,
                               zPCHAR   pchOperationName,
                               zLONG    nButtonNbr,
                               zPCHAR   pchButtonGroupType,
                               zLONG    nActionType );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateButtonActObject( zVIEW    vSubtask,
                                 zVIEW    vUserSpec,
                                 zVIEW    vNewDialog,
                                 zVIEW    vNewDialogRoot,
                                 zVIEW    vDefaults,
                                 zVIEW    vObject,
                                 zPCHAR   pchButtonText,
                                 zPCHAR   pchActionText,
                                 zLONG    nButtonNbr,
                                 zPCHAR   pchButtonGroupType,
                                 zLONG    nActionType,
                                 zLONG    nSubActionType,
                                 zPCHAR   pchNextPrev );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase5Attrs( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEnt,
                                zVIEW  vNewDialog,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AddActionDoubleClick( zVIEW    vSubtask,
                                zVIEW    vNewDialog,
                                zVIEW    vNewDialogRoot );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase1( zVIEW  vSubtask,
                           zVIEW  vUserSpec,
                           zVIEW  vUSSubEnt,
                           zVIEW  vNewDialog,
                           zVIEW  vDefaults );

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignGroupboxM( zVIEW    vSubtask,
                               zVIEW    vUSSubEnt,
                               zVIEW    vNewDialog,
                               zVIEW    vDefaults,
                               zVIEW    vAutoDesWk,
                               zPCHAR   pchMarginType );

zOPER_EXPORT zLONG OPERATION
oTZWDLGSO_GetNbrOfGroupboxes( zVIEW    vNewDialog );

zOPER_EXPORT zLONG OPERATION
oTZWDLGSO_ResetScopeToObject( zVIEW    vNewDialog );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignListWindow( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vNewDialog,
                                zVIEW  vDefaults );

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDsgnWndAndSubs( zVIEW vSubtask,
                                zVIEW vUserSpec,
                                zVIEW vNewDialog,
                                zVIEW vDefault,
                                zVIEW vAutoDesWk);

zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SetGroupboxSize( zVIEW    vNewDialog,
                           zVIEW    vDefaults );

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_LoadStyleInfo( zVIEW vSubtask, zVIEW vUserSpec, zVIEW vNewDialog );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddStyleEssentials( zVIEW vSubtask, zVIEW vUserSpec );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateWndDesign( zVIEW vSubtask, zVIEW vUserSpec );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateMaintAttribs( zVIEW vSubtask, zVIEW vUserSpec );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateListAttribs( zVIEW vSubtask, zVIEW vUserSpec );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_UpdtExistStyleAttrs( zVIEW vSubtask, zVIEW vUserSpec,
                                 zPCHAR pchAttribType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddNewEntityToStyle( zVIEW vSubtask, zVIEW vUserSpec,
                                 zPCHAR pchObjectName, zPCHAR pchEntityName );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_DeleteWindows( zVIEW vSubtask, zVIEW vNewDialog );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddDialogVORs( zVIEW vSubtask, zVIEW vUserSpec,
                           zVIEW vNewDialog, zPCHAR pchInclEntity );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw2aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDesignListbox( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zLONG    nStart_X,
                               zLONG    nStart_Y,
                               zLONG    nHeight,
                               zPCHAR   cListType );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase1List( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vParentWindow,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase2Group( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEnt,
                                zVIEW  vNewDialog,
                                zVIEW  vParentWindow,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase2INCL( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase4Group( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEnt,
                                zVIEW  vNewDialog,
                                zVIEW  vParentWindow,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase4INCL( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase3INCL( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR pchCase );

zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateCase1Buttons( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vUSSubEnt,
                              zVIEW  vNewDialog,
                              zVIEW  vParentRoot,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR pchSubWindowTag,
                              zPCHAR pchTypeFlag );
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignListQual( zVIEW  vSubtask,
                              zVIEW  vUSSubEnt,
                              zVIEW  vNewDialog,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR pchActivateOperation );


/////////////////////////////////////////////////////////////////////////////
//
//  tzadw3aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateSubObjectWnd( zVIEW vSubtask, zVIEW vUSSubEnt, zVIEW vNewDialog);
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateSubWindow( zVIEW vSubtask, zVIEW vUSSubEnt, zVIEW vNewDialog );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CalculateWindowSize( zVIEW vNewDialog, zPLONG plLine,
                                 zPLONG nMaxLength );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDsgnListboxComb( zVIEW vUserSpec, zVIEW vNewDialog,
                                 zLONG lStart_X, zLONG lStart_Y,
                                 zLONG lHeight, zPCHAR cActionType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateLstBoxActions( zVIEW vNewDialog, zVIEW vUserSpec,
                                 zLONG lListboxWidth, zLONG lStart_Y,
                                 zLONG lStart_X, zLONG lHeight,
                                 zPCHAR cActionType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDsgnPartComb( zVIEW vUserSpec, zVIEW vNewDialog,
                              zLONG lStart_X, zLONG lStart_Y,
                              zLONG lHeight, zPCHAR cActionType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateListBox( zVIEW vSubtask, zVIEW vNewDialog, zVIEW vUserSpec,
                           zLONG lStart_X, zLONG lStart_Y,
                           zLONG lHeight, zPCHAR pchListType );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw4aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintListBox( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vNewDialog,
                              zPCHAR pchListType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintLstLitPart( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zVIEW  vNewDialog,
                                 zLONG  OffsetDLG_Y,
                                 zLONG  OffsetDLG_X,
                                 zLONG  PartitionSize,
                                 zPCHAR pchListType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintListPart( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vNewDialog,
                               zLONG  lFieldRow,
                               zLONG  lFieldCol,
                               zLONG  PartitionSize,
                               zPCHAR pchListType );
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_AutoPntFldPartWLits( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zVIEW  vNewDialog,
                                 zPCHAR pchListType );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AssignTag( zVIEW  vSubtask,
                       zVIEW  vDialog,
                       zVIEW  vControl,
                       zPCHAR pchType );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw5aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateQualifierWnd( zVIEW vSubtask, zVIEW vUSSubEnt,
                                zVIEW vNewDialog );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateEntryFields( zVIEW vSubtask, zVIEW vNewDialog,
                               zPCHAR pchLiteral, zSHORT nLength );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateEntryWindow( zVIEW vSubtask, zVIEW vUSSubEnt,
                               zVIEW vNewDialog );
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_LocateLstboxAttribs( zVIEW vSubtask, zVIEW vUserSpec );

zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_GetSizeForListAttr( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zPCHAR pchListType );
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_GetSizeForAttr( zVIEW  vSubtask,
                            zVIEW  vUserSpec,
                            zPCHAR pchDomainName,
                            zPCHAR pchER_AttributeName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw6aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateGenericDialog( zVIEW vUserSpec, zVIEW vNewDialog);
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateFullKeyDialog( zVIEW vUserSpec, zVIEW vNewDialog, zVIEW vAutoDesWk);
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateListDialog( zVIEW vUserSpec, zVIEW vNewDialog, zVIEW vAutoDesWk, zPCHAR cDialogStyle);

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw7aw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateSourceCode( zVIEW vSubtask, zVIEW vUserSpec, zVIEW vNewDialog, zPCHAR pchSourceCode);
zOPER_EXPORT void OPERATION
ofnTZWDLGSO_CreateModelCode( zVIEW vUserSpec, zVIEW vNewDialog, zVIEW vAutoDesWk, zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzadw8aw.c
//
/////////////////////////////////////////////////////////////////////////////
void zPASCAL
qbldelta( int  nPatternLth,         // pattern length
          zPCHAR pchPattern,        // pattern to locate
          zPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
          zPLONG lplDelta2Table );  // Boyer-Moore Delta2 Table
long zPASCAL
qfind( int  nPatternLth,         // pattern length
       zPCHAR pchPattern,        // pattern to locate
       long lBufferLth,          // length of buffer to be searched
       zPCHAR Buffer,            // buffer to be searched for pattern
       zPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
       zPLONG lplDelta2Table );  // Boyer-Moore Delta2 Table
long zPASCAL
qchange( long lCurrentPos,       // current position at which to repl
         int  nPatternLth,       // pattern length
         zPCHAR pchPattern,      // pattern to locate
         int  nReplaceLth,       // replace string length
         zPCHAR pchReplace,      // replace string
         zPLONG lBufferLth,      // length of buffer to be searched
         zPCHAR Buffer,          // buffer to be searched for pattern
         zPLONG lplDelta1Table,  // Boyer-Moore Delta1 Table
         zPLONG lplDelta2Table,  // Boyer-Moore Delta2 Table
         long lMaxLth );         // Maximum lth of resultant string
long zPASCAL
qfindr( int  nPatternLth,        // pattern length
        zPCHAR pchPattern,       // pattern to locate
        long lBufferLth,         // length of buffer to be searched
        zPCHAR Buffer,           // buffer to be searched for pattern
        zPLONG lplDelta1Table,   // Boyer-Moore Delta1 Table
        zPLONG lplDelta2Table ); // Boyer-Moore Delta2 Table

/////////////////////////////////////////////////////////////////////////////
//
//  tzadwoaw.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_BasicWindowPainting( zVIEW    vSubtask,
                                 zVIEW    vUserSpec,
                                 zVIEW    vNewDialog,
                                 zVIEW    vDefaults,
                                 zVIEW    vAutoDesWk,
                                 zPCHAR   pchLOD_Attribute );

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateNewWindowInfo( zVIEW vSubtask,
                                 zVIEW vUserSpec,
                                 zVIEW vNewDialog,
                                 zVIEW vAutoDesWk );
zOPER_EXPORT zSHORT OPERATION
AddSpacesToString( zPCHAR pchWorkString );
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_AutoDesignWindow( zVIEW vSubtask,
                              zVIEW vUserSpec,
                              zVIEW vNewDialog,
                              zVIEW vAutoDesWk );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_BuildButton( zVIEW  vSubtask,
                         zVIEW  vUserSpec,
                         zVIEW  vNewDialog,
                         zPCHAR pchButtonText,
                         zPCHAR pchDefaultText,
                         int    *nMaxLength,
                         int    *nNbrButtons );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateTheButton( zVIEW  vSubtask,
                             zVIEW  vNewDialog,
                             zLONG  lLine,
                             zLONG  lColumn,
                             zPCHAR pchActionText,
                             zLONG  lButtonWidth );
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateTheOperation( zVIEW   vSubtask,
                                zVIEW   vOper,
                                zPCHAR  pchOperName,
                                zPCHAR  pchOperDesc,
                                zVIEW   vInclude,
                                zPCHAR  pchIncludeEntityName );

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateDIL( zVIEW vSubtask,
                       zVIEW vNewDialog,
                       zVIEW vDefaults );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmcpro.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT void OPERATION
zstrblank( zPCHAR pchString,
           zSHORT nLth,
           zSHORT bNullLastByte );
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_Enqueue_CPLR_Enqueue( zVIEW vTZCMCPRO,
                                zVIEW vSubtask,
                                zSHORT EnqueueType );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmcvto.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zVIEW OPERATION
ConvertDialog( zVIEW   vSubtask,
               zVIEW   vSrcDialog,
               zCPCHAR pchTgtDlgName,
               zPVIEW  pvXRW );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmlops.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
cfPostCheckout( zSHORT   Type,
                zVIEW    vZeidonCM,
                zVIEW    vMOI_View,
                zVIEW    vTaskLPLR,
                zBOOL    bRepository );
zSHORT
cfPostActivate( zVIEW    vSubtask,
                zSHORT   Type,
                zPCHAR   pchSyncDate,
                zVIEW    vZeidonCM,
                zVIEW    vMOI_View,
                zVIEW    vTaskLPLR,
                zSHORT   PhaseCtl,
                zBOOL    bCheckin );
zSHORT
cfPreCommit( zSHORT   Type,
             zVIEW    vZeidonCM,
             zVIEW    vMOI_View,
             zVIEW    vTaskLPLR );
zSHORT
cfTZMSDILO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZMSDILO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZMSDILO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZMSDILO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZMSDILO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZMSDILO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZDMSRCO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZDMSRCO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZDMSRCO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZDMSRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZDMSRCO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZDMSRCO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZEREMDO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZEREMDO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZEREMDO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZEREMDO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZEREMDO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZEREMDO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZERSASO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZERSASO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZERSASO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZERSASO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZERSASO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZERSASO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZZOLODO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZZOLODO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZZOLODO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZZOLODO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZZOLODO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZZOLODO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZWDLGSO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZWDLGSO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZWDLGSO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZWDLGSO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZWDLGSO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZWDLGSO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZTENVRO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZTENVRO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZTENVRO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZTENVRO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZTENVRO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZTENVRO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZOPSIGO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZOPSIGO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZOPSIGO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZOPSIGO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZOPSIGO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZOPSIGO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZOPHDRO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZOPHDRO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZOPHDRO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZOPHDRO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZOPHDRO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZOPHDRO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZADCSDO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZADCSDO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZADCSDO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZADCSDO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZADCSDO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZADCSDO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZWDVORO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZWDVORO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZWDVORO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZWDVORO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZWDVORO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZWDVORO,
                      zVIEW    vTaskLPLR );
zSHORT
cfTZPESRCO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZPESRCO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZRPSRCO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZRPSRCO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZXSLTSO_PostCheckout( zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zBOOL    bRepository );
zSHORT
cfTZXSLTSO_PostActivate( zVIEW    vSubtask,
                         zPCHAR   pchReturnSyncDate,
                         zVIEW    vCM_Subtask,
                         zVIEW    vTZPESRCO,
                         zVIEW    vTZCMLPLO,
                         zSHORT   PhaseCtl,
                         zBOOL    bCheckin );
zSHORT
cfTZPESRCO_PreCommit( zVIEW    vZeidonCM,
                      zVIEW    vTZPESRCO,
                      zVIEW    vTaskLPLR );
zSHORT
fnGetActiveMetaByType( zPVIEW   vMeta,
                       zVIEW    vCM_Subtask,
                       zVIEW    vActiveMetas,
                       zSHORT   nType,
                       zSHORT   nRelNbr );
zSHORT
fnActivateDependentBySyncDate( zVIEW   vSubtask,
                               zPCHAR  pchReturnSyncDate,
                               zVIEW   vWkTZCMLPLO,
                               zPCHAR  pchSyncDate,
                               zSHORT  nDependentType,
                               zSHORT  nForce );
zOPER_EXPORT zSHORT OPERATION
ofnTZZOLODO_BldFlatFromChild( zVIEW  vTZZOLOD1,
                              zVIEW  vTZZOLODO );
zOPER_EXPORT zSHORT OPERATION
fnTZZOLODO_BuildFlatFromRec( zVIEW vTZZOLODO );
void
fnLinkAndSyncER_ToSA( zVIEW vSubtask, zVIEW vTZEREMDO, zVIEW vTZERSASO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmlp2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMLP2O_AnalyzeEntity( zVIEW  vXOD,
                         zVIEW  vWk,
                         zPLONG EntityConstraintSeqNo,
                         zPLONG DerivedAttributeSeqNo,
                         zPCHAR ObjectName );
zOPER_EXPORT zSHORT OPERATION
oTZCMLP2O_GenerateDomainHeader( zVIEW  vLPLR,
                                zLONG  f );
zOPER_EXPORT zSHORT OPERATION
oTZCMLP2O_GenerateObjectHeader( zVIEW  vLPLR,
                                zLONG  f );
zOPER_EXPORT zSHORT OPERATION
oTZCMLP2O_wl( zLONG  lFile,
              zPCHAR StatementLine );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmlplo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT void OPERATION
ofnTZCMWKSO_AppendSlash( zPCHAR pchDirectory );
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CommitLPLR( zVIEW  vTZCMLPLO );
zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_CopySRC_ToLPLR( zVIEW  vTZCMCPRO,
                            zVIEW  vTZCMLPLO,
                            zVIEW  vSrcMeta );
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_RefreshLPLR( zVIEW  vSubtask,
                       zVIEW  vTZCMLPLO,
                       zVIEW  vTZCMCPRO );
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CheckInMetas( zVIEW  vSubtask,
                        zVIEW  vTZCM_ChkInList,
                        zVIEW  vTZCMCPLO,
                        zSHORT Selected );
zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_GetNextCheckInType( zVIEW  vSubtask,
                                zVIEW  vTZCM_ChkInList,
                                zLONG  Type );
zOPER_EXPORT zSHORT OPERATION
ofnTZCMLPLO_GetLastCheckInType( zVIEW  vSubtask,
                                zVIEW  vTZCM_ChkInList );

zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CopyFileToServerFile( zVIEW  vSubtask,
                                zPCHAR pchL_MetaSrcDir,
                                zVIEW  vTZCMCPRO,
                                zPCHAR pchName,
                                zPCHAR pchExtension,
                                zLONG  lViewCluster );
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_CopyFileFromServerFile( zVIEW  vSubtask,
                                  zPCHAR pchL_MetaSrcDir,
                                  zVIEW  vTZCMCPRO,
                                  zPCHAR pchName,
                                  zPCHAR pchExtension );
zOPER_EXPORT zSHORT OPERATION
oTZCMLPLO_ActivateFileOI( zPVIEW vFile,
                          zVIEW  vTZCMCPRO,
                          zLONG  lZKey );

zOPER_EXPORT zSHORT OPERATION
zwTZCMLPLO_LoadRefreshListForCPLR( zVIEW  vSubtask,
                                   zVIEW  vTZCMLPLO,
                                   zVIEW  vTZCMCPRO,
                                   zPCHAR pchPrevLPLR_LastRefreshDate );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmoprs.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BuildLPLR_MetaTypes( zVIEW vSubtask, zVIEW LPLR_View, zSHORT BuildMetas );
zOPER_EXPORT zSHORT OPERATION
fnActivateMetaOI( zVIEW   vSubtask,
                  zPVIEW  lpMOI_View,
                  zVIEW   ListView,
                  zSHORT  Type,
                  zLONG   lControl );
zOPER_EXPORT zSHORT OPERATION
CommitDependentOI( zVIEW   vSubtask,
                   zVIEW   MOI_View,
                   zSHORT  Type );
zOPER_EXPORT zSHORT OPERATION
CreateTemporalMetaEntity( zVIEW  vSubtask,
                          zVIEW  lpView,
                          zPCHAR pchEntityName,
                          zSHORT nPosition );
zOPER_EXPORT zSHORT OPERATION
SetZKeyToNextZKey( zVIEW  vSubtask,
                   zVIEW  lpView,
                   zPCHAR pchEntityName );
zOPER_EXPORT zSHORT OPERATION
GetWKS_FileName( zPCHAR pchZeidonWKS );
zOPER_EXPORT zSHORT OPERATION
CommitWKS( zVIEW WKS_View );
zOPER_EXPORT zSHORT OPERATION
CommitLPLR( zVIEW LPLR_View,
            zPCHAR pchLPLR );
zOPER_EXPORT zSHORT OPERATION
zxatol( zPCHAR pchHexString,
        zPLONG plIn );
zOPER_EXPORT zSHORT OPERATION
LoadZeidonPPE( zVIEW  vSubtask,
               zPVIEW pvPPEView,
               zSHORT nType,
               zVIEW  vMetaView,
               zPCHAR pchMessageTitel,
               zPCHAR pchMessagetext );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmrepo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMREPO_ActivateRepository( zPVIEW vTZCMREPO,
                              zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZCMREPO_PreCheckInBackup( zVIEW  vTZCMLPLO,
                            zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZCMREPO_PostCheckInRecovery( zVIEW  vTZCMLPLO,
                               zVIEW  vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmutil.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TruncateName8( zPCHAR pchName );
zSHORT
fnVerifyType( zSHORT Type );
int
fnBuildMetas( zVIEW  vSubtask,
              zVIEW  LPLR_View,
              zPCHAR cDirectorySpec,
              int nEntityType );
zSHORT
fnCleanupActiveMetas( zVIEW vActiveMetas,
                      zVIEW vZeidonCM );
zSHORT
fnCheckForDuplicateName( zVIEW  vMOI,
                         zVIEW  vLPLR,
                         zPCHAR pchMetaOI_Name,
                         zLONG  lMetaOI_ZKey,
                         zSHORT nNewInd );
zSHORT
fnCheckCM_ForActiveMetaByTask( zVIEW vSubtask, zLONG lMOI_InstanceID );
void
fnGetTaskOI_ListName( zVIEW vSubtask, zPCHAR pchTaskOI_ListName, zLONG lMaxLth );
void
fnGetAndSetZKey( zVIEW  vSubtask,
                 zVIEW  LPLR_View,
                 zPCHAR pchFileName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmwkso.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_GetWKS_FileName( zPCHAR pchZeidonWKS, zLONG lMaxLth );
zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_ActivateWorkStation( zPVIEW vTZCMWKSO,
                               zVIEW  vQual,
                               zLONG lControl );
zOPER_EXPORT zSHORT OPERATION
oTZCMWKSO_CommitWorkstation( zVIEW WKS_View );


/////////////////////////////////////////////////////////////////////////////
//
//  tzcmcpro.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteServerFile( zVIEW  vTZCMCPRO,
                            zPCHAR pchName,
                            zPCHAR pchExtension );
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteDependents( zVIEW vTZCMCPRO,
                            zVIEW vMetaOI,
                            zVIEW vKZDBHQUA,
                            zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteMeta( zVIEW vTZCMCPRO,
                      zVIEW vKZDBHQUA,
                      zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteMetaWithCluster( zVIEW vTZCMCPRO,
                                 zVIEW vTZCMFILE,
                                 zVIEW vKZDBHQUA,
                                 zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteFileWithCluster( zVIEW  vSubtask,
                                 zVIEW  vTZCMFILE,
                                 zPCHAR pchName,
                                 zPCHAR pchExtension );


zOPER_EXPORT zSHORT OPERATION
oTZCMCPRO_DeleteDependentsWithCluster( zVIEW vTZCMCPLR,
                                       zVIEW vTZCMFILE,
                                       zVIEW vMetaOI,
                                       zVIEW vKZDBHQUA,
                                       zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzcmqsyo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZCMQSYO_ActivateREPO_Enqueue( zPVIEW vTZCMQSYO,
                                zVIEW  vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzdmsrco.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveContextName( zVIEW        vMeta,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomName( zVIEW        vMeta,
                         LPVIEWENTITY lpViewEntity,
                         LPVIEWATTRIB lpViewAttrib,
                         zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomDataType( zVIEW        vMeta,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomType( zVIEW        vMeta,
                         LPVIEWENTITY lpViewEntity,
                         LPVIEWATTRIB lpViewAttrib,
                         zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomStringLth( zVIEW        vMeta,
                              LPVIEWENTITY lpViewEntity,
                              LPVIEWATTRIB lpViewAttrib,
                              zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomDecFormat( zVIEW        vMeta,
                              LPVIEWENTITY lpViewEntity,
                              LPVIEWATTRIB lpViewAttrib,
                              zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDLL_Name( zVIEW        vMeta,
                          LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttrib,
                          zSHORT       nMsg );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_GetViewForPDM( zPVIEW pvTZDMSRCO,
                         zPCHAR pchDomainName,
                         zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_GetViewForPDM_ZKey( zPVIEW pvTZDMSRCO,
                              zLONG  lZKey,
                              zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_UniqueDomainName( zVIEW  vDomain,
                            zVIEW  vCM_List );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_RemoveDomainFromXDM( zVIEW vSubtask, zVIEW  vDomain );
//zOPER_EXPORT zSHORT OPERATION
//oTZDMSRCO_ReplaceOrAddDomToXDM( zVIEW vSubtask, zVIEW  vDomain,
//                                zBOOL bCommit );
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_AddDomToXDM( zVIEW vSubtask, zVIEW  vDomain );

/////////////////////////////////////////////////////////////////////////////
//
//  tzdmxgpo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMXGPO_GetViewForXDM( zVIEW  vSubtask,
                         zPVIEW pvTZDMXGPO,
                         zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZDMXGPO_GetViewForPDMList( zVIEW  vSubtask,
                             zPVIEW pvTZDMXGPO,
                             zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZDMXGPO_CommitXDM( zVIEW vSubtask, zVIEW  vTZDMXGPO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzeremdo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_GetUpdViewForER( zVIEW  vSubtask,
                           zPVIEW pvTZEREMDO,
                           zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_GetRefViewForER( zVIEW  vSubtask,
                           zPVIEW pvTZEREMDO,
                           zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeleteEntity( zVIEW  vTZEREMDO,
                        zSHORT nReposition );
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeleteRelationship( zVIEW  vTZEREMDO,
                              zSHORT nReposition,
                              zSHORT nQuietMode );
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_RelinkDomainsToER( zVIEW  vTZEREMDO,
                              zSHORT nCurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZEREMDO_DeleteRelationship( zVIEW  vTZEREMDO,
                              zSHORT nReposition,
                              zSHORT nQuietMode );

/////////////////////////////////////////////////////////////////////////////
//
//  tzeremxo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZEREMXO_BuildExecutableModel( zVIEW  vSubtask, zPVIEW vReturnExecModel,
                                zVIEW  vModel, zVIEW  vTE );

/////////////////////////////////////////////////////////////////////////////
//
//  tzersa2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZERSA2O_SetCursorNewRelType( zVIEW  OldERD,
                               zVIEW  NewERD );

/////////////////////////////////////////////////////////////////////////////
//
//  tzersaso.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RefreshSA_FromEMD( zVIEW  vSubjectArea,
                             zVIEW  vEntpModel );
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RefreshOneSA_Entity( zVIEW  vSubjectArea,
                               zVIEW  vEntpModel );
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_AddOneSA_EntityFrER( zVIEW  vSubtask,
                               zVIEW  vSubjectArea,
                               zVIEW  vEntpModel );
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_RemoveOneSA_Entity( zVIEW  vSubjectArea );
zOPER_EXPORT zSHORT OPERATION
oTZERSASO_CompleteRelCreate( zVIEW vSubtask, zVIEW  vSubjectArea );

/////////////////////////////////////////////////////////////////////////////
//
//  tzopgrpo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZOPGRPO_GetViewForXGO( zVIEW  vSubtask,
                         zPVIEW pvTZOPGRPO,
                         zSHORT CurrentOrReload );
zOPER_EXPORT zSHORT OPERATION
oTZOPGRPO_CommitXGO( zVIEW  vTZOPGRPO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzopsigo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_ReplaceOrAddOperToXGO( zVIEW vSubtask, zVIEW  vOperation );
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_AddOperToXGO( zVIEW vSubtask, zVIEW  vOperation );
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_UniqueHdrName( zVIEW  vOperation,
                         zVIEW  vCM_List );
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_RemoveHdrFromXGO( zVIEW vSubtask,
                            zVIEW vOperation );
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_ReplaceOrAddHdrToXGO( zVIEW vSubtask,
                                zVIEW vOperation );
zOPER_EXPORT zSHORT OPERATION
oTZOPSIGO_AddHdrToXGO( zVIEW vSubtask,
                       zVIEW vOperation );

/////////////////////////////////////////////////////////////////////////////
//
//  tztempmo.c
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
//  tztenv2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomDataType( zVIEW  vTE,
                             zPCHAR pchEntity,
                             zPCHAR pchAttribute,
                             zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomDecFormat( zVIEW  vTE,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomName( zVIEW  vSubtask,
                         zVIEW  vTE,
                         zPCHAR pchEntity,
                         zPCHAR pchAttribute,
                         zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomStringLth( zVIEW  vTE,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_SetCursorNewRelType( zVIEW  OldERD,
                               zVIEW  NewERD );

/////////////////////////////////////////////////////////////////////////////
//
//  tztenv2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomDataType( zVIEW  vTE,
                             zPCHAR pchEntity,
                             zPCHAR pchAttribute,
                             zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomDecFormat( zVIEW  vTE,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomName( zVIEW  vSubtask,
                         zVIEW  vTE,
                         zPCHAR pchEntity,
                         zPCHAR pchAttribute,
                         zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_DeriveDomStringLth( zVIEW  vTE,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_SetCursorNewRelType( zVIEW  OldERD,
                               zVIEW  NewERD );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_TE_Migrate( zPVIEW NewTE,
                      zPCHAR TE_MetaName,
                      zVIEW  SourceLPLR,
                      zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZTENV2O_TE_RelinkDelete( zVIEW vTE, zVIEW  vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tztenvro.c
//
/////////////////////////////////////////////////////////////////////////////
typedef zSHORT ( POPERATION PFNGENERATENAME ) ( zVIEW, zPCHAR, zLONG, zCHAR );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_GetUpdViewForDTE_P( zVIEW vSubtask, zPVIEW pvTZTENVRO );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_GetRefViewForDTE_P( zVIEW vSubtask, zPVIEW pvTZTENVRO );

zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyAttRel( zVIEW  vSubtask,
                                 zVIEW  vDTE,
                                 zVIEW  vEMD );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyToken( zVIEW  vSubtask,
                                zVIEW  vDTE,
                                zVIEW  vEMD,
                                PFNGENERATENAME pfnGenerateName );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_BldPrimaryKeyAttr( zVIEW  vSubtask,
                               zVIEW  vDTE,
                               zVIEW  vEMD );
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildRelationsFromEMD( zVIEW  vSubtask,
                                 zVIEW  vDTE,
                                 zVIEW  vEntpModel );
zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_BuildTablRecsFromEMD( zVIEW  vSubtask,
                                zVIEW  vDTE,
                                zVIEW  vEntpModel,
                                zPCHAR pchScopingEntityTE );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementRelLink( zVIEW  vSubtask,
                              zVIEW  vDTE,
                              zVIEW  vEMD,
                              zVIEW  vDTE_p,
                              zVIEW  vEMD_p,
                              PFNGENERATENAME pfnGenerateName );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementSQL_RelLnk( zVIEW  vSubtask,
                                 zVIEW  vDTE,
                                 zVIEW  vEMD,
                                 zVIEW  vDTE_p,
                                 zVIEW  vEMD_p,
                                 PFNGENERATENAME pfnGenerateName );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_ImplementNET_RelLnk( zVIEW  vSubtask,
                                 zVIEW  vDTE,
                                 zVIEW  vEMD,
                                 zVIEW  vDTE_p,
                                 zVIEW  vEMD_p,
                                 PFNGENERATENAME pfnGenerateName );
zOPER_EXPORT zSHORT OPERATION
ofnTZTENVRO_CreateRelTablRec( zVIEW  vSubtask,
                              zVIEW  vDTE1,
                              zVIEW  vEMD1,
                              zVIEW  vDTE2,
                              zVIEW  vEMD2,
                              zVIEW  vDTE_new,
                              PFNGENERATENAME pfnGenerateName );
zVOID LOCALOPER
ofnTZTENVRO_GenerateUniqueName( zVIEW  vSubtask,
                                zVIEW  vDTE,
                                zPCHAR pchName,
                                zLONG  lMaxLth,
                                zCHAR  chType,
                                zPCHAR pchEntityName,
                                zPCHAR pchAttributeName,
                                zPCHAR pchScopingEntity,
                                PFNGENERATENAME pfnGenerateName );

zOPER_EXPORT zSHORT OPERATION
oTZTENVRO_SwitchForeignKeys( zVIEW vDTE, zVIEW vDTE_Copy1 );

/////////////////////////////////////////////////////////////////////////////
//
//  tzwdgrwo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_Save( zVIEW  vGRW );

/////////////////////////////////////////////////////////////////////////////
//
//  tzwdlg2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_ControlRelinkDelete( zVIEW  vLOD_LPLR,
                               zVIEW  vDialog,
                               zVIEW  vControl,
                               zPVIEW vLastLOD,
                               zPLONG LastViewZKey,
                               zVIEW  vSubtask );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveContextName( zVIEW  vLOD,
                             zPCHAR pchEntity,
                             zPCHAR pchAttribute,
                             zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveDLL_Name( zVIEW  vLOD,
                          zPCHAR pchEntity,
                          zPCHAR pchAttribute,
                          zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveDomDataType( zVIEW  vLOD,
                             zPCHAR pchEntity,
                             zPCHAR pchAttribute,
                             zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveDomDecFormat( zVIEW  vLOD,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveDomName( zVIEW  vLOD,
                         zPCHAR pchEntity,
                         zPCHAR pchAttribute,
                         zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveDomStringLth( zVIEW  vLOD,
                              zPCHAR pchEntity,
                              zPCHAR pchAttribute,
                              zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_DeriveSourceName( zVIEW  vLOD,
                            zPCHAR pchEntity,
                            zPCHAR pchAttribute,
                            zSHORT GetOrSetFlag );
zOPER_EXPORT zSHORT OPERATION
oTZWDLG2O_OptionRelinkDelete( zVIEW  vSubtask,
                              zVIEW  vLOD_LPLR,
                              zVIEW  vDialog,
                              zVIEW  vOption,
                              zPVIEW vLastLOD,
                              zPLONG LastViewZKey );

/////////////////////////////////////////////////////////////////////////////
//
//  tzwdlgso.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_CloneOptMap( zVIEW     vSubtask,
                       zVIEW     vSourceLPLR,
                       zVIEW     OrigW,
                       zVIEW     OrigWO,
                       zVIEW     NewW,
                       zVIEW     NewWO );
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_WndEventName( zVIEW        vTZWDLGSO,
                        LPVIEWENTITY lpViewEntity,
                        LPVIEWATTRIB lpViewAttrib,
                        zSHORT       nMsg );

/////////////////////////////////////////////////////////////////////////////
//
//  tzzolo2o.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLO2O_LOD_EntityCopy( zVIEW  vSubtask,
                          zVIEW  NewMainLOD,
                          zVIEW  NewRecursiveLOD,
                          zVIEW  OldMainLOD,
                          zVIEW  OldRecursiveLOD,
                          zVIEW  SourceLPLR,
                          zVIEW  CurrentLPLR,
                          zVIEW  NewERD,
                          zPCHAR ParentName );
zOPER_EXPORT zSHORT OPERATION
oTZZOLO2O_LOD_RelinkOperation( zVIEW  vLOD,
                               zVIEW  SourceLPLR,
                               zPCHAR OperationName,
                               zPCHAR SourceName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzzolodo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SaveAllLODs( zVIEW vSubtask, zPCHAR pchType );

zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_RefreshLOD_FromEMD( zVIEW  vSubtask,
                              zVIEW  vLOD,
                              zVIEW  vEntpModel );
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_GenReportVML( zVIEW  vLOD,
                        zVIEW  vProfile );
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_ReportOI( zVIEW  vLOD,
                    zPCHAR pchReportFileName );
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SaveLOD_WithTE( zVIEW vSubtask,
                          zVIEW vTZZOLODO,
                          zVIEW vTZTENVRO_REF );

/////////////////////////////////////////////////////////////////////////////
//
//  tzzoxodo.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOXODO_SaveXOD( zVIEW vSubtask,
                   zVIEW vTZZOLODO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmgnaa.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
Generate3GL( zVIEW   lpVSSubtask,    // pointer to the vml task
             zVIEW   lpSubtask,      // pointer to the appl task
             zVIEW   vInSourceMeta,
             zPCHAR  pchFileName,    // name of the vml file to generate
             zCPCHAR cpcGenLang,
             zLONG   lType );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmg1aa.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
GenerateC_Source( zVIEW  vSubtask,
                  zPCHAR pchFileName,
                  zPCHAR pchXPGName,
                  zLONG  lCFileHandle,
                  zLONG  lTempCFileHandle );

zOPER_EXPORT zSHORT OPERATION
GenerateJavaSource( zVIEW  vSubtask,
                    zPCHAR pchFileName,
                    zPCHAR pchXPGName,
                    zLONG  lCFileHandle,
                    zLONG  lTempCFileHandle,
                    zLONG  lType );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmg2aa.c
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
GenerateNewExpression( zVIEW vSubtask, zPCHAR pchOutLine );

zVOID FormatString( zPCHAR pchOutputString);

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmimaa.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
InvokeInterp( zVIEW  lpInterpSubtask,
              zVIEW  lpAppSubtask,
              zPCHAR pchSrcDLLName,
              zPCHAR pchOperationName,
              zPLONG plRetVal );
zOPER_EXPORT zSHORT OPERATION
InitInterp( zVIEW  lpInterpSubtask,
            zVIEW  lpAppSubtask,
            zPCHAR pchSrcDLLName );
zOPER_EXPORT zSHORT OPERATION
LoadXPG( zVIEW  lpInterpSubtask,
         zVIEW  lpAppSubtask,
         zPCHAR pchSrcDLLName,
         zCPCHAR cpcGenLang );
zOPER_EXPORT zSHORT OPERATION
SetStackObjectFromViews( zVIEW  lpInterpSubtask,            // interp subtask
                         zVIEW  vXPG,                      // View to the XPG
                         zVIEW  vStmtView,           // View to the statement
                         zSHORT nIndex );        // index into the expression
zOPER_EXPORT zSHORT OPERATION
UpdateStackObjectFromViews( zVIEW  lpInterpSubtask,         // interp subtask
                            zVIEW  vXPG,                   // View to the XPG
                            zVIEW  vStmtView,        // View to the statement
                            zSHORT nIndex,       // index into the expression
                            zSHORT nStackOffset );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmim1a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
StepOnce( zVIEW  lpAppSubtask, zPVOID hWRKS, zPLONG plRetVal );
zOPER_EXPORT zLONG OPERATION
Skip( zVIEW  lpAppSubtask );
zOPER_EXPORT zLONG OPERATION
Go( zVIEW  lpAppSubtask, zPVOID hWRKS, zPLONG plRetVal );
zOPER_EXPORT zSHORT OPERATION
StageOperationCall( zVIEW  lpInterpSubtask,
                    zVIEW  lpAppSubtask,
                    zPCHAR pchModuleName,
                    zPCHAR pchOperName );
zOPER_EXPORT zLONG OPERATION
EvaluateExpression( zPVOID pEvaluationValue,
                    zVIEW  lpInterpSubtask,
                    zVIEW  lpAppSubtask,
                    zVIEW  vStatementView,
                    zPVOID hWRKS );
zOPER_EXPORT zSHORT OPERATION
Operate( zLONG  lOperator,
         zSHORT nNumericType,
         zVIEW  vSubtask );
zOPER_EXPORT zLONG OPERATION
SetUpOperationCall( zVIEW  lpInterpSubtask,
                    zSHORT nIndex );
zOPER_EXPORT zSHORT OPERATION
GetOperationParmList( zVIEW  vSubtask,
                      zSHORT pnParmList[],
                      zLONG  lZKey,
                      zPCHAR pchOperationName,
                      zLONG  lOperationClass );
zOPER_EXPORT zSHORT OPERATION
GetParmPushSize( zSHORT nType );
zOPER_EXPORT zSHORT OPERATION
ConvertZeidonTypeToVMLTypeS( zCHAR cZeidonType );
zOPER_EXPORT zSHORT OPERATION
xGetDefineInternalValue( zPCHAR pchInternalValue,
                         zLONG  lOperationClass,
                         zLONG  lDefineZKey );
zOPER_EXPORT zSHORT OPERATION
GetDefineInternalValue( zVIEW  vSubtask,
                        zPCHAR pchInternalValue,
                        zLONG  lDefineZKey );
zOPER_EXPORT zSHORT OPERATION
SetPointerValues( zPSHORT nStartStackPtr, zVIEW lpInterpSubtask );
zOPER_EXPORT zSHORT OPERATION
GetOperationModuleName( zVIEW  vSubtask,
                        zPCHAR pchModuleName,
                        zLONG  lZKey,
                        zLONG  lOperationClass );
zOPER_EXPORT zSHORT OPERATION
SetPointerValuesFromLocalCall( zSHORT nStartStackPtr, zVIEW lpInterpSubtask );
zOPER_EXPORT zSHORT OPERATION
IncrementStackPtr( );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpmaa.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ParseVMLSource( zVIEW  lpVSSubtask,
                zVIEW  lpSubtask,
                zPCHAR pchSrcDirectory,
                zPCHAR pchFileName,
                zCPCHAR cpcGenLang );

zOPER_EXPORT zLONG OPERATION
ParseSource( zVIEW  lpVSSubtask,
             zVIEW  vInSourceMeta,
             zLONG  lInParseAction,
             zLONG  lInParseLimit,
             zLONG  lInParseSource,
             zPCHAR pchSourceFileName,
             zPCHAR pchInOperationName,
             zLONG  lMetaType,
             zLONG  lControl,
             zCPCHAR cpcGenLang );
zOPER_EXPORT zLONG OPERATION
IncludeCoreHeader( zVIEW  lpView,
                   zPCHAR pchHeaderName );
zOPER_EXPORT zLONG OPERATION
PositionAtOperationInSource( zLONG lZKey );
zOPER_EXPORT zLONG OPERATION
DeleteAllOpersForSource( zVIEW vXPGView );
zOPER_EXPORT zLONG OPERATION
DeleteOneOperForSource( zVIEW vXPGView );
zOPER_EXPORT zLONG OPERATION
InitReadBuffer( zVIEW  vSubtask,
                zPCHAR lpBuffer,
                zLONG  lParseSource );
zOPER_EXPORT zLONG OPERATION
InitGlobalDataForParse( zVOID );
zOPER_EXPORT zLONG OPERATION
SkipRemainingFile( zVOID );
zOPER_EXPORT zLONG OPERATION
GetTextLine( zVOID );
zOPER_EXPORT zLONG OPERATION
SaveTextLine( zVOID );
zOPER_EXPORT zLONG OPERATION
GetProfileData( zVIEW  vSubtask,
                zPCHAR pchSourceDirectory,
                zPCHAR pchSourceFileName,
                zPCHAR pchXPGFileName,
                zPCHAR pchOperationName );
zOPER_EXPORT zLONG OPERATION
ReInitOperation( zVOID );
zOPER_EXPORT zLONG OPERATION
CompareFileToXPG( zLONG lZKey );
zOPER_EXPORT zLONG OPERATION
InitErrorObject( zVIEW  vSubtask,
                 zPCHAR pchErrorFile,
                 zPCHAR pchSourceFile );
zOPER_EXPORT zLONG OPERATION
InitExpressionObject( zVOID );
zOPER_EXPORT zLONG OPERATION
GetOperationParmListFromSource( zLONG sParmList[],
                                zLONG sParmListExt[],
                                zPCHAR pchParmName,
                                zLONG  lParmLth,
                                zVIEW vSource );
zOPER_EXPORT zLONG OPERATION
ConvertVMLTypeToZeidonType( zPCHAR pchZeidonType,
                            zLONG  lVMLType );
zOPER_EXPORT zLONG OPERATION
GetOperationInformation( zVIEW  vSubtask,
                         zPLONG nOperationClass,
                         zPLONG nOperationZKey,
                         zPLONG nOperationType,
                         zPCHAR pchType,
                         zLONG  lTypeLth,
                         zPVIEW vSource,
                         zPCHAR pchOperationName,
                         zLONG  lSearchOrder,
                         zLONG  lOther );
zOPER_EXPORT zLONG OPERATION
IsKnownObjectOperation( zVIEW  vSubtask,
                        zPVIEW vSource,
                        zVIEW  lpLODView,
                        zPCHAR pchOperationName );
zOPER_EXPORT zLONG OPERATION
LoadSourceForMeta( zVIEW  vSubtask,
                   zPVIEW vSource,
                   zVIEW  vMetaView );
zOPER_EXPORT zLONG OPERATION
LoadSourceForMetaList( zVIEW  vSubtask,
                       zPVIEW vSource,
                       zVIEW  vMetaListView,
                       zLONG  lMetaType );
zOPER_EXPORT zLONG OPERATION
PrintXPG( zVIEW vXPG );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm1a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ActionQ( zVIEW vSubtask, zLONG lActionRequested );
zVOID OPERATION
GenerateSetCursorWithScope( zVIEW vSubtask, zLONG lScope );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm2a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PreBuildOperation( zLONG lOperationClass,
                   zLONG lLineNumber );
zOPER_EXPORT zLONG OPERATION
AddOperationEntityToPI( zVIEW vSubtask, zPCHAR pchOperationName, zSHORT nShow );
zOPER_EXPORT zLONG OPERATION
ResetOperation( zPCHAR pchOperationName );
zOPER_EXPORT zLONG OPERATION
AddVariableEntityToPI( zVIEW  vSubtask,
                       zPCHAR pchVariablName,     // the name of the variable
                       zLONG  lVMLType,           // the vml type that was declared as
                       zLONG  lNumberOfElements,  // number of chars if a string
                       zLONG  lLineNumber );      // line number on which it was defined
zOPER_EXPORT zLONG OPERATION
AddExprNodeEntityToView( zVIEW  vSubtask,
                         zVIEW  vTargetView,
                         zLONG  lEntryClass,
                         zLONG  lEntryDataClass,
                         zLONG  lEntryDataType,
                         zPCHAR pchEntryValue,
                         zLONG  lEntry,
                         zLONG  lParmExt );
zOPER_EXPORT zLONG OPERATION
AddExpressionEntryToString( zPCHAR  pchExprCode,
                            zPLONG  plExprCodeIndex,
                            zPCHAR  pchExprText,
                            zPLONG  plExprTextIndex,
                            zLONG   lEntryClass,
                            zLONG   lEntryDataClass,
                            zLONG   lEntryDataType,
                            zPCHAR  pchEntryValue,
                            zLONG   lEntry,
                            zLONG   lFlags );
zOPER_EXPORT zLONG OPERATION
AddSetEntityCursor( zVIEW  vSubtask,
                    zLONG  lViewID,
                    zPCHAR pchEntityName,
                    zLONG  lEntityID,
                    zLONG  lControl );

zOPER_EXPORT zLONG OPERATION
AddStatementEntityWithStringToPI( zVIEW  vSubtask,
                                  zLONG  lStatementClass,
                                  zLONG  lLineNumber,
                                  zPCHAR pchLocalCode,
                                  zPLONG nLocalCode,
                                  zPCHAR pchLocalText,
                                  zPLONG nLocalText,
                                  zLONG  lPosition );
zOPER_EXPORT zLONG OPERATION
AddGetOrSetAttributeWithString( zVIEW  vSubtask,
                                zPCHAR pchOperationName,
                                zLONG  lViewID,
                                zLONG  lUseVariable,
                                zLONG  lVariableDataType,
                                zLONG  lVariableID,
                                zPCHAR pchEntityName,
                                zLONG  lEntityID,
                                zPCHAR pchAttributeName,
                                zLONG  lAttributeID,
                                zLONG  lUseExtra,
                                zPCHAR pchExtra,
                                zLONG  lExtra,
                                zLONG  lExtraClass );
zOPER_EXPORT zLONG OPERATION
AddGetVariableFromAttribute( zVIEW  vSubtask,
                             zPCHAR pchOperationName,
                             zLONG  lViewID,
                             zLONG  lReturnLthID,
                             zLONG  lUseVariable,
                             zLONG  lVariableDataType,
                             zLONG  lVariableID,
                             zPCHAR pchEntityName,
                             zLONG  lEntityID,
                             zPCHAR pchAttributeName,
                             zLONG  lAttributeID,
                             zLONG  lUseExtra,
                             zPCHAR pchExtra,
                             zLONG  lExtra );
zOPER_EXPORT zLONG OPERATION
CreateConvertZeidonStrOp( zVIEW  vSubtask,
                          zVIEW  vSourceView,
                          zVIEW  vTargetView,
                          zCHAR  cNumberType,
                          zPCHAR pchStringLength );
zOPER_EXPORT zLONG OPERATION
InsertExprNodeValuesToEntity( zVIEW  vTargetView,
                              zLONG  lElementClass,
                              zLONG  lDataClass,
                              zLONG  lDataType,
                              zLONG  lOffset,
                              zLONG  lLevel,
                              zPCHAR pchText,
                              zLONG  lFlags,
                              zLONG  lPos );
zOPER_EXPORT zLONG OPERATION
AddStatementEntityToPI( zVIEW  vSubtask,
                        zLONG  lStatementClass,
                        zLONG  lLineNumber,
                        zVIEW  vSourceView,
                        zLONG  lPosition );
zOPER_EXPORT zLONG OPERATION
AddOperatorToView( zVIEW  vSubtask,
                   zVIEW  vTargetView,
                   zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
AddEntityOperWithEntity( zVIEW  vSubtask,
                         zPCHAR pchOperationName,
                         zLONG  lViewID,
                         zPCHAR pchEntityName,
                         zLONG  lEntityID,
                         zPCHAR pchScopeViewName,
                         zLONG  lScopeViewID,
                         zPCHAR pchScopeEntityName,
                         zLONG  lScopeEntityID,
                         zLONG  lUseExtra,
                         zPCHAR pchExtra,
                         zLONG  lExtra );

zOPER_EXPORT zLONG OPERATION
TraceExpression( zPCHAR pchExprCode,             // expr to build
                 zPLONG lpExprCodeIndex,        // index
                 zPCHAR pchExprText,             // text with expr
                 zPLONG lpExprTextIndex );      // index into text

zOPER_EXPORT zLONG OPERATION
TraceXPG( zVIEW lpInView );

zOPER_EXPORT zLONG OPERATION
IsCondOperator( zLONG lOper );

zOPER_EXPORT zLONG OPERATION
AddJavaWhileWithScope( zVIEW vSubtask, zVIEW vTargetView );

zOPER_EXPORT zLONG OPERATION
AddEntityOperWithScope( zVIEW  vSubtask,
                        zPCHAR pchOperationName,
                        zLONG  lViewID,
                        zPCHAR pchEntityName,
                        zLONG  lEntityID,
                        zLONG  lUseAttribute,
                        zPCHAR pchAttributeName,
                        zLONG  lAttributeID,
                        zPCHAR pchScopeEntityName,
                        zLONG  lScopeEntityID,
                        zLONG  lUseExtra,
                        zPCHAR pchExtra,
                        zLONG  lExtra );

zOPER_EXPORT zLONG OPERATION
AddEntityOperWithNoScope( zVIEW  vSubtask,
                          zPCHAR pchOperationName,
                          zLONG  lViewID,
                          zPCHAR pchEntityName,
                          zLONG  lEntityID,
                          zLONG  lUseExtra,
                          zPCHAR pchExtra,
                          zLONG  lExtra );

zOPER_EXPORT void OPERATION
GermanUmlautHack( zPCHAR pchIn, zPCHAR pchOut, zLONG lBuf, zBOOL bReset );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm3a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
IncludeReferenceToHeader( zVIEW vMetaView );
zOPER_EXPORT zLONG OPERATION
AddResult( zVIEW vSubtask );
zOPER_EXPORT zLONG OPERATION
AddRetCode( zVIEW vSubtask );
zOPER_EXPORT zLONG OPERATION
AddEntityOperWithScopeUsingView( zVIEW  vSubtask,
                                 zPCHAR pchOperationName,
                                 zLONG  lViewID,
                                 zPCHAR pchEntityName,
                                 zLONG  lEntityID,
                                 zLONG  lUseAttribute,
                                 zPCHAR pchAttributeName,
                                 zLONG  lAttributeID,
                                 zPCHAR pchScopeEntityName,
                                 zLONG  lScopeEntityID,
                                 zLONG  lUseExtra,
                                 zPCHAR pchExtra,
                                 zLONG  lExtra );
zOPER_EXPORT zLONG OPERATION
AddEntityOperWithNoScopeView( zVIEW vSubtask,
                              zPCHAR pchOperationName,
                              zLONG  lViewID,
                              zPCHAR pchEntityName,
                              zLONG  lEntityID,
                              zLONG  lUseExtra,
                              zPCHAR pchExtra,
                              zLONG  lExtra );
zOPER_EXPORT zLONG OPERATION
AddResultCondEqual( zVIEW  vSubtask,
                    zLONG  lStatememtnType,
                    zPCHAR pchCompareValue );
zOPER_EXPORT zLONG OPERATION
AddResultAndLoopControlToView( zVIEW  vSubtask,
                               zVIEW  vTargetView,
                               zPCHAR pchCompareValue,
                               zLONG  lVariableID,
                               zLONG  lOperator,
                               zPCHAR pchConstantValue );
zOPER_EXPORT zLONG OPERATION
CreateIntegerExpression( zVIEW  vSubtask,
                         zLONG  lTargetID,
                         zLONG  lOperatorType,
                         zLONG  lEntry1Class,
                         zLONG  lEntry1Value,
                         zLONG  lOper1Value,
                         zLONG  lEntry2Class,
                         zPCHAR pchEntry2Value );
zOPER_EXPORT zLONG OPERATION
AddResultCondExprToView( zVIEW  vSubtask,
                         zVIEW  vTargetView,
                         zLONG  lOperator,
                         zPCHAR pchCompareValue );
zOPER_EXPORT zLONG OPERATION
AddResultAndLoopControlToView( zVIEW  vSubtask,
                               zVIEW  vTargetView,
                               zPCHAR pchCompareValue,
                               zLONG  lVariableID,
                               zLONG  lOperator,
                               zPCHAR pchConstantValue );
zOPER_EXPORT zLONG OPERATION
AddCompareToView( zVIEW  vSubtask,
                  zVIEW  vTargetView,
                  zPCHAR pchOperationName,
                  LPQEXPRSTRUCT sLeft,
                  zPCHAR pchLeftLiteral,
                  LPQEXPRSTRUCT sRight,
                  zPCHAR pchRightLiteral,
                  zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
AddGetOrSetAttribute( zVIEW  vSubtask,
                      zPCHAR pchOperationName,
                      zLONG  lViewID,
                      zLONG  lUseVarFirstOrLast,
                      zLONG  lVariableDataType,
                      zLONG  lVariableID,
                      zPCHAR pchEntityName,
                      zLONG  lEntityID,
                      zPCHAR pchAttributeName,
                      zLONG  lAttributeID,
                      zLONG  lUseExtra,
                      zPCHAR pchExtra,
                         zLONG  lExtra,
                         zLONG  lExtraClass );
zOPER_EXPORT zLONG OPERATION
CompareDataTypesForConsistency( zLONG  lTargetDataType,
                                zLONG  lTargetDataClass,
                                zLONG  lSourceDataType,
                                zLONG  lSourceDataClass );

zOPER_EXPORT zLONG OPERATION
CompareTargetTypeForConsistency( zLONG  lTargetDataType,
                                 zLONG  lLeftDataType,
                                 zLONG  lLeftDataClass,
                                 zLONG  lSourceDataType,
                                 zLONG  lSourceDataClass );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm4a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
GetOperationHeaderFile( zPCHAR pchHeaderFile,
                        zLONG  lOperationZKey,
                        zLONG  lOperationClass );
zOPER_EXPORT zLONG OPERATION
GetDefineZKey( zVIEW  vSubtask,
               zPLONG lpDataType,
               zPCHAR pchDefineValue );
zOPER_EXPORT zLONG OPERATION
xGetDefineZKey( zPLONG lpDataType,
                zPCHAR pchDefineValue,
                zLONG  lOperationZKey,
                zLONG  lOperationClass );
zOPER_EXPORT zLONG OPERATION
GetDefineDataType( zLONG  lDefineZKey,
                   zLONG  lOperationZKey,
                   zLONG  lOperationClass );
zOPER_EXPORT zLONG OPERATION
GetVariableID( zPLONG nVariableID,
               zPCHAR pchVariableName );  // name of the variable looked for
zOPER_EXPORT zLONG OPERATION
GetVariableDataType( zLONG lVariableID ); // id of the variable looked for
zOPER_EXPORT zLONG OPERATION
GetVariableDataClass( zLONG lVariableID );// id of the variable looked for
zOPER_EXPORT zLONG OPERATION
GetClassBasedOnType( zLONG lDataType );
zOPER_EXPORT zLONG OPERATION
GetVariableLength( zPCHAR pchTempNumber,
                   zLONG  lMaxLth,
                   zLONG  lTempID );
zOPER_EXPORT zLONG OPERATION
GetOperationClass( zVIEW  vSubtask,
                   zPLONG nOperationClass,
                   zPCHAR pchOperationName );  // id of the oper to be found
zOPER_EXPORT zLONG OPERATION
IsLocalOperation( zPCHAR pchOperationName );   // id of the oper to be found
zOPER_EXPORT zLONG OPERATION
IsGlobalOperation( zPCHAR pchOperationName );  // id of the oper to be found
zOPER_EXPORT zLONG OPERATION
IsZeidonOperation( zPCHAR pchOperationName );  // id of the oper to be found
zOPER_EXPORT zLONG OPERATION
GetOperationZKey( zVIEW  vSubtask,
                  zLONG  lOperationClass,
                  zPCHAR pchOperationName );
zOPER_EXPORT zLONG OPERATION
GetOperationParmListPM( zVIEW  vSubtask,
                        zLONG sParmList[],
                        zLONG sParmListExt[],
                        zLONG  lZKey,
                        zLONG  lOperationClass );
zOPER_EXPORT zLONG OPERATION
GetOperationReturnType( zLONG  lOperationZKey,
                        zLONG  lOperationClass );
zOPER_EXPORT zLONG OPERATION
Priority( zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
SplitQualifier( zPCHAR pchQualifierString,    // Qualifier string to be split
                zPCHAR pchViewName,                // string to store view in
                zPCHAR pchEntityName,               // string to store Entity
                zPCHAR pchAttributeName );       // String to store attribute
zOPER_EXPORT zLONG OPERATION
VerifyQualifier( zVIEW  vSubtask,
                 zLONG  lQualiferType,
                 zPCHAR pchViewName,
                 zPLONG lpViewID,
                 zPCHAR pchEntityName,
                 zPLONG lpEntityID,
                 zPCHAR pchAttributeName,
                 zPLONG lpAttributeID );
zOPER_EXPORT zLONG OPERATION
GetLengthFromAttribute( zLONG lViewID,
                        zPCHAR pchEntityName,
                        zPCHAR pchAttributeName );
zOPER_EXPORT zLONG OPERATION
CreateLoopControlWithView( zVIEW  vSubtask,
                           zLONG  lControlType,
                           zPCHAR pchNameOrValue );
zOPER_EXPORT zLONG OPERATION
GenerateTempVariable( zVIEW  vSubtask,
                      zLONG  lDataType,
                      zLONG  lDataLength );
zOPER_EXPORT zLONG OPERATION
NewGenerateTempVariable( zVIEW  vSubtask,
                         zLONG  lDataType,
                         zLONG  lDataLength,
                         zLONG  *pNewDataType );
zOPER_EXPORT zLONG OPERATION
ReuseTempVariable( zLONG  lID );
zOPER_EXPORT zLONG OPERATION
ConvertZeidonTypeToVMLType( zCHAR cZeidonType );
zOPER_EXPORT zLONG OPERATION
ConvertVMLTypeToZeidonType( zPCHAR cZeidonType,
                            zLONG  lVMLType );
zOPER_EXPORT zLONG OPERATION
GetZeidonOperTypeFromVMLType( zPCHAR pchZeidonType,
                              zLONG  lVMLOperType );
zOPER_EXPORT zLONG OPERATION
GetOppositeOperator( zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
ReverseOperator( zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
AddReturnStatement( zVIEW  vSubtask,
                    zLONG  lVMLToken,
                    zLONG  lValue,
                    zPCHAR pchValue,
                    zLONG  lBeforeOrAfter );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm5a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
InsertForWithWhere( zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm6a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
InsertSetCursorWithWhere( zVIEW vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm7a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
OptimizeTextStatement( zVIEW  vSourceView );
zOPER_EXPORT zLONG OPERATION
CreateZeidonStringCopy( zLONG  lTargetVar,
                        zLONG  lSourceVar,
                        zLONG  lLen );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpm8a.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
OptimizeNumericExpression( zVIEW  vSourceView,
                           zLONG  lStatementType );
zOPER_EXPORT zLONG OPERATION
MoveStringExprToView( zVIEW  vReturnExpr,
                      zPCHAR pchExprCode,
                      zPLONG lpExprCodeIndex,
                      zPCHAR pchExprText,
                      zPLONG lpExprTextIndex );
zOPER_EXPORT zLONG OPERATION
MoveViewToStringExpr( zVIEW  vSubtask,
                      zVIEW  vSourceView,
                      zPCHAR pchExprCode,
                      zPLONG lpExprCode,
                      zPCHAR pchExprText,
                      zPLONG lpExprText );
zOPER_EXPORT zLONG OPERATION
TestOptimization( zVIEW  vSourceExpr,
                  zVIEW  vTargetExpr,
                  zLONG  lStartLevel );
zOPER_EXPORT zLONG OPERATION
Optimize3PartExpression( zVIEW  vSourceView,
                         zLONG  lStartLevel );
zOPER_EXPORT zLONG OPERATION
AnalyzeRightExpressionWithView( zVIEW  vSourceView,
                                zVIEW  vTargetView,
                                zLONG  lNeedTemp );
zOPER_EXPORT zLONG OPERATION
CreateOperCallWithAQs( zPCHAR pchOperationName,
                       zVIEW  vLeftView,
                       zVIEW  vRightView,
                       zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
PrependCompareWithView( zVIEW  vSourceView,
                        zVIEW  vTargetView,
                        zPCHAR pchOperationName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmpmea.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
SyntaxError( zVIEW vSubtask, zLONG lErrorNumber, zPCHAR pchErrorString );

// TZVSPMYA.C  (from tzvmpmaa.l)
int
qqparse( zVIEW vSubtask );

// TZVSPMLA.C  (from tzvmpmaa.l)
int
qqwrap( void );
void
qqcomment( zVIEW vSubtask, zPCHAR pchReadString );
void
qq_reset();
int
SetQReturnCodes( zVIEW vSubtask,
                 int   nTokenClass,
                 int   nToken );
zOPER_EXPORT zLONG OPERATION
OptimizeExpression( zVIEW  vSubtask,
                    zVIEW  vTargetView,
                    zVIEW  vSourceView );
zOPER_EXPORT zLONG OPERATION
CaseExpression( zVIEW  vSubtask,
                zVIEW  vTargetView,
                zVIEW  vSourceView );

zOPER_EXPORT zLONG OPERATION
OptimizeSimpleTextExpression( zVIEW  vSubtask,
                              zVIEW  vTargetView,
                              zVIEW  vSourceView,
                              zLONG  lAnalyzeResult,
                              zLONG  lStartLevel );
zOPER_EXPORT zLONG OPERATION
OptimizeSimpleNumericExpression( zVIEW  vSubtask,
                                 zVIEW  vTargetView,
                                 zVIEW  vSourceView,
                                 zLONG  lAnalyzeResult,
                                 zLONG  lStartLevel );
zOPER_EXPORT zLONG OPERATION
AtomizeAQ( zPCHAR pchAQ,
           zPCHAR pchViewName,
           zPCHAR pchEntityName,
           zPCHAR pchAttributeName,
           zPLONG nViewID,
           zPLONG nEntityID,
           zPLONG nAttributeID,
           zLONG  lDataClass );
zOPER_EXPORT zLONG OPERATION
OptimizeComplexExpression( zVIEW  vTargetView,
                           zVIEW  vSourceView,
                           zLONG  lAnalyzeResult,
                           zLONG  lStartLevel );
zOPER_EXPORT zLONG OPERATION
CreateOperationCallUsingAQ( zVIEW  vSubtask,
                            zVIEW  vTargetView,
                            zPCHAR pchOperationName,
                            zLONG  lViewID,
                            zLONG  lUseVariable,
                            zLONG  lVariableDataType,
                            zLONG  lVariableID,
                            zPCHAR pchEntityName,
                            zLONG  lEntityID,
                            zPCHAR pchAttributeName,
                            zLONG  lAttributeID,
                            zLONG  lUseExtra,
                            zPCHAR pchExtra,
                            zLONG  lExtra,
                            zLONG  lExtraClass );
zOPER_EXPORT zLONG OPERATION
SetupActivate( zVIEW vSubtask, zLONG lID );
zOPER_EXPORT zLONG OPERATION
GenerateLocalOperationName( zPCHAR pchReturnName,
                            zLONG  lMaxLth,
                            zPCHAR pchIncInName );
zOPER_EXPORT zLONG OPERATION
InsertActivateIntoCurrentOperation( zVIEW vSubtask, zPCHAR pchLoadControl );
zOPER_EXPORT zLONG OPERATION
InsertActivateNoWhere( zVIEW vSubtask, zPCHAR pchLoadControl );
zOPER_EXPORT zLONG OPERATION
BuildCallToLocalFunction( zVIEW  vSubtask,
                          zVIEW  vLocalView,
                          zLONG  lQualID );
zOPER_EXPORT zLONG OPERATION
BuildDropView( zVIEW vSubtask, zLONG  lQualID );
zOPER_EXPORT zLONG OPERATION
BuildActivateEmpty( zVIEW vSubtask, zLONG lLoadQual );
zOPER_EXPORT zLONG OPERATION
BuildActivate( zVIEW  vSubtask,
               zLONG  lRetViewID,
               zPCHAR pchLODName,
               zLONG  lQualViewID,
               zLONG  lControlDefineZKey );
zOPER_EXPORT zLONG OPERATION
BuildWhereStatements( zVIEW  vSubtask,
                      zLONG lRootOrRestrict );
zOPER_EXPORT zLONG OPERATION
GetRootEntityName( zPCHAR pchReturnEntityName, zLONG lMaxLth );
zOPER_EXPORT zLONG OPERATION
GetStringForSQLOperator( zPCHAR pchOperString,
                         zLONG  lMaxLth,
                         zLONG  lOperator );
zOPER_EXPORT zLONG OPERATION
BuildActivateEmptyLocal( zVIEW  vSubtask,
                         zLONG  lQualID,
                         zPCHAR pchLODName,
                         zLONG  lLoadQual );
zOPER_EXPORT zLONG OPERATION
InsertActivateEmpty( zVIEW vSubtask, zPCHAR pchLoadControl );
zOPER_EXPORT zLONG OPERATION
BuildGetView( zVIEW  vSubtask,
              zSHORT nViewID,
              zPCHAR pchViewName,
              zLONG  lViewTokenType,
              zSHORT nQualView,
              zPCHAR pchQualLevel );
zOPER_EXPORT zLONG OPERATION
BuildNameView( zVIEW  vSubtask,
               zSHORT nViewID,
               zPCHAR pchViewName,
               zSHORT nViewType,
               zSHORT nQualView,
               zPCHAR pchQualLevel );
zOPER_EXPORT zLONG OPERATION
AddEntityOperToViewWithNoScope( zVIEW  vSubtask,
                                zVIEW  vExprView,
                                zPCHAR pchOperationName,
                                zLONG  lViewID,
                                zPCHAR pchEntityName,
                                zLONG  lEntityID,
                                zLONG  lUseExtra,
                                zPCHAR pchExtra,
                                zLONG  lExtra,
                                zLONG  lOperator,
                                zLONG  lValue );
zOPER_EXPORT zLONG OPERATION
BuildCommitView( zVIEW  vSubtask,
                 zSHORT nViewID );

/////////////////////////////////////////////////////////////////////////////
//
//  tzvmecaa.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
CallInterp( zVIEW  lpInterpSubtask,     // the interpretor subtask
            zVIEW  lpAppSubtask,        // the application's subtask
            zPCHAR pchSrcDLLName,        // the source dll name
            zPCHAR pchOperationName,     // the operation name
            zPLONG plRetVal );
zOPER_EXPORT zLONG OPERATION
GetExecMode( zVIEW  lpInterpSubtask,
             zVIEW  lpAppSubtask,
             zPCHAR pchSrcDLLName,
             zPCHAR pchOperationName,
             zPCHAR pchMetaType );
zOPER_EXPORT zLONG OPERATION
GetExecModeByZKey( zVIEW  lpInterpSubtask,
                   zVIEW  lpAppSubtask,
                   zLONG  lZKey );
zOPER_EXPORT zSHORT OPERATION
SetupInterp( zVIEW vSubtask,
             zPCHAR pchAppName );
zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSystemSubtask( zPVIEW vReturnSubtask,
                            zPCHAR pchAppName );
zOPER_EXPORT zSHORT OPERATION
Exec_SfCreateSubtask( zPVIEW vReturnSubtask,
                      zPCHAR pchAppName );
zOPER_EXPORT zSHORT OPERATION
ParseSourceFromEditor( zVIEW  vSubtask,
                       zVIEW  vSourceMeta,
                       zLONG  lInParseAction,
                       zLONG  lInParseLimit,
                       zLONG  lInParseSource,
                       zPCHAR pchInSourceFileName,
                       zPCHAR pchInOperationName,
                       zCPCHAR cpcGenLang );

/////////////////////////////////////////////////////////////////////////////
//
//  tzzorpts.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SummaryReport( zVIEW  vLOD_In,
                         zPCHAR pchReportFileName );

/////////////////////////////////////////////////////////////////////////////
//
//  tzsioprs.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
 GetSironWorkObjPathName( zPCHAR pchPathName,
                          zLONG lPathName );

zOPER_EXPORT zSHORT OPERATION
CreateSironWorkObjectList( zPCHAR pchDirectoryPath,
                           zVIEW  vSubtask );

/////////////////////////////////////////////////////////////////////////////
//
//  TZsixsko.c
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zoTZSIXSKO_SaveXSK( zVIEW vTZZOLOXO, zVIEW vTZTENVRO );

/////////////////////////////////////////////////////////////////////////////
//
//  tzedfrmd for codewright interface
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CleanupLibrary( zVIEW vSubtask );            // from TZEDFRMD

/////////////////////////////////////////////////////////////////////////////
//
//  TZVSRTOO.c
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DERIVED ATTRIBUTE */  OPERATION
GetSetStringValue( zVIEW vToInstance  /*  BASED ON LOD TZVSRTOO  */,
                   LPVIEWENTITY lpViewEntity,
                   LPVIEWATTRIB lpViewAttrib,
                   zSHORT nGetOrSetFlag );

#ifdef __cplusplus
}
#endif

#endif   // #ifndef TZLODOPR_H


///////////////////////////// end of file  //////////////////////////////////
