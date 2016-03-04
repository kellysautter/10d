/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw2aw.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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
// LAST MESSAGE ID: AD10702
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.10.05  DC
   Modified code for "Create Button to transfer to ListQual window" so that
   it checks for existence of WndDesign entity before retrieving value.

1999.04.24  DC
   Modified oTZADCSDO_AutoDesignCase4Group to set new list box type.

13.05.1997 DonC
   Modified CreateButtonAct... operations and calls to those routines so
   that Action names for Cancel and Close would be ESC and for OK and Exit
   would be Alt-F4.

13.06.1997 DonC
   If no value for ListQualWndButton was specified in the default object,
   set value to "ListQualNew".

25.07.1997 DonC
   Modified oTZADCSDO_AutoDesignCase4UPD to set cursor on first M_LOD_Attribute
   because only last attribute was being processed.

25.07.1997 DonC
   Modified oTZADCSDO_AutoDesignListQual to set tabbing order for controls
   so that cursor would be positioned in edit box.

30.03.1998 DonC
   Modified oTZADCSDO_AutoDesignCase2Group to give error message when
   include views are not selected.

02.07.1998 DonC
   Modified oTZADCSDO_AutoDesignCase4UPD to eliminate Zeidon core errors
   when a Case 4 entity was previously defined as another Case type.

29.07.1998  DonC
   Added source code generation of DeleteObjectEntityFromList operation.

1998/11/13  DC
   Modified oTZADCSDO_AutoDesignCase4Group to check return code of
   subroutine call and return if in error.

1999/03/10  DC
   Modified oTZADCSDO_CreateCase1Buttons to create source code
   CreateNewInstance when a new operation is generated.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "zdrvropr.h"
#include "tzctlopr.h"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase2INCL
//
// PURPOSE: This is the routine that creates the major window for the
//          selection of entries to be included in the parent window.
//          It is called from 2Group, which processes the group on the
//          parent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase2INCL( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR szCase )
{
   zVIEW    vAutoDesWk;
   zVIEW    vListQualWnd;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zCHAR    szSelectOperationName[ 50 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szListQualName[ 46 ];
   zLONG    lSize_X;
   zLONG    lSize_Y;

   // Case 2 INCL:
   //   o Create the new subordinate window.
   //   o Add the Groupbox containing the Select List.
   //   o Add the Select and Cancel buttons.
   //   o Add the Operation VML to the source code.


   // Initial housekeeping.
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );

   // For the generated VML, set the Case no in SubObjectType.
   SetAttributeFromString( vUSSubEnt, "UIS_Include", "SubObjectType", "2" );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                           "AutoDesignWDOD", "Y" );
