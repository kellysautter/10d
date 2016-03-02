//
// MODULE NAME:  TZRPUP2D  -  Report C dialog operations
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Inc. All rights reserved.  Confidential   *
// * and Proprietary material subject to license - do not reproduce or      *
// * disclose.  This material is an unpublished work and is considered a    *
// * trade secret belonging to the copyright holder.                        *
// //////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    9/30/92
//  API:     MicroSoft foundation classes and Windows 3.1 SDK
//  ENVIRONMENT: Windows 3.1
//
//  LAST MESSAGE ID: PN00221
//
/*
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.08.09 BL
   avoid "ViewIsReadOnly" if a meta is not checked out.
2002.03.12 FH
   avoid "ViewIsReadOnly" if a meta is not checked out.
2001.05.03   PAS    50801
   Don't allow an Entity GroupSet with a duplicate Tag (entity name)
2001.02.15    BL    R54697
   improve handling of return code from InitializeLPLR
2001.01.31 RG  Z2000
   Send an error message if a SironReport should be opened as ZeidonReport
   Bug: F51660
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.05  BL    Z10
   Completely maintained Control List and Report Controls
   Added Functions for Menu Format (=Format Controls)
   If Zeidon Report not checked out, set Report Views read only
   Remove Code for Copy Report
2000.09.23  BL    Z10
   Bugfix repaint Report Controls
   Added function for Menu Format (RAD 53746)
2000.07.18  BL    Z10 Repository
   Added fnTZRPUPDD_CreateGroupSet, fnTZRPUPDD_CreatePage,
   fnTZRPUPDD_SetFlagsForReportView and modified Operations:
   if Zeidon Report not checked out disable all function for update
2000.07.13  BL  Z10
   Added TZRPUP2D_SaveAsKeepCurrentDesc, TZRPUP2D_UpdateZeidonWindows
   and modified other save as functions for Keep Current Description
2000.06.30  BL    Z10  Repository and Tools
   Set check out state in title. Is Report not checked out, disable Delete
   and Save Action  and ask for save as
2000.06.13  BL    Z10
   Modified TZRPUPDD_ReportPreBuild: if Workstation Administration open,
   do not open Report tool
2000.06.05  DKS   Z10
   Send in flag to force reload of task view for LPLR.
2000.01.13  BL    Z10
   Modified TZRPUPDD_ReportPreBuild and TZRPUPDD_EnablePainterWindows
   for new PPE handling
1999.12.13  DC    Z10
   Modified zwTZRPUPDD_CopyReport because it was not copying the correct
   Report.
1999.11.24  BL    Z10
   Modified Operation TZRPUP2D_SaveAsReport and TZRPUP2D_SaveNewReport: Bug
   in Save as
1999.11.11  DC    Z10
   Added operation TZRPUP2D_SaveNewReport so that we wouldn't have to call
   TZRPUPSD for Zeidon reports. Also changed call to TZRPUPSD.SortByDate to
   TZRPUPDD.SortByDate.
1999.11.07  BL    Z10   new menu structre
   Modified Operation TZRPUPDD_OpenReportFile, TZRPUPDD_ReportPostBuild,
   TZRPUPDD_NewReportFile and TZRPUPDD_EnablePainterWindows for set LPLR Name
   in main window
   Added Operation TZRPUP2D_SaveAsPostBuild, TZRPUP2D_SetSaveAsName,
   TZRPUP2D_SaveAsReport and TZRPUP2D_SaveNewReport for save Report as
1999.09.30  DKS   Z10   QS999
   After SwitchLPLR, display Report List.  Remove references to PageList
   option.
1998.07.05  DKS
   Synchronize code with that of reports to facilitate parallel maintenance
1998.07.21  RG
   Removed the call of fnPainterCall on line 1330 after discussion with DKS
   because he changed definition of zMSG_SETTOOLBOX to zMSG_SETGRID
*/
#define _MT 1

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define  KZOESYS_INCL
#define  zNO_KZOEINCO    // to get the correct operation thru zGET_PTR
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

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

