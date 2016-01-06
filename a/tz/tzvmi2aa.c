/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmi2aa.c - VML SubSystem Interpretor Main Module
// DESCRIPTION:   This is the source file which contains the Setup functions
//                for the Interpreter. It contains the functions to do all
//                of the setup.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/11/06
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      10B    1995/08/05
//
// LAST MESSAGE ID: VM00204
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG most recent first order

2002.11.08 DGC
   Made some changes to allow interpreter to be called from OE.

2001.10.12 HH
   improve returning result parameter from EvaluateExpression.

2001.10.09 HH
   perform cast, if an integer is used as decimal operation parameter.

2001.08.10 HH
   R55612, improve workspace for variables, WRKS system.

2001.01.20 HH
   fix bug for integer constant 0 as string parameter.

2001.01.12 HH
   improve handling of backslashes for \xnn.

2001.01.10 HH
   R54481, fix bug with return parameter of type view.

2001.01.02 HH
   improve handling of backslashes in a DEFINE.

2000.11.22 HH
   increment stack size.

2000.08.28 HH
   fix problem with operation returncodes.

2000.08.18 HH
   support decimal return values for local operations.

2000.07.18 HH
   fix for multiple interpreter entry.

1998.05.20  HH  ALL TB632
   bug TB632 fixed, qAND and qOR were wrong, when first operand was
   decimal (Operate) .

old CHANGE LOG

97/28/02    DonC
   Corrected handling of zero divisors in Operate function.

97/24/03    DonC
   Corrected EvaluateExpression handling of qCONSTANT so that an IF statement
   with a Decimal AttributeQualifier and an Integer constant would execute
   correctly.

97/19/04    DonC
   Corrected SetUpOperationCall so that Decimal parameters are passed
   correctly.  Prior error caused a crash for Decimal parameters.

20.07.1997  DonC
   Corrected handling of szExprCode and szTextCode so that IF statements
   could handle a larger number of expressions within the IF.

04.08.1997  DonC
   Corrected processing of division so that the intermediate result is not
   decimal if the final variable or attribute is decimal but the values
   in the expression are integer.  This makes the interpretor agree with C.

03.04.1998  DonC
   Modified EvaluateExpression and SetUpOperationCall to use nPtrDataType
   in sValueStack to process the correct data type when passing data to an
   interpretable subroutine by address.

27.04.1998  DonC
   Modified NEW_LINE definition in GetDefineInternalValue to support carriage
   return as well as line feed (ie., "\r\n" instead of "\n").

29.05.1998-06.04.1998  DonC (RG,HH)
   Corrections from DonC.

04.06.1998  DonC
   Modified SetUpOperation operation to support the 32 bit interface of
   Windows NT and Windows 95. The 32 bit interface differs from the 16 bit
   interface in two ways:
   1. In 32 bit, longs are passed where shorts used to be passed.
   2. In 16 bit, parms are passed in reverse order. In 32 bit, parms are
      passed in regular order.

14.07.1998  HH
   Corrected SetUpOperationCall for a single character argument (WIN32).

18.07.1998  DonC
   Corrected ConvertZeidonTypeToVMLTypeS to return a string type instead of
   0 for the case where a type match was not found.

01.08.1998  DonC
   Modified operation Operator, cases qNOTEQUAL and qCONDEQUAL, so that the
   compare was made against dVal of structure instead of lVal when the first
   value in the compare was a decimal.
   A similar change should probably be made for qOR and qAND.
   Yes Don! See TB632.

17.08.1998  HH
   Fixed several bugs concerning
   - qNOT (was not handled correctly)
   - comparison operators, where first comparand is decimal.

25.08.1998  HH
   Passing a decimal to a staged operation did not work - fixed.
   This bug occured (among others) on
   ACTIVATE  ... WHERE decimal = decimal .

14.10.1998  HH
   On staged operation call with decimal and string, there was a bug.
   See buglist entry TB 351.

*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzwrksto.h"
#include "tzvmlopr.hg"
#include <setjmp.h>

