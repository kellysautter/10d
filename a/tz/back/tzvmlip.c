/////////////////////////////////////////////////////////////////////
//
// >>>TZVMLIP.C
//
// this file serves as a simple wrapper of standard
//  C functions to be called from VML interpreter.
// Other functions, which cannot be called from VML because of
//  parameter conventions, are supported throug wrapper finctions.
// It is compiled and linked  to TZMVMLIP.DLL .
//
/////////////////////////////////////////////////////////////////////
//
//
// CHANGE LOG
//
// 2002.08.09  BL
//    modified zSearchSubString, do not compare CaseSensitive
// 2002.05.08  ASE
//    re-added operations.
//    zTrim renamed to zRTrim due to conflicts with zGlobal1.c
// 2002.02.20  DGC
//    Removed previous operations.  They were conflicting with current operations
//    in zGlobal1.c.  I verified this with Frank H.
// 2002.01.18  ASE
//    added 15 new operations:
//    zReplicate, zTrim, zLTrim, zRight, zLeft, zMax, zMin, zAbs, zCeil,
//    zFloor, zSqrt, IsNumeric, zLike, zSoundex, zRemoveAllBlanks
// 2001.10.08  HH
//    added FindFile functions.
// 2001.04.21  HH
//    corrected (rewrote) zGetStringWithoutBlank.
// 2001.06.26  RG
//    added function GenFullQualPathFromRelPath to build relative pathnames
// 2001.04.21  HH
//    corrected (rewrote) zReplaceSubString.
// 2000.11.28  HH
//    const zCHAR change zGetStringLen, zGetStringLenWithoutBlank,
//    zGetStringWithoutBlank, zStartSystemCommand, zToUpper,
//    zGetDayOfYearFromDate, zStringIsDate, zDateIsLeapYear,
//    zGetDecrementDate, zGetIncrementDate, zGetGermanNameOfMonth,
//    zFindTextValueInAttribute, zGetFirstAttributeNameForEntity,
//    zGetNextAttributeNameForEntity, zLodContainsEntity,
//    zLodContainsAttribute, zSearchSubString, zReplaceSubString,
//    zStringToInteger .
// 2000.08.17  HH
//    fix crash in zGetFirstAttributeNameForEntity - R53106.
// 2000.08.04  BL+HH
//    Fixed stack overwrite on high asciis (unsigned required).
// 2000.07.27  RG  Z2000
//    - Added function VMLIP_zstrncmpi
//    - Added function zSearchSubString
//    - Added function VMLIP_zstrncpy
//    - Added function zReplaceSubString
//
// 2000.06.11  BL  All
//    Added zCharToOem for ADW EXPORT
//    Bugfix zToUpper
//
// 2000.02.16  US  ALL
//    Hand Merge of the only useful change of the latest version that
//    was thrown out because it overwrote datatype changes of prior
//    versions.
//    By the way, change logs are really useless if they are not there!
//    So, if anybody makes changes, please LOG them and don't overwrite
//    changes of colleagues!!
//
// 1999.08.06  BL  All  new operations
//
// 1999.04.22  US  Z10  New operation
//   new operation for searching and replacing in strings
// 1999.04.12  HH
//   added zSysTransLateString as wrapper for SysTransLateString.
//   added zSysReadLine as wrapper for SysReadLine.
//   added zSysReadLineLth as wrapper for SysReadLineLth.
// 1999.03.02  HH
//   new.


#define  MAXPATTERN 256   /*  max # characters in find string         */
#define  ASCIISIZE  256   /*  # characters in extended ASCII set      */
#define  LARGE      999999/*  Arbitrarily large constant for FAST/SLOW loop of Boyer-Moore. */

#include <string.h>
#include <math.h>

#define  KZSYSSVC_INCL
#define  KZMETA_INCL

#include "kzoengaa.h"
#include "ZDRVROPR.H"
#include "tzvmlip.h"

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 3 // remove "This function or variable may be unsafe." warnings
#endif

//#pragma message("tzvmlip ===> " VAR_NAME_VALUE(_CRT_SECURE_NO_WARNINGS))

