/////////////////////////////////////////////////////////////////////////////
// Project TzCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    tzctl.dll - ZDr Design Control implementations
// FILE:         tzctlcvt.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Painter Conversion routines (convert
// .PWD to .XWD).

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2002.02.14    DKS   WEB
//    Implemented ConvertToolBar.
//
// 2001.12.05    DKS   WEB
//    Modified ConvertListBox to set the DataType for each Column in the XRA.
//
// 2001.06.10    BL    WEB
//    Modified ConvertMLE for save MLE Type (Upper/Lower Case) in the XRA.
//
// 2001.05.31    BL    WEB
//    Modified ConvertRadioBtn for save Default Radio Button in XRA
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.02.20    BL    R51278
//    generate correct Bitmap RC File
//
// 2001.02.07    BL    TREE
//    Change Error Message by Mapping Errors for Tree and all Controls
//
// 2001.01.16    DKS   WEB
//    Added ConvertCalendar operation to convert the Calendar ctrol.
//
// 2001.01.16    DKS   Z2000
//    Determine when Tab control should be OWNERDRAW.
//
// 2000.11.27    DKS   Z2000
//    Workstation generation of .C files now reports if there is a mismatch
//    between a Dialog (.PWD) and its generated resource file.
//
// 2000.09.27    BL    Z10
//    Modified ConvertGrid: If Mapping Error send a Message Box with Window
//    and Control Tag
//
// 2000.09.08    BL    Z10
//    Modified ConvertListBox: set size of szTempString to 32256 (+ 32000 for
//    DIL Message).
//
// 2000.09.04    BL    Z10  TB 50634
//    Modified ConvertComboBox: if no mapping, send a error message
//
// 2000.08.23    BL    Z10
//    Bugfix mapping errors: if no mapping, reset Dialog View
//
// 2000.06.13    DKS   Z10
//    Button headers now use NLS when specified.
//
// 2000.05.05    TMV   WEB
//    Create Entities for RadioGroup & RadioButtons
//    when creating the XRA.
//
// 2000.05.04    RG    Z2000
//    Fixed function fnBuildRC_Filename: Set '\\' between Path and Filename
//    instead of '\'
//
// 2000.01.07    RG    Z2000
//    Fixed function: fnBuildRC_Filename
//    In case of use the ResourcePath-Entry in Zeidon.ini instead of the hardcoded
//    path in a dialog the bitmaps wouldn't entered correct in the *.RC-Files.
//    We know have different possibilities:
//    1. Enter a hardcoded path for bitmaps in a dialog
//    2. Enter the path in the ResourcePath - Entry in the Zeidon.Ini file
//
// 1998.11.20    DKS
//    Added sort flag to Button Header ListBox type.
//
// 1998.10.30    DKS
//    Added conversion code for Button Header ListBox type.
//

#include "zstdafx.h"

#define TZCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "TzCtl.h"
#include "TzCtlGbl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


const static zLONG CONTROL_TYPE_COMBOBOX = 32001;
const static zLONG CONTROL_TYPE_CHECKBOX = 32002;
const static zLONG CONTROL_TYPE_EDIT     = 32003;
const static zLONG CONTROL_TYPE_RADIO    = 32004;
const static zLONG CONTROL_TYPE_RADIOGRP = 32005;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// control conversion routines return:
//    0 - control conversion handled all sub-controls
//   -1 - conversion should handle sub-controls
//  -16 - control conversion encountered an error ... skip
//        to next control
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zSHORT
fnGetDIL_Msg( zVIEW vDialog, zPCHAR pchReturnDIL_Msg, zLONG lMaxLth )
{
   GetVariableFromAttribute( pchReturnDIL_Msg, 0, zTYPE_STRING, lMaxLth, vDialog, "Control", "DIL_Text", 0, 0 );
   if ( pchReturnDIL_Msg[ 0 ] )
   {
      // DIL_Text has been set.
   }
   else
   {
      if ( CheckExistenceOfEntity( vDialog, "CtrlMapER_Domain" ) >= zCURSOR_SET )
      {
         // DIL_Inherited.
         GetStringFromAttribute( pchReturnDIL_Msg, lMaxLth, vDialog, "Control", "DIL_Inherited" );
      }
      else
      {
         // Otherwise the value is null.
         pchReturnDIL_Msg[ 0 ] = 0;
      }
   }

   return( (zSHORT) zstrlen( pchReturnDIL_Msg ) );
}

void
fnDisplayCtrlError( zVIEW   vSubtask,
                    zVIEW   vSrcDialog,
                    zCPCHAR cpcMsg,
                    zBOOL   bDelete )
{
   zVIEW vWindow;
   zCHAR  szWork[ 256 ];
   zCHAR  szWork2[ 34 ];

   strcpy_s( szWork, zsizeof( szWork ), cpcMsg );
   GetStringFromAttribute( szWork2, zsizeof( szWork2 ), vSrcDialog, szlControl, szlTag );
   strcat_s( szWork, zsizeof( szWork ), szWork2 );
   strcat_s( szWork, zsizeof( szWork ), " ... on window: " );
   CreateViewFromViewForTask( &vWindow, vSrcDialog, vSubtask );
   while ( ResetViewFromSubobject( vWindow ) == 0 )
   {
   }

   GetStringFromAttribute( szWork2, zsizeof( szWork2 ), vWindow, szlWindow, szlTag );
   DropView( vWindow );
   strcat_s( szWork, zsizeof( szWork ), szWork2 );
   if ( bDelete )
      strcat_s( szWork, zsizeof( szWork ), "\nhas been deleted from execution." );
   else
      strcat_s( szWork, zsizeof( szWork ), "." );

   IssueError( vSubtask, 0, 0, szWork );
}

//./ ADD NAME=fnBuildRC_Filename
// Source Module=tzctlcvt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  TZ OPERATION: fnBuildRC_Filename
//
//  PURPOSE:    Build a filename for a BITMAP entry in an RC file out of
//              a BMP name with path.
//
//              The name is included in " ". All '\' changed to '\\'.
//
//              Three possibilities to get the path
//              1. The path is hardcoded in a dialog
//              2. The path is entered in Zeidon.ini
//
//              If no path is entered, neither in a dialog nor
//              in Zeidon.ini, the system will search the bitmap
//              in the actual directory. If it is not found the
//              linker will give an error message.
//
//  PARAMETERS: pchRCFilename - pointer to buffer for RC filename
//              cpcBMPname - BMP name with path
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT void  OPERATION
fnBuildRC_Filename( zVIEW   vSubtask,
                    zPCHAR  pchRCFileName, // target field
                    zCPCHAR cpcBMPName )   // filename (with or without path)
{
   zCHAR  szResourcePath[ 256 ];
   zPCHAR pchPos;
   zBOOL  bResourcePath = TRUE;

   if ( cpcBMPName )
   {
      pchPos = zstrrchr( cpcBMPName, '.' );
      if ( pchPos )
      {
         pchPos++;
         if ( zstricmp( pchPos, "jpg" ) == 0  ||
              zstricmp( pchPos, "jpeg" ) == 0 ||
              zstricmp( pchPos, "gif" ) == 0 ||
              zstricmp( pchPos, "png" ) == 0 )
         {
            TraceLine( "fnBuildRC Skipping Filename: %s  BMPName: %s", pchRCFileName, cpcBMPName );
            *pchRCFileName = 0;
            return;
         }
      }
   }

   if ( *cpcBMPName == '@' )  // indirection
   {
      *pchRCFileName = 0;
      return;
   }

   // Put RC filename in quotes.
   *pchRCFileName++ = '\"';

   pchPos = zstrchr( cpcBMPName, '\\' );
   if ( pchPos )
   {
      if ( SysOpenFile( vSubtask, cpcBMPName, COREFILE_EXIST ) != -1 )
      {
         bResourcePath = FALSE;

         // The path is hardcoded in a dialog ... replace '\' with '\\'
         while ( *cpcBMPName )
         {
            if ( *cpcBMPName == '\\' )
               *pchRCFileName++ = '\\';

            *pchRCFileName++ = *cpcBMPName++;
         }
      }
   }

   if ( bResourcePath )
   {
      zCHAR  szTempPath[ zsizeof( szResourcePath ) ];
      zSHORT nLth;

      // Read Zeidon.ini to see if the path is entered here.
      szTempPath[ 0 ] = 0;
      SysReadZeidonIni( -1, "[Workstation]", "ResourcePath", szTempPath, zsizeof( szTempPath ) );
      SysConvertEnvironmentString( szResourcePath, zsizeof( szResourcePath ), szTempPath );
      nLth = (zSHORT) zstrlen( szResourcePath );

      // Set '\' at the end of the path if it is not set.
      if ( nLth && szResourcePath[ nLth - 1 ] != '\\' )
      {
          szResourcePath[ nLth++ ] = '\\';
          szResourcePath[ nLth ] = 0;
      }

      // Put the path in the target field and replace '\' with '\\'.
      pchPos = szResourcePath;
      while ( *pchPos )
      {
         if ( *pchPos == '\\' )
            *pchRCFileName++ = '\\';

         *pchRCFileName++ = *pchPos++;
      }

      // Put the filename in the target field.
      if ( cpcBMPName[ 0 ] == '\\' )
          *cpcBMPName++;

      while ( *cpcBMPName )
      {
         if ( *cpcBMPName == '\\' )
             *pchRCFileName++ = '\\';

         *pchRCFileName++ = *cpcBMPName++;
      }
   }

   *pchRCFileName++ = '\"';
   *pchRCFileName = 0;

} // fnBuildRC_Filename

// Change .bmp to whatever is specified in the Zeidon.ini file.
// Remove Drive specification from the resource path name.
void
fnResetResourceImage( zVIEW   vSubtask,
                      zVIEW   vTgtRemote,
                      zCPCHAR cpcEntityName,
                      zCPCHAR cpcAttributeName,
                      zCPCHAR cpcName,
                      zLONG   lIdNbr )
{
   if ( lIdNbr == 0 || cpcName[ 0 ] )
   {
      zVIEW vApp;
      zCHAR  szImage[ zMAX_FILENAME_LTH ];

      CreateViewFromViewForTask( &vApp, vTgtRemote, vSubtask );
      while ( ResetViewFromSubobject( vApp ) == 0 )
      {
      }

      zSHORT nLth = (zSHORT) zstrlen( cpcName ) + zMAX_FILENAME_LTH + 1;
      zPCHAR pchName = new char[ nLth ];

      strcpy_s( pchName, nLth, cpcName );
      GetStringFromAttribute( szImage, zsizeof( szImage ), vApp, "App", "Image" );
      if ( szImage[ 0 ] )
      {
         zPCHAR pchBMP;
         zPCHAR pch = new char[ nLth ];

         strcpy_s( pch, nLth, pchName );
         SysTranslateString( pch, 'L' );
         if ( (pchBMP = zstrstr( pch, ".bmp" )) != 0 )
         {
            strcpy_s( pchName + (pchBMP - pch + 1), nLth - (pchBMP - pch + 1), szImage );
            strcat_s( pchName, nLth, pchBMP + 4 );
         }

         delete [] pch;
      }

      GetStringFromAttribute( szImage, zsizeof( szImage ), vApp, "App", "ImageURL" );
      if ( pchName[ 0 ] )
      {
         if ( szImage[ 0 ] )
         {
            zPCHAR pch = new char[ nLth ];

            strcpy_s( pch, nLth, szImage );
            SysConvertEnvironmentString( szImage, zsizeof( szImage ), pch );
            SysTranslateString( szImage, 'L' );

            strcpy_s( pch, nLth, pchName );
            SysTranslateString( pch, 'L' );

            nLth = (zSHORT) zstrlen( szImage );
            if ( zstrncmp( pch, szImage, nLth ) == 0 )
               strcpy_s( pchName, nLth, pchName + nLth );

            zPCHAR pchColon = zstrchr( pchName, ':' );
            if ( pchColon )
               strcpy_s( pchName, nLth, pchColon + 1 );

            delete [] pch;
         }
      }

      DropView( vApp );
      SetAttributeFromString( vTgtRemote, cpcEntityName, cpcAttributeName, pchName );
      delete [] pchName;
   }
   else
   {
      SetAttributeFromInteger( vTgtRemote, cpcEntityName, cpcAttributeName, lIdNbr );
   }
}

zSHORT
StringLthOn4_ByteBoundary( zPCHAR pchName )
{
   zSHORT k = (zSHORT) zstrlen( pchName );
   zSHORT j = (k + 1) % sizeof( zLONG );
   if ( j )
   {
      k += sizeof( zLONG ) - j;
   }

   return( k );
}

//./ ADD NAME=ConvertButton
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertButton
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertButton( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT nCtrlID )
{
   zPCHAR pchDfltBtn;
   zPCHAR pchTag;

   CreateEntity( vTgtRemote, "Button", zPOS_AFTER );
   CreateViewFromViewForTask( &vTgtDialog, vTgtDialog, vSubtask );
   while ( ResetViewFromSubobject( vTgtDialog ) == 0 )
   {
   }

   GetAddrForAttribute( (zPPCHAR) &pchDfltBtn, vTgtDialog, "Wnd", "DfltBtn" );
   GetAddrForAttribute( (zPPCHAR) &pchTag, vTgtRemote, "Ctrl", "Tag" );
   if ( zstrcmp( pchDfltBtn, pchTag ) == 0 )
      SetAttributeFromString( vTgtRemote, "Button", "DfltBtn", "Y" );

   DropView( vTgtDialog );
   return( 0 );
}

//./ ADD NAME=ConvertText
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertText
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertText( zVIEW   vSubtask,
             zVIEW   vTgtDialog,
             zVIEW   vTgtRemote,
             zVIEW   vSrcDialog,
             zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "Label", zPOS_AFTER );

   LABEL_DATA ld;
   zLONG   lColor;
   zLONG   lSize;
   zULONG  ulLth = sizeof( ld );

   GetBlobFromAttribute( &ld, &ulLth, vSrcDialog, "Control", "CtrlBOI" );
   if ( ulLth >= 4 + 2 * sizeof( zLONG ) )
   {
      if ( ld.chBold )
         SetAttributeFromString( vTgtRemote, "Label", "Bold", "Y" );

      if ( ld.chItalic )
         SetAttributeFromString( vTgtRemote, "Label", "Italic", "Y" );

      if ( ld.chStrikeout )
         SetAttributeFromString( vTgtRemote, "Label", "Strikeout", "Y" );

      if ( ld.chUnderline )
         SetAttributeFromString( vTgtRemote, "Label", "Underscore", "Y" );

      lSize = ld.lPointSize;
      if ( lSize )
         SetAttributeFromInteger( vTgtRemote, "Label", "FontSize", lSize );

      lColor = ld.lTextColor;
      if ( lColor )
         SetAttributeFromInteger( vTgtRemote, "Label", "Color", lColor );

      if ( ulLth > 5 + (2 * sizeof( zLONG )) )
      {
      // if ( ld.chTopLeft & 1 )
      //    SetAttributeFromString( vTgtRemote, "Label", "Justify", "Left" );

         if ( ld.chCenter & 1 )
            SetAttributeFromString( vTgtRemote, "Label", "Justify", "Center" );

         if ( ld.chBottomRight & 1 )
            SetAttributeFromString( vTgtRemote, "Label", "Justify", "Right" );

         if ( ulLth > 8 + (2 * sizeof( zLONG )) )
         {
            if ( ld.szFontName[ 0 ] )
               SetAttributeFromString( vTgtRemote, "Label", "FaceName", ld.szFontName );

            if ( ld.lEscapement )
               SetAttributeFromInteger( vTgtRemote, "Label", "Escapement", ld.lEscapement );
         }
      }
   }

   return( 0 );
}

//./ ADD NAME=ConvertEditBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertEditBox
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertEditBox( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT nCtrlID )
{
   zLONG lSubtype;

   CreateEntity( vTgtRemote, "EditBox", zPOS_AFTER );
   GetIntegerFromAttribute( &lSubtype, vSrcDialog, "Control", "Subtype" );

   if ( lSubtype & zEDITBOX_LOWERCASE )
      SetAttributeFromString( vTgtRemote, "EditBox", "Case", "L" );
   else
   if ( lSubtype & zEDITBOX_UPPERCASE )
      SetAttributeFromString( vTgtRemote, "EditBox", "Case", "U" );
   else
      SetAttributeFromString( vTgtRemote, "EditBox", "Case", "N" );

   if ( lSubtype & zEDITBOX_PASSWORD )
      SetAttributeFromString( vTgtRemote, "EditBox", "Password", "Y" );
   else
      SetAttributeFromString( vTgtRemote, "EditBox", "Password", "N" );

   return( 0 );
}

