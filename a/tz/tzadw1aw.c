/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw1aw.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
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
// LAST MESSAGE ID: AD10215
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.07.06  DGC  Z2000
   Removed fnRemoveAmpersand.

2000.06.15  RG   Z2000
   Added missing ';' on lines 249, 288, 379
2000/06/14  DKS  WEB
   Remove '&' from generated control tags.

1999/08/19  DC   10a + 9j
   Modifed Next and Prev processing so that the routines are not generated
   if Autodesign style is not "List".

1999/04/23  DC
   Modified ofnTZWDLGSO_ResetScopeToObject to set type to new list box.

1999/04/23  DC
   Modified ofnTZWDLGSO_AutoDesignListbox for correct positioning of new
   listbox style.

14.04.1997 DonC
   Changed ResetView to a ResetViewFromSubobject loop in AutoDesignCase5Atters
   because ResetView was altering the cursor position on a Case 5 Dialog
   from Helmut.

25.04.1997 DonC
   Modified AutoDesignGroupboxM and the call to AutoDesignGroupboxM to
   execute that routine multiple times if all the M_LOD_Attribute entities
   would not fit on the groupbox.

13.05.1997 DonC
   Modified CreateButtonAct... operations and calls to those routines so
   that Action names for Cancel and Close would be ESC and for OK and Exit
   would be Alt-F4.

13.06.1997 DonC
   If no value for ListQualWndButton was specified in the default object,
   set value to "ListQualNew".

27.08.1997 DonC
   Modified AutoDesignCase1 to not increment group values when Case type
   is "None".

29.09.1997 DonC
   Modified oTZWDLGSO_AutoDesignGroupboxM to skip building window if there
   are no M_LOD_Attribute entities.

03.10.1997 DonC
   Modified AutodesignCase1 to not increment group position for Case 6
   entries. The error was causing a group to be mispositioned if it was to
   contain a Case 6 combo box.

01.12.1997 DonC
   Modified AutoDesignListWindow to not look for ListQualWndButton value in
   WndDesign entity of default dialog spec.

02.12.1997 DonC
   Modified AutoDesignCase1 so that OK button would transfer to MainListQual
   when a qualified list was specified in User Spec.

14.01.1998 DonC
   Modified oTZADCSDO_AutoDesignListWindow to remove work entities from prior
   iteration.

14.02.1998 DonC
   Modified oTZADCSDO_BuildAutoInclCombobox so that control width was never
   less than 10. (QS 149)

15.03.1998 DonC
   Modified oTZADCSDO_AutoDesignListWindow to correct SystemClose Action.

26.03.1996 DonC
   Modified oTZWDLGSO_CreateGroupbox to avoid error message if there is
   no LOD_Entity, which can occur if the call is from AutoDesignWindow.

29.07.1998 DonC
   Modified oTZWDLGSO_AutoDesignGroupboxM so that a window would be
   built even if the root entity had no attributes. This allows the case
   where an Associative entity is the root and 1 to 1 child entities
   are to appear as combo boxes in the main window.

29.07.1998  DonC
   Eliminated unused operations.
   Added source code generation of DeleteObjectEntity operation.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "zdrvropr.h"
#include "tzctlopr.h"
#include "ZeidonOp.H"

void
fnRemoveAmpersand( zPCHAR pchText );

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateButton
//
// PURPOSE: This routine creates a Button with its Event.  It determines
//          the size and position from the vDefault object and from the
//          Groupbox control on which the button is to be positioned.
//          The nButtonNbr is the number of the button within the Groupbox.
//          ButtonGroupType determines whether the buttons are to be
//          positioned on the bottom ("B") of the Groupbox or on the
//          side ("S") of the Groupbox.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateButton( zVIEW    vSubtask,
                        zVIEW    vUserSpec,
                        zVIEW    vNewDialog,
                        zVIEW    vButton,
                        zVIEW    vDefaults,
                        zPCHAR   szButtonText,
                        zLONG    lButtonNbr,
                        zPCHAR   szButtonGroupType )
{
   zVIEW    vPE;
   zLONG    lPosX;
   zLONG    lPosY;
   zLONG    lButtonBottomMargin;
   zLONG    lButtonRightMargin;
   zLONG    lButtonWidth;
   zLONG    lButtonHeight;
   zLONG    lButtonIncrementY;
   zLONG    lButtonIncrementX;

   // Button Sizes:
   // - ButtonWidth is ButtonHeight from vDefaults.
   // - ButtonHeight is ButtonWidth from vDefaults.
   GetIntegerFromAttribute( &lButtonHeight,
                            vDefaults, "InitValues", "ButtonHeight" );
   GetIntegerFromAttribute( &lButtonWidth,
                            vDefaults, "InitValues", "ButtonWidth" );

   if ( szButtonGroupType[ 0 ] == 'B' )
   {
      // Button Positions for ButtonGroupType bottom ("B"):
      // - The positions are partially determined by the Groupbox size.
      // - Determine the X position as well on the Button number.
      // - Pos Y is bottom of GroupBox minus ButtonHeight and ButtonBottomMargin.
      // - Pos X is constant of ButtonLefMargin for ButtonNbr 1 and is
      //   incremented by (ButtonIncrementX + ButtonWidth) for ButtonNbr > 1.
      GetIntegerFromAttribute( &lPosY,
                               vNewDialog, "Control", "SZDLG_Y" );
      GetIntegerFromAttribute( &lButtonBottomMargin,
                               vDefaults, "InitValues", "ButtonBottomMargin" );
      lPosY = lPosY - lButtonHeight - lButtonBottomMargin;

      GetIntegerFromAttribute( &lPosX,
                               vDefaults, "InitValues", "ButtonLeftMargin" );
      if ( lButtonNbr > 1 )
      {
         GetIntegerFromAttribute( &lButtonIncrementX,
                                 vDefaults, "InitValues", "ButtonIncrementX" );
         lPosX = lPosX + ((lButtonWidth + lButtonIncrementX) * (lButtonNbr - 1));
      }
   }
   else
   {
      // Button Positions for ButtonGroupType right side ("S"):
      // - The positions are partially determined by the Groupbox size.
      // - Determine the Y position as well on the Button number.
      // - Pos X is right side of GroupBox minus ButtonWidth and ButtonRightMargin
      // - Pos Y is constant of ButtonTopMargin for ButtonNbr 1 and is
      //   incremented by (ButtonIncrementY + ButtonHeight) for ButtonNbr > 1.
      GetIntegerFromAttribute( &lPosX, vNewDialog, "Control", "SZDLG_X" );
      GetIntegerFromAttribute( &lButtonRightMargin, vDefaults,
                               "InitValues", "ButtonRightMargin" );
      lPosX = lPosX - lButtonWidth - lButtonRightMargin;

      GetIntegerFromAttribute( &lPosY, vDefaults,
                               "InitValues", "ButtonTopMargin" );
      if ( lButtonNbr > 1 )
      {
         GetIntegerFromAttribute( &lButtonIncrementY, vDefaults,
                                  "InitValues", "ButtonIncrementY" );
         lPosY = lPosY + ((lButtonHeight + lButtonIncrementY) * (lButtonNbr - 1));
      }
   }

   GetViewByName( &vPE, "TZPESRCO", vUserSpec, zLEVEL_TASK );

   SetCursorLastEntity( vButton, "Control", 0 );
   CreateMetaEntity( vSubtask, vButton, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "PushBtn", "" );
   IncludeSubobjectFromSubobject( vButton, "ControlDef",
                                  vPE,     "ControlDef", zPOS_AFTER );
   // Remove the button border.
   SetAttributeFromInteger( vButton, "Control", "Subtype",
                            zCONTROL_BORDEROFF );

   // Create an event for the button.
   CreateMetaEntity( vSubtask, vButton, "Event", zPOS_AFTER );
   SetAttributeFromInteger( vButton, "Event", "Type", 1 );

   // Set the Button size and position.
   SetAttributeFromInteger( vButton, "Control", "SZDLG_Y",
                            lButtonHeight );
   SetAttributeFromInteger( vButton, "Control", "SZDLG_X",
                            lButtonWidth );
   SetAttributeFromInteger( vButton, "Control", "PSDLG_Y",
                            lPosY );
   SetAttributeFromInteger( vButton, "Control", "PSDLG_X",
                            lPosX );

   // Set Button Tag and Text.
   SetAttributeFromString( vButton, "Control", "Text",
                           szButtonText );
   fnRemoveAmpersand( szButtonText );
   SetAttributeFromString( vButton, "Control", "Tag",
                           szButtonText );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateButtonActOper
//
// PURPOSE: This routine creates the triplet Button, Action and Operation
//          on the Dialog and Window, and relates them one to the other.
//          It does not, however, create the VML code.
//          The lButtonNbr is the number of the button within the Groupbox.
//          GroupNbr is the group number within the window, which is used
//          for generating unique Button and Action names.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateButtonActOper( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zVIEW    vNewDialogRoot,
                               zVIEW    vDefaults,
                               zPCHAR   szButtonText,
                               zPCHAR   szActionText,
                               zPCHAR   szOperationName,
                               zLONG    lButtonNbr,
                               zPCHAR   szButtonGroupType,
                               zLONG    lActionType )
{
   zVIEW vButton;         // Subobject for viewing button
   zCHAR  szButtonTag[ 65 ];
   zCHAR  szActionName[ 33 ];

   // Button Tag is concatenation of Entity Name and Button Text.
   GetStringFromAttribute( szButtonTag, zsizeof( szButtonTag ), vUserSpec, "LOD_Entity", "Name" );
   strcat_s( szButtonTag, zsizeof( szButtonText ), szButtonText );
   fnRemoveAmpersand( szButtonTag );
   szButtonTag[ 32 ] = 0;   // Make sure it's max of 32 characters

   // If the Action name is not set by the caller, it is the same as the
   // Button Tag. Otherwise it is the passed ActionText.
   if ( szActionText == 0 || szActionText[ 0 ] == 0 )
      strcpy_s( szActionName, zsizeof( szActionName ), szButtonTag );
   else
      strcpy_s( szActionName, zsizeof( szActionName ), szActionText );

   // Create the basic button.
   CreateViewFromViewForTask( &vButton, vNewDialog, 0 );
   SetViewToSubobject( vButton, "CtrlCtrl" );
   oTZADCSDO_CreateButton( vSubtask, vUserSpec, vNewDialog, vButton,
                           vDefaults, szButtonText, lButtonNbr,
                           szButtonGroupType );

   // Override the Button Tag.
   SetAttributeFromString( vButton, "Control", "Tag", szButtonTag );

   // Create Action and include under Event.
   SetCursorLastEntity( vNewDialogRoot, "Action", 0 );
   CreateMetaEntity( vSubtask, vNewDialogRoot, "Action", zPOS_AFTER );
   SetAttributeFromString( vNewDialogRoot, "Action", "Tag",
                           szActionName );
   IncludeSubobjectFromSubobject( vButton,        "EventAct",
                                  vNewDialogRoot, "Action", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialogRoot, "Action", "Type", lActionType );
   DropView( vButton );

   // Create the Operation and include under the Action. (Both are done
   // by CreateTheOperation.)
   // Only add it if it is not already there.  Otherwise just include it.
   if ( SetCursorFirstEntityByString( vNewDialogRoot, "Operation", "Name",
                                      szOperationName, "Dialog" ) >= zCURSOR_SET )
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActOper",
                                     vNewDialogRoot, "Operation", zPOS_AFTER );
   else
      ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialogRoot,
                                      szOperationName, "", vNewDialogRoot,
                                      "ActOper" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateButtonActObject
//
// PURPOSE: This routine creates the Button and its associated Action,
//          setting automatic object functionality without an
//          Operation.
//          the lButtonNbr is the number of the button within the Groupbox.
//          GroupNbr is the group number within the window, which is used
//          for generating unique Button and Action names.
//
//          The SubActionType identifies the Object Based Operations that
//          is to be selected for the Action.  This automatically includes
//          the correct LOD and entity, which is contained in the UserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateButtonActObject( zVIEW    vSubtask,
                                 zVIEW    vUserSpec,
                                 zVIEW    vNewDialog,
                                 zVIEW    vNewDialogRoot,
                                 zVIEW    vDefaults,
                                 zVIEW    vObject,
                                 zPCHAR   szButtonText,
                                 zPCHAR   szActionText,
                                 zLONG    lButtonNbr,
                                 zPCHAR   szButtonGroupType,
                                 zLONG    lActionType,
                                 zLONG    lSubActionType,
                                 zPCHAR   szNextPrev )
{
   zVIEW  vButton;         // Subobject for viewing button
   zCHAR  szLOD_Name[ 33 ];
   zCHAR  szEntityName[ 33 ];
   zCHAR  szButtonTag[ 65 ];
   zCHAR  szActionName[ 33 ];

   // Button Tag is concatenation of Entity Name and Button Text.
   GetStringFromAttribute( szButtonTag, zsizeof( szButtonTag ), vUserSpec, "LOD_Entity", "Name" );
   strcat_s( szButtonTag, zsizeof( szButtonTag ), szButtonText );
   fnRemoveAmpersand( szButtonTag );
   szButtonTag[ 32 ] = 0;   // Make sure it's max of 32 characters

   // If the Action name is not set by the caller, it is the same as the
   // Button Tag. Otherwise it is the passed ActionText.
   if ( szActionText == 0 || szActionText[ 0 ] == 0 )
      strcpy_s( szActionName, zsizeof( szActionName ), szButtonTag );
   else
      strcpy_s( szActionName, zsizeof( szActionName ), szActionText );

   // Create the basic button.
   CreateViewFromViewForTask( &vButton, vNewDialog, 0 );
   SetViewToSubobject( vButton, "CtrlCtrl" );
   oTZADCSDO_CreateButton( vSubtask, vUserSpec, vNewDialog, vButton,
                           vDefaults, szButtonText, lButtonNbr, szButtonGroupType );

   // Override the Button Tag.
   SetAttributeFromString( vButton, "Control", "Tag", szButtonTag );

   // Create Action and include under Event.
   SetCursorLastEntity( vNewDialogRoot, "Action", 0 );
   CreateMetaEntity( vSubtask, vNewDialogRoot, "Action", zPOS_AFTER );
   SetAttributeFromString( vNewDialogRoot, "Action", "Tag", szActionName );
   IncludeSubobjectFromSubobject( vButton,        "EventAct",
                                  vNewDialogRoot, "Action", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialogRoot, "Action", "Type", lActionType );
   DropView( vButton );

   // Position on the correct ViewObjRef entity and LOD_Entity and
   // create ActMap subobject.
   GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vObject, "LOD", "Name" );
   SetCursorFirstEntityByString( vNewDialogRoot, "ViewObjRef", "Name", szLOD_Name, 0 );

   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
   SetCursorFirstEntityByString( vObject, "LOD_Entity", "Name", szEntityName, 0 );

   CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );

   // Set the automatic actions in the Action entity, if it's not 0.
   if ( lSubActionType )
   {
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_SubAction", lSubActionType );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                     vObject,        "LOD_Entity", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView",
                                     vNewDialogRoot, "ViewObjRef", zPOS_AFTER );
   }

   if ( szNextPrev[ 0 ] == 'N' )
   {
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_NextPrev",
                               128 );
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling",
                               2 );   // Disables on last entity
   }

   if ( szNextPrev[ 0 ] == 'P' )
   {
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_NextPrev", 256 );
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 1 );   // Disables on first entity
   }

   // If the SubActionType is Cancel, also set the Action NoMap to Y.
   if ( lSubActionType == zSubActionCancel )
      SetAttributeFromString( vNewDialogRoot, "Action", "NoMap", "Y" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetGroupboxPosition
//
// PURPOSE: This routine calculates the Groupbox position based on other
//          Groupboxes on the window as follows.
//          1. If this is the only Groupbox, PosX = 5 and PosY = 5.
//          2. If there are other Groupboxes on the window, PosY is determined
//             from the first Groupbox by adding its SizeY and its PosY and
//             the constance 5.
//          3. If there is only one other Groupbox on the window, PosX = 5.
//          4. If there are multiple Groupboxes on the window, PosX is
//             determined by adding the PosX and SizeX of the last created
//             Groupbox + 5.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SetGroupboxPosition( zVIEW    vNewDialog,
                               zVIEW    vDefaults )
{
   zSHORT   nRC;
   zLONG    lPosX;
   zLONG    lWkSize;
   zLONG    lWkPos;
   zLONG    lPosY;
   zLONG    lGroupboxNbr;

   // Count the number of existing Groupboxes.
   lGroupboxNbr = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      lGroupboxNbr++;
      nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
   }

   if ( lGroupboxNbr == 1 )
   {
      lPosX = 5;
      lPosY = 5;
   }
   else
   {
      SetCursorFirstEntity( vNewDialog, "Control", 0 );
      GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
      GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_Y" );
      lPosY = lWkSize + lWkPos + 5;
      if ( lGroupboxNbr == 2 )
         lPosX = 5;
      else
      {
         SetCursorLastEntity( vNewDialog, "Control", 0 );
         SetCursorPrevEntity( vNewDialog, "Control", 0 );
         GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_X" );
         GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_X" );
         lPosX = lWkSize + lWkPos + 5;
      }
   }

   // Set the values just calculated.
   SetCursorLastEntity( vNewDialog, "Control", 0 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lPosX );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lPosY );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   CreateGroupbox