#ifdef __cplusplus
extern "C" {
#endif


// prototypes for static functions
static void
qbldelta( zLONG  lPatternLth,       // pattern length
          zCPCHAR cpcPattern,       // pattern to locate
          zPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
          zPLONG lplDelta2Table );  // Boyer-Moore Delta2 Table
static zLONG
qfind( zLONG  lPatternLth,          // pattern length
       zCPCHAR cpcPattern,          // pattern to locate
       zLONG lBufferLth,            // length of buffer to be searched
       zPCHAR pchBuffer,            // buffer to be searched for pattern
       zPLONG lplDelta1Table,       // Boyer-Moore Delta1 Table
       zPLONG lplDelta2Table );     // Boyer-Moore Delta2 Table
static void
fnLoadDeltaOne( zLONG  lPatternLth,
                const zPUCHAR szPattern,
                zPLONG lplDelta1Table );
static void
fnLoadDeltaTwo( zLONG  lPatternLth,       // using Rytter's correction
                const zPUCHAR szPattern,  // to Knuth's scheme
                zPLONG lplDelta2Table );

zOPER_EXPORT int
VMLIP_zstrlen( zCPCHAR s1 )
{
   return( zstrlen( s1 ) );
}

zOPER_EXPORT int
VMLIP_zstrcmp( zCPCHAR s1,
               zCPCHAR s2 )
{
   return( zstrcmp( s1, s2 ) );
}

zOPER_EXPORT int
VMLIP_zstrcmpi( zCPCHAR s1,
                zCPCHAR s2 )
{
   return( zstrcmpi( s1, s2 ) );
}

zOPER_EXPORT int
VMLIP_zstrncmpi( zCPCHAR s1,
                 zCPCHAR s2, short nLth )
{
   return( zstrncmpi( s1, s2, nLth ) );
}

zOPER_EXPORT void
VMLIP_zstrncpy( zPCHAR  s1,
                zCPCHAR s2,
                short   nLth )
{
   strncpy_s( s1, nLth, s2, nLth - 1 );
   return;
}

zOPER_EXPORT void
VMLIP_zltoa( long l,
             zPCHAR pchReturn )
{
   zltoa( l, pchReturn, 16 );
   return;
}

//./ ADD NAME=WL_QC
// Source Module=tzvmlip.c
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: WL_QC
//
//  PURPOSE:    This routine Converts an instance of a special character in
//              a buffer and then writes out the buffer. The character to
//              be translated is szTransChar and any instance of it is
//              converted to a double quote.
//
//  PARAMETERS: lFile - File handle
//              pchBuffer - the string to be converted.
//              pchTransChar - The character to be converted to a quote.
//              nAddBlankLineCnt - Number of blank lines to append.
//
//  RETURNS:    0  - File delete successfully.
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zSHORT OPERATION
WL_QC( zVIEW  vDialog,
       zLONG  lFile,
       zPCHAR pchBuffer,
       zPCHAR pchTransChar,
       zSHORT nBlankLineCnt )
{
   zPCHAR pch;

   for ( pch = pchBuffer; *pch; pch++ )
   {
      if ( *pch == *pchTransChar )
         *pch = '"';
   }

   SysWriteLine( vDialog, lFile, pchBuffer );
   while ( nBlankLineCnt-- > 0 )
      SysWriteLine( vDialog, lFile, "" );

   return( 0 );
}

zOPER_EXPORT void OPERATION
zSysTranslateString( zPCHAR  pchString,
                     zCPCHAR cpcUpperLower )
{
   SysTranslateString( pchString, cpcUpperLower[ 0 ] );
   return;
}

zOPER_EXPORT int OPERATION
zSysReadLine( zVIEW  lpTaskView,
              zPCHAR pchBuffer,
              zLONG  lFileHandle,
              zULONG ulMaxLth )
{
   zPCHAR pchReturnLine;
   int nRC;
   size_t nLth;

   nRC = SysReadLine( lpTaskView, (zCOREMEM) &pchReturnLine, lFileHandle );
   if ( nRC == 1 )
   {
      if ( (nLth = zstrlen( pchReturnLine ) ) >= ulMaxLth )
         return( zCALL_ERROR );

      // szBuffer is long enough
      memcpy( pchBuffer, pchReturnLine, nLth + 1 );
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
zSysReadLineLth( zVIEW  lpTaskView,
                 zPCHAR pchBuffer,
                 zLONG  lFileHandle,
                 zULONG ulLth )
{
   zPCHAR *pchReturnLine;
   int nRC;
   nRC = SysReadLineLth( lpTaskView, (zCOREMEM) &pchReturnLine,
                          lFileHandle, ulLth );
   if ( nRC == 1 )
   {
      // szBuffer is long enough, hopefully
      memcpy( pchBuffer, pchReturnLine, ulLth );
   }

   return( nRC );
}

// Function: zSearchAndReplace
// The function searches all occurrences of cpcSearch in pchTarget and
// replaces them with cpcReplace.
// The search and the replace string can be of different size.
// If the result in pchTarget will be longer than ulTargetLth - 1 it is truncated.
// The resulting string in pchTarget has always the maximum length of
// ulTargetLen - 1 and is null terminated.
// The function returns the count of replaces that were done.
zOPER_EXPORT zSHORT OPERATION
zSearchAndReplace( zPCHAR  pchTarget,
                   zULONG  ulTargetLth,
                   zCPCHAR cpcSearch,
                   zCPCHAR cpcReplace )
{
   zPCHAR  pchPos = pchTarget;
   zULONG  ulOffset, ulOrigLth, ulSearchLth, ulReplaceLth, ulDiffLth;
   zSHORT  nReplaceCnt = 0;

   ulOrigLth = strlen( pchTarget );
   ulSearchLth  = strlen( cpcSearch );
   ulReplaceLth = strlen( cpcReplace );
   ulDiffLth = ulReplaceLth - ulSearchLth;

   do
   {
      pchPos = strstr( pchPos, cpcSearch );
      if ( pchPos == 0 )   // Search string not found
      {
         break;
      }

      ulOffset = (zULONG)(pchPos - pchTarget);

      // For each replace, the length may change.
      if ( ulOrigLth + ulDiffLth < ulTargetLth )
      {
         // Normally the Target is big enough for the replace.
         memmove( pchPos + ulReplaceLth, pchPos + ulSearchLth,
                  ulOrigLth - ulOffset - ulSearchLth + 1 );
                 // + 1 because of terminating 0
         memcpy( pchPos, cpcReplace, ulReplaceLth );
         ulOrigLth += ulDiffLth;
      }
      else
      {
         // Target is not big enough, truncate when necessary.
         if ( ulOffset + ulReplaceLth < ulTargetLth )
         {
            // The Replace String fits in the Target.
            memmove( pchPos + ulReplaceLth, pchPos + ulSearchLth,
                     ulTargetLth - ulOffset - ulReplaceLth - 1 );
            memcpy( pchPos, cpcReplace, ulReplaceLth );
         }
         else
         {
            // The Replace String does not completely fit in the Target.
            memcpy( pchPos, cpcReplace, ulTargetLth - ulOffset - 1 );
         }

         ulOrigLth = ulTargetLth - 1;
         pchTarget[ ulTargetLth ] = 0;
      }

      nReplaceCnt++;
      pchPos += ulReplaceLth; // continue searches from the end of the last Replace

      if ( pchPos >= (pchTarget + ulTargetLth - 1) )
         break;

   } while( TRUE );

   return( nReplaceCnt );
}

// Function: zSearchAndReplaceWord
// The function searches all occurrences of cpcSearch in pchTarget and
// replaces them with cpcReplace
// The search and the replace string can be of different size.
// If the result in pchTarget will be longer than ulTargetLth - 1 it is truncated.
// The resulting string in pchTarget has always the maximum length of
// ulTargetLen - 1 and is null terminated.
// The function returns the count of replaces that were done.
zOPER_EXPORT zSHORT OPERATION
zSearchAndReplaceWord( zPCHAR  pchTarget,
                       zULONG  ulTargetLth,
                       zCPCHAR cpcSearch,
                       zCPCHAR cpcReplace,
                       zCPCHAR cpcDelimiters )
{
   zPCHAR  pchTargetCopy;
   zPCHAR  pchPos;
   zULONG  ulLth, ulOffset, ulOrigLth, ulSearchLth, ulReplaceLth, ulDiffLth, ulRemainder;
   zSHORT  nReplaceCnt = 0;
   zCHAR   szDelimiters[ 256 ];
   zPCHAR  pchContext = NULL;

   strcpy_s( szDelimiters, zsizeof( szDelimiters ), cpcDelimiters );
   strcat_s( szDelimiters, zsizeof( szDelimiters ), " \t\r\n" ); // always look for space/tab/CRLF

   ulOrigLth = strlen( pchTarget );
   ulSearchLth  = strlen( cpcSearch );
   ulReplaceLth = strlen( cpcReplace );
   ulDiffLth = ulReplaceLth - ulSearchLth;

   if ( ulTargetLth < ulOrigLth )
      ulTargetLth = ulOrigLth;

   // strtok modifies the string being searched ... so we copy the original
   // to a modifiable buffer!
   pchTargetCopy = (zPCHAR) SysMalloc( ulTargetLth + 1 );
   strcpy_s( pchTargetCopy, ulTargetLth + 1, pchTarget );
   pchPos = pchTargetCopy;

   do
   {
      pchPos = strtok_s( pchPos, szDelimiters, &pchContext );
      if ( pchPos == 0 )   // Search string not found
      {
         break;
      }

      ulLth = strlen( pchTargetCopy );
      if ( strcmp( cpcSearch, pchPos ) == 0 )
      {
         // Found a match!
         ulRemainder = pchPos - pchTargetCopy;
         ulOffset = ulRemainder + ulSearchLth;
         if ( strlen( pchTarget + ulOffset ) + ulLth + ulReplaceLth < ulTargetLth )
         {
            nReplaceCnt++;
            strcpy_s( pchPos, ulRemainder, cpcReplace );
            strcpy_s( pchPos + ulReplaceLth, ulRemainder - ulReplaceLth, pchTarget + ulOffset );
            strcpy_s( pchTarget, ulTargetLth, pchTargetCopy );
            ulOrigLth = strlen( pchTarget );
         }
         else
         {
            strcpy_s( pchTargetCopy, ulTargetLth, pchTarget );
            pchPos = pchTargetCopy + ulOrigLth;
         }
      }
      else
      {
         // Just reset the null terminator.
         pchTargetCopy[ ulLth ] = pchTarget[ ulLth ];
         pchPos = pchTargetCopy + ulLth + 1;
      }

   } while( TRUE );

   SysFree( pchTargetCopy );
   return( nReplaceCnt );
}

// Function: zGetStringLen
// liefert die Länge eines Strings
zOPER_EXPORT zLONG OPERATION
zGetStringLen( zCPCHAR cpcSourceString )
{
   return( strlen( cpcSourceString ));
} // zGetStringLen

// Function: zStartSystemCommand
// startet einen DOS-internen Befehl wie "Type xxx.txt > LPT1"; ist mit
// Zeidon-Operation "StartTask" nicht möglich
zOPER_EXPORT zSHORT OPERATION
zStartSystemCommand( zVIEW    vViewId,
                     zCPCHAR  cpcCommand )
{
   zSHORT lRc;

   lRc = system( cpcCommand );

   return lRc;
} // zStartSystemCommand

// Function: zReadLine
// reads a line of flat files and converts OEM to CHAR ; works with
// Zeidon-Operation "SysReadLine"
zOPER_EXPORT zLONG OPERATION
zReadLine( zVIEW    lpTaskView,
           zPCHAR   pchBuffer,
           zLONG    ulMaxStringLth,
           zLONG    ulFileHandle )
{
   zLONG l;
   zPCHAR   pchData = NULL;

   l = SysReadLine( lpTaskView, &pchData, ulFileHandle );
   if ( l < 1 || pchData == NULL )
      return( l ); // Sorry, there was an error

   if ( (zLONG) strlen( pchData ) >= ulMaxStringLth )
      return( -1 );    // sorry, target string short

   strcpy_s( pchBuffer, ulMaxStringLth, pchData );
// OemToChar( pchBuffer, pchBuffer );
   return( l );

} // zReadLineFlatFiles

// Function: zDialogIsActive
// prüft, ob ein Dialog aktiv ist
zOPER_EXPORT zSHORT OPERATION
zDialogIsActive( zPCHAR pchDialogName )
{
   zCHAR    szMyDialogName[ 65 ];
   zVIEW    vTempSubtask;
   zCHAR    szMsg[ 129 ];
   zSHORT   nRC;

   ZeidonStringCopy( szMyDialogName, 1, 0, "__", 1, 0, 65 );
   ZeidonStringConcat( szMyDialogName, 1, 0, pchDialogName, 1, 0, 65 );

   if ( GetViewByName( &vTempSubtask, szMyDialogName, 0, zLEVEL_TASK ) < zCURSOR_SET )
   {
      nRC = -1;
      ZeidonStringCopy( szMsg, 1, 0, "Dialog ", 1, 0, 129 );
      ZeidonStringConcat( szMsg, 1, 0, pchDialogName, 1, 0, 129 );
      ZeidonStringConcat( szMsg, 1, 0, " wurde in diesem Task noch nie gestartet.", 1, 0, 129 );
   }
   else
   {
      nRC = SetCursorFirstEntity( vTempSubtask, "Subtask", "Dlg" );
      if ( nRC >= zCURSOR_SET )
      {
         nRC = 1;
         ZeidonStringCopy( szMsg, 1, 0, "Dialog ", 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, pchDialogName, 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, " ist noch aktiv.", 1, 0, 129 );
      }
      else
      {
         nRC = 0;
         ZeidonStringCopy( szMsg, 1, 0, "Dialog ", 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, pchDialogName, 1, 0, 129 );
         ZeidonStringConcat( szMsg, 1, 0, " ist nicht mehr aktiv.", 1, 0, 129 );
         DropView( vTempSubtask );
      }
   }

   TraceLineS( "(DialogIsActive)", szMsg );

   return( nRC );
} // zDialogIsActive

// Function: zToUpper
// konvertiert einen String in Großbuchstaben
zOPER_EXPORT zSHORT OPERATION
zToUpper( zCPCHAR cpcSourceString,
          zPCHAR  pchTargetString )
{
	zLONG  lLth;

	strcpy_s(pchTargetString, strlen(cpcSourceString) + 1, cpcSourceString);
	pchTargetString = AnsiUpper( pchTargetString );

   return( 0 );
} // zToUpper

#define WHITESPACE " \t\r\n"

zOPER_EXPORT zLONG OPERATION
zstrtrim( zPCHAR pchName )
{
   zPCHAR pch;
   zLONG  lLth;

   // Eliminate leading and trailing blanks.
   lLth = zstrlen(pchName);
   pch = pchName + strspn( pchName, WHITESPACE );
   if ( pch > pchName )
      strcpy_s( pchName, lLth, pch );

   lLth = zstrlen( pchName );
   while ( lLth &&
           (pchName[ lLth - 1 ] == ' ' || pchName[ lLth - 1 ] == '\t' ||
            pchName[ lLth - 1 ] == '\r' || pchName[ lLth - 1 ] == '\n') )
   {
      lLth--;
      pchName[ lLth ] = 0;
   }

   return( lLth );
}

// Function: zGetDayOfYearFromDate
// liefert zu einem Datum den Tag eines Jahres (Bsp.: 02.01. -> 2 oder
// 04.02. ->35)
// bei ungültigem Datum wird -1 zurückgeliefert
zOPER_EXPORT zLONG OPERATION
zGetDayOfYearFromDate( zCPCHAR cpcDateOrDateTime )
{
   zCHAR     szYearPart[ 5 ];
   zCHAR     szYearPart1[ 5 ];
   zCHAR     szMonthPart[ 3 ];
   zCHAR     szMonthPart1[ 3 ];
   zCHAR     szMonthHelp[ 3 ];
   zCHAR     szDayPart[ 3 ];
   zCHAR     szDayPart1[ 3 ];
   zCHAR     szDayHelp[ 3 ];
   zLONG     lMonthPart;
   zLONG     lDayPart;
   zLONG     lMonthCnt;
   zLONG     lDayCnt;
   zLONG     lDayHelp;
   zLONG     lYearPart;
   zSHORT    bLeapYear;

   ZeidonStringCopy( szYearPart, 1, 0, cpcDateOrDateTime, 1, 4, 5 );
   ZeidonStringCopy( szMonthPart, 1, 0, cpcDateOrDateTime, 5, 2, 3 );
   ZeidonStringCopy( szDayPart, 1, 0, cpcDateOrDateTime, 7, 2, 3 );

   lYearPart = zStringToInteger( szYearPart );

   //Test, ob ungueltiges Jahr
   zIntegerToString( szYearPart1, 4, lYearPart );
   if ( ZeidonStringCompare( szYearPart, 1, 0, szYearPart1, 1, 0, 5 ) != 0 )
      return( -1 );

   lMonthPart = zStringToInteger( szMonthPart );
   //Test, ob ungueltiger Monat
   zIntegerToString( szMonthHelp, 2, lMonthPart );
   //wenn Monat kleiner 10, dann führende Null ergaenzen
   if ( lMonthPart < 10 )
   {
      ZeidonStringCopy( szMonthPart1, 1, 0, "0", 1, 0, 5 );
      ZeidonStringConcat( szMonthPart1, 1, 0, szMonthHelp, 1, 0, 5 );
   }
   else
      ZeidonStringCopy( szMonthPart1, 1, 0, szMonthHelp, 1, 0, 3 );

   if ( ZeidonStringCompare( szMonthPart, 1, 0, szMonthPart1, 1, 0, 3 ) != 0 )
      return( -1 );
   else
   if ( lMonthPart < 1 || lMonthPart > 12 )
      return( -1 );

   lDayPart = zStringToInteger( szDayPart );
   //Test, ob ungueltiger Tag
   zIntegerToString( szDayHelp, 2, lDayPart );
   //wenn Tag groesser 0 und kleiner 10, dann führende Null ergaenzen
   if ( lDayPart > 0 && lDayPart < 10 )
   {
      ZeidonStringCopy( szDayPart1, 1, 0, "0", 1, 0, 5 );
      ZeidonStringConcat( szDayPart1, 1, 0, szDayHelp, 1, 0, 5 );
   }
   else
      ZeidonStringCopy( szDayPart1, 1, 0, szDayHelp, 1, 0, 3 );

   if ( ZeidonStringCompare( szDayPart, 1, 0, szDayPart1, 1, 0, 3 ) != 0 )
      return( -1 );

   bLeapYear = zYearIsLeapYear( (zSHORT) lYearPart );
   lDayCnt = 0;
   lMonthCnt = 1;

   while ( lMonthCnt < lMonthPart )
   {
      if ( lMonthCnt == 1 || lMonthCnt == 3 || lMonthCnt == 5 || lMonthCnt == 7 || lMonthCnt == 8 || lMonthCnt == 10 || lMonthCnt == 12 )
      {
         lDayCnt = lDayCnt + 31;
      }
      else
      {
         if ( lMonthCnt == 4 || lMonthCnt == 6 || lMonthCnt == 9 || lMonthCnt == 11 )
            lDayCnt = lDayCnt + 30;
         else
            if ( bLeapYear == TRUE )
               lDayCnt = lDayCnt + 29;
            else
               lDayCnt = lDayCnt + 28;
      }

      lMonthCnt = lMonthCnt + 1;
   } //endwhile

  // Prüfung, ob ein ungültiges Datum übergeben wurde
  if ( lMonthCnt == 1 || lMonthCnt == 3 || lMonthCnt == 5 || lMonthCnt == 7 || lMonthCnt == 8 || lMonthCnt == 10 || lMonthCnt == 12 )
     lDayHelp = 31;
   else
   {
      if ( lMonthCnt == 4 || lMonthCnt == 6 || lMonthCnt == 9 || lMonthCnt == 11 )
         lDayHelp = 30;
      else
         if ( bLeapYear == TRUE )
            lDayHelp = 29;
         else
            lDayHelp = 28;
      }

   //Test, ob ungueltiger Tag
   if ( lDayPart > lDayHelp || lDayPart < 1 )
      lDayCnt = -1;
   else
      lDayCnt = lDayCnt + lDayPart;

   return( lDayCnt );

} // zGetDayOfYearFromDate

// Function: zGetDateFromDayOfYear
// liefert zu einem Tag im Jahr das Datum (Bsp.: 35 -> 04.02.)
// liefert -1 bei ungültigen Input-Parametern
zOPER_EXPORT zLONG OPERATION
zGetDateFromDayOfYear(  zPCHAR   pchReturnDate,
                        zUSHORT  uYear,
                        zUSHORT  uDayOfYear )
{
   zCHAR     szYearPart[ 5 ];
   zCHAR     szYearHelp[ 5 ];
   zCHAR     szMonthPart[ 3 ];
   zCHAR     szDayPart[ 3 ];
   zCHAR     szHelp[ 3 ];
   zLONG     bLeapYear;
   zLONG     lYearLth;
   zLONG     RemainingDays;
   zLONG     bOk;
   zLONG     lMonAt;
   zLONG     lHelp;
   zSHORT    nRC;

   nRC = zIntegerToString( szYearPart, 4, uYear );
   lYearLth = zGetStringLen( szYearPart );
   while ( lYearLth < 4 )
   {
      ZeidonStringCopy( szYearHelp, 1, 0, "0", 1, 0, 5 );
      ZeidonStringConcat( szYearHelp, 1, 0, szYearPart, 1, 0, 5 );
      ZeidonStringCopy( szYearPart, 1, 0, szYearHelp, 1, 0, 5 );
      lYearLth = zGetStringLen( szYearPart );
   }

   bLeapYear = zYearIsLeapYear( uYear );

   if ( bLeapYear == TRUE )
   {
      if ( uDayOfYear > 366 )
         return( -1 );
   }
   else
   {
      if ( uDayOfYear > 365 )
         return( -1 );
   }

   bOk = 1;
   lMonAt = 1;
   RemainingDays = uDayOfYear;
   while ( bOk == 1 )
   {
      if ( lMonAt == 1 || lMonAt == 3 || lMonAt == 5 || lMonAt == 7 ||
           lMonAt == 8 || lMonAt == 10 || lMonAt == 12 )
      {
         lHelp = RemainingDays;
         RemainingDays = RemainingDays - 31;
      }
      else
         if ( lMonAt == 4 || lMonAt == 6 || lMonAt == 9 || lMonAt == 11 )
         {
            lHelp = RemainingDays;
            RemainingDays = RemainingDays - 30;
         }
         else
            if ( lMonAt == 2 )
               if ( bLeapYear == TRUE )
               {
                  lHelp = RemainingDays;
                  RemainingDays = RemainingDays - 29;
               }
               else
               {
                  lHelp = RemainingDays;
                  RemainingDays = RemainingDays - 28;
               }

      if ( RemainingDays > 0 && lMonAt <= 12 )
         lMonAt = lMonAt + 1;
      else
         bOk = 0;
   }

   nRC = zIntegerToString( szHelp, 2, lMonAt );
   if ( lMonAt < 10 )
   {
      ZeidonStringCopy( szMonthPart, 1, 0, "0", 1, 0, 3 );
      ZeidonStringConcat( szMonthPart, 1, 0, szHelp, 1, 0, 3 );
   }
   else
      ZeidonStringCopy( szMonthPart, 1, 0, szHelp, 1, 0, 3 );

   nRC = zIntegerToString( szHelp, 2, lHelp );
   if ( lHelp < 10 )
   {
      ZeidonStringCopy( szDayPart, 1, 0, "0", 1, 0, 3 );
      ZeidonStringConcat( szDayPart, 1, 0, szHelp, 1, 0, 3 );
   }
   else
      ZeidonStringCopy( szDayPart, 1, 0, szHelp, 1, 0, 3 );

   ZeidonStringCopy( pchReturnDate, 1, 0, szYearPart, 1, 0, 33 );
   ZeidonStringConcat( pchReturnDate, 1, 0, szMonthPart, 1, 0, 33 );
   ZeidonStringConcat( pchReturnDate, 1, 0, szDayPart, 1, 0, 33 );
   ZeidonStringConcat( pchReturnDate, 1, 0, "000000000", 1, 0, 33 );
   return( 0 );
} // zGetDateFromDayOfYear

// Function: zStringIsDate
// prueft, ob ein String ein gueltiges Datum ist
zOPER_EXPORT zSHORT OPERATION
zStringIsDate( zCPCHAR cpcSourceString )
{
   zLONG lRC;

   lRC = zGetDayOfYearFromDate( cpcSourceString );

   //zGetDayOfYearFromDate liefert -1 bei ungültigen Datum, sonst > 0
   if ( lRC > 0 )
      return( 1 );
   else
      return( 0 );

} // zStringIsDate

//  Function: zYearIsLeapYear
// prüft, ob ein Jahr ein Schaltjahr ist
zOPER_EXPORT zSHORT OPERATION
zYearIsLeapYear( zSHORT nYear )
{
   zSHORT    bLeapYear;
   zLONG     i;
   zLONG     j;
   zLONG     k;

   //Test, ob ungueltiges Jahr, da Parser dies nciht bemerkt
   if ( nYear < 1 )
      return( -1 );

   i = zModulo( nYear, 4 );
   j = zModulo( nYear, 100 );
   k = zModulo( nYear, 400 );

   if ( k == 0 && j == 0 )
      bLeapYear = TRUE;
   else
   {
      if ( i == 0 && j != 0 )
         bLeapYear = TRUE;
      else
         bLeapYear = FALSE;
   }

   return( bLeapYear );
} // zYearIsLeapYear

//  Function:zDateIsLeapYear
// prüft, ob ein Datum ein Schaltjahr ist
zOPER_EXPORT zSHORT OPERATION
zDateIsLeapYear( zCPCHAR cpcDateOrDateTime )
{
   zCHAR     szYearPart[ 5 ];
   zLONG     lYearPart;
   zSHORT    bLeapYear;
   zSHORT    lRc;
   zLONG     i;
   zLONG     j;
   zLONG     k;

   //Test, ob gueltiges Datum uebergeben wurde
   lRc = zStringIsDate( cpcDateOrDateTime );
   if ( lRc != 1 )
      return( -1 );

   ZeidonStringCopy( szYearPart, 1, 0, cpcDateOrDateTime, 1, 4, 5 );
   lYearPart = zStringToInteger( szYearPart );

   i = zModulo( lYearPart, 4 );
   j = zModulo( lYearPart, 100 );
   k = zModulo( lYearPart, 400 );

   if ( k == 0 && j == 0 )
      bLeapYear = TRUE;
   else
   {
      if ( i == 0 && j != 0 )
         bLeapYear = TRUE;
      else
         bLeapYear = FALSE;
   }

   return( bLeapYear );
} // zDateIsLeapYear

// Function: zGetDecrementDate
// liefert das Datum des vorherigen Tages
zOPER_EXPORT zSHORT OPERATION
zGetDecrementDate( zCPCHAR cpcSourceDate,
                   zPCHAR  pchTargetDate,
                   zLONG   lMaxLth )
{
   zCHAR    szYearPart[ 5 ];
   zCHAR    szMonthPart[ 3 ];
   zCHAR    szDayPart[ 3 ];
   zLONG    k;
   zLONG    lRC;
   zLONG    lYearPart;
   zSHORT   bLeapYear;

   ZeidonStringCopy( szYearPart, 1, 0, cpcSourceDate, 1, 4, 5 );
   ZeidonStringCopy( szMonthPart, 1, 0, cpcSourceDate, 5, 2, 3 );
   ZeidonStringCopy( szDayPart, 1, 0, cpcSourceDate, 7, 2, 3 );
   lYearPart = zStringToInteger( szYearPart );

   k = zGetDayOfYearFromDate( cpcSourceDate );

   //Test, ob zGetDayOfYearFromDate einen Fehler brachte
   if ( k < 0 )
      return( (zSHORT) k );

   if ( k == 1 )
   {
      lYearPart = lYearPart - 1;
      ZeidonStringCopy( szMonthPart, 1, 0, "12", 1, 0, 3 );
      ZeidonStringCopy( szDayPart, 1, 0, "31", 1, 0, 3 );
      lRC = zIntegerToString( szYearPart, 4, lYearPart );
      ZeidonStringCopy( pchTargetDate, 1, 0, szYearPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szMonthPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szDayPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, "000000000", 1, 0, lMaxLth );
      bLeapYear = zYearIsLeapYear( (zSHORT) lYearPart );

      if ( bLeapYear == TRUE )
         k = 367;
      else
         k = 366;
   }

   k = k - 1;

   lRC = zGetDateFromDayOfYear( pchTargetDate, (zUSHORT) lYearPart, (zUSHORT) k );

   return( (zSHORT) lRC );

} // zGetDecrementDate

// Function: zGetIncrementDate
// liefert das Datum des folgenden Tages
zOPER_EXPORT zSHORT OPERATION
zGetIncrementDate( zCPCHAR cpcSourceDate,
                   zPCHAR  pchTargetDate,
                   zLONG   lMaxLth )
{
   zCHAR    szYearPart[ 5 ];
   zCHAR    szMonthPart[ 3 ];
   zCHAR    szDayPart[ 3 ];
   zLONG    k;
   zLONG    lYearPart;
   zLONG    lRC;
   zSHORT   bLeapYear;

   ZeidonStringCopy( szYearPart, 1, 0, cpcSourceDate, 1, 4, 5 );
   ZeidonStringCopy( szMonthPart, 1, 0, cpcSourceDate, 5, 2, 3 );
   ZeidonStringCopy( szDayPart, 1, 0, cpcSourceDate, 7, 2, 3 );
   lYearPart = zStringToInteger( szYearPart );
   bLeapYear = zYearIsLeapYear( (zSHORT) lYearPart );

   k = zGetDayOfYearFromDate( cpcSourceDate );

   //Test, ob zGetDayOfYearFromDate einen Fehler brachte
   if ( k < 0 )
      return( (zSHORT) k );

   if ( bLeapYear == TRUE && k == 366 )
   {
      k = 0;
      lYearPart = lYearPart + 1;
      ZeidonStringCopy( szMonthPart, 1, 0, "01", 1, 0, 3 );
      ZeidonStringCopy( szDayPart, 1, 0, "01", 1, 0, 3 );
      lRC = zIntegerToString( szYearPart, 4, lYearPart );
      ZeidonStringCopy( pchTargetDate, 1, 0, szYearPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szMonthPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szDayPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, "000000000", 1, 0, lMaxLth );
   }
   else
   if ( bLeapYear == FALSE && k == 365 )
   {
      lYearPart = lYearPart + 1;
      k = 0;
      lRC = zIntegerToString( szYearPart, 2, lYearPart );
      ZeidonStringCopy( szMonthPart, 1, 0, "01", 1, 0, 3 );
      ZeidonStringCopy( szDayPart, 1, 0, "01", 1, 0, 3 );
      ZeidonStringCopy( pchTargetDate, 1, 0, szYearPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szMonthPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, szDayPart, 1, 0, lMaxLth );
      ZeidonStringConcat( pchTargetDate, 1, 0, "000000000", 1, 0, lMaxLth );
   }

   k = k + 1;
   lRC = zGetDateFromDayOfYear( pchTargetDate, (zUSHORT) lYearPart, (zUSHORT) k );

   return( (zSHORT) lRC );

} // zGetIncrementDate

// Function: zGetGermanNameOfMonth
// liefert für ein Datum den deutschen lMonAtsnamen
zOPER_EXPORT zSHORT OPERATION
zGetGermanNameOfMonth( zCPCHAR  cpcSourceDate,
                       zPCHAR   pchMonthName,
                       zLONG    lMaxLth )
{
   zSHORT    bOk;
   zCHAR     szMonthPart[ 3 ];

   //Test, ob gueltiges Datum uebergeben wurde
   bOk = zStringIsDate( cpcSourceDate );
   if ( bOk != 1 )
      return( -1 );

   ZeidonStringCopy( szMonthPart, 1, 0, cpcSourceDate, 5, 2, 3 );
   bOk = 0;

   if ( ZeidonStringCompare( szMonthPart, 1, 0, "01", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Januar", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "02", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Februar", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "03", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "März", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "04", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "April", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "05", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Mai", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "06", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Juni", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "07", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Juli", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "08", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "August", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "09", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "September", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "10", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Oktober", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "11", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "November", 1, 0, lMaxLth );
   else
   if ( ZeidonStringCompare( szMonthPart, 1, 0, "12", 1, 0, 3 ) == 0 )
      ZeidonStringCopy( pchMonthName, 1, 0, "Dezember", 1, 0, lMaxLth );
   else
      bOk = -1;

   return( bOk );

} // zGetGermanNameOfMonth

// Function: zIntegerToString
// konvertiert einen Integer in einen String
zOPER_EXPORT zSHORT OPERATION
zIntegerToString( zPCHAR   pchTargetString,
                  zLONG    ulTargetStringLth,
                  zLONG    ulValue )
{
   zLONG  uI = abs( ulValue );
   zLONG  ulExpectedLength = 1;
   zSHORT nRC;

   *pchTargetString = 0;

   // Ermittlung der Laenge des Integer-Wertes
   while ( uI > 10 )
   {
      uI = uI / 10;
      ulExpectedLength++;
   }

   // bei negativen Integer-Werten das Vorzeichen zur Laenge dazurechnen
   if ( ulValue < 0 )
      ulExpectedLength++;

   if ( ulExpectedLength > ulTargetStringLth )
   {
      nRC = -1;
   }
   else
   {
      zltoa( ulValue, pchTargetString, ulTargetStringLth );
      nRC = 0;
   }

   return( nRC );
} // zIntegerToString

// Function: zModulo
// liefert den ganzzahligen Rest einer Division
zOPER_EXPORT zLONG OPERATION
zModulo( zLONG ulDivident,
         zLONG ulDivisor )
{
   zLONG lModulo = -1;

   if ( ulDivisor )
      lModulo = ulDivident % ulDivisor;

   return( lModulo );

} // zModulo

// Function: zGetStringWithoutBlank
// Remove leading and trailing blanks.
zOPER_EXPORT zLONG OPERATION
zGetStringWithoutBlank( zCPCHAR  cpcSourceString,
                        zPCHAR   pchTargetString )
{
   zLONG i, j;

   i = 0;
   while ( cpcSourceString[ i ] == ' ' )
      i++; // skip over leading blanks

   if ( cpcSourceString[ i ] == 0 )
   {
      pchTargetString[ 0 ] = 0; // target is empty
      return( 0 );
   }

   j = zstrlen( cpcSourceString ) - 1;
   while ( cpcSourceString[ j ] == ' ' )
      j--; // skip over trailing blanks

   j -= (i - 1);
   memcpy( pchTargetString, cpcSourceString + i, j );
   pchTargetString[ j ] = 0;
   return( j );

} // zGetStringWithoutBlank

// Function: zGetStringLenWithoutBlank
// liefert die Länge eines Strings ohne vorangestellte und ohne nachfolgenden Blanks
zOPER_EXPORT zLONG OPERATION
zGetStringLenWithoutBlank( zCPCHAR cpcSourceString )
{
   zLONG i, j;

   i = 0;
   while ( cpcSourceString[ i ] == ' ' )
      i++; // skip over leading blanks

   if ( cpcSourceString[ i ] == 0 )
      return( 0 ); // target is empty

   j = zstrlen( cpcSourceString ) - 1;
   while ( cpcSourceString[ j ] == ' ' )
      j--; // skip over trailing blanks

   j -= (i - 1);
   return( j );
} // zGetStringLenWithoutBlank

// Function: zViewIsUpdateable
// prüft, ob eine View veränderbar ist
// RETURNS: 0                 View IS NOT Updateable
//          1                 View IS Updateable
//       zCALL_ERROR
zOPER_EXPORT zSHORT OPERATION
zViewIsUpdateable( zVIEW vView )
{
   return( MiGetUpdateForView( vView ) );
} // zViewIsUpdateable

static void
qbldelta( zLONG   lPatternLth,       // pattern length
          zCPCHAR cpcPattern,        // pattern to locate
          zPLONG  lplDelta1Table,    // Boyer-Moore Delta1 Table
          zPLONG  lplDelta2Table )   // Boyer-Moore Delta2 Table
{
   fnLoadDeltaOne( lPatternLth, (const zPUCHAR) cpcPattern, lplDelta1Table );
   fnLoadDeltaTwo( lPatternLth, (const zPUCHAR) cpcPattern, lplDelta2Table );
}

static void
fnLoadDeltaOne( zLONG  lPatternLth,
                const zPUCHAR szPattern,
                zPLONG lplDelta1Table )
{
   zLONG lPatternLthMinus1;
   zLONG k;

   lPatternLthMinus1 = lPatternLth - 1;

   /*                                                                 */
   /* init Delta1 to pattern length for all characters                */
   /*                                                                 */
   for ( k = 0; k < ASCIISIZE; k++ )
      lplDelta1Table[ k ] = lPatternLth;

   /*                                                                 */
   /* set Delta1 for characters in pattern                            */
   /*                                                                 */
   for ( k = 0; k < lPatternLth; k++ )
      lplDelta1Table[ szPattern[ k ] ] = lPatternLthMinus1 - k;

   /*                                                                 */
   /* set Delta1 to large value for last zCHAR in pattern             */
   /*                                                                 */
   lplDelta1Table[ szPattern[ lPatternLthMinus1 ] ] = LARGE;

}   /*  end of:  fnLoadDeltaOne  */

static void
fnLoadDeltaTwo( zLONG  lPatternLth,      /* using Rytter's correction  */
                const zPUCHAR szPattern, /* to Knuth's scheme          */
                zPLONG lplDelta2Table )
{
   int  k,
        q,  j,  t,
        qi, ji, ti,
        nTwicePatternLth;
   int  f[ MAXPATTERN ],
        fi[ MAXPATTERN ];

   nTwicePatternLth = lPatternLth * 2;

   for ( k = 1;  k <= lPatternLth;  k++ )
      lplDelta2Table[ k ] = nTwicePatternLth - k;

   j = lPatternLth;
   t = lPatternLth + 1;

   while ( j > 0 )
   {
      f[ j ] = t;
      while ( t <= lPatternLth  &&
              szPattern[ j - 1 ] != szPattern[ t - 1 ] )
      {
         if ( (zLONG) lPatternLth - j < lplDelta2Table[ t ] )
            lplDelta2Table[ t ] = lPatternLth - j;

         t = f[ t ];

      }   /*  end of:  while ( t <= ... )  */

      t--;
      j--;

   }   /*  end of:  while ( j > 0 )  */

   q = t;
   t = lPatternLth + 1 - q;
   qi = 1;
   ji = 1;
   ti = 0;

   while ( ji <= t )
   {
      fi[ ji ] = ti;
      while ( ti >= 1  &&  szPattern[ ji - 1 ] != szPattern[ ti - 1 ] )
         ti = fi[ ti ];
      ti++;
      ji++;
   }   /*  end of:   while ( ji <= t )  */

   while ( q < lPatternLth )
   {
      for ( k = qi;  k <= q; k++ )
         if ( (zLONG) lPatternLth + q - k  <  lplDelta2Table[ k ] )
            lplDelta2Table[ k ] = lPatternLth + q - k;

      qi = q + 1;
      q += t - fi[ t ];
      t = fi[ t ];
   }   /*  end of:  while ( q < lPatternLth )  */
}   /*  end of:  fnLoadDeltaTwo  */

////////////////////////////////////////////////////////////////////////
/*  KZEDT2AA.C  -  written by Doug Sorensen    - create date 02/10/89 */
/*                (C)  Quinsoft Inc., 1989    - last update 02/12/89 */
/*                                                                    */
/*  This program searches a buffer for a string using the Boyer-Moore */
/*  algorithm.                                                        */
/*                                                                    */
/*  Input Parms:                                                      */
/*     1. Pattern to locate                                           */
/*     2. Pattern length                                              */
/*     3. Buffer to be searched for pattern                           */
/*        - expanded(uncompressed) as necessary                      */
/*        - fixed length records                                      */
/*     4. Length of Buffer to be searched                             */
/*     5. Boyer-Moore Delta1 table                                    */
/*     6. Boyer-Moore Delta2 table                                    */
/*                                                                    */
/*  Returns:                                                          */
/*     1. Location of pattern within buffer(-1 if not found)         */
/*                                                                    */
////////////////////////////////////////////////////////////////////////
static zLONG
qfind( zLONG  lPatternLth,       // pattern length
       zCPCHAR cpcPattern1,      // pattern to locate
       zLONG  lBufferLth,        // length of buffer to be searched
       zPCHAR pchBuffer,            // buffer to be searched for pattern
       zPLONG lplDelta1Table,    // Boyer-Moore Delta1 Table
       zPLONG lplDelta2Table )   // Boyer-Moore Delta2 Table
{
   long lPatternLoc;
   int  lPatternLthMinus1;
   long j, k;
   unsigned char* szPattern = (unsigned char*)cpcPattern1;



   if ( lPatternLth <= 0 )
      return( -1 );       /* signal "not found in buffer" */

   lPatternLoc = -1;

   if ( lPatternLth == 1 )
   {
      k = 0;
      while ( k < lBufferLth )
      {
         if ( szPattern[ 0 ] == pchBuffer[ k ] )
         {
            lPatternLoc = k;
            break;
         }
         else
            k++;
      }

      return( lPatternLoc );
   }

   lPatternLthMinus1 = lPatternLth - 1;
   k = lPatternLthMinus1;

   while ( k < lBufferLth )
   {
      while ( k < lBufferLth )     /*  FAST Loop  */
      {
 /*      printf( "%c - k before = %d  ", pchBuffer[ k ], k );     */
         k += lplDelta1Table[(unsigned char) pchBuffer[ k ] ];
 /*      printf( " --  k after = %d\n", k );                 */
      }

      if ( k < LARGE )      /*  really past true end of string ??     */
         break;             /*    yes... terminate search             */
/*                                                                    */
/*  Undo the large skip caused by the match found in the FAST loop.   */
/*  Check for complete match in SLOW loop.                            */
/*                                                                    */
      k -= LARGE;
      k--;
  /*  printf( "k after FAST loop match = %d\n", k ); */
      j = lPatternLthMinus1 - 1;
      while ( pchBuffer[ k ] == szPattern[ j ] )     /* SLOW Loop */
      {
         if ( j > 0 )
         {
            k--;
            j--;
         }
         else
         {
            lPatternLoc = k;  /* pattern found ... terminate search    */
            k = LARGE;        /*  permit outer while exit              */
            break;            /*  exit inner while                     */
         }
      }   /*  end of:   while ( pchBuffer[ k ] == szPattern[ j ] )  */

      if ( lPatternLoc == -1 )   /*  pattern not yet found            */
      {
         j = k + lplDelta2Table[ j + 1 ];
         k += lplDelta1Table[ pchBuffer[ k ] ];
         if ( k < j  ||  k >= LARGE )  /* take largest possible jump */
            k = j;
      }

   }   /*  end of outer:  while ( k <= lBufferLth )   */

   return( lPatternLoc );

}   /*   end of:  qfind  */


// Function: zFindTextValueInAttribute
// Search for a text value in a string attribute
zOPER_EXPORT zSHORT OPERATION
zFindTextValueInAttribute( zCPCHAR cpcTextValue,
                           zVIEW   vView,
                           zCPCHAR cpcEntityName,
                           zCPCHAR cpcAttributeName )
{
   // Do a search on the string attribute vView.szEntityName.cpcAttributeName
   // by the string cpcTextValue. Return a 0 if cpcTextValue was found in the
   // attribute value and a -1 if it was not found.

   zSHORT nRC;
   int    lPatternLth;
   zLONG  lDelta1Table[ ASCIISIZE ];
   zLONG  lDelta2Table[ MAXPATTERN ];
   zLONG  lBufferLth;
   zLONG  lFindPos;
   zPCHAR pchBuffer;

   lPatternLth = zstrlen( cpcTextValue );
   if ( lPatternLth == 0 )
   {
      return( -1 );
   }

   // Builds a table to be used in qfind.
   GetAddrForAttribute( &pchBuffer, vView, cpcEntityName, cpcAttributeName );
   lBufferLth = zstrlen( pchBuffer );
   qbldelta( lPatternLth,
             cpcTextValue,
             lDelta1Table,
             lDelta2Table );

   // Searches the buffer area.
   lFindPos = qfind( lPatternLth,
                     cpcTextValue,
                     lBufferLth,          /* Buffer length */
                     pchBuffer,           /* Buffer text */
                     lDelta1Table,
                     lDelta2Table );

   if ( lFindPos == -1 )
      nRC = -1;
   else
      nRC = 0;

   return( nRC );

} // zFindTextValueInAttribute


// Function: zGetFirstEntityNameForView
// retrieves the first Entity of a View (Root-Entity)
zOPER_EXPORT zSHORT OPERATION
zGetFirstEntityNameForView( zVIEW  vViewToInstance,
                            zPCHAR pchEntityName )
{
   LPVIEWOD lpViewOD;
   LPVIEWENTITY lpViewEntity;

   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( vViewToInstance, 0 );
   if ( lpViewOD == 0 )
      return( -16 );

   lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity );

   strcpy_s( pchEntityName, 33, lpViewEntity->szName );
   return( 0 );
} // zGetFirstEntityNameForView


// Function: zGetNextEntityNameForView
// retrieves the next Entity of a View (relative to EntityName)
zOPER_EXPORT zSHORT OPERATION
zGetNextEntityNameForView( zVIEW    vViewToInstance,
                           zPCHAR   pchEntityName )
{
   LPVIEWENTITY lpViewEntity;
   zCHAR szEntityNameSave[ 33 ];
   LPVIEWOD lpViewOD;

   strcpy_s( szEntityNameSave, 33, pchEntityName );

   if ( (zstrcmp( pchEntityName, "" ) == 0) || (zstrcmp( AnsiUpper( szEntityNameSave ), "ROOT" ) == 0 ) )
      return( zGetFirstEntityNameForView( vViewToInstance, pchEntityName ) );

   lpViewOD = (LPVIEWOD) MiGetViewEntityForView( vViewToInstance, 0 );
   if ( !lpViewOD )
      return( -16 );

   // Look for a entity that matches pchEntityName
   for ( lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewOD->hFirstOD_Entity ); lpViewEntity;
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier ) )
   {
      if ( zstrcmp( lpViewEntity->szName, pchEntityName ) == 0 )
      {
         // Get the next entity.
         lpViewEntity = (LPVIEWENTITY) SysGetPointerFromHandle( lpViewEntity->hNextHier );
         if ( lpViewEntity )
            strcpy_s( pchEntityName, 33, lpViewEntity->szName );
         else
         {
            *pchEntityName = 0;   // No next entity.
            return( -1 );
         }
         return( 0 );
      }
   }

   // If we get here then we didn't find a match for pchEntityName.
   return( -16 );

} // zGetNextEntityNameForView


// Function: zGetFirstAttributeNameForEntity
// retrieves the first Attribute of a Entity
zOPER_EXPORT zSHORT OPERATION
zGetFirstAttributeNameForEntity( zVIEW    vViewToInstance,
                                 zCPCHAR  cpcEntityName,
                                 zPCHAR   pchAttributeName )
{
   LPVIEWENTITY lpViewOD;
   LPVIEWATTRIB lpViewAttrib;

   lpViewOD = (LPVIEWENTITY) MiGetViewEntityForView( vViewToInstance, cpcEntityName );
   if ( !lpViewOD )
      return( -16 );

   lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewOD->hFirstOD_Attrib );

   while ( lpViewAttrib && (lpViewAttrib->bHidden) )
   {
      lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
   }

   if ( lpViewAttrib )
      strcpy_s( pchAttributeName, 33, lpViewAttrib->szName );
   else
      return( -1 );

   return( 0 );
} // zGetFirstAttributeNameForEntity


