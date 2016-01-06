//
// MODULE NAME:  KZOEXMLA  -  Object services XML Handling Operations
// DESCRIPTION:  This is the source file which contains Object services
//               operations for dealing with XML.
// **********************************************************************
// * Copyright (c) 1993-2010 QuinSoft Corporation. All rights reserved. *
// * Confidential and Proprietary material subject to license -         *
// * do not reproduce or disclose. This material is an unpublished      *
// * work and is considered a trade secret belonging to the             *
// * copyright holder.                                                  *
// **********************************************************************
//  AUTHOR:  DG Christensen
//  DATE:    1999.12.13
//  API:     expat XML parser (currently Windows only)
//  ENVIRONMENT: currently Windows only but should be ALL
//

/*
CHANGE LOG
2003.08.30  DGC
   Added a check--if writing an OI (or subobject) to XML then if there are
   multiple roots add a <zOI> tag.

2003.06.27  DonC Hack
   Modified fnConvertEntity, etc. for generating special KZXMLDBO object.

2002.02.01  DGC
   Added tracing options.

2001.12.06  DGC
   Added ActivateOI_FromXML_File.

2001.04.13  PAS
   Added code to allow honor new flag zXML_KEYATTRIBSASIDENT.  This allows
   XML of the form <EEEEE id="xxxx"> where id is a key attribute of entity
   EEEEE.  NOTE: this is only supported currently for writing an XML file.

2001.04.06  DGC
   Added code for entity keys.

2001.03.28  DGC
   Made change requested by Doug.  All entities/attributes are flagged as 'updated'
   after an OI has been loaded from XML unless the incremental flags are set.

2001.02.21  DGC
   Added code to allow sending of changed data only.

2001.02.01  RS
   Add encoding iso-8859-1.

2000.11.07  DGC  Z10
   Removed "level" warning message.

2000.07.24  DGC  Z10
   Made change to write attribs with derived functions IF they are persistent.

2000.07.17  DGC
   Made fix to level requested by TMV.  Some debugging code still needs to be removed.

*/

#include "kzoephdr.h"
#include "expat\xmlparse.h"  // expat XML parser routines.

// Use the following to send XML input text to trace.
//#define TRACE_XML

#define ATTRIB_FLAGS_ACTIVATED_IDX  0
#define ATTRIB_FLAGS_UPDATED_IDX    1

#define ENTITY_FLAGS_HANGING_IDX    0
#define ENTITY_FLAGS_CREATED_IDX    1
#define ENTITY_FLAGS_UPDATED_IDX    2
#define ENTITY_FLAGS_DELETED_IDX    3
#define ENTITY_FLAGS_EXCLUDED_IDX   4
#define ENTITY_FLAGS_INCLUDED_IDX   5
#define ENTITY_FLAGS_HIDDEN_IDX     6
#define ENTITY_FLAGS_TEMPORAL_IDX   7

typedef struct EntityInfoStruct
{
   unsigned int bFirstUnderAbChild : 1;
} EntityInfoRecord;

typedef struct ParserInfoStruct
{
   zSHORT            nDepth;
   zVIEW             vSubtask;
   zVIEW             vOI;
   LPVIEWOD          lpViewOD;
   LPENTITYINSTANCE  lpPrevEntityInstance;
   LPENTITYINSTANCE  lpAbstractChildEI;
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpAbstractChild;
   LPVIEWATTRIB      lpViewAttrib;
   zULONG            nIndicators[ 100 ];

   // A stack containing the entity instance pointers for the current
   // entity and it's parents.
   LPENTITYINSTANCE lpEntityInstance[ 100 ];

   zPCHAR            pchAttributeBuffer;
   int               nBufferPhysicalSize;
   int               nBufferCurrentUsed;
   AttribFlagsRecord AttribFlags;

   EntityInfoRecord  EntityInfo[ 1000 ];

   zBOOL             bFatalError;
   zBOOL             bZeidonXML;
   zBOOL             bIncreFlags;
} ParserInfoRecord, * LPPARSERINFO;

typedef struct WriteInfoStruct
{
   zVIEW            vOI;
   zVIEW            vTemp;
   LPVIEWCSR        lpViewCsr;
   zLONG            lControl;
   LPENTITYINSTANCE lpSubobjectRoot;
   zCHAR            szBuffer[ 10000 ];

   // A stack that will contain the entity names of the current entity and
   // it's parents.
   zPCHAR           pchEntityName[ 100 ];

   // Following is the callback pointer for writing the XML data.
   LPFN_XMLWRITE    lpfnWrite;
   zPVOID           lpPtr;

} WriteInfoRecord, * LPWRITEINFO;

// Operation Prototypes
zSHORT LOCALOPER
fnConvertEntity( zVIEW            lpTaskView,
                 LPWRITEINFO      lpWriteInfo,
                 LPENTITYINSTANCE lpEntityInstance );

// Returns the number of "special" characters a string contains.  Special
// chars are XML control chars or non ASCII chars.
zLONG LOCALOPER
fnContainsSpecialChar( zPCHAR pchAttribValue )
{
   zPCHAR pch;
   zLONG  lRC = 0;

   for ( pch = pchAttribValue; *pch; pch++ )
   {
      switch ( *pch )
      {
         case '&':
         case '<':
         case '>':
         case '\'':
         case '\"':
            lRC++;
            break;

         case '\n':
         case '\r':
         case '\t':
            // These chars are alright so just break.
            break;

         default:
            if ( *pch < 32 || *pch > 127 )
               lRC++;

            break;

      } // switch ( *pchAttribValue )...
   }

   return( lRC );
}

zSHORT LOCALOPER
fnAppendKeyAttribs( LPWRITEINFO  lpWriteInfo,
                    LPVIEWENTITY lpViewEntity )
{
   LPVIEWATTRIB lpViewAttrib;
   zULONG       ulLth;
   zPCHAR       pchTempValueBuffer = 0;
   zULONG       lTempBufferLth = 0;
   zLONG        hTempValueBuffer;
   zSHORT       nReturn = zCALL_ERROR;

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      zCHAR         szAttribIdent[ 500 ];
      zPCHAR        pchAttribValue;
      // LPATTRIBFLAGS lpAttribFlags;

      if ( lpViewAttrib->szDerivedOper[ 0 ] &&
           lpViewAttrib->bPersist == FALSE )
      {
         continue;
      }

      // Skip non-key attributes here.  Will be written as attribute later.
      if ( lpViewAttrib->bKey == FALSE )
         continue;

      if ( lpViewAttrib->cType == zTYPE_BLOB )
      {
         strcpy_s( szAttribIdent, sizeof( szAttribIdent ), "KEY WAS BLOB - NOT SUPPORTED" );
      }
      else
      if ( lpViewAttrib->cType == zTYPE_STRING )
      {
         zCHAR  chType;

         // Get ptr to attr value.
         GetValueFromRecord( lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib,
                             &pchAttribValue, &chType, &ulLth );

         // Check to see if the attribute is NULL.  The we check ulLth = 1 for
         // strings because the length contains the null-terminator.
         if ( ulLth <= 1 || pchAttribValue == 0 )
         {
            // Key attributes should never be null, but if it is, put it out
            // by sending an empty string.
            szAttribIdent[ 0 ] = 0;
            ulLth = 0;
         }
         else
         {
            zLONG lCharCount;
            ulLth--;  // Subtract the null-terminator from the length.

            // Check to see how many chars there are that need to be
            // translated (e.g. '&').
            if ( (lCharCount = fnContainsSpecialChar( pchAttribValue ) ) )
            {
               zPCHAR pch;

               // We need to convert some characters into XML entities.  For
               // example, change '&' into '&amp;'.

               // First create a temp buffer.  Make sure it's big enough to
               // handle the buffer plus conversions.
               if ( lTempBufferLth < ulLth + lCharCount * 6 )
               {
                  if ( pchTempValueBuffer )
                     SysFreeMemory( hTempValueBuffer );

                  hTempValueBuffer = SysAllocMemory( &pchTempValueBuffer,
                                                     ulLth + lCharCount * 6,
                                                     0, 0, 0 );
                  if ( pchTempValueBuffer == 0 )
                     goto EndOfFunction;

                  lTempBufferLth = ulLth + lCharCount * 6;
               }

               // Now copy chars over, converting special ones.
               for ( pch = pchTempValueBuffer;
                     *pchAttribValue;
                     pchAttribValue++ )
               {
                  switch ( *pchAttribValue )
                  {
                     case '&':
                        *pch++ = '&';
                        *pch++ = 'a';
                        *pch++ = 'm';
                        *pch++ = 'p';
                        *pch++ = ';';
                        break;

                     case '<':
                        *pch++ = '&';
                        *pch++ = 'l';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '>':
                        *pch++ = '&';
                        *pch++ = 'g';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '\'':
                        *pch++ = '&';
                        *pch++ = 'a';
                        *pch++ = 'p';
                        *pch++ = 'o';
                        *pch++ = 's';
                        *pch++ = ';';
                        break;

                     case '\"':
                        *pch++ = '&';
                        *pch++ = 'q';
                        *pch++ = 'u';
                        *pch++ = 'o';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '\n':
                     case '\r':
                     case '\t':
                        *pch++ = *pchAttribValue;
                        break;

                     default:
                        if ( *pchAttribValue < 32 || *pchAttribValue > 127 )
                        {
                           // Convert the char to '&#ddd;' where 'ddd' is its ASCII value.
                           *pch++ = '&';
                           *pch++ = '#';
                           zltoa( (zLONG) (zUCHAR) *pchAttribValue, pch, lTempBufferLth -(pch - pchTempValueBuffer) );
                           while ( *++pch );       // Skip to null terminator.
                           *pch++ = ';';
                        }
                        else
                           *pch++ = *pchAttribValue;

                        break;

                  } // switch ( *pchAttribValue )...

               } // for...

               // Now that we've translated the attribute's value into the
               // temp buffer set attrib value ptr to point to the buffer.
               *pch++ = 0;
               ulLth = (zLONG) (pch - pchTempValueBuffer) - 1;
               //pchAttribValue = pchTempValueBuffer;
               strcpy_s( szAttribIdent, sizeof( szAttribIdent ), pchTempValueBuffer );

            } // if contains special chars
            else
            {
               strcpy_s( szAttribIdent, sizeof( szAttribIdent ), pchAttribValue );
               ulLth = zstrlen( szAttribIdent );
            }

         } // pchAttribValue non-NULL

      } // if ( lpViewAttrib->cType == zTYPE_STRING )...
      else
      {
         // Get attr value.  If return is -1 then the attr is NULL.
         if ( GetStringFromRecord( lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib,
                                   szAttribIdent,
                                   sizeof( szAttribIdent ) ) == -1 )
         {
            // Attr is NULL.  Skip null attributes if flag is set.
            szAttribIdent[ 0 ] = 0;
            ulLth = 0;
         }
         else
         {
            ulLth = zstrlen( szAttribIdent );
         }
      }

      // Append the key attribute name and vaue to entity buffer.
      sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                 sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                 " %s=\"%s\"", lpViewAttrib->szName, szAttribIdent );

   } // for each lpViewAttrib...

   nReturn = 0;

EndOfFunction:
   if ( pchTempValueBuffer )
      SysFreeMemory( hTempValueBuffer );

   return( nReturn );

} // fnWriteKeyAttribs

