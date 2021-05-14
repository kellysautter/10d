/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzwdgrwo.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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
/*
CHANGE LOG

2000.10.25  SR  Z2000  Length of path variables
   Modified the size of all filename and path variables in whole module
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir
   has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_LoadOrInitialize( zPVIEW   pvGRW,
                            zVIEW    vSubtask )
{
   zVIEW    vLPLR;
   zSHORT   RESULT;
   zSHORT   nRC;
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];

   RESULT = GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( RESULT > 0 )
   {
      nZRetCode = GetStringFromAttribute( szFileName, zsizeof( szFileName ), vLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, zMAX_FILESPEC_LTH + 1 );
   }
   else
   {
      ZeidonStringCopy( szFileName, 1, 0, "", 1, 0, zMAX_FILESPEC_LTH + 1 );
   }
   nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vSubtask, "Dialog", "Tag" );
   ZeidonStringConcat( szFileName, 1, 0, szTempString_0, 1, 0, zMAX_FILESPEC_LTH + 1 );
   ZeidonStringConcat( szFileName, 1, 0, ".dgr", 1, 0, zMAX_FILESPEC_LTH + 1 );
   nRC = ActivateOI_FromFile( pvGRW, "TZWDGRWO", vSubtask, szFileName, zNOI_OKAY );
   if ( nRC < 0 )
   {
      nZRetCode = SetNameForView( *pvGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
      RESULT = CreateEntity( *pvGRW, "Dialog", zPOS_AFTER );
      SetAttributeFromAttribute( *pvGRW, "Dialog", "Tag", vSubtask, "Dialog", "Tag" );
      SetAttributeFromAttribute( *pvGRW, "Dialog", "Desc", vSubtask, "Dialog", "Desc" );
      SetAttributeFromAttribute( *pvGRW, "Dialog", "ZKey", vSubtask, "Dialog", "ZKey" );
   }
   else
   {
      nZRetCode = SetNameForView( *pvGRW, "DlgGraph", vSubtask, zLEVEL_TASK );
      RESULT = SetCursorFirstEntity( *pvGRW, "ER_RelType", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         nZRetCode = DeleteEntity( *pvGRW, "ER_RelType", zREPOS_NONE );
         RESULT = SetCursorNextEntity( *pvGRW, "ER_RelType", "" );
      }
   }
   nZRetCode = oTZWDGRWO_ReInit( *pvGRW, vSubtask );
   nZRetCode = OrderEntityForView( *pvGRW, "ER_Entity", "OwningArea A Type D Name A" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_Save( zVIEW    vGRW )
{
   zVIEW    vLPLR;
   zSHORT   RESULT;
   zSHORT   nRC;
   zCHAR    szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];

   RESULT = GetViewByName( &vLPLR, "TaskLPLR", vGRW, zLEVEL_TASK );
   if ( RESULT > 0 )
   {
      nZRetCode = GetStringFromAttribute( szFileName, zsizeof( szFileName ), vLPLR, "LPLR", "MetaSrcDir" );
      ZeidonStringConcat( szFileName, 1, 0, "\\", 1, 0, zMAX_FILESPEC_LTH + 1 );
   }
   else
   {
      ZeidonStringCopy( szFileName, 1, 0, "", 1, 0, zMAX_FILESPEC_LTH + 1 );
   }
   nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vGRW, "Dialog", "Tag" );
   ZeidonStringConcat( szFileName, 1, 0, szTempString_0, 1, 0, zMAX_FILESPEC_LTH + 1 );
   ZeidonStringConcat( szFileName, 1, 0, ".dgr", 1, 0, zMAX_FILESPEC_LTH + 1 );
   nRC = CommitOI_ToFile( vGRW, szFileName, zASCII );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_ReInit( zVIEW    vGRW_in,
                  zVIEW    vSubtask_in )
{
   zVIEW    vGRW;
   zVIEW    vSubtask;
   zCHAR    szName[ 64 ];
   zSHORT   nFound;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;
   zCHAR    szTempString_0[ 1026 ];

   nZRetCode = CreateViewFromViewForTask( &vGRW, vGRW_in, 0 );
   nZRetCode = CreateViewFromViewForTask( &vSubtask, vSubtask_in, 0 );
   RESULT = SetCursorFirstEntity( vGRW, "ER_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( vGRW, "ER_Entity", "Type", "W" ) == 0 )
      {
         nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vGRW, "ER_Entity", "ZKey" );
         RESULT = SetCursorFirstEntityByInteger( vSubtask, "Window", "ZKey", lTempInteger_0, "" );
         if ( RESULT < zCURSOR_SET )
         {
            nZRetCode = DeleteEntity( vGRW, "ER_Entity", zREPOS_NONE );
         }
      }
      else
      {
         if ( CompareAttributeToString( vGRW, "ER_Entity", "Type", "A" ) == 0 )
         {
            nZRetCode = GetIntegerFromAttribute( &lTempInteger_1, vGRW, "ER_Entity", "ZKey" );
            RESULT = SetCursorFirstEntityByInteger( vSubtask, "Action", "ZKey", lTempInteger_1, "Dialog" );
            if ( RESULT < zCURSOR_SET )
            {
               nZRetCode = DeleteEntity( vGRW, "ER_Entity", zREPOS_NONE );
            }
         }
         else
         {
            if ( CompareAttributeToString( vGRW, "ER_Entity", "Type", "D" ) == 0 )
            {
               nFound = 0;
               RESULT = SetCursorFirstEntity( vSubtask, "Action", "Dialog" );
               if ( RESULT > zCURSOR_UNCHANGED )
               {
                  while ( RESULT > zCURSOR_UNCHANGED &&
                          (CompareAttributeToInteger( vSubtask, "Action", "Type", 30 ) < 0 ||
                           CompareAttributeToInteger( vSubtask, "Action", "Type", 59 ) > 0) )
                  {
                     RESULT = SetCursorNextEntity( vSubtask, "Action", "Dialog" );
                  }
               }

               while ( RESULT >= zCURSOR_SET && nFound == 0 )
               {
                  if ( CompareAttributeToAttribute( vSubtask, "Action", "DialogName", vGRW, "Dialog", "Tag" ) == 0 ||
                       CompareAttributeToString( vSubtask, "Action", "DialogName", "" ) == 0 )
                  {
                     if ( CompareAttributeToString( vSubtask, "Action", "WindowName", "" ) == 0 )
                     {
                        ZeidonStringCopy( szName, 1, 0, "'default'", 1, 0, 63 );
                     }
                     else
                     {
                        nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "WindowName" );
                     }
                  }
                  else
                  {
                     if ( CompareAttributeToString( vSubtask, "Action", "WindowName", "" ) == 0 )
                     {
                        nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "DialogName" );
                        ZeidonStringConcat( szName, 1, 0, ": 'default'", 1, 0, 63 );
                     }
                     else
                     {
                        nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "DialogName" );
                        ZeidonStringConcat( szName, 1, 0, ":", 1, 0, 63 );
                        nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vSubtask, "Action", "WindowName" );
                        ZeidonStringConcat( szName, 1, 0, szTempString_0, 1, 0, 63 );
                     }
                  }
                  if ( CompareAttributeToString( vGRW, "ER_Entity", "Name", szName ) == 0 )
                  {
                     nFound = 1;
                  }
                  RESULT = SetCursorNextEntity( vSubtask, "Action", "Dialog" );
                  RESULT = CheckExistenceOfEntity( vSubtask, "Action" );
                  if ( RESULT > zCURSOR_UNCHANGED )
                  {
                     while ( RESULT > zCURSOR_UNCHANGED &&
                             (CompareAttributeToInteger( vSubtask, "Action", "Type", 30 ) < 0 ||
                              CompareAttributeToInteger( vSubtask, "Action", "Type", 59 ) > 0) )
                     {
                        RESULT = SetCursorNextEntity( vSubtask, "Action", "Dialog" );
                     }
                  }
               }
               if ( nFound == 0 )
               {
                  nZRetCode = DeleteEntity( vGRW, "ER_Entity", zREPOS_NONE );
               }
            }
            else
            {
               nZRetCode = DeleteEntity( vGRW, "ER_Entity", zREPOS_NONE );
            }
         }
      }
      RESULT = SetCursorNextEntity( vGRW, "ER_Entity", "" );
   }
   RESULT = SetCursorFirstEntity( vSubtask, "Window", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_NodeWindowSync( vGRW, vSubtask );
      RESULT = SetCursorNextEntity( vSubtask, "Window", "" );
   }
   RESULT = SetCursorFirstEntity( vSubtask, "Action", "Dialog" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_NodeActionSync( vGRW, vSubtask );
      RESULT = SetCursorNextEntity( vSubtask, "Action", "Dialog" );
   }
   nZRetCode = DropView( vGRW );
   nZRetCode = DropView( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_NodeWindowSync( zVIEW    vGRW,
                          zVIEW    vSubtask )
{
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 255 ];
   zCHAR    szTempString_1[ 33 ];
   zCHAR    szTempString_2[ 33 ];
   zCHAR    szTempString_3[ 33 ];
   zCHAR    szTempString_4[ 33 ];

   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vSubtask, "Window", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW, "ER_Entity", "ZKey", lTempInteger_0, "" );
   if ( RESULT < zCURSOR_SET )
   {
      RESULT = CreateEntity( vGRW, "ER_Entity", zPOS_LAST );
      SetAttributeFromAttribute( vGRW, "ER_Entity", "ZKey", vSubtask, "Window", "ZKey" );
   }
   SetAttributeFromAttribute( vGRW, "ER_Entity", "Name", vSubtask, "Window", "Tag" );
   nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vSubtask, "Dialog", "Tag" );
   ZeidonStringCopy( szTempString_0, 1, 0, "Window -- ", 1, 0, 254 );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 254 );
   ZeidonStringConcat( szTempString_0, 1, 0, ":", 1, 0, 254 );
   nZRetCode = GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vSubtask, "Window", "Tag" );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_2, 1, 0, 254 );
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Desc", szTempString_0 );
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Purpose", "F" );
   if ( CompareAttributeToAttribute( vSubtask, "DfltWnd", "ZKey", vSubtask, "Window", "ZKey" ) == 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_4, zsizeof( szTempString_4 ), vSubtask, "Window", "Tag" );
      ZeidonStringCopy( szTempString_3, 1, 0, "$dlft:", 1, 0, 32 );
      ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 32 );
      nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "OwningArea", szTempString_3 );
   }
   else
   {
      SetAttributeFromAttribute( vGRW, "ER_Entity", "OwningArea", vSubtask, "Window", "Tag" );
   }
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Type", "W" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_NodeActionSync( zVIEW    vGRW,
                          zVIEW    vSubtask )
{
   zVIEW    vGRW_Win;
   zCHAR    szName[ 33 ];
   zLONG    zWAB;
   zSHORT   nEvents;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 255 ];
   zCHAR    szTempString_1[ 33 ];
   zCHAR    szTempString_2[ 33 ];
   zCHAR    szTempString_3[ 33 ];
   zCHAR    szTempString_4[ 33 ];
   zCHAR    szTempString_5[ 1026 ];
   zCHAR    szTempString_6[ 255 ];
   zCHAR    szTempString_7[ 33 ];
   zCHAR    szTempString_8[ 33 ];
   zCHAR    szTempString_9[ 33 ];
   zCHAR    szTempString_10[ 33 ];

   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vSubtask, "Action", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW, "ER_Entity", "ZKey", lTempInteger_0, "" );
   if ( RESULT < zCURSOR_SET )
   {
      RESULT = CreateEntity( vGRW, "ER_Entity", zPOS_LAST );
      SetAttributeFromAttribute( vGRW, "ER_Entity", "ZKey", vSubtask, "Action", "ZKey" );
   }
   SetAttributeFromAttribute( vGRW, "ER_Entity", "Name", vSubtask, "Action", "Tag" );
   nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vSubtask, "Window", "Tag" );
   ZeidonStringCopy( szTempString_0, 1, 0, "Action -- ", 1, 0, 254 );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 254 );
   ZeidonStringConcat( szTempString_0, 1, 0, ":", 1, 0, 254 );
   nZRetCode = GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vSubtask, "Action", "Tag" );
   ZeidonStringConcat( szTempString_0, 1, 0, szTempString_2, 1, 0, 254 );
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Desc", szTempString_0 );
   if ( CompareAttributeToAttribute( vSubtask, "DfltWnd", "ZKey", vSubtask, "Window", "ZKey" ) == 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_4, zsizeof( szTempString_4 ), vSubtask, "Window", "Tag" );
      ZeidonStringCopy( szTempString_3, 1, 0, "$dlft:", 1, 0, 32 );
      ZeidonStringConcat( szTempString_3, 1, 0, szTempString_4, 1, 0, 32 );
      nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "OwningArea", szTempString_3 );
   }
   else
   {
      SetAttributeFromAttribute( vGRW, "ER_Entity", "OwningArea", vSubtask, "Window", "Tag" );
   }
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Type", "A" );
   nZRetCode = CreateViewFromViewForTask( &vGRW_Win, vGRW, 0 );
   nZRetCode = GetIntegerFromAttribute( &zWAB, vSubtask, "Action", "Type" );
   if ( zWAB < 30 || zWAB > 59 )
   {
      nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Purpose", "W" );
   }
   else
   {
      nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Purpose", "D" );
      if ( CompareAttributeToAttribute( vSubtask, "Action", "DialogName", vGRW, "Dialog", "Tag" ) == 0 || CompareAttributeToString( vSubtask, "Action", "DialogName", "" ) == 0 )
      {
         if ( CompareAttributeToString( vSubtask, "Action", "WindowName", "" ) == 0 )
         {
            ZeidonStringCopy( szName, 1, 0, "'default'", 1, 0, 32 );
         }
         else
         {
            nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "WindowName" );
         }
      }
      else
      {
         if ( CompareAttributeToString( vSubtask, "Action", "WindowName", "" ) == 0 )
         {
            nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "DialogName" );
            ZeidonStringConcat( szName, 1, 0, ": 'default'", 1, 0, 32 );
         }
         else
         {
            nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Action", "DialogName" );
            ZeidonStringConcat( szName, 1, 0, ":", 1, 0, 32 );
            nZRetCode = GetStringFromAttribute( szTempString_5, zsizeof( szTempString_5 ), vSubtask, "Action", "WindowName" );
            ZeidonStringConcat( szName, 1, 0, szTempString_5, 1, 0, 32 );
         }
      }
      RESULT = SetCursorFirstEntityByString( vGRW_Win, "ER_Entity", "Name", szName, "" );
      if ( RESULT < zCURSOR_SET )
      {
         nZRetCode = CreateMetaEntity( vSubtask, vGRW_Win, "ER_Entity", zPOS_LAST );
         nZRetCode = SetAttributeFromString( vGRW_Win, "ER_Entity", "Name", szName );
         ZeidonStringCopy( szTempString_6, 1, 0, "Window -- ", 1, 0, 254 );
         ZeidonStringConcat( szTempString_6, 1, 0, szName, 1, 0, 254 );
         nZRetCode = SetAttributeFromString( vGRW_Win, "ER_Entity", "Desc", szTempString_6 );
         nZRetCode = SetAttributeFromString( vGRW_Win, "ER_Entity", "Purpose", "A" );
         if ( CompareAttributeToAttribute( vSubtask, "DfltWnd", "ZKey", vSubtask, "Window", "ZKey" ) == 0 )
         {
            nZRetCode = GetStringFromAttribute( szTempString_8, zsizeof( szTempString_8 ), vSubtask, "Window", "Tag" );
            ZeidonStringCopy( szTempString_7, 1, 0, "$dlft:", 1, 0, 32 );
            ZeidonStringConcat( szTempString_7, 1, 0, szTempString_8, 1, 0, 32 );
            nZRetCode = SetAttributeFromString( vGRW_Win, "ER_Entity", "OwningArea", szTempString_7 );
         }
         else
         {
            SetAttributeFromAttribute( vGRW_Win, "ER_Entity", "OwningArea", vSubtask, "Window", "Tag" );
         }
         nZRetCode = SetAttributeFromString( vGRW_Win, "ER_Entity", "Type", "D" );
      }
      nZRetCode = oTZWDGRWO_ArcActionWindow( vGRW, vGRW_Win, vSubtask );
   }
   nEvents = 0;
   nZRetCode = GetStringFromAttribute( szName, zsizeof( szName ), vSubtask, "Window", "Tag" );
   RESULT = SetCursorFirstEntityByString( vGRW_Win, "ER_Entity", "Name", szName, "" );
   RESULT = SetCursorFirstEntity( vSubtask, "ActWndEvent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_ArcWindowAction( vGRW_Win, vGRW, vSubtask, 1 );
      nEvents = nEvents + 1;
      RESULT = SetCursorNextEntity( vSubtask, "ActWndEvent", "" );
   }
   RESULT = SetCursorFirstEntity( vSubtask, "ActEvent", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_ArcWindowAction( vGRW_Win, vGRW, vSubtask, 2 );
      nEvents = nEvents + 1;
      RESULT = SetCursorNextEntity( vSubtask, "ActEvent", "" );
   }
   RESULT = SetCursorFirstEntity( vSubtask, "ActOpt", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_ArcWindowAction( vGRW_Win, vGRW, vSubtask, 3 );
      nEvents = nEvents + 1;
      RESULT = SetCursorNextEntity( vSubtask, "ActOpt", "" );
   }
   RESULT = SetCursorFirstEntity( vSubtask, "ActHot", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      nZRetCode = oTZWDGRWO_ArcWindowAction( vGRW_Win, vGRW, vSubtask, 4 );
      nEvents = nEvents + 1;
      RESULT = SetCursorNextEntity( vSubtask, "ActHot", "" );
   }
   if ( nEvents == 0 )
   {
      nZRetCode = GetStringFromAttribute( szTempString_9, zsizeof( szTempString_9 ), vSubtask, "Window", "Tag" );
      ZeidonStringConcat( szTempString_9, 1, 0, ":", 1, 0, 32 );
      nZRetCode = GetStringFromAttribute( szTempString_10, zsizeof( szTempString_10 ), vSubtask, "Action", "Tag" );
      ZeidonStringConcat( szTempString_9, 1, 0, szTempString_10, 1, 0, 32 );
      nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Name", szTempString_9 );
   }
   else
   {
      if ( nEvents > 1 )
      {
         nZRetCode = oTZWDGRWO_ArcGroupEvents( vGRW_Win, vGRW, vSubtask, nEvents );
      }
   }
   nZRetCode = DropView( vGRW_Win );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_ArcActionWindow( zVIEW    vGRW_Action,
                           zVIEW    vGRW_Window,
                           zVIEW    vSubtask )
{
   zCHAR    zWAB_Text[ 128 ];
   zULONG   nLth;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;

   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Action, "ER_RelType", zPOS_LAST );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelType", "Fact", "Action to Window" );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelType", "Type", "W" );
   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Action, "ER_RelLink_2", zPOS_LAST );
   nZRetCode = GetVariableFromAttribute( zWAB_Text, &nLth, zTYPE_STRING, 127, vSubtask, "Action", "Type", "", zUSE_DEFAULT_CONTEXT );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelLink_2", "Name", zWAB_Text );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelLink_2", "CardMin", "0" );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelLink_2", "CardMax", "1" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Action, "ER_Entity_2", vGRW_Action, "ER_Entity", zPOS_AFTER );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vGRW_Action, "ER_RelLink_2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW_Action, "ER_RelLink", "ZKey", lTempInteger_0, "" );
   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Action, "ER_RelLink_2", zPOS_LAST );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelLink_2", "CardMin", "1" );
   nZRetCode = SetAttributeFromString( vGRW_Action, "ER_RelLink_2", "CardMax", "1" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Action, "ER_Entity_2", vGRW_Window, "ER_Entity", zPOS_AFTER );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_1, vGRW_Action, "ER_RelLink_2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW_Window, "ER_RelLink", "ZKey", lTempInteger_1, "" );
   SetAttributeFromAttribute( vGRW_Window, "ER_RelType_1", "Type", vGRW_Action, "ER_RelType", "Type" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Action, "ER_RelLink_Other", vGRW_Action, "ER_RelLink_2", zPOS_AFTER );
   RESULT = SetCursorPrevEntity( vGRW_Action, "ER_RelLink_2", "" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Window, "ER_RelLink_Other", vGRW_Action, "ER_RelLink_2", zPOS_AFTER );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_ArcWindowAction( zVIEW    vGRW_Window,
                           zVIEW    vGRW_Action,
                           zVIEW    vSubtask,
                           zSHORT   nCtrlType )
{
   zVIEW    vSubtask2;
   zCHAR    szToName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szFromName[ zMAX_FILESPEC_LTH + 1 ];
   zLONG    zControl;
   zSHORT   nRC;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];
   zCHAR    szTempString_1[ 33 ];
   zCHAR    szTempString_2[ 33 ];
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;

   switch( nCtrlType )
   {
      case 1  :
         ZeidonStringCopy( szToName, 1, 0, "(Window)", 1, 0, zMAX_FILESPEC_LTH + 1 );
         nZRetCode = GetStringFromAttribute( szFromName, zsizeof( szFromName ), vSubtask, "ActWndEvent", "EventName" );
         break;
      case 2  :
         nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vSubtask, "ActCtrl", "Tag" );
         ZeidonStringCopy( szToName, 1, 0, "(Ctrl) ", 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringConcat( szToName, 1, 0, szTempString_0, 1, 0, zMAX_FILESPEC_LTH + 1 );
         nZRetCode = CreateViewFromViewForTask( &vSubtask2, vSubtask, 0 );
         nZRetCode = ResetView( vSubtask2 );
         zControl = zPOS_FIRST + zQUAL_ENTITYATTR + zRECURS;
         nRC = SetEntityCursor( vSubtask2, "Event", "ZKey", zControl, vSubtask, "ActEvent", "ZKey", 0, "Window", "" );
         if ( nRC > zCURSOR_UNCHANGED )
         {
            nZRetCode = GetStringFromAttribute( szFromName, zsizeof( szFromName ), vSubtask2, "ControlDef", "Tag" );
         }
         else
         {
            ZeidonStringCopy( szFromName, 1, 0, "unknown ctrl event", 1, 0, zMAX_FILESPEC_LTH + 1 );
         }
         nZRetCode = DropView( vSubtask2 );
         break;
      case 3  :
         nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vSubtask, "ActOpt", "Text" );
         ZeidonStringCopy( szToName, 1, 0, "(Opt) ", 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringConcat( szToName, 1, 0, szTempString_1, 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringCopy( szFromName, 1, 0, "option select", 1, 0, zMAX_FILESPEC_LTH + 1 );
         break;
      case 4  :
         nZRetCode = GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vSubtask, "ActHot", "Tag" );
         ZeidonStringCopy( szToName, 1, 0, "(Shortcut Key) ", 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringConcat( szToName, 1, 0, szTempString_2, 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringCopy( szFromName, 1, 0, "keyed", 1, 0, zMAX_FILESPEC_LTH + 1 );
         break;
      case 5  :
         ZeidonStringCopy( szToName, 1, 0, "n", 1, 0, zMAX_FILESPEC_LTH + 1 );
         ZeidonStringCopy( szFromName, 1, 0, "multiple", 1, 0, zMAX_FILESPEC_LTH + 1 );
         break;
      default  :
         return( -1 );
         break;
   }
   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Window, "ER_RelType", zPOS_LAST );
   if ( nCtrlType == 5 )
   {
      nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelType", "Fact", "Group: " );
      nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelType", "Type", "G" );
   }
   else
   {
      nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelType", "Fact", "Window to Action" );
      nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelType", "Type", "E" );
   }
   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Window, "ER_RelLink_2", zPOS_LAST );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "Name", szToName );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "CardMin", "0" );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "CardMax", "m" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Window, "ER_Entity_2", vGRW_Window, "ER_Entity", zPOS_AFTER );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vGRW_Window, "ER_RelLink_2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW_Window, "ER_RelLink", "ZKey", lTempInteger_0, "" );
   nZRetCode = CreateMetaEntity( vSubtask, vGRW_Window, "ER_RelLink_2", zPOS_LAST );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "Name", szFromName );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "CardMin", "1" );
   nZRetCode = SetAttributeFromString( vGRW_Window, "ER_RelLink_2", "CardMax", "1" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Window, "ER_Entity_2", vGRW_Action, "ER_Entity", zPOS_AFTER );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_1, vGRW_Window, "ER_RelLink_2", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW_Action, "ER_RelLink", "ZKey", lTempInteger_1, "" );
   SetAttributeFromAttribute( vGRW_Action, "ER_RelType_1", "Type", vGRW_Window, "ER_RelType", "Type" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Window, "ER_RelLink_Other", vGRW_Window, "ER_RelLink_2", zPOS_AFTER );
   RESULT = SetCursorPrevEntity( vGRW_Window, "ER_RelLink_2", "" );
   RESULT = IncludeSubobjectFromSubobject( vGRW_Action, "ER_RelLink_Other", vGRW_Window, "ER_RelLink_2", zPOS_AFTER );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_ArcGroupEvents( zVIEW    vGRW_Win,
                          zVIEW    vGRW_Act,
                          zVIEW    vSubtask,
                          zSHORT   nEvents )
{
   zVIEW    vGRW_Tmp;
   zSHORT   nZRetCode;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zCHAR    szTempString_0[ 255 ];
   zCHAR    szTempString_1[ 33 ];

   nZRetCode = oTZWDGRWO_ArcWindowAction( vGRW_Win, vGRW_Act, vSubtask, 5 );
   nZRetCode = SetAttributeFromString( vGRW_Win, "ER_RelType", "Fact", "Group: " );
   nZRetCode = CreateViewFromViewForTask( &vGRW_Tmp, vGRW_Act, 0 );
   nZRetCode = GetIntegerFromAttribute( &lTempInteger_0, vGRW_Act, "ER_Entity", "ZKey" );
   RESULT = SetCursorFirstEntityByInteger( vGRW_Act, "ER_Entity_2", "ZKey", lTempInteger_0, "Dialog" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( vGRW_Act, "ER_RelType", "Type", "E" ) == 0 )
      {
         nZRetCode = SetViewFromView( vGRW_Tmp, vGRW_Act );
         RESULT = SetCursorFirstEntity( vGRW_Tmp, "ER_RelLink_2", "" );
         nZRetCode = GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vGRW_Win, "ER_RelType", "Fact" );
         ZeidonStringConcat( szTempString_0, 1, 0, " | ", 1, 0, 254 );
         nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vGRW_Tmp, "ER_RelLink_2", "Name" );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 254 );
         nZRetCode = SetAttributeFromString( vGRW_Win, "ER_RelType", "Fact", szTempString_0 );
      }
      RESULT = SetCursorNextEntityByInteger( vGRW_Act, "ER_Entity_2", "ZKey", lTempInteger_0, "Dialog" );
   }
   nZRetCode = DropView( vGRW_Tmp );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZWDGRWO_NodeEntitySync( zVIEW    vGRW,
                          zVIEW    vSubtask,
                          zSHORT   nAddOrMerge,
                          zPCHAR   szMetaName )
{
   zCHAR    szAttr[ 33 ];
   zSHORT   RESULT;
   zSHORT   nZRetCode;
   zCHAR    szTempString_0[ 33 ];
   zCHAR    szTempString_1[ 33 ];

   if ( nAddOrMerge == 0 )
   {
      RESULT = CreateEntity( vGRW, "ER_Entity", zPOS_LAST );
   }
   ZeidonStringCopy( szAttr, 1, 0, "ZKey", 1, 0, 32 );
   SetAttributeFromAttribute( vGRW, "ER_Entity", "ZKey", vSubtask, "szMetaName", "szAttr" );
   ZeidonStringCopy( szAttr, 1, 0, "Name", 1, 0, 32 );
   SetAttributeFromAttribute( vGRW, "ER_Entity", "Name", vSubtask, "szMetaName", "szAttr" );
   SetAttributeFromAttribute( vGRW, "ER_Entity", "Desc", vSubtask, "szMetaName", "szAttr" );
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Purpose", "A" );
   if ( CompareAttributeToString( vGRW, "ER_Entity", "OwningArea", "" ) == 0 )
   {
      if ( CompareAttributeToAttribute( vSubtask, "DfltWnd", "ZKey", vSubtask, "Window", "ZKey" ) == 0 )
      {
         nZRetCode = GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vSubtask, "Window", "Tag" );
         ZeidonStringCopy( szTempString_0, 1, 0, "$dlft:", 1, 0, 32 );
         ZeidonStringConcat( szTempString_0, 1, 0, szTempString_1, 1, 0, 32 );
         nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "OwningArea", szTempString_0 );
      }
      else
      {
         SetAttributeFromAttribute( vGRW, "ER_Entity", "OwningArea", vSubtask, "Window", "Tag" );
      }
   }
   nZRetCode = SetAttributeFromString( vGRW, "ER_Entity", "Type", "E" );
   return( 0 );
}

///////////////////////////// end of file  //////////////////////////////////
