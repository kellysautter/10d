/*
   Redefinitions of YACC parser globals to permit use of
   db_QUERY with other YACC based programs.
*/
#define yyparse qqparse
#define yylval qqlval
#define yyval qqval
#define yyexca qqexca
#define yyact qqact
#define yypact qqpact
#define yypgo qqpgo
#define yyr1 qqr1
#define yyr2 qqr2
#define yychk qqchk
#define yydef qqdef
#define yydebug qqdebug
#define yyv qqv
#define yychar qqchar
#define yynerrs qqnerrs
#define yyerrflag qqerrflag
#define yylex qqlex
#define yyerror qqerror
/* vpp @win.vpp PARSER.H (Thu May 21 10:47:33 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