//   GetStringFromAttribute( szCase, zsizeof( szCase ),
//                           vUSSubEnt, "UIS_Entity", "AutodesignCaseType" );

   // Fill out the window information.
   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUSSubEnt,
                                    vNewDialog, vAutoDesWk );

   // Create a group control along with the Include Listbox.
   oTZWDLGSO_AutoDesignGroupboxL( vSubtask, vUSSubEnt, vNewDialog,
                                  vDefaults, "B", "I" );

   // Reset scope to "Object Instance".
   oTZWDLGSO_ResetScopeToObject( vNewDialog );

   // Add the select and Cancel buttons.

   lButtonNbr = 0;

   ////////
   // Select
   ////////

   lButtonNbr++;

   // Set up operation name as concatenation of SelectSubobj.
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   strcpy_s( szSelectOperationName, zsizeof( szSelectOperationName ), "SelectSubObj" );
   strcat_s( szSelectOperationName, zsizeof( szSelectOperationName ), szEntityName );
   szSelectOperationName[ 32 ] = 0;

   // Get button text and create button.
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                           vDefaults, "InitValues", "SelectOnInclude" );
   oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                  vDefaults, szButtonText, 0,
                                  szSelectOperationName, lButtonNbr, "B",
                                  zWAB_ReturnToParentWithRefresh );

   // Include the Action under the Listbox DoubleClick event.
   oTZWDLGSO_AddActionDoubleClick( vSubtask, vNewDialog, vNewDialog );

   ////////
   // Cancel
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                           vDefaults, "InitValues", "CancelOnInclude" );
   oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                    vDefaults, vObject,
                                    szButtonText, "ESC", lButtonNbr, "B",
                                    zWAB_ReturnToParent,
                                    0, "" );
   // Also set NoMap for Cancel.
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   // Set Window Caption.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "WndCaption", "" ) != 0 )
      SetAttributeFromAttribute( vNewDialog, "Window",    "Caption",
                                 vUSSubEnt,  "WndDesign", "WndCaption" );
   else
      SetAttributeFromAttribute( vNewDialog, "Window",     "Caption",
                                 vUSSubEnt,      "LOD_Entity", "Name" );

   // If there is a List Qual subwindow, go to add the subwindow and then
   // add "GoToListQual" Postbuild action and event.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListQualFlag",
                                  "Y" ) == 0 )
   {
      // Create the empty ListQual window.
      // Use temporary view vListQualWnd so as not to lose cursor position.
      CreateViewFromViewForTask( &vListQualWnd, vNewDialog, 0 );
      CreateMetaEntity( vSubtask, vListQualWnd, "Window", zPOS_AFTER );
      strcpy_s( szListQualName, zsizeof( szListQualName ), "ListQual" );
      strcat_s( szListQualName, zsizeof( szListQualName ), szEntityName );
      szListQualName[ 32 ] = 0;
      SetAttributeFromString( vListQualWnd, "Window", "Tag", szListQualName );
      // Go to complete the ListQual window.  We must pass the operation
      // name for activating the list to this routine, which will be the
      // characters, "ListQual", followed by the LOD_Entity name.
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ),
                              vUSSubEnt, "LOD_Entity", "Name" );
      strcpy_s( szListQualName, zsizeof( szListQualName ), "ListAllSubObj" );
      strcat_s( szListQualName, zsizeof( szListQualName ), szEntityName );
      szListQualName[ 32 ] = 0;
      oTZADCSDO_AutoDesignListQual( vSubtask,
                                    vUSSubEnt,
                                    vListQualWnd,
                                    vDefaults,
                                    vObject,
                                    szListQualName );

      // Create Button and Action to transfer to ListQual window.
      lButtonNbr++;
      if ( CheckExistenceOfEntity( vDefaults, "WndDesign" ) >= zCURSOR_SET )
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                                 vDefaults, "WndDesign", "ListQualWndButtonText" );
      else
         szButtonText[ 0 ] = 0;
      if ( szButtonText[ 0 ] == 0 )
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                                 vDefaults, "DfltWndDesign", "ListQualWndButtonText" );
      if ( szButtonText[ 0 ] == 0 )
         strcpy_s( szButtonText, zsizeof( szButtonText ), "ListQualNew" );
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                       vDefaults, vObject,
                                       szButtonText, 0, lButtonNbr, "B",
                                       zWAB_StartModalSubwindow,
                                       0, "" );
      // Also set NoMap.
      SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialog, "Action", "DialogName",
                                 vNewDialog, "Dialog", "Tag" );
      SetAttributeFromAttribute( vNewDialog,   "Action", "WindowName",
                                 vListQualWnd, "Window", "Tag" );

      // Create Postbuild window event and tie to Action just created.
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  2 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct",
                                     vNewDialog, "Action", zPOS_AFTER );
      DropView( vListQualWnd );
   }

   // Add the Operation VML to the source code.
   ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialog, "ListAllSubObj" );
   ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialog, "SelectSubObj" );

   // Set the window size.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 31 );

   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign",
                                   "WndStart_X", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window",    "PSDLG_X",
                                 vUSSubEnt,  "WndDesign", "WndStart_X" );
   }
   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign",
                                   "WndStart_Y", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window",    "PSDLG_Y",
                                 vUSSubEnt,  "WndDesign", "WndStart_Y" );
   }

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase2Group
//
// PURPOSE: This is the routine that processes each Autodesign Case Type 2
//          UIS_Entity for the Groupbox on the parent window.
//          It is called only from Autodesign Case Type 1.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase2Group( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEnt,
                                zVIEW  vNewDialog,
                                zVIEW  vParentRoot,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR szCase )
{
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialogRoot;
   zLONG    lWkSize;
   zLONG    lWkPos;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zCHAR    szSubWindowTag[ 33 ];
   zCHAR    szListOperationName[ 50 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szMsg[ 100 ];

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Case 2 requires that vUserSpec.ListViewObjRef and
   // vUserSpec.IncludeViewObjRef have both been selected. Give error
   // message if either entity is null.
   if ( CheckExistenceOfEntity( vUSSubEnt, "ListViewObjRef" ) < zCURSOR_SET ||
        CheckExistenceOfEntity( vUSSubEnt, "IncludeViewObjRef" ) < zCURSOR_SET )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Invalid Case 2 specification for entity: " );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
      strcat_s( szMsg, zsizeof( szMsg ), szEntityName );
      strcat_s( szMsg, zsizeof( szMsg ), "." );
      MessageSend( vSubtask, "AD10703", "Autodesigner",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Case 2 Group:
   //   o Go to 2INCL to create the new subordinate window.
   //   o Add the Groupbox containing the Attributes displayed on the parent
   //     window.
   //   o Add the Select and Delete buttons.  Note that Delete is an option
   //     on the Case2Detail window, but is only allowed there if the
   //     cardinality from parent to child is 0 to 1, instead of 1 to 1.

   // Create the empty subordinate window.
   // Use temporary view vNewDialogRoot so as not to lose cursor position.
   CreateMetaEntity( vSubtask, vNewDialogRoot, "Window", zPOS_AFTER );
   SetAttributeFromAttribute( vNewDialogRoot, "Window",     "Tag",
                              vUSSubEnt,      "LOD_Entity", "Name" );

   // Go to 2INCL to create the subordinate window.

   oTZADCSDO_AutoDesignCase2INCL( vSubtask, vUserSpec, vUSSubEnt,
                                  vNewDialogRoot, vDefaults, vObject, szCase );

   // Save the name of the window just created and then position back
   // to the parent window.
   GetStringFromAttribute( szSubWindowTag, zsizeof( szSubWindowTag ), vNewDialogRoot, "Window", "Tag" );
   DropView( vNewDialogRoot );
   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Now we're back to the original parent window.

   // Add the Groupbox containing the Attributes displayed on the parent
   // window.
   oTZWDLGSO_AutoDesignGroupboxM( vSubtask, vUSSubEnt, vNewDialog,
                                  vDefaults, vAutoDesWk, "B" );

   // Set Groupbox Text.
   SetAttributeFromAttribute( vNewDialog, "Control",   "Text",
                              vUSSubEnt,  "WndDesign", "ListGroupboxText" );

   // Set the Y position of the Groupbox just created to:
   //   FirstGroupboxPosY + FirstGroupboxSize + 10
   SetCursorFirstEntity( vNewDialog, "Control", 0 );
   GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
   GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_Y" );
   SetCursorLastEntity( vNewDialog, "Control", 0 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lWkPos + lWkSize + 10 );

   // Next, add the buttons as necessary.
   // Restart the Button counter for the group just created.
   lButtonNbr = 0;

   ////////
   // New
   ////////

   lButtonNbr++;

   // Set up operation name as concatenation of ListAllSubobj.
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   strcpy_s( szListOperationName, zsizeof( szListOperationName ), "ListAllSubObj" );
   strcat_s( szListOperationName, zsizeof( szListOperationName ), szEntityName );
   szListOperationName[ 32 ] = 0;

   // Get Button text.
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "SelectOnParent" );

   // If the List is qualified, create an ActObject that transfers to
   // the ListQual window.
   // If the List is NOT qualified, create the ActOper for ListAllSubObj.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListQualFlag","Y" ) == 0 )
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                       vDefaults, vObject,
                                       szButtonText, 0, lButtonNbr, "B",
                                       zWAB_StartModalSubwindow, 0, "" );
   else
      oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                     vDefaults, szButtonText, 0,
                                     szListOperationName, lButtonNbr, "B",
                                     zWAB_StartModalSubwindow );

   // Set the Dialog and Window names in the Action.
   SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName", vNewDialogRoot, "Dialog", "Tag" );
   SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", szSubWindowTag );

   DropView( vNewDialogRoot );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateCase1Buttons
