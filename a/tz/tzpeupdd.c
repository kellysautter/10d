//
// MODULE NAME:  TZPEUPDD  -  Presentation Environment dialog operations
// DESCRIPTION:  This is the c++ file which contains the presentation
//               environment operations
/////////////////////////////////////////////////////////////////////////////
// Copyright (c) 1998-1999 QuinSoft, Inc. All rights reserved.  Confidential
// and Proprietary material subject to license - do not reproduce or
// disclose.  This material is an unpublished work and is considered a
// trade secret belonging to the copyright holder.
/////////////////////////////////////////////////////////////////////////////
//  AUTHOR:  Gig Kirk
//  DATE:    9/30/92
//  API:     MicroSoft foundation classes and Win32
//  ENVIRONMENT: Win32
//
//

/*
CHANGE LOG
2001.10.29  DKS   Z10
   Add Property entities to permit multiple (simultaneous) ctrl update.
2001.02.15  BL    R54697
   Improve handling of return code from InitializeLPLR.
2000.10.24  SR    Z2000 Size of path variables
   Modified the size ofszFileSpec in function zwTZPEUPDD_DeletePresEnv
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.
   Adjust the size of messagetext variables.
2000.08.09  RG    Z2000
   Removed compiler warnings
2000.08.07  BL    Z10
   If PPE Tool open, do not open a second PPE Tool
2000.06.23  BL    Z10  Repository and Tools
   Completely maintained PPE Tool
   Set check out state in title. Is PPE not checked out, disable all
   menu items and Buttons for update
2000.06.13  BL    Z10
   Modified zwTZPEUPDD_PreBuildPE: if Workstation Administration open,
   do not open PPE tool
2000.02.11  BL    Z10   PPE
   Modified DeletePresEnv for delete XPE
2000.02.01  BL    Z10   PPE
   Modified oTZPESRCO_CreateNewPresEnv, oTZPESRCO_DeleteFont and
   oTZPESRCO_DeleteControlDef for delete access to Attribute CtrlFontDel
   (is deleted in LOD TZPESRCO)
2000.01.29  BL    Z10   PPE-Handling
   Modified oTZPESRCO_GrayOptions for enable or disable menu items
1999.11.11  TMV   Z10   ActiveX
   Retrieve name of script operation and pass it to GenerateActiveX_Code
1999.05.11  DKS   Z10   ActiveX
   Generating wrapper classes for ActiveX controls.
1998.12.10  DKS
   Activated save on exit processing.
*/

#define zSET_CURSOR_NEXT  1
#define zSET_CURSOR_PREV  2

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "..\zdr\Zdr.h"
#include "..\zdr\TzCtl.h"

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
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

// LAST MESSAGE ID: PE00116

zOPER_EXPORT zSHORT OPERATION
fnTZPESRCO_GetRidOfInvisible( zVIEW vSubtask, zPCHAR szEntityName,
                              zPCHAR szAttribName );

zOPER_EXPORT zSHORT OPERATION
SetTitleWithCheckOutState( zVIEW   vSubtask,
                           zPCHAR  szToolname,
                           zPCHAR  szPPEname,
                           zVIEW   vToolView,
                           zPCHAR  szEntityname,
                           zLONG   lType );