#ifdef __cplusplus
extern "C"
{
#endif

#include "ZeidonOp.H"

typedef struct sValueStruct
{
   zLONG    lDataType;
   zLONG    lPtrDataType;
   zLONG    lVal;
   ZDecimal ldVal;
   zPVOID   lpVal;
   zLONG    lOffset;

} sValueStruct;
extern sValueStruct sValueStack[ 40 ]; // value stack to interp

// jump buffer for WKKS out of memory (VERY unlikely)
extern jmp_buf g_jbWRKS; // defined in TZVMINAA.C

/* local prototypes */
static zSHORT
GetOperationNameFromZKeyM2( zVIEW  vSubtask,
                            zPCHAR pchOperationName,
                            zLONG  lOperNameLth,
                            zPCHAR pchObjectName,
                            zLONG  lObjectNameLth,
                            zLONG  lOperationClass,
                            zLONG  lZKey );

static void
CopyBackslash( zPCHAR pcTarget, zPCHAR pcSource );

static int hextoi( int i );

static zPCHAR fnWorkStorage( zPVOID hWRKS, zLONG lSize );


zLONG LOCALOPER
fnGetStackValue( zSHORT nIdx )
{
   return( sValueStack[ nIdx ].lVal );
}

static zSHORT
RereadStatement( zVIEW  vStatementView,
                 zVIEW  vStackObject,
                 zPCHAR *pchExprText,
                 zPLONG plExprCode,
                 zPVOID hWRKS )
{
   zLONG   lPos;
   zULONG  uCodeIndex;
   zCHAR   *pcExprText;
   zSHORT  nRC;

   uCodeIndex = EXPRCODE_MAXSTRING;
   nRC = GetBlobFromAttribute( g_szExprCode, &uCodeIndex, vStatementView, "Statement", "Expression" );
   *plExprCode = (zLONG) uCodeIndex;

   GetAttributeLength( &uCodeIndex, vStatementView, "Statement", "ExprText" );

   if ( uCodeIndex )
   {
      pcExprText = fnWorkStorage( hWRKS, (zLONG)uCodeIndex );

      GetBlobFromAttribute( pcExprText, &uCodeIndex, vStatementView, "Statement", "ExprText" );
   }
   else
   {
      pcExprText = NULL;
   }

   *pchExprText = pcExprText;
   nRC = GetIntegerFromAttribute( &lPos, vStackObject, "OperationCall", "StatementOffset" );
   if ( lPos > 0 )
      nRC = SetAttributeFromInteger( vStackObject, "OperationCall", "StatementOffset", 0 );
   return( (zSHORT) lPos );
}

zOPER_EXPORT zLONG OPERATION
EvaluateExpression( zPVOID pResultOfExpression,
                    zVIEW  lpInterpSubtask,
                    zVIEW  lpAppSubtask,
                    zVIEW  vStatementView,
                    zPVOID hWRKS )
{
   sQExprStruct sWorkExpr = { 0 };
   static  zCHAR szTempNumber[ 10 ];
   zVIEW   vStackObject;                           // view to the stack object
   zSHORT  nNumericType;
   zCHAR   *pcExprText;
   zLONG   lExprIndex;
   zLONG   lExprIndex2;
   zLONG   lStringLength;
   zLONG   lExprCode;
   zLONG   lRC;
   zSHORT  nRC;

   // Get the stack object as it was loaded with the XPG
   nRC = GetViewByName( &vStackObject, "StackObject", lpInterpSubtask, zLEVEL_SUBTASK );

   // Whenever a new expression is processed, we want to reset the work storage
   WRKS_Reset( hWRKS );

   lExprIndex = RereadStatement( vStatementView, vStackObject, &pcExprText, &lExprCode, hWRKS );

   // The following loop is used to locate the data type of the result for an expression of the form: var1 = var2 ....
   // The first variable in the Expression is that result field, so we will loop until we find the first variable and then set it accordingly.
   lExprIndex2 = lExprIndex;
   sWorkExpr.lElementClass = 0;
   while ( lExprIndex2 < lExprCode && sWorkExpr.lElementClass != qVARIABLE )
   {
      zmemcpy( (zPCHAR) &sWorkExpr, (zPCHAR) g_szExprCode + lExprIndex2, sizeof( sQExprStruct ) - MAXSTRING );
      if ( sWorkExpr.lElementClass == qVARIABLE )
      {
         if ( sWorkExpr.lDataType == qTDECIMAL )
            nNumericType = 1;  // 1 is DECIMAL
         else
            nNumericType = 0;  // 0 is INTEGER
      }

      lExprIndex2 += sizeof( sQExprStruct ) - MAXSTRING;
   }

   while ( lExprIndex < lExprCode )
   {
      // get the expression record out of g_szExprCode
      zmemcpy( (zPCHAR) &sWorkExpr, (zPCHAR) g_szExprCode + lExprIndex, sizeof( sQExprStruct ) - MAXSTRING );

      strcpy_s( g_szOutputString, sizeof( g_szOutputString ), "Found " );
      switch( sWorkExpr.lElementClass )
      {
         // Initialize nPtrDataType to 0 so it won't have a value if not
         // specifically set.
         sValueStack[ g_nStackPtr ].lPtrDataType = sWorkExpr.lDataType;

         case qVARIABLE:
            if ( sWorkExpr.lOffset < 0 )
            {
               sValueStack[ g_nStackPtr ].lOffset = sWorkExpr.lOffset * -1;
            }
            else
            {
               sValueStack[ g_nStackPtr ].lOffset = sWorkExpr.lOffset;
               sValueStack[ g_nStackPtr ].lDataType = sWorkExpr.lDataType;
            }

            // Set the data type for when lDataType will be a pointer.
            sValueStack[ g_nStackPtr ].lPtrDataType = sWorkExpr.lDataType;

            nRC = SetCursorFirstEntityByInteger( vStackObject, "Variable", "ID", sValueStack[ g_nStackPtr ].lOffset, "" );

            if ( sWorkExpr.lDataClass != qNUMERIC )
            {
               zULONG ulMaxLen;

               // Get the text array length of the string variable.
               SetCursorFirstEntityByInteger( g_vXPGView, "Variable", "ID", sValueStack[ g_nStackPtr ].lOffset, "" );
               GetIntegerFromAttribute( &lStringLength, g_vXPGView, "Variable", "NElements" );

               sValueStack[ g_nStackPtr ].lpVal = fnWorkStorage( hWRKS, lStringLength + 1 );

               ulMaxLen = lStringLength +1;
               GetBlobFromAttribute( sValueStack[ g_nStackPtr ].lpVal, &ulMaxLen, vStackObject,"Variable", "Value" );

//###blob      GetStringFromAttribute( (zPCHAR) sValueStack[ g_nStackPtr ].lpVal, vStackObject, "Variable", "Value" );

               if ( sWorkExpr.lOffset < 0 || sWorkExpr.lDataType == qTSTRING )
               {
                  sValueStack[ g_nStackPtr ].lDataType = qTPOINTER;
               }
            }
            else
            {
               zULONG ulMaxLen;

               if ( sWorkExpr.lDataType == qTDECIMAL )
               {
                  ulMaxLen = sizeof( sValueStack[ g_nStackPtr ].ldVal );
                  GetBlobFromAttribute( &sValueStack[ g_nStackPtr ].ldVal, &ulMaxLen, vStackObject,"Variable", "Value" );
// ###blob        nRC = GetDecimalFromAttribute( &sValueStack[ g_nStackPtr ].ldVal, vStackObject, "Variable", "Value" );
                  if ( sWorkExpr.lOffset < 0 )
                  {
                     sValueStack[ g_nStackPtr ].lpVal = (zDECIMAL *) &sValueStack[ g_nStackPtr ].ldVal;
                     sValueStack[ g_nStackPtr ].lDataType = qTPOINTER;
                  }
               }
               else
               {
                  ulMaxLen = sizeof( sValueStack[ g_nStackPtr ].lVal );
                  GetBlobFromAttribute( &sValueStack[ g_nStackPtr ].lVal, &ulMaxLen, vStackObject, "Variable", "Value" );
// ###blob        nRC = GetIntegerFromAttribute( &sValueStack[ g_nStackPtr ].lVal, vStackObject, "Variable", "Value" );
                  if ( sWorkExpr.lOffset < 0 )
                  {
                     sValueStack[ g_nStackPtr ].lpVal = &sValueStack[ g_nStackPtr ].lVal;
                     sValueStack[ g_nStackPtr ].lDataType = qTPOINTER;
                  }
               }
            }
            g_nStackPtr++;
            break;
         case qDEFINE:
            sValueStack[ g_nStackPtr ].lOffset = sWorkExpr.lOffset;
            sValueStack[ g_nStackPtr ].lDataType = sWorkExpr.lDataType;

            // Set the data type for when lDataType will be a pointer.
            sValueStack[ g_nStackPtr ].lPtrDataType = sWorkExpr.lDataType;

            if ( sWorkExpr.lDataType == qTSTRING ||
                 sWorkExpr.lDataType == qTCHARACTER ||
                 sWorkExpr.lDataType == qTENTITYNAME ||
                 sWorkExpr.lDataType == qTATTRIBUTENAME )
            {
               zCHAR szTemp[ 256 ]; // should be sufficient for a define

               GetDefineInternalValue( szTemp, sWorkExpr.lOffset );
               sValueStack[ g_nStackPtr ].lpVal = fnWorkStorage( hWRKS, zstrlen( szTemp ) + 1 );

               strcpy_s( (zPCHAR)(sValueStack[ g_nStackPtr ].lpVal), zstrlen( szTemp ) + 1, szTemp );
            }
            else
            {
               if ( sWorkExpr.lDataType == qTDECIMAL )
               {
                  sValueStack[ g_nStackPtr ].ldVal = (double) 0.0;
               }
               else
               {
                  GetDefineInternalValue( szTempNumber, sWorkExpr.lOffset );
                  sValueStack[ g_nStackPtr ].lVal = zatol( szTempNumber );
               }
            }

            g_nStackPtr++;
            break;

         case qCONSTANT:
            sValueStack[ g_nStackPtr ].lDataType = sWorkExpr.lDataType;

            // Set the data type for when lDataType will be a pointer.
            sValueStack[ g_nStackPtr ].lPtrDataType = sWorkExpr.lDataType;

            if ( sWorkExpr.lDataClass != qNUMERIC )
            {
               sValueStack[ g_nStackPtr ].lpVal = (pcExprText + sWorkExpr.lOffset);
            }
            else
            {
               if ( sWorkExpr.lDataType == qTDECIMAL )
               {
                  strcpy_s( szTempNumber, sizeof( szTempNumber ), &pcExprText[ sWorkExpr.lOffset ] );

               // sValueStack[ g_nStackPtr ].ldVal = atof( szTempNumber );
                  SysConvertStringToDecimal( szTempNumber, &sValueStack[ g_nStackPtr ].ldVal );
                  // Also set the integer value (lpVal) since that
                  // value would be used if the constant didn't have
                  // decimal positions.
                  sValueStack[ g_nStackPtr ].lVal = zatol( pcExprText + sWorkExpr.lOffset );
               }
               else
               {
                  sValueStack[ g_nStackPtr ].lVal = zatol( pcExprText +sWorkExpr.lOffset );

                  // Also set a pointer to the character, lest this is a boolean value.
                  sValueStack[ g_nStackPtr ].lpVal = (pcExprText + sWorkExpr.lOffset);
               }
            }

            g_nStackPtr++;
            break;

         case qOPERATIONCALL:
            if ( g_nCallStack[ 0 ] >= CALLSTACK_SIZE -1 )
               TraceLineS( "Internal Error: ", "Interpeter Callstack OVERFLOW" );

            g_nCallStack[ g_nCallStack[ 0 ]++ ] = g_nStackPtr;
            sValueStack[ g_nStackPtr ].lDataType = sWorkExpr.lDataClass;
            sValueStack[ g_nStackPtr ].lPtrDataType = sWorkExpr.lDataType;
            sValueStack[ g_nStackPtr ].lOffset = sWorkExpr.lOffset;

            g_nStackPtr++;
            break;

         case qOPERATOR:
            lRC = 0;
            if ( sWorkExpr.lOffset == qCLOSEOPER )
            {
               lRC = SetUpOperationCall( lpInterpSubtask, (zSHORT) (lExprIndex + sizeof( sQExprStruct ) - MAXSTRING) );

               g_nStackPtr++;
               if ( lRC != zXC_OPERATION_EXECUTED ) // If the operation was only partially completed (the operation is being interpreted) we return now.
                                                    // Otherwise, we process the return code before returning.
               {
                  return( lRC );
               }
            }
            else
            {
               nRC = Operate( sWorkExpr.lOffset, nNumericType, lpInterpSubtask );

               if ( sWorkExpr.lOffset == qEQUAL )
               {
                  g_nStackPtr--;
                  nRC = SetCursorFirstEntityByInteger( vStackObject, "Variable", "ID", sValueStack[ g_nStackPtr ].lOffset, "" );
                  if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
                  {
                     SetAttributeFromBlob( vStackObject, "Variable", "Value", &sValueStack[ g_nStackPtr ].ldVal, sizeof( sValueStack[ g_nStackPtr ].ldVal ) );

 //###blob           SetAttributeFromDecimal( vStackObject, "Variable", "Value", sValueStack[ g_nStackPtr ].ldVal );
                  }
                  else
                  {
                     SetAttributeFromBlob( vStackObject, "Variable", "Value", &sValueStack[ g_nStackPtr ].lVal, sizeof( sValueStack[ g_nStackPtr ].lVal ) );

//###blob            SetAttributeFromInteger( vStackObject, "Variable", "Value", sValueStack[ g_nStackPtr ].lVal );
                  }

                  g_nStackPtr++;
               }
            }

            break;

         default:
            break;
      }

      lExprIndex += sizeof( sQExprStruct ) - MAXSTRING;
   }

   nRC = SetAttributeFromInteger( vStackObject, "OperationCall", "StatementOffset", 0 );
   g_nStackPtr--;
   if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
   {
      *(zPLONG) pResultOfExpression = sValueStack[ g_nStackPtr ].lVal;
      return( zXC_STEP_EXECUTED );
   }
   else
   {
      *(zDECIMAL *) pResultOfExpression = sValueStack[ g_nStackPtr ].ldVal;
      return( zXC_STEP_EXECUTED );
   }
}

zOPER_EXPORT zSHORT OPERATION
Operate( zLONG  lOperator,
         zSHORT nResultType,
         zVIEW  lpInterpSubtask )
{

   // nResultType is the type of the result in an equal expression.
   // 1 is DECIMAL
   // 0 is INTEGER

   if ( lOperator == qCOMMA || lOperator == qLPAREN || lOperator == qRPAREN || lOperator == qOPENOPER )
      return( 0 );

   --g_nStackPtr;

   if ( lOperator != qNOT && lOperator != qUNARYMINUS && g_nStackPtr > 0 )
      --g_nStackPtr;

   switch( lOperator )
   {
      case qGREATERTHANEQUAL:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF integer >= integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal >= sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // IF integer >= decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal >= sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF decimal >= integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal >= sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               // IF decimal >= decimal
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal >= sValueStack[ g_nStackPtr + 1 ].ldVal);
            }
         }
         break;

      case qLESSTHANEQUAL:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF integer <= integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal <= sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // IF integer <= decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal <= sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF decimal <= integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal <= sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               // IF decimal <= decimal
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal <= sValueStack[ g_nStackPtr + 1 ].ldVal);
            }
         }
         break;

      case qGREATERTHAN:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF integer > integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal > sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // IF integer > decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal > sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF decimal > integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal > sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               // IF decimal > decimal
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal > sValueStack[ g_nStackPtr + 1 ].ldVal);
            }
         }
         break;

      case qLESSTHAN:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF integer < integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal < sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // IF integer < decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal < sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // IF decimal < integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal < sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               // IF decimal < decimal
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal < sValueStack[ g_nStackPtr + 1 ].ldVal);
            }
         }
         break;

      case qUNARYMINUS:
         if ( nResultType == 1 ||
              sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
         {
            // Decimal
            if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
            {
               sValueStack[ g_nStackPtr ].ldVal = (ZDecimal)0 - sValueStack[ g_nStackPtr ].ldVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].ldVal = - sValueStack[ g_nStackPtr ].lVal;
            }
         }
         else
         {
            // Integer
            sValueStack[ g_nStackPtr ].lVal = - sValueStack[ g_nStackPtr ].lVal;
         }
         break;


      case qMINUS:
         // We will force the result to decimal unless all three values:  result, first operand and second operand, are not decimal
         if ( nResultType == 1 || sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL || sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
         {
            if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal - sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal - sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal - sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal - sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            sValueStack[ g_nStackPtr ].lDataType = qTDECIMAL;
         }
         else
         {
            sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal - sValueStack[ g_nStackPtr + 1 ].lVal;
         }
         break;

      case qADDITION:
         // We will force the result to decimal unless all three values: result, first operand and second operand, are not decimal
         if ( nResultType == 1 || sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL || sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
         {
            if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal + sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal + sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal + sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal + sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            sValueStack[ g_nStackPtr ].lDataType = qTDECIMAL;
         }
         else
         {
            sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal + sValueStack[ g_nStackPtr + 1 ].lVal;
         }
         break;

      case qMULTIPLICATION:
         // We will force the result to decimal unless all three values: result, first operand and second operand, are not decimal
         if ( nResultType == 1 ||
              sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL || sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
         {
            if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal * sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal * sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal * sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal * sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            sValueStack[ g_nStackPtr ].lDataType = qTDECIMAL;
         }
         else
         {
            sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal * sValueStack[ g_nStackPtr + 1 ].lVal;
         }
         break;

      case qDIVISION:

         if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL && sValueStack[ g_nStackPtr + 1 ].ldVal == 0 )
         {
            MessageSend( lpInterpSubtask, "VM00204", "VML Interpretor",
                         "Divisor in division statement is 0.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            break;
         }

         if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER && sValueStack[ g_nStackPtr + 1 ].lVal == 0 )
         {
            MessageSend( lpInterpSubtask, "VM00204", "VML Interpretor",
                         "Divisor in division statement is 0.",
                         zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
            break;
         }

           if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL || sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
         {
            if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
            {
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal / sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal / sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].lVal;
               if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTDECIMAL )
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal / sValueStack[ g_nStackPtr + 1 ].ldVal;
               else
                  sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal / sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            sValueStack[ g_nStackPtr ].lDataType = qTDECIMAL;
         }
         else
         {
            sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal / sValueStack[ g_nStackPtr + 1 ].lVal;
         }
         break;

      case qEQUAL:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].ldVal = sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         break;

      case qNOTEQUAL:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal != sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal != sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal != sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal != sValueStack[ g_nStackPtr + 1 ].ldVal);
            }
         }
         break;

      case qCONDEQUAL:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal == sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal == sValueStack[ g_nStackPtr + 1 ].ldVal;
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ( sValueStack[ g_nStackPtr ].ldVal == sValueStack[ g_nStackPtr + 1 ].lVal );
            }
            else
            {
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ( sValueStack[ g_nStackPtr ].ldVal == sValueStack[ g_nStackPtr + 1 ].ldVal );
            }
         }
         break;

      case qOR:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // Integer OR Integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal || sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // Integer OR Decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal || (sValueStack[ g_nStackPtr + 1 ].ldVal != 0);
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // Decimal OR Integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ( (sValueStack[ g_nStackPtr ].ldVal != 0) || sValueStack[ g_nStackPtr + 1 ].lVal );
            }
            else
            {
               // Decimal OR Decimal (TB632, HH, 20.5.99)
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ((sValueStack[ g_nStackPtr ].ldVal != 0) || (sValueStack[ g_nStackPtr + 1 ].ldVal != 0));
            }
         }
         break;

      case qAND:
         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // Integer AND Integer
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal && sValueStack[ g_nStackPtr + 1 ].lVal;
            }
            else
            {
               // Integer AND Decimal
               sValueStack[ g_nStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal && (sValueStack[ g_nStackPtr + 1 ].ldVal != 0);
            }
         }
         else
         {
            if ( sValueStack[ g_nStackPtr + 1 ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr + 1 ].lDataType == qTSHORT || sValueStack[ g_nStackPtr + 1 ].lDataType == qTVIEW )
            {
               // Decimal AND Integer
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ((sValueStack[ g_nStackPtr ].ldVal != 0) && sValueStack[ g_nStackPtr + 1 ].lVal);
            }
            else
            {
               // Decimal AND Decimal
               sValueStack[ g_nStackPtr ].ldVal = (zLONG) ((sValueStack[ g_nStackPtr ].ldVal != 0) && (sValueStack[ g_nStackPtr + 1 ].ldVal != 0));
            }
         }
         break;

      case qNOT:
         if ( g_nStackPtr < 0 )
            g_nStackPtr = 0;

         if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER || sValueStack[ g_nStackPtr ].lDataType == qTSHORT || sValueStack[ g_nStackPtr ].lDataType == qTVIEW )
         {
            sValueStack[ g_nStackPtr ].lVal = ! sValueStack[ g_nStackPtr ].lVal;
         }
         else
         {
            sValueStack[ g_nStackPtr ].ldVal = (zLONG) (sValueStack[ g_nStackPtr ].ldVal == 0);
         }
         break;

      default:
         break;
   }
   g_nStackPtr++;
   return( 0 );
}

