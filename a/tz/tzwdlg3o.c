/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzwdlg3o.c - Zeidon application operations
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
// AUTHOR:        Don Christensen
// DATE:          1995/10/21
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:
// LAST MESSAGE ID: none
//
//
/////////////////////////////////////////////////////////////////////////////
//
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

/////////////////////////////////////////////////////////////////////////////
// OPERATION: oTZWDLGSO_DIL_TextMsg    // THIS MUST STAY AS TZWDLGSO UNTIL
//                                     // WE CAN CHANGE THE SOURCE FILE
//                                     // FOR A LOD OPERATION (I DON'T
//                                     // KNOW HOW ... DKS 9-9-1995)
// PURPOSE:   This derived attribute gets the DIL text by using the 'local'
//            persistent DIL_Text attr or the derived DIL_Inherited attr.
//            NOTE: The operation uses the name of the 'invoking' entity
//                  in order to be used by both LOD_Attribute and
//                  LOD_AttributeRec.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_DIL_TextMsg( zVIEW            vIn,
                       LPVIEWENTITY     lpViewEntity,
                       LPVIEWATTRIB     lpViewAttrib,
                       zSHORT           nMsg )
{
   zPCHAR       szAttr;
   zCHAR        szNull = 0;

   szAttr = &szNull;
   // This is currently a no-op.  The operation and derived attribute should
   // be removed when no portable files exist that contain occurrences of the
   // attribute value.
   if ( nMsg == zDERIVED_GET )
   {
      // Null attribute value.
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, "", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: oTZWDLGSO_DIL_Inherited_Ctrl
// PURPOSE:   This derived attribute determines the DIL message by the following
//            algorithm.
//            1. If the control is a Radio or Push button, the Control Text
//               attribute is used.
//            2. If there is no mapping, the Control Text attribute is used.
//            3. If there is mapping, we will use the Domain's Context to
//               determine the DIL Text.  If there is a Context specified for
//               the Control, that Context will be used.  Otherwise the default
//               Context will be used.
//            NOTE: The operation is designed for Control only.
//                  It will not handle CtrlCtrl, Option, or OptOpt.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_DIL_Inherited_Ctrl( zVIEW         vIn,
                              LPVIEWENTITY  lpViewEntity,
                              LPVIEWATTRIB  lpViewAttrib,
                              zSHORT        nMsg )
{
   zPCHAR       szAttr;
   zVIEW        vDomain;
   zLONG        lZKey;

   // If this is a derived retrieval call, then assign the attribute
   // using the derivation formula defined above
   if ( nMsg == zDERIVED_GET )
   {
      GetAddrForAttribute( &szAttr, vIn, "ControlDef", "Tag" );
      if ( strcmp( szAttr, "Radio" ) == 0 || strcmp( szAttr, "Push" ) == 0 )
      {
         GetAddrForAttribute( &szAttr, vIn, "Control", "Text" );
      }
      else
      {
         if ( CheckExistenceOfEntity( vIn, "CtrlMapER_Domain" )
                                                            < zCURSOR_SET )
         {
            // no mapping so use text of control
            GetAddrForAttribute( &szAttr, vIn, "Control", "Text" );
         }
         else
         {
            // OK, This is where we use the mapping
            // Context/Domain info to determine what the DIL needs to be.

            // First get a view to the Domain.
            GetIntegerFromAttribute( &lZKey, vIn, "CtrlMapER_Domain", "ZKey" );
            ActivateMetaOI_ByZKey( vIn, &vDomain, 0,
                                   zREFER_DOMAIN_META, zSINGLE,
                                   lZKey, zCURRENT_OI );
            if ( CheckExistenceOfEntity( vIn, "CtrlMapContext" )
                                                            >= zCURSOR_SET )
            {
               // If there is a Context override, position on that Context.
               GetIntegerFromAttribute( &lZKey, vIn, "CtrlMapContext", "ZKey" );
               SetCursorFirstEntityByInteger( vDomain, "Context", "ZKey", lZKey, 0 );
            }
            else
               // If there is no Context override, position on the default
               // Context.
               SetCursorFirstEntityByString( vDomain, "Context", "IsDefault", "Y", 0 );

            // Now retrieve the DIL Message from the Context.
            GetAddrForAttribute( &szAttr, vDomain, "Context", "DIL_NLS_Text" );
            if ( szAttr[ 0 ] == 0 )
            {
               GetAddrForAttribute( &szAttr, vDomain, "Context",
                                    "DIL_Inherited" );
            }

            DropView( vDomain );
         }
      }

      // Update the value of the DIL message text in the object
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, szAttr, 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: oTZWDLGSO_DIL_Inherited_Opt
// PURPOSE:   Temporary only.  To be deleted..
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_DIL_Inherited_Opt( zVIEW        vIn,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg )
{
   // This operation is temporary and should be deleted along with the derived
   // attribute when no portable files include a value for this attribute.
   if ( nMsg == zDERIVED_GET )
   {
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, "", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// OPERATION: oTZWDLGSO_DIL_Inherited_LOD
// PURPOSE:   Temporary.  To be deleted.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
oTZWDLGSO_DIL_Inherited_LOD( zVIEW        vIn,
                             LPVIEWENTITY lpViewEntity,
                             LPVIEWATTRIB lpViewAttrib,
                             zSHORT       nMsg )
{
   // This operation is only temporary and should be deleted along with the
   // derived attribute when no portable files include the attribue values
   // anymore.
   if ( nMsg == zDERIVED_GET )
   {
      StoreValueInRecord( vIn, lpViewEntity, lpViewAttrib, "", 0 );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: oTZWDLGSO_ConvertCtrlTextToWeb
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*TRANSFORMATION */  OPERATION
oTZWDLGSO_ConvertCtrlTextToWeb( zVIEW vTZWDLGSO  /*  BASED ON LOD TZWDLGSO  */ )
{
   zCHAR  NewText[ 255 ];
   zCHAR  OrigText[ 255 ];
   zPCHAR pszIn;
   zPCHAR pszOutEnd;
   zPCHAR pszOut;

   // Convert the Control Text attribute to Web characters.
   // Initially, that is converting "&&" to "&amp;".
   GetVariableFromAttribute( OrigText, 0, 'S', 254,
                             vTZWDLGSO, "Control", "Text", "", 0 );
   pszIn     = OrigText;
   pszOut    = NewText;
   pszOutEnd = pszOut + 254;
   while ( pszOut < pszOutEnd && *pszIn != 0 )
   {
      if ( *pszIn == '&' )
         if ( *(pszIn + 1) == '&' )
         {
            // This is a double apersand, so convert to "&amp;".
            strcpy_s( pszOut, sizeof( NewText ) - (pszOut - NewText), "&amp;" );
            pszIn  = pszIn + 2;
            pszOut = pszOut + 5;
         }
         else
         {
            // This is a single apersand, so remove it.
            pszIn++;
         }
      else
      {
         // This is reqular character, so copy it.
         *pszOut = *pszIn;
         pszOut++;
         pszIn++;
      }
   }
   *pszOut = 0;
   SetAttributeFromString( vTZWDLGSO, "Control", "Text", NewText );

   return( 0 );
} // oTZWDLGSO_ConvertCtrlTextToWeb