//
// PURPOSE: This routine creates the buttons and associated actions for
//          Case 1.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_CreateCase1Buttons( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vUSSubEnt,
                              zVIEW  vNewDialog,
                              zVIEW  vParentRoot,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR szSubWindowTag,
                              zPCHAR szTypeFlag )
{
   zVIEW    vNewDialogRoot;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zLONG    lSubAction;

   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Restart the Button counter for the group just created.
   lButtonNbr = 0;

   ////////
   // New   - Exists only if "Create on List" selected for UIS_Entity.
   ////////

   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListCreateAction", "Y" ) == 0 )
   {
      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "New" );

      // SubAction is always temporal.
      lSubAction = zSubActionCreateTemporalEntity;

      // If subwindow, create Action with Object Based functionality.
      // Otherwise, create Action with Operation functionality.
      if ( szTypeFlag[ 0 ] == 'S' )
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                          vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "B",
                                          zWAB_StartModalSubwindow, lSubAction, "" );
      else
      {
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                        vDefaults, szButtonText, 0,
                                        "CreateNewInstance", lButtonNbr, "B",
                                        zWAB_StartModalSubwindow );
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "CreateNewInstance" );
      }

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                                 vNewDialogRoot, "Dialog", "Tag" );
      SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", szSubWindowTag );
   }

   ////////
   // Update   - Exists only if "Update Attributes" selected for UIS_Entity.
   ////////

   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UsageStyle", "U" ) == 0 )
   {
      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "DetailUpdate" );

      // SubAction is always temporal.
      lSubAction = zSubActionCreateTemporalVer;

      // If subwindow, create Action with Object Based functionality.
      // Otherwise, create Action with Operation functionality.
      if ( szTypeFlag[ 0 ] == 'S' )
      {
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                          vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "B",
                                          zWAB_StartModalSubwindow,
                                          lSubAction, "" );
      }
      else
      {
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                        vDefaults, szButtonText, 0,
                                        "UpdateExistingInstance", lButtonNbr, "B",
                                        zWAB_StartModalSubwindow );

         // Add include of LOD_Entity and ViewObjRef for auto disable.
         if ( CheckExistenceOfEntity( vNewDialogRoot, "ActMap" ) < zCURSOR_SET )
            CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                        vObject,        "LOD_Entity", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView",
                                        vNewDialogRoot, "ViewObjRef", zPOS_AFTER );
      }

      // Set auto disable Action for null entity.
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 8 );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                                 vNewDialogRoot, "Dialog", "Tag" );
      SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", szSubWindowTag );

      // Also include the same Action under a double click window event
      // for the ListBox.
      oTZWDLGSO_AddActionDoubleClick( vSubtask, vNewDialog, vNewDialogRoot );
   }

   ////////
   // Detail   - Exists only if "Display Attributes" selected for UIS_Entity.
   ////////

   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "UsageStyle", "D" ) == 0 )
   {
      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "DetailDisplay" );

      // If subwindow, create Action with Object Based functionality.
      // Otherwise, create Action with Operation functionality.
      if ( szTypeFlag[ 0 ] == 'S' )
      {
         // There is no SubAction.
         lSubAction = 0;

         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                          vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "B",
                                          zWAB_StartModalSubwindow,
                                          lSubAction, "" );
      }
      else
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                        vDefaults, szButtonText, 0,
                                        "UpdateExistingInstance", lButtonNbr, "B",
                                        zWAB_StartModalSubwindow );

      // Add include of LOD_Entity and ViewObjRef for auto disable.
      if ( CheckExistenceOfEntity( vNewDialogRoot, "ActMap" ) < zCURSOR_SET )
         CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                     vObject,        "LOD_Entity", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView",
                                     vNewDialogRoot, "ViewObjRef", zPOS_AFTER );

      // Set auto disable Action for null entity.
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 8 );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                                 vNewDialogRoot, "Dialog", "Tag" );
      SetAttributeFromString( vNewDialogRoot, "Action", "WindowName",
                              szSubWindowTag );

      // Also include the same Action under a double click window event
      // for the ListBox.
      oTZWDLGSO_AddActionDoubleClick( vSubtask, vNewDialog, vNewDialogRoot );
   }

   ////////
   // Delete   - Exists only if "Delete on List" selected for UIS_Entity.
   ////////

   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListDeleteAction", "Y" ) == 0 )
   {
      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Delete" );

      // If subwindow, create Action with Object Based functionality.
      // Otherwise, create Action with Operation functionality.
      if ( szTypeFlag[ 0 ] == 'S' )
         oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                          vDefaults, vObject,
                                          szButtonText, 0, lButtonNbr, "B",
                                          zWAB_StayOnWindowWithRefresh,
//                                        zSubActionDelete, "" );
                                          268435460, "" );
      else
      {
         oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                        vDefaults, szButtonText, 0,
                                        "DeleteObjectEntityFromList", lButtonNbr, "B",
                                        zWAB_StayOnWindowWithRefresh );

         // Add include of LOD_Entity and ViewObjRef for auto disable.
         if ( CheckExistenceOfEntity( vNewDialogRoot, "ActMap" ) < zCURSOR_SET )
            CreateMetaEntity( vSubtask, vNewDialogRoot, "ActMap", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                        vObject,        "LOD_Entity", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapView",
                                       vNewDialogRoot, "ViewObjRef", zPOS_AFTER );

         // Create the DeleteObjectEntityFromList operation in source code.
         ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialogRoot, "DeleteObjectEntityFromList" );
      }

      // Set auto disable Action for null entity.
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 8 );
   }

   DropView( vNewDialogRoot );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase1List
