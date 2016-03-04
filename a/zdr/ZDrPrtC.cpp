/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrprtc.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of "C interface" routines into the
// C++ functionality for printing available through the Zeidon driver.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.06.13    DKS    Z10
//    On cancel from a printer setup dialog, set the spawning window to
//    be the topmost window.
//
// 2002.02.20    BL     Z10
//    1. do not remove leading blanks
//    2. improve handling of words which do not fit on one line R56367
//
// 2002.01.30    fh     Z10
//    to fix the "12345678<tab>test" problem, we now calc:
//      lTabStopExtent * 63 / 64 (old: lTabStopExtent * 31 / 32).
//
// 2002.01.28    DKS    Z10
//    Fixed overflow bug when line with TabStops required a line break.
//
// 2002.01.16    DKS    Z10
//    Added support for TabStops in a report.
//
// 2001.09.25    PAS    Z10
//    Trace error when report operation not found.
//
// 2001.09.21    PAS    Z10
//    Added operation to support Report Group Exits.
//
// 2001.09.20    PAS    Z10
//    Fixed empty line print.
//
// 2001.09.06    DKS    Z10
//    Corrected the computation of font height for printing multiline text.
//
// 2001.06.01    PAS
//    When printing multiline text, the check for overlapping into the end
//    (usually footer) was one line too long.  The check was using the next
//    Top instead of next Bottom.
//
// 2000.08.07    RG     Z10
//    Changed declaration of fontsize from zSHORT to zLONG to remove
//    compiler warnings in Zeidon Tools
//
// 2000.05.24    DKS    Reports
//    Protected against null hDC in Print routines.
//
// 1999.02.16    DKS
//    Used new option zLOADLIB_RESOURCES on SysLoadLibraryWithErrFlag.
//
// 1999.02.02    DKS
//    Added default font support for reports.
//
// 1999.01.21    DKS
//    Text enhancement completed Escapement implementation.
//
// 1999.01.11    DKS
//    Added Escapement to TextBOI functions.
//
// 1999.01.04    DKS
//    Added box support for text.
//
// 1998.12.29    DKS
//    Permit face name to be specified for the font.
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

typedef zSHORT (POPERATION zFARPROC_REPORT)( zVIEW, zVIEW, zCPCHAR,   \
                                             zCPCHAR, zSHORT, zSHORT );

/////////////////////////////////////////////////////////////////////////////
//
// This function sets up a printer.
//    - view to the application's subtask
//
// Returns:
//  - TRUE  ==> SetupPrinter successful
//  - FALSE ==> SetupPrinter unsuccessful
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SetupPrinter( zVIEW vSubtask )
{
// CDC dc;
   CPrintDialog dlg( FALSE );
   if ( dlg.DoModal( ) == IDOK )
      return( TRUE );
   else
      return( FALSE );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function sets up a remote printer task.  The parameters are:
//    - view to the application's subtask
//    - name of the entity to print
//    - name of the report to print
//    - report flags: +8 - Print multiple entities at top level.
//                   +32 - Count the number of pages prior to printing.
//    - semi-colon separated list of view names required for report, the first
//      of which must be the primary Source OI (driving the print process).
//
// The return value is the handle to the printer task which is passed
// to all operations which manage print functionality.
// A return value of 0 indicates the printer could not be initialized.
// A return value of -1 indicates there was a problem with a named view.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG  OPERATION
OpenRemoteReportFromClient( zVIEW   vSubtask,
                            zCPCHAR cpcSubobjectEntityName,
                            zCPCHAR cpcReportDefName,
                            zPCHAR  pchReportName,
                            zLONG   lFlags,
                            zCPCHAR cpcViewNameList )
{
   zCHAR  szApplServer[ 256 ];
   zCHAR  szLocalAddress[ 256 ];
   zCHAR  szGroup[ 128 ];
   zVIEW  vSourceOI;
   zVIEW  vData[ 10 ];
   zSHORT nCnt = 0;
   zPCHAR pch = (zPCHAR) cpcViewNameList;
   zPCHAR pchSemi;
   zLONG  lRC;

   lFlags |= zREPORT_PRINT2PDF;
   while ( pch && *pch )
   {
      pchSemi = zstrchr( pch, ';' );
      if ( pchSemi )
         *pchSemi = 0;

      if ( GetViewByName( &vData[ nCnt ], pch, vSubtask, zLEVEL_ANY ) > 0 )
      {
         nCnt++;
         if ( pchSemi )
         {
            *pchSemi = ';';
            pchSemi++;
         }

         pch = pchSemi;
      }
      else
      {
         TraceLineS( "ZDr OpenRemoteReportFromClient invalid view name: ",
                     pch );
         if ( pchSemi )
            *pchSemi = ';';

         return( -1 );
      }
   }

   vSourceOI = vData[ 0 ];

   // Check to see if the local machine is the server.
   szGroup[ 0 ] = '[';
   SysReadZeidonIni( -1, "[Network]", "DefaultNetwork", szGroup + 1, zsizeof( szGroup ) - 1 );
   strcat_s( szGroup + 1, zsizeof( szGroup ) - 1, "]" );
   SysReadZeidonIni( -1, szGroup, "ApplServer", szApplServer, zsizeof( szApplServer ) );
   NetGetLocalHostAddress( vSubtask, 0, szLocalAddress );
   if ( zstrcmp( szLocalAddress, szApplServer ) == 0 )
   {
      lRC = PrintReportToPDF( vSubtask, vSourceOI, cpcSubobjectEntityName, cpcReportDefName, pchReportName, lFlags );
   }
   else
   {
      zCHAR  szAppName[ zZEIDON_NAME_LTH + 1 ];
      zVIEW  vZeidonTools;

      GetApplDirectoryFromView( szAppName, vSubtask, zAPPL_NAME, zsizeof( szAppName ) );
      SfCreateSubtask( &vZeidonTools, 0, "Zeidon_Tools" );

      // Skeleton operation:
      // network name, server name, ...
      // Use zero for network name.
      // Pass in address of server (from INI) if known.
      // Subtask view to let OE know what LPLR we are working with ...
      //   Zeidon_System or Zeidon_Tools at first, the real LPLR will have
      //   to be specified as an argument to the function.
      // BOOL - TRUE = object; FALSE = global
      // Since not an object operation, this will be the name of the DLL
      //   containing the operation OE will call.
      // Operation name.
      // Pointer to a void (return code from the actual operation).
      // String describing the argument list ... L=Long RC; V=VIEW as only
      //   parameter (prefix with lower case p to make the type a pointer).
      // Parameters to the operation (variable function list).
      // Check apsvdiso.c for a more complex example.
      //
      // NetCallOperation( 0, pszServerAddress, vApplASubtask, TRUE,
      //                   "ApSvDisO", "oApSvDisO_ActivateOI_FromFile",
      //                   &lRC, "LSSpVV", "zeidon.app", "kzapploo",
      //                   &vKZAPPLOO, 0 );
      //
      // Call stub rather than actual operation
      switch ( nCnt )
      {
         case 10:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVVVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ], vData[ 5 ], vData[ 6 ], vData[ 7 ],
                              vData[ 8 ], vData[ 9 ] );
            break;

         case 9:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ], vData[ 5 ], vData[ 6 ], vData[ 7 ],
                              vData[ 8 ] );
            break;

         case 8:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ], vData[ 5 ], vData[ 6 ], vData[ 7 ] );
            break;

         case 7:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ], vData[ 5 ], vData[ 6 ] );
            break;

         case 6:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ], vData[ 5 ] );
            break;

         case 5:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ],
                              vData[ 4 ] );
            break;

         case 4:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ], vData[ 3 ] );
            break;

         case 3:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ], vData[ 2 ] );
            break;

         case 2:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSVV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ], vData[ 1 ] );
            break;

         case 1:
            NetCallOperation( 0, szApplServer, vZeidonTools, FALSE, "ZDrApp",
                              "PrintReportToPDF", &lRC, "SSSpSLSV",
                              szAppName, cpcSubobjectEntityName,
                              cpcReportDefName, &pchReportName,
                              lFlags, cpcViewNameList,
                              vData[ 0 ] );
            break;

         default:
            TraceLineS( "ZDr OpenRemoteReportFromClient requires at least ",
                        "one and no more than ten view names" );
            lRC = -1;
      }

      SfDropSubtask( vZeidonTools, 0 );
   }

   return( lRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function sets up a printer task.  The parameters are:
//    - view to the application's subtask
//    - name of the DLL in which the page printing function is located
//    - name of the function which will handle the printing for the
//      application.  (Note:  this function will receive the current page
//      number as a parameter and must return TRUE if there are more
//      pages to be printed ... FALSE if all pages of the document
//      (including the current page) have been printed).
//    - 0 ==> print to the printer ... 1 ==> print preview ...
//      +2 ==> close subtask when closing the print preview window
//    - Banding ... not sure of impact at this point
//    - Scale ... TRUE ==> print to scale ... FALSE ==> fit to preview window
//    - Prompt ... prompt user to setup the printer prior to printing
//    - name of the report to print
//    - name of the entity to print
//
// The return value is the handle to the printer task which is passed
// to all operations which manage print functionality.  A return value
// of zero indicates the printer could not be initialized.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG  OPERATION
OpenReport( zVIEW   vSubtask,
            zCPCHAR cpcDLL_Name,
            zCPCHAR cpcPrintPageFunction,
            zLONG   lPrintFlags,
            zSHORT  bBanding,
            zSHORT  bScale,
            zLONG   bPrompt,
            zCPCHAR cpcReportDefName,
            zPCHAR  pchReportInfo )
{
   ZSubtask *pZSubtask;

   if ( vSubtask == 0 ||
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) ||
        (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) )
   {
      return( FALSE );
   }

   // Prevent recursion ... one printing task at a time.
   pZSubtask->m_pZTask->m_uAppState |= zTASK_STATE_PRINT;
   pZSubtask->m_pZTask->m_pZSubtaskPrint = pZSubtask;

   ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
   if ( pZSubtaskMain )
      pZSubtask = pZSubtaskMain;

   SetNameForView( pZSubtask->m_vDialog, "__ZDrSubtask", 0, zLEVEL_TASK );
   if ( pZSubtask->m_pZFWnd && mIs_hWnd( pZSubtask->m_pZFWnd->m_hWnd ) )
   {
      pZSubtask->m_pZFWnd->ShowWindow( SW_SHOW );
      pZSubtask->m_pZFWnd->UpdateWindow( );
   }

   if ( lPrintFlags & zREPORT_REVERSE_PARM )
   {
      zPCHAR pchTmp = pchReportInfo;
      pchReportInfo = (zPCHAR) cpcReportDefName;
      cpcReportDefName = pchTmp;
   }

   // This ZPrintout instance will be deleted by the ZSubtask.
   ZPrintout *pPrint = new ZPrintout( pZSubtask, cpcDLL_Name, cpcPrintPageFunction,
                                      lPrintFlags, bBanding, bScale, bPrompt,
                                      cpcReportDefName, pchReportInfo, "" );
#if 0
   DWORD dwMilliSeconds = GetTickCount( );

   while ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) != 0 &&
           pZSubtask->m_pZPrintout != 0 )
   {
      MSG  msg;

      if ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
      {
         ::TranslateMessage( &msg );
         ::DispatchMessage( &msg );
      }

      if ( GetTickCount( ) - dwMilliSeconds > 300000 )
      {
         TraceLineS( "OpenReport - Print time-out", "" );
         break;
      }
   }
#endif

   PutWindowOnTop( pZSubtask->m_vDialog );

   if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 && pZSubtask->m_pZPrintout )
   {
      mDeleteInit( pZSubtask->m_pZPrintout );
      return( FALSE );
   }

   return( TRUE );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function terminates a printer task.  The parameters are:
