/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzasw5aw.c - Zeidon application operations
// DESCRIPTION:   This C file contains global and LOD operations for the
//                Zeidon application.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
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

1998.08.13  DKS
   ClearCase pretty it up.
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// int
// CreateQualifierWindow( )
//
// PURPOSE:  CreateQualifierWindow creates the window that allows the
//           user to enter in a qualification for the subobject listbox.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateQualifierWnd( zVIEW   vSubtask,
                                zVIEW   vUSSubEnt,
                                zVIEW   vNewDialog )
{
   zVIEW    vLOD;
   zVIEW    vCtrlDef;
   zLONG    lCC;
   zLONG    lNumber;
   zLONG    lSize_X;
   zLONG    lSize_Y;
   zSHORT   nRC;
   zCHAR    szWorkString[ 33 ];
   zCHAR    szLiteral[ 40 ];
   zCHAR    szTempString_0[ 3 ];
   zCHAR    szTempString_1[ 50 ];
   zCHAR    szTempString_2[ 33 ];
   zCHAR    szTempString_3[ 33 ];

// TraceLineS("IN", "CreateQualifierWnd W5" );
   GetViewByName( &vLOD, "LOD_Object", vNewDialog, zLEVEL_TASK );

   lCC = 5;

   /* SHOULD I LOOP THROUGH THE OBJECT??? */
   GetStringFromAttribute( szWorkString, zsizeof( szWorkString ), vUSSubEnt, "LOD_Entity", "Name" );
   nRC = SetCursorFirstEntityByString( vLOD, "LOD_Entity",
                                       "Name", szWorkString, "" );

   nRC = SetCursorFirstEntityByString( vLOD, "LOD_Attribute",
                                       "ParticipatesInKey", "Y", "LOD_Entity" );
   if ( nRC >= zCURSOR_SET )
      GetStringFromAttribute( szLiteral, zsizeof( szLiteral ), vLOD,
                              "ER_Attribute", "Name" );
   strcat_s( szLiteral, zsizeof( szLiteral ), " " );
   GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vUSSubEnt,
                            "UIS_Include", "SubObjectOperators" );
   strcat_s( szLiteral, zsizeof( szLiteral ), szTempString_0 );

   CreateMetaEntity( vSubtask, vNewDialog, "Window", zPOS_AFTER );

   strcpy_s( szTempString_1, zsizeof( szTempString_1 ), "QUAL" );
   GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vUSSubEnt, "LOD_Entity", "Name" );
   strcat_s( szTempString_1, zsizeof( szTempString_1 ), szTempString_2 );
   szTempString_1[ 32 ] = 0;
   SetAttributeFromString( vNewDialog, "Window", "Tag", szTempString_1 );

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   SetCursorFirstEntityByString( vCtrlDef, "WindowStyle", "Tag",
                                 "Dialog Box", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "WndStyle",
                                  vCtrlDef, "WindowStyle", zPOS_AFTER );

   lNumber = 6 * lCC;
   ofnTZWDLGSO_CreateTheButton( vSubtask, vNewDialog, 48, 10, "OK", lNumber );
   SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );  /* RETURN */
   strcpy_s( szTempString_1, zsizeof( szTempString_1 ), "ListQualSubObj" );
   GetStringFromAttribute( szTempString_3, zsizeof( szTempString_3 ), vUSSubEnt, "LOD_Entity", "Name" );
   strcat_s( szTempString_1, zsizeof( szTempString_1 ), szTempString_3 );
   szTempString_1[ 32 ] = 0;

   ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, szTempString_1, "",
                                   vNewDialog, "ActOper" );

   ofnTZWDLGSO_CreateTheButton( vSubtask, vNewDialog, 48, 74, "Cancel", 0 );
   SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 );  /* RETURN */
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   ofnTZWDLGSO_CreateEntryFields( vSubtask, vNewDialog, szLiteral, 35 );

   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );

   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_Y + 24 );