zSHORT LOCALOPER
fnWriteAttribs( zVIEW        lpTaskView,
                LPWRITEINFO  lpWriteInfo,
                zSHORT       nLevel,
                LPVIEWENTITY lpViewEntity,
                zLONG        lFlags )
{
   LPVIEWATTRIB  lpViewAttrib;
   LPATTRIBFLAGS lpAttribFlags;
   zCHAR         szAttribTag[ 500 ];
   zPCHAR        pchAttribValue;
   zBOOL         bCloseCDATA;
   zCHAR         chType;
   zULONG        ulLth;
   zPCHAR        pchTempValueBuffer = 0;
   zULONG        lTempBufferLth = 0;
   zLONG         hTempValueBuffer;
   zBOOL         bSkipIfNull;
   zSHORT        nReturn = zCALL_ERROR;

   szAttribTag[ 0 ] = 0;  // initialize to empty string
   lTempBufferLth = 10000;
   hTempValueBuffer = SysAllocMemory( &pchTempValueBuffer, lTempBufferLth, 0, 0, 0 );

   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( lpViewAttrib->szDerivedOper[ 0 ] && lpViewAttrib->bPersist == FALSE )
      {
         continue;
      }

      // Skip key attributes here.  Written with entity.
      if ( (lpWriteInfo->lControl & zXML_KEYATTRIBSASIDENT) && lpViewAttrib->bKey )
      {
         continue;
      }

      GetAttributeFlags( &lpAttribFlags, lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib );

      if ( (lFlags & 1) == 0 )  // not doing "simple attributes"
      {
         if ( lpViewAttrib->szXML_SimpleName[ 0 ] == 0 )
         {
            if ( lpViewAttrib->szXML_ExternalName[ 0 ] == 0 )
               sprintf_s( szAttribTag, sizeof( szAttribTag ), "<%s", lpViewAttrib->szName );
            else
               sprintf_s( szAttribTag, sizeof( szAttribTag ), "<%s", lpViewAttrib->szXML_ExternalName );

            // If the user only wants updated attributes then skip attributes that
            // aren't flagged as updated.
            if ( lpWriteInfo->lControl & zXML_UPDATEDONLY )
            {
               if ( lpAttribFlags->u.bFlags.bUpdated == FALSE )
                  continue;   // Attr is null so skip it.
            }

            // Usually we won't send null attributes.  However, if we have to send
            // attribute flags we'll have to send it if it's been changed.
            bSkipIfNull = TRUE;
            if ( lpWriteInfo->lControl & zXML_INCREFLAGS )
            {
               if ( lpAttribFlags->u.bFlags.bUpdated )
                  bSkipIfNull = FALSE;
            }

            // Add incremental update flags if so specified.
            if ( lpWriteInfo->lControl & zXML_INCREFLAGS )
            {
               zCHAR szIncreFlags[ 10 ] = "..";

               if ( lpAttribFlags->u.bFlags.bActivated )
                  szIncreFlags[ ATTRIB_FLAGS_ACTIVATED_IDX ] = 'Y';

               if ( lpAttribFlags->u.bFlags.bUpdated )
                  szIncreFlags[ ATTRIB_FLAGS_UPDATED_IDX ] = 'Y';

               // Don't bother writing the incre flags if none are set.
               if ( zstrcmp( szIncreFlags, ".." ) != 0 )
               {
                  sprintf_s( szAttribTag + zstrlen( szAttribTag ), sizeof( szAttribTag ),
                             " zIncreFlags=\"%s\"", szIncreFlags );
               }
            }

            strcat_s( szAttribTag, sizeof( szAttribTag ), ">" );

            bCloseCDATA = FALSE;
         }
         else
         {
            continue;  // skip since it's simple
         }
      }

      if ( lpViewAttrib->cType == zTYPE_BLOB )
      {
         // Get ptr to attr value.
         GetValueFromRecord( lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib,
                             &pchAttribValue, &chType, &ulLth );

         // Check to see if the attribute is NULL.  The we check ulLth = 1 for
         // strings because the length contains the null-terminator.
         if ( ulLth == 0 )
         {
            // Make sure we don't have to send attr flags.
            if ( bSkipIfNull )
               continue;   // Attr is null so skip it.

            // If we get here then the attribute is null but it's been
            // modified so we need to send the attribute flags.  We do this
            // by sending a null attribute.
            pchAttribValue = "";
         }
         else
         {
            strcat_s( szAttribTag, sizeof( szAttribTag ), "<![CDATA[" );
            bCloseCDATA = TRUE;
         }
      }
      else
      if ( lpViewAttrib->cType == zTYPE_STRING )
      {
         // Get ptr to attr value.
         GetValueFromRecord( lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib,
                             &pchAttribValue, &chType, &ulLth );

         // Check to see if the attribute is NULL.  The we check ulLth = 1 for
         // strings because the length contains the null-terminator.
         if ( ulLth <= 1 || pchAttribValue == 0 )
         {
            // Make sure we don't have to send attr flags.
            if ( bSkipIfNull )
               continue;   // attribute is null so skip it

            // If we get here then the attribute is null but it's been
            // modified so we need to send the attribute flags.  We do this
            // by sending a null attribute.
            pchAttribValue = "";
            ulLth = 0;
         }
         else
         {
            zLONG lCharCount;
            ulLth--;  // subtract the null-terminator from the length

            // Check to see how many chars there are that need to be translated (e.g. '&').
            if ( (lCharCount = fnContainsSpecialChar( pchAttribValue )) )
            {
               zPCHAR pch;

               // We need to convert some characters into XML entities.  For example,
               // change '&' into '&amp;'.

               // First create a temp buffer.  Make sure it's big enough to handle the
               // buffer plus conversions.
               if ( lTempBufferLth < ulLth + lCharCount * 6 )
               {
                  if ( pchTempValueBuffer )
                     SysFreeMemory( hTempValueBuffer );

                  hTempValueBuffer = SysAllocMemory( &pchTempValueBuffer,
                                                     ulLth + lCharCount * 6,
                                                     0, 0, 0 );
                  if ( pchTempValueBuffer == 0 )
                     goto EndOfFunction;

                  lTempBufferLth = ulLth + lCharCount * 6;
               }

               // Now copy chars over, converting special ones.
               for ( pch = pchTempValueBuffer;
                     *pchAttribValue;
                     pchAttribValue++ )
               {
                  switch ( *pchAttribValue )
                  {
                     case '&':
                        *pch++ = '&';
                        *pch++ = 'a';
                        *pch++ = 'm';
                        *pch++ = 'p';
                        *pch++ = ';';
                        break;

                     case '<':
                        *pch++ = '&';
                        *pch++ = 'l';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '>':
                        *pch++ = '&';
                        *pch++ = 'g';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '\'':
                        *pch++ = '&';
                        *pch++ = 'a';
                        *pch++ = 'p';
                        *pch++ = 'o';
                        *pch++ = 's';
                        *pch++ = ';';
                        break;

                     case '\"':
                        *pch++ = '&';
                        *pch++ = 'q';
                        *pch++ = 'u';
                        *pch++ = 'o';
                        *pch++ = 't';
                        *pch++ = ';';
                        break;

                     case '\n':
                     case '\r':
                     case '\t':
                        *pch++ = *pchAttribValue;
                        break;

                     default:
                        if ( *pchAttribValue < 32 || *pchAttribValue > 127 )
                        {
                           // Convert the char to '&#ddd;' where 'ddd' is its ASCII value.
                           *pch++ = '&';
                           *pch++ = '#';
                           zltoa( (zLONG) (zUCHAR) *pchAttribValue, pch, lTempBufferLth - (pch - pchTempValueBuffer) );
                           while ( *++pch )  // skip to null terminator
                           {
                           }

                           *pch++ = ';';
                        }
                        else
                           *pch++ = *pchAttribValue;

                        break;

                  } // switch ( *pchAttribValue )...

               } // for...

               // Now that we've translated the attribute's value into the
               // temp buffer, set attribute value ptr to point to the buffer.
               *pch++ = 0;
               ulLth = (zLONG) (pch - pchTempValueBuffer) - 1;
               pchAttribValue = pchTempValueBuffer;

            } // if contains special chars

         } // pchAttribValue non-NULL

      } // if ( lpViewAttrib->cType == zTYPE_STRING )...
      else
      {
         // Get attribute value.  If return is -1 then the attribute is NULL so skip it.
         if ( GetStringFromRecord( lpWriteInfo->vTemp, lpViewEntity, lpViewAttrib,
                                   pchTempValueBuffer, lTempBufferLth ) == -1 )
         {
            // Attribute is NULL.  Skip null attributes if flag is set.
            if ( bSkipIfNull )
               continue;
         }

         pchAttribValue = pchTempValueBuffer;
         ulLth = zstrlen( pchAttribValue );
      }

      if ( lFlags & 1 )  // doing "simple attributes"
      {
         if ( lpViewAttrib->szXML_SimpleName[ 0 ] )
         {
            sprintf_s( szAttribTag, sizeof( szAttribTag ), " %s=", lpViewAttrib->szXML_SimpleName );
            if ( pchAttribValue[ 0 ] )
            {
               sprintf_s( szAttribTag, sizeof( szAttribTag ), " %s=\"%s\"", lpViewAttrib->szXML_SimpleName, pchAttribValue );
               strcat_s( lpWriteInfo->szBuffer, sizeof( lpWriteInfo->szBuffer ), szAttribTag );
            }
         }
      }
      else
      {
         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTATTRIB,
                                         szAttribTag, 0, nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            goto EndOfFunction;
         }

         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ATTRIBVALUE,
                                         pchAttribValue, ulLth, nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            goto EndOfFunction;
         }

         // Close the attribute value.  If necessary close the CDATA section too.
         if ( bCloseCDATA )
            sprintf_s( szAttribTag, sizeof( szAttribTag ), "]]></%s>", lpViewAttrib->szName );
         else
         {
            if ( lpViewAttrib->szXML_ExternalName[ 0 ] == 0 )
               sprintf_s( szAttribTag, sizeof( szAttribTag ), "</%s>", lpViewAttrib->szName );
            else
               sprintf_s( szAttribTag, sizeof( szAttribTag ), "</%s>", lpViewAttrib->szXML_ExternalName );
         }

         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDATTRIB,
                                         szAttribTag, 0, nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            goto EndOfFunction;
         }
      }

   } // for each lpViewAttrib...

   nReturn = 0;

EndOfFunction:
   if ( pchTempValueBuffer )
      SysFreeMemory( hTempValueBuffer );

   return( nReturn );

} // fnWriteAttribs

