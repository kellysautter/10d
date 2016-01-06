/////////////////////////////////////////////////////////////////////////////
//
// MODULE NAME:   kzrpsrc2.c -
// DESCRIPTION:   ??
//
// //////////////////////////////////////////////////////////////////////////
// * Copyright (c) 1995 QuinSoft, Corporation.  All rights reserved.        *
// * Confidential and Proprietary material subject to license - do not      *
// * reproduce or disclose.  This material is an unpublished work and       *
// * is considered a trade secret belonging to the copyright holder.        *
// //////////////////////////////////////////////////////////////////////////
//
// AUTHOR:        ??
// DATE:          ??
// API:           ??
// ENVIRONMENT:   ??
// REVISION:      ??
//
// HISTORY:
//
/////////////////////////////////////////////////////////////////////////////
//

/*
CHANGE LOG

2005.09.05    DKS
   Added multi-root reset page numbering.

2002.06.21    DonC
   Added operation FormatSubobjectOnDocWithXRP and modified
   FormatSubobjectOnDoc to call it. This supports the Query Report function.

2001.11.05    DKS
   Added capability to drop the SourceOI view for the report.

2001.02.08    DKS  HPF
   After counting pages, print real report.

2001.01.23    DKS  HH/Bettina
   When printing multiple roots, reset cursor position to first entity.

2000.11.28    HH
   const zCHAR change FormatSubobjectOnDoc.

2000.10.19    SR   Z2000 Size of path variables
   Modified size of string szFileName in function FormatSubobjectOnDoc
   because size of attributes ExecDir, MetaSrcDir and PgmSrcDir has
   been changed to 254 in datamodel.

1999.02.08    DKS
    Added prototype for FormatSubobjectForDef
*/

#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "ZDRVROPR.H"
#include "ZEIDONOP.H"

zOPER_EXPORT zSHORT OPERATION
oTZRPSRCO_FormatSubobjectForDef( zVIEW     vReportDef,
                                 zVIEW     vSubtask,
                                 zVIEW     vSourceOI,
                                 zPCHAR    pchReportDefName,
                                 zPCHAR    pchSubobjectEntityName,
                                 zLONG     lPrintPreviewFlag,
                                 zLONG     lPrintDialogFlag,
                                 zLONG     lPrintFlags );

