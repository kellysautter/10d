/////////////////////////////////////////////////////////////////////////////
//
// File:            TZBMUPDD.C
// Copyright:       Ton Beller AG2001
// Autor:           BL
// Datum:           03. March 2001
// Describtion:     Replace absolute Bitmap Path through relative Path for all
//                  selected or all Dialogs within a LPLR
//
//
// LAST MESSAGE ID: BM00003

/* Change log most recent first order:

2001.03.15   BL
   Remove warnings

2001.03.15   BL
   Search Path case insensitive

2001.03.03   BL
   Created


*/
/////////////////////////////////////////////////////////////////////////////

#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"


#define  zSOURCE_REPLACE_BMP    -1

#define  zControl_Bitmap        2080
#define  zControl_BitmapButton  2085
#define  zControl_Icon          2060


zOPER_EXPORT zLONG OPERATION
zGetStringWithoutBlank( zCPCHAR  szSourceString,
                        zPCHAR   pszTargetString );
zOPER_EXPORT zSHORT OPERATION
zToUpper( zCPCHAR szSourceString,
          zPCHAR  pszTargetString );


zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZERRORD_CreateNewErrorRoot( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZERRORD_WriteErrorMsg( zVIEW  vSubtask,
                          zVIEW  vMetaView,
                          zPCHAR szMetaEntity,
                          zPCHAR szMetaAttribute,
                          zPCHAR szErrorMsg,
                          zSHORT nError );

static zSHORT
zwfnTZBMUPDD_IsDialogOpenForUpdate( zVIEW vSubtask, zVIEW vMetaList );
static zSHORT
zwfnTZBMUPDD_ReplaceBitmapPath( zVIEW vDialog, zPCHAR szOldPath, zPCHAR szNewPath );
static zSHORT
zwfnReplacePathRecursive( zVIEW vDialog, zPCHAR szOldPath, zPCHAR szNewPath );
static zBOOL
zwfnSearch_Replace( zVIEW vDialog, zPCHAR szCurrentPath,
                    zPCHAR szOldPath, zPCHAR szNewPath );
static void
zwfnTZBMUPDD_WriteBitmapPath( zVIEW  vSubtask, zPCHAR szDialog, zPCHAR szWindow,
                              zPCHAR szControl, zPCHAR szBitmapPath, zSHORT nError );
static void
zwfnTZBMUPDD_WritePathToList( zVIEW vDialog, zPCHAR szDialog, zPCHAR szWindow,
                              zCTRLBOI_BMP BMP, zULONG ControlDefKey );
static zSHORT
zwfnSearchPathRecursive( zVIEW vDialog, zPCHAR szDialogName, zPCHAR szWindowName );
static void
zwfnTZERRORD_WriteErrorListToFile( zVIEW vTZBMPATH, zLONG  FileHandle );


#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_ReturnFromSubWnd
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_ReturnFromSubWnd( zVIEW vSubtask )
{
   zVIEW  vCM_List  = NULL;
   zVIEW  vTaskLPLR = NULL;
   zCHAR  szLPL_Name[ 33 ];
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 1 )
   {
      if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_DIALOG_META ) > 0 )
         SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
   }

   // Set LPLR Name in Title
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szLPL_Name, sizeof( szLPL_Name ), vTaskLPLR, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, szLPL_Name, "Bitmap Maintenance" );
   }

   // Load Bitmap mit Resource Path
   SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, sizeof( szZeidonPath ) );
   SetCtrlText( vSubtask, "txtResourcePath", szZeidonPath );
   SetCtrlText( vSubtask, "edNewWithResourcePath", szZeidonPath );

   return( 0 );
} // zwTZBMUPDD_ReturnFromSubWnd

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_PostBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_PostBuild( zVIEW vSubtask )
{
   if ( InitializeDefaultLPL( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   zwTZBMUPDD_ReturnFromSubWnd( vSubtask );

   ED_LimitTextLength( vSubtask, "edOld", zMAX_FILENAME_LTH );
   ED_LimitTextLength( vSubtask, "edNew", zMAX_FILENAME_LTH );

   return( 0 );
} // zwTZBMUPDD_PostBuild

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_SwitchLPLR
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zVIEW vView;
   zVIEW vWindow;

   // Close Error List and Bitmap List
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "BitmapPath" ) >= 0 )
      InvokeWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZERRORD" ) >= 0 )
      InvokeWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );

   // Drop all Views
   if ( GetViewByName( &vView, "TZBMPATH", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZERROR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vView );

   return( 0 );
} // zwTZBMUPDD_SwitchLPLR



