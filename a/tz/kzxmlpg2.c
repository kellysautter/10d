/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   kzxmlpg2.c -
// DESCRIPTION:   Added to provide external interface to KZXMLPGO operations.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        ??
// DATE:          ??
// API:           ??
// ENVIRONMENT:   ??
// REVISION:      ??
//
// HISTORY:
//

/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2003.06.21    DonC
   Created
*/

#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
InitWebSessionObject( zPVIEW  pvKZXMLPGO,
                      zVIEW   AnyView,
                      zCPCHAR cpcUserID )
{
   zSHORT nRC;

   // Simply call the object operation by the same name.
   nRC = oKZXMLPGO_InitWebSessionObject( pvKZXMLPGO, AnyView, cpcUserID );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
InitWebPage( zVIEW   vKZXMLPGO,
             zCPCHAR cpcDialogName,
             zCPCHAR cpcWindowName )
{
   zSHORT nRC;

   // Simply call the object operation by the same name.
   nRC = oKZXMLPGO_InitWebPage( vKZXMLPGO, cpcDialogName, cpcWindowName );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
AddWebControlSubobject( zVIEW      vKZXMLPGO,
                        zCPCHAR    cpcControlName,
                        zCPCHAR    cpcControlType,
                        zCPCHAR    cpcControlContext,
                        zCPCHAR    cpcSourceViewName,
                        zCPCHAR    cpcSourceEntityName,
                        zCPCHAR    cpcSourceAttributeName,
                        zCPCHAR    cpcAutoComboBoxValue )
{
   zSHORT nRC;

   // Simply call the object operation by the same name.
   nRC = oKZXMLPGO_AddWebControlSubobject( vKZXMLPGO, cpcControlName,
                                           cpcControlType,
                                           cpcControlContext,
                                           cpcSourceViewName,
                                           cpcSourceEntityName,
                                           cpcSourceAttributeName,
                                           cpcAutoComboBoxValue );
   return( nRC );
}
