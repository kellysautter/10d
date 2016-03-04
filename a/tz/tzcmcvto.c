/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzcmcvto.c - Zeidon application operations
// DESCRIPTION:   This C file contains the conversion routines to convert
//                a dialog from source to executable format.
//
/////////////////////////////////////////////////////////////////////////////
// This is a part of the Zeidon tools package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
// Confidential and Proprietary material subject to license - do not
// reproduce or disclose.  This material is an unpublished work and
// is considered a trade secret belonging to the copyright holder.
/////////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9J   1998/01/27
//
// LAST MESSAGE ID: CM00816
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2002.05.17  DKS   Z10
   Implemented simple proportional resize when a "single" control is
   marked as proportional resize.

2001.12.17  DKS   Z10
   Set CSV Column for each ctrl in a report.

2001.12.03  DKS   WEB
   Handle RegWEB=A ... always force call to HandleEvent rather than
   processing the action.

2001.10.22  HH    WEB
   Do not transfer WndEvent 1 (PreBuild) and 2 (PostBuild) to XRA.

2001.10.16  DKS   WEB
   Create Alt+F4 WndEvent and ESC Hotkey for default processing.

2001.07.31  DKS   WEB
   Set the attribute Type, Ctrl, Alt, Shift in the Hotkey entity.

2001.05.28  PAS
   Changed BottomRange to be one less.  Position + Height - 1 is the range
   of a control.

2001.04.18  DKS   Hartmut
   Fixed loop to reset the range of controls in report so that it doesn't
   exponentially increase the number of changes/comparisons as the number
   of controls increases.

2001.02.16  BL    F50876
   Avoid starting the Painter if Ctrl+P is pressed. If there is no
   action this shortcut invokes the Painter.

2001/02/05  DKS   WEB
   Use IdNbr to sequence Opt entities (Tag revert to original usage).

2000.08.23  BL    Z10
   Modified ConvertDialog: insert check exists of Entity

1999.09.07  DKS   Z10   ActiveX
   Implemented setting of Tag and Type attributes for the CtrlMap Entity
   in the XWD.

1999.05.11  DKS   Z10   ActiveX
   Implemented setting of ActiveX attributes.

1999.02.15  DKS
   Implemented report conversion to flatten CtrlCtrl to Controls under
   a Group (the report painter permits recursive controls).

1998.10.12  DKS
   Modified Dialog conversion to set the size of the Relative Pos/Size
   area properly.

1997.03.11  DonC
   Modified ConvertDialog to retrieve TZPESRCO view if a named view by the
   same name doesn't already exist.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.h"


#define zZEIDON_FIRST_TOOLBAR         0xE81B /* AFX_IDW_DOCKBAR_TOP */ + 32

#ifdef _WINDOWS
#define zMNEMONIC_FLAG              '&'
#else
#define zMNEMONIC_FLAG              '~'
#endif

#include <zdrvropr.h>

#pragma pack( 1 )
#include <tzwdlgso.hst>
#pragma pack( )

#define zTEMPORAL_SUBOBJECT_NEXT             0x00000080L
#define zTEMPORAL_SUBOBJECT_PREV             0x00000100L
#define zTEMPORAL_SUBOBJECT_DELETE           0x00000004L
#define zTEMPORAL_SUBOBJECT_EXCLUDE          0x00000008L

typedef zSHORT (POPERATION zFARPROC_CONVERT)( zVIEW vSubtask,
                                              zVIEW vTgtDialog,
                                              zVIEW vTgtRemote,
                                              zVIEW vSrcDialog,
                                              zPSHORT nCtrlID );
typedef zSHORT (POPERATION zFARPROC_DUP_CHECK)( zVIEW vSubtask,
                                                zVIEW vSrcDialog,
                                                zLONG lFlags );
// local prototypes
void
fnProcessControls( zVIEW vSubtask, zVIEW vTgtDialog, zVIEW vTgtRemote, zVIEW vPE );
void
fnProcessControlsAtLevel( zVIEW   vSubtask,
                          zVIEW   vTgtWndDialog, // view to Wnd level for DIL
                          zVIEW   vTgtWndRemote,
                          zVIEW   vTgtDialog,
                          zVIEW   vTgtRemote,
                          zVIEW   vSrcWndDialog, // view to Window level
                          zVIEW   vSrcDialog,
                          zVIEW   vPE,
                          zLONG   lOffsetX,
                          zLONG   lOffsetY,
                          zPLONG  plExtentX,
                          zPLONG  plExtentY,
                          zLONG   lFlags );
void
fnProcessOptions( zVIEW vSubtask,
                  zVIEW vTgtWndDialog,           // view to Wnd level for DIL
                  zVIEW vTgtWndRemote,
                  zVIEW vTgtDialog,
                  zVIEW vTgtRemote,
                  zVIEW vSrcDialog );

// following defines copied from zdrapp.h :-(
#define zWND_EVENT_PRE_BUILD                 0x00000001L
#define zWND_EVENT_POST_BUILD                0x00000002L
#define zWND_EVENT_ON_RETURN                 0x00000008L
#define zWND_EVENT_CLOSE                     0x00000100L
#define zWND_EVENT_ACTION_AUTOGRAY           0x80000000L

// global
RECT   g_rectRelSizeX;
RECT   g_rectRelSizeY;
zSHORT g_nRelSizeCntX;
zSHORT g_nRelSizeCntY;
zSHORT g_nActCnt;
zSHORT g_nCtrlCnt;
zSHORT g_nCtrlRelTop;
zSHORT g_nCtrlRelLeft;
zSHORT g_nCtrlRelBottom;
zSHORT g_nCtrlRelRight;
zSHORT g_nOptCnt;
zSHORT g_nCtrlID;
zVIEW  g_vTgtDialogOrig;
zVIEW  g_vTgtRemoteOrig;
zPCHAR g_lpHotkey;
zPCHAR g_lpEventAct;
zUSHORT g_usHotkeyLth;
zCHAR  g_chDIL_Active;
zACCEL_TABLE g_AccelTable;
zACCEL_TABLE *g_lpAT;
double g_dDLTextEditUnits;

//
// Removes all occurrences of zCHAR 'c' from the string. If bDouble is TRUE,
// then don't remove both characters that are doubled; eg if:
//       pszStr  = "abc&def&&x"
//       c       = '&'
//       bDouble = TRUE
//    then result is:
//       str = "abcdef&x"
//

void
fnRemoveChar( zPCHAR pszStr,
              zCHAR  c,
              zSHORT bDouble )
{
   zPCHAR pszEnd = pszStr;

   while ( *pszStr )
   {
      if ( *pszStr != c )
         *pszEnd++ = *pszStr++;
      else
      if ( !bDouble || pszStr[ 1 ] != c )
         pszStr++;       // Skip the char.
      else
      {
         // The zCHAR 'c' is doubled and we're only supposed to skip one.
         *pszEnd++ = *pszStr++;
         pszStr++;       // Skip the char.
      }
   } // while...

   *pszEnd = 0;
}

#if 0
zSHORT
fnCloneAction( zVIEW     vSrcLPLR,
               zVIEW     vSrc,
               zVIEW     vTgt,
               zVIEW     vSubtask )
{
   zLONG     lLth;
   zCHAR     szLanguageType[ 2 ];
   zCHAR     szSourceName[ 33 ];
   zCHAR     szTag[ 33 ];
   zSHORT    nRC;

   GetStringFromAttribute( szTag, zsizeof( szTag ), vSrc, "Action", "Tag" );
   nRC = SetCursorFirstEntityByString( vTgt, "Action", "Tag", szTag, 0 );
   if ( nRC >= zCURSOR_SET )
   {
      return( 0 );
   }

   if ( CompareAttributeToString( vSrc, "Action", "Tag", "" ) != 0 )
   {
      CreateMetaEntity( vSubtask, vTgt, "Action", zPOS_AFTER );
      SetMatchingAttributesByName( vTgt, "Action", vSrc, "Action", zSET_NULL );
   }

   nRC = SetCursorFirstEntity( vSrc, "ActMap", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneActMap( vSrcLPLR, vSrc, vTgt, vSubtask ) == -1 )
         return( -1 );

      nRC = SetCursorNextEntity( vSrc, "ActMap", 0 );
   }

   if ( CheckExistenceOfEntity( vSrc, "ActOper" ) == 0 )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vSrc, "ActOper", "Name" );
      nRC = SetCursorFirstEntityByString( vTgt, "Operation", "Name",
                                          szTag, "Dialog" );
      if ( nRC < zCURSOR_SET )
      {
         GetVariableFromAttribute( szLanguageType, 0, 'S', 2, vSrc,
                                   "SourceFile", "LanguageType", "", 0 );
         nRC = SetCursorFirstEntityByString( vTgt, "SourceFile",
                                             "LanguageType", szLanguageType,
                                             0 );
         if ( nRC < zCURSOR_SET )
         {
            GetVariableFromAttribute( szSourceName, 0, 'S', 33, vTgt,
                                      "Dialog", "Tag", "", 0 );
            nRC = SetCursorFirstEntityByString( vTgt, "SourceFile", "Name",
                                                szSourceName, 0 );
            if ( nRC >= zCURSOR_SET )
            {
               lLth = zstrlen( szSourceName );
               if ( lLth >= 8 )
               {
                  strcpy_s( szSourceName, zsizeof( szSourceName ), szSourceName );
               }

               strcat_s( szSourceName, zsizeof( szSourceName ), szLanguageType );
            }

            CreateMetaEntity( vSubtask, vTgt, "SourceFile", zPOS_AFTER );
            SetAttributeFromString( vTgt, "SourceFile", "Name",
                                    szSourceName );
            SetAttributeFromString( vTgt, "SourceFile",
                                    "LanguageType", szLanguageType );
            if ( szLanguageType[ 0 ] == 'V' )
            {
               SetAttributeFromString( vTgt, "SourceFile",
                                       "Extension", "VML" );
            }
            else
            {
               SetAttributeFromString( vTgt, "SourceFile",
                                       "Extension", "C" );
            }
         }

         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrc, "ActOper", "Name" );
         nRC = SetCursorFirstEntityByString( vSrc, "Operation",
                                             "Name", szTag, "Dialog" );
         CreateMetaEntity( vSubtask, vTgt, "Operation", zPOS_AFTER );
         SetMatchingAttributesByName( vTgt, "Operation",
                                      vSrc, "Operation", zSET_NULL );
         nRC = SetCursorFirstEntity( vSrc, "Parameter", 0 );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            CreateMetaEntity( vSubtask, vTgt, "Parameter", zPOS_AFTER );
            SetMatchingAttributesByName( vTgt, "Parameter",
                                         vSrc, "Parameter", zSET_NULL );
            nRC = SetCursorNextEntity( vSrc, "Parameter", 0 );
         }
      }

      nRC = IncludeSubobjectFromSubobject( vTgt, "ActOper",
                                           vTgt, "Operation", zPOS_AFTER );
   }

   return( 0 );
}
#endif

zSHORT
fnCloneActions( zVIEW vAct, zVIEW vActSrc, zVIEW vSubtask, zCPCHAR cpcDLL_Name, zCHAR ch )
{
   zCHAR    szTag[ 35 ];
   zSHORT   nOffset;
   zSHORT   nRC;

   // Because the menu actions can be created on separate windows from the control
   // actions, we need to make sure these actions are unique.  We will prefix "sm"
   // to the side menu and "tm" to the top menu actions.
   if ( ch )
   {
      szTag[ 0 ] = ch;
      szTag[ 1 ] = 'm';
      nOffset = 2;
   }
   else
      nOffset = 0;

   nRC = SetCursorFirstEntity( vActSrc, "Action", "" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vActSrc, "ActOpt" ) == 0 )
      {
         // We have an option tied to this action, so we want to add the action.

         GetStringFromAttribute( szTag + nOffset, zsizeof( szTag ) - nOffset, vActSrc, "Action", "Tag" );
         szTag[ 32 ] = 0;  // force within 32 byte length

         CreateMetaEntity( vSubtask, vAct, "Action", zPOS_LAST );
         SetMatchingAttributesByName( vAct, "Action",
                                      vActSrc, "Action", zSET_NULL );
         SetAttributeFromString( vAct, "Action", "Tag", szTag );

         if ( CheckExistenceOfEntity( vActSrc, "ActOper" ) == 0 )
         {
            GetStringFromAttribute( szTag + nOffset, zsizeof( szTag ) - nOffset, vActSrc, "ActOper", "Name" );
            nRC = SetCursorFirstEntityByString( vAct, "Operation", "Name",
                                                szTag, "Dialog" );
            if ( nRC < zCURSOR_SET )
            {
               GetStringFromAttribute( szTag + nOffset, zsizeof( szTag ) - nOffset, vActSrc, "ActOper", "Name" );
               nRC = SetCursorFirstEntityByString( vActSrc, "Operation",
                                                   "Name", szTag + nOffset, "Dialog" );
               SetAttributeFromString( vAct, "Action", "DLL", cpcDLL_Name );
               CreateMetaEntity( vSubtask, vAct, "Operation", zPOS_AFTER );
               SetMatchingAttributesByName( vAct, "Operation",
                                            vActSrc, "Operation", zSET_NULL );
            }

            nRC = IncludeSubobjectFromSubobject( vAct, "ActOper",
                                                 vAct, "Operation", zPOS_AFTER );
         }
      }

      nRC = SetCursorNextEntity( vActSrc, "Action", "" );
   }

   return( 0 );
}

zSHORT
fnCloneOption( zVIEW    vOrigW,
               zVIEW    vOrigWO,
               zVIEW    vNewW,
               zVIEW    vNewWO,
               zVIEW    vSubtask,
               zSHORT   nPos,
               zCHAR    ch )
{
// zCHAR    szViewName[ 33 ];
   zCHAR    szTag[ 35 ];
   zSHORT   nOffset;
   zSHORT   nRC;

   // Because the menu actions can be created on separate windows from the control
   // actions, we need to make sure these actions are unique.  We will prefix "sm"
   // to the side menu actions and "tm" to the top menu actions.
   if ( ch )
   {
      nOffset = 2;
      szTag[ 0 ] = ch;
      szTag[ 1 ] = 'm';
   }
   else
      nOffset = 0;

   CreateMetaEntity( vSubtask, vNewWO, "Option", nPos );
   nPos = zPOS_AFTER;
   SetMatchingAttributesByName( vNewWO, "Option", vOrigWO, "Option", zSET_NULL );
   nRC = SetCursorFirstEntity( vOrigWO, "OptOpt", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( vOrigWO, "OptOpt" );
      SetViewToSubobject( vNewWO, "OptOpt" );
      fnCloneOption( vOrigW, vOrigWO, vNewW, vNewWO, vSubtask, nPos, ch );
      ResetViewFromSubobject( vOrigWO );
      ResetViewFromSubobject( vNewWO );
      nRC = SetCursorNextEntity( vOrigWO, "OptOpt", "" );
   }

#if 0 // skip OptMap for now
   GetStringFromAttribute( szViewName, zsizeof( szViewName ), vOrigWO, "OptMap", "X_ViewName" );
   nRC = SetCursorFirstEntity( vOrigWO, "OptMap", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CloneOptMap( vOrigW, vOrigWO, vNewW, vNewWO, szViewName, vSubtask );
      nRC = SetCursorNextEntity( vOrigWO, "OptMap", "" );
   }
#endif

   nRC = CheckExistenceOfEntity( vOrigWO, "OptAct" );
   if ( nRC == zCURSOR_SET )
   {
      if ( CompareAttributeToString( vOrigWO, "OptAct", "Tag", "" ) != 0 )
      {
         GetStringFromAttribute( szTag + nOffset, zsizeof( szTag ) - nOffset, vOrigWO, "OptAct", "Tag" );
         szTag[ 32 ] = 0;
         nRC = SetCursorFirstEntityByString( vNewW, "Action", "Tag", szTag, "" );
         if ( nRC >= 0 )
         {
            IncludeSubobjectFromSubobject( vNewWO, "OptAct", vNewW,
                                           "Action", zPOS_AFTER );
         }
      }
   }

   return( 0 );
}

zSHORT
fnCloneMenuAndOptions( zVIEW  vNewW,
                       zVIEW  vOrigW,
                       zVIEW  vSubtask,
                       zBOOL  bDflt,
                       zCHAR  ch )
{
   zCHAR    szTag[ 33 ];
   zVIEW    vNewWO;
   zVIEW    vOrigWO;
   zSHORT   nPos;
   zSHORT   nRC;

// nRC = SetCursorFirstEntity( vOrigW, "Menu", "" );
// while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( bDflt )
      {
         nPos = zPOS_FIRST;
      }
      else
      {
         CreateMetaEntity( vSubtask, vNewW, "Menu", zPOS_AFTER );
         SetMatchingAttributesByName( vNewW, "Menu", vOrigW, "Menu", zSET_NULL );
         nPos = zPOS_AFTER;
      }

      CreateViewFromViewForTask( &vNewWO, vNewW, 0 );
      CreateViewFromViewForTask( &vOrigWO, vOrigW, 0 );
      SetNameForView( vNewWO, "NewWO", vSubtask, zLEVEL_TASK );
      SetNameForView( vOrigWO, "OrigWO", vSubtask, zLEVEL_TASK );
      nRC = SetCursorFirstEntity( vOrigWO, "Option", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         fnCloneOption( vOrigW, vOrigWO, vNewW, vNewWO, vSubtask, nPos, ch );
         nPos = zPOS_AFTER;
         nRC = SetCursorNextEntity( vOrigWO, "Option", "" );
      }

      DropView( vNewWO );
      DropView( vOrigWO );
   // nRC = SetCursorNextEntity( vOrigW, "Menu", "" );
   }

   if ( bDflt == FALSE )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vOrigW, "DfltMenu", "Tag" );
      nRC = SetCursorFirstEntityByString( vNewW, "Menu", "Tag", szTag, "" );
      IncludeSubobjectFromSubobject( vNewW, "DfltMenu", vNewW,
                                     "Menu", zPOS_AFTER );
   }

   return( 0 );
}

////////////////////////// CONVERT DIALOG ///////////////////////////////////
////////////////////////// CONVERT DIALOG ///////////////////////////////////
////////////////////////// CONVERT DIALOG ///////////////////////////////////
////////////////////////// CONVERT DIALOG ///////////////////////////////////

#define zMAX_BLOB                            128000L


