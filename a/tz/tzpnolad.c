/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:  TZPNOLAD  -  Painter outliner control dialog operations
// DESCRIPTION:  This is the C file which contains the outliner control
//               dialog operations required by the painter.
/////////////////////////////////////////////////////////////////////////////
//  Copyright (c) 1995 - 2016 QuinSoft, Inc. All rights reserved. Confidential
// and Proprietary material subject to license - do not reproduce or
// disclose.  This material is an unpublished work and is considered a
// trade secret belonging to the copyright holder.
/////////////////////////////////////////////////////////////////////////////
// AUTHOR:  Gig Kirk
// DATE:    10/03/92
// API:     Microsoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT: Windows 3.1
//
// LAST MESSAGE ID: PN00411
//

/*
CHANGE LOG
2001.02.12  BL    TREE
   Fix for Freigabe: if Outliner in a GroupBox, then Error by close
   Outliner Detail Window

2001.02.09  BL    TREE
   Rename OL_SetDefaultBitmap to OL_SetDflBitmap and
   OL_SetSelectedBitmap to OL_SetSelBitmap

2001.02.02  BL    Z10
   Remove operations  "SelectText", "MultiSelOn" and "MultiSelOff"
   new Tree Control, implemation finished with Repository Test and new events

2001.01.10  BL    Z10
   new Tree Control under construction

2000.12.28  BL    Z10
   Insert defines for Tree Bitmaps

2000.11.26  BL    Z10
   Modified functions for new Tree Control

2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.10.05  BL    Z10
   Modified OL_DetailSave for repaint Zeidon Window

2000.08.23  BL    Z10  Repository and Tools
   Completely maintained Dialog Tool.
   If Dialog not checked out, disable all functions for update.

2000.06.27  DKS   Z10
   Force Recursive checkbox to be disabled for top entry.

2000.05.08  DC    Z10
   Created fnCreateEmptyActionView operation. This program was trying to
   reuse the operation in another dialog, which was creating link problems.
   The operation was being repeated in the other programs that needed it.

1998.02.26  DonC
   Made modifications to support the check box for Multiple Roots.

1998.07.16  DKS
   Fixed error in recursive entity processing.

1998.11.02  TMV
   Changing functioncalls of TransferToEditor because of a new argument

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzctlopr.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

// this define is stolen from zdrapp.h Update as needed.
#define zMAX_BLOB                            128000L

// These defines are stolen from zdctl.h ... update as needed.
#define zOLS_SINGLEEXPAND           0x00000001L
#define zOLS_OPENALLCHILDREN        0x00000002L
#define zOLS_OPENROOTCHILDREN       0x00000004L
#define zOLS_FOLDER_OPEN            0x00000008L
#define zOLS_NOSHOW_ENTITY          0x00000010L
#define zOLS_NOSHOW_LABEL           0x00000020L
#define zALS_MULTISEL               0x00001000L
#define zOLS_MULTISEL               0x00002000L
#define zOLS_NOBITMAPS              0x00004000L
#define zDOS_DIRRIGHT               0x00008000L
#define zOLS_HASBUTTONS             0x00010000L
#define zOLS_HASLINES               0x00020000L
#define zOLS_LINESATROOT            0x00040000L


#define zOL_RECURSIVE            0x00000001
#define zOL_CHILDREN_OPEN        0x00000002
#define zOL_USE_RECURSIVE_TOP    0x00000004

#define IDB_TREECLOSE                   144
#define IDB_TREEOPEN                    145
#define IDB_TREESELECT                  146

// If this structure changes, change the copy in zdctl.h.  This is
// the structure that defines each entity to be displayed by the
// outliner control.

#pragma pack( push, __zdctl_h )
#pragma pack( 1 )

typedef struct tagOutlineItem
{
   zLONG lTagAttributeOffset; // Offset to attribute name used to identify entity
   zLONG lAttributeOffset;    // Offset to array of attributes (null terminated)
   zLONG lSiblingEntityOffset; // Offset to next sibling entity struct
   zLONG lChildEntityOffset;  // Offset to first child entity struct
   zLONG lFlags;              // Recursive entity indicator ...
                              // If children should be expanded on init
                              // If the parent view should be used for mapping
   zCHAR  szEntityName[ 33 ]; // Entity/attribute name (required)
   zCHAR  szScopeEntity[ 33 ]; // Scope for entity for szEntityName
   zCHAR  szRecursiveEntity[ 33 ]; // Recursive entity name

} zOL_ITEM, *zLPOL_ITEM;

typedef struct tagOutlineItemColor_Font
{
   zLONG     lVersionsNumber;
   COLORREF  clrTextColor;
   LOGFONT   fntTextFont;
   zCHAR     szEntityName[ 33 ];
   zLONG     lUseDefaultBitmap;
   zLONG     lUseSelectedBitmap;
   zCHAR     szDefaultBitmapFile[ 255 ];
   zCHAR     szSelectedBitmapFile[ 255 ];
} zOL_COLOR_FONT, *zLPOL_COLOR_FONT;

#pragma pack( pop, __zdctl_h )

zOPER_EXPORT zSHORT OPERATION
fnLoadSelectionForSiblings( zVIEW      vSubtask,
                            zPCHAR     pchStart,
                            zPCHAR     pchTemp,
                            zPCHAR     pchColorTemp,
                            zPCHAR     pchDefaultColor,
                            zVIEW      vCtlWork,
                            zSHORT     nCountEntities );
zOPER_EXPORT zSHORT OPERATION
fnVerifySelectionStatus( zVIEW        vSubtask,
                         zVIEW        vCtlWork,
                         zVIEW        vCtlWorkTop,
                         zSHORT       nStatusFlag );
zOPER_EXPORT zSHORT OPERATION
fnSpecifyOutlinerOutliner( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
fnFindFirstSelectedEntity( zVIEW vCltWork );
zOPER_EXPORT zSHORT OPERATION
fnFindNextSelectedEntity( zVIEW  vCltWork,
                          zPCHAR szScopeEntity );
zOPER_EXPORT zSHORT OPERATION
fnWriteSiblingsToBuffer( zVIEW            vCtlWork,
                         zLPOL_ITEM       OL_ParentItem,
                         zPCHAR           pchStart,
                         zPCHAR           *lplpTemp,
                         zLPOL_COLOR_FONT OL_Color_Font,
                         zPCHAR           *pchColor );
zOPER_EXPORT zSHORT OPERATION
fnAddSiblingsToWorkOI( zVIEW            vSubtask,
                       zVIEW            vWork,
                       zVIEW            vLOD_Ent,
                       zVIEW            vLOD_Attr,
                       zSHORT           nLevel,
                       zLPOL_COLOR_FONT OL_Color_Font,
                       zPCHAR           *lplpColor,
                       zSHORT           nCountEntities );
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_DisableButtons( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
OL_DetailSave( zVIEW vSubtask );
zOPER_EXPORT zSHORT /* LOCAL */  OPERATION
OL_LoadBitmapWithResourcePath( zVIEW vSubtask, zPCHAR szFileName );

static zSHORT
fnWriteColorFontBitmapToBuffer( zVIEW            vCtlWork,
                                zPCHAR           szEntityName,
                                zLPOL_COLOR_FONT OL_Color_Font,
                                zPCHAR           *lplpColor );
static zSHORT
fnWriteDefaultColorFontBitmapBlob( zPCHAR           szEntityName,
                                   zLPOL_COLOR_FONT OL_Color_Font,
                                   zPCHAR           *lplpColor );
