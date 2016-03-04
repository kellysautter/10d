/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadw3aw.c - Zeidon application operations
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
// LAST MESSAGE ID: AD10804
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

/*
CHANGE LOG

29.07.1998  DonC
   Eliminated unused operations.

*/

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: CalculateWindowSize
//
// PURPOSE: This function finds the coordinates of field or group
//          that is farthest down on the current window and the
//          field or group that is farthest to the right of the current
//          window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CalculateWindowSize( zVIEW vNewDialog,
   zPLONG    nLine,
   zPLONG    nMaxLength )
{
   zLONG    lFieldEnd;
   zLONG    i;
   zLONG    j;
   zSHORT   RESULT;

// TraceLineS("IN", "CalculateWindowSize W3" );
   /* Now go through the painted window and calculate the minimum
      size of the window in lines and columns. */
   *nLine = 0;
   *nMaxLength = 0;
   lFieldEnd = 0;
   RESULT = SetCursorFirstEntity( vNewDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &i, vNewDialog, "Control", "PSDLG_X" );
      GetIntegerFromAttribute( &j, vNewDialog, "Control", "SZDLG_X" );
      i = i + j;
      if ( i > *nMaxLength )
      {
         *nMaxLength = i;
      }
      GetIntegerFromAttribute( &i, vNewDialog, "Control", "PSDLG_Y" );
      GetIntegerFromAttribute( &j, vNewDialog, "Control", "SZDLG_Y" );
      lFieldEnd = i + j;
      if ( lFieldEnd > *nLine )
      {
         *nLine = lFieldEnd;
      }
      RESULT = SetCursorNextEntity( vNewDialog, "Control", "" );
   }
// TraceLineS("END", "CalculateWindowSize W3" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   CreateListBox
//
// PURPOSE: This function creates a listbox with the given
//          specifications and then autodesigns this listbox with
//          the LISTATTRIB attributes for the listbox's entity.
//          In addition to auto designing the listbox's fields, any
//          actions defined for the listbox(in the style object)
//          get autodesigned.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateListBox( zVIEW    vSubtask,
                           zVIEW    vNewDialog,
                           zVIEW    vUserSpec,
                           zLONG    lStart_X,
                           zLONG    lStart_Y,
                           zLONG    lHeight,
                           zPCHAR   szListType )
{
   zVIEW    vCtrlDef;
   zVIEW    vTZPNCTWO;
   zLONG    lTotalLth;
   zLONG    lAttribLth;
   zLONG    lPaintLth;
   zLONG    lCC;
   zLONG    lCL;
   zSHORT   RESULT;
   zCHAR    szPaintStr[ 50 ];
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

// TraceLineS("IN", "CreateListBox W3" );
   /* Create the listbox. */
   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );

   SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "ListBox", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vCtrlDef, "ControlDef", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lStart_X );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lStart_Y );

   // We must set the CtrlBOI.
   ActivateEmptyObjectInstance( &vTZPNCTWO, "TZPNCTWO",
                                vUserSpec, zSINGLE );
   CreateEntity( vTZPNCTWO, "TZPNCTWO", zPOS_AFTER );
   CreateEntity( vTZPNCTWO, "ListBox", zPOS_AFTER );
   SetAttributeFromInteger( vTZPNCTWO, "ListBox", "Subtype", 8192 );

   // Set Control.CtrlBOI from work entity GroupBox
   SetBlobFromEntityAttributes( vNewDialog, "Control",
                                 "CtrlBOI", vTZPNCTWO, "ListBox" );
   DropView( vTZPNCTWO );

   // Set the control subtype to 0(Single).
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 8192 );

   lTotalLth = 0;

   if ( CheckExistenceOfEntity ( vUserSpec, X_LOD_Attribute ) != 0 )
      lTotalLth = 5;

   // Loop through the L_LOD_Attribute and find the total width needed
   // for this listbox.
   RESULT = SetCursorFirstEntity( vUserSpec, X_LOD_Attribute, "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      lAttribLth = ofnTZWDLGSO_GetSizeForListAttr( vSubtask, vUserSpec,
                                                   szListType );

      if ( lAttribLth > 20 )
         lAttribLth = 20;

      if ( lAttribLth < 5 )
         lAttribLth = 5;

      GetStringFromAttribute( szPaintStr, zsizeof( szPaintStr ), vUserSpec,
                              X_ER_Attribute, "ListPaintLit" );
      if ( szPaintStr[ 0 ] == 0 )
      {
         GetStringFromAttribute( szPaintStr, zsizeof( szPaintStr ), vUserSpec,
                                 X_ER_Attribute, "PromptPaintLit" );
      }
      if ( szPaintStr[ 0 ] == 0 )
      {
         GetStringFromAttribute( szPaintStr, zsizeof( szPaintStr ), vUserSpec,
                                 X_ER_Attribute, "Name" );
         /* Add a ':' to the end. */
         strcat_s( szPaintStr, zsizeof( szPaintStr ), ":" );
         /* Add spaces before any capital letters. */
         AddSpacesToString( szPaintStr );
      }
      lPaintLth = zstrlen( szPaintStr );
      if ( lPaintLth > 20 )
         lPaintLth = 20;
      if ( lPaintLth < 5 )
         lPaintLth = 5;

      if ( lAttribLth > lPaintLth )
         lTotalLth = lTotalLth + lAttribLth + 1;
      else
         lTotalLth = lTotalLth + lPaintLth + 1;

      RESULT = SetCursorNextEntity( vUserSpec, X_LOD_Attribute, "" );
   }

   lTotalLth ++;

   SetAttributeFromInteger( vNewDialog, "Control",
                            "SZDLG_X", lTotalLth * lCC );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lHeight );

// TraceLineS("END", "CreateListBox W3" );
   return( 0 );
}