// TraceLineS("END", "CreateQualifierWnd W5" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// int
// CreateEntryFields
//
// PURPOSE: CreateEntryFields creates a window that allows the user to
//          enter a key to load an object instance.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateEntryFields( zVIEW   vSubtask,
                               zVIEW   vNewDialog,
                               zPCHAR  szLiteral,
                               zSHORT  nLength )
{
   zVIEW    vUSSubEnt;
   zVIEW    vCtrlDef;
   zLONG    lLitLength;
   zLONG    lCC;
   zLONG    lCL;

// TraceLineS("IN", "CreateEntryFields W5" );
   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   GetViewByName( &vUSSubEnt, "USSubEnt", vNewDialog, zLEVEL_TASK );

   lCC = 5;
   lCL = 12;

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "Text", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vCtrlDef, "ControlDef", zPOS_AFTER );
   /* Remove the text border. */
   SetAttributeFromInteger( vNewDialog, "Control", "Subtype",
                            zCONTROL_BORDEROFF | zCONTROL_NOTABSTOP );
   SetAttributeFromString( vNewDialog, "Control", "Text", szLiteral );
   lLitLength = zstrlen( szLiteral );
   lLitLength = lLitLength * lCC;
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", lLitLength );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 12 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 10 );

   CreateMetaEntity( vSubtask, vNewDialog, "Control", zPOS_AFTER );
   SetCursorFirstEntityByString( vCtrlDef, "ControlDef", "Tag", "EditBox", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "ControlDef",
                                  vCtrlDef, "ControlDef", zPOS_AFTER );
   SetAttributeFromString( vNewDialog, "Control", "Tag", "EntryField" );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_X", nLength * lCC );
   SetAttributeFromInteger( vNewDialog, "Control", "SZDLG_Y", lCL );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_Y", 12 );
   SetAttributeFromInteger( vNewDialog, "Control", "PSDLG_X", 20 + lLitLength );

// TraceLineS("END", "CreateEntryFields W5" );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// int
// CreateEntryWindow( )
//
// PURPOSE: CreateEntryWindow creates a window that allows the user to
//          enter a key to load an object instance.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ofnTZWDLGSO_CreateEntryWindow( zVIEW    vSubtask,
                               zVIEW     vUSSubEnt,
                               zVIEW    vNewDialog )
{
   zVIEW    vCtrlDef;
   zLONG    lCC;
   zLONG    lSize_X;
   zLONG    lSize_Y;
   zSHORT   nLength;
   zCHAR    szTempString_0[ 50 ];
   zCHAR    szTempString_1[ 33 ];
   zCHAR    szTempString_2[ 33 ];
   zCHAR    szLiteral[ 50 ];

// TraceLineS("IN", "CreateEntryWindow W5" );

   lCC = 5;
   /* Create a temporary style object containing the ListQualifier
      attribute so that in CreateEntryWindow, the fields can be
      created from a dialog specification. */
   if ( CheckExistenceOfEntity( vUSSubEnt, "L_LOD_Attribute" ) == 0 )
   {
      SetCursorFirstEntityByString( vUSSubEnt, "L_LOD_Attribute",
                                    "ParticipatesInKey", "Y", "UIS_Entity" );
      GetStringFromAttribute( szLiteral, zsizeof( szLiteral ), vUSSubEnt,
                              "L_ER_Attribute", "ListPaintLit" );
      if ( szLiteral[ 0 ] == 0 )
      {
         GetStringFromAttribute( szLiteral, zsizeof( szLiteral ), vUSSubEnt,
                                 "L_ER_Attribute", "PromptPaintLit" );
      }

      if ( szLiteral[ 0 ] == 0 )
      {
         GetStringFromAttribute( szLiteral, zsizeof( szLiteral ), vUSSubEnt,
                                 "L_ER_Attribute", "Name" );
      }

      nLength = (zSHORT) ofnTZWDLGSO_GetSizeForListAttr( vSubtask,
                                                         vUSSubEnt, "L" );
      if ( nLength == 0 )
         nLength = 20;
   }

   CreateMetaEntity( vSubtask, vNewDialog, "Window", zPOS_AFTER );

   strcpy_s( szTempString_0, zsizeof( szTempString_0 ), "KEY" );
   GetStringFromAttribute( szTempString_1, zsizeof( szTempString_1 ), vUSSubEnt, "LOD_Entity", "Name" );
   strcat_s( szTempString_0, zsizeof( szTempString_0 ), szTempString_1 );
   szTempString_0[ 32 ] = 0;
   SetAttributeFromString( vNewDialog, "Window", "Tag", szTempString_0 );

   GetViewByName( &vCtrlDef, "TZADCTLD", vNewDialog, zLEVEL_TASK );
   SetCursorFirstEntityByString( vCtrlDef, "WindowStyle", "Tag",
                                 "Dialog Box", "" );
   IncludeSubobjectFromSubobject( vNewDialog, "WndStyle",
                                  vCtrlDef, "WindowStyle", zPOS_AFTER );

   ofnTZWDLGSO_CreateTheButton( vSubtask, vNewDialog, 48, 10, "OK", 6 * lCC );
   SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 ); /* RETURN */
   strcpy_s( szTempString_0, zsizeof( szTempString_0 ), "LoadObjForList" );
   GetStringFromAttribute( szTempString_2, zsizeof( szTempString_2 ), vUSSubEnt, "LOD_Entity", "Name" );
   strcat_s( szTempString_0, zsizeof( szTempString_0 ), szTempString_2 );
   szTempString_0[ 32 ] = 0;

   ofnTZWDLGSO_CreateTheOperation( vSubtask, vNewDialog, szTempString_0, "",
                                   vNewDialog, "ActOper" );

   ofnTZWDLGSO_CreateTheButton( vSubtask, vNewDialog, 48, 54, "Cancel", 0 );
   SetAttributeFromInteger( vNewDialog, "Action", "Type", 72 ); /* RETURN */
   SetAttributeFromString( vNewDialog, "Action", "NoMap", "Y" );

   ofnTZWDLGSO_CreateEntryFields( vSubtask, vNewDialog, szLiteral, nLength );

   ofnTZWDLGSO_CalculateWindowSize( vNewDialog, &lSize_Y, &lSize_X );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_X", lSize_X + 12 );
   SetAttributeFromInteger( vNewDialog, "Window", "SZDLG_Y", lSize_X + 24 );
