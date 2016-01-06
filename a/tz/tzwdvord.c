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

#define zViewMapping          1
#define zEntityMapping        2
#define zAttributeMapping     3

#define zShowAllMapping       1
#define zShowWithMapping      2
#define zShowWithoutMapping   3

#define zSortByName           1
#define zSortByLodStructure   2

#define zMSG_SETCURRCTRL     44
#define zMSG_BUILDTZPNEVWO   23

/*
CHANGE LOG
2001.06.03  BL    R55246
   Mapping List, if doppelclick on Attribute load Control Detail Window
2001.02.28  BL   TREE
   Modified zwTZWDVORD_Mapp_ShowView: if change View, Expand all Items
2001.02.08  BL   TREE
   Expand all Items by new Show state
2000.12.06  BL   Z10   R54254
   Modified zwTZWDVORD_IncludeVOR_Entity: show new View Datas
2000.10.30  RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.02  BL   Z10
   Added functions for Mapping List
2000.08.14  BL   Z10  Repository and Tools + Bugfix
   Added zwTZWDVORD_LoadPopup, zwTZWDVORD_Postbuild,
   zwTZWDVORD_UpdateVOR_CheckName: if component not checked out,
   disable all functions for update
1999.11.24  BL   Z10  new menu structure
   Modified Operation zwTZWDVORD_UpdateVOR_OK for set Entity cursor
from here old version
1998.09.28  DKS
   Modified Dialog to work from TZWINDOWL instead of TZWINDOW view (XC 249
   and XC 261).
1998/10/15  DC
   Modified zwTZWDVORD_ExcludeVOR_Entity to use TZWINDOWL view instead of
   SysGetPointerFromHandle( TZWINDOW. )
1998/10/16  DC
   Modified zwTZWDVORD_IncludeVOR_Entity to use TZWINDOWL view instead of
   SysGetPointerFromHandle( TZWINDOW. )
*/

// LAST MESSAGE ID: WD00304

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_ShowMapping( zVIEW vSubtask );

static zBOOL
fnTZWDVORD_Mapp_DeleteAttr( zVIEW   vTZPNMAPP,
                            zVIEW   vMapTreeWithMapp,
                            zVIEW   vDialog,
                            zSHORT  nShowAttributes,
                            zSHORT  nIsReport );

static zSHORT
fnTZWDVORD_Mapp_DeleteEntity( zVIEW   vTZPNMAPP,
                              zVIEW   vMapTreeWithMapp,
                              zVIEW   vDialog,
                              zSHORT  nShowAttributes,
                              zSHORT  nIsReport );

static zSHORT
fnTZWDVORD_SetCursorToCtrl( zVIEW vControl, zULONG ulZKey );

static zSHORT
fnTZWDVORD_ControlViewCursor( zVIEW vControl, zULONG ulZKey, zPCHAR szScope );

static zSHORT
fnTZWDVORD_LoadControlDetailWnd( zVIEW vSubtask, zVIEW vTZPNMAPP, zPCHAR szEntityName );

static zSHORT
fnTZWDVORD_BuildTZPNEVWO( zVIEW vSubtask, zVIEW vControl );

static zBOOL
fnTZWDVORD_IsChangeMapping( zVIEW vControl, zVIEW vTZPNMAPP, zPCHAR szTreeEntity );

static void
fnTZWDVORD_ChangeViewMapping( zVIEW vSubtask, zVIEW vControl,
                              zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old );

static void
fnTZWDVORD_ChangeEntityMapping( zVIEW vSubtask, zVIEW vControl,
                                zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old );

