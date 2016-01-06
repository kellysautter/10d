/*
MODULE NAME:  TZVSPMAA.L  -  VML Grammar
DESCRIPTION:  This is the LEX grammar for ZEIDON VML Language.

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
   corrected qqerror.

**---------------------------------------------------------------------*/

%{
#define KZSYSSVC_INCL
#include <kzoengaa.h>     // Object Services public prototypes
#include "tzlodopr.h"
#define  EXDECL extern
#include "tzvspmaa.ht"
#include "tzvspmaa.h"
#include "tzvspmaa.hd"
#undef   EXDECL
#undef  YYLMAX
#define YYLMAX 1024
%}
/* supported numeric formats integral integer and {N}.N float only */
%e 1400
%p 3000
dDIGIT              [0-9]
dINTLITERAL         {dDIGIT}+
dDECLITERAL         {dINTLITERAL}?[.]{dDIGIT}+

/* supported alpha types letters, underscore and a dollar sign */
dSPECIAL            [#%@\\]
dALPHA              [a-zA-Z_$]
dSYM                {dALPHA}({dALPHA}|{dDIGIT}|{dSPECIAL})*
dATTRIBUTEQUALIFIER {dSYM}[.]{dSYM}[.]{dSYM}
dENTITYQUALIFIER    {dSYM}[.]{dSYM}
dACTIVATE           [Aa][Cc][Tt][Ii][Vv][Aa][Tt][Ee]
dAFTER              [Aa][Ff][Tt][Ee][Rr]
dAND                [Aa][Nn][Dd]
dANY                [Aa][Nn][Yy]
dAS                 [Aa][Ss]
dAT                 [Aa][Tt]
dAPPLICATION        [Aa][Pp][Pp][Ll][Ii][Cc][Aa][Tt][Ii][Oo][Nn]
dATTRIBUTENAME      [Aa][Tt][Tt][Rr][Ii][Bb][Uu][Tt][Ee][Nn][Aa][Mm][Ee]qminus
dATTRIBUTE          [Aa][Tt][Tt][Rr][Ii][Bb][Uu][Tt][Ee]
dBASED              [Bb][Aa][Ss][Ee][Dd]
dBEFORE             [Bb][Ee][Ff][Oo][Rr][Ee]
dCASE               [Cc][Aa][Ss][Ee]
dTCHARACTER         [Cc][Hh][Aa][Rr][Aa][Cc][Tt][Ee][Rr]
dCONDOPERATOR       [Cc][Oo][Nn][Dd][Oo][Pp][Ee][Rr][Aa][Tt][Oo][Rr]
dCONSTRAINT         [Cc][Oo][Nn][Ss][Tt][Rr][Aa][Ii][Nn][Tt]
dCONTINUE           [Cc][Oo][Nn][Tt][Ii][Nn][Uu][Ee]
dCOMMIT             [Cc][Oo][Mm][Mm][Ii][Tt]
dCREATE             [Cc][Rr][Ee][Aa][Tt][Ee]
dCURSOR             [Cc][Uu][Rr][Ss][Oo][Rr]
dDELETE             [Dd][Ee][Ll][Ee][Tt][Ee]
dDERIVED            [Dd][Ee][Rr][Ii][Vv][Ee][Dd]
dDOMAIN             [Dd][Oo][Mm][Aa][Ii][Nn]
dDOES               [Dd][Oo][Ee][Ss]
dDIALOG             [Dd][Ii][Aa][Ll][Oo][Gg]
dEACH               [Ee][Aa][Cc][Hh]
dELSE               [Ee][Ll][Ss][Ee]
dEMPTY              [Ee][Mm][Pp][Tt][Yy]
dEND                [Ee][Nn][Dd]
dENTITY             [Ee][Nn][Tt][Ii][Tt][Yy]
dENTITYNAME         [Ee][Nn][Tt][Ii][Tt][Yy][Nn][Aa][Mm][Ee]
dEXCLUDE            [Ee][Xx][Cc][Ll][Uu][Dd][Ee]
dEXISTS             [Ee][Xx][Ii][Ss][Tt][Ss]
dEXIST              [Ee][Xx][Ii][Ss][Tt]
dFIRST              [Ff][Ii][Rr][Ss][Tt]
dFOR                [Ff][Oo][Rr]
dFROM               [Ff][Rr][Oo][Mm]
dIF                 [Ii][Ff]
dIS                 [Ii][Ss]
dINCLUDE            [Ii][Nn][Cc][Ll][Uu][Dd][Ee]
dGET                [Gg][Ee][Tt]
dGLOBAL             [Gg][Ll][Oo][Bb][Aa][Ll]
dJOIN               [Jj][Oo][Ii][Nn]
dLAST               [Ll][Aa][Ss][Tt]
dLEVEL              [Ll][Ee][Vv][Ee][Ll]
dLIKE               [Ll][Ii][Kk][Ee]
dLOCAL              [Ll][Oo][Cc][Aa][Ll]
dLOD                [Ll][Oo][Dd]
dLOOP               [Ll][Oo][Oo][Pp]
dMULTIPLE           [Mm][Uu][Ll][Tt][Ii][Pp][Ll][Ee]
dNAMED              [Nn][Aa][Mm][Ee][Dd]
dNAME               [Nn][Aa][Mm][Ee]
dNEXT               [Nn][Ee][Xx][Tt]
dNULLW              [Nn][Uu][Ll][Ll]
dNOT                [Nn][Oo][Tt]
dNONE               [Nn][Oo][Nn][Ee]
dOBJECT             [Oo][Bb][Jj][Ee][Cc][Tt]
dOF                 [Oo][Ff]
dON                 [Oo][Nn]
dOR                 [Oo][Rr]
dOTHERWISE          [Oo][Tt][Hh][Ee][Rr][Ww][Ii][Ss][Ee]
dQOPERATION         [Oo][Pp][Ee][Rr][Aa][Tt][Ii][Oo][Nn]
dOPERATOR           [Oo][Pp][Ee][Rr][Aa][Tt][Oo][Rr]
dPREVIOUS           [Pp][Rr][Ee][Vv][Ii][Oo][Uu][Ss]
dRETURN             [Rr][Ee][Tt][Uu][Rr][Nn]
dRETURNED           [Rr][Ee][Tt][Uu][Rr][Nn][Ee][Dd]
dRESTRICTING        [Rr][Ee][Ss][Tt][Rr][Ii][Cc][Tt][Ii][Nn][Gg]
dREGISTERED         [Rr][Ee][Gg][Ii][Ss][Tt][Ee][Rr][Ee][Dd]
dROOTONLY           [Rr][Oo][Oo][Tt][Oo][Nn][Ll][Yy]
dROOTONLYMULTIPLE   [Rr][Oo][Oo][Tt][Oo][Nn][Ll][Yy][Mm][Uu][Ll][Tt][Ii][Pp][Ll][Ee]
dROOTONLYCONTINUE   [Rr][Oo][Oo][Tt][Oo][Nn][Ll][Yy][Cc][Oo][Nn][Tt][Ii][Nn][Uu][Ee]
dSET                [Ss][Ee][Tt]
dTSHORT             [Ss][Hh][Oo][Rr][Tt]
dSINGLE             [Ss][Ii][Nn][Gg][Ll][Ee]
dSINGLEFORUPDATE    [Ss][Ii][Nn][Gg][Ll][Ee][Ff][Oo][Rr][Uu][Pp][Dd][Aa][Tt][Ee]
dSYSTEM             [Ss][Yy][Ss][Tt][Ee][Mm]
dSUBTASK            [Ss][Uu][Bb][Tt][Aa][Ss][Kk]
dSHORTCUTKEY        [Ss][Hh][Oo][Rr][Tt][Cc][Uu][Tt][Kk][Ee][Yy]
dTASK               [Tt][Aa][Ss][Kk]
dTDECIMAL           [Dd][Ee][Cc][Ii][Mm][Aa][Ll]
dTINTEGER           [Ii][Nn][Tt][Ee][Gg][Ee][Rr]
dTRANSFORMATION     [Tt][Rr][Aa][Nn][Ss][Ff][Oo][Rr][Mm][Aa][Tt][Ii][Oo][Nn]
dTO                 [Tt][Oo]
dTSTRING            [Ss][Tt][Rr][Ii][Nn][Gg]
dUSING              [Uu][Ss][Ii][Nn][Gg]
dUNTIL              [Uu][Nn][Tt][Ii][Ll]
dTVIEW              [Vv][Ii][Ee][Ww]
dWHERE              [Ww][Hh][Ee][Rr][Ee]
dWHILE              [Ww][Hh][Ii][Ll][Ee]
dWITHIN             [Ww][Ii][Tt][Hh][Ii][Nn]

dLESSTHANEQUAL      [<][=]
dGREATERTHANEQUAL   [>][=]
dLESSTHAN           [<]
dGREATERTHAN        [>]
dNOTEQUAL           [!][=]
dEQUAL              [=]
dMINUS              [-]
dADDITION           [+]
dMULTIPLICATION     [*]
dCOPENCOMMENT       [/][*]
dEOLCOMMENT         [/][/]
dDIVISION           [/]
dLPAREN             [(]
dRPAREN             [)]
dLBRACE             [\[]
dRBRACE             [\]]
dCOMMA              [,]
dCOLON              [:]
dWHITESPACE         [ \t\n\r\v]
dNULL               [\0]

/* translation section */
%%
%{
%}
/* RESERVEDWORD tokens */
{dAFTER}            { return( SetQReturnCodes( qRESERVEDWORD, qAFTER ) ); }
{dEACH}             { return( SetQReturnCodes( qRESERVEDWORD, qEACH ) ); }
{dACTIVATE}         { return( SetQReturnCodes( qRESERVEDWORD, qACTIVATE ) ); }
{dATTRIBUTENAME}    { return( SetQReturnCodes( qRESERVEDWORD, qTATTRIBUTENAME ) ); }
{dAS}               { return( SetQReturnCodes( qRESERVEDWORD, qAS ) ); }
{dAT}               { return( SetQReturnCodes( qRESERVEDWORD, qAT ) ); }
{dANY}              { return( SetQReturnCodes( qRESERVEDWORD, qANY ) ); }
{dATTRIBUTE}        { return( SetQReturnCodes( qRESERVEDWORD, qATTRIBUTE ) ); }
{dAPPLICATION}      { return( SetQReturnCodes( qRESERVEDWORD, qAPPLICATION ) ); }
{dBEFORE}           { return( SetQReturnCodes( qRESERVEDWORD, qBEFORE ) ); }
{dBASED}            { return( SetQReturnCodes( qRESERVEDWORD, qBASED ) ); }
{dCASE}             { return( SetQReturnCodes( qRESERVEDWORD, qCASE ) ); }
{dCONSTRAINT}       { return( SetQReturnCodes( qRESERVEDWORD, qCONSTRAINT ) ); }
{dCONTINUE}         { return( SetQReturnCodes( qRESERVEDWORD, qCONTINUE ) ); }
{dCOMMIT}           { return( SetQReturnCodes( qRESERVEDWORD, qCOMMIT ) ); }
{dCREATE}           { return( SetQReturnCodes( qRESERVEDWORD, qCREATE ) ); }
{dCURSOR}           { return( SetQReturnCodes( qRESERVEDWORD, qCURSOR ) ); }
{dDERIVED}          { return( SetQReturnCodes( qRESERVEDWORD, qDERIVED ) ); }
{dDELETE}           { return( SetQReturnCodes( qRESERVEDWORD, qDELETE ) ); }
{dDOMAIN}           { return( SetQReturnCodes( qRESERVEDWORD, qDOMAIN ) ); }
{dDOES}             { return( SetQReturnCodes( qRESERVEDWORD, qDOES ) ); }
{dDIALOG}           { return( SetQReturnCodes( qRESERVEDWORD, qDIALOG ) ); }
{dELSE}             { return( SetQReturnCodes( qRESERVEDWORD, qELSE ) ); }
{dEMPTY}            { return( SetQReturnCodes( qRESERVEDWORD, qEMPTY ) ); }
{dEND}              { return( SetQReturnCodes( qRESERVEDWORD, qEND ) ); }
{dENTITY}           { return( SetQReturnCodes( qRESERVEDWORD, qENTITY ) ); }
{dENTITYNAME}       { return( SetQReturnCodes( qRESERVEDWORD, qTENTITYNAME ) ); }
{dEXCLUDE}          { return( SetQReturnCodes( qRESERVEDWORD, qEXCLUDE ) ); }
{dEXISTS}           { return( SetQReturnCodes( qRESERVEDWORD, qEXISTS ) ); }
{dEXIST}            { return( SetQReturnCodes( qRESERVEDWORD, qEXIST ) ); }
{dFIRST}            { return( SetQReturnCodes( qRESERVEDWORD, qFIRST ) ); }
{dFOR}              { return( SetQReturnCodes( qRESERVEDWORD, qFOR ) ); }
{dFROM}             { return( SetQReturnCodes( qRESERVEDWORD, qFROM ) ); }
{dGLOBAL}           { return( SetQReturnCodes( qRESERVEDWORD, qGLOBAL ) ); }
{dGET}              { return( SetQReturnCodes( qRESERVEDWORD, qGET ) ); }
{dIF}               { return( SetQReturnCodes( qRESERVEDWORD, qIF ) ); }
{dIS}               { return( SetQReturnCodes( qRESERVEDWORD, qIS ) ); }
{dINCLUDE}          { return( SetQReturnCodes( qRESERVEDWORD, qINCLUDE ) ); }
{dLAST}             { return( SetQReturnCodes( qRESERVEDWORD, qLAST ) ); }
{dLIKE}             { return( SetQReturnCodes( qRESERVEDWORD, qLIKE ) ); }
{dLEVEL}            { return( SetQReturnCodes( qRESERVEDWORD, qLEVEL ) ); }
{dLOCAL}            { return( SetQReturnCodes( qRESERVEDWORD, qLOCAL ) ); }
{dLOD}              { return( SetQReturnCodes( qRESERVEDWORD, qLOD ) ); }
{dLOOP}             { return( SetQReturnCodes( qRESERVEDWORD, qLOOP ) ); }
{dMULTIPLE}         { return( SetQReturnCodes( qRESERVEDWORD, qMULTIPLE ) ); }
{dNEXT}             { return( SetQReturnCodes( qRESERVEDWORD, qNEXT ) ); }
{dNAMED}            { return( SetQReturnCodes( qRESERVEDWORD, qNAMED ) ); }
{dNAME}             { return( SetQReturnCodes( qRESERVEDWORD, qNAME ) ); }
{dNULLW}            { return( SetQReturnCodes( qRESERVEDWORD, qNULL ) ); }
{dNONE}             { return( SetQReturnCodes( qRESERVEDWORD, qNONE ) ); }
{dOBJECT}           { return( SetQReturnCodes( qRESERVEDWORD, qOBJECT ) ); }
{dOF}               { return( SetQReturnCodes( qRESERVEDWORD, qOF ) ); }
{dON}               { return( SetQReturnCodes( qRESERVEDWORD, qON ) ); }
{dQOPERATION}       { return( SetQReturnCodes( qRESERVEDWORD, qOPERATION ) ); }
{dOTHERWISE}        { return( SetQReturnCodes( qRESERVEDWORD, qOTHERWISE ) ); }
{dPREVIOUS}         { return( SetQReturnCodes( qRESERVEDWORD, qPREVIOUS ) ); }
{dRETURN}           { return( SetQReturnCodes( qRESERVEDWORD, qRETURN ) ); }
{dRETURNED}         { return( SetQReturnCodes( qRESERVEDWORD, qRETURNED ) ); }
{dRESTRICTING}      { return( SetQReturnCodes( qRESERVEDWORD, qRESTRICTING ) ); }
{dREGISTERED}       { return( SetQReturnCodes( qRESERVEDWORD, qREGISTERED ) ); }
{dROOTONLY}         { return( SetQReturnCodes( qRESERVEDWORD, qROOTONLY ) ); }
{dROOTONLYMULTIPLE} { return( SetQReturnCodes( qRESERVEDWORD, qROOTONLYMULTIPLE ) ); }
{dROOTONLYCONTINUE} { return( SetQReturnCodes( qRESERVEDWORD, qROOTONLYCONTINUE ) ); }
{dSINGLE}           { return( SetQReturnCodes( qRESERVEDWORD, qSINGLE ) ); }
{dSINGLEFORUPDATE}  { return( SetQReturnCodes( qRESERVEDWORD, qSINGLEFORUPDATE ) ); }
{dSUBTASK}          { return( SetQReturnCodes( qRESERVEDWORD, qSUBTASK ) ); }
{dSHORTCUTKEY}      { return( SetQReturnCodes( qRESERVEDWORD, qSHORTCUTKEY ) ); }
{dSET}              { return( SetQReturnCodes( qRESERVEDWORD, qSET ) ); }
{dSYSTEM}           { return( SetQReturnCodes( qRESERVEDWORD, qSYSTEM ) ); }
{dTASK}             { return( SetQReturnCodes( qRESERVEDWORD, qTASK ) ); }
{dTDECIMAL}         { return( SetQReturnCodes( qRESERVEDWORD, qTDECIMAL ) ); }
{dTINTEGER}         { return( SetQReturnCodes( qRESERVEDWORD, qTINTEGER ) ); }
{dTINTEGER}         { return( SetQReturnCodes( qRESERVEDWORD, qTINTEGER ) ); }
{dTRANSFORMATION}   { return( SetQReturnCodes( qRESERVEDWORD, qTRANSFORMATION ) ); }
{dTSHORT}           { return( SetQReturnCodes( qRESERVEDWORD, qTSHORT ) ); }
{dTSTRING}          { return( SetQReturnCodes( qRESERVEDWORD, qTSTRING ) ); }
{dTCHARACTER}       { return( SetQReturnCodes( qRESERVEDWORD, qTCHARACTER ) ); }
{dTVIEW}            { return( SetQReturnCodes( qRESERVEDWORD, qTVIEW ) ); }
{dTO}               { return( SetQReturnCodes( qRESERVEDWORD, qTO ) ); }
{dUNTIL}            { return( SetQReturnCodes( qRESERVEDWORD, qUNTIL ) ); }
{dUSING}            { return( SetQReturnCodes( qRESERVEDWORD, qUSING ) ); }
{dWITHIN}           { return( SetQReturnCodes( qRESERVEDWORD, qWITHIN ) ); }
{dWHERE}            { return( SetQReturnCodes( qRESERVEDWORD, qWHERE ) ); }
{dWHILE}            { return( SetQReturnCodes( qRESERVEDWORD, qWHILE ) ); }

/* META definitions */

{dNOT}                { return( SetQReturnCodes( qOPERATOR, qNOT ) ); }
{dAND}                { return( SetQReturnCodes( qJOIN, qAND ) ); }
{dOR}                 { return( SetQReturnCodes( qJOIN, qOR ) ); }

{dATTRIBUTEQUALIFIER} { return( SetQReturnCodes( qUSERDEFINED, qATTRIBUTEQUALIFIER ) ); }
{dENTITYQUALIFIER}    { return( SetQReturnCodes( qUSERDEFINED, qENTITYQUALIFIER ) ); }
{dSYM}                { return( SetQReturnCodes( qUSERDEFINED, qVARIABLE ) ); }

{dINTLITERAL}         { return( SetQReturnCodes( qCONSTANT, qTINTEGER ) ); }
{dDECLITERAL}         { return( SetQReturnCodes( qCONSTANT, qTDECIMAL ) ); }
\'([^\'\n])\'         { return( SetQReturnCodes( qCONSTANT, qTCHARACTER ) ); }
\"([^\"\n])*\"        { return( SetQReturnCodes( qCONSTANT, qTSTRING ) ); }

/* OPERATORS */

{dLESSTHANEQUAL}      { return( SetQReturnCodes( qCONDOPERATOR, qLESSTHANEQUAL ) ); }
{dGREATERTHANEQUAL}   { return( SetQReturnCodes( qCONDOPERATOR, qGREATERTHANEQUAL ) ); }
{dLESSTHAN}           { return( SetQReturnCodes( qCONDOPERATOR, qLESSTHAN ) ); }
{dGREATERTHAN}        { return( SetQReturnCodes( qCONDOPERATOR, qGREATERTHAN ) ); }
{dNOTEQUAL}           { return( SetQReturnCodes( qCONDOPERATOR, qNOTEQUAL ) ); }
{dEQUAL}              { return( SetQReturnCodes( qOPERATOR, qEQUAL ) ); }
{dMINUS}              { return( SetQReturnCodes( qOPERATOR, qMINUS ) ); }
{dADDITION}           { return( SetQReturnCodes( qOPERATOR, qADDITION ) ); }
{dMULTIPLICATION}     { return( SetQReturnCodes( qOPERATOR, qMULTIPLICATION ) ); }
{dEOLCOMMENT}         { qqcomment( "\0" ); }
{dCOPENCOMMENT}       { qqcomment( "*/" ); }
{dDIVISION}           { return( SetQReturnCodes( qOPERATOR, qDIVISION ) ); }

/* DELIMITERS */
{dLPAREN}             { return( SetQReturnCodes( qDELIMITER, qLPAREN ) ); }
{dRPAREN}             { return( SetQReturnCodes( qDELIMITER, qRPAREN ) ); }
{dLBRACE}             { return( SetQReturnCodes( qDELIMITER, qLBRACE ) ); }
{dRBRACE}             { return( SetQReturnCodes( qDELIMITER, qRBRACE ) ); }
{dCOMMA}              { return( SetQReturnCodes( qDELIMITER, qCOMMA ) ); }
{dCOLON}              { return( SetQReturnCodes( qDELIMITER, qCOLON ) ); }
{dWHITESPACE}         { ; }
{dNULL}               { lGlobalReadRetCode = GetTextLine( );
                        if ( lGlobalReadRetCode == 0 /* eof */ )
                        {
                           return( 0 );
                        }
                        SaveTextLine( );
                      }
.                     { return( SetQReturnCodes( qUNKNOWNTOKEN, 0 ) ); }

/* function section */
%%

int
SetQReturnCodes( int nTokenClass, int nToken )
{
   zLONG nIDNumber;
   zLONG nDataType;

   sQToken.nLastTokenClass =  sQToken.nTokenClass;
   sQToken.nLastToken = sQToken.nToken;
   sQToken.nLastLineNo = sQToken.nLineNo;
   zstrcpy( sQToken.szLastToken, sQToken.szToken );

   if ( nTokenClass == qUSERDEFINED &&
        nToken == qVARIABLE )
   {
      nIDNumber =
         GetDefineZKey( &nDataType,
                        qq_tbuf );
      if ( nIDNumber > 0 )
      {
         nToken = qDEFINE;
      }
   }

   sQToken.nTokenClass = nTokenClass;
   sQToken.nToken      = nToken;
   sQToken.nLineNo = qqlineno;
   zstrcpy( sQToken.szToken, qq_tbuf );

   switch ( nTokenClass )
   {
      case qRESERVEDWORD:
         return( nToken );
         break;

      case qOPERATOR:
         return( nToken );
         break;

      case qDELIMITER:
         return( nToken );
         break;

      case qUSERDEFINED :
         return( nToken );
         break;

      case qCONSTANT:
         return( nTokenClass );
         break;

      case qCONDOPERATOR:
         return( nTokenClass );
         break;

      case qJOIN:
         return( nTokenClass );
         break;

      case qUNKNOWNTOKEN:
         return( nTokenClass );
         break;

      default :
         return( 0 );
         break;
   }
}

void
qqcomment( zPCHAR szMatchString )
{
   int c;
   zPCHAR cp;

   if ( *szMatchString == 0 )
   {
      c = qqgetc( );
      while ( c != 0 )
         c = qqgetc( );

   }
   for ( cp = szMatchString; *cp != 0; )
   {
      c = qqgetc( );
      if ( c == EOF )
      {
         qqerror( "end of file in comment" );
         break;
      }
      else if ( c == '\0' )
      {
         lGlobalReadRetCode = GetTextLine( );
         if ( lGlobalReadRetCode == 0 /* eof */ )
            return;

         SaveTextLine( );
      }
      else if ( c != *cp++ )
      {
         cp = szMatchString;
         if ( c == *cp )
            cp++;
      }
   }
   return;
}

int
qqwrap( void )
{
   return( 1 );
}

void
qqerror( zPCHAR fmt )
{
   SyntaxError( eQINVALIDSYNTAX, sQToken.szToken );
   return;
}
