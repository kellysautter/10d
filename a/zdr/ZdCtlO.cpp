/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Control implementations
// FILE:         zdctlo.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of C-interface operations to Zeidon
// Controls.
//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.09.21    DKS   Enhance
//    Added SetCtrlMapping to remap a Control.
//
// 2001.07.13    TMV   2001   WebGrid
//    Add inline documentation for ZWEBGRID_XXX operations.
//
// 2001.06.01    TMV   2001   WebGrid
//    Add global operations that are able to handle global operation requests
//    of the truegrid in the web environment. Those functions (TG_XXX) are
//    implemented with the truegrid dll.
//
// 2001.06.04    DKS   53105
//    ED_HiliteText now sets focus prior to selection of text.
//
// 2001.05.18    DKS   WEB
//    Fixed ED_HiliteText to send selection start and end positions.
//
// 2001.05.03    DKS   WEB
//    Fixed BMP_SetBitmapFileName to set the URL properly.
//
// 2001.04.28    BL    TREE
//    Added Parameter bEnsureVisible to Function OL_SetTextColorForItemText
//
// 2001.04.28    BL    TREE
//    Added new Function OL_RemoveAllTextColorsForItemText
//    Rename OL_RemoveAllTextColorsForItemText to OL_ResetAllTextColorsForItemText
//
// 2001.04.25    BL    TREE
//    Added new Function OL_SetTextColorForItemText
//
// 2001.04.19    DKS   Z10
//    Change to use new operation GetAppOrWorkstationValue.
//
// 2001.02.08    BL    TREE
//    Added new functions for new Tree Control
//
// 2001.01.29    BL
//    Added functions for new Tree Control
//
// 2001.01.16    BL
//    Modified BMP_SetBitmapFileName: set retrun value from SetBitmapFileName
//
// 2000.12.26    BL
//    Modified BMP_SetBitmapResource: remove #if 0 for Tree Detail Window
//
// 2000.11.18    BL
//   Set define zUSE_MS_TREE for new Tree Control,
//   Added Functions for new Tree Control
//
//2000.10.14     HH     ???
//    Changed view synchronization, now use OL_SetCursorByEntityNumber,
//    instead of absolute position.
//
// 2000.06.07    TMV    Z10
//    Changed OL_GetCurrentEntityName to work properly if the Tree
//    is switched to the MS Tree Control implementation
//
// 1999.10.01    DKS    Z10    QS9999
//    FlyBy delayed by one second when application sets message bar.
//
// 1999.09.15    DKS    Z10    ActiveX
//    Added capability to retrieve parameters for ActiveX events.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Added LB_SetSelectedPosition operation.
//
// 1999.05.11    DKS    Z10    ActiveX
//    Added GetActiveX_WrapperInstance operation.
//
// 1999.02.16    DKS
//    Suport ZListCtrl control type for LB_SetDisplaySelectSet operation.
//
// 1999.01.11    DKS
//    Added Escapement to TextBOI functions.
//
// 1998.12.31    DKS
//    Implemented Shape and altered Text Blob operations.
//
// 1998.12.15    DKS
//    Activated SpreadSheet operations.
//
// 1998.09.24    DKS
//    Fixed PrintDiagram to provide a return code to the application when
//    the print is canceled (QS 78).
//
// 1998.08.13    DKS
//    Permit HD_ZoomDiagram to return the current settings so that they
//    may be stored for future use.

#include "zstdafx.h"

#define ZDCTL_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#include "ZdCtl.h"
#include "ZdCtlGbl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//./ ADD NAME=MB_SetMessage
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: MB_SetMessage
//                                              04/01/93 Modified: 04/01/93
//
//  PURPOSE:   To set the message bar text for a particular item
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control.
//              cpcMessage  - The message to display
//              nItem       - The sub-item within the DIL (0 ==> set to
//                            the default item)
//
//  RETURNS:    -1  - Error locating window or message bar item
//               0  - OK
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
MB_SetMessage( zVIEW    vSubtask,
               zSHORT   nItem,
               zCPCHAR  cpcMessage )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
   // TraceLineS( "MB_SetMessage: ", cpcMessage );
      pZSubtask->m_ulFlyByTickCnt = SysGetTickCount( );
      return( pZSubtask->SetStatusBarText( cpcMessage, nItem ) );
   }

   return( -1 );
}

