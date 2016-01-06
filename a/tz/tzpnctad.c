//
// MODULE NAME:  TZPNCTAD  -  Painter control dialog operations
// DESCRIPTION:  This is the c++ file which contains the painter
//               control dialog operations
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Inc. All rights reserved.  Confidential   *
// * and Proprietary material subject to license - do not reproduce or      *
// * disclose.  This material is an unpublished work and is considered a    *
// * trade secret belonging to the copyright holder.                        *
// //////////////////////////////////////////////////////////////////////////
// AUTHOR:  Gig Kirk
// DATE:    10/03/92
// API:     MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT: Windows 3.1
//
// LAST MESSAGE ID: PN00120
//

/*
CHANGE LOG
2003.12/31 DonC
   Modified code to set Control attributes for Web interfacing.
2003.05.20 DKS
   Implemented new Grid settings in spreadsheet dialog.
2002.10.16 DKS
   Fixed setting of CtrlBOI length for UPD_TEXT_PostBuild.
2002.06.24 BL
   Bugfix for remapping after remove Registered View
2002.06.11 BL for FH
   Avoid error "ViewIsReadOnly" if a dialog is noch checked out.
2002.06.10 BL  R55919
   Modified UPD_ACT_BuildWindowList: If Window does not exists in Dialog,
   remove Window Name
2002.05.21 DKS
   Keep text and common BORDER in sync.
2002.04.30 DKS/HH
   UPD_EDIT_Postbuild:
   consistency check to recognize old garbage editbox CtrlBOI's.
2002.04.29 DKS
   Permit button text to be multi-line.
2002.04.10 DKS
   Added ComboBox type to prevent message when nno mapping is specified.
2002.04.09 FH
   On a repository based dialog disable all controls.
2002.04.04 FH
   Allow euro symbol for currency.
2002.03.12 FH
   Avoid "ViewIsReadOnly" if a meta is not checked out.
2002.03.06  DKS
   Added format pic for edit controls.
2001.10.01  DKS   54532
   Sort on displayed data.
2001.06.11  BL
   Modified LoadOperations: on Receive Focus Event of Action Detail Window,
   regenerate the Operation List
2001.06.11  BL
   Modified UPD_ACT_SelectDialogOpers for show only Dialog Operations
2000.01.09  DKS   R53976
   Modified SEL_ATTR_Init to ensure we find the right ViewObjRef!
   If not we assume that something nasty has happened to the mapping,
   so we do the default processing.
2000.12.27  BL    R 54106
   Modified zwTZPNCTAD_TransferToEditor for set cursor to current operation.
2000.12.07  BL    Z2000
   Modified SEL_ATTR_SelectMapping for Control List Bugfix
2000.12.06  BL    Z2000
   Modified UPD_PUSH_Postbuild: if Dialog not checked out, disable Controls
   for update
2000.10.30  RG    Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.17  DKS   Z2000
   All returns from control maintenance dialogs now refresh the window.
2000.10.05  BL    Z2000
   Modified zwTZPNCTAD_CommonDetailPostBuild, zwTZPNCTAD_CommonDetailOK,
   UPD_EDIT_Postbuild, SEL_ATTR_Postbuild, SEL_ATTR_SelectMapping,
   SEL_ATTR_Init, SEL_ATTR_Cancel for Zeidon Reports
2000.09.23  BL    Z2000
   Modifified UPD_EDIT_OK for Zeidon Reports
2000.09.17  BL    Z2000
   Replace SetCursorFirstEntityByEntityCsr with SetCursorFirstEntityByAttr
2000.09.11  BL    Z2000
   Remove warning Level 1
2000.09.08  BL    Z2000  Fix for FH
   Set new Tag Name
2000.09.04  BL    Z2000  TB53528
   Load Control Context Mapping
2000.08.14  BL    Z10  Repository and Tools
   Completely maintained Dialog Tool.
   If Dialog not checked out, disable all functions for update
2000.06.19  DKS   Z10     Bug
   LIST_ACT_PostBuildInit checks for existence of EventAct entity.
2000.03.15  DKS   Z10     Enhance
   ListCtrl definition permits Header ListBox style to retain sort order.
2000.02.07  DKS
   Fixed urgent request for EditBox Upper/Lower case definition problem.
2000.01.17  BL
   Modified LIST_ACT_PostBuildInit: check existence of View
1999.11.23  BL
   Added Operation LoadOperations: Operation List is a modeless Window,
   if a Operation updated or new created, then refresh the Operation combobox
   Added Operation zwTZPNCTAD_DeleteAllActions for Delete all Actions from
   current Window
   Modified Operation UPD_PUSH_UpdateAction
   If EventAct exists, set cursor for Update Action window.
1999.11.19    BL     Z10       new menu structure
   Modified Operation LIST_ACT_DeleteAction and fnDeleteAction for
   delete all selected Actions
   Modified Operation fnSetupEventList for not change View Cursor
   Modified Operation ACT_USE_BuildActionUseList for set CaptionTitle
1999.11.17    BL     Z10
   Modified Operation LIST_ACT_PostBuildInit for check exists View
   Modified Operation fnDeleteAction for remove warning
   Added Operation zwTZPNCTAD_RefreshActionList: show Action List modeless
1999.10.19    DKS    Z10       QS999
   Disabled NOINTEGRALHEIGHT flag.
1999.05.11    DKS    Z10       ActiveX
   Added zCONTROL_FULL_CLIENT generic flag for controls.
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

#define zBUTTON_MULTILINE             zCONTROL_SPECIFIC_1
#define zBUTTON_HOVER                 zCONTROL_SPECIFIC_2

#define zCHECKBOX_XCHECK              zCONTROL_SPECIFIC_1
#define zCHECKBOX_LED                 zCONTROL_SPECIFIC_2

#define zLISTBOX_AUTODRAGSORT         zCONTROL_SPECIFIC_1
#define zLISTBOX_SORTEDHEADERS        zCONTROL_SPECIFIC_2
#define zLISTBOX_COLUMNHEADERS        zCONTROL_SPECIFIC_3
#define zLISTBOX_SCOPE_PARENT         zCONTROLX_SPECIFIC_1

// This can be used since one of the above must be set as well.
#define zLISTBOX_SEPARATORS           zCONTROL_RESERVED_UNUSABLE

#define zLISTBOX_SCOPE_OI             0x00004000L // 16384
#define zLISTBOX_SORT_DISPLAY         0x00008000L // 32768
#define zCOMBOBOX_SORT                0x00000010L // 16
#define zCOMBOBOX_SCOPE_OI            0x00000100L
#define zCOMBOBOX_NO_NULL             0x00000200L
#define zCOMBOBOX_EXTERNAL_DATA       0x00000800L

#define zCOMBOBOX_CONTROL                   1050L
#define zCOMBOITEM_CONTROL                  1051L

zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg,    zVIEW vSubtask,
               zVIEW  vDialog, zPCHAR szParm, zLONG lMaxLth );

zOPER_EXPORT zSHORT OPERATION
RepaintZeidonControl( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
CreateMappingEntity( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
ActionAdd( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SelectActionForControl( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
RemoveActionFromControl( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UpdateControlEvents( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_Init( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
SetDefaultRadioButton( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
ClearDefaultRadioButton( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SetScope( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UpdateRadioSubcontrol( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
PrebuildSpreadSheet( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION CtrlContextMappingInit( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION fnSetupEventList( zVIEW vSubtask, zVIEW vDialog );
zOPER_EXPORT zSHORT OPERATION fnSearchForEntityRecursively( zVIEW vLOD, zLONG lZKey,
                                                zPSHORT FoundFlag );
zOPER_EXPORT zSHORT OPERATION fnSetUpSEL_LOD( zVIEW vDialogC, zVIEW vDialogW,
                                              zVIEW vSubtask, zCPCHAR szMapView );
zOPER_EXPORT zSHORT OPERATION fnSetUpScopeListOI( zVIEW vDialogC, zVIEW vSubtask,
                                                  zCPCHAR szMapLOD_Entity);
zOPER_EXPORT zSHORT OPERATION fnDeleteAction( zVIEW vSubtask, zVIEW vDialogW );

zOPER_EXPORT zSHORT OPERATION
fnBuildOutlinerCtrls( zVIEW vSubtask, zVIEW vLOD,
                      zVIEW vTopCtrl, zVIEW vControl );

zOPER_EXPORT zSHORT OPERATION
fnResetOutliner( zVIEW vLOD, zVIEW vControl );

zOPER_EXPORT zSHORT OPERATION
UPD_ACT_Init( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SetupDialogList( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_ChangeView( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
ActionListTransfer( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
LIST_ACT_PostBuildInit( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_ACT_SetEditButtonStatus( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTAD_DisableButtons( zVIEW  vSubtask,
                           zPCHAR szButtonNameForAction,
                           zPCHAR szActionNameForEdit,
                           zPCHAR szActionNameForAction );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTAD_LoadEventPopups( zVIEW  vSubtask,
                            zPCHAR szActionNameForUpdate,
                            zPCHAR szActionNameForNew,
                            zPCHAR szActionNameForSelect,
                            zPCHAR szActionNameForRemove,
                            zPCHAR szActionNameForEdit,
                            zPCHAR szPopupName );


//./ ADD NAME=RepaintZeidonControl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      RepaintZeidonControl
//
// PURPOSE:    To re-paint a control after it has been updated
//
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

//./ ADD NAME=UPD_GeneralOK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_GeneralOK
//
// PURPOSE:    General Control OK that Accepts Control & issues repaint.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_GeneralOK( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zSHORT nRC;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Accept the Control.
   AcceptSubobject( vDialogC, "Control" );

   // Delete the WebControlPropertyOption entries.
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

//./ ADD NAME=UPD_GeneralCancel
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_GeneralCancel
//
// PURPOSE:    To End list sub-control processing
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_GeneralCancel( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zSHORT nRC;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Cancel subobject.
   CancelSubobject( vDialogC, "Control" );

   // Delete the WebControlPropertyOption entries.
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   return( 0 );
}

//./ ADD NAME=CreateMappingEntity
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      CreateMappingEntity
//
// PURPOSE:    To create a mapping entity instance under a control
//             if the control does not already have one
//
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
CreateMappingEntity( zVIEW vSubtask )
{
   zVIEW  vDialog;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialog, "CtrlMap" ) == zCURSOR_NULL &&
        MiGetUpdateForView( vDialog ) )
   {
      CreateMetaEntity( vSubtask, vDialog, "CtrlMap", zPOS_AFTER );
   }

   return( 0 );
}

//./ ADD NAME=ActionAdd
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      ActionAdd
//
// PURPOSE:    To Add an action to the window
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
ActionAdd( zVIEW vSubtask )
{
   zVIEW  vDialog;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   CreateMetaEntity( vSubtask, vDialog, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialog, "Action", "Tag", "New" );
   return( 0 );
}

//./ ADD NAME=SelectActionForControl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      SelectActionForControl
//
// PURPOSE:    To Add an action to a control event
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
SelectActionForControl( zVIEW vSubtask )
{
   zVIEW vDialog;
   zVIEW vEvents;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialog, "Action" ) > zCURSOR_UNCHANGED )
   {
      SetAttributeFromAttribute( vEvents, "EventAct", "Tag",
                                 vDialog, "Action", "Tag" );
   }

   return( 0 );
}

//./ ADD NAME=RemoveActionFromControl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      RemoveActionFromControl
//
// PURPOSE:    To Remove an action from a control event
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
RemoveActionFromControl( zVIEW vSubtask )
{
   zVIEW vEvents;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );
   return( 0 );
}

//./ ADD NAME=UpdateControlEvents
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UpdateControlEvents
//
// PURPOSE:    To Update the events defined for the pushbutton and
//             rebuild the painter window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UpdateControlEvents( zVIEW vSubtask )
{
   zVIEW vDialog;
   zVIEW vWindow;
   zVIEW vEvents;
   zLONG lType;
   zCHAR szAction[ 34 ];

   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntity( vEvents, "Event", 0 );
   do
   {
      GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
      GetStringFromAttribute( szAction, sizeof( szAction ), vEvents, "EventAct", "Tag" );
      if ( szAction[ 0 ] &&
           SetCursorFirstEntityByString( vWindow, "Action", "Tag",
                                         szAction, 0 ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                        lType, 0 ) < zCURSOR_SET )
         {
            CreateTemporalMetaEntity( vSubtask, vDialog, "Event", zPOS_AFTER );
            SetAttributeFromInteger( vDialog, "Event", "Type", lType );
         }

         if ( SetCursorFirstEntity( vDialog, "EventAct", 0 ) >= zCURSOR_SET )
            ExcludeEntity( vDialog, "EventAct", zREPOS_NONE );

         IncludeSubobjectFromSubobject( vDialog, "EventAct",
                                        vWindow, "Action", zPOS_AFTER );
      }
      else
      if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                                          lType, 0 ) > zCURSOR_UNCHANGED )
      {
         DeleteEntity( vDialog, "Event", zREPOS_NONE );
      }

   } while( SetCursorNextEntity( vEvents, "Event", 0 ) > zCURSOR_UNCHANGED );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

//./ ADD NAME=AssignDefaultButton
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      AssignDefaultButton
//
// PURPOSE:    Copy the tag from the current control to the Window
//             entity
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
AssignDefaultButton( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vDialog;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( CompareAttributeToAttribute( vDialog, "Window", "DfltButton",
                                     vControl, "Control", "Tag" ) == 0 )
   {
      SetAttributeFromString( vDialog, "Window", "DfltButton", 0 );
   }
   else
   {
      SetAttributeFromAttribute( vDialog, "Window", "DfltButton",
                                 vControl, "Control", "Tag" );
   }

   return( 0 );
}

#define LIST_MAX_BLOB  2048

//./ ADD NAME=UPD_RadioPrebuildRadioGroup
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioPrebuildRadioGroup
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioPrebuildRadioGroup( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szIndicator[ 2 ];
   zPCHAR  pCtrlBOI;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vControl, "Control" );

   // Build RadioGroup and Radio entities in TZPNCTWO OI
   if ( CheckExistenceOfEntity( vWork, "RadioGroup" ) >= 0 )
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );

   CreateEntity( vWork, "RadioGroup", zPOS_AFTER );
   if ( CheckExistenceOfEntity( vControl, "CtrlCtrl" ) >= 0 )
   {
      do
      {
         CreateEntity( vWork, "Radio", zPOS_AFTER );
         // Set work attributes DefaultIndicator & Value from
         // Control.CtrlBOI.
         GetAddrForAttribute( &pCtrlBOI, vControl, "CtrlCtrl", "CtrlBOI" );
         if ( pCtrlBOI )
         {
            szIndicator[ 0 ] = pCtrlBOI[ 0 ];
            szIndicator[ 1 ] = 0;
            SetAttributeFromString( vWork, "Radio", "Value",
                                    pCtrlBOI + 1 );
         }
         else
         {
            szIndicator[ 0 ] = 0;
            SetAttributeFromString( vWork, "Radio", "Value", "" );
         }

         SetAttributeFromString( vWork, "Radio", "DefaultIndicator",
                                 szIndicator );
         SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                    vControl, "CtrlCtrl", "Tag" );
         SetAttributeFromAttribute( vWork, "Radio", "Text",
                                    vControl, "CtrlCtrl", "Text" );
      } while ( SetCursorNextEntity( vControl, "CtrlCtrl", "" ) >= 0 );
   }

   // Set WindowFlag to 'R' to indicate being in radiogroup update window.
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "R" );

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   return( 0 );
}

//./ ADD NAME=UPD_RadioSetDefaultButton
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioSetDefaultButton
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioSetDefaultButton( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zVIEW   vWork2;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWork, "Radio" ) >= 0 )
   {
      // First clear default
      CreateViewFromViewForTask( &vWork2, vWork, 0 );
      SetCursorFirstEntity( vWork2, "Radio", "" );
      do
      {
         SetAttributeFromString( vWork2, "Radio", "DefaultIndicator", "" );

      } while ( SetCursorNextEntity( vWork2, "Radio", "" ) >= 0 );

      DropView( vWork2 );

      // Now set the new default value and refresh ListBox control.
      SetAttributeFromString( vWork, "Radio", "DefaultIndicator", "Y" );
      RefreshCtrl( vSubtask, "ListBox1" );
   }

   return( 0 );
}

//./ ADD NAME=UPD_RadioClearDefaultButton
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioClearDefaultButton
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioClearDefaultButton( zVIEW vSubtask )
{
   zVIEW   vWork;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWork, "Radio" ) >= 0 )
   {
      SetCursorFirstEntity( vWork, "Radio", "" );
      do
      {
         SetAttributeFromString( vWork, "Radio", "DefaultIndicator", "" );
      }
      while ( SetCursorNextEntity( vWork, "Radio", "" ) >= 0 );
      RefreshCtrl( vSubtask, "ListBox1" );
   }

   return( 0 );
}

//./ ADD NAME=UPD_RadioUpdateSubControl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioUpdateSubControl
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioUpdateSubControl( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vWork;
   zCHAR szTag[ 34 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Position on vControl.CtrlCtrl by vWork.Radio position
   GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "Radio", "Tag" );
   SetCursorFirstEntityByString( vControl, "CtrlCtrl", "Tag", szTag, "" );
// SetEntityCursor( vControl, "CtrlCtrl", "Tag",
//                  zPOS_FIRST | zQUAL_ENTITYATTR | zQUAL_EQUAL,
//                  vWork, "Radio", "Tag", 0, "", "" );

   SetViewToSubobject( vControl, "CtrlCtrl" );
   fnPainterCall( zMSG_BUILDTZPNEVWO, vSubtask, 0, 0, 0 );
   return( 0 );
}

#define zRADIOBUTTON_CONTROL                1031L

//./ ADD NAME=UPD_RadioGroupOK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioGroupOK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioGroupOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szBlob[ 256 ];
   zLONG   lKey;
   zSHORT  nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", "" );
   if ( nRC >= zCURSOR_SET )
      SetViewToSubobject( vControl, "CtrlCtrl" );

   if ( SetCursorFirstEntity( vWork, "Radio", "" ) >= zCURSOR_SET )
   do
   {
      GetIntegerFromAttribute( &lKey, vControl, "ControlDef", "Key" );
      if ( lKey == zRADIOBUTTON_CONTROL )
      {
         SetAttributeFromAttribute( vControl, "Control", "Tag",
                                    vWork, "Radio", "Tag" );
         SetAttributeFromAttribute( vControl, "Control", "Text",
                                    vWork, "Radio", "Text" );

         // Set the DefaultIndicator & Value attributes from the Radio
         // entity in the work OI into Control.CtrlBOI.
         GetStringFromAttribute( szBlob, sizeof( szBlob ), vWork, "Radio", "DefaultIndicator" );
         GetStringFromAttribute( szBlob + 1, sizeof( szBlob ) - 1, vWork, "Radio", "Value" );
         SetAttributeFromBlob( vControl, "Control", "CtrlBOI",
                               szBlob, (zUSHORT) (zstrlen( szBlob + 1 ) + 2) );
      }

      SetCursorNextEntity( vWork, "Radio", "" );

   } while ( SetCursorNextEntity( vControl, "Control", "" ) >= 0 );

   if ( nRC >= zCURSOR_SET )
      ResetViewFromSubobject( vControl );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );
   DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

//./ ADD NAME=UPD_RadioGroupCancel
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioGroupCancel
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioGroupCancel( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );
   DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );
   CancelSubobject( vControl, "Control" );

   return( 0 );
}

//./ ADD NAME=UPD_RadioPrebuildSubControl
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioPrebuildSubControl
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioPrebuildSubControl( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zVIEW   vGroup;
   zPCHAR  pCtrlBOI;
   zPCHAR  pchTag;
   zCHAR   szIndicator[ 2 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // If RadioGroup entity exists, control has been passed from the
   // RadioGroup window.  Otherwise, control came directly from
   // the Painter.  In the latter case, create the RadioGroup and
   // Radio entities and set the flag to indicate control came from
   // the Painter.
   if ( CheckExistenceOfEntity( vWork, "RadioGroup" ) < 0 )
   {
      CreateViewFromViewForTask( &vGroup, vControl, 0 );
      ResetViewFromSubobject( vGroup );
      CreateEntity( vWork, "RadioGroup", zPOS_AFTER );
      SetCursorFirstEntity( vGroup, "CtrlCtrl", 0 );
      do
      {
         CreateEntity( vWork, "Radio", zPOS_AFTER );
         // Set work attributes DefaultIndicator & Value from
         // Control.CtrlBOI.
         GetAddrForAttribute( &pCtrlBOI, vGroup, "CtrlCtrl", "CtrlBOI" );
         if ( pCtrlBOI )
         {
            szIndicator[ 0 ] = pCtrlBOI[ 0 ];
            szIndicator[ 1 ] = 0;
            SetAttributeFromString( vWork, "Radio", "Value",
                                    pCtrlBOI + 1 );
         }
         else
         {
            szIndicator[ 0 ] = 0;
            SetAttributeFromString( vWork, "Radio", "Value", "" );
         }

         SetAttributeFromString( vWork, "Radio", "DefaultIndicator",
                                 szIndicator );
         SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                    vGroup, "CtrlCtrl", "Tag" );
         SetAttributeFromAttribute( vWork, "Radio", "Text",
                                    vGroup, "CtrlCtrl", "Text" );
      } while ( SetCursorNextEntity( vGroup, "CtrlCtrl", "" ) >= 0 );

      DropView( vGroup );
      GetAddrForAttribute( &pchTag, vControl, "Control", "Tag" );
      SetCursorFirstEntityByString( vWork, "Radio", "Tag", pchTag, 0 );
   }

   UPD_PUSH_Init( vSubtask );
   return( 0 );
}

//./ ADD NAME=UPD_RadioSubControlOK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioSubControlOK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioSubControlOK( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;
   zCHAR   szBlob[ 256 ];
   zPCHAR  szWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Note that the Radio control is not updated as a temporal
   // subobject as the corresponding window only has edit boxes,
   // where canceling mapping is all that is required to abort
   // the transaction.

   // Set the DefaultIndicator & Value attributes from the Radio
   // entity in the work OI into Control.CtrlBOI.
   GetStringFromAttribute( szBlob, sizeof( szBlob ), vWork, "Radio", "DefaultIndicator" );
   GetStringFromAttribute( szBlob + 1, sizeof( szBlob ) - 1, vWork, "Radio", "Value" );

   // Set the value in the Control entity.
   SetAttributeFromAttribute( vControl, "Control", "RadioOrCheckboxValue",
                              vWork,    "Radio",   "Value" );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   SetAttributeFromBlob( vControl, "Control", "CtrlBOI",
                         szBlob, (zUSHORT) (zstrlen( szBlob + 1 ) + 2) );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   // Delete the RadioGroup entity if control came directly from
   // the Painter.
   // Else, set Tag and Text values and reset the subobject.
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "R" ) != 0 )
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );
   else
   {
      SetAttributeFromAttribute( vWork, "Radio", "Tag",
                                 vControl, "Control", "Tag" );
      SetAttributeFromAttribute( vWork, "Radio", "Text",
                                 vControl, "Control", "Text" );
   }

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "R" ) != 0 )
   {
      // If we are returning to the painter, then we must turn off
      // the default indicator on other radio buttons if we are
      // now the default button
      if ( szBlob[ 0 ] == 'Y' )
      {
         CreateViewFromViewForTask( &vWork, vControl, 0 );
         while ( SetCursorPrevEntity( vWork, "Control", 0 ) >= zCURSOR_SET )
         {
            GetAddrForAttribute( &szWork, vWork, "Control", "CtrlBOI" );
            if ( szWork && szWork[ 0 ] == 'Y' )
               szWork[ 0 ] = 0;
         }

         SetViewFromView( vWork, vControl );
         while ( SetCursorNextEntity( vWork, "Control", 0 ) >= zCURSOR_SET )
         {
            GetAddrForAttribute( &szWork, vWork, "Control", "CtrlBOI" );
            if ( szWork && szWork[ 0 ] == 'Y' )
               szWork[ 0 ] = 0;
         }

         DropView( vWork );
      }

      // Tell the Painter to refresh the control.
      RepaintZeidonControl( vSubtask );
   }
   else
      ResetViewFromSubobject( vControl );

   return( 0 );
}

//./ ADD NAME=UPD_RadioSubControlCancel
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_RadioSubControlCancel
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_RadioSubControlCancel( zVIEW vSubtask )
{
   zVIEW   vControl;
   zVIEW   vWork;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Delete the RadioGroup entity if control came directly from the Painter.
   // Else, reset the subobject.
   if ( CompareAttributeToString( vWork, "TZPNCTWO", "WindowFlag", "R" ) != 0 )
      DeleteEntity( vWork, "RadioGroup", zREPOS_NEXT );

   CancelSubobject( vControl, "Control" );
   ResetViewFromSubobject( vControl );

   return( 0 );
}

#define zSS_HIDEROWHEADER              zCONTROL_SPECIFIC_1
#define zSS_HIDEGRIDLINES              zCONTROL_SPECIFIC_2
#define zSS_HIDEROWSELECTIONCOL        zCONTROL_SPECIFIC_3
#define zSS_NEWGRID                    zCONTROLX_SPECIFIC_1
#define zSS_SORTEDHEADERS              zCONTROLX_SPECIFIC_2
#define zSS_REMEMBERSORT               zCONTROLX_SPECIFIC_3
#define zSS_SORT_DISPLAY               zCONTROLX_SPECIFIC_4
#define zSS_SHOWVERTICALGRIDLINES      zCONTROLX_SPECIFIC_5
#define zSS_MULTISELECT                zCONTROLX_SPECIFIC_6
#define zSS_SCOPE_OI                   zCONTROLX_SPECIFIC_7

//./ ADD NAME=PrebuildSpreadSheetUpdate
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: PrebuildSpreadSheetUpdate
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PrebuildSpreadSheet( zVIEW vSubtask )
{
   zVIEW vDialogW;
   zVIEW vControl;
   zVIEW vScope;
   zVIEW vWork;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   // Name subtask so list subctl knows we exist
   SetNameForView( vSubtask, "UPD_SS", vSubtask, zLEVEL_TASK );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vControl, "Control" );

   // Create view name, TZPULL_Entity, for EntityList Combo box.
   SetNameForView( vControl, "TZPULL_Entity", vSubtask, zLEVEL_TASK );

   // Make sure we're positioned on first CtrlMap.
   SetCursorFirstEntity( vControl, "CtrlMap", "" );

   CreateViewFromViewForTask( &vScope, vControl, 0 );
   SetNameForView( vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // If CtrlMap entities exist, position scoping view to the second one.
   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) == zCURSOR_SET )
   {
      if ( SetCursorNextEntity( vScope, "CtrlMap", "" ) != zCURSOR_SET )
         CreateMetaEntity( vSubtask, vScope, "CtrlMap", zPOS_AFTER );
   }
   else
   // Create CtrlMap entities, since they don't already exist, and
   // position the scoping view to the second one.
   {
      CreateMetaEntity( vSubtask, vControl, "CtrlMap", zPOS_AFTER );
      SetViewFromView( vScope, vControl );
      CreateMetaEntity( vSubtask, vScope, "CtrlMap", zPOS_AFTER );
   }

   SetNameForView( vScope, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
   CreateMappingEntity( vSubtask );

   // Initialize the TZPNEVWO work object if it's empty.
   // The Painter normally does this, but sometimes it seems to
   // forget.  *** Eliminate and retest.
   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vWork, "Event" ) != zCURSOR_SET )
   {
      zVIEW vWork2;

      GetViewByName( &vWork2, "TZPACTLO", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntityByString( vWork2, "Control", "Tag", "ListBox", "" );
      do
      {
         CreateEntity( vWork, "Event", zPOS_AFTER );
         SetAttributeFromAttribute( vWork, "Event", "Tag",
                                    vWork2, "Event", "Tag" );
         SetAttributeFromAttribute( vWork, "Event", "Type",
                                    vWork2, "Event", "Type" );
         CreateEntity( vWork, "EventAct", zPOS_AFTER );

      } while ( SetCursorNextEntity( vWork2, "Event", "" ) == zCURSOR_SET );
   }

   // Set up the Event List Box Properly.
   fnSetupEventList( vSubtask, vControl );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   fnSetUpSEL_LOD( vControl, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   fnSetUpScopeListOI( vControl, vSubtask, "CtrlMapLOD_Entity" );

   return( 0 );
}

//./ ADD NAME=PostbuildSpreadSheetUpdate
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      PostbuildSpreadSheetUpdate
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
PostbuildSpreadSheet( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;
   zLONG  lSubtype;
   zLONG  lSubtypeX;
   zLONG  lSplitRow;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Set the flags for the SpreadSheet that are not part of "Common"
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   GetIntegerFromAttribute( &lSubtypeX, vControl, "Control", "ExtendedStyle" );
   if ( lSubtypeX & zSS_NEWGRID )
      SetCtrlState( vSubtask, "NewGrid",
                    zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtypeX & zSS_MULTISELECT )
      SetCtrlState( vSubtask, "MultiSelect",
                    zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtypeX & zSS_SCOPE_OI )
      SetCtrlState( vSubtask, "ScopeOI",
                    zCONTROL_STATUS_CHECKED, TRUE );

   if ( (lSubtype & zSS_HIDEROWSELECTIONCOL) == 0 )
      SetCtrlState( vSubtask, "RowSelectionColumn",
                    zCONTROL_STATUS_CHECKED, TRUE );

   if ( lSubtype & zSS_HIDEROWHEADER )
   {
      SetCtrlState( vSubtask, "NoHeader",
                    zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   if ( lSubtypeX & zSS_REMEMBERSORT )
   {
      SetCtrlState( vSubtask, "AutoSortMemory",
                    zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   if ( lSubtypeX & zSS_SORTEDHEADERS )
   {
      SetCtrlState( vSubtask, "AutoSort",
                    zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "HeaderButtons",
                    zCONTROL_STATUS_CHECKED, TRUE );
   }

   // Because of settings for the original SpreadSheet cannot be
   // changed, the following code is a bit convoluted.
   // If both HideGridLines and ShowVerticalGridLines are set, show
   // vertical gridlines only.
   // If HideGridLines is set and ShowVerticalGridLines is NOT set, hide
   // both gridlines.
   // If HideGridLines is NOT set and ShowVerticalGridLines is set, show
   // horizontal gridlines only.
   // If neither HideGridLines nor ShowVerticalGridLines are set, show
   // both gridlines.

   if ( lSubtype & zSS_HIDEGRIDLINES )
   {
      if ( lSubtypeX & zSS_SHOWVERTICALGRIDLINES )
      {
         SetCtrlState( vSubtask, "VerticalGridLines",
                       zCONTROL_STATUS_CHECKED, TRUE );
      }
   }
   else
   {
      SetCtrlState( vSubtask, "HorizontalGridLines",
                    zCONTROL_STATUS_CHECKED, TRUE );
      if ( (lSubtypeX & zSS_SHOWVERTICALGRIDLINES) == 0 )
      {
         SetCtrlState( vSubtask, "VerticalGridLines",
                       zCONTROL_STATUS_CHECKED, TRUE );
      }
   }

   GetIntegerFromAttribute( &lSplitRow, vControl, "Control", "OptionFlags" );
   if ( lSplitRow )
   {
      zCHAR  szNumber[ 16 ];
      zLONG  lSplitRowAtCol = zLOUSHORT( lSplitRow );
      zLONG  lIndent2ndLine = zHIUSHORT( lSplitRow );

      zltoa( lSplitRowAtCol, szNumber, sizeof( szNumber ) );
      SetCtrlText( vSubtask, "SplitRowAtCol", szNumber );
      zltoa( lIndent2ndLine, szNumber, sizeof( szNumber ) );
      SetCtrlText( vSubtask, "Indent2ndLine", szNumber );
   }

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "Tag", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "View", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NewGrid", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Show", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Header", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MultiSelect", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SplitRowAtCol", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Indent2ndLine", zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Go to build list of Web Potential Control Properties.
   oTZWDLGSO_BuildWebCtrlPropOpts( vTZWINDOWL, vTZWINDOWL, "Dialog", "wWebGridControlProperties" );

   return( 0 );
}

//./ ADD NAME=UPD_SS_OK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_SS_OK
//
// PURPOSE:    SS Control OK that Accepts Control & issues repaint.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_SS_OK( zVIEW vSubtask )
{
   zVIEW    vControl;
   zLONG    lSubtype;
   zLONG    lSubtypeX;
   zLONG    lSplitRowAtCol;
   zLONG    lIndent2ndLine;
   zCHAR    szWork[ 16 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   GetIntegerFromAttribute( &lSubtypeX, vControl, "Control", "ExtendedStyle" );

   if ( GetCtrlState( vSubtask, "NewGrid", zCONTROL_STATUS_CHECKED ) )
      lSubtypeX |= zSS_NEWGRID;
   else
      lSubtypeX &= ~zSS_NEWGRID;

   if ( GetCtrlState( vSubtask, "MultiSelect", zCONTROL_STATUS_CHECKED ) )
      lSubtypeX |= zSS_MULTISELECT;
   else
      lSubtypeX &= ~zSS_MULTISELECT;

   if ( GetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED ) )
      lSubtypeX |= zSS_SCOPE_OI;
   else
      lSubtypeX &= ~zSS_SCOPE_OI;

   if ( GetCtrlState( vSubtask, "RowSelectionColumn", zCONTROL_STATUS_CHECKED ) )
      lSubtype &= ~zSS_HIDEROWSELECTIONCOL;
   else
      lSubtype |= zSS_HIDEROWSELECTIONCOL;

   GetCtrlText( vSubtask, "SplitRowAtCol", szWork, sizeof( szWork ) );
   lSplitRowAtCol = zatol( szWork );
   GetCtrlText( vSubtask, "Indent2ndLine", szWork, sizeof( szWork ) );
   lIndent2ndLine = zatol( szWork );
   SetAttributeFromInteger( vControl, "Control", "OptionFlags",
                            zMAKELONG( lSplitRowAtCol, lIndent2ndLine ) );

   // Because of settings for the original SpreadSheet cannot be
   // changed, the following code is a bit convoluted.
   // If neither HideGridLines nor ShowVerticalGridLines are set, show
   // both gridlines.
   // If both HideGridLines and ShowVerticalGridLines are set, show
   // vertical gridlines only.
   // If HideGridLines is NOT set and ShowVerticalGridLines is set, show
   // horizontal gridlines only.
   // If HideGridLines is set and ShowVerticalGridLines is NOT set, hide
   // both gridlines.

   lSubtype &= ~zSS_HIDEGRIDLINES;
   lSubtypeX &= ~zSS_SHOWVERTICALGRIDLINES;
   if ( GetCtrlState( vSubtask, "VerticalGridLines",
                      zCONTROL_STATUS_CHECKED ) )
   {
      if ( GetCtrlState( vSubtask, "HorizontalGridLines",
                         zCONTROL_STATUS_CHECKED ) )
      {
         // neither are set
      }
      else
      {
         lSubtype |= zSS_HIDEGRIDLINES;
         lSubtypeX |= zSS_SHOWVERTICALGRIDLINES;
      }
   }
   else
   {
      if ( GetCtrlState( vSubtask, "HorizontalGridLines",
                         zCONTROL_STATUS_CHECKED ) )
      {
         lSubtypeX |= zSS_SHOWVERTICALGRIDLINES;
      }
      else
      {
         lSubtype |= zSS_HIDEGRIDLINES;
      }
   }

   lSubtype &= ~zSS_HIDEROWHEADER;
   lSubtypeX &= ~zSS_SORTEDHEADERS;
   lSubtypeX &= ~zSS_REMEMBERSORT;
   if ( GetCtrlState( vSubtask, "NoHeader", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zSS_HIDEROWHEADER;
   else
   if ( GetCtrlState( vSubtask, "AutoSort", zCONTROL_STATUS_CHECKED ) )
      lSubtypeX |= zSS_SORTEDHEADERS;
   else
   if ( GetCtrlState( vSubtask, "AutoSortMemory", zCONTROL_STATUS_CHECKED ) )
      lSubtypeX |= zSS_REMEMBERSORT;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );
   SetAttributeFromInteger( vControl, "Control", "ExtendedStyle", lSubtypeX );
   return( UPD_GeneralOK( vSubtask ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_SSCOMBO_Init
//
// PURPOSE:  This function:
//           1.  Sets up the TZPNCTWO work object
//           2.  Creates the first CtrlMap entity instance, if necessary
//           3.  Sets up the work ComboBox entity
//           4.  Sets up the Domain Context mapping
//           5.  Sets up the Event ListBox properly
//           6.  Switches to either UPD_COMBO_D or UPD_COMBO_S if
//               a Subtype has been previously defined.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_SSCOMBO_Init( zVIEW vSubtask )
{
   zVIEW    vControl;
   zVIEW    vWork;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure ComboBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "ComboBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "ComboBox", zPOS_BEFORE );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vControl, "Control" );

   // Create the first CtrlMap entity instance, if necessary.
   if ( CheckExistenceOfEntity( vControl, "CtrlMap" ) != zCURSOR_SET )
      CreateEntity( vControl, "CtrlMap", zPOS_BEFORE );

   // Set ComboBox attribute values from Control.CtrlBOI
   SetEntityAttributesFromBlob( vWork, "ComboBox",
                                vControl, "Control", "CtrlBOI" );

   SetAttributeFromInteger( vWork, "ComboBox", "Subtype", 0 );
   SetAttributeFromInteger( vWork, "TZPNCTWO",
                            "TempCurrentComboSubtype", 0 );

   // Set up the Event List Box Properly.
   fnSetupEventList( vSubtask, vControl );

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

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

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnSearchForEntityRecursively
//
// PURPOSE:  This function searches the LOD object recursively on
//           LOD_EntityParent, looking for a match on ZKey.  When it
//           finds it, it saves the view under view name ?????
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSearchForEntityRecursively( zVIEW vLOD,
                               zLONG lZKey,
                               zPSHORT pFoundFlag )
{
   if ( CheckExistenceOfEntity( vLOD, "LOD_EntityChild" ) == zCURSOR_SET )
   {
      do
      {
         if ( CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                         "ZKey", lZKey ) != 0 )
         {
            SetViewToSubobject( vLOD, "LOD_EntityChild" );
            fnSearchForEntityRecursively( vLOD, lZKey, pFoundFlag );
            if ( *pFoundFlag == 0 )
               ResetViewFromSubobject( vLOD );
         }
      }
      while ( (CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                          "ZKey", lZKey ) != 0) &&
            (*pFoundFlag == 0) &&
            (SetCursorNextEntity( vLOD,
                                   "LOD_EntityChild", "" ) == zCURSOR_SET) );

      if ( CompareAttributeToInteger( vLOD, "LOD_EntityChild",
                                      "ZKey", lZKey ) == 0 )
         *pFoundFlag = 1;    // Indicate a match was found
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnSetUpSEL_LOD
//
// PURPOSE:  This function sets up the SEL_LOD OI for Entity or
//           Attribute mapping.  If a current CtrlMapView entity
//           instance exists, it positions on the related ViewObjRef
//           entity and activates the corresponding LOD.  Otherwise
//           it activates an empty OI.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetUpSEL_LOD( zVIEW vControl,
                zVIEW vDialogW,
                zVIEW vSubtask,
                zCPCHAR cpcMapView )
{
   zVIEW vLOD = (zVIEW) SetUpMapLOD( vSubtask, vDialogW,
                                     vControl, (zPCHAR) cpcMapView );
   if ( vLOD == 0 )
      ActivateEmptyObjectInstance( &vLOD, "TZZOLODO", vSubtask, 0 );

   SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnSetUpScopeListOI
//
// PURPOSE:  This function sets up the Scoping List OI for use by
//           a Combo Box for listing potential Scoping entities.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetUpScopeListOI( zVIEW vControl,
                    zVIEW vSubtask,
                    zCPCHAR szMapLOD_Entity )
{
   zVIEW    vLOD;
   zVIEW    vScopeL;
   zLONG    lZKey;
   zSHORT   nFoundFlag;

   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   ActivateEmptyObjectInstance( &vScopeL, "TZPNCTWO", vSubtask, 0 );
   SetNameForView( vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   CreateEntity( vScopeL, "TZPNCTWO", zPOS_BEFORE );

   // szMapLOD_Entity is CtrlMapView, if this was called for Control
   // mapping, or  ActMapLOD_Entity, if this was called for Action mapping.
   // If szMapLOD_Entity exists, set up the list to reflect that
   // entity value.  Begin looking recursively through all entities
   // if the root entity is not the one mapped.
   if ( CheckExistenceOfEntity( vControl, szMapLOD_Entity ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vControl, szMapLOD_Entity, "ZKey" );
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent",
                                      "ZKey", lZKey ) != 0 )
      {
         nFoundFlag = 0;
         fnSearchForEntityRecursively( vLOD, lZKey, &nFoundFlag );
         do
         {
            CreateEntity( vScopeL, "ScopeEntity", zPOS_BEFORE );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity", "Name",
                                       vLOD, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity",
                                       "LOD_EntityZKey", vLOD,
                                       "LOD_EntityParent", "ZKey" );

         } while ( ResetViewFromSubobject( vLOD ) == 0 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnDeleteAction
//
// PURPOSE:  This function deletes an Action if there are not
//           Control, Window Event or Option relationships to it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnDeleteAction( zVIEW    vSubtask,
                zVIEW    vDialogW )
{
   if ( CheckExistenceOfEntity( vDialogW, "ActWndEvent" ) == zCURSOR_SET ||
        CheckExistenceOfEntity( vDialogW, "ActEvent" ) == zCURSOR_SET )
   {
      zCHAR  szMsg[ 256 ];
      zCHAR  szTag[ 33 ];
      zCHAR  szAction[ 33 ];

      GetStringFromAttribute( szAction, sizeof( szAction ), vDialogW, "Action", "Tag" );

      if ( CheckExistenceOfEntity( vDialogW, "ActEvent" ) == zCURSOR_SET &&
           CheckExistenceOfEntity( vDialogW, "ActCtrl" ) == zCURSOR_SET )
      {
         GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "ActCtrl", "Tag" );
         sprintf_s( szMsg, sizeof( szMsg ),
                   "Action '%s' is used by Control Events '%s'. "
                      "Delete request denied.",
                   szAction, szTag );
      }
      else
      if ( CheckExistenceOfEntity( vDialogW, "ActWndEvent" ) == zCURSOR_SET )
      {
         GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "Window", "Tag" );
         sprintf_s( szMsg, sizeof( szMsg ),
                   "Action '%s' is used by Window Events '%s'. "
                      "Delete request denied.",
                   szAction, szTag );
      }
      else
      {
         strcpy_s( szTag, sizeof( szTag ), "Unknown???" );
         sprintf_s( szMsg, sizeof( szMsg ),
                   "Action '%s' is used by with existing Events '%s'. "
                      "Delete request denied.",
                   szAction, szTag );
      }

      MessageSend( vSubtask, "PN00101", "Dialog Maintenance", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
   if ( CheckExistenceOfEntity( vDialogW, "ActOpt" ) == zCURSOR_SET )
   {
      zCHAR  szMsg[ 256 ];
      zCHAR  szTag[ 33 ];
      zCHAR  szAction[ 33 ];

      GetStringFromAttribute( szAction, sizeof( szAction ), vDialogW, "Action", "Tag" );

      GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "ActOpt", "Tag" );
      sprintf_s( szMsg, sizeof( szMsg ),
                "Action '%s' is used by Options '%s'. Delete request denied.",
                szAction, szTag );
      MessageSend( vSubtask, "PN00102", "Dialog Maintenance", szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }
   else
      DeleteEntity( vDialogW, "Action", zREPOS_NONE );

   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_AddField
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_AddField
//
// PURPOSE:    To Add a list control to a list box
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_AddField( zVIEW vSubtask )
{
   zVIEW vWork;
   zLONG lMaxX;
   zLONG lPosX;
   zLONG lSizeX;
   zVIEW vDialog;
   zVIEW vTZPESRCO;
   zSHORT i;
   zCHAR sWork1[ 6 ];
   zCHAR sWork2[ 14 ];

   // Get the view to the list control
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Get the view to the presentation environment object
   GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK );
   if ( SetCursorFirstEntityByString( vTZPESRCO, "ControlDef", "Tag",
                                      "ListCtl", 0 ) < zCURSOR_SET )
   {
      return( -1 );
   }

   lMaxX = 0;
   i = 1;
   if ( SetCursorFirstEntity( vDialog, "CtrlCtrl", 0 ) == zCURSOR_SET )
   do
   {
      i = i + 1;
      GetIntegerFromAttribute( &lPosX, vDialog, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lSizeX, vDialog, "CtrlCtrl", "SZDLG_X" );
      if ( ( lPosX + lSizeX ) > lMaxX )
         lMaxX = lPosX + lSizeX;

   } while ( SetCursorNextEntity( vDialog, "CtrlCtrl", 0 ) == zCURSOR_SET );

   CreateTemporalMetaEntity( vSubtask, vDialog, "CtrlCtrl", zPOS_AFTER );
   SetViewToSubobject( vDialog, "CtrlCtrl" );
// CreateTemporalMetaEntity( vSubtask, vDialog, "Control", zPOS_AFTER );
   sWork1[ 0 ] = '0' + i % 10;
   sWork1[ 1 ] = 0;
   TraceLineS( "*********sWork1", sWork1 );
   strcpy_s( sWork2, sizeof( sWork2 ), "ListCtl" );
   strcat_s( sWork2, sizeof( sWork2 ), sWork1 );
   SetAttributeFromString( vDialog, "Control", "Tag", sWork2 );
   SetAttributeFromInteger( vDialog, "Control", "Type", 1081 );
   SetAttributeFromInteger( vDialog, "Control", "PSDLG_X", lMaxX );
   SetAttributeFromInteger( vDialog, "Control", "SZDLG_X", 60 );
   SetAttributeFromInteger( vDialog, "Control", "PSDLG_Y", 0 );
   SetAttributeFromInteger( vDialog, "Control", "SZDLG_Y", 11 );

   CreateMetaEntity( vSubtask, vDialog, "CtrlMap", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vDialog, "ControlDef",
                                  vTZPESRCO, "ControlDef", zPOS_AFTER );
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "A" );
   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_UpdateField
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_UpdateField
//
// PURPOSE:    To Add a list control to a list box
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_UpdateField( zVIEW vSubtask )
{
   zVIEW vDialog;

   // Get the view to the list control
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );

   SetViewToSubobject( vDialog, "CtrlCtrl" );

   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_Init
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_Init
//
// PURPOSE:    Set up the CtrlMap entity if necessary and prepare
//             for Context mapping.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_Init( zVIEW vSubtask )
{
   zCHAR  szMapFlag[ 34 ];
   zCHAR  szBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
   zULONG ulLth = sizeof( szBlob );
   zVIEW  vControl;
   zVIEW  vWork;
   zLONG  lFlag;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // If WindowStyle != "L" then we are entering from the painter screen
   // and so ListCtrl already exists so create the temporal version.
   // Otherwise, we are creating ListCtrl from the listbox update window,
   // and CreateTemporalMetaEntity was used, so we don't want to create
   // another temporal version.
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "A" ) != 0 )
   {
      CreateTemporalSubobjectVersion( vControl, "Control" );
   }

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
//              lFlag      - 0 ==> do not reset ctrl width and height
//                           1 ==> set ctrl width to column size
//                           2 ==> set ctrl height to column size
//                           4 ==> set ctrl width to column size (do not set
//                                 smaller than ctrl's design width).
//                           8 ==> set ctrl height to column size (do not set
//                                 smaller than ctrl's design height).

   lFlag = *((zPSHORT) (szBlob + 2));
   SetCtrlTranslationList( vSubtask, "SubFlag",
                           "0;;1;Width;2;Height;3;Width/Height;"
                           "4;Minimum Width;8;Minimum Height;"
                           "6;Minimum Width+Height;9;Width+Minimum Height;"
                           "12;Minimum Width/Minimum Height", 1 );
   zltoa( lFlag, szMapFlag, sizeof( szMapFlag ) );
   TranslateCtrlValue( vSubtask, "SubFlag", 0, TRUE,
                       szMapFlag, sizeof( szMapFlag ) );
   CB_SetData( vSubtask, "SubFlag", szMapFlag,
               ";Width;Height;Width/Height;"
               "Minimum Width;Minimum Height;"
               "Minimum Width+Height;Width+Minimum Height;"
               "Minimum Width/Minimum Height", 0 );

   if ( ulLth == sizeof( szBlob ) )
   {
      SetCtrlText( vSubtask, "SubControl", szBlob + (3 * sizeof( zLONG )) );
   }

   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_OK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_OK
//
// PURPOSE:    This function:
//             1. Sets up mapping for the LISTBOX control itself, if
//                such mapping doesn't exist.
//             2. Saves the Context Name.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_OK( zVIEW vSubtask )
{
   zCHAR  szMapFlag[ 34 ];
   zCHAR  szBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
   zULONG ulLth = sizeof( szBlob );
   zVIEW  vDialogW;
   zVIEW  vControl;
   zVIEW  vDialogP;
   zVIEW  vWork;
   zVIEW  vLOD;
   zVIEW  vCM_List;
   zLONG  lZKey;
   zSHORT nRC;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Store the blob.
   zmemset( szBlob, 0, sizeof( szBlob ) );
   GetCtrlText( vSubtask, "SubControl", szBlob + (3 * sizeof( zLONG )), 33 );
   if ( szBlob[ 3 * sizeof( zLONG ) ] == 0 )
      ulLth = 0;

   GetCtrlText( vSubtask, "SubFlag", szMapFlag, sizeof( szMapFlag ) );
   TranslateCtrlValue( vSubtask, "SubFlag", 0, FALSE,
                       szMapFlag, sizeof( szMapFlag ) );

   *((zPSHORT) (szBlob + 2)) = (zSHORT) zatol( szMapFlag );
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vDialogP, vControl, 0 );

   // Exclude the ListCtrlMap subobjects used for map selection.
   for ( nRC = SetCursorFirstEntity( vControl, "ListCtrlMap", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vControl, "ListCtrlMap", "" ) )
   {
      ExcludeEntity( vControl, "ListCtrlMap", zREPOS_NONE );
   }

   // Reset view to parent list control
   ResetViewFromSubobject( vDialogP );

   // If there is no mapping on the LISTBOX control(the parent), use
   // the mapping from the last LISTCTRL.
   if ( CheckExistenceOfEntity( vDialogP, "CtrlMapView" ) != zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vControl, "CtrlMapView", "ZKey" );
         SetCursorFirstEntityByInteger( vDialogW, "ViewObjRef",
                                        "ZKey", lZKey, "" );
         IncludeSubobjectFromSubobject( vDialogP, "CtrlMapView",
                                        vDialogW, "ViewObjRef", zPOS_AFTER );
         GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
         RetrieveViewForMetaList( vSubtask,
                                  &vCM_List, zREFER_LOD_META );
         SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                        "CPLR_ZKey", lZKey, "" );
         ActivateMetaOI( vSubtask, &vLOD,
                         vCM_List, zREFER_LOD_META, 0 );
         SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
         GetIntegerFromAttribute( &lZKey, vControl,
                                  "CtrlMapRelatedEntity", "ZKey" );
         SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                        "ZKey", lZKey, "" );
         IncludeSubobjectFromSubobject( vDialogP, "CtrlMapLOD_Entity",
                                        vLOD, "LOD_Entity", zPOS_AFTER );
      }
   }

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required (for the ListBox window).
   fnSetUpSEL_LOD( vDialogP, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box(for the ListBox window).
   fnSetUpScopeListOI( vDialogP, vSubtask, "CtrlMapLOD_Entity" );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   ResetViewFromSubobject( vControl );
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "L" ) != 0 )
   {
      // Tell the Painter to refresh the control.
      RepaintZeidonControl( vSubtask );
   }
   else
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                         zWAB_ProcessImmediateReturn, 0, 0 );

   // Delete the WebControlPropertyOption entries.
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_Cancel
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_Cancel
//
// PURPOSE:    To End list sub-control processing
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_Cancel( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zSHORT nRC;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Cancel subobject and reset view to parent after removing ListCtrlMap entries..
   CancelSubobject( vDialogC, "Control" );

   // Exclude the ListCtrlMap subobjects used for map selection.
   for ( nRC = SetCursorFirstEntity( vDialogC, "ListCtrlMap", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogC, "ListCtrlMap", "" ) )
   {
      ExcludeEntity( vDialogC, "ListCtrlMap", zREPOS_NONE );
   }

   nRC = ResetViewFromSubobject( vDialogC );

   // Delete the WebControlPropertyOption entries.
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   return( 0 );
}

//./ ADD NAME=UPD_LISTCTRL_New
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_LISTCTRL_New
//
// PURPOSE:    To Add a list control to a list box
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_LISTCTRL_New( zVIEW vSubtask )
{
   zVIEW vDialog;
   zVIEW vControlWork;
   zLONG lMaxX, lPosX, lSizeX;

   // Get the view to the list control
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vControlWork, vDialog, 0 );
   GetIntegerFromAttribute( &lPosX, vControlWork, "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lSizeX, vControlWork, "Control", "SZDLG_X" );
   lMaxX = lPosX + lSizeX;
   while ( SetCursorNextEntity( vControlWork, "Control", 0 ) == zCURSOR_SET )
   {
      AddToAttributeFromInteger( vControlWork, "Control", "PSDLG_X", 60 );
   }

   DropView( vControlWork );
   CreateMetaEntity( vSubtask, vDialog, "Control", zPOS_AFTER );
   SetAttributeFromString( vDialog, "Control", "Tag", "ListCtl" );
   SetAttributeFromInteger( vDialog, "Control", "Type", 1081 );
   SetAttributeFromInteger( vDialog, "Control", "PSDLG_X", lMaxX );
   SetAttributeFromInteger( vDialog, "Control", "SZDLG_X", 60 );
   SetAttributeFromInteger( vDialog, "Control", "PSDLG_Y", 0 );
   SetAttributeFromInteger( vDialog, "Control", "SZDLG_Y", 11 );
   CreateMetaEntity( vSubtask, vDialog, "CtrlMap", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_TEXT_DisableControls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_TEXT_DisableControls( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "VerticalText", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Highlight", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UseToolTip", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ToolTip", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Horizontal", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Vertical", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SunkenBorder", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "BevelLine", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "HighlightWithTip", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MouseClickOnly", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UseLeftMouse", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Rotation", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ActionDetail", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NewAction", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "RemoveAction", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ActionDetail", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "HyperText", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Apply", zCONTROL_STATUS_ENABLED, FALSE );

      SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Size", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__ColorTextFore", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "__Font", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Escapement", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "CenterH", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Right", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Top", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "CenterV", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Bottom", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // UPD_TEXT_DisableControls

/////////////////////////////////////////////////////////////////////////////
//
// When a property changes, render the change in the displayed test area.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnLoadLabelData( zVIEW vSubtask, LABEL_DATA *pLD )
{
   zCHAR   szWork[ 32 ];
   zULONG  ulLth;
   zCPCHAR cpc;

   // Initialize the blob.
   zmemset( pLD, 0, sizeof( LABEL_DATA ) );

   // Get all the current data.
   pLD->chBold = (char) GetCtrlState( vSubtask, "Bold",
                                      zCONTROL_STATUS_CHECKED );
   pLD->chItalic = (char) GetCtrlState( vSubtask, "Italic",
                                        zCONTROL_STATUS_CHECKED );
   pLD->chStrikeout = (char) GetCtrlState( vSubtask, "Strikeout",
                                           zCONTROL_STATUS_CHECKED );
   pLD->chUnderline = (char) GetCtrlState( vSubtask, "Underline",
                                           zCONTROL_STATUS_CHECKED );
   pLD->chVerticalText = (char) GetCtrlState( vSubtask, "VerticalText",
                                              zCONTROL_STATUS_CHECKED );
   GetCtrlText( vSubtask, "Size", szWork, sizeof( szWork ) );
   pLD->lPointSize = atol( szWork );
   GetCtrlProperty( vSubtask, "__ColorTextFore",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &(pLD->lTextColor), 0, 0 );
   GetCtrlProperty( vSubtask, "__ColorTextBack",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &(pLD->lTextBkColor), 0, 0 );
   GetCtrlProperty( vSubtask, "__ColorBackground",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &(pLD->lBackgroundColor), 0, 0 );
   pLD->chHighlight = (char) (GetCtrlState( vSubtask, "Highlight",
                                            zCONTROL_STATUS_CHECKED ) +
                              (2 * GetCtrlState( vSubtask,
                                                 "HighlightWithTip",
                                                 zCONTROL_STATUS_CHECKED )));
   GetCtrlProperty( vSubtask, "__ColorHighlight",
                    zCONTROL_PROPERTY_INTEGER_DATA,
                    (zPULONG) &(pLD->lHighlightColor), 0, 0 );
   ulLth = sizeof( pLD->szFontName );
   GetCtrlProperty( vSubtask, "__Font", zCONTROL_PROPERTY_STRING_DATA,
                    &ulLth, pLD->szFontName, sizeof( pLD->szFontName ) );
   pLD->chBorder = (char) (GetCtrlState( vSubtask, "Border",
                                         zCONTROL_STATUS_CHECKED ) +
                           (2 * GetCtrlState( vSubtask, "SunkenBorder",
                                              zCONTROL_STATUS_CHECKED )));
   pLD->chBeveledLine = (char) GetCtrlState( vSubtask, "BevelLine",
                                             zCONTROL_STATUS_CHECKED );
   pLD->chUseToolTip = (char) (GetCtrlState( vSubtask, "UseToolTip",
                                             zCONTROL_STATUS_CHECKED ) +
                               (2 * GetCtrlState( vSubtask, "MouseClickOnly",
                                                  zCONTROL_STATUS_CHECKED )) +
                               (4 * GetCtrlState( vSubtask, "UseLeftMouse",
                                                  zCONTROL_STATUS_CHECKED )));
   GetCtrlText( vSubtask, "ToolTip",
                pLD->szToolTip, sizeof( pLD->szToolTip ) );


   CB_GetSelectedString( vSubtask, "Horizontal", szWork, sizeof( szWork ) );
   if ( zstrcmp( szWork, "Right" ) == 0 )
      pLD->chBottomRight = 1;
   else
   if ( zstrcmp( szWork, "Center" ) == 0 )
      pLD->chCenter = 1;
   else
// if ( zstrcmp( szWork, "Left" ) == 0 )
      pLD->chTopLeft = 1;

   CB_GetSelectedString( vSubtask, "Vertical", szWork, sizeof( szWork ) );
   if ( zstrcmp( szWork, "Bottom" ) == 0 )
      pLD->chBottomRight |= 2;
   else
   if ( zstrcmp( szWork, "Center" ) == 0 )
      pLD->chCenter |= 2;
   else
// if ( zstrcmp( szWork, "Top" ) == 0 )
      pLD->chTopLeft |= 2;

   cpc = (zCPCHAR) GetCtrlState( vSubtask, "Rotation",
                                 zCONTROL_STATUS_CHECKED );
   if ( cpc && ((zLONG) cpc != -1) )
   {
      if ( *cpc == '5' )
      {
         GetCtrlText( vSubtask, "Escapement", szWork, sizeof( szWork ) );
         pLD->lEscapement = zatol( szWork );
         SetCtrlState( vSubtask, "Escapement",
                       zCONTROL_STATUS_ENABLED, TRUE );
      }
      else
      {
         if ( *cpc == '4' )
            pLD->lEscapement = 2700;
         else
         if ( *cpc == '3' )
            pLD->lEscapement = 1800;
         else
         if ( *cpc == '2' )
            pLD->lEscapement = 900;
         else
//       if ( *cpc == '1' )
            pLD->lEscapement = 0;

         SetCtrlState( vSubtask, "Escapement",
                       zCONTROL_STATUS_ENABLED, FALSE );
      }

      zltoa( pLD->lEscapement, szWork, sizeof( szWork ) );
      SetCtrlText( vSubtask, "Escapement", szWork );
   }

   pLD->chHyperText = (char) GetCtrlState( vSubtask, "HyperText",
                                           zCONTROL_STATUS_CHECKED );
   if ( pLD->chHyperText )
   {
      SetCtrlState( vSubtask, "ActionDetail", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "NewAction", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "RemoveAction", zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "ActionDetail", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NewAction", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "RemoveAction", zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Unused in online text.
   pLD->chMultiLine = 0;
   pLD->lBorderStyle = 0;
   pLD->lBorderColor = 0;
   pLD->lType = 0;

   if ( (pLD->chCenter & 2) || (pLD->chBottomRight & 2) ||
        (pLD->lTextBkColor & 0x00FFFFFF) ||
        (pLD->lBackgroundColor & 0x00FFFFFF) ||
        (pLD->lHighlightColor & 0x00FFFFFF) || pLD->chBorder || pLD->chBeveledLine ||
        pLD->chVerticalText || pLD->chHighlight ||
        pLD->chHyperText || (pLD->chUseToolTip && pLD->szToolTip[ 0 ]) )
   {
      return( 2 );
   }
   else
   if ( pLD->chBold || pLD->chItalic || pLD->chStrikeout ||
        pLD->chUnderline || pLD->lEscapement ||
        pLD->lPointSize || (pLD->lTextColor & 0x00FFFFFF) ||
        pLD->chCenter || pLD->chBottomRight || pLD->szFontName[ 0 ] )
   {
      return( 1 );
   }

   return( 0 );
}

#if 1
void
DisplayCurrentTime( zCPCHAR cpcMsg )
{
// zULONG ulTime = GetCurrentTime( );
// TraceLine( "%s:  Time: %d", cpcMsg, ulTime );
   DWORD dwTime = GetTickCount( );
   TraceLine( "%s:  Time: %d", cpcMsg, dwTime );
}
#else
#define DisplayCurrentTime( a )
#endif

/////////////////////////////////////////////////////////////////////////////
//
// When a property changes, render the change in the displayed test area.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_TEXT_OnChange( zVIEW vSubtask )
{
   LABEL_DATA ld;
   zVIEW   vControl;
   zCHAR   szText[ 256 ];

   // Get the view.
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   ld.lType = fnLoadLabelData( vSubtask, &ld );
   if ( ld.lType == 1 && (ld.lEscapement % 900) == 0 )
      ld.lType = 2;

   GetCtrlText( vSubtask, "Text", szText, sizeof( szText ) - 1 );
   SetCtrlText( vSubtask, "TestIt", szText );
   DisplayCurrentTime( "UPD_TEXT_OnChange Before SetTextProperties" );
   TX_SetTextProperties( vSubtask, "TestIt", &ld );
   DisplayCurrentTime( "UPD_TEXT_OnChange After SetTextProperties" );

   return( 0 );
}

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
//  not used   12   MultiLine      1-on ... 0-off
//  Justification
//             13   Top/Left       1-left ... 2-top ... 0-off
//             14   Center         1-horiz ... 2-vert ... 0-off
//             15   Bottom/Right   1-on ... 2-bottom ... 0-off
//  not used 16-19  BorderStyle (0-solid 1-dash 2-dot 3-dashdot 4-dashdotdot)
//               (second byte 1-top | 2-left | 4-bottom | 8-right)
//               (high order byte is line width)
//  not used 20-23  BorderColor    unsigned long RGB color value
//           24-27  Escapement
//           28-31  lType          // reserved
//           32-63  Font face name
//           64-83  *** reserved ***
//  New Label
//           84-87  Text Bk Color     unsigned long RGB color value
//           88-91  Background Color  unsigned long RGB color value
//           92-95  Highlight Color   unsigned long RGB color value
//             96   Border/Sunken  1-Border, 2-Sunken Border
//             97   Beveled Line   1-on ... 0-off
//             98   Vertical Text  1-on ... 0-off
//             99   Highlight      1-on ... 2-only with ToolTip ... 0-off
//          100-355  ToolTip Text (max length 256)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_PostBuild( zVIEW vSubtask )
{
   zVIEW   vDialogW;
   LABEL_DATA ld;
   zPCHAR  pchHorizontal = "Left";
   zPCHAR  pchVertical = "Top";
   zCHAR   szWork[ 32 ];
   zVIEW   vControl;
// zLONG   lBorderStyle;
// zLONG   lBorderColor = 0;
   zLONG   lEscapement = 0;
   zLONG   lSizeX;
   zLONG   lSizeY;
   zLONG   lSubtype;
   zULONG  ulLth = sizeof( ld );

   DisplayCurrentTime( "UPD_TEXT_PostBuild Start" );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Set the test control to the same size as the painted control.
   GetIntegerFromAttribute( &lSizeX, vControl, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lSizeY, vControl, "Control", "SZDLG_Y" );
   DisplayCurrentTime( "UPD_TEXT_PostBuild Before SetCtrlSize" );
   SetCtrlSize( vSubtask, "TestIt", lSizeX, lSizeY, TRUE );
   DisplayCurrentTime( "UPD_TEXT_PostBuild After SetCtrlSize" );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vControl );

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   if ( !ComponentIsCheckedOut( vSubtask, vControl, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vControl );

   DisplayCurrentTime( "UPD_TEXT_PostBuild Before GetBlob" );
   zmemset( &ld, 0, sizeof( ld ) );
   GetBlobFromAttribute( &ld, &ulLth, vControl, "Control", "CtrlBOI" );
   DisplayCurrentTime( "UPD_TEXT_PostBuild After GetBlob" );
   if ( ulLth == 0 )
   {
      ld.lTextColor = 0xFF000000;
      ld.lTextBkColor = 0xFF000000;
      ld.lBackgroundColor = 0xFF000000;
      ld.lHighlightColor = 0xFF000000;
   }

// TraceLineI( "Text control blob lth = ", ulLth );
   if ( (lSubtype | zCONTROL_BORDEROFF) == 0 )
   {
      ld.chBorder |= 1;
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED, TRUE );
   }

   if ( ulLth >= 4 + 2 * sizeof( zLONG ) )
   {
      SetCtrlState( vSubtask, "Bold", zCONTROL_STATUS_CHECKED,
                    ld.chBold ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Italic", zCONTROL_STATUS_CHECKED,
                    ld.chItalic ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Strikeout", zCONTROL_STATUS_CHECKED,
                    ld.chStrikeout ? TRUE : FALSE );
      SetCtrlState( vSubtask, "Underline", zCONTROL_STATUS_CHECKED,
                    ld.chUnderline ? TRUE : FALSE );

   // SetCtrlState( vSubtask, "MultiLine", zCONTROL_STATUS_CHECKED,
   //               szBlob[ 4 + (2 * sizeof( zLONG )) ] ? TRUE : FALSE );

      if ( ulLth > 5 + (2 * sizeof( zLONG )) )
      {
         if ( ld.chBottomRight & 1 )
            pchHorizontal = "Right";
         else
         if ( ld.chCenter & 1 )
            pchHorizontal = "Center";

         if ( ld.chBottomRight & 2 )
            pchVertical = "Bottom";
         else
         if ( ld.chCenter & 2 )
            pchVertical = "Center";

         if ( ulLth > 8 + (2 * sizeof( zLONG )) )
         {
#if 0
            lBorderStyle = *((zPLONG) (szBlob + 8 + (2 * sizeof( zLONG ))));
            SetCtrlState( vSubtask, "BorderTop", zCONTROL_STATUS_CHECKED,
                          (lBorderStyle & 0x00000100) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderLeft", zCONTROL_STATUS_CHECKED,
                          (lBorderStyle & 0x00000200) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderBottom", zCONTROL_STATUS_CHECKED,
                          (lBorderStyle & 0x00000400) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BorderRight", zCONTROL_STATUS_CHECKED,
                          (lBorderStyle & 0x00000800) ? TRUE : FALSE );
            switch ( lBorderStyle & 0x0000000F )
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
#endif
            SetCtrlText( vSubtask, "__Font", ld.szFontName );

         // lBorderColor = *((zPLONG) (szBlob + 8 + (3 * sizeof( zLONG ))));
            lEscapement = ld.lEscapement;

         // lBorderStyle >>= 24;
         // zltoa( lBorderStyle, szWork, sizeof( szWork ) );
         // SetCtrlText( vSubtask, "LineWidth", szWork );

            SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED,
                          (ld.chBorder & 1) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "SunkenBorder", zCONTROL_STATUS_CHECKED,
                          (ld.chBorder & 2) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "BevelLine", zCONTROL_STATUS_CHECKED,
                          (ld.chBeveledLine & 1) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "VerticalText", zCONTROL_STATUS_CHECKED,
                          (ld.chVerticalText & 1) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "Highlight", zCONTROL_STATUS_CHECKED,
                          (ld.chHighlight & 1) ? TRUE : FALSE );
            SetCtrlState( vSubtask, "HighlightWithTip", zCONTROL_STATUS_CHECKED,
                          (ld.chHighlight & 2) ? TRUE : FALSE );
            SetCtrlText( vSubtask, "ToolTip", ld.szToolTip );

            SetCtrlState( vSubtask, "UseToolTip",
                          zCONTROL_STATUS_CHECKED, (ld.chUseToolTip & 1) );
            SetCtrlState( vSubtask, "MouseClickOnly",
                          zCONTROL_STATUS_CHECKED, (ld.chUseToolTip & 2) );
            SetCtrlState( vSubtask, "UseLeftMouse",
                          zCONTROL_STATUS_CHECKED, (ld.chUseToolTip & 4) );
            SetCtrlState( vSubtask, "HyperText",
                          zCONTROL_STATUS_CHECKED, ld.chHyperText );

            SetCtrlProperty( vSubtask, "__ColorTextBack",
                             zCONTROL_PROPERTY_INTEGER_DATA,
                             ld.lTextBkColor, 0 );
            SetCtrlProperty( vSubtask, "__ColorBackground",
                             zCONTROL_PROPERTY_INTEGER_DATA,
                             ld.lBackgroundColor, 0 );
            SetCtrlProperty( vSubtask, "__ColorHighlight",
                             zCONTROL_PROPERTY_INTEGER_DATA,
                             ld.lHighlightColor, 0 );
         }
      }
      else
         SetCtrlState( vSubtask, "Left", zCONTROL_STATUS_CHECKED, TRUE );

      DisplayCurrentTime( "UPD_TEXT_PostBuild After SetCtrlState" );
      zltoa( ld.lPointSize, szWork, sizeof( szWork ) );
      SetCtrlText( vSubtask, "Size", szWork );
      SetCtrlProperty( vSubtask, "__ColorTextFore",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       ld.lTextColor, 0 );
   // zltoa( lBorderColor, szWork, sizeof( szWork ) );
   // SetCtrlText( vSubtask, "BorderColor", szWork );
      DisplayCurrentTime( "UPD_TEXT_PostBuild After SetCtrlProperty" );

      szWork[ 1 ] = 0;
      if ( ld.lEscapement == 0 )
      {
         szWork[ 0 ] = '1';
         SetCtrlState( vSubtask, "0", zCONTROL_STATUS_CHECKED, TRUE );
      }
      else
      if ( ld.lEscapement == 900 )
      {
         szWork[ 0 ] = '2';
         SetCtrlState( vSubtask, "90", zCONTROL_STATUS_CHECKED, TRUE );
      }
      else
      if ( ld.lEscapement == 1800 )
      {
         szWork[ 0 ] = '3';
         SetCtrlState( vSubtask, "270", zCONTROL_STATUS_CHECKED, TRUE );
      }
      else
      if ( ld.lEscapement == 2700 )
      {
         szWork[ 0 ] = '4';
         SetCtrlState( vSubtask, "180", zCONTROL_STATUS_CHECKED, TRUE );
      }
      else
      {
         szWork[ 0 ] = '5';
         SetCtrlState( vSubtask, "Other", zCONTROL_STATUS_CHECKED, TRUE );
      }

      zltoa( lEscapement, szWork, sizeof( szWork ) );
      SetCtrlText( vSubtask, "Escapement", szWork );
   }
   else
   {
      SetCtrlProperty( vSubtask, "__ColorTextFore",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       ld.lTextColor, 0 );
      SetCtrlProperty( vSubtask, "__ColorTextBack",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       ld.lTextBkColor, 0 );
      SetCtrlProperty( vSubtask, "__ColorBackground",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       ld.lBackgroundColor, 0 );
      SetCtrlProperty( vSubtask, "__ColorHighlight",
                       zCONTROL_PROPERTY_INTEGER_DATA,
                       ld.lHighlightColor, 0 );
   }

// SetCtrlTextLth( vSubtask, "LineWidth", 2 );  // limit border width to 99

   DisplayCurrentTime( "UPD_TEXT_PostBuild Before CB_SetData" );
   CB_SetData( vSubtask, "Horizontal", pchHorizontal, "Left;Center;Right", 0 );
   CB_SetData( vSubtask, "Vertical", pchVertical, "Top;Center;Bottom", 0 );
   DisplayCurrentTime( "UPD_TEXT_PostBuild After CB_SetData" );

   UPD_TEXT_DisableControls( vSubtask );
   DisplayCurrentTime( "UPD_TEXT_PostBuild Before OnChange" );
   UPD_TEXT_OnChange( vSubtask );
   DisplayCurrentTime( "UPD_TEXT_PostBuild After OnChange" );
   SetFocusToCtrl( vSubtask, "Tag" );

   DisplayCurrentTime( "UPD_TEXT_PostBuild End" );

   // Go to build list of Web Potential Control Properties.
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildWebCtrlPropOpts( vDialogW, vDialogW, "Dialog", "wWebTextControlProperties" );
   DisplayCurrentTime( "UPD_TEXT_PostBuild After BuildWebCtrlPropOpts" );

   return( 0 );
}

zSHORT g_nTrace;

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_Init
//
// PURPOSE:  This function does the necessary work to set up
//           the Context combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDialogW;
   zVIEW    vWork;
   zSHORT   nRC;

   DisplayCurrentTime( "UPD_TEXT_Init Start" );
   g_nTrace = SetActionTrace( vSubtask, 0 );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Create Temporal subobject for OK/Cancel
   DisplayCurrentTime( "UPD_TEXT_Init Before CreateTemporal" );
   CreateTemporalSubobjectVersion( vDialogC, "Control" );
   DisplayCurrentTime( "UPD_TEXT_Init After CreateTemporal" );

   // Set Subtype in work object for Window EditBox
   if ( CheckExistenceOfEntity( vWork, "EditBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "EditBox", zPOS_BEFORE );

// GetIntegerFromAttribute( &lWork, vDialogC, "Control", "Subtype" );

   // We just want the low order bits from the subtype, so blank
   // out the high order bits
// lWork &= 0x0000FFFFL;   why??? DKS 1999/12/02
// SetAttributeFromInteger( vWork, "EditBox", "Subtype", lWork );

   // Set up Domain Context mapping.
   DisplayCurrentTime( "UPD_TEXT_Init Before CtrlContextMapping" );
   CtrlContextMappingInit( vSubtask );
   DisplayCurrentTime( "UPD_TEXT_Init After CtrlContextMapping" );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   // Drop any CtrlMap entries without mapping.
   for ( nRC = SetCursorFirstEntity( vDialogC, "CtrlMap", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogC, "CtrlMap", "" ) )
   {
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMapView" ) < zCURSOR_SET )
      {
         DeleteEntity( vDialogC, "CtrlMap", zREPOS_NONE );
      }
   }

   // Build Combobox list of MapType values for a Text Control.
   oTZWDLGSO_BuildMapTypeList( vDialogW, vDialogW, "Dialog", "wMapTypeTextControl" );

   DisplayCurrentTime( "UPD_TEXT_Init End" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
FontColor( zVIEW vSubtask )
{
   zCHAR    szColor[ 16 ];
   COLORREF clr;
   COLORREF aclrCust[ 32 ];
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetCtrlText( vSubtask, "Color", szColor, sizeof( szColor ) );
   clr = atol( szColor );
// TraceLineI( "FontColor colorref = ", (zLONG) clr );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
   // TraceLineI( "FontColor colorref after = ", (zLONG) clr );
      if ( ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      {
         zltoa( clr, szColor, sizeof( szColor ) );
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
UpdateFont( zVIEW vSubtask )
{
   zCHAR    szColor[ 16 ];
   LOGFONT  lf;
   COLORREF clr;
   zLONG    lSize;
   zLONG    lEscapement;
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   lf.lfWidth = 0;
   lf.lfEscapement = 0;
   lf.lfOrientation = 0;
   lf.lfCharSet = ANSI_CHARSET;
   lf.lfOutPrecision = OUT_CHARACTER_PRECIS;
   lf.lfClipPrecision = CLIP_CHARACTER_PRECIS;
   lf.lfQuality = PROOF_QUALITY;
   lf.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;

   GetCtrlText( vSubtask, "Color", szColor, sizeof( szColor ) );
   clr = atol( szColor );
   GetCtrlText( vSubtask, "Size", szColor, sizeof( szColor ) );
   lSize = atol( szColor );
   GetCtrlText( vSubtask, "Escapement", szColor, sizeof( szColor ) );
   lEscapement = atol( szColor );
   lf.lfHeight = lSize;
   GetCtrlText( vSubtask, "__Font", lf.lfFaceName, sizeof( lf.lfFaceName ) );

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

   if ( OperatorPromptForFont( vSubtask, (zPLONG) &lf,
                               &lSize, (zPLONG) &clr, TRUE ) == 1 )
   {
      if ( ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      {
         zltoa( clr, szColor, sizeof( szColor ) );
         SetCtrlText( vSubtask, "Color", szColor );
         zltoa( lSize, szColor, sizeof( szColor ) );
         SetCtrlText( vSubtask, "Size", szColor );
      // zltoa( lf.lfEscapement, szColor, sizeof( szColor ) );
      // SetCtrlText( vSubtask, "Escapement", szColor );
         SetCtrlText( vSubtask, "__Font", lf.lfFaceName );

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
fnRecurseMultiChangeTextCtrls( zVIEW vMultiChange, zVIEW vDialog,
                               LABEL_DATA *pldOrig, zLONG lCtrlType,
                               zBOOL bCreate )
{
   LABEL_DATA ld;
   zULONG ulLth;
   zCHAR  szTag[ 33 ];
   zLONG  lType;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lType, vDialog, "ControlDef", "Key" );
      if ( lType == lCtrlType )
      {
         ulLth = sizeof( LABEL_DATA );
         GetStringFromAttribute( szTag, sizeof( szTag ), vDialog, "Control", "Tag" );
         if ( bCreate )
         {
            CreateEntity( vMultiChange, "ErrorMsg", zPOS_AFTER );
            SetAttributeFromString( vMultiChange, "ErrorMsg", "Name", szTag );
            GetBlobFromAttribute( &ld, &ulLth, vDialog, "Control", "CtrlBOI" );
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
                  SetAttributeFromBlob( vDialog, "Control", "CtrlBOI",
                                        pldOrig, ulLth );
               }
            }
         }
      }

      SetViewToSubobject( vDialog, "CtrlCtrl" );
      fnRecurseMultiChangeTextCtrls( vMultiChange, vDialog,
                                     pldOrig, lCtrlType, bCreate );
      ResetViewFromSubobject( vDialog );

      nRC = SetCursorNextEntity( vDialog, "Control", 0 );
   }
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
   zVIEW   vControl;
   zVIEW   vDialogW;
   zVIEW   vMultiChange;
   zULONG  ulLth;
   zLONG   lSubtype;
   zSHORT nRC;

   // Get the view.
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // Initialize the blob.
   ld.lType = fnLoadLabelData( vSubtask, &ld );
   if ( ld.lType == 1 )
   {
      if ( (ld.lEscapement % 900) == 0 )
         ld.lType = 2;
      else
         ulLth = 8 + (6 * sizeof( zLONG )) + LF_FACESIZE;
   }

   if ( ld.lType == 2 )
      ulLth = sizeof( ld );
   else
   if ( ld.lType == 0 )
      ulLth = 0;

   // Store the blob.
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", &ld, ulLth );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( ld.chBorder & 1 )
      lSubtype &= ~zCONTROL_BORDEROFF;
   else
      lSubtype |= zCONTROL_BORDEROFF;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );
   if ( CheckExistenceOfEntity( vControl, "Event" ) == zCURSOR_SET )
      SetAttributeFromInteger( vControl, "Event", "Type", 1 );

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
   {
      zVIEW  vDialog;
      zLONG  lCtrlType;

      GetIntegerFromAttribute( &lCtrlType, vControl, "ControlDef", "Key" );
      CreateViewFromView( &vDialog, vControl );
      while ( ResetViewFromSubobject( vDialog ) == 0 )
      {
         // nothing to do
      }

      // Loop through all Controls.
      fnRecurseMultiChangeTextCtrls( vMultiChange, vDialog,
                                     &ld, lCtrlType, FALSE );

      DropView( vDialog );
      DropObjectInstance( vMultiChange );
   }

   // Accept the Control.
   AcceptSubobject( vControl, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   // Remove ComboBoxEntry work entities.
   for ( nRC = SetCursorFirstEntity( vDialogW, "ComboBoxEntry", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "ComboBoxEntry", "" ) )
   {
      DeleteEntity( vDialogW, "ComboBoxEntry", zREPOS_NONE );
   }

   if ( g_nTrace )
      SetActionTrace( vSubtask, g_nTrace );

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
   zVIEW   vDialogC;
   zVIEW   vDialogW;
   zVIEW   vMultiChange;
   zSHORT nRC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
      DropObjectInstance( vMultiChange );

   // Cancel control subobject.
   CancelSubobject( vDialogC, "Control" );

// SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent |
//                                       zWAB_ProcessImmediateReturn, 0, 0 );

   // Delete the WebControlPropertyOption entries.
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   // Remove ComboBoxEntry work entities.
   for ( nRC = SetCursorFirstEntity( vDialogW, "ComboBoxEntry", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "ComboBoxEntry", "" ) )
   {
      DeleteEntity( vDialogW, "ComboBoxEntry", zREPOS_NONE );
   }

   if ( g_nTrace )
      SetActionTrace( vSubtask, TRUE );

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
SetupMultiChangeText( zVIEW vSubtask )
{
   LABEL_DATA ld;
   zULONG ulLth = sizeof( LABEL_DATA );
   zVIEW  vDialog;
   zVIEW  vDialogC;
   zVIEW  vMultiChange;
   zLONG  lCtrlType;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lCtrlType, vDialogC, "ControlDef", "Key" );
   zmemset( &ld, 0, sizeof( LABEL_DATA ) );
   GetBlobFromAttribute( &ld, &ulLth, vDialogC, "Control", "CtrlBOI" );
   CreateViewFromView( &vDialog, vDialogC );
   while ( ResetViewFromSubobject( vDialog ) == 0 )
   {
      // nothing to do
   }

   GetViewByName( &vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   if ( vMultiChange )
      DropObjectInstance( vMultiChange );

   ActivateEmptyObjectInstance( &vMultiChange, "TZERROR", vSubtask, 0 );
   SetNameForView( vMultiChange, "TZMULTICHANGE", vSubtask, zLEVEL_TASK );
   CreateEntity( vMultiChange, "ErrorList", zPOS_AFTER );

   fnRecurseMultiChangeTextCtrls( vMultiChange, vDialog,
                                  &ld, lCtrlType, TRUE );

   DropView( vDialog );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_CommonDetail
//
// PURPOSE:  This function prepares the Subtype prior to going to common
//           detail for a text control (border must be kept in sync).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_CommonDetail( zVIEW vSubtask )
{
   zVIEW   vControl;
   zLONG   lSubtype;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( GetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED ) )
      lSubtype &= ~zCONTROL_BORDEROFF;
   else
      lSubtype |= zCONTROL_BORDEROFF;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_TEXT_OnReturn
//
// PURPOSE:  This function synchronizes the Subtype and the border bits in
//           the blob for a text control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_TEXT_OnReturn( zVIEW vSubtask )
{
   zVIEW   vControl;
   zLONG   lSubtype;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED,
                 (lSubtype & zCONTROL_BORDEROFF) ? FALSE : TRUE );
   UPD_TEXT_OnChange( vSubtask );
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
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDialogW;
   zVIEW    vWork;
   zLONG    lWork;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Create Temporal subobject for OK/Cancel
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Set Subtype in work object for Window EditBox
   if ( CheckExistenceOfEntity( vWork, "EditBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "EditBox", zPOS_BEFORE );

   GetIntegerFromAttribute( &lWork, vDialogC, "Control", "Subtype" );

   // We just want the low order bits from the subtype, so blank
   // out the high order bits in the work object instance.
   lWork &= 0x0000FFFFL;
   SetAttributeFromInteger( vWork, "EditBox", "Subtype", lWork );

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      // If an Action for the Event exists, position on the linked Action
      // instance and set TZACTION to TZWINDOW.
      if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
      {
         CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
         SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                     vDialogC, "EventAct", "Tag", "" );
         SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
      }
      else
      // If not, set TZACTION to empty instance for auto graying.
      {
         fnCreateEmptyActionView( vSubtask, vDialogW );
      }

      // Set up the Event List Box Properly.
      fnSetupEventList( vSubtask, vDialogC );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNCTAD", "UPD_EDIT_REPORT" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    TextTestIt
//
// PURPOSE:  This function handles the test event for the Text Ctrl.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TextTestIt( zVIEW vSubtask )
{
   zVIEW  vControl;
   zCHAR  szTag[ 33 ];

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
      GetStringFromAttribute( szTag, sizeof( szTag ), vControl, "EventAct", "Tag" );
   else
      strcpy_s( szTag, sizeof( szTag ), "No Action Specified" );

   OperatorPrompt( vSubtask, "Action", szTag, 0, 0, 0, zICON_INFORMATION );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    TextSelectAction
//
// PURPOSE:  This function creates the Event entity for the Text Ctrl.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TextSelectAction( zVIEW vSubtask )
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
// ENTRY:    TextAddAction
//
// PURPOSE:  This function creates an Action entity and includes it to
//           the selected control.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TextAddAction( zVIEW vSubtask )
{
   zVIEW  vDialog;
   zVIEW  vControl;
   zVIEW  vWork;
// zCHAR  szTag[ 33 ];

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
// GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
// GetStringFromAttribute( szTag, sizeof( szTag ), vControl, "Control", "Tag" );
// TraceLineS( "TextAddAction TZCONTROL: ", szTag );
// GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "Control", "Tag" );
// TraceLineS( "TextAddAction TZCTLMAP3: ", szTag );

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
// ENTRY:    TextDeleteAction
//
// PURPOSE:  This function creates the Event entity for the Text Ctrl.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
TextDeleteAction( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vControl, "EventAct" ) == zCURSOR_SET )
   {
      ExcludeEntity( vControl, "EventAct", zREPOS_NONE );
      DeleteEntity( vControl, "Event", zPOS_NEXT );
      RefreshCtrl( vSubtask, "Action" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_EDIT_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_EDIT_Postbuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vDialogW;
   zLONG lFormatLth;
   zCHAR  szBlob[ (2 * 256) + (3 * sizeof( zLONG )) ];
   zULONG ulLth = sizeof( szBlob );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );

   // Consistency check to recognize old garbage editbox CtrlBOI's.
   // ulLth = (2 * (lFormatLth + 1)) + (3 * sizeof( zLONG ));
   if ( ulLth > 0 && (2 * ((lFormatLth = *(zPLONG) szBlob) + 1)) +
                     (3 * sizeof( zLONG )) == ulLth )
   {
      zLONG  lSpecialCnt = *((zPLONG) (szBlob + sizeof( zLONG )));
      zLONG  lReserved = *((zPLONG) (szBlob + (2 * sizeof( zLONG ))));
      zPCHAR pchFormat = szBlob + (3 * sizeof( zLONG ));

      // 1 reserved byte.
      SetCtrlText( vSubtask, "PlaceHolder", szBlob + (2 * sizeof( zLONG )) );
      if ( szBlob[ (2 * sizeof( zLONG )) + 2 ] )
         SetCtrlState( vSubtask, "FixedFont", zCONTROL_STATUS_CHECKED, 1 );

      SetCtrlText( vSubtask, "Format", pchFormat );
      pchFormat += lFormatLth + 1;
      SetCtrlText( vSubtask, "Override", pchFormat );
      if ( lSpecialCnt < 0 )
         SetCtrlState( vSubtask, "StripSpecial", zCONTROL_STATUS_CHECKED, 1 );
   }

   SetCtrlTextLth( vSubtask, "Format", 254 );
   SetCtrlTextLth( vSubtask, "Override", 254 );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( MiGetUpdateForView( vControl ) == 0 )
   {
      SetCtrlState( vSubtask, "EditSubtypeBox",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Format",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Override",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "FixedFont",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "StripSpecial",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Go to build list of Web Potential Control Properties.
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildWebCtrlPropOpts( vDialogW, vDialogW, "Dialog", "wWebEditControlProperties" );

   return( 0 );
} // UPD_EDIT_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_EDIT_OK
//
// PURPOSE:  This function sets Control.Subtype from EditBox.Subtype
//           and sets Context.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_EDIT_OK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vDialogW;
   zVIEW  vWork;
   zLONG  lWorkOrig;
   zLONG  lWorkNew;
   zCHAR  szBlob[ (2 * 256) + (3 * sizeof( zLONG )) ];
   zULONG ulLth = 0;
   zSHORT nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   GetCtrlText( vSubtask, "Format", szBlob + (3 * sizeof( zLONG )), 254 );
   if ( szBlob[ 3 * sizeof( zLONG ) ] )
   {
      zCHAR  szWork[ 256 ];
      zLONG lFormatLth = zstrlen( szBlob + (3 * sizeof( zLONG )) );
      zLONG lSpecialLth;
      zLONG lSpecialCnt;
      zLONG k;

      GetCtrlText( vSubtask, "Override", szWork, 254 );
      lSpecialLth = zstrlen( szWork );

      if ( lSpecialLth > lFormatLth )
      {
         MessageSend( vSubtask, "PN00119", "Dialog Maintenance",
                      "Override specification length must not "
                        "exceed Format specification length.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }

      while ( lSpecialLth < lFormatLth )
         szWork[ lSpecialLth++ ] = 0;

      lSpecialCnt = 0;
      for ( k = 0; k < lSpecialLth; k++ )
      {
         if ( szWork[ k ] == 'Y' || szWork[ k ] == 'y' )
         {
            szWork[ k ] = 'Y';
            lSpecialCnt++;
         }
         else
         {
            szWork[ k ] = '.';
         }
      }

      for ( k = 0; k < lFormatLth; k++ )
      {
         if ( szWork[ k ] == 'Y' )
            continue;

         switch ( szBlob[ k + (3 * sizeof( zLONG )) ] )
         {
            case '#':   // numeric (0-9)
            case '?':   // alpha (a-z;A-Z)
            case '*':   // alphanumeric (a-z;A-Z;0-9)
            case '^':   // uppercase (A-Z)
            case '_':   // lowercase (a-z)
            case '$':   // currency (0-9;$;.;,)
            case '€':   // currency euro (€)
            case '£':   // currency pound
            case '@':   // hex (a-f;A-F;0-9)
               break;

            // Add new masks as needed

            default:
               szWork[ k ] = 'Y';
               lSpecialCnt++;
               break;
         }
      }

      szWork[ lSpecialLth ] = 0;
      if ( GetCtrlState( vSubtask, "StripSpecial", zCONTROL_STATUS_CHECKED ) )
         lSpecialCnt = -lSpecialCnt;

      szBlob[ (3 * sizeof( zLONG )) + lFormatLth ] = 0;
      strcpy_s( szBlob + (3 * sizeof( zLONG )) + lFormatLth + 1, sizeof( szBlob + (3 * sizeof( zLONG )) + lFormatLth + 1 ), szWork );
      *((zPLONG) szBlob) = lFormatLth;
      *((zPLONG) (szBlob + sizeof( zLONG ))) = lSpecialCnt;
      *((zPLONG) (szBlob + (2 * sizeof( zLONG )))) = 0;  // 1 reserved byte
      GetCtrlText( vSubtask, "PlaceHolder",
                   szBlob + (2 * sizeof( zLONG )), 2 );
      if ( GetCtrlState( vSubtask, "FixedFont", zCONTROL_STATUS_CHECKED ) )
         szBlob[ (2 * sizeof( zLONG )) + 2 ] = 'Y';

      ulLth = (2 * (lFormatLth + 1)) + (3 * sizeof( zLONG ));
   }

   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   // Set Control.Subtype from EditBox.Subtype
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lWorkOrig, vControl, "Control", "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "EditBox", "Subtype" );

   // Blank out the low order bits in the original Subtype, because
   // we are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;
   SetAttributeFromInteger( vControl, "Control", "Subtype", lWorkOrig );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   // Delete the WebControlPropertyOption entries.
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstEntity( vDialogW, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialogW, "WebControlPropertyOption", "" ) )
   {
      DeleteEntity( vDialogW, "WebControlPropertyOption", zREPOS_NONE );
   }

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_Cancel
//
// PURPOSE:  This function just sets up parent refresh properly.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_Cancel( zVIEW vSubtask )
{
   zVIEW    vDialog;

   // Cancel CtrlMap.
   if ( GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vDialog, "CtrlMap" ) >= zCURSOR_SET )
   {
      CancelSubobject( vDialog, "CtrlMap" );
   }

   if ( GetViewByName( &vDialog, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_InitReuse
//
// PURPOSE:  This function activates the LOD associated with the
//           current view.  If no view is currently defined, it
//           temporarily sets up a dummy view.  I'm not sure what
//           it should eventually do.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_InitReuse( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zVIEW    vTZPNTROO;
   zLONG    lZKey;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   // If CtrlMapView exists, use the corresponding LOD for mapping.
   // Use CtrlMapView ZKey to locate ViewObjRef and then
   // LOD ZKey to locate LPLR LOD meta.
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapView" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapView", "ZKey" );

      // Make sure we find the right ViewObjRef!  If not we will assume that
      // something has happened to the mapping and we will do the default
      // processing below. // DKS 2001.01.09
      if ( SetCursorFirstEntityByInteger( vDialogW, "ViewObjRef", "ZKey",
                                          lZKey, "" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
         SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
         ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
         SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

         // Select current mapping in LOD.
         if ( CheckExistenceOfEntity( vDialogC, "CtrlMapLOD_Attribute" ) == zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lZKey, vDialogC,
                                     "CtrlMapRelatedEntity", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                                "ZKey", lZKey, "" ) != zCURSOR_SET )
            {
               zPCHAR pchEntity;

               GetAddrForAttribute( &pchEntity, vDialogC, "CtrlMapRelatedEntity", "Name" );
               SetCursorFirstEntityByString( vLOD, "LOD_Entity",
                                             "Name", pchEntity, "" );
               TraceLineS( "LOD Entity not located by ZKey: ", pchEntity );
            }

            SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
            GetIntegerFromAttribute( &lZKey, vDialogC,
                                     "CtrlMapLOD_Attribute", "ZKey" );
            if ( SetCursorFirstEntityByInteger( vLOD, "LOD_Attribute",
                                                "ZKey", lZKey, "" ) != zCURSOR_SET )
            {
               zPCHAR pchAttribute;

               GetAddrForAttribute( &pchAttribute, vDialogC, "CtrlMapER_Attribute", "Name" );
               SetCursorFirstEntityByString( vLOD, "ER_Attribute",
                                             "Name", pchAttribute, "LOD_Entity" );
               TraceLineS( "LOD Attribute not located by ZKey: ", pchAttribute );
            }

            SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );
         }

         return( 0 );
      }
   }

   // If no current mapping exists, try to use the last mapping
   // specified for the Entity and Attribute list boxes.  Also
   // set view to last view used.
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lZKey, vTZPNTROO,
                            "CtrlWork", "LastMapViewZKey" );
   if ( SetCursorFirstEntityByInteger( vDialogW, "ViewObjRef",
                                       "ZKey", lZKey, "" ) == zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMapView" ) == zCURSOR_SET )
         ExcludeEntity( vDialogC, "CtrlMapView", zREPOS_PREV );

      IncludeSubobjectFromSubobject( vDialogC, "CtrlMapView",
                                     vDialogW, "ViewObjRef", zPOS_AFTER );
      GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
      SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

      // Position on correct LOD_Entity
      GetIntegerFromAttribute( &lZKey, vTZPNTROO,
                               "CtrlWork", "LastMapEntityZKey" );
      SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                     "ZKey", lZKey, "" );
      SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
   }
   else
   {
      // Drop the SEL_LOD meta OI, if it exists.
      GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
      if ( vLOD )
         DropMetaOI( vSubtask, vLOD );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_Init
//
// PURPOSE:  This function activates the LOD associated with the
//           current view.  If no view is currently defined, it
//           temporarily sets up a dummy view.  I'm not sure what
//           it should eventually do.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vTZPNTROO;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Create work entity, if it doesn't exist.
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vTZPNTROO, "CtrlWork" ) != zCURSOR_SET )
      CreateEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );

   if ( MiGetUpdateForView( vDialogC ) )
   {
      // If CtrlMap doesn't exist, create it.
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMap" ) != zCURSOR_SET )
      {
         CreateTemporalMetaEntity( vSubtask, vDialogC, "CtrlMap", zPOS_AFTER );
      }
      else
      {
         CreateTemporalSubobjectVersion( vDialogC, "CtrlMap" );
      }
   }

   // Continue initialization in reusable operation.
   SEL_ATTR_InitReuse( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_ListInit
//
// PURPOSE:  This function activates the LOD associated with the
//           current view.  If no view is currently defined, it
//           temporarily sets up a dummy view.  I'm not sure what
//           it should eventually do.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_ListInit( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vTZPNTROO;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Create work entity, if it doesn't exist.
   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vTZPNTROO, "CtrlWork" ) != zCURSOR_SET )
      CreateEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );

   // Continue initialization in reusable operation.
   SEL_ATTR_InitReuse( vSubtask );

   // Make sure MapDomain2 is set up for the combo box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );

      // Include work Context if it exists.
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMapContext" ) == zCURSOR_SET )
         IncludeSubobjectFromSubobject( vDialogC, "TempMappingContext",
                                        vDialogC, "CtrlMapContext", zPOS_AFTER );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_SelectView
//
// PURPOSE:  This function
//           1. Does the necessary exclude/include of CtrlMapView.
//           2. Activates the LOD for the View just selected.
//           3. Refreshes the Entity and Attribute List Boxes and sets
//              the LOD Entity and Attribute select states.
//
//           Note that steps 2 and 3 are not done if the include is
//           to the same LOD type as for the previous CtrlMapView.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_SelectView( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vDomain;
   zLONG    lZKey;
   zSHORT   nRC;
   zCHAR    szObjectName[ 34 ];

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // Drop the SEL_LOD meta OI, if it exists.
   nRC = GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      DropMetaOI( vSubtask, vLOD );

   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapView" ) == zCURSOR_SET )
   {
      // Activate the LOD for the current ViewObjRef subobject.
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );
      GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
      SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

      SetSelectStateOfEntityForSet( vLOD, "LOD_Entity", 1, 1 );
      if ( CheckExistenceOfEntity( vLOD, "LOD_Attribute" ) == zCURSOR_SET )
         SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );

      RefreshCtrl( vSubtask, "EntityList" );
      RefreshCtrl( vSubtask, "AttributeList" );
   }

   // Make sure MapDomain2 view exists for current Attribute, but no current context exists.

   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapView" ) == zCURSOR_SET )
   {
      RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
      GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

      // Activate the Domain list for the current vLOD Attribute.
      GetIntegerFromAttribute( &lZKey, vLOD, "Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }
   // For Dialogs (ObjectName: TZWDLGSO), remove work Context, if it exists.
   MiGetObjectNameForView( szObjectName, vDialogC );
   if ( zstrcmp( szObjectName, "TZWDLGSO" ) == 0 )
      if ( CheckExistenceOfEntity( vDialogC, "TempMappingContext" ) >= zCURSOR_SET )
         ExcludeEntity( vDialogC, "TempMappingContext", zREPOS_PREV );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_SelectEntity
//
// PURPOSE:  This function refreshes the Entity and Attribute List
//           Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_SelectEntity( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vDomain;
   zVIEW    vDialogC;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zCHAR    szObjectName[ 34 ];

   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vLOD, "LOD_Attribute" ) == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );

   // Make sure MapDomain2 view exists for current Attribute, but no current context exists.

   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Activate the Domain list for the current vLOD Attribute.
   GetIntegerFromAttribute( &lZKey, vLOD, "Domain", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );

   // Remove work Context, if it exists.
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   MiGetObjectNameForView( szObjectName, vDialogC );
   if ( zstrcmp( szObjectName, "TZWDLGSO" ) == 0 )
      if ( CheckExistenceOfEntity( vDialogC, "TempMappingContext" ) >= zCURSOR_SET )
         ExcludeEntity( vDialogC, "TempMappingContext", zREPOS_PREV );

   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_SelectAttribute
//
// PURPOSE:  This function refreshes the Entity and Attribute List
//           Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_SelectAttribute( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zVIEW    vDialogC;
   zLONG    lZKey;
   zCHAR    szObjectName[ 34 ];

   // Make sure MapDomain2 view exists for current Attribute, but no current context exists.

   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Activate the Domain list for the current vLOD Attribute.
   GetIntegerFromAttribute( &lZKey, vLOD, "Domain", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );

   // Remove work Context, if it exists.
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   MiGetObjectNameForView( szObjectName, vDialogC );
   if ( zstrcmp( szObjectName, "TZWDLGSO" ) == 0 )
      if ( CheckExistenceOfEntity( vDialogC, "TempMappingContext" ) >= zCURSOR_SET )
         ExcludeEntity( vDialogC, "TempMappingContext", zREPOS_PREV );

   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SEL_ATTR_SelectMapping
//
// PURPOSE:  This function does the necessary exclude/includes
//           to set up attribute mapping.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_SelectMapping( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vLOD;
   zVIEW    vTZWINDOWL;
   zVIEW    vTZPNTROO;
   zSHORT   nRC;

   GetViewByName( &vTZPNTROO, "TZPNTROO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   if ( vLOD && CheckExistenceOfEntity( vLOD, "LOD" ) < zCURSOR_SET )
   {
      nRC = -1;
      DropView( vLOD );
   }

   // If CtrlMap exists, Accept CtrlMap
   if ( CheckExistenceOfEntity( vDialog, "CtrlMap" ) >= zCURSOR_SET )
      AcceptSubobject( vDialog, "CtrlMap" );

   if ( MiGetUpdateForView( vDialog ) == 0 )
      return( 0 );

   // There is no mapping view.
   if ( nRC <= 0 )
   {
      // If current CtrlMapContext exists, exclude it.
      if ( CheckExistenceOfEntity( vDialog, "CtrlMapContext" ) == zCURSOR_SET )
         ExcludeEntity( vDialog, "CtrlMapContext", zREPOS_PREV );

      // If current CtrlMapLOD_Attribute exists, exclude it.
      if ( CheckExistenceOfEntity( vDialog,
                                   "CtrlMapLOD_Attribute" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialog, "CtrlMapLOD_Attribute", zREPOS_PREV );
      }

      DeleteEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );
      return( 0 );
   }

   // Alter mapping only if it changed from existing.
   if ( CheckExistenceOfEntity( vDialog, "CtrlMapLOD_Attribute" )
                                                           != zCURSOR_SET ||
        CompareAttributeToAttribute( vDialog, "CtrlMapLOD_Attribute", "ZKey",
                                     vLOD, "LOD_Attribute", "ZKey" ) != 0 )
   {
      // If current CtrlMapContext exists, exclude it.
      if ( CheckExistenceOfEntity( vDialog, "CtrlMapContext" ) == zCURSOR_SET )
         ExcludeEntity( vDialog, "CtrlMapContext", zREPOS_PREV );

      // If current CtrlMapLOD_Attribute exists, exclude it.
      if ( CheckExistenceOfEntity( vDialog,
                                   "CtrlMapLOD_Attribute" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialog, "CtrlMapLOD_Attribute", zREPOS_PREV );
      }

      IncludeSubobjectFromSubobject( vDialog, "CtrlMapLOD_Attribute",
                                     vLOD, "LOD_Attribute", zPOS_AFTER );
   }

   // Set up default View Obj Ref and Entity for next mapping.
   if ( CheckExistenceOfEntity( vTZPNTROO, "CtrlWork" ) != zCURSOR_SET )
      CreateEntity( vTZPNTROO, "CtrlWork", zPOS_AFTER );

   SetAttributeFromAttribute( vTZPNTROO, "CtrlWork", "LastMapViewZKey",
                              vDialog, "CtrlMapView", "ZKey" );
   SetAttributeFromAttribute( vTZPNTROO, "CtrlWork", "LastMapEntityZKey",
                              vDialog, "CtrlMapRelatedEntity", "ZKey" );

   DropMetaOI( vSubtask, vLOD );

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   // Accept control subobject
   // Removed 4/20/07 by DonC because the accept is done when the Control window is OK'd.
// AcceptSubobject( vDialog, "CtrlMap" );

   if ( GetViewByName( &vDialog, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_CTRL_PhysOverride
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_CTRL_PhysOverride( zVIEW vSubtask )
{
#if 0
   zVIEW    vDialogC;
   zLONG    lSubtype;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
#endif
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_PUSH_Init
//
// PURPOSE:  This function sets up the TZACTION view for the
//           UPD_PUSH window.  It is the same as TZWINDOW if a current
//           EventAct exits.  It is null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_Init( zVIEW vSubtask )
{
   zVIEW  vDialogA;
   zVIEW  vDialogC;
   zVIEW  vDialogW;

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
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
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
//    OPERATION: UPD_PUSH_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_PUSH_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zLONG  lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   SetCtrlState( vSubtask, "MLT", zCONTROL_STATUS_CHECKED,
                 (lSubtype & zBUTTON_MULTILINE) ? TRUE : FALSE );
   SetCtrlState( vSubtask, "Hover", zCONTROL_STATUS_CHECKED,
                 (lSubtype & zBUTTON_HOVER) ? TRUE : FALSE );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetViewReadOnly( vTZWINDOWL );
      SetCtrlState( vSubtask, "MLT", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Hover", zCONTROL_STATUS_ENABLED, TRUE );
   }

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "edDLL", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edUp", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edDown", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edFocus", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edDisabled", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbBrowseUp", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbBrowseDown", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbBrowseFocus", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "pbBrowseDisabled", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MLT", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Hover", zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Go to build list of Web Potential Control Properties.
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildWebCtrlPropOpts( vDialogW, vDialogW, "Dialog", "wWebPushControlProperties" );

   return( 0 );

} // UPD_PUSH_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_PUSH_OK
//
// PURPOSE:  This function tells the Painter to refresh the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_OK( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zLONG  lSubtype;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   if ( GetCtrlState( vSubtask, "MLT", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zBUTTON_MULTILINE;
   else
      lSubtype &= ~zBUTTON_MULTILINE;

   if ( GetCtrlState( vSubtask, "Hover", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zBUTTON_HOVER;
   else
      lSubtype &= ~zBUTTON_HOVER;

   if ( !ComponentIsCheckedOut( vSubtask, vDialogC, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialogC );

   SetAttributeFromInteger( vDialogC, "Control", "Subtype", lSubtype );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogC, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vDialogC );

   // Accept the Control.
   AcceptSubobject( vDialogC, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_BITMAPBTN_Init
//
// PURPOSE:  This function sets up the TZACTION view for the
//           UPD_BITMAPBTN window.  It is the same as TZWINDOW if a current
//           EventAct exists.  It is null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_BITMAPBTN_Init( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vWork;
   zPCHAR   pCtrlBOI;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make Control temporal.
   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // Make sure, empty Bitmap entity exists in Work OI.
   if ( CheckExistenceOfEntity( vWork, "Bitmap" ) == zCURSOR_SET )
      DeleteEntity( vWork, "Bitmap", zREPOS_NONE );

   CreateEntity( vWork, "Bitmap", zPOS_BEFORE );

   // Set Bitmap attribute values from Control.CtrlBOI
   GetAddrForAttribute( &pCtrlBOI, vDialogC, "Control", "CtrlBOI" );
   if ( pCtrlBOI )
   {
      zCTRLBOI_BMP BMP;
      zCHAR  szName[ zMAX_FILENAME_LTH + 10 ];
      zULONG ulLth;

      // get CtrlBOI into CtrlBOI_BMP structure
      GetAttributeLength( &ulLth, vDialogC, "Control", "CtrlBOI" );
      SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP);

      SetAttributeFromString( vWork, "Bitmap", "DLL_Name", BMP.szDLLName );

      // show BMP name and user maintained ID, but no generated ID
      //   ID == 0: no ID given
      //   ID != 0 && <= zMAX_RES_ID_USER: ID from user, show it
      //   ID != 0 && > zMAX_RES_ID_USER: generated ID, don't show it
      if ( BMP.lBMPUpOrIconID && BMP.lBMPUpOrIconID <= zMAX_RES_ID_USER )
         sprintf( szName, "%ld;%s", BMP.lBMPUpOrIconID, BMP.szBMPUpOrIconName );
      else
         sprintf( szName, "%s", BMP.szBMPUpOrIconName );

      SetAttributeFromString( vWork, "Bitmap", "BitmapUP", szName );

      if ( BMP.lBMPDownID && BMP.lBMPDownID <= zMAX_RES_ID_USER )
         sprintf( szName, "%ld;%s", BMP.lBMPDownID, BMP.szBMPDownName );
      else
         sprintf( szName, "%s", BMP.szBMPDownName );

      SetAttributeFromString( vWork, "Bitmap", "BitmapDN", szName );

      if ( BMP.lBMPFocusID && BMP.lBMPFocusID <= zMAX_RES_ID_USER )
         sprintf( szName, "%ld;%s", BMP.lBMPFocusID, BMP.szBMPFocusName );
      else
         sprintf( szName, "%s", BMP.szBMPFocusName );

      SetAttributeFromString( vWork, "Bitmap", "BitmapFO", szName );

      if ( BMP.lBMPDisabledID && BMP.lBMPDisabledID <= zMAX_RES_ID_USER )
         sprintf( szName, "%ld;%s", BMP.lBMPDisabledID, BMP.szBMPDisabledName );
      else
         sprintf( szName, "%s", BMP.szBMPDisabledName );

      SetAttributeFromString( vWork, "Bitmap", "BitmapDA", szName );
   }

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
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

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_BITMAPBTN_OK
//
// PURPOSE:  This function tells the Painter to refresh the control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_BITMAPBTN_OK( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vWork;
   zVIEW  vDialogW;
   zCHAR  szCtrlBOI[ 3 * zMAX_FILENAME_LTH + 40 ];
   zPCHAR pCtrlBOI;
   zULONG ulLth;
   zCTRLBOI_BMP BMP;
   zCHAR  szName[ zMAX_FILENAME_LTH + 10 ];
   zPCHAR pchBMPName;
   zPCHAR pchBMPNameFirst = NULL;
   zLONG  lSubtype;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
   {
      AcceptSubobject( vDialogC, "Control" );
      return( 0 );
   }

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   if ( GetCtrlState( vSubtask, "MLT", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zBUTTON_MULTILINE;
   else
      lSubtype &= ~zBUTTON_MULTILINE;

   if ( GetCtrlState( vSubtask, "Hover", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zBUTTON_HOVER;
   else
      lSubtype &= ~zBUTTON_HOVER;

   SetAttributeFromInteger( vDialogC, "Control", "Subtype", lSubtype );

   // Get old CtrlBOI into CtrlBOI_BMP structure.
   GetAddrForAttribute( &pCtrlBOI, vDialogC, "Control", "CtrlBOI" );
   GetAttributeLength( &ulLth, vDialogC, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );

   // Set new CtrlBOI values
   GetStringFromAttribute( BMP.szDLLName, sizeof( BMP.szDLLName ), vWork, "Bitmap", "DLL_Name" );

   // if there is input for UP
   GetStringFromAttribute( szName, sizeof( szName ), vWork, "Bitmap", "BitmapUP" );
   SetAttributeFromString( vDialogC, "Control", "WebFileName", szName );
   if ( szName[ 0 ] )
   {
     // has the line an ID separated by ';'
     pchBMPName = szName;
     while ( pchBMPName[ 0 ] && pchBMPName[ 0 ] != ';' )
       pchBMPName++;

     if ( pchBMPName[ 0 ] == 0 )
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
       // if ID given, take it
       strcpy_s( BMP.szBMPUpOrIconName, sizeof( BMP.szBMPUpOrIconName ), ++pchBMPName );
       BMP.lBMPUpOrIconID = zatol( szName );
     }
   }
   else
   {
     BMP.szBMPUpOrIconName[ 0 ] = 0;
     BMP.lBMPUpOrIconID = 0;
   }

   pchBMPNameFirst = BMP.szBMPUpOrIconName;

   // if there is input for Down
   GetStringFromAttribute( szName, sizeof( szName ), vWork, "Bitmap", "BitmapDN" );
   if ( szName[ 0 ] )
   {
     // has the line an ID separated by ';'
     pchBMPName = szName;
     while ( pchBMPName[ 0 ] && pchBMPName[ 0 ] != ';' )
       pchBMPName++;

     if ( pchBMPName[ 0 ] == 0 )
     {
       // if no ID given look for old one
       if ( BMP.lBMPDownID <= zMAX_RES_ID_USER )
       {
         // if not generated, delete it
         BMP.lBMPDownID = 0;
       }

       strcpy_s( BMP.szBMPDownName, sizeof( BMP.szBMPDownName ), szName );
     }
     else
     {
       // if ID given, take it
       strcpy_s( BMP.szBMPDownName, sizeof( BMP.szBMPDownName ), ++pchBMPName );
       BMP.lBMPDownID = zatol( szName );
     }
   }
   else
   {
     BMP.szBMPDownName[ 0 ] = 0;
     BMP.lBMPDownID = 0;

     // if we had a name for the previous BMP, use it here
     if ( pchBMPNameFirst != NULL )
       strcpy_s( BMP.szBMPDownName, sizeof( BMP.szBMPDownName ), pchBMPNameFirst );
   }

   if ( pchBMPNameFirst == NULL )
     pchBMPNameFirst = BMP.szBMPDownName;

   // if there is input for Focus
   GetStringFromAttribute( szName, sizeof( szName ), vWork, "Bitmap", "BitmapFO" );
   if ( szName[ 0 ] )
   {
     // has the line an ID separated by ';'
     pchBMPName = szName;
     while ( pchBMPName[ 0 ] && pchBMPName[ 0 ] != ';' )
       pchBMPName++;

     if ( pchBMPName[ 0 ] == 0 )
     {
       // if no ID given look for old one
       if ( BMP.lBMPFocusID <= zMAX_RES_ID_USER )
       {
         // if not generated, delete it
         BMP.lBMPFocusID = 0;
       }

       strcpy_s( BMP.szBMPFocusName, sizeof( BMP.szBMPFocusName ), szName );
     }
     else
     {
       // if ID given, take it
       strcpy_s( BMP.szBMPFocusName, sizeof( BMP.szBMPFocusName ), ++pchBMPName );
       BMP.lBMPFocusID = zatol( szName );
     }
   }
   else
   {
     BMP.szBMPFocusName[ 0 ] = 0;
     BMP.lBMPFocusID = 0;

     // if we had a name for the previous BMP, use it here
     if ( pchBMPNameFirst != NULL )
       strcpy_s( BMP.szBMPFocusName, sizeof( BMP.szBMPFocusName ), pchBMPNameFirst );
   }

   // if there is input for Disabled
   GetStringFromAttribute( szName, sizeof( szName ), vWork, "Bitmap", "BitmapDA" );
   if ( szName[ 0 ] )
   {
     // has the line an ID separated by ';'
     pchBMPName = szName;
     while ( pchBMPName[ 0 ] && pchBMPName[ 0 ] != ';' )
       pchBMPName++;

     if ( pchBMPName[ 0 ] == 0 )
     {
       // if no ID given look for old one
       if ( BMP.lBMPDisabledID <= zMAX_RES_ID_USER )
       {
         // if not generated, delete it
         BMP.lBMPDisabledID = 0;
       }

       strcpy_s( BMP.szBMPDisabledName, sizeof( BMP.szBMPDisabledName ), szName );
     }
     else
     {
       // if ID given, take it
       strcpy_s( BMP.szBMPDisabledName, sizeof( BMP.szBMPDisabledName ), ++pchBMPName );
       BMP.lBMPDisabledID = zatol( szName );
     }
   }
   else
   {
     BMP.szBMPDisabledName[ 0 ] = 0;
     BMP.lBMPDisabledID = 0;

     // if we had a name for the previous BMP, use it here
     if ( pchBMPNameFirst != NULL )
       strcpy_s( BMP.szBMPDisabledName, sizeof( BMP.szBMPDisabledName ), pchBMPNameFirst );
   }

   ulLth = BuildCtrlBOI_BMP( &BMP, szCtrlBOI, sizeof( szCtrlBOI ) );
   SetAttributeFromBlob( vDialogC, "Control", "CtrlBOI", szCtrlBOI, ulLth );

   // Accept control subobject
   AcceptSubobject( vDialogC, "Control" );

   return( UpdateControlEvents( vSubtask ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_PUSH_RemoveAction
//
// PURPOSE:    To Remove an action from a push button
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_RemoveAction( zVIEW vSubtask )
{
   zVIEW  vEvents;
   zVIEW  vControl;
   zLONG  lType;
   zSHORT nRC;

   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );

   nRC = SetCursorFirstEntityByInteger( vControl, "Event", "Type", lType, 0 );
   if ( nRC >= zCURSOR_SET )
      DeleteEntity( vControl, "Event", zREPOS_PREV );

   fnCreateEmptyActionView( vSubtask, vControl );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_PUSH_SelectAction
//
// PURPOSE:  This function creates the Event entity for the Push
//           Button.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_SelectAction( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vWork;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if Event already exists.  Otherwise add it.
   if ( CheckExistenceOfEntity( vDialogC, "Event" ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "Dialog Maintenance",
                   "Event already has an Action,"
                     "\nFirst remove the Action and then select a new one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   CreateTemporalMetaEntity( vSubtask, vDialogC, "Event", zPOS_AFTER );
   SetAttributeFromInteger( vDialogC, "Event", "Type", 1 );

   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_PUSH_UpdateAction
//
// PURPOSE:  This function sets the TZWINDOW cursor to the correct
//           Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vDialogW;

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action",   "Tag",
                                  vWork,    "EventAct", "Tag", 0 );
      CreateTemporalSubobjectVersion( vDialogW, "Action" );
   }
   else
   {
      MessageSend( vSubtask, "PN00103", "Dialog Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_PUSH_ReturnFromSub
//
// PURPOSE:  Make sure TZACTION is the View for the Action just
//           selected.  TZACTION to be a null View, if no EventAct
//           existed prior to the Select.
//           Also drop the TZACTION View if it is null.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_PUSH_ReturnFromSub( zVIEW vSubtask )
{
   zVIEW  vDialogC;
   zVIEW  vDialogW;
   zVIEW  vDialogA;
   zVIEW  vEvent;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      // Drop TZACTION work OI, if it is empty.
      if ( CheckExistenceOfEntity( vDialogA, "Window" ) != zCURSOR_SET )
         DropObjectInstance( vDialogA );

      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );

      // Update Event/Action list.
      GetViewByName( &vEvent, "TZPNEVWO", vSubtask, zLEVEL_TASK );
      SetAttributeFromAttribute( vEvent, "EventAct", "Tag",
                                 vDialogC, "EventAct", "Tag" );
   }

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_PostBuildInit
//
// PURPOSE:  This function sets cursor position and Caption Title for
//           Both the LIST_ACT and SEL_ACT Windows..
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_PostBuildInit( zVIEW vSubtask )
{
   zVIEW vWork;
   zVIEW vWindow;
   zCHAR szTempString[ 256 ];

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Position "TZWINDOW" on Action which matches the event Action.
   if ( GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
   {

/*
BLx, 2000.08.20: do not change the Entity Cursor
      // need position for sub-dialog
      if ( CheckExistenceOfEntity( vWindow, "Action" ) == zCURSOR_SET )
      {
         zVIEW vEvents;
         if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 1 &&
              CheckExistenceOfEntity( vEvents, "EventAct" ) == zCURSOR_SET )
         {
            GetStringFromAttribute( szAction, sizeof( szAction ), vEvents, "EventAct", "Tag" );
            SetCursorFirstEntityByString( vWindow, "Action", "Tag", szAction, 0 );
            SetAllSelStatesForEntityForSet( vWindow, "Action", 0, 1, 0 );
            SetSelectStateOfEntityForSet( vWindow, "Action", 1, 1 );
         }
      }
*/

      // Set the caption for the main painter window
      GetStringFromAttribute( szTempString, sizeof( szTempString ), vWindow, "Window", "Tag" );
      SetWindowCaptionTitle( vSubtask, 0, szTempString );
   }
   else
      SetWindowCaptionTitle( vSubtask, 0, "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_PreBuildInit
//
// PURPOSE:  This function orders Actions by name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_PreBuildInit( zVIEW vSubtask )
{
   zVIEW vWindow;

   GetViewByName( &vWindow, "TZWINDOW", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vWindow, "Action", "Tag A" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_SelectAction
//
// PURPOSE:  This function does the necessary exclude/includes
//           for selection of an action for a Control/Event/Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_SelectAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogC;
   zVIEW    vDialogO;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // if Action is for a Control
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "C" ) == 0 ||
        CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "" ) == 0 )
   {
      GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

      // If current EventAct exists, exclude it.
      if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialogC, "EventAct", zREPOS_PREV );

      IncludeSubobjectFromSubobject( vDialogC, "EventAct",
                                     vDialogW, "Action", zPOS_AFTER );
      AcceptSubobject( vDialogC, "Event" );
   }
   else  // if Action is for a WndEvent
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "W" ) == 0 )
   {
      // If current WndAct exists, exclude it.
      if ( CheckExistenceOfEntity( vDialogW, "WndAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialogW, "WndAct", zREPOS_PREV );

      // Determine if WndEvent is of type 1, 2, 4, 8, 64 or 128.
      // If so, force Mapping off.
      GetIntegerFromAttribute( &lType, vDialogW, "WndEvent", "Type" );
      if ( ( (lType == 1) || (lType == 2) || (lType == 4) ||
            (lType == 8) || (lType == 64) || (lType == 128) ) &&
           CompareAttributeToString( vDialogW, "Action", "NoMap", "Y" ) != 0 )
      {
         SetAttributeFromString( vDialogW, "Action", "NoMap", "Y" );
      }

      IncludeSubobjectFromSubobject( vDialogW, "WndAct",
                                     vDialogW, "Action", zPOS_AFTER );
      AcceptSubobject( vDialogW, "WndEvent" );
   }
   else  // if Action is for an Option
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "O" ) == 0 )
   {
      GetViewByName( &vDialogO, "TZWINOPT", vSubtask, zLEVEL_TASK );

      // If current EventAct exists, exclude it.
      if ( CheckExistenceOfEntity( vDialogO, "OptAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialogO, "OptAct", zREPOS_PREV );

      IncludeSubobjectFromSubobject( vDialogO, "OptAct",
                                     vDialogW, "Action", zPOS_AFTER );
   }

   if ( GetViewByName( &vDialogW, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_Cancel
//
// PURPOSE:  This function cancels the WndEvent or Event that triggers
//           selection of an Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_Cancel( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogC;
   zVIEW    vWork;
   zCHAR    szParentFlag[ 2 ];

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szParentFlag, sizeof( szParentFlag ), vWork, "TZPNCTWO",
                           "ParentIsSelectAction" );

   // Don't process event cancel when parent window is ActionList or
   // Menu Option.
   if ( szParentFlag[ 0 ] != 'N' && szParentFlag[ 0 ] != 'O' )
   {
      if ( szParentFlag[ 0 ] == 'W' )
      {
         // Parent window is Window Detail, Cancel WndEvent.
         if ( CheckExistenceOfEntity( vDialogW, "WndEvent" ) >= zCURSOR_SET )
            CancelSubobject( vDialogW, "WndEvent" );
      }
      else
      {
         // Parent window is a Control update, Cancel Event.
         GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
         if ( CheckExistenceOfEntity( vDialogC, "Event" ) >= zCURSOR_SET )
            CancelSubobject( vDialogC, "Event" );
      }
   }

   if ( GetViewByName( &vDialogW, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LIST_ACT_Close( zVIEW vSubtask )
{
   fnPainterCall( zMSG_UPDATEZEIDONWINDOW, vSubtask, 0, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_RemoveAction
//
// PURPOSE:  This function does the necessary exclude/includes
//           for selection of an action for a Control/Event/Action.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_RemoveAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
// zVIEW    vDialogC;
   zVIEW    vDialogO;
   zVIEW    vEvents;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Action is for a Control
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "C" ) == 0 )
   {
      UPD_PUSH_RemoveAction( vSubtask );
   }

   // Action is for a WndEvent
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "ParentIsSelectAction", "W" ) == 0 )
   {

      SetAttributeFromString( vEvents, "EventAct", "Tag", 0 );
      GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
      if ( SetCursorFirstEntityByInteger( vDialogW, "WndEvent", "Type",
                                          lType, "" ) == zCURSOR_SET )
      {
         DeleteEntity( vDialogW, "WndEvent", zREPOS_PREV );
      }
   }

   // Action is for a Option
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                 "ParentIsSelectAction", "O" ) == 0 )
   {
      GetViewByName( &vDialogO, "TZWINOPT", vSubtask, zLEVEL_TASK );

      // If current EventAct exists, exclude it.
      if ( CheckExistenceOfEntity( vDialogO, "OptAct" ) == zCURSOR_SET )
         ExcludeEntity( vDialogO, "OptAct", zREPOS_PREV );

      fnCreateEmptyActionView( vSubtask, vDialogO );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_UpdateAction
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vWork;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "N" );
   CreateTemporalSubobjectVersion( vDialogW, "Action" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_AddAction
//
// PURPOSE:  This function creates an Action entity.
//           This whould be handled by subobject behavior,
//           except it doesn't create necessary ZKey.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_AddAction( zVIEW vSubtask )
{
   zVIEW vDialogW;
   zVIEW vWork;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure we set parentage correctly.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "N" );

   // Create and initialize the Action.
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );
   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", "Type", 1 );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LIST_ACT_DeleteAction
//
// PURPOSE:  This function deletes an Action if there are not
//           Control, Window Event or Option relationships to it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
LIST_ACT_DeleteAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zCHAR    szMsg[ 254 ];
   zSHORT   nPrevSelectSet;
   zSHORT   nRC;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   nPrevSelectSet = SetSelectSetForView( vDialogW, 1 );
   if ( SetCursorFirstSelectedEntity( vDialogW, "Action", "" ) == zCURSOR_SET )
   {
      // Ask the operator if it is ok to delete.
      strcpy_s( szMsg, sizeof( szMsg ), "OK to delete selected Actions" );

      if ( MessagePrompt( vSubtask,                 "CM00111",
                          "Dialog Maintenance",
                           szMsg,
                           zBEEP,         zBUTTONS_YESNO,
                           zRESPONSE_YES,  0 ) != zRESPONSE_NO )
      {
         for ( nRC = SetCursorFirstSelectedEntity( vDialogW, "Action", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextSelectedEntity( vDialogW, "Action", "" ) )
         {
            fnDeleteAction( vSubtask, vDialogW );
         }
      }
   }
   else
      MessageSend( vSubtask, "PN00104", "Dialog Maintenance",
                   "No Action selected to delete.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

   SetSelectSetForView( vDialogW, nPrevSelectSet );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_PositionOnActOper
//
// PURPOSE:  This operation positions on the ActOper specified for an
//           an action in the list object TZZOLODO so it is
//           highlighted in the reusable operation dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_PositionOnActOper( zVIEW vSubtask )
{
   zVIEW    vDialogA;
   zVIEW    vOperList;
   zVIEW    vWindow;

   if ( GetViewByName( &vOperList, "TZZOLODO", vSubtask, zLEVEL_TASK ) > 0 &&
        GetViewByName( &vDialogA, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 &&
        CheckExistenceOfEntity( vDialogA, "ActOper" ) >= zCURSOR_SET )
   {
      SetEntityCursor( vOperList, "Operation", "Name",
                       zPOS_FIRST | zQUAL_ENTITYATTR,
                       vDialogA, "ActOper", "Name", 0,
                       zSCOPE_OI, 0 );
   }


   // load Window Operation List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      ActivateWindow( vWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_SelectDialogOpers
//
// PURPOSE:  This operation selects all 'Dialog' type operations in select
//           Set 2 for the operation Combo-box in the Action update
//           window. This operation is called at window build time and
//           at return from sub-window to select opers added through
//           the operation sub-dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SelectDialogOpers( zVIEW vSubtask )
{
   zVIEW    vOperList;
   zSHORT   nRC;

   //-- jeff, to select only dlg operations
   nRC = GetViewByName( &vOperList, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( nRC > 0 )
   {
      SetAllSelStatesForEntityForSet( vOperList, "Operation", 0, 2, 0 );
      nRC = SetCursorFirstEntityByString( vOperList, "Operation",
                                          "Type", "D", "Dialog" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = SetSelectStateOfEntityForSet( vOperList, "Operation", 1, 2 );
         nRC = SetCursorNextEntityByString( vOperList, "Operation",
                                            "Type", "D", "Dialog" );
      }

      CB_SetDisplaySelectSet( vSubtask, "OtherOpers", 2 );
      //-- jeff end, to select only dlg operations
   }

   return( 0 );
}

void
MapDisableAction( zVIEW vSubtask, zVIEW vDialog, zBOOL bFromOI )
{
   zLONG  lDisableAction = 0;

   if ( bFromOI )
   {
      zPCHAR pchDisableAction;

      if ( vDialog &&
           CheckExistenceOfEntity( vDialog, "Action" ) == 0 )
      {
         GetIntegerFromAttribute( &lDisableAction, vDialog,
                                  "Action", "SB_ActionDisabling" );
         if ( lDisableAction & 0x10000000 )
         {
            SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, TRUE );
            lDisableAction &= ~0x10000000;
         }
         else
            SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, FALSE );
      }

      switch ( lDisableAction )
      {
         case 1:
            pchDisableAction = "Cursor On First Entity";
            break;

         case 2:
            pchDisableAction = "Cursor On Last Entity";
            break;

         case 4:
            pchDisableAction = "Entity Not Selected";
            break;

         case 8:
            pchDisableAction = "Entity Null";
            break;

         case 16:
            pchDisableAction = "Browse Only View";
            break;

         case 32:
            pchDisableAction = "Entity Not Null";
            break;

         default:
            pchDisableAction = "";
            break;
      }

      CB_SetData( vSubtask, "DisableAction", pchDisableAction,
                  ";Cursor On First Entity;Cursor On Last Entity;"
                  "Entity Not Selected;Entity Null;"
                  "Browse Only View;Entity Not Null", 0 );
   }
   else
   {
      zCHAR szDisableAction[ 33 ];

      CB_GetSelectedString( vSubtask, "DisableAction", szDisableAction, sizeof( szDisableAction ) );
      if ( zstrcmp( szDisableAction, "Cursor On First Entity" ) == 0 )
         lDisableAction = 1;
      else
      if ( zstrcmp( szDisableAction, "Cursor On Last Entity" ) == 0 )
         lDisableAction = 2;
      else
      if ( zstrcmp( szDisableAction, "Entity Not Selected" ) == 0 )
         lDisableAction = 4;
      else
      if ( zstrcmp( szDisableAction, "Entity Null" ) == 0 )
         lDisableAction = 8;
      else
      if ( zstrcmp( szDisableAction, "Browse Only View" ) == 0 )
         lDisableAction = 16;
      else
      if ( zstrcmp( szDisableAction, "Entity Not Null" ) == 0 )
         lDisableAction = 32;
      else
         lDisableAction = 0;

      if ( GetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED ) )
         lDisableAction |= 0x10000000;

      if ( vDialog &&
           CheckExistenceOfEntity( vDialog, "Action" ) == 0 )
      {
         SetAttributeFromInteger( vDialog, "Action",
                                  "SB_ActionDisabling", lDisableAction );
      }
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_Init
//
// PURPOSE:  This function
//           1. Sets up the TZSCOPE View to the 2nd CtrlMap entity for
//              displaying the current scoping entity in the ScopeList
//              combo box.
//           2. Set up SEL_LOD View, if current mapping exists.
//           3. Set up TZSCOPEL View for listing potential scoping
//              entities in the ScopeList combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_Init( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogCopy;
   zVIEW    vScope;
   zVIEW    vWindow;
   zSHORT   nRC;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // If Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialogW );

   // Make sure we're positioned on first ActMap.
   SetCursorFirstEntity( vDialogW, "ActMap", "" );

   CreateViewFromViewForTask( &vScope, vDialogW, 0 );
   SetNameForView( vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // If ActMap entities exist, position scoping view to the second one.
   if ( CheckExistenceOfEntity( vDialogW, "ActMap" ) == zCURSOR_SET )
   {
      if ( SetCursorNextEntity( vScope, "ActMap", "" ) != zCURSOR_SET )
         CreateMetaEntity( vSubtask, vScope, "ActMap", zPOS_AFTER );
   }
   else
   // Create ActMap entities, since they don't already exist, and
   // position the scoping view to the second one.
   {
      CreateMetaEntity( vSubtask, vDialogW, "ActMap", zPOS_AFTER );
      SetViewFromView( vScope, vDialogW );
      CreateMetaEntity( vSubtask, vScope, "ActMap", zPOS_AFTER );
   }

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   fnSetUpSEL_LOD( vDialogW, vDialogW, vSubtask, "ActMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   fnSetUpScopeListOI( vDialogW, vSubtask, "ActMapLOD_Entity" );

   UPD_ACT_SelectDialogOpers( vSubtask );
   UPD_ACT_SetupDialogList( vSubtask );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
   {
      SetViewReadOnly( vDialogW );
      SetCtrlState( vSubtask, "OtherOpers", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "DialogList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "WindowList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "SubobjView", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Handle Disable and ReadOnly checkbox manually.
   MapDisableAction( vSubtask, vDialogW, TRUE );

   // Action List is a multi selection list, delesect all Actions
   CreateViewFromViewForTask( &vDialogCopy, vDialogW, 0 );
   for ( nRC = SetCursorFirstSelectedEntity( vDialogW, "Action", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDialogW, "Action", "" ) )
   {
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );
   }

   SetCursorFirstEntityByAttr( vDialogW, "Action", "ZKey",
                               vDialogCopy, "Action", "ZKey", "" );
   DropView( vDialogCopy );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
      RefreshCtrl( vWindow, "ActionList" );

   UPD_ACT_SetEditButtonStatus( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_SelectView
//
// PURPOSE:  This function sets up the SEL_LOD view for
//           the Mapping Entity and Scope Entity List Boxes.
//           If there is current Mapping on Scope ActMap entities,
//           delete them and recreate one.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SelectView( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vDialogW;
   zVIEW    vDialogV;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogV, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   // Set up SEL_LOD view.
   GetIntegerFromAttribute( &lZKey, vDialogV, "LOD", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
   SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Remove existing relationships for the Mapping and Scope entities
   if ( CheckExistenceOfEntity( vDialogW, "ActMapLOD_Entity" ) == zCURSOR_SET )
   {
      ExcludeEntity( vDialogW, "ActMapLOD_Entity", zREPOS_PREV );
      SetCursorNextEntity( vDialogW, "ActMap", "" );
      if ( CheckExistenceOfEntity( vDialogW,
                                   "ActMapLOD_Entity" ) == zCURSOR_SET )
         ExcludeEntity( vDialogW, "ActMapLOD_Entity", zREPOS_PREV );
   }

   RefreshCtrl( vSubtask, "EntityList" );
   RefreshCtrl( vSubtask, "ScopeList" );

   // Make sure we're positioned on first ActMap.
   SetCursorFirstEntity( vDialogW, "ActMap", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_SelectEntity
//
// PURPOSE:  This function excludes any Scoping entity already a part
//           of this control and sets up the Scoping List object for
//           the Scope List Box.  Note that the Mapping Entity has been
//           automatically included as a part of the Entity List Box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SelectEntity( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;
   zSHORT   FoundFlag;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude scoping entity
   if ( CheckExistenceOfEntity( vScope, "ActMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "ActMapLOD_Entity", zREPOS_PREV );

   // Set up Scope list object for list box mapping
   // First, delete existing entities.
   if ( CheckExistenceOfEntity( vScopeL, "ScopeEntity" ) == zCURSOR_SET )
   {
      SetCursorFirstEntity( vScopeL, "ScopeEntity", 0 );
      do
         DeleteEntity( vScopeL, "ScopeEntity", zREPOS_NONE );
      while ( SetCursorNextEntity( vScopeL, "ScopeEntity", "" ) == zCURSOR_SET );
   }

   if ( CheckExistenceOfEntity( vDialogW, "ActMapLOD_Entity" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogW, "ActMapLOD_Entity", "ZKey" );
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent",
                                      "ZKey", lZKey ) != 0 )
      {
         FoundFlag = 0;
         fnSearchForEntityRecursively( vLOD, lZKey, &FoundFlag );
         do
         {
            CreateEntity( vScopeL, "ScopeEntity", zPOS_BEFORE );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity", "Name",
                                       vLOD, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity",
                                       "LOD_EntityZKey", vLOD,
                                       "LOD_EntityParent", "ZKey" );

         } while ( ResetViewFromSubobject( vLOD ) == 0 );
      }
   }

   // Note that ScopeList is name of combo box for UPD_ACT.
   RefreshCtrl( vSubtask, "ScopeList" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_SelectScope
//
// PURPOSE:  This function includes the Scoping entity under the
//           ActMap entity.  The entity is included from the SEL_LOD
//           object, but which entity to include comes from the
//           TZSCOPEL object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SelectScope( zVIEW vSubtask )
{
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;

   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude existing relationship if necessary.
   if ( CheckExistenceOfEntity( vScope, "ActMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "ActMapLOD_Entity", zREPOS_PREV );

   // Get position on correct LOD_Entity in SEL_LOD
   GetIntegerFromAttribute( &lZKey, vScopeL, "ScopeEntity", "LOD_EntityZKey" );
   SetCursorFirstEntityByInteger( vLOD, "LOD_Entity", "ZKey", lZKey, "" );
   IncludeSubobjectFromSubobject( vScope, "ActMapLOD_Entity",
                                  vLOD, "LOD_Entity", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_ACT_CheckAction
//
//   PURPOSE:  This function forces No Mapping if there is a related
//             WndEvent of the right type triggering this Action.
//
//             It also makes sure that the Action Termination is not
//             StayOnWindowWithRefresh if the Action is tied to a window
//             event.  This is because this combination is invalid and
//             can cause GUI problems.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_ACT_CheckAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogW2;
   zCHAR    szTag[ 34 ];
   zLONG    lType;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   /* If the user did not select an action terminator, then don't
      exit the window. */
   if ( CompareAttributeToString( vDialogW, "Action", "Type", 0 ) == 0 )
   {
      MessageSend( vSubtask, "PN00105", "Dialog Maintenance",
                   "An Action Terminator must be selected!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Verify that an Action Tag was entered and that it is unique for the window.
   GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "Action", "Tag" );
   if ( szTag[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "PN00114", "Dialog Maintenance",
                   "Action Tag is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Tag" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Verify that both an Operation Name and an Object Based Function are
   // not selected at the same time.
   GetIntegerFromAttribute( &lType, vDialogW, "Action", "SB_SubAction" );
   if ( lType > 0 &&
        CheckExistenceOfEntity( vDialogW, "ActOper" ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00118", "Dialog Maintenance",
                   "Operation and Object Based Function are mutually exclusive.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // If there is a duplicate Action Tag, the set cursor will find two matches.
   CreateViewFromViewForTask( &vDialogW2, vDialogW, 0 );
   SetCursorFirstEntityByString( vDialogW2, "Action", "Tag", szTag, "" );
   if ( SetCursorNextEntityByString( vDialogW2, "Action", "Tag", szTag,
                                     "" ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00115", "Dialog Maintenance",
                   "The Action Tag is not unique within this window.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetFocusToCtrl( vSubtask, "Tag" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropView ( vDialogW2 );
      return( -1 );
   }

   DropView( vDialogW2 );

   // If the Action Termination is StayOnWindowWithRefresh, check if an
   // ActWndEvent entity exists (which means a window event triggers this
   // Action).  If so, it is an error.
   if ( CompareAttributeToInteger( vDialogW, "Action", "Type",
                                   zWAB_StayOnWindowWithRefresh ) == 0 )
   {
      if ( CheckExistenceOfEntity( vDialogW, "ActWndEvent" ) >= zCURSOR_SET )
      {
         MessageSend( vSubtask, "PN00116", "Dialog Maintenance",
   "Same Window / Refresh is invalid if the action is tied to a window event.",
                     zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetFocusToCtrl( vSubtask, "Type" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   return( 0 );

} // UPD_ACT_CheckAction

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_ACT_ActionInclude
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_ACT_ActionInclude( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogO;
   zVIEW    vDialogC;
   zVIEW    vWork;
   zCHAR    szParentFlag[ 2 ];

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szParentFlag, sizeof( szParentFlag ),
                           vWork, "TZPNCTWO", "ParentIsSelectAction" );

   // Don't Accept event or include Action if parent window is ActionList.
   if ( szParentFlag[ 0 ] != 'N' )
   {
      if ( szParentFlag[ 0 ] == 'W' )
      {
         // Parent window is Window Detail, Accept WndEvent and include Action.
         if ( CheckExistenceOfEntity( vDialogW, "WndEvent" ) >= zCURSOR_SET )
         {
            if ( CheckExistenceOfEntity( vDialogW, "WndAct" ) < zCURSOR_SET )
               IncludeSubobjectFromSubobject( vDialogW, "WndAct",
                                              vDialogW, "Action", zPOS_AFTER );
            AcceptSubobject( vDialogW, "WndEvent" );
         }
      }
      else
      if ( szParentFlag[ 0 ] == 'O' )
      {
         // Parent window is Option update, include Action.
         GetViewByName( &vDialogO, "TZWINOPT", vSubtask, zLEVEL_TASK );
         if ( CheckExistenceOfEntity( vDialogO, "Option" ) >= zCURSOR_SET )
         {
            if ( CheckExistenceOfEntity( vDialogO, "OptAct" ) < zCURSOR_SET )
               IncludeSubobjectFromSubobject( vDialogO, "OptAct",
                                              vDialogW, "Action", zPOS_AFTER );
            AcceptSubobject( vDialogO, "OptAct" );
         }
      }
      else
      {
         // Parent window is a Control update, include Action.
         GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
         if ( vDialogC &&
              CheckExistenceOfEntity( vDialogC, "Event" ) >= zCURSOR_SET )
         {
            if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) < zCURSOR_SET )
               IncludeSubobjectFromSubobject( vDialogC, "EventAct",
                                              vDialogW, "Action", zPOS_AFTER );
            AcceptSubobject( vDialogC, "Event" );
         }
      }
   }

   return( 0 );

} // UPD_ACT_ActionInclude

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_CheckWndEvent
//
// PURPOSE:  This function forces No Mapping if there is a related
//           WndEvent of the right type triggering this Action.
//
//           It also makes sure that the Action Termination is not
//           StayOnWindowWithRefresh if the Action is tied to a window
//           event.  This is because this combination is invalid and
//           can cause GUI problems.
//
//           Note that UPD_ACT_Next, UPD_ACT_Prev and UPD_ACT_New
//           all call this function.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_CheckWndEvent( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zSHORT   nEventFlag;
   zLONG    lType;
   zSHORT   nRC;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( UPD_ACT_CheckAction( vSubtask ) < 0 )
      return( -1 );

   // Accept the Action and include the Action under the WndEvent or Event
   // entity (if necessary).  Also Accept the WndEvent or Event subobject.

   AcceptSubobject( vDialogW, "Action" );

   UPD_ACT_ActionInclude( vSubtask );

   // Determine if any related WndEvent is of type 1,2,4,8,64 or 128.
   // If so, force Mapping off.
   nEventFlag = 0;
   nRC = SetCursorFirstEntity( vDialogW, "ActWndEvent", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lType, vDialogW, "ActWndEvent", "Type" );
      if ( (lType == 1) || (lType == 2) || (lType == 4) ||
          (lType == 8) || (lType == 64) || (lType == 128) )
      {
         if ( CompareAttributeToString( vDialogW, "Action",
                                        "NoMap", "Y" ) != 0 )
         {
            nEventFlag = 1;
         }
      }

      nRC = SetCursorNextEntity( vDialogW, "ActWndEvent", "" );
   }

   if ( nEventFlag == 1 )
   {
      SetAttributeFromString( vDialogW, "Action", "NoMap", "Y" );

      // Notify operator that NoMap is being set.
      MessageSend( vSubtask, "PN00106", "Dialog Maintenance",
                   "Mapping forced off due to Window Event selection.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_OK
//
// PURPOSE:  This function accepts the Action and WndEvent or Event
//           that triggers the Action (if it exists).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_OK( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zSHORT   nRC;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
      return( 0 );

   // Go to process main logic.
   if ( UPD_ACT_CheckWndEvent( vSubtask ) < 0 )
      return( -1 );

   // Handle Disable and ReadOnly checkbox manually.
   MapDisableAction( vSubtask, vDialogW, FALSE );

   // Delete any ActMap entries that don't have children.
   nRC = SetCursorFirstEntity( vDialogW, "ActMap", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDialogW, "ActMapLOD_Entity" ) < zCURSOR_SET &&
           CheckExistenceOfEntity( vDialogW, "ActMapView" ) < zCURSOR_SET )
      {
         DeleteEntity( vDialogW, "ActMap", zREPOS_NONE );
      }

      nRC = SetCursorNextEntity( vDialogW, "ActMap", 0 );
   }

   if ( GetViewByName( &vDialogW, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_Cancel
//
// PURPOSE:  This function cancels the Action and WndEvent or Event
//           that triggers the Action (if it exists).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_Cancel( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogC;
   zVIEW    vWork;
   zCHAR    szParentFlag[ 2 ];

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   CancelSubobject( vDialogW, "Action" );

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szParentFlag, sizeof( szParentFlag ),
                           vWork, "TZPNCTWO", "ParentIsSelectAction" );

   // Don't process event cancel when parent window is ActionList or
   // Menu Option.
   if ( szParentFlag[ 0 ] != 'N' && szParentFlag[ 0 ] != 'O' )
   {
      if ( szParentFlag[ 0 ] == 'W' )
      {
         // Parent window is Window Detail, Cancel WndEvent.
         if ( CheckExistenceOfEntity( vDialogW, "WndEvent" ) >= zCURSOR_SET )
            CancelSubobject( vDialogW, "WndEvent" );
      }
      else
      {
         // Parent window is a Control update, Cancel Event.
         GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
         if ( vDialogC &&
              CheckExistenceOfEntity( vDialogC, "Event" ) >= zCURSOR_SET )
         {
            CancelSubobject( vDialogC, "Event" );
         }
      }
   }

   // Reset flag.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "" );

   if ( GetViewByName( &vDialogW, "NoRefresh", vSubtask, zLEVEL_TASK ) > 0 )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_New
//
// PURPOSE:  This function calls UPD_ACT_CheckWndEvent and then
//           creates a new Action entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_New( zVIEW vSubtask )
{
   zVIEW    vDialogW;

   // Check if related WndEvent forces No Mapping.
   if ( UPD_ACT_CheckWndEvent( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", "Type", 1 );

   UPD_ACT_Init( vSubtask );
   SetFocusToCtrl( vSubtask, "Tag" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_NextAction
//
// PURPOSE:  This function sets the cursor to the next Action.
//     ****  Note that this should be handled by subobject behavior.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_NextAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
// zVIEW    vWindow;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
   {
      // Check if related WndEvent forces No Mapping.
      if ( UPD_ACT_CheckWndEvent( vSubtask ) < 0 )
         return( -1 );

      // Handle Disable and ReadOnly checkbox manually.
      MapDisableAction( vSubtask, vDialogW, FALSE );

      GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   }

   SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );
   SetCursorNextEntity( vDialogW, "Action", "" );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
// fnSetUpSEL_LOD( vDialogW, vDialogW, vSubtask, "ActMapView" );
// UPD_ACT_SetEditButtonStatus( vSubtask );
//
// SetFocusToCtrl( vSubtask, "Tag" );
// if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
//    RefreshCtrl( vWindow, "ActionList" );

   UPD_ACT_Init( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_PrevAction
//
// PURPOSE:  This function sets the cursor to the next Action.
//     ****  Note that this should be handled by subobject behavior.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_PrevAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
// zVIEW    vWindow;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
   {
      // Check if related WndEvent forces No Mapping.
      if ( UPD_ACT_CheckWndEvent( vSubtask ) < 0 )
         return( -1 );

      // Handle Disable and ReadOnly checkbox manually.
      MapDisableAction( vSubtask, vDialogW, FALSE );

      GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   }

   SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );
   SetCursorPrevEntity( vDialogW, "Action", "" );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
// fnSetUpSEL_LOD( vDialogW, vDialogW, vSubtask, "ActMapView" );
// UPD_ACT_SetEditButtonStatus( vSubtask );
//
// SetFocusToCtrl( vSubtask, "Tag" );
// if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
//    RefreshCtrl( vWindow, "ActionList" );

   UPD_ACT_Init( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_DeleteAction
//
// PURPOSE:  This function deletes an Action if there are not
//           Control, Window Event or Option relationships to it, and
//           returns to the parent window, if not more Actions exist.
//           It executes the LIST_ACT_DeleteAction Operation to do the
//           delete.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_DeleteAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vWindow;
   zCHAR    szTag[ 33 ];
   zCHAR    szMsg[ 70 ];

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "Action", "Tag" );
   strcpy( szMsg, "OK to delete Actions '" );
   strcat( szMsg, szTag );
   strcat( szMsg, "'?" );

   // Prompt for delete.
   if ( MessagePrompt( vSubtask, "CM00111", "Dialog Maintenance",
                       szMsg, zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_YES,  0 ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   if ( fnDeleteAction( vSubtask, vDialogW ) < 0 )
   {
      SetFocusToCtrl( vSubtask, "Tag" );
      return( -1 );
   }

   SetCursorPrevEntity( vDialogW, "Action", "" );
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) == zCURSOR_NULL )
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   else
   {
      SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

      // Set up SEL_LOD, if current mapping exists, and any other
      // mapping data required.
      fnSetUpSEL_LOD( vDialogW, vDialogW, vSubtask, "ActMapView" );
      UPD_ACT_SetEditButtonStatus( vSubtask );

      SetFocusToCtrl( vSubtask, "Tag" );
      if ( GetSubtaskForWindowName( vSubtask, &vWindow, "LIST_ACT" ) >= 0 )
         RefreshCtrl( vWindow, "ActionList" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_Init
//
// PURPOSE:  This function
//           1. Sets up the ListBox work entity from CtrlBOI.
//           2. Sets up the TZPULL_Entity View for the EntityList
//              combo box
//           3. Sets up the TZSCOPE View to the 2nd CtrlMap entity for
//              displaying the current scoping entity in the ScopeList
//              combo box.
//           4. Initializes the TZPNEVWO Event work object as
//              necessary. (*** This shouldn't be necessary if the
//              Painter was always functioning correctly.  Remove and
//              test at some point.)
//           5. Set up SEL_LOD View, if current mapping exists.
//           6. Set up TZSCOPEL View for listing potential scoping
//              entities in the ScopeList combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vScope;
   zVIEW    vSelView;
   zVIEW    vWork;
   zLONG    lFormatType;
   zULONG   ulBlobLth;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Make sure work ListBox entity exists.
   if ( CheckExistenceOfEntity( vWork, "ListBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "ListBox", zPOS_BEFORE );

   // Set ListBox attribute values from Control.CtrlBOI
   GetAttributeLength( &ulBlobLth, vDialogC, "Control", "CtrlBOI" );
   if ( ulBlobLth )
   {
      SetEntityAttributesFromBlob( vWork, "ListBox",
                                   vDialogC, "Control", "CtrlBOI" );
   }
   else
   {
      SetAttributeFromInteger( vWork, "ListBox", "ScopeOI", 3 );
   }

   // Set Default value for format type - Drop Down List
   if ( GetVariableFromAttribute( &lFormatType, 0, zTYPE_INTEGER, 0,
                                  vWork, "ListBox", "Subtype", 0, 0 ) < 0 )
   {
      SetAttributeFromInteger( vWork, "ListBox", "Subtype", 0 );
   }

   // Set WindowFlag to 'L' to indicate that we are updating the listbox.
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "L" );

   // Create view name, TZPULL_Entity, for EntityList Combo box.
   SetNameForView( vDialogC, "TZPULL_Entity", vSubtask, zLEVEL_TASK );

   // Make sure we're positioned on first CtrlMap.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );

   CreateViewFromViewForTask( &vScope, vDialogC, 0 );
   SetNameForView( vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // If CtrlMap entities exist, position scoping view to the second one.
   if ( MiGetUpdateForView( vDialogC ) )
   {
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMap" ) == zCURSOR_SET )
      {
         if ( SetCursorNextEntity( vScope, "CtrlMap", "" ) != zCURSOR_SET )
            CreateMetaEntity( vSubtask, vScope, "CtrlMap", zPOS_AFTER );
      }
      else
      // Create CtrlMap entities, since they don't already exist, and
      // position the scoping view to the second one.
      {
         CreateMetaEntity( vSubtask, vDialogC, "CtrlMap", zPOS_AFTER );
         SetViewFromView( vScope, vDialogC );
         CreateMetaEntity( vSubtask, vScope, "CtrlMap", zPOS_AFTER );
      }
   }

   // Create a third CtrlMap that will hold the selected listbox view.
   CreateViewFromViewForTask( &vSelView, vScope, 0 );
   SetNameForView( vSelView, "TZSELLIST", vSubtask, zLEVEL_TASK );
   if ( SetCursorNextEntity( vSelView, "CtrlMap", "" ) != zCURSOR_SET )
      CreateMetaEntity( vSubtask, vSelView, "CtrlMap", zPOS_AFTER );

   // Initialize the TZPNEVWO work object if it's empty.
   // The Painter normally does this, but sometimes it seems to
   // forget.  *** Eliminate and retest.
   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vWork, "Event" ) != zCURSOR_SET )
   {
      zVIEW    vWork2;
      zSHORT   nRC;

      nRC = GetViewByName( &vWork2, "TZPACTLO", vSubtask, zLEVEL_TASK );
      if ( nRC >= 0 )
      {
         SetCursorFirstEntityByString( vWork2, "Control", "Tag", "ListBox", "" );
         do
         {
            CreateEntity( vWork, "Event", zPOS_AFTER );
            SetAttributeFromAttribute( vWork, "Event", "Tag",
                                       vWork2, "Event", "Tag" );
            SetAttributeFromAttribute( vWork, "Event", "Type",
                                       vWork2, "Event", "Type" );
            CreateEntity( vWork, "EventAct", zPOS_AFTER );

         } while ( SetCursorNextEntity( vWork2, "Event", "" ) == zCURSOR_SET );
      }
   }

   // Set up the Event List Box Properly.
   fnSetupEventList( vSubtask, vDialogC );

   // Set up SEL_LOD, if current mapping exists, and any other mapping
   // data required.
   fnSetUpSEL_LOD( vDialogC, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for the
   // ScopeList combo box.
   fnSetUpScopeListOI( vDialogC, vSubtask, "CtrlMapLOD_Entity" );

   // Force cursor position to first event.
   SetCursorFirstEntity( vWork, "Event", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_GROUP_Init
//
// PURPOSE:  This function executes UPD_LISTBOX_Init and then sets up wWebGroupControlProperties entries.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_GROUP_Init( zVIEW vSubtask )
{
   zVIEW    vDialogW;

   // Use reusable operation.
   UPD_LISTBOX_Init( vSubtask );

   // Go to build list of Web Potential Control Properties.
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildWebCtrlPropOpts( vDialogW, vDialogW, "Dialog", "wWebGroupControlProperties" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_PostBuild( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vTZWINDOWL;
   zLONG    lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vDialog, "Control", "Subtype" );
   SetCtrlState( vSubtask, "SortDisplay", zCONTROL_STATUS_CHECKED,
                 (lSubtype & zLISTBOX_SORT_DISPLAY) ? TRUE : FALSE );
   SetCtrlState( vSubtask, "AutoDragSort", zCONTROL_STATUS_CHECKED,
                 (lSubtype & zLISTBOX_AUTODRAGSORT) ? TRUE : FALSE );


   if ( (lSubtype & zLISTBOX_AUTODRAGSORT) ||
        (lSubtype & zLISTBOX_SORTEDHEADERS) ||
        (lSubtype & zLISTBOX_COLUMNHEADERS) )
   {
      if ( lSubtype & zLISTBOX_SEPARATORS )
      {
         SetCtrlState( vSubtask, "ColumnSeparators",
                       zCONTROL_STATUS_CHECKED, TRUE );
      }
   }

   if ( (lSubtype & zLISTBOX_COLUMNHEADERS) &&
        (lSubtype & zLISTBOX_SORTEDHEADERS) )
   {
      SetCtrlState( vSubtask, "AutoSortMemory",
                    zCONTROL_STATUS_CHECKED, TRUE );
   }
   else
   if ( lSubtype & zLISTBOX_COLUMNHEADERS )
      SetCtrlState( vSubtask, "Buttons", zCONTROL_STATUS_CHECKED, TRUE );
   else
   if ( lSubtype & zLISTBOX_SORTEDHEADERS )
      SetCtrlState( vSubtask, "AutoSort", zCONTROL_STATUS_CHECKED, TRUE );
   else
      SetCtrlState( vSubtask, "NoHeader", zCONTROL_STATUS_CHECKED, TRUE );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "ListStyle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "View", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Default", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Scope", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "OI", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Inhibit", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "AutoDragSort", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "IntegralHeight", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Virtual", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NoHeader", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Buttons", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "AutoSort", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "AutoSortMemory", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ColumnSeparators", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_SelectEntity
//
// PURPOSE:  This function excludes any Scoping entity already a part
//           of this control and sets up the Scoping List object for
//           the Scope List Box.  Note that the Mapping Entity has been
//           automatically included as a part of the Entity List Box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SelectEntity( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vScope;
   zVIEW    vScopeL;
   zVIEW    vLOD;
   zLONG    lZKey;
   zSHORT   FoundFlag;

   GetViewByName( &vDialogC, "TZPULL_Entity", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );
   GetViewByName( &vScopeL, "TZSCOPEL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Exclude scoping entity
   if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

   // Set up Scope list object for list box mapping
   // First, delete existing entities.
   if ( CheckExistenceOfEntity( vScopeL, "ScopeEntity" ) == zCURSOR_SET )
   {
      SetCursorFirstEntity( vScopeL, "ScopeEntity", 0 );
      do
         DeleteEntity( vScopeL, "ScopeEntity", zREPOS_NONE );
      while ( SetCursorNextEntity( vScopeL, "ScopeEntity", "" ) == zCURSOR_SET );
   }
   // If entity List has cursor position, continue on

   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapLOD_Entity", "ZKey" );
      if ( CompareAttributeToInteger( vLOD, "LOD_EntityParent",
                                      "ZKey", lZKey ) != 0 )
      {
         FoundFlag = 0;
         fnSearchForEntityRecursively( vLOD, lZKey, &FoundFlag );
         do
         {
            CreateEntity( vScopeL, "ScopeEntity", zPOS_BEFORE );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity", "Name",
                                       vLOD, "LOD_EntityParent", "Name" );
            SetAttributeFromAttribute( vScopeL, "ScopeEntity",
                                       "LOD_EntityZKey", vLOD,
                                       "LOD_EntityParent", "ZKey" );

         } while ( ResetViewFromSubobject( vLOD ) == 0 );
      }
   }

   // Note that ScopeList is name of combo box for both UPD_LISTBOX
   // and UPD_COMBO_S.
   RefreshCtrl( vSubtask, "ScopeList" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_SelectScope
//
// PURPOSE:  This function includes the Scoping entity under the
//           CtrlMap entity.  The entity is included from the SEL_LOD
//           object, but which entity to include comes from the
//           TZSCOPEL object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SelectScope( zVIEW vSubtask )
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

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_UpdateAction
//
// PURPOSE:  This function sets up the TZWINDOW view to the Action
//           entity corresponding to EventAct for the current Event.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_UpdateAction( zVIEW vSubtask )
{
   zVIEW    vWork;
   zVIEW    vDialogW;
   zCHAR    szTag[ 34 ];

   GetViewByName( &vWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // If EventAct exists, set cursor for Update Action window.
   // Otherwise, return an error.
   if ( (CheckExistenceOfEntity( vWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vWork, "EventAct", "Tag", "" ) != 0) )
   {
      GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vDialogW, "Action", "Tag", szTag, "" );
      CreateTemporalSubobjectVersion( vDialogW, "Action" );
   }
   else
   // Position on Action that corresponds to EventAct entity instance.
   {
      MessageSend( vSubtask, "PN00107", "Dialog Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_RemoveAction
//
// PURPOSE:  This function excludes the EventAct entity from the
//           current Event entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_RemoveAction( zVIEW vSubtask )
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

   // Set up the Event List Box properly
   fnSetupEventList( vSubtask, vDialog );

   RefreshCtrl( vSubtask, "EventListBox" );
   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_SelectAction
//
// PURPOSE:  This function positions on the Event entity in TZCONTROL
//           from the selected Event entity in TZPNEVWO.  If the Event
//           entity doesn't exist, it creates it.
//   *** This should be changed to a temporal subobject. ***
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SelectAction( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vEvents;
   zVIEW    vWork;
   zLONG    lType;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Error if no Event was selected.
   if ( SetCursorFirstSelectedEntity( vEvents, "Event", "" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00108", "Dialog Maintenance",
                   "No Event selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   // Error if Event already exists.  Otherwise add it.
   GetIntegerFromAttribute( &lType, vEvents, "Event", "Type" );
   if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                                       lType, 0 ) >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00117", "Dialog Maintenance",
                   "Event already has an Action!\n"
                   "First remove the Action and then select a new one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }
   else
   {
      CreateTemporalMetaEntity( vSubtask, vDialog, "Event", zPOS_AFTER );
      SetAttributeFromInteger( vDialog, "Event", "Type", lType );
   }

   // Set flag for LIST_ACT window graying and Action include for Control.
   SetAttributeFromString( vWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_ReturnFromSubwindow
//
// PURPOSE:  This function sets up the Event entity in TZPNEVWO from
//           the selected Event entity in TZCONTROL.
//           It also sets the Event Type.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vDialogW;
   zVIEW    vEvents;
   zVIEW    vWork;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      // Delete Event entity if there is not a relationship to Action
      if ( CheckExistenceOfEntity( vDialog, "Event" ) == zCURSOR_SET )
         if ( CheckExistenceOfEntity( vDialog, "EventAct" ) != zCURSOR_SET )
            DeleteEntity( vDialog, "Event", zREPOS_PREV );

      // Set up tye TZPNEVWO object instance from TZCONTROL and refresh
      // the List Box.
      fnSetupEventList( vSubtask, vDialog );
      RefreshCtrl( vSubtask, "EventListBox" );
   }
   else
   {
      GetViewByName( &vDialogW, "PAGEL", vSubtask, zLEVEL_TASK );
   }

   // Make sure WindowFlag is reset to indicate LISTBOX window.
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", "L" );

   // Make sure the SEL_LOD view is set up again.
   fnSetUpSEL_LOD( vDialog, vDialogW, vSubtask, "CtrlMapView" );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_Cancel
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_Cancel( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Reset WindowFlag to null (not updating listbox anymore).
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );
   CancelSubobject( vDialog, "Control" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_OK
//
// PURPOSE:  This function sets up X_ViewName and X_EntityName
//           attributes for interim compatibility.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vScope;
   zVIEW    vWork;
   zLONG    lWorkOrig;
   zLONG    lWorkNew;
   zLONG    lSubtypeX;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set Control.CtrlBOI from work entity ListBox.
   SetBlobFromEntityAttributes( vDialog, "Control", "CtrlBOI",
                                vWork, "ListBox" );
   SetAttributeFromString( vWork, "TZPNCTWO", "WindowFlag", 0 );

   // Build Control.Subtype attribute.
   GetIntegerFromAttribute( &lWorkOrig, vDialog, "Control", "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "ListBox", "Subtype" );
   GetIntegerFromAttribute( &lSubtypeX, vDialog, "Control", "ExtendedStyle" );
   lSubtypeX &= ~zLISTBOX_SCOPE_PARENT;

   if ( CompareAttributeToString( vWork, "ListBox",
                                  "SnapToIntegralHeight", "Y" ) != 0 )
   {
      lWorkNew |= 0x00000040L; // 64
   }

   if ( CompareAttributeToString( vWork, "ListBox",
                                  "InhibitListForNoSelScope", "Y" ) == 0 )
   {
      lWorkNew |= 0x00001000L; // 4096
   }

   if ( CompareAttributeToString( vWork, "ListBox", "ScopeOI", "3" ) == 0 )
   {
      lWorkNew |= zLISTBOX_SCOPE_OI;
   }
   else
   if ( CompareAttributeToString( vWork, "ListBox", "ScopeOI", "1" ) == 0 )
   {
      lSubtypeX |= zLISTBOX_SCOPE_PARENT;
   }

   if ( GetCtrlState( vSubtask, "SortDisplay", zCONTROL_STATUS_CHECKED ) )
      lWorkNew |= zLISTBOX_SORT_DISPLAY;

   // Blank out the low order bits in the original Subtype, because we
   // are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;

   if ( GetCtrlState( vSubtask, "AutoDragSort", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zLISTBOX_AUTODRAGSORT;
   else
      lWorkOrig &= ~zLISTBOX_AUTODRAGSORT;

   if ( GetCtrlState( vSubtask, "AutoSortMemory", zCONTROL_STATUS_CHECKED ) )
   {
      lWorkOrig |= zLISTBOX_COLUMNHEADERS;
      lWorkOrig |= zLISTBOX_SORTEDHEADERS;
   }
   else
   {
      lWorkOrig &= ~zLISTBOX_COLUMNHEADERS;
      lWorkOrig &= ~zLISTBOX_SORTEDHEADERS;
      if ( GetCtrlState( vSubtask, "AutoSort", zCONTROL_STATUS_CHECKED ) )
         lWorkOrig |= zLISTBOX_SORTEDHEADERS;

      if ( GetCtrlState( vSubtask, "Buttons", zCONTROL_STATUS_CHECKED ) )
         lWorkOrig |= zLISTBOX_COLUMNHEADERS;
   }

   if ( (lWorkOrig & zLISTBOX_AUTODRAGSORT) ||
        (lWorkOrig & zLISTBOX_SORTEDHEADERS) ||
        (lWorkOrig & zLISTBOX_COLUMNHEADERS) )
   {
      if ( GetCtrlState( vSubtask, "ColumnSeparators", zCONTROL_STATUS_CHECKED ) )
         lWorkOrig |= zLISTBOX_SEPARATORS;
      else
         lWorkOrig &= ~zLISTBOX_SEPARATORS;
   }

   if ( (lWorkOrig & zLISTBOX_SORTEDHEADERS) &&
        (lWorkOrig & zLISTBOX_SCOPE_OI) )
   {
      // Message that sorting will not work in this situation if the entity
      // being sorted is not the root entity.  The best we can do is sort
      // within the parents, but we don't even do that in the current
      // implementation.
      TraceLineS( "If the scope is Object Instance and the entity is not at the "
                    "root level, sorting will not occur properly when there ",
                  "are instances of the entity within different parents." );
   }

   SetAttributeFromInteger( vDialog, "Control", "Subtype", lWorkOrig );
   SetAttributeFromInteger( vDialog, "Control", "ExtendedStyle", lSubtypeX );

   SetCursorFirstEntity( vDialog, "CtrlMap", "" );
   CreateViewFromViewForTask( &vScope, vDialog, 0 );
   SetCursorNextEntity( vScope, "CtrlMap", "" );

   // Accept control subobject.
   AcceptSubobject( vDialog, "Control" );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_SetScope
//
// PURPOSE:  This function handles the setting of controls based on
//           the setting of the SCOPE Radio Buttons and Combo Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SetScope( zVIEW vSubtask )
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

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_LISTBOX_SelectView
//
// PURPOSE:  This function sets up the SEL_LOD view for
//           the Mapping Entity and Scope Entity List Boxes.
//           If there is current Mapping on Scope CtrlMap entities,
//           delete them and recreate one.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_LISTBOX_SelectView( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vDialogW;
   zVIEW    vDialogC;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   // Set up SEL_LOD view.
   if ( CheckExistenceOfEntity( vDialogW, "LOD" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
   }

   ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
   SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   // Remove existing relationships for the Mapping and Scope entities
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
   {
      ExcludeEntity( vDialogC, "CtrlMapLOD_Entity", zREPOS_PREV );
      SetCursorNextEntity( vDialogC, "CtrlMap", "" );
      if ( CheckExistenceOfEntity( vDialogC, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
         ExcludeEntity( vDialogC, "CtrlMapLOD_Entity", zREPOS_PREV );
   }

   RefreshCtrl( vSubtask, "EntityList" );
   RefreshCtrl( vSubtask, "ScopeList" );

   // Make sure we're positioned on first CtrlMap.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_Init
//
// PURPOSE:  This function:
//           1.  Sets up the TZPNCTWO work object
//           2.  Creates the first CtrlMap entity instance, if necessary
//           3.  Sets up the work ComboBox entity
//           4.  Sets up the Domain Context mapping
//           5.  Sets up the Event ListBox properly
//           6.  Switches to either UPD_COMBO_D or UPD_COMBO_S if
//               a Subtype has been previously defined.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vWork;
// zLONG    lCurrentSubtype;
   zLONG    lSubtype;
   zLONG    lFormatType;
   zCHAR    szStringSubtype[ 16 ];

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure ComboBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "ComboBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "ComboBox", zPOS_BEFORE );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Set ComboBox attribute values from Control.CtrlBOI
   SetEntityAttributesFromBlob( vWork, "ComboBox",
                                vDialogC, "Control", "CtrlBOI" );

   // Set Default value for format type - Drop Down List
   //
   if ( GetVariableFromAttribute( &lFormatType, 0, zTYPE_INTEGER, 0,
                                  vWork, "ComboBox", "FormatType", 0, 0 ) < 0 )
   {
       SetAttributeFromInteger( vWork, "ComboBox", "FormatType", 0 );
   }

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );

// if ( lSubtype & zCOMBOBOX_SORT )
//    SetAttributeFromString( vWork, "ComboBox", "SortByListValue", "Y" );
// else
//    SetAttributeFromString( vWork, "ComboBox", "SortByListValue", "N" );

   // If Subtype already is defined, switch to the proper window.
   GetStringFromAttribute( szStringSubtype, sizeof( szStringSubtype ), vWork, "ComboBox", "Subtype" );
   if ( szStringSubtype[ 0 ] != 0 )
   {
   // GetIntegerFromAttribute( &lCurrentSubtype, vWork,
   //                          "ComboBox", "Subtype" );
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", lSubtype );

      CreateViewFromViewForTask( &vWork, vDialogC, 0 );
      SetViewToSubobject( vWork, "CtrlCtrl" );
      SetNameForView( vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntity( vWork, "CtrlCtrl", 0 );
      if ( (lSubtype & 0x0000F000) == 0 )
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZPNCTAD", "UPD_COMBO_D" );
      else
         SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                                  "TZPNCTAD", "UPD_COMBO_S" );
   }

   // Set up the Event List Box Properly.
   fnSetupEventList( vSubtask, vDialogC );

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_SetScopeOI
//
// PURPOSE:  This function handles the setting of controls based on
//           the setting of the SCOPE Radio Buttons and Combo Boxes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_SetScopeOI( zVIEW vSubtask )
{
   zVIEW vScope;

   if ( GetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED ) )
   {
      GetViewByName( &vScope, "TZSCOPE", vSubtask, zLEVEL_TASK );

      // Exclude scoping entity
      if ( CheckExistenceOfEntity( vScope, "CtrlMapLOD_Entity" ) == zCURSOR_SET )
         ExcludeEntity( vScope, "CtrlMapLOD_Entity", zREPOS_PREV );

      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      RefreshCtrl( vSubtask, "ScopeList" );
   }
   else
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_SetSubtype
//
// PURPOSE:  This function brings up the correct Combo Box window
//           based on the Subtype selected.  This code is only
//           execute by the initial Combo Box select window,
//           UPD_COMBO.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_SetSubtype( zVIEW vSubtask )
{
   zVIEW    vWork;
   zLONG    lSubtype;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vWork, "ComboBox", "Subtype" );
   SetAttributeFromInteger( vWork, "TZPNCTWO",
                            "TempCurrentComboSubtype", lSubtype );

   if ( (lSubtype & 0x0000F000) == 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNCTAD", "UPD_COMBO_D" );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNCTAD", "UPD_COMBO_S" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTAD_SetActionAndEditState
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPNCTAD_SetActionAndEditState( zVIEW vSubtask )
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

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
} // zwTZPNCTAD_SetActionAndEditState

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: UPD_COMBO_D_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_D_PostBuild( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vTZWINDOWL;
   zLONG  lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & zCOMBOBOX_SORT )
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCOMBOBOX_EXTERNAL_DATA )
      SetCtrlState( vSubtask, "UserMap", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCOMBOBOX_NO_NULL )
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED, 1 );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "ControlSubtype", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "BoxStyle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UserMap", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: UPD_COMBO_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_S_PostBuild( zVIEW vSubtask )
{
   zVIEW vControl;
   zVIEW vTZWINDOWL;
   zLONG lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( lSubtype & zCOMBOBOX_SORT )
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCOMBOBOX_SCOPE_OI )
   {
      SetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED, 1 );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
   }

   if ( lSubtype & zCOMBOBOX_NO_NULL )
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED, 1 );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "ControlSubtype", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "BoxStyle", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_DS_SetSubtype
//
// PURPOSE:  This function looks at the ComboBox Subtype value and
//           switches to the other Window if the Subtype changes from
//           Domain to another or from another to Domain.
//          (See UPD_COMBO_SelAttrPullDown for related doc.)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_DS_SetSubtype( zVIEW vSubtask )
{
   zVIEW    vWork;
   zLONG    lCurrentSubtype;
   zLONG    lNewSubtype;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lCurrentSubtype, vWork,
                            "TZPNCTWO", "TempCurrentComboSubtype" );
   GetIntegerFromAttribute( &lNewSubtype, vWork, "ComboBox", "Subtype" );
   if ( (lCurrentSubtype & 0x0000F000) == 0 &&
        (lNewSubtype & 0x0000F000) != 0 )
   {
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", lNewSubtype );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNCTAD", "UPD_COMBO_S" );
   }

   if ( (lCurrentSubtype & 0x0000F000) != 0 &&
        (lNewSubtype & 0x0000F000) == 0 )
   {
      SetAttributeFromInteger( vWork, "TZPNCTWO",
                               "TempCurrentComboSubtype", lNewSubtype );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNCTAD", "UPD_COMBO_D" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_D_OK
//
// PURPOSE:  This function sets up X_ViewName, X_EntityName, etc.
//           attributes for interim compatibility.
//           It also sets up the Control.CtrlBOI and Control.Subtype
//           attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_D_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;
   zLONG    lWorkNew;
   zLONG    lWorkOrig;
   zLONG    lFormat;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   if ( vWork )
      DropView( vWork );

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set Control.CtrlBOI from work entity ComboBox
   SetBlobFromEntityAttributes( vDialog, "Control", "CtrlBOI",
                                vWork, "ComboBox" );

   // Build Control.Subtype attribute.
   GetIntegerFromAttribute( &lWorkOrig, vDialog, "Control", "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "ComboBox", "Subtype" );
   GetIntegerFromAttribute( &lFormat, vWork, "ComboBox", "FormatType" );
   lWorkNew |= lFormat;

   // Blank out the low order bits in the original Subtype, because
   // we are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;
   if ( GetCtrlState( vSubtask, "UserMap", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_EXTERNAL_DATA;
   else
      lWorkOrig &= ~zCOMBOBOX_EXTERNAL_DATA;

   // Get the sort option for combo.
   if ( GetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_SORT;
   else
      lWorkOrig &= ~zCOMBOBOX_SORT;

   // KELLY
   if ( GetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_NO_NULL;

   SetAttributeFromInteger( vDialog, "Control", "Subtype", lWorkOrig );

   // Delete any additional CtrlMap entities, if they exist.
   SetCursorFirstEntity( vDialog, "CtrlMap", "" );
   if ( SetCursorNextEntity( vDialog, "CtrlMap", "" ) == zCURSOR_SET )
   {
      do
         DeleteEntity( vDialog, "CtrlMap", zREPOS_NONE );
      while ( SetCursorNextEntity( vDialog, "CtrlMap", "" ) == zCURSOR_SET );
   }

   // Accept the Control.
   AcceptSubobject( vDialog, "Control" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_S_Init
//
// PURPOSE:  This function:
//           1.  Creates the 3 extra CtrlMap entities, if they don't
//               already exist.
//           2.  Names the 3 extra CtrlMap entities at the TASK level.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_S_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vDialogA;
   zVIEW    vDialogE;
   zVIEW    vDialogS;
   zVIEW    vDialogL;
   zVIEW    vWork;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Make sure the last three entities are named at TASK level.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );
   CreateViewFromViewForTask( &vDialogA, vDialogC, 0 );
   SetCursorNextEntity( vDialogA, "CtrlMap", "" );
   SetNameForView( vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialogE, vDialogA, 0 );
   SetCursorNextEntity( vDialogE, "CtrlMap", "" );
   SetNameForView( vDialogE, "TZPULL_Entity", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialogS, vDialogE, 0 );
   SetCursorNextEntity( vDialogS, "CtrlMap", "" );
   SetNameForView( vDialogS, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // If combo box is going to only display selected values.
   CreateViewFromViewForTask( &vDialogL, vDialogS, 0 );
   if ( SetCursorNextEntity( vDialogL, "CtrlMap", "" ) != 0 )
      CreateMetaEntity( vSubtask, vDialogL, "CtrlMap", zPOS_AFTER );

   SetNameForView( vDialogL, "TZSELLIST", vSubtask, zLEVEL_TASK );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   fnSetUpSEL_LOD( vDialogA, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   fnSetUpScopeListOI( vDialogE, vSubtask, "CtrlMapLOD_Entity" );

   // Set up the TempViewName attribute in the work object if there is
   // current TZPULL_Attribute mapping.
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork,    "TZPNCTWO",    "TempViewName",
                                 vDialogA, "CtrlMapView", "Name" );

   // Make sure MapDomain is set up for the edit box mapping.
   if ( GetViewByName( &vDomain, "MapDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   }

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_S_OK
//
// PURPOSE:  This function sets up X_ViewName, X_EntityName, etc.
//           attributes for interim compatibility.
//           It also sets up the Control.CtrlBOI and Control.Subtype
//           attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_S_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;
   zLONG    lWorkNew;
   zLONG    lWorkOrig;
   zLONG    lFormat;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   if ( vWork )
      DropView( vWork );

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set Control.CtrlBOI from work entity ComboBox
   SetBlobFromEntityAttributes( vDialog, "Control", "CtrlBOI",
                                vWork, "ComboBox" );

   // Build Control.Subtype attribute
   GetIntegerFromAttribute( &lWorkOrig, vDialog, "Control", "Subtype" );
   GetIntegerFromAttribute( &lWorkNew, vWork, "ComboBox", "Subtype" );
   GetIntegerFromAttribute( &lFormat, vWork, "ComboBox", "FormatType" );
   lWorkNew |= lFormat;

   // Blank out the low order bits in the original Subtype, because
   // we are writing over them with lWorkNew.
   lWorkOrig &= 0xFFFF0000L;
   lWorkOrig |= lWorkNew;

   // Get the sort option for combo.
   if ( GetCtrlState( vSubtask, "Sort", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_SORT; // 16
   else
      lWorkOrig &= ~zCOMBOBOX_SORT;

// KELLY
   if ( GetCtrlState( vSubtask, "ScopeOI", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_SCOPE_OI;

   if ( GetCtrlState( vSubtask, "NoNull", zCONTROL_STATUS_CHECKED ) )
      lWorkOrig |= zCOMBOBOX_NO_NULL;

   if ( !ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialog );

   SetAttributeFromInteger( vDialog, "Control", "Subtype", lWorkOrig );

   if ( !ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vDialog );

   SetCursorFirstEntity( vDialog, "CtrlMap", "" );

   // Accept the Control.
   AcceptSubobject( vDialog, "Control" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_S_ReturnFromSubwindow
//
// PURPOSE:  This function does the following:
//           1.  It makes sure that TZCONTROL is positioned on the
//               first CtrlMap instance, since going to the SEL_ATTR
//               subwindow may have repositioned it.
//           2.  It executes the ReturnFromSubwindow code from LISTBOX
//               to set up the object for the Event list box.
//           3.  It resets cursors in the TZPULL_Attribute view,
//               since an exclude/include may have been done with
//               a different view.
//           4.  It excludes the Entity and Scope LOD_Entity
//               relationships, if the mapping for TZPULL_Attribute
//               was changed to a different view.  In that case, it
//               also includes the new Entity, assuming the Entity
//               is the same as that for the Attribute.
//           5.  It sets up the Entity pull down OI for the Entity
//               combo box.
//           6.  It sets up the Scope pull down OI for the Scope
//               combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_S_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDialogE;
   zVIEW    vDialogW;
   zVIEW    vDialogS;
   zVIEW    vWork;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

   GetViewByName( &vDialogE, "TZPULL_Entity", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );

   // Execute the ReturnFromSubwindow code from LISTBOX to set up
   // the object for the Event list box.
   UPD_LISTBOX_ReturnFromSubwindow( vSubtask );

   // Reset cursors for CtrlMap subobject view vDialogA
   SetCursorPrevEntity( vDialogA, "CtrlMap", "" );
   SetCursorNextEntity( vDialogA, "CtrlMap", "" );

   // If the mapping for TZPULL_Attribute was changed to a different view
   // or eliminated, exclude the Entity and Scope LOD_Entity relationships.
   //(CtrlMap.X_ViewName was set to the last view mapping.)
   // Also include new Entity, based on the TZPULL_Attribute.
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( (CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) != zCURSOR_SET) ||
      (CompareAttributeToAttribute( vWork, "TZPNCTWO", "TempViewName",
                                    vDialogA, "CtrlMapView", "Name" ) != 0 ) )
   {
      if ( CheckExistenceOfEntity( vDialogE,
                                   "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialogE, "CtrlMapLOD_Entity", zREPOS_PREV );
      }

      GetViewByName( &vDialogS, "TZSCOPE", vSubtask, zLEVEL_TASK );
      if ( CheckExistenceOfEntity( vDialogS,
                                   "CtrlMapLOD_Entity" ) == zCURSOR_SET )
      {
         ExcludeEntity( vDialogS, "CtrlMapLOD_Entity", zREPOS_PREV);
      }
   }

   // Set up SEL_LOD for EntityList combo box.
   fnSetUpSEL_LOD( vDialogA, vDialogW, vSubtask, "CtrlMapView" );

   // Initialize ScopeL OI for ScopeList combo box.
   fnSetUpScopeListOI( vDialogE, vSubtask, "CtrlMapLOD_Entity" );

   // Make sure MapDomain is set up for the edit box mapping.
   if ( GetViewByName( &vDomain, "MapDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   }

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_S_SelAttrPullDown
//
// PURPOSE:  This function positions TZCONTROL to the 2nd CtrlMap
//           instance for going to the SEL_ATTR subwindow.
//          (See UPD_COMBO_ReturnFromSubwindow for related doc.)
//           It also sets X_ViewName, so that the return code can
//           check to see if the View mapping changed.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_S_SelAttrPullDown( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vWork;

   GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork, "TZPNCTWO", "TempViewName",
                                 vDialogA, "CtrlMapView", "Name" );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetCursorNextEntity( vDialogC, "CtrlMap", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_GRAPH_Init
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_GRAPH_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vDialogA;
   zVIEW    vDialogS;
   zVIEW    vWork;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Make sure ComboBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "Graph" ) != zCURSOR_SET )
      CreateEntity( vWork, "Graph", zPOS_BEFORE );

   // Create the 1 extra CtrlMap entities, if they don't already exist.
   if ( SetCursorNextEntity( vDialogC, "CtrlMap", "" ) != zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vDialogC, "CtrlMap", zPOS_AFTER );
      CreateMetaEntity( vSubtask, vDialogC, "CtrlMap", zPOS_AFTER );
   }

   // Set Graph attribute values from Control.CtrlBOI
   SetEntityAttributesFromBlob( vWork, "Graph",
                                vDialogC, "Control", "CtrlBOI" );

   // Make sure the last three entities are named at TASK level.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );
   CreateViewFromViewForTask( &vDialogA, vDialogC, 0 );
   SetCursorNextEntity( vDialogA, "CtrlMap", "" );
   SetNameForView( vDialogA, "TZDATA_Attrib", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialogS, vDialogA, 0 );
   SetCursorNextEntity( vDialogS, "CtrlMap", "" );
   SetNameForView( vDialogS, "TZSCOPE", vSubtask, zLEVEL_TASK );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   fnSetUpSEL_LOD( vDialogA, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box.
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapRelatedEntity" ) == 0 )
      fnSetUpScopeListOI( vDialogA, vSubtask, "CtrlMapRelatedEntity" );
   else
      fnSetUpScopeListOI( vDialogC, vSubtask, "CtrlMapRelatedEntity" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_GRAPH_SelDataAttrib
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_GRAPH_SelDataAttrib( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   /*
   GetViewByName( &vDialogA, "TZDATA_Attrib", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork, "TZPNCTWO", "TempViewName",
                                  vDialogA, "CtrlMapView", "Name" );
   */

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetCursorNextEntity( vDialogC, "CtrlMap", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_GRAPH_ReturnFromSubwindow
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_GRAPH_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_GRAPH_OK
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_GRAPH_OK( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Accept the Control.
   AcceptSubobject( vDialog, "Control" );

   // Set Control.CtrlBOI from work entity Graph
   SetBlobFromEntityAttributes( vDialog, "Control", "CtrlBOI",
                                vWork, "Graph" );

   SetCursorFirstEntity( vDialog, "CtrlMap", "" );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_CHECK_Init
//
// PURPOSE:  This function:
//           1.  Creates the first CtrlMap entity instance, if necessary
//           2.  Sets up the Work CheckBox entity
//           3.  Sets up the Domain Context mapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_CHECK_Init( zVIEW vSubtask )
{
   zVIEW   vDialogC;
   zVIEW   vDialogW;
   zVIEW   vDialogA;
   zVIEW   vWork;
   zPCHAR  pCtrlBOI;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vDialogC, "Control" );

   // Create the first CtrlMap entity instance, if necessary.
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMap" ) != zCURSOR_SET )
      CreateEntity( vDialogC, "CtrlMap", zPOS_BEFORE );

   // Make sure CheckBox entity exits in Work OI.
   if ( CheckExistenceOfEntity( vWork, "CheckBox" ) != zCURSOR_SET )
      CreateEntity( vWork, "CheckBox", zPOS_BEFORE );

   // Set CheckBox attribute values from Control.CtrlBOI.
   GetAddrForAttribute( &pCtrlBOI, vDialogC, "Control", "CtrlBOI" );
   if ( pCtrlBOI )
   {
      SetAttributeFromString( vWork, "CheckBox", "CheckOn", pCtrlBOI );
      while ( pCtrlBOI[ 0 ] )
         pCtrlBOI++;

      pCtrlBOI++;
      SetAttributeFromString( vWork, "CheckBox", "CheckOff", pCtrlBOI );
   }

   if ( GetViewByName( &vDialogA, "TZACTION", vSubtask, zLEVEL_TASK ) >= 0 )
      DropView( vDialogA );

   // If an Action for the Event exists, position on the linked Action
   // instance and set TZACTION to TZWINDOW.
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

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_CHECK_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_CHECK_Postbuild( zVIEW vSubtask )
{
   zVIEW   vTZWINDOWL;
   zVIEW   vDialogC;
   zPCHAR  pchStyle;
   zLONG   lSubtype;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   if ( lSubtype & zCHECKBOX_XCHECK )
      pchStyle = "X Mark";
   else
   if ( lSubtype & zCHECKBOX_LED )
      pchStyle = "LED";
   else
      pchStyle = "Check Mark";

   CB_SetData( vSubtask, "Style", pchStyle, "Check Mark;X Mark;LED", 0 );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "edChecked", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edUnchecked", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
} // UPD_CHECK_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_CHECK_OK
//
// PURPOSE:  This function sets Control.CtrlBOI values from CheckBox
//           entity and sets Context.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_CHECK_OK( zVIEW vSubtask )
{
   zVIEW   vDialogC;
   zVIEW   vWork;
   zCHAR   szCtrlBOI[ 256 ];
   zCHAR   szStyle[ 32 ];
   zLONG   lSubtype;
   zSHORT  nLth;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vDialogC, "Control", "Subtype" );
   lSubtype &= ~(zCHECKBOX_XCHECK | zCHECKBOX_LED);
   CB_GetSelectedString( vSubtask, "Style", szStyle, sizeof( szStyle ) );
   if ( zstrcmp( szStyle, "X Mark" ) == 0 )
      lSubtype |= zCHECKBOX_XCHECK;
   else
   if ( zstrcmp( szStyle, "LED" ) == 0 )
      lSubtype |= zCHECKBOX_LED;

   SetAttributeFromInteger( vDialogC, "Control", "Subtype", lSubtype );

   // Accept the Control.
   AcceptSubobject( vDialogC, "Control" );

   // Set the check value in the Control entity.
   SetAttributeFromAttribute( vDialogC, "Control",  "RadioOrCheckboxValue",
                              vWork,    "CheckBox", "CheckOn" );

   // Set CtrlBOI values

   // Null out edit text since it has no meaning and we prefer the
   // tag to be displayed in the painter
   GetStringFromAttribute( szCtrlBOI, sizeof( szCtrlBOI ), vWork, "CheckBox", "CheckOn" );
   nLth = 0;
   while ( szCtrlBOI[ nLth ] )
      nLth++;

   nLth++;
   GetStringFromAttribute( szCtrlBOI + nLth, sizeof( szCtrlBOI ) - nLth, vWork,
                           "CheckBox", "CheckOff" );
   while ( szCtrlBOI[ nLth ] )
      nLth++;

   nLth++; // with the code looking like this, I assume the extra byte
           // is for the null terminator.

   if ( !ComponentIsCheckedOut( vSubtask, vDialogC, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialogC );

   SetAttributeFromBlob( vDialogC, "Control", "CtrlBOI", szCtrlBOI, nLth );

   if ( !ComponentIsCheckedOut( vSubtask, vDialogC, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vDialogC );

   // Tell the Painter to refresh the control.
   RepaintZeidonControl( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ERD_PreBuild
//
// PURPOSE:  This function
//           1. Creates the second CtrlMap entity, if necessary.
//           2. Sets up the TZPULL_Entity View for the second CtrlMap
//              entity.
//           3. Sets up SEL_LOD View, if current mapping exists.  It
//              sets SEL_LOD to null otherwise.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ERD_PreBuild( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zVIEW    vDialogE;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   // Make sure we're positioned on first CtrlMap.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );

   // Create view name, TZPULL_Entity, for second CtrlMap entity and
   // create CtrlMap entity if it doesn't exist.
   CreateViewFromViewForTask( &vDialogE, vDialogC, 0 );
   SetNameForView( vDialogE, "TZPULL_Entity", vSubtask, zLEVEL_TASK );

   // If Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialogE );

   if ( SetCursorNextEntity( vDialogE, "CtrlMap", "" ) < 0 )
      CreateMetaEntity( vSubtask, vDialogE, "CtrlMap", zPOS_AFTER );

   // Set up the Event List Box Properly.
   fnSetupEventList( vSubtask, vDialogC );

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required.
   fnSetUpSEL_LOD( vDialogC, vDialogW, vSubtask, "CtrlMapView" );

   // If Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDialogW, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vDialogE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ERD_SelectView
//
// PURPOSE:  This function
//           1. Activates the LOD for the View just selected.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ERD_SelectView( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zVIEW    vDialogW;
   zVIEW    vControl;
   zLONG    lZKey;
   zBOOL    bEnable = TRUE;
   zCHAR    szViewName[ 33 ];

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Activate the LOD for the current ViewObjRef subobject.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );
   GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );
   SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vControl, "CtrlMap", "" );
   if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) >= zCURSOR_SET )
      ExcludeEntity( vControl, "CtrlMapLOD_Entity", zREPOS_NONE );

   SetCursorNextEntity( vControl, "CtrlMap", "" );
   if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) >= zCURSOR_SET )
      ExcludeEntity( vControl, "CtrlMapLOD_Entity", zREPOS_NONE );

   SetCursorFirstEntity( vControl, "CtrlMap", "" );

   GetCtrlText( vSubtask, "ViewName", szViewName, 33 );
   if ( zstrcmp( szViewName, "" ) == 0 )
      bEnable = FALSE;

   SetCtrlState( vSubtask, "Entity1", zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "Entity2", zCONTROL_STATUS_ENABLED, bEnable );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZPNCTAD_TransferToEditor
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_TransferToEditor( zVIEW vSubtask )
{
   zVIEW vMeta;

   GetViewByName( &vMeta, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vMeta, "ActOper" ) == zCURSOR_SET )
   {
      SetCursorFirstEntityByAttr( vMeta, "Operation", "Name",
                                  vMeta, "ActOper", "Name", "" );
   }

   TransferToEditor( vSubtask, vMeta, "PN", 0 );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTAD_CommonDisableCtrls
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPNCTAD_CommonDisableCtrls( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_ENABLED, FALSE );

   SetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_ENABLED, FALSE );
   SetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // zwTZPNCTAD_CommonDisableCtrls

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZPNCTAD_CommonDetailPostBuild
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_CommonDetailPostBuild( zVIEW vSubtask )
{
   zVIEW vControl;
   zLONG lSubtype;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( lSubtype & zCONTROL_SIZEABLEBORDER )
         SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_INVISIBLE )
         SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_DISABLED )
         SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_NOTABSTOP )
         SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_RELATIVE_POS_X )
         SetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_RELATIVE_POS_Y )
         SetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_RELATIVE_SIZE_X )
         SetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_CHECKED, 1 );

      if ( lSubtype & zCONTROL_RELATIVE_SIZE_Y )
         SetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_CHECKED, 1 );
   }

// we need this (at least for edit boxes) dks 2005.01.31 #if 0  // DonC 12/31/03 ???
   if ( (lSubtype & zCONTROL_BORDEROFF) == 0 )
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_RIGHTJUSTIFY )
      SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_CENTERJUSTIFY )
      SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED, 1 );
// #endif

   if ( lSubtype & zCONTROL_FULL_CLIENT )
      SetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_CHECKED, 1 );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "ExtendedStyle" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
   // ED_LimitTextLength( vSubtask, "DIL_Text", 126 ); too short for web ... leave at 254 dks 2009.05.20
      if ( lSubtype & zCONTROLX_DISABLE_READONLY )
         SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, 1 );
   }

   if ( lSubtype & zCONTROLX_PREFILL )
      SetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_SIZE )
      SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_POS )
      SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED, 1 );

   if ( MiGetUpdateForView( vControl ) == 0 )
      zwTZPNCTAD_CommonDisableCtrls( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZPNCTAD_CommonDetailOK
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_CommonDetailOK( zVIEW vSubtask )
{
   zVIEW  vControl;
   zLONG  lSubtype;
// zSHORT nRC;
// zBOOL  bAllSet;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vControl ) == 0 )
      return( 0 );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
// TraceLineI( "BEFORE Common OK ", lSubtype );

   // For the Common Detail stuff, we want to hold on to the low order bits,
   // and reset the high order bits according to the checkboxes on the
   // common detail window.
   lSubtype &= 0x0000FFFFL;

   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( GetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_SIZEABLEBORDER;

      if ( GetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_INVISIBLE;

      if ( GetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED ) )
      {
         lSubtype |= zCONTROL_DISABLED;
         SetAttributeFromString( vControl, "Control", "Disabled", "Y" );
      }
      else
         SetAttributeFromString( vControl, "Control", "Disabled", "N" );

      if ( GetCtrlState( vSubtask, "AutoReposX", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_POS_X;

      if ( GetCtrlState( vSubtask, "AutoReposY", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_POS_Y;

      if ( GetCtrlState( vSubtask, "AutoResizeX", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_SIZE_X;

      if ( GetCtrlState( vSubtask, "AutoResizeY", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_RELATIVE_SIZE_Y;

      if ( GetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_NOTABSTOP;

   } //endif ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )

   if ( GetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED ) == 0 )
      lSubtype |= zCONTROL_BORDEROFF;

   if ( GetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROL_RIGHTJUSTIFY;

   if ( GetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROL_CENTERJUSTIFY;

   if ( GetCtrlState( vSubtask, "FullClient", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROL_FULL_CLIENT;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "ExtendedStyle" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( GetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROLX_DISABLE_READONLY;
      else
         lSubtype &= ~zCONTROLX_DISABLE_READONLY;
   }

   if ( GetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLX_PREFILL;

   if ( GetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLX_ABSOLUTE_PIXEL_SIZE;
   else
      lSubtype &= ~zCONTROLX_ABSOLUTE_PIXEL_SIZE;

   if ( GetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED ) )
      lSubtype |= zCONTROLX_ABSOLUTE_PIXEL_POS;
   else
      lSubtype &= ~zCONTROLX_ABSOLUTE_PIXEL_POS;

   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      SetAttributeFromInteger( vControl, "Control", "ExtendedStyle", lSubtype );

#if 0
      // Loop through all the CtrlColorDefs.  If they all have CtrlColors set
      // to COLOR_WINDOW_DFLT, then delete all the CtrlColorDefs.
      bAllSet = TRUE;
      nRC = SetCursorFirstEntity( vControl, "CtrlClrOverride", "" );
      while ( nRC >= zCURSOR_SET && bAllSet )
      {
         if ( CompareAttributeToInteger( vControl, "CtrlColor",
                                         "RGB_Color", zCOLOR_WINDOW_DFLT ) != 0 )
         {
            bAllSet = FALSE;
         }

         nRC = SetCursorNextEntity( vControl, "CtrlClrOverride", "" );
      }

      if ( bAllSet == TRUE )
      {
         nRC = SetCursorFirstEntity( vControl, "CtrlClrOverride", "" );
         while( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vControl, "CtrlClrOverride", zREPOS_NEXT );
      }

      // Loop through all the CtrlFontDefs.  If they all have CtrlFonts set
      // to FONT_WINDOW_DFLT, then delete all the CtrlFontDefs.
      bAllSet = TRUE;
      nRC = SetCursorFirstEntity( vControl, "CtrlFontOverride", "" );
      while ( nRC >= zCURSOR_SET && bAllSet )
      {
         if ( CompareAttributeToString( vControl, "CtrlFont",
                                        "Tag", "Windows Default" ) != 0 )
         {
            bAllSet = FALSE;
         }

         nRC = SetCursorNextEntity( vControl, "CtrlFontOverride", "" );
      }

      if ( bAllSet == TRUE )
      {
         nRC = SetCursorFirstEntity( vControl, "CtrlFontOverride", "" );
         while( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vControl, "CtrlFontOverride", zREPOS_NEXT );
      }
#endif

   }   //endif ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
Action_Popup( zVIEW vSubtask )
{
   zwTZPNCTAD_LoadEventPopups( vSubtask, "SelectUpdate", "NewAction",
                               "SelectAction", "RemoveAction", "",
                               "Action_Popup" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ControlListAddAction
//
// PURPOSE:  This function creates an Action entity and includes it to.
//           the selected control.  It is used from Action_Popup for List
//           and combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ControlListAddAction( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW vDialogW;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   nRC = UPD_LISTBOX_SelectAction( vSubtask );
   if ( nRC < 0 )
      return( -1 );

   // Create and initialize the Action.
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );

   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", "Type", 1 );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ControlAddAction
//
// PURPOSE:  This function creates an Action entity and includes it to
//           the selected control.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ControlAddAction( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW vDialogW;

   nRC = UPD_PUSH_SelectAction( vSubtask );
   if ( nRC < 0 )
      return( -1 );

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // Create and initialize the Action.
   if ( CheckExistenceOfEntity( vDialogW, "Action" ) >= zCURSOR_SET )
      SetSelectStateOfEntityForSet( vDialogW, "Action", 0, 1 );

   CreateTemporalMetaEntity( vSubtask, vDialogW, "Action", zPOS_AFTER );
   SetAttributeFromString( vDialogW, "Action", "Tag", "New" );
   SetAttributeFromInteger( vDialogW, "Action", "Type", 1 );
   SetSelectStateOfEntityForSet( vDialogW, "Action", 1, 1 );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ControlOperationEdit
//
// PURPOSE:  This function calls editor after positioning on operation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ControlOperationEdit( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zCHAR    szTag[ 34 ];

   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vDialogC, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( szTag, sizeof( szTag ), vDialogC, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vDialogW, "Action", "Tag", szTag, "" );

      if ( CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         SetCursorFirstEntityByAttr( vDialogW, "Operation", "Name",
                                     vDialogW, "ActOper", "Name", "Dialog" );
         TransferToEditor( vSubtask, vDialogW, "PN", 0 );
      }
      else
      {
         MessageSend( vSubtask, "PN00109", "Dialog Maintenance",
                      "No Operation exists for Action.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   {
      MessageSend( vSubtask, "PN00110", "Dialog Maintenance",
                   "No Action exists for Event.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_ACT_BuildWindowList
//
// Purpose:  List windows for the given dialog for window combo box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_BuildWindowList( zVIEW vSubtask )
{
   zVIEW    vDialogList;
   zVIEW    vWindowList;
   zVIEW    vDialogW;
   zVIEW    vCurrentW;
   zSHORT   nRC;

   GetViewByName( &vDialogList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCurrentW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

// See if dialog name is null, if so do not recreate window list view.

   nRC = SetCursorFirstEntityByAttr( vDialogList, "W_MetaDef", "Name",
                                     vDialogW, "Action", "DialogName", 0 );

   if ( nRC != zCURSOR_SET )
   {
      SetAttributeFromString( vDialogW, "Action", "WindowName", "" );
      GetViewByName( &vWindowList, "TZWND_List", vSubtask, zLEVEL_TASK );
      if ( vWindowList )
         DropMetaOI( vSubtask, vWindowList );

      RefreshCtrl( vSubtask, "WindowList" );
      return( 0 );
   }


// If window list exists, for correct dialog, return.
// else, space out windowname attribute and rebuild the window list

   nRC = GetViewByName( &vWindowList, "TZWND_List", vSubtask, zLEVEL_TASK );

   if ( vWindowList )
   {
     if ( CompareAttributeToAttribute( vDialogList, "W_MetaDef", "CPLR_ZKey",
                                       vWindowList, "Dialog", "ZKey" ) == 0 )
        return( 0 );
     else
     {
        if ( vWindowList )
           DropMetaOI( vSubtask, vWindowList );
     }
   }

// If dialog is open dialog, use existing window list, else create a
// new one.

   if ( CompareAttributeToAttribute( vDialogList, "W_MetaDef", "CPLR_ZKey",
                                     vCurrentW, "Dialog", "ZKey" ) == 0 )
   {
      CreateViewFromViewForTask( &vWindowList, vCurrentW, 0 );
   }
   else
   {
      ActivateMetaOI( vSubtask, &vWindowList, vDialogList, zREFER_DIALOG_META, zSINGLE );
      if ( SetCursorFirstEntityByAttr( vWindowList, "Window", "Tag",
                                       vDialogW, "Action", "WindowName",
                                       "" ) != zCURSOR_SET )
      {
         SetAttributeFromString( vDialogW, "Action", "WindowName", "" );
      }
   }

   nRC = SetNameForView( vWindowList, "TZWND_List", vSubtask, zLEVEL_TASK );
   nRC = RefreshCtrl( vSubtask, "WindowList" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   UPD_ACT_SetupDialogList
//
// Purpose: Setup Dialog/Window List for selection Combo Box
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_ACT_SetupDialogList( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zVIEW    vDialogList;
   zVIEW    vWindowList;
   zVIEW    vTZDIALOGS;
   zSHORT   nRC;

   // Build Dialog list for Combo box.

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vDialogList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   if ( nRC <= 0 )
   {
       GetViewByName( &vTZDIALOGS, "TZDIALOGS", vSubtask, zLEVEL_ANY );
       CreateViewFromViewForTask( &vDialogList, vTZDIALOGS, 0 );
       SetNameForView( vDialogList, "TZDLG_List", vSubtask, zLEVEL_TASK );
   }

   nRC = SetCursorFirstEntityByAttr( vDialogList, "W_MetaDef", "Name",
                                     vDialogW, "Action", "DialogName", 0 );

   if ( nRC == zCURSOR_SET )
   {
      UPD_ACT_BuildWindowList( vSubtask );
   }
   else
   {
      GetViewByName( &vWindowList, "TZWND_List", vSubtask, zLEVEL_TASK );
      if ( nRC > 0 )
         DropMetaOI( vSubtask, vWindowList );
   }


   return( 0 );
}

// #define DEBUG_TAG

#define OLS_OPENALLCHILDREN        0x0002L
#define OLS_NOSHOW_ENTITY          0x0010L
#define ALS_MULTISEL               0x1000L
#define OLS_MULTISEL               0x2000L
#define OLS_DUAL                   0x4000L
#define OLS_NOSHOW_TOPNODE         0x0040L

#define OL_OPEN_CHILDREN           0x00000040L
#define OL_RECURSIVE_ENTITY        0x00000080L

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_Init
//
// PURPOSE:  This function loads in TZOUTLINER for the executing outliner
//           control(the outliner is used to create the outliner) and
//           then calls fnResetOutliner to select any LOD_EntityParent's
//           or ER_AttributeRec's selected by the user.
//           This function is called when double-clicking on an
//           outliner control in the painter(for update of the
//           outliner control).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_Init( zVIEW vSubtask )
{
   zCHAR   szTempString[ 256 ];
   zCHAR   szTempString2[ 33 ];
   zVIEW   vControl;
   zVIEW   vWork;
   zVIEW   vVOR;
   zVIEW   vLOD;
   zVIEW   vLOD2;
   zVIEW   vTempCtrl;
   zVIEW   vLOD_List;
   zLONG   lSubtype;
   zULONG  ulLth = sizeof( szTempString );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vWork, "OutlinerGrp" ) != zCURSOR_SET )
      CreateEntity( vWork, "OutlinerGrp", zPOS_BEFORE );

   CreateTemporalSubobjectVersion( vControl, "Control" );

   /* if a CtrlMapView exists for this control, there is no need to go to
      the OUTLINER_Object window, go to UPD_OUTLINER window instead */
   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
   {
      zVIEW vTop;

      CreateViewFromViewForTask( &vTop, vControl, 0 );
      if ( SetCursorNextEntity( vTop, "CtrlMap", "" ) != zCURSOR_SET )
         CreateMetaEntity( vSubtask, vTop, "CtrlMap", zPOS_AFTER );

      SetNameForView( vTop, "TZCTLMAP2", vSubtask, zLEVEL_TASK );
      SetWindowActionBehavior( vSubtask, zWAB_ReplaceWindowWithModalWindow,
                               "TZPNOLAD", "UPD_OUTLINER" );

      // load the LOD definition for outliners mapping view and name
      // it TZOUTLINER.
      GetViewByName( &vVOR, "TZWINDOWL", vSubtask, zLEVEL_TASK );
//x   SetCursorFirstEntityByEntityCsr( vVOR, "ViewObjRef",
//x                                    vControl, "CtrlMapView", 0 );
//x   GetStringFromAttribute( szTempString, sizeof( szTempString ), vVOR, "LOD", "Name" );
      GetStringFromAttribute( szTempString, sizeof( szTempString ), vControl,
                              "CtrlMapView", "Name" );
      RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
//    strcpy_s( szTempString, sizeof( szTempString ), "TZZOLODO" ); use if modifying tzpnolad.pwd
//    TraceLineS( "Going after LOD - ", szTempString );
      SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                    "Name", szTempString, "" );

//    SetNameForView( vLOD_List, "DKS", 0, zLEVEL_SUBTASK );
//    DisplayObjectInstance( vLOD_List, 0, 0 );
//
// DOUG - This OperatorPrompt should be converted to a
//        call to MessageSend. See calls to MessageSend
//        in this module. DM
//    OperatorPrompt( vSubtask, szTempString,
//                    "Looking for LOD in list",
//                    0, 0, 0, zICON_INFORMATION );


//    ActivateMetaOI_ByName( vSubtask, &vLOD, 0, zREFER_LOD_META,
//                           zSINGLE | zLEVEL_APPLICATION,
//                           szTempString, TRUE );
//
      // This next monkey business is to get hold of a meta-type view that
      // is not read-only.
      ActivateMetaOI( vSubtask, &vLOD2, vLOD_List, zREFER_LOD_META, 0 );
      ActivateOI_FromOI_ForTask( &vLOD, vLOD2, 0, zSINGLE );
      DropMetaOI( vSubtask, vLOD2 );

      SetNameForView( vLOD, "TZOUTLINER", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vTempCtrl, vControl, 0 );
      GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
      if ( lSubtype & OLS_DUAL )
         SetAttributeFromString( vWork, "OutlinerGrp", "DualMode", "1" );

      if ( lSubtype & OLS_MULTISEL )
         SetAttributeFromString( vWork, "OutlinerGrp", "MultiSelect", "1" );

      if ( lSubtype & OLS_OPENALLCHILDREN )
         SetAttributeFromString( vWork, "OutlinerGrp", "AllChildren", "1" );

      if ( !(lSubtype & OLS_NOSHOW_TOPNODE) )
         SetAttributeFromString( vWork, "OutlinerGrp", "DisplayText", "1" );

      if ( !(lSubtype & OLS_NOSHOW_ENTITY) )
         SetAttributeFromString( vWork, "OutlinerGrp", "RootIndicator", "1" );

      /* if there are CtrlCtrl's under the outliner control then the
         entities and attributes that are mapped to each CtrlCtrl were
         previously selected by the user so they must be re-selected */
      SetViewToSubobject( vTempCtrl, "CtrlCtrl" );
      fnResetOutliner( vLOD, vTempCtrl );
      ResetView( vLOD );

      // get the scoping entity from the control's CtrlBOI and set it into the OL_Item data structure
      GetBlobFromAttribute( szTempString, &ulLth, vControl,
                            "Control", "CtrlBOI" );
      szTempString[ ulLth ] = 0;
      if ( ulLth > 0 )
      {
         strcpy_s( szTempString2, sizeof( szTempString2 ), szTempString );
         SetAttributeFromString( vWork, "OutlinerGrp",
                                 "Operation", szTempString );
         ulLth = zstrlen( szTempString ) + 1;
         strcpy_s( szTempString2, sizeof( szTempString2 ), szTempString + ulLth );
         SetAttributeFromString( vWork, "OutlinerGrp",
                                 "DLL_Name", szTempString + ulLth );
      }
   }

   // Set up the Event List Box properly
   fnSetupEventList( vSubtask, vControl );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnResetOutliner
//
// PURPOSE: A recursive function that selects either a LOD_EntityParent
//          or an ER_AttributeRec in TZOUTLINER based upon the CtrlCtrls
//          under the current outliner control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnResetOutliner( zVIEW vLOD,
                 zVIEW vControl )
{
   zPCHAR pch;
   zLONG  lSubtype;
   zSHORT nRC;

   // Loop through all the controls(CtrlCtrl).
   nRC = SetCursorFirstEntity( vControl, "Control", 0 );
   while ( nRC == zCURSOR_SET )
   {
      // Loop through TZOUTLINER(vLOD) hierarchically stopping at each
      // LOD_EntityParent or LOD_EntityChild.  Then, compare vControl's
      // CtrlMapRelatedEntity to vLOD's LOD_EntityParent.  If they are
      // the same entity, then set the select state of LOD_EntityParent
      // to ON.  Also for vControl, compare CtrlMapLOD_Attribute with
      // vLOD's LOD_AttributeRecs and set the select state to ON for the
      // one that matches.
      ResetView( vLOD );
      nRC = SetEntityCursor( vLOD, "LOD_EntityParent", "ZKey",
                             zQUAL_ENTITYATTR | zRECURS | zPOS_FIRST,
                             vControl, "CtrlMapRelatedEntity", "ZKey", 0,
                             zSCOPE_OI, 0 );

      // set the select state ON.
      if ( nRC >= zCURSOR_SET )
      {
         nRC = SetSelectStateOfEntityForSet( vLOD, "LOD_EntityParent", 1, 1 );

         // if this entity was defined as a recursive entity, then set up
         // some attributes in the LOD.  The LOD attributes are not
         // originally intended for these values but we are using them for
         // our own purposes.
         GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
         if ( lSubtype & OL_RECURSIVE_ENTITY )
         {
            SetAttributeFromString( vLOD, "LOD_EntityParent",
                                    "Create", "R" );
            SetAttributeFromAttribute( vLOD, "LOD_EntityParent",
                                       "Desc", vControl,
                                       "Control", "Text" );
         }

         if ( lSubtype & OL_OPEN_CHILDREN )
            SetAttributeFromString( vLOD, "LOD_EntityParent",
                                    "Delete", "O" );
      }

      nRC = SetCursorFirstEntity( vControl, "CtrlMap", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         nRC = SetEntityCursor( vLOD, "LOD_AttributeRec", "ZKey",
                                zQUAL_ENTITYATTR | zRECURS | zPOS_FIRST,
                                vControl, "CtrlMapLOD_Attribute", "ZKey", 0,
                                zSCOPE_OI, 0 );
         /* set the select state ON */
         if ( nRC >= zCURSOR_SET )
         {
            /* we need to keep the order and the TOP_VIEW information
               in the CtrlMap since there are multiple mappings for
               each control.  We will use the tag as a tab-separated
               structure as follows:
                  byte(s)    use
                  -------   ---------------------------------------------------
                    0       Tag attribute(only one per entity - Y/N)
                    2       Selected attribute(Y/N)
                   4-6      Sequence number(001-999)
                   8-39     Context
            */
            SetAttributeFromAttribute( vLOD, "ER_AttributeRec", "Desc",
                                       vControl, "CtrlMap", "Tag" );
            GetAddrForAttribute( &pch, vControl, "CtrlMap", "Tag" );
#ifdef DEBUG_TAG
            TraceLineS( "ResetOutliner CtrlMap Tag = ", pch );
#endif
            if ( pch[ 2 ] == 'Y' )
               SetSelectStateOfEntityForSet( vLOD, "ER_AttributeRec", 1, 1 );
         }
#if 0
         else
         {
            GetAddrForAttribute( &pch, vControl, "CtrlMapER_Attribute", "Name" );
            TraceLineS( "NOT Selected ER_AttributeRec Name = ", pch );
         }
#endif

         nRC = SetCursorNextEntity( vControl, "CtrlMap", 0 );
      }

      SetViewToSubobject( vControl, "CtrlCtrl" );
      fnResetOutliner( vLOD, vControl );
      ResetViewFromSubobject( vControl );
      nRC = SetCursorNextEntity( vControl, "Control", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_ObjectOK
//
// PURPOSE:  This function creates a LOD view TZOUTLINER based on the
//           LOD selected by the user.  This is called from the
//           the OUTLINER_Object window(which is invoked when a user
//           wants to update outliner detail for the first time ...
//           when no object has yet been selected for display by the
//           outliner control).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_ObjectOK( zVIEW vSubtask )
{
   zCHAR  szTempString[ 256 ];
   zVIEW  vControl;
   zVIEW  vLOD;
   zVIEW  vLOD2;
   zVIEW  vVOR;
   zVIEW  vOL;
   zVIEW  vLOD_List;
   zSHORT nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vVOR, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      DropView( vOL );

   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) != 0 )
   {
      MessageSend( vSubtask, "PN00111", "Dialog Maintenance",
                   "An object must be selected!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   SetCursorFirstEntityByEntityCsr( vVOR, "ViewObjRef",
                                    vControl, "CtrlMapView", 0 );
   RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
   GetStringFromAttribute( szTempString, sizeof( szTempString ), vVOR, "LOD", "Name" );
   SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                 "Name", szTempString, "" );

   // This next monkey business is to get hold of a meta-type view that
   // is not read-only.
   ActivateMetaOI( vSubtask, &vLOD2, vLOD_List, zREFER_LOD_META, 0 );
   ActivateOI_FromOI_ForTask( &vLOD, vLOD2, 0, zSINGLE );
   DropMetaOI( vSubtask, vLOD2 );
   SetNameForView( vLOD, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_OnPick
//
// PURPOSE:  This function enables the buttons for the update window
//           appropriately.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_OnPick( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW  vOL;

   OL_GetCurrentEntityName( vSubtask, "Outliner", &vOL, szEntityName );
   if ( zstrcmp( szEntityName, "LOD_EntityParent" ) == 0 ||
        zstrcmp( szEntityName, "LOD_EntityChild" ) == 0 )
   {
      SetCtrlState( vSubtask, "UpdateEnt", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "UpdateAttr", zCONTROL_STATUS_ENABLED, FALSE );
   }
   else
   if ( zstrcmp( szEntityName, "ER_AttributeRec" ) == 0 )
   {
      SetCtrlState( vSubtask, "UpdateEnt", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UpdateAttr", zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "UpdateEnt", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "UpdateAttr", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_Attributes
//
// PURPOSE:  This function sets the TZOLOBWO view from the LOD
//           view of the attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_Attributes( zVIEW vSubtask )
{
   zCHAR  szTempString[ 256 ];
   zCHAR  szEntityName[ 33 ];
   zVIEW  vOL;
   zVIEW  vAttr;
   zSHORT nRC;
   zSHORT k;

   OL_GetCurrentEntityName( vSubtask, "Outliner", &vOL, szEntityName );
   CreateViewFromViewForTask( &vOL, vOL, 0 );

// cain't happen no more
// if ( zstrcmp( szEntityName, "ER_AttributeRec" ) != 0 )
// {
// Doug, change this to a MessageSend.
//    OperatorPrompt( vSubtask, "Update Error",
//                    "Cannot update information for this item!",
//                    0, 0, 0, zICON_INFORMATION );
//    SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
//    return( 0 );
// }


   nRC = SetEntityCursor( vOL, "ER_AttributeRec", 0,
                          zPOS_FIRST /* | zQUAL_SELECTED */,
                          0, 0, 0, 0, "LOD_EntityParent", 0 );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "PN00112", "Dialog Maintenance",
                   "There are no attributes for this entity!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError,
                               0, 0 );
      DropView( vOL );
      return( 0 );
   }

   if ( GetViewByName( &vAttr, "TZOLOBWO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vAttr );

   ActivateEmptyObjectInstance( &vAttr, "TZOLOBWO", vSubtask, zSINGLE );
   SetNameForView( vAttr, "TZOLOBWO", vSubtask, zLEVEL_TASK );
   CreateEntity( vAttr, "Object", zPOS_AFTER );
   CreateEntity( vAttr, "Entity", zPOS_AFTER );
   k = 0;
   while ( nRC >= 0 )
   {
      k++;
      CreateEntity( vAttr, "Attrib", zPOS_AFTER );
      SetAttributeFromAttribute( vAttr, "Attrib", "Name",
                                 vOL, "ER_AttributeRec", "Name" );
      SetAttributeFromAttribute( vAttr, "Attrib", "Entity",
                                 vOL, "LOD_EntityParent", "Name" );

      /* we need to keep the order and the TOP_VIEW information
         in the CtrlMap since there are multiple mappings for
         each control.  We will use the tag as a tab-separated
         structure as follows:
            byte(s)    use
            -------   ---------------------------------------------------
              0       Tag attribute(only one per entity - Y/N)
              2       Selected attribute(Y/N)
             4-6      Sequence number(001-999)
             8-39     Context
      */
      GetStringFromAttribute( szTempString, sizeof( szTempString ), vOL, "ER_AttributeRec", "Desc" );
#ifdef DEBUG_TAG
      TraceLineS( "OUTLINER_Attributes Desc = ", szTempString );
#endif
      if ( szTempString[ 0 ] && szTempString[ 1 ] == '\t' )
      {
         zPCHAR pch;

         szTempString[ 1 ] = szTempString[ 3 ] =
                             szTempString[ 7 ] = szTempString[ 40 ] = 0;
         SetAttributeFromString( vAttr, "Attrib", "Tag", szTempString );
         SetAttributeFromString( vAttr, "Attrib", "Idx", szTempString + 4 );
         pch = zstrchr( szTempString + 8, '\t' );
         if ( pch )
            *pch = 0;

         SetAttributeFromString( vAttr, "Attrib", "Context", szTempString + 8 );
      }
      else
      {
         SetAttributeFromString( vAttr, "Attrib", "Tag", "N" );
         SetAttributeFromInteger( vAttr, "Attrib", "Idx", 0 );
         SetAttributeFromString( vAttr, "Attrib", "Context", "" );
      }

      if ( GetSelectStateOfEntityForSet( vOL, "ER_AttributeRec", 1 ) )
         SetAttributeFromString( vAttr, "Attrib", "Selected", "Y" );

      nRC = SetEntityCursor( vOL, "ER_AttributeRec", 0,
                             zPOS_NEXT /* | zQUAL_SELECTED */,
                             0, 0, 0, 0, "LOD_EntityParent", 0 );
   }

   if ( CheckExistenceOfEntity( vAttr, "Attrib" ) == 0 )
      SetCursorFirstEntity( vAttr, "Attrib", 0 );

   DropView( vOL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_DisableSS_AttrName
//
// PURPOSE:  This function disables the Attribute name in the SS control
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_DisableSS_AttrName( zVIEW vSubtask )
{
//?SS_LockColumn( vSubtask, "SS", 1, TRUE );   // Attribute name
//?SS_LockColumn( vSubtask, "SS", 2, TRUE );   // Selected flag
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_AttrOK
//
// PURPOSE:  This function resets the LOD view from the TZOLOBWO
//           view of the attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_AttrOK( zVIEW vSubtask )
{
   zCHAR  szTempString[ 256 ];
   zVIEW  vOL;
   zVIEW  vAttr;
   zLONG  lIdx;
   zSHORT nRC;

   GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vOL, vOL, 0 );
   GetViewByName( &vAttr, "TZOLOBWO", vSubtask, zLEVEL_TASK );
   nRC = SetCursorFirstEntity( vAttr, "Attrib", 0 );
   while ( nRC >= 0 )
   {
      zSHORT nTagCnt = 0;

      SetEntityCursor( vOL, "ER_AttributeRec", "Name",
                       zPOS_FIRST | zQUAL_ENTITYATTR,
                       vAttr, "Attrib", "Name", 0,
                       "LOD_EntityParent", 0 );

      /* we need to keep the order and the TOP_VIEW information
         in the CtrlMap since there are multiple mappings for
         each control.  We will use the tag as a tab-separated
         structure as follows:
            byte(s)    use
            -------   ---------------------------------------------------
              0       Tag attribute(only one per entity - Y/N)
              2       Selected attribute(Y/N)
             4-6      Sequence number(001-999)
             8-39     Context
      */
      GetStringFromAttribute( szTempString, sizeof( szTempString ), vAttr, "Attrib", "Tag" );
      if ( szTempString[ 0 ] == 'Y' )
      {
         if ( nTagCnt )
         {
            MessageSend( vSubtask, "PN00113", "Dialog Maintenance",
                         "Only one attribute may be marked as the tag attribute!",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError,
                                     0, 0 );
            return( 0 );
         }
         else
            nTagCnt++;
      }

      szTempString[ 2 ] = 'N';  // assume not selected
      GetIntegerFromAttribute( &lIdx, vAttr, "Attrib", "Idx" );
      szTempString[ 6 ] =(zSHORT) (lIdx % 10) + '0';
      lIdx /= 10;
      szTempString[ 5 ] =(zSHORT) (lIdx % 10) + '0';
      lIdx /= 10;
      szTempString[ 4 ] =(zSHORT) (lIdx % 10) + '0';
      GetStringFromAttribute( szTempString + 8, sizeof( szTempString ) - 8, vAttr, "Attrib", "Context" );
      szTempString[ 1 ] = szTempString[ 3 ] = szTempString[ 7 ] = '\t';
#ifdef DEBUG_TAG
      TraceLineS( "OUTLINER_AttrOK Desc = ", szTempString );
#endif
      SetAttributeFromString( vOL, "ER_AttributeRec", "Desc", szTempString );

      nRC = SetCursorNextEntity( vAttr, "Attrib", 0 );
   }

   DropView( vOL );
   DropObjectInstance( vAttr );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_AttrCancel
//
// PURPOSE:  This function cancels the attribute update and drops
//           the object instance.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_AttrCancel( zVIEW vSubtask )
{
   zVIEW  vAttr;

   GetViewByName( &vAttr, "TZOLOBWO", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vAttr );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_ChangeView
//
// PURPOSE:  This function creates a LOD view TZOUTLINER based on the
//           LOD selected by the user from the list in the OUTLINER_UPD
//           window.  This is called from the the OUTLINER_UPD window
//           when the user wants the outliner control to map a different
//           LOD.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_ChangeView( zVIEW vSubtask )
{
   zCHAR  szTempString[ 256 ];
   zVIEW  vControl;
   zVIEW  vLOD;
   zVIEW  vLOD2;
   zVIEW  vVOR;
   zVIEW  vOL;
   zVIEW  vLOD_List;
   zSHORT nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vVOR, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      DropView( vOL );

   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == 0 )
   {
      RetrieveViewForMetaList( vSubtask, &vLOD_List, zREFER_LOD_META );
      GetStringFromAttribute( szTempString, sizeof( szTempString ), vVOR, "LOD", "Name" );
      TraceLineS( "Going after LOD - ", szTempString );
      SetCursorFirstEntityByString( vLOD_List, "W_MetaDef",
                                    "Name", szTempString, "" );

      // This next monkey business is to get hold of a meta-type view that
      // is not read-only.
      ActivateMetaOI( vSubtask, &vLOD2, vLOD_List, zREFER_LOD_META, 0 );
      ActivateOI_FromOI_ForTask( &vLOD, vLOD2, 0, zSINGLE );
      DropMetaOI( vSubtask, vLOD2 );

      SetNameForView( vLOD, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   }

   // Delete any existing CtrlCtrl's.
   nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", 0 );
   while ( nRC >= zCURSOR_SET )
      nRC = DeleteEntity( vControl, "CtrlCtrl", zREPOS_NEXT );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_SelectInit
//
// PURPOSE:  This function creates a LOD view TZOUTLINER based on the
//           LOD selected by the user.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_SelectInit( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW  vOL;
   zVIEW  vWork;
   zVIEW  vLOD;

   OL_GetCurrentEntityName( vSubtask, "Outliner", &vOL, szEntityName );

// cain't happen no more
// if ( zstrcmp( szEntityName, "LOD_EntityParent" ) != 0 &&
//      zstrcmp( szEntityName, "LOD_EntityChild" ) != 0 )
// {
// Doug, change this to MessageSend
//    OperatorPrompt( vSubtask, "Update Error",
//                    "Cannot update information for this item!",
//                    0, 0, 0, zICON_INFORMATION );
//    SetWindowActionBehavior( vSubtask, zWAB_TerminateActionForError, 0, 0 );
//    return( 0 );
// }

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vLOD, vOL, 0 );
   ResetView( vLOD );
   SetNameForView( vLOD, "TZPNRECURS", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vWork, "Outliner" ) != zCURSOR_SET )
      CreateEntity( vWork, "Outliner", zPOS_BEFORE );

   SetAttributeFromString( vWork, "Outliner", "EntityName", szEntityName );
   if ( CompareAttributeToString( vOL, szEntityName, "Create", "R" ) == 0 )
   {
      SetAttributeFromInteger( vWork, "Outliner", "Recursive", 1 );
      SetAttributeFromAttribute( vWork, "Outliner", "RecursiveEntityName",
                                 vOL, szEntityName, "Desc" );
   }
   else
   {
      SetAttributeFromInteger( vWork, "Outliner", "Recursive", 0 );
      SetAttributeFromString( vWork, "Outliner",
                              "RecursiveEntityName", 0 );
   }

   if ( CompareAttributeToString( vOL, szEntityName, "Delete", "O" ) == 0 )
      SetAttributeFromInteger( vWork, "Outliner", "OpenChildren", 1 );
   else
      SetAttributeFromInteger( vWork, "Outliner", "OpenChildren", 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_SelectOK
//
// PURPOSE:  This function creates a LOD view TZOUTLINER based on the
//           LOD selected by the user.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_SelectOK( zVIEW vSubtask )
{
   zCHAR  szEntityName[ 33 ];
   zVIEW  vOL;
   zVIEW  vWork;

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOL, "TZPNRECURS", vSubtask, zLEVEL_TASK );
   DropView( vOL );

   GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vWork, "Outliner", "EntityName" );
   if ( CompareAttributeToInteger( vWork, "Outliner", "Recursive", 1 ) == 0 )
   {
      SetAttributeFromString( vOL, szEntityName, "Create", "R" );
      SetAttributeFromAttribute( vOL, szEntityName, "Desc",
                                 vWork, "Outliner", "RecursiveEntityName" );
   }

   if ( CompareAttributeToInteger( vWork, "Outliner", "OpenChildren", 1 ) == 0 )
      SetAttributeFromString( vOL, szEntityName, "Delete", "O" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_SelectCancel
//
// PURPOSE:  This function creates a LOD view TZOUTLINER based on the
//           LOD selected by the user.?????????
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_SelectCancel( zVIEW vSubtask )
{
   zVIEW  vOL;

   GetViewByName( &vOL, "TZPNRECURS", vSubtask, zLEVEL_TASK );
   DropView( vOL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_OK
//
// PURPOSE: This function uses the selected entities in TZOUTLINER to
//          create CtrlCtrls under the current outliner control.  These
//          CtrlCtrls get used in the conversion routine for outliner
//          to create a CtrlBOI used for execution.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_OK( zVIEW vSubtask )
{
   zCHAR    szTempString[ 256 ];
   zVIEW    vTopCtrl;
   zVIEW    vControl;
   zVIEW    vOL;
   zVIEW    vWork;
   zLONG    lSubtype;
   zULONG   ulLth;
   zSHORT   nRC;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   ResetView( vOL );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   lSubtype &= 0xFFFF0000;  // knock out the low order bits
   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "DualMode" );
   if ( szTempString[ 0 ] )
      lSubtype |= OLS_DUAL;

   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "MultiSelect" );
   if ( szTempString[ 0 ] )
      lSubtype |= OLS_MULTISEL;

   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "AllChildren" );
   if ( szTempString[ 0 ] )
      lSubtype |= OLS_OPENALLCHILDREN;

   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "DisplayText" );
   if ( szTempString[ 0 ] == 0 )
      lSubtype |= OLS_NOSHOW_TOPNODE;

   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "RootIndicator" );
   if ( szTempString[ 0 ] == 0 )
      lSubtype |= OLS_NOSHOW_ENTITY;

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );
   SetAttributeFromInteger( vControl, "Control", "Type",
                           (lSubtype & OLS_NOSHOW_TOPNODE) ? 1 : 0 );
   GetStringFromAttribute( szTempString, sizeof( szTempString ), vWork, "OutlinerGrp", "Operation" );
   if ( szTempString[ 0 ] != 0 )
   {
      ulLth = zstrlen( szTempString ) + 1;
      GetStringFromAttribute( szTempString + ulLth, sizeof( szTempString ) - ulLth, vWork, "OutlinerGrp", "DLL_Name" );
      ulLth += zstrlen( szTempString + ulLth ) + 1;
      SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szTempString, ulLth );
   }

   AcceptSubobject( vControl, "Control" );

   // Create CtrlCtrl's under the outliner control that correspond to
   // the entities which have been selected.
   nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", 0 );

   // First delete any existing CtrlCtrl's.
   while ( nRC >= zCURSOR_SET )
      nRC = DeleteEntity( vControl, "CtrlCtrl", zREPOS_NEXT );

   // Recursive function to create the CtrlCtrl's.
   CreateViewFromViewForTask( &vTopCtrl, vControl, 0 );
   SetViewToSubobject( vControl, "CtrlCtrl" );
   fnBuildOutlinerCtrls( vSubtask, vOL, vTopCtrl, vControl );

   DropView( vTopCtrl );
   ResetViewFromSubobject( vControl );
   ResetView( vControl );
   DropObjectInstance( vOL );

   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                      zWAB_ProcessImmediateReturn, 0, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// local functions
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnGetIdxFromString( zPCHAR pchString,
                    zSHORT nLth )
{
   zSHORT nIdx = 0;
   zSHORT nPower = 1;

   while ( nLth-- > 0 )
   {
      nIdx += nPower *(pchString[ nLth ] - '0');
      nPower *= 10;
   }

   return( nIdx );
}

zVOID
CreateCtrlMapInOrder( zVIEW vSubtask, zVIEW vControl, zSHORT nIdx )
{
   zPCHAR pch;
   zSHORT nPrevIdx;
   zSHORT nRC;

   nRC = SetCursorFirstEntity( vControl, "CtrlMap", 0 );
   while ( nRC >= 0 )
   {
      GetAddrForAttribute( &pch, vControl, "CtrlMap", "Tag" );
      nPrevIdx = fnGetIdxFromString( pch + 4, 3 );
      if ( nPrevIdx > nIdx )
         break;

      nRC = SetCursorNextEntity( vControl, "CtrlMap", 0 );
   }

   CreateMetaEntity( vSubtask, vControl, "CtrlMap",
                     (zSHORT) ((nRC >= 0) ? zPOS_BEFORE : zPOS_AFTER) );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnBuildOutlinerCtrls
//
// PURPOSE: A recursive function that creates CtrlCtrls under the
//          outliner control.  These CtrlCtrls are used in ConvertOutliner
//          to create the CtrlBOI for execution.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnBuildOutlinerCtrls( zVIEW vSubtask,
                      zVIEW vOL,
                      zVIEW vTopCtrl,
                      zVIEW vControl )
{
   zCHAR  szTempString[ 256 ];
   zCHAR  szWorkString[ 33 ];
   zVIEW  vTempOL;
   zVIEW  vTempCtrl;
   zLONG  lSubtype;
   zSHORT bSelected;
   zSHORT nIdx;
   zSHORT nPrevIdx;
   zSHORT nRC;

   // Exit this function if no LOD_EntityParent exists.
   if ( (nRC = SetCursorFirstEntity( vOL, "LOD_EntityParent", 0 ) ) == zCURSOR_SET )
   {
      nPrevIdx = 0;
      CreateViewFromViewForTask( &vTempOL, vOL, 0 );
      CreateViewFromViewForTask( &vTempCtrl, vControl, 0 );
      while ( nRC == zCURSOR_SET )
      {
         /* if this entity has been selected(and this entity has attributes),
            create a control and set the control's mapping to vOL's entity
            name and attribute name */
         bSelected = FALSE;
         if ( SetCursorFirstEntity( vTempOL, "LOD_AttributeRec", 0 ) >= 0 &&
             (bSelected = GetSelectStateOfEntityForSet( vOL, "LOD_EntityParent", 1 )) == TRUE )
         {
            CreateMetaEntity( vSubtask, vControl, "Control", zPOS_AFTER );

            lSubtype = 0;
            // if this entity is recursive, then set the control's Subtype and Text attributes.
            if ( CompareAttributeToString( vOL, "LOD_EntityParent",
                                           "Create", "R" ) == 0 )
            {
               lSubtype |= OL_RECURSIVE_ENTITY;
               SetAttributeFromAttribute( vControl, "Control", "Text",
                                          vOL, "LOD_EntityParent", "Desc" );
            }

            if ( CompareAttributeToString( vOL, "LOD_EntityParent",
                                           "Delete", "O" ) == 0 )
            {
               lSubtype |= OL_OPEN_CHILDREN;
            }

            SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

            // we need to keep the order and the TOP_VIEW information
            // in the CtrlMap since there are multiple mappings for
            // each control.  We will use the tag as a tab-separated
            // structure as follows:
            //
            // byte(s)    use
            // -------   ---------------------------------------------------
            //   0       Tag attribute(only one per entity - Y/N)
            //   2       Selected attribute(Y/N)
            //  4-6      Sequence number(001-999)
            //  8-39     Context
            //

            // Loop through the LOD_AttributeRecs and see if any are
            // selected.  Use these to create CtrlMap information.  If
            // none are selected, default mapping to the first attribute.
            nRC = SetCursorFirstEntity( vTempOL, "ER_AttributeRec", "LOD_EntityParent" );
            while ( nRC >= 0 )
            {
               GetStringFromAttribute( szTempString, sizeof( szTempString ), vTempOL, "ER_AttributeRec", "Desc" );
               if ( GetSelectStateOfEntityForSet( vTempOL, "ER_AttributeRec", 1 ) )
               {
                  szTempString[ 2 ] = 'Y';
               }
               else
               {
                  szTempString[ 2 ] = 'N';
               }

#ifdef DEBUG_TAG
               TraceLineS( "BuildOutlinerCtrls Desc = ", szTempString );
#endif
               if ( szTempString[ 0 ] == 'Y' || szTempString[ 2 ] == 'Y' )
               {
                  zSHORT k;

                  // Tab fill the context.
                  for( k = 0; k <= 32; k++ )
                  {
                     if ( szTempString[ k ] == 0 )
                     {
                        while ( k <= 32 )
                           szTempString[ k++ ] = '\t';
                     }
                  }

                  szTempString[ 32 ] = 0;

                  nIdx = fnGetIdxFromString( szTempString + 4, 3 );
                  CreateCtrlMapInOrder( vSubtask, vControl, nIdx );
                  IncludeSubobjectFromSubobject( vControl,
                                                 "CtrlMapLOD_Attribute",
                                                 vTempOL, "LOD_AttributeRec",
                                                 zPOS_AFTER );
                  IncludeSubobjectFromSubobject( vControl,
                                                 "CtrlMapView",
                                                 vTopCtrl, "CtrlMapView",
                                                 zPOS_AFTER );

                  SetAttributeFromString( vControl, "CtrlMap", "Tag",
                                          szTempString );
#if 0
                  GetAddrForAttribute( &pch, vTempOL, "ER_AttributeRec", "Desc" );
                  TraceLineS( "=================> Setting Control's CtrlMap Tag = ", pch );
                  GetAddrForAttribute( &pch, vTempOL,
                                       "ER_AttributeRec", "Name" );
                  TraceLineS( "Including(TempLOD) Selected ER_AttributeRec Name = ", pch );
#endif
               }

               nRC = SetCursorNextEntity( vTempOL, "ER_AttributeRec", "LOD_EntityParent" );
            }

            // Loop backwards through the control to see what the scoping entity is ... set that in the CtrlBOI.
            SetViewFromView( vTempCtrl, vControl );
            ResetViewFromSubobject( vTempCtrl );

            // The CtrlCtrl's under the outliner do not have a ControlDef
            // associated with them (they do not need one) so if a
            // ControlDef exists, then we know we are on the root outliner
            // control so the scoping entity is vOL's root entity, otherwise
            // the scoping entity is the entity mapped to the parent
            // CtrlCtrl of the current CtrlCtrl.
            if ( CheckExistenceOfEntity( vTempCtrl, "ControlDef" ) == 0 )
            {
               ResetView( vTempOL );
               GetStringFromAttribute( szTempString, sizeof( szTempString ), vTempOL, "LOD_EntityParent", "Name" );
               GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vOL, "LOD_EntityParent", "Name" );
               // if szTempString = LOD_EntityParent.Name then this entity is the root entity and
               // we do not want a scoping entity
               if ( zstrcmp( szTempString, szWorkString ) == 0 )
                    szTempString[ 0 ] = 0;

               if ( zstrcmp( szTempString, "LOD" ) == 0 )
                    szTempString[ 0 ] = 0;
            }
            else
            {
               GetStringFromAttribute( szTempString, sizeof( szTempString ), vTempCtrl, "CtrlMapRelatedEntity", "Name" );
            }

            SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szTempString, (zUSHORT) zstrlen( szTempString ) );
         }

         if ( bSelected )
            SetViewToSubobject( vControl, "CtrlCtrl" );

         SetViewToSubobject( vOL, "LOD_EntityChild" );
         fnBuildOutlinerCtrls( vSubtask, vOL, vTopCtrl, vControl );
         if ( bSelected )
            ResetViewFromSubobject( vControl );

         ResetViewFromSubobject( vOL );
         nRC = SetCursorNextEntity( vOL, "LOD_EntityParent", 0 );
      }

      DropView( vTempOL );
      DropView( vTempCtrl );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_OUTLINER_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_OUTLINER_Cancel( zVIEW vSubtask )
{
   zVIEW    vControl;
   zVIEW    vOL;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOL, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vOL );

   CancelSubobject( vControl, "Control" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ColorNew
//
// PURPOSE:  Create a new CtrlClrOverride entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ColorNew( zVIEW vSubtask )
{
   zVIEW    vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vControl, "CtrlClrOverride", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    FontNew
//
// PURPOSE:  Create a new Font entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
FontNew( zVIEW vSubtask )
{
   zVIEW    vControl;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vControl, "Font", zPOS_AFTER );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
DisplayColorPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "ColorPopup", -1, 0, FALSE, FALSE );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
DisplayFontPopup( zVIEW vSubtask )
{
   CreateTrackingPopupMenu( vSubtask, "FontPopup", -1, 0, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BmpBtnBrowseFileName( zVIEW  vSubtask,
                          zPCHAR pchFileName,
                          zSHORT nFileNameLth )
{
   pchFileName[ 0 ] = 0;
   return( OperatorPromptForFile( vSubtask, pchFileName, nFileNameLth,
                                  "Bitmap Files (*.BMP)|*.bmp|", "BMP",
                                  zOFN_HIDEREADONLY | zOFN_NOCHANGEDIR | zOFN_FILEMUSTEXIST ) );
//                                  "BMP", TRUE ) );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BMPBTN_BrowseUP( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

   if ( UPD_BmpBtnBrowseFileName( vSubtask, szFileName, sizeof( szFileName ) ) == 1 )
   {
      SetCtrlText( vSubtask, "edUp", szFileName );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BMPBTN_BrowseDN( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

   if ( UPD_BmpBtnBrowseFileName( vSubtask, szFileName, sizeof( szFileName ) ) == 1 )
   {
      SetCtrlText( vSubtask, "edDown", szFileName );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BMPBTN_BrowseFO( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

   if ( UPD_BmpBtnBrowseFileName( vSubtask, szFileName, sizeof( szFileName ) ) == 1 )
   {
      SetCtrlText( vSubtask, "edFocus", szFileName );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
UPD_BMPBTN_BrowseDA( zVIEW vSubtask )
{
   zCHAR   szFileName[ 256 ];

   if ( UPD_BmpBtnBrowseFileName( vSubtask, szFileName, sizeof( szFileName ) ) == 1 )
   {
      SetCtrlText( vSubtask, "edDisabled", szFileName );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ACT_USE_BuildActionUseList( zVIEW vSubtask )
{
   // Build a list of Window Events, Control Events and Menu Options that use an Action.

   zSHORT  nRC;
   zVIEW   vDialog;
   zCHAR   szActionTag[ 33 ];

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // If Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( !ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_DIALOG_META ) )
      SetViewUpdate( vDialog );

   // First delete any current entries.
   nRC = SetCursorFirstEntity( vDialog, "EventList", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      DeleteEntity( vDialog, "EventList", zREPOS_NONE );
      nRC = SetCursorNextEntity( vDialog, "EventList", 0 );
   }

   // Loop through all ActWndEvent entities, create EventList entries.
   nRC = SetCursorFirstEntity( vDialog, "ActWndEvent", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vDialog, "EventList", zPOS_AFTER );
      SetAttributeFromString( vDialog, "EventList", "EventOptionType",
                              "Window Event" );
      SetAttributeFromAttribute( vDialog, "EventList", "Name",
                                 vDialog, "Window",    "Tag" );
      nRC = SetCursorNextEntity( vDialog, "ActWndEvent", 0 );
   }

   // Loop through all ActEvent entities, create EventList entries.
   nRC = SetCursorFirstEntity( vDialog, "ActEvent", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vDialog, "EventList", zPOS_AFTER );
      SetAttributeFromString( vDialog, "EventList", "EventOptionType", "Control Event" );
      SetAttributeFromAttribute( vDialog, "EventList", "Name", vDialog, "ActCtrl",  "Tag" );
      nRC = SetCursorNextEntity( vDialog, "ActEvent", 0 );
   }

   // Loop through all ActOpt entities, create EventList entries.
   nRC = SetCursorFirstEntity( vDialog, "ActOpt", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vDialog, "EventList", zPOS_AFTER );
      SetAttributeFromString( vDialog, "EventList", "EventOptionType", "Menu Option" );
      SetAttributeFromAttribute( vDialog, "EventList", "Name", vDialog, "ActOpt",   "Tag" );
      nRC = SetCursorNextEntity( vDialog, "ActOpt", 0 );
   }

  if ( !ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_DIALOG_META ) )
      SetViewReadOnly( vDialog );

   //BL, 1999.11.19 Set CaptionTitle
   GetStringFromAttribute( szActionTag, sizeof( szActionTag ), vDialog, "Action", "Tag" );
   SetWindowCaptionTitle( vSubtask, 0, szActionTag );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_RefreshActionList( zVIEW vSubtask )
{
   LIST_ACT_PostBuildInit( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPNCTAD_DeleteAllActions( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zCHAR    szMsg[ 254 ];
   zSHORT   nRC;
   zCHAR    szTag[ 33 ];

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTag, sizeof( szTag ), vDialogW, "Window", "Tag" );
   strcpy( szMsg, "OK to delete all Actions from Window '" );
   strcat( szMsg, szTag );
   strcat( szMsg, "'?" );

   // Ask the operator if it is ok to delete.
   if ( MessagePrompt( vSubtask, "CM00111", "Dialog Maintenance",
                       szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_YES,  0 ) != zRESPONSE_NO )
   {
      for ( nRC = SetCursorFirstEntity( vDialogW, "Action", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vDialogW, "Action", "" ) )
      {
         fnDeleteAction( vSubtask, vDialogW );
      }
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LoadOperations( zVIEW vSubtask )
{
   // Operation List is a modeless Window, if an Operation is updated or
   // newly created, then refresh the Operation combobox.
   MapCtrl( vSubtask, "OtherOpers" );

   UPD_ACT_SelectDialogOpers( vSubtask );

   RefreshCtrl( vSubtask, "OtherOpers" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CommonSelectedDetailPostbuild( zVIEW vSubtask )
{
   zULONG ulCommonFlags;
   zVIEW  vControl;
   zLONG  lSubtype;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   fnPainterCall( zMSG_GET_COMMON_DETAIL_FLAGS, vSubtask, 0, (zPCHAR) &ulCommonFlags, 0 );

   if ( lSubtype & zCONTROL_SIZEABLEBORDER )
      SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_INVISIBLE )
      SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_DISABLED )
      SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_NOTABSTOP )
      SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED, 1 );

   if ( (lSubtype & zCONTROL_BORDEROFF) == 0 )
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROL_RIGHTJUSTIFY )
      SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED, 1 );
   else
   if ( lSubtype & zCONTROL_CENTERJUSTIFY )
      SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED, 1 );
   else
      SetCtrlState( vSubtask, "LeftJustify", zCONTROL_STATUS_CHECKED, 1 );

   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "ExtendedStyle" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( lSubtype & zCONTROLX_DISABLE_READONLY )
         SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED, 1 );
   }
   else
      SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, 0 );

   if ( lSubtype & zCONTROLX_PREFILL )
      SetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_SIZE )
      SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED, 1 );

   if ( lSubtype & zCONTROLX_ABSOLUTE_PIXEL_POS )
      SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED, 1 );

   if ( (ulCommonFlags & zCTRL_COMMON_INVISIBLE) == 0 )
      SetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_DISABLED) == 0 )
      SetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_DISABLED_READONLY) == 0 )
      SetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_ABSOLUTE_PIXEL_SIZE) == 0 )
      SetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_ABSOLUTE_PIXEL_POS) == 0 )
      SetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_BORDER) == 0 )
      SetCtrlState( vSubtask, "Border", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_SIZE_BORDER) == 0 )
      SetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_LEFT_JUSTIFY) == 0 )
      SetCtrlState( vSubtask, "LeftJustify", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_CENTER_JUSTIFY) == 0 )
      SetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_RIGHT_JUSTIFY) == 0 )
      SetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_NO_TAB) == 0 )
      SetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_ENABLED, 0 );

   if ( (ulCommonFlags & zCTRL_COMMON_PREFILL) == 0 )
      SetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_ENABLED, 0 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CommonSelectedDetailOK( zVIEW vSubtask )
{
   zULONG ulCommonFlags;
   zVIEW  vControl;
   zLONG  lSubtype;
   zLONG  lSubtypeX;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
   fnPainterCall( zMSG_GET_COMMON_DETAIL_FLAGS, vSubtask, 0, (zPCHAR) &ulCommonFlags, 0 );

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lSubtype, vControl, "Control", "Subtype" );
// TraceLineI( "BEFORE Common OK ", lSubtype );

   if ( ulCommonFlags & zCTRL_COMMON_SIZE_BORDER )
   {
      if ( GetCtrlState( vSubtask, "SizeableBorder", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_SIZEABLEBORDER;
      else
         lSubtype &= ~zCONTROL_SIZEABLEBORDER;
   }

   if ( ulCommonFlags & zCTRL_COMMON_INVISIBLE )
   {
      if ( GetCtrlState( vSubtask, "Invisible", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_INVISIBLE;
      else
         lSubtype &= ~zCONTROL_INVISIBLE;
   }

   if ( ulCommonFlags & zCTRL_COMMON_DISABLED )
   {
      if ( GetCtrlState( vSubtask, "Disabled", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_DISABLED;
      else
         lSubtype &= ~zCONTROL_DISABLED;
   }

   if ( ulCommonFlags & zCTRL_COMMON_NO_TAB )
   {
      if ( GetCtrlState( vSubtask, "NoTab", zCONTROL_STATUS_CHECKED ) )
         lSubtype |= zCONTROL_NOTABSTOP;
      else
         lSubtype &= ~zCONTROL_NOTABSTOP;
   }

   if ( ulCommonFlags & zCTRL_COMMON_BORDER )
   {
      if ( GetCtrlState( vSubtask, "Border", zCONTROL_STATUS_CHECKED ) == 0 )
         lSubtype |= zCONTROL_BORDEROFF;
      else
         lSubtype &= ~zCONTROL_BORDEROFF;
   }

   if ( ulCommonFlags & zCTRL_COMMON_LEFT_JUSTIFY )
   {
      if ( GetCtrlState( vSubtask, "LeftJustify", zCONTROL_STATUS_CHECKED ) )
      {
         lSubtype &= ~zCONTROL_CENTERJUSTIFY;
         lSubtype &= ~zCONTROL_RIGHTJUSTIFY;
      }
   }

   if ( ulCommonFlags & zCTRL_COMMON_CENTER_JUSTIFY )
   {
      if ( GetCtrlState( vSubtask, "CenterJustify", zCONTROL_STATUS_CHECKED ) )
      {
         lSubtype |= zCONTROL_CENTERJUSTIFY;
         lSubtype &= ~zCONTROL_RIGHTJUSTIFY;
      }
      else
      {
         lSubtype &= ~zCONTROL_CENTERJUSTIFY;
      }
   }

   if ( ulCommonFlags & zCTRL_COMMON_RIGHT_JUSTIFY )
   {
      if ( GetCtrlState( vSubtask, "RightJustify", zCONTROL_STATUS_CHECKED ) )
      {
         lSubtype |= zCONTROL_RIGHTJUSTIFY;
         lSubtype &= ~zCONTROL_CENTERJUSTIFY;
      }
      else
      {
         lSubtype &= ~zCONTROL_RIGHTJUSTIFY;
      }
   }

   SetAttributeFromInteger( vControl, "Control", "Subtype", lSubtype );

   GetIntegerFromAttribute( &lSubtypeX, vControl, "Control", "ExtendedStyle" );
   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
   {
      if ( ulCommonFlags & zCTRL_COMMON_DISABLED_READONLY )
      {
         if ( GetCtrlState( vSubtask, "ReadOnly", zCONTROL_STATUS_CHECKED ) )
            lSubtypeX |= zCONTROLX_DISABLE_READONLY;
         else
            lSubtypeX &= ~zCONTROLX_DISABLE_READONLY;
      }
   }

   if ( ulCommonFlags & zCTRL_COMMON_PREFILL )
   {
      if ( GetCtrlState( vSubtask, "Prefill", zCONTROL_STATUS_CHECKED ) )
         lSubtypeX |= zCONTROLX_PREFILL;
      else
         lSubtypeX &= ~zCONTROLX_PREFILL;
   }

   if ( ulCommonFlags & zCTRL_COMMON_ABSOLUTE_PIXEL_SIZE )
   {
      if ( GetCtrlState( vSubtask, "AbsolutePixelSize", zCONTROL_STATUS_CHECKED ) )
         lSubtypeX |= zCONTROLX_ABSOLUTE_PIXEL_SIZE;
      else
         lSubtypeX &= ~zCONTROLX_ABSOLUTE_PIXEL_SIZE;
   }

   if ( ulCommonFlags & zCTRL_COMMON_ABSOLUTE_PIXEL_POS )
   {
      if ( GetCtrlState( vSubtask, "AbsolutePixelPos", zCONTROL_STATUS_CHECKED ) )
         lSubtypeX |= zCONTROLX_ABSOLUTE_PIXEL_POS;
      else
         lSubtypeX &= ~zCONTROLX_ABSOLUTE_PIXEL_POS;
   }

   if ( fnPainterCall( zMSG_ISREPORT, vSubtask, 0, 0, 0 ) == 0 )
      SetAttributeFromInteger( vControl, "Control", "ExtendedStyle", lSubtypeX );

   fnPainterCall( zMSG_UPDATE_COMMON_DETAIL, vSubtask, (zVIEW) lSubtypeX, (zPCHAR) lSubtype, 0 );
   fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, (zPCHAR) 1, 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
CommonSelectedDetailCancel( zVIEW vSubtask )
{
   fnPainterCall( zMSG_UPDATE_COMMON_DETAIL, vSubtask, (zVIEW) 0xffffffff, (zPCHAR) 0xffffffff, 0 );
   fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, (zPCHAR) 1, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_ACT_SetEditButtonStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_ACT_SetEditButtonStatus( zVIEW vSubtask )
{
   zVIEW  vTZWINDOW;
   zCHAR  szOperationName[ 33 ];

   GetViewByName( &vTZWINDOW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vTZWINDOW, "ActOper" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "Edit", zCONTROL_STATUS_ENABLED, TRUE );
   else
   {
      RefreshCtrl( vSubtask, "OtherOpers" );
      GetCtrlText( vSubtask, "OtherOpers", szOperationName, 33 );

      if ( zstrcmp( szOperationName, "" ) == 0 )
         SetCtrlState( vSubtask, "Edit", zCONTROL_STATUS_ENABLED, FALSE );
      else
         SetCtrlState( vSubtask, "Edit", zCONTROL_STATUS_ENABLED, TRUE );
   }

   RefreshCtrl( vSubtask, "Edit" );
   return( 0 );

} // UPD_ACT_SetEditButtonStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LIST_ACT_LoadActionPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
LIST_ACT_LoadActionPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vDialog;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   if ( GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vDialog, zSOURCE_DIALOG_META );

      if ( CheckExistenceOfEntity( vDialog, "Action" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "Add", bNew );
      EnableAction( vSubtask, "Delete", bDelete );
      EnableAction( vSubtask, "DeleteAll", bDelete );

      CreateTrackingPopupMenu( vSubtask, "ActionPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );

} // LIST_ACT_LoadActionPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SEL_ATTR_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SEL_ATTR_Postbuild( zVIEW vSubtask )
{
   zVIEW  vDialog;

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vDialog ) == 0 )
   {
      SetCtrlState( vSubtask, "ViewName", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "EntityList", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "AttributeList", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );

} // SEL_ATTR_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTAD_DisableButtons
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTAD_DisableButtons( zVIEW  vSubtask,
                           zPCHAR szButtonNameForAction,
                           zPCHAR szActionNameForEdit,
                           zPCHAR szActionNameForAction )
{
   zVIEW vEvents;
   zVIEW vAction;
   zBOOL bEnableAction = FALSE;
   zBOOL bEnableEdit   = FALSE;

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CheckExistenceOfEntity( vEvents, "Event" ) >= zCURSOR_SET &&
           CheckExistenceOfEntity( vEvents, "EventAct" ) >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vEvents, "EventAct", "Tag", "" ) != 0 )
         {
            bEnableAction = TRUE;

            if ( GetViewByName( &vAction, "TZACTION", vSubtask, zLEVEL_TASK ) > 0 &&
                 CheckExistenceOfEntity( vAction, "ActOper" ) >= zCURSOR_SET)
            {
               bEnableEdit = TRUE;
            }
         }
      }

      SetCtrlState( vSubtask, szButtonNameForAction, zCONTROL_STATUS_ENABLED,
                    bEnableAction );
      EnableAction( vSubtask, szActionNameForEdit, bEnableEdit );
      EnableAction( vSubtask, szActionNameForAction, bEnableAction );
   }

   return( 0 );

} // zwTZPNCTAD_DisableButtons

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPNCTAD_LoadEventPopups
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPNCTAD_LoadEventPopups( zVIEW  vSubtask,
                            zPCHAR szActionNameForUpdate,
                            zPCHAR szActionNameForNew,
                            zPCHAR szActionNameForSelect,
                            zPCHAR szActionNameForRemove,
                            zPCHAR szActionNameForEdit,
                            zPCHAR szPopupName )
{
   zVIEW  vEvents;
   zVIEW  vDialog;
   zVIEW  vDialogCopy;
   zVIEW  vTZWINDOWL;
   zBOOL  bNew          = FALSE;
   zBOOL  bEdit         = FALSE;
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

            // Check Action Operation
            CreateViewFromViewForTask( &vDialogCopy, vDialog, 0 );
            SetCursorFirstEntityByAttr( vDialogCopy, "Action", "Tag",
                                        vEvents, "EventAct", "Tag", "" );
            if ( CheckExistenceOfEntity( vDialogCopy, "ActOper" ) >= zCURSOR_SET )
               bEdit = TRUE;

            DropView( vDialogCopy );
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

} // zwTZPNCTAD_LoadEventPopups

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_COMBO_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_COMBO_PostBuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "ControlSubtype",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );

} // UPD_COMBO_PostBuild


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UPD_COMBO_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
UPD_COMBO_Cancel( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vWork;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   if ( vWork )
      DropView( vWork );

   // Cancel the Control.
   CancelSubobject( vDialog, "Control" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxAdd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxAdd( zVIEW vSubtask )
{
   zVIEW  vControl;
   zVIEW  vPE;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByInteger( vPE, "ControlDef", "Key",
                                          zCOMBOITEM_CONTROL,
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

} // ComboBoxAdd

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxUpdate
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxUpdate( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vControl, "Control" );
   return( 0 );

} // ComboBoxUpdate

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxDelete
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxDelete( zVIEW vSubtask )
{
   zVIEW  vControl;

   GetViewByName( &vControl, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
   DeleteEntity( vControl, "Control", zREPOS_PREV );
   return( 0 );

} // ComboBoxDelete

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxItemPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxItemPostBuild( zVIEW vSubtask )
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
                           "1;Button;2;DropButton;3;ComboBox", 1 );

   return( 0 );

} // ComboBoxItemPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxItemUPD_OK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxItemUPD_OK( zVIEW vSubtask )
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

} // ComboBoxItemUPD_OK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: ComboBoxItemUPD_Cancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ComboBoxItemUPD_Cancel( zVIEW vSubtask )
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

} // ComboBoxItemUPD_Cancel

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ComboBoxItemSelectAction
//
// PURPOSE:  This function creates the Event entity for the ComboBox Item.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ComboBoxItemSelectAction( zVIEW vSubtask )
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
// ENTRY:    ComboBoxItemAddAction
//
// PURPOSE:  This function creates an Action entity and includes it to
//           the selected control.  It is used from Action_Popup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ComboBoxItemAddAction( zVIEW vSubtask )
{
   zVIEW  vDialog;
   zVIEW  vControl;
   zVIEW  vWork;
// zCHAR  szTag[ 33 ];

   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
// GetViewByName( &vWork, "TZCTLMAP3", vSubtask, zLEVEL_TASK );
// GetStringFromAttribute( szTag, sizeof( szTag ), vControl, "Control", "Tag" );
// TraceLineS( "ComboBoxItemAddAction TZCONTROL: ", szTag );
// GetStringFromAttribute( szTag, sizeof( szTag ), vWork, "Control", "Tag" );
// TraceLineS( "ComboBoxItemAddAction TZCTLMAP3: ", szTag );

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
// ENTRY:    ComboBoxItemDeleteAction
//
// PURPOSE:  This function creates the Event entity for the ComboBox Item.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ComboBoxItemDeleteAction( zVIEW vSubtask )
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

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_ERD_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_ERD_PostBuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      SetCtrlState( vSubtask, "ViewName", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );

} // UPD_ERD_PostBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_RadioPostbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_RadioPostbuild( zVIEW vSubtask )
{
   zVIEW    vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetCtrlState( vSubtask, "edValue", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Check1", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );

} // UPD_RadioPostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_SS_SetActionView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_SS_SetActionView( zVIEW vSubtask )
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
   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vDialogA, vDialogW, 0 );
      SetCursorFirstEntityByAttr( vDialogA, "Action", "Tag",
                                  vDialogC, "EventAct", "Tag", "" );
      SetNameForView( vDialogA, "TZACTION", vSubtask, zLEVEL_TASK );
   }
   else
   // If not, set TZACTION to empty instance for auto graying.
   {
      fnCreateEmptyActionView( vSubtask, vDialogW );
   }

   return( 0 );

} // UPD_SS_SetActionView

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: UPD_SSCOMBO_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_SSCOMBO_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      SetCtrlState( vSubtask, "BoxStyle", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );

} // UPD_SSCOMBO_Postbuild

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: UPD_SSEDIT_Init
//
//////////////////////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_SSEDIT_Init( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Convert any 2nd CtrlMap entry to "Variable Header" mapping if it's used.
   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );
   CreateViewFromViewForTask( &vDialogA, vDialogC, 0 );
   SetNameForView( vDialogA, "TZCONTROLMAP", vSubtask, zLEVEL_TASK );
   nRC = SetCursorNextEntity( vDialogA, "CtrlMap", "" );
   if ( nRC >= zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vDialogA, "CtrlMapLOD_Attribute" ) < 0 )
         DeleteEntity( vDialogA, "CtrlMap", zREPOS_NONE );
      else
      {
         if ( CompareAttributeToString( vDialogA, "CtrlMap", "Tag", "" ) == 0 )
            SetAttributeFromString( vDialogA, "CtrlMap", "Tag", "Variable Header" );
     }
   }
   //SetNameForView( vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   DropView( vDialogA );

   // Go to build special CtrlMap list by Tag value.
   GetViewByName( &vDialogA, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildCtrlMapByTagList( vDialogC );

   // Go to reuse init code.
   UPD_LISTCTRL_Init( vSubtask );

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   // Go to build list of Web Potential Control Properties.
   GetViewByName( &vDialogA, "TZWINDOW", vSubtask, zLEVEL_TASK );
   oTZWDLGSO_BuildWebCtrlPropOpts( vDialogA, vDialogA, "Dialog", "wWebGridSubControlProperties" );

   // Build Combobox list of MapType values for a Grid Edit sub Control.
   oTZWDLGSO_BuildMapTypeList( vDialogA, vDialogA, "Dialog", "wMapTypeGridColumnControl" );

   return( 0 );
} // UPD_SSEDIT_Init

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: UPD_SSEDIT_SelectWebColMapping
//
//////////////////////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_SSEDIT_SelectWebColMapping( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogA;
   zVIEW    vWork;

   GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogA, "CtrlMapView" ) == zCURSOR_SET )
      SetAttributeFromAttribute( vWork, "TZPNCTWO", "TempViewName",
                                 vDialogA, "CtrlMapView", "Name" );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetCursorNextEntity( vDialogC, "CtrlMap", "" );

   return( 0 );
} // UPD_SSEDIT_SelectWebColMapping

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: UPD_SSEDIT_ReturnFromSubwindow
//
//////////////////////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_SSEDIT_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vDialogC;
// zVIEW    vDialogA;
   zVIEW    vDomain;
   zVIEW    vCM_List;
   zLONG    lZKey;

// GetViewByName( &vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntity( vDialogC, "CtrlMap", "" );

   // Execute the ReturnFromSubwindow code from LISTBOX to set up
   // the object for the Event list box.
   UPD_LISTBOX_ReturnFromSubwindow( vSubtask );

   // Reset cursors for CtrlMap subobject view vDialogA
// SetCursorPrevEntity( vDialogA, "CtrlMap", "" );
// SetCursorNextEntity( vDialogA, "CtrlMap", "" );

   // Make sure MapDomain is set up for the edit box mapping.
   if ( GetViewByName( &vDomain, "MapDomain", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain", vSubtask, zLEVEL_TASK );
   }

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogC,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogC, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   zwTZPNCTAD_DisableButtons( vSubtask, "ActionButton",
                              "ControlOperationEdit", "SelectUpdate" );

   return( 0 );
} // UPD_SSEDIT_ReturnFromSubwindow

//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=UPD_SSSUB_Init
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_SSSUB_Init
//
// PURPOSE:    Set up the CtrlMap entity if necessary and prepare
//             for Context mapping.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_SSSUB_Init( zVIEW vSubtask )
{
   zCHAR  szMapFlag[ 34 ];
   zCHAR  szBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
   zULONG ulLth = sizeof( szBlob );
   zVIEW  vControl;
   zVIEW  vDialogA;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zVIEW  vWork;
   zLONG  lFlag;
   zLONG  lZKey;

   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // If WindowStyle != "L" then we are entering from the painter screen
   // and so ListCtrl already exists so create the temporal version.
   // Otherwise, we are creating ListCtrl from the listbox update window,
   // and CreateTemporalMetaEntity was used, so we don't want to create
   // another temporal version.
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "A" ) != 0 )
   {
      CreateTemporalSubobjectVersion( vControl, "Control" );
   }

   // Set up Domain Context mapping.
   CtrlContextMappingInit( vSubtask );

   GetBlobFromAttribute( szBlob, &ulLth, vControl, "Control", "CtrlBOI" );
//              lFlag      - 0 ==> do not reset ctrl width and height
//                           1 ==> set ctrl width to column size
//                           2 ==> set ctrl height to column size
//                           4 ==> set ctrl width to column size (do not set
//                                 smaller than ctrl's design width).
//                           8 ==> set ctrl height to column size (do not set
//                                 smaller than ctrl's design height).

   lFlag = *((zPSHORT) (szBlob + 2));
   SetCtrlTranslationList( vSubtask, "SubFlag",
                           "0;;1;Width;2;Height;3;Width/Height;"
                           "4;Minimum Width;8;Minimum Height;"
                           "6;Minimum Width+Height;9;Width+Minimum Height;"
                           "12;Minimum Width/Minimum Height", 1 );
   zltoa( lFlag, szMapFlag, sizeof( szMapFlag ) );
   TranslateCtrlValue( vSubtask, "SubFlag", 0, TRUE,
                       szMapFlag, sizeof( szMapFlag ) );
   CB_SetData( vSubtask, "SubFlag", szMapFlag,
               ";Width;Height;Width/Height;"
               "Minimum Width;Minimum Height;"
               "Minimum Width+Height;Width+Minimum Height;"
               "Minimum Width/Minimum Height", 0 );

   // Make sure the last three entities are named at TASK level.
   SetCursorFirstEntity( vControl, "CtrlMap", "" );
   CreateViewFromViewForTask( &vDialogA, vControl, 0 );
   SetCursorNextEntity( vDialogA, "CtrlMap", "" );
   SetNameForView( vDialogA, "TZPULL_Attribute", vSubtask, zLEVEL_TASK );

   // Make sure MapDomain2 is set up for the list box mapping.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogA,
                                "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogA, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                     "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   return( 0 );

} // UPD_SSEDIT_Init

//./ ADD NAME=UPD_SSSUB_OK
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_SSSUB_OK
//
// PURPOSE:    This function:
//             1. Sets up mapping for the LISTBOX control itself, if
//                such mapping doesn't exist.
//             2. Saves the Context Name.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_SSSUB_OK( zVIEW vSubtask )
{
   zCHAR  szMapFlag[ 34 ];
   zCHAR  szBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
   zULONG ulLth = sizeof( szBlob );
   zVIEW  vDialogW;
   zVIEW  vControl;
   zVIEW  vDialogP;
   zVIEW  vWork;
   zVIEW  vLOD;
   zVIEW  vCM_List;
   zLONG  lZKey;

   GetViewByName( &vDialogW, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Store the blob.
   zmemset( szBlob, 0, sizeof( szBlob ) );

   GetCtrlText( vSubtask, "SubFlag", szMapFlag, sizeof( szMapFlag ) );
   TranslateCtrlValue( vSubtask, "SubFlag", 0, FALSE,
                       szMapFlag, sizeof( szMapFlag ) );

   *((zPSHORT) (szBlob + 2)) = (zSHORT) zatol( szMapFlag );
   SetAttributeFromBlob( vControl, "Control", "CtrlBOI", szBlob, ulLth );

   GetViewByName( &vWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vDialogP, vControl, 0 );

   // Reset view to parent list control
   ResetViewFromSubobject( vDialogP );

   // If there is no mapping on the LISTBOX control(the parent), use
   // the mapping from the last LISTCTRL.
   if ( CheckExistenceOfEntity( vDialogP, "CtrlMapView" ) != zCURSOR_SET )
   {
      if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lZKey, vControl, "CtrlMapView", "ZKey" );
         SetCursorFirstEntityByInteger( vDialogW, "ViewObjRef",
                                        "ZKey", lZKey, "" );
         IncludeSubobjectFromSubobject( vDialogP, "CtrlMapView",
                                        vDialogW, "ViewObjRef", zPOS_AFTER );
         GetIntegerFromAttribute( &lZKey, vDialogW, "LOD", "ZKey" );
         RetrieveViewForMetaList( vSubtask,
                                  &vCM_List, zREFER_LOD_META );
         SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                        "CPLR_ZKey", lZKey, "" );
         ActivateMetaOI( vSubtask, &vLOD,
                         vCM_List, zREFER_LOD_META, 0 );
         SetNameForView( vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
         GetIntegerFromAttribute( &lZKey, vControl,
                                  "CtrlMapRelatedEntity", "ZKey" );
         SetCursorFirstEntityByInteger( vLOD, "LOD_Entity",
                                        "ZKey", lZKey, "" );
         IncludeSubobjectFromSubobject( vDialogP, "CtrlMapLOD_Entity",
                                        vLOD, "LOD_Entity", zPOS_AFTER );
      }
   }

   // Set up SEL_LOD, if current mapping exists, and any other
   // mapping data required (for the ListBox window).
   fnSetUpSEL_LOD( vDialogP, vDialogW, vSubtask, "CtrlMapView" );

   // Set up TZSCOPEL for listing potential scoping entities for
   // the ScopeList combo box(for the ListBox window).
   fnSetUpScopeListOI( vDialogP, vSubtask, "CtrlMapLOD_Entity" );

   // Accept control subobject
   AcceptSubobject( vControl, "Control" );

   ResetViewFromSubobject( vControl );
   if ( CompareAttributeToString( vWork, "TZPNCTWO",
                                  "WindowFlag", "L" ) != 0 )
   {
      // Tell the Painter to refresh the control.
      RepaintZeidonControl( vSubtask );
   }
   else
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh |
                                         zWAB_ProcessImmediateReturn, 0, 0 );

   return( 0 );
}

//./ ADD NAME=UPD_SSSUB_Cancel
// Source Module=tzpnctad.c
/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      UPD_SSSUB_Cancel
//
// PURPOSE:    To End list sub-control processing
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
UPD_SSSUB_Cancel( zVIEW vSubtask )
{
   zVIEW vDialog;
   zSHORT nRC;

   // Get the view to the list control
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CancelSubobject( vDialog, "Control" );
   nRC = ResetViewFromSubobject( vDialog );

   return( 0 );
}

/*************************************************************************************************
**
**    OPERATION: SET_CurrentWebControlProperties
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SET_CurrentWebControlProperties( zVIEW vSubtask )
{
   zVIEW vDialog;
   zVIEW vDialogC;
   zSHORT nRC;
   zCHAR  szPropertyName[ 33 ];

   // Include each selected WebControlPropertyOption entity into WebControlProperty.
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialog, "TZWINDOW", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstSelectedEntity( vDialog, "WebControlPropertyOption", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDialog, "WebControlPropertyOption", "" ) )
   {
      GetStringFromAttribute( szPropertyName, sizeof( szPropertyName ), vDialog, "WebControlPropertyOption", "Name" );
      nRC = SetCursorFirstEntityByString( vDialogC, "WebControlProperty", "Name", szPropertyName, 0 );
      if ( nRC < zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vDialogC, "WebControlProperty", zPOS_AFTER );
         SetMatchingAttributesByName( vDialogC, "WebControlProperty", vDialog, "WebControlPropertyOption", zSET_NULL );
      }
   }

   return( 0 );
} // SET_CurrentWebControlProperties

/*************************************************************************************************
**
**    OPERATION: REMOVE_CurrentWebCtrlProperties
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
REMOVE_CurrentWebCtrlProperties( zVIEW vSubtask )
{
   zVIEW vDialog;
   zSHORT nRC;

   // Remove each selected WebControlProperty entity.
   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   for ( nRC = SetCursorFirstSelectedEntity( vDialog, "WebControlProperty", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vDialog, "WebControlProperty", "" ) )
   {
      DeleteEntity( vDialog, "WebControlProperty", zREPOS_NONE );
   }

   return( 0 );
} // REMOVE_CurrentWebCtrlProperties

/*************************************************************************************************
**
**    OPERATION: UPD_GeneralVariableCtrlMap
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_GeneralVariableCtrlMap( zVIEW vSubtask )
{
   zVIEW    vDialogCOrig;
   zVIEW    vDialogCNew;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   // Go to select General Variable CtrlMap mapping.
   // We will reuse the regular CtrlMap mapping operations by renaming view TZCONTROL to TZCONTROL_SAVE
   // and naming a new view positioned on the General Variable CtrlMap to TZCONTROL.
   GetViewByName( &vDialogCOrig, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateViewFromView( &vDialogCNew, vDialogCOrig );
   SetCursorFirstEntityByEntityCsr( vDialogCNew, "CtrlMap", vDialogCOrig, "ListCtrlMap", "" );

   // Rename views.
   DropNameForView( vDialogCOrig, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetNameForView( vDialogCOrig, "TZCONTROL_SAVE", vSubtask, zLEVEL_TASK );
   SetNameForView( vDialogCNew, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Set up Context view.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   if ( CheckExistenceOfEntity( vDialogCNew, "CtrlMapER_Attribute" ) == zCURSOR_SET )
   {
      // Activate the Domain for the current CtrlMapER_Domain.
      GetIntegerFromAttribute( &lZKey, vDialogCNew, "CtrlMapER_Domain", "ZKey" );
      SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
      ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
      SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
} // UPD_GeneralVariableCtrlMap

/*************************************************************************************************
**
**    OPERATION: ADD_GeneralVariableCtrlMap
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
ADD_GeneralVariableCtrlMap( zVIEW vSubtask )
{
   zVIEW    vDialogCOrig;
   zVIEW    vDialogCNew;
   zVIEW    vDomain;

   // Go to add a General Variable CtrlMap mapping entry.
   // In order to reuse the regular CtrlMap mapping operations, we will rename view TZCONTROL to
   // TZCONTROL_SAVE and add a new CtrlMap entry to the TZCONTROL view.
   GetViewByName( &vDialogCOrig, "TZCONTROL", vSubtask, zLEVEL_TASK );
   DropNameForView( vDialogCOrig, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetNameForView( vDialogCOrig, "TZCONTROL_SAVE", vSubtask, zLEVEL_TASK );

   CreateViewFromView( &vDialogCNew, vDialogCOrig );
   SetNameForView( vDialogCNew, "TZCONTROL", vSubtask, zLEVEL_TASK );
   SetCursorLastEntity( vDialogCNew, "CtrlMap", "" );
   CreateTemporalMetaEntity( vSubtask, vDialogCNew, "CtrlMap", zPOS_AFTER );

   // Make sure MapDomain2 does not exist since we don't know what Domain to use yet.
   if ( GetViewByName( &vDomain, "MapDomain2", vSubtask, zLEVEL_TASK ) >= 0 )
      DropMetaOI( vSubtask, vDomain );

   return( 0 );
} // ADD_GeneralVariableCtrlMap

/*************************************************************************************************
**
**    OPERATION: DELETE_GeneralVariableCtrlMap
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
DELETE_GeneralVariableCtrlMap( zVIEW vSubtask )
{
   // Delete the selected General Variable CtrlMap mapping.
   // Ask the operator if it is ok to delete.
   if ( MessagePrompt( vSubtask,                 "CM00111",
                       "Variable Mapping Maintenance",
                       "OK to delete selected mapping entry?",
                       0, zBUTTONS_YESNO, zRESPONSE_YES,  0 ) != zRESPONSE_NO )
   {
      zVIEW vDialogC;

      GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
      DeleteEntity(  vDialogC, "CtrlMap", zREPOS_NONE);
   }

   return( 0 );
} // DELETE_GeneralVariableCtrlMap

/*************************************************************************************************
**
**    OPERATION: SEL_ATTR_GeneralSelectMapping
**
*************************************************************************************************/
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_GeneralSelectMapping( zVIEW vSubtask )
{
   zVIEW    vDialogC;

   // Select General Variable CtrlMap.
   // We will reuse the regular SEL_ATTR_SelectMapping operation.
   // Then process any Context that has been selected.

   SEL_ATTR_SelectMapping( vSubtask );

   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapContext" ) >= zCURSOR_SET )
      ExcludeEntity( vDialogC, "CtrlMapContext", zREPOS_NONE );
   if ( CheckExistenceOfEntity( vDialogC, "TempMappingContext" ) >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vDialogC, "CtrlMapContext",
                                     vDialogC, "TempMappingContext", zPOS_AFTER );
      ExcludeEntity( vDialogC, "TempMappingContext", zREPOS_NONE );
   }

   return( 0 );
} // SEL_ATTR_GeneralSelectMapping

/*************************************************************************************************
**
**    OPERATION: PICK_ATTR_GeneralSelectMapping
**
*************************************************************************************************/
zOPER_EXPORT zSHORT OPERATION
PICK_ATTR_GeneralSelectMapping( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vSEL_LOD;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   GetViewByName( &vSEL_LOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );

   // Create Context mapping OI for the current ER_Attribute.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   GetIntegerFromAttribute( &lZKey, vSEL_LOD, "Domain", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapContext" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vDialogC, "CtrlMapContext", zREPOS_NONE );
   }
   RefreshCtrl( vSubtask, "CBContext" );

   return( 0 );
} // PICK_ATTR_GeneralSelectMapping

/*************************************************************************************************
**
**    OPERATION: PICK_ENT_GeneralSelectMapping
**
*************************************************************************************************/
zOPER_EXPORT zSHORT OPERATION
PICK_ENT_GeneralSelectMapping( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zVIEW    vDialogC;
   zVIEW    vCM_List;
   zVIEW    vDomain;
   zLONG    lZKey;

   GetViewByName( &vLOD, "SEL_LOD", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vLOD, "LOD_Attribute" ) == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vLOD, "LOD_Attribute", 1, 1 );

   // Remove any current Context.
   GetViewByName( &vDialogC, "TZCONTROL", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapContext" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vDialogC, "CtrlMapContext", zREPOS_NONE );
   }

   // Create Context mapping OI for the current ER_Attribute.
   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DOMAIN_META );
   GetIntegerFromAttribute( &lZKey, vLOD, "Domain", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vDomain, vCM_List, zREFER_DOMAIN_META, 0 );
   SetNameForView( vDomain, "MapDomain2", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vDialogC, "CtrlMapContext" ) >= zCURSOR_SET )
   {
      ExcludeEntity( vDialogC, "CtrlMapContext", zREPOS_NONE );
   }
   RefreshCtrl( vSubtask, "CBContext" );

   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

/*************************************************************************************************
**
**    OPERATION: SEL_ATTR_GeneralSelectCancel
**
*************************************************************************************************/
zOPER_EXPORT zSHORT OPERATION
SEL_ATTR_GeneralSelectCancel( zVIEW vSubtask )
{
   // Cancel General Variable CtrlMap. Reuse regular CtrlMap operation as in previous operation.
   SEL_ATTR_Cancel( vSubtask );

   return( 0 );
} // SEL_ATTR_GeneralSelectCancel

/*************************************************************************************************
**
**    OPERATION: UPD_LISTBOX_ConvertToGrid
**
*************************************************************************************************/
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
UPD_LISTBOX_ConvertToGrid( zVIEW vSubtask )
{
   zVIEW    vLOD;
   zSHORT nRC;

   GetViewByName( &vLOD, "TZCONTROL", vSubtask, zLEVEL_TASK );
   nRC = oTZWDLGSO_ConvertListBoxToGrid( vLOD );
   if ( nRC < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   // Accept the  temporal subobject and tell the Painter to refresh the control.
   AcceptSubobject( vLOD, "Control" );
   RepaintZeidonControl( vSubtask );

   return( 0 );

} // UPD_LISTBOX_ConvertToGrid


#ifdef __cplusplus
}
#endif
