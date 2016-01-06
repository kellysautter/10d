/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzadwoaw.c - Zeidon application operations
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
// LAST MESSAGE ID: AD10913
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

25.04.1997 DonC
   Modified BasicWindowPainting routine to stop adding attributes to a
   group when the max size of the groupbox has been reached.

1999.01.25 DKS
   Fixed AutoDesign of MsgBar ctrl (originally DIL).

*/

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateNewWindowInfo
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateNewWindowInfo( zVIEW  vSubtask,
                                 zVIEW  vUserSpec,
                                 zVIEW  vNewDialog,
                                 zVIEW  vAutoDesWk )
{
   zVIEW    vTempWnd;
   zVIEW    vCtrlDef;
   zLONG    bAutoDesignSubordinates;
   zLONG    bFirstWindow;
   zCHAR    szOrigName[ 50 ];
   zCHAR    szWorkString[ 256 ];

// TraceLineS("IN", "CreateNewWindowInfo" );

   GetIntegerFromAttribute( &bAutoDesignSubordinates, vAutoDesWk,
                            "AutoDesignWork", "AutoDesignSubords" );
   GetIntegerFromAttribute( &bFirstWindow, vAutoDesWk,
                            "AutoDesignWork", "FirstWindow" );
   // If the user is autodesigning window and subordinates(or autodesign
   // dialog), then the only time a new window should be created here is
   // when we are on the first window.  Otherwise, the window was created
   // in CreateSubWindow and does not need to be created again.
   if ( bAutoDesignSubordinates == 0 ||
        (bAutoDesignSubordinates == 1 && bFirstWindow == 1) )
   {
      GetStringFromAttribute( szOrigName, sizeof( szOrigName ), vNewDialog, "Window", "Tag" );
      CreateViewFromViewForTask( &vTempWnd, vNewDialog, 0 );
      CreateMetaEntity( vSubtask, vNewDialog, "Window", zPOS_AFTER );
      DeleteEntity( vTempWnd, "Window", zREPOS_NONE );

      if ( CheckExistenceOfEntity( vUserSpec, "WndStyle" ) != 0 )
      {
         GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
         SetCursorFirstEntityByString( vCtrlDef, "WindowStyle", "Tag",
                                       "Dialog Box", "" );
         IncludeSubobjectFromSubobject( vNewDialog, "WndStyle",
                                        vCtrlDef, "WindowStyle", zPOS_AFTER );
      }
      else
      {
         IncludeSubobjectFromSubobject( vNewDialog, "WndStyle",
                                        vUserSpec, "WndStyle", zPOS_AFTER );
      }

      if ( szOrigName[ 0 ] != 0 )
      {
         SetAttributeFromString( vNewDialog, "Window", "Tag", szOrigName );
      }
      else
      {
         if ( CompareAttributeToString( vUserSpec, "WndDesign", "WndName", "" ) == 0 )
         {
            SetAttributeFromAttribute( vNewDialog, "Window", "Tag",
                                       vUserSpec, "LOD_Entity", "Name" );
         }
         else
         {
            SetAttributeFromAttribute( vNewDialog, "Window", "Tag",
                                        vUserSpec, "WndDesign", "WndName" );
         }
      }

      if ( CompareAttributeToString ( vUserSpec, "WndDesign", "WndCaption", "" ) == 0 )
      {
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, "LOD_Entity", "Name" );
         strcat_s( szWorkString, sizeof( szWorkString ), " Detail" );
         SetAttributeFromString( vNewDialog, "Window", "Caption",
                                  szWorkString );
      }
      else
      {
         SetAttributeFromAttribute( vNewDialog, "Window", "Caption",
                                     vUserSpec, "WndDesign", "WndCaption" );
      }
   }

   #if 0
   if ( CompareAttributeToString ( vUserSpec, "WndDesign", "ActionType", "O" ) == 0 )
   {
      ofnTZWDLGSO_CreateMenuActions( vUserSpec, vNewDialog, vAutoDesWk );
   }
   else
   {
      ofnTZWDLGSO_CreateButtonActions( vUserSpec, vNewDialog, vAutoDesWk );
   }
   #endif

   /* I AM USING WndNoMap TO HOLD THE 3D INFO FOR NOW BECAUSE WE AREN'T
      USING WndNoMap AND I WOULD HAVE TO MAKE A META MODEL CHANGE. */
   if ( CompareAttributeToString ( vUserSpec, "WndDesign", "WndNoMap", "Y" ) == 0 )
   {
      SetAttributeFromInteger( vNewDialog, "Window", "Subtype", 1 );
   }
