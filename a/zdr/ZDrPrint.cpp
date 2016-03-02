/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrprint.cpp
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
// 2002.01.15    DKS    Z10
//    Added support for TabStops in a report (default value may be
//    specified in .ini file).
//
// 2001.11.05    DKS
//    Added capability to drop the SourceOI view for the report.
//
// 2001.06.21    TMV
//    In addition to error code, trace the error message if
//    FtpPutFileA failed see fnFtpPutFile.
//
// 2001.05.22    US
//    Additional necessary changes for FTP
//
// 2001.05.21    US
//    Use FTP instead of file copy to send files to the web server
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   RemoteServer
//    Fixed generation of .PDF file on RemoteServer.
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2001.03.13    PAS
//    The "FontScale" has been moved to a printer specific section and
//    is no longer in the [Workstation] section of the INI chain
//    Now in ZPrintout::RemoteFilePrint and in ZDrView::OnBeginPrinting
//
// 2001.02.28    DKS   Z10
//   Delete named view "TZ_ReportDef".
//
// 2000.08.07    RG    Z10
//   Changed declaration of fontsize from zSHORT to zLONG to remove
//   compiler warnings in Zeidon Tools
//
// 1999.12.03    TMV   Z10
//    Fixed warnings ... unreferenced local variable.
//
// 1999.09.03    DKS   Z10   QS999
//    Select old font prior to removing a font.
//
// 1999.02.02    DKS
//    Added default font support for reports.
//
// 1999.01.04    DKS
//    Fixed color override for fonts.
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"

#include "wininet.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// Class definitions for ZLevelItem & ZPageItem (simple single-linked lists).
//
class ZDRAPP_CLASS ZLevelItem
{
public:
   ZLevelItem( zLONG lLevel, zCPCHAR cpcEntityName,
               zLONG lFooterPos = 0, zCPCHAR cpcFooterEntityName = 0 );
   ~ZLevelItem( );

public:
   zLONG       m_lLevel;
   zLONG       m_lFooterPos;
   zCHAR       m_szEntityName[ zTAG_LTH ];
   zCHAR       m_szFooterEntityName[ zTAG_LTH ];
   ZLevelItem  *m_pNext;
};

class ZDRAPP_CLASS ZPageItem
{
public:
   ZPageItem( zLONG lPageNbr, zLONG lPosition, zLONG lRC,
              zLONG lLevel, zLONG lFlags, zLONG lMultiLineTextPos,
              zLONG lContinuedGroupFlag, zCPCHAR cpcEntityName,
              zCPCHAR cpcRptGroupSetTag, zCPCHAR cpcRptGroupTag );
   ~ZPageItem( );

public:
   zLONG       m_lPageNbr;
   zLONG       m_lPosition;
   zLONG       m_lRC;
   zLONG       m_lLevel;
   zLONG       m_lFlags;
   zLONG       m_lMultiLineTextPos;
   zLONG       m_lContinuedGroupFlag;
   zCHAR       m_szEntityName[ zTAG_LTH ];
   zCHAR       m_szRptGroupSetTag[ zTAG_LTH ];
   zCHAR       m_szRptGroupTag[ zTAG_LTH ];
   ZPageItem   *m_pNext;
   ZLevelItem  *m_pFooterHead;
   ZLevelItem  *m_pLastEntityHead;
};

ZPrintout::ZPrintout( ZSubtask *pZSubtask,
                      zCPCHAR  cpcDLL_Name,
                      zCPCHAR  cpcPrintPageFunction,
                      zLONG    lPrintFlags,
                      zSHORT   bBanding,
                      zSHORT   bScale,
                      zLONG    bPrompt,
                      zCPCHAR  cpcReportDefName,
                      zPCHAR   pchReportInfo,
                      zCPCHAR  cpcCSS_FileName ) :
           m_csReportDefName( cpcReportDefName ),
           m_csReportInfo( pchReportInfo )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPrintout ctor", "" );
#endif

   m_pZSubtask = pZSubtask;
   m_pZTask = m_pZSubtask->m_pZTask;
   m_vSubtask = pZSubtask->m_vDialog;
   m_pZSubtask->m_pZPrintout = this;
   m_pDC = 0;
   m_pPrintInfo = 0;
   m_pFontCurr = 0;
   m_hLibrary = 0;
   m_lpfnPrintPageRoutine = 0;
   m_nEscapementFactor = 1;
   m_bBanding = bBanding;
   m_lLastColumnCSV = 0;
   m_pResetPagingHead = 0;
   m_pResetPagingTail = 0;
// SetBanding( m_bBanding );

   m_bPrintPreview = FALSE;
   m_bPrintToPDF = (lPrintFlags & zREPORT_PRINT2PDF) ? TRUE : FALSE;
   m_bPrintToCSV = (lPrintFlags & zREPORT_PRINT2CSV) ? TRUE : FALSE;
   m_bCountPages = (lPrintFlags & zREPORT_COUNTPAGES) ? TRUE : FALSE;
   m_bPrintToHTML = (lPrintFlags & zREPORT_PRINT2HTML) ? TRUE : FALSE;
   m_bPrintToCtrl = (lPrintFlags & zREPORT_PRINT2CTRL) ? TRUE : FALSE;
   m_bResetPaging = (lPrintFlags & zREPORT_RESETPAGING) ? TRUE : FALSE;
   if ( m_bPrintToCSV || m_bPrintToHTML )
   {
      LPAPP lpApp;
      zCHAR szFileName[ zMAX_FILESPEC_LTH + 1 ];

      // We must get the directory that contains the ReportDef.  If we are
      // running under the Animator, we will get it from the LPLR.  If not,
      // we will get it using the Subtask.
      szFileName[ 0 ] = 0;
      if ( lPrintFlags & zREPORT_PRINTREMOTE )
      {
         SysMakeWebFileName( szFileName, zsizeof( szFileName ), m_vSubtask, 2 );
      }
      else
      {
         if ( lPrintFlags & zREPORT_USEFOLDER )
            strcpy_s( szFileName, zsizeof( szFileName ), pchReportInfo );

         if ( lPrintFlags & zREPORT_CREATENAME )
         {
            zCHAR szName[ 32 ];

            SysAppendcDirSep( szFileName );
            sprintf_s( szName, "0x%08x", (zULONG) pZSubtask->m_vDialog );
            strcat_s( szFileName, zsizeof( szFileName ), szName );
         }
         else
            strcat_s( szFileName, zsizeof( szFileName ), pchReportInfo );
      }

      if ( zstrchr( szFileName, '/' ) == 0 && zstrchr( szFileName, '\\' ) == 0 )
      {
         SfGetApplicationForSubtask( &lpApp, pZSubtask->m_vDialog );
         if ( lpApp )
         {
            zCHAR szTempName[ zMAX_FILESPEC_LTH + 1 ];

            strcpy_s( szTempName, zsizeof( szTempName ), szFileName );
            strcpy_s( szFileName, zsizeof( szFileName ), lpApp->szLocalDir );
            strcat_s( szFileName, zsizeof( szFileName ), szTempName );
         }
      }

      zstrlower( szFileName );
      if ( m_bPrintToCSV )
      {
         strcat_s( szFileName, zsizeof( szFileName ), ".csv" );
         TraceLineS( "Creating CSV File: ", szFileName );
      }
      else
      {
         strcat_s( szFileName, zsizeof( szFileName ), ".html" );
         TraceLineS( "Creating HTML File: ", szFileName );
      }

      m_csReportFileName = szFileName;
      m_lFileCSV = SysOpenFile( m_vSubtask, szFileName, COREFILE_WRITE );

      if ( m_bPrintToHTML && m_lFileCSV != -1 )
      {
         if ( lPrintFlags & zREPORT_RETURNNAME )
         {
            zPCHAR pch = zstrrchr( szFileName, '\\' );
            if ( pch == 0 )
               pch = zstrrchr( szFileName, '/' );

            if ( pch == 0 )
               pch = szFileName;
            else
               pch++;

            zLONG lLth = zstrlen( pchReportInfo ) + 1;
            strcpy_s( pchReportInfo, lLth, pch );
         }

         SysWriteFile( m_vSubtask, m_lFileCSV, "<html>\r\n", 8 );
         SysWriteFile( m_vSubtask, m_lFileCSV, "<head>\r\n", 8 );
         // SysWriteFile( m_vSubtask, m_lFileCSV, "<%@include file=\"myheader.inc\" %>\r\n", 35 );
         SysWriteFile( m_vSubtask, m_lFileCSV, "<title></title>", 15 );  // removed Test Title
         if ( cpcCSS_FileName && cpcCSS_FileName[ 0 ] )
         {
            CString cs( cpcCSS_FileName );

            cs = "<link rel=\"stylesheet\" type=\"text/css\" href=\"";
            cs += cpcCSS_FileName;
            cs += "\" />";
            SysWriteFile( m_vSubtask, m_lFileCSV, cs, cs.GetLength( ) );
         }

         SysWriteFile( m_vSubtask, m_lFileCSV, "</head>\r\n", 9 );
         SysWriteFile( m_vSubtask, m_lFileCSV, "<pre>", 5 );
      }

   // m_bPrintPreview = TRUE;
   }
   else
   {
      m_lFileCSV = -1;
      if ( m_bPrintToCtrl == FALSE )
         m_bPrintPreview = (lPrintFlags & zREPORT_PRINTPREVIEW) ? TRUE : FALSE;
   }

   m_bLandscape = (lPrintFlags & zREPORT_LANDSCAPE) ? TRUE : FALSE;
   m_bDropView = (lPrintFlags & zREPORT_DROPVIEW) ? TRUE : FALSE;
   m_bScale = bScale;
   m_bPrompt = bPrompt;
   m_bPhantom = FALSE;
   m_nPrevMode = 0;
// m_nMapMode = MM_ISOTROPIC;    // Respect aspect ratio of window
// m_nMapMode = MM_ANISOTROPIC;  // Make printout fill the page
   m_nFontMax = 20;
   m_nFontCnt = 0;
   m_uMaxPageNbr = 1;
   m_pFirstPage = 0;
   m_lPageCnt = 0;
   m_lWordCnt = 0;
   m_lCharCnt = 0;
   m_lHorzSize256ths = 0;
   m_lVertSize256ths = 0;
   m_dHeight = 0;

   zCHAR szTabStops[ 128 ];

   GetWorkstationApplicationValues( m_vSubtask, "PrintTabStops",
                                    szTabStops, zsizeof( szTabStops ),
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

   m_nTabStops = (zSHORT) zatol( szTabStops );
   if ( m_nTabStops <= 0 )
      m_nTabStops = 8;  // default number of characters per tab

   m_pTabStopHead = new ZNumberItem( m_nTabStops );
   m_pViewPortHead = 0;

   zLONG lLth = zstrlen( cpcDLL_Name ) + 1;
   m_pchDLL_Name = new char[ lLth ];
   strcpy_s( m_pchDLL_Name, lLth, cpcDLL_Name );
   lLth = zstrlen( cpcPrintPageFunction ) + 1;
   m_pchPrintPageFunction = new char[ lLth ];
   strcpy_s( m_pchPrintPageFunction, lLth, cpcPrintPageFunction );
   m_szDefaultPrinter[ 0 ] = 0;

   m_lpfnPrintPageRoutine = (zFARPROC_PRINT) GetOperationDynamicCallAddress( m_vSubtask, (LPLPLIBRARY) &m_hLibrary,
                                                                             m_pchDLL_Name, m_pchPrintPageFunction, "(print)" );
   if ( m_lpfnPrintPageRoutine )
   {
      if ( m_bPrintToCSV || m_bPrintToHTML )
      {
         CWindowDC dc( CWnd::GetDesktopWindow( ) );
         m_pDC = &dc;

         // Determine the page extent in 16ths of an inch.
         // HORZRES    - Width, in pixels, of the screen.
         // VERTRES    - Height, in raster lines, of the screen.
         // LOGPIXELSX - Number of pixels per logical inch along the screen width.
         // LOGPIXELSY - Number of pixels per logical inch along the screen height.
         m_lHorzRes = dc.GetDeviceCaps( HORZRES );
         m_lVertRes = dc.GetDeviceCaps( VERTRES );
         m_nPixelsPerInchX = dc.GetDeviceCaps( LOGPIXELSX );
         m_nPixelsPerInchY = dc.GetDeviceCaps( LOGPIXELSY );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask, -1, // flag for BeginPrint
                                      m_lHorzSize256ths, m_lVertSize256ths );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask, 1,  // only "1 page to print"
                                      m_lHorzSize256ths, m_lVertSize256ths );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask, -3, // flag for EndPrint
                                      m_lHorzSize256ths, m_lVertSize256ths );
         m_pDC = 0;
         m_pZSubtask->m_pZTask->m_uAppState &= ~zTASK_STATE_PRINT;
      }
      else
      {
#ifdef zREMOTE_SERVER
         m_bPrintToPDF = TRUE;
         SysMutexLock( vSubtask, "ZDxPrint", 0, 0 );
#else
         m_pWindow = m_pZSubtask->m_pZView;
#endif

         if ( lPrintFlags & 2 )
            m_pZTask->m_uAppState |= zTASK_STATE_CLOSE;
         else
            m_pZTask->m_uAppState &= ~zTASK_STATE_CLOSE;

         if ( lPrintFlags & 4 )
            m_pZTask->m_uAppState |= zTASK_STATE_PREPAGE;
         else
            m_pZTask->m_uAppState &= ~zTASK_STATE_PREPAGE;

         // Set up the font scale as specified in Zeidon.ini
         zCHAR  szFontData[ 128 ];

         GetWorkstationApplicationValues( m_vSubtask, "FontScale", szFontData, zsizeof( szFontData ),
                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );
#if 0
         if ( szFontData[ 0 ] )
            m_dHeight = atof( szFontData );
         else
            m_dHeight = 0;
#else
         if ( szFontData[ 0 ] )  // 2001.03.13    PAS
            TraceLineS( "FontScale has been moved to a printer specific section. ",
                        " It is no longer honored in the [Workstation] section." );
#endif

#ifdef zREMOTE_SERVER
         RemoteFilePrint( m_vSubtask );
         m_pZSubtask->m_pZPrintout = 0;
         delete( this );
#else
         if ( m_pZSubtask->m_pZView )
         {
            if ( m_bPrintPreview )
            {
               m_pZSubtask->m_pZView->SendMessage( WM_COMMAND, ID_FILE_PRINT_PREVIEW );
            }
            else
            if ( m_bPrompt )
            {
               if ( m_pZSubtask->m_pZView->SendMessage( WM_COMMAND, ID_FILE_PRINT ) )
               {
                  pZSubtask->m_pZTask->m_pZSubtaskPrint = 0;
                  pZSubtask->m_pZTask->m_uAppState &= ~zTASK_STATE_PRINT;
               }
            }
            else
               m_pZSubtask->m_pZView->SendMessage( WM_COMMAND, ID_FILE_PRINT_DIRECT );
         }
         else
         {
            ::MessageBox( 0, "Print View Unavailable in Conversational Dialog\n"
                             "Contact Zeidon Support",
                          "Zeidon Print Error", MB_OK );
         }
#endif
      }
   }
}

ZPrintout::ZPrintout( ZTask    *pZTask,
                      zCPCHAR  cpcDLL_Name,
                      zCPCHAR  cpcPrintPageFunction,
                      zLONG    lPrintFlags,
                      zSHORT   bBanding,
                      zSHORT   bScale,
                      zLONG    bPrompt,
                      zCPCHAR  cpcReportDefName,
                      zPCHAR   pchReportInfo,
                      zCPCHAR  cpcCSS_FileName ) :
           m_csReportDefName( cpcReportDefName ),
           m_csReportInfo( pchReportInfo )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPrintout alt ctor", "" );
#endif

   m_pZSubtask = 0;
   m_pZTask = pZTask;
   m_vSubtask = pZTask->m_vNewAppSubtask;
   m_pWindow = 0;
   m_pDC = 0;
   m_pPrintInfo = 0;
   m_pFontCurr = 0;
   m_hLibrary = 0;
   m_lpfnPrintPageRoutine = 0;
   m_nEscapementFactor = 1;
   m_bBanding = bBanding;
   m_lFileCSV = -1;
   m_lLastColumnCSV = 0;
   m_pResetPagingHead = 0;
   m_pResetPagingTail = 0;
// SetBanding( m_bBanding );

   m_bPrintPreview = FALSE;
   m_bPrintToPDF = (lPrintFlags & zREPORT_PRINT2PDF) ? TRUE : FALSE;
   m_bPrintToCSV = (lPrintFlags & zREPORT_PRINT2CSV) ? TRUE : FALSE;
   m_bPrintToHTML = (lPrintFlags & zREPORT_PRINT2HTML) ? TRUE : FALSE;
   m_bPrintPreview = (lPrintFlags & zREPORT_PRINTPREVIEW) ? TRUE : FALSE;
   m_bLandscape = (lPrintFlags & zREPORT_LANDSCAPE) ? TRUE : FALSE;
   m_bDropView = (lPrintFlags & zREPORT_DROPVIEW) ? TRUE : FALSE;
   m_bResetPaging = (lPrintFlags & zREPORT_RESETPAGING) ? TRUE : FALSE;
   m_bScale = bScale;
   m_bPrompt = bPrompt;
   m_bPhantom = FALSE;
   m_nPrevMode = 0;