// Top level call to start a Zeidon Report.
zOPER_EXPORT zSHORT OPERATION
FormatSubobjectOnDoc( zVIEW     vSourceOI,
                      zCPCHAR   cpcSubobjectEntityName,
                      zVIEW     vSubtask,
                      zCPCHAR   cpcReportDefName,
                      zLONG     lFlag )   //  0 - Print directly to printer (no preview).
                                          // +1 - Print Preview.
                                          // +2 - Print Dialog.
                                          // +4 - Force 0, 1, or 2 ... do not use OE settings.
                                          // +8 - Print multiple entities at top level.
                                          // +16 - Close subtask when closing the print preview window.
                                          // +32 - Count the number of pages prior to printing.
                                          // +64 - After printing is complete, drop vSourceOI.
                                          // +128 - Create CSV File.
                                          // +256 - Create HTML File.
                                          // +512 - Fit To Page (not implemented).
                                          // +1024 - Reset Paging.
                                          // +2048 - Print PageFooter using PageHeader view.
                                          // +8192 - Trace OI.
                                          // +16384 - Cause MessageBox.
                                          // +32768 - Drive by ReportDef Instead of SourceOI.
{
   zVIEW  vReportDef;
   zVIEW  vAnimator;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   LPAPP  lpApp;
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
      SfGetApplicationForSubtask( &lpApp, vSubtask );
      if ( lpApp )
      {
         strcpy_s( szFileName, sizeof( szFileName ), lpApp->szObjectDir );
      }
   }
   else
   {
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         GetStringFromAttribute( szFileName, sizeof( szFileName ),
                                 vTaskLPLR, "LPLR", "ExecDir" );
         strcat_s( szFileName, sizeof( szFileName ), "\\" );
      }
   }

   strcat_s( szFileName, sizeof( szFileName ), cpcReportDefName );
   strcat_s( szFileName, sizeof( szFileName ), ".XRP" );

   // DGC 7/11/96 -- The following is a hack for compatability with Unix.
   // Unix file names are case-sensitive.  Since DOS/Windows files (when
   // copied to a unix system via a network) are assigned lower-case letters
   // in Unix, we will (for now) make all Zeidon System file names lowercase.
   _strlwr_s( szFileName, sizeof( szFileName ) );

   nRC = ActivateOI_FromFile( &vReportDef, "TZRPSRCO", vSubtask, szFileName, zACTIVATE_SYSTEM );

   if ( nRC >= 0 )
   {
      FormatSubobjectOnDocWithXRP( vSourceOI,
                                   cpcSubobjectEntityName,
                                   vSubtask,
                                   cpcReportDefName,
                                   vReportDef,
                                   lFlag );

      TraceLineS( "Format Report ===========================> ", szFileName );
   }
   else
      TraceLineS( "Cannot load Report ======================> ", szFileName );

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
FormatSubobjectOnDocWithXRP( zVIEW     vSourceOI,
                             zCPCHAR   cpcSubobjectEntityName,
                             zVIEW     vSubtask,
                             zCPCHAR   cpcReportDefName,
                             zVIEW     vReportDef,
                             zLONG     lFlag )  //  0 - Print directly to printer (no preview).
                                                // +1 - Print Preview.
                                                // +2 - Print Dialog.
                                                // +4 - Force 0, 1, or 2 ... do not use OE settings.
                                                // +8 - Print multiple entities at top level.
                                                // +16 - Close subtask when closing the print preview window.
                                                // +32 - Count the number of pages prior to printing.
                                                // +64 - After printing is complete, drop vSourceOI.
                                                // +128 - Create CSV File.
                                                // +256 - Create HTML File.
                                                // +512 - Fit To Page (not implemented).
                                                // +1024 - Reset Paging.
                                                // +2048 - Print PageFooter using PageHeader view.
                                                // +8192 - Trace OI.
                                                // +16384 - Cause MessageBox.
                                                // +32768 - Drive by ReportDef Instead of SourceOI.
{

   // This does the real work for FormatSubobjectOnDoc. The other just
   // activates the Report object and calls this operation.

#define IDM_GETPRINTPREVIEW  206
#define IDM_GETPRINTDIALOG   207

   zCHAR  szType[ 8 ];
   zLONG  lPrintPreviewFlag;
   zLONG  lPrintDialogFlag;
   zLONG  lPrintFlags;
   zLONG  lExtent;
   zLONG  lMarginTop;
   zLONG  lMarginLeft;
   zLONG  lUnit;
   zLONG  lType;
   zLONG  lReportTop;
   zLONG  lReportLeft;
   zLONG  lPosX;
   zLONG  lPosY;
   zLONG  lSizeX;
   zLONG  lSizeY;
   zBOOL  bNoRows = FALSE;
   zSHORT nRC;

   lExtent = 0;
   lMarginTop = lMarginLeft = 999999;

   GetIntegerFromAttribute( &lPrintFlags, vReportDef,
                            "Report", "PrintFlags" );
   GetIntegerFromAttribute( &lUnit, vReportDef, "Report", "Unit" );
   if ( lUnit == 0 )  // convert units on the fly!
   {
      TraceLineS( "FormatSubobjectOnDoc CONVERTING report: ",
                  cpcReportDefName );
      GetIntegerFromAttribute( &lSizeX, vReportDef,
                               "Report", "ReportTextSize" );
      lSizeX *= 100;
      SetAttributeFromInteger( vReportDef, "Report",
                               "ReportTextSize", lSizeX );

      nRC = SetCursorFirstEntity( vReportDef, "Page", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         GetIntegerFromAttribute( &lSizeX, vReportDef, "Page", "SizeX" );
         GetIntegerFromAttribute( &lSizeY, vReportDef, "Page", "SizeY" );
         lSizeX <<= 4;  // convert from sixteenths to 256ths
         lSizeY <<= 4;  // convert from sixteenths to 256ths
         SetAttributeFromInteger( vReportDef, "Page", "SizeX", lSizeX );
         SetAttributeFromInteger( vReportDef, "Page", "SizeY", lSizeY );

         nRC = SetCursorFirstEntity( vReportDef, "GroupSet", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lSizeX, vReportDef, "GroupSet", "SZDLG_X" );
            GetIntegerFromAttribute( &lSizeY, vReportDef, "GroupSet", "SZDLG_Y" );
            lSizeX <<= 4;  // convert from sixteenths to 256ths
            lSizeY <<= 4;  // convert from sixteenths to 256ths
            SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_X", lSizeX );
            SetAttributeFromInteger( vReportDef, "GroupSet", "SZDLG_Y", lSizeY );

            GetIntegerFromAttribute( &lPosX, vReportDef, "GroupSet", "PSDLG_X" );
            GetIntegerFromAttribute( &lPosY, vReportDef, "GroupSet", "PSDLG_Y" );
            lPosX <<= 4;  // convert from sixteenths to 256ths
            lPosY <<= 4;  // convert from sixteenths to 256ths
            SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_X", lPosX );
            SetAttributeFromInteger( vReportDef, "GroupSet", "PSDLG_Y", lPosY );

            nRC = SetCursorFirstEntity( vReportDef, "Group", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lSizeX, vReportDef, "Group", "SZDLG_X" );
               GetIntegerFromAttribute( &lSizeY, vReportDef, "Group", "SZDLG_Y" );
               lSizeX <<= 4;  // convert from sixteenths to 256ths
               lSizeY <<= 4;  // convert from sixteenths to 256ths
               SetAttributeFromInteger( vReportDef, "Group", "SZDLG_X", lSizeX );
               SetAttributeFromInteger( vReportDef, "Group", "SZDLG_Y", lSizeY );

               GetIntegerFromAttribute( &lPosX, vReportDef, "Group", "PSDLG_X" );
               GetIntegerFromAttribute( &lPosY, vReportDef, "Group", "PSDLG_Y" );
               lPosX <<= 4;  // convert from sixteenths to 256ths
               lPosY <<= 4;  // convert from sixteenths to 256ths
               SetAttributeFromInteger( vReportDef, "Group", "PSDLG_X", lPosX );
               SetAttributeFromInteger( vReportDef, "Group", "PSDLG_Y", lPosY );

               nRC = SetCursorFirstEntity( vReportDef, "Control", 0 );
               while ( nRC >= zCURSOR_SET )
               {
                  GetIntegerFromAttribute( &lSizeX, vReportDef, "Control", "SZDLG_X" );
                  GetIntegerFromAttribute( &lSizeY, vReportDef, "Control", "SZDLG_Y" );
                  lSizeX <<= 4;  // convert from sixteenths to 256ths
                  lSizeY <<= 4;  // convert from sixteenths to 256ths
                  SetAttributeFromInteger( vReportDef, "Control", "SZDLG_X", lSizeX );
                  SetAttributeFromInteger( vReportDef, "Control", "SZDLG_Y", lSizeY );

                  GetIntegerFromAttribute( &lPosX, vReportDef, "Control", "PSDLG_X" );
                  GetIntegerFromAttribute( &lPosY, vReportDef, "Control", "PSDLG_Y" );
                  lPosX <<= 4;  // convert from sixteenths to 256ths
                  lPosY <<= 4;  // convert from sixteenths to 256ths
                  SetAttributeFromInteger( vReportDef, "Control", "PSDLG_X", lPosX );
                  SetAttributeFromInteger( vReportDef, "Control", "PSDLG_Y", lPosY );

                  GetIntegerFromAttribute( &lType, vReportDef, "ControlDef", "Key" );
                  if ( lType == 3020L ) // zTEXT_REPORT
                  {
                     zCHAR   szBlob[ 8 + (6 * sizeof( long )) + LF_FACESIZE ];
                     zULONG  ulLth = sizeof( szBlob );
                     zLONG   lSize;
                     GetBlobFromAttribute( szBlob, &ulLth, vReportDef,
                                           "Control", "CtrlBOI" );
                     if ( ulLth )
                     {
                        lSize = *((zPLONG) (szBlob + sizeof( long )));
                        if ( lSize != 0 )
                        {
                           if ( lSize > 0 )
                              lSize--;
                           else
                              lSize++;

                           lSize *= 10;
                           *((zPLONG) (szBlob + sizeof( long ))) = lSize;
                           SetAttributeFromBlob( vReportDef, "Control",
                                                 "CtrlBOI", szBlob, ulLth );
                        }
                     }
                  }

                  nRC = SetCursorNextEntity( vReportDef, "Control", 0 );
               }

               nRC = SetCursorNextEntity( vReportDef, "Group", 0 );
            }

            nRC = SetCursorNextEntity( vReportDef, "GroupSet", 0 );
         }

         nRC = SetCursorNextEntity( vReportDef, "Page", 0 );
      }

      SetCursorFirstEntity( vReportDef, "Page", 0 );
   }

   nRC = SetCursorFirstEntity( vReportDef, "Page", 0 );

   // Check to determine if we need to calculate Group extents on the fly.
   nRC = SetCursorFirstEntity( vReportDef, "Group", "Report" );
   if ( nRC >= zCURSOR_SET )
      GetIntegerFromAttribute( &lExtent, vReportDef, "Group", "Extent" );

   if ( lUnit == 0 || lExtent == 0 ||
        lMarginTop == 999999 || lMarginLeft == 999999 )
   {
      // calculate extent on the fly!
      nRC = SetCursorFirstEntity( vReportDef, "Page", 0 );
   }
   else
      nRC = -1;

   while ( nRC >= zCURSOR_SET )  // calculate extent on the fly!
   {
      nRC = SetCursorFirstEntity( vReportDef, "GroupSet", 0 );
      while ( nRC >= zCURSOR_SET )
      {
         lExtent = -1;  // indicate that Extent has been calculated.

         GetStringFromAttribute( szType, sizeof( szType ), vReportDef, "GroupSet", "Type" );
         if ( (szType[ 0 ] == 'P' || szType[ 0 ] == 'p') &&
              (szType[ 1 ] == 'H' || szType[ 1 ] == 'h') )
         {
            szType[ 0 ] = 'h';  // flag as page header
         }
         else
            szType[ 0 ] = 0;

         nRC = SetCursorFirstEntity( vReportDef, "Group", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            nRC = SetCursorFirstEntity( vReportDef, "Control", 0 );
            while ( nRC >= zCURSOR_SET )
            {
               GetIntegerFromAttribute( &lPosY, vReportDef,
                                        "Control", "PSDLG_Y" );
               GetIntegerFromAttribute( &lSizeY, vReportDef,
                                        "Control", "SZDLG_Y" );
               GetIntegerFromAttribute( &lPosX, vReportDef,
                                        "Control", "PSDLG_X" );
               if ( lSizeY + lPosY > lExtent )
                  lExtent = lSizeY + lPosY;

               if ( szType[ 0 ] == 'h' && lPosY < lMarginTop )
                  lMarginTop = lPosY;

               if ( lPosX < lMarginLeft )
                  lMarginLeft = lPosX;

               nRC = SetCursorNextEntity( vReportDef, "Control", 0 );
            }

            SetAttributeFromInteger( vReportDef, "Group",
                                     "Extent", lExtent );
            SetAttributeFromInteger( vReportDef, "Group",
                                     "MinimumExtent", lExtent );

            nRC = SetCursorNextEntity( vReportDef, "Group", 0 );
         }

         nRC = SetCursorNextEntity( vReportDef, "GroupSet", 0 );
      }

      nRC = SetCursorNextEntity( vReportDef, "Page", 0 );
   }

   // Set the correct margins in the report definition.
   GetIntegerFromAttribute( &lReportTop, vReportDef,
                            "Report", "MarginTop" );
   GetIntegerFromAttribute( &lReportLeft, vReportDef,
                            "Report", "MarginLeft" );
   if ( lReportTop && lMarginTop < lReportTop )
      SetAttributeFromInteger( vReportDef, "Report", "MarginTop",
                               lReportTop - lMarginTop );

   if ( lReportLeft && lMarginLeft < lReportLeft )
      SetAttributeFromInteger( vReportDef, "Report", "MarginLeft",
                               lReportLeft - lMarginLeft );

   // Check to determine if we need to calculate Control ranges on the fly.
   nRC = SetCursorFirstEntity( vReportDef, "Control", "Report" );
   if ( nRC >= zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lExtent, vReportDef,
                               "Control", "RangeBottom" );
      if ( lExtent == 0 )
      {
         zVIEW  vReportTmp;
         zLONG  lRangeTop;
         zLONG  lRangeBottom;
         zLONG  lPos;
         zLONG  lSize;
         zBOOL  bChange;

         CreateViewFromViewForTask( &vReportTmp, vReportDef, 0 );
         nRC = SetCursorFirstEntity( vReportDef, "GroupSet", "" );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            nRC = SetCursorFirstEntity( vReportDef, "Group", "" );
            while ( nRC > zCURSOR_UNCHANGED )
            {
               nRC = SetCursorFirstEntity( vReportDef, "Control", "" );
               bChange = FALSE;
               while ( nRC > zCURSOR_UNCHANGED )
               {
                  GetIntegerFromAttribute( &lRangeTop, vReportDef,
                                           "Control", "RangeTop" );
                  GetIntegerFromAttribute( &lRangeBottom, vReportDef,
                                           "Control", "RangeBottom" );
                  GetIntegerFromAttribute( &lPosY, vReportDef,
                                           "Control", "PSDLG_Y" );
                  GetIntegerFromAttribute( &lSizeY, vReportDef,
                                           "Control", "SZDLG_Y" );
                  lSizeY += lPosY;

                  if ( lRangeTop == 0 )
                  {
                     if ( lPosY )
                        lRangeTop = lPosY;
                     else
                        lRangeTop = -1;   // indicate that RangeTop has been calculated

                     SetAttributeFromInteger( vReportDef, "Control",
                                              "RangeTop", lRangeTop );
                  }

                  if ( lRangeBottom == 0 && lSizeY )
                  {
                     lRangeBottom = lSizeY;
                     SetAttributeFromInteger( vReportDef, "Control",
                                              "RangeBottom", lRangeBottom );
                  }

                  SetViewFromView( vReportTmp, vReportDef );
                  nRC = SetCursorFirstEntity( vReportTmp, "Control", "" );
                  while ( nRC > zCURSOR_UNCHANGED )
                  {
                     GetIntegerFromAttribute( &lPos, vReportTmp,
                                              "Control", "RangeTop" );
                     GetIntegerFromAttribute( &lSize, vReportTmp,
                                              "Control", "RangeBottom" );
                     if ( lPos == 0 )
                     {
                        GetIntegerFromAttribute( &lPos, vReportTmp,
                                                 "Control", "PSDLG_Y" );
                        GetIntegerFromAttribute( &lSize, vReportTmp,
                                                 "Control", "SZDLG_Y" );
                        lSize += lPos;
                        SetAttributeFromInteger( vReportTmp, "Control",
                                                 "RangeBottom", lSize );
                        if ( lPos == 0 )
                           lPos = -1;

                        SetAttributeFromInteger( vReportTmp, "Control",
                                                 "RangeTop", lPos );
                     }

                     if ( (lPos >= lRangeTop && lPos <= lRangeBottom) ||
                           (lSize >= lRangeTop && lSize <= lRangeBottom) ||
                           (lRangeTop >= lPos && lRangeTop <= lSize) ||
                           (lRangeBottom >= lPos && lRangeBottom <= lSize) )
                     {
                        if ( lPos < lRangeTop )
                        {
                           lRangeTop = lPos;
                           bChange = TRUE;
                           SetAttributeFromInteger( vReportDef, "Control",
                                                    "RangeTop", lRangeTop );
                        }

                        if ( lSize > lRangeBottom )
                        {
                           lRangeBottom = lSize;
                           bChange = TRUE;
                           SetAttributeFromInteger( vReportDef, "Control",
                                                    "RangeBottom", lRangeBottom );
                        }
                     }

                     nRC = SetCursorNextEntity( vReportTmp, "Control", "" );
                  }

                  if ( lRangeTop == 0 )
                     lRangeTop = -1;

                  SetAttributeFromInteger( vReportDef, "Control",
                                           "RangeTop", lRangeTop );
                  SetAttributeFromInteger( vReportDef, "Control",
                                           "RangeBottom", lRangeBottom );
                  if ( bChange )
                  {
                     bChange = FALSE;
                     nRC = SetCursorFirstEntity( vReportDef, "Control", "" );
                  }
                  else
                  {
                     nRC = SetCursorNextEntity( vReportDef, "Control", "" );
                  }
               }

               nRC = SetCursorFirstEntity( vReportTmp, "Control", "" );
               while ( nRC > zCURSOR_UNCHANGED )
               {
                  GetIntegerFromAttribute( &lRangeTop, vReportTmp,
                                           "Control", "RangeTop" );
                  GetIntegerFromAttribute( &lRangeBottom, vReportTmp,
                                           "Control", "RangeBottom" );
                  if ( lRangeTop == -1 )
                  {
                     lRangeTop = 0;
                     SetAttributeFromInteger( vReportTmp, "Control",
                                              "RangeTop", lRangeTop );
                  }

               // GetAddrForAttribute( &pch, vReportTmp, "Control", "Tag" );
               // TraceLineS( "Range Ctrl: ", pch );
               // TraceLineI( "       Top: ", lRangeTop );
               // TraceLineI( "    Bottom: ", lRangeBottom );
                  nRC = SetCursorNextEntity( vReportTmp, "Control", "" );
               }

               // Sort target controls by position.
               OrderEntityForView( vReportDef, "Control",
                                    "RangeBottom A PSDLG_Y A PSDLG_X A" );

               nRC = SetCursorNextEntity( vReportDef, "Group", "" );
            }

            nRC = SetCursorNextEntity( vReportDef, "GroupSet", "" );
         }

         DropView( vReportTmp );
      }
   }

   // Force PH and PF to have ph and pf on the fly!
   SetCursorFirstEntity( vReportDef, "Page", 0 );
   nRC = SetCursorFirstEntity( vReportDef, "GroupSet", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetStringFromAttribute( szType, sizeof( szType ), vReportDef, "GroupSet", "Type" );
      if ( szType[ 0 ] == 'P' &&
           (szType[ 1 ] == 'H' || szType[ 1 ] == 'F') )
      {
         nRC = SetCursorFirstEntity( vReportDef, "Group", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            SysTranslateString( szType, 'L' );
            SetAttributeFromString( vReportDef, "Group",
                                    "Type", szType );

            nRC = SetCursorNextEntity( vReportDef, "Group", 0 );
         }

         nRC = SetCursorNextEntity( vReportDef, "GroupSet", 0 );
      }

      nRC = SetCursorNextEntity( vReportDef, "Page", 0 );
   }

   SetCursorFirstEntity( vReportDef, "Page", 0 );

   // Interrogate "Print to printer", "Print Preview", "Print Dialog" and
   // "Force 0, 1, or 2 ... do not use OE settings" flag.
   if ( (lFlag & 0x00000004) == 0 )
   {
      lPrintPreviewFlag = SysSendOE_Message( IDM_GETPRINTPREVIEW, 0 );
      if ( lPrintPreviewFlag )
         lPrintPreviewFlag = 1;

      lPrintDialogFlag = SysSendOE_Message( IDM_GETPRINTDIALOG, 0 );
   }
   else
   {
      lPrintPreviewFlag = lFlag & zREPORT_PRINTPREVIEW;
      lPrintDialogFlag = lFlag & zREPORT_PRINTDIALOG;
   }

   // Knock out "Print to printer", "Print Preview", "Print Dialog" and
   // "Force 0, 1, or 2 ... do not use OE settings" flags (already
   // accounted for).
   lFlag &= ~0x00000007;

   if ( lFlag & 0x00000010 )      // Close subtask when closing
   {                              // the print preview window.
      lPrintPreviewFlag |= 2;
      lFlag &= ~0x00000010;       // cannot remove 2005.09.05 dks
      lPrintFlags &= ~0x00000010; // cannot remove 2005.09.05 dks
   }

   if ( (lFlag & 0x00000020) || (lPrintFlags & 0x00000020) )  // Count the number of pages
   {
   // TraceLineS( "HEY ... COUNTING PAGES", "" );
      lPrintPreviewFlag |= 4;     // prior to printing
      lFlag &= ~0x00000020;       // cannot remove 2005.09.05 dks
      lPrintFlags &= ~0x00000020; // cannot remove 2005.09.05 dks
   }

   if ( lPrintFlags & zREPORT_LANDSCAPE )
      lFlag |= zREPORT_LANDSCAPE;

   if ( lPrintFlags & zREPORT_PRINT2HTML )
      lFlag |= zREPORT_PRINT2HTML;

   if ( CheckExistenceOfEntity( vSourceOI, cpcSubobjectEntityName ) < zCURSOR_SET )
   {
      bNoRows = TRUE;
   }

   lPrintFlags |= lFlag;
   if ( lPrintFlags & zREPORT_MULTIPLE_ROOT )  // 0x00000008
   {
      // We could create a view and at the end reset the SourceOI to that
      // view (to preserve cursor position).
   // CreateViewFromViewForTask( &vSourceOI, vSourceOI );
   // SetNameForView( vSourceOI, "__ZeidonPrint", vSubtask, zLEVEL_SUBTASK );
      if ( bNoRows == FALSE )
      {
         SetCursorFirstEntity( vSourceOI, cpcSubobjectEntityName, 0 );
      }

   // lPrintFlags &= ~zREPORT_MULTIPLE_ROOT;
   }

   SetAttributeFromInteger( vReportDef, "Report", "TotalPageCnt", -1 );

   if ( bNoRows )
   {
      zCHAR szMsg[ 1024 ];
      strcpy_s( szMsg, sizeof( szMsg ), "Rows to print report: Report = " );
      strcat_s( szMsg, sizeof( szMsg ), cpcReportDefName );
      strcat_s( szMsg, sizeof( szMsg ), " Entity = ");
      strcat_s( szMsg, sizeof( szMsg ), cpcSubobjectEntityName );
      TraceLineS( szMsg,  "" );
   }
   else
   {
      nRC = oTZRPSRCO_FormatSubobjectForDef( vReportDef, vSubtask,
                                             vSourceOI,
      /* remove cast after VML change */     (zPCHAR) cpcReportDefName,
      /* remove cast after VML change */     (zPCHAR) cpcSubobjectEntityName,
                                             lPrintPreviewFlag,
                                             lPrintDialogFlag,
                                             lPrintFlags );
   }

   return( nRC );
}

