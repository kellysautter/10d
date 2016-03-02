////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmp4aa.c - VML SubSystem Parser Auxilary Module
// DESCRIPTION:   This is the source file which contains the auxilary
//                or helper functions.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/27
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9A   1995/08/30
//
// LAST MESSAGE ID: VM00402
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG most recent first order

2001.04.10 HH
   Correct generation of RETURN for return type void.
2001.02.21  HH
   Support unary minus operator.

2000.12.27  HH
   Check for case sensitive match of entity/attribute name(VerifyQualifier).

2000.09.15  HH
   support local C operations to be called from VML.

2000.06.23 HH
   fix for "FOR integer VIEW.ENTITY".

1999.07.12  HH all
   Bug, where a cast to (zUSHORT) or (zULONG) was erroneously generated
   is fixed now. It was the initializaion of sParmList and sParmListExt
   in GetOperationParmListFromSource.

old CHANGE LOG

27.06.1997  DonC
   Modified GetDefineZKey to create ExternalHeaderFile entry for an Operation
   in TZOPSIGK that needs it.

27.06.1997  DonC
   Modified IsKnownObjectOperation to drop a temporary view that was left
   hanging.

02.07.1997  DonC
   Modified ParserIsObjectOperation to drop a temporary view that was left
   hanging.

20.07.1997  DonC
   Corrected handling of szExprCode and szTextCode so that IF statements
   could handle a larger number of expressions within the IF.

02.09.1997  DonC
   Modified GetLengthFromAttribute to always return a length of 17 for
   DateTime data types.

16.11.1997  DonC
   Added handling of qTDATE data type in order to process date syntax errors.

10.12.1997  DonC
   Added case qTDATE to GenerateTempVariable operation.

14.01.1998  DonC
   Modified GetLengthFromAttribute to check for null entity for ER_Attribute.

17.07.1998  HH
   Modified GetOperationParmListFromSource, added parameter sParmListExt.
   And use it, to store "unsigned" information.

16.10.1998  HH
   Remember data type from function prototype in extended parmlist (TB363).

17.12.1998  HH
   added NewGenerateTempVariable, which returns the data type of the
   variable generated. Needed, because "date" type generates "string".

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"

// static function prototypes
static zLONG remember_type( zLONG lParmType );
static zSHORT parameter_is_unsigned( zVIEW vLookupView );
static zLONG
GetOperationData( zVIEW  vSubtask,
                  zPLONG plOperationZKey,
                  zPLONG plOperationType,
                  zPCHAR pchType,
                  zLONG  lTypeLth,
                  zVIEW  vGenericView,
                  zPCHAR pchOperationName );

/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS:  GetDefineZKey
//              GetVariableID
//              GetVariableName
//              GetVariableLength
//              GetVariableDataType
//              GetClassBasedOnType
//              GetVariableDataClass
//              GetOperationInformation
//              GetOperationClass
//              IsLocalOperation
//              GetOperationData
//              IsGlobalOperation
//              IsZeidonOperation
//              IsObjectOperation
//              IsKnownObjectOperation
//              GetOperationZKey
//              GetOperationReturnType
//              GetOperationParmListPM
//              GetOperationParmListFromSource
//              Priority
//              SplitQualifier
//              VerifyQualifier
//              GetLengthFromAttribute
//              CreateLoopControlWithView
//              GenerateTempVariable
//              ReuseTempVariable
//              ConvertZeidonTypeToVMLType
//              ConvertVMLTypeToZeidonType
//              GetZeidonOperTypeFromVMLType
//              GetOppositeOperator
//              ReverseOperator
//              AddReturnStatement
//
/////////////////////////////////////////////////////////////////////////////
zLONG LOCALOPER
ParserIsObjectOperation( zVIEW  vSubtask,
                         zPVIEW pvLODView,
                         zPCHAR pchOperationName );
//
//   look it up a  define
//   Is it is a parameter?
// |-YES:
// |    Is the operation an object operation?
// |  |-YES:
// |  |    Was the object operation called before ?
// |  |  |-YES:
// |  |  |    position on the header.
// |  |  |    Is the define there?
// |  |  |  |-YES:
// |  |  |  |    only add to the expression STOP!
// |  |  |  |-NO:
// |  |  |       Activate the meta for the header
// |  |  |       Look up the define.
// |  |  |       Is the define there?
// |  |  |     |-YES:
// |  |  |     |    include the define STOP!
// |  |  |     |-NO:
// |  |  |          Look up in the global list
// |  |  |          then Look up in the zeidon global list
// |  |  |-NO:
// |  |       Include the header
// |  |       Activate the meta for the header
// |  |       Look up the define.
// |  |       Is the define there?
// |  |     |-YES:
// |  |     |    include the define STOP!
// |  |     |-NO:
// |  |          Look up in the global list
// |  |         then Look up in the zeidon global list
// |  |-NO:
// |       Is it a Global Operation?
// |     |-YES:
// |     |    Include the header
// |     |    Activate the meta for the header
// |     |    Look up the define.
// |     |    Is the define there?
// |     |  |-YES:
// |     |  |    include the define STOP!
// |     |  |-NO:
// |     |       Look up in the zeidon global list
// |     |
// |     |-NO:
// |       Is it a Zeidon Operation?
// |     |-YES:
// |     |    Include the header
// |     |    Activate the meta for the header
// |     |    Look up the define.
// |     |    Is the define there?
// |     |  |-YES:
// |     |  |    include the define STOP!
// |     |  |-NO:
// |     |       ERROR STOP!
// |     |-NO:
// |          ERROR STOP!
// |-NO:
//      Start with the first object. and look up the define
//      Was it Found?
//    |-YES:
//    |    include the define STOP!
//    |-NO:
//         Look up in the global list
//         Was it Found?
//       |-YES:
//       |    include the define STOP!
//       |-NO:
//            Look up in the Zeidon global list
//            Was it Found?
//          |-YES:
//          |    include the define STOP!
//          |-NO:
//               ERROR STOP!
//
zOPER_EXPORT zLONG OPERATION
GetDefineZKey( zVIEW  vSubtask,
               zPLONG plDataType,
               zPCHAR pchDefineValue )
{
   zLONG  lDefineZKey;
   zLONG  lRefZKey;
   zVIEW  vLookupView;
   zVIEW  vHdr;
   zVIEW  vHdrList;
   zCHAR  cZeidonType[ 2 ];
   zLONG  lRC = zCURSOR_UNCHANGED;

   lRefZKey = 0;

   // it is, for what kind of class.
   switch ( g_lOperationClass )
   {
      case 0:
      case qLOCALOPERATION: // start here
      // see if it already exists in the xpg
      lRC = SetCursorFirstEntityByString( g_lpSGView, "DefinedItem", "ExternalValue",
                                          pchDefineValue, "VML_XSource" );
      // does it exist? if so break.
      if ( lRC >= zCURSOR_SET )
         break;

      // else fall through

   // Is the operation an object operation?
   case qOBJECTOPERATION:
   // the type was a global operation
   case qGLOBALOPERATION:
      // old code removed, no special define processing is now
      // needed for global oprs.

   case qZEIDONOPERATION:
      lRC = CreateViewFromViewForTask( &vLookupView, g_lpZOListView, 0 );

      lRC = GetIntegerFromAttribute( &lRefZKey, vLookupView, "HeaderFile_Oper", "ZKey" );
      // position on the header.
      lRC = SetCursorFirstEntityByInteger( vLookupView, "HeaderFile", "ZKey", lRefZKey, "" );
      // Is the hdr there?
      if ( lRC == zCURSOR_SET )
      {
         lRC = SetCursorFirstEntityByString( vLookupView, "DefinedItem", "ExternalValue", pchDefineValue, "" );
         if ( lRC == zCURSOR_SET )
         {
            lRC = SetCursorFirstEntityByString( g_lpSGView, "DefinedItem", "ExternalValue", pchDefineValue, "" );
            if ( lRC < zCURSOR_SET )
               lRC = IncludeSubobjectFromSubobject( g_lpSGView, "DefinedItem", vLookupView, "DefinedItem", zPOS_AFTER );
            DropView( vLookupView );
            break;
         }
         else
            DropView( vLookupView );
      }
      else
         DropView( vLookupView );

      // if all of this fails, then we do a massive search of
      // all there headers then the Zeidon Operation List
      lRC = RetrieveViewForMetaList( vSubtask, &vHdrList, zREFER_HDR_META );

      lRC = SetCursorFirstEntity( vHdrList, "W_MetaDef", "" );
      while ( lRC >= 0 )
      {
         ActivateMetaOI( vSubtask, &vHdr, vHdrList, zREFER_HDR_META, zSINGLE );
         GetIntegerFromAttribute( &lRefZKey, vHdr, "HeaderFile", "ZKey" );
         lRC = SetCursorFirstEntityByString( vHdr, "DefinedItem", "ExternalValue", pchDefineValue, "" );
         // see if the define is there
         if ( lRC == zCURSOR_SET )
         {
            // if it is include both the header and the define
            lRC = SetCursorFirstEntityByInteger( g_lpSGView, "ExternalHeaderFile", "ZKey", lRefZKey, "" );
            if ( lRC != zCURSOR_SET )
            {
               lRC = SetCursorLastEntity( g_lpSGView, "ExternalHeaderFile", "" );
               lRC = IncludeSubobjectFromSubobject( g_lpSGView, "ExternalHeaderFile", vHdr, "HeaderFile", zPOS_AFTER );
            }

            lRC = SetCursorFirstEntityByString( g_lpSGView, "DefinedItem", "ExternalValue", pchDefineValue, "" );
            if ( lRC != zCURSOR_SET )
               lRC = IncludeSubobjectFromSubobject( g_lpSGView, "DefinedItem", vHdr, "DefinedItem", zPOS_AFTER );
            DropView( vHdrList );
            DropMetaOI( vSubtask, vHdr );
            //////////////////////////
               lRC = GetIntegerFromAttribute( &lDefineZKey, g_lpSGView, "DefinedItem", "ZKey" );
               lRC = GetStringFromAttribute( cZeidonType, zsizeof( cZeidonType ), g_lpSGView, "DefinedItem", "DataType" );
               *plDataType = ConvertZeidonTypeToVMLType( cZeidonType[ 0 ] );
               return( lDefineZKey );
            //////////////////////////
         }

         DropMetaOI( vSubtask, vHdr );
         lRC = SetCursorNextEntity( vHdrList, "W_MetaDef", "" );
      }

      DropView( vHdrList );

      // did not find it.
      // do massive search of TZOPSIGK
      // break ommitted
      lRC = CreateViewFromViewForTask( &vLookupView, g_lpZOListView, 0 );
      lRC = SetCursorFirstEntityByString( vLookupView, "DefinedItem", "ExternalValue", pchDefineValue, "OPERSIG" );
      // found it somewhere
      if ( lRC >= zCURSOR_SET )
      {
         lRC = GetIntegerFromAttribute( &lRefZKey, vLookupView, "HeaderFile", "ZKey" );
         // position on the header.
         lRC = SetCursorFirstEntityByInteger( g_lpSGView, "ExternalHeaderFile", "ZKey", lRefZKey, "" );
         if ( lRC < zCURSOR_SET )
         {
            lRC = SetCursorLastEntity( g_lpSGView, "ExternalHeaderFile", "" );
            lRC = IncludeSubobjectFromSubobject( g_lpSGView, "ExternalHeaderFile", vLookupView, "HeaderFile", zPOS_AFTER );
         }

         lRC = SetCursorFirstEntityByString( g_lpSGView, "DefinedItem", "ExternalValue", pchDefineValue, "" );
         if ( lRC < zCURSOR_SET )
         {

            lRC = IncludeSubobjectFromSubobject( g_lpSGView, "DefinedItem", vLookupView, "DefinedItem", zPOS_AFTER );
         }

         DropView( vLookupView );
         break;
      }

      DropView( vLookupView );
      break;
   }

   if ( lRC >= zCURSOR_SET && CheckExistenceOfEntity( g_lpSGView, "DefinedItem" ) >= zCURSOR_SET )
   {
      lRC = GetIntegerFromAttribute( &lDefineZKey, g_lpSGView, "DefinedItem", "ZKey" );
      lRC = GetStringFromAttribute( cZeidonType, zsizeof( cZeidonType ), g_lpSGView, "DefinedItem", "DataType" );
      *plDataType = ConvertZeidonTypeToVMLType( cZeidonType[ 0 ] );

      return( lDefineZKey );
   }

   // It's not a define but it could be an operation for which we will
   // need an include for the prototype in the generated C.  Thus we
   // will look it up as an operation and if found we will create an
   // ExternalHeaderFile entry as necessary.
   CreateViewFromViewForTask( &vLookupView, g_lpZOListView, 0 );
   if ( SetCursorFirstEntityByString( vLookupView, "Operation", "Name", pchDefineValue, 0 ) >= zCURSOR_SET )
   {
      // Is there a Header entry for the operation?
      if ( CheckExistenceOfEntity( vLookupView, "HeaderFile_Oper" ) >= zCURSOR_SET )
      {
         // Is the Header entry already there?
         if ( SetCursorFirstEntityByAttr( g_lpSGView, "ExternalHeaderFile", "ZKey", vLookupView, "HeaderFile_Oper", "ZKey", 0 ) < zCURSOR_SET )
         {
            // No, then add it.
            SetCursorLastEntity( g_lpSGView, "ExternalHeaderFile", "" );
            IncludeSubobjectFromSubobject( g_lpSGView, "ExternalHeaderFile", vLookupView, "HeaderFile_Oper", zPOS_AFTER );
         }
      }
   }

   DropView( vLookupView );

   return( -1 );
}

