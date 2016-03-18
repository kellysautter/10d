//
// MODULE NAME:  TZRPUPSD  -  Report C dialog operations for Siron
// //////////////////////////////////////////////////////////////////////////
// *  Copyright (c) 1997 - 2016 QuinSoft, Inc. All rights reserved. Confidential *
// * and Proprietary material subject to license - do not reproduce or      *
// * disclose.  This material is an unpublished work and is considered a    *
// * trade secret belonging to the copyright holder.                        *
// //////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Guenter Trageser
//  DATE:    11.03.97
//  API:     Zeidon OE, Siron Message Lib
//  ENVIRONMENT: Windows 3.1, Windows NT
//
//  LAST MESSAGE ID: RP00011
//
//  CHANGE NOTES:
//    11.03.97 Guenter Trageser
//      New functions for start of Siron RDT (Report Design Tool)
//    04.06.97 Guenter Trageser
//      Use Siron calls from KZSIRREP
//      This allows you to start the report tool, if no Siron is
//      installed on your machine, cause the Siron DLL SIRDMSGC is now loaded
//      dynamically.
//    17.06.97 Guenter Trageser
//      correct error message, if SIRON is not installed (or not in path)
//
/*
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2002.08.09 BL
   avoid "ViewIsReadOnly" if a meta is not checked out.
2001.02.15    BL    R54697
   improve handling of return code from InitializeLPLR
2001.02.13 RG
   Removed Compiler Warnings
2001.01.31 RG  Z2000
   Send an error message if a ZeidonReport should be opened as SironReport
   Bug: F51660
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  SR  Z2000 Size of path variables
   Modified size of all filename and path variables in whole module
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
2000.08.17  RG    Z2000
   We have to use the right pathnames to change the filename in *.REP files
2000.08.14  RG    Z2000
   Change filename in *.REP file for function 'Save As' of a report
2000.07.18  BL    Z10 Repository
   Added TZRPUPSD_SetCheckOutState and modified Operation: if Siron
   Report not checked out disable all function for update
2000.07.13   BL    Z10   Save as
   Added TZRPUPSD_SaveAsKeepCurrentDesc, TZRPUPSD_SaveAsSetDefaults,
   fnTZRPUPSD_SaveAsCheckStatus, fnTZRPUPSD_SaveAsCheckSironFiles,
   fnTZRPUPSD_SaveAsGetFileName, zwTZRPUPSD_SaveAsCopySironFiles and
   modified all save as Functions for copy source files
2000.06.30   BL    Z10   Repository and Tools
   Set check out state in title. Is Report not checked out, disable Delete
   and Save Action  and ask for save as
2000.06.13  BL    Z10
   Modified TZRPUPSD_ReportPreBuild: if Workstation Administration open,
   do not open Report tool
2000.06.05  DKS   Z10
   Send in flag to force reload of task view for LPLR (uncomment?)
2000.01.13  BL    Z10   new PPE handling
   Modified TZRPUPSD_ReportPreBuild and TZRPUPSD_EnablePainterWindows
   for new PPE handling
1999.11.06  BL    Z10   new menu structre
   Modified Operation TZRPUPSD_OpenReportFile, TZRPUPSD_ReportPostBuild,
   TZRPUPSD_NewReportFile and TZRPUPSD_EnablePainterWindows for set LPLR and
   Report Name in main window
   Added Operation TZRPUPSD_SaveAsPostBuild, TZRPUPSD_SetSaveAsName,
   TZRPUPSD_SaveAsReport and TZRPUPSD_SaveNewReport for save Report as
1999.01.11  RG
   Inserted the call of operation 'SironTerminate' to stop the Siron Compiler
   at the end of the application
1999.09.30  DKS   Z10   QS999
   After SwitchLPLR, display Report List.  Remove references to PageList
   option.
07.05.1998  DKS
   Synchronize code with that of dialogs to facilitate parallel maintenance
1998.09.07  RG
   Setting the call of fnPainterCall in TZRPUPSD_OpenReportFile into comments
   because of 'Assertion failed' while opening a SironReport File
*/

#define RDT_VIEW "SIRON$RDT$TOOL"
#define RDT_ENTITY "SirPara"

#define zGROUPSET_REPORT         3000L  // from zdrvropr.h
#define zGROUP_REPORT            3010L  // from zdrvropr.h
#define zGLOBAL_DATA
#define  KZOESYS_INCL
#define  zNO_KZOEINCO    // to get the correct operation thru zGET_PTR

// We need to define MutexInfo as a dummy typedef.  DO NOT TRY TO USE THIS!
typedef long MutexInfo;

#include "kzoengaa.h"
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzctlopr.h"
#include "ZeidonOp.H"

#ifndef QUINSOFT

#include <kzsirrep.h>  // the Siron calls
#include <io.h>        // for access()


// global Variables (for Siron Callback)
static zVIEW g_vSubtask;         // View to main window (for access to
                                 // message bar in callback functions)
