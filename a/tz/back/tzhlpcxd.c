//-----------------------------------------------------------------------------
//
// .Name:         tzhlpcxd.c
//
// .Version:      1.0
//
// .Last change:  21-11-2001
//
// .Description:
//  Application source file which creates the tzhlpcxd.phl file. This
//  file is a connection between Zeidon and the *.hlp files. It
//  includes the mapping informations which are necessary to open the *.hlp
//  files at specified positions.
//  The file also includes import functionality. It is able to analyze
//  Robohelp Report files which are saved as *.txt files and which
//  includes the Robohelp topics (help mapping information). It reads
//  the Robohelp Report file and generates all necessary all Zeidon objects
//  which are later saved in the tzhlpcxd.phl file.
//
//
//  REMARK:
//  After Import of progref tzhlpcxp has to be changed by hand. It is
//  necessary because topics of progref consist of more than one
//  word and are different in zeidon and in Robohelp file progref.hlp.
//  It is not possible to create an automatic mapping.
//
// .ErrorNumber:  HL00001 (not used)
//
//
//  Functionlist :
//  Extern  zwTZHLPCXD_ActivateHlpMapFile
//  Extern  zwTZHLPCXD_CommitHlpMapFile
//  Extern  zwTZHLPCXD_ImportHlpMapInfo
//  Extern  zwTZHLPCXD_DeleteAllCtxtOfManual
//  Extern  zwTZHLPCXD_SelectManualsOfOpSys
//  Extern  zwTZHLPCXD_ExitDialog
//
//  Intern  zwTZHLPCXD_ReadRoboHelpFile
//  Intern  zwTZHLPCXD_ExtractLastWord
//  Intern  zwTZHLPCXD_FreeHelpMapInfo
//
//
//-----------------------------------------------------------------------------
// Change log most recent first order:
// ??.??.??   ??  ??
//    created
// 2001.11.21   RR
//    Complete overworked. Add import functionality.
//
//-----------------------------------------------------------------------------

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>

#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"

// Global Variables

// Temporary structures to pick up the manual information of a
// robohelp report file.
typedef struct HelpFcts zHelpFct;
struct HelpFcts
{
    zPCHAR   pszTag;
    zPCHAR   pszNumber;
    zHelpFct *pNext;
};

typedef struct HelpMappingInformation
{
  zPCHAR   pszHelpFileName;
  zHelpFct *szHelpFcts;

} zHelpMapInfo;


zOPER_EXPORT zSHORT /*DIALOG */ OPERATION
  zwTZHLPCXD_ExtractLastWord( zVIEW, zPCHAR *, zPCHAR, zCHAR );

zOPER_EXPORT zSHORT /*DIALOG */ OPERATION
  zwTZHLPCXD_FreeHelpMapInfo( zHelpMapInfo ** );

zOPER_EXPORT zSHORT /*DIALOG */ OPERATION
  zwTZHLPCXD_ReadRobhelpFile( zVIEW, zHelpMapInfo ** );

zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ErrMsg( zVIEW, zPCHAR, zPCHAR, zPCHAR, zLONG, zSHORT );