//
// OPERATION: GetVariableID
//
// DECSCRIPTION: GetVariableID simply is passed the name of a variable
//    which is positioned on using the global view. The ID number of the
//    variable is returned
//
zOPER_EXPORT zLONG OPERATION
GetVariableID( zPLONG nID, zPCHAR pchVariableName )     // name of the variable looked for
{
   zLONG lRC;                         // temp return code

   // initialize to not found
   *nID = -1;

   // since an entity exists, check to see if this same name was
   // declared for this operation.
   lRC = SetCursorFirstEntityByString( g_lpSGView, "Variable", "Name", pchVariableName, "" );

   // variable is not there
   if ( lRC < zCURSOR_SET )
      return( -1 );

   // get the id number
   lRC = GetIntegerFromAttribute( nID, g_lpSGView, "Variable", "ID" );

   return( lRC );
}

zOPER_EXPORT zLONG OPERATION
GetVariableName( zLONG lID, zPCHAR pchVariableName, zLONG lMaxLth )
{
   zLONG lRC;                         // temp return code

   // since an entity exists, check to see if this same name was declared for this operation.
   lRC = SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "ID", lID, "" );
   if ( lRC  < zCURSOR_SET )
      return( -1 );  // variable is not there

   // get the Name
   lRC = GetStringFromAttribute( pchVariableName, lMaxLth, g_lpSGView, "Variable", "Name" );
   return( lID );
}

zOPER_EXPORT zLONG OPERATION
GetVariableLength( zPCHAR pchVarLth, zLONG lMaxLth, zLONG lID )
{
   zLONG lDataType;                   // temp to hold the Type of the var
   zLONG lLth;
   zLONG lRC;                         // temp return code

   pchVarLth[ 0 ] = 0;

   // since an entity exists, check to see if this same name was declared for this operation.
   lRC = SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "ID", lID, "" );

   // variable is not there
   if ( lRC  < zCURSOR_SET )
      return( -1 );

   // get the Name
   lRC = GetIntegerFromAttribute( &lLth, g_lpSGView, "Variable", "NElements" );

   if ( pchVarLth != 0 )
   {
      //lRC = GetStringFromAttribute( szLen, zsizeof( szLen ), g_lpSGView, "Variable", "NElements" );
      lRC = GetIntegerFromAttribute( &lDataType, g_lpSGView, "Variable", "DataType" );
      if ( lDataType == qTCHARACTER )
      {
         zltoa( lLth, pchVarLth, lMaxLth );
         return( lLth );
      }
      else
      {
         zltoa( lLth - 1, pchVarLth, lMaxLth );
         return( lLth - 1 );
      }
   }

   return( lLth );
}

//
// OPERATION: GetVariableDataType
//
// DECSCRIPTION: GetVariableDataType simply is passed the id of a variable
//    which is positioned on using the global view. The data type  of the
//    variable is returned. Since we are passed an ID, we assume
//    that all other error checking has been done, therefor we do none here.
//
zOPER_EXPORT zLONG OPERATION
GetVariableDataType( zLONG lVariableID )    // id of the variable looked for
{
   zLONG lDataType;                   // temp to hold the Type of the var
   zLONG lRC;                         // temp return code

   // since an entity exists, check to see if this same name was
   // declared for this operation.
   lRC = SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "ID", lVariableID, "" );

   // variable is not there
   if ( lRC < zCURSOR_SET )
      return( -1 );

   // get the id number
   lRC = GetIntegerFromAttribute( &lDataType, g_lpSGView, "Variable", "DataType" );
   return( lDataType );
}

