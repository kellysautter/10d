//----------------------------------------------------------------------
//
// .Name:         ZGLOBAL1.c
//
// .Description:  Common global operations for Zeidon
//
//----------------------------------------------------------------------

/*
CHANGE LOG
==========
2000.12.19 HH
    const zCHAR change.

2000.10.30 RG  Z2000
    GetViewByName: for parameter zLEVEL_TASK changed View --> 0

2000.09.08 HH
   added GetDomainType.

2000.08.09 RG   Z2000
   Removed compiler warnings

2000.04.13 DGC  10b
   Changed msgq stuff to name the message object at the task level instead
   of the application level.

   Also removed some functions--SetLM...() and related functions.

2000.03.09 HH
    improve GetDBHandlerError.

2000.01.13 BL
    insert casting for remove warnings
*/

#include <windows.h>

#define zGLOBAL_DATA
#define KZSYSSVC_INCL
#include <KZOENGAA.H>
#include <ZDRVROPR.H>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

#include <stdlib.h>
#include <string.h>
#include <io.h>


// Prototypen static functions

// Functions

//----------------------------------------------------------------------
//
// .Function Name: >>>SetAttributeFromCurrentDateTime
//
// .Description: Set an Attribut to Current Date Time
//
// .Return Value: zSHORT
//    (Same as SetAttributeFromString() )
//
// .Parameter:
//    Data type,      Name,         (I/O/U) Description
//    zVIEW           View            (I)   View for Attribute
//    zPCHAR          cpcEntityName   (I)   Name of Entity
//    zPCHAR          cpcAttributeName(I)   Name of Attribute
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
SetAttributeFromCurrentDateTime( zVIEW   vView,
                                 zCPCHAR cpcEntityName,
                                 zCPCHAR cpcAttributeName )
{
  zCHAR  szTimestamp[ 22 ];
  zSHORT nRC;

  SysGetDateTime( szTimeStamp, zsizeof( szTimestamp ) );
  nRC = SetAttributeFromString( vView, cpcEntityName, cpcAttributeName,
                                szTimestamp );
  return( nRC );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>DecimalSumOf
//
// .Description: Compute the sum of a decimal attribute over all
//               instances of an entity
//
// .Return Value: ZDecimal - sum
//
// .Parameter:
//    Datentyp,       Name,         (I/O/U) Description
//    zVIEW           View            (I)   View for Entity
//    zPCHAR          cpcEntityName   (I)   Name of Entity
//    zPCHAR          cpcAttributeName(I)   Name of Attribute
//    zPCHAR          cpcParentName     (I)   Name of Parent Entity
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zDECIMAL OPERATION
DecimalSumOf( zVIEW   vSum,
              zCPCHAR cpcEntityName,
              zCPCHAR cpcAttributeName,
              zCPCHAR cpcParentName )
{
   ZDecimal  DecimalSum;
   ZDecimal  DecimalValue;
   zINTEGER  RESULT;

   DecimalSum = 0;

   RESULT = SetCursorFirstEntity( vSum, cpcEntityName, cpcParentName );
   while ( RESULT > zCURSOR_UNCHANGED )
   {
      GetDecimalFromAttribute( &DecimalValue,
                               vSum, cpcEntityName, cpcAttributeName );
      DecimalSum += DecimalValue;
      RESULT = SetCursorNextEntity( vSum, cpcEntityName, cpcParentName );
   }

   return( DecimalSum );
}

// Sets the cursor to the latest entity based on a date/time stamp
// attribute.  The attribute passed should be a date/time stamp, but
// could be any attribute with ascending colating sequence.
zOPER_EXPORT zSHORT OPERATION
SetCursorLatestEntity( zVIEW   vView,
                       zCPCHAR cpcEntityName,
                       zCPCHAR cpcAttributeName )
{
  OrderEntityForView( vView, cpcEntityName, cpcAttributeName );
  SetCursorLastEntity( vView, cpcEntityName, "" );

  return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
GetIntFromAttrByContext( zPINTEGER pnValue,
                         zVIEW   vView,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zCPCHAR cpcContext )

{
  zSHORT nRC;

  nRC = GetVariableFromAttribute( pnValue, NULL,
                                  zTYPE_INTEGER, 0, vView,
                                  cpcEntity, cpcAttribute, cpcContext, 0 );
  return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
GetStrFromAttrByContext( zPCHAR   pchValue,
                         zINTEGER nLth,
                         zVIEW    vView,
                         zCPCHAR  cpcEntity,
                         zCPCHAR  cpcAttribute,
                         zCPCHAR  cpcContext )

{
  zSHORT nRC;

  nRC = GetVariableFromAttribute( sValue, NULL,
                                  zTYPE_STRING, (zUSHORT) nLth, vView,
                                  cpcEntity, cpcAttribute, cpcContext, 0 );
  return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
SetAttrFromIntByContext( zVIEW    vView,
                         zCPCHAR  cpcEntity,
                         zCPCHAR  cpcAttribute,
                         zINTEGER nValue,
                         zCPCHAR  cpcContext )

{
  zSHORT nRC;

  nRC = SetAttributeFromVariable( vView, cpcEntity, cpcAttribute,
                                  &nValue, zTYPE_INTEGER,
                                  0, cpcContext, 0 );
  return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
SetAttrFromStrByContext( zVIEW   vView,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zCPCHAR cpcValue,
                         zCPCHAR cpcContext )

{
  zSHORT nRC;

  nRC = SetAttributeFromVariable( vView, cpcEntity, cpcAttribute,
                                 cpcValue, zTYPE_STRING,
                                 strlen( cpcValue ), cpcContext, 0 );
  return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
AddToAttrFromIntByContext( zVIEW   vView,
                           zCPCHAR cpcEntity,
                           zCPCHAR cpcAttribute,
                           zLONG   iValue,
                           zCPCHAR cpcContext )
{
  zSHORT nRC;

  nRC = AddToAttributeFromVariable( vView, cpcEntity, cpcAttribute,
                                    &iValue, zTYPE_INTEGER, 0,
                                    cpcContext );
  return( nRC );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>GetAppProfileString
//
// .Description: Read a string from an INI file
//
// .Return Value: zSHORT
//    >= 0 - Profile found, Text returned (either from Profile or
//           if entry missing DefaultValue)
//    -1   - Profile does not exist
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPCHAR          Profile       (I)   Name of the INI file
//                                        (could contain path)
//    zPCHAR          Group         (I)   section in INI file
//    zPCHAR          Field         (I)   field name
//    zPCHAR          Value         (O)   string for return value
//    zSHORT          ValueLen      (I)   max. length of value string
//                                        without 0 at the end
//    zPCHAR          DefaultValue  (I)   default value for value string
//
//----------------------------------------------------------------------
// .Detail description
//    Reads a string from an INI file.
//    If the string does not exist, return the DefaultValue.
//    If the file name does not contain a path, the windows dir is
//    assumed.
//    See Windows 3.1 API for more info.
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
GetAppProfileString( zCPCHAR cpcProfile,
                     zCPCHAR cpcGroup,
                     zCPCHAR cpcField,
                     zPCHAR  pchValue,
                     zSHORT  nValueLth,
                     zCPCHAR cpcDefaultValue )
{
  zSHORT nRC;

  if ( zaccess( Profile, 0 ) )
  {
    TraceLineS( "(zglobal1) GetAppProfileString: File does not exist: ",
                Profile );
    return( -1 );
  }

  nRC = (zSHORT) GetPrivateProfileString( cpcGroup, cpcField, cpcDefaultValue,
                                          (zCHAR *) pchValue, (int) nValueLth + 1,
                                          cpcProfile );
  return( nRC );

} // GetAppProfileString

//----------------------------------------------------------------------
//
// .Function name: >>>GetEnvVariable
//
// .Description: Get an environment variable
//
// .Return value: zSHORT
//    0 - OK
//    else Error
//
// .Parameter:
//    Data type,      Name,        (I/O/U) Description
//    zPCHAR          szReturnWert   (O)   value of the env var (returned)
//    zPCHAR          szVariableName (I)   name of the env var
//    zSHORT          nMaxReturnLen  (I)   max. length of szReturnWert
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
GetEnvVariable( zPCHAR szReturnWert,
                zCPCHAR szVariableName,
                zSHORT nMaxReturnLth )
{
   zSHORT nRC;

   nRC = SysGetEnvVar( szReturnWert, szVariableName, nMaxReturnLth );
   return( nRC );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>StrToInt
//
// .Description: Convert an String to an Integer
//
// .Return Value: zLONG
//    (Integer Value of String )
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPCHAR          cpcStr         (I)   String to convert
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zLONG OPERATION
StrToInt( zCPCHAR cpcStr )
{
  return( atol( cpcStr ) );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>StrToDecimal
//
// .Description: Convert an String to a Decimal
//
// .Return Value: ZDecimal
//             0 - OK
//             else: invalid string
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPCHAR          cpcStr         (I)   String to convert
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zDECIMAL OPERATION
StrToDecimal( zCPCHAR cpcStr )
{
   ZDecimal dec( cpcStr );
   return( dec );
// return( atof( cpcStr ) );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>CodeToChar
//
// .Description: Returns the Char for the given code
//
// .Return Value: zVOID
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPCHAR          cpcStr         (O)   String, which contains the char
//    zSHORT          sCode         (I)   Code for Char
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zVOID OPERATION
CodeToChar( zPCHAR pchStr, zSHORT sCode )
{
  pchStr[ 0 ] = (zCHAR) (sCode & 0x00ff);
  pchStr[ 1 ] = (zCHAR) 0;
}

//----------------------------------------------------------------------
//
// .Function Name: >>>CharToCode
//
// .Description: Returns the code of a zCHAR (first zCHAR in string)
//
// .Return Value: zSHORT - code for char
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPCHAR          cpcStr         (I)   String with zCHAR on first pos
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
CharToCode( zCPCHAR cpcStr )
{
  zSHORT nCode = 0;

  nCode = (zSHORT) cpcStr[ 0 ];
  return( nCode );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>StrToIntCheck
//
// .Description: Convert a String to a number, checks for valid chars
//
// .Return Value: zSHORT - 0: OK
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPLONG          plValue       (O)   Returns converted value
//    zPCHAR          pchStr         (I)   String with number
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
StrToIntCheck( zPLONG plValue,
               zPCHAR pchStr )
{
   zPCHAR pc;

   *plValue = strtol( pchStr, &pc, 10 );

   // check for errors
   if ( errno != 0 )    // error range (too many digits)
      return( -1 );

   if ( pc == pchStr )   // no digits before other chars
     return( -1 );

   if ( pc != NULL )    // invalid chars after digits
   {
      if ( *pc != ' ' && *pc != 0 )
         return( -1 );
   }

   return( 0 );
}

//----------------------------------------------------------------------
//
// .Function Name: >>>StrToDecimalCheck
//
// .Description: Convert a String to a number, checks for valid chars
//
// .Return Value: zSHORT - 0: OK
//
// .Parameter:
//    Data type,      Name,       (I/O/U) Description
//    zPDECIMAL       plDecimal     (O)   Returns converted value
//    zPCHAR          pchStr         (I)   String with number
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
StrToDecimalCheck( zPDECIMAL pdValue,
                   zPCHAR pchStr )
{
   ZDecimal zDec( pchStr );
   *pdValue = zDec;

   if ( SysCompareDecimalToNull( &zDec ) == 0 )
   {
      // not a valid decimal
      return( -1 );
   }
   else
   {
      return( 0 );
   }
}

//----------------------------------------------------------------------
//
// .Function Name: >>>DateTimeAttributeDiff
//
// .Description: Calculates the difference between two datetime attributes
//
// .Return Value: zSHORT - 0: OK
//                        -1: Overflow (result does not fit to long)
//               zCALL_ERROR: Invalid DiffType or invalid attribute type
//
// .Parameter:
//    Data type,      Name,         (I/O/U) Description
//    zPLONG          plDiff          (O)   Difference
//                                        (szDateTime2 - szDateTime1)
//    zVIEW           vView1          (I)   View for Attribute 1
//    zPCHAR          cpcEntity1      (I)   Name of Entity 1
//    zPCHAR          cpcAttribute1   (I)   Name of Attribute 1
//    zVIEW           vView2          (I)   View for Attribute 2
//    zPCHAR          cpcEntity2      (I)   Name of Entity 2
//    zPCHAR          cpcAttribute2   (I)   Name of Attribute 2
//    zSHORT          nDiffType       (I)   Unit of Diff zDT_YEAR, ...
//
//----------------------------------------------------------------------
// .Detail description
//
//----------------------------------------------------------------------
zOPER_EXPORT zSHORT OPERATION
DateTimeAttributeDiff( zPLONG  plDiff,
                       zVIEW   vView1,
                       zCPCHAR cpcEntity1,
                       zCPCHAR cpcAttribute1,
                       zVIEW   vView2,
                       zCPCHAR cpcEntity2,
                       zCPCHAR cpcAttribute2,
                       zSHORT  nDiffType )
{
   DateTimeRecord DT1;
   DateTimeRecord DT2;
   zCHAR     szTS1[ 20 ];
   zCHAR     szTS2[ 20 ];
   zSHORT    nRC;

   // read the attributes
   nRC = GetStringFromAttribute( szTS1, zsizeof( szTS1 ), vView1, cpcEntity1, cpcAttribute1 );
   if ( nRC < 0 )
      return( zCALL_ERROR );

   nRC = GetStringFromAttribute( szTS2, zsizeof( szTS2 ), vView2, cpcEntity2, cpcAttribute2 );
   if ( nRC < 0 )
      return( zCALL_ERROR );

   nRC = UfStringToDateTime( szTS1, &DT1 );
   if ( nRC < 0 )
   {
      MessageSend( vView1, 0, "DateTimeAttributeDiff",
                   "Invalid type for attribute 1", 0, 0);
      return( zCALL_ERROR );
   }

   nRC = UfStringToDateTime( szTS2, &DT2 );
   if ( nRC < 0 )
   {
      MessageSend( vView2, 0, "DateTimeAttributeDiff",
                   "Invalid type for attribute 2", 0, 0 );
      return( zCALL_ERROR );
   }

   // subtract the values
   nRC = UfDateTimeDiff( plDiff, &DT1, &DT2, nDiffType );

   return( nRC );
} // DateTimeAttributeDiff

//./ ADD NAME=GetDBHandlerError
////////////////////////////////////////////////
//
//  ENTRY:      GetDBHandlerError
//
//  PURPOSE:    To get a dbhandler warning/error info for the task.
//
//              This operation reads the latest instance of DBH_Error
//              in KZMSGQOO. The instance is deleted, so that a
//              subsequent call to this operation returns the next
//              error/warning.
//
//              The info is only set, if the KZSMGQOO has a DBH_Error
//              Entity. If not, the operation returns zCALL_ERROR.
//
//  RETURNS:    0           - info read from KZMSGQOO
//              -1          - no info found.
//              zCALL_ERROR - Error on call
//
////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT zSHORT OPERATION
GetDBHandlerError( zVIEW   vTask,
                   zPLONG  plCode,
                   zPCHAR  pchState,
                   zPCHAR  pchMsg,
                   zUSHORT nMsgLth )
{
   const zCHAR szDBH_Error[] = "DBH_Error";

   zVIEW  vMsg = 0;
   zSHORT nRC;

   // Initialize return values.
   *plCode = 0L;
   *pchState = 0;
   *pchMsg = 0;

   // Does Named View exist?
   if ( GetViewByName( &vMsg, "__MSGQ", vSubtask, zLEVEL_TASK ) != zLEVEL_TASK )
   {
      // if not, no messages exist
      return( -1 );
   }

   // position on the latest error entry
   nRC = SetCursorLastEntity( vMsg, szDBH_Error, NULL );
   if ( nRC < 0 )
   {
     // If no entity instance exist, return with -1.
     return( -1 );
   }

   // Get it.
   GetIntegerFromAttribute( plCode, vMsg, szDBH_Error, "Code" );
   GetStringFromAttribute( pchState, vMsg, szDBH_Error, "State" );
   GetVariableFromAttribute( pchMsg, NULL, zTYPE_STRING, nMsgLth,
                             vMsg, szDBH_Error, "Msg", 0, 0 );

   // Delete this entity.
   DeleteEntity( vMsg, szDBH_Error, zREPOS_LAST );

   return( 0 );
} // GetDBHandlerError

////////////////////////////////////////////////
//
//  >>> zReadLine
//
//  this function is a wrapper around SysReadLine, because SysReadLine
//   cannot be called from VML.

// RETURNS:  1 - line read successful
//           0 - End of file reached
//          zCALL_ERROR - Error in processing
//           -1 - target string (lMax) short
//
////////////////////////////////////////////////

zOPER_EXPORT zLONG OPERATION
zReadLine( zPCHAR pchBuffer, zLONG lMax, zLONG lFileHandle )
{
   zLONG n;
   zPCHAR pcData = NULL;
   n = SysReadLine( &pcData, lFileHandle );
   if ( n < 1 || pcData == NULL )
      return( n ); // Sorry, there was an error

   if ( (zLONG) strlen( pcData ) >= lMax )
   {
      // sorry, target string short
      return( -1 );
   }

   strcpy( pchBuffer, pcData );
   return( n );
}

////////////////////////////////////////////////
//
//  >>> GetDomainType
//
//  returns the domain type of a given attribute
//          0 on error
//
////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
GetDomainType( zVIEW v, zPCHAR pchEntity, zPCHAR pchAttribute )
{
   LPVIEWENTITY lpViewEntity;
   LPVIEWATTRIB lpViewAttrib;
   LPDOMAIN     lpDomain;

   lpViewEntity = MiGetViewEntityForView( v, pchEntity );
   if ( lpViewEntity == 0 )
      return( 0 );

   // find the attribute
   lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewEntity->hFirstOD_Attrib );
   while ( lpViewAttrib )
   {
      if ( zstrcmpi( lpViewAttrib->szName, pchAttribute ) == 0 )
         break;

      lpViewAttrib = (LPVIEWATTRIB) zGETPTR( lpViewAttrib->hNextOD_Attrib );
   }

   if ( lpViewAttrib == 0 )
      return( 0 );

   lpDomain = (LPDOMAIN) zGETPTR( lpViewAttrib->hDomain );
   return( (zSHORT) lpDomain->cDomainType );
}


#ifdef __cplusplus
}
#endif