//
// PURPOSE: Create a standard Groupbox control.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_CreateGroupbox( zVIEW    vSubtask,
                          zVIEW    vUSSubEnt,
                          zVIEW    vNewDialog,
                          zVIEW    vPE )
{
   zLONG    lMaxPosY;
   zLONG    lWkPos;
   zLONG    lWkSize;
   zSHORT   nRC;
   zCHAR    szGroupName[ 36 ];
   zCHAR    szSuffix[ 3 ];
   zLONG    lGroupNbr;

   // Count the number of Groupboxes to determine lGroupNbr.
   lGroupNbr = oTZWDLGSO_GetNbrOfGroupboxes( vNewDialog );
   lGroupNbr++;

   // set up the name for the Group to make it unique.
   zltoa( lGroupNbr, szSuffix, zsizeof( szSuffix ) );
   strcat_s( szGroupName, zsizeof( szGroupName ), szSuffix );
   if ( CheckExistenceOfEntity( vUSSubEnt, "LOD_Entity" ) >= zCURSOR_SET )
      GetStringFromAttribute( szGroupName, zsizeof( szGroupName ), vUSSubEnt, "LOD_Entity", "Name" );
   else
      strcpy_s( szGroupName, zsizeof( szGroupName ), "Group" );

   zltoa( lGroupNbr, szSuffix, zsizeof( szSuffix ) );
   strcat_s( szGroupName, zsizeof( szGroupName ), szSuffix );

   // Determine the position by searching any existing Controls and setting
   // the new position at the largest (PosY + SizeY) value + 5.
   lMaxPosY = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lWkPos, vNewDialog, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
      if ( lMaxPosY < lWkPos + lWkSize )
         lMaxPosY = lWkPos + lWkSize;

      nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
   }

   lMaxPosY = lMaxPosY + 5;

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", szGroupName );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "GroupBox", 0 );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lMaxPosY );

   // Remove the Groupbox border.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", zCONTROL_BORDEROFF );

   // Set the Subtype so the first edit box inside will have cursor position.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 537395200 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignGroupboxM
//
// PURPOSE: This is the routine called by various Autodesign Cases that
//          creates a Groupbox control and adds in the maintenance
//          attributes (M_LOD_....) to them.
//          The size of the Groupbox is determined by the data content
//          and by the BottomMargin and RightMargin values from the
//          Defaults object and whether the caller wants the margin on the
//          right (MarginType = "R") or on the bottom (MarginType = "B").
//          If MarginType is null, no margins are added.
//          This allows the caller to leave space for buttons on the
//          right or on the bottom.
//          The X and Y position of the control are determined using the
//          standard GroupboxPosition routine.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignGroupboxM( zVIEW    vSubtask,
                               zVIEW    vUSSubEnt,
                               zVIEW    vNewDialog,
                               zVIEW    vDefaults,
                               zVIEW    vAutoDesWk,
                               zPCHAR   szMarginType )
{
   zVIEW    vPE;
   zLONG    lMaxPosX;
   zLONG    lMaxPosY;
   zLONG    lWkPos;
   zLONG    lWkSize;
   zLONG    lRightMargin;
   zLONG    lBottomMargin;
   zSHORT   nRC;

   GetViewByName( &vPE, "TZPESRCO", vUSSubEnt, zLEVEL_TASK );
   oTZWDLGSO_CreateGroupbox( vSubtask, vUSSubEnt, vNewDialog, vPE );

   SetViewToSubobject( vNewDialog, "CtrlCtrl" );
   if ( CheckExistenceOfEntity( vUSSubEnt, "M_LOD_Attribute" ) >= zCURSOR_SET )
   {
      nRC = ofnTZWDLGSO_BasicWindowPainting( vSubtask, vUSSubEnt, vNewDialog,
                                             vDefaults, vAutoDesWk, "M_LOD_Attribute" );
   }
   else
   {
      nRC = 0;  // indicate everything is OK
   }

   // The return code will be as follows:
   //   -1 if an error occurred.
   //    0 if all controls we painted on the group.
   //    1 if all controls would not fit and only some of the controls
   //      were painted.
   // At the end of this routine, the routine code, nRC, will be returned
   // back to the caller.

   ResetViewFromSubobject( vNewDialog );
   if ( nRC < 0 )
      return( -1 );

   // Determine GroupBox size.
   //   o Determine X size by searching for highest x position of subcontrol
   //     and adding 5 to it.
   //   o Determine Y size by searching for highest y position of subcontrol
   //     and adding 16 to it.
   lMaxPosX = 0;
   lMaxPosY = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "CtrlCtrl", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lWkPos,  vNewDialog, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lWkSize, vNewDialog, "CtrlCtrl", "SZDLG_X" );
      lWkPos = lWkPos + lWkSize;
      if ( lMaxPosX < lWkPos )
         lMaxPosX = lWkPos;

      GetIntegerFromAttribute( &lWkPos,  vNewDialog, "CtrlCtrl", "PSDLG_Y" );
      if ( lMaxPosY < lWkPos )
         lMaxPosY = lWkPos;

      nRC = SetCursorNextEntity( vNewDialog, "CtrlCtrl", "" );
   }

   // Finally compute the size.
   if ( szMarginType[ 0 ] == 'R' )
   {
      GetIntegerFromAttribute( &lRightMargin, vDefaults, "InitValues", "GroupboxRightMargin" );
      lBottomMargin = 0;
   }

   if ( szMarginType[ 0 ] == 'B' )
   {
      GetIntegerFromAttribute( &lBottomMargin, vDefaults, "InitValues", "GroupboxBottomMargin" );
      lRightMargin = 0;
   }

   if ( szMarginType[ 0 ] == 0 )
   {
      lBottomMargin = 0;
      lRightMargin = 0;
   }

   // Set the Groupbox size and position.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxPosX + 5 + lRightMargin );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lMaxPosY + 16 + lBottomMargin );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignGroupboxL
//
// PURPOSE: This is the routine called by various Autodesign Cases that
//          creates a Groupbox control and adds a listbox containing
//          either the L_LOD... attributes or the I_LOD... attributes,
//          depending on the ListType passed.
//          The size of the Groupbox is determined by the data content
//          and by the BottomMargin and RightMargin values from the
//          Defaults object and whether the caller wants the margin on the
//          right (MarginType = "R") or on the bottom (MarginType = "B").
//          If MarginType is null, no margins are added.
//          This allows the caller to leave space for buttons on the
//          right or on the bottom.
//          The X and Y position of the control are determined using the
//          standard GroupboxPosition routine.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignGroupboxL( zVIEW    vSubtask,
                               zVIEW    vUSSubEnt,
                               zVIEW    vNewDialog,
                               zVIEW    vDefaults,
                               zPCHAR   szMarginType,
                               zPCHAR   cListType )
{
   zVIEW    vPE;
   zVIEW    vIncludeLOD;
   zLONG    lWkPosX;
   zLONG    lWkPosY;
   zLONG    lWkSizeY;
   zLONG    lRightMargin;
   zLONG    lBottomMargin;
   zCHAR    szEntityName[ 33 ];
   zCHAR    szLOD_EntityName[ 33 ];
   zCHAR    szLOD_Name[ 33 ];
   zCHAR    cRootOnlyFlag;
   zLONG    lGroupNbr = 0;
   zSHORT   nRC;

   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   oTZWDLGSO_CreateGroupbox( vSubtask, vUSSubEnt, vNewDialog, vPE );

   // Set Position.  Temporarily increment each X position
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 + (lGroupNbr - 1) * 100 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 5 );

   // Position now on the GroupBox control and go add the list box.
   // Note that the 112 and 52 values mean nothing since they will be
   // overridden later.
   SetViewToSubobject( vNewDialog, "CtrlCtrl" );
   ofnTZWDLGSO_AutoDesignListbox( vSubtask, vUSSubEnt, vNewDialog, 5, 112, 52, cListType );

   // Determine GroupBox size.
   //   o Determine X size by adding 10 to the listbox size, plus the
   //     RightMargin size.
   //   o Determine Y size by adding 5 to the listbox size, plus the
   //     listbox starting position, plus the BottomMargin size.
   nRC = SetCursorFirstEntity( vNewDialog, "Control", "" );
   while ( nRC >= zCURSOR_SET &&
           CompareAttributeToString( vNewDialog, "ControlDef", "Tag", "ListBox" ) != 0 )
   {
      nRC = SetCursorNextEntity( vNewDialog, "CtrlCtrl", "" );
   }

   GetIntegerFromAttribute( &lWkPosX,   vNewDialog, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lWkPosY,   vNewDialog, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lWkSizeY,  vNewDialog, "Control", "SZDLG_Y" );

   // Reset back to Groupbox control.
   ResetViewFromSubobject( vNewDialog );

   // Finally compute the size.
   if ( szMarginType[ 0 ] == 'R' )
   {
      GetIntegerFromAttribute( &lRightMargin, vDefaults, "InitValues", "GroupboxRightMargin" );
      lBottomMargin = 0;
   }
   if ( szMarginType[ 0 ] == 'B' )
   {
      GetIntegerFromAttribute( &lBottomMargin, vDefaults, "InitValues", "GroupboxBottomMargin" );
      lRightMargin = 0;
   }
   if ( szMarginType[ 0 ] == 0 )
   {
      lBottomMargin = 0;
      lRightMargin = 0;
   }

   // Set the Groupbox size and position.  The Groupbox Y size will be
   // set later.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lWkPosX + 10 + lRightMargin );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lWkPosY + lWkSizeY + 5 + lBottomMargin );
   oTZWDLGSO_SetGroupboxPosition( vNewDialog, vDefaults );

   // Set the RootOnlyList flag if the attributes to be listed are only contained in the LOD_Entity.
   cRootOnlyFlag = 'Y';
   if ( cListType[ 0 ] == 'L' )
   {
      strcpy_s( szLOD_EntityName, zsizeof( szLOD_EntityName ), "L_LOD_Entity" );
      // LOD_Entity is in USSubEnt for regular list.
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   }
   else
   {
      strcpy_s( szLOD_EntityName, zsizeof( szLOD_EntityName ), "I_LOD_Entity" );
      // LOD_Entity must be retrieved from root of LOD for include list.
      GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vUSSubEnt, "ListSubObjLOD", "Name" );
      ActivateMetaOI_ByName( vSubtask, &vIncludeLOD, 0, zREFER_LOD_META, zSINGLE, szLOD_Name, 0 );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vIncludeLOD, "LOD_EntityParent", "Name" );
      DropView( vIncludeLOD );

   }
   nRC = SetCursorFirstEntity( vUSSubEnt, szLOD_EntityName, "UIS_Entity" );
   while ( nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToString( vUSSubEnt, szLOD_EntityName, "Name", szEntityName ) != 0 )
         cRootOnlyFlag = 'N';

      nRC = SetCursorNextEntity( vUSSubEnt, szLOD_EntityName, "UIS_Entity" );
   }
   if ( cRootOnlyFlag == 'Y' )
      SetAttributeFromString( vUSSubEnt, "UIS_Entity", "RootOnlyList", "Y" );
   else
      SetAttributeFromString( vUSSubEnt, "UIS_Entity", "RootOnlyList", "N" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignGroupboxMT
//
// PURPOSE: This is the routine that creates groupboxes on Tab controls
//          for the main Case1 entity.  It creates a Groupbox control and
//          adds the maintenance attributes (MT_LOD_....) to them.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AutoDesignGroupboxMT( zVIEW    vSubtask,
                                zVIEW    vUSSubEnt,
                                zVIEW    vNewDialog,
                                zVIEW    vDefaults,
                                zVIEW    vAutoDesWk,
                                zPCHAR   szTabName,
                                zPCHAR   szTabNo )
{
   zVIEW    vPE;
   zLONG    lMaxPosX;
   zLONG    lMaxPosY;
   zLONG    lWkPos;
   zLONG    lWkSize;
   zLONG    lRightMargin;
   zLONG    lBottomMargin;
   zSHORT   nRC;
   zCHAR    szGroupName[ 36 ];

   // The GroupName is the TabName.
   strcpy_s( szGroupName, zsizeof( szGroupName ), "G" );
   strcat_s( szGroupName, zsizeof( szGroupName ), szTabNo );
   strcat_s( szGroupName, zsizeof( szGroupName ), szTabName );

   GetViewByName( &vPE, "TZPESRCO", vUSSubEnt, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", szGroupName );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "GroupBox", 0 );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 5 );

   // Remove the Groupbox border.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", zCONTROL_BORDEROFF );

   // Set the Subtype so the first edit box inside will have cursor position.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 537395200 );

   SetViewToSubobject( vNewDialog, "CtrlCtrl" );

   // We must position on the first MT_LOD_Attribute entry before calling
   // BasicWindowPainting because that routine starts at the current
   // position.
   SetCursorFirstEntity( vUSSubEnt, "MT_LOD_Attribute", 0 );
   nRC = ofnTZWDLGSO_BasicWindowPainting( vSubtask, vUSSubEnt, vNewDialog,
                                          vDefaults, vAutoDesWk, "MT_LOD_Attribute" );
   ResetViewFromSubobject( vNewDialog );
   if ( nRC < 0 )
      return( -1 );

   // Determine GroupBox size.
   //   o Determine X size by searching for highest x position of subcontrol
   //     and adding 5 to it.
   //   o Determine Y size by searching for highest y position of subcontrol
   //     and adding 16 to it.
   lMaxPosX = 0;
   lMaxPosY = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "CtrlCtrl", "" );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lWkPos,  vNewDialog, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lWkSize, vNewDialog, "CtrlCtrl", "SZDLG_X" );
      lWkPos = lWkPos + lWkSize;
      if ( lMaxPosX < lWkPos )
         lMaxPosX = lWkPos;

      GetIntegerFromAttribute( &lWkPos,  vNewDialog, "CtrlCtrl", "PSDLG_Y" );
      if ( lMaxPosY < lWkPos )
         lMaxPosY = lWkPos;

      nRC = SetCursorNextEntity( vNewDialog, "CtrlCtrl", "" );
   }

   // Finally compute the size.
   GetIntegerFromAttribute( &lRightMargin,
                            vDefaults, "InitValues", "GroupboxRightMargin" );
   lBottomMargin = 0;

   // Set the Groupbox size and position.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxPosX + 5 + lRightMargin );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lMaxPosY + 16 + lBottomMargin );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignListbox
//
// PURPOSE: This is the routine is called by the Autodesign Case processing
//          routines that builds a list box inside a combo box, but
//          creates no buttons or actions.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDesignListbox( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zLONG    lStart_X,
                               zLONG    lStart_Y,
                               zLONG    lHeight,
                               zPCHAR   cListType )
{
   zVIEW    vAutoDesWk;
   zVIEW    vTempStyle;
   zVIEW    vTopDialog;
   zLONG    lTotalLth;
   zLONG    lControlZKey;
   zLONG    lLevel;
   zLONG    lCC;
   zLONG    lCL;
   zSHORT   nRC;
   zCHAR    szViewObjName[ 33 ];

   lCC = 5;
   lCL = 12;
   GetViewByName( &vAutoDesWk, "TZADWWKO", vUserSpec, zLEVEL_TASK );

   // Create the listbox.
   ofnTZWDLGSO_CreateListBox( vSubtask, vNewDialog, vUserSpec,
                              lStart_X, lStart_Y, lHeight, cListType );

   CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );

   // For ListType "L", include from ViewObjRef and LOD_Entity.
   // For ListType "I", include from ListViewObjRef and I_LOD_Entity.
   if ( cListType[ 0 ] == 'I' )
   {
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView",
                                     vUserSpec,  "ListViewObjRef", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Entity",
                                     vUserSpec, "I_LOD_Entity", zPOS_AFTER );
      // Make sure the ViewObjRef is in the Dialog.
      GetStringFromAttribute( szViewObjName, zsizeof( szViewObjName ), vUserSpec, "ListViewObjRef", "Name" );
      CreateViewFromViewForTask( &vTopDialog, vNewDialog, 0 );
      ResetView( vTopDialog );
      if ( SetCursorFirstEntityByString( vTopDialog, "ViewObjRef", "Name",
                                         szViewObjName, 0 ) < zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vTopDialog, "ViewObjRef",
                                        vUserSpec,  "ListViewObjRef", zPOS_AFTER );
      }
      DropView( vTopDialog );
   }
   else
   {
      /* Create a new view that will point to the LOD entity in the user
         spec object.  Sometimes vUserSpec is pointing to a recursive
         subobject and LOD cannot be accessed. */
      CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );
      ResetView( vTempStyle );

      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView",
                                     vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Entity",
                                     vUserSpec, "LOD_Entity", zPOS_AFTER );
      DropView( vTempStyle );
   }

   GetIntegerFromAttribute( &lControlZKey, vNewDialog, "Control", "ZKey" );

   // Auto paint the fields in the listbox.
   nRC = ofnTZWDLGSO_AutoPaintListBox( vSubtask, vUserSpec,
                                       vNewDialog, cListType );

   // Reset the Y positions to 12 for both listbox and text.
   nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 12 );
      nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
   }

   if ( nRC == -1 )
      return( -1 );

   lLevel = 0;
   SetCursorFirstEntityByInteger( vNewDialog, "Control", "ZKey", lControlZKey, "" );
   GetIntegerFromAttribute( &lTotalLth, vNewDialog, "Control", "SZDLG_X" );
   SetCursorFirstEntityByInteger( vNewDialog, "Control", "ZKey", lControlZKey, "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddActionDoubleClick
//
// PURPOSE: This routine positions on the Listbox subobject for the current
//          Groupbox (it is assumed that the current Control is the
//          Groupbox holding the Listbox) and includes the current Action
//          under it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_AddActionDoubleClick( zVIEW    vSubtask,
                                zVIEW    vNewDialog,
                                zVIEW    vNewDialogRoot )
{
   zVIEW    vNewDialog2;
   zSHORT   nRC;

   CreateViewFromViewForTask( &vNewDialog2, vNewDialog, 0 );
   SetViewToSubobject( vNewDialog2, "CtrlCtrl" );

   // First position on the listbox.
   // We will have to loop instead of doing a SetCursor because the
   // scoping entity we would need is not in the view.
   nRC = SetCursorFirstEntity( vNewDialog2, "Control", 0 );
   while ( nRC >= zCURSOR_SET &&
           CompareAttributeToString( vNewDialog2, "ControlDef", "Tag", "ListBox" ) != 0 )
   {
      nRC = SetCursorNextEntity( vNewDialog2, "Control", 0 );
   }

   // Create the window event and include the Action.
   // Type 2 is double click.
   CreateMetaEntity( vSubtask, vNewDialog2, "Event", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog2, "Event", "Type", 2 );
   IncludeSubobjectFromSubobject( vNewDialog2,     "EventAct",
                                  vNewDialogRoot,  "Action", zPOS_AFTER );

   DropView( vNewDialog2 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ResetScopeToObject
//
// PURPOSE: This routine simply counts the current number of Groupbox
//          entities currently under the window and returns that count.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
oTZWDLGSO_ResetScopeToObject( zVIEW    vNewDialog )
{
   zVIEW    vListboxWk;
   zSHORT   nRC;
   zLONG    lSubtype;

   // Reset scope to "Object Instance".
   nRC = GetViewByName( &vListboxWk, "TZPNCTWO", vNewDialog, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      if ( CheckExistenceOfEntity( vListboxWk, "ListBox" ) < zCURSOR_SET )
         CreateEntity( vListboxWk, "ListBox", zPOS_AFTER );
      SetEntityAttributesFromBlob( vListboxWk, "ListBox", vNewDialog, "CtrlCtrl", "CtrlBOI" );
      SetAttributeFromInteger( vListboxWk, "ListBox", "ScopeOI", 3 );
      SetBlobFromEntityAttributes( vNewDialog, "CtrlCtrl", "CtrlBOI", vListboxWk, "ListBox" );
      lSubtype = 24640;
      lSubtype |= zLISTBOX_SORTEDHEADERS;
      SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "Subtype", lSubtype );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GetNbrOfGroupboxes
//
// PURPOSE: This routine simply counts the current number of Groupbox
//          entities currently under the window and returns that count.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
oTZWDLGSO_GetNbrOfGroupboxes( zVIEW    vNewDialog )
{
   zVIEW    vNewDialog2;
   zSHORT   nRC;
   zLONG    lGroupNbr;

   lGroupNbr = 0;

   if ( CheckExistenceOfEntity( vNewDialog, "Control" ) >= zCURSOR_SET )
   {
      CreateViewFromViewForTask( &vNewDialog2, vNewDialog, 0 );
      // If we are not currently on a Groupbox, return up one level.
      if ( CompareAttributeToString( vNewDialog2, "ControlDef", "Tag", "GroupBox" ) != 0 )
         ResetViewFromSubobject( vNewDialog2 );

      nRC = SetCursorFirstEntity( vNewDialog2, "Control", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         lGroupNbr++;
         nRC = SetCursorNextEntity( vNewDialog2, "Control", 0 );
      }

      DropView( vNewDialog2 );
   }

   return( lGroupNbr );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AssignTabbingOrder
//
// PURPOSE: This routine loops through all the Controls on a window and
//          assigns a value of 9999 to Text Controls and a sequential
//          number to all other Controls.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
oTZWDLGSO_AssignTabbingOrder( zVIEW    vNewDialog )
{
   zSHORT  nRC;
   zSHORT  nTabCount;

   nTabCount = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      nTabCount++;
      SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", nTabCount );

      SetViewToSubobject( vNewDialog, "CtrlCtrl" );
      nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToString( vNewDialog, "ControlDef", "Tag", "Text" ) == 0 )
            SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 9999 );
         else
         {
            nTabCount++;
            SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", nTabCount );
         }

         nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
      }
      ResetViewFromSubobject( vNewDialog );

      nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
   }

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SetGroupboxSize
//
// PURPOSE: This routine calculates the Groupbox size by the controls
//          contained in the Groupbox and sets the size.
//          The SizeX is determined by adding the X pos and X size of each
//          control and determining the max value.  Then add 5 to that value.
//          The SizeY is determined by finding the max Y pos of the controls
//          and adding the button height + 3 to it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_SetGroupboxSize( zVIEW    vNewDialog,
                           zVIEW    vDefaults )
{
   zSHORT   nRC;
   zLONG    lPosX;
   zLONG    lSizeX;
   zLONG    lMaxX;
   zLONG    lPosY;
   zLONG    lMaxY;
   zLONG    lButtonHeight;

   lMaxX = 0;
   lMaxY = 0;
   nRC = SetCursorFirstEntity( vNewDialog, "CtrlCtrl", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // MaxX
      GetIntegerFromAttribute( &lPosX, vNewDialog, "CtrlCtrl", "PSDLG_X" );
      GetIntegerFromAttribute( &lSizeX, vNewDialog, "CtrlCtrl", "SZDLG_X" );
      if ( lMaxX < lPosX + lSizeX )
         lMaxX = lPosX + lSizeX;

      // MaxY
      GetIntegerFromAttribute( &lPosY, vNewDialog, "CtrlCtrl", "PSDLG_Y" );
      if ( lMaxY < lPosY )
         lMaxY = lPosY;

      nRC = SetCursorNextEntity( vNewDialog, "CtrlCtrl", 0 );
   }

   GetIntegerFromAttribute( &lButtonHeight, vDefaults, "InitValues", "ButtonHeight" );

   // Set the values just calculated.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxX + 5 );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lMaxY + lButtonHeight + 3 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignAssocDep
//
// PURPOSE: This is the routine to process the children of an Assoc entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignAssocDep( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vUSSubEnt,
                              zVIEW  vNewDialog,
                              zVIEW  vNewDialogRoot,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR szCaseOrig )
{
   zSHORT   nRC;
   zCHAR    szCase[ 3 ];

   GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );

   // Position a handle to the subordinate entities of the
   // current entity (if there are any).
   SetViewToSubobject( vUSSubEnt, "UIS_ChildEntity" );

   // Loop through the subordinate entities and process each Autodesign
   // Case 1 and 4.  They are the only valid cases under an Assoc entity.
   nRC = SetCursorFirstEntity( vUSSubEnt, "UIS_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {

      ////////////////////////////////////////////////////////////
      // Build a Groupbox on this window and a subordinate window
      // as necessary for each sub entity.
      //
      // The processing depends on the various Case Types: 1, 4
      ////////////////////////////////////////////////////////////

      GetStringFromAttribute( szCase, zsizeof( szCase ), vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );
      if ( szCase[ 0 ] == '1' )
      {
         ////////
         // Case 1 List
         ////////

         oTZADCSDO_AutoDesignCase1List( vSubtask, vUserSpec, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                        vDefaults, vObject, szCase );
      }
      else
      // This is the Case 4 on the child entity.
      if ( szCase[ 0 ] == '4' )
      {
         if ( CompareAttributeToString( vUSSubEnt, "ER_Entity", "Purpose", "S" ) != 0 )
         {
            ////////
            // Case 4  (It is driven by the 4Group routine.)
            ////////

            oTZADCSDO_AutoDesignCase4Group( vSubtask, vUserSpec, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                            vDefaults, vObject, szCase );
         }
      }

      nRC = SetCursorNextEntity( vUSSubEnt, "UIS_Entity", "" );
   }

   ResetViewFromSubobject( vUSSubEnt );

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BuildAutoInclCombobox
//
// PURPOSE: Build an Auto Include Combobox on a parent window for an
//          entity that is to be included.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_BuildAutoInclCombobox( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zVIEW  vUSSubEnt,
                                 zVIEW  vDefaults,
                                 zVIEW  vNewDialog,
                                 zVIEW  vNewDialogTop,
                                 zVIEW  vPE,
                                 zLONG  lPromptPosX,
                                 zLONG  lComboPosX,
                                 zLONG  lComboPosY )
{
   zVIEW    vPN_Work;
   zCHAR    szControlName[ 38 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szWorkString[ 254 ];
   zLONG    lTextLength;
   zLONG    lCL;
   zLONG    lCharWidth;
   zLONG    lZKey;

   // Initialize the Text Control size from the default Edit Control size and
   // also set the increment between controls to 15.
   GetIntegerFromAttribute( &lCL,
                            vDefaults, "InitValues", "EditboxHeight" );
   if ( lCL == 0 )
      lCL = 12;
   lCharWidth = 4;

   //////////////
   // Create the Prompt Text Control.
   //////////////

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );

   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Text", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
   // Remove the text border.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype",
                            zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );

   // Prompt is either ListGroupboxText (the prompt value entered) or the entity name.
   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUSSubEnt, "WndDesign", "ListGroupboxText" );
   if ( szWorkString[ 0 ] == 0 )
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUSSubEnt, "LOD_Entity", "Name" );
   SetAttributeFromString( vNewDialog, "Control", "Text", szWorkString );
   SetAttributeFromString( vNewDialog, "Control", "Tag",  szWorkString );
   lTextLength = zstrlen( szWorkString ) * lCharWidth;
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lTextLength );

   // Use Edit Box default height for Text.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );

   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lPromptPosX );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lComboPosY );

   //////////////
   // Create the Combo Box Control and ControlDef.
   //////////////

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   strcpy_s( szControlName, zsizeof( szControlName ), "Combo" );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   strcat_s( szControlName, zsizeof( szControlName ), szEntityName );
   szControlName[ 33 ] = 0;
   SetAttributeFromString( vNewDialog, "Control", "Tag", szControlName );

   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "ComboBox", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );

   // Set up the Subtype and the CtrlBOI.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 20480 );
   ActivateEmptyObjectInstance( &vPN_Work, "TZPNCTWO", vSubtask, 0 );
   CreateEntity( vPN_Work, "TZPNCTWO", zPOS_AFTER );
   CreateEntity( vPN_Work, "ComboBox", zPOS_AFTER );
   SetAttributeFromInteger( vPN_Work, "ComboBox", "Subtype", 20480 );
   SetAttributeFromInteger( vPN_Work, "ComboBox", "FormatType", 0 );
   SetBlobFromEntityAttributes( vNewDialog, "Control", "CtrlBOI", vPN_Work, "ComboBox" );
   DropView( vPN_Work );

   // Combobox should always have a height of 100 dlgs.
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", 100 );

   // Width of Combo is assumed to be string length in Domain, since we
   // expect the field mapping to be a string.  If not, we will use a
   // constant.  Also max size is 100.
   if ( CheckExistenceOfEntity( vUSSubEnt, "I_Domain" ) >= zCURSOR_SET )
      GetIntegerFromAttribute( &lTextLength, vUSSubEnt, "I_Domain", "MaxStringLth" );
   else
      lTextLength = 10;
   if ( lTextLength > 50 )
      lTextLength = 50;
   if ( lTextLength < 10 )
      lTextLength = 10;
   lTextLength = lTextLength * lCharWidth;
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lTextLength );

   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lComboPosX );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lComboPosY );

   //////////////
   // Create CtrlMap for Edit Attribute Mapping
   //////////////

   // These are CtrlMap, CtrlMapView and CtrlMapLOD_Attribute, the last
   // two included from the USSubEnt.
   // M_LOD_Attribute was the LOD_Attribute included from the main object.
   if ( CheckExistenceOfEntity( vUserSpec, "UIS_ViewObjRef" )  >= zCURSOR_SET &&
        CheckExistenceOfEntity( vUSSubEnt, "M_LOD_Attribute" ) >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView", vUserSpec,  "UIS_ViewObjRef", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Attribute", vUSSubEnt,  "M_LOD_Attribute", zPOS_AFTER );
   }

   //////////////
   // Create CtrlMap for List Attribute Mapping
   //////////////

   // These are CtrlMap, CtrlMapView and CtrlMapLOD_Attribute, the last
   // two included from the UserSpec.
   // I_LOD_Attribute was the LOD_Attribute included from the list object.
   if ( CheckExistenceOfEntity( vUSSubEnt, "ListViewObjRef" )  >= zCURSOR_SET &&
        CheckExistenceOfEntity( vUSSubEnt, "I_LOD_Attribute" ) >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView", vUSSubEnt,  "ListViewObjRef", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Attribute", vUSSubEnt,  "I_LOD_Attribute", zPOS_AFTER );

      // Add the ViewObjRef to the Dialog, if it isn't already there.
      GetIntegerFromAttribute( &lZKey, vUSSubEnt, "ListViewObjRef", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vNewDialogTop, "ViewObjRef", "ZKey", lZKey, 0 )  < zCURSOR_SET )
         IncludeSubobjectFromSubobject( vNewDialogTop, "ViewObjRef", vUSSubEnt,  "ListViewObjRef", zPOS_AFTER );
   }

   //////////////
   // Create CtrlMap for List entity.
   //////////////

   // These are CtrlMap and CtrlMapLOD_Entity, with CtrlMapLOD_Entity
   // being included from the LOD_Entity under I_LOD_Attribute.
   // I_LOD_Attribute was the LOD_Attribute included from the list object.
   if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Attribute" ) >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Entity", vUSSubEnt,  "I_LOD_Entity", zPOS_AFTER );
   }

   //////////////
   // Create Last CtrlMap
   //////////////

   // I don't understand why but the Combo Box requires a fourth empty CtrlMap entity.
   CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesign Case5Attribs
