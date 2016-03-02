/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw4aw.c - Zeidon application operations
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
// LAST MESSAGE ID: AD10305
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.01.13  BL
   set comments for unreferenced local variable

1999/04/24  DC
   Modified ofnTZWDLGSO_AutoPaintListBox and ofnTZWDLGSO_AutoPaintListPart
   to paint new list box.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "zdrvropr.h"
#include "tzctlopr.h"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////
//
// PURPOSE:  This routine paints selected fields in a list box
// or partition (all the fields are on one line) and paints the
// field literals above the listbox or partition.
//
/////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintListBox( zVIEW  vSubtask,
                              zVIEW  vUserSpec,
                              zVIEW  vNewDialog,
                              zPCHAR szListType )
{
   zLONG    lCC;
   zLONG    lCL;
   zSHORT   nRC;
   zLONG    lFieldRow;
   zLONG    lFieldCol;
   zLONG    lPartitionStartDLG_Y;
   zLONG    lPartitionStartDLG_X;
   zLONG    lPartitionSizeDLG_X;
   zLONG    lSubtype;
   zCHAR    szParentName[ 33 ];
   zLONG    lTempInteger_0;
   zSHORT   RESULT;
//BL   zLONG    lTempInteger_1;
   zCHAR    X_LOD_Attribute[ 16 ];

// TraceLineS("IN", "AutoPaintListBox W4" );

   // Set up variables to either I_LOD listbox or L_LOD listbox.
   if ( szListType[ 0 ] == 'I' )
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "I_LOD_Attribute" );
   else
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "L_LOD_Attribute" );

   lCL = 12;
   lCC = 5;
   strcpy_s( szParentName, zsizeof( szParentName ), "Window" );
   lTempInteger_0  = zstrcmp( szParentName, "SECTION" );
   // If we are Autodesigning a ROD, set the lCL to 8 so line skips do not tak place.
   if ( lTempInteger_0 == 0 )
   {
      lCL = 8;
   }
   else
   {
      lCL = 12;
   }

   // Set the Tag
   ofnTZWDLGSO_AssignTag( vSubtask, vNewDialog, vNewDialog, "ListBox" );
// SetAttributeFromAttribute( vNewDialog, "Control", "Tag",
//                            vUserSpec, "LOD_Entity", "Name" );

   /* Get the group's dimensions. */
   GetIntegerFromAttribute( &lPartitionStartDLG_Y, vNewDialog,
                            "Control", "PSDLG_Y" );
   GetIntegerFromAttribute( &lPartitionStartDLG_X, vNewDialog,
                            "Control", "PSDLG_X" );
   GetIntegerFromAttribute( &lPartitionSizeDLG_X, vNewDialog,
                            "Control", "SZDLG_X" );

   /* If the control is a group box not a listbox, then the output
      fields should be placed lower down then the listbox's output fields. */
   if ( CompareAttributeToString( vNewDialog, "ControlDef", "Tag",
                                  "ListBox" ) == 0 )
      lFieldRow = 2;
   else
      lFieldRow = 5;
   lFieldCol = 1;

   RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );

   /* Paint the output fields into the listbox or partition. */
   nRC = ofnTZWDLGSO_AutoPaintListPart( vSubtask, vUserSpec, vNewDialog,
                                        lFieldRow, lFieldCol,
                                        lPartitionSizeDLG_X, szListType );

#if 0
   //* Eliminated 1999/04/23 for support of new listbox style.
   /* If the fields fit and there was no other problem, paint the lits. */
   if ( nRC > -1 )
   {
      /* The literals are going to be placed above the listbox
         or partition. */
      lFieldRow = lPartitionStartDLG_Y - nCL;
      lFieldCol = lPartitionStartDLG_X + nCC;

      RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );

      /* Paint the literals above the listbox or partition. */
      lTempInteger_1 = zstrcmp( szParentName, "Window" );
      if ( lTempInteger_1 == 0 )
      {
         nRC = ofnTZWDLGSO_AutoPaintLstLitPart( vUserSpec, vNewDialog,
                                                lFieldRow, lFieldCol,
                                                lPartitionSizeDLG_X,
                                                szListType );
         if ( nRC == -1 )
            return( -1 );
      }
   }
   else
   {
      return( -1 );
   }
