/////////////////////////////////////////////////////////////////////////////

   KZHDBVAA.C - Database handler for Raima Data Manager.

   Programming Notes:

      o  All function return codes use the following convention:
         - rc = 0 means that the function worked as intended.
         - rc > 0 means that the function worked but with warnings.
         - rc < 0 means that an error occured.

      o  Max internal error number = 6

   The following are not handled:

      o  Every entity is assumed to have one and only one unique key.  If and
         when this is changed, some of the code that needs to be changed is
         marked by the comment "ONEKEY".

      o  Only one database is allowed to be open.  This can easily be changed
         if the PathTable information is stored in the KZDBHWOB object in the
         Database entity instead of the root.

      o  Root qualifications can only be made using the key.  Sub-entity
         qualifications are not allowed.

///////////////////////////// DEFINES ///////////////////////////////////////
#define MultiUser   1
#define DBQuery     0
#define Transaction 1
#define KZHDBVAA_Version 165

#define TRACE 1
#if TRACE
   #define RDM_RC( a, b, c, d, e )  RaimaDM_RC( a, b, c, d, e, lpConnection )
#else
   #define RDM_RC( a, b, c, d, e )  ;
#endif

#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include <kzoengaa.h>
#include <zdrvropr.h>
#include <stdio.h>

// Following define tells Raima that we are multi-tasking.
#ifndef MULTI_TASK
#define MULTI_TASK
#endif

#if DBQuery
#include "Query.h"
#else
#include "Vista.h"
#endif

#define OI_PATH                    0
#define INS_REL                    1
#define QUAL_PATH                  2


#define zLOC_USE_PARENT            0x0001
#define zLOC_NO_USE_PARENT         0x0002
#define zLOC_CONNECT               0x0004
#define zLOC_NO_CONNECT            0x0008

#define zOVERFLOW_FLAG             0xfffefffeL

///////////////////////////// TYPEDEF'S /////////////////////////////////////

struct Overflow_Struct
{
   DB_ADDR dbNextOverflowRecord;
   zCHAR   OverflowLine[ 2 ];
};
typedef struct Overflow_Struct OverflowRecord;

struct Overflow_Indicator_Struct
{
   zLONG   lFlag;
   zUSHORT uLth;
   DB_ADDR dbFirstOverflowRecord;
   zCHAR   cBeginningOfData;
};
typedef struct Overflow_Indicator_Struct OverflowIndicatorRecord;

typedef struct KeyStruct *LPKEY;
typedef struct KeyStruct KeyRecord;
struct KeyStruct
{
   zLONG        hMemory;
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   LPKEY        lpNextKey;
   zLONG        hValueMemory;
   zPVOID       lpKeyValue;
   zUSHORT      uLth;
};

typedef struct EntityStruct *LPENTITY;
typedef struct EntityStruct EntityRecord;
struct EntityStruct
{
   LPKEY        lpFirstKey;
   LPVIEWENTITY lpViewEntity;
// DB_ADDR      dbaAddr;
   zSHORT       nKeyCnt;
};

typedef struct Set_Struct *LPSET;
typedef struct Set_Struct SetRecord;
struct Set_Struct
{
   zLONG        hMemory;
   LPSET        lpNextSet;
   LPSET        lpPrevSet;
   zPCHAR       szSetName;
   zSHORT       nSetID;

   EntityRecord Child;
};

struct PathStruct
{
   LPSET  lpFirstSet;
   LPSET  lpLastSet;
   zSHORT nSetCount;
};
typedef struct PathStruct PathRecord;
typedef struct PathStruct *LPPATH;
typedef struct PathStruct PathTable[ 5 ];
typedef PathTable *LPPATHTABLE;

typedef struct OverflowDeleteStruct OverflowDeleteRecord;
typedef struct OverflowDeleteStruct * LPOVERFLOW;
struct OverflowDeleteStruct
{
   DB_ADDR    dba;
   LPOVERFLOW lpNextOverflowDelete;
   zLONG      hMem;
};

typedef struct ConnectionArea ConnectionRecord;
typedef ConnectionRecord *LPCONNECTION;
struct ConnectionArea
{
   zVIEW       vDbhWork;
   zVIEW       lpAppView;
#if TRACE
   zSHORT      nTraceLevel;
#endif
   PathTable   lpPathTable;
   zPCHAR      RecordBuffer;
   zLONG       hRecordMem;
   zPCHAR      IntersectBuffer;
   zLONG       hIntersectionMem;
   zUSHORT     nOVERFLOW_BUFFER_LTH;
   zUSHORT     nOVERFLOW_RECORD_ID;
   DB_TASK     Currtask;                      // RDM current task.
   zBOOL       bValidConnection;
   LPOVERFLOW  lpOverflowDeleteChain;
};

///////////////////////////// GLOBAL VARIABLES //////////////////////////////
#define RECORDBUFFERLTH       64000
#define INTERSECTIONBUFFERLTH 1000
zCHAR   szAlphaNumericString[] = "1234567890ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/////////////////////////// FUNCTION PROTYPES //////////////////////////////
// Include prototype file
#include "KZHDBVPR.H"

/////////////////////////////////////////////////////////////////////////////

  KZHDBVAA_DBH_MsgProc

  The entry point for the db-handler.  All calls go through here.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
KZHDBVAA_DBH_MsgProc( LPTASK       hTask,
                      zUSHORT      Message,       // Message indicating function, etc.
                      int          Indicators,    // Indicators for function.
                      LPVIEWOD     lpViewOD,      // Target/Source object defintion.
                      LPVIEWENTITY lpViewEntity,  // Target/Source entity.
                      zVIEW        lpView,        // Target/Source view.
                      zVIEW        lpQualView,    // Qualification view.
                      zPVOID       lpWorkPtr )    // Work pointer.
{
   LPCONNECTION lpConnection;
   zSHORT       nRC;

   if ( *((zPVOID *) lpWorkPtr) )
   {
      lpConnection = *((LPCONNECTION *) lpWorkPtr);
      if ( lpView )
         lpConnection->lpAppView = lpView;
   }
   else
      lpConnection = 0;

   // Execute a function according to the message passed.
   if ( Message & DBH_Entity )
   {
      // Copy overflow record information to global variables.
      lpConnection->nOVERFLOW_RECORD_ID  = (zUSHORT) lpViewOD->nOfRecID;
      lpConnection->nOVERFLOW_BUFFER_LTH = (zUSHORT) lpViewOD->nOfBufSz;

      // Retrieve current parent path from lpView + lpViewEntity.
      nRC = GetParentPathFromOI( lpView, lpViewEntity, Message, lpConnection );
      if ( nRC < 0 )
         ; // do nothing if there was an error in GetParentPath...
      else
      if ( Message & DBH_Load )
         nRC = Load( lpViewOD, lpViewEntity, lpView, lpQualView,
                     lpConnection, Indicators );
      else
      if ( Message & DBH_Insert )
         nRC = Insert( lpViewOD, lpViewEntity, lpView, lpConnection );
      else
      if ( Message & DBH_InsertRel )
         nRC = InsertRel( lpViewOD, lpViewEntity, lpView, lpConnection );
      else
      if ( Message & DBH_Delete )
         nRC = Delete( lpViewOD, lpViewEntity, lpView, lpConnection );
      else
      if ( Message & DBH_DeleteRel )
         nRC = DeleteRel( lpViewOD, lpViewEntity, lpView, lpConnection );
      else
      if ( Message & DBH_Update )
         nRC = Update( lpViewOD, lpViewEntity, lpView, FALSE, lpConnection );
      else
      if ( Message & DBH_UpdateSeq )
         nRC = Update( lpViewOD, lpViewEntity, lpView, TRUE, lpConnection );
      else
      {
         DBH_Error( lpView, "Invalid sub-message for Entity message.", 0, 0 );
         return( zCALL_ERROR );
      }

      if ( nRC == -1 )
      {
         TraceLineS( "", "" );
         if ( Message & DBH_Load )
            TraceLineS( "Error performing Load for OD: ", lpViewOD->szName );
         else
         if ( Message & DBH_Insert )
            TraceLineS( "Error performing Insert for OD: ", lpViewOD->szName );
         else
         if ( Message & DBH_InsertRel )
            TraceLineS( "Error performing InsertRel for OD: ", lpViewOD->szName );
         else
         if ( Message & DBH_Delete )
            TraceLineS( "Error performing Delete for OD: ", lpViewOD->szName );
         else
         if ( Message & DBH_DeleteRel )
            TraceLineS( "Error performing DeleteRel for OD: ", lpViewOD->szName );
         else
         if ( Message & DBH_Update )
            TraceLineS( "Error performing Update for OD: ", lpViewOD->szName );

         if ( lpViewEntity->hParent )
         {
            TraceLineS( "PARENT:", "" );
            DisplayEntityInstance( lpView, lpViewEntity->hParent->szName );
            TraceLineS( "", "" );
            TraceLineS( "CHILD:", "" );
         }

         DisplayEntityInstance( lpView, lpViewEntity->szName );
      }
   }
   else
   if ( Message & DBH_Object )
   {
      if ( Message & DBH_TranBegin )
         nRC =  BeginTransaction( Indicators, lpViewOD, lpView, lpQualView,
                                  lpWorkPtr );
      else
      if ( Message & DBH_Commit )
      {
         nRC =  CommitTransaction( lpConnection );
         dt_closeall ( &lpConnection->Currtask );
      }
      else
      if ( Message & DBH_Rollback )
      {
         nRC =  RollbackTransaction( lpConnection );
         dt_closeall ( &lpConnection->Currtask );
      }
      else
      {
         DBH_Error( lpView, "Invalid sub-message for Object message.", 0, 0 );
         return( zCALL_ERROR );
      }
   }
   else
   if ( Message & DBH_Init )
      nRC = InitDatabase( lpView );
   else
// if ( Message & (DBH_Term | DBH_TaskCloseAll) )
   if ( Message & DBH_Term )
      nRC = CloseDatabase( lpView, Indicators );
   else
   if ( Message & DBH_TaskCloseAll )
      nRC = CloseDatabaseFiles( lpView, Indicators );

   if ( nRC == -1 )
      return( zCALL_ERROR );
   else
   if ( nRC == -2 )
      return( zDB_UNAVAIL );
   else
   if ( nRC == 2 )
      return( -1 );      // Nothing was loaded (entity not found).
   else
   if ( nRC == 5 )
   {
      if ( lpViewEntity->hParent )
         return( zCALL_ERROR );
      else
         return( zDUPLICATE_ROOT );
   }
   else
      return( nRC );

} // KZHDBVAA_DBH_MsgProc

//=========================================================================
//       ENTITY FUNCTIONS
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      InitDatabase

/////////////////////////////////////////////////////////////////////////////
zSHORT
InitDatabase( zVIEW lpView )
{
   zCHAR        szStr[ 2 ];
   zVIEW        vDbhWork;
   zSHORT       nRC;

   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );
   if ( vDbhWork == 0 )
   {
      DBH_Error( lpView, "Can't find DB-Handler work view.", 0, 0 );
      return( -1 );
   }

   szStr[ 0 ] = zDBH_RAIMADM;
   szStr[ 1 ] = 0;

   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      CreateEntity( vDbhWork, "Type", zPOS_AFTER );
      SetAttributeFromString( vDbhWork, "Type", "Type", szStr );
   }

   return( 0 );
}  /* InitDatabase */

/////////////////////////////////////////////////////////////////////////////

      CloseDatabaseFiles

      Closes files held by DB_Vista

/////////////////////////////////////////////////////////////////////////////
zSHORT
CloseDatabaseFiles( zVIEW lpView, int Indicators )
{
   zCHAR        szStr[ 2 ];
   zVIEW        vDbhWork;
   zLONG        lProcessID;
   LPCONNECTION lpConnection;
   zSHORT       nRC;

   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );
   if ( vDbhWork == 0 )
   {
      TraceLineS( "Can't find the DB-Handler work view.", "" );
      return( -1 );
   }

   CreateViewFromViewForTask( &vDbhWork, vDbhWork, 0 );
   SfLockView( vDbhWork );

   szStr[ 0 ] = zDBH_RAIMADM;
   szStr[ 1 ] = 0;
   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      DBH_Error( vDbhWork, "Error #6 -- cannot locate 'Type' entity.", 0, 0 );
      return( -1 );
   }

   hTask = (LPTASK) SysGetTaskFromView( lpView );
   sprintf_s( szLogicalName, "0x%08x:%08x", hTask, SysGetProcessID( 0 ) );
   nRC = SetCursorFirstEntityByString( vDbhWork, "Connection",
                                       "LogicalUserName", szLogicalName, "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( (zPVOID) &lpConnection, vDbhWork,
                           "Connection", "ConnectionBlob" );
      if ( lpConnection->bValidConnection )
      {
         nRC = dt_closeall( &lpConnection->Currtask );
         RDM_RC( "dt_closeall", nRC, 0, 0, 1 );
         if ( nRC )
            return( -1 );
      }

      nRC = SetCursorNextEntityByString( vDbhWork, "Connection",
                                         "LogicalUserName", szLogicalName, "" );
   }

   return( 0 );
}  /* CloseDatabaseFiles */

/////////////////////////////////////////////////////////////////////////////

      CloseDatabase

      Frees any memory allocate by the db-handler and closes all databases.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CloseDatabase( zVIEW lpView, int Indicators )
{
   zCHAR        szStr[ 2 ];
   zVIEW        vDbhWork;
   zLONG        lProcessID;
   LPCONNECTION lpConnection;
   zSHORT       nRC;

   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );
   if ( vDbhWork == 0 )
   {
      TraceLineS( "Can't find the DB-Handler work view.", "" );
      return( -1 );
   }

   CreateViewFromViewForTask( &vDbhWork, vDbhWork, 0 );
   SfLockView( vDbhWork );

   szStr[ 0 ] = zDBH_RAIMADM;
   szStr[ 1 ] = 0;
   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      DBH_Error( vDbhWork, "Error #6 -- cannot locate 'Type' entity", 0, 0 );
      return( -1 );
   }

   hTask = (LPTASK) SysGetTaskFromView( lpView );
   sprintf_s( szLogicalName, "0x%08x:%08x", hTask, SysGetProcessID( 0 ) );
   nRC = SetCursorFirstEntityByString( vDbhWork, "Connection",
                                       "LogicalUserName", szLogicalName, "" );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( (zPVOID) &lpConnection, vDbhWork,
                           "Connection", "ConnectionBlob" );

      if ( lpConnection->hIntersectionMem )
         SysFreeMemory( lpConnection->hIntersectionMem );

      if ( lpConnection->hRecordMem )
         SysFreeMemory( lpConnection->hRecordMem );

      if ( lpConnection->bValidConnection )
      {
         #if DBQuery
            nRC = q_close( );
            RDM_RC( "q_close", nRC, 0, 0, 1 );
         #else
            nRC = dt_close ( &lpConnection->Currtask );
            RDM_RC( "d_close", nRC, 0, 0, 1 );
         #endif
            if ( nRC )
               return( -1 );

         // Close tasks.
         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif
      }

      SetAttributeFromBlob( vDbhWork, "Connection", "ConnectionBlob", 0, 0 );
      DeleteEntity( vDbhWork, "Connection", zREPOS_AFTER );
      nRC = SetCursorNextEntityByString( vDbhWork, "Connection",
                                         "LogicalUserName", szLogicalName, "" );
   }

   return( 0 );
}  /* CloseDatabase */

/////////////////////////////////////////////////////////////////////////////

      BeginTransaction

      Checks to see if the database has already been opened and opens it
      if needed.

      Starts RDM transaction.

/////////////////////////////////////////////////////////////////////////////
zSHORT
BeginTransaction( int      Indicators,
                  LPVIEWOD lpViewOD,
                  zVIEW    lpView,
                  zVIEW    lpQualView,
                  zPVOID   lpWorkPtr )
{
#define MAX_LOCKS 400

#if TRACE
   zSHORT nTraceLevel;
#endif
   zSHORT nIdx;
   zSHORT nLastChar;
   zLONG  lProcessID;
   LPTASK lpTask;
   zVIEW  vDbhWork;
   zVIEW  lpTemp;
   zPCHAR szDatabaseName;
   zCHAR  szPath[ 128 ];
   zCHAR  szFullDBN[ 128 ];
   zCHAR  szOpenType[ 2 ];
   zCHAR  szWork[ 200 ];
   zCHAR  szUserId[ 50 ];
   zCHAR  szStr[ 2 ];
   zCHAR  szLogicalName[ 256 ];
   zCHAR  szUniqueDatabaseID[ 9 ];
   zCHAR  szPassword[ 20 ];
   zCHAR  cLockType;
   zLONG  hMem;
   zLONG  lEnqFile;
   int    nLockRequestCount;
   zPCHAR lpLockName[ MAX_LOCKS ];

   LOCK_REQUEST  *lpLockRequest;
   LPVIEWENTITY       lpViewEntity;
   LPDATARECORD       lpDataRecord;
   LPRELRECORD        lpRelRecord;
   ConnectionRecord   ConnRec;
   LPCONNECTION       lpConnection;
   zSHORT             nRC;

   *((zPVOID *) lpWorkPtr) = 0;
   GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME, lpView, zLEVEL_SYSTEM );
   if ( vDbhWork == 0 )
   {
      DBH_Error( lpView, "Can't find DB-Handler work view.", 0, 0 );
      return( -1 );
   }

   CreateViewFromViewForTask( &vDbhWork, vDbhWork, 0 );
   SfLockView( vDbhWork );

#if TRACE
   nTraceLevel = GetTaskDBHandlerTraceLevel( lpView );
   if ( nTraceLevel >= 100 )
   {
      if ( Indicators & DBH_StoreOI )
         nTraceLevel -= 100;
      else
         nTraceLevel -= 200;

      if ( nTraceLevel < 0 || nTraceLevel > 3 )
         nTraceLevel = 0;
   }
#endif

   if ( lpViewOD->szDfltDBName[ 0 ] == 0 )
   {
      DBH_Error( lpView, "Object has no database name", 0, 0 );
      DropView( vDbhWork );
      return( -1 );
   }
   else
      szDatabaseName = lpViewOD->szDfltDBName;

   // Gig, wait for backup or restore process to complete
   lEnqFile = -1;
   if ( zstrcmp( szDatabaseName, "ZMM" ) == 0 )
   {
      SysReadZeidonIni( -1, "[RaimaDM_dir]", szDatabaseName, szPath );
      strcpy_s( szFullDBN, zsizeof( szFullDBN ), szPath );
      strcat_s( szFullDBN, zsizeof( szFullDBN ), "\\ZMMBK.LOK" );
   // TraceLineS( "(kzhdbvaa) Checking for lock file : ", szFullDBN );
      if ( SysOpenFile( lpView, szFullDBN, COREFILE_EXIST ) >= 0 )
      {
         int k;
         WORD wErrMode;

      // strcat_s( szFullDBN, zsizeof( szFullDBN ), ".LOK" );
         for ( ; ; )
         {
            for ( k = 0 ; k < 5 ; k++ )
            {
            // TraceLineS( "(kzhdbvaa) Opening lock file : ", szFullDBN );
               wErrMode = SetErrorMode( SEM_NOOPENFILEERRORBOX );
               lEnqFile = SysOpenFile( lpView, szFullDBN, COREFILE_UPDATE );
            // TraceLineI( "                 File handle : ", lEnqFile );
               SetErrorMode( wErrMode );
               if ( lEnqFile >= 0 )
                  break;

               SysWait( 2000 );
            }

            if ( k < 5 )
               break;

            if ( OperatorPrompt( 0, "Database Backup",
                                 "The repository is temporarily in use "
                                 "for backup or restore, would you like to "
                                 "retry the transaction?",
                                 0, zBUTTONS_YESNO, zRESPONSE_YES, 0 ) ==
                                                                zRESPONSE_NO )
            {
               DBH_Error( lpView,
                          "User Terminated due to Backup in progress.", 0, 0 );
               return( -1 );
            }
         }
      }
   }

   // Gig, we've ensured the backup or restore process is not taking
   // place. Close the LOK file NOW INSTEAD OF WAITING until after the
   // Transaction start is complete. The code to close the file is still
   // below in case we decide to leave the lock file open longer
   if ( lEnqFile >= 0 )
   {
   // TraceLineI( "(kzhdbvaa) closing Enqueue file ", lEnqFile );
      SysCloseFile( lpView, lEnqFile, 0 );
      lEnqFile = -1;
   }
   // Gig, wait for backup or restore process to complete