zOPER_EXPORT zSHORT OPERATION
SP_GetShapeCtrlBOI( zVIEW    vCtrl,
                    zPSHORT  pbRectangle,
                    zPSHORT  pbRoundRect,
                    zPSHORT  pbEllipse,
                    zPSHORT  pbDiamond,
                    zPSHORT  pbHorizontal,
                    zPSHORT  pbVertical,
                    zPSHORT  pbDiagDescend,
                    zPSHORT  pbDiagAscend,
                    zPLONG   plPenWidth,
                    zPLONG   plPenType,
                    zPLONG   plPenColor,
                    zPLONG   plRoundValue,
                    zPLONG   plBackgroundColor,
                    zPLONG   plFlags )
{
   zCHAR   szBlob[ 8 + 6 * sizeof( zLONG ) ];
   zULONG  ulLth = zsizeof( szBlob );

   GetBlobFromAttribute( &szBlob, &ulLth, vCtrl, "Control", "CtrlBOI" );
//MessageBox( 0, "vCtrl|Control|CtrlBOI", "Zeidon Error", MB_OK );
//::MessageBox( 0, "vCtrl|Control|CtrlBOI", "Zeidon Error", MB_OK );

   if ( ulLth == zsizeof( szBlob ) ||
        ulLth == zsizeof( szBlob ) - sizeof( zLONG ) )
   {
      *pbRectangle   = szBlob[ 0 ];
      *pbRoundRect   = szBlob[ 1 ];
      *pbEllipse     = szBlob[ 2 ];
      *pbDiamond     = szBlob[ 3 ];
      *pbHorizontal  = szBlob[ 4 ];
      *pbVertical    = szBlob[ 5 ];
      *pbDiagDescend = szBlob[ 6 ];
      *pbDiagAscend  = szBlob[ 7 ];
      *plPenWidth    = *(zPLONG) (szBlob + 8 );
      *plPenType     = *(zPLONG) (szBlob + 8 + (1 * sizeof( zLONG )) );
      *plPenColor    = *(zPLONG) (szBlob + 8 + (2 * sizeof( zLONG )) );
      *plRoundValue  = *(zPLONG) (szBlob + 8 + (3 * sizeof( zLONG )) );
      if ( ulLth == zsizeof( szBlob ) )
      {
         *plBackgroundColor = *(zPLONG) (szBlob + 8 + (4 * sizeof( zLONG )) );
         *plFlags = *(zPLONG) (szBlob + 8 + (5 * sizeof( zLONG )) );
      }
      else
      {
         *plBackgroundColor = 0;
         *plFlags = 0;
      }
   }
   else
   {
      *pbRectangle   = TRUE;
      *pbRoundRect   = FALSE;
      *pbEllipse     = FALSE;
      *pbDiamond     = FALSE;
      *pbHorizontal  = FALSE;
      *pbVertical    = FALSE;
      *pbDiagDescend = FALSE;
      *pbDiagAscend  = FALSE;
      *plPenWidth    = 0;
      *plPenType     = 0;
      *plPenColor    = 0;
      *plRoundValue  = 0;
      *plBackgroundColor = 0;
      *plFlags = 0;
   }

   return( (zSHORT) ulLth );
}

