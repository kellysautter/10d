/*
MODULE NAME:  TZVSPMAA.Y  -  VML Grammar
DESCRIPTION:  This is the YACC grammar for ZEIDON VML Language.

*******************************************************************
* Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
* All rights reserved.                                            *
* Confidential and Proprietary material subject to license -      *
* do not reproduce or disclose. This material is an unpublished   *
* work and is considered a trade secret belonging to the          *
* copyright holder.                                               *
*******************************************************************
AUTHOR:  Jeff Beam (?)
DATE:    (?)

CHANGE LOG

2001.09.27  HH
   support parens in WHERE and RESTRICTING without WHERE.

**---------------------------------------------------------------------*/

%{
#define KZSYSSVC_INCL
#include <kzoengaa.h>     // Object Services public prototypes
#include "tzlodopr.h"
#define  EXDECL  extern
#include "tzvspmaa.ht"
//#include "tzvspmaa.h"
#include "tzvspmaa.hd"
#undef   EXDECL
%}

%union{
   long     nValue;
   zPCHAR   szValue;
   double   ldValue;
};

%token   qACTIVATE            1026
%token   qAFTER               1002
%token   qEACH                1003
%token   qTATTRIBUTENAME      1004
%token   qBEFORE              1005
%token   qBASED               1006
%token   qBREAK               1059
%token   qCASE                1007
%token   qCONDOPERATOR        1009
%token   qCONSTANT            1010
%token   qCREATE              1011
%token   qCURSOR              1012
%token   qCOMMIT              1013
%token   qDELETE              1014
%token   qELSE                1015
%token   qEND                 1016
%token   qENTITY              1017
%token   qTENTITYNAME         1018
%token   qTDATE               1019
%token   qFIRST               1020
%token   qFOR                 1021
%token   qIF                  1022
%left    qJOIN                1024
%token   qLAST                1025
%token   qLOOP                1027
%token   qLOD                 1028
%token   qMULTIPLE            1063
%token   qNEXT                1030
%token   qOF                  1032
%token   qON                  1050
%token   qOPERATION           1033
%token   qOPERATOR            1034
%token   qPREVIOUS            1035
%token   qRESTRICTING         1061
%token   qRETURN              1036
%token   qROOTONLY            1064
%token   qSINGLE              1065
%token   qSET                 1038
%token   qTDECIMAL            1039
%token   qTINTEGER            1040
%token   qTO                  1062
%token   qTSHORT              1041
%token   qTSTRING             1042
%token   qUNTIL               1043
%token   qVARIABLE            1044
%token   qTVIEW               1045
%token   qWHERE               1046
%token   qWHILE               1047
%token   qWITHIN              1048
%token   qOTHERWISE           1049
%token   qROOTONLYMULTIPLE    1066
%token   qEMPTY               1067
%token   qLIKE                1068
%token   qEXISTS              1069
%token   qEXIST               1070
%token   qDOES                1071
%token   qTCHARACTER          1072
%token   qIS                  1073
%token   qNULL                1074
%token   qATTRIBUTE           1075
%token   qDERIVED             1076
%token   qCONSTRAINT          1077
%token   qTRANSFORMATION      1078
%token   qSHORTCUTKEY         1079
%token   qREGISTERED          1080
%token   qAS                  1081
%token   qGET                 1082
%token   qAT                  1083
%token   qLEVEL               1084
%token   qSUBTASK             1085
%token   qTASK                1086
%token   qAPPLICATION         1087
%token   qSYSTEM              1088
%token   qANY                 1089
%token   qNAME                1090
%token   qUSING               1091
%token   qINCLUDE             1092
%token   qEXCLUDE             1093
%token   qFROM                1094
%token   qNONE                1095
%token   qNAMED               1096
%token   qCONTINUE            1102
%token   qROOTONLYCONTINUE    1103
%token   qSINGLEFORUPDATE     1104


%token   qTUNKNOWN            1100
%token   qTVOID               1101

%token   qAND                 1051
%token   qOR                  1052

%token   qGLOBAL              1053
%token   qLOCAL               1054
%token   qOBJECT              1055
%token   qDIALOG              1056
%token   qDOMAIN              1057

%token   qRETURNED            1058
%token   qDEFINE              1060

%token   qATTRIBUTEQUALIFIER  2001
%token   qENTITYQUALIFIER     2002
%token   qRESERVEDWORD        2003
%token   qUSERDEFINED         2004
%token   qDELIMITER           2005
%token   qEXPRESSION          2006
%token   qENDFUNC             2007
%token   qOPERATIONCALL       2008
%token   qNUMERIC             2009
%token   qTEXT                2010

/* operators begin in the 3000 series */
%token   qEQUAL               3001
%left    qADDITION            3003      qMINUS               3002
%left    qMULTIPLICATION      3004      qDIVISION            3005
%nonassoc UMINUS

/* cond operators */
%token   qCONDEQUAL           3006
%token   qLESSTHANEQUAL       3007
%token   qLESSTHAN            3008
%token   qGREATERTHANEQUAL    3009
%token   qGREATERTHAN         3010
%token   qNOTEQUAL            3011
%token   qNOT                 3012