// TraceLineS("IN", "CreateEntryWindow W5" );
   return( 0 );
}

///////////////////////////////////////////////////////////////////////
//
// ENTRY:    fnLocateListboxAttributes
//
// PURPOSE: Add up the lengths of all the L_LOD_Attribute entities under
//          the current ENTITY in the style object.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_LocateLstboxAttribs( zVIEW  vSubtask,
                                 zVIEW  vUserSpec )
{
   zLONG    lTotalLth;
   zLONG    lAttribLth;
   zLONG    lPaintLth;
   zSHORT   RESULT;
   zCHAR    szTempString_0[ 65 ];

// TraceLineS("IN", "LocateLstboxAttribs W5" );
   lTotalLth = 0;
   RESULT = SetCursorFirstEntity( vUserSpec, "L_LOD_Attribute", "" );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      lAttribLth = ofnTZWDLGSO_GetSizeForListAttr( vSubtask, vUserSpec, "L" );

      GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vUserSpec,
                               "L_ER_Attribute", "ListPaintLit" );
      if ( szTempString_0[ 0 ] == 0 )
         GetStringFromAttribute( szTempString_0, zsizeof( szTempString_0 ), vUserSpec,
                                 "L_ER_Attribute", "Name" );
      lPaintLth = zstrlen( szTempString_0 );

      if ( lAttribLth > lPaintLth )
      {
         lTotalLth = lTotalLth + lAttribLth + 1;
      }
      else
      {
         lTotalLth = lTotalLth + lPaintLth + 1;
      }
      RESULT = SetCursorNextEntity( vUserSpec, "L_LOD_Attribute", "" );
   }
// TraceLineS("END", "LocateLstboxAttribs W5" );
   return( lTotalLth + 1 );
}