// m_nMapMode = MM_ISOTROPIC;    // Respect aspect ratio of window
// m_nMapMode = MM_ANISOTROPIC;  // Make printout fill the page
   m_nFontMax = 20;
   m_nFontCnt = 0;
   m_uMaxPageNbr = 1;
   m_pFirstPage = 0;
   m_lPageCnt = 0;
   m_lWordCnt = 0;
   m_lCharCnt = 0;
   m_lHorzSize256ths = 0;
   m_lVertSize256ths = 0;
   m_dHeight = 0;

   zCHAR szTabStops[ 128 ];

   GetWorkstationApplicationValues( m_vSubtask, "PrintTabStops",
                                    szTabStops, zsizeof( szTabStops ),
                                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

   m_nTabStops = (zSHORT) zatol( szTabStops );
   if ( m_nTabStops <= 0 )
      m_nTabStops = 8;  // default number of characters per tab

   m_pTabStopHead = new ZNumberItem( m_nTabStops );
   m_pViewPortHead = 0;

   zLONG lLth = zstrlen( cpcDLL_Name ) + 1;
   m_pchDLL_Name = new char[ lLth ];
   strcpy_s( m_pchDLL_Name, lLth, cpcDLL_Name );
   lLth = zstrlen( cpcPrintPageFunction ) + 1;
   m_pchPrintPageFunction = new char[ lLth ];
   strcpy_s( m_pchPrintPageFunction, lLth, cpcPrintPageFunction );
   m_szDefaultPrinter[ 0 ] = 0;

   m_lpfnPrintPageRoutine = (zFARPROC_PRINT)
           GetOperationDynamicCallAddress( m_vSubtask,
                                           (LPLPLIBRARY) &m_hLibrary,
                                           m_pchDLL_Name,
                                           m_pchPrintPageFunction,
                                           "(print)" );
   if ( m_lpfnPrintPageRoutine )
   {
      if ( m_bPrintToCSV || m_bPrintToHTML )
      {
         CWindowDC dc( CWnd::GetDesktopWindow( ) );
         m_pDC = &dc;

         // Determine the page extent in 16ths of an inch.
         // HORZRES    - Width, in pixels, of the screen.
         // VERTRES    - Height, in raster lines, of the screen.
         // LOGPIXELSX - Number of pixels per logical inch along the screen width.
         // LOGPIXELSY - Number of pixels per logical inch along the screen height.
         m_lHorzRes = dc.GetDeviceCaps( HORZRES );
         m_lVertRes = dc.GetDeviceCaps( VERTRES );
         m_nPixelsPerInchX = dc.GetDeviceCaps( LOGPIXELSX );
         m_nPixelsPerInchY = dc.GetDeviceCaps( LOGPIXELSY );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask,
                                      -1, // flag for BeginPrint
                                      m_lHorzSize256ths,
                                      m_lVertSize256ths );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask,
                                      1,  // only "1 page to print"
                                      m_lHorzSize256ths,
                                      m_lVertSize256ths );

         (*(m_lpfnPrintPageRoutine))( m_vSubtask,
                                      -3, // flag for EndPrint
                                      m_lHorzSize256ths,
                                      m_lVertSize256ths );
         m_pDC = 0;
      }
      else
      {
         // Set up the font scale as specified in Zeidon.ini
         zCHAR  szFontData[ 128 ];

         GetWorkstationApplicationValues( m_vSubtask, "FontScale",
                                          szFontData, zsizeof( szFontData ),
                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0 );

#if 0
         if ( szFontData[ 0 ] )
            m_dHeight = atof( szFontData );
         else
            m_dHeight = 0;
#else
         if ( szFontData[ 0 ] )  // 2001.03.13    PAS
            TraceLineS( "FontScale has been moved to a printer specific section. ",
                        " It is no longer honored in the [Workstation] section." );
#endif
      }
   }
}

ZPrintout::~ZPrintout( )
{
#ifdef DEBUG_ALL
   TraceLineS( "ZPrintout dtor", "" );
#endif

   CloseReport( m_vSubtask, 1 );

   mDeleteInit( m_pFirstPage );
   mDeleteInit( m_pTabStopHead );
   mDeleteInit( m_pViewPortHead );
   mDeleteInit( m_pResetPagingHead );
   mDeleteInitA( m_pchDLL_Name );
   mDeleteInitA( m_pchPrintPageFunction );

// TraceLineI( "ZPrintout::dtor m_nFontCnt: ", m_nFontCnt );

   while ( m_FontList.m_pHeadNumber )
      RemoveFont( (ZFont *) m_FontList.m_pHeadNumber->m_lNumber );

// if ( m_hLibrary )                let object engine take care of this
//    SysFreeLibrary( m_vSubtask, m_hLibrary );

   if ( m_bPrintToPDF )
   {
      zCHAR  szDeviceFileName[ zMAX_FILENAME_LTH ];
      zCHAR  szReportFileName[ zMAX_FILENAME_LTH ];

      GetAppOrWorkstationValue( m_vSubtask, "AcrobatPDF_PathFileName",
                                szDeviceFileName,
                                zsizeof( szDeviceFileName ) );
      if ( szDeviceFileName[ 0 ] )
      {
         DWORD dwMilliSeconds = GetTickCount( );
         zCHAR  szBuffer[ 256 ];
         zLONG lFile;
         MSG   msg;

         do    // let Adobe have a chance to close the file
         {
            while ( ::PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )
            {
               ::TranslateMessage( &msg );
               ::DispatchMessage( &msg );
            }

            lFile = SysOpenFile( m_vSubtask,
                                 szDeviceFileName, COREFILE_READ );
            if ( lFile != -1 )
            {
               if ( SysReadFile( m_vSubtask, lFile,
                                 szBuffer, zsizeof( szBuffer ) ) > 0 )
               {
                  break;
               }
               else
               {
                  SysCloseFile( m_vSubtask, lFile, 0 );
               }
            }

            SysWait( 1000 );
            if ( GetTickCount( ) - dwMilliSeconds > 30000 )
               break;

         } while ( TRUE );

         if ( lFile == -1 )
         {
            TraceLineS( "PrintReportToPDF - Print time-out", "" );
         }
         else
         {
            SysCloseFile( m_vSubtask, lFile, 0 );
            StartBrowserWindowWithURL( m_vSubtask, szReportFileName,
                                       zsizeof( szReportFileName ),
                                       0, szDeviceFileName, 1 );
            SysOpenFile( m_vSubtask,
                         szDeviceFileName, COREFILE_DELETE );
            TraceLineS( "PrintReportToPDF printed to file: ",
                        szReportFileName );
         }
      }
      else
      {
         TraceLineS( "PrintReportToPDF printed to file: ",
                     m_csReportInfo );
      }

#ifdef zREMOTE_SERVER
      if ( m_pZSubtask )
      {
         mDeleteInit( m_pZSubtask->m_pZFWnd );
      }
#endif
   }

   zVIEW vReport;

   if ( GetViewByName( &vReport, "TZ_ReportDef", m_vSubtask, zLEVEL_ANY ) > 0 )
      DropView( vReport );

   if ( m_bDropView &&
        GetViewByName( &vReport, "TZ_SourceOrigOI", m_vSubtask, zLEVEL_ANY ) > 0 )
   {
      DropView( vReport );
   }

   if ( m_pZSubtask )
      m_pZSubtask->m_pZPrintout = 0;

   m_pZTask->m_vNewAppSubtask = 0;
   m_pZTask->m_pZSubtaskPrint = 0;
   m_pZTask->m_uAppState &= ~zTASK_STATE_PRINT;

   SysMutexUnlock( m_vSubtask, "ZDxPrint", 0 );
}

int
ZPrintout::RptDrawText( CDC *pDC, const CString& cs, LPRECT lpRect, UINT uFormat )
{
   if ( m_bPrintToCSV || m_bPrintToHTML || m_bPrintToPDF || pDC == 0 )
      return( 0 );

   if ( cs == "zTAG_LTHzTAG_LTHzTAG_LTH3" )
      TraceLineS( "RptDrawText: ", cs );

   return( pDC->DrawText( cs, lpRect, uFormat ) );
}

CPoint
ZPrintout::RptMoveTo( CDC *pDC, int x, int y )
{
   if ( m_bPrintToCSV || m_bPrintToHTML || m_bPrintToPDF || pDC == 0 )
      return( 0 );

   return( pDC->MoveTo( x, y ) );
}

BOOL
ZPrintout::RptLineTo( CDC *pDC, int x, int y )
{
   if ( m_bPrintToCSV || m_bPrintToHTML || m_bPrintToPDF || pDC == 0 )
      return( 0 );

   return( pDC->LineTo( x, y ) );
}

void
ZPrintout::AddFont( zVIEW  vSubtask, zCPCHAR cpcTag,
                    zLONG  nPtSize, zSHORT nWeight,
                    zSHORT bItalic, zSHORT bUnderline,
                    zSHORT bStrikeout, zLONG lTextColor,
                    zLONG lTextBkColor,
                    zSHORT nEscapement, zCPCHAR cpcFaceName )
{
#ifdef DEBUG_ALL
// TraceLineS( "ZPrintout::AddFont: ", cpcTag );
#endif

   // Scale the font as specified in Zeidon.ini
   if ( m_dHeight >= 0.1 || m_dHeight <= -0.1 )
      nPtSize = (zSHORT) ((double) nPtSize * m_dHeight);

   if ( nPtSize < 0 )
      nPtSize = -nPtSize;

   zSHORT nFlags = 0;
   if ( bItalic )
      nFlags |= 1;

   if ( bUnderline )
      nFlags |= 2;

   if ( bStrikeout )
      nFlags |= 16;

   if ( nWeight >= FW_BOLD )
      nFlags |= 32;

   if ( cpcFaceName == 0 || *cpcFaceName == 0 )
      cpcFaceName = "Arial";

   ZFont *pFont = LocateFont( nPtSize, nWeight, nFlags,
                              nEscapement, cpcFaceName );
   if ( pFont )
   {
      pFont->m_clrText = lTextColor;
      ZFont *pFontOld = LocateFont( cpcTag );
      if ( pFont != pFontOld )
      {
         if ( pFontOld )
         {
            ZNameItem *pItem = pFontOld->m_TagList.Remove( cpcTag );
            mDeleteInit( pItem );
         }

         pFont->m_TagList.AddTail( cpcTag );
      }

      m_FontList.MoveHead( (zLONG) pFont );
      return;
   }

   pFont = LocateFont( cpcTag );
   if ( pFont )
   {
      ZNameItem *pItem = pFont->m_TagList.Remove( cpcTag );
      mDeleteInit( pItem );
      RemoveFont( pFont );
   }

   pFont = new ZFont( vSubtask, cpcTag, nPtSize,
                   // -((nSize * m_pDC->GetDeviceCaps( LOGPIXELSY )) / 72),
                      nWeight, bItalic, bUnderline, bStrikeout,
                      lTextColor, lTextBkColor, nEscapement, cpcFaceName );

// LOGFONT LogFont;
// pFont->GetLogFont( &LogFont );

   m_nFontCnt++;
   pFont->m_nIdNbr = m_nFontCnt;
   m_FontList.AddTail( (zLONG) pFont );
   m_FontList.MoveHead( (zLONG) pFont );
// if ( m_nFontCnt > m_nFontMax )
// {
//    m_nFontCnt--;
//    pFont = m_FontList.DropTail( );
//    delete( pFont );
//    TraceLineI( "ZPrintout::AddFont DropTail cnt: ", m_nFontCnt );
// }
}

zBOOL
ZPrintout::RemoveFont( ZFont *pFont )
{
   zBOOL bRC = FALSE;
   ZNumberItem *pItem = m_FontList.Remove( (zLONG) pFont );
   if ( pItem )
   {
      ZFont *pFontRemove = (ZFont *) pItem->m_lNumber;
      if ( pFontRemove == pFont )
      {
         m_nFontCnt--;
         bRC = TRUE;
      }

      if ( pFont->DeleteObject( ) == 0 )
         TraceLineS( "ZPrintout::RemoveFont cannot delete font", "" );

      mDeleteInit( pFontRemove );
      delete( pItem );
   }

// TraceLineI( "ZPrintout::RemoveFont cnt: ", m_nFontCnt );
   return( bRC );
}

ZFont *
ZPrintout::LocateFont( zCPCHAR cpcTag )
{
#ifdef DEBUG_ALL
// TraceLineS( "ZPrintout::LocateFont: ", cpcTag );
#endif

   ZNumberItem *pItem = m_FontList.m_pHeadNumber;
   ZFont *pFont;

   while ( pItem )
   {
      pFont = (ZFont *) pItem->m_lNumber;
      if ( pFont->m_TagList.Find( cpcTag ) )
         return( pFont );

      pItem = pItem->m_pNextNumber;
   }

#ifdef DEBUG_ALL
   TraceLineS( "ZPrintout::LocateFont not found: ", cpcTag );
#endif

   return( 0 );
}

ZFont *
ZPrintout::LocateFont( zLONG  nPtSize, zSHORT nWeight,
                       zSHORT nFlags, zSHORT nEscapement,
                       zCPCHAR cpcFaceName )
{
   ZNumberItem *pItem = m_FontList.m_pHeadNumber;
   ZFont *pFont;

   while ( pItem )
   {
      pFont = (ZFont *) pItem->m_lNumber;
      if ( pFont->m_nPointSize == nPtSize &&
           pFont->m_nWeight == nWeight &&
           pFont->m_nFlags == nFlags &&
           pFont->m_nEscapement == nEscapement &&
           zstrcmp( pFont->m_csFaceName.GetString(), cpcFaceName ) == 0 )
      {
         return( pFont );
      }

      pItem = pItem->m_pNextNumber;
   }

   return( 0 );
}

#define RGB_GETRED( rgb )      (((rgb) >> 16) & 0xff)
#define RGB_GETGREEN( rgb )    (((rgb) >> 8) & 0xff)
#define RGB_GETBLUE( rgb )     ((rgb) & 0xff)

zSHORT
ZPrintout::ListFontsAndColors( zPCHAR pchFontList, zLONG lMaxLth )
{
#ifdef DEBUG_ALL
   TraceLineI( "ZPrintout::ListFontsAndColors count:", m_nFontCnt );
#endif

// {\f0\froman\fcharset0\fprq2{\*\panose 02020603050405020304}Times New Roman;}
// {\f2\fmodern\fcharset0\fprq1{\*\panose 02070309020205020404}Courier New;}
// {\f28\fnil\fcharset0\fprq2{\*\panose 00000000000000000000}PT Cozy Outline;}

   zSHORT nLth = 0;
   zSHORT nFontCnt = 0;

   pchFontList[ 0 ] = 0;  // initialize to null
   if ( m_nFontCnt > 0 )
   {
      ZNumberItem *pItem = m_FontList.m_pHeadNumber;
      ZFont *pFont;
      ZNumberList listColors;
      listColors.AddTail( 0 );

      strcpy_s( pchFontList + nLth, lMaxLth - nLth, "{\\fonttbl" );
      nLth += 9;

      while ( pItem && nLth < lMaxLth - 1024 )
      {
         pFont = (ZFont *) pItem->m_lNumber;
         if ( pFont->m_clrText )
            listColors.AddTail( pFont->m_clrText );

         if ( pFont->m_bSet )
         {
         // zSHORT j = nLth;
            nFontCnt++;
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\r\n{\\f" );
            nLth += 5;
            _ltoa_s( pFont->m_nIdNbr, pchFontList + nLth, lMaxLth - nLth, 10 );
            nLth += (zSHORT) zstrlen( pchFontList + nLth );
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\\f" );
            nLth += 2;
            switch ( pFont->m_byPitchFamily >> 4 )        // family
            {
               case FF_DECORATIVE:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "decorative" );
                  nLth += zsizeof( "decorative" );
                  break;

               case FF_MODERN:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "modern" );
                  nLth += zsizeof( "modern" );
                  break;

               case FF_ROMAN:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "roman" );
                  nLth += zsizeof( "roman" );
                  break;

               case FF_SCRIPT:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "script" );
                  nLth += zsizeof( "script" );
                  break;

               case FF_SWISS:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "swiss" );
                  nLth += zsizeof( "swiss" );
                  break;

               case FF_DONTCARE:
               default:
                  strcpy_s( pchFontList + nLth, lMaxLth - nLth, "nil" );
                  nLth += zsizeof( "nil" );
                  break;
            }

            strcpy_s( pchFontList + nLth - 1, lMaxLth - nLth + 1, "\\fcharset" ); // charset
            nLth += 8;
            _ltoa_s( pFont->m_byCharset, pchFontList + nLth, lMaxLth - nLth, 10 );
            nLth += (zSHORT) zstrlen( pchFontList + nLth );
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\\fprq" );    // pitch
            nLth += 5;
            _ltoa_s( (pFont->m_byPitchFamily & 0x0F), pchFontList + nLth, lMaxLth - nLth, 10 );
            nLth += (zSHORT) zstrlen( pchFontList + nLth );
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, "{\\*\\panose " );
            nLth += 11;
#if 0
            sprintf_s( pchFontList + nLth,
                      "%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x%02.2x}",
                      pFont->m_Panose.bFamilyType,
                      pFont->m_Panose.bSerifStyle,
                      pFont->m_Panose.bWeight,
                      pFont->m_Panose.bProportion,
                      pFont->m_Panose.bContrast,
                      pFont->m_Panose.bStrokeVariation,
                      pFont->m_Panose.bArmStyle,
                      pFont->m_Panose.bLetterform,
                      pFont->m_Panose.bMidline,
                      pFont->m_Panose.bXHeight );
            nLth += 21;
#else
            BYTE   byte;
            zCHAR  ch;
            zSHORT k;

            for ( k = 0; k < 10; k++ )
            {
            // TraceLineI( "nLth = ", nLth );
               byte = *(((BYTE *) &(pFont->m_Panose)) + k);
               ch = '0' + (char) (byte / 10);
               pchFontList[ nLth ] = ch;
               nLth++;
            // pchFontList[ nLth ] = 0;
               ch = '0' + (char) (byte % 10);
               pchFontList[ nLth ] = ch;
               nLth++;
            // pchFontList[ nLth ] = 0;
            }

            strcpy_s( pchFontList + nLth, lMaxLth - nLth, "}" );
            nLth++;