// Top level call to start a Zeidon Web Report.
zOPER_EXPORT zSHORT OPERATION
FormatWebReport( zVIEW     vSourceOI,
                 zCPCHAR   cpcSubobjectEntityName,
                 zVIEW     vSubtask,
                 zCPCHAR   cpcReportDefName,
                 zCPCHAR   cpcReportFileName,
                 zLONG     lFlag )        // +8 - Print multiple entities at top level.
                                          // +64 - After printing is complete, drop vSourceOI.
                                          // +256 - Create HTML File.
                                          // +2048 - Print PageFooter using PageHeader view.
                                          // +32768 - Drive by ReportDef Instead of SourceOI.
{
   zVIEW  vReportDef;
   zVIEW  vAnimator;
   zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   LPAPP  lpApp;
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
      SfGetApplicationForSubtask( &lpApp, vSubtask );
      if ( lpApp )
      {
         strcpy_s( szFileName, sizeof( szFileName ), lpApp->szObjectDir );
      }
   }
   else
   {
      if ( GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK ) >= 0 )
      {
         GetStringFromAttribute( szFileName, sizeof( szFileName ),
                                 vTaskLPLR, "LPLR", "ExecDir" );
         strcat_s( szFileName, sizeof( szFileName ), "\\" );
      }
   }

   strcat_s( szFileName, sizeof( szFileName ), cpcReportDefName );
   strcat_s( szFileName, sizeof( szFileName ), ".XRP" );

   // DGC 7/11/96 -- The following is a hack for compatability with Unix.
   // Unix file names are case-sensitive.  Since DOS/Windows files (when
   // copied to a unix system via a network) are assigned lower-case letters
   // in Unix, we will (for now) make all Zeidon System file names lowercase.
   _strlwr_s( szFileName, sizeof( szFileName ) );

   nRC = ActivateOI_FromFile( &vReportDef, "TZRPSRCO", vSubtask, szFileName, zACTIVATE_SYSTEM );

   if ( nRC >= 0 )
   {
      // Force On: "Create HTML File" and "Drive by ReportDef Instead of SourceOI".
      lFlag |= (256 | 32768);

      // Force Off: "Print Preview" and "Print Dialog".
      lFlag &= ~(1 | 2);

      FormatSubobjectOnDocWithXRP( vSourceOI,
                                   cpcSubobjectEntityName,
                                   vSubtask,
                                   cpcReportDefName,
                                   vReportDef,
                                   lFlag );

      TraceLineS( "Format Report ===========================> ", szFileName );
   }
   else
      TraceLineS( "Cannot load Report ======================> ", szFileName );

   return( nRC );
}