/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_ExitDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_ExitDialog( zVIEW vSubtask )
{
   zVIEW vView;

   if ( GetViewByName( &vView, "TZBMPATH", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "TZERROR", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vView );

   if ( GetViewByName( &vView, "CM_List", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vView );

   TerminateLPLR( vSubtask );

   return( 0 );
} // zwTZBMUPDD_ExitDialog

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_SetOldBitmapFileName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_SetOldBitmapFileName( zVIEW vSubtask )
{
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 40 ];

   // set Control Text for Old Bitmap
   GetCtrlText( vSubtask, "edOld", szFileName, zMAX_FILENAME_LTH );

   SysAppendcDirSep( szFileName );
   strcat_s( szFileName, sizeof( szFileName ), "Bitmap.bmp" );

   SetCtrlText( vSubtask, "txtOld", szFileName );

   return( 0 );
} // zwTZBMUPDD_SetOldBitmapFileName

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_SetNewBitmapFileName
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_SetNewBitmapFileName( zVIEW vSubtask )
{
   zCHAR  szZeidonPath[ zMAX_FILENAME_LTH + zMAX_FILENAME_LTH ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH ];

   // set Control Text for New Bitmap
   GetCtrlText( vSubtask, "edNew", szFileName, zMAX_FILENAME_LTH );

   strcpy_s( szZeidonPath, sizeof( szZeidonPath ), szFileName );
   SysAppendcDirSep( szZeidonPath );
   strcat_s( szZeidonPath, sizeof( szZeidonPath ), "Bitmap.bmp" );

   SetCtrlText( vSubtask, "txtNew", szZeidonPath );

   // set Control Text for New Bitmap + Resource Path
   GetCtrlText( vSubtask, "txtResourcePath", szZeidonPath, zMAX_FILENAME_LTH );

   if ( szFileName[ 0 ] != '\\' )
      SysAppendcDirSep( szZeidonPath );
   else
   if ( szZeidonPath[zstrlen( szZeidonPath ) - 1] == '\\' )
      szZeidonPath[zstrlen( szZeidonPath ) - 1] = 0;

   strcat_s( szZeidonPath, sizeof( szZeidonPath ), szFileName );
   SysAppendcDirSep( szZeidonPath );
   strcat_s( szZeidonPath, sizeof( szZeidonPath ), "Bitmap.bmp" );

   SetCtrlText( vSubtask, "edNewWithResourcePath", szZeidonPath );

   return( 0 );
} // zwTZBMUPDD_SetNewBitmapFileName

/////////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_CheckOldPath
//
//////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZBMUPDD_CheckOldPath( zVIEW vSubtask )
{
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 40 ];

   // check Old Bitmap Path
   GetCtrlText( vSubtask, "edOld", szFileName, zMAX_FILENAME_LTH );
   zGetStringWithoutBlank( szFileName, szFileName );
   if ( zstrcmp( szFileName, "") == 0 )
   {
      MessageSend( vSubtask, "BM00002", "Bitmap Maintenance",
                   "Old Path is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edOld" );
      return( -1 );
   }

   return( 0 );
} // zwTZBMUPDD_CheckOldPath

static zSHORT
zwfnTZBMUPDD_IsDialogOpenForUpdate( zVIEW vSubtask, zVIEW vMetaList )
{
   zVIEW  vMetaListC;
   zVIEW  vZeidonCM;
   zVIEW  vTaskMetas;
   zVIEW  vActiveMetas;
   zVIEW  vTaskLPLR;
   zLONG  lMetaOI_ZKey;
   zLONG  lTaskID;
   zCHAR  szTaskID[ 18 ];
   zCHAR  szMetaOI_Name[ 33 ];

   // Get the view to ZedionCM OI.
   CreateViewFromViewForTask( &vMetaListC, vMetaList, 0 );
   GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );

   // Get the view to the vTaskMetas subtask
// lTaskID = SysGetTaskFromView( vMetaList );
// zltoa( lTaskID, szTaskID, sizeof( szTaskID ) );
   lTaskID = SysGetTaskFromView( vSubtask );
   sprintf_s( szTaskID, sizeof( szTaskID ), "%08x", lTaskID );
   strcpy_s( szMetaOI_Name, sizeof( szMetaOI_Name ), "__CM." );
   strcpy_s( szMetaOI_Name + 5, sizeof( szMetaOI_Name ) - 5, szTaskID );

   GetViewByName( &vTaskMetas, szMetaOI_Name, vZeidonCM, zLEVEL_SUBTASK );

   // Get the view to the ActiveMeta OI
   GetViewByName( &vActiveMetas, "OpenCM_Metas", vZeidonCM, zLEVEL_SUBTASK );

   // I need the view to the TaskLPLR to get the name of the root LPLR
   // in the ActiveMeta OI.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstEntityByAttr( vActiveMetas, "LPLR", "Name",
                                    vTaskLPLR, "LPLR",
                                    "Name", "" ) >= zCURSOR_SET &&
        SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaType", "Type",
                                       zSOURCE_DIALOG_META,
                                       "" ) >= zCURSOR_SET )
   {
      // Check in the Active Meta List for a hit on the requested Meta
      GetIntegerFromAttribute( &lMetaOI_ZKey, vMetaListC, "W_MetaDef", "CPLR_ZKey" );

      if ( SetCursorFirstEntityByInteger( vActiveMetas, "W_MetaDef", "CPLR_ZKey",
                                          lMetaOI_ZKey, "" ) == zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lTaskID, vActiveMetas, "W_MetaDef", "TaskID" );
         if ( lTaskID )
         {
         // if ( lTaskID != SysGetTaskFromView( vMetaList ) )
            if ( lTaskID != SysGetTaskFromView( vSubtask ) )
            {
               DropView( vMetaListC );
               return( 1 );
            }
         }
      }
   }

   DropView( vMetaListC );
   return( 0 );
}