#endif
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, pFont->m_csFaceName );
            nLth += (zSHORT) zstrlen( pchFontList + nLth );
            strcpy_s( pchFontList + nLth, lMaxLth - nLth, ";}" );
            nLth += 2;
         // TraceLineS( pchFontList + j, "" );
         }
      }

      strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\r\n}" );
      nLth += 3;

      // List colors ... \red0\green0\blue0;
      strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\r\n{\\colortbl;" );
      nLth += 13;

      COLORREF cr;
      zLONG    lColor;

      pItem = listColors.m_pHeadNumber;
      while ( pItem && nLth < lMaxLth - 100 )
      {
         strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\r\n" );
         nLth += 2;

         cr = pItem->m_lNumber;
         strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\\red" );
         nLth += 4;
         lColor = RGB_GETRED( cr );
         _ltoa_s( lColor, pchFontList + nLth, lMaxLth - nLth, 10 );
         nLth += (zSHORT) zstrlen( pchFontList + nLth );

         strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\\green" );
         nLth += 6;
         lColor = RGB_GETGREEN( cr );
         _ltoa_s( lColor, pchFontList + nLth, lMaxLth - nLth, 10 );
         nLth += (zSHORT) zstrlen( pchFontList + nLth );

         strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\\blue" );
         nLth += 5;
         lColor = RGB_GETBLUE( cr );
         _ltoa_s( lColor, pchFontList + nLth, lMaxLth - nLth, 10 );
         nLth += (zSHORT) zstrlen( pchFontList + nLth );

         strcpy_s( pchFontList + nLth, lMaxLth - nLth, ";" );
         nLth += 1;

         pItem = pItem->m_pNextNumber;
      }

      strcpy_s( pchFontList + nLth, lMaxLth - nLth, "\r\n}" );
      nLth += 3;
   }

   return( nFontCnt );
}

zLONG
ZPrintout::HandlePageNumbering( zPCHAR pchPageNumber,
                                zLONG  lEntityKey,
                                zLONG  lPageNbr,
                                zPLONG plTotalPages,
                                zLONG  lFlags )
{
   ZPairedItem *pResetPagingItem;

   *pchPageNumber = 0;
   if ( lFlags & 0x80000000 )  // doing phantom print for page counting
   {
      if ( m_bResetPaging && lPageNbr > 0 )
      {
         pResetPagingItem = new ZPairedItem( lEntityKey, lPageNbr, 1 );
         if ( m_pResetPagingHead == 0 )
         {
            m_pResetPagingHead = pResetPagingItem;
            m_pResetPagingTail = m_pResetPagingHead;
         }
         else
         {
            ZPairedItem *pResetPagingTemp = m_pResetPagingHead;

            while ( pResetPagingTemp )
            {
               if ( pResetPagingTemp->m_lValue2 == lPageNbr )
                  return( 0 );  // we've already set up this page

               pResetPagingTemp = pResetPagingTemp->m_pNextItem;
            }

            m_pResetPagingTail->m_pNextItem = pResetPagingItem;
            if ( lEntityKey == 0 ||
                 lEntityKey == m_pResetPagingTail->m_lValue1 )
            {
               pResetPagingItem->m_lValue1 = m_pResetPagingTail->m_lValue1;
               pResetPagingItem->m_lValue3 = m_pResetPagingTail->m_lValue3 + 1;
            }

            m_pResetPagingTail = pResetPagingItem;
         }
      }

      return( 0 );
   }
   else
   if ( lPageNbr && (lFlags & 0x40000000) )  // set up "PageNumber"
   {
      if ( *plTotalPages >= 0 )
      {
         zCHAR szTotalPageSeparator[ 32 ];

         SysReadZeidonIni( -1, "[Workstation]", "TotalPageSeparator", szTotalPageSeparator, zsizeof( szTotalPageSeparator ) );
         if ( szTotalPageSeparator[ 0 ] == 0 )
            strcpy_s( szTotalPageSeparator, zsizeof( szTotalPageSeparator ), "of" );

         pResetPagingItem = m_pResetPagingHead;

         while ( pResetPagingItem )
         {
            if ( pResetPagingItem->m_lValue2 == lPageNbr )
            {
               lPageNbr = pResetPagingItem->m_lValue3;
               *plTotalPages = lPageNbr;
               while ( pResetPagingItem->m_pNextItem && pResetPagingItem->m_lValue1 == pResetPagingItem->m_pNextItem->m_lValue1 )
               {
                  *plTotalPages = pResetPagingItem->m_pNextItem->m_lValue3;
                  pResetPagingItem = pResetPagingItem->m_pNextItem;
               }

               break;  // out of outside loop
            }

            pResetPagingItem = pResetPagingItem->m_pNextItem;
         }

         sprintf_s( pchPageNumber, 32, "%d %s %d", lPageNbr, szTotalPageSeparator, *plTotalPages );
      }
      else
      {
         _ltoa_s( lPageNbr, pchPageNumber, 32, 10 );
      }

      return( lPageNbr );
   }

   return( -2 );
}

zBOOL
ZPrintout::SetPhantomMode( zBOOL bPhantom )
{
   zBOOL bRC = m_bPhantom;
   m_bPhantom = bPhantom;
   return( bRC );
}

zBOOL
ZPrintout::GetPhantomMode( )
{
   return( m_bPhantom );
}

zSHORT
ZPrintout::AddPagePosition( zLONG   lPageNbr,
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
   if ( lPageNbr == 0 )  // clear page position entities
   {
      mDeleteInit( m_pFirstPage );
      return( 1 );
   }

   ZLevelItem *pLevelItem  = 0;
   ZLevelItem *pLevelNext  = 0;
   zVIEW   vFooterHierOI   = 0;
   zLONG   lCurrLevel      = 0;
   zULONG  ulFooterPos     = 0;
   zUSHORT uReturnLevel    = 0;
   zSHORT  nRC             = 0;
   zCHAR   szEntityName[ zTAG_LTH ];
   zCHAR   szFooterEntityName[ zTAG_LTH ];

   szEntityName[ 0 ] = 0;
   szFooterEntityName[ 0 ] = 0;

#if 0
   if ( lPageNbr == 3 || lPageNbr == 4 || lPageNbr == 5 )
   {
      TraceLineI( "AddPagePosition adding page: ", lPageNbr );
      TraceLineS( "       =======> HierEntityName: ", cpcEntityName );
      TraceLineI( "       ==========> AbsolutePos: ", lPagePosition );
      TraceLineI( "       = MultiLineTextPosition: ", lMultiLineTextPos );
      DisplayEntityInstance( vSourceHierOI, cpcEntityName );
      TraceLineS( "       =======> RptGroupSetTag: ", cpcRptGroupSetTag );
      TraceLineS( "__________________ RptGroupTag: ", cpcRptGroupTag );
   }
#endif

   ZPageItem *pPage = new ZPageItem( lPageNbr, lPagePosition, lRC,
                                     lLevel, lFlags, lMultiLineTextPos,
                                     lContinuedGroupFlag, cpcEntityName,
                                     cpcRptGroupSetTag, cpcRptGroupTag );

#if 0 // this would be required except for the fact that we are using
      // the phantom print stuff to reposition properly.

   // Create FooterAtLevel and LastEntityAtLevel items for each entity
   // in the report definition instance.
   if ( CheckExistenceOfEntity( vReportDef, "FooterAtLevel" ) &&
        (nRC = SetCursorFirstEntity( vReportDef,
                                     "FooterAtLevel", 0 )) >= zCURSOR_SET )
   {
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( (zPLONG) &vFooterHierOI, vReportDef,
                                  "FooterAtLevel", "vSourceOI_Hier" );
         if ( vFooterHierOI )
         {
            GetEntityNameForHierarchicalCsr( &uReturnLevel,
                                             szFooterEntityName,
                                             &ulFooterPos,
                                             vFooterHierOI );
            DropView( vFooterHierOI );
         }
         else
         {
            szFooterEntityName[ 0 ] = 0;
            ulFooterPos = 0;
         }

         GetIntegerFromAttribute( &lCurrLevel, vReportDef,
                                 "FooterAtLevel", "Level" );
         GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vReportDef,
                                 "FooterAtLevel", "EntityName" );
         pLevelItem = new ZLevelItem( lCurrLevel, szEntityName,
                                      ulFooterPos, szFooterEntityName );
         if ( pPage->m_pFooterHead == 0 )
         {
            pPage->m_pFooterHead = pLevelItem;
         }
         else
         {
            pLevelNext->m_pNext = pLevelItem;
         }

         pLevelNext = pLevelItem;
         nRC = SetCursorNextEntity( vReportDef, "FooterAtLevel", 0 );
      }
   }

   if ( CheckExistenceOfEntity( vReportDef, "FooterAtLevel" ) &&
        (nRC = SetCursorFirstEntity( vReportDef, "LastEntityAtLevel", 0 )) >= zCURSOR_SET )
   {
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lCurrLevel, vReportDef, "LastEntityAtLevel", "Level" );
         GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vReportDef, "LastEntityAtLevel", "EntityName" );
         pLevelItem = new ZLevelItem( lCurrLevel, szEntityName );
         if ( pPage->m_pLastEntityHead == 0 )
         {
            pPage->m_pLastEntityHead = pLevelItem;
         }
         else
         {
            pLevelNext->m_pNext = pLevelItem;
         }

         pLevelNext = pLevelItem;
         nRC = SetCursorNextEntity( vReportDef, "LastEntityAtLevel", 0 );
      }
   }

   pLevelItem = pPage->m_pFooterHead;
   while ( pLevelItem )
   {
      CreateEntity( vReportDef, "FooterAtLevel", zPOS_NEXT );
      SetAttributeFromInteger( vReportDef, "FooterAtLevel", Level", pLevelItem->m_lLevel );
      SetAttributeFromString( vReportDef, "FooterAtLevel", "EntityName", pLevelItem->m_szEntityName );
      pLevelItem = pLevelItem->m_pNext;
   }

   pLevelItem = pPage->m_pLastEntityHead;
   while ( pLevelItem )
   {
      CreateEntity( vReportDef, "LastEntityAtLevel", zPOS_NEXT );
      SetAttributeFromInteger( vReportDef, "LastEntityAtLevel", "Level", pLevelItem->m_lLevel );
      SetAttributeFromString( vReportDef, "LastEntityAtLevel", "EntityName", pLevelItem->m_szEntityName );
      pLevelItem = pLevelItem->m_pNext;
   }
#endif

   if ( lPageNbr == 1 )
   {
      // Delete all FooterAtLevel and LastEntityAtLevel entities in the
      // report definition instance.
      nRC = SetCursorFirstEntity( vReportDef, "FooterAtLevel", 0 );
      while ( nRC >= zCURSOR_SET )
         nRC = DeleteEntity( vReportDef, "FooterAtLevel", zREPOS_FIRST );

      mDeleteInit( m_pFirstPage );

      m_pFirstPage = pPage;
      return( 0 );
   }

   ZPageItem *pPageNext = m_pFirstPage;
   zLONG k = lPageNbr - 2;
   while ( k-- > 0 )
   {
      if ( pPageNext == 0 )
         break;

      pPageNext = pPageNext->m_pNext;
   }

   if ( pPageNext )
   {
      mDeleteInit( pPageNext->m_pNext );
      pPageNext->m_pNext = pPage;
      return( 0 );
   }

   mDeleteInit( pPage );
// TraceLineI( "AddPagePosition attempt to add non-sequential page: ",
//             lPageNbr );
   return( 1 );
}

zSHORT
ZPrintout::GetPagePosition( zLONG   lPageNbr,
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
// TraceLineI( "GetPagePosition looking for page: ", lPageNbr );
   ZPageItem *pPage = m_pFirstPage;
   zLONG k = lPageNbr - 1;
   while ( k-- > 0 )
   {
      if ( pPage == 0 )
         break;

      pPage = pPage->m_pNext;
   }

   if ( pPage )
   {
      zVIEW   vFooterHierOI = 0;

      *plPagePosition = pPage->m_lPosition;
      *plRC = pPage->m_lRC;
      *plLevel = pPage->m_lLevel;
      *plFlags = pPage->m_lFlags;
      *plMultiLineTextPos = pPage->m_lMultiLineTextPos;
      *plContinuedGroupFlag = pPage->m_lContinuedGroupFlag;
      strcpy_s( pchEntityName, zTAG_LTH, pPage->m_szEntityName );

#if 0 // this would be required except for the fact that we are using
      // the phantom print stuff to reposition properly.

      // Create FooterAtLevel and LastEntityAtLevel entities in the report
      // definition instance.
      zSHORT nRC = SetCursorFirstEntity( vReportDef, "FooterAtLevel", 0 );
      while ( nRC >= zCURSOR_SET )
         nRC = DeleteEntity( vReportDef, "FooterAtLevel", zREPOS_FIRST );

      nRC = SetCursorFirstEntity( vReportDef, "LastEntityAtLevel", 0 );
      while ( nRC >= zCURSOR_SET )
         nRC = DeleteEntity( vReportDef, "LastEntityAtLevel", zREPOS_FIRST );

      ZLevelItem *pLevelItem = pPage->m_pFooterHead;
      while ( pLevelItem )
      {
         CreateEntity( vReportDef, "FooterAtLevel", zPOS_NEXT );
         SetAttributeFromInteger( vReportDef, "FooterAtLevel", "Level", pLevelItem->m_lLevel );
         SetAttributeFromString( vReportDef, "FooterAtLevel", "EntityName", pLevelItem->m_szEntityName );

         if ( pLevelItem->m_szFooterEntityName[ 0 ] )
         {
            CreateViewFromViewForTask( &vFooterHierOI, vSourceHierOI, m_pZSubtask->m_vDialog );
            SetCursorAbsolutePosition( pLevelItem->m_szFooterEntityName, vFooterHierOI, pLevelItem->m_lFooterPos );
            SetAttributeFromInteger( vReportDef, "FooterAtLevel", "vSourceOI_Hier", (zLONG) vFooterHierOI );
         }

         pLevelItem = pLevelItem->m_pNext;
      }

      pLevelItem = pPage->m_pLastEntityHead;
      while ( pLevelItem )
      {
         CreateEntity( vReportDef, "LastEntityAtLevel", zPOS_NEXT );
         SetAttributeFromInteger( vReportDef, "LastEntityAtLevel", "Level", pLevelItem->m_lLevel );
         SetAttributeFromString( vReportDef, "LastEntityAtLevel", "EntityName", pLevelItem->m_szEntityName );
         pLevelItem = pLevelItem->m_pNext;
      }
#endif

#if 0
      if ( lPageNbr == 3 || lPageNbr == 4 || lPageNbr == 5 )
      {
         TraceLineI( "GetPagePosition returning page: ", pPage->m_lPageNbr );
         TraceLineS( "       =======> HierEntityName: ", pPage->m_szEntityName );
         TraceLineI( "       ==========> AbsolutePos: ", pPage->m_lPosition );
         TraceLineI( "       = MultiLineTextPosition: ", pPage->m_lMultiLineTextPos );
         DisplayEntityInstance( vSourceHierOI, pPage->m_szEntityName );
         TraceLineS( "       =======> RptGroupSetTag: ", pPage->m_szRptGroupSetTag );
         TraceLineS( "__________________ RptGroupTag: ", pPage->m_szRptGroupTag );
      }
#endif

      return( 0 );
   }
   else
   {
//    TraceLineI( "GetPagePosition cannot locate page: ", lPageNbr );
      return( 1 );
   }
}

// Returns previous TabStop setting.
zSHORT
ZPrintout::PushTabStops( zSHORT nTabStop )
{
   if ( nTabStop > 0 )
   {
      ZNumberItem *pTabStop = m_pTabStopHead;
      while ( pTabStop->m_pNextNumber )
         pTabStop = pTabStop->m_pNextNumber;

      pTabStop->m_pNextNumber = new ZNumberItem( nTabStop );
      m_nTabStops = nTabStop;
   }

   return( m_nTabStops );
}

// Returns newly set (popped) TabStop setting.
zSHORT
ZPrintout::PopTabStops( )
{
   ZNumberItem *pTabStop = m_pTabStopHead;
   m_nTabStops = (zSHORT) pTabStop->m_lNumber;
   while ( pTabStop->m_pNextNumber && pTabStop->m_pNextNumber->m_pNextNumber )
   {
      m_nTabStops = (zSHORT) pTabStop->m_lNumber;
      pTabStop = pTabStop->m_pNextNumber;
   }

   mDeleteInit( pTabStop->m_pNextNumber );
   return( m_nTabStops );
}

ZViewPort::ZViewPort( zCPCHAR cpcCurrentEntityName,
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
   strcpy_s( m_szCurrentEntityName, zsizeof( m_szCurrentEntityName ), cpcCurrentEntityName );
   m_lRepeatHorizontal = lRepeatHorizontal;
   m_lRepeatHorizontalColumn = lRepeatHorizontalColumn;
   m_lRepeatHorizontalIndent = lRepeatHorizontalIndent;
   m_lRepeatHorizontalMargin = lRepeatHorizontalMargin;
   m_lRepeatHorizontalExtent = lRepeatHorizontalExtent;
   m_lRepeatHorizontalFlags = lRepeatHorizontalFlags;
   m_lViewPortBottomPrev = lViewPortBottomPrev;
   m_lViewPortTop = lViewPortTop;
   m_lViewPortLeft = lViewPortLeft;
   m_lViewPortBottom = lViewPortBottom;
   m_lViewPortRight = lViewPortRight;
   m_pViewPortNext = 0;
}

ZViewPort::~ZViewPort( )
{
   mDeleteInit( m_pViewPortNext );
}

// Pushes new ViewPort setting onto stack.
zLONG
ZPrintout::PushViewPort( zCPCHAR cpcCurrentEntityName,
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
   ZViewPort *pViewPort = new ZViewPort( cpcCurrentEntityName,
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
                                         lViewPortRight );

   if ( m_pViewPortHead  )
      pViewPort->m_pViewPortNext = m_pViewPortHead;
   else
      m_pViewPortHead = pViewPort;

   return( 0 );
}