zOPER_EXPORT zVIEW OPERATION
ConvertDialog( zVIEW   vSubtask,
               zVIEW   vSrcDialog,
               zCPCHAR cpcTgtDlgName,
               zPVIEW  pvXRA )
{
// zCHAR  szAppName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR  szAppSection[ zZEIDON_NAME_LTH + 7 ];
   zCHAR  szBuffer[ zLINESIZE ];
   zFARPROC_DUP_CHECK lpfnDuplicateCheck;
   zLONG  hLibrary;
   zVIEW  vMetaList;
   zVIEW  vPE;
   zVIEW  vWork;
   zPCHAR pch;
   zVIEW  vSrcWndDialog;
   zVIEW  vTgtWndDialog;
   zVIEW  vTgtWndRemote;
   zVIEW  vTgtDialog;
   zVIEW  vTgtRemote;
   zVIEW  vAct;
   zVIEW  vActSrc;
   zULONG ul;
   zULONG ulWndOper;
   zULONG ulWndType;
   zULONG ulActSB;
   zSHORT nRC;
   zLONG  lZKey;
   zLONG  lZKeyTmp;
   zLONG  lNLS;
   zBOOL  bMDI;
   zBOOL  bESC;
   zBOOL  bSystemClose;
   zCHAR  szRemoteWndTag[ 66 ];
   zSHORT nRemoteDlgTagLth;
   zCHAR  szDfltWnd[ 33 ];    // name of the default window
   zCHAR  szDfltBtn[ 33 ];    // name of the default button
   zCHAR  szDfltMenu[ 33 ];   // name of the default menu
   zCHAR  szTempString[ 256 ];
   zCHAR  szReusableMainSideWindow[ 2 ];
   zULONG ulLth;
   zLONG  lSize;
   zLONG  lExtentX;
   zLONG  lExtentY;
   zLONG  lFlags;
   zSHORT k;

   SysMutexLock( vSubtask, "ZDrConvertDialog", 0, 0 );

   if ( (nRC = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
      if ( LoadZeidonPPE( vSubtask, &vPE, zREFER_PENV_META, vMetaList,
                          "Configuration Management", "") < 0 )
      {
         TraceLineS( "(tzcmcvto) Cannot get Custom Control View - ", "TZPESRCO" );
         SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
         return( 0 );
      }

      SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
      DropView( vMetaList );
   }

   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &g_lpHotkey, zMAX_BLOB ) != 0 )
   {
      SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
      return( 0 );
   }

   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &g_lpEventAct, zMAX_BLOB ) != 0 )
   {
      SfFreeTaskMemory( g_lpHotkey );
      SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
      return( 0 );
   }

   // Load the executable version of the dialog.
   if ( (nRC = SfActivateSysEmptyOI( &vTgtDialog, "KZWDLGXO",
                                     vSubtask, zSINGLE )) != 0 )
   {
      // ERROR out of here ...
      TraceLineS( "(tzcmcvto) Cannot activate executable Dialog - ",
                  "KZWDLGXO" );
      MessageSend( vSubtask, "CM00802", "Configuration Management",
                   "Cannot activate executable Dialog",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SfFreeTaskMemory( g_lpHotkey );
      SfFreeTaskMemory( g_lpEventAct );
      SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
      return( 0 );
   }
   else
      SetNameForView( vTgtDialog, "__Convert_X", vSubtask, zLEVEL_TASK );

   // Load the executable version of the dialog.
   if ( (nRC = SfActivateSysEmptyOI( &vTgtRemote, "KZXRA",
                                     vSubtask, zSINGLE )) != 0 )
   {
      // ERROR out of here ...
      TraceLineS( "(tzcmcvto) Cannot activate Executable Remote Window - ",
                  "KZWDLGXO" );
      MessageSend( vSubtask, "CM00802", "Configuration Management",
                   "Cannot activate Executable Remote Window",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SfFreeTaskMemory( g_lpHotkey );
      SfFreeTaskMemory( g_lpEventAct );
      SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
      return( 0 );
   }
   else
      SetNameForView( vTgtRemote, "__Convert_XRA", vSubtask, zLEVEL_TASK );

   SysReadZeidonIni( -1, "[Workstation]", "ReusableMainSideWindow", szReusableMainSideWindow, zsizeof( szReusableMainSideWindow ) );

   // So we can modify the Dialog and not worry about the consequences.
   ActivateOI_FromOI_ForTask( &vSrcDialog, vSrcDialog, vSubtask, 0 );
   SetNameForView( vSrcDialog, "__Convert_S**", vSubtask, zLEVEL_TASK );

   // Make sure we are positioned at the beginning of the Dialog we are
   // about to convert.
   SetCursorFirstEntity( vSrcDialog, "Window", 0 );
   GetAddrForAttribute( &pch, vSrcDialog, "Dialog", "Tag" );

   g_lpHotkey = (zPCHAR) SfLockTaskMemory( g_lpHotkey );
   g_lpEventAct = (zPCHAR) SfLockTaskMemory( g_lpEventAct );

   GetStringFromAttribute( szRemoteWndTag, zsizeof( szRemoteWndTag ), vSrcDialog, "Dialog", "Tag" );
   nRemoteDlgTagLth = (zSHORT) zstrlen( szRemoteWndTag );
   szRemoteWndTag[ nRemoteDlgTagLth++ ] = '.';

   CreateEntity( vTgtDialog, "Dlg", zPOS_AFTER );
   SetMatchingAttributesByName( vTgtDialog, "Dlg",
                                vSrcDialog, "Dialog", zSET_ALL );
   CreateEntity( vTgtRemote, "App", zPOS_AFTER );
   SetAttributeFromAttribute( vTgtRemote, "App", "Tag",
                              vSrcDialog, "Dialog", "Tag" );
   SetAttributeFromString( vTgtRemote, "App", "Definition", "Y" );

   SetResourceFileIDs( vSubtask, vSrcDialog, 0, 0, szBuffer, zsizeof( szBuffer ), vSrcDialog );
   ResetView( vSrcDialog );

   g_dDLTextEditUnits = 0.0;

   // Get Resource Image and URL names if specified.
   GetViewByName( &vWork, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vWork )
   {
      GetAddrForAttribute( &pch, vWork, "LPLR", "Name" );
   // GetApplDirectoryFromView( szAppName, vWork, zAPPL_NAME,
   //                           zsizeof( szAppName ) );
      sprintf_s( szAppSection, zsizeof( szAppSection ), "[App.%s]", pch );

      SysReadZeidonIni( -1, szAppSection, "ExecutableRemoteImageStrip", szTempString, zsizeof( szTempString ) );
      if ( szTempString[ 0 ] == 0 )
      {
         SysReadZeidonIni( -1, "[Workstation]", "ExecutableRemoteImageStrip", szTempString, zsizeof( szTempString ) );
      }

      if ( szTempString[ 0 ] )
         SetAttributeFromString( vTgtRemote, "App", "ImageURL", szTempString );

      SysReadZeidonIni( -1, szAppSection, "ExecutableRemoteImage", szTempString, zsizeof( szTempString ) );
      if ( szTempString[ 0 ] == 0 )
      {
         SysReadZeidonIni( -1, "[Workstation]", "ExecutableRemoteImage", szTempString, zsizeof( szTempString ) );
      }

      if ( szTempString[ 0 ] )
         SetAttributeFromString( vTgtRemote, "App", "Image", szTempString );

      SysReadZeidonIni( -1, szAppSection, "DLTextEditUnits", szTempString, zsizeof( szTempString ) );
      if ( szTempString[ 0 ] )
      {
         g_dDLTextEditUnits = atof( szTempString );
         TraceLine( "Using DLTextEditUnits %g for application: %s", g_dDLTextEditUnits, pch );
      }
   }

   // Only set the DLL if there are Operations in the Dialog.
   if ( SetCursorFirstEntity( vSrcDialog, "Operation", "Dialog" ) >= zCURSOR_SET )
   {
      SetAttributeFromAttribute( vTgtDialog, "Dlg", "DLL", vSrcDialog, "Dialog", "DLL_Name" );
   }
// else
// {
//    SetAttributeFromString( vTgtDialog, "Dlg", "DLL", "" );
//    SetAttributeFromString( vTgtRemote, "App", "DLL", "" );
// }

   CreateViewFromViewForTask( &g_vTgtDialogOrig, vTgtDialog, 0 );
   CreateViewFromViewForTask( &g_vTgtRemoteOrig, vTgtRemote, 0 );
   GetAddrForAttribute( &pch, vSrcDialog, "Dialog", "Desc" );
   pch = zstrstr( pch, "(MDI)" );
   bMDI = pch ? TRUE : FALSE;

   // Embed default window into executable's Dialog entity.
   if ( CheckExistenceOfEntity( vSrcDialog, "DfltWnd" ) == 0 )
   {
      GetStringFromAttribute( szDfltWnd, zsizeof( szDfltWnd ), vSrcDialog, "DfltWnd", "Tag" );
      SetAttributeFromString( vTgtDialog, "Dlg", "DfltWnd", szDfltWnd );

      // For the XRA, set this attribute on the Window entity.
   // SetAttributeFromString( vTgtRemote, "App", "DfltWnd", szDfltWnd );
   }
   else
      szDfltWnd[ 0 ] = 0;

   // Process each Window entity.
// nRC = CheckExistenceOfEntity( vSrcDialog, "Window" );
   nRC = SetCursorFirstEntity( vSrcDialog, "Window", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      g_rectRelSizeX.top = 9999;
      g_rectRelSizeX.left = 9999;
      g_rectRelSizeX.bottom = -9999;
      g_rectRelSizeX.right = -9999;
      g_rectRelSizeY.top = 9999;
      g_rectRelSizeY.left = 9999;
      g_rectRelSizeY.bottom = -9999;
      g_rectRelSizeY.right = -9999;
      g_nRelSizeCntX = 0;
      g_nRelSizeCntY = 0;
      GetStringFromAttribute( szRemoteWndTag + nRemoteDlgTagLth, zsizeof( szRemoteWndTag ) - nRemoteDlgTagLth, vSrcDialog, "Window", "Tag" );
#ifdef DEBUG_ALL
      TraceLineS( "(tzcmcvto) Processing Window ====> ", szRemoteWndTag + nRemoteDlgTagLth );
#endif
      if ( zstrcmp( szRemoteWndTag + nRemoteDlgTagLth, "ePammsPortal" ) == 0 )
         TraceLineS( "Converting: ", "ePammsPortal" );  // looking for Top Menu

      if ( bMDI && zstrcmp( pch, szDfltWnd ) == 0 )
      {
         ulWndType |= zWNDTYPE_MDI;
         bMDI = FALSE;
      }
      else
      {
         ulWndType = zWNDTYPE_INIT;
      }

      SetAttributeFromInteger( vSrcDialog, "Window",
                               "BarID", zZEIDON_FIRST_TOOLBAR );
      ulWndOper = 0;
      CreateEntity( vTgtDialog, "Wnd", zPOS_AFTER );
      SetViewFromView( g_vTgtDialogOrig, vTgtDialog );
      CreateEntity( vTgtRemote, "Wnd", zPOS_AFTER );
      SetViewFromView( g_vTgtRemoteOrig, vTgtRemote );

      if ( zstrcmp( szRemoteWndTag + nRemoteDlgTagLth, szDfltWnd ) == 0 )
         SetAttributeFromString( vTgtRemote, "Wnd", "DfltWnd", "Y" );

      if ( CheckExistenceOfEntity( vSrcDialog, "DfltMenu" ) == zCURSOR_SET )
      {
         // Get the name of the default menu and store it for later use.
         GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vSrcDialog, "DfltMenu", "Tag" );
//       TraceLineS( "(tzcmcvto) Processing DfltMenu ", szDfltMenu );
         SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
      //xSetAttributeFromString( vTgtRemote, "Wnd", "DfltMenu", szDfltMenu );
         if ( szReusableMainSideWindow[ 0 ] == 'Y' && CheckExistenceOfEntity( vSrcDialog, "ReusableSideWindow" ) == 0 )
         {

            // Add ReusableSideWindow menu to DfltMenu.
            GetIntegerFromAttribute( &lZKey, vSrcDialog, "Dialog", "ZKey" );
            GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideDialog", "ZKey" );
            if ( lZKey == lZKeyTmp )
            {
               CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
               CreateViewFromViewForTask( &vActSrc, vSrcDialog, 0 );
               SetNameForView( vAct, "__Convert_Act", vSubtask, zLEVEL_TASK );
               SetNameForView( vActSrc, "__Convert_ActSrc", vSubtask, zLEVEL_TASK );

               // It's in this dialog so clone actions (for the correct window) tied to an option.
               GetIntegerFromAttribute( &lZKey, vSrcDialog, "Window", "ZKey" );
               GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideWindow", "ZKey" );
               if ( lZKey != lZKeyTmp && SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
               {
                  fnCloneActions( vAct, vActSrc, vSubtask, "", 's' );
                  fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, TRUE, 's' );
               // SetCursorFirstEntityByString( vActSrc, "Menu", "Tag", szDfltMenu, "" );
               // fnCloneActions( vAct, vActSrc, vSubtask, "", 0 );
               // fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, TRUE, 0 );
                  if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
                  {
                     // Get the name of the default menu and store it for later use.
                     GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                     SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
                  }

               // SysMessageBox( vSubtask, "Check Menu", "ReusableSideWindow", 0 );
               }

               DropView( vActSrc );
               DropView( vAct );
            }
            else
            {
               // Activate the ReusableSideWindow dialog and add the actions and menu to this dialog.
               CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
               ActivateMetaOI_ByZKey( vSubtask, &vActSrc, 0, zREFER_DIALOG_META, zSINGLE, lZKeyTmp, 0 );
               GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideWindow", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
               {
                  zCHAR szDLL_Name[ 32 ];

                  GetStringFromAttribute( szDLL_Name, zsizeof( szDLL_Name ), vActSrc, "Dialog", "DLL_Name" );
                  fnCloneActions( vAct, vActSrc, vSubtask, szDLL_Name, 't' );
                  fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, TRUE, 't' );
                  if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
                  {
                     // Get the name of the default menu and store it for later use.
                     GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                     SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
                  }
               }

               DropView( vActSrc );
               DropView( vAct );
            }
         }
      }
      else
      {
         szDfltMenu[ 0 ] = 0;

         // If there is no default menu (that's why we're here) and there is
         // a reusable side menu, we want to make the side menu into the default
         // menu and add its options/actions to this dialog ... so here we go.
         if ( szReusableMainSideWindow[ 0 ] == 'Y' && CheckExistenceOfEntity( vSrcDialog, "ReusableSideWindow" ) == 0 )
         {
            GetIntegerFromAttribute( &lZKey, vSrcDialog, "Dialog", "ZKey" );
            GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideDialog", "ZKey" );
            if ( lZKey == lZKeyTmp )
            {
               CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
               CreateViewFromViewForTask( &vActSrc, vSrcDialog, 0 );
               SetNameForView( vAct, "__Convert_Act", vSubtask, zLEVEL_TASK );
               SetNameForView( vActSrc, "__Convert_ActSrc", vSubtask, zLEVEL_TASK );

               // It's in this dialog so clone actions (for the correct window) tied to an option.
               GetIntegerFromAttribute( &lZKey, vSrcDialog, "Window", "ZKey" );
               GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideWindow", "ZKey" );
               if ( lZKey != lZKeyTmp && SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
               {
                  fnCloneActions( vAct, vActSrc, vSubtask, "", 't' );
                  fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, FALSE, 't' );
                  if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
                  {
                     // Get the name of the default menu and store it for later use.
                     GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                     SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
                  }

               // SysMessageBox( vSubtask, "Check Menu", "ReusableSideWindow", 0 );
               }

               DropView( vActSrc );
               DropView( vAct );
            }
            else
            {
               // Activate the ReusableSideWindow dialog and add the actions and menu to this dialog.
               CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
               ActivateMetaOI_ByZKey( vSubtask, &vActSrc, 0, zREFER_DIALOG_META, zSINGLE, lZKeyTmp, 0 );
               GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableSideWindow", "ZKey" );
               if ( SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
               {
                  zCHAR szDLL_Name[ 32 ];

                  GetStringFromAttribute( szDLL_Name, zsizeof( szDLL_Name ), vActSrc, "Dialog", "DLL_Name" );
                  fnCloneActions( vAct, vActSrc, vSubtask, szDLL_Name, 's' );
                  fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, FALSE, 's' );
                  if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
                  {
                     // Get the name of the default menu and store it for later use.
                     GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                     SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
                  }
               }

               DropView( vActSrc );
               DropView( vAct );
            }
         }
      }

      if ( szReusableMainSideWindow[ 0 ] == 'Y' && CheckExistenceOfEntity( vSrcDialog, "ReusableMainWindow" ) == 0 )
      {
         zBOOL bDefaultMenu;

         if ( szDfltMenu[ 0 ] == 0 )
            bDefaultMenu = FALSE;
         else
            bDefaultMenu = TRUE;

         // If there is a reusable top menu, we want to add or make the top menu into the
         // default menu and add its options/actions to this dialog ... so here we go.
         GetIntegerFromAttribute( &lZKey, vSrcDialog, "Dialog", "ZKey" );
         GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableMainDialog", "ZKey" );
         if ( lZKey == lZKeyTmp )
         {
            CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
            CreateViewFromViewForTask( &vActSrc, vSrcDialog, 0 );
            SetNameForView( vAct, "__Convert_Act", vSubtask, zLEVEL_TASK );
            SetNameForView( vActSrc, "__Convert_ActSrc", vSubtask, zLEVEL_TASK );

            // It's in this dialog so clone actions (for the correct window) tied to an option.
            GetIntegerFromAttribute( &lZKey, vSrcDialog, "Window", "ZKey" );
            GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableMainWindow", "ZKey" );
            if ( lZKey != lZKeyTmp &&
                 SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
            {
               fnCloneActions( vAct, vActSrc, vSubtask, "", 't' );
               fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, bDefaultMenu, 't' );
               if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
               {
                  // Get the name of the default menu and store it for later use.
                  GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                  SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
               }

            // SysMessageBox( vSubtask, "Check Menu", "ReusableMainWindow", 0 );
            }

            DropView( vActSrc );
            DropView( vAct );
         }
         else
         {
            // Activate the ReusableSideWindow dialog and add the actions and menu to this dialog.
            CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
            ActivateMetaOI_ByZKey( vSubtask, &vActSrc, 0, zREFER_DIALOG_META, zSINGLE, lZKeyTmp, 0 );
            GetIntegerFromAttribute( &lZKeyTmp, vSrcDialog, "ReusableMainWindow", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vActSrc, "Window", "ZKey", lZKeyTmp, "" ) == 0 )
            {
               zCHAR szDLL_Name[ 32 ];

               GetStringFromAttribute( szDLL_Name, zsizeof( szDLL_Name ), vActSrc, "Dialog", "DLL_Name" );
               fnCloneActions( vAct, vActSrc, vSubtask, szDLL_Name, 's' );
               fnCloneMenuAndOptions( vAct, vActSrc, vSubtask, bDefaultMenu, 's' );
               if ( CheckExistenceOfEntity( vAct, "DfltMenu" ) > zCURSOR_UNCHANGED )
               {
                  // Get the name of the default menu and store it for later use.
                  GetStringFromAttribute( szDfltMenu, zsizeof( szDfltMenu ), vAct, "DfltMenu", "Tag" );
                  SetAttributeFromString( vTgtDialog, "Wnd", "DfltMenu", szDfltMenu );
               }
            }

            DropView( vActSrc );
            DropView( vAct );
         }
      }

      // Actions are done prior to windows/controls/options so that include
      // processing for actions can be done by the windows/controls/options.
      g_nActCnt = 0;
   // nRC = CheckExistenceOfEntity( vSrcDialog, "Action" );
      nRC = SetCursorFirstEntity( vSrcDialog, "Action", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vSrcDialog, "Action", "ZKey" );
         GetAddrForAttribute( &pch, vSrcDialog, "Action", "Tag" );
         if ( zstrchr( pch, ' ' ) != 0 )
         {
            sprintf_s( szTempString, zsizeof( szTempString ),
                       "(tzcmcvto) Processing Action: \"%s.%s\"  ZKey: %ld  contains spaces",
                       szRemoteWndTag, pch, lZKey );
            TraceLineS( szTempString, "" );
            MessageSend( vSubtask, "CM00816", "Configuration Management",
                         "Action tag contains spaces",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }

         CreateEntity( vTgtDialog, "Act", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "Act", vSrcDialog, "Action", zSET_ALL );
         if ( CheckExistenceOfEntity( vSrcDialog, "ActOper" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtDialog, "Act", "DO", vSrcDialog, "ActOper", "Name" );
         }

         SetAttributeFromAttribute( vTgtDialog, "Act", "DlgN", vSrcDialog, "Action", "DialogName" );
         SetAttributeFromAttribute( vTgtDialog, "Act", "WndN", vSrcDialog, "Action", "WindowName" );
         SetAttributeFromAttribute( vTgtDialog, "Act", "Popup", vSrcDialog, "Action", "PopupMenuName" );
         GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Action", "SB_ActionDisabling" );
         if ( ul )
         {
            ulWndOper |= zWND_EVENT_ACTION_AUTOGRAY;
            SetAttributeFromInteger( vTgtDialog, "Act", "AutoGray", ul );
         }

         GetIntegerFromAttribute( (zPLONG) &ulActSB, vSrcDialog, "Action", "SB_SubAction" );

         GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Action", "SB_NextPrev" );
         ulActSB |= ul;
         GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Action", "SB_DeleteNull" );
         ulActSB |= ul;

#if 0  // do not set NEXT by default anymore ... must be explicitly specified
         if ( ulActSB & zTEMPORAL_SUBOBJECT_DELETE ||
              ulActSB & zTEMPORAL_SUBOBJECT_EXCLUDE )
         {
            if ( !(ulActSB & zTEMPORAL_SUBOBJECT_NEXT ||
                   ulActSB & zTEMPORAL_SUBOBJECT_PREV) )
            {
               ulActSB |= zTEMPORAL_SUBOBJECT_NEXT;
            }
         }
#endif
         SetAttributeFromInteger( vTgtDialog, "Act", "SB", ulActSB );
         SetAttributeFromAttribute( vTgtDialog, "Act", "Prompt", vSrcDialog, "Action", "ActionTypeAssociatedText" );
         if ( CheckExistenceOfEntity( vSrcDialog, "ActMap" ) == 0 )
         {
            if ( CheckExistenceOfEntity( vSrcDialog, "ActMapView" ) == 0 )
               SetAttributeFromAttribute( vTgtDialog, "Act", "VN", vSrcDialog, "ActMapView", "Name" );
//x         else
//x            TraceLineS( "(tzcmcvto) ActMap entity without ActMapView entity", "" );

            if ( CheckExistenceOfEntity( vSrcDialog, "ActMapLOD_Entity" ) == 0 )
               SetAttributeFromAttribute( vTgtDialog, "Act", "EN",
                                          vSrcDialog, "ActMapLOD_Entity",
                                          "Name" );
//x         else
//x            TraceLineS( "(tzcmcvto) ActMap entity without ActMapLOD_Entity entity", "" );

            if ( SetCursorNextEntity( vSrcDialog, "ActMap", 0 ) > zCURSOR_UNCHANGED )
            {
               if ( CheckExistenceOfEntity( vSrcDialog, "ActMapLOD_Entity" ) == 0 )
                  SetAttributeFromAttribute( vTgtDialog, "Act", "Scope", vSrcDialog, "ActMapLOD_Entity", "Name" );
//x            else
//x               TraceLineS( "(tzcmcvto) ActMap entity without ActMapLOD_Entity entity", "" );

               SetCursorFirstEntity( vSrcDialog, "ActMap", 0 );
            }
         }

         SetAttributeFromInteger( vTgtDialog, "Act", "SyncIdx", g_nActCnt++ );

         nRC = SetCursorNextEntity( vSrcDialog, "Action", 0 );
      }