// TraceLineS("END", "CreateNewWindowInfo" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateMenuActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateMenuActions( zVIEW    vSubtask,
                               zVIEW    vUserSpec,
                               zVIEW    vNewDialog,
                               zVIEW    vAutoDesWk )
{
   zVIEW    vTempView;
   zVIEW    vTempStyle;
   zVIEW    vNewDialog2;
   zLONG    lRC;
   zSHORT   bInclude;
   zCHAR    cAutoVersion[ 2 ];
   zCHAR    cCancel[ 2 ];
   zCHAR    cDelete[ 2 ];
   zCHAR    cSave[ 2 ];
   zCHAR    cPrev[ 2 ];
   zCHAR    cNext[ 2 ];
   zCHAR    szWorkString[ 256 ];
   zCHAR    szEntityName[ 33 ];

// TraceLineS("IN", "CreateMenuActions" );
   /* Create a new view that will point to the LOD entity in the user
      spec object.  Sometimes vUserSpec is pointing to a recursive
      subobject and LOD cannot be accessed. */
   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );
   ResetView( vTempStyle );

   GetStringFromAttribute( cAutoVersion, sizeof( cAutoVersion ), vUserSpec, "WndDesign", "AutoVersion" );
   GetStringFromAttribute( cCancel, sizeof( cCancel ), vUserSpec, "WndDesign", "UpdateCancelAction" );
   GetStringFromAttribute( cDelete, sizeof( cDelete ), vUserSpec, "WndDesign", "UpdateDeleteAction" );
   GetStringFromAttribute( cSave, sizeof( cSave ), vUserSpec, "WndDesign", "UpdateSaveAction" );
   GetStringFromAttribute( cNext, sizeof( cNext ), vUserSpec, "WndDesign", "UpdateNextAction" );
   GetStringFromAttribute( cPrev, sizeof( cPrev ), vUserSpec, "WndDesign", "UpdatePrevAction" );

   // Check the runtime permissions for this LOD_Entity to see if there is any conflicts with the actions.
   if ( cSave[ 0 ] == 'Y' && CompareAttributeToString( vUserSpec, "LOD_Entity", "Update", "Y" ) != 0 )
   {

      GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
      strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
      strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
      strcat_s( szWorkString, sizeof( szWorkString ), " does not allow an update.  " );
      strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Save window action?  " );
      lRC =  MessagePrompt( vUserSpec, "AD10901", "Autodesigner",
                            szWorkString, zBEEP,
                            zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      /* The user does not want to add the save option. */
      if ( lRC == zRESPONSE_NO )
         cSave[ 0 ] = 0;
   }
   if ( cDelete[ 0 ] == 'Y' )
   {
      /* If UIS_Include exists then this entity is to be included. */
      if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) == 0 )
         bInclude = 1;
      else
         bInclude = 0;

      /* This entity is not included so check the delete option. */
      if ( !bInclude && CompareAttributeToString( vUserSpec, "LOD_Entity", "Delete", "Y" ) != 0 )
      {
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
         strcpy_s(szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
         strcat_s(szWorkString, sizeof( szWorkString ), szEntityName );
         strcat_s(szWorkString, sizeof( szWorkString ), " does not allow a delete.  " );
         strcat_s(szWorkString, sizeof( szWorkString ), "Still add the Delete window action?  " );
         lRC = MessagePrompt( vUserSpec, "AD10902", "Autodesigner",
                              szWorkString, zBEEP,
                              zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
         /* The user does not want to add the delete option. */
         if ( lRC == zRESPONSE_NO )
            cDelete[ 0 ] = 0;
      }
      else
      /* This entity is included so check the exclude option. */
      if ( bInclude && CompareAttributeToString( vUserSpec, "LOD_Entity", "Exclude", "Y" ) != 0 )
      {
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
         strcpy_s(szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
         strcat_s(szWorkString, sizeof( szWorkString ), szEntityName );
         strcat_s(szWorkString, sizeof( szWorkString ), " does not allow an exclude.  " );
         strcat_s(szWorkString, sizeof( szWorkString ), "Still add the Delete window action?  " );
         lRC = MessagePrompt( vUserSpec, "AD10903", "Autodesigner",
                              szWorkString, zBEEP,
                              zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
         /* The user does not want to add the delete option. */
         if ( lRC == zRESPONSE_NO )
            cDelete[ 0 ] = 0;
      }
   }
   if ( cNext[ 0 ] == 'Y' &&
        CompareAttributeToString( vUserSpec, "LOD_Entity",
                                  "Update", "Y" ) != 0 )
   {

      GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec,
                              "LOD_Entity", "Name" );
      strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
      strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
      strcat_s( szWorkString, sizeof( szWorkString ), " does not allow an update.  " );
      strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Next and Prev window actions?  " );
      lRC =  MessagePrompt( vUserSpec, "AD10904", "Autodesigner",
                            szWorkString, zBEEP,
                            zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      /* The user does not want to add the Next option. */
      if ( lRC == zRESPONSE_NO )
      {
         cNext[ 0 ] = 0;
         cPrev[ 0 ] = 0;
      }
   }

   /* Create the main menu option. */
   CreateMetaEntity( vSubtask, vNewDialog, "Menu", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Menu", "Tag", "MainMenu" );
   IncludeSubobjectFromSubobject( vNewDialog, "DfltMenu",
                                    vNewDialog, "Menu", zPOS_AFTER );

   /* Create the save, delete, cancel, next and prev menu options. */
   if ( cCancel[ 0 ] == 'Y' || cSave[ 0 ] == 'Y' || cDelete[ 0 ] == 'Y' ||
        cNext[ 0 ] == 'Y' || cPrev[ 0 ] == 'Y' )
   {
      CreateMetaEntity( vSubtask, vNewDialog, "Option", zPOS_AFTER );
      if ( CompareAttributeToString ( vUserSpec, "WndDesign",
                                      "UpdtActionMenuTxt", "" ) == 0 )
      {
         SetAttributeFromString( vNewDialog, "Option", "Text", "&File" );
         SetAttributeFromString( vNewDialog, "Option", "Tag", "&File" );
      }
      else
      {
         SetAttributeFromAttribute( vNewDialog, "Option", "Text",
                                     vUserSpec, "WndDesign", "UpdtActionMenuTxt" );
         SetAttributeFromAttribute( vNewDialog, "Option", "Tag",
                                     vUserSpec, "WndDesign", "UpdtActionMenuTxt" );
      }
      CreateViewFromViewForTask( &vNewDialog2, vNewDialog, 0 );
      SetViewToSubobject( vNewDialog2, "OptOpt" );

      if ( cSave[ 0 ] == 'Y' )
      {
         CreateMetaEntity( vSubtask, vNewDialog2, "Option", zPOS_AFTER );
         if ( CompareAttributeToString ( vUserSpec, "WndDesign",
                                         "UpdateSaveText", "" ) == 0 )
         {
            SetAttributeFromString( vNewDialog2, "Option", "Text", "&Save" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Save" );
         }
         else
         {
            SetAttributeFromAttribute( vNewDialog2, "Option", "Text",
                                        vUserSpec, "WndDesign",
                                        "UpdateSaveText" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Save" );
         }
         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromString( vNewDialog, "Action", "Tag", "Save" );

         /* Set the system close event to the save action. */
         CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
         SetAttributeFromInteger( vNewDialog, "WndEvent", "Type", 256 );
         IncludeSubobjectFromSubobject( vNewDialog, "WndAct",
                                        vNewDialog, "Action", zPOS_AFTER );

         IncludeSubobjectFromSubobject( vNewDialog2, "OptAct",
                                         vNewDialog, "Action", zPOS_AFTER );

         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );
         /* If SaveRoutineFlagForRootEntity = 'S' then that means that
            the current entity is the root entity and so instead of
            autoversioning, the code "SaveInstance" will be called when
            this cancel button is pushed. */
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                  "SaveRoutineFlagForRootEntity", "S" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog,
                                            "SaveInstance", "",
                                            vNewDialog, "ActOper" );
            ofnTZWDLGSO_CreateSourceCode( vSubtask, vUserSpec, vNewDialog, "SaveInstance" );

        /*  SetAttributeFromString( vNewDialog, "Action", "OperName",
                                    "SaveInstance" ); */
            SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                                    "SaveRoutineFlagForRootEntity", " " );
         }
         else
         {
            SetAttributeFromInteger( vNewDialog, "Action",
                                     "SB_SubAction", 1 );
            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                           vUserSpec, "LOD_Entity",
                                           zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView",
                                           vTempStyle, "UIS_ViewObjRef",
                                           zPOS_AFTER );
         }
      }

      if ( cCancel[ 0 ] == 'Y' )
      {
         CreateMetaEntity( vSubtask, vNewDialog2, "Option", zPOS_AFTER );
         if ( cSave[ 0 ] != 'Y' && cDelete[ 0 ] != 'Y' )
         {
            strcpy_s(szWorkString, sizeof( szWorkString ), "Close" );
         }

         if ( CompareAttributeToString ( vUserSpec, "WndDesign",
                                         "UpdateCancelText", "" ) == 0 )
         {
            // If there is no Save or Delete button, then nothing can be updated so call option Close.
            if ( cSave[ 0 ] != 'Y' && cDelete[ 0 ] != 'Y' )
              SetAttributeFromString( vNewDialog2, "Option", "Text", "&Close" );
            else
              SetAttributeFromString( vNewDialog2, "Option", "Text", "&Cancel" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Cancel" );
         }
         else
         {
            // If there is no Save or Delete button, then nothing can be updated so call option Close.
            if ( cSave[ 0 ] != 'Y' && cDelete[ 0 ] != 'Y' )
               SetAttributeFromString( vNewDialog2, "Option", "Text", "&Close" );
            else
               SetAttributeFromAttribute( vNewDialog2, "Option", "Text",
                                          vUserSpec, "WndDesign", "UpdateCancelText" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Cancel" );
         }
         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromString( vNewDialog, "Action", "Tag", "ESC" );

         IncludeSubobjectFromSubobject( vNewDialog2, "OptAct", vNewDialog, "Action", zPOS_AFTER );

         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );
         SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

         /* If CancelRoutineFlagForRootEntity = 'C' then that means that
            the current entity is the root entity and so instead of
            autoversioning, the code "DeleteInstance" will be called when
            this cancel button is pushed. */
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "CancelRoutineFlagForRootEntity", "C" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog,
                                            "DeleteInstance", "",
                                            vNewDialog, "ActOper" );

            SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                                    "CancelRoutineFlagForRootEntity", " " );
         }
         else
         {
            SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 2 );
            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                           vUserSpec, "LOD_Entity", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView",
                                           vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
         }
      }

      if ( cDelete[ 0 ] == 'Y' )
      {
         CreateMetaEntity( vSubtask, vNewDialog2, "Option", zPOS_AFTER );
         if ( CompareAttributeToString ( vUserSpec, "WndDesign", "UpdateDeleteText", "" ) == 0 )
         {
            SetAttributeFromString( vNewDialog2, "Option", "Text", "&Delete" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Delete" );
         }
         else
         {
            SetAttributeFromAttribute( vNewDialog2, "Option", "Text",
                                        vUserSpec, "WndDesign", "UpdateDeleteText" );
            SetAttributeFromString( vNewDialog2, "Option", "Tag", "Delete" );
         }

         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromAttribute( vNewDialog, "Action", "Tag", vNewDialog2, "Option", "Text" );

         IncludeSubobjectFromSubobject( vNewDialog2, "OptAct", vNewDialog, "Action", zPOS_AFTER );

         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );

         // If DeleteRoutineFlagForRootEntity = 'D' then that means that
         // the current entity is the root entity and so instead of
         // autoversioning, the code "DeleteObjectEntity" will be called when
         // this cancel button is pushed.
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork", "DeleteRoutineFlagForRootEntity", "D" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "DeleteObjectEntity", "", vNewDialog, "ActOper" );

            SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "DeleteRoutineFlagForRootEntity", " " );
         }
         else
         {
            if ( !bInclude )
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 4 ); /* Delete */
            else
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 8 ); /* Exclude */

            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
         }
      }

      if ( cNext[ 0 ] == 'Y' )
      {
         lRC = zRESPONSE_YES;
         CreateViewFromViewForTask( &vTempView, vUserSpec, 0 );
         ResetViewFromSubobject( vTempView );

         MiGetParentEntityNameForView( szWorkString, vUserSpec,
                                       "UIS_Entity" );

         /* If vAutoDesWk.AutoDesignWork.NextRoutineFlagForRootEntity is not null, and
            the current entity is the root entity then the next action
            will not work and does not make sense on this window.  See if
            the user still wants to add the action. */
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "NextRoutineFlagForRootEntity", " " ) == 0 &&
                                         zstrcmp( "UI_Spec", szWorkString ) == 0 )
         {
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
            strcpy_s( szWorkString, sizeof( szWorkString ), "Because " );
            strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
            strcat_s( szWorkString, sizeof( szWorkString ), " is the root entity. The window "
                      "action Next will not work properly.  Still add the option?" );
            lRC = MessagePrompt( vUserSpec, "AD10905", "Autodesigner",
                                 szWorkString, zBEEP,
                                 zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
         }

         if ( lRC == zRESPONSE_YES )
         {
            CreateMetaEntity( vSubtask, vNewDialog2, "Option", zPOS_AFTER );
            if ( CompareAttributeToString( vUserSpec, "WndDesign", "UpdateNextText", "" ) == 0 )
            {
               SetAttributeFromString( vNewDialog2, "Option", "Text", "&Next" );
               SetAttributeFromString( vNewDialog2, "Option", "Tag", "Next" );
            }
            else
            {
               SetAttributeFromAttribute( vNewDialog2, "Option", "Text", vUserSpec, "WndDesign", "UpdateNextText" );
               SetAttributeFromString( vNewDialog2, "Option", "Tag", "Next" );
            }
            CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
            SetAttributeFromAttribute( vNewDialog, "Action", "Tag", vNewDialog2, "Option", "Text" );
            SetAttributeFromInteger( vNewDialog, "Action", "Type", 2 );

            /* If vAutoDesWk.AutoDesignWork.NextRoutineFlagForRootEntity is
               not null, then this means this is the root entity for auto
               designing a wdod.   Because this is the root entity, then
               the code for this Next button is ProcessNextSelectedItem
              (already in the template code). */
            IncludeSubobjectFromSubobject( vNewDialog2, "OptAct", vNewDialog, "Action", zPOS_AFTER );

            if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork",
                                           "NextRoutineFlagForRootEntity", "N" ) == 0 )
            {
               ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog,
                                               "ProcessNextSelectedItem", "",
                                               vNewDialog, "ActOper" );

               SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                                       "NextRoutineFlagForRootEntity", " " );
            }
            else
            {
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 1 );

               SetAttributeFromInteger( vNewDialog, "Action",  "SB_NextPrev", 128 );
               CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                              vUserSpec, "LOD_Entity", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapView",
                                              vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
            }
         }
      }

      if ( cPrev[ 0 ] == 'Y' )
      {
         lRC = zRESPONSE_YES;
         CreateViewFromViewForTask( &vTempView, vUserSpec, 0 );
         ResetViewFromSubobject( vTempView );

         MiGetParentEntityNameForView( szWorkString, vUserSpec, "UIS_Entity" );

         /* See comments above for next. */
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "PrevRoutineFlagForRootEntity", " " ) == 0 &&
                                         zstrcmp( "UI_Spec", szWorkString ) == 0 )
         {
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
            strcpy_s( szWorkString, sizeof( szWorkString ), "Because " );
            strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
            strcat_s( szWorkString, sizeof( szWorkString ), " is the root entity. The window "
                     "action Prev will not work properly.  Still add the option?" );
            lRC = MessagePrompt( vUserSpec, "AD10906", "Autodesigner",
                                 szWorkString, zBEEP,
                                 zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
         }

         if ( lRC == zRESPONSE_YES )
         {
            CreateMetaEntity( vSubtask, vNewDialog2, "Option", zPOS_AFTER );
            if ( CompareAttributeToString ( vUserSpec, "WndDesign", "UpdatePrevText", "" ) == 0 )
            {
               SetAttributeFromString( vNewDialog2, "Option", "Text", "&Prev" );
               SetAttributeFromString( vNewDialog2, "Option", "Tag", "Prev" );
            }
            else
            {
               SetAttributeFromAttribute( vNewDialog2, "Option", "Text", vUserSpec, "WndDesign", "UpdatePrevText" );
               SetAttributeFromString( vNewDialog2, "Option", "Tag", "Prev" );
            }
            CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
            SetAttributeFromAttribute( vNewDialog, "Action", "Tag", vNewDialog2, "Option", "Text" );
            SetAttributeFromInteger( vNewDialog, "Action", "Type", 2 );

            // See comments above for next.
            IncludeSubobjectFromSubobject( vNewDialog2, "OptAct", vNewDialog, "Action", zPOS_AFTER );

            if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork", "PrevRoutineFlagForRootEntity", "P" ) == 0 )
            {
               ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "ProcessPrevSelectedItem", "", vNewDialog, "ActOper" );

               SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "PrevRoutineFlagForRootEntity", " " );
            }
            else
            {
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 1 );

               SetAttributeFromInteger( vNewDialog, "Action", "SB_NextPrev", 256 );
               CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
            }
         }
      }

      DropView( vNewDialog2 );
   }

   DropView( vTempStyle );