LPENTITYINSTANCE LOCALOPER
fnWriteEntityStartTag( zVIEW            lpTaskView,
                       LPWRITEINFO      lpWriteInfo,
                       LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWENTITY     hViewEntity = lpEntityInstance->hViewEntity;
   LPVIEWENTITY     lpViewEntity = zGETPTR( hViewEntity );
   zPCHAR           pchEntityName = lpViewEntity->szName;
   zPCHAR           pchRealEntityName;
   LPENTITYINSTANCE lpAbstractChild = 0;
   LPVIEWENTITY     lpAbstractChildEntity;
   zBOOL            bEntityNameNeeded = FALSE;

   if ( lpViewEntity->szXML_Name[ 0 ] )
      pchRealEntityName = lpViewEntity->szXML_Name;
   else
      pchRealEntityName = lpViewEntity->szName;

   // Check to make sure we have written the start tag for the parent.  If we
   // are only writing entities that are updated it's possible we haven't.
   if ( lpEntityInstance->hParent &&
        lpEntityInstance->nLevel > lpWriteInfo->lpSubobjectRoot->nLevel &&
        lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel - 1 ] == 0 )
   {
      // The entity name at this level is 0 so we haven't written the parent's start tag
      // yet.  Do it now.
      fnWriteEntityStartTag( lpTaskView, lpWriteInfo, zGETPTR( lpEntityInstance->hParent ) );
   }

   // If the entity is an abstract child then the attribute values have
   // already been written so we don't need to write a start tag.
   if ( lpViewEntity->bAbstractChild )
      return( 0 );

   // If the entity is flagged as an abstract entity we need to do some
   // extra processing so we can "fold up" the abstract child entity.
   if ( lpViewEntity->bAbstract )
   {
      // Search for the abstract child.
      for ( lpAbstractChild = zGETPTR( lpEntityInstance->hNextHier );
            lpAbstractChild;
            lpAbstractChild = zGETPTR( lpAbstractChild->hNextHier ) )
      {
         // If the child instance is at the same level as the current instance
         // then there are no more children of lpEntityInstance.
         if ( lpAbstractChild->nLevel <= lpEntityInstance->nLevel )
         {
            lpAbstractChild = 0;
            break;
         }

         // If lpAbstractChild is not a direct child of lpEntityInstance then
         // skip it (and its twins if it has any).
         if ( lpAbstractChild->nLevel > lpEntityInstance->nLevel + 1 )
         {
            while ( lpAbstractChild->hNextTwin )
               lpAbstractChild = zGETPTR( lpAbstractChild->hNextTwin );

            continue;
         }

         lpAbstractChildEntity = zGETPTR( lpAbstractChild->hViewEntity );
         if ( lpAbstractChildEntity->bAbstractChild )
         {
            // We found what we want.
            pchEntityName = lpAbstractChildEntity->szName;
            if ( lpAbstractChildEntity->szXML_Name[ 0 ] )
               pchRealEntityName = lpAbstractChildEntity->szXML_Name;
            else
               pchRealEntityName = lpAbstractChildEntity->szName;

            // Because we are going to "fold up" the abstract entity we
            // need to store the Abstract entity name in the XML
            bEntityNameNeeded = TRUE;

            break;
         }
      }

      // Check to see if we found an abstract child of the entity.
      if ( lpAbstractChild == 0 )
      {
         // No abstract child.  This means we are going to use the regular
         // name of the abstract entity (i.e. not fold up). However, if the
         // abstract entity is *recursive* then we will use the recursive
         // parent name instead.
         if ( lpViewEntity->bRecursive )
         {
            LPVIEWENTITY lpParent;

            // Find the recursive parent.
            for ( lpParent = zGETPTR( lpViewEntity->hParent );
                  lpParent->bRecursivePar == FALSE;
                  lpParent = zGETPTR( lpParent->hParent ) )
            {
               // Nothing needs to be done here.
            }

            // Use the recursive parent name as the XML entity name.
            pchEntityName = lpParent->szName;
            if ( lpParent->szXML_Name[ 0 ] )
               pchRealEntityName = lpParent->szXML_Name;
            else
               pchRealEntityName = lpParent->szName;

            // Set flag to indicate we want the abstract entity's name
            // in the XML.
            bEntityNameNeeded = TRUE;
         }

      } //if ( lpAbstactChild == 0 )...

   } // if ( lpViewEntity->bAbstract )...

   // Set entity cursor to point to lpEntityInstance.
   fnEstablishViewForInstance( lpWriteInfo->vTemp, 0, lpEntityInstance );

   // Build Entity tag.
   sprintf_s( lpWriteInfo->szBuffer, sizeof( lpWriteInfo->szBuffer ), "<%s", pchRealEntityName );

   // Add key attributes as Identifiers of the entity (based on control flag).
   if ( lpWriteInfo->lControl & zXML_KEYATTRIBSASIDENT )
      fnAppendKeyAttribs( lpWriteInfo, lpViewEntity );

   // Add Zeidon info if needed.
   if ( lpWriteInfo->lControl & zXML_ZEIDONINFO )
   {
      sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                 sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                 " zLevel=\"%ld\"", (zLONG) lpEntityInstance->nLevel );

      // If we have decided that we need the LOD entity name then add it.
      if ( bEntityNameNeeded )
         sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                    sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                    " zEName=\"%s\"", lpViewEntity->szName );
   }

   // Add incremental update flags if so specified.
   if ( lpWriteInfo->lControl & zXML_INCREFLAGS )
   {
      zCHAR szIncreFlags[ 10 ] = "........";

      if ( lpEntityInstance->u.nInd.bHangingEntity )
         szIncreFlags[ ENTITY_FLAGS_HANGING_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bCreated )
         szIncreFlags[ ENTITY_FLAGS_CREATED_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bUpdated )
         szIncreFlags[ ENTITY_FLAGS_UPDATED_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bDeleted )
         szIncreFlags[ ENTITY_FLAGS_DELETED_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bExcluded )
         szIncreFlags[ ENTITY_FLAGS_EXCLUDED_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bIncluded )
         szIncreFlags[ ENTITY_FLAGS_INCLUDED_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bHidden )
         szIncreFlags[ ENTITY_FLAGS_HIDDEN_IDX ] = 'Y';
      if ( lpEntityInstance->u.nInd.bTemporal )
         szIncreFlags[ ENTITY_FLAGS_TEMPORAL_IDX ] = 'Y';

      // Don't bother writing the incre flags if none are set.
      if ( zstrcmp( szIncreFlags, "........" ) != 0 )
         sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                    sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                    " zIncreFlags=\"%s\"", szIncreFlags );
   }

   // Add entity key if so specified.
   if ( lpWriteInfo->lControl & zXML_ENTITYKEYS )
   {
      // Append the entity key.
      sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                 sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                 " zEKey=\"%lx\"", lpEntityInstance->ulKey );
   }

   // Write cursor positions if requested.
   if ( lpWriteInfo->lControl & zXML_SAVECURSORPOSITION ||
        lpWriteInfo->lControl & zXML_ZEIDONINFO )
   {
      LPENTITYINSTANCE   hEntityInstance = zGETHNDL( lpEntityInstance );
      LPVIEWENTITYCSR    lpViewEntityCsr;
      LPSELECTEDINSTANCE lpSrchSelectedInstance;

      // Check to see if the current lpEntityInstance is selected by a csr.
      // First find the cursor pointer.  If the entity is not recursive, then
      // we can just get the cursor by quick addition.
      if ( lpViewEntity->bRecursiveSt )
      {
         lpViewEntityCsr = zGETPTR( lpWriteInfo->lpViewCsr->hRootViewEntityCsr );
         lpViewEntityCsr += lpViewEntity->nHierNbr - lpViewEntityCsr->nHierNbr;
      }
      else
      {
         lpViewEntityCsr = zGETPTR( lpWriteInfo->lpViewCsr->hFirstOD_EntityCsr );
         lpViewEntityCsr += lpViewEntity->nHierNbr - 1;

      }

      if ( zGETPTR( lpViewEntityCsr->hEntityInstance ) == lpEntityInstance )
      {
         sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                    sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                    " zCursor=\"Y\"" );
      }

      // Now check to see if the entity is selected.
      for ( lpSrchSelectedInstance =
                      zGETPTR( lpWriteInfo->lpViewCsr->hFirstSelectedInstance );
            lpSrchSelectedInstance;
            lpSrchSelectedInstance =
                      zGETPTR( lpSrchSelectedInstance->hNextSelectedInstance ) )
      {
         // Check to see if the current EI is selected.
         if ( hEntityInstance != lpSrchSelectedInstance->xEntityInstance )
            continue;

         if ( hViewEntity != lpSrchSelectedInstance->hViewEntity )
            continue;

         // We have found a selected instance.  Write out the info.
         sprintf_s( lpWriteInfo->szBuffer + zstrlen( lpWriteInfo->szBuffer ),
                    sizeof( lpWriteInfo->szBuffer ) - zstrlen( lpWriteInfo->szBuffer ),
                    " zSelect%d=\"Y\"", lpSrchSelectedInstance->nSelectSet );
      }
   }

   // I think this is the right place to add the "simple attributes".  dks 2009.11.19
   fnWriteAttribs( lpTaskView, lpWriteInfo,
                   (zSHORT) (lpEntityInstance->nLevel + 1), lpViewEntity, 1 );

   strcat_s( lpWriteInfo->szBuffer, sizeof( lpWriteInfo->szBuffer ), ">" );

   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTENTITY,
                                   lpWriteInfo->szBuffer, 0,
                                   lpEntityInstance->nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      return( 0 );
   }

   // Ok, we wrote the opening tag so set the name in the "entity stack".
   lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] = pchRealEntityName;

   return( lpAbstractChild );
}

zSHORT LOCALOPER
fnWriteAttribEntry( zVIEW        lpTaskView,
                    LPWRITEINFO  lpWriteInfo,
                    zSHORT       nLevel,
                    LPVIEWENTITY lpViewEntity,
                    zPCHAR       pchAttribName,
                    LPVIEWATTRIB lpViewAttribValue )
{
   zSHORT       nReturn = zCALL_ERROR;
   zULONG       ulLth;
   zPCHAR       pchTempValueBuffer = 0;
   zULONG       lTempBufferLth = 0;
   zLONG        hTempValueBuffer;
   zCHAR        szAttribTag[ 500 ];
   zPCHAR       pchAttribValue;
   zCHAR        chType;

   sprintf_s( szAttribTag, sizeof( szAttribTag ), "<%s>", pchAttribName );

   // Treat the attribute lpViewAttribValue as the regular attribute and use its value.
   GetValueFromRecord( lpWriteInfo->vOI, lpViewEntity, lpViewAttribValue,
                        &pchAttribValue, &chType, &ulLth );

   // Check to see if the attribute is NULL.  The we check ulLth = 1 for
   // strings because the length contains the null-terminator.
   if ( ulLth <= 1 || pchAttribValue == 0 )
   {
      // If we get here then the attribute is null but it's been
      // modified so we need to send the attribute flags.  We do this
      // by sending a null attribute.
      pchAttribValue = "";
      ulLth = 0;
   }
   else
   {
      zLONG lCharCount;
      ulLth--;  // Subtract the null-terminator from the length.

      // Check to see how many chars there are that need to be
      // translated (e.g. '&').
      if ( (lCharCount = fnContainsSpecialChar( pchAttribValue ) ) )
      {
         zPCHAR pch;

         // We need to convert some characters into XML entities.  For
         // example, change '&' into '&amp;'.

         // First create a temp buffer.  Make sure it's big enough to
         // handle the buffer plus conversions.
         if ( lTempBufferLth < ulLth + lCharCount * 6 )
         {
            hTempValueBuffer = SysAllocMemory( &pchTempValueBuffer,
                                               ulLth + lCharCount * 6,
                                               0, 0, 0 );
            if ( pchTempValueBuffer == 0 )
               goto EndOfFunction;

            lTempBufferLth = ulLth + lCharCount * 6;
         }

         // Now copy chars over, converting special ones.
         for ( pch = pchTempValueBuffer;
               *pchAttribValue;
               pchAttribValue++ )
         {
            switch ( *pchAttribValue )
            {
               case '&':
                  *pch++ = '&';
                  *pch++ = 'a';
                  *pch++ = 'm';
                  *pch++ = 'p';
                  *pch++ = ';';
                  break;

               case '<':
                  *pch++ = '&';
                  *pch++ = 'l';
                  *pch++ = 't';
                  *pch++ = ';';
                  break;

               case '>':
                  *pch++ = '&';
                  *pch++ = 'g';
                  *pch++ = 't';
                  *pch++ = ';';
                  break;

               case '\'':
                  *pch++ = '&';
                  *pch++ = 'a';
                  *pch++ = 'p';
                  *pch++ = 'o';
                  *pch++ = 's';
                  *pch++ = ';';
                  break;

               case '\"':
                  *pch++ = '&';
                  *pch++ = 'q';
                  *pch++ = 'u';
                  *pch++ = 'o';
                  *pch++ = 't';
                  *pch++ = ';';
                  break;

               case '\n':
               case '\r':
               case '\t':
                  *pch++ = *pchAttribValue;
                  break;

               default:
                  if ( *pchAttribValue < 32 || *pchAttribValue > 127 )
                  {
                     // Convert the char to '&#ddd;' where 'ddd' is its ASCII value.
                     *pch++ = '&';
                     *pch++ = '#';
                     zltoa( (zLONG) (zUCHAR) *pchAttribValue, pch, lTempBufferLth - (pch - pchTempValueBuffer) );
                     while ( *++pch );       // Skip to null terminator.
                     *pch++ = ';';
                  }
                  else
                     *pch++ = *pchAttribValue;

                  break;

            } // switch ( *pchAttribValue )...

         } // for...

         // Now that we've translated the attribute's value into the
         // temp buffer set attrib value ptr to point to the buffer.
         *pch++ = 0;
         ulLth = (zLONG) ( pch - pchTempValueBuffer ) - 1;
         pchAttribValue = pchTempValueBuffer;

      } // if contains special chars

   } // pchAttribValue non-NULL

   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTATTRIB,
                                   szAttribTag, 0, nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      goto EndOfFunction;
   }

   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ATTRIBVALUE,
                                   pchAttribValue, ulLth, nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      goto EndOfFunction;
   }

   // Close the attribute value.
   sprintf_s( szAttribTag, sizeof( szAttribTag ), "</%s>", pchAttribName );

   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDATTRIB,
                                   szAttribTag, 0, nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      goto EndOfFunction;
   }

EndOfFunction:
   if ( pchTempValueBuffer )
      SysFreeMemory( hTempValueBuffer );

   return( nReturn );

} // fnWriteAttribEntry

zSHORT LOCALOPER
fnWriteListEntryValue( zVIEW            lpTaskView,
                       LPWRITEINFO      lpWriteInfo,
                       zSHORT           nLevel,
                       LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWATTRIB lpViewAttrib;
   LPVIEWENTITY lpViewEntity;
   zCHAR        szWriteLine[ 500 ];
   zCHAR        chType;
   zPCHAR       pchAttribName;
   zULONG       ulLth;

   // Get Name value.
   lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
   GetValueFromRecord( lpWriteInfo->vOI, lpViewEntity, lpViewAttrib,
                       &pchAttribName, &chType, &ulLth );

   // Create ListEntryValue line and write to file.
   sprintf_s( szWriteLine, sizeof( szWriteLine ), "<ListEntryValue Name=\"%s\">", pchAttribName );
   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTENTITY, szWriteLine,
                                   0, lpEntityInstance->nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   // Create CurrentValue data line and write to file.
   lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
   fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpEntityInstance );
   fnWriteAttribEntry( lpTaskView, lpWriteInfo, (zSHORT) (nLevel + 1),
                       lpViewEntity, "CurrentValue", lpViewAttrib );

   // Create Close ListEntryValue line and write to file.
   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDENTITY,
                                   "</ListEntryValue>", 0,
                                   lpEntityInstance->nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   return( 0 );

} // fnWriteListEntryValue

