
   KZHSKEL.C - Database handler for Gupta SQL Base.

   Programming Notes:

      o  All function return codes use the following convention:
         - rc = 0 means that the function worked as intended.
         - rc > 0 means that the function worked but with warnings.
         - rc < 0 means that an error occured.

///////////////////////////// DEFINES ///////////////////////////////////////
// Following macro makes it a little easier to print results to Zeidon Trace.
#define SQL_RC( a, b, c, d, e )  SQLBase_RC( a, b, c, d, e, lpConnection )

#if 0

#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include <kzoengaa.h>
#include <zdrvropr.h>

#else

#include "kzoephdr.h"
#include <kzdlgoaa.h>

#endif

#include <stdio.h>
#include <sql.h>

// Include the definitions for the generic SQL generator.
#include "KZHSQLGP.H"


#define TABLE_BUFFER_LTH           32000
#define COMMAND_BUFFER_LTH         10000
#define MAX_OPEN_DATABASES         10

///////////////////////////// TYPEDEF'S /////////////////////////////////////

// Following structure used to store information during object operations.
typedef struct ConnectionArea ConnectionRecord;
typedef ConnectionRecord *LPCONNECTION;
struct ConnectionArea
{
   int          nTraceLevel;
   LPQUALENTITY lpFirstQualEntity;
};

/////////////////////////// FUNCTION PROTYPES //////////////////////////////
// Include prototype file
#include "KZHSQLBP.H"

/////////////////////////////////////////////////////////////////////////////

  KZHSQLBA_DBH_MsgProc

  The entry point for the db-handler.  All calls go through here.

   lpWorkPtr is void pointer that points a pointer. It may be used by the
   Database Handler to store information exclusively for the current object
   operation.  It is always intialized to 0:
      lpWorkPtr = address of long pointer. lpWorkPtr should not be changed.
      *lpWorkPtr = points to user defined data. Initialized to 0.

/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
KZHSQLBA_DBH_MsgProc( LPTASK       hTask,
                      zUSHORT      Message,      // Message indicating function, etc.
                      int          Indicators,   // Indicators for function.
                      LPVIEWOD     lpViewOD,     // Target/Source object defintion.
                      LPVIEWENTITY lpViewEntity, // Target/Source entity.
                      zVIEW        lpView,       // Target/Source view.
                      zVIEW        lpQualView,   // Qualification view.
                      zPVOID       lpWorkPtr )   // Work pointer.
{
   LPCONNECTION lpConnection;
   zSHORT       nRC;

   // Execute a function according to the message passed.
   if ( Message & DBH_Entity )
   {
      lpConnection = *((LPCONNECTION *) lpWorkPtr);

      if ( Message & DBH_Load )
         nRC = Load( lpViewEntity, lpView, lpViewOD, lpQualView, lpConnection,
                     Indicators );
      else
      if ( Message & DBH_Insert )
         nRC = Insert( lpViewEntity, lpView, lpViewOD, FALSE, lpConnection );
      else
      if ( Message & DBH_InsertRel )
         nRC = InsertRel( lpViewEntity, lpView, lpViewOD, lpConnection );
      else
      if ( Message & DBH_Delete )
         nRC = Delete( lpViewEntity, lpView, lpViewOD, lpConnection );
      else
      if ( Message & DBH_DeleteRel )
         nRC = DeleteRel( lpViewEntity, lpView, lpViewOD, lpConnection );
      else
      if ( Message & DBH_Update )
         nRC = Update( lpViewEntity, lpView, lpViewOD, FALSE, lpConnection );
      else
      if ( Message & DBH_UpdateSeq )
         nRC = Update( lpViewEntity, lpView, lpViewOD, TRUE, lpConnection );

      if ( nRC == zCALL_ERROR )
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

         if ( lpViewEntity->lpParent )
         {
            TraceLineS( "PARENT:", "" );
            DisplayEntityInstance( lpView, lpViewEntity->lpParent->lpName );
            TraceLineS( "", "" );
            TraceLineS( "CHILD:", "" );
         }

         DisplayEntityInstance( lpView, lpViewEntity->lpName );

         if ( lpQualView )
         {
            TraceLineS( "Qualification View:", "" );
            DisplayObjectInstance( lpQualView, 0, 0 );
         }
      }
   }
   else
   if ( Message & DBH_Object )
   {
      if ( Message & DBH_TranBegin )
      {
         // Allocate a buffer to contain the Connection information and
         // initialize.
         *((zPVOID *) lpWorkPtr) = malloc( sizeof( ConnectionRecord ) );
         lpConnection = *((LPCONNECTION *) lpWorkPtr);
         lpConnection->lpFirstQualEntity = 0;

         nRC = BeginTransaction( Indicators, lpViewOD, lpView, lpQualView,
                                 lpWorkPtr );
      }
      else
      if ( Message & DBH_Commit )
      {
         nRC = CommitTransaction( Indicators, lpConnection );

         // Free the connection area.
         free( *((zPVOID *) lpWorkPtr) );
      }
      else
      if ( Message & DBH_Rollback )
      {
         nRC = RollbackTransaction( Indicators, lpConnection );

         // Free the connection area.
         free( *((zPVOID *) lpWorkPtr) );
      }
   }
   else
   if ( Message & DBH_Init )
      nRC = InitDatabase( lpView );
   else
   if ( Message & ( DBH_Term | DBH_TaskCloseAll ) )
      nRC = CloseDatabase( lpView, Indicators );

   return( nRC );
} // KZHSQLBA_DBH_MsgProc

