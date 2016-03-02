/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp8aa.c - VML SubSystem Where statement build
// DESCRIPTION:   This is the source file of the optimizing code
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/08/18
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9A   1995/08/30
//
// LAST MESSAGE ID: VM00801
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2002.08.29  DGC
   Added an argument to ParseSource().

20.07.1997  DonC
   Corrected handling of pchExprCode and szTextCode so that IF statements
   could handle a larger number of expressions within the IF.

18.08.1998  HH
   Corrected MoveViewToStringExpr, because zsizeof sQExprStruct was changed.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.h"

extern zCHAR  *szMessageListParse[];

/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS: MoveStringExprToView
//             MoveViewToStringExpr
//
/////////////////////////////////////////////////////////////////////////////

// zCHAR  szText[ MAXSTRING ];  // dks 2009.12.28

zOPER_EXPORT zLONG OPERATION
MoveStringExprToView( zVIEW  vReturnExpr,
                      zPCHAR pchExprCode,
                      zPLONG plExprCodeIdx,
                      zPCHAR pchExprText,
                      zPLONG plExprTextIdx )
{
   sQExprStruct sWorkExpr = { 0 };
   zLONG lExprEntry = 0;

   while ( lExprEntry < *plExprCodeIdx )
   {
      zmemcpy( (zPCHAR) &sWorkExpr, (zPCHAR) pchExprCode + lExprEntry, sizeof( sQExprStruct ) );
      lExprEntry += sizeof( sQExprStruct );

      CreateEntity( vReturnExpr, "ExprNode", zPOS_AFTER );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "ElementClass", sWorkExpr.lElementClass );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "DataClass", sWorkExpr.lDataClass );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "DataType", sWorkExpr.lDataType );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "Offset", sWorkExpr.lOffset );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "Level", sWorkExpr.lLevel );
      SetAttributeFromInteger( vReturnExpr, "ExprNode", "Flags", sWorkExpr.lFlags );
      if ( sWorkExpr.lElementClass == qCONSTANT || sWorkExpr.lElementClass == qATTRIBUTEQUALIFIER || (sWorkExpr.lElementClass == qOPERATIONCALL && sWorkExpr.lOffset > 0) )
      {
         SetAttributeFromString( vReturnExpr, "ExprNode", "Text", (zPCHAR) pchExprText + sWorkExpr.lOffset );
      }
      else
      {
         SetAttributeFromString( vReturnExpr, "ExprNode", "Text", "" );
      }
   }

   SetCursorFirstEntity( vReturnExpr, "ExprNode", "" );

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
MoveViewToStringExpr( zVIEW  vSubtask,
                      zVIEW  vSourceView,
                      zPCHAR pchExprCode,
                      zPLONG plExprCodeIdx,
                      zPCHAR pchExprText,
                      zPLONG plExprTextIdx )
{
   sQExprStruct sWorkExpr = { 0 };
   zLONG lExprIncrement = sizeof( sQExprStruct ) - MAXSTRING;
   zLONG lExprEntry;
   zLONG lExprText;

   lExprEntry = 0;
   lExprText = 0;

   if ( CheckExistenceOfEntity( vSourceView, "ExprNode" ) == zCURSOR_SET )
   {
      do
      {
      // sWorkExpr.lStructSize = sizeof( sQExprStruct );
      // GetStructFromEntityAttrs( (zPCHAR) &sWorkExpr, vSourceView, "ExprNode" );
         GetExpressionStruct( &sWorkExpr, vSourceView, "ExprNode" );

                  if ( sWorkExpr.lElementClass == qCONSTANT || sWorkExpr.lElementClass == qATTRIBUTEQUALIFIER )
         {
            sWorkExpr.lOffset = lExprText;
            GetStringFromAttribute( sWorkExpr.szText, zsizeof( sWorkExpr.szText ), vSourceView, "ExprNode", "Text" ); // was szText ... dks 2009.12.28

            strcpy_s( (zPCHAR) pchExprText + sWorkExpr.lOffset, EXPRTEXT_MAXSTRING - sWorkExpr.lOffset, sWorkExpr.szText );
            lExprText += zstrlen( sWorkExpr.szText ) + 1;  // was szText ... dks 2009.12.28
         }
         else
         if ( sWorkExpr.lElementClass == qOPERATIONCALL && sWorkExpr.lOffset <= 1024 ) // a zkey would be larger
         {
            sWorkExpr.lOffset = lExprText;
            GetStringFromAttribute( sWorkExpr.szText, zsizeof( sWorkExpr.szText ), vSourceView, "ExprNode", "Text" ); // was szText ... dks 2009.12.28

            strcpy_s( (zPCHAR) pchExprText + sWorkExpr.lOffset, EXPRTEXT_MAXSTRING - sWorkExpr.lOffset, sWorkExpr.szText );
            lExprText += zstrlen( sWorkExpr.szText ) + 1;  // was szText ... dks 2009.12.28
         }

         zmemcpy( (zPCHAR) pchExprCode + lExprEntry, (zPCHAR) &sWorkExpr, lExprIncrement );
         lExprEntry += lExprIncrement;

         // Make sure that the size of the expression is not too big for the pchExprCode or pchExprText work areas.  check resr length > twice the size
         // of the increment of pchExprCode during each loop pass.  80 is a guess of max size of the next expression text.
         if ( lExprEntry > EXPRCODE_MAXSTRING - (2 * lExprIncrement) || lExprText  > EXPRTEXT_MAXSTRING - 80 )
         {
            SyntaxError( vSubtask, eQTOOMANYEXPRSINIFSTATEMENT, "" );
            return( -1 );
         }

      } while ( SetCursorNextEntity( vSourceView, "ExprNode", "" ) >= zCURSOR_SET );
   }

   *plExprCodeIdx = lExprEntry;
   *plExprTextIdx = lExprText;

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ParseSourceFromEditor( zVIEW  vSubtask,
                       zVIEW  vSourceMeta,
                       zLONG  lIlParseAction,
                       zLONG  lIlParseLimit,
                       zLONG  lIlParseSource,
                       zPCHAR szInSourceFileName,
                       zPCHAR szInOperationName,
                       zCPCHAR cpcGenLang )
{
   zSHORT  nRC;
   LPLIBRARY hLib;
   FARPROC lf;

   hLib = SysLoadLibrary( vSubtask, "TZVSPMAA" );
   if ( hLib == 0 )
   {
      return( -1 );
   }

   lf = SysGetProc( hLib, "ParseSource" );
   if ( lf == 0 )
      return( -1 );

   nRC = (zSHORT)(*lf)( (zVIEW) vSubtask, (zVIEW) vSourceMeta, (zLONG) lIlParseAction, (zLONG) lIlParseLimit, (zLONG) lIlParseSource,
                        (zPCHAR) szInSourceFileName, (zPCHAR) szInOperationName, 0, 0, cpcGenLang );
   return( nRC );
}