zSHORT LOCALOPER
fnWriteControlEntry( zVIEW            lpTaskView,
                     LPWRITEINFO      lpWriteInfo,
                     zSHORT           nLevel,
                     LPENTITYINSTANCE lpEntityInstance )
{
   LPVIEWATTRIB     lpViewAttrib;
   LPENTITYINSTANCE lpListEntryInstance;
   LPENTITYINSTANCE lpComboInstance;
   LPENTITYINSTANCE lpChildInstance;
   LPENTITYINSTANCE lpNextInstance;
   LPVIEWENTITY     lpViewEntity;
   LPVIEWENTITY     lpViewNextEntity;
   LPENTITYINSTANCE hEntityInstance;
   LPSELECTEDINSTANCE lpSrchSelectedInstance;
   zCHAR            szWriteLine[ 500 ];
   zCHAR            chType;
   zPCHAR           pchAttribName;
   zPCHAR           pchAttribType;
   zPCHAR           pchEntityName;
   zULONG           ulLth;
   zSHORT           nFlag;

   // Get Name and Type values.
   lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
   GetValueFromRecord( lpWriteInfo->vOI, lpViewEntity, lpViewAttrib,
                       &pchAttribName, &chType, &ulLth );
   lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
   GetValueFromRecord( lpWriteInfo->vOI, lpViewEntity, lpViewAttrib,
                       &pchAttribType, &chType, &ulLth );

   // Create Control line and write to file
   sprintf_s( szWriteLine, sizeof( szWriteLine ), "<Control Name=\"%s\" Type=\"%s\">", pchAttribName, pchAttribType );
   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTENTITY, szWriteLine,
                                   0, lpEntityInstance->nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   // If there are ListEntry entries, process them.
   lpNextInstance = zGETPTR( lpEntityInstance->hNextHier );
   if ( lpNextInstance == 0 )
      pchEntityName = 0;
   else
   {
      lpViewNextEntity = zGETPTR( lpNextInstance->hViewEntity );
      if ( lpViewNextEntity == 0 )
         pchEntityName = 0;
      else
         pchEntityName = lpViewNextEntity->szName;
   }

   if ( pchEntityName &&
        zstrcmp( pchEntityName, "ListEntry" ) == 0 )
   {
      // These are ListEntry entries.
      // Create the ListEntry lines here but process ListEntryValues in fnConvertEntity.
      for ( lpListEntryInstance = zGETPTR( lpEntityInstance->hNextHier );
            lpListEntryInstance;
            lpListEntryInstance = zGETPTR( lpListEntryInstance->hNextTwin ) )
      {
         nFlag = 0;
         fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpListEntryInstance );
         lpViewEntity = zGETPTR( lpListEntryInstance->hViewEntity );
         hEntityInstance = zGETHNDL( lpListEntryInstance );

         // Now check to see if the entity is selected.
         for ( lpSrchSelectedInstance =
                        zGETPTR( lpWriteInfo->lpViewCsr->hFirstSelectedInstance );
               lpSrchSelectedInstance;
               lpSrchSelectedInstance =
                        zGETPTR( lpSrchSelectedInstance->hNextSelectedInstance ) )
         {
            // Check to see if the current EI is selected.
            if ( hEntityInstance != lpSrchSelectedInstance->xEntityInstance )
               continue;

            if ( lpViewEntity != zGETPTR( lpSrchSelectedInstance->hViewEntity ))
               continue;

            // We have found a selected instance.
            nFlag = 1;
         }

         if ( nFlag == 0 )
            strcpy_s( szWriteLine, sizeof( szWriteLine ), "<ListEntry>" );
         else
            strcpy_s( szWriteLine, sizeof( szWriteLine ), "<ListEntry zSelect1=\"Y\">" );

         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTENTITY,
                                         szWriteLine, 0,
                                         lpListEntryInstance->nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            return( zCALL_ERROR );
         }

         // Go to fnConvertEntity for the ListEntryValues.
         for ( lpChildInstance = zGETPTR( lpListEntryInstance->hNextHier );
               lpChildInstance;
               lpChildInstance = zGETPTR( lpChildInstance->hNextHier ) )
         {
            // If the child instance is at the same level as the current instance
            // then there are no more children of lpListEntryInstance.
            if ( lpChildInstance->nLevel <= lpListEntryInstance->nLevel )
               break;

            // If lpChildInstance is not a direct child of lpListEntryInstance then
            // skip it (and it's twins if it has any).
            if ( lpChildInstance->nLevel > lpListEntryInstance->nLevel + 1 )
            {
               while ( lpChildInstance->hNextTwin )
                  lpChildInstance = zGETPTR( lpChildInstance->hNextTwin );

               continue;
            }

            // Write the child instance to the XML stream.
            if ( fnConvertEntity( lpTaskView, lpWriteInfo, lpChildInstance ) != 0 )
               return( zCALL_ERROR );
         }

         strcpy_s( szWriteLine, sizeof( szWriteLine ), "</ListEntry>" );
         fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpListEntryInstance );
         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDENTITY, szWriteLine, 0,
                                         lpListEntryInstance->nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            return( zCALL_ERROR );
         }
      }
   }
   else
   // If there are ComboValue entries, process them.
   if ( pchEntityName &&
        zstrcmp( pchEntityName, "ComboValue" ) == 0 )
   {
      // Process subentities.
      // Set the Control name in the "entity stack".
      fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpEntityInstance );
      pchEntityName = lpViewEntity->szName;
      lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] = pchEntityName;

      for ( lpComboInstance = zGETPTR( lpEntityInstance->hNextHier );
            lpComboInstance;
            lpComboInstance = zGETPTR( lpComboInstance->hNextTwin ) )
      {
         nFlag = 0;
         fnEstablishViewForInstance( lpWriteInfo->vTemp, 0, lpComboInstance );
         lpViewEntity = zGETPTR( lpComboInstance->hViewEntity );
         hEntityInstance = zGETHNDL( lpComboInstance );

         // Now check to see if the entity is selected.
         for ( lpSrchSelectedInstance = zGETPTR( lpWriteInfo->lpViewCsr->hFirstSelectedInstance );
               lpSrchSelectedInstance;
               lpSrchSelectedInstance = zGETPTR( lpSrchSelectedInstance->hNextSelectedInstance ) )
         {
            // Check to see if the current EI is selected.
            if ( hEntityInstance != lpSrchSelectedInstance->xEntityInstance )
               continue;

            if ( lpViewEntity != zGETPTR( lpSrchSelectedInstance->hViewEntity ))
               continue;

            // We have found a selected instance.
            nFlag = 1;
         }
         if ( nFlag == 0 )
            strcpy_s( szWriteLine, sizeof( szWriteLine ), "<ComboValue>" );
         else
            strcpy_s( szWriteLine, sizeof( szWriteLine ), "<ComboValue zSelect1=\"Y\">" );

         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_STARTENTITY,
                                         szWriteLine, 0,
                                         lpComboInstance->nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            return( zCALL_ERROR );
         }

         // Write the attribs for the entity.
         fnWriteAttribs( lpTaskView, lpWriteInfo, (zSHORT) (lpComboInstance->nLevel + 1),
                         zGETPTR( lpComboInstance->hViewEntity), 0 );
         strcpy_s( szWriteLine, sizeof( szWriteLine ), "</ComboValue>" );
         fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpComboInstance );
         if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDENTITY,
                                         szWriteLine, 0,
                                         lpComboInstance->nLevel,
                                         lpWriteInfo->lpPtr ) == zCALL_ERROR )
         {
            return( zCALL_ERROR );
         }
      }
   }
   else
   {
      // No subobject entries.
      // Create CurrentValue data line and write to file.
      lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib );
      lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
      fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpEntityInstance );
      fnWriteAttribEntry( lpTaskView, lpWriteInfo, (zSHORT) (nLevel + 1),
                          lpViewEntity, "CurrentValue", lpViewAttrib );
   }

   // Create Close Control line and write to file
   if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDENTITY, "</Control>",
                                   0, lpEntityInstance->nLevel,
                                   lpWriteInfo->lpPtr ) == zCALL_ERROR )
   {
      return( zCALL_ERROR );
   }

   // Make sure we indicate that we ended the Control structure.
   lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] = 0;

   return( 0 );

} // fnWriteControlEntry

//./ ADD NAME=fnConvertEntity
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       fnConvertEntity
//
//  PURPOSE:     Converts an entity instance into an XML entity instance.
//
//  DESCRIPTION:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT LOCALOPER
fnConvertEntity( zVIEW            lpTaskView,
                 LPWRITEINFO      lpWriteInfo,
                 LPENTITYINSTANCE lpEntityInstance )
{
   LPENTITYINSTANCE lpChildInstance;
   LPVIEWENTITY     lpViewEntity = zGETPTR( lpEntityInstance->hViewEntity );
   zPCHAR           pchEntityName;
   zULONG           lSkipAttribFlag = 0;

   // If entity is "dead" then don't worry about it or it's children.
   if ( fnEntityInstanceIsDead( lpEntityInstance ) )
      return( 0 );

   // Skip hidden instances unless we are sending incremental flags.
   if ( lpEntityInstance->u.nInd.bHidden )
   {
      if ( (lpWriteInfo->lControl & zXML_INCREFLAGS) == 0 )
         return( 0 );  // Nope...skip hidden instances.

      // Turn off the hidden flag so we can write it's values.
      lpEntityInstance->u.nInd.bHidden = FALSE;
   }

   // If the entity is an abstract child then the attribute values have
   // already been written so don't write it.
   // If the UPDATEDONLY flag is on we only want to write entities that have
   // been updated.
   if ( lpViewEntity->bAbstractChild == FALSE &&
        ((lpWriteInfo->lControl & zXML_UPDATEDONLY) == 0 ||
          lpEntityInstance->u.nInd.bUpdated  ||
          lpEntityInstance->u.nInd.bCreated  ||
          lpEntityInstance->u.nInd.bDeleted  ||
          lpEntityInstance->u.nInd.bIncluded ||
          lpEntityInstance->u.nInd.bExcluded) )
   {
      LPENTITYINSTANCE lpAbstractChild = 0;

      pchEntityName = lpViewEntity->szName;
      if ( (lpWriteInfo->lControl & 256 ) &&
           zstrcmp( pchEntityName, "Control" ) == 0 )
      {
         fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpEntityInstance );
         fnWriteControlEntry( lpTaskView, lpWriteInfo,
                              lpEntityInstance->nLevel, lpEntityInstance );
         lSkipAttribFlag = 1;
      }
      else
         if ( (lpWriteInfo->lControl & 256 ) &&
              zstrcmp( pchEntityName, "ListEntryValue" ) == 0 )
         {
            fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpEntityInstance );
            fnWriteListEntryValue( lpTaskView, lpWriteInfo,
                                   lpEntityInstance->nLevel,
                                   lpEntityInstance );
            lSkipAttribFlag = 1;
         }
         else
         {
            lpAbstractChild = fnWriteEntityStartTag( lpTaskView, lpWriteInfo,
                                                     lpEntityInstance );

            // Write the attribs for the entity.
            fnWriteAttribs( lpTaskView, lpWriteInfo,
                            (zSHORT) (lpEntityInstance->nLevel + 1), lpViewEntity, 0 );
         }

      // If the entity is abstract write the attribs of the abstract child.
      if ( lpAbstractChild )
      {
         LPVIEWENTITY lpAbstractEntity = zGETPTR( lpAbstractChild->hViewEntity );

         // Set entity cursor to point to abstract child.
         fnEstablishViewForInstance( lpWriteInfo->vOI, 0, lpAbstractChild );

         // Write attribs for abstract child.  Note that we use the level of the parent.
         fnWriteAttribs( lpTaskView, lpWriteInfo,
                         (zSHORT) (lpEntityInstance->nLevel + 1), lpAbstractEntity, 0 );
      }

   } // if ( lpViewEntity->bAbstractChild == FALSE )...

   // Now write the child instances.
   if ( lSkipAttribFlag == 0 )
   {
      for ( lpChildInstance = zGETPTR( lpEntityInstance->hNextHier );
            lpChildInstance;
            lpChildInstance = zGETPTR( lpChildInstance->hNextHier ) )
      {
         // If the child instance is at the same level as the current instance
         // then there are no more children of lpEntityInstance.
         if ( lpChildInstance->nLevel <= lpEntityInstance->nLevel )
            break;

         // If lpChildInstance is not a direct child of lpEntityInstance then
         // skip it (and it's twins if it has any).
         if ( lpChildInstance->nLevel > lpEntityInstance->nLevel + 1 )
         {
            while ( lpChildInstance->hNextTwin )
               lpChildInstance = zGETPTR( lpChildInstance->hNextTwin );

            continue;
         }

         // Write the child instance to the XML stream.
         if ( fnConvertEntity( lpTaskView, lpWriteInfo, lpChildInstance ) != 0 )
            return( zCALL_ERROR );
      }
   }

   // If we wrote a start tag for this entity then we need to write an 'end' tag.
   if ( lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] != 0 )
   {
      sprintf_s( lpWriteInfo->szBuffer, sizeof( lpWriteInfo->szBuffer ), "</%s>",
                 lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] );

      if ( (*lpWriteInfo->lpfnWrite)( lpTaskView, zXML_ENDENTITY,
                                      lpWriteInfo->szBuffer, 0,
                                      lpEntityInstance->nLevel,
                                      lpWriteInfo->lpPtr ) == zCALL_ERROR )
      {
         return( zCALL_ERROR );
      }

      // We've written the end tag so set the entity name in the "entity
      // stack" to 0 to indicate it's been written.
      lpWriteInfo->pchEntityName[ lpEntityInstance->nLevel ] = 0;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       WriteSubobjectToXML