//
// PURPOSE: This is the routine that creates the listbox group on the parent
//          window for each Autodesign Case Type 1 UIS_Entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase1List( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vParentRoot,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR szCase )
{
   zVIEW    vNewDialogRoot;
   zLONG    lWkSize;
   zLONG    lWkPos;
   zCHAR    szSubWindowTag[ 33 ];
   zSHORT   nRC;

   // Case 1 List:
   //   o Create the GroupBox control which will hold other controls.
   //   o Create the List Box on parent window.
   //   o Create the New, Detail and Delete buttons as necessary.
   //   o Create the new window and then proceed to process the
   //     subobject.

   // First create the subordinate window, before returning back to
   // finish the controls on the parent window.

   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Create the new window and proceed to processing the subobject.
   CreateMetaEntity( vSubtask, vNewDialogRoot, "Window", zPOS_AFTER );
   SetAttributeFromAttribute( vNewDialogRoot, "Window",     "Tag",
                              vUSSubEnt,     "LOD_Entity", "Name" );

   nRC = oTZADCSDO_AutoDesignCase1( vSubtask, vUserSpec, vUSSubEnt, vNewDialogRoot, vDefaults );

   // Save the name of the window just created and then position back
   // to the parent window.
   // Skip if error on call.
   if ( nRC >= 0 )
      GetStringFromAttribute( szSubWindowTag, zsizeof( szSubWindowTag ), vNewDialogRoot, "Window", "Tag" );
   DropView( vNewDialogRoot );

   // Now we're back to the original parent window.

   // Add the Groupbox and Listbox for listing the subentity attributes
   // on the parent window.
   nRC = oTZWDLGSO_AutoDesignGroupboxL( vSubtask, vUSSubEnt, vNewDialog, vDefaults, "B", "L" );

   // Set Groupbox Text.
   SetAttributeFromAttribute( vNewDialog, "Control",   "Text",
                              vUSSubEnt,  "WndDesign", "ListGroupboxText" );

   // Set the Y position of the Groupbox just created to:
   //   FirstGroupboxPosY + FirstGroupboxSize + 10
   SetCursorFirstEntity( vNewDialog, "Control", 0 );
   GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
   GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_Y" );
   SetCursorLastEntity( vNewDialog, "Control", 0 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lWkPos + lWkSize + 10 );

   // Next, add the buttons as necessary.
   oTZADCSDO_CreateCase1Buttons( vSubtask, vUserSpec, vUSSubEnt, vNewDialog, vParentRoot,
                                 vDefaults, vObject, szSubWindowTag, "Subwindow" );

   // Recalculate the Groupbox size based on the buttons just added and set it.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase4INCL
//
// PURPOSE: This is the routine that creates the major window for the
//          selection of entries to be included in the parent window.
//          It is called from 4Group, which processes the group on the
//          parent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase4INCL( zVIEW  vSubtask,
                               zVIEW  vUserSpec,
                               zVIEW  vUSSubEnt,
                               zVIEW  vNewDialog,
                               zVIEW  vDefaults,
                               zVIEW  vObject,
                               zPCHAR szCase )
{
   zVIEW    vAutoDesWk;
   zVIEW    vListQualWnd;
   zVIEW    vTemp;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zCHAR    szSelectOperationName[ 50 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szLOD_Name[ 33 ];
   zCHAR    szListQualName[ 46 ];
   zCHAR    szMsg[ 100 ];
   zLONG    lSize_X;
   zLONG    lSize_Y;
   zSHORT   nRC;

   // Case 4 INCL:
   //   o Add info to the new subordinate window.
   //   o Add the Groupbox containing the updateable attributes on the
   //     Associative entity.
   //   o Add the Groupbox containing the attribute list of items that
   //     can be selected for inclusion.
   //   o Add the Select and Close buttons.
   //   o Add the Operation VML to the source code.

   // Exit with error message if no UIS_Include entity.
   if ( CheckExistenceOfEntity( vUSSubEnt, "UIS_Include" ) < zCURSOR_SET )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "External Object for Selection List not specified for entity: " );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
      strcat_s( szMsg, zsizeof( szMsg ), szEntityName );
      strcat_s( szMsg, zsizeof( szMsg ), "." );
      MessageSend( vSubtask, "AD10701", "Autodesigner",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // Initial housekeeping.
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );

   // For the generated VML, set the Case no in SubObjectType.
   SetAttributeFromString( vUSSubEnt, "UIS_Include", "SubObjectType", szCase );

   SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "AutoDesignWDOD", "Y" );

   // Fill out the window information.
   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUSSubEnt,
                                    vNewDialog, vAutoDesWk );

   // Initialize the Counter to make control tags unique.
   SetAttributeFromInteger( vNewDialog, "Window", "ControlTagCounter", 0 );

   // Force that the attributes are updatable.
   SetAttributeFromString( vUSSubEnt, "WndDesign", "UsageStyle", "U" );

   // Add the group control containing the updateable attributes on the
   // Associative entity.  These are the attributes in M_LOD_...
   // Only add the group if there are M_LOD_Attribute entities or if there
   // are Case5 entities.
   CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
   nRC = SetCursorFirstEntityByString( vTemp, "UIS_Entity", "AutodesignCaseType", "5", 0 );
   if ( nRC >= zCURSOR_SET ||
        CheckExistenceOfEntity( vUSSubEnt, "M_LOD_Attribute" ) >= zCURSOR_SET )
   {
      oTZWDLGSO_AutoDesignGroupboxM( vSubtask, vUSSubEnt, vNewDialog, vDefaults, vAutoDesWk, "" );
   }
   DropView( vTemp );

   // Add Case 5 combo boxes for Case 5 entities.
   oTZADCSDO_AutoDesignCase5Attrs( vSubtask, vUserSpec, vUSSubEnt, vNewDialog,
                                   vDefaults, vObject, "5" );

   // Recalculate the Groupbox size based on the Combo Boxes just added and
   // set it.
   if ( CheckExistenceOfEntity( vNewDialog, "Control" ) >= zCURSOR_SET )
      oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );

   // Create a group control containing the Include Listbox.
   oTZWDLGSO_AutoDesignGroupboxL( vSubtask, vUSSubEnt, vNewDialog, vDefaults, "B", "I" );

   // Reset scope to "Object Instance".
   oTZWDLGSO_ResetScopeToObject( vNewDialog );

   // Add the Select and Close buttons.

   lButtonNbr = 0;

   ////////
   // Select
   ////////

   lButtonNbr++;

   // Set up operation name as concatenation of SelectSubobj.
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   strcpy_s( szSelectOperationName, zsizeof( szSelectOperationName ), "SelectSubObj" );
   strcat_s( szSelectOperationName, zsizeof( szSelectOperationName ), szEntityName );
   szSelectOperationName[ 32 ] = 0;

   // Get button text and create button.
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "SelectOnInclude" );
   oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                  vDefaults, szButtonText, 0,
                                  szSelectOperationName, lButtonNbr, "B",
                                  zWAB_StayOnWindowWithRefresh );

   // Include the Action under the Listbox DoubleClick event.
   oTZWDLGSO_AddActionDoubleClick( vSubtask, vNewDialog, vNewDialog );

   ////////
   // Close
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "CloseOnInclude" );

   if ( szCase[ 0 ] == '3' )
      // For Case 3,
      // Action is null, since there is no Assoc. entity to get rid of.
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                       vDefaults, vObject,
                                       szButtonText, "ESC", lButtonNbr, "B",
                                       zWAB_ReturnToParentWithRefresh,
                                       0, "" );
   else
      // For Case 4,
      // Action is Cancel to get rid of the Temporal Assoc. Entity.
      // Note that the following operation includes the wrong ActLOD_Entity,
      // which must be replaced after the call.
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                       vDefaults, vObject,
                                       szButtonText, "ESC", lButtonNbr, "B",
                                       zWAB_ReturnToParentWithRefresh,
                                       zSubActionCancel, "" );

   // For Case 4, the ActMapLOD_Entity was created incorrectly.  The
   // LOD_Entity included should have been for the Assoc. entity, but it was
   // instead created for the child of the Assoc. entity.  Thus we will
   // correct it here.
   if ( szCase[ 0 ] == '4' || szCase[ 0 ] == '5' )
   {
      ExcludeEntity( vNewDialog, "ActMapLOD_Entity", zREPOS_NONE );
      CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
      ResetViewFromSubobject( vTemp );
      IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                     vTemp,          "LOD_Entity", zPOS_AFTER );
      DropView( vTemp );
   }

   // Also set NoMap for Close.
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   // Set Window Caption.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "WndCaption", "" ) != 0 )
      SetAttributeFromAttribute( vNewDialog, "Window",    "Caption",
                                 vUSSubEnt,      "WndDesign", "WndCaption" );
   else
      SetAttributeFromAttribute( vNewDialog, "Window",     "Caption",
                                 vUSSubEnt,      "LOD_Entity", "Name" );

   // If there is a List Qual subwindow, go to add the subwindow and then
   // add "GoToListQual" Postbuild action and event.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListQualFlag", "Y" ) == 0 )
   {
      // *** List Qual

      // Create the empty ListQual window.
      // Use temporary view vListQualWnd so as not to lose cursor position.
      CreateViewFromViewForTask( &vListQualWnd, vNewDialog, 0 );
      CreateMetaEntity( vSubtask, vListQualWnd, "Window", zPOS_AFTER );
      strcpy_s( szListQualName, zsizeof( szListQualName ), "ListQual" );
      strcat_s( szListQualName, zsizeof( szListQualName ), szEntityName );
      szListQualName[ 32 ] = 0;
      SetAttributeFromString( vListQualWnd, "Window", "Tag", szListQualName );
      // Go to complete the ListQual window.  We must pass the operation
      // name for activating the list to this routine, which will be the
      // characters, "ListQual", followed by the LOD_Entity name.
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ),
                              vUSSubEnt, "LOD_Entity", "Name" );
      strcpy_s( szListQualName, zsizeof( szListQualName ), "ListAllSubObj" );
      strcat_s( szListQualName, zsizeof( szListQualName ), szEntityName );
      szListQualName[ 32 ] = 0;
      oTZADCSDO_AutoDesignListQual( vSubtask, vUSSubEnt, vListQualWnd, vDefaults, vObject, szListQualName );

      // Create the Action that will do a CreateTemporalEntity for the
      // Associative entity.
      SetCursorLastEntity( vNewDialog, "Action", 0 );
      CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "Action", "Type", zWAB_StayOnWindow );
      SetAttributeFromString( vNewDialog, "Action", "Tag", "Prebuild" );
      SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );
      GetStringFromAttribute( szLOD_Name, zsizeof( szLOD_Name ), vObject, "LOD", "Name" );
      SetCursorFirstEntityByString( vNewDialog, "ViewObjRef", "Name", szLOD_Name, 0 );
      CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
      ResetViewFromSubobject( vTemp );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vTemp, "LOD_Entity", "Name" );
      SetCursorFirstEntityByString( vObject, "LOD_Entity", "Name", szEntityName, 0 );
      CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", zSubActionCreateTemporalEntity );
      IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                     vObject,    "LOD_Entity", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "ActMapView",
                                     vNewDialog, "ViewObjRef", zPOS_AFTER );
      DropView( vTemp );

      // Create the Prebuild Event and tie the Action to it.
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  1 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct", vNewDialog, "Action", zPOS_AFTER );

      // Create Button and Action to transfer to ListQual window.
      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "WndDesign", "ListQualWndButtonText" );
      if ( szButtonText[ 0 ] == 0 )
         GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "DfltWndDesign", "ListQualWndButtonText" );
      if ( szButtonText[ 0 ] == 0 )
         strcpy_s( szButtonText, zsizeof( szButtonText ), "ListQualNew" );

      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                       vDefaults, vObject,
                                       szButtonText, 0, lButtonNbr, "B",
                                       zWAB_StartModalSubwindow, 0, "" );
      // Also set NoMap.
      SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialog, "Action", "DialogName",
                                 vNewDialog, "Dialog", "Tag" );
      SetAttributeFromAttribute( vNewDialog,   "Action", "WindowName",
                                 vListQualWnd, "Window", "Tag" );

      // Create Postbuild window event and tie to Action just created.
      CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
      SetAttributeFromInteger( vNewDialog, "WndEvent", "Type",  2 );
      IncludeSubobjectFromSubobject( vNewDialog, "WndAct",
                                     vNewDialog, "Action", zPOS_AFTER );
      DropView( vListQualWnd );
   }

   // Add the Operation VML to the source code.
   ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialog, "ListAllSubObj" );
   ofnTZWDLGSO_CreateSourceCode( vSubtask, vUSSubEnt, vNewDialog, "SelectSubObj" );

   // Set the window size.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 31 );

   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign",
                                   "WndStart_X", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window", "PSDLG_X",
                                 vUSSubEnt,      "WndDesign", "WndStart_X" );
   }
   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign",
                                   "WndStart_Y", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window", "PSDLG_Y",
                                 vUSSubEnt,      "WndDesign", "WndStart_Y" );
   }

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase4UPD
//
// PURPOSE: This is the routine that creates the major window for the
//          update of attributes on the associative entity of a Case 4
//          associative entity/primary entity pair.
//          It is called from 4Group, which processes the group on the
//          parent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase4UPD( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vUSSubEnt,
                              zVIEW  vCallerUserSpec,
                              zVIEW  vNewDialog,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR szCase )
{
   zVIEW    vAutoDesWk;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zSHORT   RESULT;
   zLONG    lSize_X;
   zLONG    lSize_Y;

   // Case 4 UPD:
   //   o Add info to the new subordinate window.
   //   o Add the Groupbox containing the updateable attributes on the
   //     Associative entity.
   //   o Add the OK, Cancel buttons.
   //   o Process each Case 1 subentity, adding list groups back on this
   //     window as required.


   // Initial housekeeping.
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );

   // Fill out the window information.
   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUSSubEnt,
                                    vNewDialog, vAutoDesWk );

   // Initialize the Counter to make control tags unique.