// Top level call to generate a PDF using XML and XSLT.
zOPER_EXPORT zSHORT OPERATION
FormatSubobjectForXSLT( zVIEW     vSourceOI,
                        zCPCHAR   cpcSubobjectEntityName,
                        zVIEW     vSubtask,
                        zCPCHAR   cpcReportDefName,
                        zLONG     lFlag )
{
   zVIEW  vReportDef;
// zVIEW  vAnimator;
// zVIEW  vTaskLPLR;
   zCHAR  szFileName[ zMAX_FILESPEC_LTH + 1 ];
   LPAPP  lpApp;
   zSHORT nRC;

   // Use the ReportDefName passed to Activate the Report Def and
   // pass it to the GenerateXSLT_PDF object operation.  This
   // global operation (FormatSubobjectOnDoc) is required as an
   // interface between the User and the FormatSubobjectForXSLT operation
   // so that the user does not have to Activate the Report Def.

   // We must get the directory that contains the ReportDef.
   szFileName[ 0 ] = 0;
   SfGetApplicationForSubtask( &lpApp, vSubtask );
   if ( lpApp )
      strcpy_s( szFileName, sizeof( szFileName ), lpApp->szObjectDir );
   else
      return -1;

   strcat_s( szFileName, sizeof( szFileName ), cpcReportDefName );
   strcat_s( szFileName, sizeof( szFileName ), ".XRP" );

   // DGC 7/11/96 -- The following is a hack for compatability with Unix.
   // Unix file names are case-sensitive.  Since DOS/Windows files (when
   // copied to a unix system via a network) are assigned lower-case letters
   // in Unix, we will (for now) make all Zeidon System file names lowercase.
   _strlwr_s( szFileName, sizeof( szFileName ) );

   nRC = ActivateOI_FromFile( &vReportDef, "TZRPSRCO", vSubtask, szFileName, zACTIVATE_SYSTEM );

   if ( nRC >= 0 )
   {
      SetNameForView( vReportDef, "ReportDefXSLT", vSubtask, zLEVEL_TASK );
      // KJS 02/15/12 - having problems because of cpc instead of pch which is what it is in zeidonop.h and tzrpsrco.c
      //nRC = oTZRPSRCO_GenerateXSLT_PDF( vReportDef,
      //                                  vSourceOI,
      //                                  cpcSubobjectEntityName,
      //                                  lFlag );

      TraceLineS( "Format Report ===========================> ", szFileName );
   }
   else
      TraceLineS( "Cannot load Report ======================> ", szFileName );

   return( nRC );
}