//
//  PURPOSE:     Writes an OI/Subobject to a XML stream.
//
//  ARGUMENTS:
//    vOI       - OI to convert.
//    pchEntityName - Name of the root entity for the subobject to write.  If
//                NULL or "" then the whole object is written.
//    lControl  - Control flags.
//
//       Current valid values:
//
//          zXML_ZEIDONINFO - Will cause meta information about the OI to be
//             be included in the XML as attribute values in the tag.  For
//             example, the entity level, LOD name, etc. will be written.
//
//          zXML_INCREFLAGS - Incremental update flags for every entity and
//             attribute will be written as attribute values in tags.
//
//          zXML_UPDATEDONLY - Send only entities/attributes that have been
//             updated in some way.
//
//          zXML_ENTITYKEYS - Write internal entity keys to XML.
//
//          zXML_ROOTONLY   - Write only the root entity of the subobject.
//
//    lpfnWrite - Callback function that handles writing XML data to stream.
//    lpPtr     - Data pointer for callback function.
//
/////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
WriteSubobjectToXML( zVIEW         vOI,
                     zCPCHAR       pchEntityName,
                     zLONG         lControl,
                     LPFN_XMLWRITE lpfnWrite,
                     zPVOID        lpPtr )
{
   LPTASK            lpCurrentTask;
   zCHAR             szBuffer[ 1000 ];
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOD          lpViewOD;
   LPENTITYINSTANCE  lpEntityInstance;
   LPENTITYINSTANCE  lpStartEntityInstance;
   LPENTITYINSTANCE  lpEndEntityInstance;
   LPAPP             lpApp;
   zBOOL             bCloseZOI = FALSE;
   WriteInfoRecord   WriteInfo = { 0 };
   zPCHAR            pch;
   zSHORT            nRC = zCALL_ERROR;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iWriteSubobjectToXML, vOI,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   lpViewCsr = (LPVIEWCSR) zGETPTR( vOI->hViewCsr );
   lpViewOI  = (LPVIEWOI)  zGETPTR( lpViewCsr->hViewOI );
   lpViewOD  = (LPVIEWOD)  zGETPTR( vOI->hViewOD );

   // If the entity name was supplied, then verify it.
   if ( pchEntityName && pchEntityName[ 0 ] )
   {
      LPVIEWENTITYCSR lpViewEntityCsr;

      if ( fnValidViewEntity( &lpViewEntityCsr, vOI, pchEntityName, 0 ) == 0 )
      {
         fnOperationReturn( iWriteSubobjectToXML, lpCurrentTask );
         return( zCALL_ERROR );
      }

      if ( lpViewEntityCsr->hEntityInstance == UNSET_CSR )
         fnEstablishCursorForView( lpViewEntityCsr );

      lpStartEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );
      lpEndEntityInstance   = zGETPTR( lpStartEntityInstance->hNextTwin );
   }
   else
   {
      lpStartEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
      lpEndEntityInstance   = 0;
   }

   // If we are only writing the root of the subobject then stop at the next twin.
   if ( lControl & zXML_ROOTONLY )
      lpEndEntityInstance = zGETPTR( lpStartEntityInstance->hNextTwin );

   pch = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>";
   if ( (*lpfnWrite)( vOI, 0, pch, 0, 0, lpPtr ) == zCALL_ERROR )
   {
      fnOperationReturn( iWriteSubobjectToXML, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lControl & zXML_ZEIDONINFO )
   {
      lpApp = zGETPTR( lpViewOD->hApp );

      if ( lControl & zXML_INCREFLAGS )
         sprintf_s( szBuffer, sizeof( szBuffer ), "<zOI zObjectName=\"%s\" zAppName=\"%s\" "
                    "zIncreFlags=\"Y\">", lpViewOD->szName, lpApp->szName );
      else
         sprintf_s( szBuffer, sizeof( szBuffer ), "<zOI zObjectName=\"%s\" zAppName=\"%s\">",
                    lpViewOD->szName, lpApp->szName );

      if ( (*lpfnWrite)( vOI, zXML_STARTOI, szBuffer, 0, 0, lpPtr ) == zCALL_ERROR )
      {
         fnOperationReturn( iWriteSubobjectToXML, lpCurrentTask );
         return( zCALL_ERROR );
      }

      bCloseZOI = TRUE;
   }
   else
   // If we are writing multiple root entities then we'll write an enclosing tag.
   if ( (lControl & zXML_ROOTONLY ) == 0 && lpStartEntityInstance->hNextTwin )
   {
      LPENTITYINSTANCE lpSearch;

      // Check to see if there's a non-hidden EI
      for ( lpSearch = zGETPTR( lpStartEntityInstance->hNextTwin );
            lpSearch;
            lpSearch = zGETPTR( lpSearch->hNextTwin ) )
      {
         // If it's hidden, skip it.
         if ( lpSearch->u.nInd.bHidden )
            continue;

         // Entity isn't hidden so write the root tag and break out.
         strcpy_s( szBuffer, sizeof( szBuffer ), "<zOI>" );
         if ( (*lpfnWrite)( vOI, zXML_STARTOI, szBuffer, 0, 0, lpPtr ) == zCALL_ERROR )
         {
            fnOperationReturn( iWriteSubobjectToXML, lpCurrentTask );
            return( zCALL_ERROR );
         }

         bCloseZOI = TRUE;

         break;
      }
   }

   WriteInfo.vOI             = vOI;
   WriteInfo.lpViewCsr       = lpViewCsr;
   WriteInfo.lControl        = lControl;
   WriteInfo.lpfnWrite       = lpfnWrite;
   WriteInfo.lpPtr           = lpPtr;
   WriteInfo.lpSubobjectRoot = lpStartEntityInstance;
   CreateViewFromViewForTask( &WriteInfo.vTemp, vOI, 0 );

   // Loop through the EIs and write them out.
   for ( lpEntityInstance = lpStartEntityInstance;
         lpEntityInstance != lpEndEntityInstance;
         lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin ) )
   {
      if ( fnConvertEntity( vOI, &WriteInfo, lpEntityInstance ) != 0 )
         goto EndOfFunction;
   }

   if ( bCloseZOI )
   {
      pch = "</zOI>";
      if ( (*lpfnWrite)( vOI, zXML_ENDOI, pch, 0, 0, lpPtr ) == zCALL_ERROR )
         goto EndOfFunction;
   }

   // If we are writing incre flags then we might have turned off some hidden
   // flags.  Turn them back on.
   if ( lControl & zXML_INCREFLAGS )
   {
      for ( lpEntityInstance = lpStartEntityInstance;
            lpEntityInstance != lpEndEntityInstance;
            lpEntityInstance = zGETPTR( lpEntityInstance->hNextHier ) )
      {
         if ( lpEntityInstance->u.nInd.bDeleted || lpEntityInstance->u.nInd.bExcluded )
            lpEntityInstance->u.nInd.bHidden = TRUE;
      }
   }

   nRC = 0;

EndOfFunction:
   if ( WriteInfo.vTemp != 0 )
      fnDropView( WriteInfo.vTemp );

   fnOperationReturn( iWriteSubobjectToXML, lpCurrentTask );
   return( nRC );
}

//./ ADD NAME=WriteOI_ToXML
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       WriteOI_ToXML
//
//  PURPOSE:     Writes an OI to a XML stream.
//
//  See WriteSubobjectToXML for documentation on arguments.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
WriteOI_ToXML( zVIEW         vOI,
               zLONG         lControl,
               LPFN_XMLWRITE lpfnWrite,
               zPVOID        lpPtr )
{
   LPTASK   lpCurrentTask;
   zSHORT   nRC;

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iWriteOI_ToXML, vOI, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   nRC = WriteSubobjectToXML( vOI, 0, lControl, lpfnWrite, lpPtr );

   fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
   return( nRC );
}

