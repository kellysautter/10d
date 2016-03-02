// *******************************************************************
//
// DESCRIPTION:  This is the generic enqueueing source file for the
//               cognos sample application.
// OPERATIONS:   Enqueue/Dequeue operations for the cognos app
// *******************************************************************
//  AUTHOR:  Gig Kirk
//  DATE:    10/8/93
//  API:     NONE
//  ENVIRONMENT: ANY
// *******************************************************************
/*
CHANGE LOG

1998.08.20  DGC
   Testing Checkin/Checkout.

*/
#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <KZDLGOAA.H>

#include <string.h>

zPCHAR  szlAttributeName   = "AttributeName";
zPCHAR  szlCGENQUEU        = "CGENQUEU";
zPCHAR  szlEnqueue         = "Enqueue";
zPCHAR  szlEntityName      = "EntityName";
zPCHAR  szlEntitySpec      = "EntitySpec";
zPCHAR  szlER_Token        = "ER_Token";
zPCHAR  szlInstanceKey     = "InstanceKey";
zPCHAR  szlLevel           = "Level";
zPCHAR  szlObject          = "Object";
zPCHAR  szlOper            = "Oper";
zPCHAR  szlQualAttrib      = "QualAttrib";
zPCHAR  szlTimestamp       = "Timestamp";
zPCHAR  szlType            = "Type";
zPCHAR  szlUser            = "User";
zPCHAR  szlValue           = "Value";

zSHORT
fnBuildQualForEnqueue( zVIEW vEnqueue,
                       zPVIEW pvQual );