%token   qCOMMA               3013
%token   qLPAREN              3014
%token   qRPAREN              3015
%token   qLBRACE              3016
%token   qRBRACE              3017
%token   qOPENOPER            3018
%token   qCLOSEOPER           3019
%token   qSEMICOLON           3020
%token   qCOLON               3021
%token   qUNKNOWNTOKEN        3031

%token   qNOTEXISTS           3022
%token   qNOTNULL             3023

/* OPERATION CLASSES */
%token   qLOCALOPERATION      4001

%token   qDIALOGOPERATION     4004
%token   qGLOBALOPERATION     4002
%token   qDOMAINOPERATION     4005
%token   qOBJECTOPERATION     4003
%token   qZEIDONOPERATION     4006
%token   qCONTROLOPERATION    4007

%token   qBOTHVAR             5001
%token   qCONCATOPERATOR      5002

%start application
%%

/**********************************************************************\
 *                                                                    *
 *                Beginning of Grammar Rules                          *
 *                                                                    *
\**********************************************************************/

/* application is the "meta" unit of the source code. An Application
   is an operation followed by any number of operations. It is recursively
   defined to yacc as an operation followed by an application. Defining
   no operations is an error */
application :
        operation_definition
      | application operation_definition
      | error { SyntaxError( eQNOOPERATIONSDEFINED, "" ); }
      ;

/* the operation definiton, minimally is an operation_declaration
   followed by an END. That is the operation may be stubbbed out. The possible
   forms of an operation are listed below. The grammer is set up to find the
   larges possible form of an operation definition first. */
operation_definition :
        /* the first possible operation definition is one which has all the
            possible parts: a declaration, parameters,
            statements and an END */
        operation_declaration
              parameter_definition_list
              operation_body
              { ActionQ( aQTURNOFFTEXTSAVE ); }
           end_clause { ActionQ( aQINSERTOPERATIONENDSTMT ); }

      /* the second possible type is a declaration, NO
         parameters, statements and an end.
         the last type is an operation declaration, an operation body
         and the END. This is the simplest type. As was mentioned above,
         an operation can consist of just OPERATION name END. The way this
         is implimented is by allowing an opertation_body to have
         no statements. See operation_body for more detail. */
      | operation_declaration
              operation_body
              { ActionQ( aQTURNOFFTEXTSAVE ); }
           end_clause { ActionQ( aQINSERTOPERATIONENDSTMT ); }
      ;

/* an operation declaration contains the keyword OPERATION followed
   by a symbol which is the name if the operation. Is may seem silly
   to break down the grammer this far, particularly since the only place
   an operation declaration would be used to to define an operation,
   however, since we want to be as specific as possible in the actions,
   ( to speed up processing ) and in the error messages ( to be as exact
   as possible ) I chose to break it down this far. This also occurs in
   other places in the grammer. */
operation_declaration :
        /* operation followed by operation name */
        operation_type qOPERATION
           symbol { ActionQ( aQADDOPERATIONBYNAME ); }
        /* error condition as the operation name was not included */
      | operation_type qOPERATION
           error { SyntaxError( eQNOOPERATIONNAMEGIVEN,"" ); }
      ;

operation_type :
        qLOCAL  { ActionQ( aQSETOPERATIONTYPE ); }
      | qGLOBAL { ActionQ( aQSETOPERATIONTYPE ); }
      | qDIALOG { ActionQ( aQSETOPERATIONTYPE ); }
      | qDOMAIN { ActionQ( aQSETOPERATIONTYPE ); }
      | qOBJECT { ActionQ( aQSETOPERATIONTYPE ); }  qCONSTRAINT
      | qENTITY { ActionQ( aQSETOPERATIONTYPE ); }  qCONSTRAINT
      | qTRANSFORMATION { ActionQ( aQSETOPERATIONTYPE ); }
      | qDERIVED        { ActionQ( aQSETOPERATIONTYPE ); } qATTRIBUTE
      | qSHORTCUTKEY    { ActionQ( aQSETOPERATIONTYPE ); }
      | error { SyntaxError( eQOPERATIONTYPENOTGIVEN,"" ); }
      ;

/* a parameter_definition_list is a list of parameter-types. The only place
   this will exist is on the definition of an operation. If the
   operation is called with parameters then the definiton must specify
   what these parameters are. The parameter_list is delimited by an
   l and r-paren to seperate them from the normal data_type declarations. A
   parameter list can also be just an empty ( ) lapren rparen. This is
   the same as not having a parmater list but can be more readable in
   some cases. */
parameter_definition_list :
        qLPAREN { ActionQ( aQBEGINPARMLIST ); }
           parameter_declaration_list qRPAREN
              { ActionQ( aQENDPARMLIST ); }
      | qLPAREN { ActionQ( aQBEGINPARMLIST ); }
           qRPAREN { ActionQ( aQENDPARMLIST ); }
      ;