//   SetAttributeFromInteger( vNewDialog, "Window", "ControlTagCounter", 0 );

   // Force that the attributes are updatable.
   SetAttributeFromString( vUSSubEnt, "WndDesign", "UsageStyle", "U" );

   // Remove any M_LOD_Attribute entities from the parent of the Case 4 pair,
   // since they may exist if the entity was once defined as a different Case type.
   for ( RESULT = SetCursorFirstEntity( vUSSubEnt, "M_LOD_Attribute", 0 );
         RESULT >= zCURSOR_SET;
         RESULT = SetCursorNextEntity( vUSSubEnt, "M_LOD_Attribute", 0 ))
   {
      ExcludeEntity( vUSSubEnt, "M_LOD_Attribute", zREPOS_NONE );
   }

   // Copy the M_LOD_Attribute entities from the UIS_ChildEntity that called
   // us here, since that is where they were added in the User Spec dialog.
   RESULT = SetCursorFirstEntity( vCallerUserSpec, "M_LOD_Attribute", 0 );
   while ( RESULT >= zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vUSSubEnt,       "M_LOD_Attribute",
                                     vCallerUserSpec, "M_LOD_Attribute", zPOS_AFTER );

      RESULT = SetCursorNextEntity( vCallerUserSpec, "M_LOD_Attribute", 0 );
   }

   // Make sure we are positioned on the first M_LOD_Attribute entity.
   SetCursorFirstEntity( vUSSubEnt, "M_LOD_Attribute", 0 );

   // Add the group control containing the updateable attributes on the
   // Associative entity.  These are the attributes in M_LOD_...
   oTZWDLGSO_AutoDesignGroupboxM( vSubtask, vUSSubEnt, vNewDialog,
                                  vDefaults, vAutoDesWk, "R" );

   // Add Case 5 combo boxes for Case 5 entities.
   SetViewToSubobject( vUSSubEnt, "UIS_ChildEntity" );
   oTZADCSDO_AutoDesignCase5Attrs( vSubtask, vUserSpec, vUSSubEnt,
                                   vNewDialog, vDefaults, vObject, "5" );
   ResetViewFromSubobject( vUSSubEnt );

   // Create the OK and Cancel buttons.
   // Restart the Button counter for the group just created.
   lButtonNbr = 0;

   ////////
   // OK
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "OK" );
   oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                    vDefaults, vObject,
                                    szButtonText, "Alt-F4", lButtonNbr, "S",
                                    zWAB_ReturnToParentWithRefresh,
                                    zSubActionAccept, "" );

   ////////
   // Cancel
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "Cancel" );
   oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialog,
                                    vDefaults, vObject,
                                    szButtonText, "ESC", lButtonNbr, "S",
                                    zWAB_ReturnToParentWithRefresh,
                                    zSubActionCancel, "" );
   // Also set NoMap for Cancel.
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   // Recalculate the Groupbox size based on the buttons just added and
   // set it.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );


   // Set Window Caption for Update.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "WndCaption2", "" ) != 0 )
      SetAttributeFromAttribute( vNewDialog, "Window",    "Caption",
                                 vUSSubEnt,  "WndDesign", "WndCaption2" );
   else
      SetAttributeFromAttribute( vNewDialog, "Window",     "Caption",
                                 vUSSubEnt,  "LOD_Entity", "Name" );

   // Set the window size.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 31 );

   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign", "WndStart_X", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window", "PSDLG_X",
                                 vUSSubEnt,      "WndDesign", "WndStart_X" );
   }
   if ( CompareAttributeToInteger( vUSSubEnt, "WndDesign", "WndStart_Y", 0 ) != 0 )
   {
      SetAttributeFromAttribute( vNewDialog, "Window", "PSDLG_Y",
                                 vUSSubEnt,      "WndDesign", "WndStart_Y" );
   }

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignCase4Group
//
// PURPOSE: This is the routine that processes each Autodesign Case Type 4
//          UIS_Entity for the Groupbox on the parent window.
//          It is called only from Autodesign Case Type 1.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignCase4Group( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zVIEW  vUSSubEnt,
                                zVIEW  vNewDialog,
                                zVIEW  vParentRoot,
                                zVIEW  vDefaults,
                                zVIEW  vObject,
                                zPCHAR szCase )
{
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialogRoot;
   zVIEW    vTemp;
   zVIEW    vUPD_UserSpec;
   zLONG    lWkSize;
   zLONG    lWkPos;
   zCHAR    szButtonText[ 21 ];
   zLONG    lButtonNbr;
   zLONG    lSubAction;
   zLONG    lSubtype;
   zCHAR    szWindowTag[ 33 ];
   zCHAR    szSubWindowTag[ 33 ];
   zCHAR    szListOperationName[ 50 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szMsg[ 100 ];
   zSHORT   nRC;

   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Case 4 Group:
   //   o Go to 4INCL to create the new subordinate window.
   //   o Add the Groupbox containing the list of currently selected
   //     subobjects.
   //   o Add the Select and Delete buttons.
   //   o If there are attributes on the Associative entity, add the
   //     subwindow that allows them to be updated.  Prior to doing this,
   //     copy the I_LOD_... entities to the M_LOD_... entities so that
   //     the Groupbox on this new window will contain the same attributes
   //     as the listbox on the include window.
   //   o If there are Attributes on the Associative entity, add the Update
   //     button to transfer to the subwindow just created.

   //////////////
   //
   // Subwindow for Inclusion of New Sub Entities
   //
   //////////////

   // Create the empty subordinate list window.
   CreateMetaEntity( vSubtask, vNewDialogRoot, "Window", zPOS_AFTER );
   SetAttributeFromAttribute( vNewDialogRoot, "Window",     "Tag",
                              vUSSubEnt,      "LOD_Entity", "Name" );

   // Go to 4INCL to complete the window just created.

   nRC = oTZADCSDO_AutoDesignCase4INCL( vSubtask,
                                        vUserSpec,
                                        vUSSubEnt,
                                        vNewDialogRoot,
                                        vDefaults,
                                        vObject,
                                        szCase );
   if ( nRC < 0 )
      return( -1 );

   // Save the name of the window just created and then position back
   // to the parent window.
   GetStringFromAttribute( szSubWindowTag, zsizeof( szSubWindowTag ),
                           vNewDialogRoot, "Window", "Tag" );
   DropView( vNewDialogRoot );
   CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

   // Now we're back to the original parent window.

   //////////////
   //
   // ListBox on Parent Window
   //
   //////////////

   // Give Error Message if no L_LOD_Attribute entities.
   if ( CheckExistenceOfEntity( vUSSubEnt, "L_LOD_Attribute" ) < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "No List attributes selected for entity: " );
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
      strcat_s( szMsg, zsizeof( szMsg ), szEntityName );
      strcat_s( szMsg, zsizeof( szMsg ), "." );
      MessageSend( vSubtask, "AD10702", "Autodesigner",
                   szMsg,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }
   else
      // Add the Groupbox containing the Listbox of currently included entities
      // on the parent window.
      oTZWDLGSO_AutoDesignGroupboxL( vSubtask, vUSSubEnt, vNewDialog, vDefaults, "B", "L" );

   // Set Groupbox Text.
   SetAttributeFromAttribute( vNewDialog, "Control",   "Text",
                              vUSSubEnt,  "WndDesign", "ListGroupboxText" );

   // Set the Y position of the Groupbox just created to:
   //   FirstGroupboxPosY + FirstGroupboxSize + 10
   SetCursorFirstEntity( vNewDialog, "Control", 0 );
   GetIntegerFromAttribute( &lWkSize, vNewDialog, "Control", "SZDLG_Y" );
   GetIntegerFromAttribute( &lWkPos,  vNewDialog, "Control", "PSDLG_Y" );
   SetCursorLastEntity( vNewDialog, "Control", 0 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lWkPos + lWkSize + 10 );

   // Set the scoping entity of the ListBox just created if this is Case 4.
   // We must go back 2 levels on the UserSpec to get to the correct entity.
   // Use work view so as not to mess up the USSubEnt view.
   // Also set the vDialog view down to the Listbox, instead of the Groupbox.
   if ( szCase[ 0 ] == '4' )
   {
      CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
      ResetViewFromSubobject( vTemp );
      ResetViewFromSubobject( vTemp );
      SetViewToSubobject( vNewDialog, "CtrlCtrl" );
      CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Entity",
                                     vTemp,      "LOD_Entity", zPOS_AFTER );
      lSubtype = 8256;            // Adds scoping entity type.
      lSubtype |= zLISTBOX_SORTEDHEADERS;
      SetAttributeFromInteger( vNewDialog, "Control", "Subtype", lSubtype );
      ResetViewFromSubobject( vNewDialog );
      DropView( vTemp );
   }

   // Next, add the buttons as necessary.
   // Restart the Button counter for the group just created.
   lButtonNbr = 0;

   ////////
   // New
   ////////

   lButtonNbr++;

   // Set up operation name as concatenation of ListAllSubobj.
   GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUSSubEnt, "LOD_Entity", "Name" );
   strcpy_s( szListOperationName, zsizeof( szListOperationName ), "ListAllSubObj" );
   strcat_s( szListOperationName, zsizeof( szListOperationName ), szEntityName );
   szListOperationName[ 32 ] = 0;

   // Get Button text.
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "SelectOnParent" );

   // There is no SubAction for this button.
   lSubAction = 0;


   // If the List is qualified, create an ActObject that transfers to
   // the ListQual window.
   // If the List is NOT qualified, create the ActOper for ListAllSubObj.
   if ( CompareAttributeToString( vUSSubEnt, "WndDesign", "ListQualFlag", "Y" ) == 0 )
      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                       vDefaults, vObject,
                                       szButtonText, 0, lButtonNbr, "B",
                                       zWAB_StartModalSubwindow, 0, "" );
   else
      oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                     vDefaults, szButtonText, 0,
                                     szListOperationName, lButtonNbr, "B",
                                     zWAB_StartModalSubwindow );

   // Set the Dialog and Window names in the Action.
   SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                              vNewDialogRoot, "Dialog", "Tag" );
   SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", szSubWindowTag );


   //////////////
   //
   // Subwindow for Update of Associative Attributes
   //
   //////////////

   // Only add the subwindow if there are M_LOD_Attribute entities and this
   // is Case 4.
   if ( CheckExistenceOfEntity( vUSSubEnt, "M_LOD_Attribute" ) >= zCURSOR_SET &&
        szCase[ 0 ] == '4' )
   {

      // Create the empty subordinate update window.
      // Save the Tag of the current window name so that we can
      // reposition back to it after creating the subordinate window.
      GetStringFromAttribute( szWindowTag, zsizeof( szWindowTag ),
                              vNewDialogRoot, "Window", "Tag" );
      CreateMetaEntity( vSubtask, vNewDialogRoot, "Window", zPOS_AFTER );

      // We will reset the view back to the parent Asssoc. entity, because
      // the update processing is really done at that level.  We will create
      // a new view for this purpose so that we will not lose position on
      // the current UIS_Entity.
      CreateViewFromViewForTask( &vUPD_UserSpec, vUSSubEnt, 0 );
      SetNameForView( vUPD_UserSpec, "***UPD_UserSpec", vSubtask, zLEVEL_TASK );
      ResetViewFromSubobject( vUPD_UserSpec );
      SetAttributeFromAttribute( vNewDialogRoot, "Window",     "Tag",
                                 vUPD_UserSpec,  "LOD_Entity", "Name" );

      // Go to 4UPD to complete the window just created.

      oTZADCSDO_AutoDesignCase4UPD( vSubtask, vUserSpec, vUPD_UserSpec, vUSSubEnt, vNewDialogRoot,
                                    vDefaults, vObject, szCase );

      // Save the name of the window just created and then position back
      // to the parent window.
      GetStringFromAttribute( szSubWindowTag, zsizeof( szSubWindowTag ), vNewDialogRoot, "Window", "Tag" );

      DropView( vUPD_UserSpec );
      DropView( vNewDialogRoot );
      CreateViewFromViewForTask( &vNewDialogRoot, vParentRoot, 0 );

      // Now we're back to the original parent window and Control.

      // Add the Update button back on the parent.

      ////////
      // Update
      ////////

      lButtonNbr++;
      GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                              vDefaults, "InitValues", "DetailUpdate" );

      // SubAction is create temporal version.
      lSubAction = zSubActionCreateTemporalVer;

      oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                       vDefaults, vObject,
                                       szButtonText, 0, lButtonNbr, "B",
                                       zWAB_StartModalSubwindow,
                                       lSubAction, "" );
      // Now exclude the wrong LOD_Entity and include the correct one,
      // which is up one UIS_Entity higher in the UserSpec.
      ExcludeEntity( vNewDialogRoot, "ActMapLOD_Entity", zREPOS_NONE );
      CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
      ResetViewFromSubobject( vTemp );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                     vTemp,      "LOD_Entity", zPOS_AFTER );
      DropView( vTemp );

      // Set auto disable Action for null entity.
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 8 );

      // Set the Dialog and Window names in the Action.
      SetAttributeFromAttribute( vNewDialogRoot, "Action", "DialogName",
                                 vNewDialogRoot, "Dialog", "Tag" );
      SetAttributeFromString( vNewDialogRoot, "Action", "WindowName", szSubWindowTag );

      // Also include the same Action under a double click window event
      // for the ListBox.
      oTZWDLGSO_AddActionDoubleClick( vSubtask, vNewDialog, vNewDialogRoot );
   }

   /////////
   // Delete
   /////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ),
                           vDefaults, "InitValues", "Delete" );

   oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                    vDefaults, vObject,
                                    szButtonText, 0, lButtonNbr, "B",
                                    zWAB_StayOnWindowWithRefresh,
                                    zSubActionDelete, "" );

   // If this is Case 4, we need to exclude the LOD_Entity just added under
   // the CtrlMap entity and reinclude its parent in the LOD.  This is
   // because a delete for Case 4 should delete the Associative entity, not
   // the entity currently being processed.
   if ( szCase[ 0 ] == '4' )
   {
      ExcludeEntity( vNewDialogRoot, "ActMapLOD_Entity", zREPOS_NONE );
      CreateViewFromViewForTask( &vTemp, vUSSubEnt, 0 );
      ResetViewFromSubobject( vTemp );
      IncludeSubobjectFromSubobject( vNewDialogRoot, "ActMapLOD_Entity",
                                    vTemp,      "LOD_Entity", zPOS_AFTER );
      DropView( vTemp );
   }

   // If this is Case 3, we need to alter the SubAction from Delete to
   // Exclude.
   if ( szCase[ 0 ] == '3' )
   {
      SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_SubAction", zSubActionExclude );
   }

   // Set auto disable Action for null entity.
   SetAttributeFromInteger( vNewDialogRoot, "Action", "SB_ActionDisabling", 8 );

   // Recalculate the Groupbox size based on the buttons just added and
   // set it.
   oTZWDLGSO_SetGroupboxSize( vNewDialog, vDefaults );

   DropView( vNewDialogRoot );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignListQual
//
// PURPOSE: This is the routine that creates the major window for the
//          selection of entries to be included in the parent window.
//          It is called from 2Group, which processes the group on the
//          parent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZADCSDO_AutoDesignListQual( zVIEW  vSubtask,
                              zVIEW  vUSSubEnt,
                              zVIEW  vNewDialog,
                              zVIEW  vDefaults,
                              zVIEW  vObject,
                              zPCHAR szActivateOperation )
{
   zVIEW    vAutoDesWk;
   zVIEW    vNewDialogRoot;
   zVIEW    vPE;
   zCHAR    szButtonText[ 21 ];
   zCHAR    szPrompt[ 51 ];
   zLONG    lButtonNbr;
   zLONG    lPromptLength;
   zLONG    lPosX;
   zLONG    lPosY;
   zLONG    lSizeX;
   zLONG    lSizeY;
   zLONG    lInputSizeX = 100;

   // List Qual window:
   //   o Add detail to the new subordinate window.
   //   o Add the Prompt and Qual input field.
   //   o Add the OK and Cancel buttons.


   // Initial housekeeping.
   GetViewByName( &vAutoDesWk, "TZADWWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );

   // Fill out the window information.
   ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUSSubEnt, vNewDialog, vAutoDesWk );

   // Create a group to hold the controls below.
   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", "Group1" );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "GroupBox", 0 );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 5 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 5 );
   // Set the Subtype so the first edit box inside will have cursor position.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 537395200 );

   CreateViewFromViewForTask( &vNewDialogRoot, vNewDialog, 0 );

   // Set view down a level to put controls on Groupbox.
   SetViewToSubobject( vNewDialog, "CtrlCtrl" );

   // Add the Prompt field.
   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "Text", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", "ListQualPrompt" );
   GetStringFromAttribute( szPrompt, zsizeof( szPrompt ), vUSSubEnt, "WndDesign", "ListQualWndPrompt" );
   SetAttributeFromString( vNewDialog, "Control", "Text", szPrompt );
   lPromptLength = zstrlen( szPrompt ) * 4;
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 10 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 16 );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lPromptLength );
   SetAttributeFromAttribute( vNewDialog, "Control",    "SZDLG_Y",
                              vDefaults,  "InitValues", "EditboxHeight" );
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype",
                            zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );

   // Add the Input field.
   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vPE, "ControlDef", "Tag", "EditBox", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vPE,        "ControlDef", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", "ListQual" );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X",
                            15 + lPromptLength );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 16 );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lInputSizeX );
   SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 1 );
   SetAttributeFromAttribute( vNewDialog, "Control",    "SZDLG_Y",
                              vDefaults,  "InitValues", "EditboxHeight" );

   ResetViewFromSubobject( vNewDialog );

   // Add the OK and Cancel buttons.

   lButtonNbr = 0;

   ////////
   // OK
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vUSSubEnt, "WndDesign", "ListQualWndButtonText" );
   oTZADCSDO_CreateButtonActOper( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                  vDefaults, szButtonText, "Alt-F4",
                                  szActivateOperation, lButtonNbr, "S",
                                  zWAB_ReturnToParentWithRefresh );
   SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 2 );
   //Reset button position X.
   lPosX = lPromptLength + lInputSizeX + 25;
   SetCursorLastEntity( vNewDialog, "CtrlCtrl", 0 );
   SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "PSDLG_X", lPosX );


   ////////
   // Cancel
   ////////

   lButtonNbr++;
   GetStringFromAttribute( szButtonText, zsizeof( szButtonText ), vDefaults, "InitValues", "CancelOnInclude" );
   oTZADCSDO_CreateButtonActObject( vSubtask, vUSSubEnt, vNewDialog, vNewDialogRoot,
                                    vDefaults, vObject,
                                    szButtonText, "ESC", lButtonNbr, "S",
                                    zWAB_ReturnToParent, 0, "" );
   SetAttributeFromInteger( vNewDialog, "Control", "SyncKey", 3 );
   //Reset button position X.
   lPosX = lPromptLength + lInputSizeX + 25;
   SetCursorLastEntity( vNewDialog, "CtrlCtrl", 0 );
   SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "PSDLG_X", lPosX );

   // Also set NoMap for Cancel.
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   // Set Window Caption.
   SetAttributeFromAttribute( vNewDialog, "Window",    "Caption",
                              vUSSubEnt,  "WndDesign", "ListQualWndCaption" );

   // Set Groupbox size, which is lower right corner of Cancel button + 5.
   GetIntegerFromAttribute( &lPosX, vNewDialog, "CtrlCtrl", "PSDLG_X" );
   GetIntegerFromAttribute( &lPosY, vNewDialog, "CtrlCtrl", "PSDLG_Y" );
   GetIntegerFromAttribute( &lSizeX, vNewDialog, "CtrlCtrl", "SZDLG_X" );
   GetIntegerFromAttribute( &lSizeY, vNewDialog, "CtrlCtrl", "SZDLG_Y" );
   lSizeX = lPosX + lSizeX + 5;
   lSizeY = lPosY + lSizeY + 5;
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lSizeX );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lSizeY );

   // Set the window size.
   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSizeY, &lSizeX );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSizeX + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSizeY + 31 );

   // Now create the DIL if it was requested.
   ofnTZWDLGSO_CreateDIL( vSubtask, vNewDialog, vDefaults );

   DropView( vNewDialogRoot );
   return( 0 );
}
