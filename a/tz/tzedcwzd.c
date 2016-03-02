/*

   Because of problems with the CodeWright DLLs hanging around, all calls
   to CW must be made from a dll that can be explicetly free'd.  To do this,
   the ZEDITOR DLL containing all the CW calls has been created.  All calls
   to operations contained in ZEDITOR must be done via GetProcAddress.  This
   allows the ZEDITOR dll to be free'd later on.

   This module contains all the dialog operations for the TZEDCWZD dialog.
   Any dialog operations that require CW calls are then passed on to the
   ZEDITOR DLL.

   This module is compiled into the TZEDFRMD DLL.

*/
/*
CHANGE LOG

2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES;
   Remove Prefix "fn " and "vml " of  C-Function name and VML-Operation
   name in function VML_StartHelp and OpIns_StartOperationHelp

10.12.1997  DonC

   Modified AEQ_SelectView to eliminate null error for an entity with no
   attributes.

*/

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "tz__oprs.h"
#include "ZeidonOp.H"
#include "tzlodopr.h"
#include <ctype.h>

/////////////////////////////////////////////////////////////////////////////

// Following are declared in tzedfrmd.c
zSHORT fnCallEditorOperation( zVIEW vSubtask, zPCHAR pszOpName );

/////////////////////////////////////////////////////////////////////////////

#define GetWorkView( pvView )    GetViewByName( (pvView), "TZEDWRKO", 0, \
                                                zLEVEL_TASK )
#define GetProfileView( pvView ) GetViewByName( (pvView), "ProfileXFER", 0, \
                                                zLEVEL_TASK )