static zBOOL
zwfnSearch_Replace( zVIEW vSubtask, zPCHAR szOrigPath,
                    zPCHAR szSearchPath, zPCHAR szReplacePath )
{
   zCHAR   *pszPos = szOrigPath;
   zCHAR   szMsg[ 254 ];
   zCHAR   szTemp[ 33 ];
   zULONG  ulOffs, ulOrigLen, ulSearchLen, ulReplaceLen;
   zULONG  ulTargetLen = zMAX_FILENAME_LTH;
   zLONG   lLenDiff;
   zVIEW   vCM_List;

   ulOrigLen    = strlen( szOrigPath );
   ulSearchLen  = strlen( szSearchPath );
   ulReplaceLen = strlen( szReplacePath );
   lLenDiff     = ulReplaceLen - ulSearchLen;

   zToUpper( szSearchPath, szSearchPath );
   zToUpper( szOrigPath, szOrigPath );

   pszPos = strstr( szOrigPath, szSearchPath );

   if ( pszPos == NULL )   // Old Path not found
      return( FALSE );

   ulOffs = (zULONG) (pszPos - szOrigPath);

   if ( ulOrigLen + lLenDiff < ulTargetLen )
   {
      memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
               ulOrigLen - ulOffs - ulSearchLen + 1 );
      memcpy( pszPos, szReplacePath, ulReplaceLen );
      ulOrigLen += lLenDiff;
   }
   else
   {
      // Target ist nicht gross genug, ein Teil wird hinten abgeschnitten
      if ( ulOffs + ulReplaceLen < ulTargetLen )
      {
         // Der Replace String passt noch komplett in Target
         memmove( pszPos + ulReplaceLen, pszPos + ulSearchLen,
                  ulTargetLen - ulOffs - ulReplaceLen - 1 );
         memcpy( pszPos, szReplacePath, ulReplaceLen );
      }
      else
      {
         // Target is to small, send Error Message
         if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) >= 0 )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Path replacement for Window '" );
            GetStringFromAttribute( szTemp, sizeof( szTemp ), vSubtask, "Window", "Tag" );
            strcat_s( szMsg, sizeof( szMsg ), szTemp );
            strcat_s( szMsg, sizeof( szMsg ), "', Control '" );
            GetStringFromAttribute( szTemp, sizeof( szTemp ), vSubtask, "Control", "Tag" );
            strcat_s( szMsg, sizeof( szMsg ), szTemp );
            strcat_s( szMsg, sizeof( szMsg ), "' failed. New Path longer than 256 characters. " );
            zwTZERRORD_WriteErrorMsg( vSubtask, vCM_List, "W_MetaDef", "Name", szMsg, 1 );
            return( FALSE );
         }
      }
      szOrigPath[ ulTargetLen ] = 0;
   }

   return( TRUE );
}


static zSHORT
zwfnReplacePathRecursive( zVIEW vDialog, zPCHAR szOldPath, zPCHAR szNewPath )
{
   zSHORT       nRC;
   zULONG       ulKey;
   zULONG       ulLth;
   zPCHAR       pCtrlBOI;
   zCHAR        szBlob[ 4 * zMAX_FILENAME_LTH + 40 ];
   zLONG        k;
   zBOOL        bUp, bDown, bFocus, bDisable;
   zCTRLBOI_BMP BMP;

   nRC = CheckExistenceOfEntity( vDialog, "Control" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( CheckExistenceOfEntity( vDialog, "ControlDef" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &ulKey, vDialog, "ControlDef", "Key" );
         if ( ulKey == zControl_Bitmap || ulKey == zControl_BitmapButton ||
              ulKey == zControl_Icon )
         {
            GetAttributeLength( &ulLth, vDialog, "Control", "CtrlBOI" );
            if ( ulLth )
            {
               GetAddrForAttribute( &pCtrlBOI, vDialog, "Control", "CtrlBOI" );
               SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );
               bUp = zwfnSearch_Replace( vDialog, BMP.szBMPUpOrIconName,
                                         szOldPath, szNewPath );

               // if BitmapButton
               if ( ulKey == zControl_BitmapButton )
               {
                  bDown = zwfnSearch_Replace( vDialog, BMP.szBMPDownName,
                                              szOldPath, szNewPath );
                  bFocus = zwfnSearch_Replace( vDialog, BMP.szBMPFocusName,
                                               szOldPath, szNewPath );
                  bDisable = zwfnSearch_Replace( vDialog, BMP.szBMPDisabledName,
                                                 szOldPath, szNewPath );
               }
               else
               {
                  bDown = bFocus = bDisable = FALSE;
               }

               if ( bUp || bDown || bFocus || bDisable )
               {
                  k = BuildCtrlBOI_BMP( &BMP, szBlob, sizeof( szBlob ) );
                  SetAttributeFromBlob( vDialog, "Control", "CtrlBOI", szBlob, k );
               }
            } // endif ( ulLth )
         }

         if ( SetViewToSubobject( vDialog, "CtrlCtrl" ) >= 0 )
         {
            zwfnReplacePathRecursive( vDialog, szOldPath, szNewPath );
            ResetViewFromSubobject( vDialog );
         }
      }

      nRC = SetCursorNextEntity( vDialog, "Control", 0 );
   }

   return( 0 );
}

