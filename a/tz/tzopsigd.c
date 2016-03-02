/*
CHANGE LOG
2001.08.07  HH
    Import Operation from other XGO file.
2001.08.07  HH
    save Workstation as zKeys get updated.
2000.07.05 TMV  ClearCase
    Zeidon Naming conventions because of clearcase
    change filename tzopsigk.xgo to uppercase
2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0
*/

#define _NOANCHOR
#include <zstdafx.h>
#define zGLOBAL_DATA
#include "tzlodopr.h"
#include "tz__oprs.h"

#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"

// LAST MESSAGE ID: OP00901
const static zCPCHAR TZOPSIGK_FILE     = "TZOPSIGK.XGO";

////////////////////////////////////////////////////////////////
// Activate the Zeidon Operations object, using the application
// directory from Zeidon System.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ActivateOPERSIGK( zVIEW vSubtask )
{
   zVIEW  vOperSig;
   zVIEW  vT;
   zCHAR  szXGO_FileName[ 129 ];
   zSHORT nRC;

   nRC = SfCreateSubtask( &vT, vSubtask, "Zeidon System" );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   nRC = GetApplDirectoryFromView( szXGO_FileName, vT, zAPPL_DIR_OBJECT, 125 );
   nRC = SfDropSubtask( vT, 0 );
   strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), TZOPSIGK_FILE );

   nRC = ActivateOI_FromFile( &vOperSig, "TZOPGRPO", vSubtask,
                              szXGO_FileName, zSINGLE | zLEVEL_TASK );
   if ( nRC < 0 )
   {
      MessageSend( vSubtask, "OP00901", "Operations",
                   "Error in TZOPSIGK.XGO Activate",
                   zMSGQ_SYSTEM_ERROR, 0 );
   }
   else
      SetNameForView( vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );

   nRC = InitializeDefaultLPL( vSubtask );
   return( 0 );
}

