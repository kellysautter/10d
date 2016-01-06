/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmutil.c - Zeidon application operations
// DESCRIPTION:   This C file contains utility operations for the Zeidon
//                application.
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
// LAST MESSAGE ID: CM00703
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
/*
CHANGE LOG

2001.03.01  DKS Z2000
   Cleanup of metas with negative task ids now works.

2000.11.01  SR  Z2000  Size of path variables
   Modify size of path variable in function fnBuildMetas because size of
   ExecDir, MetaSrcDir und PgmSrcDir are changed to 254 in datamodel. These
   paths are used in fnBuildMetas.

29.05.1998    RG!!
   Bugfixes erhalten am 29.05.1998 von DonC, Doug ohne Aenderungsvermerk
   sollte nachgeliefert werden

1998/09/18    DC
   Modified fnCheckForDuplicateName to eliminate the prompt for a new meta
   when a duplicate meta name is encountered. If a user should encounter this
   situation, a bug must exist elsewhere and we do not want to save the meta.

1998/11/03    DC
   Modified fnCheckForDuplicateName to not check for duplicate name on delete
   for Domains and Global Operations.

1999/04/07    DC
   Modified fnGetAndSetZKey to use zIGNORE_ERRORS on ActivateFromFile.
   Also modified fnBuildMetas to eliminate conversion of meta names to
   upper case.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzcm_opr.hg"
#include "ZeidonOp.H"


//./ ADD NAME=zgSortEntityWithinParent
// Source Module=tzcmutil.c
/////////////////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: zgSortEntityWithinParent
//
//  PURPOSE:    This routine will sort an Entity (keeping its dependents)
//              by any given 2 attributes. (if the 2nd attr is a null string,
//              the only the first attr is used)
//
//              It is not efficient?, but it tends to work.
//
//  PARAMETERS: bAscDsc  -- Indicator for ascending or descending
//                          (zASCENDING or zDESCENDING)
//              vIn      -- view that contains the entity to be sorted AND
//                          its' parent
//              szEntityName  -- Entity to be sorted
//              szAttribName  -- Name of the attribute for primary sort
//              szAttribName2 -- Name of a second sort attribute or
//                               null string
//
//  RETURNS:        number of entity swaps required to sort
//             -1 - more than 32000 swaps required
//
/////////////////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
zgSortEntityWithinParent( zSHORT bAscDsc,
                          zVIEW  vIn,
                          zPCHAR szEntityName,
                          zPCHAR szAttribName,
                          zPCHAR szAttribName2 )
{
   zCHAR  sz[ 72 ];

   strcpy_s( sz, sizeof( sz ), szAttribName );
   if ( bAscDsc == zDESCENDING )
   {
      strcat_s( sz, sizeof( sz ), " D " );
   }
   else
   {
      strcat_s( sz, sizeof( sz ), " A " );
   }

   if ( szAttribName2 && szAttribName2[ 0 ] )
   {
      strcat_s( sz, sizeof( sz ), szAttribName2 );
      if ( bAscDsc == zDESCENDING )
      {
         strcat_s( sz, sizeof( sz ), " D" );
      }
      else
      {
         strcat_s( sz, sizeof( sz ), " A" );
      }
   }

   return( OrderEntityForView( vIn, szEntityName, sz ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    TruncateName8
//
// PURPOSE:  Force a name to <= 8 characters
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TruncateName8( zPCHAR szName )
{
   zSHORT k;

   for ( k = 0; k < 8; k++ )
   {
      if ( szName[ k ] == 0 )
         break;

      if ( szName[ k ] == ' ' )
         szName[ k ] = '_';
   }

   szName[ k ] = 0;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnVerifyType( zSHORT nType )
{
   if ( (nType < 0) ||
        ((nType > zSOURCE_MAX_META) && (nType < 1000)) ||
        ((nType >= 1000) && (nType < 2000)) ||
        (nType > zREFER_MAX_META) )
   {
      MessageSend( 0, "CM00701", "Configuration Management",
                   "Invalid Zeidon Type passed to CM Operation",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   if ( nType <= zSOURCE_MAX_META )
      return( 0 );
   else
      return( 2 );
}

int
fnBuildMetas( zVIEW vSubtask, zVIEW vLPLR,
              zPCHAR pchDirectorySpec, int nEntityType )
{
   zVIEW vOI_View;
   zLONG lType;
   zLONG lZKey;
   zLONG lFlags;
   zCHAR szFullName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szMetaName[ zBASE_FILENAME_LTH + 1 ];
   zCHAR szTimestamp[ 33 ];
   HFILE hFile;
   int   nRC;

#ifdef __WIN32__
   WIN32_FIND_DATA fd;
   HANDLE sh;

   // Searches a directory for a file whose name matches the specified
   // file name on the destination site identified by this object.  It
   // examines subdirectory names as well as file names.
   //
   // fd - Address of a WIN32_FIND_DATA structure that receives
   // information about the file or subdirectory that has been found.
   //
   // sh - Variable that receives a handle that can be used for
   // subsequent calls to the FindNextFile and FindClose functions.

   sh = FindFirstFile( pchDirectorySpec, &fd );
   if ( sh != INVALID_HANDLE_VALUE )
   {
      do
      {
         TraceLineS( "fnBuildMetas ", (zCPCHAR) fd.cFileName );
         strcpy_s( szMetaName, sizeof( szMetaName ), (zCPCHAR) fd.cFileName );
         strcpy_s( szFullName, sizeof( szFullName ), pchDirectorySpec );
         szFullName[ zstrlen( szFullName ) - 5 ] = 0;
         strcat_s( szFullName, sizeof( szFullName ), szMetaName );
         hFile = (HFILE) SysOpenFile( vLPLR, szFullName, COREFILE_READ );
         SysGetFileDateTime( hFile, szTimestamp, sizeof( szTimestamp ) );
         SysCloseFile( vLPLR, hFile, 0 );
         szMetaName[ zstrlen( szMetaName ) - 4 ] = 0;
      // AnsiUpper( szMetaName );
         GetIntegerFromAttribute( &lType, vLPLR, "W_MetaType", "Type" );
         if ( lType == zREFER_VOR_META )
         {
            zxatol( szMetaName, &lZKey );
            nRC = SetCursorFirstEntityByInteger( vLPLR, "W_MetaDef",
                                                 "CPLR_ZKey", lZKey, "" );
         }
         else
         {
            nRC = SetCursorFirstEntityByString( vLPLR, "W_MetaDef",
                                                "Name", szMetaName, "" );
         }

         if ( nRC != zCURSOR_SET ) // New Meta
         {
            // Get the Name from the root entity of the object, because it is
            // both upper and lower case there.
            // Note that the name attribute is actually "Tag" for a Dialog
            // and that we will keep the computed name for a named view.
            lFlags = zSINGLE | zACTIVATE_ROOTONLY | zIGNORE_ERRORS;
            ActivateOI_FromFile( &vOI_View, SRC_CMOD[ lType - 2000 ].szOD,
                                 vLPLR, szFullName, lFlags );
            if ( lType == zREFER_DIALOG_META || lType == zREFER_REPORT_META || lType == zSOURCE_XSLT_META )
            {
               GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vOI_View,
                                       SRC_CMOD[ lType - 2000 ].szOD_ROOT, "Tag" );
            }
            else
            {
               if ( lType != zREFER_VOR_META )
                  GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vOI_View,
                                          SRC_CMOD[ lType - 2000 ].szOD_ROOT, "Name" );
            }

            // We will do the compare again here, in case the set cursor
            // above didn't find a match because of upper/lower case differences.
            nRC = SetCursorFirstEntityByString( vLPLR, "W_MetaDef", "Name", szMetaName, "" );
            if ( nRC < zCURSOR_SET )
            {
               CreateEntity( vLPLR, "W_MetaDef", zPOS_AFTER );
               SetAttributeFromString( vLPLR, "W_MetaDef", "LastUpdateDate", szTimestamp );
               SetAttributeFromString( vLPLR, "W_MetaDef", "LastSyncDate", szTimestamp );
               if ( lType != zREFER_VOR_META )
                  SetAttributeFromString( vLPLR, "W_MetaDef", "Name", szMetaName );

               fnGetAndSetZKey( vSubtask, vLPLR, szFullName );

            if ( lType == zREFER_LOD_META )
               SetAttributeFromAttribute( vLPLR, "W_MetaDef", "DoNotMergeFlag", vOI_View, "LOD", "DoNotMergeFlag" );
            }

         DropObjectInstance( vOI_View );
         }

         // If the following lines are changed, then they need to be changed in
         // fnGetAndSetZKey as well.
         SetAttributeFromInteger( vLPLR, "W_MetaDef", "Status", 1 );
         SetAttributeFromInteger( vLPLR, "W_MetaDef", "UpdateInd", 2 );

      } while ( FindNextFile( sh, &fd ) );

      FindClose( sh );
   }

   return( 0 );

#else
   HWND  hWndList;
   int   nCount, i;
   HWND  hWnd;
   HINSTANCE hInst;

   hWnd = GetForegroundWindow( );
   hInst = (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE );

   hWndList = CreateWindow( "ListBox", NULL, WS_CHILD | LBS_SORT, 1, 1, 1, 1,
                            hWnd, (HMENU) 101, (HANDLE) hInst, NULL );

   SendMessage( hWndList, WM_SETREDRAW, FALSE, 0L );
   SendMessage( hWndList, LB_DIR,
                0x0000, (LONG)(LPSTR) pchDirectorySpec );
   nCount = (int) SendMessage( hWndList, LB_GETCOUNT, 0, 0L );

   for ( i = 0; i < nCount; i++ )
   {
      SendMessage( hWndList, LB_GETTEXT, i, (LONG)(LPSTR) szMetaName );
      strcpy_s( szFullName, sizeof( szFullName ), pchDirectorySpec );
      szFullName[ zstrlen( szFullName ) - 5 ] = 0;
      strcat_s( szFullName, sizeof( szFullName ), szMetaName );
      hFile = (HFILE) SysOpenFile( szFullName, COREFILE_READ );
      SysGetFileDateTime( hFile, szTimestamp, sizeof( szTimestamp ) );
      SysCloseFile( hFile, 0 );
      szMetaName[ zstrlen( szMetaName ) - 4 ] = 0;
      AnsiUpper( szMetaName );
      GetIntegerFromAttribute( &lType, vLPLR, "W_MetaType", "Type" );
      if ( lType == zREFER_VOR_META )
      {
         zxatol( szMetaName, &lZKey );
         nRC = SetCursorFirstEntityByInteger( vLPLR, "W_MetaDef",
                                              "CPLR_ZKey", lZKey, "" );
      }
      else
      {
         nRC = SetCursorFirstEntityByString( vLPLR, "W_MetaDef",
                                             "Name", szMetaName, "" );
      }

      if ( nRC != zCURSOR_SET ) // New Meta
      {
         CreateEntity( vLPLR, "W_MetaDef", zPOS_AFTER );
         SetAttributeFromString( vLPLR, "W_MetaDef",
                                 "LastUpdateDate", szTimestamp );
         SetAttributeFromString( vLPLR, "W_MetaDef",
                                 "LastSyncDate", szTimestamp );
         if ( lType != zREFER_VOR_META )
            SetAttributeFromString( vLPLR, "W_MetaDef", "Name", szMetaName );
      }

      fnGetAndSetZKey( vSubtask, vLPLR, szFullName );

      // If the following lines are changed, then they need to be changed in
      // fnGetAndSetZKey as well.
      SetAttributeFromInteger( vLPLR, "W_MetaDef", "Status", 1 );
      SetAttributeFromInteger( vLPLR, "W_MetaDef", "UpdateInd", 2 );

   } // for ( i = 0; i < nCount; i++ )...

   DestroyWindow( hWndList );
#endif
   zgSortEntityWithinParent( zASCENDING, vLPLR, "W_MetaDef", "Name", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
fnGetAndSetZKey( zVIEW vSubtask, zVIEW vLPLR, zPCHAR szFileName )
{
   zVIEW  vOI_View;
   zVIEW  vTaskLPLR;
   zCHAR  szTaskLPLR_Name[ 33 ];
   zCHAR  szLPLR_Name[ 33 ];
   zCHAR  szMetaName[ 33 ];
   zLONG  lType;
   zLONG  lFlags;
   zLONG  lUniqueFlag;
   zULONG ulZKey;
   zCHAR  szERR_Msg[ 253 ];
   zSHORT nRC;

   GetIntegerFromAttribute( &lType, vLPLR, "W_MetaType", "Type" );

   if ( lType == zREFER_POD_META )
      return;

   if ( lType == zREFER_VOR_META ||
        lType == zREFER_DOMAINGRP_META ||
        lType == zREFER_GOPGRP_META )
   {
      lFlags = zSINGLE | zIGNORE_ERRORS;
   }
   else
      lFlags = zSINGLE | zACTIVATE_ROOTONLY | zIGNORE_ERRORS;

   if ( ActivateOI_FromFile( &vOI_View, SRC_CMOD[ lType - 2000 ].szOD,
                             vLPLR, szFileName, lFlags ) != 0 )
   {
      SetNameForView( vOI_View, "MetaOI", vSubtask, zLEVEL_TASK );
      DeleteEntity( vLPLR, "W_MetaDef", zREPOS_PREV );
      strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Unable to activate file: " );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), szFileName );
      TraceLineS( "Zeidon Configuration Management", szERR_Msg );
      return;
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vOI_View,
                            SRC_CMOD[ lType -2000 ].szOD_ROOT, "ZKey" );
   if ( ulZKey == 0 )
   {
      strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Root entity, " );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), SRC_CMOD[ lType -2000 ].szOD_ROOT );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), ", ZKey is 0 in File: " );
      strcat_s( szERR_Msg, sizeof( szERR_Msg ), szFileName );
      TraceLineS( "Zeidon Configuration Management", szERR_Msg );
      return;
   }

   GetStringFromAttribute( szMetaName, sizeof( szMetaName ), vLPLR, "W_MetaDef", "Name" );

   // The following code was added by DonC on 10/30/2015 to make sure that a meta object has a unique
   // ZKey for the root entity. If it isn't unique, we will go through a loop, incrementing the key until
   // it is unique. We will then save the meta file with the modified ZKey value and notify the operator
   // that the object has been updated.
   nRC = SetCursorFirstEntityByInteger( vLPLR, "W_MetaDef", "CPLR_ZKey", ulZKey, "" );
   if ( nRC >= zCURSOR_SET )
   {
      // There is a duplicate ZKey, so loop incrementing root ZKey by 1 until it is unique. Then update the ZKey in the object and commit it.
      lUniqueFlag = 0;
      while (lUniqueFlag == 0 )
      {
         ulZKey++;
         nRC = SetCursorFirstEntityByInteger( vLPLR, "W_MetaDef", "CPLR_ZKey", ulZKey, "" );
         if ( nRC < zCURSOR_SET )
         {
            // The new ZKey is unique. Unless the object is a Domain Group or an Operation Group, reactivate the meta object, because the
            // initial activate was root only.
            if ( lType != zREFER_DOMAINGRP_META && lType != zREFER_GOPGRP_META )
            {
               DropObjectInstance( vOI_View );
               lFlags = zSINGLE | zIGNORE_ERRORS;
               ActivateOI_FromFile( &vOI_View, SRC_CMOD[ lType - 2000 ].szOD, vLPLR, szFileName, lFlags );
               SetNameForView( vOI_View, "MetaOI", vSubtask, zLEVEL_TASK );
            }
            SetAttributeFromInteger( vOI_View, SRC_CMOD[ lType -2000 ].szOD_ROOT, "ZKey", ulZKey );
            CommitOI_ToFile( vOI_View, szFileName, zSINGLE );
            strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Note that the following meta was updated because of a ZKey change: " );
            strcat_s( szERR_Msg, sizeof( szERR_Msg ), szMetaName );
            strcat_s( szERR_Msg, sizeof( szERR_Msg ), SRC_CMOD[ lType -2000 ].szOD_EXT );
            MessageSend( vSubtask, "", "Configuration Management", szERR_Msg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            lUniqueFlag = 1;
         }
      }
   }

   SetAttributeFromInteger( vLPLR, "W_MetaDef", "CPLR_ZKey", ulZKey );

   if ( lType == zREFER_VOR_META )
   {
      SetAttributeFromAttribute( vLPLR, "W_MetaDef", "Name", vOI_View, SRC_CMOD[ lType - 2000 ].szOD_ROOT, SRC_CMOD[ lType - 2000 ].szOD_NAME );
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szTaskLPLR_Name, sizeof( szTaskLPLR_Name ), vTaskLPLR, "LPLR", "Name" );
      GetStringFromAttribute( szLPLR_Name, sizeof( szLPLR_Name ), vLPLR, "LPLR", "Name" );
      if ( zstrcmp( szLPLR_Name, szTaskLPLR_Name ) != 0 )
      {
         SetNameForView( vTaskLPLR, "TZCMhold", vSubtask, zLEVEL_TASK );
         SetNameForView( vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      }

      if ( GetViewByName( &vTaskLPLR, "TZCMhold", vSubtask, zLEVEL_TASK ) > 0 )
      {
         DropNameForView( vTaskLPLR, "TZCMhold", vSubtask, zLEVEL_TASK );
         SetNameForView( vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      }
   }

   // For group types we need to copy the sub-metas to their respective
   // W_MetaDef.
   if ( lType == zREFER_DOMAINGRP_META || lType == zREFER_GOPGRP_META )
   {
      zSHORT nTargetType;
      zVIEW  vTarget;
      zLONG  lTemp;
      zCHAR  szSubEntityName[ 33 ];

      CreateViewFromViewForTask( &vTarget, vLPLR, 0 );
      SetNameForView( vTarget, "vTargetSub", vSubtask, zLEVEL_TASK );

      #ifdef DEBUG
         SetNameForView( vTarget, "TempTargetView", vSubtask, zLEVEL_TASK );
         SetNameForView( vOI_View, "OI_View", vSubtask, zLEVEL_TASK );
      #endif

      // DGC - DOMGRP
      // Set the W_MetaType cursor to reflect the current target type.
      if ( lType == zREFER_DOMAINGRP_META )
         nTargetType = zREFER_DOMAIN_META;
      else
         nTargetType = zREFER_GO_META;

      SetCursorFirstEntityByInteger( vTarget, "W_MetaType", "Type", (zLONG) nTargetType, "" );

      // For each of the sub-metas (eg "Domains") in the meta we just activated (e.g. "DomainGroup") copy the sub-meta to the W_MetaDef
      // entity in the LPLR.
      for ( nRC = SetCursorFirstEntity( vOI_View, REFER_CMOD[ nTargetType - 2000 ].szOD_ROOT, "" );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vOI_View, REFER_CMOD[ nTargetType - 2000 ].szOD_ROOT, "" ) )
      {
         GetIntegerFromAttribute( &lTemp, vOI_View, REFER_CMOD[ nTargetType - 2000 ].szOD_ROOT, "ZKey" );

         // The following code was added by DonC on 10/30/2015 to make sure that a Domain or Operation meta object has a unique ZKey for each
         // Domain or Operation entity. This is very similar to the ZKey logic above.
         nRC = SetCursorFirstEntityByInteger( vTarget, "W_MetaDef", "CPLR_ZKey", lTemp, "" );
         if ( nRC >= zCURSOR_SET )
         {
            // There is a duplicate ZKey, so loop incrementing ZKey by 1 until it is unique. Then update the ZKey in the object and commit it.
            lUniqueFlag = 0;
            while (lUniqueFlag == 0 )
            {
               lTemp++;
               nRC = SetCursorFirstEntityByInteger( vTarget, "W_MetaDef", "CPLR_ZKey", lTemp, "" );
               if ( nRC < zCURSOR_SET )
               {
                  // The new ZKey is unique, so set it in either Domain or Operation entity.
                  if ( lType == zREFER_DOMAINGRP_META )
                     strcpy_s( szSubEntityName, sizeof( szSubEntityName ), "Domain" );
                  else
                     strcpy_s( szSubEntityName, sizeof( szSubEntityName ), "Operation" );

                  SetAttributeFromInteger( vOI_View, szSubEntityName, "ZKey", lTemp );
                  CommitOI_ToFile( vOI_View, szFileName, zSINGLE );
                  strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Note that the following meta was updated because of a ZKey change: " );
                  strcat_s( szERR_Msg, sizeof( szERR_Msg ), szMetaName );
                  strcat_s( szERR_Msg, sizeof( szERR_Msg ), SRC_CMOD[ lType -2000 ].szOD_EXT );

                  MessageSend( vSubtask, "", "Configuration Management", szERR_Msg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
                  lUniqueFlag = 1;
               }
            }
         }

         // Copy Name, ZKey, and GroupName from vOI_View to vTarget (the LPLR view).
         CreateEntity( vTarget, "W_MetaDef", zREPOS_AFTER );
         SetAttributeFromAttribute( vTarget, "W_MetaDef", "Name", vOI_View, REFER_CMOD[ nTargetType - 2000 ].szOD_ROOT, REFER_CMOD[ nTargetType - 2000 ].szOD_NAME );
         SetAttributeFromAttribute( vTarget, "W_MetaDef", "CPLR_ZKey", vOI_View, REFER_CMOD[ nTargetType - 2000 ].szOD_ROOT, "ZKey" );
         SetAttributeFromAttribute( vTarget, "W_MetaDef", "GroupName", vOI_View, REFER_CMOD[ lType - 2000 ].szOD_ROOT, REFER_CMOD[ lType - 2000 ].szOD_NAME );

         // If the following values need to be changed, they also need to be
         // changed where fnGetAndSetZKey is called.
         SetAttributeFromInteger( vTarget, "W_MetaDef", "Status", 1 );
         SetAttributeFromInteger( vTarget, "W_MetaDef", "UpdateInd", 2 );
      }

      DropView( vTarget );

   } // if ( lType == zREFER_DOMAINGRP_META || lType == zREFER_GOPGRP_META )...

   SetAttributeFromAttribute( vLPLR, "W_MetaDef", "Desc", vOI_View, SRC_CMOD[ lType - 2000 ].szOD_ROOT, "Desc" );
   DropObjectInstance( vOI_View );

} // fnGetAndSetZKey

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void
fnGetTaskOI_ListName( zVIEW vSubtask, zPCHAR pchTaskOI_ListName, zLONG lMaxLth )
{
   zCHAR szTaskID[ 18 ];
   zLONG lTaskID;

   lTaskID = SysGetTaskFromView( vSubtask );
   sprintf_s( szTaskID, sizeof( szTaskID ), "%08x", lTaskID );
   strcpy_s( pchTaskOI_ListName, lMaxLth, "__CM." );  // moved all references in tzcmoprs up to 80 characters
   strcpy_s( pchTaskOI_ListName + 5, lMaxLth - 5, szTaskID );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnCheckCM_ForActiveMetaByTask( zVIEW vSubtask, zLONG MOI_InstanceID )
{
   zVIEW  vWork;
   zVIEW  vZeidonCM;
   zVIEW  vActiveMetas;
   zSHORT nOI_Found;
   zCHAR  szViewName[ 80 ];
   zLONG  lInstanceID;
   zLONG  lTaskID;
   zSHORT nRC;

   GetViewByName( &vWork, "TaskLPLR", vSubtask, zLEVEL_TASK );
   nOI_Found = 0;
// lTaskID = SysGetTaskFromView( vSubtask );
   lTaskID = SysGetTaskFromView( vSubtask );

   // Get the view to ZedionCM OI.
   GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );

   // Get the view to the ActiveMeta OI.
   GetViewByName( &vActiveMetas, "OpenCM_Metas", vZeidonCM, zLEVEL_SUBTASK );
   nRC = SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaDef", "TaskID", lTaskID, "LPLR" );
   while ( (nRC == zCURSOR_SET) || (nRC == zCURSOR_SET_NEWPARENT) )
   {
      GetStringFromAttribute( szViewName, sizeof( szViewName ), vActiveMetas, "W_MetaDef", "CM_ViewName" );
      strcat_s( szViewName, sizeof( szViewName ), ".u" );
      if ( GetViewByName( &vWork, szViewName, vZeidonCM, zLEVEL_SUBTASK ) < 0 )
      {
         nRC = SetCursorNextEntityByInteger( vActiveMetas, "W_MetaDef", "TaskID", lTaskID, "LPLR" );
      }
      else
      {
         lInstanceID = MiGetInstanceID_ForView( vWork );
         if ( lInstanceID != MOI_InstanceID )
         {
            nRC = SetCursorNextEntityByInteger( vActiveMetas, "W_MetaDef", "TaskID", lTaskID, "LPLR" );
         }
         else
         {
            nRC = zCURSOR_NULL;
            nOI_Found = 1;
         }
      }
   }

   return( nOI_Found );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnCheckForDuplicateName( zVIEW MOI_View, zVIEW vLPLR,
                         zPCHAR szMetaOI_Name, zLONG lMetaOI_ZKey,
                         zSHORT nNewInd )
{
   zSHORT nDupFound = 0;
   zCHAR  szNewName[ 33 ];
   zCHAR  szMsg[ 256 ];
   zLONG  lType;
   zVIEW  vTempLPLR;

   if ( nNewInd == 1 )
   {
      // Adding New Meta
      if ( SetCursorFirstEntityByString( vLPLR, "W_MetaDef", "Name", szMetaOI_Name, "" ) == zCURSOR_SET )
      {
         // Duplicate Named Meta exists
         GetVariableFromAttribute( &szNewName, 0, zTYPE_STRING, 125, vLPLR, "W_MetaType", "Type", "CM_Type", 0 );
         nDupFound = 1;
      }
   }
   else
   {
      // Updating existing Meta
      if ( SetCursorFirstEntityByString( vLPLR, "W_MetaDef", "Name", szMetaOI_Name, "" ) == zCURSOR_SET )
      {
         do
         {
            if ( CompareAttributeToInteger( vLPLR, "W_MetaDef", "CPLR_ZKey", lMetaOI_ZKey ) != 0 )
            {
               // Duplicately Named Meta exists
               GetVariableFromAttribute( &szNewName, 0, zTYPE_STRING, 125, vLPLR, "W_MetaType", "Type", "CM_Type", 0 );
               nDupFound = 1;
               break;
            }

         } while ( SetCursorNextEntityByString( vLPLR, "W_MetaDef", "Name", szMetaOI_Name, "" ) == zCURSOR_SET );
      }
   }

   if ( nDupFound == 0 )
   {
      // Check one last time for Duplicate Name with a pending Delete.  We won't do the check for Domains and Global Operations since they
      // are "sub metas" and there is no problem if a pending delete exists for a Domain or Global Operation of the same name.

      GetIntegerFromAttribute( &lType, vLPLR, "W_MetaType", "Type" );
      if ( lType >= 2000 )
         lType = lType - 2000;

      if ( lType != zSOURCE_DOMAIN_META && lType != zSOURCE_GO_META )
      {
         CreateViewFromViewForTask( &vTempLPLR, vLPLR, vLPLR );
         SetCursorFirstEntityByInteger( vTempLPLR, "W_MetaType", "Type", lType, "" );
         if ( SetCursorFirstEntityByString( vTempLPLR, "W_MetaDef", "Name", szMetaOI_Name, "" ) == zCURSOR_SET )
         {
            do
            {
               if ( CompareAttributeToInteger( vTempLPLR, "W_MetaDef", "CPLR_ZKey", lMetaOI_ZKey ) != 0 )
               {
                  // Duplicately Named Meta exists
                  GetVariableFromAttribute( &szNewName, 0, zTYPE_STRING, 125, vTempLPLR, "W_MetaType", "Type", "CM_Type",  0 );
                  nDupFound = 1;
                  break;
               }

            } while ( SetCursorNextEntityByString( vTempLPLR, "W_MetaDef", "Name", szMetaOI_Name, "" ) == zCURSOR_SET );
         }

         DropView( vTempLPLR );
      }
   }

   if ( nDupFound == 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "A component of type " );
      strcat_s( szMsg, sizeof( szMsg ), szNewName );
      strcat_s( szMsg, sizeof( szMsg ), " already exists\n" );
      strcat_s( szMsg, sizeof( szMsg ), "with the name of the component you are trying to add," );
      strcat_s( szMsg, sizeof( szMsg ), "\n       " );
      strcat_s( szMsg, sizeof( szMsg ), szMetaOI_Name );
      strcat_s( szMsg, sizeof( szMsg ), "\nSave of the component is aborting." );
      MessageSend( vLPLR, "CM00702", "Configuration Management",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   {
      return( 0 );
   }
}

zSHORT
fnCleanupActiveMetas( zVIEW vActiveMetas, zVIEW vZeidonCM )
{
   zCHAR  szViewName[ 81 ];
   zSHORT nRC;
   zLONG  lTaskID;
   zVIEW  vActiveLookup;
   zVIEW  vMeta;

   CreateViewFromViewForTask( &vActiveLookup, vActiveMetas, vZeidonCM );
   nRC = SetCursorFirstEntity( vActiveLookup, "W_MetaDef", "LPLR" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTaskID, vActiveLookup, "W_MetaDef", "TaskID" );
      if ( lTaskID ) // any non-zero task id is valid (Win9x has negative IDs)
      {
         nRC = SfIsObjectServicesTask( lTaskID );
         if ( nRC != 1 )
         {
            GetStringFromAttribute( szViewName, sizeof( szViewName ), vActiveLookup, "W_MetaDef", "CM_ViewName" );
            strcat_s( szViewName, sizeof( szViewName ), ".u" );
            if ( GetViewByName( &vMeta, szViewName, vZeidonCM, zLEVEL_SUBTASK ) > 0 )
            {
               DropObjectInstance( vMeta );
            }
         }
      }

      nRC = SetCursorNextEntity( vActiveLookup, "W_MetaDef", "LPLR" );
   }

   DropView( vActiveLookup );
   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
