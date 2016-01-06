/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   TZVSRTOO.C
// DESCRIPTION:   This module contains the Object operations for the
//                Zeidon VML Debugger Stack object (see TZVSRTOO.LOD)
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
==========
2002.09.17  HH
   fix bug "display string truncated"
2002.01.18  HH
   new
*/

#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>
#include "tzlodopr.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "ZEIDONOP.H"

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GetSetStringValue
//
//    Derived Attribue String value
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*DERIVED ATTRIBUTE */  OPERATION
GetSetStringValue( zVIEW vStackObject  /*  BASED ON LOD TZVSRTOO  */,
                   LPVIEWENTITY lpViewEntity,
                   LPVIEWATTRIB lpViewAttrib,
                   zSHORT nGetOrSetFlag )
{
   zVIEW    XPG;
   zVIEW    lpZeidonVML;
   zLONG    lTemp;
   ZDecimal dTemp;
   zLONG    lDataType = 0;
   zULONG   nDataLength;
   zCHAR    szData[ 256 ];

   GetViewByName( &lpZeidonVML, "ZeidonVML", vStackObject, zLEVEL_TASK );
   GetViewByName( &XPG, "XPG", lpZeidonVML, zLEVEL_SUBTASK );
   GetIntegerFromAttribute( &lTemp, vStackObject, "Variable", "ID" );
   SetCursorFirstEntityByInteger( XPG, "Variable", "ID", lTemp, "" );
   GetIntegerFromAttribute( &lDataType, XPG, "Variable", "DataType" );

   switch( nGetOrSetFlag )
   {
   case zDERIVED_GET :
      // set the derived attribute value
      // we get the BLOB value and translate it to string
      switch( lDataType )
      {
      case qTINTEGER:
      case qTSHORT:
      case qTVIEW:
         nDataLength = 4;
         lTemp = 0;
         GetBlobFromAttribute( &lTemp, &nDataLength,
                               vStackObject,"Variable", "Value" );
         if ( lDataType == 1045 )
         {
            strcpy_s( szData, sizeof( szData ), "0x" );
            zltox( lTemp, szData + 2, sizeof( szData ) - 2 );
         }
         else
            zltoa( lTemp, szData, sizeof( szData ) );
         break;

      case qTDECIMAL:
         nDataLength = sizeof( ZDecimal );
         GetBlobFromAttribute( &dTemp, &nDataLength,
                               vStackObject,"Variable", "Value" );
         SysConvertDecimalToString( &dTemp, szData, -1 );
         break;

      case qTSTRING:

         nDataLength = sizeof( szData );
         GetBlobFromAttribute( szData, &nDataLength,
                               vStackObject,"Variable", "Value" );
         szData[ nDataLength ] = 0;
         break;

      default:
         szData[ 0 ] = 0;
      }

      StoreStringInRecord( vStackObject,
                           lpViewEntity, lpViewAttrib, szData );

      break;
   case zDERIVED_SET :
         // set get derived attribute value
         //  set the BLOB value accordingly

      szData[ 0 ] = 0;
      GetStringFromRecord( vStackObject, lpViewEntity, lpViewAttrib,
                           szData, sizeof(szData) );

      switch( lDataType )
      {
         case qTINTEGER:
         case qTSHORT:
            lTemp = zatol( szData );
            SetAttributeFromBlob( vStackObject, "Variable", "Value",
                                  &lTemp, sizeof( zLONG ) );
            break;

         case qTDECIMAL:
            SysConvertStringToDecimal( szData, &dTemp );
            SetAttributeFromBlob( vStackObject, "Variable", "Value",
                            &dTemp, sizeof(zDECIMAL) );
            break;

         case qTSTRING:
            SetAttributeFromBlob( vStackObject, "Variable", "Value",
                                  szData, zstrlen( szData ) );
            break;

         default:
         case qTVIEW: // do not allow setting view
            // nothing to do
            break;
         }

         break;
   }

    return( 0 );
} // GetSetStringValue

#ifdef __cplusplus
}
#endif