#endif

   // Set listbox to sort and disable first subcontrol.
   GetIntegerFromAttribute( &lSubtype, vNewDialog, "Control", "Subtype" );
   lSubtype |= zLISTBOX_SORTEDHEADERS;
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", lSubtype );
   SetCursorFirstEntity( vNewDialog, "CtrlCtrl", 0 );
   GetIntegerFromAttribute( &lSubtype, vNewDialog, "CtrlCtrl", "Subtype" );
   lSubtype |= zCONTROL_DISABLED;
   SetAttributeFromInteger( vNewDialog, "CtrlCtrl", "Subtype", lSubtype );

// TraceLineS("END", "AutoPaintListBox W4" );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// PURPOSE: This function places the paint literals of selected
// attributes above the fields in a partition or list box.
//
// vNewDialog - handle to the group
// OffsetDLG_Y - offset row to the group
// OffsetDLG_X - offset column to the group
// PartitionSize - length size of the group
//
// Return Codes:
//     0 - Everything is O.K. and the literals were placed into the
//         partition
//    -1 - The literal widths exceeded the partitions width
//
////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintLstLitPart( zVIEW    vSubtask,
                                 zVIEW    vUserSpec,
                                 zVIEW    vNewDialog,
                                 zLONG    OffsetDLG_Y,
                                 zLONG    OffsetDLG_X,
                                 zLONG    PartitionSize,
                                 zPCHAR   szListType )
{
   zVIEW    vCtrlDef;
   zLONG    lCC;
   zLONG    lCL;
   zLONG    lFieldCol;
   zLONG    lListAttributeLength;
   zLONG    lPaintLiteralLength;
   zCHAR    szPaintLiteral[ 65 ];
   zSHORT   RESULT;
   zLONG    lTempInteger_1;
   zLONG    lTempInteger_2;
   zCHAR    X_LOD_Attribute[ 16 ];
   zCHAR    X_ER_Attribute[ 15 ];

// TraceLineS("IN", "AutoPaintLstListPart W4" );

   // Set up variables to either I_LOD listbox or L_LOD listbox.
   if ( szListType[ 0 ] == 'I' )
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "I_LOD_Attribute" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "I_ER_Attribute" );
   }
   else
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "L_LOD_Attribute" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "L_ER_Attribute" );
   }

   lCC = 5;
   lCL = 12;

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );

   lFieldCol = 1;

   RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      // This gets the length of the field to be painted
      lListAttributeLength = ofnTZWDLGSO_GetSizeForListAttr( vSubtask, vUserSpec, szListType );

      lListAttributeLength = lListAttributeLength * lCC;
      // Limit the size of the attribute length.
      if ( lListAttributeLength > 100 )
         lListAttributeLength = 100;
      if ( lListAttributeLength < 25 )
         lListAttributeLength = 25;

      // Get the paint literal
      GetStringFromAttribute( szPaintLiteral, zsizeof( szPaintLiteral ), vUserSpec,
                               X_ER_Attribute, "ListPaintLit" );
      if ( szPaintLiteral[ 0 ] == 0 )
         GetStringFromAttribute( szPaintLiteral, zsizeof( szPaintLiteral ), vUserSpec,
                                 X_ER_Attribute, "PromptPaintLit" );
      if ( szPaintLiteral[ 0 ] == 0 )
      {
         GetStringFromAttribute( szPaintLiteral, zsizeof( szPaintLiteral ), vUserSpec,
                                  X_ER_Attribute, "Name" );
         // Add a ':' to the end.
         strcat_s( szPaintLiteral, zsizeof( szPaintLiteral ), ":" );
         // Add spaces before any capital letters.
         AddSpacesToString( szPaintLiteral );
      }

      lPaintLiteralLength = zstrlen( szPaintLiteral );
      lPaintLiteralLength = lPaintLiteralLength * lCC;
      // Limit the size of the literal length.
      if ( lPaintLiteralLength > 100 )
         lPaintLiteralLength = 100;
      if ( lPaintLiteralLength < 25 )
         lPaintLiteralLength = 25;

      /*
      lTempInteger_1 = CheckForOverlappingFields( vNewDialog, OffsetDLG_Y,
                                                   OffsetDLG_Y + nCL,
                                                   lFieldCol + OffsetDLG_X - lCC, lFieldCol + OffsetDLG_X + lPaintLiteralLength - lCC );
      */

      lTempInteger_1 = 0;
      // Check to see IF there is a field or group located above
      // the listbox that would be in the way of painting the literals
      if ( lTempInteger_1 == -1 )
      {
         // Cannot place text fields above the listbox because of either a field or a group
         // InformationMessage( "Field literals can't fit because of FIELD or GROUP" );
         MessageSend( vUserSpec, "AD10301", "Autodesigner",
                      "Field literals can't fit because of FIELD or GROUP",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         return( -1 );
      }

      // Add the listbox literals at the end of the client fields.
      // This makes a difference in fnCompressWindow in autodesigning.
      SetCursorLastEntity( vNewDialog, "Control", "" );

      // Do not continue to add fields into the list box if the field sizes exceed the size of the list box
      // If the field attribute length is greater than the text literal length, then add the field attribute length to lFieldCol.
      if ( lPaintLiteralLength < lListAttributeLength )
      {
         // If the length of lFieldCol is greater than the partition size, then RETURN
         if ( lFieldCol + lListAttributeLength > PartitionSize )
         {
            return( 1 );
         }
      }
      else
      {
         // If the length of lFieldCol is greater than the partition size, then RETURN
         if ( lFieldCol + lPaintLiteralLength > PartitionSize )
         {
            return( 1 );
         }
      }

      // Create literals for the list box
      CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
      SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
      IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );

      // Remove the text border.
      SetAttributeFromInteger( vNewDialog, "Control", "Subtype", zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", OffsetDLG_Y );
      lTempInteger_2 = OffsetDLG_X + lFieldCol - lCC;
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lTempInteger_2 );
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lPaintLiteralLength );
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", 10 );

      ofnTZWDLGSO_AssignTag( vSubtask, vNewDialog, vNewDialog, "ListText" );
      SetAttributeFromString( vNewDialog, "Control", "Text", szPaintLiteral );

      // If the field length is longer than the paint literal, then position the next field by using the field length,
      // otherwise position the next field by using the paint literal length
      if ( lPaintLiteralLength < lListAttributeLength )
      {
         lFieldCol = lFieldCol + lListAttributeLength + lCC;
      }
      else
      {
         lFieldCol = lFieldCol + lPaintLiteralLength + lCC;
      }
      RESULT = SetCursorNextEntity( vUserSpec, X_LOD_Attribute, "" );
   }
