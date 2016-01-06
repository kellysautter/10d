/////////////////////////////////////////////////////////////////////////////
//
// KZHGENKY.C - Genkey handler for SQLBase
//
// Programming Notes:
//
//    o  All function return codes use the following convention:
//       - rc = 0 means that the function worked as intended.
//       - rc > 0 means that the function worked but with warnings.
//       - rc < 0 means that an error occured.
//
///////////////////////////// DEFINES ///////////////////////////////////////

/*
CHANGE LOG

2002.03.28  HH
   R56268, improve error handling when enq record was left hanging.

1999.07.02  DGC  10a
   When committing the genkey object let's force a 'COMMIT' to the database.

1999.03.16  DGC  10a
   Changed logic to use lpViewEntity->lEREntTok as an ID instead of casting
   lpViewEntity (the pointer value) to a long.  I'm trying to stop using
   pointers as long integer values (for possible conversion to AS400).

1999.05.12  DGC  10a
   Added mutexes to single-thread the generation of genkeys.

*/

#define KZHGENKY_Version 26

#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include "kzoengaa.h"
#include <stdio.h>


/////////////////////////// FUNCTION PROTYPES //////////////////////////////
// Include prototype file
#include "kzhgenky.h"

/////////////////////////////////////////////////////////////////////////////
//
// KZHGENKY_GKH_MsgProc
//
// The entry point for the db-handler.  All calls go through here.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
GKH_MsgProc( zUSHORT      Message,       // Message indicating function, etc.
             zVIEW        lpView,        // Target/Source view.
             LPVIEWOD     lpViewOD,      // Target/Source object defintion.
             LPVIEWENTITY lpViewEntity,  // Entity to create Genkey for.
             zVIEW        lpKZGKHWOB )   // Genkey work object.
{
   zSHORT nRC;

   // Execute a function according to the message passed.
   if ( Message == (Message | DBH_Init) )
   {
      nRC = 0;
   }
   else
   if ( Message == (Message | DBH_StartGenkey) )
   {
      nRC = InitializeGenkeyObject( lpView, lpViewOD, lpKZGKHWOB );
      if ( nRC == -1 )
         nRC = zCALL_ERROR;
   }
   else
   if ( Message == (Message | DBH_Rollback) )
   {
      // We don't need to do anything here--the object was committed in
      // DBH_StartGenkey.
      nRC = 0;
   }
   else
   if ( Message == (Message | DBH_Commit) )
   {
      // We don't need to do anything here--the object was committed in
      // DBH_StartGenkey.
      nRC = 0;
   }
   else
   if ( Message == (Message | DBH_GetGenkey) )
   {
      nRC = GetNextGenkey( lpView, lpViewEntity, lpKZGKHWOB );
   }
   else
   if ( Message == (Message | DBH_Term) )
   {
      nRC = 0;
   }

   return( nRC );
} // KZHGENKY_GKH_MsgProc

//=========================================================================
//       ENTITY FUNCTIONS
//=========================================================================

