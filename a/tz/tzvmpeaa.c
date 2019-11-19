/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   tzvmpeaa.c - VML SubSystem Parser SyntxError Module
// DESCRIPTION:   This is the module that contains all of the error
//                messages and the operation SyntaxError to inform the
//                user that a syntaxerror has been encountered.
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 - 2016 QuinSoft, Inc.   All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        Jeffrey S. Beam
// DATE:          1992/07/22
// API:           MicroSoft foundation classes and Windows 3.1 SDK
// ENVIRONMENT:   Windows 3.1
// REVISION:      0.9A   1995/08/30
//
// HISTORY:
//
//    1995/08/05: Beginning of 10B branch
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG
2001.09.26  HH
   added eQINVALIDSYNTAX.
2001.07.27  HH
   stop parsing on eQNOOPERATIONNAMEGIVEN.
2000.09.25  HH
   minor change for error eQINVALIDFUNCTIONDEF.
2000.08.11  HH
   added new messages eQINVALIDFUNCTIONPARAM and eQINVALIDFUNCTIONDEF.

OLD CHANGE LOG
20.07.1997  DonC
   Corrected handling of szExprCode and szTextCode so that IF statements
   could handle a larger number of expressions within the IF.

15.02.1998  DonC
   Added new error messages eQINVALIDLEFTWHEREAQ and eQOPERINCOMPOUNDDEXPR.

17.06.1998  HH
   Added new error message eQSTRINGCONCATPAREN.

16.02.1999  HH
   SyntaxError message eQINVALIDSYNTAXINWHERE.

02.03.1999  HH
   change Message 503, so that it fits a little bit better to the error
   TB519 "SET CURSOR WHERE ... AND V.E.A DOES NOT EXIST" .
*/

#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzvsecaa.h"
#include "tzvmpopr.hg"
#include "ZeidonOp.H"


/////////////////////////////////////////////////////////////////////////////
//
// OPERATIONS:  SyntaxError
//
/////////////////////////////////////////////////////////////////////////////

zPCHAR g_szMessages[ ] =
{
   // 501
   "Attempting to redefine an operation. Check for duplicate names.",
   // 502
   "The name of the operation was not given or invalid.",
   // 503
   "Parse problem caused by invalid syntax - possibly no operations have been defined in this source.",
   // 504
   "A variable by this name was already defined in this operation.",
   // 505
   "The variable name is missing from the variable declaration.",
   // 506
   "Attempting to reference a variable the was not declared above.",
   // 507
   "Attempting to reference a view that was not declared above.",
   // 508
   "Attempting to use a variable as a view. Views must be defined using the View declaraion. ",
   // 509
   "The entity name referenced was not declared as a variable of type ENTITYNAME.",
   // 510
   "The entity name referenced was not defined to this object type.",
   // 511
   "The attribute name referenced was not declared as a variable of type ATTRIBUTENAME.",
   // 512
   "The attribute name referenced was not defined to this object type.",
   // 513
   "Parameters must be delimited by commas.",
   // 514
   "All declarations must precede the first statement in an operation",
   // 515
   "No length was given for the string.",
   // 516
   "The view name was not given in the view declaration.",
   // 517
   "No object type given for the view declaration.",
   // 518
   "Statements cannot follow a return.",
   // 519
   "Invalid BASED ON LOD Phrase.",
   // 520
   "No condition given after the IF.",
   // 521
   "IF was not terminated with an END.",
   // 522
   "No condition given after the UNTIL.",
   // 523
   "No condition given after the WHILE.",
   // 524
   "WHILE was not terminated with an END.",
   // 525
   "CASE Needs a value to test.",
   // 526
   "CASE was not terminated with an END.",
   // 527
   "OF cannot follow an otherwise.",
   // 528
   "Invalid compound expression.",
   // 529
   "This combination of data types is invalid in the expression.",
   // eQOPERATIONTYPENOTGIVEN = 530
   "Unknown token type or character string found. Can't determine the error",
   // 531
   "The LOD given could not be loaded.",
   // 532
   "An invalid token was found where a value was expected.",
   // 533
   "Defines cannot be indexed as strings. ",
   // 534
   "Only text data can be indexed. ",
   // 535
   "A define was used where a variable was expected. Check parameter list",
   // 536
   "Too few parameters were supplied to this operation call",
   // 537
   "Too many parameters were supplied to this operation call",
   // eQINVALIDSYNTAX = 538
   "Invalid Syntax -",
   // 539
   "An operation was referenced that could not be located",
   // 540
   "An index was placed on a variable that was not a string type",
   // 541
   "The attribute qualifier is undefined or cannot be located.",
   // 542
   "The Registered view referenced was not found.",
   // 543
   "The view declaration is invalid",
   // 544
   "The target string cannot be one of the source strings.",
   // 545
   "An operation must have a view in its parm list to execute an Activate.",
   // 546
   "Arithmetic expressions are not allowed in IF statements.",
   // 547
   "Too many expressions in IF statement defined.",
   // 548
   "A string constant must be delimited by double quotes.",
   // eQINVALIDLEFTWHEREAQ = 549
   "A WHERE condition refers to an invalid view",
   // eQINVALIDSYNTAXINWHERE = 550
   "A WHERE condition contains invalid syntax, the error position might be one ore more lines above.",
   // eQSTRINGCONCATPAREN = 551
   "Parentheses are not allowed in a string concatenating expression.",
   // eQCOMPLEXEXPR = 552
   "A complex expression contains an illegal combination of data types.",
   // eQINVALIDFUNCTIONPARAM = 553
   "A Function call contains an invalid parameter.",
   // eQINVALIDFUNCTIONDEF = 554
   "Invalid Function Definition",
   // eQINVALIDCONSTANTDEF = 555
   "Invalid Function Definition",
   // eQINVALIDUNARYOPERATOR = 556
   "Unary Operator Invalid in Context ",
   // eMULTIPLERETURNARGUMENTS = 557
   "Multiple return arguments invalid in a Java operation ",
   // eQUALIFYINGVIEWUNDEFINED = 558
   "Qualifying view must be available for Java operation ",
};