zSHORT
fnBuildQualForEnqueueDelete( zVIEW vEnqueue,
                             zPVIEW pvQual );

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION:   EnqueueInstance
//
//   PARAMETERS:  zVIEW  vInstance - View to the instance to be enqueued
//                zPCHAR szEntityNames - A string containing 1 or
//                                  more entity names on which to enqueue
//                                  the instance. The first entity name
//                                  MUST BE the root of the object and
//                                  the remaining entity names MUST be
//                                  separated by the plus '+' character.
//
//   RETURN CODES:  0 - Enqueue successful
//                  1 - Enqueue unsuccessful, the instance is already
//                      Enqueued
//                 zCALL_ERROR - Enqueue unsuccessful, error in processing
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
EnqueueInstance( zVIEW vInstance, zPCHAR szEntityNames )
{
   zVIEW  vEnqueue;
   zVIEW  vEnqueueHolder;
   zVIEW  vQual;
   zVIEW  vWkInstance;
   zPCHAR szEntityNamePtr;
   zCHAR  szCurrentEntity[ 34 ];
   zCHAR  szLevel[ 2 ];
   zSHORT nEntity, nEnqueue, nIdx, nRC, nRetries;
   zLONG  lER_Token;
   zCHAR  szTimestamp[ 6 ];
   zCHAR  szObjectName[ 34 ];
   zCHAR  szUserID[ 34 ];
   zCHAR  szPassword[ 34 ];
   zCHAR  szKey[ 64 ];
   zCHAR  szMsg[ 256 ];

   // We put the enqueue logic in a for loop so we can restart the
   // enqueue if the enqueue fails and we cannot find the owner of
   // the enqueue.
   nRetries = 4;
   if ( MiGetObjectNameForView( szObjectName, vInstance ) < 0 )
      return( zCALL_ERROR );

   for ( ; ; )
   {
      // Set up work variables
      szEntityNamePtr = szEntityNames;
      _fstrcpy( szLevel, "R" );
      nEntity = nEnqueue = 0;

      // Set up a multiple root instance for enqueueing
      if ( ActivateEmptyObjectInstance( &vEnqueue, szlCGENQUEU,
                                        vInstance, zMULTIPLE ) != 0 )
      {
         return( zCALL_ERROR );
      }

      // Create a view on the instance so we do not step on the applications
      // cursor positions.
      CreateViewFromViewForTask( &vWkInstance, vInstance, 0 );

      // Get timestamp and user information for enqueue
      MiGetDateTimeForOI( szTimestamp, vWkInstance );
      SysGetUserID( vInstance, szUserID, zsizeof( szUserID ), szPassword, zsizeof( szPassword ) );
      if ( szUserID[ 0 ] == 0 )
         _fstrcpy( szUserID, "(null)" );

      // Loop for each entity type to be enqueued
      while ( szEntityNamePtr[ 0 ] )
      {
         // Gather the next entity name on which to enqueue
         nIdx = 0;
         while ( szEntityNamePtr[ 0 ] && szEntityNamePtr[ 0 ] != '+' )
         {
            szCurrentEntity[ nIdx++ ] = szEntityNamePtr[ 0 ];
            szEntityNamePtr++;
         }

         szCurrentEntity[ nIdx ] = 0;
         if ( szEntityNamePtr[ 0 ] == '+' )
            szEntityNamePtr++;

         nEntity++;

         // Now that an entity name has been gathered, troll the instance
         // for every occurrence of that entity type and create an
         // enqueue instance for it
         nRC = SetCursorFirstEntity( vWkInstance, szCurrentEntity,
                                     zSCOPE_OI );
         if ( nRC == zCALL_ERROR )
         {
            DropView( vWkInstance );
            DropObjectInstance( vEnqueue );
            return( zCALL_ERROR );
         }

         if ( nRC >= zCURSOR_SET )
         {
            lER_Token = MiGetERTokenForEntity( vWkInstance, szCurrentEntity );
            do
            {
               // Set up an enqueue instance, The ER_Token and the Instance
               // key are the unique keys to the object, a duplicate instance
               // on the database will result in an enqueue failure with the
               // instance in use. The UserID combined with the Timestamp is
               // the non-unique key for deletion of a logical enqueue.

               // The object name and level ( 'R' - root, 'C' - child ) are
               // informational only.

               CreateEntity( vEnqueue, szlEnqueue, zPOS_AFTER );
               SetAttributeFromInteger( vEnqueue,
                           szlEnqueue, szlER_Token, lER_Token );
               MiGetKeyFromInstance( szKey, 61, vWkInstance, szCurrentEntity );
               SetAttributeFromString( vEnqueue, szlEnqueue, szlInstanceKey,
                                       szKey );
               SetAttributeFromVariable( vEnqueue, szlEnqueue, szlTimestamp,
                                       szTimestamp, zTYPE_DATETIME,
                                       6, 0, 0 );
               SetAttributeFromString( vEnqueue, szlEnqueue, szlType,
                                       "U" );
               SetAttributeFromString( vEnqueue, szlEnqueue, szlUser,
                                       szUserID );
               SetAttributeFromString( vEnqueue, szlEnqueue, szlObject,
                                       szObjectName );
               SetAttributeFromString( vEnqueue, szlEnqueue, szlLevel,
                                       szLevel );
               nEnqueue++;
            }  while ( SetCursorNextEntity( vWkInstance, szCurrentEntity,
                                            zSCOPE_OI ) >= zCURSOR_SET );
         }

         // After processing an entity type, all other enqueues are at the
         // child level
         szLevel[ 0 ] = 'C';
      }

      // Now that we're done gathering the enqueue information, we can drop
      // the view to the instance
      DropView( vWkInstance );

      // The enqueue instance has been built, now it is time to insert it
      // into the database for locking the instance.
      nRC = CommitObjectInstance( vEnqueue );
      if ( nRC != zDUPLICATE_ROOT )
      {
         DropObjectInstance( vEnqueue );
         return( nRC );
      }

      // The enqueue failed, now we will determine the owner of the enqueue
      // to see if we are conflicting with ourself or to report the owner
      // to the enqueue requestor
      SetCursorFirstEntity( vEnqueue, szlEnqueue, 0 );
      do
      {
         fnBuildQualForEnqueue( vEnqueue, &vQual );
         nRC = ActivateObjectInstance( &vEnqueueHolder, szlCGENQUEU,
                                       vInstance, vQual, zSINGLE );
         DropObjectInstance( vQual );
         if ( nRC < -1 )
            return( zCALL_ERROR );
         else
         if ( nRC >= 0 )
            break;
         else
            DropObjectInstance( vEnqueueHolder );

      } while ( SetCursorNextEntity( vEnqueue, szlEnqueue, 0 ) >= zCURSOR_SET );

      // If the enqueue holder was not found, then the enqueue may have
      // disappeared while trying to find the holder, retry the enqueue
      // operation.
      nRetries--;
      if ( nRC == -1 )
      {
         if ( nRetries == 0 )
            return( zCALL_ERROR );
      }

      // Report on the enqueue holder
      GetStringFromAttribute( szKey, zsizeof( szKey ), vEnqueueHolder, szlEnqueue, szlUser );
      GetStringFromAttribute( &szKey[ 30 ],
                              vEnqueueHolder, szlEnqueue, szlObject );

      // if the current enqueue holder is the current user for the
      // same object type, then offer to drop the previous enqueue and
      // re-establish the enqueue for the current user
      if ( _fstrcmp( szUserID, szKey ) == 0 &&
           _fstrcmp( szObjectName, &szKey[ 30 ] ) == 0 && nRetries )
      {
         // we will retry this once and once only
         nRetries = 1;
         _fstrcpy( szMsg, "The " );
         _fstrcat( szMsg, szObjectName );
         _fstrcat( szMsg, " is already enqueued by you in this session or"
                          " a previous session. Would you like to release"
                          " the previously held enqueue?" );
         nRC = MessagePrompt( vInstance, "1", szlEnqueue, szMsg, 0,
                              zBUTTONS_YESNO, zRESPONSE_YES,
                              zICON_QUESTION );
         if ( nRC == zRESPONSE_NO )
         {
            DropObjectInstance( vEnqueue );
            DropObjectInstance( vEnqueueHolder );
            return( 1 );
         }

         // The user wants to delete a previous enqueue for the same
         // object type, build the qualification for the delete and remove
         // the prior enqueue.
         fnBuildQualForEnqueueDelete( vEnqueueHolder, &vQual );
         DropObjectInstance( vEnqueueHolder );
         nRC = ActivateObjectInstance( &vEnqueueHolder, szlCGENQUEU,
                                       vInstance, vQual, zMULTIPLE );
         DropObjectInstance( vQual );
         if ( nRC < 0 )
            return( zCALL_ERROR );

         nRC = SetCursorFirstEntity( vEnqueueHolder, szlEnqueue, 0 );
         while ( nRC >= zCURSOR_SET )
            nRC = DeleteEntity( vEnqueueHolder, szlEnqueue, zREPOS_NEXT );

         nRC = CommitObjectInstance( vEnqueueHolder );
         DropObjectInstance( vEnqueueHolder );

         // If the attempt to reuse the enqueue failed on a database error,
         // return that error. Otherwise, return zero ==> the enqueue was
         // reused.
         if ( nRC < 0 )
            return( zCALL_ERROR );
         else
            return( 0 );
      }
      else
      {
         // Report on the owner of the enqueue
         _fstrcpy( szMsg, "The " );
         _fstrcat( szMsg, szObjectName );
         _fstrcat( szMsg, " is currently in use by " );
         _fstrcat( szMsg, szKey );
         if ( _fstrcmp( szObjectName, &szKey[ 30 ] ) != 0 )
         {
            _fstrcat( szMsg, " under the object " );
            _fstrcat( szMsg, &szKey[ 30 ] );
         }
         _fstrcat( szMsg, "." );
         MessagePrompt( vInstance, "1", szlEnqueue, szMsg, 0,
                        zBUTTONS_OK, zRESPONSE_OK, 0 );
         DropObjectInstance( vEnqueue );
         DropObjectInstance( vEnqueueHolder );
         return( 1 );
      }
   }  // End of for loop for enqueueing

   // If we reach here, then the enqueue was in use and failed.
   return( 1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION:   DequeueInstance
//
//   PARAMETERS:  zVIEW  vInstance - View to the instance to be dequeued
//                zSHORT nCheckOnly - 0 to dequeue
//                                  - non-zero to check the instance is
//                                    enqueued only.
//
//   RETURN CODES:  0 - Dequeue successful
//                  1 - Enqueue not found
//                  zCALL_ERROR - Dequeue unsuccessful, error in processing
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
DequeueInstance( zVIEW vInstance, zSHORT nCheckOnly )
{
   zVIEW  vEnqueue;
   zVIEW  vQual;
   zLONG  lActivateFlags;
   zCHAR  szObjectName[ 34 ];
   zCHAR  szUserID[ 34 ];
   zCHAR  szPassword[ 34 ];
   zCHAR  szTimestamp[ 6 ];
   zSHORT nRC;

   if ( MiGetObjectNameForView( szObjectName, vInstance ) < 0 )
      return( zCALL_ERROR );

   // Set up a an enqueue instance for building the qual object
   if ( ActivateEmptyObjectInstance( &vEnqueue, szlCGENQUEU,
                                     vInstance, zSINGLE ) != 0 )
   {
      return( zCALL_ERROR );
   }

   SysGetUserID( vInstance, szUserID, zsizeof( szUserID ), szPassword, zsizeof( szPassword ) );
   if ( szUserID[ 0 ] == 0 )
      strcpy_s( szUserID, zsizeof( szUserID ), "(null)" );

   MiGetDateTimeForOI( szTimestamp, vInstance );
   if ( nCheckOnly )
      lActivateFlags = zSINGLE;
   else
      lActivateFlags = zMULTIPLE;

   // Build a partial enqueue instance for passing to the qualification
   // build function fnBuildQualForEnqueueDelete.
   CreateEntity( vEnqueue, szlEnqueue, zPOS_FIRST );
   SetAttributeFromVariable( vEnqueue, szlEnqueue, szlTimestamp,
                           szTimestamp, zTYPE_DATETIME,
                           6, 0, 0 );
   SetAttributeFromString( vEnqueue, szlEnqueue, szlUser,
                           szUserID );
   fnBuildQualForEnqueueDelete( vEnqueue, &vQual );
   DropObjectInstance( vEnqueue );
   nRC = ActivateObjectInstance( &vEnqueue, szlCGENQUEU,
                                 vInstance, vQual, lActivateFlags );
   DropObjectInstance( vQual );
   if ( nRC < -1 )
      return( zCALL_ERROR );
   else
   if ( nRC == -1 )
   {
      DropObjectInstance( vEnqueue );
      return( 1 );
   }

   // The enqueue was found, if it is a check only, drop the instance
   // and return 0, otherwise, delete the enqueue.
   if ( nCheckOnly )
   {
      DropObjectInstance( vEnqueue );
      return( 0 );
   }

   nRC = SetCursorFirstEntity( vEnqueue, szlEnqueue, 0 );
   while ( nRC >= zCURSOR_SET )
      nRC = DeleteEntity( vEnqueue, szlEnqueue, zREPOS_NEXT );

   nRC = CommitObjectInstance( vEnqueue );
   DropObjectInstance( vEnqueue );
   if ( nRC < 0 )
      return( zCALL_ERROR );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnBuildQualForEnqueue
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnBuildQualForEnqueue( zVIEW vEnqueue,
                       zPVIEW pvQual )
{
   SfActivateSysEmptyOI( pvQual, "KZDBHQUA", vEnqueue, zMULTIPLE );
   CreateEntity( *pvQual, szlEntitySpec, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlEntitySpec, szlEntityName,
                           szlEnqueue );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlEntityName,
                           szlEnqueue );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlAttributeName,
                           szlER_Token );
   SetAttributeFromAttribute( *pvQual, szlQualAttrib, szlValue,
                              vEnqueue, szlEnqueue, szlER_Token );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "=" );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "AND" );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlEntityName,
                           szlEnqueue );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlAttributeName,
                           szlInstanceKey );
   SetAttributeFromAttribute( *pvQual, szlQualAttrib, szlValue,
                              vEnqueue, szlEnqueue, szlInstanceKey );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "=" );
}

/////////////////////////////////////////////////////////////////////////////
//
//  FUNCTION: fnBuildQualForEnqueueDelete
//
/////////////////////////////////////////////////////////////////////////////
zSHORT
fnBuildQualForEnqueueDelete( zVIEW vEnqueue,
                             zPVIEW pvQual )
{
   SfActivateSysEmptyOI( pvQual, "KZDBHQUA", vEnqueue, zMULTIPLE );
   CreateEntity( *pvQual, szlEntitySpec, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlEntitySpec, szlEntityName,
                           szlEnqueue );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlEntityName,
                           szlEnqueue );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlAttributeName,
                           szlUser );
   SetAttributeFromAttribute( *pvQual, szlQualAttrib, szlValue,
                              vEnqueue, szlEnqueue, szlUser );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "=" );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "AND" );
   CreateEntity( *pvQual, szlQualAttrib, zPOS_AFTER );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlEntityName,
                           szlEnqueue );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlAttributeName,
                           szlTimestamp );
   SetAttributeFromAttribute( *pvQual, szlQualAttrib, szlValue,
                              vEnqueue, szlEnqueue, szlTimestamp );
   SetAttributeFromString( *pvQual, szlQualAttrib, szlOper, "=" );
}