//    - view to the application's subtask
//    - handle to the print task (returned from OpenReport).
//
// The return value is set based on how the print task ended as follows:
//    - TRUE  ==> normal termination (no print errors)
//    - FALSE ==> print task terminated by error or user abort
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CloseReport( zVIEW vSubtask, zLONG lFlags )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( pZSubtask->m_pZPrintout )
      {
         if ( lFlags & 0x00000001 )  // close report file only
         {
            if ( pZSubtask->m_pZPrintout->m_lFileCSV != -1 )
            {
               if ( pZSubtask->m_pZPrintout->m_bPrintToHTML )
               {
                  SysWriteFile( pZSubtask->m_vDialog, pZSubtask->m_pZPrintout->m_lFileCSV, "<br /></p>\r\n", 12 );
                  SysWriteFile( pZSubtask->m_vDialog, pZSubtask->m_pZPrintout->m_lFileCSV, "</html>\r\n", 9 );
                  TraceLineS( "ZPrintout Closing HTML file: ", pZSubtask->m_pZPrintout->m_csReportFileName );
               }
               else
                  TraceLineS( "ZPrintout Closing CSV file: ", pZSubtask->m_pZPrintout->m_csReportFileName );

               SysCloseFile( pZSubtask->m_vDialog, pZSubtask->m_pZPrintout->m_lFileCSV, 0 );
               pZSubtask->m_pZPrintout->m_lFileCSV = -1;
            }
         }

         if ( (lFlags & 0x00000002) &&  // draw filename to report
              (pZSubtask->m_pZPrintout->m_bPrintToCSV == FALSE &&
               pZSubtask->m_pZPrintout->m_bPrintToHTML == FALSE) )
         {
            TraceLineS( "ZPrintout drawing filename to report: ", pZSubtask->m_pZPrintout->m_csReportFileName );
            CreateFontForReport( pZSubtask->m_vDialog, "bold", 18 * 12, TRUE, FALSE, FALSE, FALSE, 0, 0, 0, "Arial" );
            SelectFontForReport( pZSubtask->m_vDialog, "bold" );
            DrawTextToReport( pZSubtask->m_vDialog, 0, "Report created in file:", 100, 100, 100000, 2000, 0, 0, 0, 0, 0 );
            DrawTextToReport( pZSubtask->m_vDialog, 0, pZSubtask->m_pZPrintout->m_csReportFileName, 1000, 2500, 100000, 10000, 0, 0, 0, 0, 0 );
         }
      }

      if ( (lFlags & 0x00000001) == 0 )
      {
         pZSubtask->m_pZTask->m_pZSubtaskPrint = 0;
         pZSubtask->m_pZTask->m_uAppState &= ~zTASK_STATE_PRINT;
         if ( (pZSubtask->m_pZPrintout->m_bPrintToCSV == FALSE && pZSubtask->m_pZPrintout->m_bPrintToHTML == FALSE) )
         {
            mDeleteInit( pZSubtask->m_pZPrintout );
         }
      }
   }

   return( 0 );
}

#include "math.h"

/////////////////////////////////////////////////////////////////////////////
//
// This function prints text to the page justified with respect to its
// bounding rectangle.
//
// This routine will only print one line of text.
//
// The parameters are:
//    - view to the application's subtask
//    - text string
//    - lLeft256ths              (left start position)
//    - lTop256ths               (top  start position)
//    - lRight256ths             (right  end position)
//    - lBottom256ths            (bottom end position)
//    - lJustify
//    - lBorderStyle       (top/left/bottom/right - width - style)
//    - lBorderColor       rgb color for border
//    - lEscapement        text angle
//    - lType              1 - translate "\n" to newline
//                         2 - fixed height multi-line
//
// lJustify determines the type of justification to be done.
// Currently, the justification codes are as follows:
//    2 - centerc:
//    4 - right
//    All other values default to left justification.
//
// The return value is:
//    0 - text printed (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawTextToReport( zVIEW   vSubtask,
                  zVIEW   vAppOI,
                  zCPCHAR cpcText,
                  zLONG   lLeft256ths,
                  zLONG   lTop256ths,
                  zLONG   lRight256ths,
                  zLONG   lBottom256ths,
                  zLONG   lJustify,
                  zLONG   lBorderStyle,
                  zLONG   lBorderColor,
                  zLONG   lEscapement,
                  zLONG   lType )
{
   ZSubtask *pZSubtask;
   static double pi = 3.14159265358979323846;

// if ( zstrstr( cpcText, "RE558" ) != 0 )
//    TraceLineS( "DrawTextToReport: ", cpcText );

#ifdef DEBUG_ALL
   static nTextCnt = -1;


   if ( zstrstr( cpcText, "9" ) != 0 )
   {
      nTextCnt = 1;
   }
   else
   if ( nTextCnt == 2 )
   {
   // TraceLineS( "PrintTextToReport: ", cpcText );
      nTextCnt = -1;
   }
   else
   if ( nTextCnt >= 1 && nTextCnt < 2 )
      nTextCnt++;
   else
      nTextCnt = -1;

#endif

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      zCHAR    szWorkString[ 65534 ];
      zLONG    lCopyLength;
      UINT     uFormat = 0;
      zBOOL    bPrevSpace = TRUE;
      CFont    *pFontOld = 0;
      COLORREF clrOldText = CLR_INVALID;
      COLORREF clrOldTextBk = CLR_INVALID;
      zLONG    k;

      if ( pZSubtask->m_pZPrintout->m_pFontCurr )
      {
         pFontOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pZSubtask->m_pZPrintout->m_pFontCurr );

         if ( (pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText & 0xff000000) == 0 )
         {
            clrOldText = (COLORREF)
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         }

         if ( (pZSubtask->m_pZPrintout->m_pFontCurr->m_clrTextBk & 0xff000000) == 0 )
         {
            clrOldTextBk = (COLORREF)
            pZSubtask->m_pZPrintout->m_pDC->SetBkColor( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrTextBk );
         }

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet == FALSE )
         {
            pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet = TRUE;
            OUTLINETEXTMETRIC OTM;   // metric data array
            if ( pZSubtask->m_pZPrintout->
                  m_pDC->GetOutlineTextMetrics( sizeof( OUTLINETEXTMETRIC ), &OTM ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_Panose = OTM.otmPanoseNumber;
            }

            LOGFONT lf;
            if ( pZSubtask->m_pZPrintout->m_pFontCurr->GetLogFont( &lf ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byCharset = lf.lfCharSet;
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byPitchFamily = lf.lfPitchAndFamily;
            }
         }
      }

      // Load up the work string.
      lCopyLength = zstrlen( cpcText ) + 1;
      if ( lCopyLength > zsizeof( szWorkString ) - 2 )
      {
         lCopyLength = zsizeof( szWorkString ) - 2;
         zmemcpy( szWorkString, cpcText, lCopyLength );
         szWorkString[ lCopyLength ] = 0;
      }
      else
      {
         strcpy_s( szWorkString, zsizeof( szWorkString ), cpcText );
      // if ( zstrcmp( szWorkString, cpcText ) )
      //    TraceLineS( "Huh?", "" );
      }

      // If "static" text, check for newline ("\n") ... translate if found.
      if ( lType & 0x00000002 )
      {
         zPCHAR pch = szWorkString;
         zPCHAR pchText;

         while ( (pchText = zstrchr( pch, '\\' )) != 0 )
         {
            if ( *(pchText + 1) == '\\' )
            {
               // Convert to one backslash.
               strcpy_s( pchText, zsizeof( szWorkString ) - (pchText - szWorkString), pchText + 1 );
               pchText--;  // back up so skip only one
            }
            else
            if ( *(pchText + 1) == 'n' )
            {
               *pchText = '\r';
               *(pchText + 1) = '\n';
            }

            pch = pchText + 2;
         }

         uFormat |= DT_WORDBREAK | DT_NOPREFIX | DT_EXPANDTABS;
      }
      else
      {
         if ( lJustify & 512 )
            uFormat |= DT_WORDBREAK | DT_NOPREFIX;
         else
            uFormat |= DT_SINGLELINE | DT_NOPREFIX; // force single line printing

         // Look for first newline character and replace it with null terminator.
         // Also count the characters and lines.
         k = 0;
         while ( szWorkString[ k ] != '\n' && szWorkString[ k ] != '\r' && szWorkString[ k ] != 0 )
         {
            k++;
            pZSubtask->m_pZPrintout->m_lCharCnt++;
            if ( zisspace( szWorkString[ k ] ) )
               bPrevSpace = TRUE;
            else
            {
               if ( bPrevSpace == TRUE )
               {
                  pZSubtask->m_pZPrintout->m_lWordCnt++;
                  bPrevSpace = FALSE;
               }
            }
         }

         if ( szWorkString[ k ] == '\n' || szWorkString[ k ] == '\r' )
            szWorkString[ k ] = 0;
      }

      // Note that MM_HIENGLISH has the origin at the bottom left ...
      // vs. MM_TEXT which has the origin at the top left.  The DPtoLP
      // call does appropriate conversions.
      // Determine coordinates in pixels using longs to avoid round off error.
      zLONG lLeft = (lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lTop = (lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      zLONG lRight = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lBottom = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      CSize sizeExtent;
      CRect rect( lLeft, lTop, lRight, lBottom );
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );

      // Determine formatting.  Default to left justification.
      // #define DT_TOP              0x00000000
      // #define DT_LEFT             0x00000000
      // #define DT_CENTER           0x00000001
      // #define DT_RIGHT            0x00000002
      // #define DT_VCENTER          0x00000004
      // #define DT_BOTTOM           0x00000008
      // #define DT_WORDBREAK        0x00000010
      // #define DT_SINGLELINE       0x00000020
      // #define DT_EXPANDTABS       0x00000040
      // #define DT_TABSTOP          0x00000080
      // #define DT_NOCLIP           0x00000100
      // #define DT_EXTERNALLEADING  0x00000200
      // #define DT_CALCRECT         0x00000400
      // #define DT_NOPREFIX         0x00000800
      // #define DT_INTERNAL         0x00001000
      // #define DT_EDITCONTROL      0x00002000
      // #define DT_PATH_ELLIPSIS    0x00004000
      // #define DT_END_ELLIPSIS     0x00008000
      // #define DT_MODIFYSTRING     0x00010000
      // #define DT_RTLREADING       0x00020000
      // #define DT_WORD_ELLIPSIS    0x00040000

      if ( lJustify & 2 )
      {
         uFormat |= DT_CENTER;
      }
      else
      if ( lJustify & 4 )
      {
         uFormat |= DT_RIGHT;
         if ( lJustify & 64 )    // offset text
         {
            sizeExtent = pZSubtask->m_pZPrintout->m_pDC->GetTextExtent( " ", 1 );
            rect.right -= sizeExtent.cx;
         }
      }
      else
      {
         uFormat |= DT_LEFT;
         if ( lJustify & 64 )    // offset text
         {
            sizeExtent = pZSubtask->m_pZPrintout->m_pDC->GetTextExtent( " ", 1 );
            rect.left += sizeExtent.cx;
         }
      }

      if ( lJustify & 256 )
         uFormat |= DT_VCENTER;
      else
      if ( lJustify & 512 )
         uFormat |= DT_BOTTOM;

      lLeft = rect.left;
      lTop = rect.top;
      lRight = rect.right;
      lBottom = rect.bottom;

      while ( lEscapement < 0 )
         lEscapement += 3600;

      while ( lEscapement > 3600 )
         lEscapement -= 3600;

   // TraceLineI( "Escapement: ", lEscapement );
      if ( lEscapement )
      {
         if ( lEscapement == 3600 )
            lEscapement = 0;

         // Force left-justification and single-line printing.
      // uFormat = DT_LEFT | DT_SINGLELINE | DT_NOCLIP;
         uFormat = DT_CENTER | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE | DT_NOPREFIX; // force single line printing

         sizeExtent = pZSubtask->m_pZPrintout->m_pDC->GetTextExtent( szWorkString, // in logical units
                                                                     zstrlen( szWorkString ) );
      // TraceSize( "TextExtent: ", sizeExtent );

         CSize  sizeAdjustedExtent;
         double dEscapeAngle = (pi * lEscapement) / 1800; // angle in radians
         double dSin;
         double dCos;
         double dSinAbs;
         double dCosAbs;
         double dAdjustHeight;
         double dAdjustWidth;

         dSin = sin( dEscapeAngle );
         dCos = cos( dEscapeAngle );
         dSinAbs = (dSin >= 0) ? dSin : -dSin;
         dCosAbs = (dCos >= 0) ? dCos : -dCos;

         sizeAdjustedExtent.cx = ((rect.right - rect.left) / 2) + sizeExtent.cx / 2;
         sizeAdjustedExtent.cy = ((rect.top - rect.bottom) / 2) - sizeExtent.cy / 2;
         rect.left += sizeAdjustedExtent.cx;
         rect.right += sizeAdjustedExtent.cx;
         rect.top += sizeAdjustedExtent.cy;
         rect.bottom += sizeAdjustedExtent.cy;

         sizeAdjustedExtent.cx = (rect.right - rect.left) / 2;
         rect.left -= sizeAdjustedExtent.cx;
         rect.right -= sizeAdjustedExtent.cx;
         sizeAdjustedExtent.cy = (rect.top - rect.bottom) / 2;
         rect.top -= sizeAdjustedExtent.cy;
         rect.bottom -= sizeAdjustedExtent.cy;

         if ( dSinAbs < 0.001 || dCosAbs < 0.001 )    // On an axis
         {
            dAdjustHeight = (((dSin - dCos) * sizeExtent.cy * dCosAbs) -
                             ((dSin + dCos) * sizeExtent.cx * dSinAbs)) / 2;
            dAdjustWidth  = (((dSin - dCos) * sizeExtent.cy * dSinAbs) -
                             ((dSin + dCos) * sizeExtent.cx * dCosAbs)) / 2;
            rect.left += (zLONG) dAdjustWidth;
            rect.right += (zLONG) dAdjustWidth;
            rect.top -= (zLONG) dAdjustHeight;
            rect.bottom -= (zLONG) dAdjustHeight;
         }
         else
         if ( dSin > 0 && dCos > 0 )   // Quadrant I
         {
            dAdjustHeight = (((dSin - dCos) * sizeExtent.cy * dCosAbs) -
                             ((dSin + dCos) * sizeExtent.cx * dSinAbs)) / 2;
            dAdjustWidth  = (-((dSin + dCos) * sizeExtent.cy * dCosAbs) +
                             ((dSin - dCos) * sizeExtent.cx * dSinAbs)) / 2;
            rect.left += (zLONG) dAdjustWidth;
            rect.right += (zLONG) dAdjustWidth;
            rect.top -= (zLONG) dAdjustHeight;
            rect.bottom -= (zLONG) dAdjustHeight;
         }
         else
         if ( dSin < 0 && dCos < 0 )   // Quadrant III
         {
            dAdjustHeight = (((dSin - dCos) * sizeExtent.cy * dCosAbs) -
                             ((dSin + dCos) * sizeExtent.cx * dSinAbs)) / 2;
            dAdjustWidth  = (((dSin - dCos) * sizeExtent.cy * dSinAbs) -
                             ((-dSin + dCos) * sizeExtent.cx * dCosAbs)) / 2;
            rect.left += (zLONG) dAdjustWidth;
            rect.right += (zLONG) dAdjustWidth;
            rect.top -= (zLONG) dAdjustHeight;
            rect.bottom -= (zLONG) dAdjustHeight;
         }
         else
         if ( dSin > 0 && dCos < 0 )  // Quadrant II
         {
            dAdjustHeight = (((dSin - dCos) * sizeExtent.cy * dCosAbs) +
                             ((-dSin + dCos) * sizeExtent.cx * dSinAbs)) / 2;
            dAdjustWidth  = (-((dSin + dCos) * sizeExtent.cy * dCosAbs) +
                             ((dSin - dCos) * sizeExtent.cx * dSinAbs)) / 2;
            rect.left += (zLONG) dAdjustWidth;
            rect.right += (zLONG) dAdjustWidth;
            rect.top -= (zLONG) dAdjustHeight;
            rect.bottom -= (zLONG) dAdjustHeight;
         }
         else
      // if ( dSin < 0 && dCos > 0 )  // Quadrant IV
         {
            dAdjustHeight = (((dSin - dCos) * sizeExtent.cy * dCosAbs) +
                             ((-dSin + dCos) * sizeExtent.cx * dSinAbs)) / 2;
            dAdjustWidth  = (((dSin - dCos) * sizeExtent.cy * dSinAbs) -
                             ((-dSin + dCos) * sizeExtent.cx * dCosAbs)) / 2;
            rect.left += (zLONG) dAdjustWidth;
            rect.right += (zLONG) dAdjustWidth;
            rect.top -= (zLONG) dAdjustHeight;
            rect.bottom -= (zLONG) dAdjustHeight;
         }
      }

      // sizeAdjustedExtent.cx =
      //                (zLONG) ((sizeExtent.cx + sizeExtent.cy) * dCos) / 2;
      // rect.left -= sizeAdjustedExtent.cx;
      // rect.right -= sizeAdjustedExtent.cx;
      // sizeAdjustedExtent.cy =
      //                (zLONG) ((sizeExtent.cx + sizeExtent.cy) * dSin) / 2;
      // rect.top += sizeAdjustedExtent.cy;
      // rect.bottom += sizeAdjustedExtent.cy;


      // Print the text.
      if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
      {
         int nOldMode = pZSubtask->m_pZPrintout->m_pDC->SetBkMode( TRANSPARENT );
         lCopyLength = zstrlen( szWorkString );
         pZSubtask->m_pZPrintout->m_pDC->DrawText( szWorkString, lCopyLength, rect, uFormat );
         pZSubtask->m_pZPrintout->m_pDC->SetBkMode( nOldMode );
      }

      if ( pFontOld )
      {
         if ( clrOldText != CLR_INVALID )
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( clrOldText );

         if ( clrOldTextBk != CLR_INVALID )
            pZSubtask->m_pZPrintout->m_pDC->SetBkColor( clrOldTextBk );

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pFontOld );
      }

      if ( lBorderStyle & 0x00000F00 )
      {
         zSHORT nPenWidth = (zSHORT) (lBorderStyle >> 24);
         zLONG  lLinePosX;
         zLONG  lLinePosY;
         zLONG  lLineEndX;
         zLONG  lLineEndY;
         zSHORT nPenType = (zSHORT) (lBorderStyle & 0x000000FF);
         zLONG  lPos = 1;

         lBorderStyle >>= 8;
         for ( k = 0; k < 4; k++ )
         {
            if ( lBorderStyle & (lPos << k) )
            {
               if ( k == 0 )  // top
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lTop256ths;
               }
               else
               if ( k == 1 )  // left
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lLeft256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lBottom256ths;
               }
               else
               if ( k == 2 )  // bottom
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lBottom256ths;
                  lLineEndY = lBottom256ths;
               }
               else
            // if ( k == 4 )  // right
               {
                  lLinePosX = lRight256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lBottom256ths;
               }

               DrawLineToReport( vSubtask, vAppOI, lLinePosX, lLinePosY,
                                 lLineEndX, lLineEndY, nPenWidth, nPenType, lBorderColor, k % 2 );
            }
         }
      }

      return( 0 );
   }

   return( -1 );
}