static zBOOL g_bSironConnected;  // is Siron available ?

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_AskForSave( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_UpdatePageFromList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SortByDate( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_OpenReportFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveReportFile( zVIEW vSubtask );

static void
fnCreateReportMetaEntityRDT( zVIEW   vReport,
                          zCPCHAR cpcType,
                          zLONG   lPPE_Type,
                          zSHORT  nPos );

static void
fnSetMenuState( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_ShowLOD_List( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SelectLOD( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_CancelLOD_List( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironRDT( zVIEW vSubtask );

void OPERATION
TZRPUPSD_TermSironRDT( zLONG, zLONG );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironTest( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironQuery( zVIEW vSubtask );

void OPERATION
TZRPUPSD_TermSironQuery( zLONG, zLONG );

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveNewReport( zVIEW     vSubtask,
                        zPVIEW    pvNewReport,
                        zPCHAR    szNewReportName,
                        zVIEW     vOrigReport,
                        zVIEW     vSourceLPLR );

zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_ReportCopy( zVIEW     vSubtask,
                      zPVIEW    pvNewReport,
                      zVIEW     vOrigReport,
                      zVIEW     vSourceLPLR );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW    vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szLodname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );

static zSHORT
zwfnTZRPUPSD_CheckActionAfterSaveAs( zVIEW  vSubtask );

static zSHORT
fnTZRPUPSD_SaveAsCheckName( zVIEW  vSubtask,
                            zVIEW  vSaveAs,
                            zPCHAR szOutName );

static zSHORT
fnTZRPUPSD_SaveAsCheckStatus( zVIEW    vSubtask,
                              zVIEW    vCM_List,
                              zPCHAR   szOutName );

static zSHORT
fnTZRPUPSD_SaveAsCheckSironFiles( zVIEW    vSubtask,
                                  zVIEW    vTZRPSRCO,
                                  zVIEW    vSaveAs );

static zSHORT
fnTZRPUPSD_SaveAsGetFileName( zVIEW  vView,
                              zPCHAR szEntity,
                              zPCHAR szAttribute,
                              zPCHAR szExtension,
                              zPCHAR szSourceFileName );

static zSHORT
zwTZRPUPSD_SaveAsCopySironFiles( zVIEW  vTZRPSRCO,
                                 zVIEW  vSaveAs );


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_ReportPreBuild( zVIEW vSubtask )
{
   zVIEW      vProfileXFER;
   zVIEW      vMetaList;
   zVIEW      vTZPESRCO;
   zVIEW      vRDT;
   zVIEW      vSaveAs;
   zPCHAR     szLPLR_Name;
   zSHORT     nRC;

   // create the work LOD for RDT parameters
   // Only one instance of the report painter is allowed to be active
   // This named view is an indicator, which shows that a report painter is
   // running.
   if ( GetViewByName( &vRDT, RDT_VIEW, vSubtask, zLEVEL_SYSTEM ) < 0 )
   {
      // create it
      ActivateEmptyObjectInstance( &vRDT, "KZSIRRPO", vSubtask, zLEVEL_SYSTEM);
      CreateEntity( vRDT, RDT_ENTITY, zPOS_FIRST);

      SetNameForView( vRDT, RDT_VIEW, vSubtask, zLEVEL_SYSTEM );
   }
   else
   {
      SysMessageBox( "Report Maintenance",
                     "Only one instance of Report painter is allowed!", 1);
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( -1 );
   }

   // Get the Profile information and
   // Call configuration management to set up the default LPL
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   szLPLR_Name = 0;
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

   if ( vMetaList )
      SetNameForView( vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Activate the presentation environment
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

   // if Report not checked out and user change this Report, then zeidon
   // call the window "Save Report as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Report or Open an other Report or Switch Project or
   // Exit Report Tool)
   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vSaveAs, "TZSAVEAS", vSubtask, zSINGLE );
      SetNameForView( vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
      CreateEntity( vSaveAs, "ActionAfterSaveAS", zPOS_AFTER );
   }
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS", "Typ", zNoActionAfterSaveAs );

   return( 0 );
}


zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_CreateTZPNTROO( zVIEW vSubtask )
{
   zVIEW vTZPESRCO;
   zVIEW vTZPNTROO;

   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      ActivateEmptyObjectInstance( &vTZPNTROO, "TZPNTROO", vTZPESRCO, zSINGLE );
      CreateEntity( vTZPNTROO, "Palette", zPOS_AFTER );
      SetAttributeFromBlob( vTZPNTROO, "Palette", "Pointer",
                            &vTZPESRCO, zsizeof( void * ) );
      SetNameForView( vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_ReportPostBuild( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zVIEW  vTZPNCTWO;
   zVIEW  vProfileXFER;
   zVIEW  vPAGEL;
   zLONG  lRC;

   // Connect to Siron
   lRC = SironConnect( vSubtask );
   if ( lRC != 0 )
   {
      MessageSend( vSubtask, "RP00001", "Report Maintenance",
                   "Can't connect to Siron",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
   }
   else
   {
     g_bSironConnected = TRUE;
   }

   // setup the List of Reports
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // Name the subtask associated with the main page so we can
   // set its title later
   SetNameForView( vSubtask, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );
   SetTitleWithCheckOutState( vSubtask, "Siron Reports Maintenance", "TZRPSRCO",
                              vPAGEL, "Report", zSOURCE_REPORT_META );

   // Transfer to the open report window
   if ( vMetaList )
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                               zWAB_ProcessImmediateAction,
                               "TZRPUPDD", "ReportList" );

   //  Initialize the TZPNCTWO work object.
   ActivateEmptyObjectInstance( &vTZPNCTWO, "TZPNCTWO", vSubtask,
                                zSINGLE );
   CreateEntity( vTZPNCTWO, "TZPNCTWO", zPOS_AFTER );
   SetAttributeFromString( vTZPNCTWO, "TZPNCTWO", "PainterType", "R" );
   SetNameForView( vTZPNCTWO, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK ) > 0 )
   {
      zLONG lFlags;
      lFlags = RestoreWindowPlacement( vSubtask, vProfileXFER );
   }

   // In the original Report Tool, the painter call creates a work object.
   // We do not use the painter call. We create the work object ourself.
   TZRPUPSD_CreateTZPNTROO( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_OpenReportFile( zVIEW vSubtask )
{
   zVIEW       vNewReport;
   zVIEW       vMetaList;
   zVIEW       vPrevReport;
   zVIEW       vIdleView;
   zVIEW       vTZPNTROO;
   zVIEW       vTZPNMWIN;
   zPCHAR      pszLOD;
   zCHAR       szMsg[40];
   LPVIEWCSR   lpViewCsr;
   LPVIEWOI    lpViewOI;

   zCHAR szSironName[ 33 ];

   // Drop the previous instance being worked on
   if ( GetViewByName( &vPrevReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevReport );

   // Call configuration management to open the selected file
   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   // If there are no W_MetaDefs then one can't be opened.
   if ( CheckExistenceOfEntity( vMetaList, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   if ( ActivateMetaOI( vSubtask, &vNewReport, vMetaList,
                        zSOURCE_REPORT_META, zSINGLE | zLEVEL_TASK ) != 1 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   //RG: If attribute SironName is empty, it's a ZeidonReport
   GetStringFromAttribute( szSironName, zsizeof( szSironName ), vNewReport, "Report", "SironName");
   if (zstrcmp (szSironName, "") == 0)
   {
      // The Report is a ZeidonReport
      MessageSend( vSubtask, "RP00011", "Report Maintenance",
                   "Cannot open Report. It's a Zeidon Report.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // BL, remove update flag: When you open a Zeidon Report in the Siron
   // Report Tool, is the Report View update
   lpViewCsr = (LPVIEWCSR) SysGetPointerFromHandle( vNewReport->hViewCsr );
   lpViewOI  = SysGetPointerFromHandle( lpViewCsr->hViewOI );
   lpViewOI->bUpdatedFile = FALSE;

   // if Report not checked out, set  View read only ->
   // the user cannot update the values in Detail Windows
   if ( !ComponentIsCheckedOut( vSubtask, vNewReport, zSOURCE_REPORT_META ))
      SetViewReadOnly( vNewReport );

   if ( GetViewByName( &vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        vIdleView == vSubtask )
   {
      DropNameForView( vIdleView, "__SysIdlePainter", 0, zLEVEL_SYSTEM );
   }

   // Set the current opened file name
   GetViewByName( &vTZPNMWIN, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK );
   SetAttributeFromAttribute( vTZPNTROO, "Palette", "OpenedFile",
                              vMetaList, "W_MetaDef", "Name" );

   // Name the view for mapping.
   SetNameForView( vNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "PAGEL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vNewReport, "LOD" ) >= 0 &&
        GetAddrForAttribute( &pszLOD, vNewReport, "LOD", "Name") == 0)
   {
     sprintf( szMsg, "Base LOD: %s", pszLOD);
   }
   else
   {
     sprintf( szMsg, "No LOD" );
   }

   MB_SetMessage( vTZPNMWIN, 1, " " );
   MB_SetMessage( vTZPNMWIN, 2, szMsg );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveReportFile( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vReportConvert;

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   CreateViewFromViewForTask( &vReportConvert, vReport, 0 );
   AssignZKeyToMetaOI( vSubtask, vReportConvert, zSOURCE_REPORT_META, 0 );

   ResetView( vReportConvert );

   if ( CommitMetaOI( vSubtask, vReportConvert, zSOURCE_REPORT_META ) < 0 )
   {
      MessageSend( vSubtask, "RP00003", "Report Maintenance",
                   "Unable to save Report",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -2 );
   }

   DropView( vReportConvert );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_DeleteCurrentPageWnd( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_UpdatePageFromList( zVIEW vSubtask )
{

   // we do not use it anymore
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SelectPageFromList( zVIEW vSubtask )
{

   // no more used
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SortWindows( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_ExitReport( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vProfileXFER;
   zVIEW  vLPLR;
   zVIEW  vRDT;
   zVIEW  vIdlePainter;
   zVIEW  vTZPNTROO;
   zVIEW  vSaveAs;
   zLONG  lFlags;
   zSHORT nRC;

   if ( GetViewByName( &vIdlePainter, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) < 1 )
      vIdlePainter = 0;

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nRC = TZRPUPSD_AskForSave( vSubtask );

      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC  != 0 )
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

   // Now save the Profile Info...
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );

   // until scrollbar and toolbar options exists, always show toolbar???
   lFlags = 0x00000002;

   SaveWindowPlacement( vSubtask, vProfileXFER, lFlags );

   if ( CompareAttributeToString( vProfileXFER, "WD", "StartupLPLR_Option",
                                  "L" ) == 0 )   //set Last LPLR
   {
      GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      if ( vLPLR )
      {
         SetAttributeFromAttribute( vProfileXFER, "WD", "StartupLPLR_Name",
                                    vLPLR, "LPLR", "Name" );
      }
   }

   oTZ__PRFO_CommitProfile( vProfileXFER );

   // In the original Report Tool, the painter destroys the work object.
   // We do not use the painter call. We destroy the work object ourself.
   if ( GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vTZPNTROO );

   if ( vReport )
      DropMetaOI( vSubtask, vReport );

   TerminateLPLR( vSubtask );

   // delete the work LOD for RDT tool
   if ( GetViewByName( &vRDT, RDT_VIEW, vSubtask, zLEVEL_SYSTEM ) >= 0 )
   {
      DropObjectInstance( vRDT );
   }

   // Disconnect from Siron
   if (g_bSironConnected)
   {
     SironTerminate ( vSubtask, 1);
     SironDisconnect( vSubtask );
     g_bSironConnected = FALSE;
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_AskForSave( zVIEW vSubtask )
{
   zVIEW  vReport;
   zSHORT nRC;
   zBOOL  nSaveAs = FALSE;
   zCHAR  szReportName[33];
   zCHAR  szMessageText[ zMAX_MESSAGE_LTH + 1 ];

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

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
          nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_REPORT,
                              zREFER_REPORT_META,
                              vReport, "Report", "Tag", 0 );

      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( nRC );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            // if Report not checked out and user would save it, then call
            // window save Report as
            if ( nSaveAs )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                        "TZRPUPDD", "SaveAs" );
               return( 99 );
            }

            nRC = TZRPUPSD_SaveReportFile( vSubtask );
            if ( nRC < 0 )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
               return( nRC );
            }
         }
         else
            return( 0 );
      } //endif ( nRC == zRESPONSE_CANCEL )
   }
   else  // no update
      nRC = 0;

   return( nRC );
}


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_DeleteReport( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );

   /* If there are no W_MetaDefs then one can't be opened. */
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
TZRPUPSD_SortByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zASCENDING, vMetaList,
                                   "W_MetaDef", "Name", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_PageListPopup( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_PrebuildPageList( zVIEW vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_DisplayPageList( zVIEW vSubtask )
{
   return( 0 );
}

#define zGROUPSET_REPORT                    3000L  // from zdrvropr.h
#define zGROUP_REPORT                       3010L  // from zdrvropr.h

// Don ... is this all that is needed in terms of attributes???
static void
fnCreateReportMetaEntityRDT( zVIEW   vReport,
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


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_NewPage( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vTZPESRCO;
   zSHORT nIdx, nCursor;
   zCHAR  szTag[ 34 ];

   // Get the current open report
   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );

   // Generate a name for the new page
   if ((nCursor = CheckExistenceOfEntity( vReport, "Page" )) !=
                                                            zCURSOR_NULL )
   {
      strcpy_s( szTag, zsizeof( szTag ), "Page" );
      for ( nIdx = 1; nIdx < 1000; nIdx++ )
      {
         zltoa((zLONG) nIdx, szTag + 6, zsizeof( szTag ) - 6, 10 );
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

   // Call the painter to create a new Page
   SetNameForView( vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_NewReportFile( zVIEW vSubtask )
{
   zVIEW   vNewReport;
   zVIEW   vPrevReport;
   zVIEW   vPainter;
   zVIEW   vMetaList;
   zVIEW   vLOD_LPLR;
   zCHAR   szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR   szReportName[ 33 ];
   zSHORT  nIdx;

   // get the C++ class pointer to the painter object from the
   // zeidon control object
   GetViewByName( &vPainter, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vPainter, "Palette", "NewFile" );
   if ( !szFileName[ 0 ] )
   {
      MessageSend( vSubtask, "RP00004", "Report Maintenance",
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
      MessageSend( vSubtask, "RP00005", "Report Maintenance",
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
      MessageSend( vSubtask, "RP00006", "Report Maintenance",
                   "A LOD with the same name exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -2 );
   }

   strcpy_s( szReportName, zsizeof( szReportName ) szFileName );

   // Declare a view to the appropriate source report object type
   ActivateEmptyMetaOI( vSubtask, &vNewReport, zSOURCE_REPORT_META,
                        zSINGLE | zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vNewReport, "Report", zPOS_AFTER );
   SetAttributeFromString( vNewReport, "Report", "Tag", szReportName );

   SetAttributeFromAttribute( vNewReport, "Report", "Desc",
                              vPainter, "Palette", "Desc" );

   SetAttributeFromString( vPainter, "Palette", "OpenedFile",
                           szFileName );

   // Drop the previous instance being worked on
   if ( GetViewByName( &vPrevReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) > 0 )
      DropMetaOI( vSubtask, vPrevReport );

   // set source name equal to report name
   SetAttributeFromAttribute( vNewReport, "Report", "SironName",
                              vNewReport, "Report", "Tag");

   // Name the view for mapping
   SetNameForView( vNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewReport, "TZRPSRCO", vSubtask, zLEVEL_TASK );

   SetNameForView( vNewReport, "PAGEL", vSubtask, zLEVEL_TASK );

   // Call NewPage to do the rest
   TZRPUPSD_NewPage( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_DeletePage( zVIEW vSubtask )
{
   zVIEW  vReport;
   zVIEW  vReport2;
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

   if ( MessagePrompt( vSubtask, "RP00007",
                       "Report Maintenance",
                       szMessage,
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_NO,  0 )   == zRESPONSE_NO )
   {
      return( -1 );
   }

   // Create a view to the page in the event the painter drops
   // the current view we are using
   CreateViewFromViewForTask( &vReport2, vReport, 0 );
   DeleteEntity( vReport2, "Page", zREPOS_NEXT );
   SetNameForView( vReport2, "TZWINDOW", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_DeletePageFromList( zVIEW vSubtask )
{
   zVIEW vReport;
   zSHORT nRC;
   nRC = GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   /* If the report doesn't exist, or a page doesn't exist then
      it can't be deleted, so return. */
   if ( nRC < 1 || CheckExistenceOfEntity( vReport, "Page" ) != 0 )
      return( 0 );

   GetViewByName( &vReport, "TZPNMWIN", vSubtask, zLEVEL_TASK );

   if ( TZRPUPSD_UpdatePageFromList( vSubtask ) == 0 )
   {
      nRC = TZRPUPSD_DeletePage( vSubtask );
      RefreshWindow( vReport );
      return( nRC );
   }
   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_RetrieveMetaList( zVIEW vSubtask )
{
   zVIEW vLPLR;
   zVIEW vTZRPSRCO;

   if ( GetViewByName( &vLPLR, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR, zREFER_REPORT_META );
      SetNameForView( vLPLR, "CM_List", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZRPSRCO, "Report" ) >= zCURSOR_SET )
   {
      // if Report open, then set cursor of it
      SetCursorFirstEntityByAttr( vLPLR, "W_MetaDef", "Name",
                                  vTZRPSRCO, "Report", "Tag", "" );
   }
   else
      SetCursorFirstEntity( vLPLR, "W_MetaDef", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW vLPLR;
   zSHORT nRC;
   zVIEW  vSaveAs;

   // Ask and do save
   nRC = TZRPUPSD_AskForSave( vSubtask);
   if ( nRC  != 0 )
   {
      if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
      {
         if ( nRC == 99 )
            // if Report not checked out and user change this Report, then Zeidon
            // call the window "Save Report as"
            // We save the action after Save as in the View TZSAVEAS (for example:
            // Create new Report or Open an other Report or Switch Project or
            // Exit Report Tool)
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zSwitchLPLRAfterSaveAs );
         else
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zNoActionAfterSaveAs );

         return( 0 );
      }
   }

   // Send in flag to force reload of task view for LPLR.
   if ( GetViewByName( &vLPLR, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vLPLR );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SystemMinimize( zVIEW vSubtask )
{
   zVIEW vReport;
   zVIEW vIdleView;

   TraceLineS("(tzpntrad) - SystemMinimize","");

   if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
   {
      if ( GetViewByName( &vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) < 1 )
      {
         SetWindowCaptionTitle( vSubtask, "Painter", "[Idle]" );
         SetNameForView( vSubtask, "__SysIdlePainter", 0, zLEVEL_SYSTEM );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SystemActivate( zVIEW vSubtask )
{
   zVIEW    vReport;
   zVIEW    vIdleView;

   if ( GetViewByName( &vIdleView, "__SysIdlePainter", vSubtask, zLEVEL_SYSTEM ) > 0 &&
        vIdleView == vSubtask )
   {
      DropNameForView( vIdleView, "__SysIdlePainter", 0, zLEVEL_SYSTEM );
      if ( GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK ) < 1 )
      {
         // Transfer to the open report window
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                                       zWAB_ProcessImmediateAction,
                                  "TZRPUPDD", "ReportList" );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SystemRestore( zVIEW vSubtask )
{
// TraceLineS("(tzpntrad) - SystemRestore","");

// fnPainterCall( zMSG_SYSTEMRESTORE, vSubtask, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_EnablePainterWindows( zVIEW vSubtask )
{
   zVIEW   vMetaList = NULL;
   zVIEW   vTZPESRCO;
   zVIEW   vPAGEL;
   zSHORT  nRC;

   // Activate the presentation environment, if the presentation
   // environment is no longer with us
   if ( GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vMetaList, zREFER_PENV_META );

      if ( vMetaList )
      {
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
   }

   // Set window Title with check out state
   GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK );
   SetTitleWithCheckOutState( vSubtask, "Siron Reports Maintenance", "TZRPSRCO",
                              vPAGEL, "Report", zSOURCE_REPORT_META );

#ifndef __WIN32__
   zVIEW   vPageList;
   if ( GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK ) < 1 &&
        GetSubtaskForWindowName( vSubtask, &vPageList, "PageList" ) >= 0 )
   {
      ActivateWindow( vPageList );
      return( 0 );
   }
#endif

   return( 0 );
}

static void
fnSetMenuState( zVIEW vSubtask )
{
   zVIEW   vReport;
   zVIEW   vTaskLPLR;
   zULONG  ulQueryExists;
   zPCHAR  pszLOD;
   zPCHAR  pszExecDir;
   zPCHAR  pszReportName;
   zCHAR   szSironQueryPath[ zMAX_FILESPEC_LTH + 1 ];
   // we cannot use Siron, if we are not connected
   // then we let all Siron menu items disabled
   if (! g_bSironConnected)
     return;

   // get the report and the task
   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_ANY );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vReport, "LOD" ) >= 0 &&
        GetAddrForAttribute( &pszLOD, vReport, "LOD", "Name") == 0)
   {
     SetOptionState( vSubtask, "StartSironRDT",
                     zOPTION_STATUS_ENABLED, TRUE );


     // check the source file for the Siron query
     GetAddrForAttribute( &pszExecDir,   vTaskLPLR, "LPLR", "ExecDir" );
     GetAddrForAttribute( &pszReportName, vReport, "Report", "SironName" );
     sprintf( szSironQueryPath, "%s\\%s.XSQ", pszExecDir, pszReportName );
     ulQueryExists = (zaccess( szSironQueryPath, 0 ) == 0);
   }
   else
   {
     SetOptionState( vSubtask, "StartSironRDT",
                     zOPTION_STATUS_ENABLED, FALSE );
     ulQueryExists = FALSE;
   }

   // enable menue entries, if report source exists
   SetOptionState( vSubtask, "StartSironTest",
                   zOPTION_STATUS_ENABLED, ulQueryExists );
   SetOptionState( vSubtask, "StartSironQuery",
                   zOPTION_STATUS_ENABLED, ulQueryExists );
}


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_ShowLOD_List( zVIEW vSubtask )
{
   zVIEW   vLODList;
   zVIEW   vReport;
   zLONG   lZKey;
   zSHORT  nRC;

   // Activate the LOD list
   nRC = GetViewByName
        ( &vLODList, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vLODList, zREFER_VOR_META );
      SetNameForView( vLODList, "LPLR_VOR_Meta", vSubtask, zLEVEL_TASK );
      OrderEntityForView( vLODList, "W_MetaDef", "Name A" );
   }

   // set cursor pos to old selected view (if exists)
   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vReport, "ViewObjRef" )
                                                     == zCURSOR_SET )
   {
     GetIntegerFromAttribute( &lZKey, vReport, "ViewObjRef", "ZKey");
     nRC = SetCursorFirstEntityByInteger( vLODList, "W_MetaDef",
                                          "CPLR_ZKey", lZKey, "" );
   }

   return( 0 );
} // TZRPUPSD_ShowLOD_List


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SelectLOD( zVIEW vSubtask )
{
   zVIEW   vLODList;
   zVIEW   vTZWINDOW;
   zVIEW   vTZPNMWIN;
   zVIEW   vApplVOR;
   zLONG   lZKey;
   zPCHAR  pszLOD;
   zCHAR   szMsg[40];
   zSHORT  nRC;

   TraceLineS("(tzrpupsd) - SelectLOD","");

   // use selected LOD for report
   GetViewByName( &vTZWINDOW, "TZWINDOW", vSubtask, zLEVEL_ANY );
   GetViewByName( &vLODList, "LPLR_VOR_Meta",
                  vSubtask, zLEVEL_ANY );
   if ( CompareAttributeToString( vLODList, "W_MetaDef", "Name", "" ) == 0 )
   {
      MessageSend( vSubtask, "RP00009", "Report Maintenance",
                   "No LOD selected!\n"
                   "Select a LOD for a report.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // set report to new LOD (exact: to new VOR)
   if ( CheckExistenceOfEntity( vTZWINDOW, "ViewObjRef" )
                                                     == zCURSOR_SET )
   {
      ExcludeEntity( vTZWINDOW, "ViewObjRef", zREPOS_FIRST );
   }

   nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLODList, zREFER_VOR_META, 0);
   if ( nRC == 1 )
   {
      GetIntegerFromAttribute( &lZKey, vApplVOR, "ViewObjRef", "ZKey");
      nRC = SetCursorFirstEntityByInteger( vTZWINDOW, "ViewObjRef",
                                            "ZKey", lZKey, "" );
      if ( nRC != zCURSOR_SET )
         IncludeSubobjectFromSubobject( vTZWINDOW, "ViewObjRef",
                                   vApplVOR, "ViewObjRef", zPOS_AFTER );
   }
   DropMetaOI( vSubtask, vApplVOR );

   // delete list view
   TZRPUPSD_CancelLOD_List( vSubtask );

   // show LOD name
   GetViewByName( &vTZPNMWIN, "TZPNMWIN", vSubtask, zLEVEL_TASK );
   if (GetAddrForAttribute( &pszLOD, vTZWINDOW, "LOD", "Name") == 0)
   {
     sprintf( szMsg, "Base LOD: %s", pszLOD);
   }
   else
   {
     sprintf( szMsg, "No LOD" );
   }
   MB_SetMessage( vTZPNMWIN, 2, szMsg );

   return( 0 );
} // TZRPUPSD_SelectLOD


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_CancelLOD_List( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW   vLODList;

   nRC = GetViewByName( &vLODList, "LPLR_VOR_Meta", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vLODList );
   }

   return( 0 );
} // TZRPUPSD_CancelLOD_List

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironRDT( zVIEW vSubtask )
{
   zVIEW      vReport;
   zVIEW      vTaskLPLR;
   zPCHAR     pszReportName;
   zPCHAR     pszLPLRName;
   zPCHAR     pszLODName;
   zPCHAR     pszPgmSrcDir;
   zPCHAR     pszExecDir;
   zCHAR      szSironQueryPath[ zMAX_FILESPEC_LTH + 1 ];
   zBOOL      bLOD_Selected;
   zBOOL      bLPLR_ReadWrite = TRUE;
   zBOOL      bQueryExists;
   zSHORT     nRC;

   TraceLineS("(tzrpupsd) - StartSironRDT","");

   MB_SetMessage( vSubtask, 1, "Start Siron RDT" );

   // Get the view to the Zeidon Report
   if ( GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   // Check, if we have selected a LOD
   bLOD_Selected = FALSE;
   if ( CheckExistenceOfEntity( vReport, "LOD" ) >= 0 )
   {
     if ( GetAddrForAttribute( &pszLODName, vReport, "LOD", "Name" ) >= 0)
     {
       bLOD_Selected = TRUE;
     }
   }
   if (!bLOD_Selected)
   {
      // the user needs to select a LOD first
      MessageSend( vSubtask, "RP00010", "Report Maintenance",
                   "No LOD selected for Report.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // disable the tool window, cause RDT has control now
   MB_SetMessage ( vSubtask, 1, "Start Siron RDT" );
   EnableAllSubwindows( vSubtask, FALSE );

   /*
   // start the RDT
   */

  /* Connect to the message lib */
  g_vSubtask = vSubtask;
  nRC = ZSironConnectRDT( (T_MSG_CALLBACK) TZRPUPSD_TermSironRDT );
  if (nRC < 0)
  {
    MB_SetMessage ( vSubtask, 1, "Can't connect to Siron" );
    EnableAllSubwindows( g_vSubtask, TRUE );
    return( nRC );
  }

  // check for Siron Report
  GetAddrForAttribute( &pszLPLRName, vTaskLPLR, "LPLR", "Name" );
  GetAddrForAttribute( &pszPgmSrcDir, vTaskLPLR, "LPLR", "PgmSrcDir" );
  GetAddrForAttribute( &pszExecDir,   vTaskLPLR, "LPLR", "ExecDir" );
  GetAddrForAttribute( &pszReportName, vReport, "Report", "SironName" );
  sprintf( szSironQueryPath, "%s\\%s.REP", pszPgmSrcDir, pszReportName );
  bQueryExists = (zaccess( szSironQueryPath, 0 ) == 0);

  TraceLineS("(tzrpupsd) -   Report = ", pszReportName);
  TraceLineS("(tzrpupsd) -   LOD = ", pszLODName);
  TraceLineI("(tzrpupsd) -   Exists = ", (zLONG) bQueryExists);

  /* Start the RDT */
  if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      bLPLR_ReadWrite = FALSE;

  nRC = ZSironStartRDT(
          pszReportName,
          pszLPLRName,
          pszLODName,
          (zLONG) bLPLR_ReadWrite,
          bQueryExists ? zSIRON_RDT_REP : zSIRON_RDT_EMPTY,
          NULL,
          pszPgmSrcDir,
          pszExecDir,
          "Anwender",
          "Kennwort" );

   if (nRC < 0)
   {
     ZSironDisconnectRDT( );
     EnableAllSubwindows( g_vSubtask, TRUE );
     TraceLineI( "Can't start RDT, rc = ", (zLONG) nRC);
     MB_SetMessage ( vSubtask, 1, "Start of RDT failed" );
     SysMessageBox( "RDT", "Start of RDT failed", 1);
     return( -1 );
   }

   MB_SetMessage ( vSubtask, 1, "Siron RDT is active" );

  /* Wait, until RDT has terminated */
  nRC = ZSironStartWaitForRDT( );

  return( 0 );

} // TZRPUPSD_StartSironRDT


void OPERATION
TZRPUPSD_TermSironRDT( zLONG lJob, zLONG lCompStatus )
{
   zSHORT nRC;

   /* Disconnect from the message lib */
   nRC = ZSironStopWaitForRDT();
   ZSironDisconnectRDT();
   if (nRC)
     MB_SetMessage ( g_vSubtask, 1, "Report Design Stored" );
   else
     MB_SetMessage ( g_vSubtask, 1, "Report Design Cancelled " );

   // enable the tool window
   EnableAllSubwindows( g_vSubtask, TRUE );
}


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironTest( zVIEW vSubtask )
{
   zSHORT nRC;
   zLONG  lJob;
   zLONG  lFlags;
   zCHAR  szQueryPath[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW  vReport;
   zVIEW  vTaskLPLR;
   zPCHAR pszReportName;
   zPCHAR pszLPLRName;
   zPCHAR pszExecDir;

   MB_SetMessage ( vSubtask, 1, "Start report with test data" );

   // Get the view to the Zeidon Report
   if ( GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   GetAddrForAttribute( &pszReportName, vReport, "Report", "SironName" );
   GetAddrForAttribute( &pszLPLRName, vTaskLPLR, "LPLR", "Name" );
   GetAddrForAttribute( &pszExecDir,   vTaskLPLR, "LPLR", "ExecDir" );

   /* Append LPLR exec path to query name */
   sprintf( szQueryPath, "%s\\%s.XSQ", pszExecDir, pszReportName );

   lFlags = zSIRON_QUERY_TEST;

   /* start it */
   nRC = ZSironRunQuery( szQueryPath,
                         "",
                         "",
                         "Anwender",
                         "Kennwort",
                         &lJob,
                         pszLPLRName,
                         "",
                         lFlags);

   if (nRC != 0)
   {
     TraceLineI( "(ziron) SironRunQuery returned ", nRC);
     MessageSend( vSubtask, "", "ZSiron",
                 "Start of report failed",
                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
     return( -1 );
   }

   g_vSubtask = vSubtask;

   return( 0 );
} // TZRPUPSD_StartSironTest

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_StartSironQuery( zVIEW vSubtask )
{
   zSHORT nRC;
   zLONG  lJob;
   zLONG  lFlags;
   zCHAR  szQueryPath[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW  vReport;
   zVIEW  vTaskLPLR;
   zPCHAR pszReportName;
   zPCHAR pszLPLRName;
   zPCHAR pszExecDir;

   MB_SetMessage ( vSubtask, 1, "Start report" );

   // Get the view to the Zeidon Report
   if ( GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 1 )
      return( -1 );

   GetAddrForAttribute( &pszReportName, vReport, "Report", "SironName" );
   GetAddrForAttribute( &pszLPLRName, vTaskLPLR, "LPLR", "Name" );
   GetAddrForAttribute( &pszExecDir,   vTaskLPLR, "LPLR", "ExecDir" );

   /* Append LPLR exec path to query name */
   sprintf( szQueryPath, "%s\\%s.XSQ", pszExecDir, pszReportName );

   lFlags = zSIRON_QUERY_DATA;

   /* start it */
   nRC = ZSironRunQuery( szQueryPath,
                         "",
                         "",
                         "Anwender",
                         "Kennwort",
                         &lJob,
                         pszLPLRName,
                         "",
                         lFlags);

   if (nRC != 0)
   {
     TraceLineI( "(ziron) SironRunQuery returned ", nRC);
     MessageSend( vSubtask, "", "ZSiron",
                  "Start of report failed",
                  zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
     return( -1 );
   }

   g_vSubtask = vSubtask;

   return( 0 );
} // TZRPUPSD_StartSironQuery

void OPERATION
TZRPUPSD_TermSironQuery( zLONG lJobID, zLONG lCompStatus)
{
   zCHAR szMsg[40];

   switch (lCompStatus)
   {
     case zSIRON_STATUS_READY:
       strcpy( szMsg, "Report is done");
       break;

     case zSIRON_STATUS_ERROR:
       strcpy( szMsg, "Error while running report");
       break;

     case zSIRON_STATUS_WAITING:
       strcpy( szMsg, "Report is queued");
       break;

     default:
       strcpy( szMsg, "");
   }

   MB_SetMessage ( g_vSubtask, 1, szMsg );

//   ZSironDisconnect( );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_HelpAbout( zVIEW vSubtask )
{
   IssueToolAbout( vSubtask, zTOOL_REPORT, __FILE__, __DATE__, __TIME__ );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SaveAsPostBuild
//
zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveAsPostBuild( zVIEW vSubtask )
{
   zVIEW    vTZRPSRCO;
   zVIEW    vCM_List;
   zVIEW    vSaveAs;

   GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_REPORT_META );
      SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vCM_List, "W_MetaDef", 0 );

   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );
   SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SironName", "" );

   //set current Description
   SetCtrlState( vSubtask, "cbKeepCurrentDesc", zCONTROL_STATUS_CHECKED, 1 );
   SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                              vTZRPSRCO, "Report", "Desc" );

   return( 0 );
} // TZRPUPSD_SaveAsPostBuild

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SetSaveAsName
//
zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SetSaveAsName( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vTZRPSRCO;
   zVIEW  vSaveAs;
   zVIEW  vReportData;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );


   SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "SaveAsName8",
                              vCM_List, "W_MetaDef", "Name" );

   // set Description
   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) != 1 )
   {
      SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vCM_List, "W_MetaDef", "Desc" );
   }

   // set Source Name
   if ( CompareAttributeToAttribute( vCM_List, "W_MetaDef", "Name",
                                     vTZRPSRCO, "Report", "Tag" ) == 0 )
   {
      CreateViewFromViewForTask( &vReportData, vTZRPSRCO, 0 );
   }
   else
   {
      if ( ActivateMetaOI( vSubtask, &vReportData, vCM_List,
                           zSOURCE_REPORT_META, zSINGLE | zLEVEL_TASK ) != 1 )
      {
         return( -1 );
      }
   }

   SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "SironName",
                              vReportData, "Report", "SironName" );

   DropMetaOI( vSubtask, vReportData );
   SetFocusToCtrl( vSubtask, "edReportName" );

   return( 0 );
} // TZRPUPSD_SetSaveAsName


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZRPUPSD_SaveAsCheckStatus
//
static zSHORT
fnTZRPUPSD_SaveAsCheckStatus( zVIEW    vSubtask,
                              zVIEW    vCM_List,
                              zPCHAR   szOutName )
{
   zCHAR  szMsg[155];

   //check the check out state
   if ( CompareAttributeToInteger( vCM_List, "W_MetaDef",
                                   "Status", 1 ) != 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Report '" );
      strcat_s( szMsg, zsizeof( szMsg ), szOutName );
      strcat_s( szMsg, zsizeof( szMsg ), "' is not checked out." );
      MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                   szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edReportName" );
      return( -1 );
   }

   return( 0 );
} // fnTZRPUPSD_SaveAsCheckStatus


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZRPUPSD_SaveAsCheckName
//
static zSHORT
fnTZRPUPSD_SaveAsCheckName( zVIEW  vSubtask,
                            zVIEW  vSaveAs,
                            zPCHAR szOutName )
{
   zCHAR  szNewName[33];
   zCHAR  szSourceName[33];

   // Report Name is required
   if ( zstrcmp( szOutName, "" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00137", "Report Maintenance",
                   "Report Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edReportName" );
      return( -2 );
   }

   // Source Name is required
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SironName" );
   UfCompressName( szNewName, szSourceName, 8, "", "", "", "", 0 );
   if ( zstrcmp( szSourceName, "" ) == 0 )
   {
      MessageSend( vSubtask, "ZO00137", "Dialog Maintenance",
                   "Source Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edSourceName" );
      return( -2 );
   }

   return( 0 );
} // fnTZRPUPSD_SaveAsCheckName


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZRPUPSD_SaveAsCopySironFiles
//
static zSHORT
zwTZRPUPSD_SaveAsCopySironFiles( zVIEW  vTZRPSRCO,
                                 zVIEW  vSaveAs )
{
   zCHAR    szOld_File[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szNew_File[ zMAX_FILESPEC_LTH + 1 ];

   // Copy REP file.
   fnTZRPUPSD_SaveAsGetFileName( vTZRPSRCO, "Report", "SironName",
                                 "REP", szOld_File );
   fnTZRPUPSD_SaveAsGetFileName( vSaveAs, "ActionAfterSaveAS", "SironName",
                                 "REP", szNew_File );
   SysCopyFile( szOld_File, szNew_File, TRUE );

   // Copy XSQ file.
   fnTZRPUPSD_SaveAsGetFileName( vTZRPSRCO, "Report", "SironName",
                                 "XSQ", szOld_File );
   fnTZRPUPSD_SaveAsGetFileName( vSaveAs, "ActionAfterSaveAS", "SironName",
                                 "XSQ", szNew_File );
   SysCopyFile( szOld_File, szNew_File, TRUE );

   return( 0 );

} // zwTZRPUPSD_SaveAsCopySironFiles


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZRPUPSD_SaveAsGetFileName
//
//    PARAMETER:
//    szSourceFileName : Size fo szSourceFileName has to be at least  zMAX_FILESPEC_LTH + 1
//
static zSHORT
fnTZRPUPSD_SaveAsGetFileName( zVIEW  vView,
                              zPCHAR szEntity,
                              zPCHAR szAttribute,
                              zPCHAR szExtension,
                              zPCHAR szSourceFileName )
{
   zVIEW    vTaskLPLR;
   zCHAR    szNewName[33];
   zCHAR    szFileName[33];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vView, szEntity, szAttribute );
   UfCompressName( szNewName, szFileName, 8, "", "", "", "", 0 );

   if (szExtension == "REP")
      GetStringFromAttribute( szSourceFileName, zsizeof( szSourceFileName ), vTaskLPLR, "LPLR", "PgmSrcDir" );
   else
      GetStringFromAttribute( szSourceFileName, zsizeof( szSourceFileName ), vTaskLPLR, "LPLR", "ExecDir" );
   ofnTZCMWKSO_AppendSlash( szSourceFileName );

   strcat_s( szSourceFileName, zsizeof( szSourceFileName ) szFileName );
   strcat_s( szSourceFileName, zsizeof( szSourceFileName ) "." );
   strcat_s( szSourceFileName, zsizeof( szSourceFileName ) szExtension );

   return( 0 );
} // fnTZRPUPSD_SaveAsGetFileName


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZRPUPSD_SaveAsCheckSironFiles
//
static zSHORT
fnTZRPUPSD_SaveAsCheckSironFiles( zVIEW    vSubtask,
                                  zVIEW    vTZRPSRCO,
                                  zVIEW    vSaveAs )
{
   zLONG    hFile;
   zCHAR    szMsg[ zMAX_MESSAGE_LTH + 1 ];
   zCHAR    szSironFile[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC;
   zSHORT   nReplace     = 1;
   zSHORT   nFilesExists = 0;
   zBOOL    bReadOnly = FALSE;

   // If this Report Definition is for a Siron Report, copy the related files
   // which have extensions .REP and .XSQ.
   if ( CompareAttributeToString( vTZRPSRCO, "Report", "SironName", "" ) == 0 )
      nReplace = 0;
   else
   {
      fnTZRPUPSD_SaveAsGetFileName( vSaveAs, "ActionAfterSaveAS", "SironName",
                                    "REP", szSironFile );
      hFile = SysOpenFile( szSironFile, COREFILE_EXIST );
      if ( hFile == 1 )
      {
         nFilesExists = 1;
      }
      else
      {
         fnTZRPUPSD_SaveAsGetFileName( vSaveAs, "ActionAfterSaveAS", "SironName",
                                       "XSQ", szSironFile );
         hFile = SysOpenFile( szSironFile, COREFILE_EXIST );
         if ( hFile == 1 )
         {
            nFilesExists = 1;
         }
      }

      if ( nFilesExists == 1 )
      {
         strcpy_s( szMsg, zsizeof( szMsg ), "RDT Source Files '" );
         strcat_s( szMsg, zsizeof( szMsg ), szSironFile );
         strcat_s( szMsg, zsizeof( szMsg ), "' already exists. Replace existing File?" );
         nRC = MessagePrompt( vSubtask, "ZO00138", "Dialog Maintenance",
                              szMsg, zBEEP, zBUTTONS_YESNOCANCEL,
                              zRESPONSE_YES, zICON_QUESTION );
         if ( nRC == zRESPONSE_CANCEL )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            SetFocusToCtrl( vSubtask, "edSourceName" );
            return( -1 );
         }
         else
         {
            if ( nRC == zRESPONSE_NO )
               nReplace = 0;
         }
      }
   } // endif ( CompareAttributeToString( vTZRPSRCO, "Report", "SironName", "" ) == 0 )

   if ( nReplace == 1 )
      zwTZRPUPSD_SaveAsCopySironFiles( vTZRPSRCO, vSaveAs );

   if ( MiGetUpdateForView( vTZRPSRCO ) == 0 )
   {
      bReadOnly = TRUE;
      SetViewUpdate( vTZRPSRCO );
   }

   // Set new Description and Source Name
   SetAttributeFromAttribute( vTZRPSRCO, "Report", "Desc",
                              vSaveAs, "ActionAfterSaveAS", "Desc" );
   SetAttributeFromAttribute( vTZRPSRCO, "Report", "SironName",
                              vSaveAs, "ActionAfterSaveAS", "SironName" );

   if ( bReadOnly == TRUE )
      SetViewReadOnly( vTZRPSRCO );

   return( 0 );

} // fnTZRPUPSD_SaveAsCheckSironFiles


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SaveAsReport
//
// save Report under an other name
zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveAsReport( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vTZRPSRCO;
   zVIEW  vTZRPSRCO_New;
   zVIEW  vCM_List;
   zVIEW  vSaveAs;
   zCHAR  szNewName[ 33 ];
   zCHAR  szOutName[ 33 ];
   zVIEW  vTaskLPLR;
   zVIEW  vSourceLPLR;

   zCHAR  szPathNameREP[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szPathNameXSQ[ zMAX_FILESPEC_LTH + 1 ];

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   // Validate Report Name is OK
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   // check Save as Name
   if ( fnTZRPUPSD_SaveAsCheckName( vSubtask, vSaveAs, szOutName ) < 0 )
      return( -1 );

   // Report Name already exists
   if ( SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szOutName, 0 ) > zCURSOR_UNCHANGED )
   {
      nRC = MessagePrompt( vSubtask, "ZO00138", "Report Maintenance",
                           "Report already exists. Replace existing Report?",
                           zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edReportName" );
         return( 0 );
      }

      //check the check out state
      if ( fnTZRPUPSD_SaveAsCheckStatus( vSubtask, vCM_List, szOutName ) < 0 )
         return( -1 );

      //if this Report open, then save Report, not save as
      if ( CompareAttributeToString( vTZRPSRCO, "Report", "Tag", szOutName ) == 0 )
      {
         if ( fnTZRPUPSD_SaveAsCheckSironFiles( vSubtask, vTZRPSRCO, vSaveAs ) < 0 )
            return( -1 );

         SetAttributeFromString( vTZRPSRCO, "Report", "Tag", szOutName );
         TZRPUPSD_SaveReportFile( vSubtask );
         return( 0 );
      }
      else
      {
         // if another Report open, then replace existing Report
         DeleteMetaOI( vSubtask, vCM_List, zREFER_REPORT_META );
      }
   }

   if ( fnTZRPUPSD_SaveAsCheckSironFiles( vSubtask, vTZRPSRCO, vSaveAs ) < 0 )
      return( -1 );

   TZRPUPSD_SaveNewReport( vSubtask, &vTZRPSRCO_New, szOutName, vTZRPSRCO, vTaskLPLR );

   // We have to change the filename in file *.REP if it exists
   // First get the pathname from the LPLR
   // Validate Report Name is OK
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SironName" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   CreateViewFromViewForTask( &vSourceLPLR, vTaskLPLR, 0 );
   memset (szPathNameREP, 0,  zMAX_FILESPEC_LTH+1);
   memset (szPathNameXSQ, 0,  zMAX_FILESPEC_LTH+1);
   GetStringFromAttribute( szPathNameREP, zsizeof( szPathNameREP ), vSourceLPLR, "LPLR", "PgmSrcDir" );
   GetStringFromAttribute( szPathNameXSQ, zsizeof( szPathNameXSQ ), vSourceLPLR, "LPLR", "ExecDir" );
   oTZRPSRCO_ChangeRepFileContents( vSubtask, vSourceLPLR, szPathNameREP, szPathNameXSQ,
                                    szOutName, "REP", zCHANGE_REP_SAVEAS );
   DropView (vSourceLPLR);

   zwfnTZRPUPSD_CheckActionAfterSaveAs( vSubtask );

   return( 0 );
} // TZRPUPSD_SaveAsReport

#endif // #ifndef QUINSOFT


zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SortByDate( zVIEW vSubtask )
{
   zVIEW  vMetaList;
   zSHORT nRC;

   nRC = GetViewByName( &vMetaList, "CM_List", vSubtask, zLEVEL_TASK );
   nRC = zgSortEntityWithinParent( zDESCENDING, vMetaList,
                                   "W_MetaDef", "LastUpdateDate", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TZRPUPSD_SaveNewReport( zVIEW     vSubtask,
                        zPVIEW    pvNewReport,
                        zPCHAR    szNewReportName,
                        zVIEW     vOrigReport,
                        zVIEW     vSourceLPLR )
{
   zVIEW  vCM_List;
   zVIEW  vSourceLPLR2;

   GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vSourceLPLR2, vSourceLPLR, 0 );

   // Call operation that will actually do the work of creating the report.
   oTZRPSRCO_ReportCopy( vSubtask, pvNewReport, vOrigReport, vSourceLPLR2 );

   SetAttributeFromString( *pvNewReport, "Report", "Tag", szNewReportName );
   SetAttributeFromString( *pvNewReport, "Page", "Tag", szNewReportName );

   // Clean up views.
   DropView( vSourceLPLR2 );

   // Finally, commit the Report Def.
   CommitMetaOI( vSubtask, *pvNewReport, zSOURCE_REPORT_META );

   DropMetaOI( vSubtask, vOrigReport );
   SetNameForView( *pvNewReport, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "PAGEL", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   SetNameForView( *pvNewReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   vOrigReport = *pvNewReport;

   SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "CPLR_ZKey",
                               vOrigReport, "Report", "ZKey", "" );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_Check_CheckOutStatus
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_Check_CheckOutStatus( zVIEW vSubtask )
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
} // TZRPUPSD_Check_CheckOutStatus


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_InitMenu
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_InitMenu( zVIEW vSubtask )
{
   zVIEW    vTZRPSRCO;
   zVIEW    vTaskLPLR;
   zSHORT   nRC;
   zSHORT   nReportIsCheckedOut;
   zSHORT   nEnable = TRUE;

   nRC = GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );

   nReportIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZRPSRCO,
                                                zSOURCE_REPORT_META );
   if ( nRC != zLEVEL_TASK || !MiGetUpdateForView( vTZRPSRCO ) ||
        !nReportIsCheckedOut )
   {
      nReportIsCheckedOut = 0;
      TraceLineS( "(DMUP) ", "view is read only" );
   }
   else
      nReportIsCheckedOut = 1;

   // if Report not checked out, disable menu item for save Report
   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, nReportIsCheckedOut );

   // if a error situation disable menue item new and open
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      nEnable = FALSE;

   SetOptionState( vSubtask, "NewFile", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "Open", zOPTION_STATUS_ENABLED, nEnable );

   return( 0 );
} // TZRPUPSD_InitMenu


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_CheckNameForCheckOut
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_CheckNameForCheckOut( zVIEW vSubtask )
{
   zSHORT  nEnable = 1;
   zVIEW   vCM_List;
   zVIEW   vCM_List_Copy;
   zCHAR   szNewName[33];
   zCHAR   szOutName[33];

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
} // TZRPUPSD_CheckNameForCheckOut


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SaveAsCancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_SaveAsCancel( zVIEW vSubtask )
{
   zVIEW   vSaveAs;

   if ( GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK ) > 0 )
   {
      SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                               "Typ", zNoActionAfterSaveAs );
      SetAttributeFromString( vSaveAs, "ActionAfterSaveAS", "SaveAsName8", "" );
   }

   return( 0 );
} // TZRPUPSD_SaveAsCancel


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_NewReport
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_NewReport( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = TZRPUPSD_AskForSave( vSubtask );
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
   }

   // In the original Report Tool, the painter call creates a work object.
   // We do not use the painter call. We create the work object ourself.
   TZRPUPSD_CreateTZPNTROO( vSubtask );

   return( 0 );
} // TZRPUPSD_NewReport


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_OpenReport
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_OpenReport( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zSHORT nRC;

   // Ask and do save
   nRC = TZRPUPSD_AskForSave( vSubtask );
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
                                     "Typ", zOpenComponentAfterSaveAs );
         }
         else
         {
            SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                                     "Typ", zNoActionAfterSaveAs );
         }
      }
   }

   // In the original Report Tool, the painter call creates a work object.
   // We do not use the painter call. We create the work object ourself.
   TZRPUPSD_CreateTZPNTROO( vSubtask );

   return( 0 );
} // TZRPUPSD_OpenReport


static zSHORT
zwfnTZRPUPSD_CheckActionAfterSaveAs( zVIEW  vSubtask )
{
   zVIEW   vSaveAs;
   zVIEW   vTZRPSRCO;
   zVIEW   vParentWindow;
   zLONG   lActionAfterSaveAs;

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );

   // Set window Title with check out state
   GetParentWindow( &vParentWindow, vSubtask );
   SetTitleWithCheckOutState( vParentWindow, "Siron Reports Maintenance", "TZRPSRCO",
                              vTZRPSRCO, "Report", zSOURCE_REPORT_META );

   // if Report not checked out and user change this Report, then zeidon
   // call the window "Save Report as"
   // We save the action after Save as in the View TZSAVEAS (for example:
   // Create new Report or Open an other Report or Switch Project or Exit
   // Report Tool)

   GetIntegerFromAttribute( &nActionAfterSaveAs, vSaveAs,
                            "ActionAfterSaveAS", "Typ" );
   SetAttributeFromInteger( vSaveAs, "ActionAfterSaveAS",
                            "Typ", zNoActionAfterSaveAs );

   switch ( nActionAfterSaveAs )
   {
      case zNewComponentAfterSaveAs:   // Create new Report
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZRPUPDD", "ReportNew" );
         break;

      case zOpenComponentAfterSaveAs:   // Open Report
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZRPUPDD", "ReportList" );
         break;

      case zSwitchLPLRAfterSaveAs:   // Switch Project
         TZRPUPSD_SwitchLPLR( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZCMSLPD", "SwitchLPLR" );
         break;

      case zExitToolAfterSaveAs:   // Exit Report Tool
         TZRPUPSD_ExitReport( vSubtask );
         SetWindowActionBehavior( vSubtask, zWAB_ExitDialogTask, "", "" );
         break;
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SaveAsKeepCurrentDesc
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_SaveAsKeepCurrentDesc( zVIEW vSubtask )
{
   zVIEW  vTZRPSRCO;
   zVIEW  vSaveAs;

   GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "cbKeepCurrentDesc",
                      zCONTROL_STATUS_CHECKED ) == 1 )
   {
      SetAttributeFromAttribute(   vSaveAs, "ActionAfterSaveAS", "Desc",
                                 vTZRPSRCO, "Report", "Desc" );
      RefreshCtrl( vSubtask, "edDesc" );
   }

   return( 0 );
} // TZRPUPSD_SaveAsKeepCurrentDesc



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SaveAsSetDefaults
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_SaveAsSetDefaults( zVIEW vSubtask )
{
   zVIEW  vSaveAs;
   zCHAR  szNewName[33];
   zCHAR  szOutName[33];

   GetViewByName( &vSaveAs, "TZSAVEAS", vSubtask, zLEVEL_TASK );

   // if Dialog Name not required, set default value
   GetStringFromAttribute( szNewName, zsizeof( szNewName ), vSaveAs, "ActionAfterSaveAS", "SaveAsName8" );
   UfCompressName( szNewName, szOutName, 8, "", "", "", "", 0 );

   //Name is required
   if ( zstrcmp( szOutName, "" ) != 0 )
   {
      SetAttributeFromString(   vSaveAs, "ActionAfterSaveAS", "SironName", szOutName );
   }

   RefreshCtrl( vSubtask, "edSourceName" );

   return( 0 );
} // TZRPUPSD_SaveAsSetDefaults


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: TZRPUPSD_SetCheckOutState
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
TZRPUPSD_SetCheckOutState( zVIEW vSubtask )
{
   zVIEW  vTZRPSRCO;
   zVIEW  vLPLR_VOR_Meta;
   zSHORT nEnable;

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZRPSRCO, "TZRPSRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( !ComponentIsCheckedOut( vSubtask, vTZRPSRCO, zSOURCE_REPORT_META ))
         nEnable = 0;
      else
      {
         if ( CheckExistenceOfEntity( vLPLR_VOR_Meta, "W_MetaDef" ) >= zCURSOR_SET )
            nEnable = 1;
         else
            nEnable = 0;
      }
   }

   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, nEnable );
   EnableAction( vSubtask, "Select", nEnable );

   return( 0 );
} // TZRPUPSD_SetCheckOutState