// TraceLineS("END", "AutoPaintLstListPart W4" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// PURPOSE: This function places selected attributes in an already
// existing partition without the attributes text literals.
//
// vNewDialog - handle to the partition
// lFieldRow - offset row in the group
// lFieldCol - offset column in the group
// PartitionSize - length size for the group
//
// Return Codes:
//     0 - Everything is O.K. and the fields were placed into the
//         partition
//    -1 - The field widths exceeded the partitions width
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AutoPaintListPart( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zLONG    lFieldRow,
                               zLONG    lFieldCol,
                               zLONG    PartitionSize,
                               zPCHAR   szListType )
{
   zVIEW    vAutoDesWk;
   zVIEW    vTempStyle;
   zVIEW    vSubobject;
   zVIEW    vCtrlDef;
   zLONG    lListAttributeLength;
   zLONG    lPaintLiteralLength;
   zLONG    lCC;
   zLONG    lCL;
   zCHAR    szControlType[ 33 ];
   zCHAR    szMapEntity[ 33 ];
   zCHAR    szParentName[ 33 ];
   zCHAR    szWorkString[ 65 ];
   zCHAR    szLiteralString[ 65 ];
   zLONG    lTempInteger_0;
   zSHORT   RESULT;
   zSHORT   lFieldsPainted;
   zLONG    lRC;
   zCHAR    X_LOD_Attribute[ 16 ];
   zCHAR    X_LOD_Entity[ 13 ];
   zCHAR    X_ER_Attribute[ 15 ];

// TraceLineS( "IN", "AutoPaintListPart W4" );

   // Set up variables to either I_LOD listbox or L_LOD listbox.
   if ( szListType[ 0 ] == 'I' )
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "I_LOD_Attribute" );
      strcpy_s( X_LOD_Entity, zsizeof( X_LOD_Entity ), "I_LOD_Entity" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "I_ER_Attribute" );
   }
   else
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "L_LOD_Attribute" );
      strcpy_s( X_LOD_Entity, zsizeof( X_LOD_Entity ), "L_LOD_Entity" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "L_ER_Attribute" );
   }

   lCL = 12;
   lCC = 5;

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vNewDialog, zLEVEL_TASK );
   GetStringFromAttribute( szControlType, zsizeof( szControlType ), vNewDialog, "ControlDef", "Tag" );

   // It is important to know whether the parent is a WINDOW (WDODs) or a SECTION(RODs).
   strcpy_s( szParentName, zsizeof( szParentName ), "Window" );

   // IF we are Autodesigning a ROD, set lCL to 8 so line skips do not take place
   lTempInteger_0  = zstrcmp( szParentName, "SECTION" );
   if ( lTempInteger_0 == 0 )
   {
      lCL = 8;
   }

   // Create a view that will point to the controls under the listbox.
   CreateViewFromViewForTask( &vSubobject, vNewDialog, 0 );

   // Set handle to the fields under the group
   SetViewToSubobject( vSubobject, "CtrlCtrl" );

   // Delete any existing fields in the listbox
   RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      RESULT = DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
   }

   // Create a new view that will point to the UIS_ViewObjRef entity in the user spec object.
   // Sometimes vUserSpec is pointing to a recursive subobject and UI_ViewObjRef cannot be accessed.
   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );
   ResetView( vTempStyle );

   lFieldsPainted = 0;

   //// Paint the fields in the Partition //////////
   // IF Attributes have been found to paint, paint them
   RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      // This gets the length of the field to be painted.
      lListAttributeLength = ofnTZWDLGSO_GetSizeForListAttr( vSubtask, vUserSpec, szListType );

      if ( lListAttributeLength == 0 )
         lListAttributeLength = 4;

      lListAttributeLength = lListAttributeLength * lCC;
      if ( lListAttributeLength > 100 )
         lListAttributeLength = 100;

      if ( lListAttributeLength < 25 )
         lListAttributeLength = 25;

      // If there isn't a paint literal then use the entity name as the paint literal
      GetStringFromAttribute( szLiteralString, zsizeof( szLiteralString ), vUserSpec, X_ER_Attribute, "ListPaintLit" );
      if ( szLiteralString[ 0 ] == 0 )
         GetStringFromAttribute( szLiteralString, zsizeof( szLiteralString ), vUserSpec, X_ER_Attribute, "PromptPaintLit" );
      lPaintLiteralLength = zstrlen( szLiteralString );
      if ( szLiteralString[ 0 ] == 0 )
      {
         GetStringFromAttribute( szLiteralString, zsizeof( szLiteralString ), vUserSpec, X_ER_Attribute, "Name" );
         AddSpacesToString( szLiteralString );
         lPaintLiteralLength = zstrlen( szLiteralString ) + 1;
      }

      lPaintLiteralLength = lPaintLiteralLength * lCC;
      if ( lPaintLiteralLength > 100 )
         lPaintLiteralLength = 100;

      if ( lPaintLiteralLength < 25 )
         lPaintLiteralLength = 25;

      // Do not continue to add fields into the partition if the field sizes exceed the size of the partition
      // If the field attribute length is greater than the text literal length, then add the field attribute length to lFieldCol.
      if ( lPaintLiteralLength < lListAttributeLength )
      {
         // If the length of lFieldCol is greater than the partition size, then delete all the fields that
         // have been added and give a message to the user.
         if ( lFieldCol + lListAttributeLength > PartitionSize )
         {
            RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
               RESULT = SetCursorNextEntity( vSubobject, "Control", "" );
            }
            DropView( vSubobject );
            MessageSend( vUserSpec, "AD10302", "Autodesigner",
                         "Partition too small( width )",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -1 );
         }
      }
      else
      {
      // If the text literal length is greater than the field attribute length, then add the text literal length to lFieldCol.

         // If the length of lFieldCol is greater than the partition size, then delete all the fields that have been added and give a message to the user.
         if ( lFieldCol + lPaintLiteralLength > PartitionSize )
         {
            RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
               RESULT = SetCursorNextEntity( vSubobject, "Control", "" );
            }
            DropView( vSubobject );
            MessageSend( vUserSpec, "AD10303", "Autodesigner",
                         "Partition too small( width )",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            return( -1 );
         }
      }

      // Create the partition's fields
      CreateMetaEntity( vSubtask, vSubobject, "Control", zPOS_AFTER );
      SetAttributeFromString( vSubobject, "Control", "Text", szLiteralString );
      if ( zstrcmp( szControlType, "ListBox" ) == 0 )
      {
         SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "ListBoxCtl", "" );
         IncludeSubobjectFromSubobject( vSubobject, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
      }
      else
      {
         SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
         IncludeSubobjectFromSubobject( vSubobject, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
      }
      // Set the tag
      ofnTZWDLGSO_AssignTag( vSubtask, vNewDialog, vSubobject, "ListCtrl" );
/*    GetStringFromAttribute( szTag, zsizeof( szTag ), vUserSpec, X_ER_Attribute, "Name" );
      szTag[ 30 ] = 0;

      lFieldsPainted = lFieldsPainted + 1;
      zltoa( lFieldsPainted, szWork, zsizeof( szWork ) );
      strcat_s( szTag, zsizeof( szTag ), szWork );
      SetAttributeFromString( vSubobject, "Control", "Tag", szTag );
*/
      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_X", lListAttributeLength );
      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_Y",  lCL );

      // If the domain is an integer or a long, then right justify.
      /*
      IF vUserSpec.L_Domain.DerivedDataType = 'I' OR
         vUserSpec.L_Domain.DerivedDataType = 'L' OR
         vUserSpec.L_Domain.DerivedDataType = 'M'
         vNewDialog.FLAGFLD.RJUST = 'Y'
      END
      */

      GetStringFromAttribute( szMapEntity, zsizeof( szMapEntity ), vUserSpec, "LOD_Entity", "Name" );
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, X_LOD_Entity, "Name" );
      // If this LISTATTRIB is from a different ENTITY than the current ENTITY, then make sure the value gets mapped.
      lRC = zstrcmp( szWorkString, "" );
      if ( lRC != 0 )
      {
         lRC = zstrcmp( szWorkString, szMapEntity );
         if ( lRC == 0 )
         {
            strcpy_s( szMapEntity, zsizeof( szMapEntity ), szWorkString );
         }
      }

      // setting the group and field's ENAME and ANAME
      CreateMetaEntity( vSubtask, vSubobject, "CtrlMap", zPOS_AFTER );

      // set the partitions OI name
      if ( szListType[ 0 ] == 'I' )
         IncludeSubobjectFromSubobject( vSubobject, "CtrlMapView", vUserSpec, "ListViewObjRef", zPOS_AFTER );
      else
         IncludeSubobjectFromSubobject( vSubobject, "CtrlMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );

      IncludeSubobjectFromSubobject( vSubobject, "CtrlMapLOD_Attribute", vUserSpec, X_LOD_Attribute, zPOS_AFTER );

      // lFieldRow should always stay the same
      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_X", lFieldCol );
      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_Y", lFieldRow );

      // If the field length is longer than the paint literal, then position the next field
      // by using the field length, otherwise position the next field by using the paint literal length
      if ( lPaintLiteralLength < lListAttributeLength )
      {
         lFieldCol = lFieldCol + lListAttributeLength + lCC;
      }
      else
      {
         lFieldCol = lFieldCol + lPaintLiteralLength + lCC;
      }
      RESULT = SetCursorNextEntity( vUserSpec, X_LOD_Attribute, "" );
   }

   DropView( vSubobject );
   DropView( vTempStyle );
