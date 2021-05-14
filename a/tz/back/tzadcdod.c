/*
CHANGE LOG
2001.06.16 PAS
    New context menu for entity list when UIS Type is Report.  Also changed context
    menu for Windows to allow setting to None without using combobox window.
    NOTE  - many blank lines deleted within comments - editor did it!
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.19    SR   Z2000 Size of path variables
   Modified size of string szFileName in functions zwTZADCDOD_TransferToDefault,
   zwTZADCDOD_DfltWndSpecOK and  zwTZADCDOD_NewDialogSpec because size of attributes
   ExecDir, MetaSrcDir and PgmSrcDir has been changed to 254 in datamodel.
2000.10.08  BL  Z10  TB 50704, TB 50706
   If ReInit UIS, set Report Defaults
2000.06.30  BL  Z10
   Modified zwTZADCDOD_ProcessCaseForEntity for check existence of ER_Entity
1999.11.25  BL  Z10  new menu structure
   Modified Operation zwTZADCDOD_InitStyleDialogue, zwTZADCDOD_SaveSaveAs,
   zwTZADCDOD_OpenStyleFile, zwTZADCDOD_CreateNewStyle for set LPLR Name in Title
   Modified Operation zwTZADCDOD_DeleteListAttrib, zwTZADCDOD_DeleteMaintAttrib,
   zwTZADCDOD_Delete_I_LOD_Attrib and zwTZADCDOD_DeletePaintAttribute for ask
   for delete only one
   Modified Operation zwTZADCDOD_InitGrayOptions for disable menu items
from here old version
25.07.1997 DonC
   Modified zwTZADCDOD_IncludeListVOR so that ListVORs could be excluded.
25.07.1997 DonC
   Modified DeletePaintAttribute to remove message prompt for delete.
25.07.1997 DonC
   Created NewDialogSpec operation to make sure that a Default Spec exists
   if a new Dialog Spec is created.
14.10.1997 DonC
   Modified entry to Dialog Spec to sort VOR list by name so that combo boxes would
   show better.
01.12.1997 DonC
   Modified ProcessCaseForEntity to handle case when ER_RelLink doesn't exist.
14.02.1998 DonC
   Modified zwTZADCDOD_ProcessCaseForEntity so that LOD with Assoc root
   entity would initialize any included subentities as Case 6. (QS 149)
19.04.1998 DonC
   Modified zwTZADCDOD_OpenStyleFile and added operation
   zwTZADCDOD_AddEntitiesToStyle to add any new entities in the LOD into the
   User Spec.
22.06.1998 DonC
   Modified SelectCase so that switching back and forth between tab and
   non-tab types did not present invalid error messages.
22.06.1998 DonC
   Modified AddEntitiesToStyle to do check on LOD information instead of ER
   information. Check on ER information did not work in some cases.
01.07.1998 DonC
   Modified DeletePaintAttribute so that Delete button would be correctly
   grayed.
01.07.1998 DKS
   ExitNoSave now does the same thing as ExitStyleDialog except that there
   is no save/cancel confirmation.
19.07.1998 DonC
   Changed several operations (along with adding FlatAttribute entity to
   TZADWKLO lod) so that the order of Attribute entries being added in
   MAINSEL, LISTSEL and INCLSEL windows could be controlled.
19.07.1998 DonC
   Corrected zwTZADCDOD_AddEntitiesToStyle to handle case where LOD entities
   have been deleted and readded.
19.07.1998 DKS
   ClearCase pretty it up.
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

// LAST MESSAGE ID: AD10042

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CreateDialogStyle( zVIEW    vSubtask,
                              zVIEW    vVOR_Obj,
                              zVIEW    vUserSpec,
                              zPCHAR   pchStyleName,
                              zVIEW    vObjectObj );

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CreateReportStyle( zVIEW    vSubtask,
                              zVIEW    vVOR_Obj,
                              zVIEW    vUserSpec,
                              zPCHAR   pchStyleName,
                              zVIEW    vObjectObj );
zOPER_EXPORT zVOID OPERATION
ofnTZADCSDO_fnSetWorkAttributes( zVIEW vUserSpecH,
                                 zVIEW vUIS_List );
zOPER_EXPORT zSHORT OPERATION
ofnTZADCSDO_SaveAttribSelection( zVIEW  vSubtask,
                                 zVIEW  vObjectObj,
                                 zPCHAR szEntityAttrib );
zOPER_EXPORT zSHORT OPERATION
ofnTZADCSDO_SelectEntityForUpdt( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectSubObject ( zVIEW vSubtask, zPCHAR szEntityName );


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectPaintEntity
// PURPOSE:  This function Selects one or more Attributes to be added
//           to the FlatList work object that defines Attributes for
//           inclusion in dialogs.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectPaintEntity( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vObjectObj;
   zVIEW    vFlatList;
   zVIEW    vFlatList2;
   zSHORT   nRC;
   zBOOL    bMatch;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vFlatList, "FlatList", vSubtask, zLEVEL_TASK );

   // Use the correct LOD object based on the ListTypeFlag.
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "ListTypeFlag", "L" ) == 0 )
      GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   else
      GetViewByName( &vObjectObj, "INCL_LOD_Object", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vFlatList, "FlatAttribute" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, TRUE );

   // Create a temporary view so as not to lose position.
   CreateViewFromViewForTask( &vFlatList2, vFlatList, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vObjectObj, "ER_Attribute", "LOD_Entity" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vObjectObj, "ER_Attribute", "LOD_Entity" ))
   {
      bMatch = FALSE;
      // Check if entry already exists.
      for ( nRC = SetCursorFirstEntity( vFlatList2, "FlatAttribute", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vFlatList2, "FlatAttribute", 0 ))
      {
         if ( CompareAttributeToAttribute( vFlatList2, "FlatAttribute", "AttributeName",
                                           vObjectObj, "ER_Attribute",  "Name" ) == 0 &&
              CompareAttributeToAttribute( vFlatList2, "FlatAttribute", "EntityName",
                                           vObjectObj, "LOD_Entity",    "Name" ) == 0 )
         {
            bMatch = TRUE;
         }
      }

      if ( ! bMatch )
      {
         // It's not in the list so add it.
         CreateEntity( vFlatList, "FlatAttribute", zPOS_AFTER );
         SetAttributeFromAttribute( vFlatList, "FlatAttribute", "AttributeName",
                                    vObjectObj, "ER_Attribute", "Name" );
         SetAttributeFromAttribute( vFlatList, "FlatAttribute", "EntityName",
                                    vObjectObj, "LOD_Entity",   "Name" );
      }
      SetSelectStateOfEntityForSet( vObjectObj, "ER_Attribute", 0, 1 );
   }

   // Gray the Delete button if nothing is selected.
   nRC = SetCursorFirstSelectedEntity( vFlatList2, "FlatAttribute", 0 );
   if ( nRC < zCURSOR_SET )
      SetCtrlState( vSubtask, "Delete",
                       zCONTROL_STATUS_ENABLED, FALSE );
   else
      SetCtrlState( vSubtask, "Delete",
                       zCONTROL_STATUS_ENABLED, TRUE );

   DropView( vFlatList2 );
   RefreshCtrl( vSubtask, "ListBox1" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeletePaintAttribute
// PURPOSE:  This function Deletes one or more Attributes from the Paint
//           Attribute List.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeletePaintAttribute( zVIEW vSubtask )
{
   zVIEW    vFlatList;
   zSHORT   nRC;

   GetViewByName( &vFlatList, "FlatList", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vFlatList, "FlatAttribute", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vFlatList, "FlatAttribute", "" ) )
   {        // Ask the operator if it is ok to delete.

      DeleteEntity( vFlatList, "FlatAttribute", zREPOS_NONE );
   }

   nRC = SetCursorFirstEntity( vFlatList, "FlatAttribute", "" );

   RefreshCtrl( vSubtask, "ATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    GraySelAttribDelete
// PURPOSE:  We need to do the graying by hand instead of using autogray
//           for the action because autogray also grays another list box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_GraySelAttribDelete( zVIEW vSubtask )
{
   zVIEW    vFlatList;
   zSHORT   nRC;

   GetViewByName( &vFlatList, "FlatList", vSubtask, zLEVEL_TASK );

   // Gray the Delete button if nothing is selected.
   nRC = SetCursorFirstSelectedEntity( vFlatList, "FlatAttribute", 0 );
   if ( nRC < zCURSOR_SET )
      SetCtrlState( vSubtask, "Delete",
                       zCONTROL_STATUS_ENABLED, FALSE );
   else
      SetCtrlState( vSubtask, "Delete",
                       zCONTROL_STATUS_ENABLED, TRUE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SaveSelListAttribs( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vObjectObj;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity", "ListTypeFlag", "L" ) == 0 )
   {
      GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
      ofnTZADCSDO_SaveAttribSelection( vSubtask, vObjectObj, "L_LOD_Attribute" );
   }
   else
   {
      GetViewByName( &vObjectObj, "INCL_LOD_Object", vSubtask, zLEVEL_TASK );
      ofnTZADCSDO_SaveAttribSelection( vSubtask, vObjectObj, "I_LOD_Attribute" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SaveSelMaintAttribs( zVIEW vSubtask )
{
   zVIEW    vObjectObj;

   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   ofnTZADCSDO_SaveAttribSelection( vSubtask, vObjectObj, "M_LOD_Attribute" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeleteMaintAttrib
// PURPOSE:  This function Deletes an Attribute from the style
//           object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeleteMaintAttrib( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zCHAR    szMsg[ 254 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vUserSpec, "M_LOD_Attribute", "" );

   // Ask the operator if it is ok to delete.
   if ( nRC > zCURSOR_UNCHANGED )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete selected Attributes?" );
      if ( MessagePrompt( vSubtask,                 "CM00111",
                          "Delete Confirmation",
                           szMsg,
                           zBEEP,         zBUTTONS_YESNO,
                           zRESPONSE_YES,  0 ) == zRESPONSE_NO )
         return( 0 );
   }

   while ( nRC > zCURSOR_UNCHANGED )
   {
      nZRetCode = ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vUserSpec, "M_LOD_Attribute", "" );
   }
   SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", "" );

   /* Gray the Delete option if there are no maint attribs. */
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" ) != 0 )
   {
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   RefreshCtrl( vSubtask, "MAINTATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeleteListAttrib
// PURPOSE:  This function Deletes an Attribute from the style
//           object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeleteListAttrib( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zCHAR    szMsg[ 254 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vUserSpec, "L_LOD_Attribute", "" );

   if ( nRC > zCURSOR_UNCHANGED )
   {
      // Ask the operator if it is ok to delete.
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete selected Attributes?" );
      if ( MessagePrompt( vSubtask,                 "CM00111",
                          "Delete Confirmation",
                           szMsg,
                           zBEEP,         zBUTTONS_YESNO,
                           zRESPONSE_YES,  0 ) == zRESPONSE_NO )
         return( 0 );
   }

   while ( nRC > zCURSOR_UNCHANGED )
   {
      nZRetCode = ExcludeEntity( vUserSpec, "L_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vUserSpec, "L_LOD_Attribute", "" );
   }
   SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", "" );

   RefreshCtrl( vSubtask, "LISTATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DeleteTabAttrib
// PURPOSE:  This function Deletes an Attribute from the style
//           object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeleteTabAttrib( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zCHAR    szName[ 33 ];
   zCHAR    szMsg[ 254 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vUserSpec, "MT_LOD_Attribute", "" );

   while ( nRC > zCURSOR_UNCHANGED )
   {
      // Ask the operator if it is ok to delete.
      GetStringFromAttribute( szName, zsizeof( szName ), vUserSpec, "MT_ER_Attribute", "Name" );
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete Attribute '" );
      strcat_s( szMsg, zsizeof( szMsg ), szName );
      strcat_s( szMsg, zsizeof( szMsg ), "' ?" );

      if ( MessagePrompt( vSubtask,                 "CM00111",
                          "Delete Confirmation",
                           szMsg,
                           zBEEP,         zBUTTONS_YESNO,
                           zRESPONSE_YES,  0 ) != zRESPONSE_NO )
      {
         nZRetCode = ExcludeEntity( vUserSpec, "MT_LOD_Attribute", zREPOS_NONE );
      }
      nRC = SetCursorNextSelectedEntity( vUserSpec, "MT_LOD_Attribute", "" );
   }
   SetCursorNextEntity( vUserSpec, "MT_LOD_Attribute", "" );

   /* Gray the Delete option if there are no maint attribs. */
   if ( CheckExistenceOfEntity( vUserSpec, "MT_LOD_Attribute" ) != 0 )
   {
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }
   else
   {
   // SetSelectStateOfEntityForSet( vUserSpec, "MT_LOD_Attribute", 1, 1 );
   }

   RefreshCtrl( vSubtask, "MAINTATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddMaintToList
// PURPOSE:  This function copies the currently positioned on
//           MAINTATTRIB to a LISTATTRIB under the same ENTITY.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AddMaintToList( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zSHORT nRC;
   zSHORT zRESULT;
   zLONG  lAttribKey;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   /* De-select any selected entities. */
   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" ) == 0 )
   {
      do
      {
         SetSelectStateOfEntityForSet( vUserSpec, "L_LOD_Attribute", 0, 1 );
      }
      while ( SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", "" ) == 0 );
   }

   zRESULT = SetCursorFirstSelectedEntity( vUserSpec, "M_LOD_Attribute", "" );
   while ( zRESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lAttribKey, vUserSpec,
                               "M_LOD_Attribute", "ZKey" );

      /* See if this entity already exists in L_LOD_Attribute.  If it
         does, then do not include it. */
      nRC = SetCursorFirstEntityByInteger( vUserSpec, "L_LOD_Attribute",
                                           "ZKey", lAttribKey, "" );

      if ( nRC != zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vUserSpec, "L_LOD_Attribute",
                                       vUserSpec, "M_LOD_Attribute",
                                       zPOS_AFTER );
      }

      zRESULT = SetCursorNextSelectedEntity( vUserSpec, "M_LOD_Attribute", "" );
   }

   /* Ungray the grayed options. */
   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" ) == 0 )
   {
      SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", "" );
      SetSelectStateOfEntityForSet( vUserSpec, "L_LOD_Attribute", 1, 1 );
      SetCtrlState( vSubtask, "Delete Listbox",
                    zCONTROL_STATUS_ENABLED, TRUE );
   }

   RefreshCtrl( vSubtask, "LISTATTRIB" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveStyle
// PURPOSE:  Calls fnSaveStyle to save the current style dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SaveStyle( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zVIEW  vUI_Work;
   zSHORT nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUI_Work, "TZADCWKO", vSubtask, zLEVEL_TASK );

   ResetView( vUserSpec );

   nRC = CommitMetaOI( vSubtask, vUserSpec, zSOURCE_UIS_META );
   if ( nRC < 0 )
   {
      if ( nRC = 2 )
      {
         MessageSend( vSubtask, "AD10001", "Autodesigner",
                      "Filename in use by another user",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         return( 1 );
      }
   }
   SetAttributeFromString( vUI_Work, "ROOT", "NEW", "N" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: InitializeObjectDialogue
// PURPOSE:  Initialize the Object dialogue upon Entry.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_InitStyleDialogue( zVIEW vSubtask )
{
   zVIEW    vSDWorkObj;
   zVIEW    vWDWorkObj;
   zVIEW    vStructObj;
   zVIEW    vFileObj;
   zVIEW    vFileObj2;
   zVIEW    vLPL;
   zCHAR    szLPL_Name[ 33 ];
   zSHORT   nRC;

   nRC = GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_ANY );

   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "FlatList", vSubtask, zLEVEL_TASK );

   ActivateEmptyObjectInstance( &vSDWorkObj, "TZADCWKO", vSubtask, zSINGLE );
   SetNameForView( vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );
   CreateEntity( vSDWorkObj, "ROOT", zPOS_AFTER );
   SetAttributeFromInteger( vSDWorkObj, "ROOT", "InitializeOpen", 0 );

   // Set LPL Name - Title - Spec Name on Window Title
   nRC = GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      GetStringFromAttribute( szLPL_Name, zsizeof( szLPL_Name ), vLPL, "LPLR", "Name" );
   else
      strcpy( szLPL_Name, "" );
    SetWindowCaptionTitle( vSubtask, szLPL_Name,
                           "User Interface Specification - [Untitled]" );

   /* Save the name of this window view. */
   SetNameForView( vSubtask, "TZADCDMW", vSubtask, zLEVEL_TASK );
   RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_LOD_META );
   SetNameForView( vFileObj, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vFileObj, "W_MetaDef", "Name A" );
   RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_VOR_META );
   OrderEntityForView( vFileObj, "W_MetaDef", "Name A" );
   SetNameForView( vFileObj, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vFileObj2, vFileObj, 0 );
   SetNameForView( vFileObj2, "AD_VOR_List2", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: InitGrayOptions
// PURPOSE:  Grays the proper menu options upon entering the
//           user spec dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_InitGrayOptions( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zSHORT nRC;

   nRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      SetOptionState( vSubtask, "Detail",
                      zOPTION_STATUS_ENABLED, 0 );
      SetOptionState( vSubtask, "Save",
                      zOPTION_STATUS_ENABLED, 0 );
      SetOptionState( vSubtask, "Refresh",
                      zOPTION_STATUS_ENABLED, 0 );
   }
   // Transfer to the open dialog window
   SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow |
                            zWAB_ProcessImmediateAction,
                            "TZADCDOD", "OpenDialogSpec" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: WndDataGraying
// PURPOSE:  Grays the proper menu options upon entering the
//           WndDataContent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_WndDataGraying( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zVIEW  vTmpView;
   zSHORT nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   /* The root entity cannot be eliminated. */
   CreateViewFromViewForTask( &vTmpView, vUserSpec, 0 );
   nRC = ResetViewFromSubobject( vTmpView );
   if ( nRC == 1 )
   {
      SetCtrlState( vSubtask, "EliminateWnd",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }
   DropView( vTmpView );

   /* If there are no M_LOD_Attribute then gray these options. */
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" ) != 0 )
   {
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }
   /* If there are no L_LOD_Attribute then gray these options. */
   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" ) != 0 )
   {
      SetCtrlState( vSubtask, "Delete Listbox",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }
   /* Save the name of this window view. */
   SetNameForView( vSubtask, "TZADCDDW", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: LoadFileList
// PURPOSE:  Load the file list in preparation for an Open
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_LoadFileList( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vFileObj;
   zSHORT   nRC;
   zSHORT   nResult;
   zCHAR    szTempString_0[ 33 ];

   nResult = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   /* If there is a user spec style loaded, check if it has been changed.
      If it has, then prompt the user to save it before bringing up the
      list of user specs. */
   if ( nResult != -1 && ObjectInstanceUpdatedFromFile( vUserSpec ) )
   {
      nRC = MessagePrompt( vSubtask, "AD10023", "Autodesigner",
                           "Do you wish to save the current User Spec?",
                           zBEEP, zRESPONSE_YES, 0, zICON_QUESTION );
      if ( nRC == zRESPONSE_YES )
      {
         zwTZADCDOD_SaveStyle( vSubtask );
      }
   // if ( CompareAttributeToString( vSDWorkObj, "ROOT", "NEW", "Y" ) == 0 )
   // {
   //    SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", "" );
   // }
   }

   RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_UIS_META );
   SetNameForView( vFileObj, "UI_SpecList", vSubtask, zLEVEL_TASK );

   if ( nResult != -1 &&
        CheckExistenceOfEntity( vUserSpec, "UI_Spec" ) == 0 )
   {
      if ( CompareAttributeToString( vUserSpec, "UI_Spec", "Name", "" ) != 0  && vFileObj >= 0 )
      {
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vUserSpec,
                                 "UI_Spec", "Name" );
         SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                       "Name", szTempString_0, "" );
      }
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: SaveSaveAs
// PURPOSE:      To create a new style object definition from the
//               current style object definition being worked on and
//               save it.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SaveSaveAs( zVIEW vSubtask )
{
   zVIEW    vSDWorkObj;
   zVIEW    vWDWorkObj;
   zVIEW    vUserSpec;
   zVIEW    vNewObj = 0;
   zVIEW    vFileSpec;
   zCHAR    szNewObjectName[ 256 ];
   zCHAR    szWorkString[ 256 ];
   zCHAR    szCaption[ 256 ];
   zSHORT   nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   /* CreateViewFrom( &vNewObj, "ZNEWOOD", vSubtask, zLEVEL_TASK ); */
   GetViewByName( &vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vFileSpec, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vFileSpec, zREFER_UIS_META );
      SetNameForView( vFileSpec, "UI_SpecList", vSubtask, zLEVEL_TASK );
   }

   GetStringFromAttribute( szNewObjectName, zsizeof( szNewObjectName ), vNewObj, "UI_Spec", "Name" );

   nRC = SetCursorFirstEntityByString( vFileSpec, "W_MetaDef",
                                       "Name", szNewObjectName, "" );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), szNewObjectName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " - Style object already exists, Overwrite?" );
      nRC = MessagePrompt( vSubtask, "AD10025", "Autodesigner",
                           szWorkString,
                           zBEEP,
                           zBUTTONS_YESNO, zRESPONSE_NO,
                           zICON_QUESTION );
      if ( nRC == zRESPONSE_NO )
         return( -1 );
   }

   ResetView( vNewObj );
   DeleteEntity( vNewObj, "UI_Spec", zREPOS_NONE );
   CreateMetaEntity( vSubtask, vNewObj, "UI_Spec", zPOS_AFTER );
   SetMatchingAttributesByName( vNewObj, "UI_Spec",
                                vUserSpec, "UI_Spec",
                                zSET_NULL | zSET_NOTNULL );
   SetAttributeFromString( vNewObj, "UI_Spec", "Name", szNewObjectName );

   // If the new Object cannot be saved, flip the data handle back
   // and re-initialize Save As so input mapping can still occur.
   nRC = CommitMetaOI( vSubtask, vNewObj, zSOURCE_UIS_META );
   if ( nRC < 0 )
   {
      DropObjectInstance( vNewObj );
   // ofnTZADCSDO_InitializeSaveAs( );
      return( -1 );
   }
   else
   {
      // Otherwise, check out the newly created style object, if the
      // Check Out does not work, issue a message and retain the
      // currently checked-Out Object.

      DropObjectInstance( vNewObj );
      nRC = SetCursorFirstEntityByString( vFileSpec, "W_MetaDef",
                                          "Name", szNewObjectName, "" );
      nRC = ActivateMetaOI( vSubtask, &vNewObj, vFileSpec,
                            zSOURCE_UIS_META, zSINGLE );
      if ( nRC == -1 )
      {
         strcpy_s( szWorkString, zsizeof( szWorkString ), "Check-out failed on newly created Object - " );
         strcat_s( szWorkString, zsizeof( szWorkString ), szNewObjectName );
         MessageSend( vSubtask, "AD10003", "Autodesigner",
                      szWorkString,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vUserSpec, zWAB_ReturnToParent, 0, 0 );
         DropObjectInstance( vNewObj );
         return( -1 );
      }

      if ( CompareAttributeToString ( vSDWorkObj, "ROOT", "NEW", "Y" ) == 0 &&
           CompareAttributeToString( vSDWorkObj, "ROOT", "STYLE_NAME",
                                     szNewObjectName ) != 0 )
      {
         SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", "" );
      }
   }

   SetAttributeFromString( vSDWorkObj, "ROOT", "OBJECT", szNewObjectName );
   SetAttributeFromString( vSDWorkObj, "ROOT", "NEW", "Y" );
   SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
   strcpy( szCaption, "User Interface Specification - " );
   strcat( szCaption, szNewObjectName );
   SetWindowCaptionTitle( vSubtask, 0, szCaption );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: CancelSaveAs
// PURPOSE:      Delete the SaveAs Object handle
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CancelSaveAs( zVIEW vSubtask )
{
   zVIEW    vNewObj;

   GetViewByName( &vNewObj, "ZNEWOOD", vSubtask, zLEVEL_TASK );
   DropObjectInstance( vNewObj );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: SetEliminateWndInd
// PURPOSE:  This function sets the WindowInd attribute on the W_Entity
//           entity in TZADWKLO if the user selects or de-selects the
//           Eliminate Window check box.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SetEliminateWndInd( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUIS_List;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   /* The user is eliminating this window. */
   if ( CompareAttributeToString( vUserSpec, "UIS_Entity",
                                  "EliminateWnd", "Y" ) == 0 )
   {
      SetAttributeFromString( vUIS_List, "W_Entity", "WindowInd", "N" );
   }
   else
   {
      /* This window was eliminated but now is not being eliminated. */
      if ( CheckExistenceOfEntity( vUserSpec, "WndDesign" ) == 0 )
         SetAttributeFromString( vUIS_List, "W_Entity", "WindowInd", "C" );
      else
         SetAttributeFromString( vUIS_List, "W_Entity", "WindowInd", "D" );

   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: SetCustomWndInd
// PURPOSE:  This function sets the WindowInd attribute on the W_Entity
//           entity in TZADWKLO to 'C' if the user creates a WndDesign
//           entity in TZADCSDO.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SetCustomWndInd( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUIS_List;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   /* If WindowInd is set to 'N' then this means no window will be created
      for this entity, so we don't care if WndDesign is added, WindowInd
      will still have 'N' but if it is not 'N' then it was 'D'(default
      window design) so change it to 'C'(custom window design). */
   if ( CompareAttributeToString( vUIS_List, "W_Entity",
                                  "WindowInd", "N" ) != 0 )
      SetAttributeFromString( vUIS_List, "W_Entity", "WindowInd", "C" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: DeleteCustomWndInd
// PURPOSE:  This function sets the WindowInd attribute on the W_Entity
//           entity in TZADWKLO to 'D' if the user deletes the WndDesign
//           entity in TZADCSDO.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeleteCustomWndInd( zVIEW vSubtask )
{
   zVIEW    vUIS_List;

   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   /* If WindowInd is set to 'N' then this means no window will be created
      for this entity, so we don't care if WndDesign is deleted, WindowInd
      will still have 'N' but if it is not 'N' then it was 'C'(custom
      window design) so change it to 'D'(default window design). */
   if ( CompareAttributeToString( vUIS_List, "W_Entity",
                                  "WindowInd", "N" ) != 0 )
   {
      SetAttributeFromString( vUIS_List, "W_Entity", "WindowInd", "D" );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: ResetToTop
// PURPOSE:  This function resets the dialog spec instance.  This is
//           because I display UIS_ViewObjRef and UIS_LOD on the
//           initial window and when I go to a detail window and come
//           back, I could be on a recursive sub entity so these fields
//           give errors.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_ResetToTop( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nRC;

   nRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
      ResetView( vUserSpec );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: fnSetWorkAttributes
// PURPOSE:  This function sets the work entity attributes for listing
//           characteristics of eash UIS_Entity subobject.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zVOID OPERATION
ofnTZADCSDO_fnSetWorkAttributes( zVIEW vUserSpecH,
                                 zVIEW vUIS_List )
{
   // Set up the ER_EntityType values: Assoc., Attrib., Fund.
   if ( CheckExistenceOfEntity( vUserSpecH, "ER_Entity" ) < zCURSOR_SET )
      SetAttributeFromString( vUIS_List, "W_Entity",
                              "ER_EntityType", "Attrib." );
   else
      if ( CompareAttributeToString( vUserSpecH, "ER_Entity",
                                       "Purpose", "F" ) == 0 )
         SetAttributeFromString( vUIS_List, "W_Entity",
                                 "ER_EntityType", "Fund." );
      else
         if ( CompareAttributeToString( vUserSpecH, "ER_Entity",
                                       "Purpose", "S" ) == 0 )
            SetAttributeFromString( vUIS_List, "W_Entity",
                                    "ER_EntityType", "Assoc." );
         else
            SetAttributeFromString( vUIS_List, "W_Entity",
                                    "ER_EntityType", "Attrib." );

   // Set up the LOD_EntityType values: Include, Maint., Display
   if ( CompareAttributeToString( vUserSpecH, "LOD_Entity",
                                    "Include", "Y" ) == 0 )
      SetAttributeFromString( vUIS_List, "W_Entity",
                              "LOD_EntityType", "Include" );
   else
      if ( CompareAttributeToString( vUserSpecH, "LOD_Entity",
                                    "Create", "Y" ) == 0 )
         SetAttributeFromString( vUIS_List, "W_Entity",
                                 "LOD_EntityType", "Maint." );
      else
         SetAttributeFromString( vUIS_List, "W_Entity",
                                 "LOD_EntityType", "Display" );

   // Set up Autodesign Case Type.
   SetAttributeFromAttribute( vUIS_List,  "W_Entity",   "AutodesignCaseType",
                              vUserSpecH, "UIS_Entity", "AutodesignCaseType" );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: AddEntitiesToStyle
// DESCRIPTION: Navigate the LOD and make sure all entities in the LOD
//              are in the User Spec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AddEntitiesToStyle( zVIEW vSubtask,
                               zVIEW vUserSpec,
                               zVIEW vLOD )
{
   zVIEW  vTempSpec;
   zSHORT nRC;

   SetCursorFirstEntity( vUserSpec, "UIS_Entity", 0 );
   for ( nRC = SetCursorFirstEntity( vLOD, "LOD_EntityParent", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "LOD_EntityParent", 0 ) )
   {
      // Add the entity if it doesn't exist in the User Spec.

      // We have a couple of difficulties in doing this.
      // 1. We can't use a set cursor because we would need to use a
      //    scoping entity, which doesn't work at the subobject level.
      // 2. The order of entities in the LOD could be different than
      //    the order in the UserSpec if the entity was deleted and then
      //    readded to the LOD.
      // Thus we will have to simulate a set cursor with scoping entity by
      // hand.
      CreateViewFromViewForTask( &vTempSpec, vUserSpec, 0 );
      for ( nRC = SetCursorFirstEntity( vTempSpec, "UIS_Entity", 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTempSpec, "UIS_Entity", 0 ) )
      {
        if ( CompareAttributeToAttribute( vLOD,      "LOD_EntityParent", "ZKey",
                                          vTempSpec, "LOD_Entity",       "ZKey" ) == 0 )
           break;
      }
      // If nRC = 0, we had a match, so just use that position.
      // Else, add the entity back at the old position.
      if ( nRC == 0 )
      {
         SetCursorFirstEntityByAttr( vUserSpec, "UIS_Entity", "ZKey",
                                     vTempSpec, "UIS_Entity", "ZKey", 0 );
      }
      else
      {
         CreateMetaEntity( vSubtask, vUserSpec, "UIS_Entity", zPOS_AFTER );
         CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );

         IncludeSubobjectFromSubobject( vUserSpec, "LOD_Entity",
                                        vLOD,      "LOD_EntityParent", zPOS_AFTER );
      }
      DropView( vTempSpec );

      // Go to process any children and subchildren.
      if ( CheckExistenceOfEntity( vLOD, "LOD_EntityChild" ) >= zCURSOR_SET )
      {
         SetViewToSubobject( vLOD, "LOD_EntityChild" );
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
         zwTZADCDOD_AddEntitiesToStyle( vSubtask, vUserSpec, vLOD );
         ResetViewFromSubobject( vLOD );
         ResetViewFromSubobject( vUserSpec );
      }

      SetCursorNextEntity( vUserSpec, "UIS_Entity", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: OpenStyleFile
// PURPOSE:  Open and Object file for update
// DESCRIPTION: This Operation takes the style object name from the file
//              list and attempts to open an Object with this name.
//              If for some reason the Object cannot be loaded,
//            ( Bad Attributes in File, File deleted since Object
//                list built ), the Dialogue is restarted. The
//              restart is necessary since this operation can be
//              used from the List View Window which requires an
//              Object definition be loaded. The restart
//              automatically restarts the dialogue at the graphic view.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_OpenStyleFile( zVIEW vSubtask )
{
   zVIEW    vUIS_List;
   zVIEW    vFileObj;
   zVIEW    vUserSpec;
   zVIEW    vUserSpecRoot;
   zVIEW    vUserSpecH;
   zVIEW    vWDWorkObj;
   zVIEW    vSDWorkObj;
   zVIEW    vStructObj;
   zVIEW    vObjectObj;
   zVIEW    vMainWnd;
   zVIEW    vMergeLOD;
   zVIEW    vMergeUserSpec;
   zLONG    lRC;
   zUSHORT  uLevel;
   zCHAR    szWorkString[ 256 ];
   zCHAR    szObjectName[ 33 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    szCaption[ 256 ];

   lRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( lRC != -1 )
      DropObjectInstance( vUserSpec );

   GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   lRC = GetViewByName( &vFileObj, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( lRC < zCURSOR_SET )
   {
      RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_UIS_META );
      SetNameForView( vFileObj, "UI_SpecList", vSubtask, zLEVEL_TASK );
   }

   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToInteger( vSDWorkObj, "ROOT",
                                   "InitializeOpen", 1 ) == 0 )
   {

      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vWDWorkObj, "Palette", "UIS_Name" );
      lRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef", "Name",
                                          szWorkString, 0 );
      lRC = ActivateMetaOI( vSubtask, &vUserSpec, vFileObj,
                            zSOURCE_UIS_META, zSINGLE );
      if ( lRC < 0 )
      {
         return( -1 );
      }
      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vFileObj, "W_MetaDef", "Name" );
      SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", szWorkString );
   }
   else
   {
      if ( CheckExistenceOfEntity( vFileObj, "W_MetaDef" ) != 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }

      ActivateMetaOI( vSubtask, &vUserSpec, vFileObj,
                      zSOURCE_UIS_META, zSINGLE );
      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vFileObj, "W_MetaDef", "Name" );
      SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", szWorkString );
   }

   // Also create the root level view.
   CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
   SetNameForView( vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vUserSpec, "UIS_LOD", "Name" );
   GetViewByName( &vFileObj, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   lRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                       "Name", szObjectName, "" );

   lRC = GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( lRC != -1 )
      DropMetaOI( vSubtask, vObjectObj );

   lRC = ActivateMetaOI( vSubtask, &vObjectObj, vFileObj, zREFER_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   SetNameForView( vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( lRC == -1 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "Object, " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " Not Found" );
      MessageSend( vSubtask, "AD10004", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      ResetView( vUserSpec );
      DropObjectInstance( vUserSpec );
      return( -1 );
   }

   // Set indicator in the top UIS_Entity to indicate it is a root.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "RootIndicator", "Y" );

   // Navigate through the corresponding LOD for the User Spec and add any
   // entities to the User Spec that are in the LOD but not in the User Spec.
   CreateViewFromViewForTask( &vMergeLOD, vObjectObj, 0 );
   CreateViewFromViewForTask( &vMergeUserSpec, vUserSpec, 0 );
   SetViewToSubobject( vMergeLOD, "LOD_EntityChild" );
   SetViewToSubobject( vMergeUserSpec, "UIS_ChildEntity" );
   zwTZADCDOD_AddEntitiesToStyle( vSubtask, vMergeUserSpec, vMergeLOD );
   DropView( vMergeLOD );
   DropView( vMergeUserSpec );

   /* Create the work object that lists the entities in the UI spec. */
   ActivateEmptyObjectInstance( &vUIS_List, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   CreateEntity( vUIS_List, "W_EntityList", zPOS_AFTER );
   CreateViewFromViewForTask( &vUserSpecH, vUserSpec, 0 );
   DefineHierarchicalCursor( vUserSpecH, "UI_Spec" );
   lRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vUserSpecH );
   while ( lRC > zCURSOR_UNCHANGED )
   {
      if ( lRC == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpecH, "UIS_ChildEntity" );
      }

      /* if ( zstrcmp ( szEntityName, "LOD_Entity" ) == 0 ) */
      if ( zstrcmp ( szEntityName, "UIS_Entity" ) == 0 ||
           zstrcmp( szEntityName, "UIS_ChildEntity" ) == 0 )
      {
         CreateEntity( vUIS_List, "W_Entity", zPOS_AFTER );
         SetAttributeFromAttribute( vUIS_List, "W_Entity", "Name",
                                    vUserSpecH, "LOD_Entity", "IndentName" );
         SetAttributeFromAttribute( vUIS_List, "W_Entity", "LOD_EntityZKey",
                                    vUserSpecH, "LOD_Entity", "ZKey" );

         // Set up the UIS_List work attribute values.
         ofnTZADCSDO_fnSetWorkAttributes( vUserSpecH, vUIS_List );

      }

      lRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vUserSpecH );
   }
   DropHierarchicalCursor( vUserSpecH );
   DropView( vUserSpecH );
 /*
   RESULT = SetCursorFirstEntity( vObjectObj, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      CreateEntity( vUIS_List, "W_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "Name",
                                 vObjectObj, "LOD_Entity", "IndentName" );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "LOD_EntityZKey",
                                 vObjectObj, "LOD_Entity", "ZKey" );
      RESULT = SetCursorNextEntity( vObjectObj, "LOD_Entity", "" );
   }
   */

   ResetView( vUIS_List );

   if ( CompareAttributeToInteger ( vSDWorkObj, "ROOT", "InitializeOpen", 1 ) == 0 )
   {
   /* BuildWindow( ) */
   }
   else
   {
   /* SetWindowActionBehavior( vUserSpec, zWAB_ReturnToParent, 0, 0 );
      RefreshCtrl( vSubtask, "EntityList" ); */
   }


   GetViewByName( &vMainWnd, "TZADCDMW", vSubtask, zLEVEL_TASK );

   /* Set the title and ungray the grayed options. */
   strcpy( szCaption, "User Interface Specification - " );
   strcat( szCaption, szWorkString );
   SetWindowCaptionTitle( vMainWnd, 0, szCaption );
   SetOptionState( vMainWnd, "&View", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vMainWnd, "&Save", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vMainWnd, "&Refresh", zOPTION_STATUS_ENABLED, 1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: DeleteStyleFile
// PURPOSE:  Delete a style object instance
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DeleteStyleFile( zVIEW vSubtask )
{
   zVIEW    vFileObj;
   zCHAR    szWorkString[ 50 ];
   zSHORT   nRC;
   zCHAR    szTempString_0[ 1024 ];

   GetViewByName( &vFileObj, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vFileObj, "W_MetaDef" ) != 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   strcpy_s( szWorkString, zsizeof( szWorkString ), "Delete Dialog Spec '" );
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vFileObj, "W_MetaDef", "Name" );
   strcat_s( szWorkString, zsizeof( szWorkString ), szTempString_0 );
   strcat_s( szWorkString, zsizeof( szWorkString ), "' ?" );
   nRC = MessagePrompt( vSubtask, "AD10026", "Autodesigner",
                        szWorkString, zBEEP,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
   if ( nRC == zRESPONSE_YES )
   {
      if ( DeleteMetaOI( vSubtask, vFileObj, zSOURCE_UIS_META ) == -1 )
         MessageSend( vSubtask, "AD10005", "Autodesigner",
                      "File in use by another user",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: CreateNewStyle
// PURPOSE:  Create a new style object definition
// DESCRIPTION: This Operation checks to see if the Object already
//              exists, and if so, gives the operator a chance to
//              load the existing definition.  If a new style style object
//              is to be created, an OBJECT Entity and an ENTITY
//              ENTITY with the Object name is created for a
//              starting point in updating a new style object.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CreateNewStyle( zVIEW vSubtask )
{
   zVIEW    vFileObj;
   zVIEW    vFileSpec;
   zVIEW    vSDWorkObj;
   zVIEW    vWDWorkObj;
   zVIEW    vUserSpec;
   zVIEW    vUserSpecRoot;
   zVIEW    vNewObj;
   zVIEW    vVOR_Obj;
   zVIEW    vMainWnd;
   zLONG    lRC;
   zCHAR    szObjectName[ 40 ];
   zCHAR    szStyleName[ 33 ];
   zCHAR    szWorkString[ 256 ];
   zCHAR    szCaption[ 256 ];

   GetViewByName( &vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vFileObj, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   lRC = GetViewByName( &vNewObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( lRC != -1 )
      DropObjectInstance( vNewObj );
   lRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( lRC != -1 )
      DropObjectInstance( vUserSpec );

   GetStringFromAttribute( szStyleName, zsizeof( szStyleName ), vSDWorkObj, "ROOT", "STYLE_NAME" );

   if ( CompareAttributeToString( vFileObj, "W_MetaDef", "Name", "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10006", "Autodesigner",
                   "No OBJECTs exist.  You must create one.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( CompareAttributeToString( vSDWorkObj, "ROOT", "STYLE_NAME", "" ) == 0 )
   {
      MessageSend( vSubtask, "AD10007", "Autodesigner",
                   "You must enter in a style name!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   /* Check to see if the new style object to be created already exists
      in the directory.  If so, warn the operator and give them a
      chance to load the existing definition for update. */
   strcpy_s( szWorkString, zsizeof( szWorkString ), szStyleName );

   lRC = GetViewByName( &vFileSpec, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( lRC == -1 )
   {
      RetrieveViewForMetaList( vSubtask, &vFileSpec, zREFER_UIS_META );
      SetNameForView( vFileSpec, "UI_SpecList", vSubtask, zLEVEL_TASK );
   }
   lRC = SetCursorFirstEntityByString( vFileSpec, "W_MetaDef",
                                       "Name", szStyleName, "" );
   if ( lRC > zCURSOR_UNCHANGED )
   {
      strcat_s( szWorkString, zsizeof( szWorkString ), " - Dialog Spec Exists, Open existing Dialog Spec?" );
      lRC = MessagePrompt( vSubtask, "AD10027", "Autodesigner",
                           szWorkString, zBEEP,
                           zBUTTONS_YESNO, zRESPONSE_YES,
                           zICON_EXCLAMATION );
      if ( lRC == zRESPONSE_YES )
      {
         zwTZADCDOD_OpenStyleFile( vSubtask );
         return( 0 );
      }
      else
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }
   else
   {
      ActivateEmptyMetaOI( vSubtask, &vUserSpec, zSOURCE_UIS_META, zSINGLE );
      SetNameForView( vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
      // Also create the root only view.
      CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
      SetNameForView( vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );

      SetAttributeFromString( vSDWorkObj, "ROOT", "NEW", "Y" );
   }

   // If the dialog spec is new or the operator does not wish to load
   // the existing definition, create a new dialog spec with the given
   // name and a root entity whose name matches the dialog spec name.
   // We are assuming here that there is a VOR by the same name as the LOD.
   // We will put in a check to make sure that is true.
   lRC = ActivateMetaOI( vSubtask, &vNewObj, vFileObj, zREFER_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   GetStringFromAttribute( szObjectName, zsizeof( szObjectName ), vNewObj, "LOD", "Name" );

   GetViewByName( &vFileObj, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   lRC = SetCursorFirstEntityByString( vFileObj, "W_MetaDef",
                                       "Name", szObjectName, "" );
   if ( lRC < 0 )
   {
      MessageSend( vSubtask, "AD10008", "Autodesigner",
                   "A Registered Named View does not exist for the selected LOD.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropObjectInstance( vNewObj );
      return( -1 );
   }

   lRC = ActivateMetaOI( vSubtask, &vVOR_Obj, vFileObj, zREFER_VOR_META,
                         zSINGLE | zLEVEL_APPLICATION );

   strcat_s( szObjectName, zsizeof( szObjectName ), ".LOD" );
   if ( lRC < 0 )
   {
      strcpy_s( szWorkString, zsizeof( szWorkString ), "The dialog spec object definition - " );
      strcat_s( szWorkString, zsizeof( szWorkString ), szObjectName );
      strcat_s( szWorkString, zsizeof( szWorkString ), " - does not exist!" );
      MessageSend( vSubtask, "AD10008", "Autodesigner",
                   szWorkString,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      DropObjectInstance( vNewObj );
      return( -1 );
   }
   SetNameForView( vNewObj, "LOD_Object", vSubtask, zLEVEL_TASK );

   SetAttributeFromString( vSDWorkObj, "ROOT", "OBJECT", szObjectName );
   /* If there is a UI_Spec, then this is a style instance that already
      exists so do not create a new one. */
   if ( CheckExistenceOfEntity ( vUserSpec, "UI_Spec" ) != 0 )
   {
      if ( CompareAttributeToString( vSDWorkObj, "ROOT", "UserSpecType", "R" ) == 0 )
         zwTZADCDOD_CreateReportStyle( vSubtask, vVOR_Obj, vUserSpec,
                                       szStyleName, vNewObj );
      else
         zwTZADCDOD_CreateDialogStyle( vSubtask, vVOR_Obj, vUserSpec,
                                       szStyleName, vNewObj );
   }
   SetAttributeFromAttribute( vUserSpec,  "UI_Spec", "Type",
                              vSDWorkObj, "ROOT",    "UserSpecType" );

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( CompareAttributeToString ( vSDWorkObj, "ROOT", "NEW", "Y" ) == 0 &&
        CompareAttributeToString( vSDWorkObj, "ROOT", "STYLE_NAME",
                                   szStyleName ) != 0 )
   {
      SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", "" );
   }

   SetCursorFirstEntity( vNewObj, "LOD_Entity", "" );

   /* Set the title and ungray the grayed options. */
   GetViewByName( &vMainWnd, "TZADCDMW", vSubtask, zLEVEL_TASK );
   strcpy( szCaption, "User Interface Specification - " );
   strcat( szCaption, szStyleName );
   SetWindowCaptionTitle( vMainWnd, 0, szCaption );
   SetOptionState( vMainWnd, "&View", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vMainWnd, "&Save", zOPTION_STATUS_ENABLED, 1 );
   SetOptionState( vMainWnd, "&Refresh", zOPTION_STATUS_ENABLED, 1 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: CheckForSave
// PURPOSE:  Check to see that an Updated style object does not exist
//           in memory prior to Opening a new style object, Creating a
//           new style object, or exiting the Object dialogue.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CheckForSave( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zSHORT nResult;
   zSHORT nRC;

   nResult = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   /* If there is a user spec style loaded, check if it has been changed.
      If it has, then prompt the user to save it before bringing up the
      list of user specs. */
   if ( nResult != -1 && ObjectInstanceUpdatedFromFile( vUserSpec ) )
   {
      nRC = MessagePrompt( vSubtask,"AD10027", "Autodesigner",
                           "Do you wish to save the current User Spec?",
                           zBEEP, zRESPONSE_YES, 0, zICON_QUESTION );
      if ( nRC == zRESPONSE_YES )
      {
         zwTZADCDOD_SaveStyle( vSubtask );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: ExitStyleDialogue
// PURPOSE:  Leave Style Dialog.  Validate that a style object does
//           not need to be saved and delete all handles used by
//           the dialogue.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_ExitStyleDialogue( zVIEW vSubtask )
{
   zVIEW    vSDWorkObj;
   zVIEW    vWDWorkObj;
   zVIEW    vObjectObj;
   zVIEW    vUserSpec;
   zVIEW    vStructObj;
   zVIEW    vUIS_List;
   zVIEW    vTemp;
   zSHORT   nRC;

   GetViewByName( &vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( nRC != -1 && ObjectInstanceUpdatedFromFile( vUserSpec ) )
   {
      nRC = MessagePrompt( vSubtask,"AD10028", "Autodesigner",
                           "Do you wish to save the current User Spec?",
                           zBEEP, zBUTTONS_YESNOCANCEL, 0, zICON_QUESTION );
      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            zwTZADCDOD_SaveStyle( vSubtask );
         }

         if ( CompareAttributeToString( vSDWorkObj, "ROOT", "NEW", "Y" ) == 0 )
         {
            SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", "" );
         }

         DropMetaOI( vSubtask, vUserSpec );
         if ( vObjectObj > 0 )
            DropMetaOI( vSubtask, vObjectObj );

         if ( vUIS_List > 0 )
            DropView( vUIS_List );
      }
   }
   else
   {
      if ( nRC != -1 )
      {
         DropMetaOI( vSubtask, vUserSpec );
         if ( vObjectObj > 0 )
            DropMetaOI( vSubtask, vObjectObj );

         if ( vUIS_List > 0 )
            DropView( vUIS_List );
      }
   }

   DropView( vSDWorkObj );
   DropView( vStructObj );
   nRC = GetViewByName( &vTemp, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_VOR_List2", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: ExitNoSave
// PURPOSE:  This is called when the ESC key is hit.  The dialog
//           spec is not saved.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_ExitNoSave( zVIEW vSubtask )
{
   // This code should do the same thing as ExitStyleDialog except
   // that there should be no confirmation to save.
#if 1
   zVIEW    vSDWorkObj;
   zVIEW    vWDWorkObj;
   zVIEW    vObjectObj;
   zVIEW    vUserSpec;
   zVIEW    vStructObj;
   zVIEW    vUIS_List;
   zVIEW    vTemp;
   zSHORT   nRC;

   GetViewByName( &vSDWorkObj, "TZADCWKO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vWDWorkObj, "TZPNTROO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   GetViewByName( &vStructObj, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( nRC != -1 && ObjectInstanceUpdatedFromFile( vUserSpec ) )
   {
      if ( CompareAttributeToString( vSDWorkObj, "ROOT", "NEW", "Y" ) == 0 )
      {
         SetAttributeFromString( vWDWorkObj, "Palette", "UIS_Name", "" );
      }

      DropMetaOI( vSubtask, vUserSpec );
      if ( vObjectObj > 0 )
         DropMetaOI( vSubtask, vObjectObj );

      if ( vUIS_List > 0 )
         DropView( vUIS_List );
   }
   else
   {
      if ( nRC != -1 )
      {
         DropMetaOI( vSubtask, vUserSpec );
         if ( vObjectObj > 0 )
            DropMetaOI( vSubtask, vObjectObj );

         if ( vUIS_List > 0 )
            DropView( vUIS_List );
      }
   }

   DropView( vSDWorkObj );
   DropView( vStructObj );
   nRC = GetViewByName( &vTemp, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_VOR_List2", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   return( 0 );

#else

   zVIEW    vTemp;
   zSHORT   nRC;

   nRC = GetViewByName( &vTemp, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropMetaOI( vSubtask, vTemp );
   }

   nRC = GetViewByName( &vTemp, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropMetaOI( vSubtask, vTemp );
   }

   nRC = GetViewByName( &vTemp, "UIS_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "TZADCWKO", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "FlatList", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "TZPNTROO", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "UI_SpecList", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   nRC = GetViewByName( &vTemp, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   if ( nRC >= zCURSOR_SET )
   {
      DropView( vTemp );
   }

   return( 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: LoadObjectFileList
// PURPOSE:  Checks whether the Object File List for Mapping has been
//           loaded, if it hasn't then the Object File List is loaded.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_LoadObjectFileList( zVIEW vSubtask )
{
   zVIEW vFileObj;
   zSHORT nRC;

   nRC = GetViewByName( &vFileObj, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   if ( nRC == -1 )
   {
      // Build the list for the object files.
      nRC = RetrieveViewForMetaList( vSubtask, &vFileObj, zREFER_VOR_META );
      nRC = SetNameForView( vFileObj, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:   GetDefaultWindow
// PURPOSE: This function copies the DfltWndDesign window into
//          the current WndDesign window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_GetDefaultWindow( zVIEW     vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUserSpec2;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_ANY );
   CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );
   ResetView( vUserSpec2 );
   if ( CheckExistenceOfEntity( vUserSpec2, "DfltWndDesign" ) == 0 )
   {
      SetMatchingAttributesByName( vUserSpec, "WndDesign",
                                   vUserSpec2, "DfltWndDesign", zSET_NOTNULL );
      if ( CheckExistenceOfEntity( vUserSpec, "WndStyle" ) == 0 )
         ExcludeEntity( vUserSpec, "WndStyle", zPOS_AFTER );

      if ( CheckExistenceOfEntity( vUserSpec2, "DfltWndStyle" ) == 0 )
         IncludeSubobjectFromSubobject( vUserSpec, "WndStyle",
                                        vUserSpec2, "DfltWndStyle", zPOS_AFTER );
   }
   else
   {
      MessageSend( vSubtask, "AD10009", "Autodesigner",
                   "There is no default window for this style object",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
   }

   DropView( vUserSpec2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    UpdateAttributes
// PURPOSE:  This function positions on the correct WndDesign entity
//           in the style object for the current ENTITY.  If the
//           WndDesign entity does not exist, then one is created.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_UpdateAttributes( zVIEW    vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nRC;

   ofnTZADCSDO_SelectEntityForUpdt( vSubtask );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Set flag for LISTSEL dialog to indicate path is from regular list path.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "ListTypeFlag", "L" );

   nRC = SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", "" );
   if ( nRC == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vUserSpec, "L_LOD_Attribute", 1, 1 );

   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
   if ( nRC == zCURSOR_SET )
      SetSelectStateOfEntityForSet( vUserSpec, "M_LOD_Attribute", 1, 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddAttributes
// PURPOSE: This function adds the default MAINTATTRIBs and default
//          LISTATTRIBs to the current style object ENTITY.  Any
//          MAINTATTRIBs or LISTATTRIBs that exist for this ENTITY
//          are deleted before the default attributes are added.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AddAttributes( zVIEW    vSubtask )
{
   zVIEW    vUserSpec;
   zLONG    lRC;
   zSHORT   nZRetCode;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity ( vUserSpec, "M_LOD_Attribute" ) == 0 )
   {
      while ( CheckExistenceOfEntity ( vUserSpec, "M_LOD_Attribute" ) == 0 )
      {
         nZRetCode = ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NEXT );
      }
   }

   lRC = ofnTZWDLGSO_CreateMaintAttribs( vSubtask, vUserSpec );
   if ( lRC == -1 )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity ( vUserSpec, "L_LOD_Attribute" ) == 0 )
   {
      while ( CheckExistenceOfEntity ( vUserSpec, "L_LOD_Attribute" ) == 0 )
      {
         nZRetCode = ExcludeEntity( vUserSpec, "L_LOD_Attribute", zREPOS_NEXT );
      }
   }

   lRC = ofnTZWDLGSO_CreateListAttribs( vSubtask, vUserSpec );
   if ( lRC == -1 )
   {
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" ) == 0 )
   {
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", "" );
      SetSelectStateOfEntityForSet( vUserSpec, "M_LOD_Attribute", 1, 1 );
   }
   else
   {
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }
   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" ) == 0 )
   {
      SetCtrlState( vSubtask, "Delete Listbox",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", "" );
      SetSelectStateOfEntityForSet( vUserSpec, "L_LOD_Attribute", 1, 1 );
   }
   else
   {
      SetCtrlState( vSubtask, "Delete Listbox",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectSubObject
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectSubObject( zVIEW  vSubtask, zPCHAR szViewObjEntityName )
{
   zVIEW    vVOR_List;
   zVIEW    vVOR_Obj;
   zVIEW    vUserSpec;
// zVIEW    vTempUIS;
   zVIEW    vLOD_List;
   zVIEW    vLOD_Obj;
   zVIEW    vListLOD;
   zSHORT   nRC;
   zLONG    lZKey;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   // Use correct list view depending on which routine called this one.
   if ( zstrcmp( szViewObjEntityName, "ListViewObjRef" ) == 0 )
      GetViewByName( &vVOR_List, "AD_VOR_List", vSubtask, zLEVEL_TASK );
   else
      GetViewByName( &vVOR_List, "AD_VOR_List2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_List, "AD_LOD_List", vSubtask, zLEVEL_TASK );
   GetViewByName( &vLOD_Obj, "LOD_Object", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity ( vVOR_List, "W_MetaDef" ) != 0 )
   {
      MessageSend( vSubtask, "AD10014", "Autodesigner",
                   "No OBJECTs exist.  You must create one !",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      return( 1 );
   }

   ActivateMetaOI( vSubtask, &vVOR_Obj, vVOR_List, zREFER_VOR_META,
                   zSINGLE | zLEVEL_APPLICATION );

   // Check to see that the includable LOD has an entity that matches the
   // entity in the user spec object.
   GetIntegerFromAttribute( &lZKey, vVOR_Obj, "LOD", "ZKey" );
   SetCursorFirstEntityByInteger( vLOD_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );

   // Load in the LOD for the list.
   ActivateMetaOI( vSubtask, &vListLOD, vLOD_List, zREFER_LOD_META,
                   zSINGLE | zLEVEL_APPLICATION );

   // Get the current LOD_Entity name in the user spec, search for that
   // entity name in the user spec LOD and get the ER_Entity ZKey for
   // the entity.
   GetIntegerFromAttribute( &lZKey, vUserSpec, "LOD_Entity", "ZKey" );
   SetCursorFirstEntityByInteger( vLOD_Obj, "LOD_Entity",
                                  "ZKey", lZKey, 0 );
   /*
   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUserSpec, "LOD_Entity", "Name" );
   SetCursorFirstEntityByString( vLOD_Obj, "LOD_Entity",
                                 "Name", szWorkString, "" );
   */
   /* If the vLOD_Obj has an ER_Entity then check if the ER_Entities
      from vListLOD and vLOD_Obj are the same which means that vListLOD
      can be included into vLOD_Obj.  If there is no ER_Entity than
      vLOD_Obj is a work LOD and vListLOD can only be included into
      vLOD_Obj if they are the same LOD. */
   if ( CheckExistenceOfEntity( vLOD_Obj, "ER_Entity" ) == 0 )
   {

      GetIntegerFromAttribute( &lZKey, vLOD_Obj, "ER_Entity", "ZKey" );
      if ( CheckExistenceOfEntity( vListLOD, "ER_Entity" ) == 0 )
         nRC = SetCursorFirstEntityByInteger( vListLOD, "ER_Entity",
                                              "ZKey", lZKey, "LOD" );
      else
      /* See if there is an ER_Entity in the list LOD that matches the
         ER_Entity in the user spec LOD.  If not, then the list LOD cannot
         be an object to be included from. */
      if ( CompareAttributeToAttribute( vLOD_Obj, "LOD", "ZKey",
                                        vListLOD, "LOD", "ZKey" ) == 0 )
         nRC = 0;
      else
         nRC = -1;
   }
   else
   if ( CompareAttributeToAttribute( vLOD_Obj, "LOD", "ZKey",
                                     vListLOD, "LOD", "ZKey" ) == 0 )
      nRC = 0;
   else
      /* If the ER_Entity doesn't exist, then this is a work object which
         is not in the ER model and thus can't have an entity with the
         same ER_Entity. */
      nRC = -1;

   if ( nRC <= zCURSOR_UNCHANGED )
   {
      MessageSend( vSubtask, "AD10015", "Autodesigner",
                   "This list LOD does not have an entity that matches the user spec entity!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   /* If the vLOD_Obj LOD_Entity does not have Include set to "Y" then
      this entity cannot be included so give the user an error. */
   if ( CompareAttributeToString( vLOD_Obj, "LOD_Entity", "Include",
                                  "Y" ) != 0 )
   {
      MessageSend( vSubtask, "AD10016", "Autodesigner",
                   "This entity is not set to be includeable!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   /* If vListLOD, LOD_Entity does not have the InclSrc attribute set
      to "Y" then this entity cannot be the source of an include, which
      is what the user is trying to do, so give an error. */
   if ( CompareAttributeToString( vListLOD, "LOD_Entity",
                                  "InclSrc", "Y" ) != 0 )
   {
      MessageSend( vSubtask, "AD10017", "Autodesigner",
                   "The included object's entity cannot be the source of an include!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   /* If this LOD_Entity has an ER_RelLink entity, then check to see
      if the CardMax is 1.  Meaning a 1 to 1 relationship.  If the
      relationship is 1 to 1 then when a new entity is included the old
      entity is excluded to make sure that LOD_Entity has it's Exclude
      attribute set to "Y".  If not warn the user. */
   if ( CheckExistenceOfEntity( vLOD_Obj, "ER_RelLink" ) == 0 )
   {
      if ( CompareAttributeToInteger( vLOD_Obj, "ER_RelLink",
                                      "CardMax", 1 ) == 0 &&
           CompareAttributeToString( vLOD_Obj, "LOD_Entity",
                                     "Exclude", "Y" ) != 0 )
      {
         MessageSend( vSubtask, "AD10018", "Autodesigner",
                      "This entity has a 1 to 1 relationship but cannot be excluded!",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }
   }

   /* If there was already a SubObjViewObjRef then exclude it because there
      can only be one for each entity. */
   if ( CheckExistenceOfEntity( vUserSpec, szViewObjEntityName ) == 0 )
      ExcludeEntity( vUserSpec, szViewObjEntityName, zREPOS_PREV );

   IncludeSubobjectFromSubobject( vUserSpec, szViewObjEntityName,
                                  vVOR_Obj, "ViewObjRef", zPOS_AFTER );

#if 0
   /* Create a UIS_ViewObjRef for any referenced VOR in this user spec. */
   GetIntegerFromAttribute( &lZKey, vUserSpec, szViewObjEntityName, "ZKey" );
   CreateViewFromViewForTask( &vTempUIS, vUserSpec, 0 );
   ResetView( vTempUIS );
   nRC = SetCursorFirstEntityByInteger( vTempUIS, "UIS_ViewObjRef",
                                        "ZKey", lZKey, "" );
   if ( nRC <= zCURSOR_UNCHANGED )
      IncludeSubobjectFromSubobject( vTempUIS, "UIS_ViewObjRef",
                                     vVOR_Obj, "ViewObjRef", zPOS_AFTER );
#endif
   /* If the entity to be included is the root, then gray the two include
      styles that are only applicable when the include entity is not the
      root. */
 /*  if ( SetCursorPrevEntity( vListLOD, "LOD_Entity", 0 )
                                                      <= zCURSOR_UNCHANGED )
   {
      SetCtrlState( vSubtask, "KeyEntryNotRoot",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Existing",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "KeyEntry",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "ListFull",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "ListPart",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "OperationText",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Operations",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetAttributeFromString( vUserSpec, "UIS_Include",
                              "SubObjectType", "K" );
   }
   else  */
   /* The include entity is not the root so gray the include styles that
      are only applicable when the include entity is the root. */
/*  {
      SetCtrlState( vSubtask, "KeyEntry",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ListFull",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "ListPart",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "OperationText",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Operations",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "KeyEntryNotRoot",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Existing",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetAttributeFromString( vUserSpec, "UIS_Include",
                              "SubObjectType", "O" );
   }   */

//   DropView( vTempUIS );
   DropView( vVOR_Obj );
   if ( vListLOD > 0 )
      DropView( vListLOD );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTITY:   ProcessCaseForEntity
// PURPOSE:  Process each UIS_Entity in the User Spec recursively to
//           set the Case Type and initialize the M_LOD_Attribute
//           subobject for Case1.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_ProcessCaseForEntity( zVIEW    vUserSpec,
                                 zVIEW    vLOD,
                                 zVIEW    vUIS_List )
{
   zVIEW   vUS_Parent;
   zVIEW   vLOD_Parent;
   zVIEW   vUserSpec2;
   zCHAR   szCardMax[9];
   zCHAR   szPurpose[ 2 ];
   zCHAR   szParentPurpose[ 2 ];
   zCHAR   szParentInclude[ 2 ];
   zCHAR   szInclude[ 2 ];
   zCHAR   szCreate[ 2 ];
   zCHAR   szUpdate[ 2 ];
   zCHAR   szDelete[ 2 ];
   zCHAR   szCase[ 3 ];
   zCHAR   szRootIndicator[ 2 ];
   zCHAR   szParentRootIndicator[ 2 ];
   zLONG   lLOD_ZKey;
   zSHORT  nRC;

   // First, process any recursive children.
   nRC = SetCursorFirstEntity( vUserSpec, "UIS_ChildEntity", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );

      zwTZADCDOD_ProcessCaseForEntity( vUserSpec,
                                       vLOD,
                                       vUIS_List );

      ResetViewFromSubobject( vUserSpec );
      nRC = SetCursorNextEntity( vUserSpec, "UIS_ChildEntity", 0 );
   }

   // Create a view to hold the parent UIS_Entity and then drop down one
   // recursive level.
   CreateViewFromViewForTask( &vUS_Parent, vUserSpec, 0 );
   ResetViewFromSubobject( vUS_Parent );

   // Position on the corresponding entities in the LOD.
   SetCursorFirstEntityByEntityCsr( vLOD,      "LOD_Entity",
                                    vUserSpec, "LOD_Entity", 0 );
   CreateViewFromViewForTask( &vLOD_Parent, vLOD, 0 );
   SetCursorFirstEntityByEntityCsr( vLOD_Parent, "LOD_Entity",
                                    vUS_Parent,  "LOD_Entity", 0 );

   // Get values needed for Case evaluation.
   GetStringFromAttribute( szRootIndicator, zsizeof( szRootIndicator ),
                           vUserSpec, "UIS_Entity", "RootIndicator" );
   GetStringFromAttribute( szParentRootIndicator, zsizeof( szParentRootIndicator ),
                           vUS_Parent, "UIS_Entity", "RootIndicator" );
   if ( CheckExistenceOfEntity( vLOD, "ER_RelLink" ) >= zCURSOR_SET )
      GetStringFromAttribute( szCardMax, zsizeof( szCardMax ), vLOD, "ER_RelLink", "CardMax" );
   if ( CheckExistenceOfEntity( vLOD, "ER_Entity" ) >= zCURSOR_SET )
      GetStringFromAttribute( szPurpose, zsizeof( szPurpose ), vLOD, "ER_Entity", "Purpose" );
   if ( CheckExistenceOfEntity( vLOD_Parent, "ER_Entity" ) >= zCURSOR_SET )
      GetStringFromAttribute( szParentPurpose, zsizeof( szParentPurpose ), vLOD_Parent, "ER_Entity", "Purpose" );

   GetStringFromAttribute( szParentInclude, zsizeof( szParentInclude ), vLOD_Parent, "LOD_Entity", "Include" );
   GetStringFromAttribute( szInclude, zsizeof( szInclude ), vLOD, "LOD_Entity", "Include" );
   GetStringFromAttribute( szCreate, zsizeof( szCreate ), vLOD, "LOD_Entity", "Create" );
   GetStringFromAttribute( szUpdate, zsizeof( szUpdate ), vLOD, "LOD_Entity", "Update" );
   GetStringFromAttribute( szDelete, zsizeof( szDelete ), vLOD, "LOD_Entity", "Delete" );

   // Next set the Case Type for this entity, according to the follow rules:
   //  1. If entity is root, Case 1A.
   //  2. If entity is Attrib and parent is the root, Case 1B.
   //  3. If entity is Attrib and parent is Attrib, Case 1C.
   //  4. If entity is Attrib and not includable and parent is Assoc, Case 1D.
   //  5. If entity is Attrib and parent is includable and CardMax is 1, Case none.
   //  6. If entity is Attrib and parent is includable and CardMax not 1, Case 1C.
   //  7. If entity is Assoc and creatable, Case 4.
   //  8. If entity is Fund and includable and
   //     parent is not Assoc and CardMax is 1, Case 2.
   //  9. If entity is Fund and includable and
   //     parent is not Assoc and CardMax not 1, Case 3.
   // 10. If entity is includable and parent is Assoc and parent is root, Case6.
   // 11. If entity is includable and parent is Assoc and this entity is the
   //     first entity under the parent, Case 4.
   // 12. If entity is includable and parent is Assoc and this entity is not
   //     the first entity under the parent, Case 5.
   // 13. If entity not includable and parent is Assoc, Case 1D.
   // 14. If none of the above, Case none.

   //  1. If entity is root, Case 1A.
   if ( szRootIndicator[ 0 ] == 'Y' )
      strcpy_s( szCase, zsizeof( szCase ), "1A" );
   else

   //  2. If entity is Attrib and parent is the root, Case 1B.
   if ( szPurpose[ 0 ] == 'A' && szParentRootIndicator[ 0 ] == 'Y' )
      strcpy_s( szCase, zsizeof( szCase ), "1B" );
   else

   //  3. If entity is Attrib and parent is Attrib, Case 1C.
   if ( szPurpose[ 0 ] == 'A' && szParentPurpose[ 0 ] == 'A' )
      strcpy_s( szCase, zsizeof( szCase ), "1C" );
   else

   //  4. If entity is Attrib and not includable and parent is Assoc, Case 1D.
   if ( szPurpose[ 0 ] == 'A' && szInclude[ 0 ] != 'Y' && szParentPurpose[ 0 ] == 'S' )
      strcpy_s( szCase, zsizeof( szCase ), "1D" );
   else

   //  5. If entity is Attrib and parent is includable and CardMax is 1, Case none.
   if ( szPurpose[ 0 ] == 'A' && szParentInclude[ 0 ] == 'Y' && szCardMax[ 0 ] == '1' )
      strcpy_s( szCase, zsizeof( szCase ), "N" );
   else

   //  6. If entity is Attrib and parent is includable and CardMax not 1, Case 1C.
   if ( szPurpose[ 0 ] == 'A' && szParentInclude[ 0 ] == 'Y' && szCardMax[ 0 ] != '1' )
      strcpy_s( szCase, zsizeof( szCase ), "1C" );
   else

   //  7. If entity is Assoc and creatable, Case 4.
   if ( szPurpose[ 0 ] == 'S' && szCreate[ 0 ] == 'Y' )
      strcpy_s( szCase, zsizeof( szCase ), "4" );
   else

   //  8. If entity is Fund and includable and
   //                        parent is not Assoc and CardMax is 1, Case 2.
   if ( szPurpose[ 0 ] == 'F' && szInclude[ 0 ] == 'Y' &&
                             szParentPurpose[ 0 ] != 'S' && szCardMax[ 0 ] == '1' )
      strcpy_s( szCase, zsizeof( szCase ), "2" );
   else

   //  9. If entity is Fund and includable and
   //                        parent is not Assoc and CardMax not 1, Case 3.
   if ( szPurpose[ 0 ] == 'F' && szInclude[ 0 ] == 'Y' &&
                             szParentPurpose[ 0 ] != 'S' && szCardMax[ 0 ] != '1' )
      strcpy_s( szCase, zsizeof( szCase ), "3" );
   else

   if ( szInclude[ 0 ] == 'Y' && szParentPurpose[ 0 ] == 'S' )
   {
      CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );
      ResetViewFromSubobject( vUserSpec2 );
      if ( CompareAttributeToString( vUserSpec2, "UIS_Entity", "RootIndicator",
                                     "Y" ) == 0 )
      {
   // 10. If entity is includable and parent is Assoc and parent is root, Case6.
         strcpy_s( szCase, zsizeof( szCase ), "6" );

         DropView( vUserSpec2 );
      }
      else
      {
         DropView( vUserSpec2 );
         CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );

         if ( SetCursorPrevEntity( vUserSpec2, "UIS_Entity", 0 ) < zCURSOR_SET )

   // 11. If entity is includable and parent is Assoc and this entity is the
   //     first entity under the parent, Case 4.
            strcpy_s( szCase, zsizeof( szCase ), "4" );
         else

   // 12. If entity is includable and parent is Assoc and this entity is not
   //     the first entity under the parent, Case 5.
            strcpy_s( szCase, zsizeof( szCase ), "5" );

         DropView( vUserSpec2 );
      }
   }
   else

   // 13. If entity not includable and parent is Assoc, Case 1D.
   if ( szInclude[ 0 ] == 'N' && szParentPurpose[ 0 ] == 'S' )
      strcpy_s( szCase, zsizeof( szCase ), "1D" );
   else

   // 14. If none of the above, Case none.
      strcpy_s( szCase, zsizeof( szCase ), "N" );

   // Set the value of the case just determined in both UserSpec and
   // UIS_List
   SetAttributeFromString( vUserSpec, "UIS_Entity", "AutodesignCaseType",
                           szCase );
   GetIntegerFromAttribute( &lLOD_ZKey, vLOD, "LOD_Entity", "ZKey" );
   SetCursorFirstEntityByInteger( vUIS_List, "W_Entity", "LOD_EntityZKey",
                                  lLOD_ZKey, 0 );
   SetAttributeFromString( vUIS_List, "W_Entity", "AutodesignCaseType",
                           szCase );

   // If the Case just set is 1, initialize the M_LOD_Attribute
   // entities in the UserSpec and the Create and Delete options, depending
   // on the characteristics of the LOD.
   // Also set the UsageStyle to Display is LOD_Entity is not create or update.
   if ( szCase[ 0 ] == '1' )
   {
      nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vUserSpec, "M_LOD_Attribute",
                                        vLOD,      "LOD_Attribute", zPOS_AFTER );
         nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", 0 );
      }
      if ( szCreate[ 0 ] == 'Y' )
         SetAttributeFromString( vUserSpec, "WndDesign", "ListCreateAction", "Y" );
      if ( szDelete[ 0 ] == 'Y' )
      {
         SetAttributeFromString( vUserSpec, "WndDesign", "ListDeleteAction", "Y" );
         SetAttributeFromString( vUserSpec, "WndDesign", "UpdateDeleteAction", "Y" );
      }
      if ( szCreate[ 0 ] == 'Y' || szUpdate[ 0 ] == 'Y' )
         SetAttributeFromString( vUserSpec, "WndDesign", "UsageStyle", "U" );
      else
         SetAttributeFromString( vUserSpec, "WndDesign", "UsageStyle", "D" );
   }

   DropView( vUS_Parent );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTITY: CreateDialogStyle
// PURPOSE:  Create a new style object definition from the object
//           definition selected by the user.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CreateDialogStyle( zVIEW    vSubtask,
                              zVIEW    vVOR_Obj,
                              zVIEW    vUserSpec,
                              zPCHAR   pchStyleName,
                              zVIEW    vObjectObj )
{
   zVIEW    vUIS_List;
   zLONG    OldLevel;
   zLONG    Level;
   zSHORT   nZRetCode;
   zSHORT   RESULT;

   ActivateEmptyObjectInstance( &vUIS_List, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   CreateEntity( vUIS_List, "W_EntityList", zPOS_AFTER );

   if ( CheckExistenceOfEntity( vUserSpec, "UI_Spec" ) < zCURSOR_SET )
      CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );
   SetAttributeFromString( vUserSpec, "UI_Spec", "Name", pchStyleName );
   nZRetCode = IncludeSubobjectFromSubobject( vUserSpec, "UIS_ViewObjRef",
                                              vVOR_Obj, "ViewObjRef",
                                              zPOS_AFTER );

   OldLevel = 0;
   RESULT = SetCursorFirstEntity( vObjectObj, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &Level, vObjectObj, "LOD_Entity", "IndentLvl" );
      if ( Level > OldLevel )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }
      else
      {
         while ( OldLevel != Level )
         {
            ResetViewFromSubobject( vUserSpec );
            OldLevel = OldLevel - 3;
         }
      }

      OldLevel = Level;
      CreateMetaEntity( vSubtask, vUserSpec, "UIS_Entity", zPOS_AFTER );
      CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );

      /* Create the workobject that displays the dialog spec entities. */
      CreateEntity( vUIS_List, "W_Entity", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vUserSpec, "LOD_Entity",
                                      vObjectObj, "LOD_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "Name",
                                 vUserSpec, "LOD_Entity", "IndentName" );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "LOD_EntityZKey",
                                 vUserSpec, "LOD_Entity", "ZKey" );

      // Set up the UIS_List work attribute values.
      ofnTZADCSDO_fnSetWorkAttributes( vUserSpec, vUIS_List );

      RESULT = SetCursorNextEntity( vObjectObj, "LOD_Entity", "" );
   }

   ResetView( vUIS_List );
   ResetView( vUserSpec );

   // The follow set cursor is necessary for User Spec refresh.
   SetCursorFirstEntity( vUserSpec, "UI_Spec", 0 );

   // Set indicator in the top UIS_Entity to indicate it is a root.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "RootIndicator", "Y" );

   // Now that the User Spec empty shell has been created, go back through
   // it recursively to set Case Types and initialize M_LOD_Attribute
   // subobject for Case 1 entities.
   zwTZADCDOD_ProcessCaseForEntity( vUserSpec,
                                    vObjectObj,
                                    vUIS_List );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTITY: CreateReportStyle
// PURPOSE:  Create a new style object definition from the object
//           definition selected by the user.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_CreateReportStyle( zVIEW    vSubtask,
                              zVIEW    vVOR_Obj,
                              zVIEW    vUserSpec,
                              zPCHAR   pchStyleName,
                              zVIEW    vLOD )
{
   zVIEW    vUIS_List;
   zLONG    OldLevel;
   zLONG    Level;
   zSHORT   nRC;
   zSHORT   RESULT;

   ActivateEmptyObjectInstance( &vUIS_List, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   CreateEntity( vUIS_List, "W_EntityList", zPOS_AFTER );

   if ( CheckExistenceOfEntity( vUserSpec, "UI_Spec" ) < zCURSOR_SET )
      CreateMetaEntity( vSubtask, vUserSpec, "UI_Spec", zPOS_AFTER );

   SetAttributeFromString( vUserSpec, "UI_Spec", "Name", pchStyleName );
   nRC = IncludeSubobjectFromSubobject( vUserSpec, "UIS_ViewObjRef",
                                              vVOR_Obj, "ViewObjRef",
                                              zPOS_AFTER );

   OldLevel = 0;
   RESULT = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &Level, vLOD, "LOD_Entity", "IndentLvl" );
      if ( Level > OldLevel )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }
      else
      {
         while ( OldLevel != Level )
         {
            ResetViewFromSubobject( vUserSpec );
            OldLevel = OldLevel - 3;
         }
      }

      OldLevel = Level;
      CreateMetaEntity( vSubtask, vUserSpec, "UIS_Entity", zPOS_AFTER );
      SetAttributeFromString( vUserSpec, "UIS_Entity", "AutodesignCaseType", "R" );

      // The following is just for compatibility with Dialog Spec.
      CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );

      // Initialize the M_LOD_Attribute entities in the UserSpec.
      nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vUserSpec, "M_LOD_Attribute",
                                        vLOD,      "LOD_Attribute", zPOS_AFTER );
         nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", 0 );
      }

      /* Create the workobject that displays the dialog spec entities. */
      CreateEntity( vUIS_List, "W_Entity", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vUserSpec, "LOD_Entity",
                                      vLOD, "LOD_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "Name",
                                 vUserSpec, "LOD_Entity", "IndentName" );
      SetAttributeFromAttribute( vUIS_List, "W_Entity", "LOD_EntityZKey",
                                 vUserSpec, "LOD_Entity", "ZKey" );

      // Set up the UIS_List work attribute values.
      ofnTZADCSDO_fnSetWorkAttributes( vUserSpec, vUIS_List );

      RESULT = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
   }

   ResetView( vUIS_List );
   ResetView( vUserSpec );

   // The follow set cursor is necessary for User Spec refresh.
   SetCursorFirstEntity( vUserSpec, "UI_Spec", 0 );

   // Set indicator in the top UIS_Entity to indicate it is a root.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "RootIndicator", "Y" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTITY: RefreshDialogStyle
// PURPOSE:  Create a new style object definition from the object
//           definition selected by the user.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_RefreshDialogStyle( zVIEW  vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUserSpecRoot;
   zVIEW    vObjectObj;
   zVIEW    vUIS_List;
   zVIEW    vVOR;
   zSHORT   nRC;
   zLONG    lZKey;
   zCHAR    szSpecName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec",  vSubtask, zLEVEL_TASK );
   GetViewByName( &vObjectObj, "LOD_Object",  vSubtask, zLEVEL_TASK );
   nRC = GetViewByName( &vUIS_List, "UIS_List",  vSubtask, zLEVEL_TASK );
   if ( nRC > -1 )
   {
      DropView( vUIS_List );
   }

   // Retrieve the VOR for the User Spec.
   SetCursorFirstEntity( vUserSpec, "UIS_ViewObjRef", 0 );
   GetIntegerFromAttribute( &lZKey, vUserSpec, "UIS_ViewObjRef", "ZKey" );
   ActivateMetaOI_ByZKey( vSubtask, &vVOR, 0, zREFER_VOR_META,
                          zSINGLE | zLEVEL_APPLICATION, lZKey, 0 );

   // The real building of the new Spec is done by CreateDialogSpec.  Here
   // we will delete the spec and call the routine.
   if ( CheckExistenceOfEntity( vUserSpec, "UIS_Entity" ) >= zCURSOR_SET )
      DeleteEntity( vUserSpec, "UIS_Entity", zREPOS_AFTER );
   if ( CheckExistenceOfEntity( vUserSpec, "ListVOR" ) >= zCURSOR_SET )
      ExcludeEntity( vUserSpec, "ListVOR", zREPOS_AFTER );
   nRC = SetCursorFirstEntity( vUserSpec, "UIS_ViewObjRef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vUserSpec, "UIS_ViewObjRef", zREPOS_NONE );
      nRC = SetCursorNextEntity( vUserSpec, "UIS_ViewObjRef", 0 );
   }

   GetStringFromAttribute( szSpecName, zsizeof( szSpecName ), vUserSpec, "UI_Spec", "Name" );
   if ( CompareAttributeToString( vUserSpec, "UI_Spec", "Type", "R" ) == 0 )
      zwTZADCDOD_CreateReportStyle( vSubtask,
                                    vVOR,
                                    vUserSpec,
                                    szSpecName,
                                    vObjectObj );
   else
      zwTZADCDOD_CreateDialogStyle( vSubtask,
                                    vVOR,
                                    vUserSpec,
                                    szSpecName,
                                    vObjectObj );
   DropMetaOI( vSubtask, vVOR );

   // Create new User Spec Root view.
   GetViewByName( &vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );
   DropView( vUserSpecRoot );
   CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
   SetNameForView( vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BuildEntityStructureList
// PURPOSE:  Build a 'flat' version of the Object for listing Entities
//           and their parents on the main Window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_BuildEntStructList( zVIEW vSubtask )
{
   zVIEW    vObjectObj;
   zVIEW    vStructObj;
   zVIEW    vUserSpec;
   zSHORT   RESULT;

   RESULT = GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   RESULT = GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Set the ListTypeFlag to indicate we are not coming from I_LOD... Add
   // request.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "ListTypeFlag", "L" );

   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "FlatList", vSubtask, zLEVEL_TASK );

   ResetView( vObjectObj );
   CreateEntity( vStructObj, "W_EntityList", zPOS_AFTER );

   RESULT = SetCursorFirstEntity( vObjectObj, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      CreateEntity( vStructObj, "W_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vStructObj, "W_Entity", "Name",
                                 vObjectObj, "LOD_Entity", "Name" );
      SetAttributeFromAttribute( vStructObj, "W_Entity", "LOD_EntityZKey",
                                 vObjectObj, "LOD_Entity", "ZKey" );
      RESULT = SetCursorNextEntity( vObjectObj, "LOD_Entity", "" );
   }
   SetCursorFirstEntity( vStructObj, "W_Entity", "" );
   ResetView( vObjectObj );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SaveAttributeSelection
// PURPOSE:  Save the ATTRIBs selected from ZSTROOD to the current
//           ENTITY in vUserSpec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZADCSDO_SaveAttribSelection( zVIEW  vSubtask,
                                 zVIEW  vObjectObj,
                                 zPCHAR szEntityAttrib )
{
   zVIEW  vFlatList;
   zVIEW  vUserSpec;
   zSHORT nRC;
   zLONG  lAttribKey;


   GetViewByName( &vFlatList, "FlatList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   /* Clear any selected entities. */
   if ( CheckExistenceOfEntity( vUserSpec, szEntityAttrib ) == 0 )
   {
      do
      {
         SetSelectStateOfEntityForSet( vUserSpec, szEntityAttrib, 0, 1 );
      }
      while ( SetCursorNextEntity( vUserSpec, szEntityAttrib, "" ) == 0 );
   }

   for ( nRC = SetCursorFirstEntity( vFlatList, "FlatAttribute", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vFlatList, "FlatAttribute", 0 ) )
   {
      SetCursorFirstEntityByAttr( vObjectObj, "LOD_Entity",    "Name",
                                  vFlatList,  "FlatAttribute", "EntityName", 0 );
      SetCursorFirstEntityByAttr( vObjectObj, "ER_Attribute",  "Name",
                                  vFlatList,  "FlatAttribute", "AttributeName",
                                  "LOD_Entity" );

      // Make sure the Attribute doesn't already exist in the UserSpec. If it's
      // not there, include it.
      GetIntegerFromAttribute( &lAttribKey,
                               vObjectObj, "LOD_Attribute", "ZKey" );

      nRC = SetCursorFirstEntityByInteger( vUserSpec, szEntityAttrib, "ZKey",
                                           lAttribKey, "" );
      if ( nRC != zCURSOR_SET )
         IncludeSubobjectFromSubobject( vUserSpec,  szEntityAttrib,
                                        vObjectObj, "LOD_Attribute",
                                        zPOS_AFTER );
   }

   if ( CheckExistenceOfEntity( vUserSpec, szEntityAttrib ) == 0 )
   {
      SetCursorFirstEntity( vUserSpec, szEntityAttrib, "" );
      SetSelectStateOfEntityForSet( vUserSpec, szEntityAttrib, 1, 1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    SelectEntityForUpdate
// PURPOSE:  Process a MOUSE double click on the style graph screen
// DESCRIPTION:  When going to the Entity update window, a copy of the
//               Entity selected for update is created after the
//               Entity to be updated and cursor 0 is positioned
//               on the new 'work' Entity so a cancel can be done
//               on the Entity update window. Cursor 1 is set to the
//               original Entity selected for update prior to creating
//               the work Entity for update.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZADCSDO_SelectEntityForUpdt( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vUIS_List;
   zSHORT   nZRetCode;
   zUSHORT  uLevel;
   zLONG    lZKey;
   zLONG    lZKey2;
   zCHAR    szEntityName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lZKey, vUIS_List, "W_Entity", "LOD_EntityZKey" );
   ResetView( vUserSpec );

   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   nZRetCode = SetCursorNextEntityHierarchical( &uLevel, szEntityName,
                                                vUserSpec );
   while ( nZRetCode > zCURSOR_UNCHANGED )
   {
      if ( nZRetCode == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }

      if ( zstrcmp ( "UIS_Entity", szEntityName ) == 0 ||
           zstrcmp( "UIS_ChildEntity", szEntityName ) == 0 )
      {
         GetIntegerFromAttribute( &lZKey2, vUserSpec, "LOD_Entity", "ZKey" );
         if ( lZKey == lZKey2 )
         {
            nZRetCode = zCURSOR_UNCHANGED;
         }
      }

      if ( nZRetCode != zCURSOR_UNCHANGED )
         nZRetCode = SetCursorNextEntityHierarchical( &uLevel, szEntityName,
                                                      vUserSpec );
   }

   DropHierarchicalCursor( vUserSpec );

// CreateNewVersionOfSubobject( vUserSpec, "UIS_Entity" )
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    EditSubobjInfo
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_EditSubobjInfo( zVIEW vSubtask )
{
   zVIEW    vUserSpec;

   ofnTZADCSDO_SelectEntityForUpdt( vSubtask );

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Set flag for LISTSEL dialog to indicate path is from subobject include.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "ListTypeFlag", "I" );

   if ( CheckExistenceOfEntity ( vUserSpec, "UIS_Include" ) != 0 )
      CreateMetaEntity( vSubtask, vUserSpec, "UIS_Include", zPOS_AFTER );

#if 0
   if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) != 0 )
   {
      CreateTemporalMetaEntity( vSubtask, vUserSpec, "UIS_Include", zPOS_AFTER );
   }
   else
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Include" );
   }
#endif

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZADCDOD_WS_GrayMenuTitleInfo
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_WndDesignGrayItems( zVIEW vSubtask )
{

   zVIEW vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vUserSpec, "WndDesign",
                                  "ActionType", "O" ) == 0 )
   {
      SetCtrlState( vSubtask, "MenuTitleWA",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "MenuTitleTextWA",
                    zCONTROL_STATUS_ENABLED, TRUE );

      SetCtrlState( vSubtask, "MenuTitleLA",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "MenuTitleTextLA",
                    zCONTROL_STATUS_ENABLED, TRUE );

      SetCtrlState( vSubtask, "MenuTitleSL",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "MenuTitleTextSL",
                    zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "MenuTitleWA",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MenuTitleTextWA",
                    zCONTROL_STATUS_ENABLED, FALSE );

      SetCtrlState( vSubtask, "MenuTitleLA",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MenuTitleTextLA",
                    zCONTROL_STATUS_ENABLED, FALSE );

      SetCtrlState( vSubtask, "MenuTitleSL",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "MenuTitleTextSL",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   // Gray the delete button is subobject include is set for this entity.
   // In other words, if UIS_Include entity exists.
   if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) >= zCURSOR_SET )
      SetCtrlState( vSubtask, "Delete",
                    zCONTROL_STATUS_ENABLED, FALSE );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    zwTZADCDOD_WS_GrayMenuTitleInfo
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_InclSubObjGrayItems( zVIEW vSubtask )
{

   SetCtrlState( vSubtask, "MenuTitleWA",
                 zCONTROL_STATUS_ENABLED, TRUE );
   SetCtrlState( vSubtask, "MenuTitleTextWA",
                 zCONTROL_STATUS_ENABLED, TRUE );

   SetCtrlState( vSubtask, "MenuTitleLA",
                 zCONTROL_STATUS_ENABLED, TRUE );
   SetCtrlState( vSubtask, "MenuTitleTextLA",
                 zCONTROL_STATUS_ENABLED, TRUE );

   SetCtrlState( vSubtask, "MenuTitleSL",
                 zCONTROL_STATUS_ENABLED, TRUE );
   SetCtrlState( vSubtask, "MenuTitleTextSL",
                 zCONTROL_STATUS_ENABLED, TRUE );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectInclSubObject( zVIEW vSubtask )
{
  // Execute the SelectSubObject operation while passing the correct
  // Entity Name for the Include View.
  zwTZADCDOD_SelectSubObject( vSubtask, "IncludeViewObjRef" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectListSubObject( zVIEW vSubtask )
{
  // Execute the SelectSubObject operation while passing the correct
  // Entity Name for the List View.
  zwTZADCDOD_SelectSubObject( vSubtask, "ListViewObjRef" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCaseX
// PURPOSE:  This function Accepts the UIS_Entity subobject structure
//           and makes sure any select subobject views are included under
//           UIS_Spec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCaseX( zVIEW vSubtask )
{
   zVIEW vUserSpec;
   zVIEW vUserSpecRoot;
   zCHAR szViewName[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );

   // Accept the subobject.
   AcceptSubobject( vUserSpec, "UIS_Entity" );

   // Make sure a UIS_ViewObjRef exists for ListViewObjRef and IncludeViewObjRef.
   if ( CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), vUserSpec, "ListViewObjRef", "Name" );
      if ( SetCursorFirstEntityByString( vUserSpecRoot, "UIS_ViewObjRef", "Name",
                                         szViewName, 0 ) < zCURSOR_SET )
         IncludeSubobjectFromSubobject( vUserSpecRoot, "UIS_ViewObjRef",
                                        vUserSpec,     "ListViewObjRef", zPOS_AFTER );
   }
   if ( CheckExistenceOfEntity( vUserSpec, "IncludeViewObjRef" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), vUserSpec, "IncludeViewObjRef", "Name" );
      if ( SetCursorFirstEntityByString( vUserSpecRoot, "UIS_ViewObjRef", "Name",
                                         szViewName, 0 ) < zCURSOR_SET )
         IncludeSubobjectFromSubobject( vUserSpecRoot, "UIS_ViewObjRef",
                                        vUserSpec,     "IncludeViewObjRef", zPOS_AFTER );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase1
// PURPOSE:  This function performs validations and Accepts UIS_Entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase1( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Make sure that at least one M_LOD_Attribute entity and one
   // L_LOD_Attribute entity exist.
#if 0
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10038", "Autodesigner",
                   "At least one Detail Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
#endif

   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10039", "Autodesigner",
                   "At least one Listbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Accept the subobject.
   AcceptSubobject( vUserSpec, "UIS_Entity" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase1Tab
// PURPOSE:  This function performs validations and Accepts UIS_Entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase1Tab( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Make sure that at least one UIS_Tab entity and one
   // L_LOD_Attribute entity exist.
   if ( CheckExistenceOfEntity( vUserSpec, "UIS_Tab" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10041", "Autodesigner",
                   "At least one Tab must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10039", "Autodesigner",
                   "At least one Listbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Accept the subobject.
   AcceptSubobject( vUserSpec, "UIS_Entity" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptTab
// PURPOSE:  This function performs validations and Accepts UIS_Entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptTab( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

#if 0
   // Make sure that at least one MT_LOD_Attribute entity exists.
   if ( CheckExistenceOfEntity( vUserSpec, "MT_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10040", "Autodesigner",
                   "At least one Tab Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
#endif

   // Accept the subobject.
   AcceptSubobject( vUserSpec, "UIS_Tab" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase2
// PURPOSE:  This function performs validations and calls AcceptCaseX.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase2( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Make sure that at least one M_LOD_Attribute entity and one
   // I_LOD_Attribute entity exist.
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10037", "Autodesigner",
                   "At least one Groupbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10036", "Autodesigner",
                   "At least one Select Listbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Call AcceptCaseX.
   zwTZADCDOD_AcceptCaseX( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase3
// PURPOSE:  This function makes sure there are no M_LOD_Attribute entities
//           and calls the AcceptCaseX function.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase3( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zSHORT nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Delete any M_LOD_Attribute entities that exist.
   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", 0 );
   }

   // Call AcceptCaseX.
   zwTZADCDOD_AcceptCaseX( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase4
// PURPOSE:  This function performs validations and calls AcceptCaseX.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase4( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zVIEW  vAssocUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Make sure that at least one L_LOD_Attribute entity and one
   // I_LOD_Attribute entity exist.
   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10035", "Autodesigner",
                   "At least one Listbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Attribute" )  < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10036", "Autodesigner",
                   "At least one Select Listbox Attribute must be selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Copy the WndDesign.WndCaption2 value from the include entity (that is
   // where the data is entered on the Case4Detail window) to the parent
   // associative entity (where it is used at autodesign time).
   CreateViewFromViewForTask( &vAssocUserSpec, vUserSpec, 0 );
   ResetViewFromSubobject( vAssocUserSpec );
   SetAttributeFromAttribute( vAssocUserSpec, "WndDesign", "WndCaption2",
                              vUserSpec,      "WndDesign", "WndCaption2" );
   DropView( vAssocUserSpec );

   // Call AcceptCaseX.
   zwTZADCDOD_AcceptCaseX( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AcceptCase5
// PURPOSE:  This function Accepts the UIS_Entity subobject structure for
//           Autodesign Case 5 and includes the I_LOD_Attribute from the
//           LOD_Attribute in LOD_Object.
//           The ListViewObjRef entity was already included from the action
//           tied to that Combo Box, but we must also include that entity
//           in UIS_ViewObjRef.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AcceptCase5( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zVIEW  vUserSpecRoot;
   zVIEW  vObject;
   zVIEW  vListObject;
   zCHAR  szViewName[ 33 ];
   zLONG  lZKey;
   zSHORT nRC;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUserSpecRoot, "UserSpecRoot", vSubtask, zLEVEL_TASK );
   GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );

   // Make sure the LOD_Attribute and ListViewObjRef entities have been
   // selected.  Else give an error.
   if ( CheckExistenceOfEntity( vObject,   "LOD_Attribute" )  < zCURSOR_SET ||
        CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "AD10033", "Autodesigner",
                   "Both Attribute and List View must be selected!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Accept the subobject.
   AcceptSubobject( vUserSpec, "UIS_Entity" );

   // Eliminate any L_LOD_Attribute entities that may have been created
   // when this entity was a different case type.
   nRC = SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vUserSpec, "L_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", 0 );
   }

   // Eliminate any extra M_LOD_Attribute entities or I_LOD_Attribute
   // entities (only one of each is allowed).
   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", 0 );
   if ( nRC >= zCURSOR_SET )
      nRC = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", 0 );
   }
   nRC = SetCursorFirstEntity( vUserSpec, "I_LOD_Attribute", 0 );
   if ( nRC >= zCURSOR_SET )
      nRC = SetCursorNextEntity( vUserSpec, "I_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vUserSpec, "I_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextEntity( vUserSpec, "I_LOD_Attribute", 0 );
   }
   SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", 0 );
   SetCursorFirstEntity( vUserSpec, "I_LOD_Attribute", 0 );

   // Include M_LOD_Attribute from the LOD_Object view.
   // This will be the LOD_Attribute for the Edit mapping on the Combo box.
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" ) >= zCURSOR_SET )
      ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NONE );
   IncludeSubobjectFromSubobject( vUserSpec, "M_LOD_Attribute",
                                  vObject,   "LOD_Attribute", zPOS_AFTER );

   // Include I_LOD_Attribute from the List Object view.
   // This will be the LOD_Attribute for the List mapping on the Combo box.
   // We will have to activate the LOD for the list, which is the LOD from
   // ListSubObjLOD.
   if ( CheckExistenceOfEntity( vUserSpec, "ListViewObjRef" ) >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vUserSpec, "ListSubObjLOD", "ZKey" );
      ActivateMetaOI_ByZKey( vSubtask, &vListObject, 0, zREFER_LOD_META,
                             zSINGLE | zLEVEL_APPLICATION, lZKey, 0 );

      // Position on corrent LOD_Attribute.
      GetIntegerFromAttribute( &lZKey, vUserSpec, "M_ER_Attribute", "ZKey" );
      SetCursorFirstEntityByInteger( vListObject, "ER_Attribute", "ZKey",
                                     lZKey, "LOD" );

      if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Attribute" ) >= zCURSOR_SET )
         ExcludeEntity( vUserSpec, "I_LOD_Attribute", zREPOS_NONE );
      IncludeSubobjectFromSubobject( vUserSpec,   "I_LOD_Attribute",
                                     vListObject, "LOD_Attribute", zPOS_AFTER );
      DropView( vListObject );

      // Make sure the ListViewObjRef is also included as UIS_ViewObjRef.
      GetStringFromAttribute( szViewName, zsizeof( szViewName ), vUserSpec, "ListViewObjRef", "Name" );
      if ( SetCursorFirstEntityByString( vUserSpecRoot, "UIS_ViewObjRef", "Name",
                                         szViewName, 0 ) < zCURSOR_SET )
         IncludeSubobjectFromSubobject( vUserSpecRoot, "UIS_ViewObjRef",
                                        vUserSpec,     "ListViewObjRef", zPOS_AFTER );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    Delete_I_LOD_Attrib
// PURPOSE:  This function Deletes an Attribute from the style
//           object.
//           Paint Attributes are deleted by double-clicking on
//           attributes to be deleted from the right-hand attribute
//           list in the SELOBJ2 window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_Delete_I_LOD_Attrib( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zSHORT   nZRetCode;
   zSHORT   nRC;
   zCHAR    szMsg[ 254 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vUserSpec, "I_LOD_Attribute", "" );

   if ( nRC > zCURSOR_UNCHANGED )
   {
      // Ask the operator if it is ok to delete.
      strcpy_s( szMsg, zsizeof( szMsg ), "OK to delete selected Attribute?" );
      if ( MessagePrompt( vSubtask,                 "CM00111",
                          "Configuration Management",
                           szMsg,
                           zBEEP,         zBUTTONS_YESNO,
                           zRESPONSE_YES,  zICON_EXCLAMATION ) == zRESPONSE_NO )
         return( 0 );
   }

   while ( nRC > zCURSOR_UNCHANGED )
   {
      nZRetCode = ExcludeEntity( vUserSpec, "I_LOD_Attribute", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vUserSpec, "I_LOD_Attribute", "" );
   }
   SetCursorNextEntity( vUserSpec, "I_LOD_Attribute", "" );

   if ( CheckExistenceOfEntity( vUserSpec, "I_LOD_Attribute" ) == 0 )
      SetSelectStateOfEntityForSet( vUserSpec, "I_LOD_Attribute", 1, 1 );

   RefreshCtrl( vSubtask, "AttributeList" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_GrayWndDataContent( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Gray buttons based on existence of M_LOD and L_LOD entities.
   if ( CheckExistenceOfEntity( vUserSpec, "M_LOD_Attribute" ) == 0 )
   {
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, TRUE );
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "Listbox From Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   if ( CheckExistenceOfEntity( vUserSpec, "L_LOD_Attribute" ) == 0 )
   {
      SetCtrlState( vSubtask, "Delete Listbox",
                    zCONTROL_STATUS_ENABLED, TRUE );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_GrayTabWndDataContent( zVIEW vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Gray buttons based on whether or not MT_LOD entity has been selected.
   if ( SetCursorFirstSelectedEntity( vUserSpec, "MT_LOD_Attribute", 0 ) >= zCURSOR_SET )
   {
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, TRUE );
   }
   else
   {
      SetCtrlState( vSubtask, "Delete Maint",
                    zCONTROL_STATUS_ENABLED, FALSE );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BuildEntityStructureList for Subobject Include
// PURPOSE:  Build a 'flat' version of the Object for listing Entities
//           and their parents on the main Window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_BuildInclStructList( zVIEW vSubtask )
{
   zVIEW    vObjectObj;
   zVIEW    vStructObj;
   zVIEW    vUserSpec;
   zSHORT   RESULT;
   zLONG    lZKey;

   // Verify that the List subobject has been selected.  This is the same as in
   // OK processing.
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity ( vUserSpec, "ListViewObjRef" ) != 0 )
   {
      MessageSend( vSubtask, "AD10013", "Autodesigner",
                   "You must select a List View!",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   // Set the ListTypeFlag to indicate we are coming from I_LOD... Add
   // request.
   SetAttributeFromString( vUserSpec, "UIS_Entity", "ListTypeFlag", "I" );

   // Build the Include object that will be used for selection.
   GetIntegerFromAttribute( &lZKey, vUserSpec, "ListSubObjLOD", "ZKey" );
   ActivateMetaOI_ByZKey( vSubtask, &vObjectObj, 0, zREFER_LOD_META,
                          zSINGLE, lZKey, 0 );
   SetNameForView( vObjectObj, "INCL_LOD_Object", vSubtask, zLEVEL_TASK );

   // Now proceed to build the flat object.
   ActivateEmptyObjectInstance( &vStructObj, "TZADWKLO", vSubtask, zSINGLE );
   SetNameForView( vStructObj, "FlatList", vSubtask, zLEVEL_TASK );

   CreateEntity( vStructObj, "W_EntityList", zPOS_AFTER );

   RESULT = SetCursorFirstEntity( vObjectObj, "LOD_Entity", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      CreateEntity( vStructObj, "W_Entity", zPOS_AFTER );
      SetAttributeFromAttribute( vStructObj, "W_Entity", "Name",
                                 vObjectObj, "LOD_Entity", "Name" );
      SetAttributeFromAttribute( vStructObj, "W_Entity", "LOD_EntityZKey",
                                 vObjectObj, "LOD_Entity", "ZKey" );
      RESULT = SetCursorNextEntity( vObjectObj, "LOD_Entity", "" );
   }

   SetCursorFirstEntity( vStructObj, "W_Entity", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ResetDefaults
// PURPOSE:  Reinitialize the default values in the current User Spec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_ResetDefaults( zVIEW vSubtask )
{
   zVIEW  vDefault;
   zSHORT nLanguageCode;

   GetViewByName( &vDefault, "User_Spec", vSubtask, zLEVEL_TASK );

   // Set standard button text values based on LanguageCode.
   SysGetLanguageCode( &nLanguageCode );
   if ( nLanguageCode == 49 )
   {
      // German Text values.
      SetAttributeFromString( vDefault, "InitValues", "OK",              "&Speichern" );
      SetAttributeFromString( vDefault, "InitValues", "Cancel",          "&Abbrechen" );
      SetAttributeFromString( vDefault, "InitValues", "Delete",          "&Löschen" );
      SetAttributeFromString( vDefault, "InitValues", "Next",            "&Vorwärts" );
      SetAttributeFromString( vDefault, "InitValues", "Previous",        "&Rückwärts" );
      SetAttributeFromString( vDefault, "InitValues", "Close",           "S&chließen" );
      SetAttributeFromString( vDefault, "InitValues", "New",             "&Neu..." );
      SetAttributeFromString( vDefault, "InitValues", "Exit",            "&Beenden" );
      SetAttributeFromString( vDefault, "InitValues", "DetailUpdate",    "&Aktualisieren..." );
      SetAttributeFromString( vDefault, "InitValues", "DetailDisplay",   "&Detail..." );
      SetAttributeFromString( vDefault, "InitValues", "SelectOnParent",  "&Neu..." );
      SetAttributeFromString( vDefault, "InitValues", "SelectOnInclude", "&OK" );
      SetAttributeFromString( vDefault, "InitValues", "CancelOnInclude", "&Abbrechen" );
      SetAttributeFromString( vDefault, "InitValues", "CloseOnInclude",  "&Zurück" );
      SetAttributeFromString( vDefault, "DfltWndDesign", "ListQualWndButtonText", "&Neu" );
      SetAttributeFromInteger( vDefault, "InitValues", "ButtonWidth", 55 );
   }
   else
   {
      // English Text values.
      SetAttributeFromString( vDefault, "InitValues", "OK",              "OK" );
      SetAttributeFromString( vDefault, "InitValues", "Cancel",          "Cancel" );
      SetAttributeFromString( vDefault, "InitValues", "Delete",          "&Delete" );
      SetAttributeFromString( vDefault, "InitValues", "Next",            "Ne&xt" );
      SetAttributeFromString( vDefault, "InitValues", "Previous",        "&Previous" );
      SetAttributeFromString( vDefault, "InitValues", "Close",           "&Close" );
      SetAttributeFromString( vDefault, "InitValues", "New",             "&New..." );
      SetAttributeFromString( vDefault, "InitValues", "Exit",            "&Exit" );
      SetAttributeFromString( vDefault, "InitValues", "DetailUpdate",    "&Update..." );
      SetAttributeFromString( vDefault, "InitValues", "DetailDisplay",   "&Detail..." );
      SetAttributeFromString( vDefault, "InitValues", "SelectOnParent",  "&New..." );
      SetAttributeFromString( vDefault, "InitValues", "SelectOnInclude", "&Select" );
      SetAttributeFromString( vDefault, "InitValues", "CancelOnInclude", "Cancel" );
      SetAttributeFromString( vDefault, "InitValues", "CloseOnInclude",  "&Close" );
      SetAttributeFromString( vDefault, "DfltWndDesign", "ListQualWndButtonText", "&New" );
      SetAttributeFromInteger( vDefault, "InitValues", "ButtonWidth", 40 );
   }

   // Set standard size and position information.
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonHeight", 12 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonBottomMargin", 3 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonTopMargin", 10 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonLeftMargin", 5 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonRightMargin", 5 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonIncrementY", 3 );
   SetAttributeFromInteger( vDefault, "InitValues", "ButtonIncrementX", 5 );
   SetAttributeFromInteger( vDefault, "InitValues", "EditboxHeight", 12 );
   SetAttributeFromInteger( vDefault, "InitValues", "EditboxVerticalSpacing", 3 );
   SetAttributeFromInteger( vDefault, "InitValues", "WindowWidth", 400 );
   SetAttributeFromString( vDefault, "InitValues", "DIL_MessageFlag", "Y" );

   // Set Defaults for Reports
   SetAttributeFromInteger( vDefault, "InitValues", "ReportTextSize", 10 );
   SetAttributeFromInteger( vDefault, "InitValues", "ReportTextIncrementX", 0 );
   SetAttributeFromInteger( vDefault, "InitValues", "ReportTextIncrementY", 0 );

   return( 0 );
}


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    TransferToDefault
// PURPOSE:  Activate the Default Design Specification as necessary.
//           We use a regular object file here instead of a meta object
//           so that the default spec won't get confused with regular
//           Design Specifications.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_TransferToDefault( zVIEW vSubtask )
{
   zVIEW  vDefault;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zLONG  lFile;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ),
                           vTaskLPLR, "LPLR", "MetaSrcDir" );
   strcat_s( szFileName, zsizeof( szFileName ), "\\Z__DFLT.BAS" );

   // First find out if the file exists.
   lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( lFile < 0 )
   {
      ActivateEmptyObjectInstance( &vDefault, "TZADCSDO", vSubtask, zSINGLE );
      CreateMetaEntity( vSubtask, vDefault, "UI_Spec", zPOS_AFTER );
      SetAttributeFromString( vDefault, "UI_Spec", "Name", "Z__DFLT" );
      CreateMetaEntity( vSubtask, vDefault, "DfltWndDesign", zPOS_AFTER );
   }
   else
   {
      SysCloseFile( vSubtask, lFile, 0 );
      ActivateOI_FromFile( &vDefault, "TZADCSDO", vSubtask,
                           szFileName, zSINGLE );
   }

   SetNameForView( vDefault, "User_Spec", vSubtask, zLEVEL_TASK );

   // Indicate that the type of User_Spec object is LPLR Default.
   SetAttributeFromString( vDefault, "UI_Spec", "DefaultType", "L" );

   // Initialize the values of InitValues if the entity doesn't already
   // exist.
   if ( CheckExistenceOfEntity( vDefault, "InitValues" ) < zCURSOR_SET )
   {
      CreateEntity( vDefault, "InitValues", zPOS_AFTER );
      zwTZADCDOD_ResetDefaults( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    DfltWndSpecOK
// PURPOSE:  Check to see if the User Spec is the default spec (the
//           name is Z__DFLT).  If so, save it to the file and return
//           to the top of the dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_DfltWndSpecOK( zVIEW vSubtask )
{
   zVIEW  vUserSpec;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW  vTaskLPLR;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vUserSpec, "UI_Spec", "Name", "Z__DFLT" ) == 0 )
   {
      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      GetStringFromAttribute( szFileName, zsizeof( szFileName ),
                              vTaskLPLR, "LPLR", "MetaSrcDir" );
      strcat_s( szFileName, zsizeof( szFileName ), "\\Z__DFLT.BAS" );
      CommitOI_ToFile( vUserSpec, szFileName, zSINGLE );
      //SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_SetDefaultWndSizes
// PURPOSE:    Set the default Control sizes for a single window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SetDefaultWndSizes( zVIEW    vSubtask )
{
   zVIEW  vUserSpec;

   // The name of the User Spec "User_Spec".
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );

   // Initialize the control sizes.
   SetAttributeFromInteger( vUserSpec, "UIS_Entity", "VerticalControlSize",
                            12 );
   SetAttributeFromInteger( vUserSpec, "UIS_Entity", "HorizontalControlSize",
                            5 );
   SetAttributeFromInteger( vUserSpec, "UIS_Entity", "VerticalSpacingIncrement",
                            3 );
   RefreshCtrl( vSubtask, "Height" );
   RefreshCtrl( vSubtask, "Width" );
   RefreshCtrl( vSubtask, "Space" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_PresentEntityPopup
// PURPOSE:    Present EntityPopup popup and name the current subtask for
//             use by the CaseSelect subwindow.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_PresentEntityPopup( zVIEW    vSubtask )
{
   zVIEW    vTemp;
   zVIEW    vUserSpec;
   zCHAR    szType[ 3 ];
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szType, zsizeof( szType ), vUserSpec, "UI_Spec", "Type" );

   if ( szType[ 0 ] == 'R' )
   {
      CreateTrackingPopupMenu( vSubtask, "EntityPopupR", pt.x, pt.y, FALSE, FALSE );
   }
   else
   {
      CreateTrackingPopupMenu( vSubtask, "EntityPopup", pt.x, pt.y, FALSE, FALSE );
   }

   // Name the current window view for refresh use by the CaseSelect
   // subwindow.
   if ( GetViewByName( &vTemp, "UIS_MainWindow", vSubtask, zLEVEL_TASK ) < 0 )
      SetNameForView( vSubtask, "UIS_MainWindow", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_GoToCaseDetail
// PURPOSE:    Go to maintenance window for Autodesign Case.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_GoToCaseDetail( zVIEW    vSubtask )
{
   zVIEW   vUIS_List;
   zVIEW   vUserSpec;
   zVIEW   vObject;
   zLONG   lZKey;
   zLONG   lZKey2;
   zSHORT  nRC;
   zUSHORT uLevel;
   zCHAR   szEntityName[ 33 ];
   zCHAR   szCaseType[ 3 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lZKey, vUIS_List, "W_Entity", "LOD_EntityZKey" );
   ResetView( vUserSpec );

   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vUserSpec );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }

      if ( zstrcmp ( "UIS_Entity", szEntityName ) == 0 ||
           zstrcmp( "UIS_ChildEntity", szEntityName ) == 0 )
      {
         GetIntegerFromAttribute( &lZKey2, vUserSpec, "LOD_Entity", "ZKey" );
         if ( lZKey == lZKey2 )
         {
            nRC = zCURSOR_UNCHANGED;
         }
      }

      if ( nRC != zCURSOR_UNCHANGED )
         nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName,
                                                vUserSpec );
   }

   DropHierarchicalCursor( vUserSpec );

   // Make sure a WndDesign entity exists.
   if ( CheckExistenceOfEntity( vUserSpec, "WndDesign" ) < zCURSOR_SET )
      CreateMetaEntity( vSubtask, vUserSpec, "WndDesign", zPOS_AFTER );

   GetStringFromAttribute( szCaseType, zsizeof( szCaseType ), vUserSpec,
                           "UIS_Entity", "AutodesignCaseType" );

   if ( szCaseType[ 0 ] == '1' &&
      ( szCaseType[ 1 ] != 'F' && szCaseType[ 1 ] != 'G' ))
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "Case1Detail" );
   }
   else
   if ( szCaseType[ 0 ] == '1' &&
      ( szCaseType[ 1 ] == 'F' || szCaseType[ 1 ] == 'G' ))
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "CaseTabGroup" );
   }
   else
   if ( szCaseType[ 0 ] == '2' )
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "Case2Detail" );
   }
   else
   if ( szCaseType[ 0 ] == '3' )
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "Case3Detail" );
   }
   else
   if ( szCaseType[ 0 ] == '4' )
   {
      // Make sure that the Case 4 selection is not for the Assoc Entity.
      if ( CompareAttributeToString( vUserSpec, "ER_Entity", "Purpose",
                                     "S" ) != 0 )
      {
         CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
         SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                                  "TZADCDOD", "Case4Detail" );
      }
      else
      {
         ResetView( vUserSpec );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      }
   }
   else
   if ( szCaseType[ 0 ] == '5' || szCaseType[ 0 ] == '6' )
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "Case5Detail" );
      // Make sure we are positioned on the correct entity in the
      // LOD_Object for selecting the attribute to display.
      GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
      GetViewByName( &vObject, "LOD_Object", vSubtask, zLEVEL_TASK );
      SetCursorFirstEntityByString( vObject, "LOD_Entity", "Name", szEntityName, 0 );
      // Make sure a UIS_Include entity exists
      if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) < zCURSOR_SET )
         CreateMetaEntity( vSubtask, vUserSpec, "UIS_Include", zPOS_AFTER );
   }
   else
   if ( szCaseType[ 0 ] == 'R' )
   {
      CreateTemporalSubobjectVersion( vUserSpec, "UIS_Entity" );
      SetWindowActionBehavior( vSubtask, zWAB_StartModalSubwindow,
                               "TZADCDOD", "CaseR_Detail" );
      // Initialize ReportOrientationFlag to V, if it is null.
      if ( CompareAttributeToString( vUserSpec, "UIS_Entity",
                                     "ReportOrientationFlag", "" ) == 0 )
         SetAttributeFromString( vUserSpec, "UIS_Entity",
                                 "ReportOrientationFlag", "V" );
   }
   else
   {
      ResetView( vUserSpec );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
   }

   return( 0 );

}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_SetupCaseSelect
// PURPOSE:   Check if there is an enqueue on the Case Select window as
//            explained below under PostBuildCaseSelect.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SetupCaseSelect( zVIEW    vSubtask )
{
   zVIEW  vCaseWindow;
   zSHORT nRC;

   // If the Case Select window is already up, don't go to it.
   nRC = GetViewByName( &vCaseWindow, "CaseDetail", vSubtask, zLEVEL_TASK );
   if ( nRC >= 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_PostBuildCaseSelect
// PURPOSE:   Enqueue the Case Select window so it won't be brought up
//            more than once.
//            We will do this by naming the vSubtask for the window and
//            checking for that name in the Parent Window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_PostBuildCaseSelect( zVIEW    vSubtask )
{
   SetNameForView( vSubtask, "CaseDetail", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_RefreshSelectCase
// PURPOSE:   Refresh the CaseSelect subwindow.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_RefreshCaseSelect( zVIEW    vSubtask )
{

   // Refresh the CaseSelect subwindow if it is up.
   RefreshAllSubwindows( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_SelectCase
// PURPOSE:    Set the selected case value in the UserSpec entity and
//             refresh the parent window.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SelectCase( zVIEW    vSubtask )
{
   zVIEW   vMainWindow;
   zVIEW   vUIS_List;
   zVIEW   vUserSpec;
   zVIEW   vUserSpec2;
   zLONG   lZKey;
   zLONG   lZKey2;
   zSHORT  nRC;
   zUSHORT uLevel;
   zCHAR   szEntityName[ 33 ];
   zCHAR   szOrigCaseType[ 3 ];
   zCHAR   szNewCaseType[ 3 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   GetIntegerFromAttribute( &lZKey, vUIS_List, "W_Entity", "LOD_EntityZKey" );
   ResetView( vUserSpec );

   DefineHierarchicalCursor( vUserSpec, "UI_Spec" );
   nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName, vUserSpec );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( nRC == zCURSOR_SET_RECURSIVECHILD )
      {
         SetViewToSubobject( vUserSpec, "UIS_ChildEntity" );
      }

      if ( zstrcmp ( "UIS_Entity", szEntityName ) == 0 ||
           zstrcmp( "UIS_ChildEntity", szEntityName ) == 0 )
      {
         GetIntegerFromAttribute( &lZKey2, vUserSpec, "LOD_Entity", "ZKey" );
         if ( lZKey == lZKey2 )
         {
            nRC = zCURSOR_UNCHANGED;
         }
      }

      if ( nRC != zCURSOR_UNCHANGED )
         nRC = SetCursorNextEntityHierarchical( &uLevel, szEntityName,
                                                vUserSpec );
   }

   DropHierarchicalCursor( vUserSpec );

   GetStringFromAttribute( szOrigCaseType, zsizeof( szOrigCaseType ),
                           vUserSpec, "UIS_Entity", "AutodesignCaseType" );
   GetStringFromAttribute( szNewCaseType, zsizeof( szNewCaseType ),
                           vUIS_List, "W_Entity",   "AutodesignCaseType" );
   SetAttributeFromAttribute( vUserSpec, "UIS_Entity", "AutodesignCaseType",
                              vUIS_List, "W_Entity",   "AutodesignCaseType" );

   // If the Case is being changed from non-Tab to Tab, move the maintenance
   // attributes (M_LOD_... attributes) under the UIS_Tab entity.
   if ( szOrigCaseType[ 0 ] == '1' &&
        szOrigCaseType[ 1 ] != 'E' &&
        szOrigCaseType[ 1 ] != 'F' &&
        szOrigCaseType[ 1 ] != 'G' )
   {
      if ( szNewCaseType[ 1 ] == 'E' ||
           szNewCaseType[ 1 ] == 'F' ||
           szNewCaseType[ 1 ] == 'G' )
      {
         CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );
         if ( CheckExistenceOfEntity( vUserSpec2, "UIS_Tab" ) < zCURSOR_SET )
            CreateMetaEntity( vSubtask, vUserSpec2, "UIS_Tab", zPOS_AFTER );

         SetAttributeFromAttribute( vUserSpec2, "UIS_Tab",   "Title",
                                    vUserSpec2, "WndDesign", "WndCaption" );

         // Remove any MT_LOD_Attribute entries before trying to add them back.
         for ( nRC = SetCursorFirstEntity( vUserSpec2, "MT_LOD_Attribute", 0 );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vUserSpec2, "MT_LOD_Attribute", 0 ) )
         {
            ExcludeEntity( vUserSpec2, "MT_LOD_Attribute", zREPOS_NONE );
         }

         nRC = SetCursorFirstEntity( vUserSpec2, "M_LOD_Attribute", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vUserSpec2, "MT_LOD_Attribute",
                                           vUserSpec2, "M_LOD_Attribute", zPOS_AFTER );
            ExcludeEntity( vUserSpec2, "M_LOD_Attribute", zREPOS_NONE );

            nRC = SetCursorNextEntity( vUserSpec2, "M_LOD_Attribute", 0 );
         }
         DropView( vUserSpec2 );
      }
   }

   // If the Case is being changed from Tab to non-Tab, move the Tab
   // attributes (MT_LOD_... attributes) to M_LOD_...
   if ( szOrigCaseType[ 0 ] == '1' ||
        szOrigCaseType[ 1 ] == 'E' ||
        szOrigCaseType[ 1 ] == 'F' ||
        szOrigCaseType[ 1 ] == 'G' )
   {
      if ( szNewCaseType[ 1 ] != 'E' &&
           szNewCaseType[ 1 ] != 'F' &&
           szNewCaseType[ 1 ] != 'G' )
      {
         CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );
         if ( CheckExistenceOfEntity( vUserSpec2, "UIS_Tab" ) >= zCURSOR_SET )
         {
            SetAttributeFromAttribute( vUserSpec2, "WndDesign", "WndCaption",
                                       vUserSpec2, "UIS_Tab",   "Title" );

            nRC = SetCursorFirstEntity( vUserSpec2, "MT_LOD_Attribute", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               IncludeSubobjectFromSubobject( vUserSpec2, "M_LOD_Attribute",
                                              vUserSpec2, "MT_LOD_Attribute", zPOS_AFTER );
               ExcludeEntity( vUserSpec2, "MT_LOD_Attribute", zREPOS_NONE );

               nRC = SetCursorNextEntity( vUserSpec2, "MT_LOD_Attribute", 0 );
            }
            DeleteEntity( vUserSpec2, "UIS_Tab", zPOS_AFTER );
         }
         DropView( vUserSpec2 );
      }
   }

   ResetView( vUserSpec );

   GetViewByName( &vMainWindow, "UIS_MainWindow", vSubtask, zLEVEL_TASK );
   RefreshCtrl( vMainWindow, "EntityList" );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_AddNewTabGroup
// PURPOSE:    This operation is used instead if subobject function
//             because we must create a meta entity with ZKey.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_AddNewTabGroup( zVIEW    vSubtask )
{
   zVIEW  vUserSpec;

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vUserSpec, "UIS_Tab", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZADCDOD_MoveToMT_Attributes
// PURPOSE:    This routine is used on return from selecting maintenance
//             attributes and is used to move (include/exclude) the
//             M_LOD_Attributes to the end of the MT_LOD_Attributes.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_MoveToMT_Attributes( zVIEW    vSubtask )
{
   zVIEW  vUserSpec;
   zVIEW  vUserSpec2;
   zVIEW  vObjectObj;
   zSHORT nRC;

   GetViewByName( &vObjectObj, "LOD_Object", vSubtask, zLEVEL_TASK );
   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vUserSpec2, vUserSpec, 0 );

   // Position at the end of all MT_LOD_Attributes.
   SetCursorLastEntity( vUserSpec, "MT_LOD_Attribute", 0 );

   // Loop for each M_LOD_Attribute.
   nRC = SetCursorFirstEntity( vUserSpec, "M_LOD_Attribute", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // Only do the include if the M_LOD_Attribute doesn't already exist
      // as an MT_LOD_Attribute.
      nRC = SetCursorFirstEntityByEntityCsr( vUserSpec2, "MT_LOD_Attribute",
                                             vUserSpec,  "M_LOD_Attribute", 0 );
      if ( nRC < zCURSOR_SET )
      {
         // Position on correct LOD_Entity in LOD.
         SetCursorFirstEntityByEntityCsr( vObjectObj, "LOD_Entity",
                                          vUserSpec,  "M_LOD_Entity", 0 );

         // Position on the LOD attribute for the include.  We cannot include
         // from the M_LOD_Attribute since that attribute is under a temporal
         // subobject.
         nRC = SetCursorFirstEntityByEntityCsr( vObjectObj, "LOD_Attribute",
                                                vUserSpec,  "M_LOD_Attribute", 0 );
         if ( nRC >= zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( vUserSpec,  "MT_LOD_Attribute",
                                           vObjectObj, "LOD_Attribute", zPOS_AFTER );
            ExcludeEntity( vUserSpec, "M_LOD_Attribute", zREPOS_NONE );
         }
      }

      nRC = SetCursorNextEntity( vUserSpec, "M_LOD_Attribute", 0 );
   }

   DropView( vUserSpec2 );

   // Process graying of the delete button.
   zwTZADCDOD_GrayTabWndDataContent( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Object Operation: IncludeListVOR
// PURPOSE:  This function Includes the VOR that will be used for listing
//           the root entity as the entity, ListVOR, in the User Spec.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_IncludeListVOR( zVIEW vSubtask )
{
   zVIEW    vUserSpec;
   zVIEW    vVOR;
   zVIEW    vListVOR;
   zVIEW    vListLOD;
   zVIEW    vMainLOD;
   zLONG    lZKey;
   zCHAR    szVOR_Name[ 33 ];

   GetViewByName( &vUserSpec, "User_Spec", vSubtask, zLEVEL_TASK );
   GetViewByName( &vListVOR, "AD_VOR_List", vSubtask, zLEVEL_TASK );

   if ( GetSelectStateOfEntity( vListVOR,  "W_MetaDef" ) == 0 )
   {
      // If no VOR was selected, exclude the ListVOR if it exists.
      if ( CheckExistenceOfEntity( vUserSpec, "ListVOR" ) >= zCURSOR_SET )
      {
         ExcludeEntity( vUserSpec, "ListVOR", zREPOS_AFTER );
      }
   }
   else
   {
      // Retrieve the VOR for the AD_VOR_List entry just selected.
      GetStringFromAttribute( szVOR_Name, zsizeof( szVOR_Name ), vListVOR, "W_MetaDef", "Name" );
      ActivateMetaOI_ByName( vSubtask, &vVOR, 0, zREFER_VOR_META,
                             zSINGLE | zLEVEL_APPLICATION, szVOR_Name, 0 );

      // Validate that the list view, ListVOR (about to be included), has the
      // same root ER_Entity as the main view, UIS_ViewObjRef.
      GetIntegerFromAttribute( &lZKey, vVOR, "LOD", "ZKey" );
      ActivateMetaOI_ByZKey( vSubtask, &vListLOD, 0, zREFER_LOD_META,
                             zSINGLE | zLEVEL_APPLICATION, lZKey, 0 );
      GetIntegerFromAttribute( &lZKey, vUserSpec, "UIS_LOD", "ZKey" );
      ActivateMetaOI_ByZKey( vSubtask, &vMainLOD, 0, zREFER_LOD_META,
                             zSINGLE | zLEVEL_APPLICATION, lZKey, 0 );

      if ( CheckExistenceOfEntity( vListLOD,  "ER_EntityRec" ) < zCURSOR_SET ||
           CompareAttributeToAttribute( vMainLOD, "ER_EntityRec", "Name",
                                        vListLOD, "ER_EntityRec", "Name" ) != 0 )
      {
         MessageSend( vSubtask, "AD10034", "Autodesigner",
                      "List View ER entity must match main LOD ER entity.",
                      zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
         return( -1 );
      }
      else
      {
         // Include the selected VOR.
         if ( CheckExistenceOfEntity( vUserSpec, "ListVOR" ) >= zCURSOR_SET )
            ExcludeEntity( vUserSpec, "ListVOR", zREPOS_AFTER );
         IncludeSubobjectFromSubobject( vUserSpec, "ListVOR",
                                        vVOR,      "ViewObjRef", zPOS_AFTER );
      }

      DropMetaOI( vSubtask, vMainLOD );
      DropMetaOI( vSubtask, vListLOD );
      DropMetaOI( vSubtask, vVOR );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SortByDate( zVIEW vSubtask )
{

   zVIEW  vT;
   zSHORT nRC;

   // Get view to List Object
   nRC = GetViewByName( &vT, "UI_SpecList", vSubtask, zLEVEL_TASK );

   // Sort UIS's by Name
   nRC = zgSortEntityWithinParent( zDESCENDING, vT,
                                   "W_MetaDef", "LastUpdateDate", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SortByDesc( zVIEW vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   // Get view to List Object
   nRC = GetViewByName( &vT, "UI_SpecList", vSubtask, zLEVEL_TASK );

   // Sort UIS's by Name
   nRC = zgSortEntityWithinParent( zASCENDING, vT,
                                   "W_MetaDef", "Desc", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_SortByName( zVIEW vSubtask )
{
   zVIEW  vT;
   zSHORT nRC;

   // Get view to List Object
   nRC = GetViewByName( &vT, "UI_SpecList", vSubtask, zLEVEL_TASK );

   // Sort UIS's by Name
   nRC = zgSortEntityWithinParent( zASCENDING, vT,
                                   "W_MetaDef", "Name", "" );
   SetCursorFirstEntity( vT, "W_MetaDef", "" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZADCDOD_NewDialogSpec( zVIEW vSubtask )
{
   // Make sure that a Default Spec OI exists before a new one is created.

   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zLONG  lFile;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szFileName, zsizeof( szFileName ),
                           vTaskLPLR, "LPLR", "MetaSrcDir" );
   strcat_s( szFileName, zsizeof( szFileName ), "\\Z__DFLT.BAS" );

   lFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
   if ( lFile < 0 )
   {
      // Default OI file does not exist, so give error.
      MessageSend( vSubtask, "AD10042", "Autodesigner",
                   "Defaults must be set before a new Dialog Spec is created.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZADCDOD_AutodesignReport
//
//  PURPOSE:   set the AutodesignCaseType to Report
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZADCDOD_AutodesignReport( zVIEW vSubtask )
{
   zVIEW  vUIS_List;

   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vUIS_List, "W_Entity",   "AutodesignCaseType", "R" );


   zwTZADCDOD_SelectCase( vSubtask );

   return( 0 );
} // zwTZADCDOD_AutodesignNone

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZADCDOD_AutodesignNone
//
//  PURPOSE:   set the AutodesignCaseType to None
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZADCDOD_AutodesignNone( zVIEW vSubtask )
{
   zVIEW  vUIS_List;

   GetViewByName( &vUIS_List, "UIS_List", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vUIS_List, "W_Entity",   "AutodesignCaseType", "N" );

   zwTZADCDOD_SelectCase( vSubtask );

   return( 0 );
} // zwTZADCDOD_AutodesignNone

#ifdef __cplusplus
}
#endif