zSHORT OPERATION
WriteMultipleOIs_ToXML( zVIEW         vOI[],
                        zSHORT        nViewCount,
                        zLONG         lControl,
                        LPFN_XMLWRITE lpfnWrite,
                        zPVOID        lpPtr )
{
   LPTASK            lpCurrentTask;
   zCHAR             szBuffer[ 1000 ];
   LPVIEWOI          lpViewOI;
   LPVIEWCSR         lpViewCsr;
   LPVIEWOD          lpViewOD;
   LPENTITYINSTANCE  lpEntityInstance;
   LPAPP             lpApp;
   WriteInfoRecord   WriteInfo = { 0 };
   zPCHAR            pch;
   zSHORT            k;

   if ( nViewCount == 0 )
      return( 0 );

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iWriteOI_ToXML, vOI[ 0 ], 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   for ( k = 0; k < nViewCount; k++ )
   {
      if ( fnValidView( lpCurrentTask, vOI[ k ] ) == 0 )
      {
         fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   lpViewOD = (LPVIEWOD) zGETPTR( vOI[ 0 ]->hViewOD );

   pch = "<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>";
   if ( (*lpfnWrite)( vOI[ 0 ], 0, pch, 0, 0, lpPtr ) == zCALL_ERROR )
   {
      fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
      return( zCALL_ERROR );
   }

   if ( lControl & zXML_ZEIDONINFO )
   {
      SfGetApplicationForSubtask( &lpApp, vOI[ 0 ] );

      if ( lControl & zXML_INCREFLAGS )
         sprintf_s( szBuffer, sizeof( szBuffer ), "<zOI zObjectName=\"%s\" zAppName=\"%s\" "
                    "zIncreFlags=\"Y\">", lpViewOD->szName, lpApp->szName );
      else
         sprintf_s( szBuffer, sizeof( szBuffer ), "<zOI zObjectName=\"%s\" zAppName=\"%s\">",
                    lpViewOD->szName, lpApp->szName );

      if ( (*lpfnWrite)( vOI[ 0 ], zXML_STARTOI, szBuffer, 0, 0, lpPtr ) == zCALL_ERROR )
      {
         fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   WriteInfo.lControl  = lControl;
   WriteInfo.lpfnWrite = lpfnWrite;
   WriteInfo.lpPtr     = lpPtr;

   for ( k = 0; k < nViewCount; k++ )
   {
      lpViewCsr = (LPVIEWCSR) zGETPTR( vOI[ k ]->hViewCsr );
      lpViewOI  = (LPVIEWOI)  zGETPTR( lpViewCsr->hViewOI );

      WriteInfo.vOI = vOI[ k ];

      for ( lpEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
            lpEntityInstance;
            lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin ) )
      {
         if ( fnConvertEntity( vOI[ 0 ], &WriteInfo, lpEntityInstance ) != 0 )
         {
            fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
            return( zCALL_ERROR );
         }
      }
   }

   if ( lControl & zXML_ZEIDONINFO )
   {
      pch = "</zOI>";
      if ( (*lpfnWrite)( vOI[ 0 ], zXML_ENDOI, pch, 0, 0, lpPtr ) == zCALL_ERROR )
      {
         fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
         return( zCALL_ERROR );
      }
   }

   // If we are writing incre flags then we might have turned off some hidden
   // flags.  Turn them back on.
   if ( lControl & zXML_INCREFLAGS )
   {
      for ( k = 0; k < nViewCount; k++ )
      {
         lpViewCsr = (LPVIEWCSR) zGETPTR( vOI[ k ]->hViewCsr );
         lpViewOI  = (LPVIEWOI)  zGETPTR( lpViewCsr->hViewOI );

         for ( lpEntityInstance = zGETPTR( lpViewOI->hRootEntityInstance );
               lpEntityInstance;
               lpEntityInstance = zGETPTR( lpEntityInstance->hNextTwin ) )
         {
            if ( lpEntityInstance->u.nInd.bDeleted ||
                 lpEntityInstance->u.nInd.bExcluded )
            {
               lpEntityInstance->u.nInd.bHidden = TRUE;
            }
         }
      }
   }

   fnOperationReturn( iWriteOI_ToXML, lpCurrentTask );
   return( 0 );
}

// Return the value of an identifier.
void LOCALOPER
fnGetIdentifierValue( LPPARSERINFO  lpInfo,
                      const zCHAR **ppAtts,
                      zPCHAR        pchIdName,
                      zPPCHAR       ppchIdValue )
{
   zSHORT k;

   *ppchIdValue = 0;

   for ( k = 0; ppAtts[ k ] != 0; k += 2 )
   {
      if ( zstrcmp( ppAtts[ k ], pchIdName ) == 0 )
      {
         *ppchIdValue = (zPCHAR) ppAtts[ k + 1 ];
         return;
      }
   }
}

void LOCALOPER
fnTraceAttributes( zCPCHAR       cpcElementName,
                   const zCHAR **ppAtts )
{
   zSHORT k;

   TraceLineS( "(xml) Element = ", cpcElementName );
   for ( k = 0; ppAtts[ k ] != 0; k += 2 )
   {
      zCHAR szMsg[ 2000 ];

      sprintf_s( szMsg, sizeof( szMsg ), "(xml) Attrib %s = %s", ppAtts[ k ], ppAtts[ k + 1 ] );
      TraceLineS( szMsg, "" );
   }
}

void LOCALOPER
fnProcessOI_Element( LPPARSERINFO  lpInfo,
                     const zCHAR **ppAtts )
{
   zPCHAR pchObjectName;
   zPCHAR pchAppName;
   zPCHAR pch;

   // Activate the subtask/application if it's needed.
   if ( lpInfo->vSubtask == 0 )
   {
      fnGetIdentifierValue( lpInfo, ppAtts, "zAppName", &pchAppName );
      if ( pchAppName == 0 ||
           SfCreateSubtask( &lpInfo->vSubtask, 0, pchAppName ) == zCALL_ERROR )
      {
         lpInfo->bFatalError = TRUE;
         return;
      }
   }

   // Activate an empty object instance.
   fnGetIdentifierValue( lpInfo, ppAtts, "zObjectName", &pchObjectName );
   if ( pchObjectName == 0 ||
        ActivateEmptyObjectInstance( &lpInfo->vOI, pchObjectName,
                                     lpInfo->vSubtask,
                                     zMULTIPLE ) == zCALL_ERROR )
   {
      SfDropSubtask( lpInfo->vSubtask, 0 );
      lpInfo->bFatalError = TRUE;
      return;
   }

   // Check for incremental update flags.
   fnGetIdentifierValue( lpInfo, ppAtts, "zIncreFlags", &pch );
   if ( pch && *pch == 'Y' )
      lpInfo->bIncreFlags = TRUE;

} // fnProcessOI_Element

void LOCALOPER
fnProcessEntityElement( LPPARSERINFO  lpInfo,
                        zCPCHAR       cpcElementName,
                        zCPCHAR       pchEntityName,
                        const zCHAR **ppAtts )
{
   LPVIEWENTITY      lpViewEntity;
   LPVIEWENTITY      lpParent;
   LPENTITYINSTANCE  lpEntityInstance;
   LPVIEWENTITYCSR   lpViewEntityCsr;
   zPCHAR            pchFlag;
   zSHORT            nLevel;

   // Create the entity
   lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpInfo->vOI, pchEntityName, 0 );
   if ( lpViewEntity == 0 )
   {
      lpInfo->bFatalError = TRUE;
      return;
   }

   // Check to see if the parent of the current entity is an abstract child.
   // If it is, and if the current entity is the first child found under the
   // abstract child, we need to bump up nDepth because the abstract child
   // is really two entity instances.
   lpParent = zGETPTR( lpViewEntity->hParent );
   if ( lpParent && lpInfo->lpViewEntity &&
        zstrcmp( lpParent->szName, lpInfo->lpViewEntity->szName ) != 0 )
   {
      zCHAR  szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "(xml) Parent mismatch (%s != %s) for entity: %s",
                lpInfo->lpViewEntity->szName, lpParent->szName, pchEntityName );
      TraceLineS( szMsg, "" );
      SysMessageBox( 0, "XML Parser", szMsg, -1 );
      lpInfo->bFatalError = TRUE;
      return;
   }

   lpInfo->nDepth++;

   if ( lpParent && lpParent->bAbstractChild &&
        lpInfo->EntityInfo[ lpParent->nHierNbr ].bFirstUnderAbChild == FALSE )
   {
      lpInfo->EntityInfo[ lpParent->nHierNbr ].bFirstUnderAbChild = TRUE;
      lpInfo->nDepth++;
   }

   nLevel = lpInfo->nDepth;

   if ( lpViewEntityCsr->nLevel > nLevel )
   {
      LPVIEWCSR lpViewCsr = zGETPTR( lpInfo->vOI->hViewCsr );

      while ( lpViewEntityCsr->nLevel > nLevel &&
              lpViewCsr->hFirstViewSubobject )
      {
         fnResetViewFromSubobject( lpInfo->vOI );
      }
   }
   else
   if ( lpViewEntityCsr->nLevel < nLevel && lpInfo->lpPrevEntityInstance )  // dks 2009.11.11
   {
      // The view entity csr level is < the entity level.
      // In this case, the current (last) instance must be the
      // parent of the instance we are about to create and
      // it must have recursive behaviour. Otherwise we will
      // drop out and issue a level error
      LPVIEWENTITY lpWkViewEntity =
                  zGETPTR( lpInfo->lpPrevEntityInstance->hViewEntity );

      fnSetViewToSubobject( lpInfo->vOI, lpWkViewEntity, 0 );
   }

   fnCreateEntity( lpInfo->vOI, lpViewEntity, lpViewEntityCsr, zPOS_LAST, 0 );
   lpEntityInstance = zGETPTR( lpViewEntityCsr->hEntityInstance );

   // Now set the flags for the entity.
   lpEntityInstance->u.nIndicators = 0;
   lpEntityInstance->u.nInd.bUpdated = TRUE;
   fnGetIdentifierValue( lpInfo, ppAtts, "zEKey", &pchFlag );
   if ( pchFlag )
      lpEntityInstance->ulKey = (zULONG) zxtol( pchFlag );

   // Store the indicators so we can re-set the flags after the attribute
   // values have been set.
   lpInfo->nIndicators[ lpInfo->nDepth ] = lpEntityInstance->u.nIndicators;
   lpInfo->lpEntityInstance[ lpInfo->nDepth ] = lpEntityInstance;

   lpInfo->lpViewEntity = lpViewEntity;

   // If the entity is an abstract entity then we need to create the abstract
   // child, too.  UNLESS the element and entity names are the same.  If they
   // are the the abstract parent has no child under it and the element name
   // *is* the name of the abstract element.  If this is the case then we
   // don't need to create an abstract child.
   if ( lpViewEntity->bAbstract &&
        zstrcmp( cpcElementName, pchEntityName ) != 0 )
   {
      // Init/reset any info stored for each entity type.
      lpInfo->EntityInfo[ lpViewEntity->nHierNbr ].bFirstUnderAbChild = FALSE;

      // The entity is abstract.  This means that the element name had better
      // be the name of an abstract child entity.
      lpViewEntity = fnValidViewEntity( &lpViewEntityCsr, lpInfo->vOI,
                                        cpcElementName, 0 );

      if ( lpViewEntity == 0 )
      {
         lpInfo->bFatalError = TRUE;
         return;
      }

      nLevel++;
      if ( lpViewEntityCsr->nLevel > nLevel )
      {
         LPVIEWCSR lpViewCsr = zGETPTR( lpInfo->vOI->hViewCsr );

         while ( lpViewEntityCsr->nLevel > nLevel &&
                 lpViewCsr->hFirstViewSubobject )
         {
            fnResetViewFromSubobject( lpInfo->vOI );
         }
      }
      else
      if ( lpViewEntityCsr->nLevel < nLevel )
      {
         zLONG lER_Tok;

         // The view entity csr level is < the entity level.
         // In this case, the current instance must be the
         // parent of the instance we are about to create and
         // it must have recursive behaviour. Otherwise we will
         // drop out and issue a level error
         LPVIEWENTITY lpWkViewEntity = zGETPTR( lpEntityInstance->hViewEntity );

         fnSetViewToSubobject( lpInfo->vOI, lpWkViewEntity, 0 );

         // We have just set view to subobject so that the abstract child is
         // in view.  This means that the parent of the abstract child is now
         // referenced by the recursive parent.  Find the recursive parent
         // and set lpInfo accordingly.
         lER_Tok = lpWkViewEntity->lEREntTok;
         for ( lpWkViewEntity = zGETPTR( lpWkViewEntity->hParent );
               lpWkViewEntity->lEREntTok != lER_Tok;
               lpWkViewEntity = zGETPTR( lpWkViewEntity->hParent ) )
         {
            // Nothing needs to be done here.
         }

         lpInfo->lpViewEntity = lpWkViewEntity;
      }

      fnCreateEntity( lpInfo->vOI, lpViewEntity, lpViewEntityCsr, zPOS_LAST, 0 );

      lpInfo->lpAbstractChild   = lpViewEntity;
      lpInfo->lpAbstractChildEI = zGETPTR( lpViewEntityCsr->hEntityInstance );
   }
   else
   {
      lpInfo->lpAbstractChild   = 0;
      lpInfo->lpAbstractChildEI = 0;
   }

} // fnProcessEntityElement

void LOCALOPER
fnProcessAttribElement( LPPARSERINFO  lpInfo,
                        zCPCHAR       pchAttribName,
                        const zCHAR **ppAtts )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   zPCHAR       pchFlag;

   lpInfo->nDepth++;

   lpViewEntity = lpInfo->lpViewEntity;

   // Look for the attribute.  Note that we can't use fnValidViewAttrib( )
   // because it won't find hidden attributes.
   for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( zstrcmp( lpViewAttrib->szName, pchAttribName ) == 0 ||
           zstrcmp( lpViewAttrib->szXML_ExternalName, pchAttribName ) == 0 )
         break;
   }

   // If we didn't find it, check the abstract child entity.
   if ( lpViewAttrib == 0 && lpViewEntity->bAbstract )
   {
      lpViewEntity = lpInfo->lpAbstractChild;
      for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
            lpViewAttrib;
            lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
      {
         if ( zstrcmp( lpViewAttrib->szName, pchAttribName ) == 0 )
            break;
      }
   }

   if ( lpViewAttrib == 0 )
   {
      zCHAR  szMsg[ 256 ];

      sprintf_s( szMsg, sizeof( szMsg ), "(xml) Could not find Entity.Attribute: %s.%s",
                lpViewEntity->szName, pchAttribName );
      TraceLineS( szMsg, "" );
      SysMessageBox( 0, "XML Parser", szMsg, -1 );
      lpInfo->bFatalError = -1; // set it to be a temporary error (to prevent endElement from occurring once)
      lpInfo->nDepth--;
      return;
   }

   lpInfo->lpViewAttrib = lpViewAttrib;

   // Get the attribute flags.
   lpInfo->AttribFlags.u.uFlags = 0;
   lpInfo->AttribFlags.u.bFlags.bUpdated = TRUE;
   fnGetIdentifierValue( lpInfo, ppAtts, "zIncreFlags", &pchFlag );
   if ( lpInfo->bIncreFlags && pchFlag )
   {
      lpInfo->AttribFlags.u.bFlags.bActivated =
                                 pchFlag[ ATTRIB_FLAGS_ACTIVATED_IDX ] == 'Y';
      lpInfo->AttribFlags.u.bFlags.bUpdated =
                                 pchFlag[ ATTRIB_FLAGS_UPDATED_IDX ] == 'Y';
   }

} // fnProcessAttribElement

// static int nStartCnt = 0;  // debugging