static zSHORT
fnLoadFontColorAndBitmap( zPCHAR     pchColorTemp,
                          zPCHAR     pchDefaultColor,
                          zLPOL_ITEM OL_Item,
                          zVIEW      vCtlWork,
                          zPCHAR     szEntityName,
                          zSHORT     nCountEntities );

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_ObjectPreBuild
//
// PURPOSE: This routine supports the OL_Object window. Its purpose is
// to collect the name of the view to be presented in the outliner. The
// user will only see this window when the outliner is first created.
// If this is not the first time into the outliner detail for this control,
// information about the outliner will be stored in the PWD CtrlBOI. If
// that information exists, we pass control to the OL_Detail window
// and suppress OL_Object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_ObjectPreBuild( zVIEW vSubtask )
{
   zVIEW   vDialogW;          // contains list of valid views.
   zVIEW   vCtlDialog;        // the PWD with position on the outliner
   zVIEW   vCtlWorkTop;       // top view to the control work object
   zVIEW   vCtlWork;          // the control work object

   zPCHAR  pchBuffer;
   zULONG  ulLth = zMAX_BLOB;
   zSHORT  nRC;

   // Get the views.
   GetViewByName( &vDialogW,   "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vCtlDialog, "Control" );

   GetViewByName( &vCtlWorkTop, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // Set up Outliner work entity.  Delete any existing one to start.
   if ( CheckExistenceOfEntity( vCtlWorkTop, "Outliner" ) >= zCURSOR_SET )
      DeleteEntity( vCtlWorkTop, "Outliner", zREPOS_NONE );

   CreateEntity( vCtlWorkTop, "Outliner", zPOS_FIRST );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "ViewChanged", "N" );
   CreateEntity( vCtlWorkTop, "Object", zPOS_FIRST );

   CreateViewFromViewForTask( &vCtlWork, vCtlWorkTop, 0 );
   SetNameForView( vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   // Load CtrlBOI from TZCONTROL.
   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
   {
      MessageSend( vSubtask, "PN00401", "Dialog Maintenance",
                   "Memory error - Cannot load Outliner Control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SfFreeTaskMemory( pchBuffer );
      return( 1 );
   }

   pchBuffer = (zPCHAR) SfLockTaskMemory( pchBuffer );
   GetBlobFromAttribute( pchBuffer, &ulLth, vCtlDialog, "Control", "CtrlBOI" );

   // If CtrlBOI already has data.
   if ( ulLth > 0 )
   {
      // START UP THE OUTLINER DETAIL WINDOW INSTEAD
      nRC = SetWindowActionBehavior( vSubtask,
                                     zWAB_ReplaceWindowWithModalWindow,
                                    "TZPNOLAD", "Outliner_Detail" );
   }

   pchBuffer = (zPCHAR) SfUnlockTaskMemory( pchBuffer );
   SfFreeTaskMemory( pchBuffer );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_ObjectOK
//
// PURPOSE: Cleans up if the OL_Object window is canceled.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_ObjectOK( zVIEW vSubtask )
{
   zVIEW   vCtlDialog;     // the dialog control

   // get the views
   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vCtlDialog, "CtrlMapView" ) != zCURSOR_SET )
   {
      MessageSend( vSubtask, "PN00402", "Dialog Maintenance",
                   "Specify a View for the outliner control or press Cancel.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow,
                               "TZPNOLAD", "OUTLINER_Object" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_ObjectCancel
//
// PURPOSE: Cleans up if the OL_Object window is canceled.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_ObjectCancel( zVIEW vSubtask )
{
   zVIEW   vCtlWork;
   zVIEW   vCtlDialog;

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   DeleteEntity( vCtlWork, "Outliner", zREPOS_NONE );

   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CancelSubobject( vCtlDialog, "Control" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetProperties
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_SetProperties( zVIEW vCtlWorkTop,
                  zVIEW vCtlDialog )
{
   zLONG   lWork;

   // map subtype information from dialog source object to the
   // universal control work object
   GetIntegerFromAttribute( &lWork, vCtlDialog, "Control", "Subtype" );

   if ( lWork & zOLS_MULTISEL )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "MultiSelect", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "MultiSelect", "0" );

   if ( lWork & zOLS_OPENALLCHILDREN )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "AllChildren", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "AllChildren", "0" );

   if ( lWork & zOLS_SINGLEEXPAND )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "SingleExpand", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "SingleExpand", "0" );

   if ( lWork & zOLS_NOSHOW_ENTITY )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "DisplayText", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "DisplayText", "0" );

   if ( lWork & zOLS_HASBUTTONS )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "HasButtons", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "HasButtons", "0" );

   if ( lWork & zOLS_HASLINES )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "HasLines", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "HasLines", "0" );

   if ( lWork & zOLS_LINESATROOT )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "LinesAtRoot", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "LinesAtRoot", "0" );

   if ( lWork & zOLS_NOBITMAPS )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "NoBitmaps", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "NoBitmaps", "0" );

   if ( lWork & zCONTROL_RELATIVE_POS_X )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_X", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_X", "0" );

   if ( lWork & zCONTROL_RELATIVE_POS_Y )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_Y", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_Y", "0" );

   if ( lWork & zCONTROL_RELATIVE_SIZE_X )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_X", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_X", "0" );

   if ( lWork & zCONTROL_RELATIVE_SIZE_Y )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_Y", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_Y", "0" );

   if ( lWork & zCONTROL_FULL_CLIENT )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "FullClient", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "FullClient", "0" );

   if ( lWork & zCONTROLX_DISABLE_READONLY )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "DisableAsReadOnly", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "DisableAsReadOnly", "0" );

   if ( lWork & zCONTROL_INVISIBLE )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Invisible", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "Invisible", "0" );

   if ( lWork & zCONTROL_NOTABSTOP )
      SetAttributeFromString( vCtlWorkTop, "Outliner", "NoTab", "1" );
   else
      SetAttributeFromString( vCtlWorkTop, "Outliner", "NoTab", "0" );

   return( 0 );
} // OL_SetProperties

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetPropertiesForNewTree
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_SetPropertiesForNewTree( zVIEW vCtlWorkTop )
{
   // Otherwise, this is the 1st time in for this outliner so we
   // initialize things.

   // Preset subtype information.
   SetAttributeFromInteger( vCtlWorkTop, "Object", "TopSelectedLevel",   0 );
   SetAttributeFromInteger( vCtlWorkTop, "Object", "NumberSelected",     0 );

   SetAttributeFromString( vCtlWorkTop, "Outliner", "MultiSelect",       "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "AllChildren",       "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "SingleExpand",      "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "DisplayText",       "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "HasButtons",        "1" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "HasLines",          "1" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "LinesAtRoot",       "1" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_X",    "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_POS_Y",    "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_X",   "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "Relative_SIZE_Y",   "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "FullClient",        "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "DisableAsReadOnly", "1" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "Invisible",         "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "NoTab",             "0" );
   SetAttributeFromString( vCtlWorkTop, "Outliner", "BackgroundColor",   "" );

   return( 0 );
} // OL_SetPropertiesForNewTree


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailPreBuild
//
// PURPOSE: This routine sets things up for the OL_Detail window. Broadly
// speaking, there are 3 steps to be done.
//
// 1. If this is not the first time into the detail window for this control,
//    we load the information stored in in the PWD CtrlBOI.  Otherwise,
//    we intitialize things.
//
// 2. In any case, this routine will build the Outliner's branch of the
//    universal control work object (TZPNCTWO).  We maintain 2 views to
//    this object: vCtlWorkTop always points to the top of the structure
//    while vCtlWork is used to traverse the structure recursively.
//
// 3. After the work object is built, we load the selection information
//    if this is not the first time into the detail for this outliner.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailPreBuild( zVIEW vSubtask )
{
   zVIEW   vDialogW;       // contains list of valid views.
   zVIEW   vEvent;         // contains list of available events
   zVIEW   vCtlDialog;     // the dialog control
   zVIEW   vCtlWork;       // the universal control work object
   zVIEW   vCtlWorkTop;    // top view to the universal control work object
   zVIEW   vLOD_Ent;       // the lod lod from which the work object is built
   zVIEW   vLOD_Attr;      // the lod lod from which the work object is built
   zVIEW   vSubtaskC;
   zULONG  ulLth      = zMAX_BLOB;
   zULONG  ulColorLth = zMAX_BLOB;
   zLONG   lTemp;
   zSHORT  k;
   zSHORT  nCountEntities;
   zPCHAR  pchBuffer;
   zPCHAR  pchTemp;
   zPCHAR  pchColorBuffer;
   zPCHAR  pchColorTemp;
   zPCHAR  pchStart = NULL; // KJS 11/15/2019
   zPCHAR  pchDefaultColor;
   zPCHAR  pchDefaultColorBuffer;
   zCHAR   szTempString[ 34 ];

   zLPOL_COLOR_FONT OL_Color_Font;

   // Get the views.
   GetViewByName( &vEvent,     "TZPNEVWO",   vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW,   "TZWINDOWL",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork,   "TZOUTLINER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWorkTop,"TZPNCTWO",   vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlDialog, "TZCONTROL",  vSubtask, zLEVEL_TASK );

   // Reset some views in case we are doing this in response to a change
   // of views.
   ResetViewPositions( vCtlWork );

   // Load CtrlBOI and Color, Font, Bitmap.
   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 ||
        SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchColorBuffer, zMAX_BLOB ) != 0 ||
        SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchDefaultColorBuffer, zMAX_BLOB ) != 0 )
   {
      MessageSend( vSubtask, "PN00401", "Dialog Maintenance",
                   "Memory error - Cannot load Outliner Control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 1 );
   }

   pchColorBuffer = (zPCHAR) SfLockTaskMemory( pchColorBuffer );
   GetBlobFromAttribute( pchColorBuffer, &ulColorLth,
                         vCtlDialog, "Control", "Properties" );
   pchColorTemp = pchColorBuffer;

   pchDefaultColorBuffer = (zPCHAR) SfLockTaskMemory( pchDefaultColorBuffer );
   zmemset( pchDefaultColorBuffer, 0, zMAX_BLOB );
   pchDefaultColor = pchDefaultColorBuffer;

   if ( ulColorLth == 0 )
   {
      // store Background Color (white)
      *((zLONG *) pchDefaultColor) = -1;
      pchDefaultColor += sizeof( zLONG );
      // store Default BitmapHeight = 16
      *((zLONG *) pchDefaultColor) = 16;
      pchDefaultColor += sizeof( zLONG );
      // store Default BitmapWidth = 16
      *((zLONG *) pchDefaultColor) = 16;
      pchDefaultColor += sizeof( zLONG );
   }
   else
   {
      // set Background Color
      *((zLONG *) pchDefaultColor) = *((zLONG *) pchColorTemp);
      pchDefaultColor += sizeof( zLONG );
      pchColorTemp += sizeof( zLONG );
      // store Default BitmapHeight
      *((zLONG *) pchDefaultColor) = *((zLONG *) pchColorTemp);
      pchDefaultColor += sizeof( zLONG );
      pchColorTemp += sizeof( zLONG );
      // store Default BitmapWidth
      *((zLONG *) pchDefaultColor) = *((zLONG *) pchColorTemp);
      pchDefaultColor += sizeof( zLONG );
      pchColorTemp += sizeof( zLONG );
   }

   ulLth = zMAX_BLOB;
   pchBuffer = (zPCHAR) SfLockTaskMemory( pchBuffer );
   GetBlobFromAttribute( pchBuffer, &ulLth, vCtlDialog, "Control", "CtrlBOI" );
   pchTemp = pchBuffer;

   // If this is not the first time in, we must retrieve information stored
   // in the control entity in the dialog source view (PWD).

   // If the CtrlBOI is NOT empty ...
   if ( (ulLth > 0) &&
        (CompareAttributeToString( vCtlWorkTop, "Outliner",
                                   "ViewChanged", "N" ) == 0) )
   {
      // Load the view name and advance the temp pointer.
      SetAttributeFromString( vCtlWorkTop, "Object", "Name", pchTemp );
      pchTemp += zstrlen( pchTemp ) + 1;

      // Load the level of the topmost selected entity and number of
      // selected entities.
      lTemp = *((zLONG *) pchTemp);
      SetAttributeFromInteger( vCtlWorkTop, "Object",
                               "TopSelectedLevel", lTemp );
      pchTemp += sizeof( zLONG );

      lTemp = *((zLONG *) pchTemp);
      SetAttributeFromInteger( vCtlWorkTop, "Object",
                               "NumberSelected", lTemp );
      pchTemp += sizeof( zLONG );

      // Set a pointer to the beginning of the "real", XWD CtrlBOI to
      // make the offsets useful later on.
      pchStart = pchTemp;
   }

   // Load the rest of the Object entity info.
   SetAttributeFromAttribute( vCtlWorkTop, "Object",  "Text",
                              vCtlDialog,  "Control", "Text" );

   SetAttributeFromAttribute( vCtlWorkTop, "Object",  "Tag",
                              vCtlDialog,  "Control", "Tag" );

   // Map subtype information from dialog source object to the
   // universal control work object.
   if ( ulLth > 0 )
      OL_SetProperties( vCtlWorkTop, vCtlDialog );
   else
      OL_SetPropertiesForNewTree( vCtlWorkTop );

   // Build Work object.

   // Get view to LOD lod.
   GetStringFromAttribute( szTempString, zsizeof( szTempString ), vCtlDialog, "CtrlMapView", "Name" );

   if ( SetCursorFirstEntityByString( vDialogW, "ViewObjRef", "Name",
                                      szTempString, 0 ) >= 0 )
   {
      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vDialogW, "LOD", "Name" );
      TraceLineS( "LOD found in list ", szTempString );
   }
   else
   {
      TraceLineS( "LOD not found in list ", szTempString );
   }

   if ( ActivateMetaOI_ByName( vSubtask, &vLOD_Ent, 0, zREFER_LOD_META,
                               zSINGLE | zLEVEL_APPLICATION,
                               szTempString, TRUE ) < 0 )
   {
      return( -1 );
   }

   CreateViewFromViewForTask( &vLOD_Attr, vLOD_Ent, 0 );

   // Build work object.
   OL_Color_Font  = 0;
   CreateEntity( vCtlWork, "Entity", zPOS_FIRST );
   nCountEntities = fnAddSiblingsToWorkOI( vSubtask, vCtlWork, vLOD_Ent, vLOD_Attr,
                                           1, OL_Color_Font, &pchDefaultColor, 0 );
   // Drop LOD lods.
   DropView( vLOD_Ent );
   DropView( vLOD_Attr );

   // If the CtrlBOI had info in it, then this is not the first time
   // updating this outliner.
   if ( (ulLth > 0) &&
        (CompareAttributeToString( vCtlWorkTop, "Outliner",
                                   "ViewChanged", "N" ) == 0) )
   {
      // Retrieve selection information from CtrlBOI.
      // Find first entity record in CtrlBOI.
      // Note:  pchTemp has been advanced past the view name stored in the
      //   front of the CtrlBOI. the next few lines will advance pchTemp to
      //   point to the first OL_Item in the CtrlBOI, should one exist.

      pchTemp += sizeof( zVIEW ); // skip the view
      pchTemp += zstrlen( pchTemp ) + 1; // skip the operation name; currently null
      pchTemp += zstrlen( pchTemp ) + 1; // skip the ?bottom? view name.
      pchTemp += zstrlen( pchTemp ) + 1; // skip the ?top? view name. currently null?
      //pchTemp++;

      // If Multiple Roots selected, set the Type value in the work object.
      if ( *((zLONG *) pchTemp) == 1 )
      {
         SetAttributeFromString( vCtlWorkTop, "Outliner", "MultipleRoots", "0" );
      }
      else
      {
         SetAttributeFromString( vCtlWorkTop, "Outliner", "MultipleRoots", "1" );
      }

      pchTemp += sizeof( zLONG );

      // Skip the workstring.
      pchTemp += zstrlen( pchTemp ) + 1;

      // pchTemp should now be pointing to the first OL_Item, should one exist.
      // If there is at least one selected entity ...
      GetIntegerFromAttribute( &lTemp, vCtlWorkTop,
                               "Object", "TopSelectedLevel" );
      if ( lTemp > 0 )
      {
         // gain position in work object
         ResetViewPositions( vCtlWork );

         for ( k = 1; k < lTemp; k++ )
         {
            if ( CheckExistenceOfEntity( vCtlWork, "ChildEntity" ) >= zCURSOR_SET )
               SetViewToSubobject( vCtlWork, "ChildEntity" );
         }

         pchDefaultColor = pchDefaultColorBuffer;
         pchDefaultColor += 3 * sizeof( zLONG );

         // Load selection info.
         fnLoadSelectionForSiblings( vSubtask, pchStart, pchTemp, pchColorTemp,
                                     pchDefaultColor, vCtlWork, nCountEntities );

         // Reset the work object.
         for ( k = 1; k < lTemp; k++ )
         {
            ResetViewFromSubobject( vCtlWork );
         }

         SetCursorFirstSelectedEntity( vCtlWork, "Entity", 0 );
         SetCursorFirstSelectedEntity( vCtlWork, "Attribute", 0 );
      }
   }

   // Set Text Color, Text Font and Backgrond Color from Dialog View to
   // Outliner Window.
   CreateViewFromViewForTask( &vSubtaskC, vSubtask, 0 );
   if ( SetCursorFirstEntityByString( vSubtaskC, "Ctrl", "Tag", "__Bootstrap",
                                      "" ) >= zCURSOR_SET )
   {
      SetAttributeFromBlob( vSubtaskC, "Ctrl", "Properties",
                            pchDefaultColorBuffer,
                            (zULONG) 3 * sizeof( long ) +
                             nCountEntities * sizeof( zOL_COLOR_FONT ) );
   }

   DropView( vSubtaskC );

   // Clean up and leave.
   pchBuffer = (zPCHAR) SfUnlockTaskMemory( pchBuffer );
   SfFreeTaskMemory( pchBuffer );
   pchColorBuffer = (zPCHAR) SfUnlockTaskMemory( pchColorBuffer );
   SfFreeTaskMemory( pchColorBuffer );
   pchDefaultColorBuffer = (zPCHAR) SfUnlockTaskMemory( pchDefaultColorBuffer );
   SfFreeTaskMemory( pchDefaultColorBuffer );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SearchOutlinerEntity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_SearchOutlinerEntity( zVIEW  vCtlWork,
                         zPCHAR szEntityName )
{
   zBOOL   bFound = FALSE;
   zBOOL   bDone = FALSE;
   zSHORT  nRC;

   // While there is an entity to process.
   while ( !bFound && !bDone )
   {
      if ( SetCursorFirstEntityByString( vCtlWork, "Entity", "Name",
                                         szEntityName, 0 ) >= zCURSOR_SET )
      {
         bDone  = TRUE;
         bFound = TRUE;
      }
      else
      {
         // if current entity has a child entity, recursively
         // search the subtree.
         if ( CheckExistenceOfEntity( vCtlWork, "ChildEntity" ) == zCURSOR_SET )
         {
            SetViewToSubobject( vCtlWork, "ChildEntity" );
            SetCursorFirstEntity( vCtlWork, "ChildEntity", 0 );
            nRC = OL_SearchOutlinerEntity( vCtlWork, szEntityName );

            if ( nRC == 1 )
               bFound = TRUE;
            else
               ResetViewFromSubobject( vCtlWork );
         }

         // If not found and if sibling of current entity exists, try that.
         if ( !bFound )
         {
            nRC = SetCursorNextEntity( vCtlWork, "Entity", 0 );
            if ( nRC != zCURSOR_SET )
               bDone = TRUE;
         }

      }//else
   }//while

   if ( bFound )
      return( 1 );
   else
      return( 0 );

} // OL_SearchOutlinerEntity