#ifdef DEBUG_ALL
      TraceLineS( "After Action creation =======> ",
                  szRemoteWndTag + nRemoteDlgTagLth );
      DisplayObjectInstance( vTgtDialog, "Wnd", 0 );
#endif

      SetAttributeFromInteger( vTgtDialog, "Wnd", "ActCnt", g_nActCnt );
   //xSetAttributeFromInteger( vTgtRemote, "Wnd", "ActCnt", g_nActCnt );
      SetMatchingAttributesByName( vTgtDialog, "Wnd", vSrcDialog, "Window", zSET_ALL );
      SetMatchingAttributesByName( vTgtRemote, "Wnd", vSrcDialog, "Window", zSET_ALL );
      SetAttributeFromString( vTgtRemote, "Wnd", "Tag", szRemoteWndTag );

      GetIntegerFromAttribute( &lSize, vTgtRemote, "Wnd", "SZDLG_X" );
      SetAttributeFromInteger( vTgtRemote, "Wnd", "SZDLG_X", LOWORD( lSize ) );
      GetIntegerFromAttribute( &lSize, vTgtRemote, "Wnd", "SZDLG_Y" );
      SetAttributeFromInteger( vTgtRemote, "Wnd", "SZDLG_Y", LOWORD( lSize ) );

      // Process NLS entries for Window.Caption.
      // Create an NLS entity if there are NLS entries for Caption, along
      // with the subordinate NLS_Text entries.
      SetAttributeFromAttribute( vTgtDialog, "Wnd", "Caption", vSrcDialog, "Window", "Caption" );
      nRC = SetCursorFirstEntity( vSrcDialog, "CaptionNLS_Text", "" );
      if ( nRC >= zCURSOR_SET )
      {
         SetCursorLastEntity( vTgtDialog, "NLS", "" );
         CreateEntity( vTgtDialog, "NLS", zPOS_AFTER );
         lNLS = GetRelativeEntityNumber( vTgtDialog, "NLS", 0, 0L );
         SetAttributeFromInteger( vTgtDialog, "Wnd", "NLS", lNLS );

         while ( nRC >= zCURSOR_SET )
         {
            CreateEntity( vTgtDialog, "NLS_Text", zPOS_AFTER );
            SetAttributeFromAttribute( vTgtDialog, "NLS_Text", "LI", vSrcDialog, "CaptionNLS_Text", "LanguageIndex" );
            SetAttributeFromAttribute( vTgtDialog, "NLS_Text", "Text", vSrcDialog, "CaptionNLS_Text", "Text" );
            nRC = SetCursorNextEntity( vSrcDialog, "CaptionNLS_Text", "" );
         }
      }

      // Get the name of the default button and store it for later use.
      GetStringFromAttribute( szDfltBtn, zsizeof( szDfltBtn ), vSrcDialog, "Window", "DfltButton" );
      SetAttributeFromString( vTgtDialog, "Wnd", "DfltBtn", szDfltBtn );
   //xSetAttributeFromString( vTgtRemote, "Wnd", "DfltBtn", szDfltBtn );

      SetAttributeFromAttribute( vTgtDialog, "Wnd", "ResID", vSrcDialog, "Window", "ResourceID" );

      if ( SetCursorFirstEntity( vSrcDialog, "WndStyle", 0 ) == zCURSOR_SET )
         GetAddrForAttribute( &pch, vSrcDialog, "WndStyle", "Tag" );
      else
      {
         MessageSend( vSubtask, "CM00803", "Configuration Management",
                      "Style not set for window",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         pch = "Dialog Box";
      }

      if ( SetCursorFirstEntityByString( vPE, "WindowStyle", "Tag", pch, 0 ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute( vTgtDialog, "Wnd", "Style", vPE, "WindowStyle", "Style" );
         SetAttributeFromAttribute( vTgtRemote, "Wnd", "Style", vPE, "WindowStyle", "Style" );
         SetAttributeFromAttribute( vTgtDialog, "Wnd", "StyleEx", vPE, "WindowStyle", "StyleEx" );
      }
      else
      {
         TraceLineS( "(tzcmcvto) Invalid style name - ", pch );
         TraceLineS( "(tzcmcvto) Processing incomplete", "" );
         MessageSend( vSubtask, "CM00804", "Configuration Management",
                      "Invalid Style name for window",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         DropObjectInstance( vTgtDialog );

         g_lpHotkey = (zPCHAR) SfUnlockTaskMemory( g_lpHotkey );
         g_lpEventAct = (zPCHAR) SfUnlockTaskMemory( g_lpEventAct );
         SfFreeTaskMemory( g_lpHotkey );
         SfFreeTaskMemory( g_lpEventAct );
         DropObjectInstance( vSrcDialog );
         SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
         return( 0 );
      }

      // Activate the presentation environment.
      if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
      {
         RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
         if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", "" ) < zCURSOR_SET )
         {
            MessageSend( vSubtask, "CM00805", "Configuration Management",
                         "Presentation Environment does not exist!\n"
                         "Painter exiting.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            // SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
            g_lpHotkey = (zPCHAR) SfUnlockTaskMemory( g_lpHotkey );
            g_lpEventAct = (zPCHAR) SfUnlockTaskMemory( g_lpEventAct );
            SfFreeTaskMemory( g_lpHotkey );
            SfFreeTaskMemory( g_lpEventAct );
            DropObjectInstance( vSrcDialog );
            SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
            return( 0 );
         }
         else
         {
            ActivateMetaOI( vSubtask, &vPE, vMetaList, zREFER_PENV_META, zSINGLE );
            SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
            DropView( vMetaList );
         }
      }

#if 0
   // nRC = CheckExistenceOfEntity( vSrcDialog, "WndClrOverride" );
      nRC = SetCursorFirstEntity( vSrcDialog, "WndClrOverride", "" );
      if ( nRC >= zCURSOR_SET )
      {
         // Position on the correct WindowStyle as included by this
         // SrcDialog control.
         SetCursorFirstEntityByEntityCsr( vPE, "WindowStyle",
                                          vSrcDialog, "WndStyle", "" );

         // position on the Windows Default color to include as needed
         // in the SrcDialog (when the source dialog and the PE do not
         // match up ... i.e.  the SrcDialog is missing a ColorDef)
         SetCursorFirstEntityByString( vPE, "Color", "Tag",
                                       "Windows Default", "" );
         // If any WndColorDefs have been deleted from the PE, then make
         // sure we delete them from the SrcDialog..
         nRC = SetCursorFirstEntity( vSrcDialog, "WndClrOverride", "" );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey, vSrcDialog,
                                     "WndColorDef", "ZKey" );
            nRC = SetCursorFirstEntityByInteger( vPE, "WndColorDef",
                                                 "ZKey", lZKey, 0 );
         // if ( SetEntityCursor( vPE, "WndColorDef", 0,
         //                       zPOS_FIRST | zQUAL_ENTITYCSR,
         //                       vSrcDialog, "WndColorDef", 0,
         //                       0, 0, 0 ) < 0 )
            if ( nRC < zCURSOR_SET )
            {
               DeleteEntity( vSrcDialog, "WndClrOverride", zREPOS_NONE );
            }

            nRC = SetCursorNextEntity( vSrcDialog, "WndClrOverride", "" );
         }

         // If any WndColorDefs have been added to the PE, then make sure
         // we add them to the SrcDialog.
         nRC = SetCursorFirstEntity( vPE, "WndColorDef", "" );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey, vPE, "WndColorDef", "ZKey" );
            nRC = SetCursorFirstEntityByInteger( vSrcDialog, "WndColorDef",
                                                 "ZKey", lZKey, "Window" );
            if ( nRC < zCURSOR_SET )
            {
               // Get the position of the PE WndColorDef and create the
               // Dialogs WndColorDef in the same relative position.
               nRC = (zSHORT) GetRelativeEntityNumber( vPE, "WndColorDef",
                                                       0, 0L );

               // If nRC is 0 then the PE WndColorDef is the first entity
               // so the Dialog's WndClrOverride needs to be created before
               // its first WndClrOverride.
               if ( nRC == 0 )
               {
                  SetCursorFirstEntity( vSrcDialog, "WndClrOverride", 0 );
                  CreateMetaEntity( vSubtask, vSrcDialog, "WndClrOverride",
                                    zPOS_BEFORE );
               }
               else
               {
                  // Position the Dialog on the WndClrOverride at relative
                  // position nRC - 1.  Because relative positions start at
                  // 0, you don't think you need to do the minus 1 but you
                  // do.
                  SetEntityCursor( vSrcDialog, "WndClrOverride", 0,
                                   zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                                   (zSHORT) (nRC - 1), 0, 0 );
                  CreateMetaEntity( vSubtask, vSrcDialog, "WndClrOverride", zPOS_AFTER );
               }

               IncludeSubobjectFromSubobject( vSrcDialog, "WndColorDef",
                                              vPE, "WndColorDef", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vSrcDialog, "WndColor",
                                              vPE, "Color", zPOS_AFTER );
            }

            nRC = SetCursorNextEntity( vPE, "WndColorDef", "" );
         }

         nRC = SetCursorNextEntity( vSrcDialog, "WndClrOverride", "" );
      }

      // Loop thru all SrcDialog ColorDefs to create the color stuff on the
      // TgtDialog.
      nRC = SetCursorFirstEntity( vSrcDialog, "WndClrOverride", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "WndColor", zPOS_AFTER );

         // If the WndColor is zCOLOR_DEFINITION_DFLT then check with the
         // PE instance to find what color the zeidon default is and then
         // set it.
         if ( CompareAttributeToInteger( vSrcDialog, "WndColor", "RGB_Color",
                                         zCOLOR_DEFINITION_DFLT ) == 0 )
         {
            // Position on the correct WndColorDef in the PE.
            SetCursorFirstEntityByEntityCsr( vPE, "WndColorDef",
                                             vSrcDialog, "WndColorDef",
                                             "WindowStyle" );
            vWork = vPE;
         }
         else
            vWork = vSrcDialog;

         SetAttributeFromAttribute( vTgtDialog, "WndColor", "RGB",
                                    vWork, "WndColor", "RGB_Color" );

         nRC = SetCursorNextEntity( vSrcDialog, "WndClrOverride", 0 );
      }