static zSHORT
zwfnTZBMUPDD_ReplaceBitmapPath( zVIEW vDialog, zPCHAR szOldPath, zPCHAR szNewPath )
{
   zSHORT  nRC;

   for ( nRC = SetCursorFirstEntity( vDialog, "Window", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, "Window", 0 ) )
   {
      if ( SetCursorFirstEntity( vDialog, "Control", 0 ) >= zCURSOR_SET )
         zwfnReplacePathRecursive( vDialog, szOldPath, szNewPath );
   }

   ResetView( vDialog );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_ReplaceBitmapPath
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZBMUPDD_ReplaceBitmapPath( zVIEW vSubtask, zVIEW vMetaList, zPCHAR szOldPath,
                              zPCHAR szNewPath )
{
   zVIEW   vDialog;
   zCHAR   szDialogName[ 33 ];
   zCHAR   szMsg[ 100 ];

   GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vMetaList, "W_MetaDef", "Name" );
   strcpy_s( szMsg, sizeof( szMsg ), "Replace Bitmap Path in Dialog: '" );
   strcat_s( szMsg, sizeof( szMsg ), szDialogName );
   strcat_s( szMsg, sizeof( szMsg ), "'." );
   MB_SetMessage( vSubtask, 1, szMsg );

   // if Dialog not checked out
   if ( CompareAttributeToInteger( vMetaList, "W_MetaDef", "Status", 1 ) != 0 )
   {
      zwTZERRORD_WriteErrorMsg( vSubtask, vMetaList, "W_MetaDef", "Name",
                                "Dialog is not checked out.", 1 );
   }
   else
   // if Dialog open for update
   if ( zwfnTZBMUPDD_IsDialogOpenForUpdate( vSubtask, vMetaList ) == 1 )
   {
      zwTZERRORD_WriteErrorMsg( vSubtask, vMetaList, "W_MetaDef", "Name",
                                "Dialog is already opened for update by "
                                "another task. Unable to open.", 1 );
   }
   else
   // if Activate with Errors
   if ( ActivateMetaOI( vSubtask, &vDialog, vMetaList,
                        zSOURCE_DIALOG_META, zSINGLE | zLEVEL_TASK ) != 1 )
   {
      zwTZERRORD_WriteErrorMsg( vSubtask, vMetaList, "W_MetaDef", "Name",
                                "Could not activate Dialog.", 1 );
   }
   else
   {
      // replace Bitmap Path
      zwfnTZBMUPDD_ReplaceBitmapPath( vDialog, szOldPath, szNewPath );

      if ( CommitMetaOI( vSubtask, vDialog, zSOURCE_DIALOG_META ) < 0 )
      {
         zwTZERRORD_WriteErrorMsg( vSubtask, vMetaList, "W_MetaDef", "Name",
                                   "Unable to save Dialog.", 1 );
      }

      DropMetaOI( vSubtask, vDialog );
   }

    return( 0 );
} // zwTZBMUPDD_ReplaceBitmapPath

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_LoadErrorList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_LoadErrorList( zVIEW vSubtask )
{
   zVIEW vTZERROR;
   zVIEW vTaskLPLR;
   zVIEW vWindow;

   if ( GetViewByName( &vTZERROR, "TZERROR", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   SetAttributeFromInteger( vTZERROR, "ErrorList", "MetaType", zSOURCE_REPLACE_BMP );

   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      SetAttributeFromAttribute( vTZERROR, "ErrorList", "LPLR_Name",
                                 vTaskLPLR, "LPLR", "Name" );
   }

   SetWindowActionBehavior( vSubtask, zWAB_ProcessImmediateAction |
                            zWAB_StartOrFocusModelessSubwindow,
                            "TZERRORD", "TZERRORD" );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "TZERRORD" ) >= 0 )
      RefreshWindow( vSubtask );

   return( 0 );
} // zwTZBMUPDD_LoadErrorList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_ReplaceSelectedDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_ReplaceSelectedDialog( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vTZERROR;
   zCHAR  szOldPath[ zMAX_FILENAME_LTH ];
   zCHAR  szNewPath[ zMAX_FILENAME_LTH ];
   zCHAR  szMsg[ 200 ];
   zSHORT nRC;

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( SetCursorFirstSelectedEntity( vCM_List, "W_MetaDef", "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "BM00001", "Bitmap Maintenance", "No Dialog selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( -1 );
   }

   // check Old Bitmap Path
   if ( zwTZBMUPDD_CheckOldPath( vSubtask ) < 0 )
      return( -1 );

   // send Message Box for continue
   strcpy_s( szMsg, sizeof( szMsg ), "All Bitmap Paths for selected Dialogs will be changed." );
   strcat_s( szMsg, sizeof( szMsg ), "\n\nDo you want to continue?" );
   if ( MessagePrompt( vSubtask, "BM00003", "Bitmap Maintenance",
                       szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                       zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( -1 );
   }

   // Create Error List
   zwTZERRORD_CreateNewErrorRoot( vSubtask );

   // get Bitmap Paths
   GetCtrlText( vSubtask, "edOld", szOldPath, zMAX_FILENAME_LTH );
   GetCtrlText( vSubtask, "edNew", szNewPath, zMAX_FILENAME_LTH );

   // Create Error List for Replace Bitmap Path
   for ( nRC = SetCursorFirstSelectedEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vCM_List, "W_MetaDef", 0 ) )
   {
      zwTZBMUPDD_ReplaceBitmapPath( vSubtask, vCM_List, szOldPath, szNewPath );
   }

   if ( GetViewByName( &vTZERROR, "TZERROR", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZERROR, "ErrorMsg" ) >= zCURSOR_SET )
   {
      zwTZBMUPDD_LoadErrorList( vSubtask );
   }

   MB_SetMessage( vSubtask, 1, "Replace completed." );

   return( 0 );
} // zwTZBMUPDD_ReplaceSelectedDialog

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_ReplaceAllDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_ReplaceAllDialog( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vTZERROR;
   zCHAR  szOldPath[ zMAX_FILENAME_LTH ];
   zCHAR  szNewPath[ zMAX_FILENAME_LTH ];
   zCHAR  szMsg[ 200 ];
   zSHORT nRC;

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   // check Old Bitmap Path
   if ( zwTZBMUPDD_CheckOldPath( vSubtask ) < 0 )
      return( -1 );

   // send Message Box for continue
   strcpy_s( szMsg, sizeof( szMsg ), "All Bitmap Paths for all Dialogs will be changed." );
   strcat_s( szMsg, sizeof( szMsg ), "\n\nDo you want to continue?" );
   if ( MessagePrompt( vSubtask, "BM00003", "Bitmap Maintenance",
                       szMsg, zBEEP, zBUTTONS_YESNO, zRESPONSE_NO,
                       zICON_QUESTION ) == zRESPONSE_NO )
   {
      return( -1 );
   }

   // Create Error List for Replace Bitmap Path
   zwTZERRORD_CreateNewErrorRoot( vSubtask );

   // get Bitmap Paths
   GetCtrlText( vSubtask, "edOld", szOldPath, zMAX_FILENAME_LTH );
   GetCtrlText( vSubtask, "edNew", szNewPath, zMAX_FILENAME_LTH );

   // replace Bitmap Path in all Dialogs
   for ( nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vCM_List, "W_MetaDef", 0 ) )
   {
      zwTZBMUPDD_ReplaceBitmapPath( vSubtask, vCM_List, szOldPath, szNewPath );
   }

   if ( GetViewByName( &vTZERROR, "TZERROR", vSubtask, zLEVEL_TASK ) >= 0 &&
        CheckExistenceOfEntity( vTZERROR, "ErrorMsg" ) >= zCURSOR_SET )
   {
      zwTZBMUPDD_LoadErrorList( vSubtask );
   }

   MB_SetMessage( vSubtask, 1, "Replace completed." );

   return( 0 );
} // zwTZBMUPDD_ReplaceAllDialog

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_SetCountError
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_SetCountError( zVIEW vSubtask )
{
   zVIEW     vTZBMPATH;
   zLONG     lCountError;
   zLONG     lCountPath;
   zSHORT    nRC;
   zCHAR     szCount[ 10 ];
   zCHAR     szControlText[ 100 ];

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vTZBMPATH, "BitmapPath" ) >= zCURSOR_SET )
   {
      // Set Count Errors to Textfield
      lCountError = 0;
      lCountPath  = 0;

      for ( nRC = SetCursorFirstEntity( vTZBMPATH, "BitmapPath", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZBMPATH, "BitmapPath", "" ) )
      {
         if ( CompareAttributeToInteger( vTZBMPATH, "BitmapPath", "Error", 1 ) == 0 )
            lCountError++;
         else
            lCountPath++;
      }

      strcpy_s( szControlText, sizeof( szControlText ), " " );

      if ( lCountError > 0 )
      {
         zltoa( lCountError, szCount, sizeof( szCount ) );
         strcat_s( szControlText, sizeof( szControlText ), szCount );
         strcat_s( szControlText, sizeof( szControlText ), " Error(s) " );
         strcat_s( szControlText, sizeof( szControlText ), ", " );
      }

      if ( lCountPath > 0 )
      {
         zltoa( lCountPath, szCount, sizeof( szCount ) );
         strcat_s( szControlText, sizeof( szControlText ), szCount );
         strcat_s( szControlText, sizeof( szControlText ), " Bitmap Path(s) " );
      }
      else
      {
         strcat_s( szControlText, sizeof( szControlText ), " No Bitmap Paths " );
      }
   }
   else
   {
      strcpy_s( szControlText, sizeof( szControlText ), " No Bitmap Paths " );
   }

   strcat_s( szControlText, sizeof( szControlText ), "found" );
   SetCtrlText( vSubtask, "txtCountError", szControlText );

   return( 0 );
} // zwTZBMUPDD_SetCountError

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_CreateNewBitmapRoot
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZBMUPDD_CreateNewBitmapRoot( zVIEW vSubtask, zSHORT nForAllDialogs )
{
   zVIEW  vTZBMPATH;
   zCHAR  szCurrentDateTime[ 20 ];
   zCHAR  szTitle[ 100 ];

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
   {
      // Create Work LOD for Error List
      ActivateEmptyObjectInstance( &vTZBMPATH, "TZBMPATH", vSubtask, zMULTIPLE );
      SetNameForView( vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK );
   }

  SysGetDateTime( szCurrentDateTime, sizeof( szCurrentDateTime ) );

   szTitle[ 0 ] = 0;
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime + 6, 2 );
   strcat_s( szTitle, sizeof( szTitle ), "." );
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime + 4, 2 );
   strcat_s(  szTitle, sizeof( szTitle ), "." );
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime, 4 );
   strcat_s(  szTitle, sizeof( szTitle ), "  " );
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime + 8, 2 );
   strcat_s(  szTitle, sizeof( szTitle ), ":" );
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime + 10, 2 );
   strcat_s(  szTitle, sizeof( szTitle ), ":" );
   strncat_s( szTitle, sizeof( szTitle ), szCurrentDateTime + 12, 2 );

   if ( nForAllDialogs )
      strcat_s( szTitle, sizeof( szTitle ), " for all Dialogs" );
   else
      strcat_s( szTitle, sizeof( szTitle ), " for selected Dialogs" );

   CreateEntity( vTZBMPATH, "PathList", zPOS_LAST );
   SetAttributeFromString( vTZBMPATH, "PathList", "DateTimeAndDialogs", szTitle );

   return( 0 );
} // zwTZBMUPDD_CreateNewBitmapRoot

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_LoadBitmapList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_LoadBitmapList( zVIEW vSubtask )
{
   zVIEW  vTZBMPATH;

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZBMUPDD_CreateNewBitmapRoot( vSubtask, TRUE );
   }

   zwTZBMUPDD_SetCountError( vSubtask );

   return( 0 );
} // zwTZBMUPDD_LoadBitmapList

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_RefreshBitmapList
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_RefreshBitmapList( zVIEW vSubtask )
{
   zwTZBMUPDD_SetCountError( vSubtask );
   RefreshWindow( vSubtask );

    return( 0 );
} // zwTZBMUPDD_RefreshBitmapList

