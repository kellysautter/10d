/*
MODULE NAME:  TZWRKSTO.C  -  Work Storage Manager
DESCRIPTION:  This module provides methods to handle work storage.
              It is used in the Zeidon Ineterpreter.
OPERATIONS:   WRKS_Init
              WRKS_Close
              WRKS_Reset
              WRKS_Get

///////////////////////////////////////////////////////////////////
// Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
// All rights reserved.                                            *
// Confidential and Proprietary material subject to license -      *
// do not reproduce or disclose. This material is an unpublished   *
// work and is considered a trade secret belonging to the          *
// copyright holder.                                               *
///////////////////////////////////////////////////////////////////
AUTHOR:  Hartmut Haas
DATE:    2001.08.10

CHANGE LOG

2001.08.10 - 2001.08.10  HH
   new.

//---------------------------------------------------------------------*/

#include <stdlib.h>
#define  KZSYSSVC_INCL
#define  KZMETA_INCL
#include "kzoengaa.h"
#include "tzwrksto.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define WRKS_INIT_SIZE 4096

typedef struct t_WrksAnchor T_WRKSANCHOR;
typedef struct t_MemBlock   T_MEMBLOCK;

struct t_MemBlock
{
   T_MEMBLOCK *pNext;       // chaining
   zCHAR      *pData;       // data area
   zLONG      lData;        // length of data area
   zLONG      lDataUsed;    // length used
};

struct t_WrksAnchor
{
   T_MEMBLOCK *pFirstBlock;    // start of chain
};

////////////////////////////////////////////////
// exported functions
////////////////////////////////////////////////

////////////////////////////////////////////////
//
//  >>>WRKS_Init
//
// PURPOSE:     Initialize environment
//
// DESCRIPTION: allocate the anchor.
//
// RETURNS:     0 ==> OK
//             -1 ==> error
//
////////////////////////////////////////////////

zOPER_EXPORT zLONG OPERATION
WRKS_Init( zPVOID *ppAnchor )
{
   T_WRKSANCHOR *pAnchor;

   pAnchor = SysMalloc( sizeof( T_WRKSANCHOR ) );
   if ( pAnchor == NULL )
      return( -1 );

   pAnchor->pFirstBlock = SysMalloc( sizeof( T_MEMBLOCK ) );
   if ( pAnchor->pFirstBlock == NULL )
   {
      SysFree( pAnchor );
      return( -1 );
   }

   pAnchor->pFirstBlock->pData = SysMalloc( WRKS_INIT_SIZE );
   if ( pAnchor->pFirstBlock->pData == NULL )
   {
      SysFree( pAnchor->pFirstBlock );
      SysFree( pAnchor );
      return( -1 );
   }

   pAnchor->pFirstBlock->lData = WRKS_INIT_SIZE;
   pAnchor->pFirstBlock->lDataUsed = 0;

   *ppAnchor = pAnchor;
   return( 0 );
}

////////////////////////////////////////////////
//
//  >>>WRKS_Close
//
// PURPOSE:     Free environment
//
// DESCRIPTION: Free all memory.
//
// RETURNS:     void
//
////////////////////////////////////////////////

zOPER_EXPORT zVOID OPERATION
WRKS_Close( zPVOID *ppAnchor )
{
   T_WRKSANCHOR *pAnchor = *(T_WRKSANCHOR **)ppAnchor;
   T_MEMBLOCK *pBlock = pAnchor->pFirstBlock;

   // free the chain of blocks
   while ( pBlock )
   {
      T_MEMBLOCK *pNextBlock = pBlock->pNext;
      if ( pBlock->pData )
         SysFree( pBlock->pData );
      // free the block struct itself
      SysFree( pBlock );
      pBlock = pNextBlock;
   }

   // free anchor struct
   SysFree( pAnchor );
   *ppAnchor = NULL;
}

////////////////////////////////////////////////
//
//  >>>WRKS_Reset
//
// PURPOSE:     Reset
//
// DESCRIPTION: keep all blocks, but set the unused
//
// RETURNS:     void
//
////////////////////////////////////////////////

zOPER_EXPORT zVOID OPERATION
WRKS_Reset( zPVOID pAnchor )
{
   T_MEMBLOCK *pBlock = ((T_WRKSANCHOR*)pAnchor)->pFirstBlock;

   // go through chain of blocks, set unused
   while ( pBlock )
   {
      memset( pBlock->pData, 0, pBlock->lData );
      pBlock->lDataUsed = 0;
      pBlock = pBlock->pNext;
   }
}

////////////////////////////////////////////////
//
//  >>>WRKS_Get
//
// PURPOSE:     Get Workspace Area
//
// DESCRIPTION:
// go through chain, take first area which is big enough
// if nothing found, allocate a new block
//
// PARAMETER:
//    zPVOID pAnchor = Environment anchor
//    zLONG  lNeeded = lenght of data needed
//    zPVOID *ppData = Return data area
//
// RETURNS:     zLONG
//              0  ==> ok
//              -1 ==> Error
//
////////////////////////////////////////////////

zOPER_EXPORT zLONG OPERATION
WRKS_Get( zPVOID pAnchor, zLONG lNeeded, zPVOID *ppData )
{
   T_MEMBLOCK *pBlock = ((T_WRKSANCHOR *) pAnchor)->pFirstBlock;
   T_MEMBLOCK *pLastBlock = NULL;
   zLONG      lAlloc;
   zPCHAR     pData;

   // go through chain of blocks
   while ( pBlock )
   {
      zLONG lFree = pBlock->lData - pBlock->lDataUsed;
      if ( lFree >= lNeeded )
      {
         // ok, we found a slot big enough
         *ppData = (pBlock->pData + pBlock->lDataUsed);
         pBlock->lDataUsed += lNeeded;
         return( 0 );
      }

      pLastBlock = pBlock;
      pBlock = pBlock->pNext;
   }

   // we did not find a fitting slot, allocate a new block
   lAlloc = WRKS_INIT_SIZE;
   if ( lNeeded * 2 > lAlloc )
      lAlloc = lNeeded * 2;

   if ( (pBlock = SysMalloc( sizeof( T_MEMBLOCK ) )) == NULL )
   {
      // Error allocating memory
      *ppData = NULL;
      return( -1 );
   }

   if ( (pData = SysMalloc( lAlloc )) == NULL )
   {
      // Error allocating memory
      SysFree( pBlock );
      *ppData = NULL;
      return( -1 );
   }

   // add to chain
   pLastBlock->pNext = pBlock;
   pBlock->pData = pData;
   pBlock->lData = lAlloc;
   pBlock->lDataUsed = lNeeded;
   *ppData = pData;
   return( 1 );
}

#ifdef __cplusplus
}
#endif