//./ ADD NAME=ConvertMLE
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertMLE
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertMLE( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT nCtrlID )
{
   zLONG lSubtype;

   CreateEntity( vTgtRemote, "MLE", zPOS_AFTER );
   GetIntegerFromAttribute( &lSubtype, vSrcDialog, "Control", "Subtype" );

   if ( lSubtype & zEDITBOX_LOWERCASE )
      SetAttributeFromString( vTgtRemote, "MLE", "Case", "L" );
   else
   if ( lSubtype & zEDITBOX_UPPERCASE )
      SetAttributeFromString( vTgtRemote, "MLE", "Case", "U" );
   else
      SetAttributeFromString( vTgtRemote, "MLE", "Case", "N" );

   return( 0 );
}

//./ ADD NAME=ConvertRTE
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertRTE
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertRTE( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "EditBox", zPOS_AFTER );
   SetAttributeFromString( vTgtRemote, "EditBox", "EditStyle", "R" );
   return( 0 );
}

//./ ADD NAME=ConvertCheckBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertCheckBox
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertCheckBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "CheckBox", zPOS_AFTER );
   return( 0 );
}

//./ ADD NAME=ConvertTab
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertTab
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertTab( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT nCtrlID )
{
   zCHAR  szBlob[ 5 * sizeof( zLONG ) ];
   zULONG ulLth = zsizeof( szBlob );

   CreateEntity( vTgtRemote, "NoteBook", zPOS_AFTER );
   OrderEntityForView( vSrcDialog, szlCtrlCtrl, "Type A" );

   *((zPLONG) szBlob) = 0;
   *((zPLONG) (szBlob + sizeof( zLONG ))) = 0;
   *((zPLONG) (szBlob + 2 * sizeof( zLONG ))) = 0; // dflt tabs per row
   *((zPLONG) (szBlob + 3 * sizeof( zLONG ))) = 0; // text color
   *((zPLONG) (szBlob + 4 * sizeof( zLONG ))) = 0; // tab color
   GetBlobFromAttribute( szBlob, &ulLth, vSrcDialog, szlControl, szlCtrlBOI );

   zLONG lTabStyle = *((zPLONG) (szBlob + sizeof( zLONG ) * 2));
   if ( *((zPLONG) (szBlob + 3 * sizeof( zLONG ))) ||    // text color
        *((zPLONG) (szBlob + 4 * sizeof( zLONG ))) )     // tab color
   {
      lTabStyle |= 0x00000001;
   }

   // Count the tabs ... ending up on the first one.  Determine if
   // the tab control should be ownerdraw.
   zSHORT k = 0;
   zSHORT nRC = SetCursorLastEntity( vSrcDialog, szlCtrlCtrl, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( (lTabStyle & 0x00000001) == 0 )
      {
         ulLth = 2 * sizeof( zLONG );
         GetBlobFromAttribute( szBlob, &ulLth, vSrcDialog, szlCtrlCtrl, szlCtrlBOI );
         if ( ulLth >= 2 * sizeof( zLONG ) &&
              (*((zPLONG) (szBlob + 0 * sizeof( zLONG ))) ||   // text color
               *((zPLONG) (szBlob + 1 * sizeof( zLONG )))) )   // tab color
         {
            lTabStyle |= 0x00000001;
         }
      }

      k++;
      nRC = SetCursorPrevEntity( vSrcDialog, szlCtrlCtrl, 0 );
   }

   *((zPLONG) (szBlob + sizeof( zLONG ))) = k;   // set number of tabs
   *((zPLONG) (szBlob + 2 * sizeof( zLONG ))) = lTabStyle;

   ulLth = zsizeof( szBlob );
   SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, szBlob, ulLth );

   if ( k > 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

//./ ADD NAME=ConvertTabCtl
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertTabCtl
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertTabCtl( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT nCtrlID )
{
   zVIEW  vDlg;
   zLONG  lSizeX;
   zLONG  lSizeY;

   CreateEntity( vTgtRemote, "NotePage", zPOS_AFTER );

   // Determine the sort order number for this page.
   zSHORT k = 1;

   CreateViewFromViewForTask( &vDlg, vTgtRemote, vSubtask );
   while ( SetCursorPrevEntity( vDlg, szlCtrl, 0 ) > zCURSOR_UNCHANGED )
      k++;

   DropView( vDlg );

   SetAttributeFromInteger( vTgtRemote, "NotePage", "Order", k );

   CreateViewFromViewForTask( &vDlg, vSrcDialog, vSubtask );
   ResetViewFromSubobject( vDlg );
   GetIntegerFromAttribute( &lSizeX, vDlg, szlControl, szlSZDLG_X );
   GetIntegerFromAttribute( &lSizeY, vDlg, szlControl, szlSZDLG_Y );

   DropView( vDlg );

   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlPSDLG_X, 3 );
   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlPSDLG_Y, 14 );
   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlSZDLG_X, lSizeX );
   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlSZDLG_Y, lSizeY );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) >= 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

typedef zVIEW (POPERATION zFARPROC_SETUPMAPLOD)( zVIEW, zVIEW, zVIEW, zPCHAR );
typedef zSHORT (POPERATION zFARPROC_DROPMETAOI)( zVIEW, zVIEW );

/////////////////////////////////////////////////////////////////////////////
//
// This function dynamically calls a CM function to retrieve the LOD
// associated with a Ctrl's mapping.
//
/////////////////////////////////////////////////////////////////////////////
zVIEW
fnSetUpMapLOD( zVIEW vSubtask, zVIEW vSrcDialog )
{
#ifdef DEBUG_ALL
   TraceLineS( "fnSetUpMapLOD", "" );
#endif

   zFARPROC_SETUPMAPLOD lpfnDynRoutine;
   LPLIBRARY lpLibrary = 0;
   zVIEW     vLOD = 0;

   lpfnDynRoutine = (zFARPROC_SETUPMAPLOD) GetOperationDynamicCallAddress( vSubtask, &lpLibrary,
                                                                           "tzlodopr", "SetUpMapLOD",
                                                                           "(tzctlcvt ListBox)" );
   if ( lpfnDynRoutine )
   {
      zVIEW  vWindow;

      CreateViewFromViewForTask( &vWindow, vSrcDialog, vSubtask );
      while ( ResetViewFromSubobject( vWindow ) == 0 )
      {
      }

      vLOD = (*lpfnDynRoutine)( vSubtask, vWindow, vSrcDialog, "CtrlMapView" );
      DropView( vWindow );
   }

   return( vLOD );
}

/////////////////////////////////////////////////////////////////////////////
//
// This function dynamically calls a CM function to retrieve the LOD
// associated with a Ctrl's mapping.
//
/////////////////////////////////////////////////////////////////////////////
void
fnDropMetaOI( zVIEW vSubtask, zVIEW vLOD )
{
#ifdef DEBUG_ALL
   TraceLineS( "fnDropMetaOI", "" );
#endif

   zFARPROC_DROPMETAOI lpfnDynRoutine;
   LPLIBRARY lpLibrary = 0;

   lpfnDynRoutine = (zFARPROC_DROPMETAOI) GetOperationDynamicCallAddress( vSubtask, &lpLibrary,
                                                                          "tzlodopr", "DropMetaOI",
                                                                          "(tzctlcvt ListBox)" );
   if ( lpfnDynRoutine )
   {
      (*lpfnDynRoutine)( vSubtask, vLOD );
   }
}

#define zLISTBOX_SORTEDHEADERS        zCONTROL_SPECIFIC_2
#define zLISTBOX_COLUMNHEADERS        zCONTROL_SPECIFIC_3
#define zLISTBOX_SCOPE_PARENT         zCONTROLX_SPECIFIC_1

//./ ADD NAME=ConvertListBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertListBox
//
//  PURPOSE:
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertListBox( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT nCtrlID )
{
   zSHORT nRC;

   CreateEntity( vTgtRemote, "ListBox", zPOS_AFTER );
   SetAttributeFromAttribute( vSrcDialog, szlControl, szlText, vSrcDialog, szlControl, szlTag );
   SetAttributeFromAttribute( vTgtDialog, szlCtrl, szlText, vTgtDialog, szlCtrl, szlTag );

   // Note that the composite Control entity has already been processed using the SetMatchingAttributesByName operation.
   nRC = CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      zVIEW  vLOD;
      zCHAR  szTempString[ 32256 ];  // + 32000 for DIL Message
      zPCHAR pchBuffer;
      zPCHAR pchEntityName;
      zPCHAR pchSubCtrlName;
      zPCHAR pchCtrlTag;
      zULONG ulLth = 0;
      zLONG  lCharLth;
      zLONG  lSubtype;
      zLONG  lExtent;
      zBOOL  bButtonHeaders;
      zBOOL  bPropertyList = FALSE;
      zSHORT nDIL_Lth;
      zLONG  k;

      GetIntegerFromAttribute( &lSubtype, vSrcDialog, szlControl, szlSubtype );
      if ( (lSubtype & zLISTBOX_COLUMNHEADERS) || (lSubtype & zLISTBOX_SORTEDHEADERS) )
      {
         bButtonHeaders = TRUE;
      }
      else
      {
         bButtonHeaders = FALSE;
      }

      // Note that we are stealing the attribute name on the composite control to store the scoping entity name!!!
      if ( SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
         {
         // DisplayEntityInstance( vSrcDialog, szlCtrlMapLOD_Entity );
            SetAttributeFromAttribute( vTgtDialog, szlCtrlMap, "AN", vSrcDialog, szlCtrlMapLOD_Entity, szlName );
         }

         if ( SetCursorNextEntity( vTgtDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
            DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_BEFORE );
      }

      // Get the view name from the last CtrlMap for the selected list view
      if ( SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapView ) == 0 )
         {
            if ( SetCursorNextEntity( vTgtDialog, szlCtrlMap, 0 ) <= zCURSOR_UNCHANGED )
            {
               CreateEntity( vTgtDialog, szlCtrlMap, zPOS_AFTER );
            }

            SetAttributeFromAttribute( vTgtDialog, szlCtrlMap, "VN", vSrcDialog, szlCtrlMapView, szlName );
         }
         else
         {
            SetCursorNextEntity( vTgtDialog, szlCtrlMap, 0 );
            DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_BEFORE );
         }
      }

      // Reposition on the first CtrlMap.
      SetCursorFirstEntity( vSrcDialog, szlCtrlMap, 0 );

      // ViewName and EntityName must be set on the composite control for
      // a ListBox ... enforced by the ListBox maintenance dialog???
      if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
      {
         GetAddrForAttribute( (zCOREMEM) &pchEntityName, vSrcDialog, szlCtrlMapLOD_Entity, szlName );
      }
      else
      {
         zCHAR  szTag[ 34 ];

         GetStringFromAttribute( szTag, zsizeof( szTag ), vSrcDialog, szlControl, szlTag );
         if ( szTag[ 0 ] == '_' && szTag[ 1 ] == '_' &&
              szTag[ 2 ] == 'P' && szTag[ 3 ] == 'r' &&
              szTag[ 4 ] == 'o' && szTag[ 5 ] == 'p' )
         {
            bPropertyList = TRUE;
            pchEntityName = "";
         }
         else
         {
            fnDisplayCtrlError( vSubtask, vSrcDialog,
                                "ListBox MAPPING error ... Control has no mapping entity.\nListBox: ", TRUE );
            return( -16 );
         }
      }

      if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
      {
         IssueError( vSubtask, 0, 0, "Memory alloc error - convert ListBox" );
         DrFreeTaskMemory( pchBuffer );
         return( -16 );
      }

      // Handle the sub-controls within the listbox ... setting up the mapping information in a blob.
      pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
      OrderEntityForView( vSrcDialog, szlCtrlCtrl, "PSDLG_X A" );
      SetViewToSubobject( vSrcDialog, szlCtrlCtrl );
      SetCursorFirstEntity( vSrcDialog, szlControl, 0 );

      // Set up tab stops for ListBox processing.
      if ( bButtonHeaders )
      {
         ulLth = 3 * sizeof( zLONG );

         // Get the subtype of the first column to determine if it is editable or not.
         GetIntegerFromAttribute( &lSubtype, vSrcDialog, szlControl, szlSubtype );
      }
      else
      {
         ulLth = 2 * sizeof( zLONG );

         // Ensure that the first column flag will not be set.
         lSubtype = zCONTROL_DISABLED;
      }

      k = 0;
      while ( nRC > zCURSOR_UNCHANGED )
      {
         // Force the first tab stop to zero (also use this as a flag
         // to specify whether the first column in a list ctrl is disabled).
         if ( k == 0 )
         {
            if ( lSubtype & zCONTROL_DISABLED )
               *((zPLONG) (pchBuffer + ulLth)) = 0;
            else
               *((zPLONG) (pchBuffer + ulLth)) = -1;
         }
         else
         {
            // Save the length of this attribute.
            GetIntegerFromAttribute( &lCharLth, vSrcDialog, szlControl, szlPSDLG_X );
            *((zPLONG) (pchBuffer + ulLth)) = lCharLth;
         }

         k++;
         ulLth += sizeof( zLONG );
         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
      }

      *((zPLONG) (pchBuffer + sizeof( zLONG ))) = k; // set # of tabstops

      if ( bButtonHeaders )
      {
         zVIEW vTgtWindow;
         zLONG lNLS;

         CreateViewFromViewForTask( &vTgtWindow, vTgtDialog, vSubtask );
         while ( ResetViewFromSubobject( vTgtWindow ) == 0 )
         {
         }

         nRC = SetCursorFirstEntity( vSrcDialog, szlControl, 0 );
         while ( nRC > zCURSOR_UNCHANGED )
         {
            // Save the tag attribute for the ListCtrl.
            GetAddrForAttribute( (zCOREMEM) &pchCtrlTag, vSrcDialog, szlControl, szlTag );
            k = StringLthOn4_ByteBoundary( pchCtrlTag );
            *((zPLONG) (pchBuffer + ulLth)) = k;  // set the tag length
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchCtrlTag );
            ulLth += k + 1;

            // Save the text attribute for the ListCtrl.
            GetAddrForAttribute( (zCOREMEM) &pchCtrlTag, vSrcDialog, szlControl, szlText );
            k = zstrlen( pchCtrlTag );
            szTempString[ 0 ] = '\t';
            nDIL_Lth = fnGetDIL_Msg( vSrcDialog, szTempString + 1, zsizeof( szTempString ) - 1 );
            szTempString[ nDIL_Lth + 1 ] = '\t';
            szTempString[ nDIL_Lth + 2 ] = 0;
            nDIL_Lth = (zSHORT) zstrlen( szTempString ) + 1;
            k += nDIL_Lth + sizeof( zLONG );

            // Process NLS entries for Control.Text.
            // Create an NLS entity if there are NLS entries for Text, along
            // with the subordinate NLS_Text entries.
            lNLS = -1;
            nRC = SetCursorFirstEntity( vSrcDialog, "ControlNLS_Text", "" );
            if ( nRC >= zCURSOR_SET )
            {
               SetCursorLastEntity( vTgtWindow, "NLS", "" );
               CreateEntity( vTgtWindow, "NLS", zPOS_AFTER );
               lNLS = GetRelativeEntityNumber( vTgtWindow, "NLS", 0, 0L );

               while ( nRC >= zCURSOR_SET )
               {
                  CreateEntity( vTgtWindow, "NLS_Text", zPOS_AFTER );
                  SetAttributeFromAttribute( vTgtWindow, "NLS_Text", "LI", vSrcDialog, "ControlNLS_Text", "LanguageIndex" );
                  SetAttributeFromAttribute( vTgtWindow, "NLS_Text", "Text", vSrcDialog, "ControlNLS_Text", "Text" );
                  nRC = SetCursorNextEntity( vSrcDialog, "ControlNLS_Text", "" );
               }
            }

            // Set the Text length + DIL Msg length + NLS index
            *((zPLONG) (pchBuffer + ulLth)) = k;
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchCtrlTag );
            if ( nDIL_Lth )
               strcat_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, szTempString );

            *((zPLONG) (pchBuffer + ulLth + k - sizeof( zLONG ))) = lNLS;
            ulLth += k + 1;

            nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
         }

         // Offset to beginning of Mapping information.
         *((zPLONG) (pchBuffer + 2 * sizeof( zLONG ))) = ulLth;

         DropView( vTgtWindow );
      }

      // Set up mapping information for each sub-control in the ListBox.
      zLONG  lColumnCount = 0;
      zSHORT nLOD_RC;

      lExtent = 0;
      nRC = SetCursorFirstEntity( vSrcDialog, szlControl, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( ulLth > zMAX_BLOB - 256 )
         {
            pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
            DrFreeTaskMemory( pchBuffer );
            ResetViewFromSubobject( vSrcDialog );
            IssueError( vSubtask, 0, 0, "Memory error - cannot convert ListBox" );
            return( -16 );
         }

         nLOD_RC = zCURSOR_SET - 1;
         vLOD = 0;
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapRelatedEntity ) == 0 )
         {
            vLOD = fnSetUpMapLOD( vSubtask, vSrcDialog );
            GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapRelatedEntity, szlName );
            if ( vLOD )
            {
               nLOD_RC = SetCursorFirstEntityByString( vLOD, "LOD_Entity", "Name", pchSubCtrlName, 0 );
            }
         }
         else
         {
            if ( bPropertyList )
            {
               pchSubCtrlName = "";
            }
            else
            {
               ResetViewFromSubobject( vSrcDialog );
               fnDisplayCtrlError( vSubtask, vSrcDialog,
                                   "ListBox MAPPING error ... ListCtrl has no mapping entity.\nListBox - ",
                                   TRUE );
               return( -16 );
            }
         }

         CreateEntity( vTgtRemote, "ListBoxColumn", zPOS_AFTER );
         GetAddrForAttribute( (zCOREMEM) &pchCtrlTag, vSrcDialog, szlControl, szlText );
         SetAttributeFromString( vTgtRemote, "ListBoxColumn", "Text", pchCtrlTag );
         SetAttributeFromInteger( vTgtRemote, "ListBoxColumn", "Tag", lColumnCount );

         // Save the length of this attribute.
         GetIntegerFromAttribute( &lCharLth, vSrcDialog, szlControl, szlPSDLG_X );

         k = lCharLth;
         GetIntegerFromAttribute( &lCharLth, vSrcDialog, szlControl, szlSZDLG_X );
         SetAttributeFromInteger( vTgtRemote, "ListBoxColumn", "ColumnWidth", lCharLth );

         // Set default alignment.
         SetAttributeFromString( vTgtRemote, "ListBoxColumn", "Alignment", "L" );

         k += lCharLth;
         if ( lExtent < k )
            lExtent = k;

         GetIntegerFromAttribute( &lSubtype, vSrcDialog, szlControl, szlSubtype );
         if ( lSubtype & zCONTROL_RIGHTJUSTIFY )
         {
            SetAttributeFromString( vTgtRemote, "ListBoxColumn", "Alignment", "R" );
            lCharLth = -lCharLth;  // flag for right justification
         }

         if ( lSubtype & zCONTROL_CENTERJUSTIFY )
         {
            SetAttributeFromString( vTgtRemote, "ListBoxColumn", "Alignment", "C" );
         }

         *((zPLONG) (pchBuffer + ulLth)) = lCharLth; // dialog units
         ulLth += sizeof( zLONG );

         // If the same entity ... omit from list ... flagged by 0 byte.
         if ( zstrcmp( pchEntityName, pchSubCtrlName ) == 0 )
         {
            // Set the length of the entity.
            *((zPLONG) (pchBuffer + ulLth)) = (zLONG) 0;
            ulLth += sizeof( zLONG );
         }
         else
         {
            // Set the length of the entity.
            k = StringLthOn4_ByteBoundary( pchSubCtrlName );
            *((zPLONG) (pchBuffer + ulLth)) = k;
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
            ulLth += k + 1;
         }

         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapER_Attribute ) == 0 )
         {
            GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapER_Attribute, szlName );
            if ( vLOD && nLOD_RC >= zCURSOR_SET )
            {
               if ( CheckExistenceOfEntity( vLOD, "LOD_Attribute" ) == 0 &&
                    CheckExistenceOfEntity( vLOD, "ER_Attribute" ) == 0 &&
                    SetCursorFirstEntityByString( vLOD, "ER_Attribute", "Name", pchSubCtrlName, "LOD_Entity" ) >= zCURSOR_SET )
               {
                  if ( CheckExistenceOfEntity( vLOD, "Domain" ) == 0 )
                  {
                     zCHAR szDataType[ 2 ];

                     GetStringFromAttribute( szDataType, zsizeof( szDataType ), vLOD, "Domain", "DataType" );
                     if ( szDataType[ 0 ] && szDataType[ 0 ] != 'S' )
                        SetAttributeFromString( vTgtRemote, "ListBoxColumn", "DataType", szDataType );
                  }
               }
            }

            if ( MiValidView( vLOD ) )
               fnDropMetaOI( vSubtask, vLOD );
         }
         else
         {
            if ( bPropertyList )
            {
               pchSubCtrlName = "";
            }
            else
            {
               if ( vLOD )
                  fnDropMetaOI( vSubtask, vLOD );

               fnDisplayCtrlError( vSubtask, vSrcDialog,
                                   "ListBox MAPPING error ... ListCtrl has no mapping attribute.\nListBox - ",
                                   TRUE );
               ResetViewFromSubobject( vSrcDialog );
               return( -16 );
            }
         }

         // Set the length of the attribute.
         k = StringLthOn4_ByteBoundary( pchSubCtrlName );
         *((zPLONG) (pchBuffer + ulLth)) = k;
         ulLth += sizeof( zLONG );
         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
         ulLth += k + 1;

         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapContext ) == 0 )
         {
            GetStringFromAttribute( pchBuffer + ulLth + sizeof( zLONG ), zMAX_BLOB - ulLth - sizeof( zLONG ), vSrcDialog, szlCtrlMapContext, szlName );
         // GetContextFromSrcCtrl( vSrcDialog, pchBuffer + ulLth + sizeof( zLONG ) );

            // Set the length of the attribute.
            k = StringLthOn4_ByteBoundary( pchBuffer + ulLth + sizeof( zLONG ) );
            *((zPLONG) (pchBuffer + ulLth)) = k;
            ulLth += sizeof( zLONG ) + k + 1;
         }
         else
         {
            // Set the length of the entity.
            *((zPLONG) (pchBuffer + ulLth)) = 0L;
            ulLth += sizeof( zLONG );
         }

         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
         lColumnCount++;
      }

      *((zPLONG) pchBuffer) = lExtent;  // set the extent

      // Set the structure of attributes for this ListBox.
      ResetViewFromSubobject( vSrcDialog );
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, ulLth );
      pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
      DrFreeTaskMemory( pchBuffer );

      return( 0 );
   }
   else
   {
      ResetViewFromSubobject( vSrcDialog );
      fnDisplayCtrlError( vSubtask, vSrcDialog, "ListBox error ... ListBox has no ListCtrls.\nListBox - ", TRUE );
      return( -16 );
   }
}