//
// PURPOSE: This is the routine to add each Case 5\6 entity as an AutoInclude
//          Combo box in the group that contains the attributes on the
//          Associative entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase5Attrs( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEntOrig,
                                zVIEW  vNewDialog,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR szCase)
{
   zVIEW    vUSSubEnt;
   zVIEW    vPE;
   zVIEW    vNewDialogTemp;
   zVIEW    vNewDialogTop;
   zVIEW    vUSSubEntParent;
   zSHORT   nRC;
   zCHAR    szEntityName[ 33 ];
   zCHAR    szWorkString[ 254 ];
   zCHAR    szEntityCase[ 3 ];
   zLONG    lPromptPosX;
   zLONG    lComboPosX;
   zLONG    lComboPosY;
   zLONG    lMaxLth;
   zLONG    lCI;
   zLONG    lCharWidth;
   zLONG    lWkSize1;
   zLONG    lWkSize2;

   // Just make it a constant right now.
   lCI = 15;
   lCharWidth = 4;

   GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   // Use a work view for vUSSubEnt so we don't mess it up for the caller
   // or for ourselves.
   CreateViewFromViewForTask( &vUSSubEnt, vUSSubEntOrig, 0 );

   // If there is no Case 5/6 entity, simply return.
   if ( SetCursorFirstEntityByString( vUSSubEnt, "UIS_Entity", "AutodesignCaseType",
                                      szCase, 0 ) < zCURSOR_SET )
   {
      DropView( vUSSubEnt );
      return( 0 );
   }

   // Create the Group box if it doesn't already exist.
   if ( CheckExistenceOfEntity( vNewDialog, "Control" ) < zCURSOR_SET )
   {
      oTZWDLGSO_CreateGroupbox( vSubtask, vUSSubEnt, vNewDialog, vPE );
   }

   // Create a view to the Dialog Window.
   CreateViewFromViewForTask( &vNewDialogTop, vNewDialog, 0 );
// ResetView( vNewDialogTop );
   nRC = ResetViewFromSubobject( vNewDialogTop );
   while ( nRC == 0 )
   {
      nRC = ResetViewFromSubobject( vNewDialogTop );
   }

SetNameForView( vNewDialogTop, "***NewDialogTop", vSubtask, zLEVEL_TASK );
SetNameForView( vNewDialog, "***NewDialog", vSubtask, zLEVEL_TASK );
   // We are currently positioned on the Groupbox Control.  Set the view
   // down one level for creating the subcontrols.
   SetViewToSubobject( vNewDialog, "CtrlCtrl" );

   // The Prompt position will be the same as prior prompt positions, or it
   // will be a constant if no prior positions.
   // The Combobox position will be the greater of prior Control positions
   // or 5 greater than the end of the largest Prompt.
   CreateViewFromViewForTask( &vNewDialogTemp, vNewDialog, 0 );
   if ( SetCursorFirstEntity( vNewDialogTemp, "Control", 0 ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lPromptPosX, vNewDialogTemp, "Control", "PSDLG_X" );
      SetCursorNextEntity( vNewDialogTemp, "Control", 0 );
      GetIntegerFromAttribute( &lComboPosX, vNewDialogTemp, "Control", "PSDLG_X" );
   }
   else
   {
      lPromptPosX = 6;
      lComboPosX = 50;
   }
   DropView( vNewDialogTemp );

   lMaxLth = 0;
   nRC = SetCursorFirstEntity( vUSSubEnt, "UIS_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetStringFromAttribute( szEntityCase, zsizeof( szEntityCase ), vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );
      if ( szEntityCase[ 0 ] == szCase[ 0 ] )
      {
         GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUSSubEnt, "WndDesign", "ListGroupboxText" );
         if ( szWorkString[ 0 ] == 0 )
            GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUSSubEnt, "LOD_Entity", "Name" );
         if ( lMaxLth < ( zLONG )zstrlen( szWorkString ) )
            lMaxLth = zstrlen( szWorkString );
      }
      nRC = SetCursorNextEntity( vUSSubEnt, "UIS_Entity", "" );
   }
   lMaxLth = lMaxLth * lCharWidth;
   if ( lComboPosX < lPromptPosX + lMaxLth )
      lComboPosX = lPromptPosX + lMaxLth;

   // If there are already controls, the ComboBox Y pos will start after
   // the last control.  Otherwise constants will be set.
   // those controls, which is the same as the first two controls.
   if ( SetCursorLastEntity( vNewDialog, "Control", 0 ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lComboPosY, vNewDialog, "Control", "PSDLG_Y" );

   }
   else
   {
      lComboPosY = 0;
   }

   // Loop through the UIS entities and process each Autodesign
   // Case 5/6.
   nRC = SetCursorFirstEntity( vUSSubEnt, "UIS_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {

      ///////////////////////////////////////////////////////////////
      // Build an AutoInclude Combo box for each Case 5/6 UIS_Entity.
      ///////////////////////////////////////////////////////////////

      GetStringFromAttribute( szEntityCase, zsizeof( szEntityCase ),
                              vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );
      if ( szEntityCase[ 0 ] == szCase[ 0 ] )
      {

         // Increment the Y pos by default value.
         lComboPosY = lComboPosY + lCI;

         // Go to create the Combobox.
         oTZADCSDO_BuildAutoInclCombobox( vSubtask, vUserSpec, vUSSubEnt, vDefaults, vNewDialog,
                                          vNewDialogTop, vPE, lPromptPosX, lComboPosX, lComboPosY );
      }

      nRC = SetCursorNextEntity( vUSSubEnt, "UIS_Entity", "" );
   }

   // Reset vNewDialog back to the Groupbox view.
   ResetViewFromSubobject( vNewDialog );

   // Recompute the Groupbox size and then add right margin to leave room
   // for buttons.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );
   GetIntegerFromAttribute( &lWkSize1, vNewDialog, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lWkSize2, vDefaults, "InitValues", "GroupboxRightMargin" );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lWkSize1 + lWkSize2 );

   //////////////
   // Create Activate Operation for Case 5/6 Entities
   //////////////

   // Create the operation that will Activate the list views for all
   // Case 5/6 entities, as necessary, and add it under the PreBuild
   // event on the window.  The name of the operation will be
   // "ListSub" followed by the name of the immediate parent entity
   // for the Case 5 or Case 6 entity.

   CreateViewFromViewForTask( &vUSSubEntParent, vUSSubEntOrig, 0 );
   ResetViewFromSubobject( vUSSubEntParent );

   // vUSSubEntParent is now positioned on Associative entity.
   // Set up the operation name.
   strcpy_s( szWorkString, zsizeof( szWorkString ), "ListSub" );
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEntParent, "LOD_Entity", "Name" );
   strcat_s( szWorkString, zsizeof( szWorkString ), szEntityName );
   szWorkString[ 33 ] = 0;

   // Create Action.
   SetCursorLastEntity( vNewDialogTop, "Action", 0 );
   CreateMetaEntity( vSubtask, vNewDialogTop, "Action", zPOS_AFTER );
   SetAttributeFromString( vNewDialogTop, "Action", "Tag", szWorkString );
   SetAttributeFromInteger( vNewDialogTop, "Action", "Type",  1 );
   SetAttributeFromString( vNewDialogTop, "Action", "NoMap",  "Y" );

   // Add the PreBuild window event and include the Action.
   CreateMetaEntity( vSubtask, vNewDialogTop, "WndEvent", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialogTop, "WndEvent", "Type",  1 );
   IncludeSubobjectFromSubobject( vNewDialogTop, "WndAct", vNewDialogTop, "Action", zPOS_AFTER );

   // Create the Operation and include under the Action. (Both are done
   // by CreateTheOperation.)
   // Only add it if it is not already there.  Otherwise just include it.
   if ( SetCursorFirstEntityByString( vNewDialogTop, "Operation", "Name",
                                      szWorkString, "Dialog" ) >= zCURSOR_SET )
      IncludeSubobjectFromSubobject( vNewDialog,    "ActOper",
                                     vNewDialogTop, "Operation", zPOS_AFTER );
   else
      ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialogTop, szWorkString,
                                      "", vNewDialogTop, "ActOper" );

   // Go to create the VML for the operation.
   DropView( vUSSubEnt );
   CreateViewFromViewForTask( &vUSSubEnt, vUSSubEntOrig, 0 );
   ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogTop, "ListSub" );

   DropView( vUSSubEntParent );

   DropView( vUSSubEnt );
   DropView( vNewDialogTop );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateListViewLST
//
// PURPOSE: This routine retrieves or creates the LST VOR of an existing LOD
//          for use by AutoDesignListWindow and includes it in the Dialog.
//
//          Note that when this routine returns, the ListVOR entity in the
//          User Spec has been created (actually included).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_CreateListViewLST( zVIEW    vSubtask,
                             zVIEW    vUserSpec,
                             zVIEW    vNewDialog,
                             zVIEW    vLOD )
{
   zVIEW    vVOR;
   zCHAR    szListViewName[ 64 ];
// zSHORT   nLth;
   zSHORT   nRC;

   GetStringFromAttribute( szListViewName, zsizeof( szListViewName ), vUserSpec, "UIS_LOD", "Name" );
// nLth = zstrlen( szListViewName );
   // The length can't be greater than 32 with LST extention.
// if ( nLth + 3 > 32 )
// {
      szListViewName[ 29 ] = 0;
// }
   strcat_s( szListViewName, zsizeof( szListViewName ), "LST" );

   // If the ListVOR entity does not already exist in the UserSpec, add it.
   if ( CheckExistenceOfEntity( vUserSpec, "ListVOR" ) < zCURSOR_SET )
   {
      // Check to see if a VOR already exists.
      nRC = ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zSOURCE_VOR_META,
                                   zSINGLE, szListViewName, 0 );

      // VOR exists.
      if ( nRC >= 0 )
      {
         SetNameForView( vVOR, "ListVOR", vSubtask, zLEVEL_TASK );
         IncludeSubobjectFromSubobject( vUserSpec, "ListVOR",
                                        vVOR,      "ViewObjRef", zPOS_AFTER );
      }
      else
      // VOR doesn't exist so create one for LOD vLOD.
      {
         ActivateEmptyMetaOI( vSubtask, &vVOR, zSOURCE_VOR_META, zSINGLE );
         CreateMetaEntity( vSubtask, vVOR, "ViewObjRef", zPOS_AFTER );
         SetAttributeFromString( vVOR, "ViewObjRef", "Name", szListViewName );

         IncludeSubobjectFromSubobject( vVOR, "LOD", vLOD, "LOD", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vUserSpec, "ListVOR",
                                        vVOR,      "ViewObjRef", zPOS_AFTER );

         nRC = CommitMetaOI( vSubtask, vVOR, zSOURCE_VOR_META );
      }
   }

   // Make sure the List View is included in the Dialog.
   nRC = SetCursorFirstEntityByString( vNewDialog, "ViewObjRef", "Name", szListViewName, 0 );
   if ( nRC < zCURSOR_SET )
      IncludeSubobjectFromSubobject( vNewDialog, "ViewObjRef", vUserSpec,  "ListVOR", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignListWindow
//
// PURPOSE: This is the routine creates the List window, which is the
//          window that starts the dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignListWindow( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vNewDialog,
                                zVIEW  vDefaults )
{
   zVIEW    vAutoDesWk;
   zVIEW    vObject;
   zVIEW    vTempLOD;
   zVIEW    vListQualWnd;
   zSHORT   nRC;
   zCHAR    szWorkString[ 256 ];
   zCHAR    szSubWindowTag[ 33 ];
   zCHAR    szButtonText[ 21 ];
   zLONG    lPosX;
   zLONG    lPosY;
   zLONG    lGroupboxWidth;
   zLONG    lButtonIncrementX;
   zLONG    lSize_X;
   zLONG    lSize_Y;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignWDOD", "Y" );

   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );

   // Set the caption from the Group...

   // The list window name is the Entity name, followed by the characters
   // "_List".
   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "LOD_Entity", "Name" );
   strcat_s( szWorkString, zsizeof( szWorkString ), "_List" );
   SetAttributeFromString( vNewDialog, "Window", "Tag", szWorkString );

   // Go add the list view VOR to the User Spec and set the name of the
   // corresponding LOD instance to the name of the VOR.  This way we can
   // know the LOD instance for the VOR, even if it is different from the LOD
   // in the User Spec.
   oTZWDLGSO_CreateListViewLST( vSubtask, vUserSpec, vNewDialog, vObject );

   // We need the listbox that is to be autodesigned by AutoDesignGroupboxL
   // to use the List view for the object instead of the main view.  This is
   // because the list view may be for a special LOD for performance reasons.
   // The problem is that the normal use of AutoDesignGroupboxL, passing a
   // parm of 'L', will use the main view and LOD, instead of the list view
   // and LOD.  Because of that problem, we will set up the UIS_Include
   // subobject in the UserSpec, duplicating the L_LOD_Attributes in
   // I_LOD_Attributes and setting the correct VOR in ListViewObjRef.
   // At the same time, we will validate that each L_LOD_Attribute is
   // actually in the List LOD, since the List LOD is not necessarily the
   // same LOD as the one in the User Spec, from which the attributes were
   // first created.
   //
   // After creating the list box, we will have to verify that the
   // CtrlMapLOD_Entity, that identifies the entity being listed, is the
   // same entity as LOD_Entity in the UserSpec.  In AutoDesignGroupboxL
   // the LOD_Entity selected came from one of the attributes being listed,
   // which could have been on a different entity in the LOD.  Thus after
   // we have created the list box, we will verify that the correct entity
   // was included.  If it wasn't, we will have to exclude the current
   // entity and include the correct one out of the list LOD.

   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "ListLOD", "Name" );
   nRC = ActivateMetaOI_ByName( vSubtask, &vTempLOD, 0, zREFER_LOD_META,
                                zSINGLE, szWorkString, 0 );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "AD10211", "Autodesigner",
                   "List LOD cannot be activated.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // First exclude any that exist from past execution.
   for ( nRC = SetCursorFirstEntity( vUserSpec, "I_LOD_Attribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vUserSpec, "I_LOD_Attribute", 0 ) )
   {
      ExcludeEntity( vUserSpec, "I_LOD_Attribute", zREPOS_NONE );
   }

   // Next include them back.
   nRC = SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "L_LOD_Entity", "Name" );
      nRC = SetCursorFirstEntityByString( vTempLOD, "LOD_Entity", "Name", szWorkString, 0 );
      if ( nRC < zCURSOR_SET )
      {
         MessageSend( vSubtask, "AD10212", "Autodesigner",
                     "Entity in User Spec not in List LOD.",
                     zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "L_ER_Attribute", "Name" );
      nRC = SetCursorFirstEntityByString( vTempLOD, "ER_Attribute", "Name", szWorkString, "LOD_Entity" );
      if ( nRC < zCURSOR_SET )
      {
         MessageSend( vSubtask, "AD10213", "Autodesigner",
                     "Attribute in User Spec not in List LOD.",
                     zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      IncludeSubobjectFromSubobject( vUserSpec, "I_LOD_Attribute",
                                     vUserSpec, "L_LOD_Attribute", zPOS_AFTER );
      nRC = SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", 0 );
   }
   CreateMetaEntity( vSubtask, vUserSpec, "UIS_Include", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vUserSpec, "ListViewObjRef",
                                  vUserSpec, "ListVOR", zPOS_AFTER );

   // Add the Groupbox and Listbox for listing the root entity for the
   // object.
   oTZWDLGSO_AutoDesignGroupboxL( vSubtask, vUserSpec, vNewDialog, vDefaults, "B", "I" );

   // Now check to see if the CtrlMapLOD_Entity for the list box is the same
   // as the LOD_Entity in the UserSpec.  If not, we'll have to locate the
   // correct entity in the LOD and include it in place of the current one.
   // vNewDialog.Control is currently positioned on the Groupbox.  We must
   // step down to the Listbox.
   SetViewToSubobject( vNewDialog, "CtrlCtrl" );
   if ( CompareAttributeToAttribute( vUserSpec,  "LOD_Entity",        "Name",
                                     vNewDialog, "CtrlMapLOD_Entity", "Name" ) !=  0 )
   {
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "LOD_Entity", "Name" );
      SetCursorFirstEntityByString( vObject, "LOD_Entity", "Name", szWorkString, 0 );
      ExcludeEntity( vNewDialog, "CtrlMapLOD_Entity", zREPOS_NONE );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Entity", vObject, "LOD_Entity", zPOS_AFTER );
   }
   ResetViewFromSubobject( vNewDialog );

   // Reset scope to "Object Instance".
   oTZWDLGSO_ResetScopeToObject( vNewDialog );

   // Set Window Caption.  The Groupbox we just created also contains the
   // text we want, so we will eliminate it from there.
   if ( CompareAttributeToString( vUserSpec, "WndDesign", "ListGroupboxText", "" ) != 0 )
      SetAttributeFromAttribute( vNewDialog, "Window",    "Caption",
                                 vUserSpec,  "WndDesign", "ListGroupboxText" );
   else
   {
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "UIS_LOD", "Name" );
      strcat_s( szWorkString, zsizeof( szWorkString ), " List" );
      SetAttributeFromString( vNewDialog, "Window", "Caption", szWorkString );
   }
   SetAttributeFromString( vNewDialog, "Control",   "Text", "" );

   // Add the new,etc. buttons as necessary.  The routine to do this requires
   // the name of the subwindow, which in this case, is the name of the
   // LOD_Entity for the first UIS_Entity.
   GetStringFromAttribute( szSubWindowTag, zsizeof( szSubWindowTag ),
                           vUserSpec, "LOD_Entity", "Name" );
   oTZADCSDO_CreateCase1Buttons( vSubtask, vUserSpec, vUserSpec, vNewDialog, vNewDialog,
                                 vDefaults, vObject, szSubWindowTag, "MainWindow" );

   // Recalculate the Groupbox size based on the buttons just added and set it.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );

   // Add the Exit button.

   ////////
   // Exit
   ////////

   // We will have to reposition the button based on the position and size
   // of the Groupbox.  We want to position it at the same PosY and the
   // PosX + Groupbox width + increment.
   // We will compute these values before creating the Exit button, while
   // we're still positioned on the Groupbox control.
   GetIntegerFromAttribute( &lPosY, vNewDialog, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lPosX, vNewDialog, "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lGroupboxWidth, vNewDialog, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lButtonIncrementX, vDefaults, "InitValues", "ButtonIncrementX" );
   lPosX = lPosX + lGroupboxWidth + lButtonIncrementX;

   // Go create the button.
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Exit" );
   oTZADCSDO_CreateButton( vSubtask, vUserSpec, vNewDialog, vNewDialog,
                           vDefaults, szButtonText, 1, "" );
   SetAttributeFromString( vNewDialog, "Control", "Tag", "Exit" );

   // Create Action and include under Event.
   SetCursorLastEntity( vNewDialog, "Action", 0 );
   CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Action", "Tag", "Alt-F4" );
   IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Action", "Type", zWAB_ReturnToParentWithRefresh );

   // Finish the button by setting the position and getting the Size_Y value
   // for the ListQual button.                           .
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lPosY );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lPosX );
   GetIntegerFromAttribute( &lSize_Y, vNewDialog, "Control", "SZDLG_Y" );


   // Set the Default Window.
   IncludeSubobjectFromSubobject( vNewDialog, "DfltWnd", vNewDialog, "Window", zPOS_AFTER );

   // If there is a List Qual subwindow, go to add the subwindow and then
   // add "GoToListQual" button and Postbuild event.
   // If there is no List Qual subwindow, add Prebuild and ReturnFromSubwindow events.
   if ( CompareAttributeToString( vUserSpec, "WndDesign", "ListQualFlag", "Y" ) == 0 )
   {
      // Create the empty ListQual window.
      // Use temporary view vListQualWnd so as not to lose cursor position.
      CreateViewFromViewForTask( &vListQualWnd, vNewDialog, 0 );
      CreateMetaEntity( vSubtask, vListQualWnd, "Window", zPOS_AFTER );
      SetAttributeFromString( vListQualWnd, "Window", "Tag", "MainListQual" );

      // Go to complete the ListQual window.
      oTZADCSDO_AutoDesignListQual( vSubtask, vUserSpec, vListQualWnd,
                                    vDefaults, vObject, "ActivateMainList" );

      // Create Button to transfer to ListQual window.
#if 0
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "WndDesign", "ListQualWndButtonText" );
      if ( szButtonText[ 0 ] == 0 )
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "DfltWndDesign", "ListQualWndButtonText" );
#endif
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "DfltWndDesign", "ListQualWndButtonText" );
      if ( szButtonText[ 0 ] == 0 )
         strcpy_s( szButtonText, zsizeof( szButtonText ), "ListQualNew" );

      oTZADCSDO_CreateButton( vSubtask, vUserSpec, vNewDialog, vNewDialog,
                              vDefaults, szButtonText, 1, "" );
      SetAttributeFromString( vNewDialog, "Control", "Tag", szButtonText );

      // Create Action and include under Event.
      SetCursorLastEntity( vNewDialog, "Action", 0 );
      CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
      SetAttributeFromString( vNewDialog, "Action", "Tag", szButtonText );
      IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "Action", "Type", zWAB_StartModalSubwindow );

      // Finish the button by setting the position, which is based on Exit
      // button.
      lPosY = lPosY + lSize_Y + 2;
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lPosY );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lPosX );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialog, "Action", "DialogName", vNewDialog, "Dialog", "Tag" );
      SetAttributeFromString( vNewDialog, "Action", "WindowName", "MainListQual" );

      // Create Postbuild window event and tie to Action just created.
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  2 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct", vNewDialog, "Action", zPOS_AFTER );
      DropView( vListQualWnd );
   }
   else
   {
      // Create window events PreBuild and ReturnFromSubwindow to execute
      // operation ActivateGroupForList.
      CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
      SetAttributeFromString ( vNewDialog, "Action", "Tag",   "ActivateList" );
      SetAttributeFromString ( vNewDialog, "Action", "NoMap", "Y" );
      SetAttributeFromInteger( vNewDialog, "Action", "Type",  1 );
      ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "ActivateMainList", "", vNewDialog, "ActOper" );

      // PreBuild
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  1 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct",
                                    vNewDialog, "Action", zPOS_AFTER );
      // Refresh
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  8 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct", vNewDialog, "Action", zPOS_AFTER );
   }

   // Create SystemClose event and tie it to Exit Action.
   SetCursorFirstEntityByString( vNewDialog, "Action", "Tag", "Alt-F4", 0 );
   CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  256 );
   IncludeSubobjectFromSubobject( vNewDialog, "WndAct", vNewDialog, "Action", zPOS_AFTER );

   // Set the window size.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 31 );

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   // Create the empty shell for the next window.
   CreateMetaEntity( vSubtask, vNewDialog, "Window", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DuplicateButton
//
// PURPOSE: This routine copies a Button from one view to another.
//          Everything is duplicated.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_DuplicateButton( zVIEW    vSubtask,
                           zVIEW    vTargetView,
                           zVIEW    vSourceView )
{
   CreateMetaEntity( vSubtask, vTargetView, "Control", zPOS_AFTER );
   SetMatchingAttributesByName( vTargetView, "Control", vSourceView, "Control", zSET_NULL );
   IncludeSubobjectFromSubobject( vTargetView, "ControlDef", vSourceView, "ControlDef", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vTargetView, "Event", zPOS_AFTER );
   SetMatchingAttributesByName( vTargetView, "Event", vSourceView, "Event", zSET_NULL );
   IncludeSubobjectFromSubobject( vTargetView, "EventAct", vSourceView, "EventAct", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase1
//
// PURPOSE: This is the routine that processes each Autodesign Case Type 1
//          UIS_Entity.
//          It is called both externally for the root UIS_Entity and
//          internally, for each recursive UIS_Entity of Case Type 1.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase1( zVIEW  vSubtask,
                           zVIEW  vUserSpec,
                           zVIEW  vUSSubEnt,
                           zVIEW  vNewDialogOrig,
                           zVIEW  vDefaults )
{
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialogRoot;
   zVIEW    vNewDialog;
   zVIEW    vSrcButtonView;
   zVIEW    vObject;
   zVIEW    vUSSubEntCall;
   zVIEW    vPE;
   zSHORT   nRC;
   zCHAR    szCase[ 3 ];
   zCHAR    szParentCase[ 3 ];
   zCHAR    szButtonText[ 21 ];
   zCHAR    szTabCount[ 4 ];
   zCHAR    szMsg[ 110 ];
   zCHAR    szEntityName[ 33 ];
   zLONG    lButtonNbr;
   zLONG    lSize_X;
   zLONG    lSize_Y;
   zLONG    lGroupNbr;
   zLONG    lMaxWindowWidth;
   zLONG    lWkSize;
   zLONG    lWkPos;
   zLONG    lTabCnt;
   zLONG    lNextPos = 5;
   zLONG    lSubGroupPosY;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );

   // Create a new vNewDialog view so as to not mess up the callers vDialog.
   CreateViewFromViewForTask( &vNewDialog, vNewDialogOrig, 0 );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignWDOD", "Y" );
   GetStringFromAttribute( szCase, zsizeof( szCase ), vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );

// if ( szCase[ 0 ] == '1' )
   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUSSubEnt, vNewDialog, vAutoDesWk );

   // Initialize the Counter to make control tags unique.