// Function: zGetNextAttributeNameForEntity
// retrieves the next Attribute of a Entity (relative to AtrributeName)
zOPER_EXPORT zSHORT OPERATION
zGetNextAttributeNameForEntity( zVIEW   vViewToInstance,
                                zCPCHAR cpcEntityName,
                                zPCHAR  pchAttributeName )
{
   LPVIEWENTITY lpViewOD;
   LPVIEWATTRIB lpViewAttrib;

   if ( zstrcmp( cpcEntityName, "" ) == 0 )
   {
   // IssueError( vViewToInstance, 0, 0, "Die Funktion zGetNextAttributeNameForEntity() wurde ohne Entitätsnamen aufgerufen." );
      IssueError( vViewToInstance, 0, 0, "The function zGetNextAttributeNameForEntity() was called with no entity name." );
      return( -16 );
   }

   if ( zstrcmp( pchAttributeName, "" ) == 0 )
      return( zGetFirstAttributeNameForEntity( vViewToInstance, cpcEntityName, pchAttributeName ) );

   lpViewOD = (LPVIEWENTITY) MiGetViewEntityForView( vViewToInstance, cpcEntityName );
   if ( !lpViewOD )
      return( -16 );

   // Look for an attribute that matches pchAttributeName
   for ( lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewOD->hFirstOD_Attrib ); lpViewAttrib;
         lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib ))
   {
      if ( lpViewAttrib->bHidden )
         continue;

      if ( zstrcmp( lpViewAttrib->szName, pchAttributeName ) == 0 )
      {
         // Get the next attribute.
         do
         {
            lpViewAttrib = (LPVIEWATTRIB) SysGetPointerFromHandle( lpViewAttrib->hNextOD_Attrib );
         } while ( lpViewAttrib && (lpViewAttrib->bHidden) );

         if ( lpViewAttrib )
            strcpy_s( pchAttributeName, 33, lpViewAttrib->szName );

         else
         {
            *pchAttributeName = 0;   // No next attribute.
            return( -1 );
         }
         return( 0 );
      }
   }
   // If we get here then we didn't find a match for pchAttributeName.
   return( -16 );
} // zGetNextAttributeNameForEntity