#define zCOMBOBOX_DROPDOWN_LIST       0x00000000L // default
#define zCOMBOBOX_DROPDOWN            0x00000001L // 1
#define zCOMBOBOX_SIMPLE              0x00000002L // 2
#define zCOMBOBOX_SORT                0x00000010L // 16
#define zCOMBOBOX_DOMAIN_LIST         0x00000000L // default
#define zCOMBOBOX_POSITION_ONLY       0x00001000L // 4096
#define zCOMBOBOX_SELECT              0x00002000L // 8192  //LIST_ENTITY
#define zCOMBOBOX_INCLUDE             0x00004000L // 16384 //LIST_ENTITY
#define zCOMBOBOX_FOREIGN_KEY         0x00008000L // 32768 //LIST_ENTITY

#define zCOMBOBOX_MULTICOL    zCONTROL_SPECIFIC_1

//./ ADD NAME=ConvertComboBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     ConvertComboBox
//
//  PURPOSE:   To convert the combo box CtrlMap entries to a CtrlBOI.
//             There are four types of combo boxes (from a Zeidon point
//             of view):
//
//                1. Domain (table)
//                2. Entity list
//                3. Include (also lists an entity)
//
//             The Domain type requires (unless user list)
//                - an attribute specification which defines the source of
//                  the data listed and the target of the data value for
//                  the list entry selected.  It also requires a Domain or
//                  context table, which defines the source of the data
//                  listed.
//
//             The Entity list type requires
//                - an attribute specification which defines the source of
//                  the data displayed and the target of the selected data
//                - an attribute specification for the source data listed
//                - an entity specification for the scope of the source list
//                - an entity specification for the data listed (the source
//                  mapping attribute may be on a different entity than the
//                  entity traversed for the list).
//
//             The Include type requires
//                - an attribute specification which defines the source of
//                  the data displayed and the target entity of the include
//                - an attribute specification for the source data listed
//                - an entity specification for the scope of the source list
//                - an entity specification for the data listed (the source
//                  mapping attribute may be on a different entity than the
//                  entity traversed for the list).
//
//             The entities from the dialog are in the following order:
//                1. Target attribute
//                2. Source attribute
//                3. Source list control entity
//                4. Source scope entity
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertComboBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   zPCHAR  pchSubCtrlName;
   zPCHAR  pchBuffer;
   zULONG  ulSubtype;
   zSHORT  nRC;
   zLONG   k = 0;
   zLONG   lMappingError = 0;

   CreateEntity( vTgtRemote, "ComboBox", zPOS_AFTER );
   GetAddrForAttribute( &pchBuffer, vTgtDialog, szlCtrl, szlTag );

   // Note that the Control entity has already been processed using the
   // SetMatchingAttributesByName operation.
   GetIntegerFromAttribute( (zPLONG) &ulSubtype, vTgtDialog, szlCtrl, szlSubtype );

   // All the CtrlMap information gets put into a BOI except if the
   // combo box is going to display only selected entities.  Then,
   // one CtrlMap exists but will be created later so delete all the
   // target controls CtrlMaps.
   nRC = SetCursorFirstEntity( vTgtDialog, szlCtrlMap, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
      nRC = DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_AFTER );

   // There must be five CtrlMap entities ... enforced by the painter.
   if ( (nRC = SetCursorFirstEntity( vSrcDialog, szlCtrlMap, 0 )) > zCURSOR_UNCHANGED )
   {
      if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
      {
         IssueError( vSubtask, 0, 0, "Memory alloc error - convert ComboBox" );
         return( 1 );
      }

      pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
      zULONG ulLth = 0;
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( ulLth > zMAX_BLOB - 200 )
         {
            IssueError( vSubtask, 0, 0, "Cannot convert ComboBox" );
            pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
            DrFreeTaskMemory( pchBuffer );
            return( 1 );
         }

         k++;
         if ( k < 2 ||
              (ulSubtype & zCOMBOBOX_POSITION_ONLY) ||
              (ulSubtype & zCOMBOBOX_SELECT) ||
              (ulSubtype & zCOMBOBOX_INCLUDE) )
         {
            if ( k < 3 )
            {
               if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapView ) == zCURSOR_SET )
               {
                  // Put the view name into the CtrlBOI.
                  GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapView, szlName );
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
                  ulLth += zstrlen( pchSubCtrlName ) + 1;

                  // Put the entity name into the CtrlBOI.
                  GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapRelatedEntity, szlName );
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
                  ulLth += zstrlen( pchSubCtrlName ) + 1;

                  // Put the attribute name into the CtrlBOI.
                  GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapER_Attribute, szlName );
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
                  ulLth += zstrlen( pchSubCtrlName ) + 1;

                  // If there is a context, put it into the CtrlBOI.
                  if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapContext ) == 0 )
                  {
                     GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlCtrlMapContext, szlName );
                     ulLth += zstrlen( pchBuffer + ulLth ) + 1;
                  }
                  else
                  {
                     // Set the length of the entity.
                     *((zPSHORT) (pchBuffer + ulLth)) = (zSHORT) 0;
                     ulLth += sizeof( zSHORT );
                  }
               }
               else
               {
                  // No Mapping
                  lMappingError += k;

                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, "" );
                  ulLth++;
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, "" );
                  ulLth++;
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, "" );
                  ulLth++;
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, "" );
                  ulLth++;
               }
            }
            else
            {
               if ( CheckExistenceOfEntity( vSrcDialog,
                                            szlCtrlMapLOD_Entity ) == 0 )
               {
                  GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapLOD_Entity, szlName );
                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
                  ulLth += zstrlen( pchSubCtrlName ) + 1;
               }
               else
               if ( k == 3 )
               {
                  SetCursorPrevEntity( vSrcDialog, szlCtrlMap, 0 );
                  if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
                  {
                     GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlCtrlMapRelatedEntity, szlName );
                  }
                  else
                     pchSubCtrlName = "";

                  strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchSubCtrlName );
                  ulLth += zstrlen( pchSubCtrlName ) + 1;
                  SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 );
               }
               else
               {
                  // If the last CtrlMap has a view name then this combo box
                  // is only going to display selected values.  Keep this
                  // CtrlMap in the Target, but all the rest will be deleted.
                  if ( k == 5 && CheckExistenceOfEntity( vSrcDialog, szlCtrlMapView ) == zCURSOR_SET )
                  {
                     GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlCtrlMapView, szlName );
                     ulLth += zstrlen( pchBuffer + ulLth ) + 1;
                  }
                  else
                     pchBuffer[ ulLth++ ] = 0;
               }
            }
         }

         if ( k == 4 )
         {
            if ( SetEntityCursor( vSrcDialog, szlCtrlMap, 0, zPOS_NEXT | zTEST_CSR_RESULT, 0, 0, 0, 0, 0, 0 ) < 0 )
            {
               pchBuffer[ ulLth++ ] = 0;
            }
         }

         nRC = SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 );
      }

      // Set the structure of attributes for this ComboBox.
      SetAttributeFromBlob( vTgtDialog, szlCtrl,
                            szlCtrlBOI, pchBuffer, ulLth );
      pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
      DrFreeTaskMemory( pchBuffer );
   }

   // If Mapping Error.
   if ( (ulSubtype & zCOMBOBOX_EXTERNAL_DATA) == 0 && lMappingError > 0 )
   {
      // If no ComboBox type or ComboBox from type Domain.
      if ( lMappingError == 3 || ulSubtype == 0  ||
           ((ulSubtype & (zCOMBOBOX_POSITION_ONLY | zCOMBOBOX_SELECT | zCOMBOBOX_INCLUDE | zCOMBOBOX_FOREIGN_KEY)) == 0 ) )
      {
         fnDisplayCtrlError( vSubtask, vSrcDialog,
                             "ComboBox MAPPING error ... Control has no mapping entity.\nComboBox: ", FALSE );
      }
      else
      if ( lMappingError == 1 ) // no Mapping for Edit Control
      {
         fnDisplayCtrlError( vSubtask, vSrcDialog,
                             "ComboBox MAPPING error ... EditCtrl has no mapping entity.\nComboBox - ", TRUE );
         return( -16 );
      }
      else  // no Mapping for List Box
      {
         fnDisplayCtrlError( vSubtask, vSrcDialog,
                             "ComboBox MAPPING error ... ListCtrl has no mapping entity.\nComboBox - ", TRUE );
         return( -16 );
      }
   }

   if ( (ulSubtype & 0x0000F000) != 0 &&  // not a domain type of ComboBox
        SetCursorFirstEntity( vSrcDialog, szlCtrlCtrl, 0 ) >= zCURSOR_SET &&
        SetCursorNextEntity( vSrcDialog, szlCtrlCtrl, 0 ) >= zCURSOR_SET )
   {
      ulSubtype |= zCOMBOBOX_MULTICOL;
      OrderEntityForView( vSrcDialog, szlCtrlCtrl, "PSDLG_X A" );
      SetAttributeFromInteger( vTgtDialog, szlCtrl, szlSubtype, ulSubtype );
      SetCursorFirstEntity( vSrcDialog, szlCtrlCtrl, 0 );
      return( -1 );  // conversion should convert sub-controls
   }
   else
      return( 0 );
}

//./ ADD NAME=ConvertGraph
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:     ConvertGraph
//
//  PURPOSE: I'm not going to use this procedure right now because I do not
//           care about performance but I will use it in the future.  It
//           will need some work because now it doesn't work.
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertGraph( zVIEW   vSubtask,
              zVIEW   vTgtDialog,
              zVIEW   vTgtRemote,
              zVIEW   vSrcDialog,
              zPSHORT nCtrlID )
{
   zVIEW  vWork;
   zPCHAR pchTitle;
   zPCHAR pchBuffer;
   zLONG  lType;

   // Note that the Control entity has already been processed using the
   // SetMatchingAttributesByName operation.

   // GET LENGTH OF BLOB
   // ALLOCATE LENGTH OF BUFFER
   // GET BLOB WITH BUFFER
   // ADD CTRLMAPS TO BUFFER AFTER TITLE

   ActivateEmptyObjectInstance( &vWork, "TZPNCTWO", vSubtask, zSINGLE );

   CreateEntity( vWork, "TZPNCTWO", zPOS_AFTER );
   CreateEntity( vWork, "Graph", zPOS_AFTER );
   //  Set Graph attribute values from Control.CtrlBOI
   SetEntityAttributesFromBlob( vWork, "Graph", vSrcDialog, szlControl, szlCtrlBOI );

   if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
   {
      IssueError( vSubtask, 0, 0, "Memory alloc error - convert ComboBox" );
      return( 1 );
   }

   pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
   GetAddrForAttribute( (zCOREMEM) &pchTitle, vWork, "Graph", "GraphTitle" );
   GetIntegerFromAttribute( &lType, vWork, "Graph", "GraphType" );
   zULONG ulLth = 0;
   strcpy_s( pchBuffer, zMAX_BLOB, pchTitle );
   ulLth += zstrlen( pchTitle ) + 1;
   *((zPLONG) (pchBuffer + ulLth)) = lType;
   ulLth += sizeof( zLONG );

   // set the structure of attributes for this ComboBox
   SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, ulLth );
   pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
   DrFreeTaskMemory( pchBuffer );

   return( 0 );
}