// Returns popped ViewPort setting.
zLONG
ZPrintout::PopViewPort( zPCHAR  pchCurrentEntityName,
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
   ZViewPort *pViewPort = m_pViewPortHead;
   if ( m_pViewPortHead )
   {
      if ( m_pViewPortHead->m_pViewPortNext )
         m_pViewPortHead = m_pViewPortHead->m_pViewPortNext;

      strcpy_s( pchCurrentEntityName, zTAG_LTH, pViewPort->m_szCurrentEntityName );
      *plRepeatHorizontal = pViewPort->m_lRepeatHorizontal;
      *plRepeatHorizontalColumn = pViewPort->m_lRepeatHorizontalColumn;
      *plRepeatHorizontalIndent = pViewPort->m_lRepeatHorizontalIndent;
      *plRepeatHorizontalMargin = pViewPort->m_lRepeatHorizontalMargin;
      *plRepeatHorizontalExtent = pViewPort->m_lRepeatHorizontalExtent;
      *plRepeatHorizontalFlags = pViewPort->m_lRepeatHorizontalFlags;
      *plViewPortBottomPrev = pViewPort->m_lViewPortBottomPrev;
      *plViewPortTop = pViewPort->m_lViewPortTop;
      *plViewPortLeft = pViewPort->m_lViewPortLeft;
      *plViewPortBottom = pViewPort->m_lViewPortBottom;
      *plViewPortRight = pViewPort->m_lViewPortRight;

      pViewPort->m_pViewPortNext = 0;
      if ( pViewPort == m_pViewPortHead )
         m_pViewPortHead = 0;

      mDeleteInit( pViewPort );
      return( 1 );
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

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// class ZPageItem : simple single-linked list of pages
//
ZPageItem::ZPageItem( zLONG lPageNbr, zLONG lPosition, zLONG lRC,
                      zLONG lLevel, zLONG lFlags, zLONG lMultiLineTextPos,
                      zLONG lContinuedGroupFlag, zCPCHAR cpcEntityName,
                      zCPCHAR cpcRptGroupSetTag, zCPCHAR cpcRptGroupTag )
{
   m_lPageNbr = lPageNbr;
   m_lPosition = lPosition;
   m_lRC = lRC;
   m_lLevel = lLevel;
   m_lFlags = lFlags;
   m_lMultiLineTextPos = lMultiLineTextPos;
   m_lContinuedGroupFlag = lContinuedGroupFlag;
   strcpy_s( m_szEntityName, zsizeof( m_szEntityName ), cpcEntityName );
   strcpy_s( m_szRptGroupSetTag, zsizeof( m_szRptGroupSetTag ), cpcRptGroupSetTag );
   strcpy_s( m_szRptGroupTag, zsizeof( m_szRptGroupTag ), cpcRptGroupTag );
   m_pNext = 0;
   m_pFooterHead = 0;
   m_pLastEntityHead = 0;
}

ZPageItem::~ZPageItem( )
{
   mDeleteInit( m_pNext );
   mDeleteInit( m_pFooterHead );
   mDeleteInit( m_pLastEntityHead );
}

ZLevelItem::ZLevelItem( zLONG lLevel, zCPCHAR cpcEntityName,
                        zLONG lFooterPos, zCPCHAR cpcFooterEntityName )
{
   m_lLevel = lLevel;
   strcpy_s( m_szEntityName, zsizeof( m_szEntityName ), cpcEntityName );
   m_lFooterPos = lFooterPos;
   if ( cpcFooterEntityName && *cpcFooterEntityName )
      strcpy_s( m_szFooterEntityName, zsizeof( m_szFooterEntityName ), m_szFooterEntityName );
   else
      m_szFooterEntityName[ 0 ] = 0;

   m_pNext = 0;
}

ZLevelItem::~ZLevelItem( )
{
   mDeleteInit( m_pNext );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT zSHORT OPERATION
_zCall_PrintReportToPDF( zVIEW  vArgs,
                         zPVIEW pvViewList[] )
{
   zVIEW vZDrSystem;
   HWND  hWndRemote;

   if ( GetViewByName( &vZDrSystem, "__ZDrSystem",
                       *pvViewList[ 0 ], zLEVEL_SYSTEM ) > 0 )
   {
      GetIntegerFromAttribute( (zPLONG) &hWndRemote, vZDrSystem,
                               "System", "hWndRemote" );
      if ( hWndRemote )
      {
         zPCHAR pchViewNameList;

         // Get value for AppName
         SetCursorFirstEntity( vArgs, "Argument", 0 );
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

         // Get value for SubobjectEntityName
         SetCursorNextEntity( vArgs, "Argument", 0 );
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

         // Get value for ReportDefName
         SetCursorNextEntity( vArgs, "Argument", 0 );
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

         // Get value for ReportName
         SetCursorNextEntity( vArgs, "Argument", 0 );
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

         // Get value for Flags
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );
         SetCursorNextEntity( vArgs, "Argument", 0 );

         // Get value for ViewNameList
         SetCursorNextEntity( vArgs, "Argument", 0 );
         GetAddrForAttribute( &pchViewNameList, vArgs, "Argument", "Value" );

         // Now go through the list of views and count them.
         zSHORT nCnt = 0;
         zPCHAR pch = (zPCHAR) pchViewNameList;

         while ( pch && *pch )
         {
            // Get value for data view (ignore the value).
            SetCursorNextEntity( vArgs, "Argument", 0 );

            pch = zstrchr( pch, ';' );
            nCnt++;
            if ( pch )
               pch++;
         }

         zPVOID hvg = GlobalAlloc( GPTR, nCnt * sizeof( zVIEW ) );

         if ( SfAllocTaskMemory( vArgs, (zCOREMEM) &hvg,
                                 nCnt * sizeof( zVIEW ) ) == 0 )
         {
            zPVIEW pvg = (zPVIEW) SfLockTaskMemory( hvg );

            // Now go through the list of views and convert them to handles.
            while ( nCnt-- )
            {
               pvg[ nCnt ] =
                    (zVIEW) SysGetHandleFromPointer( *pvViewList[ nCnt ] );
            }

            SfUnlockTaskMemory( pvg );

            // Convert the view to handle to be reconverted on other end of
            // SendMessage.
            vArgs = (zVIEW) SysGetHandleFromPointer( vArgs );
            zSHORT nRC = (zSHORT) ::SendMessage( hWndRemote,
                                                 zZEIDON_REMOTE_PRINT_REPORT,
                                                 (WPARAM) vArgs,
                                                 (LPARAM) hvg );
            SfFreeTaskMemory( hvg );
         }
         else
            return( -2 );
      }
   }

   return( -1 );
}

zVIEW
ActivateReportDef( zVIEW   vSubtask,
                   zCPCHAR cpcReportDefName )
{
   zVIEW  vReportDef;
   zVIEW  vAnimator;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   zSHORT nRC;

   // Use the ReportDefName passed to Activate the Report Def and
   // pass it to the FormatSubobjectForDef object operation.  This
   // global operation (FormatSubobjectOnDoc) is required as an
   // interface between the User and the FormatSubobjectForDef operation
   // so that the user does not have to Activate the Report Def.

   // We must get the directory that contains the ReportDef.  If we are
   // running under the Animator, we will get it from the LPLR.  If not,
   // we will get it using the Subtask.
   szFileName[ 0 ] = 0;
   if ( GetViewByName( &vAnimator, "__TZVSXCAD", vSubtask, zLEVEL_TASK ) < 0 )
   {
      LPAPP  lpApp;

      SfGetApplicationForSubtask( &lpApp, vSubtask );
      if ( lpApp )
         strcpy_s( szFileName, zsizeof( szFileName ), lpApp->szObjectDir );
   }
   else
   {
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         GetStringFromAttribute( szFileName, zsizeof( szFileName ), vTaskLPLR, "LPLR", "ExecDir" );
         strcat_s( szFileName, zsizeof( szFileName ), "\\" );
      }
   }

   strcat_s( szFileName, zsizeof( szFileName ), cpcReportDefName );
   strcat_s( szFileName, zsizeof( szFileName ), ".XRP" );
   zstrlower( szFileName );

   TraceLineS( "ActivateReportDef: ", szFileName );
   nRC = ActivateOI_FromFile( &vReportDef, "TZRPSRCO", vSubtask,
                              szFileName, zACTIVATE_SYSTEM );

   if ( nRC < 0 )
      return( 0 );
   else
      return( vReportDef );
}

class ZWinApp : public CWinApp
{
public:
   ZWinApp( zCPCHAR cpcAppName );
   virtual ~ZWinApp( );

// Attributes
   HGLOBAL  GetDevMode( ) { return( m_hDevMode ); }

// Overrides
// virtual BOOL InitInstance( );
// virtual int  ExitInstance( );

}; // class ZWinApp

ZWinApp::ZWinApp( zCPCHAR cpcAppName ) :
         CWinApp( cpcAppName )
{
}

ZWinApp::~ZWinApp( )
{
}

zBOOL
ZPrintout::RemoteFilePrint( zVIEW vSubtask )
{
   ZDrApp  *pZDrApp = (ZDrApp *) AfxGetApp( );
   ZWinApp *pWinApp = 0;

// CWaitCursor wait;

   // Get default print info.
   CPrintInfo printInfo;
   ASSERT( printInfo.m_pPD );    // must be set

   printInfo.m_bDirect = TRUE;
   if ( m_lpfnPrintPageRoutine == 0 )
   {
   // mDeleteInit( this );
      return( FALSE );
   }

   if ( pZDrApp == 0 )
      pWinApp = new ZWinApp( "ZeidonWEB" );

   printInfo.SetMaxPage( m_uMaxPageNbr );

   // Set the default printer to the printer specified in the Zeidon.ini
   // file.  For example:
   // ExecutableRemotePrinter=Acrobat Distiller
   // ;ExecutableRemotePrinter=Acrobat PDFWriter
   // ;
   // ;When using Acrobat, in NT, AcrobatPDF_PathFileName must match Registry entry:
   // ; HKEY_CURRENT_USER\Software\ADOBE\Acrobat PDFWriter\PDFFileName=C:\temp\Zeidon.pdf
   // ;
   // ;In Win9x, AcrobatPDF_PathFileName must match the Win.ini entry:
   // ; [Acrobat PDFWriter]
   // ;   PDFFileName=C:\temp\Zeidon.pdf
   // ;
   // AcrobatPDF_PathFileName=C:\temp\Zeidon.pdf
   // ExecutableRemoteDocPath=c:\temp\
   // ExecutableRemoteDocURL=http://pcrs:8080/Documents/
   //
   zCHAR   szPrinterFileDevice[ zTAG_LTH ];
   zCHAR   szDeviceFileName[ zMAX_FILENAME_LTH ];
   zCHAR   szReportFileName[ zMAX_FILENAME_LTH ];
   zCHAR   szRunOnceFileName[ zMAX_FILENAME_LTH ];
   CString csTitle;

   // Get Report URL name.
   GetAppOrWorkstationValue( vSubtask, "ExecutableRemotePrinter",
                             szPrinterFileDevice,
                             zsizeof( szPrinterFileDevice ) );

   // Set up document title.
   GetAppOrWorkstationValue( vSubtask, "AcrobatPDF_PathFileName",
                             szDeviceFileName, zsizeof( szDeviceFileName ) );
   GetAppOrWorkstationValue( vSubtask, "RunOncePDF_PathFileName",
                             szRunOnceFileName, zsizeof( szRunOnceFileName ) );

   // AcrobatPDF_PathFileName is used if we are trying to call the runonce.bat file.
   // This has the directory and filename structure for where to create the pdf file.
   // Currently we are not even using runonce.bat because we are creating this information
   // directly in the pdf writer options and so don't need this.  I won't get rid of the code
   // but probably will at some point because this is really only for bioPDF.
   // KJS 10/23/09 - I am commenting out this code because we are not currently using it.
   /*
   if ( szDeviceFileName[ 0 ] )
   {
      sprintf_s( szReportFileName, szDeviceFileName, m_csReportInfo );
      SysOpenFile( vSubtask, szReportFileName, COREFILE_DELETE );

      if ( szRunOnceFileName[ 0 ] )
      {
         CString csCmdLine = "\"";

         csCmdLine += szRunOnceFileName;
         csCmdLine += "\" ";
         csCmdLine += szReportFileName;
         TraceLineS( "RunOnce Command: ", csCmdLine );
         StartTask( vSubtask, csCmdLine, SW_SHOWNORMAL );
      }
   }
   */

   // Set up document title.
   // Get the reportname.
   sprintf_s( szReportFileName, m_csReportInfo );
   if ( szReportFileName[ 0 ] )
   {
      // Title is used to set the title (which is the reportname) for the document we are printing.
      csTitle = szReportFileName;
     TraceLineS(" In RemoteFilePrint: ", csTitle );
   }
   else
   {
      // We should never get here but this is if no report name was specified.
      csTitle = "ZeidonWEB";
     TraceLineS(" Error In RemoteFilePrint reportname is somehow null so we are using: ", csTitle );
   }

   m_szDefaultPrinter[ 0 ] = 0;
   if ( szPrinterFileDevice[ 0 ] )
   {
      zLONG lNameBuffer = zsizeof( m_szDefaultPrinter );

      if ( GetDefaultPrinterName( m_szDefaultPrinter, &lNameBuffer ) )
      {
         if ( zstrcmp( szPrinterFileDevice, m_szDefaultPrinter ) )
         {
            if ( SetDefaultPrinterName( szPrinterFileDevice ) == FALSE )
               m_szDefaultPrinter[ 0 ] = 0;
         }
         else
            m_szDefaultPrinter[ 0 ] = 0;
      }
   }

   printInfo.m_pPD->m_pd.lStructSize = (DWORD) sizeof( PRINTDLG );

   if ( (pZDrApp && pZDrApp->GetPrinterDeviceDefaults( &printInfo.m_pPD->m_pd )) ||
        (pWinApp && pWinApp->GetPrinterDeviceDefaults( &printInfo.m_pPD->m_pd )) )
   {
   // zCHAR szDeviceName[ CCHDEVICENAME + 1 ];

      // Protect memory handle with ::GlobalLock and ::GlobalUnlock.
      LPDEVMODE pDevMode = (LPDEVMODE)
         (pZDrApp ? ::GlobalLock( pZDrApp->GetDevMode( ) ) :
                    ::GlobalLock( pWinApp->GetDevMode( ) ));

      // Set orientation to landscape.
      pDevMode->dmOrientation = m_bLandscape ? DMORIENT_LANDSCAPE :
                                               DMORIENT_PORTRAIT;

      // Set the way we rotate text by getting INI setting for printer.
      zCHAR szYN[ 2 ];

      GetWorkstationAppValue( vSubtask,
                              (zPCHAR) pDevMode->dmDeviceName,
                              "NegateEscapement", szYN, zsizeof( szYN ) );
      if ( szYN[ 0 ] == 'Y' || szYN[ 0 ] == 'y' )
      {
         if ( m_pZTask->m_nTraceAction > 2 )
         {
            TraceLineS( "Using printer (with NegateEscapement): ",
                        (zPCHAR) pDevMode->dmDeviceName );
         }

         m_nEscapementFactor = -1;
      }
      else
      {
         if ( m_pZTask->m_nTraceAction > 2 )
            TraceLineS( "Using printer: ", (zPCHAR) pDevMode->dmDeviceName );

         m_nEscapementFactor = 1;
      }

      // Set any font scaling specific to printer from INI setting.
      zCHAR szFontScale[ 128 ];

      GetWorkstationAppValue( vSubtask,
                              (zPCHAR) pDevMode->dmDeviceName,
                              "FontScale", szFontScale, zsizeof( szFontScale ) );
      if ( szFontScale[ 0 ] )
      {
         m_dHeight = atof( szFontScale );
         if ( m_pZTask->m_nTraceAction > 2 )
            TraceLineS( "        =====: with FontScale factor ", szFontScale );
      }
      else
         m_dHeight = 0;

      pZDrApp ? ::GlobalUnlock( pZDrApp->GetDevMode( ) ) :
                ::GlobalUnlock( pWinApp->GetDevMode( ) );
   }

   // Set up From and To page range from Min and Max.
   printInfo.m_pPD->m_pd.nFromPage = (WORD) printInfo.GetMinPage( );
   printInfo.m_pPD->m_pd.nToPage = (WORD) printInfo.GetMaxPage( );
   printInfo.m_nNumPreviewPages = 0; //pApp->m_nNumPreviewPages;

   if ( printInfo.m_pPD->m_pd.hDC == 0 )
   {
      // Call CreatePrinterDC if DC was not created by above.
      if ( printInfo.m_pPD->CreatePrinterDC( ) == 0 )
      {
         if ( pWinApp )
            delete( pWinApp );

         return( FALSE );
      }
   }

   // hDC must be set (did you remember to call DoPreparePrinting?)
   ASSERT( printInfo.m_pPD->m_pd.hDC );

   // Setup the printing DC.
   CDC dcPrint;
   if ( printInfo.m_bDocObject == FALSE )
   {
      dcPrint.Attach( printInfo.m_pPD->m_pd.hDC );  // attach printer dc
      dcPrint.m_bPrinting = TRUE;
   }

// OnBeginPrinting( &dcPrint, &pInfo );
// TraceLineX( "BeginPrinting DC ", (zLONG) &dcPrint );

   // Default preparation.
   m_pDC = &dcPrint;

   // Determine how many pixels are in a 16th of an inch.
   CPoint pt( 1000, 1000 );   // a HIENGLISH point is .001 inches

   // Change the mapping mode to high english.
   int nOldMapMode = dcPrint.SetMapMode( MM_HIENGLISH );

   // Convert the client rect sizes to high english.
   dcPrint.LPtoDP( &pt );

   // Determine the page extent in 16ths of an inch.
   // HORZRES    - Width, in pixels, of the screen.
   // VERTRES    - Height, in raster lines, of the screen.
   // LOGPIXELSX - Number of pixels per logical inch along the screen width.
   // LOGPIXELSY - Number of pixels per logical inch along the screen height.
   m_lHorzRes = dcPrint.GetDeviceCaps( HORZRES );
   m_lVertRes = dcPrint.GetDeviceCaps( VERTRES );
   m_nPixelsPerInchX = dcPrint.GetDeviceCaps( LOGPIXELSX );
   m_nPixelsPerInchY = dcPrint.GetDeviceCaps( LOGPIXELSY );
   m_nPixelsOffsetX = dcPrint.GetDeviceCaps( PHYSICALOFFSETX );
   m_nPixelsOffsetY = dcPrint.GetDeviceCaps( PHYSICALOFFSETY );

   if ( m_pZSubtask && (m_pZSubtask->m_pZTask->m_nTraceAction & 0x74) == 0x74 )
   {
      TraceLineI( "DeviceCaps LOGPIXELSX ", m_nPixelsPerInchX );
      TraceLineI( "DeviceCaps LOGPIXELSY ", m_nPixelsPerInchY );
      TraceLineI( "DeviceCaps HORZRES ", m_lHorzRes );
      TraceLineI( "DeviceCaps VERTRES ", m_lVertRes );
      TraceLineI( "DeviceCaps PHYSICALWIDTH ",
                  dcPrint.GetDeviceCaps( PHYSICALWIDTH ) );
      TraceLineI( "DeviceCaps PHYSICALHEIGHT ",
                  dcPrint.GetDeviceCaps( PHYSICALHEIGHT ) );
      TraceLineI( "DeviceCaps PHYSICALOFFSETX ",
                  dcPrint.GetDeviceCaps( PHYSICALOFFSETX ) );
      TraceLineI( "DeviceCaps PHYSICALOFFSETY ",
                  dcPrint.GetDeviceCaps( PHYSICALOFFSETY ) );
   }

   m_lHorzSize256ths = (m_lHorzRes * 256) / m_nPixelsPerInchX;
   m_lVertSize256ths = (m_lVertRes * 256) / m_nPixelsPerInchY;

   // Reset the mapping mode.
   dcPrint.SetMapMode( nOldMapMode );

   // Set up document info and start the document printing process.
   DOCINFO docInfo;

   zmemset( &docInfo, 0, sizeof( DOCINFO ) );
   docInfo.cbSize = sizeof( DOCINFO );

   // This is where we set the report name to the pdf writer.
   docInfo.lpszDocName = csTitle;

   CString csPortName = printInfo.m_pPD->GetPortName( );
   int nFormatID = AFX_IDS_PRINTONPORT;
// int nFormatID = AFX_IDS_PRINTTOFILE;
   docInfo.lpszOutput = 0;
// docInfo.lpszOutput = csOutput;

   // Start document printing process.
   if ( printInfo.m_bDocObject == FALSE &&
        dcPrint.StartDoc( &docInfo ) == SP_ERROR )
   {
      // Cleanup and show error message.
   // OnEndPrinting( &dcPrint, &pInfo );
      if ( m_szDefaultPrinter[ 0 ] )
      {
         SetDefaultPrinterName( m_szDefaultPrinter );
         m_szDefaultPrinter[ 0 ] = 0;
      }

      dcPrint.Detach( );   // will be cleaned up by CPrintInfo destructor
      AfxMessageBox( AFX_IDP_FAILED_TO_START_PRINT );
      if ( pWinApp )
         delete( pWinApp );

      return( FALSE );
   }

   // Guarantee values are in the valid range.
   UINT nEndPage = printInfo.GetToPage( );
   UINT nStartPage = printInfo.GetFromPage( );

   if ( nEndPage < printInfo.GetMinPage( ) )
      nEndPage = printInfo.GetMinPage( );

   if ( nEndPage > printInfo.GetMaxPage( ) )
      nEndPage = printInfo.GetMaxPage( );

   if ( nStartPage < printInfo.GetMinPage( ) )
      nStartPage = printInfo.GetMinPage( );

   if ( nStartPage > printInfo.GetMaxPage( ) )
      nStartPage = printInfo.GetMaxPage( );

   int nStep = (nEndPage >= nStartPage) ? 1 : -1;
   nEndPage = (nEndPage == 0xffff) ? 0xffff : nEndPage + nStep;
   UINT nEndPageSave = nEndPage;

   // If it's a doc object, we don't loop page-by-page
   // because doc objects don't support that kind of levity.

   zVIEW  vSourceOI;
   zVIEW  vReportDef;
   zPCHAR pchSubobjectEntityName;
   zLONG  lFlags;
   zSHORT nRC;

   GetViewByName( &vSourceOI, "TZ_SourceOrigOI", vSubtask, zLEVEL_TASK );
   GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK );
   GetAddrForAttribute( &pchSubobjectEntityName, vReportDef,
                        "Report", "WK_SubobjectEntityName" );
   GetIntegerFromAttribute( &lFlags, vReportDef, "Report", "PrintFlags" );

   while ( TRUE )
   {
      if ( lFlags & zREPORT_MULTIPLE_ROOT )
         SetCursorFirstEntity( vSourceOI, pchSubobjectEntityName, 0 );

      nEndPage = nEndPageSave;
      m_uMaxPageNbr = 1;
      printInfo.SetMaxPage( m_uMaxPageNbr );

      // Begin page printing loop.
      for ( printInfo.m_nCurPage = nStartPage;
            printInfo.m_nCurPage != nEndPage;
            printInfo.m_nCurPage += nStep )
      {
      // OnPrepareDC( &dcPrint, &pInfo );
         if ( m_uMaxPageNbr >= printInfo.m_nCurPage )
            printInfo.m_bContinuePrinting = TRUE;
         else
         {
            printInfo.m_bContinuePrinting = FALSE; // end of print
            break;
         }

         // Set up drawing rect to entire page (in logical coordinates).
         printInfo.m_rectDraw.SetRect( 0, 0,
                                       dcPrint.GetDeviceCaps( HORZRES ),
                                       dcPrint.GetDeviceCaps( VERTRES ) );
         dcPrint.DPtoLP( &printInfo.m_rectDraw );

         // Attempt to start the current page.
         if ( dcPrint.StartPage( ) < 0 )
         {
         // bError = TRUE;
            break;
         }

         // OnPrepareDC( &dcPrint, &pInfo );
         printInfo.m_bContinuePrinting = TRUE;

         // Page successfully started, so now render the page.
      // OnPrint( &dcPrint, &pInfo );

         dcPrint.SetMapMode( MM_HIENGLISH );

         if ( m_lPageCnt < (zLONG) printInfo.m_nCurPage )
            m_lPageCnt = printInfo.m_nCurPage;

         nRC = (*m_lpfnPrintPageRoutine)( vSubtask, printInfo.m_nCurPage,
                                          m_lHorzSize256ths,
                                          m_lVertSize256ths );
         if ( nRC > 0 )
         {
            nEndPage += nStep;
            m_uMaxPageNbr = printInfo.m_nCurPage + nStep;

            // If we set this when bPreview is TRUE, it will Disable the
            // "Prev" button in print preview.  If it is set when not in
            // preview mode, the printing of all pages is synchronous, otw,
            // the printing is asynchronous.  Since we want synchronous
            // printing, we set the minimum page to be the current page
            // when print preview is turned off.
            if ( printInfo.m_bPreview == FALSE )
               printInfo.SetMinPage( m_uMaxPageNbr );
         }
         else
         {
            m_uMaxPageNbr = printInfo.m_nCurPage;
         }

         if ( nRC <= 0 ||
              printInfo.GetMaxPage( ) < m_uMaxPageNbr )
         {
            printInfo.SetMaxPage( m_uMaxPageNbr );
         }

         if ( m_bPhantom == FALSE )
         {
            if ( dcPrint.EndPage( ) < 0 )
            {
            // bError = TRUE;
               break;
            }
         }
      }

      if ( m_bPhantom )
      {
         SetAttributeFromInteger( vReportDef, "Report",
                                  "TotalPageCnt", m_uMaxPageNbr );
         m_bPhantom = FALSE;
      }
      else
         break;
   }

   // Cleanup document printing process.
   if ( printInfo.m_bDocObject == FALSE )
   {
   // if ( bError == FALSE )
         dcPrint.EndDoc( );
   // else
   //    dcPrint.AbortDoc( );
   }

// OnEndPrinting( &dcPrint, &pInfo );    // clean up after printing

   if ( m_szDefaultPrinter[ 0 ] )
   {
      SetDefaultPrinterName( m_szDefaultPrinter );
      m_szDefaultPrinter[ 0 ] = 0;
   }

   dcPrint.Detach( );   // will be cleaned up by CPrintInfo destructor
   if ( pWinApp )
      delete( pWinApp );

   return( TRUE );
}

