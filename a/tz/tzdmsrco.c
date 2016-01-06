/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzdmsrco.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Gig Kirk
// DATE:          1992/09/30
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2000.01.03  BL
   remove warnings

2000.12.27  BL
   Added oTZDMSRCO_CheckDomainType: if Domain from Type "Format",
   check Context Edit String

from here old version

26.02.1998  DonC
   Modified ofnTZDMSRCO_CopyDomToXDM to set DataType of XDM to DateTime if
   the source DataType is Date or Time.

*/

#define  KZMETA_INCL
#include "tzlodopr.h"
#include "tzvmlip.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


zOPER_EXPORT zSHORT OPERATION
ofnTZDMSRCO_CopyDomToXDM( zVIEW vSubtask, zVIEW vDomain, zVIEW vXDM );

// returns 0 - not unique
//         1 - unique
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_UniqueDomainName( zVIEW vDomain, zVIEW vCM_List )
{
   zCHAR    szDomainName[ 33 ];
   zSHORT   nRC;

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomain, "Domain", "Name" );
   nRC = SetCursorFirstEntityByString( vCM_List, "W_MetaDef", "Name",
                                       szDomainName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   if ( CompareAttributeToAttribute( vCM_List, "W_MetaDef", "CPLR_ZKey",
                                     vDomain, "Domain", "ZKey" ) != 0 )
   {
      return( 0 );
   }

   nRC = SetCursorNextEntityByString( vCM_List, "W_MetaDef", "Name",
                                      szDomainName, "" );
   if ( nRC != zCURSOR_SET )
      return( 1 );

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_RemoveDomainFromXDM( zVIEW vSubtask, zVIEW vDomain )
{
   zVIEW    vXDM;
   zCHAR    szDomainName[ 33 ];
   zLONG    lZKey;
   zSHORT   nRC;

   nRC = oTZDMXGPO_GetViewForXDM( vDomain, &vXDM, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomain, "Domain", "Name" );
   if ( SetCursorFirstEntityByString( vXDM, "Domain", "Name",
                                      szDomainName, "" ) != zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lZKey, vXDM, "Domain", "ZKey" );
      if ( SetCursorFirstEntityByInteger( vXDM, "Domain", "ZKey",
                                          lZKey, "" ) == zCURSOR_SET )
      {
         return( -1 );
      }
   }

   DeleteEntity( vXDM, "Domain", zREPOS_NONE );

   nRC = oTZDMXGPO_CommitXDM( vSubtask, vXDM );
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_ReplaceOrAddDomToXDM( zVIEW vSubtask, zVIEW vDomain, zSHORT bCommit )
{
   zVIEW    vXDM;
   zCHAR    szDomainName[ 33 ];
   zLONG    lZKey;
   zSHORT   nRC;

   nRC = oTZDMXGPO_GetViewForXDM( vDomain, &vXDM, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vDomain, "Domain", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXDM, "Domain", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      nRC = DeleteEntity( vXDM, "Domain", zREPOS_PREV );
   }

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomain, "Domain", "Name" );
   if ( SetCursorFirstEntityByString( vXDM, "Domain", "Name",
                                      szDomainName, "" ) == zCURSOR_SET )
   {
      nRC = DeleteEntity( vXDM, "Domain", zREPOS_PREV );
   }

   ofnTZDMSRCO_CopyDomToXDM( vSubtask, vDomain, vXDM );

   if ( bCommit )
      nRC = oTZDMXGPO_CommitXDM( vSubtask, vXDM );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_AddDomToXDM( zVIEW vSubtask, zVIEW vDomain )
{
   zVIEW    vXDM;
   zSHORT   nRC;
   zCHAR    szDomainName[ 33 ];
   zLONG    lZKey;

   nRC = oTZDMXGPO_GetViewForXDM( vDomain, &vXDM, zCURRENT_OI );
   if ( nRC < 0 )
      return( nRC );

   GetIntegerFromAttribute( &lZKey, vDomain, "Domain", "ZKey" );
   if ( SetCursorFirstEntityByInteger( vXDM, "Domain", "ZKey",
                                       lZKey, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomain, "Domain", "Name" );
   if ( SetCursorFirstEntityByString( vXDM, "Domain", "Name",
                                      szDomainName, "" ) == zCURSOR_SET )
   {
      return( 1 );
   }

   ofnTZDMSRCO_CopyDomToXDM( vSubtask, vDomain, vXDM );

   nRC = oTZDMXGPO_CommitXDM( vSubtask, vXDM );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
ofnTZDMSRCO_CopyDomToXDM( zVIEW vSubtask, zVIEW vDomain, zVIEW vXDM )
{
   zPCHAR   pchAttr;
   zCHAR    szDataType[ 2 ];
   zSHORT   nRC;

   CreateEntity( vXDM, "Domain", zPOS_FIRST );
   SetMatchingAttributesByName( vXDM, "Domain", vDomain, "Domain", zSET_ALL );

   // If the source Domain.DataType is Date or Time ("D" or "I"), set the
   // target Domain.DataType to DateTime ("T"), since that is the only
   // internal type used.
   GetStringFromAttribute( szDataType, sizeof( szDataType ), vDomain, "Domain", "DataType" );
   if ( *szDataType == 'D' || *szDataType == 'I' )
      SetAttributeFromString( vXDM, "Domain", "DataType", "T" );

   // If there is an operation for the current domain, copy DLL_Name and
   // operation name to the XDM.
   if ( CheckExistenceOfEntity( vDomain, "SelectedOperation" ) == zCURSOR_SET )
   {
      // Now set additional XDM.Domain attributes.
      GetAddrForAttribute( &pchAttr, vDomain, "DomainGroup",
                           "DomainAndGlobalOpGroupName" );
      if ( *pchAttr )
      {
         SetAttributeFromAttribute( vXDM, "Domain", "DLL_Name",
                                    vDomain, "DomainGroup",
                                    "DomainAndGlobalOpGroupName" );
      }
      else
      {
         SetAttributeFromAttribute( vXDM, "Domain", "DLL_Name",
                                    vDomain, "DomainGroup", "Name" );
      }

      SetAttributeFromAttribute( vXDM, "Domain", "OperName",
                                 vDomain, "SelectedOperation", "Name" );
   }

// copy complete structure of the Domain Object to the XDM Group.
   nRC = SetCursorFirstEntity( vDomain, "Context", "" );
   while ( nRC == zCURSOR_SET )
   {
      CreateEntity( vXDM, "Context", zPOS_AFTER );
      SetMatchingAttributesByName( vXDM, "Context",
                                   vDomain, "Context", zSET_ALL );

      nRC = SetCursorFirstEntity( vDomain, "TableEntry", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vXDM, "TableEntry", zPOS_AFTER );
         SetMatchingAttributesByName( vXDM, "TableEntry",
                                      vDomain, "TableEntry", zSET_ALL );

         nRC = SetCursorFirstEntity( vDomain, "ExtValueNLS_Text", "" );
         while ( nRC == zCURSOR_SET )
         {
            CreateEntity( vXDM, "NLS_TableEntry", zPOS_AFTER );
            SetMatchingAttributesByName( vXDM, "NLS_TableEntry",
                                         vDomain, "ExtValueNLS_Text", zSET_ALL );

            nRC = SetCursorNextEntity( vDomain, "ExtValueNLS_Text", "" );
         }

         nRC = SetCursorNextEntity( vDomain, "TableEntry", "" );
      }

      nRC = SetCursorFirstEntity( vDomain, "RegularExpression", "" );
      while ( nRC == zCURSOR_SET )
      {
         CreateEntity( vXDM, "RegularExpression", zPOS_AFTER );
         SetMatchingAttributesByName( vXDM, "RegularExpression",
                                      vDomain, "RegularExpression",
                                      zSET_ALL );

         nRC = SetCursorNextEntity( vDomain, "RegularExpression", "" );
      }

      nRC = SetCursorNextEntity( vDomain, "Context", "" );
   }

   zgSortEntityWithinParent( zASCENDING, vXDM, "Domain", "Name", "" );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDomName
//
// PURPOSE:   Derived function to set Name for Domain in ER, LOD, TE,
//            UIS, Dialog, and XDM.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomName( zVIEW            vMeta,
                         LPVIEWENTITY     lpViewEntity,
                         LPVIEWATTRIB     lpViewAttrib,
                         zSHORT           nMsg )
{
   zLONG  lDomainZKey;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zCHAR  szDomainName[ 33 ];
   zSHORT nRC;

   // Derive Domain Name
   // Just return if TaskLPLR is not there.
   if ( GetViewByName( &vCM_List, "TaskLPLR", vMeta, zLEVEL_TASK ) < 0 )
      return( -1 );

   nRC = GetIntegerFromAttribute( &lDomainZKey, vMeta,
                                  lpViewEntity->szName, "ZKey" );
   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szDomainName, sizeof( szDomainName ), vDomain, "Domain", "Name" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szDomainName, 0 );
   }
   else
   {
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, "UNKNOWN", 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDomDataType
//
// PURPOSE:   Derived function to set DataType for Domain in ER, LOD, TE,
//            UIS, Dialog, and XDM.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomDataType( zVIEW        vMeta,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg )
{
   zLONG  lDomainZKey;
   zVIEW  vDomain;
   zVIEW  vCM_List = 0;
   zCHAR  szDomainDataType[ 33 ];
   zSHORT nRC;

   // Derive Domain Name
   GetIntegerFromAttribute( &lDomainZKey, vMeta, lpViewEntity->szName, "ZKey" );
   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      nRC = GetStringFromAttribute( szDomainDataType, sizeof( szDomainDataType ), vDomain, "Domain", "DataType" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szDomainDataType, 0 );
   }
   else
   {
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, "UNKNOWN", 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDomType
//
// PURPOSE:   Derived function to set Type for Domain in UIS.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomType( zVIEW        vMeta,
                         LPVIEWENTITY lpViewEntity,
                         LPVIEWATTRIB lpViewAttrib,
                         zSHORT       nMsg )
{
   zLONG  lDomainZKey;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zCHAR  szDomainType[ 33 ];
   zSHORT nRC;

   // Derive Domain Type.
   GetIntegerFromAttribute( &lDomainZKey, vMeta, lpViewEntity->szName, "ZKey" );
   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szDomainType, sizeof( szDomainType ), vDomain, "Domain", "DomainType" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szDomainType, 0 );
   }
   else
   {
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, "UNKNOWN", 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDomStringLth
//
// PURPOSE:   Derived function to set Max String Length for Domain in ER, LOD, TE,
//            UIS, Dialog, and XDM.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomStringLth( zVIEW        vMeta,
                              LPVIEWENTITY lpViewEntity,
                              LPVIEWATTRIB lpViewAttrib,
                              zSHORT       nMsg )
{
   zLONG  lDomainZKey;
   zLONG  lDomainMaxStringLth;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zSHORT nRC;

   // Derive Domain String Length
   GetIntegerFromAttribute( &lDomainZKey, vMeta, lpViewEntity->szName, "ZKey" );
   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      nRC = GetIntegerFromAttribute( &lDomainMaxStringLth, vDomain,
                                     "Domain", "MaxStringLth" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib,
                          &lDomainMaxStringLth, 0 );
   }
   else
   {
      StoreStringInRecord( vMeta, lpViewEntity, lpViewAttrib, 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDomDecFormat
//
// PURPOSE:   Derived function to set Decimal Fromat for Domain in ER, LOD,
//            TE, UIS, Dialog, and XDM.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDomDecFormat( zVIEW        vMeta,
                              LPVIEWENTITY lpViewEntity,
                              LPVIEWATTRIB lpViewAttrib,
                              zSHORT       nMsg )
{
   zLONG  lDomainZKey;
   zLONG  lDomainDecimalFormat;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zSHORT nRC;

   // Derive Domain Decimal Format
   GetIntegerFromAttribute( &lDomainZKey, vMeta, lpViewEntity->szName, "ZKey" );
   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey, zCURRENT_OI );
   if ( nRC >= 0 )
   {
      nRC = GetIntegerFromAttribute( &lDomainDecimalFormat, vDomain,
                                     "Domain", "DecimalFormat" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib,
                          &lDomainDecimalFormat, 0 );
   }
   else
   {
      StoreStringInRecord( vMeta, lpViewEntity, lpViewAttrib, 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveContextName
//
// PURPOSE:   Derived function to set Context Name for Domain in ER, LOD, TE,
//            UIS, Dialog, and XDM.
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveContextName( zVIEW        vMeta,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg )
{
   zLONG  lDomainZKey;
   zLONG  lContextZKey;
   zVIEW  vDomain;
   zVIEW  vCM_List;
   zCHAR  szEntityName[ 33 ];
   zCHAR  szContextName[ 33 ];
   zSHORT nRC;

   // Derive Context Name
   GetIntegerFromAttribute( &lContextZKey, vMeta, lpViewEntity->szName, "ZKey" );
   strcpy_s( szEntityName, sizeof( szEntityName ), lpViewEntity->szName );
   if ( zstrcmp( szEntityName, "CtrlMapContext" ) == 0 )
   {
      SetCursorFirstEntity( vMeta, "CtrlMapER_Domain", "" );
      GetIntegerFromAttribute( &lDomainZKey, vMeta,
                               "CtrlMapER_Domain", "ZKey" );
   }
   else
   {
      if ( zstrcmp( szEntityName, "OptMapContext" ) == 0 )
      {
         SetCursorFirstEntity( vMeta, "OptMapER_Domain", "" );
         GetIntegerFromAttribute( &lDomainZKey, vMeta,
                                  "OptMapER_Domain", "ZKey" );
      }
      else
      {
         if ( zstrcmp( szEntityName, "WndCaptionMapContext" ) == 0 )
         {
            SetCursorFirstEntity( vMeta, "WndCaptionMapER_Domain", "" );
            GetIntegerFromAttribute( &lDomainZKey, vMeta,
                                     "WndCaptionMapER_Domain", "ZKey" );
         }
         else
         {
            StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib,
                                "UNKNOWN", 0 );
            return( 0 );
         }
      }
   }

   if ( (nRC = RetrieveViewForMetaList( vMeta, &vCM_List, zREFER_DOMAIN_META )) < 0 )
   {
      return( nRC );
   }

   nRC = ActivateMetaOI_ByZKey( vMeta, &vDomain, vCM_List,
                                zREFER_DOMAIN_META,
                                zSINGLE | zLEVEL_APPLICATION,
                                lDomainZKey,
                                zCURRENT_OI );
   nRC = SetCursorFirstEntityByInteger( vDomain, "Context",
                                        "ZKey", lContextZKey, "" );
   if ( nRC >= 0 )
   {
      GetStringFromAttribute( szContextName, sizeof( szContextName ), vDomain, "Context", "Name" );
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib,
                          szContextName, 0 );
   }
   else
   {
      StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, "UNKNOWN", 0 );
   }

   DropView( vDomain );
   DropView( vCM_List );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveDLL_Name
//
// PURPOSE:
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveDLL_Name( zVIEW        vMeta,
                          LPVIEWENTITY lpViewEntity,
                          LPVIEWATTRIB lpViewAttrib,
                          zSHORT       nMsg )
{
   zVIEW  vTaskLPLR;
   zVIEW  vSource;
   zLONG  lZKey;
   zCHAR  szZKey[ 12 ];
   zCHAR  szDLL_Name[ 34 ];
   zSHORT nRC;

   // Default to null.
   szDLL_Name[ 0 ] = 0;

   if ( CheckExistenceOfEntity( vMeta, "SourceFile" ) == zCURSOR_SET )
   {
      GetStringFromAttribute( szZKey, sizeof( szZKey ), vMeta, "SourceFile", "ZKey" );
      if ( GetViewByName( &vSource, szZKey, vMeta, zLEVEL_TASK ) > 0 )
      {
         if ( CheckExistenceOfEntity( vSource, "ExecComposite" ) >= zCURSOR_SET )
         {
            GetStringFromAttribute( szDLL_Name, sizeof( szDLL_Name ), vSource,
                                    "ExecComposite", "Name" );
         }
      }
      else
      {
         lZKey = zatol( szZKey );
         if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vMeta, zLEVEL_TASK ) > 0 )
         {
            nRC = ActivateMetaOI_ByZKey( vMeta, &vSource, 0,
                                         zREFER_SRC_META,
                                         zSINGLE | zLEVEL_APPLICATION,
                                         lZKey, zCURRENT_OI );
            if ( nRC >= 0 )
            {
               if ( CheckExistenceOfEntity( vSource, "ExecComposite" ) >=
                                                              zCURSOR_SET )
               {
                  GetStringFromAttribute( szDLL_Name, sizeof( szDLL_Name ), vSource,
                                          "ExecComposite", "Name" );
               }

               DropView( vSource );
            }
         }
      }
   }

   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szDLL_Name, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DeriveOperName
//
// PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DeriveOperName( zVIEW            vMeta,
                          LPVIEWENTITY     lpViewEntity,
                          LPVIEWATTRIB     lpViewAttrib,
                          zSHORT           nMsg )
{
   zCHAR  szOperName[ 34 ];

   // Default to NULL
   szOperName[ 0 ] = 0;
   if ( CheckExistenceOfEntity( vMeta, "Operation" ) >= zCURSOR_SET )
      GetStringFromAttribute( szOperName, sizeof( szOperName ), vMeta, "Operation", "Name" );

   StoreValueInRecord( vMeta, lpViewEntity, lpViewAttrib, szOperName, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DIL_Inherited
//
// PURPOSE:   This derived attribute gets the DIL text for a Domain Context
//            by using the description of the Context or Domain.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DIL_Inherited( zVIEW            vIn,
                         LPVIEWENTITY     lpViewEntity,
                         LPVIEWATTRIB     lpViewAttrib,
                         zSHORT           nMsg )
{
   zPCHAR       pchAttr;
   zCHAR        szNull = 0;

   pchAttr = &szNull;
   // If this is a derived retrieval call, then assign the attribute
   // using the derivation formula defined above
   if ( nMsg == zDERIVED_GET )
   {
      GetAddrForAttribute( &pchAttr, vIn, "Context", "Desc" );
      if ( *pchAttr == 0 )    // no desc
      {
         GetAddrForAttribute( &pchAttr, vIn, "Domain", "Desc" );
      }

      // Update the value of the dil message text in the object
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, pchAttr, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// OPERATION: oTZDMSRCO_DIL_TextMsg
//
// PURPOSE:   This derived attribute gets the DIL text by using the 'local'
//            persistent DIL_Text attr or the derived DIL_Inherited attr.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZDMSRCO_DIL_TextMsg( zVIEW            vIn,
                       LPVIEWENTITY     lpViewEntity,
                       LPVIEWATTRIB     lpViewAttrib,
                       zSHORT           nMsg )
{
   zPCHAR       pchAttr;
   zCHAR        szNull = 0;

   pchAttr = &szNull;
   // If this is a derived retrieval call, then assign the attribute
   // using the derivation formula defined above
   if ( nMsg == zDERIVED_GET )
   {
      GetAddrForAttribute( &pchAttr, vIn, "Context", "DIL_Text" );
      if ( *pchAttr == 0 )    // no DIL text local
      {
         GetAddrForAttribute( &pchAttr, vIn, "Context", "DIL_Inherited" );
      }

      // Update the value of the DIL message text in the object
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, pchAttr, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZDMSRCO_CheckDomainType
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*TRANSFORMATION */  OPERATION
oTZDMSRCO_CheckDomainType( zVIEW vMeta  /*  BASED ON LOD TZDGSRCO  */ )
{
   zVIEW  vTZDGSRCO;
   zCHAR  szMsg[ 254 ];
   zCHAR  szTemp[ 33 ];
   zSHORT nRC;

   // if Domain from Type "Format", check Context Edit String

   CreateViewFromViewForTask( &vTZDGSRCO, vMeta, 0 );

   nRC = SetCursorFirstEntityByString( vTZDGSRCO, "Domain", "DomainType", "F", "" );
   while( nRC >= zCURSOR_SET )
   {
      for ( nRC = SetCursorFirstEntity( vTZDGSRCO, "Context", "" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vTZDGSRCO, "Context", "" ) )
      {
         GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZDGSRCO, "Context", "EditString" );
         zGetStringWithoutBlank( szTemp, szTemp );

         if ( zstrcmp( szTemp, "") == 0 )
         {
            strcpy_s( szMsg, sizeof( szMsg ), "Domain '" );
            GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZDGSRCO, "Domain", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), szTemp );
            strcat_s( szMsg, sizeof( szMsg ), "' is from Type 'Format' \nand the Context '");
            GetStringFromAttribute( szTemp, sizeof( szTemp ), vTZDGSRCO, "Context", "Name" );
            strcat_s( szMsg, sizeof( szMsg ), szTemp );
            strcat_s( szMsg, sizeof( szMsg ), "' \ndoes not have a Format/Range String." );

            MessageSend( vMeta, "DM00107", "Domain Maintenance",
                         szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );

            DropView( vTZDGSRCO );
            return( -1 );
         }
      }

      nRC = SetCursorNextEntityByString( vTZDGSRCO, "Domain", "DomainType", "F", "" );
   }

   DropView( vTZDGSRCO );

   return( 0 );
} // oTZDMSRCO_CheckDomainType

///////////////////////////// end of file  //////////////////////////////////