//./ ADD NAME=ConvertGroupBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertGroupBox
//
//  PURPOSE:
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertGroupBox( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   zULONG ulSubtype;

   CreateEntity( vTgtRemote, "GroupBox", zPOS_AFTER );
   GetIntegerFromAttribute( (zPLONG) &ulSubtype, vSrcDialog, szlControl, szlSubtype );
   ulSubtype |= zCONTROL_NOTABSTOP;
   SetAttributeFromInteger( vSrcDialog, szlControl, szlSubtype, ulSubtype );
   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlSubtype, ulSubtype );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) >= 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

//./ ADD NAME=ConvertGroupBox
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertControlBar
//
//  PURPOSE:
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertControlBar( zVIEW   vSubtask,
                   zVIEW   vTgtDialog,
                   zVIEW   vTgtRemote,
                   zVIEW   vSrcDialog,
                   zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "ControlBar", zPOS_AFTER );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) >= 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

//./ ADD NAME=ConvertBitmap
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertBitmap
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertBitmap( zVIEW   vSubtask,
               zVIEW   vTgtDialog,
               zVIEW   vTgtRemote,
               zVIEW   vSrcDialog,
               zPSHORT nCtrlID )
{
// zCHAR  szURL[ zMAX_FILENAME_LTH ];
   zCTRLBOI_BMP BMP;
   zPCHAR pchCtrlBOI;
   zULONG ulLth;

   CreateEntity( vTgtRemote, "Bitmap", zPOS_AFTER );

   GetAttributeLength( &ulLth, vSrcDialog, "Control", "CtrlBOI" );
   GetAddrForAttribute( &pchCtrlBOI, vSrcDialog, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );
   fnResetResourceImage( vSubtask, vTgtRemote, "Bitmap", "URL", BMP.szBMPUpOrIconName, BMP.lBMPUpOrIconID );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) >= 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

//./ ADD NAME=ConvertBitmapBtn
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertBitmapBtn
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertBitmapBtn( zVIEW   vSubtask,
                  zVIEW   vTgtDialog,
                  zVIEW   vTgtRemote,
                  zVIEW   vSrcDialog,
                  zPSHORT nCtrlID )
{
// zCHAR  szURL[ zMAX_FILENAME_LTH ];
   zCTRLBOI_BMP BMP;
   zPCHAR pchCtrlBOI;
   zULONG ulLth;

   CreateEntity( vTgtRemote, "BitmapBtn", zPOS_AFTER );

   GetAttributeLength( &ulLth, vSrcDialog, "Control", "CtrlBOI" );
   GetAddrForAttribute( &pchCtrlBOI, vSrcDialog, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );
   fnResetResourceImage( vSubtask, vTgtRemote, "BitmapBtn", "URL_Up", BMP.szBMPUpOrIconName, BMP.lBMPUpOrIconID );
   fnResetResourceImage( vSubtask, vTgtRemote, "BitmapBtn", "URL_Down", BMP.szBMPDownName, BMP.lBMPDownID );
   fnResetResourceImage( vSubtask, vTgtRemote, "BitmapBtn", "URL_Focus", BMP.szBMPFocusName, BMP.lBMPFocusID );
   fnResetResourceImage( vSubtask, vTgtRemote, "BitmapBtn", "URL_Disabled", BMP.szBMPDisabledName, BMP.lBMPDisabledID );
   return( 0 );
}

//./ ADD NAME=ConvertIcon
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertIcon
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertIcon( zVIEW   vSubtask,
             zVIEW   vTgtDialog,
             zVIEW   vTgtRemote,
             zVIEW   vSrcDialog,
             zPSHORT nCtrlID )
{
// zCHAR  szURL[ zMAX_FILENAME_LTH ];
   zCTRLBOI_BMP BMP;
   zPCHAR pchCtrlBOI;
   zULONG ulLth;

   CreateEntity( vTgtRemote, "Icon", zPOS_AFTER );

   GetAttributeLength( &ulLth, vSrcDialog, "Control", "CtrlBOI" );
   GetAddrForAttribute( &pchCtrlBOI, vSrcDialog, "Control", "CtrlBOI" );
   SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );
   fnResetResourceImage( vSubtask, vTgtRemote, "Icon", "URL", BMP.szBMPUpOrIconName, BMP.lBMPUpOrIconID );
   return( 0 );
}

//./ ADD NAME=ConvertRadioBtnGroup
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertRadioBtnGroup
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertRadioBtnGroup( zVIEW   vSubtask,
                      zVIEW   vTgtDialog,
                      zVIEW   vTgtRemote,
                      zVIEW   vSrcDialog,
                      zPSHORT nCtrlID )
{
   zULONG ulSubtype;

   CreateEntity( vTgtRemote, "RadioGroup", zPOS_AFTER );
   GetIntegerFromAttribute( (zPLONG) &ulSubtype, vSrcDialog, szlControl, szlSubtype );
   ulSubtype |= zCONTROL_NOTABSTOP;
   SetAttributeFromInteger( vSrcDialog, szlControl, szlSubtype, ulSubtype );
   SetAttributeFromInteger( vTgtDialog, szlCtrl, szlSubtype, ulSubtype );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) >= 0 )
      return( -1 );  // conversion should convert sub-controls
   else
      return( 0 );
}

#define zRADIOBUTTON_DEFAULT          0x00000001L
//./ ADD NAME=ConvertRadioBtn
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertRadioBtn
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertRadioBtn( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   zPCHAR pch;

   CreateEntity( vTgtRemote, "RadioButton", zPOS_AFTER );
   GetAddrForAttribute( (zCOREMEM) &pch, vSrcDialog, szlControl, szlCtrlBOI );
   if ( *pch )
   {
      zULONG ulSubtype;
      zVIEW  vGroup;

      SetAttributeFromString( vTgtRemote, "RadioButton", "Checked", "Y" );

      CreateViewFromViewForTask( &vGroup, vTgtDialog, vSubtask );
      ResetViewFromSubobject( vGroup );
      GetIntegerFromAttribute( (zPLONG) &ulSubtype, vGroup, szlCtrl, szlSubtype );
      ulSubtype |= zRADIOBUTTON_DEFAULT;
      SetAttributeFromInteger( vGroup, szlCtrl, szlSubtype, ulSubtype );
      DropView( vGroup );
   }

   return( 0 );
}

#define zGRID_EDITBOX          0x00000000L
#define zGRID_COMBOBOX         0x00010000L
#define zGRID_DATE             0x00020000L
#define zGRID_FLOAT            0x00040000L
#define zGRID_INTEGER          0x00080000L
#define zGRID_PIC              0x00100000L
#define zGRID_PICTURE          0x00200000L
#define zGRID_TEXT             0x00400000L
#define zGRID_TIME             0x00800000L
#define zGRID_BUTTON           0x01000000L
#define zGRID_STATICTEXT       0x02000000L
#define zGRID_CHECKBOX         0x04000000L

//./ ADD NAME=ConvertGrid
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertGrid
//
//  PURPOSE:    Convert Grid (SpreadSheet)
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertGrid( zVIEW   vSubtask,
             zVIEW   vTgtDialog,
             zVIEW   vTgtRemote,
             zVIEW   vSrcDialog,
             zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "Grid", zPOS_AFTER );

   // Note that the composite Control entity has already been processed
   // using the SetMatchingAttributesByName operation.
   zSHORT nRC = CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      zVIEW  vTopSrcDialog;
      zCHAR  szTempString[ 256 ];
      zPCHAR pchEmpty = "";
      zPCHAR pchSubCtrlName;
      zPCHAR pchBuffer;
      zPCHAR pchEntityName;
      zPCHAR pchEntity;
      zPCHAR pchAttribute;
      zPCHAR pchContext;
      zPCHAR pchText;
      zCHAR  szSubBlob[ (3 * sizeof( zLONG )) + 34 ]; // flags + 2 reserved longs + tag
      zULONG ulSubLth;
      zULONG ulLth = sizeof( zLONG ) + sizeof( zULONG );
      zLONG  lCharLth;
      zULONG ulSubtype;
      zLONG  lKey;
      zLONG  lControlType;
      zSHORT nDIL_Lth;
      zLONG  lCols;
      zLONG  k;

      CreateViewFromViewForTask( &vTopSrcDialog, vSrcDialog, vSubtask );

      // Note that we are stealing the attribute name on the composite
      // control to store the scoping entity name!!!
      if ( SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
         {
            SetAttributeFromAttribute( vTgtDialog, szlCtrlMap, "AN", vSrcDialog, szlCtrlMapLOD_Entity, szlName );
            // this is temporary for bootstrap
            GetAddrForAttribute( (zCOREMEM) &pchText, vTgtDialog, szlCtrlMap, "AN" );
            if ( *pchText == 0 )
               SetAttributeFromString( vTgtDialog, szlCtrlMap, "AN", "Type" );
         }

         if ( SetCursorNextEntity( vTgtDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
            DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_BEFORE );

         SetCursorFirstEntity( vSrcDialog, szlCtrlMap, 0 );
      }

      // ViewName and EntityName must be set on the composite control for
      // a Grid ... enforced by the Grid maintenance dialog???
      if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
      {
         GetAddrForAttribute( (zCOREMEM) &pchEntityName, vSrcDialog, szlCtrlMapLOD_Entity, szlName );
      }
      else
      {
         pchEntityName = pchEmpty;
         fnDisplayCtrlError( vSubtask, vSrcDialog, "Grid MAPPING error ... Control has no mapping entity.\nGrid: ", TRUE );
         DropView( vTopSrcDialog );
         return( -16 );
      }

      if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
      {
         IssueError( vSubtask, 0, 0, "Memory alloc error - convert Grid" );
         pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
         DrFreeTaskMemory( pchBuffer );
         DropView( vTopSrcDialog );
         return( 1 );
      }

      pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
      OrderEntityForView( vSrcDialog, szlCtrlCtrl, "PSDLG_X A" );
      SetViewToSubobject( vSrcDialog, szlCtrlCtrl );  // process sub-controls

      // Set up mapping information for Grid processing.
      nRC = SetCursorFirstEntity( vSrcDialog, szlControl, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( ulLth > zMAX_BLOB - 200 )
         {
            IssueError( vSubtask, 0, 0, "Cannot convert Grid" );
            pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
            DrFreeTaskMemory( pchBuffer );
            DropView( vTopSrcDialog );
            return( 1 );
         }

         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapRelatedEntity ) != 0 )
         {
         // The following statement was removed by DonC on 11/21/2007 because it is sometimes valid not to
         // have mapping for a Grid Control.
         // fnDisplayCtrlError( vSubtask, vSrcDialog,
         //                     "Grid MAPPING error ... Grid Ctrl has no mapping entity.\nGrid - ", TRUE );
            GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlControl, szlTag );
            TraceLine( "Grid MAPPING error ... Grid Ctrl (%s) has no mapping entity", pchSubCtrlName );
            nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
            continue;
         }

         // Using the control type (Key) set lControlType to a high order
         // number that will be or'ed with the control subtype.
         GetIntegerFromAttribute( &lKey, vSrcDialog, szlControlDef, szlKey );
         if ( lKey == zGRID_EDIT_CONTROL || lKey == zGRID_SUB_CONTROL ||
              lKey == zGRID_COMBO_CONTROL || lKey == zGRID_CHECK_CONTROL )
         {
            // we like these just fine!
            // zGRID_EDIT_CONTROL                  2011L
            // zGRID_COMBO_CONTROL                 2012L
            // zGRID_CHECK_CONTROL                 2013L
            // zGRID_SUB_CONTROL                   2014L
         }
         else
         {
            // We have to create a "sub control" with the proper information and
            // move the real control underneath the "sub control".
            zVIEW vTemp;
            zVIEW vPE;
            zCHAR szTag[ 34 ];

            GetViewByName( &vPE, "TZPESRCO", vSubtask, zLEVEL_TASK );
            if ( vPE && SetCursorFirstEntityByInteger( vPE, szlControlDef, szlKey, zGRID_SUB_CONTROL, 0 ) == 0 )
            {
               lKey = zGRID_SUB_CONTROL;
            // GetIntegerFromAttribute( &lKey, vSrcDialog, szlControlDef, szlKey );
               CreateViewFromView( &vTemp, vSrcDialog );
               CreateEntity( vSrcDialog, szlControl, zPOS_AFTER );  // doesn't have to be a meta
               SetMatchingAttributesByName( vSrcDialog, szlControl, vTemp, szlControl, zSET_ALL );
               szTag[ 0 ] = '_'; // reset the tag to the tag preceded by an underscore
               GetStringFromAttribute( szTag + 1, zsizeof( szTag ) - 1, vTemp, szlControl, szlTag );
               SetAttributeFromString( vTemp, szlControl, szlTag, szTag );
               IncludeSubobjectFromSubobject( vSrcDialog, szlControlDef, vPE, szlControlDef, zPOS_BEFORE );

               if ( SetCursorFirstEntity( vTemp, szlCtrlMap, 0 ) == zCURSOR_SET )
               {
                  CreateEntity( vSrcDialog, szlCtrlMap, zPOS_AFTER );  // doesn't have to be a meta
                  SetMatchingAttributesByName( vSrcDialog, szlCtrlMap, vTemp, szlCtrlMap, zSET_NULL | zSET_NOTNULL );
                  if ( CheckExistenceOfEntity( vTemp, szlCtrlMapLOD_Attribute ) == zCURSOR_SET )
                  {
                     IncludeSubobjectFromSubobject( vSrcDialog, szlCtrlMapLOD_Attribute, vTemp, szlCtrlMapLOD_Attribute, zPOS_AFTER );
                  }

                  if ( CheckExistenceOfEntity( vTemp, szlCtrlMapLOD_Entity ) == zCURSOR_SET )
                  {
                     IncludeSubobjectFromSubobject( vSrcDialog, szlCtrlMapLOD_Entity, vTemp, szlCtrlMapLOD_Entity, zPOS_AFTER );
                  }

                  if ( CheckExistenceOfEntity( vTemp, szlCtrlMapContext ) == zCURSOR_SET )
                  {
                     IncludeSubobjectFromSubobject( vSrcDialog, szlCtrlMapContext, vTemp, szlCtrlMapContext, zPOS_AFTER );
                  }

                  if ( CheckExistenceOfEntity( vTemp, szlCtrlMapView ) == zCURSOR_SET )
                  {
                     IncludeSubobjectFromSubobject( vSrcDialog, szlCtrlMapView, vTemp, szlCtrlMapView, zPOS_AFTER );
                  }
               }

               if ( MoveSubobject( vSrcDialog, szlCtrlCtrl, vTemp, szlControl, zPOS_AFTER, zREPOS_NEXT ) == zCALL_ERROR )
               {
                  fnDisplayCtrlError( vSubtask, vTemp, "Grid MAPPING error ... Grid SubCtrl unable to move.\nGrid - ", TRUE );
               }

               DropView( vTemp );
            }
         }

         if ( lKey == zGRID_SUB_CONTROL )
         {
            // Trick conversion into thinking we have specified a sub-control.
            lKey = zGRID_EDIT_CONTROL;
            ulSubLth = zsizeof( szSubBlob );
            GetBlobFromAttribute( szSubBlob, &ulSubLth, vSrcDialog, szlControl, "CtrlBOI" );
            if ( ulSubLth != zsizeof( szSubBlob ) )
            {
               zmemset( szSubBlob, 0, zsizeof( szSubBlob ) );
               ulSubLth = zsizeof( szSubBlob );
            }

            if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) == 0 )
            {
               GetStringFromAttribute( szSubBlob + (3 * sizeof( zLONG )), zsizeof( szSubBlob ) - (3 * sizeof( zLONG )), vSrcDialog, szlCtrlCtrl, szlTag );

               SetViewToSubobject( vSrcDialog, szlCtrlCtrl );
               if ( MoveSubobject( vTopSrcDialog, szlControl, vSrcDialog, szlControl, zPOS_AFTER, zREPOS_NEXT ) == zCALL_ERROR )
               {
                  fnDisplayCtrlError( vSubtask, vSrcDialog, "Grid MAPPING error ... Grid SubCtrl unable to move.\nGrid - ", TRUE );
               }

               ResetViewFromSubobject( vSrcDialog );
            }

            SetAttributeFromBlob( vSrcDialog, szlControl, szlCtrlBOI, szSubBlob, ulSubLth );
         // zmemcpy( pchBuffer + ulLth, szSubBlob, zsizeof( szSubBlob ) );
         // pchBuffer[ ulLth ] = (char) 0xFF;
         // ulLth += ulSubLth;
         }

         // Adding Entity name Lth and Entity name to CtrlBOI.
         GetAddrForAttribute( (zCOREMEM) &pchEntity, vSrcDialog, szlCtrlMapRelatedEntity, szlName );

         // If the same entity ... omit from list ... flagged by 0 byte.
         if ( zstrcmp( pchEntityName, pchEntity ) == 0 )
         {
            *((zPLONG) (pchBuffer + ulLth)) = 0L;  // set length of entity
            ulLth += sizeof( zLONG );
         }
         else
         {
            k = zstrlen( pchEntity ) + 1;
            *((zPLONG) (pchBuffer + ulLth)) = k;  // set length of entity
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchEntity );
            ulLth += k;
         }

         // Adding Attribute name Lth and Attribute name to CtrlBOI.
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapER_Attribute ) == 0 )
         {
            GetAddrForAttribute( (zCOREMEM) &pchAttribute, vSrcDialog, szlCtrlMapER_Attribute, szlName );
         }
         else
         {
         // if ( CompareAttributeToString( vSrcDialog, szlControlDef, szlTag, "PushBtn" ) != 0 )
            if ( lKey == zPUSHBUTTON_CONTROL )
            {
               GetAddrForAttribute( (zCOREMEM) &pchSubCtrlName, vSrcDialog, szlControl, szlTag );
               TraceLine( "Grid MAPPING error ... Grid Ctrl (%s) has no mapping attribute", pchSubCtrlName );
            }
            else
            {
               fnDisplayCtrlError( vSubtask, vSrcDialog, "Grid MAPPING error ... Grid Ctrl has no mapping attribute.\nGrid - ", TRUE );
               ResetViewFromSubobject( vSrcDialog );
               DropView( vTopSrcDialog );
               return( -16 );
            }
         }

         k = zstrlen( pchAttribute ) + 1;
         *((zPLONG) (pchBuffer + ulLth)) = k;  // set length of attribute
         ulLth += sizeof( zLONG );
         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchAttribute );
         ulLth += k;

         // Adding Context name Lth and Context name to CtrlBOI
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapContext ) == 0 )
         {
            GetAddrForAttribute( (zCOREMEM) &pchContext, vSrcDialog, szlCtrlMapContext, szlName );
         }
         else
            pchContext = pchEmpty;

         k = zstrlen( pchContext ) + 1;
         *((zPLONG) (pchBuffer + ulLth)) = k;  // set length of attribute
         ulLth += sizeof( zLONG );
         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchContext );
         ulLth += k;

         // Get the control type (Key) and using that set lControlType to a
         // high order number that will be or'ed with the control subtype.
         if ( lKey == zGRID_EDIT_CONTROL ) // || lKey == zGRID_SUB_CONTROL )
            lControlType = zGRID_EDITBOX;
         else
         if ( lKey == zGRID_COMBO_CONTROL )
            lControlType = zGRID_COMBOBOX;
         else
         if ( lKey == zGRID_CHECK_CONTROL )
            lControlType = zGRID_CHECKBOX;
      // else
      // {
      // // MessageBox( 0, "Invalid Grid Control", "Grid Conversion", MB_OK );
      //    lControlType = 0;
      // }

         *((zPLONG) (pchBuffer + ulLth)) = lControlType;
         ulLth += sizeof( zLONG );

         // Set the Subtype (right-justify, disabled, etc.).
         GetIntegerFromAttribute( (zPLONG) &ulSubtype, vSrcDialog, szlControl, szlSubtype );

         // Sizeable borders do not make sense for a grid sub-control ...
         // so we ensure the flag is set off and at runtime, we use this
         // flag position to implement MiUpdate.
         ulSubtype &= ~zCONTROL_SIZEABLEBORDER;

         *((zPLONG) (pchBuffer + ulLth)) = ulSubtype;
         ulLth += sizeof( zLONG );

         if ( lControlType == zGRID_CHECKBOX )
         {
            zPCHAR pch;

            GetAddrForAttribute( (zCOREMEM) &pch, vSrcDialog, szlControl, szlText );
            lCharLth = zstrlen( pch ) + 1;               // Text
            *((zPLONG) (pchBuffer + ulLth)) = lCharLth;
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pch );
            ulLth += lCharLth;

            GetAddrForAttribute( (zCOREMEM) &pch, vSrcDialog, szlControl, szlCtrlBOI );
            lCharLth = zstrlen( pch ) + 1;
            *((zPLONG) (pchBuffer + ulLth)) = lCharLth;   // Value ON
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pch );
            ulLth += lCharLth;

            pch += lCharLth;
            lCharLth = zstrlen( pch ) + 1;
            *((zPLONG) (pchBuffer + ulLth)) = lCharLth;   // Value OFF
            ulLth += sizeof( zLONG );
            strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pch );
            ulLth += lCharLth;
         }

         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
      }

      // Store offset to tags in second slot.
      *((zPULONG) (pchBuffer + sizeof( zLONG ))) = ulLth;

      // Set up tags for Grid processing.
      nRC = SetCursorFirstEntity( vSrcDialog, szlControl, 0 );
      lCols = 0;
      while ( nRC > zCURSOR_UNCHANGED )
      {
         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapRelatedEntity ) != 0 )
         {
            nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
            continue;
         }

         lCols++;

         // New grid ... put 0xFF in first byte as a flag.
         ulSubLth = zsizeof( szSubBlob );
         GetBlobFromAttribute( szSubBlob, &ulSubLth, vSrcDialog, szlControl, "CtrlBOI" );
         if ( ulSubLth == zsizeof( szSubBlob ) )
         {
            zmemcpy( pchBuffer + ulLth, szSubBlob, zsizeof( szSubBlob ) );
            pchBuffer[ ulLth ] = (char) 0xFF;
            ulLth += ulSubLth;
         }

         // Save the width of this attribute in dialog units.
         GetIntegerFromAttribute( &lCharLth, vSrcDialog, szlControl, szlSZDLG_X );
         *((zPLONG) (pchBuffer + ulLth)) = (zLONG) lCharLth;
         ulLth += sizeof( zLONG );

         // Set the length of the text.
         GetAddrForAttribute( (zCOREMEM) &pchText, vSrcDialog, szlControl, szlText );

         k = zstrlen( pchText ) + 1;
         nDIL_Lth = fnGetDIL_Msg( vSrcDialog, szTempString + 1, zsizeof( szTempString ) - 1 );
         if ( nDIL_Lth )
         {
            szTempString[ 0 ] = '\t';
            nDIL_Lth = (zSHORT) zstrlen( szTempString );
            k += nDIL_Lth;
         }

         *((zPLONG) (pchBuffer + ulLth)) = k;  // set the text length
         ulLth += sizeof( zLONG );

         // Set the value of the text.
         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchText );
         if ( nDIL_Lth )
            strcat_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, szTempString );

         ulLth += k;

         // Set the length of the tag.
         GetAddrForAttribute( (zCOREMEM) &pchText, vSrcDialog, szlControl, szlTag );

         k = zstrlen( pchText ) + 1;
         *((zPLONG) (pchBuffer + ulLth)) = k;
         ulLth += sizeof( zLONG );

         // Set the name of the tag.
         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, pchText );
         ulLth += k;

         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
      }

      *((zPLONG) pchBuffer) = lCols;  // store the number of controls

      // Set the structure of attributes for this Grid.
      ResetViewFromSubobject( vSrcDialog );
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, (zUSHORT) ulLth );
      pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
      DrFreeTaskMemory( pchBuffer );
      DropView( vTopSrcDialog );
      return( 0 );
   }
   else
      return( 1 );
}

