/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzzorpts.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"



zOPER_EXPORT zSHORT OPERATION
oTZZOLODO_SummaryReport( zVIEW  vLOD_In,
                         zPCHAR szReportFileName )
{
   zVIEW    vLOD;
   zCHAR    Line[ 512 ];
   zCHAR    szData[ 512 ];
   zLONG    lRptFile;
   zLONG    lShowZKey;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 61 ];
   zCHAR    szTempString_1[ 1026 ];
   zCHAR    szTempString_2[ 1026 ];
   zCHAR    szTempString_3[ 1026 ];
   zCHAR    szTempString_4[ 1026 ];
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zCHAR    szTempString_5[ 33 ];
   zLONG    lTempInteger_1;
   zCHAR    szTempString_6[ 33 ];
   zCHAR    szTempString_7[ 33 ];
   zCHAR    szTempString_8[ 33 ];

   if ( ZeidonStringCompare ( szReportFileName, 1, 0, "", 1, 0, 128 ) == 0 )
   {
      return( -16 );
   }
   else
   {
      lRptFile = SysOpenFile( vLOD_In, szReportFileName, 64 );
      if ( lRptFile < 0 )
      {
         return( -1 );
      }
   }

   lShowZKey = 0;
   nZRetCode = CreateViewFromViewForTask( &vLOD, vLOD_In, 0 );
   ZeidonStringCopy( Line, 1, 0, " LOD: ", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD", "Name" );
   zstrblank( szData, 15, 1 );
   ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "ZKey: ", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD", "ZKey" );
   zstrblank( szData, 14, 1 );
   ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Desc: ", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vLOD, "LOD", "Desc" );
   ZeidonStringConcat( Line, 1, 0, szTempString_0, 1, 0, 511 );
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
   ZeidonStringCopy( Line, 1, 0, " DLL: ", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD", "DLL_Name" );
   zstrblank( szData, 15, 1 );
   ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "Rules: Activate=", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vLOD, "LOD", "ActivateConstraint" );
   ZeidonStringConcat( Line, 1, 0, szTempString_1, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "  ActivateEmpty=", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vLOD, "LOD", "ActivateEmptyConstraint" );
   ZeidonStringConcat( Line, 1, 0, szTempString_2, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "  Commit=", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), vLOD, "LOD", "CommitConstraint" );
   ZeidonStringConcat( Line, 1, 0, szTempString_3, 1, 0, 511 );
   ZeidonStringConcat( Line, 1, 0, "  DropOI=", 1, 0, 511 );
   nZRetCode = GetStringFromAttribute( szTempString_4, zsizeof( szTempString_4 ), vLOD, "LOD", "DropOIConstraint" );
   ZeidonStringConcat( Line, 1, 0, szTempString_4, 1, 0, 511 );
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, "" );
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, "------------- Hierarchy ----------------" );
   RESULT = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = GetStringFromAttribute( Line, zsizeof( Line ), vLOD, "LOD_Entity", "IndentName" );
      nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
      RESULT = SetCursorNextEntity ( vLOD, "LOD_Entity", "" );
   }
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, "" );
   nZRetCode = SysWriteLine( vLOD_In,  lRptFile, "-------------- Detail ------------------" );
   RESULT = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = SysWriteLine( vLOD_In,  lRptFile, "" );
      nZRetCode = GetStringFromAttribute( Line, zsizeof( Line ), vLOD, "LOD_Entity", "Name" );
      zstrblank( Line, 36, 1 );
      lTempInteger_0 = CheckExistenceOfEntity ( vLOD, "ER_Entity" );
      if ( lTempInteger_0 == 0 )
      {
         ZeidonStringConcat( Line, 1, 0, "ER=", 1, 0, 511 );
         if ( CompareAttributeToAttribute ( vLOD, "LOD_Entity", "Name", vLOD, "ER_Entity", "Name" ) == 0 )
         {
            ZeidonStringConcat( Line, 1, 0, "same", 1, 0, 511 );
         }
         else
         {
            nZRetCode = GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), vLOD, "ER_Entity", "Name" );
            ZeidonStringConcat( Line, 1, 0, szTempString_5, 1, 0, 511 );
         }
         ZeidonStringConcat( Line, 1, 0, "[", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "ER_Entity", "ZKey" );
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         ZeidonStringConcat( Line, 1, 0, "]", 1, 0, 511 );
      }

      lTempInteger_1 = CheckExistenceOfEntity ( vLOD, "ER_RelLink" );
      if ( lTempInteger_1 == 0 )
      {
         ZeidonStringConcat( Line, 1, 0, " Rel=[", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "ER_RelLink", "ZKey" );
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         ZeidonStringConcat( Line, 1, 0, "]", 1, 0, 511 );
      }
      nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
      ZeidonStringCopy( Line, 1, 0, "       CR", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, "  DEL", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " UPD", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " INC", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " Src", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " EXC", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " HID", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " Recursv", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " ACr", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " Deriv", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " SeqTyp", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " Work", 1, 0, 511 );
      ZeidonStringConcat( Line, 1, 0, " ParDel", 1, 0, 511 );
      nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
      ZeidonStringCopy( Line, 1, 0, "        ", 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Create" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Delete" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Update" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Include" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "InclSrc" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Exclude" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Hidden" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Recursive" );
      zstrblank( szData, 9, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "AutoCreate" );
      zstrblank( szData, 5, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Derived" );
      zstrblank( szData, 7, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "SeqType" );
      zstrblank( szData, 8, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "Work" );
      zstrblank( szData, 6, 1 );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "LOD_Entity", "ParentDeleteBehave" );
      ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
      nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
      RESULT = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         if ( CompareAttributeToString( vLOD, "LOD_Attribute", "Work", "" ) == 0 || CompareAttributeToString ( vLOD, "LOD_Attribute", "Work", "N" ) == 0 )
         {
            nZRetCode = GetStringFromAttribute( szTempString_6, zsizeof( szTempString_6 ), vLOD, "ER_Attribute", "Name" );
            ZeidonStringCopy( Line, 1, 0, "...", 1, 0, 511 );
            ZeidonStringConcat( Line, 1, 0, szTempString_6, 1, 0, 511 );
         }
         else
         {
            nZRetCode = GetStringFromAttribute( szTempString_7, zsizeof( szTempString_7 ), vLOD, "ER_Attribute", "Name" );
            ZeidonStringCopy( Line, 1, 0, ".w.", 1, 0, 511 );
            ZeidonStringConcat( Line, 1, 0, szTempString_7, 1, 0, 511 );
         }

         zstrblank( Line, 34, 1 );
         if ( lShowZKey == 1 )
         {
            ZeidonStringConcat( Line, 1, 0, "[", 1, 0, 511 );
            nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "ER_Attribute", "ZKey" );
            ZeidonStringConcat( szData, 1, 0, "]", 1, 0, 511 );
            zstrblank( szData, 11, 1 );
         }
         else
         {
            if ( CompareAttributeToString( vLOD, "LOD_Attribute", "ParticipatesInKey", "Y" ) == 0 )
            {
               ZeidonStringCopy( szData, 1, 0, "InKey ", 1, 0, 511 );
            }
            else
            {
               ZeidonStringCopy( szData, 1, 0, "      ", 1, 0, 511 );
            }
            if ( CompareAttributeToString( vLOD, "ER_Attribute", "NotNull", "Y" ) == 0 )
            {
               ZeidonStringConcat( szData, 1, 0, "Req ", 1, 0, 511 );
            }
            else
            {
               ZeidonStringConcat( szData, 1, 0, "    ", 1, 0, 511 );
            }
         }
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         ZeidonStringConcat( Line, 1, 0, "Type ", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "Domain", "DerivedDataType" );
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         ZeidonStringConcat( Line, 1, 0, "  Lth=", 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szData, zsizeof( szData ), vLOD, "ER_Attribute", "Lth" );
         zstrblank( szData, 5, 1 );
         ZeidonStringConcat( Line, 1, 0, szData, 1, 0, 511 );
         nZRetCode = GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), vLOD, "Domain", "DerivedName" );
         ZeidonStringConcat( Line, 1, 0, szTempString_8, 1, 0, 511 );
         nZRetCode = SysWriteLine( vLOD_In,  lRptFile, Line );
         RESULT = SetCursorNextEntity ( vLOD, "LOD_Attribute", "" );
      }

      RESULT = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
   }

   nZRetCode = SysCloseFile( vLOD_In, lRptFile, 0 );
   return( 0 );
}