static void
fnTZWDVORD_ChangeAttributeMapping( zVIEW vSubtask, zVIEW vControl,
                                   zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old );

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_GetVOR_ListView
// PURPOSE:    Set up the list view to the LPLR VOR entries and do any
//             other initialization necessary for the Select VOR dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_GetVOR_ListView( zVIEW vSubtask )
{
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vTZWINDOWL;
   zSHORT   nRC;

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      nRC = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
      SetNameForView( vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   if ( nRC == -1 )
   {
      MessageSend( vSubtask, "WD00301", "Dialog Maintenance",
                   "Cannot update Views without a dialog!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
      return( 0 );
   }

   OrderEntityForView( vTZWINDOWL, "ViewObjRef", "Name A" );

   nRC = GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta",
                        vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR_VOR_Meta, zREFER_VOR_META );
      nRC = SetNameForView( vLPLR_VOR_Meta, "LPLR_VOR_Meta",
                            vSubtask, zLEVEL_TASK );
      OrderEntityForView( vLPLR_VOR_Meta, "W_MetaDef", "Name A" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_CloseVOR_ListView
// PURPOSE:    Reset the view names (TZWINDOWL AND PAGEL) based upon
//             whether we are in Dialog or Report maintenance.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_CloseVOR_ListView( zVIEW vSubtask )
{
   zVIEW    vTZWINDOWL;
   zSHORT   nLevel;

   if ( GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY ) > 0 )
   {
      nLevel = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
      DropNameForView( vTZWINDOWL, "TZWINDOWL", 0, nLevel );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_SetVOR_Caption
// PURPOSE:    Set up the caption for the Select VOR dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_SetVOR_Caption( zVIEW vSubtask )
{
   zVIEW    vTZWINDOW;
   zCHAR    szWrk[ 256 ];

   if ( GetViewByName( &vTZWINDOW, "PAGEL", vSubtask, zLEVEL_ANY ) > 0 )
      GetStringFromAttribute( szWrk, sizeof( szWrk ), vTZWINDOW, "Report", "Tag" );
   else
   if ( GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY ) > 0 )
      GetStringFromAttribute( szWrk, sizeof( szWrk ), vTZWINDOW, "Dialog", "Tag" );

   SetWindowCaptionTitle( vSubtask, 0, szWrk );

   if ( !MiGetUpdateForView( vTZWINDOW ) )
   {
      SetCtrlState( vSubtask, "Include", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Disinclude", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_ExcludeVOR_Entity
// PURPOSE:    Exclude the ViewObjRef entity from the dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_ExcludeVOR_Entity( zVIEW  vSubtask )
{
   zVIEW    vTZWINDOW;

   GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vTZWINDOW, "ViewObjRef" )
                                                     == zCURSOR_SET )
   {
      //  Check that there are no references to the VOR from any MapRef
      //  entity in the Dialog.  If there are, present a message that
      //  identifies such and abort the exclude.
      //  ****** Add code when Gig creates necessary operation!!  ****
      ExcludeEntity( vTZWINDOW, "ViewObjRef", zREPOS_PREV );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_IncludeVOR_Entity
// PURPOSE:    Include the ViewObjRef entity from an Application
//             VOR to the dialog.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_IncludeVOR_Entity( zVIEW vSubtask )
{
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vTZWINDOW;
   zVIEW    vApplVOR;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );
   nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLPLR_VOR_Meta,
                         zREFER_VOR_META, 0 );
   if ( nRC == 1 )
   {
      GetIntegerFromAttribute( &lZKey, vApplVOR, "ViewObjRef", "ZKey");
      nRC = SetCursorFirstEntityByInteger( vTZWINDOW, "ViewObjRef",
                                           "ZKey", lZKey, "" );
      if ( nRC >= zCURSOR_SET )
      {
         ExcludeEntity( vTZWINDOW, "ViewObjRef", zREPOS_PREV );
      }

      IncludeSubobjectFromSubobject( vTZWINDOW, "ViewObjRef",
                                     vApplVOR, "ViewObjRef", zPOS_AFTER );
   }

   DropMetaOI( vSubtask, vApplVOR );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_DeleteVOR_Meta
// PURPOSE:    Delete a View Obj Ref Meta.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_DeleteVOR_Meta( zVIEW vSubtask )
{
   zVIEW    vLPLR_VOR_Meta;
   zSHORT   nRC;

   //  **** CM must add the code necessary to abort the delete if
   //       if there are dialog references to a VOR meta. ****

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta",
                                               vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vLPLR_VOR_Meta, "W_MetaDef" )
                                                     == zCURSOR_SET )
   {
      // Prompt for Delete
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DIALOG,
                          zREFER_VOR_META,
                          vLPLR_VOR_Meta, "W_MetaDef", "Name", 0 );
      if ( nRC == zRESPONSE_YES )
      {
         DeleteMetaOI( vSubtask, vLPLR_VOR_Meta, zSOURCE_VOR_META );
         SetCursorPrevEntity( vLPLR_VOR_Meta, "W_MetaDef", "" );
         SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 1, 1 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateApplVOR
// PURPOSE:    Activate the selected Application VOR in preparation for
//             update window.  Also get view to LPLR LOD meta for LOD
//             include.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateApplVOR( zVIEW vSubtask )
{
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vApplVOR;
   zSHORT   nRC;

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta",
                                              vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vLPLR_VOR_Meta, "W_MetaDef" )
                                                     == zCURSOR_SET )
   {
      nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLPLR_VOR_Meta,
                            zSOURCE_VOR_META, 0 );

      if ( nRC == 1 )
         SetNameForView( vApplVOR, "ApplVOR", vSubtask, zLEVEL_TASK );
      else
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }
   else
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_UpdateVOR_CheckName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_UpdateVOR_CheckName( zVIEW vSubtask,
                                zVIEW vApplVOR,
                                zVIEW vLPLR_VOR_Meta )
{
   zVIEW    vLPLR_VOR_Copy;
   zULONG   ulZKey;
   zULONG   ulZKeyCopy;
   zCHAR    szViewName[ 33 ];
   zSHORT   nRC;

   // Can't create a VOR without Name.
   GetStringFromAttribute( szViewName, sizeof( szViewName ), vApplVOR, "ViewObjRef", "Name" );
   if ( szViewName[ 0 ] == 0 )
   {
      MessageSend( vSubtask, "WD00302", "Dialog Maintenance",
                   "View Name required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "ViewName" );
      return( -1 );
   }

   // Check duplicate Name
   CreateViewFromViewForTask( &vLPLR_VOR_Copy, vLPLR_VOR_Meta, 0 );
   GetIntegerFromAttribute( (zPLONG) &ulZKey, vApplVOR,
                            "ViewObjRef", "ZKey" );
   nRC = SetCursorFirstEntityByString( vLPLR_VOR_Copy, "W_MetaDef", "Name",
                                       szViewName, "" );
   GetIntegerFromAttribute( (zPLONG) &ulZKeyCopy, vLPLR_VOR_Copy,
                            "W_MetaDef", "CPLR_ZKey" );

   if ( ulZKey == ulZKeyCopy )
      nRC = SetCursorNextEntityByString( vLPLR_VOR_Copy, "W_MetaDef",
                                         "Name", szViewName, "" );
   DropView( vLPLR_VOR_Copy );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "WD00302", "Dialog Maintenance",
                   "A Registered View already exists by that Name.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
      SetFocusToCtrl( vSubtask, "ViewName" );
      return( -1 );
   }

   return( 0 );

} // zwTZWDVORD_UpdateVOR_CheckName

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateVOR_OK
// PURPOSE:    Commit the Application VOR meta.  We need to add
//             code to check that the View Name hasn't already been
//             used when the VOR is checked in.  This requires a
//             special interface to CM.
//             If the VOR meta is successfully committed, the meta
//             view is then dropped.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateVOR_OK( zVIEW vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vTZWINDOW;
   zVIEW    vParentWindow;

   GetViewByName( &vApplVOR, "ApplVOR", vSubtask, zLEVEL_ANY );
   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vTZWINDOW, "PAGEL", vSubtask, zLEVEL_ANY ) < 0 )
      GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY );

   if ( !MiGetUpdateForView( vTZWINDOW ) )
      return( 0 );

   if ( zwTZWDVORD_UpdateVOR_CheckName( vSubtask, vApplVOR, vLPLR_VOR_Meta ) < 0 )
      return( -1 );

   // Can't create a VOR without a LOD.
   if ( CheckExistenceOfEntity( vApplVOR, "LOD" ) != 0 )
   {
      MessageSend( vSubtask, "WD00302", "Dialog Maintenance",
                   "You must specify an Object for this View",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetFocusToCtrl( vSubtask, "IncludeCombo" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( ObjectInstanceUpdatedFromFile( vApplVOR ) == 0 )
      DropMetaOI( vSubtask, vApplVOR );
   else
   {
      if ( CommitMetaOI( vSubtask, vApplVOR, zSOURCE_VOR_META ) == -1 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
      else
      {
         OrderEntityForView( vLPLR_VOR_Meta, "W_MetaDef", "Name A" );
         SetCursorFirstEntityByAttr( vLPLR_VOR_Meta, "W_MetaDef", "Name",
                                     vApplVOR, "ViewObjRef", "Name", "" );
         DropMetaOI( vSubtask, vApplVOR );
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_AddNewApplVOR
// PURPOSE:    Accept the new ApplVOR Name from the Add window and
//             create a new meta OI.
//             The View Name was entered into the attribute:
//                                         TZPNTROO.Palette.NewFile
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_AddNewApplVOR( zVIEW  vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vWindow;

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "VOR_Update" ) >= 0 )
   {
      if ( zwTZWDVORD_UpdateVOR_OK( vSubtask ) < 0 )
         return( -1 );
   }

   ActivateEmptyMetaOI( vSubtask, &vApplVOR, zSOURCE_VOR_META,
                         zSINGLE | zLEVEL_APPLICATION );

   CreateMetaEntity( vSubtask, vApplVOR, "ViewObjRef", zPOS_AFTER );
   SetNameForView( vApplVOR, "ApplVOR", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateVOR_Cancel
// PURPOSE:    Drop the VOR meta OI before returning.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateVOR_Cancel( zVIEW vSubtask )
{
   zVIEW    vApplVOR;

   GetViewByName( &vApplVOR, "ApplVOR", vSubtask, zLEVEL_ANY );

   DropMetaOI( vSubtask, vApplVOR );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateInit
// PURPOSE:    Initialize the LPLR_LOD_Meta view.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateInit( zVIEW vSubtask )
{
   zVIEW    vLPLR_LOD_Meta;
   zSHORT   nRC;

   nRC = GetViewByName( &vLPLR_LOD_Meta, "LPLR_LOD_Meta", vSubtask, zLEVEL_ANY );

   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vLPLR_LOD_Meta, zREFER_LOD_META );
      nRC = SetNameForView( vLPLR_LOD_Meta, "LPLR_LOD_Meta",
                            vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateVOR_Next
// PURPOSE:    Commit the Application VOR meta and prepare for next
//             Application VOR Update window.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateVOR_Next( zVIEW vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vParentWindow;
   zSHORT   nRC;

   if ( zwTZWDVORD_UpdateVOR_OK( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );
   SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 0, 1 );
   SetCursorNextEntity( vLPLR_VOR_Meta, "W_MetaDef", "" );
   SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 1, 1 );

   nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLPLR_VOR_Meta,
                         zSOURCE_VOR_META, 0 );
   SetNameForView( vApplVOR, "ApplVOR", vSubtask, zLEVEL_TASK );

   SetFocusToCtrl( vSubtask, "ViewName" );
   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateVOR_Prev
// PURPOSE:    Commit the Application VOR meta and prepare for previous
//             Application VOR Update window.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateVOR_Prev( zVIEW vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vParentWindow;
   zSHORT   nRC;

   if ( zwTZWDVORD_UpdateVOR_OK( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );
   SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 0, 1 );
   SetCursorPrevEntity( vLPLR_VOR_Meta, "W_MetaDef", "" );
   SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 1, 1 );

   nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLPLR_VOR_Meta,
                         zSOURCE_VOR_META, 0 );
   SetNameForView( vApplVOR, "ApplVOR", vSubtask, zLEVEL_TASK );

   SetFocusToCtrl( vSubtask, "ViewName" );
   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_UpdateVOR_Delete
// PURPOSE:    Delete the current Application VOR.  Try to reposition
//             on the next VOR.  If no other VORs exist, exit the window
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_UpdateVOR_Delete( zVIEW vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vLPLR_VOR_Meta;
   zVIEW    vLPLR_VOR_Copy;
   zVIEW    vParentWindow;
   zSHORT   nRC;
   zCHAR    szViewName[ 33 ];
   zCHAR    szMsg[ 70 ];

   GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY );
   GetViewByName( &vApplVOR, "ApplVOR", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vLPLR_VOR_Copy, vLPLR_VOR_Meta, 0 );
   nRC = SetCursorFirstEntityByAttr( vLPLR_VOR_Copy, "W_MetaDef", "CPLR_ZKey",
                                     vApplVOR, "ViewObjRef", "ZKey", "" );
   DropView( vLPLR_VOR_Copy );

   // if a new registered View?
   if ( nRC < zCURSOR_SET )
   {
      GetStringFromAttribute( szViewName, sizeof( szViewName ), vApplVOR, "ViewObjRef", "Name" );
      strcpy_s( szMsg, sizeof( szMsg ), "OK to delete Registered View '" );
      strcat_s( szMsg, sizeof( szMsg ), szViewName );
      strcat_s( szMsg, sizeof( szMsg ), "'?" );
      nRC = MessagePrompt( vSubtask, "WD00303", "Dialog Maintenance",
                           szMsg, zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_NO, zICON_QUESTION );
      if ( nRC == zRESPONSE_YES )
         DropMetaOI( vSubtask, vApplVOR );
      else
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
   }
   else
   {
      // Prompt for Delete
      nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_DIALOG,
                          zREFER_VOR_META,
                          vLPLR_VOR_Meta, "W_MetaDef", "Name", 0 );
      if ( nRC == zRESPONSE_NO )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }

      DropMetaOI( vSubtask, vApplVOR );
      DeleteMetaOI( vSubtask, vLPLR_VOR_Meta, zSOURCE_VOR_META );

      if ( SetCursorPrevEntity( vLPLR_VOR_Meta, "W_MetaDef", "" ) == zCURSOR_SET )
         SetSelectStateOfEntityForSet( vLPLR_VOR_Meta, "W_MetaDef", 1, 1 );
   }

   if ( CheckExistenceOfEntity( vLPLR_VOR_Meta, "W_MetaDef" ) == zCURSOR_SET )
   {
      nRC = ActivateMetaOI( vSubtask, &vApplVOR, vLPLR_VOR_Meta,
                            zSOURCE_VOR_META, 0 );
      SetNameForView( vApplVOR, "ApplVOR", vSubtask, zLEVEL_TASK );
      SetFocusToCtrl( vSubtask, "ViewName" );
      GetParentWindow( &vParentWindow, vSubtask );
      RefreshWindow( vParentWindow );
   }
   else
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
      return( -1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZWDVORD_IncludeLOD
// PURPOSE:    Include a LOD under the VOR entity.  Note that we will not
//             allow a LOD to be changed once it has been set.  We will also
//             do this by graying the combo box if a LOD has already been
//             included.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZWDVORD_IncludeLOD( zVIEW vSubtask )
{
   zVIEW    vApplVOR;
   zVIEW    vLPLR_LOD_Meta;
   zVIEW    vLOD_Meta;

   GetViewByName( &vApplVOR, "ApplVOR", vSubtask, zLEVEL_ANY );
   GetViewByName( &vLPLR_LOD_Meta, "LPLR_LOD_Meta", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vApplVOR, "LOD" ) == zCURSOR_SET )
   {
//    MessageSend( vSubtask, "WD00303", "Dialog Maintenance",
//                 "LOD already selected.  To change LOD, create new VOR.",
//                 zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
//    // Reset the combo box back to its previous value.
//    RefreshCtrl( vSubtask, "IncludeCombo" );
      ExcludeEntity( vApplVOR, "LOD", zREPOS_FIRST );
   }
// else
   {
      ActivateMetaOI( vSubtask, &vLOD_Meta, vLPLR_LOD_Meta,
                      zREFER_LOD_META, 0 );
      IncludeSubobjectFromSubobject( vApplVOR, "LOD",
                                     vLOD_Meta, "LOD", zPOS_AFTER );
      DropMetaOI( vSubtask, vLOD_Meta );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_LoadPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_LoadPopup( zVIEW vSubtask )
{
   zVIEW  vLPLR_VOR_Meta;
   zVIEW  vTZWINDOW;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   if ( GetViewByName( &vTZWINDOW, "PAGEL", vSubtask, zLEVEL_ANY ) < 0 )
      GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vLPLR_VOR_Meta, "LPLR_VOR_Meta", vSubtask, zLEVEL_ANY ) > 0 )
   {
      nIsCheckedOut = MiGetUpdateForView( vTZWINDOW );

      if ( CheckExistenceOfEntity( vLPLR_VOR_Meta, "W_MetaDef" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "AddApplVOR", bNew );
      EnableAction( vSubtask, "DeleteApplVOR", bDelete );

      CreateTrackingPopupMenu( vSubtask, "ViewPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );

} // zwTZWDVORD_LoadPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Postbuild( zVIEW vSubtask )
{
   zVIEW  vTZWINDOW;

   if ( GetViewByName( &vTZWINDOW, "PAGEL", vSubtask, zLEVEL_ANY ) < 0 )
      GetViewByName( &vTZWINDOW, "TZWINDOWL", vSubtask, zLEVEL_ANY );

   if ( !MiGetUpdateForView( vTZWINDOW ) )
   {
      SetCtrlState( vSubtask, "Add", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ViewName", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "IncludeCombo", zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "mleDesc", zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );

} // zwTZWDVORD_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_CreateTreeObject
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_CreateTreeObject( zVIEW vSubtask,
                                  zVIEW vMapTreeCtrl,
                                  zVIEW vTZWINDOWL )
{
   zVIEW    vLOD;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zLONG    lEntityNo = 0;
   zLONG    lAttributeNo;
   zCHAR    szViewAndLodName[ 71 ];
   zCHAR    szLodName[ 33 ];
   zSHORT   nRC;

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_LOD_META );

   CreateEntity( vMapTreeCtrl, "RegView", zPOS_AFTER );
   GetStringFromAttribute( szViewAndLodName, sizeof( szViewAndLodName ), vTZWINDOWL, "ViewObjRef", "Name" );
   strcat_s( szViewAndLodName, sizeof( szViewAndLodName ), " ( Object Name: " );
   GetStringFromAttribute( szLodName, sizeof( szLodName ), vTZWINDOWL, "LOD", "Name" );
   strcat_s( szViewAndLodName, sizeof( szViewAndLodName ), szLodName );
   strcat_s( szViewAndLodName, sizeof( szViewAndLodName ), " )" );
   SetAttributeFromString( vMapTreeCtrl, "RegView", "ViewAndLodName", szViewAndLodName );
   SetAttributeFromInteger( vMapTreeCtrl, "RegView", "ShowFlag", zShowAllMapping );
   SetAttributeFromInteger( vMapTreeCtrl, "RegView", "SortFlag", zSortByLodStructure );
   SetAttributeFromAttribute( vMapTreeCtrl, "RegView", "ZKey",
                              vTZWINDOWL, "ViewObjRef", "ZKey" );

   GetIntegerFromAttribute( &lZKey, vTZWINDOWL, "LOD", "ZKey" );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vLOD, vCM_List, zREFER_LOD_META, 0 );

   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_Entity", "" ) )
   {
      lEntityNo++;
      lAttributeNo = 0;
      CreateEntity( vMapTreeCtrl, "RegEntity", zPOS_AFTER );
      SetAttributeFromAttribute( vMapTreeCtrl, "RegEntity", "Name",
                                 vLOD, "LOD_Entity", "Name" );
      SetAttributeFromAttribute( vMapTreeCtrl, "RegEntity", "ZKey",
                                 vLOD, "LOD_Entity", "ZKey" );
      SetAttributeFromInteger( vMapTreeCtrl, "RegEntity", "SequenceNo",
                               lEntityNo );

      for ( nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", "" ) )
      {
         lAttributeNo++;
         CreateEntity( vMapTreeCtrl, "RegAttribute", zPOS_AFTER );
         SetAttributeFromAttribute( vMapTreeCtrl, "RegAttribute", "Name",
                                    vLOD, "ER_Attribute", "Name" );
         SetAttributeFromAttribute( vMapTreeCtrl, "RegAttribute", "ZKey",
                                    vLOD, "ER_Attribute", "ZKey" );
         SetAttributeFromInteger( vMapTreeCtrl, "RegAttribute", "SequenceNo",
                                  lAttributeNo );
      }
   }

   DropMetaOI( vSubtask, vLOD );
   return( 0 );

} // zwTZWDVORD_Mapp_CreateTreeObject

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_ShowView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_ShowView( zVIEW vSubtask )
{
   zVIEW    vMapTreeCtrl;
   zVIEW    vTZPNMAPP;
   zVIEW    vTZWINDOWL;
   zVIEW    vWindow;
   zSHORT   nRC;

   GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl_Orig", vSubtask, zLEVEL_TASK ) < 0 )
      GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );
   else
      SetNameForView( vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      nRC = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
      SetNameForView( vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   // View does not exists in Worklod, create it
   if ( SetCursorFirstEntityByAttr( vMapTreeCtrl, "RegView", "ZKey",
                                    vTZWINDOWL, "ViewObjRef", "ZKey",
                                    "" ) < zCURSOR_SET )
   {
      zwTZWDVORD_Mapp_CreateTreeObject( vSubtask, vMapTreeCtrl, vTZWINDOWL );
      zwTZWDVORD_Mapp_CreateTreeObject( vSubtask, vTZPNMAPP, vTZWINDOWL );
   }

   SetAttributeFromInteger( vMapTreeCtrl, "RegView", "ShowFlag", zShowAllMapping );
   SetAttributeFromInteger( vMapTreeCtrl, "RegView", "SortFlag", zSortByLodStructure );
// SetAllSelStatesForEntityForSet( vTZPNMAPP, "RegView", 0, 1, 0 );
   SetAllSelStatesForEntityForSet( vMapTreeCtrl, "RegView", 0, 1, 0 );
// SetSelectStateOfEntity( vTZPNMAPP, "RegView", 1 );
// SetSelectStateOfEntity( vMapTreeCtrl, "RegView", 1 );
// DisplayEntityInstance( vMapTreeCtrl, "RegView" );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "VOR_Select" ) >= 0 )
      RefreshCtrl( vWindow, "DlgViews" );

   OL_SetCtrlState( vSubtask, "ViewTree", zCONTROL_STATUS_EXPANDALL, TRUE );
   SetFocusToCtrl( vSubtask, "ViewTree" );
   return( 0 );

} // zwTZWDVORD_Mapp_ShowView

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_Postbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_Postbuild( zVIEW vSubtask )
{
   zVIEW    vTZWINDOWL;
   zVIEW    vTZPNMAPP;
   zVIEW    vMapTreeCtrl;
   zVIEW    vWindow;
   zSHORT   nRC;

   if ( GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY ) == -1 )
      GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
   {
      SetNameForView( vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
      // if Zeidon Reports, set new ListBox Title
      SetCtrlRowColText( vSubtask, "lbMappingView", 0, 1, "GroupSet - Group" );
      SetCtrlRowColText( vSubtask, "lbMappingEntity", 0, 1, "GroupSet - Group" );
      SetCtrlRowColText( vSubtask, "lbMappingAttribute", 0, 1, "GroupSet - Group" );
   }

   // Create Work LOD
   ActivateEmptyObjectInstance( &vMapTreeCtrl, "TZPNMAPP", vSubtask, zMULTIPLE );
   SetNameForView( vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "VOR_Select" ) >= 0 )
      nRC = zCURSOR_SET;
   else
      nRC = SetCursorFirstEntity( vTZWINDOWL, "ViewObjRef", "" );

   // Create Entity and Attribute Names
   if ( nRC >= zCURSOR_SET )
   {
      zwTZWDVORD_Mapp_CreateTreeObject( vSubtask, vMapTreeCtrl, vTZWINDOWL );
      // Create Work LOD
      ActivateOI_FromOI_ForTask( &vTZPNMAPP, vMapTreeCtrl, 0, zMULTIPLE );
      SetNameForView( vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK );
      zwTZWDVORD_Mapp_ShowMapping( vSubtask );
   }
   else
   {
      // Create Work LOD
      ActivateOI_FromOI_ForTask( &vTZPNMAPP, vMapTreeCtrl, 0, zMULTIPLE );
      SetNameForView( vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK );
   }

   SetFocusToCtrl( vSubtask, "ViewTree" );

   return( 0 );

} // zwTZWDVORD_Mapp_Postbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_CreateViewMap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_CreateViewMap( zVIEW  vTZPNMAPP,
                               zVIEW  vDialog,
                               zPCHAR szWindowName,
                               zLONG  lZKey,
                               zPCHAR szParentTag )
{
   zCHAR   szControlTag[ 33 ] = { 0 };
   zSHORT  nRC;

   if ( SetCursorFirstEntity( vDialog, "Control", "" ) >= zCURSOR_SET )
   {
      do
      {
         for ( nRC = SetCursorFirstEntityByInteger( vDialog, "CtrlMapView",
                                                    "ZKey", lZKey, "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vDialog, "CtrlMapView",
                                                   "ZKey", lZKey, "" ) )
         {
            // create Work instance for controls with View mapping and
            // without Entity mapping and without Attribute mapping
            // (Control is for example a Outline Control)
            if ( CompareAttributeToString( vDialog, "CtrlMapView", "Name",
                                           "" ) != 0 &&
                 SetCursorFirstEntity( vDialog, "CtrlMapLOD_Entity",
                                       "" ) < zCURSOR_SET &&
                 SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute",
                                       "" ) < zCURSOR_SET &&
                 SetCursorFirstEntityByAttr( vTZPNMAPP, "RegViewMapping", "ZKey",
                                             vDialog, "Control", "ZKey",
                                             "" ) < zCURSOR_SET )
            {
               CreateEntity( vTZPNMAPP, "RegViewMapping", zPOS_AFTER );
               SetAttributeFromString( vTZPNMAPP, "RegViewMapping", "WindowTag",
                                       szWindowName );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "ControlDef",
                                          vDialog, "ControlDef", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "CtrlDefKey",
                                          vDialog, "ControlDef", "Key" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "Text",
                                          vDialog, "Control", "Text" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "Tag",
                                          vDialog, "Control", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "ZKey",
                                          vDialog, "Control", "ZKey" );
               SetAttributeFromString( vTZPNMAPP, "RegViewMapping", "ParentTag", szParentTag );
            }
         }

         GetStringFromAttribute( szControlTag, sizeof( szControlTag ), vDialog, "Control", "Tag" );
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         zwTZWDVORD_Mapp_CreateViewMap( vTZPNMAPP, vDialog, szWindowName, lZKey, szControlTag );
         ResetViewFromSubobject( vDialog );

      } while ( SetCursorNextEntity( vDialog, "Control", "" ) >= zCURSOR_SET );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_CreateViewMap

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_CreateEntityMap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_CreateEntityMap( zVIEW  vTZPNMAPP,
                                 zVIEW  vDialog,
                                 zPCHAR szWindowName,
                                 zLONG  lZKey,
                                 zPCHAR szParentTag )
{
   zCHAR   szControlTag[ 33 ] = { 0 };
   zSHORT  nRC;

   if ( SetCursorFirstEntity( vDialog, "Control", "" ) >= zCURSOR_SET )
   {
      do
      {
         for ( nRC = SetCursorFirstEntityByInteger( vDialog, "CtrlMapLOD_Entity",
                                                    "ZKey", lZKey, "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vDialog, "CtrlMapLOD_Entity",
                                                   "ZKey", lZKey, "" ) )
         {
            // create Work instance for controls with Entity mapping and
            // without Attribute mapping
            // (Control is for example a ListBox Control)
            if ( CompareAttributeToString( vDialog, "CtrlMapLOD_Entity", "Name",
                                           "" ) != 0 &&
                 CompareAttributeToAttribute( vDialog, "CtrlMapView", "ZKey",
                                              vTZPNMAPP, "RegView",
                                              "ZKey" ) == 0 &&
                 SetCursorFirstEntity( vDialog, "CtrlMapLOD_Attribute",
                                       "" ) < zCURSOR_SET &&
                 SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntityMapping",
                                             "ZKey", vDialog, "Control",
                                             "ZKey", "" ) < zCURSOR_SET )
            {
               CreateEntity( vTZPNMAPP, "RegEntityMapping", zPOS_AFTER );
               SetAttributeFromString( vTZPNMAPP, "RegEntityMapping", "WindowTag",
                                       szWindowName );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "ControlDef",
                                          vDialog, "ControlDef", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "CtrlDefKey",
                                          vDialog, "ControlDef", "Key" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "Text",
                                          vDialog, "Control", "Text" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "Tag",
                                          vDialog, "Control", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "ZKey",
                                          vDialog, "Control", "ZKey" );
               SetAttributeFromString( vTZPNMAPP, "RegEntityMapping", "ParentTag", szParentTag );
               // Create mapping instance for both the parent RegView
               CreateEntity( vTZPNMAPP, "RegViewMapping", zPOS_LAST );
               SetMatchingAttributesByName( vTZPNMAPP, "RegViewMapping",
                                            vTZPNMAPP, "RegEntityMapping", zSET_ALL );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_ZKey",
                                          vTZPNMAPP, "RegEntity", "ZKey" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_MappingIsCreated",
                                          vTZPNMAPP, "RegEntity", "MappingIsCreated" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_Name",
                                          vTZPNMAPP, "RegEntity", "Name" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_SequenceNo",
                                          vTZPNMAPP, "RegEntity", "SequenceNo" );
            }
         }

         GetStringFromAttribute( szControlTag, sizeof( szControlTag ), vDialog, "Control", "Tag" );
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         zwTZWDVORD_Mapp_CreateEntityMap( vTZPNMAPP, vDialog, szWindowName, lZKey, szControlTag );
         ResetViewFromSubobject( vDialog );

      } while ( SetCursorNextEntity( vDialog, "Control", "" ) >= zCURSOR_SET );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_CreateEntityMap

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_CreateAttribMap
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_CreateAttribMap( zVIEW  vTZPNMAPP,
                                 zVIEW  vDialog,
                                 zPCHAR szWindowName,
                                 zLONG  lZKey,
                                 zPCHAR szParentTag )
{
   zCHAR   szControlTag[ 33 ] = { 0 };
   zSHORT  nRC;

   if ( SetCursorFirstEntity( vDialog, "Control", "" ) >= zCURSOR_SET )
   {
      do
      {
         for ( nRC = SetCursorFirstEntityByInteger( vDialog, "CtrlMapER_Attribute",
                                                    "ZKey", lZKey, "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntityByInteger( vDialog, "CtrlMapER_Attribute",
                                                   "ZKey", lZKey, "" ) )
         {
            // create Work instance for controls with Attribute mapping
            // (Control is for example a Edit Control or ListBox SubControl)
            if ( CompareAttributeToString( vDialog, "CtrlMapER_Attribute", "Name",
                                           "" ) != 0 &&
                 CompareAttributeToAttribute( vDialog, "CtrlMapView", "ZKey",
                                              vTZPNMAPP, "RegView", "ZKey" ) == 0 &&
                 CompareAttributeToAttribute( vDialog, "CtrlMapRelatedEntity", "ZKey",
                                              vTZPNMAPP, "RegEntity", "ZKey" ) == 0 &&
                 SetCursorFirstEntityByAttr( vTZPNMAPP, "RegAttributeMapping", "ZKey",
                                             vDialog, "Control", "ZKey",
                                             "" ) < zCURSOR_SET )
            {
               CreateEntity( vTZPNMAPP, "RegAttributeMapping", zPOS_AFTER );
               SetAttributeFromString( vTZPNMAPP, "RegAttributeMapping", "WindowTag",
                                       szWindowName );
               SetAttributeFromAttribute( vTZPNMAPP, "RegAttributeMapping", "ControlDef",
                                          vDialog, "ControlDef", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegAttributeMapping", "CtrlDefKey",
                                          vDialog, "ControlDef", "Key" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegAttributeMapping", "Text",
                                          vDialog, "Control", "Text" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegAttributeMapping", "Tag",
                                          vDialog, "Control", "Tag" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegAttributeMapping", "ZKey",
                                          vDialog, "Control", "ZKey" );
               SetAttributeFromString( vTZPNMAPP, "RegAttributeMapping", "ParentTag", szParentTag );
               // Create mapping instances for both the parent RegEntity and RegView
               CreateEntity( vTZPNMAPP, "RegEntityMapping", zPOS_LAST );
               SetMatchingAttributesByName( vTZPNMAPP, "RegEntityMapping",
                                            vTZPNMAPP, "RegAttributeMapping", zSET_ALL );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "RegAttr_ZKey",
                                          vTZPNMAPP, "RegAttribute", "ZKey" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "RegAttr_MappingIsCreated",
                                          vTZPNMAPP, "RegAttribute", "MappingIsCreated" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "RegAttr_Name",
                                          vTZPNMAPP, "RegAttribute", "Name" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegEntityMapping", "RegAttr_SequenceNo",
                                          vTZPNMAPP, "RegAttribute", "SequenceNo" );
               CreateEntity( vTZPNMAPP, "RegViewMapping", zPOS_LAST );
               SetMatchingAttributesByName( vTZPNMAPP, "RegViewMapping",
                                            vTZPNMAPP, "RegEntityMapping", zSET_ALL );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_ZKey",
                                          vTZPNMAPP, "RegEntity", "ZKey" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_MappingIsCreated",
                                          vTZPNMAPP, "RegEntity", "MappingIsCreated" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_Name",
                                          vTZPNMAPP, "RegEntity", "Name" );
               SetAttributeFromAttribute( vTZPNMAPP, "RegViewMapping", "RegEntity_SequenceNo",
                                          vTZPNMAPP, "RegEntity", "SequenceNo" );
            }
         }

         GetStringFromAttribute( szControlTag, sizeof( szControlTag ), vDialog, "Control", "Tag" );
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         zwTZWDVORD_Mapp_CreateAttribMap( vTZPNMAPP, vDialog, szWindowName, lZKey, szControlTag );
         ResetViewFromSubobject( vDialog );

      } while ( SetCursorNextEntity( vDialog, "Control", "" ) >= zCURSOR_SET );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_CreateAttribMap

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_CreateMapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_CreateMapping( zVIEW  vTZPNMAPP,
                               zVIEW  vDialog,
                               zLONG  lZKey,
                               zSHORT nMappingType,
                               zSHORT nIsReport )
{
   zCHAR   szWindowName[ 71 ];
   zCHAR   szEntityName[ 33 ];
   zCHAR   szGoupName[ 33 ];
   zCHAR   szScope[ 33 ];
   zCHAR   szTemp[ 33 ];
   zSHORT  nRC;

   if ( nIsReport == 0 )   // Dialog Painter
   {
      strcpy_s( szEntityName, sizeof( szEntityName ), "Window" );
      strcpy_s( szGoupName, sizeof( szGoupName ), "Window" );
      strcpy_s( szScope, sizeof( szScope ), "" );
   }
   else  // Zeidon Reports
   {
      strcpy_s( szEntityName, sizeof( szEntityName ), "Group" );
      strcpy_s( szGoupName, sizeof( szGoupName ), "GroupSet" );
      strcpy_s( szScope, sizeof( szScope ), "Report" );
   }

   for ( nRC = SetCursorFirstEntity( vDialog, szEntityName, szScope );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, szEntityName, szScope ) )
   {
      GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vDialog, szGoupName, "Tag" );

      // if Zeidon Report, set Group Tag additional
      if ( nIsReport == 1 )
      {
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vDialog, szEntityName, "Tag" );
         strcat_s( szWindowName, sizeof( szWindowName ), " - " );
         strcat_s( szWindowName, sizeof( szWindowName ), szTemp );
      }

      if ( nMappingType == zViewMapping )
         zwTZWDVORD_Mapp_CreateViewMap( vTZPNMAPP, vDialog, szWindowName, lZKey, "" );
      else
      if ( nMappingType == zEntityMapping )
         zwTZWDVORD_Mapp_CreateEntityMap( vTZPNMAPP, vDialog, szWindowName, lZKey, "" );
      else
      if ( nMappingType == zAttributeMapping )
         zwTZWDVORD_Mapp_CreateAttribMap( vTZPNMAPP, vDialog, szWindowName, lZKey, "" );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_CreateMapping

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_ShowMapping
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_ShowMapping( zVIEW vSubtask )
{
   zVIEW    vTZPNMAPP;
   zVIEW    vMapTreeCtrl;
   zVIEW    vTZWINDOWL;
   zVIEW    vDialog;
   zVIEW    vOL = 0;
   zLONG    lZKey;
   zCHAR    szEntityName[ 33 ];
   zSHORT   nIsReport = 0;
   zSHORT   nRC;

   GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      nRC = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
      nIsReport = 1;

   szEntityName[ 0 ] = 0;

   if ( OL_GetCurrentEntityName( vSubtask, "ViewTree",
                                 &vMapTreeCtrl, szEntityName ) >= 0 )
   {
      CreateViewFromViewForTask( &vDialog, vTZWINDOWL, 0 );
      GetIntegerFromAttribute( &lZKey, vMapTreeCtrl, szEntityName, "ZKey" );

      // if Cursor on View Level
      if ( zstrcmp( szEntityName, "RegView" ) == 0 )
      {
         SetCtrlText( vSubtask, "txtTitle", " View Mapping" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                                     vMapTreeCtrl, "RegView", "ZKey", "" );

         // View mapping does not exists, create it
         if ( CompareAttributeToInteger( vTZPNMAPP, "RegView",
                                         "MappingIsCreated", 1 ) != 0 )
         {
            SetAttributeFromInteger( vTZPNMAPP, "RegView", "MappingIsCreated", 1 );
            zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                           zViewMapping, nIsReport );
         }
         SetCtrlState( vSubtask, "lbMappingView", zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "lbMappingEntity", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "lbMappingAttribute", zCONTROL_STATUS_VISIBLE, 0 );
         RefreshCtrl( vSubtask, "lbMappingView" );
      }
      else
      // if Cursor on Entity Level
      if ( zstrcmp( szEntityName, "RegEntity" ) == 0 )
      {
         SetCtrlText( vSubtask, "txtTitle", " Entity Mapping" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                                     vMapTreeCtrl, "RegView", "ZKey", "" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                                     vMapTreeCtrl, "RegEntity", "ZKey", "" );

         // Entity mapping does not exists, create it
         if ( CompareAttributeToInteger( vTZPNMAPP, "RegEntity", "MappingIsCreated", 1 ) != 0 )
         {
            SetAttributeFromInteger( vTZPNMAPP, "RegEntity", "MappingIsCreated", 1 );
            zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                           zEntityMapping, nIsReport );
         }
         SetCtrlState( vSubtask, "lbMappingView", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "lbMappingEntity", zCONTROL_STATUS_VISIBLE, 1 );
         SetCtrlState( vSubtask, "lbMappingAttribute", zCONTROL_STATUS_VISIBLE, 0 );
         RefreshCtrl( vSubtask, "lbMappingEntity" );
      }
      else
      // if Cursor on Attribute Level
      if ( zstrcmp( szEntityName, "RegAttribute" ) == 0 )
      {
         SetCtrlText( vSubtask, "txtTitle", " Attribute Mapping" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                                     vMapTreeCtrl, "RegView", "ZKey", "" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                                     vMapTreeCtrl, "RegEntity", "ZKey", "" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegAttribute", "ZKey",
                                     vMapTreeCtrl, "RegAttribute", "ZKey", "" );

         // Attribute mapping does not exists, create it
         if ( CompareAttributeToInteger( vTZPNMAPP, "RegAttribute",
                                         "MappingIsCreated", 1 ) != 0 )
         {
            SetAttributeFromInteger( vTZPNMAPP, "RegAttribute", "MappingIsCreated", 1 );
            zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                           zAttributeMapping, nIsReport );
         }
         SetCtrlState( vSubtask, "lbMappingView", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "lbMappingEntity", zCONTROL_STATUS_VISIBLE, 0 );
         SetCtrlState( vSubtask, "lbMappingAttribute", zCONTROL_STATUS_VISIBLE, 1 );
         RefreshCtrl( vSubtask, "lbMappingAttribute" );
      }

      DropView( vDialog );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_ShowMapping

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_DropWorkView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_DropWorkView( zVIEW vSubtask )
{
   zVIEW  vTZPNMAPP;
   zVIEW  vMapTreeCtrl;

   if ( GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vTZPNMAPP );

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMapTreeCtrl );

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl_Orig", vSubtask, zLEVEL_TASK ) >= 0 )
      DropObjectInstance( vMapTreeCtrl );

   return( 0 );

} // zwTZWDVORD_Mapp_DropWorkView

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_SortByName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_SortByName( zVIEW vSubtask )
{
   zVIEW  vMapTreeCtrl;
   zSHORT nRC;

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      OrderEntityForView( vMapTreeCtrl, "RegEntity", "Name A");

      for ( nRC = SetCursorFirstEntity( vMapTreeCtrl, "RegEntity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMapTreeCtrl, "RegEntity", "" ) )
      {
         OrderEntityForView( vMapTreeCtrl, "RegAttribute", "Name A");
      }

      SetAttributeFromInteger( vMapTreeCtrl, "RegView", "SortFlag", zSortByName );
      RefreshCtrl( vSubtask, "ViewTree" );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_SortByName

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_SortLodStructure
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_SortLodStructure( zVIEW vSubtask )
{
   zVIEW  vMapTreeCtrl;
   zSHORT nRC;

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      OrderEntityForView( vMapTreeCtrl, "RegEntity", "SequenceNo A");

      for ( nRC = SetCursorFirstEntity( vMapTreeCtrl, "RegEntity", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMapTreeCtrl, "RegEntity", "" ) )
      {
         OrderEntityForView( vMapTreeCtrl, "RegAttribute", "SequenceNo A");
      }

      SetAttributeFromInteger( vMapTreeCtrl, "RegView", "SortFlag", zSortByLodStructure );
      RefreshCtrl( vSubtask, "ViewTree" );
   }

   return( 0 );

} // zwTZWDVORD_Mapp_SortLodStructure

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_ShowAllAttribute
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_ShowAllAttribute( zVIEW vSubtask )
{
   zVIEW  vMapTreeCtrl;
   zVIEW  vTZWINDOWL;

   GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl_Orig", vSubtask, zLEVEL_TASK ) < 0 )
      GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );
   else
      SetNameForView( vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   // View does not exists in Worklod, create it
   if ( SetCursorFirstEntityByAttr( vMapTreeCtrl, "RegView", "ZKey",
                                    vTZWINDOWL, "ViewObjRef", "ZKey",
                                    "" ) < zCURSOR_SET )
   {
      zwTZWDVORD_Mapp_CreateTreeObject( vSubtask, vMapTreeCtrl, vTZWINDOWL );
   }

   SetAttributeFromInteger( vMapTreeCtrl, "RegView", "ShowFlag", zShowAllMapping );
   OL_SetCtrlState( vSubtask, "ViewTree", zCONTROL_STATUS_EXPANDALL, TRUE );

   return( 0 );

} // zwTZWDVORD_Mapp_ShowAllAttribute

static zBOOL
fnTZWDVORD_Mapp_DeleteAttr( zVIEW   vTZPNMAPP,
                            zVIEW   vMapTreeWithMapp,
                            zVIEW   vDialog,
                            zSHORT  nShowAttributes,
                            zSHORT  nIsReport )
{
   zBOOL  bFound = FALSE;
   zLONG  lZKey;
   zSHORT nRC;

   // delete Attributes with or without mapping
   for ( nRC = SetCursorFirstEntity( vMapTreeWithMapp, "RegAttribute", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMapTreeWithMapp, "RegAttribute", "" ) )
   {
      SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                                  vMapTreeWithMapp, "RegEntity", "ZKey", "" );
      SetCursorFirstEntityByAttr( vTZPNMAPP, "RegAttribute", "ZKey",
                                  vMapTreeWithMapp, "RegAttribute", "ZKey", "" );

      // Attribute mapping does not exists, create it
      if ( CompareAttributeToInteger( vTZPNMAPP, "RegAttribute", "MappingIsCreated", 1 ) != 0 )
      {
         SetAttributeFromInteger( vTZPNMAPP, "RegAttribute", "MappingIsCreated", 1 );
         GetIntegerFromAttribute( &lZKey, vTZPNMAPP, "RegAttribute", "ZKey" );
         zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey, zAttributeMapping, nIsReport );
      }
      if ( nShowAttributes == zShowWithMapping )
      {
         if ( SetCursorFirstEntity( vTZPNMAPP, "RegAttributeMapping", "" ) >= zCURSOR_SET )
         {
            bFound = TRUE;
         }
         else
         {
            DeleteEntity( vMapTreeWithMapp, "RegAttribute", zREPOS_NONE );
         }
      }
      else
      if ( nShowAttributes == zShowWithoutMapping )
      {
         if ( SetCursorFirstEntity( vTZPNMAPP, "RegAttributeMapping",
                                    "" ) < zCURSOR_SET )
         {
            bFound = TRUE;
         }
         else
         {
            DeleteEntity( vMapTreeWithMapp, "RegAttribute", zREPOS_NONE );
         }
      }
   } //endfor ( nRC = SetCursorFirstEntity( vMapTreeWithMapp, "RegAttribute", "" );

   return( bFound );

} // fnTZWDVORD_Mapp_DeleteAttr

static zSHORT
fnTZWDVORD_Mapp_DeleteEntity( zVIEW   vTZPNMAPP,
                              zVIEW   vMapTreeWithMapp,
                              zVIEW   vDialog,
                              zSHORT  nShowAttributes,
                              zSHORT  nIsReport )
{
   zLONG  lZKey;

   SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                               vMapTreeWithMapp, "RegEntity", "ZKey", "" );
   // Entity mapping does not exists, create it
   if ( CompareAttributeToInteger( vTZPNMAPP, "RegEntity",
                                   "MappingIsCreated", 1 ) != 0 )
   {
      SetAttributeFromInteger( vTZPNMAPP, "RegEntity", "MappingIsCreated", 1 );
      GetIntegerFromAttribute( &lZKey, vTZPNMAPP, "RegEntity", "ZKey" );
      zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                     zEntityMapping, nIsReport );
   }
   if ( nShowAttributes == zShowWithMapping )
   {
      if ( SetCursorFirstEntity( vTZPNMAPP, "RegEntityMapping",
                                 "" ) < zCURSOR_SET )
      {
         DeleteEntity( vMapTreeWithMapp, "RegEntity", zREPOS_NONE );
      }
   }
   else
   if ( nShowAttributes == zShowWithoutMapping )
   {
      if ( SetCursorFirstEntity( vTZPNMAPP, "RegEntityMapping",
                                 "" ) >= zCURSOR_SET )
      {
         DeleteEntity( vMapTreeWithMapp, "RegEntity", zREPOS_NONE );
      }
   }

   return( 0 );
} // fnTZWDVORD_Mapp_DeleteEntity

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_AttrMapp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_Mapp_AttrMapp( zVIEW   vSubtask,
                          zSHORT  nShowAttributes )