//=========================================================================
//       ENTITY FUNCTIONS
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      InitDatabase

      This function is called the first time any object is loaded or saved
      for the current task.  It allows the db-handler to initialize the
      database.  If no initialization needs to be done, this routine
      can be a no-operation.

      NOTE: This function is called for every task.  This routine must be
      able to handle being called more than once.

/////////////////////////////////////////////////////////////////////////////
zSHORT
InitDatabase( zVIEW lpView )
{
   zVIEW        vDbhWork;
   zCHAR        szStr[ 2 ];
   zSHORT       nRC;

   // Retrieve the Zeidon DB-Handler work view.  This view allows us to keep
   // track of all the different database types.  The view is named at the
   // system level and is available to all db-handlers.
   // *** Get object definition name.
   nRC = GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME,
                        lpView, zLEVEL_SYSTEM );
   if ( nRC < 0 )
   {
      DBH_Error( lpView, "Can't find DB-Handler work view.", 0, 0 );
      return( zCALL_ERROR );
   }

   // Create a string that contains the database type, to be used in the
   // next section. The following creates a string for SqlBase.
   szStr[ 0 ] = zSQL_SQLBASE;
   szStr[ 1 ] = 0;

   // Check to see if an entity exists in the Zeidon DB-Handler work view
   // with a type matching szStr.
   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      // The entity does not exist, so this is the first time any database
      // calls have been made for the current database -- in this example
      // SqlBase.  Create an entity, set the type, and then make any calls
      // to the database API that initializes the database.
      CreateEntity( vDbhWork, "Type", zPOS_AFTER );
      SetAttributeFromString( vDbhWork, "Type", "Type", szStr );

      //
      // Insert code here to initialize the database.
      //
   }

   return( 0 );
}  /* InitDatabase */

/////////////////////////////////////////////////////////////////////////////

      CloseDatabase

      This function is called the when a task is closed.  It allows the
      db-handler to close any databases and free memory.

      NOTE: This function is called for every task.  This routine must be
      able to handle being called more than once.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CloseDatabase( zVIEW lpView, int Indicators )
{
   zVIEW        vDbhWork;
   zCHAR        szStr[ 2 ];
   zSHORT       nRC;

   // Retrieve the Zeidon DB-Handler work view.  This view allows us to keep
   // track of all the different database types.  The view is named at the
   // system level and is available to all db-handlers.
   nRC = GetViewByName( &vDbhWork, zDBHANDLER_WORK_VIEW_NAME,
                        lpView, zLEVEL_SYSTEM );
   if ( nRC < 0 )
   {
      DBH_Error( lpView, "Can't find DB-Handler work view.", 0, 0 );
      return( zCALL_ERROR );
   }

   // Create a string that contains the database type.  The example below
   // creates a string for SqlBase.
   szStr[ 0 ] = zSQL_SQLBASE;
   szStr[ 1 ] = 0;

   // Check to see if an entity exists in the Zeidon DB-Handler work view
   // with a type matching szStr.  Since one should have been created by
   // InitDatabase (see above), it is an error if no entity is found.
   nRC = SetCursorFirstEntityByString( vDbhWork, "Type", "Type", szStr, "" );
   if ( nRC != zCURSOR_SET )
   {
      DBH_Error( vDbhWork, "Internal error -- cannot find SQLBase type in "
                 "db-handler work object.", 0, 0 );
      DropView( vDbhWork );
      return( zCALL_ERROR );
   }

   return( 0 );
}  /* CloseDatabase */

/////////////////////////////////////////////////////////////////////////////

      BeginTransaction

      This function is called at the beginning of every object Activate
      and Commit.  For databases that allow it, the db-handler can
      start transaction processing here to be used for enqueuing and
      potential roll-back.

/////////////////////////////////////////////////////////////////////////////
zSHORT
BeginTransaction( int          Indicators,
                  LPVIEWOD     lpViewOD,
                  zVIEW        lpView,
                  zVIEW        lpQualView,
                  zPVOID       lpWorkPtr )
{
   LPCONNECTION lpConnection = *((LPCONNECTION *) lpWorkPtr);
   zSHORT       nRC;

   // Retrieve the trace level for the object operation.
   lpConnection->nTraceLevel = GetTaskDBHandlerTraceLevel( lpView );

   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "##################  BEGIN TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
      if ( Indicators == ( Indicators & DBH_StoreOI ) )
         TraceLineS( "Storing Object Def ", lpViewOD->lpName );
      else
         TraceLineS( "Activating Object Def ", lpViewOD->lpName );
   }

   // Copy the contents of lpQualView to memory.
   // *** What are we doing here?
   if ( lpQualView )
   {
      nRC = SqlRetrieveQualObject( zSQL_SQLBASE, lpView, lpQualView, lpViewOD,
                                   &lpConnection->lpFirstQualEntity );
      if ( nRC < 0 )
         return( zCALL_ERROR );
   }

   //
   // Insert code to start transaction here.
   //

   return( 0 );
}  /* BeginTransaction */

/////////////////////////////////////////////////////////////////////////////

      CommitTransaction

      This function is called at the end of every object Activate and
      Commit if the operation was successfully completed.
  *** Is this really called for activate??

      For databases that allow it, the db-handler can end transaction
      processing here.

/////////////////////////////////////////////////////////////////////////////
zSHORT
CommitTransaction( int Indicators, LPCONNECTION lpConnection )
{
   zSHORT k;
   zSHORT nRC;

   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "#################  COMMIT TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
   }

   // Free up qual data.
   if ( lpConnection->lpFirstQualEntity )
      SqlFreeQualEntity( &lpConnection->lpFirstQualEntity );

   //
   // Insert code here to commit transaction.
   //

   return( 0 );
}  /* CommitTransaction */