zOPER_EXPORT zLONG  OPERATION
PrintReportToPDF( zVIEW   vSubtask,
                  zVIEW   vSourceOI,
                  zCPCHAR cpcSubobjectEntityName,
                  zCPCHAR cpcReportDefName,
                  zPCHAR  pchReportName,
                  zLONG   lFlags )
{
   return( GenerateReportToPDF( vSubtask, vSourceOI, cpcSubobjectEntityName,
                                cpcReportDefName, pchReportName, "", lFlags ) );
}

zOPER_EXPORT zLONG  OPERATION
GenerateReportToPDF( zVIEW   vSubtask,
                     zVIEW   vSourceOI,
                     zCPCHAR cpcSubobjectEntityName,
                     zCPCHAR cpcReportDefName,
                     zPCHAR  pchReportName,
                     zCPCHAR cpcCSS_FileName,
                     zLONG   lFlags )
{
   zVIEW  vReportDef;
   zVIEW  vKZXMLPGO;
   zLONG  lPrintFlags;

   vReportDef = ActivateReportDef( vSubtask, cpcReportDefName );
   if ( vReportDef == 0 )
      return( -3 );

   // KJS 02/21/09 - I would like to be able to open up the file in a different
   // directory so that we don't have all these files in the regular jsp directory
   // Would like to set this along with the file name.
   zCHAR   szPathFileName[ zMAX_FILENAME_LTH ];
   CString   szFileName;

   // KJS 02/20/2009 - We would like our pdfs to be created in a separate directory.
   // Use PDF_PathFileName to get this.  Currently I have this set as /zencas/pdf/ but
   // I know Aadit would like to put these in a totally different directory not under
   // zencas.  Might need to change something because I had a hard time getting a file
   // to open when PDF_PathFileName was something like "C:\Program Files...".
   GetAppOrWorkstationValue( vSubtask, "PDF_PathFileName",
                             szPathFileName, zsizeof( szPathFileName ) );

   szFileName += szPathFileName;
   szFileName += pchReportName;


   // KJS 10/31/08 - We are going to set the report name in KZXMLPGO so that
   // we can retrieve this name in FindOpenFile (kzoejava.c) when trying to
   // open the file in the jsp files.
   GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
   SetAttributeFromString( vKZXMLPGO, "Session",
                           "PrintFileName",
                           szFileName );
                           //pchReportName );
   SetAttributeFromString( vKZXMLPGO, "Session",
                           "PrintFileType",
                           "pdf" );


   GetIntegerFromAttribute( &lPrintFlags, vReportDef,
                            "Report", "PrintFlags" );
   SetCursorFirstEntity( vReportDef, "Page", 0 );

// #define zREPORT_PRINTNAMED            0x00040000
// #define zREPORT_RETURNNAME            0x00080000
// #define zREPORT_CREATENAME            0x00100000

   lFlags |= zREPORT_PRINT2PDF;
   if ( lFlags & zREPORT_MULTIPLE_ROOT )
      SetCursorFirstEntity( vSourceOI, cpcSubobjectEntityName, 0 );

   SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );

   // These values are stored for future reference ... the only place
   // they are set.
   SetAttributeFromString( vReportDef, "Report",
                           "WK_SubobjectEntityName",
                           cpcSubobjectEntityName );
   SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lFlags );
   SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", -1 );

   ZDrApp   *pZDrApp = (ZDrApp *) AfxGetApp( );
   ZTask    *pZTask;
   ZSubtask *pZSubtask;

   if ( pZDrApp )
   {
      pZTask = pZDrApp->m_pZTask;
      GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   }
   else
   {
      pZTask = new ZTask( vSubtask );
      pZSubtask = new ZSubtask( pZTask, 0 );  // ZSubtask - alternate ctor
      if ( g_pSubtaskList == 0 )
      {
         if ( vSubtask->bInvalidMainOI || vSubtask->hMainOI )
         {
            ::MessageBox( 0, "Subtask's MainOI is already used",
                          "HTML Report Error", MB_OK );
         }

         pZSubtask->m_vDialog = vSubtask;
         vSubtask->bInvalidMainOI = 1;
         vSubtask->hMainOI = (LPVIEWOI) pZSubtask;
      }
   }

   pZTask->m_uAppState |= zTASK_STATE_PRINT;
   pZTask->m_vNewAppSubtask = vSubtask;
   ZPrintout *pPrint = new ZPrintout( pZTask,
                                      "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
                                      0, FALSE, FALSE, FALSE,
                                      cpcReportDefName, pchReportName, cpcCSS_FileName );
   pZSubtask->m_pZPrintout = pPrint;
   if ( lPrintFlags & zREPORT_LANDSCAPE )
      pPrint->m_bLandscape = TRUE;

   if ( lFlags & 0x00000020 )
      pPrint->m_bPhantom = TRUE;

   SysMutexLock( vSubtask, "ZDxPrint", 0, 0 );
   pPrint->RemoteFilePrint( vSubtask );

   delete( pPrint ); // unlocks the mutex
   if ( pZDrApp == 0 )
   {
      mDeleteInit( pZTask );
      pZSubtask->m_pZTask = 0;
      if ( g_pSubtaskList == 0 )
      {
         pZSubtask->m_vDialog = 0;
         vSubtask->bInvalidMainOI = 0;
         vSubtask->hMainOI = 0;
      }

      mDeleteInit( pZSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zLONG  OPERATION
GenerateReportToHTML( zVIEW   vSubtask,
                      zVIEW   vSourceOI,
                      zCPCHAR cpcSubobjectEntityName,
                      zCPCHAR cpcReportDefName,
                      zPCHAR  pchReportName,
                      zCPCHAR cpcCSS_FileName,
                      zLONG   lFlags )
{
   zVIEW  vReportDef;
   zLONG  lPrintFlags;
   zPCHAR pchSubobjectEntityName = 0;

   if ( vSourceOI == 0 )
   {
      vReportDef = ActivateReportDef( vSubtask, cpcReportDefName );
      if ( vReportDef )
      {
         zPCHAR pchViewName;

         GetAddrForAttribute( &pchViewName, vReportDef,
                              "Report", "HTML_ReportViewName" );
         GetAddrForAttribute( &pchSubobjectEntityName, vReportDef,
                              "Report", "HTML_ReportEntityName" );
         if ( pchViewName && pchViewName[ 0 ] &&
              pchSubobjectEntityName && pchSubobjectEntityName[ 0 ] )
         {
            cpcSubobjectEntityName = pchSubobjectEntityName;
            GetViewByName( &vSourceOI, pchViewName, vSubtask, zLEVEL_TASK );
         }
      }
   }
   else
   if ( cpcReportDefName == 0 || cpcReportDefName[ 0 ] == 0 )
      GetViewByName( &vReportDef, "TZ_ReportDef", vSubtask, zLEVEL_TASK );
   else
      vReportDef = ActivateReportDef( vSubtask, cpcReportDefName );

   if ( vReportDef == 0 )
      return( -3 );

   GetIntegerFromAttribute( &lPrintFlags, vReportDef,
                            "Report", "PrintFlags" );
   SetCursorFirstEntity( vReportDef, "Page", 0 );

// #define zREPORT_PRINTNAMED            0x00040000
// #define zREPORT_RETURNNAME            0x00080000
// #define zREPORT_CREATENAME            0x00100000

   lFlags |= zREPORT_PRINT2HTML;
   if ( (lFlags & 0x00008000) == 0 && lFlags & zREPORT_MULTIPLE_ROOT )
      SetCursorFirstEntity( vSourceOI, cpcSubobjectEntityName, 0 );

   SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );

   // These values are stored for future reference ... the only place
   // they are set.
   SetAttributeFromString( vReportDef, "Report",
                           "WK_SubobjectEntityName",
                           cpcSubobjectEntityName );
   SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lFlags );
   SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", -1 );

   ZDrApp   *pZDrApp = (ZDrApp *) AfxGetApp( );
   ZTask    *pZTask;
   ZSubtask *pZSubtask;

   if ( pZDrApp )
   {
      pZTask = pZDrApp->m_pZTask;
      GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   }
   else
   {
      pZTask = new ZTask( vSubtask );
      pZSubtask = new ZSubtask( pZTask, 0 );  // ZSubtask - alternate ctor
      if ( g_pSubtaskList == 0 )
      {
         if ( vSubtask->bInvalidMainOI || vSubtask->hMainOI )
         {
            ::MessageBox( 0, "Subtask's MainOI is already used",
                          "HTML Report Error", MB_OK );
         }

         pZSubtask->m_vDialog = vSubtask;
         vSubtask->bInvalidMainOI = 1;
         vSubtask->hMainOI = (LPVIEWOI) pZSubtask;
      }
   }

   pZTask->m_uAppState |= zTASK_STATE_PRINT;
   pZTask->m_vNewAppSubtask = vSubtask;

   ZPrintout *pPrint;

   if ( lFlags & 0x00008000 ) // +32768 - Drive by ReportDef Instead of SourceOI.
   {
      pPrint = new ZPrintout( pZSubtask,
                              "KZRPSRCO", "oTZRPSRCO_FormatReportGroupSets",
                              lFlags, FALSE, FALSE, FALSE,
                              cpcReportDefName, pchReportName, cpcCSS_FileName );
   }
   else
   {
      pPrint = new ZPrintout( pZSubtask,
                              "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
                              lFlags, FALSE, FALSE, FALSE,
                              cpcReportDefName, pchReportName, cpcCSS_FileName );
   }

   mDeleteInit( pPrint );
   if ( pZDrApp == 0 )
   {
      mDeleteInit( pZTask );
      pZSubtask->m_pZTask = 0;
      if ( g_pSubtaskList == 0 )
      {
         pZSubtask->m_vDialog = 0;
         vSubtask->bInvalidMainOI = 0;
         vSubtask->hMainOI = 0;
      }

      mDeleteInit( pZSubtask );
   }

   return( 0 );
}