/////////////////////////////////////////////////////////////////////////////
//
//    InitializeGenkeyObject
//
//    Opens the Genkey entity and finds the current genkey values.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
InitializeGenkeyObject( zVIEW    vSubtask,
                        LPVIEWOD lpViewOD,
                        zVIEW    lpKZGKHWOB )
{
   zPCHAR   szTableName;
   zVIEW    lpGENKEYWO;
   zLONG    lLong;
   zSHORT   nTraceLevel;
   zSHORT   nReturnCode;
   zSHORT   nRC;

   nReturnCode = 0;
   nTraceLevel = GetTaskDBHandlerTraceLevel( vSubtask );
   if ( nTraceLevel > 0 )
   {
      TraceLineS( "==================================================", "" );
      TraceLineS( "==================  Genkey Creation   ==============", "" );
      TraceLineS( "==================================================", "" );
      TraceLineI( "KZHGENKY Version 1.", (zLONG) KZHGENKY_Version );
   }

#ifdef DATABASE_LOCK

   nRC = ActivateEmptyObjectInstance( &lpGENKEYWO, "GENKEYWO", vSubtask,
                                      zMULTIPLE );
   if ( nRC )
   {
      IssueError( vSubtask, 16, 16, "Error activating GENKEYWO object." );
      if ( nTraceLevel > 0 )
      {
         TraceLineS( "", "" );
         TraceLineS( "============= End of Genkey Creation =============", "" );
         TraceLineS( "", "" );
      }

      return( zCALL_ERROR );
   }

   SetNameForView( lpGENKEYWO, "_GENKEYWO", 0, zLEVEL_TASK );

   // Create enqueue entity.
   nRC = CreateEntity( lpGENKEYWO, "ZeidonGenkeyTable", zPOS_FIRST );
   if ( nRC )
   {
      DropView( lpGENKEYWO );
      return( zCALL_ERROR );
   }

   nRC = SetAttributeFromString( lpGENKEYWO, "ZeidonGenkeyTable", "TableName",
                                 "..ENQ KZHGENKY" );
   if ( nRC )
   {
      DropView( lpGENKEYWO );
      return( zCALL_ERROR );
   }

   nRC = SetAttributeFromInteger( lpGENKEYWO, "ZeidonGenkeyTable",
                                  "CurrentGenkey", 999 );
   if ( nRC )
   {
      DropView( lpGENKEYWO );
      return( zCALL_ERROR );
   }

   // Try to commit the OI with the locking entity.  If it succeeds, then
   // nobody else is currently using the GenKey database object.  The
   // creation of the entity keeps other processes from using the GenKey
   // object until we are done with it.  If the Commit fails, then someone
   // else is currently using the GenKey datbase object.
#if 0
   nRC = CommitMultipleOIs( &lpGENKEYWO, 1, 0, zCOMMIT_FORCETRAN );
#else
   nRC = CommitObjectInstance( lpGENKEYWO );
#endif
   // Whether the commit succeeded or not, we don't need the OI anymore.
   DropView( lpGENKEYWO );
   if ( nRC )
   {
      IssueError( vSubtask, 16, 16,
         "Error writing ENQ entity - Check ZeidonGenkeyTable" );

      if ( nTraceLevel > 0 )
      {
         TraceLineS( "", "" );
         TraceLineS( "============= End of Genkey Creation =============", "" );
         TraceLineS( "", "" );
      }

      return( zCALL_ERROR );
   }

#else  // DATABASE_LOCK

   // Lock a mutex.
   SysMutexLock( vSubtask, "Zeidon GKH", 0, 0 );

#endif // DATABASE_LOCK else...

   // Now retrieve all GenKey objects from the database.
   nRC = ActivateObjectInstance( &lpGENKEYWO, "GENKEYWO", vSubtask, 0,
                                 zMULTIPLE );
   SetNameForView( lpGENKEYWO, "_GENKEYWO", 0, zLEVEL_TASK );

#if 0
   // A hack...Loop through all the entities and delete any trailing spaces
   // from the entity names.  Note that because we get a pointer to the
   // entity name and then use that to change the string the entity does not
   // get flagged as updated.
   for ( nRC = SetCursorFirstEntity( lpGENKEYWO, "ZeidonGenkeyTable", 0 );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( lpGENKEYWO, "ZeidonGenkeyTable", 0 ) )
   {
      zSHORT k;

      GetAddrForAttribute( &szTableName, lpGENKEYWO, "ZeidonGenkeyTable",
                           "TableName" );
      k = zstrlen( szTableName ) - 1;
      while ( szTableName[ k ] == ' ' )
         szTableName[ k-- ] = 0;
   }
#endif

   // Now loop through the OI containing the names of entities that need
   // genkeys.  For each one, find the entity in the GenKey list that has
   // the same name and get the current GenKey value.
   for ( nRC = SetCursorFirstEntity( lpKZGKHWOB, "Genkey", 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( lpKZGKHWOB, "Genkey", 0 ) )
   {
      // Get name of user entity that needs a genkey.
      GetAddrForAttribute( &szTableName, lpKZGKHWOB, "Genkey", "TableName" );

      // Find the entity in the genkey list that has the same name as the
      // user's entity.
      nRC = SetCursorFirstEntityByString( lpGENKEYWO, "ZeidonGenkeyTable",
                                          "TableName", szTableName, "" );

      // If the entity wasn't found, then this is the first time a genkey has
      // been requested for this entity.  Create the entity in the GenKey
      // list and initialize any values.
      if ( nRC != zCURSOR_SET )
      {
         CreateEntity( lpGENKEYWO, "ZeidonGenkeyTable", zPOS_FIRST );
         SetAttributeFromString( lpGENKEYWO, "ZeidonGenkeyTable",
                                 "TableName", szTableName );
         SetAttributeFromInteger( lpGENKEYWO, "ZeidonGenkeyTable",
                                 "CurrentGenkey", 0 );
      }

      // Copy the GenKey value from the GenKey list to the GenKey handler
      // work object.  This is how the value is passed back to CommitObj....
      SetAttributeFromAttribute( lpKZGKHWOB, "Genkey", "CurrentGenkey",
                                 lpGENKEYWO, "ZeidonGenkeyTable",
                                 "CurrentGenkey" );

      // Retrieve the count of entity instances that need genkeys and add it
      // to the current value in the genkey list.
      GetIntegerFromAttribute( &lLong, lpKZGKHWOB, "Genkey", "EntityCount" );
      nRC = AddToAttributeFromInteger( lpGENKEYWO, "ZeidonGenkeyTable",
                                       "CurrentGenkey", lLong );
      if ( nRC )
      {
         nReturnCode = zCALL_ERROR;
         break;
      }
   }

#ifdef DATABASE_LOCK

   // Delete Enqueue entity from Genkey OI.
   nRC = SetCursorFirstEntityByString( lpGENKEYWO, "ZeidonGenkeyTable",
                                       "TableName", "..ENQ KZHGENKY", "" );
   if ( nRC != zCURSOR_SET )
   {
      IssueError( vSubtask, 16, 16, "Error - can't find ..ENQ entity" );
      nReturnCode = zCALL_ERROR;
   }
   else
      DeleteEntity( lpGENKEYWO, "ZeidonGenkeyTable", zREPOS_AFTER );

#endif

   // Commit the GenKey list.  This will delete the enqueue entity from the
   // database to allow other processes to create genkeys and will also
   // create any entities that have had genkeys requested for the first time.
#if 1 // DGC 99.07.02
   {
      ViewClusterRecord Cluster;

      Cluster.vOI      = lpGENKEYWO;
      Cluster.lControl = 0;

      nRC = CommitMultipleOIs( &Cluster, 1, 0, zCOMMIT_FORCETRAN );
   }
#else
   nRC = CommitObjectInstance( lpGENKEYWO );
#endif
   if ( nRC )
      nReturnCode = zCALL_ERROR;

#ifndef DATABASE_LOCK

   SysMutexUnlock( vSubtask, "Zeidon GKH", 0 );

#endif

   if ( nTraceLevel > 0 )
   {
      TraceLineS( "", "" );
      TraceLineS( "============= End of Genkey Creation =============", "" );
      TraceLineS( "", "" );
   }

   DropView( lpGENKEYWO );

   return( nReturnCode );
}  /* InitializeGenkeyObject */

/////////////////////////////////////////////////////////////////////////////
//
//    GetNextGenkey
//
//    Sets the genkey attribute in lpView for the current lpViewEntity.
//
//    RETURNS:       0 - OK
//         zCALL_ERROR - Error.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT GetNextGenkey ( zVIEW        lpView,
                       LPVIEWENTITY lpViewEntity,
                       zVIEW        lpKZGKHWOB )
{
   zSHORT       nRC;
   zLONG        lGenkey;
   LPVIEWATTRIB lpViewAttrib;

   nRC = SetCursorFirstEntityByInteger( lpKZGKHWOB, "Genkey", "EntityID",
                                        lpViewEntity->lEREntTok, "" );
   if ( nRC != zCURSOR_SET )
   {
      IssueError( lpView, 16, 16, "lpViewEntity not found in KZGKHWOB" );
      return( zCALL_ERROR );
   }

   GetIntegerFromAttribute( &lGenkey, lpKZGKHWOB, "Genkey", "CurrentGenkey" );
   lGenkey++;
   SetAttributeFromInteger( lpKZGKHWOB, "Genkey", "CurrentGenkey", lGenkey );

   // Find the genkey view attrib.
   lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
   while ( lpViewAttrib->bGenkey == FALSE )
      lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );

   // Copy the genkey to the entity.
   StoreValueInRecord( lpView, lpViewEntity, lpViewAttrib,
                       (zPVOID) &lGenkey, sizeof( lGenkey ) );

   return( 0 );
}  /* GetNextGenkey */