/////////////////////////////////////////////////////////////////////////////

      RollbackTransaction

      This function is called at the end of every object Activate and
      Commit that was NOT successfully completed.

      For databases that allow it, the db-handler can roll back the
      transaction.

/////////////////////////////////////////////////////////////////////////////
zSHORT
RollbackTransaction( int Indicators, LPCONNECTION lpConnection )
{
   zSHORT k;
   zSHORT nRC;

   if ( lpConnection == 0 )
   {
      TraceLineS( "(kzhsqlba) lpConnection is NULL!!!!!!", "" );
      return( 0 );
   }

   if ( lpConnection->nTraceLevel > 0 )
   {
      TraceLineS( "####################################################", "" );
      TraceLineS( "################ ROLLBACK TRANSACTION ##############", "" );
      TraceLineS( "####################################################", "" );
   }

   // Free up qual data.
   if ( lpConnection->lpFirstQualEntity )
      SqlFreeQualEntity( &lpConnection->lpFirstQualEntity );
  // *** What is SqlFreeQualEntity?  Will the Siron DBH use it?

   //
   // Insert code here to rollback transaction.
   //

   return( 0 );
}  /* RollbackTransaction */

//=========================================================================
//       ENTITY FUNCTIONS
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      Load

      Reads entities of type lpViewEntity from the database and puts
      them into the object instance being activated.

      To understand Load, you first need to understand the sequence
      of events during an Activate.  When the Object Engine gets a
      request to Activate an object instance, it calls the DB Handler
      routine multiple times, having the DB Handler read one or more
      entities of the same type during each call.  Overall, the
      object instance is read in modified hierarchical order (MHO).

      The difference between regular hierarchical order (RHO) and
      MHO is as follows.  In RHO, a read of an entity is immediately
      followed by a read of its first child.  In MHO, a read of an
      entity is immediately followed by a read of its next twin under
      its parent.

      Consider the following object definition,

                      A
                      |
                    _____
                    |   |
                    B   D
                    |
                    C

      with the following instance, where C1 and C2 are children
      B1 and C3 and C4 are children of B2.

                      A1
                      |
                  _________
                  |       |
                B1, B2    D1
                |   |
            C1,C2  C3,C4

      In MHO, the first record read is A1, followed by B1 and B2.  Then
      C1 and C2 are read for B1, followed by C3 and C4 for B2.  Lastly,
      D1 is read.  Note that in RHO, C1 and C2 would be read after
      B1 but before B2.

      Getting back to the Load function, it is first called by the
      Object Engine to read all A entities that meet the Activate
      qualification criteria.  Then, for each instance of A (only A1
      in the above example), all the B's for that A are read.  In our
      example above, both B1 and B2 are read in the second call to
      Load.  Then, for each instance of B (B1 and B2 above), all the
      C's for each B are read.  In our example above, C1 and C2 are
      read in the third call to Load, and C3 and C4 are called in the
      fourth call to Load.  Finally, proceeding in hierarchical order,
      all D's for each A are read (only D1 for A1 in the example
      above.  In all in our example, the Load routine was called 5
      times to read 8 entities.

      The important concept to remember is that Load is called once to
      read all the children of one type for a parent instance.  Thus, if the
      parent has an imbedded key for the children of that type, then
      that key is used to access all the children.  In like manner, if a
      parent contained the array of data for the children of that type,
      the children would simply be taken from members in the array.

      The above should fit in well with Siron.  If a relationship is
      implemented via an imbedded key, then reading B1 and B2
      together is Siron's normal way of retrieving multiple records.
      Then for each B (B1 and B2), the group of C children with the
      same imbedded key is read.  If C is an array or an occurs-
      depending-on, it is even easier.

      Note that we are leaving out one capability that Zeidon supports
      for relational databases, which is to allow root qualification
      using search criteria on lower level entities.  However, if Siron
      can support that capability, we can add it back.

      Returns:    1 - Load worked--multiple roots found but only 1 loaded
                      because Indicators = zSINGLE.
                  0 - Load worked--entity(-ies) loaded.
                 -1 - Load worked but couldn't find anything to load.
        zCALL_ERROR - Call error