/* a parameter declaration list is recursivly defined as a
   parameter_declaration followed by 0-n more parameter declaration. Notice
   that this means that there must be at least one parameter declaration
   at this point. If there are no parms, it would have been caught in the
   previous statement with qLPAREN qRPAREN. */
parameter_declaration_list :
        /* a single declaration */
        parameter_declaration
        /* multiple declarations */
      | parameter_declaration_list qCOMMA
           parameter_declaration             /* <-- recursive definition */
      | parameter_declaration_list
           error  { SyntaxError( eQPARAMETERSNOTDELIMITED, "" ); }
           parameter_declaration             /* <-- recursive definition */
      ;

/* currently, only local variables can be passed to an operation. */
parameter_declaration :
        local_declaration { ActionQ( aQMARKLASTVARASPARM ); }
      | pointer_indicator
           local_declaration { ActionQ( aQMARKLASTVARASPARMPTR ); }
      | view_declaration  { ActionQ( aQMARKLASTVARASPARM ); }
      | pointer_indicator
            view_declaration { ActionQ( aQMARKLASTVARASPARMPTR ); }
      ;

/* the operation body is made op of nothing, declarations, statements,
   or declarations and statements. This is the crux (wanted to use that
   word somewhere) of what the operation is to do. The order is either
   a declaration_list, that is a list of locally defined variables
   and/or object instance statements, a statement_list, a list of
   executable statements, both a declaration list and a statement list,
   or nothing. As mentioned earlier, the empty list is the vay in which
   a "stubbed out" operation is implimented. NO statement and NO
   declarations is an empty body. No body, no actions. ( also a
   dating philosophy ). No calls to ActionQ are necessary as all the
   calls have been done down on the statement level */
operation_body :
        declaration_list
      | statement_list
      | declaration_list statement_list
      | statement_list
           declaration_list { SyntaxError( eQDECLMUSTPRECEEDSTMTS, "" ); yyerrok; }
      |   /* No statements is a valid case */
      ;

/* the end clause inserts an end statement so that it can be intepreted.
   this is pretty much a time waster, however, for debugging it looks
   nice to stop at the end. */
end_clause :
        qEND { ActionQ( aQINSERTENDSTMT ); }
      ;

/* local declarations can be either local variables or object

   instance/view  declarations. ActionQs and error messages occur at more
   atomic level than this, therefore, neither actions nor errors need
   to be reported at this level */
declaration_list :
        declaration                              /* No ActionQs Necessary */
      | declaration_list declaration        /* No ActionQs Necessary */
      ;


/* a declaration is a local declaration or an object instance statement */
declaration :
        local_declaration
      | view_declaration
      ;

/* a local declaration is really a local variable declaration or a parameter
   list declaration. The format is data_type ( see above ) followed by a
   variable list. */
local_declaration :
        data_type symbol { ActionQ( aQDECLARELOCALVARIABLE ); }
      | data_type error     { SyntaxError( eQNOVARIABLENAMEGIVEN, "" ); }
      ;

/* a data_type is the way a local symbol is declared or a parameter is
   passed. The format is one of the keywords:
      INTEGER, DECIMAL, STRING, ENTITYNAME, ATTRIBUTENAME,
   It is broken out here for lower level error reporting */
data_type :
        qTINTEGER          { ActionQ( aQSETLASTDATATYPEINTEGER ); }
      | qTDECIMAL          { ActionQ( aQSETLASTDATATYPEDECIMAL ); }
      | qTSTRING dimension { ActionQ( aQSETLASTDATATYPESTRING ); }
      | qTSTRING           { SyntaxError( eQNODIMENSIONFORSTRING, "" ); yyerrok; }
      | qTENTITYNAME       { ActionQ( aQSETLASTDATATYPEENTITYNAME ); }
      | qTATTRIBUTENAME    { ActionQ( aQSETLASTDATATYPEATTRIBUTENAME ); }
      | qTSHORT            { ActionQ( aQSETLASTDATATYPESHORT ); }
      | qTCHARACTER        { ActionQ( aQSETLASTDATATYPECHARACTER ); }
      ;

pointer_indicator :
         qRETURNED
      ;

/* a dimension is the parenthesizes length of a declared string. ie
   STRING ( 33 ) szString Notice that the dimension comes after the data_type
   and not after the name of the variable. */
dimension :
        qLPAREN qCONSTANT { ActionQ( aQSETSTRINGDEFINEDLENGTH ); } qRPAREN
      | qLPAREN { SyntaxError( eQNODIMENSIONFORSTRING, "" ); yyerrok; } qRPAREN
      ;

/* a view declaration is where the views that are to be used are declared
   The format is VIEW followed by the name of the view. That
   is followed by the FOR OBJECT DEFINITION phrase (strickly for
   readability ) and the object definition name */
view_declaration :
        view_name view_type
      | view_name
      | view_name error { SyntaxError( eQINVALIDVIEWDECLARATION, "" ); }
      ;

/* the view name is the name to be used for the view variable after the
   keyword view */
view_name :
        qTVIEW symbol { ActionQ( aQADDVIEWVARIABLE ); }
      ;