zOPER_EXPORT zLONG OPERATION
SetUpOperationCall( zVIEW  vInterpSubtask,
                    zSHORT nIndex )
{
   zVIEW lpAppSubtask;
   zVIEW vXPG;
   zVIEW vStatementView;

#ifdef __WIN32__
   zLONG  lParmList[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#else
   zSHORT lParmList[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
#endif

   zSHORT nDefinedParmList[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   zCHAR  cParmListType[ ] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
   zSHORT   nParmIndex;
   zLONG    lZero;
   zSHORT   nDefinedParmIndex;
   zSHORT   nLoopCounter;
   zSHORT   nStartStackPtr;
   zSHORT   i;
   zLONG    lMode;
   zSHORT   nRC;
   zLONG    lRC;
   zSHORT   lPosOfOper;
   zSHORT   nParmPushSize;
   FARPROC    lf;
   ZDecimal   dValue;
   LPLIBRARY  hLib;
   zCHAR    szReturnName[ 65 ];
   zCHAR    szObjectName[ 33 ];
   zCHAR    szModuleName[ 50 ];
   zCHAR    szCurrentName[ 50 ];
   zCHAR    szMsg[ 100 ];
   zVIEW    vStackObject;                  // view to the stack object
   zLONG    lOperReturnType;

   // Get the stack object as it was loaded with the XPG
   nRC = GetViewByName( &vStackObject, "StackObject", vInterpSubtask, zLEVEL_SUBTASK );

   nParmIndex = 0;

   // pop all of the stack values until an operation ordinal is found
   g_nStackPtr--;
   nStartStackPtr = g_nStackPtr;
   g_nCallStack[ 0 ]--;
   lPosOfOper = g_nCallStack[ g_nCallStack[ 0 ] ];
// g_nCallStack[ g_nCallStack[ 0 ] ] = 0; // ??

   if ( sValueStack[ lPosOfOper ].lDataType == qZEIDONOPERATION ||
        sValueStack[ lPosOfOper ].lDataType == qGLOBALOPERATION ||
        sValueStack[ lPosOfOper ].lDataType == qLOCALOPERATION ||
        sValueStack[ lPosOfOper ].lDataType == qOBJECTOPERATION )
   {
      nRC = GetOperationNameFromZKeyM2( vInterpSubtask, szReturnName, sizeof( szReturnName ), szObjectName, sizeof( szObjectName ),
                                        sValueStack[ lPosOfOper ].lDataType, sValueStack[ lPosOfOper ].lOffset );
   }
   else
   {
      if ( sValueStack[ lPosOfOper ].lpVal )
         strcpy_s( szReturnName, sizeof( szReturnName ), (zCPCHAR) sValueStack[ lPosOfOper ].lpVal );

      sValueStack[ lPosOfOper ].lOffset = 0;
   }

   // If the Operation is for an Object, use the Object Name, prefixed by the constant "o", as the prefix for the Operation Name.
   if ( sValueStack[ lPosOfOper ].lDataType == qOBJECTOPERATION )
   {
      strcpy_s( szCurrentName, sizeof( szCurrentName ), "o" );
      strcat_s( szCurrentName, sizeof( szCurrentName ), szObjectName );
      strcat_s( szCurrentName, sizeof( szCurrentName ), "_" );
      strcat_s( szCurrentName, sizeof( szCurrentName ), szReturnName );
      szCurrentName[ 32 ] = 0;
      strcpy_s( szReturnName, sizeof( szReturnName ), szCurrentName );
   }

   GetOperationModuleName( vInterpSubtask, szModuleName, sValueStack[ lPosOfOper ].lOffset, sValueStack[ lPosOfOper ].lDataType );
   GetOperationParmList( vInterpSubtask, nDefinedParmList, sValueStack[ lPosOfOper ].lOffset,
// DGC 9/10/2002         (zPCHAR) sValueStack[ lPosOfOper ].lpVal,
                         (zPCHAR) szReturnName,
                         sValueStack[ lPosOfOper ].lDataType );
   GetViewByName( &lpAppSubtask, "ApplicationView", vInterpSubtask, zLEVEL_SUBTASK );

   if ( sValueStack[ lPosOfOper ].lDataType == qZEIDONOPERATION )
      strcpy_s( szCurrentName, sizeof( szCurrentName ), "" );
   else
   if ( sValueStack[ lPosOfOper ].lDataType == qOBJECTOPERATION )
      strcpy_s( szCurrentName, sizeof( szCurrentName ), "LOD" );
   else
   if ( sValueStack[ lPosOfOper ].lDataType == qGLOBALOPERATION )
      strcpy_s( szCurrentName, sizeof( szCurrentName ), "Global" );
   else
      strcpy_s( szCurrentName, sizeof( szCurrentName ), "Dialog" );

   if ( sValueStack[ g_nStackPtr ].lDataType == qZEIDONOPERATION )
      lMode = -1;
   else
      lMode = GetExecMode( vInterpSubtask, lpAppSubtask, szModuleName, szReturnName, szCurrentName );

   nDefinedParmIndex = 0;
#if 0
   while ( sValueStack[ g_nStackPtr ].lDataType != qZEIDONOPERATION && sValueStack[ g_nStackPtr ].lDataType != qOBJECTOPERATION &&
           sValueStack[ g_nStackPtr ].lDataType != qGLOBALOPERATION && sValueStack[ g_nStackPtr ].lDataType != qLOCALOPERATION )
#endif

   for ( g_nStackPtr = lPosOfOper + 1;
         g_nStackPtr <= nStartStackPtr;
         g_nStackPtr++ )
   {
      nDefinedParmIndex++;

      if ( nDefinedParmList[ nDefinedParmIndex ] < 0 )
         nDefinedParmList[ nDefinedParmIndex ] = qTPOINTER;

      // put backwards on the parm stack
      if ( nDefinedParmList[ nDefinedParmIndex ] == qTPOINTER )
      {
         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

         // Set the data type for a pointer.
         if ( sValueStack[ g_nStackPtr ].lPtrDataType == qTDECIMAL )
            cParmListType[ nParmIndex ] = zTYPE_DECIMAL;
         else
         if ( sValueStack[ g_nStackPtr ].lPtrDataType == qTINTEGER )
            cParmListType[ nParmIndex ] = zTYPE_INTEGER;
         else
         if ( sValueStack[ g_nStackPtr ].lPtrDataType == qTVIEW )
            cParmListType[ nParmIndex ] = zTYPE_INTEGER;
         else
            cParmListType[ nParmIndex ] = zTYPE_STRING;

#ifdef __WIN32__
         lParmList[ nParmIndex++ ] = (zLONG) sValueStack[ g_nStackPtr ].lpVal;
#else
         if ( nParmPushSize == 2 || nParmPushSize == 0 )
         {
            lParmList[ nParmIndex-- ] = LOWORD( sValueStack[ g_nStackPtr ].lpVal );
            lParmList[ nParmIndex-- ] = HIWORD( sValueStack[ g_nStackPtr ].lpVal );
         }
         else
         if ( nParmPushSize == 1 )
         {
            lParmList[ nParmIndex-- ] = LOWORD( sValueStack[ g_nStackPtr ].lpVal );
         }
#endif

      }
      else
      if ( nDefinedParmList[ nDefinedParmIndex ] == qTINTEGER || nDefinedParmList[ nDefinedParmIndex ] == qTVIEW )
      {
         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

#ifdef __WIN32__
         if ( nParmPushSize == 1 )
            lParmList[ nParmIndex++ ] = sValueStack[ g_nStackPtr ].lVal;
#else
         if ( nParmPushSize == 2 || nParmPushSize == 0 )
         {
            lParmList[ nParmIndex-- ] = LOWORD( sValueStack[ g_nStackPtr ].lVal );
            lParmList[ nParmIndex-- ] = HIWORD( sValueStack[ g_nStackPtr ].lVal );
         }
         else
         if ( nParmPushSize == 1 )
         {
            lParmList[ nParmIndex-- ] = LOWORD( sValueStack[ g_nStackPtr ].lVal );
         }
#endif
      }
      else
      if ( nDefinedParmList[ nDefinedParmIndex ] == qTSHORT )
      {
         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

#ifdef __WIN32__
         if ( nParmPushSize == 1 )
            lParmList[ nParmIndex++ ] = sValueStack[ g_nStackPtr ].lVal;
#else
         if ( nParmPushSize == 2 )
         {
            lParmList[ nParmIndex-- ] = (zSHORT) sValueStack[ g_nStackPtr ].lVal;
            lParmList[ nParmIndex-- ] = 0;
         }
         else
         if ( nParmPushSize == 1 || nParmPushSize == 0 )
            lParmList[ nParmIndex-- ] = (zSHORT) sValueStack[ g_nStackPtr ].lVal;
#endif
      }
      else
      if ( nDefinedParmList[ nDefinedParmIndex ] == qTDECIMAL )
      {
#ifdef __WIN32__
         zPLONG pl;
         zSHORT i;
         ZDecimal dTemp;

         if ( sValueStack[ g_nStackPtr ].lDataType != qTDECIMAL )
         {
            // the value is integer or short and must be converted to decimal - perform cast.
            dTemp = sValueStack[ g_nStackPtr ].lVal;

            // staged operation will use ldval
            sValueStack[ g_nStackPtr ].ldVal = dTemp;
            pl = (zPLONG) &dTemp;
         }
         else
         {
            pl = (zPLONG)(zDECIMAL *) &sValueStack[ g_nStackPtr ].ldVal;
         }

         // Copy the decimal structure to longs.  We need to work backwards.  'i' is the # of longs that fit into a decimal structure.
         for ( i = 0; i < sizeof( zDECIMAL ) / sizeof( zLONG ); i++)
            lParmList[ nParmIndex++ ] = pl[ i ];
#else
         LPBYTE pByte, lByte;
         int    j, s = 0;

         pByte = (LPBYTE) &lParmList[ nParmIndex ];
         lByte = (LPBYTE) &sValueStack[ g_nStackPtr ].ldVal.dDecValue;
         s = sizeof( zDECIMAL ) / 2;
         for ( j = 0; j < s; j++ )
         {
            *pByte++ = *lByte++;
            *pByte = *lByte++;
            *pByte--; *pByte--; *pByte--;
         }

         nParmIndex -= sizeof( zDECIMAL ) / 2; /* was 5 */
#endif
      }
      else
      if ( nDefinedParmList[ nDefinedParmIndex ] == qTCHARACTER )
      {
         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

#ifdef __WIN32__
         if ( nParmPushSize == 1 )
            lParmList[ nParmIndex++ ] = (zLONG) * (zPCHAR) sValueStack[ g_nStackPtr ].lpVal;
#else
         if ( nParmPushSize == 1 )
         {
            lParmList[ nParmIndex-- ] = (zSHORT)(zCHAR)*((zPCHAR)sValueStack[ g_nStackPtr ].lpVal);
         }
#endif
      }
      else // if ( sValueStack[ g_nStackPtr ].lDataType == qTSTRING )
      {
         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

#ifdef __WIN32__
         if ( nParmPushSize == 1 )
         {
            if ( sValueStack[ g_nStackPtr ].lDataType == qTINTEGER )
            {
               // it is possible, that a string parameter is an integer value.
               //  Example SetCursorFirst(,, ScopingEntity = 0 )
               //  in that case, take the integer value 0 rather than string value "0"
               lParmList[ nParmIndex++ ] = (zLONG) sValueStack[ g_nStackPtr ].lVal;
            }
            else
            {
               lParmList[ nParmIndex++ ] = (zLONG) sValueStack[ g_nStackPtr ].lpVal;
            }
         }
#else
         if ( nParmPushSize == 2 || nParmPushSize == 0 )
         {
            lParmList[ nParmIndex-- ] = LOWORD( (zPCHAR) sValueStack[ g_nStackPtr ].lpVal );
            lParmList[ nParmIndex-- ] = HIWORD( (zPCHAR) sValueStack[ g_nStackPtr ].lpVal );
         }
         else
         if ( nParmPushSize == 1 )
         {
            lParmList[ nParmIndex-- ] = LOWORD((zPCHAR)sValueStack[ g_nStackPtr ].lpVal );
         }
#endif
      }

//      g_nStackPtr--;

   } // for...

   if ( lMode == zXC_OPERATION_STAGED || lMode == zXC_SETUP_DEBUGGER )
   {
      // The following code is used when the operation being called is also to be interpreted. If the operation being called is to be
      // executed as a DLL, we fall out the bottom of this code and call the routine. If the operation is being interpreted, we execute
      // the following code to set up the parameters in the stack object (vStackObject) and do other initialization. At the end of this
      // code, we return back a level, where the code exists to interpret the operation.

      if ( g_nCallStack[ 0 ] >= CALLSTACK_SIZE -1 )
         TraceLineS( "Internal Error: ", "Interpeter Callstack OVERFLOW" );

      g_nCallStack[ 0 ]++;
      nRC = GetViewByName( &vStatementView, "StatementView", vInterpSubtask, zLEVEL_SUBTASK );
      nRC = UpdateStackObjectFromViews( vInterpSubtask, g_vXPGView, vStatementView, nIndex, nStartStackPtr );
      g_nStackPtr = nStartStackPtr;
      StageOperationCall( vInterpSubtask, lpAppSubtask, szModuleName, szReturnName );

      GetViewByName( &vXPG, "XPG", vInterpSubtask, zLEVEL_SUBTASK );
      GetViewByName( &vStatementView, "StatementView", vInterpSubtask, zLEVEL_SUBTASK );

      // Get the stack object as it was loaded with the XPG
      GetViewByName( &vStackObject, "StackObject", vInterpSubtask, zLEVEL_SUBTASK );

#ifdef __WIN32__
      nParmIndex = 0;
      nDefinedParmIndex = 1;
#else
      nParmIndex = 35;
      nDefinedParmIndex = nDefinedParmList[ 0 ];
#endif

      g_nStackPtr = lPosOfOper + 1;

      lZero = 0;
      SetEntityCursor( vStackObject, "Variable", "ParameterFlag", zPOS_LAST | zQUAL_INTEGER | zQUAL_GREATER_THAN, &lZero, "", "", 0, "", "" );

      // After the previous SetEntityCursor, we are positioned on the last parameter to be passed, which is correct for 16 bit but not for
      // 32 bit. Therefore for 32 bit, we must back up to the first parameter in the list. This is accomplished by setting cursor previous n times,
      // where n is 1 less than the number of parms to be passed.
      //
#ifdef __WIN32__
      for ( i = 1; i < nDefinedParmList[ 0 ]; i++ )
         SetCursorPrevEntity( vStackObject, "Variable", "" );
#endif

      nLoopCounter = 0;
      while ( nLoopCounter < nDefinedParmList[ 0 ] )
      {
         zULONG nTemp;

         nParmPushSize = GetParmPushSize( nDefinedParmList[ nDefinedParmIndex ] );

#ifdef __WIN32__
         if ( nParmPushSize != 0 )
         {
            zSHORT nParmIncrement = 1;
            nTemp = lParmList[ nParmIndex ];

            if ( nDefinedParmList[ nDefinedParmIndex ] == qTPOINTER )
            {
               zULONG uLth;

               switch ( cParmListType[ nParmIndex ] )
               {
               case zTYPE_DECIMAL:
                  uLth = sizeof( zDECIMAL );
                  break;
               case zTYPE_INTEGER:
                  uLth = sizeof( zLONG );
                  break;
               case zTYPE_STRING:
                  uLth = zstrlen( (zPCHAR)nTemp );
                  break;
               default:
                  // we should not get here
                  TraceLineI( "Interpreter Error SetupOperationCall ParmListType=", nDefinedParmList[ nDefinedParmIndex ] );
                  uLth = sizeof( zLONG ); // assume integer size ??
                  break;
               }

               SetAttributeFromBlob( vStackObject, "Variable", "Value", (zPVOID) nTemp, uLth );

//###blob     nRC = SetAttributeFromVariable( vStackObject, "Variable", "Value", nTemp, cParmListType[ nParmIndex ], 1024, "", 0 );
            }
            else
            if ( nDefinedParmList[ nDefinedParmIndex ] == qTDECIMAL )
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", &sValueStack[ g_nStackPtr ].ldVal, sizeof(sValueStack[ g_nStackPtr ].ldVal) );
//###blob      SetAttributeFromDecimal( vStackObject, "Variable", "Value", sValueStack[ g_nStackPtr ].ldVal );
               // **HH** 14.10.1998
               //  note, that nParmIndex was incremented by the number of longs which are used to hold a zDECIMAL.
               nParmIncrement = sizeof( zDECIMAL ) / sizeof( zLONG );
            }
            else
            if ( nDefinedParmList[ nDefinedParmIndex ] == qTSTRING )
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", (zPVOID) nTemp, zstrlen( (zPCHAR)nTemp ) +1 );
//###blob      SetAttributeFromString( vStackObject, "Variable", "Value", (zPCHAR) nTemp );
            }
            else
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", &nTemp, sizeof ( zLONG ) );
//###blob      SetAttributeFromInteger( vStackObject, "Variable", "Value", nTemp );
            }

            nParmIndex += nParmIncrement;
            nStartStackPtr++;
            g_nStackPtr++;
         }
#else
         if ( nParmPushSize == 4 )
         {
            LPBYTE pByte, lByte;

            lByte = (LPBYTE) &dValue;
            pByte = (LPBYTE) &lParmList[ nParmIndex ];

            *lByte = *pByte;
            *(lByte+1) = *(pByte+1);
            *(lByte+2) = *(pByte-2);
            *(lByte+3) = *(pByte-1);
            *(lByte+4) = *(pByte-4);
            *(lByte+5) = *(pByte-3);
            *(lByte+6) = *(pByte-6);
            *(lByte+7) = *(pByte-5);

            SetAttributeFromDecimal( vStackObject, "Variable", "Value", dValue );

            nParmIndex -= 4;
         }
         else
         if ( nParmPushSize == 2 || nParmPushSize == 0 )
         {
            zULONG nTemp;

            nTemp = MAKELONG( lParmList[ nParmIndex ], lParmList[ nParmIndex - 1 ] );

            if ( nDefinedParmList[ nDefinedParmIndex ] == qTPOINTER )
               nRC = SetAttributeFromVariable( vStackObject, "Variable", "Value", (zPCHAR) nTemp, cParmListType[ nParmIndex ], 1024, "", 0 );
            else
            if ( nDefinedParmList[ nDefinedParmIndex ] == qTSTRING )
               SetAttributeFromString( vStackObject, "Variable", "Value", (zPCHAR) nTemp );
            else
               SetAttributeFromInteger( vStackObject, "Variable", "Value", nTemp );

            nParmIndex -= 2;
         }
         else
         {
            SetAttributeFromInteger( vStackObject, "Variable", "Value", lParmList[ nParmIndex ] );
            nParmIndex -= 1;
         }
#endif


#ifdef __WIN32__
         SetCursorNextEntity( vStackObject, "Variable", "" );
         nDefinedParmIndex++;
#else
         SetCursorPrevEntity( vStackObject, "Variable", "" );
         nDefinedParmIndex--;
#endif

         nLoopCounter++;
      }

      // When the operation being executed is staged (meaning we will be executing the operation interpretively), the value of g_nStackPtr
      // needs to be the same as nStartStackPtr.
      g_nStackPtr = nStartStackPtr;

      if ( lMode == zXC_SETUP_DEBUGGER )
         return( zXC_SETUP_DEBUGGER );
      else
         return( zXC_OPERATION_STAGED );
   }

   // When the operation being executed is not staged (meaning we will issue a C call to the operation below), the value of g_nStackPtr
   // needs to be the same as lPosOfOper.
   g_nStackPtr = lPosOfOper;

   if ( szModuleName[ 0 ] == 0 )
      hLib = SysLoadLibrary( vInterpSubtask, "KZOENGAA" );
   else
      hLib = SysLoadLibrary( lpAppSubtask, szModuleName );

   if ( hLib == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Executable module: " );
      strcat_s( szMsg, sizeof( szMsg ), szModuleName );
      strcat_s( szMsg, sizeof( szMsg ), ", \n   was not found." );
      MessageSend( vInterpSubtask, "VM00201", "VML Interpretor", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   lf = (FARPROC) SysGetProc( hLib, szReturnName );
   if ( lf == 0 )
   {
      strcpy_s( szMsg, sizeof( szMsg ), "Operation: " );
      strcat_s( szMsg, sizeof( szMsg ), szReturnName );
      strcat_s( szMsg, sizeof( szMsg ), ",\n  was not found in module: " );
      strcat_s( szMsg, sizeof( szMsg ), szModuleName );
      strcat_s( szMsg, sizeof( szMsg ), "." );
      MessageSend( vInterpSubtask, "VM00202", "VML Interpretor", szMsg, zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
      return( 0 );
   }

   lOperReturnType = sValueStack[ g_nStackPtr ].lPtrDataType;
   switch ( sValueStack[ g_nStackPtr ].lDataType )
   {
   case qDIALOGOPERATION: //      4004
      lOperReturnType = qTSHORT;
      break;
   case qLOCALOPERATION:   //     4001
   case qZEIDONOPERATION:  //     4006
   case qGLOBALOPERATION:  //     4002
   case qOBJECTOPERATION:  //     4003
   case qCONTROLOPERATION: //     4007
      break; // should not come here
   }
   if ( lOperReturnType == qTDECIMAL )
   {
      // returnvalue is decimal
#ifdef __WIN32__
      typedef zDECIMAL (POPERATION PFN_VML)( zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG );
#else
      typedef zDECIMAL (POPERATION PFN_VML)( zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT );
#endif
      PFN_VML df = (PFN_VML) lf;

      dValue = (*df)( lParmList[ 0 ], lParmList[ 1 ],
                     lParmList[ 2 ], lParmList[ 3 ], lParmList[ 4 ],
                     lParmList[ 5 ], lParmList[ 6 ], lParmList[ 7 ],
                     lParmList[ 8 ], lParmList[ 9 ], lParmList[ 10],
                     lParmList[ 11], lParmList[ 12], lParmList[ 13],
                     lParmList[ 14], lParmList[ 15], lParmList[ 16],
                     lParmList[ 17], lParmList[ 18], lParmList[ 19],
                     lParmList[ 20], lParmList[ 21], lParmList[ 22],
                     lParmList[ 23], lParmList[ 24], lParmList[ 25],
                     lParmList[ 26], lParmList[ 27], lParmList[ 28],
                     lParmList[ 29], lParmList[ 30], lParmList[ 31],
                     lParmList[ 32], lParmList[ 33], lParmList[ 34],
                     lParmList[ 35] );
      sValueStack[ g_nStackPtr ].ldVal = dValue;
   }
   else
   if ( lOperReturnType == qTINTEGER )
   {
      // returnvalue is long
#ifdef __WIN32__
      typedef zLONG (POPERATION PFN_VML)( zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG );
#else
      typedef zLONG (POPERATION PFN_VML)( zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT );
#endif
      PFN_VML df = (PFN_VML) lf;

      lRC = (*df)( lParmList[ 0 ], lParmList[ 1 ],
                     lParmList[ 2 ], lParmList[ 3 ], lParmList[ 4 ],
                     lParmList[ 5 ], lParmList[ 6 ], lParmList[ 7 ],
                     lParmList[ 8 ], lParmList[ 9 ], lParmList[ 10],
                     lParmList[ 11], lParmList[ 12], lParmList[ 13],
                     lParmList[ 14], lParmList[ 15], lParmList[ 16],
                     lParmList[ 17], lParmList[ 18], lParmList[ 19],
                     lParmList[ 20], lParmList[ 21], lParmList[ 22],
                     lParmList[ 23], lParmList[ 24], lParmList[ 25],
                     lParmList[ 26], lParmList[ 27], lParmList[ 28],
                     lParmList[ 29], lParmList[ 30], lParmList[ 31],
                     lParmList[ 32], lParmList[ 33], lParmList[ 34],
                     lParmList[ 35] );
      sValueStack[ g_nStackPtr ].lDataType = qTINTEGER;
      sValueStack[ g_nStackPtr ].lVal = lRC;
   }
   else
   {
      // return value is short
#ifdef __WIN32__
      typedef zSHORT   (POPERATION PFN_VML)( zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG,
                                             zLONG, zLONG, zLONG, zLONG );
#else
      typedef zSHORT   (POPERATION PFN_VML)( zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT,
                                             zSHORT, zSHORT, zSHORT, zSHORT );
#endif
      PFN_VML df = (PFN_VML) lf;

      nRC = (*df)( lParmList[ 0 ], lParmList[ 1 ],
                     lParmList[ 2 ], lParmList[ 3 ], lParmList[ 4 ],
                     lParmList[ 5 ], lParmList[ 6 ], lParmList[ 7 ],
                     lParmList[ 8 ], lParmList[ 9 ], lParmList[ 10],
                     lParmList[ 11], lParmList[ 12], lParmList[ 13],
                     lParmList[ 14], lParmList[ 15], lParmList[ 16],
                     lParmList[ 17], lParmList[ 18], lParmList[ 19],
                     lParmList[ 20], lParmList[ 21], lParmList[ 22],
                     lParmList[ 23], lParmList[ 24], lParmList[ 25],
                     lParmList[ 26], lParmList[ 27], lParmList[ 28],
                     lParmList[ 29], lParmList[ 30], lParmList[ 31],
                     lParmList[ 32], lParmList[ 33], lParmList[ 34],
                     lParmList[ 35] );
      sValueStack[ g_nStackPtr ].lDataType = qTINTEGER;
      sValueStack[ g_nStackPtr ].lVal = nRC;
   }


   SetPointerValues( &nStartStackPtr, vInterpSubtask );

   // 7/27
   g_nCallStack[ g_nCallStack[ 0 ] ] = 0; // ??

   return( zXC_OPERATION_EXECUTED );
}

static zSHORT
GetOperationNameFromZKeyM2( zVIEW  vSubtask,
                            zPCHAR pchOperationName,
                            zLONG  lOperNameLth,
                            zPCHAR pchObjectName,
                            zLONG  lObjectNameLth,
                            zLONG  lOperationClass,
                            zLONG  lZKey )
{
   zSHORT nRC;                         // temp return code
   zCHAR szLookupEName[ 32 ];
   zVIEW vLookupView;
   zVIEW vSrcView;
   zVIEW vTaskLPLR;
   zLONG lSourceZKey;

   // Check to see if the TaskLPLR is up.  If it is not, then we must have been called by Core so there's no LPLR active.  In this case we'll just retrieve
   // the op name from the XPG.
   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   strcpy_s( szLookupEName, sizeof( szLookupEName ), "Operation" );
   switch ( lOperationClass )
   {
      case qLOCALOPERATION:
         CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
         break;

      case qOBJECTOPERATION:
         if ( vTaskLPLR == 0 )
         {
            SetCursorFirstEntityByInteger( g_vXPGView, "ExternalOperation", "ZKey", lZKey, 0 );
            GetStringFromAttribute( pchOperationName, lOperNameLth, g_vXPGView, "ExternalOperation", "Name" );
            return( (zSHORT) lZKey );
         }

         CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
         strcpy_s( szLookupEName, sizeof( szLookupEName ), "ExternalOperation" );
         SetCursorFirstEntityByInteger( vLookupView, szLookupEName, "ZKey", lZKey, "" );
         GetIntegerFromAttribute( &lSourceZKey, vLookupView, "ExternalOperation", "SourceZKey" );

         // activate it from configuration management.
         ActivateMetaOI_ByZKey( vSubtask, &vSrcView, 0, zREFER_LOD_META, zSINGLE, lSourceZKey, zCURRENT_OI );
         nRC = SetCursorFirstEntityByInteger( vSrcView, "Operation", "ZKey", lZKey, "LOD" );
         if ( nRC > zCURSOR_UNCHANGED )    // found
         {
            GetStringFromAttribute( pchOperationName, lOperNameLth, vSrcView, "Operation", "Name" );
            GetStringFromAttribute( pchObjectName, lObjectNameLth, vSrcView, "LOD", "Name" );
            DropView( vLookupView );
            DropMetaOI( vSubtask, vSrcView );
            return( (zSHORT) lZKey );
         }
         else
         {
            DropView( vLookupView );
            DropMetaOI( vSubtask, vSrcView );
            return( -1 );
         }

      case qGLOBALOPERATION:
      {
         zVIEW vTempList;

         if ( vTaskLPLR == 0 )
         {
            SetCursorFirstEntityByInteger( g_vXPGView, "ExternalOperation", "ZKey", lZKey, 0 );
            GetStringFromAttribute( pchOperationName, lOperNameLth, g_vXPGView, "ExternalOperation", "Name" );
            return( (zSHORT) lZKey );
         }

         RetrieveViewForMetaList( vSubtask, &g_lpGOListView, zREFER_GO_META );
         SetCursorFirstEntityByInteger( g_lpGOListView, "W_MetaDef", "CPLR_ZKey", lZKey, "" );

         // Save the current meta type cursor by creating a temp view and passing it to ActivateMeta.  ActivateMeta will change the meta
         // type to Global Op Group.
         CreateViewFromViewForTask( &vTempList, g_lpGOListView, 0 );

         ActivateMetaOI( vSubtask, &vLookupView, vTempList, zREFER_GO_META, zSINGLE | zLEVEL_APPLICATION );

         DropView( vTempList );

         break;
      }

      case qZEIDONOPERATION:
         oTZOPGRPO_GetViewForXGO( &vLookupView, zSYS_CURRENT_OI );
         break;

      default:
         return( -1 );
   }

   if ( vLookupView == 0 )
      return( -1 );

   nRC =
      SetCursorFirstEntityByInteger( vLookupView, szLookupEName, "ZKey", lZKey, "" );
   if ( nRC  > zCURSOR_UNCHANGED )    // found
   {
      GetStringFromAttribute( pchOperationName, lOperNameLth, vLookupView, szLookupEName, "Name" );
      DropView( vLookupView );
      return( (zSHORT) lZKey );
   }
   else
   {
      DropView( vLookupView );
      return( -1 );
   }
}

zOPER_EXPORT zSHORT OPERATION
GetOperationParmList( zVIEW  vSubtask,
                      zSHORT plParmList[],
                      zLONG  lZKey,
                      zPCHAR pchOperationName,
                      zLONG  lOperationClass )
{
   zSHORT nRC;                         // temp return code
   zSHORT nCheckParms;
   zLONG  lParmType;
   zCHAR  cZeidonType[ 2 ];
   zCHAR  cPtrFlag[ 2 ];
   zVIEW  vLookupView;
   zVIEW  vTaskLPLR;
   zCHAR  szLookupEName[ 32 ];

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vTaskLPLR == 0 )
   {
      strcpy_s( szLookupEName, sizeof( szLookupEName ), "ExternalOperation" );
      CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
   }
   else
   {
      strcpy_s( szLookupEName, sizeof( szLookupEName ), "Operation" );
      switch( lOperationClass )
      {
         case qLOCALOPERATION:
            CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
            nRC = SetCursorFirstEntityByInteger( vLookupView, "Operation", "ZKey", lZKey, "" );
            if ( nRC == zCURSOR_UNCHANGED )    // found
               nRC = SetCursorFirstEntityByString( vLookupView, szLookupEName, "Name", pchOperationName, "" );

            if ( nRC > zCURSOR_UNCHANGED )    // found
            {
               zLONG lZero = 0;
               zLONG lPtrFlag = 0;

               // STOP
               nCheckParms = 1;
               nRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag", zPOS_FIRST | zQUAL_INTEGER | zQUAL_GREATER_THAN, &lZero, "", "", 0, "", "" );
               while ( nRC > zCURSOR_UNCHANGED )
               {
                  GetIntegerFromAttribute( &lParmType, vLookupView, "Variable", "DataType" );
                  GetIntegerFromAttribute( &lPtrFlag, vLookupView, "Variable", "ParameterFlag" );
                  if ( lPtrFlag > 1 )
                  {
                     if ( lParmType > 0 )
                        lParmType *= -1;
                  }

                  plParmList[ nCheckParms ] = (zSHORT) lParmType;

                  nCheckParms++;

                  nRC = SetEntityCursor( vLookupView, "Variable", "ParameterFlag", zPOS_NEXT | zQUAL_INTEGER | zQUAL_GREATER_THAN, &lZero, "", "", 0, "", "" );
               }

               nCheckParms--;
               plParmList[ 0 ] = nCheckParms;

               DropView( vLookupView );
               return( 0 );
            }

            break;

         case qOBJECTOPERATION:
            CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
            strcpy_s( szLookupEName, sizeof( szLookupEName ), "ExternalOperation" );
            break;

         case qGLOBALOPERATION:
            ActivateMetaOI_ByZKey( vSubtask, &vLookupView, 0, zREFER_GO_META, zSINGLE, lZKey, zCURRENT_OI );

            SetCursorFirstEntityByInteger( vLookupView, "Operation", "ZKey", lZKey, "" );
            break;

         case qZEIDONOPERATION:
            oTZOPGRPO_GetViewForXGO( &vLookupView, zSYS_CURRENT_OI );
            break;

         default:
            nCheckParms = 0;
            while ( nCheckParms < 20 )
               plParmList[ nCheckParms++ ] = 0;

            nCheckParms = 0;
            return( -1 );
            break;
      }
   }

   nRC = SetCursorFirstEntityByInteger( vLookupView, szLookupEName, "ZKey", lZKey, "" );

   nCheckParms = 0;
   if ( nRC > zCURSOR_UNCHANGED )    // found
   {
      nCheckParms = 1;
      nRC = SetCursorFirstEntity( vLookupView, "Parameter", "" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nRC = GetStringFromAttribute( cZeidonType, sizeof( cZeidonType ), vLookupView, "Parameter", "DataType" );

         lParmType = ConvertZeidonTypeToVMLTypeS( cZeidonType[ 0 ] );
         GetStringFromAttribute( cPtrFlag, sizeof( cPtrFlag ), vLookupView, "Parameter", "PFlag" );
         if ( cPtrFlag[ 0 ] == 'Y' )
         {
            if ( lParmType > 0 )
               lParmType *= -1;
         }

         plParmList[ nCheckParms ] = (zSHORT) lParmType;

         nCheckParms++;
         nRC = SetCursorNextEntity( vLookupView, "Parameter", "" );
      }

      nCheckParms--;
      plParmList[ 0 ] = nCheckParms;

      DropView( vLookupView );
      return( 0 );
   }

   DropView( vLookupView );

   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
GetOperationModuleName( zVIEW  vSubtask,
                        zPCHAR szModuleName,
                        zLONG  lZKey,
                        zLONG  lOperationClass )
{
// zSHORT nCheckParms;
// zSHORT lParmType;
// zCHAR  cZeidonType[ 2 ];
// zCHAR  cPtrFlag[ 2 ];
   zVIEW  vLookupView;
   zVIEW  vSrcView;
   zVIEW  vTaskLPLR;
   zCHAR  szLookupEName[ 32 ];
// zLONG  lZKey;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );
   if ( vTaskLPLR == 0 )
   {
      // The LPLR is not active so we must be executing the operation at runtime from Core.  Just retrieve the DLL name from the XPG OI.
      // Note that the cursor for ExternalOperation should have been set earlier when we were retrieving the operation name.
      GetStringFromAttribute( szModuleName, sizeof( szModuleName ), g_vXPGView, "ExternalOperation", "MetaDLL_Name" );
      return( 0 );
   }

   strcpy_s( szLookupEName, sizeof( szLookupEName ), "Operation" );

   switch( lOperationClass )
   {
      case qLOCALOPERATION:
         GetStringFromAttribute( szModuleName, sizeof( szModuleName ), g_vXPGView, "VML_XSource", "Name" );
         return( 0 );

      case qOBJECTOPERATION:
         CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );
         strcpy_s( szLookupEName, sizeof( szLookupEName ), "ExternalOperation" );
         break;

      case qGLOBALOPERATION:
      {
         zVIEW vTempList;

         SetCursorFirstEntityByInteger( g_lpGOListView, "W_MetaDef", "CPLR_ZKey", lZKey, "" );

         // Save the current meta type cursor by creating a temp view and passing it to ActivateMeta.  ActivateMeta will change the meta
         // type to Global Op Group.
         CreateViewFromViewForTask( &vTempList, g_lpGOListView, 0 );

         ActivateMetaOI( vSubtask, &vSrcView, vTempList, zREFER_GO_META, zSINGLE | zLEVEL_APPLICATION );

         DropView( vTempList );

         // The Module Name is the DomainAndGlobalOpGroupName attribute, or if that attribute is null, it is the Name attribute.
         GetStringFromAttribute( szModuleName, sizeof( szModuleName ), vSrcView, "GlobalOperationGroup", "DomainAndGlobalOpGroupName" );
         if ( szModuleName[ 0 ] == 0 )
            GetStringFromAttribute( szModuleName, sizeof( szModuleName ), vSrcView, "GlobalOperationGroup", "Name" );
         DropView( vSrcView );

         return( 0 );
      }

      case qZEIDONOPERATION:
         oTZOPGRPO_GetViewForXGO( &vLookupView, zSYS_CURRENT_OI );
         SetCursorFirstEntityByInteger( vLookupView, szLookupEName, "ZKey", lZKey, "" );
         SetCursorFirstEntityByAttr( vLookupView, "SourceFileOfOperation", "ZKey", vLookupView, "SourceFile", "ZKey", "" );
         GetStringFromAttribute( szModuleName, sizeof( szModuleName ), vLookupView, "ExecComposite", "Name" );

         DropView( vLookupView );
         return( 0 );

      default:
         szModuleName[ 0 ] = 0;
         return( -1 );
   }

   SetCursorFirstEntityByInteger( vLookupView, szLookupEName, "ZKey", lZKey, "" );
   if ( lOperationClass == qOBJECTOPERATION )
   {
      GetStringFromAttribute( szModuleName, sizeof( szModuleName ), vLookupView, szLookupEName, "MetaDLL_Name" );
   }
   else
   {
      GetStringFromAttribute( szModuleName, sizeof( szModuleName ), vLookupView, szLookupEName, "DLL_Name" );
   }

   DropView( vLookupView );
   return( 0 );
}

#ifdef __WIN32__

zOPER_EXPORT zSHORT OPERATION
GetParmPushSize( zSHORT nType )
{
   switch( nType )
   {
      case qTDECIMAL:
         return( sizeof( zDECIMAL ) - sizeof( zLONG ) );
         break;

      case qTPOINTER:
      case qTINTEGER:
      case qTVIEW:
      case qTSTRING:
      case qTATTRIBUTENAME:
      case qTENTITYNAME:
      case qTCHARACTER:
      case qTSHORT:
         return( 1 );
         break;

     default:
         return( 0 );
         break;
   }
}

#else

zOPER_EXPORT zSHORT OPERATION
GetParmPushSize( zSHORT nType )
{
   switch( nType )
   {
      case qTDECIMAL:
         return( 4 );
         break;

      case qTPOINTER:
      case qTINTEGER:
      case qTVIEW:
      case qTSTRING:
      case qTATTRIBUTENAME:
      case qTENTITYNAME:
         return( 2 );
         break;

      case qTCHARACTER:
      case qTSHORT:
         return( 1 );
         break;

     default:
         return( 0 );
         break;
   }
}

#endif // !__WIN32__

zOPER_EXPORT zSHORT OPERATION
ConvertZeidonTypeToVMLTypeS( zCHAR cZeidonType )
{
   if ( cZeidonType == 'S' )
      return( qTSTRING );

   if ( cZeidonType == 'C' ||
        cZeidonType == 'Y' )
      return( qTCHARACTER );

   if ( cZeidonType == 'L' || cZeidonType == 'I' )
      return( qTINTEGER );

   if ( cZeidonType == 'H' || cZeidonType == 'N' )
      return( qTSHORT );

   if ( cZeidonType == 'M' || cZeidonType == 'F' || cZeidonType == 'R' || cZeidonType == 'W' )
      return( qTDECIMAL );

   if ( cZeidonType == 'V' )
      return( qTVIEW );

   return( qTSTRING );
}

zOPER_EXPORT zSHORT OPERATION
GetDefineInternalValue( zPCHAR szInternalValue,
                        zLONG  lDefineZKey )
{
   zLONG lRC;
   zVIEW vLookupView;
   zCHAR szLookupEName[ 32 ];

   strcpy_s( szLookupEName, sizeof( szLookupEName ), "Operation" );

   CreateViewFromViewForTask( &vLookupView, g_vXPGView, 0 );

   lRC = SetCursorFirstEntityByInteger( vLookupView, "DefinedItem", "ZKey", lDefineZKey, "VML_XSource" );
   if ( lRC >= zCURSOR_SET )
   {
      // The normal Define processing returns the InternalValue for the defined item.  However, special processing exists for the NEW_LINE
      // defined item to return the actual new line character '\n' instead of that defined in the object.
      // HH: replacing NEW_LINE by \r\n is not ok, this would not be the same as working compiled.
      // Interpreter and Compiler MUST NEVER work different.
      // Whenever a define contains a backslash combination, the interpreter has to translate it according to C language rules.
      zPCHAR pcOrig = NULL;
      GetAddrForAttribute( &pcOrig, vLookupView, "DefinedItem", "InternalValue" );
      if ( pcOrig )
         CopyBackslash( szInternalValue, pcOrig );

      DropView( vLookupView );
      return( 0 );
   }
   // try global list
   else
   {
      DropView( vLookupView );
      oTZOPGRPO_GetViewForXGO( &vLookupView, zSYS_CURRENT_OI );
      lRC = SetCursorFirstEntity( vLookupView, "OPERSIG", "" );
      if ( lRC == zCURSOR_SET )
      {
         lRC = SetCursorFirstEntityByInteger( vLookupView, "DefinedItem", "ZKey", lDefineZKey, "OPERSIG" );
         if ( lRC >= zCURSOR_SET )
         {
            zPCHAR pcOrig;
            GetAddrForAttribute( &pcOrig, vLookupView, "DefinedItem", "InternalValue" );
            if ( pcOrig )
               CopyBackslash( szInternalValue, pcOrig );

            DropView( vLookupView );
            return( 0 );
         }
      }
   }

   DropView( vLookupView );
   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
SetPointerValues( zPSHORT nStartStackPtr, zVIEW vInterpSubtask )
{
   zSHORT nRC;
   zVIEW  vStackObject;                           // view to the stack object

   // Get the stack object as it was loaded with the XPG
   GetViewByName( &vStackObject, "StackObject", vInterpSubtask, zLEVEL_SUBTASK );


   //if ( g_nStackPtr > *nStartStackPtr )
   //{
   //   g_nStackPtr -= (*nStartStackPtr) + 1;
   //   (*nStartStackPtr) = (*nStartStackPtr) + g_nStackPtr;
   //}
   // while ( *nStartStackPtr > 0 )

   g_nStackPtr = g_nCallStack[ g_nCallStack[ 0 ] ];
// g_nCallStack[ g_nCallStack[ 0 ] ] = 0; // ??

   while ( *nStartStackPtr > g_nStackPtr )
   {
      if ( sValueStack[ *nStartStackPtr ].lpVal != NULL && sValueStack[ *nStartStackPtr ].lDataType == qTPOINTER )
      {
         nRC = SetCursorFirstEntityByInteger( vStackObject, "Variable", "ID", sValueStack[ *nStartStackPtr ].lOffset, "" );
         if ( nRC == zCURSOR_SET )
         {
            if ( sValueStack[ *nStartStackPtr ].lpVal == &sValueStack[ *nStartStackPtr ].ldVal )
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", &sValueStack[ *nStartStackPtr ].ldVal, sizeof( zDECIMAL ) );
//###blob      SetAttributeFromDecimal( vStackObject, "Variable", "Value", sValueStack[ *nStartStackPtr ].ldVal );
            }
            else
            if ( sValueStack[ *nStartStackPtr ].lpVal == &sValueStack[ *nStartStackPtr ].lVal )
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", &sValueStack[ *nStartStackPtr ].lVal,  sizeof( zLONG ) );
//###blob      nRC = SetAttributeFromInteger( vStackObject, "Variable", "Value", (zLONG)sValueStack[ *nStartStackPtr ].lVal );
            }
            else
            {
               SetAttributeFromBlob( vStackObject, "Variable", "Value", sValueStack[ *nStartStackPtr ].lpVal,
                                     zstrlen( (zPCHAR)(sValueStack[ *nStartStackPtr ].lpVal) ) + 1 );
//###blob      SetAttributeFromString( vStackObject, "Variable", "Value", (zPCHAR) sValueStack[ *nStartStackPtr ].lpVal );
            }
         }
         else
         {
            TraceLineI( "(IM2A) stack variable not found id: ", sValueStack[ *nStartStackPtr ].lOffset );
            MessageSend( vInterpSubtask, "VM00203", "VML Interpretor", "Check TraceLine output", zMSGQ_OBJECT_CONSTRAINT_ERROR, zBEEP );
         }
      }

      (*nStartStackPtr)--;
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
SetPointerValuesFromLocalCall( zSHORT nStartStackPtr, zVIEW vInterpSubtask )
{
   zSHORT nRC;
   zVIEW  vStackObject;                           // view to the stack object
   zLONG  lZero;

   // Get the stack object as it was loaded with the XPG
   GetViewByName( &vStackObject, "StackObject", vInterpSubtask, zLEVEL_SUBTASK );


   if ( g_nStackPtr > nStartStackPtr )
   {
      //g_nStackPtr -= nStartStackPtr + 1;
      //nStartStackPtr = nStartStackPtr + g_nStackPtr;
   }

   lZero = 0;
   nRC = SetEntityCursor( vStackObject, "Variable", "ParameterFlag", zPOS_LAST | zQUAL_INTEGER | zQUAL_GREATER_THAN, &lZero, "", "", 0, "", "" );
   while ( nRC == zCURSOR_SET )
   {
      if ( sValueStack[ nStartStackPtr ].lpVal != NULL && sValueStack[ nStartStackPtr ].lDataType == qTPOINTER )
      {
         zULONG ulMaxLen;

         if ( sValueStack[ nStartStackPtr ].lpVal == (zDECIMAL *) &sValueStack[ nStartStackPtr ].ldVal )

         {
            // decimal
            ulMaxLen = sizeof( zDECIMAL );
            GetBlobFromAttribute( sValueStack[ nStartStackPtr ].lpVal, &ulMaxLen, vStackObject, "Variable", "Value" );
//###blob   nRC = GetDecimalFromAttribute( (zPDECIMAL)sValueStack[ nStartStackPtr ].lpVal, vStackObject, "Variable", "Value" );
         }
         else
         if ( sValueStack[ nStartStackPtr ].lpVal == &sValueStack[ nStartStackPtr ].lVal )
         {
            // integer
            ulMaxLen = sizeof( zLONG );
            GetBlobFromAttribute( sValueStack[ nStartStackPtr ].lpVal, &ulMaxLen, vStackObject, "Variable", "Value" );
//###blob   nRC = GetIntegerFromAttribute( (zPLONG)sValueStack[ nStartStackPtr ].lpVal, vStackObject, "Variable", "Value" );
         }
         else
         {
            //string

            GetAttributeLength( &ulMaxLen, vStackObject, "Variable", "Value" );

            if ( ulMaxLen )
               GetBlobFromAttribute( sValueStack[ nStartStackPtr ].lpVal, &ulMaxLen, vStackObject, "Variable", "Value" );
//          nRC = GetStringFromAttribute( (zPCHAR)sValueStack[ nStartStackPtr ].lpVal, vStackObject, "Variable", "Value" );
         }
      }

      nStartStackPtr--;
      SetEntityCursor( vStackObject, "Variable", "ParameterFlag", zPOS_PREV | zQUAL_INTEGER | zQUAL_GREATER_THAN, &lZero, "", "", 0, "", "" );
   }

   if ( sValueStack[ g_nStackPtr ].lDataType == qTDECIMAL )
   {
      sValueStack[ nStartStackPtr ].lDataType = qTDECIMAL;
      sValueStack[ nStartStackPtr ].ldVal = sValueStack[ g_nStackPtr ].ldVal;
   }
   else
   {
      sValueStack[ nStartStackPtr ].lDataType = qTINTEGER;
      sValueStack[ nStartStackPtr ].lVal = sValueStack[ g_nStackPtr ].lVal;
   }

   return( 0 );
}

//*** Change Start  -  Don Christensen  -  2/1/94
zOPER_EXPORT zSHORT OPERATION
IncrementStackPtr( )
{
   g_nStackPtr++;
   return( 0 );

}
//*** Change End

static void
CopyBackslash( zPCHAR pcTarget, zPCHAR pcSource )
{
   for ( ; *pcSource; ++pcTarget, ++pcSource )
   {
      if ( *pcSource == '\\' )
      {
         // handle backslash according to C language
         ++pcSource;
         switch ( *pcSource )
         {
         case 'a':
            *pcTarget = '\a'; // alert
            break;
         case 'b':
            *pcTarget = '\b'; // backspace
            break;
         case 'f':
            *pcTarget = '\f'; // formfeed
            break;
         case 'n':
            *pcTarget = '\n'; // newline
            break;
         case 'r':
            *pcTarget = '\r'; // carriage return
            break;
         case 't':
            *pcTarget = '\t'; // horizontal tab
            break;
         case 'v':
            *pcTarget = '\v'; // vertical tab
            break;
         case 0:
            *pcTarget = 0; // should not occur
            return;
         case 'x':
         case 'X':
            // evaluate hex combination
            *pcTarget =
              (zCHAR)( 16 *hextoi( pcSource[ 1 ] ) +hextoi( pcSource[ 2 ] ) );
            pcSource +=2;
            break;
         default:
            *pcTarget = *pcSource;
            break;
         }
      }
      else
         *pcTarget = *pcSource;
   }
   *pcTarget = 0;
}

static int hextoi( int i )
{
   if ( i >= (int) '0' && i <= (int) '9' )
   {
      return( i - (int) '0' );
   }
   else
   if ( i >= (int) 'a' && i <= (int) 'f' )
   {
      return( i - (int) 'a' + 10 );
   }
   else
   if ( i >= (int) 'A' && i <= (int) 'F' )
   {
      return( i - (int) 'A' + 10 );
   }
   else
      return( 255 ); // error in input, should not occur
}

// >>>fnWorkStorage
// Get area from workstorage manager

static zPCHAR fnWorkStorage( zPVOID hWRKS, zLONG lSize )
{
   zPVOID pData;

   if ( WRKS_Get( hWRKS, lSize, &pData ) < 0 )
   {
      // Error Longjump
      TraceLineS( "Interpreter CRASH *Out of Memory*", "" );
      longjmp( g_jbWRKS, -1 );
   }

   return(zPCHAR) pData;
}

#ifdef __cplusplus
}
#endif