/////////////////////////////////////////////////////////////////////////////
zSHORT
Load( LPVIEWENTITY lpViewEntity,
      zVIEW        lpView,
      LPVIEWOD     lpViewOD,
      zVIEW        lpQualView,
      LPCONNECTION lpConnection,
      int          Indicators )
{
   LPENTITYINSTANCE lpEntityInstance;
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   LPRELRECORD  lpRelRecord;
   SQLTDAL  lLth;
   SQLTPDL  Lth;
   zPVOID   lpMem;
   zPCHAR   pchTableName;
   zPCHAR   pchPtr;
   zPCHAR   pchBufferPtr[ 500 ];
   zLONG    hMem;
   zLONG    lMemLth;
   zBOOL    bKeysOnly;
   zUSHORT  uLth;
   zSHORT   nColumnCount;
   zSHORT   nRowCount;
   zSHORT   nRC, nRC2;

   nRC = 0;
   bKeysOnly = FALSE;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;
   pchTableName = lpDataRecord->lpRecordName;

   // The following writes out trace information, depending on the
   // Trace Level.
   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "================= Performing load ===================", "" );
      TraceLineS( "  Entity name: ", lpViewEntity->lpName );
      TraceLineS( "  Table name: ", lpDataRecord->lpRecordName );

      if ( lpConnection->nTraceLevel > 2 )
      {
         if ( lpConnection->lpFirstQualEntity )
            TraceLineS( "  Table IS qualified.", "" );
         else
            TraceLineS( "  Table is not qualified.", "" );

         if ( lpViewEntity->lpParent )
            DisplayEntityInstance( lpView, lpViewEntity->lpParent->lpName );
      }
   }

   // AutoLoad:
   // When the parent has the imbedded keys for the child and the
   // only attributes in the child are those imbedded keys, then
   // logic can be inserted here to get those attributes from the
   // parent and not access the child.  This can improve performance
   // in some cases.
   // I recommend that we do not implement it initially in the Siron
   // DB Handler, but add it at a later date.

   nRC = SqlBuildSelect( zSQL_SQLBASE, lpView, lpViewOD, lpViewEntity,
                         lpConnection->lpFirstQualEntity,
                         lpConnection->szSqlCmd );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   // If nRC == 1, then some of the foreign keys are null, which means that
   // there are no children.
   if ( nRC == 1 )
   {
      if ( lpConnection->nTraceLevel > 0 )
      {
         TraceLineS( "Foreign key is null, so following command was not ",
                     "executed." );
         SqlDisplayCommand( lpConnection->szSqlCmd, 0 );
         TraceLineI( "Number of rows selected = ", (zLONG) 0 );
      }
      return( 0 );
   }

   // What is sqlcom?????
   nRC = sqlcom( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
   DisplayCommand( "sqlcom", nRC, lpConnection );
   if ( nRC )
      return( zCALL_ERROR );

   // Set the select buffer for each data field.
   // Why????  What are we doing here?
   nColumnCount = 1;
   lpDataField  = lpDataRecord->lpFirstDataField;
   pchPtr       = lpConnection->szGlobalTableBuffer;
   for ( ; lpDataField; lpDataField = lpDataField->lpNextDataField )
   {
      // If we only want the keys and the attrib/datafield is not a key, cont.
      if ( bKeysOnly && lpDataField->lpViewAttrib->bKey == FALSE )
         continue;

      // If column is not stored as a long varchar then set up buffer.
      if ( lpDataField->cFldType != zTYPE_BLOB && lpDataField->uFldLth <= 254 )
      {
         nRC = sqldes( lpConnection->hSqlCsr, (SQLTSLC) nColumnCount, 0,
                       (SQLTDDL PTR) &Lth, 0, 0, 0, 0 );
         SQL_RC( "sqldes", nRC, "Column = ", lpDataField->lpFldName, 3 );
         if ( nRC )
            return( zCALL_ERROR );

         nRC = sqlssb( lpConnection->hSqlCsr, (SQLTSLC) nColumnCount, SQLPSTR,
                       (SQLTDAP) pchPtr, (SQLTPDL) Lth + (SQLTPDL) 1, 0, 0, 0 );
         SQL_RC( "sqlssb", nRC, 0, 0, 3 );
         if ( nRC )
            return( zCALL_ERROR );

         pchBufferPtr[ nColumnCount ] = pchPtr;
         pchPtr = pchPtr + Lth + 1;
      }

      nColumnCount++;
   } // for ( ... )...

   // Execute the sql command.
   nRC = sqlexe( lpConnection->hSqlCsr );
   SQL_RC( "sqlexe", nRC, 0, 0, 2 );
   if ( nRC )
      return( zCALL_ERROR );

   // Fetch first row from the table.
   nRowCount = 0;
   //zmemset( lpConnection->szGlobalTableBuffer, 0,
   //         sizeof( szGlobalTableBuffer ) );
   nRC = sqlfet( lpConnection->hSqlCsr );
   SQL_RC( "sqlfet", nRC, 0, 0, 2 );
   if ( nRC < 0 )
      return( zCALL_ERROR );

   hMem    = 0;
   lMemLth = 0;
   lpMem   = 0;

   // If a row was fetched, copy the table data into a newly created entity.
   if ( nRC == 0 )
   {
      // For each row, create an entity instance and copy the attribute
      // values from the table to the entity.
      do
      {
         nRowCount++;
         LoadEntity( lpView, lpViewEntity->lpName, zPOS_AFTER, 0 );

         lpDataField = lpDataRecord->lpFirstDataField;
         nColumnCount = 1;
         for ( ; lpDataField; lpDataField = lpDataField->lpNextDataField )
         {
            // If we only want the keys and the attrib/datafield is not a key,
            // continue.
            if ( bKeysOnly && lpDataField->lpViewAttrib->bKey == FALSE )
               continue;

            // If attribute is a blob or long string, then value the must be
            // retrieved from a long varchar.
            if ( lpDataField->cFldType == zTYPE_BLOB ||
                 lpDataField->uFldLth > 254 )
            {
               // Get length from long varchar.
               nRC = sqlrlo( lpConnection->hSqlCsr, (SQLTSLC) nColumnCount,
                             (SQLTDAP) &uLth, (SQLTDAL) sizeof( uLth ),
                             (SQLTDAL PTR) &lLth );
               SQL_RC( "sqlrlo", nRC, "Col name = ",
                       lpDataField->lpFldName, 3 );
               if ( nRC < 0 )
               {
                  nRC = zCALL_ERROR;
                  break;
               }

               // If the length is 0, then there is no more to be read.
               if ( uLth > 0 )
               {
                  // If memory needs to be allocated, then allocate it.
                  if ( hMem == 0 || lMemLth <= (zLONG) uLth )
                  {
                     if ( hMem != 0 )
                        SysFreeMemory( hMem );

                     // Make sure that the length of the buffer is a least
                     // 1000 and always at least 1 greater then the length of
                     // the column value.
                     lMemLth = (uLth < 1000) ? 1001 : (zLONG) uLth + 1;
                     hMem = SysAllocMemory( (zPVOID) &lpMem, lMemLth, 0,
                                            zCOREMEM_ALLOC, 0 );
                     if ( lpMem == 0 )
                        return( zCALL_ERROR );
                  }

                  zmemset( lpMem, 0, (zUSHORT) lMemLth );

                  nRC = sqlrlo( lpConnection->hSqlCsr, (SQLTSLC) nColumnCount,
                                (SQLTDAP) lpMem, (SQLTDAL) lMemLth,
                                (SQLTDAL PTR) &lLth );
                  SQL_RC( "sqlrlo", nRC, 0, 0, 3 );

                  if ( nRC < 0 )
                  {
                     nRC = zCALL_ERROR;
                     break;
                  }
                  else
                  if ( lLth != (SQLTDAL) uLth )
                  {
                     TraceLineI( "Length returned = ", lLth );
                     TraceLineI( "Length should be ", (zLONG) uLth );
                     nRC = zCALL_ERROR;
                     break;
                  }

                  StoreValueInRecord( lpView, lpViewEntity,
                                      lpDataField->lpViewAttrib,
                                      lpMem, uLth );
               } // if ( uLth > 0 )...

               nRC2 = sqlelo( lpConnection->hSqlCsr );
               SQL_RC( "sqlelo", nRC2, 0, 0, 3 );
            } // if ( lpDataField->cFldType == zTYPE_BLOB || ... )...
            else
            {
               zPCHAR lp1, lp2;

               // If the attribute is a DateTime field, then the string must be
               // "unformatted".
               if ( lpDataField->cFldType == zTYPE_DATETIME )
               {
                  // Go through each of the characters in the string and copy
                  // only the characters that are digits.
                  lp1 = lp2 = pchBufferPtr[ nColumnCount ];
                  while ( lp1[ 0 ] )
                  {
                     if ( lp1[ 0 ] != '0' && lp1[ 0 ] != '1' && lp1[ 0 ] != '2' &&
                          lp1[ 0 ] != '3' && lp1[ 0 ] != '4' && lp1[ 0 ] != '5' &&
                          lp1[ 0 ] != '6' && lp1[ 0 ] != '7' && lp1[ 0 ] != '8' &&
                          lp1[ 0 ] != '9')
                     {
                        lp1++;
                     }
                     else
                        lp2[ 0 ]++ = lp1[ 0 ]++;
                  }

                  lp2[ 0 ] = 0;

                  // Make sure that length is no greater than 18
                  if ( zstrlen( pchBufferPtr[ nColumnCount ] ) > 17 )
                     pchBufferPtr[ nColumnCount ][ 17 ] = 0;
               }

               StoreStringInRecord( lpView, lpViewEntity,
                                    lpDataField->lpViewAttrib,
                                    pchBufferPtr[ nColumnCount ] );
            } // if ( lpDataField->cFldType == zTYPE_BLOB || ... )...else...

            nColumnCount++;
         } // for ( ... )...

         if ( nRC < 0 )
            break;

         // Fetch next row from the table.
         nRC = sqlfet( lpConnection->hSqlCsr );
         SQL_RC( "sqlfet", nRC, 0, 0, 3 );
         if ( nRC > 100 )
         {
            nRC = zCALL_ERROR;
            break;
         }

         // If the current view entity does not have a parent (i.e. it is the
         // root) and the zSINGLE has been specified, then stop fetching rows
         // and return 1.
         if ( lpViewEntity->lpParent == 0 && !( Indicators & zMULTIPLE ) )
         {
            nRC = 1;   // Indicate that multiple roots were found.
            break;
         }

         // If the current row count is a greater than the root limit for the
         // current OD, then set nRC to 1 to break fetch loop.
         if ( nRC == 0 && lpViewEntity->lpParent == 0 &&
              lpViewOD->nMultiRootLimit )
         {
            if ( nRowCount > lpViewOD->nMultiRootLimit  )
               nRC = 1;   // Indicate that more roots were found.
         }

      } while ( nRC == 0 );
   } // if ( nRC == 0 )...
   else
      nRC = -1;   // Indicate that no rows were fetched.

   // If hMem is not zero, then free the memory allocated.
   if ( hMem )
      SysFreeMemory( hMem );

   if ( lpConnection->nTraceLevel > 0 )
      TraceLineI( "Number of rows retrieved = ", (zLONG) nRowCount );

   return( nRC );
}  /* Load */