#if 0
zBOOL
fnExpandTabs( zPCHAR pchReturnString, zCPCHAR cpcString,
              zLONG  lMaxReturnLth, zSHORT nTabSpaces )
{
   ASSERT( nTabSpaces > 0 );

   zBOOL  bChange = FALSE;
   zPCHAR pchTab;
   zPCHAR pchTemp;
   zLONG  lOffset;
   zLONG  k;

   strcpy_s( pchReturnString, lMaxReturnLth, cpcString );
   pchTab = zstrchr( pchReturnString, '\t' );
   if ( pchTab )
   {
      bChange = TRUE;
      pchTemp = zstrchr( cpcString, '\t' );
      while ( pchTab )
      {
         lOffset = pchTab - pchReturnString;
         k = nTabSpaces - (lOffset % nTabSpaces);
         while ( k-- )
         {
            if ( lOffset < lMaxReturnLth )
               pchReturnString[ lOffset++ ] = ' ';
            else
            {
               // Not enough room ... return original string.
               strcpy_s( pchReturnString, lMaxReturnLth, cpcString );
               return( FALSE );
            }
         }

         pchTemp++;
         strcpy_s( pchReturnString + lOffset, lMaxReturnLth - lOffset, pchTemp );
         pchTemp = zstrchr( pchTemp, '\t' );
         pchTab = zstrchr( pchReturnString, '\t' );
      }
   }

   return( bChange );
}
#endif