#if TRACE
   if ( nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "##################  BEGIN TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
      TraceLineI( "KZHDBVAA Version 1.", (zLONG) KZHDBVAA_Version );
      if ( Indicators & DBH_StoreOI )
         TraceLineS( "Storing Object def = ", lpViewOD->szName );
      else
         TraceLineS( "Activating Object def = ", lpViewOD->szName );

      TraceLineS( "Database name = ", szDatabaseName );
   }
#endif

   // Check to see if the database needs to be opened.  If there is a
   // Database entity in the work object with a szDatabaseName equal to the
   // name of the database as specified in the OD, then the database has been
   // opened.  If there is no database entity with a matching name, then the
   // database needs to be opened.
   szStr[ 0 ] = zDBH_RAIMADM;
   szStr[ 1 ] = 0;
   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      DBH_Error( lpView, "Internal error -- cannot find SQLServer type in "
                 "db-handler work object.", 0, 0 );
      DropView( vDbhWork );
      if ( lEnqFile >= 0 )
         SysCloseFile( lpView, lEnqFile, 0 );

      return( -1 );
   }

   // Retrieve user id.
   szUserId[ 0 ] = 0;
   SysGetDB_UserID( lpView, szUserId, zsizeof( szUserId ), szPassword, zsizeof( szPassword ) );
   if ( szUserId[ 0 ] == 0 )
   {
      SysReadZeidonIni( -1, "[RaimaDM_dir]", "DBUserID", szUserId );
      if ( szUserId[ 0 ] == 0 )
      {
         DBH_Error( lpView, "User ID not found in ZEIDON.INI", 0, 0 );
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
   }

   if ( zstrlen( szUserId ) > 7 )
   {
//    zCHAR szTemp[ 500 ];

//    strcpy_s( szTemp, zsizeof( szTemp ), "Your Raima Database User ID specified in ZEIDON.INI "
//             "is greater than 7 characters.\n\nDatabase User ID = '" );
//    strcat_s( szTemp, zsizeof( szTemp ), szUserId );
//    strcat_s( szTemp, zsizeof( szTemp ), "'\n\nOK to truncate ID to 7 characters and continue?" );

      nRC = zRESPONSE_YES;
//    nRC = OperatorPrompt( lpView, "Database Handler Warning", szTemp,
//                          0, zBUTTONS_YESNO, 1, 0 );
      if ( nRC == zRESPONSE_YES )
         szUserId[ 7 ] = 0;
      else
      {
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
   }

   hTask = (LPTASK) SysGetTaskFromView( lpView );
   sprintf_s( szLogicalName, "0x%08x:%08x", hTask, SysGetProcessID( 0 ) );
   nRC = SetCursorFirstEntityByString( vDbhWork, "Connection",
                                       "LogicalUserName", szLogicalName, "" );
   if ( nRC != zCURSOR_SET )
   {
      // Initialize connection record.
      zmemset( (zPVOID) &ConnRec, 0, zsizeof( ConnectionRecord ) );
      lpConnection = &ConnRec;
      lpConnection->bValidConnection = FALSE;
#if TRACE
      lpConnection->nTraceLevel      = nTraceLevel;
#endif

      // Allocate record buffers
      lpConnection->hIntersectionMem =
                 SysAllocMemory( &lpConnection->IntersectBuffer,
                                 INTERSECTIONBUFFERLTH, 0,
                                 zCOREMEM_ALLOC, 0 );
      if ( lpConnection->IntersectBuffer == 0 )
      {
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

      lpConnection->hRecordMem = SysAllocMemory( &lpConnection->RecordBuffer,
                                                 RECORDBUFFERLTH, 0,
                                                 zCOREMEM_ALLOC, 0 );
      if ( lpConnection->RecordBuffer == 0 )
      {
         SysFreeMemory( lpConnection->hIntersectionMem );
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

   // CreateViewFromViewForTask( &lpConnection->vDbhWork, vDbhWork );
   // vDbhWork = lpConnection->vDbhWork;
   // SetNameForView( vDbhWork, "DG Temp View raimadm", 0, zLEVEL_SYSTEM );

      // Create db-vista task.
      nRC = dt_opentask( &lpConnection->Currtask );
      RDM_RC( "dt_opentask", nRC, 0, 0, 2 );
      if ( nRC != S_OKAY )
      {
         DBH_Error( lpView, "Error starting Task.", 0, 0 );
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

      // Set up local database error handler.
      dt_set_dberr( LocalDBError, &lpConnection->Currtask );

#if DBQuery
      // Create db-query task.
      nRC = qt_opentask( &lpConnection->Currtask );
      RDM_RC( "qt_opentask", nRC, 0, 0, 2 );
      if ( nRC != Q_OKAY )
      {
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
#endif

      // Set up the TAF directory.
      szWork[ 0 ] = 0;
      SysReadZeidonIni( -1, "[RaimaDM_dir]", "TAF", szWork );
      if ( szWork[ 0 ] == 0 )
      {
         DBH_Error( lpView, "TAF not found in ZEIDON.INI", 0, 0 );

         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
      else
      {
         // Make sure that last char is a backslash.
         nLastChar = zstrlen( szWork );
         if ( szWork[ nLastChar - 1 ] != '\\' )
         {
            DBH_Error( lpView,
                       "RaimaDM DBH -- TAF directory setting must end with "
                       "a backslash.  Edit ZEIDON.INI and make correction.",
                       "\nTAF = ", szWork );
            nRC = -1;
         }
         else
            nRC = S_OKAY;

         if ( nRC == S_OKAY )
         {
            nRC = dt_dbtaf( szWork, &lpConnection->Currtask );
            RDM_RC( "d_dbtaf", nRC, "TAF directory = ", szWork, 2 );
            if ( nRC != S_OKAY )
               DBH_Error( lpView, "Error setting TAF directory.", 0, 0 );
         }

         if ( nRC == S_OKAY )
         {
            nRC = dt_dblog( szWork, &lpConnection->Currtask );
            RDM_RC( "d_dblog", nRC, "Log directory = ", szWork, 2 );
            if ( nRC != S_OKAY )
               DBH_Error( lpView, "Error setting LOG directory.", 0, 0 );
         }

         if ( nRC != S_OKAY )
         {
            #if DBQuery
               nRC = qt_closetask( &lpConnection->Currtask );
               RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
            #else
               nRC = dt_closetask( &lpConnection->Currtask );
               RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
            #endif

            DropView( vDbhWork );
            if ( lEnqFile >= 0 )
               SysCloseFile( lpView, lEnqFile, 0 );

            return( -1 );
         }
      }

      // Generate a unique user id.
      CreateViewFromViewForTask( &lpTemp, vDbhWork, 0 );
      nIdx = 0;
      strncpy_s( szUniqueDatabaseID, zsizeof( szUniqueDatabaseID ), szUserId, 7 );
      szUniqueDatabaseID[ 7 ] = 0;
      nLastChar = zstrlen( szUniqueDatabaseID );
      strcat_s( szUniqueDatabaseID, "1" );
      nRC = SetCursorFirstEntityByString( lpTemp, "Connection",
                                          "UniqueDatabaseID",
                                          szUniqueDatabaseID, "" );
      while ( nRC == zCURSOR_SET )
      {
         nIdx++;
         szUniqueDatabaseID[ nLastChar ] = szAlphaNumericString[ nIdx ];
         nRC = SetCursorFirstEntityByString( lpTemp, "Connection",
                                             "UniqueDatabaseID",
                                             szUniqueDatabaseID, "" );
      }

      DropView( lpTemp );

      nRC = dt_dbuserid( szUniqueDatabaseID, &lpConnection->Currtask );
      RDM_RC( "d_dbuserid", nRC, "UserID = ", szUniqueDatabaseID, 1 );
      if ( nRC != S_OKAY )
      {
         DBH_Error( lpView, "Invalid User ID.", 0, 0 );

         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

#if MultiUser
      // Set up the lock manager.
      nRC = dt_lockcomm( LMC_GENERAL, &lpConnection->Currtask );
      RDM_RC( "d_lockcomm", nRC, "Lock comm type = LMC_GENERAL", 0, 2 );
      //nRC = dt_lockcomm( LMC_NETBIOS, &lpConnection->Currtask );
      //RDM_RC( "d_lockcomm", nRC, "Lock comm type = LMC_NETBIOS", 0, 2 );
      if ( nRC != S_OKAY )
      {
         DBH_Error( lpView, "Error setting Lock Manager.", 0, 0 );

         // Close task if comm manager ended in error.
         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
#endif

      // The database needs to be opened + an entity in the work obj created.
      // First retrieve database path from ZEIDON.INI
      szPath[ 0 ] = 0;                       // Set szPath to null string.
      SysReadZeidonIni( -1, "[RaimaDM_dir]", szDatabaseName, szPath );
      if ( szPath[ 0 ] == 0 )
      {
         DBH_Error( lpView, "Database directories not found in ZEIDON.INI",
                    "Database name = ", szDatabaseName );

         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

      strcpy_s( szFullDBN, zsizeof( szFullDBN ), szPath );
      strcat_s( szFullDBN, zsizeof( szFullDBN ), "\\" );
      strcat_s( szFullDBN, zsizeof( szFullDBN ), szDatabaseName );

      // Now open database using fully qualified database name.
#if MultiUser
      strcpy_s( szOpenType, zsizeof( szOpenType ), "s" );
#else
      strcpy_s( szOpenType, zsizeof( szOpenType ), "o" );
#endif

#if DBQuery
      nRC = q_open( szFullDBN, szOpenType );
      RDM_RC( "q_open", nRC, "   Database name = ", szFullDBN, 1 );
      TraceLineS( "Open type = ", szOpenType );
#else
      nRC = dt_open( szFullDBN, szOpenType, &lpConnection->Currtask );
      RDM_RC( "d_open", nRC, "Database name = ", szFullDBN, 1 );
      TraceLineS( "Open type = ", szOpenType );
#endif
      if ( nRC == S_UNAVAIL )
      {
         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -2 );
      }
      else
      if ( nRC != S_OKAY )
      {
//       DBH_Error( lpView, "Error opening database.",
//                  "Database name = ", szFullDBN );
         TraceLineS( "Error opening database: ", szFullDBN );

         #if DBQuery
            nRC = qt_closetask( &lpConnection->Currtask );
            RDM_RC( "qt_closetask", nRC, 0, 0, 1 );
         #else
            nRC = dt_closetask( &lpConnection->Currtask );
            RDM_RC( "dt_closetask", nRC, 0, 0, 1 );
         #endif

         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

      nRC = CreateEntity( vDbhWork, "Connection", zPOS_AFTER );
      SetAttributeFromString( vDbhWork, "Connection",
                              "LogicalUserName", szLogicalName );
      TraceLineS( "BeginTransaction adding Connection: ", szLogicalName );
      SetAttributeFromInteger( vDbhWork, "Connection", "TaskID", lProcessID );
      SetAttributeFromString( vDbhWork, "Connection", "UserName", szUserId );
      SetAttributeFromString( vDbhWork, "Connection", "DatabaseName",
                              szDatabaseName );
      SetAttributeFromBlob( vDbhWork, "Connection", "ConnectionBlob",
                            (zPVOID) &ConnRec, zsizeof( ConnectionRecord ) );
      SetAttributeFromString( vDbhWork, "Connection", "UniqueDatabaseID",
                              szUniqueDatabaseID );

      GetAddrForAttribute( (zPVOID) &lpConnection, vDbhWork,
                           "Connection", "ConnectionBlob" );
      *((zPVOID *) lpWorkPtr) = (zPVOID) lpConnection;

   }  // if ( nRC != zCURSOR_SET )...
   else
   {
      GetAddrForAttribute( (zPVOID) &lpConnection, vDbhWork,
                           "Connection", "ConnectionBlob" );
      *((zPVOID *) lpWorkPtr) = (zPVOID) lpConnection;
#if TRACE
      lpConnection->nTraceLevel = nTraceLevel;
#endif
   }

   // Connection to database is now a valid connection.
   lpConnection->vDbhWork = vDbhWork;
   lpConnection->bValidConnection = TRUE;
   lpConnection->lpOverflowDeleteChain = 0;

   // If lpQualView is not null, then copy information from lpQualView to
   // internal tables.
   if ( lpQualView )
   {
      nRC = CopyQualToPath( lpQualView, lpView, lpConnection );
      if ( nRC == -1 )
      {
         lpConnection->vDbhWork = 0;
         DropView( vDbhWork );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }
   }

#if Transaction
   // Database has been opened.  Now start a transaction.
   nRC = dt_trbegin( szDatabaseName, &lpConnection->Currtask );
   RDM_RC( "d_trbegin", nRC, 0, 0, 1 );
   if ( nRC )
   {
      DBH_Error( lpView, "Error starting Transaction.", 0, 0 );
      lpConnection->vDbhWork = 0;
      DropView( vDbhWork );
      if ( lEnqFile >= 0 )
         SysCloseFile( lpView, lEnqFile, 0 );

      return( -1 );
   }
#endif

#if MultiUser
   //
   // Now lock all records and sets.
   //
   if ( Indicators & DBH_StoreOI )
      cLockType = 'w';     // Lock records/sets for write access.
   else
      cLockType = 'r';     // Lock records/sets for read access only.

   // Allocate enough memory to lock MAX_LOCKS records and sets.
   nLockRequestCount = 0;
   hMem = SysAllocMemory( (zCOREMEM) &lpLockRequest,
                          (zLONG) zsizeof( LOCK_REQUEST ) * MAX_LOCKS,
                          0, zCOREMEM_ALLOC, 0 );
   if ( lpLockRequest == 0 )
   {
      DBH_Error( lpView, "Out of memory -- transaction aborted.", 0, 0 );
      nRC = dt_trabort( &lpConnection->Currtask );
      RDM_RC( "d_trabort", nRC, 0, 0, 1 );
      if ( lEnqFile >= 0 )
         SysCloseFile( lpView, lEnqFile, 0 );

      return( -1 );
   }

   // Add overflow record to lock request table if it exists.
   if ( lpViewOD->nOfRecID )
   {
      lpLockRequest[ nLockRequestCount ].type = cLockType;
      lpLockRequest[ nLockRequestCount ].item = lpViewOD->nOfRecID;
      lpLockName[ nLockRequestCount ] = 0;
      nLockRequestCount++;
   }

   // Go through all the entities in the object def, requesting locks for the
   // records and sets.
   lpViewEntity = lpViewOD->hFirstOD_Entity;
   while ( lpViewEntity )
   {
      int k;
      int nID;

      // If the view entity doesn't have a data record, then the entity could
      // be a work entity or a derived entity. In any case, ignore it (we
      // don't need locks for derived/work entities).
      if ( lpViewEntity->hFirstDataRecord == 0 )
      {
         lpViewEntity = lpViewEntity->hNextHier;
         continue;
      }

      if ( nLockRequestCount + 4 > MAX_LOCKS )
      {
         DBH_Error( lpView, "Trying to lock too many records/sets. "
                    "Talk to Zeidon support.", 0, 0 );
         nRC = dt_trabort( &lpConnection->Currtask );
         RDM_RC( "d_trabort", nRC, 0, 0, 1 );
         SysFreeMemory( hMem );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         return( -1 );
      }

      lpDataRecord = lpViewEntity->hFirstDataRecord;
      lpRelRecord  = lpDataRecord->hFirstRelRecord;
      nID          = (int) lpDataRecord->lRecordID;

      // Make sure record ID is not already in the lock request table.
      for ( k = 0; k < nLockRequestCount; k++ )
      {
         if ( nID == lpLockRequest[ k ].item )
            break;
      }
      // If nID is not in lock request table, add it.
      if ( k == nLockRequestCount )
      {
         lpLockRequest[ nLockRequestCount ].type = cLockType;
         lpLockRequest[ nLockRequestCount ].item = nID;
         lpLockName[ nLockRequestCount ] = lpDataRecord->szRecordName;
         nLockRequestCount++;
      }

      // If there is a rel record, then sets need to be locked.
      if ( lpRelRecord )
      {
         nID = (int) lpRelRecord->lSetID1;
         if ( nID != 0 )
         {
            // Make sure set ID is not already in the lock request table.
            for ( k = 0; k < nLockRequestCount; k++ )
               if ( nID == lpLockRequest[ k ].item )
                  break;

            // If nID is not in lock request table, add it.
            if ( k == nLockRequestCount )
            {
               lpLockRequest[ nLockRequestCount ].type = cLockType;
               lpLockRequest[ nLockRequestCount ].item = nID;
               lpLockName[ nLockRequestCount ] = lpRelRecord->szSetName1;
               nLockRequestCount++;
            }
         }

         nID = (int) lpRelRecord->lSetID2;
         if ( nID != 0 )
         {
            // Make sure set ID is not already in the lock request table.
            for ( k = 0; k < nLockRequestCount; k++ )
               if ( nID == lpLockRequest[ k ].item )
                  break;

            // If nID is not in lock request table, add it.
            if ( k == nLockRequestCount )
            {
               lpLockRequest[ nLockRequestCount ].type = cLockType;
               lpLockRequest[ nLockRequestCount ].item = nID;
               lpLockName[ nLockRequestCount ] = lpRelRecord->szSetName2;
               nLockRequestCount++;
            }
         }

         // Check for intersection record.
         nID = (int) lpRelRecord->lRecordID;
         if ( nID != 0 )
         {
            // Make sure set ID is not already in the lock request table.
            for ( k = 0; k < nLockRequestCount; k++ )
               if ( nID == lpLockRequest[ k ].item )
                  break;

            // If nID is not in lock request table, add it.
            if ( k == nLockRequestCount )
            {
               lpLockRequest[ nLockRequestCount ].type = cLockType;
               lpLockRequest[ nLockRequestCount ].item = nID;
               lpLockName[ nLockRequestCount ] = lpRelRecord->szRecordName;
               nLockRequestCount++;
            }
         }
      } // if ( lpRelRecord )...

      lpViewEntity = lpViewEntity->hNextHier;
   } // while ( lpViewEntity )...

#if TRACE
   // Display lock information if trace level > 1.
   if ( lpConnection->nTraceLevel > 1 )
   {
      int  k;
      zCHAR szWork[ 100 ];

      TraceLineS( "", "" );
      TraceLineI( "Locking following records and sets: ",
                  (zLONG) nLockRequestCount );
      for ( k = 0; k < nLockRequestCount; k++ )
      {
         strcpy_s( szWork, zsizeof( szWork ), " Record/Set Name = " );
         if ( lpLockName[ k ] )
            strcat_s( szWork, zsizeof( szWork ), lpLockName[ k ] );
         else
            strcat_s( szWork, zsizeof( szWork ), "Overflow record" );
         strcat_s( szWork, zsizeof( szWork ), " ID = " );
         TraceLineI( szWork, (zLONG) lpLockRequest[ k ].item );
      }
   }
#endif

   if ( nLockRequestCount == 0 )
      DBH_Error( lpView, "Not locking any records/sets!", 0, 0 );
   else
   {
      zSHORT nRC2;

      nRC = dt_lock( nLockRequestCount, lpLockRequest, &lpConnection->Currtask,
                    CURR_DB );
      RDM_RC( "d_lock", nRC, 0, 0, 1 );
      if ( nRC != S_OKAY )
      {
         DBH_Error( lpView, "Failure to lock records/sets.", 0, 0 );
         nRC2 = dt_trabort( &lpConnection->Currtask );
         RDM_RC( "d_trabort", nRC2, 0, 0, 1 );

         SysFreeMemory( hMem );
         if ( lEnqFile >= 0 )
            SysCloseFile( lpView, lEnqFile, 0 );

         if ( nRC == S_UNAVAIL )
            return( -2 );
         else
            return( -1 );
      }
   }

   // Free allocated memory.
   SysFreeMemory( hMem );
#endif

   if ( lEnqFile >= 0 )
   {
//    TraceLineI( "(kzhdbvaa) closing Enqueue file ", lEnqFile );
      SysCloseFile( lpView, lEnqFile, 0 );
   }

   return( 0 );
}  /* BeginTransaction */

/////////////////////////////////////////////////////////////////////////////

      CommitTransaction

/////////////////////////////////////////////////////////////////////////////
zSHORT
CommitTransaction( LPCONNECTION lpConnection )
{
   zSHORT nRC;
   zSHORT k;

   if ( lpConnection == 0 )
   {
      TraceLineS( "(kzhdbvaa) lpConnection is NULL!!!!!", "" );
      return( -1 );
   }

   // Free any allocated memory.
   for ( k = 0; k < 5; k++ )
      FreeSets( &lpConnection->lpPathTable[ k ] );

#if TRACE
   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "#################  COMMIT TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
   }
#endif

   // If there are overflow records in the delete chain, call the function
   // that will delete them.  Second value is true to indicate that records
   // are to be deleted.
   if ( lpConnection->lpOverflowDeleteChain )
      DeleteOverflowRecords( lpConnection, TRUE );

#if Transaction
   // Commit the transaction.
   nRC = dt_trend( &lpConnection->Currtask );
   RDM_RC( "d_trend", nRC, 0, 0, 1 );
   if ( nRC )
      return( -1 );
#endif

// TraceLineI( "(kzhdbvaa) Dropping work view. vDbhWork = ",
//             (zLONG) lpConnection->vDbhWork );
   nRC = DropView( lpConnection->vDbhWork );
   lpConnection->vDbhWork = 0;
// TraceLineI( "(kzhdbvaa) Dropping work view. nRC = ", (zLONG) nRC );

   return( 0 );
}  /* CommitTransaction */

/////////////////////////////////////////////////////////////////////////////

      RollbackTransaction

/////////////////////////////////////////////////////////////////////////////
zSHORT RollbackTransaction ( LPCONNECTION lpConnection )
{
   zSHORT nRC;
   zSHORT k;

   if ( lpConnection == 0 )
   {
      TraceLineS( "(kzhdbvaa) lpConnection is NULL!!!!!", "" );
      return( 0 );
   }

   // Free any allocated memory.
   for ( k = 0; k < 5; k++ )
      FreeSets( &lpConnection->lpPathTable[ k ] );

#if TRACE
   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "################ ROLLBACK TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
   }
#endif

   // If there are overflow records in the delete chain, call the function
   // that will delete them.  Second value is false to indicate that records
   // are NOT to be deleted but that the delete chain is to be freed up.
   if ( lpConnection->lpOverflowDeleteChain )
      DeleteOverflowRecords( lpConnection, FALSE );

#if Transaction
   // Rollback the transaction.
   nRC = dt_trabort( &lpConnection->Currtask );
   RDM_RC( "d_trabort", nRC, 0, 0, 1 );
   if ( nRC )
      return( -1 );
#endif

// TraceLineI( "(kzhdbvaa) Dropping work view. vDbhWork = ",
//             (zLONG) lpConnection->vDbhWork );
   nRC = DropView( lpConnection->vDbhWork );
   lpConnection->vDbhWork = 0;
// TraceLineI( "(kzhdbvaa) Dropping work view. nRC = ", (zLONG) nRC );

   return( 0 );
}  /* RollbackTransaction */

//=========================================================================
//       ENTITY FUNCTIONS
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      Load

      Loads entities of type lpViewEntity from the database.

      Returns:  2 - Load worked but couldn't find anything to load.
                1 - Load worked--multiple roots found but only 1 loaded
                    because Indicators = zSINGLE.
                0 - Load worked--entity(-ies) loaded.
               -1 - Call error

/////////////////////////////////////////////////////////////////////////////
zSHORT
Load( LPVIEWOD     lpViewOD,
      LPVIEWENTITY lpViewEntity,
      zVIEW        lpView,
      zVIEW        lpQualView,
      LPCONNECTION lpConnection,
      int          Indicators )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   DB_ADDR  dbaChild;
   zPCHAR   szRecordName;
   zSHORT   bEntityIsQualified;
   zSHORT   bEntitiesLoaded;
   zSHORT   nRC, nReturnCode;

   nReturnCode = 0;
   bEntitiesLoaded = FALSE;

   // First, check to see if the entity is qualified (i.e. contained in
   // QUAL_PATH).
   bEntityIsQualified = FALSE;  // Assume entity is not qualified.
   if ( lpQualView )
   {
      LPSET lpSet;

      lpSet = lpConnection->lpPathTable[ QUAL_PATH ].lpFirstSet;
      while ( lpSet )
      {
         if ( lpSet->Child.lpViewEntity == lpViewEntity )
         {
            bEntityIsQualified = TRUE;
            break;
         }

         lpSet = lpSet->lpNextSet;
      } // while ( lpSet )...
   } // if ( lpQualView )...

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   szRecordName = lpDataRecord->szRecordName;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "================= Performing load ====================", "" );
      TraceLineS( "  Entity name: ", lpViewEntity->szName );
      TraceLineS( "  Record name: ", szRecordName );
      if ( lpRelRecord )
      {
         if ( lpRelRecord->szSetName1[ 0 ] )
            TraceLineS( "  Parent set1: ", lpRelRecord->szSetName1 );
         if ( lpRelRecord->szSetName2[ 0 ] )
            TraceLineS( "  Parent set2: ", lpRelRecord->szSetName2 );
      }

      if ( bEntityIsQualified )
         TraceLineS( "  Entity IS qualified.", "" );
      else
         TraceLineS( "  Entity is not qualified.", "" );

      if ( lpConnection->nTraceLevel > 2 && lpViewEntity->hParent )
         DisplayEntityInstance( lpView, lpViewEntity->hParent->szName );
   }
#endif

   // Locate parent entity.
   nRC = NavigateOI_ParentPath ( DBH_Load, lpConnection );
   if ( nRC )
      return( -1 );

   // If lpViewEntity has a parent, then retrieve entities using parent set.
   if ( lpViewEntity->hParent )
   {
      // If entity is qualified, use the values copied from lpQualView to
      // retrive the entity(-ies).
      if ( bEntityIsQualified )
      {
         // =====
         // =====  Retrieve child entities using lpQualView
         // =====
      }  // if ( bEntityIsQualified )...
      else
      {
         // =====
         // =====  Retrieve all child entities (there is no lpQualView).
         // =====

         if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
         {
            // First check to see if the current record is the member of the
            // parent set.  If it is not, then there are no children to load.
            nRC = dt_ismember( (int) lpRelRecord->lSetID1,
                              &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_ismember", nRC, "Set name = ",
                    lpRelRecord->szSetName1, 2 );
            if ( nRC == S_EOS )
               return( 2 );
            else
            if ( nRC != S_OKAY )
               return( -1 );

            // If the parent is a member of the parent set, then retrieve the
            // owner record.
            if ( nRC == S_OKAY )
            {
               // Set current member of parent set from the current owner.
               nRC = dt_setmr( (int) lpRelRecord->lSetID1,
                              &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_setmr", nRC, 0, 0, 2 );
               if ( nRC != S_OKAY )
                  return( -1 );

               // Set current record from owner of parent set.
               nRC = dt_setro( (int) lpRelRecord->lSetID1,
                              &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_setro", nRC, 0, 0, 2 );
               if ( nRC != S_OKAY )
                  return( -1 );

               // Load record and create an entity instance.
               nRC = CreateEntityInsFromCurrentRec( lpView, lpViewEntity,
                                                    lpConnection );
               if ( nRC < 0 )
                  return( -1 );

               bEntitiesLoaded = TRUE;
            } // if ( nRC == S_OKAY )...
         }  // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )...
         else
         if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER ||
              lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            // Set current record as owner of parent set.
            nRC = dt_setor( (int) lpRelRecord->lSetID1,
                           &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
            if ( nRC != S_OKAY )
               return( -1 );

            // Find first member of the set.
            nRC = dt_findfm( (int) lpRelRecord->lSetID1,
                            &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_findfm", nRC, 0, 0, 2 );
            if ( nRC == S_EOS )
               return( 2 );
            else
            if ( nRC != S_OKAY )
               return( -1 );

            // Create an entity instance for each member of the set.
            while ( nRC == S_OKAY )
            {
               // If relationship is many-to-many, then the intersection
               // record must be found and then the owner of the intersection
               // record must be found.
               if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
               {
                  // If the lpViewEntity has an autoseq attribute, read the
                  // intersection record and save its data.
                  if ( lpViewEntity->bAutoSeq )
                  {
                     nRC = dt_recread( lpConnection->IntersectBuffer,
                                      &lpConnection->Currtask, CURR_DB );
                     RDM_RC( "d_recread", nRC, "Read intersection record for"
                             " autoseq info", 0, 2 );
                     if ( nRC != S_OKAY )
                        return( -1 );
                  }

                  // Set intersection record as the member of set 2.
                  nRC = dt_setmr( (int) lpRelRecord->lSetID2,
                                 &lpConnection->Currtask, CURR_DB );
                  RDM_RC( "d_setmr", nRC, "Set name = ",
                          lpRelRecord->szSetName2, 2 );
                  if ( nRC != S_OKAY )
                     return( -1 );

                  // Set the owner of set 2 as the current record.
                  nRC = dt_setro( (int) lpRelRecord->lSetID2,
                                 &lpConnection->Currtask, CURR_DB );
                  RDM_RC( "d_setro", nRC, 0, 0, 2 );
               } // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...

               nRC = CreateEntityInsFromCurrentRec( lpView, lpViewEntity,
                                                    lpConnection );
               if ( nRC == -1 )
                  return( -1 );
               bEntitiesLoaded = TRUE;

               // Find next member.
               nRC = dt_findnm( (int) lpRelRecord->lSetID1,
                               &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_findnm", nRC, 0, 0, 2 );
               if ( nRC != S_OKAY && nRC != S_EOS )
                  return( -1 );
            } // while ( nRC == S_OKAY )...
         } // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER && ... )...

         // Save current record dba as child dba.
         nRC = dt_crget( &dbaChild, &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crget", nRC, "Saving dba of child. Child record = ",
                  szRecordName, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbaChild, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );
      }  // if ( bEntityIsQualfied )...else...

   }  // if ( lpViewEntity->hParent )...
   else
   {
      // If the entity is qualified, then use the values copied from
      // lpQualView to retrive the root entity(-ies).
      if ( bEntityIsQualified )
      {
         // =====
         // =====  Retrieve root entity using lpQualView
         // =====

         // Try to locate first record by key.  If return code is 1 (no record
         // found) or -1 (error), then return.
         nRC = LocateFirstRecord(
                   &lpConnection->lpPathTable[ QUAL_PATH ].lpFirstSet->Child,
                   zLOC_NO_USE_PARENT | zLOC_NO_CONNECT, lpConnection );
         if ( nRC < 0 )
            return( nRC );
         else
         if ( nRC == 1 )
            return( 2 );   // Indicates no entity was found.

         while ( nRC == 0 )
         {
            // Save current record dba as child dba.
            nRC = dt_crget( &dbaChild, &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_crget", nRC, "Saving dba of child. Child record = ",
                     szRecordName, 3 );
#if TRACE
            if ( lpConnection->nTraceLevel > 2 )
            {
               zCHAR szWork[ 10 ];
               int  rectype;

               DBA_ToStr( &dbaChild, szWork );
               TraceLineS( "current dba = ", szWork );

               dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
               TraceLineI( "Current record type = ", (zLONG) rectype );
            }
#endif
            if ( nRC != S_OKAY )
               return( -1 );

            nRC = CreateEntityInsFromCurrentRec( lpView, lpViewEntity,
                                                 lpConnection );
            if ( nRC == -1 )
               return( -1 );
            bEntitiesLoaded = TRUE;

            nRC = LocateNextRecord(
                   &lpConnection->lpPathTable[ QUAL_PATH ].lpFirstSet->Child,
                   lpConnection );
            if ( nRC < 0 )
               return( -1 );

            // If a second root entity was found but only 1 root is allowed,
            // set return code as 1 and break.
            if ( nRC == 0 && Indicators == zSINGLE )
            {
               nReturnCode = 1;
               break;
            }
         }
      }  // if ( bEntityIsQualified )...
      else
      {
         // =====
         // =====  Retrieve all root entities (there is no lpQualView).
         // =====

         // Get the first record occurence.
         nRC = dt_recfrst( (int) lpDataRecord->lRecordID,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_recfrst", nRC, "Get first record. Record name = ",
                    lpDataRecord->szRecordName, 1 );
         if ( nRC == S_NOTFOUND )
            return( 2 );
         else
         if ( nRC != S_OKAY )
            return( -1 );

         // Loop for each record occurence, creating an entity for each record
         // found.
         while ( nRC == S_OKAY )
         {
            nRC = CreateEntityInsFromCurrentRec( lpView, lpViewEntity,
                                                 lpConnection );
            if ( nRC == -1 )
               return( -1 );
            bEntitiesLoaded = TRUE;

            nRC = dt_recnext( &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_recnext", nRC, "Get next record.", 0, 1 );

            // If multiple root entities were found but only 1 root entity is
            // allowed, set return code to indicate multiple roots were found.
            if ( nRC == S_OKAY && Indicators == zSINGLE )
            {
               nReturnCode = 1;
               break;
            }
         }  // while ( nRC == S_OKAY )...

         // Save current record dba as child dba.
         nRC = dt_crget( &dbaChild, &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crget", nRC, "Saving dba of child. Child record = ",
                  szRecordName, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbaChild, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );
      }  // if ( bEntityIsQualfied )...else...
   }  // if ( lpViewEntity->hParent )...else...

   return( nReturnCode );
}  /* Load */

/////////////////////////////////////////////////////////////////////////////

      Insert

      RETURNS: 0 - Everything OK
               5 - Insert of Duplicate entity attempted, but failed.
              -1 - Error

/////////////////////////////////////////////////////////////////////////////
zSHORT
Insert( LPVIEWOD     lpViewOD,
        LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        LPCONNECTION lpConnection )
{
   DB_ADDR      IntersectionRecordDBA, dbaParent;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   zPCHAR szRecordName;
   zLONG  nRecordId;
   int    nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   nRecordId    = lpDataRecord->lRecordID;
   szRecordName = lpDataRecord->szRecordName;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Insert ==========", "" );
      TraceLineS( "Entity name: ", lpViewEntity->szName );
      TraceLineS( "Record name: ", szRecordName );
      if ( lpRelRecord )
      {
         if ( lpRelRecord->szSetName1[ 0 ] )
            TraceLineS( "  Parent set1: ", lpRelRecord->szSetName1 );
         if ( lpRelRecord->szSetName2[ 0 ] )
            TraceLineS( "  Parent set2: ", lpRelRecord->szSetName2 );
      }

      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->szName );
   }
#endif

   nRC = NavigateOI_ParentPath ( DBH_Insert, lpConnection );
   if ( nRC )
      return( -1 );

   // If there is a parent entity to the entity inserted, then the parent
   // record needs to be connected to the parent set.  Find the parent
   // record and get it ready to be connected.
   if ( lpViewEntity->hParent )
   {
      // The current db-vista record should be the parent of the set.  Make
      // it owner/member of parent set.
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
      {
#if TRACE
         if ( lpConnection->nTraceLevel > 1 )
         {
            TraceLineS( "", "" );
            TraceLineS( "Parent is owner", "" );
         }
#endif

         // The parent is the owner of the relationship, so set the current
         // record as the owner of the set.
         nRC = dt_setor( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }
      else
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
#if TRACE
         if ( lpConnection->nTraceLevel > 1 )
         {
            TraceLineS( "", "" );
            TraceLineS( "Parent is many-to-many", "" );
         }
#endif

         // Relationship is many-to-many, so an intersection record needs to
         // be created.  Set-up owner of first set.
         nRC = dt_setor( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         // If the entity has autoseq, then the attribute must be saved in the
         // intersection record.
         if ( lpViewEntity->bAutoSeq )
         {
            zCHAR       chType;
            zUSHORT     uLth;
            zPVOID      lpPtr;
            LPDATAFIELD lpDataField;

           //zmemset( lpConnection->IntersectBuffer, 0, INTERSECTIONBUFFERLTH );

            // Find the autoseq attribute.
            lpDataField = lpDataRecord->hFirstDataField;
            while ( lpDataField->hViewAttrib->bAutoSeq == FALSE )
               lpDataField = lpDataField->hNextDataField;

            GetValueFromRecord( lpView, lpViewEntity,
                                lpDataField->hViewAttrib,
                                (zCOREMEM) &lpPtr, &chType, &uLth );
            zmemcpy( &lpConnection->IntersectBuffer[ lpDataField->uFldOffset ],
                     lpPtr, uLth );

            nRC = dt_fillnew( (int) lpRelRecord->lRecordID,
                             lpConnection->IntersectBuffer,
                             &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_fillnew", nRC, "Intsersection record name = ",
                    lpRelRecord->szRecordName, 1 );
            if ( nRC != S_OKAY )
               return( -1 );
         }
         else
         {
            // Create intersection record.
            nRC = dt_makenew( (int) lpRelRecord->lRecordID,
                             &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_makenew", nRC, "Rec = ", lpRelRecord->szRecordName, 1 );
            if ( nRC != S_OKAY )
               return( -1 );
         }

         nRC = dt_connect( (int) lpRelRecord->lSetID1,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Save the dba of the intersection record for later use.
         nRC = dt_crget( &IntersectionRecordDBA,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crget", nRC, "Saving dba of intersection record = ",
                  szRecordName, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &IntersectionRecordDBA, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
      }  // if ( lpReclRecord->cOwnerMember = zDBH_MANY_TO_MANY )...
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
      {
#if TRACE
         if ( lpConnection->nTraceLevel > 1 )
         {
            TraceLineS( "", "" );
            TraceLineS( "Parent is member", "" );
         }
#endif

         // Save current record dba as parent dba for later use.
         nRC = dt_crget( &dbaParent, &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crget", nRC, "Saving dba of parent.  Parent record = ",
                 lpViewEntity->hParent->hFirstDataRecord->szRecordName, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbaParent, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );
      }
   }  // if ( lpViewEntity->hParent )...
#if TRACE
   else
   if ( lpConnection->nTraceLevel > 2 )
   {
      TraceLineS( "", "" );
      TraceLineS( "There is no parent", "" );
   }

   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "", "" );
      TraceLineS( "=== Creating Record ", szRecordName );
   }
#endif

   // Initialize RecordBuffer with all zeros.
   //zmemset( RecordBuffer, 0, RECORDBUFFERLTH );

   // Copy attribute values from entity to RecordBuffer and then create new
   // record with new data.
   nRC = CopyEntityToRecordBuffer( lpView, lpViewEntity, FALSE, lpConnection );
   if ( nRC < 0 )
      return( -1 );

   nRC = dt_fillnew( (int) nRecordId, lpConnection->RecordBuffer,
                    &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_fillnew", nRC, "Record name = ", szRecordName, 1 );
   if ( nRC == S_DUPLICATE )
   {
      // Duplicate record is only a problem if the entity inserted is NOT
      // the root of the instance.
      if ( lpViewEntity->hParent )
      {
         DBH_Error( lpView, "Attempt to add a duplicate record.  See trace "
                    "file for more.", "Record name = ", szRecordName );
         TraceLineS( "", "" );
         DisplayEntityInstance( lpView, lpViewEntity->szName );
      }

      return( 5 );
   }
   if ( nRC != S_OKAY )
      return( -1 );


   // If entity has a parent, then the entity needs to be connected to the
   // parent set.  Find this set and connect.
   if ( lpRelRecord )
   {
      // The current db-vista record should be the parent of the set.  Make
      // it owner/member of parent set.
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
      {
         // Make record just created owner of parent set.
         nRC = dt_setor( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Set current record from dbaParent.
         nRC = dt_crset( &dbaParent, &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crset", nRC, "Setting current rec from parent dba", 0, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbaParent, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );

         // Connect the newly created record to it's parent set.
         nRC = dt_connect( (int) lpRelRecord->lSetID1,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Reset the current record from the child (owner) of parent set.
         nRC = dt_setro( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setro", nRC, "Resetting curr rec from child of set", 0, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }  // if ( lpReclRecord->cOwnerMember = zDBH_PARENT_IS_MEMBER )...
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
      {
         // Connect the newly created record to it's parent set.
         nRC = dt_connect( (int) lpRelRecord->lSetID1,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
         if ( nRC != S_OKAY )
            return( -1 );
      }  // if ( lpReclRecord->cOwnerMember = zDBH_PARENT_IS_OWNER )...
      else
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         // Make the newly created record the owner of the second set.
         nRC = dt_setor( (int) lpRelRecord->lSetID2,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Make the intersection record the current record.
         nRC = dt_crset( &IntersectionRecordDBA,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crset", nRC, "Setting intersection rec as CR", 0, 3 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &IntersectionRecordDBA, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );

         // Connect the newly created record with the intersection record.
         nRC = dt_connect( (int) lpRelRecord->lSetID2,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName2, 1 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Reset the current record as the record just created.
         nRC = dt_setro( (int) lpRelRecord->lSetID2,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }  // if ( lpReclRecord->cOwnerMember = zDBH_MANY_TO_MANY )...
   }  // if ( lpRelRecord )...

   return( 0 );
}  /* Insert */

/////////////////////////////////////////////////////////////////////////////

      InsertRel

/////////////////////////////////////////////////////////////////////////////
zSHORT
InsertRel( LPVIEWOD     lpViewOD,
           LPVIEWENTITY lpViewEntity,
           zVIEW        lpView,
           LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   DB_ADDR      dbaIntersect, dbaParent;
   zPCHAR       szRecordName;
   zSHORT       nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   szRecordName = lpDataRecord->szRecordName;

   // There had better be a rel record...
   if ( lpRelRecord == 0 )
   {
      DBH_Error( lpView, "#3 Internal error--no rel record.", 0, 0 );
      return( -1 );
   }

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Insert Rel ==========", "" );
      TraceLineS( "Entity name: ", lpViewEntity->szName );
      TraceLineS( "Record name: ", szRecordName );
      if ( lpRelRecord )
      {
         if ( lpRelRecord->szSetName1[ 0 ] )
            TraceLineS( "  Parent set1: ", lpRelRecord->szSetName1 );
         if ( lpRelRecord->szSetName2[ 0 ] )
            TraceLineS( "  Parent set2: ", lpRelRecord->szSetName2 );
      }

      if ( lpConnection->nTraceLevel > 2 )
      {
         TraceLineS( "PARENT:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->hParent->szName );

         TraceLineS( "CHILD:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->szName );
      }
   }
#endif

   nRC = NavigateOI_ParentPath ( DBH_InsertRel, lpConnection );
   if ( nRC )
      return( -1 );

   // If parent is owner or many-to-many, set the current record as the
   // owner of the parent set.
   if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER ||
        lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      nRC = dt_setor( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // If relationship is many to many, then create intersection record.
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         nRC = dt_makenew( (int) lpRelRecord->lRecordID,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_makenew", nRC, "Record name = ",
                    lpRelRecord->szRecordName, 1 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Connect the intersection record with it's owner.
         nRC = dt_connect( (int) lpRelRecord->lSetID1,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Save the dba of the intersect record for later use.
         nRC = dt_crget( &dbaIntersect, &lpConnection->Currtask, CURR_DB );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbaIntersect, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
         if ( nRC != S_OKAY )
            return( -1 );
      }
   } // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER ||...
   else
   {
      // Save current record dba as parent dba for later use.
      nRC = dt_crget( &dbaParent, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crget", nRC, "Saving dba of Parent.  Parent record = ",
               lpViewEntity->hParent->hFirstDataRecord->szRecordName, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dbaParent, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif
      if ( nRC != S_OKAY )
         return( -1 );
   }  // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER ||...else...


   // Locate record to be included.
   nRC = LocateFirstRecord(
                     &lpConnection->lpPathTable[ INS_REL ].lpFirstSet->Child,
                     zLOC_NO_USE_PARENT | zLOC_NO_CONNECT, lpConnection );
   if ( nRC == 1 )
   {
      DBH_Error( lpView, "Included record not found.  See Trace for more.",
                 0, 0 );
      TraceLineS( "The following entity could not be found on the database:",
                  "" );
      DisplayEntityInstance( lpView, lpViewEntity->szName );
      return( -1 );
   }
   else
   if ( nRC < 0 )
      return( nRC );


   // Connect the record with it's parent.
   if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
   {
      nRC = dt_connect( (int) lpRelRecord->lSetID1,
                       &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
      if ( nRC != S_OKAY )
         return( -1 );
   }
   else
   if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
   {
      // Set newly found record as owner of parent set.
      nRC = dt_setor( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Set parent record as current record.
      nRC = dt_crset( &dbaParent, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crset", nRC, 0, 0, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dbaParent, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
      if ( nRC != S_OKAY )
#endif
         return( -1 );

      // Connect parent record with owner of parent set.
      nRC = dt_connect( (int) lpRelRecord->lSetID1,
                       &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName1, 1 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Set owner of parent set as current record.
      nRC = dt_setro( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );
   }  // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )...
   else
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      // Set newly found record as owner of 2nd parent set.
      nRC = dt_setor( (int) lpRelRecord->lSetID2,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Set intersection record as current record.
      nRC = dt_crset( &dbaIntersect, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crset", nRC, 0, 0, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dbaIntersect, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif
      if ( nRC != S_OKAY )
         return( -1 );

      // Connect intersection set with owner of 2nd parent set.
      nRC = dt_connect( (int) lpRelRecord->lSetID2,
                       &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_connect", nRC, "Set name = ", lpRelRecord->szSetName2, 1 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Set owner of 2nd set as current record.
      nRC = dt_setro( (int) lpRelRecord->lSetID2,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
      if ( nRC != S_OKAY )
         return( -1 );
   }  // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...

   return( 0 );
}  /* InsertRel */

/////////////////////////////////////////////////////////////////////////////

      Delete

/////////////////////////////////////////////////////////////////////////////
zSHORT
Delete( LPVIEWOD     lpViewOD,
        LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   int  nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Delete ==========", "" );
      TraceLineS( "Entity name: ", lpViewEntity->szName );
      TraceLineS( "Record name: ", lpDataRecord->szRecordName );
      if ( lpRelRecord )
      {
         if ( lpRelRecord->szSetName1[ 0 ] )
            TraceLineS( "  Parent set1: ", lpRelRecord->szSetName1 );
         if ( lpRelRecord->szSetName2[ 0 ] )
            TraceLineS( "  Parent set2: ", lpRelRecord->szSetName2 );
      }

      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->szName );
   }
#endif

   // Find record to be deleted.
   nRC = NavigateOI_ParentPath( DBH_Delete, lpConnection );
   if ( nRC )
      return( -1 );

   // If the record has a parent, then disconnect the record from it's parent.
   if ( lpRelRecord )  // There must be a RelRecord if there is a parent.
   {
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         nRC = dt_discon( (int) lpRelRecord->lSetID2,
                         &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_discon", nRC, "Disconnecting record ",
                  lpDataRecord->szRecordName, 1 );
#if TRACE
         if ( lpConnection->nTraceLevel > 0 )
            TraceLineS( "Set name = ", lpRelRecord->szSetName2 );
#endif
         if ( nRC != S_OKAY && nRC != S_EOS )
            return( -1 );

         // The dt_discon call changed the current record. Reset it.
         nRC = dt_setro( (int) lpRelRecord->lSetID2,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
      {
         nRC = dt_discon( (int) lpRelRecord->lSetID1,
                         &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_discon", nRC, "Disconnecting record ",
                  lpDataRecord->szRecordName, 1 );
#if TRACE
         if ( lpConnection->nTraceLevel > 0 )
            TraceLineS( "Set name = ", lpRelRecord->szSetName1 );
#endif
         if ( nRC != S_OKAY && nRC != S_EOS )
            return( -1 );
      }
      else
      if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
      {
         nRC = dt_discon( (int) lpRelRecord->lSetID1,
                         &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_discon", nRC, "Disconnecting record ",
                  lpDataRecord->szRecordName, 1 );
#if TRACE
         if ( lpConnection->nTraceLevel > 0 )
            TraceLineS( "Set name = ", lpRelRecord->szSetName1 );
#endif
         if ( nRC != S_OKAY && nRC != S_EOS )
            return( -1 );

         // The member (i.e. parent) of the set just disconnected has been made
         // the current record.  Reset this back to the owner (i.e. child ) of
         // the parent set.
         nRC = dt_setro( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setro", nRC, "Resetting current record from child of "
                    " the parent set.", 0, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }

   }  // if ( lpRelRecord )...

#if TRACE
   if ( lpConnection->nTraceLevel > 2 )
   {
      int rectype;

      dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
      TraceLineI ( "Current record type = ", (zLONG) rectype );
   }
#endif

   // Read what's already out there.
   nRC = dt_recread( lpConnection->RecordBuffer,
                    &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_recread", nRC, "Read record for delete: ",
           lpViewEntity->szName, 2 );
   if ( nRC != S_OKAY )
      return( -1 );

   // Delete any overflow records.
   nRC = DeleteOverflowRecsForCurrentRec( lpViewEntity, lpConnection );

#if TRACE
   if ( lpConnection->nTraceLevel > 2 )
   {
      zCHAR   szWork[ 10 ];
      int     rectype;
      DB_ADDR dba;

      nRC = dt_crget( &dba, &lpConnection->Currtask, CURR_DB );
      DBA_ToStr( &dba, szWork );
      RDM_RC( "d_crget", nRC, "DBA of record about to be deleted.", szWork, 3 );

      dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
      TraceLineI( "Record type = ", (zLONG) rectype );
   }
#endif

   // Delete the record.
   nRC = dt_delete( &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_delete", nRC, "Deleting record ", lpDataRecord->szRecordName, 1 );
   if ( nRC == S_HASMEM || nRC == S_ISMEM )
   {
// For the moment, ignore this as an error. -- 10/03/93
#if 0
      DBH_Error( lpView,
                 "An attempt was made to delete an entity from the database "
                 "but the entity still has relationships with other entities."
                 "  Commit aborted.", "Entity name = ", lpViewEntity->szName );
      return( -1 );
#else
      int  k;
      zCHAR cLockType[ 5 ];
      zCHAR sz[ 100 ];

//    OperatorSend( 0, "dgc", "Locking all sets for delete", 0 );

      // Lock ALL sets.
      for ( k = 20000; k < 20234 ; k++ )
      {
         strcpy_s( sz, zsizeof( sz ), "Set ID = " );
         zltoa( (zLONG) i, cLockType );
         strcat_s( sz, zsizeof( sz ), cLockType );
         strcat_s( sz, zsizeof( sz ), ". Lock type = " );
         nRC = dt_setlstat( i, cLockType, &lpConnection->Currtask, CURR_DB );
         strcat_s( sz, zsizeof( sz ), cLockType );
         RDM_RC( "d_setlstat", nRC, sz, 0, 1 );
         if ( *cLockType == 'f' )
         {
            nRC = dt_setlock( i, "w", &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setlock", nRC, 0, 0, 1 );
         }
      }

      // Now delete record regardless of connections.
      nRC = dt_disdel( &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_disdel", nRC, "Deleting record ", lpDataRecord->szRecordName, 1 );
#endif
   }
   else
   if ( nRC != S_OKAY )
      return( -1 );

   // If the relationship between the entity and it's parent is many-to-many,
   // then the intersection record needs to be deleted.
   if ( lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      // Set the member of the 1st set as the current record.
      nRC = dt_setrm( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setrm", nRC, "Set name ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Delete the intersection record (the current record).
      nRC = dt_disdel( &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_disdel", nRC, "Deleting intersection record ",
                 lpRelRecord->szRecordName, 1 );
      if ( nRC != S_OKAY )
         return( -1 );
   }

   return( 0 );
}  /* Delete */

/////////////////////////////////////////////////////////////////////////////

      DeleteRel

/////////////////////////////////////////////////////////////////////////////
zSHORT
DeleteRel( LPVIEWOD     lpViewOD,
           LPVIEWENTITY lpViewEntity,
           zVIEW        lpView,
           LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   int  nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Delete Rel ==========", "" );
      TraceLineS( "Entity name: ", lpViewEntity->szName );
      TraceLineS( "Record name: ", lpDataRecord->szRecordName );
      if ( lpRelRecord )
      {
         if ( lpRelRecord->szSetName1[ 0 ] )
            TraceLineS( "  Parent set1: ", lpRelRecord->szSetName1 );
         if ( lpRelRecord->szSetName2[ 0 ] )
            TraceLineS( "  Parent set2: ", lpRelRecord->szSetName2 );
      }

      if ( lpConnection->nTraceLevel > 2 )
      {
         TraceLineS( "Parent:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->hParent->szName );
         TraceLineS( "", "" );
         TraceLineS( "Child:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->szName );
         TracePathTable( OI_PATH, lpConnection );
      }
   }
#endif

   // Find record to be deleted.
   nRC = NavigateOI_ParentPath( DBH_DeleteRel, lpConnection );
   if ( nRC < 0 )
      return( -1 );
   else
   if ( nRC > 0 )
   {
      zCHAR szMsg[ 500 ];

      // If the relationship could not be established, then there is no
      // relationship to delete.  Show a message and possably continue.
      DisplayEntityInstance( lpView, lpViewEntity->hParent->szName );
      DisplayEntityInstance( lpView, lpViewEntity->szName );

      strcpy_s( szMsg, zsizeof( szMsg ), "Attempting to exclude an entity that has already "
               "been excluded." );
      strcat_s( szMsg, zsizeof( szMsg ), "\n\n Parent entity name = " );
      strcat_s( szMsg, zsizeof( szMsg ), lpViewEntity->hParent->szName );
      strcat_s( szMsg, zsizeof( szMsg ), "\n Child entity name = " );
      strcat_s( szMsg, zsizeof( szMsg ), lpViewEntity->szName );
      strcat_s( szMsg, zsizeof( szMsg ), ", Record name = " );
      strcat_s( szMsg, zsizeof( szMsg ), lpDataRecord->szRecordName );
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         strcat_s( szMsg, zsizeof( szMsg ), "\n Set name1 = " );
         strcat_s( szMsg, zsizeof( szMsg ), lpRelRecord->szSetName1 );
         strcat_s( szMsg, zsizeof( szMsg ), "\n Intersection record = " );
         strcat_s( szMsg, zsizeof( szMsg ), lpRelRecord->szRecordName );
         strcat_s( szMsg, zsizeof( szMsg ), "\n Set name2 = " );
         strcat_s( szMsg, zsizeof( szMsg ), lpRelRecord->szSetName2 );
      }
      else
      {
         strcat_s( szMsg, zsizeof( szMsg ), "\n Set name = " );
         strcat_s( szMsg, zsizeof( szMsg ), lpRelRecord->szSetName1 );
      }

      strcat_s( szMsg, zsizeof( szMsg ), "\nSee Trace for attribute values." );
      strcat_s( szMsg, zsizeof( szMsg ), "\n\nDo you want to ignore this error and continue "
               "with the Commit?" );
      nRC = zRESPONSE_YES;
      TraceLineS( "(kzhdbvaa) ", szMsg );
//    nRC = OperatorPrompt( lpView, "Zeidon RaimaDM Handler", szMsg,
//                          1, zBUTTONS_YESNO, 0, zICON_EXCLAMATION );
      if ( nRC == zRESPONSE_YES )
         return( 0 );
      else
         return( -1 );
   }

   // If the relationship between the entity and it's parent is many-to-many,
   // then the intersection record needs to be deleted.
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      // Set the member of the 1st set as the current record.
      nRC = dt_setrm( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setrm", nRC, "Set name ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Delete the intersection record (the current record).
      nRC = dt_disdel( &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_disdel", nRC, "Deleting intersection record ",
                 lpRelRecord->szRecordName, 1 );
   }
   else
   {
      // For all other (non many-to-many) relationships, disconnect the record.
      nRC = dt_discon( (int) lpRelRecord->lSetID1,
                      &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_discon", nRC, "Set name ", lpRelRecord->szSetName1, 2 );

      // If we just disconnected the last record of the set, that's okay.
      if ( nRC == S_EOS )
         nRC = S_OKAY;
   }

   if ( nRC != S_OKAY )
      return( -1 );
   else
      return( 0 );
}  /* DeleteRel */

/////////////////////////////////////////////////////////////////////////////

      Update

/////////////////////////////////////////////////////////////////////////////
zSHORT
Update( LPVIEWOD     lpViewOD,
        LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        zSHORT       bAutoSeqOnly,
        LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   int  nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Update ==========", "" );
      TraceLineS( "Entity name: ", lpViewEntity->szName );
      TraceLineS( "Record name: ", lpDataRecord->szRecordName );

      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->szName );
   }
#endif

   // Find record to be updated.
   nRC = NavigateOI_ParentPath ( DBH_Update, lpConnection );
   if ( nRC < 0 )
      return( nRC );

   // If the entity has an autoseq attribute and the relationship is
   // many-to-many, then the autoseq attribute needs to be updated in the
   // intersection record.
   if ( lpViewEntity->bAutoSeq && lpDataRecord->hFirstRelRecord )
   {
      LPRELRECORD lpRelRecord;

      lpRelRecord = lpDataRecord->hFirstRelRecord;
      if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         zCHAR       chType;
         zUSHORT     uLth;
         zPVOID      lpPtr;
         LPDATAFIELD lpDataField;

         lpDataField = lpDataRecord->hFirstDataField;
         while ( lpDataField->hViewAttrib->bAutoSeq == FALSE )
            lpDataField = lpDataField->hNextDataField;

         // Set intersection record as current record.
         nRC = dt_setrm( (int) lpRelRecord->lSetID2,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setrm", nRC, "Set name ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         nRC = dt_recread( lpConnection->IntersectBuffer,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_recread", nRC, "Read intersection record for"
                  " autoseq info", 0, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         GetValueFromRecord( lpView, lpViewEntity, lpDataField->hViewAttrib,
                             (zCOREMEM) &lpPtr, &chType, &uLth );
         zmemcpy( &lpConnection->IntersectBuffer[ lpDataField->uFldOffset ],
                  lpPtr, uLth );

         // Rewrite intersection buffer.
         nRC = dt_recwrite( lpConnection->IntersectBuffer,
                           &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_recwrite", nRC, "Updating Intersection record ",
                 lpRelRecord->szRecordName, 1 );

         // Reset current record back to child of intersection record.
         nRC = dt_setro( (int) lpRelRecord->lSetID2,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setro", nRC, 0, 0, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      } // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...
   } // if ( lpViewEntity->bAutoSeq && lpDataRecord->hFirstRelRecord )...

   // Read what's already out there.
   nRC = dt_recread( lpConnection->RecordBuffer,
                    &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_recread", nRC, "Read record for update:",
           lpViewEntity->szName, 2 );
   if ( nRC != S_OKAY )
      return( -1 );

   // Delete any overflow records.
   nRC = DeleteOverflowRecsForCurrentRec( lpViewEntity, lpConnection );
   if ( nRC )
      return( -1 );

   // Copy attribute values to record buffer.  This will also create any
   // overflow records necessary.
   nRC = CopyEntityToRecordBuffer( lpView, lpViewEntity, bAutoSeqOnly,
                                   lpConnection );
   if ( nRC < 0 )
      return( -1 );

   // Rewrite record buffer.
   nRC = dt_recwrite( lpConnection->RecordBuffer,
                     &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_recwrite", nRC, "Updating entity ", lpViewEntity->szName, 1 );

   if ( nRC == S_OKAY )
      return( 0 );
   else
      return( -1 );
}  /* Update */


//=========================================================================
//       UTILITY FUNCTIONS -- in alphabetical order
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      AddEntityToParentPath

      Saves the key values of the current entity of type lpViewEntity in
      lpPathTable for later reference.

      Returns:  1 - Entity added to path ok AND entity has 1 unique key.
                0 - Entity added to path with multiple keys.
               -1 - Error in call.

/////////////////////////////////////////////////////////////////////////////
zSHORT
AddEntityToParentPath( zVIEW        lpView,
                       zSHORT       nPathType,
                       LPVIEWENTITY lpViewEntity,
                       LPCONNECTION lpConnection )
{
   LPRELRECORD  lpRelRecord;
   LPDATARECORD lpDataRecord;
   LPSET        lpSet;
   zPCHAR       szSetName;
   zSHORT       nSetID;
   zSHORT       nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   if ( lpRelRecord )
   {
      nSetID    = (zSHORT) lpRelRecord->lSetID1;
      szSetName = lpRelRecord->szSetName1;
   }
   else
   {
      nSetID    = 0;
      szSetName = "";
   }

   // Find Set record in PathTable.
   lpSet = lpConnection->lpPathTable[ nPathType ].lpFirstSet;
   while ( lpSet && lpSet->nSetID != nSetID )
      lpSet = lpSet->lpNextSet;

   // If the set was found, free keys, otherwise allocate set record.
   if ( lpSet )
   {
      FreeKeys( &lpSet->Child );
   }
   else
   {
      nRC = AddSetToPathTable( nPathType, &lpSet, lpConnection );
      if ( nRC == -1 )
         return( -1 );

      lpSet->nSetID = nSetID;
      lpSet->szSetName = szSetName;
   } // if ( lpSet )...else...

   // Save child entity information & keys.
   lpSet->Child.lpViewEntity = lpViewEntity;

   nRC = AddKeysToEntity( lpView, lpViewEntity, &lpSet->Child, lpConnection );
   if ( nRC )
      return( -1 );
   else
   if ( lpSet->Child.nKeyCnt == 1 )
      nRC = 1;
   else
      nRC = 0;

   return( nRC );
}  /* AddEntityToParentPath */

/////////////////////////////////////////////////////////////////////////////

      AddKeysToEntity

      Searches through the definition of lpViewEntity looking for key
      attributes.  If any are found, then save the key values in lpEntity.

/////////////////////////////////////////////////////////////////////////////
zSHORT
AddKeysToEntity( zVIEW        lpView,
                 LPVIEWENTITY lpViewEntity,
                 LPENTITY     lpEntity,
                 LPCONNECTION lpConnection )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   zSHORT       nKeyCnt;
   zULONG       ulLth;
   zPVOID       lpDataPtr;
   zCHAR        TempAttributeBuffer[ 1000 ];
   zLONG        hMem;
   LPKEY        lpKey;
   zCHAR        chType;
   zSHORT       nRC;

   nKeyCnt = 0;
   lpEntity->lpFirstKey = 0;

   // Loop through each attribute looking for keys.
   lpDataField = lpViewEntity->hFirstDataRecord->hFirstDataField;
   while( lpDataField )
   {
      lpViewAttrib = lpDataField->hViewAttrib;
      // If attrib is a key, then Key record and save the key's value.
      if ( lpViewAttrib->bKey )
      {
         nKeyCnt++;

         // Allocate key memory and insert it into key chain.
         hMem = SysAllocMemory( (zPVOID) &lpKey, (zLONG) zsizeof( KeyRecord ),
                                0, zCOREMEM_ALLOC, 0 );
         if ( lpKey == 0 )
         {
            DBH_Error( lpConnection->lpAppView,
                       "(kzhdbvaa) Error allocating memory.", "lpKey #1", 0 );
            return( -1 );
         }
         zmemset( (zPCHAR) lpKey, 0, zsizeof( KeyRecord ) );
         lpKey->hMemory       = hMem;
         lpKey->lpViewAttrib  = lpViewAttrib;
         lpKey->lpDataField   = lpDataField;
         lpKey->lpNextKey     = lpEntity->lpFirstKey;
         lpEntity->lpFirstKey = lpKey;

         // Convert key from internal Zeidon form to physical binary form.
         nRC = GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib,
                                   (zPVOID) &lpDataPtr, &chType, &ulLth );
         if ( nRC < 0 )
            return( -1 );

         // If the physical format of the data is different from the logical
         // format, then convert to the physical format.
         if ( lpDataField->cFldType != lpViewAttrib->cType )
         {
            nRC = ConvertAttributeValue( lpViewAttrib->cType, lpDataPtr,
                                         lpDataField->cFldType,
                                         TempAttributeBuffer, &ulLth );
            if ( nRC < 0 )
               return( -1 );
            lpDataPtr = (zPVOID) TempAttributeBuffer;
         }

         // Set key value.
         hMem = SysAllocMemory( (zPVOID) &lpKey->lpKeyValue, (zLONG) ulLth, 0,
                                zCOREMEM_ALLOC, 0 );
         if ( lpKey->lpKeyValue == 0 )
         {
            DBH_Error( lpConnection->lpAppView,
                       "Error allocating memory.", "lpKeyValue #1", 0 );
            return( -1 );
         }

         zmemset( (zPCHAR) lpKey->lpKeyValue, 0, ulLth );
         lpKey->hValueMemory = hMem;
         lpKey->ulLth        = ulLth;
         zmemcpy( lpKey->lpKeyValue, lpDataPtr, ulLth );

         // ONEKEY
         // Since we are assuming that there is one and only one key per
         // entity, break out of while loop.
         break;
      }  // if ( lpViewAttrib->bKey )...

      // Point to next view attrib.
      lpDataField = lpDataField->hNextDataField;
   }  // while ( lpDataField )...

   lpEntity->nKeyCnt = nKeyCnt;

   return( 0 );
}  /* AddKeysToEntity */

/////////////////////////////////////////////////////////////////////////////

      AddSetToPathTable

      Allocates a set and inserts it as the last set into the set chain for
      the path type identified.

/////////////////////////////////////////////////////////////////////////////
zSHORT AddSetToPathTable ( zSHORT       nPathType,
                           LPSET        *plpSet,
                           LPCONNECTION lpConnection )
{
   zLONG        hMem;

   // Allocate Set.
   hMem = SysAllocMemory( (zPVOID) plpSet,
                           zsizeof( SetRecord ), 0, zCOREMEM_ALLOC, 0 );
   if ( plpSet[ 0 ] == 0 )
   {
      DBH_Error( lpConnection->lpAppView,
                 "Error allocating memory.", "lpSet", 0 );
      return( -1 );
   }
   zmemset( (zPCHAR) *plpSet, 0, zsizeof( SetRecord ) );
   (*plpSet)->hMemory = hMem;

   // Add lpSet as last set in set chain.
   (*plpSet)->lpNextSet = 0;
   lpConnection->lpPathTable[ nPathType ].nSetCount++;
   if ( lpConnection->lpPathTable[ nPathType ].lpLastSet )
   {
      (*plpSet)->lpPrevSet = lpConnection->lpPathTable[ nPathType ].lpLastSet;
      lpConnection->lpPathTable[ nPathType ].lpLastSet->lpNextSet = *plpSet;
      lpConnection->lpPathTable[ nPathType ].lpLastSet = *plpSet;
   }
   else
   {
      (*plpSet)->lpPrevSet = 0;
      lpConnection->lpPathTable[ nPathType ].lpFirstSet = *plpSet;
      lpConnection->lpPathTable[ nPathType ].lpLastSet  = *plpSet;
   }
}  /* AddSetToPathTable */

#if DBQuery

/////////////////////////////////////////////////////////////////////////////

      BuildWhereClause

/////////////////////////////////////////////////////////////////////////////
void BuildWhereClause ( zVIEW  lpView,
                        zPCHAR szWhereClause )
{
   int      nRC;
   zULONG   nLth;
   zCHAR    szWork1[ 60 ];
   zCHAR    bKeyValue[ 1000 ];
   zPCHAR   szRecordName;
   zPLONG   TempLongPtr;
   zPDECIMAL TempDecimalPtr;
   LPVIEWENTITY  lpViewEntity;
   LPVIEWATTRIB  lpViewAttrib;
   LPDATARECORD  lpDataRecord;
   LPRELRECORD   lpRelRecord;
   LPDATAFIELD   lpDataField;

   nLth = zsizeof( lpViewEntity );
   nRC = GetBlobFromAttribute( &lpViewEntity, &nLth, lpView, "DB_Key",
                               "lpViewEntity" );
   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   szRecordName = lpDataRecord->szRecordName;

   strcpy_s( szWhereClause, zsizeof( szWhereClause ), " WHERE ( " );
   nRC = SetCursorFirstEntity( lpView, "DB_Key", "DB_Entity" );

   if ( lpRelRecord )
   {
      strcpy_s( szWhereClause, zsizeof( szWhereClause ), " ( db_addr( " );
      copy parent record name.
      strcpy_s( szWhereClause, zsizeof( szWhereClause ), " ) = " );
      copy parent record dba.
      strcpy_s( szWhereClause, zsizeof( szWhereClause ), " ) AND " );
   }  // if ( lpRelRecord )...

   strcpy_s( szWork1, zsizeof( szWork1 ), "( " );

   // For each DB_Key entity, add a "fld = fld" statement to the WHERE clause.
   while ( nRC == zCURSOR_SET )
   {
      strcat_s( szWhereClause, zsizeof( szWhereClause ), szWork1 );

      nLth = 999;
      GetBlobFromAttribute( bKeyValue, &nLth, lpView, "DB_Key", "KeyValue" );
      nLth = zsizeof( lpViewAttrib );
      GetBlobFromAttribute( &lpViewAttrib, &nLth, lpView, "DB_Key",
                            "lpViewAttrib" );
      nLth = zsizeof( lpDataField );
      GetBlobFromAttribute( &lpDataField, &nLth, lpView, "DB_Key",
                            "lpDataField" );

      strcat_s( szWhereClause, zsizeof( szWhereClause ), szRecordName );
      strcat_s( szWhereClause, zsizeof( szWhereClause ), "." );
      strcat_s( szWhereClause, zsizeof( szWhereClause ), lpDataField->hFldName );
      strcat_s( szWhereClause, zsizeof( szWhereClause ), " = " );

      switch ( lpViewAttrib->cType )
      {
         case zTYPE_INTEGER:
            TempLongPtr = (zPLONG) bKeyValue;
            zltoa( *TempLongPtr, szWork1 );
            strcat_s( szWhereClause, zsizeof( szWhereClause ), szWork1 );
            break;

         case zTYPE_DECIMAL:
            TempDecimalPtr = (zPDECIMAL) bKeyValue;
            break;

         case zTYPE_STRING:
            strcat_s( szWhereClause, zsizeof( szWhereClause ), "\"" );
            strcat_s( szWhereClause, zsizeof( szWhereClause ), bKeyValue );
            strcat_s( szWhereClause, zsizeof( szWhereClause ), "\"" );
            break;
      }  // switch (...)...

      strcat_s( szWhereClause, zsizeof( szWhereClause ), " )" );
      strcpy_s( szWork1, zsizeof( szWork1 ), " AND (" );

      // Set cursor to next key entity.
      nRC = SetCursorNextEntity( lpView, "DB_Key", "DB_Entity" );
   }  // while ( nRC == zCURSOR_SET )...

}  /* BuildWhereClause */

#endif  // #if DBQuery

/////////////////////////////////////////////////////////////////////////////

      ConvertAttributeValue

      Converts a value from one data type to another.  Note that it is not
      a good idea to use the same pointer for the in value and out value.

      NOTE: Will not convert if datatypes are the same!

/////////////////////////////////////////////////////////////////////////////
zSHORT
ConvertAttributeValue( zCHAR    cInType,
                       zPVOID   lpInValue,
                       zCHAR    cOutType,
                       zPVOID   lpOutValue,
                       zPULONG  pulLth )
{
   int      TempInt, *TempIntPtr;
   zLONG    TempLong;
   zPLONG   TempLongPtr;
   zDECIMAL  TempDecimal;
   float    TempFloat;
   zCHAR    szWork[ 100 ];

   switch ( cInType )
   {
      case 'C' :
      case zTYPE_STRING:
         switch ( cOutType )
         {
            case 'L' :
               *puLth = zsizeof( zPLONG );
               TempLong = zatol( lpInValue );
               zmemcpy( (zPVOID) lpOutValue, (zPVOID) &TempLong, *pulLth );
               return( 0 );
            case 'N' :
               *puLth = zsizeof( int );
               TempInt = (int) zatol( lpInValue );
               zmemcpy( lpOutValue, (zPVOID) &TempInt, *pulLth );
               return( 0 );
            case 'T' :
               *puLth = sizeof( DateTimeRecord );
               UfStringToDateTime( (zCPCHAR) lpInValue,
                                   (LPDATETIME) lpOutValue );
               return( 0 );
         }
         break;

      case zTYPE_INTEGER:
         switch ( cOutType )
         {
            case 'N' :
               *puLth = zsizeof( int );
               TempLongPtr = (zPLONG) lpInValue;
               TempInt = (int) *TempLongPtr;
               zmemcpy( lpOutValue, (zPVOID) &TempInt, *pulLth );
               return( 0 );
            case 'S' :
            case 'C' :
               TempLongPtr = (zPLONG) lpInValue;
               zltoa( *TempLongPtr, lpOutValue );
               *puLth = zstrlen( lpOutValue ) + 1;
               return( 0 );
         }
         break;

      case zTYPE_DECIMAL:
         switch ( cOutType )
         {
            case 'S' :
               ConvertDecimalToString( (zPCHAR) lpOutValue, 0,
                                       *((zPDECIMAL) lpInValue), 0 );
               *puLth = zstrlen( (zPCHAR) lpOutValue ) + 1;
               return( 0 );
            case 'F' :
               TempDecimal = *((zPDECIMAL) lpInValue);
               TempFloat   = (float) TempDecimal;
               *puLth = zsizeof( float );
               zmemcpy( lpOutValue, (zPVOID) &TempFloat, *pulLth );
               return( 0 );
         }
         break;

      case 'N':
         switch ( cOutType )
         {
            case 'L' :
               *puLth = sizeof( zLONG );
               TempIntPtr = (int *) lpInValue;
               TempLong = (zLONG) *TempIntPtr;
               zmemcpy( lpOutValue, (zPVOID) &TempLong, *pulLth );
               return( 0 );
            case 'S' :
            case 'C' :
               TempIntPtr = (int *) lpInValue;
               zltoa( (zLONG) *TempIntPtr, lpOutValue );
               *puLth = zstrlen( lpOutValue ) + 1;
               return( 0 );
         }
         break;

      case zTYPE_DATETIME:
         switch ( cOutType )
         {
            case 'S' :
               UfDateTimeToString( (LPDATETIME) lpInValue,
                                   (zPCHAR) lpOutValue, 18 );
               *puLth = zstrlen( (zPCHAR) lpOutValue ) + 1;
               return( 0 );
         }
         break;
   } // switch ( cInType )...

   strcpy_s( szWork, zsizeof( szWork ), "Internal error--converting illegal types. From-type = " );
   zstrncat( szWork, (zPCHAR) &cInType, 1 );
   strcat_s( szWork, zsizeof( szWork ), "; To-type = " );
   zstrncat( szWork, (zPCHAR) &cOutType, 1 );

   TraceLineS( "", "" );
   TraceLineS( "******* Conversion error ********", "" );
   TraceLineS( szWork, "" );

   return( -1 );
}  /* ConvertAttributeValue */

/////////////////////////////////////////////////////////////////////////////

      CopyEntityToRecordBuffer

      Copy all the attribute values from the current entity in lpView to
      the current record buffer (a global variable).

/////////////////////////////////////////////////////////////////////////////
zSHORT
CopyEntityToRecordBuffer( zVIEW        lpView,
                          LPVIEWENTITY lpViewEntity,
                          zSHORT       bAutoSeqOnly,
                          LPCONNECTION lpConnection )
{
   LPVIEWATTRIB lpViewAttrib;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPDATAFIELD  lpDataField;
   zULONG       ulLth;
   zPVOID       AttribAddr;
   zCHAR        cDummy;
   zCHAR        TempAttributeBuffer[ 1000 ];
   zSHORT       nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;
   lpDataField  = lpDataRecord->hFirstDataField;
   while ( lpDataField )
   {
      lpViewAttrib = lpDataField->hViewAttrib;

      // If the attribute is an autoseq attribute for many-to-many
      // relationships, then the attribute is stored in the corresponding
      // table and should not be copied to the record buffer.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
      {
         lpDataField = lpDataField->hNextDataField;
         continue;
      }

      // If the attribute is not an auto-sequence attribute and we are only
      // changing auto-sequencing attributes, then continue.
      if ( lpViewAttrib->bAutoSeq == FALSE && bAutoSeqOnly )
      {
         lpDataField = lpDataField->hNextDataField;
         continue;
      }

      GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib,
                          (zPVOID) &AttribAddr, &cDummy, &uLth );

      // If the physical data type is different from the logical data type,
      // then the data must be converted.
      if ( lpDataField->cFldType != lpViewAttrib->cType )
      {
         nRC = ConvertAttributeValue( lpViewAttrib->cType, AttribAddr,
                                      lpDataField->cFldType,
                                      (zPVOID) TempAttributeBuffer, &uLth );
         if ( nRC > 0 )
            return( -1 );
         AttribAddr = (zPVOID) TempAttributeBuffer;
      }

      // if attribute has a length, copy attribute value to RecordBuffer.
      if ( uLth )
      {
         // Check to see if attribute needs to be stored in the overflow
         // record.  If does if one of the following is true:
         //    o  The length of the attribute is longer than the physical
         //       field length.
         //    o  The attribute is a Blob.
         //    o  The first 4 bytes are equal to the OVERFLOW_FLAG and the
         //       max attribute length is greater than the the field length.
         //       Note that this can only happen with strings (because blobs
         //       are already accounted for) so we must consider the effect of
         //       the null terminator, which is why we use '>=' to compare the
         //       logical and the physical lengths.
         if ( (uLth > lpDataField->uFldLth) ||
              (lpViewAttrib->cType == zTYPE_BLOB) ||
              (lpViewAttrib->cType == zTYPE_STRING &&
               *((zPLONG) AttribAddr) == zOVERFLOW_FLAG &&
               lpViewAttrib->ulLth >= lpDataField->uFldLth) )
         {
            if ( lpConnection->nOVERFLOW_BUFFER_LTH )  // Gig, 3/13/93
            {
               nRC = CreateOverflowRecord( AttribAddr, uLth,
                       &lpConnection->RecordBuffer[ lpDataField->uFldOffset ],
                       lpDataField, lpConnection );
               if ( nRC < 0 )
                  return( -1 );
            }
            else
               DBH_Error( lpView, "No overflow record defined!", 0, 0 );
         }
         else
            zmemcpy( (zPVOID) &lpConnection->
                                      RecordBuffer[ lpDataField->uFldOffset ],
                     (zPVOID) AttribAddr, uLth );
      }  // if ( uLth )...
      else
      {
         // Attribute doesn't have a length, so set field value to all zeros.
         uLth = lpDataField->uFldLth;
         zmemset( &lpConnection->RecordBuffer[ lpDataField->uFldOffset ],
                  0, uLth );
      }

      // Get next data field.
      lpDataField = lpDataField->hNextDataField;
   }  // while ( lpDataField )...

   return( 0 );
}  /* CopyEntityToRecordBuffer */

/////////////////////////////////////////////////////////////////////////////

      CopyQualToPath

      Copy the information in the qualification object to lpPathTable.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CopyQualToPath( zVIEW        lpQualView,
                zVIEW        lpView,
                LPCONNECTION lpConnection )
{
   zVIEW        lpSourceView;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;
   zPVOID       lpQualValue;
   zPVOID       vDbhWorkValue;
   zCHAR        lpValueBuffer[ 1000 ];
   zPCHAR       szOper;
   zUSHORT      uLth;
   LPSET        lpSet;
   LPKEY        lpKey;
   zPCHAR       pchEntityName;
   zPCHAR       pchAttribName;
   zLONG        hMem;
   zSHORT       nRC;

   // Free up old Qual path.
   if ( lpConnection->lpPathTable[ QUAL_PATH ].lpFirstSet )
      FreeSets( &lpConnection->lpPathTable[ QUAL_PATH ] );

   // ONEKEY
   // Go through the qualification object looking for "IS" commands.  If any
   // are found, then change the "IS" command to be "=".  This code assumes
   // that there is only 1 key per entity.
   for ( nRC = SetCursorFirstEntity( lpQualView, "EntitySpec", "" );
         nRC == zCURSOR_SET;
         nRC = SetCursorNextEntity( lpQualView, "EntitySpec", "" ) )
   {
      for ( nRC = SetCursorFirstEntity( lpQualView, "QualAttrib", "" );
            nRC == zCURSOR_SET;
            nRC = SetCursorNextEntity( lpQualView, "QualAttrib", "" ) )
      {
         GetAddrForAttribute( &szOper, lpQualView, "QualAttrib", "Oper" );
         if ( zstrcmpi( szOper, "IS" ) == 0 )
         {
            // Change "IS" command to "=".
            SetAttributeFromString( lpQualView, "QualAttrib", "Oper", "=" );

            // Find the key attribute in the view entity.
            GetAddrForAttribute( &pchEntityName, lpQualView,
                                 "QualAttrib", "EntityName" );
            if ( pchEntityName == 0 || pchEntityName[ 0 ] == 0 )
            {
               TraceLineS( "The following qualification has no Entity Name:",
                           "" );
               DisplayEntityInstancePath( lpQualView, "QualAttrib" );
               DBH_Error( lpQualView, "Qualification has no Entity Name.",
                          0, 0 );
               return( -1 );
            }

            lpViewEntity = MiGetViewEntityForView( lpView, pchEntityName );
            for ( lpViewAttrib = lpViewEntity->hFirstOD_Attrib;
                  lpViewAttrib;
                  lpViewAttrib = lpViewAttrib->hNextOD_Attrib )
            {
               if ( lpViewAttrib->bKey )
               {
                  SetAttributeFromString( lpQualView, "QualAttrib",
                                          "AttributeName",
                                          lpViewAttrib->szName );
                  break;
               }
            }

            if ( lpViewAttrib == 0 )
            {
               TraceLineS( "The following Entity has no key: ",
                           lpViewEntity->szName );
               DisplayEntityInstancePath( lpQualView, "QualAttrib" );
               DBH_Error( lpQualView, "Entity has no key.", 0, 0 );
               return( -1 );
            }

            // Get Source View.
            GetIntegerFromAttribute( (zPLONG) &lpSourceView, lpQualView,
                                     "QualAttrib", "SourceViewID" );
            if ( pchEntityName == 0 || pchEntityName[ 0 ] == 0 )
            {
               TraceLineS( "The following qualification has no Source View:",
                           "" );
               DisplayEntityInstancePath( lpQualView, "QualAttrib" );
               DBH_Error( lpQualView, "Qualification has no Source View.",
                          0, 0 );
               return( -1 );
            }

            // Get Source Entity Name.
            GetAddrForAttribute( &pchEntityName, lpQualView, "QualAttrib",
                                 "SourceEntityName" );
            if ( pchEntityName == 0 || pchEntityName[ 0 ] == 0 )
            {
               TraceLineS( "The following qualification has no Source ",
                           "Entity Name:" );
               DisplayEntityInstancePath( lpQualView, "QualAttrib" );
               DBH_Error( lpQualView,
                          "Qualification has no Source Entity Name.", 0, 0 );
               return( -1 );
            }

            // Find key attribute in Source View.
            lpViewEntity = MiGetViewEntityForView( lpSourceView, pchEntityName );
            for ( lpViewAttrib = lpViewEntity->hFirstOD_Attrib;
                  lpViewAttrib;
                  lpViewAttrib = lpViewAttrib->hNextOD_Attrib )
            {
               if ( lpViewAttrib->bKey )
               {
                  SetAttributeFromString( lpQualView, "QualAttrib",
                                          "AttributeName",
                                          lpViewAttrib->szName );
                  break;
               }
            }

            if ( lpViewAttrib == 0 )
            {
               TraceLineS( "The following Source Entity has no key: ",
                           lpViewEntity->szName );
               DisplayEntityInstancePath( lpQualView, "QualAttrib" );
               DBH_Error( lpQualView, "Source Entity has no key.", 0, 0 );
               return( -1 );
            }

            // Now copy attribute value from source entity to qualification
            // Value attribute.
            SetAttributeFromAttribute( lpQualView, "QualAttrib", "Value",
                                       lpSourceView, pchEntityName,
                                       lpViewAttrib->szName );

            // Set SourceView, SourceEntity to null.
            SetAttributeFromInteger( lpQualView, "QualAttrib",
                                     "SourceViewID", 0 );
            SetAttributeFromString( lpQualView, "QualAttrib",
                                    "SourceEntityName", "" );
         } // if ( zstrcmpi( szOper, "IS" ) == 0 )...
      } // for ... QualAttrib

   } // for ... EntitySpec

   // Copy every entity in lpQualView to qual path table.
   nRC = SetCursorFirstEntity( lpQualView, "EntitySpec", "" );
   while ( nRC == zCURSOR_SET )
   {
      nRC = AddSetToPathTable( QUAL_PATH, &lpSet, lpConnection );
      if ( nRC == -1 )
         return( -1 );

      lpSet->Child.nKeyCnt = 0;

      nRC = SetCursorFirstEntity( lpQualView, "QualAttrib", "" );
      while ( nRC == zCURSOR_SET )
      {
         // Convert key value in QualView (string) to
         // lpKey->lpKeyValue (binary).
         GetAddrForAttribute( (zPVOID) &szOper, lpQualView,
                              "QualAttrib", "Oper" );
         if ( szOper[ 0 ] && zstrcmp( szOper, "=" ) != 0 )
         {
            TraceLineS( "The following Qualification operation was not '='."
                        "  Operation is ", szOper );
            DisplayEntityInstancePath( lpQualView, "QualAttrib" );
            DBH_Error( lpQualView,
                       "Qualification operation must be '='.", 0, 0 );
            return( -1 );
         }

         GetAddrForAttribute( &pchEntityName, lpQualView, "QualAttrib",
                              "EntityName" );

         lpViewEntity = MiGetViewEntityForView( lpView, pchEntityName );

         // If lpViewEntity is null, then the entity name in lpQualView does
         // not exist in the target view OD.
         if ( lpViewEntity == 0 )
         {
            TraceLineS( "The following Qualification entity instance was not ",
                        "found in the target Object Defintion:" );
            DisplayEntityInstancePath( lpQualView, "QualAttrib" );
            DBH_Error( lpQualView,
                       "Qualification entity name not found in target Object "
                       "Definition.  Set trace for more.", 0, 0 );
            return( -1 );
         }

         // Copy set info to lpSet
         if ( lpViewEntity->hParent )
         {
            LPRELRECORD lpRelRecord;

            lpRelRecord = lpViewEntity->hFirstDataRecord->hFirstRelRecord;
            lpSet->nSetID = (zSHORT) lpRelRecord->lSetID1;
            lpSet->szSetName = lpRelRecord->szSetName1;
         }
         else
         {
            lpSet->nSetID    = 0;
            lpSet->szSetName = "";
         }

         lpSet->Child.nKeyCnt++;
         lpSet->Child.lpViewEntity = lpViewEntity;

         // Allocate key memory.
         hMem = SysAllocMemory( (zPVOID) &lpKey, (zLONG) zsizeof( KeyRecord ),
                                 0, zCOREMEM_ALLOC, 0 );
         if ( lpKey == 0 )
         {
            DBH_Error( lpQualView,
                       "Error allocating memory.", "lpKey #2", 0 );
            return( -1 );
         }

         // Insert key at the beginning of the key chain.
         zmemset( (zPCHAR) lpKey, 0, zsizeof( KeyRecord ) );
         lpKey->hMemory   = hMem;
         lpKey->lpNextKey = lpSet->Child.lpFirstKey;
         lpSet->Child.lpFirstKey = lpKey;

         // Find lpViewAttrib and lpDataField that corresponds to current
         // QualAttrib.
         GetAddrForAttribute( &pchAttribName, lpQualView, "QualAttrib",
                              "AttributeName" );
         lpDataField = lpViewEntity->hFirstDataRecord->hFirstDataField;
         while ( lpDataField &&
                 zstrcmp( lpDataField->hViewAttrib->szName,
                          pchAttribName ) != 0 )
            lpDataField = lpDataField->hNextDataField;

         // If lpDataField is null, then the attrib name in lpQualView does
         // not exist in the target view OD.
         if ( lpDataField == 0 )
         {
            DBH_Error( lpQualView,
                       "Qualification attribute name not found in target "
                       "Object Definition.  Set trace for more.", 0, 0 );
            TraceLineS( "The following Qualification attrib name was not ",
                        "found in the target Object Defintion:" );
            DisplayEntityInstancePath( lpQualView, "QualAttrib" );
            return( -1 );
         }

         lpKey->lpViewAttrib = lpDataField->hViewAttrib;
         lpKey->lpDataField  = lpDataField;

         // Convert key value in QualView (string) to lpKey->lpKeyValue (binary).
         GetAddrForAttribute( (zPVOID) &lpQualValue, lpQualView,
                              "QualAttrib", "Value" );
         if ( lpDataField->cFldType != zTYPE_STRING )
         {
            nRC = ConvertAttributeValue( 'S', (zPVOID) lpQualValue,
                                         lpDataField->cFldType,
                                         lpValueBuffer, &uLth );
            if ( nRC < 0 )
               return( -1 );

            vDbhWorkValue = (zPVOID) lpValueBuffer;
         }
         else
         {
            vDbhWorkValue = lpQualValue;
            uLth = zstrlen( (zPCHAR) lpQualValue ) + 1;
         }

         hMem = SysAllocMemory( (zPVOID) &lpKey->lpKeyValue, (zLONG) uLth, 0,
                                zCOREMEM_ALLOC, 0 );
         if ( lpKey->lpKeyValue == 0 )
         {
            DBH_Error( lpQualView, "(kzhdbvaa) Error allocating memory.",
                       "lpKeyValue #2", 0 );
            return( -1 );
         }

         zmemset( (zPCHAR) lpKey->lpKeyValue, 0, uLth );
         lpKey->hValueMemory = hMem;
         lpKey->ulLth = ulLth;
         zmemcpy( lpKey->lpKeyValue, vDbhWorkValue, ulLth );

         nRC = SetCursorNextEntity( lpQualView, "QualAttrib", "" );
      } // while ( nRC == zCURSOR_SET )...

      nRC = SetCursorNextEntity( lpQualView, "EntitySpec", "" );
   } // while ( nRC == zCURSOR_SET )...

   return( 0 );
}  /* CopyQualToPath */

/////////////////////////////////////////////////////////////////////////////

      CreateEntityInsFromCurrentRec

      Create an entity and then copy all attributes from the current record
      to the new entity.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CreateEntityInsFromCurrentRec( zVIEW        lpView,
                               LPVIEWENTITY lpViewEntity,
                               LPCONNECTION lpConnection )
{
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   LPDATARECORD lpDataRecord;
   LPVIEWATTRIB lpViewAttrib;
   zPVOID       pDataPtr, lpPtr;
   zLONG        hMem;
   zCHAR        TempAttributeBuffer[ 1000 ];
   zUSHORT      uLth;
   zSHORT       nRC;

   lpDataRecord = lpViewEntity->hFirstDataRecord;
   lpRelRecord  = lpDataRecord->hFirstRelRecord;

   // Retrieve current record.
   nRC = dt_recread( lpConnection->RecordBuffer,
                    &lpConnection->Currtask, CURR_DB );
   RDM_RC( "d_readrec", nRC, "Record name = ", lpDataRecord->szRecordName, 2 );
   if ( nRC != S_OKAY )
      return( -1 );

   // For each data field, copy the attribute value from the current record
   // to the entity.
   nRC = LoadEntity( lpView, lpViewEntity->szName, zPOS_AFTER, 0 );

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "Entity just created. Name = ", lpViewEntity->szName );
   }
#endif

   lpDataField = lpDataRecord->hFirstDataField;
   while ( lpDataField )
   {
      lpViewAttrib = lpDataField->hViewAttrib;

      // pDataPtr points to beginning of field in RecordBuffer.  If attribute
      // is an autoseq attribute and the relationship with the parent is
      // many-to-many, then retrieve the attributes value from the intersection
      // record instead of the entity record.
      if ( lpViewAttrib->bAutoSeq &&
           lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         pDataPtr = &lpConnection->IntersectBuffer[ lpDataField->uFldOffset ];
      else
         pDataPtr = &lpConnection->RecordBuffer[ lpDataField->uFldOffset ];

      // Check to see if the attribute is stored in the overflow record.  It
      // is stored in the overflow record if one of the following conditions
      // apply:
      //    o  The attribute is a Blob.
      //    o  The first 4 bytes are equal to the OVERFLOW_FLAG and the
      //       maximum attribute length is greater than the the field length.
      //       Note that this can only happen with strings (because blobs are
      //       already accounted for) so we must consider the effect of the
      //       null terminator, wich is why we use '>=' to compare the logical
      //       and the physical lengths.
      if ( (lpViewAttrib->cType == zTYPE_BLOB) ||
           (lpViewAttrib->cType == zTYPE_STRING &&
            *((zPLONG) pDataPtr) == zOVERFLOW_FLAG &&
            lpViewAttrib->ulLth >= lpDataField->ulFldLth) )
      {
         OverflowIndicatorRecord *Overflow;

         // Get length of overflow data.
         Overflow = (OverflowIndicatorRecord *) pDataPtr;
         uLth = Overflow->ulLth;

         // If length is greater than 0, then retrieve the attribute value
         // from the overflow area.
         if ( uLth > 0 )
         {
            hMem = SysAllocMemory( (zPVOID) &lpPtr, uLth, 0, zCOREMEM_ALLOC, 0 );
            if ( lpPtr == 0 )
            {
               DBH_Error( lpView, "Error alloc mem.", "Overflow Area.", 0 );
               return( -1 );
            }

#if TRACE
            if ( lpConnection->nTraceLevel > 1 )
            {
               TraceLineS( "", "" );
               TraceLineS( "Reading overflow records for attrib ",
                           lpViewAttrib->szName );
            }
#endif

            nRC = ReadOverflowRecord( pDataPtr, lpPtr, lpDataField, lpConnection );
            if ( nRC < 0 )
               return( -1 );

            pDataPtr = lpPtr;
         }
      }
      else
         hMem = 0;

      // If physical data type and logical data type are different, then
      // convert.
      if ( lpDataField->cFldType != lpViewAttrib->cType )
      {
         ConvertAttributeValue( lpDataField->cFldType, pDataPtr,
                                lpViewAttrib->cType,
                                (zPVOID) TempAttributeBuffer, &uLth );
         pDataPtr = (zPVOID) TempAttributeBuffer;
      }

      // Store data in record.
      StoreValueInRecord( lpView, lpViewEntity, lpViewAttrib, pDataPtr, uLth );

      // If memory was allocated for overflow records, free it.
      if ( hMem )
      {
         SysFreeMemory( hMem );
         hMem = 0;
      }

      // Point to next lpDataField.
      lpDataField = lpDataField->hNextDataField;
   } // while ( lpDataField )...

   return( 0 );
}  /* CreateEntityInsFromCurrentRec */

/////////////////////////////////////////////////////////////////////////////

      CreateOverflowRecord

      Copy the data from pDataValue to zero or more overflow records.

      Parameters:

      pchDataValue - Pointer to the full attribute value.
      uLth         - Length of attribute.
      pchBufferPtr - Pointer to are in record buffer to hold attribute.
      lpDataField  - Data field pointer for attribute.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CreateOverflowRecord( zPCHAR       pchDataValue,
                      zULONG       ulLth,
                      zPCHAR       pchBufferPtr,
                      LPDATAFIELD  lpDataField,
                      LPCONNECTION lpConnection )
{
   DB_ADDR dbLastDBA, dbCurrentRecord;
   long    hOverflowMem;
   OverflowIndicatorRecord *OverflowInd;
   OverflowRecord *OverflowRec;
   zULONG  lth;
   zSHORT  nRC;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "Storing attribute to overflow record", "" );
   }
#endif

   // Copy overflow flag and length of attribute to current record buffer.
   OverflowInd = (OverflowIndicatorRecord *) pchBufferPtr;
   OverflowInd->lFlag = zOVERFLOW_FLAG;
   OverflowInd->ulLth = ulLth;
   dbCurrentRecord    = 0;

   // First find any extra space in the record.
   lth = (zUSHORT) (lpDataField->uFldLth -
                   ( (zLONG) &OverflowInd->cBeginningOfData -
                     (zLONG) &OverflowInd->lFlag ) );
   // If extra space is greater than length of attribute, set lth to uLth to
   // use only the extra space that is needed to store the attribute.
   if ( lth > uLth )
      lth = uLth;

   // Copy first portion of attribute to extra space in record.
   zmemcpy( &OverflowInd->cBeginningOfData, pchDataValue, lth );

   // Reset pchDataValue to point past data stored in extra space.
   pchDataValue = &pchDataValue[ lth ];

   // Length of data that needs to be stored is now less.
   uLth = uLth - lth;
   dbLastDBA = 0;

   // If length is > 0, then data must be written to the overflow record.
   if ( uLth > 0 )
   {
      hOverflowMem = SysAllocMemory( (zPVOID) &OverflowRec,
                      lpConnection->nOVERFLOW_BUFFER_LTH + zsizeof( DB_ADDR ),
                      0, zCOREMEM_ALLOC, 0 );
      if ( OverflowRec == 0 )
      {
         DBH_Error( lpConnection->lpAppView,
                    "Error allocating memory.", "Overflow record", 0 );
         return( -1 );
      }

      // Store current record dba before we change the current record by
      // writing overflow records.
      nRC = dt_crget( &dbCurrentRecord, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crget", nRC, "Saving current dba", 0, 3 );

      // Starting with the END of the data value, save sections of data to
      // overflow record.
      while ( uLth > 0 )
      {
         zLONG k, j;

         k = (zLONG) ((zLONG) uLth / (zLONG) lpConnection->nOVERFLOW_BUFFER_LTH);
         j = (zLONG) ((zLONG) uLth % (zLONG) lpConnection->nOVERFLOW_BUFFER_LTH);

         if ( j == 0 )
         {
            lth = lpConnection->nOVERFLOW_BUFFER_LTH;
            k--;
         }
         else
            lth = (zUSHORT) j;

         // Copy attribute data to buffer that will be written to the overflow
         // record.
         zmemcpy( &OverflowRec->OverflowLine,
                  pchDataValue + (k * lpConnection->nOVERFLOW_BUFFER_LTH),
                  lth );
         uLth = uLth - lth;
         OverflowRec->dbNextOverflowRecord = dbLastDBA;
#if TRACE
         if ( lpConnection->nTraceLevel > 1 )
         {
            zCHAR szWork[ 20 ];

            TraceLineS( "", "" );
            TraceLineS( "About to store :",
                        (zPCHAR) OverflowRec->OverflowLine );
            TraceLineI( "Length = ", (zLONG) lth );
            DBA_ToStr( &dbLastDBA, szWork );
            TraceLineS( "Last DBA = ", szWork );
         }
#endif

         // Save Overflow rec to database.
         nRC = dt_fillnew( lpConnection->nOVERFLOW_RECORD_ID,
                          (zPVOID) OverflowRec,
                          &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_fillnew", nRC, "Creating overflow record", 0, 2 );
         if ( nRC != S_OKAY )
         {
            SysFreeMemory( hOverflowMem ); // Memory must be freed before abort.
            return( -1 );
         }

         nRC = dt_crget( &dbLastDBA, &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_crget", nRC, "Saving current record as LastDBA", "", 2 );
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            zCHAR szWork[ 10 ];
            int  rectype;

            DBA_ToStr( &dbLastDBA, szWork );
            TraceLineS( "current dba = ", szWork );

            dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
            TraceLineI( "Current record type = ", (zLONG) rectype );
         }
#endif
      }  // while ( uLth > 0 )...

      SysFreeMemory( hOverflowMem );
   } // if ( uLth > 0 )...

   // Save dba of first overflow record.
   OverflowInd->dbFirstOverflowRecord = dbLastDBA;

   // If dbCurrentRecord is not null, then we need to restore the current
   // record dba to the way it was before the function was called.
   if ( dbCurrentRecord )
   {
      nRC = dt_crset( &dbCurrentRecord, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crset", nRC, "Restoring current record after writes.", 0, 3 );
   }

   return( 0 );
}  /* CreateOverflowRecord */

/////////////////////////////////////////////////////////////////////////////

      DBA_ToStr

      Convert a database address to a string.

/////////////////////////////////////////////////////////////////////////////
void
DBA_ToStr( DB_ADDR *dba, zPCHAR pch )
{
   int  file, nRC;
   long slot;
   zCHAR szWork[ 10 ];

   nRC = d_decode_dba( *dba, &file, &slot );
   strcpy_s( pch, "@[" );
   zltoa( (zLONG) file, szWork );
   strcat_s( pch, szWork );
   strcat_s( pch, ":" );
   zltoa( (zLONG) slot, szWork );
   strcat_s( pch, szWork );
   strcat_s( pch, "]" );
}  /* DBA_ToStr */

/////////////////////////////////////////////////////////////////////////////

      DBH_Error

/////////////////////////////////////////////////////////////////////////////
void
DBH_Error( zVIEW  lpView,
           zPCHAR pchErrorMsg,
           zPCHAR pchInfo1,
           zPCHAR pchInfo2 )
{
   TraceLineS( "===== DBH_Entry Error =====", "" );
   TraceLineS( "Msg=> ", pchErrorMsg );

   if ( pchInfo1 )
   {
      if ( pchInfo2 )
         TraceLineS( pchInfo1, pchInfo2 );
      else
         TraceLineS( pchInfo1, "" );
   }

   MessageSend( lpView, "KZH0401", "RaimaDM_DBHandler",
                pchErrorMsg,
                zMSGQ_SYSTEM_ERROR, 0 );
// OperatorPrompt( lpView, "RaimaDM DBHandler", pchErrorMsg,
//                 1, 0, 0, zICON_ERROR );
}  /* DBH_Error */

/////////////////////////////////////////////////////////////////////////////

      DeleteOverflowRecords

      Delete Overflow records from database if bDelete = TRUE.  If bDelete
      = FALSE, just free up memory taken up by overflow delete chain.

/////////////////////////////////////////////////////////////////////////////
zSHORT
DeleteOverflowRecords( LPCONNECTION lpConnection, zBOOL bDelete )
{
   zSHORT     nRC;
   DB_ADDR    dba;
   long       hOverflowMem;
   LPOVERFLOW lpOverflow;
   OverflowRecord *lpOverflowRec;

#if TRACE
   if ( lpConnection->nTraceLevel > 2 )
   {
      TraceLineS( "", "" );
      TraceLineS( "Deleting overflow records from chain", 0 );
   }
#endif

   if ( bDelete )
   {
      // Allocate memory to store overflow data.
      hOverflowMem = SysAllocMemory( (zPVOID) &lpOverflowRec,
                        lpConnection->nOVERFLOW_BUFFER_LTH + zsizeof( DB_ADDR ),
                        0, zCOREMEM_ALLOC, 0 );
      if ( lpOverflowRec == 0 )
      {
         DBH_Error( lpConnection->lpAppView,
                     "Error allocating memory.", "Overflow record", 0 );
         return( -1 );
      }

      lpOverflow = lpConnection->lpOverflowDeleteChain;
      while ( lpOverflow )
      {
         dba = lpOverflow->dba;
         while ( dba != 0 )
         {
            nRC = dt_crset( &dba, &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_crset", nRC, 0, 0, 3 );
            if ( nRC != S_OKAY )
            {
               SysFreeMemory( hOverflowMem );
               return( -1 );
            }

            nRC = dt_recread( (zPVOID) lpOverflowRec,
                             &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_recread", nRC, 0, 0, 2 );
            if ( nRC != S_OKAY )
            {
               SysFreeMemory( hOverflowMem );
               return( -1 );
            }

            // Set dba to point to next overflow record (if there is one).
            dba = lpOverflowRec->dbNextOverflowRecord;

//          nRC = dt_delete( &lpConnection->Currtask, CURR_DB );
            nRC = dt_disdel( &lpConnection->Currtask, CURR_DB );  // Gig, 10/3/93
            RDM_RC( "d_delete", nRC, 0, 0, 2 );
            if ( nRC != S_OKAY )
            {
               SysFreeMemory( hOverflowMem );
               return( -1 );
            }
         } // while ( dba != 0 )...

         lpOverflow = lpOverflow->lpNextOverflowDelete;
      } // while ( lpOverflow )...

      SysFreeMemory( hOverflowMem );
   } // if ( bDelete )...

   lpOverflow = lpConnection->lpOverflowDeleteChain;
   while ( lpOverflow )
   {
      long hMem;

      hMem = lpOverflow->hMem;
      lpOverflow = lpOverflow->lpNextOverflowDelete;
      SysFreeMemory( hMem );
   }

   return( 0 );
}  /* DeleteOverflowRecords */

/////////////////////////////////////////////////////////////////////////////

      DeleteOverflowRecsForCurrentRec

      Loop through all the data fields for lpViewEntity and check for the
      use of overflow records.  If an overflow record is found, then add it's
      database address to the Overflow Delete Chain in lpConnection.  The
      records will be deleted just before the end of the transaction.

/////////////////////////////////////////////////////////////////////////////
zSHORT
DeleteOverflowRecsForCurrentRec( LPVIEWENTITY lpViewEntity,
                                 LPCONNECTION lpConnection )
{
   LPDATAFIELD  lpDataField;
   LPVIEWATTRIB lpViewAttrib;
   long         hMem;
   LPOVERFLOW   lpOverflow;
   OverflowIndicatorRecord *OverflowInd;

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "Deleting overflow records for entity ",
                  lpViewEntity->szName );
   }
#endif

   lpDataField = lpViewEntity->hFirstDataRecord->hFirstDataField;
   while ( lpDataField )
   {
      // The attribute is stored in the overflow record if one of the
      // following is true:
      //    o  The attribute is a blob.
      //    o  The maximum allowable length of the attribute is longer than
      //       the maximum field length AND the first 4 bytes of the field's
      //       value is equal to the to OVERFLOW_FLAG.
      lpViewAttrib = lpDataField->hViewAttrib;
      if ( (lpViewAttrib->cType == zTYPE_BLOB) ||
           (lpViewAttrib->cType == zTYPE_STRING &&
            lpViewAttrib->ulLth >= lpDataField->uFldLth &&
            *((zPLONG) &lpConnection->RecordBuffer[ lpDataField->uFldOffset ])
                                                        == zOVERFLOW_FLAG) )
      {
#if TRACE
         if ( lpConnection->nTraceLevel > 2 )
         {
            TraceLineS( "", "" );
            TraceLineS( "Adding overflow record to delete chain ",
                        lpViewAttrib->szName );
         }
#endif

         OverflowInd = (OverflowIndicatorRecord *)
                       &lpConnection->RecordBuffer[ lpDataField->uFldOffset ];
         if ( OverflowInd->dbFirstOverflowRecord != 0 )
         {
            // Allocate memory for overflow delete chain.
            hMem = SysAllocMemory( (zPVOID) &lpOverflow,
                                   zsizeof( OverflowDeleteRecord ),
                                   0, zCOREMEM_ALLOC, 0 );
            if ( lpOverflow == 0 )
            {
               DBH_Error( lpConnection->lpAppView,
                          "Error allocating memory.", "Overflow record", 0 );
               return( -1 );
            }

            lpOverflow->dba  = OverflowInd->dbFirstOverflowRecord;
            lpOverflow->hMem = hMem;

            // Add lpOverflow to overflow chain in lpConnection.
            lpOverflow->lpNextOverflowDelete =
                                        lpConnection->lpOverflowDeleteChain;
            lpConnection->lpOverflowDeleteChain = lpOverflow;

            // Zero out dba to indicate that record will be deleted.
            OverflowInd->dbFirstOverflowRecord == 0;
         } // if ( OverflowInd->dbFirstOverflowRecord != 0 )...

      } // if ( lpViewAttrib->cType == zTYPE_BLOB || ... )...

      lpDataField = lpDataField->hNextDataField;
   } // while ( lpDataField )...

   return( 0 );
}  /* DeleteOverflowRecsForCurrentRec */

/////////////////////////////////////////////////////////////////////////////

      FreeKeys

      Free all space allocated for keys for lpEntity.

/////////////////////////////////////////////////////////////////////////////
void
FreeKeys( LPENTITY lpEntity )
{
   LPKEY lpKey;

   lpKey = lpEntity->lpFirstKey;
   while ( lpKey )
   {
      lpEntity->lpFirstKey = lpKey->lpNextKey;

      SysFreeMemory( lpKey->hValueMemory );
      SysFreeMemory( lpKey->hMemory );

      lpKey = lpEntity->lpFirstKey;
   }

   lpEntity->lpFirstKey = 0;
}  /* FreeKeys */

/////////////////////////////////////////////////////////////////////////////

      FreeSets

      Free all space allocated for sets for lpPath.

/////////////////////////////////////////////////////////////////////////////
void
FreeSets( LPPATH lpPath )
{
   LPSET lpSet;

   // Return if path is null.
   if ( lpPath == 0 )
      return;

   lpSet = lpPath->lpFirstSet;
   while ( lpSet )
   {
      lpPath->lpFirstSet = lpSet->lpNextSet;
      FreeKeys( &lpSet->Child );
      SysFreeMemory( lpSet->hMemory );
      lpSet = lpPath->lpFirstSet;
   }

   lpPath->lpFirstSet = 0;
   lpPath->lpLastSet  = 0;
   lpPath->nSetCount  = 0;
}  /* FreeSets */

/////////////////////////////////////////////////////////////////////////////

      GetParentPathFromOI

      Retrieve the parent path starting with lpViewEntity to the root
      entity and save it in the lpPathTable[ OI_PATH ].

/////////////////////////////////////////////////////////////////////////////
zSHORT
GetParentPathFromOI( zVIEW        lpView,
                     LPVIEWENTITY lpViewEntity,
                     zSHORT       Message,
                     LPCONNECTION lpConnection )
{
   zSHORT bUniqueKey;
   zSHORT nRC;

   // First, delete old parent path from vDbhWork.
   FreeSets( &lpConnection->lpPathTable[ OI_PATH ] );

   // If Message is Load or Insert, then the first (or bottom-most) entity
   // should not be retrieved.  This is because for these operations either
   // the entity does not exist in the OI (for a load operation) or the
   // entity does not exist in the database (for a store operation).
   //
   // If the Message is InsertRel, then entity values must be saved for later
   // use but must not be included as part of the OI path (for the same
   // reasons that an Inserted entity is not included).
   if ( Message & ( DBH_Load | DBH_Insert ) )
   {
      lpViewEntity = lpViewEntity->hParent;
   }
   else
   if ( Message & DBH_InsertRel )
   {
      // First, delete old INS_REL X_Path (if it exists).
      FreeSets( &lpConnection->lpPathTable[ INS_REL ] );

      // Store the values of the inserted entity.
      nRC = AddEntityToParentPath( lpView, INS_REL, lpViewEntity,
                                   lpConnection );
      if ( nRC < 0 )
         return( -1 );

      // Point to parent entity of lpViewEntity.
      lpViewEntity = lpViewEntity->hParent;
   }

   // Now create DB_Path entity for OI_PATH.
   // For each entity in the OI parent path, go through all the attributes
   // and retrieve the value of any keys and store them in vDbhWork.
   // Stop when one of two things happen:
   //    o  The keys for the root entity are retrieved.
   //    o  The entity just retrieved has a unique key (i.e. it can be found
   //       directly without it's parent relationship).
   bUniqueKey = FALSE;
   while ( lpViewEntity )
   {
      nRC = AddEntityToParentPath( lpView, OI_PATH, lpViewEntity,
                                   lpConnection );
      if ( nRC < 0 )
         return( -1 );

      if ( nRC == 1 )  // If nRC = 1, then the entity has a unique key.
      {
         // If the message is NOT Delete or DeleteRel, then nothing else
         // needs to be retrieved, so break.
         if ( !(Message & (DBH_Delete | DBH_DeleteRel)) )
            break;

         // If the previous entity had a unique key, then break.
         if ( bUniqueKey )
            break;

         bUniqueKey = TRUE;
      }

      // Point to parent entity of lpViewEntity.
      lpViewEntity = lpViewEntity->hParent;
   }  // while ( lpViewEntity )...

   return( 0 );
}  /* GetParentPathFromOI */

/////////////////////////////////////////////////////////////////////////////

      LocateFirstRecord

      uControl flags:

      zLOC_USE_PARENT - Connect must be TRUE.  The current record is the
                        parent of the set.  Use this to locate the record.
      zLOC_NO_USE_PARENT, zLOC_CONNECT - Locate the record using keys and
                        then connect the record to it's parent.
      zLOC_NO_USE_PARENT, zLOC_NO_CONNECT - Locate the record using keys,
                        ignoring any sets.

      ASSUMES: If the locate is to take place using the parent, then the
               current record is assumed to be the parent.

      Returns: 1 - Record not found.
               0 - Record found.
              -1 - Error.

/////////////////////////////////////////////////////////////////////////////
int
LocateFirstRecord( LPENTITY     lpEntity,
                   zUSHORT      uControl,
                   LPCONNECTION lpConnection )
{
   zPCHAR   pchEntityName, pchParentRecordName;
   zPCHAR   pchRecordName;
   DB_ADDR  ParentRecordDBA, dba;
   zSHORT   nKeyCnt;
   zPCHAR   lpKeyValue;
   zSHORT   bUseParent, bConnect;
   zSHORT   nRC;

   LPVIEWENTITY lpViewEntity;
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   LPVIEWATTRIB lpViewAttrib;
   LPDATAFIELD  lpDataField;

#if DBQuery
   zCHAR  szWork1[ 100 ], szWork2[ 100 ];
   zCHAR  szWhereClause[ 500 ], szSelect[ 1000 ];
   zPCHAR szErrMsg;
   VALUE  vals[ 24 ];
   int    nErrPos;
   int    count;
#endif

   if ( zLOC_NO_USE_PARENT == ( zLOC_NO_USE_PARENT & uControl ) )
   {
      bUseParent = FALSE;
      if ( zLOC_NO_CONNECT == ( zLOC_NO_CONNECT & uControl ) )
         bConnect = FALSE;
      else
         bConnect = TRUE;
   }
   else
   {
      bUseParent = TRUE;
      bConnect   = TRUE;
   }

   // Get data from entity.
   lpViewEntity  = lpEntity->lpViewEntity;
   pchEntityName = lpViewEntity->szName;
   nKeyCnt       = lpEntity->nKeyCnt;

   lpDataRecord  = lpViewEntity->hFirstDataRecord;
   pchRecordName = lpDataRecord->szRecordName;
   lpRelRecord   = lpDataRecord->hFirstRelRecord;

   // If there is no parent, don't to use parent or connect.
   if ( lpViewEntity->hParent == 0 )
   {
      bUseParent = FALSE;
      bConnect   = FALSE;
   }

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "Locating entity ", pchEntityName );
      TraceLineI( "   uControl   = ", (zLONG) uControl );
      TraceLineI( "   bUseParent = ", (zLONG) bUseParent );
      TraceLineI( "   bConnect   = ", (zLONG) bConnect );
   }
#endif

   // If the parent entity is to be used in the Locate, get the dba of the
   // the parent, which should be the current record.
   if ( bUseParent || bConnect )
   {
      // Save current record dba as ParentRecordDBA for later use.
      nRC = dt_crget( &ParentRecordDBA, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crget", nRC, "Saving current record as parent.  Record = ",
              pchRecordName, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &ParentRecordDBA, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif

      pchParentRecordName =
                     lpViewEntity->hParent->hFirstDataRecord->szRecordName;
   }  // if ( lpViewEntity->hParent )...

   // If relationship is parent-is-member, then there is no need to locate
   // the record using keys.  The record can be found by locating the owner
   // of the parent set.
   if ( bUseParent &&
        lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
   {
      // There can only be one child if the parent is the member.
      // Check to make sure that the current record is a member of the set.
      nRC = dt_ismember( (int) lpRelRecord->lSetID1,
                         &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_ismember", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC == S_EOS )
         return( 1 );
      else
      if ( nRC != S_OKAY )
         return( -1 );

      // Set current (parent) record as member of parent set.
      nRC = dt_setmr( (int) lpRelRecord->lSetID1,
                      &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setmr", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Set current record from current owner (child) of parent set.
      nRC = dt_setro( (int) lpRelRecord->lSetID1,
                      &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );
   }  // if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )...

   // If nKeyCnt == 1, then the record may be retrieved using the index.
   // Locate the record here and connect the record to it's owner later.
   else
   if ( nKeyCnt == 1 )
   {
      lpViewAttrib = lpEntity->lpFirstKey->lpViewAttrib;
      lpDataField  = lpEntity->lpFirstKey->lpDataField;
      lpKeyValue   = lpEntity->lpFirstKey->lpKeyValue;

      nRC = dt_keyfind( lpDataField->lFldID, lpKeyValue,
                       &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_keyfind", nRC, "Field name = ", lpDataField->szFldName, 2 );
#if TRACE
      if ( lpConnection->nTraceLevel > 1 )
      {
         zCHAR   szWork [ 20 ];
         zUSHORT uLth;
         zSHORT  nRC2;

         TraceLineI( "Key Id    ==> ", lpDataField->lFldID );
         TraceLineS( "Record name   ", lpViewAttrib->hViewEntity->
                     hFirstDataRecord->szRecordName );
         szWork[ 0 ] = lpDataField->cFldType;
         szWork[ 1 ] = 0;
         TraceLineS( "Key type  ==> ", szWork );

         if ( lpDataField->cFldType != 'S' )
         {
            nRC2 = ConvertAttributeValue ( lpDataField->cFldType, lpKeyValue,
                                          'S', szWork, &uLth );
            if ( nRC2 < 0 )
               return( -1 );

            TraceLineS( "Key value ==> ", szWork );
         }
         else
            TraceLineS( "Key value ==> ", (zPCHAR) lpKeyValue );
      }
#endif
      if ( nRC == S_NOTFOUND )
         return( 1 );
      else
      if ( nRC != S_OKAY )
         return( -1 );

      // Now connect the current record to it's parent set.
      if ( bConnect )
      {
#if TRACE
         if ( lpConnection->nTraceLevel > 1 )
         {
            TraceLineS( "", "" );
            TraceLineS( "Connecting current record with it's parent", "" );
         }
#endif

         if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_MEMBER )
         {
            // This should never happen--already checked for.
         }
         else
         if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
         {
            nRC = dt_setmr( lpRelRecord->lSetID1,
                           &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setmr", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
            if ( nRC != S_OKAY )
               return( -1 );
         }
         else
         if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
         {
            // Set current record as owner of parent set 2.
            nRC = dt_setor( lpRelRecord->lSetID2,
                           &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
            if ( nRC != S_OKAY )
               return( -1 );

            // Find first member of parent set 2.
            nRC = dt_findfm( lpRelRecord->lSetID2,
                            &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_findfm", nRC, "Set name = ",
                    lpRelRecord->szSetName2, 2 );
            if ( nRC != S_OKAY )
               return( -1 );

            // Read thru each of the members of set 2 until the owner of the
            // intersection record thru set 1 = ParentRecordDBA.
            while ( TRUE )
            {
               // Set current member of parent set 1 from current record.
               nRC = dt_setmr( lpRelRecord->lSetID1,
                              &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_setmr", nRC, "Set name = ",
                       lpRelRecord->szSetName1, 2 );
               if ( nRC != S_OKAY )
                  return( -1 );

               // Get dba of current owner of parent set 1.
               nRC = dt_csoget( lpRelRecord->lSetID1, &dba,
                               &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_csoget", nRC, 0, 0, 3 );
               if ( nRC != S_OKAY )
                  return( -1 );

#if TRACE
               if ( lpConnection->nTraceLevel > 2 )
               {
                  zCHAR szWork[ 20 ];

                  TraceLineS( "", "" );
                  TraceLineS( "Comparing dba with ParentRecordDBA", "" );
                  DBA_ToStr( &dba, szWork );
                  TraceLineS( "dba             = ", szWork );
                  DBA_ToStr( &ParentRecordDBA, szWork );
                  TraceLineS( "ParentRecordDBA = ", szWork );
               }
#endif

               // If dba = ParentRecordDBA, then the corrent intersection
               // record has been found--break " while ( TRUE ) " loop.
               if ( dba == ParentRecordDBA )
                  break;

               // Match wasn't found, so get next member of parent set 2.
               nRC = dt_findnm( lpRelRecord->lSetID2,
                               &lpConnection->Currtask, CURR_DB );
               RDM_RC( "d_findnm", nRC, "Set name = ",
                       lpRelRecord->szSetName2, 2 );
               if ( nRC != S_OKAY )
                  return( -1 );
            }  // while ( TRUE )...

            // Reset current record from owner of parent set 2.
            nRC = dt_setro( lpRelRecord->lSetID2,
                           &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setro", nRC, "Set name = ", lpRelRecord->szSetName2, 2 );
            if ( nRC != S_OKAY )
               return( -1 );

         }  // else if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...
      }  // if ( lpRelRecord )...
   } // if ( lKeyCount == 1 )...

   // If lKeyCount > 1, then the record must be retrieved using DBQuery.
   else
   if ( TRUE )
   {
      // DBQuery not supported yet.
      DBH_Error( lpConnection->lpAppView, "#4 Internal error", 0, 0 );
      return( -1 );
   }
   else
   if ( lpRelRecord == 0 )
   {
#if DBQuery
      // Build select stmt for locating root entity.
      hey!!! there ain't no vDbhWork
      BuildWhereClause( vDbhWork, szWhereClause, pchRecordName, 0 );
      sprintf_s( szSelect, "SELECT 1 FROM %s %s;", pchRecordName,
               szWhereClause );

      // Execute select statement.
      nRC = q_sqlinit( szSelect, &nErrPos, &szErrMsg );
      RDM_RC( "q_sqlinit", nRC, "Select stmt = ", szSelect, 2 );
      if ( nRC != Q_OKAY )
         return( -1 );

      nRC = q_nextval( &count, vals );
      RDM_RC( "q_nextval", nRC, 0, 0, 2 );
#endif
   } // if ( !lpRelRecord )...
   else
   if ( lpRelRecord->cOwnerMember == zDBH_PARENT_IS_OWNER )
   {
#if DBQuery
      // Build select stmt for locating child entity.
      sprintf_s( szSelect, "SELECT 1 FROM %s", lpRelRecord->szSetName1 );
      DBA_ToStr( &ParentRecordDBA, szWork2 );
      sprintf_s( szWork1, " ( db_addr( %s ) = %s ) AND ", pchParentRecordName,
               szWork2 );
      hey!!! there ain't no vDbhWork
      BuildWhereClause( vDbhWork, szWhereClause, pchRecordName, szWork1 );
      strcat_s( szSelect, szWhereClause );
      strcat_s( szSelect, ";" );

      // Execute select statement.
      nRC = q_sqlinit( szSelect, &nErrPos, &szErrMsg );
      RDM_RC( "q_sqlinit", nRC, "Select stmt = ", szSelect, 2 );
      if ( nRC != Q_OKAY )
         return( -1 );

      nRC = q_nextval( &count, vals );
      RDM_RC( "q_nextval", nRC, 0, 0, 2 );

      // Set current record from member of parent set.
      nRC = dt_setrm( (int) lpRelRecord->lSetID1,
                     &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setrm", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );
#endif
   }
   else
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
#if DBQuery
      // Set current record as owner of 1st set.
      nRC = dt_setor( (int) lpRelRecord->lSetID1,
                      &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_setor", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Find 1st member in the set.
      nRC = dt_findfm( (int) lpRelRecord->lSetID1,
                      &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_findfm", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // Try to find the child entity by polling thru each intersection
      // member of the parent set and seeing if there is a match.
      while ( nRC != S_EOS )
      {
         // Set the current record as the member of the 1st intersect set.
         nRC = dt_setrm( (int) lpRelRecord->lSetID1,
                        &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_setrm", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );

         // Build SELECT statment.
         nRC = dt_crget( &ParentRecordDBA, &lpConnection->Currtask, CURR_DB );
         DBA_ToStr( &ParentRecordDBA, szWork1 );
         sprintf_s( szWork2, " ( db_addr( %s ) = %s ) AND ",
                  lpRelRecord->szRecordName, szWork1 );
         hey!!! there ain't no vDbhWork
         BuildWhereClause( vDbhWork, szWhereClause, pchRecordName, szWork2 );
         sprintf_s( szSelect, "SELECT 1 FROM %s %s;",
                  lpRelRecord->szSetName2, szWhereClause );

         // Execute select statement.
         nRC = q_sqlinit( szSelect, &nErrPos, &szErrMsg );
         RDM_RC( "q_sqlinit", nRC, "Select stmt = ", szSelect, 2 );
         if ( nRC != Q_OKAY )
            return( -1 );

         // Check to see if a row was found.
         nRC = q_nextval( &count, vals );
         if ( nRC == Q_OKAY )
         {
            // A row was found--set current record from owner of parent set.
            nRC = dt_setro( (int) lpRelRecord->lSetID2,
                           &lpConnection->Currtask, CURR_DB );
            RDM_RC( "d_setro", nRC, "Set name = ",
                        lpRelRecord->szSetName2, 2 );
            if ( nRC != S_OKAY )
               return( -1 );

            break;  // Record was found, no need to try any more.
         }
         else
         if ( nRC != Q_NOTFOUND )
         {
            RDM_RC( "q_nextval", nRC, 0, 0, 2 );
            return( -1 );
         }

         // Get the next member of the 1st intersection set.
         nRC = dt_findnm( (int) lpRelRecord->lSetID1,
                         &lpConnection->Currtask, CURR_DB );
         RDM_RC( "d_findnm", nRC, "Set name = ", lpRelRecord->szSetName1, 2 );
         if ( nRC != S_OKAY )
            return( -1 );
      }  // while ( nRC != S_EOS )...
#endif
   }  // if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )...

#if TRACE
   if ( lpConnection->nTraceLevel > 2 )
   {
      zCHAR szWork[ 10 ];
      int  rectype;

      // Save current dba for entity.
      nRC = dt_crget( &dba, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crget", nRC, "Checking current record after Locate.", 0, 3 );
      DBA_ToStr( &dba, szWork );
      TraceLineS( "current dba = ", szWork );

      dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
      TraceLineI( "Current record type = ", (zLONG) rectype );
   }
#endif

   return( 0 );
}  /* LocateFirstRecord */

/////////////////////////////////////////////////////////////////////////////

      LocateNextRecord

      Returns:  1 - No record found.
                0 - Record found.
               -1 - Error.

/////////////////////////////////////////////////////////////////////////////
int
LocateNextRecord( LPENTITY lpEntity,
                  LPCONNECTION lpConnection )
{
   zPCHAR   pchEntityName;
   zPCHAR   pchKeyValue;
   zSHORT   nKeyCnt;
   zULONG   ulKeyLth;
   zCHAR    lpFieldValue[ 1000 ];
   int      nRC;

   LPVIEWENTITY lpViewEntity;
   LPDATAFIELD  lpDataField;

#if DBQuery
   zCHAR  szWork1[ 100 ], szWork2[ 100 ];
   zCHAR  szWhereClause[ 500 ], szSelect[ 1000 ];
   zPCHAR pchErrMsg, pchRecordName;
   VALUE  vals[ 24 ];
   int    nErrPos;
   int    count;
#endif

   // Get data from entity.
   lpViewEntity  = lpEntity->lpViewEntity;
   pchEntityName = lpViewEntity->szName;
   nKeyCnt       = lpEntity->nKeyCnt;

   if ( nKeyCnt == 1 )
   {
      lpDataField = lpEntity->lpFirstKey->lpDataField;
      pchKeyValue = lpEntity->lpFirstKey->lpKeyValue;
      ulKeyLth    = lpEntity->lpFirstKey->ulLth;

      // Set key pointer to next key.
      nRC = dt_keynext( lpDataField->lFldID, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_keynext", nRC, "Field name = ", lpDataField->szFldName, 2 );
      if ( nRC == S_NOTFOUND )
         return( 1 );
      else
      if ( nRC != S_OKAY )
         return( -1 );

      // Read the key's value.
      nRC = dt_keyread( &lpFieldValue, &lpConnection->Currtask );
      RDM_RC( "d_keyread", nRC, 0, 0, 2 );
      if ( nRC != S_OKAY )
         return( -1 );

      // If key read = key value, then record found.
      if ( zmemcmp( pchKeyValue, lpFieldValue, uKeyLth ) == 0 )
         return( 0 );
      else
         return( 1 );
   }
   else
   {
#if DBQuery
      nRC = q_nextval( &count, vals );
      RDM_RC( "q_nextval", nRC, 0, 0, 2 );
      if ( nRC == Q_NOTFOUND )
         return( 1 );
      else
      if ( nRC != Q_OKAY )
         return( -1 );
      else
         return( 0 );
#endif
   }
}  /* LocateNextRecord */

/////////////////////////////////////////////////////////////////////////////

      NavigateOI_ParentPath

      Returns:  1 - Child record of LAST set not located.
                0 - Record located.
               -1 - Error locating record.

/////////////////////////////////////////////////////////////////////////////
int
NavigateOI_ParentPath( zUSHORT Message, LPCONNECTION lpConnection )
{
   zSHORT  nRC;
   LPSET   lpOI_Set;
   zBOOL   bLastSet;

   // Set up the ptr that points to the OI path.
   lpOI_Set = lpConnection->lpPathTable[ OI_PATH ].lpFirstSet;
   bLastSet = TRUE;
   nRC = 0;

   // Only Navigate if there are OI entities.
   if ( lpOI_Set )
   {
      // Starting with the last set in OI view, loop until
      // an entity is found that can be located by unique key.  If no entity
      // is found, then set lpOI_Set to previous OI set.  If
      // there are no more sets, then the root entity must be located.
      while ( TRUE )
      {
         // Now check to see if the OI child can be
         // retrieved directly using a key.  If it can, then locate the
         // child that way.  If it can't be located by key, then try
         // again on the prev set in the OI path.
         // If we a dealing with a delete or delete-rel operation, then we
         // must retrieve the child entity AND it's parent (if it has one)
         // so that they may be disconnected.  The following is true if:
         //
         //    o  The child entity of the set can be found by 1 unique key.
         //
         //    AND
         //
         //       o  The operation is NOT a delete or delete rel operation.
         //          If it is not, then we can retrieve the bottom-most
         //          entity without locating it's parent.
         //       OR
         //       o  If the operation is a delete or delete rel, then we must
         //          locate the parent of the record as well as the child, so
         //          locate this record only if:
         //             -  It is NOT the bottom-most record in the parent path
         //                (i.e. lpOI_Set->lpPrevSet != 0)
         //             OR
         //             -  It is the root (i.e. lpOI_Set->lpNextSet == 0)
         //
         if ( lpOI_Set->Child.nKeyCnt == 1 &&
              ( !(Message & (DBH_Delete | DBH_DeleteRel)) ||
                (lpOI_Set->lpPrevSet != 0 || lpOI_Set->lpNextSet == 0)) )
         {
#if TRACE
            if ( lpConnection->nTraceLevel > 1 )
            {
               TraceLineS( "Child entity can be found by key.", "" );
               TraceLineS( "Locate child by key -- NO_USE_PARENT & NO_CONNECT",
                           "" );
            }
#endif

            // Child entity can be found by using key. If nRC is != 0, then
            // either:
            //
            //    nRC =  1 -- record was not found.   OR
            //    nRC = -1 -- error locating record.
            //
            // In either case, exit routine.
            nRC = LocateFirstRecord( &lpOI_Set->Child,
                                     zLOC_NO_USE_PARENT | zLOC_NO_CONNECT,
                                     lpConnection );
            if ( nRC )
            {
               // If the record not found is the child of the last set, then
               // set return code to 1, otherwise set to -1.
               if ( bLastSet && nRC > 0 )
                  nRC = 1;
               else
               {
                  DBH_Error( lpConnection->lpAppView,
                             "A child record could not be found by key. "
                             "See trace for more.", 0, 0 );
                  nRC = -1;
               }

               // Record was not found, so end loop.
               goto EndOfFunction;
            }
            else
               // Match was found; break out of while loop.
               break;
         }

         // Try again with the previous OI set.
         lpOI_Set = lpOI_Set->lpNextSet;
         if ( lpOI_Set == 0 )
         {
            DBH_Error( lpConnection->lpAppView,
                       "Internal error #5 -- Entity does NOT have"
                       " one and only one key.", "Entity name = ",
                       lpOI_Set->Child.lpViewEntity->szName );
            nRC = -1;
            goto EndOfFunction;
         }

         // The next set we check is not the last set.
         bLastSet = FALSE;
      } // while ( TRUE )...

      // A matching set/child has been found.  Now, with any
      // remaining sets after the current OI set, locate the child entity.
      lpOI_Set = lpOI_Set->lpPrevSet;
      while ( lpOI_Set )
      {
         nRC = LocateFirstRecord( &lpOI_Set->Child, zLOC_USE_PARENT,
                                  lpConnection );
         if ( nRC )
         {
            // If there are more sets, then the record not located was not
            // a child of the last set.  This is an error, so return -1.
            // If the unlocated record IS the child of the last set, then
            // return 1.
            if ( lpOI_Set->lpPrevSet || nRC < 0 )
            {
               DBH_Error( lpConnection->lpAppView,
                          "A child record could not be found under i'ts "
                          "parent.  See trace for more.", 0, 0 );
               nRC = -1;
            }
            else
               nRC = 1;

            goto EndOfFunction;
         }

         lpOI_Set = lpOI_Set->lpPrevSet;
      }

      // No error occurred up until now, so set return code to OK.
      nRC = 0;
   } // if ( lpOI_Set )...


   // Termination of this function must come here to free up any memory.
   EndOfFunction:

   // Clean up.
   FreeSets( &lpConnection->lpPathTable[ OI_PATH ] );

#if TRACE
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "End of navigation", "" );
      TraceLineI( "Return code for navigation = ", (zLONG) nRC );
   }
#endif

   return( nRC );
}  /* NavigateOI_ParentPath */

#if TRACE
/////////////////////////////////////////////////////////////////////////////

      RaimaDM_RC

/////////////////////////////////////////////////////////////////////////////
void
RaimaDM_RC( zPCHAR       szRoutine,
            zSHORT       nRC,
            zPCHAR       pchInfo1,
            zPCHAR       pchInfo2,
            zSHORT       nTrcLvl,
            LPCONNECTION lpConnection )
{
   if ( lpConnection->nTraceLevel == 0 || ( nRC >= 0 && lpConnection->nTraceLevel < nTrcLvl ) )
      return;

   TraceLineS( "", "" );
   if ( nRC < 0 )
      TraceLineS( "===== DBVista Error =====", "" );

   TraceLineS( "DBVista Call : ", szRoutine );

   if ( pchInfo1 )
   {
      if ( pchInfo2 )
         TraceLineS( pchInfo1, pchInfo2 );
      else
         TraceLineS( pchInfo1, "" );
   }

   TraceLineI( "Return Code :", nRC );
}  /* RaimaDM_RC */
#endif

/////////////////////////////////////////////////////////////////////////////

      ReadOverflowRecord

      Parameters:

      lpSrc       - Pointer to data in current record buffer.
      lpDest      - Pointer to buffer that will contain full attribute value
                    when read is done.  Assumed to be large enough.
      lpDataField - Datafield pointer to attribute in question.

/////////////////////////////////////////////////////////////////////////////
zSHORT ReadOverflowRecord ( zPVOID       lpSrc,
                            zPCHAR       lpDest,
                            LPDATAFIELD  lpDataField,
                            LPCONNECTION lpConnection )
{
   DB_ADDR dbaSave, dba;
   long    hOverflowMem;
   OverflowIndicatorRecord *OverflowInd;
   OverflowRecord *OverflowRec;
   zULONG  ulLth, lth;
   zSHORT  nRC;

   // Save current dba if we are going to be reading overflow records.
   OverflowInd = (OverflowIndicatorRecord *) lpSrc;
   ulLth = OverflowInd->ulLth;
   if ( OverflowInd->dbFirstOverflowRecord != 0 )
   {
      nRC = dt_crget( &dbaSave, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crget", nRC, "Saving current rec before reading overflow.",
              0, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dbaSave, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif

      if ( nRC != S_OKAY )
         return( -1 );

      hOverflowMem = SysAllocMemory( (zPVOID) &OverflowRec,
                       lpConnection->nOVERFLOW_BUFFER_LTH + zsizeof( DB_ADDR ),
                       0, zCOREMEM_ALLOC, 0 );
      if ( OverflowRec == 0 )
      {
         DBH_Error( lpConnection->lpAppView,
                    "Error allocating memory.", "Overflow record", 0 );
         return( -1 );
      }
   } // if ( OverflowInd->dbFirstOverflowRecord != 0 )...

   zmemset( lpDest, 0, uLth );  // Fill destination buffer with zeros.

   // Copy beginning of data from the space that's left in the record buffer.
   lth = lpDataField->uFldLth -
         (int) ( (zLONG) &OverflowInd->cBeginningOfData -
                 (zLONG) &OverflowInd->lFlag );
   if ( lth > uLth )
      lth = uLth;

   zmemcpy( (zPVOID) lpDest, &OverflowInd->cBeginningOfData, lth );

   lpDest = &lpDest[ lth ];
   uLth = uLth - lth;

   // Read first overflow record and get the data from it.  Continue processing
   // with the next overflow record until there are no more overflow records
   // in the overflow chain.
   dba = OverflowInd->dbFirstOverflowRecord;
   while ( dba != 0 )
   {
      nRC = dt_crset( &dba, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crset", nRC, "Setting current rec in Overflow chain.", 0, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dba, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif
      if ( nRC != S_OKAY )
      {
         SysFreeMemory( hOverflowMem );
         return( -1 );
      }

      // Read overflow record.
      nRC = dt_recread( OverflowRec, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_recread", nRC, "Reading overflow record.", 0, 2 );
      if ( nRC != S_OKAY )
      {
         SysFreeMemory( hOverflowMem );
         return( -1 );
      }

      // Copy data from overflow rec to lpDest.
      if ( uLth > lpConnection->nOVERFLOW_BUFFER_LTH )
         lth = lpConnection->nOVERFLOW_BUFFER_LTH;
      else
         lth = uLth;
      zmemcpy( (zPVOID) lpDest, (zPVOID) OverflowRec->OverflowLine, lth );
      uLth = uLth - lth;

      lpDest = &lpDest[ lth ];

      dba = OverflowRec->dbNextOverflowRecord;
   } // while ( dba != 0 )...

   // If overflow records were read, reset current record to the way it was
   // before calling this func.
   if ( OverflowInd->dbFirstOverflowRecord != 0 )
   {
      // Release overflow record memory
      SysFreeMemory( hOverflowMem );

      nRC = dt_crset( &dbaSave, &lpConnection->Currtask, CURR_DB );
      RDM_RC( "d_crset", nRC, "Resetting current record after overflow.",
              0, 3 );
#if TRACE
      if ( lpConnection->nTraceLevel > 2 )
      {
         zCHAR szWork[ 10 ];
         int  rectype;

         DBA_ToStr( &dbaSave, szWork );
         TraceLineS( "current dba = ", szWork );

         dt_crtype( &rectype, &lpConnection->Currtask, CURR_DB );
         TraceLineI( "Current record type = ", (zLONG) rectype );
      }
#endif
      if ( nRC != S_OKAY )
         return( -1 );
   } // if ( OverflowInd->dbFirstOverflowRecord != 0 )...

   return( 0 );
}  /* ReadOverflowRecord */

/////////////////////////////////////////////////////////////////////////////

      TracePathTable

/////////////////////////////////////////////////////////////////////////////
void
TracePathTable( zSHORT nPathType, LPCONNECTION lpConnection )
{
   LPSET   lpSet, lpLastSet;
   LPKEY   lpKey;
   LPPATH  lpPath;
   zCHAR   szWork[ 50 ];
   zUSHORT uLth, bPointersScrewed;

   TraceLineS( "", "" );
   if ( nPathType == OI_PATH )
      TraceLineS( "Printing OI_PATH", "" );
   else
   if ( nPathType == INS_REL )
      TraceLineS( "Printing INS_REL", "" );
   else
   if ( nPathType == QUAL_PATH )
      TraceLineS( "Printing QUAL_PATH", "" );

   lpPath = &lpConnection->lpPathTable[ nPathType ];
   TraceLineI( "# of sets = ", (zLONG) lpPath->nSetCount );

   bPointersScrewed = FALSE;
   lpLastSet = 0;
   lpSet = lpPath->lpFirstSet;
   while ( lpSet )
   {
      if ( lpSet->lpPrevSet != lpLastSet )
         bPointersScrewed = TRUE;
      lpLastSet = lpSet;

      TraceLineS( "SET:", "" );
      TraceLineS( "   Set name = ", lpSet->szSetName );
      TraceLineI( "   Set id   = ", (zLONG) lpSet->nSetID );

      TraceLineS( "   Child ent  = ", lpSet->Child.lpViewEntity->szName );
      TraceLineI( "   Child keys = ", (zLONG) lpSet->Child.nKeyCnt );

      lpKey = lpSet->Child.lpFirstKey;
      while ( lpKey )
      {
         TraceLineS( "      KEY:", "" );
         TraceLineS( "         Attrib Name = ", lpKey->lpViewAttrib->szName );
         TraceLineS( "         Field Name  = ", lpKey->lpDataField->szFldName );
         if ( lpKey->lpDataField->cFldType != 'S' )
         {
            ConvertAttributeValue( lpKey->lpDataField->cFldType,
                                   lpKey->lpKeyValue, 'S', szWork, &uLth );
            TraceLineS( "         Key value   = ", szWork );
         }
         else
            TraceLineS( "         Key value   = ", (zPCHAR) lpKey->lpKeyValue );

         TraceLineI( "         Key length  = ", (zLONG) lpKey->ulLth );

         lpKey = lpKey->lpNextKey;
      }

      lpSet = lpSet->lpNextSet;
   }  // while ( lpSet )...

   TraceLineS( "", "" );
   if ( bPointersScrewed )
      TraceLineS( "Pointers are SCREWED UP!", "" );
   else
      TraceLineS( "Pointers seem to be OK", "" );

}  /* TracePathTable */

DB_VOID OPERATION
LocalDBError( int nErrorNum, zPCHAR pchErrorMsg )
{
   TraceLineS( "RaimaDM API call error:", "" );
   TraceLineI( "Error code    = ", nErrorNum );
   TraceLineS( "Error message = ", pchErrorMsg );

} // LocalDBError