/////////////////////////////////////////////////////////////////////////////

      Insert

      Inserts a new instance of an entity to the database.  If the
      entity instance is related to one or more parents??? whose
      relationship is maintained through an imbedded key, setting
      the necessary imbedded keys is done here.  If the relationship
      is maintained through a relationship table, then that
      relationship is not created here, but under InsertRel below.

/////////////////////////////////////////////////////////////////////////////
zSHORT
Insert( LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        LPVIEWOD     lpViewOD,
        zSHORT       bAutoSeqOnly,
        LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   zPCHAR szRecordName;
   zLONG  nRecordId;
   zSHORT nRC, nBuildRC;

   lpConnection->DBConnection[ lpConnection->nCurrentDB ].bDBUpdated = TRUE;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;
   nRecordId    = lpDataRecord->lRecordID;
   szRecordName = lpDataRecord->lpRecordName;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Insert ==========", "" );
      TraceLineS( "Inserting entity ", lpViewEntity->lpName );
      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->lpName );
   }

   // Build SQL for insert.  If nBuildRC == 1 after build, then there are some
   // long var chars that need to be bound.
   nBuildRC = SqlBuildInsert( zSQL_SQLBASE, lpView, lpViewOD, lpViewEntity,
                              lpConnection->szSqlCmd );
   if ( nBuildRC == zCALL_ERROR )
      return( zCALL_ERROR );

   nRC = sqlcom( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
   DisplayCommand( "sqlcom", nRC, lpConnection );
   if ( nRC )
      return( zCALL_ERROR );

   // If needed, bind any long varchars.
   if ( nBuildRC == 1 )
   {
      nRC = WriteLongChar( lpView, lpViewEntity, lpConnection );
      if ( nRC < 0 )
         return( zCALL_ERROR );
   }

   // Insert table. If return code is 805, then the row already exists.
   nRC = sqlexe( lpConnection->hSqlCsr );
   if ( nRC == 805 && lpViewEntity->lpParent == 0 )
   {
      nRC = zDUPLICATE_ROOT;
      SQL_RC( "sqlexe", nRC, 0, 0, 2 );
      return( zDUPLICATE_ROOT );
   }
   else
   {
      SQL_RC( "sqlexe", nRC, 0, 0, 2 );
      if ( nRC )
         return( zCALL_ERROR );
   }

   // Now check to see if a row needs to be added to a corresponance table.
   if ( lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      nRC = SqlBuildInsertCorrTable( zSQL_SQLBASE, lpView, lpViewOD,
                                     lpViewEntity, lpConnection->szSqlCmd );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
      DisplayCommand( "sqlcex", nRC, lpConnection );
      if ( nRC )
         return( zCALL_ERROR );
   }

   return( 0 );
}  /* Insert */