// TraceLineS("END", "CreateMenuActions" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BuildButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_BuildButton( zVIEW  vSubtask,
                         zVIEW  vUserSpec,
                         zVIEW  vNewDialog,
                         zPCHAR szButtonText,
                         zPCHAR szDefaultText,
                         int    *nMaxLength,
                         int    *nNbrButtons )
{
   zVIEW vCtrlDef;
   int      nTempLength;
   zLONG    lCL;
   zLONG    lCC;
   zCHAR    szEntityName[ 33 ];
   zLONG    lTempInteger_0;
   long   lDlgUnits;
   HDC    hDC;

   lCL = 12;
   lCC = 5;
   GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
   *nNbrButtons = *nNbrButtons + 1;
   lTempInteger_0 = zstrcmp( "", szButtonText );
   if ( lTempInteger_0 == 0 )
   {
      strcat_s( szButtonText, sizeof( szButtonText ), szDefaultText );
   }

   nTempLength = zstrlen( szButtonText );

   // Get the width of the text in pixels, and convert that to Dlg Units.
   lDlgUnits = GetDialogBaseUnits( );
   g_lDlgUnitCharX = LOWORD( lDlgUnits );
   g_lDlgUnitCharY = HIWORD( lDlgUnits );

   hDC = GetDC( GetActiveWindow( ) );

#ifdef __WIN32__
   {  SIZE size;
   GetTextExtentPoint32( hDC,          // handle of device context
                         szButtonText, // address of text string
                         nTempLength,  // number of characters in string
                         &size );      // address of structure for string size

   nTempLength = size.cx;
   nTempLength = mConvertPixelToDlgUnitX( nTempLength );
   }
#else
   lDlgUnits = GetTextExtent( hDC, szButtonText, nTempLength );
   nTempLength = LOWORD( lDlgUnits );
   nTempLength = mConvertPixelToDlgUnitX( nTempLength );
#endif

   // I divide by lCC because my calculations on how to place the buttons
   // on the window are done in characters not dlg units.  I probably
   // should change the calculations to use dlg units in the future.
   nTempLength = (nTempLength + 10) / lCC;
   ReleaseDC( GetActiveWindow( ), hDC );

   if ( *nMaxLength < nTempLength )
   {
      *nMaxLength = nTempLength;
   }

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "PushBtn", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );

   // Remove the button border.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", zCONTROL_BORDEROFF );
   SetAttributeFromString( vNewDialog, "Control", "Text", szButtonText );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );

   // Create an event for the button.
   CreateMetaEntity( vSubtask, vNewDialog, "Event", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Event", "Type", 1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateButtonActions
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateButtonActions( zVIEW vSubtask,
                                 zVIEW vUserSpec,
                                 zVIEW vNewDialog,
                                 zVIEW vAutoDesWk )
{
   zVIEW    vTempView;
   zVIEW    vTempStyle;
   int      iMaxLength;
   int      iNbrButtons;
   zSHORT   nRC;
   zSHORT   bInclude;
   int      lCC;
   int      lCL;
   int      nScreenWidth;
   zLONG    lButtonPosition;
   zLONG    lLine;
   zLONG    lMaxWidth;
   zCHAR    szWorkString[ 256 ];
   zCHAR    szEntityName[ 33 ];
   zCHAR    cCancel[ 2 ];
   zCHAR    cDelete[ 2 ];
   zCHAR    cSave[ 2 ];
   zCHAR    cPrev[ 2 ];
   zCHAR    cNext[ 2 ];
   zCHAR    cAutoVersion[ 2 ];
   zSHORT   RESULT;

// TraceLineS("IN", "CreateButtonActions" );
   // Create a new view that will point to the LOD entity in the user
   // spec object.  Sometimes vUserSpec is pointing to a recursive
   // subobject and LOD cannot be accessed.
   CreateViewFromViewForTask( &vTempStyle, vUserSpec, 0 );
   ResetView( vTempStyle );

   lCL = 12;
   lCC = 5;

   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lLine, &lMaxWidth );

// lLine = 46;
   lLine = lLine + lCL;
   nScreenWidth = 321;
   iNbrButtons = 0;
   iMaxLength = 0;
   GetStringFromAttribute( cAutoVersion, sizeof( cAutoVersion ), vUserSpec, "WndDesign", "AutoVersion" );
   GetStringFromAttribute( cCancel, sizeof( cCancel ), vUserSpec, "WndDesign", "UpdateCancelAction" );
   GetStringFromAttribute( cDelete, sizeof( cDelete ), vUserSpec, "WndDesign", "UpdateDeleteAction" );
   GetStringFromAttribute( cSave, sizeof( cSave ), vUserSpec, "WndDesign", "UpdateSaveAction" );
   GetStringFromAttribute( cNext, sizeof( cNext ), vUserSpec, "WndDesign", "UpdateNextAction" );
   GetStringFromAttribute( cPrev, sizeof( cPrev ), vUserSpec, "WndDesign", "UpdatePrevAction" );

   // Check the runtime permissions for this LOD_Entity to see if there
   // is any conflicts with the actions.
   if ( cSave[ 0 ] == 'Y' &&
        CompareAttributeToString( vUserSpec, "LOD_Entity", "Update", "Y" ) != 0 )
   {

      GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
      strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
      strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
      strcat_s( szWorkString, sizeof( szWorkString ), " does not allow an update.  " );
      strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Save window action?  " );
      nRC = MessagePrompt( vUserSpec, "AD10907", "Autodesigner",
                           szWorkString, zBEEP,
                           zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      // The user does not want to add the save option.
      if ( nRC == zRESPONSE_NO )
         cSave[ 0 ] = 0;
   }
   if ( cDelete[ 0 ] == 'Y' )
   {
      // If UIS_Include exists, then this entity is included.
      if ( CheckExistenceOfEntity( vUserSpec, "UIS_Include" ) == 0 )
         bInclude = 1;
      else
         bInclude = 0;
      if ( !bInclude && CompareAttributeToString( vUserSpec, "LOD_Entity", "Delete", "Y" ) != 0 )
      {
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
         strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
         strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
         strcat_s( szWorkString, sizeof( szWorkString ), " does not allow a delete.  " );
         strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Delete window action?  " );
         nRC = MessagePrompt( vUserSpec, "AD10908", "Autodesigner",
                              szWorkString, zBEEP,
                              zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
         // The user does not want to add the delete option.
         if ( nRC == zRESPONSE_NO )
            cDelete[ 0 ] = 0;
      }
      else
      if ( bInclude && CompareAttributeToString( vUserSpec, "LOD_Entity", "Exclude", "Y" ) != 0 )
      {
         GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
         strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
         strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
         strcat_s( szWorkString, sizeof( szWorkString ), " does not allow an exclude.  " );
         strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Delete window action?  " );
         nRC = MessagePrompt( vUserSpec, "AD10909", "Autodesigner",
                              szWorkString, zBEEP,
                              zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
         // The user does not want to add the delete option.
         if ( nRC == zRESPONSE_NO )
            cDelete[ 0 ] = 0;
      }
   }
   if ( cNext[ 0 ] == 'Y' &&
        CompareAttributeToString( vUserSpec, "LOD_Entity", "Update", "Y" ) != 0 )
   {

      GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
      strcpy_s( szWorkString, sizeof( szWorkString ), "The runtime permission for entity " );
      strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
      strcat_s( szWorkString, sizeof( szWorkString ), " does not allow an update.  " );
      strcat_s( szWorkString, sizeof( szWorkString ), "Still add the Next and Prev window actions?  " );
      nRC = MessagePrompt( vUserSpec, "AD10910", "Autodesigner",
                           szWorkString, zBEEP,
                           zBUTTONS_YESNO, zRESPONSE_YES, zICON_QUESTION );
      /* The user does not want to add the Next option. */
      if ( nRC == zRESPONSE_NO )
      {
         cNext[ 0 ] = 0;
         cPrev[ 0 ] = 0;
      }
   }

   if ( cCancel[ 0 ] == 'Y' || cSave[ 0 ] == 'Y' || cDelete[ 0 ] == 'Y' ||
        cNext[ 0 ] == 'Y' || cPrev[ 0 ] == 'Y' )
   {
      if ( cSave[ 0 ] == 'Y' )
      {
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, "WndDesign", "UpdateSaveText" );
         ofnTZWDLGSO_BuildButton( vSubtask, vUserSpec, vNewDialog, szWorkString,
                                  "Save", &iMaxLength, &iNbrButtons );
         SetAttributeFromString( vNewDialog, "Control", "Tag", "Save" );

         // Set window default button to be save.
         SetAttributeFromString( vNewDialog, "Window", "DfltButton", "Save" );

         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromString( vNewDialog, "Action", "Tag", "Save" );

         IncludeSubobjectFromSubobject( vNewDialog, "EventAct",
                                       vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );

         // If SaveRoutineFlagForRootEntity = 'S' then that means that
         // the current entity is the root entity and so instead of
         // autoversioning, the code "SaveInstance" will be called when
         // this cancel button is pushed.
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "SaveRoutineFlagForRootEntity", "S" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "SaveInstance", "", vNewDialog, "ActOper" );
            ofnTZWDLGSO_CreateSourceCode( vSubtask, vUserSpec, vNewDialog, "SaveInstance" );

            SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "SaveRoutineFlagForRootEntity", " " );
         }
         else
         {
            SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 1 );
            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
         }
      }

      if ( cCancel[ 0 ] == 'Y' )
      {
         if ( cSave[ 0 ] != 'Y' && cDelete[ 0 ] != 'Y' )
         {
            /* If there is no Save or Delete button, then nothing can be updated
               so call button Close. */
            strcpy_s( szWorkString, sizeof( szWorkString ), "Close" );
            // Set window default button to be Cancel.
            SetAttributeFromString( vNewDialog, "Window", "DfltButton", "Cancel" );
         }
         else
            GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, "WndDesign", "UpdateCancelText" );

         ofnTZWDLGSO_BuildButton( vSubtask, vUserSpec, vNewDialog, szWorkString,
                                  "Cancel", &iMaxLength, &iNbrButtons );
         SetAttributeFromString( vNewDialog, "Control", "Tag", "Cancel" );
         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromString( vNewDialog, "Action", "Tag", "ESC" );

         // Set the system close event to the cancel action.
         CreateMetaEntity( vSubtask, vNewDialog, "WndEvent", zPOS_AFTER );
         SetAttributeFromInteger( vNewDialog, "WndEvent", "Type", 256 );
         IncludeSubobjectFromSubobject( vNewDialog, "WndAct", vNewDialog, "Action", zPOS_AFTER );
         IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );

         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );
         SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

         // If CancelRoutineFlagForRootEntity = 'C' then that means that
         // the current entity is the root entity and so instead of
         // autoversioning, the code "DeleteInstance" will be called when
         // this cancel button is pushed.
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "CancelRoutineFlagForRootEntity", "C" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "DeleteInstance", "", vNewDialog, "ActOper" );

            SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "CancelRoutineFlagForRootEntity", " " );
         }
         else
         {
            SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 2 );
            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
         }
      }

      if ( cDelete[ 0 ] == 'Y' )
      {
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, "WndDesign", "UpdateDeleteText" );
         ofnTZWDLGSO_BuildButton( vSubtask, vUserSpec, vNewDialog, szWorkString, "&Delete", &iMaxLength, &iNbrButtons );
         SetAttributeFromString( vNewDialog, "Control", "Tag", "Delete" );
         CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
         SetAttributeFromString( vNewDialog, "Action", "Tag", szWorkString );
         IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );

         SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );
         // If DeleteRoutineFlagForRootEntity = 'D' then that means that
         // the current entity is the root entity and so instead of
         // autoversioning, the code "DeleteObjectEntity" will be called when
         // this cancel button is pushed.
         if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "DeleteRoutineFlagForRootEntity", "D" ) == 0 )
         {
            ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "DeleteObjectEntity", "", vNewDialog, "ActOper" );
            SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "DeleteRoutineFlagForRootEntity", " " );
         }
         else
         {
            if ( !bInclude )
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 4 ); /* Delete */
            else
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 8 ); /* Exclude */

            CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
            IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
         }
      }

      if ( cNext[ 0 ] == 'Y' )
      {
         nRC = zRESPONSE_YES;
         CreateViewFromViewForTask( &vTempView, vUserSpec, 0 );
         ResetViewFromSubobject( vTempView );

         MiGetParentEntityNameForView( szWorkString, vUserSpec, "UIS_Entity" );

         /* If vAutoDesWk.AutoDesignWork.NextRoutineFlagForRootEntity is not
            null, and the current entity is the root entity then the next
            action will not work and does not make sense on this window.
            See if the user still wants to add the action. */
         if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork",
                                        "NextRoutineFlagForRootEntity", " " ) == 0 &&
                                        zstrcmp( "UI_Spec", szWorkString ) == 0 )
         {
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
            strcpy_s( szWorkString, sizeof( szWorkString ), "Because " );
            strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
            strcat_s( szWorkString, sizeof( szWorkString ), " is the root entity. The window "
                      " action Next will not work properly.  Still add the button?" );
            nRC = MessagePrompt( vUserSpec, "AD10911", "Autodesigner",
                                 szWorkString, zBEEP,
                                 zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
         }
         if ( nRC == zRESPONSE_YES )
         {
            GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec,
                                    "WndDesign", "UpdateNextText" );
            ofnTZWDLGSO_BuildButton( vSubtask, vUserSpec, vNewDialog, szWorkString,
                                     "&Next", &iMaxLength, &iNbrButtons );
            SetAttributeFromString( vNewDialog, "Control", "Tag", "Next" );
            CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
            SetAttributeFromString( vNewDialog, "Action", "Tag", szWorkString );
            SetAttributeFromInteger( vNewDialog, "Action", "Type", 2 );
            IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );

            /* If vAutoDesWk.AutoDesignWork.NextRoutineFlagForRootEntity is
               not null, then this means this is the root entity for auto
               designing a wdod.  Because this is the root entity, then
               the code for this Next button is ProcessNextSelectedItem
              (already in the template code). */
            if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork",
                                           "NextRoutineFlagForRootEntity", "N" ) == 0 )
            {
               ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog,
                                               "ProcessNextSelectedItem",
                                               "", vNewDialog, "ActOper" );

               SetAttributeFromString( vAutoDesWk, "AutoDesignWork",
                                       "NextRoutineFlagForRootEntity", " " );
            }
            else
            {
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 1 );

               SetAttributeFromInteger( vNewDialog, "Action", "SB_NextPrev", 128 );
               CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity",
                                              vUserSpec, "LOD_Entity", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapView",
                                              vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
            }
         }
      }

      if ( cPrev[ 0 ] == 'Y' )
      {
         nRC = zRESPONSE_YES;
         CreateViewFromViewForTask( &vTempView, vUserSpec, 0 );
         ResetViewFromSubobject( vTempView );

         MiGetParentEntityNameForView( szWorkString, vUserSpec, "UIS_Entity" );

         // See comments above for next.
         if ( CompareAttributeToString ( vAutoDesWk, "AutoDesignWork",
                                         "PrevRoutineFlagForRootEntity", " " ) == 0 &&
                                         zstrcmp( "UI_Spec", szWorkString ) == 0 )
         {
            GetStringFromAttribute( szEntityName, sizeof( szEntityName ), vUserSpec, "LOD_Entity", "Name" );
            strcpy_s( szWorkString, sizeof( szWorkString ), "Because " );
            strcat_s( szWorkString, sizeof( szWorkString ), szEntityName );
            strcat_s( szWorkString, sizeof( szWorkString ), " is the root entity. The window "
                      "action Prev will not work properly.  Still add the button?" );
            nRC = MessagePrompt( vUserSpec, "AD10912", "Autodesigner",
                                 szWorkString, zBEEP,
                                 zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );
         }
         if ( nRC == zRESPONSE_YES )
         {
            GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, "WndDesign", "UpdatePrevText" );
            ofnTZWDLGSO_BuildButton( vSubtask, vUserSpec, vNewDialog, szWorkString, "&Prev", &iMaxLength, &iNbrButtons );
            SetAttributeFromString( vNewDialog, "Control", "Tag", "Prev" );
            CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
            SetAttributeFromString( vNewDialog, "Action", "Tag", szWorkString );
            SetAttributeFromInteger( vNewDialog, "Action", "Type", 2 );
            IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );

            // See comments above for next.
            if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "PrevRoutineFlagForRootEntity", "P" ) == 0 )
            {
               ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, "ProcessPrevSelectedItem", "", vNewDialog, "ActOper" );

               SetAttributeFromString( vAutoDesWk, "AutoDesignWork", "PrevRoutineFlagForRootEntity", " " );
            }
            else
            {
               SetAttributeFromInteger( vNewDialog, "Action", "SB_SubAction", 1 );
               SetAttributeFromInteger( vNewDialog, "Action", "SB_NextPrev", 256 );
               CreateMetaEntity( vSubtask, vNewDialog, "ActMap", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapLOD_Entity", vUserSpec, "LOD_Entity", zPOS_AFTER );
               IncludeSubobjectFromSubobject( vNewDialog, "ActMapView", vTempStyle, "UIS_ViewObjRef", zPOS_AFTER );
            }
         }
      }

      iMaxLength = 40;

      // Set the X and Y coordinates for each of the button actions.
      lButtonPosition = lMaxWidth + 9;
      lLine = 6;
      RESULT = SetCursorFirstEntityByInteger( vNewDialog, "ControlDef", "ZKey", 1020, "Window" );
      while ( RESULT > zCURSOR_UNCHANGED )
      {
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLine );
         SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lButtonPosition );
         SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", iMaxLength );
         lLine = lLine + 15;
         RESULT = SetCursorNextEntityByInteger( vNewDialog, "ControlDef", "ZKey", 1020, "Window" );
      }
   }

   RESULT = SetCursorFirstEntity( vNewDialog, "Control", "" );
   DropView( vTempStyle );