// TraceLineS("END", "AutoPaintListPart W4" );
   return( 0 );
}

///////////////////////////////////////////////////////////////////////
//
// PURPOSE:  This function paints the list attributes from the user spec
//           into the existing partition.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_AutoPntFldPartWLits( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zVIEW  vNewDialog,
                                 zPCHAR szListType )
{
   zVIEW  vAutoDesWk;
   zVIEW  vSubobject;
   zVIEW  vTempStyle;
   zVIEW  vCtrlDef;
   zLONG  k, j;
   zLONG  lCC, lCL;
   zLONG  lMaxLines;
   zLONG  lLine, lFirstPaintLine, lFieldsPainted, lColumn;
   zLONG  lMaxFieldDLG_X, lMaxText, lPush;
   zLONG  lGroupDLG_X, lGroupDLG_Y;
   zSHORT RESULT;
   zLONG  lLastPaintLine, lLastWindowLine, lMaxLength;
// zCHAR  cUsage[ 2 ];
   zCHAR  szMapEntity[ 33 ];
   zCHAR  szMapAttribute[ 33 ];
   zCHAR  szType[ 33 ];
   zCHAR  szWorkString[ 256 ];
   zCHAR  X_LOD_Attribute[ 16 ];
   zCHAR  X_ER_Attribute[ 15 ];

// TraceLineS( "IN", " AutoPaintPartitionWithFields" );

   // Set up variables to either I_LOD listbox or L_LOD listbox.
   if ( szListType[ 0 ] == 'I' )
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "I_LOD_Attribute" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "I_ER_Attribute" );
   }
   else
   {
      strcpy_s( X_LOD_Attribute, zsizeof( X_LOD_Attribute ), "L_LOD_Attribute" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "L_ER_Attribute" );
   }

