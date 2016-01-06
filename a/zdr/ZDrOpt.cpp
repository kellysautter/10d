/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdropt.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of option processing functions.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 1999.09.15    DKS   Z10   ActiveX
//    Added Mapping List.
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


// Comparison function required to order objects added to the list.  In
// this case it just compares the names embedded in the ZOption type.
zSHORT
fnCompareOptionNodes( const ZOption *pZOption1,
                      const ZOption *pZOption2 )
{
   return( pZOption1->m_pzsTag->Compare( *(pZOption2->m_pzsTag) ) );
}

// Comparison function required to seek for an object in the list.  In
// this case it just compares the names embedded in the ZOption type.
zSHORT
fnSeekOptionNode( const CString *m_pzsTag,
                  const ZOption *pZOption )
{
   return( m_pzsTag->Compare( *(pZOption->m_pzsTag) ) );
}

// Function to process the options in order.
void
fnOrderedOptions( const ZOption *pZOption,
                  WPARAM wParam,
                  LPARAM lParam )
{
   TraceLineI( *(pZOption->m_pzsTag), pZOption->m_uOptionID );
}

// Function to delete the option data.
void
fnDeleteOptionObject( ZOption *pZOption,
                      WPARAM wParam,
                      LPARAM lParam )
{
   mDeleteInit( pZOption );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZMapping::ZMapping( zKZWDLGXO_CtrlMap_DEF CtrlMapDef ) :
          m_csTag( CtrlMapDef.Tag ),
          m_csVName( CtrlMapDef.VN ),
          m_csEName( CtrlMapDef.EN ),
          m_csAName( CtrlMapDef.AN ),
          m_csContext( CtrlMapDef.Context )
{
   m_chType = CtrlMapDef.Type[ 0 ];
}

ZMapping::ZMapping( zCPCHAR cpcTag ) :
          m_csTag( cpcTag ),
          m_csVName( "" ),
          m_csEName( "" ),
          m_csAName( "" ),
          m_csContext( "" )
{
   m_chType = 0;
}

ZMapping::~ZMapping( )
{
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZBarTip::ZBarTip( HWND hWnd, zLONG lFirstId, zLONG lLastId )
{
   ASSERT( hWnd );
   ASSERT( lFirstId <= lLastId );
   m_hWnd = hWnd;
   m_lFirstId = lFirstId;
   m_lLastId = lLastId;
}

void
ZSubtask::AddBarTip( HWND hWnd, zLONG lFirstId, zLONG lLastId )
{
   ZBarTip *pBarTip;
   zSHORT nPos = m_nBarTipCnt;

   while ( nPos )
   {
      nPos--;
      pBarTip = m_BarTipList[ nPos ];
      if ( pBarTip->m_hWnd == hWnd )
      {
         pBarTip->m_lFirstId = lFirstId;
         pBarTip->m_lLastId = lLastId;
         return;
      }
      else
      if ( mIs_hWnd( pBarTip->m_hWnd ) == FALSE )
      {
         m_BarTipList.RemoveAt( nPos );
         m_nBarTipCnt--;
         nPos = m_nBarTipCnt;
         mDeleteInit( pBarTip );
      }
   }

   pBarTip = new ZBarTip( hWnd, lFirstId, lLastId );
   m_BarTipList.SetAtGrow( m_nBarTipCnt++, pBarTip );
}

HWND
ZSubtask::LocateBarTip( zLONG lId )
{
   ZBarTip *pBarTip;
   zSHORT nPos = m_nBarTipCnt;

   while ( nPos-- )
   {
      pBarTip = m_BarTipList[ nPos ];
      if ( lId >= pBarTip->m_lFirstId && lId <= pBarTip->m_lLastId )
      {
         return( pBarTip->m_hWnd );
      }
   }

   return( 0 );
}

void
ZSubtask::CleanupBarTips( )
{
   ZBarTip *pBarTip;
   while ( m_nBarTipCnt > 0 )
   {
      m_nBarTipCnt--;
      pBarTip = m_BarTipList[ m_nBarTipCnt ];
      mDeleteInit( pBarTip );
   }
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZAction::ZAction( zLONG lActType, zCPCHAR cpcAction,
                  zCPCHAR cpcActDlg, zCPCHAR cpcActWnd )
{
   m_lActType = lActType; // type of action requested in code for this window

   // data for requested action (e.g.  Y, 1, 2 ...)
   if ( cpcAction && cpcAction[ 0 ] )
      strcpy_s( m_szAction, sizeof( m_szAction ), cpcAction );
   else
      m_szAction[ 0 ] = 0;

   // name of dialog for the action type
   if ( cpcActDlg && cpcActDlg[ 0 ] )
      strcpy_s( m_szActDlg, sizeof( m_szActDlg ), cpcActDlg );
   else
      m_szActDlg[ 0 ] = 0;

   // name of window for the action type
   if ( cpcActWnd && cpcActWnd[ 0 ] )
      strcpy_s( m_szActWnd, sizeof( m_szActWnd ), cpcActWnd );
   else
      m_szActWnd[ 0 ] = 0;

   m_pNextAction = 0;
}

ZAction::~ZAction( )
{
// mDeleteInit( m_pNextAction );  these have to be actively deleted
}