static void
zwfnTZBMUPDD_WritePathToList( zVIEW vDialog, zPCHAR szDialog, zPCHAR szWindow,
                              zCTRLBOI_BMP BMP, zULONG ulControlDefKey )
{
   zCHAR szControl[ 33 ];

   GetStringFromAttribute( szControl, sizeof( szControl ), vDialog, "Control", "Tag" );

   if ( BMP.szBMPUpOrIconName[ 0 ] )
   {
      zwfnTZBMUPDD_WriteBitmapPath( vDialog, szDialog, szWindow, szControl,
                                    BMP.szBMPUpOrIconName, 0 );
   }

   // if BitmapButton
   if ( ulControlDefKey == zControl_BitmapButton )
   {
      if ( BMP.szBMPDownName[ 0 ] )
      {
         zwfnTZBMUPDD_WriteBitmapPath( vDialog, szDialog, szWindow, szControl,
                                       BMP.szBMPDownName, 0 );
      }

      if ( BMP.szBMPFocusName[ 0 ] )
      {
         zwfnTZBMUPDD_WriteBitmapPath( vDialog, szDialog, szWindow, szControl,
                                       BMP.szBMPFocusName, 0 );
      }

      if ( BMP.szBMPDisabledName[ 0 ] )
      {
         zwfnTZBMUPDD_WriteBitmapPath( vDialog, szDialog, szWindow, szControl,
                                       BMP.szBMPDisabledName, 0 );
      }
   }
}