// Function: zLodContainsEntity
// prüft, ob ein LOD eine bestimmte Entität enthält
// checks whether a LOD contains a certain entity
zOPER_EXPORT zSHORT OPERATION
zLodContainsEntity( zVIEW   vViewToInstance,
                    zCPCHAR cpcEntityNameToSearch )
{
   zSHORT    nRC;
   zCHAR     szEntityName[ 33 ];

   ZeidonStringCopy( szEntityName, 1, 0, "", 1, 0, 81 );
   nRC = 0;

   // durch alle Entitäten des LOD loopen, bis cpcEntityNameToSearch gefunden oder am Ende:
   while ( nRC >= zCURSOR_SET &&
           ZeidonStringCompare( cpcEntityNameToSearch, 1, 0, szEntityName, 1, 0, 33 ) != 0 )
   {
      nRC = zGetNextEntityNameForView( vViewToInstance, szEntityName );
   }

   // gesuchte Entität entspricht der aktuellen Entität:
   if ( ZeidonStringCompare( cpcEntityNameToSearch, 1, 0, szEntityName, 1, 0, 33 ) != 0 )
      return( -1 );
   else
      return( 1 );

} // zLodContainsEntity


// Function: zLodContainsAttribute
// prüft, ob eine Entität ein bestimmtes Attribut enthält
// checks whether an entity has a particular attribute
zOPER_EXPORT zSHORT OPERATION
zLodContainsAttribute( zVIEW   vViewToInstance,
                       zCPCHAR cpcEntityName,
                       zCPCHAR cpcAttributeNameToSearch )
{
   zSHORT    nRC;
   zCHAR     szAttributeName[ 33 ];

// ZeidonStringCopy( szAttributeName, 1, 0, "", 1, 0, 81 );
   szAttributeName[ 0 ] = 0;
   nRC = zCURSOR_SET;

   // durch alle Attribute des LOD loopen, bis cpcAttributeNameToSearch gefunden oder am Ende:
   // Looping through all the attributes of the LOD until cpcAttributeNameToSearch found or at the end:
   while ( nRC >= zCURSOR_SET && ZeidonStringCompare( cpcAttributeNameToSearch, 1, 0, szAttributeName, 1, 0, 33 ) != 0 )
   {
      nRC = zGetNextAttributeNameForEntity( vViewToInstance, cpcEntityName, szAttributeName );
   }

   // gesuchtes Attribut entspricht dem aktuellen Attribut:
   // requested attribute matches the current attribute:
   if ( ZeidonStringCompare( cpcAttributeNameToSearch, 1, 0, szAttributeName, 1, 0, 33 ) != 0 )
      return( -1 );
   else
      return( 1 );

   return( 0 );
} // zLodContainsAttribute