//./ ADD NAME=ConvertDIL
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertDIL
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertDIL( zVIEW   vSubtask,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT nCtrlID )
{
   STATUSBAR_ITEM *pStatusItem;
   zPCHAR  pchBuffer;
   zULONG  ulLth;
   zSHORT  nRC;

   CreateEntity( vTgtRemote, "Status", zPOS_AFTER );

   // need mode indicator ... bitmap, button, control, text, separator???
   // need alignment indicator ... left, center, right???
   // need border style ... none, plain, raised, recessed, embossed???
   // auto-size indicator???

   if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
   {
      IssueError( vSubtask, 0, 0, "Memory alloc error - convert DIL" );
      return( 1 );
   }

   pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );
   nRC = CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl );
   if ( nRC >= zCURSOR_UNCHANGED )
   {
      zTZWDLGSO_Control_DEF CtrlDef;
      zULONG  ulIdNbr;
      zSHORT  nItems = 0;
      zLONG   lLastEnd = 0;

      while ( nRC > zCURSOR_UNCHANGED )
      {
         nItems++;
         nRC = SetCursorNextEntity( vSrcDialog, szlCtrlCtrl, 0 );
      }

      ulLth = 2 * sizeof( zLONG ) + nItems * sizeof( STATUSBAR_ITEM );
      pStatusItem = (STATUSBAR_ITEM *) (pchBuffer + 2 * sizeof( zLONG ));
      GetIntegerFromAttribute( (zPLONG) &ulIdNbr, vSrcDialog, szlControl, szlSubtype );
      *((zPLONG) pchBuffer) = ulIdNbr;  // nAutoDIL
      *((zPLONG) (pchBuffer + sizeof( zLONG ))) = nItems; // nItems

      nItems = 0;
      nRC = SetCursorFirstEntity( vSrcDialog, szlCtrlCtrl, 0 );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         nItems++;
         // get the structure of attributes for this control
         CtrlDef.CheckByte = 0xFE;
         CtrlDef.lStructSize = sizeof( zTZWDLGSO_Control_DEF );
         GetStructFromEntityAttrs( (zPCHAR) &CtrlDef, vSrcDialog, szlCtrlCtrl );
         if ( CtrlDef.CheckByte != 0xFE )
            MessageBox( 0, "CtrlDef Structure Length Error", "GetStructFromEntityAttrs", MB_OK );

         pStatusItem->lSeqNbr = nItems;
         pStatusItem->lOffset = CtrlDef.PSDLG_X - lLastEnd;
         lLastEnd = CtrlDef.PSDLG_X + CtrlDef.SZDLG_X;
         pStatusItem->lWidth = CtrlDef.SZDLG_X;

         strcpy_s( pchBuffer + ulLth, zMAX_BLOB - ulLth, CtrlDef.Text );
         pStatusItem->pchText = (zPCHAR)(zLONG) ulLth;
         ulLth += StringLthOn4_ByteBoundary( CtrlDef.Text ) + 1;

         pStatusItem->lMode = CtrlDef.Subtype;
         pStatusItem++;

         nRC = SetCursorNextEntity( vSrcDialog, szlCtrlCtrl, 0 );
      }

      // set the structure of attributes for this DIL
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, ulLth );
   }
   else
   {
      ulLth = sizeof( STATUSBAR_ITEM ) + 2 * sizeof( zLONG );
      *((zPLONG) pchBuffer) = 1;  // nAutoDIL
      *((zPLONG) (pchBuffer + sizeof( zLONG ))) = 1; // nItems
      pStatusItem = (STATUSBAR_ITEM *) (pchBuffer + 2 * sizeof( zLONG ));
      pStatusItem->lSeqNbr = 1;
      pStatusItem->lOffset = 4;
      pStatusItem->lWidth = 0;   // auto size
      pStatusItem->pchText = (zPCHAR)(zLONG) ulLth;
      *(pchBuffer + ulLth) = 0;
      pStatusItem->lMode = 0;  // Text/Left/Recessed
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, ulLth + sizeof( zSHORT ) );
   }

   MoveSubobject( vTgtDialog, szlCtrl, vTgtDialog, szlCtrl, zPOS_FIRST, zREPOS_PREV );
   pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
   DrFreeTaskMemory( pchBuffer );
   return( 0 );
}

//./ ADD NAME=ConvertHD
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertHD
//
//  PURPOSE:
//
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertHD( zVIEW   vSubtask,
           zVIEW   vTgtDialog,
           zVIEW   vTgtRemote,
           zVIEW   vSrcDialog,
           zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "HD", zPOS_AFTER );
   return( 0 );
}

//./ ADD NAME=ConvertERD
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertERD
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertERD( zVIEW   /* vSubtask */,
            zVIEW   vTgtDialog,
            zVIEW   vTgtRemote,
            zVIEW   vSrcDialog,
            zPSHORT nCtrlID )
{
   // Note that we are stealing the attribute name on the ERD control
   // to store the scoping entity name!!!
   if ( SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
   {
      if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapLOD_Entity ) == 0 )
         SetAttributeFromAttribute( vTgtDialog, szlCtrlMap, "AN", vSrcDialog, szlCtrlMapLOD_Entity, szlName );

      if ( SetCursorNextEntity( vTgtDialog, szlCtrlMap, 0 ) > zCURSOR_UNCHANGED )
         DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_BEFORE );

      SetCursorFirstEntity( vSrcDialog, szlCtrlMap, 0 );
   }

   return( 0 );
}

//./ ADD NAME=ConvertToolBar
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertToolBar
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertToolBar( zVIEW   vSubtask,
                zVIEW   vTgtDialog,
                zVIEW   vTgtRemote,
                zVIEW   vSrcDialog,
                zPSHORT nCtrlID )
{
   zSHORT nRC;

   CreateEntity( vTgtRemote, "ToolBar", zPOS_AFTER );

   // Note that the composite Control entity has already been processed
   // using the SetMatchingAttributesByName operation.
   nRC = CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl );
   if ( nRC > zCURSOR_UNCHANGED )
   {
      zCHAR  szTempString[ 256 ];
      zCTRLBOI_BMP BMP;
      zPCHAR pchCtrlBOI;
      zPCHAR pchBuffer;
      zPCHAR pch;
      zLONG  lType;
      zLONG  lTypeNext;
      zULONG ulLth;
      zLONG  lBtnCnt;
      zLONG  lBtnInfoCnt;

      if ( DrAllocTaskMemory( (zCOREMEM) &pchBuffer, zMAX_BLOB ) != 0 )
      {
         IssueError( vSubtask, 0, 0, "Memory alloc error - convert ToolBar" );
         DrFreeTaskMemory( pchBuffer );
         return( -16 );
      }

      // Handle the sub-controls within the toolbar ... setting up the
      // mapping information in a blob.
      pchBuffer = (zPCHAR) DrLockTaskMemory( pchBuffer );

      GetAttributeLength( &ulLth, vSrcDialog, "Control", "CtrlBOI" );
      GetAddrForAttribute( &pchCtrlBOI, vSrcDialog, "Control", "CtrlBOI" );
      SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );
      fnResetResourceImage( vSubtask, vTgtRemote, "ToolBar", "URL", BMP.szBMPUpOrIconName, BMP.lBMPUpOrIconID );

      // Starting blob with room for four longs.
      ulLth = zMAX_BLOB - (4 * sizeof( zLONG ));
      GetBlobFromAttribute( pchBuffer + (4 * sizeof( zLONG )), (zPULONG) &ulLth, vSrcDialog, szlControl, szlCtrlBOI );
      *((zPULONG) pchBuffer) = ulLth;
      ulLth += (4 * sizeof( zLONG ));

      OrderEntityForView( vSrcDialog, szlCtrlCtrl, "PSDLG_X A" );
      SetViewToSubobject( vSrcDialog, szlCtrlCtrl );

      // Set the correct ToolBar IdNbr.
      zVIEW vWindow;
      CreateViewFromViewForTask( &vWindow, vSrcDialog, vSubtask );
      while ( ResetViewFromSubobject( vWindow ) == 0 )
      {
      }

      GetIntegerFromAttribute( &lType, vWindow, "Window", "BarID" );
      SetAttributeFromInteger( vTgtDialog, "Ctrl", "IdNbr", lType );
      SetAttributeFromInteger( vWindow, "Window", "BarID", lType + 1 );
      DropView( vWindow );

      lBtnCnt = 0;
      lBtnInfoCnt = 0;

      // Position on the first Control entity.
      nRC = SetCursorFirstEntity( vSrcDialog, szlControl, 0 );
      while ( nRC >= zCURSOR_SET )
      {
         lBtnCnt++;
         lBtnInfoCnt++;

         // Set type/tag/text/tip/action/view/entity/attribute/scope labels.
         GetIntegerFromAttribute( &lType, vSrcDialog, szlControl, szlSubtype );
         GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, szlControl, "Underline" );
         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
         if ( nRC >= zCURSOR_SET )
         {
            GetIntegerFromAttribute( &lTypeNext, vSrcDialog, szlControl, szlSubtype );
            SetCursorPrevEntity( vSrcDialog, szlControl, 0 );
         }
         else
            lTypeNext = 0;

         // If the next ctrl is a combobox or a separator is requested.
         if ( lType == 3 || lTypeNext == 3 || szTempString[ 0 ] == '1' )
         {
            lType |= 0x80000000;
            lBtnInfoCnt++;
         }

         *((zPLONG) (pchBuffer + ulLth)) = lType;
         ulLth += sizeof( zPLONG );
         GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlControl, szlTag );
         ulLth += zstrlen( pchBuffer + ulLth ) + 1;
         GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlControl, szlText );
         pch = zstrchr( pchBuffer + ulLth, '\t' );
         ulLth += zstrlen( pchBuffer + ulLth ) + 1;
         if ( pch == 0 )
            pchBuffer[ ulLth++ ] = 0;  // null ToolTip
         else
            *pch = 0;   // separate Text from ToolTip

         if ( CheckExistenceOfEntity( vSrcDialog, szlEventAct ) == 0 )
         {
            GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlEventAct, szlTag );
            ulLth += zstrlen( pchBuffer + ulLth ) + 1;
         }
         else
            pchBuffer[ ulLth++ ] = 0;  // null Action

         if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapER_Attribute ) == 0 )
         {
            GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlCtrlMapView, szlName );
            ulLth += zstrlen( pchBuffer + ulLth ) + 1;
            GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlCtrlMapRelatedEntity, szlName );
            ulLth += zstrlen( pchBuffer + ulLth ) + 1;
            GetStringFromAttribute( pchBuffer + ulLth, zMAX_BLOB - ulLth, vSrcDialog, szlCtrlMapER_Attribute, szlName );
            ulLth += zstrlen( pchBuffer + ulLth ) + 1;
            pchBuffer[ ulLth++ ] = 0;  // null Scope for now
         }
         else
         {
            pchBuffer[ ulLth++ ] = 0;  // null View
            pchBuffer[ ulLth++ ] = 0;  // null Entity
            pchBuffer[ ulLth++ ] = 0;  // null Attribute
            pchBuffer[ ulLth++ ] = 0;  // null Scope
         }

         nRC = SetCursorNextEntity( vSrcDialog, szlControl, 0 );
      }

      *((zPLONG) (pchBuffer + (1 * sizeof( zLONG )))) = 0; // reserved
      *((zPLONG) (pchBuffer + (2 * sizeof( zLONG )))) = lBtnCnt;
      *((zPLONG) (pchBuffer + (3 * sizeof( zLONG )))) = lBtnInfoCnt;

      // Set the structure of attributes for this ToolBar.
      ResetViewFromSubobject( vSrcDialog );
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, pchBuffer, ulLth );
      pchBuffer = (zPCHAR) DrUnlockTaskMemory( pchBuffer );
      DrFreeTaskMemory( pchBuffer );

      return( 0 );
   }
   else
   {
      ResetViewFromSubobject( vSrcDialog );
      SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, "", 0 );
      fnDisplayCtrlError( vSubtask, vSrcDialog, "ToolBar error ... ListBox has no ListCtrls.\nToolBar - ", TRUE );
      return( -16 );
   }
}

