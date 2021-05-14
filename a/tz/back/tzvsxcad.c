#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

/*
CHANGE LOG
2002.08.15  RR
   Replace Name of Zeidon Online Manuals by DEFINES
2001.02.13  BL  R54697
   improve handling of return code from InitializeLPLR
2000.12.07  BL
   Fix for Auto Tester: set new Message Box Title
2000.10.30 RG  Z2000
   GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.10.25  SR  Z2000  Size of path variables
   Modified size of zExecDir in functions zwTZVSXAD_InitializeDialog,
   zwTZVSXAD_SaveApplicSettings and zwTZVSXAD_ReturnFromSubwindow
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
2000.07.09   BL + TB51013
   Added zwTZVSXCAD_SwitchExecuteStatus for new Popup menu in main window
   Added zwTZVSXCAD_ExecuteDialog: Execute Dialog with current flag
2000.03.31   BL
   Modified zwTZVSXCAD_SetDescriptionInDIL: in error situation does not exits
   the View vCM_ShowDialog (create this view again)
2000.03.28   BL  new Repository structure
   ActivateOI_FromFile with zIGNORE_ATTRIB_ERRORS
2000.02.07   BL
   Modified zwTZVSXCAD_SetupInterp and zwTZVSXCAD_SetupDLL for reset
   TaskUseCount in View TaskLPLR
2000.01.07   BL  Z10
   Modified Operation zwTZVSXCAD_SaveApplicSettings, zwTZVSXCAD_SaveSettings,
   zwTZVSXCAD_SaveSetupFile:
   bugfix ZDR.EXE - Error
1999.12.01   DC  Z10
   Modified zwTZVSXCAD_SetupDLL and zwTZVSXCAD_SetupInterp to reposition
   CM_List view around SaveSettings since SaveSettings code was creating
   a new CM_List view.
1999.11.11   BL  Z10  new menu structure
   Added Operation zwTZVSXCAD_SaveSettings, zwTZVSXCAD_SaveApplicSettings,
   zwTZVSXCAD_SaveSetupFile for save settings
   Modified Operation zwTZVSXCAD_BuildDialogSelectList, zwTZVSXCAD_ExcludeDialog:
   set not null Attributes in View
   Added Operation zwTZVSXCAD_PostBuildSetExeStatus and zwTZVSXCAD_CancelSetExecStaus
   for save temporaly
   Modified Operation zwTZVSXCAD_ReturnFromSubwindow: if external LPLR, then
   X_Checker."APPLICATION"."Name" not equal CM_List."LPLR"."Name" and Zeidon load
   all Data, this is not correct
   Modified Operation zwTZVSXCAD_AcceptOperationBreakp, zwTZVSXCAD_SwitchToDialogMeta:
   no Prompt for save
1999.11.09 and 1999.11.10   BL   Z10  new menu structure
   Modified Operation zwTZVSXCAD_Debug, zwTZVSXCAD_SetupInterp, zwTZVSXCAD_SetupDLL,
   zwTZVSXCAD_Exit_X_Checker and added Operation zwTZVSXCAD_ShowAllDialogs,
   zwTZVSXCAD_InitializeLaunch and zwTZVSXCAD_RemoveAllDialogs for show only selected
   Dialog in main window
   Added Operation zwTZVSXCAD_SetDescriptionInDIL: if single click in main
   window, then set Dialog Description in DIL
1999.11.08   BL   Z10  new menu structure
   Modified Operation zwTZVSXCAD_InitializeDialog, zwTZVSXCAD_ReturnFromSubwindow:
   show only selected Dialog in main window
   Added Opration zwTZVSXCAD_IncludeShowDialog, zwTZVSXCAD_ExcludeShowDialog,
   zwTZVSXCAD_InitializeShowDialog, zwTZVSXCAD_ShowDialogPreBuild,
   zwTZVSXCAD_ShowDialogOK and zwTZVSXCAD_ShowDialogCancel for Menu Edit
1999.09.20   BL   Z10
   Operation: zwTZVSXCAD_SetupInterp, bugfix for following problem:
   if Interpr.Flag = Non-Interp. and do you want execute interpretative, then
   Zeidon send the message Load failed for ...dll
1999.09.13 BL   Z10   update GUI, new menu structure
   insert operation zwTZVSXCAD_Debug and zwTZVSXCAD_AcceptOperationBreakp
from here old version
02.24.1997  DonC
   Added check of return code on SetupInterp in zwTZVSXCAD_SetupInterp.
07.06.1997  DonC
   Modified handling of "CurrentMeta" view to change it to a work object so
   that list box graying and member deletion would be handled correctly.
15.02.1998  DonC
   Modified several operations to sort lists by name. (QS 138)
   Also, in IncludeDOperation, made sure the SourceFile for the operation
   being included existed in the target of the include.
18.08.1998  DonC
   Modified call to InitializeDefaultLPL to check for error return code.
1998/08/26  DC
   Modified zwTZVSXCAD_PosOnDialogSource to initialize OperationList entities
   in a dialog if they do not already exist. (XC 255)
*/

// LAST MESSAGE ID: VS00103



zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_InitializeShowDialog( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_InitializeLaunch( zVIEW vSubtask );
zSHORT /* LOCAL */  OPERATION
zwTZVSXCAD_SaveApplicSettings( zVIEW vSubtask,
                               zSHORT nExitTool,
                               zSHORT nAskForSave );


zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_BuildDialogSelectList( zVIEW    vSubtask )
{
   zVIEW    vDialogList;
   zVIEW    vDialogMeta;
   zVIEW    vX_Checker;
   zSHORT   nRC;

   GetViewByName( &vDialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );
   GetViewByName( &vX_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );

   // This routine builds the DialogList work object "TZDIALOGS" by copying
   // all W_MetaDef entities in vDialogMeta that are not in vX_Checker.

   ActivateEmptyObjectInstance( &vDialogList, "TZCMLPLO", vSubtask, zSINGLE );
   CreateEntity( vDialogList, "LPLR", zPOS_AFTER );
   CreateEntity( vDialogList, "W_MetaType", zPOS_AFTER );
   SetAttributeFromInteger( vDialogList, "W_MetaType", "Type", 2011 );
   SetNameForView( vDialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vDialogMeta, "W_MetaDef", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = SetCursorFirstEntityByAttr( vX_Checker,  "Dialog",    "Name",
                                        vDialogMeta, "W_MetaDef", "Name", 0 );
      if ( nRC < zCURSOR_SET )
      {
         CreateEntity( vDialogList, "W_MetaDef", zPOS_AFTER );
         SetMatchingAttributesByName( vDialogList, "W_MetaDef",
                                      vDialogMeta, "W_MetaDef", zSET_ALL );
      }

      nRC = SetCursorNextEntity( vDialogMeta, "W_MetaDef", 0 );
   }

   OrderEntityForView( vDialogList, "W_MetaDef", "Name A" );
   OrderEntityForView( vX_Checker, "Dialog", "Name A" );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ReuseInitializeDialog( zVIEW   vSubtask,
                                  zVIEW   DialogMeta,
                                  zVIEW   X_Checker )
{
   zCHAR    szName[ 33 ];
   zSHORT   nRC;

   // Make sure the entries in X_Checker.Dialog are in sync with the
   // entries from DialogMeta.W_MetaDef.  Make sure the ZKey values in
   // X_Checker.Dialog.ZKey match the ZKey values from
   // DialogMeta.W_MetaDef.CPLR_ZKey.  Also make sure there are no entries
   // in X_Checker that are not in DialogMeta.
   for ( nRC =  SetCursorFirstEntity( X_Checker, "Dialog", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextEntity( X_Checker, "Dialog", 0 ))
   {
      GetStringFromAttribute( szName, zsizeof( szName ), X_Checker, "Dialog", "Name" );
      nRC = SetCursorFirstEntityByString( DialogMeta, "W_MetaDef", "Name",
                                             szName, 0 );
      if ( nRC >= zCURSOR_SET )
         SetAttributeFromAttribute( X_Checker,  "Dialog",    "ZKey",
                                    DialogMeta, "W_MetaDef", "CPLR_ZKey" );
      else
         DeleteEntity( X_Checker, "Dialog", zREPOS_NONE );
   }

   SetCursorFirstEntity( X_Checker, "Dialog", 0 );

   // Add the InterpretableFlag value to each Dialog W_MetaDef.
   for ( nRC =  SetCursorFirstEntity( DialogMeta, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextEntity( DialogMeta, "W_MetaDef", 0 ))
   {
      GetStringFromAttribute( szName, zsizeof( szName ), DialogMeta, "W_MetaDef", "Name" );
      SetCursorFirstEntityByString( X_Checker, "Dialog", "Name",
                                    szName, 0 );
      nRC = SetCursorFirstEntityByAttr( X_Checker,  "Dialog",    "Name",
                                        DialogMeta, "W_MetaDef", "Name", 0 );
      if ( nRC >= zCURSOR_SET )
         SetAttributeFromString( DialogMeta, "W_MetaDef", "InterpretableFlag",
                                 "Interp." );
      else
         SetAttributeFromString( DialogMeta, "W_MetaDef", "InterpretableFlag",
                                 "Non-Interp." );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_InitializeDialog( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    CM_List;
   zVIEW    DialogMeta;
   zCHAR    szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT   nRC;

   if ( InitializeDefaultLPL( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &CM_List, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &CM_List, "TZCMWKSO", CM_List, zLEVEL_SUBTASK );
   nRC = GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );

   if ( nRC == zLEVEL_APPLICATION )
   {
      if ( CompareAttributeToAttribute( X_Checker, "APPLICATION", "Name", CM_List, "LPLR", "Name" ) == 0 )
      {
         return( 0 );
      }
   }

   nRC = GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), CM_List, "LPLR", "ExecDir" );
   ZeidonStringConcat( szExecDir, 1, 0, "\\TZVSXC01.POR", 1, 0, zMAX_FILESPEC_LTH + 1 );

   nRC = ActivateOI_FromFile( &X_Checker, "TZVSXCOO", vSubtask, szExecDir,
                                    zSINGLE + zNOI_OKAY + zIGNORE_ATTRIB_ERRORS );
   if ( nRC == 0 )
   {
      SetNameForView( X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
      RetrieveViewForMetaList( vSubtask, &DialogMeta, zREFER_DIALOG_META );

      zwTZVSXCAD_ReuseInitializeDialog( vSubtask, DialogMeta, X_Checker );

      //BL, 1999.11.08
      SetNameForView( DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

      zwTZVSXCAD_InitializeLaunch( vSubtask );
      zwTZVSXCAD_InitializeShowDialog( vSubtask );

      return( 0 );
   }

   if ( nRC == -1 )
   {
      nRC = ActivateEmptyObjectInstance( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_TASK );
   }

   CreateEntity( X_Checker, "APPLICATION", zPOS_AFTER );

   SetAttributeFromAttribute( X_Checker, "APPLICATION", "Name", CM_List, "LPLR", "Name" );
   CreateEntity( X_Checker, "Dialogs", zPOS_AFTER );
   CreateEntity( X_Checker, "LODs", zPOS_AFTER );

   CreateEntity( X_Checker, "GlobalOperations", zPOS_AFTER );
   SetNameForView( X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   RetrieveViewForMetaList( vSubtask, &DialogMeta, zREFER_DIALOG_META );

   //BL, 1999.11.08
   SetNameForView( DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );
   zwTZVSXCAD_InitializeLaunch( vSubtask );
   zwTZVSXCAD_InitializeShowDialog( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SwitchToDialogMeta( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    DialogMeta;
   zVIEW    DialogList;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   nRC = GetViewByName( &DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &DialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK ) > 0 )
      AcceptSubobject( DialogList, "W_MetaType" );

   AcceptSubobject( X_Checker, "Dialogs" );

   if ( nRC == zLEVEL_TASK )
   {
      nRC = DropView( DialogMeta );
   }

   RetrieveViewForMetaList( vSubtask, &DialogMeta, zREFER_DIALOG_META );
   SetNameForView( DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_BuildDialogMetaList( zVIEW    vSubtask )
{
   zVIEW    X_Checker  = NULL;
   zVIEW    DialogMeta = NULL;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   nRC = GetViewByName( &DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      nRC = DropView( DialogMeta );
   }

   RetrieveViewForMetaList( vSubtask, &DialogMeta, zREFER_DIALOG_META );

   if ( DialogMeta )
      SetNameForView( DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   RefreshCtrl( vSubtask, "ListBox1" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SwitchToObjectMeta( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    ObjectMeta;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   nRC = GetViewByName( &ObjectMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      DropView( ObjectMeta );
   }

   RetrieveViewForMetaList( vSubtask, &ObjectMeta, zREFER_LOD_META );
   SetNameForView( ObjectMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SwitchToGlobalMeta( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    GlobalMeta;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   nRC = GetViewByName( &GlobalMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   if ( nRC == zLEVEL_TASK )
   {
      nRC = DropView( GlobalMeta );
   }

   RetrieveViewForMetaList( vSubtask, &GlobalMeta, zREFER_GO_META );
   SetNameForView( GlobalMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_SaveSettings
//
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SaveSettings( zVIEW vSubtask )
{
   zVIEW  vDialogMeta_Original;

   zwTZVSXCAD_SaveApplicSettings( vSubtask,
                                  1,    // nExitTool
                                  1 );  // nAskForSave

   if (GetViewByName(&vDialogMeta_Original,"DialogMeta_Original",0,zLEVEL_TASK) >= 0)
      DropView( vDialogMeta_Original );

   return( 0 );
} // zwTZVSXCAD_SaveSettings

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_Exit_X_Checker( zVIEW    vSubtask )
{
   zwTZVSXCAD_SaveApplicSettings( vSubtask,
                                  1,   // nExitTool
                                  1 ); // nAskForSave
   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_SaveSetupFile
//
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SaveSetupFile( zVIEW vSubtask )
{
   zwTZVSXCAD_SaveApplicSettings( vSubtask,
                                  0,    // nExitTool
                                  0 );  // nAskForSave
   return( 0 );
} // zwTZVSXCAD_SaveSetupFile


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_SaveApplicSettings
//
zSHORT /* LOCAL */  OPERATION
zwTZVSXCAD_SaveApplicSettings( zVIEW vSubtask,
                               zSHORT nExitTool,
                               zSHORT nAskForSave )
{
   zVIEW    X_Checker;
   zVIEW    CM_List;
   zVIEW    vCurrentMeta;
   zVIEW    vDialogMeta_Original;
   zVIEW    vZeidonCM;
   zVIEW    vLPLR;
   zVIEW    vLPLR_Copy;
   zCHAR    szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR    szLPLR_Name[ 33 ];
   zSHORT   nRC;

   if ( GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION ) < 0 ||
        GetViewByName( &vCurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK ) < 0 )
   {
      return( -1 );
   }

   GetViewByName( &CM_List, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &CM_List, "TZCMWKSO", CM_List, zLEVEL_SUBTASK );
   GetViewByName( &vDialogMeta_Original, "DialogMeta_Original", vSubtask, zLEVEL_TASK );


   //BL, 1999.11.09 update Dialog List in main window?
   if ( ObjectInstanceUpdatedFromFile( X_Checker ) ||
        CompareAttributeToInteger( vCurrentMeta, "W_MetaType", "SortOrder", 1 ) == 0 )
   {
      SetAttributeFromInteger( vCurrentMeta, "W_MetaType", "SortOrder", 0 );
      if ( nAskForSave == 1 )
         nRC = MessagePrompt( vSubtask, "VS00103",
                              "Application Animation Maintenance",
                              "Save settings?",
                              zBEEP,
                              zBUTTONS_YESNO, zRESPONSE_YES, 0 );
      else
         nRC = zRESPONSE_YES; //if menu item <Save Setting>, then save without Prompt

      if ( nRC == zRESPONSE_YES )
      {
         GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), CM_List, "LPLR", "ExecDir" );
         ZeidonStringConcat( szExecDir, 1, 0, "\\TZVSXC01.POR", 1, 0, zMAX_FILESPEC_LTH + 1 );
         CommitOI_ToFile( X_Checker, szExecDir, zASCII );

         //BL, 1999.11.10 save showed Dialogs for main window in XLP-File
         GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), vDialogMeta_Original, "LPLR", "ExecDir" );
         GetStringFromAttribute( szLPLR_Name, zsizeof( szLPLR_Name ), vDialogMeta_Original, "LPLR", "Name" );
         strcat_s( szExecDir, zsizeof( szExecDir ), "\\" );
         strcat_s( szExecDir, zsizeof( szExecDir ), szLPLR_Name );
         strcat_s( szExecDir, zsizeof( szExecDir ), ".XLP" );
         GetViewByName( &vCurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );
         CommitOI_ToFile( vCurrentMeta, szExecDir, zASCII );
      }
      else
      {
         //BL, 1999.11.10 Reset all Views
         if ( GetViewByName( &vCurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vCurrentMeta );

         //if exit Tool?
         if ( nExitTool == 1 )
         {
            GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
            GetStringFromAttribute( szLPLR_Name, zsizeof( szLPLR_Name ), vDialogMeta_Original, "LPLR", "Name" );
            if ( GetViewByName( &vLPLR, szLPLR_Name, vZeidonCM, zLEVEL_SUBTASK ) >= 0 )
            {
               //BL, 2000.01.07 write old settings in the Zeidon internal View
               CreateViewFromViewForTask( &vLPLR_Copy, vLPLR, 0 );
               RetrieveViewForMetaList( vSubtask, &vLPLR_Copy, zREFER_DIALOG_META );
               nRC = SetCursorFirstEntity( vDialogMeta_Original, "W_MetaDef", "" );
               nRC = SetCursorFirstEntity( vLPLR_Copy, "W_MetaDef", "" );
               while ( nRC == zCURSOR_SET )
               {
                  SetAttributeFromAttribute( vLPLR_Copy, "W_MetaDef", "GroupName",
                                             vDialogMeta_Original, "W_MetaDef", "GroupName" );
                  nRC = SetCursorNextEntity( vDialogMeta_Original, "W_MetaDef", "" );
                  nRC = SetCursorNextEntity( vLPLR_Copy, "W_MetaDef", "" );
               }

               DropView( vLPLR_Copy );
            }
         }

         CreateViewFromViewForTask( &vCurrentMeta, vDialogMeta_Original, 0 );
         SetNameForView( vCurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );
      }
   }

   return( 0 );
} // zwTZVSXCAD_SaveApplicSettings

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeDialog( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vDialog;
   zVIEW    vDialogList;
   zVIEW    CurrentMeta;
   zSHORT   nRC;
   zCHAR    szTempString_0[ 33 ];

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vDialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );
   GetViewByName( &CurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   SetCursorLastEntity( X_Checker, "Dialog", "" );
   nRC = SetCursorFirstSelectedEntity( vDialogList, "W_MetaDef", 0 );
   while ( nRC >= 0)
   {
      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vDialogList, "W_MetaDef", "Name" );
      nRC = SetCursorFirstEntityByString( X_Checker, "Dialog", "Name", szTempString_0, "" );
      if ( nRC < zCURSOR_SET )
      {
         nRC = CreateEntity( X_Checker, "Dialog", zPOS_AFTER );
         SetCursorFirstEntityByAttr( CurrentMeta, "W_MetaDef", "Name",
                                     vDialogList, "W_MetaDef", "Name", 0 );

         ActivateMetaOI( vSubtask, &vDialog, CurrentMeta,
                         zREFER_DIALOG_META, zSINGLE );

         SetAttributeFromAttribute( X_Checker, "Dialog", "Name",
                                    vDialog, "Dialog", "Tag" );
         SetAttributeFromAttribute( X_Checker, "Dialog", "ZKey",
                                    vDialog, "Dialog", "ZKey" );

         nRC = SetCursorFirstEntity( vDialog, "SourceFile", "" );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            IncludeSubobjectFromSubobject( X_Checker, "DialogSourceFile",
                                          vDialog, "SourceFile", zPOS_AFTER );
            nRC = SetCursorNextEntity( vDialog, "SourceFile", "" );
         }

         DropMetaOI( vSubtask, vDialog );
      }

      DeleteEntity( vDialogList, "W_MetaDef", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vDialogList, "W_MetaDef", 0 );
   }

   OrderEntityForView( X_Checker, "Dialog", "Name A" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeDialog( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    DialogList;
   zVIEW    vCM_List;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &DialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( X_Checker, "Dialog", 0 );
   while ( nRC >= 0)
   {
      CreateEntity( DialogList, "W_MetaDef", zPOS_AFTER );
      SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "Name",
                                  X_Checker, "Dialog", "Name", "" );
      SetMatchingAttributesByName( DialogList, "W_MetaDef",
                                   vCM_List, "W_MetaDef", zSET_ALL );
      OrderEntityForView( DialogList, "W_MetaDef", "Name A" );
      DeleteEntity( X_Checker, "Dialog", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( X_Checker, "Dialog", 0 );
   }

   OrderEntityForView( DialogList, "W_MetaDef", "Name A" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_PosOnDialogSource( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vDialog;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetIntegerFromAttribute( &lZKey, X_Checker, "Dialog", "ZKey" );
   nRC = GetViewByName( &vDialog, "TZWDLGSO",
                        vSubtask, zLEVEL_APPLICATION );
   if ( nRC == zLEVEL_APPLICATION )
   {
      nRC = DropMetaOI( vSubtask, vDialog );
   }

   // Create Dialog entity temporal.
   CreateTemporalSubobjectVersion( X_Checker, "Dialog" );

   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vDialog, vCM_List, zREFER_DIALOG_META, zSINGLE );
   SetNameForView( vDialog, "TZWDLGSO", vSubtask, zLEVEL_TASK );

   // If the OperationList derived entities have not been created, create
   // them here.
   if ( CheckExistenceOfEntity( vDialog, "OperationList" ) < zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( vDialog, "Operation", "Dialog" );
      while ( nRC >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( vDialog, "OperationList",
                                        vDialog, "Operation", zPOS_AFTER );
         nRC = SetCursorNextEntity( vDialog, "Operation", "Dialog" );
      }
   }

   OrderEntityForView( vDialog, "OperationList", "Name A" );
   OrderEntityForView( X_Checker, "DialogOperation", "Name A" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeDOperation( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vSourceMeta;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vSourceMeta, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( vSourceMeta == 0 )
      GetViewByName( &vSourceMeta, "TZWDLGSO", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstSelectedEntity( vSourceMeta, "OperationList", 0 );
   while ( nRC >= 0 )
   {
      GetIntegerFromAttribute( &lZKey, vSourceMeta, "OperationList",
                               "ZKey" );
      nRC = SetCursorFirstEntityByInteger( X_Checker, "DialogOperation", "ZKey",
                                              lZKey, "" );
      if ( nRC < zCURSOR_SET )
      {
         // First position on the correct SourceFile and add it if it isn't
         // there.
         GetIntegerFromAttribute( &lZKey, vSourceMeta, "OperSourceFile", "ZKey" );
         nRC = SetCursorFirstEntityByInteger( X_Checker, "DialogSourceFile", "ZKey",
                                                 lZKey, "" );
         if ( nRC < zCURSOR_SET )
         {
            IncludeSubobjectFromSubobject( X_Checker,   "DialogSourceFile",
                                           vSourceMeta, "OperSourceFile", zPOS_AFTER );
         }

         IncludeSubobjectFromSubobject( X_Checker,   "DialogOperation",
                                        vSourceMeta, "OperationList", zPOS_AFTER );
      }

      SetSelectStateOfEntity( vSourceMeta, "OperationList", 0 );
      nRC = SetCursorNextSelectedEntity( vSourceMeta, "OperationList", 0 );
   }

   OrderEntityForView( X_Checker, "DialogOperation", "Name A" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeDOperation( zVIEW vSubtask )
{
   zVIEW    X_Checker;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   nRC = SetCursorFirstSelectedEntity( X_Checker, "DialogOperation", 0 );
   while ( nRC >= 0 )
   {
      ExcludeEntity( X_Checker, "DialogOperation", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( X_Checker, "DialogOperation", 0 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LoadAllSourcesForLOD( zVIEW    X_Checker,
                      zVIEW    vLOD )
{
   zSHORT   nRC;

   nRC = SetCursorFirstEntity( vLOD, "SourceFile", "" );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      IncludeSubobjectFromSubobject( X_Checker, "LODSourceFile",
                                     vLOD, "SourceFile", zPOS_AFTER );
      nRC = SetCursorNextEntity( vLOD, "SourceFile", "" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeLOD( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vLOD;
   zVIEW    LODMeta;
   zCHAR    szTempString_0[ 33 ];
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &LODMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), LODMeta, "W_MetaDef", "Name" );
   nRC = SetCursorFirstEntityByString( X_Checker, "LOD", "Name", szTempString_0, "" );
   if ( nRC == zCURSOR_SET )
   {
      return( 0 );
   }

   SetCursorLastEntity( X_Checker, "LOD", "" );

   ActivateMetaOI( vSubtask, &vLOD, LODMeta, zREFER_LOD_META, zSINGLE );
   nRC = CheckExistenceOfEntity( vLOD, "SourceFile" );
   if ( nRC == 0 )
   {
      CreateEntity( X_Checker, "LOD", zPOS_AFTER );
      SetAttributeFromAttribute( X_Checker, "LOD", "Name", vLOD, "LOD", "Name" );
      SetAttributeFromAttribute( X_Checker, "LOD", "ZKey", vLOD, "LOD", "ZKey" );
      LoadAllSourcesForLOD( X_Checker, vLOD );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeLOD( zVIEW    vSubtask )
{
   zVIEW    X_Checker;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   DeleteEntity( X_Checker, "LOD", zREPOS_PREV );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_PosOnLODSource( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vSourceFile;
   zVIEW    vCM_List;
   zLONG    lZKey;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetIntegerFromAttribute( &lZKey, X_Checker, "Dialog", "ZKey" );
   nRC = GetViewByName( &vSourceFile, "TZZOLODO", vSubtask, zLEVEL_APPLICATION );
   if ( nRC == zLEVEL_APPLICATION )
   {
      nRC = DropMetaOI( vSubtask, vSourceFile );
   }

   // Create Dialog entity temporal.
   CreateTemporalSubobjectVersion( X_Checker, "LOD" );

   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vCM_List, "W_MetaDef",
                                  "CPLR_ZKey", lZKey, "" );
   ActivateMetaOI( vSubtask, &vSourceFile, vCM_List, zREFER_LOD_META, zSINGLE );
   SetNameForView( vSourceFile, "TZZOLODO", vSubtask, zLEVEL_TASK );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeLOperation( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vSourceMeta;
   zSHORT   nRC;
   zLONG    lTempInteger;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &vSourceMeta, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( vSourceMeta == 0 )
      GetViewByName( &vSourceMeta, "TZWDLGSO", vSubtask, zLEVEL_TASK );

   GetIntegerFromAttribute( &lTempInteger, vSourceMeta,
                            "Operation", "ZKey" );
   nRC = SetCursorFirstEntityByInteger( X_Checker, "LOD_Operation",
                                        "ZKey", lTempInteger, "" );
   if ( nRC != zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( X_Checker, "LOD_Operation",
                                     vSourceMeta, "Operation", zPOS_AFTER );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeLOperation( zVIEW    vSubtask )
{
   zVIEW    X_Checker;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   ExcludeEntity( X_Checker, "LOD_Operation", zREPOS_PREV );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LoadAllSourcesForGlobal( zVIEW    X_Checker,
                         zVIEW    vGlobal )
{
   zVIEW    vSourceFile = 0;  // initialize to something DKS 2000.03.09?
   zSHORT   nRC;

   nRC = SetCursorFirstEntity( X_Checker, "GlobalSourceFile", "" );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      while ( nRC > zCURSOR_UNCHANGED &&
              (CompareAttributeToAttribute( X_Checker, "GlobalOperation",
                                            "ZKey", vGlobal, "SourceFile",
                                            "ZKey" ) != 0) )
      {
         nRC = SetCursorNextEntity( X_Checker, "GlobalSourceFile", "" );
      }
   }

   MessageSend( X_Checker, "VS00101", "Application Animation Maintenance",
                "We don't expect to be here yet",
                zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   if ( nRC != zCURSOR_SET )
   {
      IncludeSubobjectFromSubobject( X_Checker, "GlobalSourceFile",
                                     vSourceFile, "SourceFile", zPOS_AFTER );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeGlobal( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    vGlobal;
   zVIEW    GlobalMeta;
   zCHAR    szTempString_0[ 33 ];
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &GlobalMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), GlobalMeta, "W_MetaDef", "Name" );
   nRC = SetCursorFirstEntityByString( X_Checker, "GlobalOperation", "Name", szTempString_0, "" );
   if ( nRC == zCURSOR_SET )
   {
      return( 0 );
   }

   SetCursorLastEntity( X_Checker, "GlobalOperation", "" );
   CreateEntity( X_Checker, "GlobalOperation", zPOS_AFTER );

   ActivateMetaOI( vSubtask, &vGlobal, GlobalMeta, zREFER_GO_META, zSINGLE );

   SetAttributeFromAttribute( X_Checker, "GlobalOperation", "Name", vGlobal, "Operation", "Name" );
   SetAttributeFromAttribute( X_Checker, "GlobalOperation", "ZKey", vGlobal, "Operation", "ZKey" );
   LoadAllSourcesForGlobal( X_Checker, vGlobal );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeGlobal( zVIEW    vSubtask )
{
   zVIEW    X_Checker;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   DeleteEntity( X_Checker, "GlobalOperation", zREPOS_PREV );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SetupDLL( zVIEW    vSubtask )
{
   zVIEW    CM_List;
   zVIEW    LPLR;
   zVIEW    X_Checker;
   zVIEW    vTempSubtask;
   zVIEW    vCM_ShowDialog;
   zCHAR    szAppName[ 129 ];
   zCHAR    szDialogName[ 129 ];
   zLONG    lTaskUseCnt;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &CM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   //BL, 1999.11.09
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( CM_List, "W_MetaDef", "Name",
                               vCM_ShowDialog, "W_MetaDef", "Name", 0 );
   GetStringFromAttribute( szDialogName, zsizeof( szDialogName ), CM_List, "W_MetaDef", "Name" );

   //BL, 1999.11.11 Prompt for Save and reset views
   zwTZVSXCAD_SaveSettings( vSubtask );

   GetViewByName( &CM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByString( CM_List, "W_MetaDef", "Name",
                                 szDialogName, 0 );

   GetViewByName( &LPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szAppName, zsizeof( szAppName ), LPLR, "LPLR", "Name" );
   GetStringFromAttribute( szDialogName, zsizeof( szDialogName ), CM_List, "W_MetaDef", "Name" );

   // Create the appl subtask and name it with the name sent in.
   SfCreateSubtask( &vTempSubtask, vSubtask, szAppName );

   // 5/10/93
   SetNameForView( vTempSubtask, szAppName, vSubtask, zLEVEL_TASK );
   SetNewApplicationSubtask( vSubtask, vTempSubtask );
   if ( CompareAttributeToInteger( X_Checker, "APPLICATION", "InterpMode", 0 ) == 0 )
   {
      //BL, 2000.01.07 Reset TaskUseCount
      GetIntegerFromAttribute( &lTaskUseCnt, LPLR, "LPLR", "TaskUseCount" );
      if ( lTaskUseCnt > 0 )
      {
         lTaskUseCnt--;
         SetAttributeFromInteger( LPLR, "LPLR", "TaskUseCount", lTaskUseCnt );
      }

      SetWindowActionBehavior( vSubtask, zWAB_ReplaceCurrentAppWithApp,
                               szDialogName, "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION   zwTZVSXCAD_HelpAbout
// PURPOSE:    This Entry does the About
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_HelpAbout( zVIEW vSubtask )
{

   IssueToolAbout( vSubtask, zTOOL_DEBUG, __FILE__, __DATE__, __TIME__ );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZVSXCAD_HelpBasics
// PURPOSE:    This Entry transfers to basic help
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_HelpBasics( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, BASICS_MANUAL );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZVSXCAD_HelpOnHelp
// PURPOSE:    This Entry transfers to HelpOnHelp
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_HelpOnHelp( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, SYSTEM_HELP );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION:  zwTZVSXCAD_HelpContents
// PURPOSE:    This Entry transfers to help Contents
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_HelpContents( zVIEW vSubtask )
{
   StartZeidonHelp( vSubtask, USERS_GUIDE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ReturnFromSubwindow( zVIEW    vSubtask )
{
   // This routine resets the TZVSXCAD object after a Switch LPLR.  To do
   // this it just examines the current TZVSXCAD object to see if it is
   // correct.  If not, it drops it and loads a new one.

   zVIEW    X_Checker   = NULL;
   zVIEW    CM_List     = NULL;
   zVIEW    vDialogList = NULL;
   zCHAR    szExecDir[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW    vDialogMeta_Original;
   zSHORT   nZRetCode;
   zSHORT   nRC;

   // Drop the DialogList object so it will be forced to be built afresh next
   // time. This is necessary around a switch LPLR.
   if ( GetViewByName( &vDialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK ) > 0 )
      DropObjectInstance( vDialogList );

   if ( GetViewByName( &CM_List, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByInteger( CM_List, "W_MetaType", "Type",
                                     zREFER_DIALOG_META, 0 );

   nZRetCode = GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );

   //BL, 1999.11.11 if switch LPLR ?
   if ( GetViewByName( &vDialogMeta_Original, "DialogMeta_Original", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      zwTZVSXCAD_InitializeLaunch( vSubtask );
      nRC = 0;
   }
   else
      nRC = -1;

   GetViewByName( &vDialogMeta_Original, "DialogMeta_Original", vSubtask, zLEVEL_TASK );

   if ( nZRetCode > 0 && nRC == 0 )
   {
      DropObjectInstance( X_Checker );
      GetStringFromAttribute( szExecDir, zsizeof( szExecDir ), CM_List, "LPLR", "ExecDir" );
      ZeidonStringConcat( szExecDir, 1, 0, "\\TZVSXC01.POR", 1, 0, zMAX_FILESPEC_LTH+1 );
      nZRetCode = ActivateOI_FromFile( &X_Checker, "TZVSXCOO", vSubtask, szExecDir,
                                       zSINGLE + zNOI_OKAY + zIGNORE_ATTRIB_ERRORS );
      if ( nZRetCode == 0 )
      {
         SetNameForView( X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
      }

      //Create new View vDialogMeta_Original
      zwTZVSXCAD_InitializeLaunch( vSubtask );
   }

   if ( CM_List && nZRetCode < 0 )
   {
      ActivateEmptyObjectInstance( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_TASK );

      CreateEntity( X_Checker, "APPLICATION", zPOS_AFTER );

      SetAttributeFromAttribute( X_Checker, "APPLICATION", "Name", CM_List, "LPLR", "Name" );
      CreateEntity( X_Checker, "Dialogs", zPOS_AFTER );
      CreateEntity( X_Checker, "LODs", zPOS_AFTER );

      CreateEntity( X_Checker, "GlobalOperations", zPOS_AFTER );
      SetNameForView( X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   }

   if ( CM_List && X_Checker )
      zwTZVSXCAD_ReuseInitializeDialog( vSubtask, CM_List, X_Checker );

   //BL, 1999.11.08
   zwTZVSXCAD_InitializeShowDialog( vSubtask );

   RefreshWindow( vSubtask );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_Debug
//
// set Interpret-Flag and call Dialog for
// Debug
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_Debug( zVIEW    vSubtask )
{
   zVIEW    X_Checker;
   zVIEW    DialogMeta;
   zVIEW    vDialog;
   zVIEW    CM_List;
   zVIEW    vCM_ShowDialog;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );

   nRC = GetViewByName( &CM_List, "TZWDLGSO", vSubtask, zLEVEL_APPLICATION );
   if ( nRC == zLEVEL_APPLICATION )
      DropMetaOI( vSubtask, CM_List );

   GetViewByName( &DialogMeta, "CurrentMeta", vSubtask, zLEVEL_TASK );

   //BL, 1999.11.09
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( DialogMeta, "W_MetaDef", "Name",
                               vCM_ShowDialog, "W_MetaDef", "Name", 0 );

   nRC = SetCursorFirstEntityByAttr( X_Checker, "Dialog", "Name",
                                     DialogMeta, "W_MetaDef", "Name", 0 );

   if ( nRC < zCURSOR_SET )
   {
      CreateTemporalEntity( X_Checker, "Dialog", zPOS_AFTER );
      ActivateMetaOI( vSubtask, &vDialog, DialogMeta, zREFER_DIALOG_META, zSINGLE );
      SetAttributeFromAttribute( X_Checker, "Dialog", "Name", vDialog, "Dialog", "Tag" );
      SetAttributeFromAttribute( X_Checker, "Dialog", "ZKey", vDialog, "Dialog", "ZKey" );

      nRC = SetCursorFirstEntity( vDialog, "SourceFile", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         IncludeSubobjectFromSubobject( X_Checker, "DialogSourceFile",
                                        vDialog, "SourceFile", zPOS_AFTER );
         nRC = SetCursorNextEntity( vDialog, "SourceFile", "" );
      }

      DropMetaOI( vSubtask, vDialog);
   }
   else
      CreateTemporalSubobjectVersion( X_Checker, "Dialog" );


   ActivateMetaOI( vSubtask, &CM_List, DialogMeta, zREFER_DIALOG_META, zSINGLE );
   SetNameForView( CM_List, "TZWDLGSO", vSubtask, zLEVEL_TASK );

   // If the OperationList derived entities have not been created, create
   // them here.
   if ( CheckExistenceOfEntity( CM_List, "OperationList" ) < zCURSOR_SET )
   {
      nRC = SetCursorFirstEntity( CM_List, "Operation", "Dialog" );
      while ( nRC >= zCURSOR_SET )
      {
         IncludeSubobjectFromSubobject( CM_List, "OperationList",
                                        CM_List, "Operation", zPOS_AFTER );
         nRC = SetCursorNextEntity( CM_List, "Operation", "Dialog" );
      }
   }

   OrderEntityForView( CM_List, "OperationList", "Name A" );
   OrderEntityForView( X_Checker, "DialogOperation", "Name A" );

   return( 0 );
} // zwTZVSXCAD_Debug

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_AcceptOperationBreakp
//
// accept the Operation Breakpoint
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_AcceptOperationBreakp( zVIEW vSubtask )
{
   zVIEW    X_Checker;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );

   AcceptSubobject( X_Checker, "Dialog" );
   zwTZVSXCAD_SwitchToDialogMeta( vSubtask );

   return( 0 );
} // zwTZVSXCAD_AcceptOperationBreakp


zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SetupInterp( zVIEW    vSubtask )
{
   zVIEW    CM_List;
   zVIEW    LPLR;
   zVIEW    X_Checker;
   zVIEW    vDialog;
   zVIEW    vCM_ShowDialog;
   zCHAR    szAppName[ 129 ];
   zCHAR    szDialogName[ 129 ];
   zLONG    lTaskUseCnt;
   zSHORT   nInterpFlag;
   zSHORT   nRC;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &CM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   //BL, 1999.11.09
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByAttr( CM_List, "W_MetaDef", "Name",
                               vCM_ShowDialog, "W_MetaDef", "Name", 0);
   GetStringFromAttribute( szDialogName, zsizeof( szDialogName ), CM_List, "W_MetaDef", "Name" );

   //BL, 1999.11.11 Prompt for Save and reset views
   zwTZVSXCAD_SaveSettings( vSubtask );

   GetViewByName( &CM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByString( CM_List, "W_MetaDef", "Name",
                                 szDialogName, 0 );

   GetViewByName( &LPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szAppName, zsizeof( szAppName ), LPLR, "LPLR", "Name" );
   if ( ZeidonStringCompare( szAppName, 1, 0, "TZ8C", 1, 0, 128 ) == 0 )
   {
      ZeidonStringCopy( szAppName, 1, 0, "Zeidon_Tools", 1, 0, 128 );
   }

   //BL, 20.09.1999
   //if Interpr.Flag = Non-Interp. and do you want execute interpretative, then
   //Zeidon send the message Load failed for ...dll
   //this is the bugfix for the problem
   nRC = SetCursorFirstEntityByAttr( X_Checker, "Dialog", "Name",
                                     CM_List, "W_MetaDef", "Name", 0 );
   if ( nRC < zCURSOR_SET )
   {
      nInterpFlag = 1;
      CreateTemporalEntity( X_Checker, "Dialog", zPOS_AFTER );
   }
   else
      nInterpFlag = 0;

   ActivateMetaOI_ByName( vSubtask, &vDialog, 0, zREFER_DIALOG_META,
                          zSINGLE, szDialogName, 0 );

   //BL, 20.09.1999
   if ( nInterpFlag == 1 )
   {
      SetAttributeFromAttribute( X_Checker, "Dialog", "Name", vDialog, "Dialog", "Tag" );
      SetAttributeFromAttribute( X_Checker, "Dialog", "ZKey", vDialog, "Dialog", "ZKey" );

      nRC = SetCursorFirstEntity( vDialog, "SourceFile", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         IncludeSubobjectFromSubobject( X_Checker, "DialogSourceFile",
                                        vDialog, "SourceFile", zPOS_AFTER );
         nRC = SetCursorNextEntity( vDialog, "SourceFile", "" );
      }
   }

   // If not there error.
   if ( CheckExistenceOfEntity( vDialog, "DfltWnd" ) != 0 )
   {
      MessageSend( vSubtask, "VS00102", "Application Animation Maintenance",
                   "No Default Window Exists for Dialog",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropMetaOI( vSubtask, vDialog );
      return( 0 );
   }

   DropMetaOI( vSubtask, vDialog );

   nRC = SetupInterp( vSubtask, szAppName );
   if ( nRC < 0 )
      return( nRC );

   // BL, 2000.01.07 Reset TaskUseCnt
   if ( GetViewByName( &LPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( &lTaskUseCnt, LPLR, "LPLR", "TaskUseCount" );
      if ( lTaskUseCnt > 0 )
      {
         lTaskUseCnt--;
         SetAttributeFromInteger( LPLR, "LPLR", "TaskUseCount", lTaskUseCnt );
      }
   }

   nRC = SetWindowActionBehavior( vSubtask,
                                  zWAB_ReplaceCurrentAppWithApp,
                                  szDialogName, "" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_IncludeShowDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_IncludeShowDialog( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;
   zSHORT  nRC;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   for ( nRC =  SetCursorFirstSelectedEntity( vCM_DialogList, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextSelectedEntity( vCM_DialogList, "W_MetaDef", 0 ))
   {
      SetAttributeFromInteger( vCM_List, "W_MetaType", "SortOrder", 1 );
      SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "Name",
                                  vCM_DialogList, "W_MetaDef", "Name", "" );
      SetAttributeFromAttribute( vCM_List, "W_MetaDef", "GroupName",
                                 vCM_List, "W_MetaDef", "Name" );
      CreateEntity( vCM_ShowDialog, "W_MetaDef", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_ShowDialog, "W_MetaDef",
                                   vCM_List, "W_MetaDef", zSET_ALL );
      DeleteEntity( vCM_DialogList, "W_MetaDef", zREPOS_NONE );
   }

   OrderEntityForView( vCM_ShowDialog, "W_MetaDef", "Name A" );

   return( 0 );
} // zwTZVSXCAD_IncludeShowDialog

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ExcludeShowDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ExcludeShowDialog( zVIEW vSubtask )
{
   zVIEW  vCM_List;
   zVIEW  vCM_DialogList;
   zVIEW  vCM_ShowDialog;
   zSHORT nRC;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   for ( nRC =  SetCursorFirstSelectedEntity( vCM_ShowDialog, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextSelectedEntity( vCM_ShowDialog, "W_MetaDef", 0 ))
   {
      SetAttributeFromInteger( vCM_List, "W_MetaType", "SortOrder", 1 );
      SetCursorFirstEntityByAttr( vCM_List, "W_MetaDef", "Name",
                                  vCM_ShowDialog, "W_MetaDef", "Name", "" );
      SetAttributeFromString( vCM_List, "W_MetaDef", "GroupName", "" );
      CreateEntity( vCM_DialogList, "W_MetaDef", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_DialogList, "W_MetaDef",
                                   vCM_List, "W_MetaDef", zSET_ALL );
      DeleteEntity( vCM_ShowDialog, "W_MetaDef", zREPOS_NONE );
   }

   OrderEntityForView( vCM_DialogList, "W_MetaDef", "Name A" );

   return( 0 );
} // zwTZVSXCAD_ExcludeShowDialog

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_InitializeShowDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_InitializeShowDialog( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_ShowDialog;
   zVIEW   vCM_DialogList;
   zVIEW   vTaskLPLR;
   zSHORT  nRC;

   if ( GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK ) >= 0 )
         DropView( vCM_ShowDialog );
      if ( GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK ) >= 0 )
         DropView( vCM_DialogList );

      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

      //View for show Dialog in main window
      ActivateEmptyObjectInstance( &vCM_ShowDialog, "TZCMLPLO", vSubtask, zSINGLE );
      CreateEntity( vCM_ShowDialog, "LPLR", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_ShowDialog, "LPLR",
                                   vTaskLPLR, "LPLR", zSET_ALL );
      CreateEntity( vCM_ShowDialog, "W_MetaType", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_ShowDialog, "W_MetaType",
                                   vCM_List, "W_MetaType", zSET_ALL );
      SetNameForView( vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );

      //View for Dialogs do not show in main window
      ActivateEmptyObjectInstance( &vCM_DialogList, "TZCMLPLO", vSubtask, zSINGLE );
      CreateEntity( vCM_DialogList, "LPLR", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_DialogList, "LPLR",
                                   vTaskLPLR, "LPLR", zSET_ALL );
      CreateEntity( vCM_DialogList, "W_MetaType", zPOS_AFTER );
      SetMatchingAttributesByName( vCM_DialogList, "W_MetaType",
                                   vCM_List, "W_MetaType", zSET_ALL );
      SetNameForView( vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );

      for ( nRC =  SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
            nRC >= zCURSOR_SET;
            nRC =  SetCursorNextEntity( vCM_List, "W_MetaDef", 0 ))
      {
         if ( CompareAttributeToString( vCM_List, "W_MetaDef", "GroupName", "" ) != 0 )
         {
            CreateEntity( vCM_ShowDialog, "W_MetaDef", zPOS_AFTER );
            SetMatchingAttributesByName( vCM_ShowDialog, "W_MetaDef",
                                         vCM_List, "W_MetaDef", zSET_ALL );
         }
         else
         {
            CreateEntity( vCM_DialogList, "W_MetaDef", zPOS_AFTER );
            SetMatchingAttributesByName( vCM_DialogList, "W_MetaDef",
                                         vCM_List, "W_MetaDef", zSET_ALL );
         }
      }

      SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
      SetCursorFirstEntity( vCM_ShowDialog, "W_MetaDef", 0 );
   }

   return( 0 );
} // zwTZVSXCAD_InitializeShowDialog

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_SetDescriptionInDIL
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_SetDescriptionInDIL( zVIEW vSubtask )
{
   zCHAR  szDescription[ 254 ];
   zVIEW  vCM_ShowDialog = NULL;

   if ( GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZVSXCAD_InitializeShowDialog( vSubtask );
      GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   }

   if ( vCM_ShowDialog &&
        CheckExistenceOfEntity( vCM_ShowDialog, "W_MetaDef" ) >= zCURSOR_SET )
   {
      GetStringFromAttribute( szDescription, zsizeof( szDescription ), vCM_ShowDialog, "W_MetaDef", "Desc" );
      MB_SetMessage( vSubtask, 1, szDescription );
   }
   else
      MB_SetMessage( vSubtask, 1, "" );

   return( 0 );
} // zwTZVSXCAD_SetDescriptionInDIL

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ShowAllDialogs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ShowAllDialogs( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;
   zSHORT  nRC;
   zSHORT  nRC1;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   for ( nRC =  SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextEntity( vCM_List, "W_MetaDef", 0 ))
   {

      SetAttributeFromInteger( vCM_List, "W_MetaType", "SortOrder", 1 );

      nRC1 = SetCursorFirstEntityByAttr( vCM_ShowDialog, "W_MetaDef", "Name",
                                         vCM_List, "W_MetaDef", "Name", "" );

      if ( nRC1 < zCURSOR_SET )
      {
         SetAttributeFromAttribute( vCM_List, "W_MetaDef", "GroupName",
                                    vCM_List, "W_MetaDef", "Name" );
         CreateEntity( vCM_ShowDialog, "W_MetaDef", zPOS_AFTER );
         SetMatchingAttributesByName( vCM_ShowDialog, "W_MetaDef",
                                      vCM_List, "W_MetaDef", zSET_ALL );

         SetCursorFirstEntityByAttr( vCM_DialogList, "W_MetaDef", "Name",
                                     vCM_List, "W_MetaDef", "Name", "" );
         DeleteEntity( vCM_DialogList, "W_MetaDef", zREPOS_NONE );
      }
   }

   OrderEntityForView( vCM_ShowDialog, "W_MetaDef", "Name A" );
   SetCursorFirstEntity( vCM_ShowDialog, "W_MetaDef", 0 );

   return( 0 );
} // zwTZVSXCAD_ShowAllDialogs

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_RemoveAllDialogs
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_RemoveAllDialogs( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;
   zSHORT  nRC;
   zSHORT  nRC1;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   for ( nRC =  SetCursorFirstEntity( vCM_List, "W_MetaDef", 0 );
         nRC >= zCURSOR_SET;
         nRC =  SetCursorNextEntity( vCM_List, "W_MetaDef", 0 ))
   {
      SetAttributeFromInteger( vCM_List, "W_MetaType", "SortOrder", 1 );

      nRC1 = SetCursorFirstEntityByAttr( vCM_ShowDialog, "W_MetaDef", "Name",
                                         vCM_List, "W_MetaDef", "Name", "" );

      if ( nRC1 >= zCURSOR_SET )
      {
         SetAttributeFromString( vCM_List, "W_MetaDef", "GroupName", "" );
         SetCursorFirstEntityByAttr( vCM_DialogList, "W_MetaDef", "Name",
                                     vCM_List, "W_MetaDef", "Name", "" );
         CreateEntity( vCM_DialogList, "W_MetaDef",zPOS_AFTER );
         SetMatchingAttributesByName( vCM_DialogList, "W_MetaDef",
                                      vCM_List, "W_MetaDef", zSET_ALL );

         DeleteEntity( vCM_ShowDialog, "W_MetaDef", zREPOS_NONE );
      }
   }

   OrderEntityForView( vCM_DialogList, "W_MetaDef", "Name A" );
   SetCursorFirstEntity( vCM_DialogList, "W_MetaDef", 0 );

   return( 0 );
} // zwTZVSXCAD_RemoveAllDialogs

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ShowDialogPreBuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ShowDialogPreBuild( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( vCM_List, "W_MetaType" );
   CreateTemporalSubobjectVersion( vCM_DialogList, "W_MetaType" );
   CreateTemporalSubobjectVersion( vCM_ShowDialog, "W_MetaType" );

   return( 0 );
} // zwTZVSXCAD_ShowDialogPreBuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ShowDialogCancel
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ShowDialogCancel( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   CancelSubobject( vCM_List, "W_MetaType" );
   CancelSubobject( vCM_DialogList, "W_MetaType" );
   CancelSubobject( vCM_ShowDialog, "W_MetaType" );

   return( 0 );
} // zwTZVSXCAD_ShowDialogCancel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ShowDialogOK
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_ShowDialogOK( zVIEW vSubtask )
{
   zVIEW   vCM_List;
   zVIEW   vCM_DialogList;
   zVIEW   vCM_ShowDialog;

   GetViewByName( &vCM_DialogList, "CM_DialogList", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   GetViewByName( &vCM_List, "CurrentMeta", vSubtask, zLEVEL_TASK );

   AcceptSubobject( vCM_List, "W_MetaType" );
   AcceptSubobject( vCM_DialogList, "W_MetaType" );
   AcceptSubobject( vCM_ShowDialog, "W_MetaType" );

   return( 0 );
} // zwTZVSXCAD_ShowDialogOK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_InitializeLaunch
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_InitializeLaunch( zVIEW vSubtask )
{
   zVIEW  vCurrentMeta;
   zVIEW  vDialogMeta_Original;
   zVIEW  vTaskLPLR;
   zSHORT nRC;

   if ( GetViewByName( &vCurrentMeta, "CurrentMeta", vSubtask, zLEVEL_TASK ) > 0 )
   {
      if ( GetViewByName( &vDialogMeta_Original, "DialogMeta_Original", vSubtask, zLEVEL_TASK ) >= 0 )
         DropView( vDialogMeta_Original );

      GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

      SetAttributeFromInteger( vCurrentMeta, "W_MetaType", "SortOrder", 0 );

      ActivateEmptyObjectInstance( &vDialogMeta_Original, "TZCMLPLO", vSubtask, zSINGLE );
      CreateEntity( vDialogMeta_Original, "LPLR", zPOS_AFTER );
      SetMatchingAttributesByName( vDialogMeta_Original, "LPLR",
                                   vTaskLPLR, "LPLR", zSET_ALL );
      CreateEntity( vDialogMeta_Original, "W_MetaType", zPOS_AFTER );
      SetMatchingAttributesByName( vDialogMeta_Original, "W_MetaType",
                                   vCurrentMeta, "W_MetaType", zSET_ALL );
      SetNameForView( vDialogMeta_Original, "DialogMeta_Original", vSubtask, zLEVEL_TASK );

      for ( nRC =  SetCursorFirstEntity( vCurrentMeta, "W_MetaDef", 0 );
            nRC >= zCURSOR_SET;
            nRC =  SetCursorNextEntity( vCurrentMeta, "W_MetaDef", 0 ))
      {
         CreateEntity( vDialogMeta_Original, "W_MetaDef", zPOS_AFTER );
         SetMatchingAttributesByName( vDialogMeta_Original, "W_MetaDef",
                                      vCurrentMeta, "W_MetaDef", zSET_ALL );
      }
   }

   return( 0 );
} // zwTZVSXCAD_InitializeLaunch

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_PostBuildSetExeStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_PostBuildSetExeStatus( zVIEW vSubtask )
{
   zVIEW X_Checker;
   zVIEW DialogList;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &DialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );

   CreateTemporalSubobjectVersion( DialogList, "W_MetaType" );
   CreateTemporalSubobjectVersion( X_Checker, "Dialogs" );

   return( 0 );
} // zwTZVSXCAD_PostBuildSetExeStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_CancelSetExecStaus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZVSXCAD_CancelSetExecStaus( zVIEW vSubtask )
{
   zVIEW X_Checker;
   zVIEW DialogList;

   GetViewByName( &X_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
   GetViewByName( &DialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );

   CancelSubobject( DialogList, "W_MetaType" );
   CancelSubobject( X_Checker, "Dialogs" );

   return( 0 );
} // zwTZVSXCAD_CancelSetExecStaus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_SwitchExecuteStatus
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZVSXCAD_SwitchExecuteStatus( zVIEW vSubtask )
{
   zVIEW  vDialogList;
   zVIEW  vCM_ShowDialog;
   zVIEW  vX_Checker;
   zLONG  lZKey;

   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );

   // save cursor position
   GetIntegerFromAttribute( &lZKey, vCM_ShowDialog, "W_MetaDef", "CPLR_ZKey" );

   if ( GetViewByName( &vDialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK ) < 0 )
   {
      zwTZVSXCAD_BuildDialogSelectList( vSubtask );
   }

   if ( CompareAttributeToString( vCM_ShowDialog, "W_MetaDef", "InterpretableFlag",
                                  "Non-Interp." ) == 0 )
   {
      GetViewByName( &vDialogList, "TZDIALOGS", vSubtask, zLEVEL_TASK );
      if ( SetCursorFirstEntityByAttr( vDialogList, "W_MetaDef", "Name",
                           vCM_ShowDialog, "W_MetaDef", "Name", "" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vDialogList, "W_MetaDef", 1 );
      }

      zwTZVSXCAD_IncludeDialog( vSubtask );
   }
   else
   {
      GetViewByName( &vX_Checker, "TZVSXCOO", vSubtask, zLEVEL_APPLICATION );
      if ( SetCursorFirstEntityByAttr( vX_Checker, "Dialog", "Name",
                        vCM_ShowDialog, "W_MetaDef", "Name", "" ) >= zCURSOR_SET )
      {
         SetSelectStateOfEntity( vX_Checker, "Dialog", 1 );
      }

      zwTZVSXCAD_ExcludeDialog( vSubtask );
   }

   zwTZVSXCAD_ReturnFromSubwindow( vSubtask );

   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );
   SetCursorFirstEntityByInteger( vCM_ShowDialog, "W_MetaDef", "CPLR_ZKey", lZKey, "" );

   return( 0 );
} // zwTZVSXCAD_SwitchExecuteStatus

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZVSXCAD_ExecuteDialog
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZVSXCAD_ExecuteDialog( zVIEW vSubtask )
{
   zVIEW  vCM_ShowDialog;

   GetViewByName( &vCM_ShowDialog, "CM_ShowDialog", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vCM_ShowDialog, "W_MetaDef", "InterpretableFlag",
                                  "Non-Interp." ) == 0 )
   {
      zwTZVSXCAD_SetupDLL( vSubtask );
   }
   else
   {
      zwTZVSXCAD_SetupInterp( vSubtask );
   }

   return( 0 );
} // zwTZVSXCAD_ExecuteDialog