{
   zVIEW  vTZPNMAPP;
   zVIEW  vMapTreeCtrl;
   zVIEW  vMapTreeWithMapp;
   zVIEW  vTZWINDOWL;
   zVIEW  vDialog;
   zBOOL  bFound;
   zSHORT nIsReport = 0;
   zSHORT nRC;

   if ( GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      nRC = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
      nIsReport = 1;

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl_Orig", vSubtask, zLEVEL_TASK ) < 0 )
      GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vDialog, vTZWINDOWL, 0 );

   ActivateOI_FromOI_ForTask( &vMapTreeWithMapp, vMapTreeCtrl, 0, zMULTIPLE );
   SetNameForView( vMapTreeWithMapp, "MapTreeWithMapp", vSubtask, zLEVEL_TASK );

   // View does not exists in Worklod, create it
   if ( SetCursorFirstEntityByAttr( vMapTreeWithMapp, "RegView", "ZKey",
                                    vTZWINDOWL, "ViewObjRef", "ZKey",
                                    "" ) < zCURSOR_SET )
   {
      zwTZWDVORD_Mapp_CreateTreeObject( vSubtask, vMapTreeWithMapp, vTZWINDOWL );
   }

   for ( nRC = SetCursorFirstEntity( vMapTreeWithMapp, "RegEntity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMapTreeWithMapp, "RegEntity", "" ) )
   {
      // delete Attributes with or without mapping
      bFound = fnTZWDVORD_Mapp_DeleteAttr( vTZPNMAPP, vMapTreeWithMapp,
                                           vDialog, nShowAttributes, nIsReport );

      // delete Entity with or without mapping
      if ( !bFound )
         fnTZWDVORD_Mapp_DeleteEntity( vTZPNMAPP, vMapTreeWithMapp,
                                       vDialog, nShowAttributes, nIsReport );
   }//endfor ( nRC = SetCursorFirstEntity( vMapTreeWithMapp, "RegEntity", "" );

   DropView( vDialog );
   SetAttributeFromInteger( vMapTreeWithMapp, "RegView", "ShowFlag", nShowAttributes );

   // switch Views
   SetNameForView( vMapTreeCtrl, "MapTreeCtrl_Orig", vSubtask, zLEVEL_TASK );
   SetNameForView( vMapTreeWithMapp, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   OL_SetCtrlState( vSubtask, "ViewTree", zCONTROL_STATUS_EXPANDALL, TRUE );
   return( 0 );

} // zwTZWDVORD_Mapp_AttrMapp

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_AttrWithMapp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_AttrWithMapp( zVIEW vSubtask )
{
   zwTZWDVORD_Mapp_AttrMapp( vSubtask, zShowWithMapping );
   return( 0 );

} // zwTZWDVORD_Mapp_AttrWithMapp

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_Mapp_AttrWithoutMapp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_AttrWithoutMapp( zVIEW vSubtask )
{
   zwTZWDVORD_Mapp_AttrMapp( vSubtask, zShowWithoutMapping );
   return( 0 );

} // zwTZWDVORD_Mapp_AttrWithoutMapp

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZWDVORD_Mapp_EntireView
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_Mapp_EntireView( zVIEW vSubtask )
{
   zVIEW    vTZPNMAPP;
   zVIEW    vMapTreeCtrl;
   zVIEW    vTZWINDOWL;
   zVIEW    vDialog;
   zVIEW    vOL = 0;
   zLONG    lZKey;
   zCHAR    szEntityName[ 33 ];
   zSHORT   nIsReport = 0;
   zSHORT   nRC2;
   zSHORT   nRC;

   GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK );
   GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      nRC = GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   else
      nIsReport = 1;

   szEntityName[ 0 ] = 0;

   CreateViewFromViewForTask( &vDialog, vTZWINDOWL, 0 );
   GetIntegerFromAttribute( &lZKey, vMapTreeCtrl, "RegView", "ZKey" );

   // Map View Level
   SetCtrlText( vSubtask, "txtTitle", " View Mapping" );
   SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                               vMapTreeCtrl, "RegView", "ZKey", "" );

   // View mapping does not exists, create it
   if ( CompareAttributeToInteger( vTZPNMAPP, "RegView",
                                   "MappingIsCreated", 1 ) != 0 )
   {
      SetAttributeFromInteger( vTZPNMAPP, "RegView", "MappingIsCreated", 1 );
      zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                     zViewMapping, nIsReport );
   }

   // Map all Entity Levels
   for ( nRC = SetCursorFirstEntity( vMapTreeCtrl, "RegEntity", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMapTreeCtrl, "RegEntity", "" ) )
   {
      GetIntegerFromAttribute( &lZKey, vMapTreeCtrl, "RegEntity", "ZKey" );
      SetCtrlText( vSubtask, "txtTitle", " Entity Mapping" );
      SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                                  vMapTreeCtrl, "RegView", "ZKey", "" );
      SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                                  vMapTreeCtrl, "RegEntity", "ZKey", "" );

      // Entity mapping does not exists, create it
      if ( CompareAttributeToInteger( vTZPNMAPP, "RegEntity",
                                      "MappingIsCreated", 1 ) != 0 )
      {
         SetAttributeFromInteger( vTZPNMAPP, "RegEntity", "MappingIsCreated", 1 );
         zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                        zEntityMapping, nIsReport );
      }
      // Map all Attribute Level
      for ( nRC2 = SetCursorFirstEntity( vMapTreeCtrl, "RegAttribute", "" );
            nRC2 >= zCURSOR_SET;
            nRC2 = SetCursorNextEntity( vMapTreeCtrl, "RegAttribute", "" ) )
      {
         GetIntegerFromAttribute( &lZKey, vMapTreeCtrl, "RegAttribute", "ZKey" );
         SetCtrlText( vSubtask, "txtTitle", " Attribute Mapping" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegView", "ZKey",
                                     vMapTreeCtrl, "RegView", "ZKey", "" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegEntity", "ZKey",
                                     vMapTreeCtrl, "RegEntity", "ZKey", "" );
         SetCursorFirstEntityByAttr( vTZPNMAPP, "RegAttribute", "ZKey",
                                     vMapTreeCtrl, "RegAttribute", "ZKey", "" );

         // Attribute mapping does not exists, create it
         if ( CompareAttributeToInteger( vTZPNMAPP, "RegAttribute",
                                         "MappingIsCreated", 1 ) != 0 )
         {
            SetAttributeFromInteger( vTZPNMAPP, "RegAttribute", "MappingIsCreated", 1 );
            zwTZWDVORD_Mapp_CreateMapping( vTZPNMAPP, vDialog, lZKey,
                                           zAttributeMapping, nIsReport );
         }
      } // Attribute Level
   } // Entity Level

   DropView( vDialog );

   OL_SelectItemAtPosForEntity( vSubtask, "ViewTree", "RegView", 32 );

   zwTZWDVORD_Mapp_ShowMapping( vSubtask );
   return( 0 );

} // zwTZWDVORD_Mapp_EntireView