zOPER_EXPORT zSHORT OPERATION
SystemClose( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SystemClose" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZEDCWZD_SetFocusEditor( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "zwTZEDCWZD_SetFocusEditor" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_RefreshAEQ( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "ViewList" );
   RefreshCtrl( vSubtask, "EntityList" );
   RefreshCtrl( vSubtask, "AttributeList" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_RefreshAEQ_AttrList( zVIEW vSubtask )
{
   RefreshCtrl( vSubtask, "AttributeList" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_ClearViews( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zSHORT nRC;

   GetWorkView( &vEdWrk );
   if ( vEdWrk == 0 )
   {
      zCHAR sz[ 200 ];

      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at AEQ_ClearViews." );
      OperatorPrompt( vSubtask, "ED - Error", sz, 0, zBUTTONS_OK, 0, zICON_STOP );
      return( zCALL_ERROR );
   }

   nRC = SetCursorFirstEntity( vEdWrk, "View", "" );
   while ( nRC == zCURSOR_SET )
      nRC = DeleteEntity( vEdWrk, "View", zREPOS_NEXT );

   AEQ_RefreshAEQ( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_Postbuild( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_Postbuild" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_Close( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_Close" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_GetViews( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_GetViews" );
   return( 0 );
} // AEQ_GetViews

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteView( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_PasteView" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntity( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_PasteViewEntity" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntity_Quoted( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_PasteViewEntity_Quoted" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntityAttr( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_PasteViewEntityAttr" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_PasteViewEntityAttr_Quoted( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "AEQ_PasteViewEntityAttr_Quoted" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_SelectView( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vList;
   zVIEW  vLOD;
   zVIEW  vEdWrk;
   zPCHAR szLOD_Name;
   zPCHAR szName;

   GetWorkView( &vEdWrk );
   if ( vEdWrk == 0 )
   {
      zCHAR sz[ 200 ];

      strcpy_s( sz, zsizeof( sz ), "TZEDWRKO not Available at ObjSelAQ_ObjSelView." );
      OperatorPrompt( vSubtask, "ED - Error", sz, 0, zBUTTONS_OK, 0, zICON_STOP );
      return( zCALL_ERROR );
   }

   // If Entity entity exists, then the LOD has already been loaded for
   // for this view.  Exit because nothing else needs to be done.
   if ( CheckExistenceOfEntity( vEdWrk, "Entity" ) == zCURSOR_SET )
   {
      AEQ_RefreshAEQ( vSubtask );
      return( 0 );
   }

   // Activate the LOD specified by vEdWrk.View.ObjectName
   GetAddrForAttribute( &szLOD_Name, vEdWrk, "View", "ObjectName" );
   nRC = RetrieveViewForMetaList( vSubtask, &vList, zREFER_LOD_META );
   nRC = SetCursorFirstEntity( vList, "W_MetaDef", "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &szName, vList, "W_MetaDef", "Name" );
      if ( zstrcmpi( szName, szLOD_Name ) == 0 )
         break;

      nRC = SetCursorNextEntity( vList, "W_MetaDef", "" );
   }

   if ( nRC != zCURSOR_SET )
   {
      zCHAR sz[ 200 ];

      DropView( vList );
      strcpy_s( sz, zsizeof( sz ), "LOD " );
      strcat_s( sz, zsizeof( sz ), szLOD_Name );
      strcat_s( sz, zsizeof( sz ), " not found. Possibly wrong LPLR?" );
      OperatorPrompt( vSubtask, "ZeidonEditor", sz, 0, 0, 1, zICON_STOP );
      return( zCALL_ERROR );
   }

   nRC = ActivateMetaOI( vSubtask, &vLOD, vList, zREFER_LOD_META,
                         zSINGLE | zLEVEL_APPLICATION );
   DropView( vList );

   // Copy Entity and Attribute names from LOD to vEdWrk.
   nRC = SetCursorFirstEntity( vLOD, "LOD_Entity", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateEntity( vEdWrk, "Entity", zPOS_LAST );
      SetAttributeFromAttribute( vEdWrk, "Entity", "EntityName",
                                 vLOD, "LOD_Entity", "IndentName" );

      nRC = SetCursorFirstEntity( vLOD, "LOD_Attribute", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vEdWrk, "Attribute", zPOS_LAST );
         SetAttributeFromAttribute( vEdWrk, "Attribute", "AttributeName",
                                    vLOD, "ER_Attribute", "Name" );

         nRC = SetCursorNextEntity( vLOD, "LOD_Attribute", "" );
      }

      nRC = SetCursorNextEntity( vLOD, "LOD_Entity", "" );
   }

   DropMetaOI( vSubtask, vLOD );
   nRC = SetCursorFirstEntity( vEdWrk, "Entity", "" );
   SetSelectStateOfEntity( vEdWrk, "Entity", 1 );

   if ( CheckExistenceOfEntity( vEdWrk, "Attribute" ) >= zCURSOR_SET )
      SetSelectStateOfEntity( vEdWrk, "Attribute", 1 );

   AEQ_RefreshAEQ( vSubtask );

   return( 0 );

} // AEQ_SelectView

zOPER_EXPORT zSHORT OPERATION
AEQ_ViewPopup( zVIEW vSubtask )
{
   POINT  pt;

   pt.x = -1;
   CreateTrackingPopupMenu( vSubtask, "ViewPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_EntityPopup( zVIEW vSubtask )
{
   POINT  pt;

   pt.x = -1;
   CreateTrackingPopupMenu( vSubtask, "EntityPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
AEQ_AttributePopup( zVIEW vSubtask )
{
   POINT  pt;

   pt.x = -1;
   CreateTrackingPopupMenu( vSubtask, "AttributePopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_Cancel( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_Cancel" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_Exit( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_Exit" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_SaveFile( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_SaveFile" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_SaveAndParse( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_SaveAndParse" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_SaveAll( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_SaveAll" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SaveBuffers_ParseAll( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "SaveBuffers_ParseAll" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_Prebuild( zVIEW vSubtask )
{
   zSHORT   nRC;
   zVIEW    vOp;
   zVIEW    vSource;

   nRC = GetViewByName( &vOp, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( nRC > 0 )
      DropView( vOp );

   RetrieveViewForMetaList( vSubtask, &vOp, zREFER_LOD_META );
   OrderEntityForView( vOp, "W_MetaDef", "Name A" );
   SetNameForView( vOp, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   // Make sure that there is no TZOPSRCO.
   nRC = GetViewByName( &vSource, "TZOPSRCO", vSubtask, zLEVEL_TASK );
   if ( vSource )
      DropMetaOI( vSubtask, vSource );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortMetaListByName( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "Name" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SortMetaListByDate ( zVIEW vSubtask )
{
   zVIEW  vMetaList;

   GetViewByName( &vMetaList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   OrderEntityForView( vMetaList, "W_MetaDef", "LastUpdateDate D" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OBJ_PasteObjectName( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "OBJ_PasteObjectName" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_Prebuild( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zVIEW  vList;

   GetWorkView( &vEdWrk );
   GetViewByName( &vList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vList )
      DropView( vList );

   RetrieveViewForMetaList( vSubtask, &vList, zREFER_VOR_META );
   SetNameForView( vList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VOR_PasteName( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "VOR_PasteName" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_RefreshSelectButton( zVIEW vSubtask )
{
   zVIEW  vEdWrk;
   zCHAR  szCurrentType[ 5 ];

   GetWorkView( &vEdWrk );
   GetStringFromAttribute( szCurrentType /* Returned String */,
                           vEdWrk /* View Id */,
                           "OperListType" /* Entity Name */,
                           "Type" /* Attribute Name */ );
   if ( zstrcmp( szCurrentType, "Z" ) == 0 )
   {
      MB_SetMessage( vSubtask, 0, "Press F1 for Help on the operation." );
   }

   RefreshCtrl( vSubtask, "SelectButton" );
   return( 0 );
}

/*

   This operation builds the operation list for the OperationList window.
   Depending on what the value is for vProfileXFER.ED.OpIns_CurrentType
   we build different operations lists.

*/
zOPER_EXPORT zSHORT OPERATION
OpIns_BuildOperList( zVIEW vSubtask )
{
#define DISP_SELECT_SET    1
#define SEL_SELECT_SET     2

   zCHAR  szCurrentType[ 5 ];
   zVIEW  vProfileXFER;
   zVIEW  vEdWrk;
   zSHORT nRC;
   zCHAR  szKeyword[ 255 ];
   zLONG  lSelectCount = 0;

   GetWorkView( &vEdWrk );
   GetProfileView( &vProfileXFER );
   GetStringFromAttribute( szCurrentType, zsizeof( szCurrentType ), vProfileXFER, "ED",
                           "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, zsizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vProfileXFER, "ED", "OpIns_CurrentType",
                              szCurrentType );
   }

   // Check to see if the operations for the current type are already
   // loaded. If they are, then we obviously don't need to load them again.
   if ( SetCursorFirstEntityByString( vEdWrk, "OperListType", "Type",
                                      szCurrentType, 0 ) != zCURSOR_SET )
   {
      // Operations haven't been loaded yet, so load 'em up.

      CreateEntity( vEdWrk, "OperListType", zPOS_LAST );
      SetAttributeFromString( vEdWrk, "OperListType", "Type", szCurrentType );

      switch ( *szCurrentType )
      {
         case 'Z':
         {
            zVIEW vTZOPSIGK;

            // Get list of Zeidon operations.

            // Check to see if we've already activated the Zeidon opers list.
            // If not, activate it and save the view.
            GetViewByName( &vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );
            if ( !vTZOPSIGK )
            {
               oTZOPGRPO_GetViewForXGO( vSubtask, &vTZOPSIGK, zSYS_CURRENT_OI );
               SetNameForView( vTZOPSIGK, "TZOPSIGK", vSubtask, zLEVEL_TASK );
            }

            // Now copy each of the operations to the editor work object.
            for ( nRC = SetCursorFirstEntity( vTZOPSIGK, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vTZOPSIGK, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vTZOPSIGK, "Operation", zSET_ALL );
            }

            break;
         } // Case 'Z'...

         case 'G':
         {
            zVIEW vOp;

            // Get list of Global operations.

            RetrieveViewForMetaList( vSubtask, &vOp, zREFER_GO_META );

            // Now copy each of the operations to the editor work object.
            for ( nRC = SetCursorFirstEntity( vOp, "W_MetaDef", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vOp, "W_MetaDef", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vOp, "W_MetaDef", zSET_ALL );
            }

            DropView( vOp );

            break;
         } // Case 'G'...

         case 'L':
         {
            zVIEW vMeta;
            zLONG lMetaType;

            // Get list of "Local" operations.  "Local" operations in this
            // case mean all operations that are for the current meta.
            // For example, if the current meta is a Dialog, then the "Local"
            // operations include local and Dialog operations.

            // Get the source view and the meta type.
            GetIntegerFromAttribute( (zPLONG) &vMeta, vEdWrk, "Buffer",
                                     "SourceViewID" );
            GetIntegerFromAttribute( &lMetaType, vEdWrk, "Buffer",
                                     "MetaType" );

            // Create a new, temp view of the meta so that we can muck around
            // with the cursors.
            CreateViewFromViewForTask( &vMeta, vMeta, 0 );

            //
            // Now copy the "Local" meta operations to vEdWrk.
            //

            if ( lMetaType == zREFER_DIALOG_META ||
                 lMetaType == zREFER_LOD_META )
            {
               // Make sure we are reading the "Local" operations for
               // the current source file.
               SetCursorFirstEntityByAttr( vMeta, "SourceFile", "Name",
                                           vEdWrk, "Buffer",
                                           "SourceFileName", 0 );
            }

            // Copy all the operations to vEdWrk
            for ( nRC = SetCursorFirstEntity( vMeta, "Operation", 0 );
                  nRC == zCURSOR_SET;
                  nRC = SetCursorNextEntity( vMeta, "Operation", 0 ) )
            {
               CreateEntity( vEdWrk, "Oper", zPOS_LAST );
               SetMatchingAttributesByName( vEdWrk, "Oper",
                                            vMeta, "Operation",
                                            zSET_ALL );
            }

            DropView( vMeta );

            break;

         } // Case 'L'...

         case 'O':
         {
            // Since we load the object operations by object, we don't have
            // to do anything here.
            break;

         } // Case 'O'...

         default:
            SysMessageBox( "Zeidon Internal",
                           "This operation type not yet supported", 0 );
            break;

      } // switch ( *szCurrentType )...

      // Now that we've loaded the operations, sort them.
      OrderEntityForView( vEdWrk, "Oper", "Name A" );

   } // if ( SetCursor...)...

   // Disable the "Objects" button unless we are showing object operations.
   if ( *szCurrentType == 'O' )
      SetCtrlState( vSubtask, "ListObjects", zCONTROL_STATUS_ENABLED, TRUE );
   else
      SetCtrlState( vSubtask, "ListObjects", zCONTROL_STATUS_ENABLED, FALSE );

   // Now set the select states depending on the keyword values.
   GetStringFromAttribute( szKeyword, zsizeof( szKeyword ), vProfileXFER, "ED", "OpIns_Keyword" );
   if ( *szKeyword &&
        CompareAttributeToString( vProfileXFER,
                                  "ED", "OpIns_UseKeyword", "Y" ) == 0 )
   {
      zCHAR  szIgnoreCase[ 5 ];
      zPCHAR pszKeyword;
      zBOOL  bStartColOne;
      zSHORT nKeywordLth;

      // De-select all operations.
      SetAllSelectStatesForEntity( vEdWrk, "Oper", FALSE, 0 );

      GetStringFromAttribute( szIgnoreCase, zsizeof( szIgnoreCase ), vProfileXFER, "ED",
                              "OpIns_KeywordIgnoreCase" );
      if ( *szIgnoreCase == 'Y' )
      {
         for ( pszKeyword = szKeyWord; *pszKeyword; pszKeyword++ )
            *pszKeyword = ztoupper( *pszKeyword );
      }

      // If the keyword starts with a "/", then the keyword search starts
      // with the first zCHAR (e.g. column) ONLY.
      pszKeyword = szKeyword;
      if ( *pszKeyword == '/' )
      {
         pszKeyword++;
         bStartColOne = TRUE;
         nKeywordLth = zstrlen( pszKeyword );
      }
      else
         bStartColOne = FALSE;

      for ( nRC = SetCursorFirstEntity( vEdWrk, "Oper", 0 );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( vEdWrk, "Oper", 0 ) )
      {
         zCHAR  szOperName[ 255 ];

         GetStringFromAttribute( szOperName, zsizeof( szOperName ), vEdWrk, "Oper", "Name" );
         if ( *szIgnoreCase == 'Y' )
            strupr( szOperName );

         if ( bStartColOne )
         {
            if ( zstrncmp( szOperName, pszKeyword, nKeywordLth ) == 0 )
            {
               SetSelectStateOfEntity( vEdWrk, "Oper", TRUE );
               lSelectCount++;
            }
         }
         else
         {
            if ( strstr( szOperName, pszKeyword ) )
            {
               SetSelectStateOfEntity( vEdWrk, "Oper", TRUE );
               lSelectCount++;
            }
         }
      } // for...
   }
   else
   {
      // Select all operations.
      SetAllSelectStatesForEntity( vEdWrk, "Oper", TRUE, 0 );
      lSelectCount = CountEntitiesForView( vEdWrk, "Oper" );
   }

   //SetSelectSetForView( vEdWrk, nOldSelectSet );
   SetCursorFirstSelectedEntity( vEdWrk, "Oper", 0 );
   LB_SetDisplaySelectSet( vSubtask, "OperationList", DISP_SELECT_SET );
   LB_SetSelectSelectSet( vSubtask, "OperationList", SEL_SELECT_SET );

   // Re-use szKeyword to set the message...
   sprintf( szKeyword, "%d Operations listed", lSelectCount );
   MB_SetMessage( vSubtask, 0, szKeyword );

   RefreshWindow( vSubtask );

   return( 0 );

} // OpIns_BuildOperList

zOPER_EXPORT zSHORT OPERATION
OpIns_RebuildOperList( zVIEW vSubtask )
{
   zCHAR  szCurrentType[ 5 ];
   zVIEW  vProfileXFER;
   zVIEW  vEdWrk;

   GetWorkView( &vEdWrk );
   GetProfileView( &vProfileXFER );
   GetStringFromAttribute( szCurrentType, zsizeof( szCurrentType ), vProfileXFER, "ED",
                           "OpIns_CurrentType" );

   // Default of CurrentType is 'Z'...
   if ( *szCurrentType == 0 )
   {
      strcpy_s( szCurrentType, zsizeof( szCurrentType ), "Z" );
      SetAttributeFromString( vProfileXFER, "ED", "OpIns_CurrentType",
                              szCurrentType );
   }

   // Check to see if the operations for the current type are already
   // loaded. If they are, then delete the list.
   if ( SetCursorFirstEntityByString( vEdWrk, "OperListType", "Type",
                                      szCurrentType, 0 ) == zCURSOR_SET )
   {
      DeleteEntity( vEdWrk, "OperListType", zREPOS_FIRST );
   } // if ( SetCursor...)...

   // Now call BuildOperList to rebuild the oper list.
   OpIns_BuildOperList( vSubtask );

   return( 0 );

} // OpIns_RebuildOperList

zOPER_EXPORT zSHORT OPERATION
OpIns_Postbuild( zVIEW vSubtask )
{
   // Reposition window...
   fnCallEditorOperation( vSubtask, "OpIns_Postbuild" );

   OpIns_BuildOperList( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_Close( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "OpIns_Close" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
OpIns_InsertOperation( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "OpIns_InsertOperation" );
   return( 0 );
}

/*

   Loads a list of Objects (LODs) for the current lplr.

*/
zOPER_EXPORT zSHORT OPERATION
ObjList_Prebuild( zVIEW vSubtask )
{
   zVIEW vObjList;

   GetViewByName( &vObjList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   if ( vObjList )
      DropView( vObjList );

   RetrieveViewForMetaList( vSubtask, &vObjList, zREFER_LOD_META );
   SetNameForView( vObjList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );

   return( 0 );
}

/*

   The user selected a LOD in the Object list.  Load the LOD meta and copy
   all object operations to the editor work object.

*/
zOPER_EXPORT zSHORT OPERATION
ObjList_AddObjOpersToWorkList( zVIEW vSubtask )
{
   zVIEW  vObjList;
   zVIEW  vLOD;
   zVIEW  vEdWrk;
   zSHORT nRC;
   zSHORT nLth;
   zCHAR  szOpName[ 255 ];

   GetWorkView( &vEdWrk );

   GetViewByName( &vObjList, "TZEDCWZD_TZCMLPLO_List", vSubtask, zLEVEL_TASK );
   ActivateMetaOI( vSubtask, &vLOD, vObjList, zREFER_LOD_META, 0 );
   GetStringFromAttribute( szOpName, zsizeof( szOpName ), vObjList, "W_MetaDef", "Name" );
   strcat_s( szOpName, zsizeof( szOpName ), "." );
   nLth = zstrlen( szOpName );

   for ( nRC = SetCursorFirstEntity( vLOD, "OperationList", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vLOD, "OperationList", 0 ) )
   {
      GetStringFromAttribute( szOpName + nLth, zsizeof( szOpName ) - nLth,
                              vLOD, "OperationList", "Name" );

      CreateEntity( vEdWrk, "Oper", zPOS_LAST );
      SetAttributeFromString( vEdWrk, "Oper", "Name", szOpName );
   }

   DropMetaOI( vSubtask, vLOD );
   DropView( vObjList );

   return( 0 );

} // ObjList_AddObjOpersToWorkList

//===========================================================================
// Following structure contains a list of all standard VML insert strings.
// It is used only by VML_BuildTextList
//===========================================================================
typedef struct _VML_TextStringRecord
{
   zPCHAR  pszText;
   zPCHAR  pszType;
   zPCHAR  pszInsertText;
   zPCHAR  pszInsertTextFull;
} VML_TextStringRecord;

VML_TextStringRecord VML_TextStrings[] =
{
   "VIEW var-name BASED ON LOD lod-name", "D",
   "VIEW & BASED ON LOD ",
   "VIEW &var-name BASED ON LOD lod-name",

   "VIEW var-name REGISTERED AS", "D",
   "VIEW & REGISTERED AS ",
   "VIEW &var-name REGISTERED AS regv-name",

   "INTEGER var-name", "D",
   "INTEGER &",
   "INTEGER &var-name",

   "SHORT var-name", "D",
   "SHORT &",
   "SHORT &var-name",

   "STRING ( lth ) var-name", "D",
   "STRING ( & ) ",
   "STRING ( &lth ) var-name",

   "DECIMAL var-name", "D",
   "DECIMAL &",
   "DECIMAL &var-name",

   "CHARACTER var-name", "D",
   "CHARACTER &",
   "CHARACTER &var-name",

   "VIEW var-name", "D",
   "VIEW &",
   "VIEW &var-name",

   "ENTITYNAME var-name", "D",
   "ENTITYNAME &",
   "ENTITYNAME &var-name",

   "ATTRIBUTENAME var-name", "D",
   "ATTRIBUTENAME &",
   "ATTRIBUTENAME &var-name",

   "ACTIVATE view WHERE", "V",
   "ACTIVATE & WHERE ",
   "ACTIVATE &view [ SINGLE | MULTIPLE | ROOTONLY | ROOTONLYMULTIPLE | EMPTY ]\r\n"
   "   [ WHERE Condition [ RESTRICTING Entity TO Condition ] ]",

   "CASE int-var...", "C",
   "CASE &\r\n   OF :\r\nEND",
   "CASE &int-var\r\n   OF int-constant:\r\n   OTHERWISE:\r\nEND",

   "COMMIT view", "V",
   "COMMIT &",
   "COMMIT &view",

   "CREATE ENTITY view.entity", "E",
   "CREATE ENTITY &",
   "CREATE ENTITY &view.entity [ AFTER | BEFORE | FIRST | LAST ]",

   "DELETE ENTITY view.entity", "E",
   "DELETE ENTITY &",
   "DELETE ENTITY &view.entity [ NEXT | PREVIOUS | FIRST | LAST | NONE ]",

   "EXCLUDE view.entity", "E",
   "EXCLUDE &",
   "EXCLUDE &view.entity [ AFTER | BEFORE | NONE ]",

   "FOR view.entity ... END", "E",
   "FOR &\r\nEND",
   "FOR &[ EACH ] [ integer ] view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]\r\n"
   "END",

   "GET VIEW view NAMED view-name", "V",
   "GET VIEW & NAMED \"\"",
   "GET VIEW &view NAMED \"view-name\"\r\n"
   "    [ AT LEVEL { TASK | SUBTASK | APPLICATION | SYSTEM | ANY } ] [ USING Subtask ]",

   "IF condition... END", "C",
   "IF &\r\nEND",
   "IF &condition\r\n[ ELSE ]\r\nEND",

   "IF condition... ELSE...END", "C",
   "IF &\r\nELSE\r\nEND",
   "IF &condition\r\nELSE\r\nEND",

   "INCLUDE target-view.entity FROM source-view.entity", "E",
   "INCLUDE & FROM ",
   "INCLUDE &target-view.entity FROM source-view.entity [ AFTER | BEFORE ]",

   "LOOP ... END", "C",
   "LOOP &\r\nEND",
   "LOOP &[ WHILE condition ]\r\n{ END | UNTIL condition }",

   "LOOP WHILE condition... END", "C",
   "LOOP WHILE &\r\nEND",
   "LOOP WHILE &condition ]\r\nEND",

   "LOOP ... UNTIL condition", "C",
   "LOOP\r\nUNTIL &",
   "LOOP\r\nUNTIL &condition",

   "NAME VIEW view view-name", "V",
   "NAME VIEW & \"\"",
   "NAME VIEW &view \"view-name\"\r\n"
   "   [ AT LEVEL { TASK | SUBTASK | APPLICATION | SYSTEM | ANY } ] [ USING Subtask ]",

   "SET CURSOR view.entity", "E",
   "SET CURSOR &",
   "SET CURSOR &[ FIRST | LAST | NEXT | PREVIOUS ] view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",

   "SET CURSOR FIRST view.entity WHERE", "E",
   "SET CURSOR FIRST &\r\n    WHERE ",
   "SET CURSOR FIRST &view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",

   "SET CURSOR NEXT view.entity WHERE", "E",
   "SET CURSOR NEXT &\r\n    WHERE ",
   "SET CURSOR NEXT &view.entity\r\n"
   "    [ WITHIN scoping-view.entity ]\r\n"
   "    [ WHERE condition ]",

   "=",  "O", "= ",  0,
   "!=", "O", "!= ", 0,
   "<=", "O", "<= ", 0,
   ">=", "O", ">= ", 0,
   "<",  "O", "< ",  0,
   ">",  "O", "> ",  0,
   "+",  "O", "+ ",  0,
   "-",  "O", "- ",  0,
   "*",  "O", "* ",  0,
   "/",  "O", "/ ",  0,
   "(",  "O", "( ",  0,
   ")",  "O", ") ",  0,

   "OR",  "O", "OR ",  0,
   "AND", "O", "AND ",  0,

   "space",     "?", " ",     0,
   "backspace", "?", "%ml1",  0,
   "newline",   "?", "\n&",   0,

   // Following indicates the end of the list:
   0, 0, 0, 0
};

//===========================================================================

zOPER_EXPORT zSHORT OPERATION
VML_DisplayList( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zCHAR  szShowStr[ 10 ];
   zSHORT nRC;

   GetProfileView( &vProfileXFER );
   *szShowStr = 0;

   // Build a string that contains each of the VML types that the user
   // wants shown.

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowDeclarations", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "D" );
   }

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowControlStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "C" );
   }

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowViewStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "V" );
   }

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowEntityStatements", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "E" );
   }

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowOperators", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "O" );
   }

   if ( CompareAttributeToString( vProfileXFER, "ED",
                                  "VML_ShowOther", "Y" ) == 0 )
   {
      strcat_s( szShowStr, zsizeof( szShowStr ), "?" );
   }

   // szShowStr lists all the types that the user wants shown. eg "CV?" means
   // that the user wants all control, view, and 'other' vml statements
   // shown.
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      zCHAR szType[ 5 ];

      GetStringFromAttribute( szType, zsizeof( szType ), vProfileXFER, "VML_Text", "Type" );
      if ( zstrchr( szShowStr, *szType ) )
         SetSelectStateOfEntity( vProfileXFER, "VML_Text", TRUE );
      else
         SetSelectStateOfEntity( vProfileXFER, "VML_Text", FALSE );
   }

   SetCursorFirstSelectedEntity( vProfileXFER, "VML_Text", 0 );
   LB_SetDisplaySelectSet( vSubtask, "TextList", DISP_SELECT_SET );
   LB_SetSelectSelectSet( vSubtask, "TextList", SEL_SELECT_SET );

   return( 0 );

} // VML_DisplayList

zOPER_EXPORT zSHORT OPERATION
VML_BuildTextList( zVIEW vSubtask )
{
   zSHORT i;
   zVIEW  vProfileXFER;

   GetProfileView( &vProfileXFER );

   // Add each of the 'standard' VML statements to the text list.
   for ( i = 0; VML_TextStrings[ i ].pszText != 0; i++ )
   {
      if ( SetCursorFirstEntityByString( vProfileXFER, "VML_Text", "Text",
                                         VML_TextStrings[ i ].pszText, 0 )
                                                              != zCURSOR_SET )
      {
         // Text doesn't exist, so create it.
         CreateEntity( vProfileXFER, "VML_Text", zPOS_LAST );

         SetAttributeFromString( vProfileXFER, "VML_Text", "Text",
                                 VML_TextStrings[ i ].pszText );
         SetAttributeFromString( vProfileXFER, "VML_Text", "Type",
                                 VML_TextStrings[ i ].pszType );
         SetAttributeFromString( vProfileXFER, "VML_Text", "UserDefined", "N" );

         if ( VML_TextStrings[ i ].pszInsertText )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertText",
                                    VML_TextStrings[ i ].pszInsertText );

         if ( VML_TextStrings[ i ].pszInsertTextFull )
            SetAttributeFromString( vProfileXFER, "VML_Text", "InsertTextFull",
                                    VML_TextStrings[ i ].pszInsertTextFull );
      }
   }

   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );
   VML_DisplayList( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_Postbuild( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   // Call postbuild routine in ZEDITOR to reposition the window.
   fnCallEditorOperation( vSubtask, "VML_Postbuild" );

   GetProfileView( &vProfileXFER );

   // Set the default values of the Show... attributes.
   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowDeclarations", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowDeclarations", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowControlStatements", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowControlStatements", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowViewStatements", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowViewStatements", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowEntityStatements", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowEntityStatements", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowOperators", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowOperators", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_ShowOther", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_ShowOther", "Y" );
   }

   if ( CompareAttributeToString( vProfileXFER,
                                  "ED", "VML_InsertFull", "" ) == 0 )
   {
      SetAttributeFromString( vProfileXFER, "ED", "VML_InsertFull", "Y" );
   }

   VML_DisplayList( vSubtask );

   return( 0 );

} // VML_BuildTextList

/*

   Insert the currently selected text.

*/
zOPER_EXPORT zSHORT OPERATION
VML_InsertText( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "VML_InsertText" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_ShowPopupMenu( zVIEW vSubtask )
{
   POINT  pt;

   pt.x = -1;
   CreateTrackingPopupMenu( vSubtask, "Popup", pt.x, pt.y, FALSE, FALSE );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
VML_CreateText_Exit( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;

   GetProfileView( &vProfileXFER );
   OrderEntityForView( vProfileXFER, "VML_Text", "Text A" );
   VML_DisplayList( vSubtask );

   return( 0 );
} // VML_CreateText_Exit

zOPER_EXPORT zSHORT OPERATION
VML_CreateText_DeleteAll( zVIEW vSubtask )
{
   zVIEW  vProfileXFER;
   zSHORT nRC;

   if ( MessagePrompt( vSubtask, "ED0101", "Zeidon Editor",
                       "Delete All VML Keywords?", 0, zBUTTONS_YESNO,
                       zRESPONSE_NO, zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( 0 );
   }

   GetProfileView( &vProfileXFER );
   for ( nRC = SetCursorFirstEntity( vProfileXFER, "VML_Text", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( vProfileXFER, "VML_Text", 0 ) )
   {
      DeleteEntity( vProfileXFER, "VML_Text", zREPOS_NONE );
   }

   return( 0 );
} // VML_CreateText_DeleteAll

zOPER_EXPORT zSHORT OPERATION
VML_Close( zVIEW vSubtask )
{
   fnCallEditorOperation( vSubtask, "VML_Close" );
   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
OpIns_StartOperationHelp( zVIEW vSubtask )
{

   zVIEW  vEdWrk;
   zCHAR  szOperName[40];
   zCHAR  szOperType[ 5 ];
   zSHORT nRC;

   // FIND OUT WHAT TYPE OF OPERATION LIST IS CURRENTLY DISPLAYED.
   GetWorkView( &vEdWrk );
   GetStringFromAttribute( szOperType, zsizeof( szOperType ), vEdWrk, "OperListType", "Type" );

   // IF IT'S NOT A ZEIDON OPER LIST, GET OUT. NO HELP IS AVAILABLE.
   if ( szOperType[ 0 ] != 'Z' )
   {
      return( 0 );
   }

   // GET THE OPERATION TAG FOR THE CURRENT CURSOR POSITION.
   GetStringFromAttribute( szOperName, zsizeof( szOperName ), vEdWrk, "Oper", "Name" );

   // CALL HELP
   nRC = StartZeidonHelp( vSubtask, szOperName );

   // IF HELP WAS NOT FOUND, NOTIFY THE USER.
   if ( nRC == zCALL_ERROR )
   {
      MB_SetMessage( vSubtask, 0, "Help for this operation is not currently available." );
   }

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
VML_SelectItem( zVIEW vSubtask )
{
   MB_SetMessage( vSubtask, 0, "Press F1 for Help on this item." );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
VML_StartHelp( zVIEW vSubtask )
{

   zCHAR  szBuffer[60];
   zPCHAR pszText;

   zSHORT nRC;
   zSHORT i;
   zSHORT j;

   zVIEW  vProfileXFER;


   // GET ADDRESS OF THE STRING FOR THE TEXT IN THE LIST BOX
   GetProfileView( &vProfileXFER );
   GetAddrForAttribute( &pszText,       /* Return Address */
                        vProfileXFER,  /* View Id */
                        "VML_Text",    /* Entity name */
                        "Text" /* Attribute Name */ );

   // EXTRACT FIRST ALL CAP WORD FROM pszText
   i = 0;
   j = 0;

   // look for first cap
   while ( !isupper( pszText[ i ] ) &&  /* not upper case */
           pszText[ i ] != 0 )          /* not null terminator */
   {
      i++;
   }

   // if no cap is found, no help is available. get out now.
   if ( pszText[ i ] == 0)
   {
      MB_SetMessage( vSubtask, 0, "Help for this operation is not currently available." );
      return( 0 );
   }

   // copy first all cap word into the buffer
   while ( isupper( pszText[ i ] )  &&      /* upper case */
           pszText[ i ] != 0 &&             /* not null terminator */
           j < 59 )                         /* room left in buffer */
   {
      szBuffer[ j++ ] = pszText[ i++ ];
   }
   szBuffer[j] = 0;

   // CALL HELP
   nRC = StartZeidonHelp( vSubtask, szBuffer );

   // IF HELP WAS NOT FOUND, NOTIFY THE USER.
   if ( nRC == zCALL_ERROR )
   {
      MB_SetMessage( vSubtask, 0, "Help for this item is not available." );
   }

   return( 0 );
}
