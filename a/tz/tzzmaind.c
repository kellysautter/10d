#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//  Module TZZMAIND.C
//
//  CHANGE NOTES:
//
//  1997/01/12 Guenter Trageser: New tool zTOOL_REPORT
//                               (entry needed for About dialog)
//
// 2000.10.30 RG  Z2000
//     GetViewByName: for parameter zLEVEL_TASK changed View --> 0
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_Postbuild
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_PostBuild( zVIEW vSubtask )
{
   zSHORT nRC;
   zCHAR  szLPL_Name[ 34 ];
   zVIEW  vLPL;
   zVIEW  vMetaList;

   InitializeLPLR( vSubtask, "" );
   nRC = GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
   {
      GetStringFromAttribute( szLPL_Name, zsizeof( szLPL_Name ), vLPL, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, 0, szLPL_Name );
   }

   // Get view to all list types
   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_SA_META );
   SetNameForView( vMetaList, "TZERDS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_SA_META );
   SetNameForView( vMetaList, "TZERDR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_DOMAIN_META );
   SetNameForView( vMetaList, "TZDMNS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_DOMAIN_META );
   SetNameForView( vMetaList, "TZDMNR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_DTE_META );
   SetNameForView( vMetaList, "TZDBMS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_DTE_META );
   SetNameForView( vMetaList, "TZDBMR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_LOD_META );
   SetNameForView( vMetaList, "TZLODS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_LOD_META );
   SetNameForView( vMetaList, "TZLODR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_DIALOG_META );
   SetNameForView( vMetaList, "TZDLGS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_DIALOG_META );
   SetNameForView( vMetaList, "TZDLGR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_PENV_META );
   SetNameForView( vMetaList, "TZPENS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
   SetNameForView( vMetaList, "TZPENR", vSubtask, zLEVEL_TASK );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zSOURCE_GO_META );
   SetNameForView( vMetaList, "TZOPRS", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_GO_META );
   SetNameForView( vMetaList, "TZOPRR", vSubtask, zLEVEL_TASK );

   MB_SetMessage( vSubtask, 1, "?" );
   MB_SetMessage( vSubtask, 2, "*" );
   return( 0 );
}

zSHORT
fnLaunchTool( zVIEW vSubtask, zPCHAR szDialogName )
{
   zVIEW vTempSubtask;

   SfCreateSubtask( &vTempSubtask, vSubtask, "Zeidon_Tools" );
// SetNameForView( vTempSubtask, szAppName, vSubtask, zLEVEL_TASK );
   SetNewApplicationSubtask( vSubtask, vTempSubtask );
// SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessDialog,
   SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessApp,
                            szDialogName, "" );
   return( 0 );

// SetWindowActionBehavior( vSubtask, zWAB_StartOrFocusModelessApp,
//                          szDialogName, 0 );
// return( 0 );

// zCHAR szCommand[ 256 ];
//
// GetApplDirectoryFromView( szCommand, vSubtask,
//                           zAPPL_DIR_LIB, 200 );
// strcat_s( szCommand, zsizeof( szCommand ), "\\zdr.exe \\\\Zeidon_Tools\\" );
// strcat_s( szCommand, zsizeof( szCommand ), szDialogName );
// return( StartTask( vSubtask, szCommand, SW_SHOWNORMAL ) );
}

zSHORT
fnRefreshComponentList( zVIEW vSubtask, zPCHAR szListType )
{
   zVIEW vListWindow;
   zVIEW vList;
   zCHAR szListView[ 10 ];

   vListWindow = 0;
   GetViewByName( &vListWindow, "APCMLIST", vSubtask, zLEVEL_TASK );
   if ( !szListType )
   {
      if ( GetViewByName( &vList, "TZLPLRLS", vSubtask, zLEVEL_TASK ) > 0 )
         DropNameForView( vList, "TZLPLRLS", vSubtask, zLEVEL_TASK );
   }
   else
   if ( szListType[ 0 ] )
   {
      strcpy_s( szListView, zsizeof( szListView ), "TZ" );
      strcat_s( szListView, zsizeof( szListView ), szListType );
      if ( vListWindow &&
            GetCtrlState( vListWindow, "Refer", zCONTROL_STATUS_CHECKED ) > 0 )
      {
         strcat_s( szListView, zsizeof( szListView ), "R" );
      }
      else
         strcat_s( szListView, zsizeof( szListView ), "S" );

      GetViewByName( &vList, szListView, vSubtask, zLEVEL_TASK );
      SetNameForView( vList, "TZLPLRLS", vSubtask, zLEVEL_TASK );
   }

   if ( vListWindow )
      RefreshWindow( vListWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_ListPostBuild
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_ListPostBuild( zVIEW vSubtask )
{
   SetNameForView( vSubtask, "APCMLIST", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusER
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusER( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "ERD" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusDomains
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusDomains( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "DMN" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusDTE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusDTE( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "DBM" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusLOD( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "LOD" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusDialog( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "DLG" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusPE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusPE( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "PEN" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusBrowser
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusBrowser( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, 0 ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_FocusOperations
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_FocusOperations( zVIEW vSubtask )
{
   return( fnRefreshComponentList( vSubtask, "OPR" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartER
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartER( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzeremdd" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartDomains
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartDomains( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzdmupdd" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartDTE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartDTE( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzteupdd" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartLOD
//
// PURPOSE:    This Entry starts the ER Tool
//
////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartLOD( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzzolodd" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartDialog
//
//   PURPOSE:    This Entry starts the ER Tool
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartDialog( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzpntrad" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartPE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartPE( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzpeupdd" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartBrowser
//
// PURPOSE:    This Entry starts the ER Tool
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartBrowser( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzobrwad" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartAnimator
//
//   PURPOSE:    This Entry starts the ER Tool
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartAnimator( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzvsxcad" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_StartOperations
//
//   PURPOSE:    This Entry starts the ER Tool
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartOperations( zVIEW vSubtask )
{
   return( fnLaunchTool( vSubtask, "tzopupdd" ) );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_CheckForSwitch( zVIEW vSubtask )
{
zVIEW vList;

   if ( GetViewByName( &vList, "TZLPLRLS", vSubtask, zLEVEL_TASK ) < 1 )
   {
      zwTZZMAIND_PostBuild( vSubtask );
      fnRefreshComponentList( vSubtask, 0 );
   }

        return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_StartEditor( zVIEW vSubtask )
{
   zCHAR szCommand[ 256 ];

   SysGetEnvVar( szCommand, "ZEIDON", 200 );
   strcat_s( szCommand, zsizeof( szCommand ), "\\CW -ZEditor" );
   return( StartTask( vSubtask, szCommand, SW_SHOWNORMAL ) );
}

zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_DisplayPopup1( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   CreateTrackingPopupMenu( vSubtask, "Popup1", pt.x, pt.y, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_ToolAboutPostBuild
//
//   PURPOSE:    For ToolAbout window, Set Icon visible, Set caption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_ToolAboutPostBuild( zVIEW vSubtask )
{
   zVIEW  vAbout;
   zLONG  lTool;
   zPCHAR szToolTitle;

   SetCtrlState( vSubtask, "IconSYSADM", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconWKSADM", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconDOMAIN", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconERD", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconDTE", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconLOD", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconDIALOG", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconPENV", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconGO", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconHDR", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconDBG", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "IconREPORT", zCONTROL_STATUS_VISIBLE, 0 );

   GetViewByName( &vAbout, "ToolAbout", vSubtask, zLEVEL_TASK);

// Make proper tool icon visible

   GetIntegerFromAttribute( &lTool, vAbout, "TZ", "AboutToolTitleID" );

// LoadString( g_hInstanceDLL, lTool, szTool, 100 );

   if ( lTool ==  zTOOL_SYSADM )
      SetCtrlState( vSubtask, "IconSYSADM", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_WKSADM )
      SetCtrlState( vSubtask, "IconWKSADM", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_DOMAIN )
      SetCtrlState( vSubtask, "IconDOMAIN", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_ERD )
      SetCtrlState( vSubtask, "IconERD", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_DTE )
      SetCtrlState( vSubtask, "IconDTE", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_LOD )
      SetCtrlState( vSubtask, "IconLOD", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_DIALOG )
      SetCtrlState( vSubtask, "IconDIALOG", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_PENV )
      SetCtrlState( vSubtask, "IconPENV", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_GO )
      SetCtrlState( vSubtask, "IconGO", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_HDR )
      SetCtrlState( vSubtask, "IconHDR", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_DEBUG )
      SetCtrlState( vSubtask, "IconDBG", zCONTROL_STATUS_VISIBLE, 1 );
   else
   if ( lTool ==  zTOOL_REPORT )
      SetCtrlState( vSubtask, "IconREPORT", zCONTROL_STATUS_VISIBLE, 1 );

// Set window caption as tool name


   GetAddrForAttribute( &szToolTitle, vAbout, "TZ", "AboutToolTitle" );
   SetWindowCaptionTitle( vSubtask, 0, szToolTitle );


   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:    zwTZZMAIND_ToolAboutExit
//
//   PURPOSE:    Drop the About Porifle View on exit from window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZZMAIND_ToolAboutExit( zVIEW vSubtask )
{
   zVIEW  vAbout;

   GetViewByName( &vAbout, "ToolAbout", vSubtask, zLEVEL_TASK);
   DropView( vAbout );

   return( 0 );
}

#ifdef __cplusplus
}
#endif