static zSHORT
fnTZWDVORD_SetCursorToCtrl( zVIEW vControl, zULONG ulZKey )
{
   zSHORT  nRC;

   if ( CompareAttributeToInteger( vControl, "Control", "ZKey",
                                   ulZKey ) == 0 )
   {
      return( zCURSOR_SET );
   }

   nRC = SetCursorFirstEntity( vControl, "CtrlCtrl", "" );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetViewToSubobject( vControl, "CtrlCtrl" );
      if ( fnTZWDVORD_SetCursorToCtrl( vControl, ulZKey ) == zCURSOR_SET )
         return( zCURSOR_SET );

      nRC = ResetViewFromSubobject( vControl );
      nRC = SetCursorNextEntity( vControl, "CtrlCtrl", "" );
   }

   return( -1 );
}

static zSHORT
fnTZWDVORD_ControlViewCursor( zVIEW vControl, zULONG ulZKey, zPCHAR szScope )
{
   zSHORT  nRC;

   nRC = SetCursorFirstEntity( vControl, "Control", szScope );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = fnTZWDVORD_SetCursorToCtrl( vControl, ulZKey );
      if ( nRC == zCURSOR_SET )
         return( nRC );

      nRC = SetCursorNextEntity( vControl, "Control", szScope );
   }

   return( -1 );
}