static zSHORT
fnLoadFontColorAndBitmap( zPCHAR     pchColorTemp,
                          zPCHAR     pchDefaultColor,
                          zLPOL_ITEM OL_Item,
                          zVIEW      vCtlWork,
                          zPCHAR     szEntityName,
                          zSHORT     nCountEntities )
{
   zLPOL_COLOR_FONT OL_Color_Font;
   zLPOL_COLOR_FONT OL_Default_Color_Font;
   zBOOL            bFound;
   zBOOL            bFoundDefault;
   zSHORT           k, j;

   k = 0;
   bFound = FALSE;
   while ( bFound == FALSE )
   {
      k++;
      if ( k > nCountEntities )
         return( 0 );

      OL_Color_Font = (zLPOL_COLOR_FONT) pchColorTemp;

      if ( zstrcmp( OL_Color_Font->szEntityName, szEntityName ) == 0 )
      {
         bFound = TRUE;
         SetAttributeFromInteger( vCtlWork, "Entity", "TextColor",
                                  OL_Color_Font->clrTextColor );
         SetAttributeFromBlob( vCtlWork, "Entity", "TextFont",
                               &OL_Color_Font->fntTextFont, sizeof( LOGFONT ) );
         SetAttributeFromInteger( vCtlWork, "Entity", "UseDefaultBitmap",
                                  OL_Color_Font->lUseDefaultBitmap );
         SetAttributeFromInteger( vCtlWork, "Entity", "UseSelectedBitmap",
                                  OL_Color_Font->lUseSelectedBitmap );
         SetAttributeFromString( vCtlWork, "Entity", "DefaultBitmapFile",
                                 OL_Color_Font->szDefaultBitmapFile );
         SetAttributeFromString( vCtlWork, "Entity", "SelectedBitmapFile",
                                 OL_Color_Font->szSelectedBitmapFile );

         j = 0;
         bFoundDefault = FALSE;
         while ( bFoundDefault == FALSE )
         {
            j++;
            if ( j > nCountEntities )
               return( 0 );

            OL_Default_Color_Font = (zLPOL_COLOR_FONT) pchDefaultColor;

            if ( zstrcmp( OL_Default_Color_Font->szEntityName,
                          OL_Item->szEntityName ) == 0 )
            {
               bFoundDefault = TRUE;
               OL_Default_Color_Font->clrTextColor       = OL_Color_Font->clrTextColor;
               OL_Default_Color_Font->fntTextFont        = OL_Color_Font->fntTextFont;
               OL_Default_Color_Font->lUseDefaultBitmap  = OL_Color_Font->lUseDefaultBitmap;
               OL_Default_Color_Font->lUseSelectedBitmap = OL_Color_Font->lUseSelectedBitmap;
               strcpy_s( OL_Default_Color_Font->szDefaultBitmapFile, zsizeof( OL_Default_Color_Font->szDefaultBitmapFile ),
                         OL_Color_Font->szDefaultBitmapFile );
               strcpy_s( OL_Default_Color_Font->szSelectedBitmapFile, zsizeof( OL_Default_Color_Font->szSelectedBitmapFile ),
                         OL_Color_Font->szSelectedBitmapFile );
               memcpy( pchDefaultColor, OL_Default_Color_Font, sizeof( zOL_COLOR_FONT ));
            }

            pchDefaultColor += sizeof( zOL_COLOR_FONT );
         } // end:  while ( bFoundDefault == FALSE )
      }

      pchColorTemp += sizeof( zOL_COLOR_FONT );
   } // end:  while ( bFound == FALSE )

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnLoadSelectionForSiblings
//
// PURPOSE: This routine recursively load the selection information
// for the work object from the PWD CtrlBOI. It follows our now standard
// DFS traverse.
//
// This routine assumes that pchTemp points to the OL_Item to be processed
// and that vCtlWork is at the level where the associated Entity entity
// can be found. NOTE: this routine does NOT assume that vCtlWork points
// to the correct Entity entity. Position on the correct Entity entity
// must be established based on the name of the entity stored in the
// OL_Item.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnLoadSelectionForSiblings( zVIEW      vSubtask,
                            zPCHAR     pchStart,
                            zPCHAR     pchTemp,
                            zPCHAR     pchColorTemp,
                            zPCHAR     pchDefaultColor,
                            zVIEW      vCtlWork,
                            zSHORT     nCountEntities )
{
   zBOOL      bSibling = TRUE;
   zLPOL_ITEM OL_Item;
   zCHAR      szTempString[ 34 ];
   zPCHAR     pchLocalTemp         = pchTemp;
   zPCHAR     pchLocalColorTemp    = pchColorTemp;
   zPCHAR     pchLocalDefaultColor = pchDefaultColor;

   // While there is an entity to process
   while ( bSibling )
   {
      // Process the current entity

      // Get the OL_ITEM
      OL_Item = (zLPOL_ITEM) pchLocalTemp;
      strcpy_s( szTempString, zsizeof( szTempString ), OL_Item->szEntityName );
//    TraceLineS( "Loaded EntityName: ", szTempString );

      // Gain position on related entity in work object
      if ( SetCursorFirstEntityByString( vCtlWork, "Entity", "Name",
                                   szTempString, 0 ) < zCURSOR_SET )
      {
         // Look for first selected entity.
         ResetViewPositions( vCtlWork );
         if ( !OL_SearchOutlinerEntity( vCtlWork, szTempString ) )
            return( -1 );
      }

      // Mark as selected
      SetSelectStateOfEntity( vCtlWork, "Entity", 1 );

      // load Text Color, Text Font and Bitmaps
      if ( pchColorTemp )
         fnLoadFontColorAndBitmap( pchLocalColorTemp, pchLocalDefaultColor,
                                   OL_Item, vCtlWork, szTempString, nCountEntities );

      // Determine the tag attr and mark it as selected
      strcpy_s( szTempString, zsizeof( szTempString ), pchStart + OL_Item->lTagAttributeOffset );
      SetCursorFirstEntityByString( vCtlWork, "Attribute", "Name",
                                    szTempString, 0 );
//    TraceLineS( "Loaded AttribName: ", szTempString );
      SetSelectStateOfEntityForSet( vCtlWork, "Attribute", 1, 3 );

      // If a recursive child exists.
      if ( OL_Item->szRecursiveEntity[ 0 ] )
      {
         // Go down level
         SetViewToSubobject( vCtlWork, "ChildEntity" );

         // Gain position on the entity
         SetCursorFirstEntityByString( vCtlWork, "Entity", "Name",
                                       OL_Item->szRecursiveEntity, 0 );

         // Mark it as selected and mark it as recursive
         SetSelectStateOfEntity( vCtlWork, "Entity", 1 );
         SetAttributeFromString( vCtlWork, "Entity",
                                 "RecursiveSelected", "1" );

         // Gain position on the same attribute as tag for parent
         // szTempString should still contain the name of the attr
         SetCursorFirstEntityByString( vCtlWork, "Attribute", "Name",
                                       szTempString, 0 );

         // Mark attribute as selected
         SetSelectStateOfEntityForSet( vCtlWork, "Attribute", 1, 3 );

         // Go back up a level
         ResetViewFromSubobject( vCtlWork );
      }

      // if a non-recursive child exists
      if ( OL_Item->lChildEntityOffset > 0 )
      {
         // Process siblings at next level down
         SetViewToSubobject( vCtlWork, "ChildEntity" );

         fnLoadSelectionForSiblings( vSubtask,
                                     pchStart,
                                     pchStart + OL_Item->lChildEntityOffset,
                                     pchColorTemp,
                                     pchDefaultColor,
                                     vCtlWork,
                                     nCountEntities );

         ResetViewFromSubobject( vCtlWork );
      }

      // If a sibling exists
      if ( OL_Item->lSiblingEntityOffset > 0 )
      {
         pchLocalTemp = pchStart + OL_Item->lSiblingEntityOffset;
      }
      else
      {
         bSibling = FALSE;
      }
   }

   return( 0 );
}

