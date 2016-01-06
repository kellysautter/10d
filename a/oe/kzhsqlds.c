/*

   KZHSQLDS.C - Helper functions needed for QuinSoft DB2 static SQL.

*/
#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "KZHSQLGP.H"
#include "KZHSQLDS.H"
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

// ------------------------------------------------------------------------
//
// Following are global variables declared as "extern" in generated static
// SQL files.
//
// ------------------------------------------------------------------------
struct g_BlobData_t
{
   unsigned long    length;
   zCHAR            data[ 10000 ];
} g_BlobData;

struct g_StringData_t
{
   unsigned long    length;
   zCHAR            data[ 10000 ];
} g_StringData;

long   g_lColumnLth = 0;
zPCHAR g_pvBuffer   = 0;
long   g_lBufferLth = 0;

// Allocates a new buffer if the old buffer is not big enough.
zSHORT LOCALOPER
SDB2_AllocBuffer( zPPCHAR pvBuffer,
                  zPLONG  plLth,
                  zLONG   lNewLth )
{
   zPLONG ph;
   zLONG  h;
   zPCHAR pch;

   // If the requested length is < current length then nothing to do.
   if ( lNewLth <= *plLth )
      return( 0 );

   // Allocate the new buffer.  We have to allocate extra space for the
   // memory handle and the length of the blob.
   h = SysAllocMemory( (zCOREMEM) &pch, lNewLth + (2 * sizeof( zLONG )),
                       0, zCOREMEM_ALLOC, 0 );
   if ( pch == 0 )
      return( zCALL_ERROR );

   // Store the handle at the front of the buffer.
   ph = (zPLONG) pch;
   *ph = h;
   ph++;
   pch = (zPCHAR) ph;

   // We have allocated a new buffer.  Free the old buffer.  If *plLth is 0
   // then there is no old buffer.
   if ( *plLth > 0 )
   {
      // The handle for the buffer is stored at the begining of the buffer.
      ph = (zPLONG) *pvBuffer;
      ph--;
      SysFreeMemory( *ph );
   }

   // Store the new values.
   *plLth = lNewLth;
   *pvBuffer = (zPCHAR) pch;

   return( 0 );
}

zSHORT LOCALOPER
SDB2_FreeBuffer( zPPVOID pvBuffer,
                 long *  plLth )
{
   zPLONG ph;

   if ( plLth && *plLth == 0 )
      return( 0 );

   // The handle for the buffer is stored at the begining of the buffer.
   ph = (zPLONG) *pvBuffer;
   ph--;
   SysFreeMemory( *ph );

   // Null the values.
   *plLth = 0;
   *pvBuffer = 0;

   return( 0 );
}

LPVIEWENTITY LOCALOPER
SDB2_GetViewEntity( LPVIEWOD lpViewOD,
                    zPCHAR   pchEntityName )
{
   LPVIEWENTITY lpViewEntity;

   for ( lpViewEntity = (LPVIEWENTITY) zGETPTR( lpViewOD->hFirstOD_Entity );
         lpViewEntity;
         lpViewEntity = (LPVIEWENTITY) zGETPTR( lpViewEntity->hNextHier ) )
   {
      if ( zstrcmpi( lpViewEntity->szName, pchEntityName ) == 0 )
         return( lpViewEntity );
   }

   return( 0 );
}

LPVIEWATTRIB LOCALOPER
SDB2_GetViewAttrib( LPVIEWENTITY lpViewEntity,
                    zPCHAR       pchAttribName )
{
   LPVIEWATTRIB lpViewAttrib;

   for ( lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
         lpViewAttrib;
         lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib ) )
   {
      if ( zstrcmp( lpViewAttrib->szName, pchAttribName ) == 0 )
         return( lpViewAttrib );
   }

   return( 0 );
}

zSHORT LOCALOPER
SDB2_SetAttributeFromString( zVIEW        lpView,
                             LPVIEWOD     lpViewOD,
                             LPVIEWENTITY lpViewEntity,
                             zPCHAR       pchAttribName,
                             zPCHAR       pchValue,
                             short        nNullIndicator )
{
   LPVIEWATTRIB lpViewAttrib;

   if ( nNullIndicator < 0 )
      return( 0 );

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   return( StoreStringInRecord( lpView, lpViewEntity,
                                lpViewAttrib, pchValue ) );
}

zSHORT LOCALOPER
SDB2_SetAttributeFromInteger( zVIEW        lpView,
                              LPVIEWOD     lpViewOD,
                              LPVIEWENTITY lpViewEntity,
                              zPCHAR       pchAttribName,
                              zLONG        lValue,
                              short        nNullIndicator )
{
   LPVIEWATTRIB lpViewAttrib;

   if ( nNullIndicator < 0 )
      return( 0 );

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   return( StoreValueInRecord( lpView, lpViewEntity, lpViewAttrib,
                               (zPVOID) &lValue, 0 ) );
}

