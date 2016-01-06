/*
CHANGE LOG
2002.06.10 BL  R55732
   Modified wTZOPRUSD_TransferToEditor to fix the following problem
   in the Dialog Painter:
   1. Load a Window A in the Dialog Painter
   2. Use the 3. ComboBox to goto Editor.
   3. Zeidon ask for save. If you press the Cancel-Button,
      the Window A is disabled.
2002.03.12 FH
   avoid "ViewIsReadOnly" if a meta is not checked out.
2002.02.27 DKS
   Added more information when action cannot be deleted.
2000.11.01 BL
   Modified wTZOPRUSD_DestroyWindow for SetFocus to Action Detail
   Window
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
2000.09.29  BL     Z10
   Added Code for Move selected Operations from a Source File
   about Source File List
2000.09.24  BL     Z10
   Modified MoveOperation for reuse ZKey
2000.09.17  HH
   do not show generated operations in list
2000.09.04  BL     Z10
   Modified wTZOPRUSD_MoveOperation, fix for following Bug:
   If an Operation is moved any assignement between this Operation
   and an Action is lost.
2000.09.02  BL     Z10
   Added wTZOPRUSD_DestroyWindow: do not call a termination with
   Return
2000.08.31  BL     Z10  TB 50677
   Modified wTZOPRUSD_VerifyParms for bugfix Parameter Name for
   Entity Constraint Operation
2000.08.18  HH     Z10
   Allow return types different from short, for LOCAL operations.
2000.08.23  BL     Z10
   Remove workaround for Repository and Tools (with comment BLX)
2000.08.16  BL     Z10
   Code with comment BLXx is a workaround for Repository and Tools, if
   a Dialog not checked out
2000.07.26  BL
   If LOD or Dialog not checked out, disable all functions for update
   Added wTZOPRUSD_MovePostbuild, wTZOPRUSD_MoveCheckFileType,
   wTZOPRUSD_MoveOperationToFile, wTZOPRUSD_CreateDeleteLOD,
   wTZOPRUSD_SourceFileCancel, wTZOPRUSD_LoadSourceFileDetail,
   wTZOPRUSD_MoveOperationCancel, wTZOPRUSD_MoveRtnFromSubwindow,
   wTZOPRUSD_DeleteOperFromFile, wTZOPRUSD_RemoveMultiSelection and
   modified wTZOPRUSD_MoveOperation for move Operation with Operation
   Code to new Source File
2000.07.15  BL
   Added wTZOPRUSD_AcceptSubobject, wTZOPRUSD_SourceFilePostbuild,
   wTZOPRUSD_DisableSourceFile, wTZOPRUSD_NewOperation,
   wTZOPRUSD_DeleteCurrentOperation, wTZOPRUSD_CheckActionForDeleteOp,
   wTZOPRUSD_AddOperationForNewFile, wTZOPRUSD_DeleteParameter
   for completely maintened Operations and source files
2000.04.11  DC   Z10
   Modified SaveOperation so that 22 byte restriction below is only for
   LOD operations.
2000.04.11  HH
   fix wTZOPRUSD_MoveOperation for operations with no params.
2000.03.12  DKS  Z10
   Restrict operation name to 22 bytes for VML operation.
1999.11.29  DonC Z10
   Modified wTZOPRUSD_CheckExistsSourceFile to only execute for Object
   operations.
1999.10.25  BL  Z10    new menu structure
   Added Operation wTZOPRUSD_DeleteAllOperations for delete all Operations
   Added Operation wTZOPRUSD_DeleteSelectedOperat for delete all selected
   Operations
   Added Operation wTZOPRUSD_LoadSourceFiles for set selection in window
   <Source File List>
1999.10.24  BL  Z10    new menu structure
   Modified Operation wTZOPRUSD_GoToUpdateOperation and wTZOPRUSD_SaveOperation
   for set selection in window <Operation List>
   Modified Operation wTZOPRUSD_MoveOperation and fnBuildOperationList
   for move all selected Operations
   Added Operation wTZOPRUSD_DeleteAllFiles for delete all Source Files
   Modified Operation wTZOPRUSD_DeleteSourceFile for delete all selected
   Source Files
1999.10.12  BL  Z10    new menu structure
   Insert Operation wTZOPRUSD_CheckExistsSourceFile
   if Source File not exists, then create new Source File by add Operation
1999.07.12  DC 9J+Z10  QS999
   Modified wTZOPRUSD_TransferToEditor so that CANCEL would work
   correctly.
///// START OF OLD ORDERING
13.07.1997  DonC
   Modified wTZOPRUSD_SetUpTZOPRUSO_View so that no action is taken when
   there is no position on a window.
27.08.1997  DonC
   Added operations UpdatePrevOperation and UpdateNextOperation to correct
   Next and Previous actions when stepping through operations.
16.12.1997  DonC
   Modified UpdatePrevOperation and UpdateNextOperation to check for
   AcceptSubobject return code.
05.02.1998  DonC
   Added operations ParameterAccept, ParameterNext and ParameterPrevious
   to make sure the Parameter.PFlag attribute is set for string parameters.
20.03.1998  DonC
   Modified DeleteOperation to not alter cursor position of zWINDOW view.
1998.09.28  DKS
   Modified Dialog to work from TZWINDOWL instead of TZWINDOW view (XC 249
   and XC 261).
1998.10.01  DKS
   Modified operation InvokeEditor to use TZWINDOWL instead of TZWINDOW.
1998/10/20  DC
   Modified wTZOPRUSD_DeleteOperation to eliminate duplicate prompt for
   deletion.  Also changed several operations to use TZWINDOWL view
   instead of TZWINDOW.
1998.11.02 TMV
   Changing functioncalls of TransferToEditor because of a new argument
   Invoke the editor to let it delete the operation from source
   using some features of regular expression search
   instead of calling DeleteOperationFromSourceFile
   which damages the source under some circumstances
1998.11.23 DC/HH
   Modified wTZOPRUSD_ParameterAccept to eliminate code that forces the
   "Return value" flag for a string parameter.
   According to the changes for bug TB370, Don's change (05.02.1998)
   was de-activated, so that string parameters are no more forced
   to have PFlag on. See TZOGSRCO.VML.
1999/02/11 DC
   Modified wTZOPRUSD_TransferToEditor to prompt operator to update a meta
   if it has been modified prior to going to Editor. (TB 514)
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

#include "tzctlopr.h"

// LAST MESSAGE ID: OP00820

zPCHAR szlName            = "Name";
zPCHAR szlSourceFile      = "SourceFile";
zPCHAR szlZKey            = "ZKey";

zOPER_EXPORT zLONG  OPERATION
fnPainterCall( zSHORT nMsg,    zVIEW vSubtask,
               zVIEW  vDialog, zPCHAR szParm, zLONG lMaxLth );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_NeedParmOfType( zVIEW vSubtask,
                          zVIEW vMETAOBJECT,
                          zSHORT nCurrCsr,
                          zPCHAR szZType,
                          zPCHAR szDesc );
zOPER_EXPORT zSHORT OPERATION
LOCAL_DeleteRest( zVIEW vSOURCE );

zOPER_EXPORT zSHORT OPERATION
fnBuildOperationList( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
wTZOPRUSD_AcceptSubobject( zVIEW  vSubtask,
                           zPCHAR szEntityName,
                           zPCHAR AttributeName,
                           zPCHAR szMessageText );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_DisableSourceFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
wTZOPRUSD_CheckActionForDeleteOp( zVIEW  vSubtask,
                                  zVIEW  vDialog,
                                  zLONG  lCallByDialog,
                                  zPCHAR szName );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_VerifyParms( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveCheckFileType( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveOperationToFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteOperFromFile( zVIEW  vSubtask,
                              zSHORT MoveOrDeleteFlag );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_MoveOperationCode( zVIEW  vSubtask,
                            zSHORT nCopyOrMoveFlag );

zOPER_EXPORT zSHORT OPERATION
TZEDFRMD_DropOperationFromSource( zVIEW  vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_RemoveMultiSelection( zVIEW vSubtask );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_RemoveMultiSelByFile( zVIEW vSubtask );

zOPER_EXPORT zSHORT OPERATION
fnTZOPRUSD_MoveOperation( zVIEW vSubtask,
                          zVIEW vTargetMeta,
                          zVIEW vSourceMeta,
                          zVIEW vTZOPRUSO_Move );

///////////////////////////////////////////////////////////////////////////////
// OPERATION: fnSetButtonControls
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetButtonControls( zPCHAR szOType, zVIEW vSubtask )
{
   zVIEW  vSOURCE;
   zSHORT nIsCheckedOut = 1;

   switch ( szOType[ 0 ] )
   {
      case 'Z' : // Zeidon
      case 'G' : // Global
      case 'L' : // local
      case 'P' : // PE control
      case 'T' : // transformation
         if ( GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 )
            nIsCheckedOut = MiGetUpdateForView( vSOURCE );

         EnableAction( vSubtask, "ParmAdd", nIsCheckedOut );
         EnableAction( vSubtask, "ParmEdit", 1 );
         if ( CheckExistenceOfEntity( vSOURCE, "Parameter" ) >= zCURSOR_SET )
            EnableAction( vSubtask, "ParmDelete", nIsCheckedOut );
         else
            EnableAction( vSubtask, "ParmDelete", 0 );
         break;
      case 'D' : // dialog
      case 'K' : // shortcutkey
      case 'C' : // object constraint
      case 'E' : // entity constraint
      case 'A' : // derived attribute
      case 'M' : // domain
         EnableAction( vSubtask, "ParmAdd", 0 );
         EnableAction( vSubtask, "ParmEdit", 1 );
         EnableAction( vSubtask, "ParmDelete", 0 );
         break;
//    case 'T' : // transformation
//       EnableAction( vSubtask, "ParmAdd", 0 );
//       EnableAction( vSubtask, "ParmEdit", 1 );
//       EnableAction( vSubtask, "ParmDelete", 0 );
//       break;
      default :
         EnableAction( vSubtask, "ParmAdd", 0 );
         EnableAction( vSubtask, "ParmEdit", 1 );
         EnableAction( vSubtask, "ParmDelete", 0 );
         break;
      }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION: fnSetEditControls
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnSetEditControls( zPCHAR szOType, zVIEW vSubtask )
{
   switch ( szOType[ 0 ] )
   {
      case 'Z' : // Zeidon
      case 'G' : // Global
      case 'L' : // local
      case 'P' : // PE control
      case 'T' : // transformation ?????
         SetCtrlState( vSubtask, "EditDataType",
                          zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "EditPFlag",
                          zCONTROL_STATUS_ENABLED, 1 );
         SetCtrlState( vSubtask, "EditUFlag",
                           zCONTROL_STATUS_ENABLED, 1 );
         break;
      case 'D' : // dialog
      case 'K' : // shortcutkey constraint
      case 'C' : // object constraint
      case 'E' : // entity constraint
      case 'A' : // derived attribute
      case 'M' : // domain
      default :
         SetCtrlState( vSubtask, "EditDataType",
                          zCONTROL_STATUS_ENABLED, 0 );
         SetCtrlState( vSubtask, "EditPFlag",
                          zCONTROL_STATUS_ENABLED, 0 );
         SetCtrlState( vSubtask, "EditUFlag",
                          zCONTROL_STATUS_ENABLED, 0 );
         break;
      }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_SelectComboBox
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SelectComboBox( zVIEW vSubtask )
{
   zVIEW  vMetaView;

   // Activate either the LOD combo box or the Dialog combo box depending
   // on whether we are in the LOD dialog or the Dialog dialog. The
   // OPERUPDATE window has two combo boxes overlaying each other.  This
   // code makes the one visible and the other invisible, thereby in effect
   // activating the correct one.
   // If there is a named view "TZZOLODD", then we assume we came from the
   // LOD dialog.  Otherwise we assume we came from the Dialog dialog.


   // First, make both combo boxes invisible.

   SetCtrlState( vSubtask, "DIALOGCONTEXT", zCONTROL_STATUS_VISIBLE, 0 );
   SetCtrlState( vSubtask, "LODCONTEXT", zCONTROL_STATUS_VISIBLE, 0 );

   // Now make the correct one visible.

   if ( GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK ) < 0 )
      SetCtrlState( vSubtask, "DIALOGCONTEXT", zCONTROL_STATUS_VISIBLE, 1 );
   else
      SetCtrlState( vSubtask, "LODCONTEXT", zCONTROL_STATUS_VISIBLE, 1 );

   wTZOPRUSD_DisableSourceFile( vSubtask );

   if ( ComponentIsCheckedOut( vSubtask, vMetaView, zSOURCE_LOD_META ) == 1 )
      wTZOPRUSD_VerifyParms( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SetCtrlState( zVIEW vSubtask )
{
   zVIEW  vSOURCE;
   zCHAR  szOType[ 3 ];

   // before this gets called
   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szOType, sizeof( szOType ), vSOURCE, "Operation", "Type" );
   fnSetButtonControls( szOType, vSubtask );
   fnSetEditControls( szOType, vSubtask );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_VerifyParms
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_VerifyParms( zVIEW vSubtask )
{
   zVIEW  vSOURCE;
   zVIEW  vSOURCE_Copy;
   zCHAR  szOperType[ 2 ];
   zSHORT nNextParm;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vSOURCE ) == 0)
      return( 0 );

   CreateViewFromViewForTask( &vSOURCE_Copy, vSOURCE, 0 );

   GetStringFromAttribute( szOperType, sizeof( szOperType ), vSOURCE_Copy, "Operation", "Type" );

   switch ( szOperType[ 0 ] )
   {
      case 'Z' : // Zeidon
      case 'G' : // Global
      case 'L' : // local
      case 'P' : // PE control
         fnSetButtonControls( szOperType, vSubtask );
         break;
      case 'D' : // dialog
      case 'K' : // shortcutkey
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         // be sure the first is a view
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V",
                                               "View To Window" );
         LOCAL_DeleteRest( vSOURCE_Copy );
         break;
      case 'T' : // transformation
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V",
                                               "View To Instance" );
         break;
      case 'C' : // object constraint
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         // be sure the first is a view
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V",
                                               "View to Instance" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N", "Event" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N","State" );
         nNextParm = LOCAL_DeleteRest( vSOURCE_Copy );
         break;
      case 'E' : // entity constraint
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         // be sure the first is a view
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V",
                                               "View to Instance" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S",
                                               "Name Of Entity" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N", "Event" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N", "State" );
         nNextParm = LOCAL_DeleteRest( vSOURCE_Copy );
         break;
      case 'A' : // derived attribute
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         // be sure the first is a view
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V",
                                               "View to Instance" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S",
                                               "Internal Entity Structure" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S",
                                               "Internal Attrib Structure" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N",
                                               "Get Or Set Flag" );
         nNextParm = LOCAL_DeleteRest( vSOURCE_Copy );
         break;
      case 'M' : // domain
         SetAttributeFromString( vSOURCE_Copy, "Operation", "ReturnDataType", "N" );
         fnSetButtonControls( szOperType, vSubtask );
         // be sure the first is a view
         nNextParm = SetCursorFirstEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "L", "Entry Type" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "Y", "DataType" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S", "Data" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S", "ContextName" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "V", "View" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S", "View Entity" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "S",
                                               "View Attribute" );
         nNextParm = SetCursorNextEntity( vSOURCE_Copy, "Parameter", "" );
         nNextParm = wTZOPRUSD_NeedParmOfType( vSubtask, vSOURCE_Copy,
                                               nNextParm, "N",
                                               "MaxString Len" );
         nNextParm = LOCAL_DeleteRest( vSOURCE_Copy );
         break;

      default :
         break;
   }

   DropView( vSOURCE_Copy );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_NeedParmOfType( zVIEW  vSubtask,
                          zVIEW  vSOURCE,
                          zSHORT nCurrCsr,
                          zPCHAR szZType,
                          zPCHAR szDesc )
{
   zSHORT nRetCode;
   zCHAR szCurrentType[ 2 ];

   // Create a Parameter entity of a particular type.

   nRetCode = nCurrCsr;
   if ( nRetCode == zCURSOR_SET )
   {
      // if the first parm is there, get the type, else set the type 0
      nRetCode =
         GetStringFromAttribute( szCurrentType, sizeof( szCurrentType ),
                                 vSOURCE, "Parameter", "DataType" );
      if ( zstrcmp( szCurrentType, szZType ) != 0 )
         DeleteEntity( vSOURCE, "Parameter", zREPOS_NONE );
      else
      {
         nRetCode =
            SetAttributeFromString( vSOURCE,
                                    "Parameter", "ShortDesc", szDesc );
         return( 0 );
      }
   }

   CreateMetaEntity( vSubtask, vSOURCE, "Parameter", zPOS_AFTER );
   SetAttributeFromString( vSOURCE, "Parameter", "DataType", szZType );
   SetAttributeFromString( vSOURCE, "Parameter", "ShortDesc", szDesc );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
LOCAL_DeleteRest( zVIEW vSOURCE )
{
   zSHORT nRetCode;

   nRetCode = SetCursorNextEntity( vSOURCE, "Parameter", "" );

   while ( nRetCode == zCURSOR_SET )
      nRetCode = DeleteEntity( vSOURCE, "Parameter", zREPOS_AFTER );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ReturnFromEdit( zVIEW vSubtask )
{

   fnBuildOperationList( vSubtask );
   RefreshCtrl( vSubtask, "OperationList" );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_TransferToEditor
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_TransferToEditor( zVIEW vSubtask, zLONG lInvokeType )
{
   zVIEW  vMETAOPERATIONS;
   zVIEW  vT;
   zSHORT nRC;

   //  Get view to main meta (LOD or Dialog) and source, which as of
   //  Release .9A is in the same object.
   GetViewByName( &vMETAOPERATIONS, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Check to see if a view with name "TZZOLODO" exists.  If it does, then
   // the editor is being invoked from the LOD dialog.  Otherwise it is being
   // invoked from the Dialog dialog.  Pass this info to the editor.
   nRC = 0;
   if ( GetViewByName( &vT, "TZZOLODO", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      // if LOD not checked out, does not ask for save
      if ( ComponentIsCheckedOut( vSubtask, vT, zSOURCE_LOD_META ) == 1 )
      {
         if ( MiGetUpdateForView( vT ) && ObjectInstanceUpdatedFromFile( vT ) )
         {
            nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_LOD,
                                zREFER_LOD_META, vT, "LOD", "Name", 0 );
            if ( nRC == zRESPONSE_YES )
            {
               zVIEW  vT2;
               CreateViewFromViewForTask( &vT2, vT, 0 ); // Use temp view to save pos
               CommitMetaOI( vSubtask, vT2, zSOURCE_LOD_META );
              // DropView( vT2 );
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
               return( -1 );
            }
         }
      }

      TransferToEditor( vSubtask, vMETAOPERATIONS, "ZO", lInvokeType );
   }
   else
   {
      GetViewByName( &vT, "TZWINDOWL", vSubtask, zLEVEL_TASK );

      // If Dialog is not checked out, do not ask for save.
      if ( ComponentIsCheckedOut( vSubtask, vT, zSOURCE_DIALOG_META ) == 1 )
      {
         if ( MiGetUpdateForView( vT ) && ObjectInstanceUpdatedFromFile( vT ) )
         {
            nRC = IssueToolMsg( vSubtask, zTOOLMSG_SAVE, zTOOL_DIALOG,
                                zREFER_DIALOG_META, vT, "Dialog", "Tag", 0 );
            if ( nRC == zRESPONSE_YES )
            {
               zVIEW  vT2;

            // DisplayObjectInstance( vT, 0, 0 );
               AcceptAllTemporalSubobjects( vT ); // dks 2007.04.20  need to remove any versioned entities
               CreateViewFromViewForTask( &vT2, vT, 0 ); // use temp view to retain pos in original
               CommitMetaOI( vSubtask, vT2, zSOURCE_DIALOG_META );
               DropView( vT2 );
            }
            else
            if ( nRC == zRESPONSE_CANCEL )
            {
               SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
               fnPainterCall( zMSG_ENABLEPAINTERWINDOWS, vSubtask, 0, "Y", 0 );
               wTZOPRUSD_ReturnFromEdit( vSubtask );
               return( -1 );
            }
         }
      }

      TransferToEditor( vSubtask, vMETAOPERATIONS, "WD", lInvokeType );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_InvokeEditor( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zCHAR    szName[ 34 ];

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   wTZOPRUSD_RemoveMultiSelection( vSubtask );

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "LOD" );

   // Now go set up for the editor.
   wTZOPRUSD_TransferToEditor( vSubtask, zSHOW_OPERATION );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_OperationPopup
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_OperationPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vSOURCE;
   zVIEW  vMeta;
   zBOOL  bNew          = FALSE;
   zBOOL  bOpen         = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   if ( GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = MiGetUpdateForView( vSOURCE );

      if ( CheckExistenceOfEntity( vSOURCE, "OperationList" ) >= zCURSOR_SET )
      {
         bOpen = TRUE;
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      // enable action list in Dialog Tool
      if ( GetViewByName( &vMeta, "TZWINDOWL", vSubtask, zLEVEL_TASK ) >= 0 )
         EnableAction( vSubtask, "GoToRelatedActions", bOpen );
      else
         EnableAction( vSubtask, "GoToRelatedActions", 0 );

      EnableAction( vSubtask, "GoToNewOper", bNew );
      EnableAction( vSubtask, "DeleteOperation", bDelete );
      EnableAction( vSubtask, "DeleteAllOperations", bDelete );
      EnableAction( vSubtask, "MoveOperation", bDelete );

      CreateTrackingPopupMenu( vSubtask, "OperationPopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_ParameterPopup
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ParameterPopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   CreateTrackingPopupMenu( vSubtask, "ParameterPopup", pt.x, pt.y, FALSE, FALSE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ParameterDelete( zVIEW vSubtask )
{
   zVIEW vSOURCEMETA;
   zVIEW vWork;
   zCHAR szMsg[ 80 ];

   GetViewByName( &vSOURCEMETA, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   if ( CheckExistenceOfEntity( vSOURCEMETA, "Parameter" ) >= zCURSOR_SET )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "OK to delete the parameter " );
      GetStringFromAttribute( szMsg + 27, sizeof( szMsg ) - 27, vSOURCEMETA,
                              "Parameter", "ShortDesc" );
      strcat_s( szMsg, sizeof( szMsg ), "?" );
      if (  MessagePrompt( vSubtask, "OP00804",
                           "Operations",
                           szMsg,
                           zBEEP, zBUTTONS_YESNO,
                           zRESPONSE_NO,  0 ) == zRESPONSE_YES )
      {
         if ( DeleteEntity( vSOURCEMETA, "Parameter", zREPOS_PREV )
                                                         == zCURSOR_NULL )
         {
            if ( GetSubtaskForWindowName( vSubtask, &vWork, "PARAMETEREDIT" )
                                                            >= 0 )
            {
               SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
            }
         }
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_CreateDeleteLOD
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_CreateDeleteLOD( zVIEW vSubtask )
{
   // if delete operation from source file or move operation witch
   // operation code, create a work LOD witch all deleting operations

   zVIEW    vSOURCE;
   zVIEW    vDeleteOp;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateEmptyObjectInstance( &vDeleteOp, "TZEDWDEL", vSubtask, zMULTIPLE );
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vSOURCE, "SourceFile", zSET_ALL );
      SetNameForView( vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );
   }

   // source file does not exists, create it
   if ( SetCursorFirstEntityByAttr( vDeleteOp, "SourceFile", "ZKey",
                           vSOURCE, "SourceFile", "ZKey", "" ) < zCURSOR_SET )
   {
      CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
      SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                   vSOURCE, "SourceFile", zSET_ALL );
   }

   // create Operation
   CreateEntity( vDeleteOp, "Operation", zPOS_AFTER );
   SetMatchingAttributesByName( vDeleteOp, "Operation",
                                vSOURCE, "Operation", zSET_ALL );
   return( 0 );

} // wTZOPRUSD_CreateDeleteLOD

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteOperation( zVIEW  vSubtask,
                           zSHORT nDeleteFromSourceFile )
{
   // if delete operation from source file, create a work LOD witch
   // all deleting operations

   zVIEW    vSOURCE;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // if delete from Source File, create a Work LOD with all operations
   if ( nDeleteFromSourceFile == 1 )
      wTZOPRUSD_CreateDeleteLOD( vSubtask );

   DeleteEntity( vSOURCE, "Operation", zREPOS_NONE );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_GoToUpdateOperation( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zCHAR    szName[ 34 ];

   // This operation is used to update an Operation, instead of auto
   // subobject functionality because we must take the OperationList
   // position used in the list box and transform it to a position on
   // an Operation entity under the correct SourceFile entity.

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   wTZOPRUSD_RemoveMultiSelection( vSubtask );

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName, "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName, "LOD" );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vSOURCE, "Operation" );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SaveOperation( zVIEW vSubtask )
{
   // This operation is used to save an Operation, instead of auto
   // subobject functionality because we must rebuild the OperationList
   // entities in case we are adding a new entry and we are validating the
   // uniqueness of the operation name here.

   zVIEW  vMetaView;
   zVIEW  vMetaView2;
   zCHAR  szOperationName[ 33 ];
   zCHAR  szReturnType[ 2 ];
   zCHAR  szOperationType[ 2 ];
   zCHAR  szObjectName[ 33 ];
   zSHORT nRC;

   GetViewByName( &vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Validate that the Operation Name is unique.
   // Uniqueness is determined by searching for an Operation of the same
   // name in the Operation List.  If there is one there, it must have the
   // same ZKey as the Operation we are processing, otherwise we have a
   // duplicate Operation.  The List itself should not have an existing
   // duplicate because duplication is checked for each Operation
   // added or updated.
   CreateViewFromViewForTask( &vMetaView2, vMetaView, 0 );
   GetStringFromAttribute( szOperationName, sizeof( szOperationName ),
                           vMetaView, "Operation", "Name" );

   GetStringFromAttribute( szReturnType, sizeof( szReturnType ),
                           vMetaView, "SourceFile", "LanguageType" );

   // Validate that the length of the operation name is restricted to
   // 22 characters for a VML Operation on a LOD.
   MiGetObjectNameForView( szObjectName, vMetaView );
   if ( szReturnType[ 0 ] == 'V' &&
        zstrcmp( szObjectName, "TZZOLODO" ) == 0 )
   {
      if ( zstrlen( szOperationName ) > 22 )
      {
         DropView( vMetaView2 );
         MessageSend( vSubtask, "OP00814",
                      "Operations",
                      "VML operation name restricted to 22 characters.",
                       zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         SetFocusToCtrl( vSubtask, "edName" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   nRC = SetCursorFirstEntityByString( vMetaView2, "OperationList", "Name",
                                       szOperationName, 0 );
   if ( nRC >= zCURSOR_SET )
   {
      if ( CompareAttributeToAttribute( vMetaView2, "OperationList", "ZKey",
                                        vMetaView, "Operation", "ZKey" ) == 0 )
         nRC = -1;
      else
         nRC = 0;
   }

   DropView( vMetaView2 );
   if ( nRC >= zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00809",
                   "Operations",
                   "An Operation by the same name currently exists.",
                    zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }


   // Only if it is not a LOCAL Operation:
   // Validate that the Return Code type is either blank or "short integer"
   // for a VML Operation.
   if ( szReturnType[ 0 ] == 'V' )
   {
      GetStringFromAttribute( szReturnType, sizeof( szReturnType ),
                              vMetaView, "Operation", "ReturnDataType" );
      GetStringFromAttribute( szOperationType, sizeof( szOperationType ),
                              vMetaView, "Operation", "Type" );
      if ( szReturnType[ 0 ] != 0 && szReturnType[ 0 ] != 'N' &&
           szOperationType[ 0 ] != 'L' )
      {
         MessageSend( vSubtask, "OP00811",
                     "Operations",
                     "Only Short Number is valid Return Data Type for VML Operations.",
                     zMSGQ_OBJECT_CONSTRAINT_WARNING, 0 );
         SetFocusToCtrl( vSubtask, "cbReturnDataType" );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
         return( -1 );
      }
   }

   // Make sure VerifyParms is executed to build necessary parms.
   if ( MiGetUpdateForView( vMetaView ) == 0 )
      wTZOPRUSD_VerifyParms( vSubtask );

   // Accept the current Operation subobject.
   if ( AcceptSubobject( vMetaView, "Operation" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SaveSourceFile( zVIEW vSubtask )
{
   // This operation is used to Accept a SourceFile subobject, instead of auto
   // subobject functionality because we must initialize two SourceFile
   // attributes.
   // It also includes consistency edits that should be move to the
   // TZOPRUSO LOD at some point, but are put here initially because of
   // expedience.

   zVIEW  vMetaView;
   zVIEW  vWindow;
   zCHAR  szLanguageType[ 4 ];

   GetViewByName( &vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( MiGetUpdateForView( vMetaView ) == 0 )
      return( 0 );

   // Check that the Language Type was entered.
   GetStringFromAttribute( szLanguageType, sizeof( szLanguageType ),
                           vMetaView, "SourceFile", "LanguageType" );
   if ( szLanguageType[ 0 ] == 0 )  // ??? '\0f' )
   {
      MessageSend( vSubtask, "OP00806", "Operations",
                   "A Language Type was not selected.",
                   zMSGQ_OBJECT_CONSTRAINT_WARNING, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }


   // Make sure that the Subdirectory attribute is null and that the
   // Extension attribute is set according to the Language type.
   SetAttributeFromString( vMetaView, "SourceFile", "Subdirectory", "" );
   if ( szLanguageType[ 0 ] == 'V' )
      SetAttributeFromString( vMetaView, "SourceFile", "Extension",
                              "VML" );
   else
   {
      if ( szLanguageType[ 0 ] == 'S' )
         SetAttributeFromString( vMetaView, "SourceFile", "Extension",
                                 "Scala" );
      else
      {
      if ( szLanguageType[ 0 ] == 'J' )
         SetAttributeFromString( vMetaView, "SourceFile", "Extension",
                                 "Java" );
      else
         SetAttributeFromString( vMetaView, "SourceFile", "Extension",
                                 "C" );
      }
   }

   // Accept the current SourceFile subobject.
   if ( wTZOPRUSD_AcceptSubobject( vSubtask, "SourceFile",
                                   "Name", "Source File" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vMetaView, "SourceFile", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) >= 0 )
      RefreshWindow( vWindow );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERMOVE" ) >= 0 )
      RefreshWindow( vWindow );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "MoveSelectedOperations" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

///////////////////////////////////////////////////////////////////////////////
// OPERATION:  wTZOPRUSD_SetUpTZOPRUSO_View
// PURPOSE:
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_SetUpTZOPRUSO_View( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zSHORT nRC;

   // Set up access to either the LOD object or the Dialog object, depending
   // on whether we are in the LOD dialog or the Dialog dialog.
   // Then go to build the OperationList entries.

   nRC = GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = GetViewByName( &vMetaView, "TZWINDOWL", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         // If neither view exists, then take no action.
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
         return( -1 );
      }
   }

   SetNameForView( vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Go to build the OperationList entries.
   fnBuildOperationList( vSubtask );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
fnBuildOperationList( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zVIEW  vTargetMeta;
   zSHORT nRC;
   zSHORT nRC1;
   zSHORT nIsCheckedOut;
   zBOOL  bDeleteView = TRUE;
   zCHAR  szRootName[ 34 ];
   zCHAR  szOperationName[ 33 ];
   zVIEW  vW;

   // This is the reusable routine that builds the OperationList entities.
   // It must first determine the name of the root entity, since it could be
   // either "LOD" or "Dialog", then deletes the current entries and finally
   // creates new ones.

   // Determine name of root entity.
   if ( GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK ) < 0 )
      strcpy_s( szRootName, sizeof( szRootName ), "Dialog" );
   else
      strcpy_s( szRootName, sizeof( szRootName ), "LOD" );

   // Now get the real meta view.
   GetViewByName( &vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // save Cursor Position
   szOperationName[ 0 ] = 0;
   if ( CheckExistenceOfEntity( vMetaView, "Operation" ) >= zCURSOR_SET )
      GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vMetaView, "Operation", "Name" );

   if ( GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK ) < 0 )
   {
      bDeleteView = FALSE;
      GetViewByName( &vTargetMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   }

   // If LOD or Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   nIsCheckedOut = MiGetUpdateForView( vMetaView );
   if ( nIsCheckedOut == 0 )
      SetViewUpdate( vMetaView );

   // First delete any OperationList entries that exist.
   nRC = SetCursorFirstEntity( vMetaView, "OperationList", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      nRC = ExcludeEntity( vMetaView, "OperationList", zREPOS_NEXT );
   }

   // Now, create the OperationList entries from the Operation entries and
   // sort them by Name.
   for ( nRC = SetCursorFirstEntity( vMetaView, "Operation", szRootName );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vMetaView, "Operation", szRootName ) )
   {
      zLONG lClass;
      // do not add "generated" operations to the operation list
      //  recognize by bit 1 in the Operation.Class attribute
      GetIntegerFromAttribute( &lClass, vMetaView, "Operation", "Class" );
      if ( lClass & 0x00000001 )
         continue; // generated operation, skip it

      IncludeSubobjectFromSubobject( vMetaView, "OperationList",
                                     vMetaView, "Operation", zPOS_AFTER );
      SetSelectStateOfEntity( vMetaView, "OperationList", 0 );
      //was the Operation in Operation List selected, then set the selction
      nRC1 = SetCursorFirstEntityByEntityCsr( vTargetMeta, "Operation",
                                              vMetaView, "Operation", szRootName );
      if ( nRC1 >= zCURSOR_SET )
      {
         if ( GetSelectStateOfEntity( vTargetMeta, "Operation" ) == 1 )
         {
            SetSelectStateOfEntity( vTargetMeta, "Operation", 0 );
            SetSelectStateOfEntity( vMetaView, "Operation", 0 );
            SetSelectStateOfEntity( vMetaView, "OperationList", 1 );
         }
      }
   }

   if ( nIsCheckedOut == 0 )
      SetViewReadOnly( vMetaView );

   OrderEntityForView( vMetaView, "OperationList", "Name A" );

   // set cursor on original position
   if ( GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) >= 0 )
   {
      SetCursorFirstSelectedEntity( vMetaView, "SourceFile", "" );

      if ( CheckExistenceOfEntity( vMetaView, "OperationList" ) >= zCURSOR_SET &&
           (SetCursorFirstSelectedEntity( vMetaView, "OperationList",
                                          "" ) >= zCURSOR_SET ||
            SetCursorFirstEntityByString( vMetaView, "OperationList", "Name",
                                          szOperationName, "" ) >= zCURSOR_SET) )
      {
         SetSelectStateOfEntity( vMetaView, "OperationList", 1 );
      }
   }

   if ( bDeleteView )
      DropView( vTargetMeta );

   return( 0 );
}

/*
   The main window received focus.  Make sure that there are operations to
   list.
*/
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ReceiveFocus( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zVIEW  vTemp;
   zSHORT nRC;

   // Get the meta object and then the TZOPRUSO object.  If they are
   // different, then the user has loaded a different meta, so rebuild the
   // list.

   nRC = GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
      GetViewByName( &vMetaView, "TZWINDOWL", vSubtask, zLEVEL_TASK );

   GetViewByName( &vTemp, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   if ( vTemp != vMetaView )
   {
      // Go to build the OperationList entries.
      wTZOPRUSD_SetUpTZOPRUSO_View( vSubtask );

      // Refresh the list
       RefreshCtrl( vSubtask, "OperationList" );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSourceFile( zVIEW vSubtask )
{
   // This operation is used to delete a SourceFile, instead of auto subobject
   // functionality because of validation on delete.

   zVIEW    vMeta;
   zVIEW    vW;
   zCHAR    szName[ 34 ];
   zCHAR    szMsg[ 80 ];
   zSHORT   nRC;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.24 delete all selected Source Files:
   //Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                        "OK to delete selected Source Files", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstSelectedEntity( vMeta, "SourceFile", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vMeta, "SourceFile", "" ) )
      {
         // Verify that the SourceFile has no Operations.
         if ( CheckExistenceOfEntity( vMeta, "Operation" ) >= zCURSOR_SET )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Source File '" );
            GetStringFromAttribute( szName, sizeof( szName ), vMeta, "SourceFile", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), szName );
            strcat_s( szMsg, sizeof( szMsg ), "' cannot be deleted if it has Operations." );
            MessageSend( vSubtask, "OP00802", "Operations",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         }
         else
            DeleteEntity( vMeta, "SourceFile", zREPOS_NONE );
      }
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SRCLIST" ) >= 0 )
      RefreshWindow( vW );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddSourceFile( zVIEW vSubtask )
{
   zVIEW  vMeta;

   wTZOPRUSD_RemoveMultiSelByFile( vSubtask );

   // This operation is used to add a SourceFile, instead of auto subobject
   // functionality in order to set the ZKey.
   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   CreateTemporalMetaEntity( vSubtask, vMeta, "SourceFile", zPOS_AFTER );
   SetAttributeFromString( vMeta, "SourceFile", "LanguageType", "V" );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddParameter( zVIEW vSubtask )
{
   // This operation is used to add a Parameter, instead of auto subobject
   // functionality in order to set the ZKey.

   zVIEW    vMeta;
   zVIEW    vWindow;
   zVIEW    vParentWindow;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vMeta, "Parameter" ) >= zCURSOR_SET )
   {
      if ( wTZOPRUSD_AcceptSubobject( vSubtask, "Parameter", "ShortDesc", "Parameter" ) < 0 )
         return( -1 );

      SetSelectStateOfEntity( vMeta, "Parameter", 0 );
   }

   CreateTemporalMetaEntity( vSubtask, vMeta, "Parameter", zPOS_AFTER );
   SetSelectStateOfEntity( vMeta, "Parameter", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "PARMUPDATE" ) >= 0 )
   {
      GetParentWindow ( &vParentWindow, vWindow );
      wTZOPRUSD_VerifyParms( vWindow );
      RefreshWindow( vParentWindow );
      SetFocusToCtrl( vSubtask, "edName" );
   }
   else
   {
      wTZOPRUSD_VerifyParms( vSubtask );
      RefreshWindow( vSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_AddOperation( zVIEW vSubtask )
{
   // This operation is used to add an Operation, instead of auto subobject
   // functionality in order to set the ZKey.

   zVIEW    vMeta;
   zVIEW    vParentWindow;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vMeta, "OperationList" ) == zCURSOR_SET )
      SetSelectStateOfEntity( vMeta, "OperationList", 0 );

   CreateTemporalMetaEntity( vSubtask, vMeta, "Operation", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vMeta, "OperationList",
                                  vMeta, "Operation", zPOS_AFTER );
   SetSelectStateOfEntity( vMeta, "OperationList", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vParentWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vParentWindow );

   wTZOPRUSD_DisableSourceFile( vSubtask );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_MovePrebuild( zVIEW vSubtask )
{
   // This operation creates the TZOPRUSO2 view needed by the Move window.
   // It also uses position on the OperationList entity to position on the
   // corresponding Operation entity.

   zVIEW    vSourceMeta;
   zVIEW    vTargetMeta;
   zVIEW    vTZOPRUSO_Move;

   GetViewByName( &vSourceMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK ) < 0 )
   {
      CreateViewFromViewForTask( &vTargetMeta, vSourceMeta, 0 );
      SetNameForView( vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateOI_FromOI_ForTask( &vTZOPRUSO_Move, vSourceMeta, 0, zSINGLE );
      SetNameForView( vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   }

   // Position on correct Operation entity.
   SetCursorFirstEntityByAttr( vSourceMeta, "SourceFile", "ZKey",
                               vSourceMeta, "OperSourceFile", "ZKey", 0 );
   SetCursorFirstEntityByAttr( vSourceMeta, "Operation", "ZKey",
                               vSourceMeta, "OperationList", "ZKey", 0 );

   // Position on correct Operation entity.
   SetCursorFirstEntityByAttr( vTZOPRUSO_Move, "SourceFile", "ZKey",
                               vSourceMeta, "OperSourceFile", "ZKey", 0 );
   SetCursorFirstEntityByAttr( vTZOPRUSO_Move, "Operation", "ZKey",
                               vSourceMeta, "OperationList", "ZKey", 0 );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_MoveOperation( zVIEW vSubtask )
{
   zVIEW    vSourceMeta;
   zVIEW    vTargetMeta;
   zVIEW    vTZOPRUSO_Move;

   // This operation is used to move an Operation and its parameters from
   // one SourceFile entity to another.
   // We will move the operation on which we're currently positioned in
   // TZOPRUSO to the end of the SourceFile for view, TZOPRUSO2, from
   // which the new souce entity was selected.

   if ( wTZOPRUSD_MoveCheckFileType( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSourceMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   fnTZOPRUSD_MoveOperation( vSubtask, vTargetMeta, vSourceMeta,
                             vTZOPRUSO_Move );

   DropObjectInstance( vTZOPRUSO_Move );
   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_OperActionsCreate( zVIEW vSubtask )
{
   // This operation creates temporary Actions underneath the selected
   // OperationList entity.
   // The meta here is a TZWDLGSO and we will use the entities from that LOD.

   zVIEW    vOrigMeta;
   zVIEW    vMeta;
   zCHAR    szActionname[ 33 ];
   zSHORT   nRC;
   zSHORT   nIsCheckedOut;

   GetViewByName( &vOrigMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // If LOD or Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   nIsCheckedOut = MiGetUpdateForView( vOrigMeta );
   if ( nIsCheckedOut == 0 )
      SetViewUpdate( vOrigMeta );

   CreateViewFromViewForTask( &vMeta, vOrigMeta, 0 );
   nRC = SetCursorFirstEntity( vMeta, "ActOper", "Dialog" );
   while ( nRC >= zCURSOR_SET)
   {
      // Create the OperationAction entry if the ActOper matches the OperationList.
      if ( CompareAttributeToAttribute( vMeta, "ActOper",       "ZKey",
                                        vMeta, "OperationList", "ZKey" ) == 0 )
      {
         IncludeSubobjectFromSubobject( vMeta, "OperationAction",
                                        vMeta, "Action", zPOS_AFTER );
      }

      nRC = SetCursorNextEntity( vMeta, "ActOper", "Dialog" );
   }
   DropView( vMeta );

   // If LOD or Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   if ( nIsCheckedOut == 0 )
      SetViewReadOnly( vOrigMeta );

   // set action name in the caption.
   GetStringFromAttribute( szActionname, sizeof( szActionname ), vOrigMeta, "OperationList", "Name");
   SetWindowCaptionTitle( vSubtask, 0, szActionname );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_OperActionsRemove( zVIEW vSubtask )
{
   // This operation removes temporary Actions underneath the selected
   // OperationList entity.
   // The meta here is a TZWDLGSO and we will use the entities from that LOD.

   zVIEW    vMeta;
   zSHORT   nRC;
   zSHORT   nIsCheckedOut;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // If LOD or Dialog not checked out, View is read only. To set a attribute
   // View has to be updateble. After change attribute set View read only again
   nIsCheckedOut = MiGetUpdateForView( vMeta );
   if ( nIsCheckedOut == 0 )
      SetViewUpdate( vMeta );


   nRC = SetCursorFirstEntity( vMeta, "OperationAction", "" );
   while ( nRC >= zCURSOR_SET)
   {
      ExcludeEntity( vMeta, "OperationAction", zREPOS_NONE );
      nRC = SetCursorNextEntity( vMeta, "OperationAction", "" );
   }

   if ( nIsCheckedOut == 0 )
      SetViewReadOnly( vMeta );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_UpdateNextOperation( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zVIEW    vWindow;
   zCHAR    szName[ 34 ];

   // This operation is used to update an Operation, instead of auto
   // subobject functionality because we must take the OperationList
   // position used in the list box and transform it to a position on
   // an Operation entity under the correct SourceFile entity.

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Accept the current Operation subobject.
   if ( wTZOPRUSD_SaveOperation( vSubtask ) < 0 )
      return( -1 );

   // Position on next Operation.
   SetSelectStateOfEntity( vSOURCE, "OperationList", 0 );
   SetCursorNextEntity( vSOURCE, "OperationList", 0 );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "LOD" );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vSOURCE, "Operation" );
   SetSelectStateOfEntity( vSOURCE, "OperationList", 1 );

   wTZOPRUSD_DisableSourceFile( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_UpdatePrevOperation( zVIEW vSubtask )
{
   zVIEW    vDialog;
   zVIEW    vSOURCE;
   zVIEW    vWindow;
   zCHAR    szName[ 34 ];

   // This operation is used to update an Operation, instead of auto
   // subobject functionality because we must take the OperationList
   // position used in the list box and transform it to a position on
   // an Operation entity under the correct SourceFile entity.

   // Position on the correct Operation to update through the SourceFile
   // path.  Note that the position must be set through the root, which
   // is either LOD or Dialog.

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Accept the current Operation subobject.
   if ( wTZOPRUSD_SaveOperation( vSubtask ) < 0 )
      return( -1 );

   // Position on previous Operation.
   SetSelectStateOfEntity( vSOURCE, "OperationList", 0 );
   SetCursorPrevEntity( vSOURCE, "OperationList", 0 );
   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "Dialog" );
   else
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                    "LOD" );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vSOURCE, "Operation" );
   SetSelectStateOfEntity( vSOURCE, "OperationList", 1 );

   wTZOPRUSD_DisableSourceFile( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ParameterAccept( zVIEW vSubtask )
{
   zVIEW    vSource;

   GetViewByName( &vSource, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( wTZOPRUSD_AcceptSubobject( vSubtask, "Parameter",
                                   "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ParameterNext( zVIEW vSubtask )
{
   zVIEW    vSource;
   zVIEW    vParentWindow;

   GetViewByName( &vSource, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( wTZOPRUSD_AcceptSubobject( vSubtask, "Parameter",
                                   "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vSource, "Parameter", 0 );
   SetCursorNextEntity( vSource, "Parameter", 0 );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vSource, "Parameter" );
   SetSelectStateOfEntity( vSource, "Parameter", 1 );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_ParameterPrevious( zVIEW vSubtask )
{
   zVIEW    vSource;
   zVIEW    vParentWindow;

   GetViewByName( &vSource, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   // Accept the current Parameter subobject.
   if ( wTZOPRUSD_AcceptSubobject( vSubtask, "Parameter",
                                   "ShortDesc", "Parameter" ) < 0 )
   {
      return( -1 );
   }

   SetSelectStateOfEntity( vSource, "Parameter", 0 );
   SetCursorPrevEntity( vSource, "Parameter", 0 );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalSubobjectVersion( vSource, "Parameter" );
   SetSelectStateOfEntity( vSource, "Parameter", 1 );

   GetParentWindow( &vParentWindow, vSubtask );
   RefreshWindow( vParentWindow );

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_CheckExistsSourceFile
//
// if Source File not exists, then create new Source File
//
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_CheckExistsSourceFile( zVIEW vSubtask )
{
   zVIEW vMeta;
   zVIEW vDialog;
   zVIEW vWindow;
   zCHAR szRootName[ 33 ];
   zCHAR szAttributeName[ 33 ];

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      strcpy_s( szRootName, sizeof( szRootName ), "Dialog" );
      strcpy_s( szAttributeName, sizeof( szAttributeName ), "Tag" );
   }
   else
   {
      strcpy_s( szRootName, sizeof( szRootName ), "LOD" );
      strcpy_s( szAttributeName, sizeof( szAttributeName ), "Name" );
   }

   wTZOPRUSD_RemoveMultiSelection( vSubtask );

   // The code below is only executed for Object operations.
   if ( GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      // If no SourceFile exists, create an initial SourceFile entity by the
      // same name as the LOD or Dialog.
      if ( CheckExistenceOfEntity( vMeta, "SourceFile" ) < zCURSOR_SET )
      {
         CreateMetaEntity( vSubtask, vMeta, "SourceFile", zPOS_AFTER );
         SetAttributeFromAttribute( vMeta, "SourceFile", "Name",
                                    vMeta, szRootName, szAttributeName );
         SetAttributeFromString( vMeta, "SourceFile", "Extension", "VML" );
         SetAttributeFromString( vMeta, "SourceFile", "LanguageType", "V" );

         // Refresh Window <Source File List>
         GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" );
         if ( vWindow )
            RefreshWindow( vWindow );
      }
   }

   wTZOPRUSD_AddOperation( vSubtask );

   return( 0 );

} // wTZOPRUSD_CheckExistsSourceFile

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteAllFiles
//
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteAllFiles( zVIEW vSubtask )
{
   zVIEW    vMeta;
   zVIEW    vW;
   zCHAR    szName[ 34 ];
   zCHAR    szMsg[ 80 ];
   zSHORT   nRC;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   //BL, 1999.10.24 delete all selected Source Files:
   //Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                        "OK to delete all Source Files", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      for ( nRC = SetCursorFirstEntity( vMeta, "SourceFile", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMeta, "SourceFile", "" ) )
      {
         // Verify that the SourceFile has no Operations.
         if ( CheckExistenceOfEntity( vMeta, "Operation" ) >= zCURSOR_SET )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Source File '" );
            GetStringFromAttribute( szName, sizeof( szName ), vMeta, "SourceFile", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), szName );
            strcat_s( szMsg, sizeof( szMsg ), "' cannot be deleted if it has Operations." );
            MessageSend( vSubtask, "OP00802", "Operations",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         }
         else
            DeleteEntity( vMeta, "SourceFile", zREPOS_NONE );
      }
   }

   if ( GetSubtaskForWindowName( vSubtask, &vW, "SRCLIST" ) >= 0 )
      RefreshWindow( vW );

   return( 0 );

} // wTZOPRUSD_DeleteAllFiles

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_LoadSourceFiles
//
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_LoadSourceFiles( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zSHORT nRC;

   // Set up access to either the LOD object or the Dialog object, depending
   // on whether we are in the LOD dialog or the Dialog dialog.
   // Then go to build the OperationList entries.

   nRC = GetViewByName( &vMetaView, "TZZOLODO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      nRC = GetViewByName( &vMetaView, "TZWINDOWL", vSubtask, zLEVEL_TASK );
      if ( nRC < 0 )
      {
         // If neither view exists, then take no action.
         SetWindowActionBehavior( vSubtask, zWAB_ReturnToParent, 0, 0 );
         return( -1 );
      }
   }

   SetNameForView( vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   return( 0 );

} // wTZOPRUSD_LoadSourceFiles

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteSelectedOperat
//
// delete all  selected Operation
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteSelectedOperat( zVIEW vSubtask )
{
   zVIEW    vSOURCE;
   zVIEW    vDialog = 0;
   zVIEW    vDeleteOp = 0;
   zVIEW    vW;
   zCHAR    szName[ 34 ];
   zCHAR    szRootName[ 34 ];
   zCHAR    szEntityName[ 10 ];
   zSHORT   nRC;
   zLONG    lSourceType;
   zSHORT   nDelete;
   zSHORT   nDeleteFromSourceFile;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      strcpy_s( szRootName, sizeof( szRootName ), "Dialog" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "WD" );
      lSourceType = zSOURCE_DIALOG_META;
   }
   else
   {
      strcpy_s( szRootName, sizeof( szRootName ), "LOD" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "ZO" );
      lSourceType = zSOURCE_LOD_META;
   }

   //BL, 1999.10.24 delete all selected Operations:
   //Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                        "OK to delete selected Operations?", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      if ( MessagePrompt( vSubtask, "OP00810", "Operations",
                          "Delete selected Operations from Source File?",
                          0, zBUTTONS_YESNO, zRESPONSE_YES,
                          zICON_QUESTION ) == zRESPONSE_YES )
      {
         nDeleteFromSourceFile = 1;
         if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vDeleteOp );
      }
      else
         nDeleteFromSourceFile = 0;

      for ( nRC = SetCursorFirstSelectedEntity( vSOURCE, "OperationList", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextSelectedEntity( vSOURCE, "OperationList", "" ) )
      {
         GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );
         // The following validation is executed for Dialog metas only.
         nDelete = wTZOPRUSD_CheckActionForDeleteOp( vSubtask, vDialog,
                                                     lSourceType, szName );
         if ( nDelete == 1 )
         {
            SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                          szRootName );
            wTZOPRUSD_DeleteOperation( vSubtask, nDeleteFromSourceFile );
          }
      } //end for
   } //end if ( nRC == zRESPONSE_YES )

   if ( GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) >= 0 )
      RefreshWindow( vW );

   // Delete Operation from Source File
   if ( nDeleteFromSourceFile == 1 )
      wTZOPRUSD_DeleteOperFromFile( vSubtask, zDELETE_OPERATION );

   return( 0 );

} // wTZOPRUSD_DeleteSelectedOperat

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteAllOperations
//
// delete all Operations
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteAllOperations( zVIEW vSubtask )
{
   zVIEW    vSOURCE;
   zVIEW    vDialog = 0;
   zVIEW    vDeleteOp = 0;
   zVIEW    vW;
   zCHAR    szName[ 34 ];
   zCHAR    szRootName[ 34 ];
   zCHAR    szEntityName[ 10 ];
   zSHORT   nRC;
   zLONG    lSourceType;
   zSHORT   nDelete;
   zSHORT   nDeleteFromSourceFile;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      strcpy_s( szRootName, sizeof( szRootName ), "Dialog" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "WD" );
      lSourceType = zSOURCE_DIALOG_META;
   }
   else
   {
      strcpy_s( szRootName, sizeof( szRootName ), "LOD" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "ZO" );
      lSourceType = zSOURCE_LOD_META;
   }

   //BL, 1999.10.24 delete all Operation:
   //Prompt to confirm Delete only one
   nRC = MessagePrompt( vSubtask, "", "Zeidon Objects Maintenance",
                        "OK to delete all Operations?", 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_YES )
   {
      if ( MessagePrompt( vSubtask, "OP00810", "Operations",
                          "Delete all Operations from Source File?",
                          0, zBUTTONS_YESNO, zRESPONSE_YES,
                          zICON_QUESTION ) == zRESPONSE_YES )
      {
         nDeleteFromSourceFile = 1;
         if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vDeleteOp );
      }
      else
         nDeleteFromSourceFile = 0;

      for ( nRC = SetCursorFirstEntity( vSOURCE, "OperationList", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vSOURCE, "OperationList", "" ) )
      {
         GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );

         // The following validation is executed for Dialog metas only.
         nDelete = wTZOPRUSD_CheckActionForDeleteOp( vSubtask, vDialog,
                                                     lSourceType, szName );
         if ( nDelete == 1 )
         {
            SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName,
                                          szRootName );
            wTZOPRUSD_DeleteOperation( vSubtask, nDeleteFromSourceFile );
         }
      } //end for
   } //end if ( nRC == zRESPONSE_YES )

   if ( GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) >= 0 )
      RefreshWindow( vW );

   // Delete Operation from Source File
   if ( nDeleteFromSourceFile == 1 )
      wTZOPRUSD_DeleteOperFromFile( vSubtask, zDELETE_OPERATION );

   return( 0 );

} // wTZOPRUSD_DeleteAllOperations

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_AcceptSubobject
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
wTZOPRUSD_AcceptSubobject( zVIEW  vSubtask,
                           zPCHAR szEntityName,
                           zPCHAR AttributeName,
                           zPCHAR szMessageText )
{
   zVIEW    vTZOPRUSO;
   zVIEW    vTZOPRUSO_Copy;
   zCHAR    szNewName[ 100 ];
   zCHAR    szMessage[ 200 ];
   zSHORT   nRC;

   GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szNewName, sizeof( szNewName ), vTZOPRUSO, szEntityName, AttributeName );

   CreateViewFromViewForTask( &vTZOPRUSO_Copy, vTZOPRUSO, 0 );

   nRC = SetCursorFirstEntityByString( vTZOPRUSO_Copy, szEntityName,
                                       AttributeName, szNewName, "" );
   nRC = SetCursorNextEntityByString( vTZOPRUSO_Copy, szEntityName,
                                      AttributeName, szNewName, "" );
   DropView( vTZOPRUSO_Copy );

   if ( nRC == zCURSOR_SET )
   {
      strcpy_s( szMessage, sizeof( szMessage ), "A " );
      strcat_s( szMessage, sizeof( szMessage ), szMessageText );
      strcat_s( szMessage, sizeof( szMessage ), " already exists by this name!" );
      MessageSend( vSubtask, "OP00814", "Operations",
                   szMessage, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      SetFocusToCtrl( vSubtask, "edName" );
      return( -1 );
   }

   if ( AcceptSubobject( vTZOPRUSO, szEntityName ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
      return( -1 );
   }

   return( 0 );

} // wTZOPRUSD_AcceptSubobject

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_SourceFilePostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_SourceFilePostbuild( zVIEW vSubtask )
{
   zVIEW  vMeta;
   zVIEW  vMetaCopy;
   zVIEW  vWindow;
   zSHORT nRC;

   // This operation is used to add a SourceFile, instead of auto subobject
   // functionality in order to set the ZKey.

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   CreateViewFromViewForTask( &vMetaCopy, vMeta, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vMeta, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vMeta, "SourceFile", "" ) )
   {
      nRC =SetSelectStateOfEntity( vMeta, "SourceFile", 0 );
   }

   SetCursorFirstEntityByAttr( vMeta, "SourceFile", "Name",
                               vMetaCopy, "SourceFile", "Name", "" );
   DropView( vMetaCopy );

   SetSelectStateOfEntity( vMeta, "SourceFile", 1 );

   if ( GetViewByName( &vMetaCopy, "TZOPRUSO2", vSubtask, zLEVEL_TASK ) >= 0 )
      SetCursorFirstEntityByAttr( vMetaCopy, "SourceFile", "Name",
                                  vMeta, "SourceFile", "Name", "" );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERMOVE" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "MoveSelectedOperations" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );

} // wTZOPRUSD_SourceFilePostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DisableSourceFile
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_DisableSourceFile( zVIEW vSubtask )
{
   zVIEW  vMeta;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CompareAttributeToString( vMeta, "Operation", "Name", "" ) == 0 )
   {
      SetCtrlState( vSubtask, "cbSourceFile", zCONTROL_STATUS_ENABLED, 1 );
      if ( CountEntitiesForView( vMeta, "SourceFile" ) > 1 )
         SetFocusToCtrl( vSubtask, "cbSourceFile" );
      else
         SetFocusToCtrl( vSubtask, "edName" );
   }
   else
   {
      SetCtrlState( vSubtask, "cbSourceFile", zCONTROL_STATUS_ENABLED, 0 );
      if ( !MiGetUpdateForView( vMeta ))
      {
         SetCtrlState( vSubtask, "LODCONTEXT", zCONTROL_STATUS_ENABLED, 0 );
         SetCtrlState( vSubtask, "DIALOGCONTEXT", zCONTROL_STATUS_ENABLED, 0 );
      }

      SetFocusToCtrl( vSubtask, "edName" );
   }

   return( 0 );

} // wTZOPRUSD_DisableSourceFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_NewOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_NewOperation( zVIEW vSubtask )
{
   zVIEW    vMeta;
   zVIEW    vWindow;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( wTZOPRUSD_SaveOperation( vSubtask ) < 0 )
      return( -1 );

   if ( CheckExistenceOfEntity( vMeta, "OperationList" ) == zCURSOR_SET )
      SetSelectStateOfEntity( vMeta, "OperationList", 0 );

   CreateTemporalMetaEntity( vSubtask, vMeta, "Operation", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vMeta, "OperationList",
                                  vMeta, "Operation", zPOS_AFTER );
   SetSelectStateOfEntity( vMeta, "OperationList", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   wTZOPRUSD_DisableSourceFile( vSubtask );

   return( 0 );

} // wTZOPRUSD_NewOperation

zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_CopyOperation( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zVIEW  vMetaView2;
   zCHAR  szName[ 34 ];
   zSHORT nRC;

   // This operation is used to copy an Operation.

   if ( wTZOPRUSD_SaveOperation( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vMetaView2, vMetaView, 0 );

   // Use a temporal subobject version for updating the Operation subobject.
   CreateTemporalMetaEntity( vSubtask, vMetaView, "Operation", zPOS_AFTER );
   IncludeSubobjectFromSubobject( vMetaView, "OperationList",
                                  vMetaView, "Operation", zPOS_AFTER );
   SetMatchingAttributesByName( vMetaView, "Operation",
                                vMetaView2, "Operation", zSET_NOTNULL );
   GetStringFromAttribute( szName + 1, sizeof( szName ) - 1, vMetaView, "Operation", "Name" );
   szName[ 0 ] = '_';
   szName[ 33 ] = 0;
   SetAttributeFromString( vMetaView, "Operation", "Name", szName );

   nRC = SetCursorFirstEntity( vMetaView2, "Parameter", "" );
   while ( nRC >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vMetaView, "Parameter", zPOS_AFTER );
      SetMatchingAttributesByName( vMetaView, "Parameter",
                                   vMetaView2, "Parameter", zSET_NOTNULL );
      nRC = SetCursorNextEntity( vMetaView2, "Parameter", "" );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteCurrentOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_DeleteCurrentOperation( zVIEW vSubtask )
{
   zVIEW    vSOURCE;
   zVIEW    vDialog = 0;
   zVIEW    vDeleteOp = 0;
   zVIEW    vW;
   zCHAR    szName[ 34 ];
   zCHAR    szRootName[ 34 ];
   zCHAR    szMsg[ 80 ];
   zCHAR    szEntityName[ 10 ];
   zSHORT   nRC;
   zLONG    lSourceType;
   zSHORT   nDeleteFromSourceFile = 0;

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      strcpy_s( szRootName, sizeof( szRootName ), "Dialog" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "WD" );
      lSourceType = zSOURCE_DIALOG_META;
   }
   else
   {
      strcpy_s( szRootName, sizeof( szRootName ), "LOD" );
      strcpy_s( szEntityName, sizeof( szEntityName ), "ZO" );
      lSourceType = zSOURCE_LOD_META;
   }

   GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "Operation", "Name" );
   strcpy_s( szMsg, sizeof( szMsg ), "OK to delete Operation '" );
   strcat_s( szMsg, sizeof( szMsg ), szName );
   strcat_s( szMsg, sizeof( szMsg ), "'?" );
   nRC = MessagePrompt( vSubtask, "OP00815", "Operations", szMsg, 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

   // The following validation is executed for Dialog metas only.
   if ( wTZOPRUSD_CheckActionForDeleteOp( vSubtask, vDialog, lSourceType, szName ) == 0 )
      return( -1 );

   if ( zstrcmp( szName, "" ) != 0 )
   {
      if ( MessagePrompt( vSubtask, "OP00810", "Operations",
                          "Delete Operation from Source File?",
                          0, zBUTTONS_YESNO, zRESPONSE_YES,
                          zICON_QUESTION ) == zRESPONSE_YES )
      {
         nDeleteFromSourceFile = 1;
         if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) > 0 )
            DropView( vDeleteOp );
      }
      else
         nDeleteFromSourceFile = 0;
   }

   wTZOPRUSD_DeleteOperation( vSubtask, nDeleteFromSourceFile );

   // Delete Operation from Source File
   if ( nDeleteFromSourceFile == 1 )
      wTZOPRUSD_DeleteOperFromFile( vSubtask, zDELETE_OPERATION );

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vSOURCE, "OperationList" ) == zCURSOR_NULL ||
        GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) < 0 )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      SetCursorPrevEntity( vSOURCE, "OperationList", "" );
      GetStringFromAttribute( szName, sizeof( szName ), vSOURCE, "OperationList", "Name" );
      SetCursorFirstEntityByString( vSOURCE, "Operation", "Name", szName, szRootName );
      // Use a temporal subobject version for updating the Operation subobject.
      CreateTemporalSubobjectVersion( vSOURCE, "Operation" );
      SetSelectStateOfEntity( vSOURCE, "OperationList", 1 );
      wTZOPRUSD_DisableSourceFile( vSubtask );

      if ( GetSubtaskForWindowName( vSubtask, &vW, "OPERLIST" ) >= 0 )
         RefreshWindow( vW );
   }

   return( 0 );

} // wTZOPRUSD_DeleteCurrentOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_CheckActionForDeleteOp
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
wTZOPRUSD_CheckActionForDeleteOp( zVIEW  vSubtask,
                                  zVIEW  vDialog,
                                  zLONG  lCallByDialog,
                                  zPCHAR szName )
{
   zSHORT nDelete = 1;
   zCHAR  szMsg[ 128 ];
   zVIEW  vTemp;

   // The following validation is executed for Dialog metas only.
   if ( lCallByDialog == zSOURCE_DIALOG_META )
   {
      CreateViewFromViewForTask( &vTemp, vDialog, 0 );

      if ( SetCursorFirstEntityByString( vTemp, "ActOper", "Name",
                                         szName, "Dialog" ) >= zCURSOR_SET )
      {
         strcpy_s( szMsg, sizeof( szMsg ), "Operation '" );
         strcat_s( szMsg, sizeof( szMsg ), szName );
         strcat_s( szMsg, sizeof( szMsg ), "' cannot be deleted if it is used by an Action." );
         TraceLineS( szMsg, "" );
         DisplayObjectInstance( vTemp, "Action", 0 );
         MessageSend( vSubtask, "OP00817", "Operations", szMsg,
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
         nDelete = 0;
      }

      DropView( vTemp );
   }

   return( nDelete );

} // wTZOPRUSD_CheckActionForDeleteOp

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_AddOperationForNewFile
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_AddOperationForNewFile( zVIEW vSubtask )
{
   zVIEW    vMeta;
   zVIEW    vMetaCopy;
   zVIEW    vParentWindow;
   zSHORT   nRC;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CountEntitiesForView(   vMeta, "SourceFile" ) > 1 )
   {
      CreateViewFromViewForTask( &vMetaCopy, vMeta, 0 );

      // Cancel temporal Operation
      for ( nRC = SetCursorFirstEntity( vMetaCopy, "SourceFile", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vMetaCopy, "SourceFile", "" ) )
      {
         for ( nRC = SetCursorFirstEntity( vMetaCopy, "Operation", "" );
               nRC >= zCURSOR_SET;
               nRC = SetCursorNextEntity( vMetaCopy, "Operation", "" ) )
         {
            CancelSubobject( vMetaCopy, "Operation" );
         }
      }

      DropView( vMetaCopy );

      CreateTemporalMetaEntity( vSubtask, vMeta, "Operation", zPOS_AFTER );
      IncludeSubobjectFromSubobject( vMeta, "OperationList",
                                     vMeta, "Operation", zPOS_AFTER );
      SetSelectStateOfEntity( vMeta, "OperationList", 1 );

      if ( GetSubtaskForWindowName( vSubtask, &vParentWindow, "OPERLIST" ) >= 0 )
         RefreshWindow( vParentWindow );
   }

   return( 0 );

} // wTZOPRUSD_AddOperationForNewFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteParameter
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_DeleteParameter( zVIEW vSubtask )
{
   zVIEW    vSOURCE;
   zVIEW    vWindow;
   zVIEW    vParentWindow;
   zSHORT   nCloseWindow = 0;
   zSHORT   nRC;
   zCHAR    szShortDesc[ 65 ];
   zCHAR    szMsg[ 150 ];

   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szShortDesc, sizeof( szShortDesc ), vSOURCE, "Parameter", "ShortDesc" );
   strcpy_s( szMsg, sizeof( szMsg ), "OK to delete Parameter '" );
   strcat_s( szMsg, sizeof( szMsg ), szShortDesc );
   strcat_s( szMsg, sizeof( szMsg ), "'?" );
   nRC = MessagePrompt( vSubtask, "OP00815", "Operations", szMsg, 0,
                        zBUTTONS_YESNO, zRESPONSE_NO, zICON_QUESTION );

   if ( nRC == zRESPONSE_NO )
      return( 0 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "PARMUPDATE" ) >= 0 )
      nCloseWindow = 1;

   DeleteEntity( vSOURCE, "Parameter", zREPOS_PREV );

   if ( nCloseWindow == 1 &&
        CheckExistenceOfEntity( vSOURCE, "Parameter" ) == zCURSOR_NULL )
   {
      SetWindowActionBehavior( vSubtask, zWAB_ReturnToParentWithRefresh, 0, 0 );
   }
   else
   {
      if ( nCloseWindow == 1 )
      {
         SetFocusToCtrl( vSubtask, "edName" );
         wTZOPRUSD_VerifyParms( vWindow );
         GetParentWindow( &vParentWindow, vSubtask );
         RefreshWindow( vParentWindow );
      }
   }

   wTZOPRUSD_VerifyParms( vSubtask );

   return( 0 );

} // wTZOPRUSD_DeleteParameter

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_SourceFilePopup
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_SourceFilePopup( zVIEW vSubtask )
{
   POINT  pt;
// CPoint pt( GetMessagePos( ) );
   pt.x = pt.y = -1;
   zVIEW  vSOURCE;
   zBOOL  bNew          = FALSE;
   zBOOL  bDelete       = FALSE;
   zSHORT nIsCheckedOut = 0;

   if ( GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK ) > 0 )
   {
      nIsCheckedOut = MiGetUpdateForView( vSOURCE );

      if ( CheckExistenceOfEntity( vSOURCE, "SourceFile" ) >= zCURSOR_SET )
      {
         if ( nIsCheckedOut == 1 )
            bDelete = TRUE;
      }

      if ( nIsCheckedOut == 1 )
         bNew = TRUE;

      EnableAction( vSubtask, "CreateNewSourceFile", bNew );
      EnableAction( vSubtask, "DeleteSourceFile", bDelete );
      EnableAction( vSubtask, "DeleteAllFiles", bDelete );
      EnableAction( vSubtask, "MoveSelectedOperations", bDelete );

      CreateTrackingPopupMenu( vSubtask, "SourceFilePopup", pt.x, pt.y, FALSE, FALSE );
   }

   return( 0 );

} // wTZOPRUSD_SourceFilePopup

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MovePostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MovePostbuild( zVIEW vSubtask )
{
   SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );
   return( 0 );

} // wTZOPRUSD_MovePostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveCheckFileType
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveCheckFileType( zVIEW vSubtask )
{
   zVIEW    vSourceMeta;
   zVIEW    vTargetMeta;

   GetViewByName( &vSourceMeta, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   // if move Operation with Operation Code
   if ( GetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      if ( CompareAttributeToAttribute( vSourceMeta, "SourceFile", "Extension",
                                        vTargetMeta, "SourceFile", "Extension" ) != 0 )
      {
         MessageSend( vSubtask, "OP00819", "Operations",
                      "Language Type of Source Files do not match.",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
         return( -1 );
      }
   }

   // if source and target file identical
   if ( CompareAttributeToAttribute( vSourceMeta, "SourceFile", "Name",
                                     vTargetMeta, "SourceFile", "Name" ) == 0 )
   {
      MessageSend( vSubtask, "OP00819", "Operations",
                   "Target and Source File are identical.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   return( 0 );

} // wTZOPRUSD_MoveCheckFileType

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveOperationToFile
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveOperationToFile( zVIEW vSubtask )
{
   zVIEW  vTargetFile;
   zVIEW  vDeleteOp;

   // Move Operation Code to new Source File
   GetViewByName( &vTargetFile, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK ) > 0 )
      DropView( vDeleteOp );

   // Create LOD for move Operation Code from old Source Files
   wTZOPRUSD_CreateDeleteLOD( vSubtask );

   // Added new Sourcen File for move Opertion to new File
   GetViewByName( &vDeleteOp, "DeleteOp", vSubtask, zLEVEL_TASK );
   CreateEntity( vDeleteOp, "SourceFile", zPOS_AFTER );
   SetMatchingAttributesByName( vDeleteOp, "SourceFile",
                                vTargetFile, "SourceFile", zSET_ALL );

   // Move Operation from Source File
   wTZOPRUSD_DeleteOperFromFile( vSubtask, zMOVE_OPERATION );

   return( 0 );

} // wTZOPRUSD_MoveOperationToFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_SourceFileCancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_SourceFileCancel( zVIEW vSubtask )
{
   zVIEW  vMetaView;
   zVIEW  vWindow;
   zSHORT nRC;
   GetViewByName( &vMetaView, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   CancelSubobject( vMetaView, "SourceFile" );

   nRC = CheckExistenceOfEntity( vMetaView, "SourceFile" );
   if ( nRC == zCURSOR_UNDEFINED )
      SetCursorPrevEntity( vMetaView, "SourceFile", "" );
   if ( nRC >= zCURSOR_UNDEFINED )
      SetSelectStateOfEntity( vMetaView, "SourceFile", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERMOVE" ) >= 0 )
      RefreshWindow( vWindow );
   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "MoveSelectedOperations" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );

} // wTZOPRUSD_SourceFileCancel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_LoadSourceFileDetail
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_LoadSourceFileDetail( zVIEW vSubtask )
{
   zVIEW  vTZOPRUSO;
   zVIEW  vTZOPRUSO2;

   GetViewByName( &vTZOPRUSO, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOPRUSO2, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   SetCursorFirstEntityByAttr( vTZOPRUSO, "SourceFile", "Name",
                               vTZOPRUSO2, "SourceFile", "Name", "" );

   // save current operation position
   CreateTemporalSubobjectVersion( vTZOPRUSO, "SourceFile" );

   return( 0 );

} // wTZOPRUSD_LoadSourceFileDetail

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveOperationCancel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveOperationCancel( zVIEW vSubtask )
{
   zVIEW vTZOPRUSO_Move;
   zVIEW vTZOPRUSO2;

   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOPRUSO2, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   DropView( vTZOPRUSO2 );
   DropObjectInstance( vTZOPRUSO_Move );

   return( 0 );

} // wTZOPRUSD_MoveOperationCancel

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveRtnFromSubwindow
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveRtnFromSubwindow( zVIEW vSubtask )
{
   zVIEW vTZOPRUSO_Move;
   zVIEW vTZOPRUSO2;

   GetViewByName( &vTZOPRUSO2, "TZOPRUSO2", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );

   // If change Source File Name for current Operation, set new Name
   if ( CompareAttributeToAttribute( vTZOPRUSO_Move, "SourceFile", "ZKey",
                                     vTZOPRUSO2, "SourceFile", "ZKey" ) == 0 )
   {
      SetAttributeFromAttribute( vTZOPRUSO_Move, "SourceFile", "Name",
                                 vTZOPRUSO2, "SourceFile", "Name" );
      SetAttributeFromAttribute( vTZOPRUSO_Move, "SourceFile", "Extension",
                                 vTZOPRUSO2, "SourceFile", "Extension" );
      RefreshCtrl( vSubtask, "txtFileName" );
      RefreshCtrl( vSubtask, "txtType" );
   }

   return( 0 );

} // wTZOPRUSD_MoveRtnFromSubwindow

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DeleteOperFromFile
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
wTZOPRUSD_DeleteOperFromFile( zVIEW  vSubtask,
                              zSHORT nMoveOrDeleteFlag )
{
   zVIEW  vProfileXFER;
   zVIEW  vTaskLPLR;
   zVIEW  vEdWrk;
   zVIEW  vDialog;
   zVIEW  vMETAOPERATIONS;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   GetViewByName( &vProfileXFER, "ProfileXFER", vSubtask, zLEVEL_TASK );
   GetViewByName( &vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );
   GetViewByName( &vMETAOPERATIONS, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   SetNameForView( vMETAOPERATIONS, "METAOPERATIONS", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
      SetAttributeFromString( vProfileXFER, "ED", "InvokingTool", "WD" );
   else
      SetAttributeFromString( vProfileXFER, "ED", "InvokingTool", "ZO" );

   if ( vEdWrk == 0 )
   {
      ActivateEmptyObjectInstance( &vEdWrk, "TZEDWRKO", vTaskLPLR, zSINGLE );

      // Create empty root.
      CreateEntity( vEdWrk, "Editor", zPOS_FIRST );
      SetNameForView( vEdWrk, "TZEDWRKO", vSubtask, zLEVEL_SUBTASK );
   }

   SetAttributeFromString( vProfileXFER, "ED", "InvokeEditorCommand", "S" );

   if ( nMoveOrDeleteFlag == zDELETE_OPERATION )
   {
      // Delete Operation from File
      TZEDFRMD_DropOperationFromSource( vSubtask );
   }
   else
   {
      // Move Operation from File to File
      TZEDFRMD_MoveOperationCode( vSubtask, zMOVE_OPERATION );
   }

   return( 0 );

} // wTZOPRUSD_DeleteOperFromFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_RemoveMultiSelection
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_RemoveMultiSelection( zVIEW vSubtask )
{
   zVIEW  vMeta;
   zVIEW  vMetaCopy;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vMeta, "OperationList" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vMetaCopy, vMeta, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vMeta, "OperationList", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vMeta, "OperationList", "" ) )
   {
      nRC = SetSelectStateOfEntity( vMeta, "OperationList", 0 );
   }

   SetCursorFirstEntityByAttr( vMeta, "OperationList", "Name",
                               vMetaCopy, "OperationList", "Name", "" );
   DropView( vMetaCopy );

   SetSelectStateOfEntity( vMeta, "OperationList", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );

} // wTZOPRUSD_RemoveMultiSelection

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_DestroyWindow
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_DestroyWindow( zVIEW vSubtask )
{
   zVIEW  vWindow;

   SetWindowState( vSubtask, zWINDOW_STATUS_DESTROY, 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "UPD_ACT" ) >= 0 )
      SetWindowState( vWindow, zWINDOW_STATUS_SETFOCUS, 1 );

   return( 0 );

} // wTZOPRUSD_DestroyWindow

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveSelPrebuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveSelPrebuild( zVIEW vSubtask )
{
   // This operation creates the TZOPRUSO2 view needed by the Move window.
   // It also uses position on the OperationList entity to position on the
   // corresponding Operation entity.

   zVIEW    vSourceMeta;
   zVIEW    vTargetMeta;
   zVIEW    vTZOPRUSO_Move;

   GetViewByName( &vSourceMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK ) < 0 )
   {
      CreateViewFromViewForTask( &vTargetMeta, vSourceMeta, 0 );
      SetNameForView( vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK );
   }

   if ( GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK ) < 0 )
   {
      ActivateOI_FromOI_ForTask( &vTZOPRUSO_Move, vSourceMeta, 0, zSINGLE );
      SetNameForView( vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   }

   // Position on correct Source File entity.
   SetCursorFirstEntityByAttr( vTZOPRUSO_Move, "SourceFile", "ZKey",
                               vSourceMeta, "SourceFile", "ZKey", 0 );

   OrderEntityForView( vTZOPRUSO_Move, "Operation", "Name A" );

   return( 0 );

} // wTZOPRUSD_MoveSelPrebuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveSelPostbuild
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveSelPostbuild( zVIEW vSubtask )
{
   zVIEW vTZOPRUSO_Move;
   zVIEW vSOURCE;

   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSOURCE, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   SetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED, 1 );

   if ( CheckExistenceOfEntity( vTZOPRUSO_Move, "Operation" ) < zCURSOR_SET ||
        MiGetUpdateForView( vSOURCE ) == 0 )
   {
      SetCtrlState( vSubtask, "OK", zCONTROL_STATUS_ENABLED, 0 );
   }

   return( 0 );

} // wTZOPRUSD_MoveSelPostbuild

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_RemoveMultiSelByFile
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_RemoveMultiSelByFile( zVIEW vSubtask )
{
   zVIEW  vMeta;
   zVIEW  vMetaCopy;
   zVIEW  vWindow;
   zSHORT nRC;

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );

   if ( CheckExistenceOfEntity( vMeta, "SourceFile" ) < zCURSOR_SET )
      return( 0 );

   CreateViewFromViewForTask( &vMetaCopy, vMeta, 0 );

   for ( nRC = SetCursorFirstSelectedEntity( vMeta, "SourceFile", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vMeta, "SourceFile", "" ) )
   {
      nRC =SetSelectStateOfEntity( vMeta, "SourceFile", 0 );
   }

   SetCursorFirstEntityByAttr( vMeta, "SourceFile", "Name",
                               vMetaCopy, "SourceFile", "Name", "" );
   DropView( vMetaCopy );

   SetSelectStateOfEntity( vMeta, "SourceFile", 1 );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "SRCLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );

} // wTZOPRUSD_RemoveMultiSelByFile

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_SourceFileUpdate
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_SourceFileUpdate( zVIEW vSubtask )
{
   zVIEW vMeta;

   wTZOPRUSD_RemoveMultiSelByFile( vSubtask );

   GetViewByName( &vMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   CreateTemporalSubobjectVersion( vMeta, "SourceFile" );

   return( 0 );

} // wTZOPRUSD_SourceFileUpdate

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveCheckSelOperation
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
wTZOPRUSD_MoveCheckSelOperation( zVIEW vSubtask )
{
   zVIEW  vTZOPRUSO_Move;

   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );

   if ( SetCursorFirstSelectedEntity( vTZOPRUSO_Move, "Operation",
                                      "" ) < zCURSOR_SET )
   {
      MessageSend( vSubtask, "OP00819", "Operations",
                   "No Operation selected.",
                   zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, "", "" );
      return( -1 );
   }

   return( 0 );

} // wTZOPRUSD_MoveCheckSelOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZOPRUSD_IncludeNewOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnTZOPRUSD_IncludeNewOperation( zVIEW  vTargetMeta,
                                zLONG  lSourceZKey,
                                zPCHAR szEntityName,
                                zPCHAR szScope )