/////////////////////////////////////////////////////////////////////////////

      InsertRel

      Inserts a relationship table to maintain a relationship
      between two entities.  If Siron does not support relationship
      tables, then this code would not be used.

/////////////////////////////////////////////////////////////////////////////
zSHORT
InsertRel( LPVIEWENTITY lpViewEntity,
           zVIEW        lpView,
           LPVIEWOD     lpViewOD,
           LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   zPCHAR       pchRecordName;
   int          nRC;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;
   pchRecordName = lpDataRecord->lpRecordName;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Insert Rel ==========", "" );
      TraceLineS( "Inserting relationship for ", lpDataRecord->lpRecordName );

      if ( lpConnection->nTraceLevel > 2 )
      {
         TraceLineS( "PARENT:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->lpParent->lpName );

         TraceLineS( "CHILD:", "" );
         DisplayEntityInstance( lpView, lpViewEntity->lpName );
      }
   }

   // The only thing that needs to be done is to insert the correspondence
   // table if the relationship is many-to-many.
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      lpConnection->DBConnection[ lpConnection->nCurrentDB ].bDBUpdated = TRUE;

      nRC = SqlBuildInsertCorrTable( zSQL_SQLBASE, lpView, lpViewOD,
                                     lpViewEntity, lpConnection->szSqlCmd );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
      DisplayCommand( "sqlcex", nRC, lpConnection );
      if ( nRC )
         return( zCALL_ERROR );
   }

   return( 0 );
}  /* InsertRel */

/////////////////////////////////////////////////////////////////////////////

      Delete

      Deletes an instance of an entity.  If a relationship between
      this entity and another entity is maintained by an imbedded key
      in the other entity, then that imbedded key must be handled in
      this routine.  If the relationship is maintained by a
      relationship table, then that relationship is deleted under
      DeleteRel below.

/////////////////////////////////////////////////////////////////////////////
zSHORT
Delete( LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        LPVIEWOD     lpViewOD,
        LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   int  nRC;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   lpConnection->DBConnection[ lpConnection->nCurrentDB ].bDBUpdated = TRUE;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Delete ==========", "" );
      TraceLineS( "Deleting record ", lpDataRecord->lpRecordName );
      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->lpName );
   }

   if ( lpRelRecord && lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      nRC = SqlBuildDeleteCorrTable( zSQL_SQLBASE, lpView, lpViewOD,
                                     lpViewEntity, lpConnection->szSqlCmd );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
      DisplayCommand( "sqlcex", nRC, lpConnection );
      if ( nRC )
         return( zCALL_ERROR );
   }

   nRC = SqlBuildDelete( zSQL_SQLBASE, lpView, lpViewOD, lpViewEntity,
                         lpConnection->szSqlCmd );
   if ( nRC == zCALL_ERROR )
      return( zCALL_ERROR );

   nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
   DisplayCommand( "sqlcex", nRC, lpConnection );
   if ( nRC )
      return( zCALL_ERROR );

   return( 0 );
}  /* Delete */

/////////////////////////////////////////////////////////////////////////////

      DeleteRel

      Deletes a relationship table entry to delete an occurence of a
      relationship between two entities.  If Siron does not support
      relationship tables, then this code would not be used.

/////////////////////////////////////////////////////////////////////////////
zSHORT
DeleteRel( LPVIEWENTITY lpViewEntity,
           zVIEW        lpView,
           LPVIEWOD     lpViewOD,
           LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPRELRECORD  lpRelRecord;
   int  nRC;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   lpRelRecord  = lpDataRecord->lpFirstRelRecord;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Delete Rel ==========", "" );
      TraceLineS( "Deleting record ", lpDataRecord->lpRecordName );
      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->lpName );
   }

   // The only thing that needs to be done is to delete the correspondence
   // table if the relationship is many-to-many.
   if ( lpRelRecord->cOwnerMember == zDBH_MANY_TO_MANY )
   {
      lpConnection->DBConnection[ lpConnection->nCurrentDB ].bDBUpdated = TRUE;

      nRC = SqlBuildDeleteCorrTable( zSQL_SQLBASE, lpView, lpViewOD,
                                     lpViewEntity, lpConnection->szSqlCmd );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
      DisplayCommand( "sqlcex", nRC, lpConnection );
      if ( nRC )
         return( zCALL_ERROR );
   }

   return( 0 );
}  /* DeleteRel */