#endif

      CreateViewFromViewForTask( &vAct, vSrcDialog, 0 );
      g_usHotkeyLth = 0;  // offset into the hotkey table
      bESC = FALSE;
   // nRC = CheckExistenceOfEntity( vSrcDialog, "Hotkey" );
      nRC = SetCursorFirstEntity( vSrcDialog, "Hotkey", 0 );
      g_lpAT = (LPACCEL_TABLE) g_lpHotkey;
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "Hotkey", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "Hotkey", vSrcDialog, "Hotkey", zSET_ALL );
         if ( CheckExistenceOfEntity( vSrcDialog, "HotAct" ) == 0 )
         {
            ulLth = sizeof( zACCEL_TABLE );
            g_lpAT->chType = zACCEL_ACTION;
            GetBlobFromAttribute( &(g_lpAT->ac), &ulLth, vSrcDialog, "Hotkey", "KeyCombo" );
            SetEntityCursor( vAct, "Action", 0, zPOS_FIRST | zQUAL_ENTITYCSR, vSrcDialog, "HotAct", 0, 0, 0, 0 );
            g_lpAT->nID = (zSHORT) GetRelativeEntityNumber( vAct, "Action", 0, 0L );
            g_usHotkeyLth += sizeof( zACCEL_TABLE );

            GetAddrForAttribute( &pch, vSrcDialog, "HotAct", "Tag" );
            if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act", "Tag", pch, 0 ) == zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vTgtDialog, "HotAct", g_vTgtDialogOrig, "Act", zPOS_AFTER );
               CreateEntity( vTgtRemote, "Hotkey", zPOS_AFTER );
               SetMatchingAttributesByName( vTgtRemote, "Hotkey", vSrcDialog, "Hotkey", zSET_ALL );
               SetAttributeFromString( vTgtRemote, "Hotkey", "ActionTag", pch );
               GetIntegerFromAttribute( (zPLONG) &ul, g_vTgtDialogOrig, "Act", "SyncIdx" );
               SetAttributeFromInteger( vTgtRemote, "Hotkey", "SyncIdx", ul + 1 );
               SetAttributeFromInteger( vTgtRemote, "Hotkey", "ID", g_lpAT->nID );
               SetAttributeFromString( vTgtRemote, "Hotkey", "Type", "A" );
               if ( g_lpAT->ac.fCtrl == 1 )
                  SetAttributeFromString( vTgtRemote, "Hotkey", "Ctrl", "Y" );

               if ( g_lpAT->ac.fAlt == 1 )
                  SetAttributeFromString( vTgtRemote, "Hotkey", "Alt", "Y" );

               if ( g_lpAT->ac.fShift == 1 )
                  SetAttributeFromString( vTgtRemote, "Hotkey", "Shift", "Y" );

               GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Hotkey", "VKey" );
               if ( ul == VK_ESCAPE && g_lpAT->ac.fCtrl == 0 && g_lpAT->ac.fAlt == 0 && g_lpAT->ac.fShift == 0 )
               {
                  bESC = TRUE;
               }
            }

            g_lpAT++;
         }

         nRC = SetCursorNextEntity( vSrcDialog, "Hotkey", 0 );
      }

      if ( bESC == FALSE )
      {
         CreateEntity( vTgtRemote, "Hotkey", zPOS_AFTER );
         SetAttributeFromString( vTgtRemote, "Hotkey", "Tag", "ESC" );
         SetAttributeFromString( vTgtRemote, "Hotkey", "ActionTag", "ESC" );
         SetAttributeFromString( vTgtRemote, "Hotkey", "Type", "A" );
         SetAttributeFromInteger( vTgtRemote, "Hotkey", "SyncIdx", -1 );
         SetAttributeFromInteger( vTgtRemote, "Hotkey", "VKey", VK_ESCAPE );
      }

      DropView( vAct );
      if ( g_usHotkeyLth )
         SetAttributeFromBlob( vTgtDialog, "Wnd", "Hotkey", g_lpHotkey, g_usHotkeyLth );

      bSystemClose = FALSE;
   // nRC = CheckExistenceOfEntity( vSrcDialog, "WndEvent" );
      nRC = SetCursorFirstEntity( vSrcDialog, "WndEvent", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "WndEvent", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "WndEvent", vSrcDialog, "WndEvent", zSET_ALL );

         if ( CheckExistenceOfEntity( vSrcDialog, "WndAct" ) >= zCURSOR_SET )
         {
            GetAddrForAttribute( &pch, vSrcDialog, "WndAct", "Tag" );
            if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act", "Tag", pch, 0 ) == zCURSOR_SET )
            {
               GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "WndEvent", "Type" );
               IncludeSubobjectFromSubobject( vTgtDialog, "WndAct", g_vTgtDialogOrig, "Act", zPOS_AFTER );

               // Some events must not be transferred to the LightDriver.
               if ( ul != zWND_EVENT_PRE_BUILD && ul != zWND_EVENT_POST_BUILD && ul != zWND_EVENT_ON_RETURN )  // REFRESH?
               {
                  CreateEntity( vTgtRemote, "WndEvent", zPOS_AFTER );
                  SetAttributeFromString( vTgtRemote, "WndEvent", "ActionTag", pch );
                  if ( ul == zWND_EVENT_CLOSE )
                     bSystemClose = TRUE;

                  SetAttributeFromInteger( vTgtRemote, "WndEvent", "Tag", ul );
                  GetIntegerFromAttribute( (zPLONG) &ul, g_vTgtDialogOrig, "Act", "SyncIdx" );
                  SetAttributeFromInteger( vTgtRemote, "WndEvent", "SyncIdx", ul + 1 );
               }
            }
            else
            {
               MessageSend( vSubtask, "CM00806", "Configuration Management",
                            "Wnd cannot locate action for include",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               TraceLineS( "(tzcmcvto) Window cannot locate action for include ", pch );
            }
         }

         GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "WndEvent", "Type" );
         ulWndOper |= ul;

         nRC = SetCursorNextEntity( vSrcDialog, "WndEvent", 0 );
      }

      if ( bSystemClose == FALSE )
      {
         CreateEntity( vTgtRemote, "WndEvent", zPOS_AFTER );
         SetAttributeFromInteger( vTgtRemote, "WndEvent", "Tag", zWND_EVENT_CLOSE );
         SetAttributeFromString( vTgtRemote, "WndEvent", "ActionTag", "Alt+F4" );
         SetAttributeFromInteger( vTgtRemote, "WndEvent", "SyncIdx", -1 );
      }

      // Set up "special" window events for the RemoteServer.
      nRC = SetCursorFirstEntity( vPE, "EventDef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetAddrForAttribute( &pch, vPE, "EventDef", "RegWEB" );
         if ( *pch == 'Y' || *pch == 'y' )
         {
            GetIntegerFromAttribute( (zPLONG) &ul, vPE, "EventDef", "Type" );
            if ( SetCursorFirstEntityByInteger( vSrcDialog, "WndEvent", "Type", ul, 0 ) != zCURSOR_SET )
            {
               CreateEntity( vTgtRemote, "WndEvent", zPOS_AFTER );
               SetAttributeFromInteger( vTgtRemote, "WndEvent", "SyncIdx", -1 );
               SetAttributeFromInteger( vTgtRemote, "WndEvent", "Tag", ul );
               strcpy_s( szTempString, zsizeof( szTempString ), szRemoteWndTag + nRemoteDlgTagLth );
               k = (zSHORT) zstrlen( szTempString );
               szTempString[ k++ ] = ';';
               zltoa( ul, szTempString + k, zsizeof( szTempString ) - k );
               SetAttributeFromString( vTgtRemote, "WndEvent", "ActionTag", szTempString );
            }
         }

         nRC = SetCursorNextEntity( vPE, "EventDef", 0 );
      }

      GetAddrForAttribute( &pch, vSrcDialog, "Window", "DIL_Active" );
      g_chDIL_Active = pch[ 0 ];
      if ( g_chDIL_Active )
      {
         ulWndOper |= 0x80000000;  // zWND_EVENT_DIL_ACTIVE;
         if ( CheckExistenceOfEntity( vTgtDialog, "DIL" ) == zCURSOR_NULL )
         {
            CreateEntity( vTgtDialog, "DIL", zPOS_AFTER );
         // SetAttributeFromString( vTgtDialog, "DIL", "Msg", "" );
         //xCreateEntity( vTgtRemote, "MB", zPOS_AFTER );
         // SetAttributeFromString( vTgtRemote, "MB", "Msg", "" );
         }
      }

      SetAttributeFromBlob( vTgtDialog, "Wnd", "OperCtrl", &ulWndOper, sizeof( zLONG ) );

   // nRC = CheckExistenceOfEntity( vSrcDialog, "Menu" );
      nRC = SetCursorFirstEntity( vSrcDialog, "Menu", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         g_nOptCnt = 0;
         GetAddrForAttribute( &pch, vSrcDialog, "Menu", "Tag" );
//       TraceLineS( "(tzcmcvto) Processing Menu ", pch );
         CreateEntity( vTgtDialog, "Menu", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "Menu", vSrcDialog, "Menu", zSET_ALL );
         CreateEntity( vTgtRemote, "Menubar", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtRemote, "Menubar", vSrcDialog, "Menu", zSET_ALL );

         if ( zstrcmp( pch, szDfltMenu ) == 0 )
         {
            // Flag this menu as the default menu.
            SetAttributeFromString( vTgtRemote, "Menubar", "DfltMenu", "Y" );
         }

         // This is a recursive operation to process all options within
         // this menu.
         CreateViewFromViewForTask( &vTgtWndDialog, vTgtDialog, 0 );
         CreateViewFromViewForTask( &vTgtWndRemote, vTgtRemote, 0 );
         fnProcessOptions( vSubtask, vTgtWndDialog, vTgtWndRemote,
                           vTgtDialog, vTgtRemote, vSrcDialog );
         DropView( vTgtWndDialog );
         DropView( vTgtWndRemote );
         SetAttributeFromInteger( vTgtDialog, "Menu", "OptCnt", g_nOptCnt );
         nRC = SetCursorNextEntity( vSrcDialog, "Menu", 0 );
      }

      g_nCtrlRelTop = g_nCtrlRelLeft = 9999;
      g_nCtrlRelBottom = g_nCtrlRelRight = -9999;
      g_nCtrlCnt = g_nCtrlID = 0;

   // if ( zstrcmp( "Test", szRemoteWndTag + nRemoteDlgTagLth ) == 0 )
   //    TraceLineS( "", "" );

      if ( CheckExistenceOfEntity( vSrcDialog, "Control" ) > zCURSOR_UNCHANGED )
      {
         lExtentX = 0;
         lExtentY = 0;
         GetIntegerFromAttribute( &lFlags, vSrcDialog, "Window", "GeneralFlag" );

         CreateViewFromViewForTask( &vSrcWndDialog, vSrcDialog, 0 );
         CreateViewFromViewForTask( &vTgtWndDialog, vTgtDialog, 0 );
         CreateViewFromViewForTask( &vTgtWndRemote, vTgtRemote, 0 );

         // This is a recursive routine.
         fnProcessControlsAtLevel( vSubtask, vTgtWndDialog, vTgtWndRemote,
                                   vTgtDialog, vTgtRemote,
                                   vSrcWndDialog, vSrcDialog, vPE, 0, 0,
                                   &lExtentX, &lExtentY, lFlags );
         if ( lFlags & 0x00000001 )
         {
            SetAttributeFromInteger( vTgtWndRemote, "Wnd", "ScrollX", lExtentX );
            SetAttributeFromInteger( vTgtWndRemote, "Wnd", "ScrollY", lExtentY );
         }

         if ( g_nRelSizeCntX <= 1 && g_nRelSizeCntY <= 1 )
            ulWndType |= zWNDTYPE_SINGLE_RELSIZE;

         SetCursorFirstEntity( vTgtDialog, "Ctrl", 0 );
         SetCursorFirstEntity( vTgtRemote, "Ctrl", 0 );

         // This is a recursive routine.
         fnProcessControls( vSubtask, vTgtDialog, vTgtRemote, vPE );

         DropView( vSrcWndDialog );
         DropView( vTgtWndDialog );
         DropView( vTgtWndRemote );

         // sort target controls by id
      }

      SetAttributeFromInteger( vTgtDialog, "Wnd", "CtrlCnt", g_nCtrlCnt );
   //xSetAttributeFromInteger( vTgtRemote, "Wnd", "CtrlCnt", g_nCtrlCnt );
      if ( g_nCtrlRelTop != 9999 && g_nCtrlRelBottom != -9999 )
      {
         SetAttributeFromInteger( vTgtDialog, "Wnd", "REL_Y", zMAKELONG( g_nCtrlRelTop, g_nCtrlRelBottom ) );
      // SetAttributeFromInteger( vTgtRemote, "Wnd", "REL_Y", g_nCtrlRelTop );
      // SetAttributeFromInteger( vTgtRemote, "Wnd", "REL_CY", g_nCtrlRelBottom );
//       GetAddrForAttribute( &pch, vSrcDialog, "Window", "Tag" );
//       if ( zstrcmp( pch, "OIDisplay" ) == 0 )
//       {
//          TraceLineI( "OIDisplay RelTop: ", g_nCtrlRelTop );
//          TraceLineI( "OIDisplay RelBot: ", g_nCtrlRelBottom );
//       }
      }

      if ( g_nCtrlRelLeft != 9999 && g_nCtrlRelRight != -9999 )
      {
         SetAttributeFromInteger( vTgtDialog, "Wnd", "REL_X", zMAKELONG( g_nCtrlRelLeft, g_nCtrlRelRight ) );
      // SetAttributeFromInteger( vTgtRemote, "Wnd", "REL_X", g_nCtrlRelLeft );
      // SetAttributeFromInteger( vTgtRemote, "Wnd", "REL_CX", g_nCtrlRelRight );

//       GetAddrForAttribute( &pch, vSrcDialog, "Window", "Tag" );
//       if ( zstrcmp( pch, "OIDisplay" ) == 0 )
//       {
//          TraceLineI( "OIDisplay RelLeft : ",  g_nCtrlRelLeft );
//          TraceLineI( "OIDisplay RelRight: ", g_nCtrlRelRight );
//       }
      }

      SetAttributeFromInteger( vTgtDialog, "Wnd", "Subtype", ulWndType );

      nRC = SetCursorNextEntity( vSrcDialog, "Window", 0 );
   }

   // Name the executable so it will stay around for debugging.
   // SetNameForView( vTgtDialog, "NewExecutable", vSubtask, zLEVEL_TASK );
   // IssueError( vTgtDialog, 0, 0, "End of Convert" );

   g_lpHotkey = (zPCHAR) SfUnlockTaskMemory( g_lpHotkey );
   g_lpEventAct = (zPCHAR) SfUnlockTaskMemory( g_lpEventAct );
   SfFreeTaskMemory( g_lpHotkey );
   SfFreeTaskMemory( g_lpEventAct );

////////////////////////// CONVERT LOGIC END ////////////////////////////////

   GetAddrForAttribute( &pch, vSrcDialog, "Dialog", "Tag" );
// TraceLineS( "Returning dialog to CM - ", pch );

   SetAttributeFromString( vTgtRemote, "App", "Image", "" );
   SetAttributeFromString( vTgtRemote, "App", "ImageURL", "" );
   SetAttributeFromString( vTgtRemote, "App", "ReportURL", "" );
   *pvXRA = vTgtRemote;

   DropView( g_vTgtDialogOrig );
   DropView( g_vTgtRemoteOrig );

   hLibrary = (zLONG) SysLoadLibrary( vSubtask, "tzctl" );
   if ( hLibrary )
   {
      if ( (lpfnDuplicateCheck = (zFARPROC_DUP_CHECK) SysGetProc( (LPLIBRARY) hLibrary, "CheckDuplicateTags" )) )
      {
         nRC = (*lpfnDuplicateCheck)( vSubtask, vSrcDialog, 0 );
         if ( nRC > 0 )
         {
            sprintf_s( szTempString, zsizeof( szTempString ),
                       "(tzcmcvto) Duplicate control tags found in Dialog: %s", pch );
            TraceLineS( szTempString, "" );
            strcat_s( szTempString, zsizeof( szTempString ), "\n(see Zeidon trace for details)" );
            MessageSend( vSubtask, "CM00815", "Configuration Management",
                         szTempString, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
   }

   DropObjectInstance( vSrcDialog );
   SysMutexUnlock( vSubtask, "ZDrConvertDialog", 0 );
   return( vTgtDialog );
}

void
fnRemoveAmpersand( zPCHAR pchText )
{
   zLONG lLth = zstrlen( pchText );
   zPCHAR pch = zstrchr( pchText, '&' );
   while ( pch )
   {
      strcpy_s( pch, lLth, pch + 1 );
      pch = zstrchr( pch, '&' );
   }
}

// This is a recursive routine.
void
fnProcessOptions( zVIEW vSubtask,
                  zVIEW vTgtWndDialog,        // view to Wnd level for DIL
                  zVIEW vTgtWndRemote,
                  zVIEW vTgtDialog,
                  zVIEW vTgtRemote,
                  zVIEW vSrcDialog )
{
   zPCHAR  pch;
   zLONG   ul;
   zSHORT  nRC;
   zLONG   lNLS;
   zSHORT  nDIL_Msg;
   zUSHORT nOptionTag = 0;
   zCHAR   szTempString[ 256 ];

// nRC = CheckExistenceOfEntity( vSrcDialog, "Option" );
   nRC = SetCursorFirstEntity( vSrcDialog, "Option", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      g_nOptCnt++;
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "Option", "Tag" );
//    TraceLineS( "(tzcmcvto) Processing Option ", szTempString );
      CreateEntity( vTgtDialog, "Opt", zPOS_AFTER );
      SetMatchingAttributesByName( vTgtDialog, "Opt", vSrcDialog, "Option", zSET_ALL );

      CreateEntity( vTgtRemote, "Opt", zPOS_AFTER );
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "Option", "Separator" );
      // Is it a Separator?
      if ( szTempString[ 0 ] == 'Y' )
      {
         CreateEntity( vTgtRemote, "Separator", zPOS_AFTER );
         SetAttributeFromInteger( vTgtRemote, "Opt", "Tag", nOptionTag );
         SetAttributeFromInteger( vTgtRemote, "Opt", "IdNbr", nOptionTag );
         nOptionTag++;
         CreateEntity( vTgtRemote, "Opt", zPOS_AFTER );
      }

      SetMatchingAttributesByName( vTgtRemote, "Opt", vSrcDialog, "Option", zSET_ALL );
   // fnRemoveAmpersand( szTempString );
   // SetAttributeFromString( vTgtRemote, "Opt", "Tag", szTempString );

      // LightDriver needs an Index for each Menu item, including
      // separators and popups ... starting with 0 for each item.
      SetAttributeFromInteger( vTgtRemote, "Opt", "IdNbr", nOptionTag );
      nOptionTag++;

      // Process NLS entries for Option.Text.
      // Create an NLS entity if there are NLS entries for Text, along
      // with the subordinate NLS_Text entries.
      SetAttributeFromAttribute( vTgtDialog, "Opt", "Text", vSrcDialog, "Option", "Text" );
      nRC = SetCursorFirstEntity( vSrcDialog, "OptionNLS_Text", "" );
      if ( nRC >= zCURSOR_SET )
      {
         SetCursorLastEntity( vTgtWndDialog, "NLS", "" );
         CreateEntity( vTgtWndDialog, "NLS", zPOS_AFTER );
         lNLS = GetRelativeEntityNumber( vTgtWndDialog, "NLS", 0, 0L );
         SetAttributeFromInteger( vTgtDialog, "Opt", "NLS", lNLS );

         while ( nRC >= zCURSOR_SET )
         {
            CreateEntity( vTgtWndDialog, "NLS_Text", zPOS_AFTER );
            SetAttributeFromAttribute( vTgtWndDialog, "NLS_Text", "LI",
                                       vSrcDialog, "OptionNLS_Text", "LanguageIndex" );
            SetAttributeFromAttribute( vTgtWndDialog, "NLS_Text", "Text",
                                       vSrcDialog, "OptionNLS_Text", "Text" );
            nRC = SetCursorNextEntity( vSrcDialog, "OptionNLS_Text", "" );
         }
      }

      if ( g_chDIL_Active )
      {
         // If DIL is active for the window, use the Option Text value as
         // the DIL Message.  If there are NLS entities for the DIL message,
         // add them to the DIL.  Note that we do not check if the text is
         // null, since there should not be a null Option.

         GetVariableFromAttribute( szTempString, 0, zTYPE_STRING,
                                   zsizeof( szTempString ), vSrcDialog,
                                   "Option", "Text", 0, 0 );
         fnRemoveChar( szTempString, '&', TRUE );
         if ( SetCursorFirstEntityByString( vTgtWndDialog, "DIL", "Msg",
                                            szTempString, 0 ) != zCURSOR_SET )
         {
            SetCursorLastEntity( vTgtWndDialog, "DIL", 0 );
            CreateEntity( vTgtWndDialog, "DIL", zPOS_AFTER );
            SetAttributeFromString( vTgtWndDialog, "DIL", "Msg", szTempString );
         //xSetCursorLastEntity( vTgtWndRemote, "MB", 0 );
         //xCreateEntity( vTgtWndRemote, "MB", zPOS_AFTER );
         //xSetAttributeFromString( vTgtWndRemote, "MB", "Msg", szTempString );
            SetAttributeFromString( vTgtRemote, "Opt", "DIL", szTempString );

            // Process any NLS_Text messages.
            nRC = SetCursorFirstEntity( vSrcDialog, "OptionNLS_Text", "" );
            while ( nRC >= zCURSOR_SET )
            {
               CreateEntity( vTgtWndDialog, "NLS_DIL_Text", zPOS_AFTER );
               SetAttributeFromAttribute( vTgtWndDialog, "NLS_DIL_Text", "LI",
                                          vSrcDialog, "OptionNLS_Text", "LanguageIndex" );
               SetAttributeFromAttribute( vTgtWndDialog, "NLS_DIL_Text", "Text",
                                          vSrcDialog, "OptionNLS_Text", "Text" );
               nRC = SetCursorNextEntity( vSrcDialog, "OptionNLS_Text", "" );
            }
         }

         nDIL_Msg = (zSHORT) GetRelativeEntityNumber( vTgtWndDialog, "DIL",  0, 0L );
      // SetAttributeFromInteger( vTgtRemote, "Opt", "DIL", nDIL_Msg );
         SetAttributeFromInteger( vTgtDialog, "Opt", "DIL", nDIL_Msg );
      }

      GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Option", "Underline" );
      if ( ul )
      {
         zCHAR  szTempString[ 256 ];

         nRC = (zSHORT) ul - 1;
         GetStringFromAttribute( szTempString + 1, zsizeof( szTempString ) - 1, vSrcDialog, "Option", "Text" );
         strncpy_s( szTempString, zsizeof( szTempString ), szTempString + 1, nRC );
         szTempString[ nRC ] = zMNEMONIC_FLAG;
         szTempString[ nRC + 1 ] = 0;
         SetAttributeFromString( vTgtDialog, "Opt", "Text", szTempString );

         // Java likes to have the Position of the mnemonic.
         SetAttributeFromInteger( vTgtRemote, "Opt", "Mnemonic", ul );
      }

      if ( CheckExistenceOfEntity( vSrcDialog, "OptAct" ) > zCURSOR_UNCHANGED )
      {
         GetAddrForAttribute( &pch, vSrcDialog, "OptAct", "Tag" );
//       TraceLineS( "(tzcmcvto) Processing OptAct ", pch );
         if ( pch[ 0 ] )
         {
            if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act", "Tag",
                                               pch, 0 ) == zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vTgtDialog, "OptAct",
                                              g_vTgtDialogOrig, "Act",
                                              zPOS_AFTER );
               SetAttributeFromString( vTgtRemote, "Opt", "ActionTag", pch );
               GetIntegerFromAttribute( (zPLONG) &ul, g_vTgtDialogOrig, "Act", "SyncIdx" );
               SetAttributeFromInteger( vTgtRemote, "Opt", "SyncIdx", ul + 1 );
            }
            else
            {
               MessageSend( vSubtask, "CM00807", "Configuration Management",
                            "Opt cannot locate action for include",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               TraceLineS( "(tzcmcvto) Option cannot locate action for include ", pch );
            }
         }
      }

      if ( CheckExistenceOfEntity( vSrcDialog, "OptMap" ) > zCURSOR_UNCHANGED )
      {
         GetAddrForAttribute( &pch, vSrcDialog, "OptMap", "Tag" );
         CreateEntity( vTgtDialog, "OptMap", zPOS_AFTER );

         // Now reset the target values from the relationship values.
         if ( CheckExistenceOfEntity( vSrcDialog, "OptMapView" ) == 0 )
            SetAttributeFromAttribute( vTgtDialog, "OptMap", "VN",
                                       vSrcDialog, "OptMapView", "Name" );

         if ( CheckExistenceOfEntity( vSrcDialog, "OptMapLOD_Attribute" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtDialog, "OptMap", "EN",
                                       vSrcDialog, "OptMapRelatedEntity", "Name" );
            SetAttributeFromAttribute( vTgtDialog, "OptMap", "AN",
                                       vSrcDialog, "OptMapER_Attribute", "Name" );
         }

         if ( CheckExistenceOfEntity( vSrcDialog, "OptMapContext" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtDialog, "OptMap", "Context",
                                       vSrcDialog, "OptMapContext", "Name" );
         }

         SetAttributeFromAttribute( vTgtDialog, "Opt", "ChkOn", vSrcDialog, "Option", "CheckMarkValueOn" );
         SetAttributeFromAttribute( vTgtDialog, "Opt", "ChkOff", vSrcDialog, "Option", "CheckMarkValueOff" );
      }

      if ( CheckExistenceOfEntity( vSrcDialog, "OptOpt" ) > zCURSOR_UNCHANGED )
      {
         // If the Opt has children, then make it a Menu
         CreateEntity( vTgtRemote, "Menu", zPOS_AFTER );

         if ( SetViewToSubobject( vSrcDialog, "OptOpt" ) >= 0 )
         {
            SetViewToSubobject( vTgtDialog, "OptOpt" );
            SetViewToSubobject( vTgtRemote, "OptOpt" );
            fnProcessOptions( vSubtask, vTgtWndDialog, vTgtWndRemote, vTgtDialog, vTgtRemote, vSrcDialog );
            ResetViewFromSubobject( vSrcDialog );
            ResetViewFromSubobject( vTgtRemote );
            ResetViewFromSubobject( vTgtDialog );
         }
      }

      nRC = SetCursorNextEntity( vSrcDialog, "Option", 0 );
   }
}