// SetAttributeFromInteger( vNewDialog, "Window", "ControlTagCounter", 0 );

   // Make sure DialogRoot is on the main window itself.
   CreateViewFromViewForTask( &vNewDialogRoot, vNewDialog, 0 );

   // Tab Control Processing
   strcpy_s( szParentCase, zsizeof( szParentCase ), szCase );
   if ( szCase[ 1 ] == 'F' || szCase[ 1 ] == 'G' )
   {
      // If the Case for this window is 1F or 1G (which means each Groupbox
      // should be placed within a Tab Control), add the main Tab Control here.
      // Each sub-Tab-Control will be added under this one inside the loop
      // below or the loop that processes subentities.
      GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
      CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
      SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Tab", "" );
      IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
      lTabCnt = 0;
      SetAttributeFromString( vNewDialog, "Control", "Tag",  "Tab1" );
      SetAttributeFromString( vNewDialog, "Control", "Text", "Tab1" );
      SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 1 );
      SetViewToSubobject( vNewDialog, "CtrlCtrl" );

      // Loop through each Detail window set and create a Tab control, with
      // subcontrols for each.
      nRC = SetCursorFirstEntity( vUSSubEnt, "UIS_Tab", 0 );
      if ( nRC < zCURSOR_SET )
      {
         // Currently, it is an error if there is no UIS_Tab entry.
         GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "ER_Entity", "Name" );
         strcpy_s( szMsg, zsizeof( szMsg ), "A Tab dialog is requested but no Main Tab is defined for Entity: " );
         strcat_s( szMsg, zsizeof( szMsg ), szEntityName );
         strcat_s( szMsg, zsizeof( szMsg ), "." );
         MessageSend( vSubtask, "AD10214", "Autodesigner",
                      szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         DropView( vNewDialog );
         return( -1 );
      }
      while ( nRC >= zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
         SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "TabCtl", "" );
         IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vPE, "ControlDef", zPOS_AFTER );
         lTabCnt++;
         zltoa( lTabCnt, szTabCount, zsizeof( szTabCount ) );
         strcpy_s( szButtonText, zsizeof( szButtonText ), "TabCtl" );
         strcat_s( szButtonText, zsizeof( szButtonText ), szTabCount );
         SetAttributeFromString( vNewDialog, "Control", "Tag",  szButtonText );
         if ( CompareAttributeToString( vUSSubEnt, "UIS_Tab", "Title", "" ) != 0 )
            SetAttributeFromAttribute( vNewDialog, "Control", "Text", vUSSubEnt,  "UIS_Tab", "Title" );
         else
            SetAttributeFromAttribute( vNewDialog, "Control", "Text", vUSSubEnt,  "LOD_Entity", "Name" );
         SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 1 );

         // dks ... set tabctl position to 1 and 11
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 1 );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 11 );

         // Create a group control to hold the Tab Detail attributes.
         SetViewToSubobject( vNewDialog, "CtrlCtrl" );
         nRC = oTZWDLGSO_AutoDesignGroupboxMT( vSubtask, vUSSubEnt,
                                               vNewDialog, vDefaults,
                                               vAutoDesWk, "R", szTabCount );

         nRC = SetCursorNextEntity( vUSSubEnt, "UIS_Tab", 0 );

         // Set cursor back up to Tab if there are more Tab detail groups.
         if ( nRC >= zCURSOR_SET )
            ResetViewFromSubobject( vNewDialog );
      }
      nRC = 0;

      // Note that position is on Groupbox of the last TabCtl.  The buttons
      // will be placed here so we must save a view to this position for
      // moving the Tabs later.
      CreateViewFromViewForTask( &vSrcButtonView, vNewDialog, 0 );
   }
   else
   {
      // Create a group control to hold the non-Tab Detail attributes.
      // We will actually loop to create multiple group controls in case
      // there are too many attributes to fit on a single group.
      // Start painting at the first M_LOD_Attribute.
      SetCursorFirstEntity( vUSSubEnt, "M_LOD_Attribute", 0 );
      nRC = 1;
      while ( nRC > 0 )
      {
         nRC = oTZWDLGSO_AutoDesignGroupboxM( vSubtask, vUSSubEnt, vNewDialog,
                                             vDefaults, vAutoDesWk, "R" );
      }
   }

   // Add Case 6 combo boxes for Case 6 entities.
   SetViewToSubobject( vUSSubEnt, "UIS_ChildEntity" );
   oTZADCSDO_AutoDesignCase5Attrs( vSubtask, vUserSpec, vUSSubEnt, vNewDialog,
                                   vDefaults, vObject, "6" );
   ResetViewFromSubobject( vUSSubEnt );

   if ( nRC < 0 )
   {
      DropView( vNewDialog );
      return( -1 );
   }
   lButtonNbr = 0;

   // Make sure we are positioned on the first Group.
   SetCursorFirstEntity( vNewDialog, "Control", 0 );

   // Set Window Caption.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "WndCaption", "" ) != 0 )
      SetAttributeFromAttribute( vNewDialogRoot, "Window",    "Caption",
                                 vUSSubEnt,      "WndDesign", "WndCaption" );
   else
      SetAttributeFromAttribute( vNewDialogRoot, "Window",     "Caption",
                                 vUSSubEnt,      "LOD_Entity", "Name" );

   // Add the buttons, depending on whether this is for the root UIS_Entity
   // or not.
   if ( CompareAttributeToString( vUSSubEnt, "UIS_Entity", "RootIndicator", "Y" ) == 0 )
   {
      // The buttons are for a root UIS_Entity.

      ////////
      //OK
      ////////

      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "OK" );

      // If this is a qualified list, the OK function transfers to the
      // MainListQual window. Otherwise it returns to parent.

      if ( CheckExistenceOfEntity( vUSSubEnt, "WndDesign" ) >= zCURSOR_SET &&
           CompareAttributeToString( vUSSubEnt, "WndDesign", "ListQualFlag", "Y" ) == 0 )
      {
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, "Alt-F4",
                                        "SaveInstance", lButtonNbr, "S",
                                        zWAB_ReplaceWindowWithModalWindow );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "SaveInstance" );

         // Set the Dialog and Window names in the Action.
         SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                                    vNewDialogRoot, "Dialog", "Tag" );
         SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", "MainListQual" );
      }
      else
      {
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, "Alt-F4",
                                        "SaveInstance", lButtonNbr, "S",
                                        zWAB_ReturnToParentWithRefresh );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "SaveInstance" );
      }

      ////////
      //Cancel
      ////////

      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Cancel" );
      oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                     vNewDialogRoot, vDefaults,
                                     szButtonText, "ESC",
                                     "DeleteInstance", lButtonNbr, "S",
                                     zWAB_ReturnToParentWithRefresh );
      ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "DeleteInstance" );

      // Also set NoMap for Cancel.
      SetAttributeFromString( vNewDialogRoot, "Action", "NoMap", "Y" );

      ////////
      // Optional New
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateCreateAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "New" );
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, 0,
                                        "ProcessNewOI", lButtonNbr, "S",
                                        zWAB_StayOnWindowWithRefresh );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "CreateNewInstance" );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "ProcessNewOI" );
      }

      ////////
      // Optional Delete
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateDeleteAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Delete" );
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, 0,
                                        "DeleteObjectEntity", lButtonNbr, "S",
                                        zWAB_ReturnToParentWithRefresh );

         // Create the DeleteObjectEntity operation in source code.
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "DeleteObjectEntity" );
      }

      ////////
      // Optional Next
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateNextAction", "Y" ) == 0 &&
           CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "WDOD_Style", "L" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Next" );
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, 0,
                                        "ProcessNextOI", lButtonNbr, "S",
                                        zWAB_StayOnWindowWithRefresh );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "ProcessNextOI" );

         // Also add the Disable Next button "Object Instance" funtionality.
         SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 2 );   // Disables on last entity
         CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity", vUSSubEnt,  "LOD_Entity", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView", vUSSubEnt,  "ListVOR", zPOS_AFTER );
      }

      ////////
      // Optional Previous
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdatePrevAction", "Y" ) == 0 &&
           CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "WDOD_Style", "L" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Previous" );
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog,
                                        vNewDialogRoot, vDefaults,
                                        szButtonText, 0,
                                        "ProcessPreviousOI", lButtonNbr, "S",
                                        zWAB_StayOnWindowWithRefresh );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "ProcessPreviousOI" );

         // Also add the Disable Previous button "Object Instance" funtionality.
         SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 1 );   // Disables on first entity
         CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity", vUSSubEnt,  "LOD_Entity", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView", vUSSubEnt,  "ListVOR", zPOS_AFTER );
      }
   }
   else
   {
      // The buttons are for a subordinate UIS_Entity.

      ////////
      // OK
      ////////

      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "OK" );
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                       vNewDialogRoot, vDefaults, vObject,
                                       szButtonText, "Alt-F4", lButtonNbr, "S",
                                       zWAB_ReturnToParentWithRefresh,
                                       zSubActionAccept, "" );

      ////////
      // Cancel
      ////////

      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Cancel" );
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                       vNewDialogRoot, vDefaults, vObject,
                                       szButtonText, "ESC", lButtonNbr, "S",
                                       zWAB_ReturnToParentWithRefresh,
                                       zSubActionCancel, "" );
      // Also set NoMap for Cancel.
      SetAttributeFromString( vNewDialogRoot, "Action", "NoMap", "Y" );

      ////////
      // Optional New
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateCreateAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "New" );
         // 33 is CreateNewTemporalEntityAfterAccept
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                          vNewDialogRoot, vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "S",
                                          zWAB_StayOnWindowWithRefresh, 33, "" );
      }

      ////////
      // Optional Delete
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateDeleteAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Delete" );
         // 268435460 is Delete with confirmation
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                          vNewDialogRoot, vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "S",
                                          zWAB_ReturnToParentWithRefresh,
                                       // zSubActionDelete, "" );
                                          268435460, "" );
      }

      ////////
      // Optional Next
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdateNextAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Next" );
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                          vNewDialogRoot, vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "S",
                                          zWAB_StayOnWindowWithRefresh,
                                          zSubActionAccept, "Next" );
      }

      ////////
      // Optional Previous
      ////////

      if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UpdatePrevAction", "Y" ) == 0 )
      {
         lButtonNbr++;
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Previous" );
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog,
                                          vNewDialogRoot, vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "S",
                                          zWAB_StayOnWindowWithRefresh,
                                          zSubActionAccept, "Previous" );
      }
   }

   // Recalculate the Groupbox size based on the buttons just added and
   // set it.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );

   // Reset the view back to the top or the TabCtl entity.  Then, in case
   // we are processing a Tab Control, position on last TabCtl entity.
   // (It will be on Groupbox when not a Tab.)
   ResetViewFromSubobject( vNewDialog );
   SetCursorLastEntity( vNewDialog, "Control", 0 );

   // Position a handle to the subordinate entities of the current entity (if there are any).
   SetViewToSubobject( vUSSubEnt, "UIS_ChildEntity" );

   // Initialize GroupNbr to count each repeating group, not including the main group.

   // Set the Y position for all subordinate groups.  It is the last group Y position + its size + 5.
   GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
   lSubGroupPosY = lWkPos + lWkSize + 5;

   // Loop through the subordinate entities and process each Autodesign case as necessary.
   nRC = SetCursorFirstEntity( vUSSubEnt, "UIS_Entity", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Increment GroupNbr for generating unique Groupbox and Button Tags.

      //////////////////////////////////////////////////////////////
      // Build a Groupbox on this window and a subordinate window
      // as necessary for each sub entity.
      //
      // If the Case for this window is 1F or 1G (which means each Groupbox
      // should be placed within a Tab Control), add the Tab Control as
      // a parent of each Groupbox control.
      //
      // The processing depends on the various Case Types: 1, 2, 3, 4
      ///////////////////////////////////////////////////////////////

      GetStringFromAttribute( szCase, zsizeof( szCase ), vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );

      // Add Tab Control if necessary.
      if ( (szParentCase[ 1 ] == 'F' || szParentCase[ 1 ] == 'G') &&
           (szCase[ 0 ] == '1' || szCase[ 0 ] == '2' ||
            szCase[ 0 ] == '3' || szCase[ 0 ] == '4') )
      {
         CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
         SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "TabCtl", "" );
         IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                        vPE,        "ControlDef", zPOS_AFTER );
         lTabCnt++;
         zltoa( lTabCnt, szTabCount, zsizeof( szTabCount ) );
         strcpy_s( szButtonText, zsizeof( szButtonText ), "TabCtl" );
         strcat_s( szButtonText, zsizeof( szButtonText ), szTabCount );
         SetAttributeFromString( vNewDialog, "Control", "Tag",  szButtonText );

         // We will initially set the Tab Text to the name of the entity.
         // Later, we will reset it with the Groupbox Text, if there is any.
         SetAttributeFromAttribute( vNewDialog, "Control",    "Text",
                                    vUSSubEnt,  "LOD_Entity", "Name" );

         SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 1 );

         // dks ... set tabctl position to 1 and 11
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 1 );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 11 );

         SetViewToSubobject( vNewDialog, "CtrlCtrl" );
      }

      // Use a temporary US_Spec view for calling subroutines, in case
      // they modify cursor position.
      CreateViewFromViewForTask( &vUSSubEntCall, vUSSubEnt, 0 );

      if ( szCase[ 0 ] == '1' )
      {
         ////////
         // Case 1 List
         ////////

         oTZADCSDO_AutoDesignCase1List( vSubtask, vUserSpec, vUSSubEntCall, vNewDialog,
                                        vNewDialogRoot, vDefaults, vObject, szCase );
      }
      else
      if ( szCase[ 0 ] == '2' )
      {
         ////////
         // Case 2  (It is driven by the 2Group routine.)
         ////////

         oTZADCSDO_AutoDesignCase2Group( vSubtask, vUserSpec, vUSSubEntCall, vNewDialog,
                                         vNewDialogRoot, vDefaults, vObject, szCase );
      }
      else
      if ( szCase[ 0 ] == '3' )
      {
         ////////
         // Case 3 - Reuse Case4Group
         ////////

         oTZADCSDO_AutoDesignCase4Group( vSubtask, vUserSpec, vUSSubEntCall, vNewDialog,
                                         vNewDialogRoot, vDefaults, vObject, szCase );
      }
      else
      // At this point, Case 4 is encountered only for the Assoc. entity.
      // We must go down one level to the child to actually process the
      // Case 4 routine.
      if ( szCase[ 0 ] == '4' &&
           CompareAttributeToString( vUSSubEnt, "ER_Entity", "Purpose", "S" ) == 0 )
      {
         // If this is the Assoc. entity, continue the subobject loop.
         oTZADCSDO_AutoDesignAssocDep( vSubtask, vUserSpec, vUSSubEntCall, vNewDialog,
                                       vNewDialogRoot, vDefaults, vObject, szCase );
      }

      // If the Parent case is Tab Control, we need to go back up a level
      // to the Tab Control for the Dialog view.
      if ( (szParentCase[ 1 ] == 'F' ||
            szParentCase[ 1 ] == 'G' ) &&
           (szCase[ 0 ] == '1' || szCase[ 0 ] == '2' ||
            szCase[ 0 ] == '3' || szCase[ 0 ] == '4') )
      {
         ResetViewFromSubobject( vNewDialog );
      }

      DropView( vUSSubEntCall );

      // Set the X position as incremented for each group and the Y position
      // as a constant computed earlier, if the Case was not "None" or "6".
      // For Case none and 6, no group was created above.
      if ( szCase[ 0 ] != 'N' && szCase[ 0 ] != '6' )
      {
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lNextPos );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lSubGroupPosY );
         GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_X" );
         lNextPos = lNextPos + lWkSize + 5;
      }

      nRC = SetCursorNextEntity( vUSSubEnt, "UIS_Entity", "" );
   }

   // If we are processing Case 1F or 1G for Tab Control, we must finish
   // off the control as follows.
   // 1. For Case 1F and 1G, move the buttons from under the Group entity to
   //    the main Window.
   //    Then subtract the button X size from that Group X size to get
   //    the new size for the Group, without the buttons.
   // 2. Loop through all the Groups and determine the max X,Y dimensions that
   //    we'll need in sizing the Tab Control.  Set the size in the Tab entity.
   // 3. For Case 1F, loop through all the TabCtl entities and move the
   //    buttons off the window and under each TabCtl entity, at the same
   //    position for all TabCtls.  Reset the X size of the Tab entity
   //    accordingly.
   // 4. For Case 1G, set the X position of the buttons based on the size
   //    and position of the Tab control.
   // 5. Set Blob info (number of tabs) in the Tab Control.
   // 6. Loop through all TabCtl entities and set their size from the Tab
   //    entity minus 2.

   if ( szParentCase[ 1 ] == 'F' ||
        szParentCase[ 1 ] == 'G' )
   {
      zCHAR   szBlob[ 3 * sizeof( long ) ];
      zCHAR   szButtonTag[ 37 ];
      zCHAR   szButtonSuffix[ 4 ];
      zPLONG lpTabsPerRow;
      zVIEW   vTgtButtonView;
      zLONG   lMaxSizeX;
      zLONG   lMaxSizeY;
      zLONG   lSizeX;
      zLONG   lSizeY;
      zLONG   lButtolSizeY;
      zLONG   lGroupSizeY;
      zLONG   lTabIndex;
      zLONG   lButtolPosX;

      // Note that the current position of vNewDialog is on the last
      // TabCtl entity.

      if ( szParentCase[ 1 ] == 'F' || szParentCase[ 1 ] == 'G' )
      {
         // 1. Move the buttons from the first Group to the window.
         //
         // Note that the vSrcButtonView has already been positioned on the
         // Groupbox that contains the buttons.
         SetCursorFirstEntity( vNewDialog, "Control", "" ); // On first TabCtl
         CreateViewFromViewForTask( &vTgtButtonView, vNewDialog, 0 );
         ResetViewFromSubobject( vTgtButtonView );          // View on Tab

         SetViewToSubobject( vSrcButtonView, "CtrlCtrl" );  // View is on Subctl
         nRC = SetCursorFirstEntity( vSrcButtonView, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            if ( CompareAttributeToString( vSrcButtonView, "ControlDef", "Tag", "PushBtn" ) == 0 )
            {
               GetIntegerFromAttribute( &lButtolSizeY, vSrcButtonView, "Control", "SZDLG_X" );
               oTZADCSDO_DuplicateButton( vSubtask, vTgtButtonView, vSrcButtonView );
               DeleteEntity( vSrcButtonView, "Control", zREPOS_NONE );
            }

            nRC = SetCursorNextEntity( vSrcButtonView, "Control", "" );
         }

         // Reset Group Size X.
         ResetViewFromSubobject( vSrcButtonView );      // View back to Group
         GetIntegerFromAttribute( &lGroupSizeY, vSrcButtonView, "Control", "SZDLG_X" );
         SetAttributeFromInteger( vSrcButtonView, "Control", "SZDLG_X", lGroupSizeY - lButtolSizeY - 5 );
         DropView( vTgtButtonView );
         DropView( vSrcButtonView );
      }

      // 2. Loop through all the Groups and determine the max X,Y dimensions
      //    that we'll need in sizing the Tab Control.
      //    Also set the Group X,Y positions to 5,5.
      //    Move Text from Groupbox control entity to TabCtl control entity.
      lMaxSizeX = 0;
      lMaxSizeY = 0;
      nRC = SetCursorFirstEntity( vNewDialog, "Control", "" );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CheckExistenceOfEntity( vNewDialog, "CtrlCtrl" ) >= zCURSOR_SET )
         {
            // Process Size and Position
            SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "PSDLG_X", 5 );
            SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "PSDLG_Y", 5 );
            GetIntegerFromAttribute( &lSizeX, vNewDialog, "CtrlCtrl", "SZDLG_X" );
            if ( lMaxSizeX < lSizeX )
               lMaxSizeX = lSizeX;
            GetIntegerFromAttribute( &lSizeY, vNewDialog, "CtrlCtrl", "SZDLG_Y" );
            if ( lMaxSizeY < lSizeY )
               lMaxSizeY = lSizeY;

            // Move Groupbox control entity Text.
            if ( CompareAttributeToString( vNewDialog, "CtrlCtrl", "Text", "" ) != 0 )
            {
               SetAttributeFromAttribute( vNewDialog, "Control",  "Text", vNewDialog, "CtrlCtrl", "Text" );
               SetAttributeFromString( vNewDialog, "CtrlCtrl", "Text", "" );
            }
         }

         nRC = SetCursorNextEntity( vNewDialog, "Control", "" );
      }
      ResetViewFromSubobject( vNewDialog );          // Back up to Tab
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxSizeX + 10 );
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lMaxSizeY + 20 );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 5 );

      // 3. If this is Case 1F, we will move the buttons to all the TabCtl
      // entities here, and then increase the Tab SizeX to hold the buttons.
      // The position of the buttons will be the Tab SizeX value just
      // computed.
      // The first loop duplicates the buttons on each TabCtl.
      // The second loop deletes the buttons from under the Window.
      if ( szParentCase[ 1 ] == 'F' )
      {
         CreateViewFromViewForTask( &vTgtButtonView, vNewDialog, 0 );
         CreateViewFromViewForTask( &vSrcButtonView, vNewDialog, 0 );
         ResetViewFromSubobject( vSrcButtonView );          // View on Window

         SetViewToSubobject( vTgtButtonView, "CtrlCtrl" );  // View on TabCtl
         // FOR EACH TabCtl
         lTabIndex = 0;
         nRC = SetCursorFirstEntity( vTgtButtonView, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            lTabIndex++;
            zltoa( lTabIndex, szButtonSuffix, zsizeof( szButtonSuffix ) );
            SetViewToSubobject( vTgtButtonView, "CtrlCtrl" );  // View on Group
            // FOR EACH Button entity
            nRC = SetCursorFirstEntity( vSrcButtonView, "Control", "" );
            while ( nRC >= zCURSOR_SET )
            {
               if ( CompareAttributeToString( vSrcButtonView, "ControlDef", "Tag", "PushBtn" ) == 0 )
               {
                  oTZADCSDO_DuplicateButton( vSubtask, vTgtButtonView, vSrcButtonView );
                  SetAttributeFromInteger( vTgtButtonView, "Control", "PSDLG_X", lMaxSizeX + 10 );
                  // We must add a suffix to make the Button Tags unique.
                  // Make sure the Tag is max 29 chars to make room for the suffix.
                  GetStringFromAttribute( szButtonTag, zsizeof( szButtonTag ), vTgtButtonView, "Control", "Tag" );
                  szButtonTag[ 29 ] = 0;
                  strcat_s( szButtonTag, zsizeof( szButtonTag ), szButtonSuffix );
                  SetAttributeFromString( vTgtButtonView, "Control", "Tag", szButtonTag );
               }

               nRC = SetCursorNextEntity( vSrcButtonView, "Control", "" );
            }

            ResetViewFromSubobject( vTgtButtonView );   // Back up to TabCtl
            nRC = SetCursorNextEntity( vTgtButtonView, "Control", "" );
         }
         // Loop to delete the buttons from under the Window.
         nRC = SetCursorFirstEntity( vSrcButtonView, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            if ( CompareAttributeToString( vSrcButtonView, "ControlDef", "Tag", "PushBtn" ) == 0 )
            {
               DeleteEntity( vSrcButtonView, "Control", zREPOS_NONE );
            }

            nRC = SetCursorNextEntity( vSrcButtonView, "Control", "" );
         }

         // Reset the Tab SizeX.
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lMaxSizeX + lButtolSizeY + 15 );
         DropView( vTgtButtonView );
         DropView( vSrcButtonView );
      }

      // 4. For Case 1G, set the X position of the buttons based on the size
      //    and position of the Tab control.
      if ( szParentCase[ 1 ] == 'G' )
      {
         // Button Pos X is Tab start + Tab size + 5.
         // We're positioned on the Tab control.
         GetIntegerFromAttribute( &lButtolPosX, vNewDialog, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &lSizeX, vNewDialog, "Control", "SZDLG_X" );
         lButtolPosX = lButtolPosX + lSizeX + 5;

         // FOR EACH Button
         CreateViewFromViewForTask( &vSrcButtonView, vNewDialog, 0 );
         nRC = SetCursorFirstEntity( vSrcButtonView, "Control", "" );
         while ( nRC >= zCURSOR_SET )
         {
            if ( CompareAttributeToString( vSrcButtonView, "ControlDef", "Tag", "PushBtn" ) == 0 )
            {
               SetAttributeFromInteger( vSrcButtonView, "Control", "PSDLG_X", lButtolPosX );
            }
            nRC = SetCursorNextEntity( vSrcButtonView, "Control", "" );
         }

         DropView( vSrcButtonView );
      }