//
// OPERATION: GetClassFromType
//
zOPER_EXPORT zLONG OPERATION
GetClassBasedOnType( zLONG lDataType )
{
   zLONG lDataClass;

   switch ( lDataType )
   {
    case qTINTEGER:
    case qTSHORT:
    case qTDECIMAL:
    case qTVIEW:
    case -qTINTEGER:
    case -qTSHORT:
    case -qTDECIMAL:
    case -qTVIEW:
      lDataClass = qNUMERIC;
      break;

    case qTSTRING:
    case qTCHARACTER:
    case qTENTITYNAME:
    case qTATTRIBUTENAME:
    case -qTSTRING:
    case -qTCHARACTER:
    case -qTENTITYNAME:
    case -qTATTRIBUTENAME:
      lDataClass = qTEXT;
      break;

    case qTDATE:
      lDataClass = qTUNKNOWN;
      break;

    default:
      lDataClass = qTUNKNOWN;
   }

   return( lDataClass );
}

//
// OPERATION: GetVariableDataClass
//
// DECSCRIPTION: GetVariableDataClass simply is passed the id of a variable
//    which is positioned on using the global view. The data Class of the
//    variable is returned. Since we are passed an ID, we assume
//    that all other error checking has been done, therefore we do none here.
//
zOPER_EXPORT zLONG OPERATION
GetVariableDataClass( zLONG lVariableID )   // id of the variable looked for
{
   zLONG  lDataClass;                     // temp to hold the Type of the var

   // since an entity exists, check to see if this same name was
   // declared for this operation.
   if ( SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "ID", lVariableID, "" ) < zCURSOR_SET )
   {
      // variable is not there
      return( -1 );
   }

   // get the id number
   GetIntegerFromAttribute( &lDataClass, g_lpSGView, "Variable", "VMLType" );

   return( lDataClass );
}

