/*
MODULE NAME:  TZWRKSTO.H  -  Work Storage Manager
DESCRIPTION:  This module provides methods to handle work storage.
              It is used in the Zeidon Ineterpreter.
OPERATIONS:   WRKS_Init
              WRKS_Close
              WRKS_Reset
              WRKS_Get

*******************************************************************
* Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
* All rights reserved.                                            *
* Confidential and Proprietary material subject to license -      *
* do not reproduce or disclose. This material is an unpublished   *
* work and is considered a trade secret belonging to the          *
* copyright holder.                                               *
*******************************************************************
AUTHOR:  Hartmut Haas
DATE:    2001.08.10

CHANGE LOG

2001.08.10 - 2001.08.10  HH
   new.

**---------------------------------------------------------------------*/
#ifndef _TZWRKSTO_H_
#define _TZWRKSTO_H_

#ifdef __cplusplus
extern "C"
{
#endif


zOPER_EXPORT zLONG OPERATION
WRKS_Init( zPVOID *ppAnchor );

zOPER_EXPORT zVOID OPERATION
WRKS_Close( zPVOID *ppAnchor );

zOPER_EXPORT zVOID OPERATION
WRKS_Reset( zPVOID pAnchor );

zOPER_EXPORT zLONG OPERATION
WRKS_Get( zPVOID pAnchor, zLONG lNeeded, zPVOID *ppData );

#ifdef __cplusplus
}
#endif

#endif