static zSHORT
fnWriteDefaultColorFontBitmapBlob( zPCHAR           szEntityName,
                                   zLPOL_COLOR_FONT OL_Color_Font,
                                   zPCHAR           *ppchColor )
{
   if ( OL_Color_Font == 0 )
      return( -1 );

   // Set Version Number
   OL_Color_Font->lVersionsNumber = 1;

   // Set Entity Name
   strcpy_s( OL_Color_Font->szEntityName, zsizeof( OL_Color_Font->szEntityName ), szEntityName );

   // Set Tree Default Bitmap for Tree Item
   OL_Color_Font->lUseDefaultBitmap  = 1;
   OL_Color_Font->lUseSelectedBitmap = 1;

   memcpy( *ppchColor, OL_Color_Font, sizeof( zOL_COLOR_FONT ) );
   *ppchColor += sizeof( zOL_COLOR_FONT );

   return( 0 );

}  //fnWriteDefaultColorFontBitmapBlob

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnAddSiblingsToWorkOI
//
// PURPOSE: This routine will build the work object to reflect the
//          object structure of the view. So, this will follow the
//          LOD lod (TZZOLODO) for the view to build the work object.
//
//          Recursively add an Entity entity to the work OI (TZOUTLINER)
//          along with its corresponding Attribute entities and all
//          of its ChildEntity entities (the recursive part).
//
//          This routine will assume that vLOD (TZZOLODO) is positioned
//          on the LOD_EntityParent entity to be copied and that vWork
//          is positioned on an empty Entity entity which is to recieve
//          the information from vLOD.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnAddSiblingsToWorkOI( zVIEW            vSubtask,
                       zVIEW            vCtlWork,
                       zVIEW            vLOD_Ent,
                       zVIEW            vLOD_Attr,
                       zSHORT           nLevel,
                       zLPOL_COLOR_FONT OL_Color_Font,
                       zPCHAR           *ppchColor,
                       zSHORT           nCountEntities1 )
{
   zBOOL   bSibling = TRUE;
   zCHAR   szTemp[ 34 ];
   zSHORT  nRC;

   static  zSHORT nCountEntities;

   if ( nCountEntities1 == 0 )
      nCountEntities = 0;

   // While there is an entity to process.
   while ( bSibling )
   {
      nCountEntities++;
      if ( nCountEntities == 107 )
         TraceLineS( "", "" );

      // Copy over the entity info.
      GetStringFromAttribute( szTemp, zsizeof( szTemp ), vLOD_Ent, "LOD_EntityParent", "Name" );
      SetAttributeFromString( vCtlWork, "Entity", "Name", szTemp );

      // Write empty color, font and Bitmap Blob.
      OL_Color_Font = (zLPOL_COLOR_FONT) *ppchColor;
      fnWriteDefaultColorFontBitmapBlob( szTemp, OL_Color_Font, ppchColor );

      SetAttributeFromString( vCtlWork, "Entity", "SelectState", "N" );
      SetSelectStateOfEntity( vCtlWork, "Entity", 0 );

      SetAttributeFromInteger( vCtlWork, "Entity", "Level", nLevel );

      if ( CompareAttributeToString( vLOD_Ent, "LOD_EntityParent", "Recursive", "Y" ) == 0 )
      {
         SetAttributeFromString( vCtlWork, "Entity", "RecursivePossible", "1" );
      }
      else
      {
         SetAttributeFromString( vCtlWork, "Entity", "RecursivePossible", "0" );
      }

      SetAttributeFromString( vCtlWork, "Entity", "RecursiveSelected", "0" );

      // Create an attribute entity for each attribute.
      if ( SetCursorFirstEntityByString( vLOD_Attr, "LOD_Entity", "Name", szTemp, 0 ) == zCURSOR_SET )
      {
         nRC = SetCursorFirstEntity( vLOD_Attr, "LOD_Attribute", "LOD_Entity" );
         while ( nRC == zCURSOR_SET )
         {
            if ( nRC = CheckExistenceOfEntity( vLOD_Attr, "ER_Attribute" ) == zCURSOR_SET )
            {
               CreateEntity( vCtlWork, "Attribute", zPOS_AFTER );
               SetAttributeFromAttribute( vCtlWork, "Attribute", "Name", vLOD_Attr, "ER_Attribute", "Name" );
               nRC = SetCursorNextEntity( vLOD_Attr, "LOD_Attribute", "LOD_Entity" );
            }
         }
      }

      // If children exist ...
      if ( CheckExistenceOfEntity( vLOD_Ent, "LOD_EntityChild" ) == zCURSOR_SET )
      {
         // ... add the children (and their attributes and children).

         // Create a ChildEntity in vCtlWork.
         CreateEntity( vCtlWork, "ChildEntity", zPOS_FIRST );

         // Process siblings at next level down.
         SetViewToSubobject( vCtlWork, "ChildEntity" );
         SetViewToSubobject( vLOD_Ent, "LOD_EntityChild" );

         fnAddSiblingsToWorkOI( vSubtask, vCtlWork, vLOD_Ent, vLOD_Attr,
                                (zSHORT) (nLevel + 1), OL_Color_Font, ppchColor,
                                nCountEntities );

         ResetViewFromSubobject( vCtlWork );
         ResetViewFromSubobject( vLOD_Ent );
      }

      // If a sibling exists ...
      if ( SetCursorNextEntity( vLOD_Ent, "LOD_EntityParent", 0 ) == zCURSOR_SET )
      {
         // ... create an Entity entity to receive it.
         CreateEntity( vCtlWork, "Entity", zPOS_AFTER );
      }
      else
      {
         bSibling = FALSE;
      }
   }

   return( nCountEntities );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_GetProperties
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG /*LOCAL */  OPERATION
OL_GetProperties( zVIEW vCtlWorkTop )
{
   // Map subtype information from control work object to the Subtype attribute
   // in the dialog view.
   zLONG lWork = 0x0L;

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "MultiSelect", "1" ) == 0 )
   {
      lWork |= zOLS_MULTISEL;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "AllChildren", "1" ) == 0 )
   {
      lWork |= zOLS_OPENALLCHILDREN;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "SingleExpand", "1" ) == 0 )
   {
      lWork |= zOLS_SINGLEEXPAND;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "DisplayText", "1" ) == 0 )
   {
      lWork |= zOLS_NOSHOW_ENTITY;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "HasButtons", "1" ) == 0 )
   {
      lWork |= zOLS_HASBUTTONS;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "HasLines", "1" ) == 0 )
   {
      lWork |= zOLS_HASLINES;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "LinesAtRoot", "1" ) == 0 )
   {
      lWork |= zOLS_LINESATROOT;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "NoBitmaps", "1" ) == 0 )
   {
      lWork |= zOLS_NOBITMAPS;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "Relative_POS_X", "1" ) == 0 )
   {
      lWork |= zCONTROL_RELATIVE_POS_X;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "Relative_POS_Y", "1" ) == 0 )
   {
      lWork |= zCONTROL_RELATIVE_POS_Y;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "Relative_SIZE_X", "1" ) == 0 )
   {
      lWork |= zCONTROL_RELATIVE_SIZE_X;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "Relative_SIZE_Y", "1" ) == 0 )
   {
      lWork |= zCONTROL_RELATIVE_SIZE_Y;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "FullClient", "1" ) == 0 )
   {
      lWork |= zCONTROL_FULL_CLIENT;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "DisableAsReadOnly", "1" ) == 0 )
   {
      lWork |= zCONTROLX_DISABLE_READONLY;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "Invisible", "1" ) == 0 )
   {
      lWork |= zCONTROL_INVISIBLE;
   }

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "NoTab", "1" ) == 0 )
   {
      lWork |= zCONTROL_NOTABSTOP;
   }

   return( lWork );
} // OL_GetProperties

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetTextColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetTextColor( zVIEW vSubtask )
{
   COLORREF clr;
   COLORREF aclrCust[ 34 ];
   zCHAR    szColor[ 34 ];
   zVIEW    vTZWINDOWL;
   zVIEW    vCtlWork;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      return( 0 );

   if ( GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Entity", "TextColor" );

   if ( zstrcmp( szColor, "" ) == 0 )
      clr = RGB( 0, 0, 0 );    // set Black for Text Color
   else
      clr = atol( szColor );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr, (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
      SetAttributeFromInteger( vCtlWork, "Entity", "TextColor", clr );
      RefreshCtrl( vSubtask, "edTextColor" );
      OL_SetTextColorForEntity( vSubtask, "__Bootstrap", 0, clr );
   }

   return( 0 );
} // OL_SetTextColor

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetTextFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetTextFont( zVIEW vSubtask )
{
   COLORREF clr;
   zVIEW    vTZWINDOWL;
   zVIEW    vCtlWork;
   zPCHAR   pchFontBlob;
   zCHAR    szColor[ 34 ];
   zULONG   ulLth = sizeof( LOGFONT );
   zLONG    lSize = 0;
   zBOOL    bShowWindowAgain = TRUE;
   LOGFONT  TextFont;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      return( 0 );

   if ( GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK ) < 0 )
      return( 0 );

   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchFontBlob, sizeof( LOGFONT )) != 0 )
   {
      MessageSend( vSubtask, "PN00406", "Dialog Maintenance",
                   "Memory error - Cannot read Text Color",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   pchFontBlob = (zPCHAR) SfLockTaskMemory( pchFontBlob );

   GetBlobFromAttribute( pchFontBlob, &ulLth, vCtlWork, "Entity", "TextFont" );
   memcpy( (LOGFONT *) &TextFont, pchFontBlob, sizeof( LOGFONT ) );
   lSize = TextFont.lfHeight;

   // get old Text Color
   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Entity", "TextColor" );
   if ( zstrcmp( szColor, "" ) == 0 )
      clr = RGB( 0, 0, 0 );    // set Black for Text Color
   else
      clr = atol( szColor );

   while ( bShowWindowAgain &&
           OperatorPromptForFont( vSubtask, &TextFont, &lSize, (zPLONG) &clr, TRUE ) == 1 )
   {
      if ( lSize > 16 )
      {
         MessageSend( vSubtask, "PN00406", "Dialog Maintenance",
                      "A Font Size greater 16 is not supported.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
      else
      {
         if ( lSize <= 2 )
         {
            SetAttributeFromBlob( vCtlWork, "Entity", "TextFont", &TextFont, 0 );
         }
         else
         {
            SetAttributeFromBlob( vCtlWork, "Entity", "TextFont", &TextFont, sizeof( LOGFONT ) );
         }

         // set new Text Color
         SetAttributeFromInteger( vCtlWork, "Entity", "TextColor", clr );
         RefreshCtrl( vSubtask, "edTextColor" );
         OL_SetTextColorForEntity( vSubtask, "__Bootstrap", 0, clr );
         OL_SetTxtFontForEntity( vSubtask, "__Bootstrap", 0, TextFont );
         bShowWindowAgain = FALSE;
      }
   }

   pchFontBlob = (zPCHAR) SfUnlockTaskMemory( pchFontBlob );
   SfFreeTaskMemory( pchFontBlob );

   return( 0 );
} // OL_SetTextFont

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetNewBackgrdColor
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetNewBackgrdColor( zVIEW vSubtask )
{
   zVIEW    vCtlWork;
   zCHAR    szColor[ 34 ];
   COLORREF clr;

   GetViewByName( &vCtlWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Outliner", "BackgroundColor" );

   if ( zstrcmp( szColor, "" ) == 0 )
      clr = RGB( 255, 255, 255 );    // set White for Background Color
   else
      clr = atol( szColor );

   OL_SetBackgroundColor( vSubtask, "__Bootstrap", clr );

   return( 0 );
} // OL_SetNewBackgrdColor

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetNewTextColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetNewTextColor( zVIEW vSubtask )
{
   zVIEW    vCtlWork;
   zCHAR    szEntityName[ 34 ];
   zCHAR    szColor[ 34 ];
   COLORREF clr;

   OL_GetCurrentEntityName( vSubtask, "__Bootstrap", &vCtlWork, szEntityName );
   if ( zstrcmp( szEntityName, "Entity" ) == 0 )
   {
      GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Entity", "TextColor" );

      if ( zstrcmp( szColor, "" ) == 0 )
         clr = RGB( 0, 0, 0 );    // set Black for Text Color
      else
         clr = atol( szColor );

      OL_SetTextColorForEntity( vSubtask, "__Bootstrap", 0, clr );
   }

   return( 0 );
} // OL_SetNewTextColor

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetBackgrdColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetBackgrdColor( zVIEW vSubtask )
{
   zCHAR    szColor[ 34 ];
   COLORREF clr;
   COLORREF aclrCust[ 34 ];
   zVIEW    vTZWINDOWL;
   zVIEW    vCtlWork;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork,   "TZPNCTWO",  vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Outliner", "BackgroundColor" );

   if ( zstrcmp( szColor, "" ) == 0 )
      clr = RGB( 255, 255, 255 );    // set White for Background Color
   else
      clr = atol( szColor );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr, (zPLONG)(zPVOID) aclrCust, TRUE ) == 1 )
   {
      if ( ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      {
         zltoa( clr, szColor, zsizeof( szColor ) );
         SetAttributeFromString( vCtlWork, "Outliner", "BackgroundColor", szColor );
         RefreshCtrl( vSubtask, "edBackgroundColor" );
         OL_SetBackgroundColor( vSubtask, "__Bootstrap", clr );
      }
   }

   return( 0 );
} // OL_SetBackgrdColor

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  OL_DetailSave
//
// PURPOSE: Saves control work object (TZOUTLINER) to the CtrlBOI in the
// view to the control (TZCONTROL) eg the PWD.
//
// The following things are done.
//
// 1. If this is outliner in the outliner detail window - the bootstrapping
// case - then we specify the CtrlBOI in hard code.
//
// 2. We check to see if the selection of entities is valid. If it is
// not valid, we put out an error message and return to the OL_Detail window.
// If it is, we continue with the save.
//
// 3. We save information in the PWD view (TZCONTROL). Some of the
// information is stored in some extra attributes. But most of the
// info is loaded into the PWD CtrlBOI. The PWD CtrlBOI contains a few
// fields at the beginning that are only used by this dialog. But
// after that, the remainder of the CtrlBOI is precisely in the form
// to be handed onto the XWD. (In fact, all ConvertOutliner in tzctlcvt.cpp
// does is to strip off these few leading fields.) Finally, we accept
// the temporal entity containing the view information.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailSave( zVIEW vSubtask )
{
   zVIEW   vCtlDialog;     // the PWD for the dialog
   zVIEW   vCtlWork;       // the universal control work object
   zVIEW   vCtlWorkTop;    // top view to the universal control work object
   zVIEW   TZWINDOWL;
   zPCHAR  pchBuffer;
   zPCHAR  pchTemp;
   zPCHAR  pchTempColor;
   zPCHAR  pchStart;
   zPCHAR  pchColorBlob;
   zCHAR   szColor[ 34 ];
   zULONG  ulLth;
   zSHORT  nRC;
   zLONG   lTemp;
   zLONG   lWork;

   zLPOL_ITEM       OL_CurrentItem;
   zLPOL_COLOR_FONT OL_Color_Font;

   // GET VIEWS
   GetViewByName( &TZWINDOWL,   "TZWINDOWL",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlDialog,  "TZCONTROL",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWorkTop, "TZPNCTWO",   vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork,    "TZOUTLINER", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, TZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      SetViewUpdate( vCtlWork );
      SetViewUpdate( vCtlWorkTop );
      return( 0 );
   }

   // If this is the BOOTSTRAPPING case, do that instead.
   if ( CompareAttributeToString( vCtlWorkTop, "Object", "Tag", "__Bootstrap" ) == 0  )
   {
      // Go to the bootstrapping routine.
      fnSpecifyOutlinerOutliner( vSubtask );

      // Notify the user.
      MessageSend( vSubtask, "PN00404", "Dialog Maintenance",
                   "Outliner control for the outliner detail window has been specified",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 0 );
   }

   SetNameForView( vCtlWork, "dksCtlWork", 0, zLEVEL_TASK );

   // VERIFY THAT THE SELECTION OF ENTITIES IS VALID
   ResetViewPositions( vCtlWork );
   nRC = fnVerifySelectionStatus( vSubtask, vCtlWork, vCtlWorkTop, 0 );

   // Save some info in the dialog view (PWD).
   SetAttributeFromAttribute( vCtlDialog,  "Control", "Text", vCtlWorkTop, "Object",  "Text" );
   SetAttributeFromAttribute( vCtlDialog,  "Control", "Tag", vCtlWorkTop, "Object",  "Tag" );

   // Map subtype information from control work object to the Subtype attribute in the dialog view.
   lWork = OL_GetProperties( vCtlWorkTop );
   SetAttributeFromInteger( vCtlDialog, "Control", "Subtype", lWork );

   // Load the CtrlBOI with work object and selection info.  Get a buffer.
   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 ||
        SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchColorBlob, zMAX_BLOB ) != 0 )
   {
      MessageSend( vSubtask, "PN00406", "Dialog Maintenance",
                   "Memory error - Cannot save Outliner Control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "TZPNOLAD", "Outliner_Detail" );
      return( 1 );
   }

   pchColorBlob = (zPCHAR) SfLockTaskMemory( pchColorBlob );
   pchTempColor = pchColorBlob;

   // Get Background Color.
   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWorkTop, "Outliner", "BackgroundColor" );
   if ( zstrcmp( szColor, "" ) == 0 )
      lTemp = -1;
   else
      lTemp = zatol( szColor );

   *((zLONG *) pchTempColor) = lTemp;
   pchTempColor += sizeof( zLONG );

   // Get Bitmap Height and Bitmap Width.
   GetIntegerFromAttribute( &lTemp, vCtlWorkTop, "Outliner", "BitmapHeight" );
   *((zLONG *) pchTempColor) = lTemp;
   pchTempColor += sizeof( zLONG );
   GetIntegerFromAttribute( &lTemp, vCtlWorkTop, "Outliner", "BitmapWidth" );
   *((zLONG *) pchTempColor) = lTemp;
   pchTempColor += sizeof( zLONG );

   pchBuffer = (zPCHAR) SfLockTaskMemory( pchBuffer );
   pchTemp = pchBuffer;
   ulLth = 0;

   // Load the Object entity info into the buffer.
   GetStringFromAttribute( pchTemp, zMAX_BLOB, vCtlDialog, "CtrlMapView", "Name" );
   pchTemp += zstrlen( pchTemp ) + 1;

   GetIntegerFromAttribute( &lTemp, vCtlWorkTop, "Object", "TopSelectedLevel" );
   *((zLONG *) pchTemp) = lTemp;
   pchTemp += sizeof( zLONG );

   GetIntegerFromAttribute( &lTemp, vCtlWorkTop, "Object", "NumberSelected" );
   *((zLONG *) pchTemp) = lTemp;
   pchTemp += sizeof( zLONG );

   // Hold onto the position of the beginning of the real CtrlBOI for
   // the XWD in order to calculate the correct offsets for the driver.
   pchStart = pchTemp;

   pchTemp += sizeof( zVIEW ); // leave room for a view

   // Load a null terminator. MapOutlinerFromOI (in ZdCtlMst) will look for an operation name and dll name,
   // but will skip them if it finds a null string.
   *pchTemp = 0;
   pchTemp++;

   // Load the ?bottom? view name. This should be the name for
   // the application view.
   GetStringFromAttribute( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), vCtlDialog, "CtrlMapView", "Name" );
   pchTemp += zstrlen( pchTemp ) + 1;

   // Load a null string for the ?top? view name. MapOutlinerFromOI
   // will skip past this.
   *pchTemp = 0;
   pchTemp++;

   // Set Multiple Roots selected flag.
   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "MultipleRoots", "1" ) == 0 )
   {
      *((zLONG *) pchTemp) = 0;
   }
   else
   {
      *((zLONG *) pchTemp) = 1;
   }

   pchTemp += sizeof( zLONG );

   // ??? - load the workstring.  We'll use the view name for the data for lack of a better thing until we learn different.
   GetStringFromAttribute( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), vCtlDialog, "CtrlMapView", "Name" );
   pchTemp += zstrlen( pchTemp ) + 1;

   // Look for first selected entity.
   ResetViewPositions( vCtlWork );

   // Write out entities to buffer.
   OL_CurrentItem = 0;
   OL_Color_Font  = 0;
   fnWriteSiblingsToBuffer( vCtlWork, OL_CurrentItem, pchStart, &pchTemp, OL_Color_Font, &pchTempColor );

   // Write buffer to the CtrlBOI.
   SetAttributeFromBlob( vCtlDialog, "Control", "CtrlBOI", pchBuffer, (zULONG) (pchTemp - pchBuffer) );

   // Write the Background and Text Color Properties.
   SetAttributeFromBlob( vCtlDialog, "Control", "Properties", pchColorBlob, (zULONG) (pchTempColor - pchColorBlob) );

   fnPainterCall( zMSG_REPAINTZEIDONWINDOW, vSubtask, 0, 0, 0 );

   // Accept the temporal entity containing the view information.
   AcceptSubobject( vCtlDialog, "Control" );

   // Clean up.
   DeleteEntity( vCtlWorkTop, "Outliner", zREPOS_NONE );

   pchBuffer = (zPCHAR) SfUnlockTaskMemory( pchBuffer );
   SfFreeTaskMemory( pchBuffer );
   pchColorBlob = (zPCHAR) SfUnlockTaskMemory( pchColorBlob );
   SfFreeTaskMemory( pchColorBlob );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnVerifySelectionStatus
//
// PURPOSE: This routine will traverse the outliner work object recursively
// and will verify that the selection of entities is valid. A selection of
// entities is valid if the following criteria are met:
// a) If 2 siblings are selected, their common parent is also selected.
// b) Given any 2 selected entities, every entity in the path connecting
//    them is also selected.
//
// This routine makes use of a semiphore, nStatusFlag, to control processing.
// The values of nStatusFlag are:
// 0 - No entity above the current level has been selected.
// 1 - The parent entity wrt the current level has been selected.
// 2 - The parent entity wrt the current level has NOT been selected BUT,
//     at least one other entity not in this subtree has been selected.
//
// This routine follows our now standard DFS search of the work object with
// one large change. The main loop which processes all siblings on a given
// level is embedded/repeated in a 3-way switch statement; one for each of
// the 3 possible states.
//
// RETURNS:  0 if no errors found.
//          -1 if an error is detected.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnVerifySelectionStatus( zVIEW    vSubtask,
                         zVIEW    vCtlWork,
                         zVIEW    vCtlWorkTop,
                         zSHORT   nStatusFlag )

{
   zBOOL   bSibling = TRUE;
   zBOOL   bSelected = FALSE;
   zSHORT  nRC;

   switch ( nStatusFlag )
   {
      // No entity above this level has been selected.
      case 0:
         bSelected = FALSE;
         while ( bSibling ) // while there is an sibling to process
         {
            // If this entity is selected
            if ( GetSelectStateOfEntity( vCtlWork, "Entity" ) > 0 )
            {
               DisplayEntityInstance( vCtlWork, "Entity" );

               if ( bSelected ) // if a previous sibling was also selected
                  return( -1 );

               // Otherwise, note the selection.
               bSelected = TRUE;
               SetAttributeFromAttribute( vCtlWorkTop, "Object", "TopSelectedLevel",
                                          vCtlWork,    "Entity", "Level" );
               SetAttributeFromInteger( vCtlWorkTop, "Object",
                                        "NumberSelected", 1 );
            }

            // If children exist.
            if ( CheckExistenceOfEntity( vCtlWork,
                                         "ChildEntity" ) == zCURSOR_SET )
            {
               // Go down a level and process children.
               SetViewToSubobject( vCtlWork, "ChildEntity" );

               // If the current entity was selected.
               if ( bSelected )
               {
                  // Set flag to 1.
                  nRC = fnVerifySelectionStatus( vSubtask, vCtlWork,
                                                 vCtlWorkTop, 1 );
                  if ( nRC == -1 )
                  {
                     return( -1 );
                  }
               }
               else
               {
                  nRC = fnVerifySelectionStatus( vSubtask, vCtlWork,
                                                 vCtlWorkTop, 0 );
                  if ( nRC == -1 )
                  {
                     return( -1 );
                  }
               }

               ResetViewFromSubobject( vCtlWork );
            }

            // If a sibling exists at this level.
            if ( SetCursorNextEntity( vCtlWork,
                                      "Entity", 0 ) == zCURSOR_UNCHANGED )
            {
               bSibling = FALSE;   // pull out of loop
            }
         }

         break;

      // The parent of this level has been selected.
      case 1:

         // While there is an sibling to process
         while ( bSibling )
         {
            bSelected = FALSE;

            // If this entity is selected
//            if ( CompareAttributeToString( vCtlWork, "Entity",
//                                           "SelectState", "Y" ) == 0 )
            if ( GetSelectStateOfEntity( vCtlWork, "Entity" ) > 0 )
            {
               DisplayEntityInstance( vCtlWork, "Entity" );
               bSelected = TRUE;
               AddToAttributeFromInteger( vCtlWorkTop, "Object",
                                          "NumberSelected", 1 );
            }

            // IF CHILDREN EXIST
            if ( CheckExistenceOfEntity( vCtlWork,
                                         "ChildEntity" ) == zCURSOR_SET )
            {
               // Go down a level and process children
               SetViewToSubobject( vCtlWork, "ChildEntity" );

               // If the current entity was selected
               if ( bSelected )
               {
                  nRC = fnVerifySelectionStatus( vSubtask, vCtlWork,
                                                 vCtlWorkTop, 1 );
                  if ( nRC == -1 )
                  {
                     return( -1 );
                  }
               }
               else
               {
                  nRC = fnVerifySelectionStatus( vSubtask, vCtlWork,
                                                 vCtlWorkTop, 2 );
                  if ( nRC == -1 )
                  {
                     return( -1 );
                  }
               }

               ResetViewFromSubobject( vCtlWork );
            }

            // If a sibling exists at this level
            if ( SetCursorNextEntity( vCtlWork,
                                      "Entity", 0 ) == zCURSOR_UNCHANGED )
            {
               // pull  out of loop
               bSibling = FALSE;
            }
         }

         break;

      // The parent of this level has not been selected but some
      // some other entity has been selected.
      case 2:
         // While there is an sibling to process
         while ( bSibling )
         {
            // If this entity is selected
//          if ( CompareAttributeToString( vCtlWork, "Entity",
//                                         "SelectState", "Y" ) == 0 )
            if ( GetSelectStateOfEntity( vCtlWork, "Entity" ) > 0 )
            {
               return( -1 );
            }

            // If children exist
            if ( CheckExistenceOfEntity( vCtlWork,
                                         "ChildEntity" ) == zCURSOR_SET )
            {
               // Go down a level and process children
               SetViewToSubobject( vCtlWork, "ChildEntity" );

               nRC = fnVerifySelectionStatus( vSubtask, vCtlWork,
                                              vCtlWorkTop, 2 );
               if ( nRC == -1 )
               {
                  return( -1 );
               }

               ResetViewFromSubobject( vCtlWork );
            }

            // If a sibling exists at this level
            if ( SetCursorNextEntity( vCtlWork,
                                      "Entity", 0 ) == zCURSOR_UNCHANGED )
            {
               // pull out of loop
               bSibling = FALSE;
            }
         }

         break;
   }

   return( 0 );
}

static zSHORT
fnWriteColorFontBitmapToBuffer( zVIEW            vCtlWork,
                                zPCHAR           szEntityName,
                                zLPOL_COLOR_FONT OL_Color_Font,
                                zPCHAR           *ppchColor )
{
   zLONG    lTemp;
   zCHAR    szTemp[ 255 ];
   zPCHAR   pchFontBlob;
   zULONG   ulLth = sizeof( LOGFONT );

   if ( OL_Color_Font == 0 )
      return( -1 );

   // Set Version Number
   OL_Color_Font->lVersionsNumber = 1;

   // Set Entity Name
   strcpy_s( OL_Color_Font->szEntityName, zsizeof( OL_Color_Font->szEntityName ), szEntityName );

   // Set Item Text Color
   GetIntegerFromAttribute( &lTemp, vCtlWork, "Entity", "TextColor" );
   OL_Color_Font->clrTextColor = lTemp;

   // Set Item Text Font
   if ( SfAllocTaskMemory( vCtlWork, (zCOREMEM) &pchFontBlob, sizeof( LOGFONT ) ) != 0 )
   {
      MessageSend( vCtlWork, "PN00406", "Dialog Maintenance",
                   "Memory error - Cannot read Text Color",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   pchFontBlob = (zPCHAR) SfLockTaskMemory( pchFontBlob );
   GetBlobFromAttribute( pchFontBlob, &ulLth, vCtlWork, "Entity", "TextFont" );
   zmemcpy( (LOGFONT *) &OL_Color_Font->fntTextFont, pchFontBlob, sizeof( LOGFONT ) );

   pchFontBlob = (zPCHAR) SfUnlockTaskMemory( pchFontBlob );
   SfFreeTaskMemory( pchFontBlob );

   // Set Bitmaps for Tree Item
   GetIntegerFromAttribute( &lTemp, vCtlWork, "Entity", "UseDefaultBitmap" );
   OL_Color_Font->lUseDefaultBitmap = lTemp;
   GetIntegerFromAttribute( &lTemp, vCtlWork, "Entity", "UseSelectedBitmap" );
   OL_Color_Font->lUseSelectedBitmap = lTemp;

   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCtlWork, "Entity", "DefaultBitmapFile" );
   strcpy_s( OL_Color_Font->szDefaultBitmapFile, zsizeof( OL_Color_Font->szDefaultBitmapFile ), szTemp );
   GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCtlWork, "Entity", "SelectedBitmapFile" );
   strcpy_s( OL_Color_Font->szSelectedBitmapFile, zsizeof( OL_Color_Font->szSelectedBitmapFile ), szTemp );

   // Set Blob
   zmemcpy( *ppchColor, OL_Color_Font, sizeof( zOL_COLOR_FONT ) );
   *ppchColor += sizeof( zOL_COLOR_FONT );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_GetScopeEntityName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_GetScopeEntityName( zVIEW  vCtlWork,
                       zPCHAR pchScopeEntity )
{
   zVIEW  vCtlWork_Copy;
   zCHAR  szScope[ 34 ];
   zBOOL  bFound   = FALSE;
   szScope[ 0 ] = 0;
   pchScopeEntity[ 0 ] = 0;

   CreateViewFromViewForTask( &vCtlWork_Copy, vCtlWork, 0 );

   while ( bFound == FALSE && ResetViewFromSubobject( vCtlWork_Copy ) == 0 )
   {
      if ( CompareAttributeToString( vCtlWork_Copy, "Entity", "SelectState", "Y" ) == 0 )
      {
         bFound = TRUE;
         GetStringFromAttribute( szScope, zsizeof( szScope ), vCtlWork_Copy, "Entity", "Name" );
      }
   }

   // if Parent not found, set Root Entity Name as Scope
   if ( bFound == FALSE )
      strcpy_s( szScope, zsizeof( szScope ), "zSCOPE_OI" );

   strcpy_s( pchScopeEntity, 34, szScope );

   DropView( vCtlWork_Copy );

   return( 0 );
} // OL_GetScopeEntityName

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnWriteSiblingsToBuffer
//
// PURPOSE: This routine writes out the selected entities and their
// selected attributes to the buffer that will be written to the PWD
// CtrlBOI. This routine will assume that selection of entities is
// valid.
//
// This routine follows the standard DFS search.
//
// The tricky part here is that the CtrlBOI handles info about
// recursive entities differently than everybody else. Everybody
// including the control work object, vCtlWork (TZOUTLINER), maintains
// an entry for both the parent and its recursive child and marks the
// child as recursive. The relationship to its identical parent is assumed.
//
// OTOH, the CtrlBOI (perhaps to save space) only maintains an OL_Item
// entry for the parent. If there is a recursive child under it, the
// name for the child entity is stored in OL_Item->szRecursiveChild.
// No OL_Item for the child is written to the CtrlBOI and attr info
// for  the child is assumed to be the same as for the parent.
//
// This routine will assume that *ppchTemp points to the place in
// the CtrlBOI that can recieve the next valid OL_Item.
//
// This routine will assume that vCtlWork|Entity points to a selected
// entity.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnWriteSiblingsToBuffer( zVIEW            vCtlWork,
                         zLPOL_ITEM       OL_ParentItem,
                         zPCHAR           pchStart,
                         zPCHAR           *ppchTemp,
                         zLPOL_COLOR_FONT OL_Color_Font,
                         zPCHAR           *ppchColor )
{
   zLPOL_ITEM OL_CurrentItem;
   zLPOL_ITEM OL_SiblingItem = 0;
   zBOOL      bSibling = TRUE;
   zBOOL      bFound;
   zCHAR      szTemp[ 34 ];
   zCHAR      szScopeEntity[ 34 ];
   zSHORT     nPrevSelSet;
   zLONG      lLth;

   if ( fnFindFirstSelectedEntity( vCtlWork ) < 0 )
      return( 0 );

   // While there is an entity to process.
   while ( bSibling )
   {
      // If the current entity is recursive.
      if ( CompareAttributeToString( vCtlWork, "Entity",
                                     "RecursiveSelected", "1" ) == 0 )
      {
         // Register the name of the child with the parent
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCtlWork, "Entity", "Name" );

         // save the selection for get Scope Entity
         SetAttributeFromString( vCtlWork, "Entity", "SelectState", "Y" );
         SetSelectStateOfEntity( vCtlWork, "Entity", FALSE );

         if ( OL_ParentItem )
         {
            strcpy_s( OL_ParentItem->szRecursiveEntity, zsizeof( OL_ParentItem->szRecursiveEntity ), szTemp );

            // Mark parent as having a recursive child
            OL_ParentItem->lFlags |= zOL_RECURSIVE;
         }

         if ( SetCursorNextSelectedEntity( vCtlWork, "Entity", 0 ) < zCURSOR_SET )
         {
            // Look for first selected entity.
            ResetViewPositions( vCtlWork );
            if ( fnFindFirstSelectedEntity( vCtlWork ) < 0 )
               bSibling = FALSE;
         }
      }
      else
      {
         // Otherwise, the current entity is not recursive so we
         // write out current entity.
         // If this is the first non-recursive child
         if ( OL_SiblingItem == 0 )
         {
            // ... and if this is not the very first time into this routine.
            if ( OL_ParentItem )
            {
               // Link new current item to the parent.
               OL_ParentItem->lChildEntityOffset = *ppchTemp - pchStart;
            }
         }
         else
         {
            // Link new current item to previous sibling.
            OL_SiblingItem->lSiblingEntityOffset = *ppchTemp - pchStart;
         }

         // Create room in the buffer.
         OL_CurrentItem = (zLPOL_ITEM) *ppchTemp;
         *ppchTemp += sizeof( zOL_ITEM );

         // Put in the name.
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCtlWork, "Entity", "Name" );
         strcpy_s( OL_CurrentItem->szEntityName, zsizeof( OL_CurrentItem->szEntityName ), szTemp );

         // Set Scope Entity Name.
         OL_GetScopeEntityName( vCtlWork, szScopeEntity );
         strcpy_s( OL_CurrentItem->szScopeEntity, zsizeof( OL_CurrentItem->szScopeEntity ), szScopeEntity );

         // Create room in the buffer.
         OL_Color_Font = (zLPOL_COLOR_FONT) *ppchColor;

         // Set Version Number, Text Color, Text Font and Bitmap
         fnWriteColorFontBitmapToBuffer( vCtlWork, szTemp, OL_Color_Font, ppchColor );

         // Preset child and sibling pointers to null.
         OL_CurrentItem->szRecursiveEntity[ 0 ] = 0;
         OL_CurrentItem->lSiblingEntityOffset = 0;
         OL_CurrentItem->lChildEntityOffset = 0;
         OL_CurrentItem->lFlags = MAKELONG( 0, sizeof( zOL_ITEM ) );

         // Put in tag attribute.
         nPrevSelSet = SetSelectSetForView( vCtlWork, 3 );
         SetCursorFirstSelectedEntity( vCtlWork, "Attribute", "Entity" );
         GetStringFromAttribute( szTemp, zsizeof( szTemp ), vCtlWork, "Attribute", "Name" );
         DisplayEntityInstance( vCtlWork, "Attribute" );
         SetSelectSetForView( vCtlWork, nPrevSelSet );

         strcpy_s( *ppchTemp, zMAX_BLOB, szTemp );
         OL_CurrentItem->lTagAttributeOffset = *ppchTemp - pchStart;
         lLth = zstrlen( *ppchTemp ) + 1;
         *ppchTemp += lLth;

         // Put in attributes (including tag) and null contexts
         // ??? - NOTE: currently we are only loading one attribute per entity.
         TraceLineS( "fnWriteSiblingsToBuffer Attribute: ", szTemp );
         strcpy_s( *ppchTemp, zMAX_BLOB - lLth, szTemp );
         OL_CurrentItem->lAttributeOffset = *ppchTemp - pchStart;
         *ppchTemp += zstrlen( *ppchTemp ) + 1;

         // Put in the double null terminator to end the attr list
         *(*ppchTemp) = 0;
         (*ppchTemp)++;
         *(*ppchTemp) = 0;
         (*ppchTemp)++;

         // Save the selection for get Scope Entity.
         SetAttributeFromString( vCtlWork, "Entity", "SelectState", "Y" );
         SetSelectStateOfEntity( vCtlWork, "Entity", FALSE );

         // Check existence of a selected child.
         bFound = FALSE;

         if ( CheckExistenceOfEntity( vCtlWork, "ChildEntity" ) == zCURSOR_SET )
         {
            SetViewToSubobject( vCtlWork, "ChildEntity" );
            if ( fnFindFirstSelectedEntity( vCtlWork ) < 0 )
               ResetViewFromSubobject( vCtlWork );
            else
               bFound = TRUE;
         }

         // If at least one selected child exists.
         if ( bFound )
         {
            // write out entities to buffer
            fnWriteSiblingsToBuffer( vCtlWork, OL_CurrentItem, pchStart,
                                     ppchTemp, OL_Color_Font, ppchColor );
            ResetViewFromSubobject( vCtlWork );
         }

         if ( fnFindNextSelectedEntity( vCtlWork, szScopeEntity ) > 0 )
            OL_SiblingItem = OL_CurrentItem;
         else
            bSibling = FALSE;

      } // end:  else this current entity is not recursive
   } // end:  while ( bSibling )

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
fnNextSelectedEntity( zVIEW vCtlWork, zPCHAR pchScopeEntity )
{
   zBOOL   bFound = FALSE;
   zBOOL   bDone  = FALSE;
   zCHAR   szScopeEntityNew[ 34 ];

   if ( SetCursorNextSelectedEntity( vCtlWork, "Entity", 0 ) >= zCURSOR_SET )
      return( 1 );

   // search Sibling Entity
   while ( !bDone && ResetViewFromSubobject( vCtlWork ) == 0 )
   {
      if ( CompareAttributeToString( vCtlWork, "Entity", "SelectState", "Y" ) == 0 )
      {
         bFound = TRUE;
         if ( fnFindFirstSelectedEntity( vCtlWork ) < 0 )
         {
            bDone = TRUE;
         }
         else
         {
            // Compare Scope Entity = Parent Entity
            OL_GetScopeEntityName( vCtlWork, szScopeEntityNew );
            if ( zstrcmp( szScopeEntityNew, pchScopeEntity ) == 0 )
               return( 1 );
            else
               bDone = TRUE;
         }
      }
   } // endwhile ( !bDone && bFound == FALSE && ResetViewFromSubobject( vCtlWork ) == 0 )

   // no Sibling found, if Scope = first Entity in View, search again
   if ( bFound == FALSE )
   {
      if ( fnFindFirstSelectedEntity( vCtlWork ) >= 0 )
         return( 1 );
   }

   return( -1 );
}