zOPER_EXPORT zLONG  OPERATION
PrintReportToHTML( zVIEW   vSubtask,
                   zVIEW   vSourceOI,
                   zCPCHAR cpcSubobjectEntityName,
                   zCPCHAR cpcReportDefName,
                   zPCHAR  pchReportName,
                   zLONG   lFlags )
{
   return( GenerateReportToHTML( vSubtask, vSourceOI, cpcSubobjectEntityName,
                                 cpcReportDefName, pchReportName, "", lFlags ) );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function prints text to the HTML file.
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
//                        16 - don't override CSS Font
//                        32 - don't override CSS Position
//                        64 - don't override CSS Size
//
// lJustify determines the type of justification to be done.
// Currently, the justification codes are as follows:
//    2 - center
//    4 - right
//    All other values default to left justification.
//
// The return value is:
//    0 - text printed (no errors)
//   -1 - invalid view to the application's subtask
//   -2 - report has not been opened
//

// Source                                            Output
// This is some text <div style="color:#FF0000;">    This is some text This is a header in a div section
// <h4>This is a header in a div section</h4>        This is a paragraph in a div section
// <p>This is a paragraph in a div section</p>
// </div>


// <p>This is a paragraph <span style="color:#0000FF;">
// This is a paragraph</span> This is a paragraph</p>
// <p>
// <span style="color:#00DD45;">
// This is another paragraph
// </span>
// </p>

// <span  style="width:132px; height:18px;
// Z-INDEX:101;POSITION:absolute;LEFT:8px;TOP:20px;">Church</span>

/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
PrintTextToHTML( zVIEW   vSubtask,
                 zVIEW   vAppOI,
                 zCPCHAR cpcText,
                 zCPCHAR cpcClass,
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

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout &&
        pZSubtask->m_pZPrintout->m_lFileCSV != -1 )
   {
      if ( cpcText == 0 )
      {
         SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV,
                       "</pre>\r\n", 8 );
                    // "<br /></pre>\r\n", 14 );
         SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV,
                       "<pre>", 5 );
         pZSubtask->m_pZPrintout->m_lLastColumnCSV = 0;
      }
      else
      {
         zLONG lTextLth = zstrlen( cpcText ) + 1;
         zLONG lLth;
         zLONG lPixelsLeft = lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256;
         zLONG lPixelsTop = lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY / 256;
         zLONG lPixelsWidth = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256) - lPixelsLeft;
         zLONG lPixelsHeight = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY / 256) - lPixelsTop;
      // pZSubtask->m_pZPrintout->m_lHorzSize256ths
      // pZSubtask->m_pZPrintout->m_lHorzRes pZSubtask->m_pZPrintout->m_lVertRes
      // zLONG lSpaces = mConvertPixelToMapDlgX( lPixels );
      // lSpaces = mConvertMapDlgToCharX( lSpaces );

      // <span  style="width:132px; height:18px;
      // POSITION:absolute;LEFT:8px;TOP:20px;">Church</span>

         CWindowDC dc( CWnd::GetDesktopWindow( ) );
         zPCHAR pch = new char[ lTextLth + 512 ];
         zPCHAR pchText = new char[ lTextLth + 256 ];
         CString csJustify;

         if ( lJustify & 2 )   // center
         {
            csJustify = "text-align:center;";
         }
         else
         if ( lJustify & 4 )   // right
         {
            csJustify = "text-align:right;";
         }
         else
         if ( lJustify == 0 )  // left
         {
         }

         CString csSpan( "<span" );
         CString csText;
         CString csFont;

         sprintf_s( pch, lTextLth + 512, "position:absolute;left:%dpx;top:%dpx;", lPixelsLeft, lPixelsTop );
         CString csPosition( pch );

         sprintf_s( pch, lTextLth + 512, "width:%dpx; height:%dpx;", lPixelsWidth, lPixelsHeight );
         CString csSize( pch );

         if ( cpcClass && cpcClass[ 0 ] )
         {
            csSpan += " class=\"";
            csSpan += cpcClass;
            csSpan += "\"";
         }
         else
         {
            lType &= ~0x00000070; // no CSS, so turn off style bits
         }

         if ( pZSubtask->m_pZPrintout->m_pFontCurr )
         {
            LOGFONT lf;

            if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != CLR_INVALID &&
                 pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != 0xFF000000 &&
                 pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != 0 )
            {
               BYTE byRed = GetRValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
               BYTE byGreen = GetGValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
               BYTE byBlue = GetBValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );

               sprintf_s( pch, lTextLth + 512, "color:#%.2x%.2x%.2x;", byRed, byGreen, byBlue );
               csFont += pch;
            }

            if ( pZSubtask->m_pZPrintout->m_pFontCurr->GetLogFont( &lf ) )
            {
               if ( lf.lfItalic )
                  csFont += "font-style:italic;";

               if ( lf.lfUnderline )
                  csFont += "text-decoration:underline;";

               if ( lf.lfStrikeOut )
                  csFont += "text-decoration:line-through;";

               if ( lf.lfWeight >= 700 )
                  csFont += "font-weight:bold;";
            }

            if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_csFaceName[ 0 ] )
            {
            // if ( zstrcmp( pZSubtask->m_pZPrintout->m_pFontCurr->m_csFaceName, "Arial Narrow" ) == 0 )
            //    lType /= lType - lType;

               csFont += "font-family:";
               csFont += pZSubtask->m_pZPrintout->m_pFontCurr->m_csFaceName;
               csFont += ";";
            }

            if ( pZSubtask->m_pZPrintout->m_pFontCurr->m_nPointSize )
            {
               sprintf_s( pch, lTextLth + 512, "font-size:%dpt;", pZSubtask->m_pZPrintout->m_pFontCurr->m_nPointSize / 16 );
               csFont += pch;
            }

            CFont font;
            lf.lfHeight /= 16;  // this logical font needs the same point size reduction as above
            font.CreateFontIndirect( &lf );
            CFont *pOldFont = dc.SelectObject( &font );
            MakeShortString( &dc, pchText, lTextLth, cpcText, lPixelsWidth, 0 );
            csText = pchText;
            dc.SelectObject( pOldFont );

            pZSubtask->m_pZPrintout->m_pFontCurr = 0;
         }
         else
         {
            MakeShortString( &dc, pchText, lTextLth, cpcText, lPixelsWidth, 0 );
            csText = pchText;
         }

         // lType may have the following bits set:
         //   16 - don't override CSS Font
         //   32 - don't override CSS Position
         //   64 - don't override CSS Size
         if ( (lType & 0x00000070) != 0x00000070 || csJustify.IsEmpty( ) == FALSE ) // if any of the bits are not set, add style=
         {
            csSpan += " style=\"";
            if ( (lType & 0x00000040) == 0 ) // add size
               csSpan += csSize;

            if ( (lType & 0x00000020) == 0 ) // add position
               csSpan += csPosition;

            if ( (lType & 0x00000010) == 0 ) // add font
               csSpan += csFont;

            if ( csJustify.IsEmpty( ) == FALSE )
               csSpan += csJustify;

            csSpan += "\""; // terminating quotes
         }

         csSpan += ">";
         csSpan += csText;
         csSpan += "</span>";

         lLth = csSpan.GetLength( );
         SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV,
                       csSpan, lLth );
         delete [] pch;
         delete [] pchText;
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
         for ( zSHORT k = 0; k < 4; k++ )
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

               PrintLineToHTML( vSubtask, vAppOI, lLinePosX, lLinePosY,
                                lLineEndX, lLineEndY,
                                nPenWidth, nPenType,
                                lBorderColor, k % 2 );
            }
         }
      }

      return( 0 );
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
PrintLineToHTML( zVIEW   vSubtask,
                 zVIEW   vAppOI,
                 zLONG   lLeft256ths,
                 zLONG   lTop256ths,
                 zLONG   lRight256ths,
                 zLONG   lBottom256ths,
                 zLONG   nPenWidth,
                 zLONG   nPenType,
                 zLONG   lPenColor,
                 zSHORT  nLineStyle )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout &&
        pZSubtask->m_pZPrintout->m_lFileCSV != -1 )
   {
      if ( (pZSubtask->m_pZTask->m_uAppState & zTASK_STATE_PRINT) == 0 )
      {
         return( -2 );
      }

      if ( nPenWidth == 0 )
         nPenWidth = 1;

      CString csPenType;

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
           csPenType = "NOSHADE";
           break;
      }

      switch ( nLineStyle )
      {
         case 1:  // vertical
            break;

         case 2:  // diagonal descending
            break;

         case 3:  // diagonal ascending
            break;

         default: // horizontal ... this is the only one we support
            break;
      }

      zLONG lPixelsLeft = lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256;
      zLONG lPixelsTop = lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256;
      zLONG lPixelsWidth = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256) - lPixelsLeft;
      zLONG lPixelsHeight = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256) - lPixelsTop;

      zCHAR   szText[ 256 ];

#if 0
      CString csJustify;

      if ( lJustify & 2 )   // center
      {
         csJustify = "center";
      }
      else
      if ( lJustify & 4 )   // right
      {
         csJustify = "right";
      }
      else
      if ( lJustify == 0 )  // left
      {
         csJustify = "left";
      }
#endif

   // <hr style="width:190px;height:3px;POSITION:absolute;LEFT:25px;TOP:101px;" />

      if ( pZSubtask->m_pZPrintout->m_pFontCurr &&
           pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != CLR_INVALID &&
           pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != 0xFF000000 &&
           pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText != 0 )
      {
         BYTE byRed = GetRValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         BYTE byGreen = GetGValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );
         BYTE byBlue = GetBValue( pZSubtask->m_pZPrintout->m_pFontCurr->m_clrText );

         sprintf_s( szText, zsizeof( szText ),
                    "<hr %s style=\"color:#%.2x%.2x%.2x;width:%dpx;height:%dpx;"
                      "POSITION:absolute;LEFT:%dpx;TOP:%dpx;\" />",
                    csPenType.GetString(), byRed, byGreen, byBlue,
                    lPixelsWidth, nPenWidth, lPixelsLeft, lPixelsTop );
      }
      else
      {
         sprintf_s( szText, zsizeof( szText ),
                    "<hr %s style=\"width:%dpx;height:%dpx;"
                      "POSITION:absolute;LEFT:%dpx;TOP:%dpx;\" />",
                    csPenType.GetString(), lPixelsWidth, nPenWidth, lPixelsLeft, lPixelsTop );
      }

      SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, szText, zstrlen( szText ) );
      return( 0 );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function draws a bitmap to HTML.  The parameters are:
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
PrintBitmapToHTML( zVIEW   vSubtask,
                   zVIEW   vAppOI,
                   zLONG   lLeft256ths,
                   zLONG   lTop256ths,
                   zLONG   lRight256ths,
                   zLONG   lBottom256ths,
                   zCPCHAR cpcBitmapCtrlBOI )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 &&
        pZSubtask->m_pZPrintout &&
        pZSubtask->m_pZPrintout->m_lFileCSV != -1 )
   {
      zCHAR  szText[ 1024 ];
      zCHAR  szBitmapFileName[ 256 ];
      zLONG lPixelsLeft = lLeft256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256;
      zLONG lPixelsTop = lTop256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY / 256;
      zLONG lPixelsWidth = (lRight256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchX / 256) - lPixelsLeft;
      zLONG lPixelsHeight = (lBottom256ths * pZSubtask->m_pZPrintout->m_nPixelsPerInchY / 256) - lPixelsTop;
      zPCHAR pchBitmapName = (zPCHAR) cpcBitmapCtrlBOI;

      if ( pchBitmapName && *pchBitmapName == 0 )
         pchBitmapName++;

      SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szBitmapFileName, zsizeof( szBitmapFileName ) );
      if ( szBitmapFileName[ 0 ] )
         SysAppendcDirSep( szBitmapFileName );

      strcat_s( szBitmapFileName, pchBitmapName );
      TraceLineS( "PrintBitmapToHTML File Name: ", szBitmapFileName );
      sprintf_s( szText, zsizeof( szText ),
                 "<span style=\"width:%dpx; height:%dpx;POSITION:absolute;LEFT:%dpx;TOP:%dpx;\"><img border=0 hspace=6 width=%dpx height=%dpx src=\"%s\"></span>",
                 lPixelsWidth, lPixelsHeight, lPixelsLeft, lPixelsTop, lPixelsWidth, lPixelsHeight, szBitmapFileName );
      SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, szText, zstrlen( szText ) );
      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zLONG  OPERATION
PrintReportToCSV( zVIEW   vSubtask,
                  zVIEW   vSourceOI,
                  zCPCHAR cpcSubobjectEntityName,
                  zCPCHAR cpcReportDefName,
                  zPCHAR  pchReportName,
                  zLONG   lFlags )
{
   zVIEW  vReportDef;
   zLONG  lPrintFlags;

   vReportDef = ActivateReportDef( vSubtask, cpcReportDefName );
   if ( vReportDef == 0 )
      return( -3 );

   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   SetCursorFirstEntity( vReportDef, "Page", 0 );

// #define zREPORT_PRINTNAMED            0x00040000
// #define zREPORT_RETURNNAME            0x00080000
// #define zREPORT_CREATENAME            0x00100000

   lFlags |= zREPORT_PRINT2CSV;
   if ( lFlags & zREPORT_MULTIPLE_ROOT )
      SetCursorFirstEntity( vSourceOI, cpcSubobjectEntityName, 0 );

   SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );

   // These values are stored for future reference ... the only place
   // they are set.
   SetAttributeFromString( vReportDef, "Report", "WK_SubobjectEntityName", cpcSubobjectEntityName );
   SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lFlags );
   SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", -1 );

   ZDrApp   *pZDrApp = (ZDrApp *) AfxGetApp( );
   ZTask    *pZTask;
   ZSubtask *pZSubtask;

   if ( pZDrApp )
   {
      pZTask = pZDrApp->m_pZTask;
      GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   }
   else
   {
      pZTask = new ZTask( vSubtask );
      pZSubtask = new ZSubtask( pZTask, 0 );  // ZSubtask - alternate ctor
   }

   pZTask->m_uAppState |= zTASK_STATE_PRINT;
   pZTask->m_vNewAppSubtask = vSubtask;
   ZPrintout *pPrint = new ZPrintout( pZSubtask, "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
                                      lFlags, FALSE, FALSE, FALSE, cpcReportDefName, pchReportName, "" );
   mDeleteInit( pPrint );
   return( 0 );
}

zOPER_EXPORT zLONG  OPERATION
PrintTextToCSV( zVIEW   vSubtask,
                zCPCHAR cpcText,
                zLONG   lColumnCSV )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZSubtask *pZSubtaskMain = pZSubtask->GetSplitterMainSubtask( );
      if ( pZSubtaskMain )
         pZSubtask = pZSubtaskMain;
   }

   if ( pZSubtask && pZSubtask->m_pZPrintout &&
        pZSubtask->m_pZPrintout->m_lFileCSV != -1 )
   {
      if ( cpcText == 0 || lColumnCSV == 0 )
      {
         SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, "\r\n", 2 );
         pZSubtask->m_pZPrintout->m_lLastColumnCSV = 0;
      }
      else
      {
         while ( pZSubtask->m_pZPrintout->m_lLastColumnCSV < lColumnCSV - 1 )
         {
            if ( pZSubtask->m_pZPrintout->m_lLastColumnCSV == 0 )
               SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, "\"\"", 2 );
            else
               SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, ",\"\"", 3 );

            pZSubtask->m_pZPrintout->m_lLastColumnCSV++;
         }

         zLONG lLth = zstrlen( cpcText );
         zBOOL bNumeric;

         if ( lLth )
         {
            bNumeric = TRUE;
            for ( zLONG k = 0; k < lLth; k++ )
            {
               if ( isdigit( cpcText[ k ] ) == 0 )
               {
                  bNumeric = FALSE;
                  break;
               }
            }
         }
         else
         {
            bNumeric = FALSE;
         }

         if ( pZSubtask->m_pZPrintout->m_lLastColumnCSV > 0 )
         {
            SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, ",", 1 );
         }

         if ( bNumeric == FALSE )
         {
            SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, "\"", 1 );
         }

         SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, cpcText, lLth );
         if ( bNumeric == FALSE )
         {
            SysWriteFile( vSubtask, pZSubtask->m_pZPrintout->m_lFileCSV, "\"", 1 );
         }

         pZSubtask->m_pZPrintout->m_lLastColumnCSV = lColumnCSV;
      }

      return( 0 );
   }

   return( -1 );
}

