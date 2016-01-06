//
// MODULE NAME:  TZPNCTLD  -  Painter tab and bitmap control maintenance
//               dialog
// DESCRIPTION:  This is the C file which contains the painter tab control
//               sub-dialog operations
/////////////////////////////////////////////////////////////////////////////////
//* Copyright © 1997-1999 QuinSoft, Inc. All rights reserved.  Confidential *
//* and Proprietary material subject to license - do not reproduce or       *
//* disclose.  This material is an unpublished work and is considered a     *
//* trade secret belonging to the copyright holder.                         *
/////////////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    09/30/92
//  API:     MicroSoft foundation classes and Windows 3.1 SDK
//  ENVIRONMENT: Windows 3.1
//

/* Change log most recent first order:

2002.06.11   BL for FH
   Avoid error "ViewIsReadOnly" if a dialog is noch checked out.

2002.06.03   FH
   Avoid error "ViewIsReadOnly" if a dialog is noch checked out.

2001.03.12   BL
   Modified TabPostBuild for Tab Control wtih GroupBox Parent

2001.02.28   BL   ActiveX
   Do not drop the dialog object instance

2001.02.28   BL   ActiveX
   Do not drop the dialog object instance

2001.2.26   TMV   2000   ActiveX
   Remove fnPainterCall from UPD_OK call this call is obsolete for
   ActiveX controls

2001.02.04  BL   Z2000
   Modified TabPostBuild and SubTabPostBuild: if Dialog not checked out,
   disable Controls for update

2001.01.16  DKS  Z2000
   Handle Tab control color settings.

2000.12.06  BL   Z2000
   Modified TabPostBuild: if Dialog not checked out, disable Controls
   for update

2000.10.30  RG   Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.09.17  BL    Z2000
   Replace SetCursorFirstEntityByEntityCsr with SetCursorFirstEntityByAttr

2000.09.09  BL    Z2000  Fix for FH
   Modified UPD_BmpCtlOK for REPAINTZEIDONWINDOW

2000.09.04  BL    Z2000  TB53528
   Load Control Context Mapping

2000.08.21  BL    Z2000  Repository and Tools
   Completely maintained Dialog Tool.
   If Dialog not checked out, disable all functions for update

2000.07.07  RG    Z2000
   Removed compiler warnings

2000.07.27  DKS   Z2000
   TMV requested context update on ActiveX detail.

1999.07.27  DKS   Z2000   QS999
   Tab event-action sub-dialog shows corresponding event/actions properly.

1999.05.11  DKS
   Permit Double-Click to go to appropriate sub-window for action maintenance.

1999.02.09  DKS
   Reimplemented lost code for Icon PosBuild event.

1998.12.29  DKS
   Added checks for report.

1998.11.02  TMV
   Changing functioncalls of TransferToEditor because of a new argument

14.11.1996  GT
   Don't show generated IDs for BMPs/ICONs

16.05.1997  DKS
   Modified OperatorPromptForFile signature for SaveAs option

16.12.1997  GT
   New operations for timer control

23.01.1998  DonC
   Created new operation GoToUpdateSubTab.

25.04.1998  DonC
   Modified UPD_BmpCtlBrowseFileName to change OperatorPrompt to allow
   read-only bitmaps. (QS 180)

03.06.1998  DonC
   Modified GoToUpdateSubTab to correct setting up tab events in 9J.

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

#include "tzctlopr.h"
#include "ZeidonOp.H"

zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg,    zVIEW vSubtask,
               zVIEW  vDialog, zPCHAR szParm, zLONG lMaxLth );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTLD_DisableButtons( zVIEW  vSubtask,
                           zPCHAR szActionNameForAction,
                           zPCHAR szActionNameForEdit );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPNCTLD_CtrlContextMapping( zVIEW vSubtask );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetActionView( zVIEW vSubtask );


#define zTAB_CONTROL                        2090L

//./ ADD NAME=RepaintZeidonControl
// Source Module=tzpnctld.c
////////////////////////////////////////////////
//
// ENTRY:      RepaintZeidonControl
//
// PURPOSE:    To re-paint a control after it has been updated
//
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RepaintZeidonControl( zVIEW vSubtask )
{
   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );
   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
TabPreBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zLONG   lType;
   zSHORT  k = 0;
   zSHORT  nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vControl, "Control" );

   zgSortEntityWithinParent( zASCENDING, vControl, "CtrlCtrl", "Type", "" );
   nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", 0 );
   while ( nRC == zCURSOR_SET )
   {
      k++;
      GetIntegerFromAttribute( &lType, vControl, "CtrlCtrl", "Type" );
      if ( lType == 0 )
         SetAttributeFromInteger( vControl, "CtrlCtrl", "Type", lType );

      nRC = SetCursorNextEntity( vControl, "CtrlCtrl", 0 );
   }

   SetCursorFirstEntity( vControl, "CtrlCtrl", 0 );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: DisableTabControls
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
DisableTabControls( zVIEW vSubtask, zVIEW vTZWINDOWL )
{

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "RaggedRight", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Multiline", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Vertical", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__ColorText", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__ColorTab", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SetTextColor", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SetTabColor", zCONTROL_STATUS_ENABLED, FALSE );
   }

    return( 0 );
} // DisableTabControls




// Values for m_lTabStyle (high order first):
//   4th digit - 0 ==> TCS_RAGGEDRIGHT
//               1 ==> turn off RAGGEDRIGHT
//   5th digit - 0 ==> TCS_SINGLELINE
//               1 ==> TCS_MULTILINE
//   6th digit - 0 ==> TCS_TOP
//               1 ==> TCS_LEFT
//               2 ==> TCS_BOTTOM
//               4 ==> TCS_RIGHT
//   7th digit - 0 ==> TCS_HORIZONTAL
//               1 ==> TCS_VERTICAL
//   8th digit - 1 ==> TCS_OWNERDRAWFIXED  (set by conversion)
//               0 ==> turn off TCS_OWNERDRAWFIXED
//

zOPER_EXPORT zSHORT OPERATION
TabPostBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;
   zLONG   lType;
   zLONG   lTabStyle;
   zULONG  ulTabColor = 0xFF000000;
   zULONG  ulTextColor = 0xFF000000;
   zCHAR   szBlob[ 5 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lType, vControl, "ControlDef", "Key" );
   if ( lType != zTAB_CONTROL )
   {
      TraceLineI( "TabPreBuild resetting view for control type ", lType );
      ResetViewFromSubobject( vControl );
   }

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth >= 3 * sizeof( zLONG ) )
   {
      lTabStyle = *((zPLONG) (szBlob + sizeof( zLONG ) * 2));
      if ( lTabStyle < 0 )
         lTabStyle = 0;

      if ( ulLth >= 4 * sizeof( zLONG ) )
      {
      // zCHAR  szColor[ 16 ];
         zPULONG pulTextColor = (zPULONG) (szBlob + sizeof( zLONG ) * 3);
         ulTextColor = *pulTextColor;

         zPLONG pulTabColor = (zPLONG) (szBlob + sizeof( zLONG ) * 4);
         ulTabColor = *pulTabColor;
      }
   }
   else
      lTabStyle = 0;

   SetCtrlProperty( vSubtask, "__ColorText",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    ulTextColor, 0 );
   SetCtrlProperty( vSubtask, "__ColorTab",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    ulTabColor, 0 );

   if ( (lTabStyle & 0x00010000) == 0 )
      SetCtrlState( vSubtask, "RaggedRight", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lTabStyle & 0x00001000 )
      SetCtrlState( vSubtask, "Multiline", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lTabStyle & 0x00000100 )
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );
   else
   if ( lTabStyle & 0x00000200 )
      SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED, TRUE );
   else
   if ( lTabStyle & 0x00000400 )
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED, TRUE );
   else
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lTabStyle & 0x00000010 )
      SetCtrlState( vSubtask, "Vertical", zCONTROL_STATUS_CHECKED, TRUE );

   DisableTabControls( vSubtask, vTZWINDOWL );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UpdateTab( zVIEW vSubtask )
{
   zVIEW   vControl;
   zPLONG  plTabStyle;
   zPULONG pulTextColor;
   zPULONG pulTabColor;
   zPCHAR  pchWork;
   zLONG   lTabStyle;
   zCHAR   szBlob[ 5 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );
// zCHAR   szColor[ 16 ];

   pchWork = (zPCHAR) GetCtrlState( vSubtask, "Edge",
                                    zCONTROL_STATUS_CHECKED );
   lTabStyle = zatol( pchWork ) << 8;

   if ( GetCtrlState( vSubtask, "RaggedRight",
                      zCONTROL_STATUS_CHECKED ) == 0 )
   {
      lTabStyle |= 0x00010000;
   }

   if ( GetCtrlState( vSubtask, "Multiline", zCONTROL_STATUS_CHECKED ) )
      lTabStyle |= 0x00001000;

   if ( GetCtrlState( vSubtask, "Vertical", zCONTROL_STATUS_CHECKED ) )
      lTabStyle |= 0x00000010;

   if ( lTabStyle & 0x00000500 ) // left or right edge, force multi & vertical
      lTabStyle |= 0x00001010;
   else
      lTabStyle &= ~0x00000010;   // top or bottom edge, force horizontal

   zmemset( szBlob, 0, 5 * sizeof( zLONG ) );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   plTabStyle = (zPLONG) (szBlob + sizeof( zLONG ) * 2);
   *plTabStyle = lTabStyle;

   pulTextColor = (zPULONG) (szBlob + sizeof( zLONG ) * 3);
   GetCtrlProperty( vSubtask, "__ColorText",
                    zCONTROL_PROPERTY_INTEGER_DATA, pulTextColor, 0, 0 );

   pulTabColor = (zPULONG) (szBlob + sizeof( zLONG ) * 4);
   GetCtrlProperty( vSubtask, "__ColorTab",
                    zCONTROL_PROPERTY_INTEGER_DATA, pulTabColor, 0, 0 );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   SetAttributeFromBlob( vControl, "Control", "CtrlBOI",
                         szBlob, 5 * sizeof( zLONG ) );

   zgSortEntityWithinParent( zASCENDING, vControl, "CtrlCtrl", "Type", "" );

   // Accept the Control.
   AcceptSubobject( vControl, "Control" );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   // Repaint properly.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CancelTab( zVIEW vSubtask )
{
   zVIEW   vControl;

   // Accept the Control.
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   CancelSubobject( vControl, "Control" );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   // Repaint properly.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SetTabTextColor( zVIEW vSubtask )
{
   zCHAR    szColor[ 16 ];
   COLORREF clr;
   COLORREF aclrCust[ 32 ];
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "TextColor", szColor, sizeof( szColor ) );
   clr = zatol( szColor );
// TraceLineI( "TextColor colorref = ", (zLONG) clr );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG) aclrCust, TRUE ) == 1 )
   {
   // TraceLineI( "TextColor colorref after = ", (zLONG) clr );
      zltoa( clr, szColor, sizeof( szColor ) );
      SetCtrlText( vSubtask, "TextColor", szColor );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SetTabTabColor( zVIEW vSubtask )
{
   zCHAR    szColor[ 16 ];
   COLORREF clr;
   COLORREF aclrCust[ 32 ];
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "TabColor", szColor, sizeof( szColor ) );
   clr = zatol( szColor );
// TraceLineI( "TabColor colorref = ", (zLONG) clr );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG) aclrCust, TRUE ) == 1 )
   {
   // TraceLineI( "TabColor colorref after = ", (zLONG) clr );
      zltoa( clr, szColor, sizeof( szColor ) );
      SetCtrlText( vSubtask, "TabColor", szColor );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
GoToUpdateSubTab( zVIEW vSubtask )
{
   // Set up for updating a subtab by Setting View to subcontrol.
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;

   // Set view down to the subtab.
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByAttr( vTZWINDOWL, "CtrlCtrl", "ZKey",
                               vControl, "CtrlCtrl", "ZKey", "Window" );
// CreateTemporalSubobjectVersion( vTZWINDOWL, "CtrlCtrl" );
   SetViewToSubobject( vControl, "CtrlCtrl" );

   // Build the TZPNEVWO object for the control events.
   fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SubTabPostBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;
   zULONG  ulTabColor = 0xFF000000;
   zULONG  ulTextColor = 0xFF000000;
   zCHAR   szBlob[ 2 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth >= sizeof( szBlob ) )
   {
   // zCHAR   szColor[ 16 ];
      zPULONG pulTextColor = (zPULONG) (szBlob + sizeof( zLONG ) * 0);
      ulTextColor = *pulTextColor;

      zPULONG pulTabColor = (zPULONG) (szBlob + sizeof( zLONG ) * 1);
      ulTabColor = *pulTabColor;
   }

   SetCtrlProperty( vSubtask, "__ColorText", zCONTROL_PROPERTY_INTEGER_DATA, ulTextColor, 0 );
   SetCtrlProperty( vSubtask, "__ColorTab", zCONTROL_PROPERTY_INTEGER_DATA, ulTabColor, 0 );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "TextColor", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "TabColor", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SetTextColor", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SetTabColor", zCONTROL_STATUS_ENABLED, FALSE );
   }

   SetActionView( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SubTabOK( zVIEW vSubtask )
{
   // Accept subtab and reset view back to parent.
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;
   zPULONG pulTextColor;
   zPULONG pulTabColor;
   zCHAR   szBlob[ 2 * sizeof( zLONG ) ];
   zULONG  ulLth = sizeof( szBlob );
// zCHAR   szColor[ 16 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   pulTextColor = (zPULONG) (szBlob + sizeof( zLONG ) * 0);
   GetCtrlProperty( vSubtask, "__ColorText",
                    zCONTROL_PROPERTY_INTEGER_DATA, pulTextColor, 0, 0 );

   pulTabColor = (zPULONG) (szBlob + sizeof( zLONG ) * 1);
   GetCtrlProperty( vSubtask, "__ColorTab",
                    zCONTROL_PROPERTY_INTEGER_DATA, pulTabColor, 0, 0 );

   if ( (*pulTextColor & 0x00FFFFFF) == 0 &&
        (*pulTabColor & 0x00FFFFFF) == 0 )
   {
      ulLth = 0;
   }

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

// AcceptSubobject( vTZWINDOWL, "CtrlCtrl" );
   ResetViewFromSubobject( vControl );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SubTabCancel( zVIEW vSubtask )
{
   // Cancel subtab and reset view back to parent.
   zVIEW vControl;
   zVIEW vTZWINDOWL;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

// CancelSubobject( vTZWINDOWL, "CtrlCtrl" );
   ResetViewFromSubobject( vControl );
// SetViewFromView( vControl, vTZWINDOWL );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActionInitPrebuild( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
      {
         CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
         SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                          vDialogC, "EventAct", "" );
         SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
      }
      else
      // If not, set TZACTION to empty instance for auto graying.
      {
         fnCreateEmptyActionView( vSubtask, vDialogW );
      }
   }

   // Set up Domain Context mapping.
   zwTZPNCTLD_CtrlContextMapping( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpCtlPostBuild( zVIEW vSubtask )
{
   zVIEW        vControl;
   zVIEW        vTZWINDOWL;
   zLONG        lSubtype;
   zPCHAR       pCtrlBOI;
   zULONG       ulLth;
   zCTRLBOI_BMP BMP;
   zCHAR        szName[ zMAX_FILENAME_LTH + 10 ];

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
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
         zsprintf( szName, "%ld;%s", BMP.lBMPUpOrIconID,
                   BMP.szBMPUpOrIconName );
      else
         zsprintf( szName, "%s", BMP.szBMPUpOrIconName );

      SetCtrlText( vSubtask, "FileName", szName );
   }

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & 0x00000001 )
      SetCtrlState( vSubtask, "Fit", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & 0x00000002 )
      SetCtrlState( vSubtask, "UseDDB", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & 0x00001000 )
      SetCtrlState( vSubtask, "Hide", zCONTROL_STATUS_CHECKED, 1 );

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
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

   // get old CtrlBOI into CtrlBOI_BMP structure
   GetAddrForAttribute( &pCtrlBOI, vControl, "Control", "CtrlBOI" );
   GetAttributeLength( &ulLth, vControl, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );

   // set new CtrlBOI
   GetCtrlText( vSubtask, "DLL_Name", BMP.szDLLName, sizeof( BMP.szDLLName ) );

   // If there is a file name specified.
   GetCtrlText( vSubtask, "FileName", szName, sizeof( szName ) );
   if ( szName[ 0 ] )
   {
      // If there is an ID on the line separated by ';'
      pszBMPName = szName;
      while ( *pszBMPName && *pszBMPName != ';')
         pszBMPName++;

      if ( *pszBMPName == 0 )
      {
         // If no ID is specified, look for old one.
         if ( BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
         {
            // If not generated, delete it.
            BMP.lBMPUpOrIconID = 0;
         }

         strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), szName );
      }
      else
      {
         // If the ID has been specified, take it.
         strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), ++pszBMPName );
         BMP.lBMPUpOrIconID = zatol( szName );
      }
   }
   else
   {
      BMP.szBMPUpOrIconName[ 0 ] = 0;
      BMP.lBMPUpOrIconID = 0;
   }

   SetAttributeFromString( vControl, "Control", "WebFileName", BMP.szBMPUpOrIconName );
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
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpCtlBrowseFileName( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

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

zOPER_EXPORT zSHORT OPERATION
UPD_IconPostBuild( zVIEW vSubtask )
{
   zVIEW        vControl;
   zVIEW        vTZWINDOWL;
   zULONG       ulLth;
   zPCHAR       pCtrlBOI;
   zCTRLBOI_BMP BMP;
   zCHAR        szName[ zMAX_FILENAME_LTH + 10 ];

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetAttributeLength( &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth )
   {
      GetAddrForAttribute( &pCtrlBOI, vControl, "Control", "CtrlBOI" );
      SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );

      SetCtrlText( vSubtask, "DLL_Name", BMP.szDLLName );

      // Show BMP name and user maintained ID, but no generated ID.
      //   ID == 0: no ID given
      //   ID != 0 && <= zMAX_RES_ID_USER: ID from user, show it
      //   ID != 0 && > zMAX_RES_ID_USER: generated ID, don't show it
      if ( BMP.lBMPUpOrIconID && BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
         zsprintf( szName, "%ld;%s", BMP.lBMPUpOrIconID,
                   BMP.szBMPUpOrIconName );
      else
         zsprintf( szName, "%s", BMP.szBMPUpOrIconName );

      SetCtrlText( vSubtask, "FileName", szName );
   }

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "DLL_Name", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "FileName", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Browse", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_IconOK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szBlob[ 256 ];
   zLONG  k;
   zULONG ulLth;
   zPCHAR pCtrlBOI;
   zCTRLBOI_BMP BMP;
   zCHAR  szName[ zMAX_FILENAME_LTH + 10 ];
   zPCHAR pszBMPName;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Get old CtrlBOI into CtrlBOI_BMP structure.
   GetAddrForAttribute( &pCtrlBOI, vControl, "Control", "CtrlBOI" );
   GetAttributeLength( &ulLth, vControl, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP);

   // Set new CtrlBOI.
   GetCtrlText( vSubtask, "DLL_Name", BMP.szDLLName, sizeof( BMP.szDLLName ) );

   // If there is a file name specified.
   GetCtrlText( vSubtask, "FileName", szName, sizeof( szName ) );
   if ( szName[ 0 ] )
   {
      // If there is an ID on the line separated by ';'
      pszBMPName = szName;
      while ( *pszBMPName && *pszBMPName != ';')
         pszBMPName ++;

      if ( *pszBMPName == 0 )
      {
         // If no ID has been specified, look for old one.
         if ( BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
         {
            // If not generated, delete it.
            BMP.lBMPUpOrIconID = 0;
         }

         strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), szName );
      }
      else
      {
         // If an ID has been specified, take it.
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

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_IconBrowseFileName( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

   szFileName[ 0 ] = 0;
   if ( OperatorPromptForFile( vSubtask, szFileName, sizeof( szFileName ),
                               "Icon Files (*.ICO)|*.ico|",
                               "ICO", TRUE ) == 1 )
   {
      SetCtrlText( vSubtask, "FileName", szFileName );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ActionUpdate
//
// PURPOSE:  Select or Update the Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ActionUpdate( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZPNEVWO
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
      CreateTemporalSubobjectVersion( vDialogW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00103", "Dialog",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_SelectAction( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vEvents;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   while ( ResetViewFromSubobject( vDialogW ) == 0 )
      ;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, "Event", "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "Dialog",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vDialogC, "Event", "Type",
                                       lType, 0 ) >= zCURSOR_SET )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZPNCTAD", "UPD_ACT" );
      return( ActionUpdate( vSubtask ) );
//    MessageSend( vSubtask, "PN00117", "Dialog",
//                 "Event already has an Action!\n"
//                 "First remove the Action and then select a new one.",
//                 zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
//    SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
//    return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vDialogC, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vDialogC, "Event", "Type", lType );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}


#if 0
zOPER_EXPORT zSHORT OPERATION
ActionSelect( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vWork;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDialogC, "Event" ) != zCURSOR_SET )
   {
      CreateTemporalMetaEntity( vSubtask, vDialogC, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vDialogC, "Event", "Type", 1 );
   }
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByEntityCsr( vDialogW, "Action",
                                       vDialogC, "EventAct", "" );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}
#endif

zOPER_EXPORT zSHORT OPERATION
ActionNew( zVIEW vSubtask )
{
   zVIEW  vDialogW;
   zVIEW  vDialogC;
   zVIEW  vEvents;
   zVIEW  vWork;
   zLONG  lType;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, "Event", "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "Dialog",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( 0 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vDialogC, "Event", "Type",
                                       lType, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "Dialog",
                   "Event already exists.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vDialogC, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vDialogC, "Event", "Type", lType );
   }

//   ActionSelect( vSubtask );

   // LIST_ACT_AddAction( vSubtask );
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );

   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", "Type", 1 );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

#if 0
   // If current EventAct exists, exclude it.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
      ExcludeEntity( vDialogC, "EventAct", zREPOS_PREV );

   IncludeSubobjectFromSubobject( vDialogC, "EventAct",
                                  vDialogW, "Action", zPOS_AFTER );
#endif

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActionRemove( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vEvents;
   zLONG    lType;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                                       lType, 0 ) == zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDialog, "EventAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialog, "EventAct", zREPOS_PREV );

      DeleteEntity( vDialog, "Event", zREPOS_PREV );
   }

   fnSetupEventList( vSubtask, vDialog );
   fnCreateEmptyActionView( vSubtask, vDialog );

   RefreshCtrl( vSubtask, "EventAction" );
   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

    return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OperationEdit( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zCHAR  szTag[ 34 ];

   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vDialogC, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( szTag, sizeof( szTag ), vDialogC, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vDialogW, "Action", "Tag", szTag, "" );
      if ( CheckExistenceOfEntity( vDialogW, "Operation" ) == zCURSOR_SET &&
           CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vDialogW, "Operation", "Name",
                                     vDialogW, "ActOper", "Name", "Dialog" );
      }

      if ( CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         TransferToEditor( vSubtask, vDialogW, "PN", 0 );
      }
      else
      {
         MessageSend( vSubtask, "PN00109", "Dialog",
                      "No Operation exists for Action.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   {
      MessageSend( vSubtask, "PN00110", "Dialog",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_Init
//
// PURPOSE:  This function sets up the TZACTION view for the
//           UPD_PUSH window.  It is the same as TZWINDOW if a current
//           EventAct exits.  It is null otherwise.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_Init( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogC, "EventAct", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OK
//
// PURPOSE:  This function tells the Painter to refresh the control.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OK( zVIEW vSubtask )
{
   TZActiveX *pAtx;
   zVIEW     vDialogC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   fnPainterCall( zMSG_GETCURRCTRLWND, vSubtask, vSubtask, (zPCHAR) &pAtx, 0 );
   if ( pAtx ) // && pAtx->IsKindOf( RUNTIME_CLASS( TZActiveX ) ) )
   {
      zSHORT nAttrCnt = pAtx->GetAttributeCount( );
      zSHORT nEntityCnt = (zSHORT) CountEntitiesForView( vDialogC, "CtrlMap" );

//    TraceLineI( "UPD_OK AttrCnt: ", nAttrCnt );
//    TraceLineI( "UPD_OK EntityCnt: ", nEntityCnt );
      if ( nEntityCnt > nAttrCnt )
      {
         zCHAR szTempString[ 256 ];

         zsprintf( szTempString, "ActiveX control specified a maximum\n"
                                 "of %d mapping entities.\n"
                                 "There are %d user defined mappings.\n"
                                 "Continue with close?",
                   nAttrCnt, nEntityCnt );
         if ( MessagePrompt( vSubtask, "xxxxx",
                             "ActiveX Update",
                             szTempString,
                             zBEEP, zBUTTONS_YESNO,
                             zRESPONSE_NO,  0 ) == zRESPONSE_NO )
         {
            SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
            return( -1 );
         }
      }
   }

   // Accept control subobject.
   AcceptSubobject( vDialogC, "Control" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                         zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_Popup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "UPD_Popup", -1, 0, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;
   zVIEW  vTZPNEVWO;
   zCHAR  szEventTag[ 33 ];

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      if ( vDialogA )
         DropView( vDialogA );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogW, "Action", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

      // save cursor
      GetViewByName( &vTZPNEVWO, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szEventTag, sizeof( szEventTag ), vTZPNEVWO, "Event", "Tag" );

      // Build the TZPNEVWO object for the control events.
      fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );

      // set cursor
      GetViewByName( &vTZPNEVWO, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntityByString( vTZPNEVWO, "Event", "Tag", szEventTag, "" );
      RefreshCtrl( vSubtask, "EventAction" );
   }

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_TimerPostBuild( zVIEW vSubtask )
{
// zVIEW   vControl;
// zLONG   lSubtype;
// zPCHAR  pch;

// GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Set state "timer enabled".
   // We do not use subtype flag zCONTROL_DISABLED, since this flag
   // is changed by modal subdialogs, but should not be changed for
   // our timer.
// GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
// if ( lSubtype & zCONTROL_SPECIFIC_1 )
//    SetCtrlState( vSubtask, "Enabled", zCONTROL_STATUS_CHECKED, 1 );

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   return( 0 );

} // UPD_TimerPostBuild

zOPER_EXPORT zSHORT OPERATION
UPD_TimerOK( zVIEW vSubtask )
{
   zVIEW  vControl;
// zLONG  lSubtype = 0;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Get current state (enabled/disabled).
// if ( GetCtrlState( vSubtask, "Enabled", zCONTROL_STATUS_CHECKED ) > 0 )
//    lSubtype |= zCONTROL_SPECIFIC_1;
// else
//    lSubtype &= ~zCONTROL_SPECIFIC_1;
//
// SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   // Accept control subobject.
   AcceptSubobject( vControl, "Control" );
   return( 0 );

} // UPD_TimerOK

zOPER_EXPORT zSHORT OPERATION
ActiveX_Properties( zVIEW vSubtask )
{
   TZActiveX *pAtx;

   fnPainterCall( zMSG_GETCURRCTRLWND, vSubtask, vSubtask, (zPCHAR) &pAtx, 0 );
   if ( pAtx ) // && pAtx->IsKindOf( RUNTIME_CLASS( TZActiveX ) ) )
   {
      pAtx->ShowProperties( );
      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
ActiveX_AddMapEntity( zVIEW vSubtask )
{
   zVIEW vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vControl, "CtrlMap", zPOS_AFTER );
   SetAttributeFromString( vControl, "CtrlMap", "Tag", "New" );

   EnableAction( vSubtask, "DeleteMapping", TRUE );
   SetCtrlState( vSubtask, "UpdateContext", zCONTROL_STATUS_ENABLED, TRUE );
   RefreshCtrl( vSubtask, "Mapping" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActiveX_DeleteMapEntity( zVIEW vSubtask )
{
   zVIEW vControl;
   zBOOL bEnable = TRUE;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) == zCURSOR_SET )
   {
      DeleteEntity( vControl, "CtrlMap", zREPOS_PREV );
      RefreshCtrl( vSubtask, "Mapping" );
   }

   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) < zCURSOR_SET )
      bEnable = FALSE;

   EnableAction( vSubtask, "DeleteMapping", bEnable );
   SetCtrlState( vSubtask, "UpdateContext", zCONTROL_STATUS_ENABLED, bEnable );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActiveX_ContextMappingInit( zVIEW vSubtask )
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
ActiveX_BuildContextList( zVIEW vSubtask )
{
   ActiveX_ContextMappingInit( vSubtask );
   RefreshCtrl( vSubtask, "UpdateContext" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ActiveX_ContextSelect( zVIEW vSubtask )
{
   MapCtrl( vSubtask, "UpdateContext" );
   RefreshCtrl( vSubtask, "Mapping" );
   return( 0 );
}

#define zCALENDAR_PERMIT_NULL           zCONTROL_SPECIFIC_1
#define zCALENDAR_CHECKBOX              zCONTROL_SPECIFIC_2

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateCalendarInit
//
// PURPOSE:  This function sets up the TZACTION view for the
//           UpdateCalendar window.  It is the same as TZWINDOW if a current
//           EventAct exits.  It is null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateCalendarPreBuild( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogC, "EventAct", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   // Set up Domain Context mapping.
   zwTZPNCTLD_CtrlContextMapping( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateCalendarPostBuild
//
// PURPOSE:  This function sets Calendar values from Control.CtrlBOI.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateCalendarPostBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;
   zLONG   lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & zCALENDAR_PERMIT_NULL )
      SetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCALENDAR_CHECKBOX )
      SetCtrlState( vSubtask, "CheckBox", zCONTROL_STATUS_CHECKED, 1 );

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "CheckBox", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateCalendarOK
//
// PURPOSE:  This function sets Control.CtrlBOI values from CheckBox
//           entity and sets Context.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateCalendarOK( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vWork;
   zLONG    lSubtype;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );

   if ( GetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCALENDAR_PERMIT_NULL;
   else
      lSubtype &= ~zCALENDAR_PERMIT_NULL;

   if ( GetCtrlState( vSubtask, "CheckBox", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCALENDAR_CHECKBOX;
   else
      lSubtype &= ~zCALENDAR_CHECKBOX;

   SetAttributeFromInteger( vDialogC, "Control", "Subtype", lSubtype );

   // Accept the Control.
   AcceptSubobject( vDialogC, "Control" );

   // Set CtrlBOI values

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CalendarActionPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "CalendarActionPopup",
                            -1, 0, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateDayTimerInit
//
// PURPOSE:  This function sets up the TZACTION view for the
//           UpdateDayTimer window.  It is the same as TZWINDOW if a current
//           EventAct exits.  It is null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateDayTimerPreBuild( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByEntityCsr( vDialogA, "Action",
                                       vDialogC, "EventAct", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   // Set up Domain Context mapping.
   zwTZPNCTLD_CtrlContextMapping( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateDayTimerPostBuild
//
// PURPOSE:  This function sets DayTimer values from Control.CtrlBOI, which
//     has the format:
//        0 - Style - 4-byte long (3 unused) - MiniCal Appointment Both
//        4 - Rows - long
//        8 - Cols - long
//       12 - Size - long
//       16 - Max Appointment Days - long
//       20 - Type - (4-byte long) MultiSelect, PermitNull, Today, Position
//       24 - Selected Background Color - long
//       28 - Background Color - long
//       32 - 8 longs (reserved)
//       64 - Scoping Entity Name - char[ 33 ]
//       97 - Scope - 3-byte zCHAR (2 unused) - Scope Entity OI
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateDayTimerPostBuild( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vTZWINDOWL;
   zCHAR   szBlob[ 100 ];
   zULONG  ulLth = sizeof( szBlob );
   zCHAR   szTemp[ 32 ];

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth < sizeof( szBlob ) )
   {
      SetCtrlState( vSubtask, "Both", zCONTROL_STATUS_CHECKED, TRUE );
      SetCtrlText( vSubtask, "Rows", "1" );
      SetCtrlText( vSubtask, "Cols", "1" );
      SetCtrlText( vSubtask, "Size", "86" );
   // SetCtrlText( vSubtask, "MaxShowAppointment", "0" );
      SetCtrlState( vSubtask, "Today", zCONTROL_STATUS_CHECKED, TRUE );
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED, TRUE );
      SetCtrlProperty( vSubtask, "__ColorSelected",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       0xFF000000, 0 );
      SetCtrlProperty( vSubtask, "__ColorBackground",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       0xFF000000, 0 );
      SetCtrlState( vSubtask, "Default", zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   {
      if ( szBlob[ 1 ] == 'M' )
         SetCtrlState( vSubtask, "MiniCal", zCONTROL_STATUS_CHECKED, TRUE );
      else
      if ( szBlob[ 1 ] == 'A' )
         SetCtrlState( vSubtask, "Appointment", zCONTROL_STATUS_CHECKED, TRUE );
      else
   // if ( szBlob[ 1 ] == 'B' )
         SetCtrlState( vSubtask, "Both", zCONTROL_STATUS_CHECKED, TRUE );

      zltoa( *((zPLONG) (szBlob + 4)), szTemp, sizeof( szTemp ) );
      SetCtrlText( vSubtask, "Rows", szTemp );
      zltoa( *((zPLONG) (szBlob + 8)), szTemp, sizeof( szTemp ) );
      SetCtrlText( vSubtask, "Cols", szTemp );
      if ( *((zPLONG) (szBlob + 12)) )
      {
         zltoa( *((zPLONG) (szBlob + 12)), szTemp, sizeof( szTemp ) );
         SetCtrlText( vSubtask, "Size", szTemp );
      }

      zltoa( *((zPLONG) (szBlob + 16)), szTemp, sizeof( szTemp ) );
      SetCtrlText( vSubtask, "MaxShowAppointment", szTemp );

      // Type - (4-byte long) MultiSelect, PermitNull, Today, Position
      if ( szBlob[ 20 ] == 'M' )
         SetCtrlState( vSubtask, "MultiSelect", zCONTROL_STATUS_CHECKED, TRUE );

      if ( szBlob[ 21 ] == 'N' )
         SetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_CHECKED, TRUE );

      if ( szBlob[ 22 ] == 'T' )
         SetCtrlState( vSubtask, "Today", zCONTROL_STATUS_CHECKED, TRUE );

      if ( szBlob[ 23 ] == 'L' )
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );
      else
      if ( szBlob[ 23 ] == 'R' )
         SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED, TRUE );
      else
      if ( szBlob[ 23 ] == 'B' )
         SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED, TRUE );
      else
   // if ( szBlob[ 23 ] == 'T' )
         SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED, TRUE );

      SetCtrlProperty( vSubtask, "__ColorSelected",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       *((zPLONG) (szBlob + 24)), 0 );
      SetCtrlProperty( vSubtask, "__ColorBackground",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       *((zPLONG) (szBlob + 28)), 0 );

      SetCtrlText( vSubtask, "ScopeList", szBlob + 64 );
      if ( szBlob[ 97 ] == 'O' ) // letter o
         SetCtrlState( vSubtask, "OI", zCONTROL_STATUS_CHECKED, TRUE );
      else
      if ( szBlob[ 97 ] == 'S' )
         SetCtrlState( vSubtask, "Scope", zCONTROL_STATUS_CHECKED, TRUE );
      else
   // if ( szBlob[ 97 ] == 'P' )
         SetCtrlState( vSubtask, "Default", zCONTROL_STATUS_CHECKED, TRUE );
   }

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "Style", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Rows", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Cols", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Size", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__ColorSelected", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__ColorBackground", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MaxShowAppointment", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MultiSelect", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Today", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Position", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Scope", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateDayTimerOK
//
// PURPOSE:  This function sets Control.CtrlBOI values from DayTimer.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UpdateDayTimerOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szBlob[ 100 ];
   zULONG  ulLth = sizeof( szBlob );
   zCHAR   szTemp[ 32 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Accept the Control.
   AcceptSubobject( vControl, "Control" );

   // Set CtrlBOI values
   zmemset( szBlob, 0, sizeof( szBlob ) );

   if ( GetCtrlState( vSubtask, "MiniCal", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 1 ] = 'M';
   else
   if ( GetCtrlState( vSubtask, "Appointment", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 1 ] = 'A';
   else
// if ( GetCtrlState( vSubtask, "Both", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 1 ] = 'B';

   GetCtrlText( vSubtask, "Rows", szTemp, sizeof( szTemp ) );
   *((zPLONG) (szBlob + 4)) = zatol( szTemp );
   GetCtrlText( vSubtask, "Cols", szTemp, sizeof( szTemp ) );
   *((zPLONG) (szBlob + 8)) = zatol( szTemp );
   GetCtrlText( vSubtask, "Size", szTemp, sizeof( szTemp ) );
   *((zPLONG) (szBlob + 12)) = zatol( szTemp );
   GetCtrlText( vSubtask, "MaxShowAppointment", szTemp, sizeof( szTemp ) );
   *((zPLONG) (szBlob + 16)) = zatol( szTemp );

   // Type - (4-byte long) MultiSelect, PermitNull, Today, Position
   if ( GetCtrlState( vSubtask, "MultiSelect", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 20 ] = 'M';

   if ( GetCtrlState( vSubtask, "PermitNull", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 21 ] = 'N';

   if ( GetCtrlState( vSubtask, "Today", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 22 ] = 'T';

   if ( GetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 23 ] = 'L';
   else
   if ( GetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 23 ] = 'R';
   else
   if ( GetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 23 ] = 'B';
   else
// if ( GetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 23 ] = 'T';

   zPULONG pulColor = ((zPULONG) (szBlob + 24));
   GetCtrlProperty( vSubtask, "__ColorSelected", zCONTROL_PROPERTY_INTEGER_DATA, pulColor, 0, 0 );
   pulColor = ((zPULONG) (szBlob + 28));
   GetCtrlProperty( vSubtask, "__ColorBackground", zCONTROL_PROPERTY_INTEGER_DATA, pulColor, 0, 0 );

   GetCtrlText( vSubtask, "ScopeList", szBlob + 64, 33 );
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, sizeof( szBlob ) );

   if ( GetCtrlState( vSubtask, "OI", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 97 ] = 'O'; // letter o
   else
   if ( GetCtrlState( vSubtask, "Scope", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 97 ] = 'S';
   else
// if ( GetCtrlState( vSubtask, "Default", zCONTROL_STATUS_CHECKED ) )
      szBlob[ 97 ] = 'P';

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_DayTimerSetScope
//
// PURPOSE:  This function handles the setting of controls based on
//           the setting of the SCOPE Radio Buttons and Combo Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_DayTimerSetScope( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vTZWINDOWL;
   zLONG    lScopeOI;
   zSHORT   nIsCheckedOut;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

   GetIntegerFromAttribute( &lScopeOI, vWork, "ListBox", "ScopeOI" );
   if ( lScopeOI == 2 )
   {
      if ( nIsCheckedOut == 1 )
      {
         SetCtrlState( vSubtask, "Inhibit", zCONTROL_STATUS_ENABLED, TRUE );
         SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, TRUE );
         SetCtrlState( vSubtask, "ScopeListText",
                       zCONTROL_STATUS_ENABLED, TRUE );
      }
   }
   else
   if ( lScopeOI == 3 )
   {
      SetCtrlState( vSubtask, "Inhibit", zCONTROL_STATUS_CHECKED, FALSE );
      SetCtrlState( vSubtask, "Inhibit", zCONTROL_STATUS_ENABLED, FALSE );
      SetAttributeFromString( vWork, "ListBox",
                              "InhibitListForNoSelScope", "N" );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeListText",
                       zCONTROL_STATUS_ENABLED, FALSE );
   }
   else
   {
      if ( nIsCheckedOut == 1 )
         SetCtrlState( vSubtask, "Inhibit", zCONTROL_STATUS_ENABLED, TRUE );

      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeListText",
                       zCONTROL_STATUS_ENABLED, FALSE );
      if ( lScopeOI != 1 )
         SetAttributeFromInteger( vWork, "ListBox", "ScopeOI", 1 );
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_DayTimerSelectScope
//
// PURPOSE:  This function includes the Scoping entity under the
//           CtrlMap entity.  The entity is included from the SEL_LOD
//           object, but which entity to include comes from the
//           TZSCOPEL object.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_DayTimerSelectScope( zVIEW vSubtask )
{
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;

   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude existing relationship if necessary.
   if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

   /* If the user selected the blank, then no entities will be selected. */
   if ( SetCursorFirstSelectedEntity( vScopeL, "ScopeEntity", 0 ) == 0 )
   {
      // Get position on correct LOD_Entity in SEL_LOD
      GetIntegerFromAttribute( &lZKey, vScopeL, "ScopeEntity",
                                                      "LOD_EntityZKey" );
      SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                                      "ZKey", lZKey, "" );
      IncludeSubobjectFromSubobject( vScope, "CtrlMapLOD_Entity",
                                    vLOD, "LOD_Entity", zPOS_AFTER );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
DayTimerActionPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "DayTimerActionPopup",
                            -1, 0, FALSE, FALSE );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTLD_DisableButtons
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTLD_DisableButtons( zVIEW  vSubtask,
                           zPCHAR szActionNameForAction,
                           zPCHAR szActionNameForEdit )
{
   zVIEW vEvents;
   zVIEW vDialogA;
   zVIEW vTZWINDOWL;
   zBOOL bEnableAction    = FALSE;
   zBOOL bEnableEdit      = FALSE;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bEnableAction = TRUE;

            if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) > 0 &&
                 CheckExistenceOfEntity( vDialogA, "ActOper" ) >= zCURSOR_SET)
            {
               bEnableEdit = TRUE;
            }
         }
      }

      EnableAction( vSubtask, szActionNameForEdit, bEnableEdit );
      EnableAction( vSubtask, szActionNameForAction, bEnableAction );
   }

   return( 0 );
} // zwTZPNCTLD_DisableButtons

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ActiveX_Postbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ActiveX_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;
   zVIEW  vControl;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   ActiveX_ContextMappingInit( vSubtask );

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ))
   {
      SetCtrlState( vSubtask, "Properties", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UpdateContext", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Add", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );
   }

   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) < zCURSOR_SET )
   {
      EnableAction( vSubtask, "DeleteMapping", FALSE );
      SetCtrlState( vSubtask, "UpdateContext", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // ActiveX_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SetActionView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SetActionView( zVIEW vSubtask )
{

   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZPNEVWO
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   {
      // If not, set TZACTION to empty instance for auto graying.
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   zwTZPNCTLD_DisableButtons( vSubtask, "SelectUpdate", "GotoEditor" );

   return( 0 );
} // SetActionView

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTLD_LoadEventPopups
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTLD_LoadEventPopups( zVIEW  vSubtask,
                            zPCHAR szActionNameForUpdate,
                            zPCHAR szActionNameForNew,
                            zPCHAR szActionNameForSelect,
                            zPCHAR szActionNameForRemove,
                            zPCHAR szPopupName )
{
   zVIEW  vEvents;
   zVIEW  vDialog;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bUpdate = TRUE;
            if ( nIsCheckedOut == 1 )
               bDelete = TRUE;
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

      CreateTrackingPopupMenu( vSubtask, szPopupName, -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // zwTZPNCTLD_LoadEventPopups

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LoadActionPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LoadActionPopup( zVIEW vSubtask )
{
   zwTZPNCTLD_LoadEventPopups( vSubtask,
                               "SelectUpdate",
                               "NewAction",
                               "SelectAction",
                               "RemoveAction",
                               "UPD_ActionPopup" );


   return( 0 );
} // LoadActionPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ActiveX_LoadMappingPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ActiveX_LoadMappingPopup( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bUpdate       = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META );

      if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) >= zCURSOR_SET )
      {
         bUpdate = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "UpdateMapping", bUpdate );
      EnableAction( vSubtask, "AddMapping", bNew );
      EnableAction( vSubtask, "DeleteMapping", bDelete );

      CreateTrackingPopupMenu( vSubtask, "UPD_MappingPopup", -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // ActiveX_LoadMappingPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTLD_CtrlContextMapping
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPNCTLD_CtrlContextMapping( zVIEW vSubtask )
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

} // zwTZPNCTLD_CtrlContextMapping


#ifdef __cplusplus
}
#endif