zOPER_EXPORT zSHORT OPERATION
SP_SetShapeCtrlBOI( zVIEW   vCtrl,
                    zSHORT  bRectangle,
                    zSHORT  bRoundRect,
                    zSHORT  bEllipse,
                    zSHORT  bDiamond,
                    zSHORT  bHorizontal,
                    zSHORT  bVertical,
                    zSHORT  bDiagDescend,
                    zSHORT  bDiagAscend,
                    zLONG   lPenWidth,
                    zLONG   lPenType,
                    zLONG   lPenColor,
                    zLONG   lRoundValue,
                    zLONG   lBackgroundColor,
                    zLONG   lFlags )
{
   zCHAR   szBlob[ 8 + 6 * sizeof( zLONG ) ];
   zULONG  ulLth;

   if ( bRoundRect || bEllipse || bDiamond ||
        bHorizontal || bVertical || bDiagDescend || bDiagAscend ||
        lPenWidth || lPenType || lPenColor || lRoundValue ||
        lBackgroundColor || lFlags )
   {
      zPLONG lpLong;

      ulLth = zsizeof( szBlob );

      szBlob[ 0 ] = (char) bRectangle;
      szBlob[ 1 ] = (char) bRoundRect;
      szBlob[ 2 ] = (char) bEllipse;
      szBlob[ 3 ] = (char) bDiamond;
      szBlob[ 4 ] = (char) bHorizontal;
      szBlob[ 5 ] = (char) bVertical;
      szBlob[ 6 ] = (char) bDiagDescend;
      szBlob[ 7 ] = (char) bDiagAscend;

      lpLong = (zPLONG) (szBlob + 8 );
      *lpLong = lPenWidth;
      lpLong = (zPLONG) (szBlob + 8 + sizeof( zLONG ) );
      *lpLong = lPenType;
      lpLong = (zPLONG) (szBlob + 8 + (2 * sizeof( zLONG )) );
      *lpLong = lPenColor;
      lpLong = (zPLONG) (szBlob + 8 + (3 * sizeof( zLONG )) );
      *lpLong = lRoundValue;
      lpLong = (zPLONG) (szBlob + 8 + (4 * sizeof( zLONG )) );
      *lpLong = lBackgroundColor;
      lpLong = (zPLONG) (szBlob + 8 + (5 * sizeof( zLONG )) );
      *lpLong = lFlags;
   }
   else
   {
      ulLth = 0;
   }

   SetAttributeFromBlob( vCtrl, "Control", "CtrlBOI", szBlob, ulLth );
   return( (zSHORT) ulLth );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//./ ADD NAME=GetActiveX_WrapperInstance
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION:  GetActiveX_WrapperInstance
//                                              04/26/99 Modified: 04/26/99
//
//  PURPOSE:    This operation provides access to the requested ActiveX
//              control's default dispatch interface.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control for which to get the dispatch
//                            interface.
//              cpcCtrlTag  - The unique name of the control.
//
//  RETURNS:    0 - Object not located
//              otherwise - pointer to Editor object
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 2
zOPER_EXPORT COleDispatchDriver * OPERATION
GetActiveX_WrapperInstance( zVIEW    vSubtask,
                            zCPCHAR  cpcCtrlTag )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( pzma->GetWrapperInstance( ) );
   }

   return( 0 );
}

//./ ADD NAME=GetActiveX_ParameterCount
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION:  GetActiveX_ParameterCount
//                                              04/26/99 Modified: 04/26/99
//
//  PURPOSE:    This operation provides access to the requested ActiveX
//              control's parameters for the specified event.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control for which to get the dispatch
//                            interface.
//              cpcCtrlTag  - The unique name of the control.
//              lEventID    - DispId of the event
//
//  RETURNS:    >= 0 - number of parameters for the specified event
//              -1 - Error locating window or control
//              -2 - Invalid control type
//              -3 - Event not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 4
zOPER_EXPORT zSHORT OPERATION
GetActiveX_ParameterCount( zVIEW    vSubtask,
                           zCPCHAR  cpcCtrlTag,
                           zLONG    lEventID )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZActiveX *pAX = DYNAMIC_DOWNCAST( ZActiveX, pzma->m_pCtrl );
      if ( pAX )
      {
         return( pAX->GetEventParmCnt( (DISPID) lEventID ) );
      }

      TraceLineS( "drvr - Invalid control type for GetActiveX_ParameterCount ",
                  cpcCtrlTag );

      return( -2 );
   }

   return( -1 );
}