zOPER_EXPORT zLONG  OPERATION
PrintReportToCtrl( zVIEW   vSubtask,
                   zVIEW   vSourceOI,
                   zCPCHAR cpcSubobjectEntityName,
                   zCPCHAR cpcReportDefName,
                   zPCHAR  pchReportName,
                   zLONG   lFlags,
                   zCPCHAR cpcReportCtrlTag )
{
   zVIEW  vReportDef;
   zLONG  lPrintFlags;

   vReportDef = ActivateReportDef( vSubtask, cpcReportDefName );
   if ( vReportDef == 0 )
      return( -3 );

   GetIntegerFromAttribute( &lPrintFlags, vReportDef, "Report", "PrintFlags" );
   SetCursorFirstEntity( vReportDef, "Page", 0 );

   lFlags |= zREPORT_PRINT2CTRL;
   if ( lFlags & zREPORT_MULTIPLE_ROOT )
      SetCursorFirstEntity( vSourceOI, cpcSubobjectEntityName, 0 );

   SetNameForView( vSourceOI, "TZ_SourceOrigOI", 0, zLEVEL_TASK );
   SetNameForView( vReportDef, "TZ_ReportDef", 0, zLEVEL_TASK );

   // These values are stored for future reference ... the only place
   // they are set.
   SetAttributeFromString( vReportDef, "Report", "WK_SubobjectEntityName",  cpcSubobjectEntityName );
   SetAttributeFromInteger( vReportDef, "Report", "PrintFlags", lFlags );
   SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", -1 );

   ZDrApp   *pZDrApp = (ZDrApp *) AfxGetApp( );
   ZTask    *pZTask;
   ZSubtask *pZSubtask;

   if ( pZDrApp )
   {
      pZTask = pZDrApp->m_pZTask;
      GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 );
   }
   else
   {
      pZTask = new ZTask( vSubtask );
      pZSubtask = new ZSubtask( pZTask, 0 );  // ZSubtask - alternate ctor
   }

   pZTask->m_uAppState |= zTASK_STATE_PRINT;
   pZTask->m_vNewAppSubtask = vSubtask;
   ZPrintout *pPrint = new ZPrintout( pZSubtask,
                                      "KZRPSRCO", "oTZRPSRCO_FormatDocPage",
                                      lFlags, FALSE, FALSE, FALSE,
                                      cpcReportDefName,
                                      (zPCHAR) cpcReportCtrlTag, "" );
   return( 0 );
}

//./ ADD NAME=GenerateUniqueFileName
// Source Module=zdrprint.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GenerateUniqueFileName
//                                              04/11/01 Modified: 04/11/01
//
//  PURPOSE:   To generate a unique file name give a specified path, partial
//             file name, and extension.
//
//  PARAMETERS: pchReturnFileName - Return file name
//              nMaxNameLth  - Maximum length of return file name.
//              cpcPathName  - The path in which to generate the file name.
//              cpcFileName  - A partial file name (uniqueness will be
//                             attained by appending unique characters.
//              cpcExtension - The file name extension (may be omitted).
//
//  RETURNS:   FALSE - Could not generate unique name
//             TRUE  - Unique name generated and returned successfully
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zBOOL OPERATION
GenerateUniqueFileName( zPCHAR  pchReturnFileName,
                        zSHORT  nMaxNameLth,
                        zCPCHAR cpcPathName,
                        zCPCHAR cpcFileName,
                        zCPCHAR cpcExtension )
{
   GUID my_guid;
   zCHAR szGuid[ 40 ];
   zSHORT nLth = (zSHORT) zstrlen( cpcPathName );

   ::CoCreateGuid( &my_guid );

   sprintf_s( szGuid, zsizeof( szGuid ), "%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
              my_guid.Data1, my_guid.Data2, my_guid.Data3,
              my_guid.Data4[ 0 ], my_guid.Data4[ 1 ],
              my_guid.Data4[ 2 ], my_guid.Data4[ 3 ],
              my_guid.Data4[ 4 ], my_guid.Data4[ 5 ],
              my_guid.Data4[ 6 ], my_guid.Data4[ 7 ] );

   // Subtract: '\\' + date + '.' + null + ext.
   nMaxNameLth -= (40 + (zSHORT) zstrlen( cpcExtension ));

   if ( nLth + (zSHORT) zstrlen( cpcFileName ) < nMaxNameLth )
   {
      strcpy_s( pchReturnFileName, nMaxNameLth, cpcPathName );
      if ( nLth && pchReturnFileName[ nLth - 1 ] != '\\' &&
           pchReturnFileName[ nLth - 1 ] != '/' )
      {
         pchReturnFileName[ nLth ] = '\\';
         nLth++;
         pchReturnFileName[ nLth ] = 0;
      }

      strcpy_s( pchReturnFileName + nLth, nMaxNameLth - nLth, cpcFileName );
      strcat_s( pchReturnFileName, nMaxNameLth, szGuid );
      if ( cpcExtension && *cpcExtension )
      {
         strcat_s( pchReturnFileName, nMaxNameLth, "." );
         strcat_s( pchReturnFileName, nMaxNameLth, cpcExtension );
      }

      return( TRUE );
   }

   pchReturnFileName[ 0 ] = 0;
   return( FALSE );
}

//./ ADD NAME=GenerateUniqueFileNameForWEB
// Source Module=zdrprint.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GenerateUniqueFileNameForWEB
//                                              04/11/01 Modified: 04/11/01
//
//  PURPOSE:   To generate a unique file name give a specified path, partial
//             file name, and extension.
//
//  PARAMETERS: vSubtask     - View to current window.
//              pchReturnFileName - Return file name
//              nMaxNameLth  - Maximum length of return file name.
//              cpcFileName  - A partial file name (uniqueness will be
//                             attained by appending unique characters.
//              cpcExtension - The file name extension (may be omitted).
//
//  RETURNS:   FALSE - Could not generate unique name
//             TRUE  - Unique name generated and returned successfully
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 6
zOPER_EXPORT zBOOL OPERATION
GenerateUniqueFileNameForWEB( zVIEW   vSubtask,
                              zPCHAR  pchReturnFileName,
                              zSHORT  nMaxNameLth,
                              zCPCHAR cpcFileName,
                              zCPCHAR cpcExtension )
{
   zCHAR  szDocPath[ zMAX_FILENAME_LTH ];
   int   nLth;

   GetAppOrWorkstationValue( vSubtask, "ExecutableRemoteDocPath", szDocPath, zsizeof( szDocPath ) );
   nLth = (zSHORT) zstrlen( szDocPath );
   if ( nLth && szDocPath[ nLth - 1 ] != '\\' )
   {
      szDocPath[ nLth ] = '\\';
      nLth++;
      szDocPath[ nLth ] = 0;
   }

   return( GenerateUniqueFileName( pchReturnFileName, nMaxNameLth,
                                   szDocPath, cpcFileName, cpcExtension ) );
}

#ifdef zREMOTE_SERVER

//./ ADD NAME=fnExchChar
// Source Module=zdrprint.cpp
//**************************************************************************/
//
//  ENTRY:       ExchChar
//                                            5/22/2001 (us)
//  PURPOSE:     Change all Characters "cFrom" in the string "pszStr" into
//               "cTo"
//
//  PARAMETERS:  zPCHAR  pchStr -
//               zCHAR cFrom -
//               zCHAR cTo -
/**************************************************************************/
//./END + 4
void
fnExchChar( zPCHAR pchStr,
            zCHAR cFrom,
            zCHAR cTo )
{
   zPCHAR pch = pchStr;

   while ( *pch )
   {
      if ( *pch == cFrom )
         *pch = cTo;

      pch++;
   }
}

//./ ADD NAME=fnFtpPutFile
// Source Module=
/////////////////////////////////////////////////////////////////////////////
//
//                                           05/15/01 (us) Modified: 05/15/01
//
//  PURPOSE:  Send a local file to any host that has an ftp server running
//
//  RESTRICTIONS:
//     - the current setup of windows for proxy settings is used
//
//  PARAMETERS:
//     pszFtpHostName -
//        Host name of the target host
//     pszFtpUserName -
//        user name on the target host
//     pszFtpPassword -
//        user password on the target host
//     iPort -
//        port the ftp server on the target host is running on
//        if 0 the default port 21 is used
//     pszFullSourceFileName -
//        path and filename of the local source file
//        relative path and filenames are allowed
//     pszFullDestinationFileName -
//        path and filename of the destination file
//        relative path and filenames are allowed
//        be careful to use the right direcory separators ('\' or '/')
//        depending of the server operating system
//     iTextMode -
//        if this variable is 0 the file is sent binary else in text mode
//
//  RETURNS:   0 - file sent successfully
//            -1 - Error opening Internet session
//            -2 - Wrong user name or password
//            -3 - DNS could not resolve hostname
//            -4 - Error connecting to ftp connection
//            -5 - Error sending the file
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 8
int
fnFtpPutFile( zCPCHAR pszFtpHostName,
              zCPCHAR pszFtpUserName,
              zCPCHAR pszFtpPassword,
              int   iPort,
              zCPCHAR pszFullSourceFileName,
              zCHAR      *pszFullDestinationFileName,
              const int  iTextMode )
{
   HINTERNET hInternetSession;
   HINTERNET hFtpSession;
   DWORD     dwPutFlags;

   hInternetSession = InternetOpenA( "ZeidonWeb", INTERNET_OPEN_TYPE_PRECONFIG, 0, 0, 0 );
   if ( hInternetSession == 0 )
   {
      TraceLineS( "FTP:", "Error opening Internet Session" );
      return( -1 );
   }

   if ( iPort == 0 ) // use default ftp Port
      iPort = 21;

   hFtpSession = InternetConnectA( hInternetSession,
                                   pszFtpHostName,
                                   iPort,
                                   pszFtpUserName,
                                   pszFtpPassword,
                                   INTERNET_SERVICE_FTP,
                                   0, 0 );

   if ( hFtpSession == 0 )
   {
      DWORD error;

      error = GetLastError( );
      if ( (error == ERROR_INTERNET_INCORRECT_USER_NAME) ||
           (error == ERROR_INTERNET_INCORRECT_PASSWORD) )
      {
         TraceLineS( "FTP:", "Incorrect Username or Password");
         return( -2 );
      }
      else
      if ( error == ERROR_INTERNET_NAME_NOT_RESOLVED )
      {
         TraceLineS( "FTP:", "Hostname could not be resolved" );
         return( -3 );
      }
      else
      {
         TraceLineS( "FTP:", "Error connecting to FTP Server" );
         return( -4 );
      }
   }

/* if ( FtpSetCurrentDirectoryA( hFtpSession, pszDestinationPath ) == FALSE )
   {
      DWORD error;

      error = GetLastError();
      printf( "FTP SetDirectory Error: %ld\n", error );
      return( -3 );
   }
*/
   fnExchChar( pszFullDestinationFileName, '\\', '/' );

   if ( iTextMode == 0 )
      dwPutFlags = FTP_TRANSFER_TYPE_BINARY;
   else
      dwPutFlags = FTP_TRANSFER_TYPE_ASCII;

   if ( FtpPutFileA( hFtpSession,
                     pszFullSourceFileName,
                     pszFullDestinationFileName,
                     dwPutFlags, 0 ) == FALSE )
   {
      DWORD dwError = GetLastError( );
      zPVOID pvMsgBuf = 0;
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                     0, dwError,
                     MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
                     &pvMsgBuf, 0, 0 );

      TraceLineI( "FTP: PutFile Error: ", dwError );
      // error code is good, error text is better
      TraceLineS( "FTP: PutFile Error Message: ", (zCPCHAR) pvMsgBuf );

      // Free the buffer.
      LocalFree( pvMsgBuf );
      return( -5 );
   }
   else
      TraceLineS( "FTP: File sent successfully to: ",
                  pszFullDestinationFileName );

   InternetCloseHandle( hFtpSession );
   InternetCloseHandle( hInternetSession );

   return( 0 );
}
#endif

//./ ADD NAME=StartBrowserWindowWithURL
// Source Module=zdrprint.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: StartBrowserWindowWithURL
//                                              04/11/01 Modified: 04/11/01
//
//  PURPOSE:   To start a browser on a WEB client.  Note that the cpcURL
//             and the cpcFileName parameters are mutully exclusive.
//
//  PARAMETERS: vSubtask    - View to current dialog window (subtask).
//              pchReturnFileName - Return generated file name.
//              nMaxNameLth - Maximum return file name length ... if zero,
//                            the file name will not be returned
//              cpcURL      - Complete URL specification.
//              cpcFileName - Name of the file for which the URL will be
//                            generated.  If the path for this file matches
//                            the path specified in the Zeidon.ini file for
//                            "ExecutableRemoteDocPath", the URL is generated
//                            by simply replacing the path with the value
//                            specified by "ExecutableRemoteDocURL".
//                            Otherwise, it is assumed that the file needs
//                            to be copied to the URL directory first.
//              ulControlFlag -  0 ==> File name is specified.
//                               1 ==> Generate a unique file name.
//                              16 ==> URL is completely specified.
//
//  RETURNS:   0 - Unique name generated and returned successfully
//            -1 - Error locating Window
//            -2 - Could not locate SocketSubtask
//            -3 - Could not rename file to generated (unique) file name
//            -4 - Invalid parameter combinations
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
StartBrowserWindowWithURL( zVIEW   vSubtask,
                           zPCHAR  pchReturnFileName,
                           zSHORT  nMaxNameLth,
                           zCPCHAR cpcURL,
                           zCPCHAR cpcFileName,
                           zULONG  ulControlFlag )
{
#ifdef zREMOTE_SERVER
   ZSubtask *pZSubtask;
   zSHORT   nRC = 0;

   if ( ((ulControlFlag & 0x00000010) && (cpcURL == 0 || *cpcURL == 0)) ||
        ((ulControlFlag & 0x00000010) == 0 && (cpcFileName == 0 || *cpcFileName == 0)) )
   {
      TraceLineS( "StartBrowserWindowWithURL error: ",
                  (ulControlFlag & 0x00000010) ? "URL not specified" :
                                                 "File name not specified" );
      return( -4 );
   }

   if ( pchReturnFileName && nMaxNameLth > 0 )
      pchReturnFileName[ 0 ] = 0;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      ZXRA_Item   *pXRA;
      ZSocketTask *pZSocketTask = pZSubtask->m_pZSocketTask;

      if ( pZSocketTask &&
           (pXRA = pZSocketTask->
             m_pCacheWndList->Find( *(pZSubtask->m_pzsRemoteWndTag),
                                    pZSubtask,
                                    "StartDocumentOnClient" )) != 0 )
      {
         zVIEW   vXRA;
         CString cs;
         zPCHAR  pchFileName;
         zCHAR   szReportFileName[ zMAX_FILENAME_LTH ];
         zCHAR   szDocPath[ zMAX_FILENAME_LTH ];
         zCHAR   szDocURL[ zMAX_FILENAME_LTH ];
         zCHAR   szFtpHostName[ zMAX_FILENAME_LTH ];
         zCHAR   szFtpUserName[ zMAX_FILENAME_LTH ];
         zCHAR   szFtpPassword[ zMAX_FILENAME_LTH ];
         zCHAR   szFtpPort[ 6 ];
         zCPCHAR cpcTag = 0;
         zSHORT  nLth;

         if ( (ulControlFlag & 0x00000010) == 0 )
         {
            // Get Doc URL name.
            GetAppOrWorkstationValue( vSubtask, "FtpHostName", szFtpHostName, zsizeof( szFtpHostName ) );
            GetAppOrWorkstationValue( vSubtask, "FtpUserName", szFtpUserName, zsizeof( szFtpUserName ) );
            GetAppOrWorkstationValue( vSubtask, "FtpPassword", szFtpPassword, zsizeof( szFtpPassword ) );
            GetAppOrWorkstationValue( vSubtask, "FtpPort", szFtpPort, zsizeof( szFtpPort ) );
            GetAppOrWorkstationValue( vSubtask, "ExecutableRemoteDocURL", szDocURL, zsizeof( szDocURL ) );
            nLth = (zSHORT) zstrlen( szDocURL );
            if ( nLth && szDocURL[ nLth - 1 ] != '/' )
            {
               szDocURL[ nLth ] = '/';
               nLth++;
               szDocURL[ nLth ] = 0;
            }

            GetAppOrWorkstationValue( vSubtask, "ExecutableRemoteDocPath", szDocPath, zsizeof( szDocPath ) );
            nLth = (zSHORT) zstrlen( szDocPath );
            if ( nLth && szDocPath[ nLth - 1 ] != '\\' )
            {
               szDocPath[ nLth ] = '\\';
               nLth++;
               szDocPath[ nLth ] = 0;
            }

            pchFileName = new char[ zstrlen( cpcFileName ) + 1 ];
            strcpy_s( pchFileName, zstrlen( cpcFileName ) + 1, cpcFileName );
            cs = cpcFileName;
            cs.MakeLower( );

            CString csDocPath = szDocPath;
            csDocPath.MakeLower( );

            zPCHAR pchExt = zstrrchr( pchFileName, '.' );
            if ( pchExt )
            {
               *pchExt = 0;
               pchExt++;
            }
            else
               pchExt = "doc";

            zPCHAR pchName = zstrrchr( pchFileName, '\\' );
            if ( pchName )
               pchName++;
            else
               pchName = "";

            if ( cs.Find( csDocPath ) > 0 )
            {
               // The file is already in the URL directory.
               if ( ulControlFlag & 0x00000001 ) // generate unique file name
               {
                  GenerateUniqueFileName( szReportFileName,
                                          zsizeof( szReportFileName ),
                                          szDocPath, pchName, pchExt );

                  if ( fnFtpPutFile( szFtpHostName, szFtpUserName, szFtpPassword,
                                     atoi( szFtpPort ), cpcFileName, szReportFileName, 0 ) < 0 )
                  {
                     return( -3 );
                  }
                  else
                  {
                     cpcTag = szReportFileName + csDocPath.GetLength( );
                     strcat_s( szDocURL, zsizeof( szDocURL ), cpcTag );
                  }
               }
               else
               {
                  cpcTag = szReportFileName + csDocPath.GetLength( );
                  strcat_s( szDocURL, zsizeof( szDocURL ), cpcTag );
               }
            }
            else
            {
               // The file needs to be copied to the URL directory.
               GenerateUniqueFileName( szReportFileName, zsizeof( szReportFileName ), szDocPath, pchName, pchExt );
               if ( fnFtpPutFile( szFtpHostName,
                                  szFtpUserName,
                                  szFtpPassword,
                                  atoi( szFtpPort ),
                                  cpcFileName,
                                  szReportFileName,
                                  0 ) < 0 )
               {
                  return( -3 );
               }
               else
               {
                  cpcTag = szReportFileName + csDocPath.GetLength( );
                  strcat_s( szDocURL, zsizeof( szDocURL ), cpcTag );
               }
            }

            if ( pchReturnFileName && nMaxNameLth > 0 )
               strncat_s( pchReturnFileName, nMaxNameLth, szReportFileName, nMaxNameLth - 1 );
         }
         else
         {
            cpcTag = cpcURL;
            strcpy_s( szDocURL, zsizeof( szDocURL ), cpcURL );
         }

         if ( pXRA->m_vXRA_Delta == 0 )
         {
            pXRA->ActivateEmptyDelta( pZSubtask->m_pZSocketTask->m_vRegisteredSubtask,
                                      pZSubtask->m_pZSocketTask->m_szAppTag );
         }

         CreateViewFromViewForTask( &vXRA, pXRA->m_vXRA_Delta, m_pZSubtask->m_vDialog );
         while ( ResetViewFromSubobject( vXRA ) == 0 )
         {
         }

         CreateEntity( vXRA, "Doc", zPOS_AFTER );
         SetAttributeFromString( vXRA, "Doc", "Tag", cpcTag );
         SetAttributeFromString( vXRA, "Doc", "URL", szDocURL );
         TraceLineS( "StartBrowserWindowWithURL started URL: ", szDocURL );

         DropView( vXRA );
         return( nRC );
      }
      else
      {
         TraceLineS( "RemoteServer error condition ... cannot find: ",
                     pZSubtask->m_pZSocketTask->m_szRemoteWndTag );
      }

      return( -2 );
   }