//./ ADD NAME=ConvertTimer
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertTimer
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertTimer( zVIEW   vSubtask,
              zVIEW   vTgtDialog,
              zVIEW   vTgtRemote,
              zVIEW   vSrcDialog,
              zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "Timer", zPOS_AFTER );
   SetAttributeFromAttribute( vTgtRemote, "Timer", "Interval", vTgtDialog, szlCtrl, "StyleX" );
   SetAttributeFromString( vTgtRemote, szlCtrl, "StyleX", "" );
   return( 0 );
}

//./ ADD NAME=ConvertCalendar
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertCalendar
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertCalendar( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "Calendar", zPOS_AFTER );
   return( 0 );
}

//./ ADD NAME=ConvertDayTimer
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertDayTimer
//
//  PURPOSE:
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertDayTimer( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   CreateEntity( vTgtRemote, "DayTimer", zPOS_AFTER );
   return( 0 );
}

//./ ADD NAME=ConvertOutliner
// Source Module=tzctlcvt.c
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:      ConvertOutliner
//
//  PURPOSE:
//
//  DESCRIPTION: The tree processes a blob of memory with the following
//               format:
//
//                  zVIEW   vSubtask  - must be "plugged" by calling custom
//                                      control
//                  zLONG  lOffset    - correction offset caused by building
//                                      this tree blob in funny order???
//                  zPCHAR lpViewName - application OI view name
//                  OutlineItem       - root entity structure as follows:
//
// typedef struct tagOutlineItem
// {
//    zLONG    lTagAttributeOffset; // Offset to attribute name used to identify entity
//    zLONG    lAttributeOffset;    // Offset to array of attributes (null terminated)
//    zLONG    lSiblingEntityOffset; // Offset to next sibling entity struct
//    zLONG    lChildEntityOffset;  // Offset to first child entity struct
//    zLONG    lFlags;              // Recursive entity indicator ...
//                                  // If children should be expanded on init
//                                  // If the parent view should be used for mapping
//    zCHAR    szEntityName[ zTAG_LTH ]; // Entity/attribute name
//    zCHAR    szScopeEntity[ zTAG_LTH ]; // Scope for entity for szEntityName
//    zCHAR    szRecursiveEntity[ zTAG_LTH ]; // Recursive entity name
//
// } zTREE_ITEM, FAR *zLPOL_ITEM;
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 5
zOPER_EXPORT zSHORT OPERATION
ConvertOutliner( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   zPCHAR  lpOldBuffer;
   zPCHAR  lpBuffer;
   zUSHORT lOffset = sizeof( zVIEW );
   zLONG   lLth = zMAX_BLOB;
   zUSHORT nStringLth;

   CreateEntity( vTgtRemote, "Tree", zPOS_AFTER );

   // Get the CTRL_BOI from the PWD.
   if ( DrAllocTaskMemory( (zCOREMEM) &lpOldBuffer, zMAX_BLOB ) != 0 )
   {
      IssueError( vSubtask, 0, 0, "Memory error - cannot convert Tree" );
      return( 1 );
   }

   lpOldBuffer = (zPCHAR) DrLockTaskMemory( lpOldBuffer );
   GetBlobFromAttribute( lpOldBuffer, (zPULONG) &lLth, vSrcDialog, szlControl, szlCtrlBOI );

   // strip off the registered view name and 2 longs
   nStringLth = (zSHORT) zstrlen( lpOldBuffer ) + 1;
   lpBuffer = lpOldBuffer + nStringLth;
   lLth -= nStringLth;

   nStringLth = sizeof( zTREE_ITEM ) + 3 * sizeof( zLONG );
   if ( lLth < (zLONG) nStringLth )
   {
      lpOldBuffer = (zPCHAR) DrUnlockTaskMemory( lpOldBuffer );
      DrFreeTaskMemory( lpOldBuffer );

      fnDisplayCtrlError( vSubtask, vSrcDialog, "Tree MAPPING error ... Control has no mapping entity.\nTree: ", TRUE );
      return( -16 );
   }

   lpBuffer += sizeof( zLONG );
   lpBuffer += sizeof( zLONG );
   lLth -= sizeof( zLONG );
   lLth -= sizeof( zLONG );

   // Note that the composite Control entity has already been processed
   // using the SetMatchingAttributesByName operation.
// GetBlobFromAttribute( lpBuffer + lOffset, &lLth, vSrcDialog,
//                       szlControl, szlCtrlBOI );
// if ( lLth > 0 )
//    lOffset += lLth;
// else
//    lOffset++;

   SetAttributeFromBlob( vTgtDialog, szlCtrl, szlCtrlBOI, lpBuffer, lLth );
   lpOldBuffer = (zPCHAR) DrUnlockTaskMemory( lpOldBuffer );
   DrFreeTaskMemory( lpOldBuffer );

   return( 0 );
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
//
// Control will contain a CtrlMap for the "root" entity name and its
// corresponding identifier attribute (if there is one).  There will
// also be a CtrlMap for each attribute to display for the entity.
// For each sub-entity of the "root" entity there will be a CtrlCtrl
// with CtrlMap(s) as specified above.
//
//       We need to keep the order and the TOP_VIEW information
//       in the CtrlMap since there are multiple mappings for
//       each control.  We will use the tag as a tab-separated
//       structure as follows:
//
//          byte(s)      use
//          -------   ---------------------------------------------------
//            0       Tag attribute (only one per entity - Y/N)
//            2       Selected attribute (Y/N)
//           4-6      Sequence number (001-999)
//           8-39     Context
//
//                    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//                    ³     Control     ³
//                    ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ
//                             ³
//             ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//             ³                                 ³
//    ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿               ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
//    ³    CtrlCtrl     ³               ³     CtrlMap     ³
//    ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ               ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//
//
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
zUSHORT
fnBuildTreeItems( zVIEW       vSrcDialog,
                      zPCHAR  lpBuffer,
                      zLONG   lOffset )
{
#if 0
   zLPOL_ITEM OL_Item;
   zLPOL_ITEM OL_ItemTemp;
   zCHAR      szTempString[ 256 ];
   zPCHAR     pch;
   zLONG      lSubtype;
   zULONG     ulLth = zTAG_LTH;
   zSHORT     nRC;

   OL_Item = (zLPOL_ITEM) (lpBuffer + lOffset);
   lOffset += zsizeof( zTREE_ITEM );

   // set the mapping entity name into the OL_Item data structure
   GetStringFromAttribute( OL_Item->szEntityName, vSrcDialog, szlCtrlMapRelatedEntity, szlName );

   // get the scoping entity from the Controls CtrlBOI and set it into the OL_Item data structure
   GetBlobFromAttribute( OL_Item->szScopeEntity, &ulLth, vSrcDialog, szlControl, szlCtrlBOI );
   OL_Item->szScopeEntity[ ulLth ] = 0;

   // get the list of attributes to be displayed for this entity
   OL_Item->lTagAttributeOffset = 0;
   OL_Item->lAttributeOffset = 0;
   nRC = SetCursorFirstEntity( vSrcDialog, szlCtrlMap, 0 );
   while ( nRC > zCURSOR_UNCHANGED )
   {
      if ( OL_Item->lAttributeOffset == 0 )
      {
         OL_Item->lTagAttributeOffset = lOffset;
         lpBuffer[ OL_Item->lTagAttributeOffset ] = 0;

         lOffset += zTAG_LTH;
         OL_Item->lAttributeOffset = lOffset;
      }

      GetStringFromAttribute( szTempString, zsizeof( szTempString ), vSrcDialog, szlCtrlMap, szlTag );
//    TraceLineS( "Tree CtrlMap Tag ", szTempString );

      GetStringFromAttribute( lpBuffer + lOffset, vSrcDialog, szlCtrlMapER_Attribute, szlName );
      if ( szTempString[ 0 ] == 'Y' ||
           lpBuffer[ OL_Item->lTagAttributeOffset ] == 0 )
      {
//       TraceLineS( "Tree tag ", lpBuffer + lOffset );
         strcpy_s( lpBuffer + OL_Item->lTagAttributeOffset, commented out???, lpBuffer + lOffset );
      }

      if ( szTempString[ 2 ] == 'Y' )
      {
//       TraceLineS( "Tree attribute ", lpBuffer + lOffset );
         lOffset += zstrlen( lpBuffer + lOffset ) + 1;
         if ( (pch = zstrchr( szTempString + 8, '\t' )) != 0 )
            *pch = 0;

         strcpy_s( lpBuffer + lOffset, commented out???, szTempString + 8 ); // context
         lOffset += zstrlen( lpBuffer + lOffset ) + 1;
      }

      nRC = SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 );
   }

   lpBuffer[ lOffset++ ] = 0;    // double null list terminator

   // if this control has a CtrlCtrl then that means there are children
   // so set the offset for the child entities.
   nRC = CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl );
   if ( nRC > zCURSOR_UNCHANGED )
      OL_Item->lChildEntityOffset = lOffset;
   else
      OL_Item->lChildEntityOffset = 0;

   GetIntegerFromAttribute( &lSubtype, vSrcDialog, szlControl, szlSubtype );
   if ( lSubtype & zTREE_RECURSIVE_ENTITY )
   {
      OL_Item->lFlags |= zTREE_RECURSIVE;
      GetStringFromAttribute( OL_Item->szRecursiveEntity, vSrcDialog, szlControl, szlText );
   }

   if ( lSubtype & zTREE_OPEN_CHILDREN )
      OL_Item->lFlags |= zTREE_CHILDREN_OPEN;

   // If there are CtrlCtrls then set to the subobject and call this function recursively.
   OL_ItemTemp = OL_Item;
   while ( nRC > zCURSOR_UNCHANGED )
   {
      OL_ItemTemp = (zLPOL_ITEM) (lpBuffer + lOffset);
      SetViewToSubobject( vSrcDialog, szlCtrlCtrl );
      lOffset = fnBuildTreeItems( vSrcDialog, lpBuffer, lOffset );
      ResetViewFromSubobject( vSrcDialog );

      // If there is more than one CtrlCtrl under the current control,
      // set the sibling offset to indicate during execution that there
      // are more CtrlCtrl's.
      nRC = SetCursorNextEntity( vSrcDialog, szlCtrlCtrl, 0 );
      if ( nRC > zCURSOR_UNCHANGED )
         OL_ItemTemp->lSiblingEntityOffset = lOffset;
      else
         OL_ItemTemp->lSiblingEntityOffset = 0;
   }

   return( (zUSHORT) lOffset );
#else
   return( 0 );
#endif
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//
// This is the documentation for the Bitmap and Icon processing from both
// the painter (specification) and the driver (runtime) perspectives.
//
// The four controls involved have the following keys:
//
//  ICON - 2060   BITMAP - 2080   BITMAP BUTTON - 2085   TOOLBAR - 2100
//
// In all cases, relevent information concerning the icon or bitmap is
// stored in the CtrlBOI as follows:
//
//    zPCHAR pchResourceDLLName  - name of the DLL in which the icons or
//                                 bitmaps reside (in the .PWD, absence of
//                                 a DLL name indicates the driver will NOT
//                                 try to load the resource from a DLL).
//    zPCHAR pchIconOrBitmapName - name of the icon or (up) bitmap
//    zPCHAR pchBitmapDownName   - "down" bitmap name (applies only to
//                                 bitmap button)
//    zPCHAR pchBitmapFocusName  - "focus" bitmap name (applies only to
//                                 bitmap button)
//
// These strings are separated by nulls.
//
// The name of any icon or bitmap in the runtime (driver) case may be of
// the form:
//
//    - resource id followed by a semi-colon
//    - string resource id or file name
//    - resource id followed by a semi-colon followed by a string resource id
//      or file name
//
// For example, the following forms are valid:
//
//    4097;c:\10d\a\dr\driver.ico
//    MYICON
//    c:\windows\castle.bmp
//    TEXTUP TEXTDN TEXTFO
//
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  BuildCtrlBOI_BMP
//
//  PURPOSE:    For the dialog PWD (in LOD "TZWDLGSO"):
//              Build the BLOB CtrlBOI for an Control with BMP or ICON
//              resources out of resource names/IDs
//
//  PARAMETERS: pBMP - split information
//              pchCtrlBOI - pointer to BLOB-buffer
//              lCtrlBOI_Size - size of BLOB-buffer
//
//  RETURNS:    Length of BLOB CtrlBOI
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zLONG OPERATION
BuildCtrlBOI_BMP( zCTRLBOI_BMP *pBMP,
                  zPCHAR       pchCtrlBOI,
                  zLONG        lCtrlBOI_Size )
{
   zCHAR  szID[ 32 ];
   zPCHAR pch;
   zPCHAR pchCtrl = pchCtrlBOI;
   zLONG  lLth;

   strncpy_s( pchCtrlBOI, lCtrlBOI_Size, pBMP->szDLLName, lCtrlBOI_Size - 1 );
   lLth = zstrlen( pBMP->szDLLName ) + 1;

   pch = pchCtrl + lLth;
   *pch = 0;
   if ( pBMP->szBMPUpOrIconName[ 0 ] )
   {
      if ( pBMP->lBMPUpOrIconID )
      {
         _ltoa_s( pBMP->lBMPUpOrIconID, szID, zsizeof( szID ), 10 );
         strncat_s( pch, lCtrlBOI_Size - lLth, szID, lCtrlBOI_Size - (lLth + 1) );
         strcat_s( pch, lCtrlBOI_Size - lLth, ";" );
      }

      strncat_s( pch, lCtrlBOI_Size - lLth, pBMP->szBMPUpOrIconName, lCtrlBOI_Size - (lLth + 1) );
   }

   lLth += zstrlen( pch ) + 1;

   pch = pchCtrl + lLth;
   *pch = 0;
   if ( pBMP->szBMPDownName[ 0 ] )
   {
      if ( pBMP->lBMPDownID )
      {
         _ltoa_s( pBMP->lBMPDownID, szID, zsizeof( szID ), 10 );
         strncat_s( pch, lCtrlBOI_Size - lLth, szID, lCtrlBOI_Size - (lLth + 1) );
         strcat_s( pch, lCtrlBOI_Size - lLth, ";" );
      }

      strncat_s( pch, lCtrlBOI_Size - lLth, pBMP->szBMPDownName, lCtrlBOI_Size - (lLth + 1) );
   }

   lLth += zstrlen( pch ) + 1;

   pch = pchCtrl + lLth;
   *pch = 0;
   if ( pBMP->szBMPFocusName[ 0 ] )
   {
      if ( pBMP->lBMPFocusID )
      {
         _ltoa_s( pBMP->lBMPFocusID, szID, zsizeof( szID ), 10 );
         strncat_s( pch, lCtrlBOI_Size - lLth, szID, lCtrlBOI_Size - (lLth + 1) );
         strcat_s( pch, lCtrlBOI_Size - lLth, ";" );
      }

      strncat_s( pch, lCtrlBOI_Size - lLth, pBMP->szBMPFocusName, lCtrlBOI_Size - (lLth + 1) );
   }

   lLth += zstrlen( pch ) + 1;

   pch = pchCtrl + lLth;
   *pch = 0;
   if ( pBMP->szBMPDisabledName[ 0 ] )
   {
      if ( pBMP->lBMPDisabledID )
      {
         _ltoa_s( pBMP->lBMPDisabledID, szID, zsizeof( szID ), 10 );
         strncat_s( pch, lCtrlBOI_Size - lLth, szID, lCtrlBOI_Size - (lLth + 1) );
         strcat_s( pch, lCtrlBOI_Size - lLth, ";" );
      }

      strncat_s( pch, lCtrlBOI_Size - lLth, pBMP->szBMPDisabledName, lCtrlBOI_Size - (lLth + 1) );
   }

   lLth += zstrlen( pch ) + 1;

   // Check max. length
   if ( lLth > lCtrlBOI_Size )
   {
      pchCtrlBOI[ lCtrlBOI_Size - 1 ] = 0;
      lLth = lCtrlBOI_Size;
   }

   return( lLth );

} // BuildCtrlBOI_BMP