// TraceLineS("END", "CreateButtonActions" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AddSpacesToString
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
AddSpacesToString( zPCHAR szWorkString )
{
   zCHAR  szWorkString2[ 100 ];
   zSHORT j, k;

   szWorkString2[ 0 ] = szWorkString[ 0 ];
   j = 1;
   k = 1;
   while ( szWorkString[ j ] != 0 )
   {
      if ( szWorkString[ j ] >= 'A' && szWorkString[ j ] <= 'Z' )
      {
         szWorkString2[ k++ ] = ' ';
         szWorkString2[ k++ ] = szWorkString[ j++ ];
      }
      else
         szWorkString2[ k++ ] = szWorkString[ j++ ];
   }

   szWorkString2[ k ] = 0;
   strcpy_s( szWorkString, sizeof( szWorkString ), szWorkString2 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    BasicWindowPainting
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_BasicWindowPainting( zVIEW    vSubtask,
                                 zVIEW    vUserSpec,
                                 zVIEW    vNewDialog,
                                 zVIEW    vDefaults,
                                 zVIEW    vAutoDesWk,
                                 zPCHAR   szLOD_AttributeName )
{
   zVIEW    vNewDialog2;
   zVIEW    vUserSpecRoot;
   zVIEW    vCtrlDef;
   zVIEW    vWndTop;
   zVIEW    vTZPNCTWO;
   zLONG    lCL;   // The heighth of a control
   zLONG    lCC;   // The width increment of a control
   zLONG    lCI;   // The vertical spacing between controls.
   zLONG    lLastPaintLine;
   zLONG    lLine;
   zLONG    lMaxLength;
   zLONG    i;
   zLONG    j;
   zLONG    k;
   zLONG    lScreenWidth;
   zLONG    lFirstPaintLine;
   zLONG    lLastWindowLine;
   zLONG    lMaxFieldDLG_X;
   zLONG    lMaxText;
   zLONG    lColumn;
   zLONG    lTmpNumber;
   zLONG    lFieldsPainted;
   zLONG    lLevel;
   zLONG    lTempInteger_1;
   zSHORT   RESULT;
   zSHORT   nRC;
   zSHORT   nRoutineRC = 0;
   zSHORT   lMaxGroupHeight = 400;
   zCHAR    szER_AttributeName[ 33 ];
   zCHAR    szDomainName[ 33 ];
   zCHAR    szWorkString[ 100 ];
   zCHAR    szWork[ 6 ];
   zCHAR    szUsage[ 33 ];
   zCHAR    szFirstDataFieldName[ 10 ];
   zCHAR    cUsage[ 2 ];
   zLONG    lFirstPosY;

// TraceLineS("IN", "BasicWindowPainting" );

   if ( zstrcmp( szLOD_AttributeName, "M_LOD_Attribute" ) == 0 )
   {
      strcpy_s( szER_AttributeName, sizeof( szER_AttributeName ), "M_ER_Attribute" );
      strcpy_s( szDomainName, sizeof( szDomainName ), "M_Domain" );
   }
   else
   {
      strcpy_s( szER_AttributeName, sizeof( szER_AttributeName ), "MT_ER_Attribute" );
      strcpy_s( szDomainName, sizeof( szDomainName ), "MT_Domain" );
   }

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );

   lCC = 5;
   lFirstPosY = 12;
   lFirstPaintLine = 0;
   lLastPaintLine = 0;
   lLine = 0;
   lMaxFieldDLG_X = 0;
   szFirstDataFieldName[ 0 ] = 0;
   lMaxLength = 0;
   lMaxText = 0;
   lFieldsPainted = 0;
   lScreenWidth = 321;
   CreateViewFromViewForTask( &vNewDialog2, vNewDialog, 0 );
   GetStringFromAttribute( cUsage, sizeof( cUsage ), vUserSpec, "WndDesign", "UsageStyle" );

   // Create a view to the Window entity.
   CreateViewFromViewForTask( &vWndTop, vNewDialog, 0 );
   ResetView( vWndTop );

   // Create a new view that will point to the root entity in the user
   // spec object.  Sometimes vUserSpec is pointing to a recursive
   // subobject and LOD cannot be accessed.
   CreateViewFromViewForTask( &vUserSpecRoot, vUserSpec, 0 );
// ResetView( vUserSpecRoot );  does too much
   while ( ResetViewFromSubobject( vUserSpecRoot ) == 0 )
   {
   }

   // Initialize the Editbox and Text height and the vertical increment
   // between them from the Default object.
   GetIntegerFromAttribute( &lCL, vDefaults, "InitValues", "EditboxHeight" );
   if ( lCL == 0 )
      lCL = 12;

   GetIntegerFromAttribute( &lCI, vDefaults, "InitValues", "EditboxVerticalSpacing" );
   if ( lCI == 0 )
      lCI = 3;

   // Set lMaxLength Based on fields in Style Loaded.
   RESULT = SetCursorFirstEntity( vNewDialog, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      if ( CompareAttributeToString( vNewDialog, "Control", "Tag", "RadioGrp" ) != 0 &&
           CompareAttributeToString( vNewDialog, "Control", "Tag", "GroupBox" ) != 0 &&
           CompareAttributeToString( vNewDialog, "Control", "Tag", "ListBox" ) != 0 )
      {
         GetIntegerFromAttribute( &i, vNewDialog, "Control", "PSDLG_X" );
         GetIntegerFromAttribute( &lTempInteger_1, vNewDialog, "Control", "SZDLG_X" );
         i = i + lTempInteger_1;
         lTmpNumber = i - lCC;
         if ( lTmpNumber > lMaxLength )
         {
            lMaxLength = i;
         }
      }

      RESULT = SetCursorNextEntity( vNewDialog, "Control", "" );
   }

   RESULT = SetCursorFirstEntity( vNewDialog, "Control", "" );
// RESULT = SetCursorFirstEntity( vUserSpec, szLOD_AttributeName, "" );

   // So that this code can be used to create multiple groups, we assume
   // we are positioned on the correct szLOD_AttributeName at this point,
   // so we will just continue from there.
   // We are going to loop here for all the attributes unless they won't
   // fit within a group.  In that case, we'll put as many attributes on
   // the group as possible and indicate through the return code that not
   // all attributes fit.

   RESULT = zCURSOR_SET;
   while ( RESULT >= zCURSOR_SET )
   {
      // Find an Empty Line in which to place the Attribute.
      lLastPaintLine = lLine;
      if ( CheckExistenceOfEntity ( vNewDialog, "Control" ) != 0 )
         lLine = lFirstPosY;
      else
         lLine = lLine + lCI + lCL;

      CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );

      // Get next control number for window for assigning unique Tags.
      // Use the WndTop view for Dialog to make sure we are positioned on the
      // Window itself for getting ControlTagCounter and not on a subcontrol.
      GetIntegerFromAttribute( &lFieldsPainted, vWndTop, "Window", "ControlTagCounter" );
      lFieldsPainted++;
      SetAttributeFromInteger( vWndTop, "Window", "ControlTagCounter", lFieldsPainted );
      zltoa( lFieldsPainted, szWork, sizeof( szWork ) );
      strcpy_s( szWorkString, sizeof( szWorkString ), "Text" );
      strcat_s( szWorkString, sizeof( szWorkString ), szWork );
      SetAttributeFromString( vNewDialog, "Control", "Tag", szWorkString );

      SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
      IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                     vCtrlDef, "ControlDef", zPOS_AFTER );

      // Remove the text border.
      SetAttributeFromInteger( vNewDialog, "Control", "Subtype",
                               zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLine );
      if ( lFirstPaintLine == 0 )
         lFirstPaintLine = lLine;

      // Text fields will all start in column 6.
      lColumn = 6;
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lColumn );
      if ( CompareAttributeToString( vUserSpec, szER_AttributeName,
                                     "PromptPaintLit", "" ) == 0 )
      {
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, szER_AttributeName, "Name" );

         // Add a ':' to the end.
         strcat_s( szWorkString, sizeof( szWorkString ), ":" );

         // Add spaces before any capital letters.
         AddSpacesToString( szWorkString );
      }
      else
      {
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vUserSpec, szER_AttributeName, "PromptPaintLit" );
      }

      SetAttributeFromString( vNewDialog, "Control", "Tag", szWorkString );
      SetAttributeFromString( vNewDialog, "Control", "Text", szWorkString );
      j = zstrlen( szWorkString );
      if ( j > 0 )
      {
         j--;
         szWorkString[ j ] = 0;  // remove colon for setting of Tag below
      }

      j = j * lCC;

      // Limit the maximum size of a text field to be 25 chars.
      if ( j > 75 )
         j = 75;

      if ( j > lMaxText )
      {
         lMaxText = j;
      }

      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", j );

      // Text fields should only have a height of 10 dlgs.
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", 10 );
      lColumn = lColumn + j;
      lColumn = lColumn + lCC;

      j = ofnTZWDLGSO_GetSizeForAttr( vSubtask, vUserSpec, szDomainName, szER_AttributeName );

      j = j * lCC;
      j = j + lCC;

      // Limit the size of the update field to be no more than 30 and
      // no less than 2.
      if ( j > 150 )
         j = 150;
      else
      if ( j < 15 )
         j = 15;

      // Create the appropriate Data Field for the Attribute.
      CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   // strcpy_s( szWorkString, sizeof( szWorkString ), "EditBox" );
   // strcat_s( szWorkString, sizeof( szWorkString ), szWork );
      SetAttributeFromString( vNewDialog, "Control", "Tag", szWorkString );

      // Save the name of the first data field in UIS_Entity for use by
      // other functions.
      if ( szFirstDataFieldName[ 0 ] == 0 )
      {
         strcpy_s( szFirstDataFieldName, sizeof( szFirstDataFieldName ), szWorkString );
         SetAttributeFromString( vUserSpec, "UIS_Entity", "FirstDataFieldName", szFirstDataFieldName );
      }

      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", j );
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLine );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lColumn );
      if ( lMaxFieldDLG_X < lColumn )
      {
         lMaxFieldDLG_X = lColumn;
      }

      lColumn = lColumn + j - lCC;
      if ( lMaxLength < lColumn )
      {
         lMaxLength = lColumn;
      }

      if ( cUsage[ 0 ] == 'U' )
         SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "EditBox", "" );
      else
         SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );

      IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );

      // If the control usage is update, then check to make sure the control
      // should not be an output control (if it is a generated key etc.)
      if ( cUsage[ 0 ] == 'U' )
      {
         // If attribute is a generated key.
         if ( CompareAttributeToString( vUserSpec, szLOD_AttributeName,  "ParticipatesInKey", "G" ) == 0 ||
              CompareAttributeToString( vUserSpec, szLOD_AttributeName, "Derived", "Y" ) == 0 )
         {
            SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
            ExcludeEntity( vNewDialog, "ControlDef", zREPOS_PREV );
            IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
         // SetAttributeFromInteger( vNewDialog, "Control", "Type", 1000 );
         }
         else
         // See if Domain for field qualifies for a COMBO Box.
         if ( CompareAttributeToString( vUserSpec, szDomainName, "DomainType", "T" ) == 0 )
         {
            SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "ComboBox", "" );
            ExcludeEntity( vNewDialog, "ControlDef", zREPOS_PREV );
            IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
            // We must set the CtrlBOI.
            ActivateEmptyObjectInstance( &vTZPNCTWO, "TZPNCTWO", vUserSpec, zSINGLE );
            CreateEntity( vTZPNCTWO, "TZPNCTWO", zPOS_AFTER );
            CreateEntity( vTZPNCTWO, "ComboBox", zPOS_AFTER );
            SetAttributeFromInteger( vTZPNCTWO, "ComboBox", "Subtype", 0 );
            // Set Control.CtrlBOI from work entity ComboBox
            SetBlobFromEntityAttributes( vNewDialog, "Control", "CtrlBOI", vTZPNCTWO, "ComboBox" );
            DropView( vTZPNCTWO );

            // Set the control subtype to 0 (Domain).
            SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 0 );
            // A combobox's height should be needs to be larger than a
            // regular control because it has to have room to drop down.
            SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL * 5 );
            // A combobox's width should be a little wider to account for the up and down arrows.
            AddToAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lCC * 3 );
            // Do I need to set CtrlMapContext and\or CtrlMapView?
         }
      }

      CreateMetaEntity( vSubtask, vNewDialog, "CtrlMap", zPOS_AFTER );
      if ( CheckExistenceOfEntity( vUserSpec, "UIS_ViewObjRef" ) == 0 )
      {
         IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView",
                                        vUserSpec, "UIS_ViewObjRef", zPOS_AFTER );
      }
      else
      {
         IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapView",
                                        vUserSpecRoot, "UIS_ViewObjRef", zPOS_AFTER );
      }

      IncludeSubobjectFromSubobject( vNewDialog, "CtrlMapLOD_Attribute",
                                     vUserSpec, szLOD_AttributeName, zPOS_AFTER );

      // Exit the loop and set the return code if we've reached the max size of the group.
      if ( lLine > lMaxGroupHeight )
      {
         nRoutineRC = 1;
         break;
      }

      RESULT = SetCursorNextEntity( vUserSpec, szLOD_AttributeName, "" );
   }

   lLastPaintLine = lLine;

   //////////////////////////////////////////////////////////////////////////
   // END OF BASIC WINDOW PAINTING
   //////////////////////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////////////////
   // ALIGN THE FIELDS BASED ON THE LARGEST PAINT LITERAL
   // AND RIGHT JUSTIFY Text LITERALS
   //////////////////////////////////////////////////////////////////////////

   SetViewFromView( vNewDialog2, vNewDialog );
   RESULT = SetCursorFirstEntity( vNewDialog2, "Control", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetIntegerFromAttribute( &lLine, vNewDialog2, "Control", "PSDLG_Y" );
      if ( lLine >= lFirstPaintLine && lLine <= lLastPaintLine )
      {
         GetStringFromAttribute( szUsage, sizeof( szUsage ), vNewDialog2, "ControlDef", "Tag" );
         GetStringFromAttribute( szWorkString, sizeof( szWorkString ), vNewDialog2, "Control", "Text" );
         nRC = zstrcmp( szUsage, "Text" );
         // An output field is a text field without the text.
         // if ( zstrcmp( szUsage, "Text" ) != 0 )
         if ( nRC != 0 || (nRC == 0 && CheckExistenceOfEntity( vNewDialog2, "CtrlMap" ) == 0) )
         {
            GetIntegerFromAttribute( &j, vNewDialog2, "Control", "PSDLG_X" );
            GetIntegerFromAttribute( &k, vNewDialog2, "Control", "SZDLG_X" );
            if ( j < lMaxFieldDLG_X )
            {
               j = lMaxFieldDLG_X - j;
               /*
               if ( lMaxFieldDLG_X + k  > lMaxLength + lCC )
               {
                  if ( lMaxFieldDLG_X + k <= nScreenWidth + 1 )
                  {
                     lMaxLength = lMaxFieldDLG_X +( k - lCC );
                  }
                  else
                  {
                     j = j -( ( lMaxFieldDLG_X + k ) - ( nScreenWidth + 1 ) );
                     if ( j > 0 )
                     {
                        GetIntegerFromAttribute( &lTempInteger_14,
                                                  vNewDialog2,
                                                  "Control", "PSDLG_X" );
                        lTempInteger_14 = lTempInteger_14 + j;
                        SetAttributeFromInteger( vNewDialog2,
                                                  "Control", "PSDLG_X",
                                                  lTempInteger_14 );
                        lMaxLength = nScreenWidth;
                        j = -1;
                     }
                  }
               }
               */
               if ( j > 0 )
               {
                  SetAttributeFromInteger( vNewDialog2, "Control", "PSDLG_X", lMaxFieldDLG_X );
               }
            }
         }
      }

      RESULT = SetCursorNextEntity( vNewDialog2, "Control", "" );
   }

   lLastWindowLine = lLine;

   //////////////////////////////////////////////////////////////////////////
   // END OF FIELD ALIGNMENT AND Text JUSTIFICATION
   //////////////////////////////////////////////////////////////////////////

   //////////////////////////////////////////////////////////////////////////
   //  WINDOW COMPRESSION
   //////////////////////////////////////////////////////////////////////////

   // If the window painted has too many lines for the
   // terminal being used for painting. Attempt to
   // compress the window into a more manageable size by combining
   // painted fields which are located on separate lines

   // If the user wants to cascade the windows, then get the indent level
   // and divide by three(so we get numbers 1, 2, 3 etc not 0, 3, 6, etc).
   // Then add one to the level so that the first LOD_Entity is at level
   // 1 instead of zero.
   if ( CompareAttributeToString( vUserSpec, "WndDesign", "Cascade", "Y" ) == 0 )
   {
      GetIntegerFromAttribute( &lLevel, vUserSpec, "LOD_Entity", "IndentLvl" );
      lLevel = ((lLevel / 3) + 1) * 12;

      // Use temp view to reset to top of object.
      SetAttributeFromInteger( vWndTop, "Window", "PSDLG_X", lLevel );
      SetAttributeFromInteger( vWndTop, "Window", "PSDLG_Y", lLevel );
   }

   DropView( vNewDialog2 );
   DropView( vUserSpecRoot );
   DropView( vWndTop );