static zSHORT
fnTZWDVORD_LoadControlDetailWnd( zVIEW vSubtask, zVIEW vTZPNMAPP, zPCHAR szEntityName )
{
   zVIEW  vPE;
   zVIEW  vMapTreeCtrl;
   zVIEW  vView;
   zCHAR  szTreeEntity[ 33 ];
   zPCHAR pchDialog;
   zPCHAR pchWindow;

   if ( GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      TraceLineS( "LoadControlDetailWnd unable to get PE view", "" );
      return( -1 );
   }

   if ( OL_GetCurrentEntityName( vSubtask, "ViewTree",
                                 &vView, szTreeEntity ) < 0 )
   {
      return( -1 );
   }

   if ( SetEntityCursor( vPE, "ControlDef", "Key",
                         zPOS_FIRST | zQUAL_ENTITYATTR,
                         vTZPNMAPP, szEntityName, "CtrlDefKey",
                         0, 0, 0 ) >= zCURSOR_SET )
   {
      if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK ) > 0 )
         SetCursorFirstEntityByAttr( vMapTreeCtrl, szTreeEntity, "ZKey",
                                     vTZPNMAPP, szTreeEntity, "ZKey", "" );

      GetAddrForAttribute( &pchDialog, vPE,
                           "ControlDef", "MaintenanceDialog" );
      GetAddrForAttribute( &pchWindow, vPE,
                           "ControlDef", "MaintenanceWindow" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               pchDialog, pchWindow );
      return( 0 );
   }

   return( -1 );
}