/////////////////////////////////////////////////////////////////////////////
//
// This function prints multi-line text to the report inside of a
// bounding rectangle. Since the number of lines in multi-line text
// is not known before hand, the bottom of the bounding rect is
// the maximum amount of room on the page that the text is allowed
// to occupy. Typically, this is the bottom of the page minus the
// height of the page footer -should it exist-. If the entire bounding
// rect is not used, the newly calculated actual bottom of the bounding
// rect is passed back.
//
// New line characters in the input string will generate new lines
// on the report.
//
// If a single line in the input string is too long to fit onto one
// line of the report - as defined by the bounding rect - then a new
// line will be started at the beginning of the next 'word'.
//
// If a single 'word' will not fit on one line, it will be clipped to
// the width of the bounding rect and the next line will begin at the
// beginning of the next 'word'.
//
// If the number of lines required to print the entire input string exceeds
// the size of the bounding rect, the string will be printed up to, but
// NOT including the first 'word' that does not fit and the offset to the
// beginning of the first un-printed 'word' will be returned.
//
// The parameters are:
//    - view to the application's subtask
//    - text string
//    - lLeft256ths              (left start position)
//    - lTop256ths               (top  start position)
//    - lRight256ths             (right  end position)
//    - lBottom256ths            (bottom end position)
//    - lJustify
//    - lBorderStyle       (top/left/bottom/right - width - style)
//    - lBorderColor       rgb color for border
//    - lEscapement        text angle
//    - lType              1 - translate "\n" to newline
//    - plNewBottom256ths - returned, new bottom position of bounding rect
//      on page given in 256ths of an inch.
//    - plNextWord - returned, offset in text string of 1st unprinted
//      character.
//
// lJustify determines the type of justification to be done.
// Currently, the justification codes are as follows:
//    0 - left
//    1 - right
//    All other values default to left justification.
//
// The return value is:
//    0 - text printed (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
// The return value is:
//    1 - Text printed but not all of it fit into the bounding rect.
//        plNextWord returns the offset into the input string to the
//        first un-printed, non-space, printable character.
//    0 - Text printed entirely within the bounding rect.
//        plNewBottom256ths returns the actual bottom of the bounding rect
//        that was used.
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawMultiLineTextToReport( zVIEW   vSubtask,
                           zVIEW   vAppOI,
                           zCPCHAR cpcText,
                           zLONG   lMaxTextLth,
                           zLONG   lLeft256ths,
                           zLONG   lTop256ths,
                           zLONG   lRight256ths,
                           zLONG   lBottom256ths,
                           zLONG   lJustify,
                           zLONG   lBorderStyle,
                           zLONG   lBorderColor,
                           zLONG   lEscapement,
                           zLONG   lType,
                           zPLONG  plNewBottom256ths,
                           zPLONG  plNextWord )
{
   ZSubtask *pZSubtask;

#ifdef DEBUG_ALL
   if ( zstrstr( cpcText, "w/Billy" ) )
      TraceLineS( "", "" );
#endif

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      CFont    *pFontOld = 0;
      COLORREF clrOldText = CLR_INVALID;

      if ( pZSubtask->m_pZPrintout->m_pFontCurr )
      {
         pFontOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pZSubtask->m_pZPrintout->m_pFontCurr );

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != CLR_INVALID )
         {
            clrOldText = (COLORREF)
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         }

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet == FALSE )
         {
            pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet = TRUE;
            OUTLINETEXTMETRIC OTM;   // metric data array
            if ( pZSubtask->m_pZPrintout->
                  m_pDC->GetOutlineTextMetrics( sizeof( OUTLINETEXTMETRIC ), &OTM ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_Panose = OTM.otmPanoseNumber;
            }

            LOGFONT lf;
            if ( pZSubtask->m_pZPrintout->m_pFontCurr->GetLogFont( &lf ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byCharset = lf.lfCharSet;
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byPitchFamily = lf.lfPitchAndFamily;
            }
         }
      }

      // Determine coordinates in pixels using longs to avoid round off error.
      zLONG lLeft = (lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lTop = (lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      zLONG lRight = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lBottom = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      // Check to see if we need to crop the right side of the
      // bounding rect in order to guarantee that we fit onto the page.
      if ( lRight > pZSubtask->m_pZPrintout->m_lHorzRes )
         lRight = pZSubtask->m_pZPrintout->m_lHorzRes;

      // Determine line height.
   // TEXTMETRIC tm;
      CSize size;

   // pZSubtask->m_pZPrintout->m_pDC->GetTextMetrics( &tm );
   // size.cx = 0; size.cy = tm.tmHeight + tm.tmExternalLeading;
   // pZSubtask->m_pZPrintout->m_pDC->LPtoDP( &size );

      GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC, "AjTy", 4, &size );
   // size = pZSubtask->m_pZPrintout->m_pDC->GetTextExtent( "AjTy", 4 );
   // size.cx = 0; size.cy += tm.tmDescent;

      zLONG lCurrentY = lTop;

      pZSubtask->m_pZPrintout->m_pDC->LPtoDP( &size );

      zLONG lLineHeight = size.cy;
      zLONG lLine256ths = ((lLineHeight * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchX);
   // zSHORT nLineHeight = (zSHORT) ((size.cy * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchX);
      lTop256ths = -32000;

      // While there is room for a new line to be printed, print a line.
      // The algorithm is as follows:
      //
      // While there is text remaining to be printed
      //  - Determine the text to be printed given extent constraints.
      //  - Print the text for the current line.
      //  - Advance to the next line if there is room.

#if 0
      // If the input string is empty, return.
      if ( cpcText[ 0 ] == 0 )
      {
         // Print nothing and return.  Advance Y position by the line
         // height to indicate a printed line even though the line is
         // empty.  Convert ending Y position back into 256ths of an inch.
         *plNewBottom256ths += (lLineHeight * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchY;
         *plNextWord = 0;
         if ( pFontOld )
         {
            if ( clrOldText != CLR_INVALID )
               pZSubtask->m_pZPrintout->m_pDC->SetTextColor( clrOldText );

            pZSubtask->m_pZPrintout->m_pDC->SelectObject( pFontOld );
         }

         return( 0 );
      }
#endif

      zCHAR  szPrint[ 65534 ];
      zCHAR  szPrint2[ 65534 ];
      zCHAR  szWork[ 32766 ];
      zPCHAR pchText;
      zLONG  lTabStopExtent;
      zLONG  lLth2 = 0;
      zBOOL  bDone = FALSE;        // loop control
      zBOOL  bLineFull;            // loop control
      zBOOL  bTab;
      zLONG  lStart = *plNextWord; // ==> start of next line
      zLONG  k = lStart;           // ==> start of next unprocessed word
      zLONG  j = lStart;           // ==> end of last word processed
      zBOOL  bPrevSpace = TRUE;
      zSHORT nRC = 0;

      // If "static" text, check for newline ("\n") ... translate if found.
      if ( (lType & 0x00000001) &&
           lMaxTextLth < zsizeof( szPrint ) - 1 )
      {
         zPCHAR pch = szPrint;

         strcpy_s( szPrint, zsizeof( szPrint ), cpcText );
         while ( (pchText = zstrchr( pch, '\\' )) != 0 )
         {
            if ( *(pchText + 1) == 'n' )
            {
               *pchText = '\r';
               *(pchText + 1) = '\n';
            }

            pch = pchText + 2;
         }

         cpcText = szPrint;
      }

   // if ( zstrchr( cpcText + lStart, '\t' ) != 0 )  // always get extent
   // {
         GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC,
                               "                                ",  // 32
                            // "12345678911234567892123456789312",  // blanks
                               pZSubtask->m_pZPrintout->m_nTabStops, &size );
         lTabStopExtent = size.cx * 2;
   // }
   // else
   //    lTabStopExtent = -1;

      pchText = 0;   // initialize for allocation test
      if ( lMaxTextLth >= zsizeof( szWork ) )
      {
         DrAllocTaskMemory( &pchText, lMaxTextLth + 1 );
         strncpy_s( pchText, lMaxTextLth + 1, cpcText, lMaxTextLth );
         cpcText = pchText;
      }
      else
      {
         strncpy_s( szWork, zsizeof( szWork ), cpcText, lMaxTextLth );
         szWork[ lMaxTextLth ] = 0;
         cpcText = szWork;
      }

      while ( bDone == FALSE )
      {
         // Extract the next line to be printed.
         bLineFull = FALSE;  // loop control
         CRect rect( lLeft, lCurrentY, lRight, lCurrentY + lLineHeight );
         pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );

         // We are turning these values to LP values (not 256ths).
         if ( lTop256ths == -32000 )
         {
            lTop256ths = rect.top;
            lLeft256ths = rect.left;
            lRight256ths = rect.right;
         }

         lBottom256ths = rect.bottom;

         if ( lJustify & 64 )  // offset requested
         {
            if ( lJustify & 4 )
               rect.right -= 4;
            else
            if ( lJustify & 2 )
               rect.left += 4;
         }

         while ( TRUE )  // begin purist's goto
         {
            while ( bLineFull == FALSE )
            {
               // Look ahead for end of next word.
               szPrint[ 0 ] = 0;
               bTab = FALSE;
               pZSubtask->m_pZPrintout->m_lCharCnt++;
               while ( cpcText[ k ] != ' '  &&
                       cpcText[ k ] != '\n' &&
                       cpcText[ k ] != '\r' &&
                       cpcText[ k ] != '\t' &&
                       cpcText[ k ] != 0 )
               {
                  k++;
                  if ( zisspace( cpcText[ k ] ) )
                     bPrevSpace = TRUE;
                  else
                  if ( bPrevSpace == TRUE )
                  {
                     pZSubtask->m_pZPrintout->m_lWordCnt++;
                     bPrevSpace = FALSE;
                  }
               }

            // if ( cpcText[ k ] == '\t' )
            //    bTab = TRUE;

               // If we have an empty line with nothing other than
               // a '\n' or 0 or tab or blank.
               if ( k == j && cpcText[ k ] != '\t' && cpcText[ k ] != ' ' )
               {
                  // If we hit the end of text
                  if ( cpcText[ k ] == 0 )
                  {
                     // We will print nothing and return.
                     // Convert ending Y position back into 256ths of an inch.
                     *plNewBottom256ths = ((zLONG) lCurrentY * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchY;
                     *plNextWord = 0;
                     goto CHECK_BORDER_label;
                  }
                  else  // Otherwise, we hit a lone '\n'
                  {
//BL why?            szPrint[ 0 ] = cpcText[ k ]; // used as a flag later
                     bLineFull = TRUE;

                     // We assume that line feeds and carriage returns
                     // appear in pairs -- but we're not that sure --
                     // so, if the next character is one or the other,
                     // we will skip past it.
                     if ( (cpcText[ k + 1 ] == '\n' && cpcText[ k ] == '\r') ||
                          (cpcText[ k + 1 ] == '\r' || cpcText[ k ] == '\n') )
                     {
                        k++;
                     }

                     // BL do not remove leading blanks.
                  // do
                  // {
                        k++;
                  // } while ( cpcText[ k ] == ' ' );

                     lStart = k;
                     j = k;
                  }

                  size.cx = 0;
               }
               else  // the line is not empty and we found a real 'word'
                     // k != j or tab or blank
               {
                  if ( k - lStart >= zsizeof( szPrint ) )
                  {
                     TraceLineI( "Cannot handle extremely long multi-line text length: ", k - lStart );
                     break;
                  }

                  // Check length of current line.
                  strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, k - lStart );
                  szPrint[ k - lStart ] = 0;
                  GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC, szPrint, k - lStart, &size );
                  if ( cpcText[ k ] == '\t' )
                  {
                     k--;
                     if ( k < lStart )
                        lStart = k;

                     bTab = TRUE;
                  }

                  // If the line won't fit.
               // pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &size );
                  if ( size.cx > rect.right - rect.left )
                  {
                     // If the line only has one word in it.
                     if ( lStart == j && rect.left == lLeft )
                     {
                        // szPrint already holds what we want.  We will print
                        // the line even though it is too long and will let
                        // it get cropped.
                        bLineFull = TRUE;

                        // BL do not remove leading blanks.
                     // do
                     // {
                           k++;
                     // } while ( cpcText[ k ] == ' ' );

                        lStart = k;
                        j = k;
                     }
                     else  // the line has more than one word in it
                     {
#if 0
                        // Load up the line to be printed.
                        strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, j - lStart + 1 );
                        szPrint[ j - lStart + 1 ] = 0;
                        bLineFull = TRUE;

                        // Look for beginning of next line.
                        do
                        {
                           j++;
                        } while ( cpcText[ j ] == ' ' );

                        lStart = j;
                        k = j;
#else
                        //BL wenn Kopier-Start-Pos!= Pos. des vorherigen
                        // Wortes ( j ) ist
                        // If the start position of the copy area does not
                        // match the position of the previous word (j).
                        if ( lStart != j )
                        {
                           j++;
                           while ( cpcText[ j ] == ' ')
                             j++;

                           strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, j - lStart );
                           szPrint[ j - lStart ] = 0;
                           lStart = j;
                           k = j;
                        }
                        else
                        {
                           // BL Wenn Wort bezüglich seiner Länge nicht
                           // darstellbar ist, dann hier ein neues Verfahren
                           // ansetzen.  Bisher wurden jeweils ein Zeichen
                           // von vorm in jeweils einer neuen Zeile gedruckt,
                           // bis das Wort nach diesem Verfahren eine Länge
                           // erreicht hat, so daß es in eine Zeile paßt.
                           // Das neus Verfahren schreibt von diesem Wort so
                           // viele Zeichen in eine Zeile, wie darstellbar
                           // sind. Die verbleibenden Zeichen, werden in
                           // Folgezeilen gedruckt.
                           // If a word does not fit because of its length,
                           // we have to use a different algorithm. The old
                           // algorithm printed character by character from
                           // the left into a new line until the text
                           // has a length that fits into a line.  The new
                           // algorithm prints as many characters to a line
                           // as will fit. The remaining characters of a
                           // word are then printed to the next line(s).
                           strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, j - lStart + 1 );
                           szPrint[ j - lStart + 1 ] = 0;
                           GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC,
                                                 szPrint, j - lStart + 1, &size );
                           lStart = j;
                           k = j;
                           while ( cpcText[ k ] != ' '  &&
                                   cpcText[ k ] != '\n' &&
                                   cpcText[ k ] != '\r' &&
                                   cpcText[ k ] != '\t' &&
                                   cpcText[ k ] != 0 &&
                                   size.cx < rect.right - rect.left )
                           {
                              k++;
                              strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, k - lStart );
                              szPrint[ k - lStart ] = 0;
                              GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC, szPrint, k - lStart, &size );
                           }

                           if ( k != lStart )
                           {
                              k--;
                              strncpy_s( szPrint, zsizeof( szPrint ), cpcText + lStart, k - lStart );
                              szPrint[ k - lStart ] = 0;
                              lStart = k;
                              j = k;
                           }
                           else
                              k++;
                        }

                        bLineFull = TRUE;
#endif
                     }
                  }
                  else     // else, the line will fit
                  {
                     if ( bTab )
                     {
                        break;
                     }
                     else
                     if ( cpcText[ k ] == 0 ) // if at end of text and it fits
                     {
                        // szPrint already has the right thing in it.
                        // We will print this last line and be done.
                        bDone = TRUE;
                        bLineFull = TRUE;
                     }
                     else
                     if ( cpcText[ k ] == '\n' || cpcText[ k ] == '\r' )
                     {
                        // If we found a newline character, we need to print
                        // the line as is.
                        // We assume that line feeds and carriage returns
                        // appear in pairs -- but we're not that sure --
                        // so, if the next character is one or the other,
                        // we will skip past it.
                        if ( cpcText[ k + 1 ] == '\n' ||
                             cpcText[ k + 1 ] == '\r' )
                        {
                           k++;
                        }

                        // Again, szPrint has the right thing in it from
                        // when we did the GetTextExtent above.
                        bLineFull = TRUE;

                        // BL do not remove leading blanks.
                     // do
                     // {
                           k++;
                     // } while ( cpcText[ k ] == ' ' );

                        lStart = k;
                        j = k;
                     }
                     else
                     {
                        // Otherwise, we must have found a space and so we
                        // need to look for another word.

                        // Mark the end of this word.
                        j = k - 1;

                        // BL do not remove leading blanks.
                     // do
                     // {
                           k++;
                     // } while ( cpcText[ k ] == ' ' );
                     }
                  }
               } // end else k != j or tab or blank
            } //end while ( bLineFull == FALSE )

            // Print the line if it is not an empty line.
            if ( szPrint[ 0 ] != '\n' && szPrint[ 0 ] != '\r' )
            {
               UINT uFormat;

               if ( lJustify & 2 )
                  uFormat = DT_CENTER;
               else
               if ( lJustify & 4 )
                  uFormat = DT_RIGHT;
               else
                  uFormat = DT_LEFT;

               uFormat |= DT_WORDBREAK | DT_NOPREFIX | DT_EXPANDTABS;

               // Note:  MM_HIENGLISH has the origin at the bottom left ...
               // vs. MM_TEXT which has the origin at the top left.
               // Build the printing rect.

               if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
               {
#ifdef DEBUG_ALL
                  zPCHAR pchTemp = new char[ zstrlen( szPrint ) + 256 ];
                  sprintf( pchTemp,
                           "PrintMultiLineTextToReport: "
                              "'%s' y-Extent: %ld  Rect:",
                           szPrint, size.cy );
                  TraceRect( pchTemp, rect );
#endif
                  // BL do not print a empty string
                  if ( szPrint[ 0 ] )
                     pZSubtask->m_pZPrintout->m_pDC->DrawText( szPrint, zstrlen( szPrint ), rect, uFormat );
#ifdef DEBUG_ALL
                  delete [] pchTemp;
#endif
               }

               if ( bLineFull )
                  break;

               if ( bTab )   // there is a tab ... adjust rect.left
               {
                  k += 2;
                  lStart = k;
                  j = k;

                  // If we are beyond 31/32 of a TabStop, we need to complete
                  // the current TabStop and then add a full TabStop.
                  zLONG lMod = size.cx % lTabStopExtent;
                  if ( lMod > lTabStopExtent * 63 / 64 )
                     size.cx += lTabStopExtent;

                  size.cx += lTabStopExtent - lMod;
                  rect.left += size.cx;

                  // Advance beyond white space.
                  while ( cpcText[ k ] == ' ' )
                     k++;

                  // BL set new Copy-Start- Pos.
                  lStart = k;
                  continue;  // the one case we want to continue the loop
               }

               if ( lLth2 )
                  szPrint2[ lLth2++ ] = '\n';

               strcpy_s( szPrint2 + lLth2, zsizeof( szPrint2 ) - lLth2, szPrint );
               lLth2 = zstrlen( szPrint2 );
            }

            break;

         }  // end of purist's goto

         // Advance to next line if needed.
         // If the bDone flag is set here, it is because we found the
         // end of the cpcText and the line just printed fit.
         *plNewBottom256ths = ((zLONG) (lCurrentY + lLineHeight) * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchY;
         if ( bDone )
         {
            *plNextWord = 0;  // meaningless
            goto CHECK_BORDER_label;
         }
         else // otw, we are not done ... advance to the next line if possible.
         {
            // If the next line won't fit
            lCurrentY += lLineHeight;
            if ( lCurrentY + lLineHeight > lBottom ) //PAS 2001-06-01 added lLineHeight
            {
               *plNextWord = k;     // start of next word
               nRC = 1;
               goto CHECK_BORDER_label;
            }
         }
      }