///////////////////////////////////////////////////////////////////////
//
// ENTRY:    ofnTZWDLGSO_GetSizeForListAttr
//
// PURPOSE:  Determine the size of a list control (in bytes) that should be
//           painted for an attribute.
//
////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_GetSizeForListAttr( zVIEW  vSubtask,
                                zVIEW  vUserSpec,
                                zPCHAR szListType )
{
   zLONG    lAttribLth;
   zCHAR    szDataType[ 2 ];
   zCHAR    X_Domain[ 9 ];
   zCHAR    X_ER_Attribute[ 15 ];

// TraceLineS( "IN", " AutoPaintPartitionWithFields" );

   // Set up variables to either I_LOD listbox or L_LOD listbox.
   if ( szListType[ 0 ] == 'I' )
   {
      strcpy_s( X_Domain, zsizeof( X_Domain ), "I_Domain" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "I_ER_Attribute" );
      // If a paint length is specified for the attribute, use it.
      GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                               X_ER_Attribute, "ListPaintLth" );
   }
   else
   if ( szListType[ 0 ] == 'M' )
   {
      strcpy_s( X_Domain, zsizeof( X_Domain ), "L_Domain" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "L_ER_Attribute" );
      // If a paint length is specified for the attribute, use it.
      GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                               X_ER_Attribute, "PromptPaintLth" );
   }
   else
   {
      strcpy_s( X_Domain, zsizeof( X_Domain ), "L_Domain" );
      strcpy_s( X_ER_Attribute, zsizeof( X_ER_Attribute ), "L_ER_Attribute" );
      // If a paint length is specified for the attribute, use it.
      GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                               X_ER_Attribute, "ListPaintLth" );
   }

   if ( lAttribLth == 0 )
   {
      // Since no length was specified, determine length as follows:
      //   Integers (value of L) are assigned a fixed length of 6.
      //   Decimals (value of M) are assigned a fixed lenght of 11.
      //   Dates (value of T) are assigned a fixed lenght of 10.
      //   Strings use either length of attribute or domain.
      GetStringFromAttribute( szDataType, zsizeof( szDataType ), vUserSpec,
                              X_Domain, "DataType" );
      switch ( szDataType[ 0 ] )
      {
        case 'L':
          //  Number
          lAttribLth = 6;
          break;

        case zTYPE_DECIMAL:
          //  Decimal
          lAttribLth = 11;
          break;

        case 'T':
          //  Date&Time
          lAttribLth = 10;
          break;

        case 'S':
          //  String
          GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                                   X_ER_Attribute, "Lth" );
          if ( lAttribLth == 0 )
            GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                                   X_Domain, "MaxStringLth" );
          break;

        default:
          // We asssume the default is 6 (which should never happen).
          lAttribLth = 6;
      }
   }

   return( lAttribLth );
}

/////////////////////////////////////////////////////////////////////////////
//
// ENTRY:    ofnTZWDLGSO_GetSizeForAttr
//
// PURPOSE:  Determine the size of the control (in bytes) that should be
//           painted for an attribute.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
ofnTZWDLGSO_GetSizeForAttr( zVIEW  vSubtask,
                            zVIEW  vUserSpec,
                            zPCHAR szDomainName,
                            zPCHAR szER_AttributeName )
{
   zLONG    lAttribLth;
   zCHAR    szDataType[ 2 ];

   // If a paint length is specified for the attribute, use it.
   GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                            szER_AttributeName, "PromptPaintLth" );
   if ( lAttribLth == 0 )
   {
      // Since no length was specified, determine length as follows:
      //   Integers (value of L) are assigned a fixed length of 6.
      //   Decimals (value of M) are assigned a fixed lenght of 11.
      //   Dates (value of T) are assigned a fixed lenght of 10.
      //   Strings use either length of attribute or domain.
      GetStringFromAttribute( szDataType, zsizeof( szDataType ), vUserSpec,
                              szDomainName, "DataType" );
      switch ( szDataType[ 0 ] )
      {
        case 'L':
           //  Number
           lAttribLth = 6;
           break;

        case zTYPE_DECIMAL:
           //  Decimal
           lAttribLth = 11;
           break;

        case 'T':
           //  Date&Time
           lAttribLth = 10;
           break;

        case 'S':
           //  String
           GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                                    szER_AttributeName, "Lth" );
           if ( lAttribLth == 0 )
              GetIntegerFromAttribute( &lAttribLth, vUserSpec,
                                       szDomainName, "MaxStringLth" );
           break;

        default:
           // We asssume the default is 6 (which should never happen).
           lAttribLth = 6;
      }
   }

   return( lAttribLth );
}