// This is a recursive routine to set PE style information in the target
// controls.
void
fnProcessControls( zVIEW vSubtask, zVIEW vTgtDialog, zVIEW vTgtRemote, zVIEW vPE )
{
   zSHORT nRC;

// nRC = CheckExistenceOfEntity( vTgtDialog, "Ctrl" );
   nRC = SetCursorFirstEntity( vTgtDialog, "Ctrl", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      g_nCtrlCnt++;

#if FUTURE
      GetAddrForAttribute( &pch, vTgtDialog, "Ctrl", "Style" );

      // there needs to be a test here to determine if the Control ID
      // that we are about to set has already been allotted to another
      // control within this window ??? ... a future enhancement
      if ( pch[ 0 ] )
      {
         if ( SetCursorFirstEntityByString( vPE, "Style", "Tag",
                                            pch, 0 ) == zCURSOR_SET )
         {
            SetAttributeFromAttribute( vTgtDialog, "Ctrl", "IdNbr",
                                       vPE, "Style", "IdNbr" );
            SetAttributeFromAttribute( vTgtDialog, "Window", "Text",
                                       vPE, "Style", "Text" );
         }
         else
         {
            TraceLineS( "(tzcmcvto) Invalid style name - ", pch );
            TraceLineS( "(tzcmcvto) Processing incomplete", "" );
            MessageSend( vSubtask, "CM00808", "Configuration Management",
                         "Invalid Style name for control",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
      else
#endif       // if FUTURE
//??? {
//???    g_nCtrlID++;
//???    while ( SetCursorForEntity( Control with ID == g_nCtrlID ) == FOUND )
//???       g_nCtrlID++;
//???
//???    SetAttributeFromInteger( vTgtDialog, "Ctrl", "IdNbr", g_nCtrlID );
//??? }

      if ( SetViewToSubobject( vTgtDialog, "CtrlCtrl" ) >= 0 )
      {
      // SetViewToSubobject( vTgtRemote, "CtrlCtrl" );
         fnProcessControls( vSubtask, vTgtDialog, vTgtRemote, vPE );
      // ResetViewFromSubobject( vTgtRemote );
         ResetViewFromSubobject( vTgtDialog );
      }

      nRC = SetCursorNextEntity( vTgtDialog, "Ctrl", 0 );
   }
}

#define zTEXT_CONTROL                       1000L
#define zEDIT_CONTROL                       1010L
#define zCOMBOBOX_CONTROL                   1050L

void
fnCountRelCtrls( zVIEW vCtrl, RECT *rect, zULONG ulSubtype,
                 zPSHORT pnRelSizeCntX, zPSHORT pnRelSizeTotalX,
                 zPSHORT pnRelSizeCntY, zPSHORT pnRelSizeTotalY )
{
   zULONG ul;

// zLONG  lKey = GetEntityKey( vCtrl, "Control" );
   zCHAR  szTag[ 33 ] = { 0 };
   zLONG  lZKeyIn;
   zLONG  lZKeyChk;
   zLONG  lTop;
   zLONG  lLeft;
   zLONG  lBottom;
   zLONG  lRight;
   zLONG  lTopLast    = rect->top; // was -1;
   zLONG  lLeftLast   = rect->left; // was -1;
   zLONG  lBottomLast = rect->bottom - rect->top; // actually SZDLG_Y was -1;
   zLONG  lRightLast  = rect->right - rect->left; // actually SZDLG_X was -1;
   zLONG  lType;
   zSHORT nRC;

   *pnRelSizeCntX = 0;
   *pnRelSizeCntY = 0;
   *pnRelSizeTotalX = 0;
   *pnRelSizeTotalY = 0;
   GetIntegerFromAttribute( &lZKeyIn, vCtrl, "Control", "ZKey" );
// GetStringFromAttribute( szTag, zsizeof( szTag ), vCtrl, "Control", "Tag" );
// TraceLineS( "= Invoked fnCountRelCtrls  for: ", szTag );
   nRC = SetCursorFirstEntity( vCtrl, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( (zPLONG) &ul, vCtrl, "Control", "Subtype" );
      if ( ul & zCONTROL_RELATIVE )
      {
         GetIntegerFromAttribute( &lType, vCtrl, "ControlDef", "Key" );
         GetIntegerFromAttribute( &lZKeyChk, vCtrl, "Control", "ZKey" );
         GetStringFromAttribute( szTag, zsizeof( szTag ), vCtrl, "Control", "Tag" );
         GetIntegerFromAttribute( &lTop, vCtrl, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lLeft, vCtrl, "Control", "PSDLG_X" );
         if ( lType == zCOMBOBOX_CONTROL )
            lBottom = 12;
         else
            GetIntegerFromAttribute( &lBottom, vCtrl, "Control", "SZDLG_Y" );

         GetIntegerFromAttribute( &lRight, vCtrl, "Control", "SZDLG_X" );

         // If the controls are congruent with control being set, we will assume
         // that only one of the controls are visible at a time, so we will only
         // count the control itself for relative size/position processing.
         if ( (lTopLast != lTop || lLeftLast != lLeft ||
               lBottomLast != lBottom || lRightLast != lRight) ||
              lZKeyIn == lZKeyChk )
         {
         // if ( lTopLast == -1 && lLeftLast == -1 &&
         //      lBottomLast == -1 && lRightLast == -1 )
         // {
         //    // First relative pos/size control.
         //    lTopLast = lTop;
         //    lLeftLast = lLeft;
         //    lBottomLast = lBottom;
         //    lRightLast = lRight;
         // }

            lBottom += lTop;
            lRight += lLeft;

         //xif ( ul & zCONTROL_RELATIVE_SIZE_Y )
         //x   TraceLineS( "fnCountRelCtrls RelSizeY: ", szTag );

            if ( (ul & zCONTROL_RELATIVE_SIZE_Y) &&
                 (lLeft > rect->right || lRight < rect->left) == 0 )
            {
               (*pnRelSizeTotalY)++;
               if ( (ulSubtype & zCONTROL_RELATIVE_POS_Y) &&
                    lBottom < rect->top )
               {
                  (*pnRelSizeCntY)++;
               }
            }

         //xif ( ul & zCONTROL_RELATIVE_SIZE_X )
         //x   TraceLineS( "fnCountRelCtrls RelSizeX: ", szTag );

            if ( (ul & zCONTROL_RELATIVE_SIZE_X) &&
                 (lTop > rect->bottom || lBottom < rect->top) == 0 )
            {
               (*pnRelSizeTotalX)++;
               if ( (ulSubtype & zCONTROL_RELATIVE_POS_X) &&
                    lRight < rect->left )
               {
                  (*pnRelSizeCntX)++;
               }
            }
         }
         else
         {
         // TraceLineS( "-- ignore relative congruent control: ", szTag );
         }
      }

      nRC = SetCursorNextEntity( vCtrl, "Control", 0 );
   }
}

// This is a recursive routine to set target information in each control
// from the source control.
void
fnProcessControlsAtLevel( zVIEW   vSubtask,
                          zVIEW   vTgtWndDialog, // view to Wnd level
                          zVIEW   vTgtWndRemote,
                          zVIEW   vTgtDialog,
                          zVIEW   vTgtRemote,
                          zVIEW   vSrcWndDialog, // view to Window level
                          zVIEW   vSrcDialog,
                          zVIEW   vPE,
                          zLONG   lOffsetX,
                          zLONG   lOffsetY,
                          zPLONG  plExtentX,
                          zPLONG  plExtentY,
                          zLONG   lFlags )
{
   zVIEW   vMetaList;
   zLONG   hLibrary;
   zFARPROC_CONVERT lpfnDynamicRoutine;
   zTZWDLGSO_Control_DEF ControlDef = { 0 };
   zPCHAR  pch;
   zPCHAR  pchAttr;
   zUSHORT uLth;
   zCHAR   szTempString[ 256 ];
   zULONG  ul;
   zLONG   lCtrlType;
   zLONG   lNLS;
   zLONG   lKey;
   zLONG   lSize;
   zSHORT  nGenTag = 1000;
   zLONG   k;
// LOGFONT lf;
   zSHORT  nDIL_Msg;
   zSHORT  nRC;

   ControlDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
   ControlDef.CheckByte = 0xFE;
   nRC = SetCursorFirstEntity( vSrcDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // Get the structure of attributes for this control.
      GetStructFromEntityAttrs( (zPCHAR) &ControlDef, vSrcDialog, "Control" );
      if ( ControlDef.CheckByte != 0xFE )
         MessageBox( 0, "ControlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

   // if ( //zstrcmp( ControlDef.Tag, "EntityList" ) == 0 ||
   //      zstrncmp( ControlDef.Tag, "dks", 3 ) == 0 )
   // {
   //    TraceLineS( "(tzcmcvto) Processing Control ", ControlDef.Tag );
   // }

      CreateEntity( vTgtDialog, "Ctrl", zPOS_AFTER );
      CreateEntity( vTgtRemote, "Ctrl", zPOS_AFTER );

      // Hard code the control type in the .XWD.
      SetMatchingAttributesByName( vTgtDialog, "Ctrl", vSrcDialog, "Control", zSET_ALL );
      SetMatchingAttributesByName( vTgtRemote, "Ctrl", vSrcDialog, "Control", zSET_ALL );

      GetIntegerFromAttribute( (zPLONG) &lCtrlType, vSrcDialog, "ControlDef", "Key" );

      if ( (lCtrlType == zTEXT_CONTROL || lCtrlType == zEDIT_CONTROL) && g_dDLTextEditUnits != 0.0 )
      {
         GetIntegerFromAttribute( &lSize, vTgtDialog, "Ctrl", "SZDLG_Y" );
         lSize = (zLONG) (lSize * g_dDLTextEditUnits);
         SetAttributeFromInteger( vTgtDialog, "Ctrl", "SZDLG_Y", lSize );
      }

   // GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vSrcDialog, "Control", "Tag" );
   //xfnRemoveAmpersand( ControlDef.Tag );
   //xSetAttributeFromString( vTgtRemote, "Ctrl", "Tag", ControlDef.Tag );

      SetAttributeFromInteger( vTgtDialog, "Ctrl", "StyleX", ControlDef.ExtendedStyle );
      SetAttributeFromInteger( vTgtRemote, "Ctrl", "StyleX", ControlDef.ExtendedStyle );

      SetAttributeFromInteger( vTgtDialog, "Ctrl", "Type", lCtrlType );
      SetAttributeFromInteger( vTgtRemote, "Ctrl", "Type", lCtrlType );
      if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", lCtrlType, 0 ) == 0 )
      {
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vPE, "ControlDef", "Tag" );
         SetAttributeFromString( vTgtDialog, "Ctrl", "TagPE", szTempString );
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vPE, "ControlDef", "ActiveX_Script" );
         SetAttributeFromString( vTgtDialog, "Ctrl", "Script", szTempString );
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vPE, "ControlDef", "ActiveX_ScriptDLL" );
         SetAttributeFromString( vTgtDialog, "Ctrl", "ScriptDLL", szTempString );
      }
      else
      {
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "ControlDef", "ActiveX_Script" );
         SetAttributeFromString( vTgtDialog, "Ctrl", "Script", szTempString );
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "ControlDef", "ActiveX_ScriptDLL" );
         SetAttributeFromString( vTgtDialog, "Ctrl", "ScriptDLL", szTempString );
      }

      // Process NLS entries for Control.Text.
      // Create an NLS entity if there are NLS entries for Text, along
      // with the subordinate NLS_Text entries.
      SetAttributeFromString( vTgtDialog, "Ctrl", "Text", ControlDef.Text );
      nRC = SetCursorFirstEntity( vSrcDialog, "ControlNLS_Text", "" );
      if ( nRC >= zCURSOR_SET )
      {
         SetCursorLastEntity( vTgtWndDialog, "NLS", "" );
         CreateEntity( vTgtWndDialog, "NLS", zPOS_AFTER );
         lNLS = GetRelativeEntityNumber( vTgtWndDialog, "NLS", 0, 0L );
         SetAttributeFromInteger( vTgtDialog, "Ctrl", "NLS", lNLS );

         while ( nRC >= zCURSOR_SET )
         {
            CreateEntity( vTgtWndDialog, "NLS_Text", zPOS_AFTER );
            SetAttributeFromAttribute( vTgtWndDialog, "NLS_Text", "LI", vSrcDialog, "ControlNLS_Text", "LanguageIndex" );
            SetAttributeFromAttribute( vTgtWndDialog, "NLS_Text", "Text", vSrcDialog, "ControlNLS_Text", "Text" );
            nRC = SetCursorNextEntity( vSrcDialog, "ControlNLS_Text", "" );
         }
      }

      // If the control is tagless generate a tag.
      if ( ControlDef.Tag[ 0 ] == 0 )
      {
         nGenTag++;    // nGenTag started at 1000
         zltoa( nGenTag, szTempString, zsizeof( szTempString ) );
         szTempString[ 0 ] = '~';
         SetAttributeFromString( vTgtDialog, "Ctrl", "Tag", szTempString );
         SetAttributeFromString( vTgtRemote, "Ctrl", "Tag", szTempString );
      }

      g_nCtrlID++;
      SetAttributeFromInteger( vTgtDialog, "Ctrl", "IdNbr", g_nCtrlID );
      SetAttributeFromInteger( vTgtRemote, "Ctrl", "IdNbr", g_nCtrlID );

      // We want the top-most/left-most positions of any fixed-position/
      // relative-size controls.  We also want the bottom-most/right-most
      // extents of any relative-size controls, or the bottom-most/
      // right-most top-left positions of any relative-position controls ...
      // whichever is bottom-most/right-most
      ul = ControlDef.Subtype;
   // GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Control", "Subtype" );
      if ( ul & zCONTROL_RELATIVE )
      {
         zVIEW  vCtrl;
         RECT   rect;
         zLONG  lType;
         zSHORT nRelSizeCntX;
         zSHORT nRelSizeCntY;
         zSHORT nRelSizeTotalX;
         zSHORT nRelSizeTotalY;

      // if ( (ul & zCONTROL_RELATIVE_POS_X) ||
      //      (ul & zCONTROL_RELATIVE_POS_Y) )
         {
            GetIntegerFromAttribute( &lType, vSrcDialog, "ControlDef", "Key" );
            rect.top = ControlDef.PSDLG_Y;
            rect.left = ControlDef.PSDLG_X;
            if ( lType == zCOMBOBOX_CONTROL )
               rect.bottom = 12 + rect.top;
            else
               rect.bottom = ControlDef.SZDLG_Y + rect.top;

            rect.right = ControlDef.SZDLG_X + rect.left;

            CreateViewFromViewForTask( &vCtrl, vSrcDialog, 0 );
            fnCountRelCtrls( vCtrl, &rect, ul, &nRelSizeCntX, &nRelSizeTotalX, &nRelSizeCntY, &nRelSizeTotalY );
         // TraceLineS( "Checked relative control: ", ControlDef.Tag );
         // TraceLineI( "     nRelSizeCntX: ", nRelSizeCntX );
         // TraceLineI( "     nRelSizeCntY: ", nRelSizeCntY );
         // TraceLineI( "   nRelSizeTotalX: ", nRelSizeTotalX );
         // TraceLineI( "   nRelSizeTotalY: ", nRelSizeTotalY );
            DropView( vCtrl );
         // if ( zstrcmp( "AddMapEntity", ControlDef.Tag ) == 0 )
         //    TraceLineS( "", "" );

            SetAttributeFromInteger( vTgtDialog, "Ctrl", "RELCNT_X", zMAKELONG( nRelSizeCntX, nRelSizeTotalX ) );
            SetAttributeFromInteger( vTgtDialog, "Ctrl", "RELCNT_Y", zMAKELONG( nRelSizeCntY, nRelSizeTotalY ) );
            if ( nRelSizeCntX == 0 && nRelSizeTotalX == 1 )
               nRelSizeCntX = 1;

            if ( nRelSizeCntY == 0 && nRelSizeTotalY == 1 )
               nRelSizeCntY = 1;
         }
      // else
      // {
      //    nRelSizeCntX = nRelSizeCntY = nRelSizeTotalX = nRelSizeTotalY = 0;
      // }

//       if ( (ControlDef.Tag[ 0 ] == 'A' && ControlDef.Tag[ 1 ] == 't' &&
//             ControlDef.Tag[ 2 ] == 't' && ControlDef.Tag[ 3 ] == 'r' &&
//             ControlDef.Tag[ 4 ] == 'i' && ControlDef.Tag[ 5 ] == 'b') ||
//            (ControlDef.Tag[ 0 ] == 'H' && ControlDef.Tag[ 1 ] == 'i' &&
//             ControlDef.Tag[ 2 ] == 'e' && ControlDef.Tag[ 3 ] == 'r' &&
//             ControlDef.Tag[ 4 ] == 'D' && ControlDef.Tag[ 5 ] == 'i') )
//       {
//          TraceLineS( "Checking control: ", ControlDef.Tag );
//          TraceLineI( "   PSDLG_X: ", ControlDef.PSDLG_X );
//          TraceLineI( "   PSDLG_Y: ", ControlDef.PSDLG_Y );
//          TraceLineI( "   SZDLG_X: ", ControlDef.SZDLG_X );
//          TraceLineI( "   SZDLG_Y: ", ControlDef.SZDLG_Y );
//          TraceLineI( "   g_nCtrlRelTop: ", g_nCtrlRelTop );
//          TraceLineI( "   g_nCtrlRelBottom: ", g_nCtrlRelBottom );
//       }

         if ( ul & (zCONTROL_RELATIVE_SIZE_X | zCONTROL_RELATIVE_POS_X) )
         {
            if ( g_nRelSizeCntX < nRelSizeCntX )
               g_nRelSizeCntX = nRelSizeCntX;

            if ( g_nCtrlRelLeft > ControlDef.PSDLG_X + lOffsetX )
            {
               g_nCtrlRelLeft = (zSHORT) (ControlDef.PSDLG_X + lOffsetX);
            }

            if ( g_nCtrlRelRight < ControlDef.PSDLG_X + lOffsetX + ControlDef.SZDLG_X )
            {
               g_nCtrlRelRight = (zSHORT) (ControlDef.PSDLG_X + lOffsetX + ControlDef.SZDLG_X);
            }
         }

         if ( ul & (zCONTROL_RELATIVE_SIZE_Y | zCONTROL_RELATIVE_POS_Y) )
         {
            if ( g_nRelSizeCntY < nRelSizeCntY )
               g_nRelSizeCntY = nRelSizeCntY;

            if ( g_nCtrlRelTop > ControlDef.PSDLG_Y + lOffsetY )
            {
               g_nCtrlRelTop = (zSHORT) (ControlDef.PSDLG_Y + lOffsetY);
            }

            if ( g_nCtrlRelBottom < ControlDef.PSDLG_Y + lOffsetY + ControlDef.SZDLG_Y )
            {
               g_nCtrlRelBottom = (zSHORT) (ControlDef.PSDLG_Y + lOffsetY + ControlDef.SZDLG_Y);
            }
         }

//       if ( (ControlDef.Tag[ 0 ] == 'A' && ControlDef.Tag[ 1 ] == 't' &&
//             ControlDef.Tag[ 2 ] == 't' && ControlDef.Tag[ 3 ] == 'r' &&
//             ControlDef.Tag[ 4 ] == 'i' && ControlDef.Tag[ 5 ] == 'b') ||
//            (ControlDef.Tag[ 0 ] == 'H' && ControlDef.Tag[ 1 ] == 'i' &&
//             ControlDef.Tag[ 2 ] == 'e' && ControlDef.Tag[ 3 ] == 'r' &&
//             ControlDef.Tag[ 4 ] == 'D' && ControlDef.Tag[ 5 ] == 'i') )
//       {
//          TraceLineI( "   g_nCtrlRelTop: ", g_nCtrlRelTop );
//          TraceLineI( "   g_nCtrlRelBottom: ", g_nCtrlRelBottom );
//       }
      }

      if ( ul & zCONTROL_INVISIBLE )
         SetAttributeFromString( vTgtRemote, "Ctrl", "Visible", "N" );

      if ( ul & zCONTROL_DISABLED )
         SetAttributeFromString( vTgtRemote, "Ctrl", "Enabled", "N" );

      ul = ControlDef.ExtendedStyle;
      if ( ul & zCONTROLX_DISABLE_READONLY )
         SetAttributeFromString( vTgtRemote, "Ctrl", "ReadOnly", "Y" );

      if ( lFlags & 0x00000001 )
      {
         if ( ControlDef.PSDLG_X + lOffsetX + ControlDef.SZDLG_X > *plExtentX )
         {
            *plExtentX = ControlDef.PSDLG_X + lOffsetX + ControlDef.SZDLG_X;
         }

         if ( ControlDef.PSDLG_Y + lOffsetY + ControlDef.SZDLG_Y > *plExtentY )
         {
            *plExtentY = ControlDef.PSDLG_Y + lOffsetY + ControlDef.SZDLG_Y;
         }
      }

      // If DIL messages are active, get the source DIL message from
      // the source dialog, add it as a DIL entity in the target dialog and
      // set its relative cursor position in the target control.
      // DIL Messages are created in a hierarchical order as follows.
      // 1. If there is a DIL_Text value in the Control, then that is used.
      //    In this case we also process any NLS DIL entries.
      // 2. If there is Control Mapping, we use the DIL_Inherited value
      //    from the Control entity.
      // 3. If the Control is a Radio or Push button, we use the Text value
      //    from the Control entity.
      // 4. Otherwise, the value is null.

      if ( g_chDIL_Active )
      {
         GetVariableFromAttribute( szTempString, 0, zTYPE_STRING, zsizeof( szTempString ), vSrcDialog, "Control", "DIL_Text", 0, 0 );
         if ( szTempString[ 0 ] )
         {
            // Case 1 above, DIL_Text.
         }
         else
         {
            if ( CheckExistenceOfEntity( vSrcDialog, "CtrlMapER_Domain" ) >= zCURSOR_SET )
            {
               // Case 2 above, DIL_Inherited.
               GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "Control", "DIL_Inherited" );
            }
            else
            {
               GetAddrForAttribute( &pchAttr, vSrcDialog, "ControlDef", "Tag" );
               if ( zstrcmp( pchAttr, "Radio" ) == 0 || zstrcmp( pchAttr, "Push" ) == 0 )
               {
                  //Case 3 above, Text.
                  GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "Control", "Text" );
                  fnRemoveChar( szTempString, '&', TRUE );
               }
               else
                  // Otherwise the value is null.
                  szTempString[ 0 ] = 0;
            }
         }

         if ( SetCursorFirstEntityByString( vTgtWndDialog, "DIL", "Msg", szTempString, 0 ) != zCURSOR_SET )
         {
            SetCursorLastEntity( vTgtWndDialog, "DIL", 0 );
            CreateEntity( vTgtWndDialog, "DIL", zPOS_AFTER );
            SetAttributeFromString( vTgtWndDialog, "DIL", "Msg", szTempString );
         //xSetCursorLastEntity( vTgtWndRemote, "MB", 0 );
         //xCreateEntity( vTgtWndRemote, "MB", zPOS_AFTER );
         //xSetAttributeFromString( vTgtWndRemote, "MB", "Msg",
         //x                        szTempString );
            if ( szTempString[ 0 ] )
               SetAttributeFromString( vTgtRemote, "Ctrl", "DIL", szTempString );

            // Process any NLS_DIL messages.
            nRC = SetCursorFirstEntity( vSrcDialog, "ControlNLS_DIL_Text", "" );
            while ( nRC >= zCURSOR_SET )
            {
               CreateEntity( vTgtWndDialog, "NLS_DIL_Text", zPOS_AFTER );
               SetAttributeFromAttribute( vTgtWndDialog, "NLS_DIL_Text", "LI",
                                          vSrcDialog, "ControlNLS_DIL_Text", "LanguageIndex" );
               SetAttributeFromAttribute( vTgtWndDialog, "NLS_DIL_Text", "Text",
                                          vSrcDialog, "ControlNLS_DIL_Text", "Text" );
               nRC = SetCursorNextEntity( vSrcDialog, "ControlNLS_DIL_Text", "" );
            }
         }

         nDIL_Msg = (zSHORT) GetRelativeEntityNumber( vTgtWndDialog, "DIL", 0, 0L );
      // SetAttributeFromInteger( vTgtRemote, "Ctrl", "DIL", nDIL_Msg );
         SetAttributeFromInteger( vTgtDialog, "Ctrl", "DIL", nDIL_Msg );
      }
      else
      {
         // If no DIL requested for window, so indicate.
         SetAttributeFromInteger( vTgtDialog, "Ctrl", "DIL", 0 );
      // SetAttributeFromInteger( vTgtRemote, "Ctrl", "DIL", 0 );
      }
      // End of DIL processing

      GetIntegerFromAttribute( (zPLONG) &ul, vSrcDialog, "Control", "Underline" );
      if ( ul )
      {
         nRC = (zSHORT) ul - 1;
         GetStringFromAttribute( szTempString + 1, zsizeof( szTempString ) - 1, vSrcDialog, "Control", "Text" );
         strncpy_s( szTempString, zsizeof( szTempString ), szTempString + 1, nRC );
         szTempString[ nRC ] = zMNEMONIC_FLAG;
         szTempString[ nRC + 1 ] = 0;
         SetAttributeFromString( vTgtDialog, "Ctrl", "Text", szTempString );
         SetAttributeFromString( vTgtRemote, "Ctrl", "Text", szTempString );
      }

      // get drag and drop information and create a blob from it for the
      // executable version of the dialog.  The blob has a format of:
      //
      //    4-byte flag - S (source), T (target), B (both) ... only 1 byte used
      //    sizeof( zLONG ) - length of first key (source or target)
      //    first key string
      //    null terminator
      //    second key string (if both source and target)
      //    second null terminator
      //
      k = 4 + sizeof( zLONG ); // 4-byte flag + key length
      GetStringFromAttribute( szTempString + k, zsizeof( szTempString ) - k, vSrcDialog, "Control", "DD_SourceKey" );
      if ( szTempString[ k ] )
      {
         zSHORT j;
         zPLONG pn = (zPLONG) (szTempString + sizeof( zLONG ));
         k = zstrlen( szTempString + k ) + 1;
         if ( (j = (k % sizeof( zLONG ))) != 0 )
         {
            k += (sizeof( zLONG ) - j);      // (multiple of 4)
         }

         *pn = k;       // just the length of the source key + null term
         k += 4 + sizeof( zLONG );
      }

      GetStringFromAttribute( szTempString + k, zsizeof( szTempString ) - k, vSrcDialog, "Control", "DD_TargetKey" );
      if ( (k > (4 + sizeof( zLONG ))) && *(szTempString + k) )
      {
         k += zstrlen( szTempString + k ) + 1;  // include null terminator
         szTempString[ 0 ] = 'B';      // both source and target Drag & Drop
      }
      else
      if ( k > (4 + sizeof( zLONG )) )
      {
         szTempString[ 0 ] = 'S';      // source Drag & Drop only
      }
      else
      if ( szTempString[ k ] )
      {
         zPLONG pn = (zPLONG) (szTempString + sizeof( zLONG ));
         k = zstrlen( szTempString + k ) + 1;   // include null terminator
         *pn = k;       // just the length of the target key + null term
         szTempString[ 0 ] = 'T';               // target drag & drop only
         k += 4 + sizeof( zLONG );
      }
      else
      {
//       szTempString[ 0 ] = 'N';            // no drag & drop
         k = 0;
      }

      if ( k )
      {
         szTempString[ 1 ] = ' ';
         szTempString[ 2 ] = ' ';
         szTempString[ 3 ] = ' ';
         SetAttributeFromBlob( vTgtDialog, "Ctrl", "DD_BOI", szTempString, k );
      }