CHECK_BORDER_label:

      if ( pchText )
         DrFreeTaskMemory( pchText );

      if ( pFontOld )
      {
         if ( clrOldText != CLR_INVALID )
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( clrOldText );

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pFontOld );
      }

      if ( (lBorderStyle & 0x00000F00) && (lTop256ths != -32000) )
      {
         zSHORT nPenWidth = (zSHORT) (lBorderStyle >> 24);
         zLONG  lLinePosX;
         zLONG  lLinePosY;
         zLONG  lLineEndX;
         zLONG  lLineEndY;
         zSHORT nPenType = (zSHORT) (lBorderStyle & 0x000000FF);
         zLONG  lPos = 1;

         if ( nPenWidth == 0 )
            nPenWidth = 1;

         lLeft256ths -= (nPenWidth + 11);
         lRight256ths++;

         lBorderStyle >>= 8;
         for ( k = 0; k < 4; k++ )
         {
            if ( lBorderStyle & (lPos << k) )
            {
               if ( k == 0 )  // top
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lTop256ths;
               }
               else
               if ( k == 1 )  // left
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lLeft256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lBottom256ths;
               }
               else
               if ( k == 2 )  // bottom
               {
                  lLinePosX = lLeft256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lBottom256ths;
                  lLineEndY = lBottom256ths;
               }
               else
            // if ( k == 4 )  // right
               {
                  lLinePosX = lRight256ths;
                  lLineEndX = lRight256ths;
                  lLinePosY = lTop256ths;
                  lLineEndY = lBottom256ths;
               }

               switch ( nPenType )
               {
                  case 1:
                    nPenType = PS_DASH;
                    break;

                  case 2:
                    nPenType = PS_DOT;
                    break;

                  case 3:
                    nPenType = PS_DASHDOT;
                    break;

                  case 4:
                    nPenType = PS_DASHDOTDOT;
                    break;

                  default:
                    nPenType = PS_SOLID;
                    break;
               }

               if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
               {
                  CSize size( nPenWidth, nPenWidth );
                  if ( nPenType == PS_SOLID )
                  {
                     pZSubtask->m_pZPrintout->m_pDC->DPtoHIMETRIC( &size );
                  // size.cy *= 2;
                  }

                  CPen *pPen = new CPen( nPenType, (zSHORT) size.cy, lBorderColor );
                  CPen *pPenOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPen );
                  CBrush *pBrushOld = (CBrush *) pZSubtask->m_pZPrintout->m_pDC->SelectStockObject( HOLLOW_BRUSH );
                  pZSubtask->m_pZPrintout->m_pDC->MoveTo( lLinePosX, lLinePosY );
                  pZSubtask->m_pZPrintout->m_pDC->LineTo( lLineEndX, lLineEndY );
                  pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPenOld );
                  pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrushOld );
                  delete( pPen );
               }
            }
         }
      }

      return( nRC );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function draws a line to the device.  The parameters are:
//    - view to the application's subtask
//    - nPosX (left start position)
//    - nPosY (top start position)
//    - nEndX (right end position)
//    - nEndY (bottom end position)
//    - nPenWidth (pen width used to draw the rectangles box ...
//      0 ==> use default pen width)
//    - nPenType : 0 - SOLID ... 1 - DASH ... 2 - DOT ...
//                 3 DASH_DOT ... 4 - DASH_DOT_DOT
//    - lPenColor - RGB pen color
//    - nLineStyle : 0 - Horizontal ... 1 - Vertical ...
//                   2 - DiagonalDescending ... 3 - Diagonal Ascending
//
// The return value is:
//    0 - line drawn (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawLineToReport( zVIEW   vSubtask,
                  zVIEW   vAppOI,
                  zLONG   lPosX,
                  zLONG   lPosY,
                  zLONG   lEndX,
                  zLONG   lEndY,
                  zLONG   lPenWidth,
                  zLONG   lPenType,
                  zLONG   lPenColor,
                  zSHORT  nLineStyle )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      if ( lPenWidth == 0 )
         lPenWidth = 1;

      switch ( lPenType )
      {
         case 1:
           lPenType = PS_DASH;
           break;

         case 2:
           lPenType = PS_DOT;
           break;

         case 3:
           lPenType = PS_DASHDOT;
           break;

         case 4:
           lPenType = PS_DASHDOTDOT;
           break;

         default:
           lPenType = PS_SOLID;
           break;
      }

      switch ( nLineStyle )
      {
         case 1:  // vertical
            lPosX = (lPosX + lEndX) / 2;
            lEndX = lPosX;
            break;

         case 2:  // diagonal descending
            break;

         case 3:  // diagonal ascending
         {
            zLONG nTemp = lPosY;
            lPosY = lEndY;
            lEndY = nTemp;
            break;
         }

         default: // horizontal
            lPosY = (lPosY + lEndY) / 2;
            lEndY = lPosY;
            break;
      }

      // Note that MM_HIENGLISH has the origin at the bottom left ...
      // vs. MM_TEXT which has the origin at the top left.  The DPtoLP
      // call does appropriate conversions.

      zLONG lPosX1 = lPosX;
      zLONG lPosY1 = lPosY;
      zLONG lEndX1 = lEndX;
      zLONG lEndY1 = lEndY;

      lPosX1 = (lPosX1 * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      lPosY1 = (lPosY1 * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      lEndX1 = (lEndX1 * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      lEndY1 = (lEndY1 * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      zSHORT k = (zSHORT) ((lPenWidth + 1) / 2);
      lPosX1 -= k;
      lPosY1 -= k;
      lEndX1 -= k;
      lEndY1 -= k;

      CRect rect( lPosX1, lPosY1, lEndX1, lEndY1 );
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );
      lPosX1 = rect.left;
      lPosY1 = rect.top;
      lEndX1 = rect.right;
      lEndY1 = rect.bottom;

      if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
      {
         lPenColor &= 0x00ffffff;

         CSize size( lPenWidth, lPenWidth );
         if ( lPenType == PS_SOLID )
         {
            pZSubtask->m_pZPrintout->m_pDC->DPtoHIMETRIC( &size );
         // size.cy *= 2;
         }

         CPen *pPen = new CPen( lPenType, (zSHORT) size.cy, lPenColor );
         CPen *pPenOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPen );
      // CBrush *pBrushOld = (CBrush *)
      //    pZSubtask->m_pZPrintout->m_pDC->SelectStockObject( HOLLOW_BRUSH );
         pZSubtask->m_pZPrintout->m_pDC->MoveTo( lPosX1, lPosY1 );
         pZSubtask->m_pZPrintout->m_pDC->LineTo( lEndX1, lEndY1 );

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPenOld );
      // pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrushOld );
         delete( pPen );
      }

      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function draws a rectangle to the device.  The parameters are:
//    - view to the application's subtask
//    - lLeft256ths (left position)
//    - lTop256ths (top position)
//    - lRight256ths (right position)
//    - lBottom256ths (bottom position)
//    - lPenWidth (pen width used to draw the rectangles box ...
//      0 ==> use default pen width)
//    - lPenType : 0 - SOLID ... 1 - DASH ... 2 - DOT ...
//                 3 - DASH_DOT ... 4 - DASH_DOT_DOT
//    - lPenColor - RGB pen color
//    - lRounded  - Rounding factor for rounded corners ...
//                  0 ==> no rounding
//                 -1 ==> diamond shape
//
// The return value is:
//    0 - rectangle drawn (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawRectangleToReport( zVIEW   vSubtask,
                       zVIEW   vAppOI,
                       zLONG   lLeft256ths,
                       zLONG   lTop256ths,
                       zLONG   lRight256ths,
                       zLONG   lBottom256ths,
                       zLONG   lPenWidth,
                       zLONG   lPenType,
                       zLONG   lPenColor,
                       zLONG   lRounded,
                       zLONG   lBackgroundColor )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      if ( lPenWidth == 0 )
         lPenWidth = 1;

      switch ( lPenType )
      {
         case 0:
            lPenType = PS_SOLID;
            break;

         case 1:
            lPenType = PS_DASH;
            break;

         case 2:
            lPenType = PS_DOT;
            break;

         case 3:
            lPenType = PS_DASHDOT;
            break;

         case 4:
            lPenType = PS_DASHDOTDOT;
            break;

         default:
            lPenType = PS_SOLID;
            break;
      }

      // Note that MM_HIENGLISH has the origin at the bottom left ...
      // vs. MM_TEXT which has the origin at the top left.  The DPtoLP
      // call does appropriate conversions.
      // Determine coordinates in pixels using longs to avoid round off error.
      zLONG lLeft = (lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lTop = (lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      zLONG lRight = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lBottom = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      CRect rect( lLeft, lTop, lRight, lBottom );
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );

      if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
      {
         lPenColor &= 0x00ffffff;
         lBackgroundColor &= 0x00ffffff;

         CSize size( lPenWidth, lPenWidth );
         if ( lPenType == PS_SOLID )
         {
            pZSubtask->m_pZPrintout->m_pDC->DPtoHIMETRIC( &size );
         // size.cy *= 2;
         }

         CPen *pPen = new CPen( lPenType, (zSHORT) size.cy, lPenColor );
         CPen *pPenOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPen );
         CBrush *pBrush = 0;
         CBrush *pBrushOld;

         if ( lBackgroundColor )
         {
            pBrush = new CBrush( lBackgroundColor );
            pBrushOld = (CBrush *)
              pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrush );
         }
         else
         {
            pBrushOld = (CBrush *)
              pZSubtask->m_pZPrintout->m_pDC->SelectStockObject( HOLLOW_BRUSH );
         }

         if ( lRounded < 0 )  // draw a diamond
         {
   //       TraceLineI( "Drawing a DIAMOND: ", lRounded );
            pZSubtask->m_pZPrintout->m_pDC->MoveTo( (rect.left + rect.right) / 2, rect.top );
            pZSubtask->m_pZPrintout->m_pDC->LineTo( rect.right, (rect.top + rect.bottom) / 2 );
            pZSubtask->m_pZPrintout->m_pDC->LineTo( (rect.left + rect.right) / 2, rect.bottom );
            pZSubtask->m_pZPrintout->m_pDC->LineTo( rect.left, (rect.top + rect.bottom) / 2 );
            pZSubtask->m_pZPrintout->m_pDC->LineTo( (rect.left + rect.right) / 2, rect.top );
         }
         else
         if ( lRounded == 0 )
         {
            pZSubtask->m_pZPrintout->m_pDC->Rectangle( rect );
         }
         else
         {
            pZSubtask->m_pZPrintout->m_pDC->RoundRect( rect, CPoint( lRounded, lRounded ) );
         }

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPenOld );
         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrushOld );
         delete( pPen );
         if ( pBrush )
            delete( pBrush );
      }

      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function draws an ellipse to the device.  The parameters are:
//    - view to the application's subtask
//    - lLeft256ths (left position)
//    - lTop256ths (top position)
//    - lRight256ths (right position)
//    - lBottom256ths (bottom position)
//    - lPenWidth (pen width used to draw the ellipse ...
//      0 ==> use default pen width)
//    - lPenType : 0 - SOLID ... 1 - DASH ... 2 - DOT ...
//                 3 DASH_DOT ... 4 - DASH_DOT_DOT
//    - lPenColor - RGB pen color
//
// The return value is:
//    0 - ellipse drawn (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawEllipseToReport( zVIEW   vSubtask,
                     zVIEW   vAppOI,
                     zLONG   lLeft256ths,
                     zLONG   lTop256ths,
                     zLONG   lRight256ths,
                     zLONG   lBottom256ths,
                     zLONG   lPenWidth,
                     zLONG   lPenType,
                     zLONG   lPenColor,
                     zLONG   lBackgroundColor )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      if ( lPenWidth == 0 )
         lPenWidth = 1;

      switch ( lPenType )
      {
         case 0:
            lPenType = PS_SOLID;
            break;

         case 1:
            lPenType = PS_DASH;
            break;

         case 2:
            lPenType = PS_DOT;
            break;

         case 3:
            lPenType = PS_DASHDOT;
            break;

         case 4:
            lPenType = PS_DASHDOTDOT;
            break;

         default:
            lPenType = PS_SOLID;
            break;
      }

      // Note that MM_HIENGLISH has the origin at the bottom left ...
      // vs. MM_TEXT which has the origin at the top left.  The DPtoLP
      // call does appropriate conversions.
      // Determine coordinates in pixels using longs to avoid round off error.
      zLONG lLeft = (lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lTop = (lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      zLONG lRight = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      zLONG lBottom = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      CRect rect( lLeft, lTop, lRight, lBottom );
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );

      if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
      {
         lPenColor &= 0x00ffffff;
         lBackgroundColor &= 0x00ffffff;

         CSize size( lPenWidth, lPenWidth );
         if ( lPenType == PS_SOLID )
         {
            pZSubtask->m_pZPrintout->m_pDC->DPtoHIMETRIC( &size );
         // size.cy *= 2;
         }

         CPen *pPen = new CPen( lPenType, (zSHORT) size.cy, lPenColor );
         CPen *pPenOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPen );
         CBrush *pBrush = 0;
         CBrush *pBrushOld;

         if ( lBackgroundColor )
         {
            pBrush = new CBrush( lBackgroundColor );
            pBrushOld = (CBrush *) pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrush );
         }
         else
         {
            pBrushOld = (CBrush *) pZSubtask->m_pZPrintout->m_pDC->SelectStockObject( HOLLOW_BRUSH );
         }

         pZSubtask->m_pZPrintout->m_pDC->Ellipse( rect );
         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pPenOld );
         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pBrushOld );
         delete( pPen );
         if ( pBrush )
            delete( pBrush );
      }

      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function draws a bitmap to the device.  The parameters are:
//    - view to the application's subtask
//    - lLeft256ths (left position)
//    - lTop256ths (top position)
//    - lRight256ths (right position)
//    - lBottom256ths (bottom position)
//    - cpcBitmapCtrlBOI (CtrlBOI from the current control)
//
// The return value is:
//    0 - bitmap drawn (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//   -3 - bitmap cannot be located
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrawBitmapToReport( zVIEW   vSubtask,
                    zVIEW   vAppOI,
                    zLONG   lLeft256ths,
                    zLONG   lTop256ths,
                    zLONG   lRight256ths,
                    zLONG   lBottom256ths,
                    zCPCHAR cpcBitmapCtrlBOI )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      ZDib      dib;
      LPLIBRARY hLibrary;
      HINSTANCE hInstDLL = 0;
      zCHAR     szFileName[ zMAX_FILENAME_LTH ];
      zPCHAR    pchBitmapName = (zPCHAR) cpcBitmapCtrlBOI;
      BYTE      *pBmp = 0;
      zLONG     lLeft;
      zLONG     lTop;
      zLONG     lRight;
      zLONG     lBottom;
      zSHORT    nRC = -3;

      // Note that MM_HIENGLISH has the origin at the bottom left ...
      // vs. MM_TEXT which has the origin at the top left.  The DPtoLP
      // call does appropriate conversions.
      // Determine coordinates in pixels using longs to avoid round-
      // off error.
      lLeft = (lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      lTop = (lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;
      lRight = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX) / 256;
      lBottom = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY) / 256;

      CRect rect( lLeft, lTop, lRight, lBottom );
   // TraceRect( "DrawBitmapToReport: ", rect );
      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &rect );

      if ( pchBitmapName && *pchBitmapName == 0 &&
           *(pchBitmapName + 1) == '@' ) // V.E.A.P.X reference
      {
         GetVEAPX_Reference( vSubtask, vAppOI, pchBitmapName + 1, szFileName, zsizeof( szFileName ) );
         if ( *szFileName )
         {
            zPCHAR pchExt = zstrrchr( szFileName, '.' );

            if ( pchExt && (zstricmp( pchExt + 1, "jpg" ) == 0 || zstricmp( pchExt + 1, "jpeg" ) == 0) )
            {
               UINT nWidth;
               UINT nHeight;

               pBmp = LoadBitmapFromJPG( szFileName, &nWidth, &nHeight );
            // TraceRect( "DrawJPeg: ", rect );
               ZJpegFile::DrawIt( *(pZSubtask->m_pZPrintout->m_pDC), rect, pBmp, nWidth, nHeight, TRUE );
               delete [] pBmp;
               nRC = 0;
            }
         }

         if ( pchBitmapName == 0 )
            return( nRC );
      }
      else
      if ( pchBitmapName && *pchBitmapName )   // DLL_Name
      {
         if ( pZSubtask->m_pZTask->m_uAppState & zAPP_STATE_NT )
         {
            hLibrary = SysLoadLibraryWithErrFlag( vSubtask, pchBitmapName, zLOADLIB_RESOURCES );
            hInstDLL = (HINSTANCE) SysGetInstance( hLibrary );
         }
         else
         {
            zFARPROC_HINSTANCE lpfnDynRoutine;

            hLibrary = SysLoadLibraryWithErrFlag( vSubtask, pchBitmapName, 0 );

            lpfnDynRoutine = (zFARPROC_HINSTANCE)
               GetOperationDynamicCallAddress( vSubtask, (LPLPLIBRARY) &hLibrary,
                                               pchBitmapName, "GetDLL_Instance", 0 );  // no error message
            if ( lpfnDynRoutine )
               hInstDLL = (*lpfnDynRoutine)( vSubtask );
         }

         pchBitmapName += zstrlen( pchBitmapName ) + 1;  // skip past DLL name
      }
      else
      {
         hLibrary = 0;
         if ( pchBitmapName )
            pchBitmapName++;      // skip past null DLL name
      }

      zPCHAR pchSemiColon = zstrchr( pchBitmapName, ';' );

      if ( pchSemiColon )
         pchSemiColon++;
      else
         pchSemiColon = pchBitmapName;

      if ( *pchSemiColon == '.' && *(pchSemiColon + 1) == '\\' )
         pchSemiColon += 2;

      do    // purist's goto
      {
         if ( hInstDLL )
         {
            UINT uResId;

            if ( (isdigit( *pchBitmapName ) || (pchSemiColon != 0)) &&
                 (uResId = (zUSHORT) zatol( pchBitmapName )) != 0 )
            {
               if ( dib.Load( hInstDLL, uResId ) )
                  break;

               if ( pZSubtask == 0 || pZSubtask->m_pZTask->m_nTraceAction > 2 )
               {
                  TraceLineI( "DrawBitmapToReport: Cannot access bitmap res id ", uResId );
               }
            }

            if ( dib.Load( hInstDLL, pchSemiColon ) )
               break;

            if ( pZSubtask == 0 || pZSubtask->m_pZTask->m_nTraceAction > 2 )
            {
               TraceLineS( "DrawBitmapToReport: Cannot access bitmap resource ", pchSemiColon );
            }
         }

         if ( pchSemiColon )
         {
            if ( dib.Load( pchSemiColon ) == FALSE &&
                 pchSemiColon[ 0 ] && pchSemiColon[ 1 ] != ':' )
            {
               zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
               LPAPP pApp;

               if ( SfGetApplicationForSubtask( &pApp, vSubtask ) == 0 &&
                    pApp )
               {
                  strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );

                  if ( szZeidonPath[ 0 ] )
                  {
                     SysAppendcDirSep( szZeidonPath );
                     strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
                     if ( dib.Load( szZeidonPath ) )
                        break;

                     if ( pZSubtask == 0 ||
                          pZSubtask->m_pZTask->m_nTraceAction > 2 )
                     {
                        TraceLineS( "DrawBitmapToReport: Cannot access bitmap file in Application Dir ", szZeidonPath );
                     }
                  }
               }

               SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szZeidonPath, zsizeof( szZeidonPath ) );
               if ( szZeidonPath[ 0 ] )
               {
                  SysAppendcDirSep( szZeidonPath );
                  strcat_s( szZeidonPath, zsizeof( szZeidonPath ), pchSemiColon );
                  if ( dib.Load( szZeidonPath ) )
                     break;

                  if ( pZSubtask == 0 || pZSubtask->m_pZTask->m_nTraceAction > 2 )
                  {
                     TraceLineS( "DrawBitmapToReport: Cannot access bitmap file in ResourcePath ", szZeidonPath );
                  }
               }

               if ( pZSubtask == 0 || pZSubtask->m_pZTask->m_nTraceAction > 1 )
               {
                  TraceLineS( "DrawBitmapToReport: Cannot access bitmap file ", pchSemiColon );
               }
            }
         }

      } while ( FALSE );  // end purist's goto

      if ( dib.m_bBitmapLoaded )
      {
         if ( pZSubtask->m_pZPrintout->m_bPhantom == FALSE )
         {
            if ( dib.Draw( *(pZSubtask->m_pZPrintout->m_pDC), &rect, 0, FALSE, 0, TRUE ) == TRUE )
            {
               return( 0 );
            }

            return( -2 );
         }

         return( 0 );
      }

      return( -3 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This sets the number of characters in a TabStop for a report.  The
// default value is 8.
//    - view to the application's subtask
//    - number of characters in a TabStop
//
// The return value is:
//   -1 - invalid view to the application's subtask
//  otw - previous number of TabStops
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
PushReportTabStops( zVIEW vSubtask, zSHORT nTabStops )
{
   ZSubtask *pZSubtask;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->PushTabStops( nTabStops ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This resets the number of characters in a TabStop for a report to the
// previous setting.
//    - view to the application's subtask
//
// The return value is:
//   -1 - invalid view to the application's subtask
//  otw - new (previous) number of TabStops
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
PopReportTabStops( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->PopTabStops( ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// This pushes the current "view port" for a report.
//
//    - View to the application's subtask
//    - Current Entity Name
//    - RepeatHorizontal
//    - RepeatHorizontalColumn
//    - RepeatHorizontalIndent
//    - RepeatHorizontalMargin
//    - RepeatHorizontalExtent
//    - RepeatHorizontalFlags
//    - ViewPortBottomPrev
//    - ViewPortTop
//    - ViewPortLeft
//    - ViewPortBottom
//    - ViewPortRight
//
// The return value is:
//   -1 - invalid view to the application's subtask
//    0 - view port pushed onto stack
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PushReportViewPort( zVIEW   vSubtask,
                    zCPCHAR cpcCurrentEntityName,
                    zLONG   lRepeatHorizontal,
                    zLONG   lRepeatHorizontalColumn,
                    zLONG   lRepeatHorizontalIndent,
                    zLONG   lRepeatHorizontalMargin,
                    zLONG   lRepeatHorizontalExtent,
                    zLONG   lRepeatHorizontalFlags,
                    zLONG   lViewPortBottomPrev,
                    zLONG   lViewPortTop,
                    zLONG   lViewPortLeft,
                    zLONG   lViewPortBottom,
                    zLONG   lViewPortRight )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->PushViewPort( cpcCurrentEntityName,
                                                     lRepeatHorizontal,
                                                     lRepeatHorizontalColumn,
                                                     lRepeatHorizontalIndent,
                                                     lRepeatHorizontalMargin,
                                                     lRepeatHorizontalExtent,
                                                     lRepeatHorizontalFlags,
                                                     lViewPortBottomPrev,
                                                     lViewPortTop,
                                                     lViewPortLeft,
                                                     lViewPortBottom,
                                                     lViewPortRight ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This pops the current "view port" for a report.
//
//    - View to the application's subtask
//    - Current Entity Name
//    - RepeatHorizontal
//    - RepeatHorizontalColumn
//    - RepeatHorizontalIndent
//    - RepeatHorizontalMargin
//    - RepeatHorizontalExtent
//    - RepeatHorizontalFlags
//    - ViewPortBottomPrev
//    - ViewPortTop
//    - ViewPortLeft
//    - ViewPortBottom
//    - ViewPortRight
//
// The return value is:
//   -1 - invalid view to the application's subtask
//    0 - nothing to pop from stack
//    1 - view port pushed onto stack
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
PopReportViewPort( zVIEW   vSubtask,
                   zPCHAR  pchCurrentEntityName,
                   zPLONG  plRepeatHorizontal,
                   zPLONG  plRepeatHorizontalColumn,
                   zPLONG  plRepeatHorizontalIndent,
                   zPLONG  plRepeatHorizontalMargin,
                   zPLONG  plRepeatHorizontalExtent,
                   zPLONG  plRepeatHorizontalFlags,
                   zPLONG  plViewPortBottomPrev,
                   zPLONG  plViewPortTop,
                   zPLONG  plViewPortLeft,
                   zPLONG  plViewPortBottom,
                   zPLONG  plViewPortRight )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->PopViewPort( pchCurrentEntityName,
                                                    plRepeatHorizontal,
                                                    plRepeatHorizontalColumn,
                                                    plRepeatHorizontalIndent,
                                                    plRepeatHorizontalMargin,
                                                    plRepeatHorizontalExtent,
                                                    plRepeatHorizontalFlags,
                                                    plViewPortBottomPrev,
                                                    plViewPortTop,
                                                    plViewPortLeft,
                                                    plViewPortBottom,
                                                    plViewPortRight ) );
   }

   pchCurrentEntityName[ 0 ] = 0;
   *plRepeatHorizontal = 0;
   *plRepeatHorizontalColumn = 0;
   *plRepeatHorizontalIndent = 0;
   *plRepeatHorizontalMargin = 0;
   *plRepeatHorizontalExtent = 0;
   *plRepeatHorizontalFlags = 0;
   *plViewPortBottomPrev = 0;
   *plViewPortTop = 0;
   *plViewPortLeft = 0;
   *plViewPortBottom = 0;
   *plViewPortRight = 0;
   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function creates a font for use in a report.  The parameters are:
//    - view to the application's subtask
//    - FontTag (unique identifier for the font)
//    - Font size
//    - Bold or not (TRUE ==> BOLD ... FALSE ==> not bold)
//    - Italic or not (TRUE ==> Italic ... FALSE ==> not italic)
//    - Underline or not (TRUE ==> Underline ... FALSE ==> not underline)
//    - Strikeout or not (TRUE ==> Strikeout ... FALSE ==> not strikeout)
//    - RGB Text Color (CLR_INVALID ==> default)
//    - RGB Text Background Color (CLR_INVALID ==> default)
//    - Escapement (angle in tenths of a degree from the x-axis)
//    - FaceName (font face name 0 ==> default)
//
// The return value is:
//    0 - text printed (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
CreateFontForReport( zVIEW   vSubtask,
                     zCPCHAR cpcFontTag,
                     zLONG   nFontSize,
                     zSHORT  bBold,
                     zSHORT  bItalic,
                     zSHORT  bUnderline,
                     zSHORT  bStrikeout,
                     zLONG   lTextColor,
                     zLONG   lTextBkColor,
                     zLONG   nEscapement,
                     zCPCHAR cpcFaceName )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      ///////////////////////////////////////////////////////////////////////
      //
      // The normal conversion routines for converting between point size
      // and pixel heights of fonts are:
      //
      // CellHeight = (PointSize * VerticalResolution) / 72 + InternalLeading;
      //
      // PointSize = (CellHeight - InternalLeading) * 72 / VerticalResolution;
      //
      // There is a problem in that the internal leading value is not
      // available until after the font is selected.  The simplest way to
      // overcome the internal leading problem is to specify the lfHeight
      // attribute as a negative value.  The font mapper treats the absolute
      // value of that number as the desired em height (i.e.  the value
      // defined by CellHeight - InternalLeading) rather than as the cell
      // height.  To define a height for a font with a given point size, use:
      // Font.lfHeight = -(PointSize * GetDeviceCaps( hDC, LOGPIXELSY)) / 72;
      //
      ///////////////////////////////////////////////////////////////////////

      nFontSize = -(nFontSize * 16) / 10;
      pZSubtask->m_pZPrintout->AddFont( vSubtask, cpcFontTag, (zSHORT) nFontSize,
                                     // -((nSize * pZSubtask->m_pZPrintout->m_pDC->GetDeviceCaps( LOGPIXELSY )) / 72),
                                        bBold ? FW_BOLD : FW_NORMAL,
                                        bItalic, bUnderline, bStrikeout,
                                        lTextColor, lTextBkColor,
                                        (zSHORT) (nEscapement * pZSubtask->m_pZPrintout->m_nEscapementFactor),
                                        cpcFaceName );
      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function selects a (previously defined) font for the device.  The
// parameters are:
//    - view to the application's subtask
//    - font tag (if tag is null, original default font is re-selected)
//
// The return value is:
//    0 - font selected
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//   -3 - font has not been defined
//   -4 - hDC is null
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SelectFontForReport( zVIEW   vSubtask,
                     zCPCHAR cpcFontTag )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

//    TraceLineX( "m_pDC from SelectFontForReport = ", (zLONG) pZSubtask->m_pZPrintout->m_pDC );
      if ( pZSubtask->m_pZPrintout->m_pDC == 0 )
      {
         return( -4 );
      }

      if ( cpcFontTag )
      {
         pZSubtask->m_pZPrintout->m_pFontCurr = pZSubtask->m_pZPrintout->LocateFont( cpcFontTag );
         if ( pZSubtask->m_pZPrintout->m_pFontCurr )
            return( 0 );
         else
            return( -3 );
      }
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function gets the width of the text for the current device.  The
// parameters are:
//    - view to the application's subtask
//    - text string
//
// The return value is:
//  >= 0 - text width
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
GetTextWidth( zVIEW   vSubtask,
              zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      CSize    sizeExtent;
      CFont    *pFontOld = 0;
      COLORREF clrOldText = CLR_INVALID;

      if ( pZSubtask->m_pZPrintout->m_pFontCurr )
      {
         pFontOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pZSubtask->m_pZPrintout->m_pFontCurr );

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != CLR_INVALID )
         {
            clrOldText = (COLORREF)
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         }

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet == FALSE )
         {
            pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet = TRUE;
            OUTLINETEXTMETRIC OTM;   // metric data array
            if ( pZSubtask->m_pZPrintout->m_pDC->GetOutlineTextMetrics( sizeof( OUTLINETEXTMETRIC ), &OTM ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_Panose = OTM.otmPanoseNumber;
            }

            LOGFONT lf;
            if ( pZSubtask->m_pZPrintout->m_pFontCurr->GetLogFont( &lf ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byCharset = lf.lfCharSet;
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byPitchFamily = lf.lfPitchAndFamily;
            }
         }
      }

      GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC, cpcText,  zstrlen( cpcText ), &sizeExtent );

      // Determine line width.
   // TEXTMETRIC tm;
   // pZSubtask->m_pZPrintout->m_pDC->GetTextMetrics( &tm );
   // sizeExtent.cx += tm.tmExternalLeading * 2;

      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &sizeExtent );
      zSHORT nWidth = (zSHORT) ((sizeExtent.cx * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchX);

      if ( pFontOld )
      {
         if ( clrOldText != CLR_INVALID )
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( clrOldText );

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pFontOld );
      }

      return( nWidth );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function get the height of the text for the current device.  The
// parameters are:
//    - view to the application's subtask
//    - text string
//
// The return value is:
//  >= 0 - text height
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
GetTextHeight( zVIEW   vSubtask,
               zCPCHAR cpcText )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout && pZSubtask->m_pZPrintout->m_pDC &&
        pZSubtask->m_pZPrintout->m_pDC->m_hDC )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      CSize    sizeExtent;
      CFont    *pFontOld = 0;
      COLORREF clrOldText = CLR_INVALID;

      if ( pZSubtask->m_pZPrintout->m_pFontCurr )
      {
         pFontOld = pZSubtask->m_pZPrintout->m_pDC->SelectObject( pZSubtask->m_pZPrintout->m_pFontCurr );

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != CLR_INVALID )
         {
            clrOldText = (COLORREF) pZSubtask->m_pZPrintout->m_pDC->SetTextColor( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         }

         if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet == FALSE )
         {
            pZSubtask->m_pZPrintout->m_pFontCurr->m_bSet = TRUE;
            OUTLINETEXTMETRIC OTM;   // metric data array
            if ( pZSubtask->m_pZPrintout->m_pDC->GetOutlineTextMetrics( sizeof( OUTLINETEXTMETRIC ), &OTM ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_Panose = OTM.otmPanoseNumber;
            }

            LOGFONT lf;
            if ( pZSubtask->m_pZPrintout->m_pFontCurr->GetLogFont( &lf ) )
            {
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byCharset = lf.lfCharSet;
               pZSubtask->m_pZPrintout->m_pFontCurr->m_byPitchFamily = lf.lfPitchAndFamily;
            }
         }
      }

      GetTextExtentPoint32( pZSubtask->m_pZPrintout->m_pDC->m_hDC, cpcText, zstrlen( cpcText ), &sizeExtent );

      // Determine line height.
   // TEXTMETRIC tm;
   // pZSubtask->m_pZPrintout->m_pDC->GetTextMetrics( &tm );
   // sizeExtent.cy += tm.tmDescent;

      pZSubtask->m_pZPrintout->m_pDC->DPtoLP( &sizeExtent );
      zSHORT nHeight = (zSHORT) ((sizeExtent.cy * 256) / pZSubtask->m_pZPrintout->m_nPixelsPerInchY);

      if ( pFontOld )
      {
         if ( clrOldText != CLR_INVALID )
            pZSubtask->m_pZPrintout->m_pDC->SetTextColor( clrOldText );

         pZSubtask->m_pZPrintout->m_pDC->SelectObject( pFontOld );
      }

      return( nHeight );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function handles page numbering for multi-root reports that want
// paging to reset for each new root entity.
//
// The return value is:
//   0 - the page number has been set
// > 0 - page number ... page and "of" page numbers have been successfully retrieved
//  -1 - invalid view to the application's subtask
//  -2 - page numbering retrieval error
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
HandlePageNumbering( zVIEW  vSubtask,
                     zPCHAR pchPageNumber,
                     zLONG  lEntityKey,
                     zLONG  lPageNbr,
                     zPLONG plTotalPages,
                     zLONG  lFlags )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->HandlePageNumbering( pchPageNumber,
                                                            lEntityKey,
                                                            lPageNbr,
                                                            plTotalPages,
                                                            lFlags ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function sets the "phantom mode" for a report.
//
// The return value is:
//   0 or 1 - previous phantom mode
//   -1 - invalid view to the application's subtask
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zBOOL OPERATION
SetPhantomMode( zVIEW vSubtask,
                zBOOL bPhantom )
{
   ZSubtask *pZSubtask;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->SetPhantomMode( bPhantom ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function sets the "phantom mode" for a report.
//
// The return value is:
//   0 or 1 - current phantom mode
//   -1 - invalid view to the application's subtask
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zBOOL OPERATION
GetPhantomMode( zVIEW vSubtask )
{
   ZSubtask *pZSubtask;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->GetPhantomMode( ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function sets view positioning information for a report.
//
// The return value is:
//    1 - information could not be set
//    0 - information successfully set
//   -1 - invalid view to the application's subtask
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
AddPagePosition( zVIEW   vSubtask,
                 zLONG   lPageNbr,
                 zVIEW   vSourceHierOI,
                 zVIEW   vReportDef,
                 zLONG   lPagePosition,
                 zLONG   lRC,
                 zLONG   lLevel,
                 zLONG   lFlags,
                 zLONG   lMultiLineTextPos,
                 zLONG   lContinuedGroupFlag,
                 zCPCHAR cpcEntityName,
                 zCPCHAR cpcRptGroupSetTag,
                 zCPCHAR cpcRptGroupTag )
{
   ZSubtask *pZSubtask;

   if ( vSubtask &&
        GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->AddPagePosition( lPageNbr,
                                                        vSourceHierOI,
                                                        vReportDef,
                                                        lPagePosition,
                                                        lRC, lLevel,
                                                        lFlags,
                                                        lMultiLineTextPos,
                                                        lContinuedGroupFlag,
                                                        cpcEntityName,
                                                        cpcRptGroupSetTag,
                                                        cpcRptGroupTag ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function gets view positioning information for a report.
//
// The return value is:
//    1 - information not available
//    0 - information successfully returned
//   -1 - invalid view to the application's subtask
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
GetPagePosition( zVIEW   vSubtask,
                 zLONG   lPageNbr,
                 zVIEW   vSourceHierOI,
                 zVIEW   vReportDef,
                 zPLONG  plPagePosition,
                 zPLONG  plRC,
                 zPLONG  plLevel,
                 zPLONG  plFlags,
                 zPLONG  plMultiLineTextPos,
                 zPLONG  plContinuedGroupFlag,
                 zPCHAR  pchEntityName,
                 zPCHAR  pchRptGroupSetTag,
                 zPCHAR  pchRptGroupTag )
{
   ZSubtask *pZSubtask;

   if ( vSubtask && GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 && pZSubtask->m_pZPrintout )
   {
      return( pZSubtask->m_pZPrintout->GetPagePosition( lPageNbr,
                                                        vSourceHierOI,
                                                        vReportDef,
                                                        plPagePosition,
                                                        plRC, plLevel,
                                                        plFlags,
                                                        plMultiLineTextPos,
                                                        plContinuedGroupFlag,
                                                        pchEntityName,
                                                        pchRptGroupSetTag,
                                                        pchRptGroupTag ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: IsValidView
//
//    This is used mostly for debugging
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*GLOBAL */  OPERATION
IsValidView( zVIEW vAny )
{
   return( MiValidView( vAny ) );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: MakeViewFromInteger
//
//    Simple casting operation to eliminate warnings
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
MakeViewFromInteger( zPVIEW pvReturnedView,
                     zLONG lViewSavedAsInteger )
{
   *pvReturnedView = (zVIEW) lViewSavedAsInteger;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: MakeIntegerFromView
//
//    Simple casting operation to eliminate warnings
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
MakeIntegerFromView( zVIEW vAnyView )
{
   return( (zLONG) vAnyView );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: MakePointerFromInteger
//
//    Simple casting operation to eliminate warnings
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
MakePointerFromInteger( zPCHAR *ppchReturnedPointer,
                        zLONG  lPointerSavedAsInteger )
{
   *ppchReturnedPointer = (zPCHAR) lPointerSavedAsInteger;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: MakeIntegerFromPointer
//
//    Simple casting operation to eliminate warnings
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
MakeIntegerFromPointer( zPCHAR pchAnyPointer )
{
   return( (zLONG) pchAnyPointer );
}


zPCHAR g_Events[ 5 ] =
   {
      "<unknown> ",
      "Pre-Print ",
      "Continue  ",
      "GroupSplit",
      "Post-Print"
   };

/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: InvokeReportExitCode
//
// PURPOSE:  This routine dynamically invokes the requested report
//           operation code.
//
// EVENTS:
//          zRGE_EVENT_PREPRINT
//          zRGE_EVENT_GROUP_CONTINUE
//          zRGE_EVENT_GROUP_SPLITTING
//          zRGE_EVENT_POSTPRINT
//
// RETURNS:
//          zRGE_RETURN_NORMAL
//          zRGE_RETURN_SUPPRESS
//          zRGE_RETURN_NEWPAGE
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
InvokeReportExitCode( zVIEW  vSubtask,
                      zVIEW  vReportDef,
                      zPCHAR cpcCodeName,
                      zSHORT nEvent,
                      zSHORT nPageNbr,
                      zVIEW  vSourceOI )
{
   zPCHAR    pchDLL_Name;
   zPCHAR    pchGrpSetName;
   zPCHAR    pchGroupName;
   zFARPROC_REPORT lpfnDynRoutine;
   LPLIBRARY hLibrary = 0;
   zSHORT    nRC = 0;
   zSHORT    bTrace;

   if ( *cpcCodeName )
   {
      // Get the DLL name, GroupSet and Group names.
      GetAddrForAttribute( (zPCHAR *) &pchDLL_Name, vReportDef, "Report", "wDLL_Name" );
      if ( *pchDLL_Name )
      {
         zSHORT nIdx;

         GetAddrForAttribute( (zPCHAR *) &pchGrpSetName, vReportDef, "GroupSet", "Tag" );
         GetAddrForAttribute( (zPCHAR *) &pchGroupName, vReportDef, "Group", "Tag" );

         if ( nEvent > 0 && nEvent < 5 )
            nIdx = nEvent;
         else
            nIdx = 0;

         lpfnDynRoutine = (zFARPROC_REPORT) GetOperationDynamicCallAddress( vSubtask, (LPLPLIBRARY) &(hLibrary), pchDLL_Name, cpcCodeName, 0 );

         if ( lpfnDynRoutine )
         {
            ZSubtask *pZSubtask;

            bTrace = FALSE;
            if ( vSubtask &&
                 GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
            {
               if ( pZSubtask == 0 || (pZSubtask->m_pZTask->m_nTraceAction & 0x70) == 0x70 )
               {
                  bTrace = TRUE;
               }
            }

            if ( bTrace )
            {
               TraceLine( "==>rptExit [DLL:%s]%s for Event:(%d)%s; "
                                "Page:%d; GrpSet:%s; Grp:%s",
                          pchDLL_Name, cpcCodeName, nEvent, g_Events[ nIdx ],
                          nPageNbr, pchGrpSetName, pchGroupName );
            }

            nRC = (*lpfnDynRoutine)( vSubtask, vSourceOI, pchGrpSetName, pchGroupName, nEvent, nPageNbr );
            if ( bTrace )
            {
               TraceLine( "<==rptExit [DLL:%s]%s returned: %d", pchDLL_Name, cpcCodeName, nRC );
            }
         }
         else
         {
            TraceLine( "InvokeReportExitCode cannot load: [DLL:%s]%s "
                       "for Event:(%d)%s  Page:%d  GrpSet:%s  Grp:%s",
                       pchDLL_Name, cpcCodeName, nEvent, g_Events[ nIdx ],
                       nPageNbr, pchGrpSetName, pchGroupName );
         }
      }
   }

   return( nRC );
}

//./ ADD NAME=GetCtrlPrintStatus
// Source Module=zdrprtc.cpp
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetCtrlPrintStatus
//
// PURPOSE:  This routine returns the current Print Status of a control
//           within a Group
//
// PARAMETERS:  vSubtask    - The same subtask view received via the report exit
//              szGroupSet  - Tag for the GroupSet
//              szGroup     - Tag for the Group within the GroupSet
//              szControl   - Tag for the Control within the Group
//
// RETURNS:   -1 - ERROR, Couldn't locate control
//             0 - Control not in a Group that is actively being printed
//                 (zRCONTROL_STATUS_GROUPINACTIVE)
//             1 - Control not printed (zRCONTROL_STATUS_NOTPRINTED)
//             2 - Control partially printed (multi-line text)
//                 (zRCONTROL_STATUS_PARTIALPRINT)
//             3 - Control printed (zRCONTROL_STATUS_PRINTED)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
GetCtrlPrintStatus( zVIEW    vSubtask,
                    zPCHAR   szGroupSet,
                    zPCHAR   szGroup,
                    zPCHAR   szControl )
{
   zVIEW  vReportDef;
   zVIEW  vReport;
   zLONG  lRC;
   zCHAR  szMsg[ 1000 ];
   zCHAR  szCtrl[ zTAG_LTH ];
   zCHAR  szText[ 257 ];
   zLONG  lStatus;
   zLONG  lPageNbr;
   zLONG  lPageX;
   zLONG  lPageY;

   // Get the report view.
   if ( GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vReport, vReportDef, vSubtask );

   lRC = -1;
   if ( SetCursorFirstEntityByString( vReport, "GroupSet", "Tag", szGroupSet, "Report" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByString( vReport, "Group", "Tag", szGroup, "" ) >= zCURSOR_SET )
      {
         if ( szControl[ 0 ] != '*' )
         {
            if ( SetCursorFirstEntityByString( vReport, "Control", "Tag", szControl, "" ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lRC, vReport, "Control", "wPrintStatus" );
            }
         }
         else
         {
            if ( SetCursorFirstEntity( vReport, "Control", 0 ) >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lRC, vReport, "Control", "wPrintStatus" );
            }

            for ( zSHORT rc = SetCursorFirstEntity( vReport, "Control", 0 );
                  rc >= zCURSOR_SET;
                  rc = SetCursorNextEntity( vReport, "Control", 0 ) )
            {
               GetStringFromAttributeByContext( szCtrl, vReport, "Control", "Tag", "", 256 );
               GetIntegerFromAttribute( &lStatus, vReport, "Control", "wPrintStatus" );
               GetStringFromAttributeByContext( szText, vReport, "Control", "wLastPrintText", "", 256 );
               GetIntegerFromAttribute( &lPageNbr, vReport, "Control", "wLastPrintPage" );
               GetIntegerFromAttribute( &lPageX, vReport, "Control", "wLastPrintPosX" );
               GetIntegerFromAttribute( &lPageY, vReport, "Control", "wLastPrintPosY" );
               sprintf_s( szMsg, zsizeof( szMsg ), "RPT CTL Dump: %-32s - Status=%d; Page=%d; X=%d; Y=%d; Text=%s",
                          szCtrl, lStatus, lPageNbr, lPageX, lPageY, szText );
               TraceLineS( szMsg, "" );
            }
         }
      }
   }

   DropView( vReport );
   return( (zSHORT) lRC );
}

//./ ADD NAME=GetCtrlPrintText
// Source Module=zdrprtc.cpp
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetCtrlPrintText
//
// PURPOSE:  This routine returns the first 255 characters of the text
//           last printed for the specified control within a Group
//           This is valid even if the controls print status is 0 (not
//           in group actively being printed) or 1 (not yet printed).
//           In this way, you can determine the controls last printed
//           text on a previous GroupHeader.
//
// PARAMETERS:  vSubtask    - The same subtask view received via the report exit
//              szGroupSet  - Tag for the GroupSet
//              szGroup     - Tag for the Group within the GroupSet
//              szControl   - Tag for the Control within the Group
//              pcTextBuf   - Pointer to buffer where text is returned
//              usLth       - Maximum length of return buffer
//
// RETURNS:   -1 - ERROR, Couldn't locate control
//             n - Length of text returned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
GetCtrlPrintText( zVIEW    vSubtask,
                  zPCHAR   szGroupSet,
                  zPCHAR   szGroup,
                  zPCHAR   szControl,
                  zPCHAR   szText,
                  zUSHORT  usLth )
{
   zVIEW  vReportDef;
   zVIEW  vReport;
   zLONG  lRC;

   // Get the report view.
   if ( GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vReport, vReportDef, vSubtask );

   lRC = -1;
   if ( SetCursorFirstEntityByString( vReport, "GroupSet", "Tag", szGroupSet, "Report" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByString( vReport, "Group", "Tag", szGroup, "" ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByString( vReport, "Control", "Tag", szControl, "" ) >= zCURSOR_SET )
         {
            GetStringFromAttributeByContext( szText, vReport, "Control", "wLastPrintText", "", usLth );
            lRC = zstrlen( szText );
         }
      }
   }

   DropView( vReport );
   return( (zSHORT) lRC );
}

//./ ADD NAME=GetCtrlPrintPosition
// Source Module=zdrprtc.cpp
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetCtrlPrintPosition
//
// PURPOSE:  This routine returns the page number and X, Y position of the
//           specified control that was last printed.  This is valid even if
//           the control's print status is 0 (not in group actively being
//           printed) or 1 (not yet printed).  In this way, you can determine
//           the controls last printed position on a previous GroupHeader.
//
// PARAMETERS:  vSubtask    - The same subtask view received via the report exit
//              szGroupSet  - Tag for the GroupSet
//              szGroup     - Tag for the Group within the GroupSet
//              szControl   - Tag for the Control within the Group
//              plPageNbr   - Pointer to returned integer Page Number
//              plPageX     - Pointer to returned integer Page X position
//              plPageY     - Pointer to returned integer Page Y position
//
// RETURNS:   -1 - ERROR, Couldn't locate control
//             0 - values returned
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
GetCtrlPrintPosition( zVIEW    vSubtask,
                      zPCHAR   szGroupSet,
                      zPCHAR   szGroup,
                      zPCHAR   szControl,
                      zPLONG   plPageNbr,
                      zPLONG   plPageX,
                      zPLONG   plPageY)
{
   zVIEW  vReportDef;
   zVIEW  vReport;
   zLONG  lRC;

   // Get the report view.
   if ( GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK ) < 0 )
      return( -1 );

   CreateViewFromViewForTask( &vReport, vReportDef, vSubtask );

   lRC = -1;
   if ( SetCursorFirstEntityByString( vReport, "GroupSet", "Tag", szGroupSet, "Report" ) >= zCURSOR_SET )
   {
      if ( SetCursorFirstEntityByString( vReport, "Group", "Tag", szGroup, "" ) >= zCURSOR_SET )
      {
         if ( SetCursorFirstEntityByString( vReport, "Control", "Tag", szControl, "" ) >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( plPageNbr, vReport, "Control", "wLastPrintPage" );
            GetIntegerFromAttribute( plPageX, vReport, "Control", "wLastPrintPosX" );
            GetIntegerFromAttribute( plPageY, vReport, "Control", "wLastPrintPosY" );
            lRC = 0;
         }
      }
   }

   DropView( vReport );
   return( (zSHORT) lRC );
}

//./ ADD NAME=GetPrintCurrentEntity
// Source Module=zdrprtc.cpp
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetPrintCurrentEntity
//
// PURPOSE:  This routine gets the GetPrintCurrentEntity attribute from the
//           report definition Report entity.
//
// PARAMETERS:  vSubtask - The same subtask view received via the report exit
//
// RETURNS:   CurrentEntityName (may be empty)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zSHORT OPERATION
GetPrintCurrentEntity( zVIEW   vSubtask,
                       zPCHAR  pchEntityName )
{
   zVIEW  vReportDef;

   // Get the report view.
   *pchEntityName = 0;
   if ( GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetStringFromAttribute( pchEntityName, zTAG_LTH, vReportDef, "Report", "CurrentEntityName" );
   }

   return( 0 );
}

//./ ADD NAME=GetContinuedGroupFlag
// Source Module=zdrprtc.cpp
/////////////////////////////////////////////////////////////////////////////
//
// FUNCTION: GetContinuedGroupFlag
//
// PURPOSE:  This routine gets the ContinuedGroupFlag attribute from the
//           report definition Report entity.
//
// PARAMETERS:  vSubtask - The same subtask view received via the report exit
//
// RETURNS:   0 - Group is not continued
//            Otherwise - Group is continued
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
zOPER_EXPORT zLONG OPERATION
GetContinuedGroupFlag( zVIEW  vSubtask )
{
   zVIEW  vReportDef;
   zLONG  lContinuedGroupFlag = 0;

   // Get the report view.
   if ( GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK ) > 0 )
   {
      GetIntegerFromAttribute( &lContinuedGroupFlag, vReportDef, "Report", "ContinuedGroupFlag" );
   }

   return( lContinuedGroupFlag );
}