// TraceLineS( "IN", " AutoPaintPartitionWithFields" );

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   GetViewByName( &vAutoDesWk, "TZADWWKO", vNewDialog, zLEVEL_TASK );
   CreateViewFromViewForTask( &vSubobject, vNewDialog, 0 );

   lCL = 12;
   lCC = 5;

   /* Create a new view that will point to the UIS_ViewObjRef entity
      in the user spec object.  Sometimes vUserSpec is pointing to a
      recursive subobject and UI_ViewObjRef cannot be accessed. */
   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );
   ResetView( vTempStyle );

   SetViewToSubobject( vSubobject, "CtrlCtrl" );
   /* Delete any existing fields in the listbox */
   RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      RESULT = DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
   }

   GetIntegerFromAttribute( &lMaxLines, vNewDialog, "Control", "SZDLG_Y" );
   szMapEntity[ 0 ] = 0;

   lFirstPaintLine = 0;
   lLastPaintLine = 1;
   lLine          = 1;
   lMaxFieldDLG_X = 0;
   lMaxLength     = 0;
   lMaxText       = 0;
   lPush          = 0;
   lFieldsPainted = 0;

// cUsage[ 0 ] = 'U';

   /* If there is a paint object, loop through its attributes */
   RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      /* The last line painted was at this posititon */
      lLastPaintLine = lLine;
      lColumn = 1;

      /* Create the Text Field for the Attribute */
      CreateMetaEntity( vSubtask, vSubobject, "Control", zPOS_AFTER );
      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_Y", lLine );
      if ( lFirstPaintLine == 0 )
         lFirstPaintLine = lLine;

      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_X", lColumn );
      /* Creating text field. */
      SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
      IncludeSubobjectFromSubobject( vSubobject, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
      // Remove the text border.
      SetAttributeFromInteger( vSubobject, "Control", "Subtype",
                               zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, X_ER_Attribute, "ListPaintLit" );
      GetStringFromAttribute( szMapAttribute, zsizeof( szMapAttribute ), vUserSpec, X_ER_Attribute, "Name" );

      // If there is no paint literal for this attribute then
      // use the attribute name as the paint literal.
      if ( szWorkString[ 0 ] == 0 )
         strcpy_s( szWorkString, zsizeof( szWorkString ), szMapAttribute );

      SetAttributeFromString( vSubobject, "Control", "Text", szWorkString );

      j = zstrlen( szWorkString );
      j = j * lCC;

      // Determine the size of the longest paint literal.
      if ( j > lMaxText )
         lMaxText = j;

      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_X", j );
      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_Y", lCL );

      /* Add the length of the longest paint literal
         to the column so the attribute field will be
         painted there */
      lColumn += j;
      lColumn = lColumn + lCC;

      // If there is no paint length for the field, then
      // use the field's length.
      j = ofnTZWDLGSO_GetSizeForListAttr( vSubtask, vUserSpec, szListType );

      j = j * lCC;

      // If there is no paint length for the field, then
      // use the field's length.
      if ( j > 100 )
         j = 100;
      else
      if ( j < 25 )
         j = 25;

      // Create the appropriate Data Field for the Attribute.
      CreateMetaEntity( vSubtask, vSubobject, "Control", zPOS_AFTER );

      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_Y",  lLine );
      SetAttributeFromInteger( vSubobject, "Control", "PSDLG_X", lColumn );
      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_X", j );
      SetAttributeFromInteger( vSubobject, "Control", "SZDLG_Y", lCL );
      // Determine the maximum column length.
      if ( lMaxFieldDLG_X < lColumn )
         lMaxFieldDLG_X = lColumn;

      lColumn += j;
      lColumn = lColumn - 1 * lCC;

      // Determine the maximum length of colum and literal.
      if ( lMaxLength < lColumn )
         lMaxLength = lColumn;

      SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "EditBox", "" );
      IncludeSubobjectFromSubobject( vSubobject, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
   // SetAttributeFromInteger( vSubobject, "Control", "Type", 1010 );
      /*
      if ( cUsage[ 0 ] == 'U' )
      {
         GetStringFromAttribute( cWork, vUserSpec,
                                 X_LOD_Attribute, "DERIVEDF" );
         if ( cWork[ 0 ] != 0 )
            SetAttributeFromCharacter( vSubobject,
                                        "Control", "Type", 1000 );
      }
      */
      /* setting the group and field's ENAME and ANAME */
      CreateMetaEntity( vSubtask, vSubobject, "CtrlMap", zPOS_AFTER );

      /* set the partitions OI name */
      IncludeSubobjectFromSubobject( vSubobject, "CtrlMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vSubobject, "CtrlMapLOD_Attribute", vUserSpec, X_LOD_Attribute, zPOS_AFTER );
      lFieldsPainted++;
      lLine = lLine + lCL;

      RESULT = SetCursorNextEntity( vUserSpec, X_LOD_Attribute, "" );
   }

   lLastPaintLine = lLine;

   /* Get the dimensions of the partition. */
   GetIntegerFromAttribute( &lGroupDLG_X, vNewDialog, "Control", "SZDLG_X" );
   GetIntegerFromAttribute( &lGroupDLG_Y, vNewDialog, "Control", "SZDLG_Y" );

   RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lLine, vSubobject, "Control", "PSDLG_Y" );
      if ( lLine >= lFirstPaintLine && lLine <= lLastPaintLine )
      {
         GetStringFromAttribute( szType, zsizeof( szType ), vSubobject, "ControlDef", "Tag" );
         /* If the control is not a text control. */
         if ( zstrcmp( szType, "Text" ) != 0 )
         {
            GetIntegerFromAttribute( &j, vSubobject, "Control", "PSDLG_X" );
            GetIntegerFromAttribute( &k, vSubobject, "Control", "SZDLG_X" );

            if ( j < lMaxFieldDLG_X )
            {
               j = lMaxFieldDLG_X - j;
               if ( ( lMaxFieldDLG_X + k ) >
                    ( lMaxLength + lCC ) )
               {
                  if ( ( lMaxFieldDLG_X + k ) <= lGroupDLG_X )
                  {
                     lMaxLength = lMaxFieldDLG_X +( k - lCC );
                  }
                  else
                  {
                     j -=( ( lMaxFieldDLG_X + k ) -
                                          lGroupDLG_X );
                     if ( j > 0 )
                     {
                        AddToAttributeFromInteger( vSubobject, "Control", "PSDLG_X", j );
                        lMaxLength = lGroupDLG_X - lCC;
                        j = -1;
                     }
                  }
               }

               if ( j > 0 )
               {
                  SetAttributeFromInteger( vSubobject, "Control", "PSDLG_X", lMaxFieldDLG_X );
               }
            }
         }

         GetIntegerFromAttribute( &j, vSubobject, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &k, vSubobject, "Control", "SZDLG_X" );
         if ( ( j + k ) >= lGroupDLG_X )
         {
            RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
            while ( RESULT > zCURSOR_UNCHANGED )
            {
               RESULT = DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
            }

            MessageSend( vNewDialog, "AD10304", "Autodesigner",
                         "Partition too small (width)",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

            /* the partition is not wide enough */
            return( -1 );
         }

         ///////////////////////////////////////////////////////////
         // Code to truncate fields which extend over the partition to
         // the right COMMENTED out. This is a 'too small' error as
         // noted above.
         // else
         // if ( (j + k) >= lGroupDLG_X )
         // {
         //    k =( lGroupDLG_X - 1 ) - j;
         //    SetAttributeFromInteger( vSubobject, "Control",
         //                             "SZDLG_X", k );
         // }
         ////////////////////////////////////////////////////////////
      }

      RESULT = SetCursorNextEntity( vSubobject, "Control", "" );
   }

   lLastWindowLine = lLine;

   /* If the window painted has too many lines for the
      terminal being used for painting. Attempt to
      compress the window into a more manageable size by combining
      painted fields which are located on separate lines            */

   /*
   for ( i = 1; i < 8; i++ )
   {
      if ( lLastWindowLine + 12 <= lMaxLines )
      {
         break;
      }
      fnCompressPartition( vNewDialog, i, lFirstPaintLine, lMaxText,
                           lMaxLines,
                           &lLastPaintLine, &lLastWindowLine,
                           &lMaxLength );
   }
   */

   if ( ( lLastWindowLine + lCL ) > lMaxLines )
   {
      RESULT = SetCursorFirstEntity( vSubobject, "Control", "" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         RESULT = DeleteEntity( vSubobject, "Control", zREPOS_NEXT );
      }

      MessageSend( vNewDialog, "AD10305", "Autodesigner",
                   "Partition too small( height )",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

// TraceLineS( "END", " AutoPaintPartitionWithFields" );

   /* Return the last paint line for AutoDesign */
   return( lLastPaintLine );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_AssignTag( zVIEW  vSubtask,
                       zVIEW  vDialog,
                       zVIEW  vControl,
                       zPCHAR szType )
{
   zVIEW vTmpDlg;
   zLONG lIdx;
   zSHORT nLth;
   zCHAR szTag[ 25 ];

   strcpy_s( szTag, zsizeof( szTag ), szType );
   nLth = (zSHORT) zstrlen( szTag );

   // Use a temp view for Dialog to make sure we are positioned on the
   // Window itself for getting ControlTagCounter and not on a subcontrol.
   CreateViewFromViewForTask( &vTmpDlg, vControl, 0 );
   ResetView( vTmpDlg );

   // Select next integer to make control tag unique.
   GetIntegerFromAttribute( &lIdx, vTmpDlg, "Window", "ControlTagCounter" );
   lIdx++;
   SetAttributeFromInteger( vTmpDlg, "Window", "ControlTagCounter", lIdx );
   zltoa( lIdx, szTag + nLth, zsizeof( szTag ) - nLth );
   DropView( vTmpDlg );

   SetAttributeFromString( vControl, "Control", "Tag", szTag );
   return( 0 );
}