void
startElement( zPVOID        pvUserData,
              const zCHAR  *cpcElementName,
              const zCHAR **ppAtts )
{
   zPCHAR       pchEntityFlag;
   zPCHAR       pchEntityName;
   LPPARSERINFO lpInfo = (LPPARSERINFO) pvUserData;

   // If we've had a fatal error then just skip processing.
   if ( lpInfo->bFatalError )
      return;

   #ifdef TRACE_XML
      fnTraceAttributes( cpcElementName, ppAtts );
   #endif

   // If the element name is "zOI" then get info from attributes.
   if ( zstrcmp( cpcElementName, "zOI" ) == 0 )
   {
      lpInfo->bZeidonXML = TRUE;
      fnProcessOI_Element( lpInfo, ppAtts );
      return;
   }
   else
   if ( lpInfo->vOI == 0 )
   {
      if ( ActivateEmptyObjectInstance( &lpInfo->vOI, lpInfo->lpViewOD->szName,
                                        lpInfo->vSubtask, zMULTIPLE ) == zCALL_ERROR )
      {
         lpInfo->bFatalError = TRUE;
         return;
      }
   }

   // Check to see if the tag is an entity.  It will be an entity if there
   // is an identifier called zLevel.
   fnGetIdentifierValue( lpInfo, ppAtts, "zLevel", &pchEntityFlag );
   if ( pchEntityFlag && *pchEntityFlag )
   {
      // Trying to find out when this occurs ... dks 2009.11.13
      SysMessageBox( lpInfo->vSubtask, "Error", "Located zLevel parsing XML", 1 );

      // Check to see if the entity name is supplied.  It will be supplied
      // if the entity name is different from the element name.
      fnGetIdentifierValue( lpInfo, ppAtts, "zEName", &pchEntityName );
      if ( pchEntityName == 0 || *pchEntityName == 0 )
      {
         // It wasn't supplied so use the element name.
         pchEntityName = (zPCHAR) cpcElementName;
      }

      fnProcessEntityElement( lpInfo, cpcElementName, pchEntityName, ppAtts );
      return;
   }
   else
   if ( lpInfo->bZeidonXML == FALSE )
   {
      LPVIEWENTITY lpViewEntity;

      // Check to see if the element name matches an entity name.
      for ( lpViewEntity = zGETPTR( lpInfo->lpViewOD->hFirstOD_Entity );
            lpViewEntity;
            lpViewEntity = zGETPTR( lpViewEntity->hNextHier ) )
      {
         if ( zstrcmp( cpcElementName, lpViewEntity->szName ) == 0 )
            break;
      }

      if ( lpViewEntity == 0 )  // did not find entity the normal way
      {
         LPVIEWENTITY lpParentViewEntity;

          // Check for entity using the XML entity name (to permit us to process duplicate XML entity names).
         for ( lpViewEntity = zGETPTR( lpInfo->lpViewOD->hFirstOD_Entity );
               lpViewEntity;
               lpViewEntity = zGETPTR( lpViewEntity->hNextHier ) )
         {
            if ( lpViewEntity->szXML_Name[ 0 ] )
            {
               if ( zstrcmp( cpcElementName, lpViewEntity->szXML_Name ) == 0 )
               {
                  lpParentViewEntity = zGETPTR( lpViewEntity->hParent );
                  if ( zstrcmp( lpParentViewEntity->szName, lpInfo->lpViewEntity->szName ) == 0 )
                     break;
               }
            }
         }
      }

      if ( lpViewEntity )
      {
         zSHORT k;

      // TraceLine( "StartElement ParentEntity: %s   Entity: %s [%d]", lpInfo->lpViewEntity ? lpInfo->lpViewEntity->szName : "<root>", lpViewEntity->szName, nStartCnt );
         fnProcessEntityElement( lpInfo, lpViewEntity->szName, lpViewEntity->szName, ppAtts );
         if ( lpInfo->bFatalError )
            return;

         // Let's look for "simple attributes" and set them if defined in the LOD.  We will not
         // get a startElement or an endElement for the simple attributes, so we have to take
         // care of everything right here and now.
         for ( k = 0; ppAtts[ k ] != 0; k += 2 )
         {
            LPVIEWATTRIB  lpViewAttrib;
            LPATTRIBFLAGS lpAttribFlags;

            // Look for the "simple attribute".
            for ( lpViewAttrib = zGETPTR( lpViewEntity->hFirstOD_Attrib );
                  lpViewAttrib;
                  lpViewAttrib = zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
            {
               if ( zstrcmp( lpViewAttrib->szXML_SimpleName, ppAtts[ k ] ) == 0 )
                  break;
            }

            // If we didn't find it, we're not too worried, but will trace the failure.
            if ( lpViewAttrib == 0 )
            {
               TraceLine( "(xml) Could not find Entity.SimpleAttribute: %s.%s",
                          lpViewEntity->szName, ppAtts[ k ] );
            }
            else
            {
            // TraceLine( "(xml) Processing Entity.SimpleAttribute: %s.%s=%s",
            //            lpViewEntity->szName, ppAtts[ k ], ppAtts[ k + 1 ] );

               if ( lpViewAttrib->cType == zTYPE_BLOB )
               {
                  StoreValueInRecord( lpInfo->vOI, lpViewEntity, lpViewAttrib,
                                      (zPCHAR) ppAtts[ k + 1 ], strlen( ppAtts[ k + 1 ] ) );
               }
               else
               {
                  StoreStringInRecord( lpInfo->vOI, lpViewEntity, lpViewAttrib,
                                       ppAtts[ k + 1 ] );
               }

               // Set attribute flags.
               GetAttributeFlags( &lpAttribFlags, lpInfo->vOI, lpViewEntity, lpViewAttrib );
               lpAttribFlags->u.uFlags = lpInfo->AttribFlags.u.uFlags;
            }
         }

         return;
      }
   }

   if ( lpInfo->lpViewEntity == 0 )
   {
      TraceLineS( "(xml) ERROR - Attribute without an entity.  Tag = ", cpcElementName );
      return;
   }

   // If we get here then the element must be an attribute.
   fnProcessAttribElement( lpInfo, cpcElementName, ppAtts );
}

void
endElement( zPVOID       pvUserData,
            const zCHAR *cpcElementName )
{
   LPPARSERINFO lpInfo = (LPPARSERINFO) pvUserData;

   #ifdef TRACE_XML
      TraceLineS( "(xml) End of element ", cpcElementName );
   #endif

   if ( lpInfo->bFatalError )
   {
      if ( lpInfo->bFatalError < 0 )
         lpInfo->bFatalError = 0;

      return;
   }

   // If lpViewAttrib is not 0 then we've just finished reading attribute data.
   // Set the attribute value.
   if ( lpInfo->lpViewAttrib )
   {
      LPVIEWATTRIB  lpViewAttrib = lpInfo->lpViewAttrib;
      LPVIEWENTITY  lpViewEntity = zGETPTR( lpViewAttrib->hViewEntity );
      LPATTRIBFLAGS lpAttribFlags;

      if ( lpViewAttrib->cType == zTYPE_BLOB )
      {
         StoreValueInRecord( lpInfo->vOI, lpViewEntity, lpViewAttrib,
                             lpInfo->pchAttributeBuffer,
                             lpInfo->nBufferCurrentUsed );
      }
      else
      {
         // Terminate the string.
         lpInfo->pchAttributeBuffer[ lpInfo->nBufferCurrentUsed ] = 0;

         StoreStringInRecord( lpInfo->vOI, lpViewEntity, lpViewAttrib,
                              lpInfo->pchAttributeBuffer );
      }

      // Set attribute flags.
      GetAttributeFlags( &lpAttribFlags, lpInfo->vOI, lpViewEntity, lpViewAttrib );
      lpAttribFlags->u.uFlags = lpInfo->AttribFlags.u.uFlags;

      // Set ptr to indicate we are no longer processing an attribute.
      lpInfo->lpViewAttrib = 0;

      // Indicate Attr buffer is empty.
      lpInfo->nBufferCurrentUsed = 0;
   }
   else
   if ( lpInfo->lpViewEntity )
   {
      // Must be the end of the entity.  This means that the current entity
      // is now the parent of lpViewEntity.
      lpInfo->lpViewEntity = zGETPTR( lpInfo->lpViewEntity->hParent );

      // We are done with this entity so we can now set its incremental update flags.
      if ( lpInfo->lpEntityInstance[ lpInfo->nDepth ] == 0 ) // zero if error
         SysMessageBox( lpInfo->vSubtask, "Error", "Depth Error parsing XML", 1 );
      else
         lpInfo->lpEntityInstance[ lpInfo->nDepth ]->u.nIndicators = lpInfo->nIndicators [ lpInfo->nDepth ];

      // If there is an abstract child then copy the indicators.
      if ( lpInfo->lpAbstractChildEI )
         lpInfo->lpAbstractChildEI->u.nIndicators = lpInfo->nIndicators [ lpInfo->nDepth ];

      // If the entity is an abstract child it may have had a child entity underneath it.
      // If so we need to decrement nDepth because the abstract child takes up two entities.
      if ( lpInfo->lpViewEntity &&
           lpInfo->EntityInfo[ lpInfo->lpViewEntity->nHierNbr ].bFirstUnderAbChild )
      {
         lpInfo->nDepth--;
         lpInfo->EntityInfo[ lpInfo->lpViewEntity->nHierNbr ].bFirstUnderAbChild = 0;
         lpInfo->lpViewEntity = zGETPTR( lpInfo->lpViewEntity->hParent );
      }
   }

   // We're ending an element so depth is now one less.
   lpInfo->nDepth--;
}

void
GetElementData( void       *pvUserData,
                const zCHAR *pchData,
                int        nLth )
{
   LPPARSERINFO lpInfo = (LPPARSERINFO) pvUserData;
   zPCHAR       pchSrc, pchTgt;

   #ifdef TRACE_XML
      TraceBuffer( "(xml) Element data = ", (zPVOID) pchData, nLth );
   #endif

   // Ignore data unless we're reading attribute value data.
   if ( lpInfo->lpViewAttrib == 0 )
      return;

   if ( lpInfo->bFatalError == TRUE )
      return;

   // The attribute value can come in chunks so we have to concatenate all
   // the chunks before we can set the attribute.  Allocate space big enough
   // to hold the current chunk.
   if ( lpInfo->nBufferPhysicalSize < nLth + lpInfo->nBufferCurrentUsed )
   {
      int    nNewSize = nLth + lpInfo->nBufferCurrentUsed + 5000;
      zPCHAR pchNewBuffer;

      // Allocate a new chunk.
      pchNewBuffer = malloc( nNewSize );

      // If there was an old buffer then copy data to new buffer.
      if ( lpInfo->pchAttributeBuffer )
      {
         zmemcpy( pchNewBuffer, lpInfo->pchAttributeBuffer,
                  lpInfo->nBufferCurrentUsed );

         free( lpInfo->pchAttributeBuffer );
      }

      lpInfo->pchAttributeBuffer  = pchNewBuffer;
      lpInfo->nBufferPhysicalSize = nNewSize;
   }

   // Append new data to the buffer.  Do it one char at a time so we can
   // convert special chars.
   pchTgt = &lpInfo->pchAttributeBuffer[ lpInfo->nBufferCurrentUsed ];
   for ( pchSrc = (zPCHAR) pchData;
         pchSrc != (zPCHAR) pchData + nLth;
         pchSrc++ )
   {
      // We need to convert chars from their UTF-8 representation back to
      // their ASCII representation.  For char with hex value 0 - 7f it's
      // just a straight translation. Other chars need something special.
      if ( (zUCHAR) *pchSrc == 194 )
      {
         // Next byte after 194 represents char.
         *pchTgt++ = *++pchSrc;
      }
      else
      if ( (zUCHAR) *pchSrc == 195 )
      {
         // Next byte after 194 represents char.
         *pchTgt++ = *++pchSrc + 64;
      }
      else
         *pchTgt++ = *pchSrc;

      lpInfo->nBufferCurrentUsed++;
   }
}

int
unknownEncodingHandler( zPVOID         lpPtr,
                        const XML_Char *Name,
                        XML_Encoding   *info )
{
   int  k;

   zmemset( info, 0, sizeof( XML_Encoding ) );
   for ( k = 0; k < 256; k++ )
      info->map[ k ] = k;

   return( 1 );
}

//./ ADD NAME=ActivateOI_FromXML
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       ActivateOI_FromXML
//
//  PURPOSE:     Activates on OI from an XML data stream.
//
//  PARAMETERS: lppView       - View returned to instance activated
//              pchOD_Name    - Name of the view object.  If 0, the
//                              returned view is for the same object as
//                              the initial view(?).
//              vSubtask      - Used to qualify the application in which
//                              to look for the object.  If the view
//                              passed is a subtask view, then the
//                              application that is used is the
//                              application tied to the subtask.
//                              Otherwise, the application for the view
//                              object associated with the passed view
//                              is used.
//              lControl      - Controls certain aspects of the file
//                              loaded.
//                            zSINGLE   - (default) only one entity at the
//                                        root level.
//                            zMULTIPLE - multiple entities at the root
//                                        level
//                            zLEVEL_APPLICATION - created view is tied
//                                        to the application task,
//                                        rather than the active task.
//                            zLEVEL_SYSTEM   - created view is tied
//                                        with the system task,
//                                        rather than the active task.
//                            zLEVEL_SAME - created view is tied to the
//                                        same task as lpAppQualView.
//                                        Note: specifying this option
//                                        requires that lpAppQualView be
//                                        be specified and that it not be
//                                        a subtask view.
//                            zNOI_OKAY - when set, if the file could not
//                                        be found, no error message is
//                                        issued, just return code set.
//                            zREADONLY - Marks the View and the Instance
//                                        as read only.  Any view created
//                                        from this view will also be
//                                        marked as read only.
//                            zIGNORE_ERRORS - Loads what info it can if
//                                        the file is ASCII
//              pfnRead        - Stream reader function.  Called by Core to
//                               read XML data.
//              lpvData        - Data pointer that is passed throught to
//                               the XML reader function.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zSHORT OPERATION
ActivateOI_FromXML( zPVIEW       lppView,
                    zPCHAR       pchOD_Name,
                    zVIEW        vSubtask,
                    zLONG        lControl,
                    LPFN_XMLREAD pfnRead,
                    zPVOID       lpPtr )
{
   zPCHAR           pchBuffer;
   zULONG           ulLth;
   zLONG            l;
   zBOOL            bSystemObj;
   LPVIEWOD         lpViewOD;
   int              bDone      = FALSE;
   XML_Parser       parser     = XML_ParserCreate( 0 );
   ParserInfoRecord ParserInfo = { 0 };
   zSHORT           nRC        = 0;

   bSystemObj = (lControl & zACTIVATE_SYSTEM) ? TRUE : FALSE;

   // Verify only one zLEVEL_ option requested
   l = lControl & (zLEVEL_APPLICATION | zLEVEL_SYSTEM | zLEVEL_SAME);
   if ( l && (-l & l) != l )
   {
      // error, More than one zLEVEL_ option requested
      // "KZOEE023 - Invalid parameter, "
      fnIssueCoreError( zGETPTR( vSubtask->hTask ), vSubtask, 8, 23, 0,
                        "More than one zLEVEL_ option requested", 0 );
      return( zCALL_ERROR );
   }

   // If zLEVEL_SAME is requested
   if ( lControl & zLEVEL_SAME )
   {
      if ( vSubtask == 0 || vSubtask->hSubtask )
      {
         // "KZOEE100 - Invalid View, view is a Subtask View"
         fnIssueCoreError( zGETPTR( vSubtask->hTask ), vSubtask, 8, 100, 0,
                           "vSubtask", "with zLEVEL_SAME" );
         return( zCALL_ERROR );
      }
      else
      {
         LPVIEWCSR lpViewCsr;

         // Turn off the 'same' indicator
         lControl ^= zLEVEL_SAME;
         // To determine what the level is of the qualifying view
         //   1. Change vSubtask to point to the initial
         //      view created for the object instance
         //   2. Check for application level, if there use
         //      zLEVEL_APPLICATION.
         //   3. Check if the view task is the main task, if so use
         //      zLEVEL_SYSTEM.
         //   4. Use the task level...
         lpViewCsr = zGETPTR( vSubtask->hViewCsr );
         while ( lpViewCsr->hNextViewCsr )
            lpViewCsr = zGETPTR( lpViewCsr->hNextViewCsr );

         vSubtask = zGETPTR( lpViewCsr->hView );
         if ( vSubtask->bApplicationView )
            lControl |= zLEVEL_APPLICATION;
         else
         if ( vSubtask->hTask == AnchorBlock->hMainTask )
            lControl |= zLEVEL_SYSTEM;
      }
   }

   // Make sure request is for valid OD.
   lpViewOD = ActivateViewObject( vSubtask, pchOD_Name, bSystemObj );
   if ( lpViewOD == 0 )
      return( zCALL_ERROR );

   ParserInfo.vSubtask = vSubtask;
   ParserInfo.nDepth   = 0;

   if ( pchOD_Name && *pchOD_Name )
   {
      ParserInfo.lpViewOD = ActivateViewObject( vSubtask, pchOD_Name, 0 );
      if ( ParserInfo.lpViewOD == 0 )
         return( zCALL_ERROR );
   }

   XML_SetUserData( parser, &ParserInfo );
   XML_SetElementHandler( parser, startElement, endElement );
   XML_SetCharacterDataHandler( parser, GetElementData );
   XML_SetUnknownEncodingHandler( parser, unknownEncodingHandler, 0 );

   while ( bDone == FALSE )
   {
      bDone = (int) (*pfnRead)( *lppView, &pchBuffer, &ulLth, lpPtr );
      if ( bDone )
         pchBuffer[ ulLth ] = 0;

      if ( !XML_Parse( parser, pchBuffer, ulLth, bDone ) )
      {
         zCHAR szMsg[ 1000 ];

         sprintf_s( szMsg, sizeof( szMsg ), "%s at line %d",
                   XML_ErrorString( XML_GetErrorCode( parser ) ),
                   XML_GetCurrentLineNumber( parser ) );
         TraceLineS( "(xml) Error txt: ", szMsg );

         SysMessageBox( *lppView, "Error", "Error parsing XML", 1 );
         nRC = zCALL_ERROR;
         break;
      }
   }

   XML_ParserFree( parser );

   if ( ParserInfo.pchAttributeBuffer )
      free( ParserInfo.pchAttributeBuffer );

   if ( ParserInfo.bFatalError )
      nRC = zCALL_ERROR;

   if ( nRC == zCALL_ERROR )
   {
      if ( ParserInfo.vOI )
         fnDropView( ParserInfo.vOI );

      *lppView = 0;
   }
   else
   {
      *lppView = ParserInfo.vOI;
      fnResetView( *lppView, 0 );
   }

   return( nRC );

} // ActivateOI_FromXML

typedef struct _XMLData
{
   zCHAR szReadBuffer[ 5000 ];
   zLONG hFile;
} XMLDATA, * PXMLDATA;

/*
   Reads XML data from a stream.

   Parameters:
      ppchReturnBuffer - A return pointer that is changed to point to the
                         begining of the return data.
      pulReturnLth     - The length of the data in the return buffer.
      lpData           - Pass through data pointer.  This is the pvData pointer
                         that is passed into ActivateOI_FromXML( ).

   Returns:
      TRUE  - End of file (no more data)
      FALSE - More data.

*/
zSHORT LOCALOPER
fnReadXMLData( zVIEW   lpTaskView,
               zPPCHAR ppchReturnBuffer,
               zPULONG pulReturnLth,
               zPVOID  lpData )
{
   PXMLDATA pInfo = (PXMLDATA) lpData;

   *ppchReturnBuffer = pInfo->szReadBuffer;
   *pulReturnLth = SysReadFile( lpTaskView, pInfo->hFile,
                                pInfo->szReadBuffer,
                                sizeof( pInfo->szReadBuffer ) );
   if ( *pulReturnLth < sizeof( pInfo->szReadBuffer ) )
      return( TRUE );   // Nothing more to read.
   else
      return( FALSE );  // Still more info in file.
}

zSHORT OPERATION
ActivateOI_FromXML_File( zPVIEW  pvOI,
                         zCPCHAR pchOD_Name,
                         zVIEW   vSubtask,            /* Subtask qual */
                         zCPCHAR pchFileName,
                         zLONG   lControl )           /* Control Value */
{
   LPTASK  lpCurrentTask;
   XMLDATA XMLData;
   zSHORT  nRC = zCALL_ERROR;

   // If task not active or disabled, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iActivateOI_FromXML_File,
                                          vSubtask, 0 )) == 0 )
   {
      return( zCALL_ERROR );
   }

   if ( (vSubtask || (lControl & zACTIVATE_SYSTEM) == 0) &&
        fnValidView( lpCurrentTask, vSubtask ) == 0 )
   {
      fnOperationReturn( iActivateOI_FromXML_File, lpCurrentTask );
      return( zCALL_ERROR );
   }

   XMLData.hFile = SysOpenFile( vSubtask, pchFileName, COREFILE_READ );
   if ( XMLData.hFile == -1 )
   {
      //  "KZOEE071 - Error opening instance file "
      fnIssueCoreError( lpCurrentTask, vSubtask, 16, 71, 0, pchFileName, 0 );
   }
   else
   {
      nRC = ActivateOI_FromXML( pvOI, (zPCHAR) pchOD_Name, vSubtask, lControl,
                                fnReadXMLData, (zPVOID) &XMLData );

      SysCloseFile( vSubtask, XMLData.hFile, 0 );
   }

   fnOperationReturn( iActivateOI_FromXML_File, lpCurrentTask );
   return( nRC );
}