zOPER_EXPORT zSHORT OPERATION
fnFindNextSelectedEntity( zVIEW vCtlWork, zPCHAR pchScopeEntity )
{
   zSHORT  nFound = -1;
   zVIEW   vCtlWork_Test;

   if ( SetCursorNextSelectedEntity( vCtlWork, "Entity", 0 ) >= zCURSOR_SET )
      return( 1 );

   // fnNextSelectedEntity change the View ==> Test with a Copy
   CreateViewFromViewForTask( &vCtlWork_Test, vCtlWork, 0 );
   nFound = fnNextSelectedEntity( vCtlWork_Test, pchScopeEntity );
   DropView( vCtlWork_Test );

   // if Test OK, search Sibling Entity in Original View
   if ( nFound > 0 )
   {
      nFound = fnNextSelectedEntity( vCtlWork, pchScopeEntity );
   }


   return( nFound );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnFindFirstSelectedEntity
//
// PURPOSE: This routine works fine but could be rewritten to work
// faster by taking advantage of the NumberSelected and TopSelectedLevel
// attributes set by fnVerifySelectionStatus.
//
// This routine traverses (DFS) the work object and gains position on
// the first selected entity.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnFindFirstSelectedEntity( zVIEW vCtlWork )
{
   zBOOL   bFound = FALSE;
   zBOOL   bDone = FALSE;

   // While there is an entity to process.
   while ( bFound == FALSE && bDone == FALSE )
   {
      if ( GetSelectStateOfEntity( vCtlWork, "Entity" ) > 0 )
      {
         bDone  = TRUE;
         bFound = TRUE;
      }
      else
      {
         // If current entity has a child entity, recursively
         // search the subtree.
         if ( CheckExistenceOfEntity( vCtlWork, "ChildEntity" ) == zCURSOR_SET )
         {
            SetViewToSubobject( vCtlWork, "ChildEntity" );
            SetCursorFirstEntity( vCtlWork, "ChildEntity", 0 );
            if ( fnFindFirstSelectedEntity( vCtlWork ) < 0 )
               ResetViewFromSubobject( vCtlWork );
            else
               bFound = TRUE;
         }

         // If not found and  if sibling of current entity exists, try that.
         if ( bFound == FALSE )
         {
            if ( SetCursorNextEntity( vCtlWork, "Entity", 0 ) != zCURSOR_SET )
               bDone = TRUE;
         }
      }
   }  // end of:  while ( bFound == FALSE && bDone == FALSE )

   if ( bFound )
      return( 1 );
   else
      return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:  fnSpecifyOutlinerOutliner
//
// PURPOSE: This routine will specify the details for the outliner
// control in the outliner detail window. This is the bootstrapping case.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSpecifyOutlinerOutliner( zVIEW vSubtask )
{
   zVIEW   vCtlDialog;     // the PWD for the dialog
   zVIEW   vCtlWork;
   zPCHAR  pchBuffer;
   zPCHAR  pchTemp;
   zPCHAR  pchColorBlob;
   zPCHAR  pchStart;
   zCHAR   szColor[ 34 ];
   zULONG  ulLth;
   zLONG   lWork;
   zLPOL_ITEM OL_Item;

   // Get view to PWD.
   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   // SAVE Text AND Tag
   // ??? - these are dummy fields for now
   SetAttributeFromString( vCtlDialog, "Control", "Text", "Source Object" );
   SetAttributeFromString( vCtlDialog, "Control", "Tag", "__Bootstrap" );

   // Load the CtrlBOI with work object and selection info.
   // Get a buffer.
   if ( SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 ||
        SfAllocTaskMemory( vSubtask, (zCOREMEM) &pchColorBlob, zMAX_BLOB ) != 0 )
   {
      MessageSend( vSubtask, "PN00407", "Dialog Maintenance",
                   "Memory error - Cannot save Outliner Control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      // SfFreeTaskMemory( pchBuffer );
      return( 1 );
   }

   pchColorBlob = (zPCHAR) SfLockTaskMemory( pchColorBlob );
   pchTemp = pchColorBlob;

   // Store Background Color.
   GetStringFromAttribute( szColor, zsizeof( szColor ), vCtlWork, "Outliner", "BackgroundColor" );
   if ( zstrcmp( szColor, "" ) == 0 )
      lWork = -1;
   else
      lWork = zatol( szColor );

   *((zLONG *) pchTemp) = lWork;
   pchTemp += sizeof( zLONG );

   // Get Bitmap Height and Bitmap Width.
   GetIntegerFromAttribute( &lWork, vCtlWork, "Outliner", "BitmapHeight" );
   *((zLONG *) pchTemp ) = lWork;
   pchTemp += sizeof( zLONG );
   GetIntegerFromAttribute( &lWork, vCtlWork, "Outliner", "BitmapWidth" );
   *((zLONG *) pchTemp ) = lWork;

   // Write Background Color.
   SetAttributeFromBlob( vCtlDialog, "Control", "Properties",
                         pchColorBlob, 3 * sizeof( zLONG ) );

   pchBuffer = (zPCHAR) SfLockTaskMemory( pchBuffer );
   pchTemp = pchBuffer;
   ulLth = 0;

   // Load the name of the work object into the buffer.
   strcpy_s( pchTemp, zMAX_BLOB, "__Bootstrap" );
   pchTemp += zstrlen( pchTemp ) + 1;

   // Load in 2 longs as place holders.
   *((zLONG *) pchTemp) = 0;
   pchTemp += sizeof( zLONG );
   *((zLONG *) pchTemp) = 0;
   pchTemp += sizeof( zLONG );

   // Hold onto the position of the begining of the real CtrlBOI for
   // the XWD in order to calculate the correct offsets for the driver.
   pchStart = pchTemp;

   // Leave room for a view.
   pchTemp += sizeof( zVIEW );

   // Load a null terminator. MapOutlinerFromOI (in trs) will
   // look for a operation name and dll name but will skip them
   // if it finds a null string.
   *pchTemp = 0;
   pchTemp++;

   // Load the ?bottom? view name. This should be the name for
   // the application view.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "TZOUTLINER" );
   pchTemp += zstrlen( pchTemp ) + 1;

   // Load a null string for the ?top? view name. MapOutlinerFromOI
   // will skip past this.
   *pchTemp = 0;
   pchTemp++;

   // Load the nType = 1. This will cause MapOutlinerFromOI to
   // skip a 'false' root and to begin with the first data entity
   // struct.
   *((zLONG *) pchTemp) = 1;
   pchTemp += sizeof( zLONG );

   // Load the workstring. ??? We'll use the view name for
   // the data for lack of a better thing.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "TZOUTLINER" );
   pchTemp += zstrlen( pchTemp ) + 1;

   // Load the entity structs for 4 entities from TZPNOLAD.

   // OBJECT ... put a zPLOL_ITEM stuct into the BOI.
   OL_Item = (zLPOL_ITEM) pchTemp;
   pchTemp += sizeof( zOL_ITEM );

   // Put in the name and null scope entity.
   strcpy_s( OL_Item->szEntityName, zsizeof( OL_Item->szEntityName ), "Object" );
   OL_Item->szScopeEntity[ 0 ] = 0;
   OL_Item->szRecursiveEntity[ 0 ] = 0;

   // Put in tag attribute.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "Name" );
   OL_Item->lTagAttributeOffset = pchTemp - pchStart;
   pchTemp += zstrlen( pchTemp ) + 1;

   // Put in attributes (including tag) and null contexts.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "Name" );
   OL_Item->lAttributeOffset = pchTemp - pchStart;
   pchTemp += zstrlen( pchTemp ) + 1;
   *pchTemp = 0;
   pchTemp++;

   // Put in the double null terminator to end the attr list.
   *pchTemp = 0;
   pchTemp++;

   // Set pointers.
   OL_Item->lSiblingEntityOffset = 0;
   OL_Item->lChildEntityOffset = pchTemp - pchStart;

   // ENTITY ... put a zPLOL_ITEM stuct into the BOI.
   OL_Item = (zLPOL_ITEM) pchTemp;
   pchTemp += sizeof( zOL_ITEM );

   // Put in the name and null scope entity.
   strcpy_s( OL_Item->szEntityName, zsizeof( OL_Item->szEntityName ), "Entity" );
   OL_Item->szScopeEntity[ 0 ] = 0;

   OL_Item->lFlags = MAKELONG( zOL_RECURSIVE, sizeof( zOL_ITEM ) );
   strcpy_s( OL_Item->szRecursiveEntity, zsizeof( OL_Item->szRecursiveEntity ), "ChildEntity" );

   // Put in tag attribute.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "Name" );
   OL_Item->lTagAttributeOffset = pchTemp - pchStart;
   pchTemp += zstrlen( pchTemp ) + 1;

   // Put in attributes (including tag) and null contexts.
   strcpy_s( pchTemp, zMAX_BLOB - (pchTemp - pchBuffer), "Name" );
   OL_Item->lAttributeOffset = pchTemp - pchStart;
   pchTemp += zstrlen( pchTemp ) + 1;
   *pchTemp = 0;
   pchTemp++;

   // Put in the double null terminator to end the attr list.
   *pchTemp = 0;
   pchTemp++;

   // Set pointers.
   OL_Item->lSiblingEntityOffset = 0;
   OL_Item->lChildEntityOffset = 0;

   // Write buffer to the CtrlBOI.
   SetAttributeFromBlob( vCtlDialog, "Control", "CtrlBOI", pchBuffer, (zULONG) (pchTemp - pchBuffer) );

   // Map subtype information from control work object.
   // To the Subtype attribute in the PWD.
   lWork = 0x00000000L;

   lWork |= zOLS_OPENALLCHILDREN;
   lWork |= zOLS_HASBUTTONS;
   lWork |= zOLS_HASLINES;
   lWork |= zOLS_LINESATROOT;
   lWork |= zCONTROLX_DISABLE_READONLY;

   SetAttributeFromInteger( vCtlDialog, "Control", "Subtype", lWork );

   // Tie actions to events in the PWD.
   // ??? - this is dummy info for now.
   while ( CheckExistenceOfEntity( vCtlDialog, "Event" ) == zCURSOR_SET )
   {
      DeleteEntity( vCtlDialog, "Event", zREPOS_FIRST );
   }

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 1 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "SelectChange", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 2 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "DoubleClick", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 5 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "Expand", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 6 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "Collapse", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 7 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "PreSelectChange", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   CreateMetaEntity( vSubtask, vCtlDialog, "Event", zPOS_LAST );
   SetAttributeFromInteger( vCtlDialog, "Event", "Type", 8 );
   SetCursorFirstEntityByString( vCtlDialog, "Action", "Tag", "LoadTreePopup", 0 );
   IncludeSubobjectFromSubobject( vCtlDialog, "EventAct",
                                  vCtlDialog, "Action", zPOS_AFTER );

   // Accept the temporal entity containing the view information.
   AcceptSubobject( vCtlDialog, "Control" );

   // Clean up.
   pchBuffer = (zPCHAR) SfUnlockTaskMemory( pchBuffer );
   SfFreeTaskMemory( pchBuffer );
   pchColorBlob = (zPCHAR) SfUnlockTaskMemory( pchColorBlob );
   SfFreeTaskMemory( pchColorBlob );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailCancel
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailCancel( zVIEW vSubtask )
{
   zVIEW   vCtlWork;
   zVIEW   vCtlWorkTop;
   zVIEW   vCtlDialog;

   GetViewByName( &vCtlWork,   "TZOUTLINER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWorkTop, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   SetViewUpdate( vCtlWork );
   SetViewUpdate( vCtlWorkTop );

   DeleteEntity( vCtlWorkTop, "Outliner", zREPOS_NONE );

   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   CancelSubobject( vCtlDialog, "Control" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailActionPopup
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailActionPopup( zVIEW vSubtask )
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

            // Check Action Operation.
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

      EnableAction( vSubtask, "ActionDetail", bUpdate );
      EnableAction( vSubtask, "ActionNew", bNew );
      EnableAction( vSubtask, "ActionSelect", bNew );
      EnableAction( vSubtask, "ActionRemove", bDelete );
      EnableAction( vSubtask, "OperationEdit", bEdit );

      CreateTrackingPopupMenu( vSubtask, "Detail_Popup", -1, 0, FALSE, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailAddEvent
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailAddEvent( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vEvents;
   zVIEW    vCtlWork;
   zLONG    lType;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork, "TZPNCTWO", vSubtask, zLEVEL_TASK );

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
   SetAttributeFromString( vCtlWork, "TZPNCTWO", "ParentIsSelectAction", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailAddAction
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailAddAction( zVIEW vSubtask )
{
   zVIEW    vDialogW;
   zSHORT   nRC;

   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   // Create the Event entity to hold the Action which will be included
   // in the Update Action window.
   nRC = OL_DetailAddEvent( vSubtask );
   if ( nRC < 0 )
      return( -1 );

   // Create the temporal Action for transfer to Update Action window.
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
// ENTRY: OL_DetailRemoveAction
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailRemoveAction( zVIEW vSubtask )
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

   OL_DisableButtons( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailSelectAction
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailSelectAction( zVIEW vSubtask )
{
   zSHORT   nRC;

   // Create the Event entity to hold the Action which will be included
   // in the Update Action window.
   nRC = OL_DetailAddEvent( vSubtask );
   if ( nRC < 0 )
      return( -1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailUpdateAction
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailUpdateAction( zVIEW vSubtask )
{
   zVIEW    vCtlWork;
   zVIEW    vDialogW;

   GetViewByName( &vCtlWork, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( (CheckExistenceOfEntity( vCtlWork, "EventAct" ) == zCURSOR_SET) &&
        (CompareAttributeToString( vCtlWork, "EventAct", "Tag", "" ) != 0) )
   {
      SetCursorFirstEntityByAttr( vDialogW, "Action",   "Tag",
                                  vCtlWork,    "EventAct", "Tag", 0 );
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
// ENTRY: OL_DetailOperationEdit
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailOperationEdit( zVIEW vSubtask )
{
   zVIEW    vDialogC;
   zVIEW    vDialogW;
   zCHAR    szTag[ 34 ];

   GetViewByName( &vDialogC, "TZPNEVWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vDialogW, "TZWINDOW", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vDialogC, "EventAct" ) == zCURSOR_SET &&
        CompareAttributeToString( vDialogC, "EventAct", "Tag", "" ) != 0 )
   {
      GetStringFromAttribute( szTag, zsizeof( szTag ), vDialogC, "EventAct", "Tag" );
      SetCursorFirstEntityByString( vDialogW, "Action", "Tag", szTag, "" );

      if ( CheckExistenceOfEntity( vDialogW, "ActOper" ) == zCURSOR_SET )
      {
         SetCursorFirstEntityByEntityCsr( vDialogW, "Operation",
                                          vDialogW, "ActOper", "Dialog" );
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
// ENTRY: OL_DetailPreSelectChange
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailPreSelectChange( zVIEW vSubtask )
{
   zVIEW vCtlWork;
   zCHAR  szEntityName[ 34 ];

   OL_GetCurrentEntityName( vSubtask, "__Bootstrap", &vCtlWork, szEntityName );
   if ( szEntityName[ 0 ] == 0 || zstrcmp( szEntityName, "Entity" ) != 0 )
   {
//BLZ SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   MapWindow( vSubtask );
   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_ShowBitmaps
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_ShowBitmaps( zVIEW vSubtask, zSHORT bEnable, zPCHAR zsEntityName,
                zPCHAR szEntityName2, zPCHAR szBitmapTagName, zPCHAR szEditTagName,
                zPCHAR szButtonTagName, zSHORT lBitmapID )
{
   zVIEW  vCtlWorkTop;
   zVIEW  vCtlWork;
   zVIEW  vTZWINDOWL;
   zBOOL  bShowBitmaps = TRUE;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 40 ];

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWorkTop, "TZPNCTWO",   vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork,    "TZOUTLINER", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      bShowBitmaps = FALSE;

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "NoBitmaps", "1" ) != 0 &&
        TAB_GetActiveTab( vSubtask, "TreeTab" ) == 4 )
   {
      if ( CompareAttributeToInteger( vCtlWork, "Entity", zsEntityName,
                                      1 ) == 0 )
      {
         bEnable = FALSE;
         BMP_SetBitmapFileName( vSubtask, szBitmapTagName, "" );
         BMP_SetBitmapResource( vSubtask, szBitmapTagName, "ZDCTL", lBitmapID );
      }
      else
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vCtlWork, "Entity", szEntityName2 );
         if ( zstrcmp( szFileName, "" ) == 0 )
            BMP_SetBitmapResource( vSubtask, szBitmapTagName, "ZDCTL", lBitmapID );
         else
         if ( BMP_SetBitmapFileName( vSubtask, szBitmapTagName, szFileName ) < 0 )
            OL_LoadBitmapWithResourcePath( vSubtask, szFileName );
      }
   }
   else
   {
      bEnable      = FALSE;
      bShowBitmaps = FALSE;
   }

   SetCtrlState( vSubtask, szEditTagName, zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, szButtonTagName, zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "edHeight", zCONTROL_STATUS_ENABLED, bShowBitmaps );
   SetCtrlState( vSubtask, "edWidth", zCONTROL_STATUS_ENABLED, bShowBitmaps );

   return( 0 );
} // OL_ShowBitmaps

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetBitmapStateForDefault
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetBitmapStateForDefault( zVIEW vSubtask )
{
   OL_ShowBitmaps( vSubtask, TRUE, "UseDefaultBitmap",
                   "DefaultBitmapFile", "bmpDefaultBmp", "edDefaultBmp",
                   "pbDefaultBmp", IDB_TREECLOSE );

   return( 0 );
} // OL_SetBitmapStateForDefault



/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetBitmapStateForSelected
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetBitmapStateForSelected( zVIEW vSubtask )
{
   OL_ShowBitmaps( vSubtask, TRUE, "UseSelectedBitmap",
                   "SelectedBitmapFile", "bmpSelectedBmp", "edSelectedBmp",
                   "pbSelectedBmp", IDB_TREEOPEN );

   return( 0 );
} // OL_SetBitmapStateForSelected


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetBitmapState
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
OL_SetBitmapState( zVIEW  vSubtask,
                   zSHORT nState )
{
   zVIEW vCtlWorkTop;
   zBOOL bNoBitmaps = (zBOOL) nState;

   GetViewByName( &vCtlWorkTop, "TZPNCTWO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "NoBitmaps", "1" ) == 0 )
   {
      bNoBitmaps = FALSE;
   }

   SetCtrlState( vSubtask, "cbDefault_UseDfl",  zCONTROL_STATUS_ENABLED, bNoBitmaps );
   SetCtrlState( vSubtask, "cbSelected_UseDfl", zCONTROL_STATUS_ENABLED, bNoBitmaps );

   OL_ShowBitmaps( vSubtask, nState, "UseDefaultBitmap",
                   "DefaultBitmapFile", "bmpDefaultBmp", "edDefaultBmp",
                   "pbDefaultBmp", IDB_TREECLOSE );

   OL_ShowBitmaps( vSubtask, nState, "UseSelectedBitmap",
                   "SelectedBitmapFile", "bmpSelectedBmp", "edSelectedBmp",
                   "pbSelectedBmp", IDB_TREEOPEN );

   return( 0 );
} // OL_SetBitmapState

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_DisableBitmaps
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_DisableBitmaps( zVIEW vSubtask )
{
   zVIEW  vCtlWork;
   zVIEW  vTZWINDOWL;
   zCHAR  szEntityName[ 34 ];
   zBOOL  bEnable = TRUE;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   OL_GetCurrentEntityName( vSubtask, "__Bootstrap", &vCtlWork, szEntityName );

   if ( szEntityName[ 0 ] == 0 || zstrcmp( szEntityName, "Entity" ) != 0 ||
        !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      bEnable = FALSE;
   }

   OL_SetBitmapState( vSubtask, bEnable );

   return( 0 );
} // OL_DisableBitmaps

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailSelectChange
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailSelectChange( zVIEW vSubtask )
{
   zVIEW  vCtlWork;
   zVIEW  vTZWINDOWL;
   zCHAR  szEntityName[ 34 ];
   zBOOL  bEnable = TRUE;
   zSHORT nPrevSelSet;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   OL_GetCurrentEntityName( vSubtask, "__Bootstrap", &vCtlWork, szEntityName );
   if ( szEntityName[ 0 ] == 0 || zstrcmp( szEntityName, "Entity" ) != 0 ||
        !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      bEnable = FALSE;

      if ( zstrcmp( szEntityName, "Entity" ) == 0 )
      {
         SetCursorFirstSelectedEntity( vCtlWork, "Attribute", "Entity" );
         RefreshCtrl( vSubtask, "DisplayAttribute" );
         RefreshCtrl( vSubtask, "edTextColor" );
      }

      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_ENABLED, FALSE );
   }

   SetCtrlState( vSubtask, "DisplayAttribute",  zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "edTextColor",       zCONTROL_STATUS_ENABLED, bEnable );
   SetCtrlState( vSubtask, "pbTextColor",       zCONTROL_STATUS_ENABLED, bEnable );

   // Enable or disable Controls from Tab "Tree Bitmaps"
   OL_SetBitmapState( vSubtask, bEnable );
   RefreshCtrl( vSubtask, "edDefaultBmp" );
   RefreshCtrl( vSubtask, "edSelectedBmp" );
   RefreshCtrl( vSubtask, "cbDefault_UseDfl" );
   RefreshCtrl( vSubtask, "cbSelected_UseDfl" );
   RefreshCtrl( vSubtask, "edTextColor" );

   if ( bEnable == FALSE )
      return( 0 );

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );
   if ( CompareAttributeToString( vCtlWork, "Entity", "RecursivePossible", "1" ) == 0 )
   {
      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      // force the check off (in case the model has changed)
      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_CHECKED, FALSE );
      MapCtrl( vSubtask, "RecursiveSelected" );

      // now disable it
      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_ENABLED, FALSE );
   }

   nPrevSelSet = SetSelectSetForView( vCtlWork, 3 );
   TraceLineS( "OL_DetailSelectChange Selected Entity ...", "" );
   SetCursorFirstSelectedEntity( vCtlWork, "Attribute", "Entity" );
   DisplayEntityInstance( vCtlWork, "Attribute" );
   SetEntityCursor( vCtlWork, "Attribute", 0,
                    zPOS_FIRST | zQUAL_SELECTED | zSPARENT_KEEPCHILDREN,
                    0, 0, 0, 0, 0, 0 );
   DisplayEntityInstance( vCtlWork, "Attribute" );
   SetSelectSetForView( vCtlWork, nPrevSelSet );

   RefreshCtrl( vSubtask, "DisplayAttribute" );
   RefreshCtrl( vSubtask, "RecursiveSelected" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY: OL_DetailViewChange
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailViewChange( zVIEW vSubtask )
{
   zVIEW  vDialogW;
   zVIEW  vCtlWorkTop;
   zVIEW  vCtlDialog;
   zLONG  lWork;

   // Get views
   GetViewByName( &vDialogW,     "TZWINDOWL",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlDialog,   "TZCONTROL",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWorkTop,  "TZPNCTWO", vSubtask, zLEVEL_TASK );

   // If not select a new View.
   if ( CompareAttributeToAttribute( vCtlDialog, "CtrlMapView", "ZKey",
                                     vDialogW,   "ViewObjRef",  "ZKey" ) == 0 )
   {
      return( 0 );
   }

   // Redo the control subobject.
   CancelSubobject( vCtlDialog, "Control" );
   CreateTemporalSubobjectVersion( vCtlDialog, "Control" );

   if ( CheckExistenceOfEntity( vCtlDialog, "CtrlMapView" ) >= zCURSOR_SET )
      ExcludeEntity( vCtlDialog, "CtrlMapView", zPOS_FIRST );

   IncludeSubobjectFromSubobject( vCtlDialog, "CtrlMapView",
                                  vDialogW,   "ViewObjRef", zPOS_AFTER );

   // Redo the work object ... knock out the old outliner work object.
   DeleteEntity( vCtlWorkTop, "Object", zREPOS_NONE );

   // set up the new outliner work object.
   SetAttributeFromString( vCtlWorkTop, "Outliner", "ViewChanged", "Y" );
   CreateEntity( vCtlWorkTop, "Object", zPOS_FIRST );

   // Map subtype information from control work object.
   // to the Subtype attribute in the dialog view
   lWork = OL_GetProperties( vCtlWorkTop );
   SetAttributeFromInteger( vCtlDialog, "Control", "Subtype", lWork );

   // Redo all the initialization stuff.
   OL_DetailPreBuild( vSubtask );
   OL_SetCtrlState( vSubtask, "__Bootstrap", zCONTROL_STATUS_EXPANDALL, TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    OL_DetailReturnFromSubwindow
//
// PURPOSE:  This function sets up the Event entity in TZPNEVWO from
//           the selected Event entity in TZCONTROL.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
OL_DetailReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vEvents;
   zVIEW    vEventCopy;
   zLONG    lType;

   GetViewByName( &vDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Set up tye TZPNEVWO object instance from TZCONTROL and refresh
   // the List Box.
   // Loop through all vEvents.Event and set the EventAct.Tag value
   // to vDialog.EventAct.Tag, if the relationship exists, and to
   // NULL otherwise.
   CreateViewFromViewForTask( &vEventCopy, vEvents, 0 );

   SetCursorFirstEntity( vEventCopy, "Event", 0 );
   do
   {
      GetIntegerFromAttribute( &lType, vEventCopy, "Event", "Type" );
      if ( SetCursorFirstEntityByInteger( vDialog, "Event", "Type",
                       lType, 0 ) == zCURSOR_SET )
         SetAttributeFromAttribute( vEventCopy, "EventAct", "Tag",
                                    vDialog, "EventAct", "Tag" );
      else
         SetAttributeFromString( vEventCopy, "EventAct", "Tag", "" );
   } while ( SetCursorNextEntity( vEventCopy, "Event", 0 ) >
                                                      zCURSOR_UNCHANGED );
   DropView( vEventCopy );

   RefreshCtrl( vSubtask, "EventListBox" );
   OL_DisableButtons( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
      SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
} // OL_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetActionView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetActionView( zVIEW vSubtask )
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

   OL_DisableButtons( vSubtask );

   return( 0 );
} // OL_SetActionView

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_GetBkgColorAndBitmapSize
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_GetBkgColorAndBitmapSize( zVIEW vSubtask )
{
   zVIEW  vCtlDialog;
   zVIEW  vCtlWork;
   zCHAR  szColor[ 34 ];
   zCHAR  szBlob[ zMAX_BLOB ];
   zULONG ulLth = 0;
   zLONG  lTemp;

   GetViewByName( &vCtlDialog, "TZCONTROL", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCtlWork,   "TZPNCTWO",  vSubtask, zLEVEL_TASK );

   GetAttributeLength( &ulLth, vCtlDialog, "Control", "Properties" );

   if ( ulLth > 0 )
   {
      GetBlobFromAttribute( szBlob, &ulLth, vCtlDialog, "Control", "Properties" );

      // set Background Color
      memcpy( (zPLONG) &lTemp, szBlob, sizeof( long ) );
      zltoa( lTemp, szColor, zsizeof( szColor ) );

      if ( lTemp > -1 )
      {
         SetAttributeFromString( vCtlWork, "Outliner", "BackgroundColor", szColor );
         RefreshCtrl( vSubtask, "edBackgroundColor" );
      }

      // set Bitmap Height
      memcpy( (zPLONG) &lTemp, szBlob + sizeof( long ), sizeof( long ) );
      SetAttributeFromInteger( vCtlWork, "Outliner", "BitmapHeight", lTemp );

      // set Bitmap Width
      memcpy( (zPLONG) &lTemp, szBlob + 2 * sizeof( long ), sizeof( long ) );
      SetAttributeFromInteger( vCtlWork, "Outliner", "BitmapWidth", lTemp );
   }

   return( 0 );
} // OL_GetBkgColorAndBitmapSize

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_DetailPostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_DetailPostBuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( !ComponentIsCheckedOut( vSubtask, vTZWINDOWL, zSOURCE_DIALOG_META ) )
   {
      zVIEW vCtlWork;
      zVIEW vCtlWorkTop;

      GetViewByName( &vCtlWork,   "TZOUTLINER", vSubtask, zLEVEL_TASK );
      GetViewByName( &vCtlWorkTop,"TZPNCTWO",   vSubtask, zLEVEL_TASK );
      SetViewReadOnly( vCtlWorkTop );
      SetViewReadOnly( vCtlWork );

      SetCtrlState( vSubtask, "DisplayView", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edTag", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "edText", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Sizeable", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Multi", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Open", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ShowEntities", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MultipleRoots", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "RecursiveSelected", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "DisplayAttribute", zCONTROL_STATUS_ENABLED, FALSE );
   }

   // get Backround Color and Bitmap Size
   OL_GetBkgColorAndBitmapSize( vSubtask );

   OL_SetActionView( vSubtask );
   OL_DisableButtons( vSubtask );

   CB_SetSelectSelectSet( vSubtask, "DisplayAttribute", 3 );
   OL_DetailSelectChange( vSubtask );
   SetWindowState( vSubtask, zWINDOW_STATUS_NO_REFRESH, 1 );
   return( 0 );

} // OL_DetailPostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_DisableButtons
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_DisableButtons( zVIEW vSubtask )
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

      SetCtrlState( vSubtask, "ActionButton", zCONTROL_STATUS_ENABLED,
                    bEnableAction );
      EnableAction( vSubtask, "OperationEdit", bEnableEdit );
      EnableAction( vSubtask, "ActionDetail", bEnableAction );
   }

   return( 0 );
} // OL_DisableButtons

zOPER_EXPORT zSHORT OPERATION
OL_BitmapBrowseFileName( zVIEW  vSubtask,
                         zPCHAR pchFileName,
                         zSHORT nFileNameLth )
{
   pchFileName[ 0 ] = 0;
   return( OperatorPromptForFile( vSubtask, pchFileName, nFileNameLth,
                                  "Bitmap Files (*.BMP)|*.bmp|",
                                  "BMP",
                                  zOFN_HIDEREADONLY | zOFN_NOCHANGEDIR |
                                  zOFN_FILEMUSTEXIST ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadDefaultBitmap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_LoadDefaultBitmap( zVIEW vSubtask )
{
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 40 ];

   if ( OL_BitmapBrowseFileName( vSubtask, szFileName,
                                 zsizeof( szFileName ) ) == 1 )
   {
      BMP_SetBitmapFileName( vSubtask, "bmpDefaultBmp", szFileName );
      SetCtrlText( vSubtask, "edDefaultBmp", szFileName );
      MapCtrl( vSubtask, "edDefaultBmp" );
      return( 0 );
   }

    return( -1 );
} // OL_LoadDefaultBitmap

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadSelectedBitmap
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_LoadSelectedBitmap( zVIEW vSubtask )
{
   zCHAR   szFileName[ zMAX_FILENAME_LTH + 40 ];

   if ( OL_BitmapBrowseFileName( vSubtask, szFileName,
                                 zsizeof( szFileName ) ) == 1 )
   {
      SetCtrlText( vSubtask, "edSelectedBmp", szFileName );
      MapCtrl( vSubtask, "edSelectedBmp" );
      BMP_SetBitmapFileName( vSubtask, "bmpSelectedBmp", szFileName );
      return( 0 );
   }

    return( -1 );
} // OL_LoadSelectedBitmap

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetBitmap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetBitmap( zVIEW vSubtask )
{
   // Is Tab Bitmap active?
   if ( TAB_GetActiveTab( vSubtask, "TreeTab" ) == 4 )
   {
      OL_DisableBitmaps( vSubtask );
   }

   return( 0 );
} // OL_SetBitmap

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadBitmapWithResourcePath
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /* LOCAL */  OPERATION
OL_LoadBitmapWithResourcePath( zVIEW vSubtask, zPCHAR szFileName )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
   zCHAR  szWorkString[ zMAX_FILENAME_LTH + 40 ];

   // Load Bitmap mit Resource Path
   SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, zsizeof( szZeidonPath ) );

   if ( szZeidonPath[ 0 ] )
   {
      SysAppendcDirSep( szZeidonPath );
      strcat_s( szZeidonPath, zsizeof( szZeidonPath ), szFileName );
      SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
      BMP_SetBitmapFileName( vSubtask, "bmpDefaultBmp", szWorkString );
   }

   return( 0 );
} // OL_LoadBitmapWithResourcePath

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetDflBitmap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetDflBitmap( zVIEW vSubtask )
{
   zVIEW  vCtlWork;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 40 ];

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vCtlWork, "Entity", "DefaultBitmapFile" );

   if ( zstrcmp( szFileName, "" ) == 0 )
   {
      BMP_SetBitmapFileName( vSubtask, "bmpDefaultBmp", "" );
      BMP_SetBitmapResource( vSubtask, "bmpDefaultBmp",
                             "ZDCTL", IDB_TREECLOSE );
   }
   else
   {
      if ( BMP_SetBitmapFileName( vSubtask, "bmpDefaultBmp",
                                  szFileName ) < 0 )
      {
         OL_LoadBitmapWithResourcePath( vSubtask, szFileName );
      }
   }

   return( 0 );
} // OL_SetDflBitmap

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_SetSelBitmap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_SetSelBitmap( zVIEW vSubtask )
{
   zVIEW vCtlWork;
   zCHAR szFileName[ zMAX_FILENAME_LTH + 40 ];

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szFileName, zsizeof( szFileName ), vCtlWork, "Entity", "SelectedBitmapFile" );

   if ( zstrcmp( szFileName, "" ) == 0 )
   {
      BMP_SetBitmapFileName( vSubtask, "bmpSelectedBmp", "" );
      BMP_SetBitmapResource( vSubtask, "bmpSelectedBmp", "ZDCTL", IDB_TREEOPEN );
   }
   else
   {
      if ( BMP_SetBitmapFileName( vSubtask, "bmpSelectedBmp", szFileName ) < 0 )
      {
         OL_LoadBitmapWithResourcePath( vSubtask, szFileName );
      }
   }

   return( 0 );
} // OL_SetSelBitmap

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadDefaultBitmapFromPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_LoadDefaultBitmapFromPopup( zVIEW vSubtask )
{
   zVIEW vCtlWork;

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   TAB_SetActiveTab( vSubtask, "TreeTab", 4 );

   if ( OL_LoadDefaultBitmap( vSubtask ) == 0 )
   {
      SetAttributeFromInteger( vCtlWork, "Entity", "UseDefaultBitmap", 0 );
      RefreshCtrl( vSubtask, "cbDefault_UseDfl" );
      SetCtrlState( vSubtask, "edDefaultBmp", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "pbDefaultBmp", zCONTROL_STATUS_ENABLED, TRUE );
   }

   return( 0 );
} // OL_LoadDefaultBitmapFromPopup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadSelectedBitmapFromPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_LoadSelectedBitmapFromPopup( zVIEW vSubtask )
{
   zVIEW vCtlWork;

   GetViewByName( &vCtlWork, "TZOUTLINER", vSubtask, zLEVEL_TASK );

   TAB_SetActiveTab( vSubtask, "TreeTab", 4 );

   if ( OL_LoadSelectedBitmap( vSubtask ) == 0 )
   {
      SetAttributeFromInteger( vCtlWork, "Entity", "UseSelectedBitmap", 0 );
      RefreshCtrl( vSubtask, "cbSelected_UseDfl" );
      SetCtrlState( vSubtask, "edSelectedBmp", zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "pbSelectedBmp", zCONTROL_STATUS_ENABLED, TRUE );
   }

    return( 0 );
} // OL_LoadSelectedBitmapFromPopup

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: OL_LoadTreePopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OL_LoadTreePopup( zVIEW vSubtask )
{
   zVIEW vCtlWork;
   zVIEW vCtlWorkTop;
   zBOOL bEnable = TRUE;
   zCHAR szEntityName[ 34 ];

   OL_GetCurrentEntityName( vSubtask, "__Bootstrap", &vCtlWork, szEntityName );

   if ( szEntityName[ 0 ] != 0 && zstrcmp( szEntityName, "Entity" ) == 0 )
   {
      GetViewByName( &vCtlWorkTop, "TZPNCTWO", vSubtask, zLEVEL_TASK );

      if ( CompareAttributeToString( vCtlWorkTop, "Outliner", "NoBitmaps",
                                     "1" ) == 0 )
      {
         bEnable = FALSE;
      }

      EnableAction( vSubtask, "LoadDefaultBitmapFromPopup", bEnable );
      EnableAction( vSubtask, "LoadSelectedBitmapFromPopup", bEnable );

      CreateTrackingPopupMenu( vSubtask, "Tree_Popup", -1, 0, FALSE, FALSE );
   }

   return( 0 );
} // OL_LoadTreePopup

#ifdef __cplusplus
}
#endif