/* an optional phrase that gives the view LOD def name */
view_type :
        qBASED qON qLOD
           symbol { ActionQ( aQLOADDEFASINSTANCE ); }
      | qREGISTERED qAS
           symbol { ActionQ( aQLOADDEFFROMVOR ); }
      ;

/* a statement_list is a statement followed by any number of statements. */
statement_list :
        multiple_statements
      | exit_statement
      | multiple_statements exit_statement
      ;

multiple_statements :
        statement
      | multiple_statements statement
      ;

/* statements are the basic unit of procedure in VML as in all
   languages. Statements consists of one statement followed by
   0-N other statements. The types of statements are:
   expression, condition, selection, loop, and exit. */
statement :
        conditional_statement
      | loop_statement
      | selection_statement
      | action_statement
      | assignment_statement
      ;

/* a conditional statement is essentially an if/else statement. It
   exist in a number of ways:
      the IF END with no statements nor else
      the IF statements END with no else
      either of the above with an else phrase which is
      ELSE END
      ELSE statements END.
   note that if the ELSE exists, no END will be required on the IF. */
conditional_statement :
        if_phrase end_clause
      | if_phrase else_phrase end_clause
      | if_phrase error { SyntaxError( eQNOENDFORIFSTMT, "" ); }
      ;


/* the loop statement has several options which allow it to be
   used for any number of looping constructs. All loop statements begin
   with the loop phrase ( again broken out for reporting purposes ) and
   is followed by the keywords necessary to control the loop type:
      UNTIL: this creates a post-check loop. That is, loop followed
         by statements followed by an UNTIL will iterate at least once.
      WHILE: this create a pre-check loop. That is a loop followed by
         a while will check the condition of the while first before
         entering the loop statement.
      FOR: this creates a variable controlled loop for a fixed number of
         iterations.
*/

loop_statement :
        loop_phrase
        until_phrase
      | loop_phrase
           statement_list
        until_phrase
      | loop_phrase while_clause
        end_clause
      | loop_phrase while_clause
           statement_list
        end_clause
      | loop_phrase while_clause
        error { SyntaxError( eQWHILECLAUSENEVERENDED, "" ); }
      | loop_phrase while_clause
           statement_list
        error { SyntaxError( eQWHILECLAUSENEVERENDED, "" ); }
      ;

until_phrase :
        qUNTIL compound_condition { ActionQ( aQINSERTUNTILSTMT ); }
      | qUNTIL multiple_condition { ActionQ( aQINSERTUNTILSTMT ); }
      | qUNTIL error { SyntaxError( eQUNTILNEEDSCONDITION, "" );  }
      ;

/* the selection statement is synonymous to a switch in 'C' or
   a case in Pascal. The Selection statement keys on some constant
   value  which it attempts to match to an appropriate of_list as
   defined above. Of_lists are optional and are 0 to N. The case
   is followed by and end. */
selection_statement :
        case_phrase
           of_list /*  { ActionQ( aQEXTRAENDNEEDED ); } */
        end_clause
      | case_phrase
           otherwise { ActionQ( aQEXTRAENDNEEDED ); }
        end_clause
      | case_phrase
           of_list
           otherwise { ActionQ( aQEXTRAENDNEEDED ); }
        end_clause
      | case_phrase
           of_list
           otherwise
        error { SyntaxError( eQCASENEVERENDED, "" ); yyerrok; }
      | case_phrase
           of_list
        error { SyntaxError( eQCASENEVERENDED, "" ); yyerrok; }
      | case_phrase
           otherwise
        error { SyntaxError( eQCASENEVERENDED, "" ); yyerrok; }
      | case_phrase
        error { SyntaxError( eQCASENEVERENDED, "" ); yyerrok; }
      | case_phrase
           otherwise
           of_list { SyntaxError( eQOFCANTFOLLOWOTHERWISE, "" ); yyerrok; }
        end_clause
      | case_phrase { ActionQ( aQADDLONEDEFAULT ); }
        end_clause
      ;

/* an action_statement is a statement that performs some sort of
   core type action. Things like activating instances, positioning cursors,
   creating or deleteing entities etc. */
action_statement :
        set_action
      | for_action
      | activate_action
      | commit_action
      | create_action
      | delete_action
      | get_action
      | name_action
      | include_action
      | exclude_action
      ;

/* the assignment statement is any expression that is valid in VML. An
   assignment can be an operation call ( function call ) or an
   assignment. Operation calls are separated because one cannot set
   an operation call equal to a value. */
assignment_statement :
        operation_call { ActionQ( aQINSERTOPERATIONCALL ); }
      | left_element qEQUAL { ActionQ( aQADDASSIGNMENTOPERATOR ); }
            r_expression  { ActionQ( aQINSERTEXPRESSIONSTMT ); }
      ;

/* the exit statement is the immediate return from an operation. i.e
   alters the flow of control. The keyword is RETURN followed by a
   value if one is expected to be returned. */
exit_statement :
        return_value { ActionQ( aQADDRETURNSTATEMENT ); }
      ;

