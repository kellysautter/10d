
/*
MODULE NAME:  KZHVSETA.H  -  Value Set Handling
DESCRIPTION:  This provides methods to remember a nameded set of
              values. It can be used to remember the (key) values
              of an entity.
OPERATIONS:   VSET_Init
              VSET_Free
              VSET_New
              VSET_AddValue
              VSET_Accept
              VSET_Clear
*******************************************************************
* Copyright (c) 2001 QuinSoft Corporation and TONBELLER AG.       *
* All rights reserved.                                            *
* Confidential and Proprietary material subject to license -      *
* do not reproduce or disclose. This material is an unpublished   *
* work and is considered a trade secret belonging to the          *
* copyright holder.                                               *
*******************************************************************
AUTHOR:  Hartmut Haas
DATE:    2001.04.05

CHANGE LOG
----------
2002.02.13  HH
   add keep parameter to VSET_Clear.
2001.04.05 - 2001.05.05  HH
   new.

**---------------------------------------------------------------------*/

#ifndef _KZHVSETA_H_
#define _KZHVSETA_H_

zOPER_EXPORT zLONG OPERATION
VSET_Init( zPVOID *ppAnchor );

zOPER_EXPORT zVOID OPERATION
VSET_Free( zPVOID *ppAnchor );

zOPER_EXPORT zLONG OPERATION
VSET_New( zPVOID *ppAnchor, zCPCHAR szName );

zOPER_EXPORT zLONG OPERATION
VSET_AddValue( zPVOID *ppAnchor, zPVOID pData, zLONG lData );

zOPER_EXPORT zLONG OPERATION
VSET_Accept( zPVOID *ppAnchor );

zOPER_EXPORT zVOID OPERATION
VSET_Delete( zPVOID *ppAnchor, zCHAR *szName, zLONG nKeep );

zOPER_EXPORT zVOID OPERATION
VSET_Clear( zPVOID *ppAnchor, zLONG nKeep );

#endif