//    dks ... get tab sizes and subtract 11 from height to get tabctl sizes

      // 5. Set Blob info (number of tabs) in the Tab Control.
      lpTabsPerRow = (zPLONG) (szBlob + sizeof( long ) * 2);
      *lpTabsPerRow = lTabCnt;
      SetAttributeFromBlob( vNewDialog, "Control", "CtrlBOI", szBlob, 3 * sizeof( long ) );

      // 6. For each TabCtl entity, set x and y sizes from Tab entity sizes minus 2.
      GetIntegerFromAttribute( &lSize_X, vNewDialog, "Control", "SZDLG_X" );
      GetIntegerFromAttribute( &lSize_Y, vNewDialog, "Control", "SZDLG_Y" );
      lSize_X = lSize_X - 2;
      lSize_Y = lSize_Y - 2;
      nRC = SetCursorFirstEntity( vNewDialog, "CtrlCtrl", "" );
      while ( nRC >= zCURSOR_SET )
      {
         SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "SZDLG_X", lSize_X );
         SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "SZDLG_Y", lSize_Y );

         nRC = SetCursorNextEntity( vNewDialog, "CtrlCtrl", "" );
      }

      ResetViewFromSubobject( vNewDialog );    // Back to Window
   }

   ResetViewFromSubobject( vUSSubEnt );

   // Set the window size.  We will first compute the window size and compare
   // it to the max window size.  If it is too large, then we will modify the
   // size and position of each Groupbox to fit within the max size.
   // However, we only only modifiy the positions of Groupboxes  that start
   // greater than x position 10.  Any Groupboxes starting prior to that
   // will be consider part of those that are positioned vertically and
   // shouldn't be a part of this calculation.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   GetIntegerFromAttribute( &lMaxWindowWidth, vDefaults, "InitValues", "WindowWidth" );
   if ( lMaxWindowWidth == 0 )
      lMaxWindowWidth = 400;
   if ( lSize_X > lMaxWindowWidth )
   {
      lGroupNbr = oTZWDLGSO_GetNbrOfGroupboxes( vNewDialog );
      if ( lGroupNbr < 2 )
         lGroupNbr = 2;
      lWkSize = lMaxWindowWidth / (lGroupNbr - 1 );
      lSize_X = lMaxWindowWidth;

      lWkPos = 5;
      lWkSize = lWkSize - 3;

      // Skip to first Groupbox starting after x position 10.
      nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         if ( CompareAttributeToInteger( vNewDialog, "Control", "PSDLG_X", 10 ) > 0 )
            break;
         nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
      }

      lWkPos = lWkPos + lWkSize;
      while ( nRC >= zCURSOR_SET )
      {
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lWkPos );
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lWkSize );
         lWkPos = lWkPos + lWkSize;

         nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
      }
   }

   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 31 );

   // Assign tabbing order to window.
   oTZWDLGSO_AssignTabbingOrder( vNewDialog );

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   DropView( vNewDialog );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoDsgnWndAndSubs( zVIEW vSubtask,
                                zVIEW vUserSpec,
                                zVIEW vNewDialog,
                                zVIEW vDefaults,
                                zVIEW vAutoDesWk )
{
   zVIEW  vUSSubEnt;
   zSHORT nRC;

   // Set up the recursive view for UserSpec.
   CreateViewFromViewForTask( &vUSSubEnt, vUserSpec, 0 );
   SetNameForView( vUSSubEnt, "USSubEnt", vSubtask, zLEVEL_TASK );
   SetNameForView( vNewDialog, "***NewDialog", vSubtask, zLEVEL_TASK );

   nRC = oTZADCSDO_AutoDesignCase1( vSubtask, vUserSpec, vUSSubEnt, vNewDialog, vDefaults );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    LoadStyleInformation
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_LoadStyleInfo( zVIEW vSubtask, zVIEW vUserSpec,
                           zVIEW vNewDialog )
{
   zVIEW    vAutoDesWk;
   zVIEW    vPainterObj;
   zVIEW    vObjectObj;
   zVIEW    vFileObj;
   zSHORT   Level;
   zLONG    bKeepLooping;
   zCHAR    szEntityName[ 33 ];
   zCHAR    szWindowName[ 33 ];
   zCHAR    szWorkString[ 256 ];
   zCHAR    szStyleName[ 33 ];
   zSHORT   nRC;
   zSHORT   nZRetCode;

// TraceLineS("IN", "LoadStyleInfo" );
   GetViewByName( &vPainterObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   nZRetCode = GetStringFromAttribute( szStyleName, zsizeof( szStyleName ), vPainterObj, "Palette", "UIS_Name" );
   if ( szStyleName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "AD10201", "Autodesigner",
                   "No Dialog Spec Name selected!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   if ( vUserSpec > 0 )
      DropObjectInstance( vUserSpec );

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_UIS_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef", "Name", szStyleName, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "The user spec definition - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szStyleName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " does not exist!" );
      MessageSend( vSubtask, "AD10202", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // If the user only has a reference view to this UIS then nothing
   // can be added to it(default attributes etc...) so create a new
   // UIS from the old.
   nRC = ActivateMetaOI( vSubtask, &vUserSpec, vFileObj, zSOURCE_UIS_META, zSINGLE );
   if ( nRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on dialog spec - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szStyleName );
      MessageSend( vSubtask, "AD10203", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szStyleName, zsizeof( szStyleName ), vUserSpec, "UIS_LOD", "Name" );
   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_LOD_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                       "Name", szStyleName, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "The object definition - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szStyleName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " does not exist!" );
      MessageSend( vSubtask, "AD10204", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   nRC = ActivateMetaOI( vSubtask, &vObjectObj, vFileObj, zREFER_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   if ( nRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on Object - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szStyleName );
      MessageSend( vSubtask, "AD10205", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   SetNameForView( vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vAutoDesWk, "AutoDesignWork", "ObjectEntityName" );
   if ( zstrcmp( "", szWindowName ) == 0 )
   {
      GetStringFromAttribute( szWindowName, zsizeof( szWindowName ), vNewDialog, "Window", "Tag" );
   }

   ResetView( vUserSpec );
   nRC = ofnTZWDLGSO_AddStyleEssentials( vSubtask, vUserSpec );
   if ( nRC == -1 )
   {
      return( nRC );
   }

   ResetView( vUserSpec );
   strcat_s( szEntityName, zsizeof( szEntityName ), "" );
   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "AutoDesignWDOD", "Y" ) == 0 )
   {
      SetCursorNextEntityHierarchical( (zPUSHORT) &Level, szEntityName, vUserSpec );
      DropHierarchicalCursor( vUserSpec );
      return( 0 );
   }
   nZRetCode = SetCursorNextEntityHierarchical((zPUSHORT) &Level, szEntityName, vUserSpec );
   bKeepLooping = 1;
   /* See if there is an entity name for this window.  This is in case
      the window name does not correspond with the object entity name. */
   while ( nZRetCode > zCURSOR_UNCHANGED && bKeepLooping == 1 )
   {
      if ( nZRetCode == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }

      if ( zstrcmp ( "UIS_Entity", szEntityName ) == 0 ||
           zstrcmp( "UIS_ChildEntity", szEntityName ) == 0 )
      {
         GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "LOD_Entity", "Name" );
         if ( zstrcmp( szWindowName, szWorkString ) == 0 )
         {
            bKeepLooping = 0;
         }
         else
            nZRetCode = SetCursorNextEntityHierarchical( (zPUSHORT) &Level, szEntityName, vUserSpec );
      }
      else
         nZRetCode = SetCursorNextEntityHierarchical( (zPUSHORT) &Level, szEntityName, vUserSpec );

   }
   if ( bKeepLooping == 1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "There is no entity in the user spec with the name " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szWindowName );
      strcat_s( szWorkString, zsizeof( szWorkString ), ". Enter an existing name." );
      MessageSend( vSubtask, "AD10206", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropHierarchicalCursor( vUserSpec );
      return( -1 );
   }
   DropHierarchicalCursor( vUserSpec );

// TraceLineS("END", "LoadStyleInfo" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddStyleEssentials
//
// PURPOSE: This function checks the style object to see if any
//          ENTITIES are missing the WndDesign Entity or if
//          any ENTITIES are missing MAINTATTRIB and LISTATTRIB.
//          If an ENTITY does not have a WndDesign entity, then
//          one is created from the DfltWndDesign entity.
//          If an ENTITY is missing MAINTATTRIBs then MAINTATTRIBs are
//          created from the object definition ATTRIBs.  The changes
//          made to the style object are not saved.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddStyleEssentials( zVIEW vSubtask, zVIEW vUserSpec )
{
   zVIEW    vTempSpec;
   zVIEW    vObjectObj;
   zCHAR    szEntityName[ 33 ];
   zSHORT   Level;
   zSHORT   nZRetCode;

// TraceLineS("IN", "AddStyleEssentials W1" );

   CreateViewFromViewForTask( &vTempSpec, vUserSpec, 0 );
   GetViewByName( &vObjectObj, "LOD_Object", vUserSpec, zLEVEL_TASK );
   ResetView( vUserSpec );
   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   nZRetCode = SetCursorNextEntityHierarchical( (zPUSHORT) &Level, szEntityName, vUserSpec );
   while ( nZRetCode != zCURSOR_UNCHANGED )
   {
      if ( nZRetCode == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }
#if 0
      if ( zstrcmp ( "UIS_Entity", szEntityName ) == 0 ||
           zstrcmp( "UIS_ChildEntity", szEntityName ) == 0 )
      {
         GetIntegerFromAttribute( &lZKey, vUserSpec, "LOD_Entity", "ZKey" );
         nZRetCode = SetCursorFirstEntityByInteger( vObjectObj, "LOD_Entity",
                                                    "ZKey", lZKey, "LOD" );

         if ( CompareAttributeToString( vUserSpec, "UIS_Entity",
                                        "EliminateWnd", "Y" ) != 0 )
         {
            if ( CheckExistenceOfEntity ( vUserSpec, "WndDesign" ) != 0 )
            {
               ofnTZWDLGSO_CreateWndDesign( vUserSpec );
            }
            if ( CheckExistenceOfEntity ( vUserSpec, "M_LOD_Attribute" ) != 0 )
            {
               nRC = ofnTZWDLGSO_CreateMaintAttribs( vUserSpec );
               if ( nRC == -1 )
               {
                  DropHierarchicalCursor( vUserSpec );
                  return( -1 );
               }
            }
            if ( CheckExistenceOfEntity ( vUserSpec, "L_LOD_Attribute" ) != 0 )
            {
               nRC = ofnTZWDLGSO_CreateListAttribs( vUserSpec );
               if ( nRC == -1 )
               {
                  DropHierarchicalCursor( vUserSpec );
                  return( -1 );
               }
            }
         }
      }
#endif
      nZRetCode = SetCursorNextEntityHierarchical( (zPUSHORT) &Level, szEntityName, vUserSpec );
   }
   DropHierarchicalCursor( vUserSpec );

// TraceLineS("END", "AddStyleEssentials W1" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateWndDesign
//
// PURPOSE:  This function creates a WndDesign entity from the
//           DfltWndDesign entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateWndDesign( zVIEW vSubtask, zVIEW vUserSpec )
{
   zVIEW vTempSpec;

   CreateViewFromViewForTask( &vTempSpec, vUserSpec, 0 );
   ResetView( vTempSpec );
   if ( CheckExistenceOfEntity ( vTempSpec, "DfltWndDesign" ) != 0 )
   {
      CreateMetaEntity( vSubtask, vTempSpec, "DfltWndDesign", zPOS_AFTER );
   }

   CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );
   SetMatchingAttributesByName( vUserSpec, "WndDesign",
                                vTempSpec, "DfltWndDesign", zSET_ALL );
   DropView( vTempSpec );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateMaintAttributes
//
// PURPOSE:  If the style object does not contain any MAINTATTRIB for
//           the current ENTITY, then this function creates
//           MAINTATTRIBs for the current ENTITY.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateMaintAttribs( zVIEW vSubtask, zVIEW vUserSpec )
{
   zVIEW    vObjectObj;
   zVIEW    vTempStyle;
   zSHORT   lPosition;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &vObjectObj, "LOD_Object", vUserSpec, zLEVEL_TASK );
   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );

   nRC = ResetViewFromSubobject( vTempStyle );
   while ( nRC != 1 )
   {
      GetIntegerFromAttribute( &lZKey, vTempStyle, "LOD_Entity", "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vObjectObj, "LOD_Entity",
                                           "ZKey", lZKey, "LOD" );

      lPosition = zPOS_BEFORE;
      nRC = SetCursorFirstEntity( vTempStyle, "L_LOD_Attribute", "" );
      if ( nRC != zCURSOR_NULL )
      {
         GetIntegerFromAttribute( &lZKey, vTempStyle, "L_LOD_Attribute", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( vObjectObj, "LOD_Attribute", "ZKey", lZKey, "LOD" );

         IncludeSubobjectFromSubobject( vUserSpec, "M_LOD_Attribute", vObjectObj, "LOD_Attribute", lPosition );
         lPosition = zPOS_AFTER;
      }

      nRC = ResetViewFromSubobject( vTempStyle );
   }

   ResetView( vObjectObj );

   SetViewFromView( vTempStyle, vUserSpec );
   DropHierarchicalCursor( vTempStyle );
   nRC = SetCursorFirstEntity( vTempStyle, "LOD_Entity", "" );

   GetIntegerFromAttribute( &lZKey, vTempStyle, "LOD_Entity", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( vObjectObj, "LOD_Entity", "ZKey", lZKey, "LOD" );

   nRC = SetCursorFirstEntity( vObjectObj, "LOD_Attribute", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      IncludeSubobjectFromSubobject( vUserSpec, "M_LOD_Attribute", vObjectObj, "LOD_Attribute", zPOS_AFTER );
      nRC = SetCursorNextEntity( vObjectObj, "LOD_Attribute", "" );
   }

   DropView( vTempStyle );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateListAttributes
//
// PURPOSE:  If the style object does not contain a LISTATTRIB for
//           the current ENTITY, then this function creates a
//           LISTATTRIB.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateListAttribs( zVIEW vSubtask, zVIEW vUserSpec )
{
   zVIEW    vObjectObj;
   zVIEW    vTempStyle;
   zLONG    nRC;
   zLONG    bKeyFound;
   zLONG    lZKey;

   GetViewByName( &vObjectObj, "LOD_Object", vUserSpec, zLEVEL_TASK );
   ResetView( vObjectObj );

   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );

   nRC = SetCursorFirstEntity( vTempStyle, "LOD_Entity", "" );
   GetIntegerFromAttribute( &lZKey, vTempStyle, "LOD_Entity", "ZKey" );
   SetCursorFirstEntityByInteger( vObjectObj, "LOD_Entity",
                                  "ZKey", lZKey, "LOD" );

   bKeyFound = 0;
   nRC = SetCursorFirstEntity( vObjectObj, "LOD_Attribute", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( vObjectObj, "LOD_Attribute", "ParticipatesInKey", "Y" ) == 0 )
      {
         IncludeSubobjectFromSubobject( vUserSpec, "L_LOD_Attribute", vObjectObj, "LOD_Attribute", zPOS_AFTER );
         bKeyFound = 1;
      }

      nRC = SetCursorNextEntity( vObjectObj, "LOD_Attribute", "" );
   }

   /* If no key was found, then just make the first attribute the list
      attribute(if one exists). */
   if ( bKeyFound == 0 )
   {
      nRC = SetCursorFirstEntity( vObjectObj, "LOD_Attribute", "" );
      if ( nRC > zCURSOR_UNCHANGED )
         IncludeSubobjectFromSubobject( vUserSpec, "L_LOD_Attribute", vObjectObj, "LOD_Attribute", zPOS_AFTER );
   }

   DropView( vTempStyle );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_UpdtExistStyleAttrs( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zPCHAR szAttribType )
{
   zVIEW    vTempObj;
   zVIEW    vObjectObj;
   zLONG    lZKey;
   zSHORT   nRC;

   CreateViewFromViewForTask( &vTempObj, vUserSpec, 0 );
   GetViewByName( &vObjectObj, "LOD_Object", vUserSpec, zLEVEL_TASK );
   ResetView( vObjectObj );

   nRC = SetCursorFirstEntity( vTempObj, szAttribType, "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lZKey, vTempObj, szAttribType, "ZKey" );
      SetCursorFirstEntityByInteger( vObjectObj, "LOD_Attribute", "ZKey", lZKey, "LOD" );
      ExcludeEntity( vTempObj, szAttribType, zREPOS_PREV );
      IncludeSubobjectFromSubobject( vTempObj, szAttribType, vObjectObj, "LOD_Attribute", zPOS_AFTER );
      nRC = SetCursorNextEntity( vTempObj, szAttribType, "" );
   }

   DropView( vTempObj );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   AddNewEntityToStyle
//
// PURPOSE:  When a general type style object is created, there is no
//           object associated with it.  Because of this an ENTITY is
//           created under UI_Spec in the style object but there is no
//           information in ENTITY.  When the user goes to use this
//           style object they specify a certain object definition to
//           use.  AddNewEntityToStyle fills in information from a
//           selected ENTITY in the object definition to this ENTITY
//           in the style object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddNewEntityToStyle( zVIEW   vSubtask,
                                 zVIEW   vUserSpec,
                                 zPCHAR  szObjectName,
                                 zPCHAR  szEntityName )
{
   zVIEW  vObjectObj;
   zVIEW  vVOR;
   zVIEW  vFileObj;
   zCHAR  szWorkString[ 50 ];
   zLONG  lZKey;
   zSHORT nRC;

   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );

   if ( vObjectObj > 0 )
      DropObjectInstance( vObjectObj );

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_VOR_META );
   nRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef", "Name", szObjectName, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "There is no VOR with the name " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      MessageSend( vSubtask, "AD10207", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   nRC = ActivateMetaOI( vSubtask, &vVOR, vFileObj, zREFER_VOR_META, zSINGLE | zLEVEL_APPLICATION );
   GetIntegerFromAttribute( &lZKey, vVOR, "LOD", "ZKey" );

   nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_LOD_META );
   nRC = SetCursorFirstEntityByInteger( vFileObj, "W_MetaDef", "CPLR_ZKey", lZKey, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "The object definition - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " does not exist!" );
      MessageSend( vSubtask, "AD10208", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   nRC = ActivateMetaOI( vSubtask, &vObjectObj, vFileObj, zREFER_LOD_META,
                               zSINGLE | zLEVEL_APPLICATION );
   if ( nRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on object definition - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      MessageSend( vSubtask, "AD10209", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropObjectInstance( vObjectObj );
      return( -1 );
   }

   SetNameForView( vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntityByString( vObjectObj, "LOD_Entity", "Name", szEntityName, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "The entity name - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szEntityName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " does not exist!" );
      MessageSend( vSubtask, "AD10210", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropObjectInstance( vObjectObj );
      return( -1 );
   }

   IncludeSubobjectFromSubobject( vUserSpec, "UIS_ViewObjRef", vVOR, "ViewObjRef", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vUserSpec, "UIS_Entity", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vUserSpec, "LOD_Entity", vObjectObj, "LOD_Entity", zPOS_AFTER );

   /* BuildEntityStructureList( ) */
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeleteWindows
//
// PURPOSE: This function deletes everything under a dialog to prepare
//          for creating a new dialog with AutoDesignDialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_DeleteWindows( zVIEW vSubtask, zVIEW vNewDialog )
{
   zVIEW vCtrlDef;
   zSHORT nRC;
   static zCHAR szZKey[ 10 ];

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   SetCursorFirstEntity( vNewDialog, "Window", "" );
   CreateMetaEntity( vSubtask, vNewDialog, "Window", zPOS_BEFORE );

   SetCursorFirstEntityByString( vCtrlDef, "WindowStyle", "Tag", "Primary Window", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "WndStyle", vCtrlDef, "WindowStyle", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", 280 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", 128 );
   nRC = SetCursorNextEntity( vNewDialog, "Window", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      nRC = DeleteEntity( vNewDialog, "Window", zREPOS_NEXT );
   }

   nRC = SetCursorFirstEntity( vNewDialog, "ViewObjRef", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      nRC = ExcludeEntity( vNewDialog, "ViewObjRef", zREPOS_NEXT );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   AddDialogVORs
//
// PURPOSE: This function includes the UIS_ViewObjRefs from the user
//          interface spec(vUserSpec) to the ViewObjRefs in the
//          dialog instance(vNewDialog).
//
//          On 8/1/96 Don C changed the logic to only copy the first
//          ViewObjRef, which is the main one driving the Dialog.  The
//          others will be created on lower level routines.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AddDialogVORs( zVIEW vSubtask, zVIEW vUserSpec,
                           zVIEW vNewDialog, zPCHAR szInclEntity )
{
   zVIEW  vTemp;
   zSHORT nRC;
   zLONG  lZKey;

   CreateViewFromViewForTask( &vTemp, vUserSpec, 0 );
   ResetView( vTemp );

   // Include the first (hopefully only) ViewObjRef.
   nRC = SetCursorFirstEntity( vTemp, szInclEntity, "" );
//   while ( nRC > zCURSOR_UNCHANGED )
   if ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vTemp, szInclEntity, "ZKey" );
      nRC = SetCursorFirstEntityByInteger( vNewDialog, "ViewObjRef", "ZKey", lZKey, "" );

      // Include the VOR into the dialog if it does not already exist there.
      if ( nRC != zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vNewDialog, "ViewObjRef", vTemp, szInclEntity, zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vTemp, szInclEntity, "" );
   }

   DropMetaOI( vSubtask, vTemp );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   GetUserExitAddress
//
// PURPOSE: This function returns the address of a user exit, if it
//          exists.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zPVOID OPERATION
ofnTZWDLGSO_GetUserExitAddress( zVIEW vView,
                                zPCHAR szEntryName )
{
   LPLIBRARY lpLibrary;
   zPVOID    lpfEntry;

   // Try loading the library.
   lpLibrary = SysLoadLibraryWithErrFlag( vView, "tzaduser", 0 );
   if ( lpLibrary == 0 )
   {
      TraceLineS( "TZADUSER module not found.", "" );
      return( 0 );
   }
   else
      TraceLineS( "TZADUSER module found.", "" );

   // Try getting the exit operation address. Simply return the
   // address, which will be 0 if the operation doesn't exist.

   lpfEntry = SysGetProc( lpLibrary, szEntryName );
   if ( lpfEntry == 0 )
   {
      TraceLineS( szEntryName, " operation name not found." );
      return( 0 );
   }
   else
      TraceLineS( szEntryName, " operation name found." );

   return( lpfEntry );
}
