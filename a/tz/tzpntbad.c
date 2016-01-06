//
// MODULE NAME:  TZPNTBAD  -  Painter ToolBar and Palette control maintenance
//               dialog
// DESCRIPTION:  This is the C file which contains the painter ToolBar control
//               sub-dialog operations
/////////////////////////////////////////////////////////////////////////////////
//* Copyright © 1997-2002 QuinSoft, Inc. All rights reserved.  Confidential *
//* and Proprietary material subject to license - do not reproduce or       *
//* disclose.  This material is an unpublished work and is considered a     *
//* trade secret belonging to the copyright holder.                         *
/////////////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    09/30/92
//  API:     MicroSoft foundation classes and Windows 3.1 SDK
//  ENVIRONMENT: Windows 3.1
//
// LAST MESSAGE ID: PN00220

/* Change log most recent first order:

2002.03.06   DKS
   Added create new action for toolbar control.

2002.02.11   DKS
   Modified TabPostBuild for Tab Control with GroupBox Parent.

*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "stdlib.h"
#include <ctype.h>
#include <string.h>

#include "tz__oprs.h"
#include "tzlodopr.h"

extern "C"
{

#include "tzctlopr.h"
#include "ZeidonOp.H"

#pragma pack( 1 )
#include <tzwdlgso.hst>
#pragma pack( )

zOPER_EXPORT zSHORT OPERATION
RepaintZeidonControl( zVIEW vSubtask );


#define zTOOLBAR_CONTROL                    2100L
#define zTOOLITEM_CONTROL                   2101L

#define zTOOLBAR_PALETTE      zCONTROL_SPECIFIC_1
#define zTOOLBAR_TEXT         zCONTROL_SPECIFIC_2
#define zTOOLBAR_TOOLTIPS     zCONTROL_SPECIFIC_3
#define zTOOLBAR_BOTTOM       zCONTROL_CENTERJUSTIFY
#define zTOOLBAR_RIGHT        zCONTROL_RIGHTJUSTIFY
#define zTOOLBAR_LEFT         zCONTROL_LEFTJUSTIFY

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarPreBuild( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vControl, vControl, 0 );
   SetNameForView( vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vControl, vControl, 0 );
   SetViewToSubobject( vControl, "CtrlCtrl" );
   SetNameForView( vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );

   return( 0 );

} // ToolBarPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarPostBuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szBlob[ zMAX_FILENAME_LTH * 3 ];
   zLONG  lSubtype;
   zULONG ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
   if ( ulLth )
   {
      zSHORT k;

      k = (zSHORT) zstrlen( szBlob ) + 1;
      zPCHAR pchFileName = szBlob + k;
      SetCtrlText( vSubtask, "DLL", szBlob );
      SetCtrlText( vSubtask, "BitmapFile", pchFileName );
      if ( pchFileName[ 0 ] )
      {
         zPCHAR pch;
         pch = zstrchr( pchFileName, ';' );
         if ( pch )
            pch++;
         else
            pch = pchFileName;

         BMP_SetBitmapFileName( vSubtask, "Bitmap", pch );
      }
   }

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & zTOOLBAR_PALETTE )
      SetCtrlState( vSubtask, "Palette", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtype & zTOOLBAR_TEXT )
      SetCtrlState( vSubtask, "ButtonText", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtype & zTOOLBAR_TOOLTIPS )
      SetCtrlState( vSubtask, "ToolTips", zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtype & zTOOLBAR_BOTTOM )
      SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED, TRUE );
   else
   if ( lSubtype & zTOOLBAR_RIGHT )
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED, TRUE );
   else
   if ( lSubtype & zTOOLBAR_LEFT )
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );
   else
// if ( lSubtype & zTOOLBAR_TOP )
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED, TRUE );

   ED_LimitTextLength( vSubtask, "DLL", zMAX_FILENAME_LTH - 1 );
   ED_LimitTextLength( vSubtask, "ResName", zMAX_FILENAME_LTH - 1 );
   ED_LimitTextLength( vSubtask, "BitmapFile", zMAX_FILENAME_LTH - 1 );
   SetCtrlTranslationList( vSubtask, "BarItemList",
                           "1;Button;2;DropButton;3;ComboBox;4;EditCombo", 3 );
   SetCtrlTranslationList( vSubtask, "BarItemList", "1;Y", 9 );
   return( 0 );

} // ToolBarPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarUPD_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarUPD_OK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szBlob[ zMAX_FILENAME_LTH * 2 ];
   zLONG  lSubtype;
   zULONG ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   DropView( vControl );
   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   DropView( vControl );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetCtrlText( vSubtask, "DLL", szBlob, sizeof( szBlob ) - 1 );
   ulLth = zstrlen( szBlob ) + 1;
   GetCtrlText( vSubtask, "BitmapFile", szBlob + ulLth, (zUSHORT) (sizeof( szBlob ) - (ulLth + 1)) );
   ulLth += zstrlen( szBlob + ulLth ) + 1;
   szBlob[ ulLth++ ] = 0;  // double terminate for future additions
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( GetCtrlState( vSubtask, "Palette", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_PALETTE;
   else
      lSubtype &= ~zTOOLBAR_PALETTE;

   if ( GetCtrlState( vSubtask, "ButtonText", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_TEXT;
   else
      lSubtype &= ~zTOOLBAR_TEXT;

   if ( GetCtrlState( vSubtask, "ToolTips", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_TOOLTIPS;
   else
      lSubtype &= ~zTOOLBAR_TOOLTIPS;

   if ( GetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_BOTTOM;
   else
      lSubtype &= ~zTOOLBAR_BOTTOM;

   if ( GetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_RIGHT;
   else
      lSubtype &= ~zTOOLBAR_RIGHT;

   if ( GetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zTOOLBAR_LEFT;
   else
      lSubtype &= ~zTOOLBAR_LEFT;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   // Accept the Control.
   AcceptSubobject( vControl, "Control" );

   // Repaint properly.
   RepaintZeidonControl( vSubtask );
   return( 0 );

} // ToolBarUPD_OK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarUPD_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarUPD_Cancel( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   DropView( vControl );
   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   DropView( vControl );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CancelSubobject( vControl, "Control" );

   // Repaint properly.
   RepaintZeidonControl( vSubtask );
   return( 0 );

} // ToolBarUPD_Cancel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LocateBitmapFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LocateBitmapFile( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szFileName[ zMAX_FILENAME_LTH * 2 ]; // space for ResId
   zPCHAR pch;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetCtrlText( vSubtask, "BitmapFile", szFileName,
                sizeof( szFileName ) - 1 );
   pch = zstrchr( szFileName, ';' );
   if ( pch )
      pch++;
   else
      pch = szFileName;

   if ( OperatorPromptForFile( vSubtask, pch, sizeof( szFileName ) - 1,
                               "(*.bmp)|*.bmp|All Files (*.*)|*.*||",
                               0, TRUE ) == 1 )
   {
      SetCtrlText( vSubtask, "BitmapFile", szFileName );
      BMP_SetBitmapFileName( vSubtask, "Bitmap", szFileName );
   }

   return( 0 );

} // LocateBitmapFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarAdd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarAdd( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vPE;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key",
                                          zTOOLITEM_CONTROL,
                                          0 ) == zCURSOR_SET )
      {
         CreateTemporalMetaEntity( vSubtask, vControl, "Control", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vControl, "ControlDef",
                                        vPE, "ControlDef",
                                        zPOS_BEFORE );
         return( 0 );
      }
   }

   SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   return( 0 );

} // ToolBarAdd

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarUpdate
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarUpdate( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vControl, "Control" );
   return( 0 );

} // ToolBarUpdate

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarDelete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarDelete( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   DeleteEntity( vControl, "Control", zREPOS_PREV );
   return( 0 );

} // ToolBarDelete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarItemPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarItemPostBuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTemp;
   zCHAR  szText[ 256 ];
   zPCHAR pchToolTip;

   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTemp, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetViewFromView( vTemp, vControl );
   GetStringFromAttribute( szText, sizeof( szText ), vControl, "Control", "Text" );
   pchToolTip = zstrchr( szText, '\t' );
   if ( pchToolTip )
   {
      *pchToolTip = 0;
      pchToolTip++;
      SetCtrlText( vSubtask, "ToolTip", pchToolTip );
   }

   SetCtrlText( vSubtask, "Text", szText );
   ED_LimitTextLength( vSubtask, "Text", 175 );
   ED_LimitTextLength( vSubtask, "ToolTip", 79 );
   SetCtrlTranslationList( vSubtask, "Type",
//                         "1;Button", 1 );
                           "1;Button;2;DropButton;3;ComboBox;4;EditCombo", 1 );

   return( 0 );

} // ToolBarItemPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarItemUPD_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarItemUPD_OK( zVIEW vSubtask )
{
   zCHAR  szText[ 256 ];
   zVIEW  vControl;
   zSHORT nLth;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   ResetViewFromSubobject( vControl );

   // Accept the Control.
   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntity( vControl, "EventAct", 0 ) < zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vControl, "Event", 0 ) >= zCURSOR_SET )
         DeleteEntity( vControl, "Event", zREPOS_NONE );
   }

   GetCtrlText( vSubtask, "Text", szText, 175 );
   nLth = (zSHORT) zstrlen( szText );
   szText[ nLth++ ] = '\t';
   GetCtrlText( vSubtask, "ToolTip", szText + nLth, 79 );
   if ( szText[ 0 ] == '\t' && szText[ 1 ] == 0 )
      szText[ 0 ] = 0;

   SetAttributeFromString( vControl, "Control", "Text", szText );

   AcceptSubobject( vControl, "Control" );

   return( 0 );

} // ToolBarItemUPD_OK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ToolBarItemUPD_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ToolBarItemUPD_Cancel( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   ResetViewFromSubobject( vControl );

   // Cancel the Control.
   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntity( vControl, "EventAct", 0 ) < zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vControl, "Event", 0 ) >= zCURSOR_SET )
         DeleteEntity( vControl, "Event", zREPOS_NONE );
   }

   CancelSubobject( vControl, "Control" );
   return( 0 );

} // ToolBarItemUPD_Cancel

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ToolBarItemSelectAction
//
// PURPOSE:  This function creates the Event entity for the ToolBar Item.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ToolBarItemSelectAction( zVIEW vSubtask )
{
   zVIEW  vDialog;
   zVIEW  vControl;
   zVIEW  vWork;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
      CreateTemporalSubobjectVersion( vControl, "Event" );
   else
      CreateTemporalMetaEntity( vSubtask, vControl, "Event", zPOS_AFTER );

   // Set flag for LIST_ACT window graying and Action include for Control.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ToolBarItemAddAction
//
// PURPOSE:  This function creates an Action entity and includes it to
//           the selected control.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ToolBarItemAddAction( zVIEW vSubtask )
{
   zVIEW  vDialog;
   zVIEW  vControl;
   zVIEW  vWork;
// zCHAR  szTag[ 33 ];

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
// GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
// GetStringFromAttribute( szTag, sizeof( szTag ), vControl, "Control", "Tag" );
// TraceLineS( "ToolBarItemAddAction TZCONTROL: ", szTag );
// GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "Control", "Tag" );
// TraceLineS( "ToolBarItemAddAction TZCTLMAP3: ", szTag );

   if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
   {
      ExcludeEntity( vControl, "EventAct", zREPOS_NONE );
   }

   // Create and initialize the Action.
   if ( CheckExistenceOfEntity( vDialog, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialog, "Action", 0, 1 );

   CreateTemporalMetaEntity( vSubtask, vDialog, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialog, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialog, "Action", "Type", 1 );
   SetSelectStateOfEntityForSet( vDialog, "Action", 1, 1 );

   // Set flag for LIST_ACT window graying and Action include for Control.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ToolBarItemDeleteAction
//
// PURPOSE:  This function creates the Event entity for the ToolBar Item.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ToolBarItemDeleteAction( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
   {
      ExcludeEntity( vControl, "EventAct", zREPOS_NONE );
      RefreshCtrl( vSubtask, "Action" );
   }

   return( 0 );
}

#define zCONTROLBAR_GRIPPERS          zCONTROL_RESERVED_UNUSABLE
#define zCONTROLBAR_LEFT              zCONTROL_FULL_CLIENT
#define zCONTROLBAR_BOTTOM            zCONTROL_CENTERJUSTIFY
#define zCONTROLBAR_RIGHT             zCONTROL_RIGHTJUSTIFY
#define zCONTROLBAR_TOP               zCONTROL_SPECIFIC_1
#define zCONTROLBAR_XX2               zCONTROL_SPECIFIC_2
#define zCONTROLBAR_XX3               zCONTROL_SPECIFIC_3

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ControlBarPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ControlBarPostBuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   if ( ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      zLONG lSubtype;

      GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
      if ( lSubtype & zCONTROLBAR_GRIPPERS )
         SetCtrlState( vSubtask, "Grippers", zCONTROL_STATUS_CHECKED, TRUE );

      if ( lSubtype & zCONTROLBAR_LEFT )
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );

      if ( lSubtype & zCONTROLBAR_BOTTOM )
         SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED, TRUE );

      if ( lSubtype & zCONTROLBAR_RIGHT )
         SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED, TRUE );

      if ( lSubtype & zCONTROLBAR_TOP )
         SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "Grippers", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );

} // ControlBarPreBuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      ControlBarUPD_OK
//
// PURPOSE:    OK - Accepts control & issues repaint.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ControlBarUPD_OK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zLONG  lSubtype = 0;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( GetCtrlState( vSubtask, "Grippers", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLBAR_GRIPPERS;

   if ( GetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLBAR_LEFT;

   if ( GetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLBAR_BOTTOM;

   if ( GetCtrlState( vSubtask, "Right", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLBAR_RIGHT;

   if ( GetCtrlState( vSubtask, "Top", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLBAR_TOP;

   if ( (lSubtype & (zCONTROLBAR_LEFT | zCONTROLBAR_BOTTOM |
                     zCONTROLBAR_RIGHT | zCONTROLBAR_TOP)) == 0 )
   {
      MessageSend( vSubtask, "PN00220", "Dialog Maintenance",
                   "ControlBar must be dockable on at least on side",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   // Accept the Control.
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

}  // end of:  extern "C"