zOPER_EXPORT zLONG OPERATION
zStringToInteger( zCPCHAR cpcStr )
{
   return( atol( cpcStr ) );
} // zStringToInteger

zOPER_EXPORT zLONG OPERATION
zCharToOem( zPCHAR pchBuffer )
{
   CharToOem( pchBuffer, pchBuffer );
   return( 0 );

} // zCharToOem

////////////////////////////////////////////////
//
//   ENTRY:    zSearchSubString
//
//   PROTOTYPE:
//
//        zCPCHAR cpcSearchString
//        zCPCHAR cpcSubString
//        zCPCHAR cpcDirection = 'f'  - search forward in the string
//                             = 'b'  - search backward in the string
//        short   nPosition    start searching at this position
//
//   RETURN:  position of the first occurrence of the substring
//            -1, if the substring isn't found
//
////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zSearchSubString( zCPCHAR cpcSearchString,
                  zCPCHAR cpcSubString,
                  zCPCHAR cpcDirection,
                  zSHORT  nPosition )
{
   zSHORT nLoc;
   zSHORT nLthSearchString;
   zSHORT nLthSubString;

   nLthSearchString = (zSHORT) zstrlen( cpcSearchString );
   nLthSubString = (zSHORT) zstrlen( cpcSubString );

   if ( *cpcDirection == 'f' || *cpcDirection == 'F' )
   {
      // Search forward in SearchString.
      for ( nLoc = nPosition; nLoc <= nLthSearchString - nLthSubString; nLoc++ )
      {
          if ( zstrncmpi( cpcSearchString + nLoc, cpcSubString, nLthSubString ) == 0 )
         {
            return( nLoc );
         }
      }
   }

   if ( *cpcDirection == 'b' || *cpcDirection == 'B' )
   {
      // Search backward in SearchString.
      for ( nLoc = nPosition; nLoc >= 0; nLoc-- )
      {
         if ( zstrncmpi( cpcSearchString + nLoc, cpcSubString, nLthSubString ) == 0 )
         {
            return( nLoc );
         }
      }
   }

   return( -1 );   // SubString not found
}