static zSHORT
fnTZWDVORD_BuildTZPNEVWO( zVIEW vSubtask, zVIEW vControl )
{
   zVIEW   vTZPESRCO;
   zVIEW   vCtrlTemp;
   zVIEW   vEvents;
   zLONG   lType;
   zLONG   lEventType;
   zSHORT  nRC;

   if ( GetViewByName( &vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vEvents );

   if ( vControl == 0 ||
        GetViewByName( &vTZPESRCO, "TZPESRCO", vSubtask, zLEVEL_TASK ) < 1 )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vControl, "ControlDef" ) <= zCURSOR_UNCHANGED )
      return( -1 );

   CreateViewFromViewForTask( &vCtrlTemp, vControl, 0 );
   while ( ResetViewFromSubobject( vCtrlTemp ) == 0 )
   {
   }

   ActivateEmptyObjectInstance( &vEvents, "TZPNEVWO", vSubtask, zMULTIPLE );
   SetNameForView( vEvents, "TZPNEVWO", vSubtask, zLEVEL_TASK );

   // Now copy all the events from the ctrl definition to the event work
   // object.
   GetIntegerFromAttribute( &lType, vControl, "ControlDef", "Key" );
   SetCursorFirstEntityByInteger( vTZPESRCO, "ControlDef", "Key", lType, 0 );

   nRC = SetCursorFirstEntity( vTZPESRCO, "CtrlEventDef", 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      CreateEntity( vEvents, "Event", zPOS_AFTER );
      SetMatchingAttributesByName( vEvents, "Event",
                                   vTZPESRCO, "CtrlEventDef", zSET_ALL );
      CreateEntity( vEvents, "EventAct", zPOS_AFTER );
      GetIntegerFromAttribute( &lEventType, vEvents, "Event", "Type" );
      if ( SetCursorFirstEntityByInteger( vControl, "Event", "Type",
                                          lEventType, 0 ) > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vControl, "EventAct" ) > zCURSOR_UNCHANGED )
         {
            SetMatchingAttributesByName( vEvents, "EventAct",
                                         vControl, "EventAct", zSET_ALL );
            SetCursorFirstEntityByEntityCsr( vCtrlTemp, "Action",
                                             vControl, "EventAct", "" );

            if ( CheckExistenceOfEntity( vCtrlTemp,
                                         "ActOper" ) > zCURSOR_UNCHANGED )
            {
               SetAttributeFromAttribute( vEvents, "EventAct", "Operation",
                                          vCtrlTemp, "ActOper", "Name" );
            }
         }
      }

      nRC = SetCursorNextEntity( vTZPESRCO, "CtrlEventDef", 0 );
   }

   SetCursorFirstEntity( vTZPESRCO, "CtrlEventDef", 0 );

   if ( CheckExistenceOfEntity( vEvents, "Event" ) > zCURSOR_UNCHANGED )
      SetCursorFirstEntity( vEvents, "Event", 0 );

   DropView( vCtrlTemp );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_LoadControlDetailWnd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZWDVORD_LoadControlDetailWnd( zVIEW vSubtask, zPCHAR szEntityName )
{
   zULONG ulZKey;
   zSHORT nRC;
   zVIEW  vTZPNMAPP;
   zVIEW  vTZWINDOWL;
   zVIEW  vTZWINDOW;
   zVIEW  vControl;
   zCHAR  szScope[ 33 ];
   zBOOL  bIsReport = FALSE;

   if ( GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   nRC = GetViewByName( &vTZWINDOWL, "PAGEL", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
   {
      strcpy_s( szScope, sizeof( szScope ), "" );
      GetViewByName( &vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_ANY );
   }
   else
   {
      strcpy_s( szScope, sizeof( szScope ), "Report" );
      bIsReport = TRUE;
      SetNameForView( vTZWINDOWL, "TZWINDOWL", vSubtask, zLEVEL_TASK );
   }
   while ( ResetViewFromSubobject( vTZWINDOWL ) == 0 );

   // set Cursor to selected Window
   if ( bIsReport == FALSE &&
        SetCursorFirstEntityByAttr( vTZWINDOWL, "Window", "Tag",
                                    vTZPNMAPP, szEntityName, "WindowTag",
                                    "" ) < zCURSOR_SET )
   {
      return( -1 );
   }

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vControl );
   if ( GetViewByName( &vControl, "TZACTION", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vControl );

   CreateViewFromViewForTask( &vControl, vTZWINDOWL, 0 );
   SetNameForView( vControl, "TZCONTROL", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTZWINDOW, "TZWINDOW", vSubtask, zLEVEL_TASK ) < 0 )
   {
      CreateViewFromViewForTask( &vTZWINDOW, vTZWINDOWL, 0 );
      SetNameForView( vTZWINDOW, "TZWINDOW", vSubtask, zLEVEL_TASK );
   }

   GetIntegerFromAttribute( (zPLONG) &ulZKey, vTZPNMAPP, szEntityName, "ZKey" );

   if ( fnTZWDVORD_ControlViewCursor( vControl, ulZKey, szScope ) < 0 )
      return( -1 );

   // Build the TZPNEVWO object for the control events.
   fnTZWDVORD_BuildTZPNEVWO( vSubtask, vControl );

   nRC = fnTZWDVORD_LoadControlDetailWnd( vSubtask, vTZPNMAPP, szEntityName );
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_LoadCtrlDetail_Entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_LoadCtrlDetail_Entity( zVIEW vSubtask )
{
   return( zwTZWDVORD_LoadControlDetailWnd( vSubtask, "RegEntityMapping" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_LoadCtrlDetail_View
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_LoadCtrlDetail_View( zVIEW vSubtask )
{
   return( zwTZWDVORD_LoadControlDetailWnd( vSubtask, "RegViewMapping" ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_LoadCtrlDetail_Attrib
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_LoadCtrlDetail_Attrib( zVIEW vSubtask )
{
   return( zwTZWDVORD_LoadControlDetailWnd( vSubtask, "RegAttributeMapping" ) );
}

static zBOOL
fnTZWDVORD_IsChangeMapping( zVIEW vControl, zVIEW vTZPNMAPP, zPCHAR szTreeEntity )
{
   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) < zCURSOR_SET ||
        CompareAttributeToAttribute( vTZPNMAPP, "RegView", "ZKey",
                                     vControl, "CtrlMapView", "ZKey" ) != 0 )
   {
      return( TRUE );
   }

   if ( zstrcmp( szTreeEntity, "RegViewMapping" ) == 0 &&
        CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) >= zCURSOR_SET )
   {
      return( TRUE );
   }

   if ( zstrcmp( szTreeEntity, "RegEntityMapping" ) == 0 &&
        (CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) < zCURSOR_SET ||
         CompareAttributeToAttribute( vTZPNMAPP, "RegEntity", "ZKey",
                                      vControl, "CtrlMapLOD_Entity", "ZKey" ) != 0) )
   {
      return( TRUE );
   }

   if ( zstrcmp( szTreeEntity, "RegAttributeMapping" ) == 0 &&
        (CheckExistenceOfEntity( vControl, "CtrlMapER_Attribute" ) < zCURSOR_SET ||
         CompareAttributeToAttribute( vTZPNMAPP, "RegAttribute", "ZKey",
                                      vControl, "CtrlMapER_Attribute", "ZKey" ) != 0) )
   {
      return( TRUE );
   }

   return( FALSE );
}

static void
fnTZWDVORD_ChangeViewMapping( zVIEW vSubtask, zVIEW vControl,
                              zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old )
{
   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) >= zCURSOR_SET &&
        CompareAttributeToString( vControl, "CtrlMapView", "Name", "" ) != 0 &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegView", "ZKey",
                                    vControl, "CtrlMapView", "ZKey", "" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntity( vControl, "CtrlMapLOD_Entity", "" ) < zCURSOR_SET )
      {
         CreateEntity( vTZPNMAPP_new, "RegViewMapping", zPOS_AFTER );
         SetMatchingAttributesByName( vTZPNMAPP_new, "RegViewMapping",
                                      vTZPNMAPP_old, "RegViewMapping", zSET_ALL );
      }
      else
      if ( CompareAttributeToString( vControl, "CtrlMapLOD_Entity", "Name", "" ) != 0 &&
           SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegEntity", "ZKey",
                                       vControl, "CtrlMapLOD_Entity", "ZKey", "" ) >= zCURSOR_SET )
      {
         CreateEntity( vTZPNMAPP_new, "RegEntityMapping", zPOS_AFTER );
         SetMatchingAttributesByName( vTZPNMAPP_new, "RegEntityMapping",
                                      vTZPNMAPP_old, "RegViewMapping", zSET_ALL );
      }
   }

   DeleteEntity( vTZPNMAPP_old, "RegViewMapping", zREPOS_PREV );
   RefreshCtrl( vSubtask, "lbMappingView" );
}