/* the if phrase is an IF with a compound condition. Statements
   dependant on the if are OPTIONAL */
if_phrase :
        qIF compound_condition { ActionQ( aQINSERTIFSTMT ); }
      | qIF multiple_condition { ActionQ( aQINSERTIFSTMT ); }
      | qIF compound_condition { ActionQ( aQINSERTIFSTMT ); } statement_list
      | qIF multiple_condition { ActionQ( aQINSERTIFSTMT ); } statement_list
      | qIF error { SyntaxError( eQNOCONDITIONAFTERIF, "" ); } statement_list
      ;

/* the ELSE phrase is an ELSE only. Statements dependant on the
   ELSE are OPTIONAL */
else_phrase :
        qELSE { ActionQ( aQINSERTELSESTMT ); }
      | qELSE { ActionQ( aQINSERTELSESTMT ); } statement_list
      ;

/* loop phrase is the keyword loop */
loop_phrase :
         qLOOP { ActionQ( aQINSERTLOOPSTMT ); }
      ;

/* a compound condition can be a simple condition or multiple conditions
   joined by a conditional_join, that is an AND or an OR. It also
   allows the use of parens around a condition if the need for grouping
   multiple conditions is needed. */
/* WRONG RECURSION */
compound_condition :
        condition
      | condition conditional_join compound_condition
      | condition conditional_join multiple_condition
      ;

multiple_condition :
        lparen compound_condition rparen
      | lparen compound_condition rparen conditional_join multiple_condition
      | lparen compound_condition rparen conditional_join compound_condition
      | lparen multiple_condition rparen
      | lparen multiple_condition rparen conditional_join multiple_condition
      | lparen multiple_condition rparen conditional_join compound_condition
      | negation_op multiple_condition
      ;

/* the while_clause also modifies the loop statement. A condition is
   given that is checked on each iteration. */
while_clause :
        while_keyword compound_condition { ActionQ( aQINSERTWHILESTMT ); }
      | while_keyword multiple_condition { ActionQ( aQINSERTWHILESTMT ); }
      | while_keyword error { SyntaxError( eQWHILENEEDSCONDITION, "" );  }
      ;

/* the case phrase is a unique production within the selection statement
   however, due to the low level of error reporting with which we desire
   it has been broken out into a case_phrase. */
case_phrase :
          qCASE r_expression { ActionQ( aQINSERTCASESTMT ); }
/*      | qCASE error { SyntaxError( eQCASENEEDSAVALUETOTEST, "" ); yyerrok; }
*/      ;

/* an of list is a group of of_clauses followed by a list of
   dependant statements. */
of_list:
        of_list of_clause statement_list { ActionQ( aQEXTRAENDNEEDED ); }
      | of_list of_clause { ActionQ( aQEXTRAENDNEEDED ); }
      | of_clause statement_list { ActionQ( aQEXTRAENDNEEDED ); }
      | of_clause { ActionQ( aQEXTRAENDNEEDED ); }
      ;
/*
of_list:
        of_clause statement_list
      | of_clause
      | of_list of_clause statement_list
      | of_list of_clause
      ;
*/
otherwise :
        qOTHERWISE qCOLON { ActionQ( aQINSERTOTHERWISESTMT ); } statement_list
      | qOTHERWISE qCOLON { ActionQ( aQINSERTOTHERWISESTMT ); }
      ;

set_action :
        qSET qCURSOR fnpl_phrase cursor_target
            { ActionQ( aQSETCURSORNOWHERE ); }
      | qSET qCURSOR fnpl_phrase cursor_target
            where_clause { ActionQ( aQSETCURSORWITHWHERE ); }
      ;

commit_action :
        qCOMMIT symbol { ActionQ( aQADDCOMMIT ); }
      ;

get_action :
        qGET qTVIEW view_var
           task_level
              qUSING symbol { ActionQ( aQADDGETVIEWATLEVELFROMLEVEL ); }
      | qGET qTVIEW view_var
           task_level { ActionQ( aQADDGETVIEWATLEVELU ); }
      | qGET qTVIEW view_var
            { ActionQ( aQADDGETVIEW ); }
      ;

name_action :
        qNAME qTVIEW name_var task_level
            qUSING symbol
            { ActionQ( aQADDNAMEVIEWWITHVIEW ); }
      | qNAME qTVIEW name_var task_level
            { ActionQ( aQADDNAMEVIEW ); }
      | qNAME qTVIEW name_var
            { ActionQ( aQADDNAMEVIEW ); }
      ;

include_action :
        qINCLUDE qENTITYQUALIFIER { ActionQ( aQSETINCLUDETARGET ); }
            source_eq position
      ;

source_eq :
        qFROM qENTITYQUALIFIER { ActionQ( aQSETINCLUDESOURCE ); }
      ;

position :
        qBEFORE { ActionQ( aQADDINCLUDEBEFORE ); }
      | qAFTER { ActionQ( aQADDINCLUDE ); }
      | { ActionQ( aQADDINCLUDE ); }
      ;

exclude_action :
        qEXCLUDE qENTITYQUALIFIER { ActionQ( aQSETINCLUDETARGET ); }
            exclude_position
      ;