zLONG
fnGetMaxResourceID( zVIEW  vDialog,
                    zLONG  lMaxResID )
{
   zCPCHAR cpcCtrlEntity = "Control";
   zCPCHAR cpcCtrlDefEntity = "ControlDef";
   zCTRLBOI_BMP BMP;
   zPCHAR  pchCtrlBOI;
   zLONG   lCtrlType;
   zULONG  ulLth;
   zBOOL   bResourceDLL = FALSE;
   zSHORT  nRC;

   for ( nRC = SetCursorFirstEntity( vDialog, cpcCtrlEntity, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, cpcCtrlEntity, 0 ) )
   {
      if ( CheckExistenceOfEntity( vDialog, "CtrlCtrl" ) >= zCURSOR_SET )
      {
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         lMaxResID = fnGetMaxResourceID( vDialog, lMaxResID );
         ResetViewFromSubobject( vDialog );
      }

      // Determine the control type.
      GetIntegerFromAttribute( &lCtrlType, vDialog, cpcCtrlDefEntity, "Key" );

      // Is it a control that can have resources?
      if ( lCtrlType == zICON_CONTROL ||
           lCtrlType == zBITMAP_CONTROL ||
           lCtrlType == zBITMAP_BUTTON_CONTROL ||
           lCtrlType == zTOOLBAR_CONTROL )
      {
         // Does the control actually have resources?
         GetAttributeLength( &ulLth, vDialog, cpcCtrlEntity, "CtrlBOI" );
         if ( ulLth )
         {
            // Get resources.
            GetAddrForAttribute( &pchCtrlBOI, vDialog, cpcCtrlEntity, "CtrlBOI" );
            SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );

            // If no resource DLL has been specified ...
            if ( BMP.szDLLName[ 0 ] == 0 )
            {
               // for BMP up / Icon
               if ( BMP.szBMPUpOrIconName[ 0 ] )
               {
                  if ( BMP.lBMPUpOrIconID > lMaxResID )
                     lMaxResID = BMP.lBMPUpOrIconID;
                  else
                     lMaxResID++;
               }

               // for BMP down
               if ( BMP.szBMPDownName[ 0 ] )
               {
                  if ( BMP.lBMPDownID > lMaxResID )
                     lMaxResID = BMP.lBMPDownID;
                  else
                     lMaxResID++;
               }

               // for BMP focus
               if ( BMP.szBMPFocusName[ 0 ] )
               {
                  if ( BMP.lBMPFocusID > lMaxResID )
                     lMaxResID = BMP.lBMPFocusID;
                  else
                     lMaxResID++;
               }

               // for BMP disabled
               if ( BMP.szBMPDisabledName[ 0 ] )
               {
                  if ( BMP.lBMPDisabledID > lMaxResID )
                     lMaxResID = BMP.lBMPDisabledID;
                  else
                     lMaxResID++;
               }
            }
            else
            {
               if ( bResourceDLL == FALSE )
               {
                  bResourceDLL = TRUE;
                  TraceLineS( "Dialog Conversion:  Resource DLL specified ... no resources generated in .RC file: ", BMP.szDLLName );
               }
            }
         }
      }
   } // each control in a window

   return( lMaxResID );
}

// returns: 1 - value set
//          0 - value already set
//         -1 - value already set to some other value
zSHORT
fnSetRC_Byte( zVIEW vSubtask, zPCHAR pchRC_Bytes, zLONG lId, zCPCHAR cpcResName )
{
   zPCHAR pch;
   zPLONG pl = (zPLONG) (pchRC_Bytes + (lId - zMAX_RES_ID_USER - 1) * sizeof( zLONG ));
   if ( *pl )
   {
      pch = (zPCHAR) *pl;
      if ( zstricmp( pch, cpcResName ) == 0 )
         return( 0 );   // already set to the correct value
      else
         return( -1 );  // this has already been set to something else
   }

   zPVOID pv;
   zLONG  lLth = zstrlen( cpcResName ) + 1;
   DrAllocTaskMemory( (zCOREMEM) &pv, lLth );
   pch = (zPCHAR) DrLockTaskMemory( pv );
   strcpy_s( pch, lLth, cpcResName );
   *pl = (zLONG) pch;
   return( 1 );
}

zBOOL
fnIsRC_ByteSet( zPCHAR pchRC_Bytes, zLONG lId, zCPCHAR cpcResName )
{
   zPLONG pl = (zPLONG) (pchRC_Bytes + (lId - zMAX_RES_ID_USER - 1) * sizeof( zLONG ));
   if ( *pl )
   {
      zPCHAR pch = (zPCHAR) *pl;
      if ( zstricmp( pch, cpcResName ) )
         return( TRUE );
   }

   return( FALSE );
}

zSHORT
fnWriteResourceLine( zVIEW         vSubtask,
                     zLONG         hFile,
                     zCOMPILE_DIRS *pcd,
                     zSHORT        nResType,
                     zPCHAR        pchLine,
                     zLONG         lMaxLineLth,
                     zPLONG        plMaxResID,
                     zPCHAR        pchRC_Bytes,
                     zCPCHAR       cpcResName,
                     zPLONG        plResID )
{
   zSHORT nChangeLevel = 0;

   // for BMP up / Icon
   // If name exists and if no ID exists or ID not user defined?
   if ( cpcResName[ 0 ] && (*plResID == 0 || *plResID > zMAX_RES_ID_USER) )
   {
      if ( *plResID == 0 ||
           (*plResID > zMAX_RES_ID_USER && fnIsRC_ByteSet( pchRC_Bytes, *plResID, cpcResName )) )
      {
         // create resource ID, if not given
         do
         {
            (*plMaxResID)++;
            *plResID = *plMaxResID;

         } while ( fnIsRC_ByteSet( pchRC_Bytes, *plResID, cpcResName ) );

         nChangeLevel = 2;
      }

      zSHORT nRC = fnSetRC_Byte( vSubtask, pchRC_Bytes, *plResID, cpcResName );

      // Write resource line.
      if ( nResType == 0 )
         sprintf_s( pchLine, lMaxLineLth, "%-6ld ICON  ", *plResID );
      else
   // if ( nResType == 1 || nResType == 2 )  // bitmap to toolbar
         sprintf_s( pchLine, lMaxLineLth, "%-6ld BITMAP ", *plResID );

      if ( hFile )
      {
         if ( nRC > 0 )
         {
            zSHORT nLth = (zSHORT) zstrlen( pchLine );
            fnBuildRC_Filename( vSubtask, pchLine + nLth, cpcResName );
            if ( pchLine[ nLth ] )
               SysWriteLine( vSubtask, hFile, pchLine );
         }
         else
         if ( nRC < 0 )
            nChangeLevel++;
      }

      nChangeLevel++;
      if ( pcd )
         pcd->nResourceFiles++;
   }

   return( nChangeLevel );
}

zSHORT
fnWriteCtrlResourcesToRC( zVIEW         vSubtask,
                          zLONG         hFile,
                          zVIEW         vDialog,
                          zCOMPILE_DIRS *pcd,
                          zPCHAR        pchLine,
                          zLONG         lMaxLineLth,
                          zPLONG        plMaxResID,
                          zPCHAR        pchRC_Bytes )
{
   zCPCHAR cpcCtrlEntity = "Control";
   zCPCHAR cpcCtrlDefEntity = "ControlDef";
   zCTRLBOI_BMP BMP;
   zPCHAR  pchCtrlBOI;
   zLONG   lCtrlType;
   zULONG  ulLth;
   zSHORT  nResType;
   zSHORT  nChangeLevel;
   zBOOL   bCtrlChanged;
   zSHORT  nReturnLevel = 0;
   zSHORT  nRC;

   // Write resource lines for the controls
   for ( nRC = SetCursorFirstEntity( vDialog, cpcCtrlEntity, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, cpcCtrlEntity, 0 ) )
   {
      if ( CheckExistenceOfEntity( vDialog, "CtrlCtrl" ) >= zCURSOR_SET )
      {
         SetViewToSubobject( vDialog, "CtrlCtrl" );
         nChangeLevel = fnWriteCtrlResourcesToRC( vSubtask, hFile, vDialog, pcd, pchLine, lMaxLineLth, plMaxResID, pchRC_Bytes );
         if ( nChangeLevel > nReturnLevel )
            nReturnLevel = nChangeLevel;

         ResetViewFromSubobject( vDialog );
      }

      // look for control type
      GetIntegerFromAttribute( &lCtrlType, vDialog, cpcCtrlDefEntity, "Key" );

      // determine resource type
      switch ( lCtrlType )
      {
        case zICON_CONTROL:
          nResType = 0; // ICON
          break;

        case zBITMAP_CONTROL:
        case zBITMAP_BUTTON_CONTROL:
          nResType = 1; // BITMAP
          break;

        case zTOOLBAR_CONTROL:
          nResType = 2; // TOOLBAR
          break;

        default:
          nResType = -1;
      }

      // if ICON, BITMAP or BITMAP BUTTON
      if ( nResType >= 0 )
      {
         // Get the resource names.
         GetAttributeLength( &ulLth, vDialog, cpcCtrlEntity, "CtrlBOI" );
         if ( ulLth )
         {
            // Get resources.
            GetAddrForAttribute( &pchCtrlBOI, vDialog, cpcCtrlEntity, "CtrlBOI" );
            SplitCtrlBOI_BMP( pchCtrlBOI, ulLth, &BMP );

            // If no resource DLL has been specified ...
            if ( BMP.szDLLName[ 0 ] == 0 )
            {
               nChangeLevel = 0;
               bCtrlChanged = FALSE;

               // for BMP up / Icon
               nChangeLevel = fnWriteResourceLine( vSubtask, hFile, pcd, nResType, pchLine, lMaxLineLth, plMaxResID, pchRC_Bytes,
                                                   BMP.szBMPUpOrIconName, &BMP.lBMPUpOrIconID );
               if ( nChangeLevel )
               {
                  bCtrlChanged = TRUE;
                  if ( nChangeLevel > nReturnLevel )
                     nReturnLevel = nChangeLevel;
               }

               // for BMP down
               nChangeLevel = fnWriteResourceLine( vSubtask, hFile, pcd, nResType, pchLine, lMaxLineLth, plMaxResID, pchRC_Bytes,
                                                   BMP.szBMPDownName, &BMP.lBMPDownID );
               if ( nChangeLevel )
               {
                  bCtrlChanged = TRUE;
                  if ( nChangeLevel > nReturnLevel )
                     nReturnLevel = nChangeLevel;
               }

               // for BMP focus
               nChangeLevel = fnWriteResourceLine( vSubtask, hFile, pcd, nResType, pchLine, lMaxLineLth, plMaxResID, pchRC_Bytes,
                                                   BMP.szBMPFocusName, &BMP.lBMPFocusID );
               if ( nChangeLevel )
               {
                  bCtrlChanged = TRUE;
                  if ( nChangeLevel > nReturnLevel )
                     nReturnLevel = nChangeLevel;
               }

               // for BMP disabled
               nChangeLevel = fnWriteResourceLine( vSubtask, hFile, pcd, nResType, pchLine, lMaxLineLth, plMaxResID, pchRC_Bytes,
                                                   BMP.szBMPDisabledName, &BMP.lBMPDisabledID );
               if ( nChangeLevel )
               {
                  bCtrlChanged = TRUE;
                  if ( nChangeLevel > nReturnLevel )
                     nReturnLevel = nChangeLevel;
               }

               // Save the generated resource IDs to BLOB.
               if ( bCtrlChanged )
               {
                  ulLth = BuildCtrlBOI_BMP( &BMP, pchLine, zLINESIZE );
                  SetAttributeFromBlob( vDialog, cpcCtrlEntity, "CtrlBOI", pchLine, ulLth );
               }
            } // if create RC entry
         }
      }
   } // each control per window

   return( nReturnLevel );
}

zOPER_EXPORT zSHORT OPERATION
SetResourceFileIDs( zVIEW         vSubtask,
                    zVIEW         vTaskLPLR,
                    zLONG         hFile,
                    zCOMPILE_DIRS *pcd,
                    zPCHAR        pchLine,
                    zLONG         lMaxLineLth,
                    zVIEW         vDialog )
{
   const  zPCHAR cpcWinEntity = "Window";
   zSHORT nChangeLevel = 0;
   zLONG  lMaxResID;
   zPVOID pv;
   zPCHAR pchRC_Bytes;
   zLONG  lBytes;
   zSHORT nRC;

   // Check the controls for old resource IDs to determine lMaxResID.
   lMaxResID = zMAX_RES_ID_USER + 1;
   for ( nRC = SetCursorFirstEntity( vDialog, cpcWinEntity, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, cpcWinEntity, 0 ) )
   {
      lMaxResID = fnGetMaxResourceID( vDialog, lMaxResID );
   }

   // Make sure that we are in valid range for zeidon resource IDs.
   if ( lMaxResID < zMAX_RES_ID_USER )
      lBytes = 1;
   else
      lBytes = (lMaxResID - zMAX_RES_ID_USER) + 1;

   DrAllocTaskMemory( (zCOREMEM) &pv, lBytes * sizeof( zLONG ) );
   pchRC_Bytes = (zPCHAR) DrLockTaskMemory( pv );
   zmemset( pchRC_Bytes, 0, lBytes * sizeof( zLONG ) );
   lMaxResID = zMAX_RES_ID_USER;

   // Check the controls for old resource IDs to determine lMaxResID.
   for ( nRC = SetCursorFirstEntity( vDialog, cpcWinEntity, 0 );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vDialog, cpcWinEntity, 0 ) )
   {
      // Need to pass in TaskLPLR rather than vSubtask to write the RC!!!
      nRC = fnWriteCtrlResourcesToRC( vTaskLPLR, hFile, vDialog, pcd, pchLine, lMaxLineLth, &lMaxResID, pchRC_Bytes );
      if ( nRC > 0 )
      {
         if ( nRC > nChangeLevel )
            nChangeLevel = nRC;
#if 0
         zVIEW vMetaListPE;

         // Open PE, it is used for commiting a dialog meta
         // Don C note on 03/18/1998.  Guenter T wrote this code and as of
         // this date we don't see any reason for it, but we are leaving it
         // in anyway.
         RetrieveViewForMetaList( vSubtask, vSubtask, &vMetaListPE, zREFER_PENV_META );
         if ( SetCursorFirstEntity( vMetaListPE, "W_MetaDef", "" ) < zCURSOR_SET )
         {
            // I assume this means we have an LPLR without Dialogs, such as a reusable CPLR with only Global Operations.
            DropView( vMetaListPE );
            DropMetaOI( vDialog );
            break;
         }
         else
         {
            DropView( vMetaListPE );
         }
#endif
      }
   } // each window (to get old resources)

   zPCHAR pch;
   zPLONG pl;

   while ( lBytes-- )
   {
      pl = (zPLONG) (pchRC_Bytes + (lBytes * sizeof( zLONG )));
      pch = (zPCHAR) *pl;
      if ( pch )
      {
         pv = DrUnlockTaskMemory( pch );
         DrFreeTaskMemory( pv );
      }
   }

   pv = DrUnlockTaskMemory( pchRC_Bytes );
   DrFreeTaskMemory( pv );

   return( nChangeLevel );
}