/////////////////////////////////////////////////////////////////////////////

      Update

      Updates data in an entity.  No consideration need be made
      concerning imbedded keys as only main attribute data is
      updated here.

/////////////////////////////////////////////////////////////////////////////
zSHORT
Update( LPVIEWENTITY lpViewEntity,
        zVIEW        lpView,
        LPVIEWOD     lpViewOD,
        zSHORT       bAutoSeqOnly,
        LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   zSHORT  nRC, nBuildRC;

// The following is an example of how to retrieve the lpEntityInstance for
// the current entity in the view.  The entity definition is determined by
// lpViewEntity.
#if 0
   {
      LPENTITYINSTANCE  lpEntityInstance;
      LPVIEWCSR         lpViewCsr;
      LPVIEWENTITYCSR   lpViewEntityCsr;
      zBOOL             bFlag;

      lpViewCsr = lpView->lpViewCsr;
      lpViewEntityCsr = lpViewCsr->lpRootViewEntityCsr;
      while ( lpViewEntityCsr &&
              lpViewEntityCsr->lpViewEntity != lpViewEntity )
      {
         lpViewEntityCsr = lpViewEntityCsr->lpNextHier;
      }

      if ( lpViewEntityCsr )
      {
         lpEntityInstance = lpViewEntityCsr->lpEntityInstance;
         TraceLineS( "DGC: Found lpEntityInstance", "" );

         bFlag = lpEntityInstance->u.nInd.bDBHProcessed;
      }
      else
         TraceLineS( "DGC: Couldn't find lpEntityInstance", "" );
   }
#endif

   lpConnection->DBConnection[ lpConnection->nCurrentDB ].bDBUpdated = TRUE;

   lpDataRecord = lpViewEntity->lpFirstDataRecord;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "", "" );
      TraceLineS( "========== Update ==========", "" );
      TraceLineS( "Updating record ", lpDataRecord->lpRecordName );
      if ( lpConnection->nTraceLevel > 2 )
         DisplayEntityInstance( lpView, lpViewEntity->lpName );
   }

   nBuildRC = SqlBuildUpdate( zSQL_SQLBASE, lpView, lpViewOD, lpViewEntity,
                              bAutoSeqOnly, lpConnection->szSqlCmd );
   if ( nBuildRC == zCALL_ERROR )
      return( zCALL_ERROR );

   nRC = sqlcom( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
   DisplayCommand( "sqlcom", nRC, lpConnection );
   if ( nRC )
      return( zCALL_ERROR );

   if ( nBuildRC == 1 )
   {
      nRC = WriteLongChar( lpView, lpViewEntity, lpConnection );
      if ( nRC < 0 )
         return( zCALL_ERROR );
   }

   nRC = sqlexe( lpConnection->hSqlCsr );
   SQL_RC( "sqlexe", nRC, 0, 0, 2 );
   if ( nRC )
      return( zCALL_ERROR );

   // If the entity has auto seq attributes, then the corr table might have
   // to be updated.
   if ( lpViewEntity->bAutoSeq )
   {
      nRC = SqlBuildUpdateCorrTable( zSQL_SQLBASE, lpView, lpViewOD,
                                     lpViewEntity, lpConnection->szSqlCmd );
      if ( nRC == zCALL_ERROR )
         return( zCALL_ERROR );

      nRC = sqlcex( lpConnection->hSqlCsr, lpConnection->szSqlCmd, 0 );
      DisplayCommand( "sqlcex", nRC, lpConnection );
      if ( nRC )
         return( zCALL_ERROR );
   }

   return( 0 );
}  /* Update */

//=========================================================================
//       UTILITY FUNCTIONS -- in alphabetical order
//=========================================================================

/////////////////////////////////////////////////////////////////////////////

      DBH_Error

/////////////////////////////////////////////////////////////////////////////
void
DBH_Error( zVIEW lpView, zPCHAR pchErrorMsg, int nRC, zPCHAR pchInfo )
{
   MessageSend( lpView, "KZH0101", "SQLBase DBH Error",
                pchErrorMsg,
                zMSGQ_SYSTEM_ERROR, 0 );
//   OperatorPrompt( lpView, "SQLBase DBH Error", pchErrorMsg, zBUTTONS_OK,
//                   0, 1, zICON_ERROR );

   TraceLineS( "===== DBH_Entry Error =====", "" );
   TraceLineS( "Msg=> ", pchErrorMsg );

   if ( nRC )
      TraceLineI( "nRC=> ", nRC );

   if ( pchInfo )
      TraceLineS( pchInfo, "" );
}  /* DBH_Error */

/////////////////////////////////////////////////////////////////////////////

      DisplayCommand

/////////////////////////////////////////////////////////////////////////////
void
DisplayCommand( zPCHAR       pchRoutine,
                zSHORT       nRC,
                LPCONNECTION lpConnection )
{
   zCHAR   szText[ SQLMERR ];
   SQLTEPO ErrorPos;
   zPCHAR  pchErrorChar;

   if ( lpConnection->nTraceLevel == 0 && nRC < 100 )
      return;

   if ( lpConnection->bMultipleDatabases )
   {
      TraceLineS( "", "" );
      TraceLineS( "Database name = ",
       lpConnection->DBConnection[ lpConnection->nCurrentDB ].szDatabaseName );
   }

   TraceLineS( "", "" );
   if ( nRC > 100 )
   {
      TraceLineS( "===== SQLBase Error =====", "" );
      sqlepo( lpConnection->hSqlCsr, &ErrorPos );
      pchErrorChar = &lpConnection->szSqlCmd[ ErrorPos ];
   }
   else
      pchErrorChar = 0;

   if ( lpConnection->nTraceLevel > 1 )
   {
      TraceLineS( "SQLBase Call : ", pchRoutine );
      SqlDisplayCommand( lpConnection->szSqlCmd, pchErrorChar );
      TraceLineI( "Return Code :", nRC );
   }
   else
      SqlDisplayCommand( lpConnection->szSqlCmd, pchErrorChar );

   if ( nRC )
   {
      sqlerr( nRC, szText );
      TraceLineS( "Error message: ", szText );
   }

   if ( nRC > 100 )
      MessageSend( lpConnection->lpAppView, "KZH0102", "SQLBase DBH Error",
                   szText,
                   zMSGQ_SYSTEM_ERROR, 0 );
//      OperatorPrompt( lpConnection->lpAppView, "SQLBase SQL Error", szText,
//                      zBUTTONS_OK, 0, 1, zICON_ERROR );
}  /* DisplayCommand */

