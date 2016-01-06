
/*
CHANGE LOG
2002.03.12 FH
   Avoid "ViewIsReadOnly" if a meta is not checked out.
2001.05.01  DKS
   MigrateReports enhanced to permit save of all PRPs.
2001.02.15    BL    R54697
   Improve handling of return code from InitializeLPLR.
2000.12.26  DC  Z2000
   Modified most of the calls to ComponentIsCheckedOut so that they use
   a view to the root and not to vControl. The vControl view is often to
   a subobject which causes an error message to be returned during the call.
2000.11.10  SR  Z2000 Size of path variables
   Modified size of size of szFileSpec1 and szFileSpec2 of function
   MigrateReports because size of attribuFes ExecDir, MetaSrcDir and
   PgmSrcDir has been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.17  DKS   Z2000
   All returns from control maintenance dialogs now refresh the window.
2000.10.05  BL    Z10
   Completely maintained Control List and Report Controls
   If Zeidon Report not checked out disable all function for update
2000.09.23  BL    Z10
   Bugfix repaint Report Controls
   Modified AutoDesignReport: if AutoDesignReport and Page not open,
   load Page
2000.07.18  BL    Z10 Repository
   Added ShowCtrlList_SetState, ShowCtrlListForCurrentEntity,
   InitMenuDisableAll and
   modified Operations:
   if Zeidon Report not checked out disable all function for update
2000.07.03  BL
   Added ReturnFromSubwindow
2000.01.13  BL
   Modified AutoDesignReport for new PPE handling.
   Insert casting for remove warnings.
*/

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

#pragma pack( 1 )
#include <tzwdlgso.hst>
#pragma pack( )