static void
fnTZWDVORD_ChangeEntityMapping( zVIEW vSubtask, zVIEW vControl,
                                zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old )
{
   if ( CheckExistenceOfEntity( vControl, "CtrlMapLOD_Entity" ) >= zCURSOR_SET &&
        CompareAttributeToString( vControl, "CtrlMapLOD_Entity", "Name",
                                  "" ) != 0 &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegView", "ZKey",
                                    vControl, "CtrlMapView", "ZKey",
                                    "" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegEntity", "ZKey",
                                     vControl, "CtrlMapLOD_Entity", "ZKey",
                                     "" ) >= zCURSOR_SET )
   {
      CreateEntity( vTZPNMAPP_new, "RegEntityMapping", zPOS_AFTER );
      SetMatchingAttributesByName( vTZPNMAPP_new, "RegEntityMapping",
                                   vTZPNMAPP_old, "RegEntityMapping",
                                   zSET_ALL );
   }
   else
   if ( CheckExistenceOfEntity( vControl, "CtrlMapView" ) >= zCURSOR_SET &&
        CompareAttributeToString( vControl, "CtrlMapView", "Name",
                                  "" ) != 0 &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegView", "ZKey",
                                    vControl, "CtrlMapView", "ZKey",
                                    "" ) >= zCURSOR_SET )
   {
      CreateEntity( vTZPNMAPP_new, "RegViewMapping", zPOS_AFTER );
      SetMatchingAttributesByName( vTZPNMAPP_new, "RegViewMapping",
                                   vTZPNMAPP_old, "RegEntityMapping",
                                   zSET_ALL );
   }

   DeleteEntity( vTZPNMAPP_old, "RegEntityMapping", zREPOS_PREV );
   RefreshCtrl( vSubtask, "lbMappingEntity" );
}