//./ ADD NAME=ConvertTrueGrid
// Source Module=tzctlcvt.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  ENTRY:       ConvertTrueGrid
//                                            4/25/2001 Modified
//  PURPOSE:
//
//  PARAMETERS:  zVIEW   vSubtask -
//              zVIEW   vTgtDialog -
//              zVIEW   vTgtRemote -
//              zVIEW   vSrcDialog -
//              zPSHORT nCtrlID -
////////////////////////////////////////////////////////////////////////////
//./END + 6
zOPER_EXPORT zSHORT OPERATION
ConvertTrueGrid( zVIEW   vSubtask,
                 zVIEW   vTgtDialog,
                 zVIEW   vTgtRemote,
                 zVIEW   vSrcDialog,
                 zPSHORT nCtrlID )
{
   zLONG lControlType = 0;
   zLONG lSequence = 0;
   zCHAR szViewName[ MAX_PATH ];
   zCHAR szEntityName[ MAX_PATH ];
   zCHAR szAttributeName[ MAX_PATH ];
   zCHAR szContextName[ MAX_PATH ];


   // Note that the Control entity has already been processed
   // using the SetMatchingAttributesByName operation.
   // create the remote Object
   // by default the TrueGrid is always a multiselect control
   CreateEntity( vTgtRemote, "Grid", zPOS_AFTER );
   SetAttributeFromString( vTgtRemote, "Grid", "MultiSelect", "Y" );

   if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) == zCURSOR_SET )
   {
      CreateEntity( vTgtDialog, szlCtrlCtrl, zPOS_AFTER );
      SetViewToSubobject( vTgtDialog, szlCtrlCtrl );
      SetCursorFirstEntity( vTgtDialog, szlCtrl,"" );

      SetViewToSubobject( vSrcDialog, szlCtrlCtrl );

      zSHORT nRC = SetCursorFirstEntity( vSrcDialog, szlControl,"" );
      while ( nRC > zCURSOR_UNCHANGED )
      {
         // setup xra
         CreateEntity( vTgtRemote, "GridColumn", zPOS_AFTER );
         // supply default values
         SetAttributeFromString( vTgtRemote, "GridColumn", "InsertNode", "Y");
         SetAttributeFromString( vTgtRemote, "GridColumn", "Editable", "Y");
         SetAttributeFromString( vTgtRemote, "GridColumn", "DeleteNode", "N" );
         SetAttributeFromString( vTgtRemote, "GridColumn", "ChangeNode", "N" );
         // supply common attributes
         SetAttributeFromInteger( vTgtRemote, "GridColumn", "Tag", lSequence );
         //SetAttributeFromAttribute( vTgtRemote, "GridColumn", "Tag", vSrcDialog, szlControl, "Tag" );
         SetAttributeFromAttribute( vTgtRemote, "GridColumn", "Text", vSrcDialog, szlControl, "Text" );
         // set appearing order and Tab order
         SetAttributeFromInteger( vTgtRemote, "GridColumn", "IdNbr", lSequence );
         SetAttributeFromInteger( vTgtRemote, "GridColumn", "NodeIndex", lSequence );

         GetIntegerFromAttribute( &lControlType, vSrcDialog, szlControl, szlType );
         switch ( lControlType )
         {
            case CONTROL_TYPE_COMBOBOX:
               CreateEntity( vTgtRemote, "GridComboBox", zPOS_AFTER ); // thats it
               SetAttributeFromInteger( vTgtRemote, "GridComboBox", "Tag", lSequence );
               break;

            case CONTROL_TYPE_CHECKBOX:
               CreateEntity( vTgtRemote, "GridCheckBox", zPOS_AFTER ); // thats it
               SetAttributeFromInteger( vTgtRemote, "GridCheckBox", "Tag", lSequence );
               break;

            case CONTROL_TYPE_EDIT:
               CreateEntity( vTgtRemote, "GridEditBox", zPOS_AFTER );
               SetAttributeFromInteger( vTgtRemote, "GridEditBox", "Tag", lSequence );
               // *** currently not supported but may be in the future
         //
         //      GetIntegerFromAttribute( &lSubtype, vSrcDialog, "Control", "Subtype" );
         //      if ( lSubtype & zEDITBOX_LOWERCASE )
         //         SetAttributeFromString( vTgtRemote, "GridEditBox", "Case", "L" );
         //      else
         //      if ( lSubtype & zEDITBOX_UPPERCASE )
         //         SetAttributeFromString( vTgtRemote, "GridEditBox", "Case", "U" );
         //      else
         //         SetAttributeFromString( vTgtRemote, "GridEditBox", "Case", "N" );
         //
         //      if ( lSubtype & zEDITBOX_PASSWORD )
         //         SetAttributeFromString( vTgtRemote, "GridEditBox", "Password", "Y" );
         //      else
         //         SetAttributeFromString( vTgtRemote, "GridEditBox", "Password", "N" );
         //            // *** so only do the default
               SetAttributeFromString( vTgtRemote, "GridEditBox", "Case", "N" );
               SetAttributeFromString( vTgtRemote, "GridEditBox", "Password", "N" );
               break;

            case CONTROL_TYPE_RADIOGRP:
               CreateEntity( vTgtRemote, "GridRadioGroup", zPOS_AFTER );
               SetMatchingAttributesByName( vTgtRemote, "GridRadioGroup", vSrcDialog, szlControl, zSET_ALL );
               SetAttributeFromInteger( vTgtRemote, "GridRadioGroup", "Tag", lSequence );
               if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlCtrl ) > zCURSOR_UNCHANGED )
               {
                  SetViewToSubobject(   vSrcDialog, szlCtrlCtrl );
                  nRC = SetCursorFirstEntity( vSrcDialog, szlControl,"" );
                  while ( nRC > zCURSOR_UNCHANGED )
                  {
                     CreateEntity( vTgtRemote, "GridRadioButton", zPOS_AFTER );
                     SetMatchingAttributesByName( vTgtRemote, "GridRadioButton", vSrcDialog, szlControl, zSET_ALL );

                     nRC = SetCursorNextEntity( vSrcDialog, szlControl,"" );
                  }

                  ResetViewFromSubobject( vSrcDialog );
               }

               break;

            default:
               break;
         }

         SetMatchingAttributesByName( vTgtDialog, szlCtrl, vSrcDialog, szlControl, zSET_ALL );

         zSHORT nRCSubControl = SetCursorFirstEntity( vSrcDialog, szlCtrlMap, "" );
         while ( nRCSubControl > zCURSOR_UNCHANGED )
         {
            szViewName[0] = 0;      // init all buffers
            szEntityName[0] = 0;
            szAttributeName[0] = 0;
            szContextName[0] = 0;
            CreateEntity( vTgtDialog, szlCtrlMap, zPOS_AFTER );
            if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapView ) == zCURSOR_SET )
            {
               GetStringFromAttribute( szViewName, zsizeof( szViewName ), vSrcDialog, szlCtrlMapView, szlName );
               GetStringFromAttribute( szEntityName, zsizeof( szEntityName ), vSrcDialog, szlCtrlMapRelatedEntity, szlName );
               GetStringFromAttribute( szAttributeName, zsizeof( szAttributeName ), vSrcDialog, szlCtrlMapER_Attribute, szlName );
            }

            if ( CheckExistenceOfEntity( vSrcDialog, szlCtrlMapContext ) == zCURSOR_SET )
            {
               GetStringFromAttribute( szContextName, zsizeof( szContextName ), vSrcDialog, szlCtrlMapContext, szlName );
            }

            SetAttributeFromString( vTgtDialog, szlCtrlMap, "VN", szViewName );
            SetAttributeFromString( vTgtDialog, szlCtrlMap, "EN", szEntityName );
            SetAttributeFromString( vTgtDialog, szlCtrlMap, "AN", szAttributeName );
            SetAttributeFromString( vTgtDialog, szlCtrlMap, "Context", szContextName );

            // Incomplete Mapping?
            // DeleteEntity( vTgtDialog, szlCtrlMap, zREPOS_BEFORE );
            nRCSubControl = SetCursorNextEntity( vSrcDialog, szlCtrlMap, 0 );
         }

         // SubControl mappings done next control
         nRC = SetCursorNextEntity( vSrcDialog, szlControl,"" );
         if( nRC > zCURSOR_UNCHANGED )
            CreateEntity( vTgtDialog, szlCtrl, zPOS_AFTER );

         lSequence++;
      }

      ResetViewFromSubobject( vSrcDialog );
      ResetViewFromSubobject( vTgtDialog );
   }

   return ( 0 );
}

zBOOL
fnFindTagAtLevel( zVIEW vWindow, zCPCHAR cpcCtrlTag )
{
   zPCHAR pchTag;
   zBOOL  bDuplicate = FALSE;

   zSHORT nRC = SetCursorFirstEntity( vWindow, "Control", 0 );
   while ( bDuplicate == FALSE && nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( &pchTag, vWindow, "Control", "Tag" );
      if ( zstrcmp( cpcCtrlTag, pchTag ) == 0 )
      {
         bDuplicate = TRUE;
      }
      else
      if ( SetViewToSubobject( vWindow, "CtrlCtrl" ) >= 0 )
      {
         bDuplicate = fnFindTagAtLevel( vWindow, cpcCtrlTag );
         ResetViewFromSubobject( vWindow );
      }

      nRC = SetCursorNextEntity( vWindow, "Control", 0 );
   }

   return( bDuplicate );
}

zBOOL
CheckDuplicateTagForWindow( zVIEW   vSubtask,
                            zVIEW   vSrcDialog,
                            zCPCHAR cpcWndTag,
                            zCPCHAR cpcCtrlTag )
{
   zVIEW  vWindow;
   zBOOL  bDuplicate = FALSE;

   // Process each Window entity.
   CreateViewFromViewForTask( &vWindow, vSrcDialog, vSubtask );
   while ( ResetViewFromSubobject( vWindow ) == 0 )
   {
   }

   zSHORT nRC = SetCursorFirstEntityByString( vWindow, "Window", "Tag", cpcWndTag, 0 );
   if ( nRC >= zCURSOR_SET )
      bDuplicate = fnFindTagAtLevel( vWindow, cpcCtrlTag );

   DropView( vWindow );
   return( bDuplicate );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnProcessControls( zVIEW vSrcDialog, zVIEW vCtrl,
                   ZNameList *pNameList, zSHORT& nCnt )
{
   zPCHAR pchTag;
   zSHORT nRC = SetCursorFirstEntity( vCtrl, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( &pchTag, vCtrl, "Control", "Tag" );
      if ( pNameList->AddTail( pchTag ) == 0 )  // 0 ==> already in list
      {
         zPCHAR pchDlgTag;
         zPCHAR pchWndTag;

         GetAddrForAttribute( &pchDlgTag, vSrcDialog, "Dialog", "Tag" );
         GetAddrForAttribute( &pchWndTag, vSrcDialog, "Window", "Tag" );
         TraceLine( "Duplicate Control Tag: %s  found in Dialog.Window: %s.%s", pchTag, pchDlgTag, pchWndTag );
         nCnt++;
      }

      if ( SetViewToSubobject( vCtrl, "CtrlCtrl" ) >= 0 )
      {
         fnProcessControls( vSrcDialog, vCtrl, pNameList, nCnt );
         ResetViewFromSubobject( vCtrl );
      }

      nRC = SetCursorNextEntity( vCtrl, "Control", 0 );
   }
}

// The only view we care about is vSrcDialog for this routine.
zOPER_EXPORT zSHORT OPERATION
CheckDuplicateTags( zVIEW   vSubtask,
                    zVIEW   vSrcDialog,
                    zLONG   lFlags )
{
   ZNameList *pNameList;
   zPCHAR pchTag;
   zVIEW  vCtrl;
   zSHORT nCnt = 0;

   // Process each Window entity.
   CreateViewFromViewForTask( &vCtrl, vSrcDialog, vSubtask );
   zSHORT nRC = SetCursorFirstEntity( vSrcDialog, "Window", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      pNameList = new ZNameList( );
      SetViewFromView( vCtrl, vSrcDialog );
      fnProcessControls( vSrcDialog, vCtrl, pNameList, nCnt );
      if ( lFlags & 0x00000001 ) // check action tags as well
      {
         nRC = SetCursorFirstEntity( vCtrl, "Action", 0 );
         while ( nRC >= zCURSOR_SET )
         {
            GetAddrForAttribute( &pchTag, vCtrl, "Action", "Tag" );
            if ( pNameList->AddTail( pchTag ) == 0 )  // 0 ==> already in list
            {
               zPCHAR pchDlgTag;
               zPCHAR pchWndTag;

               GetAddrForAttribute( &pchDlgTag, vSrcDialog, "Dialog", "Tag" );
               GetAddrForAttribute( &pchWndTag, vSrcDialog, "Window", "Tag" );
               TraceLine( "Duplicate Action Tag: %s  found in Dialog.Window: %s.%s", pchTag, pchDlgTag, pchWndTag );
               nCnt++;
            }

            nRC = SetCursorNextEntity( vCtrl, "Action", 0 );
         }
      }

      delete( pNameList );
      nRC = SetCursorNextEntity( vSrcDialog, "Window", 0 );
   }

   DropView( vCtrl );
   return( nCnt );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
fnProcessRptControls( zVIEW vSrcReport, zVIEW vReport,
                      ZNameList *pNameList, zSHORT& nCnt )
{
   zPCHAR pchTag;
   zSHORT nRC = SetCursorFirstEntity( vReport, "Control", 0 );
   while ( nRC >= zCURSOR_SET )
   {
      GetAddrForAttribute( &pchTag, vReport, "Control", "Tag" );
      if ( pNameList->AddTail( pchTag ) == 0 )
      {
         zPCHAR pchGrpSetTag;
         zPCHAR pchGrpTag;

         GetAddrForAttribute( &pchGrpSetTag, vSrcReport, "GroupSet", "Tag" );
         GetAddrForAttribute( &pchGrpTag, vSrcReport, "Group", "Tag" );
         TraceLine( "Duplicate Control Tag: %s  found in GroupSet.Group: %s.%s", pchTag, pchGrpSetTag, pchGrpTag );
         nCnt++;
      }

      if ( SetViewToSubobject( vReport, "CtrlCtrl" ) >= 0 )
      {
         fnProcessRptControls( vSrcReport, vReport, pNameList, nCnt );
         ResetViewFromSubobject( vReport );
      }

      nRC = SetCursorNextEntity( vReport, "Control", 0 );
   }
}

// The only view we care about is vSrcReport for this routine.
zOPER_EXPORT zSHORT OPERATION
CheckDuplicateReportTags( zVIEW   vSubtask,
                          zVIEW   vSrcReport,
                          zLONG   lFlags )
{
   ZNameList *pNameList;
   zVIEW  vReport;
   zSHORT nCnt = 0;
   zSHORT nRC;

   // Process each Page entity.
   CreateViewFromViewForTask( &vReport, vSrcReport, vSubtask );
   while ( ResetViewFromSubobject( vReport ) == 0 )
   {
      // nothing to do
   }

   // Loop through all GroupSets.
   for ( nRC = SetCursorFirstEntity( vReport, "GroupSet", "Page" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vReport, "GroupSet", "Page" ) )
   {
      // Loop through all Groups within the GroupSet.
      for ( nRC = SetCursorFirstEntity( vReport, "Group", "GroupSet" );
            nRC >= zCURSOR_SET;
            nRC = SetCursorNextEntity( vReport, "Group", "GroupSet" ) )
      {
         pNameList = new ZNameList( );
         fnProcessRptControls( vSrcReport, vReport, pNameList, nCnt );
         delete( pNameList );
      }
   }

   DropView( vReport );
   return( nCnt );
}

// The only view we care about is vSrcXSLT for this routine.
zOPER_EXPORT zSHORT OPERATION
CheckDuplicateXSLT_Tags( zVIEW   vSubtask,
                         zVIEW   vSrcXSLT,
                         zLONG   lFlags )
{
   ZNameList *pNameList;
   zVIEW  vXSLT;
   zSHORT nCnt = 0;
   zSHORT nRC;

   // Process each Panel entity.
   CreateViewFromViewForTask( &vXSLT, vSrcXSLT, vSubtask );
   while ( ResetViewFromSubobject( vXSLT ) == 0 )
   {
      // nothing to do
   }

   // Loop through all Panels.
   for ( nRC = SetCursorFirstEntity( vXSLT, "GroupSet", "Panel" );
         nRC >= zCURSOR_SET;
         nRC = SetCursorNextEntity( vXSLT, "GroupSet", "Panel" ) )
   {
      pNameList = new ZNameList( );
      fnProcessRptControls( vSrcXSLT, vXSLT, pNameList, nCnt );
      delete( pNameList );
   }

   DropView( vXSLT );
   return( nCnt );
}