//************************************************************************************************//
//
//    OPERATION: ConvertXML_SpecialCharacters
//
//************************************************************************************************//
zSHORT /* GLOBAL */  OPERATION
ConvertXML_SpecialCharacters( zVIEW  vAnyView,
                              zPCHAR pchReturnedString,
                              zPCHAR pchSourceString,
                              zLONG  lMaxReturnedStringLength )
{
   zLONG  lMaxLength;
   zPCHAR SourcePtr;
   zPCHAR TargetPtr;
   zCHAR  szTrace[ 22 ];

   // Convert any special XML characters from the Source String to the Returned String.
   // Special character conversion is:
   // "   &quot;
   // '   &apos;
   // <   &lt;
   // >   &gt;
   // &   &amp;

   lMaxLength = lMaxReturnedStringLength - 6;    // Leave room for the max special character length.
   SourcePtr = pchSourceString;
   TargetPtr = pchReturnedString;
   while ( *SourcePtr != 0 && (TargetPtr - pchReturnedString) < lMaxLength )
   {
      if ( *SourcePtr == '"' )
      {
         strcpy_s( TargetPtr, lMaxReturnedStringLength - (TargetPtr - pchReturnedString),  "&quot;" );
         TargetPtr = TargetPtr + 6;
      }
      else
      if ( *SourcePtr == '\'' )
      {
         strcpy_s( TargetPtr, lMaxReturnedStringLength - (TargetPtr - pchReturnedString), "&apos;" );
         TargetPtr = TargetPtr + 6;
      }
      else
      if ( *SourcePtr == '<' )
      {
         strcpy_s( TargetPtr, lMaxReturnedStringLength - (TargetPtr - pchReturnedString), "&lt;" );
         TargetPtr = TargetPtr + 4;
      }
      else
      if ( *SourcePtr == '>' )
      {
         strcpy_s( TargetPtr, lMaxReturnedStringLength - (TargetPtr - pchReturnedString), "&gt;" );
         TargetPtr = TargetPtr + 4;
      }
      else
      if ( *SourcePtr == '&' )
      {
         strcpy_s( TargetPtr, lMaxReturnedStringLength - (TargetPtr - pchReturnedString), "&amp;" );
         TargetPtr = TargetPtr + 5;
      }
      else
      {
         *TargetPtr = *SourcePtr;
         TargetPtr++;
         strcpy_s( szTrace, sizeof( szTrace ), "*** Source Character: " );
         *(szTrace + 21) = *SourcePtr;
      }

      SourcePtr++;
   }
   *TargetPtr = 0;

   return( 0 );

} // ConvertXML_SpecialCharacters