///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZHLPCXD_ActivateHlpMapFile
//
//    Attempts to load the portable file containing help mapping information.
//
////////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ActivateHlpMapFile( zVIEW vSubtask )
{
   zCHAR     szBuffer[ 500 ];
   zCHAR     szFileName[ 301 ];
   zVIEW     TaskLPLR;
   zVIEW     vTZHLPCXO;
   zSHORT    nRC;

   nRC = InitializeLPLR( vSubtask, "" );
   if ( nRC <= 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "InitializeLPLR", "Cannot initialize LPLR",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   nRC = GetViewByName( &TaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "GetViewByName", "Cannot get view TaskLPLR",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Build full qualified name of help mapping file.
   nRC = SysGetLocalDirectory( szFileName );
   if ( nRC < 0 )
   {
      strcpy_s( szBuffer, zsizeof( szBuffer ), "Unable to get current system directory" );
      strcat_s( szBuffer, zsizeof( szBuffer ), szFileName );
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "SysGetLocalDirectory", szBuffer,
                         zMSGQ_SYSTEM_ERROR, nRC );
     return( nRC );
   }

   nRC = ZeidonStringConcat( szFileName, 1, 0, "tzhlpcxp.phl", 1, 0, 300 );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "ActivateOI_FromFile",
                         "Unable to create full qualified name of help file tzhlpcxp.phl",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Read help mapping file.
   nRC = ActivateOI_FromFile( &vTZHLPCXO, "TZHLPCXO", vSubtask,
                              szFileName, zMULTIPLE );

   // If help mapping file does not exist or is not readable, then create a
   // new empty object instance.
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "ActivateOI_FromFile",
                         "Unable to open help file. Trying to create a new one.",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );

      nRC = ActivateEmptyObjectInstance( &vTZHLPCXO, "TZHLPCXO", vSubtask, zSINGLE );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                            "ActivateEmptyObjectInstance",
                            "Unable to activate empty object instance of TZHLPCXO",
                            zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
         return( nRC );
      }

      nRC = CreateEntity( vTZHLPCXO, "Root", zPOS_FIRST );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                            "CreateEntity",
                            "Unable to create an instance of entity Root",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
         return( nRC );
      }
   }

   TraceLineS( "Mapping file loaded: ", szFileName );

   // Name the view.
   nRC = SetNameForView( vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ActivateHlpMapFile",
                         "SetNameForView",
                         "Cannot get name for view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
   }

   return( nRC );
} // zwTZHLPCXD_ActivateHlpMapFile

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZHLPCXD_CommitHlpMapFile
//
//    DESCRIPTION:
//      Attempts to save the portable file containing help mapping information
//
//    PARAMETERS:
//      I zVIEW vSubtask :
//
//    RETURNVALUES:
//      0           : if successful
//      otherwise   : returncode of sublevel functions if they fail
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_CommitHlpMapFile( zVIEW vSubtask )
{
   zCHAR     szFileName[ 301 ];
   zVIEW     TaskLPLR;
   zVIEW     vTZHLPCXO;
   zSHORT    nRC;

   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_CommitHlpMapFile",
                         "GetViewByName", "Cannot get view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   nRC = GetViewByName( &TaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_CommitHlpMapFile",
                         "GetViewByName", "Cannot get view TaskLPLR",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Build full qualified name of mapping file.
   nRC = SysGetLocalDirectory( szFileName );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_CommitHlpMapFile",
                         "SysGetLocalDirectory",
                         "Cannot get create full qualified destination file name ...\tzhlpcxp.phl",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   nRC = ZeidonStringConcat( szFileName, 1, 0, "tzhlpcxp.phl", 1, 0, 300 );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_CommitHlpMapFile",
                         "ZeidonStringConcat",
                         "Cannot create full qualified destination file name ...\tzhlpcxp.phl",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Write Instances to mapping file.
   nRC = CommitOI_ToFile( vTZHLPCXO, szFileName, zASCII );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_CommitHlpMapFile",
                         "CommitOI_ToFile", "Unable to commit TZHLPXO",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
   }

   return( nRC );
} // zwTZHLPCXD_CommitHlpMapFile

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION:  zwTZHLPCXD_ImportHlpMapInfo
//
//    DESCRIPTION:
//      Read the mapping information of "Robohelp"-File and add the
//      information to the new created entity instances of View vTZHLPCXO,
//      which includes the mapping information of all manuals.
//      This function does not commit the result, so the result does not
//      become operative until the result is saved by a another function.
//
//    PARAMETERS:
//      I zVIEW vSubtask :
//
//    RETURNVALUES:
//      0           : if successful
//      otherwise   : returncode of sublevel functions if they fail
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ImportHlpMapInfo( zVIEW vSubtask )
{
   zVIEW        vTZHLPCXO;
   zSHORT       nRC;
   zHelpMapInfo *pHelpMapInfo;
   zHelpFct     *pHelpFcts;
   zCHAR        szBuffer[ 512 ];

   // Read the manual mapping information which is necessary to open a
   // helpfile and jump at a specified position.
   // Put the information into a temporary structure of Type zHelpMapInfo.
   nRC = zwTZHLPCXD_ReadRobhelpFile( vSubtask, &pHelpMapInfo );
   if ( nRC < 0 || &pHelpMapInfo == 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                         "zwTZHLPCXD_ReadRobhelpFile",
                         "Reading Robohelp file fails",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
      return( nRC );
   }

   pHelpFcts = pHelpMapInfo->szHelpFcts;

   // send some debug information to trace window.
   TraceLineS( "zwTZHLPCXD_ImportHlpMapInfo ", pHelpMapInfo->pszHelpFileName );
   TraceLineS( "zwTZHLPCXD_ImportHlpMapInfo ", pHelpFcts->pszTag );

   // Get the zeidon object to take the help mapping information.
   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                         "GetViewByName", "Cannot create view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Check if Entity OperSystem already exists.If not then created the entity.
   // (Root entity is already created in the prebuild of the window).
   if ( CheckExistenceOfEntity( vTZHLPCXO, "OperSystem" ) < zCURSOR_SET )
   {
      nRC = CreateEntity( vTZHLPCXO, "OperSystem", zPOS_AFTER );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                            "CreateEntity",
                            "Cannot create entity instance of entity OperSystem",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
         return( nRC );
      }

      // Set Attribute OSName with default information.
      nRC = SetAttributeFromString( vTZHLPCXO, "OperSystem", "OSName",
                                    "MS-Windows" );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                            "SetAttributeFromString",
                            "Cannot set default Operating System",
                            zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
         return( nRC );
      }
   }
   else
   {
      // Check if Manual, which is defined in Attribute FileName,
      // already exists. If yes then open Message window
      // and tell user that manual first has to be deleted.
      nRC = SetCursorFirstEntityByString( vTZHLPCXO, "Context", "FileName",
                                          pHelpMapInfo->pszHelpFileName, 0 );
      if ( nRC == zCURSOR_SET )
      {
         strcpy_s( szBuffer, zsizeof( szBuffer ), "Manual already exists. Please delete manual " );
         strcat_s( szBuffer, zsizeof( szBuffer ), pHelpMapInfo->pszHelpFileName );
         strcat_s( szBuffer, zsizeof( szBuffer ), " first, before you recreate it." );
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                            "SetCursorFirstEntityByString", szBuffer,
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
         return( nRC );
      }
   }

   // Take the help mapping information from temporary structure pHelpMapInfo
   // and put the information to new created zeidon objects.
   pHelpFcts = pHelpMapInfo->szHelpFcts;
   while ( pHelpFcts )
   {
      nRC = CreateEntity( vTZHLPCXO, "Context", zPOS_AFTER);
      if (nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                            "CreateEntity",
                            "Cannot create entity instance of entity Context",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
         nRC = zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
         return( nRC );
      }

      nRC = SetAttributeFromString( vTZHLPCXO, "Context", "Tag",
                                    pHelpFcts->pszTag );
      if (nRC < 0 )
      {
         strcpy_s( szBuffer, zsizeof( szBuffer ), "Unable to set Attribute 'Tag'");
      }
      else
      {
         nRC = SetAttributeFromString( vTZHLPCXO, "Context", "Number", pHelpFcts->pszNumber );
         if ( nRC < 0 )
         {
            strcpy_s( szBuffer, zsizeof( szBuffer ), "Unable to set attribute Number" );
         }
         else
         {
            nRC = SetAttributeFromString( vTZHLPCXO, "Context", "FileName",
                                          pHelpMapInfo->pszHelpFileName );
            if ( nRC < 0 )
            {
               strcpy_s( szBuffer, zsizeof( szBuffer ), "Unable to set attribute FileName" );
            }
         }
      }

      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ImportHlpMapInfo",
                            "SetAttributeFromString", szBuffer,
                            zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
         nRC = zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
         return( nRC );
      }

      pHelpFcts = pHelpFcts->pNext;
   }

   // Create Warning message for Progref-Manual.
   if ( !zstrcmp( pHelpMapInfo->pszHelpFileName, "Progref") )
   {
      strcpy_s( szBuffer, zsizeof( szBuffer ), "File tzhlpcxd.phl has to be changed by hand! " );
      strcat_s( szBuffer, zsizeof( szBuffer ), "Topic Names for Progref are different in Zeidon and Robohelp" );
      nRC = MessageSend( vSubtask, "HL00001",
                         "I M P O R T A N T             I N F O R M A T I O N",
                         szBuffer, zMSGQ_OBJECT_CONSTRAINT_INFORMATION, FALSE );
   }

   zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
   return( nRC );

} // zwTZHLPCXD_ImportHlpMapInfo

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZHLPCXD_DeleteAllCtxtOfManual
//
//    DESCRIPTION:
//      This function deletes all information of a specified manual.
//      The result is not committed to database.
//
//    PARAMETERS:
//      I vSubtask : not in use at the moment
//
//    RETURNVALUES:
//      0           : if successful
//      otherwise   : returncode of sublevel functions if they fail
//
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_DeleteAllCtxtOfManual( zVIEW vSubtask )
{
   zCHAR     OperSysName[ 301 ];
   zCHAR     FileName[ 301 ];
   zVIEW     vTZHLPCXO;
   zSHORT    nRC;

   // get the zeidon object which includes the manual information.
   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_DeleteAllCtxtOfManual",
                         "GetViewByName", "Cannot get view TZHLPCXO.",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   // Get the current operating system name.
   nRC = GetVariableFromAttribute( OperSysName, 0, 'S', 300, vTZHLPCXO,
                                   "OperSystem", "OSName", "", 0 );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_DeleteAllCtxtOfManual",
                         "GetVariableFromAttribute",
                         "Cannot read attribute OSName",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
      return( nRC );
   }

   // Get the current selected manual name (= filename).
   nRC = GetVariableFromAttribute( FileName, 0, 'S', 300, vTZHLPCXO,
                                   "Context", "FileName", "", 0 );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_DeleteAllCtxtOfManual",
                         "GetVariableFromAttribute",
                         "Cannot read attribute FileName.",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
     return( nRC );
   }