#if 1
      uLth = 0;
   // nRC = CheckExistenceOfEntity( vSrcDialog, "Event" );
      nRC = SetCursorFirstEntity( vSrcDialog, "Event", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "Event", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "Event", vSrcDialog, "Event", zSET_ALL );

         if ( CheckExistenceOfEntity( vSrcDialog, "EventAct" ) >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lKey, vSrcDialog, "EventAct", "ZKey" );
         // if ( lKey == 88038468 )
         //    TraceLineS( "", "" );

            if ( SetCursorFirstEntityByInteger( vSrcWndDialog, "Action",
                                                "ZKey", lKey, 0 ) == zCURSOR_SET )
            {
               if ( CompareAttributeToAttribute( vSrcDialog,
                                                 "EventAct", "Tag",
                                                 vSrcWndDialog,
                                                 "Action", "Tag" ) != 0 )
               {
#ifdef DEBUG_ALL
                  GetAddrForAttribute( &pch, vSrcDialog, "EventAct", "Tag" );
                  GetAddrForAttribute( &pchAttr, vSrcWndDialog, "Action", "Tag" );
                  zsprintf( szTempString, "Event Tag mismatch: %s <====> %s", pch, pchAttr );
                  DisplayEntityInstance( vSrcWndDialog, "Window" );
#endif
               // if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act",
               //                                    "Tag", pch, 0 ) == zCURSOR_SET )
               // {
               //    GetAddrForAttribute( &pch, vSrcWndDialog, "Action", "Tag" );
               //    SetAttributeFromString( g_vTgtDialogOrig, "Act", "Tag", pch );
               // }
               //
                  ExcludeEntity( vSrcDialog, "EventAct", zREPOS_NONE );
                  IncludeSubobjectFromSubobject( vSrcDialog, "EventAct", vSrcWndDialog, "Action", zPOS_AFTER );
               }
            }

            // Note that this assumes there must be an action for an event???
            GetAddrForAttribute( &pch, vSrcDialog, "EventAct", "Tag" );
//          TraceLineS( "(tzcmcvto) Processing EventAct ", pch );
            if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act", "Tag",
                                               pch, 0 ) == zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vTgtDialog, "EventAct",
                                              g_vTgtDialogOrig, "Act",
                                              zPOS_AFTER );

               CreateEntity( vTgtRemote, "Event", zPOS_AFTER );
               SetAttributeFromString( vTgtRemote, "Event", "ActionTag", pch );
               SetAttributeFromAttribute( vTgtRemote, "Event", "Tag", vSrcDialog, "Event", "Type" );
               GetIntegerFromAttribute( (zPLONG) &ul, g_vTgtDialogOrig, "Act", "SyncIdx" );
               SetAttributeFromInteger( vTgtRemote, "Event", "SyncIdx", ul + 1 );

               // Load up a table with all the events for this control.
               GetIntegerFromAttribute( (zPLONG) (g_lpEventAct + uLth), vTgtDialog, "Event", "Type" );
               uLth += sizeof( zLONG );
               GetIntegerFromAttribute( (zPLONG) (g_lpEventAct + uLth), vTgtDialog, "EventAct", "SyncIdx" );

            // GetAddrForAttribute( &pch, vTgtDialog, "EventAct", "Tag" );
            // TraceLineI( pch, *((zPLONG) (g_lpEventAct + uLth)) );

               uLth += sizeof( zLONG );

            // GetStringFromAttribute( g_lpEventAct + uLth, vSrcDialog, "EventAct", "Tag" );
            // uLth += zstrlen( g_lpEventAct + uLth ) + 1;
            }
            else
            {
               GetAddrForAttribute( &pchAttr, vSrcWndDialog, "Window", "Tag" );
               sprintf_s( szTempString, zsizeof( szTempString ),
                          "Control (%s) cannot locate\n"
                          "Action (%s) with ZKey (%ld)\n"
                          "for include in Window (%s)",
                          ControlDef.Tag, pch, lKey, pchAttr );

            // DisplayObjectInstance( g_vTgtDialogOrig, "Wnd", 0 );
               MessageSend( vSubtask, "CM00809", "Configuration Management",
                            szTempString,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               TraceLineS( "(tzcmcvto) ", szTempString );
            }
         }

         nRC = SetCursorNextEntity( vSrcDialog, "Event", 0 );
      }

      if ( uLth )
      {
         *((zPLONG) (g_lpEventAct + uLth)) = -1L;    // long terminator
         uLth += sizeof( zLONG ); // room for extra terminator long
         SetAttributeFromBlob( vTgtDialog, "Ctrl", "EventAct",
                               g_lpEventAct, uLth );
      }

      // Set up "special" control events for the RemoteServer.
      nRC = SetCursorFirstEntity( vPE, "CtrlEventDef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetAddrForAttribute( &pch, vPE, "CtrlEventDef", "RegWEB" );
         if ( *pch == 'Y' || *pch == 'A' )
         {
            GetIntegerFromAttribute( (zPLONG) &ul, vPE, "CtrlEventDef", "Type" );
            nRC = SetCursorFirstEntityByInteger( vSrcDialog, "Event",
                                                 "Type", ul, 0 );
            if ( nRC == zCURSOR_SET && *pch == 'A' )
            {
               nRC = SetCursorFirstEntityByInteger( vTgtDialog, "Event",
                                                    "Type", ul, 0 );
               if ( nRC == zCURSOR_SET )
               {
                  DeleteEntity( vTgtDialog, "Event", zREPOS_PREV );
                  nRC = zCURSOR_SET - 1;
               }
            }

            if ( nRC != zCURSOR_SET )
            {
               CreateEntity( vTgtRemote, "Event", zPOS_AFTER );
               SetAttributeFromInteger( vTgtRemote, "Event", "SyncIdx", -1 );
               SetAttributeFromInteger( vTgtRemote, "Event", "Tag", ul );
               GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, "Control", "Tag" );
               k = zstrlen( szTempString );
               szTempString[ k++ ] = ';';
               zltoa( ul, szTempString + k, zsizeof( szTempString ) - k );
               SetAttributeFromString( vTgtRemote, "Event", "ActionTag", szTempString );
            }
         }

         nRC = SetCursorNextEntity( vPE, "CtrlEventDef", 0 );
      }


#else
   // nRC = CheckExistenceOfEntity( vSrcDialog, "Event" );
      nRC = SetCursorFirstEntity( vSrcDialog, "Event", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "Event", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtDialog, "Event", vSrcDialog, "Event", zSET_ALL );

         // note that this assumes there must be an action for an event???
         GetAddrForAttribute( &pch, vSrcDialog, "EventAct", "Tag" );
//       TraceLineS( "(tzcmcvto) Processing EventAct ", pch );
         if ( SetCursorFirstEntityByString( g_vTgtDialogOrig, "Act", "Tag",
                                            pch, 0 ) == zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vTgtDialog, "EventAct",
                                           g_vTgtDialogOrig, "Act", zPOS_AFTER );
         }
         else
         {
            sprintf_s( szTempString, zsizeof( szTempString ),
                       "Ctrl (%s) cannot locate action (%s) for include",
                       ControlDef.Tag, pch );
            MessageSend( vSubtask, "CM00809", "Configuration Management",
                         szTempString,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            TraceLineS( "(tzcmcvto) ", szTempString );
         }

         nRC = SetCursorNextEntity( vSrcDialog, "Event", 0 );
      }
#endif

#if 0
      if ( zstrcmp( ControlDef.Tag, "ListDKS" ) == 0 )
      {
         TraceLineS( "Displaying (1) Ctrl: ", ControlDef.Tag );
         DisplayEntityInstance( vTgtDialog, "Ctrl" );
         DisplayEntityInstance( vTgtDialog, "CtrlMap" );
      }
#endif

   // nRC = CheckExistenceOfEntity( vSrcDialog, "CtrlMap" );
      nRC = SetCursorFirstEntity( vSrcDialog, "CtrlMap", 0 );
      if ( nRC >= zCURSOR_SET )
      {
         while ( nRC >= zCURSOR_SET )
         {
            CreateEntity( vTgtDialog, "CtrlMap", zPOS_AFTER );
            SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "Tag",
                                       vSrcDialog, "CtrlMap", "Tag" );
            SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "Type",
                                       vSrcDialog, "CtrlMap", "Type" );

            // Now reset the target values from the relationship values.
            if ( CheckExistenceOfEntity( vSrcDialog, "CtrlMapView" ) == 0 )
            {
               SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "VN",
                                          vSrcDialog, "CtrlMapView", "Name" );
            }

            if ( CheckExistenceOfEntity( vSrcDialog, "CtrlMapLOD_Attribute" ) == 0 )
            {
               SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "EN",
                                          vSrcDialog, "CtrlMapRelatedEntity",
                                          "Name" );
               SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "AN",
                                          vSrcDialog, "CtrlMapER_Attribute",
                                          "Name" );
            }

            if ( CheckExistenceOfEntity( vSrcDialog, "CtrlMapLOD_Entity" ) == 0 )
            {
               SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "EN",
                                          vSrcDialog, "CtrlMapLOD_Entity",
                                          "Name" );
            }

            if ( CheckExistenceOfEntity( vSrcDialog, "CtrlMapContext" ) == 0 )
            {
               SetAttributeFromAttribute( vTgtDialog, "CtrlMap", "Context",
                                          vSrcDialog, "CtrlMapContext",
                                          "Name" );
            }

            nRC = SetCursorNextEntity( vSrcDialog, "CtrlMap", 0 );
         }

         SetCursorFirstEntity( vSrcDialog, "CtrlMap", 0 );
         SetCursorFirstEntity( vTgtDialog, "CtrlMap", 0 );
      }

      // Activate the presentation environment.
      if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
      {
         RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
         if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", "" ) < zCURSOR_SET )
         {
            MessageSend( vSubtask, "CM00811", "Configuration Management",
                         "Presentation Environment does not exist!\n"
                         "Painter conversion terminating.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return;
         }
         else
         {
            ActivateMetaOI( vSubtask, &vPE, vMetaList,
                            zREFER_PENV_META, zSINGLE );
            SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
            DropView( vMetaList );
         }
      }

      SetAttributeFromInteger( vTgtDialog, "Ctrl", "ColorFg", -1 );
      SetAttributeFromInteger( vTgtDialog, "Ctrl", "ColorBk", -1 );

#if 0
      // Loop thru all SrcDialog ColorDefs to create the color stuff on the
      // TgtDialog.
      nRC = SetCursorFirstEntity( vSrcDialog, "CtrlClrOverride", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "CtrlColor", zPOS_AFTER );

         GetAddrForAttribute( &pch, vSrcDialog, "CtrlColorDef", "Tag" );
         if ( zstrcmp( pch, "Text" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtDialog, "Ctrl", "ColorFg",
                                       vSrcDialog, "CtrlColor", "RGB_Color" );
         }

         SetAttributeFromString( vTgtDialog, "CtrlColor", "Tag", pch );
         SetAttributeFromAttribute( vTgtDialog, "CtrlColor", "RGB",
                                    vSrcDialog, "CtrlColor", "RGB_Color" );

         nRC = SetCursorNextEntity( vSrcDialog, "CtrlClrOverride", 0 );
      }

      // Loop thru all SrcDialog FontDefs to create the font stuff on the
      // TgtDialog.
      uLth = sizeof( LOGFONT );
      nRC = SetCursorFirstEntity( vSrcDialog, "CtrlFontOverride", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTgtDialog, "CtrlFont", zPOS_AFTER );
         SetAttributeFromAttribute( vTgtDialog, "CtrlFont", "Tag",
                                    vSrcDialog, "CtrlFontDef", "Tag" );
         SetAttributeFromAttribute( vTgtDialog, "CtrlFont", "RGB",
                                    vSrcDialog, "CtrlFont", "RGB_Color" );
         SetAttributeFromAttribute( vTgtDialog, "CtrlFont", "Size",
                                    vSrcDialog, "CtrlFont", "PointSize" );
         GetBlobFromAttribute( &lf, 0, vSrcDialog,
                               "CtrlFont", "WindowsStructure" );
         SetAttributeFromBlob( vTgtDialog, "CtrlFont", "LFont",
                               &lf, sizeof( LOGFONT ) );

         nRC = SetCursorNextEntity( vSrcDialog, "CtrlFontOverride", 0 );
      }