//./ ADD NAME=GetActiveX_ParameterValue
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION:  GetActiveX_ParameterValue
//                                               04/26/99 Modified: 04/26/99
//
//  PURPOSE:    This operation provides access to the requested ActiveX
//              control's parameters for the specified event.
//
//  PARAMETERS: vSubtask    - The subtask view for the window containing
//                            the control for which to get the dispatch
//                            interface.
//              cpcCtrlTag  - The unique name of the control.
//              lEventID    - DispId of the event
//              lParmID     - Parameter id for the event (consecutive integers
//                            starting at 1)
//              pchValue    - pointer to return buffer for string value
//              nMaxLth     - maximum length of string value (including null
//                            terminator)
//
//  RETURNS:    >= 0 - length of parameter returned
//              -1 - Error locating window or control
//              -2 - Invalid control type
//              -3 - Event not located
//              -4 - Parameter not located
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
GetActiveX_ParameterValue( zVIEW    vSubtask,
                           zCPCHAR  cpcCtrlTag,
                           zLONG    lEventID,
                           zLONG    lParmID,
                           zPCHAR   pchValue,
                           zSHORT   nMaxLth )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      ZActiveX *pAX = DYNAMIC_DOWNCAST( ZActiveX, pzma->m_pCtrl );
      if ( pAX )
      {
         return( pAX->GetEventParm( (DISPID) lEventID, lParmID,
                                    pchValue, nMaxLth ) );
      }

      TraceLineS( "drvr - Invalid control type for GetActiveX_ParameterValue ",
                  cpcCtrlTag );

      return( -2 );
   }

   return( -1 );
}

//./ ADD NAME=SetCtrlMapping
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: SetCtrlMapping
//                                                    2001.07.16
//
//  PURPOSE:    This operation sets a new Mapping for a Control
//
//  PARAMETERS: vSubtask         - The subtask view for the window containing
//                                 the control to be mapped.
//              cpcCtrlTag       - The unique name of the control.
//              cpcViewName      - new View Name to be mapped
//              cpcEntityName    - new Entity Name to be mapped
//              cpcAttributeName - new Attribute Name to be mapped
//              cpcContextName   - new Context Name to be mapped
//              lColumn          - Column for which the new mapping is to be
//                                 applied (ignored for single map ctrls).
//
//  RETURNS:    0 - New Mapping set successfully
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zSHORT OPERATION
SetCtrlMapping( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zCPCHAR cpcViewName,
                zCPCHAR cpcEntityName,
                zCPCHAR cpcAttributeName,
                zCPCHAR cpcContextName,
                zLONG   lColumn )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( pzma->SetCtrlMapping( cpcViewName, cpcEntityName,
                                    cpcAttributeName, cpcContextName,
                                    lColumn ) );
   }

   return( -1 );
}

//./ ADD NAME=GetCtrlMapping
// Source Module=zdctlo.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: GetCtrlMapping
//                                                    2001.07.16
//
//  PURPOSE:    This operation gets the current Mapping for a Control
//
//  PARAMETERS: vSubtask         - The subtask view for the window containing
//                                 the control to be mapped.
//              cpcCtrlTag       - The unique name of the control.
//              pchViewName      - Mapping View Name
//              pchEntityName    - Mapping Entity Name
//              pchAttributeName - Mapping Attribute Name
//              pchContextName   - Mapping Context Name
//              lColumn          - Column for which the new mapping is to be
//                                 retrieved (ignored for single map ctrls).
//
//  RETURNS:    0 - Mapping returned successfully
//              1 - Invalid request (e.g.  outside range of valid columns)
//             -1 - Error locating control
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 9
zOPER_EXPORT zSHORT OPERATION
GetCtrlMapping( zVIEW   vSubtask,
                zCPCHAR cpcCtrlTag,
                zPCHAR  pchViewName,
                zPCHAR  pchEntityName,
                zPCHAR  pchAttributeName,
                zPCHAR  pchContextName,
                zLONG   lColumn )
{
   ZSubtask *pZSubtask;
   ZMapAct  *pzma;

   if ( GetWindowAndCtrl( &pZSubtask, &pzma, vSubtask, cpcCtrlTag ) == 0 )
   {
      return( pzma->GetCtrlMapping( pchViewName, pchEntityName,
                                    pchAttributeName, pchContextName,
                                    lColumn ) );
   }

   return( -1 );
}