zSHORT LOCALOPER
fnWriteXML_ToFile( zVIEW  lpTaskView,
                   zULONG lControl,
                   zPCHAR pchText,
                   zULONG ulLth,
                   zSHORT nLevel,
                   zPVOID lpData )
{
   zLONG hFile = *((zPLONG) lpData);

// if ( lControl & zXML_STARTENTITY )  // too many blank lines as far as I can tell ... dks 2009.11.30
//    SysWriteLine( lpTaskView, hFile, "" );

   // Some XML elements will be indented.
   if ( lControl & (zXML_STARTATTRIB | zXML_STARTENTITY | zXML_ENDENTITY) )
   {
      if ( nLevel > 0 )
      {
         zCHAR  szBuffer[ 500 ];
         zSHORT nIndent = nLevel * 3 - 2;

         sprintf_s( szBuffer, sizeof( szBuffer ), "%*s ", nIndent, " " );
         SysWriteLineLth( lpTaskView, hFile, szBuffer, zstrlen( szBuffer ), FALSE );
      }
   }

   if ( lControl & (zXML_STARTATTRIB | zXML_ATTRIBVALUE) )
      SysWriteLineLth( lpTaskView, hFile, pchText, zstrlen( pchText ), FALSE );
   else
      SysWriteLine( lpTaskView, hFile, pchText );

   return( 0 );
}

zSHORT OPERATION
CommitOI_ToXML_File( zVIEW   vOI,
                     zCPCHAR pchFileName,
                     zLONG   nFlags )
{
   LPTASK lpCurrentTask;
   zCHAR  szOpenFileName[ zMAX_FILENAME_LTH + 1 ];
   zLONG  hFile;
   zSHORT nRC = zCALL_ERROR;

   if ( vOI == 0 )
   {
      TraceLineS( "Invalid view for CommitOI_ToXML_File: ", pchFileName );
      return( nRC );
   }

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCommitOI_ToXML_File, vOI,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // If the file name is not qualfied, use the object dir for the application.
   if ( zstrchr( pchFileName, cDirSep ) == 0 )
   {
      LPAPP lpApp;

      fnGetApplicationForSubtask( &lpApp, vOI );
      if ( lpApp )
         strcpy_s( szOpenFileName, sizeof( szOpenFileName ), lpApp->szObjectDir );
      else
      {
         lpApp = zGETPTR( AnchorBlock->hSystemApp );
         strcpy_s( szOpenFileName, sizeof( szOpenFileName ), lpApp->szObjectDir );
      }

      SysAppendcDirSep( szOpenFileName );
   }
   else
      szOpenFileName[ 0 ] = 0;

   strcat_s( szOpenFileName, sizeof( szOpenFileName ), pchFileName );

   hFile = SysOpenFile( vOI, szOpenFileName, COREFILE_WRITE );
   if ( hFile == -1 )
   {
      //  "KZOEE071 - Error opening instance file "
      fnIssueCoreError( lpCurrentTask, vOI, 16, 71, 0, pchFileName, 0 );
   }
   else
   {
      nRC = WriteOI_ToXML( vOI, nFlags, fnWriteXML_ToFile, (zPVOID) &hFile );
      SysCloseFile( vOI, hFile, 0 );
   }

   fnOperationReturn( iCommitOI_ToXML_File, lpCurrentTask );
   return( nRC );
}

zSHORT OPERATION
CommitSubobjectToXML_File( zVIEW   vOI,
                           zCPCHAR pchFileName,
                           zCPCHAR pchSubobjectRoot,
                           zLONG   nFlags )
{
   LPTASK lpCurrentTask;
   zCHAR  szOpenFileName[ zMAX_FILENAME_LTH + 1 ];
   zLONG  hFile;
   zSHORT nRC = zCALL_ERROR;

   // If task not active or disabled, or view invalid, return zCALL_ERROR.
   if ( (lpCurrentTask = fnOperationCall( iCommitSubobjectToXML_File, vOI,
                                          zVALID_VIEW )) == 0 )
   {
      return( zCALL_ERROR );
   }

   // If the file name is not qualfied, use the object dir for the application.
   if ( !zstrchr( (zPCHAR) pchFileName, cDirSep ) )
   {
      LPAPP lpApp;

      fnGetApplicationForSubtask( &lpApp, vOI );
      if ( lpApp )
         strcpy_s( szOpenFileName, sizeof( szOpenFileName ), lpApp->szObjectDir );
      else
      {
         lpApp = zGETPTR( AnchorBlock->hSystemApp );
         strcpy_s( szOpenFileName, sizeof( szOpenFileName ), lpApp->szObjectDir );
      }

      SysAppendcDirSep( szOpenFileName );
   }
   else
      szOpenFileName[ 0 ] = 0;

   strcat_s( szOpenFileName, sizeof( szOpenFileName ), pchFileName );

   hFile = SysOpenFile( vOI, szOpenFileName, COREFILE_WRITE );
   if ( hFile == -1 )
   {
      //  "KZOEE071 - Error opening instance file "
      fnIssueCoreError( lpCurrentTask, vOI, 16, 71, 0, pchFileName, 0 );
   }
   else
   {
      nRC = WriteSubobjectToXML( vOI, pchSubobjectRoot, nFlags,
                                 fnWriteXML_ToFile, (zPVOID) &hFile );
      SysCloseFile( vOI, hFile, 0 );
   }

   fnOperationReturn( iCommitSubobjectToXML_File, lpCurrentTask );
   return( nRC );
}