////////////////////////////////////////////////
//
//   ENTRY:    zReplaceSubString
//
//             StartPos    EndPos
//             |           |
// "aaaaaaaaaaaxxxxxxxxxxxxaaaaaaaaaaaaa" szModString
//            "bbbbbbb"
//
// "aaaaaaaaaaabbbbbbbaaaaaaaaaaaaa"      result szModString
//
// REMARK:
//   ModString MUST be long enough, NO LENGTH CHECK
//
//   RETURN: 0
//           -16 Call Error
//
////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zReplaceSubString( zPCHAR  pchModString,  // string to be modified
                   zSHORT  nStartPos,     // start of modification
                   zSHORT  nEndPos,       // end of modification
                   zCPCHAR cpcRepString ) // replace string
{
   zSHORT nRepLth, nModRepLth, k;
   zPCHAR pch;

   if ( nStartPos < 0 || nStartPos < 0 )
      return( -16 );

   nModRepLth = nEndPos - nStartPos;

   if ( nModRepLth < 0 )
      return( -16 );

   nRepLth = (zSHORT) zstrlen( cpcRepString );
   if ( nRepLth > nModRepLth )
   {
      // result will be longer, create space by moving the tail towards string end.
      zSHORT nSpaceNeeded = nRepLth - nModRepLth;
      pch = pchModString + zstrlen( pchModString ); // points to end 0
      while ( pch >= pchModString + nEndPos )
      {
         // starting from back, move tail nSpaceNeeded byte backward
         *(pch + nSpaceNeeded) = *pch;
         --pch;
      }
   }
   else
   if ( nRepLth < nModRepLth )
   {
      // result will be shorter, move tail forward
      zSHORT nTooMuch = nModRepLth - nRepLth;
      pch = pchModString + nEndPos;
      while ( *pch )
      {
         // from nEndPos to end, move tail nTooMuch byte forward
         *(pch - nTooMuch) = *pch;
         ++pch;
      }

      // closing 0-byte
      *(pch - nTooMuch) = 0;
   }

   // Now, as we got here, the replace string exactly fits
   k = 0;
   pch = pchModString + nStartPos;
   while( cpcRepString[ k ] )
   {
      *(pch++) = cpcRepString[ k++ ];
   }

   return( 0 );
}