/////////////////////////////////////////////////////////////////////////////

      SQLBase_RC

/////////////////////////////////////////////////////////////////////////////
void
SQLBase_RC( zPCHAR       pchRoutine,
            zSHORT       nRC,
            zPCHAR       pchInfo1,
            zPCHAR       pchInfo2,
            zSHORT       nTrcLvl,
            LPCONNECTION lpConnection )
{
   zCHAR szText[ SQLMERR ];
   zCHAR szMsg[ 200 + SQLMERR ];

   if ( (nRC < 100) &&
        (lpConnection->nTraceLevel < nTrcLvl ||
         lpConnection->nTraceLevel == 0) )
   {
      return;
   }

   TraceLineS( "", "" );
   if ( nRC >= 100 )
      TraceLineS( "===== SQLBase Error =====", "" );

   TraceLineS( "SQLBase Call : ", pchRoutine );

   if ( pchInfo1 )
   {
      if ( pchInfo2 )
         TraceLineS( pchInfo1, pchInfo2 );
      else
         TraceLineS( pchInfo1, "" );
   }

   TraceLineI( "Return Code :", nRC );

   if ( nRC )
   {
      if ( nRC == zDUPLICATE_ROOT )
         strcpy_s( szText, sizeof( szText ), "Attempt to add duplicate root." );
      else
         sqlerr( nRC, szText );
      TraceLineS( "Error message: ", szText );
   }

   if ( nRC >= 100 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "SQL Base Run-time error performing " );
      strcat_s( szMsg, sizeof( szMsg ), pchRoutine );
      strcat_s( szMsg, sizeof( szMsg ), ":\n" );
      strcat_s( szMsg, sizeof( szMsg ), szText );
      MessageSend( lpConnection->lpAppView, "KZH0103", "SQLBase DBH Error",
                   szMsg,
                   zMSGQ_SYSTEM_ERROR, 0 );
//      OperatorPrompt( lpConnection->lpAppView, "SQLBase Error", szMsg,
//                      zBUTTONS_OK, 0, 1, zICON_ERROR );
   }
}  /* SQLBase_RC */

/////////////////////////////////////////////////////////////////////////////

      WriteLongChar

/////////////////////////////////////////////////////////////////////////////
zSHORT
WriteLongChar( zVIEW        lpView,
               LPVIEWENTITY lpViewEntity,
               LPCONNECTION lpConnection )
{
   LPDATARECORD lpDataRecord;
   LPDATAFIELD  lpDataField;
   zPVOID       lpValue;
   zCHAR        chType;
   zLONG        lLth;
   zUSHORT      uLth;
   zSHORT       nLongVarCount;
   zSHORT       nRC;

   // For each data field, check to see if the attribute is a blob or has
   // a length of 255 or greater.  If either of these are true, then the
   // the attribute needs to be saved as a long varchar.
   lpDataRecord = lpViewEntity->lpFirstDataRecord;
   nLongVarCount = 0;
   lpDataField = lpDataRecord->lpFirstDataField;
   while ( lpDataField )
   {
      if ( lpDataField->cFldType == zTYPE_BLOB ||
            lpDataField->uFldLth > 254 )
      {
         nLongVarCount++;

         // Get pointer to var and get length.
         GetValueFromRecord( lpView, lpViewEntity,
                             lpDataField->lpViewAttrib, (zPVOID) &lpValue,
                             &chType, &uLth );

         nRC = sqlbln( lpConnection->hSqlCsr, (SQLTBNN) nLongVarCount );
         SQL_RC( "sqlbln", nRC, "Column = ", lpDataField->lpFldName, 3 );
         if ( nRC )
            return( zCALL_ERROR );

         // Write length.
         nRC = sqlwlo( lpConnection->hSqlCsr, (SQLTDAP) &uLth,
                       (SQLTDAL) sizeof( uLth ) );
         SQL_RC( "sqlwlo", nRC, "Writing length", 0, 3 );
         if ( nRC )
            return( zCALL_ERROR );

         if ( uLth > 0 )
         {
            // Write attribute value.
            lLth = (zLONG) uLth;
            nRC = sqlwlo( lpConnection->hSqlCsr, (SQLTDAP) lpValue,
                          (SQLTDAL) lLth );
            SQL_RC( "sqlwlo", nRC, "Writing long varchar", 0, 3 );
            if ( nRC )
               return( zCALL_ERROR );
         }

         // End the long varchar operation.
         nRC = sqlelo( lpConnection->hSqlCsr );
         SQL_RC( "sqlelo", nRC, 0, 0, 3 );
         if ( nRC )
            return( zCALL_ERROR );

      } // if ( lpDataField->cFldType == zTYPE_BLOB || ... )...

      lpDataField = lpDataField->lpNextDataField;
   } // while ( lpDataField )...

   return( 0 );
}  /* WriteLongChar */