zSHORT LOCALOPER
SDB2_SetAttributeFromBlob( zVIEW        lpView,
                           LPVIEWOD     lpViewOD,
                           LPVIEWENTITY lpViewEntity,
                           zPCHAR       pchAttribName,
                           zPVOID       pvValue,
                           short        nNullIndicator,
                           zULONG       uLth )
{
   LPVIEWATTRIB lpViewAttrib;

   if ( nNullIndicator < 0 )
      return( 0 );

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   return( StoreValueInRecord( lpView, lpViewEntity, lpViewAttrib,
                               pvValue, uLth ) );
}

zSHORT LOCALOPER
SDB2_GetStringFromAttribute( zPCHAR   pchValue,
                             zVIEW    lpView,
                             LPVIEWOD lpViewOD,
                             zPCHAR   pchEntityName,
                             zPCHAR   pchAttribName )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;

   lpViewEntity = SDB2_GetViewEntity( lpViewOD, pchEntityName );
   if ( lpViewEntity == 0 )
      return( zCALL_ERROR );

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   return( GetStringFromRecord( lpView, lpViewEntity, lpViewAttrib,
                                pchValue, 100000 ) );
}

zSHORT LOCALOPER
SDB2_GetIntegerFromAttribute( zPLONG   plValue,
                              zVIEW    lpView,
                              LPVIEWOD lpViewOD,
                              zPCHAR   pchEntityName,
                              zPCHAR   pchAttribName )
{
   zSHORT       nRC;
   zPLONG       plTemp;
   zULONG       lLth;
   zCHAR        chType;
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;

   lpViewEntity = SDB2_GetViewEntity( lpViewOD, pchEntityName );
   if ( lpViewEntity == 0 )
      return( zCALL_ERROR );

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   nRC = GetValueFromRecord( lpView, lpViewEntity, lpViewAttrib,
                             (zCOREMEM) &plTemp, &chType, &lLth );
   *plValue = *plTemp;
   return( nRC );
}

zSHORT LOCALOPER
SDB2_GetStringFromQualification( zPCHAR       pchValue,
                                 LPVIEWENTITY lpViewEntity,
                                 zPCHAR       pchAttribName,
                                 LPQUALENTITY lpQualEntity )
{
   LPVIEWATTRIB lpViewAttrib;
   LPQUALATTRIB lpQualAttrib;

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   if ( lpQualEntity->bContainsIS )
   {
      // For an "IS" clause we should be able to get the value from a source
      // view.

      // Find the qual attrib that contains the 'IS' clause.
      for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
            zstrcmpi( lpQualAttrib->szOper, "IS" ) != 0;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         ; // Nothing needs to be done here.
      }

      return( GetStringFromRecord( lpQualAttrib->lpSourceView,
                                   lpQualAttrib->lpSourceViewEntity,
                                   lpViewAttrib, pchValue, 100000 ) );
   }
   else
   {
      // Find the qual attrib that matches lpViewAttrib.
      for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
            lpQualAttrib->lpViewAttrib != lpViewAttrib;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         ; // Nothing needs to be done here.
      }

      strcpy_s( pchValue, lpQualAttrib->szValue );
      return( 0 );
   }
}

zSHORT LOCALOPER
SDB2_GetIntegerFromQualification( zPLONG       plValue,
                                  LPVIEWENTITY lpViewEntity,
                                  zPCHAR       pchAttribName,
                                  LPQUALENTITY lpQualEntity )
{
   LPVIEWATTRIB lpViewAttrib;
   LPQUALATTRIB lpQualAttrib;

   lpViewAttrib = SDB2_GetViewAttrib( lpViewEntity, pchAttribName );
   if ( lpViewAttrib == 0 )
      return( zCALL_ERROR );

   if ( lpQualEntity->bContainsIS )
   {
      zULONG       lLth;
      zCHAR        chType;

      // Find the qual attrib that contains the 'IS' clause.
      for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
            zstrcmpi( lpQualAttrib->szOper, "IS" ) != 0;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         ; // Nothing needs to be done here.
      }

      // For an "IS" clause we should be able to get the value from a source
      // view.
      return( GetValueFromRecord( lpQualAttrib->lpSourceView,
                                  lpQualAttrib->lpSourceViewEntity,
                                  lpViewAttrib,
                                  (zCOREMEM) plValue, &chType, &lLth ) );
   }
   else
   {
      // Find the qual attrib that matches lpViewAttrib.
      for ( lpQualAttrib = lpQualEntity->lpFirstQualAttrib;
            lpQualAttrib->lpViewAttrib != lpViewAttrib;
            lpQualAttrib = lpQualAttrib->lpNextQualAttrib )
      {
         ; // Nothing needs to be done here.
      }

      *plValue = zatol( lpQualAttrib->szValue );
      return( 0 );
   }

}

#ifdef __cplusplus
}
#endif