#endif

      // Check the painter/conversion control registration object
      // for the conversion routine for the current control ...
      // if conversion code exists ... execute it.
      GetIntegerFromAttribute( &lKey, vSrcDialog, "ControlDef", "Key" );
      SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", lKey, 0 );
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vPE,
                              "ControlDef", "PainterDLL_Name" );
      GetStringFromAttribute( szTempString + 100, zsizeof( szTempString ) - 100, vPE,
                              "ControlDef", "PainterConvertOperationName" );
      if ( szTempString[ 100 ] )  // there is a conversion operation
      {
         // For performance reasons, the address to the COP should be loaded
         // once ... when the CCtrl object was initially instantiated ...
         // FUTURE??? implementation!!!!
         hLibrary = (zLONG) SysLoadLibrary( vSubtask, szTempString );
         if ( hLibrary )
         {
            if ( (lpfnDynamicRoutine =
                  (zFARPROC_CONVERT) SysGetProc( (LPLIBRARY) hLibrary,
                                                 szTempString + 100 )) == 0 )
            {
               // ERROR !!!
               strcpy_s( szTempString, zsizeof( szTempString ), "(tzcmcvto) Cannot access Ctrl convert operation: " );
               strcat_s( szTempString, zsizeof( szTempString ), szTempString + 100 );
               MessageSend( vSubtask, "CM00813", "Configuration Management",
                            szTempString + 11,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               strcat_s( szTempString, zsizeof( szTempString ), " for token " );
               TraceLineI( szTempString, lCtrlType );
            }
            else
            {
               // If sub-controls will be part of driver controls ... they
               // must be given a unique id (increment g_nCtrlID and use it).
               nRC = (*lpfnDynamicRoutine)( vSubtask, vTgtDialog,
                                            vTgtRemote,
                                            vSrcDialog, &g_nCtrlID );

               if ( zstrcmp( ControlDef.Tag, "ListDKS" ) == 0 )
               {
                  DisplayEntityInstance( vTgtDialog, "Ctrl" );
                  DisplayEntityInstance( vTgtDialog, "CtrlMap" );
               }

               // Control conversion routines return:
               //    0 - control conversion handled all sub-controls
               //   -1 - conversion should handle sub-controls
               //  -16 - control conversion encountered an error ... skip
               //        to next control
               if ( nRC <= -16 )
               {
               // zVIEW vTemp;
               // CreateViewFromViewForTask( &vTemp, vTgtDialog );
               // while ( ResetViewFromSubobject( vTemp ) == 0 )
               // {
               // }
               //
               // DisplayObjectInstance( vTemp, "Wnd", 0 );
               // DropView( vTemp );

                  DeleteEntity( vTgtDialog, "Ctrl", zREPOS_PREV );

               // CreateViewFromViewForTask( &vTemp, vTgtDialog );
               // while ( ResetViewFromSubobject( vTemp ) == 0 )
               // {
               // }
               //
               // DisplayObjectInstance( vTemp, "Wnd", 0 );
               // DropView( vTemp );
               // MessageBox( 0, "DeleteEntity", "After", MB_OK );
               }
               else
               if ( nRC < 0 )
               {
                  SetViewToSubobject( vSrcDialog, "CtrlCtrl" );
                  SetViewToSubobject( vTgtDialog, "CtrlCtrl" );
                  SetViewToSubobject( vTgtRemote, "CtrlCtrl" );
                  fnProcessControlsAtLevel( vSubtask, vTgtWndDialog, vTgtWndRemote,
                                            vTgtDialog, vTgtRemote,
                                            vSrcWndDialog, vSrcDialog, vPE,
                                            ControlDef.PSDLG_X + lOffsetX,
                                            ControlDef.PSDLG_Y + lOffsetY,
                                            plExtentX, plExtentY, lFlags );
                  ResetViewFromSubobject( vSrcDialog );
                  ResetViewFromSubobject( vTgtRemote );
                  ResetViewFromSubobject( vTgtDialog );
               }
            }
         }
         else
         {
            // ERROR !!!
            strcpy_s( szTempString + 100, zsizeof( szTempString ) - 100, szTempString );
            strcpy_s( szTempString, zsizeof( szTempString ),
                     "(tzcmcvto) Cannot load Ctrl conversion library: " );
            strcat_s( szTempString, zsizeof( szTempString ), szTempString + 100 );
            MessageSend( vSubtask, "CM00814", "Configuration Management",
                         szTempString + 7,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            strcat_s( szTempString, zsizeof( szTempString ), " for token " );
            TraceLineI( szTempString, lCtrlType );
         }
      }

      nRC = SetCursorNextEntity( vSrcDialog, "Control", 0 ); //???
   }
}

////////////////////////// CONVERT REPORT ///////////////////////////////////
////////////////////////// CONVERT REPORT ///////////////////////////////////
////////////////////////// CONVERT REPORT ///////////////////////////////////
////////////////////////// CONVERT REPORT ///////////////////////////////////

// Recursive routine to set ColumnCSV for each ctrl.
// Ctrls are ordered on RangeBottom when this is called.
zBOOL
SetCSV_CtrlColumns( zVIEW vReport, zPSHORT pnCSV_Column,
                    zLONG lRangeBottom, zLONG lOffset, zPSHORT pnColumn )
{
   zLONG  lPosX;
   zLONG  lRangeY;
   zSHORT nRC;
   zSHORT k;

   nRC = SetCursorFirstEntity( vReport, "Control", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lRangeY, vReport, "Control", "RangeBottom" );
      if ( lRangeY != lRangeBottom )
      {
         lRangeBottom = lRangeY;
         *pnColumn = 0;
      }

      GetIntegerFromAttribute( &lPosX, vReport, "Control", "PSDLG_X" );
      lPosX += lOffset;
      for ( k = *pnColumn; k < 255; k++ )
      {
         if ( lPosX >= pnCSV_Column[ k ] && lPosX < pnCSV_Column[ k + 1 ] )
         {
            *pnColumn = k;
            break; // this is the column for this ctrl
         }
         else
         if ( lPosX < pnCSV_Column[ k ] )
         {
            // Need to split ... create a new column (shorter).
            zSHORT j = 255;
            while ( j > k )
            {
               pnCSV_Column[ j - 1 ] = pnCSV_Column[ j ];
               j--;
            }

            pnCSV_Column[ k ] = (zSHORT) lPosX;
            return( FALSE );
         }
      }

      SetAttributeFromInteger( vReport, "Control", "ColumnCSV", k + 1 );

      nRC = SetCursorFirstEntity( vReport, "CtrlCtrl", "" );
      if ( nRC >= zCURSOR_SET )
      {
         while ( nRC >= zCURSOR_SET )
         {
            SetViewToSubobject( vReport, "CtrlCtrl" );
            if ( SetCSV_CtrlColumns( vReport, pnCSV_Column, lRangeBottom,
                                     lPosX, pnColumn ) == FALSE )
            {
               return( FALSE );
            }

            ResetViewFromSubobject( vReport );
            nRC = SetCursorNextEntity( vReport, "CtrlCtrl", "" );
         }
      }

      nRC = SetCursorNextEntity( vReport, "Control", "" );
   }

   return( TRUE );
}

// Ctrls are ordered on RangeBottom when this is called.
//
zBOOL
SetCSV_Columns( zVIEW   vReport,
                zPSHORT pnCSV_Column )
{
   zLONG  lRangeY;
   zSHORT nColumn;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vReport, "Page", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vReport, "GroupSet", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = SetCursorFirstEntity( vReport, "Group", "" );
         while ( nRC >= zCURSOR_SET )
         {
            if ( CheckExistenceOfEntity( vReport, "Control" ) == 0 )
            {
               nColumn = 0;
               GetIntegerFromAttribute( &lRangeY, vReport,
                                        "Control", "RangeBottom" );
               if ( SetCSV_CtrlColumns( vReport, pnCSV_Column,
                                        lRangeY, 0, &nColumn ) == FALSE )
               {
                  return( FALSE );
               }
            }

            nRC = SetCursorNextEntity( vReport, "Group", "" );
         }

         nRC = SetCursorNextEntity( vReport, "GroupSet", "" );
      }

      nRC = SetCursorNextEntity( vReport, "Page", "" );
   }

   return( TRUE );
}