////////////////////////////////////////////////////////////////
// Commit the Zeidon Operations object named "TZOPSIGK".
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CommitOPERSIGK( zVIEW vSubtask )
{
   zVIEW  vOperSig;
   zVIEW  vT;
   zCHAR  szXGO_FileName[ 129 ];
   zSHORT nRC;
   zVIEW  WKS_View;
   zVIEW  vZeidonCM;

   nRC = SfCreateSubtask( &vT, vSubtask, "Zeidon System" );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   nRC = GetApplDirectoryFromView( szXGO_FileName, vT, zAPPL_DIR_OBJECT, 125 );
   nRC = SfDropSubtask( vT, 0 );
   strcat_s( szXGO_FileName, zsizeof( szXGO_FileName ), TZOPSIGK_FILE );

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );

   // Order the object by name.
   OrderEntityForView( vOperSig, "Operation", "Name A" );
   CommitOI_ToFile( vOperSig, szXGO_FileName, zASCII );

   // did we generate new zKeys?
   //  if yes, commit the workstation
   nRC = GetViewByName( &vZeidonCM, "ZeidonCM", vSubtask, zLEVEL_APPLICATION );
   if ( nRC >= 0 )
      nRC = GetViewByName( &WKS_View, "TZCMWKSO", vZeidonCM, zLEVEL_SUBTASK );

   if ( nRC >= 0 && ObjectInstanceUpdatedFromFile( WKS_View ) )
   {
      oTZCMWKSO_CommitWorkstation( WKS_View );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////
// Reorder the Operations
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ReorderOperations( zVIEW vSubtask )
{
   zVIEW  vOperSig;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );

   // Order the object by name.
   OrderEntityForView( vOperSig, "Operation", "Name A" );

   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create Operation entity because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateNewParameter( zVIEW vSubtask )
{
   zVIEW  vOperSig;
   zSHORT nRC;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   nRC = CheckExistenceOfEntity( vOperSig, "Parameter" );
   if ( nRC == zCURSOR_SET )
      AcceptSubobject( vOperSig, "Parameter" );

   CreateTemporalMetaEntity( vSubtask, vOperSig, "Parameter", zPOS_AFTER );
   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create Parameter entity because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateNewOperation( zVIEW vSubtask )
{
   zVIEW  vOperSig;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vOperSig, "Operation", zPOS_AFTER );

   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create HeaderFile and DefinedItem
// entities because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateNewHeader( zVIEW vSubtask )
{
   zVIEW  vOperSig;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vOperSig, "HeaderFile", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vOperSig, "DefinedItem", zPOS_AFTER );

   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create SourceFileOfOperation and
// ExecComposite entities because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateNewSource( zVIEW vSubtask )
{
   zVIEW  vOperSig;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vOperSig, "SourceFileOfOperation", zPOS_AFTER );
   CreateMetaEntity( vSubtask, vOperSig, "ExecComposite", zPOS_AFTER );

   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create DefinedItem entities because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateNewDefinedItem( zVIEW vSubtask )
{
   zVIEW  vOperSig;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   CreateTemporalMetaEntity( vSubtask, vOperSig, "DefinedItem", zPOS_AFTER );

   return( 0 );
}

////////////////////////////////////////////////////////////////
// We must use CM operation to create Parameter entity because of ZKey.
////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CopySelectedOperation( zVIEW vSubtask )
{
   zVIEW  vOperSig;
   zVIEW  vOperSig2;
   zCHAR  szFile[ 33 ];
   zLONG  lZKey;
   zSHORT nRC;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   CreateViewFromViewForTask( &vOperSig2, vOperSig, 0 );
   CreateTemporalMetaEntity( vSubtask, vOperSig2, "Operation", zPOS_AFTER );

   GetIntegerFromAttribute( &lZKey, vOperSig2, "Operation", "ZKey" );
   SetMatchingAttributesByName( vOperSig2, "Operation",
                                vOperSig, "Operation", zSET_ALL );
   SetAttributeFromInteger( vOperSig2, "Operation", "ZKey", lZKey );

   // Copy all the parameters over.
   nRC = SetCursorFirstEntity( vOperSig, "Parameter", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateMetaEntity( vSubtask, vOperSig2, "Parameter", zPOS_LAST );
      GetIntegerFromAttribute( &lZKey, vOperSig2, "Parameter", "ZKey" );
      SetMatchingAttributesByName( vOperSig2, "Parameter",
                                   vOperSig, "Parameter", zSET_ALL );
      SetAttributeFromInteger( vOperSig2, "Parameter", "ZKey", lZKey );
      nRC = SetCursorNextEntity( vOperSig, "Parameter", 0 );
   }

   // Include the Header File.
   GetStringFromAttribute( szFile, zsizeof( szFile ), vOperSig, "HeaderFile_Oper", "Name" );
   nRC = SetCursorFirstEntityByString( vOperSig2, "HeaderFile",
                                       "Name", szFile,  0 );
   if ( nRC < zCURSOR_SET )
   {
      // Error the Header File is not there.
      SysMessageBox( vSubtask, "Import Operations from other XGO file",
                     "The Header File is missing in the target", 0 );
      CancelSubobject( vOperSig, "Operation" );
      return( 0 );
   }

   IncludeSubobjectFromSubobject( vOperSig2, "HeaderFile_Oper",
                                  vOperSig2, "HeaderFile", zPOS_AFTER );

   // Include the Source File.
   szFile[ 0 ] = 0;
   GetStringFromAttribute( szFile, zsizeof( szFile ), vOperSig, "SourceFile", "Name" );
   nRC = SetCursorFirstEntityByString( vOperSig2, "SourceFileOfOperation",
                                       "Name", szFile,  0 );
   if ( nRC < zCURSOR_SET )
   {
      // Error the Header File is not there
      SysMessageBox( vSubtask, "Copy Operation",
                     "The Source File is missing in the target", 0 );
      CancelSubobject( vOperSig, "Operation" );
      return( 0 );
   }

   IncludeSubobjectFromSubobject( vOperSig2, "SourceFile",
                                  vOperSig2, "SourceFileOfOperation", zPOS_AFTER );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SelectOperation
//
//    Select Operation From Source and Copy to Target
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
SelectOperation( zVIEW vSubtask )
{
   // Select the Operation in the Target XGO File.
   zVIEW  vOperSig;
   zVIEW  vOperSig2;
   zCHAR  szOperation[ 33 ];
   zCHAR  szFile[ 33 ];
   zSHORT nRC;

   GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   GetViewByName( &vOperSig2, "TZOPSIG2", vSubtask, zLEVEL_TASK );

   GetStringFromAttribute( szOperation, zsizeof( szOperation ), vOperSig2, "Operation", "Name" );
   nRC = SetCursorFirstEntityByString( vOperSig, "Operation", "Name",
                                       szOperation, 0 );

   // If it is already there, issue an error message.
   if ( nRC >= 0 )
   {
      SysMessageBox( vSubtask, "Import Operations from other XGO file",
                     "The Operation to be copied is already in target", 0 );
      return( 0 );
   }

   CreateTemporalEntity( vOperSig, "Operation", zPOS_LAST );
   SetMatchingAttributesByName( vOperSig, "Operation",
                                vOperSig2, "Operation", zSET_ALL );

   // Copy all the parameters over.
   nRC = SetCursorFirstEntity( vOperSig2, "Parameter", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      CreateEntity( vOperSig, "Parameter", zPOS_LAST );
      SetMatchingAttributesByName( vOperSig, "Parameter",
                                   vOperSig2, "Parameter", zSET_ALL );
      nRC = SetCursorNextEntity( vOperSig2, "Parameter", 0 );
   }

   // Include the Header File
   GetStringFromAttribute( szFile, zsizeof( szFile ), vOperSig2, "HeaderFile_Oper", "Name" );
   nRC = SetCursorFirstEntityByString( vOperSig, "HeaderFile",
                                       "Name", szFile,  0 );
   if ( nRC < zCURSOR_SET )
   {
      // Error the Header File is not there.
      SysMessageBox( vSubtask, "Import Operations from other XGO file",
                     "The Header File is missing in the target", 0 );
      CancelSubobject( vOperSig, "Operation" );
      return( 0 );
   }

   IncludeSubobjectFromSubobject( vOperSig, "HeaderFile_Oper",
                                  vOperSig, "HeaderFile", zPOS_AFTER );

   // Include the Source File.
   szFile[ 0 ] = 0;
   GetStringFromAttribute( szFile, zsizeof( szFile ), vOperSig2, "SourceFile", "Name" );
   nRC = SetCursorFirstEntityByString( vOperSig, "SourceFileOfOperation",
                                       "Name", szFile,  0 );
   if ( nRC < zCURSOR_SET )
   {
      // Error the Header File is not there
      SysMessageBox( vSubtask, "Import Operations from other XGO file",
                     "The Source File is missing in the target", 0 );
      CancelSubobject( vOperSig, "Operation" );
      return( 0 );
   }

   IncludeSubobjectFromSubobject( vOperSig, "SourceFile",
                                  vOperSig, "SourceFileOfOperation", zPOS_AFTER );

   AcceptSubobject( vOperSig, "Operation" );

   return( 0 );
} // SelectOperation

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: OpenXGO2
//
//    Open the Source XGO File
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
OpenXGO2( zVIEW vSubtask )
{
   // Open the other XGO file using standard open file dialog
   zVIEW   vOperSig2;
   zSHORT  nRC;
   zCHAR szFileName[ 512 ] = { 0 };

   if ( OperatorPromptForFile( vSubtask, szFileName, zsizeof( szFileName ),
                               "Import Files (*.XGO)|*.xgo|", "XGO",
                               zOFN_FILEMUSTEXIST ) != 1 )
   {
      return( 0 );
   }

   // activate Source object from file

   nRC = ActivateOI_FromFile( &vOperSig2, "TZOPGRPO", vSubtask,
                              szFileName, zSINGLE | zLEVEL_TASK );

   if ( nRC < 0 )
   {
      SysMessageBox( vSubtask, "Import Operations from other XGO file",
                     "Error in TZOPSIGK.XGO Activate", 0 );
      return( 0 );
   }
   else
   {
      SetNameForView( vOperSig2, "TZOPSIG2", vSubtask, zLEVEL_TASK );
   }

   // set file name to the Object.
   // we overwrite the Desc attribute in the root.
   // It doesn't matter, because it is not used otherwise and
   // the OperSig2 is not committed back to file.
   SetAttributeFromString( vOperSig2, "OPERSIG", "Desc",  szFileName );

   return( 0 );

} // OpenXGO2

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: CheckForUpdate
//
//    Check Update, issue Warning if necessary
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
CheckForUpdate( zVIEW vSubtask )
{
   zVIEW  vOperSig;
   zSHORT nRC;

   nRC = GetViewByName( &vOperSig, "TZOPSIGK", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 || ObjectInstanceUpdatedFromFile( vOperSig ) == 0 )
      return( 0 );

   if ( MessagePrompt( vSubtask, "OP00001", "Zeidon Operation Maintenance",
                       "Do you really want to quit? Changes will be discarded!",
                       zBEEP, zBUTTONS_YESNO, zRESPONSE_NO, 0 ) == zRESPONSE_NO )
   {
      SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
   }

   return( 0 );

} // CheckForUpdate

#ifdef __cplusplus
}
#endif