/////////////////////////////////////////////////////////////////////////////
//
//  OPERATION:  SplitCtrlBOI_BMP
//
//  PURPOSE:    For the dialog PWD (in LOD "TZWDLGSO"):
//              Split the BLOB CtrlBOI for an Control with BMP or ICON
//              resources to resource names/IDs
//
//  PARAMETERS: pchCtrlBOI - pointer to BLOB
//              lCtrlBOI_Size - size of BLOB
//              pBMP - splitted information
//
//  RETURNS:    -
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT void  OPERATION
SplitCtrlBOI_BMP( zPCHAR       pchCtrlBOI,
                  zLONG        lCtrlBOI_Size,
                  zCTRLBOI_BMP *pBMP )
{
   zPCHAR pch0;
   zPCHAR pch1;
   zPCHAR pchEnd;

   // initialize return struct
   zmemset( pBMP, 0, sizeof( zCTRLBOI_BMP ) );

   if ( lCtrlBOI_Size && pchCtrlBOI )
   {
      pch0 = pchCtrlBOI;
      pchEnd = pch0 + lCtrlBOI_Size;

      // get DLL Name
      strcpy_s( pBMP->szDLLName, zsizeof( pBMP->szDLLName ), pch0 );

      // get BMP up or Icon
      pch0 += zstrlen( pch0 ) + 1;
      if ( pch0 != pchEnd )
      {
         pch1 = pch0;
         while ( *pch1 != ';' && *pch1 )
            pch1++;

         // does the resource ID exist?
         if ( *pch1 == ';' )
         {
            // get it
            pBMP->lBMPUpOrIconID = zatol( pch0 );
            strcpy_s( pBMP->szBMPUpOrIconName, zsizeof( pBMP->szBMPUpOrIconName ), ++pch1 );
         }
         else
         {
            strcpy_s( pBMP->szBMPUpOrIconName, zsizeof( pBMP->szBMPUpOrIconName ), pch0 );
         }

         // get BMP down
         pch0 += zstrlen( pch0 ) + 1;
         if ( pch0 != pchEnd )
         {
            pch1 = pch0;
            while ( *pch1 != ';' && *pch1 )
               pch1++;

            // does the resource ID exist?
            if ( *pch1 == ';' )
            {
               // get it
               pBMP->lBMPDownID = zatol( pch0 );
               strcpy_s( pBMP->szBMPDownName, zsizeof( pBMP->szBMPDownName ), ++pch1 );
            }
            else
            {
               strcpy_s( pBMP->szBMPDownName, zsizeof( pBMP->szBMPDownName ), pch0 );
            }

            // get BMP focus
            pch0 += zstrlen( pch0 ) + 1;
            if ( pch0 != pchEnd )
            {
               pch1 = pch0;
               while ( *pch1 != ';' && *pch1 )
                  pch1++;

               // does the resource ID exist?
               if ( *pch1 == ';' )
               {
                  // get it
                  pBMP->lBMPFocusID = zatol( pch0 );
                  strcpy_s( pBMP->szBMPFocusName, zsizeof( pBMP->szBMPFocusName ), ++pch1 );
               }
               else
               {
                  strcpy_s( pBMP->szBMPFocusName, zsizeof( pBMP->szBMPFocusName ), pch0 );
               }

               // get BMP disabled
               pch0 += zstrlen( pch0 ) + 1;
               if ( pch0 != pchEnd )
               {
                  pch1 = pch0;
                  while ( *pch1 != ';' && *pch1 )
                     pch1++;

                  // does the resource ID exist?
                  if ( *pch1 == ';' )
                  {
                     // get it
                     pBMP->lBMPDisabledID = zatol( pch0 );
                     strcpy_s( pBMP->szBMPDisabledName, zsizeof( pBMP->szBMPDisabledName ), ++pch1 );
                  }
                  else
                  {
                     strcpy_s( pBMP->szBMPDisabledName, zsizeof( pBMP->szBMPDisabledName ), pch0 );
                  }
               }
            }
         }
      }
   }

} // SplitCtrlBOI_BMP
