/*
CHANGE LOG

2001.06.09  PAS
   Modified function zwTZHKEYAD_SaveHK_ForLPLR to save "source Meta" with
   latest release level to eliminate binary data.

2000.10.24  SR  Z2000  Size of path variables
   Modified size of szFileSpec in function zwTZHKEYAD_InitHK_ForLPLR and
   zwTZHKEYAD_SaveHK_ForLPLR because size of attributes ExecDir, MetaSrcDir
   and PgmSrcDir has been changed to 254 in datamodel.

2000.01.13   BL
   insert casting for remove warnings

1999.04.04   DKS
   Attempt to get a viable global Hotkey dialog.

*/

#define zGLOBAL_DATA
#include "..\..\a\oe\kzoephdr.h"   // added to get "..\oe\kzoengaa.hg" to work
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "ZeidonOp.H"

//*** NOTE: Go to "..\oe\kzoengaa.hg" to change the values.
//zPCHAR szlReleaseCompatible = zRELEASE_COMPAT_LEVEL;
//zPCHAR szlReleaseCurrent    = zRELEASE_CURRENT_LEVEL;

// LAST MESSAGE ID: HK00105

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Set the VKey Ctrl/Alt/Shift check boxes
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
PostBSetVKey( zVIEW vSubtask )
{
   zULONG ulLth = sizeof( zACCEL_DEF );
   zVIEW  vHotkey;
   zACCEL_DEF ac;

   GetViewByName( &vHotkey, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   if ( vHotkey &&
        CheckExistenceOfEntity( vHotkey, "Hotkey" ) > zCURSOR_UNCHANGED )
   {
      GetBlobFromAttribute( &ac, &ulLth, vHotkey, "Hotkey", "KeyCombo" );
      SetCtrlState( vSubtask, "Ctrl", zCONTROL_STATUS_CHECKED, ac.fCtrl );
      SetCtrlState( vSubtask, "Alt", zCONTROL_STATUS_CHECKED, ac.fAlt );
      SetCtrlState( vSubtask, "Shift", zCONTROL_STATUS_CHECKED, ac.fShift );
//    if ( CheckExistenceOfEntity( vHotkey, "Operation" ) > zCURSOR_UNCHANGED )
//    {
//       zPCHAR pch;
//
//       GetAddrForAttribute( &pch, vHotkey, "Operation", "DLL_Name" );
//       SetCtrlText( vSubtask, "DLL_Name", pch );
//    }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Save the VKey Ctrl/Alt/Shift based on the check box settings
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SaveVKey( zVIEW vSubtask )
{
   zVIEW  vHotkey;
   zACCEL_DEF ac;
   zLONG  lVKey;

   zmemset( &ac, 0, sizeof( zACCEL_DEF ) );
   GetViewByName( &vHotkey, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lVKey, vHotkey, "Hotkey", "VKey" );
   ac.uVK = (zUSHORT) lVKey;

   ac.fCtrl = (zUSHORT) GetCtrlState( vSubtask, "Ctrl",
                                      zCONTROL_STATUS_CHECKED );
   ac.fAlt = (zUSHORT) GetCtrlState( vSubtask, "Alt",
                                     zCONTROL_STATUS_CHECKED );
   ac.fShift = (zUSHORT) GetCtrlState( vSubtask, "Shift",
                                       zCONTROL_STATUS_CHECKED );

   SetAttributeFromBlob( vHotkey, "Hotkey", "KeyCombo",
                         &ac, sizeof( zACCEL_DEF ) );
// if ( CheckExistenceOfEntity( vHotkey, "Operation" ) > zCURSOR_UNCHANGED )
// {
//    zCHAR szText[ 64 ];
//
//    GetCtrlText( vSubtask, "DLL_Name", szText, zsizeof( szText ) );
//    SetAttributeFromString( vHotkey, "Operation", "DLL_Name", 0 );
// }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Set the title of the window to contain the LPLR Name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_SetTitle( zVIEW vSubtask )
{
   zVIEW vLPL;
   zPCHAR szLPL_Name;

   // Set LPL Name on Window Title.
   GetViewByName( &vLPL, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vLPL )
   {
      GetAddrForAttribute( &szLPL_Name, vLPL, "LPLR", "Name" );
      SetWindowCaptionTitle( vSubtask, 0, szLPL_Name );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Create a HK if one doesn't exist for current LPLR.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_InitHK_ForLPLR( zVIEW vSubtask )
{
   zVIEW vCM_List;
   zVIEW vKZHKEYSO;
   zVIEW vTaskLPLR;
   zCHAR  szFileSpecEnv[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR  szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   // zSHORT nRC;

   RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_GO_META );
   SetNameForView( vCM_List, "CM_List", vSubtask, zLEVEL_TASK );
#if 0
   // Get File list of Presentation Environments (only one).
   nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_HKEY_META );

   // Always have a Named List of PPE's (even though there can only be One)!
   // This is done to be consistent with other dialogs.  The only time the
   // that the named List will not exist is during a SwithLPLR.
   // This fact is used a "flag" to the ReturnFromSubWnd logic.
   nRC = SetNameForView( vCM_List, "CM_HK_List", vSubtask, zLEVEL_TASK );

   // See if one exists.
   nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );

   // If not, create one, else open it.
   if ( nRC == zCURSOR_NULL )
   {
   // oTZPESRCO_GrayOptions( vSubtask );
   }
   else
   {
      nRC = ActivateMetaOI( vSubtask, &vKZHKEYSO, vCM_List, zREFER_HKEY_META, zSINGLE );
      SetNameForView( vKZHKEYSO, "TZPESRCV", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vCtrlList, vKZHKEYSO, 0 );
      SetNameForView( vCtrlList, "TZPECTRL", vSubtask, zLEVEL_TASK );
      SetAttributeFromInteger( vKZHKEYSO, "ApplHKyDef", "CtrlFontDel", 0 );
   }
#endif

   // Get the directory.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vTaskLPLR == 0 )
   {
      MessageSend( vSubtask, "HK00101", "Hot Keys",
                   "Can't locate LPLR?",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   GetStringFromAttribute( szFileSpecEnv, zsizeof( szFileSpecEnv ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   SysConvertEnvironmentString( szFileSpec, zsizeof( szFileSpec ), szFileSpecEnv );
   strcat_s( szFileSpec, zsizeof( szFileSpec ), "\\ZEIDON.PHK" );
   TraceLineS( "Hotkey Dialog opening file: ", szFileSpec );

   // Load the HKEY object.
   if ( ActivateOI_FromFile( &vKZHKEYSO, "KZHKEYSO", vSubtask, szFileSpec,
                             zIGNORE_ERRORS | zSINGLE | zLEVEL_APPLICATION | zNOI_OKAY ) < 0 )
   {
      CreateEntity( vKZHKEYSO, "Application", zPOS_AFTER );
      CommitOI_ToFile( vKZHKEYSO, szFileSpec, zASCII );
   }

   SetNameForView( vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_TASK );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Save the HK object(both Source & Executable???)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_SaveHK_ForLPLR( zVIEW vSubtask )
{
   zPCHAR  lpGlobalHotkey;
   zUSHORT usGlobalHotkeyLth;
   LPACCEL_TABLE lpAT;
   LPACCEL_TABLE lpAT_Start;
   zCHAR   szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zVIEW   vKZHKEYSO;
   zVIEW   vKZHKEYXO;
   zVIEW   vTaskLPLR;
   zULONG  ulMaxLth;
   zSHORT  nRC;

   GetViewByName( &vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_ANY );

   // Get the directory.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vTaskLPLR == 0 )
   {
      MessageSend( vSubtask, "HK00102", "Hot Keys",
                   "Can't locate LPLR?",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   GetStringFromAttribute( szFileSpec, zsizeof( szFileSpec ), vTaskLPLR, "LPLR", "MetaSrcDir" );
   strcat_s( szFileSpec, zsizeof( szFileSpec ), "\\ZEIDON.PHK" );
   MiSetOI_ReleaseForView( vKZHKEYSO, szlReleaseCurrent );
   CommitOI_ToFile( vKZHKEYSO, szFileSpec, zASCII | zENCODE_BLOBS | zNO_NULL_STRING_TERM );

   SfAllocTaskMemory( vSubtask, (zCOREMEM) &lpGlobalHotkey, 65000 );
   lpAT = (LPACCEL_TABLE) SfLockTaskMemory( lpGlobalHotkey );
   lpAT_Start = lpAT;
   usGlobalHotkeyLth = 0;

   SfActivateSysEmptyOI( &vKZHKEYXO, "KZHKEYXO", vSubtask,
                         zSINGLE | zLEVEL_APPLICATION );
   CreateEntity( vKZHKEYXO, "App", zPOS_AFTER );
   nRC = SetEntityCursor( vKZHKEYSO, "Hotkey", 0, zPOS_FIRST,
                          0, 0, 0, 0, 0, 0 );
   while ( nRC >= 0 )
   {
      CreateEntity( vKZHKEYXO, "Hotkey", zPOS_AFTER );
      SetMatchingAttributesByName( vKZHKEYXO, "Hotkey",
                                   vKZHKEYSO, "Hotkey", zSET_ALL );
      SetAttributeFromAttribute( vKZHKEYXO, "Hotkey", "COP",
                                 vKZHKEYSO, "Operation", "Name" );
      SetAttributeFromAttribute( vKZHKEYXO, "Hotkey", "DLL",
                                 vKZHKEYSO, "Hotkey", "DLL_Name" );

      lpAT->chType = zACCEL_HOTKEY;
      ulMaxLth = sizeof( zACCEL_DEF );
      GetBlobFromAttribute( &(lpAT->ac), &ulMaxLth,
                            vKZHKEYSO, "Hotkey", "KeyCombo" );
      lpAT->nID = (zSHORT) GetRelativeEntityNumber( vKZHKEYSO,
                                                    "Hotkey", 0, 0L );
      usGlobalHotkeyLth += sizeof( zACCEL_TABLE );
      lpAT++;

      nRC = SetCursorNextEntity( vKZHKEYSO, "Hotkey", 0 );
   }

   zmemset( lpAT, 0xFF, sizeof( zACCEL_TABLE ) ); // terminator
   usGlobalHotkeyLth += sizeof( zACCEL_TABLE );
   lpAT = lpAT_Start;

   SetAttributeFromBlob( vKZHKEYXO, "App", "Hotkey",
                         lpAT, usGlobalHotkeyLth );
   SfUnlockTaskMemory( lpAT );
   SfFreeTaskMemory( lpGlobalHotkey );

   GetStringFromAttribute( szFileSpec, zsizeof( szFileSpec ), vTaskLPLR, "LPLR", "ExecDir" );
   strcat_s( szFileSpec, zsizeof( szFileSpec ), "\\ZEIDON.XKY" );
   CommitOI_ToFile( vKZHKEYXO, szFileSpec, zASCII );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Create a HK if one doesn't exist for LPLR upon entry
//             to the HK Dialog.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_PreBAppl( zVIEW vSubtask )
{
   // Call configuration management to set up the default LPL.
   InitializeLPLR( vSubtask, "" );
   zwTZHKEYAD_InitHK_ForLPLR( vSubtask );
   zwTZHKEYAD_SetTitle( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//PERATION:    zwTZHKEYAD_MainRtnFromSubWnd
//
// PURPOSE:    This Entry re-loads a HK if a SwitchLPLR has invalidated
//             the current HK.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW vKZHKEYSO;

   // Is HK Available??
   GetViewByName( &vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   if ( vKZHKEYSO == 0 )
   {
      // no - return from SwitchLPLR
      zwTZHKEYAD_InitHK_ForLPLR( vSubtask );
      zwTZHKEYAD_SetTitle( vSubtask );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZHKEYAD_SwitchLPLR
//
// PURPOSE:    This Entry switches to a new LPLR.  All named views to
//             the current LPLR file List must be droped first.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_SwitchLPLR( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vKZHKEYSO;

   // Get Access to current HK Object and prompt for Save.
   nRC = GetViewByName( &vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   if ( nRC > 0 && ObjectInstanceUpdatedFromFile( vKZHKEYSO ) == 1 )
   {
      zCHAR  szMsg[ 300 ];

      strcpy_s( szMsg, zsizeof( szMsg ), "Do you wish to save current Application Hotkeys?" );
      nRC = MessagePrompt( vSubtask, "HK00103",
                           "Hot Keys",
                            szMsg,
                            zBEEP, zBUTTONS_YESNOCANCEL,
                            zRESPONSE_NO,  0 );
      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                                  "TZHKEYAD", "Application" );
         return( 0 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
            zwTZHKEYAD_SaveHK_ForLPLR( vSubtask );
      }
   }

   // Drop object to force reloading later.
   DropObjectInstance( vKZHKEYSO );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_ExitAppl( zVIEW vSubtask )
{
   zVIEW  vKZHKEYSO;
   zSHORT nRC;

   // Get Access to current HK Object and prompt for Save.
   nRC = GetViewByName( &vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   if ( nRC > 0 && ObjectInstanceUpdatedFromFile( vKZHKEYSO ) == 1 )
   {
      zCHAR  szMsg[ 300 ];

      strcpy_s( szMsg, zsizeof( szMsg ), "Do you wish to save current Application Hotkeys?" );
      nRC = MessagePrompt( vSubtask, "HK00104",
                           "Hot Keys",
                            szMsg,
                            zBEEP, zBUTTONS_YESNOCANCEL,
                            zRESPONSE_NO,  0 );
      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                                  "TZHKEYAD", "Application" );
         return( 0 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
            zwTZHKEYAD_SaveHK_ForLPLR( vSubtask );
      }
   }

//xTerminateLPLR( vSubtask );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Create a work object of all Global Opers that are hotkey
//             candidates.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_PreBHotkeyOpers( zVIEW vSubtask )
{
   zVIEW  vKZHKEYSO;
   zSHORT nRC;

   // Create the work object.
   nRC = GetViewByName( &vKZHKEYSO, "KZHKEYSO", vSubtask, zLEVEL_ANY );
   if ( vKZHKEYSO == 0 )
   {
      MessageSend( vSubtask, "HK00105", "Hot Keys",
                   "HKEYSO not found",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZHKEYAD_IncludeOperation( zVIEW vSubtask )
{
   zVIEW vHotKeys;
   zVIEW vOperationList;
   zVIEW vOperation;

   GetViewByName( &vHotKeys, "KZHKEYSO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOperationList, "CM_List", vSubtask, zLEVEL_TASK );
   if ( ActivateMetaOI( vSubtask, &vOperation, vOperationList, zREFER_GO_META, 0 ) >= 0 )
   {
      if ( SetCursorFirstEntity( vHotKeys, "Operation", 0 ) >= zCURSOR_SET )
         ExcludeEntity( vHotKeys, "Operation", zREPOS_NONE );

      IncludeSubobjectFromSubobject( vHotKeys, "Operation",
                                     vOperation, "Operation", zPOS_FIRST );
      DropView( vOperation );
   }

   return( 0 );
}