zOPER_EXPORT zSHORT OPERATION
RepaintZeidonControl( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
fnContextMappingInit( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
PreBuildUserSpec( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
AutoDesignReport( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
TZRPUPDD_UpdatePageFromList( zVIEW vSubtask );

static zSHORT
InitMenuDisableAll( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GROUPSET_CheckValues( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UPD_GROUP_PostBuild( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UPD_GROUP_OK( zVIEW vSubtask );

void
fnCreateReportMappingEntities( zVIEW vSubtask, zVIEW vDialog );

zLONG
fnGetSize( zPCHAR pchSize, zLONG lFactor )
{
   zPCHAR pch = zstrchr( pchSize, '.' );
   zLONG  lSize;

   if ( pch && *(pch + 1) )
   {
      zSHORT nCnt = 0;
      zSHORT nDivisor = 1;

      *pch = 0;
      pch++;
      lSize = zatol( pchSize ) * lFactor;
      while ( pch[ nCnt ] )
      {
         nCnt++;
         nDivisor *= 10;
      }

      lSize += (zatol( pch ) * lFactor) / nDivisor;
   }
   else
      lSize = zatol( pchSize ) * lFactor;

   return( lSize );
}

void
fnSetSize( zPCHAR pchSize, zLONG lSize, zLONG lFactor )
{
   zLONG lUnit = (lSize % lFactor) * 1000;

   lUnit /= lFactor;
   while ( lUnit && lUnit % 10 == 0 )
      lUnit /= 10;

   lSize /= lFactor;

   if ( lUnit )
      zsprintf( pchSize, "%d.%d", lSize, lUnit );
   else
      zsprintf( pchSize, "%d", lSize );
}

zOPER_EXPORT zSHORT OPERATION
LandScapeChanged( zVIEW vSubtask )
{
   zVIEW vReport;
   zCHAR  szSize[ 32 ];
   zLONG lSize;

   GetViewByName( &vReport, "PAGEL", vSubtask, zLEVEL_TASK );
   if ( GetCtrlState( vSubtask, "LandScape", zCONTROL_STATUS_CHECKED ) )
   {
      GetIntegerFromAttribute( &lSize, vReport, "Page", "SizeY" );
      fnSetSize( szSize, lSize, 256 );
      SetCtrlText( vSubtask, "Width", szSize );

      GetIntegerFromAttribute( &lSize, vReport, "Page", "SizeX" );
      fnSetSize( szSize, lSize, 256 );
      SetCtrlText( vSubtask, "Height", szSize );
   }
   else
   {
      GetIntegerFromAttribute( &lSize, vReport, "Page", "SizeX" );
      fnSetSize( szSize, lSize, 256 );
      SetCtrlText( vSubtask, "Width", szSize );

      GetIntegerFromAttribute( &lSize, vReport, "Page", "SizeY" );
      fnSetSize( szSize, lSize, 256 );
      SetCtrlText( vSubtask, "Height", szSize );
   }

   return( 0 );
}

//./ ADD NAME=ReportDetailPostbuild
// Source Module=tzrpupdd.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      ReportDetailPostBuild
//
// PURPOSE:    To set up the report text size
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ReportDetailPostBuild( zVIEW vSubtask )
{
   zVIEW    vReport;
   zCHAR    szSize[ 32 ];
   zLONG    lPrintFlags;
   zLONG    lSize;
   zSHORT   nEnable = 1;

   GetViewByName( &vReport, "PAGEL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lPrintFlags, vReport, "Report", "PrintFlags" );
   if ( lPrintFlags & zREPORT_LANDSCAPE )
      SetCtrlState( vSubtask, "LandScape", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lPrintFlags & zREPORT_FIT2PAGE )
      SetCtrlState( vSubtask, "FitToPage", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lPrintFlags & zREPORT_PRINT2HTML )
      SetCtrlState( vSubtask, "HTML", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lPrintFlags & zREPORT_RESETPAGING )
      SetCtrlState( vSubtask, "ResetPageNbr", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lPrintFlags & zREPORT_COUNTPAGES )
      SetCtrlState( vSubtask, "CountPages", zCONTROL_STATUS_CHECKED, TRUE );

   GetIntegerFromAttribute( &lSize, vReport, "Report", "ReportTextSize" );
   fnSetSize( szSize, lSize, 10 );
   SetCtrlText( vSubtask, "ReportTextSize", szSize );

   GetIntegerFromAttribute( &lSize, vReport, "Report", "MarginTop" );
   fnSetSize( szSize, lSize, 256 );
   SetCtrlText( vSubtask, "Top", szSize );

   GetIntegerFromAttribute( &lSize, vReport, "Report", "MarginLeft" );
   fnSetSize( szSize, lSize, 256 );
   SetCtrlText( vSubtask, "Left", szSize );

   GetIntegerFromAttribute( &lSize, vReport, "Report", "MarginBottom" );
   fnSetSize( szSize, lSize, 256 );
   SetCtrlText( vSubtask, "Bottom", szSize );

   GetIntegerFromAttribute( &lSize, vReport, "Report", "MarginRight" );
   fnSetSize( szSize, lSize, 256 );
   SetCtrlText( vSubtask, "Right", szSize );

   LandScapeChanged( vSubtask );

   CB_SetData( vSubtask, "Units", "Inches (256ths)",
               "Inches (256ths);Millimeters (10ths) Currently unsupported", 0 );

   // If Report is not checked out, disable ReportTextSize, LandScape, HTML,
   // and FitToPage.
   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ) )
      nEnable = 0;

   SetCtrlState( vSubtask, "Tag", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "DLL_Name", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Desc", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "ReportTextSize", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "LandScape", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "FitToPage", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "HTML", zCONTROL_STATUS_ENABLED, nEnable );

   return( 0 );
}

//./ ADD NAME=ReportDetailOK
// Source Module=tzrpupdd.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      ReportDetailOK
//
// PURPOSE:    To set up the report text size
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ReportDetailOK( zVIEW vSubtask )
{
   zVIEW vReport;
   zCHAR  szSize[ 32 ];
   zLONG lPrintFlags;
   zLONG lSize;

   GetViewByName( &vReport, "PAGEL", vSubtask, zLEVEL_TASK );

   // if Report not checked out, do not set values
   if ( ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ) )
   {
      GetIntegerFromAttribute( &lPrintFlags, vReport, "Report", "PrintFlags" );
      if ( GetCtrlState( vSubtask, "LandScape", zCONTROL_STATUS_CHECKED ) )
         lPrintFlags |= zREPORT_LANDSCAPE;
      else
         lPrintFlags &= ~zREPORT_LANDSCAPE;

      if ( GetCtrlState( vSubtask, "FitToPage", zCONTROL_STATUS_CHECKED ) )
         lPrintFlags |= zREPORT_FIT2PAGE;
      else
         lPrintFlags &= ~zREPORT_FIT2PAGE;

      if ( GetCtrlState( vSubtask, "HTML", zCONTROL_STATUS_CHECKED ) )
         lPrintFlags |= zREPORT_PRINT2HTML;
      else
         lPrintFlags &= ~zREPORT_PRINT2HTML;

      if ( GetCtrlState( vSubtask, "ResetPageNbr", zCONTROL_STATUS_CHECKED ) )
         lPrintFlags |= zREPORT_RESETPAGING;
      else
         lPrintFlags &= ~zREPORT_RESETPAGING;

      if ( GetCtrlState( vSubtask, "CountPages", zCONTROL_STATUS_CHECKED ) )
         lPrintFlags |= zREPORT_COUNTPAGES;
      else
         lPrintFlags &= ~zREPORT_COUNTPAGES;

      SetAttributeFromInteger( vReport, "Report", "PrintFlags", lPrintFlags );

      GetCtrlText( vSubtask, "ReportTextSize", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 10 );
      SetAttributeFromInteger( vReport, "Report", "ReportTextSize", lSize );

      GetCtrlText( vSubtask, "Height", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Page", "SizeY", lSize );

      GetCtrlText( vSubtask, "Width", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Page", "SizeX", lSize );

      GetCtrlText( vSubtask, "Top", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Report", "MarginTop", lSize );

      GetCtrlText( vSubtask, "Left", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Report", "MarginLeft", lSize );

      GetCtrlText( vSubtask, "Bottom", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Report", "MarginBottom", lSize );

      GetCtrlText( vSubtask, "Right", szSize, sizeof( szSize ) );
      lSize = fnGetSize( szSize, 256 );
      SetAttributeFromInteger( vReport, "Report", "MarginRight", lSize );

      fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowReportControlBar( zVIEW   vSubtask )
{
   zVIEW vReport;

   GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   fnCreateReportMappingEntities( vSubtask, vReport );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
HideReportControlBar( zVIEW   vSubtask )
{
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowHideReportControlBar( zVIEW   vSubtask )
{
   zVIEW vReport;
   zBOOL bVisible;

   GetViewByName( &vReport, "TZOPENRPT", vSubtask, zLEVEL_TASK );
   bVisible = (zBOOL) GetCtrlState( vSubtask, "ControlBar",
                                    zCONTROL_STATUS_VISIBLE );
   SetCtrlState( vSubtask, "ControlBar", zCONTROL_STATUS_VISIBLE, !bVisible );
   SetOptionState( vSubtask, "ShowHideControlBar", zOPTION_STATUS_TEXT,
                   (zULONG) (bVisible ? "Show Control Bar" : "Hide Control Bar") );
   fnCreateReportMappingEntities( vSubtask, vReport );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShowReportMappingPopup( zVIEW   vSubtask )
{
   zCHAR szEntityName[ 33 ];

   OL_GetCurrentEntityName( vSubtask, "View", 0, szEntityName );
   TraceLineS( "EntityName: ", szEntityName );
   if ( zstrcmp( szEntityName, "DrivingViewObjRef" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "DrivingViewObjRef", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "FullReportEntity" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "FullReportEntity", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "FullReportEntityChild" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "FullReportEntityChild", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "PartialReportEntity" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "PartialReportEntity", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "PartialReportEntityChild" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "PartialReportEntityChild", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "Page" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "Page", -1, 0, FALSE, FALSE );

   if ( zstrcmp( szEntityName, "GroupSet" ) == 0 )
      CreateTrackingPopupMenu( vSubtask, "GroupSet", -1, 0, FALSE, FALSE );


   return( 0 );
}

//./ ADD NAME=RepaintZeidonControl
// Source Module=tzrpupdd.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      RepaintZeidonControl
//
// PURPOSE:    To re-paint a control after it has been updated
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RepaintZeidonControl( zVIEW vSubtask )
{
   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnContextMappingInit
//
// PURPOSE:  This function builds the MapDomain View for the Context
//           Combo Box, if there is current mapping.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnContextMappingInit( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );

   if ( CheckExistenceOfEntity( vDialog,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialog, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   }
   else
      ActivateEmptyObjectInstance( &vDomain, "TZDGSRCO", vSubtask, 0 );

   SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
PreBuildUserSpec( zVIEW vSubtask )
{
   zVIEW     vProfile;
   zVIEW     vTaskLPLR;
   zVIEW     vSpecList;

   oTZ__PRFO_GetViewToProfile( &vProfile, "WD", vSubtask, zCURRENT_OI );
   SetNameForView( vProfile, "ProfileXFER", vSubtask, zLEVEL_TASK );
   InitializeDefaultLPL( vSubtask );
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vSpecList, vTaskLPLR, 0 );
   SetNameForView( vSpecList, "UI_SpecList", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vSpecList, "W_MetaType", "Type", 2012, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AutoDesignReport( zVIEW     vSubtask )
{
   zVIEW     vSpecList;
   zVIEW     vReportSpec;
   zVIEW     vReportDef = 0;
   zVIEW     vPE;
   zVIEW     vPE_LPLR;
   zSHORT    nRC;

   GetViewByName( &vSpecList, "UI_SpecList", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vPE_LPLR, zREFER_PENV_META );
   nRC = LoadZeidonPPE( vSubtask, &vPE, zREFER_PENV_META, vPE_LPLR,
                        "Report Maintenance", "Autodesigner aborted.");
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( -16 );
   }

   nRC = ActivateMetaOI( vSubtask, &vReportSpec, vSpecList,
                         zREFER_UIS_META, zSINGLE );
   if ( nRC >= 0 )
   {
      nRC = GetViewByName( &vReportDef, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         TZRPUPDD_UpdatePageFromList( vSubtask );
         nRC = GetViewByName( &vReportDef, "TZWINDOW", vSubtask, zLEVEL_TASK );
      }

      if ( nRC < 0 )
      {
         MessageSend( vSubtask, "RP00101", "Autodesigner",
                      "A current page is not selected in Painter.  Exiting Autodesign.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }
      else
      {
         oTZRPSRCO_AutodesignReport( vReportDef, vReportSpec, vPE, vSubtask );
         SetNameForView( vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK );
      }
   }

   DropObjectInstance( vReportSpec );

   nRC = SetCursorFirstEntity( vReportDef, "Page", "" );
   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, "", 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_EDIT_TEXT_Init
//
// PURPOSE:  This function does the necessary work to set up
//           the Context combo box, and for EditBox sets up
//           Subtype in the work object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_EDIT_TEXT_Init( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;
// zLONG    lWork;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Create Temporal subobject for OK/Cancel
   CreateTemporalSubobjectVersion( vDialog, "Control" );

   // Set Subtype in work object for Window EditBox
   if ( CheckExistenceOfEntity( vWork, "EditBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "EditBox", zPOS_BEFORE );

// GetIntegerFromAttribute( &lWork, vDialog, "Control", "Subtype" );

   // We just want the low order bits from the subtype, so blank
   // out the high order bits
// lWork &= 0x0000FFFFL;   why??? DKS 1999/12/02
// SetAttributeFromInteger( vWork, "EditBox", "Subtype", lWork );

   // Set up Domain Context mapping.
   fnContextMappingInit( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FontColor( zVIEW vSubtask )
{
   zVIEW     vControl;
   zVIEW     vReport;
   zCHAR     szColor[ 16 ];
   COLORREF  clr;
   COLORREF  aclrCust[ 32 ];
   zCHAR     szCommandTag[ 33 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetLastCommandTag( vSubtask, szCommandTag );
   if ( zstrcmp( szCommandTag, "UpdateBorderColor" ) == 0 )
      GetCtrlText( vSubtask, "BorderColor", szColor, sizeof( szColor ) );
   else
      GetCtrlText( vSubtask, "Color", szColor, sizeof( szColor ) );

   clr = atol( szColor );
// TraceLineI( "FontColor colorref = ", (zLONG) clr );
   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
//    TraceLineI( "FontColor colorref after = ", (zLONG) clr );
      GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      {
         zltoa( clr, szColor, sizeof( szColor ) );
         if ( zstrcmp( szCommandTag, "UpdateBorderColor" ) == 0 )
            SetCtrlText( vSubtask, "BorderColor", szColor );
         else
            SetCtrlText( vSubtask, "Color", szColor );
      }
   }

   return( 0 );
}

// typedef struct tagLOGFONT
// {
//    LONG lfHeight;
//    LONG lfWidth;
//    LONG lfEscapement;
//    LONG lfOrientation;
//    LONG lfWeight;
//    BYTE lfItalic;
//    BYTE lfUnderline;
//    BYTE lfStrikeOut;
//    BYTE lfCharSet;
//    BYTE lfOutPrecision;
//    BYTE lfClipPrecision;
//    BYTE lfQuality;
//    BYTE lfPitchAndFamily;
//    CHAR lfFaceName[ LF_FACESIZE ];
// } LOGFONT;

zOPER_EXPORT zSHORT OPERATION
FontSpecs( zVIEW vSubtask )
{
   zVIEW    vControl;
   zVIEW    vReport;
   zCHAR    szColor[ 16 ];
   LOGFONT  lf;
   COLORREF clr;
   zLONG    lSize;
   zLONG    lEscapement;

   lf.lfWidth = 0;
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   lf.lfCharSet = ANSI_CHARSET;
   lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
   lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
   lf.lfQuality = PROOF_QUALITY;
   lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "Color", szColor, sizeof( szColor ) );
   clr = atol( szColor );
   GetCtrlText( vSubtask, "Size", szColor, sizeof( szColor ) );
   lSize = fnGetSize( szColor, 10 ) / 10;
   GetCtrlText( vSubtask, "Escapement", szColor, sizeof( szColor ) );
   lEscapement = atol( szColor );
   lf.lfHeight = lSize;
   GetCtrlText( vSubtask, "FontFace", lf.lfFaceName, sizeof( lf.lfFaceName ) );

   if ( GetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED ) )
      lf.lfWeight = 700;
   else
      lf.lfWeight = 400;

   if ( GetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED ) )
      lf.lfItalic = 1;
   else
      lf.lfItalic = 0;

   if ( GetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED ) )
      lf.lfUnderline = 1;
   else
      lf.lfUnderline = 0;

   if ( GetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED ) )
      lf.lfStrikeOut = 1;
   else
      lf.lfStrikeOut = 0;

   if ( OperatorPromptForFont( vSubtask, &lf, &lSize,
                               (zPLONG) &clr, TRUE ) == 1 )
   {
      GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      {
         zltoa( clr, szColor, sizeof( szColor ) );
         SetCtrlText( vSubtask, "Color", szColor );
         fnSetSize( szColor, lSize * 10, 10 );
         SetCtrlText( vSubtask, "Size", szColor );
//       zltoa( lf.lfEscapement, szColor, sizeof( szColor ) );
//       SetCtrlText( vSubtask, "Escapement", szColor );
         SetCtrlText( vSubtask, "FontFace", lf.lfFaceName );

         SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED,
                       lf.lfWeight > 500 ? TRUE : FALSE );
         SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED,
                       lf.lfItalic );
         SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED,
                       lf.lfStrikeOut );
         SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED,
                       lf.lfUnderline );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnRecurseChangeRptTags( zVIEW vReport )
{
   zCHAR  szTag[ 33 ];
   zCHAR  szText[ 256 ];
   zPCHAR pch;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vReport, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szTag, sizeof( szTag ), vReport, "Control", "Tag" );
      if ( szTag[ 0 ] == 'R' &&
           szTag[ 1 ] == 'p' &&
           szTag[ 2 ] == 't' &&
           szTag[ 3 ] == '_' )
      {
         GetStringFromAttribute( szText, sizeof( szText ), vReport, "Control", "Text" );
         if ( szText[ 0 ] )
         {
            szText[ 32 ] = 0;
            strcpy_s( szTag, sizeof( szTag ), szText );
         }
         else
         {
            szText[ 0 ] = 0;
            if ( CheckExistenceOfEntity( vReport, "CtrlMap" ) == 0 &&
                 CheckExistenceOfEntity( vReport, "CtrlMapLOD_Attribute" ) == 0 &&
                 CheckExistenceOfEntity( vReport, "CtrlMapER_Attribute" ) == 0 )
            {
               GetStringFromAttribute( szText, sizeof( szText ), vReport, "CtrlMapER_Attribute", "Name" );
            }
         }

         // Remove embedded spaces.
         while ( (pch = zstrchr( szText, ' ' )) != 0 )
            strcpy( pch, pch + 1 );

         if ( szText[ 0 ] )
            SetAttributeFromString( vReport, "Control", "Tag", szText );
      }

      SetViewToSubobject( vReport, "CtrlCtrl" );
      fnRecurseChangeRptTags( vReport );
      ResetViewFromSubobject( vReport );

      nRC = SetCursorNextEntity( vReport, "Control", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ChangeRptTags
//
// PURPOSE:  This function changes the (Rpt_) tags if there is a text
//           or an attribute set for the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ChangeRptTags( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vReport;
   zSHORT nRC;

   // Get the view.
   GetViewByName( &vControl, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   CreateViewFromView( &vReport, vControl );
   while ( ResetViewFromSubobject( vReport ) == 0 )
   {
      // nothing to do
   }

   // Loop through all GroupSets.
   for ( nRC = SetCursorFirstEntity( vReport, "GroupSet", "Page" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReport, "GroupSet", "Page" ) )
   {
      // Loop through all Groups within the GroupSet.
      for ( nRC = SetCursorFirstEntity( vReport, "Group", "GroupSet" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vReport, "Group", "GroupSet" ) )
      {
         fnRecurseChangeRptTags( vReport );
      }
   }

   DropView( vReport );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnRecurseMultiChangeRptTextCtrls( zVIEW vMultiChange, zVIEW vReport,
                                  LABEL_DATA *pldOrig, zLONG lCtrlType,
                                  zBOOL bCreate )
{
   LABEL_DATA ld;
   zULONG ulLth;
   zCHAR  szTag[ 33 ];
   zLONG  lType;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vReport, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vReport, "ControlDef", "Key" );
      if ( lType == lCtrlType )
      {
         ulLth = 84; // for reports
         GetStringFromAttribute( szTag, sizeof( szTag ), vReport, "Control", "Tag" );
         if ( bCreate )
         {
            CreateEntity( vMultiChange, "ErrorMsg", zPOS_AFTER );
            SetAttributeFromString( vMultiChange, "ErrorMsg", "Name", szTag );
            GetBlobFromAttribute( &ld, &ulLth, vReport, "Control", "CtrlBOI" );
            if ( zmemcmp( pldOrig, &ld, ulLth ) == 0 )
               SetSelectStateOfEntity( vMultiChange, "ErrorMsg", TRUE );
         }
         else
         {
            if ( SetCursorFirstEntityByString( vMultiChange, "ErrorMsg",
                                               "Name", szTag,
                                               0 ) >= zCURSOR_SET )
            {
               if ( GetSelectStateOfEntity( vMultiChange, "ErrorMsg" ) )
               {
                  SetAttributeFromBlob( vReport, "Control", "CtrlBOI",
                                        pldOrig, ulLth );
               }
            }
         }
      }

      SetViewToSubobject( vReport, "CtrlCtrl" );
      fnRecurseMultiChangeRptTextCtrls( vMultiChange, vReport,
                                     pldOrig, lCtrlType, bCreate );
      ResetViewFromSubobject( vReport );

      nRC = SetCursorNextEntity( vReport, "Control", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_TEXT_PostBuild_DisableCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_TEXT_PostBuild_DisableCtrls( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Size", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "__ColorText", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "__ColorBackground", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "__Font", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Escapement", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "MultiLine", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Horizontal", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Vertical", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "BorderTop", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "BorderLeft", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "BorderBottom", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "BorderRight", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Solid", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Dash", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Dot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DashDot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DashDotDot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineWidth", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "BorderColor", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // UPD_TEXT_PostBuild_DisableCtrls

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_PostBuild
//
// PURPOSE:  This function does the necessary work to set up
//           the Text font information using the following
//           structure:
//
//            byte  field          value
//            ----  ------      --------------------------------------------
//              0   Bold           1-on ... 0-off
//              1   Italic         1-on ... 0-off
//              2   Strikeout      1-on ... 0-off
//              3   Underline      1-on ... 0-off
//             4-7  PointSize      long != 0
//            8-11  Text Color     unsigned long RGB color value
//             12   MultiLine      1-on ... 0-off
//  Justification
//             13   Top/Left     (Offset)  1-left ... -1 left + offset  | 2-top
//             14   Center       1-horizontal ... 2-vertical
//             15   Bottom/Right (Offset) 1-right ... -1 right - offset | 2-bottom
//           16-19  BorderStyle (0-solid 1-dash 2-dot 3-dashdot 4-dashdotdot)
//               (second byte 1-top | 2-left | 4-bottom | 8-right)
//               (high order byte is line width)
//           20-23  BorderColor    unsigned long RGB color value
//           24-27  Escapement
//           28-31  lType          // reserved
//           32-51  Font face name
//           52-71  Reserved[ 20 ]
//           72-75  TextBkColor
//           76-79  BackgroundColor
//           80-83  HighlightColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_PostBuild( zVIEW vSubtask )
{
   LABEL_DATA ld;
   zPCHAR  pchHorizontal = "Left";
   zPCHAR  pchVertical = "Top";
   zCHAR   szWork[ 32 ];
   zVIEW   vControl;
   zVIEW   vReport;
   zLONG   lSubtype;
   zLONG   lBorder;
   zULONG  ulCommon;
   zULONG  ulLth = 84; // for reports

   fnPainterCall( zMSG_GET_COMMON_DETAIL_FLAGS, vSubtask, 0, (zPCHAR) &ulCommon, 0 );
   if ( ulCommon )
   {
      SetCtrlState( vSubtask, "PropertyGrid",
                    zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "Tag:", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "Text:", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "Tag", zCONTROL_STATUS_VISIBLE, FALSE );
      SetCtrlState( vSubtask, "Text", zCONTROL_STATUS_VISIBLE, FALSE );
   }
   else
   {
      SetCtrlState( vSubtask, "Tag:", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "Text:", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "Tag", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "Text", zCONTROL_STATUS_VISIBLE, TRUE );
      SetCtrlState( vSubtask, "PropertyGrid",
                    zCONTROL_STATUS_VISIBLE, FALSE );
   }

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   zmemset( &ld, 0, sizeof( ld ) );
   ld.lTextColor = 0xFF000000;
   ld.lTextBkColor = 0xFF000000;
   ld.lBackgroundColor = 0xFF000000;
   ld.lHighlightColor = 0xFF000000;

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & 0x00001000 )
      SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED, 1 );

   GetBlobFromAttribute( &ld, &ulLth, vControl, "Control", "CtrlBOI" );
// TraceLineI( "Text control blob lth = ", ulLth );
   if ( ulLth )
   {
      SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED,
                    ld.chBold ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED,
                    ld.chItalic ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED,
                    ld.chStrikeout ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED,
                    ld.chUnderline ? TRUE : FALSE );

      if ( ld.chMultiLine == 1 )
         SetCtrlState( vSubtask, "Variable", zCONTROL_STATUS_CHECKED, TRUE );
      else
      if ( ld.chMultiLine == 2 )
         SetCtrlState( vSubtask, "Fixed", zCONTROL_STATUS_CHECKED, TRUE );
      else
   // if ( ld.chMultiLine == 0 )
         SetCtrlState( vSubtask, "Off", zCONTROL_STATUS_CHECKED, TRUE );

      if ( ulLth > 5 + (2 * sizeof( zLONG )) )
      {
         if ( ld.chCenter & 1 )
            pchHorizontal = "Center";
         else
         if ( ld.chBottomRight < 0 )
            pchHorizontal = "Right Offset";
         else
         if ( ld.chBottomRight == 1 )
            pchHorizontal = "Right";
         else
         if ( ld.chTopLeft < 0 )
            pchHorizontal = "Left Offset";

         if ( ld.chBottomRight < 0 )
            ld.chBottomRight *= -1;

         if ( ld.chCenter & 2 )
            pchVertical = "Center";
         else
         if ( ld.chBottomRight & 2 )
            pchVertical = "Bottom";

         if ( ulLth > 8 + (2 * sizeof( zLONG )) )
         {
            GetIntegerFromAttribute( &lBorder, vControl, "Control", "BorderBottom" );
            TraceLineI("**** Border Bottom PostBuild *** ", lBorder );
            SetCtrlState( vSubtask, "BorderTop", zCONTROL_STATUS_CHECKED,
                          (ld.lBorderStyle & 0x00000100) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderLeft", zCONTROL_STATUS_CHECKED,
                          (ld.lBorderStyle & 0x00000200) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderBottom", zCONTROL_STATUS_CHECKED,
                          (ld.lBorderStyle & 0x00000400) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderRight", zCONTROL_STATUS_CHECKED,
                          (ld.lBorderStyle & 0x00000800) ? TRUE : FALSE );
            switch ( ld.lBorderStyle & 0x0000000F )
            {
               case 1:
                  SetCtrlState( vSubtask, "Dash",
                                zCONTROL_STATUS_CHECKED, TRUE );
                  break;

               case 2:
                  SetCtrlState( vSubtask, "Dot",
                                zCONTROL_STATUS_CHECKED, TRUE );
                  break;

               case 3:
                  SetCtrlState( vSubtask, "DashDot",
                                zCONTROL_STATUS_CHECKED, TRUE );
                  break;

               case 4:
                  SetCtrlState( vSubtask, "DashDotDot",
                                zCONTROL_STATUS_CHECKED, TRUE );
                  break;

               default:
                  SetCtrlState( vSubtask, "Solid",
                                zCONTROL_STATUS_CHECKED, TRUE );
                  break;
            }

            SetCtrlText( vSubtask, "__Font", ld.szFontName );

            ld.lBorderStyle >>= 24;
            zltoa( ld.lBorderStyle, szWork, sizeof( szWork ) );
            SetCtrlText( vSubtask, "LineWidth", szWork );
         }
      }
      else
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );

      fnSetSize( szWork, ld.lPointSize, 10 );
      SetCtrlText( vSubtask, "Size", szWork );
      zltoa( ld.lBorderColor, szWork, sizeof( szWork ) );
      SetCtrlText( vSubtask, "BorderColor", szWork );
      zltoa( ld.lEscapement, szWork, sizeof( szWork ) );
      SetCtrlText( vSubtask, "Escapement", szWork );
   }

   CB_SetData( vSubtask, "Horizontal", pchHorizontal,
               "Left;Center;Right;Left Offset;Right Offset", 0 );
   CB_SetData( vSubtask, "Vertical", pchVertical,
               "Top;Center;Bottom", 0 );

   SetCtrlProperty( vSubtask, "__ColorText",
                    zCONTROL_PROPERTY_INTEGER_DATA, ld.lTextColor, 0 );
// TraceLine( "UPD_TEXT_PostBuild Text Color: 0x%08x", ld.lTextColor );
   SetCtrlProperty( vSubtask, "__ColorBackground",
                    zCONTROL_PROPERTY_INTEGER_DATA, ld.lTextBkColor, 0 );

   SetCtrlTextLth( vSubtask, "LineWidth", 2 );  // limit border width to 99

   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ) )
      UPD_TEXT_PostBuild_DisableCtrls( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_OK
//
// PURPOSE:  This function sets the Context as necessary and tells
//           the Painter to refresh the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_OK( zVIEW vSubtask )
{
   LABEL_DATA ld;
   zCHAR   szWork[ 32 ];
   zVIEW   vControl;
   zVIEW   vMultiChange;
   zLONG   lSubtype;
   zULONG  ulLth;
   zULONG  ulCommon;

   // Initialize the blob.
   zmemset( &ld, 0, sizeof( LABEL_DATA ) );

   // Get the view.
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   lSubtype &= 0xFFFF0000L;
   if ( GetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED ) > 0 )
      lSubtype |= 0x00001000;
   else
      lSubtype &= ~0x00001000;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   // Set things.
   if ( GetCtrlState( vSubtask, "Variable", zCONTROL_STATUS_CHECKED ) )
      ld.chMultiLine = 1;
   else
   if ( GetCtrlState( vSubtask, "Fixed", zCONTROL_STATUS_CHECKED ) )
      ld.chMultiLine = 2;
   else
// if ( GetCtrlState( vSubtask, "Off", zCONTROL_STATUS_CHECKED ) )
      ld.chMultiLine = 0;

   ld.chBold = (char) GetCtrlState( vSubtask, "Bold",
                                    zCONTROL_STATUS_CHECKED );
   ld.chItalic = (char) GetCtrlState( vSubtask, "Italic",
                                      zCONTROL_STATUS_CHECKED );
   ld.chStrikeout = (char) GetCtrlState( vSubtask, "Strikeout",
                                         zCONTROL_STATUS_CHECKED );
   ld.chUnderline = (char) GetCtrlState( vSubtask, "Underline",
                                         zCONTROL_STATUS_CHECKED );

   CB_GetSelectedString( vSubtask, "Horizontal", szWork, sizeof( szWork ) );
   //SetAttributeFromString( vControl, "Control", "TextAlignHorizontal", szWork );

   if ( zstrcmp( szWork, "Center" ) == 0 )
   {
      ld.chCenter = 1;
      SetAttributeFromString( vControl, "Control", "TextAlignHorizontal", "center" );
   }
   else
   if ( zstrcmp( szWork, "Right Offset" ) == 0 )
      ld.chBottomRight = -1;
   else
   if ( zstrcmp( szWork, "Right" ) == 0 )
   {
      ld.chBottomRight = 1;
      SetAttributeFromString( vControl, "Control", "TextAlignHorizontal", "right" );
   }
   else
   if ( zstrcmp( szWork, "Left Offset" ) == 0 )
      ld.chTopLeft = -1;
   else
// if ( zstrcmp( szWork, "Left" ) == 0 )
   {
      ld.chTopLeft = 1;
      SetAttributeFromString( vControl, "Control", "TextAlignHorizontal", "left" );
   }

   CB_GetSelectedString( vSubtask, "Vertical", szWork, sizeof( szWork ) );
   if ( zstrcmp( szWork, "Center" ) == 0 )
      ld.chCenter |= 2;
   else
   if ( zstrcmp( szWork, "Bottom" ) == 0 )
   {
      if ( ld.chBottomRight < 0 )
      {
         ld.chBottomRight *= -1;
         ld.chBottomRight |= 2;
         ld.chBottomRight *= -1;
      }
      else
         ld.chBottomRight |= 2;
   }

   GetCtrlText( vSubtask, "BorderColor", szWork, sizeof( szWork ) );
   ld.lBorderColor = atol( szWork );

   GetCtrlText( vSubtask, "LineWidth", szWork, sizeof( szWork ) );
   ld.lBorderStyle = atol( szWork );
   ld.lBorderStyle <<= 24;  // push the pen width to the high-order byte.

   if ( GetCtrlState( vSubtask, "BorderTop", zCONTROL_STATUS_CHECKED ) )
   {
      ld.lBorderStyle |= 0x00000100;
      SetAttributeFromInteger( vControl, "Control", "BorderTop", 1 );
      TraceLineI( "*** Setting BorderTop to 1 *** ", 1 );
   }

   if ( GetCtrlState( vSubtask, "BorderLeft", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000200;

   if ( GetCtrlState( vSubtask, "BorderBottom", zCONTROL_STATUS_CHECKED ) )
   {
      ld.lBorderStyle |= 0x00000400;
      SetAttributeFromInteger( vControl, "Control", "BorderBottom", 1 );
      TraceLineI( "*** Setting BorderBottom to 1 *** ", 1 );
   }

   if ( GetCtrlState( vSubtask, "BorderRight", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000800;

   if ( GetCtrlState( vSubtask, "Dash", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000001;

   if ( GetCtrlState( vSubtask, "Dot", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000002;

   if ( GetCtrlState( vSubtask, "DashDot", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000003;

   if ( GetCtrlState( vSubtask, "DashDotDot", zCONTROL_STATUS_CHECKED ) )
      ld.lBorderStyle |= 0x00000004;

   GetCtrlText( vSubtask, "Size", szWork, sizeof( szWork ) );
   ld.lPointSize = fnGetSize( szWork, 10 );
   GetCtrlProperty( vSubtask, "__ColorText",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &ld.lTextColor, 0, 0 );
   GetCtrlProperty( vSubtask, "__ColorBackground",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &ld.lTextBkColor, 0, 0 );
   ulLth = LF_FACESIZE;
   GetCtrlProperty( vSubtask, "__Font", zCONTROL_PROPERTY_STRING_DATA,
                    &ulLth, ld.szFontName, sizeof( ld.szFontName ) );
   GetCtrlText( vSubtask, "Escapement", szWork, sizeof( szWork ) );
   ld.lEscapement = atol( szWork );

   if ( ld.chMultiLine || ld.chBold || ld.chItalic || ld.chStrikeout ||
        ld.chUnderline || ld.lPointSize || ld.lTextColor || ld.lTextBkColor ||
        ld.lBorderStyle || ld.lBorderColor || ld.lEscapement ||
        ld.lType || ld.chCenter || ld.chBottomRight || ld.chTopLeft < 0 ||
        ld.szFontName )
   {
      ulLth = 84;
   }
   else
   {
      ulLth = 0;
   }

// TraceLine( "UPD_TEXT_OK Text Color: 0x%08x", ld.lTextColor );

   // Store the blob.
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", &ld, ulLth );

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
   {
      zVIEW  vReport;
      zLONG  lCtrlType;
      zSHORT nRC;

      GetIntegerFromAttribute( &lCtrlType, vControl, "ControlDef", "Key" );
      CreateViewFromView( &vReport, vControl );
      while ( ResetViewFromSubobject( vReport ) == 0 )
      {
         // nothing to do
      }

      // Loop through all GroupSets.
      for ( nRC = SetCursorFirstEntity( vReport, "GroupSet", "Page" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vReport, "GroupSet", "Page" ) )
      {
         // Loop through all Groups within the GroupSet.
         for ( nRC = SetCursorFirstEntity( vReport, "Group", "GroupSet" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vReport, "Group", "GroupSet" ) )
         {
            fnRecurseMultiChangeRptTextCtrls( vMultiChange, vReport,
                                              &ld, lCtrlType, FALSE );
         }
      }

      DropView( vReport );
      DropObjectInstance( vMultiChange );
   }

   // Accept control subobject.
   AcceptSubobject( vControl, "Control" );

   fnPainterCall( zMSG_GET_COMMON_DETAIL_FLAGS, vSubtask, 0, (zPCHAR) &ulCommon, 0 );
   if ( ulCommon )
   {
      zVIEW  vTZPESRCO;
      zVIEW  vTemp;
      zPCHAR pch;
      zPCHAR pchTag;
      zPCHAR pchUpdate;
      zCHAR  szEntity[ 33 ];
      zLONG  lKey;
      zLONG  lKey1;
      zSHORT nRC;
      zSHORT nPos = -1;

      GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
      lKey = GetEntityKey( vControl, "Control" );
      CreateViewFromViewForTask( &vTemp, vControl, 0 );
      do
      {
         fnPainterCall( zMSG_GET_NEXT_SELECTED_CTRL, vSubtask, vTemp, (zPCHAR) &nPos, 0 );
         if ( nPos >= 0 )
         {
            lKey1 = GetEntityKey( vTemp, "Control" );
            if ( lKey == lKey1 )
               continue;

            nRC = SetCursorFirstEntity( vTZPESRCO, "PropertyDef", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               GetAddrForAttribute( &pch, vTZPESRCO,
                                    "PropertyDef", "UpdateFlag" );
               if ( *pch == 'Y' )
               {
                  GetAddrForAttribute( &pchUpdate, vTZPESRCO,
                                       "PropertyDef", "Update" );
                  if ( *pchUpdate )
                  {
                     if ( *pchUpdate == '[' &&
                          (pch = zstrchr( pchUpdate, ']' )) != 0 &&
                          (pch - pchUpdate) < sizeof( szEntity ) )
                     {
                        strncpy_s( szEntity, sizeof( szEntity ), pchUpdate + 1, pch - pchUpdate );
                        szEntity[ pch - (pchUpdate + 1) ] = 0;
                        if ( CheckExistenceOfEntity( vTemp, szEntity ) )
                           szEntity[ 0 ] = 0;
                     }
                     else
                     {
                        strcpy_s( szEntity, sizeof( szEntity ), "Control" );
                     }

                     if ( szEntity[ 0 ] )
                     {
                        GetAddrForAttribute( &pchTag, vTZPESRCO, "PropertyDef", "Tag" );
                        if ( *pchTag == '*' )
                        {
                           SetMatchingAttributesByName( vTemp, szEntity, vControl, szEntity, zSET_ALL );
                        }
                        else
                        if ( *pchTag == '~' )
                        {
                           zLONG lZKey;
                           zVIEW vSubtask;

                           CreateViewFromViewForTask( &vSubtask, vControl, 0 );
                           while ( ResetViewFromSubobject( vSubtask ) == 0 )
                           {
                           }

                           GetIntegerFromAttribute( &lZKey, vControl,
                                                    szEntity, "ZKey" );
                           if ( SetCursorFirstEntityByInteger( vSubtask,
                                                               "ViewObjRef",
                                                               "ZKey", lZKey,
                                                               "" ) == 0 )
                           {
                              ExcludeEntity( vTemp, szEntity, zREPOS_PREV );
                              IncludeSubobjectFromSubobject( vTemp, szEntity,
                                                             vSubtask,
                                                             "ViewObjRef",
                                                             zPOS_AFTER );
                           }
                        }
                        else
                        {
                           SetAttributeFromAttribute( vTemp, "Control",
                                                      pchTag,
                                                      vControl, "Control",
                                                      pchTag );
                        }
                     }
                  }
               }

               nRC = SetCursorNextEntity( vTZPESRCO, "PropertyDef", 0 );
            }
         }

      } while ( nPos >= 0 );

      DropView( vTemp );
   }

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_Cancel
//
// PURPOSE:  This function handles the temporal subobject for cancel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_Cancel( zVIEW vSubtask )
{
   zVIEW   vMultiChange;
   zVIEW   vDialogC;
   zULONG  ulCommon;

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
      DropObjectInstance( vMultiChange );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Cancel control subobject
   CancelSubobject( vDialogC, "Control" );

   fnPainterCall( zMSG_GET_COMMON_DETAIL_FLAGS, vSubtask, 0, (zPCHAR) &ulCommon, 0 );
   if ( ulCommon )
   {
      zSHORT nPos = 32000;    // clean up selection tracking windows
      fnPainterCall( zMSG_GET_NEXT_SELECTED_CTRL, vSubtask, 0, (zPCHAR) &nPos, 0 );
   }

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent | zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
DropMultiSelectOI( zVIEW vSubtask )
{
   zVIEW  vMultiChange;

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
      DropObjectInstance( vMultiChange );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetupMultiChangeText
//
// PURPOSE:  This function creates the MultiChange object for multiple
//           changes to the same type of control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SetupMultiChangeRptText( zVIEW vSubtask )
{
   LABEL_DATA ld;
   zULONG ulLth = 84; // for reports
   zVIEW  vReport;
   zVIEW  vDialogC;
   zVIEW  vMultiChange;
   zLONG  lCtrlType;
   zSHORT nRC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lCtrlType, vDialogC, "ControlDef", "Key" );
   GetBlobFromAttribute( &ld, &ulLth, vDialogC, "Control", "CtrlBOI" );
   CreateViewFromView( &vReport, vDialogC );
   while ( ResetViewFromSubobject( vReport ) == 0 )
   {
      // nothing to do
   }

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
      DropObjectInstance( vMultiChange );

   ActivateEmptyObjectInstance( &vMultiChange, "TZERROR", vSubtask, 0 );
   SetNameForView( vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   CreateEntity( vMultiChange, "ErrorList", zPOS_AFTER );

   // Loop through all GroupSets.
   for ( nRC = SetCursorFirstEntity( vReport, "GroupSet", "Page" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReport, "GroupSet", "Page" ) )
   {
      // Loop through all Groups within the GroupSet.
      for ( nRC = SetCursorFirstEntity( vReport, "Group", "GroupSet" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vReport, "Group", "GroupSet" ) )
      {
         fnRecurseMultiChangeRptTextCtrls( vMultiChange, vReport,
                                           &ld, lCtrlType, TRUE );
      }
   }

   DropView( vReport );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShapePostBuild_DisableCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ShapePostBuild_DisableCtrls( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "RoundRect", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Ellipse", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Diamond", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "RoundValue", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "__ColorShape", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "FullSize", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "HorizontalLine", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "VerticalLine", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DiagonalDescend", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "DiagonalAscend", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "PenWidth", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "__ColorLine", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineTypeSolid", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineTypeDash", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineTypeDot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineTypeDashDot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "LineTypeDashDotDot", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );

} // ShapePostBuild_DisableCtrls

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ShapePostBuild
//
// PURPOSE:  This function does the necessary work to set up
//           the Shape check boxes.
//
//             byte  field       value
//             ----- ------      -----------------------------
//               0   Rectangle   1 - on ... 0 - off
//               1   RoundRect   1 - on ... 0 - off
//               2   Ellipse     1 - on ... 0 - off
//               3   Diamond     1 - on ... 0 - off
//               4   HorizontalLine 1 - on ... 0 - off
//               5   VerticalLine 1 - on ... 0 - off
//               6   DiagonalDescend 1 - on ... 0 - off
//               7   DiagonalAscend 1 - on ... 0 - off
//             8-11  PenWidth
//             12-15 PenType
//             16-19 PenColor
//             20-23 RoundRectValue
//             24-27 ShapeColor
//             28-31 Flags (1-FullSize)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ShapePostBuild( zVIEW vSubtask )
{
   zCHAR   szBlob[ 8 + (6 * sizeof( zLONG )) ];
   zVIEW   vControl;
   zVIEW   vReport;
   zLONG   lSubtype;
   zULONG  ulPenColor = 0xFF000000;
   zULONG  ulShapeColor = 0xFF000000;
   zULONG  ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & 0x00001000 )
      SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED, 1 );

   zmemset( szBlob, 0, ulLth );
   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
// TraceLineI( "Shape control blob lth = ", ulLth );
   if ( ulLth == sizeof( szBlob ) ||
        ulLth == sizeof( szBlob ) - (1 * sizeof( zLONG )) || // remove this soon
        ulLth == sizeof( szBlob ) - (2 * sizeof( zLONG )) )
   {
      zLONG   lPenWidth;
      zLONG   lPenType;
      zLONG   lRoundValue;
      zLONG   lFlags = 0;

      SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_CHECKED,
                    szBlob[ 0 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "RoundRect", zCONTROL_STATUS_CHECKED,
                    szBlob[ 1 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Ellipse", zCONTROL_STATUS_CHECKED,
                    szBlob[ 2 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Diamond", zCONTROL_STATUS_CHECKED,
                    szBlob[ 3 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "HorizontalLine", zCONTROL_STATUS_CHECKED,
                    szBlob[ 4 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "VerticalLine", zCONTROL_STATUS_CHECKED,
                    szBlob[ 5 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "DiagonalDescend", zCONTROL_STATUS_CHECKED,
                    szBlob[ 6 ] ? TRUE : FALSE );
      SetCtrlState( vSubtask, "DiagonalAscend", zCONTROL_STATUS_CHECKED,
                    szBlob[ 7 ] ? TRUE : FALSE );

      lPenWidth = *((zPLONG) (szBlob + 8));
      lPenType = *((zPLONG) (szBlob + 8 + sizeof( zLONG )));
      ulPenColor = *((zPLONG) (szBlob + 8 + (2 * sizeof( zLONG ))));
      lRoundValue = *((zPLONG) (szBlob + 8 + (3 * sizeof( zLONG ))));
      if ( ulLth == sizeof( szBlob ) )
      {
         ulShapeColor = *((zPULONG) (szBlob + 8 + (4 * sizeof( zLONG ))));
         lFlags = *((zPLONG) (szBlob + 8 + (5 * sizeof( zLONG ))));
      }

      SetCtrlState( vSubtask, "FullSize", zCONTROL_STATUS_CHECKED,
                    (lFlags & 0x00000001) ? TRUE : FALSE );

      zltoa( lPenWidth, szBlob, sizeof( szBlob ) );
      SetCtrlText( vSubtask, "PenWidth", szBlob );
      zltoa( lPenType, szBlob, sizeof( szBlob ) );
      switch ( lPenType )
      {
         case 1:
            SetCtrlState( vSubtask, "LineTypeDash",
                          zCONTROL_STATUS_CHECKED, TRUE );
            break;

         case 2:
            SetCtrlState( vSubtask, "LineTypeDot",
                          zCONTROL_STATUS_CHECKED, TRUE );
            break;

         case 3:
            SetCtrlState( vSubtask, "LineTypeDashDot",
                          zCONTROL_STATUS_CHECKED, TRUE );
            break;

         case 4:
            SetCtrlState( vSubtask, "LineTypeDashDotDot",
                          zCONTROL_STATUS_CHECKED, TRUE );
            break;

         default:
            SetCtrlState( vSubtask, "LineTypeSolid",
                          zCONTROL_STATUS_CHECKED, TRUE );
            break;
      }

   // SetCtrlText( vSubtask, "LineType", szBlob );

      zltoa( lRoundValue, szBlob, sizeof( szBlob ) );
      SetCtrlText( vSubtask, "RoundValue", szBlob );
   }
   else
   {
      SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_CHECKED, TRUE );
      SetCtrlText( vSubtask, "PenWidth", "1" );
   }

   SetCtrlProperty( vSubtask, "__ColorLine",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    ulPenColor, 0 );
   SetCtrlProperty( vSubtask, "__ColorShape",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    ulShapeColor, 0 );
// TraceLine( "ShapePostBuild Text Color: 0x%08x", ulShapeColor );

   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ) )
      ShapePostBuild_DisableCtrls( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ShapeCancel
//
// PURPOSE:  This function handles the temporal subobject for cancel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ShapeCancel( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Accept control subobject
   CancelSubobject( vDialogC, "Control" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
                                         zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ShapeOK
//
// PURPOSE:  This function handles the temporal subobject for cancel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ShapeOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zCHAR   szBlob[ 8 + (6 * sizeof( zLONG )) ];
   zBOOL   bRectangle;
   zBOOL   bRoundRect;
   zBOOL   bEllipse;
   zBOOL   bDiamond;
   zBOOL   bHorizontalLine;
   zBOOL   bVerticalLine;
   zBOOL   bDiagonalDescend;
   zBOOL   bDiagonalAscend;
   zLONG   lPenWidth;
   zLONG   lPenType;
   zULONG  ulPenColor;
   zULONG  ulShapeColor;
   zLONG   lRoundValue;
   zLONG   lFlags = 0;
   zPCHAR  pch;
   zLONG   lSubtype;
   zULONG  ulLth;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // set things
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( GetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED ) > 0 )
      lSubtype |= 0x00001000;
   else
      lSubtype &= ~0x00001000;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   lSubtype &= 0xFFFF0000L;
   bRectangle = (zBOOL) GetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_CHECKED );
   bRoundRect = (zBOOL) GetCtrlState( vSubtask, "RoundRect", zCONTROL_STATUS_CHECKED );
   bEllipse = (zBOOL) GetCtrlState( vSubtask, "Ellipse", zCONTROL_STATUS_CHECKED );
   bDiamond = (zBOOL) GetCtrlState( vSubtask, "Diamond", zCONTROL_STATUS_CHECKED );
   bHorizontalLine = (zBOOL) GetCtrlState( vSubtask, "HorizontalLine", zCONTROL_STATUS_CHECKED );
   bVerticalLine = (zBOOL) GetCtrlState( vSubtask, "VerticalLine", zCONTROL_STATUS_CHECKED );
   bDiagonalDescend = (zBOOL) GetCtrlState( vSubtask, "DiagonalDescend", zCONTROL_STATUS_CHECKED );
   bDiagonalAscend = (zBOOL) GetCtrlState( vSubtask, "DiagonalAscend", zCONTROL_STATUS_CHECKED );
   if ( GetCtrlState( vSubtask, "FullSize", zCONTROL_STATUS_CHECKED ) )
      lFlags |= 0x00000001;

   GetCtrlText( vSubtask, "PenWidth", szBlob, sizeof( szBlob ) );
   lPenWidth = atol( szBlob );
   pch = (zPCHAR) GetCtrlState( vSubtask, "LineType", zCONTROL_STATUS_CHECKED );
   if ( pch && pch != (zPCHAR) -1 )
      lPenType = atol( pch );
   else
      lPenType = 0;

   GetCtrlProperty( vSubtask, "__ColorLine",
                    zCONTROL_PROPERTY_INTEGER_DATA, &ulPenColor, 0, 0 );
   GetCtrlText( vSubtask, "RoundValue", szBlob, sizeof( szBlob ) );
   lRoundValue = atol( szBlob );
   GetCtrlProperty( vSubtask, "__ColorShape", zCONTROL_PROPERTY_INTEGER_DATA, &ulShapeColor, 0, 0 );

   if ( bRoundRect || bEllipse || bDiamond || bHorizontalLine ||
        bVerticalLine || bDiagonalDescend || bDiagonalAscend ||
        lPenWidth != 1 || lPenType || ulPenColor || lRoundValue || ulShapeColor || lFlags )
   {
      zPLONG lpLong;

      // initialize the blob
      zmemset( szBlob, 0, sizeof( szBlob ) );
      ulLth = sizeof( szBlob );

      szBlob[ 0 ] = bRectangle;
      szBlob[ 1 ] = bRoundRect;
      szBlob[ 2 ] = bEllipse;
      szBlob[ 3 ] = bDiamond;
      szBlob[ 4 ] = bHorizontalLine;
      szBlob[ 5 ] = bVerticalLine;
      szBlob[ 6 ] = bDiagonalDescend;
      szBlob[ 7 ] = bDiagonalAscend;

      lpLong = (zPLONG) (szBlob + 8);
      *lpLong = lPenWidth;
      lpLong = (zPLONG) (szBlob + 8 + sizeof( zLONG ));
      *lpLong = lPenType;
      lpLong = (zPLONG) (szBlob + 8 + (2 * sizeof( zLONG )));
      *lpLong = (zLONG) ulPenColor;
      lpLong = (zPLONG) (szBlob + 8 + (3 * sizeof( zLONG )));
      *lpLong = lRoundValue;
      lpLong = (zPLONG) (szBlob + 8 + (4 * sizeof( zLONG )));
      *lpLong = (zLONG) ulShapeColor;
   // TraceLine( "ShapeOK Text Color: 0x%08x", ulShapeColor );
      lpLong = (zPLONG) (szBlob + 8 + (5 * sizeof( zLONG )));
      *lpLong = (zLONG) lFlags;
   }
   else
   {
      ulLth = 0;
   }

   // store the blob
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ShapeUpdateColor( zVIEW vSubtask )
{
   zCHAR     szColor[ 16 ];
   COLORREF  clr;
   COLORREF  aclrCust[ 32 ];
   zVIEW     vControl;
   zVIEW     vReport;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "PenColor", szColor, sizeof( szColor ) );
   clr = atol( szColor );
// TraceLineI( "FontColor colorref = ", (zLONG) clr );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
   // TraceLineI( "FontColor colorref after = ", (zLONG) clr );
      GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      {
         zltoa( clr, szColor, sizeof( szColor ) );
         SetCtrlText( vSubtask, "PenColor", szColor );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: RefreshPropertyAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
RefreshPropertyAttribute( zVIEW vSubtask )
{
   zCHAR szProperty[ 256 ];
   zCHAR szPropertyValue[ 256 ];
   zVIEW vControl;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "CtrlProperty" ) == 0 )
   {
      GetStringFromAttribute( szProperty, sizeof( szProperty ), vControl, "CtrlProperty", "Name" );
      GetStringFromAttribute( szPropertyValue, sizeof( szPropertyValue ), vControl, "CtrlProperty", "Value" );
      TraceLineS( "RefreshPropertyAttribute Name: ", szProperty );
      TraceLineS( "RefreshPropertyAttribute Value: ", szPropertyValue );

      CB_SelectString( vSubtask, "BlockProperty", szProperty, 1 );
   // SetCtrlText( vSubtask, "BlockProperty", szPropertyValue );
      SetCtrlText( vSubtask, "BlockPropertyValue", szPropertyValue );
   }
   else
   {
      SetCtrlText( vSubtask, "BlockProperty", "" );
      SetCtrlText( vSubtask, "BlockPropertyValue", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: BlockPostBuild_DisableCtrls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
BlockPostBuild_DisableCtrls( zVIEW vSubtask )
{
// SetCtrlState( vSubtask, "Rectangle", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );

} // BlockPostBuild_DisableCtrls

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BlockPostBuild
//
// PURPOSE:  This function does the necessary work to set up
//           the Block check boxes.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BlockPostBuild( zVIEW vSubtask )
{
   CB_SetData( vSubtask, "BlockProperty", "",
               "position;border;font-family;"
               "azimuth;background-attachment;background-color;background-image;background-repeat;border-after-color;border-after-style;"
               "border-after-width;border-before-color;border-before-style;border-before-width;border-bottom-color;border-bottom-style;"
               "border-bottom-width;border-end-color;border-end-style;border-end-width;border-left-color;border-left-style;border-left-width;"
               "border-right-color;border-right-style;border-right-width;border-start-color;border-start-style;border-start-width;border-top-color;"
               "border-top-style;border-top-width;bottom;break-after;break-before;color;country;cue-after;cue-before;elevation;end-indent;"
               "hyphenation-keep;hyphenation-ladder-count;id;intrusion-displace;keep-together;keep-with-next;keep-with-previous;language;"
               "last-line-end-indent;left;line-height;linefeed-treatment;margin-bottom;margin-left;margin-right;margin-top;"
               "padding-after;padding-before;padding-bottom;padding-end;padding-left;padding-right;padding-start;padding-top;"
               "pause-after;pause-before;pitch;pitch-range;play-during;relative-position;richness;right;role;source-document;"
               "space-after;space-before;speak;speak-header;speak-numeral;speak-punctuation;speech-rate;start-indent;stress;"
               "text-altitude;text-depth;top;visibility;voice-family;volume",
               0 );  // ==> ListData is a semi-colon separated list


   RefreshPropertyAttribute( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BlockCancel
//
// PURPOSE:  This function handles the temporal subobject for cancel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BlockCancel( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Accept control subobject
   CancelSubobject( vDialogC, "Control" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
                                         zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BlockOK
//
// PURPOSE:  This function handles the temporal subobject for cancel.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BlockOK( zVIEW vSubtask )
{
   zVIEW   vControl;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // set things
   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: AddBlockAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
AddBlockAttribute( zVIEW vSubtask )
{
   zCHAR szProperty[ 256 ];
   zCHAR szPropertyValue[ 256 ];
   zVIEW vControl;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vControl, "CtrlProperty", zPOS_AFTER );
   CB_GetSelectedString( vSubtask, "BlockProperty", szProperty, sizeof( szProperty ) );
// GetCtrlText( vSubtask, "BlockProperty", szPropertyValue, sizeof( szProperty ) );
   GetCtrlText( vSubtask, "BlockPropertyValue", szPropertyValue, sizeof( szPropertyValue ) );

   SetAttributeFromString( vControl, "CtrlProperty", "Name", szProperty );
   SetAttributeFromString( vControl, "CtrlProperty", "Value", szPropertyValue );
   RefreshCtrl( vSubtask, "BlockProperties" );
   RefreshPropertyAttribute( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UpdateBlockAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateBlockAttribute( zVIEW vSubtask )
{
   zCHAR szProperty[ 256 ];
   zCHAR szPropertyValue[ 256 ];
   zVIEW vControl;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "CtrlProperty" ) == 0 )
   {
      CB_GetSelectedString( vSubtask, "BlockProperty", szProperty, sizeof( szProperty ) );
   // GetCtrlText( vSubtask, "BlockProperty", szPropertyValue, sizeof( szProperty ) );
      GetCtrlText( vSubtask, "BlockPropertyValue", szPropertyValue, sizeof( szPropertyValue ) );

      SetAttributeFromString( vControl, "CtrlProperty", "Name", szProperty );
      SetAttributeFromString( vControl, "CtrlProperty", "Value", szPropertyValue );
      RefreshCtrl( vSubtask, "BlockProperties" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DeleteBlockAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DeleteBlockAttribute( zVIEW vSubtask )
{
   zVIEW vControl;

   // get the view
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "CtrlProperty" ) == 0 )
   {
      DeleteEntity( vControl, "CtrlProperty", zREPOS_NEXT );
      RefreshCtrl( vSubtask, "BlockProperties" );
      RefreshPropertyAttribute( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
BlockUpdateColor( zVIEW vSubtask )
{
   zCHAR     szColor[ 16 ];
   COLORREF  clr;
   COLORREF  aclrCust[ 32 ];
   zVIEW     vControl;
   zVIEW     vReport;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

// GetCtrlText( vSubtask, "PenColor", szColor, sizeof( szColor ) );
   clr = atol( szColor );
// TraceLineI( "FontColor colorref = ", (zLONG) clr );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
   // TraceLineI( "FontColor colorref after = ", (zLONG) clr );
      GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
      {
         zltoa( clr, szColor, sizeof( szColor ) );
      // SetCtrlText( vSubtask, "PenColor", szColor );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActionInitPrebuild( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpCtlPostBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vReport;
   zLONG   lSubtype;
   zPCHAR  pCtrlBOI;
   zULONG  ulLth;
   zCTRLBOI_BMP BMP;
   zCHAR   szName[ zMAX_FILENAME_LTH + 10 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetAttributeLength( &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth )
   {
      GetAddrForAttribute( &pCtrlBOI, vControl, "Control", "CtrlBOI" );
      SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );

      SetCtrlText( vSubtask, "DLL_Name", BMP.szDLLName );

      // show BMP name and user maintained ID, but no generated ID
      //   ID == 0: no ID given
      //   ID != 0 && <= zMAX_RES_ID_USER: ID from user, show it
      //   ID != 0 && > zMAX_RES_ID_USER: generated ID, don't show it
      if ( BMP.lBMPUpOrIconID && BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
        sprintf( szName, "%ld;%s", BMP.lBMPUpOrIconID, BMP.szBMPUpOrIconName );
      else
        sprintf( szName, "%s", BMP.szBMPUpOrIconName );

      SetCtrlText( vSubtask, "FileName", szName );
   }

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & 0x00000001 )
      SetCtrlState( vSubtask, "Fit", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & 0x00000002 )
      SetCtrlState( vSubtask, "UseDDB", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & 0x00001000 )
      SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED, 1 );

   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   if ( !ComponentIsCheckedOut( vSubtask, vReport, zSOURCE_REPORT_META ))
   {
      SetCtrlState( vSubtask, "DLL_Name", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "FileName", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Browse", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Fit", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UseDDB", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpCtlOK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zLONG  lSubtype;
   zCHAR  szBlob[ 256 ];
   zLONG  k;
   zULONG ulLth;
   zPCHAR pCtrlBOI;
   zCTRLBOI_BMP BMP;
   zCHAR  szName[ zMAX_FILENAME_LTH + 10 ];
   zPCHAR pszBMPName;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   // get old CtrlBOI into CtrlBOI_BMP structure
   GetAddrForAttribute( &pCtrlBOI, vControl, "Control", "CtrlBOI" );
   GetAttributeLength( &ulLth, vControl, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );

   // set new CtrlBOI
   GetCtrlText( vSubtask, "DLL_Name", BMP.szDLLName, sizeof( BMP.szDLLName ) );

   // if there is input for file name
   GetCtrlText( vSubtask, "FileName", szName, sizeof( szName ) );
   if ( szName[ 0 ] )
   {
      // has the line an ID separated by ';'
      pszBMPName = szName;
      while ( *pszBMPName && *pszBMPName != ';' )
         pszBMPName++;

      if ( *pszBMPName == 0 )
      {
         // if no ID given look for old one
         if ( BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
         {
            // if not generated, delete it
            BMP.lBMPUpOrIconID = 0;
         }

         strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), szName );
      }
      else
      {
         // if ID exists, take it
         strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), ++pszBMPName );
         BMP.lBMPUpOrIconID = zatol( szName );
      }
   }
   else
   {
      BMP.szBMPUpOrIconName[ 0 ] = 0;
      BMP.lBMPUpOrIconID = 0;
   }

   k = BuildCtrlBOI_BMP( &BMP, szBlob, sizeof( szBlob ) );
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, k );

   // For the Common Detail stuff, we want to hold on to the low order bits,
   // and reset the high order bits according to the checkboxes on the
   // common detail window.  The individual control is concerned about
   // the low order bits and leaves the high order bits alone.
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   lSubtype &= 0xFFFF0000L;

   if ( GetCtrlState( vSubtask, "Fit", zCONTROL_STATUS_CHECKED ) > 0 )
      lSubtype |= 0x00000001;
   else
      lSubtype &= ~0x00000001;

   if ( GetCtrlState( vSubtask, "UseDDB", zCONTROL_STATUS_CHECKED ) > 0 )
      lSubtype |= 0x00000002;
   else
      lSubtype &= ~0x00000002;

   if ( GetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED ) > 0 )
      lSubtype |= 0x00001000;
   else
      lSubtype &= ~0x00001000;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpCtlBrowseFileName( zVIEW vSubtask )
{
   zCHAR   szFileName[ zMAX_FILENAME_LTH + 1 ];

   szFileName[ 0 ] = 0;
   if ( OperatorPromptForFile( vSubtask, szFileName, sizeof( szFileName ),
                               "Bitmap Files (*.BMP)|*.bmp|",
                               "BMP",
                               zOFN_HIDEREADONLY | zOFN_NOCHANGEDIR |
                               zOFN_FILEMUSTEXIST ) == 1 )
//                             "BMP", TRUE ) == 1 )
   {
      SetCtrlText( vSubtask, "FileName", szFileName );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

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
   zVIEW    vDialog;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   if ( GetViewByName( &vDialog, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vDialog );

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );

   if ( CheckExistenceOfEntity( vDialog,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialog, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
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
   RefreshCtrl( vSubtask, "Context" );
// RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListContextSelect( zVIEW vSubtask )
{
   MapCtrl( vSubtask, "Context" );
   RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Recursive function for moving ctrls covered by a group to be sub-ctrls
// of the group.
//
#define zGROUPBOX_CONTROL                   1090L

void
fnRecurseGroupCtrls( zVIEW vDialog )
{
   zVIEW  vCtrlDialog;
   zTZWDLGSO_Control_DEF ControlDef;
   zTZWDLGSO_Control_DEF GroupBoxDef;
   zLONG  lKey;
   zSHORT nRC;

   ControlDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
   ControlDef.CheckByte = 0xFE;
   GroupBoxDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
   GroupBoxDef.CheckByte = 0xFE;
   nRC = SetCursorFirstEntity( vDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lKey, vDialog, "ControlDef", "Key" );
      if ( lKey == zGROUPBOX_CONTROL )
      {
         CreateViewFromViewForTask( &vCtrlDialog, vDialog, 0 );

         // Get the structure of attributes for this group box.
         GetStructFromEntityAttrs( (zPCHAR) &GroupBoxDef, vDialog, "Control" );
         if ( GroupBoxDef.CheckByte != 0xFE )
            MessageBox( 0, "GroupBoxDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

         nRC = SetCursorFirstEntity( vCtrlDialog, "Control", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            // get the structure of attributes for this control
            GetStructFromEntityAttrs( (zPCHAR) &ControlDef, vCtrlDialog, "Control" );
            if ( ControlDef.CheckByte != 0xFE )
               MessageBox( 0, "ControlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

            if ( GroupBoxDef.PSDLG_X < ControlDef.PSDLG_X && GroupBoxDef.PSDLG_Y < ControlDef.PSDLG_Y &&
                 GroupBoxDef.PSDLG_X + GroupBoxDef.SZDLG_X > ControlDef.PSDLG_X + ControlDef.SZDLG_X &&
                 GroupBoxDef.PSDLG_Y + GroupBoxDef.SZDLG_Y > ControlDef.PSDLG_Y + ControlDef.SZDLG_Y )
            {
               ControlDef.PSDLG_X -= GroupBoxDef.PSDLG_X;
               ControlDef.PSDLG_Y -= GroupBoxDef.PSDLG_Y;
               SetAttributeFromInteger( vCtrlDialog, "Control", "PSDLG_X", ControlDef.PSDLG_X );
               SetAttributeFromInteger( vCtrlDialog, "Control", "PSDLG_Y", ControlDef.PSDLG_Y );
               nRC = MoveSubobject( vDialog, "CtrlCtrl", vCtrlDialog, "Control", zPOS_LAST, zREPOS_NEXT );
            }
            else
               nRC = SetCursorNextEntity( vCtrlDialog, "Control", 0 );
         }

         DropView( vCtrlDialog );
      }

      SetViewToSubobject( vDialog, "CtrlCtrl" );
      fnRecurseGroupCtrls( vDialog );
      ResetViewFromSubobject( vDialog );

      nRC = SetCursorNextEntity( vDialog, "Control", 0 );
   }
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShowCtrlList_SetState
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
ShowCtrlList_SetState( zVIEW  vSubtask,
                       zULONG nEnable )
{
   SetCtrlState( vSubtask, "Detail",  zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Mapping", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Context", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "Delete",  zCONTROL_STATUS_ENABLED, nEnable );

   EnableAction( vSubtask, "CtrlListAddMapEntity", (zSHORT) nEnable );
   EnableAction( vSubtask, "CtrlListDeleteMapEntity", (zSHORT) nEnable );

   return( 0 );
} // ShowCtrlList_SetState

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShowCtrlListForCurrentEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
ShowCtrlListForCurrentEntity( zVIEW vSubtask,
                              zVIEW vDialog )
{
   zVIEW  vPE;
   zVIEW  vControl;
   zLONG  lParentCnt;

   SetCtrlState( vSubtask, "CtrlDetail", zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "GroupSetDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "GroupDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "UnknownDetail", zCONTROL_STATUS_VISIBLE, 0 );

   CreateViewFromViewForTask( &vControl, vDialog, 0 );
   SetViewFromView( vControl, vDialog );
   SetNameForView( vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetNameForView( vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );

   // Build the TZPNEVWO object for the control events.
   fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 &&
        SetEntityCursor( vPE, "ControlDef", "Key", zPOS_FIRST | zQUAL_ENTITYATTR,
                         vControl, "ControlDef", "Key", 0, 0, 0 ) >= zCURSOR_SET )
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

   if ( lParentCnt )
      SetCtrlState( vSubtask, "ChangeType",
                    zCONTROL_STATUS_ENABLED, FALSE );
   else
      SetCtrlState( vSubtask, "ChangeType",
                    zCONTROL_STATUS_ENABLED, TRUE );

   ShowCtrlList_SetState( vSubtask, TRUE );

   RefreshCtrl( vSubtask, "Type" );
   RefreshCtrl( vSubtask, "Tag" );
   RefreshCtrl( vSubtask, "Text" );
   RefreshCtrl( vSubtask, "Top" );
   RefreshCtrl( vSubtask, "Left" );
   RefreshCtrl( vSubtask, "Height" );
   RefreshCtrl( vSubtask, "Width" );
   RefreshCtrl( vSubtask, "Mapping" );
   RefreshCtrl( vSubtask, "Context" );
   RefreshCtrl( vSubtask, "CSS_Class" );
   RefreshCtrl( vSubtask, "CSS_Font" );
   RefreshCtrl( vSubtask, "CSS_Position" );
   RefreshCtrl( vSubtask, "CSS_Size" );

   // Set up the Context
   CtrlContextMappingInit( vSubtask );

   return( 0 );

} // ShowCtrlListForCurrentEntity

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnChangeCtrlMapView( zVIEW vControl,
                     zVIEW vNewViewName,
                     zLONG lOriginalViewID )
{
   zBOOL  bInclude;
   zSHORT nRC;

   for ( nRC = SetCursorFirstEntity( vControl, "CtrlMap", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vControl, "CtrlMap", "" ) )
   {
      if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
      {
         bInclude = TRUE;
         if ( CompareAttributeToInteger( vNewViewName, "ViewObjRef", "ZKey",
                                         lOriginalViewID ) == 0 )
         {
            bInclude = FALSE;
         }
         else
         {
            ExcludeEntity( vControl, "CtrlMapView", zREPOS_NONE );
         }

         if ( bInclude )
         {
            IncludeSubobjectFromSubobject( vControl,  "CtrlMapView",
                                           vNewViewName, "ViewObjRef", zPOS_AFTER );
         }
      }
   }
}

zOPER_EXPORT zSHORT OPERATION
fnChangeMappingViewR( zVIEW vReport,
                      zVIEW vOldViewName,
                      zVIEW vNewViewName,
                      zBOOL bAllCtrlsAtLevel,
                      zBOOL bRecurse )
{
   zLONG  lOriginalViewID;
   zSHORT nRC;

   // Get Original View ID.
   GetIntegerFromAttribute( &lOriginalViewID, vOldViewName, "ViewObjRef", "ZKey" );

   // Loop through all Controls at this level.
   if ( bAllCtrlsAtLevel )
      nRC = SetCursorFirstEntity( vReport, "Control", "" );
   else
      nRC = CheckExistenceOfEntity( vReport, "Control" );

   while ( nRC >= zCURSOR_SET )
   {
      // Change mapping view of each view that is identified by original view ID.
      fnChangeCtrlMapView( vReport, vNewViewName, lOriginalViewID );

      // Process any recursive sub controls.
      if ( bRecurse && CheckExistenceOfEntity( vReport, "CtrlCtrl" ) == 0 )
      {
         SetViewToSubobject( vReport, "CtrlCtrl" );
         fnChangeMappingViewR( vReport, vOldViewName, vNewViewName, TRUE, TRUE );
         ResetViewFromSubobject( vReport );
      }

      if ( bAllCtrlsAtLevel )
         nRC = SetCursorNextEntity( vReport, "Control", "" );
      else
         break;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
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
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PainterSelectedCallback( zVIEW   vSubtask, // view to painter window
                         zVIEW   vReport,
                         zVIEW   vLOD,
                         zVIEW   vControl, // do not reposition this view
                         zPVOID  pCtrl,    // painter c++ pointer to ctrl
                         zLONG   lType,    // type of call
                         zPVOID  pvData,   // this data can get changed
                         zLONG   lFlags )
{
   zVIEW  vOldName;
   zVIEW  vNewName;
// zLONG  lRC;

   if ( MiGetUpdateForView( vReport ) == 0 )
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
         CreateViewFromView( &vOldName, vReport );
         if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
            GetIntegerFromAttribute( &lZKey, vControl, "CtrlMapView", "ZKey" );
         else
            lZKey = 0;

         CreateViewFromView( &vNewName, vReport );
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

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateTagInPlace( zVIEW vSubtask )
{
   zVIEW vReport;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   if ( vReport )
   {
      zCHAR szOptionTag[ 65 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateTagInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                            "PainterSelectedCallback",
                                            vReport, 0, 0x00000010,
                                            szOptionTag, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateTextInPlace( zVIEW vSubtask )
{
   zVIEW vReport;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   if ( vReport )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateTextInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                            "PainterSelectedCallback",
                                            vReport, 0, 0x00000020, 0, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateFontFaceNameInPlace( zVIEW vSubtask )
{
   zVIEW vReport;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   if ( vReport )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdatFontFaceNameInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                            "PainterSelectedCallback",
                                            vReport, 0, 0x00000040, 0, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UpdateFontSizeInPlace( zVIEW vSubtask )
{
   zVIEW vReport;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   if ( vReport )
   {
      zCHAR szOptionTag[ 33 ];
      zPCHAR pch;

      GetLastCommandTag( vSubtask, szOptionTag );
      TraceLineS( "UpdateFontSizeInPlace Command: ", szOptionTag );
      pch = zstrrchr( szOptionTag, '.' );
      if ( pch )
         *pch = 0;

      lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                            "PainterSelectedCallback",
                                            vReport, 0, 0x00000080, 0, 0 );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ChangeSelectedViewMapping( zVIEW vSubtask )
{
   zVIEW vReport;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vReport && vTZZOVEAO )
   {
      zCHAR szVEA[ 3 * 33 ];

      GetStringFromAttribute( szVEA, sizeof( szVEA ), vTZZOVEAO, "ViewObjRef", "Name" );

      lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd",
                                            "PainterSelectedCallback",
                                            vReport, 0, 1, szVEA, 0 );
   }

   return( (zSHORT) lRC );
}

zBOOL
fnGetCM_ListAndLOD( zVIEW vSubtask, zPVIEW pvCM_List, zPVIEW pvLOD,
                    zVIEW vTZZOVEAO, zPCHAR pchVEA, zLONG lMaxLth, zBOOL bAttr )
{
   zLONG  lZKey;
   zLONG  lZKeyA;
   zSHORT nLth;

   GetStringFromAttribute( pchVEA, lMaxLth, vTZZOVEAO, "ViewObjRef", "Name" );
   nLth = (zSHORT) zstrlen( pchVEA );
   pchVEA[ nLth++ ] = '.';
   GetStringFromAttribute( pchVEA + nLth, lMaxLth - nLth, vTZZOVEAO, "LOD_Entity", "Name" );
   if ( bAttr )
   {
      nLth = (zSHORT) zstrlen( pchVEA );
      pchVEA[ nLth++ ] = '.';
      GetStringFromAttribute( pchVEA + nLth, lMaxLth - nLth, vTZZOVEAO, "ER_Attribute", "Name" );
   }

   GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD", "ZKey" );
   RetrieveViewForMetaList( vSubtask,
                            pvCM_List, zREFER_LOD_META );
   SetCursorFirstEntityByInteger( *pvCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, pvLOD, *pvCM_List, zREFER_LOD_META, 0 );

   GetIntegerFromAttribute( &lZKey, vTZZOVEAO, "LOD_Entity", "ZKey" );
   if ( bAttr )
      GetIntegerFromAttribute( &lZKeyA, vTZZOVEAO, "LOD_Attribute", "ZKey" );

   if ( SetCursorFirstEntityByInteger( *pvLOD, "LOD_Entity", "ZKey", lZKey, "" ) == zCURSOR_SET &&
        (bAttr == FALSE ||
         SetCursorFirstEntityByInteger( *pvLOD, "LOD_Attribute", "ZKey", lZKeyA, "" ) == zCURSOR_SET) )
   {
      return( TRUE );
   }

   return( FALSE );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetSelectedEntityMapping( zVIEW vSubtask )
{
   zVIEW vReport;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vReport && vTZZOVEAO )
   {
      zVIEW vCM_List;
      zVIEW vLOD;
      zCHAR szVEA[ 3 * 33 ];

      if ( fnGetCM_ListAndLOD( vSubtask, &vCM_List, &vLOD, vTZZOVEAO, szVEA, sizeof( szVEA ), FALSE ) )
      {
         lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd", "PainterSelectedCallback",
                                               vReport, vLOD, 3, szVEA, 0 );
      }

      DropMetaOI( vSubtask, vLOD );
   }

   return( (zSHORT) lRC );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetSelectedAttributeMapping( zVIEW vSubtask )
{
   zVIEW vReport;
   zVIEW vTZZOVEAO;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   if ( vReport && vTZZOVEAO )
   {
      zVIEW vCM_List;
      zVIEW vLOD;
      zCHAR szVEA[ 3 * 33 ];

      if ( fnGetCM_ListAndLOD( vSubtask, &vCM_List, &vLOD, vTZZOVEAO, szVEA, sizeof( szVEA ), TRUE ) )
      {
         lRC = CallPainterForSelectedControls( vSubtask, "tzrpupdd", "PainterSelectedCallback",
                                               vReport, vLOD, 7, szVEA, 0 );
      }

      DropMetaOI( vSubtask, vLOD );
   }

   return( (zSHORT) lRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListChangeMappingViewGS
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListChangeMappingViewGS( zVIEW vSubtask )
{
   zVIEW  vPAGECTRL;
   zVIEW  vOldViewName;
   zVIEW  vNewViewName;
   zVIEW  vReport;
   zCHAR  szGroupSetName[ 33 ];
   zCHAR  szNewViewName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vPAGECTRL, "PAGECTRL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szGroupSetName, sizeof( szGroupSetName ), vPAGECTRL, "GroupSet", "Tag" );
   if ( SetCursorFirstEntityByString( vReport, "GroupSet", "Tag",
                                      szGroupSetName, "Report" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "Group Set Name could not be found in work vReport.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   CreateViewFromView( &vOldViewName, vReport );
   CreateViewFromView( &vNewViewName, vReport );
   GetCtrlText( vSubtask, "NewViewName", szNewViewName, 32 );
   if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                      szNewViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "New View Name is not a Registered View in this Dialog.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      DropView( vNewViewName );
      return( -1 );
   }

   // Loop through all Groups within the GroupSet.
   for ( nRC = SetCursorFirstEntity( vReport, "Group", "GroupSet" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReport, "Group", "GroupSet" ) )
   {
      // Go to execute change view mapping at each Control level.
      fnChangeMappingViewR( vReport, vOldViewName, vNewViewName, TRUE, TRUE );
   }

   DropView( vNewViewName );
   DropView( vOldViewName );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CtrlListChangeMappingViewRpt
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CtrlListChangeMappingViewRpt( zVIEW vSubtask )
{
// zVIEW  vPAGECTRL;
   zVIEW  vOldViewName;
   zVIEW  vNewViewName;
   zVIEW  vReport;
// zCHAR  szGroupSetName[ 33 ];
   zCHAR  szNewViewName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vReport, "TZWINDOW", vSubtask, zLEVEL_TASK );
   CreateViewFromView( &vOldViewName, vReport );
 //GetVariableFromAttribute( szGroupSetName, 0, 'S', 32,
 //                          vOldViewName, "Dialog", "wOriginalViewName", "", 0 );
 //if ( SetCursorFirstEntityByString( vOldViewName, "ViewObjRef", "Name",
 //                                   szOriginalViewName, 0 ) < zCURSOR_SET )
 //{
 //   MessageSend( vSubtask, "", "Change View Mapping",
 //                "Original View Name is not a Registered View in this Dialog.",
 //                zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
 //   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
 //   DropView( vOldViewName );
 //   return( -1 );
 //}
   CreateViewFromView( &vNewViewName, vReport );
   GetCtrlText( vSubtask, "NewViewName", szNewViewName, 32 );
   if ( SetCursorFirstEntityByString( vNewViewName, "ViewObjRef", "Name",
                                      szNewViewName, 0 ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "", "Change View Mapping",
                   "New View Name is not a Registered View in this Dialog.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView( vOldViewName );
      DropView( vNewViewName );
      return( -1 );
   }

   // Loop through all Groups within the Report.
   for ( nRC = SetCursorFirstEntity( vReport, "Group", "Report" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReport, "Group", "Report" ) )
   {
      // Go to execute change view mapping at each Control level.
      fnChangeMappingViewR( vReport, vOldViewName, vNewViewName, TRUE, TRUE );
   }

   DropView( vNewViewName );
   DropView( vOldViewName );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreateEntityGroupSet( zVIEW vSubtask, zBOOL bHorizontal )
{
   zVIEW vReport;
   zVIEW vTZZOVEAO;
   zVIEW vPE;
   zVIEW vTemp;
   zLONG lRC = -1;

   GetViewByName( &vReport, "vReport", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZZOVEAO, "TZZOVEAO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( vReport && vTZZOVEAO && vPE &&
        SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Rpt_Text", "" ) == 0 )
   {
      zVIEW vCM_List;
      zVIEW vLOD;
      zCHAR szVEA[ 3 * 33 ];

      if ( fnGetCM_ListAndLOD( vSubtask, &vCM_List, &vLOD, vTZZOVEAO, szVEA, sizeof( szVEA ), FALSE ) )
      {
         zVIEW  vReportHeader;
         zVIEW  vReportDetail;
         zCHAR  szViewName[ 34 ];
         zCHAR  szEntityName[ 34 ];
         zCHAR  szEntityTemp[ 68 ];
         zCHAR  szAttribName[ 34 ];
         zLONG  lPageSizeX;
         zLONG  lZKey;
         zLONG  lPosX;
         zLONG  lPosY;
         zLONG  lCnt;
         zSHORT nRC = zCURSOR_SET;
         zSHORT nLth = (zSHORT) zstrlen( szVEA );

         szVEA[ nLth++ ] = '.';

         GetStringFromAttribute( szViewName, sizeof( szViewName ), vTZZOVEAO, "ViewObjRef", "Name" );
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vTZZOVEAO, "LOD_Entity", "Name" );
         GetIntegerFromAttribute( &lPageSizeX, vReport, "Page", "SizeX" );
         if ( lPageSizeX == 0 )
            lPageSizeX = 2176;  // 8 1/2

         lCnt = zstrlen( szEntityName );
         TraceLineS( "=====> EntityName: ", szEntityName );
         TraceLineI( "=====> PageSizeX: ", lPageSizeX );

         strcpy_s( szEntityTemp, sizeof( szEntityTemp ), szEntityName );
         CreateViewFromViewForTask( &vTemp, vTZZOVEAO, vSubtask );

         while ( SetCursorFirstEntityByString( vReport, "GroupSet", "Tag",
                                               szEntityTemp, 0 ) != zCURSOR_SET )
         {
            nRC = SetCursorPrevEntity( vTemp, "LOD_Entity", 0 );
            if ( nRC != zCURSOR_SET )
               break;

            GetStringFromAttribute( szEntityTemp, sizeof( szEntityTemp ), vTemp, "LOD_Entity", "Name" );
         }

         strcpy_s( szEntityTemp, sizeof( szEntityTemp ), szEntityName );
         SetViewFromView( vTemp, vTZZOVEAO );
         if ( nRC != zCURSOR_SET )
            SetCursorFirstEntity( vReport, "GroupSet", 0 );

         CreateMetaEntity( vSubtask, vReport, "GroupSet", zPOS_AFTER );
         SetAttributeFromString( vReport, "GroupSet", "Tag", szEntityName );
         SetAttributeFromString( vReport, "GroupSet", "Type", "E" );
         SetAttributeFromInteger( vReport, "GroupSet", "PSDLG_X", 0 );
         SetAttributeFromInteger( vReport, "GroupSet", "PSDLG_Y", 64 );
         SetAttributeFromInteger( vReport, "GroupSet", "SZDLG_X", lPageSizeX );
         SetAttributeFromInteger( vReport, "GroupSet", "SZDLG_Y", 320 );
         SetAttributeFromInteger( vReport, "GroupSet", "PPE_Type", 3000 );

         strcpy_s( szEntityTemp + lCnt, sizeof( szEntityTemp ) - lCnt, "Header" );
         CreateMetaEntity( vSubtask, vReport, "Group", zPOS_AFTER );
         CreateViewFromView( &vReportHeader, vReport );
         SetAttributeFromString( vReport, "Group", "Tag", szEntityTemp );
         SetAttributeFromString( vReport, "Group", "Type", "gh" );
         SetAttributeFromString( vReport, "Group", "SubType", "0" );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_X", 0 );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_Y", 5 );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_X", lPageSizeX );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_Y", 200 );
         SetAttributeFromInteger( vReport, "Group", "PPE_Type", 3010 );

         strcpy_s( szEntityTemp + lCnt, sizeof( szEntityTemp ) - lCnt, "Detail" );
         CreateMetaEntity( vSubtask, vReport, "Group", zPOS_AFTER );
         CreateViewFromView( &vReportDetail, vReport );
         SetAttributeFromString( vReport, "Group", "Tag", szEntityTemp );
         SetAttributeFromString( vReport, "Group", "Type", "ga" );
         SetAttributeFromString( vReport, "Group", "SubType", "0" );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_X", 0 );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_Y", 225 );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_X", lPageSizeX );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_Y", 300 );
         SetAttributeFromInteger( vReport, "Group", "PPE_Type", 3010 );

         if ( bHorizontal == FALSE )
            SetViewFromView( vReportHeader, vReportDetail );

         // Create detail attributes in the group header and detail.
         lPosX = 50;
         lPosY = 50;
         nRC = SetCursorFirstEntity( vTemp, "LOD_Attribute", 0 );
         while ( nRC == zCURSOR_SET )
         {
            GetStringFromAttribute( szAttribName, sizeof( szAttribName ), vTemp, "ER_Attribute", "Name" );
            strcpy_s( szVEA + nLth, sizeof( szVEA ) - nLth, szAttribName );
            TraceLineS( "CreateEntityGroupSet creating control for attribute: ", szVEA );

            CreateMetaEntity( vSubtask, vReportHeader, "Control", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vReportHeader, "ControlDef",
                                           vPE, "ControlDef", zPOS_AFTER );

            CreateMetaEntity( vSubtask, vReportDetail, "Control", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vReportDetail, "ControlDef",
                                           vPE, "ControlDef", zPOS_AFTER );

            SetAttributeFromString( vReportDetail, "Control", "Tag", szAttribName );
            SetAttributeFromString( vReportDetail, "Control", "Text", szAttribName );
            if ( bHorizontal )
               SetAttributeFromString( vReportHeader, "Control", "Text", szAttribName );

            strcat_s( szAttribName, sizeof( szAttribName ), ":" );  // to differentiate tags
            if ( bHorizontal == FALSE )
               SetAttributeFromString( vReportHeader, "Control", "Text", szAttribName );

            SetAttributeFromString( vReportHeader, "Control", "Tag", szAttribName );

            SetAttributeFromInteger( vReportHeader, "Control", "PSDLG_X", lPosX );
            SetAttributeFromInteger( vReportHeader, "Control", "PSDLG_Y", lPosY );
            SetAttributeFromInteger( vReportHeader, "Control", "SZDLG_X", 200 );
            SetAttributeFromInteger( vReportHeader, "Control", "SZDLG_Y", 50 );

            if ( bHorizontal )
               SetAttributeFromInteger( vReportDetail, "Control", "PSDLG_X", lPosX );
            else
               SetAttributeFromInteger( vReportDetail, "Control", "PSDLG_X", lPosX + 220 );

            SetAttributeFromInteger( vReportDetail, "Control", "PSDLG_Y", lPosY );
            SetAttributeFromInteger( vReportDetail, "Control", "SZDLG_X", 200 );
            SetAttributeFromInteger( vReportDetail, "Control", "SZDLG_Y", 50 );

            // Create mapping.
            GetIntegerFromAttribute( &lZKey, vTemp, "LOD_Attribute", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute", "ZKey",
                                                lZKey, "" ) == zCURSOR_SET )
            {
               CreateMetaEntity( vSubtask, vReportDetail, "CtrlMap", zPOS_AFTER );
               SetAttributeFromString( vReportDetail, "CtrlMap", "Tag", szViewName );
               PainterSelectedCallback( vSubtask, // view to painter window
                                        vReport,
                                        vLOD,
                                        vReportDetail, // do not reposition this view
                                        0,             // painter c++ pointer to ctrl
                                        0x00000007,    // type of call
                                        szVEA,         // this data can get changed
                                        0 );
            }

            if ( bHorizontal )
            {
               lPosX += 210;
               if ( lPosX > lPageSizeX )
               {
                  lPosX = 50;
                  lPosY += 55;
               }
            }
            else
            {
               lPosY += 55;
            }

            nRC = SetCursorNextEntity( vTemp, "LOD_Attribute", 0 );
         }

         lPosY += 100;
         SetAttributeFromInteger( vReport, "Group", "SZDLG_Y", lPosY );

         strcpy_s( szEntityTemp + lCnt, sizeof( szEntityTemp ) - lCnt, "Footer" );
         CreateMetaEntity( vSubtask, vReport, "Group", zPOS_AFTER );
         SetAttributeFromString( vReport, "Group", "Tag", szEntityTemp );
         SetAttributeFromString( vReport, "Group", "Type", "gf" );
         SetAttributeFromString( vReport, "Group", "SubType", "0" );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_X", 0 );
         SetAttributeFromInteger( vReport, "Group", "PSDLG_Y", lPosY + 50 );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_X", lPageSizeX );
         SetAttributeFromInteger( vReport, "Group", "SZDLG_Y", 200 );
         SetAttributeFromInteger( vReport, "Group", "PPE_Type", 3010 );

         DropView( vReportHeader );
         DropView( vReportDetail );
         DropView( vTemp );

         fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, "", 0 );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreateEntityGroupSetHorizontal( zVIEW vSubtask )
{
   return( CreateEntityGroupSet( vSubtask, TRUE ) );
}

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CreateEntityGroupSetVertical( zVIEW vSubtask )
{
   return( CreateEntityGroupSet( vSubtask, FALSE ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShowCtrlListForCurrentGroupSet
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ShowCtrlListForCurrentGroupSet( zVIEW vSubtask )
{
   zVIEW   vReport;
   zLONG   lFlags;

   SetCtrlState( vSubtask, "GroupSetDetail", zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "CtrlDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "GroupDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "UnknownDetail", zCONTROL_STATUS_VISIBLE, 0 );

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lFlags, vReport, "GroupSet", "Flags" );
   SetCtrlState( vSubtask, "NewspaperColumn",
                    zCONTROL_STATUS_CHECKED, lFlags ? TRUE : FALSE );

   RefreshCtrl( vSubtask, "GroupSetTag" );
   RefreshCtrl( vSubtask, "GroupSetType" );
   RefreshCtrl( vSubtask, "RepeatCnt" );
   RefreshCtrl( vSubtask, "Indent" );
   RefreshCtrl( vSubtask, "Margin" );

   return( 0 );
} // ShowCtrlListForCurrentGroupSet

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShowCtrlListForCurrentGroup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ShowCtrlListForCurrentGroup( zVIEW vSubtask )
{
   UPD_GROUP_PostBuild( vSubtask );

   SetCtrlState( vSubtask, "GroupDetail", zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "CtrlDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "GroupSetDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "UnknownDetail", zCONTROL_STATUS_VISIBLE, 0 );

   RefreshCtrl( vSubtask, "GroupTag" );
   RefreshCtrl( vSubtask, "GroupType" );
   RefreshCtrl( vSubtask, "Subtype" );
   RefreshCtrl( vSubtask, "RepeatHeader" );
   RefreshCtrl( vSubtask, "PageBreak" );
   RefreshCtrl( vSubtask, "ForcePageBreak" );

   return( 0 );
} // ShowCtrlListForCurrentGroup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ShowCtrlListForUnknownEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
ShowCtrlListForUnknownEntity( zVIEW  vSubtask,
                              zPCHAR szEntityName )
{
   zCHAR szTitle[ 50 ];

   strcpy_s( szTitle, sizeof( szTitle ), " " );
   strcat_s( szTitle, sizeof( szTitle ), szEntityName );
   strcat_s( szTitle, sizeof( szTitle ), " Detail" );

   SetCtrlText( vSubtask, "txtTitle", szTitle );

   SetCtrlState( vSubtask, "UnknownDetail", zCONTROL_STATUS_VISIBLE, 1 );
   SetCtrlState( vSubtask, "CtrlDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "GroupSetDetail", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "GroupDetail", zCONTROL_STATUS_VISIBLE, 0 );

   return( 0 );
} // ShowCtrlListForUnknownEntity

zOPER_EXPORT zSHORT OPERATION
ShowCtrlList( zVIEW vSubtask )
{
   zVIEW vDialog;
   zVIEW vControl;
   zCHAR  szEntityName[ 33 ];

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vControl );

   if ( GetViewByName( &vControl, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vControl );

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      SetNameForView( vDialog, "PAGECTRL", vSubtask, zLEVEL_TASK );
      SetNameForView( vDialog, "NoRefresh", vSubtask, zLEVEL_TASK );
   }

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vDialog, szEntityName ) >= 0 )
   {
      // show Control Detail
      if ( zstrcmp( szEntityName, "Control" ) == 0 )
      {
         ShowCtrlListForCurrentEntity( vSubtask, vDialog );
      }
      else
      {
         // Show GroupSet or Group Detail
         if ( zstrcmp( szEntityName, "GroupSet" ) == 0 )
            ShowCtrlListForCurrentGroupSet( vSubtask );
         else
         if ( zstrcmp( szEntityName, "Group" ) == 0 )
            ShowCtrlListForCurrentGroup( vSubtask );
         else
            ShowCtrlListForUnknownEntity( vSubtask, szEntityName );

         if ( GetViewByName( &vControl, "TZPNEVWO", vSubtask, zLEVEL_TASK ) >= 0 )
            DropView( vControl );

         RefreshCtrl( vSubtask, "Mapping" );
         RefreshCtrl( vSubtask, "Context" );

         SetCtrlState( vSubtask, "ChangeType", zCONTROL_STATUS_ENABLED, FALSE );
         ShowCtrlList_SetState( vSubtask, FALSE );

         if ( GetViewByName( &vControl, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vControl );
      }
   }

   // if Report not checked out, set  View read only -> the user cannot update the values in Detail Windows
   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   if ( !ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_REPORT_META ))
   {
      SetCtrlState( vSubtask, "ChangeType", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Context", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Detail", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SaveCtrlList_CheckControlValues
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
SaveCtrlList_CheckControlValues( zVIEW vSubtask )
{
   zVIEW vControl;
   zBOOL  bError = TRUE;

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      if ( MapCtrl( vSubtask, "Tag" )    != 0  ||
           MapCtrl( vSubtask, "Text" )   != 0  ||
           MapCtrl( vSubtask, "Top" )    != 0  ||
           MapCtrl( vSubtask, "Left" )   != 0  ||
           MapCtrl( vSubtask, "Height" ) != 0  ||
           MapCtrl( vSubtask, "Width" )  != 0  ||
           MapCtrl( vSubtask, "CSS_Class" )    != 0 ||
           MapCtrl( vSubtask, "CSS_Font" )     != 0 ||
           MapCtrl( vSubtask, "CSS_Position" ) != 0 ||
           MapCtrl( vSubtask, "CSS_Size" )     != 0 )
      {
         TraceLineS( "SaveCtrlList Mapping Error", "" );
         return( -1 );
      }
      else
      {
         zCHAR szTag[ 33 ];

         if ( GetCtrlText( vSubtask, "Tag", szTag, sizeof( szTag ) ) == 0 )
            OL_SetTextForCurrentItem( vSubtask, "CtrlList", szTag );
      }
   }

   return( 0 );

} // SaveCtrlList_CheckControlValues

zOPER_EXPORT zSHORT OPERATION
SaveCtrlList( zVIEW vSubtask )
{
   zVIEW   vReport;
   zSHORT  nRC = 0;
   zCHAR   szEntityName[ 33 ];

   GetViewByName( &vReport, "PAGECTRL", vSubtask, zLEVEL_TASK );

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vReport, szEntityName ) >= 0 )
   {
      if ( zstrcmp( szEntityName, "Control" ) == 0 )
         nRC = SaveCtrlList_CheckControlValues( vSubtask );
      else
      if ( zstrcmp( szEntityName, "GroupSet" ) == 0 )
         nRC = UPD_GROUPSET_CheckValues( vSubtask );
      else
      if ( zstrcmp( szEntityName, "Group" ) == 0 )
      {
         nRC = UPD_GROUP_OK( vSubtask );
         if ( nRC >= 0 )
         {
            zCHAR szTag[ 33 ];

            if ( GetCtrlText( vSubtask, "GroupTag", szTag, sizeof( szTag ) ) == 0 )
               OL_SetTextForCurrentItem( vSubtask, "CtrlList", szTag );
         }
      }

      if ( nRC < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
         return( nRC );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListMapping( zVIEW vSubtask )
{
   zVIEW vDialog;
   zCHAR  szEntityName[ 33 ];

// TraceLineS( "CtrlListMapping", "" );
   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vDialog, szEntityName ) >= 0 &&
        zstrcmp( szEntityName, "Control" ) == 0 )
   {
      while ( ResetViewFromSubobject( vDialog ) == 0 )
      {
      }

      GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
      if ( vDialog )
         SetNameForView( vDialog, "NoRefresh", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ReturnFromCtrlList( zVIEW vSubtask )
{
   zVIEW  vDialog;

   if ( SaveCtrlList( vSubtask ) < 0 )
      return( -1 );

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
   {
   // TraceLineS( "ReturnFromCtrlList ResetViewFromSubobject", "" );
      DropNameForView( vDialog, "NoRefresh", vSubtask, zLEVEL_TASK );
      while ( ResetViewFromSubobject( vDialog ) == 0 )
      {
      }
   }

   if ( GetViewByName( &vDialog, "MapDomain", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vDialog );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ReturnToCtrlList( zVIEW vSubtask )
{
   zVIEW  vDialog;

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      while ( ResetViewFromSubobject( vDialog ) == 0 )
      {
      }
   }

   ShowCtrlList( vSubtask );
   return( 1 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "CtrlListPopup", -1, 0, FALSE, FALSE );
   return( 0 );
}

//./ ADD NAME=PopupMaintenance
// Source Module=tzpntrad.c
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
           SetEntityCursor( vPE, "ControlDef", "ZKey",
                            zPOS_FIRST | zQUAL_ENTITYATTR,
                            vControl, "ControlDef", "ZKey",
                            0, 0, 0 ) >= zCURSOR_SET )
      {
         zLONG lParentCnt = CountEntitiesForView( vPE, "ValidParent" );

         if ( lParentCnt )
            CreateTrackingPopupMenu( vSubtask, "CtrlDetail",
                                     -1, 0, FALSE, FALSE );
         else
            CreateTrackingPopupMenu( vSubtask, "CtrlMaintenance",
                                     -1, 0, FALSE, FALSE );

         return( 0 );
      }
   }

   CreateTrackingPopupMenu( vSubtask, "Unknown", -1, 0, FALSE, FALSE );
   return( 0 );
}

//./ ADD NAME=ChangeCtrlType
// Source Module=tzpntrad.c
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
   zVIEW  vDialog;
   zVIEW  vControl;
   zVIEW  vPE;
   zCHAR  szEntityName[ 33 ];
   zSHORT nRC;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "ChangeCtrlType unable to get PE view", "" );
      return( -1 );
   }

// nRC = SetCursorFirstEntityByEntityCsr( vPE, "ControlDef", vPE, "ConvertableFrom", "" );
// fnPainterCall( zMSG_CHANGESELECTEDCONTROLS, vSubtask, 0, 0, 0 );


   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   if ( vDialog )
      SetNameForView( vDialog, "NoRefresh", vSubtask, zLEVEL_TASK );

   if ( OL_GetCurrentEntityName( vSubtask, "CtrlList",
                                 &vDialog, szEntityName ) >= 0 )
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
      SetNameForView( m_vDialog, szlTZCONTROL, vSubtask, zLEVEL_TASK );
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
               if ( SetCursorFirstEntity( m_vDialog, "ControlDef", 0 )
                                                          == zCURSOR_SET )
               {
                  ExcludeEntity( m_vDialog, "ControlDef", zREPOS_NONE );
               }

               SetCursorFirstEntityByInteger( vPE, "ControlDef",
                                              "Key", lNewType, 0 );
               IncludeSubobjectFromSubobject( m_vDialog, "ControlDef",
                                              vPE, "ControlDef",
                                              zPOS_BEFORE );
               SendCtrlMsg( zCTRLMSG_CHANGE_TYPE, 0,
                            lNewType, lType, 0, 0, pCtrl );
               RepaintZeidonCtrl( );
            }
         }

      }  while ( ResetViewFromSubobject( m_vDialog ) == 0 );
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

      GetStringFromAttribute( szTag, sizeof( szTag ), vPE, "ControlDef", "Tag" );
      GetStringFromAttribute( szAcceptsAllParents, sizeof( szAcceptsAllParents ), vPE,
                              "ControlDef", "AcceptsAllParents" );
      GetStringFromAttribute( szAcceptsAllChildren, sizeof( szAcceptsAllChildren ), vPE,
                              "ControlDef", "AcceptsAllChildren" );
      GetStringFromAttribute( szRequiresParent, sizeof( szRequiresParent ), vPE,
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
            GetStringFromAttribute( szCurrTag, sizeof( szCurrTag ), vPE, "ControlDef", "Tag" );
            GetStringFromAttribute( szCurrAcceptsAllParents, sizeof( szCurrAcceptsAllParents ), vPE,
                                    "ControlDef", "AcceptsAllParents" );
            GetStringFromAttribute( szCurrAcceptsAllChildren, sizeof( szCurrAcceptsAllChildren ), vPE,
                                    "ControlDef", "AcceptsAllChildren" );
            GetStringFromAttribute( szCurrRequiresParent, sizeof( szCurrRequiresParent ), vPE,
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

      GetStringFromAttribute( szTag, sizeof( szTag ), vPE, "ControlDef", "Tag" );

      nPrevSelSet = SetSelectSetForView( vPE, 4 );
      SetCursorFirstSelectedEntity( vPE, "ControlDef", 0 );
      GetStringFromAttribute( szNewTag, sizeof( szNewTag ), vPE, "ControlDef", "Tag" );
      SetSelectSetForView( vPE, nPrevSelSet );

      sprintf_s( szMsg, sizeof( szMsg ),
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

   GetViewByName( &vControl, "TZWINDOW", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vControl ) == 0 )
   {
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CtrlListDetail( zVIEW vSubtask )
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

   GetViewByName( &vControl, "TZWINDOW", vSubtask, zLEVEL_TASK );
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
      zPCHAR pchDialog;
      zPCHAR pchWindow;

      GetAddrForAttribute( &pchDialog, vPE,
                           "ControlDef", "MaintenanceDialog" );
      GetAddrForAttribute( &pchWindow, vPE,
                           "ControlDef", "MaintenanceWindow" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               pchDialog, pchWindow );
   }

   return( 0 );
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
   SetAttributeFromString( vControl, "CtrlMap", "Tag", "New" );
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
   zCHAR  szControlTag[ 34 ];
   zCHAR  szMsg[ 100 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( vControl )
   {
      GetStringFromAttribute( szControlTag, sizeof( szControlTag ), vControl, "Control", "Tag" );
      strcpy_s( szMsg, sizeof( szMsg ), "OK to delete Control '" );
      strcat_s( szMsg, sizeof( szMsg ), szControlTag );
      strcat_s( szMsg, sizeof( szMsg ), "'?" );
      if ( MessagePrompt( vSubtask, "PE002",
                          "Dialog Maintenance",
                          szMsg, zBEEP, zBUTTONS_YESNO,
                          zRESPONSE_NO,  0 ) == zRESPONSE_YES )
      {
         DeleteEntity( vControl, "Control", zREPOS_AFTER );
         GetViewByName( &vControl, "TZWINDOW", vSubtask, zLEVEL_TASK );
         while ( ResetViewFromSubobject( vControl ) == 0 )
         {
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

static zSHORT
InitMenuDisableAll( zVIEW vSubtask )
{
   zSHORT bEnable = TRUE;
   zVIEW  vLPLR;

   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Copy", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteControls", zOPTION_STATUS_ENABLED, 0 );

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

   SetOptionState( vSubtask, "AutodesignReport", zOPTION_STATUS_ENABLED, 0 );

   // if a Error situation, disable all menu items without switch Project
   if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
      bEnable = FALSE;

   SetOptionState( vSubtask, "NewFile", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Open", zOPTION_STATUS_ENABLED, bEnable );
   SetOptionState( vSubtask, "Dialog Spec", zOPTION_STATUS_ENABLED, bEnable );


   return( 0 );
} // InitMenuDisableAll

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
InitMenu( zVIEW vSubtask )
{
   zCHAR    szLabel[ 64 ];
   zVIEW    vPAGEL;
   zSHORT   nIsCheckedOut;
   zSHORT   nEnable;

   zBOOL   bVisible;

   InitMenuDisableAll( vSubtask );

   if ( GetViewByName( &vPAGEL, "PAGEL", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   SetDrawMenuBar( vSubtask, FALSE );

   bVisible = (zBOOL) GetCtrlState( vSubtask, "ControlBar",
                                    zCONTROL_STATUS_VISIBLE );
   SetOptionState( vSubtask, "ShowHideControlBar", zOPTION_STATUS_TEXT,
                   (zULONG) (bVisible ? "Hide Control Bar" : "Show Control Bar") );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vPAGEL, zSOURCE_REPORT_META );

   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "AutodesignReport", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( fnPainterCall( zMSG_UNDO, vSubtask, 0, szLabel, sizeof( szLabel ) ) == 0 )
   {
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_TEXT, (zULONG) szLabel );
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 1 );
   }
   else
   {
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_TEXT, (zULONG) "&Undo\tCtrl+Z" );
      SetOptionState( vSubtask, "Undo", zOPTION_STATUS_ENABLED, 0 );
   }

   if ( fnPainterCall( zMSG_REDO, vSubtask, 0, szLabel, sizeof( szLabel ) ) == 0 )
   {
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_TEXT, (zULONG) szLabel );
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 1 );
   }
   else
   {
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_TEXT, (zULONG) "&Redo\tCtrl+Y" );
      SetOptionState( vSubtask, "Redo", zOPTION_STATUS_ENABLED, 0 );
   }

   if ( fnPainterCall( zMSG_CLIPBOARD_COPY, vSubtask, 0, szLabel, sizeof( szLabel ) ) == 0 )
      nEnable = 1;
   else
      nEnable = 0;

   SetOptionState( vSubtask, "Copy", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "DeleteControls", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AlignLeft", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AlignRight", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AlignTop", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AlignBottom", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "EqualHeight", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "EqualWidth", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "EqualWidthHeight", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "EquiSpaceVertical", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "EquiSpaceHorizontal", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AbutVertical", zOPTION_STATUS_ENABLED, nEnable );
   SetOptionState( vSubtask, "AbutHorizontal", zOPTION_STATUS_ENABLED, nEnable );

   if ( fnPainterCall( zMSG_CLIPBOARD_PASTE, vSubtask, 0, szLabel, sizeof( szLabel ) ) == 0 )
   {
      SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 1 );
   }
   else
   {
      SetOptionState( vSubtask, "Paste", zOPTION_STATUS_ENABLED, 0 );
   }

   SetDrawMenuBar( vSubtask, TRUE );
   return( 0 );
}

zCPCHAR cpcOptionTags[ ] = { "Tag", "Text", "FontFaceName", "FontSize" };

zOPER_EXPORT zSHORT OPERATION
InitPopup( zVIEW vSubtask )
{
// zCHAR  szUnknown[ 33 ];
   zCHAR  szCurrentCtrl[ 33 ];
   zCHAR  szOptionTag[ 65 ];
   zCHAR  szActionTag[ 65 ];
   zLONG  lFlag;
   zSHORT nPos = -1;
   zSHORT k;
   zBOOL  bFirstTime = TRUE;

   SetDrawMenuBar( vSubtask, FALSE );

// strcpy_s( szUnknown, sizeof( szUnknown ), "__Unknown" );
   szCurrentCtrl[ 0 ] = 0;
   while ( (nPos = GetNextCtrlAtPoint( vSubtask, nPos, szCurrentCtrl, 1 )) >= 0 )
   {
      for ( k = 0; k < mElementCnt( cpcOptionTags ); k++ )
      {
         if ( bFirstTime )
         {
         // zstrcpy( szUnknown + 9, cpcOptionTags[ k ] );
         // RemoveOptionFromPopupMenu( vSubtask, szUnknown );
            lFlag = 4 + 128;
         }
         else
            lFlag = 4;

         strcpy_s( szOptionTag, sizeof( szOptionTag ), szCurrentCtrl );
         strcat_s( szOptionTag, sizeof( szOptionTag ), "." );
         strcat_s( szOptionTag, sizeof( szOptionTag ), cpcOptionTags[ k ] );
         strcpy_s( szActionTag, sizeof( szActionTag ), "Update" );
         strcat_s( szActionTag, sizeof( szActionTag ), cpcOptionTags[ k ] );
         strcat_s( szActionTag, sizeof( szActionTag ), "InPlace" );
         AddOptionToPopupMenu( vSubtask, cpcOptionTags[ k ], 0, szOptionTag,
                               szCurrentCtrl, szActionTag, lFlag );
      }

      bFirstTime = FALSE;
   }

   SetDrawMenuBar( vSubtask, TRUE );
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

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zCHECK_TIMESTAMP_ONLY          0x10000000
#define zCHECK_POSITION_ONLY           0x20000000

zBOOL
CheckMigrateCtrls( zVIEW vReport, zVIEW vPE, zLONG lCheckFlags )
{
   zBOOL  bChange = FALSE;
   zVIEW  vWork = 0;
   zLONG  lKey;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zLONG  lPosX1;
   zLONG  lPosY1;
   zLONG  lSizeX1;
   zLONG  lSizeY1;
   zSHORT nRC;

   if ( lCheckFlags & zCHECK_POSITION_ONLY )
   {
      CreateViewFromViewForTask( &vWork, vReport, 0 );
      SetViewFromView( vWork, vReport );
   }

   nRC = SetCursorFirstEntity( vReport, "Control", 0 );
   while ( nRC == zCURSOR_SET )
   {
      if ( lCheckFlags & zCHECK_POSITION_ONLY )
      {
         lKey = GetEntityKey( vReport, "Control" );
         GetIntegerFromAttribute( &lPosX, vReport, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &lPosY, vReport, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lSizeX, vReport, "Control", "SZDLG_X" );
         GetIntegerFromAttribute( &lSizeY, vReport, "Control", "SZDLG_Y" );

         nRC = SetCursorFirstEntity( vWork, "Control", 0 );
         while ( nRC == zCURSOR_SET )
         {
            if ( lKey != (zLONG) GetEntityKey( vWork, "Control" ) )
            {
               GetIntegerFromAttribute( &lPosX1, vWork, "Control", "PSDLG_X" );
               GetIntegerFromAttribute( &lPosY1, vWork, "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lSizeX1, vWork, "Control", "SZDLG_X" );
               GetIntegerFromAttribute( &lSizeY1, vWork, "Control", "SZDLG_Y" );

               if ( lPosX1 >= lPosX && lPosY1 >= lPosY &&
                    lPosX1 + lSizeX1 <= lPosX + lSizeX &&
                    lPosY1 + lSizeY1 <= lPosY + lSizeY )
               {
                  // We have found containment of "vWork" control by "vReport" control.
                  // Move the work control under the report control.

                  // but first adjust the position
                  SetAttributeFromInteger( vWork, "Control",
                                           "PSDLG_X", lPosX1 - lPosX );
                  SetAttributeFromInteger( vWork, "Control",
                                           "PSDLG_Y", lPosY1 - lPosY );
                  SetViewToSubobject( vReport, "CtrlCtrl" );
                  MoveSubobject( vReport, "Control",
                                 vWork, "Control", zPOS_AFTER, zREPOS_NEXT );
                  ResetViewFromSubobject( vReport );
                  nRC = SetCursorFirstEntity( vWork, "Control", 0 );
                  bChange = TRUE;
                  continue;
               }
            }

            nRC = SetCursorNextEntity( vWork, "Control", 0 );
         }
      }
      else
      {
         GetIntegerFromAttribute( &lKey, vReport, "ControlDef", "ZKey" );
         if ( CompareAttributeToInteger( vReport, "ControlDef", "Key", lKey ) != 0 )
         {
            SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key", lKey, 0 );
            ExcludeEntity( vReport, "ControlDef", zREPOS_NONE );
            IncludeSubobjectFromSubobject( vReport, "ControlDef", vPE, "ControlDef", zPOS_BEFORE );

            bChange = TRUE;
         }
      }

      SetViewToSubobject( vReport, "CtrlCtrl" );
      if ( CheckMigrateCtrls( vReport, vPE, lCheckFlags ) )
         bChange = TRUE;

      ResetViewFromSubobject( vReport );

      nRC = SetCursorNextEntity( vReport, "Control", 0 );
   }

   if ( lCheckFlags & zCHECK_POSITION_ONLY )
      DropView( vWork );


   return( bChange );
}

// z11001Zeidon    TEMP     TZRPSRCO 06/04/00   15:50:27 1.0a
// z10000Zeidon    ACCOUNTR TZRPSRCO 06/04/00   15:50:27 1.0a

zSHORT
fnLocateReportDate( zPCHAR pchBuffer )
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
            if ( pchSlash1 && pchSlash1 + 3 == pchSlash2 && pchColon1 + 3 == pchColon2 )
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

               zstrcpy( pchBuffer + 2, pchSlash2 + 1 );
               pchBuffer[ 4 ] = '/';
               zstrcpy( pchBuffer + 5, pchSlash1 - 2 );
               pchBuffer[ 10 ] = ' ';
               zstrcpy( pchBuffer + 11, pchColon1 - 2 );
               return( 0 );   // we think we did this correctly
            }
         }
      }
   }

   return( -1 );   // there was some sort of problem
}

zOPER_EXPORT zSHORT OPERATION
MigrateReports( zVIEW vSubtask )
{
   zPCHAR pch;
   zVIEW  vReportList;
   zVIEW  vReport;
   zVIEW  vPE;
   zBOOL  bChange;
   zSHORT nRC;
   zLONG  lCheckFlags = 0;
   zBOOL  bSaveAll = FALSE;
   zLONG  k = 0;
   zVIEW  vTaskLPLR = 0;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      MessageSend( vSubtask, "PN00201", "Report",
                   "Presentation Environment does not exist!\n"
                   "Cannot migrate Reports.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   if ( MessageBox( 0, "Do you want to reset child controls\n"
                       "based on position?",
                    "Dialog Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_POSITION_ONLY;
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) <= 0 )
      {
         MessageSend( vSubtask, "PN00202", "Report Maintenance",
                      "Unable to open Task LPLR!\n"
                      "Cannot migrate Reports.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }
   else
   if ( MessageBox( 0, "Do you want to save\n"
                       "Reports based on Date only?",
                    "Dialog Migration", MB_YESNO ) == IDYES )
   {
      lCheckFlags |= zCHECK_TIMESTAMP_ONLY;
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) <= 0 )
      {
         MessageSend( vSubtask, "PN00202", "Report Maintenance",
                      "Unable to open Task LPLR!\n"
                      "Cannot migrate Reports.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( 0 );
      }
   }
   else
   if ( MessageBox( 0, "Do you want to save all Reports?",
                    "Report Migration", MB_YESNO ) == IDYES )
   {
      bSaveAll = TRUE;
      if ( lCheckFlags == 0 )
      {
         if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) <= 0 )
         {
            MessageSend( vSubtask, "PN00202", "Report Maintenance",
                         "Unable to open Task LPLR!\n"
                         "Cannot migrate Reports.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
             return( 0 );
         }
      }
   }

   if ( lCheckFlags == 0 && bSaveAll == FALSE )
      return( 0 );

   RetrieveViewForMetaList( vSubtask, &vReportList, zREFER_REPORT_META );
   nRC = SetCursorFirstEntity( vReportList, "W_MetaDef", 0 );
   while ( nRC == zCURSOR_SET )
   {
      k++;
      if ( ActivateMetaOI( vSubtask, &vReport, vReportList, zSOURCE_REPORT_META, zSINGLE | zLEVEL_TASK ) == 1 )
      {
         zCHAR szMsg[ 128 ];

         SetNameForView( vReport, "__CheckReport", vSubtask, zLEVEL_TASK );
         GetAddrForAttribute( &pch, vReport, "Report", "Tag" );
         bChange = bSaveAll;
         if ( lCheckFlags &&
              ((lCheckFlags & zCHECK_TIMESTAMP_ONLY) == 0 ||
               (lCheckFlags & zCHECK_POSITION_ONLY) != 0) )
         {
            sprintf_s( szMsg, sizeof( szMsg ), "Checking Report: %s for Migration", pch );
            TraceLineS( szMsg, "" );
            MB_SetMessage( vSubtask, 1, szMsg );
            nRC = SetCursorFirstEntity( vReport, "Page", 0 );
            while ( nRC == zCURSOR_SET )
            {
               nRC = SetCursorFirstEntity( vReport, "GroupSet", 0 );
               while ( nRC == zCURSOR_SET )
               {
                  nRC = SetCursorFirstEntity( vReport, "Group", 0 );
                  while ( nRC == zCURSOR_SET )
                  {
                     if ( CheckMigrateCtrls( vReport, vPE, lCheckFlags ) )
                        bChange = TRUE;

                     nRC = SetCursorNextEntity( vReport, "Group", 0 );
                  }

                  nRC = SetCursorNextEntity( vReport, "GroupSet", 0 );
               }

               nRC = SetCursorNextEntity( vReport, "Page", 0 );
            }

            if ( bChange )
            {
               TraceLineS( "MigrateReports modified report: ", pch );
               CommitMetaOI( vSubtask, vReport, zSOURCE_REPORT_META );
            }
            else
               TraceLineS( "MigrateReports unmodified report: ", pch );
         }
         else
         if ( bSaveAll )
         {
            sprintf_s( szMsg, sizeof( szMsg ), "Saving Report: %s", pch );
            MB_SetMessage( vSubtask, 1, szMsg );
            TraceLineS( "MigrateReports (all) report: ", pch );
            CommitMetaOI( vSubtask, vReport, zSOURCE_REPORT_META );
         }
         else
         {
            zCHAR  szFileSpec1[ zMAX_FILESPEC_LTH + 1 ];
            zCHAR  szFileSpec2[ zMAX_FILESPEC_LTH + 1 ];
            zPCHAR pch1;
            zPCHAR pch2;
            zLONG  lFilePRP = -1;
            zLONG  lFileXRP = -1;
            zSHORT nLth;

            sprintf_s( szMsg, sizeof( szMsg ), "Checking Report: %s for synchronization", pch );
            TraceLineS( szMsg, "" );
            MB_SetMessage( vSubtask, 1, szMsg );
            GetStringFromAttribute( szFileSpec2, sizeof( szFileSpec2 ), vTaskLPLR, "LPLR", "ExecDir" );
            nLth = (zSHORT) zstrlen( szFileSpec2 );
            if ( nLth && szFileSpec2[ nLth - 1 ] != '\\' )
               szFileSpec2[ nLth++ ] = '\\';

            strcpy_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, pch );
            strcat_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, ".xwd" );

            GetStringFromAttribute( szFileSpec1, sizeof( szFileSpec1 ), vTaskLPLR, "LPLR", "MetaSrcDir" );
            nLth = (zSHORT) zstrlen( szFileSpec1 );
            if ( nLth && szFileSpec1[ nLth - 1 ] != '\\' )
               szFileSpec1[ nLth++ ] = '\\';

            strcpy_s( szFileSpec1 + nLth, sizeof( szFileSpec1 ) - nLth, pch );
            nLth += (zSHORT) zstrlen( pch );
            strcpy_s( szFileSpec1 + nLth, sizeof( szFileSpec1 ) - nLth, ".prd" );
            lFilePRP = SysOpenFile( vSubtask, szFileSpec1, COREFILE_READ );
            lFileXRP = SysOpenFile( vSubtask, szFileSpec2, COREFILE_READ );
            strcpy_s( szFileSpec2, sizeof( szFileSpec2 ), szFileSpec1 );
            strcpy_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, ".tmp" );
            if ( lFilePRP >= 0 &&
                 (lFileXRP < 0 ||
                  (lFileXRP >= 0 &&
                   SysReadLine( vSubtask, &pch1, lFilePRP ) >= 0 &&
                   SysReadLine( vSubtask, &pch2, lFileXRP ) >= 0 &&
                   fnLocateReportDate( pch1 ) == 0 &&
                   fnLocateReportDate( pch2 ) == 0 &&
                   (zstrcmp( pch1, pch2 ) > 0 || bSaveAll))) && // ==> out of sync
                 SysCopyFile( vSubtask, szFileSpec1, szFileSpec2, TRUE ) > 0 )
            {
               SysCloseFile( vSubtask, lFilePRP, 0 );
               lFilePRP = -1;
               if ( lFileXRP >= 0 )
                  SysCloseFile( vSubtask, lFileXRP, 0 );

               lFileXRP = -1;

               strcpy_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, ".pr~" );
               SysRenameFile( vSubtask, szFileSpec1, szFileSpec2, TRUE );
               strcpy_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, ".tmp" );
               SysRenameFile( vSubtask, szFileSpec2, szFileSpec1, TRUE );

               TraceLineS( "MigrateReports synchronize report: ", pch );
               CommitMetaOI( vSubtask, vReport, zSOURCE_REPORT_META );
               strcpy_s( szFileSpec2 + nLth, sizeof( szFileSpec2 ) - nLth, ".pr~" );
               SysRenameFile( vSubtask, szFileSpec2, szFileSpec1, TRUE );
            }
            else
            {
               TraceLineS( "MigrateReports unmodified report: ", pch );
            }

            if ( lFilePRP >= 0 )
               SysCloseFile( vSubtask, lFilePRP, 0 );

            if ( lFileXRP >= 0 )
               SysCloseFile( vSubtask, lFileXRP, 0 );
         }

         DropObjectInstance( vReport );
      }
      else
      {
         TraceLineI( "MigrateReports unexpected failure to open report in list ", k );
      }

      nRC = SetCursorNextEntity( vReportList, "W_MetaDef", 0 );
   }

   DropView( vReportList );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ReturnFromSubwindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ReturnFromSubwindow( zVIEW vSubtask )
{
   RefreshWindow( vSubtask );

   return( 0 );
} // ReturnFromSubwindow

#ifdef __cplusplus
}
#endif