exclude_position :
        qBEFORE { ActionQ( aQADDEXCLUDEBEFORE ); }
      | qNONE { ActionQ( aQADDEXCLUDENONE ); }
      | qAFTER { ActionQ( aQADDEXCLUDE ); }
      | { ActionQ( aQADDEXCLUDE ); }
      ;

name_var :
        symbol { ActionQ( aQSAVENAMEVIEWNAME ); }
            qCONSTANT { ActionQ( aQSAVENAMEVIEWCONSTANT ); }

      | symbol { ActionQ( aQSAVENAMEVIEWNAME ); }
            symbol { ActionQ( aQSAVENAMEVIEWVARIABLE ); }
      ;

view_var :
        symbol { ActionQ( aQSAVEGETVIEWID ); }
           qNAMED view_qual
      ;

view_qual:
        qCONSTANT { ActionQ( aQSAVEGETVIEWCONSTANT ); }
      | symbol { ActionQ( aQSAVEGETVIEWNAME ); }
      ;

task_level :
         qAT qLEVEL qSUBTASK { ActionQ( aQADDGETVIEWATLEVEL ); }
      |  qAT qLEVEL qTASK    { ActionQ( aQADDGETVIEWATLEVEL ); }
      |  qAT qLEVEL qAPPLICATION { ActionQ( aQADDGETVIEWATLEVEL ); }
      |  qAT qLEVEL qSYSTEM { ActionQ( aQADDGETVIEWATLEVEL ); }
      |  qAT qLEVEL qANY    { ActionQ( aQADDGETVIEWATLEVEL ); }
      ;

for_action :
        for_entry
           statement_list { ActionQ( aQSETNEXT ); }
        end_clause

      | for_entry  { ActionQ( aQSETNEXT ); }
        end_clause
      ;

for_entry :
        qualified_for cursor_target
           where_clause { ActionQ( aQADDFORWITHWHERE ); }
      | qualified_for cursor_target { ActionQ( aQADDFORNOWHERE ); }
      ;

qualified_for  :
         qFOR { ActionQ( aQSETEACHFLAG ); }     qEACH
      |  qFOR { ActionQ( aQSETVARIABLEFLAG ); } symbol
      |  qFOR { ActionQ( aQSETCONSTANTFLAG ); } qCONSTANT
      |  qFOR { ActionQ( aQSETNOEACHFLAG ); }
      ;

cursor_target :
         qENTITYQUALIFIER { ActionQ( aQSETCURSORLASTCONTROL ); }
            scope
      |  qENTITYQUALIFIER { ActionQ( aQSETCURSORLASTCONTROL ); }
      ;

scope:
       qWITHIN qENTITYQUALIFIER { ActionQ( aQSETSCOPE ); }
     | qWITHIN qOBJECT          { ActionQ( aQSETOBJECTSCOPE ); }
    ;

activate_action :
        activate_phrase
            where_clause { ActionQ( aQADDACTIVATEWHERE ); }
            restricting_phrase { ActionQ( aQADDACTIVATESTMT ); }
      | activate_phrase
            where_clause { ActionQ( aQADDACTIVATEWHERE ); }
                         { ActionQ( aQADDACTIVATESTMT ); }
      | activate_phrase
            restricting_phrase { ActionQ( aQADDACTIVATESTMT ); }
      | activate_phrase qEMPTY { ActionQ( aQADDACTIVATESTMTEMPTY ); }
      | activate_phrase { ActionQ( aQADDACTIVATESTMTNOWHERE ); }
      ;

activate_phrase :
        qACTIVATE symbol { ActionQ( aQVERIFYACTIVATEOBJECT ); }
           zdefine
      | qACTIVATE symbol { ActionQ( aQVERIFYACTIVATEOBJECT ); }
           { ActionQ( aQSAVEACTIVATESINGLE ); }
      ;

zdefine :
        qSINGLE   { ActionQ( aQSAVEACTIVATESINGLE ); }
      | qMULTIPLE { ActionQ( aQSAVEACTIVATEMULTIPLE ); }
      | qROOTONLY { ActionQ( aQSAVEACTIVATEROOTONLY ); }
      | qROOTONLYMULTIPLE { ActionQ( aQSAVEACTIVATEROOTONLYMULTIPLE ); }
      | qCONTINUE { ActionQ( aQSAVEACTIVATECONTINUE ); }
      | qROOTONLYCONTINUE { ActionQ( aQSAVEACTIVATEROOTONLYCONTINUE ); }
      | qSINGLEFORUPDATE { ActionQ( aQSAVEACTIVATESINGLEFORUPDATE ); }
      ;

restricting_phrase :
        restricting_phrase restricting_clause
      | restricting_clause
      ;

restricting_clause :
        qRESTRICTING qENTITYQUALIFIER { ActionQ( aQSETRESTRICTION ); }
           qTO attribute_where_phrase { ActionQ( aQADDRESTRICTACTIVATEWHERE ); }
      ;