// TraceLineS( "zwTZHLPCXD_DeleteAllCtxtOfManual - OperSysName: ", OperSysName);
// TraceLineS( "zwTZHLPCXD_DeleteAllCtxtOfManual - FileName: ", FileName);

   // Search all the entries of a manual in the zeidon object and
   // delete them in the runtime system (not in the final mapping file).
   for ( nRC = SetCursorFirstEntityByString( vTZHLPCXO, "OperSystem",
                                             "OSName", OperSysName, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntityByString( vTZHLPCXO, "OperSystem",
                                            "OSName", OperSysName, 0 ) )
   {
      // Now we delete the all items of the specified manual.
      for ( nRC = SetCursorFirstEntityByString( vTZHLPCXO, "Context",
                                                "FileName", FileName, 0 );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntityByString( vTZHLPCXO, "Context",
                                               "FileName", FileName, 0 ) )
      {
         nRC = DeleteEntity( vTZHLPCXO, "Context", zREPOS_NONE );
         if ( nRC < 0  && nRC != zCURSOR_UNDEFINED )
         {
            zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_DeleteAllCtxtOfManual",
                               "DeleteEntity",
                               "Cannot delete an entity instance of entity Context.",
                               zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
            return( nRC );
         }
      }
   }

   return( 0 );

} // zwTZHLPCXD_DeleteAllCtxtOfManual

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION:   zwTZHLPCXD_SelectManualsOfOpSys
//
//    DESCRIPTION:
//      It is called to set the cursor to the first entry of a manual when
//      a new manual is selected in the combo box of window "DeleteManual".
//      So the right manual entries are shown in the second combo box.
//
//    PARAMETERS:
//      I vSubtask : not in use at the moment
//
//    RETURNVALUES:
//      0           : if successful
//      otherwise   : returncode of sublevel functions if they fail
//
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_SelectManualsOfOpSys( zVIEW vSubtask )
{
   zSHORT    nRC            = 0;
   zVIEW     vTZHLPCXO      = 0;
   zVIEW     TaskLPLR       = 0;

   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_SelectManualsOfOpSys",
                         "GetViewByName", "Cannot get view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   nRC = SetNameForView( vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_SelectManualsOfOpSys",
                         "SetNameForView", "Cannot set name for view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   if ( CheckExistenceOfEntity( vTZHLPCXO,"Context") < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_SelectManualsOfOpSys",
                         "CheckExistenceOfEntity",
                         "No manual exist for Operating System",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
      return( nRC );
   }
   else
   {
      // Set cursor to the first entry of a zeidon manual in the zeidon object.
      nRC = SetCursorFirstSelectedEntity( vTZHLPCXO, "Context", "OperSystem" );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_SelectManualsOfOpSys",
                            "SetCursorFirstEntity",
                            "Cannot set Cursor to the first entity instance of entity Context.",
                            zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
         return( nRC );
      }

      nRC = SetWindowActionBehavior( vSubtask, zWAB_StayOnWindowWithRefresh,
                                      "", "" );
      if ( nRC < 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_SelectManualsOfOpSys",
                            "SetWindowActionBehavior", "",
                            zMSGQ_SYSTEM_ERROR, nRC );
      }
  }

  return( nRC );

} // zw_TZHLPCXD_SelectManualsOfOpSys

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION:   TZHLPCXD_ReadRoboHelpFile
//
//    DESCRIPTION:
//      This is an internal function.
//      This functions reads the zeidon object which includes the name  of the
//      import file and opens the import file.
//      The function analyzes the file line by line and reads all information
//      which is necessary to open a help file and to jump at a specified
//      position. These names and topics are put into a C-Structure of type
//      zHelpMapInfo.
//      A pointer to that structure is returned or NULL in case of error. The
//      function allocates memory for all of the structure elements. The calling
//      function is responsible to deallocate the memory if the structure is not
//      used any more.
//
//
//    PARAMETERS:
//      I   vSubtask    : subtask view for messages
//      I/O //pHelpInfo : pointer to structure to take the mapping information
//                        of help files
//
//    RETURNVALUES:
//      0           : if successful
//      zCALL_ERROR : if error
//      otherwise   : returncode of sublevel functions if they fail
//
///////////////////////////////////////////////////////////////////////////////////
zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ReadRobhelpFile( zVIEW vSubtask, zHelpMapInfo **pHelpInfo )
{
   zVIEW     vTZHLPCXO             = 0;
   zSHORT    nRC                   = 0;
   zCHAR     szMsg[ 5000 ];

   // Variables to specify robohelp report file the file which is
   // to be imported.
   zCHAR     szSourceFileName[ zMAX_FILENAME_LTH + 1 ];
   zLONG     hFile                 = 0;
   zPCHAR    pchLine               = 0;
   zPCHAR    pChr                  = 0;

   // Variables to take the mapping information of the robohelp
   // report file.
   zPCHAR    pFctNumber            = 0;
   zPCHAR    pFctID                = 0;
   zPCHAR    pFctFilename          = 0;
   zPCHAR    pTmpFctFilename       = 0;
   zHelpFct  *pLastHelpFct         = 0;
   zHelpFct  *pTmpHelpFcts         = 0;
   zHelpMapInfo *pHelpMapInfo      = 0;

   zLONG     lLth = 0;

   // Get the zeidon object.
   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                         "GetViewByName", "Cannot get view TZHLPCXO",
                         zMSGQ_SYSTEM_ERROR, nRC );
      pHelpInfo = 0;
      return( nRC );
   }

   // Get the name of the robohelp report file (= import file) from zeidon object.
   nRC = GetStringFromAttribute( szSourceFileName, zsizeof( szSourceFileName ), vTZHLPCXO, "Root", "ImportFile" );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                         "GetStringFromAttribute",
                         "Cannot find import file name",
                         zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
      pHelpInfo = 0;
      return( nRC );
   }

   // Open import file and return with error if the file was not found.
   hFile = SysOpenFile( vSubtask, szSourceFileName, COREFILE_READ );
   if ( hFile == -1 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "Cannot find Robohelp file: " );
      strcat_s( szMsg, zsizeof( szMsg ), szSourceFileName);
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                         "SysOpenFile", szMsg, zMSGQ_REQUIRED_DATA_ITEM_ERROR, nRC );
      nRC = zCALL_ERROR;
      pHelpInfo = 0;
      return( nRC );
   }

   // Create structure to take the help mapping information and initialize it.
   pHelpMapInfo = (zHelpMapInfo *) malloc( zsizeof (zHelpMapInfo) );
   if ( pHelpMapInfo  == 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                         "pHelpMapInfo",
                         "Insufficient memory available for struct zHelpMapInfo",
                         zMSGQ_SYSTEM_ERROR, nRC );
      nRC = zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
      return( zCALL_ERROR );
   }

   // We only have one Manual called "Zeidon".
   pFctFilename = (zPCHAR) malloc( zsizeof( "Zeidon" ) );
   if ( pFctFilename == 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                         "pFctFilename",
                         "Insufficient memory available for pFctFilename",
                         zMSGQ_SYSTEM_ERROR, nRC );
      nRC = zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
      return( zCALL_ERROR );
   }

   strcpy_s( pFctFilename, zsizeof( "Zeidon" ), "Zeidon" );
   pHelpMapInfo->pszHelpFileName = pFctFilename;
   pHelpMapInfo->szHelpFcts = 0;

   // Analyze import file and put information into zHelpMapInfo structure.
   nRC = SysReadLine( vSubtask, &pchLine, hFile );
   while ( nRC == 1 )
   {
      // Ignore empty lines
      while ( zstrcmp( pchLine, "" ) == 0 )
         nRC = SysReadLine( &pchLine, hFile );

      // Ignore first word in line
      if ( (pChr = zstrstr( pchLine, "#define " )) != 0 )
         pchLine = pChr + 8;

      // Ignore leading spaces
      while ( (pChr = zstrstr( pchLine, " " )) == pchLine )
         pchLine = pchLine+1;

      if ( nRC == 1 )
      {
         // Set pointer to the end of Topic.
         pChr = zstrstr( pchLine, "\t");
         lLth  = pChr-pchLine;
         if ( pChr == 0 || lLth <= 0 )
         {
            nRC = zCALL_ERROR;
         }
         else
         {
            // Allocate memory for topic.
            pFctID = (zPCHAR) malloc( lLth * zsizeof( char ) + 1 );
            if ( pFctID == 0 )
            {
               zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                                  "pFctID", "Not enough memory",
                                  zMSGQ_SYSTEM_ERROR, 0 );
               nRC = zCALL_ERROR;
            }
            else
            {
               // Copy Topic into structure
               strncpy_s( pFctID, lLth * zsizeof( char ) + 1, pchLine, lLth );
               pFctID[ lLth ] = 0;
            }
         }
      }

      if ( nRC == 1 )
      {
         // Get topic id
         nRC = zwTZHLPCXD_ExtractLastWord( vSubtask, &pFctNumber, pchLine, '\t' );

         // If information is complete (function name and function id)
         // then allocate memory for a new structure and assign information
         if ( nRC != zCALL_ERROR )
         {
            pTmpHelpFcts = (zHelpFct *) malloc( zsizeof( zHelpFct ) );
            if ( pTmpHelpFcts  == 0 )
            {
               zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ReadRobhelpFile",
                                  "pTmpHelpFcts",
                                  "Not enough memory to create struct zHelpFct",
                                  zMSGQ_SYSTEM_ERROR, nRC );
               nRC = zCALL_ERROR;
            }
            else
            {
               pTmpHelpFcts->pszTag    = pFctID;
               pTmpHelpFcts->pszNumber = pFctNumber;
               pTmpHelpFcts->pNext     = 0;

               // Reset help pointers.
               pFctID                  = 0;
               pFctNumber              = 0;

               // Concatenate structures.
               if ( pHelpMapInfo->szHelpFcts == 0 )
               {
                 pLastHelpFct             = pTmpHelpFcts;
                 pHelpMapInfo->szHelpFcts = pLastHelpFct;
               }
               else
               {
                 pLastHelpFct->pNext = pTmpHelpFcts;
                 pLastHelpFct        = pLastHelpFct->pNext;
               }
            }
         }
     }

     // Read next line of import file.
     if ( nRC != zCALL_ERROR )
        nRC = SysReadLine( &pchLine, hFile );
   }

   // Closing Help Map file.
   SysCloseFile( vSubtask, hFile, 0 );

   if (nRC == zCALL_ERROR )
   {
      nRC = zwTZHLPCXD_FreeHelpMapInfo( &pHelpMapInfo );
   }
   else
   {
      *pHelpInfo=pHelpMapInfo;

      // Following code is only used for trace.
   /* TraceLineS( "Filename: ", pHelpMapInfo->pszHelpFileName );
      pTmpHelpFcts = pHelpMapInfo->szHelpFcts;
      while ( pTmpHelpFcts != 0 )
      {
         TraceLineS( "Functionnumber: ", pTmpHelpFcts->pszTag );
         TraceLineS( "Functionname  : ", pTmpHelpFcts->pszNumber );
         pTmpHelpFcts = pTmpHelpFcts->pNext;
      }
   */
   }

   return( nRC );

} // zwTZHLPCXD_ReadRobhelpFile

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZHLPCXD_ExtractLastWord
//
//    DESCRIPTION:
//      This is an internal function.
//      This function analyzes a string (pLine). It takes all characters behind
//      the specified separator character, allocates memory for the substring
//      and returns the pointer to the new string.
//      If pLine or vSubtask is NULL the function returns an error. When the
//      separator was not found in pLine than ppDestStr leave unchanged.
//
//    PARAMETERS:
//      I   vSubtask   : pointer to subtask used for user messages
//      I/O *ppDestStr : pointer to substring
//      I   pLine      : string to be analyzed
//      I   Separator  : separator character which is used to find the
//                       substring in the string
//
//    RETURNVALUES:
//    0            : if successful
//    zCALL_ERROR  : otherwise
//
///////////////////////////////////////////////////////////////////////////////////
zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ExtractLastWord( zVIEW vSubtask, zPCHAR *ppDestStr,
                            zPCHAR pLine, zCHAR Separator )
{
   zSHORT   nRC  = 0;
   zLONG    lLth = 0;
   zPCHAR   pChr = 0;
   zPCHAR   pDestStr = 0;

   // Check Parameters.
   if ( vSubtask == 0 || pLine == 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ExtractLastWord",
                         "vSubtask or pLine",
                         "Invalid parameters. Pointers are NULL",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
      return( zCALL_ERROR );
   }

   // Extract substring of string pLine. Take all characters behind the separator
   // character,allocate memory for the substring and return the pointer to the
   // new string.
   if ( (pChr = strrchr( pLine, Separator )) != 0 )
   {
      lLth = strlen( pChr ) + 1;
      pDestStr = (zPCHAR) malloc( lLth * sizeof( zCHAR ) );
      if ( pDestStr == 0 )
      {
         zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ExtractLastWord",
                            "pDestStr", "Not enough memory",
                            zMSGQ_SYSTEM_ERROR, 0 );
         nRC = zCALL_ERROR;
         return( nRC );
      }

      strncpy_s( pDestStr, lLth * sizeof( zCHAR ), (pChr + 1), (lLth * zsizeof( char ) - 1 ) );
      *ppDestStr = pDestStr;
   }

   return( nRC );

} // zwTZHLPCXD_ExtractLastWord

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: zwTZHLPCXD_FreeHelpMapInfo
//
//    DESCRIPTION:
//      This is an internal function.
//      It deallocates memory of struct zHelpMapInfo including all of its
//      substructures if necessary. The function returns the initialized pointer
//      to structure. For a successful usage of this function it is necessary
//      to initialize all pointers with NULL if no memory is allocated for its
//      elements.
//
//    PARAMETERS:
//      I/O //pHelpInfo : Pointer to struct for which memory should be freed.
//
//    RETURNVALUES:
//      0 allways    : currently not used.
//
///////////////////////////////////////////////////////////////////////////////////
zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_FreeHelpMapInfo( zHelpMapInfo **pHelpInfo )
{
   zSHORT       nRC  = 0;
   zPCHAR       pChr = 0;
   zLONG        Len  = 0;
   zHelpFct     *pTmpHelpFcts  = 0;
   zHelpFct     *pTmpHelpFcts2 = 0;
   zHelpMapInfo *pHelpMapInfo  = 0;

   // Uuse temporary variable because of a better handling.
   pHelpMapInfo = *pHelpInfo;

   // Free structure if necessary.
   if ( pHelpMapInfo )
   {
      // Free sub structures of type zHelpFct if necessary.
      pTmpHelpFcts = pHelpMapInfo->szHelpFcts;
      while ( pTmpHelpFcts )
      {
         pTmpHelpFcts2 = pTmpHelpFcts->pNext;
         if ( pTmpHelpFcts->pszTag )
            free( pTmpHelpFcts->pszTag );

         if ( pTmpHelpFcts->pszNumber )
            free( pTmpHelpFcts->pszNumber );

         free( pTmpHelpFcts );

         pTmpHelpFcts = pTmpHelpFcts2;
      }

      if ( pHelpMapInfo->pszHelpFileName )
         free( pHelpMapInfo->pszHelpFileName );

      free( pHelpMapInfo );

      // Reinitialize returned pointer.
      *pHelpInfo = pHelpMapInfo = 0;
   }

   return( nRC );
} // zwTZHLPCXD_ExtractLastWord

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION:    zwTZHLPCXD_ExitDialog
//
//    DESCRIPTION:
//      This function closes the application. If the object instance is updated
//      a window pops up and the user has the possibility to commit the object
//      instance before the application closes.
//
//    PARAMETERS:
//      I vSubtask : View Pointer used for message boxes
//
//    RETURNVALUES:
//      0           : if successful
//      zCALL_ERROR : if error
//      otherwise   : returncode of sublevel functions if they fail
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ExitDialog( zVIEW vSubtask )
{
   zCHAR  szBuffer[ 512 ];
   zVIEW  vTZHLPCXO = 0;
   zSHORT nRC       = 0;

   // Get the zeidon object which includes the information of help mapping.
   nRC = GetViewByName( &vTZHLPCXO, "TZHLPCXO", vSubtask, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ExitDialog", "GetViewByName",
                         "Cannot get view TZHLPCXO", zMSGQ_SYSTEM_ERROR, nRC );
      return( nRC );
   }

   nRC = ObjectInstanceUpdatedFromFile( vTZHLPCXO );
   if ( nRC < 0 )
   {
      zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ExitDialog",
                         "ObjectInstanceUpdatedFromFile", "Cannot update view TZHLPXO",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, nRC );
      return( nRC );
   }

   if ( nRC == 1 )
   {
      strcpy_s( szBuffer, zsizeof( szBuffer ),
               "Object has been modified. \n\nWould you like to save it ?" );
      nRC = MessagePrompt( vSubtask, "HL00011", "Help Context Mapping",
                           szBuffer, 0, zBUTTONS_YESNOCANCEL,
                           zRESPONSE_YES, zICON_QUESTION );

      switch ( nRC )
      {
         // User said: commit object instance before leaving application.
         case zRESPONSE_YES :
            nRC = zwTZHLPCXD_CommitHlpMapFile( vSubtask );
            break;

         // User does not want ot save the object instance. Leave without
         // doing anything.
         case zRESPONSE_NO :
            break;

         // User wants to cancel the operation.
         case zRESPONSE_CANCEL :
            nRC = SetWindowActionBehavior( vSubtask, zWAB_StayOnWindow, 0, 0 );
            break;

         // Otherwise pop up error message and write result to trace.
         default :
            zwTZHLPCXD_ErrMsg( vSubtask, "zwTZHLPCXD_ExitDialog",
                               "MessagePrompt", "Unknown Return Code",
                               zMSGQ_SYSTEM_ERROR, nRC );
      }
   }

   return( nRC );
} // zwTZHLPCXD_ExitDialog