zOPER_EXPORT zSHORT OPERATION
zExpungeAllSubstring( zPCHAR pchOriginal,
                      zPCHAR pchExpunge )
{
   zPCHAR pch;
   zSHORT nLth;
   zSHORT nRC = 0;
   zSHORT nFullLth;

   nFullLth = (zSHORT) zstrlen( pchOriginal );

   nLth = (zSHORT) zstrlen( pchExpunge );
   if ( nLth > 0 )
   {
      pch = zstrstr( pchOriginal, pchExpunge );
      while ( pch )
      {
         nRC = 1;
         strcpy_s( pch, nFullLth, pch + nLth );
         pch = zstrstr( pch, pchExpunge );
      }
   }

   return( nRC );
}
/*
//----------------------------------------------------------------------
//
// .Operationname: GenFullQualPathFromRelPath
//
// .Description:  ............
//
// .Descriptor:    ............
//
// .Parameter:     zPCHAR pchPathName
//                 zPCHAR pchBaseDir
//                 zPCHAR pchOutput
//                 short nMaxLth
//                 ............
//
// .Returncode:    short
//
// ---------------------------------------------------------------------
//
// .Operation description
//
//  This function return a pathname in variable "output",
//  which is build in the following way
//  - pathname is full qualified, that means
//    - pathname begins with a: (where a is any alpha character)
//    - pathname begins with "\"
//    - pathname begins with "\\"
//    output = pathname
//  - pathname is not full qualified
//    - pathname = "" or pathname = "." ==> output = basedir
//    - otherwise: output = basedir + \ + pathname
//
//  Returncode: = 1, pathname was fully qualified
//              = 0, relative path is build
//              = -1, built pathname is too long
//
//----------------------------------------------------------------------
*/
zOPER_EXPORT zSHORT OPERATION
GenFullQualPathFromRelPath( zPCHAR pchPathName,  // Pathname or empty string
                            zPCHAR pchBaseDir,   // base for relative paths
                            zPCHAR pchOutput,    // Resultbuffer, must be great enough
                            zSHORT nMaxLth )
{
   zCHAR szTempPath[ zMAX_FILESPEC_LTH + 1 ];
   zCHAR szTempBase[ zMAX_FILESPEC_LTH + 1 ];
   // if pathname = "" || "."
// strcpy_s( pchOutput, nMaxLth, pchBaseDir );
   SysConvertEnvironmentString( szTempPath, zsizeof( szTempPath ), pchPathName );
   SysConvertEnvironmentString( szTempBase, zsizeof( szTempBase ), pchBaseDir );

   if ( strlen( szTempPath ) >= 2 )
   {
      /* Conditions for fully qualified pathnames:
         (first character = a-z OT A-Z) AND second character = :
          or
          first character = \
      */
      if ( (((szTempPath[ 0 ] >= 'a' && szTempPath[ 0 ] <= 'z') || (szTempPath[ 0 ] >= 'A' && szTempPath[ 0 ] <= 'Z')) && szTempPath[ 1 ] == ':') ||
           szTempPath[ 0 ] == '\\' )
       {
          strcpy_s( pchOutput, nMaxLth, pchPathName ); // fully qualified path
          return( 1 );
       }
       else
       {
          strcpy_s( pchOutput, nMaxLth, pchBaseDir );
          if ( szTempBase[ strlen( szTempBase ) - 1 ] != '\\' )
             strcat_s( pchOutput, nMaxLth, "\\" );  // add backslash, if it's needed

          strcat_s( pchOutput, nMaxLth, pchPathName ); // add pathname after basedir
      }
   }

   // The result, is it greater then the max. length ??
   if ( zstrlen( pchOutput ) < (size_t) nMaxLth )
      return( 0 );
   else
      return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION: FindFileStart
//
//  wrapper for FindFirstFile function
//
//  Parameter
//            szFileString = search string (input, containing wildcards)
//            pchReturnString = first matching file name (output)
//
//  Return
//            Handle, needed for subsequent calls
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
FindFileStart( zCPCHAR cpcFileString,
               zPCHAR  pchReturnString )
{
   HANDLE h;
   WIN32_FIND_DATA tFindFileData;
   LPWIN32_FIND_DATA lpFindFileData = &tFindFileData;

   h = FindFirstFile( cpcFileString, lpFindFileData );

   if ( lpFindFileData )
   {
      strcpy_s( pchReturnString, 256, lpFindFileData->cFileName );
   }

   return( (zLONG) h );

} // FindFileStart

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: FindFileNext
//
//   wrapper for FindNextFile
//
//   Parameter
//             lHandle = Handle returned from FindFileStart
//
//   Return    != 0 file found
//              == 0 no more file
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT  OPERATION
FindFileNext( zLONG  lHandle,
              zPCHAR pchReturnString )
{
   WIN32_FIND_DATA tFindFileData;
   LPWIN32_FIND_DATA lpFindFileData = &tFindFileData;

   if ( FindNextFile( (HANDLE) lHandle, lpFindFileData ) )
   {
      strcpy_s( pchReturnString, 256, lpFindFileData->cFileName );
      return( 1 );
   }
   else
   {
      pchReturnString[ 0 ] = 0;
      return( 0 );
   }

} // FindFileNext

/////////////////////////////////////////////////////////////////////////////
//
//   OPERATION: FindFileClose
//
//   wrapper for FindClose
//
//   Parameter
//             lHandle = Handle returned from FindFileStart
//
//   Return    != 0 success
//             == 0 error
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
FindFileClose( zLONG ulHandle )
{
   BOOL bRC;

   bRC = FindClose( (HANDLE)ulHandle );

   if ( bRC )
      return( 1 );
   else
      return( 0 );

} // FindFileClose

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zReplicate
// Author:         ASE
// Date:           7. December 2001
// last changed:   14.December 2001
// description:
//                 creates 'ResultString' consisting of 'String' repeated 'Repeats' times
//
// parameters:     cpcString     - Input String (to be replicated)
//                 uRepeats      - Number of times to repeat
//                 pchResultString  - Output/Result String
//                 uResultStringLth - Initial length of the Output String
//                                 ResultString is going to be null-terminated, so
//                                 the ResultString has to be 1 byte larger than the
//                                 Length of 'String' * Repeats
//
// returns:         0           - successful
//                 -1           - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zReplicate( zCPCHAR cpcString, zUSHORT uRepeats,
            zPCHAR pchResultString, zUSHORT uResultStringLth )
{
   zSHORT k = 0;

   // Check if the output size is large enough.
   if ( uResultStringLth < (zstrlen( cpcString ) * uRepeats) + 1 )
      return( -1 );

   if ( uRepeats < 1 )
   {
      if ( uResultStringLth < 1 )
         return( -1 );

      pchResultString[ 0 ] = 0;
      return( 0 );
   }

   // Use strcpy_s for the first 'replication'.
   strcpy_s( pchResultString, uResultStringLth, cpcString );

   // Append the remaining 'replications' using zstrcat.
   for ( k = 1; k < uRepeats; k++ )
   {
      strcat_s( pchResultString, uResultStringLth, cpcString );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zRTrim
// Author:         ASE
// Date:           13.December 2001
// last changed:   07.May 2002
// description:
//                 removes blanks and tabstops at the end of a string
//
// parameters:     cpcString       - Input String
//                 ResultString    - Output/Result String
//                 ResultStringLth - Initial length of the Output String
//                                ResultString is going to be null-terminated
//
// returns:         0           - success
//                 -1           - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zRTrim( zCPCHAR cpcString, zPCHAR ResultString, zUSHORT ResultStringLth )
{
   zLONG i = 0, lth;
   zCPCHAR pString = cpcString;

   // set pointer to the end of the string
   cpcString += strlen( cpcString ) - 1;

   // decrease string pointer as long as spaces are found
   while ( *cpcString == ' ' || *cpcString == '\t' )
      cpcString--;

   // calculate new string's length and, if the outstring is large enough, copy it.
   lth = (cpcString - pString) + 1;
   if ( ResultStringLth <= lth )
      return( -1 );
   strncpy_s( ResultString, ResultStringLth, pString, lth );
   ResultString[ lth ] = 0;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zLTrim
// Author:         ASE
// Date:           13.December 2001
// last changed:   15.January 2002
// description:
//                 removes blanks and tabstops at the start of a string
//
// parameters:     cpcString       - Input String
//                 ResultString    - Output/Result String
//                 ResultStringLth - Initial length of the Output String
//                                ResultString is going to be null-terminated
//
// returns:         0           - successful
//                 -1           - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zLTrim( zCPCHAR cpcString, zPCHAR ResultString, zUSHORT ResultStringLth )
{
   // increase string pointer as long as spaces are found
   while ( *cpcString == ' ' || *cpcString == '\t' )
      cpcString++;

   // calculate new string's length and, if the outstring is large enough, copy it.
   if ( ResultStringLth < (strlen( cpcString ) + 1) )
      return( -1 );

   strcpy_s( ResultString, ResultStringLth, cpcString );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zLeft
// Author:         ASE
// Date:           13.December 2001
// last changed:   15.January 2002
// description:
//                 puts the 'Numleft' leftmost characters of 'cpcString' in 'ResultString'.
//                 if 'Numleft' exceeds the Input Strings' length, 'Numleft' will be
//                 reduced to the cpcString's actual length.
//
// parameters:     cpcString     - Input String
//                 Numleft       - Number of characters to copy
//                 ResultString    - Output/Result String
//                 ResultStringLth - Initial length of the Output String
//                                ResultString is going to be null-terminated, so
//                                the ResultString has to be 1 byte larger than the
//                                Number of characters to copy
//
// returns:         0           - successful
//                 -1           - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zLeft( zCPCHAR cpcString, zUSHORT Numleft,
       zPCHAR ResultString, zUSHORT ResultStringLth )
{
   // check if 'Numleft' is within range
   if ( Numleft > strlen( cpcString ) )
      Numleft = (zUSHORT) strlen( cpcString );

   // check if ResultString is large enough
   if ( ResultStringLth <= Numleft )
      return( -1 );

   // copy and append terminating null
   strncpy_s( ResultString, ResultStringLth, cpcString, Numleft );
   ResultString[Numleft] = 0;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zRight
// Author:         ASE
// Date:           13.December 2001
// last changed:   15.January 2002
// description:
//                 puts the rightmost characters of a string into a new string
//                 if 'Numright' exceeds the Input Strings' length, Numright will be
//                 reduced to the String's actual length.
//
// parameters:     cpcString       - Input String
//                 Numright        - Number of characters to copy
//                 ResultString    - Output/Result String
//                 ResultStringLth - Initial length of the Output String
//                                ResultString is going to be null-terminated, so
//                                the ResultString has to be 1 byte larger than the
//                                Number of characters to copy
//
// returns:         0           - successful
//                 -1           - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zRight( zCPCHAR cpcString, zUSHORT Numright,
        zPCHAR ResultString, zUSHORT ResultStringLth )
{
   zUSHORT uLth = (zUSHORT) strlen( cpcString );

   // check if 'Numright' is within range
   if ( Numright > uLth )
      Numright = uLth;
   // check if ResultString is large enough
   if ( ResultStringLth < (Numright + 1) )
      return( -1 );

   // adjust string pointer and copy
   if ( uLth != Numright )
      cpcString += (uLth - Numright );

   strcpy_s( ResultString, ResultStringLth, cpcString );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zMax
// Author:         ASE
// Date:           13.December 2001
// last changed:   13.December 2001
// description:
//                 compares two values and returns the greater value. if the two values
//                 are equal in size, this value is returned aswell.
//
// parameters:     Value1       - First Value for the compare
//                 Value2       - Second Value for the compare
//
// returns:        Value        - The larger Value
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zMax( zVIEW lpView, zDECIMAL Value1, zDECIMAL Value2 )
{
   ZDecimal d1;
   ZDecimal d2;
   d1 = Value1;
   d2 = Value2;

   if ( d1 >= d2 )
      return( Value1 );

// if ( d2 > d1 )
      return( Value2 );

// return( Value1 ); // equal size
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zMin
// Author:         ASE
// Date:           13.December 2001
// last changed:   13.December 2001
// description:
//                 compares two values and returns the lower value. if the two values
//                 are equal in size, this value is returned aswell.
//
// parameters:     Value1       - First Value for the compare
//                 Value2       - Second Value for the compare
//
// returns:        Value        - The lower Value
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zMin( zVIEW lpView, zDECIMAL Value1, zDECIMAL Value2 )
{
   ZDecimal d1;
   ZDecimal d2;
   d1 = Value1;
   d2 = Value2;

   if ( d1 <= d2 )
      return( Value1 );

// if ( d2 < d1 )
      return( Value2 );

// return( Value1 ); // equal size
}

// Function: zRound
// rundet eine Dezimalzahl auf Scales-Stellen nach dem Komma
zOPER_EXPORT zSHORT OPERATION
zRound( zVIEW     lpView,
        zPDECIMAL pdDecimalValue,
        zSHORT    nScales )
{
   zDECIMAL  *dDecimal = pdDecimalValue;
   ZDecimal  dDec;
   ZDecimal  dDecDiff;
   ZDecimal  dMultiplier;
   ZDecimal  dBound;
   zLONG     lInt;
   zSHORT    nRC;

   dDec = *dDecimal;
   lInt = dDec;
   dDec = dDec - lInt;
   dDecDiff = 1 - dDec;
   nRC = zExponent( lpView, &dMultiplier, 10, nScales );
   nRC = zExponent( lpView, &dBound, 10, -1 * nScales );
   dBound = dBound / 2;
   dDecDiff = dDecDiff * dMultiplier;
   lInt = dDecDiff;
   dDecDiff = (dDecDiff - lInt) / dMultiplier;
   if ( dDecDiff > dBound )
   {
      dDecDiff = dDec * dMultiplier;
      lInt = dDecDiff;
      dDecDiff = (dDecDiff - lInt) / dMultiplier;
      dDec = *dDecimal;
      dDec = (dDec - dDecDiff);
      *dDecimal = dDec;
   }
   else
   {
      dDec = *dDecimal;
      dDec = (dDec + dDecDiff);
      *dDecimal = dDec;
   }

   return( 0 );
} // zRound

// Function: zExponent
// berechnet die Potenz zu einer Basis
zOPER_EXPORT zSHORT OPERATION
zExponent( zVIEW       lpView,
           zPDECIMAL   pdDecimalReturnValue,
           zSHORT      nBase,
           zSHORT      nExponent )
{
   zDECIMAL* dDecimal = pdDecimalReturnValue;
   zLONG     i;
   ZDecimal  dReturn;

   i = nExponent;
   dReturn = 1;
   if ( nExponent == 0 )
   {
      i = 0;
   }

   while ( i != 0 )
   {
      dReturn = dReturn * nBase;
      if ( nExponent > 0 )
      {
         i = i - 1;
      }
      else
      {
         i = i + 1;
      }

   }

   if ( nExponent < 0 )
   {
      dReturn = 1 / dReturn;
   }

   *dDecimal = (ZDecimal)( dReturn);

   return( 0 );
} // zExponent

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zAbs
// Author:         ASE
// Date:           13.December 2001
// last changed:   13.December 2001
// description:
//                 returns the absolute value of the input value
//
// parameters:     Value        - Input Value
//
// returns:        Value        - The absolute Value
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zAbs( zVIEW lpView, zDECIMAL Value )
{
   ZDecimal dTemp;
   dTemp = Value;
   double d = dTemp;

   dTemp = fabs( d );
   return( dTemp );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zCeil
// Author:         ASE
// Date:           13.December 2001
// last changed:   13.December 2001
// description:
//                 returns the smallest integer that is larger than
//                 or equal to the input-value
//
// parameters:     Value        - Input Value
//
// returns:        Value        - The result
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zCeil( zVIEW lpView, zDECIMAL Value )
{
   ZDecimal dTemp;
   dTemp = Value;
   double d = dTemp;

   dTemp = ceil( d );
   return( dTemp );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zFloor
// Author:         ASE
// Date:           13.December 2001
// last changed:   13.December 2001
// description:
//                 returns the largest integer that is smaller than or
//                 equal to the input-value
//
// parameters:     Value        - Input Value
//
// returns:        Value        - The result
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zFloor( zVIEW lpView, zDECIMAL Value )
{
   ZDecimal dTemp;
   dTemp = Value;
   double d = dTemp;

   dTemp = floor( d );
   return( dTemp );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zSqrt
// Author:         ASE
// Date:           09.January 2002
// last changed:   09.January 2002
// description:
//                 computes the square-root
//
// parameters:     Value        - Input Value
//
// returns:        Value        - square-root of Input Value
//                 -1           - invalid Input Value (negative)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zDECIMAL OPERATION
zSqrt( zVIEW lpView, zDECIMAL Value )
{
   ZDecimal dTemp;
   dTemp = Value;
   double d = dTemp;

   if ( dTemp < 0 )
      dTemp = -1;
   else
      dTemp = sqrt( d );

   return( dTemp );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   IsNumeric
// Author:         ASE
// Date:           17.January 2002
// last changed:   18.January 2002
// description:
//                 checks if a string contains a numeric value
//                 except numbers, also ',' and '.' are considered valid characters
//                 aswell as '+' or '-' as first characters.
//                 the input string must contain at least one digit to be considered
//                 numeric.
//
// parameters:     cpcString    - Input String
//
// returns:        0            - String contains a numeric value
//                 -1           - String contains non-numeric characters
//                 -2           - String contains no digits at all
//                 zCALL_ERROR  - Input String has length zero
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
IsNumeric( zCPCHAR cpcString )
{
   zULONG ulStringLth, i;

   ulStringLth = strlen( cpcString );
   if ( ulStringLth == 0 )
      return( zCALL_ERROR );

   if ( strcspn( cpcString, "1234567890" ) == ulStringLth )
      return( -2 );

   // check string character by character
   for ( i = 0; i < ulStringLth; i++ )
   {
      // check if it's a digit
      if ( isdigit( cpcString[ i ] ) == 0 )
      {
         // if not, check if it is a 'valid exception' like, . + or -
         // otherwise return error
         if ( cpcString[ i ] == ',' || cpcString[ i ] == '.' )
            continue;

         if ( (i == 0 && strchr( "+-", cpcString[ i ] ) != 0) )
            continue;

         return( -1 );
      }
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zLike
// Author:         ASE
// Date:           10.January 2002
// last changed:   10.January 2002
// description:
//                 compares a wildcard string with a string
//                 allowed wildcards are '*' and '?'.
//                 '?' = any character
//                 '*' = multiple or no arbitrary characters
//
// parameters:     Wild         - Wildcard String
//                 cpcString    - String to compare with
//
// returns:        0            - Strings do not match
//                 1            - Strings do match
//
// comments:       based on www.codeproject.com/useritems/wildcmp.asp
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zLike( zCPCHAR Wild, zCPCHAR cpcString )
{
   zCPCHAR cp = "", mp = "";

   while ( (*cpcString) && (*Wild != '*') )
   {
      if ( (*Wild != *cpcString) && (*Wild != '?') )
      {
         return( 0 );
      }

      Wild++;
      cpcString++;
   }

   while ( *cpcString )
   {
      if ( *Wild == '*' )
      {
         if ( !*++Wild )
         {
            return( 1 );
         }

         mp = Wild;
         cp = cpcString + 1;
      }
      else
      if ( (*Wild == *cpcString) || (*Wild == '?') )
      {
         Wild++;
         cpcString++;
      }
      else
      {
         Wild = mp;
         cpcString = cp++;
      }
   }

   while ( *Wild == '*' )
   {
      Wild++;
   }

   return( !*Wild );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zSoundex
// Author:         ASE
// Date:           11.January 2002
// last changed:   11.January 2002
// description:
//                 The Soundex-algorithm generates a code based on the pronunciation
//                 of a word. Similar sounding names like Meyer, Maier,
//                 Meier all have the same Soundex Code, so using Soundex in search
//                 routines could help finding more results. Soundex is based on
//                 english pronunciation and is often used to code surnames in
//                 genealogical research.
//
//                 The Input String must not contain blanks.
//
//                 The Output Soundex Code always has a length of 5 (4 digits +
//                 terminating null), there is no length-check, so make sure the
//                 place you reserved for SoundexString is large enough.
//
// parameters:     cpcString     - Input String
//                 SoundexString - resulting Soundex Code
//
// returns:        0             - Soundex Code generated
//                 -1            - Input String contains non-alphabetic characters
//                 zCALL_ERROR   - Input String has length zero
//
// comments:       further information on the Soundex-algorithm can be found at:
//                 http://www.geocities.com/Heartland/Hills/3916/soundex.html
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zSoundex( zCPCHAR cpcString, zPCHAR SoundexString )
{
   zCHAR ch, lastch = 0;
   zULONG i, szCount = 0;

   if ( strlen( cpcString ) == 0 )
      return( zCALL_ERROR );

   // check if string is valid
   for ( i = 0; i < strlen( cpcString ); i++ )
   {
      if ( isalpha( cpcString[ i ] ) == 0 || cpcString[ i ] == 'ß' )
      {
         return( -1 );
      }
   }

   // string is valid -> begin actual soundex-code-generation
   for ( i = 0; i < strlen( cpcString ) && szCount < 4; i++ )
   {
      ch = ztoupper( cpcString[ i ] );

      if ( strchr( "AEHIOUWY",ch ) != 0 && szCount != 0 )
      {
         lastch = ch;
         continue;
      }

      if ( strchr( "BFPV",ch ) != 0 )
         ch = '1';

      if ( strchr( "CGJKQSXZ",ch ) != 0 )
         ch = '2';

      if ( strchr( "DT",ch ) != 0 )
         ch = '3';

      if ( strchr( "L",ch ) != 0 )
         ch = '4';

      if ( strchr( "MN",ch ) != 0 )
         ch = '5';

      if ( strchr( "R",ch ) != 0 )
         ch = '6';

      if ( szCount == 0 )
      {
         SoundexString[ szCount ] = ztoupper( cpcString[ i ] );
         szCount++;
         lastch = ch;
         continue;
      }

      if ( ch != lastch )
      {
         SoundexString[ szCount ] = ch;
         szCount++;
      }

      lastch = ch;
   }

   // if necessary, fill the soundex-code up with zeros
   for ( i = 3; i > szCount - 1; i-- )
      SoundexString[ i ] = '0';

   SoundexString[ 4 ] = 0;

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zRemoveAllBlanks
// Author:         ASE
// Date:           16.January 2002
// last changed:   16.January 2002
// description:
//                 removes all blanks and tabstops from a string
//
// parameters:     cpcString       - Input String
//                 pchResultString - Output String (without blanks)
//                                   pchResultString is null-terminated.
//                 uMaxStringLth - Maximum Output String Length
//
// returns:         0           - successful
//                 -1           - failed: pchResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zRemoveAllBlanks( zCPCHAR cpcString,
                  zPCHAR  pchResultString,
                  zUSHORT uMaxStringLth )
{
   zLONG lLth = strlen( cpcString );
   zLONG lSpaceCnt = 0;
   zLONG lPos = 0;
   zLONG k = 0;

   // Count spaces/tabstops and check if the output string is large enough to
   // hold the modified string.
   for ( k = 0; k < lLth; k++ )
   {
      if ( cpcString[ k ] == ' ' || cpcString[ k ] == '\t' )
         lSpaceCnt++;
   }

   uMaxStringLth--;  // just to make tests below simpler

   // Step through the original string and copy wanted characters to
   // the output string.
   for ( k = 0; k < lLth; k++ )
   {
      if ( cpcString[ k ] != ' ' && cpcString[ k ] != '\t' )
      {
         pchResultString[ lPos ] = cpcString[ k ];
         if ( lPos < uMaxStringLth )
            lPos++;
         else
            break;
      }
   }

   pchResultString[ lPos ] = 0;  // null terminate return string
   if ( uMaxStringLth < lLth - lSpaceCnt )
      return( -1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zTagFromText
// Author:         ASE
// Date:           16.January 2002
// last changed:   16.January 2002
// description:
//                 removes all "Tag invalid" characters from a string
//
// parameters:     cpcString       - Input String
//                 pchResultString - Result String (null terminated without
//                                   invalid characters)
//                 uMaxStringLth - Maximum Output String Length
//
// returns:         0           - successful
//                 -1           - failed: pchResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zTagFromText( zCPCHAR cpcString,
              zPCHAR  pchResultString,
              zUSHORT uMaxStringLth )
{
   zLONG lLth = strlen( cpcString );
   zLONG lSpaceCnt = 0;
   zLONG lPos = 0;
   zBOOL bSpace;
   zLONG k = 0;

   // Count spaces/tabstops and check if the output string is large enough to
   // hold the modified string.
   for ( k = 0; k < lLth; k++ )
   {
      if ( cpcString[ k ] == ' ' || cpcString[ k ] == '\t' )
         lSpaceCnt++;
   }

   uMaxStringLth--;  // just to make tests below simpler

   // Step through the original string and copy wanted characters to
   // the output string.
   for ( k = 0; k < lLth; k++ )
   {
      bSpace = FALSE;
      while ( cpcString[ k ] == ' ' || cpcString[ k ] == '\t' )
      {
         bSpace = TRUE;
         k++;
      }

      if ( cpcString[ k ] == '-' )
         pchResultString[ lPos ] = '_';
      else
      if ( zisalnum( cpcString[ k ] ) == 0 && cpcString[ k ] != '_' )
         continue;
      else
      {
         if ( bSpace )
            pchResultString[ lPos ] = ztoupper( cpcString[ k ] );
         else
            pchResultString[ lPos ] = cpcString[ k ];
      }

      if ( lPos < uMaxStringLth )
         lPos++;
      else
         break;
   }

   pchResultString[ lPos ] = 0;  // null terminate return string
   if ( uMaxStringLth < lLth - lSpaceCnt )
      return( -1 );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zGetDelimitedString
// Author:         ASE
// Date:           16.January 2002
// last changed:   16.January 2002
// description:
//                 Get a delimited string and removes it from the source
//
// parameters:     pchString       - Source String
//                 pchDelimiter    - Delimiter character
//                 pchResultString - Result String (null terminated without
//                                   invalid characters)
//                 nResultStringLth - Result String Length
//
// returns:        >= 0        - successful
//                 -1          - failed: no ResultString
//                 -2          - failed: ResultString too short
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zGetDelimitedString( zPCHAR pchSource,
                     zPCHAR pchDelimiter,
                     zPCHAR pchResultString,
                     zSHORT nResultStringMaxLth )
{
   zPCHAR pch = zstrchr( pchSource, pchDelimiter[ 0 ] );
   zSHORT nRC;

   if ( pch )
   {
      zSHORT nLth;

      *pch = 0;
      nLth = (zSHORT) zstrlen( pchSource );
      if ( nLth <= nResultStringMaxLth )
      {
         strcpy_s( pchResultString, nResultStringMaxLth, pchSource );
         strcpy_s( pchSource, nLth, pch + 1 );
         return( nLth );
      }
      else
         nRC = -2;
   }
   else
      nRC = -1;

   pchResultString[ 0 ] = 0;
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// Functionname:   zAppendQuotedString
// Author:         ASE
// Date:           28.February 2007
// last changed:   28.February 2007
//
// description:    Append a string to a quoted string.  The quotes may not
//                 be real quotes, but a stand-in character such as "^".
//                 If the target is does not begin with the SkipString (such
//                 as "style=") and the DfltQuoteChar is specified, the
//                 target string will be initialized with the SkipString
//                 and quoted using the specified DfltQuoteChar.
//
// parameters:     pchTarget       - Target String
//                 pchAppendString - String to append to target
//                 pchSkipString   - Skip literal prior to locating quote
//                 nResultStringLth - Result String Length
//
// returns:        result string length
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
zAppendQuotedString( zPCHAR pchTarget,        // the only place in the Zeidon tools that this is called is with a target length of 256 + 1
                     zPCHAR pchAppendString,  // so we are not passing the maximum length in - if this is called with a target that has a
                     zPCHAR pchSkipString,    // shorter maximum length, an overlay will occur
                     zPCHAR pchDfltQuoteChar )
{
   zCHAR  ch;
   zLONG  lLth;

   lLth = zstrlen( pchTarget );
   if ( lLth == 0 )
   {
      if ( pchDfltQuoteChar && pchDfltQuoteChar[ 0 ] )
      {
         // Build the target using the SkipString + DfltQuoteChar.
         if ( pchSkipString && pchSkipString[ 0 ] )
         {
         // lLth = strlen( pchAppendString ) + strlen( pchSkipString );
            strcpy_s( pchTarget, 257, pchSkipString );
            lLth = (zSHORT) zstrlen( pchTarget );
         }

         ch = pchDfltQuoteChar[ 0 ];
         pchTarget[ lLth++ ] = ch;
         pchTarget[ lLth++ ] = ch;
         pchTarget[ lLth ] = 0;
      }
      else
      {
         // Build the target from the AppendString.
         lLth = strlen( pchAppendString );
         strcpy_s( pchTarget, 257, pchAppendString );
         lLth = zstrlen( pchTarget );
         return( (zSHORT) lLth );
      }
   }
   else
   {
      char szTemp[ 8192 ];  // should be way big enough (256 should be ok)

      // Target is not empty.
      if ( pchSkipString && pchSkipString[ 0 ] )
      {
         zSHORT nSkipLth = (zSHORT) zstrlen( pchSkipString );

         if ( zstrncmp( pchTarget, pchSkipString, nSkipLth ) == 0 )
         {
            // The Target begins with the SkipString ... assume it also has the quote char as part of the string.
            ch = pchTarget[ nSkipLth ];
         }
         else
         if ( pchDfltQuoteChar && pchDfltQuoteChar[ 0 ] )
         {
            // Ensure the Target begins with the SkipString + DfltQuoteChar.
            ch = pchDfltQuoteChar[ 0 ];
            if ( ch == pchTarget[ 0 ] )
            {
               // It's already quoted.
               strcpy_s( szTemp, zsizeof( szTemp ), pchSkipString );
               strcat_s( szTemp, zsizeof( szTemp ),pchTarget );
               lLth = zstrlen( szTemp );
               strcpy_s( pchTarget, 257, szTemp );
            }
            else
            {
               // Needs to be quoted.
               strcpy_s( szTemp, zsizeof( szTemp ), pchSkipString );
               strcat_s( szTemp, zsizeof( szTemp ), pchDfltQuoteChar );
               strcat_s( szTemp, zsizeof( szTemp ), pchTarget );
               lLth = zstrlen( szTemp );
               strcpy_s( pchTarget, 257, szTemp );
               pchTarget[ lLth++ ] = ch;
               pchTarget[ lLth ] = 0;
            }
         }
         else
         {
            // Assume the Target begins with the QuoteChar.
            ch = pchTarget[ 0 ];
            strcpy_s( szTemp, zsizeof( szTemp ), pchSkipString );
            strcat_s( szTemp, zsizeof( szTemp ), pchTarget );
            lLth = zstrlen( szTemp );
            strcpy_s( pchTarget, 257, szTemp );
         }
      }
      else
      {
         if ( pchDfltQuoteChar && pchDfltQuoteChar[ 0 ] )
         {
            if ( pchTarget[ 0 ] == pchDfltQuoteChar[ 0 ] || pchTarget[ 0 ] == pchTarget[ lLth - 1 ] )
            {
               ch = pchTarget[ 0 ];  // good to go!
            }
            else
            {
               ch = pchDfltQuoteChar[ 0 ];
               szTemp[ 0 ] = ch;
               strcpy_s( szTemp + 1, zsizeof( szTemp ), pchTarget );
               lLth = zstrlen( szTemp );
               strcpy_s( pchTarget, 257, szTemp );
               pchTarget[ lLth++ ] = ch;
               pchTarget[ lLth ] = 0;
            }
         }
         else
         {
            // Assume the Target begins with the QuoteChar.
            ch = pchTarget[ 0 ];
         }
      }
   }

   if ( lLth > 0 && pchTarget[ lLth - 1 ] == ch )
      lLth--;
   else
      ch = 0;

   strcpy_s( pchTarget + lLth, 257 - lLth, pchAppendString );
   lLth = zstrlen( pchTarget );
   if ( ch )
   {
      pchTarget[ lLth++ ] = ch;
      pchTarget[ lLth ] = 0;
   }
   else
   if ( pchDfltQuoteChar && pchDfltQuoteChar[ 0 ] )
   {
      lLth = zstrlen( pchSkipString ) + zstrlen( pchDfltQuoteChar ) + zstrlen( pchAppendString );
      strcpy_s( pchTarget, 257, pchSkipString );
      strcat_s( pchTarget, 257, pchDfltQuoteChar );
      strcat_s( pchTarget, 257, pchAppendString );
      lLth = zstrlen( pchTarget );
      pchTarget[ lLth++ ] = pchDfltQuoteChar[ 0 ];
      pchTarget[ lLth ] = 0;
   }

   return( (zSHORT) lLth );
}

#ifdef __cplusplus
} //end of extern "C"
#endif