zOPER_EXPORT zLONG OPERATION
zGetStringWithoutBlank( zPCHAR   szSourceString,
                        zPCHAR   pszTargetString );

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_AcceptWindowStyle( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_AcceptSubobject( zVIEW  vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szMessageText );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptWndStyleEvent( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptFontOption( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptColorOption( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_CtrlDefOK( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_EnableCtrlForActiveX( zVIEW vSubtask );


zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptCtrlEventDef( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_MapDefOK( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_DeleteSubobject( zVIEW vSubtask,
                            zSHORT nObject,
                            zPCHAR szEntityName,
                            zSHORT nCloseWindow );

static zSHORT
zwfnTZPEUPDD_CheckDelete( zVIEW    vSubtask,
                        zVIEW    vPresEnv,
                        zPCHAR   szTag,
                        zPCHAR   szEntityName,
                        zPCHAR   szEntityName1,
                        zPCHAR   szChildName,
                        zPCHAR   szChildName1,
                        zPCHAR   szMessageEntity,
                        zPCHAR   szMessageChildEnitity,
                        zPCHAR   szMessageChildEnitity1 );

static void
zwfnTZPEUPDD_DisableAllMenuItems( zVIEW vSubtask );


/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Set the title of the window to contain the LPLR Name.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_SetTitle( zVIEW vSubtask )
{
   zVIEW vPresEnv;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vSubtask, "Presentation Environment",
                              "TZPESRCV", 0,
                               "PresEnvDef", zSOURCE_PENV_META );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:    Create a PE if one doesn't exist for current LPLR.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_InitPE_ForLPLR( zVIEW vSubtask )
{
   zVIEW vCM_List = NULL;
   zVIEW vPresEnv;
   zVIEW vWorkObj;
   zVIEW vCtrlList;
   zSHORT nRC;

   // Get file list of Presentation Environments (only one).
   if ( RetrieveViewForMetaList( vSubtask, &vCM_List, zREFER_PENV_META ) < 0 )
      return( -1 );

   // Always have a Named List of PPE's (even though there can only be One!)
   // This is done to be consistent with other dialogs.  The only time the
   // that the named List will not exist is during a SwithLPLR.
   // This fact is used a "flag" to the ReturnFromSubWnd logic.

   nRC = SetNameForView( vCM_List, "CM_PPE_List", vSubtask, zLEVEL_TASK );

   // see if one exists
   nRC = SetCursorFirstEntity( vCM_List, "W_MetaDef", "" );

   // if not, create one, else open it
   if ( nRC == zCURSOR_NULL )
   {
   // oTZPESRCO_GrayOptions( vSubtask );
   }
   else
   {
      if ( ActivateMetaOI( vSubtask, &vPresEnv, vCM_List, zSOURCE_PENV_META, zSINGLE ) < 0 )
         return( -1 );

      SetNameForView( vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );
      CreateViewFromViewForTask( &vCtrlList, vPresEnv, 0 );
      SetNameForView( vCtrlList, "TZPECTRL", vSubtask, zLEVEL_TASK );
   }

   // Create the work object.
   ActivateEmptyObjectInstance( &vWorkObj, "TZPEWRKO", vSubtask, zSINGLE );
   SetNameForView( vWorkObj, "TZPEWRKV", vSubtask, zLEVEL_TASK );
   CreateEntity( vWorkObj, "ROOT", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//
// ENTRY:
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_PreBuildPE( zVIEW vSubtask )
{
   zVIEW vProfileXFER;
   zVIEW vPresEnv;

   // Call configuration management to set up the default LPL
   if ( InitializeLPLR( vSubtask, "" ) < 0 )
      return( -1 );

   if ( zwTZPEUPDD_InitPE_ForLPLR( vSubtask ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, "", "" );
      return( 0 );
   }

   // Get the Profile information and
   // Call configuration management to set up the default LPL
   oTZ__PRFO_GetViewToProfile( &vProfileXFER, "WD", vSubtask, zCURRENT_OI );
   if ( vProfileXFER )
      SetNameForView( vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );

   // if PPE not checked out, set PPE View read only ->
   // the user cannot update the values in Detail Windows
   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) >= 0 )
   {
      if ( !ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) )
         SetViewReadOnly( vPresEnv );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZPEUPDD_MainRtnFromSubWnd
//
// PURPOSE:    This Entry re-loads a PE if a SwitchLPLR has invalidated
//             the current PE.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_MainRtnFromSubWnd( zVIEW vSubtask )
{
   zVIEW vCM_List;
   zVIEW vPresEnv;

   // is PE list available?
   GetViewByName( &vCM_List, "CM_PPE_List", vSubtask, zLEVEL_TASK );
   if ( vCM_List == 0 )
   {
      // no - return from SwitchLPLR
      // Get Current PE and drop it
      GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
      if ( vPresEnv )
      {
         DropView( vPresEnv );
      }

      // Name a List view
      zwTZPEUPDD_InitPE_ForLPLR( vSubtask );
      zwTZPEUPDD_SetTitle( vSubtask );

      // if PPE not checked out, set PPE View read only ->
      // the user cannot update the values in Detail Windows
      GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
      if ( !ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ))
         SetViewReadOnly( vPresEnv );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION:  zwTZPEUPDD_SwitchLPLR
//
// PURPOSE:    This Entry switches to a new LPLR.  All named views to
//             the current LPLR file List must be droped first.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_SwitchLPLR( zVIEW vSubtask )
{
   zSHORT nRC;
   zVIEW  vPresEnv;
   zVIEW  vCM_List;
   zCHAR  szWorkMessage[ 300 ];

   // Get access to current PE Object and prompt for Save.
   nRC = GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   if ( (nRC > 0) &&
        (ObjectInstanceUpdatedFromFile( vPresEnv ) == 1) &&
        (ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1) )
   {
      strcpy_s( szWorkMessage, sizeof( szWorkMessage ),
                "Do you wish to save current Presentation Environment?" );
      nRC = MessagePrompt( vSubtask, "PE00101",
                           "Presentation Environment",
                           szWorkMessage,
                           zBEEP, zBUTTONS_YESNOCANCEL,
                           0,      0 );
      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToTopWindow,
                                  "TZPEUPDD", "TZTEUPDD" );
         return( 0 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
            CommitMetaOI( vSubtask, vPresEnv, zSOURCE_PENV_META );
      }
   }

   // Drop view to PE CM List.
   GetViewByName( &vCM_List, "CM_PPE_List", vSubtask, zLEVEL_TASK );
   if ( vCM_List )
      DropView( vCM_List );

   // Enable Actions for double Click on List Box
   EnableAction( vSubtask, "OpenWndStyle", TRUE );
   EnableAction( vSubtask, "OpenControlDef", TRUE );

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
zwTZPEUPDD_CreateNewPresEnv( zVIEW vSubtask )
{
   zVIEW vPresEnv;
   zVIEW vCtrlList;

   ActivateEmptyMetaOI( vSubtask, &vPresEnv, zSOURCE_PENV_META, zSINGLE );
   CreateMetaEntity( vSubtask, vPresEnv, "PresEnvDef", zPOS_AFTER );
   SetAttributeFromString( vPresEnv, "PresEnvDef", "Name", "Zeidon" );
   SetNameForView( vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   SetAttributeFromInteger( vPresEnv, "PresEnvDef", "LastCtrl", 0 );
   CreateViewFromViewForTask( &vCtrlList, vPresEnv, 0 );
   SetNameForView( vCtrlList, "TZPECTRL", vSubtask, zLEVEL_TASK );

   zwTZPEUPDD_SetTitle( vSubtask );

   MessageSend( vSubtask, "PE00102", "Presentation Environment",
                "A new presentation environment has been created for this LPLR.",
                zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

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
zwTZPEUPDD_DeletePresEnv( zVIEW vSubtask )
{
   zVIEW   vPresEnv;
   zVIEW   vCM_List;
   zVIEW   vLPLR;
   zSHORT  nRC;
   zCHAR   szFileSpec[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR   szERR_Msg[ zMAX_FILESPEC_LTH + zSHORT_MESSAGE_LTH + 1 ];

   nRC = MessagePrompt( vSubtask, "PE00103",
                       "Presentation Environment",
                       "Delete the Presentation Environment?",
                       zBEEP, zBUTTONS_YESNO,
                       zRESPONSE_YES,      0 );
   if ( nRC == zRESPONSE_YES )
   {
      GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
      GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
      DropMetaOI( vSubtask, vPresEnv );

      // Get File list of Presentation Environments (only one).
      nRC = RetrieveViewForMetaList( vSubtask, &vCM_List, zSOURCE_PENV_META );

      // If the user just created a PE and before saving it they are
      // deleting it, then there will be no W_MetaDef.
      if ( CheckExistenceOfEntity( vCM_List, "W_MetaDef" ) == 0 )
         DeleteMetaOI( vSubtask, vCM_List, zSOURCE_PENV_META );

      //BL, 2000.02.11 delete XPE-File
      GetStringFromAttribute( szFileSpec, sizeof( szFileSpec ), vLPLR, "LPLR", "ExecDir" );
      ofnTZCMWKSO_AppendSlash( szFileSpec );
      strcat_s( szFileSpec, sizeof( szFileSpec ), "Zeidon.xpe" );
      if ( SysOpenFile( vSubtask, szFileSpec, COREFILE_DELETE ) < 0 )
      {
         strcpy_s( szERR_Msg, sizeof( szERR_Msg ), "Corresponding Executable File " );
         strcat_s( szERR_Msg, sizeof( szERR_Msg ), szFileSpec );
         strcat_s( szERR_Msg, sizeof( szERR_Msg ), " not found." );
         MessageSend( vSubtask, "CM00458", "Configuration Management",
                      szERR_Msg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      }

      DropView( vCM_List );
      zwTZPEUPDD_SetTitle( vSubtask );
   }

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
zwTZPEUPDD_CreateNewFont( zVIEW vSubtask )
{
#if 0
   zVIEW    vPresEnv;
   zVIEW    vWorkObj;
   LOGFONT  lf;
   COLORREF clr;
   zLONG    lPointSize;
   zCHAR    szNewName[ 256 ];
   zLONG    lLastFont;
   zSHORT   nRC;

// TraceLineS( "!!! IN ", "CreateNewFont" );

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWorkObj, "TZPEWRKV", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vWorkObj, "ROOT", "NewName" );
   zGetStringWithoutBlank( szNewName, szNewName );

   if ( zstrcmp( szNewName, "") == 0 )
   {
      MessageSend( vSubtask, "ER00137", "Presentation Environment",
                   "Font Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edFontName" );
      return( -1 );
   }

   GetIntegerFromAttribute( &lLastFont, vPresEnv, "PresEnvDef", "LastFont" );
   nRC = SetCursorFirstEntityByString( vPresEnv, "Font",
                                       "Tag", szNewName, "" );

   if ( nRC == zCURSOR_SET )
   {
      MessageSend( vSubtask, "PE00104", "Presentation Environment",
                   "A Font already exists by this name!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edFontName" );
      return( -1 );
   }

   CreateMetaEntity( vSubtask, vPresEnv, "Font", zPOS_AFTER );

   SetAttributeFromString( vPresEnv, "Font", "Tag", szNewName );
   SetAttributeFromString( vWorkObj, "ROOT", "NewName", "" );

   if ( OperatorPromptForFont( vSubtask, &lf, &lPointSize,
                               (zPLONG) &clr, FALSE ) == 1 )
   {
      SetAttributeFromBlob( vPresEnv, "Font", "WindowsStructure",
                            &lf, sizeof( LOGFONT ) );
      SetAttributeFromInteger( vPresEnv, "Font", "RGB_Color", clr );
      SetAttributeFromInteger( vPresEnv, "Font", "PointSize", lPointSize );

      SetAttributeFromInteger( vPresEnv, "Font", "Id", lLastFont );
      AddToAttributeFromInteger( vPresEnv, "PresEnvDef", "LastFont", 1 );
   }

// TraceLineS( "!!! END ", "CreateNewFont" );
#endif
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
zwTZPEUPDD_UpdateFont( zVIEW vSubtask )
{
#if 0
   zVIEW    vPresEnv;
   LOGFONT  lf;
   COLORREF clr;
   zLONG    lPointSize;
   zBOOL    bUpdate = FALSE;

// TraceLineS( "!!! IN ", "UpdateFont" );

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CompareAttributeToInteger( vPresEnv, "Font", "Invisible",
                                   1 ) == 0 )
   {
      MessageSend( vSubtask, "PE00105", "Presentation Environment",
                   "This font is invisible and cannot be updated!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   GetBlobFromAttribute( &lf, 0, vPresEnv, "Font", "WindowsStructure" );
   GetIntegerFromAttribute( (zPLONG) &clr, vPresEnv, "Font", "RGB_Color" );
   GetIntegerFromAttribute( &lPointSize, vPresEnv, "Font", "PointSize" );

   if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1 )
      bUpdate = TRUE;
   if ( OperatorPromptForFont( vSubtask, &lf, &lPointSize,
                               (zPLONG) &clr, bUpdate ) == 1 )
   {
      SetAttributeFromBlob( vPresEnv, "Font", "WindowsStructure",
                            &lf, sizeof( LOGFONT ) );
      SetAttributeFromInteger( vPresEnv, "Font", "RGB_Color", clr );
      SetAttributeFromInteger( vPresEnv, "Font", "PointSize", lPointSize );
   }

// TraceLineS( "!!! END ", "CreateNewFont" );
#endif
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
zwTZPEUPDD_UpdateControlDef( zVIEW vSubtask )
{
   zVIEW vPresEnv;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CompareAttributeToInteger( vPresEnv, "ControlDef", "Invisible",
                                   1 ) == 0 )
   {
      MessageSend( vSubtask, "PE00106", "Presentation Environment",
                   "This control definition is invisible "
                   "and cannot be updated!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   CreateTemporalSubobjectVersion( vPresEnv, "ControlDef" );
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
zwTZPEUPDD_CreateNewColor( zVIEW vSubtask )
{
#if 0
   zVIEW    vPresEnv;
   zVIEW    vWorkObj;
   COLORREF clr;
   COLORREF aclrCust[ 32 ];
   zCHAR    szNewName[ 256 ];
   zSHORT   nRC;

// TraceLineS( "!!! IN ", "CreateNewColor" );

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetViewByName( &vWorkObj, "TZPEWRKV", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vWorkObj, "ROOT", "NewName" );
   zGetStringWithoutBlank( szNewName, szNewName );

   if ( zstrcmp( szNewName, "") == 0 )
   {
      MessageSend( vSubtask, "ER00137", "Presentation Environment",
                   "Color Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edColorName" );
      return( -1 );
   }

   nRC = SetCursorFirstEntityByString( vPresEnv, "Color",
                                       "Tag", szNewName, "" );

   if ( nRC == zCURSOR_SET )
   {
      MessageSend( vSubtask, "PE00107", "Presentation Environment",
                   "A Color already exists by this name!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edColorName" );
      return( -1 );
   }

   CreateMetaEntity( vSubtask, vPresEnv, "Color", zPOS_AFTER );

   SetAttributeFromString( vPresEnv, "Color", "Tag", szNewName );
   SetAttributeFromString( vWorkObj, "ROOT", "NewName", "" );

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG) aclrCust, FALSE ) == 1 )
   {
      SetAttributeFromInteger( vPresEnv, "Color", "RGB_Color", clr );
      SetAttributeFromBlob( vPresEnv, "Color", "CustomColors",
                            aclrCust, sizeof( aclrCust ) );
   }

// TraceLineS( "!!! END ", "CreateNewColor" );
#endif
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
zwTZPEUPDD_UpdateColor( zVIEW vSubtask )
{
#if 0
   zVIEW    vPresEnv;
   COLORREF clr;
   COLORREF aclrCust[ 32 ];
   zULONG   nArraySize = sizeof( COLORREF * ) * 32;
   zBOOL    bUpdate = FALSE;

// TraceLineS( "!!! IN ", "UpdateColor" );

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   GetIntegerFromAttribute( (zPLONG) &clr, vPresEnv, "Color", "RGB_Color" );
   GetBlobFromAttribute( &aclrCust, &nArraySize,
                         vPresEnv, "Color", "CustomColors" );

   if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1 )
      bUpdate = TRUE;

   if ( OperatorPromptForColor( vSubtask, (zPLONG) &clr,
                                (zPLONG) aclrCust, bUpdate ) == 1 )
   {
      SetAttributeFromInteger( vPresEnv, "Color", "RGB_Color", clr );
      SetAttributeFromBlob( vPresEnv, "Color", "CustomColors",
                            aclrCust, sizeof( aclrCust ) );
   }

// TraceLineS( "!!! END ", "UpdateColor" );
#endif
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
zwTZPEUPDD_CreateNewWindowStyle( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "WindowStyle" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptWindowStyle( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "WindowStyle", zPOS_AFTER );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );

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
zwTZPEUPDD_CreateNewEvent( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   // Accept current ControlDef and create temporal entity for new one.
   if ( CheckExistenceOfEntity( vPresEnv, "CtrlEventDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlEventDef( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlEventDef", zPOS_AFTER );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

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
zwTZPEUPDD_SelectValidChildren( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vCtrlList;
   zVIEW  vPresEnvCopy;
   zVIEW  vWindow;
   zSHORT nRC;
   zBOOL  bInclude;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCtrlList, "TZPECTRL", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );

   nRC = SetCursorFirstSelectedEntity( vCtrlList, "ControlDef", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      bInclude = TRUE;
      if ( CheckExistenceOfEntity( vPresEnv, "ValidChild" ) == zCURSOR_SET &&
           SetCursorFirstEntityByEntityCsr( vPresEnvCopy, "ValidChild",
                                            vCtrlList, "ControlDef",
                                            "" ) == zCURSOR_SET )
      {
         bInclude = FALSE;
      }

      if ( bInclude )
      {
         IncludeSubobjectFromSubobject( vPresEnv, "ValidChild",
                                        vCtrlList, "ControlDef", zPOS_AFTER );
      }

      nRC = SetCursorNextSelectedEntity( vCtrlList, "ControlDef", "" );
   }

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );

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
zwTZPEUPDD_SelectValidParents( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vCtrlList;
   zVIEW  vPresEnvCopy;
   zVIEW  vWindow;
   zSHORT nRC;
   zBOOL  bInclude;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCtrlList, "TZPECTRL", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );

   nRC = SetCursorFirstSelectedEntity( vCtrlList, "ControlDef", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      bInclude = TRUE;
      if ( CheckExistenceOfEntity( vPresEnv, "ValidParent" ) == zCURSOR_SET &&
           SetCursorFirstEntityByEntityCsr( vPresEnvCopy, "ValidParent",
                                            vCtrlList, "ControlDef",
                                            "" ) == zCURSOR_SET )
      {
         bInclude = FALSE;
      }

      if ( bInclude )
      {
         IncludeSubobjectFromSubobject( vPresEnv, "ValidParent",
                                        vCtrlList, "ControlDef", zPOS_AFTER );
      }

      nRC = SetCursorNextSelectedEntity( vCtrlList, "ControlDef", "" );
   }

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );

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
zwTZPEUPDD_DeleteConvertables( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zSHORT nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   nRC = SetCursorFirstSelectedEntity( vPresEnv, "ConvertableFrom", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      ExcludeEntity( vPresEnv, "ConvertableFrom", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vPresEnv, "ConvertableFrom", "" );
   }

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
zwTZPEUPDD_SelectConvertables( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vCtrlList;
   zVIEW  vPresEnvCopy;
   zVIEW  vWindow;
   zSHORT nRC;
   zBOOL  bInclude;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetViewByName( &vCtrlList, "TZPECTRL", vSubtask, zLEVEL_ANY );

   CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );

   nRC = SetCursorFirstSelectedEntity( vCtrlList, "ControlDef", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      bInclude = TRUE;
      if ( CheckExistenceOfEntity( vPresEnv, "ConvertableFrom" ) == zCURSOR_SET &&
           SetCursorFirstEntityByEntityCsr( vPresEnvCopy, "ConvertableFrom",
                                            vCtrlList, "ControlDef",
                                            "" ) == zCURSOR_SET )
      {
         bInclude = FALSE;
      }

      if ( bInclude )
      {
         IncludeSubobjectFromSubobject( vPresEnv, "ConvertableFrom",
                                        vCtrlList, "ControlDef", zPOS_AFTER );
      }

      nRC = SetCursorNextSelectedEntity( vCtrlList, "ControlDef", "" );
   }

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );

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
zwTZPEUPDD_DeleteValidChildren( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zSHORT nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   nRC = SetCursorFirstSelectedEntity( vPresEnv, "ValidChild", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      ExcludeEntity( vPresEnv, "ValidChild", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vPresEnv, "ValidChild", "" );
   }

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
zwTZPEUPDD_DeleteValidParents( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zSHORT nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   nRC = SetCursorFirstSelectedEntity( vPresEnv, "ValidParent", "" );
   while( nRC > zCURSOR_UNCHANGED )
   {
      ExcludeEntity( vPresEnv, "ValidParent", zREPOS_NONE );
      nRC = SetCursorNextSelectedEntity( vPresEnv, "ValidParent", "" );
   }

   return( 0 );
}


static zSHORT
zwfnTZPEUPDD_CheckDelete( zVIEW    vSubtask,
                          zVIEW    vPresEnv,
                          zPCHAR   szTag,
                          zPCHAR   szEntityName,
                          zPCHAR   szEntityName1,
                          zPCHAR   szChildName,
                          zPCHAR   szChildName1,
                          zPCHAR   szMessageEntity,
                          zPCHAR   szMessageChildEnitity,
                          zPCHAR   szMessageChildEnitity1 )
{
   zVIEW  vPresEnvCopy;
   zCHAR  szMessageText[ 254 ];
   zCHAR  szParentTag[ 33 ];
   zCHAR  szDefTag[ 33 ];
   zCHAR  szMessageParentEntity[50];
   zSHORT nRC;
   zBOOL  bDeleteError = FALSE;

   CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );

   nRC = SetCursorFirstEntity( vPresEnvCopy, "WindowStyle", "" );
   while ( nRC == zCURSOR_SET && !bDeleteError )
   {
      nRC = SetCursorFirstEntity( vPresEnvCopy, szEntityName, "" );
      while ( nRC == zCURSOR_SET && !bDeleteError )
      {
         if ( SetCursorFirstEntityByString( vPresEnvCopy, szChildName, "Tag", szTag, "" ) == zCURSOR_SET )
         {
            bDeleteError = TRUE;
            strcpy_s( szMessageParentEntity, sizeof( szMessageParentEntity ), "Window Style" );
            GetStringFromAttribute( szParentTag, sizeof( szParentTag ), vPresEnvCopy, "WindowStyle", "Tag" );
            GetStringFromAttribute( szDefTag, sizeof( szDefTag ), vPresEnvCopy, szEntityName, "Tag" );
         }

         nRC = SetCursorNextEntity( vPresEnvCopy, szEntityName, "" );
      }
      nRC = SetCursorNextEntity( vPresEnvCopy, "WindowStyle", "" );
   }

   if ( !bDeleteError )
   {
      nRC = SetCursorFirstEntity( vPresEnvCopy, "ControlDef", "" );
      while ( nRC == zCURSOR_SET && !bDeleteError )
      {
         nRC = SetCursorFirstEntity( vPresEnvCopy, szEntityName1, "" );
         while ( nRC == zCURSOR_SET && !bDeleteError )
         {
            if ( SetCursorFirstEntityByString( vPresEnvCopy, szChildName1,
                                               "Tag", szTag, "" ) == zCURSOR_SET )
            {
               bDeleteError = TRUE;
               strcpy_s( szMessageParentEntity, sizeof( szMessageParentEntity ), "Control Definition" );
               GetStringFromAttribute( szParentTag, sizeof( szParentTag ), vPresEnvCopy, "ControlDef", "Tag" );
               strcpy_s( szMessageChildEnitity, sizeof( szMessageChildEnitity ), szMessageChildEnitity1 );
               GetStringFromAttribute( szDefTag, sizeof( szDefTag ), vPresEnvCopy, szEntityName1, "Tag" );
            }

            nRC = SetCursorNextEntity( vPresEnvCopy, szEntityName1, "" );
         }
         nRC = SetCursorNextEntity( vPresEnvCopy, "ControlDef", "" );
      }
   }

   if ( bDeleteError )
   {
      strcpy_s( szMessageText, sizeof( szMessageText ), "The " );
      strcat_s( szMessageText, sizeof( szMessageText ), szMessageEntity );
      strcat_s( szMessageText, sizeof( szMessageText ), " cannot be removed, because it " );
      strcat_s( szMessageText, sizeof( szMessageText ), "is referenced by \n" );
      strcat_s( szMessageText, sizeof( szMessageText ), szMessageParentEntity );
      strcat_s( szMessageText, sizeof( szMessageText ), " '" );
      strcat_s( szMessageText, sizeof( szMessageText ), szParentTag );
      strcat_s( szMessageText, sizeof( szMessageText ), "', " );
      strcat_s( szMessageText, sizeof( szMessageText ), szMessageChildEnitity );
      strcat_s( szMessageText, sizeof( szMessageText ), " ' " );
      strcat_s( szMessageText, sizeof( szMessageText ), szDefTag );
      strcat_s( szMessageText, sizeof( szMessageText ), "'." );
      MessageSend( vSubtask, "PE00116", "Presentation Environment",
                   szMessageText,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      DropView( vPresEnvCopy );
      return( -1 );
   }

   DropView( vPresEnvCopy );

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
zwTZPEUPDD_DeleteFont( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zLONG  lId;
   zCHAR  szTag[ 33 ];

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vPresEnv, "Font" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lId, vPresEnv, "Font", "Id" );

      // If we are positioned on the Font that was created last,
      // then just delete it.
      if ( CompareAttributeToInteger( vPresEnv, "PresEnvDef",
                                      "LastFont", lId + 1 ) == 0 )
      {
         // if Font reused in Window Style or Control Definition, do not
         // delete this Font
         GetStringFromAttribute( szTag, sizeof( szTag ), vPresEnv, "Font", "Tag" );
         if ( zwfnTZPEUPDD_CheckDelete( vSubtask, vPresEnv, szTag, "WndFontDef",
                                        "CtrlFontDef", "WndFont", "CtrlFont",
                                        "Font", "Window Font Option",
                                        "Control Font Option" ) >= 0 )
         {
            DeleteEntity( vPresEnv, "Font", zREPOS_PREV );
            AddToAttributeFromInteger( vPresEnv, "PresEnvDef", "LastFont", -1 );
         }
      }
      else
      {
         // This font is now invisible.
         SetAttributeFromInteger( vPresEnv, "Font", "Invisible", 1 );
         SetAttributeFromString( vPresEnv, "Font", "Tag", "INVISIBLE" );
      }
   }
#endif

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
zwTZPEUPDD_DeleteColor( zVIEW vSubtask )
{
#if 0
   zVIEW vPresEnv;
   zCHAR szTag[ 33 ];

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   if ( CheckExistenceOfEntity( vPresEnv, "Color" ) == zCURSOR_SET )
   {
      // if Color reused in Window Style or Control Definition, do not
      // delete this Color
      GetStringFromAttribute( szTag, sizeof( szTag ), vPresEnv, "Color", "Tag" );
      if ( zwfnTZPEUPDD_CheckDelete( vSubtask, vPresEnv, szTag, "WndColorDef",
                                     "CtrlColorDef", "WndColor", "CtrlColor",
                                     "Color", "Window Color Option",
                                     "Control Color Option" ) >= 0 )
      {
         DeleteEntity( vPresEnv, "Color", zREPOS_NEXT );
      }
   }
#endif

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
zwTZPEUPDD_DeleteControlDef( zVIEW vSubtask )
{
   zVIEW vPresEnv;
   zLONG lId;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "ControlDef" ) == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lId, vPresEnv, "ControlDef", "Id" );

      // If we are positioned on the ControlDef created last then
      // just delete it.
      if ( CompareAttributeToInteger( vPresEnv, "PresEnvDef",
                                      "LastCtrl", lId + 1 ) == 0 )
      {
            DeleteEntity( vPresEnv, "ControlDef", zREPOS_PREV );
            AddToAttributeFromInteger( vPresEnv, "PresEnvDef", "LastCtrl", -1 );
      }

      else
      {
         // This ControlDef is now invisible ... note that the invisible
         // attribute is also used to specify which ControlDef's are
         // visible only to reports ... The invisible attribute may have
         // the following values:
         //
         //   -2 - control def is visible only to the report builder
         //   -1 - control def is visible only to the painter
         //    0 - control def is visible to both painter and report builder
         //    1 - control def is invisible (to be deleted)
         //
         SetAttributeFromInteger( vPresEnv, "ControlDef", "Invisible", 1 );
         SetAttributeFromString( vPresEnv, "ControlDef", "Tag", "INVISIBLE" );
      }
   }

   return( 0 );
}


static void
zwfnTZPEUPDD_DisableAllMenuItems( zVIEW vSubtask )
{
   SetOptionState( vSubtask, "FileNew", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeletePresEnv", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "NewWndStyle", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "NewCtrlDef", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteWndStyle", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "DeleteCtrlDef", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "OpenWndStyle", zOPTION_STATUS_ENABLED, 0 );
   SetOptionState( vSubtask, "OpenCtrlDef", zOPTION_STATUS_ENABLED, 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_GrayOptions( zVIEW vSubtask )
{
   zVIEW   vPresEnv;
   zVIEW   vLPLR;
   zSHORT  nIsCheckedOut = 0;

   zwfnTZPEUPDD_DisableAllMenuItems( vSubtask );

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) < 0 )
   {
      if ( GetViewByName( &vLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) > 0 )
         SetOptionState( vSubtask, "FileNew", zOPTION_STATUS_ENABLED, 1 );

      return( 0 );
   }

   SetOptionState( vSubtask, "FileNew", zOPTION_STATUS_ENABLED, 0 );

   nIsCheckedOut = ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META );
   if ( nIsCheckedOut == 1 )
   {
      SetOptionState( vSubtask, "DeletePresEnv", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "Save", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "NewWndStyle", zOPTION_STATUS_ENABLED, 1 );
      SetOptionState( vSubtask, "NewCtrlDef", zOPTION_STATUS_ENABLED, 1 );
   }

#if 0
   if ( GetSelectStateOfEntity( vPresEnv, "Font" ) == 1 )
   {
      SetOptionState( vSubtask, "OpenFont", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteFont", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( GetSelectStateOfEntity( vPresEnv, "Color" ) == 1 )
   {
      SetOptionState( vSubtask, "OpenColor", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteColor", zOPTION_STATUS_ENABLED, 1 );
   }
#endif

   if ( GetSelectStateOfEntity( vPresEnv, "WindowStyle" ) == 1 )
   {
      SetOptionState( vSubtask, "OpenWndStyle", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteWndStyle", zOPTION_STATUS_ENABLED, 1 );
   }

   if ( GetSelectStateOfEntity( vPresEnv, "ControlDef" ) == 1 )
   {
      SetOptionState( vSubtask, "OpenCtrlDef", zOPTION_STATUS_ENABLED, 1 );
      if ( nIsCheckedOut == 1 )
         SetOptionState( vSubtask, "DeleteCtrlDef", zOPTION_STATUS_ENABLED, 1 );
   }

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
zwTZPEUPDD_SavePresEnv( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zSHORT nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   ResetView( vPresEnv );

   OrderEntityForView( vPresEnv, "ControlDef", "Tag A" );
   AssignZKeyToMetaOI( vSubtask, vPresEnv, zSOURCE_PENV_META, 0 );

#if 0
   // temporary code to get the PPE to conform to ZKey usage.
   {
      zSHORT nRC = SetCursorFirstEntity( vPresEnv, "ControlDef", "" );
      while ( nRC >= zCURSOR_SET )
      {
         SetAttributeFromAttribute( vPresEnv, "ControlDef", "Key",
                                    vPresEnv, "ControlDef", "ZKey" );

         nRC = SetCursorNextEntity( vPresEnv, "ControlDef", "" );
      }
   }
#endif

   if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ))
      nRC = CommitMetaOI( vSubtask, vPresEnv, zSOURCE_PENV_META );

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
zwTZPEUPDD_ExitPresEnv( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vWorkObj;
   zSHORT nRC;

   nRC = GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   if ( nRC == -1 )
      return( 0 );

   nRC = GetViewByName( &vWorkObj, "TZPEWRKV", vSubtask, zLEVEL_ANY );

   if ( nRC != -1 && ObjectInstanceUpdatedFromFile( vPresEnv ) &&
        ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ))
   {
      nRC = MessagePrompt( vSubtask, "PE00110",
                          "Presentation Environment",
                          "Do you wish to save the Presentation Environment?",
                          zBEEP, zBUTTONS_YESNOCANCEL,
                          0,      0 );
      if ( nRC == zRESPONSE_CANCEL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( 0 );
      }
      else
      {
         if ( nRC == zRESPONSE_YES )
         {
            zwTZPEUPDD_SavePresEnv( vSubtask );
         }

         DropMetaOI( vSubtask, vPresEnv );
         DropView( vWorkObj );
      }
   }
   else
   {
      if ( nRC != -1 )
      {
         DropMetaOI( vSubtask, vPresEnv );
         DropView( vWorkObj );
      }
   }

   TerminateLPLR( vSubtask );
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
zwTZPESRCO_DelInvisibleCtrlDef( zVIEW vSubtask )
{
   fnTZPESRCO_GetRidOfInvisible( vSubtask, "ControlDef", "LastCtrl" );
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
zwTZPESRCO_DelInvisibleFont( zVIEW vSubtask )
{
// fnTZPESRCO_GetRidOfInvisible( vSubtask, "Font", "LastFont" );
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
fnTZPESRCO_GetRidOfInvisible( zVIEW vSubtask, zPCHAR szEntityName,
                              zPCHAR szAttribName )
{
   zVIEW  vPresEnv;
   zVIEW  vTmpView;
   zLONG  lId;
   zSHORT nRC;
   zSHORT nRC2;

   // Get Access to LOD Object.
   nRC = GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   nRC = SetCursorFirstEntity( vPresEnv, szEntityName, 0 );
   while ( nRC >= zCURSOR_SET )
   {
      // If the Font or ControlDef is invisible, delete it
      if ( CompareAttributeToInteger( vPresEnv, szEntityName,
                                      "Invisible", 1 ) == 0 )
      {
         GetIntegerFromAttribute( &lId, vPresEnv, szEntityName, "Id" );
         nRC = DeleteEntity( vPresEnv, szEntityName, zREPOS_NEXT );

         CreateViewFromViewForTask( &vTmpView, vPresEnv, 0 );
         nRC2 = SetCursorFirstEntity( vTmpView, szEntityName, 0 );
         while ( nRC2 > zCURSOR_UNCHANGED )
         {
            if ( CompareAttributeToInteger( vTmpView, szEntityName,
                                            "Id", lId ) > 0 )
            {
               AddToAttributeFromInteger( vTmpView, szEntityName, "Id", -1 );
               AddToAttributeFromInteger( vPresEnv, "PresEnvDef",
                                          szAttribName, -1 );
            }

            nRC2 = SetCursorNextEntity( vTmpView, szEntityName, 0 );
         }

         DropView( vTmpView );
      }
      else
         nRC = SetCursorNextEntity( vPresEnv, szEntityName, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:      CreateEventDefEntity
//
// PURPOSE:    Create a temporal Event entity for add event processing
//
// RETURNS:    0 - Current meta entity accepted, new meta entity created
//             1 - Could not accept current CtrlEventDef entity
//            -1 - Error encountered attempting to create CtrlEventDef entity
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_CreateEventDefEntity( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vPresEnv, "EventDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptWndStyleEvent( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "EventDef", zPOS_AFTER );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_AddNewCtrlDef( zVIEW vSubtask )
{
   zVIEW  vPresEnv;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   // Accept current ControlDef and create temporal entity for new one.
   if ( CheckExistenceOfEntity( vPresEnv, "ControlDef" ) >= zCURSOR_SET )
   {
      if ( AcceptSubobject( vPresEnv, "ControlDef" ) < 0 )
      {
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   // We do not create a TemporalMetaEntity since for the Presentation
   // Environment, the ZKey is really the Key for the entity, and must
   // not change on a migrate.  This has CHANGED as of release 10 and
   // we now do create a TemporalMetaEntity!!!!
   CreateTemporalMetaEntity( vSubtask, vPresEnv, "ControlDef", zPOS_AFTER );

#if 0

   zVIEW  vPresNew;
   zCHAR  szBuffer[ 32 ];
   zLONG  lKey;
   zSHORT nRC;

   CreateViewFromViewForTask( &vPresNew, vPresEnv, 0 );

   szBuffer[ 0 ] = 0;
   while ( (nRC = OperatorPromptForInput( vSubtask,
                                          "Presentation Environment",
                                          "Enter Key for new ControlDef:\n"
                                          "   Zeidon standard: 1001-4000\n"
                                          "   ActiveX: 4001-32000\n"
                                          "   New in-house: 32001-32700",
                                          FALSE, szBuffer,
                                          sizeof( szBuffer ) )) == IDOK )
   {
      lKey = atol( szBuffer );
      if ( lKey > 1000 && lKey < 10000 &&
           SetCursorFirstEntityByInteger( vPresNew, "ControlDef", "Key",
                                          lKey, "" ) <= zCURSOR_UNCHANGED )
      {
         SetAttributeFromInteger( vPresEnv, "ControlDef", "Key", lKey );
         if ( lKey >= 4000 && lKey <= 32000 )
         {
            SetCursorFirstEntityByInteger( vPresNew, "ControlDef",
                                           "Key", 4000, "" );
            SetMatchingAttributesByName( vPresEnv, "ControlDef",
                                         vPresNew, "ControlDef", zSET_NULL );
            SetAttributeFromString( vPresEnv, "ControlDef", "Tag", "new" );
         }

         break;
      }
      else
         MessageBox( 0, "Key for ControlDef must be unique",
                     "Presentation Environment", MB_OK );
   }

   DropView( vPresNew );
   if ( nRC == IDCANCEL )
   {
      if ( DeleteEntity( vPresEnv, "ControlDef", zREPOS_PREV ) != zCURSOR_SET )
         SetCursorNextEntity( vPresEnv, "ControlDef", 0 );

      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh, 0, 0 );
   }
#endif

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
zwTZPEUPDD_CreateNewControlDef( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vPresNew;
   zVIEW  vMainWindow;
   zLONG  lKey;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( GetCtrlState( vSubtask, "ActiveX", zCONTROL_STATUS_CHECKED ) )
   {
      lKey = 4001;
   }
   else
   if ( GetCtrlState( vSubtask, "InHouse", zCONTROL_STATUS_CHECKED ) )
   {
      lKey = 32001;
   }
   else
   {
      MessageSend( vSubtask, "PE00109", "Presentation Environment",
                   "Please select the type of the new control!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 0 );
   }

   CreateViewFromViewForTask( &vPresNew, vPresEnv, 0 );
   while ( SetCursorFirstEntityByInteger( vPresNew, "ControlDef", "Key",
                                          lKey, "" ) >= zCURSOR_SET )
   {
      lKey++;
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "ControlDef", zPOS_AFTER );

   SetAttributeFromInteger( vPresEnv, "ControlDef", "Key", lKey );
   if ( lKey >= 4000 && lKey <= 32000 )
   {
      if ( SetCursorFirstEntityByInteger( vPresNew, "ControlDef",
                             "Key", 4000, "" ) >= zCURSOR_SET )
      {
         SetMatchingAttributesByName( vPresEnv, "ControlDef",
                                      vPresNew, "ControlDef", zSET_NULL );
      }
   }

   DropView( vPresNew );

   SetAttributeFromString( vPresEnv, "ControlDef", "Tag", "new" );
   SetAttributeFromString( vPresEnv, "ControlDef", "PainterDLL_Name", "TZCTL" );

   GetSubtaskForWindowName( vSubtask, &vMainWindow, "ControlDefUpdate" );
   if ( vMainWindow )
   {
      zwTZPEUPDD_EnableCtrlForActiveX( vMainWindow );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }

   GetSubtaskForWindowName( vSubtask, &vMainWindow, "TZPEUPDD" );
   if ( vMainWindow )
      RefreshWindow( vMainWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_CtrlDefOK( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zCHAR    szScript[ 33 ];
   zCHAR    szScriptDLL[ 33 ];
   zCHAR    szGUID[ 80 ];
   zLONG    lId;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   GetIntegerFromAttribute( &lId, vPresEnv, "ControlDef", "Id" );
   GetStringFromAttribute( szScript, sizeof( szScript ), vPresEnv,
                           "ControlDef", "ActiveX_Script" );
   GetStringFromAttribute( szScriptDLL, sizeof( szScriptDLL ), vPresEnv,
                           "ControlDef", "ActiveX_ScriptDLL" );
   GetStringFromAttribute( szGUID, sizeof( szGUID ), vPresEnv, "ControlDef", "GUID" );

   if ( lId && szGUID[ 0 ] )
   {
      MessageSend( vSubtask, "PE00xxx", "Presentation Environment",
                   "ActiveX Class Id incompatible with Bitmap Id",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }
   else
   if ( (szScript[ 0 ] || szGUID[ 0 ] || szScriptDLL[ 0 ]) &&
        (szScript[ 0 ] == 0 || szGUID[ 0 ] == 0 || szScriptDLL[ 0 ] == 0) )
   {
      MessageSend( vSubtask, "PE00xxx", "Presentation Environment",
                   "ActiveX Script, DLL and Class Id must all be specified",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "ControlDef",
                                    "Control Definition" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_BuildActiveX_CtrlList( zVIEW vSubtask )
{
   zVIEW     vPresEnv;
   zVIEW     vTZPEAXWO;
   zCHAR     szGUID[ 80 ];
   zLONG     lId;
   zSHORT    nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lId, vPresEnv, "ControlDef", "Id" );
   if ( lId )
   {
      MessageSend( vSubtask, "PE00xxx", "Presentation Environment",
                   "Bitmap Id incompatible with ActiveX control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   nRC = ActivateEmptyObjectInstance( &vTZPEAXWO, "TZPEAXWO",
                                      vSubtask, zSINGLE );
   nRC = SetNameForView( vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_TASK );
   BuildTZPEAXWO( vTZPEAXWO, 0, FALSE );
   CreateEntity( vTZPEAXWO, "ActiveX", zPOS_FIRST );   // blank entity

   GetStringFromAttribute( szGUID, sizeof( szGUID ), vPresEnv, "ControlDef", "GUID" );
   SetCursorFirstEntityByString( vTZPEAXWO, "ActiveX", "ClassId", szGUID, "" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_SetActiveX_GUID( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vTZPEAXWO;
   zCHAR    szGUID[ 80 ];
   zSHORT   nRC;

   GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szGUID, sizeof( szGUID ), vTZPEAXWO, "ActiveX", "ClassId" );
   if ( CompareAttributeToString( vPresEnv, "ControlDef",
                                  "GUID", szGUID ) != 0 )
   {
      SetAttributeFromString( vPresEnv, "ControlDef", "GUID", szGUID );
      if ( CompareAttributeToString( vPresEnv, "ControlDef",
                                     "Tag", "new" ) == 0 )
      {
         SetAttributeFromAttribute( vPresEnv, "ControlDef", "Tag",
                                    vTZPEAXWO, "ActiveX", "Description" );
      }

      nRC = SetCursorFirstEntity( vPresEnv, "CtrlEventDef", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         DeleteEntity( vPresEnv, "CtrlEventDef", zREPOS_FIRST );
         nRC = SetCursorFirstEntity( vPresEnv, "CtrlEventDef", 0 );
      }

      if ( szGUID[ 0 ] )
      {
         nRC = SetCursorFirstEntity( vTZPEAXWO, "Event", 0 );
         while ( nRC >= zCURSOR_SET )
         {
         // CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlEventDef", zPOS_AFTER );
            CreateMetaEntity( vSubtask, vPresEnv, "CtrlEventDef", zPOS_AFTER );
            SetAttributeFromAttribute( vPresEnv, "CtrlEventDef", "Tag",
                                       vTZPEAXWO, "Event", "Name" );
            SetAttributeFromAttribute( vPresEnv, "CtrlEventDef", "Type",
                                       vTZPEAXWO, "Event", "DispatchId" );
            nRC = SetCursorNextEntity( vTZPEAXWO, "Event", 0 );
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Select sets are used to list the Events defined to the PE as follows:
//    2 - defined to PE
//    3 - selected from list defined to PE
//    4 - defined in Registry (but not in PE)
//    5 - selected from list defined in Registry
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_BuildActiveX_EventLst( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vTZPEAXWO;
   zCHAR    szGUID[ 80 ];
   zLONG    lDispatchId;
   zSHORT   nRC;
   zBOOL    bCheckedOut = FALSE;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetIntegerFromAttribute( &lDispatchId, vPresEnv, "ControlDef", "Id" );
   if ( lDispatchId )
   {
      MessageSend( vSubtask, "PE00xxx", "Presentation Environment",
                   "Bitmap Id incompatible with ActiveX control",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( 1 );
   }

   nRC = ActivateEmptyObjectInstance( &vTZPEAXWO, "TZPEAXWO",
                                      vSubtask, zSINGLE );
   nRC = SetNameForView( vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szGUID, sizeof( szGUID ), vPresEnv, "ControlDef", "GUID" );
   BuildTZPEAXWO( vTZPEAXWO, szGUID, TRUE );
   if ( SetCursorFirstEntityByString( vTZPEAXWO, "ActiveX", "ClassId",
                                      szGUID, "" ) >= zCURSOR_SET )
   {
      zSHORT nPrevSelSet = SetSelectSetForView( vTZPEAXWO, 2 );
      SetAllSelectStatesForEntity( vTZPEAXWO, "Event", FALSE, 0 );
      SetSelectSetForView( vTZPEAXWO, 4 );
      SetAllSelectStatesForEntity( vTZPEAXWO, "Event", FALSE, 0 );

      nRC = SetCursorFirstEntity( vTZPEAXWO, "Event", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lDispatchId, vTZPEAXWO,
                                  "Event", "DispatchId" );
         if ( SetCursorFirstEntityByInteger( vPresEnv, "CtrlEventDef",
                                             "Type", lDispatchId,
                                             0 ) >= zCURSOR_SET )
         {
            SetSelectSetForView( vTZPEAXWO, 2 );
            SetSelectStateOfEntity( vTZPEAXWO, "Event", TRUE );
         }
         else
         {
            SetSelectSetForView( vTZPEAXWO, 4 );
            SetSelectStateOfEntity( vTZPEAXWO, "Event", TRUE );
         }

         nRC = SetCursorNextEntity( vTZPEAXWO, "Event", 0 );
      }

      SetSelectSetForView( vTZPEAXWO, nPrevSelSet );
      LB_SetDisplaySelectSet( vSubtask, "PE_Events", 2 );
      LB_SetDisplaySelectSet( vSubtask, "RegistryEvents", 4 );
      LB_SetSelectSelectSet( vSubtask, "PE_Events", 3 );
      LB_SetSelectSelectSet( vSubtask, "RegistryEvents", 5 );
   }
   else
   {
      MessageSend( vSubtask, "PE00xxx", "Presentation Environment",
                   "ActiveX control does not have events in Registry",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
   }

   if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1 )
         bCheckedOut = TRUE;

   EnableAction( vSubtask, "AddActiveXEventToPE", bCheckedOut );
   EnableAction( vSubtask, "DeleteFromList", bCheckedOut );
   EnableAction( vSubtask, "DeleteActiveXEventFromPE", bCheckedOut );

   return( 0 );
}


zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_AddActiveX_EventToPE( zVIEW vSubtask )
{
   zVIEW    vTZPEAXWO;
   zSHORT   nPrevSelSet;
   zSHORT   nRC;

   GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_ANY );
   nPrevSelSet = SetSelectSetForView( vTZPEAXWO, 5 );
   nRC = SetCursorFirstSelectedEntity( vTZPEAXWO, "Event", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTZPEAXWO, "Event", FALSE );
      SetSelectSetForView( vTZPEAXWO, 4 );
      SetSelectStateOfEntity( vTZPEAXWO, "Event", FALSE );
      SetSelectSetForView( vTZPEAXWO, 2 );
      SetSelectStateOfEntity( vTZPEAXWO, "Event", TRUE );
      SetSelectSetForView( vTZPEAXWO, 5 );
      nRC = SetCursorNextSelectedEntity( vTZPEAXWO, "Event", 0 );
   }

   SetSelectSetForView( vTZPEAXWO, nPrevSelSet );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_DeleteActiveX_Event( zVIEW vSubtask )
{
   zVIEW    vTZPEAXWO;
   zSHORT   nPrevSelSet;
   zSHORT   nRC;

   GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_ANY );
   nPrevSelSet = SetSelectSetForView( vTZPEAXWO, 3 );
   nRC = SetCursorFirstSelectedEntity( vTZPEAXWO, "Event", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      SetSelectStateOfEntity( vTZPEAXWO, "Event", FALSE );
      SetSelectSetForView( vTZPEAXWO, 2 );
      SetSelectStateOfEntity( vTZPEAXWO, "Event", FALSE );
      SetSelectSetForView( vTZPEAXWO, 4 );
      SetSelectStateOfEntity( vTZPEAXWO, "Event", TRUE );
      SetSelectSetForView( vTZPEAXWO, 3 );
      nRC = SetCursorNextSelectedEntity( vTZPEAXWO, "Event", 0 );
   }

   SetSelectSetForView( vTZPEAXWO, nPrevSelSet );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Select sets are used to list the Events defined to the PE as follows:
//    2 - defined to PE
//    3 - selected from list defined to PE
//    4 - defined in Registry (but not in PE)
//    5 - selected from list defined in Registry
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_SaveActiveX_EventList( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vTZPEAXWO;
   zLONG    lDispatchId;
   zSHORT   nPrevSelSet;
   zSHORT   nRC;

   GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   nPrevSelSet = SetSelectSetForView( vTZPEAXWO, 2 );
   nRC = SetCursorFirstSelectedEntity( vTZPEAXWO, "Event", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lDispatchId, vTZPEAXWO,
                               "Event", "DispatchId" );
      if ( SetCursorFirstEntityByInteger( vPresEnv, "CtrlEventDef",
                                          "Type", lDispatchId,
                                          0 ) <= zCURSOR_UNCHANGED )
      {
      // CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlEventDef", zPOS_AFTER );
         CreateMetaEntity( vSubtask, vPresEnv, "CtrlEventDef", zPOS_AFTER );
         SetAttributeFromAttribute( vPresEnv, "CtrlEventDef", "Tag",
                                    vTZPEAXWO, "Event", "Name" );
         SetAttributeFromAttribute( vPresEnv, "CtrlEventDef", "Type",
                                    vTZPEAXWO, "Event", "DispatchId" );
      }

      nRC = SetCursorNextSelectedEntity( vTZPEAXWO, "Event", 0 );
   }

   SetSelectSetForView( vTZPEAXWO, 4 );
   nRC = SetCursorFirstSelectedEntity( vTZPEAXWO, "Event", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lDispatchId, vTZPEAXWO,
                               "Event", "DispatchId" );
      if ( SetCursorFirstEntityByInteger( vPresEnv, "CtrlEventDef",
                                          "Type", lDispatchId,
                                          0 ) >= zCURSOR_SET )
      {
         DeleteEntity( vPresEnv, "CtrlEventDef", zPOS_AFTER );
      }

      nRC = SetCursorNextSelectedEntity( vTZPEAXWO, "Event", 0 );
   }

   SetSelectSetForView( vTZPEAXWO, nPrevSelSet );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_AddMapDef( zVIEW vSubtask )
{
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   // Accept current CtrlMapDef and create temporal entity for new one.
   if ( CheckExistenceOfEntity( vPresEnv, "CtrlMapDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_MapDefOK( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlMapDef", zPOS_AFTER );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_DeleteMapDef( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "MapMaintenance" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_CTRLMAPDEF,
                               "CtrlMapDef",
                               nCloseWindow );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_MapDefOK( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vPresEnvCopy;
   zCHAR    szMapDefTag[ 33 ];
   zCHAR    szMapDefType[ 33 ];
   zSHORT   nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szMapDefTag, sizeof( szMapDefTag ), vPresEnv, "CtrlMapDef", "Tag" );
   zGetStringWithoutBlank( szMapDefTag, szMapDefTag );
   if ( zstrcmp( szMapDefTag, "") == 0 )
   {
      MessageSend( vSubtask, "PE00114", "Presentation Environment",
                   "Control Mapping Name is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }
   else
   {
      SetAttributeFromString( vPresEnv, "CtrlMapDef", "Tag", szMapDefTag );
   }

   GetStringFromAttribute( szMapDefType, sizeof( szMapDefType ), vPresEnv, "CtrlMapDef", "Type" );
   if ( zstrcmp( szMapDefType, "") == 0 )
   {
      MessageSend( vSubtask, "PE00114", "Presentation Environment",
                   "Control Mapping Type is required.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "cbType" );
      return( -1 );
   }

   CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );
   nRC = SetCursorFirstEntityByString( vPresEnvCopy, "CtrlMapDef",
                                       "Tag", szMapDefTag, "" );
   nRC = SetCursorNextEntityByString( vPresEnvCopy, "CtrlMapDef",
                                      "Tag", szMapDefTag, "" );
   DropView( vPresEnvCopy );

   if ( nRC == zCURSOR_SET )
   {
      MessageSend( vSubtask, "PE00115", "Presentation Environment",
                   "A Control Mapping already exists by this name!",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }

   if ( AcceptSubobject( vPresEnv, "CtrlMapDef" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_GenerateCodePostBuild( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vTZPEAXWO;
   zCHAR    szTag[ 33 ];
   zCHAR    szClassName[ 34 ];
   zCHAR    szGUID[ 80 ];
   zCHAR    szZeidonPath[ 260 ];
   zPCHAR   pch;
   LPAPP    pApp;

   if ( GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_TASK ) <= 0 )
   {
      ActivateEmptyObjectInstance( &vTZPEAXWO, "TZPEAXWO",
                                   vSubtask, zSINGLE );
      SetNameForView( vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_TASK );
      BuildTZPEAXWO( vTZPEAXWO, 0, FALSE );
   }

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szTag, sizeof( szTag ), vPresEnv, "ControlDef", "Tag" );
   SetWindowCaptionTitle( vSubtask, 0, szTag );
   GetStringFromAttribute( szGUID, sizeof( szGUID ), vPresEnv, "ControlDef", "GUID" );
   if ( szGUID[ 0 ] == 0 ||
        SetCursorFirstEntityByString( vTZPEAXWO, "ActiveX",
                                      "ClassId", szGUID, 0 ) < zCURSOR_SET )
   {
      SetCtrlState( vSubtask, "GenerateCode", zCONTROL_STATUS_ENABLED, FALSE );
   }

   while ( (pch = zstrchr( szTag, ' ' )) != 0 )
   {
      zstrcpy( pch, pch + 1 );
   }

   szClassName[ 0 ] = 'I';
   strcpy_s( szClassName + 1, sizeof( szClassName + 1 ), szTag );
   SetCtrlText( vSubtask, "ClassName", szClassName );
   szClassName[ 32 ] = 0;
   SetCtrlTextLth( vSubtask, "ClassName", 32 );

   szTag[ 8 ] = 0;
   SetCtrlText( vSubtask, "SourceFileName", szTag );
   SetCtrlTextLth( vSubtask, "SourceFileName", 8 );

   if ( SfGetApplicationForSubtask( &pApp, vSubtask ) == 0 && pApp )
      strcpy_s( szZeidonPath, sizeof( szZeidonPath ), pApp->szLocalDir );
   else
      SysGetEnvVar( szZeidonPath, "ZEIDON", sizeof( szZeidonPath ) );

   SetCtrlText( vSubtask, "DirectoryName", szZeidonPath );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zwTZPEUPDD_GenerateCode( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zVIEW    vTZPEAXWO;
   zCHAR    szTypeLib[ 260 ];
   zCHAR    szClassName[ 33 ];
   zCHAR    szSourceFileName[ 33 ];
   zCHAR    szPathName[ 260 ];
   zCHAR    szGUID[ 80 ];
   zCHAR    szScriptOper[ 80 ];
   zPCHAR   pch;
   zSHORT   nLth;
   zSHORT   nRC;

   GetViewByName( &vTZPEAXWO, "TZPEAXWO", vSubtask, zLEVEL_ANY );
   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );
   GetStringFromAttribute( szTypeLib, sizeof( szTypeLib ), vTZPEAXWO, "ActiveX", "TypeLib" );
   GetStringFromAttribute( szScriptOper, sizeof( szScriptOper ), vPresEnv, "ControlDef", "ActiveX_Script" );

   GetCtrlText( vSubtask, "ClassName", szClassName, sizeof( szClassName ) );
   GetCtrlText( vSubtask, "SourceFileName", szSourceFileName, sizeof( szSourceFileName ) );
   GetCtrlText( vSubtask, "DirectoryName", szPathName, sizeof( szPathName ) );
   if ( (pch = zstrchr( szClassName, ' ' )) != 0 ||
        (pch = zstrchr( szSourceFileName, ' ' )) != 0 ||
        (pch = zstrchr( szPathName, ' ' )) != 0 )
   {
      MessageBox( 0, "Names cannot contain spaces", "Zeidon ActiveX", MB_OK );
      return( 0 );
   }

   nLth = (zSHORT) zstrlen( szPathName );
   if ( nLth && szPathName[ nLth - 1 ] == '\\' )
   {
      nLth--;
      szPathName[ nLth ] = 0;
   }

   GetStringFromAttribute( szGUID, sizeof( szGUID ), vPresEnv, "ControlDef", "GUID" );
   nRC = GenerateActiveX_Code( vSubtask, szTypeLib, szScriptOper,
                               szClassName, szSourceFileName, szPathName, szGUID );
   if ( nRC )
   {
      if ( nRC == 1 )
         MessageBox( 0, "Code could not be generated",
                     "Zeidon ActiveX TypeLib Error", MB_OK );
      else
      if ( nRC == 2 )
         MessageBox( 0, "File not replaced (at user request)",
                     "Zeidon ActiveX Generation Aborted", MB_OK );
      else
   // if ( nRC == 3 )
         MessageBox( 0, "Code could not be generated",
                     "Zeidon ActiveX Generation Error", MB_OK );
   }
   else
      MessageBox( 0, "Code generated successfully", "Zeidon ActiveX", MB_OK );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_LoadPopup( zVIEW vSubtask,
                      zPCHAR szEntityName,
                      zPCHAR szActionNameForUpdate,
                      zPCHAR szActionNameForNew,
                      zPCHAR szActionNameForDelete,
                      zPCHAR szPopupName )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vPresEnv;
   zBOOL  bOpen       = FALSE;
   zBOOL  bNew        = FALSE;
   zBOOL  bDelete     = FALSE;
   zBOOL  bSetVisible = FALSE;
   zSHORT nComponentIsCheckedOut = 0;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      nComponentIsCheckedOut = ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META );

      if ( CheckExistenceOfEntity( vPresEnv, szEntityName ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nComponentIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nComponentIsCheckedOut == 1 )
          bNew = TRUE;

      EnableAction( vSubtask, szActionNameForUpdate, bOpen );
      EnableAction( vSubtask, szActionNameForNew, bNew );
      EnableAction( vSubtask, szActionNameForDelete, bDelete );
#if 0
      if ( zstrcmp( szPopupName, "FontPopup" ) == 0 )
      {
         if ( bDelete == TRUE &&
              CompareAttributeToInteger( vPresEnv, "Font", "Invisible", 1 ) == 0 )
         {
            bSetVisible = TRUE;
         }
         EnableAction( vSubtask, "FontSetVisible", bSetVisible );
      }
#endif
      if ( zstrcmp( szPopupName, "ControlDefPopup" ) == 0 )
      {
         if ( bDelete == TRUE &&
              CompareAttributeToInteger( vPresEnv, "ControlDef", "Invisible", 1 ) == 0 )
         {
            bSetVisible = TRUE;
         }
         EnableAction( vSubtask, "ControlDefSetVisible", bSetVisible );
      }

      CreateTrackingPopupMenu( vSubtask, szPopupName, pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZPEUPDD_LoadPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadFontPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadFontPopup( zVIEW vSubtask )
{
// zwTZPEUPDD_LoadPopup( vSubtask, "Font", "OpenFont", "CreateNewFont",
//                       "DeleteFont", "FontPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadFontPopup



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadColorPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadColorPopup( zVIEW vSubtask )
{
// zwTZPEUPDD_LoadPopup( vSubtask, "Color", "OpenColor", "CreateNewColor",
//                       "DeleteColor", "ColorPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadColorPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadWindowStylePopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadWindowStylePopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadPopup( vSubtask, "WindowStyle", "OpenWndStyle", "CreateNewWndStyle",
                         "DeleteWndStyle", "WindowStylePopup" );

   return( 0 );
} // zwTZPEUPDD_LoadWindowStylePopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadControlDefPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadControlDefPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadPopup( vSubtask, "ControlDef", "OpenControlDef", "CreateNewControlDef",
                         "DeleteControlDef", "ControlDefPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadControlDefPopup



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadStyleColorsPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadStyleColorsPopup( zVIEW vSubtask )
{
#if 0
   zwTZPEUPDD_LoadPopup( vSubtask, "WndColorDef", "UpdateWndColorDef", "AddWndColorDef",
                         "DeleteWndColorDef", "ColorPopup" );
#endif

   return( 0 );
} // zwTZPEUPDD_LoadStyleColorsPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadStyleFontsPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadStyleFontsPopup( zVIEW vSubtask )
{
// zwTZPEUPDD_LoadPopup( vSubtask, "WndFontDef", "UpdateWndFontDef", "AddWndFontDef",
//                       "DeleteWndFontDef", "FontPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadStyleFontsPopup



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadWindowEventPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadWindowEventPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadPopup( vSubtask, "EventDef", "UpdateWndEvent", "EventAdd",
                         "DeleteWndEvent", "EventPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadWindowEventPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptWindowStyle
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptWindowStyle( zVIEW vSubtask )
{
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "WindowStyle",
                                    "Window Style" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZPEUPDD_AcceptWindowStyle


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorWindowStyle
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorWindowStyle( zVIEW vSubtask,
                                 zSHORT nCursorPosition )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "WindowStyle" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptWindowStyle( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "WindowStyle", "" );
   else
      SetCursorPrevEntity( vPresEnv, "WindowStyle", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "WindowStyle" );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZPEUPDD_SetCursorWindowStyle


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextWindowStyle
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextWindowStyle( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorWindowStyle( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextWindowStyle





/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevWindowStyle
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevWindowStyle( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorWindowStyle( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevWindowStyle

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptSubobject
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_AcceptSubobject( zVIEW  vSubtask,
                            zPCHAR szEntityName,
                            zPCHAR szMessageText )
{
   zVIEW    vPresEnv;
   zVIEW    vPresEnvCopy;
   zCHAR    szNewName[ 256 ];
   zCHAR    szMessage[ 100 ];
   zSHORT   nRC;
   zBOOL    bCheck = TRUE;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vPresEnv, szEntityName, "Tag" );
   zGetStringWithoutBlank( szNewName, szNewName );

   if ( zstrcmp( szNewName, "") == 0 )
   {
      strcpy_s( szMessage, sizeof( szMessage ), szMessageText );
      strcat_s( szMessage, sizeof( szMessage ), " Name is required.");
      MessageSend( vSubtask, "PE00114", "Presentation Environment",
                   szMessage,
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }
   else
   {
      SetAttributeFromString( vPresEnv, szEntityName, "Tag", szNewName );
   }

   if ( zstrcmp( szEntityName, "ControlDef" ) == 0 &&
        zstrcmp( szNewName, "INVISIBLE" ) == 0 )
   {
      bCheck = FALSE;
   }

   if ( bCheck )
   {
      CreateViewFromViewForTask( &vPresEnvCopy, vPresEnv, 0 );
      nRC = SetCursorFirstEntityByString( vPresEnvCopy, szEntityName,
                                          "Tag", szNewName, "" );
      nRC = SetCursorNextEntityByString( vPresEnvCopy, szEntityName,
                                         "Tag", szNewName, "" );
      DropView( vPresEnvCopy );

      if ( nRC == zCURSOR_SET )
      {
         strcpy_s( szMessage, sizeof( szMessage ), "A " );
         strcat_s( szMessage, sizeof( szMessage ), szMessageText );
         strcat_s( szMessage, sizeof( szMessage ), " already exists by this name!" );
         MessageSend( vSubtask, "PE00115", "Presentation Environment",
                      szMessage,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         SetFocusToCtrl( vSubtask, "edName" );
         return( -1 );
      }
   }

   if ( AcceptSubobject( vPresEnv, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
} // zwTZPEUPDD_AcceptSubobject


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptWndStyleEvent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptWndStyleEvent( zVIEW vSubtask )
{
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "EventDef",
                                    "Window Style Event" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZPEUPDD_AcceptWndStyleEvent


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursWndStyleEvent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursWndStyleEvent( zVIEW  vSubtask,
                                 zSHORT nCursorPosition )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "EventDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptWndStyleEvent( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "EventDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "EventDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "EventDef" );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZPEUPDD_SetCursWndStyleEvent

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextWindowStyleEvent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextWindowStyleEvent( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursWndStyleEvent( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextWindowStyleEvent


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevWindowStyleEvent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevWindowStyleEvent( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursWndStyleEvent( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevWindowStyleEvent


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_FontOptionPostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_FontOptionPostbuild( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zLONG  lEnable = 0;
   zSHORT nComponentIsCheckedOut = 0;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "Font" ) >= zCURSOR_SET )
   {
      lEnable = 1;
      if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1 )
         nComponentIsCheckedOut = 1;
   }

   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, lEnable );
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, lEnable );
   SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, nComponentIsCheckedOut );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nComponentIsCheckedOut );
#endif
   return( 0 );
} // zwTZPEUPDD_FontOptionPostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_ColorOptionPostbuild
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_ColorOptionPostbuild( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zLONG  lEnable = 0;
   zSHORT nComponentIsCheckedOut = 0;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "Color" ) >= zCURSOR_SET )
   {
      lEnable = 1;
      if ( ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META ) == 1 )
         nComponentIsCheckedOut = 1;
   }

   SetCtrlState( vSubtask, "edName", zCONTROL_STATUS_ENABLED, lEnable );
   SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, lEnable );
   SetCtrlState( vSubtask, "New", zCONTROL_STATUS_ENABLED, nComponentIsCheckedOut );
   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nComponentIsCheckedOut );
#endif
   return( 0 );
} // zwTZPEUPDD_ColorOptionPostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_CreateNewFontOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_CreateNewFontOption( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vPresEnv, "WndFontDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptFontOption( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "WndFontDef", zPOS_AFTER );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif
   return( 0 );
} // zwTZPEUPDD_CreateNewFontOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptFontOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptFontOption( zVIEW vSubtask )
{
#if 0
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "WndFontDef",
                                    "Window Font Option" ) < 0 )
   {
      return( -1 );
   }
#endif
   return( 0 );
} // zwTZPEUPDD_AcceptFontOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptColorOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptColorOption( zVIEW vSubtask )
{
#if 0
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "WndColorDef",
                                    "Window Color Option" ) < 0 )
   {
      return( -1 );
   }
#endif
   return( 0 );
} // zwTZPEUPDD_AcceptColorOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_CreateNewColorOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_CreateNewColorOption( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vPresEnv, "WndColorDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptColorOption( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "WndColorDef", zPOS_AFTER );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif
   return( 0 );
} // zwTZPEUPDD_CreateNewColorOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorFontOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorFontOption( zVIEW  vSubtask,
                                zSHORT nCursorPosition )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "WndFontDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptFontOption( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "WndFontDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "WndFontDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "WndFontDef" );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif
   return( 0 );
} // zwTZPEUPDD_SetCursorFontOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextFontOption
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextFontOption( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorFontOption( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextFontOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevFontOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevFontOption( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorFontOption( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevFontOption


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorColorOption
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorColorOption( zVIEW  vSubtask,
                                 zSHORT nCursorPosition )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "WndColorDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptColorOption( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "WndColorDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "WndColorDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "WndColorDef" );
   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif
   return( 0 );
} // zwTZPEUPDD_SetCursorColorOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextColorOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextColorOption( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorColorOption( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextColorOption


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevColorOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevColorOption( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorColorOption( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevColorOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteSubobject
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_DeleteSubobject( zVIEW vSubtask,
                            zSHORT nObject,
                            zPCHAR szEntityName,
                            zSHORT nCloseWindow )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;
   zSHORT nRC;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   nRC = IssueToolMsg( vSubtask, zTOOLMSG_DELETE, zTOOL_PENV,
                       nObject,
                       vPresEnv, szEntityName, "Tag", 0 );

   if ( nRC == zRESPONSE_YES )
   {
      nRC = DeleteEntity( vPresEnv, szEntityName, zREPOS_PREV );
      if ( nCloseWindow == 1 &&
           CheckExistenceOfEntity( vPresEnv, szEntityName ) == zCURSOR_NULL )
      {
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
      }
      else
      {
         SetFocusToCtrl( vSubtask, "edName" );
      }
      GetParentWindow( &vMainWindow, vSubtask );
      RefreshWindow( vMainWindow );
   }

   return( 0 );
} // zwTZPEUPDD_DeleteSubobject


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteWindowStyle
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteWindowStyle( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "WndStyleUpdate" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_WINDOWSTYLE,
                               "WindowStyle",
                               nCloseWindow );

   return( 0 );
} // zwTZPEUPDD_DeleteWindowStyle


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteWindowEvent
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteWindowEvent( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "WndEventMaintenance" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_STYLEEVENT,
                               "EventDef",
                               nCloseWindow );

   return( 0 );
} // zwTZPEUPDD_DeleteWindowEvent

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteColorOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteColorOption( zVIEW vSubtask )
{
#if 0
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "UpdateWndColor" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_COLOROPTION,
                               "WndColorDef",
                               nCloseWindow );
#endif
   return( 0 );
} // zwTZPEUPDD_DeleteColorOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteFontOption
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteFontOption( zVIEW vSubtask )
{
#if 0
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "UpdateWndFont" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_FONTOPTION,
                               "WndFontDef",
                               nCloseWindow );
#endif
   return( 0 );
} // zwTZPEUPDD_DeleteFontOption

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_EnableCtrlForActiveX
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_EnableCtrlForActiveX( zVIEW vSubtask )
{
   zVIEW    vPresEnv;
   zLONG    lKey;
   zSHORT   nEnable = 0;
   zSHORT   nCheckedOut = 0;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   // if Control a ActiveX, then enable Controls
   GetIntegerFromAttribute( &lKey, vPresEnv, "ControlDef", "Key" );
   if ( lKey >= 4000 && lKey <= 32000 )
      nEnable = 1;

   SetCtrlState( vSubtask, "SelectActiveX_GUID", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "GenerateCode", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "ActiveX_Script", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "ActiveX_Script_DLL", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "SelectActiveX_GUID", zCONTROL_STATUS_ENABLED, nEnable );
   SetCtrlState( vSubtask, "GenerateCode", zCONTROL_STATUS_ENABLED, nEnable );

   EnableAction( vSubtask, "SelectActiveX_Events", nEnable );

   nCheckedOut = ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META );
   if ( nEnable == 1 )
   {
      SetCtrlState( vSubtask, "SelectActiveX_GUID", zCONTROL_STATUS_ENABLED, nCheckedOut );
      SetCtrlState( vSubtask, "GenerateCode", zCONTROL_STATUS_ENABLED, nCheckedOut );
   }

   return( 0 );
} // zwTZPEUPDD_EnableCtrlForActiveX

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_CrtlDefPostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_CrtlDefPostbuild( zVIEW vSubtask )
{
   zSHORT  nCheckedOut = 0;
   zVIEW   vPresEnv;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   zwTZPEUPDD_EnableCtrlForActiveX( vSubtask );

   nCheckedOut = ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META );

   SetCtrlState( vSubtask, "Delete", zCONTROL_STATUS_ENABLED, nCheckedOut );
   SetCtrlState( vSubtask, "AcceptAllParents", zCONTROL_STATUS_ENABLED, nCheckedOut );
   SetCtrlState( vSubtask, "ParentRequired", zCONTROL_STATUS_ENABLED, nCheckedOut );
   SetCtrlState( vSubtask, "AcceptAllChildren", zCONTROL_STATUS_ENABLED, nCheckedOut );

   return( 0 );
} // zwTZPEUPDD_CrtlDefPostbuild


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorControlDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorControlDef( zVIEW  vSubtask,
                                zSHORT nCursorPosition )
{
   zVIEW  vPresEnv;
   zVIEW  vMainWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "ControlDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_CtrlDefOK( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "ControlDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "ControlDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "ControlDef" );

   zwTZPEUPDD_EnableCtrlForActiveX( vSubtask );

   GetParentWindow( &vMainWindow, vSubtask );
   RefreshWindow( vMainWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZPEUPDD_SetCursorControlDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextControlDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextControlDef( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorControlDef( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextControlDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevControlDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevControlDef( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorControlDef( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevControlDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptCtrlEventDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptCtrlEventDef( zVIEW vSubtask )
{
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "CtrlEventDef",
                                    "Control Event" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
} // zwTZPEUPDD_AcceptCtrlEventDef



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorEventDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorEventDef( zVIEW vSubtask,
                              zSHORT nCursorPosition )
{
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlEventDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlEventDef( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "CtrlEventDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "CtrlEventDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "CtrlEventDef" );

   zwTZPEUPDD_EnableCtrlForActiveX( vSubtask );

   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );

   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZPEUPDD_SetCursorEventDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextCtrlEventDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextCtrlEventDef( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorEventDef( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextCtrlEventDef



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevCtrlEventDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevCtrlEventDef( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorEventDef( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevCtrlEventDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadCtrlEventPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadCtrlEventPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadPopup( vSubtask, "CtrlEventDef", "UpdateEventDef", "AddEvent",
                         "DeleteEvent", "EventPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadCtrlEventPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteCtrlEventDef
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteCtrlEventDef( zVIEW vSubtask )
{
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "EventMaintenance" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_CTRLEVENT,
                               "CtrlEventDef",
                               nCloseWindow );

   return( 0 );
} // zwTZPEUPDD_DeleteCtrlEventDef


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadMappingPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadMappingPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadPopup( vSubtask, "CtrlMapDef", "UpdateMapDef", "AddMapDef",
                         "DeleteMapDef", "MappingPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadMappingPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorControlMapp
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorControlMapp( zVIEW  vSubtask,
                                 zSHORT nCursorPosition )
{
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlMapDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_MapDefOK( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "CtrlMapDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "CtrlMapDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "CtrlMapDef" );
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );

   return( 0 );
} // zwTZPEUPDD_SetCursorControlMapp


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextControlMapping
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextControlMapping( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorControlMapp( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextControlMapping

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevControlMapping
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevControlMapping( zVIEW vSubtask )
{
   zwTZPEUPDD_SetCursorControlMapp( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevControlMapping

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadCtrlFontPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadCtrlFontPopup( zVIEW vSubtask )
{
// zwTZPEUPDD_LoadPopup( vSubtask, "CtrlFontDef", "UpdateFontOption", "AddFontOption",
//                       "DeleteFontOption", "FontPopup" );
   return( 0 );
} // zwTZPEUPDD_LoadCtrlFontPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptCtrlFont( zVIEW vSubtask )
{
// if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
//                                  "CtrlFontDef",
//                                  "Control Font Option" ) < 0 )
// {
//    return( -1 );
// }
   return( 0 );
} // zwTZPEUPDD_AcceptCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_CreateNewCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_CreateNewCtrlFont( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlFontDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlFont( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlFontDef", zPOS_AFTER );
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif

   return( 0 );
} // zwTZPEUPDD_CreateNewCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteCtrlFont( zVIEW vSubtask )
{
#if 0
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "AddFontArea" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_CTRLFONT,
                               "CtrlFontDef",
                               nCloseWindow );
#endif

   return( 0 );
} // zwTZPEUPDD_DeleteCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorCtrlFont( zVIEW  vSubtask,
                              zSHORT nCursorPosition )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlFontDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlFont( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "CtrlFontDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "CtrlFontDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "CtrlFontDef" );
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif

   return( 0 );
} // zwTZPEUPDD_SetCursorCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextCtrlFont( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorCtrlFont( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevCtrlFont
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevCtrlFont( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorCtrlFont( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevCtrlFont

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadCtrlColorPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadCtrlColorPopup( zVIEW vSubtask )
{
// zwTZPEUPDD_LoadPopup( vSubtask, "CtrlColorDef", "UpdateColorOption",
//                       "AddColorOption", "DeleteColorOption", "ColorPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadCtrlColorPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_AcceptCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_AcceptCtrlColor( zVIEW vSubtask )
{
#if 0
   if ( zwTZPEUPDD_AcceptSubobject( vSubtask,
                                    "CtrlColorDef",
                                    "Control Color Option" ) < 0 )
   {
      return( -1 );
   }
#endif

   return( 0 );
} // zwTZPEUPDD_AcceptCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_CreateNewCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_CreateNewCtrlColor( zVIEW vSubtask )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlColorDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlColor( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   CreateTemporalMetaEntity( vSubtask, vPresEnv, "CtrlColorDef", zPOS_AFTER );
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif

   return( 0 );
} // zwTZPEUPDD_CreateNewCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_DeleteCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_DeleteCtrlColor( zVIEW vSubtask )
{
#if 0
   zVIEW  vWindow;
   zSHORT nCloseWindow = 0;

   GetSubtaskForWindowName( vSubtask, &vWindow, "AddColorArea" );

   if ( vWindow )
      nCloseWindow = 1;

   zwTZPEUPDD_DeleteSubobject( vSubtask,
                               zENAME_PPE_CTRLCOLOR,
                               "CtrlColorDef",
                               nCloseWindow );
#endif

   return( 0 );
} // zwTZPEUPDD_DeleteCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_SetCursorCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
zwTZPEUPDD_SetCursorCtrlColor( zVIEW  vSubtask,
                               zSHORT nCursorPosition )
{
#if 0
   zVIEW  vPresEnv;
   zVIEW  vWindow;

   GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY );

   if ( CheckExistenceOfEntity( vPresEnv, "CtrlColorDef" ) >= zCURSOR_SET )
   {
      if ( zwTZPEUPDD_AcceptCtrlColor( vSubtask ) < 0 )
      {
         return( -1 );
      }
   }

   if ( nCursorPosition == zSET_CURSOR_NEXT )
      SetCursorNextEntity( vPresEnv, "CtrlColorDef", "" );
   else
      SetCursorPrevEntity( vPresEnv, "CtrlColorDef", "" );

   CreateTemporalSubobjectVersion( vPresEnv, "CtrlColorDef" );
   GetParentWindow( &vWindow, vSubtask );
   RefreshWindow( vWindow );
   SetFocusToCtrl( vSubtask, "edName" );
#endif

   return( 0 );
} // zwTZPEUPDD_SetCursorCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_NextCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_NextCtrlColor( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorCtrlColor( vSubtask, zSET_CURSOR_NEXT );

   return( 0 );
} // zwTZPEUPDD_NextCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PrevCtrlColor
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PrevCtrlColor( zVIEW vSubtask )
{
// zwTZPEUPDD_SetCursorCtrlColor( vSubtask, zSET_CURSOR_PREV );

   return( 0 );
} // zwTZPEUPDD_PrevCtrlColor

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadValidChildPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadValidPopup( zVIEW   vSubtask,
                           zPCHAR  szEntityName,
                           zPCHAR  szActionForSelect,
                           zPCHAR  szActionForDelete,
                           zPCHAR  szPopupName )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vPresEnv;
   zBOOL  bDelete = FALSE;
   zSHORT nComponentIsCheckedOut = 0;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      nComponentIsCheckedOut = ComponentIsCheckedOut( vSubtask, vPresEnv, zSOURCE_PENV_META );

      if ( CheckExistenceOfEntity( vPresEnv, szEntityName ) >= zCURSOR_SET )
      {
         if ( nComponentIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      EnableAction( vSubtask, szActionForSelect, nComponentIsCheckedOut );
      EnableAction( vSubtask, szActionForDelete, bDelete );

      CreateTrackingPopupMenu( vSubtask, szPopupName, pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
} // zwTZPEUPDD_LoadValidChildPopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadValidChildPopup
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadValidChildPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadValidPopup( vSubtask, "ValidChild", "SelectControls",
                              "DeleteChild", "ChildPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadValidChildPopup



/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadValidParentPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadValidParentPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadValidPopup( vSubtask, "ValidParent", "SelectControls",
                              "DeleteParent", "ParentPopup" );

   return( 0 );
} // zwTZPEUPDD_LoadValidParentPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_LoadConvertablesPopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_LoadConvertablesPopup( zVIEW vSubtask )
{
   zwTZPEUPDD_LoadValidPopup( vSubtask, "ConvertableFrom", "SelectControls",
                              "DeleteConvertables", "ConvertablePopup" );

   return( 0 );
} // zwTZPEUPDD_LoadConvertablesPopup


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_FontSetVisible
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_FontSetVisible( zVIEW vSubtask )
{
#if 0
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      if ( CompareAttributeToInteger( vPresEnv, "Font", "Invisible", 1 ) == 0 )
      {
         SetAttributeFromString( vPresEnv, "Font", "Invisible", "" );
         if ( CompareAttributeToString( vPresEnv, "Font", "Tag", "INVISIBLE" ) == 0 )
            SetAttributeFromString( vPresEnv, "Font", "Tag", "VISIBLE" );
      }
   }
#endif
   return( 0 );
} // zwTZPEUPDD_FontSetVisible


/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_ControlDefSetVisible
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_ControlDefSetVisible( zVIEW vSubtask )
{
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      if ( CompareAttributeToInteger( vPresEnv, "ControlDef", "Invisible", 1 ) == 0 )
      {
         SetAttributeFromString( vPresEnv, "ControlDef", "Invisible", "" );
         if ( CompareAttributeToString( vPresEnv, "ControlDef", "Tag", "INVISIBLE" ) == 0 )
            SetAttributeFromString( vPresEnv, "ControlDef", "Tag", "VISIBLE" );
      }
   }

   return( 0 );
} // zwTZPEUPDD_ControlDefSetVisible

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PropertyAdd
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PropertyAdd( zVIEW vSubtask )
{
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      CreateTemporalMetaEntity( vSubtask, vPresEnv, "PropertyDef", zPOS_AFTER );
   }

   return( 0 );
} // zwTZPEUPDD_PropertyAdd

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PropertyUpdate
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PropertyUpdate( zVIEW vSubtask )
{
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      CreateTemporalSubobjectVersion( vPresEnv, "PropertyDef" );
   }

   return( 0 );
} // zwTZPEUPDD_PropertyUpdate

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PropertyOK
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PropertyOK( zVIEW vSubtask )
{
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
   {
      AcceptSubobject( vPresEnv, "PropertyDef" );
   }

   return( 0 );
} // zwTZPEUPDD_PropertyOK

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZPEUPDD_PropertyCancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZPEUPDD_PropertyCancel( zVIEW vSubtask )
{
   zVIEW    vPresEnv;

   if ( GetViewByName( &vPresEnv, "TZPESRCV", vSubtask, zLEVEL_ANY ) > 0 )
      CancelSubobject( vPresEnv, "PropertyDef" );

   return( 0 );

} // zwTZPEUPDD_PropertyCancel

#ifdef __cplusplus
}
#endif