///////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION:    zwTZHLPCXD_ErrMsg
//
//    DESCRIPTION:
//      This function creates an error message box and additionally writes
//      the error messages to Zeidon Object Services if the environment
//      variable HLP_TRACE_ON is set to 1. If HLP_TRACE_ON variable does not
//      exist, the not trace messages are printed to Zeidon Object Services
//      window.
//
//    PARAMETERS:
//      I vSubtask : View Pointer used for message boxes
//
//    RETURNVALUES:
//       0          : if successful
//      -1          : if error
//
///////////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DIALOG */  OPERATION
zwTZHLPCXD_ErrMsg( zVIEW  vSubtask, zPCHAR cpcFct, zPCHAR cpcErrObj,
                   zPCHAR cpcMsgText, zLONG lMsgType, zSHORT returnCode )
{
   zSHORT nRC;
   zCHAR  szMsg[ 4096 ];
   zCHAR  vTrace[ 2 ];
   zCHAR  rcStr[ 10 ];

   // Check parameters.
   if ( vSubtask == 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "zwTZHLPCXD_ErrMsg | " );
      strcat_s( szMsg, zsizeof( szMsg ), "zMSGQ_OBJECT_CONSTRAINT_ERROR | " );
      strcat_s( szMsg, zsizeof( szMsg ), "Parameter Checking: " );
      strcat_s( szMsg, zsizeof( szMsg ), "Parameter vSubtask is null. |" );
      TraceLineI( szMsg, nRC );
   }

   strcpy_s( szMsg, zsizeof( szMsg ), cpcErrObj );
   strcat_s( szMsg, zsizeof( szMsg ), "\nReturn code = " );
   zltoa( returnCode, rcStr );
   strcat_s( szMsg, zsizeof( szMsg ), rcStr );
   strcat_s( szMsg, zsizeof( szMsg ), "\n" );
   strcat_s( szMsg, zsizeof( szMsg ), cpcMsgText );

   // Open Error Message Box and view error message.
   nRC = MessageSend( vSubtask, "HL00001", cpcFct, szMsg, lMsgType, FALSE );
   if ( nRC < 0 )
   {
      strcpy_s( szMsg, zsizeof( szMsg ), "HL00001 | zwTZHLPCXD_ErrMsg | " );
      strcat_s( szMsg, zsizeof( szMsg ), "zMSGQ_OBJECT_CONSTRAINT_ERROR | " );
      strcat_s( szMsg, zsizeof( szMsg ), "MessageSend: " );
      strcat_s( szMsg, zsizeof( szMsg ), "Error processing error message." );
      TraceLineI( szMsg, nRC );
   }

   // Write data to Zeidon Object Services window if environment variable
   // 'HLP_TRACE_ON' is set and has value "1"
   nRC = (zSHORT) GetEnvironmentVariable( "HLP_TRACE_ON", vTrace, 2 );
   if ( !zstrcmp( vTrace, "1" ) && nRC > 0 )
   {
      strcat_s( szMsg, zsizeof( szMsg ), cpcFct );
      strcat_s( szMsg, zsizeof( szMsg ), " | " );
      zltoa( lMsgType, rcStr );
      strcat_s( szMsg, zsizeof( szMsg ), rcStr );
      strcat_s( szMsg, zsizeof( szMsg ), " | " );
      strcat_s( szMsg, zsizeof( szMsg ), szMsg );
      strcat_s( szMsg, zsizeof( szMsg ), ": " );
      TraceLineI( szMsg, returnCode );
   }

   return( nRC );
}

#ifdef __cplusplus
}
#endif
