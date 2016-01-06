#include "kzoephdr.h"

zSHORT  /* DERIVED ATTRIBUTE */  OPERATION
oKZWDLGXO_DerivedControlText( zVIEW        vXWD  /*  BASED ON LOD KZWDLGXO  */,
                              LPVIEWENTITY lpEntity,
                              LPVIEWATTRIB lpAttribute,
                              zSHORT       GetOrSetFlag )
{
   zVIEW   vXWD2;
   zPCHAR  pchAttr;
   zLONG   lNLS;
   zSHORT  LanguageIndex;
   zSHORT  nRC;

   nRC = GetVariableFromAttribute( &lNLS, 0, zTYPE_INTEGER, 0,
                                   vXWD, "Ctrl", "NLS", "", zACCEPT_NULL_ENTITY );
   if ( nRC < 0 )
   {
      GetAddrForAttribute( &pchAttr, vXWD, "Ctrl", "Text" );
   }
   else
   {
      CreateViewFromViewForTask( &vXWD2, vXWD, 0 );
      nRC = ResetViewFromSubobject( vXWD2 );
      while ( nRC == 0 )
         nRC = ResetViewFromSubobject( vXWD2 );

      SetCursorFirstEntity( vXWD2, "NLS", "" );
      SetCursorRelativeEntity( vXWD2, "NLS", lNLS, "" );
      SysGetLanguageCode( &LanguageIndex );
      nRC = SetCursorFirstEntityByInteger( vXWD2, "NLS_Text", "LI", (zLONG) LanguageIndex, "" );
      if ( nRC >= zCURSOR_SET )
         GetAddrForAttribute( &pchAttr, vXWD2, "NLS_Text", "Text" );
      else
      {
         GetAddrForAttribute( &pchAttr, vXWD, "Ctrl", "Text" );
      }

      DropView( vXWD2 );
   }

   StoreValueInRecord( vXWD, lpEntity, lpAttribute, pchAttr, 0 );
   return( 0 );
}

zSHORT  /* DERIVED ATTRIBUTE */  OPERATION
oKZWDLGXO_DerivedOptionText( zVIEW        vXWD,  // BASED ON LOD KZWDLGXO
                             LPVIEWENTITY lpEntity,
                             LPVIEWATTRIB lpAttribute,
                             zSHORT       GetOrSetFlag )
{
   zVIEW   vXWD2;
   zPCHAR  pchAttr;
   zCHAR   szNullString[ 2 ];
   zLONG   lNLS;
   zSHORT  LanguageIndex;
   zSHORT  nRC;

   szNullString[ 0 ] = 0;

   nRC = GetVariableFromAttribute( &lNLS, 0, zTYPE_INTEGER, 0,
                                   vXWD, "Opt", "NLS", "", zACCEPT_NULL_ENTITY );
   if ( nRC < 0 )
      pchAttr = szNullString;
   else
   {
      CreateViewFromViewForTask( &vXWD2, vXWD, 0 );
      nRC = ResetViewFromSubobject( vXWD2 );
      while ( nRC == 0 )
         nRC = ResetViewFromSubobject( vXWD2 );
      SetCursorFirstEntity( vXWD2, "NLS", "" );
      SetCursorRelativeEntity( vXWD2, "NLS", lNLS, "" );
      SysGetLanguageCode( &LanguageIndex );
      nRC = SetCursorFirstEntityByInteger( vXWD2, "NLS_Text", "LI", (zLONG) LanguageIndex, "" );
      if ( nRC >= zCURSOR_SET )
         GetAddrForAttribute( &pchAttr, vXWD2, "NLS_Text", "Text" );
      else
         pchAttr = szNullString;

      DropView( vXWD2 );
   }

   StoreValueInRecord( vXWD, lpEntity, lpAttribute, (zPVOID) pchAttr, 0 );

   return( 0 );
}


zSHORT  /* DERIVED ATTRIBUTE */  OPERATION
oKZWDLGXO_DerivedCaption( zVIEW        vXWD,  //  BASED ON LOD KZWDLGXO
                          LPVIEWENTITY lpEntity,
                          LPVIEWATTRIB lpAttribute,
                          zSHORT       GetOrSetFlag )
{
   zPCHAR  pchAttr;
   zLONG   lNLS;
   zSHORT  LanguageIndex;
   zSHORT  nRC;

   nRC = GetVariableFromAttribute( &lNLS, 0, zTYPE_INTEGER, 0,
                                   vXWD, "Wnd", "NLS", "", zACCEPT_NULL_ENTITY );
   if ( nRC < 0 )
   {
      GetAddrForAttribute( &pchAttr, vXWD, "Wnd", "Caption" );
   }
   else
   {
      SetCursorFirstEntity( vXWD, "NLS", "" );
      SetCursorRelativeEntity( vXWD, "NLS", lNLS, "" );
      SysGetLanguageCode( &LanguageIndex );
      nRC = SetCursorFirstEntityByInteger( vXWD, "NLS_Text", "LI", (zLONG) LanguageIndex, "" );
      if ( nRC >= zCURSOR_SET )
         GetAddrForAttribute( &pchAttr, vXWD, "NLS_Text", "Text" );
      else
         GetAddrForAttribute( &pchAttr, vXWD, "Wnd", "Caption" );
   }

   StoreValueInRecord( vXWD, lpEntity, lpAttribute, (zPVOID) pchAttr, 0 );

   return( 0 );
}


zSHORT  /* DERIVED ATTRIBUTE */  OPERATION
oKZWDLGXO_DerivedDIL_Msg( zVIEW        vXWD,  // BASED ON LOD KZWDLGXO
                          LPVIEWENTITY lpEntity,
                          LPVIEWATTRIB lpAttribute,
                          zSHORT       GetOrSetFlag )
{
   zPCHAR  pchAttr;
   zSHORT  LanguageIndex;
   zSHORT  nRC;


   if ( CheckExistenceOfEntity( vXWD, "NLS_DIL_Text" ) >= zCURSOR_SET )
   {
      SysGetLanguageCode( &LanguageIndex );
      nRC = SetCursorFirstEntityByInteger( vXWD, "NLS_DIL_Text", "LI", (zLONG) LanguageIndex, "" );
      if ( nRC >= zCURSOR_SET )
         GetAddrForAttribute( &pchAttr, vXWD, "NLS_DIL_Text", "Text" );
      else
         GetAddrForAttribute( &pchAttr, vXWD, "DIL", "Msg" );
   }
   else
      GetAddrForAttribute( &pchAttr, vXWD, "DIL", "Msg" );

   StoreValueInRecord( vXWD, lpEntity, lpAttribute, (zPVOID) pchAttr, 0 );

   return( 0 );
}