zVIEW OPERATION
ConvertReport( zVIEW   vSubtask,
               zVIEW   vSrcReport,
               zCPCHAR szTgtRptName )
{
   zFARPROC_DUP_CHECK lpfnDuplicateCheck;
// zVIEW  vMetaList;
// zVIEW  vPE;
// zPCHAR pch;
// zLONG  lKey;
   zPCHAR pch;
   zVIEW  vTgtReport;
   zVIEW  vTmpReport;
   zVIEW  vSubCtrl;
   zLONG  hLibrary;
   zLONG  lPos;
   zLONG  lSize;
   zLONG  lPosY;
   zLONG  lSizeY;
   zLONG  lOffsetX;
   zLONG  lOffsetY;
   zLONG  lExtent;
   zLONG  lRangeTop;
   zLONG  lRangeBottom;
   zBOOL  bChange;
   zCHAR  szTempString[ 256 ];
   zCHAR  szAllowPageBreak[ 2 ];
   zSHORT pnCSV_Column[ 256 ];
   zSHORT k;
   zSHORT nRC;
// zLONG  lChangeCnt = 0;

//#define DEBUG_RANGE
#ifdef DEBUG_RANGE
      zCHAR szMsg[ 301 ];
      zCHAR szTag[ 101 ];
      zCHAR szPosY[ 11 ];
      zCHAR szSizeY[ 11 ];
      zCHAR szTop[ 11 ];
      zCHAR szBottom[ 11 ];
#endif

   if ( ActivateOI_FromOI_ForTask( &vTgtReport, vSrcReport, 0, 0 ) < 0 )
   {
      // ERROR out of here ...
      TraceLineS( "(tzcmcvto) Cannot activate executable Report - ",
                  "TZRPSRCO" );
      MessageSend( vSubtask, "CM00802", "Configuration Management",
                   "Cannot activate executable Report",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   else
      SetNameForView( vTgtReport, "__Convert_X", vSubtask, zLEVEL_TASK );

#if 0
   // Activate the presentation environment
   if ( (nRC = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", "" ) < zCURSOR_SET )
      {
         TraceLineS( "(tzcmcvto) Cannot get Custom Control View - ",
                     "TZPESRCO" );
         MessageSend( vSubtask, "CM00801", "Configuration Management",
                      "Error locating Presentation Environment information",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         ActivateMetaOI( vSubtask, &vPE, vMetaList, zREFER_PENV_META, zSINGLE );
         SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
         DropView( vMetaList );
      }
   }
#endif

   // Make sure we are positioned at the beginning of the Report we are
   // about to convert.
   SetCursorFirstEntity( vTgtReport, "Report", 0 );
// GetAddrForAttribute( &pch, vTgtReport, "Report", "Tag" );

   CreateViewFromViewForTask( &vTmpReport, vTgtReport, 0 );
   nRC = SetCursorFirstEntity( vTgtReport, "Page", "" );
   while ( nRC >= zCURSOR_SET )
   {
//    GetAddrForAttribute( &pch, vTgtReport, "Page", "Tag" );
//    TraceLineS( "(tzcmcvto) Processing Page ", pch );

      nRC = SetCursorFirstEntity( vTgtReport, "GroupSet", "" );
      while ( nRC >= zCURSOR_SET )
      {
      // GetAddrForAttribute( &pch, vTgtReport, "GroupSet", "Tag" );
      // TraceLineS( "(tzcmcvto) Processing GroupSet ", pch );

         nRC = SetCursorFirstEntity( vTgtReport, "Group", "" );
         while ( nRC >= zCURSOR_SET )
         {
         // GetAddrForAttribute( &pch, vTgtReport, "Group", "Tag" );
         // TraceLineS( "(tzcmcvto) Processing Group ", pch );

            GetStringFromAttribute( szAllowPageBreak, zsizeof( szAllowPageBreak ), vTgtReport, "Group",
                                    "AllowPageBreak" );
            lExtent = -1;  // indicate that Extent has been calculated.

            nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
            // GetAddrForAttribute( &pch, vTgtReport, "Control", "Tag" );
            // TraceLineS( "(tzcmcvto) Processing Control ", pch );

               GetIntegerFromAttribute( &lPos, vTgtReport,
                                        "Control", "PSDLG_X" );
               GetIntegerFromAttribute( &lPosY, vTgtReport,
                                        "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lSizeY, vTgtReport,
                                        "Control", "SZDLG_Y" );
               if ( lSizeY + lPosY > lExtent )
                  lExtent = lSizeY + lPosY;

               if ( CheckExistenceOfEntity( vTgtReport, "CtrlCtrl" ) == 0 )
               {
                  nRC = SetCursorFirstEntity( vTgtReport, "CtrlCtrl", "" );
                  while ( nRC >= zCURSOR_SET )
                  {
                     CreateViewFromViewForTask( &vSubCtrl, vTgtReport, 0 );
                     SetViewFromView( vSubCtrl, vTgtReport );
                     SetViewToSubobject( vSubCtrl, "CtrlCtrl" );
                     GetIntegerFromAttribute( &lOffsetX, vTgtReport,
                                              "CtrlCtrl", "PSDLG_X" );
                     GetIntegerFromAttribute( &lOffsetY, vTgtReport,
                                              "CtrlCtrl", "PSDLG_Y" );
                     SetAttributeFromInteger( vTgtReport, "CtrlCtrl",
                                              "PSDLG_X", lPos + lOffsetX );
                     SetAttributeFromInteger( vTgtReport, "CtrlCtrl",
                                              "PSDLG_Y", lPosY + lOffsetY );
                     if ( MoveSubobject( vTgtReport, "Control",
                                         vSubCtrl, "Control",
                                         zPOS_AFTER, zREPOS_NEXT ) == zCALL_ERROR )
                     {
                        GetAddrForAttribute( &pch, vTgtReport, "CtrlCtrl", "Tag" );
                        MessageBox( 0, "MoveSubobject failed", pch, MB_OK );
                        break;
                     }

                     ResetViewFromSubobject( vSubCtrl );
                     DropView( vSubCtrl );
                     SetCursorFirstEntity( vTgtReport, "Control", "" );
                     nRC = SetCursorFirstEntity( vTgtReport, "CtrlCtrl", "" );
                  }

                  nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
               }
               else
               {
                  nRC = SetCursorNextEntity( vTgtReport, "Control", "" );
               }
            }

            SetAttributeFromInteger( vTgtReport, "Group", "Extent", lExtent );
            SetAttributeFromInteger( vTgtReport, "Group", "MinimumExtent", lExtent );

#if 1
            // Set the range for each control.
            bChange = TRUE;
            while ( bChange )
            {
               bChange = FALSE;
               nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
               while ( nRC >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lRangeTop, vTgtReport, "Control", "RangeTop" );
                  GetIntegerFromAttribute( &lRangeBottom, vTgtReport, "Control", "RangeBottom" );
                  GetIntegerFromAttribute( &lPosY, vTgtReport, "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lSizeY, vTgtReport, "Control", "SZDLG_Y" );
                  lSizeY += lPosY;
                  lSizeY--;

                  if ( lRangeTop == 0 )
                  {
                     if ( lPosY )
                        lRangeTop = lPosY;
                     else
                        lRangeTop = -1; // indicate RangeTop has been calculated

                     SetAttributeFromInteger( vTgtReport, "Control","RangeTop", lRangeTop );
                  }

                  if ( lRangeBottom == 0 && lSizeY )
                  {
                     lRangeBottom = lSizeY;
                     SetAttributeFromInteger( vTgtReport, "Control", "RangeBottom", lRangeBottom );
                  }

                  SetViewFromView( vTmpReport, vTgtReport );
                  nRC = SetCursorFirstEntity( vTmpReport, "Control", "" );
                  while ( nRC >= zCURSOR_SET )
                  {
                     GetIntegerFromAttribute( &lPos, vTmpReport, "Control", "RangeTop" );
                     GetIntegerFromAttribute( &lSize, vTmpReport, "Control", "RangeBottom" );
                     if ( lPos == 0 )
                     {
                        GetIntegerFromAttribute( &lPos, vTmpReport, "Control", "PSDLG_Y" );
                        GetIntegerFromAttribute( &lSize, vTmpReport, "Control", "SZDLG_Y" );
                        lSize += lPos;
                        lSize--;

                        SetAttributeFromInteger( vTmpReport, "Control", "RangeBottom", lSize );
                        if ( lPos == 0 )
                           lPos = -1;

                        SetAttributeFromInteger( vTmpReport, "Control", "RangeTop", lPos );
                     }

                     if ( (lPos >= lRangeTop && lPos <= lRangeBottom) ||
                          (lSize >= lRangeTop && lSize <= lRangeBottom) ||
                          (lRangeTop >= lPos && lRangeTop <= lSize) ||
                          (lRangeBottom >= lPos && lRangeBottom <= lSize) )
                     {
                        if ( lPos < lRangeTop )
                        {
                           lRangeTop = lPos;
                           bChange = TRUE;
                           SetAttributeFromInteger( vTgtReport, "Control", "RangeTop", lRangeTop );
                        }

                        if ( lSize > lRangeBottom )
                        {
                           lRangeBottom = lSize;
                           bChange = TRUE;
                           SetAttributeFromInteger( vTgtReport, "Control", "RangeBottom", lRangeBottom );
                        }
                     }

                     nRC = SetCursorNextEntity( vTmpReport, "Control", "" );
                  }

                  if ( lRangeTop == 0 )
                     lRangeTop = -1;

                  SetAttributeFromInteger( vTgtReport, "Control", "RangeTop", lRangeTop );
                  SetAttributeFromInteger( vTgtReport, "Control", "RangeBottom", lRangeBottom );
               // if ( bChange )
               // {
               //    lChangeCnt++;
               //    if ( (lChangeCnt % 10) == 0 )
               //    {
               //       TraceLineI( "ConvertReport change cnt: ",
               //                   lChangeCnt );
               //    }
               //
               //    bChange = FALSE;
               //    nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
               // }
               // else
                  {
                     nRC = SetCursorNextEntity( vTgtReport, "Control", "" );
                  }
               }
            }
#ifdef DEBUG_RANGE

            nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
               GetStringFromAttribute( szTag, zsizeof( szTag ), vTgtReport, "Control", "Tag" );
               GetStringFromAttribute( szPosY, zsizeof( szPosY ), vTgtReport, "Control", "PSDLG_Y" );
               GetStringFromAttribute( szSizeY, zsizeof( szSizeY ), vTgtReport, "Control", "SZDLG_Y" );
               GetStringFromAttribute( szTop, zsizeof( szTop ), vTgtReport, "Control", "RangeTop" );
               GetStringFromAttribute( szBottom, zsizeof( szBottom ), vTgtReport, "Control", "RangeBottom" );
               GetIntegerFromAttribute( &lPosY, vTgtReport, "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lSizeY, vTgtReport, "Control", "SZDLG_Y" );
               lSizeY += lPosY;
               lSizeY--;

               sprintf_s( szMsg, zsizeof( szMsg ), "Top %s; Bot %s; Pos %s; Siz %s; End %ld; for %s",
                         szTop, szBottom, szPosY, szSizeY, lSizeY, szTag );
               TraceLineS( "Debug Range - ", szMsg );

               nRC = SetCursorNextEntity( vTgtReport, "Control", "" );
            }
#endif
#else
            // Not using LineSizeY anymore???

            nRC = SetCursorFirstEntity( vTgtReport, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lPosY, vTgtReport,
                                        "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lSizeY, vTgtReport,
                                        "Control", "SZDLG_Y" );
               lExtent = lSizeY;
               nRC = SetCursorFirstEntity( vTmpReport, "Control", "" );
               while ( nRC >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lPos, vTmpReport,
                                           "Control", "PSDLG_Y" );
                  if ( lPos == lPosY )
                  {
                     GetIntegerFromAttribute( &lSize, vTmpReport,
                                              "Control", "SZDLG_Y" );
                     if ( lExtent < lSize )
                        lExtent = lSize;
                  }

                  nRC = SetCursorNextEntity( vTmpReport, "Control", "" );
               }

               SetAttributeFromInteger( vTgtReport, "Control",
                                        "LineSizeY", lExtent );
               nRC = SetCursorNextEntity( vTgtReport, "Control", "" );
            }
#endif

            nRC = SetCursorFirstEntity( vTmpReport, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lRangeTop, vTmpReport, "Control", "RangeTop" );
               GetIntegerFromAttribute( &lRangeBottom, vTmpReport, "Control", "RangeBottom" );
               if ( lRangeTop == -1 )
               {
                  lRangeTop = 0;
                  SetAttributeFromInteger( vTmpReport, "Control", "RangeTop", lRangeTop );
               }

            // GetAddrForAttribute( &pch, vTmpReport, "Control", "Tag" );
            // TraceLineS( "Range Ctrl: ", pch );
            // TraceLineI( "       Top: ", lRangeTop );
            // TraceLineI( "    Bottom: ", lRangeBottom );
               nRC = SetCursorNextEntity( vTmpReport, "Control", "" );
            }

            // Sort target controls by position.
            OrderEntityForView( vTgtReport, "Control",
                                "RangeBottom A PSDLG_X A PSDLG_Y A" );

            nRC = SetCursorNextEntity( vTgtReport, "Group", "" );
         }

         nRC = SetCursorNextEntity( vTgtReport, "GroupSet", "" );
      }

      nRC = SetCursorNextEntity( vTgtReport, "Page", "" );
   }

   for ( k = 0; k < 256; k++ )
      pnCSV_Column[ k ] = k * 256;  // start out at one column per inch

   while ( ResetViewFromSubobject( vTmpReport ) == 0 )
   {
   }

   while ( SetCSV_Columns( vTmpReport, pnCSV_Column ) == FALSE )
   {
      while ( ResetViewFromSubobject( vTmpReport ) == 0 )
      {
      }
   }

   DropView( vTmpReport );

   // Sort target controls by position.  Controls are already sorted by
   // RangeBottom/PSDLG_X/PSDLG_Y for the CSV processing, but originally
   // the expected sort order was RangeBottom/PSDLG_Y/PSDLG_X, so even
   // though I don't believe it is necessary, we are re-sorting back to
   // the original order.
   while ( ResetViewFromSubobject( vTgtReport ) == 0 )
   {
   }

   nRC = SetCursorFirstEntity( vTgtReport, "Page", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTgtReport, "GroupSet", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = SetCursorFirstEntity( vTgtReport, "Group", "" );
         while ( nRC >= zCURSOR_SET )
         {
            OrderEntityForView( vTgtReport, "Control", "RangeBottom A PSDLG_Y A PSDLG_X A" );

            nRC = SetCursorNextEntity( vTgtReport, "Group", "" );
         }

         nRC = SetCursorNextEntity( vTgtReport, "GroupSet", "" );
      }

      nRC = SetCursorNextEntity( vTgtReport, "Page", "" );
   }

   hLibrary = (zLONG) SysLoadLibrary( vSubtask, "tzctl" );
   if ( hLibrary )
   {
      if ( (lpfnDuplicateCheck =
            (zFARPROC_DUP_CHECK) SysGetProc( (LPLIBRARY) hLibrary, "CheckDuplicateReportTags" )) )
      {
         nRC = (*lpfnDuplicateCheck)( vSubtask, vSrcReport, 0 );
         if ( nRC > 0 )
         {
            sprintf_s( szTempString, zsizeof( szTempString ),
                       "(tzcmcvto) Duplicate control tags found in Report: %s",
                       pch );
            TraceLineS( szTempString, "" );
            strcat_s( szTempString, zsizeof( szTempString ), "\n(see Zeidon trace for details)" );
            MessageSend( vSubtask, "CM00815", "Configuration Management",
                         szTempString, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
   }

   // Name the executable so it will stay around for debugging.
// SetNameForView( vTgtReport, "NewExecutable", vSubtask, zLEVEL_TASK );
// IssueError( vTgtReport, 0, 0, "End of Convert" );

// GetAddrForAttribute( (zPPCHAR) &pch, vTgtReport, "Report", "Tag" );
// TraceLineS( "Returning report to CM - ", pch );
   return( vTgtReport );
}

zVIEW OPERATION
ConvertXSLT( zVIEW   vSubtask,
             zVIEW   vSrcXSLT,
             zCPCHAR cpcTgtXSLT_Name )
{
   zFARPROC_DUP_CHECK lpfnDuplicateCheck;
// zVIEW  vMetaList;
// zVIEW  vPE;
// zPCHAR pch;
// zLONG  lKey;
   zPCHAR pch;
   zVIEW  vTgtXSLT;
   zVIEW  vTmpXSLT;
   zVIEW  vSubCtrl;
   zLONG  hLibrary;
   zLONG  lPos;
   zLONG  lSize;
   zLONG  lPosY;
   zLONG  lSizeY;
   zLONG  lOffsetX;
   zLONG  lOffsetY;
   zLONG  lExtent = 0;
   zLONG  lRangeTop;
   zLONG  lRangeBottom;
   zBOOL  bChange;
   zCHAR  szTempString[ 256 ];
// zCHAR  szAllowPageBreak[ 2 ];
   zSHORT pnCSV_Column[ 256 ];
   zSHORT k;
   zSHORT nRC;
// zLONG  lChangeCnt = 0;

//#define DEBUG_RANGE
#ifdef DEBUG_RANGE
      zCHAR szMsg[ 301 ];
      zCHAR szTag[ 101 ];
      zCHAR szPosY[ 11 ];
      zCHAR szSizeY[ 11 ];
      zCHAR szTop[ 11 ];
      zCHAR szBottom[ 11 ];
#endif

   if ( ActivateOI_FromOI_ForTask( &vTgtXSLT, vSrcXSLT, 0, 0 ) < 0 )
   {
      // ERROR out of here ...
      TraceLineS( "(tzcmcvto) Cannot activate executable XSLT - ", "TZRPSRCO" );
      MessageSend( vSubtask, "CM00802", "Configuration Management",
                   "Cannot activate executable XSLT",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }
   else
      SetNameForView( vTgtXSLT, "__Convert_X", vSubtask, zLEVEL_TASK );

#if 0
   // Activate the presentation environment
   if ( (nRC = GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK )) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
      if ( SetCursorFirstEntity( vMetaList, "W_MetaDef", "" ) < zCURSOR_SET )
      {
         TraceLineS( "(tzcmcvto) Cannot get Custom Control View - ",
                     "TZPESRCO" );
         MessageSend( vSubtask, "CM00801", "Configuration Management",
                      "Error locating Presentation Environment information",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
      else
      {
         ActivateMetaOI( vSubtask, &vPE, vMetaList, zREFER_PENV_META, zSINGLE );
         SetNameForView( vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
         DropView( vMetaList );
      }
   }
#endif

   // Make sure we are positioned at the beginning of the XSLT we are
   // about to convert.
   SetCursorFirstEntity( vTgtXSLT, "XSLT", 0 );
// GetAddrForAttribute( &pch, vTgtXSLT, "XSLT", "Tag" );

   CreateViewFromViewForTask( &vTmpXSLT, vTgtXSLT, 0 );
   nRC = SetCursorFirstEntity( vTgtXSLT, "Panel", "" );
   while ( nRC >= zCURSOR_SET )
   {
//    GetAddrForAttribute( &pch, vTgtXSLT, "Panel", "Tag" );
//    TraceLineS( "(tzcmcvto) Processing Panel ", pch );

      nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
      while ( nRC >= zCURSOR_SET )
      {
      // GetAddrForAttribute( &pch, vTgtXSLT, "Control", "Tag" );
      // TraceLineS( "(tzcmcvto) Processing Control ", pch );

         GetIntegerFromAttribute( &lPos, vTgtXSLT, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &lPosY, vTgtXSLT, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lSizeY, vTgtXSLT, "Control", "SZDLG_Y" );
         if ( lSizeY + lPosY > lExtent )
            lExtent = lSizeY + lPosY;

         if ( CheckExistenceOfEntity( vTgtXSLT, "CtrlCtrl" ) == 0 )
         {
            nRC = SetCursorFirstEntity( vTgtXSLT, "CtrlCtrl", "" );
            while ( nRC >= zCURSOR_SET )
            {
               CreateViewFromViewForTask( &vSubCtrl, vTgtXSLT, 0 );
               SetViewFromView( vSubCtrl, vTgtXSLT );
               SetViewToSubobject( vSubCtrl, "CtrlCtrl" );
               GetIntegerFromAttribute( &lOffsetX, vTgtXSLT, "CtrlCtrl", "PSDLG_X" );
               GetIntegerFromAttribute( &lOffsetY, vTgtXSLT, "CtrlCtrl", "PSDLG_Y" );
               SetAttributeFromInteger( vTgtXSLT, "CtrlCtrl", "PSDLG_X", lPos + lOffsetX );
               SetAttributeFromInteger( vTgtXSLT, "CtrlCtrl", "PSDLG_Y", lPosY + lOffsetY );
               if ( MoveSubobject( vTgtXSLT, "Control", vSubCtrl, "Control",
                                   zPOS_AFTER, zREPOS_NEXT ) == zCALL_ERROR )
               {
                  GetAddrForAttribute( &pch, vTgtXSLT, "CtrlCtrl", "Tag" );
                  MessageBox( 0, "MoveSubobject failed", pch, MB_OK );
                  break;
               }

               ResetViewFromSubobject( vSubCtrl );
               DropView( vSubCtrl );
               SetCursorFirstEntity( vTgtXSLT, "Control", "" );
               nRC = SetCursorFirstEntity( vTgtXSLT, "CtrlCtrl", "" );
            }

            nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
         }
         else
         {
            nRC = SetCursorNextEntity( vTgtXSLT, "Control", "" );
         }
      }

      SetAttributeFromInteger( vTgtXSLT, "Group", "Extent", lExtent );
      SetAttributeFromInteger( vTgtXSLT, "Group", "MinimumExtent", lExtent );

#if 1
      // Set the range for each control.
      bChange = TRUE;
      while ( bChange )
      {
         bChange = FALSE;
         nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lRangeTop, vTgtXSLT, "Control", "RangeTop" );
            GetIntegerFromAttribute( &lRangeBottom, vTgtXSLT, "Control", "RangeBottom" );
            GetIntegerFromAttribute( &lPosY, vTgtXSLT, "Control", "PSDLG_Y" );
            GetIntegerFromAttribute( &lSizeY, vTgtXSLT, "Control", "SZDLG_Y" );
            lSizeY += lPosY;
            lSizeY--;

            if ( lRangeTop == 0 )
            {
               if ( lPosY )
                  lRangeTop = lPosY;
               else
                  lRangeTop = -1; // indicate RangeTop has been calculated

               SetAttributeFromInteger( vTgtXSLT, "Control", "RangeTop", lRangeTop );
            }

            if ( lRangeBottom == 0 && lSizeY )
            {
               lRangeBottom = lSizeY;
               SetAttributeFromInteger( vTgtXSLT, "Control", "RangeBottom", lRangeBottom );
            }

            SetViewFromView( vTmpXSLT, vTgtXSLT );
            nRC = SetCursorFirstEntity( vTmpXSLT, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lPos, vTmpXSLT, "Control", "RangeTop" );
               GetIntegerFromAttribute( &lSize, vTmpXSLT, "Control", "RangeBottom" );
               if ( lPos == 0 )
               {
                  GetIntegerFromAttribute( &lPos, vTmpXSLT, "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lSize, vTmpXSLT, "Control", "SZDLG_Y" );
                  lSize += lPos;
                  lSize--;

                  SetAttributeFromInteger( vTmpXSLT, "Control", "RangeBottom", lSize );
                  if ( lPos == 0 )
                     lPos = -1;

                  SetAttributeFromInteger( vTmpXSLT, "Control", "RangeTop", lPos );
               }

               if ( (lPos >= lRangeTop && lPos <= lRangeBottom) ||
                    (lSize >= lRangeTop && lSize <= lRangeBottom) ||
                    (lRangeTop >= lPos && lRangeTop <= lSize) ||
                    (lRangeBottom >= lPos && lRangeBottom <= lSize) )
               {
                  if ( lPos < lRangeTop )
                  {
                     lRangeTop = lPos;
                     bChange = TRUE;
                     SetAttributeFromInteger( vTgtXSLT, "Control", "RangeTop", lRangeTop );
                  }

                  if ( lSize > lRangeBottom )
                  {
                     lRangeBottom = lSize;
                     bChange = TRUE;
                     SetAttributeFromInteger( vTgtXSLT, "Control", "RangeBottom", lRangeBottom );
                  }
               }

               nRC = SetCursorNextEntity( vTmpXSLT, "Control", "" );
            }

            if ( lRangeTop == 0 )
               lRangeTop = -1;

            SetAttributeFromInteger( vTgtXSLT, "Control", "RangeTop", lRangeTop );
            SetAttributeFromInteger( vTgtXSLT, "Control", "RangeBottom", lRangeBottom );
         // if ( bChange )
         // {
         //    lChangeCnt++;
         //    if ( (lChangeCnt % 10) == 0 )
         //    {
         //       TraceLineI( "ConvertXSLT change cnt: ",
         //                   lChangeCnt );
         //    }
         //
         //    bChange = FALSE;
         //    nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
         // }
         // else
            {
               nRC = SetCursorNextEntity( vTgtXSLT, "Control", "" );
            }
         }
      }
#ifdef DEBUG_RANGE

      nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttribute( szTag, zsizeof( szTag ), vTgtXSLT, "Control", "Tag" );
         GetStringFromAttribute( szPosY, zsizeof( szPosY ), vTgtXSLT, "Control", "PSDLG_Y" );
         GetStringFromAttribute( szSizeY, zsizeof( szSizeY ), vTgtXSLT, "Control", "SZDLG_Y" );
         GetStringFromAttribute( szTop, zsizeof( szTop ), vTgtXSLT, "Control", "RangeTop" );
         GetStringFromAttribute( szBottom, zsizeof( szBottom ), vTgtXSLT, "Control", "RangeBottom" );
         GetIntegerFromAttribute( &lPosY, vTgtXSLT, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lSizeY, vTgtXSLT, "Control", "SZDLG_Y" );
         lSizeY += lPosY;
         lSizeY--;

         sprintf_s( szMsg, zsizeof( szMsg ), "Top %s; Bot %s; Pos %s; Siz %s; End %ld; for %s",
                   szTop, szBottom, szPosY, szSizeY, lSizeY, szTag );
         TraceLineS( "Debug Range - ", szMsg );

         nRC = SetCursorNextEntity( vTgtXSLT, "Control", "" );
      }
#endif
#else
      // Not using LineSizeY anymore???

      nRC = SetCursorFirstEntity( vTgtXSLT, "Control", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lPosY, vTgtXSLT, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lSizeY, vTgtXSLT, "Control", "SZDLG_Y" );
         lExtent = lSizeY;
         nRC = SetCursorFirstEntity( vTmpXSLT, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lPos, vTmpXSLT, "Control", "PSDLG_Y" );
            if ( lPos == lPosY )
            {
               GetIntegerFromAttribute( &lSize, vTmpXSLT, "Control", "SZDLG_Y" );
               if ( lExtent < lSize )
                  lExtent = lSize;
            }

            nRC = SetCursorNextEntity( vTmpXSLT, "Control", "" );
         }

         SetAttributeFromInteger( vTgtXSLT, "Control", "LineSizeY", lExtent );
         nRC = SetCursorNextEntity( vTgtXSLT, "Control", "" );
      }
#endif

      nRC = SetCursorFirstEntity( vTmpXSLT, "Control", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lRangeTop, vTmpXSLT, "Control", "RangeTop" );
         GetIntegerFromAttribute( &lRangeBottom, vTmpXSLT, "Control", "RangeBottom" );
         if ( lRangeTop == -1 )
         {
            lRangeTop = 0;
            SetAttributeFromInteger( vTmpXSLT, "Control", "RangeTop", lRangeTop );
         }

      // GetAddrForAttribute( &pch, vTmpXSLT, "Control", "Tag" );
      // TraceLineS( "Range Ctrl: ", pch );
      // TraceLineI( "       Top: ", lRangeTop );
      // TraceLineI( "    Bottom: ", lRangeBottom );
         nRC = SetCursorNextEntity( vTmpXSLT, "Control", "" );
      }

      // Sort target controls by position.
      OrderEntityForView( vTgtXSLT, "Control", "RangeBottom A PSDLG_X A PSDLG_Y A" );

      nRC = SetCursorNextEntity( vTgtXSLT, "Panel", "" );
   }

   for ( k = 0; k < 256; k++ )
      pnCSV_Column[ k ] = k * 256;  // start out at one column per inch

   while ( ResetViewFromSubobject( vTmpXSLT ) == 0 )
   {
   }

   while ( SetCSV_Columns( vTmpXSLT, pnCSV_Column ) == FALSE )
   {
      while ( ResetViewFromSubobject( vTmpXSLT ) == 0 )
      {
      }
   }

   DropView( vTmpXSLT );

   // Sort target controls by position.  Controls are already sorted by
   // RangeBottom/PSDLG_X/PSDLG_Y for the CSV processing, but originally
   // the expected sort order was RangeBottom/PSDLG_Y/PSDLG_X, so even
   // though I don't believe it is necessary, we are re-sorting back to
   // the original order.
   while ( ResetViewFromSubobject( vTgtXSLT ) == 0 )
   {
   }

   nRC = SetCursorFirstEntity( vTgtXSLT, "Panel", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vTgtXSLT, "GroupSet", "" );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = SetCursorFirstEntity( vTgtXSLT, "Group", "" );
         while ( nRC >= zCURSOR_SET )
         {
            OrderEntityForView( vTgtXSLT, "Control", "RangeBottom A PSDLG_Y A PSDLG_X A" );

            nRC = SetCursorNextEntity( vTgtXSLT, "Group", "" );
         }

         nRC = SetCursorNextEntity( vTgtXSLT, "GroupSet", "" );
      }

      nRC = SetCursorNextEntity( vTgtXSLT, "Panel", "" );
   }

   hLibrary = (zLONG) SysLoadLibrary( vSubtask, "tzctl" );
   if ( hLibrary )
   {
      if ( (lpfnDuplicateCheck =
            (zFARPROC_DUP_CHECK) SysGetProc( (LPLIBRARY) hLibrary, "CheckDuplicateXSLTTags" )) )
      {
         nRC = (*lpfnDuplicateCheck)( vSubtask, vSrcXSLT, 0 );
         if ( nRC > 0 )
         {
            sprintf_s( szTempString, zsizeof( szTempString ),
                       "(tzcmcvto) Duplicate control tags found in XSLT: %s",
                       pch );
            TraceLineS( szTempString, "" );
            strcat_s( szTempString, zsizeof( szTempString ), "\n(see Zeidon trace for details)" );
            MessageSend( vSubtask, "CM00815", "Configuration Management",
                         szTempString, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }
   }

   // Name the executable so it will stay around for debugging.
// SetNameForView( vTgtXSLT, "NewExecutable", vSubtask, zLEVEL_TASK );
// IssueError( vTgtXSLT, 0, 0, "End of Convert" );

// GetAddrForAttribute( (zPPCHAR) &pch, vTgtXSLT, "XSLT", "Tag" );
// TraceLineS( "Returning report to CM - ", pch );
   return( vTgtXSLT );
}