static zSHORT
zwfnSearchPathRecursive( zVIEW vDialog, zPCHAR szDialogName, zPCHAR szWindowName )
{
   zSHORT       nRC;
   zULONG       ulKey;
   zULONG       ulLth;
   zPCHAR       pCtrlBOI;
   zCTRLBOI_BMP BMP;

   nRC = CheckExistenceOfEntity( vDialog, "Control" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( CheckExistenceOfEntity( vDialog, "ControlDef" ) >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &ulKey, vDialog, "ControlDef", "Key" );
         if ( ulKey == zControl_Bitmap || ulKey == zControl_BitmapButton ||
              ulKey == zControl_Icon )
         {
            GetAttributeLength( &ulLth, vDialog, "Control", "CtrlBOI" );
            if ( ulLth )
            {
               GetAddrForAttribute( &pCtrlBOI, vDialog, "Control", "CtrlBOI" );
               SplitCtrlBOI_BMP( pCtrlBOI, ulLth, &BMP );
               zwfnTZBMUPDD_WritePathToList( vDialog, szDialogName, szWindowName,
                                             BMP, ulKey );
            } // endif ( ulLth )
         }

         if ( SetViewToSubobject( vDialog, "CtrlCtrl" ) >= 0 )
         {
            zwfnSearchPathRecursive( vDialog, szDialogName, szWindowName );
            ResetViewFromSubobject( vDialog );
         }
      }

      nRC = SetCursorNextEntity( vDialog, "Control", 0 );
   }

   return( 0 );
}