// TraceLineS("END", "BasicWindowPainting" );
   return( nRoutineRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    AutoDesignWindow
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_AutoDesignWindow( zVIEW vSubtask,
                              zVIEW vUserSpec,
                              zVIEW vNewDialog,
                              zVIEW vAutoDesWk )
{
   zLONG    lCL;
   zLONG    lCC;
   zLONG    lRC = 0;
   zSHORT   nScreenWidth;
   zSHORT   RESULT;

// TraceLineS("IN", "AutoDesignWindows" );
   nScreenWidth = 321;
   lCL = 12;
   lCC = 5;

   if ( CompareAttributeToString( vAutoDesWk, "AutoDesignWork", "AutoDesignNewWnd", "Y" ) == 0 )
   {
      ofnTZWDLGSO_CreateNewWindowInfo( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );
   }
   else
   {
      RESULT = SetCursorFirstEntity( vNewDialog, "Control", "" );
   }

// lRC = ofnTZWDLGSO_BasicWindowPainting( vUserSpec, vNewDialog, vAutoDesWk );

   if ( CompareAttributeToString( vUserSpec, "WndDesign", "ActionType", "O" ) == 0 )
   {
      ofnTZWDLGSO_CreateMenuActions( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );
   }
   else
   {
      ofnTZWDLGSO_CreateButtonActions( vSubtask, vUserSpec, vNewDialog, vAutoDesWk );
   }

// TraceLineS("END", "AutoDesignWindows" );
   return( lRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateTheButton
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateTheButton( zVIEW  vSubtask,
                             zVIEW  vNewDialog,
                             zLONG  lLine,
                             zLONG  lColumn,
                             zPCHAR szActionText,
                             zLONG  lButtonWidth )
{
   zVIEW vCtrlDef;
   zLONG lCC;
   zLONG lCL;

   lCL = 12;
   lCC = 5;
   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "PushBtn", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );
   // Remove the button border.
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype", zCONTROL_BORDEROFF );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lLine );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", lColumn );
   SetAttributeFromString( vNewDialog, "Control", "Tag", szActionText );
   SetAttributeFromString( vNewDialog, "Control", "Text", szActionText );
   if ( lButtonWidth == 0 )
   {
      lButtonWidth = 40;
   }

   if ( lButtonWidth == 0 )
   {
      lButtonWidth = 1 * lCC;
   }

   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lButtonWidth );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );
   CreateMetaEntity( vSubtask, vNewDialog, "Action", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Action", "Tag", szActionText );
   CreateMetaEntity( vSubtask, vNewDialog, "Event", zPOS_AFTER );
   SetAttributeFromInteger( vNewDialog, "Event", "Type", 1 );
   IncludeSubobjectFromSubobject( vNewDialog, "EventAct", vNewDialog, "Action", zPOS_AFTER );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    CreateTheOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateTheOperation( zVIEW   vSubtask,
                                zVIEW   vOper,
                                zPCHAR  szOperName,
                                zPCHAR  szOperDesc,
                                zVIEW   vInclude,
                                zPCHAR  szIncludeEntityName )
{
   zPCHAR   szName;
   zSHORT   nRC;
   zCHAR    szWorkString[ 256 ];

// TraceLineS("IN CreateTheOperation for:", szOperName );

   // If the Operation has not already been added, add it.

   nRC = SetCursorFirstEntityByString( vOper, "Operation", "Name", szOperName, "" );
   if ( nRC < zCURSOR_SET )
   {
      // create and initialize a Dialog Operation
      CreateMetaEntity( vSubtask, vOper, "Operation", zPOS_LAST );
      SetAttributeFromString( vOper, "Operation", "Name", szOperName );
      SetAttributeFromString( vOper, "Operation", "Desc", szOperDesc );
      SetAttributeFromString( vOper, "Operation", "Type", "D" );

      // create the single parameter required of all dialog operations
      CreateMetaEntity( vSubtask, vOper, "Parameter", zPOS_AFTER );
      SetAttributeFromString( vOper, "Parameter", "ShortDesc", "vNewDialog" );
      SetAttributeFromString( vOper, "Parameter", "DataType", "V" );
   }

   // include the operation under the appropriate 'invoking' instance
   nRC = CheckExistenceOfEntity( vInclude, szIncludeEntityName );
   if ( nRC < zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( vInclude, szIncludeEntityName,
                                     vOper, "Operation", zPOS_AFTER );
   }
   else
   {
      strcpy_s( szWorkString, sizeof( szWorkString ), "Attempt to add operation '" );
      strcat_s( szWorkString, sizeof( szWorkString ), szOperName );
      strcat_s( szWorkString, sizeof( szWorkString ), "' to an Action that has associated operation '" );
      GetAddrForAttribute( &szName, vInclude, szIncludeEntityName, "Name" );
      strcat_s( szWorkString, sizeof( szWorkString ), szName );
      strcat_s( szWorkString, sizeof( szWorkString ), "'." );
      MessageSend( vInclude, "AD10913", "Autodesigner",
                   szWorkString, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

// TraceLineS("END", "CreateTheOperation" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateDIL( zVIEW   vSubtask,
                       zVIEW   vNewDialog,
                       zVIEW   vDefaults )
{
   zVIEW  vCtrlDef;
   zCHAR  szDIL_Flag[ 2 ];
   zLONG  lMaxY;
   zLONG  lPosY;
   zLONG  lSizeY;
   zSHORT nRC;

   // Put a DIL message on the window if requested and then reset the
   // height of the window.
   // The PosY of the DIL will be the highest PosY value of any Groupbox + its
   // height + 5.
   GetStringFromAttribute( szDIL_Flag, sizeof( szDIL_Flag ), vDefaults, "InitValues", "DIL_MessageFlag" );
   if ( szDIL_Flag[ 0 ] == 'Y' )
   {
      // Determine PosY.
      lMaxY = 0;
      nRC = SetCursorFirstEntity( vNewDialog, "Control", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lPosY, vNewDialog, "Control", "PSDLG_Y" );
         GetIntegerFromAttribute( &lSizeY, vNewDialog, "Control", "SZDLG_Y" );
         if ( lMaxY < lPosY + lSizeY )
            lMaxY = lPosY + lSizeY;

         nRC = SetCursorNextEntity( vNewDialog, "Control", 0 );
      }

      CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
      SetAttributeFromString( vNewDialog, "Control", "Tag", "DIL1" );
      SetAttributeFromInteger( vNewDialog, "Control", "Subtype", 1 );
      SetAttributeFromString( vNewDialog, "Window", "DIL_Active", "Y" );

      GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
      SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "MsgBar", "" );
      IncludeSubobjectFromSubobject( vNewDialog, "ControlDef", vCtrlDef, "ControlDef", zPOS_AFTER );

      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", lMaxY + 5 );
      SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 1 );
      SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", 16 );
   // SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", 0 );
      SetAttributeFromAttribute( vNewDialog, "Control", "SZDLG_X",
                                 vNewDialog, "Window",  "SZDLG_X" );
   }

   return( 0 );
}
