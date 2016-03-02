//
// MODULE NAME:  TZXSLTDD  -  Painter dialog operations
// DESCRIPTION:  This is the c++ file which contains the painter
//               dialog operations
/////////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1998 QuinSoft, Inc. All rights reserved.  Confidential   *
// * and Proprietary material subject to license - do not reproduce or      *
// * disclose.  This material is an unpublished work and is considered a    *
// * trade secret belonging to the copyright holder.                        *
/////////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    9/30/92
//  API:     MicroSoft foundation classes and Windows 3.1 SDK
//  ENVIRONMENT: Windows 3.1
//
//  LAST MESSAGE ID: PN00223
//

/*
CHANGE LOG
2012.06.12 DKS
   Start XSLT Implementation
*/

#define _MT 1

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define KZOESYS_INCL
#define zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
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

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#include "tzctlopr.h"
#include "ZeidonOp.H"

#pragma pack( 1 )
#include <tzwdlgso.hst>
#pragma pack( )

typedef zSHORT (POPERATION zPAINTERCALL)( zSHORT, zVIEW, zPVOID,
                                          zVIEW, zPCHAR );

typedef zSHORT (POPERATION zFARPROC_DUP_CHECK)( zVIEW vSubtask,
                                                zVIEW vSrcXSLT,
                                                zLONG lFlags );

zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg, zVIEW vSubtask, zVIEW  vXSLT, zPCHAR szParm, zLONG lMaxSize );

zOPER_EXPORT zSHORT OPERATION
AskForSave( zVIEW vSubtask );
void
fnCreateMappingEntities( zVIEW vSubtask, zVIEW vXSLT );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_InitMenuItems( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_InitModalSubwindow( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OPT_UPD_PrevMenuItem( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OptionDelete( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OK( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Delete( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Cancel( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_Init( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_OK( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OPT_UPD_DisableActionButton( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
LIST_ACT_AddAction( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_SelectAction( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SetUpTZOPRUSO_View( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZXSLTDD_LoadEventPopups( zVIEW  vSubtask,
                          zPCHAR szActionNameForUpdate,
                          zPCHAR szActionNameForNew,
                          zPCHAR szActionNameForSelect,
                          zPCHAR szActionNameForRemove,
                          zPCHAR szActionNameForEdit,
                          zPCHAR szPopupName );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_EnableMenuDeleteAll( zVIEW   vSubtask,
                              zPCHAR  szEntityName );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_EnableMenuItems( zVIEW    vSubtask,
                          zPCHAR   szListname,
                          zPCHAR   szEntityName,
                          zPCHAR   szMenuItem,
                          zPSHORT  nEnableUpdate,
                          zPSHORT  nEnableDelete );
zSHORT
zwfnTZXSLTDD_SaveAsSetSourceName( zVIEW   vSubtask,
                                  zVIEW   vSaveAs,
                                  zVIEW   vXSLTData,
                                  zSHORT  nIndex );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SaveAs( zVIEW   vSubtask,
                 zPCHAR  szXSLTName);
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteAllActions( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_DeleteAllActions( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_CheckExistsSourceFile( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddSourceFile( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_UpdateAction( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSelectedOperat( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSourceFile( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SetSaveAsName( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_GoToUpdateOperation( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_DeleteAction( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
OpenXSLT_File( zVIEW vSubtask, zVIEW vNewXSLT );


zSHORT
zwfnTZXSLTDD_Load_PPE( zVIEW vSubtask )
{
   zVIEW   vMetaList;
   zVIEW   vTZPESRCO;
   zSHORT  nRC;

   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
      return( 0 );

   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
   nRC = LoadZeidonPPE( vSubtask, &vTZPESRCO, zREFER_PENV_META, vMetaList,
                        "XSLT Maintenance",
                        "Painter cannot be started.");

   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( -16 );
   }

   if ( SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef",
                                       "Key", 1000, 0 ) >= 0 )
   {
      SetNameForView( vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
      DropView( vMetaList );
   }
   else
   {
      MessageSend( vSubtask, "PN00201", "XSLT Maintenance",
                   "Invalid Presentation Environment!\n"
                   "New PPE required.  Painter exiting.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( -16 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_PreBuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vMetaList;

   zVIEW  vTZXSLTSO;
   zVIEW  vTaskLPLR;
   zVIEW  vSaveAs;
   zCHAR  szApplName[ 34 ];
   zPCHAR pchLPLR_Name;

   // Get the Profile information and
   // Call configuration management to set up the default LPLR.
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   pchLPLR_Name = 0;
   GetViewByName( &vTZXSLTSO, "__SysApplicationXSLT", vSubtask, zLEVEL_SYSTEM );

   if ( vTZXSLTSO )
   {
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
         DropObjectInstance( vTaskLPLR );

      GetApplDirectoryFromView( szApplName, vTZXSLTSO, zAPPL_NAME, 33 );
      if ( InitializeLPLR( vSubtask, szApplName ) < 0 )
         return( -1 );
   }
   else
   if ( CompareAttributeToString( vProfileXFER, "WD", "StartupLPLR_Option",
                                  "D" ) == 0 )   // default LPLR
   {
      if ( InitializeLPLR( vSubtask, "" ) < 0 )
         return( -1 );
   }
   else
   {
      GetAddrForAttribute( &pchLPLR_Name, vProfileXFER, "WD", "StartupLPLR_Name" );

      if ( InitializeLPLR( vSubtask, pchLPLR_Name ) < 0 )
         return( -1 );
   }

   // Get a list of XSLTs
   if ( GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_XSLT_META );

   SetNameForView( vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Activate the presentation environment.
   if ( zwfnTZXSLTDD_Load_PPE( vSubtask ) < 0 )
      return( -1 );

   // If the XSLT is not checked out and the user changes this XSLT,
   // then Zeidon calls the window "Save XSLT as".
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new XSLT or Open an other XSLT or Switch Project or
   // Exit XSLT Tool).
   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vSaveAs, "TZSAVEAS", vSubtask, zSINGLE );
      SetNameForView( vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
      CreateEntity( vSaveAs, "ActionAfterSaveAS", zPOS_AFTER );
   }

   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zNoActionAfterSaveAs );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_PostBuild( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zVIEW  vTZPNCTWO;
   zVIEW  vNewXSLTL = 0;
   zVIEW  vPanelList;
   zVIEW  vTZXSLTSO;
   zVIEW  vProfileXFER;
   zCHAR  szPanelName[ 33 ];

   // Setup the List of XSLTs.
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Name the subtask associated with the main page so we can set its
   // title later.
   SetNameForView( vSubtask, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   // Set the top entities to "XSLT" and "Panel" (rather than the default
   // of "XSLT" and "Window") ... but do not do initialization.
   fnPainterCall( zMSG_INITPAINTER, vSubtask, 0, (zPCHAR) -1, 0 );

   GetViewByName( &vTZXSLTSO, "__SysApplicationXSLT", vSubtask, zLEVEL_SYSTEM );
   if ( vTZXSLTSO )
   {
      // Drop the previous instance being worked on if it exists.
      GetViewByName( &vNewXSLTL, "TZOPENRPT", vSubtask, zLEVEL_TASK );
      if ( vNewXSLTL )
         DropMetaOI( vSubtask, vNewXSLTL );

      OpenXSLT_File( vSubtask, vTZXSLTSO );
      GetViewByName( &vNewXSLTL, "PANELL", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByString( vNewXSLTL, "Panel", "Tag",
                                         szPanelName, 0 ) >= zCURSOR_SET )
      {
      // TZXSLTDD_UpdatePanelFromList( vSubtask );
      }

      if ( GetViewByName( &vPanelList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) > 0 )
      {
         SetWindowActionBehavior( vPanelList, zWAB_ReturnToParent |
                                  zWAB_ProcessImmediateAction, 0, 0 );
      }
   }
   else
   {
#ifndef __WIN32__
      if ( GetViewByName( &vPanelList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) < 1 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow |
                                            zWAB_ProcessImmediateAction,
                                  "TZXSLTDD", "PanelList" );
      }
#endif

      // Transfer to the open report window
      if ( vMetaList )
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                                  zWAB_ProcessImmediateAction,
                                  "TZXSLTDD", "XSLTList" );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Zeidon XSLT Maintenance", "PANELL",
                              vNewXSLTL, "XSLT", zSOURCE_XSLT_META );

   // Initialize the TZPNCTWO work object.
   ActivateEmptyObjectInstance( &vTZPNCTWO, "TZPNCTWO", vSubtask, zSINGLE );
   CreateEntity( vTZPNCTWO, "TZPNCTWO", zPOS_AFTER );
   SetAttributeFromString( vTZPNCTWO, "TZPNCTWO", "PainterType", "X" ); // XSLT
   SetNameForView( vTZPNCTWO, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set the top entities to "XSLT" and "Panel" (rather than the default
   // of "XSLT" and "Window") ... and do initialization.
   fnPainterCall( zMSG_INITPAINTER, vSubtask, 0, (zPCHAR) 2, 0 );

   if ( GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zLONG lFlags;
      zSHORT nState;

      lFlags = RestoreWindowPlacement( vSubtask, vProfileXFER );
      nState = (lFlags & 0x00000001) ? 1 : 0;
      fnPainterCall( zMSG_SETSCROLLBARS, vSubtask, 0, (zPCHAR) 1, 0 );
      nState = (zSHORT) fnPainterCall( zMSG_RESTORE_PLACEMENT, vSubtask, 0, (zPCHAR) vProfileXFER, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ExitXSLT( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vProfileXFER;
   zVIEW  vLPLR;
   zVIEW  vTZXSLTSO;
   zVIEW  vSaveAs;
   zLONG  lFlags;
   zSHORT nRC;

   // Now see if the painter is being invoked by an application.
   GetViewByName( &vTZXSLTSO, "__SysApplicationXSLT",
                  vSubtask, zLEVEL_SYSTEM );
   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
      nRC = AskForSave( vSubtask );
      if ( vSaveAs )
      {
         if ( nRC )
         {
            if ( nRC == 99 )
               // if XSLT not checked out and user change this XSLT, then Zeidon
               // call the window "Save XSLT as"
               // We save the action after Save as in the View TZSAVEAS (for example:
               // Create new XSLT or Open an other XSLT or Switch Project or
               // Exit XSLT Tool)
               SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                        "Typ", zExitToolAfterSaveAs );
            else
               SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                        "Typ", zNoActionAfterSaveAs );

            return( 0 );
         }

         DropMetaOI( vSubtask, vSaveAs );
      }
   }

   // We are terminating.
   fnPainterCall( zMSG_TERMPAINTER, vSubtask, 0, (zPCHAR) 1, 0 );

   // Now save the Profile Info...
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );

   // Until scrollbar and toolbar options exists, always show toolbar???
// if ( GetOptionState( vSubtask, "ScrollBars", zOPTION_STATUS_CHECKED ) )
      lFlags = 0x00000001;
// else
//    lFlags = 0x00000002;

   fnPainterCall( zMSG_SAVE_PLACEMENT, vSubtask, 0, (zPCHAR) vProfileXFER, 0 );
   SaveWindowPlacement( vSubtask, vProfileXFER, lFlags );

   if ( CompareAttributeToString( vProfileXFER, "WD", "StartupLPLR_Option", "L" ) == 0 )   // set Last LPLR
   {
      GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vLPLR )
      {
         SetAttributeFromAttribute( vProfileXFER, "WD", "StartupLPLR_Name", vLPLR, "LPLR", "Name" );
      }
   }

   oTZ__PRFO_CommitProfile( vProfileXFER );

   // Delete all the painter windows explicitly before the
   // default CWnd destructor does it and causes problems.
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );
   fnPainterCall( zMSG_DELETETOOLBAR, vSubtask, 0, 0, 0 );

   if ( MiValidView( vXSLT ) )
      DropMetaOI( vSubtask, vXSLT );

   TerminateLPLR( vSubtask );
   if ( MiValidView( vTZXSLTSO ) )
   {
      DropNameForView( vTZXSLTSO, "__SysApplicationXSLT", vSubtask, zLEVEL_SYSTEM );
   // SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AskForSave( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zBOOL  bSaveAs = FALSE;
   zCHAR  szXSLTName[ 33 ];
   zCHAR  szMessageText[ 254 ];
   zSHORT nRC;

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) <= 0 )
      return( 0 );

   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   if ( MiGetUpdateForView( vXSLT ) && ObjectInstanceUpdatedFromFile( vXSLT ) )
   {
      // if XSLT not checked out, then ask for save as
      if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
      {
         bSaveAs = TRUE;
         GetStringFromAttribute( szXSLTName, zsizeof( szXSLTName ), vXSLT, "XSLT", "Tag" );
         strcpy_s( szMessageText, zsizeof( szMessageText ), "XSLT is not checked out, but XSLT '" );
         strcat_s( szMessageText, zsizeof( szMessageText ), szXSLTName );
         strcat_s( szMessageText, zsizeof( szMessageText ), "' has been modified. \n\nWould you like to " );
         strcat_s( szMessageText, zsizeof( szMessageText ), "save it with differend name?" );
         nRC = MessagePrompt( vSubtask, "DM00115", "XSLT Maintenance",
                              szMessageText, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );
      }
      else
      {
         nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_DIALOG,
                             zREFER_XSLT_META,
                             vXSLT, "XSLT", "Tag", 0 );
      }

      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( nRC );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            // If XSLT not checked out and user would save it, then call
            // window save XSLT as.
            if ( bSaveAs )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                        "TZXSLTDD", "XSLT_SaveAs" );
               return( 99 );
            }

         // nRC = SaveXSLTFile( vSubtask );
            if ( nRC )
            {
               SetWindowActionBehavior( vSubtask,
                                        zWAB_TerminateActionForError,
                                        0, 0 );
               return( nRC );
            }
         }
         else
            return( zRESPONSE_NO );
      }
   } // endif ( MiGetUpdateForView( vXSLT ) && ...

   return( 0 );
}

//./ ADD NAME=EnablePainterWindows
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      EnablePainterWindows
//
//  PURPOSE:    To enable painter windows when a control dialog returns
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
EnablePainterWindows( zVIEW vSubtask )
{
   zVIEW   vMetaList;
   zVIEW   vWindowList = 0;
   zVIEW   vTZPESRCO = 0;
   zVIEW   vTZXSLTL;
   zSHORT  nRC;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // Activate the presentation environment, if the presentation
   // environment is no longer with us
   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      // Activate the presentation environment
      if ( zwfnTZXSLTDD_Load_PPE( vSubtask ) < 0 )
         return( -1 );

      fnPainterCall( zMSG_REPLACETOOLBARBUTTONS, vSubtask, 0, 0, 0 );
   } // endif ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "XSLT Maintenance", "TZXSLTL",
                              vTZXSLTL, "XSLT", zSOURCE_XSLT_META );

   if ( GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_XSLT_META );
      if ( vMetaList )
         SetNameForView( vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindowList, "WND_LIST" ) >= 0 )
   {
//    SetWindowState( vWindowList, zWINDOW_STATUS_VISIBLE, 1 );
//    SetWindowState( vWindowList, zWINDOW_STATUS_ENABLED, 1 );
#ifndef __WIN32__
      SetCtrlState( vWindowList, "ListBox1", zCONTROL_STATUS_ENABLED, 1 );
      RefreshCtrl( vWindowList, "ListBox1" );
#endif
   }

#ifndef __WIN32__
   if ( GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK ) < 1 &&
        GetSubtaskForWindowName( vSubtask, &vWindowList, "WND_LIST" ) >= 0 )
   {
      ActivateWindow( vWindowList );
      return( 0 );
   }
#endif

   nRC = (zSHORT) fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, "Y", 0 );
   return( nRC );
}

//./ ADD NAME=DisablePainterWindows
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DisablePainterWindows
//
//  PURPOSE:    To disable painter windows when a control dialog returns
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DisablePainterWindows( zVIEW vSubtask )
{
   return( (zSHORT) fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, 0, 0 ) );
}

//./ ADD NAME=SwitchLPLR
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     SwitchLPLR
//
//  PURPOSE:   To switch to a new project library, simply drop
//             our local view for the dialog object TZXSLTS
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SwitchLPLR( zVIEW vSubtask )
{
   zVIEW vTZXSLTS;
   zVIEW vTZPESRCO;
   zVIEW vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = AskForSave( vSubtask);

   if ( nRC )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if XSLT not checked out and user change this XSLT, then zeidon
            // call the window "Save XSLT as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new XSLT or Open an other XSLT or Switch Project or
            // Exit XSLT Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zSwitchLPLRAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
            if ( nRC < 0 || nRC == zRESPONSE_CANCEL )
               return( 0 );
         }
      }
   }

   // Send in flag to force reload of task view for LPLR.
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, (zPCHAR) 1, 0 );
   if ( GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZXSLTS );

   //BL, 2000.01.26 drop View for XSLT list in Window ACTION DETAIL.
   if ( GetViewByName( &vTZXSLTS, "TZDLG_List", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZXSLTS );

   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vTZPESRCO );

   // This SetName causes the list of XSLTs to be presented after the
   // switch LPLR completes.
   SetNameForView( vSubtask, "__SysIdlePainter", 0, zLEVEL_SYSTEM );

   return( 0 );
}

//./ ADD NAME=SystemMinimize
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SystemMinimize
//
//  PURPOSE:    Called when painter is minimized, tell painter to
//              minimize all windows being painter
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SystemMinimize( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vIdleView;

   TraceLineS("(tzxsltdd) - SystemMinimize","");

   fnPainterCall( zMSG_SYSTEMMINIMIZE, vSubtask, 0, 0, 0 );
   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
   {
      if ( GetViewByName( &vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) < 1 )
      {
         SetWindowCaptionTitle( vSubtask, "Painter", "[Idle]" );
         SetNameForView( vSubtask, "__SysIdlePainter", 0, zLEVEL_SYSTEM );
      }
   }

   return( 0 );
}

//./ ADD NAME=SystemActivate
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SystemActivate
//
//  PURPOSE:    Called when painter is restored or maximized, restore
//              all painter windows
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SystemActivate( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vKZWDLGXO;
   zVIEW    vIdleView;
   zVIEW    vTaskLPLR;
   zVIEW    vWindowList = 0;
   zCHAR    szWorkString[ 256 ];

// TraceLineS( "(tzxsltdd) - SystemActivate", "" );

   if ( GetViewByName( &vKZWDLGXO,
                       "__SysPainterHotkey", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        GetViewByName( &vIdleView,
                       "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        vIdleView == vSubtask )
   {
      // The driver is trying to hotkey to us, see if we have an LPLR and
      // an open dialog already up.
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
      {
         GetApplDirectoryFromView( szWorkString, vKZWDLGXO, zAPPL_NAME,
                                   zsizeof( szWorkString ) - 1 );
         if ( CompareAttributeToString( vTaskLPLR, "LPLR", "Name",
                                        szWorkString ) == 0 )
         {
            if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
            {
               if ( CompareAttributeToAttribute( vXSLT, "XSLT", "Tag",
                                vKZWDLGXO, "Dlg", "Tag" ) == 0 )
               {
                  DropNameForView( vKZWDLGXO, "__SysPainterHotkey",
                                   0, zLEVEL_SYSTEM );
                  SetNameForView( vKZWDLGXO, "__SysPainterHotkey",
                                  vSubtask, zLEVEL_TASK );
                  DropNameForView( vSubtask, "__SysIdlePainter",
                                   0, zLEVEL_SYSTEM );
                  GetStringFromAttribute( szWorkString, zsizeof( szWorkString ),
                                          vKZWDLGXO, "Wnd", "Tag" );
                  GetViewByName( &vWindowList, "TZXSLTL", vSubtask, zLEVEL_TASK );
                  if ( SetCursorFirstEntityByString( vWindowList,
                       "Window", "Tag", szWorkString, 0 ) >= zCURSOR_SET )
                  {
                     fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, szWorkString, zsizeof( szWorkString ) );
//                   UpdateWindowFromList( vSubtask );
                  }

                  return( 0 );
               }
               else
                  DropMetaOI( vSubtask, vXSLT );
            }
         }
         else
         {
            if ( GetViewByName( &vXSLT, "TZXSLTS", vSubtask, zLEVEL_TASK ) > 0 )
               DropView( vXSLT );

            if ( GetViewByName( &vXSLT, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
               DropView( vXSLT );

            TerminateLPLR( vSubtask );
            if ( GetViewByName( &vXSLT, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
               DropNameForView( vXSLT, "TaskLPLR", vSubtask, zLEVEL_TASK );
         }
      }

      TZXSLTDD_PreBuild( vSubtask );
      TZXSLTDD_PostBuild( vSubtask );
   }
   else
   if ( GetViewByName( &vIdleView,
                       "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        vIdleView == vSubtask )
   {
      DropNameForView( vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM );

#ifdef __WIN32__
   // SetOptionState( vSubtask, "WindowList", zOPTION_STATUS_ENABLED, 0 );
#else
      if ( GetViewByName( &vXSLT, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) < 1 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow |
                                  zWAB_ProcessImmediateAction,
                                  "TZXSLTDD", "WND_LIST" );
      }
#endif
      if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      {
         // Transfer to the open dialog window
         if ( GetViewByName( &vXSLT, "TZXSLTS", vSubtask, zLEVEL_TASK ) >= 0 )
            DropView( vXSLT );

         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                                            zWAB_ProcessImmediateAction,
                                  "TZXSLTDD", "XSLT_OPEN" );
      }
   }

   if ( fnPainterCall( zMSG_PAINTERINITIALIZED, vSubtask, 0, 0, 0 ) == 0 )
   {
//    TraceLineS( "(tzxsltdd) - SystemActivate - ", "Early return" );
      return( 0 );
   }

#ifndef __WIN32__
   GetViewByName( &vWindowList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK );
   if ( vWindowList )
      PutWindowOnTop( vWindowList );

   fnPainterCall( zMSG_PUTTOOLBOXONTOP, vSubtask, 0, 0, 0 );
#endif

   GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );

   fnCreateMappingEntities( vSubtask, vXSLT );

   return( 0 );
}

//./ ADD NAME=SystemRestore
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SystemRestore
//
//  PURPOSE:    Called when painter is restored or maximized, restore
//              all painter windows
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SystemRestore( zVIEW vSubtask )
{
   TraceLineS("(tzxsltdd) - SystemRestore","");

   fnPainterCall( zMSG_SYSTEMRESTORE, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowControlBar( zVIEW   vSubtask )
{
   zVIEW vXSLT;

   GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );
   fnCreateMappingEntities( vSubtask, vXSLT );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
HideControlBar( zVIEW   vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowHideControlBar( zVIEW   vSubtask )
{
   zVIEW vXSLT;
   zBOOL bVisible;

   GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );
   bVisible = (zBOOL) GetCtrlState( vSubtask, "ControlBar",
                                    zCONTROL_STATUS_VISIBLE );
   SetCtrlState( vSubtask, "ControlBar", zCONTROL_STATUS_VISIBLE, !bVisible );
   SetOptionState( vSubtask, "ShowHideControlBar", zOPTION_STATUS_TEXT,
                   (zULONG) (bVisible ? "Show Control Bar" : "Hide Control Bar") );
   fnCreateMappingEntities( vSubtask, vXSLT );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowMappingPopup( zVIEW   vSubtask )
{
   zCHAR szEntityName[ 33 ];

   OL_GetCurrentEntityName( vSubtask, "View", 0, szEntityName );
   TraceLineS( "EntityName: ", szEntityName );
   if ( zstrcmp( szEntityName, "ViewObjRef" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "ViewObjRef", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "LOD" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "LOD", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "LOD_Entity" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "LOD_Entity", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "ER_Attribute" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "ER_Attribute", -1, 0, FALSE, FALSE );

   return( 0 );
}

zBOOL
ValidateCtrlAndActionTags( zVIEW vSubtask, zVIEW vXSLT )
{
   zFARPROC_DUP_CHECK lpfnDuplicateCheck;
   zCHAR  szTempString[ 256 ];
   zLONG  hLibrary;
   zPCHAR pchDlg;
   zPCHAR pchWnd;
   zSHORT nRC;

   // If there are duplicate tags in Ctrl or across Ctrl/Action, it can
   // cause problems at run time for the generated JSP.  So let's warn
   // the developer if the condition exists.

   hLibrary = (zLONG) SysLoadLibrary( vSubtask, "tzctl" );
   if ( hLibrary )
   {
      if ( (lpfnDuplicateCheck =
            (zFARPROC_DUP_CHECK) SysGetProc( (LPLIBRARY) hLibrary,
                                             "CheckDuplicateTags" )) )
      {
      // TraceLine( "(tzcmcvto) Checking for duplicate control/action tags XSLT.Window: %s.%s",
      //            pchDlg, pchWnd );
         nRC = (*lpfnDuplicateCheck)( vSubtask, vXSLT, 1 );
         if ( nRC > 0 )
         {
            GetAddrForAttribute( &pchDlg, vXSLT, "XSLT", "Tag" );
            GetAddrForAttribute( &pchWnd, vXSLT, "Window", "Tag" );
            zsprintf( szTempString,
                      "(tzcmcvto) Duplicate control/action tags found in XSLT.Window: %s.%s",
                      pchDlg, pchWnd );
            TraceLineS( szTempString, "" );
            strcat_s( szTempString, zsizeof( szTempString ), "\n(see Zeidon trace for details)" );
            MessageSend( vSubtask, "CM00815", "JSP Generation",
                         szTempString, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( TRUE );  // duplicates found
         }
      }
   }

   return( FALSE );  // no duplicates found
}

//./ ADD NAME=DeleteCurrentPainterWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteCurrentPainterWindow
//
//  PURPOSE:    To hide the currently selected window
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteCurrentPainterWindow( zVIEW vSubtask )
{
   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vSubtask, 0, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnGetTagPrefix( zPCHAR pchTag )
{
   char   ch = 0;
   zSHORT nCnt = 0;
   zSHORT nPos = 0;
   zSHORT k = 0;

   while ( pchTag[ nPos ] && nCnt < 2 )
   {
      if ( isupper( pchTag[ nPos ] ) )
      {
         if ( nCnt )
            pchTag[ nCnt ] = pchTag[ nPos ];

         nCnt++;
      }
      else
      if ( ch == 0 &&
           pchTag[ nPos ] != 'a' &&
           pchTag[ nPos ] != 'e' &&
           pchTag[ nPos ] != 'i' &&
           pchTag[ nPos ] != 'o' &&
           pchTag[ nPos ] != 'u' )
      {
         // May need first non-vowel lower-case character.
         ch = toupper( pchTag[ nPos ] );
      }

      nPos++;
   }

   if ( ch && nCnt < 2 )
      pchTag[ nCnt++ ] = ch;

   pchTag[ nCnt ] = 0;

   return( nCnt );
}

void
fnRecurseChangeDlgTags( zVIEW vXSLT )
{
   zCHAR  szTag[ 33 ];
   zCHAR  szText[ 256 ];
   zPCHAR pch;
   zSHORT nCnt;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "Control", "Tag" );
      if ( szTag[ 0 ] == 0   ||
          (szTag[ 0 ] == 'T' &&
           szTag[ 1 ] == 'e' &&
           szTag[ 2 ] == 'x' &&
           szTag[ 3 ] == 't') )
      {
         GetStringFromAttribute( szText, zsizeof( szText ), vXSLT, "Control", "Text" );
         if ( szText[ 0 ] )
         {
            szText[ 32 ] = 0;
            strcpy_s( szTag, zsizeof( szTag ), szText );
         }
         else
         {
            szText[ 0 ] = 0;
            if ( CheckExistenceOfEntity( vXSLT, "CtrlMap" ) == 0 &&
                 CheckExistenceOfEntity( vXSLT, "CtrlMapLOD_Attribute" ) == 0 &&
                 CheckExistenceOfEntity( vXSLT, "CtrlMapER_Attribute" ) == 0 )
            {
               GetStringFromAttribute( szText, zsizeof( szText ), vXSLT,
                                       "CtrlMapER_Attribute", "Name" );
            }
         }

         // Remove embedded spaces.
         while ( (pch = zstrchr( szText, ' ' )) != 0 )
            strcpy( pch, pch + 1 );

         if ( szText[ 0 ] )
         {
            // Give the tag a prefix.
            GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "ControlDef", "Tag" );
            nCnt = fnGetTagPrefix( szTag );
            if ( nCnt )
               szText[ 32 - nCnt ] = 0;

            strcpy_s( szTag + nCnt, zsizeof( szTag + nCnt ), szText );
            SetAttributeFromString( vXSLT, "Control", "Tag", szTag );
         }
      }

      SetViewToSubobject( vXSLT, "CtrlCtrl" );
      fnRecurseChangeDlgTags( vXSLT );
      ResetViewFromSubobject( vXSLT );

      nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ChangeDlgTags
//
// PURPOSE:  This function changes the (Text) tags if there is a text
//           or an attribute set for the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ChangeDlgTags( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vXSLT;

   // Get the view.
   GetViewByName( &vControl, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   CreateViewFromView( &vXSLT, vControl );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
      // nothing to do
   }

   // Loop through all Controls.
   fnRecurseChangeDlgTags( vXSLT );

   DropView( vXSLT );

   // This has to be done because the control is mapped after this operation.
   while ( ResetViewFromSubobject( vControl ) == 0 )
   {
      // nothing to do
   }


   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: TZXSLTDD_ProfileSave
//
// PURPOSE:   Save updated Profile.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ProfileSave( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "EMD", vSubtask, zCURRENT_OI );
   oTZ__PRFO_CommitProfile( vProfileXFER );

   return( 0 );
}

void
fnCreateMappingEntities( zVIEW vSubtask, zVIEW vXSLT )
{
// zPCHAR   pchLOD_Name;
// zPCHAR   pchEntityName;
// zPCHAR   pchAttributeName;
   zVIEW    vLOD;
   zVIEW    vPainter;
   zVIEW    vTZZOVEAO;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &vPainter, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   if ( GetCtrlState( vPainter, "ControlBar", zCONTROL_STATUS_VISIBLE ) <= 0 ||
        GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      return;
   }

   TraceLineS( "In fnCreateMappingEntities",
               " ============== DOING WORK ==============" );

   // Drop the previous instance if it exists.
// DropObjectInstance( vTZZOVEAO );  // no ... drop it when View list changes

   ActivateEmptyObjectInstance( &vTZZOVEAO, "TZZOVEAO", vSubtask, zMULTIPLE );
   SetNameForView( vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );

   // Activate the LOD for the current ViewObjRef subobject.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   if ( vXSLT )
   {
      nRC = SetCursorFirstEntity( vXSLT, "ViewObjRef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTZZOVEAO, "ViewObjRef", zPOS_NEXT );
         SetMatchingAttributesByName( vTZZOVEAO, "ViewObjRef",
                                      vXSLT, "ViewObjRef", zSET_ALL );
         CreateEntity( vTZZOVEAO, "LOD", zPOS_NEXT );
         SetMatchingAttributesByName( vTZZOVEAO, "LOD",
                                      vXSLT, "LOD", zSET_ALL );
      // GetAddrForAttribute( &pchLOD_Name, vXSLT, "ViewObjRef", "Name" );
         GetIntegerFromAttribute( &lZKey, vXSLT, "LOD", "ZKey" );
         SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                        "CPLR_ZKey", lZKey, "" );
         ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
         nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", 0 );
         while ( nRC >= zCURSOR_SET )
         {
         // GetAddrForAttribute( &pchEntityName, vLOD, "LOD_Entity", "Name" );
         // TraceLine( "CreateMappingEntities LOD: %s   Entity Name: %s",
         //            pchLOD_Name, pchEntityName );
            CreateEntity( vTZZOVEAO, "LOD_Entity", zPOS_NEXT );
            SetMatchingAttributesByName( vTZZOVEAO, "LOD_Entity",
                                         vLOD, "LOD_Entity", zSET_ALL );
            nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", 0 );
            while ( nRC >= zCURSOR_SET )
            {
            // GetAddrForAttribute( &pchAttributeName, vLOD, "LOD_Entity", "Name" );
            // TraceLine( "CreateMappingEntities LOD: %s   Entity: %s   Attribute: %s",
            //            pchLOD_Name, pchEntityName, pchAttributeName );
               CreateEntity( vTZZOVEAO, "LOD_Attribute", zPOS_NEXT );
               SetMatchingAttributesByName( vTZZOVEAO, "LOD_Attribute",
                                            vLOD, "LOD_Attribute", zSET_ALL );

               CreateEntity( vTZZOVEAO, "ER_Attribute", zPOS_NEXT );
               SetMatchingAttributesByName( vTZZOVEAO, "ER_Attribute",
                                            vLOD, "ER_Attribute", zSET_ALL );

               nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", 0 );
            }

            nRC = SetCursorNextEntity( vLOD, "LOD_Entity", 0 );
         }

         DropObjectInstance( vLOD );
         nRC = SetCursorNextEntity( vXSLT, "ViewObjRef", 0 );
      }

      SetCursorFirstEntity( vXSLT, "ViewObjRef", 0 );
   }

   RefreshCtrl( vPainter, "View" );
// MiSetInstanceUpdateFlag( vXSLT, 0 );

// DisplayObjectInstance( vTZZOVEAO, 0, "LOD_Entity" );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AlignLeft
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AlignLeft( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_LEFT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_AlignLeft

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AlignRight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AlignRight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_RIGHT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_AlignRight

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AlignTop
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AlignTop( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_TOP, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_AlignTop

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AlignBottom
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AlignBottom( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_BOTTOM, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_AlignBottom

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EqualHeight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_EqualHeight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_HEIGHT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_EqualHeight

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EqualWidth
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_EqualWidth( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_WIDTH, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_EqualWidth

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EqualWidthHeight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_EqualWidthHeight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_WIDTH_HEIGHT, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_EqualWidthHeight

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EquiSpaceVertical
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_EquiSpaceVertical( zVIEW vSubtask )
{
   fnPainterCall( zMSG_EQUAL_SPACE_VERTICAL, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_EquiSpaceVertical

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EquiSpaceHorizontal
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_EquiSpaceHorizontal( zVIEW vSubtask )
{
   fnPainterCall( zMSG_EQUAL_SPACE_HORIZONTAL, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_EquiSpaceHorizontal

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AbutVertical
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AbutVertical( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ABUT_VERTICAL, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_AbutVertical

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AbutHorizontal
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_AbutHorizontal( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ABUT_HORIZONTAL, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_AbutHorizontal

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteControls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_DeleteControls( zVIEW vSubtask )
{
   fnPainterCall( zMSG_DELETE_SELECTED, vSubtask, 0, 0, 0 );
   return( 0 );
} // TZXSLTDD_DeleteControls

//./ ADD NAME=RetrieveMetaList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      RetrieveMetaList
//
//  PURPOSE:    To ensure a view to the meta list is available for file open.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RetrieveMetaList( zVIEW vSubtask )
{
   zVIEW vTZXSLTS;
   zVIEW vTZXSLTL;

   // Obtain list of XSLT's and position on the current XSLT if there is one active.
   if ( GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vTZXSLTS, zREFER_XSLT_META );
      SetNameForView( vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZXSLTL, "XSLT" ) >= zCURSOR_SET )
   {
      // If XSLT open, then set cursor to it.
      SetCursorFirstEntityByAttr( vTZXSLTS, "W_MetaDef", "Name",
                                  vTZXSLTL, "XSLT", "Tag", "" );
   }
   else
   {
      OrderEntityForView( vTZXSLTS, "W_MetaDef", "LastUpdateDate D" );
      SetCursorFirstEntity( vTZXSLTS, "W_MetaDef", "" );
   }

// MessageBox( 0, "Check cursor position of TZXSLTS", "TZXSLTDD.C", MB_OK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_Check_CheckoutStatus
//
//    set check out state in window title
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_Check_CheckoutStatus( zVIEW vSubtask )
{
   zVIEW   vTZXSLTS;
   zSHORT  nEnable = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZXSLTS, "W_MetaDef" ) >= zCURSOR_SET )
   {
         nEnable = CheckOutStateForCurrentWindow( vSubtask, vTZXSLTS );
         if ( nEnable )
            bEnable = TRUE;
   }

   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteXSLT", bEnable );

   return( 0 );
} // TZXSLTDD_Check_CheckoutStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_CheckNameForCheckOut
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_CheckNameForCheckOut( zVIEW vSubtask )
{
   zSHORT  nEnable = 1;
   zVIEW   vTZXSLTS;
   zVIEW   vTZXSLTS_Copy;
   zCHAR   szNewName[ 33 ];
   zCHAR   szOutName[ 33 ];

   GetCtrlText( vSubtask, "edXSLTName", szNewName, 33 );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   if ( GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vTZXSLTS_Copy, vTZXSLTS, 0 );

   if ( SetCursorFirstEntityByString( vTZXSLTS_Copy, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      // set check out state in Title
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vTZXSLTS_Copy );
   }
   else
   {
      SetWindowCaptionTitle( vSubtask, "Save XSLT as", "" );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );

   DropView( vTZXSLTS_Copy );

   return( 0 );
} // TZXSLTDD_CheckNameForCheckOut

zSHORT
zwfnTZXSLTDD_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vXSLTs;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTs, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "LXSLT Maintenance",
                               "TZXSLTL", vXSLTs, "XSLT", zSOURCE_XSLT_META );

   // if XSLT not checked out and user change this XSLT, then zeidon
   // call the window "Save XSLT as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new XSLT or Open an other XSLT or Switch Project or Exit
   // XSLT0 Tool)

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                            zNoActionAfterSaveAs );

   switch ( lActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new XSLT
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZXSLTDD", "DLG_NEW" );
         break;

      case zOpenComponentAfterSaveAs:   // Open XSLT
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZXSLTDD", "XSLT_OPEN" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit XSLT Tool
         ExitXSLT( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SetScrollbars( zVIEW vSubtask )
{
   zLONG lState = GetOptionState( vSubtask, "ScrollBars",
                                  zOPTION_STATUS_CHECKED );
   lState = !lState;
   SetOptionState( vSubtask, "ScrollBars", zOPTION_STATUS_CHECKED, lState );
   fnPainterCall( zMSG_SETSCROLLBARS, vSubtask, 0, (zPCHAR) lState, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SetGrid( zVIEW vSubtask )
{
   zLONG lState = GetOptionState( vSubtask, "Grid",
                                  zOPTION_STATUS_CHECKED );
   lState = !lState;
   SetOptionState( vSubtask, "Grid", zOPTION_STATUS_CHECKED, lState );
   fnPainterCall( zMSG_SETGRID, vSubtask, 0, (zPCHAR) lState, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
InitMenu( zVIEW vSubtask )
{
   zCHAR   szLabel[ 64 ];
   zBOOL   bVisible;

   SetDrawMenuBar( vSubtask, FALSE );

   bVisible = (zBOOL) GetCtrlState( vSubtask, "ControlBar",
                                    zCONTROL_STATUS_VISIBLE );
   SetOptionState( vSubtask, "ShowHideControlBar", zOPTION_STATUS_TEXT,
                   (zULONG) (bVisible ? "Hide Control Bar" : "Show Control Bar") );

   if ( TZXSLTDD_InitMenuItems( vSubtask ) < 0 )
   {
      SetDrawMenuBar( vSubtask, TRUE );
      return( -1 );
   }

   if ( fnPainterCall( zMSG_UNDO, vSubtask, 0, szLabel, zsizeof( szLabel ) ) == 0 )
   {
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_TEXT, (zULONG) szLabel );
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 1 );
   }
   else
   {
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_TEXT, (zULONG) "&Undo\tCtrl+Z" );
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 0 );
   }

   if ( fnPainterCall( zMSG_REDO, vSubtask, 0, szLabel, zsizeof( szLabel ) ) == 0 )
   {
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_TEXT, (zULONG) szLabel );
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 1 );
   }
   else
   {
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_TEXT, (zULONG) "&Redo\tCtrl+Y" );
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 0 );
   }

   if ( fnPainterCall( zMSG_GETTABBING, vSubtask, 0, 0, 0 ) == 0 )
   {
      SetOptionState( vSubtask, "ShowHide", zOPTION_STATUS_TEXT, (zULONG) "Show &Tabstops" );
   }
   else
   {
      SetOptionState( vSubtask, "ShowHide", zOPTION_STATUS_TEXT, (zULONG) "Hide &Tabstops" );
   }

   SetOptionState( vSubtask, "DeleteControls", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AlignLeft", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AlignRight", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AlignTop", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AlignBottom", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "EqualHeight", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "EqualWidth", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "EqualWidthHeight", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "EquiSpaceVertical", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "EquiSpaceHorizontal", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AbutVertical", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vSubtask, "AbutHorizontal", zOPTION_STATUS_ENABLED, 1 );

   if ( fnPainterCall( zMSG_CLIPBOARD_COPY, vSubtask, 0, szLabel, zsizeof( szLabel ) ) == 0 )
      SetOptionState( vSubtask, "Copy", zOPTION_STATUS_ENABLED, 1 );
   else
      SetOptionState( vSubtask, "Copy", zOPTION_STATUS_ENABLED, 0 );

   if ( fnPainterCall( zMSG_CLIPBOARD_PASTE, vSubtask, 0, szLabel, zsizeof( szLabel ) ) == 0 )
      SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 1 );
   else
      SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 0 );

   if ( fnPainterCall( zMSG_CLIPBOARD_PASTE_MENU, vSubtask, 0, szLabel, zsizeof( szLabel ) ) == 0 )
      SetOptionState( vSubtask, "PasteMenu", zOPTION_STATUS_ENABLED, 1 );
   else
      SetOptionState( vSubtask, "PasteMenu", zOPTION_STATUS_ENABLED, 0 );

   SetDrawMenuBar( vSubtask, TRUE );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
WindowListClose( zVIEW vSubtask )
{
   zVIEW vMainWindow;

   if ( GetViewByName( &vMainWindow, "TZPNMWIN", vSubtask, zLEVEL_TASK ) >= zCURSOR_SET )
   {
      // set Check-Status for WindowList
   // SetOptionState( vMainWindow, "WindowList", zOPTION_STATUS_CHECKED, 0 );
   }

   return( 0 );
}

//./ ADD NAME=DeleteXSLT
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteXSLT
//
//  PURPOSE:    To delete a dialog file using the config. mgmt. function
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteXSLT( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );

   /* If there are no W_MetaDefs then one can't be opened. */
   if ( CheckExistenceOfEntity( vMetaList, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DIALOG,
                      zREFER_XSLT_META, vMetaList, "W_MetaDef",
                      "Name", 0 ) == zRESPONSE_YES )
   {
      nRC = DeleteMetaOI( vSubtask, vMetaList, zREFER_XSLT_META );
      SetCursorPrevEntity( vMetaList, "W_MetaDef", "" );
   }

   return( 0 );
}

//./ ADD NAME=SortByName
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SortByName
//
//  PURPOSE:    To sort the dialog list by Name.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SortByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vMetaList,
                                   "W_MetaDef", "Name", "" );

   return( 0 );
}

//./ ADD NAME=SortByDate
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SortByDate
//
//  PURPOSE:    To delete a dialog file using the config. mgmt. function
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SortByDate( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vMetaList,
                                   "W_MetaDef", "LastUpdateDate", "" );

   return( 0 );
}

//./ ADD NAME=SortByDesc
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SortByDesc
//
//  PURPOSE:    To delete a dialog file using the config. mgmt. function
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SortByDesc( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vMetaList,
                                   "W_MetaDef", "Desc", "" );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PrebuildWindowList( zVIEW vSubtask )
{
   zVIEW vProfileXFER;

   SetNameForView( vSubtask, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK ) > 0 )
      RestoreWindowPlacement( vSubtask, vProfileXFER );

   return( 0 );
}

//./ ADD NAME=DisplayWindowList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SetWindowList( zVIEW vSubtask )
{
   zVIEW vWindowList;

   // is the window already created ?
   if ( GetViewByName( &vWindowList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) < 1 )
   {
      // set Check-Status for WindowList
   // SetOptionState( vSubtask, "WindowList", zOPTION_STATUS_CHECKED, 1 );

      // Start the window list modeless window
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
                               "TZXSLTDD", "WND_LIST" );
   }
   else
   {
      // is the window at this time hidden ?
      if ( GetWindowState( vWindowList, zWINDOW_STATUS_VISIBLE ) > 0 )
      {
         // reset Check-Status for WindowList
      // SetOptionState( vSubtask, "WindowList", zOPTION_STATUS_CHECKED, 0 );

         // Hide the Window
         SetWindowState( vWindowList, zWINDOW_STATUS_VISIBLE, 0 );
      }
      else
      {
         // reset Check-Status for WindowList
      // SetOptionState( vSubtask, "WindowList", zOPTION_STATUS_CHECKED, 1 );

         // show the Window
         SetWindowState( vWindowList, zWINDOW_STATUS_VISIBLE, 1 );
      }
   }

   return( 0 );
}

//./ ADD NAME=SetDefaultWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SetDefaultWindow( zVIEW vSubtask )
{
   zVIEW vXSLT;

   if ( GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vXSLT, "Window" ) == zCURSOR_SET )
      {
         if ( SetCursorFirstEntity( vXSLT, "DfltWnd", 0 ) == zCURSOR_SET )
            ExcludeEntity( vXSLT, "DfltWnd", zREPOS_NONE );

         IncludeSubobjectFromSubobject( vXSLT, "DfltWnd",
                                        vXSLT, "Window", zPOS_AFTER );
      }
   }
   return( 0 );
}

//./ ADD NAME=ClearDefaultWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ClearDefaultWindow( zVIEW vSubtask )
{
   zVIEW vXSLT;

   if ( GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( SetCursorFirstEntity( vXSLT, "DfltWnd", 0 ) == zCURSOR_SET )
         ExcludeEntity( vXSLT, "DfltWnd", zREPOS_NONE );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_CreatePanelList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZXSLTDD_CreatePanelList( zVIEW vSubtask,
                          zVIEW vNewXSLT,
                          zVIEW vNewXSLTL )
{

#ifndef __WIN32__
   zVIEW       vPanelList;
#endif

   // If the window list modeless window is up, simply fill in the windows
   // for the list, otherwise display the first window in the dialog
#ifndef __WIN32__
   if ( GetViewByName( &vPanelList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) > 0 )
   {
      RefreshCtrl( vPanelList, "ListBox1" );
      ActivatePanel( vPanelList );

      // If only one window exists in the dialog file, paint it
      if ( SetCursorFirstEntity( vNewXSLTL, "Panel", 0 ) == zCURSOR_SET &&
           SetCursorNextEntity( vNewXSLTL, "Panel", 0 ) != zCURSOR_SET )
      {
         // Paint the first window in the dialog
         SetNameForView( vNewXSLTL, "TZXSLT", vSubtask, zLEVEL_TASK );
         fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
      }
      else
      {
         SetWindowState( vPanelList, zWINDOW_STATUS_VISIBLE, TRUE );
      }
   }
   else
   {
      // Paint the first window in the dialog
      fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }
#else
   // If only one window exists in the dialog file, paint it
   if ( SetCursorFirstEntity( vNewXSLTL, "Panel", 0 ) == zCURSOR_SET &&
        SetCursorNextEntity( vNewXSLTL, "Panel", 0 ) != zCURSOR_SET )
   {
      // Paint the first window in the dialog
      SetNameForView( vNewXSLTL, "TZXSLT", vSubtask, zLEVEL_TASK );
      fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }
   else
   {
      zCHAR  szDlgTag[ 33 ];
      zCHAR  szWndTag[ 33 ];

      GetStringFromAttribute( szDlgTag, zsizeof( szDlgTag ), vNewXSLT, "XSLT", "Tag" );
      GetProfileStr( vSubtask, "Design", szDlgTag, "..Start",
                     "Panel", szWndTag, zsizeof( szWndTag ) );
      if ( szWndTag[ 0 ] &&
           SetCursorFirstEntityByString( vNewXSLTL, "Panel", "Tag",
                                         szWndTag, "" ) == zCURSOR_SET )
      {
         // Paint the first window in the dialog
         SetNameForView( vNewXSLTL, "TZXSLT", vSubtask, zLEVEL_TASK );
         fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
      }
   }

   fnPainterCall( zMSG_REFRESHWINDOWLIST, vSubtask, 0, 0, 0 );
#endif

   return( 0 );
} // TZXSLTDD_CreatePanelList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_GenerateXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_GenerateXSLT( zVIEW vSubtask )
{
   zVIEW   vMetaList = 0;
   zVIEW   vTaskLPLR = 0;
   zVIEW   vXSLT = 0;
   zVIEW   vXRA = 0;
   zCHAR   szXSLTName[ 33 ];
   zCHAR   szMsg[ zMAX_FILESPEC_LTH + 100 ];
   zCHAR   szRemotePath[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR   szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT  nRC;

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   SysReadZeidonIni( -1, "[Workstation]", "ExecutableRemotePath", szRemotePath, zsizeof( szRemotePath ) );
   if ( szRemotePath[ 0 ] == 0 )
      GetStringFromAttribute( szRemotePath, zsizeof( szRemotePath ), vTaskLPLR, "LPLR", "ExecDir" );

   ofnTZCMWKSO_AppendSlash( szRemotePath );

   CreateViewFromViewForTask( &vMetaList, vTaskLPLR, 0 );
   RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_XSLT_META );

   for ( nRC = SetCursorFirstEntity( vMetaList, "W_MetaDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMetaList, "W_MetaDef", "" ) )
   {
      GetStringFromAttribute( szXSLTName, zsizeof( szXSLTName ), vMetaList, "W_MetaDef", "Name" );
      strcpy_s( szMsg, zsizeof( szMsg ), "Generate XSLT for: '" );
      strcat_s( szMsg, zsizeof( szMsg ), szXSLTName );
      strcat_s( szMsg, zsizeof( szMsg ), "'." );
      MB_SetMessage( vSubtask, 1, szMsg );

      if ( ActivateMetaOI( vSubtask, &vXSLT, vMetaList,
                           zREFER_XSLT_META, zSINGLE | zLEVEL_TASK ) != 1 )

      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Could not load XSLT '" );
         strcat_s( szMsg, zsizeof( szMsg ), szXSLTName );
         strcat_s( szMsg, zsizeof( szMsg ), "'." );
         MessageSend( vSubtask, "PN00219", "XSLT Maintenance", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         strcpy_s( szFileSpec, zsizeof( szFileSpec ), szRemotePath );
         strcat_s( szFileSpec, zsizeof( szFileSpec ), szXSLTName );
         strcat_s( szFileSpec, zsizeof( szFileSpec ), ".XRA" );

      //xConvertXSLT( vMetaList, vXSLT, "", &vXRA );

         if ( vXRA )
         {
            if ( CommitOI_ToFile( vXRA, szFileSpec, zSINGLE ) != 0 )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XRA '" );
               strcat_s( szMsg, zsizeof( szMsg ), szRemotePath );
               strcat_s( szMsg, zsizeof( szMsg ), "'." );
               MessageSend( vSubtask, "CM00428", "XSLT Maintenance",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            }
            DropObjectInstance( vXRA );
         }

         DropMetaOI( vSubtask, vXSLT );
      }
   } // endfor

   DropView( vMetaList );
   MB_SetMessage( vSubtask, 1, "" );

    return( 0 );
} // TZXSLTDD_GenerateXSLT

//./ ADD NAME=SaveXSLTFile
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SaveXSLTFile
//
//  PURPOSE:    To save a dialog file and call the conversion function
//              to create an executable dialog the driver can process
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SaveXSLTFile( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLTConvert;

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   CreateViewFromViewForTask( &vXSLTConvert, vXSLT, 0 );
   AssignZKeyToMetaOI( vSubtask, vXSLTConvert, zSOURCE_XSLT_META, 0 );

   ResetView( vXSLTConvert );

   SetNameForView( vSubtask, "TZCM_XSLTSubtask", vSubtask, zLEVEL_TASK );

   if ( CommitMetaOI( vSubtask, vXSLTConvert, zSOURCE_XSLT_META ) < 0 )
   {
      MessageSend( vSubtask, "PN00219", "XSLT Maintenance",
                   "Unable to save XSLT",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   DropView( vXSLTConvert );

   return( 0 );
}

//./ ADD NAME=SaveAllXSLTs
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SaveAllXSLTs
//
//  PURPOSE:    To save a dialog file and call the conversion function
//              to create an executable dialog the driver can process
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SaveAllXSLTs( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vLPLR;
   zSHORT nRC;

   RetrieveViewForMetaList( vSubtask, &vLPLR, zSOURCE_XSLT_META );
   nRC = SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );
   while ( nRC >= zCURSOR_SET )
   {
      ActivateMetaOI( vSubtask, &vXSLT, vLPLR, zSOURCE_XSLT_META, zSINGLE );
      SetCursorFirstEntity( vXSLT, "Panel", "" );
      CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
      DropView( vXSLT );
      nRC = SetCursorNextEntity( vLPLR, "W_MetaDef", "" );
   }

   return( 0 );
}

//./ ADD NAME=UpdatePanelFromList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UpdatePanelFromList
//
//  PURPOSE:    To update a window from the list of windows
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdatePanelFromList( zVIEW vSubtask )
{
   zVIEW  vXSLTList;
   zCHAR  szTag[ 33 ];
   zSHORT nRC;

   // Get the view to the window list
   if ( GetViewByName( &vXSLTList, "TZXSLTL", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( CheckExistenceOfEntity( vXSLTList, "Panel" ) != zCURSOR_SET )
      return( -1 );

   SetNameForView( vXSLTList, "TZXSLT", vSubtask, zLEVEL_TASK );

   // Get Address of window tag and see if there is an active painter
   // for the window
   fnPainterCall( zMSG_GETACTIVEWINDOWNAME, vSubtask, 0, szTag, zsizeof( szTag ) );
   if ( szTag[ 0 ] == 0 )
      return( -1 );

   nRC = (zSHORT) fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, szTag, zsizeof( szTag ) );
   return( 0 );
}

//./ ADD NAME=SelectPanelFromList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SelectPanelFromList
//
//  PURPOSE:    To Select a window from the list of windows
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SelectPanelFromList( zVIEW vSubtask )
{
   zVIEW  vXSLTList;

   // Get the view to the window list
   if ( GetViewByName( &vXSLTList, "TZXSLTL", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( CheckExistenceOfEntity( vXSLTList, "Panel" ) != zCURSOR_SET ||
        !GetSelectStateOfEntityForSet( vXSLTList, "Panel", 1 ) )
   {
      SetCursorFirstEntity( vXSLTList, "Panel", 0 );
   }

   SetNameForView( vXSLTList, "TZXSLT", vSubtask, zLEVEL_TASK );
   return( 0 );
}


//./ ADD NAME=NewPanel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NewPanel
//
//  PURPOSE:    To create a new window in the painter
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NewPanel( zVIEW vSubtask )
{
   zVIEW  vXSLT;
#ifndef __WIN32__
   zVIEW  vPanelList;
#endif
   zVIEW  vPE;
   zSHORT nIdx, nCursor;
   zCHAR  szTag[ 34 ];
   zPCHAR pchAttr;

   // Get the current open dialog.
   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   // Generate a name for the new window.
   if ( (nCursor = CheckExistenceOfEntity( vXSLT, "Panel" )) !=
                                                            zCURSOR_NULL )
   {
      strcpy_s( szTag, zsizeof( szTag ), "Panel" );
      for ( nIdx = 1; nIdx < 1000; nIdx++ )
      {
         zltoa( (zLONG) nIdx, szTag + 6, zsizeof( szTag ) - 6, 10 );
         if ( SetCursorFirstEntityByString( vXSLT, "Panel", "Tag",
                                            szTag, 0 ) < zCURSOR_SET )
         {
            break;
         }
      }
   }
   else
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "XSLT", "Tag" );
   }

   // If a dialog is currently being processed by the painter, update
   // the current window from the C++ controls to the Zeidon controls
   // and then clean out the painter, create a new window and
   // paint the new window into the painter
   SetCursorLastEntity( vXSLT, "Panel", 0 );
   CreateMetaEntity( vSubtask, vXSLT, "Panel", zPOS_AFTER );
   SetAttributeFromString( vXSLT, "Panel", "Tag", szTag );
   if ( nCursor == zCURSOR_NULL )
   {
      GetAddrForAttribute( &pchAttr, vXSLT, "XSLT", "Desc" );
      if ( pchAttr[ 0 ] )
         SetAttributeFromString( vXSLT, "Panel", "Caption", pchAttr );
      else
         SetAttributeFromString( vXSLT, "Panel", "Caption", szTag );
   }
   else
   {
      SetAttributeFromString( vXSLT, "Panel", "Caption", szTag );
   }

   SetAttributeFromInteger( vXSLT, "Panel", "PSDLG_X", 2L );
   SetAttributeFromInteger( vXSLT, "Panel", "PSDLG_Y", 2L );
   SetAttributeFromInteger( vXSLT, "Panel", "SZDLG_X", 280L );
   SetAttributeFromInteger( vXSLT, "Panel", "SZDLG_Y", 128L );

   if ( nCursor == zCURSOR_NULL )
   {
      SetCursorFirstEntityByString( vPE, "WindowStyle", "Tag",
                                    "Primary Window", 0 );
   }
   else
   {
      SetCursorFirstEntityByString( vPE, "WindowStyle", "Tag",
                                    "XSLT Box", 0 );
   }

   if ( CheckExistenceOfEntity( vPE, "WindowStyle" ) > zCURSOR_NULL )
   {
      IncludeSubobjectFromSubobject( vXSLT, "WndStyle",
                                     vPE, "WindowStyle",
                                     zPOS_BEFORE );
   }

   // Add default actions for OK and Cancel.
   if ( OperatorPrompt( vSubtask, "New Window",
                        "Do you want default buttons and actions created?",
                        1, zBUTTONS_YESNO,
                        zRESPONSE_YES, zICON_QUESTION ) == zRESPONSE_YES )
   {
      CreateMetaEntity( vSubtask, vXSLT, "Action", zPOS_AFTER );
      SetAttributeFromString( vXSLT, "Action", "Tag", "Alt-F4" );
      SetAttributeFromInteger( vXSLT, "Action", "Type", 72 );
      CreateMetaEntity( vSubtask, vXSLT, "Control", zPOS_AFTER );
      SetAttributeFromString( vXSLT, "Control", "Tag", "OK" );
      SetAttributeFromString( vXSLT, "Control", "Text", "OK" );
      SetAttributeFromInteger( vXSLT, "Control", "PSDLG_X", 230 );
      SetAttributeFromInteger( vXSLT, "Control", "PSDLG_Y", 12 );
      SetAttributeFromInteger( vXSLT, "Control", "SZDLG_X", 40 );
      SetAttributeFromInteger( vXSLT, "Control", "SZDLG_Y", 12 );
      SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", 1020, "" );
      IncludeSubobjectFromSubobject( vXSLT, "ControlDef",
                                     vPE,     "ControlDef", zPOS_AFTER );
      CreateMetaEntity( vSubtask, vXSLT, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vXSLT, "Event", "Type", 1 );
      IncludeSubobjectFromSubobject( vXSLT, "EventAct",
                                     vXSLT, "Action", zPOS_AFTER );

      CreateMetaEntity( vSubtask, vXSLT, "Action", zPOS_AFTER );
      SetAttributeFromString( vXSLT, "Action", "Tag", "ESC" );
      SetAttributeFromInteger( vXSLT, "Action", "Type", 71 );
      SetAttributeFromString( vXSLT, "Action", "NoMap", "Y" );
      CreateMetaEntity( vSubtask, vXSLT, "Control", zPOS_AFTER );
      SetAttributeFromString( vXSLT, "Control", "Tag", "Cancel" );
      SetAttributeFromString( vXSLT, "Control", "Text", "Cancel" );
      SetAttributeFromInteger( vXSLT, "Control", "PSDLG_X", 230 );
      SetAttributeFromInteger( vXSLT, "Control", "PSDLG_Y", 27 );
      SetAttributeFromInteger( vXSLT, "Control", "SZDLG_X", 40 );
      SetAttributeFromInteger( vXSLT, "Control", "SZDLG_Y", 12 );
      SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", 1020, "" );
      IncludeSubobjectFromSubobject( vXSLT, "ControlDef",
                                     vPE,     "ControlDef", zPOS_AFTER );
      CreateMetaEntity( vSubtask, vXSLT, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vXSLT, "Event", "Type", 1 );
      IncludeSubobjectFromSubobject( vXSLT, "EventAct",
                                     vXSLT, "Action", zPOS_AFTER );
   }

   // Call the painter to create a new window
   SetNameForView( vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

#ifndef __WIN32__
   if ( GetViewByName( &vPanelList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vPanelList, "ListBox1" );
#endif

   fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );

   GetViewByName( &vXSLT, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   RefreshWindow( vXSLT );
   GetViewByName( &vXSLT, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK );
   RefreshWindow( vXSLT );
   return( 0 );
}

// Recursive function for copying controls
void
fnRecursProcessControls( zVIEW vSubtask, zVIEW vXSLT, zVIEW vPanelList )
{
   // Create the control entity
   CreateMetaEntity( vSubtask, vXSLT, "Control", zPOS_AFTER );
   SetMatchingAttributesByName( vXSLT, "Control",
                                vPanelList, "Control",
                                zSET_NULL | zSET_NOTNULL );

   // Copy CtrlMap from source to target
   if ( SetCursorFirstEntity( vPanelList, "CtrlMap", 0 ) == zCURSOR_SET )
   do
   {
      CreateMetaEntity( vSubtask, vXSLT, "CtrlMap", zPOS_AFTER );
      SetMatchingAttributesByName( vXSLT, "CtrlMap",
                                   vPanelList, "CtrlMap",
                                   zSET_NULL | zSET_NOTNULL );
   }  while ( SetCursorNextEntity( vPanelList, "CtrlMap", 0 ) ==
                                                               zCURSOR_SET );

   // NOTE, Once we start including E/R information and domain information
   // under the mapping entity, we must do INCLUDES here for those
   // subordinate entities.

   // Copy Events under the Ctrl from source to target
   if ( SetCursorFirstEntity( vPanelList, "Event", 0 ) == zCURSOR_SET )
   do
   {
      CreateTemporalMetaEntity( vSubtask, vXSLT, "Event", zPOS_AFTER );
      SetMatchingAttributesByName( vXSLT, "Event",
                                   vPanelList, "Event",
                                   zSET_NULL | zSET_NOTNULL );
      if ( CheckExistenceOfEntity( vPanelList, "EventAct" ) == zCURSOR_SET )
      {
         CreateTemporalMetaEntity( vSubtask, vXSLT, "EventAct", zPOS_AFTER );
         SetMatchingAttributesByName( vXSLT, "EventAct",
                                      vPanelList, "EventAct",
                                      zSET_NULL | zSET_NOTNULL );
      }
   }  while ( SetCursorNextEntity( vPanelList, "Event", 0 ) ==
                                                               zCURSOR_SET );

   // Copy DIL_Msg from source to target
// Commented out by Don C 2/11/93
// if ( SetCursorFirstEntity( vPanelList, "CtrlDIL_Msg", 0 ) == zCURSOR_SET )
// do
// {
//    CreateMetaEntity( vSubtask, vXSLT, "CtrlDIL_Msg", zPOS_AFTER );
//    SetMatchingAttributesByName( vXSLT, "CtrlDIL_Msg",
//                                 vPanelList, "CtrlDIL_Msg",
//                                 zSET_NULL | zSET_NOTNULL );
// }  while ( SetCursorNextEntity( vPanelList, "CtrlDIL_Msg", 0 ) ==
//                                                           zCURSOR_SET );

   // Recursively call ourself for all sub-controls
   if ( SetCursorFirstEntity( vPanelList, "CtrlCtrl", 0 ) == zCURSOR_SET )
   {
      SetViewToSubobject( vPanelList, "CtrlCtrl" );
      SetViewToSubobject( vXSLT, "CtrlCtrl" );
      do
      {
         fnRecursProcessControls( vSubtask, vXSLT, vPanelList );
      }  while ( SetCursorNextEntity( vPanelList, "Control", 0 ) ==
                                                               zCURSOR_SET );
      ResetViewFromSubobject( vPanelList );
      ResetViewFromSubobject( vXSLT );
   }
}

// Recursive function for copying options
void
fnRecursProcessOptions( zVIEW vSubtask, zVIEW vXSLT, zVIEW vPanelList )
{
   // Create the option entity
   CreateMetaEntity( vSubtask, vXSLT, "Option", zPOS_AFTER );
   SetMatchingAttributesByName( vXSLT, "Option",
                                vPanelList, "Option",
                                zSET_NULL | zSET_NOTNULL );

   // Copy OptAct from source to target
   if ( SetCursorFirstEntity( vPanelList, "OptAct", 0 ) == zCURSOR_SET )
   do
   {
      CreateMetaEntity( vSubtask, vXSLT, "OptAct", zPOS_AFTER );
      SetMatchingAttributesByName( vXSLT, "OptAct",
                                   vPanelList, "OptAct",
                                   zSET_NULL | zSET_NOTNULL );
   }  while ( SetCursorNextEntity( vPanelList, "OptAct", 0 ) ==
                                                               zCURSOR_SET );

   // Copy OptMap from source to target
   if ( SetCursorFirstEntity( vPanelList, "OptMap", 0 ) == zCURSOR_SET )
   do
   {
      CreateMetaEntity( vSubtask, vXSLT, "OptMap", zPOS_AFTER );
      SetMatchingAttributesByName( vXSLT, "OptMap",
                                   vPanelList, "OptMap",
                                   zSET_NULL | zSET_NOTNULL );
   }  while ( SetCursorNextEntity( vPanelList, "OptMap", 0 ) ==
                                                               zCURSOR_SET );

   // NOTE, Once we start including E/R information and domain information
   // under the mapping entity, we must do INCLUDES here for those
   // subordinate entities.

   // Recursively call ourself for all sub-options
   if ( SetCursorFirstEntity( vPanelList, "OptOpt", 0 ) == zCURSOR_SET )
   {
      SetViewToSubobject( vPanelList, "OptOpt" );
      SetViewToSubobject( vXSLT, "OptOpt" );
      do
      {
         fnRecursProcessOptions( vSubtask, vXSLT, vPanelList );
      }  while ( SetCursorNextEntity( vPanelList, "Option", 0 ) ==
                                                               zCURSOR_SET );
      ResetViewFromSubobject( vPanelList );
      ResetViewFromSubobject( vXSLT );
   }
}

#if 0
//./ ADD NAME=DlgCloneWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DlgCloneWindow
//
//  PURPOSE:    To clone an existing window in the painter
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DlgCloneWindow( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vPanelList;
   zSHORT nIdx;
   zCHAR  szTag[ 34 ];

   // Get the current open dialog.
   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   // Get a cursor to the window to be cloned
   GetViewByName( &vPanelList, "TZXSLTL", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vXSLT, "Window" ) == zCURSOR_NULL )
      return( -1 );

   SetCursorLastEntity( vXSLT, "Window", 0 );
   oTZWDLGSO_CloneWindow( vXSLT, vPanelList, vSubtask, vSubtask );

   // Generate a name for the new window
   strcpy_s( szTag, zsizeof( szTag ), "Window" );
   for ( nIdx = 1; nIdx < 1000; nIdx++ )
   {
      zltoa((zLONG) nIdx, szTag + 6, zsizeof( szTag ) - 6, 10 );
      if ( SetCursorFirstEntityByString( vPanelList, "Window", "Tag",
                                         szTag, 0 ) < zCURSOR_SET )
      {
         break;
      }
   }

   SetAttributeFromString( vXSLT, "Window", "Tag", szTag );
#ifndef __WIN32__
   if ( GetViewByName( &vPanelList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vPanelList, "ListBox1" );
#endif

   return( (zSHORT) fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, szTag, zsizeof( szTag ) ) );
}
#endif

//./ ADD NAME=CleanupActions
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CleanupActions
//
//  PURPOSE:    This function lists all actions for the dialog and
//              offers the opportunity to delete the action if it
//              is unreferenced.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CleanupActions( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vWindow;

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zCHAR szTempString[ 256 ];
      zSHORT nLth1;
      zSHORT nLth2;
      zSHORT nRC;

      strcpy_s( szTempString, zsizeof( szTempString ), "Action unreferenced XSLT.Window.Action: " );
      nLth1 = (zSHORT) zstrlen( szTempString );
      GetStringFromAttribute( szTempString + nLth1, zsizeof( szTempString ) - nLth1,
                              vXSLT, "XSLT", "Tag" );
      nLth1 += (zSHORT) zstrlen( szTempString + nLth1 );
      szTempString[ nLth1++ ] = '.';

      CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );
      nRC = SetCursorFirstEntity( vXSLT, "Action", "XSLT" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vXSLT, "ActWndEvent" ) == 0 ||
              CheckExistenceOfEntity( vXSLT, "ActEvent" ) == 0 ||
              CheckExistenceOfEntity( vXSLT, "ActOpt" ) == 0   ||
              CheckExistenceOfEntity( vXSLT, "ActHot" ) == 0 )
         {
            nRC = SetCursorNextEntity( vXSLT, "Action", "XSLT" );
            continue;
         }

         GetStringFromAttribute( szTempString + nLth1, zsizeof( szTempString ) - nLth1,
                                 vXSLT, "Window", "Tag" );
         nLth2 = (zSHORT) zstrlen( szTempString + nLth1 );
         szTempString[ nLth1 + nLth2++ ] = '.';
         GetStringFromAttribute( szTempString + nLth1 + nLth2, zsizeof( szTempString ) - nLth1 + nLth2,
                                 vXSLT, "Action", "Tag" );
         strcat_s( szTempString + nLth1 + nLth2, zsizeof( szTempString ) - nLth1 - nLth2, "\n Delete this action?" );
         nRC = MessagePrompt( vSubtask, "PE002",
                              "XSLT Maintenance",
                              szTempString,
                              zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_NO,  0 );
         if ( nRC == zRESPONSE_CANCEL )
         {
            break;
         }
         else
         if ( nRC == zRESPONSE_YES )
         {
            DeleteEntity( vXSLT, "Action", zREPOS_NONE );

            // Refresh Action List
            if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
               RefreshWindow( vWindow );
         }

         nRC = SetCursorNextEntity( vXSLT, "Action", "XSLT" );
      }
   }

   return( 0 );
}

//./ ADD NAME=CleanupOperations
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CleanupOperations
//
//  PURPOSE:    This function lists all operations for the dialog and
//              offers the opportunity to delete the operation if it
//              is unreferenced.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CleanupOperations( zVIEW vSubtask )
{
   zVIEW vXSLT;

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zCHAR szTempString[ 256 ];
      zSHORT nLth;
      zSHORT nLth2;
      zSHORT nRC;

      strcpy_s( szTempString, zsizeof( szTempString ), "Operation unreferenced in XSLT - " );
      nLth = (zSHORT) zstrlen( szTempString );
      CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );
      nRC = SetCursorFirstEntity( vXSLT, "Operation", "XSLT" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByEntityCsr( vXSLT, "ActOper",
                                                vXSLT, "Operation",
                                                "XSLT" ) >= zCURSOR_SET )
         {
            nRC = SetCursorNextEntity( vXSLT, "Operation", "XSLT" );
            continue;
         }

         GetStringFromAttribute( szTempString + nLth, zsizeof( szTempString ) - nLth,
                                 vXSLT, "Window", "Tag" );
         nLth2 = (zSHORT) zstrlen( szTempString + nLth );
         strcpy_s( szTempString + nLth + nLth2, zsizeof( szTempString ) - nLth - nLth2, " - " );
         nLth2 += 3;
         GetStringFromAttribute( szTempString + nLth + nLth2, zsizeof( szTempString ) - nLth + nLth2, vXSLT, "Operation", "Name" );
         strcat_s( szTempString + nLth + nLth2, zsizeof( szTempString ) - nLth - nLth2, "\n Delete this operation?" );
         if ( MessagePrompt( vSubtask, "PE002",
                             "XSLT Maintenance",
                             szTempString,
                             zBEEP, zBUTTONS_YESNO,
                             zRESPONSE_NO,  0 ) != zRESPONSE_NO )
         {
            DeleteEntity( vXSLT, "Operation", zREPOS_NONE );
         }

         nRC = SetCursorNextEntity( vXSLT, "Operation", "XSLT" );
      }
   }

   return( 0 );
}

//./ ADD NAME=ActionListTransfer
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ActionListTransfer
//
//  PURPOSE:    This function sets the ParentIsSelectAction attribute
//              to "N" to set up proper graying in the LIST_ACT window.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActionListTransfer( zVIEW vSubtask )
{
   zVIEW  vWork;
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   TZXSLTDD_InitModalSubwindow( vSubtask );

   if ( GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vWork, "TZPNCTWO", vSubtask, zSINGLE );
      CreateEntity( vWork, "TZPNCTWO", zPOS_AFTER );
      SetNameForView( vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   }

   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "N" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_CheckXSLTName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZXSLTDD_CheckXSLTName( zVIEW   vSubtask,
                            zVIEW   vPainter,
                            zPCHAR  szXSLTName )
{
   zSHORT  nIdx;
   zVIEW   vMetaList;
   zVIEW   vLOD_LPLR;

   GetStringFromAttribute( szXSLTName, zsizeof( szXSLTName ), vPainter, "Palette", "NewFile" );
   if ( szXSLTName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                   "Must enter in a dialog name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetFocusToCtrl( vSubtask, "EDITTAG" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   // Truncate FileName to 8
   for ( nIdx = 0; nIdx < 8; nIdx++ )
   {
      if ( szXSLTName[ nIdx ] == ' ' )
         szXSLTName[ nIdx ] = '_';
   }
   szXSLTName[ 8 ] = 0;

   // There must be no other XSLT with same name.
   if ( GetViewByName( &vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_XSLT_META );
      SetNameForView( vMetaList, "TZXSLTS", vSubtask, zLEVEL_TASK );
   }

   if ( SetCursorFirstEntityByString( vMetaList, "W_MetaDef", "Name",
                                      szXSLTName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "PN00220", "XSLT Maintenance",
                   "A XSLT by the same name already exists",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "EDITTAG" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   // There must be no LOD with same name.
   RetrieveViewForMetaList( vSubtask, &vLOD_LPLR, zREFER_LOD_META );
   if ( SetCursorFirstEntityByString( vLOD_LPLR,
                                      "W_MetaDef", "Name",
                                      szXSLTName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "PN00221", "Zeidon Object Maintenance",
                   "A LOD with the same name exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "EDITTAG" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   return( 0 );
} // TZXSLTDD_CheckXSLTName



//./ ADD NAME=NewXSLT_File
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NewXSLT_File
//
//  PURPOSE:    To start creating a new dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NewXSLT_File( zVIEW vSubtask )
{
   zVIEW   vNewXSLT;
   zVIEW   vNewXSLTL;
   zVIEW   vNewOptions;
   zVIEW   vPrevXSLT;
   zVIEW   vPainter;
   zCHAR   szXSLTName[ 33 ];

   szXSLTName[ 0 ] = 0;

   // get the C++ class pointer to the painter object from the
   // zeidon control object
   GetViewByName( &vPainter, "TZPNTROO", vSubtask, zLEVEL_TASK );

   if ( TZXSLTDD_CheckXSLTName( vSubtask, vPainter, szXSLTName ) < 0 )
      return( 0 );

   // Declare a view to the appropriate source dialog object type
   ActivateEmptyMetaOI( vSubtask, &vNewXSLT, zSOURCE_XSLT_META,
                        zSINGLE | zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vNewXSLT, "XSLT", zPOS_AFTER );
   SetAttributeFromString( vNewXSLT, "XSLT", "Tag", szXSLTName );
   SetAttributeFromString( vNewXSLT, "XSLT", "DLL_Name",
                           szXSLTName );

   // Now create an empty SourceFile entity by the same name as the XSLT.
   CreateMetaEntity( vSubtask, vNewXSLT, "SourceFile", zPOS_AFTER );
   SetAttributeFromString( vNewXSLT, "SourceFile", "Name", szXSLTName );
   SetAttributeFromString( vNewXSLT, "SourceFile", "Extension", "VML" );
   SetAttributeFromString( vNewXSLT, "SourceFile", "LanguageType", "V" );

   SetAttributeFromAttribute( vNewXSLT, "XSLT", "Desc",
                              vPainter, "Palette", "Desc" );
   SetAttributeFromString( vPainter, "Palette", "OpenedFile", szXSLTName );

   // Clear the painter windows
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );

   // Drop the previous instance being worked on.
   if ( GetViewByName( &vPrevXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevXSLT );

   // Name the view for mapping.
   SetNameForView( vNewXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vNewXSLTL, vNewXSLT, 0 );
   SetNameForView( vNewXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vNewOptions, vNewXSLT, 0 );
   SetNameForView( vNewOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   // Call NewPanel to do the rest
   NewPanel( vSubtask );

   // Include the window just created as the default window.
   IncludeSubobjectFromSubobject( vNewXSLT, "DfltWnd",
                                  vNewXSLT, "Window", zPOS_AFTER );

   return( 0 );
}

//./ ADD NAME=DeleteWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteWindow
//
//  PURPOSE:    To delete a window in the painter
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteWindow( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLT2;
   zCHAR  szTag[ 33 ];
   zCHAR  szMessage[ 100 ];

   fnPainterCall( zMSG_GETACTIVEWINDOWNAME, vSubtask, 0, szTag, zsizeof( szTag ) );
   if ( szTag[ 0 ] == 0 )
      return( -1 );

   // Get the view to the window list
   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   if ( SetCursorFirstEntityByString( vXSLT, "Window",
                                      "Tag", szTag, 0 ) < zCURSOR_SET )
      return( -1 );

   // Create a view to the window in the event the painter drops
   // the current view we are using
   CreateViewFromViewForTask( &vXSLT2, vXSLT, 0 );

   // Make operator confirm the delete
   strcpy_s( szMessage, zsizeof( szMessage ), "OK to delete Window '" );
   strcat_s( szMessage, zsizeof( szMessage ), szTag );
   strcat_s( szMessage, zsizeof( szMessage ), "'?" );

   if ( MessagePrompt( vSubtask, "PE002",
                       "XSLT Maintenance",
                       szMessage,
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 ) == zRESPONSE_NO )
   {
      DropView( vXSLT2 );
      return( -1 );
   }

   if ( fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vSubtask, 0, 0, 0 ) == 0 )
   {
      DeleteEntity( vXSLT2, "Window", zREPOS_NEXT );

#ifndef __WIN32__
      SetNameForView( vXSLT2, "TZXSLT", vSubtask, zLEVEL_TASK );
      if ( GetViewByName( &vPanelList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) > 0 )
         RefreshCtrl( vPanelList, "ListBox1" );
#endif
   }

   DropView( vXSLT2 );
   return( 0 );
}

//./ ADD NAME=DeleteWindowFromList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeletePanelFromList
//
//  PURPOSE:    To delete a window from the painter list. This operation
//              calls update window by name to display and 'make current'
//              the window to be deleted and then calls DeleteWindow to
//              delete the window.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeletePanelFromList( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zSHORT nRC;

   nRC = GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   /* If the XSLT doesn't exist, or a panel doesn't exist then
      it can't be deleted, so return. */
   if ( nRC < 1 || CheckExistenceOfEntity( vXSLT, "Window" ) != 0 )
      return( 0 );

   GetViewByName( &vXSLT, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   if ( UpdatePanelFromList( vSubtask ) == 0 )
   {
      nRC = DeleteWindow( vSubtask );
      RefreshWindow( vXSLT );
      fnPainterCall( zMSG_REFRESHWINDOWLIST, vSubtask, 0, 0, 0 );
      return( nRC );
   }

   return( -1 );
}

//./ ADD NAME=ShowTabbing
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ShowTabbing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ShowTabbing( zVIEW vSubtask )
{
   return( (zSHORT) fnPainterCall( zMSG_SHOWTABBING, vSubtask, 0, 0, 0 ) );
}

//./ ADD NAME=RemoveTabbing
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      RemoveTabbing
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RemoveTabbing( zVIEW vSubtask )
{
   if ( MessagePrompt( vSubtask, "PE002",
                       "XSLT Maintenance",
                       "Remove All Tabstops?",
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 ) == zRESPONSE_YES )
   {
      return( (zSHORT) fnPainterCall( zMSG_REMOVETABBING, vSubtask, 0, 0, 0 ) );
   }

   return( 0 );
}

//./ ADD NAME=NextAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NextAction
//
//  PURPOSE:    To Change the next Action definition for the current window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NextAction( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   SetCursorNextEntity( vXSLT, "Action", 0 );

   return( 0 );
}

//./ ADD NAME=PrevAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      PrevAction
//
//  PURPOSE:    To Change the previous Action definition for
//              the current window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PrevAction( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   SetCursorPrevEntity( vXSLT, "Action", 0 );

   return( 0 );
}

//./ ADD NAME=NewAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NewAction
//
//  PURPOSE:    To Create a new Action in the current window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NewAction( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vXSLT, "Action", zPOS_AFTER );

   return( 0 );
}

//./ ADD NAME=DeleteAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteAction
//
//  PURPOSE:    To Delete an Action in the current window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteAction( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( DeleteEntity( vXSLT, "Action", zREPOS_NEXT ) == zCURSOR_NULL )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

   return( 0 );
}

//./ ADD NAME=ActionAdd
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ActionAdd
//
//  PURPOSE:    To Add an action to the window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActionAdd( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   CreateMetaEntity( vSubtask, vXSLT, "Action", zPOS_AFTER );
   SetAttributeFromString( vXSLT, "Action", "Tag", "New" );
   return( 0 );
}

//./ ADD NAME=ActionUpdate
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ActionUpdate
//
// PURPOSE:  To Update an action for the window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActionUpdate( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   if ( CheckExistenceOfEntity( vXSLT, "Action" ) > zCURSOR_UNCHANGED )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZXSLTDD", "ACT_UPD" );
   }

   return( 0 );
}

//./ ADD NAME=SortWindows
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SortWindows
//
//  PURPOSE:    To Sort windows in a XSLT
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SortWindows( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zCHAR szSelected[ 34 ];

   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szSelected, zsizeof( szSelected ), vXSLT, "Window", "Tag" );
   zgSortEntityWithinParent( zASCENDING, vXSLT, "Window", "Tag", "" );
   SetCursorFirstEntityByString( vXSLT, "Window", "Tag", szSelected, 0 );
   SetAllSelStatesForEntityForSet( vXSLT, "Window", 0, 1, 0 );
   SetSelectStateOfEntityForSet( vXSLT, "Window", 1, 1 );
   return( 0 );
}

//./ ADD NAME=SortActinsForWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SortActionsForWindow
//
//  PURPOSE:    To Sort actions for a window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SortActionsForWindow( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zCHAR szSelected[ 34 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szSelected, zsizeof( szSelected ), vXSLT, "Action", "Tag" );
   zgSortEntityWithinParent( zASCENDING, vXSLT,
                             "Action", "Tag", "" );
   SetCursorFirstEntityByString( vXSLT, "Action", "Tag", szSelected, 0 );
   SetAllSelStatesForEntityForSet( vXSLT, "Action", 0, 1, 0 );
   SetSelectStateOfEntityForSet( vXSLT, "Action", 1, 1 );
   return( 0 );
}

//./ ADD NAME=ActionDeleteFromList
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ActionDeleteFromList
//
//  PURPOSE:    To Delete an action for the window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActionDeleteFromList( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vXSLT, "Action" ) > zCURSOR_UNCHANGED )
   {
      DeleteEntity( vXSLT, "Action", zREPOS_NEXT );
   }

   return( 0 );
}

//./ ADD NAME=WndSelectAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WndSelectAction
//
//  PURPOSE:    To select an action for a window Event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WndSelectAction( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vEvents;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vXSLT, "Action" ) > zCURSOR_UNCHANGED )
   {
      SetAttributeFromAttribute( vEvents, "EventAct", "Tag",
                                 vXSLT, "Action", "Tag" );
   }

   return( 0 );
}

//./ ADD NAME=WndRemoveAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WndRemoveAction
//
//  PURPOSE:    To Remove an action for a window Event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WndRemoveAction( zVIEW vSubtask )
{
   zVIEW vEvents;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Recursive function for building TZPNOPWO Object
// NOTE: This CODE was borrowed/stolen from the painter control and
//       is duplicated in the painter control, the painter control
//       version should be eradicated and the build of the TZPNOPWO
//       object should be done in the prebuild of the menu window
//       ONLY and by this MODULE!!! 1/29/93 - Gig
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnBuildRecursSubOptions( zVIEW vSubtask, zVIEW vXSLT, zVIEW vOptionList,
                         zSHORT nLevel, zSHORT nID )
{
   zSHORT nIdx;
   zCHAR  szText[ 34 ];
   zCHAR  szDisplayText[ 80 ];

   if ( SetCursorFirstEntity( vXSLT, "Option", 0 ) == zCURSOR_SET )
   {
      do
      {
         szDisplayText[ 0 ] = 0;
         for ( nIdx = 0; nIdx <( 3 *(nLevel - 1) ); nIdx++ )
            szDisplayText[ nIdx ] = '.';
         szDisplayText[ nIdx ] = 0;
         GetStringFromAttribute( szText, zsizeof( szText ), vXSLT, "Option", "Text" );
         strcat_s( szDisplayText, zsizeof( szDisplayText ), szText );
         CreateEntity( vOptionList, "Option", zPOS_AFTER );
         SetAttributeFromAttribute( vOptionList, "Option", "ZKey",
                                    vXSLT, "Option", "ZKey" );
         SetAttributeFromInteger( vOptionList, "Option", "ID", nID++ );
      /* SetAttributeFromInteger( vOptionList, "Option",
                                  "StartLevel", nLevel ); */
         SetAttributeFromInteger( vOptionList, "Option",
                                  "Level", nLevel );
         SetAttributeFromString( vOptionList, "Option", "DisplayText",
                                 szDisplayText );
         SetViewToSubobject( vXSLT, "OptOpt" );
         nID = fnBuildRecursSubOptions( vSubtask, vXSLT, vOptionList,
                                        (zSHORT) (nLevel + 1), nID );
         ResetViewFromSubobject( vXSLT );

      } while ( SetCursorNextEntity( vXSLT, "Option", 0 ) == zCURSOR_SET );
   }

   return( nID );
}

/////////////////////////////////////////////////////////////////////////////
//
// Function to build the OptionList work object(TZPNOPWO)
//
/////////////////////////////////////////////////////////////////////////////
void
fnBuildOptionList( zVIEW vSubtask, zVIEW vXSLT, zPVIEW vOptionList )
{
   zVIEW  vOption;

   GetViewByName( &vOption, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( vOptionList, "TZPNOPWO",
                                   vSubtask, zSINGLE );
      SetNameForView( *vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   }
   else
   {
   // nRC = DeleteEntity( *vOptionList, "Root", zREPOS_NONE );
      DropView( *vOptionList );
      ActivateEmptyObjectInstance( vOptionList, "TZPNOPWO",
                                   vSubtask, zSINGLE );
      SetNameForView( *vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   }

   CreateEntity( *vOptionList, "Root", zPOS_AFTER );

   if ( CheckExistenceOfEntity( vOption, "Option" ) == zCURSOR_SET )
   {
      fnBuildRecursSubOptions( vSubtask, vXSLT, *vOptionList, 1, 1 );
   }
}

//./ ADD NAME=fnPositionOnZeidonOption
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      fnPositionOnZeidonOption
//
//  PURPOSE:    To Position on option corresponding to the
//              displayed/selected work object Option.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnPositionOnZeidonOption( zVIEW vXSLT, zVIEW vOptionList, zVIEW vSubtask )
{
   zUSHORT uLevel;
   zSHORT  nRC;
   zLONG   lZKey;
   zCHAR   szEntityName[ 256 ];

   if ( CheckExistenceOfEntity( vOptionList, "Option" ) != 0 )
      return( 0 );

   // Get ZKey of Option on which work object is positioned.
   GetIntegerFromAttribute( &lZKey, vOptionList, "Option", "ZKey" );

   // Position on first option in window
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   nRC = SetCursorFirstEntity( vXSLT, "Option", 0 );

   // Define a hierarchical cursor on the dialog view
   DefineHierarchicalCursor( vXSLT, "Option" );

   // Search hierarchically until finding the corresponding Option
   for ( ; ; )
   {
      if ( nRC == zCURSOR_UNCHANGED )
      {
         MessageSend( vSubtask, "PN00204", "XSLT Maintenance",
                      "Program Error",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         break;
      }

      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         SetViewToSubobject( vXSLT, szEntityName );

      if ( CompareAttributeToInteger( vXSLT, "Option",
                                      "ZKey", lZKey ) == 0 )
      {
         break;
      }

      nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vXSLT );
   }

   // Drop the hierarchical cursor
   DropHierarchicalCursor( vXSLT );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: TZXSLTDD_DeleteMenu
//
// PURPOSE:   Delete a Menu
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteMenu( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vWindow;
   zSHORT nRC;

// Get Access to Window

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   nRC = MessagePrompt( vSubtask, "PE002",
                       "XSLT Maintenance",
                       "OK to delete selected Menus?",
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 );
// Delete Menu
   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vXSLT, "Menu", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vXSLT, "Menu", "" ) )
      {
         nRC = DeleteEntity( vXSLT, "Menu", zREPOS_NONE );
      }
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_MENU" ) >= 0 )
        RefreshWindow( vWindow );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

//./ ADD NAME=LIST_MENU_Prebuild
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     LIST_MENU_Prebuild
//
//  PURPOSE:    Menu List Initialization.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
LIST_MENU_Prebuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zCHAR szTempString[ 255 ];

   // Set the caption for the window
   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vXSLT, "Window", "Tag");
      SetWindowCaptionTitle( vSubtask, 0, szTempString );
   }
   else
      SetWindowCaptionTitle( vSubtask, 0, "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LIST_MENU_OK( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zSHORT nRC;
   zCHAR  szMsg[ 100 ];
   zCHAR  szMenuName[ 33 ];

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // Make sure that all Menu entities have at least one Option.
      for ( nRC = SetCursorFirstEntity( vXSLT, "Menu", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXSLT, "Menu", 0 ) )
      {
         if ( CheckExistenceOfEntity( vXSLT, "Option" ) < zCURSOR_SET )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Menu item, " );
            GetStringFromAttribute( szMenuName, zsizeof( szMenuName ), vXSLT, "Menu", "Tag" );
            strcat_s( szMsg, zsizeof( szMsg ), szMenuName );
            strcat_s( szMsg, zsizeof( szMsg ), ", must have at least one Option" );
            MessageSend( vSubtask, "PN00222", "XSLT Maintenance",
                         szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            return( -1 );
         }
      }
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_Prebuild
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_Prebuild
//
//  PURPOSE:    Build the Option work object and initialize TZXSLTOPT.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Prebuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vOptionList;
   zVIEW  vOptions;
   zCHAR  szTempString[ 255 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vOptions, vXSLT, 0 );
   SetNameForView( vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   fnBuildOptionList( vSubtask, vXSLT, &vOptionList );

   // Highlight the first Option
   if ( CheckExistenceOfEntity( vOptionList, "Option" ) >= 0 )
   {
      SetCursorFirstEntity( vOptionList, "Option", "" );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );
   }
   // Position on the first zeidon option.
   fnPositionOnZeidonOption( vOptions, vOptionList, vSubtask );

   // Set the caption for the window
   GetStringFromAttribute( szTempString, zsizeof( szTempString ), vXSLT, "Window", "Tag");
   SetWindowCaptionTitle( vSubtask, 0, szTempString );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_Postbuild
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_Postbuild
//
//  PURPOSE:    Set state of checkBox control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Postbuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLTCopy;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstEntity( vXSLT, "DfltMenu", 0 ) == zCURSOR_SET )
   {
      if ( CompareAttributeToAttribute( vXSLT, "DfltMenu", "Tag",
                                        vXSLT, "Menu", "Tag" ) == 0 )
         SetCtrlState( vSubtask, "DefaultCheckBox", zCONTROL_STATUS_CHECKED, TRUE );
   }

   // Menu List is a multi selection list, delesect all Menues
   CreateViewFromViewForTask( &vXSLTCopy, vXSLT, 0 );
   for ( nRC = SetCursorFirstSelectedEntity( vXSLT, "Menu", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vXSLT, "Menu", "" ) )
   {
      SetSelectStateOfEntity( vXSLT, "Menu", 0 );
   }
   SetCursorFirstEntityByAttr( vXSLT, "Menu", "ZKey",
                               vXSLTCopy, "Menu", "ZKey", "" );
   DropView( vXSLTCopy );
   SetSelectStateOfEntity( vXSLT, "Menu", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_MENU" ) >= 0 )
      RefreshWindow( vWindow );

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
   {
      SetCtrlState( vSubtask, "DefaultCheckBox", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Add", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "DefaultCheckBox", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Up", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Down", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_ReturnFromSubWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_ReturnFromSubWindow
//
//  PURPOSE:    Rebuild the Option work object and position the
//              cursor on the correct entry.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_ReturnFromSubWindow( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vOptionList;
   zVIEW  vOptions;
   zLONG  lZKey;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   fnBuildOptionList( vSubtask, vXSLT, &vOptionList );

   // Position on correct member in list.
   // If no Option exists at that level, go up a level.
   if ( CheckExistenceOfEntity( vXSLT, "Menu" ) >= 0 )
   {
      if ( CheckExistenceOfEntity( vOptions, "Option" ) < 0 )
         ResetViewFromSubobject( vOptions );

      if ( CheckExistenceOfEntity( vOptions, "Option" ) >= 0 )
      {
         GetIntegerFromAttribute( &lZKey, vOptions, "Option", "ZKey" );
         SetCursorFirstEntityByInteger( vOptionList, "Option",
                                        "ZKey", lZKey, "" );
         SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );
      }
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_SetDefaultMenu
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_SetDefaultMenu
//
//  PURPOSE:    To go to the previous menu in the dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_SetDefaultMenu( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntity( vXSLT, "DfltMenu", 0 ) == zCURSOR_SET )
   {
      ExcludeEntity( vXSLT, "DfltMenu", zREPOS_NONE );
   }
   if ( CheckExistenceOfEntity( vXSLT, "Menu" ) == zCURSOR_SET )
      IncludeSubobjectFromSubobject( vXSLT, "DfltMenu",
                                     vXSLT, "Menu", zPOS_AFTER );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_ClearDefaultMenu
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_ClearDefaultMenu
//
//  PURPOSE:    To go to the previous menu in the dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_ClearDefaultMenu( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntity( vXSLT, "DfltMenu", 0 ) == zCURSOR_SET )
   {
      ExcludeEntity( vXSLT, "DfltMenu", zREPOS_NONE );
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_SetClearDefaultMenu
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_SetClearDefaultMenu
//
//  PURPOSE:    To either Set or Unset Default Menu based on CheckBox
//              This operation is performed when the checkbox changes
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_SetClearDefaultMenu( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( GetCtrlState( vSubtask, "DefaultCheckBox",
                      zCONTROL_STATUS_CHECKED ) == TRUE )
   {
      UPD_MENU_SetDefaultMenu( vSubtask );
   }
   else
   {
      UPD_MENU_ClearDefaultMenu( vSubtask );
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_MenuPrev
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MenuPrev
//
//  PURPOSE:    To go to the previous menu in the dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MenuPrev( zVIEW vSubtask )
{
   zVIEW  vOptions;
   zVIEW  vXSLT;
   zVIEW  vOptionList;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   AcceptSubobject( vXSLT, "Menu" );
   if ( SetCursorPrevEntity( vXSLT, "Menu", 0 ) == zCURSOR_SET )
   {
      SetViewFromView( vOptions, vXSLT );
      fnBuildOptionList( vSubtask, vXSLT, &vOptionList );
   }

   CreateTemporalSubobjectVersion( vXSLT, "Menu" );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_MenuNext
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MenuNext
//
//  PURPOSE:    To go to the next menu in the dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MenuNext( zVIEW vSubtask )
{
   zVIEW  vOptions;
   zVIEW  vXSLT;
   zVIEW  vOptionList;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   AcceptSubobject( vXSLT, "Menu" );
   if ( SetCursorNextEntity( vXSLT, "Menu", 0 ) == zCURSOR_SET )
   {
      SetViewFromView( vOptions, vXSLT );
      fnBuildOptionList( vSubtask, vXSLT, &vOptionList );
   }
   CreateTemporalSubobjectVersion( vXSLT, "Menu" );
   return( 0 );
}

//./ ADD NAME=UPD_MENU_MenuAdd
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MenuAdd
//
//  PURPOSE:    To add a menu to the dialog
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MenuAdd( zVIEW vSubtask )
{
   zVIEW  vOptions;
   zVIEW  vXSLT;
   zVIEW  vOptionList;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLT, "Menu" ) == zCURSOR_SET )
      AcceptSubobject( vXSLT, "Menu" );
   CreateTemporalMetaEntity( vSubtask, vXSLT, "Menu", zPOS_AFTER );
   SetAttributeFromString( vXSLT, "Menu", "Tag", "New Menu");
   SetViewFromView( vOptions, vXSLT );
   CreateMetaEntity( vSubtask, vOptions, "Option", zPOS_AFTER );
   SetAttributeFromString( vOptions, "Option", "Tag", "File" );
   SetAttributeFromString( vOptions, "Option", "Text", "&File..." );
   fnBuildOptionList( vSubtask, vXSLT, &vOptionList );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MoveOptions
//
//  PURPOSE:    To Move an item up in the menu bar
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MoveOptions( zVIEW  vSubtask,
                      zVIEW  NewWO,
                      zVIEW  OrigWO,
                      zSHORT nPosition )
{
   zSHORT   nRC;
   zSHORT   RESULT;

   nRC = CreateMetaEntity( vSubtask, NewWO, "Option", nPosition );
   nRC = SetMatchingAttributesByName( NewWO, "Option", OrigWO,
                                      "Option", zSET_NULL );
   RESULT = SetCursorFirstEntity( OrigWO, "OptOpt", "" );
   if ( RESULT > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( OrigWO, "OptOpt" );
      SetViewToSubobject( NewWO, "OptOpt" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         IncludeSubobjectFromSubobject( NewWO, "Option",
                                        OrigWO, "Option", zPOS_AFTER );
         /*
         IncludeSubobjectFromSubobject( NewWO, "OptOpt", OrigWO,
                                       "OptOpt", zPOS_AFTER );
         */
         if ( CheckExistenceOfEntity( OrigWO, "OptMap" ) == zCURSOR_SET )
            IncludeSubobjectFromSubobject( NewWO, "OptMap",
                                          OrigWO, "OptMap", zPOS_AFTER );
         if ( CheckExistenceOfEntity( OrigWO, "OptAct" ) == zCURSOR_SET )
            IncludeSubobjectFromSubobject( NewWO, "OptAct",
                                          OrigWO, "OptAct", zPOS_AFTER );

         RESULT = SetCursorNextEntity( OrigWO, "Option", "" );
      }

      ResetViewFromSubobject( OrigWO );
      ResetViewFromSubobject( NewWO );
   }
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZXSLTDD_CloneOption( zVIEW    NewWO,
                       zVIEW    NewW,
                       zVIEW    OrigWO,
                       zVIEW    OrigW,
                       zLONG    lPosition,
                       zVIEW    vSubtask )
{
   zSHORT   nRC;
   zSHORT   RESULT;
   zLONG    lTempInteger_0;
   zLONG    lTempInteger_1;
   zCHAR    szTempString_0[ 33 ];

   nRC = CreateMetaEntity( vSubtask, NewWO, "Option", (zSHORT) lPosition );
   nRC = SetMatchingAttributesByName( NewWO, "Option",
                                            OrigWO, "Option", zSET_NULL );
   RESULT = SetCursorFirstEntity( OrigWO, "OptOpt", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( OrigWO, "OptOpt" );
      SetViewToSubobject( NewWO, "OptOpt" );
      oTZXSLTDD_CloneOption( NewWO, NewW, OrigWO,
                             OrigW, zPOS_AFTER, vSubtask );
      ResetViewFromSubobject( OrigWO );
      ResetViewFromSubobject( NewWO );
      RESULT = SetCursorNextEntity( OrigWO, "OptOpt", "" );
   }

   lTempInteger_0 = CheckExistenceOfEntity( OrigWO, "OptMap" );
   if ( lTempInteger_0 == zCURSOR_SET )
      oTZWDLGSO_CloneOptMap( vSubtask, OrigW, OrigWO, NewW, NewWO, vSubtask );

   lTempInteger_1 = CheckExistenceOfEntity( OrigWO, "OptAct" );
   if ( lTempInteger_1 == zCURSOR_SET )
   {
      if ( CompareAttributeToString( OrigWO, "OptAct", "Tag", "" ) != 0 )
      {
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), OrigWO, "OptAct", "Tag" );
         RESULT = SetCursorFirstEntityByString( NewW, "Action", "Tag", szTempString_0, "" );
         if ( RESULT >= 0 )
         {
            IncludeSubobjectFromSubobject( NewWO, "OptAct",
                                           NewW, "Action", zPOS_AFTER );
         }
      }
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_MoveUpSameLevel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MoveUpSameLevel
//
//  PURPOSE:    To Move an item up in the menu bar
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MoveUpSameLevel( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOption;
   zVIEW  vOption2;
   zVIEW  vWindow;
   zLONG  lZKey;

   GetViewByName( &vOption, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vOptionList, "Option" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vOption2, vOption, 0 );
   if ( SetCursorPrevEntity( vOption, "Option", 0 ) > zCURSOR_UNCHANGED )
   {
      oTZXSLTDD_CloneOption( vOption, vWindow, vOption2, vWindow,
                             zPOS_BEFORE, vSubtask );
      /*
      UPD_MENU_MoveOptions( vSubtask, vOption, vOption2, zPOS_BEFORE );
      */
      DeleteEntity( vOption2, "Option", zREPOS_PREV );
   }

   DropView( vOption2 );
   fnBuildOptionList( vSubtask, vWindow, &vOptionList );
   GetIntegerFromAttribute( &lZKey, vOption, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_MoveDownSameLevel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MoveDownSameLevel
//
//  PURPOSE:    To Move an item down in the menu bar
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MoveDownSameLevel( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOption;
   zVIEW  vOption2;
   zVIEW  vWindow;
   zLONG  lZKey;

   GetViewByName( &vOption, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vOptionList, "Option" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vOption2, vOption, 0 );
   if ( SetCursorNextEntity( vOption, "Option", 0 ) > zCURSOR_UNCHANGED )
   {
      oTZXSLTDD_CloneOption( vOption, vWindow, vOption2, vWindow,
                             zPOS_AFTER, vSubtask );
      /*
      UPD_MENU_MoveOptions( vSubtask, vOption, vOption2, zPOS_AFTER );
      */
      DeleteEntity( vOption2, "Option", zREPOS_PREV );
   }

   DropView( vOption2 );
   fnBuildOptionList( vSubtask, vWindow, &vOptionList );
   GetIntegerFromAttribute( &lZKey, vOption, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_MoveUpOneLevel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MoveUpOneLevel
//
//  PURPOSE:    To take a menu bar item and move it up one level
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MoveUpOneLevel( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOption;
   zVIEW  vOption2;
   zVIEW  vWindow;
   zLONG  lZKey;

   GetViewByName( &vOption, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vOptionList, "Option" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vOption2, vOption, 0 );
   if ( ResetViewFromSubobject( vOption ) == zCURSOR_SET )
   {
      oTZXSLTDD_CloneOption( vOption, vWindow, vOption2, vWindow,
                             zPOS_AFTER, vSubtask );
      /*
      UPD_MENU_MoveOptions( vSubtask, vOption, vOption2, zPOS_AFTER );
      */
      DeleteEntity( vOption2, "Option", zREPOS_PREV );
   }

   DropView( vOption2 );
   fnBuildOptionList( vSubtask, vWindow, &vOptionList );
   GetIntegerFromAttribute( &lZKey, vOption, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

   // return that everything is AOK
   return( 0 );
}

//./ ADD NAME=UPD_MENU_MoveDownOneLevel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_MoveDownOneLevel
//
//  PURPOSE:    To take a menu bar item and move it down one level
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_MoveDownOneLevel( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOption;
   zVIEW  vOption2;
   zVIEW  vWindow;
   zLONG  lZKey;

   GetViewByName( &vOption, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWindow, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vOptionList, "Option" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vOption2, vOption, 0 );
   if ( SetCursorPrevEntity( vOption, "Option", 0 ) > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( vOption, "OptOpt" );
      oTZXSLTDD_CloneOption( vOption, vWindow, vOption2, vWindow,
                             zPOS_AFTER, vSubtask );
      /*
      UPD_MENU_MoveOptions( vSubtask, vOption, vOption2, zPOS_BEFORE );
      */
      DeleteEntity( vOption2, "Option", zREPOS_PREV );
   }

   DropView( vOption2 );
   fnBuildOptionList( vSubtask, vWindow, &vOptionList );
   GetIntegerFromAttribute( &lZKey, vOption, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

   // return that everything is AOK
   return( 0 );
}

//./ ADD NAME=UPD_MENU_OptionAdd
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_OptionAdd
//
//  PURPOSE:    To add an Option entry into the menu bar
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OptionAdd( zVIEW vSubtask )
{
   zVIEW   vOptions;
   zVIEW   vXSLT;
   zVIEW   vOptionList;
   zVIEW   vTmpOptions;
   zVIEW   vRecursOptions;
   zSHORT  nWork;
   zUSHORT uLevel;
   zLONG   lZKey;
   zBOOL   bTagFound;
   zCHAR   szTag[ 10 ];
   zCHAR   szEntityName[ 33 ];
   static zCHAR  szWork[ 6 ];
   zSHORT  nRC;

   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   // Go down a level, if an Option exists, and set cursor on last
   // entry.
   if ( CheckExistenceOfEntity( vOptions, "Option" ) >= 0 )
   {
      SetViewToSubobject( vOptions, "OptOpt" );
      SetCursorLastEntity( vOptions, "Option", "" );
   }

   CreateMetaEntity( vSubtask, vOptions, "Option", zPOS_AFTER );
   SetAttributeFromString( vOptions, "Option", "Text", "New" );

   // Get the tag
   CreateViewFromViewForTask( &vTmpOptions, vOptions, 0 );
   while ( ResetViewFromSubobject( vTmpOptions ) == 0 )
   {
   }

   CreateViewFromViewForTask( &vRecursOptions, vTmpOptions, 0 );

   for ( nWork = 1; nWork < 1000; nWork++ )
   {
      zltoa( nWork, szWork, zsizeof( szWork ), 10 );

      strcpy_s( szTag, zsizeof( szTag ), "New" );
      strcat_s( szTag, zsizeof( szTag ), szWork );

      SetViewFromView( vRecursOptions, vTmpOptions );
      // Now make sure that the tag is unique
      bTagFound = 0;
      DefineHierarchicalCursor( vRecursOptions, "Menu" );
      nRC = SetCursorNextEntityHierarchical( &uLevel,
                                             szEntityName, vRecursOptions );
      while ( nRC >= zCURSOR_SET && uLevel > 3 )
      {
         if ( nRC == zCURSOR_SET_RECURSIVECHILD )
         {
            SetViewToSubobject( vRecursOptions, "OptOpt" );
         }
         if ( CompareAttributeToString( vRecursOptions, "Option",
                                        "Tag", szTag ) == 0 )
         {
            bTagFound = 1;
            break;
         }
         nRC = SetCursorNextEntityHierarchical( &uLevel,
                                                szEntityName, vRecursOptions );
      }

      DropHierarchicalCursor( vRecursOptions );
      if ( bTagFound == FALSE )
         break;
   }

   DropView( vTmpOptions );
   DropView( vRecursOptions );
   SetAttributeFromString( vOptions, "Option", "Tag", szTag );

   fnBuildOptionList( vSubtask, vXSLT, &vOptionList );

   // Highlight correct line
   GetIntegerFromAttribute( &lZKey, vOptions, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

   return( 0 );
}

//./ ADD NAME=UPD_MENU_OptionSelect
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_OptionSelect
//
//  PURPOSE:    To select a menu bar entry in the list by mouse
//              single-click
//              Position on the Window Option that corresponds to
//              the work Option selected.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OptionSelect( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOptions;
   zVIEW  vXSLT;
   zLONG  lZKey;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vOptionList, "Option" ) != 0 )
      return( 0 );

   // Position on the current Option
   fnPositionOnZeidonOption( vOptions, vOptionList, vSubtask );

   // Because OptionList was rebuilt, position has to be reset to the
   // current option.
   GetIntegerFromAttribute( &lZKey, vOptions, "Option", "ZKey" );
   SetCursorFirstEntityByInteger( vOptionList, "Option",
                                  "ZKey", lZKey, "" );
   SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );
   return( 0 );
}

//./ ADD NAME=UPD_MENU_OptionUpdate
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_OptionUpdate
//
//  PURPOSE:    To select a menu bar entry in the list by mouse
//              double-click
//              Position on the Window Option that corresponds to
//              the work Option selected and transfer to the
//              Option Update window.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OptionUpdate( zVIEW vSubtask )
{
   zVIEW  vOptions;
   zVIEW  vOptionList;

   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK ) == -1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

//   UPD_MENU_OptionSelect( vSubtask );
   CreateTemporalSubobjectVersion( vOptions, "Option" );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OPT_UPD_NextMenuItem
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OPT_UPD_NextMenuItem( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOptions;
   zVIEW  vParentWindow;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( OPT_UPD_OK( vSubtask ) >= 0 )
   {
      UPD_MENU_ReturnFromSubWindow( vSubtask );
      GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 0, 1 );

      SetCursorNextEntity( vOptionList, "Option", "" );
      // Position on the current Option
      fnPositionOnZeidonOption( vOptions, vOptionList, vSubtask );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

      UPD_MENU_OptionUpdate( vSubtask );
      OPT_UPD_Init( vSubtask );
      OPT_UPD_DisableActionButton( vSubtask );
      GetParentWindow( &vParentWindow, vSubtask );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "Tag" );
   }

   return( 0 );
} // OPT_UPD_NextMenuItem

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OPT_UPD_PrevMenuItem
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OPT_UPD_PrevMenuItem( zVIEW vSubtask )
{
   zVIEW  vOptionList;
   zVIEW  vOptions;
   zVIEW  vParentWindow;

   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( OPT_UPD_OK( vSubtask ) >= 0 )
   {
      UPD_MENU_ReturnFromSubWindow( vSubtask );
      GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 0, 1 );

      SetCursorPrevEntity( vOptionList, "Option", "" );
      // Position on the current Option
      fnPositionOnZeidonOption( vOptions, vOptionList, vSubtask );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );

      UPD_MENU_OptionUpdate( vSubtask );
      OPT_UPD_Init( vSubtask );
      OPT_UPD_DisableActionButton( vSubtask );
      GetParentWindow( &vParentWindow, vSubtask );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "Tag" );
   }

   return( 0 );
} // OPT_UPD_PrevMenuItem

//./ ADD NAME=UPD_MENU_OptionDelete
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_OptionDelete
//
//  PURPOSE:    To delete a menu bar Option entry.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OptionDelete( zVIEW vSubtask )
{
   zVIEW  vOptions;
   zVIEW  vXSLT;
   zVIEW  vOptionList;
   zLONG  lZKey;
   zCHAR  szMenuItem[ 33 ];
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szMenuItem, zsizeof( szMenuItem ), vOptions, "Option", "Text" );
   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Menu Item '" );
   strcat_s( szMsg, zsizeof( szMsg ), szMenuItem );
   strcat_s( szMsg, zsizeof( szMsg ), "'" );
   // check SubItems
   if ( CheckExistenceOfEntity( vOptions, "OptOpt" ) >= zCURSOR_SET )
      strcat_s( szMsg, zsizeof( szMsg ), " with all Subitems?" );
   else
      strcat_s( szMsg, zsizeof( szMsg ), "?" );

   if ( MessagePrompt( vSubtask, "", "XSLT Maintenance",
                       szMsg, 0, zBUTTONS_YESNO, zRESPONSE_NO,
                       zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   DeleteEntity( vOptions, "Option", zPOS_PREV );

   // If there are no Options at this level, reset the view back a
   // level.
   if ( CheckExistenceOfEntity( vOptions, "Option" ) < 0 )
      ResetViewFromSubobject( vOptions );

   fnBuildOptionList( vSubtask, vXSLT, &vOptionList );
   // If any option exists, position on that option in the OptionList.
   if ( CheckExistenceOfEntity( vOptions, "Option" ) == 0 )
   {
      GetIntegerFromAttribute( &lZKey, vOptions, "Option", "ZKey" );
      SetCursorFirstEntityByInteger( vOptionList, "Option",
                                     "ZKey", lZKey, "" );
      SetSelectStateOfEntityForSet( vOptionList, "Option", 1, 1 );
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_OK
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_OK
//
//  PURPOSE:    To save the results of the menu bar editing
//              and delete the work object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_OK( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLTCopy;
   zSHORT nRC;
   zCHAR  szTag[ 33 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vXSLT, "Menu", "Tag", "" ) == 0 )
   {
      MessageSend( vSubtask, "PN00205", "XSLT Maintenance",
                   "Menu Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetFocusToCtrl( vSubtask, "MenuName" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   CreateViewFromViewForTask( &vXSLTCopy, vXSLT, 0 );
   GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "Menu", "Tag" );
   nRC = SetCursorFirstEntityByString( vXSLTCopy, "Menu", "Tag", szTag, "" );
   nRC = SetCursorNextEntityByString( vXSLTCopy, "Menu", "Tag", szTag, "" );
   DropView( vXSLTCopy );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00205", "XSLT Maintenance",
                   "The Menu Name is not unique within this window.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetFocusToCtrl( vSubtask, "MenuName" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vXSLT, "Option" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00206", "XSLT Maintenance",
                   "Menu Item for Menu is required.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   nRC = AcceptSubobject( vXSLT, "Menu" );
   SetSelectStateOfEntity( vXSLT, "Menu", 1 );

   fnPainterCall( zMSG_REPAINTACTIONBAR, vSubtask, 0, 0, 0 );
   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );

   SetCursorFirstSelectedEntity( vXSLT, "Menu", "" );
   return( 0 );
}

//./ ADD NAME=UPD_MENU_Delete
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_Delete
//
//  PURPOSE:    To delete a Menu entry from the Window
//              If the last Menu entry was just deleted, you'll
//              just stay on this window with everything grayed, except
//              the Add Menu button.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Delete( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vOptionList;
   zVIEW  vOptions;

   GetViewByName( &vOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOptionList, "TZPNOPWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLT, "Menu" ) >= 0 )
   {
      DeleteEntity( vXSLT, "Menu", zPOS_AFTER );
      SetViewFromView( vOptions, vXSLT );
      fnBuildOptionList( vSubtask, vXSLT, &vOptionList );
   }

   return( 0 );
}

//./ ADD NAME=UPD_MENU_Cancel
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UPD_MENU_Cancel
//
//  PURPOSE:    To cancel the results of the menu bar editing
//              Also delete the work object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_MENU_Cancel( zVIEW vSubtask )
{
   zVIEW vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   CancelSubobject( vXSLT, "Menu" );

   return( 0 );
}

//./ ADD NAME=LIST_HK_Prebuild
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      LIST_HK_Prebuild
//
//  PURPOSE:    Set Caption
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
LIST_HK_Prebuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zCHAR szTempString[ 255 ];

   // Set the caption for the window
   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vXSLT, "Window", "Tag" );
      SetWindowCaptionTitle( vSubtask, 0, szTempString );
   }
   else
      SetWindowCaptionTitle( vSubtask, 0, "" );

   return( 0 );
}

//./ ADD NAME=NewHotkey
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NewHotkey
//
//  PURPOSE:    To create a new hotkey specification.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NewHotkey( zVIEW vSubtask )
{
   zACCEL_DEF ac;
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vXSLT, "Hotkey", zPOS_AFTER );
   SetAttributeFromString( vXSLT, "Hotkey", "Tag", "New" );
   ac.fCtrl = 0;
   ac.fAlt = 0;
   ac.fShift = 0;
   ac.uVK = 112;
   SetAttributeFromInteger( vXSLT, "Hotkey", "VKey", ac.uVK );
   SetAttributeFromBlob( vXSLT, "Hotkey", "KeyCombo",
                         &ac, sizeof( zACCEL_DEF ) );
   return( 0 );
}

//./ ADD NAME=DeleteHotkey
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DeleteHotkey
//
//  PURPOSE:    To delete a hotkey specification.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DeleteHotkey( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( MessagePrompt( vSubtask, "PE002",
                       "XSLT Maintenance",
                       "OK to delete selected Shortcut Keys?",
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 ) == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vXSLT, "Hotkey", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vXSLT, "Hotkey", "" ) )
      {
         DeleteEntity( vXSLT, "Hotkey", zREPOS_NONE );
      }
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_HK" ) >= 0 )
         RefreshWindow( vWindow );
   }
   return( 0 );
}


//./ ADD NAME=ShowHotkey
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ShowHotkey
//
//  PURPOSE:    To show the breakdown of the key combo components
//              for a hotkey.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ShowHotkey( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zACCEL_DEF ac;
   zULONG  ulLth = sizeof( zACCEL_DEF );

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   GetBlobFromAttribute( &ac, &ulLth, vXSLT, "Hotkey", "KeyCombo" );
   SetCtrlState( vSubtask, "Ctrl", zCONTROL_STATUS_CHECKED, ac.fCtrl );
   SetCtrlState( vSubtask, "Alt", zCONTROL_STATUS_CHECKED, ac.fAlt );
   SetCtrlState( vSubtask, "Shift", zCONTROL_STATUS_CHECKED, ac.fShift );

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
   {
      SetCtrlState( vSubtask, "Ctrl", zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Alt", zCONTROL_STATUS_ENABLED, 0 );
      SetCtrlState( vSubtask, "Shift", zCONTROL_STATUS_ENABLED, 0 );
   }

   return( 0 );
}

//./ ADD NAME=InitVkey
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      InitVkey
//
//  PURPOSE:    To set the VKey attribute from the KeyCombo blob
//              for a hotkey.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
InitVkey( zVIEW vSubtask )
{
   zVIEW      vXSLT;
   zVIEW      vXSLTCopy;
   zVIEW      vWindow;
   zACCEL_DEF ac;
   zCHAR      szTempString[ 256 ];
   zSHORT     nRC;
   zULONG     ulLth = sizeof( zACCEL_DEF );

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   // Shortcut List is a multi selection list, delesect all Shortcuts
   CreateViewFromViewForTask( &vXSLTCopy, vXSLT, 0 );
   for ( nRC = SetCursorFirstSelectedEntity( vXSLT, "Hotkey", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vXSLT, "Hotkey", "" ) )
   {
      SetSelectStateOfEntity( vXSLT, "Hotkey", 0 );
   }
   SetCursorFirstEntityByAttr( vXSLT, "Hotkey", "ZKey",
                               vXSLTCopy, "Hotkey", "ZKey", "" );
   DropView( vXSLTCopy );
   SetSelectStateOfEntity( vXSLT, "Hotkey", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_HK" ) >= 0 )
      RefreshCtrl( vWindow, "lbHotkey" );

   GetBlobFromAttribute( &ac, &ulLth, vXSLT, "Hotkey", "KeyCombo" );
   SetAttributeFromInteger( vXSLT, "Hotkey", "VKey", ac.uVK );

   // Put the window name in the caption.
   GetStringFromAttribute( szTempString, zsizeof( szTempString ), vXSLT, "Window", "Tag");
   SetWindowCaptionTitle( vSubtask, 0, szTempString );

   return( 0 );
}

//./ ADD NAME=SetHotkey
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      SetHotkey
//
//  PURPOSE:    To set the key combo from its components.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SetHotkey( zVIEW vSubtask )
{
   zACCEL_DEF ac;
   zVIEW  vXSLT;
   zLONG  lVKey;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   // check for null value
   if ( CompareAttributeToString( vXSLT, "Hotkey", "VKey", "" ) == 0 )
   {
      MessageSend( vSubtask, "PN00207", "XSLT Maintenance",
                   "No Key selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "VKey" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   AcceptSubobject( vXSLT, "Hotkey" );
   ac.fCtrl = (zUSHORT) GetCtrlState( vSubtask, "Ctrl",
                                      zCONTROL_STATUS_CHECKED );
   ac.fAlt = (zUSHORT) GetCtrlState( vSubtask, "Alt",
                                     zCONTROL_STATUS_CHECKED );
   ac.fShift = (zUSHORT) GetCtrlState( vSubtask, "Shift",
                                       zCONTROL_STATUS_CHECKED );
   GetIntegerFromAttribute((zPLONG) &lVKey, vXSLT, "Hotkey", "VKey" );
   ac.uVK = (zUSHORT) lVKey;
   SetAttributeFromBlob( vXSLT, "Hotkey", "KeyCombo",
                         &ac, sizeof( zACCEL_DEF ) );
   return( 0 );
}

//./ ADD NAME=CHG_CTRL_Prebuild
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CHG_CTRL_Prebuild
//
//  PURPOSE:    Setup for switching the type of a control in the painter
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CHG_CTRL_Prebuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vPE;
   zSHORT nRC;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) <= 0 ||
        GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "CHG_CTRL_Prebuild unable to get required views", "" );
      return( -1 );
   }

   /* If there were no controls on the window and the user just created some
      and then immediately selected one and hit the toolbar change button,
      for whatever reason vXSLT shows no controls under it yet and so
      we must call ACTIVATEPAINTERWINDOW which creates the Zeidon info. */
// if ( CheckExistenceOfEntity( vXSLT, "Control" ) != 0 )
      fnPainterCall( zMSG_ACTIVATEPAINTERWINDOW, vSubtask, 0, 0, 0 );
// fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, 0, 0 );

   nRC = SetCursorFirstEntityByEntityCsr( vPE, "ControlDef",
                                          vXSLT, "ControlDef", "" );
   return( 0 );
}

//./ ADD NAME=LIST_CTRL_EventsActs
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   LIST_CTRL_EventsActs
//
// PURPOSE: Setup for resetting Event/Action combinations in the painter:
//
//          1. Sets up the Outliner work attributes from CtrlBOI.
//          2. Initializes the TZPNEVWO Event work object as
//             necessary. (*** This shouldn't be necessary if the
//             Painter was always functioning correctly.  Remove and
//             test at some point.)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zSHORT
fnIncludeCtrlMapViews( zVIEW vTopCtrl, zVIEW vControl )
{
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vControl, "Control", 0 );
   while ( nRC == zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vControl, "CtrlMapView",
                                     vTopCtrl, "CtrlMapView", zPOS_AFTER );

      SetViewToSubobject( vControl, "CtrlCtrl" );
      fnIncludeCtrlMapViews( vTopCtrl, vControl );
      ResetViewFromSubobject( vControl );

      nRC = SetCursorNextEntity( vControl, "Control", 0 );
   }

   return( 0 );
}

zSHORT
fnSetOutliner( zVIEW vSubtask )
{
   zVIEW    vTopCtrl;
   zVIEW    vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   /* recursive function to fix the CtrlCtrl's */
   CreateViewFromViewForTask( &vTopCtrl, vControl, 0 );
   SetViewToSubobject( vControl, "CtrlCtrl" );
   fnIncludeCtrlMapViews( vTopCtrl, vControl );

   DropView( vTopCtrl );
   ResetViewFromSubobject( vControl );
   ResetView( vControl );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_CTRL_TreeList( zVIEW vSubtask )
{
   zVIEW  vCtrlCtrl;     // the dialog control view

   GetViewByName( &vCtrlCtrl, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vCtrlCtrl, vCtrlCtrl, 0 );
   SetNameForView( vCtrlCtrl, "TZCONTROLL", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_CTRL_PreSelectChange( zVIEW vSubtask )
{
// zVIEW  vCtrlCtrl;     // the dialog control view
//
// GetViewByName( &vCtrlCtrl, "TZCONTROLL", vSubtask, zLEVEL_TASK );
// DisplayEntityInstance( vCtrlCtrl, "Control" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_CTRL_SelectChange( zVIEW vSubtask )
{
// zVIEW  vCtrlCtrl;     // the dialog control view
//
// GetViewByName( &vCtrlCtrl, "TZCONTROLL", vSubtask, zLEVEL_TASK );
// DisplayEntityInstance( vCtrlCtrl, "Control" );
   RefreshCtrl( vSubtask, "Text" );
   RefreshCtrl( vSubtask, "Tag" );
   RefreshCtrl( vSubtask, "PosX" );
   RefreshCtrl( vSubtask, "PosY" );
   RefreshCtrl( vSubtask, "SizeX" );
   RefreshCtrl( vSubtask, "SizeY" );
   RefreshCtrl( vSubtask, "View" );
   RefreshCtrl( vSubtask, "Entity" );
   RefreshCtrl( vSubtask, "Attr" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LIST_CTRL_EventsActs( zVIEW vSubtask )
{
   return( fnSetOutliner( vSubtask ) );

#if 0
   zVIEW    vXSLTA;
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;
   zVIEW    vWork;

   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   fnPainterCall( zMSG_ACTIVATEPAINTERWINDOW, vSubtask, 0, 0, 0 );

   if ( GetViewByName( &vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vXSLTA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZXSLT.
   if ( CheckExistenceOfEntity( vXSLTC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vXSLTA, vXSLTW, 0 );
      SetCursorFirstEntityByAttr( vXSLTA, "Action", "Tag",
                                  vXSLTC, "EventAct", "Tag", "" );
      SetNameForView( vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // If not, set TZACTION to empty instance for auto graying.
      fnCreateEmptyActionView( vSubtask, vXSLTW );
      /*
      ActivateEmptyObjectInstance( &vXSLTA, 0, vXSLTW, zSINGLE );
      SetNameForView( vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );
      */
   }

   if ( CheckExistenceOfEntity( vXSLTC, "Event" ) != zCURSOR_SET )
   {
//    GetViewByName( &vWork, "TZPACTLO", vSubtask, zLEVEL_TASK );
//    SetCursorFirstEntityByString( vWork, "Control", "Tag", "Outliner", "" );
//    do
      {
         CreateEntity( vXSLTC, "Event", zPOS_AFTER );
         SetAttributeFromInteger( vXSLTC, "Event", "ZKey", 5044371 );
         SetAttributeFromInteger( vXSLTC, "Event", "Type", 1 );
//       SetAttributeFromAttribute( vXSLTC, "Event", "Tag",
//                                  vWork, "Event", "Tag" );
//       SetAttributeFromAttribute( vXSLTC, "Event", "Type",
//                                  vWork, "Event", "Type" );
      }
//    while ( SetCursorNextEntity( vWork, "Event", "" ) == zCURSOR_SET );
   }

   return( 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_CTRL_SelectAction
//
// PURPOSE:  This function creates the Event entity for the Push
//           Button, if it doesn't already exist.
//           **** Note that we should use Create Temporal entity. ***
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_CTRL_SelectAction( zVIEW vSubtask )
{
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;
   zVIEW    vWork;

   GetViewByName( &vXSLTC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLTC, "Event" ) != zCURSOR_SET )
   {
      CreateTemporalMetaEntity( vSubtask, vXSLTC, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vXSLTC, "Event", "Type", 1 );
   }
   if ( CheckExistenceOfEntity( vXSLTC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vXSLTW, "Action", "Tag",
                                  vXSLTC, "EventAct", "Tag", "" );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      LIST_CTRL_ReturnFromSub
//
// PURPOSE:    Make sure TZACTION is the View for the Action just
//             selected.  TZACTION to be a null View, if no EventAct
//             existed prior to the Select.
//             Also drop the TZACTION View if it is null.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_CTRL_ReturnFromSub( zVIEW vSubtask )
{
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;
   zVIEW    vXSLTA;
   zVIEW    vEvent;

   GetViewByName( &vXSLTC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZXSLT.
   if ( CheckExistenceOfEntity( vXSLTC, "EventAct" ) == zCURSOR_SET )
   {
      // Drop TZACTION work OI, if it is empty.
      if ( CheckExistenceOfEntity( vXSLTA, "Window" ) != zCURSOR_SET )
         DropObjectInstance( vXSLTA );

      CreateViewFromViewForTask( &vXSLTA, vXSLTW, 0 );
      SetCursorFirstEntityByAttr( vXSLTA, "Action", "Tag",
                                  vXSLTC, "EventAct", "Tag", "" );
      SetNameForView( vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );

      // Update Event/Action list.
      GetViewByName( &vEvent, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      SetAttributeFromAttribute( vEvent, "EventAct", "Tag",
                                 vXSLTC, "EventAct", "Tag" );
   }

   return( 0 );
}

//./ ADD NAME=CloneXSLTWindow
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      CloneXSLTWindow
//
//  PURPOSE:    This function simply sets a flag to identify the
//              as clone instead of merge for the TZPNCWDD SEL_CLONE
//              dialog.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CloneXSLTWindow( zVIEW vSubtask )
{
   zVIEW vWork;
   zVIEW vXSLT;

   GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );

   // If the dialog is not updatable, then don't do the merge.
   if ( MiGetUpdateForView( vXSLT ) == 0 )
   {
      MessageSend( vSubtask, "PN00207", "XSLT Maintenance",
                   "This dialog is not updatable.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "CloneFunction", "C" );

   return( 0 );
}

//./ ADD NAME=MergeXSLTWindow
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      MergeXSLTWindow
//
//  PURPOSE:    This function simply sets a flag to identify the
//              as merge instead of clone for the TZPNCWDD SEL_CLONE
//              dialog.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
MergeXSLTWindow( zVIEW vSubtask )
{
   zVIEW vWork;
   zVIEW vXSLT;

   GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );

   // If the dialog is not updatable, then don't do the merge.
   if ( MiGetUpdateForView( vXSLT ) == 0 )
   {
      MessageSend( vSubtask, "PN00208", "XSLT Maintenance",
                   "This dialog is not updatable.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "CloneFunction", "M" );

   return( 0 );
}

//./ ADD NAME=WND_UPD_SelectAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_SelectAction
//
//  PURPOSE:    To select an action for a window Event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_SelectAction( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vEvents;
   zVIEW vWork;
   zLONG lType;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, "Event", "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00209", "XSLT Maintenance",
                   "No Event Selected",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( 0 );
   }

   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vXSLT, "WndEvent",
                                       "Type", lType, 0 ) != zCURSOR_SET )
   {
      CreateTemporalMetaEntity( vSubtask, vXSLT, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vXSLT, "WndEvent", "Type", lType );
   }

   // Set flag for LIST_ACT window graying and select behavior.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "W" );

   return( 0 );
}

//./ ADD NAME=WND_UPD_RemoveAction
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_RemoveAction
//
//  PURPOSE:    To Remove an action for a window Event
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_RemoveAction( zVIEW vSubtask )
{
zVIEW vEvents;
zVIEW vXSLT;
zLONG lType;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vXSLT, "WndEvent", "Type",
                                       lType, "" ) == zCURSOR_SET )
   {
      DeleteEntity( vXSLT, "WndEvent", zREPOS_PREV );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    WND_UPD_AddAction
//
// PURPOSE:  This function creates an Action entity and includes it to.
//           the selected Window.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
WND_UPD_AddAction( zVIEW vSubtask )
{
   zVIEW  vWork;

   WND_UPD_SelectAction( vSubtask );
   LIST_ACT_AddAction( vSubtask );

   // Set flag for select behavior.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "W" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_Init
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_Init( zVIEW vSubtask )
{
   zVIEW vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
// CreateTemporalSubobjectVersion( vXSLT, "Window" );
   /* NO MORE WndPhysOverride
   if ( MiGetUpdateForView( vXSLT ) &&
        CheckExistenceOfEntity( vXSLT, "WndPhysOverride" ) != 0 )
   {
      CreateMetaEntity( vSubtask, vXSLT, "WndPhysOverride", zPOS_AFTER );
   }
   */
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_Postbuild
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_Postbuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vTZXSLTS;
   zVIEW  vXSLTList;
   zULONG ulFlag;
   zSHORT nRC;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( (zPLONG) &ulFlag, vXSLT,
                            "Window", "GeneralFlag" );
   if ( ulFlag & 0x00000001 )
      SetCtrlState( vSubtask, "ScrollBars", zCONTROL_STATUS_CHECKED, TRUE );

   if ( ulFlag & 0x00000002 )
      SetCtrlState( vSubtask, "SelectFirstEdit", zCONTROL_STATUS_CHECKED, TRUE );

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
   {
      SetCtrlState( vSubtask, "ScrollBars", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SelectFirstEdit", zCONTROL_STATUS_ENABLED, FALSE );
   }

   nRC = GetViewByName( &vXSLTList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   if ( nRC <= 0 )
   {
       GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_ANY );
       CreateViewFromViewForTask( &vXSLTList, vTZXSLTS, 0 );
       SetNameForView( vXSLTList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      DLG_MAINT_Postbuild
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
DLG_MAINT_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZXSLTS;
   zVIEW  vXSLTList;
   zSHORT nRC;

   nRC = GetViewByName( &vXSLTList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   if ( nRC <= 0 )
   {
       GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_ANY );
       CreateViewFromViewForTask( &vXSLTList, vTZXSLTS, 0 );
       SetNameForView( vXSLTList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: WND_UPD_SetColors
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
WND_UPD_SetColors( zVIEW vSubtask,
                   zVIEW vXSLT,
                   zVIEW vPE )
{
// zLONG  lZKey;
// zSHORT nRC;

#if 0
   // If any WndColorDefs have been added or deleted from the PE, then
   // make sure we add or delete them from the XSLT.
   nRC = SetCursorFirstEntity( vXSLT, "WndClrOverride", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vXSLT, "WndColorDef", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vPE, "WndColorDef",
                                           "ZKey", lZKey, "" );
      if ( nRC < zCURSOR_SET )
         DeleteEntity( vXSLT, "WndClrOverride", zREPOS_NONE );

      nRC = SetCursorNextEntity( vXSLT, "WndClrOverride", "" );
   }

   nRC = SetCursorFirstEntity( vPE, "WndColorDef", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vPE, "WndColorDef", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vXSLT, "WndColorDef",
                                           "ZKey", lZKey, "Window" );
      if ( nRC < zCURSOR_SET )
      {
         // Get the position of the PE WndColorDef and create the
         // XSLTs WndColorDef in the same relative position.
         nRC = (zSHORT) GetRelativeEntityNumber( vPE, "WndColorDef",
                                                 0, 0L );

         // If nRC is 0 then the PE WndColorDef is the first entity
         // so the XSLT's WndClrOverride needs to be created before
         // its first WndClrOverride.
         if ( nRC == 0 )
         {
            SetCursorFirstEntity( vXSLT, "WndClrOverride", 0 );
            CreateMetaEntity( vSubtask, vXSLT, "WndClrOverride", zPOS_BEFORE );
         }
         else
         {
            // Position the XSLT on the WndClrOverride at relative
            // position nRC - 1.  Because relative positions start at
            // 0, you don't think you need to do the minus 1 but you do.
            SetEntityCursor( vXSLT, "WndClrOverride", 0,
                             zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                             (zSHORT) (nRC - 1), 0, 0 );
            CreateMetaEntity( vSubtask, vXSLT, "WndClrOverride", zPOS_AFTER );
         }

         IncludeSubobjectFromSubobject( vXSLT, "WndColorDef",
                                        vPE, "WndColorDef", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vXSLT, "WndColor",
                                        vPE, "Color", zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vPE, "WndColorDef", "" );
   }
#endif

   return( 0 );
} // WND_UPD_SetColors

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_UpdateColors
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_UpdateColors( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vPE;
   zSHORT nRC;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntityByEntityCsr( vPE, "WindowStyle",
                                          vXSLT, "WndStyle", "" );
   if ( nRC < zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByAttr( vPE, "WindowStyle", "Tag",
                                       vXSLT, "WndStyle", "Tag",
                                       "" ) < zCURSOR_SET )
      {
         return( 0 );
      }
   }

   // If XSLT not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
      SetViewUpdate( vXSLT );

   nRC = SetCursorFirstEntityByString( vPE, "Color", "Tag",
                                       "Windows Default", "" );

#if 0
   if ( CheckExistenceOfEntity( vXSLT, "WndClrOverride" ) == zCURSOR_SET )
   {
      // If any WndColorDefs have been added or deleted from the PE, then
      // make sure we add or delete them from the XSLT.
      WND_UPD_SetColors( vSubtask, vXSLT, vPE );
   }
   else
   {
      // Create CtrlColorDefs, all with the COLOR_WINDOW_DFLT CtrlColor.
      nRC = SetCursorFirstEntity( vPE, "WndColorDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vXSLT, "WndClrOverride", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vXSLT, "WndColorDef",
                                        vPE, "WndColorDef", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vXSLT, "WndColor",
                                        vPE, "Color", zPOS_AFTER );
         nRC = SetCursorNextEntity( vPE, "WndColorDef", "" );
      }
   }
#endif

   // If XSLT not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
      SetViewReadOnly( vXSLT );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_ColorsOK
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_ColorsOK( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zBOOL  bAllSet;
// zSHORT nRC;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   // Loop through all the WndColorDefs.  If they all have WndColors set
   // to COLOR_WINDOW_DFLT, then delete all the WndColorDefs.
   bAllSet = TRUE;

#if 0
   nRC = SetCursorFirstEntity( vXSLT, "WndClrOverride", "" );
   while ( nRC >= zCURSOR_SET && bAllSet )
   {
      if ( CompareAttributeToInteger( vXSLT, "WndColor",
                                      "RGB_Color", zCOLOR_WINDOW_DFLT ) != 0 )
      {
         bAllSet = FALSE;
      }

      nRC = SetCursorNextEntity( vXSLT, "WndClrOverride", "" );
   }
#endif

   if ( bAllSet == TRUE )
   {
      // If XSLT not checked out, View is read only. To set a attribute
      // View has to be updateble. After change attribute set View read only again
      if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
         SetViewUpdate( vXSLT );

#if 0
      nRC = SetCursorFirstEntity( vXSLT, "WndClrOverride", "" );
      while ( nRC >= zCURSOR_SET )
         nRC = DeleteEntity( vXSLT, "WndClrOverride", zREPOS_NEXT );
#endif

      if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
         SetViewReadOnly( vXSLT );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_UpdateFonts
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_UpdateFonts( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vPE;
   zSHORT nRC;
   zLONG  lZKey;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntityByEntityCsr( vPE, "WindowStyle",
                                          vXSLT, "WndStyle", "" );
   if ( nRC < zCURSOR_SET )
      return( 0 );

   nRC = SetCursorFirstEntityByString( vPE, "Font", "Tag",
                                       "Windows Default", "" );

   if ( CheckExistenceOfEntity( vXSLT, "WndFontOverride" ) == zCURSOR_SET )
   {
      /* If any WndFontDefs have been added or deleted from the PE, then
         make sure we add or delete them from the XSLT. */
      nRC = SetCursorFirstEntity( vXSLT, "WndFontOverride", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vXSLT, "WndFontDef", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vPE, "WndFontDef",
                                              "ZKey", lZKey, "" );
         /*
         nRC = SetCursorFirstEntityByEntityCsr( vPE, "WndFontDef",
                                                vXSLT, "WndFontDef",
                                                "" );
         */
         if ( nRC < zCURSOR_SET )
         {
            DeleteEntity( vXSLT, "WndFontOverride", zREPOS_NONE );
         }
         nRC = SetCursorNextEntity( vXSLT, "WndFontOverride", "" );
      }

      nRC = SetCursorFirstEntity( vPE, "WndFontDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vPE, "WndFontDef", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vXSLT, "WndFontDef",
                                              "ZKey", lZKey, "Window" );
         /*
         nRC = SetCursorFirstEntityByEntityCsr( vXSLT, "WndFontDef",
                                                vPE, "WndFontDef", "Window" );
         */
         if ( nRC < zCURSOR_SET )
         {
            /* Get the position of the PE WndFontDef and create the
               XSLTs WndFontDef in the same relative position. */
            nRC = (zSHORT) GetRelativeEntityNumber( vPE, "WndFontDef", 0, 0L );

            /* If nRC is 0 then the PE WndFontDef is the first entity
               so the XSLT's WndFontOverride needs to be created before
               its first WndFontOverride. */
            if ( nRC == 0 )
            {
               SetCursorFirstEntity( vXSLT, "WndFontOverride", 0 );
               CreateMetaEntity( vSubtask, vXSLT, "WndFontOverride", zPOS_BEFORE );
            }
            else
            {
               /* Position the XSLT on the WndFontOverride at relative
                  position nRC - 1.  Because relative positions start at
                  0, you don't think you need to do the minus 1 but you
                  do. */
               SetEntityCursor( vXSLT, "WndFontOverride", 0,
                                zPOS_FIRST | zPOS_RELATIVE, 0, 0, 0,
                                (zSHORT) (nRC - 1), 0, 0 );
               CreateMetaEntity( vSubtask, vXSLT, "WndFontOverride", zPOS_AFTER );
            }

            IncludeSubobjectFromSubobject( vXSLT, "WndFontDef",
                                           vPE, "WndFontDef", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vXSLT, "WndFont",
                                           vPE, "Font", zPOS_AFTER );
         }
         nRC = SetCursorNextEntity( vPE, "WndFontDef", "" );
      }
   }
   else
   {
#if 0
      // Create CtrlColorDefs, all with the COLOR_WINDOW_DFLT CtrlColor.
      nRC = SetCursorFirstEntity( vPE, "WndFontDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vXSLT, "WndFontOverride", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vXSLT, "WndFontDef",
                                        vPE, "WndFontDef", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vXSLT, "WndFont",
                                        vPE, "Font", zPOS_AFTER );
         nRC = SetCursorNextEntity( vPE, "WndFontDef", "" );
      }
#endif
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_FontsOK
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_FontsOK( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zSHORT nRC;
   zBOOL  bAllSet;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   /* Loop through all the WndFontDefs.   If they all have WndFonts set
      to COLOR_WINDOW_DFLT, then delete all the WndFontDefs. */
   bAllSet = TRUE;
   nRC = SetCursorFirstEntity( vXSLT, "WndFontOverride", "" );
   while ( nRC >= zCURSOR_SET && bAllSet )
   {
      if ( CompareAttributeToString( vXSLT, "WndFont",
                                     "Tag", "Windows Default" ) != 0 )
         bAllSet = FALSE;
      nRC = SetCursorNextEntity( vXSLT, "WndFontOverride", "" );
   }
   if ( bAllSet == TRUE )
   {
      nRC = SetCursorFirstEntity( vXSLT, "WndFontOverride", "" );
      while ( nRC >= zCURSOR_SET )
         nRC = DeleteEntity( vXSLT, "WndFontOverride", zREPOS_NEXT );
   }

   return( 0 );
}

zSHORT
fnWndUpdate( zVIEW vSubtask, zBOOL bStayOnWindow )
{
   zVIEW    vTZPNTROO;
   zVIEW    vXSLT;
   zVIEW    vXSLTL;
   zCHAR    szNewTag[ 34 ];
   zULONG   ulFlag;

   // First, if a window name changed, make sure it is unique and
   // make the appropriate change in the painter active object
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // If the window name was changed, set the appropriate new name in
   // the painter's active list UNLESS a duplicate name was created, if
   // a duplicate name is created, issue an ERROR.
   GetStringFromAttribute( szNewTag, zsizeof( szNewTag ), vXSLT, "Window", "Tag" );
   if ( szNewTag[ 0 ] == 0 ||
        (SetCursorFirstEntityByString( vXSLTL, "Window", "Tag",
                                       szNewTag, 0 ) == zCURSOR_SET &&
         SetCursorNextEntityByString( vXSLTL, "Window", "Tag",
                                      szNewTag, 0 ) == zCURSOR_SET ) )
   {
      MessageSend( vSubtask, "PN00210", "XSLT Maintenance",
                   "Window tag is null or not unique",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "WindowName" );
      return( 1 );
   }

   GetIntegerFromAttribute( (zPLONG) &ulFlag, vXSLT,
                            "Window", "GeneralFlag" );
   if ( GetCtrlState( vSubtask, "ScrollBars",
                      zCONTROL_STATUS_CHECKED ) == TRUE )
   {
      ulFlag |= 0x00000001;
   }
   else
   {
      ulFlag &= ~0x00000001;
   }

   if ( GetCtrlState( vSubtask, "SelectFirstEdit",
                      zCONTROL_STATUS_CHECKED ) == TRUE )
   {
      ulFlag |= 0x00000002;
   }
   else
   {
      ulFlag &= ~0x00000002;
   }

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
      SetViewUpdate( vXSLT );

   SetAttributeFromInteger( vXSLT, "Window", "GeneralFlag", ulFlag );

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
      SetViewReadOnly( vXSLT );

   // Tell the painter to update the window name if appropriate
   fnPainterCall( zMSG_UPDATEACTIVEWINDOWNAME, vSubtask, 0, szNewTag, zsizeof( szNewTag ) );

#ifndef __WIN32__
   // Refresh the window list in the event the window name has changed
   if ( GetViewByName( &vPanelList, "TZWND_LIST_Subtask", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vPanelList, "ListBox1" );
#endif

   if ( bStayOnWindow == FALSE )
      fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );

   AcceptSubobject( vXSLT, "Window" );

   if ( bStayOnWindow == FALSE )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                         zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

//./ ADD NAME=WND_UPD_OK
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      WND_UPD_OK
//
//  PURPOSE:    To Update the Window Detail
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
WND_UPD_OK( zVIEW vSubtask )
{
   return( fnWndUpdate( vSubtask, FALSE ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    WND_UPD_ReturnFromSubwindow
//
// PURPOSE:  This function sets up the Event entity in TZPNEVWO from
//           the selected Event entity in TZXSLTL.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
WND_UPD_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vXSLTW;
   zVIEW    vEvents;

   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Delete WndEvent entity if there is not a relationship to Action
   if ( CheckExistenceOfEntity( vXSLTW, "WndEvent" ) == zCURSOR_SET )
      if ( CheckExistenceOfEntity( vXSLTW, "WndAct" ) != zCURSOR_SET )
         DeleteEntity( vXSLTW, "WndEvent", zREPOS_PREV );

   // Set up the TZPNEVWO object instance from TZXSLT and refresh
   // the List Box.
   fnSetupWndEventList( vXSLTW );
   RefreshCtrl( vSubtask, "EventListBox" );

   return( 0 );
}

//./ ADD NAME=UpdateNextWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UpdateNextWindow
//
//  PURPOSE:    To Update the next Window Detail
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdateNextWindow( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vXSLTL;

   // First, update the current window.
   if ( fnWndUpdate( vSubtask, TRUE ) == 0 )
   {
      GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
      if ( SetCursorNextEntity( vXSLT, "Window", "" ) != 0 )
         SetCursorFirstEntity( vXSLT, "Window", "" );

      if ( GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
         SetViewFromView( vXSLTL, vXSLT );

      WND_UPD_ReturnFromSubwindow( vSubtask );

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "WindowName" );
   }

   return( 0 );
}

//./ ADD NAME=UpdatePreviousWindow
// Source Module=tzxsltdd.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      UpdatePreviousWindow
//
//  PURPOSE:    To Update the previous Window Detail
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdatePreviousWindow( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vXSLTL;

   // First, update the current window.
   if ( fnWndUpdate( vSubtask, TRUE ) == 0 )
   {
      GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
      if ( SetCursorPrevEntity( vXSLT, "Window", "" ) != 0 )
         SetCursorLastEntity( vXSLT, "Window", "" );

      if ( GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
         SetViewFromView( vXSLTL, vXSLT );

      WND_UPD_ReturnFromSubwindow( vSubtask );

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "WindowName" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    WND_UPD_OperationEdit
//
// PURPOSE:  This function sets up the TZXSLT view to the Action Operation
//           entity corresponding to EventAct for the current Event.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
WND_UPD_OperationEdit( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vXSLTW;
   zCHAR    sTag[ 34 ];

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );

   // If EventAct exists, set cursor for Update Action window.  Otherwise, return an error.

   if ( CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( sTag, zsizeof( sTag ), vWork, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vXSLTW, "Action", "Tag", sTag, "" );

      if ( CheckExistenceOfEntity( vXSLTW, "ActOper" ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( sTag, zsizeof( sTag ), vXSLTW, "ActOper", "Name" );
         SetCursorFirstEntityByString( vXSLTW, "Operation", "Name", sTag, "XSLT" );
         TransferToEditor( vSubtask, vXSLTW, "PN", 0 );
      }
      else
      {
         MessageSend( vSubtask, "PN00211", "XSLT Maintenance",
                      "No Operation Exists For Action.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   {
      // Position on Action that corresponds to EventAct entity instance.
      MessageSend( vSubtask, "PN00212", "XSLT Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    WND_UPD_UpdateAction
//
// PURPOSE:  This function sets up the TZXSLT view to the Action
//           entity corresponding to EventAct for the current Event.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
WND_UPD_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vXSLTW;
   zCHAR    sTag[ 34 ];

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );

   // If EventAct exists, set cursor for Update Action window.
   // Otherwise, return an error.
   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      GetStringFromAttribute( sTag, zsizeof( sTag ), vWork, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vXSLTW, "Action", "Tag", sTag, "" );
      CreateTemporalSubobjectVersion( vXSLTW, "Action" );
   }
   else
   {
      // Issue an error
      MessageSend( vSubtask, "PN00213", "XSLT Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   // Set flag for LIST_ACT window graying and select behavior.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "W" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_Init
//
// PURPOSE:  This function sets up the TZACTION view for the
//           OPT_UPD window.   It is the same as TZXSLT if a current
//           OptAct exits.  It is null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_Init( zVIEW vSubtask )
{
   zVIEW    vXSLTA;
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zVIEW    vXSLTL;
   zVIEW    vEvents;
   zSHORT   nRC;

   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vXSLTA );

   // If an Action for the Option exists, position on the linked Action
   // instance and set TZACTION to TZXSLT.
   if ( CheckExistenceOfEntity( vXSLTO, "OptAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vXSLTA, vXSLTW, 0 );
      SetCursorFirstEntityByAttr( vXSLTA, "Action", "ZKey",
                                  vXSLTO, "OptAct", "ZKey", "" );
      SetNameForView( vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // If not, set TZACTION to empty instance for auto graying.
      fnCreateEmptyActionView( vSubtask, vXSLTW );
    }

   // Make sure TZXSLTL view exits.
   if ( GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      CreateViewFromViewForTask( &vXSLTL, vXSLTW, 0 );
      SetNameForView( vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   }

   // Initialize the TZPNEVWO work object if it's empty.
   nRC = GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   if ( nRC )
   {
      ActivateEmptyObjectInstance( &vEvents, "TZPNEVWO",
                                   vSubtask, zMULTIPLE );
      SetNameForView( vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity( vEvents, "Event" ) != zCURSOR_SET )
   {
      CreateEntity( vEvents, "Event", zPOS_AFTER );
      SetAttributeFromString( vEvents, "Event", "Tag",
                              "Menu Item Selected");
      CreateEntity( vEvents, "EventAct", zPOS_AFTER );
   }

   if ( CheckExistenceOfEntity( vXSLTO, "OptAct" ) == zCURSOR_SET )
        SetAttributeFromAttribute( vEvents,"EventAct", "Tag",
                                   vXSLTO, "OptAct", "Tag" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_OK
//
// PURPOSE:  This function tells the Painter to refresh the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_OK( zVIEW vSubtask )
{
   zVIEW   vXSLTO;
   zVIEW   vTmpOptions;
   zVIEW   vTemp;
   zSHORT  nRC;
   zUSHORT uLevel;
   zBOOL   bTagFound;
   zCHAR   szTag[ 256 ];
   zCHAR   szEntityName[ 33 ];

   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLTO, "Option", "NLS_Text" );

   // Make sure that the option tag is unique
   GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLTO, "Option", "Tag" );
   CreateViewFromViewForTask( &vTmpOptions, vXSLTO, 0 );
   while ( ResetViewFromSubobject( vTmpOptions ) == 0 )
   {
   }

   CreateViewFromViewForTask( &vTemp, vTmpOptions, 0 );
   bTagFound = 0;
   DefineHierarchicalCursor( vTmpOptions, "Menu" );
   nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vTmpOptions );

   // Loop through all the Options and OptOpts for this Menu and check the
   // tags.
   while ( nRC >= zCURSOR_SET && uLevel > 3 )
   {
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vTmpOptions, "OptOpt" );
      }

      if ( CompareAttributeToString( vTmpOptions, "Option", "Tag", szTag ) == 0 )
      {
         SetViewFromView( vTemp, vTmpOptions );
         DropHierarchicalCursor( vTemp );
         if ( CompareEntityToEntity( vTemp, "Option", vXSLTO, "Option" ) != 0 )
         {
            bTagFound = 1;
            break;
         }
      }

      nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vTmpOptions );
   }

   DropHierarchicalCursor( vTmpOptions );
   DropView( vTmpOptions );
   DropView( vTemp );
   if ( bTagFound )
   {
      MessageSend( vSubtask, "PN00214", "XSLT Maintenance",
                   "Tag is not unique!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetFocusToCtrl( vSubtask, "Tag" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   AcceptSubobject( vXSLTO, "Option" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OPT_UPD_DisableActionButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OPT_UPD_DisableActionButton( zVIEW vSubtask )
{
   zVIEW vEvents;
   zVIEW vAction;
   zBOOL bEnable = FALSE;

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
            bEnable = TRUE;
      }

      SetCtrlState( vSubtask, "UpdateAction", zCONTROL_STATUS_ENABLED, bEnable );
   }

   bEnable = FALSE;

   if ( GetViewByName( &vAction, "TZACTION", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vAction, "ActOper" ) >= zCURSOR_SET)
   {
      bEnable = TRUE;
   }

   EnableAction( vSubtask, "OPT_UPD_OperationEdit", bEnable );

   return( 0 );
} // OPT_UPD_DisableActionButton

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      OPT_UPD_RemoveAction
//
//  PURPOSE:    To Remove an action from a menu option
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_RemoveAction( zVIEW vSubtask )
{
   zVIEW vXSLTO;
   zVIEW vEvents;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );
   ExcludeEntity( vXSLTO, "OptAct", zREPOS_PREV );
   fnCreateEmptyActionView( vSubtask, vXSLTO );

   OPT_UPD_DisableActionButton( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_SelectAction
//
// PURPOSE:  This function positions on the correct Action entity if
//           an OptAct entity exists and sets an indicator to note
//           that this is an Include OptAct for the SEL_ATTR window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_SelectAction( zVIEW vSubtask )
{
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zVIEW    vWork;

   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLTO, "OptAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vXSLTW, "Action", "Tag",
                                  vXSLTO, "OptAct", "Tag", "" );
   }

   // Set flag for Action include for Option.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "O" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_UpdateAction
//
// PURPOSE:  This function sets the TZXSLT cursor to the correct
//           Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zVIEW    vWork;

   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLTO, "OptAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vXSLTW, "Action", "Tag",
                                  vXSLTO, "OptAct", "Tag", "" );
      CreateTemporalSubobjectVersion( vXSLTW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00215", "XSLT Maintenance",
                   "No Action exists for Item.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "W" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_AddAction
//
// PURPOSE:  This function creates an Action entity and includes it to.
//           the selected Option.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_AddAction( zVIEW vSubtask )
{
   zVIEW    vWork;

   OPT_UPD_SelectAction( vSubtask );
   LIST_ACT_AddAction( vSubtask );
   LIST_ACT_SelectAction( vSubtask );

   // Set flag for Action include for Option.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "O" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_OperationEdit
//
// PURPOSE:  This function calls editor after positioning on operation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_OperationEdit( zVIEW vSubtask )
{
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;
   zCHAR    sTag[ 34 ];

   GetViewByName( &vXSLTC, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLTC, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vXSLTC, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( sTag, zsizeof( sTag ), vXSLTC, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vXSLTW, "Action", "Tag", sTag, "" );

      if ( CheckExistenceOfEntity( vXSLTW, "ActOper" ) == zCURSOR_SET )
      {
         SetCursorFirstEntityByEntityCsr( vXSLTW, "Operation", vXSLTW, "ActOper", "XSLT" );
         TransferToEditor( vSubtask, vXSLTW, "PN", 0 );
      }
      else
      {
         MessageSend( vSubtask, "PN00216", "XSLT Maintenance",
                      "No Operation Exists For Action.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   {
      MessageSend( vSubtask, "PN00217", "XSLT Maintenance",
                   "No Action exists for Menu Item.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OPT_UPD_ReturnFromSub
//
// PURPOSE:  Make sure TZACTION is the View for the Action just
//           selected.  TZACTION to be a null View, if no OptAct
//           existed prior to the Select.
//           Also drop the TZACTION View if it is null.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OPT_UPD_ReturnFromSub( zVIEW vSubtask )
{
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zVIEW    vXSLTA;
   zVIEW    vEvents;

   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // If an Action for the Option exists, position on the linked Action
   // instance and set TZACTION to TZXSLT.
   if ( CheckExistenceOfEntity( vXSLTO, "OptAct" ) == zCURSOR_SET )
   {
      // Drop TZACTION work OI, if it is empty.
      if ( CheckExistenceOfEntity( vXSLTA, "Window" ) != zCURSOR_SET )
         DropObjectInstance( vXSLTA );

      CreateViewFromViewForTask( &vXSLTA, vXSLTW, 0 );
      SetCursorFirstEntityByAttr( vXSLTA, "Action", "Tag", vXSLTO, "OptAct", "Tag", "" );
      SetNameForView( vXSLTA, "TZACTION", vSubtask, zLEVEL_TASK );
      SetAttributeFromAttribute( vEvents,"EventAct", "Tag", vXSLTO, "OptAct", "Tag" );
   }
   else
      SetAttributeFromString( vEvents, "EventAct", "Tag", "" );

   OPT_UPD_DisableActionButton( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OptContextMappingInit
//
// PURPOSE:  This function builds the MapDomain View for the Context
//           Combo Box, if there is current mapping.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OptContextMappingInit( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   GetViewByName( &vXSLT, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );

   if ( CheckExistenceOfEntity( vXSLT, "OptMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current OptMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vXSLT, "OptMapER_Domain", "ZKey");
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   }
   else
      ActivateEmptyObjectInstance( &vDomain, "TZDGSRCO", vSubtask, 0 );

   SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_OPT_SelectMapping
//
// PURPOSE:  This function does the necessary exclude/includes
//           to set up attribute mapping.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_OPT_SelectMapping( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vLOD;
   zVIEW    vTZPNTROO;
   zVIEW    vTZXSLTL;
   zSHORT   nRC;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vXSLT, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META ) )
   {
      DropMetaOI( vSubtask, vLOD );
      return( 0 );
   }

   if ( nRC <= 0 )
   {
      // If current OptMapContext exists, exclude it.
      if ( CheckExistenceOfEntity( vXSLT, "OptMapContext" ) == zCURSOR_SET )
         ExcludeEntity( vXSLT, "OptMapContext", zREPOS_PREV );

      // If current OptMapLOD_Attribute exists, exclude it.
      if ( CheckExistenceOfEntity( vXSLT, "OptMapLOD_Attribute" ) == zCURSOR_SET )
      {
         ExcludeEntity( vXSLT, "OptMapLOD_Attribute", zREPOS_PREV );
      }

      return( 0 );
   }

   // Alter mapping only if it changed from existing.
   if ( CheckExistenceOfEntity( vXSLT, "OptMapLOD_Attribute" ) != zCURSOR_SET ||
        CompareAttributeToAttribute( vXSLT, "OptMapLOD_Attribute", "ZKey",
                                     vLOD, "LOD_Attribute", "ZKey" ) != 0 )
   {
      // If current OptMapContext exists, exclude it.
      if ( CheckExistenceOfEntity( vXSLT, "OptMapContext" ) == zCURSOR_SET )
         ExcludeEntity( vXSLT, "OptMapContext", zREPOS_PREV );

      // If current OptMapLOD_Attribute exists, exclude it.
      if ( CheckExistenceOfEntity( vXSLT, "OptMapLOD_Attribute" ) == zCURSOR_SET )
      {
         ExcludeEntity( vXSLT, "OptMapLOD_Attribute", zREPOS_PREV );
      }

      IncludeSubobjectFromSubobject( vXSLT, "OptMapLOD_Attribute",
                                     vLOD, "LOD_Attribute", zPOS_AFTER );
   }

   // Set up default View Obj Ref and Entity for next mapping.
   if ( CheckExistenceOfEntity( vTZPNTROO, "CtrlWork" ) != zCURSOR_SET )
      CreateEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );

   SetAttributeFromAttribute( vTZPNTROO, "CtrlWork", "LastMapViewZKey",
                              vXSLT, "OptMapView", "ZKey" );
   SetAttributeFromAttribute( vTZPNTROO, "CtrlWork", "LastMapEntityZKey",
                              vXSLT, "OptMapRelatedEntity", "ZKey" );
   DropMetaOI( vSubtask, vLOD );

   // Set up Domain Context mapping.
   OptContextMappingInit( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_OPT_Init
//
// PURPOSE:  This function activates the LOD associated with the
//           current view.  If no view is currently defined, it
//           temporarily sets up a dummy view.  I'm not sure what
//           it should eventually do.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_OPT_Init( zVIEW vSubtask )
{
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zVIEW    vTZPNTROO;
   zLONG    lZKey;

   GetViewByName( &vXSLTW, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   // If XSLT not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vXSLTW, zSOURCE_XSLT_META ) )
      SetViewUpdate( vXSLTO );

   // Create work entity, if it doesn't exist.
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vTZPNTROO, "CtrlWork" ) != zCURSOR_SET )
      CreateEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );

   // If OptMap doesn't exist, create it.
   if ( CheckExistenceOfEntity( vXSLTO, "OptMap" ) != zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vXSLTO, "OptMap", zPOS_AFTER );
   }

   // If OptMapView exists, use the corresponding LOD for mapping.
   // Use OptMapView ZKey to locate ViewObjRef and then
   // LOD ZKey to locate LPLR LOD meta.
   if ( CheckExistenceOfEntity( vXSLTO, "OptMapView" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vXSLTO, "OptMapView", "ZKey");
      SetCursorFirstEntityByInteger( vXSLTW, "ViewObjRef", "ZKey", lZKey, "" );
      GetIntegerFromAttribute( &lZKey, vXSLTW, "LOD", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
      SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
      // Select current mapping in LOD
      if ( CheckExistenceOfEntity( vXSLTO, "OptMapLOD_Attribute" ) == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vXSLTO, "OptMapRelatedEntity", "ZKey" );
         SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, "" );
         SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
         GetIntegerFromAttribute( &lZKey, vXSLTO, "OptMapLOD_Attribute", "ZKey" );
         SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute", "ZKey", lZKey, "" );
         SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );
      }
   }
   else
   {
      // If no current mapping exists, try to use the last mapping
      // specified for the Entity and Attribute list boxes.  Also
      // set view to last view used.
      GetIntegerFromAttribute( &lZKey, vTZPNTROO, "CtrlWork", "LastMapViewZKey");
      if ( SetCursorFirstEntityByInteger( vXSLTW, "ViewObjRef", "ZKey", lZKey, "" ) == zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vXSLTO, "OptMapView" ) == zCURSOR_SET )
            ExcludeEntity( vXSLTO, "OptMapView", zREPOS_PREV );

         IncludeSubobjectFromSubobject( vXSLTO, "OptMapView", vXSLTW, "ViewObjRef", zPOS_AFTER );
         GetIntegerFromAttribute( &lZKey, vXSLTW, "LOD", "ZKey" );
         SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
         ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
         SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

         // Position on correct LOD_Entity
         GetIntegerFromAttribute( &lZKey, vTZPNTROO, "CtrlWork", "LastMapEntityZKey");
         SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, "" );
         SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
      }
   }

   // if XSLT not checked out, set  View read only ->
   // the user cannot update the values in Detail Windows
   if ( !ComponentIsCheckedOut( vSubtask, vXSLTW, zSOURCE_XSLT_META ) )
      SetViewReadOnly( vXSLTO );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_OPT_SelectView
//
// PURPOSE:  This function
//           1. Does the necessary exclude/include of OptMapView.
//           2. Activates the LOD for the View just selected.
//           3. Refreshes the Entity and Attribute List Boxes and sets
//              the LOD Entity and Attribute select states.
//
//           Note that steps 2 and 3 are not done if the include is
//           to the same LOD type as for the previous OptMapView.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_OPT_SelectView( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zVIEW    vXSLTO;
   zVIEW    vXSLTW;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &vXSLTO, "TZXSLTOPT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLTL", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   // Drop the SEL_LOD meta OI, if it exists.
   if ( nRC >= 0 )
      DropMetaOI( vSubtask, vLOD );

   if ( CheckExistenceOfEntity( vXSLTO, "OptMapView" ) == zCURSOR_SET )
   {
      // Activate the LOD for the current ViewObjRef subobject.
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );
      GetIntegerFromAttribute( &lZKey, vXSLTW, "LOD", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
      SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

      SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
      SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );
      RefreshCtrl( vSubtask, "EntityList" );
      RefreshCtrl( vSubtask, "AttributeList" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_OPT_SelectEntity
//
// PURPOSE:  This function refreshes the Entity and Attribute List Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_OPT_SelectEntity( zVIEW vSubtask )
{
   zVIEW  vLOD;

   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );
   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZXSLTDD_TransferToEditor
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_TransferToEditor( zVIEW vSubtask )
{
   zVIEW  vMETAOBJECT;

   // Get Access to dialog Object
   GetViewByName( &vMETAOBJECT, "TZXSLT", vSubtask, zLEVEL_TASK );

   // Disable focus to this window
   SetWindowState( vSubtask, zWINDOW_STATUS_ENABLED, FALSE );

   TransferToEditor( vSubtask, vMETAOBJECT, "WD", 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ControlPostBuild( zVIEW vSubtask )
{
   zVIEW   vTZPESRCO;
   zVIEW   vTZ__PRFO;
   zVIEW   vTZPNTBAR;
   zSHORT  nRC;

   // Get Views
   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
   oTZ__PRFO_GetViewToProfile( &vTZ__PRFO, "WD", vSubtask, zCURRENT_OI );

   if ( GetViewByName( &vTZPNTBAR, "TZPNTBAR", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vTZPNTBAR );

   ActivateEmptyObjectInstance( &vTZPNTBAR, "TZPNTBAR", vSubtask, zMULTIPLE );
   SetNameForView( vTZPNTBAR, "TZPNTBAR", vSubtask, zLEVEL_TASK );

   OrderEntityForView( vTZPESRCO, "ControlDef", "Tag A" );

   for ( nRC = SetCursorFirstEntity( vTZPESRCO, "ControlDef", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZPESRCO, "ControlDef", "" ) )
   {
      // Added all not deleted controls with painter icons
      if ( CompareAttributeToString( vTZPESRCO, "ControlDef", "PainterBitmap", "" ) != 0 &&
           CompareAttributeToInteger( vTZPESRCO, "ControlDef", "Invisible", 1 ) != 0  && // no deleted Controls
           CompareAttributeToInteger( vTZPESRCO, "ControlDef", "Invisible", -2 ) != 0 )  // no XSLT Control
      {
         CreateEntity( vTZPNTBAR, "ToolBarIcons", zPOS_AFTER );
         SetAttributeFromAttribute( vTZPNTBAR, "ToolBarIcons", "Tag", vTZPESRCO, "ControlDef", "Tag" );
         if ( SetEntityCursor( (zVIEW) vTZ__PRFO, "ControlDef", "Tag", zPOS_FIRST | zQUAL_ENTITYATTR,
                               vTZPESRCO, "ControlDef", "Tag", 0, 0, 0 ) >= zCURSOR_SET )
         {
            SetAttributeFromInteger( vTZPNTBAR, "ToolBarIcons", "OnPalette", 1 );
         }
         else
         {
            SetAttributeFromInteger( vTZPNTBAR, "ToolBarIcons", "OnPalette", 0 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_Control_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_Control_Cancel( zVIEW vSubtask )
{
   zVIEW  vTZPNTBAR;

   if ( GetViewByName( &vTZPNTBAR, "TZPNTBAR", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vTZPNTBAR );

   return( 0 );
} // TZXSLTDD_Control_Cancel

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_Control_OK( zVIEW vSubtask )
{
   zVIEW  vTZ__PRFO;
   zVIEW  vTZPNMWIN;
   zVIEW  vTZPNTBAR;
   zSHORT nRC;

   GetViewByName( &vTZPNTBAR, "TZPNTBAR", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntityByInteger( vTZPNTBAR, "ToolBarIcons", "OnPalette", 1, "" );

   if ( nRC < zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN???OK", "XSLT Maintenance",
                   "At least one control must\nbe selected on the Palette",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
      return( 1 );
   }

   oTZ__PRFO_GetViewToProfile( &vTZ__PRFO, "WD", vSubtask, zCURRENT_OI );

   nRC = SetCursorFirstEntity( vTZ__PRFO, "ControlDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = DeleteEntity( vTZ__PRFO, "ControlDef", zREPOS_NEXT );
   }

   // This stuff has already been set and checked above.
   nRC = zCURSOR_SET;

   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vTZ__PRFO, "ControlDef", zPOS_AFTER );
      SetAttributeFromAttribute( vTZ__PRFO, "ControlDef", "Tag", vTZPNTBAR, "ToolBarIcons", "Tag" );
      nRC = SetCursorNextEntityByInteger( vTZPNTBAR, "ToolBarIcons", "OnPalette", 1, "" );
   }

   oTZ__PRFO_CommitProfile( vTZ__PRFO );

   GetViewByName( &vTZPNMWIN, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   fnPainterCall( zMSG_REPLACETOOLBARBUTTONS, vTZPNMWIN, 0, 0, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_InitVOR_Transfer( zVIEW vSubtask )
{
   zVIEW    vTZZOVEAO;

   // Drop the previous instance if it exists.
   if ( GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZZOVEAO );

   TZXSLTDD_InitModalSubwindow( vSubtask );
   return( 0 );
}

// All actions which go to a modal sub-window from the painter main window
// should use this operation to cause the window to be updated prior to
// going to the sub-window.
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_InitModalSubwindow( zVIEW vSubtask )
{
   zVIEW  vWindow;

   GetViewByName( &vWindow, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( vWindow )
   {
      zVIEW vEvents;
      zVIEW vTZPESRCO;

      // Update the zeidon window before anything else
      fnPainterCall( zMSG_UPDATEZEIDONWINDOW, vSubtask, 0, 0, 0 );

      // Create an Events Object for window events
      if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
      {
         DropObjectInstance( vEvents );
      }

      ActivateEmptyObjectInstance( &vEvents, "TZPNEVWO", vSubtask, zMULTIPLE );
      SetNameForView( vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntity( vTZPESRCO, "WindowStyle", 0 );

      if ( SetCursorFirstEntity( vTZPESRCO, "EventDef", 0 ) == zCURSOR_SET )
      {
         do
         {
            CreateEntity( vEvents, "Event", zPOS_AFTER );
            SetMatchingAttributesByName( vEvents, "Event", vTZPESRCO, "EventDef", zSET_ALL );
            CreateEntity( vEvents, "EventAct", zPOS_AFTER );

         }  while ( SetCursorNextEntity( vTZPESRCO, "EventDef", 0 ) == zCURSOR_SET );
      }

      // Now set the actions defined for the window
      if ( SetCursorFirstEntity( vWindow, "WndEvent", 0 ) > zCURSOR_UNCHANGED )
      {
         zLONG lType;
         do
         {
            GetIntegerFromAttribute( &lType, vWindow, "WndEvent", "Type" );
            if ( SetCursorFirstEntityByInteger( vEvents, "Event", "Type", lType, 0 ) > zCURSOR_UNCHANGED )
            {
               SetMatchingAttributesByName( vEvents, "EventAct", vWindow, "WndAct", zSET_ALL );
            }
         } while ( SetCursorNextEntity( vWindow, "WndEvent", 0 ) > zCURSOR_UNCHANGED );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZXSLTDD_HelpAbout
//
// PURPOSE:    This Entry does the About
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_DIALOG, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZXSLTDD_HelpBasics
//
// PURPOSE:    This Entry transfers to basic help
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZXSLTDD_HelpOnHelp
//
// PURPOSE:    This Entry transfers to HelpOnHelp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  TZXSLTDD_HelpContents
//
// PURPOSE:    This Entry transfers to help Contents
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}
// END OF PAINTER DIALOG OPERATIONS

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_PreBuildSelControls( zVIEW vSubtask )
{
   zVIEW  vTZPESRCO;
   zVIEW  vTZ__PRFO;

   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
   oTZ__PRFO_GetViewToProfile( &vTZ__PRFO, "WD", vSubtask, zCURRENT_OI );
   if ( SetCursorFirstEntity( (zVIEW) vTZ__PRFO, "ControlDef", 0 ) < zCURSOR_SET )
   {
      SetAllSelStatesForEntityForSet( vTZPESRCO, "ControlDef", 1, 1, 0 );
   }
   else
   {
      SetAllSelStatesForEntityForSet( vTZPESRCO, "ControlDef", 0, 1, 0 );
      do
      {
         if ( SetEntityCursor( vTZPESRCO, "ControlDef", "Tag", zPOS_FIRST | zQUAL_ENTITYATTR,
                               (zVIEW) vTZ__PRFO, "ControlDef", "Tag", 0, 0, 0 ) >= zCURSOR_SET )
         {
            SetSelectStateOfEntityForSet( vTZPESRCO, "ControlDef", 1, 1 );
         }

      } while ( SetCursorNextEntity( (zVIEW) vTZ__PRFO, "ControlDef", 0 ) >= zCURSOR_SET );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_UpdateToolbarControls( zVIEW vSubtask )
{
   zVIEW  vTZPESRCO;
   zVIEW  vTZ__PRFO;
   zVIEW  vTZPNMWIN;

   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
   oTZ__PRFO_GetViewToProfile( &vTZ__PRFO, "WD", vSubtask, zCURRENT_OI );
   if ( SetCursorFirstEntity( vTZ__PRFO, "ControlDef", 0 ) >= zCURSOR_SET )
   {
      while ( DeleteEntity( vTZ__PRFO, "ControlDef", zREPOS_NEXT ) == zCURSOR_SET );
   }

   if ( SetCursorFirstSelectedEntity( vTZPESRCO, "ControlDef", 0 ) >= zCURSOR_SET )
   {
      do
      {
         CreateEntity( vTZ__PRFO, "ControlDef", zPOS_AFTER );
         SetAttributeFromAttribute( vTZ__PRFO, "ControlDef", "Tag",
                                    vTZPESRCO, "ControlDef", "Tag" );
      } while ( SetCursorNextSelectedEntity( vTZPESRCO, "ControlDef", 0 ) >= zCURSOR_SET );
   }

   oTZ__PRFO_CommitProfile( vTZ__PRFO );
   SetAllSelStatesForEntityForSet( vTZPESRCO, "ControlDef", 0, 1, 0 );
   GetViewByName( &vTZPNMWIN, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   fnPainterCall( zMSG_REPLACETOOLBARBUTTONS, vTZPNMWIN, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_CancelToolbarControls( zVIEW vSubtask )
{
   zVIEW  vTZPESRCO;

   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
   SetAllSelStatesForEntityForSet( vTZPESRCO, "ControlDef", 0, 1, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
WND_UPD_Popup( zVIEW vSubtask )
{
   TZXSLTDD_LoadEventPopups( vSubtask, "UpdateAction", "NewAction", "SelectAction",
                             "RemoveAction", "WND_UPD_OperationEdit", "WND_UPD_Popup" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OPT_UPD_Popup( zVIEW vSubtask )
{
   TZXSLTDD_LoadEventPopups( vSubtask, "UpdateAction", "NewAction", "SelectAction",
                             "RemoveAction", "", "OPT_UPD_Popup" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ControlActionDetail
//
// PURPOSE:  This function sets the TZXSLT cursor to the correct
//           Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ControlActionDetail( zVIEW vSubtask )
{
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;

   GetViewByName( &vXSLTC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vXSLTC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vXSLTW, "Action", "Tag", vXSLTC, "EventAct", "Tag", "" );
      CreateTemporalSubobjectVersion( vXSLTW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00218", "XSLT Maintenance",
                   "No Action exists for Button.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CtrlContextMappingInit
//
// PURPOSE:  This function builds the MapDomain View for the Context
//           Combo Box, if there is current mapping.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CtrlContextMappingInit( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   if ( GetViewByName( &vXSLT, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vXSLT );

   GetViewByName( &vXSLT, "TZCONTROL", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );

   if ( CheckExistenceOfEntity( vXSLT, "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vXSLT, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   }
   else
      ActivateEmptyObjectInstance( &vDomain, "TZDGSRCO", vSubtask, 0 );

   SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListBuildContextList( zVIEW vSubtask )
{
   CtrlContextMappingInit( vSubtask );
   RefreshCtrl( vSubtask, "MapType" );
   RefreshCtrl( vSubtask, "Context" );
// RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListContextSelect( zVIEW vSubtask )
{
   MapCtrl( vSubtask, "MapType" );
   MapCtrl( vSubtask, "Context" );
   RefreshCtrl( vSubtask, "CtrlList" );
   RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListDisplayActionDetail( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vXSLT;
   zVIEW vAction;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAction, "TZACTION", vSubtask, zLEVEL_TASK );
   if ( vAction == 0 )
   {
      // If an Action for the Event exists, position on the linked Action
      // instance and set TZACTION to TZXSLT.  Otherwise, set TZACTION
      // to an empty instance for auto graying.
      if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
      {
         CreateViewFromViewForTask( &vAction, vXSLT, 0 );
         SetCursorFirstEntityByAttr( vAction, "Action", "Tag", vControl, "EventAct", "Tag", "" );

         SetNameForView( vAction, "TZACTION", vSubtask, zLEVEL_TASK );
      }
      else
      {
         fnCreateEmptyActionView( vSubtask, vXSLT );
      }
   }

#if 0
   RefreshCtrl( vSubtask, "Operation" );
   RefreshCtrl( vSubtask, "Termination" );
   RefreshCtrl( vSubtask, "XSLT" );
   RefreshCtrl( vSubtask, "Window" );
   RefreshCtrl( vSubtask, "Function" );
#endif

   return( 0 );
}

#define zEDIT_CONTROL                       1010L
#define zMLE_CONTROL                        1015L
#define zPUSHBUTTON_CONTROL                 1020L
#define zCOMBOBOX_CONTROL                   1050L
#define zCALENDAR_CONTROL                   1060L
#define zLISTBOX_CONTROL                    1080L
#define zGROUPBOX_CONTROL                   1090L
#define zTAB_CONTROL                        2090L
#define zTABCTL_CONTROL                     2091L

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Recursive function for moving ctrls covered by a group to be sub-ctrls
// of the group.
//
void
fnRecurseGroupCtrls( zVIEW vXSLT )
{
   zVIEW  vCtrlXSLT;
   zTZWDLGSO_Control_DEF ControlDef;
   zTZWDLGSO_Control_DEF GroupBoxDef;
   zLONG  lKey;
   zSHORT nRC;

   ControlDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
   ControlDef.CheckByte = 0xFE;
   GroupBoxDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
   GroupBoxDef.CheckByte = 0xFE;
   nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lKey, vXSLT, "ControlDef", "Key" );
      if ( lKey == zGROUPBOX_CONTROL )
      {
         CreateViewFromViewForTask( &vCtrlXSLT, vXSLT, 0 );

         // Get the structure of attributes for this group box.
         GetStructFromEntityAttrs( (zPCHAR) &GroupBoxDef, vXSLT, "Control" );
         if ( GroupBoxDef.CheckByte != 0xFE )
            MessageBox( 0, "GroupBoxDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

         nRC = SetCursorFirstEntity( vCtrlXSLT, "Control", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            // get the structure of attributes for this control
            GetStructFromEntityAttrs( (zPCHAR) &ControlDef, vCtrlXSLT, "Control" );
            if ( ControlDef.CheckByte != 0xFE )
               MessageBox( 0, "ControlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

            if ( GroupBoxDef.PSDLG_X < ControlDef.PSDLG_X && GroupBoxDef.PSDLG_Y < ControlDef.PSDLG_Y &&
                 GroupBoxDef.PSDLG_X + GroupBoxDef.SZDLG_X > ControlDef.PSDLG_X + ControlDef.SZDLG_X &&
                 GroupBoxDef.PSDLG_Y + GroupBoxDef.SZDLG_Y > ControlDef.PSDLG_Y + ControlDef.SZDLG_Y )
            {
               ControlDef.PSDLG_X -= GroupBoxDef.PSDLG_X;
               ControlDef.PSDLG_Y -= GroupBoxDef.PSDLG_Y;
               SetAttributeFromInteger( vCtrlXSLT, "Control", "PSDLG_X", ControlDef.PSDLG_X );
               SetAttributeFromInteger( vCtrlXSLT, "Control", "PSDLG_Y", ControlDef.PSDLG_Y );
               nRC = MoveSubobject( vXSLT, "CtrlCtrl", vCtrlXSLT, "Control", zPOS_LAST, zREPOS_NEXT );
            }
            else
               nRC = SetCursorNextEntity( vCtrlXSLT, "Control", 0 );
         }

         DropView( vCtrlXSLT );
      }

      SetViewToSubobject( vXSLT, "CtrlCtrl" );
      fnRecurseGroupCtrls( vXSLT );
      ResetViewFromSubobject( vXSLT );

      nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListEnableButtonCheckbox
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListEnableButtonCheckbox( zVIEW  vSubtask,
                              zSHORT nEnable )
{
   SetCtrlState( vSubtask, "ChangeType", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AddMapEntity", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DeleteMapEntity", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "MapType", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Context", zCONTROL_STATUS_ENABLED, nEnable );

   SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_ENABLED, nEnable );

   SetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
} // CtrlListEnableButtonCheckbox


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListRefreshCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListRefreshCtrls( zVIEW  vSubtask )
{
   RefreshCtrl( vSubtask, "Type" );
   RefreshCtrl( vSubtask, "Tag" );
   RefreshCtrl( vSubtask, "Text" );
   RefreshCtrl( vSubtask, "Top" );
   RefreshCtrl( vSubtask, "Left" );
   RefreshCtrl( vSubtask, "Height" );
   RefreshCtrl( vSubtask, "Width" );
   RefreshCtrl( vSubtask, "Mapping" );
   RefreshCtrl( vSubtask, "MapType" );
   RefreshCtrl( vSubtask, "Context" );
   RefreshCtrl( vSubtask, "EventAction" );
   RefreshCtrl( vSubtask, "AutoReposX" );
   RefreshCtrl( vSubtask, "AutoResizeX" );
   RefreshCtrl( vSubtask, "AutoReposY" );
   RefreshCtrl( vSubtask, "AutoResizeY" );
   RefreshCtrl( vSubtask, "DD_Src" );
   RefreshCtrl( vSubtask, "DD_Tgt" );
   RefreshCtrl( vSubtask, "DIL_Inherited" );
   RefreshCtrl( vSubtask, "DIL_Text" );
   RefreshCtrl( vSubtask, "CSS_Id" );
   RefreshCtrl( vSubtask, "CSS_Class" );
   RefreshCtrl( vSubtask, "CSS_Class1" );
   RefreshCtrl( vSubtask, "CSS_Font" );
   RefreshCtrl( vSubtask, "CSS_Position" );
   RefreshCtrl( vSubtask, "CSS_Size" );
   RefreshCtrl( vSubtask, "TabOrder" );

   return( 0 );
} // CtrlListRefreshCtrls

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListEnableCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListEnableCtrls( zVIEW  vSubtask,
                     zSHORT nEnable )
{
   CtrlListEnableButtonCheckbox( vSubtask, nEnable );

   SetCtrlState( vSubtask, "Type", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Tag", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Text", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Height", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Width", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Detail", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Common", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Mapping", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "EventAction", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DD_Src", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DD_Tgt", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DIL_Inherited", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DIL_Text", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Id", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Class", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Class1", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Font", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Position", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "CSS_Size", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "TabOrder", zCONTROL_STATUS_ENABLED, nEnable );

   CtrlListRefreshCtrls( vSubtask );

   return( 0 );
} // CtrlListEnableCtrls

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListSetCheckedState
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListSetCheckedState( zVIEW  vSubtask,
                         zVIEW  vControl,
                         zLONG  lSubtype )
{
   if ( lSubtype & zCONTROL_SIZEABLEBORDER )
      SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_INVISIBLE )
      SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_DISABLED )
      SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_RELATIVE_POS_X )
      SetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_RELATIVE_POS_Y )
      SetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_RELATIVE_SIZE_X )
      SetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_RELATIVE_SIZE_Y )
      SetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_NOTABSTOP )
      SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED, 0 );

   if ( (lSubtype & zCONTROL_BORDEROFF ) == 0 )
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_RIGHTJUSTIFY )
      SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_CENTERJUSTIFY )
      SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROL_FULL_CLIENT )
      SetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_CHECKED, 0 );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "ExtendedStyle" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( lSubtype & zCONTROLX_DISABLE_READONLY )
         SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, 1 );
      else
         SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, 0 );
   }
   else
      SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, 0 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_SIZE )
      SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED, 0 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_POS )
      SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED, 0 );

   return( 0 );
} // CtrlListSetCheckedState

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListShowAllCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CtrlListShowAllCtrls( zVIEW vSubtask,
                      zVIEW vControl )
{
   zVIEW  vPE;
   zVIEW  vTZXSLTL;
   zLONG  lParentCnt;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 &&
        SetEntityCursor( vPE, "ControlDef", "Key",
                         zPOS_FIRST | zQUAL_ENTITYATTR,
                         vControl, "ControlDef", "Key",
                         0, 0, 0 ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vPE, "ValidParent", 0 ) == zCURSOR_SET )
         lParentCnt = 1;
      else
         lParentCnt = 0;
   }
   else
   {
      lParentCnt = 1;
   }

   if ( !ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META ) )
   {
      CtrlListEnableButtonCheckbox( vSubtask, FALSE );
      SetCtrlState( vSubtask, "Detail", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Common", zCONTROL_STATUS_ENABLED, TRUE );
      CtrlListRefreshCtrls( vSubtask );
   }
   else
   {
      CtrlListEnableCtrls( vSubtask, TRUE );
      if ( lParentCnt )
         SetCtrlState( vSubtask, "ChangeType", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // CtrlListShowAllCtrls

zSHORT
fnShowCtrlList( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vControl;
   zCHAR  szEntityName[ 33 ];
   zLONG lSubtype;

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vControl );

   if ( GetViewByName( &vControl, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vControl );

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) >= 0 )
      SetNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vXSLT, szEntityName ) >= 0 &&
        zstrcmp( szEntityName, "Control" ) == 0 )
   {
      TraceLineS( "ShowCtrlList EntityName is Control: ", szEntityName );
      CreateViewFromViewForTask( &vControl, vXSLT, 0 );
      SetViewFromView( vControl, vXSLT );
      SetNameForView( vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
      SetNameForView( vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );

      // Build the TZPNEVWO object for the control events.
      fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );

      // if XSLT check out, enable all Controls
      CtrlListShowAllCtrls( vSubtask, vControl );

      GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
      CtrlListSetCheckedState( vSubtask, vControl, lSubtype );

      // Set up the Context
      CtrlContextMappingInit( vSubtask );
      CtrlListDisplayActionDetail( vSubtask );
   }
   else
   {
      TraceLineS( "ShowCtrlList EntityName: ", szEntityName );
      if ( GetViewByName( &vControl, "TZPNEVWO", vSubtask, zLEVEL_TASK ) >= 0 )
         DropView( vControl );

      RefreshCtrl( vSubtask, "Mapping" );
      RefreshCtrl( vSubtask, "EventAction" );
      RefreshCtrl( vSubtask, "MapType" );
      RefreshCtrl( vSubtask, "Context" );

      // Disable all Controls
      CtrlListEnableCtrls( vSubtask, FALSE );

      if ( GetViewByName( &vControl, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
         DropView( vControl );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowCtrlList( zVIEW vSubtask )
{
   ED_LimitTextLength( vSubtask, "DIL_Text", 126 );
   ED_LimitTextLength( vSubtask, "FindText", 255 );
   SetCtrlState( vSubtask, "FindByTag", zCONTROL_STATUS_CHECKED, 1 );
   SetCtrlState( vSubtask, "FindByText", zCONTROL_STATUS_CHECKED, 1 );
   SetCtrlText( vSubtask, "InvisibleForFind", "" );

   return( fnShowCtrlList( vSubtask ) );
}

zSHORT
fnResetTagsR( zVIEW vXSLT )
{
   zCHAR  szTag[ 33 ];
   zSHORT nLth;
   zSHORT nRC;

   // Loop through all Controls at this level.
   nRC = SetCursorFirstEntity( vXSLT, "Control", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "Control", "Tag" );
      nLth = (zSHORT) zstrlen( szTag );
      if ( nLth > 0 && szTag[ nLth - 1 ] >= '0' && szTag[ nLth - 1 ] <= '9' )
         nLth--;

      if ( nLth > 0 )
      {
         szTag[ nLth ] = 0;
         SetAttributeFromString( vXSLT, "Control", "Tag", szTag );
      }

      // Process any sub-controls.
      if ( CheckExistenceOfEntity( vXSLT, "CtrlCtrl" ) == 0 )
      {
         SetViewToSubobject( vXSLT, "CtrlCtrl" );
         fnResetTagsR( vXSLT );
         ResetViewFromSubobject( vXSLT );
      }

      nRC = SetCursorNextEntity( vXSLT, "Control", "" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListResetTags( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }

      fnResetTagsR( vXSLT );
   }

   RefreshCtrl( vSubtask, "CtrlList" );
   RefreshCtrl( vSubtask, "Tag" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListSaveStyleFlags
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListSaveStyleFlags( zVIEW vSubtask,
                        zVIEW vControl )
{
   zLONG lSubtype;

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );

    // Is Tab Presentation active?
   if ( TAB_GetActiveTab( vSubtask, "CtrlListTab" ) == 3 )
   {
      if ( GetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_SIZEABLEBORDER;
      else
         lSubtype &= ~zCONTROL_SIZEABLEBORDER;

      if ( GetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_INVISIBLE;
      else
         lSubtype &= ~zCONTROL_INVISIBLE;

      if ( GetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_DISABLED;
      else
         lSubtype &= ~zCONTROL_DISABLED;

      if ( GetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_NOTABSTOP;
      else
         lSubtype &= ~zCONTROL_NOTABSTOP;

      if ( GetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED ) == 0 )
         lSubtype |= zCONTROL_BORDEROFF;
      else
         lSubtype &= ~zCONTROL_BORDEROFF;

      if ( GetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RIGHTJUSTIFY;
      else
         lSubtype &= ~zCONTROL_RIGHTJUSTIFY;

      if ( GetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_CENTERJUSTIFY;
      else
         lSubtype &= ~zCONTROL_CENTERJUSTIFY;

      if ( GetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_FULL_CLIENT;
      else
         lSubtype &= ~zCONTROL_FULL_CLIENT;
   }

   // Is Tab Misc active?
   if ( TAB_GetActiveTab( vSubtask, "CtrlListTab" ) == 4 )
   {
      if ( GetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_POS_X;
      else
         lSubtype &= ~zCONTROL_RELATIVE_POS_X;

      if ( GetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_POS_Y;
      else
         lSubtype &= ~zCONTROL_RELATIVE_POS_Y;

      if ( GetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_SIZE_X;
      else
         lSubtype &= ~zCONTROL_RELATIVE_SIZE_X;

      if ( GetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_SIZE_Y;
      else
         lSubtype &= ~zCONTROL_RELATIVE_SIZE_Y;
   }

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   return( 0 );
} // CtrlListSaveStyleFlags


zOPER_EXPORT zSHORT OPERATION
SaveCtrlList( zVIEW vSubtask )
{
   zVIEW vControl;
   zLONG lSubtype = 0;

   if ( GetViewByName( &vControl, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
      TraceLineS( "SaveCtrlList ", "Found TZPNEVWO" );
   else
      TraceLineS( "SaveCtrlList ", "NOT Found TZPNEVWO" );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( vControl )
   {
//    MessageBox( 0, "SaveCtrlList", "Control", MB_OK | MB_SYSTEMMODAL );
      if ( MapCtrl( vSubtask, "Tag" ) == 0 &&
           MapCtrl( vSubtask, "Text" ) == 0 &&
           MapCtrl( vSubtask, "Top" ) == 0 &&
           MapCtrl( vSubtask, "Left" ) == 0 &&
           MapCtrl( vSubtask, "Height" ) == 0 &&
           MapCtrl( vSubtask, "Width" ) == 0 &&
           MapCtrl( vSubtask, "AutoReposX" ) == 0 &&
           MapCtrl( vSubtask, "AutoResizeX" ) == 0 &&
           MapCtrl( vSubtask, "AutoReposY" ) == 0 &&
           MapCtrl( vSubtask, "AutoResizeY" ) == 0 &&
           MapCtrl( vSubtask, "DD_Src" ) == 0 &&
           MapCtrl( vSubtask, "DD_Tgt" ) == 0 &&
           MapCtrl( vSubtask, "DIL_Inherited" ) == 0 &&
           MapCtrl( vSubtask, "DIL_Text" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Id" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Class" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Class1" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Font" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Position" ) == 0 &&
           MapCtrl( vSubtask, "CSS_Size" ) == 0 &&
           MapCtrl( vSubtask, "TabOrder" ) == 0 )
      {
         zCHAR szTag[ 33 ];

         if ( GetCtrlText( vSubtask, "Tag", szTag, zsizeof( szTag ) ) == 0 )
            OL_SetTextForCurrentItem( vSubtask, "CtrlList", szTag );

         CtrlListSaveStyleFlags( vSubtask, vControl );

         if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
         {
            GetIntegerFromAttribute( &lSubtype, vControl, "Control", "ExtendedStyle" );
            // Is Tab Presentation active?
            if ( TAB_GetActiveTab( vSubtask, "CtrlListTab" ) == 3 )
            {
               if ( GetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED ) )
                  lSubtype |= zCONTROLX_DISABLE_READONLY;
               else
                  lSubtype &= ~zCONTROLX_DISABLE_READONLY;
            }
         }

         // Is Tab Presentation active?
         if ( TAB_GetActiveTab( vSubtask, "CtrlListTab" ) == 3 )
         {
            if ( GetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED ) )
               lSubtype |= zCONTROLX_ABSOLUTE_PIXEL_SIZE;
            else
               lSubtype &= ~zCONTROLX_ABSOLUTE_PIXEL_SIZE;

            if ( GetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED ) )
               lSubtype |= zCONTROLX_ABSOLUTE_PIXEL_POS;
            else
               lSubtype &= ~zCONTROLX_ABSOLUTE_PIXEL_POS;
         }

         if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
            SetAttributeFromInteger( vControl, "Control", "ExtendedStyle", lSubtype );
      }
      else
      {
         TraceLineS( "SaveCtrlList Mapping Error", "" );
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( -1 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListMapping( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zCHAR  szEntityName[ 33 ];

// TraceLineS( "CtrlListMapping", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vXSLT, szEntityName ) >= 0 &&
        zstrcmp( szEntityName, "Control" ) == 0 )
   {
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }

      GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
      if ( vXSLT )
         SetNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListRefreshMapping( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListCancel( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      DropNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }
   }

   if ( GetViewByName( &vXSLT, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vXSLT );

   return( 0 );
} // CtrlListCancel

zOPER_EXPORT zSHORT OPERATION
ReturnFromCtrlList( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   if ( SaveCtrlList( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
   // TraceLineS( "ReturnFromCtrlList ResetViewFromSubobject", "" );
      DropNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }
   }

   if ( GetViewByName( &vXSLT, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vXSLT );

   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ReturnToCtrlList( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }
   }

   ShowCtrlList( vSubtask );
   return( 1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      PreviousWindowCtrlList
//
//  PURPOSE:    To Update the previous Window Ctrl List
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PreviousWindowCtrlList( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLTL;
   zVIEW  vMapDomain;

   if ( SaveCtrlList( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
   // TraceLineS( "ReturnFromCtrlList ResetViewFromSubobject", "" );
      DropNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }

      if ( SetCursorPrevEntity( vXSLT, "Window", "" ) != 0 )
         SetCursorLastEntity( vXSLT, "Window", "" );

      if ( GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
         SetViewFromView( vXSLTL, vXSLT );
   }

   if ( GetViewByName( &vMapDomain, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vMapDomain );

// TZXSLTDD_InitModalSubwindow( vSubtask );
   OL_ResetNodeList( vSubtask, "CtrlList" );
   ShowCtrlList( vSubtask );
   RefreshCtrl( vSubtask, "Name" );
   RefreshCtrl( vSubtask, "Caption" );
   RefreshCtrl( vSubtask, "CtrlList" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      NextWindowCtrlList
//
//  PURPOSE:    To Update the next Window Ctrl List
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
NextWindowCtrlList( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vXSLTL;
   zVIEW  vMapDomain;

   if ( SaveCtrlList( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
   // TraceLineS( "ReturnFromCtrlList ResetViewFromSubobject", "" );
      DropNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
      }

      if ( SetCursorNextEntity( vXSLT, "Window", "" ) != 0 )
         SetCursorFirstEntity( vXSLT, "Window", "" );

      if ( GetViewByName( &vXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) > 0 )
         SetViewFromView( vXSLTL, vXSLT );
   }

   if ( GetViewByName( &vMapDomain, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vMapDomain );

// TZXSLTDD_InitModalSubwindow( vSubtask );
   OL_ResetNodeList( vSubtask, "CtrlList" );
   ShowCtrlList( vSubtask );
   RefreshCtrl( vSubtask, "Name" );
   RefreshCtrl( vSubtask, "Caption" );
   RefreshCtrl( vSubtask, "CtrlList" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "CtrlListPopup", -1, 0, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      CtrlListRemoveAction
//
// PURPOSE:    To Remove an action from a ctrl.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CtrlListRemoveAction( zVIEW vSubtask )
{
   zVIEW    vXSLT;
   zVIEW    vEvents;
   zLONG    lType;

   GetViewByName( &vXSLT, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vXSLT, "Event", "Type",
                                       lType, 0 ) == zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vXSLT, "EventAct" ) == zCURSOR_SET )
         ExcludeEntity( vXSLT, "EventAct", zREPOS_PREV );

      DeleteEntity( vXSLT, "Event", zREPOS_PREV );
   }

   fnSetupEventList( vSubtask, vXSLT );
   RefreshCtrl( vSubtask, "EventAction" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CtrlListSelectAction
//
// PURPOSE:  This function creates the Event entity for the ctrl.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CtrlListSelectAction( zVIEW vSubtask )
{
   zVIEW    vXSLTC;
   zVIEW    vXSLTW;
   zVIEW    vEvents;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vXSLTC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXSLTW ) == 0 )
   {
   }

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, "Event", "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "XSLT Maintenance",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vXSLTC, "Event", "Type",
                                       lType, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "XSLT Maintenance",
                   "Event already has an Action!\n"
                   "First remove the Action and then select a new one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vXSLTC, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vXSLTC, "Event", "Type", lType );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CtrlListUpdateAction
//
// PURPOSE:  This function sets the TZXSLT cursor to the correct
//           Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CtrlListUpdateAction( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vXSLTW;
   zCHAR    szTag[ 34 ];

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLTW, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXSLTW ) == 0 )
   {
   }

   // If EventAct exists, set cursor for Update Action window.
   // Otherwise, return an error.
   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vWork, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vXSLTW, "Action", "Tag", szTag, "" );
      CreateTemporalSubobjectVersion( vXSLTW, "Action" );
   }
   else
   // Position on Action that corresponds to EventAct entity instance.
   {
      MessageSend( vSubtask, "PN00107", "XSLT Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CtrlListSelectOrUpdate
//
// PURPOSE:  This function calls Select or Update Action processing.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CtrlListSelectOrUpdate( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vTZXSLTL;

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZPNCTAD", "UPD_ACT" );
      return( CtrlListUpdateAction( vSubtask ) );
   }
   else
   {
      GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

      if ( ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META ) )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                  "TZPNCTAD", "SEL_ACT" );
         return( CtrlListSelectAction( vSubtask ) );
      }
   }

   return( 0 );
}

//./ ADD NAME=PopupMaintenance
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     PopupMaintenance
//
//  PURPOSE:   To popup the correct menu based on the current selection
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PopupMaintenance( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szEntityName[ 33 ];

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vControl, szEntityName ) >= 0 &&
        zstrcmp( szEntityName, "Control" ) == 0 )
   {
      zVIEW  vPE;

      if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 &&
           SetEntityCursor( vPE, "ControlDef", "Key",
                            zPOS_FIRST | zQUAL_ENTITYATTR,
                            vControl, "ControlDef", "Key",
                            0, 0, 0 ) >= zCURSOR_SET )
      {
         zLONG lParentCnt = CountEntitiesForView( vPE, "ValidParent" );
         zBOOL bDelete  = FALSE;
         zVIEW vXSLT;

         GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
         if ( ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            bDelete = TRUE;

         if ( lParentCnt )
         {
            EnableAction( vSubtask, "DeleteCtrl", bDelete );
            CreateTrackingPopupMenu( vSubtask, "CtrlDetail",
                                     -1, 0, FALSE, FALSE );
         }
         else
         {
            EnableAction( vSubtask, "DeleteCtrl", bDelete );
            EnableAction( vSubtask, "ChangeCtrlType", bDelete );
            CreateTrackingPopupMenu( vSubtask, "CtrlMaintenance",
                                     -1, 0, FALSE, FALSE );
         }
         return( 0 );
      }
   }

   CreateTrackingPopupMenu( vSubtask, "Unknown", -1, 0, FALSE, FALSE );
   return( 0 );
}

//./ ADD NAME=ChangeCtrlType
// Source Module=tzxsltdd.c
//////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     ChangeCtrlType
//
//  PURPOSE:   To switch the type of a control in the painter
//
//  DESCRIPTION: This method attempts to change the selected controls
//               to the control type positioned on in the control object.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ChangeCtrlType( zVIEW vSubtask )
{
#if 0
   zVIEW  vXSLT;
   zVIEW  vControl;
   zVIEW  vPE;
   zCHAR  szEntityName[ 33 ];
   zSHORT nRC;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "ChangeCtrlType unable to get PE view", "" );
      return( -1 );
   }

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
      SetNameForView( vXSLT, "NoRefresh", vSubtask, zLEVEL_TASK );

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vXSLT, szEntityName ) >= 0 )
   {
      TraceLineS( "ChangeCtrlType: ", szEntityName );
      if ( zstrcmp( szEntityName, "Control" ) == 0 )
      {

#if 0
   zVIEW  vCtrlDef;
   zVIEW  vParentDef;
   zLONG  lNewType;
   zLONG  lParentType;
   zLONG  lType, lNewID;
   zSHORT nPos;
   zSHORT nRC;
   TZPainterCtrl *pCtrl;

      // Get the new type of control
      GetIntegerFromAttribute( &lNewType, vPE, "ControlDef", "Key" );
      SetNameForView( m_vXSLT, szlTZCONTROL, vSubtask, zLEVEL_TASK );
      for ( nPos = 0; nPos < m_nCtrlCnt; nPos++ )
      {
         pCtrl = m_CtrlList[ nPos ];
         lParentType = 0;
         if ( pCtrl->m_pParentCtrl )
         {
            lParentType = pCtrl->m_pParentCtrl->m_lType;
         }

         lType = pCtrl->m_lType;
         if ( pCtrl->IsSelected( ) && lNewType != lType )
         {
            vParentDef = 0;
            if ( lParentType )
            {
               CreateViewFromViewForTask( &vParentDef, vPE, 0 );
               SetCursorFirstEntityByInteger( vParentDef,
                                              "ControlDef", "Key",
                                              lParentType, 0 );
            }

            pCtrl->PositionOnZeidonCtrl( );
            CreateViewFromViewForTask( &vCtrlDef, vPE, 0 );
            SetCursorFirstEntityByInteger( vCtrlDef, "ControlDef",
                                           "Key", pCtrl->m_lType, 0 );

            // PositionOnZeidonControl above may have moved the cursor
            // in TZPESRCO to create the previous control, now ensure
            // that it is moved back to the new control
            SetCursorFirstEntityByInteger( vPE, "ControlDef",
                                           "Key", lNewType, 0 );
            nRC = CtrlValidate( vCtrlDef, vParentDef, vPE,
                                zCTRL_VALID_CHANGE );
            if ( nRC < 0 )
               lNewID = 0;
            else
            {
               // Get the ID of the control to be painted
               GetIntegerFromAttribute( &lNewID, vCtrlDef,
                                        "ControlDef", "Key" );
            }

            // Drop the view to the parent control if one was created
            if ( vParentDef )
               DropView( vParentDef );

            DropView( vCtrlDef );

            if ( nRC == 0 &&           // Old parent is still valid
                 lNewID == lNewType )  // Ctrl is being changed to type
                                       // requested.
            {
               if ( SetCursorFirstEntity( m_vXSLT, "ControlDef", 0 )
                                                          == zCURSOR_SET )
               {
                  ExcludeEntity( m_vXSLT, "ControlDef", zREPOS_NONE );
               }

               SetCursorFirstEntityByInteger( vPE, "ControlDef",
                                              "Key", lNewType, 0 );
               IncludeSubobjectFromSubobject( m_vXSLT, "ControlDef",
                                              vPE, "ControlDef",
                                              zPOS_BEFORE );
               SendCtrlMsg( zCTRLMSG_CHANGE_TYPE, 0,
                            lNewType, lType, 0, 0, pCtrl );
               RepaintZeidonCtrl( );
            }
         }

      }  while ( ResetViewFromSubobject( m_vXSLT ) == 0 );
#endif
      }
   }
   else
      TraceLineS( "ChangeCtrlType: ", "Cannot locate Entity name" );

#endif

   TraceLineS( "ChangeCtrlType!!!!!!", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ChgCtrlPostbuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vPE;

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, 0 );
   LB_SetDisplaySelectSet( vSubtask, "ChangeTypeList", 3 );
   LB_SetSelectSelectSet( vSubtask, "ChangeTypeList", 4 );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( vPE && SetEntityCursor( vPE, "ControlDef", "Key",
                                zPOS_FIRST | zQUAL_ENTITYATTR,
                                vControl, "ControlDef", "Key",
                                0, 0, 0 ) >= zCURSOR_SET )
   {
      zCHAR  szTag[ 33 ],               szCurrTag[ 33 ];
      zCHAR  szAcceptsAllParents[ 2 ],  szCurrAcceptsAllParents[ 2 ];
      zCHAR  szAcceptsAllChildren[ 2 ], szCurrAcceptsAllChildren[ 2 ];
      zCHAR  szRequiresParent[ 2 ],     szCurrRequiresParent[ 2 ];
      zLONG  lKey,                      lCurrKey;
      zLONG  lInvisible,                lCurrInvisible;
      zLONG  lValidParentCnt,           lCurrValidParentCnt;
      zLONG  lValidChildCnt,            lCurrValidChildCnt;
      zSHORT nPrevSelSet;
      zSHORT nRC;

      GetStringFromAttribute( szTag, zsizeof( szTag ), vPE, "ControlDef", "Tag" );
      GetStringFromAttribute( szAcceptsAllParents, zsizeof( szAcceptsAllParents ), vPE,
                              "ControlDef", "AcceptsAllParents" );
      GetStringFromAttribute( szAcceptsAllChildren, zsizeof( szAcceptsAllChildren ), vPE,
                              "ControlDef", "AcceptsAllChildren" );
      GetStringFromAttribute( szRequiresParent, zsizeof( szRequiresParent ), vPE,
                              "ControlDef", "RequiresParent" );
      GetIntegerFromAttribute( &lKey, vPE,
                               "ControlDef", "Key" );
      GetIntegerFromAttribute( &lInvisible, vPE,
                               "ControlDef", "Invisible" );
      lValidParentCnt = CountEntitiesForView( vPE, "ValidParent" );
      lValidChildCnt = CountEntitiesForView( vPE, "ValidChild" );

      nPrevSelSet = SetSelectSetForView( vPE, 4 );
      SetAllSelectStatesForEntity( vPE, "ControlDef", FALSE, 0 );
      SetSelectSetForView( vPE, 3 );
      SetAllSelectStatesForEntity( vPE, "ControlDef", FALSE, 0 );
      SetSelectSetForView( vPE, 2 );
      SetAllSelectStatesForEntity( vPE, "ControlDef", FALSE, 0 );
      SetSelectSetForView( vPE, 1 );
      SetAllSelectStatesForEntity( vPE, "ControlDef", FALSE, 0 );

      SetSelectSetForView( vPE, 3 );
      nRC = SetCursorFirstEntity( vPE, "ControlDef", 0 );
      while ( nRC == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lCurrKey, vPE,
                                  "ControlDef", "Key" );
         if ( lCurrKey != lKey )
         {
            GetStringFromAttribute( szCurrTag, zsizeof( szCurrTag ), vPE, "ControlDef", "Tag" );
            GetStringFromAttribute( szCurrAcceptsAllParents, zsizeof( szCurrAcceptsAllParents ), vPE,
                                    "ControlDef", "AcceptsAllParents" );
            GetStringFromAttribute( szCurrAcceptsAllChildren, zsizeof( szCurrAcceptsAllChildren ), vPE,
                                    "ControlDef", "AcceptsAllChildren" );
            GetStringFromAttribute( szCurrRequiresParent, zsizeof( szCurrRequiresParent ), vPE,
                                    "ControlDef", "RequiresParent" );
            GetIntegerFromAttribute( &lCurrInvisible, vPE,
                                     "ControlDef", "Invisible" );
            lCurrValidParentCnt = CountEntitiesForView( vPE, "ValidParent" );
            lCurrValidChildCnt = CountEntitiesForView( vPE, "ValidChild" );

            if ( szAcceptsAllParents[ 0 ] == szCurrAcceptsAllParents[ 0 ] &&
                 szAcceptsAllChildren[ 0 ] == szCurrAcceptsAllChildren[ 0 ] &&
                 szRequiresParent[ 0 ] == szCurrRequiresParent[ 0 ] &&
                 lInvisible == lCurrInvisible &&
                 (lValidParentCnt == lCurrValidParentCnt ||
                  (lValidParentCnt && lCurrValidParentCnt)) &&
                 (lValidChildCnt == lCurrValidChildCnt ||
                  (lValidChildCnt && lCurrValidChildCnt)) )
            {
               SetSelectStateOfEntity( vPE, "ControlDef", TRUE );
            }
         }

         nRC = SetCursorNextEntity( vPE, "ControlDef", 0 );
      }

      SetSelectSetForView( vPE, nPrevSelSet );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ChgCtrlPickNewType( zVIEW vSubtask )
{
   zVIEW  vPE;

   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, 0 );
      TraceLineS( "ChangeCtrlType unable to get PE view", "" );
      return( -1 );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ChgCtrlToggleCheck( zVIEW vSubtask )
{
   zVIEW  vPE;
   zSHORT nPrevSelSet;

   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "ChangeCtrlType unable to get PE view", "" );
      return( -1 );
   }

   nPrevSelSet = SetSelectSetForView( vPE, 3 );
   SetAllSelectStatesForEntity( vPE, "ControlDef", FALSE, 0 );
   SetSelectSetForView( vPE, nPrevSelSet );

   ChgCtrlPostbuild( vSubtask );
   if ( GetCtrlState( vSubtask, "ChangeChildType",
                      zCONTROL_STATUS_CHECKED ) == TRUE )
   {
//    SetCtrlState( vSubtask, "ChangeChildType", zCONTROL_STATUS_CHECKED, 1 );
   }
   else
   {
   }

   RefreshCtrl( vSubtask, "ChangeChild" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ChgCtrlChangeType( zVIEW vSubtask )
{
   zCHAR  szTag[ 33 ];
   zCHAR  szNewTag[ 33 ];
   zVIEW  vControl;
   zVIEW  vPE;
   zLONG  lKey;
   zSHORT nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "ChgCtrlChangeType unable to get PE view", "" );
      return( -1 );
   }

   if ( SetEntityCursor( vPE, "ControlDef", "Key",
                         zPOS_FIRST | zQUAL_ENTITYATTR,
                         vControl, "ControlDef", "Key",
                         0, 0, 0 ) >= zCURSOR_SET )
   {
      zCHAR  szMsg[ 512 ];
      zSHORT nPrevSelSet;

      GetStringFromAttribute( szTag, zsizeof( szTag ), vPE, "ControlDef", "Tag" );

      nPrevSelSet = SetSelectSetForView( vPE, 4 );
      SetCursorFirstSelectedEntity( vPE, "ControlDef", 0 );
      GetStringFromAttribute( szNewTag, zsizeof( szNewTag ), vPE, "ControlDef", "Tag" );
      SetSelectSetForView( vPE, nPrevSelSet );

      sprintf_s( szMsg, zsizeof( szMsg ),
                "OK to change control type from: \"%s\" to \"%s\"?\n"
                "N.B.  This is a provisional implementation and the\n"
                "integrity of the changed control cannot be guaranteed\n"
                "Please visit the detail for the control (and any\n"
                "sub-controls) to ensure integrity!\n"
                "Events and control specific information are deleted.\n"
                "Mapping is retained and may therefore be erroneous!",
                szTag, szNewTag );

      if ( MessagePrompt( vSubtask, "PExxx",
                          "Change Control Type",
                          szMsg,
                          zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_NO,  0 ) != zRESPONSE_NO )
      {
         ExcludeEntity( vControl, "ControlDef", zREPOS_NONE );
         IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                        vPE, "ControlDef", zPOS_BEFORE );
         SetAttributeFromBlob( vControl, "Control", "CtrlBOI", "", 0 );

         nRC = SetCursorFirstEntity( vControl, "Event", 0 );
         while ( nRC == zCURSOR_SET )
         {
            nRC = DeleteEntity( vControl, "Event", zREPOS_FIRST );
         }

         GetIntegerFromAttribute( &lKey, vPE, "ControlDef", "Key" );

         if ( SetCursorFirstEntity( vPE, "ValidChild", 0 ) == zCURSOR_SET &&
              SetEntityCursor( vPE, "ControlDef", "Key",
                               zPOS_FIRST | zQUAL_ENTITYATTR,
                               vPE, "ValidChild", "Key",
                               0, 0, 0 ) >= zCURSOR_SET )
         {
            if ( SetCursorFirstEntity( vPE, "ValidParent", 0 ) == zCURSOR_SET )
            {
               zLONG lKeyParent;

               GetIntegerFromAttribute( &lKeyParent, vPE,
                                        "ValidParent", "Key" );
               if ( lKey == lKeyParent )
               {
                  nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", 0 );
                  if ( nRC == zCURSOR_SET )
                  {
                     SetViewToSubobject( vControl, "CtrlCtrl" );
                     while ( nRC == zCURSOR_SET )
                     {
                        ExcludeEntity( vControl, "ControlDef", zREPOS_NONE );
                        IncludeSubobjectFromSubobject( vControl,
                                                       "ControlDef", vPE,
                                                       "ControlDef",
                                                       zPOS_BEFORE );
                        SetAttributeFromBlob( vControl, "Control",
                                              "CtrlBOI", "", 0 );

                        nRC = SetCursorFirstEntity( vControl, "Event", 0 );
                        while ( nRC == zCURSOR_SET )
                        {
                           nRC = DeleteEntity( vControl, "Event",
                                               zREPOS_FIRST );
                        }

                        nRC = SetCursorNextEntity( vControl, "Control", 0 );
                     }

                     ResetViewFromSubobject( vControl );
                  }
               }
            }
         }
      }
   }
   else
      TraceLineS( "ChangeCtrlType: ", "Cannot locate Entity name" );

   GetViewByName( &vControl, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vControl ) == 0 )
   {
   }

   return( 0 );
}

void
CtrlList_LoadDetailWindow( zVIEW  vSubtask,
                           zVIEW  vPE,
                           zSHORT IsDoubleClick )
{
   zPCHAR pchXSLT;
   zPCHAR pchWindow;

   // reset Expand State
   if ( IsDoubleClick )
   {
      zLONG lExpand = OL_GetCtrlState( vSubtask, "CtrlList", zCONTROL_STATUS_EXPAND );
      OL_SetCtrlState( vSubtask, "CtrlList", zCONTROL_STATUS_EXPAND, !lExpand );
   }

   GetAddrForAttribute( &pchXSLT, vPE,
                        "ControlDef", "MaintenanceXSLT" );
   GetAddrForAttribute( &pchWindow, vPE,
                        "ControlDef", "MaintenanceWindow" );
   SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                            pchXSLT, pchWindow );

}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListDetailWindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
CtrlListDetailWindow( zVIEW  vSubtask,
                      zSHORT nIsDoubleClick )
{
   zCHAR  szEntityName[ 33 ];
   zCHAR  *pchTag;
   zVIEW vControl;
   zVIEW vPE;

// TraceLineS( "CtrlListDetail", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vControl, szEntityName ) < 0 ||
        zstrcmp( szEntityName, "Control" ) != 0 )
   {
      return( 0 );
   }

   if ( SaveCtrlList( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vControl, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vControl ) == 0 )
   {
   }

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pchTag, vControl, "Control", "Tag" );
   fnPainterCall( zMSG_SETCURRCTRL, vSubtask, 0, pchTag, zTAG_LTH );
   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "CtrlListDetail unable to get PE view", "" );
      return( -1 );
   }

   if ( SetEntityCursor( vPE, "ControlDef", "Key",
                         zPOS_FIRST | zQUAL_ENTITYATTR,
                         vControl, "ControlDef", "Key",
                         0, 0, 0 ) >= zCURSOR_SET )
   {
      CtrlList_LoadDetailWindow( vSubtask, vPE, nIsDoubleClick );
   }

   return( 0 );
} // CtrlListDetailWindow

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListDetail_ByDoubleClick
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListDetail_ByDoubleClick( zVIEW vSubtask )
{
   return( CtrlListDetailWindow( vSubtask, 1 ) );

} // CtrlListDetail_ByDoubleClick

zOPER_EXPORT zSHORT OPERATION
CtrlListDetail( zVIEW vSubtask )
{
   return( CtrlListDetailWindow( vSubtask, 0 ) );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListAddMapEntity( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW vControl;

// TraceLineS( "CtrlListAddMapEntity", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vControl, szEntityName ) < 0 ||
        zstrcmp( szEntityName, "Control" ) != 0 )
   {
      return( 0 );
   }

   GetViewByName( &vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vControl, "CtrlMap", zPOS_AFTER );
   SetAttributeFromString( vControl, "CtrlMap", "Tag", "" );
   RefreshCtrl( vSubtask, "Mapping" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListDeleteMapEntity( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW vControl;

// TraceLineS( "CtrlListDeleteMapEntity", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vControl, szEntityName ) < 0 ||
        zstrcmp( szEntityName, "Control" ) != 0 )
   {
      return( 0 );
   }

   GetViewByName( &vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) == zCURSOR_SET )
   {
      DeleteEntity( vControl, "CtrlMap", zREPOS_PREV );
      RefreshCtrl( vSubtask, "Mapping" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListDeleteMapAttribute( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW vControl;

// TraceLineS( "CtrlListDeleteMapAttribute", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vControl, szEntityName ) < 0 ||
        zstrcmp( szEntityName, "Control" ) != 0 )
   {
      return( 0 );
   }

   GetViewByName( &vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) == zCURSOR_SET )
   {
      ExcludeEntity( vControl, "CtrlMapER_Attribute", zREPOS_NONE );
      RefreshCtrl( vSubtask, "Mapping" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListDeleteCtrl( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szCtrlTag[ 34 ];
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( vControl )
   {
      GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vControl, "Control", "Tag" );
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Control '" );
      strcat_s( szMsg, zsizeof( szMsg ), szCtrlTag );
      strcat_s( szMsg, zsizeof( szMsg ), "'?" );
      if ( MessagePrompt( vSubtask, "PE002",
                          "XSLT Maintenance",
                          szMsg, zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_NO,  0 ) == zRESPONSE_YES )
      {
         DeleteEntity( vControl, "Control", zREPOS_AFTER );
         GetViewByName( &vControl, "TZXSLT", vSubtask, zLEVEL_TASK );
         while ( ResetViewFromSubobject( vControl ) == 0 )
         {
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
Undo( zVIEW vSubtask )
{
   fnPainterCall( zMSG_UNDO, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Redo( zVIEW vSubtask )
{
   fnPainterCall( zMSG_REDO, vSubtask, 0, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
CopySelectedCtrlsToClipboard( zVIEW vSubtask )
{
   fnPainterCall( zMSG_CLIPBOARD_COPY, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CopyClipboardToSelectedCtrl( zVIEW vSubtask )
{
   fnPainterCall( zMSG_CLIPBOARD_PASTE, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CopyMenuToClipboard( zVIEW vSubtask )
{
   fnPainterCall( zMSG_CLIPBOARD_COPY_MENU, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CopyClipboardMenuToWindow( zVIEW vSubtask )
{
   fnPainterCall( zMSG_CLIPBOARD_PASTE_MENU, vSubtask, 0, 0, 0 );
   return( 0 );
}

zSHORT
fnPositionOnVOR( zVIEW     vTgt,
                 zVIEW     vSrc,
                 zVIEW     vSrcLPLR,
                 zPCHAR    pchViewName,
                 zVIEW     vSubtask )
{
   zVIEW     vVOR;
   zVIEW     vVOR_List;
   zVIEW     vLOD;
   zVIEW     vLOD_List;
   zCHAR     szMsg[ 65 ];
   zCHAR     szName[ 33 ];
   zSHORT    nRC;

   if ( SetCursorFirstEntityByString( vTgt, "ViewObjRef", "Name",
                                      pchViewName, 0 ) < 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vVOR_List, zREFER_VOR_META );
      nRC = SetCursorFirstEntityByString( vVOR_List, "W_MetaDef",
                                          "Name", pchViewName, 0 );
      if ( nRC >= 0 )
      {
         ActivateMetaOI( vSubtask, &vVOR, vVOR_List, zREFER_VOR_META, zSINGLE );
         IncludeSubobjectFromSubobject( vTgt, "ViewObjRef", vVOR,
                                        "ViewObjRef", zPOS_AFTER );
      }
      else
      {
         RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
         nRC = SetCursorFirstEntityByString( vSrc, "ViewObjRef",
                                             "Name", pchViewName, 0 );
         if ( nRC < 0 )
         {
            strcpy_s( szMsg, zsizeof( szMsg ), "Window has mapping with no VOR: " );
            strcat_s( szMsg, zsizeof( szMsg ), pchViewName );
            MessageSend( vSubtask, "WD00207", "Named View Clone", szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            DropView( vVOR_List );
            DropView( vLOD_List );
            return( -1 );
         }

         GetStringFromAttribute( szName, zsizeof( szName ), vSrc, "LOD", "Name" );
         nRC = SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                             "Name", szName, 0 );
         if ( nRC >= 0 )
         {
            ActivateEmptyMetaOI( vSubtask, &vVOR, zSOURCE_VOR_META, zSINGLE );
            CreateMetaEntity( vSubtask, vVOR, "ViewObjRef", zPOS_AFTER );
            SetMatchingAttributesByName( vVOR, "ViewObjRef", vSrc,
                                         "ViewObjRef", zSET_NULL );
            ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zSINGLE );
            IncludeSubobjectFromSubobject( vVOR, "LOD", vLOD,
                                           "LOD", zPOS_AFTER );
            CommitMetaOI( vSubtask, vVOR, zSOURCE_VOR_META );
            IncludeSubobjectFromSubobject( vTgt, "ViewObjRef", vVOR,
                                           "ViewObjRef", zPOS_AFTER );
            DropMetaOI( vSubtask, vLOD );
         }
         else
         {
            GetVariableFromAttribute( szName, 0, 'S', 33,
                                      vSrc, "LOD", "Name", "", 0 );
            strcpy_s( szMsg, zsizeof( szMsg ), "Non-existent LOD: " );
            strcat_s( szMsg, zsizeof( szMsg ), szName );
            MessageSend( vSubtask, "WD00208", "Named View Clone",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            DropView( vVOR_List );
            DropView( vLOD_List );
            return( -1 );
         }

         DropView( vLOD_List );
      }

      DropMetaOI( vSubtask, vVOR );
      DropView( vVOR_List );
   }

   return( 0 );
}

zSHORT
fnCloneActMap( zVIEW     vSrcLPLR,
               zVIEW     vSrc,
               zVIEW     vTgt,
               zVIEW     vSubtask )
{
   zVIEW     vLOD;
   zVIEW     vLOD_List;
   zCHAR     szMsg[ 65 ];
   zCHAR     szTag[ 33 ];
   zSHORT    nRC;

   CreateMetaEntity( vSubtask, vTgt, "ActMap", zPOS_AFTER );
   SetMatchingAttributesByName( vTgt, "ActMap", vSrc, "ActMap", zSET_NULL );
   if ( CheckExistenceOfEntity( vSrc, "ActMapView" ) >= 0 )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vSrc, "ActMapView", "Name" );
      if ( fnPositionOnVOR( vTgt, vSrc, vSrcLPLR, szTag, vSubtask ) >= 0 )
      {
         IncludeSubobjectFromSubobject( vTgt, "ActMapView",
                                        vTgt, "ViewObjRef", zPOS_AFTER );
      }
      else
      {
         return( -1 );
      }

      RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
      GetStringFromAttribute( szTag, zsizeof( szTag ), vTgt, "LOD", "Name" );
      nRC = SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                          "Name", szTag, 0 );
      if ( GetViewByName( &vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK ) > 0 )
         DropMetaOI( vSubtask, vLOD );

      ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zSINGLE );
      SetNameForView( vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK );
      DropView( vLOD_List );
      if ( CheckExistenceOfEntity( vSrc, "ActMapLOD_Entity" ) >= 0 )
      {
         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrc, "ActMapLOD_Entity", "Name" );
         nRC = SetCursorFirstEntityByString( vLOD, "LOD_Entity",
                                             "Name", szTag, 0 );
         if ( nRC >= 0 )
         {
            IncludeSubobjectFromSubobject( vTgt, "ActMapLOD_Entity",
                                           vLOD, "LOD_Entity", zPOS_AFTER );
         }
         else
         {
            GetVariableFromAttribute( szTag, 0, 'S', 33, vSrc,
                                      "ActMapLOD_Entity", "Name", "", 0 );
            strcpy_s( szMsg, zsizeof( szMsg ), "LOD_Entity doesn't exist: " );
            strcat_s( szMsg, zsizeof( szMsg ), szTag );
            MessageSend( vSubtask, "WD00214", "XSLT Clone", szMsg,
                         zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         }
      }
   }
   else
   {
      GetViewByName( &vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

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
                                          szTag, "XSLT" );
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
                                      "XSLT", "Tag", "", 0 );
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
                                             "Name", szTag, "XSLT" );
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

zSHORT
fnCtrlAttributeMapping( zVIEW  vSubtask,
                        zVIEW  vSrcLPLR,
                        zVIEW  vSrcC,
                        zVIEW  vTgtC,
                        zVIEW  vLOD )
{
   zVIEW     vDomain;
   zVIEW     vDomainList;
   zCHAR     szMsg[ 65 ];
   zCHAR     szTag[ 33 ];
   zLONG     lZKey;
   zSHORT    nRC;

   GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlMapER_Attribute", "Name" );
   nRC = SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name",
                                       szTag, "LOD_Entity" );
   if ( nRC >= 0 )
   {
      IncludeSubobjectFromSubobject( vTgtC, "CtrlMapLOD_Attribute",
                                     vLOD, "LOD_Attribute", zPOS_AFTER );
      if ( CheckExistenceOfEntity( vSrcC, "CtrlMapContext" ) >= 0 )
      {
         nRC = SetCursorFirstEntityByInteger( vSrcLPLR, "W_MetaType",
                                              "Type", 2003, 0 );
         GetIntegerFromAttribute( &lZKey, vSrcC, "CtrlMapER_Domain", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vSrcLPLR, "W_MetaDef",
                                              "CPLR_ZKey", lZKey, 0 );
         RetrieveViewForMetaList( vSubtask, &vDomainList, zREFER_DOMAIN_META );
         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcLPLR, "W_MetaDef", "Name" );
         nRC = SetCursorFirstEntityByString( vDomainList, "W_MetaDef",
                                             "Name", szTag, 0 );
         if ( nRC >= 0 )
         {
            ActivateMetaOI( vSubtask, &vDomain, vDomainList,
                            zREFER_DOMAIN_META, zSINGLE );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlMapContext", "Name" );
            nRC = SetCursorFirstEntityByString( vDomain, "Context", "Name",
                                                szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtC, "CtrlMapContext",
                                              vDomain, "Context", zPOS_AFTER );
            }
            else
            {
               GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcC,
                                         "CtrlMapContext", "Name", "", 0 );
               strcpy_s( szMsg, zsizeof( szMsg ), "Context doesn't exist within Domain: " );
               strcat_s( szMsg, zsizeof( szMsg ), szTag );
               strcat_s( szMsg, zsizeof( szMsg ), ", " );
               GetVariableFromAttribute( szTag, 0, 'S', 33,
                                         vSrcLPLR, "W_MetaDef",
                                         "Name", "", 0 );
               strcat_s( szMsg, zsizeof( szMsg ), szTag );
               MessageSend( vSubtask, "WD00211", "XSLT Clone",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
            }

            DropMetaOI( vSubtask, vDomain );
         }
         else
         {
            GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcLPLR,
                                      "W_MetaDef", "Name", "", 0 );
            strcpy_s( szMsg, zsizeof( szMsg ), "Domain doesn't exist: " );
            strcat_s( szMsg, zsizeof( szMsg ), szTag );
            MessageSend( vSubtask, "WD00212", "XSLT Clone",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         }

         DropView( vDomainList );
      }
   }
   else
   {
      GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcC,
                                "CtrlMapER_Attribute", "Name", "", 0 );
      strcpy_s( szMsg, zsizeof( szMsg ), "ER_Attribute doesn't exist: " );
      strcat_s( szMsg, zsizeof( szMsg ), szTag );
      MessageSend( vSubtask, "WD00213", "XSLT Clone", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
   }

   return( 0 );
}

zSHORT
fnCloneCtrlMap( zVIEW     vTgt,
                zVIEW     vTgtC,
                zVIEW     vSrc,
                zVIEW     vSrcC,
                zVIEW     vSrcLPLR,
                zVIEW     vSubtask )
{
   zVIEW     vLOD;
   zVIEW     vLOD_List;
   zCHAR     szMsg[ 65 ];
   zCHAR     szTag[ 33 ];
   zSHORT    nRC;

   CreateMetaEntity( vSubtask, vTgtC, "CtrlMap", zPOS_AFTER );
   SetMatchingAttributesByName( vTgtC, "CtrlMap",
                                vSrcC, "CtrlMap", zSET_NULL );
   if ( CheckExistenceOfEntity( vSrcC, "CtrlMapView" ) >= 0 )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlMapView", "Name" );
      if ( fnPositionOnVOR( vTgt, vSrc, vSrcLPLR, szTag, vSubtask ) >= 0 )
      {
         IncludeSubobjectFromSubobject( vTgtC, "CtrlMapView",
                                        vTgt, "ViewObjRef", zPOS_AFTER );
      }
      else
      {
         return( -1 );
      }

      RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
      GetStringFromAttribute( szTag, zsizeof( szTag ), vTgt, "LOD", "Name" );
      nRC = SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                          "Name", szTag, 0 );
      if ( GetViewByName( &vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK ) > 0 )
         DropMetaOI( vSubtask, vLOD );

      ActivateMetaOI( vSubtask, &vLOD, vLOD_List, zREFER_LOD_META, zSINGLE );
      SetNameForView( vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK );
      DropView( vLOD_List );
   }

   if ( CheckExistenceOfEntity( vSrcC, "CtrlMapLOD_Entity" ) >= 0 )
   {
      GetViewByName( &vLOD, "TZTMPLOD", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlMapLOD_Entity", "Name" );
      nRC = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name",
                                          szTag, 0 );
      if ( nRC >= 0 )
      {
         IncludeSubobjectFromSubobject( vTgtC, "CtrlMapLOD_Entity",
                                        vLOD, "LOD_Entity", zPOS_AFTER );
      }
      else
      {
         GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcC,
                                   "CtrlMapLOD_Entity", "Name", "", 0 );
         strcpy_s( szMsg, zsizeof( szMsg ), "LOD_Entity doesn't exist: " );
         strcat_s( szMsg, zsizeof( szMsg ), szTag );
         MessageSend( vSubtask, "WD00209", "Control Clone",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      }
   }

   if ( CheckExistenceOfEntity( vSrcC, "CtrlMapLOD_Attribute" ) >= 0 )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlMapRelatedEntity", "Name" );
      nRC = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name",
                                          szTag, 0 );
      if ( nRC >= 0 )
      {
         fnCtrlAttributeMapping( vSubtask, vSrcLPLR, vSrcC, vTgtC, vLOD );
      }
      else
      {
         GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcC,
                                   "CtrlMapRelatedEntity", "Name", "", 0 );
         strcpy_s( szMsg, zsizeof( szMsg ), "LOD_Entity doesn't exist: " );
         strcat_s( szMsg, zsizeof( szMsg ), szTag );
         MessageSend( vSubtask, "WD00210", "Control Clone",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      }
   }

   return( 0 );
}

// Return TRUE if a duplicate tag is found.
zBOOL
fnEnsureUniqueCtrlTag( zVIEW vSubtask, zCPCHAR cpcCtrlTag )
{
   zPCHAR pchTag;
   zBOOL  bDuplicate = FALSE;
   zSHORT nRC = SetCursorFirstEntity( vSubtask, "Control", 0 );

   while ( bDuplicate == FALSE && nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( &pchTag, vSubtask, "Control", "Tag" );
      if ( zstrcmp( cpcCtrlTag, pchTag ) == 0 )
      {
         bDuplicate = TRUE;
      }
      else
      if ( SetViewToSubobject( vSubtask, "CtrlCtrl" ) >= 0 )
      {
         bDuplicate = fnEnsureUniqueCtrlTag( vSubtask, cpcCtrlTag );
         ResetViewFromSubobject( vSubtask );
      }

      nRC = SetCursorNextEntity( vSubtask, "Control", 0 );
   }

   return( bDuplicate );
}

zSHORT
fnCloneControl( zVIEW     vSrcLPLR,
                zVIEW     vSrc,
                zVIEW     vSrcC,
                zVIEW     vTgt,
                zVIEW     vTgtC,
                zVIEW     vPE,
                zBOOL     bXSLT,
                zVIEW     vSubtask )
{
   zVIEW     vXSLT;
   zCHAR     szMsg[ 65 ];
   zCHAR     szTag[ 33 ];
   zLONG     lKey;
   zLONG     lUniqueId;
   zSHORT    nLth;
   zSHORT    nRC;

   CreateMetaEntity( vSubtask, vTgtC, "Control", zPOS_AFTER );
   SetMatchingAttributesByName( vTgtC, "Control",
                                vSrcC, "Control", zSET_NULL );
   if ( bXSLT == FALSE )
   {
      if ( CheckExistenceOfEntity( vSrcC, "Font" ) == 0 )
      {
         CreateMetaEntity( vSubtask, vTgtC, "Font", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtC, "Font", vSrcC, "Font", zSET_NULL );
      }

      if ( CheckExistenceOfEntity( vSrcC, "WebControlProperty" ) == 0 )
      {
         CreateMetaEntity( vSubtask, vTgtC, "WebControlProperty", zPOS_AFTER );
         SetMatchingAttributesByName( vTgtC, "WebControlProperty",
                                      vSrcC, "WebControlProperty", zSET_NULL );
      }
   }

   /*if ( CheckExistenceOfEntity( vSrcC, "ControlNLS_Text" ) == 0 )
   {
      CreateMetaEntity( vSubtask, vTgtC, "ControlNLS_Text", zPOS_AFTER );
      SetMatchingAttributesByName( vTgtC, "ControlNLS_Text",
                                   vSrcC, "ControlNLS_Text", zSET_NULL );
   }

   if ( CheckExistenceOfEntity( vSrcC, "ControlNLS_DIL_Text" ) == 0 )
   {
      CreateMetaEntity( vSubtask, vTgtC, "ControlNLS_DIL_Text", zPOS_AFTER );
      SetMatchingAttributesByName( vTgtC, "ControlNLS_DIL_Text",
                                   vSrcC, "ControlNLS_DIL_Text", zSET_NULL );
   }*/

   // Reset the current control's tag so we won't find it by Tag while we
   // ensure that the tag is unique.
   GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "Control", "Tag" );
   strcpy_s( szMsg, zsizeof( szMsg ), szTag ); // hold onto original tag
   SetAttributeFromString( vSrcC, "Control", "Tag",
                           "__&&&__^^^__Clone__@@@__%%%__" );

   CreateViewFromViewForTask( &vXSLT, vTgtC, 0 );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   if ( fnEnsureUniqueCtrlTag( vXSLT, szTag ) )
   {
      // Force unique CtrlTag here.
      nLth = (zSHORT) zstrlen( szTag );
      while ( nLth && szTag[ nLth - 1 ] >= '0' && szTag[ nLth - 1 ] <= '9' )
         nLth--;

      lUniqueId = zatol( szTag + nLth );
      if ( nLth == 0 )
      {
         strcpy_s( szTag, zsizeof( szTag ), "Tag" );
         nLth = 3;
      }

      while ( fnEnsureUniqueCtrlTag( vXSLT, szTag ) )
      {
         lUniqueId++;
         zltoa( lUniqueId, szTag + nLth, zsizeof( szTag ) - nLth, 10 );
      }
   }

   SetAttributeFromString( vSrcC, "Control", "Tag", szMsg );
   SetAttributeFromString( vTgtC, "Control", "Tag", szTag );
   DropView( vXSLT );

   GetIntegerFromAttribute( &lKey, vSrcC, "ControlDef", "Key" );
   nRC = SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", lKey, 0 );
   if ( nRC >= 0 )
   {
      IncludeSubobjectFromSubobject( vTgtC, "ControlDef",
                                     vPE, "ControlDef", zPOS_AFTER );
   }
   else
   {
      if ( CompareAttributeToString( vSrcC, "ControlDef",
                                     "Tag", "Bitmap" ) == 0 )
      {
         nRC = SetCursorFirstEntityByString( vPE, "ControlDef", "Tag",
                                             "BitmapBtn", 0 );
         IncludeSubobjectFromSubobject( vTgtC, "ControlDef",
                                        vPE, "ControlDef", zPOS_AFTER );
      }
      else
      {
         GetVariableFromAttribute( szTag, 0, 'S', 33,
                                   vSrcC, "ControlDef", "Tag", "", 0 );
         strcpy_s( szMsg, zsizeof( szMsg ), "ControlDef doesn't exist: " );
         strcat_s( szMsg, zsizeof( szMsg ), szTag );
         MessageSend( vSubtask, "WD00204", "XSLT Clone",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      }
   }

   if ( bXSLT == FALSE )
   {
#if 0
      nRC = SetCursorFirstEntity( vSrcC, "CtrlClrOverride", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC,
            "CtrlColorDef", "Tag" );
         nRC = SetCursorFirstEntityByString( vPE, "CtrlColorDef", "Tag",
                                             szTag, 0 );
         if ( nRC >= 0 )
         {
            CreateMetaEntity( vSubtask, vTgtC, "CtrlClrOverride", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vTgtC, "CtrlColorDef",
                                           vPE, "CtrlColorDef", zPOS_AFTER );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlColor", "Tag" );
            nRC = SetCursorFirstEntityByString( vPE, "Color", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtC, "CtrlColor",
                                              vPE, "Color", zPOS_AFTER );
            }
            else
            {
               IncludeSubobjectFromSubobject( vTgtC, "CtrlColor",
                                              vPE, "CtrlColor", zPOS_AFTER );
            }
         }
         else
         {
            GetVariableFromAttribute( szTag, 0, 'S', 33, vSrcC,
                                      "CtrlColorDef", "Tag", "", 0 );
            strcpy_s( szMsg, zsizeof( szMsg ), "Color Definition doesn't exist: " );
            strcat_s( szMsg, zsizeof( szMsg ), szTag );
            MessageSend( vSubtask, "WD00205", "XSLT Clone",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         }

         nRC = SetCursorNextEntity( vSrcC, "CtrlClrOverride", 0 );
      }

      nRC = SetCursorFirstEntity( vSrcC, "CtrlFontOverride", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlFontDef", "Tag" );
         nRC = SetCursorFirstEntityByString( vPE, "CtrlFontDef", "Tag",
                                             szTag, 0 );
         if ( nRC >= 0 )
         {
            CreateMetaEntity( vSubtask, vTgtC, "CtrlFontOverride", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vTgtC, "CtrlFontDef", vPE,
                                           "CtrlFontDef", zPOS_AFTER );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "CtrlFont", "Tag" );
            nRC = SetCursorFirstEntityByString( vPE, "Font", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtC, "CtrlFont",
                                              vPE, "Font", zPOS_AFTER );
            }
            else
            {
               IncludeSubobjectFromSubobject( vTgtC, "CtrlFont",
                                              vPE, "CtrlFont", zPOS_AFTER );
            }
         }
         else
         {
            GetVariableFromAttribute( szTag, 0, 'S', 33,
               vSrcC, "CtrlFontDef", "Tag", "", 0 );
            strcpy_s( szMsg, zsizeof( szMsg ), "Font Definition doesn't exist: " );
            strcat_s( szMsg, zsizeof( szMsg ), szTag );
            MessageSend( vSubtask, "WD00206", "XSLT Clone",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         }

         nRC = SetCursorNextEntity( vSrcC, "CtrlFontOverride", 0 );
      }
#endif
   }

   nRC = SetCursorFirstEntity( vSrcC, "CtrlCtrl", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      SetViewToSubobject( vSrcC, "CtrlCtrl" );
      SetViewToSubobject( vTgtC, "CtrlCtrl" );
      nRC = fnCloneControl( vSrcLPLR, vSrc, vSrcC, vTgt, vTgtC,
                            vPE, bXSLT, vSubtask );
      ResetViewFromSubobject( vSrcC );
      ResetViewFromSubobject( vTgtC );
      if ( nRC == -1 )
         return( -1 );

      nRC = SetCursorNextEntity( vSrcC, "CtrlCtrl", 0 );
   }

   nRC = SetCursorFirstEntity( vSrcC, "CtrlMap", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneCtrlMap( vTgt, vTgtC, vSrc, vSrcC, vSrcLPLR, vSubtask ) == -1 )
         return( -1 );

      nRC = SetCursorNextEntity( vSrcC, "CtrlMap", 0 );
   }

   if ( bXSLT == FALSE )
   {
      nRC = SetCursorFirstEntity( vSrcC, "Event", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( CompareAttributeToString( vSrcC, "EventAct", "Tag", "" ) != 0 )
         {
            CreateMetaEntity( vSubtask, vTgtC, "Event", zPOS_AFTER );
            SetMatchingAttributesByName( vTgtC, "Event",
                                         vSrcC, "Event", zSET_NULL );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "EventAct", "Tag" );
            nRC = SetCursorFirstEntityByString( vTgt, "Action", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtC, "EventAct",
                                              vTgt, "Action", zPOS_AFTER );
            }
         }

         nRC = SetCursorNextEntity( vSrcC, "Event", 0 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_DisableMenuForError( zVIEW vSubtask, zBOOL bEnable )
{
   SetOptionState( vSubtask, "NewFile",     zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Open",        zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "XSLT Spec", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "ScrollBars",  zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Grid",        zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "TargetSpecification",
                                            zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "CompilerSpecification",
                                            zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "ToolbarControls",
                                            zOPTION_STATUS_ENABLED, bEnable );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DisableAllMenuItems
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_DisableAllMenuItems( zVIEW vSubtask )
{
   zVIEW vTaskLPLR;

   // If there is an error situation disable all menu item without
   // "Switch Project".
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      zwfnTZXSLTDD_DisableMenuForError( vSubtask, FALSE );
   else
      zwfnTZXSLTDD_DisableMenuForError( vSubtask, TRUE );

   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "GenerateXSLT", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Copy", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddWindow", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddMenu", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddAction", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddShortcut", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddSourceFile", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateMenu", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateAction", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateShortcut", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateSourceFile", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteWindow", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteControls", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteMenus", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteActions", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteShortcuts", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteSourceFiles", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteOperations", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllMenus", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllAction", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAllShortcuts", zOPTION_STATUS_ENABLED, 0 );

   SetOptionState( vSubtask, "AlignLeft", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AlignRight", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AlignTop", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AlignBottom", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EqualHeight", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EqualWidth", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EqualWidthHeight", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EquiSpaceVertical", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "EquiSpaceHorizontal", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AbutVertical", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AbutHorizontal", zOPTION_STATUS_ENABLED, 0 );

   SetOptionState( vSubtask, "SetTabstops", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "RemoveTabbing", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AutodesignXSLT", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "AutodesignWindow", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "CopyXSLTWindow", zOPTION_STATUS_ENABLED, 0 );

   SetOptionState( vSubtask, "IncrementBothW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "IncrementXW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "IncrementYW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementBothW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementXW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementYW", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "IncrementBothD", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "IncrementXD", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "IncrementYD", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementBothD", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementXD", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DecrementYD", zOPTION_STATUS_ENABLED, 0 );

   SetOptionState( vSubtask, "MergeXSLT Window", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteAct", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Caption", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "ControlText", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "OptionText", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "ControlDILText", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "SystemConversion", zOPTION_STATUS_ENABLED, 0 );

   return( 0 );
} // TZXSLTDD_DisableAllMenuItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_InitEditMenuItems
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_InitEditMenuItems( zVIEW vSubtask )
{
   zSHORT   nUpdate = 0;
   zSHORT   nDelete = 0;
   zSHORT   nEnable = 0;

   //Menus
   TZXSLTDD_EnableMenuItems( vSubtask, "LIST_MENU", "Menu", "MenuList",
                               &nUpdate, &nDelete );
   SetOptionState( vSubtask, "UpdateMenu", zOPTION_STATUS_ENABLED, nUpdate );
   SetOptionState( vSubtask, "DeleteMenus", zOPTION_STATUS_ENABLED, nDelete );
   nEnable = TZXSLTDD_EnableMenuDeleteAll( vSubtask, "Menu" );
   SetOptionState( vSubtask, "DeleteAllMenus", zOPTION_STATUS_ENABLED, nEnable );

   //Actions
   TZXSLTDD_EnableMenuItems( vSubtask, "LIST_ACT", "Action", "ActionList",
                               &nUpdate, &nDelete );
   SetOptionState( vSubtask, "UpdateAction", zOPTION_STATUS_ENABLED, nUpdate );
   SetOptionState( vSubtask, "DeleteActions", zOPTION_STATUS_ENABLED, nDelete );
   nEnable = TZXSLTDD_EnableMenuDeleteAll( vSubtask, "Action" );
   SetOptionState( vSubtask, "DeleteAllAction", zOPTION_STATUS_ENABLED, nEnable );

   //ShortCuts
   TZXSLTDD_EnableMenuItems( vSubtask, "LIST_HK", "Hotkey", "Hotkeys",
                               &nUpdate, &nDelete );
   SetOptionState( vSubtask, "UpdateShortcut", zOPTION_STATUS_ENABLED, nUpdate );
   SetOptionState( vSubtask, "DeleteShortcuts", zOPTION_STATUS_ENABLED, nDelete );
   nEnable = TZXSLTDD_EnableMenuDeleteAll( vSubtask, "Hotkey" );
   SetOptionState( vSubtask, "DeleteAllShortcuts", zOPTION_STATUS_ENABLED, nEnable );

   return( 0 );

} // TZXSLTDD_InitEditMenuItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_InitMenuItems
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_InitMenuItems( zVIEW vSubtask )
{
   zVIEW    vWindow;
   zVIEW    vTZXSLTL;
   zVIEW    vTZXSLT;
   zVIEW    vTZOPRUSO;
   zCHAR    szExecutableRemote[ zMAX_FILESPEC_LTH ];
   zSHORT   nIsCheckedOut = 0;

   TZXSLTDD_DisableAllMenuItems( vSubtask );

   // Generate XSLT's
   SysReadZeidonIni( -1, "[Workstation]", "ExecutableRemote", szExecutableRemote, zsizeof( szExecutableRemote ) );
   if ( szExecutableRemote[ 0 ] )
      SetOptionState( vSubtask, "GenerateXSLT", zOPTION_STATUS_ENABLED, 1 );

   if ( GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META );

   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddWindow", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddSourceFile", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AddOperation", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AutodesignXSLT", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AutodesignWindow", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "CopyXSLTWindow", zOPTION_STATUS_ENABLED, 1 );

      SetOptionState( vSubtask, "IncrementBothW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "IncrementXW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "IncrementYW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementBothW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementXW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementYW", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "IncrementBothD", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "IncrementXD", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "IncrementYD", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementBothD", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementXD", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DecrementYD", zOPTION_STATUS_ENABLED, 1 );

      SetOptionState( vSubtask, "MergeXSLT Window", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "DeleteAct", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "SystemConversion", zOPTION_STATUS_ENABLED, 1 );

      if ( GetViewByName( &vTZXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         SetOptionState( vSubtask, "AddMenu", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddAction", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "AddShortcut", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "DeleteWindow", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "RemoveTabbing", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "Caption", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "ControlText", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "OptionText", zOPTION_STATUS_ENABLED, 1 );
         SetOptionState( vSubtask, "ControlDILText", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   //Source File
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) < 0 )
      SetOptionState( vSubtask, "SourceFiles", zCONTROL_STATUS_CHECKED, 0 );
   else
   {
      SetOptionState( vSubtask, "SourceFiles", zCONTROL_STATUS_CHECKED, 1 );
      if ( GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 &&
           GetSelectStateOfEntity( vTZOPRUSO, "SourceFile" ) == 1 )
      {
         SetOptionState( vSubtask, "UpdateSourceFile", zOPTION_STATUS_ENABLED, 1 );
         if ( nIsCheckedOut == 1 )
            SetOptionState( vSubtask, "DeleteSourceFiles", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   //Operations
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) < 0 )
      SetOptionState( vSubtask, "Opers", zCONTROL_STATUS_CHECKED, 0 );
   else
   {
      SetOptionState( vSubtask, "Opers", zCONTROL_STATUS_CHECKED, 1 );
      if ( GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 &&
           GetSelectStateOfEntity( vTZOPRUSO, "OperationList" ) == 1 )
      {
         SetOptionState( vSubtask, "UpdateOperation", zOPTION_STATUS_ENABLED, 1 );
         if ( nIsCheckedOut == 1 )
            SetOptionState( vSubtask, "DeleteOperations", zOPTION_STATUS_ENABLED, 1 );
      }
   }

   TZXSLTDD_InitEditMenuItems( vSubtask );

   return( 0 );
} // TZXSLTDD_InitMenuItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EnableMenuDeleteAll
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_EnableMenuDeleteAll( zVIEW   vSubtask,
                              zPCHAR  szEntityName )
{
   zVIEW  vTZXSLTL;
   zSHORT nEnable = 0;

   if ( GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZXSLTL, szEntityName ) >= zCURSOR_SET  &&
        ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META ) == 1 )
   {
      nEnable = 1;
   }

   return( nEnable );
} // TZXSLTDD_EnableMenuDeleteAll

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_EnableMenuItems
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_EnableMenuItems( zVIEW    vSubtask,
                          zPCHAR   szListname,
                          zPCHAR   szEntityName,
                          zPCHAR   szMenuItem,
                          zPSHORT  nEnableUpdate,
                          zPSHORT  nEnableDelete )
{
   zVIEW  vWindow;
   zVIEW  vTZXSLTL;

   *nEnableUpdate = 0;
   *nEnableDelete = 0;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, szListname ) < 0 )
      SetOptionState( vSubtask, szMenuItem, zCONTROL_STATUS_CHECKED, 0 );
   else
   {
      SetOptionState( vSubtask, szMenuItem, zCONTROL_STATUS_CHECKED, 1 );
      if ( GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) >= 0 &&
           GetSelectStateOfEntity( vTZXSLTL, szEntityName ) == 1 )
      {
         *nEnableUpdate = 1;
         if ( ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META ) == 1 )
            *nEnableDelete = 1;
      }
   }

   return( 0 );
} // TZXSLTDD_EnableMenuItems

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteAllMenus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteAllMenus( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vWindow;
   zSHORT nRC;
   zCHAR  szTag[ 33 ];
   zCHAR  szMsg[ 200 ];

   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "Window", "Tag" );

   strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete all Menus from Window '" );
   strcat_s( szMsg, zsizeof( szMsg ), szTag );
   strcat_s( szMsg, zsizeof( szMsg ), "'?" );
   nRC = MessagePrompt( vSubtask, "PE002",
                        "XSLT Maintenance",
                        szMsg,
                        zBEEP, zBUTTONS_YESNO,
                        zRESPONSE_NO,  0 );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstEntity( vXSLT, "Menu", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXSLT, "Menu", "" ) )
      {
         nRC = DeleteEntity( vXSLT, "Menu", zREPOS_NONE );
      }
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_MENU" ) >= 0 )
        RefreshWindow( vWindow );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
} // TZXSLTDD_DeleteAllMenus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteAllHotkeys
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteAllHotkeys( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zVIEW  vWindow;
   zSHORT nRC;
   zCHAR  szTag[ 33 ];
   zCHAR  szMsg[ 200 ];

   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTag, zsizeof( szTag ), vXSLT, "Window", "Tag" );

   strcpy( szMsg, "OK to delete all Shortcut Keys from Window '" );
   strcat( szMsg, szTag);
   strcat(szMsg, "'?" );

   if ( MessagePrompt( vSubtask, "PE002",
                       "XSLT Maintenance",
                       szMsg,
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 ) == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstEntity( vXSLT, "Hotkey", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXSLT, "Hotkey", "" ) )
      {
         DeleteEntity( vXSLT, "Hotkey", zREPOS_NONE );
      }
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_HK" ) >= 0 )
         RefreshWindow( vWindow );
   }
   return( 0 );
} // TZXSLTDD_DeleteAllHotkeys

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteAllActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteAllActions( zVIEW vSubtask )
{
   zVIEW  vWindow;

   zwTZPNCTAD_DeleteAllActions( vSubtask );

   // Refresh Action List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
} //TZXSLTDD_DeleteAllActions

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_NewXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_NewXSLT( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = AskForSave( vSubtask );
   if ( nRC )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // If XSLT not checked out and user change this XSLT, then
            // zeidon call the window "Save XSLT as".
            // We save the action after Save as in the View TZSAVEAS (e.g.
            // Create new XSLT or Open an other XSLT or Switch Project or
            // Exit XSLT Tool).
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNewComponentAfterSaveAs );
         }
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }
   }

   return( 0 );
} // TZXSLTDD_NewXSLT

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_OpenXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_OpenXSLT( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = AskForSave( vSubtask );
   if ( nRC )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if XSLT not checked out and user change this XSLT, then zeidon
            // call the window "Save XSLT as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new XSLT or Open an other XSLT or Switch Project or
            // Exit XSLT Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zOpenComponentAfterSaveAs );
         }
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                                     zNoActionAfterSaveAs );
      }
   }

   return( 0 );

} // TZXSLTDD_OpenXSLT

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_UpdateZeidonWindows
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_UpdateZeidonWindows( zVIEW vSubtask )
{
   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZXSLTDD_UpdateZeidonWindows

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadOperationList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_LoadOperationList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // TZXSLTDD_LoadOperationList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadSourceFileList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_LoadSourceFileList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // TZXSLTDD_LoadSourceFileList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadShortcutList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_LoadShortcutList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_HK" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // TZXSLTDD_LoadShortcutList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadMenuList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_LoadMenuList( zVIEW vSubtask )
{
   zVIEW  vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_MENU" ) >= 0 )
   {
      SetWindowState( vWindow, zWINDOW_STATUS_DESTROY, 1 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );
} // TZXSLTDD_LoadMenuList

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadWindowPopups
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZXSLTDD_LoadWindowPopups( zVIEW vSubtask,
                             zPCHAR szEntityName,
                             zPCHAR szActionNameForNew,
                             zPCHAR szActionNameForDelete,
                             zPCHAR szActionNameForDeleteAll,
                             zPCHAR szPopupName )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vXSLT;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META );

      if ( CheckExistenceOfEntity( vXSLT, szEntityName ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, szActionNameForNew, bNew );
      EnableAction( vSubtask, szActionNameForDelete, bDelete );
      EnableAction( vSubtask, szActionNameForDeleteAll, bDelete );

      CreateTrackingPopupMenu( vSubtask, szPopupName, pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // TZXSLTDD_LoadWindowPopups

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_MENU_LoadPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_MENU_LoadPopup( zVIEW vSubtask )
{
   TZXSLTDD_LoadWindowPopups( vSubtask,
                                "Menu",
                                "NewMenu",
                                "DeleteMenu",
                                "DeleteAllMenus",
                                "MenuPopup" );

   return( 0 );
} // UPD_MENU_LoadPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SEL_OPT_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SEL_OPT_Postbuild( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
   {
      SetCtrlState( vSubtask, "ViewName", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "AttributeList", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // SEL_OPT_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_LoadEventPopups
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
TZXSLTDD_LoadEventPopups( zVIEW  vSubtask,
                          zPCHAR szActionNameForUpdate,
                          zPCHAR szActionNameForNew,
                          zPCHAR szActionNameForSelect,
                          zPCHAR szActionNameForRemove,
                          zPCHAR szActionNameForEdit,
                          zPCHAR szPopupName )
{
   zVIEW  vEvents;
   zVIEW  vXSLT;
   zVIEW  vXSLTCopy;
   zVIEW  vTZXSLTL;
   zBOOL  bNew          = FALSE;
   zBOOL  bEdit         = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZXSLTL, zSOURCE_XSLT_META );

      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bUpdate = TRUE;
            if ( nIsCheckedOut == 1 )
               bDelete = TRUE;

            // Check Action Operation
            if ( zstrcmp( szPopupName, "CtrlListPopup" ) == 0 )
               CreateViewFromViewForTask( &vXSLTCopy, vTZXSLTL, 0 );
            else
               CreateViewFromViewForTask( &vXSLTCopy, vXSLT, 0 );

            SetCursorFirstEntityByAttr( vXSLTCopy, "Action", "ZKey",
                                        vEvents, "EventAct", "ZKey", "" );
            if ( CheckExistenceOfEntity( vXSLTCopy, "ActOper" ) >= zCURSOR_SET )
               bEdit = TRUE;
            DropView( vXSLTCopy );
         }
      }

      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           nIsCheckedOut == 1 )
      {
         bNew = TRUE;
      }

      EnableAction( vSubtask, szActionNameForUpdate, bUpdate );
      EnableAction( vSubtask, szActionNameForNew, bNew );
      EnableAction( vSubtask, szActionNameForSelect, bNew );
      EnableAction( vSubtask, szActionNameForRemove, bDelete );
      EnableAction( vSubtask, szActionNameForEdit, bEdit );

      CreateTrackingPopupMenu( vSubtask, szPopupName, -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // TZXSLTDD_LoadEventPopups

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LIST_HK_LoadPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LIST_HK_LoadPopup( zVIEW vSubtask )
{
   TZXSLTDD_LoadWindowPopups( vSubtask,
                                "Hotkey",
                                "NewHotkey",
                                "DeleteHotkey",
                                "DeleteAllShortcuts",
                                "ShortcutPopup" );

   return( 0 );
} // LIST_HK_LoadPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListLoadMappingPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListLoadMappingPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vXSLT;
   zVIEW  vMapping;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vMapping, "TZCTLMAP2", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META );

      if ( CheckExistenceOfEntity( vMapping, "CtrlMap" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "CtrlListAddMapEntity", bNew );
      EnableAction( vSubtask, "CtrlListDeleteMapEntity", bDelete );

      CreateTrackingPopupMenu( vSubtask, "MappingPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // CtrlListLoadMappingPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListLoadEventPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListLoadEventPopup( zVIEW vSubtask )
{
   TZXSLTDD_LoadEventPopups( vSubtask,
                               "CtrlListUpdateAction",
                               "ActionAdd",
                               "CtrlListSelectAction",
                               "CtrlListRemoveAction",
                               "WND_UPD_OperationEdit",
                               "CtrlListPopup" );

   return( 0 );
} // CtrlListLoadEventPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListSetCtrlFlags
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListSetCtrlFlags( zVIEW vSubtask )
{
   zVIEW vControl;
   zLONG lSubtype;

   if ( GetViewByName( &vControl, "NoRefresh", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
      CtrlListSetCheckedState( vSubtask, vControl, lSubtype );
   }

   return( 0 );
} // CtrlListSetCtrlFlags

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_MENU_LoadMenuPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_MENU_LoadMenuPopup( zVIEW vSubtask )
{
   zVIEW  vTZPNOPWO;
   zVIEW  vXSLT;
   zBOOL  bNew         = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZPNOPWO, "TZPNOPWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META );

      if ( CheckExistenceOfEntity( vTZPNOPWO, "Option" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
      {
         bNew = TRUE;
      }

      EnableAction( vSubtask, "OptionAdd", bNew );
      EnableAction( vSubtask, "OptionDelete", bDelete );

      CreateTrackingPopupMenu( vSubtask, "MenuPopup", -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // UPD_MENU_LoadMenuPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_ACT_AddAction
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ACT_AddAction( zVIEW vSubtask )
{
   LIST_ACT_AddAction( vSubtask );

   return( 0 );
} //TZXSLTDD_ACT_AddAction

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AddOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_AddOperation( zVIEW vSubtask )
{
   wTZOPRUSD_CheckExistsSourceFile( vSubtask );

   return( 0 );
} //TZXSLTDD_AddOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_AddSourceFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_AddSourceFile( zVIEW vSubtask )
{
   wTZOPRUSD_AddSourceFile( vSubtask );

   return( 0 );
} //TZXSLTDD_AddSourceFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_ACT_UpdateAction
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ACT_UpdateAction( zVIEW vSubtask )
{
   LIST_ACT_UpdateAction( vSubtask );

   return( 0 );
} //TZXSLTDD_ACT_UpdateAction

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_UpdateOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_UpdateOperation( zVIEW vSubtask )
{
   wTZOPRUSD_GoToUpdateOperation( vSubtask );

   return( 0 );
} //TZXSLTDD_UpdateOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_UpdateSourceFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_UpdateSourceFile( zVIEW vSubtask )
{
   zVIEW  vSOURCE;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vSOURCE, "SourceFile" );

   return( 0 );
} //TZXSLTDD_UpdateSourceFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteAction
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteAction( zVIEW vSubtask )
{
   zVIEW  vWindow;

   LIST_ACT_DeleteAction( vSubtask );

   // Refresh Action List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
} //TZXSLTDD_DeleteAction

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteOperation( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteSelectedOperat( vSubtask );

   return( 0 );
} //TZXSLTDD_DeleteOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_DeleteSourceFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_DeleteSourceFile( zVIEW vSubtask )
{
   wTZOPRUSD_DeleteSourceFile( vSubtask );

   return( 0 );
} //TZXSLTDD_DeleteSourceFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SaveAsPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW    vTZXSLTL;
   zVIEW    vSourceFile;
   zVIEW    vTZXSLTS;
   zVIEW    vSaveAs;
   zSHORT   nRC;

   RetrieveMetaList( vSubtask );

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZXSLTS, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vTZXSLTS, "W_MetaDef", 0 );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );
   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "DLL_Name", "" );

   //set current Description
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );
   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc",
                              vTZXSLTL, "XSLT", "Desc" );

   // delete old Source Files
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      nRC = DeleteEntity( vSaveAs, "SourceFile", zREPOS_NONE );
   }

   // create new source Files
   CreateViewFromViewForTask( &vSourceFile, vTZXSLTL, 0 );
   for ( nRC = SetCursorFirstEntity( vSourceFile, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSourceFile, "SourceFile", "" ) )
   {
      CreateEntity( vSaveAs, "SourceFile", zPOS_AFTER );
      SetAttributeFromAttribute(   vSaveAs, "SourceFile", "SourceName",
                                 vSourceFile, "SourceFile", "Name" );
      SetAttributeFromAttribute(   vSaveAs, "SourceFile", "LanguageType",
                                 vSourceFile, "SourceFile", "LanguageType" );
   }
   DropView( vSourceFile );

   if ( CheckExistenceOfEntity( vSaveAs, "SourceFile" ) >= zCURSOR_SET )
      OrderEntityForView( vSaveAs, "SourceFile", "LanguageType D SourceName A" );

   return( 0 );
} // TZXSLTDD_SaveAsPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SaveAsSetDefaults
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_SaveAsSetDefaults( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zVIEW  vTZXSLTS;
   zVIEW  vTZXSLTCopy;
   zCHAR  szNewName[ 33 ];
   zCHAR  szOutName[ 33 ];
   zSHORT nRC;
   zSHORT nIndex = -1;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vTZXSLTCopy, vTZXSLTS, 0 );

   // if XSLT Name not required, set default value
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   //Name is not required, create Source File Names
   if ( zstrcmp( szOutName, "" ) != 0 )
   {
      SetAttributeFromString(   vSaveAs, "ActionAfterSaveAS", "DLL_Name", szOutName );

      for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
      {
         // if XSLT in XSLTList, call TZXSLTDD_SetSaveAsName
         if ( SetCursorFirstEntityByString( vTZXSLTCopy, "W_MetaDef", "Name",
                                            szOutName, "" ) >= zCURSOR_SET )
         {
            TZXSLTDD_SetSaveAsName( vSubtask );
         }
         else
         {
            // if XSLT not in XSLTList, create all new Source
            // File Names
            nIndex = zwfnTZXSLTDD_SaveAsSetSourceName( vSubtask, vSaveAs,
                                                       0, nIndex );
         }
      }
   } //endif ( zstrcmp( szOutName, "" ) != 0 )

   DropView( vTZXSLTCopy );

   return( 0 );
} // TZXSLTDD_SaveAsSetDefaults

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsSetSourceName
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsSetSourceName( zVIEW   vSubtask,
                                  zVIEW   vSaveAs,
                                  zVIEW   vXSLTData,
                                  zSHORT  nIndex )
{
   zCHAR  szNewName[33];
   zCHAR  szIndex[ 4 ];
   zSHORT nPosition = 0;
   zSHORT nRC;

   szNewName[ 0 ] = 0;

   do
   {
      nIndex++;
      GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS",
                              "SaveAsName8" );
      if ( nIndex > 0 )
      {
         nPosition = (zSHORT) zstrlen( szNewName );
         if ( nPosition > 6 )
            nPosition = 6;

         zltoa( nIndex, szIndex, zsizeof( szIndex ), 10 );
         if ( nIndex < 10 )
         {
            strncpy_s( szNewName + nPosition, zsizeof( szNewName ) - nPosition, "_", 1 );
            strncpy_s( szNewName + 1 + nPosition, zsizeof( szNewName ) - 1 - nPosition, szIndex, 2 );
         }
         else
         {
            if ( nPosition == 6 )
               nPosition--;
            strncpy_s( szNewName + nPosition, zsizeof( szNewName ) - nPosition, "_", 1 );
            strncpy_s( szNewName + 1 + nPosition, zsizeof( szNewName ) - 1 - nPosition, szIndex, 3 );
         }
      }
      nRC = -1;
      if ( vXSLTData )
         nRC = SetCursorFirstEntityByString( vXSLTData, "SourceFile", "Name", szNewName, "" );

   } while ( nRC >= zCURSOR_SET );

   SetAttributeFromString( vSaveAs, "SourceFile", "SaveAsName", szNewName );

   return( nIndex );
} // zwfnTZXSLTDD_SaveAsSetSourceName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SetSaveAsName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW  vTZXSLTS;
   zVIEW  vTZXSLTL;
   zVIEW  vSaveAs;
   zVIEW  vXSLTData;
   zSHORT nRC;
   zSHORT nRC1 = -1;
   zSHORT nIndex = 0;

   GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "SaveAsName8",
                              vTZXSLTS, "W_MetaDef", "Name" );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vTZXSLTS, "W_MetaDef", "Desc" );
   }

   // set DLL Name and Source File Name
   if ( CompareAttributeToAttribute( vTZXSLTS, "W_MetaDef", "Name",
                                     vTZXSLTL, "XSLT", "Tag" ) == 0 )
   {
      CreateViewFromViewForTask( &vXSLTData, vTZXSLTL, 0 );
   }
   else
   {
      if ( ActivateMetaOI( vSubtask, &vXSLTData, vTZXSLTS, zSOURCE_XSLT_META,
                           zSINGLE | zLEVEL_TASK ) != 1 )
      {
         return( -1 );
      }
   }

   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "DLL_Name",
                              vXSLTData, "XSLT", "DLL_Name" );

   if ( CheckExistenceOfEntity( vXSLTData, "SourceFile" ) >= zCURSOR_SET )
      OrderEntityForView( vXSLTData, "SourceFile", "LanguageType D Name A" );
   else
      nIndex = -1;

   // set new source Files name
   nRC1 = SetCursorFirstEntity( vXSLTData, "SourceFile", "" );
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      if ( nRC1 == zCURSOR_SET &&
           CompareAttributeToAttribute( vSaveAs, "SourceFile", "LanguageType",
                          vXSLTData, "SourceFile", "LanguageType" ) == 0 )
      {
         SetAttributeFromAttribute( vSaveAs, "SourceFile", "SaveAsName",
                                    vXSLTData, "SourceFile", "Name" );
         nRC1 = SetCursorNextEntity( vXSLTData, "SourceFile", "" );
      }
      else
      {
         nIndex = zwfnTZXSLTDD_SaveAsSetSourceName( vSubtask, vSaveAs,
                                                    vXSLTData, nIndex );
      }
   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   DropMetaOI( vSubtask, vXSLTData );
   SetFocusToCtrl( vSubtask, "edXSLTName" );

   return( 0 );
} // TZXSLTDD_SetSaveAsName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SaveAsKeepCurrentDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vSaveAs;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vTZXSLTL, "XSLT", "Desc" );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   return( 0 );
} // TZXSLTDD_SaveAsKeepCurrentDesc

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_ResetView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ResetView( zVIEW vSubtask )
{
   zVIEW   vSaveAs;

   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ",
                               zNoActionAfterSaveAs );
      SetAttributeFromString( vSaveAs, "ActionAfterSaveAS",
                              "SaveAsName8", "" );
   }

   return( 0 );
} // TZXSLTDD_ResetView

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsCheckFileName
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsCheckFileName( zVIEW    vSubtask,
                                  zVIEW    vSaveAs )
{
   zVIEW    vSaveAsCopy;
   zSHORT   nRC;
   zCHAR    szNewName[33];
   zCHAR    szFileName[33];
   zCHAR    szMsg[ 100 ];

   CreateViewFromViewForTask( &vSaveAsCopy, vSaveAs, 0 );

   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      // Source File Name is required
      GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "SourceFile", "SaveAsName" );
      UfCompressName( szNewName, szFileName, 8, "", "", "", "", 0 );
      if ( zstrcmp( szFileName, "" ) == 0 )
      {
         MessageSend( vSubtask, "ZO00137", "XSLT Maintenance",
                      "Source File Name is required.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "ssSourceFile" );
         DropView( vSaveAsCopy );
         return( -2 );
      }

      // check duplicate Source File Names
      SetCursorFirstEntityByString( vSaveAsCopy, "SourceFile", "SaveAsName",
                                    szNewName, "" );
      if ( SetCursorNextEntityByString( vSaveAsCopy, "SourceFile", "SaveAsName",
                                        szNewName, "" ) >= zCURSOR_SET )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Source File Name '" );
         strcat_s( szMsg, zsizeof( szMsg ), szNewName );
         strcat_s( szMsg, zsizeof( szMsg ), "' is not unique." );
         MessageSend( vSubtask, "ZO00137", "XSLT Maintenance",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "ssSourceFile" );
         DropView( vSaveAsCopy );
         return( -2 );
      }

   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   DropView( vSaveAsCopy );

   return( 0 );
} //zwfnTZXSLTDD_SaveAsCheckFileName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsCheckName
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsCheckName( zVIEW    vSubtask,
                              zVIEW    vSaveAs,
                              zPCHAR   szOutName )
{
   zVIEW  vLOD_LPLR;
   zCHAR  szMsg[155];
   zCHAR  szDLLName[33];
   zCHAR  szNewName[33];

   // XSLT Name is required
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "XSLT Name is required." );
      MessageSend( vSubtask, "ZO00137", "XSLT Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edXSLTName" );
      return( -2 );
   }

   // DLL Name is required
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "DLL_Name" );
   UfCompressName( szNewName, szDLLName, 8, "", "", "", "", 0 );
   if ( zstrcmp( szDLLName, "" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "DLL Name is required." );
      MessageSend( vSubtask, "ZO00137", "XSLT Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edDLLName" );
      return( -2 );
   }

   // There must be no LOD with same name.
   RetrieveViewForMetaList( vSubtask, &vLOD_LPLR, zREFER_LOD_META );
   if ( SetCursorFirstEntityByString( vLOD_LPLR,
                                      "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "PN00221", "Zeidon Object Maintenance",
                   "A LOD with the same name exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edXSLTName" );
      return( -2 );
   }
   DropView( vLOD_LPLR );

   if ( zwfnTZXSLTDD_SaveAsCheckFileName( vSubtask, vSaveAs ) < 0 )
   {
      return( -2 );
   }

   return( 0 );
} // zwfnTZXSLTDD_SaveAsCheckName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsCheckStatus
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsCheckStatus( zVIEW    vSubtask,
                                zVIEW    vTZXSLTS,
                                zPCHAR   szOutName )
{
   zCHAR  szMsg[155];

   //check the check out state
   if ( CompareAttributeToInteger( vTZXSLTS, "W_MetaDef",
                                   "Status", 1 ) != 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "XSLT '" );
      strcat_s( szMsg, zsizeof( szMsg ), szOutName );
      strcat_s( szMsg, zsizeof( szMsg ), "' is not checked out." );
      MessageSend( vSubtask, "ZO00137", "XSLT Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edXSLTName" );
      return( -1 );
   }

   return( 0 );
} // zwfnTZXSLTDD_SaveAsCheckStatus

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsGetFileName
//
//    PARAMETER:
//    szSourceFileName : Size fo szSourceFileName has to be at least  zMAX_FILESPEC_LTH + 1
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsGetFileName( zVIEW  vTaskLPLR,
                                zVIEW  vView,
                                zPCHAR szAttribute,
                                zPCHAR szSourceFileName )
{
   zCHAR    szExtension[ zMAX_EXTENSION_LTH + 1 ];
   zCHAR    szNewName[ 33 ];
   zCHAR    szFileName[ 33 ];

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vView, "SourceFile", szAttribute );
   UfCompressName( szNewName, szFileName, 8, "", "", "", "", 0 );

   GetStringFromAttribute( szSourceFileName, zsizeof( szSourceFileName ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   GetStringFromAttributeByContext(   szExtension, vView, "SourceFile",
                                    "LanguageType", "LanguageType", zMAX_EXTENSION_LTH );

   ofnTZCMWKSO_AppendSlash( szSourceFileName );
   strcat_s( szSourceFileName, zsizeof( szSourceFileName ), szFileName );
   strcat_s( szSourceFileName, zsizeof( szSourceFileName ), "." );
   strcat_s( szSourceFileName, zsizeof( szSourceFileName ), szExtension );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsCopyFiles
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsCopyFiles( zVIEW    vSubtask,
                              zVIEW    vTZXSLTL,
                              zVIEW    vSaveAs )
{
   zVIEW    vTaskLPLR;
   zVIEW    vSourceFile;
   zLONG    hFile;
   zSHORT   nRC;
   zBOOL    bReadOnly = FALSE;
   zCHAR    szMsg[ zMAX_FILESPEC_LTH + zSHORT_MESSAGE_LTH + 1 ];
   zCHAR    szSourceFileName[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szNewFileName[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   strcpy_s( szSourceFileName, zsizeof( szSourceFileName ), "" );

   // Source File Name already exists
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      zwfnTZXSLTDD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SaveAsName",
                                      szNewFileName );
      SetAttributeFromInteger( vSaveAs, "SourceFile", "ReplaceFile", 1 );

      hFile = SysOpenFile( vSubtask, szNewFileName, COREFILE_EXIST );
      if ( hFile == 1 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Source File '" );
         strcat_s( szMsg, zsizeof( szMsg ), szNewFileName );
         strcat_s( szMsg, zsizeof( szMsg ), "' already exists. Replace existing File?" );
         nRC = MessagePrompt( vSubtask, "ZO00138", "XSLT Maintenance",
                              szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );

         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            SetFocusToCtrl( vSubtask, "ssSourceFile" );
            return( -1 );
         }
         else
         {
            if ( nRC == zRESPONSE_NO )
               SetAttributeFromInteger( vSaveAs, "SourceFile", "ReplaceFile", 0 );
         }
      }
   } // endfor ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );

   if ( MiGetUpdateForView( vTZXSLTL ) == 0 )
   {
      bReadOnly = TRUE;
      SetViewUpdate( vTZXSLTL );
   }

   // Set new Description and DLL Name
   SetAttributeFromAttribute( vTZXSLTL, "XSLT", "Desc",
                              vSaveAs, "ActionAfterSaveAS", "Desc" );
   SetAttributeFromAttribute( vTZXSLTL, "XSLT", "DLL_Name",
                              vSaveAs, "ActionAfterSaveAS", "DLL_Name" );

   // copy Source Files and set new Source File Name
   CreateViewFromViewForTask( &vSourceFile, vTZXSLTL, 0 );
   for ( nRC = SetCursorFirstEntity( vSaveAs, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vSaveAs, "SourceFile", "" ) )
   {
      if ( CompareAttributeToInteger( vSaveAs, "SourceFile", "ReplaceFile", 1 ) == 0 )
      {
         zwfnTZXSLTDD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SourceName",
                                         szSourceFileName );
         zwfnTZXSLTDD_SaveAsGetFileName( vTaskLPLR, vSaveAs, "SaveAsName",
                                         szNewFileName );
         SysCopyFile( vSubtask, szSourceFileName, szNewFileName, TRUE );
      }

      // set new Source FIle Name
      if ( SetCursorFirstEntityByAttr( vSourceFile, "SourceFile", "Name",
                                       vSaveAs, "SourceFile", "SourceName",
                                       "" ) == zCURSOR_SET )
      {
         SetAttributeFromAttribute( vSourceFile, "SourceFile", "Name",
                                    vSaveAs, "SourceFile", "SaveAsName" );
      }
   }

   DropView( vSourceFile );

   if ( bReadOnly == TRUE )
      SetViewReadOnly( vTZXSLTL );

   return( 0 );
} // zwfnTZXSLTDD_SaveAsCopyFiles

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SaveAsXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SaveAsXSLT( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZXSLTS;
   zVIEW  vTZXSLTL;
   zVIEW  vSaveAs;
   zCHAR  szNewName[ 33 ];
   zCHAR  szOutName[ 33 ];
   zCHAR  szMsg[ 255 ];

   GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // Validate XSLT Name is OK
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   //Name is required
   if ( zwfnTZXSLTDD_SaveAsCheckName( vSubtask, vSaveAs, szOutName ) < 0 )
      return( -1 );

   // Name already exists
   if ( SetCursorFirstEntityByString( vTZXSLTS, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "XSLT already exists. Replace existing XSLT?" );
      nRC = MessagePrompt( vSubtask, "ZO00138", "XSLT Maintenance",
                           szMsg, zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edXSLTName" );
         return( -1 );
      }

      // check the check out state
      if ( zwfnTZXSLTDD_SaveAsCheckStatus( vSubtask, vTZXSLTS, szOutName ) < 0 )
         return( -1 );

      // if this XSLT open, then save XSLT, not save as
      if ( CompareAttributeToString( vTZXSLTL, "XSLT", "Tag", szOutName ) == 0 )
      {
         if ( zwfnTZXSLTDD_SaveAsCopyFiles( vSubtask, vTZXSLTL, vSaveAs ) < 0 )
            return( -1 );

         SetAttributeFromString( vTZXSLTL, "XSLT", "Tag", szOutName );
         SaveXSLTFile( vSubtask );
         return( 0 );
      }
      else
      {
         // if another XSLT open, then replace existing XSLT
         nRC = DeleteMetaOI( vSubtask, vTZXSLTS, zSOURCE_XSLT_META );
      }
   }

   if ( zwfnTZXSLTDD_SaveAsCopyFiles( vSubtask, vTZXSLTL, vSaveAs ) < 0 )
      return( -1 );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", szOutName );
   TZXSLTDD_SaveAs( vSubtask, szOutName );

   zwfnTZXSLTDD_CheckActionAfterSaveAs( vSubtask );

   return( 0 );

} // TZXSLTDD_SaveAsXSLT

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwfnTZXSLTDD_SaveAsSourceFiles
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
zwfnTZXSLTDD_SaveAsSourceFiles( zVIEW   vSubtask,
                                zVIEW   vNewXSLT,
                                zVIEW   vOldXSLT )
{
   zSHORT  nRC;

   // Copy each SourceFile subobject, with Operation information.
   nRC = SetCursorFirstEntity( vOldXSLT, "SourceFile", "" );

   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, vNewXSLT, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vNewXSLT, "SourceFile",
                                   vOldXSLT, "SourceFile", zSET_NULL );
      nRC = SetCursorFirstEntity( vOldXSLT, "Operation", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         CreateMetaEntity( vSubtask, vNewXSLT, "Operation", zPOS_AFTER );
         SetMatchingAttributesByName( vNewXSLT, "Operation",
                                      vOldXSLT, "Operation", zSET_NULL );
         nRC = SetCursorFirstEntity( vOldXSLT, "Parameter", "" );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            // Abort with error message if the Parameter.ShortDesc attribute is
            // null, since ShortDesc is a required attribute and we don't want
            // to continue to migrate a XSLT with an error.
            if ( CompareAttributeToString( vOldXSLT, "Parameter", "ShortDesc", "" ) == 0 )
            {
               SetAttributeFromString( vOldXSLT, "Parameter", "ShortDesc", "Subtask" );
            }

            CreateMetaEntity( vSubtask, vNewXSLT, "Parameter", zPOS_AFTER );
            SetMatchingAttributesByName( vNewXSLT, "Parameter",
                                         vOldXSLT, "Parameter", zSET_NULL );

            nRC = SetCursorNextEntity( vOldXSLT, "Parameter", "" );
         }
         nRC = SetCursorNextEntity( vOldXSLT, "Operation", "" );
      }
      nRC = SetCursorNextEntity( vOldXSLT, "SourceFile", "" );
   }

   return( 0 );
} // zwfnTZXSLTDD_SaveAsSourceFiles

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_SaveAs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_SaveAs( zVIEW   vSubtask,
                 zPCHAR  szXSLTName)
{
   zVIEW     vLPLR;
   zVIEW     vLPLR2;
   zVIEW     vOldXSLT;
   zVIEW     vNewXSLT;
   zVIEW     vTZXSLTS;
   zSHORT    nRC;
   zCHAR     szViewObjRefName[ 33 ];
   zCHAR     szDfltWnd[ 33 ];
   zCHAR     szOldXSLTName[ 33 ];

   GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOldXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
   SetNameForView( vOldXSLT, "vOldXSLT", vSubtask, zLEVEL_TASK );

   ActivateEmptyMetaOI( vSubtask, &vNewXSLT, zSOURCE_XSLT_META, zSINGLE );
   SetNameForView( vNewXSLT, "vNewXSLT", vSubtask, zLEVEL_TASK );

   //Create Root-Entity
   CreateMetaEntity( vSubtask, vNewXSLT, "XSLT", zPOS_AFTER );
   SetAttributeFromString( vNewXSLT, "XSLT", "Tag", szXSLTName );
   SetAttributeFromAttribute( vNewXSLT, "XSLT", "LastSyncDate",
                              vOldXSLT, "XSLT", "LastSyncDate" );
   SetAttributeFromAttribute( vNewXSLT, "XSLT", "Desc",
                              vOldXSLT, "XSLT", "Desc" );
   SetAttributeFromAttribute( vNewXSLT, "XSLT", "DLL_Name",
                              vOldXSLT, "XSLT", "DLL_Name" );

   // Copy each Registered View Name that is used by the XSLT.
   CreateViewFromViewForTask( &vLPLR2, vLPLR, 0 );
   nRC = SetCursorFirstEntity( vOldXSLT, "ViewObjRef", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetStringFromAttribute( szViewObjRefName, zsizeof( szViewObjRefName ), vOldXSLT, "ViewObjRef", "Name" );
      oTZWDLGSO_PositionOnVOR( vNewXSLT, vOldXSLT, vLPLR2,
                               szViewObjRefName, vSubtask );
      nRC = SetCursorNextEntity( vOldXSLT, "ViewObjRef", "" );
   }

   // Copy each SourceFile subobject, with Operation information.
   zwfnTZXSLTDD_SaveAsSourceFiles( vSubtask, vNewXSLT, vOldXSLT );

   // Copy Windows
   GetStringFromAttribute( szOldXSLTName, zsizeof( szOldXSLTName ), vOldXSLT, "XSLT", "Tag" );
   nRC = SetCursorFirstEntity( vOldXSLT, "Window", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( oTZWDLGSO_CloneWindow( vNewXSLT, vOldXSLT, vLPLR, vSubtask ) == -1 )
         return( -1 );

      // Set new XSLT Name in Action
      for ( nRC = SetCursorFirstEntityByString( vNewXSLT, "Action", "XSLTName",
                                                szOldXSLTName, "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByString( vNewXSLT, "Action", "XSLTName",
                                               szOldXSLTName, "" ) )
      {
         SetAttributeFromString( vNewXSLT, "Action", "XSLTName", szXSLTName );
      }


      nRC = SetCursorNextEntity( vOldXSLT, "Window", "" );
   }

   // Now include the Primary Window.
   if ( SetCursorFirstEntity( vOldXSLT, "DfltWnd", 0 ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szDfltWnd, zsizeof( szDfltWnd ), vOldXSLT, "DfltWnd", "Tag" );
      SetCursorFirstEntityByString( vNewXSLT, "Window", "Tag", szDfltWnd, "" );
      IncludeSubobjectFromSubobject( vNewXSLT, "DfltWnd",
                                     vNewXSLT, "Window", zPOS_AFTER );
   }

   SetCursorFirstEntity( vNewXSLT, "XSLT", "" );
   SetNameForView( vNewXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );

   SaveXSLTFile( vSubtask );

   GetViewByName( &vTZXSLTS, "TZXSLTS", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( vTZXSLTS, "W_MetaDef", "CPLR_ZKey",
                               vNewXSLT, "XSLT", "ZKey", "" );

   OpenXSLT_File( vSubtask, 0 );
   DropObjectInstance( vOldXSLT );

   return( 0 );
} // TZXSLTDD_SaveAs

/////////////////////////////////////////////////////////////////////////////
//
// One of vTgt and vTgtCtrl must be non-null (both may be non-null).
// If vTgt is null, use vTgtCtrl as the Top level view.  Otherwise if
// vTgtCtrl is not null, use vTgt as the Top level view and copy all
// of the source controls as children of the control pointed to by vTgtCtrl.
//
// In source ctrls, the first bit of the OptionFlags attribute to specify:
//    0 - parent control is required
//    1 - parent control is NOT required.
//
// N.B.  This operation is called dynamically from tzctlptx.cpp fnMergeCtrls!
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
MergeCtrls( zVIEW  vSubtask,
            zVIEW  vTgt,
            zVIEW  vSrc,
            zVIEW  vTgtCtrl,
            zVIEW  vSrcLPLR,
            zVIEW  vPE,
            zBOOL  bXSLT )
{
   zVIEW  vTgtC;
   zVIEW  vSrcC;
   zLONG  lTypeTgt;
   zLONG  lTypeSrc;
   zLONG  lZKey = 0;
   zSHORT nRC;

   CreateViewFromViewForTask( &vSrcC, vSrc, 0 );
   if ( vTgt == 0 )
      vTgt = vTgtCtrl;

   if ( vTgtCtrl )
   {
      zLONG  lCtrlCnt;
      zBOOL  bMatch = FALSE;

      CreateViewFromViewForTask( &vTgtC, vTgtCtrl, 0 );

      // First try to match on type of ctrl where the ctrl count is 1 in
      // in both the source and target ctrls.
      lCtrlCnt = CountEntitiesForView( vSrcC, "Control" );
      GetIntegerFromAttribute( &lTypeTgt, vTgtC, "ControlDef", "Key" );
      while ( bMatch == FALSE && lCtrlCnt == 1 )
      {
         GetIntegerFromAttribute( &lTypeSrc, vSrcC, "ControlDef", "Key" );
         if ( lTypeSrc == lTypeTgt )
            bMatch = TRUE;
         else
         {
            SetViewToSubobject( vSrcC, "CtrlCtrl" );
            lCtrlCnt = CountEntitiesForView( vSrcC, "Control" );
         }
      }

      if ( bMatch == FALSE )
      {
         while ( ResetViewFromSubobject( vSrcC ) == 0 )
         {
         }
      }

      // Now go as deep as possible in the subobjects matching on type of ctrl
      // where the ctrl count is 1 in in both the source and target ctrls.
      while ( CountEntitiesForView( vTgtC, "CtrlCtrl" ) == 1 &&
              CountEntitiesForView( vSrcC, "CtrlCtrl" ) == 1 )
      {
         SetViewToSubobject( vTgtC, "CtrlCtrl" );
         SetViewToSubobject( vSrcC, "CtrlCtrl" );
         GetIntegerFromAttribute( &lTypeTgt, vTgtC, "ControlDef", "Key" );
         GetIntegerFromAttribute( &lTypeSrc, vSrcC, "ControlDef", "Key" );
         if ( lTypeSrc != lTypeTgt )
         {
            ResetViewFromSubobject( vTgtC );
            ResetViewFromSubobject( vSrcC );
            break;
         }
      }

      GetIntegerFromAttribute( &lTypeTgt, vTgtC, "ControlDef", "Key" );
      GetIntegerFromAttribute( &lTypeSrc, vSrcC, "ControlDef", "Key" );
      if ( lTypeSrc == lTypeTgt )
      {
         zLONG lFlags;

         GetIntegerFromAttribute( &lFlags, vSrcC, "Control", "OptionFlags" );
         if ( (lFlags & 0x00000001) == 0x00000001 ) // parent not required
            SetViewToSubobject( vTgtC, "CtrlCtrl" );
      }
      else
      if ( CheckExistenceOfEntity( vTgtC, "Control" ) == 0 )
         SetViewToSubobject( vTgtC, "CtrlCtrl" );
   }
   else
   {
      CreateViewFromViewForTask( &vTgtC, vTgt, 0 );
   }

   GetIntegerFromAttribute( &lTypeTgt, vTgtC, "ControlDef", "Key" );
   GetIntegerFromAttribute( &lTypeSrc, vSrcC, "ControlDef", "Key" );
   while ( CountEntitiesForView( vSrcC, "Control" ) == 1 )
   {
      zLONG lFlags;

      GetIntegerFromAttribute( &lFlags, vSrcC, "Control", "OptionFlags" );
      if ( (lFlags & 0x00000001) == 0x00000001 ) // parent not required
         SetViewToSubobject( vSrcC, "CtrlCtrl" );
      else
         break;
   }

   GetIntegerFromAttribute( &lTypeTgt, vTgtC, "ControlDef", "Key" );
   GetIntegerFromAttribute( &lTypeSrc, vSrcC, "ControlDef", "Key" );
   if ( bXSLT == FALSE )
   {
      nRC = SetCursorFirstEntity( vSrc, "Action", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( fnCloneAction( vSrcLPLR, vSrc, vTgt, vSubtask ) == -1 )
         {
            DropView( vTgtC );
            DropView( vSrcC );
            return( -1 );
         }

         nRC = SetCursorNextEntity( vSrc, "Action", 0 );
      }
   }

// SetNameForView( vSrcC, "CloneViewSrcC", vSubtask, zLEVEL_TASK );
   nRC = SetCursorFirstEntity( vSrcC, "Control", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // We will try to prevent recursive copies by holding onto the
      // ZKey of the current source control.  If we are going to copy
      // that control again, we will break.
      GetIntegerFromAttribute( &lZKey, vSrcC, "Control", "ZKey" );
      if ( fnCloneControl( vSrcLPLR, vSrc, vSrcC, vTgt, vTgtC, vPE,
                           bXSLT, vSubtask ) == -1 )
      {
         DropView( vTgtC );
         DropView( vSrcC );
         return( -1 );
      }

   // MessageBox( 0, "BeforeNextEntity", "MergeCtrls", MB_OK );
/*
BL 2000.09.04:
This code was commented out, because cut and paste with more than one
control did not work. The Compare is always TRUE, since vSrcC does
not change position and thus lZKey is compared to itself.
DKS: this code was introduced to avoid a loop. As there were changes in
fnCloneControl, this check is not necessary any more.
      if ( bXSLT == FALSE &&
           CompareAttributeToInteger( vSrcC, "Control", "ZKey", lZKey ) == 0 )
      {
         break;
      }
*/
      nRC = SetCursorNextEntity( vSrcC, "Control", 0 );
   // MessageBox( 0, "AfterNextEntity", "MergeCtrls", MB_OK );
      if ( CompareAttributeToInteger( vSrcC, "Control", "ZKey", lZKey ) == 0 )
      {
         break;
      }
   }

   DropView( vTgtC );
   DropView( vSrcC );
   return( 0 );
}

#if 0
// Recursive routine to clone options within a Menu.
zSHORT
fnCloneOptions( zVIEW  vTgt,
                zVIEW  vSrc,
                zVIEW  vTgtO,
                zVIEW  vSrcO,
                zVIEW  vSrcLPLR,
                zVIEW  vPE,
                zVIEW  vSubtask )
{
   zSHORT nRC;


   nRC = SetCursorFirstEntity( vSrcO, "Option", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateMetaEntity( vSubtask, vTgtO, "Option", zPOS_AFTER );
      SetMatchingAttributesByName( vTgtO, "Option",
                                   vSrcO, "Option", zSET_NULL );

      if ( CheckExistenceOfEntity( vSrcO, "OptAct" ) > zCURSOR_UNCHANGED )
      {
         GetAddrForAttribute( &pch, vSrcO, "OptAct", "Tag" );
         if ( pch[ 0 ] )
         {
            if ( SetCursorFirstEntityByString( vTgt, "Act", "Tag",
                                               pch, 0 ) == zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vTgtO, "OptAct",
                                              vTgt, "Act",
                                              zPOS_AFTER );
            }
            else
            {
               MessageSend( vSubtask, "CM00807", "Configuration Management",
                            "Opt cannot locate action for include",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
               TraceLineS( "(tzxsltdd) Option cannot locate action "
                              "for include ", pch );
            }
         }
      }

      if ( CheckExistenceOfEntity( vSrcO, "OptMap" ) > zCURSOR_UNCHANGED )
      {
         GetAddrForAttribute( &pch, vSrcO, "OptMap", "Tag" );
         CreateEntity( vTgtO, "OptMap", zPOS_AFTER );

         // Now reset the target values from the relationship values.
         if ( CheckExistenceOfEntity( vSrcO, "OptMapView" ) == 0 )
            SetAttributeFromAttribute( vTgtO, "OptMap", "VN",
                                       vSrcO, "OptMapView",
                                       "Name" );

         if ( CheckExistenceOfEntity( vSrcO, "OptMapLOD_Attribute" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtO, "OptMap", "EN",
                                       vSrcO, "OptMapRelatedEntity",
                                       "Name" );
            SetAttributeFromAttribute( vTgtO, "OptMap", "AN",
                                       vSrcO, "OptMapER_Attribute",
                                       "Name" );
         }

         if ( CheckExistenceOfEntity( vSrcO, "OptMapContext" ) == 0 )
         {
            SetAttributeFromAttribute( vTgtO, "OptMap", "Context",
                                       vSrcO, "OptMapContext", "Name" );
         }

         SetAttributeFromAttribute( vTgtO, "Opt", "ChkOn", vSrcO, "Option", "CheckMarkValueOn" );
         SetAttributeFromAttribute( vTgtO, "Opt", "ChkOff", vSrcO, "Option", "CheckMarkValueOff" );
      }

      nRC = SetCursorFirstEntity( vSrcO, "OptOpt", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         SetViewToSubobject( vSrcO, "OptOpt" );
         SetViewToSubobject( vTgtO, "OptOpt" );
         nRC = fnCloneOptions( vTgt, vSrc, vTgtO, vSrcO, vSrcLPLR, vPE, vSubtask );
         ResetViewFromSubobject( vSrcO );
         ResetViewFromSubobject( vTgtO );
         if ( nRC == -1 )
            return( -1 );

         nRC = SetCursorNextEntity( vSrcO, "OptOpt", 0 );
      }


      if ( CheckExistenceOfEntity(
         if ( CompareAttributeToString( vSrcC, "EventAct", "Tag", "" ) != 0 )
         {
            CreateMetaEntity( vSubtask, vTgtC, "Event", zPOS_AFTER );
            SetMatchingAttributesByName( vTgtC, "Event",
                                         vSrcC, "Event", zSET_NULL );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcC, "EventAct", "Tag" );
            nRC = SetCursorFirstEntityByString( vTgt, "Action", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtC, "EventAct", vTgt, "Action", zPOS_AFTER );
            }
         }

      nRC = SetCursorNextEntity( vSrcO, "Option", 0 );
   }


   nRC = SetCursorFirstEntity( vSrcO, "CtrlMap", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneCtrlMap( vTgt, vTgtO, vSrc, vSrcO, vSrcLPLR, vSubtask ) == -1 )
         return( -1 );

      nRC = SetCursorNextEntity( vSrcO, "CtrlMap", 0 );
   }

   if ( bXSLT == FALSE )
   {
      nRC = SetCursorFirstEntity( vSrcO, "Event", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( CompareAttributeToString( vSrcO, "EventAct", "Tag", "" ) != 0 )
         {
            CreateMetaEntity( vSubtask, vTgtO, "Event", zPOS_AFTER );
            SetMatchingAttributesByName( vTgtO, "Event", vSrcO, "Event", zSET_NULL );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcO, "EventAct", "Tag" );
            nRC = SetCursorFirstEntityByString( vTgt, "Action", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtO, "EventAct", vTgt, "Action", zPOS_AFTER );
            }
         }

         nRC = SetCursorNextEntity( vSrcO, "Event", 0 );
      }
   }


   nRC = SetCursorFirstEntity( vSrcO, "CtrlMap", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneCtrlMap( vTgt, vTgtO, vSrc, vSrcO, vSrcLPLR, vSubtask ) == -1 )
         return( -1 );

      nRC = SetCursorNextEntity( vSrcO, "CtrlMap", 0 );
   }

   if ( bXSLT == FALSE )
   {
      nRC = SetCursorFirstEntity( vSrcO, "Event", 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( CompareAttributeToString( vSrcO, "EventAct", "Tag", "" ) != 0 )
         {
            CreateMetaEntity( vSubtask, vTgtO, "Event", zPOS_AFTER );
            SetMatchingAttributesByName( vTgtO, "Event", vSrcO, "Event", zSET_NULL );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcO, "EventAct", "Tag" );
            nRC = SetCursorFirstEntityByString( vTgt, "Action", "Tag", szTag, 0 );
            if ( nRC >= 0 )
            {
               IncludeSubobjectFromSubobject( vTgtO, "EventAct", vTgt, "Action", zPOS_AFTER );
            }
         }

         nRC = SetCursorNextEntity( vSrcO, "Event", 0 );
      }
   }

   return( 0 );

   nRC = SetCursorFirstEntity( vSrcO, "Menu", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneMenu( vSrcLPLR, vSrcO, vTgtO, vPE,
                        bXSLT, vSubtask ) == -1 )
      {
         DropView( vTgt< );
         DropView( vSrcO );
         return( -1 );
      }

      nRC = SetCursorNextEntity( vSrcO, "Menu", 0 );
   }

   DropView( vTgtO );
   DropView( vSrcO );
   return( 0 );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// Use vTgt as the Top level view and copy the menu, options and related
// actions from the source window.
//
// N.B.  This operation is called dynamically from tzctlptx.cpp fnMergeMenu!
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
MergeMenu( zVIEW  vSubtask,
           zVIEW  vTgt,
           zVIEW  vSrc,
           zVIEW  vSrcLPLR,
           zVIEW  vPE )
{
   zVIEW  vTgtO;
   zVIEW  vSrcO;
   zCHAR  szMsg[ 65 ];
   zCHAR  szTag[ 33 ];
   zLONG  lUniqueId;
   zSHORT nLth;
   zSHORT nRC;

   CreateViewFromViewForTask( &vSrcO, vSrc, 0 );
   CreateViewFromViewForTask( &vTgtO, vTgt, 0 );

   while ( ResetViewFromSubobject( vSrcO ) == 0 )
   {
   }

   while ( ResetViewFromSubobject( vTgtO ) == 0 )
   {
   }

   nRC = SetCursorFirstEntity( vSrcO, "Action", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( fnCloneAction( vSrcLPLR, vSrcO, vTgtO, vSubtask ) == -1 )
      {
         DropView( vTgtO );
         DropView( vSrcO );
         return( -1 );
      }

      nRC = SetCursorNextEntity( vSrcO, "Action", 0 );
   }

   CreateMetaEntity( vSubtask, vTgtO, "Menu", zPOS_AFTER );
   SetMatchingAttributesByName( vTgtO, "Menu",
                                vSrcO, "Menu", zSET_NULL );

   // Force unique MenuTag here.
   // Reset the current menu's tag so we won't find it by Tag while we
   // ensure that the tag is unique.
   GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcO, "Menu", "Tag" );
   strcpy_s( szMsg, zsizeof( szMsg ), szTag ); // hold onto original tag
   SetAttributeFromString( vTgtO, "Menu", "Tag",
                           "__&&&__^^^__Clone__@@@__%%%__" );

   SetNameForView( vTgt, "CloneViewTgtO", vSubtask, zLEVEL_TASK );

   nLth = (zSHORT) zstrlen( szTag );
   while ( nLth && szTag[ nLth - 1 ] >= '0' && szTag[ nLth - 1 ] <= '9' )
      nLth--;

   lUniqueId = zatol( szTag + nLth );
   if ( nLth == 0 )
   {
      strcpy_s( szTag, zsizeof( szTag ), "Tag" );
      nLth = 3;
   }

   while ( SetCursorFirstEntityByString( vTgt, "Menu", "Tag",
                                         szTag, 0 ) == 0 )
   {
      lUniqueId++;
      zltoa( lUniqueId, szTag + nLth, zsizeof( szTag ) - nLth, 10 );
   }

// MessageBox( 0, "Text", "Title", MB_OK );
   SetAttributeFromString( vTgtO, "Menu", "Tag", szTag );

// fnCloneOptions( vTgt, vSrc, vTgtO, vSrcO, vSrcLPLR, vPE, vSubtask );
   nRC = SetCursorFirstEntity( vSrcO, "Option", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      oTZXSLTDD_CloneOption( vTgtO, vTgt, vSrcO, vSrc, zPOS_AFTER, vSubtask );
      nRC = SetCursorNextEntity( vSrcO, "Option", 0 );
   }

   DropView( vTgtO );
   DropView( vSrcO );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
GroupCtrlsMigration9J( zVIEW vSubtask )
{
   zVIEW  vXSLT;
   zSHORT nRC;

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   fnPainterCall( zMSG_UPDATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      fnRecurseGroupCtrls( vXSLT );
      nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
   }

   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnChangeMappingViewR( zVIEW vXSLT,
                      zVIEW vOldViewName,
                      zVIEW vNewViewName,
                      zBOOL bAllCtrlsAtLevel,
                      zBOOL bRecurse );

zBOOL g_bSkipPrompt = FALSE;
zLONG g_lFromWAB;
zLONG g_lToWAB;
zLONG g_lFromHeight;
zLONG g_lToHeight;
zCHAR g_szDlgTag[ 33 ];
zCHAR g_szGroupBox[ 33 ];
zCHAR g_szChangeFrom[ 4096 ];
zCHAR g_szChangeTo[ 4096 ];

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateAllXLST( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateAllXSLT( zVIEW vSubtask );

#define zLISTBOX_COLUMNHEADERS         zCONTROL_SPECIFIC_3
#define zLISTBOX_SORTEDHEADERS         zCONTROL_SPECIFIC_2

#define zCOMBOBOX_NO_NULL             0x00000200L

#define zCHECK_OK                      0x00000001
#define zCHECK_EVENTS                  0x00000002
#define zCHECK_EVENTS_WEBJAVASCRIPT    0x00000004
//#define zCHECK_READONLY_LISTBOX        0x00000004
#define zCHECK_READONLY_EDITBOX        0x00000008
#define zCHECK_READONLY_ALL_CTRLS      0x00000020
#define zCHECK_CTRL_TYPE               0x00000010
#define zCHECK_DATETIMEDOMAIN_EDITBOX  0x00000040
#define zCHECK_DATEDOMAIN_EDITBOX      0x00000080
#define zCHECK_CALENDAR_TO_EDITBOX     0x00000100
#define zCHECK_ACTIVEX_PROPERTIES      0x00000200
//#define zCHECK_CHANGETAB_EVENT         0x00000400
//#define zCHECK_CHANGE_BREADCRUMB       0x00000400
//#define zCHECK_CHANGE_MOVEINCREMENT    0x00000400
//#define zCHECK_CHANGE_NOTETOREVIEWER   0x00000400
#define zCHECK_CHANGE_GROUPBOX_NEW     0x00000400
#define zCHECK_POSTBUILD_SETUP         0x00000800
#define zCHECK_INCREMENT_POS_SIZE      0x00010000
#define zCHECK_INCREMENT_POS_SIZEX     0x00011000
#define zCHECK_INCREMENT_POS_SIZEY     0x00012000
#define zCHECK_INCREMENT_POS_SIZEXY    0x00013000
#define zCHECK_DECREMENT_POS_SIZE      0x00020000
#define zCHECK_DECREMENT_POS_SIZEX     0x00021000
#define zCHECK_DECREMENT_POS_SIZEY     0x00022000
#define zCHECK_DECREMENT_POS_SIZEXY    0x00023000
#define zCHECK_CHANGE_EVENTS           0x00100000
#define zCHECK_COMBOBOX_NO_NULL        0x00200000
#define zCHECK_REPORT_EVENTS           0x00400000
#define zCHECK_REPORT_TAB_PAGES        0x00800000
#define zCHECK_REPORT_WEBMENUS         0x01000000
#define zCHECK_ADD_GROUPBOX            0x02000000
#define zCHECK_CHANGE_MAPPING          0x04000000
#define zCHECK_CHANGE_WAB              0x08000000
#define zCHECK_TIMESTAMP_ONLY          0x10000000
#define zCHECK_RESIZE_EDIT_CTRLS       0x20000000
#define zCHECK_SAVE_ALL_JSP            0x40000000
#define zCHECK_SAVE_ALL                0x80000000

zBOOL
CheckMigrateCtrls( zVIEW vXSLT,
                   zVIEW vPE,
                   zLONG lCheckFlags,
                   zCPCHAR cpcDlgTag,
                   zCPCHAR cpcWndTag,
                   zLONG lData )
{
   zCHAR  szMsg[ 256 ];
   zCHAR  szDlgTag[ 34 ];
   zCHAR  szWndTag[ 34 ];
   zCHAR  szCtrlTag[ 34 ];
   zPCHAR pch;
   zBOOL  bChange = FALSE;
   zLONG  lSubtype;
   zLONG  lKey;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zLONG  lIncrX;
   zLONG  lIncrY;
   zULONG ulLth;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vXSLT, "Control", "Tag" );

      if ( lCheckFlags & zCHECK_READONLY_ALL_CTRLS )
      {
         GetIntegerFromAttribute( &lSubtype, vXSLT, "Control", "ExtendedStyle" );

         if ( (lSubtype & zCONTROLX_DISABLE_READONLY) == 0 )
         {
            bChange = TRUE;
            lSubtype |= zCONTROLX_DISABLE_READONLY;
            SetAttributeFromInteger( vXSLT, "Control", "ExtendedStyle", lSubtype );
         }
      }
      else
      if ( lCheckFlags & (zCHECK_REPORT_EVENTS | zCHECK_CHANGE_WAB) )
      {
         zVIEW  vAct;
         zPCHAR pchAction;
         zPCHAR pchOperation;
         zLONG  lType;
         zLONG  lActType;

         CreateViewFromViewForTask( &vAct, vXSLT, 0 );
         while ( ResetViewFromSubobject( vAct ) == 0 )
         {
         }

         zSHORT nRC = SetCursorFirstEntity( vXSLT, "Event", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            szDlgTag[ 0 ] = 0;
            szWndTag[ 0 ] = 0;
            pchOperation = "";
            if ( SetEntityCursor( vAct, "Action", 0,
                                  zPOS_FIRST | zQUAL_ENTITYCSR,
                                  vXSLT, "EventAct",
                                  0, 0, 0, 0 ) >= zCURSOR_SET )
            {
               GetAddrForAttribute( &pchAction, vAct, "Action", "Tag" );
               GetIntegerFromAttribute( &lActType, vAct, "Action", "Type" );
               GetStringFromAttribute( szDlgTag, zsizeof( szDlgTag ), vAct, "Action", "XSLTName" );
               GetStringFromAttribute( szWndTag, zsizeof( szWndTag ), vAct, "Action", "WindowName" );
               if ( CheckExistenceOfEntity( vAct, "ActOper" ) == 0 )
                  GetAddrForAttribute( &pchOperation, vAct, "ActOper", "Name" );
            }
            else
            {
               lActType = 0;
               pchAction = "";
            }

            GetIntegerFromAttribute( &lType, vXSLT, "Event", "Type" );
            TraceLine( "Event Type: 0x%08x  Dlg.Wnd.Ctrl: %s.%s.%s", lType, cpcDlgTag, cpcWndTag, szCtrlTag );
            TraceLine( "                          Action: %s", pchAction );
            TraceLine( "                             WAB: %s", GetActionString( lActType ) );
            TraceLine( "                             Oper: %s", pchOperation );
            TraceLine( "                             ====> %s.%s", szDlgTag, szWndTag );
         // sprintf_s( szMsg, zsizeof( szMsg ), "Dlg.Wnd.Ctrl: %s.%s.%s Action: %s WAB: %s (%s.%s) Operation: %s",
         //           cpcDlgTag, cpcWndTag, szCtrlTag, pchAction,
         //           GetActionString( lActType ), szDlgTag, szWndTag, pchOperation );
         // TraceLine( "Event Type: 0x%08x  %s", lType, szMsg );
            if ( (lCheckFlags & zCHECK_CHANGE_WAB) && lActType == g_lFromWAB )
            {
               if ( OperatorPrompt( vXSLT, "Convert selected WAB", szMsg, 0,
                                    zBUTTONS_YESNO, 0, 0 ) == zRESPONSE_YES )
               {
                  bChange = TRUE;
                  SetAttributeFromInteger( vAct, "Action", "Type", g_lToWAB );
               }
            }

            nRC = SetCursorNextEntity( vXSLT, "Event", 0 );
         }
      }
      else
      if ( lCheckFlags &
           (zCHECK_INCREMENT_POS_SIZE | zCHECK_DECREMENT_POS_SIZE) )
      {
         bChange = TRUE;
         lIncrX = LOWORD( lData );
         if ( lIncrX < 0 )
            lIncrX = -lIncrX;

         lIncrY = HIWORD( lData );
         if ( lIncrY < 0 )
            lIncrY = -lIncrY;

         if ( ((lCheckFlags & zCHECK_INCREMENT_POS_SIZEX) == zCHECK_INCREMENT_POS_SIZEX) ||
              ((lCheckFlags & zCHECK_DECREMENT_POS_SIZEX) == zCHECK_DECREMENT_POS_SIZEX) )
         {
            GetIntegerFromAttribute( &lPosX, vXSLT, "Control", "PSDLG_X" );
            GetIntegerFromAttribute( &lSizeX, vXSLT, "Control", "SZDLG_X" );
            if ( (lCheckFlags & zCHECK_INCREMENT_POS_SIZEX) == zCHECK_INCREMENT_POS_SIZEX )
            {
               lPosX += lIncrX * lPosX / 100;
               lSizeX += lIncrX * lSizeX / 100;
               if ( lSizeX <= 0 )
               {
                  TraceLineS( "(trpntrad) Warning: Control size x became equal to zero; Control: ", szCtrlTag );
               }
            }
            else
            {
               lPosX -= lIncrX * lPosX / 100;
               lSizeX -= lIncrX * lSizeX / 100;
               if ( lSizeX <= 0 )
               {
                  TraceLineS( "(trpntrad) Warning: Control size y became equal to zero; Control: ", szCtrlTag );
               }
            }

            SetAttributeFromInteger( vXSLT, "Control", "PSDLG_X", lPosX );
            SetAttributeFromInteger( vXSLT, "Control", "SZDLG_X", lSizeX );
         }

         if ( ((lCheckFlags & zCHECK_INCREMENT_POS_SIZEY) == zCHECK_INCREMENT_POS_SIZEY) ||
              ((lCheckFlags & zCHECK_DECREMENT_POS_SIZEY) == zCHECK_DECREMENT_POS_SIZEY) )
         {
            GetIntegerFromAttribute( &lPosY, vXSLT, "Control", "PSDLG_Y" );
            GetIntegerFromAttribute( &lSizeY, vXSLT, "Control", "SZDLG_Y" );
            if ( (lCheckFlags & zCHECK_INCREMENT_POS_SIZEY) == zCHECK_INCREMENT_POS_SIZEY )
            {
               lPosY += lIncrY * lPosY / 100;
               lSizeY += lIncrY * lSizeY / 100;
            }
            else
            {
               lPosY -= lIncrY * lPosY / 100;
               lSizeY -= lIncrY * lSizeY / 100;
            }

            SetAttributeFromInteger( vXSLT, "Control", "PSDLG_Y", lPosY );
            SetAttributeFromInteger( vXSLT, "Control", "SZDLG_Y", lSizeY );
         }
      }
      else
      {
         GetIntegerFromAttribute( &lKey, vXSLT, "ControlDef", "Key" );
         if ( CompareAttributeToInteger( vXSLT, "ControlDef", "Key", lKey ) != 0 )
         {
            if ( lCheckFlags & zCHECK_SAVE_ALL )
            {
               if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", lKey, 0 ) == 0 )
               {
                  bChange = TRUE;
                  ExcludeEntity( vXSLT, "ControlDef", zREPOS_NONE );
                  IncludeSubobjectFromSubobject( vXSLT, "ControlDef", vPE, "ControlDef", zPOS_BEFORE );
               }
               else
               {
                  TraceLine( "MigrateControls could NOT match PE for ControlDef "
                               "Key (%d) for Control: %s in Dlg.Wnd: %s.%s",
                             lKey, szCtrlTag, cpcDlgTag, cpcWndTag );
               }
            }
            else
            {
               TraceLine( "MigrateControls found unmatched ControlDef "
                            "Key (%d) for Control: %s in Dlg.Wnd: %s.%s",
                          lKey, szCtrlTag, cpcDlgTag, cpcWndTag );
            }
         }

         GetIntegerFromAttribute( &lKey, vXSLT, "ControlDef", "Key" );

         if ( (lCheckFlags & zCHECK_ACTIVEX_PROPERTIES) &&
              lKey > 4000 && lKey < 32000 ) // ActiveX
         {
            GetAttributeLength( &ulLth, vXSLT, "Control", "Properties" );
            if ( ulLth == 0 )
            {
               GetAttributeLength( &ulLth, vXSLT, "Control", "CtrlBOI" );
               if ( ulLth )
               {
                  SetAttributeFromAttribute( vXSLT, "Control", "Properties",
                                             vXSLT, "Control", "CtrlBOI" );
                  SetAttributeFromBlob( vXSLT, "Control", "CtrlBOI", "", 0 );
               }
            }
         }

         if ( (lCheckFlags & zCHECK_REPORT_TAB_PAGES) &&
              (lKey == zTAB_CONTROL) )
         {
            TraceLineS( "CreateSubSections -->", "" );
            TraceLineS( "CreateSubSections -->", "" );
            GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vXSLT, "Control", "Tag" );
            TraceLine( "CreateSubSections -->   // %s.%s.%s",
                       cpcDlgTag, cpcWndTag, szCtrlTag );
            TraceLine( "CreateSubSections -->   IF mSecTmpl.SecuritySection.SectionName = \"%s\"",
                       cpcWndTag );
            zSHORT nRC = SetCursorFirstEntity( vXSLT, "CtrlCtrl", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               TraceLineS( "CreateSubSections -->",
                           "      CREATE ENTITY  mSecTmpl.SecuritySubSection" );

               GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vXSLT, "CtrlCtrl", "Tag" );
               TraceLine( "CreateSubSections -->      mSecTmpl.SecuritySubSection.SubSectionName = \"%s\"",
                          szCtrlTag );
               TraceLineS( "CreateSubSections -->",
                           "      mSecTmpl.SecuritySubSection.AllowAccess =\"N\"" );

               nRC = SetCursorNextEntity( vXSLT, "CtrlCtrl", 0 );
            }
         }

         if ( (lCheckFlags & zCHECK_CHANGE_EVENTS) &&
              (lKey == zEDIT_CONTROL || lKey == zCOMBOBOX_CONTROL) )
         {
            zVIEW vAct;
            zCHAR szTag[ 34 ];

            CreateViewFromViewForTask( &vAct, vXSLT, 0 );
            while ( ResetViewFromSubobject( vAct ) == 0 )
            {
            }

            if ( lKey == zEDIT_CONTROL )
            {
               nRC = SetCursorFirstEntityByInteger( vXSLT, "Event",
                                                    "Type", 18, "" );
               while ( nRC >= zCURSOR_SET )
               {
                  if ( SetEntityCursor( vAct, "Action", 0,
                                        zPOS_FIRST | zQUAL_ENTITYCSR,
                                        vXSLT, "EventAct",
                                        0, 0, 0, 0 ) >= zCURSOR_SET )
                  {
                     if ( CompareAttributeToString( vAct, "Action",
                                                    "NoMap", "Y" ) != 0 )
                     {
                        GetStringFromAttribute( szTag, zsizeof( szTag ), vAct, "Action", "Tag" );
                        TraceLineS( "Edit Change Event found for Action: ",
                                    szTag );
                        TraceLineS( "   with Mapping not forced off in Window: ",
                                    cpcWndTag );
                     }
                  }

                  nRC = SetCursorNextEntityByInteger( vXSLT, "Event",
                                                      "Type", 18, "" );
               }
            }

            if ( lKey == zCOMBOBOX_CONTROL )
            {
               nRC = SetCursorFirstEntityByInteger( vXSLT, "Event",
                                                    "Type", 1, "" );
               while ( nRC >= zCURSOR_SET )
               {
                  if ( SetEntityCursor( vAct, "Action", 0,
                                        zPOS_FIRST | zQUAL_ENTITYCSR,
                                        vXSLT, "EventAct",
                                        0, 0, 0, 0 ) >= zCURSOR_SET )
                  {
                     if ( CompareAttributeToString( vAct, "Action",
                                                    "NoMap", "Y" ) != 0 )
                     {
                        GetStringFromAttribute( szTag, zsizeof( szTag ), vAct, "Action", "Tag" );
                        TraceLineS( "Combo Change Event 1 found for Action: ",
                                    szTag );
                        TraceLineS( "   with Mapping not forced off in Window: ",
                                    cpcWndTag );
                     }
                  }

                  nRC = SetCursorNextEntityByInteger( vXSLT, "Event",
                                                      "Type", 1, "" );
               }

               nRC = SetCursorFirstEntityByInteger( vXSLT, "Event",
                                                    "Type", 5, "" );
               while ( nRC >= zCURSOR_SET )
               {
                  if ( SetEntityCursor( vAct, "Action", 0,
                                        zPOS_FIRST | zQUAL_ENTITYCSR,
                                        vXSLT, "EventAct",
                                        0, 0, 0, 0 ) >= zCURSOR_SET )
                  {
                     if ( CompareAttributeToString( vAct, "Action",
                                                    "NoMap", "Y" ) != 0 )
                     {
                        GetStringFromAttribute( szTag, zsizeof( szTag ), vAct, "Action", "Tag" );
                        TraceLineS( "Combo Change Event 5 found for Action: ",
                                    szTag );
                        TraceLineS( "   with Mapping not forced off in Window: ",
                                    cpcWndTag );
                     }
                  }

                  nRC = SetCursorNextEntityByInteger( vXSLT, "Event",
                                                      "Type", 5, "" );
               }
            }

            if ( lKey == zCALENDAR_CONTROL )
            {
               nRC = SetCursorFirstEntityByInteger( vXSLT, "Event",
                                                    "Type", 1, "" );
               while ( nRC >= zCURSOR_SET )
               {
                  if ( SetEntityCursor( vAct, "Action", 0,
                                        zPOS_FIRST | zQUAL_ENTITYCSR,
                                        vXSLT, "EventAct",
                                        0, 0, 0, 0 ) >= zCURSOR_SET )
                  {
                     if ( CompareAttributeToString( vAct, "Action",
                                                    "NoMap", "Y" ) != 0 )
                     {
                        GetStringFromAttribute( szTag, zsizeof( szTag ), vAct, "Action", "Tag" );
                        TraceLineS( "Calendar Change Event found for Action: ",
                                    szTag );
                        TraceLineS( "   with Mapping not forced off in Window: ",
                                    cpcWndTag );
                     }
                  }

                  nRC = SetCursorNextEntityByInteger( vXSLT, "Event",
                                                      "Type", 1, "" );
               }
            }

            DropView( vAct );
         }

         if ( (lCheckFlags & zCHECK_OK) && lKey == zPUSHBUTTON_CONTROL )
         {
            GetAddrForAttribute( &pch, vXSLT, "Control", "Text" );
            if ( zstrcmp( pch, "&OK" ) == 0 )
            {
               bChange = TRUE;
               SetAttributeFromString( vXSLT, "Control", "Text", "OK" );
            }
            else
            if ( zstrcmp( pch, "&Cancel" ) == 0 )
            {
               bChange = TRUE;
               SetAttributeFromString( vXSLT, "Control", "Text", "Cancel" );
            }
         }

      // if ( (lCheckFlags & zCHECK_CHANGETAB_EVENT) &&
      //      lKey == zTABCTL_CONTROL )
      // {
      //    if ( SetCursorFirstEntityByInteger( vXSLT, "Event", "Type",
      //                                        1, "" ) >= zCURSOR_SET )
      //    {
      //       bChange = TRUE;
      //       SetAttributeFromInteger( vXSLT, "Event", "Type", 3 );
      //    }
      // }

      // if ( (lCheckFlags & zCHECK_CHANGE_BREADCRUMB) &&
      //      lKey == zTEXT_CONTROL )
      // {
      //    GetAddrForAttribute( &pch, vXSLT, "Control", "Tag" );
      //    GetIntegerFromAttribute( &lSubtype, vXSLT, "Control", "Subtype" );
      //
      //    if ( zstrcmp( pch, "BreadCrumb" ) == 0 )
      //    {
      //       if ( (lSubtype & zCONTROL_INVISIBLE) == 0 )
      //       {
      //          bChange = TRUE;
      //          lSubtype |= zCONTROL_INVISIBLE;
      //          SetAttributeFromInteger( vXSLT, "Control", "Subtype", lSubtype );
      //       }
      //    }
      // }

      // if ( (lCheckFlags & zCHECK_CHANGE_MOVEINCREMENT) &&
      //      lKey == zEDIT_CONTROL )
      // {
      //    GetAddrForAttribute( &pch, vXSLT, "Control", "Tag" );
      //    if ( zstrcmp( pch, "MoveIncrement" ) == 0 )
      //    {
      //       bChange = TRUE;
      //       SetAttributeFromInteger( vXSLT, "Control", "SZDLG_X", 16 );
      //    }
      // }

      // if ( (lCheckFlags & zCHECK_CHANGE_NOTETOREVIEWER) &&
      //      lKey == zEDIT_CONTROL )
      // {
      //    GetAddrForAttribute( &pch, vXSLT, "Control", "Tag" );
      //    if ( zstrcmp( pch, "NoteToReviewer" ) == 0 )
      //    {
      //       bChange = TRUE;
      //       SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", zMLE_CONTROL, "" );
      //       ExcludeEntity( vXSLT, "ControlDef", zREPOS_NONE );
      //       IncludeSubobjectFromSubobject( vXSLT, "ControlDef",
      //                                      vPE, "ControlDef", zPOS_BEFORE );
      //       SetAttributeFromBlob( vXSLT, "Control", "CtrlBOI", "", 0 );
      //
      //       nRC = SetCursorFirstEntity( vXSLT, "Event", 0 );
      //       while ( nRC == zCURSOR_SET )
      //       {
      //          nRC = DeleteEntity( vXSLT, "Event", zREPOS_FIRST );
      //       }
      //    }
      // }

         if ( (lCheckFlags & zCHECK_CHANGE_GROUPBOX_NEW) &&
              lKey == zGROUPBOX_CONTROL )
         {
            zBOOL  bFound = FALSE;
            zSHORT nCnt = 0;

            nRC = SetCursorFirstEntity( vXSLT, "CtrlCtrl", 0 );
            while ( nRC == zCURSOR_SET )
            {
               nCnt++;
               GetAddrForAttribute( &pch, vXSLT, "CtrlCtrl", "Text" );
               if ( zstrcmp( pch, "New" ) == 0 )
               {
                  bFound = TRUE;
               }

               nRC = SetCursorNextEntity( vXSLT, "CtrlCtrl", 0 );
            }

            if ( bFound && nCnt == 2 )
            {
               bChange = TRUE;
               SetAttributeFromString( vXSLT, "Control", "CSS_Class", "listgroup" );
               SetAttributeFromInteger( vXSLT, "Control", "SZDLG_X", 243 );
               SetAttributeFromInteger( vXSLT, "Control", "SZDLG_Y", 20 );
               nRC = SetCursorFirstEntity( vXSLT, "CtrlCtrl", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  GetAddrForAttribute( &pch, vXSLT, "CtrlCtrl", "Text" );
                  if ( zstrcmp( pch, "New" ) == 0 )
                  {
                     SetAttributeFromString( vXSLT, "CtrlCtrl", "CSS_Class", "newbutton" );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "PSDLG_X", 171 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "PSDLG_Y", 6 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "SZDLG_X", 39 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "SZDLG_Y", 13 );
                  }
                  else
                  {
                     SetAttributeFromString( vXSLT, "CtrlCtrl", "CSS_Class", "listheader" );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "PSDLG_X", 5 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "PSDLG_Y", 6 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "SZDLG_X", 77 );
                     SetAttributeFromInteger( vXSLT, "CtrlCtrl", "SZDLG_Y", 8 );
                  }

                  nRC = SetCursorNextEntity( vXSLT, "CtrlCtrl", 0 );
               }
            }
         }


         if ( lKey == zLISTBOX_CONTROL )
         {
            GetIntegerFromAttribute( &lSubtype, vXSLT, "Control", "Subtype" );
            if ( lSubtype & (zLISTBOX_COLUMNHEADERS | zLISTBOX_SORTEDHEADERS) )
            {
               if ( (lCheckFlags & zCHECK_EVENTS) &&
                    SetCursorFirstEntityByInteger( vXSLT, "Event", "Type",
                                                   12, "" ) < zCURSOR_SET )
               {
                  if ( SetCursorFirstEntityByInteger( vXSLT, "Event", "Type",
                                                      13, "" ) >= zCURSOR_SET ||
                       SetCursorFirstEntityByInteger( vXSLT, "Event", "Type",
                                                      14, "" ) >= zCURSOR_SET )
                  {
                     bChange = TRUE;
                     SetAttributeFromInteger( vXSLT, "Event", "Type", 12 );
                  }
               }

               /*
               if ( lCheckFlags & zCHECK_READONLY_LISTBOX )
               {
                  GetIntegerFromAttribute( &lSubtype, vXSLT,
                                           "Control", "ExtendedStyle" );

                  if ( (lSubtype & zCONTROLX_DISABLE_READONLY) == 0 )
                  {
                     bChange = TRUE;
                     lSubtype |= zCONTROLX_DISABLE_READONLY;
                     SetAttributeFromInteger( vXSLT, "Control",
                                              "ExtendedStyle", lSubtype );
                  }
               }
               */
            }
         }

         if ( ((lCheckFlags & zCHECK_DATETIMEDOMAIN_EDITBOX) ||
               (lCheckFlags & zCHECK_DATEDOMAIN_EDITBOX) ||
               (lCheckFlags & zCHECK_READONLY_EDITBOX)) &&
              lKey == zEDIT_CONTROL )
         {
            if ( lCheckFlags & zCHECK_READONLY_EDITBOX )
            {
               GetIntegerFromAttribute( &lSubtype, vXSLT,
                                        "Control", "ExtendedStyle" );

               if ( (lSubtype & zCONTROLX_DISABLE_READONLY) == 0 )
               {
                  bChange = TRUE;
                  lSubtype |= zCONTROLX_DISABLE_READONLY;
                  SetAttributeFromInteger( vXSLT, "Control",
                                           "ExtendedStyle", lSubtype );
               }
            }

            if ( ((lCheckFlags & zCHECK_DATETIMEDOMAIN_EDITBOX) ||
                  (lCheckFlags & zCHECK_DATEDOMAIN_EDITBOX)) )
            {
               if ( CheckExistenceOfEntity( vXSLT, "CtrlMapER_Domain" ) == 0 )
               {
                  GetAddrForAttribute( &pch, vXSLT,
                                       "CtrlMapER_Domain", "Name" );
                  if ( ((lCheckFlags & zCHECK_DATEDOMAIN_EDITBOX) &&
                        zstrcmp( "Date", pch ) == 0) ||
                       ((lCheckFlags & zCHECK_DATETIMEDOMAIN_EDITBOX) &&
                        zstrcmp( "DateTime", pch ) == 0) )
                  {
                     if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key",
                                                         zCALENDAR_CONTROL,
                                                         0 ) == 0 )
                     {
                        bChange = TRUE;
                        ExcludeEntity( vXSLT, "ControlDef", zREPOS_NONE );
                        IncludeSubobjectFromSubobject( vXSLT, "ControlDef",
                                                       vPE, "ControlDef",
                                                       zPOS_BEFORE );
                     }
                  }
               }
            }
         }

         if ( (lCheckFlags & zCHECK_CALENDAR_TO_EDITBOX) &&
              lKey == zCALENDAR_CONTROL )
         {
            if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key",
                                                zEDIT_CONTROL, 0 ) == 0 )
            {
               bChange = TRUE;
               ExcludeEntity( vXSLT, "ControlDef", zREPOS_NONE );
               IncludeSubobjectFromSubobject( vXSLT, "ControlDef",
                                              vPE, "ControlDef",
                                              zPOS_BEFORE );
            }
         }

#if 1
         if ( (lCheckFlags & zCHECK_CTRL_TYPE) && lKey == zPUSHBUTTON_CONTROL )
         {
            bChange = TRUE;
            SetAttributeFromString( vXSLT, "Control", "CSS_Class", "formStylebutton" );
         }
#else
         if ( lCheckFlags & zCHECK_CTRL_TYPE )
         {
            GetIntegerFromAttribute( &lSubtype, vXSLT, "Control", "Type" );
            if ( lSubtype )
            {
               zCHAR szMsg[ 256 ];

               GetAddrForAttribute( &pch, vXSLT, "Control", "Tag" );
               sprintf_s( szMsg, zsizeof( szMsg ), "(%s) Type for ctrl: \"%s\" is: ",
                         cpcWndTag, pch );
               TraceLineX( szMsg, lSubtype );
            }
         }
#endif
         if ( (lCheckFlags & zCHECK_COMBOBOX_NO_NULL) &&
              (lKey == zCOMBOBOX_CONTROL) )
         {
            GetIntegerFromAttribute( &lSubtype, vXSLT, "Control", "Subtype" );
            if ( lSubtype & zCOMBOBOX_NO_NULL )
            {
               zCHAR szMsg[ 256 ];

               GetAddrForAttribute( &pch, vXSLT, "Control", "Tag" );
               sprintf_s( szMsg, zsizeof( szMsg ), "(%s) ComboBox No Null for ctrl: \"%s\"",
                         cpcWndTag, pch );
               TraceLineS( szMsg, "" );
            }
         }

         if ( (lCheckFlags & zCHECK_RESIZE_EDIT_CTRLS) &&
              (lKey == zEDIT_CONTROL || lKey == zTEXT_CONTROL) )
         {
            GetIntegerFromAttribute( &lSizeY, vXSLT, "Control", "SZDLG_Y" );
            if ( lSizeY == g_lFromHeight || g_lFromHeight == 0 )
            {
               bChange = TRUE;
               SetAttributeFromInteger( vXSLT, "Control", "SZDLG_Y", g_lToHeight );
            }
         }
      }

      SetViewToSubobject( vXSLT, "CtrlCtrl" );
      if ( CheckMigrateCtrls( vXSLT, vPE, lCheckFlags,
                              cpcDlgTag, cpcWndTag, lData ) )
      {
         bChange = TRUE;
      }

      ResetViewFromSubobject( vXSLT );
      nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
   }

   return( bChange );
}

// z11001Zeidon    TEMP     TZWDLGSO 04/05/00   13:27:08 1.0a
// z10000Zeidon    ACCOUNT  KZWDLGXO 04/05/00   13:27:07 1.0a
zSHORT
fnLocateXSLTDate( zPCHAR pchBuffer )
{
   zPCHAR pchColon2;
   zPCHAR pchColon1;
   zPCHAR pchSlash2;
   zPCHAR pchSlash1;
   zLONG  k;

   pchColon2 = zstrchr( pchBuffer, '\n' );
   if ( pchColon2 )
      *pchColon2 = 0;

   pchColon2 = zstrchr( pchBuffer, '\r' );
   if ( pchColon2 )
      *pchColon2 = 0;

   pchColon2 = zstrrchr( pchBuffer, ':' );
   if ( pchColon2 )
   {
      *pchColon2 = 0;
      pchColon1 = zstrrchr( pchBuffer, ':' );
      if ( pchColon1 )
      {
         pchSlash2 = zstrrchr( pchBuffer, '/' );
         if ( pchSlash2 )
         {
            *pchSlash2 = 0;
            pchSlash1 = zstrrchr( pchBuffer, '/' );
            if ( pchSlash1 && pchSlash1 + 3 == pchSlash2 &&
                 pchColon1 + 3 == pchColon2 )
            {
               *pchColon2 = ':';
               *(pchColon2 + 3) = 0;
               k = zatol( pchSlash2 + 1 );
               if ( k > 80 )
               {
                  pchBuffer[ 0 ] = '1';
                  pchBuffer[ 1 ] = '9';
               }
               else
               {
                  pchBuffer[ 0 ] = '2';
                  pchBuffer[ 1 ] = '0';
               }

               strcpy_s( pchBuffer + 2, pchSlash2 + 1 );
               pchBuffer[ 4 ] = '/';
               strcpy_s( pchBuffer + 5, pchSlash1 - 2 );
               pchBuffer[ 10 ] = ' ';
               strcpy_s( pchBuffer + 11, pchColon1 - 2 );
               return( 0 );   // we think we did this correctly
            }
         }
      }
   }

   return( -1 );   // there was some sort of problem
}

zBOOL
ProportionallyResizeWindow( zVIEW vXSLT, zLONG lCheckFlags, zLONG lIncr,
                            zCPCHAR cpcDlgTag, zCPCHAR cpcWndTag )
{
   zLONG  lIncrX;
   zLONG  lIncrY;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zSHORT nClientWidth;
   zSHORT nClientHeight;
   zSHORT nClientSizeX;
   zSHORT nClientSizeY;
   zBOOL  bStop = FALSE;

   lIncrX = LOWORD( lIncr );
   if ( lIncrX < 0 )
       lIncrX = -lIncrX;

   lIncrY = HIWORD( lIncr );
   if ( lIncrY < 0 )
       lIncrY = -lIncrY;

   GetIntegerFromAttribute( &lPosX, vXSLT, "Window", "PSDLG_X" );
   GetIntegerFromAttribute( &lPosY, vXSLT, "Window", "PSDLG_Y" );
   GetIntegerFromAttribute( &lSizeX, vXSLT, "Window", "SZDLG_X" );
   GetIntegerFromAttribute( &lSizeY, vXSLT, "Window", "SZDLG_Y" );

   nClientWidth = LOWORD( lSizeX );
   nClientHeight = LOWORD( lSizeY );
   nClientSizeX = HIWORD( lSizeX );
   nClientSizeY = HIWORD( lSizeY );
   if ( ((lCheckFlags & zCHECK_INCREMENT_POS_SIZEX) ==
                                                zCHECK_INCREMENT_POS_SIZEX) ||
        ((lCheckFlags & zCHECK_INCREMENT_POS_SIZEY) ==
                                                zCHECK_INCREMENT_POS_SIZEY) )
   {
      if ( (lCheckFlags & zCHECK_INCREMENT_POS_SIZEX) ==
                                                zCHECK_INCREMENT_POS_SIZEX )
      {
         lPosX += lIncrX * lPosX / 100;
         nClientWidth += (zSHORT) (lIncrX * nClientWidth / 100);
         nClientSizeX += (zSHORT) (lIncrX * nClientSizeX / 100);
      }

      if ( (lCheckFlags & zCHECK_INCREMENT_POS_SIZEY) == zCHECK_INCREMENT_POS_SIZEY )
      {
         lPosY += lIncrY * lPosY / 100;
         nClientHeight += (zSHORT) (lIncrY * nClientHeight / 100);
         nClientSizeY += (zSHORT) (lIncrY * nClientSizeY / 100);
      }
   }
   else
   {
      if ( (lCheckFlags & zCHECK_DECREMENT_POS_SIZEX) == zCHECK_DECREMENT_POS_SIZEX )
      {
         lPosX -= lIncrX * lPosX / 100;
         nClientWidth -= (zSHORT) (lIncrX * nClientWidth / 100);
         nClientSizeX -= (zSHORT) (lIncrX * nClientSizeX / 100);
      }

      if ( (lCheckFlags & zCHECK_DECREMENT_POS_SIZEY) ==
                                                zCHECK_DECREMENT_POS_SIZEY )
      {
         lPosY -= lIncrY * lPosY / 100;
         nClientHeight -= (zSHORT) (lIncrY * nClientHeight / 100);
         nClientSizeY -= (zSHORT) (lIncrY * nClientSizeY / 100);
      }
   }

   lSizeX = MAKELONG( nClientWidth, nClientSizeX );
   lSizeY = MAKELONG( nClientHeight, nClientSizeY );

   if ( nClientWidth <= 0 || nClientHeight <= 0 )
   {
      zCHAR   szWindowTag[ 34 ];
      zCHAR   szMsg[ 512 ];

      GetStringFromAttribute( szWindowTag, zsizeof( szWindowTag ), vXSLT, "Window", "Tag" );
      strcpy_s( szMsg, zsizeof( szMsg ), "Window size became equal to zero; Window: " );
      strcat_s( szMsg, zsizeof( szMsg ), szWindowTag) ;
      strcat_s( szMsg, zsizeof( szMsg ), "\nDo you wish to abort the resize?" );

      if ( MessagePrompt( vXSLT, "PE002", "XSLT Maintenance",
                          szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,  0 ) != zRESPONSE_YES )
      {
         bStop = TRUE;
      }
   }

   if ( bStop == FALSE )
   {
      SetAttributeFromInteger( vXSLT, "Window", "PSDLG_X", lPosX );
      SetAttributeFromInteger( vXSLT, "Window", "PSDLG_Y", lPosY );
      SetAttributeFromInteger( vXSLT, "Window", "SZDLG_X", lSizeX );
      SetAttributeFromInteger( vXSLT, "Window", "SZDLG_Y", lSizeY );

      CheckMigrateCtrls( vXSLT, 0, lCheckFlags, cpcDlgTag, cpcWndTag, lIncr );
   }

   return( TRUE ); // change had to occur
}

zOPER_EXPORT zSHORT OPERATION
MigrateXSLTs( zVIEW vSubtask )
{
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zPCHAR pchMenuTag;
   zPCHAR pchOptionTag;
   zPCHAR pchActionTag;
   zPCHAR pchTag;
   zPCHAR pch;
   zVIEW  vXSLTList;
   zVIEW  vXSLT;
   zVIEW  vPE;
   zBOOL  bChange;
   zSHORT nRC;
   zLONG  lCheckFlags = 0;
   zBOOL  bSaveAll = FALSE;
   zBOOL  bSaveAllJSP = FALSE;
   zLONG  lIncr;
   zLONG  k = 0;
   zVIEW  vTaskLPLR = 0;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      MessageSend( vSubtask, "PN00201", "XSLT Maintenance",
                   "Presentation Environment does not exist!\n"
                   "Cannot migrate XSLTs.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                   "Unable to open Task LPLR!\n"
                   "Cannot migrate XSLTs.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   do // purist's goto
   {

   if ( MessageBox( 0, "Do you want to build JSP for\n"
                       "all XSLTs with name starting with 'w'?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_SAVE_ALL_JSP;
      bSaveAllJSP = TRUE;
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) <= 0 )
      {
         MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                      "Unable to open Task LPLR!\n"
                      "Cannot migrate XSLTs.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }

      break; // do only this option
   }
   else
   if ( MessageBox( 0, "Do you want to save all XSLTs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      bSaveAll = TRUE;
      break; // do only this option
   }
// else
// if ( MessageBox( 0, "Do you want to reset the ChangeTab event\n"
//                     "to the new ChangeTab (Pre MapFromOI) event?",
//                  "XSLT Migration", MB_YESNO ) == IDYES )
// {
//    lCheckFlags |= zCHECK_CHANGETAB_EVENT;
//    break; // do only this option
// }
// else
// if ( MessageBox( 0, "Do you want to reset the BreadCrumb visible flag",
//                  "XSLT Migration", MB_YESNO ) == IDYES )
// {
//    lCheckFlags |= zCHECK_CHANGE_BREADCRUMB;
//    break; // do only this option
// }
   else
   if ( MessageBox( 0, "Do you want to change WebJavaScript?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
   // if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
   //                              "Specify the Change From JavaScript",
   //                              FALSE, g_szChangeFrom,
   //                              zsizeof( g_szChangeFrom ) ) == IDOK )

      OperatorPromptForFile( vSubtask, g_szChangeFrom, 256,
                             "POR Files (*.POR)|*.por|",
                             "POR",
                             zOFN_HIDEREADONLY | zOFN_NOCHANGEDIR |
                             zOFN_FILEMUSTEXIST );
      OperatorPromptForFile( vSubtask, g_szChangeTo, 256,
                             "POR Files (*.POR)|*.por|",
                             "POR",
                             zOFN_HIDEREADONLY | zOFN_NOCHANGEDIR |
                             zOFN_FILEMUSTEXIST );
      if ( g_szChangeFrom[ 0 ] && g_szChangeTo[ 0 ] )
      {
         zLONG lFile;

         lFile = SysOpenFile( vSubtask, g_szChangeFrom, COREFILE_READ );
         SysReadFile( vSubtask, lFile, g_szChangeFrom, 4096 );
         SysCloseFile( vSubtask, lFile, 0 );
         lFile = SysOpenFile( vSubtask, g_szChangeTo, COREFILE_READ );
         SysReadFile( vSubtask, lFile, g_szChangeTo, 4096 );
         SysCloseFile( vSubtask, lFile, 0 );
         if ( g_szChangeFrom[ 0 ] && g_szChangeTo[ 0 ] )
         {
            zCHAR szBuffer[ 16000 ];

            zsprintf( szBuffer, "Do you want to change WebJavaScript from: %s\n\n\n\n     to: %s",
                      g_szChangeFrom, g_szChangeTo );
            if ( MessageBox( 0, szBuffer,  "XSLT Migration", MB_YESNO ) == IDYES )
            {
               lCheckFlags |= zCHECK_EVENTS_WEBJAVASCRIPT;
               bSaveAll = TRUE;
            }
         }
      }

      break; // do only this option
   }
// else
// if ( MessageBox( 0, "Do you want to reset the MoveIncrement width to 16",
//                  "XSLT Migration", MB_YESNO ) == IDYES )
// {
//    lCheckFlags |= zCHECK_CHANGE_MOVEINCREMENT;
//    break; // do only this option
// }
// else
// if ( MessageBox( 0, "Do you want to reset the NoteToReviewer to MLEdit",
//                  "XSLT Migration", MB_YESNO ) == IDYES )
// {
//    lCheckFlags |= zCHECK_CHANGE_NOTETOREVIEWER;
//    break; // do only this option
// }
   else
   if ( MessageBox( 0, "Do you want to reset the New Button in the Groupbox",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_CHANGE_GROUPBOX_NEW;
      break; // do only this option
   }
   else
   if ( MessageBox( 0, "Do you want to report all Menus for\n"
                       "all XSLTs with name starting with 'w'?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_REPORT_WEBMENUS;
      break; // do only this option
   }
   else
   if ( MessageBox( 0, "Do you want to report all Notepages for\n"
                       "all Notebooks in all XSLTs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_REPORT_TAB_PAGES;
   }
   else
   if ( MessageBox( 0, "Do you want to change mapping for a XSLT?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      zCHAR szMsg[ 256 ];

      lCheckFlags |= zCHECK_CHANGE_MAPPING;
      g_szDlgTag[ 0 ] = 0;
      while ( g_szDlgTag[ 0 ] == 0 )
      {
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter XSLT", 0,
                                 g_szDlgTag, zsizeof( g_szDlgTag ) - 1 );
      }

      sprintf_s( szMsg, zsizeof( szMsg ), "Are you sure you want to change mapping for XSLT: %s", g_szDlgTag );
      if ( OperatorPrompt( vSubtask, "XSLT Migration", szMsg, 0,
                           zBUTTONS_OKCANCEL, 0, 0 ) != zRESPONSE_OK )
      {
         return( 0 );
      }

      break; // do only this option
   }
   else
   if ( MessageBox( 0, "Do you want to add a GroupBox for a XSLT for\n"
                       "controls at the root level after a specified GroupBox?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      zCHAR szMsg[ 256 ];

      lCheckFlags |= zCHECK_ADD_GROUPBOX;
      g_szDlgTag[ 0 ] = 0;
      g_szGroupBox[ 0 ] = 0;
      while ( g_szGroupBox[ 0 ] == 0 || g_szDlgTag[ 0 ] == 0 )
      {
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter XSLT", 0,
                                 g_szDlgTag, zsizeof( g_szDlgTag ) - 1 );
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter GroupBox", 0,
                                 g_szGroupBox, zsizeof( g_szGroupBox ) - 1 );
      }

      sprintf_s( szMsg, zsizeof( szMsg ), "Are you sure you want to add a GroupBox after specified GroupBox: %s\n"
                       "  to XSLT: %s?", g_szGroupBox, g_szDlgTag );
      if ( OperatorPrompt( vSubtask, "XSLT Migration", szMsg, 0,
                           zBUTTONS_OKCANCEL, 0, 0 ) != zRESPONSE_OK )
      {
         return( 0 );
      }

      break; // do only this option
   }
   else
   if ( MessageBox( 0, "Do you want to report\n"
                       "all Events for XSLTs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_REPORT_EVENTS;
   }
   else
   if ( MessageBox( 0, "Do you want to change WABs\n"
                       "for all Actions for XSLTs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      zCHAR szWAB[ 256 ];

      lCheckFlags |= zCHECK_CHANGE_WAB;
      g_lFromWAB = g_lToWAB = -1;
      szWAB[ 0 ] = 0;
      while ( g_lFromWAB < 0 || g_lFromWAB > 99 || g_lToWAB < 0 || g_lToWAB > 99 )
      {
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'From' WAB", 0,
                                 szWAB, zsizeof( szWAB ) - 1 );
         g_lFromWAB = zatol( szWAB );
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'To' WAB", 0,
                                 szWAB, zsizeof( szWAB ) - 1 );
         g_lToWAB = zatol( szWAB );
      }

      zsprintf( szWAB, "Are you sure you want to convert selected WABs\n"
                       "  from: %s\n  to:   %s?",
                       GetActionString( g_lFromWAB ), GetActionString( g_lToWAB ) );
      if ( OperatorPrompt( vSubtask, "XSLT Migration", szWAB, 0,
                           zBUTTONS_OKCANCEL, 0, 0 ) != zRESPONSE_OK )
      {
         return( 0 );
      }
   }
   else
   if ( MessageBox( 0, "Do you want to save\n"
                       "dialogs based on Date only?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_TIMESTAMP_ONLY;
   }
   else
   if ( MessageBox( 0, "Do you want to report all Notepages for\n"
                       "all Notebooks in all XSLTs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_REPORT_TAB_PAGES;
   }
   else
   if ( MessageBox( 0, "Do you want to check for the ComboBox No Null\n"
                       "specification for all dialogs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_COMBOBOX_NO_NULL;
   }
   else
   if ( MessageBox( 0, "Do you want to migrate the PostBuild event\n"
                       "to the SetupWindow event for all dialogs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_POSTBUILD_SETUP;
      bSaveAll = TRUE;
   }
   else
   if ( MessageBox( 0, "Do you want to resize all dialogs?",
                    "XSLT Migration", MB_YESNO ) == IDYES )
   {
      zCHAR  szBuffer[ 32 ];
      zLONG  lIncrX;
      zLONG  lIncrY;

      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      bSaveAll = TRUE;
      lIncr = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage X increment(+)"
                                     "/decrement(-) ... 0 for no X resize",
                                   FALSE, szBuffer,
                                   zsizeof( szBuffer ) ) == IDOK )
      {
         lIncrX = atol( szBuffer );
      }
      else
         lIncrX = 0;

      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage Y increment(+)"
                                     "/decrement(-) ... 0 for no Y resize",
                                   FALSE, szBuffer,
                                   zsizeof( szBuffer ) ) == IDOK )
      {
         lIncrY = atol( szBuffer );
      }
      else
         lIncrY = 0;

      lIncr = MAKELONG( abs( lIncrX ), abs( lIncrY ) );
      if ( lIncr )
      {
         if ( lIncrX )
         {
            if ( lIncrX > 0 )
               lCheckFlags |= zCHECK_INCREMENT_POS_SIZEX;
            else
               lCheckFlags |= zCHECK_DECREMENT_POS_SIZEX;
         }

         if ( lIncrY )
         {
            if ( lIncrY > 0 )
               lCheckFlags |= zCHECK_INCREMENT_POS_SIZEY;
            else
               lCheckFlags |= zCHECK_DECREMENT_POS_SIZEY;
         }
      }
      else
         return( 0 );
   }
   else
   {
      if ( MessageBox( 0, "Do you want to set ActiveX Properties?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_ACTIVEX_PROPERTIES;
      }

      if ( MessageBox( 0, "Do you want to change EditBoxes\n"
                          "based on the Date Domain?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_DATEDOMAIN_EDITBOX;
      }

      if ( MessageBox( 0, "Do you want to change EditBoxes\n"
                          "files based on the DateTime Domain?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_DATETIMEDOMAIN_EDITBOX;
      }

      if ( MessageBox( 0, "Do you want to change\n"
                          "&OK to OK and &Cancel to Cancel?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_OK;
      }

      if ( MessageBox( 0, "Do you want to check for 'change' events\n"
                          "that do not have mapping forced off\n"
                          "(see Trace Log for listing)?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_CHANGE_EVENTS;
      }

      if ( MessageBox( 0, "Do you want to change\n"
                          "the Right Mouse Button Events for Header Listboxes?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_EVENTS;
      }

      /*
      if ( MessageBox( 0, "Do you want to change\n"
                          "Header Listboxes to Disable as Readonly?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_READONLY_LISTBOX;
      }
      */

      if ( MessageBox( 0, "Do you want to change\n"
                          "Editboxes to Disable as Readonly?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_READONLY_EDITBOX;
      }

      if ( MessageBox( 0, "Do you want to change\n"
                          "All controls to Disable as Readonly?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_READONLY_ALL_CTRLS;
      }

      if ( MessageBox( 0, "Do you want to change all text/edit controls to specified height?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         zCHAR szHeight[ 256 ];

         lCheckFlags |= zCHECK_RESIZE_EDIT_CTRLS;
         szHeight[ 0 ] = 0;
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'From' height (0 for all)", 0,
                                 szHeight, zsizeof( szHeight ) - 1 );
         g_lFromHeight = zatol( szHeight );
         szHeight[ 0 ] = 0;
         OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'To' height", 0,
                                 szHeight, zsizeof( szHeight ) - 1 );
         g_lToHeight = zatol( szHeight );
      }

      if ( MessageBox( 0, "Do you want to change Calendar\n"
                          "controls to EditBox controls?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_CALENDAR_TO_EDITBOX;
      }

#if 1
      if ( MessageBox( 0, "Do you want to set all 'New' buttons class to formStylebutton?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_CTRL_TYPE;
      }
#else
      if ( MessageBox( 0, "Do you want to check\n"
                          "Type of ctrls?",
                       "XSLT Migration", MB_YESNO ) == IDYES )
      {
         lCheckFlags |= zCHECK_CTRL_TYPE;
      }
#endif

   }
   } while( FALSE );  // end of: purist's goto

   if ( bSaveAll )
      lCheckFlags |= zCHECK_SAVE_ALL;

   if ( lCheckFlags == 0 && bSaveAll == FALSE )
      return( 0 );

// RetrieveViewForMetaList( vSubtask, &vXSLTList, zSOURCE_XSLT_META );
   RetrieveViewForMetaList( vSubtask, &vXSLTList, zREFER_XSLT_META );
   nRC = SetCursorFirstEntity( vXSLTList, "W_MetaDef", 0 );
   while ( nRC == zCURSOR_SET )
   {
      k++;
      if ( ActivateMetaOI( vSubtask, &vXSLT, vXSLTList,
                           zSOURCE_XSLT_META,
                           zSINGLE | zLEVEL_TASK ) == 1 )
      {
         zCHAR szMsg[ 256 ];

         SetNameForView( vXSLT, "__CheckXSLT", vSubtask, zLEVEL_TASK );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         bChange = bSaveAll;

         if ( lCheckFlags & zCHECK_SAVE_ALL_JSP )
         {
            if ( pchDlgTag[ 0 ] == 'w' )
            {
               g_bSkipPrompt = TRUE;
               SetNameForView( vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
               GenerateAllXSLT( vSubtask );
               g_bSkipPrompt = FALSE;
            }
         }
         else
         if ( lCheckFlags & zCHECK_EVENTS_WEBJAVASCRIPT )
         {
            if ( pchDlgTag[ 0 ] == 'w' )
            {
               zBOOL bWindowChange;

               TraceLineS( "WebJavaScript checking dialog: ", pchDlgTag );

               bChange = FALSE;
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  bWindowChange = FALSE;
                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
                  nRC = SetCursorFirstEntity( vXSLT, "Action", 0 );
                  while ( nRC == zCURSOR_SET )
                  {
                     GetAddrForAttribute( &pchActionTag, vXSLT, "Action", "Tag" );
                     GetAddrForAttribute( &pch, vXSLT, "Action", "WebJavaScript" );
                     if ( zstrcmp( pch, g_szChangeFrom ) == 0 )
                     {
                        TraceLine( "Changing WebJavaScript for XSLT.Window.Action: %s.%s.%s",
                                   pchDlgTag, pchWndTag, pchActionTag );
                        SetAttributeFromString( vXSLT, "Action", "WebJavaScript", g_szChangeTo );
                        bChange = TRUE;
                        bWindowChange = TRUE;
                     }

                     nRC = SetCursorNextEntity( vXSLT, "Action", 0 );
                  }

                  if ( bWindowChange )
                  {
                     TraceLine( "WebJavaScript modified for dialog.window: %s.%s", pchDlgTag, pchWndTag );
                  }
                  else
                  {
                     TraceLine( "WebJavaScript NOT modified for dialog.window: %s.%s", pchDlgTag, pchWndTag );
                  }

                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
                  TraceLineS( "", "" );
               }

               if ( bChange )
               {
                  TraceLineS( "WebJavaScript modified for dialog: ", pchDlgTag );
                  CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
               }
               else
               {
                  TraceLineS( "WebJavaScript NOT modified for dialog: ", pchDlgTag );
               }
            }
         }
         else
         if ( lCheckFlags & zCHECK_REPORT_WEBMENUS )
         {
            if ( pchDlgTag[ 0 ] == 'w' )
            {
               SetNameForView( vXSLT, "TZXSLTL", vSubtask, zLEVEL_TASK );
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
                  if ( CheckExistenceOfEntity( vXSLT, "DfltMenu" ) == 0 )
                  {
                     GetAddrForAttribute( &pchTag, vXSLT, "DfltMenu", "Tag" );
                     TraceLine( "Reporting Menus for XSLT.Window: %s.%s   DfltMenu: %s",
                                pchDlgTag, pchWndTag, pchTag );
                  }
                  else
                  {
                     TraceLine( "Reporting Menus for XSLT.Window: %s.%s",
                                pchDlgTag, pchWndTag );
                  }

                  if ( CheckExistenceOfEntity( vXSLT, "ReusableMainWindow" ) == 0 )
                  {
                     GetAddrForAttribute( &pchTag, vXSLT, "ReusableMainXSLT", "Tag" );
                     GetAddrForAttribute( &pchWndTag, vXSLT, "ReusableMainWindow", "Tag" );
                     TraceLine( "   Reusable Main Menu XSLT.Window: %s.%s", pchTag, pchWndTag );
                  }

                  if ( CheckExistenceOfEntity( vXSLT, "ReusableSideWindow" ) == 0 )
                  {
                     GetAddrForAttribute( &pchTag, vXSLT, "ReusableSideXSLT", "Tag" );
                     GetAddrForAttribute( &pchWndTag, vXSLT, "ReusableSideWindow", "Tag" );
                     TraceLine( "   Reusable Side Menu XSLT.Window: %s.%s", pchTag, pchWndTag );
                  }

                  nRC = SetCursorFirstEntity( vXSLT, "Menu", 0 );
                  while ( nRC == zCURSOR_SET )
                  {
                     GetAddrForAttribute( &pchMenuTag, vXSLT, "Menu", "Tag" );
                     nRC = SetCursorFirstEntity( vXSLT, "Option", 0 );
                     while ( nRC == zCURSOR_SET )
                     {
                        GetAddrForAttribute( &pchOptionTag, vXSLT, "Option", "Tag" );
                        if ( CheckExistenceOfEntity( vXSLT, "OptAct" ) == 0 )
                        {
                           GetAddrForAttribute( &pchTag, vXSLT, "OptAct", "Tag" );
                           TraceLine( "   Menu.Option.Action: %s.%s.%s", pchMenuTag, pchOptionTag, pchTag );
                        }
                        else
                        {
                           TraceLine( "   Menu.Option.Action: %s.%s", pchMenuTag, pchOptionTag, "<no action>" );
                        }

                        nRC = SetCursorNextEntity( vXSLT, "Option", 0 );
                     }

                     nRC = SetCursorNextEntity( vXSLT, "Menu", 0 );
                  }

                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
                  TraceLineS( "", "" );
               }
            }
         }
         else
         if ( lCheckFlags & zCHECK_POSTBUILD_SETUP )
         {
            bChange = FALSE;
            nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
            while ( nRC == zCURSOR_SET )
            {
               if ( SetCursorFirstEntityByInteger( vXSLT, "WndEvent",
                                                   "Type", 0x00200000,
                                                   0 ) != zCURSOR_SET &&
                    SetCursorFirstEntityByInteger( vXSLT, "WndEvent",
                                                   "Type", 2,
                                                   0 ) == zCURSOR_SET )
               {
                  bChange = TRUE;
                  SetAttributeFromInteger( vXSLT, "WndEvent",
                                           "Type", 0x00200000 );
               }

               nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
            }

            if ( bChange )
            {
               TraceLineS( "MigrateXSLTs modified dialog: ", pchDlgTag );
               CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
            }
         }
         else
         if ( lCheckFlags &
              (zCHECK_INCREMENT_POS_SIZE | zCHECK_DECREMENT_POS_SIZE) )
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
                  ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
               }

               TraceLineS( "MigrateXSLTs modified dialog: ", pchDlgTag );
               CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
            }
         }
         else
         if ( (lCheckFlags & zCHECK_CHANGE_MAPPING) && zstrcmp( g_szDlgTag, pchDlgTag ) == 0 )
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               sprintf_s( szMsg, zsizeof( szMsg ), "Checking XSLT: %s for Migration", pchDlgTag );
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == 0 )
               {
                  zVIEW  vOldViewName;
                  zVIEW  vNewViewName;
                  zCHAR  szOldViewName[ 33 ];
                  zCHAR  szNewViewName[ 33 ];

                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );

                  CreateViewFromViewForTask( &vOldViewName, vXSLT, 0 );
                  CreateViewFromViewForTask( &vNewViewName, vXSLT, 0 );
                  strcpy_s( szOldViewName, zsizeof( szOldViewName ), "mSubLC" );
                  strcpy_s( szNewViewName, zsizeof( szNewViewName ), "mSPLDef" );
                  if ( SetCursorFirstEntityByString( vOldViewName, "ViewObjRef", "Name",
                                                     szOldViewName, 0 ) < zCURSOR_SET )
                  {
                     MessageSend( vSubtask, "", "Change View Mapping",
                                  "Old View Name is not a Registered View in this XSLT.",
                                  zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
                     SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
                     DropView( vOldViewName );
                     DropView( vNewViewName );
                     bChange = FALSE;
                     break;
                  }

                  if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                                     szNewViewName, 0 ) < zCURSOR_SET )
                  {
                     MessageSend( vSubtask, "", "Change View Mapping",
                                  "New View Name is not a Registered View in this XSLT.",
                                  zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
                     SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
                     DropView( vOldViewName );
                     DropView( vNewViewName );
                     bChange = FALSE;
                     break;
                  }

                  // Go to execute change view mapping for this control and its children.
                  fnChangeMappingViewR( vXSLT, vOldViewName, vNewViewName, TRUE, TRUE );
                  bChange = TRUE;

                  DropView( vNewViewName );
                  DropView( vOldViewName );


                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
               }

               if ( bChange )
               {
                  TraceLineS( "MigrateXSLTs modified dialog: ", pchDlgTag );
                  CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
                  SetNameForView( vXSLT, "Migrated", vSubtask, zLEVEL_TASK );
                  MessageSend( vSubtask, "PN00202", "XSLT Migration",
                               "Check mapping for view named Migrated",
                               zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
               }
               else
                  TraceLineS( "MigrateXSLTs unmodified dialog: ", pchDlgTag );
            }

            return( 0 );
         }
         else
         if ( (lCheckFlags & zCHECK_ADD_GROUPBOX) && zstrcmp( g_szDlgTag, pchDlgTag ) == 0 )
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               zPCHAR pchCtrlTag;

               sprintf_s( szMsg, zsizeof( szMsg ), "Checking XSLT: %s for Migration", pchDlgTag );
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == 0 )
               {
                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
                  nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
                  while ( nRC == 0 )
                  {
                     GetAddrForAttribute( &pchCtrlTag, vXSLT, "Control", "Tag" );
                     if ( zstrcmp( g_szGroupBox, pchCtrlTag ) == 0 )
                     {
                        zCHAR szCtrlTag[ 64 ];
                        zVIEW vGroupBox;
                        zVIEW vNextControl;
                        zLONG lPosX;
                        zLONG lPosY;
                        zLONG lSizeY;
                        zLONG lKey;
                        zLONG lReposX = 0;
                        zLONG lReposY = 0;

                        CreateViewFromView( &vGroupBox, vXSLT );
                        CreateViewFromView( &vNextControl, vXSLT );

                        CreateMetaEntity( vSubtask, vGroupBox, "Control", zPOS_AFTER );
                        strcpy_s( szCtrlTag, zsizeof( szCtrlTag ), pchWndTag );
                        strcat_s( szCtrlTag, zsizeof( szCtrlTag ), "Section" );
                        SetAttributeFromString( vGroupBox, "Control", "Tag", szCtrlTag );
                        strcpy_s( szCtrlTag, zsizeof( szCtrlTag ), pchWndTag );
                        strcat_s( szCtrlTag, zsizeof( szCtrlTag ), " Section" );
                        SetAttributeFromString( vGroupBox, "Control", "Text", szCtrlTag );
                        SetMatchingAttributesByName( vGroupBox, "Control",
                                                     vXSLT, "Control", zSET_NULL );

                        SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", zGROUPBOX_CONTROL, "" );
                        IncludeSubobjectFromSubobject( vGroupBox, "ControlDef",
                                                       vPE,       "ControlDef", zPOS_AFTER );
                        GetIntegerFromAttribute( &lPosY, vXSLT, "Control", "PSDLG_Y" );
                        GetIntegerFromAttribute( &lSizeY, vXSLT, "Control", "SZDLG_Y" );
                        lPosY += lSizeY + 5;
                        SetAttributeFromInteger( vGroupBox, "Control", "PSDLG_Y", lPosY );
                        SetViewToSubobject( vGroupBox, "CtrlCtrl" );
                        nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
                        nRC = SetCursorNextEntity( vXSLT, "Control", 0 );  // both set cursors next intended
                        while ( nRC == 0 )
                        {
                           GetIntegerFromAttribute( &lKey, vXSLT, "ControlDef", "Key" );
                           if ( lKey == zGROUPBOX_CONTROL )
                           {
                              break;  // done with this window
                           }

                           GetIntegerFromAttribute( &lPosX, vXSLT, "Control", "PSDLG_X" );
                           GetIntegerFromAttribute( &lPosY, vXSLT, "Control", "PSDLG_Y" );
                           if ( lReposX == 0 )
                              lReposX = lPosX - 6;

                           if ( lReposY == 0 )
                              lReposY = lPosY - 14;

                           lPosX -= lReposX;
                           lPosY -= lReposY;
                           SetAttributeFromInteger( vXSLT, "Control", "PSDLG_X", lPosX );
                           SetAttributeFromInteger( vXSLT, "Control", "PSDLG_Y", lPosY );
                           SetViewFromView( vNextControl, vXSLT );
                           nRC = SetCursorNextEntity( vNextControl, "Control", 0 );

                           MoveSubobject( vGroupBox, "Control",
                                          vXSLT, "Control",
                                          zPOS_AFTER, zREPOS_NEXT );
                           SetViewFromView( vXSLT, vNextControl );
                           bChange = TRUE;
                        }

                        break;
                     }

                     nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
                  }

                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
               }

               if ( bChange )
               {
                  TraceLineS( "MigrateXSLTs modified dialog: ", pchDlgTag );
                  CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
               }
               else
                  TraceLineS( "MigrateXSLTs unmodified dialog: ", pchDlgTag );
            }

            return( 0 );
         }
         else
         if ( lCheckFlags && (lCheckFlags & zCHECK_TIMESTAMP_ONLY) == 0 )
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg,
                            zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               sprintf_s( szMsg, zsizeof( szMsg ), "Checking XSLT: %s for Migration", pchDlgTag );
               TraceLineS( szMsg, "" );
               MB_SetMessage( vSubtask, 1, szMsg );
               OrderEntityForView( vXSLT, "Window", "Tag A" );
               nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
                  if ( lCheckFlags & (zCHECK_REPORT_EVENTS | zCHECK_CHANGE_WAB) )
                  {
                     zLONG  lType;

                     nRC = SetCursorFirstEntity( vXSLT, "WndEvent", 0 );
                     while ( nRC >= zCURSOR_SET )
                     {
                        GetIntegerFromAttribute( &lType, vXSLT, "WndEvent", "Type" );
                        TraceLine( "Event Type: 0x%08x   for Dlg.Wnd: %s.%s",
                                   lType, pchDlgTag, pchWndTag );

                        nRC = SetCursorNextEntity( vXSLT, "WndEvent", 0 );
                     }
                  }

                  if ( CheckMigrateCtrls( vXSLT, vPE, lCheckFlags,
                                          pchDlgTag, pchWndTag, lIncr ) )
                  {
                     bChange = TRUE;
                  }

                  nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
               }

               if ( bChange )
               {
                  TraceLineS( "MigrateXSLTs modified dialog: ", pchDlgTag );
                  CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
               }
               else
                  TraceLineS( "MigrateXSLTs unmodified dialog: ", pchDlgTag );
            }
         }
         else
         if ( bSaveAll )
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg,
                            zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               sprintf_s( szMsg, zsizeof( szMsg ), "Saving XSLT: %s", pchDlgTag );
               MB_SetMessage( vSubtask, 1, szMsg );
               TraceLineS( "MigrateXSLTs (all) dialog: ", pchDlgTag );
               CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
            }
         }
         else
         {
            if ( !ComponentIsCheckedOut( vSubtask, vXSLT, zSOURCE_XSLT_META ) )
            {
               strcpy_s( szMsg, zsizeof( szMsg ), "Unable to save XSLT '" );
               strcat_s( szMsg, zsizeof( szMsg ), pchDlgTag );
               strcat_s( szMsg, zsizeof( szMsg ), "' because it is not checked out" );
               MessageSend( vSubtask, "PN00202", "XSLT Maintenance",
                            szMsg, zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
            }
            else
            {
               zCHAR  szFileSpec1[ zMAX_FILESPEC_LTH + 1 ];
               zCHAR  szFileSpec2[ zMAX_FILESPEC_LTH + 1 ];
               zPCHAR pch1;
               zPCHAR pch2;
               zLONG  lFilePWD = -1;
               zLONG  lFileXWD = -1;
               zSHORT nLth;

               sprintf_s( szMsg, zsizeof( szMsg ), "Checking XSLT: %s for synchronization", pchDlgTag );
               TraceLineS( szMsg, "" );
               MB_SetMessage( vSubtask, 1, szMsg );
               GetStringFromAttribute( szFileSpec2, zsizeof( szFileSpec2 ), vTaskLPLR,
                                       "LPLR", "ExecDir" );
               nLth = (zSHORT) zstrlen( szFileSpec2 );
               if ( nLth && szFileSpec2[ nLth - 1 ] != '\\' )
                  szFileSpec2[ nLth++ ] = '\\';

               strcpy_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, pchDlgTag );
               strcat_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, ".xwd" );

               GetStringFromAttribute( szFileSpec1, zsizeof( szFileSpec1 ), vTaskLPLR, "LPLR", "MetaSrcDir" );
               nLth = (zSHORT) zstrlen( szFileSpec1 );
               if ( nLth && szFileSpec1[ nLth - 1 ] != '\\' )
                  szFileSpec1[ nLth++ ] = '\\';

               strcpy_s( szFileSpec1 + nLth, pchDlgTag );
               nLth += (zSHORT) zstrlen( pchDlgTag );
               strcpy_s( szFileSpec1 + nLth, ".pwd" );
               lFilePWD = SysOpenFile( vSubtask, szFileSpec1, COREFILE_READ );
               lFileXWD = SysOpenFile( vSubtask, szFileSpec2, COREFILE_READ );
               strcpy_s( szFileSpec2, zsizeof( szFileSpec2 ), szFileSpec1 );
               strcpy_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, ".tmp" );
               if ( lFilePWD >= 0 &&
                    (lFileXWD < 0 ||
                     (lFileXWD >= 0 &&
                      SysReadLine( vSubtask, &pch1, lFilePWD ) >= 0 &&
                      SysReadLine( vSubtask, &pch2, lFileXWD ) >= 0 &&
                      fnLocateXSLTDate( pch1 ) == 0 &&
                      fnLocateXSLTDate( pch2 ) == 0 &&
                      (zstrcmp( pch1, pch2 ) > 0 || bSaveAll))) && // ==> out of sync
                    SysCopyFile( vSubtask, szFileSpec1, szFileSpec2, TRUE ) > 0 )
               {
                  SysCloseFile( vSubtask, lFilePWD, 0 );
                  lFilePWD = -1;
                  if ( lFileXWD >= 0 )
                     SysCloseFile( vSubtask, lFileXWD, 0 );

                  lFileXWD = -1;

                  strcpy_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, ".pw~" );
                  SysRenameFile( vSubtask, szFileSpec1, szFileSpec2, TRUE );
                  strcpy_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, ".tmp" );
                  SysRenameFile( vSubtask, szFileSpec2, szFileSpec1, TRUE );

                  TraceLineS( "MigrateXSLTs synchronize dialog: ", pchDlgTag );
                  CommitMetaOI( vSubtask, vXSLT, zSOURCE_XSLT_META );
                  strcpy_s( szFileSpec2 + nLth, zsizeof( szFileSpec2 ) - nLth, ".pw~" );
                  SysRenameFile( vSubtask, szFileSpec2, szFileSpec1, TRUE );
               }
               else
               {
                  TraceLineS( "MigrateXSLTs unmodified dialog: ", pchDlgTag );
               }

               if ( lFilePWD >= 0 )
                  SysCloseFile( vSubtask, lFilePWD, 0 );

               if ( lFileXWD >= 0 )
                  SysCloseFile( vSubtask, lFileXWD, 0 );
            }
         }

         DropObjectInstance( vXSLT );
      }
      else
      {
         TraceLineI( "MigrateXSLTs unexpected failure to open dialog in list ", k );
      }

      nRC = SetCursorNextEntity( vXSLTList, "W_MetaDef", 0 );
   }

   DropView( vXSLTList );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndIncrement( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncrX;
   zLONG  lIncrY;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrX = atol( szBuffer );
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrY = atol( szBuffer );
      lIncr = MAKELONG( abs( lIncrX ), abs( lIncrY ) );
      if ( lIncr )
      {
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_INCREMENT_POS_SIZEXY;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndIncrX( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the X percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         lIncr = MAKELONG( lCheckFlags, 0 );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_INCREMENT_POS_SIZEX;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndIncrY( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the Y percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         lIncr = MAKELONG( 0, lCheckFlags );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_INCREMENT_POS_SIZEY;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndDecrement( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncrX;
   zLONG  lIncrY;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrX = atol( szBuffer );
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrY = atol( szBuffer );
      lIncr = MAKELONG( abs( lIncrX ), abs( lIncrY ) );
      if ( lIncr )
      {
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_DECREMENT_POS_SIZEXY;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndDecrX( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the X percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         lIncr = MAKELONG( lCheckFlags, 0 );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_DECREMENT_POS_SIZEX;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeWndDecrY( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the Y percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         lIncr = MAKELONG( 0, lCheckFlags );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );
         lCheckFlags = zCHECK_DECREMENT_POS_SIZEY;
         ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgIncrement( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncrX;
   zLONG  lIncrY;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrX = atol( szBuffer );
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrY = atol( szBuffer );
      lIncr = MAKELONG( abs( lIncrX ), abs( lIncrY ) );
      if ( lIncr )
      {
         zSHORT nRC;

         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );

         lCheckFlags = zCHECK_INCREMENT_POS_SIZEXY;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgIncrX( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the X percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         zSHORT nRC;

         lIncr = MAKELONG( lCheckFlags, 0 );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );

         lCheckFlags = zCHECK_INCREMENT_POS_SIZEX;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags,
                                        lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgIncrY( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the Y percentage increment "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         zSHORT nRC;

         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         lIncr = MAKELONG( 0, lCheckFlags );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );

         lCheckFlags = zCHECK_INCREMENT_POS_SIZEY;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgDecrement( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncrX;
   zLONG  lIncrY;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the X percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrX = atol( szBuffer );
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the Y percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lIncrY = atol( szBuffer );
      lIncr = MAKELONG( abs( lIncrX ), abs( lIncrY ) );
      if ( lIncr )
      {
         zSHORT nRC;

         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );
         lCheckFlags = zCHECK_DECREMENT_POS_SIZEXY;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgDecrX( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the X percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         zSHORT nRC;

         lIncr = MAKELONG( lCheckFlags, 0 );
         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );

         lCheckFlags = zCHECK_DECREMENT_POS_SIZEX;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags, lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ProportionallyResizeDlgDecrY( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 32 ];
   zVIEW  vXSLT;
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zLONG  lCheckFlags;
   zLONG  lIncr;

   if ( GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      szBuffer[ 0 ] = '2';
      szBuffer[ 1 ] = '4';
      szBuffer[ 2 ] = 0;
      if ( OperatorPromptForInput( vSubtask, "XSLT Migration",
                                   "Specify the Y percentage decrement "
                                      "... 0 to stop resize", FALSE,
                                   szBuffer, zsizeof( szBuffer ) ) != IDOK )
      {
         return( 0 );
      }

      lCheckFlags = atol( szBuffer );
      if ( lCheckFlags > 0 )
      {
         zSHORT nRC;

         GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );
         lIncr = MAKELONG( 0, lCheckFlags );
         CreateViewFromViewForTask( &vXSLT, vXSLT, 0 );

         lCheckFlags = zCHECK_DECREMENT_POS_SIZEY;
         nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
            ProportionallyResizeWindow( vXSLT, lCheckFlags,
                                        lIncr, pchDlgTag, pchWndTag );
            nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnChangeCtrlMapView( zVIEW vControl,
                     zVIEW vNewViewName,
                     zLONG lOriginalViewZKey )
{
// zCHAR  szEntity[ 33 ];
// zCHAR  szAttribute[ 33 ];
// zLONG  lZKeyEntity;
// zLONG  lZKeyAttribute;
   zSHORT nRC;

   for ( nRC = SetCursorFirstEntity( vControl, "CtrlMap", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vControl, "CtrlMap", "" ) )
   {
      if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
      {
         // Check if the CtrlMapView we are on is the same as the view for which
         // we're to make the change. If not skip.
         if ( CompareAttributeToInteger( vControl, "CtrlMapView", "ZKey",
                                         lOriginalViewZKey ) == 0 )
         {

            if ( CompareAttributeToInteger( vNewViewName, "ViewObjRef", "ZKey",
                                            lOriginalViewZKey ) != 0 )
            {
#if 0
               // Set up to reset the Entity and Attribute since the ZKeys differ
               // from one ViewObjRef to another.
               if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Attribute" ) == 0 )
               {
                  GetIntegerFromAttribute( &lZKeyEntity, vControl, "CtrlMapRelatedEntity", "ZKey" );
                  GetStringForAttribute( szEntity, vControl, "CtrlMapRelatedEntity", "Name" );
                  GetIntegerFromAttribute( &lZKeyAttribute, vControl, "CtrlMapLOD_Attribute", "ZKey" );
                  GetStringForAttribute( szAttribute, vControl, "CtrlMapLOD_Attribute", "Name" );
               }
               else
               {
                  szEntity[ 0 ] = 0;
                  szAttribute[ 0 ] = 0;
                  lZKeyEntity = 0;
                  lZKeyAttribute = 0;
               }

#endif

               ExcludeEntity( vControl, "CtrlMapView", zREPOS_NONE );
               IncludeSubobjectFromSubobject( vControl,  "CtrlMapView",
                                              vNewViewName, "ViewObjRef", zPOS_AFTER );

#if 0
               // When we do this, we need to reset the Entity and Attribute since the
               // ZKeys differ from one ViewObjRef to another.

               IncludeSubobjectFromSubobject( vTgtC, "CtrlMapLOD_Attribute",
                                              vLOD, "LOD_Attribute", zPOS_AFTER );

               // Select current mapping in LOD.
               if ( CheckExistenceOfEntity( vXSLTC, "CtrlMapLOD_Attribute" ) == zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lZKey, vXSLTC,
                                           "CtrlMapRelatedEntity", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                                      "ZKey", lZKey, "" ) != zCURSOR_SET )
                  {

                     GetAddrForAttribute( &pchEntity, vXSLTC, "CtrlMapRelatedEntity", "Name" );
                     SetCursorFirstEntityByString( vLOD, "LOD_Entity",
                                                   "Name", pchEntity, "" );
                     TraceLineS( "LOD Entity not located by ZKey: ", pchEntity );
                  }

                  SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
                  GetIntegerFromAttribute( &lZKey, vXSLTC,
                                           "CtrlMapLOD_Attribute", "ZKey" );
                  if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute",
                                                      "ZKey", lZKey, "" ) != zCURSOR_SET )
                  {

                     GetAddrForAttribute( &pchAttribute, vXSLTC, "CtrlMapER_Attribute", "Name" );
                     SetCursorFirstEntityByString( vLOD, "ER_Attribute",
                                                   "Name", pchAttribute, "LOD_Entity" );
                     TraceLineS( "LOD Attribute not located by ZKey: ", pchAttribute );
                  }

                  SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );
               }
#endif

            }
         }
      }
   }
}

zSHORT
fnChangeMappingViewR( zVIEW vXSLT,
                      zVIEW vOldViewName,
                      zVIEW vNewViewName,
                      zBOOL bAllCtrlsAtLevel,
                      zBOOL bRecurse )
{
   zLONG  lOriginalViewZKey;
   zSHORT nRC;

   // Get Original View ID.
   GetIntegerFromAttribute( &lOriginalViewZKey, vOldViewName, "ViewObjRef", "ZKey" );

   // Loop through all Controls at this level.
   if ( bAllCtrlsAtLevel )
      nRC = SetCursorFirstEntity( vXSLT, "Control", "" );
   else
      nRC = CheckExistenceOfEntity( vXSLT, "Control" );

   while ( nRC >= zCURSOR_SET )
   {
      // Change mapping view of each view that is identified by original view ID.
      fnChangeCtrlMapView( vXSLT, vNewViewName, lOriginalViewZKey );

      // Process any recursive sub controls.
      if ( bRecurse && CheckExistenceOfEntity( vXSLT, "CtrlCtrl" ) == 0 )
      {
         SetViewToSubobject( vXSLT, "CtrlCtrl" );
         fnChangeMappingViewR( vXSLT, vOldViewName, vNewViewName, TRUE, TRUE );
         ResetViewFromSubobject( vXSLT );
      }

      if ( bAllCtrlsAtLevel )
         nRC = SetCursorNextEntity( vXSLT, "Control", "" );
      else
         nRC = -1;
   }

   return( 0 );
}

// Recursive function to go through child controls.  vControl is positioned
// on the original Ctrl, vWnd is positioned on the Ctrl in a different window.
void
fnSetPsSzClassR( zVIEW vWnd, zVIEW vControl, zCPCHAR cpcDlg, zCPCHAR cpcWnd,
                 zBOOL bRecurse, zBOOL bSiblings )
{
   zPCHAR pchCtrlTag;
   zPCHAR pchCtrlCtrlTag;
   zPCHAR pchClass;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zSHORT nRC;

   // Get the Tag of currently selected control.
   GetAddrForAttribute( &pchCtrlTag, vControl, "Control", "Tag" );

   GetIntegerFromAttribute( &lPosX, vControl, "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lPosY, vControl, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lSizeX, vControl, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lSizeY, vControl, "Control", "SZDLG_Y" );
   GetAddrForAttribute( &pchClass, vControl, "Control", "CSS_Class" );
   SetAttributeFromInteger( vWnd, "Control", "PSDLG_X", lPosX );
   SetAttributeFromInteger( vWnd, "Control", "PSDLG_Y", lPosY );
   SetAttributeFromInteger( vWnd, "Control", "SZDLG_X", lSizeX );
   SetAttributeFromInteger( vWnd, "Control", "SZDLG_Y", lSizeY );
   SetAttributeFromString( vWnd, "Control", "CSS_Class", pchClass );

// TraceLine( "Setting Dlg.Wnd.Ctrl: %s.%s.%s  Pos/Size: %d,%d/%d,%d  Class: %s",
//            cpcDlg, cpcWnd, pchCtrlTag,
//            lPosX, lPosY, lSizeX, lSizeY, pchClass );

   if ( bRecurse && CheckExistenceOfEntity( vControl, "CtrlCtrl" ) == 0 )
   {
      SetViewToSubobject( vControl, "CtrlCtrl" );
      SetViewToSubobject( vWnd, "CtrlCtrl" );

      GetAddrForAttribute( &pchCtrlCtrlTag, vControl, "Control", "Tag" );
      if ( SetCursorFirstEntityByString( vWnd, "Control",
                                         "Tag", pchCtrlCtrlTag, 0 ) >= zCURSOR_SET )
      {
         // Recurse through child controls and siblings.
         fnSetPsSzClassR( vWnd, vControl, cpcDlg, cpcWnd, TRUE, TRUE );
      }

      ResetViewFromSubobject( vWnd );
      ResetViewFromSubobject( vControl );
   }

   if ( bSiblings )
   {
      nRC = SetCursorFirstEntity( vControl, "Control", 0 );
      while ( nRC == 0 )
      {
         // Don't process the original Ctrl.
         GetAddrForAttribute( &pchCtrlCtrlTag, vControl, "Control", "Tag" );
         if ( zstrcmp( pchCtrlTag, pchCtrlCtrlTag ) != 0 )
         {
            // We've got to make sure the vWnd has a Ctrl with the same Tag
            // as the Ctrl currently with cursor position in vControl.
            if ( SetCursorFirstEntityByString( vWnd, "Control", "Tag", pchCtrlCtrlTag, 0 ) >= zCURSOR_SET )
            {            // Recurse through child controls but not siblings.
               fnSetPsSzClassR( vWnd, vControl, cpcDlg, cpcWnd, TRUE, FALSE );
            }
         }

         nRC = SetCursorNextEntity( vControl, "Control", 0 );
      }

      SetCursorFirstEntityByString( vControl, "Control", "Tag", pchCtrlTag, 0 );
   }
}

void
fnDisplayControlsR( zVIEW vXSLT, zCPCHAR cpcDlg, zCPCHAR cpcWnd )
{
   zPCHAR pchCtrlTag;
   zPCHAR pchClass;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zSHORT nRC;

   // Get the Tag of currently selected control.
   GetAddrForAttribute( &pchCtrlTag, vXSLT, "Control", "Tag" );

   GetIntegerFromAttribute( &lPosX, vXSLT, "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lPosY, vXSLT, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lSizeX, vXSLT, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lSizeY, vXSLT, "Control", "SZDLG_Y" );
   GetAddrForAttribute( &pchClass, vXSLT, "Control", "CSS_Class" );
   TraceLine( "Dlg.Wnd.Ctrl: %s.%s.%s  Pos/Size: %d,%d/%d,%d  Class: %s",
              cpcDlg, cpcWnd, pchCtrlTag, lPosX, lPosY, lSizeX, lSizeY, pchClass );

   if ( CheckExistenceOfEntity( vXSLT, "CtrlCtrl" ) == 0 )
   {
      SetViewToSubobject( vXSLT, "CtrlCtrl" );

      nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
      while ( nRC == 0 )
      {
         fnDisplayControlsR( vXSLT, cpcDlg, cpcWnd );
         nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
      }

      ResetViewFromSubobject( vXSLT );
   }
}

void
DisplayControls( zVIEW vXSLT, zCPCHAR cpcText1, zCPCHAR cpcText2 )
{
   zPCHAR pchDlgTag;
   zPCHAR pchWndTag;
   zSHORT nRC;

   TraceLineS( "", "" );
   TraceLineS( cpcText1, cpcText2 );

   CreateViewFromView( &vXSLT, vXSLT );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   // Get the Tag of current dialog.
   GetAddrForAttribute( &pchDlgTag, vXSLT, "XSLT", "Tag" );

   nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
   while ( nRC == 0 )
   {
      // Get the Tag of current window.
      GetAddrForAttribute( &pchWndTag, vXSLT, "Window", "Tag" );
      nRC = SetCursorFirstEntity( vXSLT, "Control", 0 );
      while ( nRC == 0 )
      {
         fnDisplayControlsR( vXSLT, pchDlgTag, pchWndTag );
         nRC = SetCursorNextEntity( vXSLT, "Control", 0 );
      }
      nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
   }

   DropView( vXSLT );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    PainterSelectedCallback
//
// PURPOSE:  This function is the function called by the painter for each
//           selected control.
//
//           lType - Reason for callback:
//                    0x00000001 - change View
//                    0x00000002 - change Entity
//                    0x00000004 - change Attribute
//                    0x00000010 - change Tag (in place)
//                    0x00000020 - change Text (in place)
//                    0x00000040 - change Font Face Name (in place)
//                    0x00000080 - change Font Size (in place)
//                    0x00000100 - change Pos of Control with same ...
//                    0x00000200 - change Size of Control with same ...
//                    0x00000400 - change Class of Control with same ...
//                    0x00000700 - change Pos/Size/Class of Control with same
//                                 Tag for all Windows for XSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PainterSelectedCallback( zVIEW   vSubtask, // view to painter window
                         zVIEW   vXSLT,
                         zVIEW   vLOD,
                         zVIEW   vControl, // do not reposition this view
                         zPVOID  pCtrl,    // painter C++ pointer to ctrl
                         zLONG   lType,    // type of call
                         zPVOID  pvData,   // this data can get changed
                         zLONG   lFlags )
{
   zVIEW  vOldName;
   zVIEW  vNewName;
// zLONG  lRC;

   if ( MiGetUpdateForView( vXSLT ) == 0 )
      return( 0 );

   if ( lType & 0x00000007 ) // change View and/or Entity and/or Attribute
   {
      zPCHAR pchDotEntity = 0;
      zPCHAR pchDotAttribute = 0;
      zPCHAR pchDot;
      zPCHAR pchNewViewName = (zPCHAR) pvData;
      zPCHAR pchEntityName = 0;
      zPCHAR pchAttribName = 0;
      zLONG  lZKey;

      pchDot = zstrchr( pchNewViewName, '.' );
      if ( pchDot )
      {
         pchDotEntity = pchDot;
         *pchDot++ = 0;
         pchEntityName = pchDot;
         pchDot = zstrchr( pchEntityName, '.' );
         if ( pchDot )
         {
            pchDotAttribute = pchDot;
            *pchDot++ = 0;
            pchAttribName = pchDot;
         }
      }

      if ( pchNewViewName && *pchNewViewName && (lType & 0x00000001) ) // change View
      {
         CreateViewFromView( &vOldName, vXSLT );
         if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
            GetIntegerFromAttribute( &lZKey, vControl, "CtrlMapView", "ZKey" );
         else
            lZKey = 0;

         CreateViewFromView( &vNewName, vXSLT );
         if ( (lZKey == 0 ||
               SetCursorFirstEntityByInteger( vOldName, "ViewObjRef", "ZKey",
                                              lZKey, 0 ) >= zCURSOR_SET) &&
              SetCursorFirstEntityByString( vNewName, "ViewObjRef", "Name",
                                            pchNewViewName, 0 ) >= zCURSOR_SET )
         {
            // Go to execute change view mapping at each Control level.
            fnChangeMappingViewR( vControl, vOldName, vNewName, FALSE, TRUE );
         }

         DropView( vOldName );
         DropView( vNewName );
      }

      if ( pchAttribName && *pchAttribName && (lType & 0x00000004) ) // change Attribute
      {
         zBOOL bInclude = TRUE;

         // We assume position has already been established on correct LOD_Entity.
         if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Attribute" ) == 0 )
         {
            if ( CompareAttributeToAttribute( vControl, "CtrlMapLOD_Attribute", "ZKey",
                                              vLOD, "LOD_Attribute", "ZKey" ) == 0 )
            {
               bInclude = FALSE;
            }
            else
            {
               ExcludeEntity( vControl, "CtrlMapLOD_Attribute", zREPOS_NONE );
            }
         }

         if ( bInclude )
         {
            IncludeSubobjectFromSubobject( vControl, "CtrlMapLOD_Attribute",
                                           vLOD, "LOD_Attribute", zPOS_AFTER );
         // IncludeSubobjectFromSubobject( vControl, "CtrlMapRelatedEntity",
         //                                vLOD, "LOD_Entity", zPOS_AFTER );
         }
      }
      else
      if ( pchEntityName && *pchEntityName && (lType & 0x00000002) ) // change Entity
      {
         zBOOL bInclude = TRUE;

         // We assume position has already been established on correct LOD_Entity.
         if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) == 0 )
         {
            if ( CompareAttributeToAttribute( vControl, "CtrlMapLOD_Entity", "ZKey",
                                              vLOD, "LOD_Entity", "ZKey" ) == 0 )
            {
               bInclude = FALSE;
            }
            else
            {
               ExcludeEntity( vControl, "CtrlMapLOD_Entity", zREPOS_NONE );
            }
         }

         if ( bInclude )
         {
            IncludeSubobjectFromSubobject( vControl, "CtrlMapLOD_Entity",
                                           vLOD, "LOD_Entity", zPOS_AFTER );
         }
      }

      if ( pchDotEntity )
         *pchDotEntity = '.';

      if ( pchDotAttribute )
         *pchDotAttribute = '.';
   }
   else
   if ( lType & 0x00000070 ) // change Tag, Text, Font
   {
      if ( lType & 0x00000010 )
         ShowInplaceControlOverCtrl( vSubtask, vControl, pCtrl, "InplaceTag" );
      else
      if ( lType & 0x00000020 )
         ShowInplaceControlOverCtrl( vSubtask, vControl, pCtrl, "InplaceText" );
      else
      if ( lType & 0x00000040 )
         ShowInplaceControlOverCtrl( vSubtask, vControl, pCtrl, "InplaceFontFace" );
      else
      if ( lType & 0x00000080 )
         ShowInplaceControlOverCtrl( vSubtask, vControl, pCtrl, "InplaceFontSize" );
   }
   else
   if ( lType & 0x00000700 ) // change Position, Size, Class for all Controls
   {                         // with the same Tag for all Windows in XSLT
      zVIEW  vWnd;
      zPCHAR pchDlg;
      zPCHAR pchWnd;
      zPCHAR pchCtrlTag;
      zPCHAR pchWndCurr;
      zSHORT nRC;

      // Get the Tag of currently selected control.
      GetAddrForAttribute( &pchCtrlTag, vControl, "Control", "Tag" );

      // So we can modify the XSLT and not worry about the consequences.
      CreateViewFromViewForTask( &vWnd, vControl, 0 );
      while ( ResetViewFromSubobject( vWnd ) == 0 )
      {
      }

      CreateViewFromViewForTask( &vControl, vControl, 0 );  // protect original vControl
      GetAddrForAttribute( &pchDlg, vWnd, "XSLT", "Tag" );
      GetAddrForAttribute( &pchWnd, vWnd, "Window", "Tag" );

      nRC = SetCursorFirstEntity( vWnd, "Window", 0 );
      while ( nRC == zCURSOR_SET )
      {
         GetAddrForAttribute( &pchWndCurr, vWnd, "Window", "Tag" );
         if ( zstrcmp( pchWnd, pchWndCurr ) != 0 )
         {
            nRC = SetEntityCursor( vWnd, "Control", "Tag",
                                   zPOS_FIRST | zQUAL_STRING | zRECURS,
                                   pchCtrlTag, 0, 0, 0, "Window", 0 );
            if ( nRC > zCURSOR_UNCHANGED )
            {
               // Recurse through child controls but not siblings.
               fnSetPsSzClassR( vWnd, vControl, pchDlg, pchWndCurr, TRUE, FALSE );
            }
         }

         // The above SetEntityCursor could have put us down in a subobject.
         while ( ResetViewFromSubobject( vWnd ) == 0 )
         {
         }

         nRC = SetCursorNextEntity( vWnd, "Window", 0 );
      }

      DropView( vWnd );
      DropView( vControl );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetCtrlSizPosClasToSelectThruDlg( zVIEW vSubtask )
{
   zCHAR  szMsg[ 256 ];
   zPCHAR pchDlg;
   zVIEW  vXSLT;
   zLONG  lRC = -1;

// GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
   {
      // Prompt operator to ensure ALL windows are to be generated.
      GetAddrForAttribute( &pchDlg, vXSLT, "XSLT", "Tag" );
      sprintf_s( szMsg, zsizeof( szMsg ), "Reset Control Pos/Size/Class for ALL Windows in XSLT: %s?", pchDlg );
      if ( OperatorPrompt( vSubtask, "Reset Pos/Size/Class",
                           szMsg, 1, zBUTTONS_YESNO,
                           zRESPONSE_YES, zICON_QUESTION ) == zRESPONSE_NO )
      {
         return( -1 );
      }

   // DisplayControls( vXSLT, "Changing XSLT BEFORE UPDATE ALL:", pchDlg );
      fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );

   // DisplayControls( vXSLT, "Changing XSLT: ", pchDlg );
      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 0x00000700, 0, 0 );

   // DisplayControls( vXSLT, "Changed XSLT: ", pchDlg );
      fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   // DisplayControls( vXSLT, "Changed XSLT after REPAINT ALL: ", pchDlg );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateTagInPlace( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
   {
      zCHAR szOptionTag[ 65 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateTagInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 0x00000010,
                                            szOptionTag, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateTextInPlace( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateTextInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 0x00000020, 0, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateFontFaceNameInPlace( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdatFontFaceNameInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 0x00000040, 0, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateFontSizeInPlace( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   if ( vXSLT )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateFontSizeInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 0x00000080, 0, 0 );
   }

   return( (zSHORT) lRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZXSLTDD_TargetSpecification
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZXSLTDD_TargetSpecification( zVIEW vSubtask )
{
   zVIEW vMeta;
   zVIEW vTaskLPLR;
   zVIEW vTZXSLTL;
   zLONG lMetaZKey;

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   // if no selection found, select current meta
   if ( CheckExistenceOfEntity( vTaskLPLR, "TargetExecutable" ) != zCURSOR_SET ||
        SetCursorFirstSelectedEntity( vTaskLPLR, "TargetExecutable",
                                      "" ) >= zCURSOR_SET )
   {
      return( -1 );
   }

   CreateViewFromViewForTask( &vMeta, vTZXSLTL, 0 );
   ResetView( vMeta );

   GetIntegerFromAttribute( &lMetaZKey, vMeta, "XSLT", "ZKey" );

   if ( SetCursorFirstEntityByInteger( vTaskLPLR, "Meta", "ZKey",
                                       lMetaZKey, "LPLR" ) >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTaskLPLR, "TargetExecutable", TRUE );
   }

   DropView( vMeta );
   return( 0 );

} // TZXSLTDD_TargetSpecification

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ChangeSelectedViewMapping( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vXSLT && vTZZOVEAO )
   {
      zCHAR szVEA[ 3 * 33 ];

      GetStringFromAttribute( szVEA, zsizeof( szVEA ), vTZZOVEAO, "ViewObjRef", "Name" );

      lRC = CallPainterForSelectedControls( vSubtask, "tzxsltdd",
                                            "PainterSelectedCallback",
                                            vXSLT, 0, 1, szVEA, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetSelectedEntityMapping( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vXSLT && vTZZOVEAO )
   {
      zVIEW vCM_List;
      zVIEW vLOD;
      zLONG lZKey;
      zCHAR szVEA[ 3 * 33 ];
      zSHORT nLth;

      GetStringFromAttribute( szVEA, zsizeof( szVEA ), vTZZOVEAO, "ViewObjRef", "Name" );
      nLth = (zSHORT) zstrlen( szVEA );
      szVEA[ nLth++ ] = '.';
      GetStringFromAttribute( szVEA + nLth, zsizeof( szVEA ) - nLth, vTZZOVEAO, "LOD_Entity", "Name" );
      GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD", "ZKey" );
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );

      GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD_Entity", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, "" ) == zCURSOR_SET )
      {
         lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                               "PainterSelectedCallback",
                                               vXSLT, vLOD, 3, szVEA, 0 );
      }

      DropMetaOI( vSubtask, vLOD );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetSelectedAttributeMapping( zVIEW vSubtask )
{
   zVIEW vXSLT;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vXSLT, "vXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vXSLT && vTZZOVEAO )
   {
      zVIEW vCM_List;
      zVIEW vLOD;
      zLONG lZKey;
      zLONG lZKeyA;
      zCHAR szVEA[ 3 * 33 ];
      zSHORT nLth;

      GetStringFromAttribute( szVEA, zsizeof( szVEA ), vTZZOVEAO, "ViewObjRef", "Name" );
      nLth = (zSHORT) zstrlen( szVEA );
      szVEA[ nLth++ ] = '.';
      GetStringFromAttribute( szVEA + nLth, zsizeof( szVEA ) - nLth, vTZZOVEAO, "LOD_Entity", "Name" );
      nLth = (zSHORT) zstrlen( szVEA );
      szVEA[ nLth++ ] = '.';
      GetStringFromAttribute( szVEA + nLth, zsizeof( szVEA ) - nLth, vTZZOVEAO, "ER_Attribute", "Name" );
      GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD", "ZKey" );
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );

      GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD_Entity", "ZKey" );
      GetIntegerFromAttribute( &lZKeyA, vTZZOVEAO, "LOD_Attribute", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, "" ) == zCURSOR_SET &&
           SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute", "ZKey", lZKeyA, "" ) == zCURSOR_SET )
      {
         lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                               "PainterSelectedCallback",
                                               vXSLT, vLOD, 7, szVEA, 0 );
      }

      DropMetaOI( vSubtask, vLOD );
   }

   return( (zSHORT) lRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnChangeMappingView( zVIEW vSubtask )
{
   zVIEW  vOldViewName;
   zVIEW  vNewViewName;
   zVIEW  vTZXSLTL;
   zVIEW  vXSLT;
   zCHAR  szOriginalViewName[ 33 ];
   zCHAR  szNewViewName[ 33 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vOldViewName, vTZXSLTL, 0 );
   GetVariableFromAttribute( szOriginalViewName, 0, 'S', 32,
                             vOldViewName, "XSLT", "wOriginalViewName", "", 0 );
   if ( SetCursorFirstEntityByString( vOldViewName, "ViewObjRef", "Name",
                                      szOriginalViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "Original View Name is not a Registered View in this XSLT.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      return( -1 );
   }

   CreateViewFromViewForTask( &vNewViewName, vTZXSLTL, 0 );
   GetVariableFromAttribute( szNewViewName, 0, 'S', 32,
                             vNewViewName, "XSLT", "wNewViewName", "", 0 );
   if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                      szNewViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "New View Name is not a Registered View in this XSLT.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      DropView( vNewViewName );
      return( -1 );
   }

   // Go to execute change view mapping at each Control level.
   fnChangeMappingViewR( vXSLT, vOldViewName, vNewViewName, TRUE, TRUE );

   DropView( vNewViewName );
   DropView( vOldViewName );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ChangeMappingViewChildren
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnChangeMappingViewChildren( zVIEW vSubtask, zBOOL bAtLevel )
{
   zVIEW  vOldViewName;
   zVIEW  vNewViewName;
   zVIEW  vXSLT;
   zVIEW  vTZXSLTL;
   zCHAR  szCtrlTag[ 33 ];
   zCHAR  szNewViewName[ 33 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szCtrlTag, zsizeof( szCtrlTag ), vTZXSLTL, "Control", "Tag" );

   CreateViewFromViewForTask( &vOldViewName, vTZXSLTL, 0 );
   CreateViewFromViewForTask( &vNewViewName, vTZXSLTL, 0 );
   SetNameForView( vOldViewName, "vOldViewName", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewViewName, "vNewViewName", vSubtask, zLEVEL_TASK );
   GetCtrlText( vSubtask, "NewViewName", szNewViewName, 32 );
   if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                      szNewViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "New View Name is not a Registered View in this XSLT.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      DropView( vNewViewName );
      return( -1 );
   }

   // Go to execute change view mapping for this control and its children.
   fnChangeMappingViewR( vXSLT, vOldViewName, vNewViewName, bAtLevel, TRUE );

   DropView( vNewViewName );
   DropView( vOldViewName );

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ChangeMappingViewChildren( zVIEW vSubtask )
{
   return( fnChangeMappingViewChildren( vSubtask, FALSE ) );
}

#if 1

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ChangeMappingView( zVIEW vSubtask )
{
   zVIEW  vXSLT;

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
   }

   return( fnChangeMappingViewChildren( vSubtask, TRUE ) );
}

#else

zOPER_EXPORT zSHORT OPERATION
TZXSLTDD_ChangeMappingView( zVIEW vSubtask )
{
   zVIEW  vOldViewName;
   zVIEW  vNewViewName;
   zVIEW  vXSLT;
   zVIEW  vTZXSLTL;
   zCHAR  szOriginalViewName[ 33 ];
   zCHAR  szNewViewName[ 33 ];

   GetViewByName( &vXSLT, "TZXSLT", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vOldViewName, vTZXSLTL, 0 );
   GetVariableFromAttribute( szOriginalViewName, 0, 'S', 32,
                             vOldViewName, "XSLT", "wOriginalViewName", "", 0 );
   if ( SetCursorFirstEntityByString( vOldViewName, "ViewObjRef", "Name",
                                       szOriginalViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "Original View Name is not a Registered View in this XSLT.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      return( -1 );
   }

   CreateViewFromViewForTask( &vNewViewName, vTZXSLTL, 0 );
   GetVariableFromAttribute( szNewViewName, 0, 'S', 32,
                             vNewViewName, "XSLT", "wNewViewName", "", 0 );
   if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                       szNewViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "New View Name is not a Registered View in this XSLT.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      DropView( vNewViewName );
      return( -1 );
   }

   // Go to execute change view mapping at each Control level.
   // If the Control entity on which we currently have position is selected, then we are only to
   // process that entity and its children.
   // If it is not selected, we will process all the entities in a loop.
   if ( GetSelectStateOfEntity( vXSLT, "Control" ) == 1 )
   {
      TZXSLTDD_ChangeMappingViewR( vXSLT, vOldViewName, vNewViewName );
   }
   else
   {
      zSHORT nRC;

      // Loop through all Controls at this level.
      for ( nRC = SetCursorFirstEntity( vXSLT, "Control", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vXSLT, "Control", "" ) )
      {
         TZXSLTDD_ChangeMappingViewR( vXSLT, vOldViewName, vNewViewName );
      }
   }

   DropView( vNewViewName );
   DropView( vOldViewName );

   return( 0 );
}
#endif

//./ ADD NAME=OpenXSLT_File
// Source Module=tzpntrad.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      OpenXSLT_File
//
//  PURPOSE:    To open a dialog file and paint the first window into
//              the painter
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
OpenXSLT_File( zVIEW vSubtask, zVIEW vNewXSLT )
{
   zVIEW       vNewXSLTL;
   zVIEW       vNewOptions;
   zVIEW       vMetaList;
   zVIEW       vPrevXSLT;
   zVIEW       vIdleView;
   zVIEW       vTZPNTROO;
   zVIEW       vTZZOVEAO;
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

// TraceLineS( "In OpenXSLT_File", "============== " );

   // Clear the painter windows.
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );

   // Drop the previous instance being worked on.
   if ( GetViewByName( &vPrevXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevXSLT );

   // Call configuration management to open the selected file.
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // If there are no W_MetaDefs, then one can't be opened.
   if ( CheckExistenceOfEntity( vMetaList, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( vNewXSLT == 0 &&
        ActivateMetaOI( vSubtask, &vNewXSLT, vMetaList,
                        zSOURCE_XSLT_META, zSINGLE | zLEVEL_TASK ) != 1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( GetViewByName( &vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        vIdleView == vSubtask )
   {
      DropNameForView( vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM );
   }

   // Set the current opened file name.
   if ( GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK ) >= 0 )
      SetAttributeFromAttribute( vTZPNTROO, "Palette", "OpenedFile",
                                 vMetaList, "W_MetaDef", "Name" );

   // Drop the previous instance if it exists.
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vTZZOVEAO )
      DropObjectInstance( vTZZOVEAO );

   // Name the view for mapping.
   SetNameForView( vNewXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK );
   fnCreateMappingEntities( vSubtask, vNewXSLT );

   // BL, remove update flag
   // View vNewXSLT (TZXSLTSO) is updated after activate through
   // attribute derived operations in LOD)
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vNewXSLT->hViewCsr );
   lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   // If XSLT not checked out, set View read only ->
   // the user cannot update the values in Detail Windows.
   if ( !ComponentIsCheckedOut( vSubtask, vNewXSLT, zSOURCE_XSLT_META ) )
      SetViewReadOnly( vNewXSLT );

   CreateViewFromViewForTask( &vNewXSLTL, vNewXSLT, 0 );
   SetNameForView( vNewXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vNewOptions, vNewXSLT, 0 );
   SetNameForView( vNewOptions, "TZXSLTOPT", vSubtask, zLEVEL_TASK );

   // If the panel list modeless window is up, simply fill in the panels
   // for the list, otherwise display the first panel in the XSLT
   TZXSLTDD_CreatePanelList( vSubtask, vNewXSLT, vNewXSLTL );

   if ( MiGetUpdateForView( vNewXSLT ) && ObjectInstanceUpdatedFromFile( vNewXSLT ) )
   {
   // SysMessageBox( vNewXSLT, "Open XSLT File", "Updated", -1 );
      MiSetInstanceUpdateFlag( vNewXSLT, FALSE );  // it should not be updated on open
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vValidate;
   zLONG  lZKey;
   zLONG  lListHandle;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // So we can modify the XSLT and not worry about the consequences.
   GetIntegerFromAttribute( &lZKey, vTZXSLTL, "Window", "ZKey" );
   ActivateOI_FromOI_ForTask( &vTZXSLT, vTZXSLTL, vSubtask, 0 );
   SetCursorFirstEntityByInteger( vTZXSLT, "Window", "ZKey", lZKey, "" );
   CreateViewFromViewForTask( &vValidate, vTZXSLT, 0 );
   SetViewFromView( vValidate, vTZXSLT );

   lListHandle = 0;
   ValidateCtrlAndActionTags( vSubtask, vValidate );
//xoTZXSLTSO_GenerateXSLT( vTZXSLT, vSubtask, &lListHandle );
// oTZXSLTSO_GenerateXSLT( vTZXSLT );

   DropMetaOI_FromList( lListHandle );
   DropObjectInstance( vTZXSLT );
   return( 0 );

} // GenerateXSLT

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateXSLT_Java
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateXSLT_Java( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vValidate;
   zLONG  lZKey;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // So we can modify the XSLT and not worry about the consequences.
   GetIntegerFromAttribute( &lZKey, vTZXSLTL, "Window", "ZKey" );
   ActivateOI_FromOI_ForTask( &vTZXSLT, vTZXSLTL, vSubtask, 0 );
   SetCursorFirstEntityByInteger( vTZXSLT, "Window", "ZKey", lZKey, "" );
   CreateViewFromViewForTask( &vValidate, vTZXSLT, 0 );
   SetViewFromView( vValidate, vTZXSLT );

   ValidateCtrlAndActionTags( vSubtask, vValidate );
//xoTZWDLGSO_GenerateXSLT_Java( vTZXSLT, vSubtask );
// oTZWDLGSO_GenerateXSLT( vTZXSLT );

   DropObjectInstance( vTZXSLT );
   return( 0 );

} // GenerateXSLT_Java

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateAllXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateAllXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vValidate;
   zPCHAR pchDlg;
   zPCHAR pchWnd;
   zCHAR  szMsg[ 256 ];
   zLONG  lListHandle;
   zSHORT nRC;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // So we can modify the XSLT and not worry about the consequences.
   ActivateOI_FromOI_ForTask( &vTZXSLTL, vTZXSLTL, vSubtask, 0 );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );
   CreateViewFromViewForTask( &vValidate, vTZXSLTL, 0 );

   // Prompt operator to ensure ALL windows are to be generated.
   GetAddrForAttribute( &pchDlg, vTZXSLTL, "XSLT", "Tag" );
   sprintf_s( szMsg, zsizeof( szMsg ), "Generate XSLT for ALL Panels in: %s?", pchDlg );
   if ( g_bSkipPrompt == FALSE )
   {
      if ( OperatorPrompt( vSubtask, "Generate XSLT",
                           szMsg, 1, zBUTTONS_YESNO,
                           zRESPONSE_YES, zICON_QUESTION ) == zRESPONSE_NO )
      {
         DropObjectInstance( vTZXSLTL );
         return( -1 );
      }
   }

   lListHandle = 0;
   nRC = SetCursorFirstEntity( vTZXSLT, "Window", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &pchWnd, vTZXSLTL, "Window", "Tag" );
      sprintf_s( szMsg, zsizeof( szMsg ), "Generating JSP: %s.%s", pchDlg, pchWnd );
      MB_SetMessage( vSubtask, 1, szMsg );
      SetViewFromView( vValidate, vTZXSLT );
      ValidateCtrlAndActionTags( vSubtask, vValidate );
   //xoTZWDLGSO_GenerateXSLT( vTZXSLT, vSubtask, &lListHandle );
   // oTZWDLGSO_GenerateXSLT( vTZXSLT );
      nRC = SetCursorNextEntity( vTZXSLT, "Window", 0 );
   }

   DropMetaOI_FromList( lListHandle );
   DropObjectInstance( vTZXSLTL );
   return( 0 );

} // GenerateAllXSLT

#if 0     // dks

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateAllXSLT_Java
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateAllXSLT_Java( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vValidate;
   zPCHAR pchDlg;
   zPCHAR pchWnd;
   zCHAR  szMsg[ 256 ];
   zSHORT nRC;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );

   // So we can modify the XSLT and not worry about the consequences.
   ActivateOI_FromOI_ForTask( &vTZXSLTL, vTZXSLTL, vSubtask, 0 );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );
   CreateViewFromViewForTask( &vValidate, vTZXSLTL, 0 );

   // Prompt operator to ensure ALL windows are to be generated.
   GetAddrForAttribute( &pchDlg, vTZXSLTL, "XSLT", "Tag" );
   sprintf_s( szMsg, zsizeof( szMsg ), "Generate XSLT for ALL Panels in: %s?", pchDlg );
   if ( g_bSkipPrompt == FALSE )
   {
      if ( OperatorPrompt( vSubtask, "Generate Java XSLT",
                           szMsg, 1, zBUTTONS_YESNO,
                           zRESPONSE_YES, zICON_QUESTION ) == zRESPONSE_NO )
      {
         DropObjectInstance( vTZXSLTL );
         return( -1 );
      }
   }

   nRC = SetCursorFirstEntity( vTZXSLT, "Window", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &pchWnd, vTZXSLTL, "Window", "Tag" );
      sprintf_s( szMsg, zsizeof( szMsg ), "Generating JSP Java: %s.%s", pchDlg, pchWnd );
      MB_SetMessage( vSubtask, 1, szMsg );
      SetViewFromView( vValidate, vTZXSLT );
      ValidateCtrlAndActionTags( vSubtask, vValidate );
      oTZWDLGSO_GenerateXSLT_Java( vTZXSLT, vSubtask );
   // oTZWDLGSO_GenerateXSLT( vTZXSLT );
      nRC = SetCursorNextEntity( vTZXSLT, "Window", 0 );
   }

   DropObjectInstance( vTZXSLTL );
   return( 0 );

} // GenerateAllXSLT_Java

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vTZXSLTSO;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

   oTZXSLTSO_GenerateXSLT( &vTZXSLTSO, vTZXSLT );
   DropView( vTZXSLT );

   return( 0 );

} // GenerateXRP

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GenerateAllXSLT
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GenerateAllXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zVIEW  vTZXSLTSO;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

   oTZXSLTSO_GenerateXSLT( &vTZXSLTSO, vTZXSLT );
   DropView( vTZXSLT );

   return( 0 );

} // GenerateAllXSLT

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetReadOnlyCurrentWindow( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

   CheckMigrateCtrls( vTZXSLT, 0, zCHECK_READONLY_ALL_CTRLS, 0, 0, 0 );

   DropView( vTZXSLT );

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetReadOnlyEntireXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zSHORT nRC;

   GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

   nRC = SetCursorFirstEntity( vTZXSLT, "Window", 0 );
   while ( nRC == zCURSOR_SET )
   {
      CheckMigrateCtrls( vTZXSLT, 0, zCHECK_READONLY_ALL_CTRLS, 0, 0, 0 );
      nRC = SetCursorNextEntity( vTZXSLT, "Window", 0 );
   }

   DropView( vTZXSLT );

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetResizeCurrentWindow( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zCHAR  szHeight[ 256 ];
   zSHORT nRC;

   szHeight[ 0 ] = 0;
   nRC = OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'From' height (0 for all)", 0,
                                 szHeight, zsizeof( szHeight ) - 1 );
   g_lFromHeight = zatol( szHeight );
   if ( nRC == zRESPONSE_OK && g_lFromHeight >= 0 )
   {
      szHeight[ 0 ] = 0;
      nRC = OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'To' height", 0,
                                    szHeight, zsizeof( szHeight ) - 1 );
      g_lToHeight = zatol( szHeight );

      if ( nRC == zRESPONSE_OK && g_lToHeight > 0 )
      {
         zPCHAR pchWndTag;

         GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
         CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

         GetAddrForAttribute( &pchWndTag, vTZXSLT, "Window", "Tag" );
         fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchWndTag, zTAG_LTH );

         CheckMigrateCtrls( vTZXSLT, 0, zCHECK_RESIZE_EDIT_CTRLS, 0, 0, 0 );
         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );

         DropView( vTZXSLT );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetResizeEntireXSLT( zVIEW vSubtask )
{
   zVIEW  vTZXSLTL;
   zVIEW  vTZXSLT;
   zCHAR  szHeight[ 256 ];
   zSHORT nRC;

   szHeight[ 0 ] = 0;
   nRC = OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'From' height (0 for all)", 0,
                                 szHeight, zsizeof( szHeight ) - 1 );
   g_lFromHeight = zatol( szHeight );
   if ( nRC == zRESPONSE_OK && g_lFromHeight >= 0 )
   {
      szHeight[ 0 ] = 0;
      nRC = OperatorPromptForInput( vSubtask, "XSLT Migration", "Enter 'To' height", 0,
                                    szHeight, zsizeof( szHeight ) - 1 );
      g_lToHeight = zatol( szHeight );

      if ( nRC == zRESPONSE_OK && g_lFromHeight >= 0 )
      {
         fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         GetViewByName( &vTZXSLTL, "TZXSLTL", vSubtask, zLEVEL_TASK );
         CreateViewFromViewForTask( &vTZXSLT, vTZXSLTL, 0 );

         nRC = SetCursorFirstEntity( vTZXSLT, "Window", 0 );
         while ( nRC == zCURSOR_SET )
         {
            CheckMigrateCtrls( vTZXSLT, 0, zCHECK_RESIZE_EDIT_CTRLS, 0, 0, 0 );
            nRC = SetCursorNextEntity( vTZXSLT, "Window", 0 );
         }

         fnPainterCall( zMSG_REPAINTALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
         DropView( vTZXSLT );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: FindCtrl
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
FindCtrl( zVIEW vSubtask )
{
   zVIEW  vTZXSLT;
   zCHAR  szText[ 256 ];

   if ( GetViewByName( &vTZXSLT, "TZXSLT", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   GetCtrlText( vSubtask, "FindText", szText, zsizeof( szText ) );
   if ( szText[ 0 ] )
   {
      zCHAR  szAttrib[ 256 ];
      zSHORT nLth;
      zLONG  lRC = 0;

      SaveCtrlList( vSubtask );
      GetCtrlText( vSubtask, "InvisibleForFind", szAttrib, zsizeof( szAttrib ) );
      if ( szAttrib[ 0 ] )
         lRC = zatol( szAttrib );

      szAttrib[ 0 ] = 0;

      if ( GetCtrlState( vSubtask, "FindByTag", zCONTROL_STATUS_CHECKED ) )
         strcpy_s( szAttrib, zsizeof( szAttrib ), "Tag" );

      if ( GetCtrlState( vSubtask, "FindByText", zCONTROL_STATUS_CHECKED ) )
      {
         nLth = (zSHORT) zstrlen( szAttrib );
         if ( nLth > 0 )
            szAttrib[ nLth++ ] = ',';

         strcpy_s( szAttrib + nLth, zsizeof( szAttrib ) - nLth, "Text" );
      }

      lRC = OL_FindItemByAttribute( vSubtask, "CtrlList", "Control",
                                    szAttrib, szText, lRC, lRC != 0 ? 1 : 0 );
      if ( lRC && lRC != -1 )
      {
         TraceLineI( "FindCtrl RC: ", lRC );
         zltoa( lRC, szAttrib, zsizeof( szAttrib ), 10 );
         SetCtrlText( vSubtask, "InvisibleForFind", szAttrib );
      // RefreshCtrl( vSubtask, "Mapping" );
      // RefreshCtrl( vSubtask, "EventAction" );
      // RefreshCtrl( vSubtask, "MapType" );
      // RefreshCtrl( vSubtask, "Context" );
      // RefreshWindowExceptForCtrl( vSubtask, "CtrlList" );
      // fnShowCtrlList( vSubtask );

      // if ( OL_SetCursorByEntityNumber( vTZXSLT, "Control", lRC ) == 0 )
         {
            OL_SelectItemAtPosForEntity( vSubtask, "CtrlList", "Control", 2 + 4 + 16 );
         }

         GetStringFromAttribute( szText, zsizeof( szText ), vTZXSLT, "Control", "Tag" );
         strcat_s( szText, zsizeof( szText ), " - Control found" );
         SysMessageBox( 0, szText, "Please click on Ctrl to Show Information Properly", -1 );
      }
      else
         SetCtrlText( vSubtask, "InvisibleForFind", "" );
   }

   return( 0 );

} // FindCtrl

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ReturnFromBuildXSLTFlow( zVIEW vSubtask )
{
   zVIEW  vFlow;

   if ( GetViewByName( &vFlow, "TZFLOW", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vFlow );

   return( 0 );
}

void
fnSetTagType( zVIEW vFlow )
{
   zCHAR  szText[ 256 ];
   zCHAR  szType[ 32 ];

   // Get Tag and Type of widget (Control/MenuOptio/Hotkey) that invokes the event
   GetStringFromAttribute( szText, zsizeof( szText ), vFlow, "Widget", "Tag" );
   GetStringFromAttribute( szType, zsizeof( szType ), vFlow, "Widget", "Type" );
   if ( szType[ 0 ] )
   {
      strcat_s( szText, zsizeof( szText ), " - " );
      strcat_s( szText, zsizeof( szText ), szType );
   }

   SetAttributeFromString( vFlow, "Widget", "TagType", szText );

}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
BuildXSLTFlow( zVIEW vSubtask )
{
   zCHAR  szWork[ 256 ];
   zCHAR  szText[ 256 ];
   zCHAR  szTag[ 32 ];
   zCHAR  szOper[ 32 ];
   zCHAR  szDlgTag[ 32 ];
   zCHAR  szWndTag[ 32 ];
   zVIEW  vXSLT;
   zVIEW  vFlow;
   zLONG  lActType;
   zSHORT nLth;
   zSHORT nRC;

   ActivateEmptyObjectInstance( &vFlow, "TZFLOW", vSubtask, zSINGLE );
   CreateEntity( vFlow, "TZFLOW", zPOS_NEXT );
   SetNameForView( vFlow, "TZFLOW", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vXSLT, "TZOPENXSLT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      CreateViewFromView( &vXSLT, vXSLT );
      while ( ResetViewFromSubobject( vXSLT ) == 0 )
      {
         // nothing to do
      }

      SetAttributeFromAttribute( vFlow, "TZFLOW", "Tag", vXSLT, "XSLT", "Tag" );
      SetAttributeFromAttribute( vFlow, "TZFLOW", "Name", vXSLT, "XSLT", "Desc" );
      GetStringFromAttribute( szText, zsizeof( szText ), vFlow, "TZFLOW", "Tag" );
      GetStringFromAttribute( szWork, zsizeof( szWork ), vFlow, "TZFLOW", "Name" );
      if ( szWork[ 0 ] )
      {
         nLth = (zSHORT) zstrlen( szText );
         szText[ nLth++ ] = ' ';
         szText[ nLth++ ] = '-';
         szText[ nLth++ ] = ' ';
         strcpy_s( szText + nLth, zsizeof( szText ) - nLth, szWork );
      }

      SetAttributeFromString( vFlow, "TZFLOW", "Name", szText );
      nRC = SetCursorFirstEntity( vXSLT, "Window", 0 );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vFlow, "Window", zPOS_NEXT );
         SetAttributeFromAttribute( vFlow, "Window", "Tag", vXSLT, "Window", "Tag" );
         SetAttributeFromAttribute( vFlow, "Window", "Caption", vXSLT, "Window", "Caption" );
         GetStringFromAttribute( szText, zsizeof( szText ), vFlow, "Window", "Tag" );
         nLth = (zSHORT) zstrlen( szText );
         szText[ nLth++ ] = ' ';
         szText[ nLth++ ] = '-';
         szText[ nLth++ ] = ' ';
         GetStringFromAttribute( szText + nLth, zsizeof( szText ) - nLth, vFlow, "Window", "Caption" );
         SetAttributeFromString( vFlow, "Window", "TagCaption", szText );
         nRC = SetCursorFirstEntity( vXSLT, "Action", 0 );
         while ( nRC == zCURSOR_SET )
         {
            CreateEntity( vFlow, "Action", zPOS_NEXT );
            SetAttributeFromAttribute( vFlow, "Action", "Tag", vXSLT, "Action", "Tag" );
            SetAttributeFromAttribute( vFlow, "Action", "Operation", vXSLT, "Action", "Tag" );
            if ( CheckExistenceOfEntity( vXSLT, "ActOper" ) == 0 )
               SetAttributeFromAttribute( vFlow, "Action", "Operation", vXSLT, "ActOper", "Name" );
            else
               SetAttributeFromString( vFlow, "Action", "Operation", "n/a" );

            GetIntegerFromAttribute( &lActType, vXSLT, "Action", "Type" );
            GetStringFromAttribute( szTag, zsizeof( szTag ), vFlow, "Action", "Tag" );
            GetStringFromAttribute( szOper, zsizeof( szOper ), vFlow, "Action", "Operation" );
            GetStringFromAttribute( szDlgTag, zsizeof( szDlgTag ), vXSLT, "Action", "XSLTName" );
            GetStringFromAttribute( szWndTag, zsizeof( szWndTag ), vXSLT, "Action", "WindowName" );
            if ( szWndTag[ 0 ] )
            {
               sprintf_s( szText, zsizeof( szText ), "Tag: %s  Oper: %s   WAB: %s   ====> %s.%s",
                          szTag, szOper, GetActionString( lActType ), szDlgTag, szWndTag );
            }
            else
            {
               sprintf_s( szText, zsizeof( szText ), "Tag: %s  Oper: %s   WAB: %s",
                          szTag, szOper, GetActionString( lActType ) );
            }

            SetAttributeFromString( vFlow, "Action", "TagOperWAB_DlgWnd", szText );

            if ( CheckExistenceOfEntity( vXSLT, "ActWndEvent" ) == 0 )
            {
               nRC = SetCursorFirstEntity( vXSLT, "ActWndEvent", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  CreateEntity( vFlow, "Widget", zPOS_NEXT );
                  SetAttributeFromAttribute( vFlow, "Widget", "Tag", vXSLT, "ActWndEvent", "EventName" );
                  SetAttributeFromString( vFlow, "Widget", "Type", "WindowEvent" );
                  fnSetTagType( vFlow );
                  nRC = SetCursorNextEntity( vXSLT, "ActWndEvent", 0 );
               }
            }

            if ( CheckExistenceOfEntity( vXSLT, "ActEvent" ) == 0 )
            {
               nRC = SetCursorFirstEntity( vXSLT, "ActEvent", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  if ( CheckExistenceOfEntity( vXSLT, "ActCtrl" ) == 0 )
                  {
                     CreateEntity( vFlow, "Widget", zPOS_NEXT );

                     GetStringFromAttribute( szText, zsizeof( szText ), vXSLT, "ActCtrl", "Tag" );
                     nLth = (zSHORT) zstrlen( szText );
                     szText[ nLth++ ] = ' ';
                     szText[ nLth++ ] = '-';
                     szText[ nLth++ ] = ' ';
                     GetStringFromAttribute( szText + nLth, zsizeof( szText ) - nLth, vXSLT, "ActCtrl", "Text" );

                     SetAttributeFromString( vFlow, "Widget", "Tag", szText );
                     SetAttributeFromString( vFlow, "Widget", "Type", "Control" );
                     fnSetTagType( vFlow );
                  }

                  nRC = SetCursorNextEntity( vXSLT, "ActEvent", 0 );
               }
            }

            if ( CheckExistenceOfEntity( vXSLT, "ActOpt" ) == 0 )
            {
               nRC = SetCursorFirstEntity( vXSLT, "ActOpt", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  CreateEntity( vFlow, "Widget", zPOS_NEXT );
                  SetAttributeFromAttribute( vFlow, "Widget", "Tag", vXSLT, "ActOpt", "Text" );
                  SetAttributeFromString( vFlow, "Widget", "Type", "Menu Option" );
                  fnSetTagType( vFlow );
                  nRC = SetCursorNextEntity( vXSLT, "ActOpt", 0 );
               }
            }

            if ( CheckExistenceOfEntity( vXSLT, "ActHot" ) == 0 )
            {
               nRC = SetCursorFirstEntity( vXSLT, "ActHot", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  CreateEntity( vFlow, "Widget", zPOS_NEXT );
                  SetAttributeFromAttribute( vFlow, "Widget", "Tag", vXSLT, "ActHot", "Title" );
                  SetAttributeFromString( vFlow, "Widget", "Type", "Hotkey" );
                  fnSetTagType( vFlow );
                  nRC = SetCursorNextEntity( vXSLT, "ActHot", 0 );
               }
            }

            nRC = SetCursorNextEntity( vXSLT, "Action", 0 );
         }

         nRC = SetCursorNextEntity( vXSLT, "Window", 0 );
      }

      DropView( vXSLT );
      return( 0 );
   }

   return( -1 );
}

#endif

#ifdef __cplusplus
}
#endif