static void
fnTZWDVORD_ChangeAttributeMapping( zVIEW vSubtask, zVIEW vControl,
                                   zVIEW vTZPNMAPP_new, zVIEW vTZPNMAPP_old )
{
   if ( CheckExistenceOfEntity( vControl, "CtrlMapER_Attribute" ) >= zCURSOR_SET &&
        CompareAttributeToString( vControl, "CtrlMapER_Attribute", "Name",
                                  "" ) != 0 &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegView", "ZKey",
                                    vControl, "CtrlMapView", "ZKey",
                                    "" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegEntity", "ZKey",
                                     vControl, "CtrlMapRelatedEntity", "ZKey",
                                     "" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByAttr( vTZPNMAPP_new, "RegAttribute", "ZKey",
                                     vControl, "CtrlMapER_Attribute", "ZKey",
                                     "" ) >= zCURSOR_SET )
   {
      CreateEntity( vTZPNMAPP_new, "RegAttributeMapping", zPOS_AFTER );
      SetMatchingAttributesByName( vTZPNMAPP_new, "RegAttributeMapping",
                                   vTZPNMAPP_old, "RegAttributeMapping", zSET_ALL );
   }

   DeleteEntity( vTZPNMAPP_old, "RegAttributeMapping", zREPOS_PREV );
   RefreshCtrl( vSubtask, "lbMappingAttribute" );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZWDVORD_ReturnFromSubwindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZWDVORD_ReturnFromSubwindow( zVIEW vSubtask )
{
   zVIEW  vView;
   zVIEW  vMapTreeCtrl;
   zVIEW  vTZPNMAPP;
   zVIEW  vTZPNMAPP_old;
   zVIEW  vTZPNMAPP_new;
   zVIEW  vControl;
   zCHAR  szTreeEntity[ 33 ];
   zCHAR  szMappingEntity[ 33 ];

   if ( GetViewByName( &vTZPNMAPP, "TZPNMAPP", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( GetViewByName( &vControl, "TZCONTROL", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( OL_GetCurrentEntityName( vSubtask, "ViewTree", &vView, szTreeEntity ) < 0 )
   {
      return( -1 );
   }

   strcpy_s( szMappingEntity, sizeof( szMappingEntity ), szTreeEntity );
   strcat_s( szMappingEntity, sizeof( szMappingEntity ), "Mapping" );
   SetAttributeFromAttribute( vTZPNMAPP, szMappingEntity, "Text",
                              vControl, "Control", "Text" );
   SetAttributeFromAttribute( vTZPNMAPP, szMappingEntity, "Tag",
                              vControl, "Control", "Tag" );

   // if change Mapping
   if ( fnTZWDVORD_IsChangeMapping( vControl, vTZPNMAPP, szMappingEntity ) == FALSE )
      return( 0 );

   CreateViewFromViewForTask( &vTZPNMAPP_old, vTZPNMAPP, 0 );
   CreateViewFromViewForTask( &vTZPNMAPP_new, vTZPNMAPP, 0 );

   if ( zstrcmp( szMappingEntity, "RegViewMapping" ) == 0 )
   {
      fnTZWDVORD_ChangeViewMapping( vSubtask, vControl, vTZPNMAPP_new, vTZPNMAPP_old );
   }
   else
   if ( zstrcmp( szMappingEntity, "RegEntityMapping" ) == 0 )
   {
      fnTZWDVORD_ChangeEntityMapping( vSubtask, vControl,
                                      vTZPNMAPP_new, vTZPNMAPP_old );
   }
   else
   if ( zstrcmp( szMappingEntity, "RegAttributeMapping" ) == 0 )
   {
      fnTZWDVORD_ChangeAttributeMapping( vSubtask, vControl,
                                         vTZPNMAPP_new, vTZPNMAPP_old );
   }

   if ( GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( CompareAttributeToInteger( vMapTreeCtrl, "RegView", "ShowFlag",
                                      zShowWithMapping ) == 0 )
      {
         zwTZWDVORD_Mapp_AttrMapp( vSubtask, zShowWithMapping );
      }
      else
      if ( CompareAttributeToInteger( vMapTreeCtrl, "RegView", "ShowFlag",
                                      zShowWithoutMapping ) == 0 )
      {
         zwTZWDVORD_Mapp_AttrMapp( vSubtask, zShowWithoutMapping );
      }

      GetViewByName( &vMapTreeCtrl, "MapTreeCtrl", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByAttr( vMapTreeCtrl, szTreeEntity, "ZKey",
                                       vTZPNMAPP_old, szTreeEntity, "ZKey",
                                       "RegView" ) < zCURSOR_SET )
      {
         SetCursorFirstEntity( vMapTreeCtrl, "RegEntity", "" );
      }
   }

   DropView( vTZPNMAPP_old );
   DropView( vTZPNMAPP_new );

   return( 0 );

} // zwTZWDVORD_ReturnFromSubwindow

#ifdef __cplusplus
}
#endif