create_action :
        qCREATE qENTITY qENTITYQUALIFIER { ActionQ( aQADDCREATESTMTAFTER ); }  qAFTER
      | qCREATE qENTITY qENTITYQUALIFIER { ActionQ( aQADDCREATESTMTBEFORE ); } qBEFORE
      | qCREATE qENTITY qENTITYQUALIFIER { ActionQ( aQADDCREATESTMTFIRST ); } qFIRST
      | qCREATE qENTITY qENTITYQUALIFIER { ActionQ( aQADDCREATESTMTLAST ); } qLAST
      | qCREATE qENTITY qENTITYQUALIFIER { ActionQ( aQADDCREATESTMTAFTER ); }
      ;

delete_action :
        qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTFIRST ); } qFIRST
      | qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTLAST ); } qLAST
      | qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTPREVIOUS ); } qPREVIOUS
      | qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTNEXT ); } qNEXT
      | qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTNONE ); } qNONE
      | qDELETE qENTITY qENTITYQUALIFIER { ActionQ( aQADDDELETESTMTNEXT ); }
      ;

/* operation call is the name of an operation followed by a parameter
   value list. i.e. list of values */
operation_call :
        symbol { ActionQ( aQADDOPERATIONNAMETOEXPR ); }
            parameter_value_list
      ;


/* a left element is anything that can appear on the left of an EQUAL
   sign. That is, something that can have a value assigned to it. In VML
   the only 2 things are a VARIABLE and a OCEA structure */
left_element :
        symbol { ActionQ( aQADDVARIABLEOFFSETASTARGET ); } offset
      | symbol { ActionQ( aQADDVARIABLEASTARGET ); }
      | qATTRIBUTEQUALIFIER { ActionQ( aQADDOCEATOEXPRASTARGET ); }
      ;

/* a right expression is any expression that can exist on the right
   of the equal sign in an expression */
r_expression :
        r_expression qADDITION { ActionQ( aQADDOPERATORTOEXPR ); }
           r_expression
      | r_expression qMINUS { ActionQ( aQADDOPERATORTOEXPR ); }
           r_expression
      | r_expression qMULTIPLICATION { ActionQ( aQADDOPERATORTOEXPR ); }
           r_expression
      | r_expression qDIVISION { ActionQ( aQADDOPERATORTOEXPR ); }
           r_expression
      | lparen r_expression rparen
      | qMINUS { ActionQ( aQADDUNARYOPERATOR ); }
           r_expression %prec UMINUS
      | simple_expression
      ;
return_value :
        qRETURN r_expression
      | qRETURN
      ;

/* a condition is any statement that compares two expressions. A simple
   condition ( which this is ) is a r_expression a conditional
   operator and another r_expression. For example: i < 9. Notice
   the equal sign is explicited checked. This is because an equal sign
   used in any other type of expression is for assignment, therefore
   it is not listed as a conditional operator. */

condition :
        r_expression qCONDOPERATOR { ActionQ( aQADDCONDOPERATORTOEXPR ); }
           r_expression { ActionQ( aQCHECKSTOREDOCEA ); }
      | r_expression qEQUAL { ActionQ( aQADDCONDEQUALTOEXPR ); }
           r_expression { ActionQ( aQCHECKSTOREDOCEA ); }
      | exist_condition { ActionQ( aQADDCHECKEXIST ); }
      | r_expression qCONDOPERATOR
           error { SyntaxError( eQINVALIDCONDEXPRESSION, "" ); }
      ;

exist_condition :
        qENTITYQUALIFIER { ActionQ( aQADDWHERELEFTEQCOND ); }
           exist_phrase { ActionQ( aQINCREMENTWHEREINDEX ); }
      ;

/* a conditional join is either the token AND or the token OR
   that can be used to to a boolean join on two conditional
   expressions */
conditional_join :
        qJOIN { ActionQ( aQADDJOINTOEXPR ); }
      ;

/* this is only separated out so that some action can be performed
   without creating a reduce error in YACC */
lparen :
        qLPAREN { ActionQ( aQADDLPARENTOEXPR ); }
      ;

rparen :
        qRPAREN { ActionQ( aQADDRPARENTOEXPR ); }
      ;

/* ##### */
negation_op :
        qNOT { ActionQ( aQADDNOTOPERATORTOEXPR ); }
      ;


while_keyword :
        qWHILE  { ActionQ( aQDELETELOOPSTMT ); }
      ;

/* an of clause creates a label for a case statement to branch. It
   consistes of an OF keyword followed by some constant and a colon
   to delimit it from the first statement. Current thinking is that
   the constant can be any numeric or string constant, however, this
   thinking may have to change. */
of_clause:
        qOF aconstant  { ActionQ( aQADDCONSTANTTOEXPR ); }
               qCOLON { ActionQ( aQINSERTOFSTMT ); }
      | qOF qDEFINE { ActionQ( aQADDDEFINETOEXPR ); }
               qCOLON { ActionQ( aQINSERTOFSTMT ); }
      ;