#endif

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// There are a few circumstances where the code won't work (or appears not
// to work):
//
// If you leave out the SendMessageTimeout call, no other application
// recognizes the change until the application is restarted.
// If a different 32-bit application does not handle the WM_SETTINGCHANGE
// message, then it appears to that application as if the default printer
// has not been changed.  You will need to exit and re-enter the application
// to force it to recognize the change.
//
// MAXBUFFERSIZE is too small to hold the following string:
//
// printername,portname,drivername
// When using the WriteProfileString API in the earlier sample code (which
// is only necessary for Windows NT 4.0 and earlier), keep in mind that the
// port name passed to WriteProfileString was retrieved using the 32-bit
// API GetPrinter.  This port name, although valid, is not readable by
// 16-bit applications and may cause problems for 16-bit applications.  If
// this method is to be used under Windows NT (4.0 and earlier) and 16-bit
// applications need to understand the new port name, then the 16-bit alias
// for the port name ("NE0X:" where X is a number) should be used.  The
// mapping between 32-bit port names and their 16-bit aliases can be found
// in the registry.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <winspool.h>

// Size of internal buffer used to hold "printername,drivername,portname"
// string.  Increasing this may be necessary for huge strings.
#define MAXBUFFERSIZE 250

/////////////////////////////////////////////////////////////////////////////
// GetDefaultPrinterName
//
// Parameters:
//   cpcPrinterName: Buffer alloc'd by caller to hold printer name.
//   pdwBufferSize: On input, ptr to size of cpcPrinterName.
//          On output, min required size of cpcPrinterName.
//
// NOTE: You must include enough space for the null terminator!
//
// Returns: TRUE for success, FALSE for failure.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
GetDefaultPrinterName( zPCHAR pchPrinterName, zPLONG plBufferSize )
{
   zBOOL bFlag;
   OSVERSIONINFO osv;
   TCHAR cBuffer[ MAXBUFFERSIZE ];
   PRINTER_INFO_2 *ppi2 = 0;
   DWORD dwNeeded = 0;
   DWORD dwReturned = 0;

   // What version of Windows are you running?
   osv.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   GetVersionEx( &osv );

   // If Windows 95 or 98, use EnumPrinters...
   if ( osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
   {
      // The first EnumPrinters( ) tells you how big our buffer should
      // be in order to hold ALL of PRINTER_INFO_2. Note that this will
      // usually return FALSE. This only means that the buffer (the 4th
      // parameter) was not filled in. You don't want it filled in here...
      EnumPrinters( PRINTER_ENUM_DEFAULT, 0, 2, 0, 0, &dwNeeded, &dwReturned );
      if ( dwNeeded == 0 )
         return( FALSE );

      // Allocate enough space for PRINTER_INFO_2...
      ppi2 = (PRINTER_INFO_2 *) GlobalAlloc( GPTR, dwNeeded );
      if ( ppi2 == 0 )
         return( FALSE );

      // The second EnumPrinters( ) will fill in all the current information...
      bFlag = EnumPrinters( PRINTER_ENUM_DEFAULT, 0, 2, (LPBYTE) ppi2, dwNeeded, &dwNeeded, &dwReturned );
      if ( bFlag == FALSE )
      {
         GlobalFree( ppi2 );
         return( FALSE );
      }

      // If given buffer too small, set required size and fail...
      if ( zstrlen( ppi2->pPrinterName ) >= (zULONG) *plBufferSize )
      {
         *plBufferSize = zstrlen( ppi2->pPrinterName ) + 1;
         GlobalFree( ppi2 );
         return( FALSE );
      }

      // Copy printer name into passed-in buffer...
      strcpy_s( pchPrinterName, *plBufferSize, ppi2->pPrinterName );

      // Set buffer size parameter to min required buffer size...
      *plBufferSize = zstrlen( ppi2->pPrinterName ) + 1;
   }
   else
   if ( osv.dwPlatformId == VER_PLATFORM_WIN32_NT )
   {
      // If Windows NT, use the GetDefaultPrinter API for Windows 2000,
      // or GetProfileString for version 4.0 and earlier...
#if( WINVER >= 0x0500 )
      if ( osv.dwMajorVersion >= 5 ) // Windows 2000 or later
      {
         bFlag = GetDefaultPrinter( pchPrinterName, (DWORD *) plBufferSize );
         if ( bFlag == FALSE )
            return( FALSE );
      }
      else // NT4.0 or earlier
#endif
      {
         // Retrieve the default string from Win.ini (the registry).
         // String will be in form "printername,drivername,portname".
         if ( GetProfileString( "windows", "device", ",,,", cBuffer, MAXBUFFERSIZE ) <= 0 )
         {
            return( FALSE );
         }

         // Printer name precedes first "," character...
         strtok( cBuffer, "," );

         // If given buffer too small, set required size and fail ...
         if ( zstrlen( cBuffer ) >= (zULONG) *plBufferSize )
         {
            *plBufferSize = zstrlen( cBuffer ) + 1;
            return( FALSE );
         }

         // Copy printer name into passed-in buffer...
         strcpy_s( pchPrinterName, *plBufferSize, cBuffer );

         // Set buffer size parameter to min required buffer size...
         *plBufferSize = zstrlen( cBuffer ) + 1;
      }
   }

   // Cleanup...
   if ( ppi2 )
      GlobalFree( ppi2 );

   return( TRUE );
}

#undef MAXBUFFERSIZE

/////////////////////////////////////////////////////////////////////////////
// SetDefaultPrinterName
//
// Parameters:
//   pchPrinterName: Valid name of existing printer to make default.
//
// Returns: TRUE for success, FALSE for failure.
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
SetDefaultPrinterName( zPCHAR pchPrinterName )
{
   zBOOL bFlag;
   OSVERSIONINFO osv;
   DWORD dwNeeded = 0;
   HANDLE hPrinter = 0;
   PRINTER_INFO_2 *ppi2 = 0;
   zPCHAR pBuffer = 0;
   zLONG lResult;

   // What version of Windows are you running?
   osv.dwOSVersionInfoSize = sizeof( OSVERSIONINFO );
   GetVersionEx( &osv );

   if ( pchPrinterName == 0 )
      return( FALSE );

   // If Windows 95 or 98, use SetPrinter...
   if ( osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS )
   {
      // Open this printer so you can get information about it...
      bFlag = OpenPrinter( pchPrinterName, &hPrinter, 0 );
      if ( bFlag == FALSE || hPrinter == 0 )
          return( FALSE );

      // The first GetPrinter( ) tells you how big our buffer should
      // be in order to hold ALL of PRINTER_INFO_2.  Note that this will
      // usually return FALSE.  This only means that the buffer (the 3rd
      // parameter) was not filled in.  You don't want it filled in here...
      GetPrinter( hPrinter, 2, 0, 0, &dwNeeded );
      if ( dwNeeded == 0 )
      {
         ClosePrinter( hPrinter );
         return( FALSE );
      }

      // Allocate enough space for PRINTER_INFO_2...
      ppi2 = (PRINTER_INFO_2 *) GlobalAlloc( GPTR, dwNeeded );
      if ( ppi2 == 0 )
      {
         ClosePrinter( hPrinter );
         return( FALSE );
      }

      // The second GetPrinter( ) will fill in all the current information
      // so that all you need to do is modify what you're interested in...
      bFlag = GetPrinter( hPrinter, 2, (LPBYTE) ppi2, dwNeeded, &dwNeeded );
      if ( bFlag == FALSE )
      {
         ClosePrinter( hPrinter );
         GlobalFree( ppi2 );
         return( FALSE );
      }

      // Set default printer attribute for this printer...
      ppi2->Attributes |= PRINTER_ATTRIBUTE_DEFAULT;
      bFlag = SetPrinter( hPrinter, 2, (LPBYTE) ppi2, 0 );
      if ( bFlag == FALSE )
      {
         ClosePrinter( hPrinter );
         GlobalFree( ppi2 );
         return( FALSE );
      }

      // Tell all open applications that this change occurred.
      // Allow each application 1 second to handle this message.
      lResult = SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0L,
                                    (LPARAM)(LPCTSTR) "windows", SMTO_NORMAL, 1000, 0 );
   }
   else
   if ( osv.dwPlatformId == VER_PLATFORM_WIN32_NT )
   {
      // If Windows NT, use the SetDefaultPrinter API for Windows 2000,
      // or WriteProfileString for version 4.0 and earlier...
#if( WINVER >= 0x0500 )
      if ( osv.dwMajorVersion >= 5 ) // Windows 2000 or later...
      {
         bFlag = SetDefaultPrinter( pchPrinterName );
         if ( bFlag == FALSE )
            return( FALSE );
      }
      else // NT4.0 or earlier...
#endif
      {
         // Open this printer so you can get information about it...
         bFlag = OpenPrinter( pchPrinterName, &hPrinter, 0 );
         if ( bFlag == FALSE || hPrinter == 0 )
            return( FALSE );

         // The first GetPrinter( ) tells you how big our buffer should
         // be in order to hold ALL of PRINTER_INFO_2. Note that this will
         // usually return FALSE. This only means that the buffer (the 3rd
         // parameter) was not filled in. You don't want it filled in here...
         GetPrinter( hPrinter, 2, 0, 0, &dwNeeded );
         if ( dwNeeded == 0 )
         {
            ClosePrinter( hPrinter );
            return( FALSE );
         }

         // Allocate enough space for PRINTER_INFO_2...
         ppi2 = (PRINTER_INFO_2 *) GlobalAlloc( GPTR, dwNeeded );
         if ( ppi2 == 0 )
         {
            ClosePrinter( hPrinter );
            return( FALSE );
         }

         // The second GetPrinter( ) fills in all the current<BR/>
         // information...
         bFlag = GetPrinter( hPrinter, 2, (LPBYTE) ppi2, dwNeeded, &dwNeeded );
         if ( (bFlag == FALSE) || (ppi2->pDriverName == 0) ||
              (ppi2->pPortName == 0) )
         {
            ClosePrinter( hPrinter );
            GlobalFree( ppi2 );
            return( FALSE );
         }

         // Allocate buffer big enough for concatenated string.
         // String will be in form "printername,drivername,portname"...
         zLONG lLth = zstrlen( pchPrinterName ) + zstrlen( ppi2->pDriverName ) + zstrlen( ppi2->pPortName ) + 3;
         pBuffer = (zPCHAR) GlobalAlloc( GPTR, lLth );
         if ( pBuffer == 0 )
         {
            ClosePrinter( hPrinter );
            GlobalFree( ppi2 );
            return( FALSE );
         }

         // Build string in form "printername,drivername,portname"...
         strcpy_s( pBuffer, lLth, pchPrinterName );
         strcat_s( pBuffer, lLth, "," );
         strcat_s( pBuffer, lLth, ppi2->pDriverName );
         strcat_s( pBuffer, lLth, "," );
         strcat_s( pBuffer, lLth, ppi2->pPortName );

         // Set the default printer in Win.ini and registry...
         bFlag = WriteProfileString( "windows", "device", pBuffer );
         if ( bFlag == FALSE )
         {
            ClosePrinter( hPrinter );
            GlobalFree( ppi2 );
            GlobalFree( pBuffer );
            return( FALSE );
         }
      }

      // Tell all open applications that this change occurred.
      // Allow each app 1 second to handle this message.
      lResult = SendMessageTimeout( HWND_BROADCAST, WM_SETTINGCHANGE, 0L, 0L, SMTO_NORMAL, 1000, 0 );
   }

   // Cleanup...
   if ( hPrinter )
      ClosePrinter( hPrinter );

   if ( ppi2 )
      GlobalFree( ppi2 );

   if ( pBuffer )
      GlobalFree( pBuffer );

   return( TRUE );
}


// Creating a DEVMODE and DEVNAMES Structure.
// The code below uses GetPrinter( ) to create DEVMODE and DEVNAMES
// structures based on a printer name.  The code is a global function which
// returns the data structures through its parameters.

// Returns a DEVMODE and DEVNAMES for the printer name specified.
zBOOL
GetPrinterDevice( zPCHAR  pchPrinterName,
                  HGLOBAL *phDevNames,
                  HGLOBAL *phDevMode )
{
   ASSERT( phDevMode && phDevNames );

   // Open printer.
   HANDLE hPrinter;

   *phDevMode = 0;
   *phDevNames = 0;
   if ( OpenPrinter( pchPrinterName, &hPrinter, 0 ) == FALSE )
      return( FALSE );

   // Obtain PRINTER_INFO_2 structure and close printer.
   DWORD dwBytesReturned;
   DWORD dwBytesNeeded;

   GetPrinter( hPrinter, 2, 0, 0, &dwBytesNeeded );
   PRINTER_INFO_2 *p2 = (PRINTER_INFO_2 *) GlobalAlloc( GPTR, dwBytesNeeded );
   if ( GetPrinter( hPrinter, 2, (LPBYTE) p2, dwBytesNeeded, &dwBytesReturned ) == 0 )
   {
      GlobalFree( p2 );
      ClosePrinter( hPrinter );
      return( FALSE );
   }

   ClosePrinter( hPrinter );

   // Allocate a global handle for DEVMODE.
   HGLOBAL hDevMode;
   hDevMode = GlobalAlloc( GHND, sizeof( *p2->pDevMode ) + p2->pDevMode->dmDriverExtra );
   ASSERT( hDevMode );
   DEVMODE *pDevMode = (DEVMODE *) GlobalLock( hDevMode );
   ASSERT( pDevMode );

   // Copy DEVMODE data from PRINTER_INFO_2::pDevMode
   zmemcpy( pDevMode, p2->pDevMode, sizeof( *(p2->pDevMode) ) + p2->pDevMode->dmDriverExtra );
   GlobalUnlock( hDevMode );

   // Compute size of DEVNAMES structure from PRINTER_INFO_2's data.
   zUSHORT uDriverNameLth = (zSHORT) zstrlen( p2->pDriverName ) + 1;     // driver name
   zUSHORT uPrinterNameLth = (zSHORT) zstrlen( p2->pPrinterName ) + 1;   // printer name
   zUSHORT uPortNameLth = (zSHORT) zstrlen( p2->pPortName ) + 1;         // port name

   // Allocate a global handle big enough to hold DEVNAMES.
   HGLOBAL hDevNames = GlobalAlloc( GHND, sizeof( DEVNAMES ) + (uDriverNameLth + uPrinterNameLth + uPortNameLth) * sizeof( TCHAR ) );
   ASSERT( hDevNames );
   DEVNAMES *pDevNames = (DEVNAMES *) GlobalLock( hDevNames );
   ASSERT( pDevNames );

   // Copy the DEVNAMES information from PRINTER_INFO_2
   // lOffset = TCHAR Offset into structure.
   zUSHORT uOffset = sizeof( DEVNAMES ) / sizeof( TCHAR );
   ASSERT( sizeof( DEVNAMES ) == uOffset * sizeof( TCHAR ) );

   pDevNames->wDriverOffset = uOffset;
   zmemcpy( (zPCHAR) pDevNames + uOffset, p2->pDriverName, uDriverNameLth * sizeof( TCHAR ) );
   uOffset += uDriverNameLth;

   pDevNames->wDeviceOffset = uOffset;
   zmemcpy( (zPCHAR) pDevNames + uOffset, p2->pPrinterName, uPrinterNameLth * sizeof( TCHAR ) );
   uOffset += uPrinterNameLth;

   pDevNames->wOutputOffset = uOffset;
   zmemcpy( pDevNames + uOffset, p2->pPortName, uPortNameLth*sizeof( TCHAR ) );
   pDevNames->wDefault = 0;

   GlobalUnlock( hDevNames );
   GlobalFree( p2 );   // free PRINTER_INFO_2

   // Set the new hDevMode and hDevNames.
   *phDevMode = hDevMode;
   *phDevNames = hDevNames;
   return( TRUE );
}