zOPER_EXPORT zLONG OPERATION
GetOperationInformation( zVIEW  vSubtask,
                         zPLONG plOperationClass,
                         zPLONG plOperationZKey,
                         zPLONG plOperationType,
                         zPCHAR pchType,
                         zLONG  lTypeLth,
                         zPVIEW pvSource,
                         zPCHAR pchOperationName,
                         zLONG  lSearchOrder,
                         zLONG  lOther )
{
   zVIEW vLookupView;
   zCHAR szObjectName[ zZEIDON_NAME_LTH + 1 ];
   zCHAR szParentEntityName[ zZEIDON_NAME_LTH + 1 ];
   zLONG lRC;                                       // temp return code

   // 1: If there is no parameter, or it is not a variable
   //    then we look up locally first, then to the global list. then Z
   // 2: if there is a variable as the first parameter, and the
   //    parameter is a view to a known lod.
   //    then we look up locally first, then to the src for
   //    the lod of the view, then globally then Z
   // 3: if there is a variable as the first parameter, and the
   //    parameter is a view ( to a window )
   //    then we look up locally first, then to the src for
   //    the dialog of the view, then globally then Z
   // first get the operation class. it could possibly not exist at this
   // point.

   *plOperationClass = -1;
   *plOperationZKey = 0;
   *plOperationType = 0;
   *pvSource = 0;

   // Check first to see if it is a local operation
   //  multiple Source files are allowed

   // check first, if the meta is for a LOD or Dialog
   //  (not Global Operation).
   // At the end, the view must be positionend on the right Operation.

   MiGetObjectNameForView( szObjectName, g_vSourceMeta );
   if ( zstrcmp( szObjectName, "TZOGSRCO") != 0 )
   {
      // it is NOT a Global Operation Source
      CreateViewFromViewForTask( pvSource, g_vSourceMeta, 0 );

      // find the Operation
      //  do this under the scope of the parent of "SourceFile"
      MiGetParentEntityNameForView( szParentEntityName, *pvSource, "SourceFile" );

      if ( SetCursorFirstEntityByString( *pvSource, "Operation", "Name",
                                         pchOperationName, szParentEntityName ) >= zCURSOR_SET )
      {
         *plOperationClass = qLOCALOPERATION;
         GetOperationData( vSubtask, plOperationZKey, plOperationType, pchType, lTypeLth, *pvSource, pchOperationName );
         return( 0 );
      }

      // no success finding the Operation in the Operation list.
      DropView( *pvSource );
      *pvSource = 0;
   }

   // old code, still active for Global Operations
   CreateViewFromViewForTask( &vLookupView, g_lpSGView, 0 );

   lRC = SetCursorFirstEntityByString( vLookupView, "Operation", "Name", pchOperationName, "" );

   if ( lRC > zCURSOR_UNCHANGED ) // found
   {
      *plOperationClass = qLOCALOPERATION;
      if ( CompareAttributeToAttribute( vLookupView, "SourceFileOfOperation", "ZKey",
                                        g_vSourceMeta, "SourceFile", "ZKey" ) == 0 )
      {
         CreateViewFromViewForTask( pvSource, g_vSourceMeta, 0 );
         GetOperationData( vSubtask, plOperationZKey, plOperationType, pchType, lTypeLth, *pvSource, pchOperationName );
      }
      else
      {
         MessageSend( vSubtask, "VM00401", "VML Parser",
                      "Local operation can only be used inside same source file",
                      zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
      }

      DropView( vLookupView );
      return( 0 );
   }

   DropView( vLookupView );

   // if 2 then look in the object
   if ( lSearchOrder == 2 )
   {
      lRC = IsKnownObjectOperation( vSubtask, pvSource, (zVIEW) lOther, pchOperationName );
      if ( lRC != -1 )
      {
         *plOperationClass = qOBJECTOPERATION;
         GetOperationData( vSubtask, plOperationZKey, plOperationType, pchType, lTypeLth, *pvSource, pchOperationName );
         return( 0 );
      }
   }

   if ( lSearchOrder == 3 )
   {
   }

   // next look global
   lRC = IsGlobalOperation( pchOperationName );

   if ( lRC != -1 )
   {
      lRC = LoadSourceForMetaList( vSubtask, pvSource, g_lpGOListView, zREFER_GO_META );

      *plOperationClass = qGLOBALOPERATION;
      GetOperationData( vSubtask, plOperationZKey, plOperationType, pchType, lTypeLth, *pvSource, pchOperationName );
      return( 0 );
   }

   if ( IsZeidonOperation( pchOperationName ) )
   {
      *plOperationClass = qZEIDONOPERATION;

      CreateViewFromViewForTask( pvSource, g_lpZOListView, 0 );
      GetOperationData( vSubtask, plOperationZKey, plOperationType, pchType, lTypeLth, *pvSource, pchOperationName );
      return( 0 );
   }

   return( -1 );
}

//
// OPERATION: GetOperationClass
//
// DECSCRIPTION: GetOperationClass tries to determine where the operation
//    that is being called is stored. There are 4 possibilities: 1> could
//    be a locally defined operation, that is it could be a function defined
//    in this local source member. OR it could be a local function defined
//    to another source member also tied to this META type.  2> could be
//    an object operation. But only ties to views within this operation
//    3> could be a reusable operation defined in the applications operation
//    list. 4> could be a core operation defined in the Zeidon operation list.
//    Or it may not be defined yet. This is the order in which things will
//    be checked. A value indicating the class of the operation will be
//    returned once it has been determined else a -1 will be returned to say
//    that it is invalid. One of the following only are legit return values:
//      qLOCALOPERATION qOBJECTOPERATION qGLOBALOPERATION qZEIDONOPERATION
//    or -1
//
zOPER_EXPORT zLONG OPERATION
GetOperationClass( zVIEW  vSubtask,
                   zPLONG plOperationClass,
                   zPCHAR pchOperationName ) // id of the oper to be found
{
   zLONG lRC;                         // temp return code

   // FIRST: try to locate it as a local operation using the global view.
   // we can assume that at least one operation exists ( the one we are
   // building minimally ) so we need not check for the existence of an
   // operation entity
   lRC = IsLocalOperation( pchOperationName );
   if ( lRC != -1 )
   {
      *plOperationClass = qLOCALOPERATION;
      return( 0 );
   }

   // SECOND: try to locate it as an object operation.
   lRC = ParserIsObjectOperation( vSubtask, &g_lpLastLOD, pchOperationName );
   if ( lRC != -1 )
   {
      *plOperationClass = qOBJECTOPERATION;
      return( 0 );
   }
   else
      g_lpLastLOD = (zVIEW) 0;

   // THIRD: try to locate it as a reusable operation.
   lRC = IsGlobalOperation( pchOperationName );
   if ( lRC == 1 )
   {
      *plOperationClass = qGLOBALOPERATION;
      return( 0 );
   }

   // LASTLY: try to locate it as a Zeidon operation.
   lRC = IsZeidonOperation( pchOperationName );
   if ( lRC == 1 )
   {
      *plOperationClass = qZEIDONOPERATION;
      return( 0 );
   }

   // ain't defined yet
   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
IsLocalOperation( zPCHAR pchOperationName ) // id of the oper to be found
{
   zVIEW vLookupView;
   zLONG lRC;                         // temp return code

   lRC = CreateViewFromViewForTask( &vLookupView, g_lpSGView, 0 );

   lRC = SetCursorFirstEntityByString( vLookupView, "Operation", "Name", pchOperationName, "" );

   if ( lRC > zCURSOR_UNCHANGED ) // found
   {
      DropView( vLookupView );
      // may need to return a DIALOG if one calls the other.
      return( qLOCALOPERATION );
   }

   // try to locate it as a local operation under
   // another source for the same meta type operation.
   lRC = SetCursorFirstEntityByString( g_vSourceMeta, "Operation", "Name", pchOperationName, "" );

   // could also be local to another source within this xpg,
   // we need to look this up also
   if ( lRC > zCURSOR_UNCHANGED ) // found
   {
      DropView( vLookupView );
      return( qLOCALOPERATION );  // may need to return a DIALOG if one calls the other.
   }

   DropView( vLookupView );
   return( -1 );
}

static zLONG
GetOperationData( zVIEW  vSubtask,
                  zPLONG plOperationZKey,
                  zPLONG plOperationType,
                  zPCHAR pchType,
                  zLONG  lTypeLth,
                  zVIEW  vGenericView,
                  zPCHAR pchOperationName )
{
   zLONG lRC;                         // temp return code

   lRC = SetCursorFirstEntityByString( vGenericView, "Operation", "Name", pchOperationName, "" );

// if ( 0 )
// {
//    SetNameForView( vGenericView, "vGenericView", vSubtask, zLEVEL_TASK );
//    MessageSend( vSubtask, "VM00402", pchOperationName,
//                 "GetOperationData - Please check View: vGenericView for Operation Data",
//                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
// }

   if ( lRC < zCURSOR_SET )
   {
      return( -1 );  //  not found
   }

   GetIntegerFromAttribute( plOperationZKey, vGenericView, "Operation", "ZKey" );
   GetStringFromAttribute( pchType, lTypeLth, vGenericView, "Operation", "ReturnDataType" );
   *plOperationType = ConvertZeidonTypeToVMLType( pchType[ 0 ] );
   GetStringFromAttribute( pchType, lTypeLth, vGenericView, "Operation", "Type" );

   return( 0 );
}

//
// OPERATION: IsGlobalOperation
//
// DECSCRIPTION: IsGlobalOperation attempts to see if the operation name that
//    is sent is is defined in the Application's Reusable Operation list.
//
zOPER_EXPORT zLONG OPERATION
IsGlobalOperation( zPCHAR pchOperationName ) // id of the oper to be found
{
   zLONG lRC;                         // temp return code

   if ( g_lpGOListView == 0 )
      return( -1 );

   lRC = SetCursorFirstEntityByString( g_lpGOListView, "W_MetaDef", "Name", pchOperationName, "" );
   if ( lRC > zCURSOR_UNCHANGED )    // found
      return( 1 );

   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
IsZeidonOperation( zPCHAR pchOperationName ) // id of the oper to be found
{
   zLONG lZKey;
   zLONG lRC;                         // temp return code

   if ( g_lpZOListView == 0 )
      return( 0 );

   lRC = SetCursorFirstEntityByString( g_lpZOListView, "Operation", "Name", pchOperationName, "" );
   if ( lRC  < zCURSOR_SET )    // not found
      return( 0 );

   GetIntegerFromAttribute( &lZKey, g_lpZOListView, "HeaderFile_Oper", "ZKey" );
   SetCursorFirstEntityByInteger( g_lpZOListView, "HeaderFile", "ZKey", lZKey, "");

// if ( 0 )
// {
//    SetNameForView( g_lpZOListView, "g_lpZOListView", 0, zLEVEL_TASK );
//    MessageSend( g_lpZOListView, "VM00402", pchOperationName,
//                 "IsZeidonOperation - Please check View: g_lpZOListView for HeaderFile_Oper and HeaderFile",
//                 zMSGQ_OBJECT_CONSTRAINT_ERROR, 0 );
// }

   return( 1 );
}

zLONG LOCALOPER
ParserIsObjectOperation( zVIEW  vSubtask,
                         zPVIEW pvLODView,
                         zPCHAR pchOperationName )
{
   zVIEW vSource;
   zLONG lRC;                         // temp return code

   // go through all the "view" variables to see if this operation
   // is tied to the object.
   lRC = SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "DataType", qTVIEW, "" );
   while ( lRC == zCURSOR_SET )
   {
      lRC = GetIntegerFromAttribute( (zPLONG) pvLODView, g_lpSGView, "Variable", "Value" );

      if ( lRC == 0 && *pvLODView != 0 )
      {
         lRC = SetCursorFirstEntity( *pvLODView, "SourceFile", "" );
         while ( lRC == zCURSOR_SET )
         {
            lRC = LoadSourceForMeta( vSubtask, &vSource, *pvLODView );

            if ( lRC == 0 )
            {
               lRC = SetCursorFirstEntityByString( vSource, "Operation", "Name",
                                                   pchOperationName, "" );
               DropView( vSource );
               if ( lRC > zCURSOR_UNCHANGED )    // found
               {
                  return( 0 );
               }
            }

            lRC = SetCursorNextEntity( *pvLODView, "SourceFile", "" );
         }
      }

      lRC = SetCursorNextEntityByInteger( g_lpSGView, "Variable", "DataType", qTVIEW, "" );
   }

   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
IsKnownObjectOperation( zVIEW  vSubtask,
                        zPVIEW pvSource,
                        zVIEW  vLODView,
                        zPCHAR pchOperationName )
{
   zLONG lRC;                         // temp return code

   lRC = SetCursorFirstEntity( vLODView, "SourceFile", "" );
   while ( lRC == zCURSOR_SET )
   {
      lRC = LoadSourceForMeta( vSubtask, pvSource, vLODView );
      if ( lRC == 0 )
      {
         lRC = SetCursorFirstEntityByString( *pvSource, "Operation", "Name", pchOperationName, "" );
         if ( lRC > zCURSOR_UNCHANGED )    // found
         {
            return( 0 );
         }

         DropView( *pvSource );
      }

      lRC = SetCursorNextEntity( vLODView, "SourceFile", "" );
   }

   *pvSource = 0;
   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
GetOperationZKey( zVIEW  vSubtask,
                  zLONG  lOperationClass,
                  zPCHAR pchOperationName ) // id of the oper to be found
{
   zVIEW vLookupView = 0;
   zLONG lZKeyNumber;
   zLONG lRC;                         // temp return code

   switch ( lOperationClass )
   {
      case qLOCALOPERATION:
         lRC = CreateViewFromViewForTask( &vLookupView, g_lpSGView, 0 );
         break;

      case qOBJECTOPERATION:
         lRC = CreateViewFromViewForTask( &vLookupView, g_lpLastLOD, 0 );
         //lRC = ParserIsObjectOperation( vSubtask, vLookupView, pchOperationName );
         break;

      case qGLOBALOPERATION:
      {
         if ( g_lpGOListView )
         {
            zVIEW vTempList;

            // Save the current meta type cursor by creating a temp view and
            // passing it to ActivateMeta.  ActivateMeta will change the meta
            // type to Global Op Group.
            CreateViewFromViewForTask( &vTempList, g_lpGOListView, 0 );

            ActivateMetaOI( vSubtask, &vLookupView, vTempList,
                            zREFER_GO_META, zSINGLE | zLEVEL_APPLICATION );
            DropView( vTempList );
         }

         break;
      }

      case qZEIDONOPERATION:
         lRC = CreateViewFromViewForTask( &vLookupView, g_lpZOListView, 0 );
         break;

      default:
         return( -1 );
         break;
   }

   if ( vLookupView == 0 )
      return( -1 );

   lRC = SetCursorFirstEntityByString( vLookupView, "Operation", "Name",  pchOperationName, "" );
   if ( lRC > zCURSOR_UNCHANGED )    // found
   {
      GetIntegerFromAttribute( &lZKeyNumber, vLookupView, "Operation", "ZKey" );
      DropView( vLookupView );
      return( lZKeyNumber );
   }

   DropView( vLookupView );
   return( -1 );
}

// creates a parm list from the Parameter attrib of the operation list.
// the parm type is gotten and if it is a pointer, made negative.
//
zOPER_EXPORT zLONG OPERATION
GetOperationParmListPM( zVIEW vSubtask,
                        zLONG sParmList[],
                        zLONG sParmListExt[],
                        zLONG lZKey,
                        zLONG lOperationClass )
{
   zLONG lVal;
   zLONG lCheckParms;
   zLONG lParmType;
   zCHAR cZeidonType[ 2 ];
   zCHAR cPtrFlag[ 2 ];
   zLONG lPtrFlag;
   zVIEW vLookupView;
   zLONG lRC;                         // temp return code

   switch( lOperationClass )
   {
      case qLOCALOPERATION:
         CreateViewFromViewForTask( &vLookupView, g_lpSGView, 0 );
         lRC = SetCursorFirstEntityByInteger( vLookupView, "Operation", "ZKey", lZKey, "" );
         if ( lRC > zCURSOR_UNCHANGED )    // found
         {
            lRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag",
                                   zPOS_FIRST | zQUAL_INTEGER | zQUAL_NOT | zQUAL_EQUAL, &lVal,
                                   "", "", 0, "", 0 );
            lCheckParms = 1;
            lVal = 0;
            while ( lRC > zCURSOR_UNCHANGED )
            {
               GetIntegerFromAttribute( &lParmType, vLookupView, "Variable", "DataType" );
               GetIntegerFromAttribute( &lPtrFlag, vLookupView, "Variable", "ParameterFlag" );
               if ( lPtrFlag == 2 )
               {
                  if ( lParmType > 0 )
                     lParmType *= -1;

                  sParmListExt[ lCheckParms ] |= POINTER_BIT;
               }

               // check, whether the parameter is unsigned.
               //  this info is stored in UNSIGNED_BIT of the extended parm list.
               if ( parameter_is_unsigned( vLookupView ) )
                  sParmListExt[ lCheckParms ] |= UNSIGNED_BIT;

               sParmList[ lCheckParms ] = lParmType;
               sParmListExt[ lCheckParms ] |= remember_type( lParmType );
               lCheckParms++;

               lRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag",
                                      zPOS_NEXT | zQUAL_INTEGER | zQUAL_NOT | zQUAL_EQUAL,
                                      &lVal, "", "", 0, "", 0 );
            }

            sParmList[ 0 ] = lCheckParms;

            DropView( vLookupView );
            return( lZKey );
         }

         DropView( vLookupView );
         return( 0 );
         break;

      case qOBJECTOPERATION:
         lRC = CreateViewFromViewForTask( &vLookupView, g_lpLastLOD, 0 );
      // lRC = GetObjectViewForOperation( &vLookupView, lZKey );
         break;

      case qGLOBALOPERATION:
      {
         zVIEW vTempList;

         // Save the current meta type cursor by creating a temp view and
         // passing it to ActivateMeta.  ActivateMeta will change the meta
         // type to Global Op Group.
         CreateViewFromViewForTask( &vTempList, g_lpGOListView, 0 );

         ActivateMetaOI( vSubtask, &vLookupView, g_lpGOListView,
                         zREFER_GO_META, zSINGLE | zLEVEL_APPLICATION );
         DropView( vTempList );

         break;
      }

      case qZEIDONOPERATION:
         lRC = CreateViewFromViewForTask( &vLookupView, g_lpZOListView, 0 );
         break;

      default:
         lCheckParms = 0;
         while ( lCheckParms < PARM_LIST_SIZE )
            sParmList[ lCheckParms++ ] = 0;

         lCheckParms = 0;
         return( -1 );
         break;
   }

   lRC = SetCursorFirstEntityByInteger( vLookupView, "Operation", "ZKey", lZKey, "" );
   if ( lRC > zCURSOR_UNCHANGED )    // found
   {
      lCheckParms = 1;
      lRC = SetCursorFirstEntity( vLookupView, "Parameter", "" );
      while ( lRC > zCURSOR_UNCHANGED )
      {
         GetStringFromAttribute( cZeidonType, zsizeof( cZeidonType ), vLookupView, "Parameter", "DataType" );

         lParmType = ConvertZeidonTypeToVMLType( cZeidonType[ 0 ] );

         sParmListExt[ lCheckParms ] |= remember_type( lParmType );

         GetStringFromAttribute( cPtrFlag, zsizeof( cPtrFlag ), vLookupView, "Parameter", "PFlag" );
         if ( cPtrFlag[ 0 ] == 'Y' )
         {
            if ( lParmType > 0 )
               lParmType *= -1;

            sParmListExt[ lCheckParms ] |= POINTER_BIT;
         }

         // Determine if this parameter is unsigned.  This info is stored in UNSIGNED_BIT of the extended parm list.
         if ( parameter_is_unsigned( vLookupView ) )
            sParmListExt[ lCheckParms ] |= UNSIGNED_BIT;

         sParmList[ lCheckParms ] = lParmType;

         lCheckParms++;

         lRC = SetCursorNextEntity( vLookupView, "Parameter", "" );
      }

      sParmList[ 0 ] = lCheckParms;

      DropView( vLookupView );
      return( lZKey );
   }

   DropView( vLookupView );

   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
GetOperationParmListFromSource( zLONG sParmList[],
                                zLONG sParmListExt[],
                                zPCHAR pchParmName,
                                zLONG  lParmLth,
                                zVIEW  vSource )
{
   zVIEW vLookupView;
   zCHAR cZeidonType[ 2 ];
   zCHAR cPtrFlag[ 2 ];
   zLONG lCheckParms;
   zLONG lParmType;
   zLONG lReturnType = 0;
   zBOOL bStringRejected = FALSE;
   zLONG lRC;             // return code

#if 1
   zmemset( sParmList, 0, PARM_LIST_SIZE * sizeof( zLONG ) );
   zmemset( sParmListExt, 0, PARM_LIST_SIZE * sizeof( zLONG ) );
#else
   lCheckParms = 0;
   while ( lCheckParms < PARM_LIST_SIZE )
   {
      sParmList[ lCheckParms ] = 0;
      sParmListExt[ lCheckParms ] = 0;
      lCheckParms++;
   }
#endif

   pchParmName[ 0 ] = 0;
   lCheckParms = 0;

   if ( vSource == 0 )
      return( -1 );

   CreateViewFromViewForTask( &vLookupView, vSource, 0 );

   //if ( lOperationClass == qLOCALOPERATION )
   //{
   //   lCheckParms = 1;
   //   lVal = 0;
   //   lRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag",
   //                          zPOS_FIRST | zQUAL_INTEGER | zQUAL_NOT | zQUAL_EQUAL,
   //                          &lVal, "", "", 0, "", 0 );
   //
   //   while ( lRC > zCURSOR_UNCHANGED )
   //   {
   //      GetIntegerFromAttribute( &lParmType, vLookupView, "Variable", "DataType" );
   //      GetIntegerFromAttribute( &lPtrFlag, vLookupView, "Variable", "ParameterFlag" );
   //      if ( lPtrFlag == 2 )
   //      {
   //         if ( lParmType > 0 )
   //            lParmType *= -1;
   //      }
   //
   //      sParmList[ lCheckParms ] = lParmType;
   //      lCheckParms++;
   //      lRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag",
   //                             zPOS_NEXT | zQUAL_INTEGER | zQUAL_NOT | zQUAL_EQUAL,
   //                             &lVal, "", "", 0, "", 0 );
   //   }
   //
   //   sParmList[ 0 ] = lCheckParms;
   //   return( 0 );
   //}

   lCheckParms = 1;  // skip first bucket (that's where the count is kept).
   lRC = SetCursorFirstEntity( vLookupView, "Parameter", "" );
   while ( lRC > zCURSOR_UNCHANGED )
   {
      GetStringFromAttribute( cZeidonType, zsizeof( cZeidonType ), vLookupView, "Parameter", "DataType" );

      lParmType = ConvertZeidonTypeToVMLType( cZeidonType[ 0 ] );

      sParmListExt[ lCheckParms ] |= remember_type( lParmType );

      GetStringFromAttribute( cPtrFlag, zsizeof( cPtrFlag ), vLookupView, "Parameter", "PFlag" );
      if ( cPtrFlag[ 0 ] == 'Y' )
      {
         if ( g_szGenLang[ 0 ] == 'J' )
         {
            if ( lParmType != qTVIEW )  // qTVIEW:1045
            {
               if ( lReturnType == 0 )
               {
                  if ( lParmType != qTSTRING || bStringRejected == FALSE )
                  {
                     lReturnType = lParmType;
                     GetStringFromAttribute( pchParmName, lParmLth, vLookupView, "Parameter", "ShortDesc" );
                  }
               }
               else
               if ( lReturnType == qTSTRING )  // qTSTRING:1042
               {
                  bStringRejected = TRUE;
                  if ( lParmType == qTSTRING )  // reject multiple "return" string parameters
                  {
                     lReturnType = 0;  // if there's more than one return parameter, don't return a type
                     pchParmName[ 0 ] = 0;
                  }
               }
            }
         }

         if ( lParmType > 0 )
            lParmType *= -1;

         // In the future, we would like to get rid of using -lParmType as a flag and use
         // the extended parm list instead.
         sParmListExt[ lCheckParms ] |= POINTER_BIT;
      }

      sParmList[ lCheckParms ] = lParmType;

      // If this parameter is unsigned, set the UNSIGNED_BIT in the extended parm list.
      if ( parameter_is_unsigned( vLookupView ) )
         sParmListExt[ lCheckParms ] |= UNSIGNED_BIT;

      lCheckParms++;

      lRC = SetCursorNextEntity( vLookupView, "Parameter", "" );
   }

   sParmList[ 0 ] = lCheckParms;

   DropView( vLookupView );

   return( lReturnType );
}

//
// OPERATION: Priority
//
// DESCRIPTION: Gives a number value representing the priority of the
//    operator that was sent in. That is, some operators have
//    higher precidence than others an are put int the operator
//    stack first. This gives the priority according to the
//    scheme predetermined by the language.
//
zOPER_EXPORT zLONG OPERATION
Priority( zLONG lOperator )
{
   switch( lOperator )
   {
      /* lowest to highest using c order precidence */
      case qEQUAL:
         return( 1 );

      case qOR:
         return( 2 );

      case qAND:
         return( 2 );

      case qNOTEQUAL:
         return( 3 );

      case qCONDEQUAL:
         return( 3 );

      case qLESSTHANEQUAL:
         return( 4 );

      case qLESSTHAN:
         return( 4 );

      case qGREATERTHANEQUAL:
         return( 4 );

      case qGREATERTHAN:
         return( 4 );

      case qMINUS:
         return( 5 );

      case qADDITION:
         return( 5 );

      case qMULTIPLICATION:
         return( 6 );

      case qDIVISION:
         return( 6 );

      case qNOT:
         return( 7 );

      case qUNARYMINUS:
         return( 7 );

      case qCOLON:
         return( 8 );

      case qRPAREN:
         return( 9 );

      case qLPAREN:
         return( 0 );

      default:
         return( 0 );
   }
}

//
// OPERATION: SplitQualifier
//
// DESCRIPTION: SplitQualifier takes in a string that is expected to contain either an
//    attribute qualifier (v.e.a) or an entity qualifier (v.e). This simply splits
//    out the components into the view, entity, attribute and returns the values in
//    the respective string parameters.
//
zOPER_EXPORT zLONG OPERATION
SplitQualifier( zPCHAR szQualifierString, // Qualifier string to be split (v.e.a)
                zPCHAR pchViewName,        // return View
                zPCHAR pchEntityName,      // return Entity
                zPCHAR pchAttributeName )  // return Attribute
{
   int i = 0;  // represents position in the qualifier string
   int j = 0;  // position in the target string

   // initialize the return strings
   pchViewName[ 0 ] = 0; // View name
   pchEntityName[ 0 ] = 0; // entity name if known
   pchAttributeName[ 0 ] = 0; // attribute name if known

   // breakout oView
   while( szQualifierString[ i ] != '.' && szQualifierString[ i ] != 0 )
      pchViewName[ j++ ] = szQualifierString[ i++ ];

   pchViewName[ j ] = 0;

   // if the OCEA ended with an INSTANCE name only, then stop
   if ( szQualifierString[ i ] == 0 )
      return( 1 );

   j = 0; i++;

   while( szQualifierString[ i ] != '.' && szQualifierString[ i ] != 0 )
      pchEntityName[ j++ ] = szQualifierString[ i++ ];

   pchEntityName[ j ] = 0;

   // if the Qualifier ended with an entity name, then stop
   if ( szQualifierString[ i ] == 0 )
      return( 2 );

   j = 0; i++;

   while( szQualifierString[ i ] != '.' && szQualifierString[ i ] != 0 )
      pchAttributeName[ j++ ] = szQualifierString[ i++ ];

   pchAttributeName[ j ] = 0;

   return( 3 );
}

//
// OPERATION: VerifyQualifier
//
// DESCRIPTION: VerifyQualifier is passed an integer and three strings. The
//    integer tells the operation what type of qualifier to verify. Either
//    a qENTITYQUALIFIER, or a qATTRIBUTEQUALIFIER. To Verify the Qualifier
//    several things must happen.
//
//    First: we need to see if the view was defined. If it is, then we
//       need to be sure that it was defined as a VIEW. AND if it IS
//       defined as a VIEW, whether or not the def is known.
//
//    Second: If we are verifying a qENTITYQUALIFIER or a qATTRIBUTEQUALIFIER
//       Then we need to determine if the ENTITY name is a Variable or a
//       literal. If it is a literal then the VIEW must have a def.
//
//    Third: If the verifying is of a qATTRIBUTEQUALIFIER. See if it is a
//       literal or variable. If either the VIEW is unknown or the Entity
//       is a variable, then the attribute must be too.
//
//    Return Codes:
//
zOPER_EXPORT zLONG OPERATION
VerifyQualifier( zVIEW  vSubtask,
                 zLONG  lQualiferType,
                 zPCHAR pchViewName,
                 zPLONG lpViewID,
                 zPCHAR pchEntityName,
                 zPLONG lpEntityID,
                 zPCHAR pchAttributeName,
                 zPLONG lpAttributeID )
{
   zLONG  lViewClass;
   zLONG  lEntityMustBeVar;
   zLONG  lAttributeMustBeVar;
   zVIEW  lpViewOfView;
   zLONG  lRC;

   // Get the View ID
   // Also set cursor position on view if it exists.
   lRC = GetVariableID( lpViewID, pchViewName );

   if ( *lpViewID == -1 )
   {
      // post error to error object
      SyntaxError( vSubtask, eQVIEWUNDEFINED, pchViewName );

      // DM - temporarily define the view to cut down on errors in parse.
      // Set the line number to -1 to denote temporary status.
      AddVariableEntityToPI( vSubtask,
                             pchViewName,      // the name of the variable
                             qTVIEW,           // the vml type that was declared as
                             0,                // number of chars if a string
                             -1 );             // line number on which it was defined


      return( VIEWUNDEFINED );
   }

   // Check to see if the view variable has a LOD attached to it (if it does,
   // then Variable.Value contains a view variable).  If there is no LOD,
   // then we assume that V.E.A is valid.
   if ( CompareAttributeToInteger( g_lpSGView, "Variable", "Value", 0 ) == 0 )
      return( VALIDVIEWTEMPORARY );

   // Get the View Class
   lViewClass = GetVariableDataClass( *lpViewID );

   if ( lViewClass != qTVIEW )
   {
      SyntaxError( vSubtask, eQVIEWNOTDEFINEDASVIEW, pchViewName );
      return( VIEWNOTDEFINEDASVIEW );
   }

   // okay, the view is declared and as a view. Try to get the Value stored
   // in the variable just verified. If it is -1 then the view is for an
   // unknown object definition and all of the parts must be unknown; that
   // is, they must be variables.

   lRC = GetIntegerFromAttribute( (zPLONG) &lpViewOfView, g_lpSGView, "Variable", "Value" );

   // if lpViewOfView is -1 then the view id unknown
   if ( lpViewOfView == 0 )
   {
      lEntityMustBeVar = 1;
      lAttributeMustBeVar = 1;
   }
   else
   {
      lRC = ResetView( lpViewOfView );
      if ( lRC < 0 )
         SyntaxError( vSubtask, eQVIEWUNDEFINED, pchViewName );

      lEntityMustBeVar = 0;
      lAttributeMustBeVar = 0;
   }

   // If the View is known, then the def of this view is loaded as an instance of
   // the object_object. Therefore, we can use this view to verify the other parts.
   if ( lQualiferType == qENTITYQUALIFIER ||
        lQualiferType == qATTRIBUTEQUALIFIER )
   {
      // attempt to verify the entity
      // if all must be a var, then the entity name must be declared as an
      // entity
      if ( lEntityMustBeVar == 1 )
      {
         // See if the variable is defined
         lRC = GetVariableID( lpEntityID, pchEntityName );

         if ( *lpEntityID == -1 )
         {
            SyntaxError( vSubtask, eQENTITYUNDEFINEDASVAR, pchEntityName );
            return( INVALIDENTITY );
         }
      }
      else
      {
         zCHAR szRealEntityName[ 33 ] = { 0 };
         // see if the entity is a literal. We are looking in the object
         // object for an entity of ENTITY with the attribute of name
         // set to the entity sent in. Look under the object as the scoping
         // entity.
         *lpEntityID = 0;
         lRC = SetCursorFirstEntityByString( lpViewOfView, "LOD_Entity", "Name",
                                             pchEntityName, "LOD" );

         // Check for case sensitive match
         if ( lRC >= zCURSOR_SET )
            GetStringFromAttribute( szRealEntityName, zsizeof( szRealEntityName ), lpViewOfView, "LOD_Entity", "Name" );

         if ( lRC < zCURSOR_SET ||
              zstrcmp( pchEntityName, szRealEntityName ) != 0 )
         {
            // even if it does not have to be a variable it can be
            // See if the variable is defined
            lRC = GetVariableID( lpEntityID, pchEntityName );

            if ( *lpEntityID == -1 )
            {
               SyntaxError( vSubtask, eQENTITYNOTDEFINEDINOJECT, pchEntityName );
               return( INVALIDENTITY );
            }

            lAttributeMustBeVar = 1;
         }
      }

      if ( lQualiferType == qATTRIBUTEQUALIFIER )
      {
         // attempt to verify the attribute
         // if all must be a var, then the attribute name must be declared as an
         // attributename
         if ( lAttributeMustBeVar == 1 )
         {
            // See if the variable is defined
            lRC = GetVariableID( lpAttributeID, pchAttributeName );

            if ( *lpAttributeID == -1 )
            {
               SyntaxError( vSubtask, eQATTRIBUTEUNDEFINEDASVAR, pchAttributeName );
               return( INVALIDATTRIBUTE );
            }

            return( qENTITYANDATTRIBUTEAREUNKNOWN );
         }
         else
         {
            zCHAR szRealAttrName[ 33 ] = { 0 };

            // see if the attrib is a literal. We are looking in the object
            // object for an entity of ATTRIB with the attribute of NAME
            // set to the attrib sent in. Look under the current parent as the
            // scoping entity. Since it could be found under either ENTITY
            // or CHILDENTITY and since we have to already be positioned on
            // the correct one.
            lRC = SetCursorFirstEntityByString( lpViewOfView, "ER_Attribute", "Name", pchAttributeName, "LOD_Entity" );
            // Check for case sensitive match
            if ( lRC >= zCURSOR_SET )
               GetStringFromAttribute( szRealAttrName, zsizeof( szRealAttrName ), lpViewOfView, "ER_Attribute", "Name" );

            if ( lRC < zCURSOR_SET || zstrcmp( pchAttributeName, szRealAttrName ) != 0 )
            {
               // even if it does not have to be a variable it can be
               // See if the variable is defined
               lRC = GetVariableID( lpAttributeID, pchAttributeName );

               if ( *lpAttributeID == -1 )
               {
                  SyntaxError( vSubtask, eQATTRIBUTENOTDEFINEDINOJECT, pchAttributeName );
                  return( INVALIDATTRIBUTE );
               }

               if ( *lpEntityID == 0 )
                  return( qATTRIBUTEISUNKNOWN );
               else
                  return( qENTITYANDATTRIBUTEAREUNKNOWN );
            }
            else
            {
               zCHAR cType[ 2 ];

               // if it is a known valid attribute, put the type in
               // the pointer to the attributeID
               lRC = GetStringFromAttribute( cType, zsizeof( cType ), lpViewOfView, "Domain", "DerivedDataType" );

               *lpAttributeID = ConvertZeidonTypeToVMLType( cType[ 0 ] );

               return( qENTITYANDATTRIBUTEARECONSTANT );
            }
         }
      }
      else
      {
         if ( *lpEntityID != 0 )
            return( qENTITYISUNKNOWN );
         else
            return( qENTITYISCONSTANT );
      }
   }
   else
   {
      if ( lEntityMustBeVar == 0 )
         return( VALIDVIEW );
      else
         return( VALIDVIEWUNKNOWN );
   }
}

zOPER_EXPORT zLONG OPERATION
GetLengthFromAttribute( zLONG  lViewID,
                        zPCHAR pchEntityName,
                        zPCHAR pchAttributeName )
{
   zVIEW  lpViewOfView;
   zCHAR  szDataType[ 2 ];
   zLONG  lRC;

   // okay, the view is declared and as a view. Try to get the Value stored
   // in the variable just verified. If it is -1 then the view is for an
   // unknown object definition and all of the parts must be unknown that
   // is they must be variables.

   GetVariableName( lViewID, g_szViewName, zsizeof( g_szViewName ) );

   GetIntegerFromAttribute( (zPLONG) &lpViewOfView, g_lpSGView, "Variable", "Value" );

   // If lpViewOfView is 0, then there is no LOD attached to the view
   // variable, so we can't find a length.  Return -1.
   if ( lpViewOfView == 0 )
      return( -1 );

   // If the ER_Attribute is null, immediately return with error.
   if ( CheckExistenceOfEntity( lpViewOfView, "ER_Attribute" ) < zCURSOR_SET )
      return( -1 );

   if ( CompareAttributeToString( lpViewOfView, "ER_Attribute","Name", pchAttributeName ) != 0 )
   {
      lRC = SetCursorFirstEntityByString( lpViewOfView, "LOD_Entity", "Name", pchEntityName, "LOD" );

      lRC = SetCursorFirstEntityByString( lpViewOfView, "ER_Attribute", "Name", pchAttributeName, "LOD_Entity" );
   }
   else
      lRC = zCURSOR_SET;

   if ( lRC >= zCURSOR_SET )
   {
      zLONG lLength;

      // if the data type of the attribute is an integer, we are
      // going to force the length to 10.  This is because as of
      // 2/21/95 the MaxStringLth for the Integer Domain was 4, as
      // well as the override length in ZKeys.  This should be corrected
      // over time, but I was afraid to change it as I didn't know the
      // impact on other parts of the system.
      GetStringFromAttribute( szDataType, zsizeof( szDataType ), lpViewOfView, "Domain", "DerivedDataType" );
      if ( szDataType[ 0 ] == 'L' )
      {
         return( 9 );
      }

      // If the data type of the attribute is DateTime, then set the
      // length to 17.
      if ( szDataType[ 0 ] == 'T' )
         return( 17 );

      // if it is a known valid attribute, put the type in
      // the pointer to the attributeID
      GetIntegerFromAttribute( &lLength, lpViewOfView, "ER_Attribute", "Lth" );
      if ( lLength == 0 )
      {

         GetIntegerFromAttribute( &lLength, lpViewOfView, "Domain", "DerivedMaxStringLth" );
      }

      return( lLength - 1 );
   }

   return( -1 );
}

//
// OPERATION: CreateLoopControl
//
zOPER_EXPORT zLONG OPERATION
CreateLoopControlWithView( zVIEW  vSubtask,
                           zLONG  lControlType,
                           zPCHAR pchNameOrValue )
{
   zLONG lNewID;
   zLONG lVarID;
   zVIEW vTargetView;
   zLONG lRC;

   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );
   CreateEntity( vTargetView, "Expression", zPOS_AFTER );

   // Insert new bottom of stack.
   lRC = AddOperatorToView( vSubtask, 0, -2 );

   lNewID = GenerateTempVariable( vSubtask, qTINTEGER, 2 );
   // add TempVar
   lRC = AddExprNodeEntityToView( vSubtask, vTargetView, qVARIABLE, qNUMERIC,
                                  qTINTEGER, "", lNewID, 0 );

   // add EQUAL
   lRC = AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, qNUMERIC,
                                  qTINTEGER, "", qEQUAL, 0 );

   if ( lControlType == qVARIABLE )
      lRC = GetVariableID( &lVarID, pchNameOrValue );

   // initial value either a var or constant
   lRC = AddExprNodeEntityToView( vSubtask, vTargetView, lControlType, qNUMERIC,
                                  qTINTEGER, pchNameOrValue, lVarID, 0 );

   // Dump the expression from the stack.
   lRC = AddOperatorToView( vSubtask, vTargetView, -1 );

   // add the initialization statement
   lRC = AddStatementEntityToPI( vSubtask, qEXPRESSION, qqlineno, vTargetView, zPOS_AFTER );
   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );
   return( lNewID );
}