// OPERATION: SyntaxError
//
// DESCRIPTION: SyntaxError for right now, trace lines error messages
//

zOPER_EXPORT zLONG OPERATION
SyntaxError( zVIEW vSubtask, zLONG lErrorNumber, zPCHAR szErrorString )
{
   zVIEW vError;
   zVIEW vSearchError;
   zBOOL bFound;
   zBOOL bDone;
   zCHAR szMessage[ 256 ];
   zCHAR szNum[ 10 ];
   zLONG lRC = 0;

   if ( GetViewByName( &vError, "TZVMEROO", vSubtask, zLEVEL_TASK ) > 0 )
   {
   // IssueError( vError, 0, 0, "Syntax Error" );

      // build msc type error message
      GetStringFromAttribute( szMessage, zsizeof( szMessage ), vError, "Root", "SourceFileName" );
      strcat_s( szMessage, zsizeof( szMessage ), "(" );
      zltoa( qqlineno, szNum, zsizeof( szNum ) );
      strcat_s( szMessage, zsizeof( szMessage ), szNum );
      strcat_s( szMessage, zsizeof( szMessage ), ") : error " );
      zltoa( lErrorNumber, szNum, zsizeof( szNum ) );
      strcat_s( szMessage, zsizeof( szMessage ), szNum );
      strcat_s( szMessage, zsizeof( szMessage ), ": " );
      strcat_s( szMessage, zsizeof( szMessage ), g_szMessages[ lErrorNumber - 501 ] );
      switch ( lErrorNumber )
      {
         case eQINVALIDFUNCTIONPARAM /*553*/:
         case eQINVALIDFUNCTIONDEF /*554*/  :
         case 556:
            break;
         default:
            strcat_s( szMessage, zsizeof( szMessage ), " Last Token " );
            if ( !szErrorString || !szErrorString[ 0 ] )
               strcat_s( szMessage, zsizeof( szMessage ), sQToken.szToken );
      }

      strcat_s( szMessage, zsizeof( szMessage ), szErrorString );

      // look to see if a similar error message has been posted
      // we need to match both nErrorNumber and szErrorString
      CreateViewFromViewForTask( &vSearchError, vError, 0 );

      bFound = FALSE;
      bDone = FALSE;

      // Try to find 1st occurrence of current Last Token
      lRC = SetCursorFirstEntityByString( vSearchError, "Error", "LastToken", szErrorString, "" );

      // if none was found, skip the following loop
      if ( lRC < zCURSOR_SET )
         bDone = TRUE;

      while ( !bDone )
      {
         // If we have found an occurrence of current Last Token
         if ( lRC >= zCURSOR_SET )
         {
            // check error number
            lRC = CompareAttributeToInteger( vSearchError, "Error", "ErrorNumber", lErrorNumber );
            // if a match
            if ( lRC == 0 )
            {
               bDone = TRUE;
               bFound = TRUE;
            }

            // otherwise, look again
            else
            {
               lRC = SetCursorNextEntityByString( vSearchError, "Error", "LastToken", szErrorString, "" );
            }
         }
         // otherwise, no more occurrences of szErrorString found
         else
         {
            // so we quit looking
            bDone = TRUE;
         }
      }

      // if a simlar error was NOT previously issued
      if ( bFound == FALSE )
      {
         // put the error in error LOD
         CreateEntity( vError, "Error", zPOS_AFTER );
         SetAttributeFromString ( vError, "Error", "String", szMessage );
         SetAttributeFromString ( vError, "Error", "LastToken", szErrorString );
         SetAttributeFromInteger( vError, "Error", "ErrorNumber", lErrorNumber );

      // SysWriteLine( g_lfErrorHandle, "------" );
      // SysWriteLine( g_lfErrorHandle, "------" );
         SysWriteLine( vError, g_lfErrorHandle, szMessage );
      }

      // drop the search view
      DropView( vSearchError );
   }

   // some errors require the parsing process to be stopped,
   //  because Object Engine errors would come up.
   switch ( lErrorNumber )
   {
      case eQNOOPERATIONNAMEGIVEN:
         SkipRemainingFile( );
         TraceLineS( "VML Parsing STOPPED on ERROR --", szMessage );
         break;

      default:
         TraceLineS( "VML Parsing ERROR --", szMessage );
         break;
   }

   return( 0 );
}