static void
zwfnTZBMUPDD_WriteBitmapPath( zVIEW  vSubtask,
                              zPCHAR szDialog,
                              zPCHAR szWindow,
                              zPCHAR szControl,
                              zPCHAR szBitmapPath,
                              zSHORT nError )
{
   zVIEW  vTZBMPATH;
   zCHAR  szMsg[ 256 ];

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZBMUPDD_LoadBitmapList( vSubtask );
      GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK );
   }

   CreateEntity( vTZBMPATH, "BitmapPath", zPOS_LAST );

   if ( nError == 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "*** Error: " );
      strcat_s( szMsg, sizeof( szMsg ), szBitmapPath );
      strcat_s( szMsg, sizeof( szMsg ), " ***" );
   }
   else
   {
      strcpy_s( szMsg, sizeof( szMsg ), szBitmapPath );
   }

   SetAttributeFromString( vTZBMPATH, "BitmapPath", "Dialog", szDialog );
   SetAttributeFromString( vTZBMPATH, "BitmapPath", "Window", szWindow );
   SetAttributeFromString( vTZBMPATH, "BitmapPath", "Control", szControl );
   SetAttributeFromString( vTZBMPATH, "BitmapPath", "BitmapPath", szMsg );
   SetAttributeFromInteger( vTZBMPATH, "BitmapPath", "Error", nError );

} // zwfnTZBMUPDD_WriteBitmapPath

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_SearchBitmaps
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZBMUPDD_SearchBitmaps( zVIEW vSubtask, zVIEW vCM_List, zPCHAR szMetaSrcDir )
{
   zVIEW  vDialog;
   zCHAR  szDialogName[33];
   zCHAR  szWindowName[ 33 ];
   zCHAR  szMsg[ 100 ];
   zCHAR  szFullName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   GetStringFromAttribute( szDialogName, sizeof( szDialogName ), vCM_List, "W_MetaDef", "Name" );
   strcpy_s( szMsg, sizeof( szMsg ), "Read Bitmap Path in Dialog: '" );
   strcat_s( szMsg, sizeof( szMsg ), szDialogName );
   strcat_s( szMsg, sizeof( szMsg ), "'." );
   MB_SetMessage( vSubtask, 1, szMsg );

   strcpy_s( szFullName, sizeof( szFullName ), szMetaSrcDir );
   strcat_s( szFullName, sizeof( szFullName ), szDialogName );
   strcat_s( szFullName, sizeof( szFullName ), ".pwd" );

   // if Activate with Errors
   if ( ActivateOI_FromFile( &vDialog, "TZWDLGSO", vCM_List, szFullName,
                             zSINGLE | zNOI_OKAY | zIGNORE_ERRORS | zLEVEL_TASK ) < 0 )
   {
      zwfnTZBMUPDD_WriteBitmapPath( vSubtask, szDialogName, "", "",
                                    "Could not activate Dialog.", 1 );
      return( -1 );
   }

   for ( nRC = SetCursorFirstEntity( vDialog, "Window", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, "Window", 0 ) )
   {
      if ( SetCursorFirstEntity( vDialog, "Control", 0 ) >= zCURSOR_SET )
      {
         GetStringFromAttribute( szWindowName, sizeof( szWindowName ), vDialog, "Window", "Tag" );
         zwfnSearchPathRecursive( vDialog, szDialogName, szWindowName );
      }
   }

   DropView( vDialog );
   return( 0 );
} // zwTZBMUPDD_SearchBitmaps

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_UsedBitmaps
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_UsedBitmaps( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vTaskLPLR;
   zVIEW  vWindow;
   zCHAR  szMetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   if ( SetCursorFirstSelectedEntity( vCM_List, "W_MetaDef", "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "BM00001", "Bitmap Maintenance", "No Dialog selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   // Create Bitmap Path List
   zwTZBMUPDD_CreateNewBitmapRoot( vSubtask, FALSE );

   GetStringFromAttribute( szMetaSrcDir, sizeof( szMetaSrcDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysAppendcDirSep( szMetaSrcDir );

   for ( nRC = SetCursorFirstSelectedEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vCM_List, "W_MetaDef", 0 ) )
   {
      zwTZBMUPDD_SearchBitmaps( vSubtask, vCM_List, szMetaSrcDir );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "BitmapPath" ) >= 0 )
      zwTZBMUPDD_RefreshBitmapList( vSubtask );

   MB_SetMessage( vSubtask, 1, " " );

   return( 0 );
} // zwTZBMUPDD_UsedBitmaps

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_UsedBitmapsAll
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_UsedBitmapsAll( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zVIEW  vCM_List;
   zVIEW  vTaskLPLR;
   zCHAR  szMetaSrcDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   if ( GetViewByName( &vCM_List, "CM_List", vSubtask, zLEVEL_TASK ) < 0 ||
        GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   // Create Bitmap Path List
   zwTZBMUPDD_CreateNewBitmapRoot( vSubtask, TRUE );

   GetStringFromAttribute( szMetaSrcDir, sizeof( szMetaSrcDir ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysAppendcDirSep( szMetaSrcDir );

   // replace Bitmap Path in all Dialogs
   for ( nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vCM_List, "W_MetaDef", 0 ) )
   {
      zwTZBMUPDD_SearchBitmaps( vSubtask, vCM_List, szMetaSrcDir );
   }

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "BitmapPath" ) >= 0 )
      zwTZBMUPDD_RefreshBitmapList( vSubtask );

   MB_SetMessage( vSubtask, 1, " " );

   return( 0 );
} // zwTZBMUPDD_UsedBitmapsAll

static void
zwfnTZERRORD_WriteErrorListToFile( zVIEW vTZBMPATH, zLONG  FileHandle )
{
   zVIEW  vTaskLPLR;
   zSHORT nRC;
   zCHAR  szMsg[ 500 ];
   zCHAR  szTemp[ 255 ];
   zLONG  i;

   // write Tool and DateTime
   strcpy_s( szMsg, sizeof( szMsg ), "Zeidon: Bitmap List from " );
   GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZBMPATH, "PathList", "DateTimeAndDialogs" );
   strcat_s( szMsg, sizeof( szMsg ), szTemp );
   SysWriteLine( vTZBMPATH, FileHandle, szMsg );
   SysWriteLine( vTZBMPATH, FileHandle, "" );

   // write LPLR Name
   if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vTZBMPATH, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vTaskLPLR, "LPLR", "Name" );
      if ( szTemp[ 0 ] )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Project: " );
         strcat_s( szMsg, sizeof( szMsg ), szTemp );
         SysWriteLine( vTZBMPATH, FileHandle, szMsg );
         SysWriteLine( vTZBMPATH, FileHandle, "" );
      }
   }

   strcpy_s( szMsg, sizeof( szMsg ), "Dialog: Window:            Control: " );
   strcat_s( szMsg, sizeof( szMsg ), "       Bitmap:" );
   SysWriteLine( vTZBMPATH, FileHandle, szMsg );

   // write Error Message
   for ( nRC = SetCursorFirstEntity( vTZBMPATH, "BitmapPath", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vTZBMPATH, "BitmapPath", "" ) )
   {
      GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZBMPATH, "BitmapPath", "Dialog" );
      strcpy_s( szMsg, sizeof( szMsg ), szTemp );
      for ( i = zstrlen( szTemp ); i < 11; i++ )
         strcat_s( szMsg, sizeof( szMsg ), " " );

      GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZBMPATH, "BitmapPath", "Window" );
      strcat_s( szMsg, sizeof( szMsg ), szTemp );
      for ( i = zstrlen( szTemp ); i < 34; i++ )
         strcat_s( szMsg, sizeof( szMsg ), " " );

      GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZBMPATH, "BitmapPath", "Control" );
      strcat_s( szMsg, sizeof( szMsg ), szTemp );
      for ( i = zstrlen( szTemp ); i < 34; i++ )
         strcat_s( szMsg, sizeof( szMsg ), " " );

      GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZBMPATH, "BitmapPath", "BitmapPath" );
      strcat_s( szMsg, sizeof( szMsg ), szTemp );
      SysWriteLine( vTZBMPATH, FileHandle, szMsg );
   }

   SysWriteLine( vTZBMPATH, FileHandle, "" );
   SysWriteLine( vTZBMPATH, FileHandle, "End of Bitmap List" );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_BitmapListSaveToFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_BitmapListSaveToFile( zVIEW vSubtask )
{
   zVIEW  vTZBMPATH;
   zULONG ulZFlags = 0;
   zLONG  FileHandle;
   zCHAR  szMsg[ 300 ];
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   szFileName[ 0 ] = 0;
   ulZFlags = zOFN_OVERWRITEPROMPT | zOFN_CREATEPROMPT | zOFN_SAVEAS | zOFN_HIDEREADONLY;

   if ( OperatorPromptForFile( vSubtask, szFileName, sizeof( szFileName ),
                               "Text Files (*.txt)|*.txt|", "txt", ulZFlags ) == 1 )
   {
      FileHandle = SysOpenFile( vTZBMPATH, szFileName, COREFILE_WRITE );

      if ( FileHandle == -1 )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Unable to open " );
         strcat_s( szMsg, sizeof( szMsg ), szFileName );
         MessageSend( vSubtask, "FEH001", "Bitmap Maintenance", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         return( -1 );
      }

      zwfnTZERRORD_WriteErrorListToFile( vTZBMPATH, FileHandle );
      SysCloseFile( vTZBMPATH, FileHandle, 0 );
   }

   return( 0 );
} // zwTZBMUPDD_BitmapListSaveToFile

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: zwTZBMUPDD_CopyToField
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZBMUPDD_CopyToField( zVIEW vSubtask )
{
   zVIEW vTZBMPATH;
   zVIEW vParentWindow = NULL;
   zCHAR szBitmap[ 256 ];

   if ( GetViewByName( &vTZBMPATH, "TZBMPATH", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   GetParentWindow( &vParentWindow, vSubtask );

   if ( vParentWindow )
   {
      GetStringFromAttribute( szBitmap, sizeof( szBitmap ), vTZBMPATH, "BitmapPath", "BitmapPath" );
      SetCtrlText( vParentWindow, "edOld", szBitmap );
   }

   return( 0 );
} // zwTZBMUPDD_CopyToField

#ifdef __cplusplus
}
#endif