zOPER_EXPORT zLONG OPERATION
fnPainterCall( zSHORT nMsg,    zVIEW vSubtask,
               zVIEW  vDialog, zPCHAR szParm, zLONG lMaxLth );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_AskForSave( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_UpdatePageFromList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SortByDate( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_OpenReportFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SaveReportFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ReportCopy( zPVIEW    pvNewReport,
                      zVIEW     vOrigReport,
                      zVIEW     vSourceLPLR,
                      zVIEW     vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUP2D_SaveNewReport( zVIEW     vSubtask,
                        zPVIEW    pvNewReport,
                        zPCHAR    szNewReportName,
                        zVIEW     vOrigReport,
                        zVIEW     vSourceLPLR );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szLodname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );

static zSHORT
fnTZRPUP2D_CheckActionAfterSaveAs( zVIEW  vSubtask );

static zSHORT
fnTZRPUP2D_SaveAsCheckName( zVIEW  vSubtask,
                            zVIEW  vCM_List,
                            zVIEW  vPAGEL,
                            zPCHAR szOutName,
                            zPCHAR szNewName );

static zSHORT
fnTZRPUPDD_CreateGroupSet( zVIEW vSubtask, zVIEW vNewReport );

static zSHORT
fnTZRPUPDD_CreatePage( zVIEW vSubtask,
                       zVIEW vNewReportL );

static zSHORT
fnTZRPUPDD_SetFlagsForReportView( zVIEW vSubtask,
                                  zVIEW vNewReport );


void
fnCreateReportMappingEntities( zVIEW vSubtask, zVIEW vReport )
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

   TraceLineS( "In fnCreateMappingEntitys",
               " ============== DOING WORK ==============" );

   // Drop the previous instance if it exists.
// DropObjectInstance( vTZZOVEAO );  // no ... drop it when View list changes

   ActivateEmptyObjectInstance( &vTZZOVEAO, "TZZOVEAO", vSubtask, zMULTIPLE );
   SetNameForView( vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );

   // Activate the LOD for the current ViewObjRef subobject.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   if ( vReport )
   {
      nRC = SetCursorFirstEntity( vReport, "ViewObjRef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         CreateEntity( vTZZOVEAO, "ViewObjRef", zPOS_NEXT );
         SetMatchingAttributesByName( vTZZOVEAO, "ViewObjRef",
                                      vReport, "ViewObjRef", zSET_ALL );
         CreateEntity( vTZZOVEAO, "LOD", zPOS_NEXT );
         SetMatchingAttributesByName( vTZZOVEAO, "LOD",
                                      vReport, "LOD", zSET_ALL );
      // GetAddrForAttribute( &pchLOD_Name, vReport, "ViewObjRef", "Name" );
         GetIntegerFromAttribute( &lZKey, vReport, "LOD", "ZKey" );
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
         nRC = SetCursorNextEntity( vReport, "ViewObjRef", 0 );
      }

      SetCursorFirstEntity( vReport, "ViewObjRef", 0 );
   }

   RefreshCtrl( vPainter, "View" );
// MiSetInstanceUpdateFlag( vReport, 0 );

// DisplayObjectInstance( vTZZOVEAO, 0, "LOD_Entity" );
}

zSHORT
fnTZRPUPDD_OpenReportFile( zVIEW vSubtask, zVIEW vNewReport )
{
   zVIEW  vNewReportL;
   zVIEW  vMetaList;
   zVIEW  vPrevReport;
   zVIEW  vTZPNTROO;
   zVIEW  vTZZOVEAO;
   zCHAR  szSironName[ 33 ];

// TraceLineS( "In OpenReportFile", "============== " );

   // Clear the painter windows
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );

   // Drop the previous instance being worked on.
   if ( GetViewByName( &vPrevReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevReport );

   // Call configuration management to open the selected file
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // If there are no W_MetaDefs, then one can't be opened.
   if ( CheckExistenceOfEntity( vMetaList, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( vNewReport == 0 &&
        ActivateMetaOI( vSubtask, &vNewReport, vMetaList,
                        zSOURCE_REPORT_META, zSINGLE | zLEVEL_TASK ) != 1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   //RG: If attribute SironName isn't empty, it's a SironReport
   GetStringFromAttribute( szSironName, zsizeof( szSironName ), vNewReport, "Report", "SironName" );
   if ( zstrcmp( szSironName, "" ) != 0 )
   {
      // The Report is a SironReport
      MessageSend( vSubtask, "RP00011", "Report Maintenance",
                   "Cannot open Report. It's a Siron Report.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Don ... what all to do here???  ... what attributes need to be set???
   if ( SetCursorFirstEntity( vNewReport, "Page", 0 ) != zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vNewReport, "Page", zPOS_AFTER );
   }

   // Force header and footer GroupSets along with a detail Group for each.
   fnTZRPUPDD_CreateGroupSet( vSubtask, vNewReport );

   // Set the current opened file name.
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK );
   SetAttributeFromAttribute( vTZPNTROO, "Palette", "OpenedFile",
                              vMetaList, "W_MetaDef", "Name" );

   // Drop the previous instance if it exists.
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vTZZOVEAO )
      DropObjectInstance( vTZZOVEAO );

   // Name the view for mapping.
   SetNameForView( vNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
// SetNameForView( vNewReport, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   fnCreateReportMappingEntities( vSubtask, vNewReport );

   // if Report not checked out, set  View read only ->
   // the user cannot update the values in Detail Windows
   if ( !ComponentIsCheckedOut( vSubtask, vNewReport, zSOURCE_REPORT_META ))
      SetViewReadOnly( vNewReport );

   // Execute routine to initialize Report Display subobject data, as necessary.
   oTZRPSRCO_InitReportStructure( vNewReport );

   CreateViewFromViewForTask( &vNewReportL, vNewReport, 0 );
   SetNameForView( vNewReportL, "PAGEL", vSubtask, zLEVEL_TASK );

   // If the page list modeless window is up, simply fill in the windows
   // for the list, otherwise display the first page in the report
   fnTZRPUPDD_CreatePage( vSubtask, vNewReportL );

   // remove update flag and set View read only (if not checked out )
   fnTZRPUPDD_SetFlagsForReportView( vSubtask, vNewReportL );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_OpenReportFile( zVIEW vSubtask )
{
   return( fnTZRPUPDD_OpenReportFile( vSubtask, 0 ) );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SaveReportFile( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vReportConvert;
   zBOOL  bApplicationInvoked;

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   GetViewByName( &vReportConvert, "__SysApplicationReport",
                  vSubtask, zLEVEL_SYSTEM );
   if ( vReportConvert )
      bApplicationInvoked = TRUE;
   else
      bApplicationInvoked = FALSE;

   // Positions in the report def are interpreted in 2 ways.
   // In the painter, they determine the position on the screen.
   // However, at print time, they are not accurate if one of the
   // controls within a group are a multiline text control. The size
   // of a multi line text control can NOT be determined until it is
   // printed. For this reason, the routines responsible for printing
   // will have to adjust the positions of any control that follows
   // a multiline control. The loop in the printing routine will loop
   // thru all controls for a group. For things to be printed right,
   // it needs to encounter the controls sorted by position, otherwise,
   // the adjustment for multiline controls will be nonsense. This
   // sorting can be done at print time. For efficiency sake, we
   // will do it here at save time.

   // This sorts lexicographically by psdlg_x in ascending order
   // and psdlg_y in ascending order.
   OrderEntityForView( vReport, "Control", "PSDLG_Y A PSDLG_X A" );

   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   CreateViewFromViewForTask( &vReportConvert, vReport, 0 );
   AssignZKeyToMetaOI( vSubtask, vReportConvert, zSOURCE_REPORT_META, 0 );

   ResetView( vReportConvert );

   if ( bApplicationInvoked == FALSE )
   {
      if ( CommitMetaOI( vSubtask, vReportConvert, zSOURCE_REPORT_META ) < 0 )
      {
         MessageSend( vSubtask, "RP00219", "Report Maintenance",
                      "Unable to save Report",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -2 );
      }

      DropView( vReportConvert );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_ReportPreBuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zVIEW  vMetaList;
   zVIEW  vTZPESRCO;
   zVIEW  vTZRPSRCO;
   zVIEW  vTaskLPLR;
   zVIEW  vSaveAs;
   zCHAR  szApplName[ 34 ];
   zPCHAR szLPLR_Name;
   zSHORT nRC;

   // Get the Profile information and
   // Call configuration management to set up the default LPL.
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   szLPLR_Name = 0;
   GetViewByName( &vTZRPSRCO, "__SysApplicationReport",
                  vSubtask, zLEVEL_SYSTEM );
   if ( vTZRPSRCO )
   {
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
         DropObjectInstance( vTaskLPLR );

      GetApplDirectoryFromView( szApplName, vTZRPSRCO, zAPPL_NAME, 33 );
      if ( InitializeLPLR( vSubtask, szApplName ) < 0 )
         return( -1 );
   }
   else
   if ( CompareAttributeToString( vProfileXFER, "WD", "StartupLPLR_Option",
                                  "D" ) == 0 )   //default LPLR
   {
      if ( InitializeLPLR( vSubtask, "" ) < 0 )
         return( -1 );
   }
   else
   {
      GetAddrForAttribute( &szLPLR_Name,
                           vProfileXFER, "WD", "StartupLPLR_Name" );
      if ( InitializeLPLR( vSubtask, szLPLR_Name ) < 0 )
         return( -1 );
   }

   // Get a list of reports
   if ( GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_REPORT_META );

   SetNameForView( vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Activate the presentation environment.
   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );
      nRC = LoadZeidonPPE( vSubtask, &vTZPESRCO, zREFER_PENV_META, vMetaList,
                           "Report Maintenance",
                           "Painter exiting.");
      if ( nRC < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
         return( -16 );
      }

      SetNameForView( vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
      DropView( vMetaList );
   }

   // If the Report is not checked out and the user changes this Report,
   // then Zeidon calls the window "Save Report as".
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Report or Open an other Report or Switch Project or
   // Exit Report Tool).
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
TZRPUPDD_ReportPostBuild( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zVIEW  vTZPNCTWO;
   zVIEW  vNewReportL = 0;
   zVIEW  vPageList;
   zVIEW  vTZRPSRCO;
   zVIEW  vProfileXFER;
   zCHAR  szPageName[ 33 ];

   // Setup the List of Reports.
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Name the subtask associated with the main page so we can set its
   // title later.
   SetNameForView( vSubtask, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   // Set the top entities to "Report" and "Page" (rather than the default
   // of "Dialog" and "Window") ... but do not do initialization.
   fnPainterCall( zMSG_INITPAINTER, vSubtask, 0, (zPCHAR) -1, 0 );

   GetViewByName( &vTZRPSRCO, "__SysApplicationReport",
                  vSubtask, zLEVEL_SYSTEM );
   if ( vTZRPSRCO )
   {
      // Drop the previous instance being worked on if it exists.
      GetViewByName( &vNewReportL, "TZOPENRPT", vSubtask, zLEVEL_TASK );
      if ( vNewReportL )
         DropMetaOI( vSubtask, vNewReportL );

      fnTZRPUPDD_OpenReportFile( vSubtask, vTZRPSRCO );
      GetViewByName( &vNewReportL, "PAGEL", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByString( vNewReportL, "Page", "Tag",
                                         szPageName, 0 ) >= zCURSOR_SET )
      {
         TZRPUPDD_UpdatePageFromList( vSubtask );
      }

      if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) > 0 )
      {
         SetWindowActionBehavior( vPageList, zWAB_ReturnToParent |
                                  zWAB_ProcessImmediateAction, 0, 0 );
      }
   }
   else
   {
#ifndef __WIN32__
      if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) < 1 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow |
                                            zWAB_ProcessImmediateAction,
                                  "TZRPUPZD", "PageList" );
      }
#endif

      // Transfer to the open report window
      if ( vMetaList )
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                                  zWAB_ProcessImmediateAction,
                                  "TZRPUPZD", "ReportList" );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Zeidon Reports Maintenance", "PAGEL",
                              vNewReportL, "Report", zSOURCE_REPORT_META );

   // Initialize the TZPNCTWO work object.
   ActivateEmptyObjectInstance( &vTZPNCTWO, "TZPNCTWO", vSubtask, zSINGLE );
   CreateEntity( vTZPNCTWO, "TZPNCTWO", zPOS_AFTER );
   SetAttributeFromString( vTZPNCTWO, "TZPNCTWO", "PainterType", "R" );
   SetNameForView( vTZPNCTWO, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set the top entities to "Report" and "Page" (rather than the default
   // of "Dialog" and "Window") ... and do initialization.
   fnPainterCall( zMSG_INITPAINTER, vSubtask, 0, (zPCHAR) 1, 0 );

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
TZRPUPDD_DeleteCurrentPageWnd( zVIEW vSubtask )
{
   fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_UpdatePageFromList( zVIEW vSubtask )
{
   zVIEW  vReportList;
   zPCHAR pchTag;
   zSHORT nRC;

   // Get the view to the page list
   if ( GetViewByName( &vReportList, "PAGEL", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   if ( CheckExistenceOfEntity( vReportList, "Page" ) != zCURSOR_SET )
      return( -1 );

   SetNameForView( vReportList, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // Get Address of page tag and see if there is an active painter
   // for the page
   GetAddrForAttribute( &pchTag, vReportList, "Page", "Tag" );

   nRC = (zSHORT) fnPainterCall( zMSG_UPDATEWINDOWBYNAME, vSubtask, 0, pchTag, zTAG_LTH );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SelectPageFromList( zVIEW vSubtask )
{
   zVIEW  vReportList;

   // Get the view to the window list
   if ( GetViewByName( &vReportList, "PAGEL", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( CheckExistenceOfEntity( vReportList, "Page" ) != zCURSOR_SET ||
        !GetSelectStateOfEntityForSet( vReportList, "Page", 1 ) )
   {
      SetCursorFirstEntity( vReportList, "Page", 0 );
   }

   SetNameForView( vReportList, "TZWINDOW", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SortWindows( zVIEW vSubtask )
{
   zVIEW vReport;
   zCHAR szSelected[ 34 ];

   GetViewByName( &vReport, "PAGEL", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szSelected, zsizeof( szSelected ), vReport, "Page", "Tag" );
   zgSortEntityWithinParent( zASCENDING, vReport,
                             "Page", "Tag", "" );
   SetCursorFirstEntityByString( vReport, "Page", "Tag", szSelected, 0 );
   SetAllSelStatesForEntityForSet( vReport, "Page", 0, 1, 0 );
   SetSelectStateOfEntityForSet( vReport, "Page", 1, 1 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_ExitReport( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vProfileXFER;
   zVIEW  vLPLR;
   zVIEW  vTZRPSRCO;
   zVIEW  vSaveAs;
   zLONG  lFlags;
   zSHORT nRC;

   // Now see if the painter is being invoked by an application.
   GetViewByName( &vTZRPSRCO, "__SysApplicationReport",
                  vSubtask, zLEVEL_SYSTEM );
   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
      nRC = TZRPUPDD_AskForSave( vSubtask );
      if ( vSaveAs )
      {
         if ( nRC )
         {
            if ( nRC == 99 )
               // if Report not checked out and user change this Report, then Zeidon
               // call the window "Save Report as"
               // We save the action after Save as in the View TZSAVEAS (for example:
               // Create new Report or Open an other Report or Switch Project or
               // Exit Report Tool)
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

   if ( CompareAttributeToString( vProfileXFER, "WD", "StartupLPLR_Option",
                                  "L" ) == 0 )   // set Last LPLR
   {
      GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vLPLR )
      {
         SetAttributeFromAttribute( vProfileXFER, "WD", "StartupLPLR_Name",
                                    vLPLR, "LPLR", "Name" );
      }
   }

   oTZ__PRFO_CommitProfile( vProfileXFER );

   // Delete all the painter windows explicitly before the
   // default CWnd destructor does it and causes problems.
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );
   fnPainterCall( zMSG_DELETETOOLBAR, vSubtask, 0, 0, 0 );

   if ( MiValidView( vReport ) )
      DropMetaOI( vSubtask, vReport );

   TerminateLPLR( vSubtask );
   if ( MiValidView( vTZRPSRCO ) )
   {
      DropNameForView( vTZRPSRCO, "__SysApplicationReport",
                       vSubtask, zLEVEL_SYSTEM );
   // SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_AskForSave( zVIEW vSubtask )
{
   zVIEW  vReport;
   zCHAR  szReportName[ 33 ];
   zCHAR  szMessageText[ 254 ];
   zBOOL  nSaveAs = FALSE;
   zSHORT nRC;

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   if ( MiGetUpdateForView( vReport ) == 1  &&
        ObjectInstanceUpdatedFromFile( vReport ) == 1 )
   {
      // if Report not checked out, then ask for save as
      if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ) )
      {
         nSaveAs = TRUE;
         GetStringFromAttribute( szReportName, zsizeof( szReportName ), vReport, "Report", "Tag" );
         strcpy_s( szMessageText, zsizeof( szMessageText ), "Report is not checked out, but Report '" );
         strcat_s( szMessageText, zsizeof( szMessageText ), szReportName );
         strcat_s( szMessageText, zsizeof( szMessageText ), "' has been modified. \n\nWould you like to " );
         strcat_s( szMessageText, zsizeof( szMessageText ), "save it with differend name?" );
         nRC = MessagePrompt( vSubtask, "DM00115", "Report Maintenance",
                              szMessageText, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );
      }
      else
      {
          nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_REPORT,
                              zREFER_REPORT_META,
                              vReport, "Report", "Tag", 0 );
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
            // If Report not checked out and user would save it, then call
            // window save Report as.
            if ( nSaveAs )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                        "TZRPUPZD", "SaveAs" );
               return( 99 );
            }

            nRC = TZRPUPDD_SaveReportFile( vSubtask );
            if ( nRC < 0 )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
               return( nRC );
            }
         }
         else
         {
            zVIEW  vTZRPSRCO;

            // Now see if the painter is being invoked by an application.
            GetViewByName( &vTZRPSRCO, "__SysApplicationReport",
                           vSubtask, zLEVEL_SYSTEM );
            if ( vTZRPSRCO )
               DropObjectInstance( vTZRPSRCO );

            return( 0 );
         }

      } //endif ( nRC == zRESPONSE_CANCEL )
   }
   else  // no update
      nRC = 0;

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_DeleteReport( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // If there are no W_MetaDefs then one can't be opened.
   if ( CheckExistenceOfEntity( vMetaList, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_REPORT,
                      zREFER_REPORT_META, vMetaList, "W_MetaDef",
                      "Name", 0 ) == zRESPONSE_YES )
   {
      nRC = DeleteMetaOI( vSubtask, vMetaList, zREFER_REPORT_META );
      SetCursorPrevEntity( vMetaList, "W_MetaDef", "" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SortByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vMetaList,
                                   "W_MetaDef", "Name", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SortByDate( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vMetaList,
                                   "W_MetaDef", "LastUpdateDate", "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_PageListPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "PageListPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_PrebuildPageList( zVIEW vSubtask )
{
   zVIEW vProfileXFER;
   zVIEW vMainWindow;

   SetNameForView( vSubtask, "TZPAGE_LIST", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK ) > 0 )
   {
      RestoreWindowPlacement( vSubtask, vProfileXFER );

      // set Check-Status for WindowList
      if ( GetViewByName( &vMainWindow, "TZPNMWIN", vSubtask, zLEVEL_TASK ) >= zCURSOR_SET )
      {
         // set Check-Status for WindowList
         SetOptionState( vMainWindow, "PageList", zOPTION_STATUS_CHECKED, 1 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_DisplayPageList( zVIEW vSubtask )
{
   zVIEW vPageList;

   // is the window already created ?
   if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) < 1 )
   {
      // set Check-Status for WindowList
      SetOptionState( vSubtask, "PageList", zOPTION_STATUS_CHECKED, 1 );

      // Start the window list modeless window
      SetWindowActionBehavior( vSubtask, zWAB_StartModelessSubwindow,
                               "TZRPUPZD", "PageList" );
   }
   else
   {
      // is the window at this time hidden ?
      if ( GetWindowState( vPageList, zWINDOW_STATUS_VISIBLE ) > 0 )
      {
         // reset Check-Status for WindowList
         SetOptionState( vSubtask, "PageList", zOPTION_STATUS_CHECKED, 0 );

         // Hide the Window
         SetWindowState( vPageList, zWINDOW_STATUS_VISIBLE, 0 );
      }
      else
      {
         // reset Check-Status for WindowList
         SetOptionState( vSubtask, "PageList", zOPTION_STATUS_CHECKED, 1 );

         // show the Window
         SetWindowState( vPageList, zWINDOW_STATUS_VISIBLE, 1 );
      }
   }
   return( 0 );
}

#define zGROUPSET_REPORT                    3000L  // from zdrvropr.h
#define zGROUP_REPORT                       3010L  // from zdrvropr.h

// Don ... is this all that is needed in terms of attributes???
void
fnCreateReportMetaEntity( zVIEW   vSubtask,
                          zVIEW   vReport,
                          zCPCHAR cpcType,
                          zLONG   lPPE_Type,
                          zSHORT  nPos )
{
   zPCHAR pchEntity;
   zLONG  lPosY;
   zLONG  lSizeY;

   if ( lPPE_Type == zGROUPSET_REPORT )
   {
      pchEntity = "GroupSet";
      if ( cpcType[ 2 ] == 'F' )
         lPosY = 80;
      else
         lPosY = 4;

      lSizeY = 20;
   }
   else
   {
      pchEntity = "Group";
      lPosY = 4;
      lSizeY = 8;
   }

   CreateMetaEntity( vSubtask, vReport, pchEntity, nPos );
   SetAttributeFromString( vReport, pchEntity, "Type", cpcType );
   SetAttributeFromInteger( vReport, pchEntity, "PPE_Type", lPPE_Type );
   SetAttributeFromInteger( vReport, pchEntity, "PSDLG_X", 1L );
   SetAttributeFromInteger( vReport, pchEntity, "PSDLG_Y", lPosY );
   SetAttributeFromInteger( vReport, pchEntity, "SZDLG_X", 20L );
   SetAttributeFromInteger( vReport, pchEntity, "SZDLG_Y", lSizeY );
}

static zSHORT
fnTZRPUPDD_CreateGroupSet( zVIEW vSubtask, zVIEW vNewReport )
{
   zSHORT  nRC;
   zBOOL   bHeader = FALSE;
   zBOOL   bFooter = FALSE;
   zCHAR   szType[ 8 ];       // Don ... only needs to be 4???

   // force header and footer GroupSets along with a detail Group for each

   nRC = SetCursorFirstEntity( vNewReport, "GroupSet", 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetStringFromAttribute( szType, zsizeof( szType ), vNewReport, "GroupSet", "Type" );
      if ( szType[ 0 ] )
      {
         if ( szType[ 1 ] == 'H' || szType[ 1 ] == 'h' )
         {
            bHeader = TRUE;
            if ( SetCursorFirstEntity( vNewReport,
                                       "Group", 0 ) != zCURSOR_SET )
            {
               fnCreateReportMetaEntity( vSubtask, vNewReport, "ga",
                                         zGROUP_REPORT, zPOS_AFTER );
            }
         }

         if ( szType[ 1 ] == 'F' || szType[ 1 ] == 'f' )
         {
            bFooter = TRUE;
            if ( SetCursorFirstEntity( vNewReport,
                                       "Group", 0 ) != zCURSOR_SET )
            {
               fnCreateReportMetaEntity( vSubtask, vNewReport, "ga",
                                         zGROUP_REPORT, zPOS_AFTER );
            }
         }
      }

      nRC = SetCursorNextEntity( vNewReport, "GroupSet", 0 );
   }

   if ( bHeader == FALSE )
   {
      fnCreateReportMetaEntity( vSubtask, vNewReport, "PH",
                                zGROUPSET_REPORT, zPOS_FIRST );
      fnCreateReportMetaEntity( vSubtask, vNewReport, "ga",
                                zGROUP_REPORT, zPOS_AFTER );
   }

   if ( bFooter == FALSE )
   {
      fnCreateReportMetaEntity( vSubtask, vNewReport, "PF",
                                zGROUPSET_REPORT, zPOS_LAST );
      fnCreateReportMetaEntity( vSubtask, vNewReport, "ga",
                                zGROUP_REPORT, zPOS_AFTER );
   }

   return( 0 );
}

static zSHORT
fnTZRPUPDD_CreatePage( zVIEW vSubtask,
                       zVIEW vNewReportL )
{
   // If the page list modeless window is up, simply fill in the windows
   // for the list, otherwise display the first page in the report

#ifndef __WIN32__
   zVIEW  vPageList;
   if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) > 0 )
   {
      RefreshCtrl( vPageList, "ListBox1" );
      ActivateWindow( vPageList );

      // If only one page exists in the report file, paint it
      if ( SetCursorFirstEntity( vNewReportL, "Page", 0 ) == zCURSOR_SET &&
           SetCursorNextEntity( vNewReportL, "Page", 0 ) != zCURSOR_SET )
      {
         // Paint the first page in the report
         fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
      }
   }
   else
   {
      // Paint the first window in the report
      fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }
#else
   // If only one page exists in the report file, paint it
   if ( SetCursorFirstEntity( vNewReportL, "Page", 0 ) == zCURSOR_SET &&
        SetCursorNextEntity( vNewReportL, "Page", 0 ) != zCURSOR_SET )
   {
      // Paint the first page in the report
      fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }

   fnPainterCall( zMSG_REFRESHWINDOWLIST, vSubtask, 0, 0, 0 );
#endif

   return( 0 );
}


static zSHORT
fnTZRPUPDD_SetFlagsForReportView( zVIEW vSubtask,
                                  zVIEW vNewReport )
{
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   // BL, remove update flag: When you open a Zeidon Report is the
   // Report View update
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vNewReport->hViewCsr );
   lpViewOI  = (LPVIEWOI) SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   // if Report not checked out, set  View read only ->
   // the user cannot update the values in Detail Windows
   if ( !ComponentIsCheckedOut( vSubtask, vNewReport, zSOURCE_REPORT_META ))
   {
      zVIEW  vTZWINDOW;
      zVIEW  vTZOPENRPT;
      GetViewByName( &vTZWINDOW, "TZWINDOW", vSubtask, zLEVEL_TASK );
      GetViewByName( &vTZOPENRPT, "TZOPENRPT", vSubtask, zLEVEL_TASK );
      SetViewReadOnly( vNewReport );
      SetViewReadOnly( vTZWINDOW );
      SetViewReadOnly( vTZOPENRPT );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_NewPage( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vPageList;
   zVIEW  vTZPESRCO;
   zSHORT nIdx, nCursor;
   zCHAR  szTag[ 34 ];

   // Get the current open report
   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );

   // Generate a name for the new page
   if ((nCursor = CheckExistenceOfEntity( vReport, "Page" )) != zCURSOR_NULL )
   {
      strcpy_s( szTag, zsizeof( szTag ), "Page" );
      for ( nIdx = 1; nIdx < 1000; nIdx++ )
      {
         zltoa( (zLONG) nIdx, szTag + 6, zsizeof( szTag ) - 6 );
         if ( SetCursorFirstEntityByString( vReport, "Page", "Tag", szTag, 0 ) < zCURSOR_SET )
         {
            break;
         }
      }
   }
   else
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vReport, "Report", "Tag" );
   }

   // If a report is currently being processed by the painter, update
   // the current page from the C++ controls to the Zeidon controls
   // and then clean out the painter, create a new page and
   // paint the new page into the painter
   SetCursorLastEntity( vReport, "Page", 0 );
   CreateMetaEntity( vSubtask, vReport, "Page", zPOS_AFTER );
   SetAttributeFromString( vReport, "Page", "Tag", szTag );

   fnCreateReportMetaEntity( vSubtask, vReport, "PH", zGROUPSET_REPORT, zPOS_FIRST );
   fnCreateReportMetaEntity( vSubtask, vReport, "ga", zGROUP_REPORT, zPOS_AFTER );

   fnCreateReportMetaEntity( vSubtask, vReport, "PF", zGROUPSET_REPORT, zPOS_LAST );
   fnCreateReportMetaEntity( vSubtask, vReport, "ga", zGROUP_REPORT, zPOS_AFTER );

   // Call the painter to create a new Page
   SetNameForView( vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) > 0 )
      RefreshCtrl( vPageList, "ListBox1" );

   fnPainterCall( zMSG_CREATEZEIDONWINDOW, vSubtask, 0, 0, 0 );

   GetViewByName( &vReport, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   RefreshWindow( vReport );
   GetViewByName( &vReport, "TZPAGE_LIST", vSubtask, zLEVEL_TASK );
   RefreshWindow( vReport );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_NewReportFile( zVIEW vSubtask )
{
   zVIEW   vNewReport;
   zVIEW   vNewReportL;
   zVIEW   vPrevReport;
   zVIEW   vPainter;
   zVIEW   vMetaList;
   zVIEW   vLOD_LPLR;
   zCHAR   szFileName[ 256 ];
   zCHAR   szReportName[ 33 ];
   zSHORT  nIdx;

   // get the C++ class pointer to the painter object from the
   // zeidon control object
   GetViewByName( &vPainter, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vPainter,
                           "Palette", "NewFile" );
   if ( szFileName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "RP00202", "Report Maintenance",
                   "Must enter in a report name!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   // Truncate FileName to 8
   for ( nIdx = 0; nIdx < 8; nIdx++ )
   {
      if ( szFileName[ nIdx ] == ' ' )
         szFileName[ nIdx ] = '_';
   }

   szFileName[ 8 ] = 0;

   // There must be no other Report with same name.
   if ( GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_REPORT_META );
      SetNameForView( vMetaList, "CM_List", vSubtask, zLEVEL_TASK );
   }
   if ( SetCursorFirstEntityByString( vMetaList,
                                      "W_MetaDef", "Name",
                                      szFileName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "RP00220", "Report Maintenance",
                   "A Report by the same name already exists",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   // There must be no LOD with same name.
   RetrieveViewForMetaList( vSubtask, &vLOD_LPLR, zREFER_LOD_META );
   if ( SetCursorFirstEntityByString( vLOD_LPLR,
                                      "W_MetaDef", "Name",
                                      szFileName, 0 ) > zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "RP00221", "Report Maintenance",
                   "A LOD with the same name exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   strcpy_s( szReportName, zsizeof( szReportName ), szFileName );

   // Declare a view to the appropriate source report object type
   ActivateEmptyMetaOI( vSubtask, &vNewReport, zSOURCE_REPORT_META,
                        zSINGLE | zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vNewReport, "Report", zPOS_AFTER );
   SetAttributeFromString( vNewReport, "Report", "Tag", szReportName );

   SetAttributeFromAttribute( vNewReport, "Report", "Desc", vPainter, "Palette", "Desc" );

   SetAttributeFromString( vPainter, "Palette", "OpenedFile", szFileName );

   // Clear the painter windows
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, 0, 0 );

   // Drop the previous instance being worked on
   if ( GetViewByName( &vPrevReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevReport );

   // Name the view for mapping
   SetNameForView( vNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vNewReportL, vNewReport, 0 );
   SetNameForView( vNewReportL, "PAGEL", vSubtask, zLEVEL_TASK );

   // Call NewPage to do the rest
   TZRPUPDD_NewPage( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_DeletePage( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vReport2;
   zVIEW  vPageList;
   zPCHAR szTag;
   zCHAR  szMessage[ 100 ];

   // Get the view to the page list
   if ( GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( CheckExistenceOfEntity( vReport, "Page" ) != 0 )
      return( -1 );

   // Make operator confirm the delete
   strcpy_s( szMessage, zsizeof( szMessage ), "OK to delete Page '" );
   GetAddrForAttribute( &szTag, vReport, "Page", "Tag" );
   strcat_s( szMessage, zsizeof( szMessage ), szTag );
   strcat_s( szMessage, zsizeof( szMessage ), "'?" );

   if ( MessagePrompt( vSubtask, "RP00202", "Report Maintenance",
                       szMessage, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      return( -1 );
   }

   // Create a view to the page in the event the painter drops the current view we are using.
   CreateViewFromViewForTask( &vReport2, vReport, 0 );
   if ( fnPainterCall( zMSG_DELETECURRENTPAINTERWINDOW, vSubtask, 0, 0, 0 ) == 0 )
   {
      DeleteEntity( vReport2, "Page", zREPOS_NEXT );
      SetNameForView( vReport2, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( GetViewByName( &vPageList, "TZPAGE_LIST", vSubtask, zLEVEL_TASK ) > 0 )
         RefreshCtrl( vPageList, "ListBox1" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_DeletePageFromList( zVIEW vSubtask )
{
   zVIEW vReport;
   zSHORT nRC;

   nRC = GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // If the report doesn't exist, or a page doesn't exist then it can't be deleted, so return.
   if ( nRC < 1 || CheckExistenceOfEntity( vReport, "Page" ) != 0 )
      return( 0 );

   GetViewByName( &vReport, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   if ( TZRPUPDD_UpdatePageFromList( vSubtask ) == 0 )
   {
      nRC = TZRPUPDD_DeletePage( vSubtask );
      RefreshWindow( vReport );
      return( nRC );
   }

   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_RetrieveMetaList( zVIEW vSubtask )
{
   zVIEW vLPLR;

   if ( GetViewByName( &vLPLR, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_REPORT_META );
      SetNameForView( vLPLR, "CM_List", vSubtask, zLEVEL_TASK );
   }

   TZRPUPDD_SortByDate( vSubtask );
   if ( SetCursorFirstSelectedEntity( vLPLR, "W_MetaDef", 0 ) != zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vLPLR, "W_MetaDef", 0 ) == zCURSOR_SET )
      {
         SetSelectStateOfEntityForSet( vLPLR, "W_MetaDef", 1, 1 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW  vLPLR;
   zSHORT nRC;
   zVIEW  vSaveAs;

   // Ask and do save
   nRC = TZRPUPDD_AskForSave( vSubtask);
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if Report not checked out and user change this Report, then Zeidon
            // call the window "Save Report as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Report or Open an other Report or Switch Project or Exit Report Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zSwitchLPLRAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zNoActionAfterSaveAs );
         }
      }

      return( 0 );
   }

   // Send in flag to force reload of task view for LPLR.
   fnPainterCall( zMSG_DELETEALLPAINTERWINDOWS, vSubtask, 0, (zPCHAR) 1, 0 );

   if ( GetViewByName( &vLPLR, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vLPLR );

   // This SetName causes the list of dialogs to be presented after the
   // switch LPLR completes.
   SetNameForView( vSubtask, "__SysListReports", 0, zLEVEL_SYSTEM );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SystemMinimize( zVIEW vSubtask )
{
// TraceLineS("(tzpntrad) - SystemMinimize","");

   fnPainterCall( zMSG_SYSTEMMINIMIZE, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SystemActivate( zVIEW vSubtask )
{
   zVIEW    vReport;
   zVIEW    vPageList = 0;

   if ( fnPainterCall( zMSG_PAINTERINITIALIZED, vSubtask, 0, 0, 0 ) == 0 )
   {
//    TraceLineS( "(tzpntrad) - SystemActivate ", "Early return" );
      return( 0 );
   }
// else
//    TraceLineS( "(tzpntrad) - SystemActivate", "" );

   GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   fnCreateReportMappingEntities( vSubtask, vReport );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_SystemRestore( zVIEW vSubtask )
{
   TraceLineS("(tzpntrad) - SystemRestore","");

   fnPainterCall( zMSG_SYSTEMRESTORE, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_REPORT, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_EnablePainterWindows( zVIEW vSubtask )
{
   zVIEW   vMetaList = NULL;
   zVIEW   vPageList;
   zVIEW   vTZPESRCO;
   zVIEW   vPAGEL = 0;
   zSHORT  nRC;

   // Activate the presentation environment, if the presentation
   // environment is no longer with us
   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );

      if ( vMetaList )
      {
         nRC = LoadZeidonPPE( vSubtask, &vTZPESRCO, zREFER_PENV_META, vMetaList,
                              "Report Maintenance", "Painter exiting.");
         if ( nRC < 0 )
         {
            SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
            return( -16 );
         }

         SetNameForView( vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
         DropView( vMetaList );
      }

      fnPainterCall( zMSG_REPLACETOOLBARBUTTONS, vSubtask, 0, 0, 0 );
   }

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Zeidon Reports Maintenance", "PAGEL",
                              vPAGEL, "Report", zSOURCE_REPORT_META );

   if ( GetSubtaskForWindowName( vSubtask, &vPageList, "PageList" ) >= 0 )
   {
      SetCtrlState( vPageList, "ListBox1", zCONTROL_STATUS_ENABLED, 1 );
      RefreshCtrl( vPageList, "ListBox1" );
   }

   if ( GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK ) < 1 &&
        GetSubtaskForWindowName( vSubtask, &vPageList, "PageList" ) >= 0 )
   {
      ActivateWindow( vPageList );
      return( 0 );
   }

   return( (zSHORT) fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, "Y", 0 ) );
}

zOPER_EXPORT zSHORT OPERATION
SetToolBox( zVIEW vSubtask )
{
   zLONG lState = GetOptionState( vSubtask, "ToolBox", zOPTION_STATUS_CHECKED );
   lState = !lState;
   SetOptionState( vSubtask, "ToolBox", zOPTION_STATUS_CHECKED, lState );
// 1998.07.21 RG
// fnPainterCall( zMSG_SETTOOLBOX, vSubtask, 0, (zPCHAR) lState, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PageListClose( zVIEW vSubtask )
{
   zVIEW vMainWindow;

   if ( GetViewByName( &vMainWindow, "TZPNMWIN", vSubtask, zLEVEL_TASK ) >= zCURSOR_SET )
   {
      // set Check-Status for WindowList
      SetOptionState( vMainWindow, "PageList", zOPTION_STATUS_CHECKED, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_GROUP_SetPrintOptions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GROUP_SetPrintOptions( zVIEW vSubtask )
{
   zVIEW vReport;
   zBOOL bRepeatHeader   = TRUE;
   zBOOL bPageBreak      = TRUE;
   zBOOL bForcePageBreak = TRUE;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      return( 0 );

   if ( CompareAttributeToString( vReport, "Group", "Type", "gh" ) == 0 )
   {
      bPageBreak = FALSE;
      SetCtrlState( vSubtask, "PageBreak", zCONTROL_STATUS_CHECKED, 0 );
   }
   else
   if ( CompareAttributeToString( vReport, "Group", "Type", "ga" ) == 0 )
   {
      bRepeatHeader = FALSE;
      SetCtrlState( vSubtask, "RepeatHeader", zCONTROL_STATUS_CHECKED, 0 );
   }
   else
   if ( CompareAttributeToString( vReport, "Group", "Type", "gf" ) == 0 )
   {
      bRepeatHeader = FALSE;
      bPageBreak    = FALSE;
      SetCtrlState( vSubtask, "RepeatHeader", zCONTROL_STATUS_CHECKED, 0 );
      SetCtrlState( vSubtask, "PageBreak", zCONTROL_STATUS_CHECKED, 0 );
   }
   else
   {
      bRepeatHeader   = FALSE;
      bPageBreak      = FALSE;
      bForcePageBreak = FALSE;
      SetCtrlState( vSubtask, "RepeatHeader", zCONTROL_STATUS_CHECKED, 0 );
      SetCtrlState( vSubtask, "PageBreak", zCONTROL_STATUS_CHECKED, 0 );
      SetCtrlState( vSubtask, "ForcePageBreak", zCONTROL_STATUS_CHECKED, 0 );
   }

   SetCtrlState( vSubtask, "RepeatHeader", zCONTROL_STATUS_ENABLED, bRepeatHeader );
   SetCtrlState( vSubtask, "PageBreak", zCONTROL_STATUS_ENABLED, bPageBreak );
   SetCtrlState( vSubtask, "ForcePageBreak", zCONTROL_STATUS_ENABLED, bForcePageBreak );

   return( 0 );
} // UPD_GROUP_SetPrintOptions


zOPER_EXPORT zSHORT OPERATION
UPD_GROUP_PostBuild( zVIEW vSubtask )
{
   zVIEW vReport;
   zVIEW vWindow;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );
   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
   {
      SetCtrlState( vSubtask, "GroupType", zCONTROL_STATUS_ENABLED, FALSE );
      return( 0 );
   }

   // enable Controls for CtrlList
   SetCtrlState( vSubtask, "GroupType", zCONTROL_STATUS_ENABLED, TRUE );
   SetCtrlState( vSubtask, "Subtype", zCONTROL_STATUS_ENABLED, TRUE );

   // if GroupSet a Page Header or Page Footer, set Group Type = Entity
   if ( CompareAttributeToString( vReport, "GroupSet", "Type", "PH" ) == 0 ||
        CompareAttributeToString( vReport, "GroupSet", "Type", "PF" ) == 0 )
   {
      // if Window UPD_GROUPSET open, do not set correct Group Type
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "UPD_GROUPSET" ) < 0 )
      {
         SetAttributeFromString( vReport, "Group", "Type", "ga" );
         SetCtrlState( vSubtask, "GroupType", zCONTROL_STATUS_ENABLED, FALSE );
      }
   }
   else
   {
      // GroupSet is from Type "Entity", disable ComboBox for Subtype
      SetAttributeFromString( vReport, "Group", "SubType", "0" );
      SetCtrlState( vSubtask, "Subtype", zCONTROL_STATUS_ENABLED, FALSE );
   }

   UPD_GROUP_SetPrintOptions( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_GROUP_OK( zVIEW vSubtask )
{
   zCHAR   szOverlayTag[ 33 ];
   zVIEW   vReport;
   zVIEW   vReportC;
   zVIEW   vWindow;
   zCHAR   szType[ 3 ];
   zCHAR   szContext[ 33 ];
   zCHAR   szMsg[ 100 ];
   zSHORT  nRC;
   zLONG   lFlags;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vReport ) == 0 )
      return( 0 );

   GetStringFromAttribute( szType, zsizeof( szType ), vReport, "Group", "Type" );

   // Group Type  is required
   if ( zstrcmp( szType, "" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Group Type is required." );
      MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "GroupType" );
      return( -1 );
   }

   // check duplicate Group Header and duplicate Group Footer
   if ( zstrcmp( szType, "gh" ) == 0 || zstrcmp( szType, "gf" ) == 0 )
   {
      CreateViewFromViewForTask( &vReportC, vReport, 0 );
      SetCursorFirstEntityByString( vReportC, "Group", "Type", szType, "" );
      nRC = SetCursorNextEntityByString( vReportC, "Group", "Type", szType, "" );
      DropView( vReportC );
      if ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttributeByContext( szContext, vReport, "Group", "Type", "ReportGroupType", 32 );
         strcpy_s( szMsg, zsizeof( szMsg ), szContext );
         strcat_s( szMsg, zsizeof( szMsg ), " already exists in the GroupSet." );
         MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "GroupType" );
         return( -1 );
      }
   }

   GetIntegerFromAttribute( &lFlags, vReport, "Group", "Flags" );
   GetStringFromAttribute( szOverlayTag, zsizeof( szOverlayTag ), vReport, "Group", "OverlayTag" );
   if ( szOverlayTag[ 0 ] )
   {
      zVIEW  vReportTemp;
      zSHORT nRC;

      CreateViewFromViewForTask( &vReportTemp, vReport, 0 );
      nRC = SetCursorFirstEntityByString( vReportTemp, "Group", "Tag", szOverlayTag, 0 );
      DropView( vReportTemp );
      if ( nRC < zCURSOR_SET )
      {
         MessageSend( vSubtask, "", "Report Maintenance",
                      "Overlay Group Tag not found",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "Overlay" );
         return( 0 );
      }

      lFlags |= 0x00000001;
   }
   else
      lFlags &= ~0x00000001;

   SetAttributeFromInteger( vReport, "Group", "Flags", lFlags );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "UPD_GROUPSET" ) < 0 &&
        GetSubtaskForWindowName( vSubtask, &vWindow, "CtrlList" ) < 0 )
   {
      fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_GROUPSET_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GROUPSET_PostBuild( zVIEW vSubtask )
{
   zVIEW   vReport;
   zLONG   lFlags;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lFlags, vReport, "GroupSet", "Flags" );
   if ( lFlags )
   {
      SetCtrlState( vSubtask, "NewspaperColumn", zCONTROL_STATUS_CHECKED, TRUE );
   }

   return( 0 );

} // UPD_GROUPSET_PostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_GROUPSET_CheckValues
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GROUPSET_CheckValues( zVIEW vSubtask )
{
   zVIEW   vReport;
   zVIEW   vReportC;
   zLONG   lFlags;
   zCHAR   szType[ 3 ];
   zCHAR   szContext[ 33 ];
   zCHAR   szTag[ 33 ];
   zCHAR   szMsg[ 128 ];
   zSHORT  nDupCount;
   zSHORT  nRC;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szType, zsizeof( szType ), vReport, "GroupSet", "Type" );

   // GroupSet Type  is required
   if ( zstrcmp( szType, "" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "GroupSet Type is required." );
      MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "GroupSetType" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( GetCtrlState( vSubtask, "NewspaperColumn", zCONTROL_STATUS_CHECKED ) )
      lFlags = 1;
   else
      lFlags = 0;

   SetAttributeFromInteger( vReport, "GroupSet", "Flags", lFlags );

   if ( zstrcmp( szType, "PH" ) == 0 || zstrcmp( szType, "PF" ) == 0 )
   {
      // Check duplicate Page Header and duplicate Page Footer.
      CreateViewFromViewForTask( &vReportC, vReport, 0 );
      SetCursorFirstEntityByString( vReportC, "GroupSet", "Type", szType, "" );
      nRC = SetCursorNextEntityByString( vReportC, "GroupSet", "Type", szType, "" );
      if ( nRC >= zCURSOR_SET )
      {
         GetStringFromAttributeByContext( szContext, vReport, "GroupSet", "Type",
                                          "ReportGroupSetType", 32 );
         strcpy_s( szMsg, zsizeof( szMsg ), szContext );
         strcat_s( szMsg, zsizeof( szMsg ), " already exists in the Page." );
         MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "GroupSetType" );
         DropView( vReportC );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      // Check Group Type: if GroupSet Type = Page Header or Page Footer,
      // then Group does not exists from Type Group Header or Group Footer
      if ( SetCursorFirstEntityByString( vReportC, "Group", "Type", "gh", "" ) >= zCURSOR_SET ||
           SetCursorFirstEntityByString( vReportC, "Group", "Type", "gf", "" ) >= zCURSOR_SET )
      {
         GetStringFromAttributeByContext( szContext, vReport, "GroupSet", "Type",
                                          "ReportGroupSetType", 32 );
         strcpy_s( szMsg, zsizeof( szMsg ), szContext );
         strcat_s( szMsg, zsizeof( szMsg ), " GroupSet cannot include \na Group of Type " );
         GetStringFromAttributeByContext( szContext, vReportC, "Group", "Type",
                                          "ReportGroupType", 32 );
         strcat_s( szMsg, zsizeof( szMsg ), szContext );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "GroupSetType" );
         DropView( vReportC );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      DropView( vReportC );
   }

   if ( zstrcmp( szType, "E" ) == 0 )
   {
      // Check duplicate tags.
      nDupCount = 0;
      GetStringFromAttribute( szTag, zsizeof( szTag ), vReport, "GroupSet", "Tag" );
      CreateViewFromViewForTask( &vReportC, vReport, 0 );
      nRC = SetCursorFirstEntityByString( vReportC, "GroupSet", "Tag", szTag, "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {

         if ( CompareAttributeToString( vReportC, "GroupSet", "Type", szType ) == 0 )
         {
            nDupCount++;
         }

         nRC = SetCursorNextEntityByString( vReportC, "GroupSet", "Tag", szTag, "" );
      }

      if ( nDupCount >= 2 )
      {
         GetStringFromAttributeByContext( szContext, vReport, "GroupSet", "Type",
                                          "ReportGroupSetType", 32 );
         strcpy_s( szMsg, zsizeof( szMsg ), szContext );
         strcat_s( szMsg, zsizeof( szMsg ), " GroupSet named '" );
         strcat_s( szMsg, zsizeof( szMsg ), szTag );
         strcat_s( szMsg, zsizeof( szMsg ), "' already exists in the Page." );
         MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetFocusToCtrl( vSubtask, "Tag" );
         DropView( vReportC );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      DropView( vReportC );
   }

   return( 0 );

} // UPD_GROUPSET_CheckValues

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_GROUPSET_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GROUPSET_OK( zVIEW vSubtask )
{
   zVIEW   vReport;
   zLONG   lFlags;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );

   if ( UPD_GROUPSET_CheckValues( vSubtask ) < 0 )
      return( -1 );

   if ( GetCtrlState( vSubtask, "NewspaperColumn", zCONTROL_STATUS_CHECKED ) )
      lFlags = 1;
   else
      lFlags = 0;

   SetAttributeFromInteger( vReport, "GroupSet", "Flags", lFlags );

   // Accept control subobject
   AcceptSubobject( vReport, "GroupSet" );
   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );

   return( 0 );

} // UPD_GROUPSET_OK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_SaveAsPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZRPUP2D_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW    vPAGEL;
   zVIEW    vCM_List;
   zVIEW    vSaveAs;

   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_REPORT_META );
      SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );

   // Set current Description.
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );
   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc",
                              vPAGEL, "Report", "Desc" );

   return( 0 );

} // TZRPUP2D_SaveAsPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_SetSaveAsName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TZRPUP2D_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vSaveAs;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "SaveAsName8",
                              vCM_List, "W_MetaDef", "Name" );

   // Set Description.
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vCM_List, "W_MetaDef", "Desc" );
   }

   return( 0 );

} // TZRPUP2D_SetSaveAsName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZRPUP2D_SaveAsCheckName
//
/////////////////////////////////////////////////////////////////////////////
static zSHORT
fnTZRPUP2D_SaveAsCheckName( zVIEW  vSubtask,
                            zVIEW  vCM_List,
                            zVIEW  vPAGEL,
                            zPCHAR szOutName,
                            zPCHAR szNewName )
{
   zVIEW  vSaveAs;
   zSHORT nRC;
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   //Name is required
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Report Name is required." );
      MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edReportName" );
      return( -2 );
   }

   //Name already exists
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Report already exists. Replace existing Report?" );
      nRC = MessagePrompt( vSubtask, "ZO00138", "Report Maintenance",
                           szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edDialogName" );
         return( -1 );
      }

      //check the check out state
      if ( CompareAttributeToInteger( vCM_List, "W_MetaDef",
                                      "Status", 1 ) != 0 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "Report '" );
         strcat_s( szMsg, zsizeof( szMsg ), szNewName );
         strcat_s( szMsg, zsizeof( szMsg ), "' is not checked out." );
         MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                      szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edReportName" );
         return( -1 );
      }

      //if this Report open, then save Report, not save as
      if ( CompareAttributeToString( vPAGEL, "Report", "Tag", szOutName ) == 0 )
      {
         SetAttributeFromString( vPAGEL, "Report", "Tag", szOutName );
         SetAttributeFromAttribute( vPAGEL, "Report", "Desc", vSaveAs, "ActionAfterSaveAS", "Desc" );
         TZRPUPDD_SaveReportFile( vSubtask );
         return( 0 );
      }
      else
      {
         // if another Report open, then replace existing Report
         nRC = DeleteMetaOI( vSubtask, vCM_List, zREFER_REPORT_META );
      }

   }

   return( 1 );

} // fnTZRPUP2D_SaveAsCheckName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_SaveAsReport
//
//    save Report under another name
//
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
TZRPUP2D_SaveAsReport( zVIEW vSubtask )
{
   zVIEW  vPAGEL;
   zVIEW  vPAGEL_New;
   zVIEW  vCM_List;
   zVIEW  vSaveAs;
   zVIEW  vTaskLPLR;
   zCHAR  szNewName[ 33 ];
   zCHAR  szOutName[ 33 ];
   zVIEW  vProfileXFER;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // Validate Report Name is OK
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   // check Save as Name
   if ( fnTZRPUP2D_SaveAsCheckName( vSubtask, vCM_List, vPAGEL, szOutName, szNewName ) < 1 )
   {
      return( -1 );
   }

   TZRPUP2D_SaveNewReport( vSubtask, &vPAGEL_New, szOutName, vPAGEL, vTaskLPLR );

   fnPainterCall( zMSG_RESTORE_PLACEMENT, vSubtask, 0, (zPCHAR) vProfileXFER, 0 );

   fnTZRPUP2D_CheckActionAfterSaveAs( vSubtask );

   return( 0 );

} // TZRPUP2D_SaveAsReport

zOPER_EXPORT zSHORT OPERATION
TZRPUP2D_SaveNewReport( zVIEW     vSubtask,
                        zPVIEW    pvNewReport,
                        zPCHAR    szNewReportName,
                        zVIEW     vOrigReport,
                        zVIEW     vSourceLPLR )
{
   zVIEW     vSourceLPLR2;
   zVIEW     vCM_List;
   zVIEW     vSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vSourceLPLR2, vSourceLPLR, 0 );

   // Call operation that will actually do the work of creating the report.
   oTZRPSRCO_ReportCopy( pvNewReport, vOrigReport, vSourceLPLR2, vSubtask );
   SetAttributeFromString( *pvNewReport, "Report", "Tag", szNewReportName );
   SetAttributeFromString( *pvNewReport, "Page", "Tag", szNewReportName );

   SetAttributeFromAttribute( *pvNewReport, "Report", "Desc",
                              vSaveAs, "ActionAfterSaveAS", "Desc" );

   // Clean up views.
   DropView( vSourceLPLR2 );

   SetNameForView( *pvNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "PAGEL", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );

   TZRPUPDD_SaveReportFile( vSubtask );

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "CPLR_ZKey",
                               *pvNewReport, "Report", "ZKey", "" );

   TZRPUPDD_OpenReportFile( vSubtask );
   DropObjectInstance( vOrigReport );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_Check_CheckOutStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_Check_CheckOutStatus( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zSHORT  nEnable = 0;
   zBOOL   bEnable = FALSE;

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
   {
         nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_List );
         if ( nEnable )
            bEnable = TRUE;
   }

   SetCtrlState( vSubtask, "Delete", zOPTION_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "DeleteReport", bEnable );

   return( 0 );

} // TZRPUP2D_Check_CheckOutStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_CheckNameForCheckOut
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_CheckNameForCheckOut( zVIEW vSubtask )
{
   zSHORT  nEnable = 1;
   zVIEW   vCM_List;
   zVIEW   vCM_List_Copy;
   zCHAR   szNewName[ 33 ];
   zCHAR   szOutName[ 33 ];

   GetCtrlText( vSubtask, "edReportName", szNewName, 33 );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vCM_List_Copy, vCM_List, 0 );

   if ( SetCursorFirstEntityByString( vCM_List_Copy, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      // set check out state in Title
      nEnable = CheckOutStateForCurrentWindow( vSubtask, vCM_List_Copy );
   }
   else
   {
      SetWindowCaptionTitle( vSubtask, "Save Report as", "" );
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );

   DropView( vCM_List_Copy );

   return( 0 );
} // TZRPUP2D_CheckNameForCheckOut


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_SaveAsCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW   vSaveAs;

   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                               "Typ", zNoActionAfterSaveAs );

      SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );
   }

   return( 0 );
} // TZRPUP2D_SaveAsCancel



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_OpenReport
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_OpenReport( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = TZRPUPDD_AskForSave( vSubtask );
   if ( nRC )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if Report not checked out and user change this Report, then Zeidon
            // call the window "Save Report as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Report or Open an other Report or Switch Project or
            // Exit Report Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zOpenComponentAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zNoActionAfterSaveAs );
         }
      }
   }

   return( 0 );
} // TZRPUP2D_OpenReport



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_NewReport
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_NewReport( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = TZRPUPDD_AskForSave( vSubtask );
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
         {
            // if Report not checked out and user change this Report, then Zeidon
            // call the window "Save Report as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Report or Open an other Report or Switch Project or
            // Exit Report Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zNewComponentAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zNoActionAfterSaveAs );
         }
      }

      return( 0 );
   }

   return( 0 );

} // TZRPUP2D_NewReport

static zSHORT
fnTZRPUP2D_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vPAGEL;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "Zeidon Reports Maintenance", "PAGEL",
                              vPAGEL, "Report", zSOURCE_REPORT_META );

   // if Report not checked out and user change this Report, then zeidon
   // call the window "Save Report as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Report or Open an other Report or Switch Project or Exit
   // Report Tool)

   GetIntegerFromAttribute( &lActionAfterSaveAs, vSaveAs, "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zNoActionAfterSaveAs );

   switch ( lActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new Report
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow, "TZRPUPZD", "ReportNew" );
         break;

      case zOpenComponentAfterSaveAs:   // Open Report
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow, "TZRPUPZD", "ReportList" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         TZRPUPDD_SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow, "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit Report Tool
         TZRPUPDD_ExitReport( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_SaveAsKeepCurrentDesc
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vPAGEL;
   zVIEW  vSaveAs;

   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute( vSaveAs, "ActionAfterSaveAS", "Desc", vPAGEL, "Report", "Desc" );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   return( 0 );

} // TZRPUP2D_SaveAsKeepCurrentDesc

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_UpdateZeidonWindows
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_UpdateZeidonWindows( zVIEW vSubtask )
{
   zVIEW   vProfileXFER;

   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   fnPainterCall( zMSG_UPDATEALLZEIDONWINDOWS, vSubtask, 0, 0, 0 );
   fnPainterCall( zMSG_SAVE_PLACEMENT, vSubtask, 0, (zPCHAR) vProfileXFER, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PrebuildCtrlList( zVIEW vSubtask )
{
   zVIEW vReport;

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );
   while ( vReport && ResetViewFromSubobject( vReport ) == 0 )
   {
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_DeleteControls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_DeleteControls( zVIEW vSubtask )
{
   fnPainterCall( zMSG_DELETE_SELECTED, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_DeleteControls

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AlignLeft
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AlignLeft( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_LEFT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AlignLeft


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AlignRight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AlignRight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_RIGHT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AlignRight


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AlignTop
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AlignTop( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_TOP, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AlignTop


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AlignBottom
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AlignBottom( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ALIGN_BOTTOM, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AlignBottom


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_EqualHeight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_EqualHeight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_HEIGHT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_EqualHeight


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_EqualWidth
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_EqualWidth( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_WIDTH, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_EqualWidth


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_EqualWidthHeight
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_EqualWidthHeight( zVIEW vSubtask )
{
   fnPainterCall( zMSG_SIZE_WIDTH_HEIGHT, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_EqualWidthHeight


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_EquiSpaceVertical
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_EquiSpaceVertical( zVIEW vSubtask )
{
   fnPainterCall( zMSG_EQUAL_SPACE_VERTICAL, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_EquiSpaceVertical



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_EquiSpaceHorizontal
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_EquiSpaceHorizontal( zVIEW vSubtask )
{
   fnPainterCall( zMSG_EQUAL_SPACE_HORIZONTAL, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_EquiSpaceHorizontal


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AbutVertical
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AbutVertical( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ABUT_VERTICAL, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AbutVertical


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUP2D_AbutHorizontal
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUP2D_AbutHorizontal( zVIEW vSubtask )
{
   fnPainterCall( zMSG_ABUT_HORIZONTAL, vSubtask, 0, 0, 0 );

   return( 0 );
} // TZRPUP2D_AbutHorizontal


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GENERATE_XSLT_File
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GENERATE_XSLT_File( zVIEW vSubtask )
{
   zVIEW  vTZRPSRCO;
   SHORT  nRC;

   nRC = GetViewByName( &vTZRPSRCO, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = oTZRPSRCO_GenerateXSLT( vTZRPSRCO );
   }

   return( 0 );
} // GENERATE_XSLT_File


/*************************************************************************************************
**
**    OPERATION: CreateEntityGroupSet
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreateEntityGroupSetA( zVIEW vViewToWindow )
{
   zVIEW  vTZRPSRCO;
   SHORT  nRC;

   TraceLineS("*** CreateEntityGroupSet *** ", "");

   nRC = GetViewByName( &vTZRPSRCO, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = oTZRPSRCO_CreateEntityGroupSet( vTZRPSRCO );
   }
   return( 0 );
} // CreateEntityGroupSet




/*************************************************************************************************
**
**    OPERATION: DeleteGroupSetForEntity
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DeleteGroupSetForEntity( zVIEW vViewToWindow )
{

   zVIEW  vReportDef;
   SHORT  nRC;

   TraceLineS("*** CreateEntityGroupSet *** ", "");

   nRC = GetViewByName( &vReportDef, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = oTZRPSRCO_DeleteGroupSet( vReportDef );
   }

   return( 0 );
} // DeleteGroupSetForEntity



/*************************************************************************************************
**
**    OPERATION: SetGroupSetEntity
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetGroupSetEntity( zVIEW vViewToWindow )
{

   zVIEW  vReport;
   zVIEW  vReportTmp;
   SHORT  nRC;
   zCHAR  szTest[ 33 ];

   nRC = GetViewByName( &vReport, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      // WHEN OUTLINER IS DrivingLOD (LOD_Entity)
      nRC = GetViewByName( &vReportTmp, "DrivingLOD", vViewToWindow, zLEVEL_TASK );
      GetStringFromAttribute( szTest, zsizeof( szTest ), vReportTmp, "LOD_EntityParent", "Name" );
      SetAttributeFromAttribute( vReport, "DrivingViewObjRef", "wGroupSetName",
                                 vReportTmp, "LOD_EntityParent", "Name" );
      // Since we are selecting a lod entity, this will be an entity group set.
      SetAttributeFromString( vReport, "DrivingViewObjRef", "wGroupSetType", "E" );

      /* // WHEN OUTLINER IS TZRPTLODENT (FullReportEntity)
      nRC = GetViewByName( &vReportTmp, "TZRPTLODENT", vViewToWindow, zLEVEL_TASK );
      GetStringFromAttribute( szTest, zsizeof( szTest ), vReportTmp, "FullReportEntity", "Name" );
      SetAttributeFromAttribute( vReport, "DrivingViewObjRef", "wGroupSetName",
                                 vReportTmp, "FullReportEntity", "Name" );
      // Since we are selecting a lod entity, this will be an entity group set.
      SetAttributeFromString( vReport, "DrivingViewObjRef", "wGroupSetType", "E" );
      */
   }
   return( 0 );
} // SetGroupSetEntity



/*************************************************************************************************
**
**    OPERATION: GOTO_AddGroupSet
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
GOTO_AddGroupSet( zVIEW vViewToWindow )
{
   zVIEW  vReportTmp;
   zVIEW  vReport;
   SHORT  nRC;

   nRC = GetViewByName( &vReport, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = GetViewByName( &vReportTmp, "TZRPTLODENT", vViewToWindow, zLEVEL_TASK );
      if ( nRC >= 0 )
         DropView( vReportTmp );
      CreateViewFromViewForTask( &vReportTmp, vReport, 0 );
      SetNameForView( vReportTmp, "TZRPTLODENT", vViewToWindow , zLEVEL_TASK );
   }

   return( 0 );
} // GOTO_AddGroupSet


/*************************************************************************************************
**
**    OPERATION: CreatePageHeader
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreatePageHeader( zVIEW vViewToWindow )
{
   zVIEW  vReportDef;
   SHORT  nRC;

   nRC = GetViewByName( &vReportDef, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = oTZRPSRCO_CreatePageHeader( vReportDef );
   }

   return( 0 );
} // CreatePageHeader


/*************************************************************************************************
**
**    OPERATION: CreatePageFooter
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreatePageFooter( zVIEW vViewToWindow )
{
   zVIEW  vReportDef;
   SHORT  nRC;

   nRC = GetViewByName( &vReportDef, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      nRC = oTZRPSRCO_CreatePageFooter( vReportDef );
   }

   return( 0 );
} // CreatePageFooter




/*************************************************************************************************
**
**    OPERATION: ChangeDrivingView
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ChangeDrivingView( zVIEW vViewToWindow )
{
   zVIEW  vReportDef;

   GetViewByName( &vReportDef, "TZOPENRPT", vViewToWindow, zLEVEL_TASK );

   // Execute routine to initialize Report Display subobject data, as necessary.
   oTZRPSRCO_InitReportStructure( vReportDef );

   return( 0 );
} // ChangeDrivingView


#ifdef __cplusplus
}
#endif