zOPER_EXPORT zLONG OPERATION
GenerateTempVariable( zVIEW vSubtask,
                      zLONG lDataType,
                      zLONG lDataLength )
{
   zLONG lDummy;
   return( NewGenerateTempVariable( vSubtask, lDataType, lDataLength, &lDummy ) );
}

zOPER_EXPORT zLONG OPERATION
NewGenerateTempVariable( zVIEW  vSubtask,
                         zLONG  lDataType,
                         zLONG  lDataLength,
                         zPLONG plNewDataType )
{
   zCHAR szHold[ 32 ];
   zLONG lID;
   zLONG lDefinedLength;
   zLONG lModify;
   zLONG lSeq;
   zLONG lReturnDataType = lDataType;
   zLONG lRC;

   switch ( lDataType )
   {
      case qTENTITYNAME:
      case qTATTRIBUTENAME:
      case qTDATE:
         lReturnDataType = qTSTRING;

      case qTSTRING:
         strcpy_s( szHold, zsizeof( szHold ), "szTempString_" );
         lDataLength += 1;
         break;

      case qTCHARACTER:
         strcpy_s( szHold, zsizeof( szHold ), "cTempCharact_" );
         lDataLength = 2;
         break;

      case qTINTEGER:
      case qTSHORT:
         strcpy_s( szHold, zsizeof( szHold ), "lTempInteger_" );
         break;

      case qTDECIMAL:
         strcpy_s( szHold, zsizeof( szHold ), "dTempDecimal_" );
         break;

      case qTVIEW:
         strcpy_s( szHold, zsizeof( szHold ), "vTempViewVar_" );
         break;

      default:
         strcpy_s( szHold, zsizeof( szHold ), "kTempVariable" );
         break;
   }

   *plNewDataType = lReturnDataType;
   lSeq = 0;
   do
   {
      zltoa( lSeq, szHold + 13, zsizeof( szHold ) - 13 );

      lRC = GetVariableID( &lID, szHold );

      if ( lID == -1 ) // is not there
      {
         if ( lDataType == qTINTEGER && lDataLength == 2 )
            lDataType = qTSHORT;

         // add reuse code later
         lID = AddVariableEntityToPI( vSubtask, szHold, lDataType, lDataLength, 0 );
         return( lID );
      }
      else // it is there see if we can reuse it.
      {
         GetIntegerFromAttribute( &lModify, g_lpSGView, "Variable", "Modify" );

         if ( lModify == 0 ) // can reuse it
         {
            GetIntegerFromAttribute( &lDefinedLength, g_lpSGView, "Variable", "NElements" );

            if ( lDefinedLength >= lDataLength )
            {
               SetAttributeFromInteger( g_lpSGView, "Variable", "Modify", 1 );
               return( lID );
            }
         }
      }

      lSeq++;

   } while ( lID != -1 );

   return( -1 );
}