{
   zSHORT   nRC;

   nRC = SetCursorFirstEntityByInteger( vTargetMeta, szEntityName, "ZKey",
                                        lSourceZKey, szScope );
   while ( nRC >= zCURSOR_SET )
   {
      ExcludeEntity( vTargetMeta, szEntityName, zREPOS_NONE );
      IncludeSubobjectFromSubobject( vTargetMeta, szEntityName,
                                     vTargetMeta, "Operation", zPOS_AFTER  );
      nRC = SetCursorNextEntityByInteger( vTargetMeta, szEntityName, "ZKey",
                                          lSourceZKey, szScope );
   }

   return( 0 );

} // fnTZOPRUSD_IncludeNewOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZOPRUSD_SetNewOperationAssign
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*LOCAL */  OPERATION
fnTZOPRUSD_SetNewOperationAssign( zVIEW  vSubtask,
                                  zVIEW  vSourceMeta,
                                  zVIEW  vTargetMeta )
{
   zVIEW    vDialog;
   zLONG    lSourceZKey;
   zCHAR    szOperationType[ 2 ];

   GetIntegerFromAttribute( &lSourceZKey, vSourceMeta, "Operation", "ZKey" );

   // if Move Dialog Operation, set new Operation in all assigned Actions
   if ( GetViewByName( &vDialog, "TZWINDOWL", vSubtask, zLEVEL_TASK ) > 0 )
   {
      fnTZOPRUSD_IncludeNewOperation( vTargetMeta, lSourceZKey,
                                      "ActOper", "Dialog" );
   }
   else
   {
      // Copy LOD Operation, check Operation Type
      GetStringFromAttribute( szOperationType, sizeof( szOperationType ),
                              vSourceMeta, "Operation", "Type" );
      switch ( szOperationType[ 0 ] )
      {
         case 'C' : // object constraint
            fnTZOPRUSD_IncludeNewOperation( vTargetMeta, lSourceZKey,
                                            "LOD_ConstraintOper", "LOD" );
            break;

         case 'E' : // entity constraint
            fnTZOPRUSD_IncludeNewOperation( vTargetMeta, lSourceZKey,
                                            "LOD_EntityConstraintOperRec", "LOD" );
            break;

         case 'A' : // derived attribute
            fnTZOPRUSD_IncludeNewOperation( vTargetMeta, lSourceZKey,
                                            "LOD_AttrDerivationOperRec", "LOD" );
            break;

         default :
            break;

      } //endswitch
   }

   return( 0 );

} // fnTZOPRUSD_SetNewOperationAssign

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: fnTZOPRUSD_MoveOperation
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
fnTZOPRUSD_MoveOperation( zVIEW vSubtask,
                          zVIEW vTargetMeta,
                          zVIEW vSourceMeta,
                          zVIEW vTZOPRUSO_Move )
{
   zSHORT   nRC;
   zCHAR    szOperationName[ 33 ];
   zCHAR    szMsg[ 100 ];

   // This operation is used to move an Operation and its parameters from
   // one SourceFile entity to another.
   // We will move the operation on which we're currently positioned in
   // TZOPRUSO to the end of the SourceFile for view, TZOPRUSO2, from
   // which the new souce entity was selected.
   GetStringFromAttribute( szOperationName, sizeof( szOperationName ), vTZOPRUSO_Move, "Operation", "Name" );
   strcpy_s( szMsg, sizeof( szMsg ), "Move Operation: " );
   strcat_s( szMsg, sizeof( szMsg ), szOperationName );
   MB_SetMessage( vSubtask, 0, szMsg );

   SetCursorLastEntity( vTargetMeta, "Operation", 0 );
   CreateMetaEntity( vSubtask, vTargetMeta, "Operation", zPOS_AFTER );

   //Position on correct Operation entity.
   SetCursorFirstEntityByAttr( vSourceMeta, "SourceFile", "ZKey",
                               vTZOPRUSO_Move, "SourceFile", "ZKey", 0 );
   SetCursorFirstEntityByAttr( vSourceMeta, "Operation", "ZKey",
                               vTZOPRUSO_Move, "Operation", "ZKey", 0 );
   SetMatchingAttributesByName( vTargetMeta, "Operation",
                                vTZOPRUSO_Move, "Operation", zSET_NULL );

   nRC = SetCursorFirstEntity( vSourceMeta, "Parameter", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vTargetMeta, "Parameter", zPOS_AFTER );
      SetMatchingAttributesByName( vTargetMeta, "Parameter",
                                   vSourceMeta, "Parameter", zSET_ALL );
      nRC = SetCursorNextEntity( vSourceMeta, "Parameter", 0 );
   }

   // if move Operation with Operation Code
   if ( GetCtrlState( vSubtask, "cbCopyCode", zCONTROL_STATUS_CHECKED ) == 1 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Move Operation Code: " );
      strcat_s( szMsg, sizeof( szMsg ), szOperationName );
      MB_SetMessage( vSubtask, 0, szMsg );
      wTZOPRUSD_MoveOperationToFile( vSubtask );
   }

   // if Move Dialog Operation or LOD Operation, set new Operation
   //  in all assigned Actions or Entities/Attributes...
   fnTZOPRUSD_SetNewOperationAssign( vSubtask, vSourceMeta, vTargetMeta );

   SetSelectStateOfEntity( vTargetMeta, "Operation", 1 );

   // reuse ZKey
   SetAttributeFromAttribute( vTargetMeta, "Operation", "ZKey",
                              vSourceMeta, "Operation", "ZKey" );
   DeleteEntity( vSourceMeta, "Operation", zREPOS_NONE );

   return( 0 );

} // fnTZOPRUSD_MoveOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: wTZOPRUSD_MoveSelOperations
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
wTZOPRUSD_MoveSelOperations( zVIEW vSubtask )
{
   zVIEW    vSourceMeta;
   zVIEW    vTargetMeta;
   zVIEW    vTZOPRUSO_Move;
   zVIEW    vWindow;
   zSHORT   nRC;

   if ( wTZOPRUSD_MoveCheckFileType( vSubtask ) < 0 )
      return( -1 );

   if ( wTZOPRUSD_MoveCheckSelOperation( vSubtask ) < 0 )
      return( -1 );

   GetViewByName( &vTZOPRUSO_Move, "TZOPRUSO_Move", vSubtask, zLEVEL_TASK );
   GetViewByName( &vSourceMeta, "TZOPRUSO", vSubtask, zLEVEL_TASK );
   GetViewByName( &vTargetMeta, "TZOPRUSO2", vSubtask, zLEVEL_TASK );

   for ( nRC = SetCursorFirstSelectedEntity( vTZOPRUSO_Move, "Operation", "" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextSelectedEntity( vTZOPRUSO_Move, "Operation", "" ) )
   {
      fnTZOPRUSD_MoveOperation( vSubtask, vTargetMeta,
                                vSourceMeta, vTZOPRUSO_Move );
   }

   DropObjectInstance( vTZOPRUSO_Move );
   fnBuildOperationList( vSubtask );

   if ( GetSubtaskForWindowName( vSubtask, &vWindow, "OPERLIST" ) >= 0 )
      RefreshWindow( vWindow );

   return( 0 );

} // wTZOPRUSD_MoveSelOperations

#ifdef __cplusplus
}
#endif