fnpl_phrase :
        qFIRST     { ActionQ( aQSETFIRSTCURSOR  ); }
      | qNEXT      { ActionQ( aQSETNEXTCURSOR  ); }
      | qPREVIOUS  { ActionQ( aQSETPREVIOUSCURSOR  ); }
      | qLAST      { ActionQ( aQSETLASTCURSOR  ); }
      ;

where_clause :
        qWHERE { ActionQ( aQSETUPWHERE ); } attribute_where_phrase
      ;

attribute_where_phrase :
        attribute_where_phrase qJOIN { ActionQ( aQADDWHEREJOIN ); }
        attribute_where_phrase
      | qLPAREN { ActionQ( aQSETWHERELEFTPAREN ); } attribute_where_phrase
        qRPAREN { ActionQ( aQSETWHERERIGHTPAREN ); }
      | attribute_op_phrase
      ;

attribute_op_phrase :
        qATTRIBUTEQUALIFIER { ActionQ( aQADDWHERELEFTAQ ); }
           where_op where_value { ActionQ( aQINCREMENTWHEREINDEX ); }

      | qATTRIBUTEQUALIFIER { ActionQ( aQADDWHERELEFTAQ ); }
           is_null_phrase { ActionQ( aQINCREMENTWHEREINDEX ); }

      | qENTITYQUALIFIER { ActionQ( aQADDWHERELEFTEQ ); }
           exist_phrase { ActionQ( aQINCREMENTWHEREINDEX ); }

      | qENTITYQUALIFIER { ActionQ( aQADDWHERELEFTEQ ); } is_keyword
          qENTITYQUALIFIER { ActionQ( aQADDWHERERIGHTEQ ); }
      ;

where_op :
        qCONDOPERATOR { ActionQ( aQADDWHERECONDOPER ); }
      | qEQUAL        { ActionQ( aQADDWHERECONDEQUAL ); }
      | qLIKE         { ActionQ( aQADDWHERELIKE ); }
      ;

exist_phrase :
        qEXISTS { ActionQ( aQADDWHEREEXISTS ); }
      | qDOES qNOT qEXIST
           { ActionQ( aQADDWHERENOTEXIST ); }
      ;

is_null_phrase :
        is_keyword  qNOT { ActionQ( aQCONFIRMNOT ); }
            qNULL { ActionQ( aQADDWHEREISNOTNULL ); }
      | is_keyword  qNULL { ActionQ( aQADDWHEREISNULL ); }
      ;

is_keyword :
        qIS { ActionQ( aQADDWHEREIS ); }
      ;

where_value :
        symbol           { ActionQ( aQADDWHEREVARIABLE ); }
      | aconstant        { ActionQ( aQADDWHERECONSTANT ); }
      | qATTRIBUTEQUALIFIER { ActionQ( aQADDWHERERIGHTAQ ); }
      ;

/* a constant is any literal that can be taken as is. This includes,
   integers, decimal numbers, and strings. These are tokens that
   represent themselves. */
aconstant :
        qMINUS { ActionQ( aQADDUNARYOPERATOR ); } qCONSTANT
      | qCONSTANT
      ;



/* a parameter value list is a parameterized list of values that denote
   a set of values to be sent into an operation. The list is seperated
   by commas. If an operation is called and no parameters are required,
   the l and r-parens are still needed to process the token as an
   operation call */
parameter_value_list :
        openoper { ActionQ( aQADDOPENOPERTOEXPR ); } value_list closeoper
      | openoper { ActionQ( aQADDOPENOPERTOEXPR ); } closeoper
      ;

openoper :
        qLPAREN
      ;

closeoper :
        qRPAREN { ActionQ( aQADDCLOSEOPERTOEXPR ); }
      ;

simple_expression :
        value
      | operation_call
      ;

/* a value is any token that can be evaluated to a single value.
   Symbols, OCEA's, integers, decimals, and strings can be evaluated
   to a single value. */
value :
        symbol  { ActionQ( aQADDVARIABLETOEXPR ); }
      | symbol  { ActionQ( aQADDVARIABLEOFFSETTOEXPR ); } offset
      | qDEFINE        { ActionQ( aQADDDEFINETOEXPR ); }
      | qATTRIBUTEQUALIFIER { ActionQ( aQADDAQTOEXPR ); }
      | qCONSTANT { ActionQ( aQADDCONSTANTTOEXPR ); }
      | error { SyntaxError( eQBADTOKENWHEREVALUEEXPECTED, "" ); }
      ;

/* a value list is one to N values delimited by commas. See above */
value_list :
        r_expression
      | r_expression qCOMMA { ActionQ( aQADDCOMMATOEXPR ); } value_list
      ;


offset :
        qLBRACE { ActionQ( aQADDLBRACETOEXPR ); }
           index_and_length
              qRBRACE  { ActionQ( aQADDRBRACETOEXPR ); }
      ;

index_and_length :
        r_expression
           qCOLON { ActionQ( aQADDCOLONTOEXPR ); }
              r_expression
      | r_expression
      ;

symbol :
       qVARIABLE  { ActionQ( aQSETVARIABLETEXT ); }
      ;
%%