zOPER_EXPORT zLONG OPERATION
ReuseTempVariable( zLONG lID )
{
   zLONG lRC;

   lRC = SetCursorFirstEntityByInteger( g_lpSGView, "Variable", "ID", lID, "" );
   if ( lRC == zCURSOR_SET )
      SetAttributeFromInteger( g_lpSGView, "Variable", "Modify", 0 );

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
AddReturnStatement( zVIEW  vSubtask,
                    zLONG  lVMLToken,
                    zLONG  lValue,
                    zPCHAR pchValue,
                    zLONG  lBeforeOrAfter )
{
   zVIEW vTargetView;
   zLONG lRC;

   CreateEntity( g_lpExprView, "Expression", zPOS_AFTER );
   CreateViewFromViewForTask( &vTargetView, g_lpExprView, 0 );

   // Insert new bottom of stack.
   lRC = AddOperatorToView( vSubtask, vTargetView, -2 );

   if ( pchValue )
   {
      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0,
                               qTINTEGER, "", qLPAREN, 0 );

      // add return value
      AddExprNodeEntityToView( vSubtask, vTargetView, lVMLToken, qNUMERIC,
                               qTINTEGER, pchValue, lValue, 0 );

      AddExprNodeEntityToView( vSubtask, vTargetView, qOPERATOR, 0,
                               qTINTEGER, "", qRPAREN, 0 );

      // Dump the expression from the stack.
      AddOperatorToView( vSubtask, vTargetView, -1 );
   }

   // add the initialization statement
   lRC = AddStatementEntityToPI( vSubtask, (lValue == -1) ? qRETURNEND : qRETURN,
                                 qqlineno, vTargetView, lBeforeOrAfter );

   DeleteEntity( vTargetView, "Expression", zREPOS_NONE );
   DropView( vTargetView );
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
ConvertZeidonTypeToVMLType( zCHAR cZeidonType )
{
   if ( cZeidonType == 'S' )
      return( qTSTRING );

   if ( cZeidonType == 'C' || cZeidonType == 'Y' )
      return( qTCHARACTER );

   if ( cZeidonType == 'L' || cZeidonType == 'I' )
      return( qTINTEGER );

   if ( cZeidonType == 'N' )
      return( qTSHORT );

   if ( cZeidonType == 'M' || cZeidonType == 'F' ||
        cZeidonType == 'R' || cZeidonType == 'W' )
      return( qTDECIMAL );

   if ( cZeidonType == 'V' )
      return( qTVIEW );

   if ( cZeidonType == 'T' )
      return( qTDATE );

   if ( cZeidonType == 'O' )
      return( qTVOID ); // void
   //if ( cZeidonType == 'O' )
   //   return( 0 ); // void

   // if nothing, then void
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
ConvertVMLTypeToZeidonType( zPCHAR cZeidonType, zLONG lVMLType )
{
   cZeidonType[ 0 ] = 0;
   cZeidonType[ 1 ] = 0;
   switch ( lVMLType )
   {
      case qTSTRING:
         cZeidonType[ 0 ] = 'S';
         break;

      case  qTCHARACTER:
         cZeidonType[ 0 ] = 'C';
         break;

      case   qTINTEGER:
         cZeidonType[ 0 ] = 'L';
         break;

      case qTSHORT:
         cZeidonType[ 0 ] = 'N';
         break;

      case qTDECIMAL:
         cZeidonType[ 0 ] = 'M';
         break;

      case qTVIEW:
         cZeidonType[ 0 ] = 'V';
         break;

      case 0:
      default:
         cZeidonType[ 0 ] = 'O';
         break;
   }

   // if nothing, then void
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
GetZeidonOperTypeFromVMLType( zPCHAR pchZeidonType, zLONG lVMLOperType )
{
   pchZeidonType[ 1 ] = 0;
   switch( lVMLOperType )
   {
      case qLOCAL:
         pchZeidonType[ 0 ] = 'L';
         break;

      case qGLOBAL:
         pchZeidonType[ 0 ] = 'G';
         break;

      case qDIALOG:
         pchZeidonType[ 0 ] = 'D';
         break;

      case qDOMAIN:
         pchZeidonType[ 0 ] = 'M';
         break;

      case qOBJECT: // constraint
         pchZeidonType[ 0 ] = 'O';
         break;

      case qENTITY: // constraint
         pchZeidonType[ 0 ] = 'E';
         break;

      case qTRANSFORMATION:
         pchZeidonType[ 0 ] = 'T';
         break;

      case qDERIVED:
         pchZeidonType[ 0 ] = 'A';
         break;

      case qSHORTCUTKEY:
         pchZeidonType[ 0 ] = 'K';
         break;

      case qZEIDONOPERATION:
         pchZeidonType[ 0 ] = 'Z';
         break;

      default:
         pchZeidonType[ 0 ] = 'L';
         break;
   }

   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
GetOppositeOperator( zLONG lOperator )
{
   switch( lOperator )
   {
      case qOR:
         return( qAND );

      case qAND:
         return( qOR );

      case qNOTEQUAL:
         return( qCONDEQUAL );

      case qCONDEQUAL:
         return( qNOTEQUAL );

      case qLESSTHANEQUAL:
         return( qGREATERTHAN );

      case qLESSTHAN:
         return( qGREATERTHANEQUAL );

      case qGREATERTHANEQUAL:
         return( qLESSTHAN );

      case qGREATERTHAN:
         return( qLESSTHANEQUAL );

      default:
         return( lOperator );
   }
}

//  This operator was created by Don C on 2/2/95 as a replacement
//  for GetOppositeOperator, above.  It was required to correct a
//  statement such as "IF var1 != v2.e2.a2", because the above was
//  not generating the correct operator in that case.  I was afraid
//  to correct GetOppositeOperator because it was used in several
//  places and my correction might create a further error.  I thus
//  replaced most calls for GetOppositeOperator with calls for
//  ReverseOperator.
//
zOPER_EXPORT zLONG OPERATION
ReverseOperator( zLONG lOperator )
{
   switch( lOperator )
   {
      case qOR:
         return( qAND );

      case qAND:
         return( qOR );

      case qNOTEQUAL:
         return( qNOTEQUAL );

      case qCONDEQUAL:
         return( qCONDEQUAL );

      case qLESSTHANEQUAL:
         return( qGREATERTHANEQUAL );

      case qLESSTHAN:
         return( qGREATERTHAN );

      case qGREATERTHANEQUAL:
         return( qLESSTHANEQUAL );

      case qGREATERTHAN:
         return( qLESSTHAN );

      default:
         return( lOperator );
   }
}

// remember the original type
static zLONG
remember_type( zLONG lParmType )
{
   switch( lParmType)
   {
      case qTCHARACTER:
      case qTSTRING:
         return( NPARM_TYPE_CHAR );

      case qTINTEGER:
         return( NPARM_TYPE_LONG );

      case qTSHORT:
         return( NPARM_TYPE_SHORT );

      case qTDECIMAL:
         return( NPARM_TYPE_DECIMAL );

      case qTVIEW:
         return( NPARM_TYPE_VIEW );

      case qTDATE:
         return( NPARM_TYPE_DATE );

      case qTVOID:
         return( NPARM_TYPE_VOID );
   }

   return( 0 );
}

static zSHORT
parameter_is_unsigned( zVIEW vLookupView )
{
   zCHAR cUnsignedFlag[ 2 ];

   GetStringFromAttribute( cUnsignedFlag, zsizeof( cUnsignedFlag ), vLookupView, "Parameter", "UFlag" );
   if ( cUnsignedFlag[ 0 ] == 'Y' )
   {
      return( 1 ); // the parameter is unsigned
   }

   return( 0 );
}
