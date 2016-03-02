/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2010 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrapp.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of zdrapp initialization.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.07    DKS   WEB
//    Added ResetSubtasks in support of Delta Data.
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 1999.09.22    DKS   Z10    QS999
//    Started memory leak analyis.
//
// 1999.09.15    DKS   Z10    ActiveX
//    Added capability to retrieve parameters for ActiveX events.
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"

#define zGLOBAL_DATA
#include "ZDrGbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// Initialization of MFC Extension DLL

#include "afxdllx.h"    // standard MFC Extension DLL routines

// static CDynLinkLibrary *g_ZDrAppDynLinkLibrary = 0;
static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };

zPCHAR g_MemHead = 0;

extern "C" int APIENTRY
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )
{
   // Remove this if you use lpReserved.
   UNREFERENCED_PARAMETER( lpReserved );

   if ( dwReason == DLL_PROCESS_ATTACH )
   {
   // TRACE0( "ZDRAPP.DLL Initializing!\n" );
      g_hInstanceDLL = hInstance;

      // Extension DLL one-time initialization - do not allocate memory here,
      // use the TRACE or ASSERT macros or call MessageBox
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )
         return( 0 );

      // Other initialization could be done here, as long as
      // it doesn't result in direct or indirect calls to AfxGetApp.
      // This extension DLL needs to add doc templates to the
      // app object, so it waits for the application to boot
      // and call an initialization function (see below).

      // This allows for greater flexibility later in development.
   }
   else
   if ( dwReason == DLL_PROCESS_DETACH )
   {
   // TRACE0( "ZDRAPP.DLL Terminating!\n" );
      AfxTermExtensionModule( extensionDLL );
   }

   return( 1 );   // ok
}

/////////////////////////////////////////////////////////////////////////////
// Exported DLL initialization is run in context of running application
//
// If you are going to use your extension DLL from one or more regular DLLs,
// you must export an initialization function that creates a CDynLinkLibrary
// object.  That function must be called from each of the regular DLLs that
// use the extension DLL.  An appropriate place to call this initialization
// function is in the InitInstance member function of the regular DLL's
// CWinApp-derived object before using any of the extension DLL's exported
// classes or functions.
//
extern "C" void WINAPI
ZeidonInitZDRAPP( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                  zLONG lMapDlgCharX, zLONG lMapDlgCharY )
{
// TraceLineS( "ZDRAPP.DLL InitZDRAPP!!!", "" );
   // Add other initialization here

   g_vRegister = vSubtask;

   // Initialize globals
   if ( g_pSubtaskList == 0 )
   {
      zVIEW vZDrSystem;

      if ( GetViewByName( &vZDrSystem, "__ZDrSystem",
                          vSubtask, zLEVEL_SYSTEM ) > 0 )
      {
         GetAddrForAttribute( (zPCHAR *) &g_pSubtaskList, vZDrSystem,
                              "System", "Subtasks" );
#ifdef zREMOTE_SERVER
         GetAddrForAttribute( (zPCHAR *) &g_pSocketList, vZDrSystem,
                              "System", "Sockets" );
#endif
      }
   }

   g_lDlgUnitCharX = lDlgUnitCharX;
   g_lDlgUnitCharY = lDlgUnitCharY;
   g_lMapDlgCharX = lMapDlgCharX;
   g_lMapDlgCharY = lMapDlgCharY;
   g_lDblClkX = GetSystemMetrics( SM_CXDOUBLECLK );
   g_lDblClkY = GetSystemMetrics( SM_CYDOUBLECLK );
}

#if 1    // until I figure out templates better ... just hardcode list

/////////////////////////////////////////////////////////////////////////////
// class ZPtrArray : pointer array class which grows dynamically as
//                   necessary
//

ZPtrArray::ZPtrArray( zSHORT nInitialPointers,
                      zSHORT nIncrementPointers,
                      ZSubtask **pBufferArray )
{
   ASSERT( nInitialPointers > 0 );
   ASSERT( nIncrementPointers >= 0 );
   ASSERT( nInitialPointers * sizeof( zPVOID ) < 64000 );

   // if the buffer is supplied, it cannot be incremented
   ASSERT( nIncrementPointers == 0 || pBufferArray == 0 );

   m_lCnt = 0;                // current count of pointers in the array
   m_lMax = nInitialPointers; // maximum pointers allocated in the array
   m_nIncr = nIncrementPointers; // array allocation expansion increment
   if ( pBufferArray == 0 )
   {
      m_bLocalBuffer = TRUE;
      m_pArrayBuffer = (ZSubtask **) new char[ m_lMax * sizeof( ZSubtask * ) ];
   }
   else
   {
      m_bLocalBuffer = FALSE;
      m_pArrayBuffer = pBufferArray;
   }
}

ZPtrArray::~ZPtrArray( )
{
   m_lCnt = 0;
   m_lMax = 0;
   if ( m_bLocalBuffer )
      mDeleteInitA( m_pArrayBuffer );
}

// Returns index where inserted (-1 if error).
zLONG
ZPtrArray::Insert( ZSubtask *pZSubtask )
{
// TraceLineX( "ZPtrArray::Insert ", (zLONG) pZSubtask );
   ASSERT( pZSubtask );
   ASSERT( Find( pZSubtask ) == -1 );

   if ( m_lCnt >= m_lMax )
   {
      if ( m_bLocalBuffer == FALSE )
         return( -1 );

      TraceLineX( "ZPtrArray::Insert expanding buffer for: ", (zLONG) pZSubtask );
      ASSERT( (m_lMax + m_nIncr) * sizeof( ZSubtask * ) < 64000 );
      zPCHAR pTemp = new char[ (m_lMax + m_nIncr) * sizeof( ZSubtask * ) ];
      if ( pTemp == 0 )
         return( -1 );

      zmemcpy( pTemp, m_pArrayBuffer, m_lMax * sizeof( ZSubtask * ) );
      mDeleteInitA( m_pArrayBuffer );
      m_pArrayBuffer = (ZSubtask **) pTemp;
      m_lMax += m_nIncr;
   }

   m_pArrayBuffer[ m_lCnt ] = pZSubtask;
   m_lCnt++;

// TraceLineX( "ZPtrArray::Attach for: ", (zLONG) pZSubtask );
// zLONG lPos = m_lCnt;
// while ( lPos-- )
// {
//    TraceLineX( "ZPtrArray::Attach list ",
//                (zLONG) m_pArrayBuffer[ lPos ] );
// }

   return( m_lCnt );
}

// Returns index where located (-1 if error).
zLONG
ZPtrArray::Find( ZSubtask *pZSubtask )
{
   zLONG lPos = m_lCnt;
   while ( lPos-- )
   {
      if ( pZSubtask == m_pArrayBuffer[ lPos ] )
         return( lPos );
   }

   return( -1 );
}

// Returns index where found (-1 if error).
zLONG
ZPtrArray::Detach( ZSubtask *pZSubtask )
{
// TraceLineX( "ZPtrArray::Detach ", (zLONG) pZSubtask );
   zLONG lPos = Find( pZSubtask );
   if ( lPos == -1 )
   {
      TraceLineX( "ZPtrArray::Detach did NOT find ", (zLONG) pZSubtask );
      lPos = m_lCnt;
      while ( lPos-- )
      {
         TraceLineX( "ZPtrArray::Detach list ",
                     (zLONG) m_pArrayBuffer[ lPos ] );
      }
   }
   else
   {
      m_lCnt--;

      // Maintain integrity of array by eliminating detached entry
      zmemcpy( m_pArrayBuffer + lPos,      // detached entry position
               m_pArrayBuffer + lPos + 1,  // next entry position
               (m_lCnt - lPos) * sizeof( ZSubtask * ) );

//    TraceLineX( "ZPtrArray::Detach FOUND ", (zLONG) pZSubtask );
//    lPos = m_lCnt;
//    while ( lPos-- )
//    {
//       TraceLineX( "ZPtrArray::Detach resultant list ",
//                   (zLONG) m_pArrayBuffer[ lPos ] );
//    }
   }

   return( lPos );
}

// Returns current count of pointers in array.
zLONG
ZPtrArray::GetSize( )
{
   return( m_lCnt );
}

// Returns zero on error.
ZSubtask *
ZPtrArray::operator []( zLONG lIdx ) const
{
   ASSERT( lIdx >= 0 && lIdx < m_lCnt );
   return( m_pArrayBuffer[ lIdx ] );
}

#else

/////////////////////////////////////////////////////////////////////////////
// class ZPtrArray : pointer array class which grows dynamically as
//                   necessary
//

template <class T>
ZPtrArray< T >::ZPtrArray( zSHORT nInitialPointers,
                           zSHORT nIncrementPointers,
                           T *pBufferArray )
{
   ASSERT( nInitialPointers > 0 );
   ASSERT( nIncrementPointers >= 0 );
   ASSERT( nInitialPointers * sizeof( zPVOID ) < 64000 );

   // if the buffer is supplied, it cannot be incremented
   ASSERT( nIncrementPointers == 0 || pBufferArray == 0 );

   m_lCnt = 0;                // current count of pointers in the array
   m_lMax = nInitialPointers; // maximum pointers allocated in the array
   m_nIncr = nIncrementPointers; // array allocation expansion increment
   if ( pBufferArray == 0 )
   {
      m_bLocalBuffer = TRUE;
      m_pArrayBuffer = (T *) new char[ m_lMax * zsizeof( T ) ];
   }
   else
   {
      m_bLocalBuffer = FALSE;
      m_pArrayBuffer = pBufferArray;
   }
}

template <class T>
ZPtrArray< T >::~ZPtrArray( )
{
   m_lCnt = 0;
   m_lMax = 0;
   if ( m_bLocalBuffer )
      mDeleteInitA( m_pArrayBuffer );
}

// Returns index where inserted (-1 if error).
template <class T>
zSHORT
ZPtrArray< T >::Insert( T pT )
{
   ASSERT( pT );
   if ( m_lCnt >= m_lMax )
   {
      if ( m_bLocalBuffer == FALSE )
         return( -1 );

      ASSERT( (m_lMax + m_nIncr) * zsizeof( T ) < 64000 );
      T *pTemp = (T *) new char[ (m_lMax + m_nIncr) * zsizeof( T ) ];
      if ( pTemp == 0 )
         return( -1 );

      zmemcpy( pTemp, m_pArrayBuffer, m_lMax * zsizeof( T ) );
      mDeleteInitA( m_pArrayBuffer );
      m_pArrayBuffer = pTemp;
      m_lMax += m_nIncr;
   }

   m_pArrayBuffer[ m_lCnt ] = pT;
   m_lCnt++;
   return( m_nCnt );
}

// Returns index where located (-1 if error).
template <class T>
zSHORT
ZPtrArray< T >::Find( T pT )
{
   zLONG lPos = m_lCnt;
   while ( lPos-- )
   {
      if ( pT == m_pArrayBuffer[ lPos ] )
         return( lPos );
   }

   return( -1 );
}

// Returns index where found (-1 if error).
template <class T>
zLONG
ZPtrArray< T >::Detach( T pT )
{
   zLONG lPos = Find( pT );
   if ( lPos != -1 )
   {
      m_lCnt--;
      zmemcpy( m_pArrayBuffer + lPos,
               m_pArrayBuffer + lPos + 1,
               (m_nCnt - lPos) * zsizeof( T ) );
   }

   return( lPos );
}

// Returns current count of pointers in array.
template <class T>
zLONG
ZPtrArray< T >::GetSize( )
{
   return( m_lCnt );
}

// Returns zero on error.
template <class T>
T
ZPtrArray< T >::operator []( zLONG lIdx ) const
{
   ASSERT( lIdx >= 0 && lIdx < m_lCnt );
   return( m_pArrayBuffer[ lIdx ] );
}

#endif

/////////////////////////////////////////////////////////////////////////////
// class ZNumberList : simple single-linked list of numbers
//
ZNumberItem::ZNumberItem( zLONG lNumber, zLONG lFlag )
{
// TraceLineS( "Add NumberItem: ", m_csNumber );
   m_lFlag = lFlag;
   m_lNumber = lNumber;
   m_pNextNumber = 0;
}

ZNumberItem::~ZNumberItem( )
{
// TraceLineS( "Delete NumberItem: ", m_csNumber );
   mDeleteInit( m_pNextNumber );
}

ZNumberList::ZNumberList( zBOOL bUnique )
{
   m_pHeadNumber = 0;
   m_pTailNumber = 0;
   m_bUnique = bUnique;
}

ZNumberList::~ZNumberList( )
{
// TraceLineS( "Delete NumberList", "" );
   ZNumberItem *pItem = m_pHeadNumber;
   m_pHeadNumber = 0;
   ZNumberItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextNumber;
      pItem->m_pNextNumber = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZNumberItem *
ZNumberList::AddHead( zLONG lNumber, zLONG lFlag )
{
   if ( m_bUnique && Find( lNumber ) )
      return( 0 );

   if ( m_pHeadNumber == 0 )
   {
      m_pHeadNumber = new ZNumberItem( lNumber, lFlag );
      m_pTailNumber = m_pHeadNumber;
   }
   else
   {
      ZNumberItem *pItem = new ZNumberItem( lNumber, lFlag );
      pItem->m_pNextNumber = m_pHeadNumber;
      m_pHeadNumber = pItem;
   }

   return( m_pHeadNumber );
}

// Return pointer if added ... 0 if already in the list.
ZNumberItem *
ZNumberList::AddTail( zLONG lNumber, zLONG lFlag )
{
   if ( m_bUnique && Find( lNumber ) )
      return( 0 );

   if ( m_pHeadNumber == 0 )
   {
      m_pHeadNumber = new ZNumberItem( lNumber, lFlag );
      m_pTailNumber = m_pHeadNumber;
      return( m_pHeadNumber );
   }
   else
   {
      m_pTailNumber->m_pNextNumber = new ZNumberItem( lNumber, lFlag );
      m_pTailNumber = m_pTailNumber->m_pNextNumber;
      return( m_pTailNumber );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZNumberItem *
ZNumberList::Find( zLONG lNumber )
{
   ZNumberItem *pItem = m_pHeadNumber;
   while ( pItem )
   {
      if ( lNumber == pItem->m_lNumber )
         return( pItem );

      pItem = pItem->m_pNextNumber;
   }

   return( 0 );
}

// Return TRUE if found ... FALSE if not found in the list.
ZNumberItem *
ZNumberList::Remove( zLONG lNumber )
{
   ZNumberItem *pItem = m_pHeadNumber;
   ZNumberItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( lNumber == pItem->m_lNumber )
      {
         if ( pItemPrev )
         {
            pItemPrev->m_pNextNumber = pItem->m_pNextNumber;
            if ( pItemPrev->m_pNextNumber == 0 )
               m_pTailNumber = pItemPrev;
         }
         else
         {
            m_pHeadNumber = pItem->m_pNextNumber;
            if ( m_pHeadNumber == 0 )
               m_pTailNumber = 0;
         }

         pItem->m_pNextNumber = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextNumber;
   }

   return( 0 );
}

// Return ptr if found for move ... 0 otw.
ZNumberItem *
ZNumberList::MoveHead( zLONG lNumber )
{
   if ( m_pHeadNumber && lNumber == m_pHeadNumber->m_lNumber )
      return( m_pHeadNumber );

   ZNumberItem *pItem = m_pHeadNumber->m_pNextNumber;
   ZNumberItem *pItemPrev = m_pHeadNumber;
   while ( pItem )
   {
      if ( lNumber == pItem->m_lNumber )
      {
         pItemPrev->m_pNextNumber = pItem->m_pNextNumber;
         if ( pItemPrev->m_pNextNumber == 0 )
            m_pTailNumber = pItemPrev;

         pItem->m_pNextNumber = m_pHeadNumber;
         m_pHeadNumber = pItem;

         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextNumber;
   }

   return( 0 );
}

// Return pointer if dropped ... 0 if not in the list.
ZNumberItem *
ZNumberList::DropHead( )
{
   ZNumberItem *pItem = m_pHeadNumber;
   if ( m_pHeadNumber )
   {
      m_pHeadNumber = m_pHeadNumber->m_pNextNumber;
      pItem->m_pNextNumber = 0;
      if ( m_pHeadNumber == 0 )
         m_pTailNumber = 0;
   }

   return( pItem );
}

// Return pointer if dropped ... 0 if not in the list.
ZNumberItem *
ZNumberList::DropTail( )
{
   if ( m_pHeadNumber )
   {
      ZNumberItem *pItem = m_pHeadNumber;
      ZNumberItem *pItemPrev = 0;
      while ( pItem->m_pNextNumber )
      {
         pItemPrev = pItem;
         pItem = pItem->m_pNextNumber;
      }

      if ( pItemPrev )
      {
         pItemPrev->m_pNextNumber = 0;
         m_pTailNumber = pItemPrev;
      }
      else
      if ( pItem == m_pHeadNumber )
      {
         m_pHeadNumber = 0;
         m_pTailNumber = 0;
      }

      return( pItem );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// class ZNameList : simple single-linked list of names
//
ZNameItem::ZNameItem( zCPCHAR cpcName, zLONG lFlag ) :
           m_csName( cpcName )
{
// TraceLineS( "Add NameItem: ", m_csName );
   m_lFlag = lFlag;
   m_pNextName = 0;
}

ZNameItem::~ZNameItem( )
{
// TraceLineS( "Delete NameItem: ", m_csName );
   mDeleteInit( m_pNextName );
}

ZNameList::ZNameList( zBOOL bUnique )
{
   m_pHeadName = 0;
   m_pTailName = 0;
   m_bUnique = bUnique;
}

ZNameList::~ZNameList( )
{
// TraceLineS( "Delete NameList", "" );
   ZNameItem *pItem = m_pHeadName;
   m_pHeadName = 0;
   ZNameItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextName;
      pItem->m_pNextName = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZNameItem *
ZNameList::AddHead( zCPCHAR cpcName, zLONG lFlag )
{
   ASSERT( cpcName );

   if ( m_bUnique && Find( cpcName ) )
      return( 0 );

   if ( m_pHeadName == 0 )
   {
      m_pHeadName = new ZNameItem( cpcName, lFlag );
      m_pTailName = m_pHeadName;
   }
   else
   {
      ZNameItem *pItem = new ZNameItem( cpcName, lFlag );
      pItem->m_pNextName = m_pHeadName;
      m_pHeadName = pItem;
   }

   return( m_pHeadName );
}

// Return pointer if added ... 0 if already in the list.
ZNameItem *
ZNameList::AddTail( zCPCHAR cpcName, zLONG lFlag )
{
   ASSERT( cpcName );

   if ( m_bUnique && Find( cpcName ) )
      return( 0 );

   if ( m_pHeadName == 0 )
   {
      m_pHeadName = new ZNameItem( cpcName, lFlag );
      m_pTailName = m_pHeadName;
      return( m_pHeadName );
   }
   else
   {
      m_pTailName->m_pNextName = new ZNameItem( cpcName, lFlag );
      m_pTailName = m_pTailName->m_pNextName;
      return( m_pTailName );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZNameItem *
ZNameList::Find( zCPCHAR cpcName )
{
   ASSERT( cpcName );

   ZNameItem *pItem = m_pHeadName;
   while ( pItem )
   {
      if ( zstrcmp( cpcName, pItem->m_csName.GetString() ) == 0 )
      {
         return( pItem );
      }

      pItem = pItem->m_pNextName;
   }

   return( 0 );
}

// Return pointer if found ... 0 if not found in the list.
ZNameItem *
ZNameList::Remove( zCPCHAR cpcName )
{
   ASSERT( cpcName );

   ZNameItem *pItem = m_pHeadName;
   ZNameItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( zstrcmp( cpcName, pItem->m_csName.GetString() ) == 0 )
      {
         if ( pItemPrev )
         {
            pItemPrev->m_pNextName = pItem->m_pNextName;
            if ( pItemPrev->m_pNextName == 0 )
               m_pTailName = pItemPrev;
         }
         else
         {
            m_pHeadName = pItem->m_pNextName;
            if ( m_pHeadName == 0 )
               m_pTailName = 0;
         }

         pItem->m_pNextName = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextName;
   }

   return( 0 );
}

// Return ptr if found for move ... 0 otw.
ZNameItem *
ZNameList::MoveHead( zCPCHAR cpcName )
{
   if ( m_pHeadName && zstrcmp( cpcName, m_pHeadName->m_csName.GetString() ) == 0 )
      return( m_pHeadName );

   ZNameItem *pItem = m_pHeadName->m_pNextName;
   ZNameItem *pItemPrev = m_pHeadName;
   while ( pItem )
   {
      if ( zstrcmp( cpcName, pItem->m_csName.GetString() ) == 0 )
      {
         pItemPrev->m_pNextName = pItem->m_pNextName;
         if ( pItemPrev->m_pNextName == 0 )
            m_pTailName = pItemPrev;

         pItem->m_pNextName = m_pHeadName;
         m_pHeadName = pItem;

         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextName;
   }

   return( 0 );
}

// Return pointer if dropped ... 0 if not in the list.
ZNameItem *
ZNameList::DropHead( )
{
   ZNameItem *pItem = m_pHeadName;
   if ( m_pHeadName )
   {
      m_pHeadName = m_pHeadName->m_pNextName;
      pItem->m_pNextName = 0;
      if ( m_pHeadName == 0 )
         m_pTailName = 0;
   }

   return( pItem );
}

// Return pointer if dropped ... 0 if not in the list.
ZNameItem *
ZNameList::DropTail( )
{
   if ( m_pHeadName )
   {
      ZNameItem *pItem = m_pHeadName;
      ZNameItem *pItemPrev = 0;
      while ( pItem->m_pNextName )
      {
         pItemPrev = pItem;
         pItem = pItem->m_pNextName;
      }

      if ( pItemPrev )
      {
         pItemPrev->m_pNextName = 0;
         m_pTailName = pItemPrev;
      }
      else
      if ( pItem == m_pHeadName )
      {
         m_pHeadName = 0;
         m_pTailName = 0;
      }

      return( pItem );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// class ZPointerList : simple single-linked list of numbers
//
ZPointerItem::ZPointerItem( zPVOID pvPointer, zLONG lFlag )
{
// TraceLineS( "Add PointerItem: ", m_csPointer );
   m_lFlag = lFlag;
   m_pvPointer = pvPointer;
   m_pNextPointer = 0;
}

ZPointerItem::~ZPointerItem( )
{
// TraceLineS( "Delete PointerItem: ", m_csPointer );
   mDeleteInit( m_pNextPointer );
}

ZPointerList::ZPointerList( zBOOL bUnique )
{
   m_pHeadPointer = 0;
   m_pTailPointer = 0;
   m_bUnique = bUnique;
}

ZPointerList::~ZPointerList( )
{
// TraceLineS( "Delete PointerList", "" );
   ZPointerItem *pItem = m_pHeadPointer;
   m_pHeadPointer = 0;
   ZPointerItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextPointer;
      pItem->m_pNextPointer = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZPointerItem *
ZPointerList::AddHead( zPVOID pvPointer, zLONG lFlag )
{
   if ( m_bUnique && Find( pvPointer ) )
      return( 0 );

   if ( m_pHeadPointer == 0 )
   {
      m_pHeadPointer = new ZPointerItem( pvPointer, lFlag );
      m_pTailPointer = m_pHeadPointer;
   }
   else
   {
      ZPointerItem *pItem = new ZPointerItem( pvPointer, lFlag );
      pItem->m_pNextPointer = m_pHeadPointer;
      m_pHeadPointer = pItem;
   }

   return( m_pHeadPointer );
}

// Return pointer if added ... 0 if already in the list.
ZPointerItem *
ZPointerList::AddTail( zPVOID pvPointer, zLONG lFlag )
{
   if ( m_bUnique && Find( pvPointer ) )
      return( 0 );

   if ( m_pHeadPointer == 0 )
   {
      m_pHeadPointer = new ZPointerItem( pvPointer, lFlag );
      m_pTailPointer = m_pHeadPointer;
      return( m_pHeadPointer );
   }
   else
   {
      m_pTailPointer->m_pNextPointer = new ZPointerItem( pvPointer, lFlag );
      m_pTailPointer = m_pTailPointer->m_pNextPointer;
      return( m_pTailPointer );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZPointerItem *
ZPointerList::Find( zPVOID pvPointer )
{
   ZPointerItem *pItem = m_pHeadPointer;
   while ( pItem )
   {
      if ( pvPointer == pItem->m_pvPointer )
         return( pItem );

      pItem = pItem->m_pNextPointer;
   }

   return( 0 );
}

// Return TRUE if found ... FALSE if not found in the list.
ZPointerItem *
ZPointerList::Remove( zPVOID pvPointer )
{
   ZPointerItem *pItem = m_pHeadPointer;
   ZPointerItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( pvPointer == pItem->m_pvPointer )
      {
         if ( pItemPrev )
         {
            pItemPrev->m_pNextPointer = pItem->m_pNextPointer;
            if ( pItemPrev->m_pNextPointer == 0 )
               m_pTailPointer = pItemPrev;
         }
         else
         {
            m_pHeadPointer = pItem->m_pNextPointer;
            if ( m_pHeadPointer == 0 )
               m_pTailPointer = 0;
         }

         pItem->m_pNextPointer = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextPointer;
   }

   return( 0 );
}

// Return ptr if found for move ... 0 otw.
ZPointerItem *
ZPointerList::MoveHead( zPVOID pvPointer )
{
   if ( m_pHeadPointer && pvPointer == m_pHeadPointer->m_pvPointer )
      return( m_pHeadPointer );

   ZPointerItem *pItem = m_pHeadPointer->m_pNextPointer;
   ZPointerItem *pItemPrev = m_pHeadPointer;
   while ( pItem )
   {
      if ( pvPointer == pItem->m_pvPointer )
      {
         pItemPrev->m_pNextPointer = pItem->m_pNextPointer;
         if ( pItemPrev->m_pNextPointer == 0 )
            m_pTailPointer = pItemPrev;

         pItem->m_pNextPointer = m_pHeadPointer;
         m_pHeadPointer = pItem;

         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextPointer;
   }

   return( 0 );
}

// Return pointer if dropped ... 0 if not in the list.
ZPointerItem *
ZPointerList::DropHead( )
{
   ZPointerItem *pItem = m_pHeadPointer;
   if ( m_pHeadPointer )
   {
      m_pHeadPointer = m_pHeadPointer->m_pNextPointer;
      pItem->m_pNextPointer = 0;
      if ( m_pHeadPointer == 0 )
         m_pTailPointer = 0;
   }

   return( pItem );
}

// Return pointer if dropped ... 0 if not in the list.
ZPointerItem *
ZPointerList::DropTail( )
{
   if ( m_pHeadPointer )
   {
      ZPointerItem *pItem = m_pHeadPointer;
      ZPointerItem *pItemPrev = 0;
      while ( pItem->m_pNextPointer )
      {
         pItemPrev = pItem;
         pItem = pItem->m_pNextPointer;
      }

      if ( pItemPrev )
      {
         pItemPrev->m_pNextPointer = 0;
         m_pTailPointer = pItemPrev;
      }
      else
      if ( pItem == m_pHeadPointer )
      {
         m_pHeadPointer = 0;
         m_pTailPointer = 0;
      }

      return( pItem );
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// class ZPairedList : simple single-linked list of paired numbers.
//
ZPairedItem::ZPairedItem( zLONG lValue1, zLONG lValue2,
                          zLONG lValue3, zLONG lValue4 )
{
// TraceLineS( "Add PairedItem: ", m_lValue1 );
   m_lValue1 = lValue1;
   m_lValue2 = lValue2;
   m_lValue3 = lValue3;
   m_lValue4 = lValue4;
   m_pNextItem = 0;
}

ZPairedItem::~ZPairedItem( )
{
// TraceLineS( "Delete NumberItem: ", m_lValue1 );
   mDeleteInit( m_pNextItem );
}

ZPairedList::ZPairedList( )
{
   m_pHeadItem = 0;
}

ZPairedList::~ZPairedList( )
{
   Cleanup( );
}

void
ZPairedList::Cleanup( )
{
// TraceLineS( "Delete NumberList", "" );
   ZPairedItem *pItem = m_pHeadItem;
   m_pHeadItem = 0;
   ZPairedItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

void
ZPairedList::Display( zCPCHAR cpcText )
{
   zCHAR szMsg[ 256 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "Display NumberList (0x%08x): ", (zULONG) this );
   TraceLineS( szMsg, cpcText );

   ZPairedItem *pItem = m_pHeadItem;

   while ( pItem )
   {
      sprintf_s( szMsg, zsizeof( szMsg ), " 0x%8x 0x%8x 0x%8x 0x%8x",
                pItem->m_lValue1, pItem->m_lValue2,
                pItem->m_lValue3, pItem->m_lValue4 );
      TraceLineS( szMsg, "" );
      pItem = pItem->m_pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZPairedItem *
ZPairedList::Add( zLONG lValue1, zLONG lValue2,
                  zLONG lValue3, zLONG lValue4 )
{
   ZPairedItem *pItem = m_pHeadItem;

   if ( Find( lValue1, lValue2, lValue3, lValue4 ) )
      return( 0 );

   if ( m_pHeadItem == 0 )
   {
      m_pHeadItem = new ZPairedItem( lValue1, lValue2, lValue3, lValue4 );
      return( m_pHeadItem );
   }
   else
   {
      pItem = m_pHeadItem;
      while ( pItem->m_pNextItem )
         pItem = pItem->m_pNextItem;

      pItem->m_pNextItem = new ZPairedItem( lValue1, lValue2,
                                            lValue3, lValue4 );
      return( pItem->m_pNextItem );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZPairedItem *
ZPairedList::Find( zLONG lValue1, zLONG lValue2,
                   zLONG lValue3, zLONG lValue4, zLONG lFlag )
{
   ZPairedItem *pItem = m_pHeadItem;
   while ( pItem )
   {
      if ( ((lValue1 == pItem->m_lValue1) || (lFlag & 1) == 0) &&
           ((lValue2 == pItem->m_lValue2) || (lFlag & 2) == 0) &&
           ((lValue3 == pItem->m_lValue3) || (lFlag & 4) == 0) &&
           ((lValue4 == pItem->m_lValue4) || (lFlag & 8) == 0) )
      {
         return( pItem );
      }

      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

// Return ptr if found ... 0 if not found in the list.
ZPairedItem *
ZPairedList::Remove( ZPairedItem *pPairedItem )
{
   ZPairedItem *pItem = m_pHeadItem;
   ZPairedItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( pPairedItem == pItem )
      {
         if ( pItemPrev )
            pItemPrev->m_pNextItem = pItem->m_pNextItem;
         else
            m_pHeadItem = pItem->m_pNextItem;

         pItem->m_pNextItem = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

// Return ptr if found ... 0 if not found in the list.
ZPairedItem *
ZPairedList::DropHead( )
{
   ZPairedItem *pItem = m_pHeadItem;
   if ( m_pHeadItem )
   {
      m_pHeadItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
   }

   return( pItem );
}

/////////////////////////////////////////////////////////////////////////////
// class ZCorrelatedList : simple single-linked list of correlated numbers
//                         and values.
//
ZCorrelatedItem::ZCorrelatedItem( zLONG lNumber, zCPCHAR cpcValue ) :
                 m_csValue( cpcValue )
{
// TraceLineS( "Add NumberItem: ", m_csNumber );
   m_lNumber = lNumber;
   m_pcsKey = 0;
   m_pNextItem = 0;
}

ZCorrelatedItem::ZCorrelatedItem( zCPCHAR cpcKey, zCPCHAR cpcValue ) :
                 m_csValue( cpcValue )
{
// TraceLineS( "Add NumberItem: ", m_csNumber );
   m_lNumber = 0;
   m_pcsKey = new CString( cpcKey );
   m_pNextItem = 0;
}

ZCorrelatedItem::~ZCorrelatedItem( )
{
// TraceLineS( "Delete NumberItem: ", m_csNumber );
   mDeleteInit( m_pcsKey );
   mDeleteInit( m_pNextItem );
}

ZCorrelatedList::ZCorrelatedList( )
{
   m_pHeadItem = 0;
}

ZCorrelatedList::~ZCorrelatedList( )
{
// TraceLineS( "Delete NumberList", "" );
   ZCorrelatedItem *pItem = m_pHeadItem;
   m_pHeadItem = 0;
   ZCorrelatedItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZCorrelatedItem *
ZCorrelatedList::Add( zLONG lNumber, zCPCHAR cpcValue )
{
   ASSERT( cpcValue );

   ZCorrelatedItem *pItem = m_pHeadItem;

   if ( (pItem = Find( lNumber )) != 0 && (pItem == Find( cpcValue, FALSE )) )
      return( 0 );

   if ( m_pHeadItem == 0 )
   {
      m_pHeadItem = new ZCorrelatedItem( lNumber, cpcValue );
      return( m_pHeadItem );
   }
   else
   {
      pItem = m_pHeadItem;
      while ( pItem->m_pNextItem )
         pItem = pItem->m_pNextItem;

      pItem->m_pNextItem = new ZCorrelatedItem( lNumber, cpcValue );
      return( pItem->m_pNextItem );
   }
}

ZCorrelatedItem *
ZCorrelatedList::Add( zCPCHAR cpcKey, zCPCHAR cpcValue )
{
   ASSERT( cpcValue );

   ZCorrelatedItem *pItem = m_pHeadItem;

   if ( (pItem = Find( cpcKey, TRUE )) != 0 && (pItem == Find( cpcValue, FALSE )) )
      return( 0 );

   if ( m_pHeadItem == 0 )
   {
      m_pHeadItem = new ZCorrelatedItem( cpcKey, cpcValue );
      return( m_pHeadItem );
   }
   else
   {
      pItem = m_pHeadItem;
      while ( pItem->m_pNextItem )
         pItem = pItem->m_pNextItem;

      pItem->m_pNextItem = new ZCorrelatedItem( cpcKey, cpcValue );
      return( pItem->m_pNextItem );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZCorrelatedItem *
ZCorrelatedList::Find( zLONG lNumber )
{
   ZCorrelatedItem *pItem = m_pHeadItem;
   while ( pItem )
   {
      if ( lNumber == pItem->m_lNumber )
         return( pItem );

      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

// Return pointer if found ... 0 if not found in the list.
ZCorrelatedItem *
ZCorrelatedList::Find( zCPCHAR cpcValue, zBOOL bKey )
{
   ASSERT( cpcValue );

   ZCorrelatedItem *pItem = m_pHeadItem;
   while ( pItem )
   {
      if ( (bKey == FALSE && zstrcmp( cpcValue, pItem->m_csValue.GetString() ) == 0) ||
           (bKey && zstrcmp( cpcValue, (*(pItem->m_pcsKey)).GetString() ) == 0) )
      {
         return( pItem );
      }

      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

// Return TRUE if found ... FALSE if not found in the list.
ZCorrelatedItem *
ZCorrelatedList::Remove( zLONG lNumber )
{
   ZCorrelatedItem *pItem = m_pHeadItem;
   ZCorrelatedItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( lNumber == pItem->m_lNumber )
      {
         if ( pItemPrev )
            pItemPrev->m_pNextItem = pItem->m_pNextItem;
         else
            m_pHeadItem = pItem->m_pNextItem;

         pItem->m_pNextItem = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

// Return TRUE if found ... FALSE if not found in the list.
ZCorrelatedItem *
ZCorrelatedList::Remove( zCPCHAR cpcValue, zBOOL bKey )
{
   ASSERT( cpcValue );

   ZCorrelatedItem *pItem = m_pHeadItem;
   ZCorrelatedItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( (bKey == FALSE && zstrcmp( cpcValue, pItem->m_csValue.GetString() ) == 0) ||
           (bKey && zstrcmp( cpcValue, (*(pItem->m_pcsKey)).GetString() ) == 0) )
      {
         if ( pItemPrev )
            pItemPrev->m_pNextItem = pItem->m_pNextItem;
         else
            m_pHeadItem = pItem->m_pNextItem;

         pItem->m_pNextItem = 0;
         return( pItem );
      }

      pItemPrev = pItem;
      pItem = pItem->m_pNextItem;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// class ZDualCorrelatedList : simple single-linked list of correlated
//                             numbers with a dual value (number+string).
//
ZDualCorrelatedItem::ZDualCorrelatedItem( zLONG   lKey,
                                          zLONG   lValue,
                                          zCPCHAR cpcKey,
                                          zCPCHAR cpcValue )
{
// TraceLineS( "Add NumberItem: ", m_csNumber );
   m_lKey = lKey;
   m_lValue = lValue;
   if ( cpcKey )
      m_pcsKey = new CString( cpcKey );
   else
      m_pcsKey = 0;

   if ( cpcValue )
      m_pcsValue = new CString( cpcValue );
   else
      m_pcsValue = 0;

   m_pNextItem = 0;
}

ZDualCorrelatedItem::~ZDualCorrelatedItem( )
{
// TraceLineS( "Delete ZDualCorrelatedItem: ", m_csNumber );
   mDeleteInit( m_pcsKey );
   mDeleteInit( m_pcsValue );
   mDeleteInit( m_pNextItem );
}

ZDualCorrelatedList::ZDualCorrelatedList( )
{
   m_pHeadItem = 0;
}

ZDualCorrelatedList::~ZDualCorrelatedList( )
{
// TraceLineS( "Delete ZDualCorrelatedItem", "" );
   Cleanup( );
}

void
ZDualCorrelatedList::Cleanup( )
{
   ZDualCorrelatedItem *pItem = m_pHeadItem;
   ZDualCorrelatedItem *pNextItem;

   m_pHeadItem = 0;
   while ( pItem )
   {
      pNextItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... even if already in the list.
ZDualCorrelatedItem *
ZDualCorrelatedList::Add( zLONG lKey, zLONG lValue, zCPCHAR cpcKey, zCPCHAR cpcValue )
{
   ZDualCorrelatedItem *pItem;

   if ( (pItem = Find( lKey, lValue, cpcKey, cpcValue )) != 0 )
   {
      return( 0 );
   }

   if ( m_pHeadItem == 0 )
   {
      m_pHeadItem = new ZDualCorrelatedItem( lKey, lValue, cpcKey, cpcValue );
      return( m_pHeadItem );
   }
   else
   {
      pItem = new ZDualCorrelatedItem( lKey, lValue, cpcKey, cpcValue );
      pItem->m_pNextItem = m_pHeadItem;
      return( pItem );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZDualCorrelatedItem *
ZDualCorrelatedList::FindKey( zLONG lKey )
{
   return( Find( lKey, 0, 0, 0, 1 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindKey( zCPCHAR cpcKey )
{
   return( Find( 0, 0, cpcKey, 0, 4 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindKey( zLONG lKey, zCPCHAR cpcKey )
{
   return( Find( lKey, 0, cpcKey, 0, 5 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindValue( zLONG lValue )
{
   return( Find( 0, lValue, 0, 0, 2 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindValue( zCPCHAR cpcValue )
{
   return( Find( 0, 0, 0, cpcValue, 8 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindValue( zLONG lValue, zCPCHAR cpcValue )
{
   return( Find( 0, lValue, 0, cpcValue, 10 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::Find( zLONG lKey, zLONG lValue, zCPCHAR cpcKey, zCPCHAR cpcValue, zLONG lFlag )
{
   m_pCurrItem = m_pHeadItem;
   while ( m_pCurrItem )
   {
      if ( (((lFlag & 1) == 0) || (lKey == m_pCurrItem->m_lKey)) &&
           (((lFlag & 2) == 0) || (lValue == m_pCurrItem->m_lValue)) &&
           (((lFlag & 4) == 0) || (m_pCurrItem->m_pcsKey && zstrcmp( cpcKey, (*(m_pCurrItem->m_pcsKey)).GetString() ) == 0)) &&
           (((lFlag & 8) == 0) || (m_pCurrItem->m_pcsValue && zstrcmp( cpcValue, (*(m_pCurrItem->m_pcsValue)).GetString() ) == 0)) )
      {
         break;
      }

      m_pCurrItem = m_pCurrItem->m_pNextItem;
   }

   return( m_pCurrItem );
}

// Return pointer if found ... 0 if not found in the list.
ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextKey( zLONG lKey )
{
   return( FindNext( lKey, 0, 0, 0, 1 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextKey( zCPCHAR cpcKey )
{
   return( FindNext( 0, 0, cpcKey, 0, 4 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextKey( zLONG   lKey,
                                  zCPCHAR cpcKey )
{
   return( FindNext( lKey, 0, cpcKey, 0, 5 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextValue( zLONG lValue )
{
   return( FindNext( 0, lValue, 0, 0, 2 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextValue( zCPCHAR cpcValue )
{
   return( FindNext( 0, 0, 0, cpcValue, 8 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNextValue( zLONG   lValue,
                                    zCPCHAR cpcValue )
{
   return( FindNext( 0, lValue, 0, cpcValue, 10 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::FindNext( zLONG lKey, zLONG lValue, zCPCHAR cpcKey, zCPCHAR cpcValue, zLONG lFlag )
{
   if ( m_pCurrItem == 0 )
      m_pCurrItem = m_pHeadItem;
   else
      m_pCurrItem = m_pCurrItem->m_pNextItem;

   while ( m_pCurrItem )
   {
      if ( (((lFlag & 1) == 0) || (lKey == m_pCurrItem->m_lKey)) &&
           (((lFlag & 2) == 0) || (lValue == m_pCurrItem->m_lValue)) &&
           (((lFlag & 4) == 0) || (zstrcmp( cpcKey, (*(m_pCurrItem->m_pcsKey)).GetString() ) == 0)) &&
           (((lFlag & 8) == 0) || (zstrcmp( cpcValue, (*(m_pCurrItem->m_pcsValue)).GetString() ) == 0)) )
      {
         break;
      }

      m_pCurrItem = m_pCurrItem->m_pNextItem;
   }

   return( m_pCurrItem );
}

// Return TRUE if found ... FALSE if not found in the list.
ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByKey( zLONG lKey )
{
   return( Remove( lKey, 0, 0, 0, 1 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByKey( zCPCHAR cpcKey )
{
   return( Remove( 0, 0, cpcKey, 0, 4 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByKey( zLONG   lKey,
                                  zCPCHAR cpcKey )
{
   return( Remove( lKey, 0, cpcKey, 0, 5 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByValue( zLONG lValue )
{
   return( Remove( 0, lValue, 0, 0, 2 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByValue( zCPCHAR cpcValue )
{
   return( Remove( 0, 0, 0, cpcValue, 8 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::RemoveByValue( zLONG   lValue,
                                    zCPCHAR cpcValue )
{
   return( Remove( 0, lValue, 0, cpcValue, 10 ) );
}

ZDualCorrelatedItem *
ZDualCorrelatedList::Remove( zLONG lKey, zLONG lValue, zCPCHAR cpcKey, zCPCHAR cpcValue, zLONG lFlag )
{
   ZDualCorrelatedItem *pItem = m_pHeadItem;

   while ( pItem )
   {
      if ( (((lFlag & 1) == 0) || (lKey == m_pCurrItem->m_lKey)) &&
           (((lFlag & 2) == 0) || (lValue == m_pCurrItem->m_lValue)) &&
           (((lFlag & 4) == 0) || (zstrcmp( cpcKey, (*(m_pCurrItem->m_pcsKey)).GetString() ) == 0)) &&
           (((lFlag & 8) == 0) || (zstrcmp( cpcValue, (*(m_pCurrItem->m_pcsValue)).GetString() ) == 0)) )
      {
         break;
      }

      pItem = pItem->m_pNextItem;
   }

   if ( pItem == m_pCurrItem )
      m_pCurrItem = 0;

   return( pItem );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

ZColorItem::ZColorItem( zLONG lColumn, zCPCHAR cpcValue,
                        zLONG lClrText, zLONG lClrBack, zLONG lClrFlags ) :
            m_csValue( cpcValue )
{
   m_pNextItem = 0;
   m_lColumn = lColumn;
   m_lClrText = lClrText;
   m_lClrBack = lClrBack;
   m_lClrSelText = CLR_INVALID;  // use default color
   m_lClrSelBack = CLR_INVALID;  // use default color
   m_lClrFlags = lClrFlags;
}

ZColorItem::~ZColorItem( )
{
   mDeleteInit( m_pNextItem );
}

ZColorList::ZColorList( zFARPROC_ITEMCOMPARE lpfnCompare )
{
   m_pHeadItem = 0;
   m_lpfnCompare = lpfnCompare;
}

ZColorList::~ZColorList( )
{
// TraceLineS( "Delete NumberList", "" );
   ZColorItem *pItem = m_pHeadItem;
   m_pHeadItem = 0;
   ZColorItem *pNextItem;

   while ( pItem )
   {
      pNextItem = pItem->m_pNextItem;
      pItem->m_pNextItem = 0;
      delete( pItem );
      pItem = pNextItem;
   }
}

// Return pointer if added ... 0 if already in the list.
ZColorItem *
ZColorList::Add( zLONG   lColumn,
                 zCPCHAR cpcValue,
                 zLONG   lClrText,
                 zLONG   lClrBack,
                 zLONG   lClrFlags )
{
   ASSERT( cpcValue );

   ZColorItem *pColorItem = m_pHeadItem;
   zLONG lFlagsMasked = lClrFlags & 0x0FFFFFF0;
   while ( pColorItem )
   {
      if ( pColorItem->m_lColumn == lColumn &&
           zstrcmp( pColorItem->m_csValue.GetString(), cpcValue ) == 0 &&
           (pColorItem->m_lClrFlags & 0x0FFFFFF0) == lFlagsMasked )
      {
         break;
      }

      pColorItem = pColorItem->m_pNextItem;
   }

   if ( pColorItem )
   {
      if ( lClrFlags & 0x80000000 )
      {
         pColorItem->m_lClrSelText = lClrText;
         pColorItem->m_lClrSelBack = lClrBack;
      }
      else
      {
         pColorItem->m_lClrText = lClrText;
         pColorItem->m_lClrBack = lClrBack;
         pColorItem->m_lClrFlags = lClrFlags;
      }

      return( 0 );
   }

   if ( m_pHeadItem == 0 )
   {
      m_pHeadItem = new ZColorItem( lColumn, cpcValue,
                                    lClrText, lClrBack, lClrFlags );
      return( m_pHeadItem );
   }
   else
   {
      pColorItem = m_pHeadItem;
      while ( pColorItem->m_pNextItem )
         pColorItem = pColorItem->m_pNextItem;

      pColorItem->m_pNextItem = new ZColorItem( lColumn, cpcValue,
                                                lClrText, lClrBack, lClrFlags );
      return( pColorItem->m_pNextItem );
   }
}

// Return pointer if found ... 0 if not found in the list.
ZColorItem *
ZColorList::Find( zLONG lColumn, zCPCHAR cpcValue, zBOOL bTestBoth )
{
   ZColorItem *pColorItem = m_pHeadItem;
   while ( pColorItem )
   {
      if ( (*m_lpfnCompare)( pColorItem, lColumn, cpcValue, bTestBoth ) == 0 )
      {
         return( pColorItem );
      }

      pColorItem = pColorItem->m_pNextItem;
   }

   return( 0 );
}

// Return pointer if found ... 0 if not found in the list.
ZColorItem *
ZColorList::FindNext( zLONG   lColumn,
                      zCPCHAR cpcValue,
                      zBOOL   bTestBoth )
{
   ZColorItem *pColorItem;
   if ( m_pCurrItem == 0 )
      pColorItem = m_pHeadItem;
   else
   {
      pColorItem = m_pCurrItem->m_pNextItem;
      m_pCurrItem = 0;
   }

   while ( pColorItem )
   {
      if ( (*m_lpfnCompare)( pColorItem, lColumn, cpcValue, bTestBoth ) == 0 )
      {
         return( pColorItem );
      }
   }

   return( m_pCurrItem );
}

// Return TRUE if found ... FALSE if not found in the list.
ZColorItem *
ZColorList::Remove( zLONG   lColumn,
                    zCPCHAR cpcValue )
{
   ZColorItem *pColorItem = m_pHeadItem;
   ZColorItem *pItemPrev = 0;
   while ( pColorItem )
   {
      if ( lColumn == pColorItem->m_lColumn &&
           zstrcmp( cpcValue, pColorItem->m_csValue.GetString() ) == 0 )
      {
         if ( pItemPrev )
            pItemPrev->m_pNextItem = pColorItem->m_pNextItem;
         else
            m_pHeadItem = pColorItem->m_pNextItem;

         pColorItem->m_pNextItem = 0;
         return( pColorItem );
      }

      pItemPrev = pColorItem;
      pColorItem = pColorItem->m_pNextItem;
   }

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
// class ZXRA_List : simple single-linked list of names
//
ZXRA_Item::ZXRA_Item( zCPCHAR cpcName, zVIEW vXRA ) :
           m_csName( cpcName )
{
// TraceLineS( "ZXRA_Item ctor: ", m_csName );
// TraceLineI( "ZXRA_Item ctor: ", (zLONG) this );
   m_pNext = 0;
   m_bDefSent = FALSE;
   m_vXRA_Delta = 0;       // Outgoing (to LightClient) XRW delta
   m_pZSubtask = 0;        // Pointer to related ZSubtask

   m_vXRA_Def = vXRA;      // XRW Definition for this window

#ifdef DEBUG_ALL
// SetNameForView( m_vXRA_Def, "__XRA_Def", m_vDialog, zLEVEL_SUBTASK );
#endif

   // Complete XRW subtask view (with data)
   ActivateOI_FromOI_ForTask( &m_vXRA, vXRA, 0, zSINGLE );

#ifdef DEBUG_ALL
// SetNameForView( m_vXRA, "__XRA_Definition", m_vDialog, zLEVEL_SUBTASK );
#endif

   SetAttributeFromString( m_vXRA_Def, "App", "Definition", "Y" );
   SetAttributeFromString( m_vXRA, "App", "Definition", "" );
}

ZXRA_Item::~ZXRA_Item( )
{
// TraceLineS( "ZXRA_Item dtor: ", m_csName );
// TraceLineI( "ZXRA_Item dtor: ", (zLONG) this );
   mDropOI_Init( m_vXRA_Def );
   mDropOI_Init( m_vXRA );
   mDropOI_Init( m_vXRA_Delta );
// mDeleteInit( m_pNext );
}

void
ZXRA_Item::ActivateEmptyDelta( zVIEW vSubtask, zCPCHAR cpcAppTag )
{
   mDropOI_Init( m_vXRA_Delta );

   // Activate using SfActivateSysEmptyOI so that the kzxra.lod
   // does not have to be in the application LPLR.
   SfActivateSysEmptyOI( &m_vXRA_Delta, "KZXRA", vSubtask, zSINGLE );

#ifdef DEBUG_ALL
   SetNameForView( m_vXRA_Delta, "__XRA_Delta", vSubtask, zLEVEL_SUBTASK );
#endif

   CreateEntity( m_vXRA_Delta, "App", zPOS_AFTER );
   SetAttributeFromString( m_vXRA_Delta, "App", "Tag", cpcAppTag );
   CreateEntity( m_vXRA_Delta, "Wnd", zPOS_AFTER );
   SetAttributeFromString( m_vXRA_Delta, "Wnd", "Tag", m_csName );
}

/////////////////////////////////////////////////////////////////////////////

ZXRA_List::ZXRA_List( ZSocketTask *pZSocketTask )
{
   m_pZSocketTask = pZSocketTask;
   m_pHead = 0;
#ifdef DEBUG_ALL
   zCHAR szMsg[ 128 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] XRA_List ctor",
             m_pZSocketTask,
             AfxGetThread( ),
             m_pZSocketTask->m_pWinThread,
             m_pZSocketTask->m_sock );
   TraceLineS( szMsg, "" );
#endif
}

ZXRA_List::~ZXRA_List( )
{
#ifdef DEBUG_ALL
   zCHAR szMsg[ 128 ];

   sprintf_s( szMsg, zsizeof( szMsg ), "[S%ld.T%ld.t%ld.s%ld] XRA_List dtor",
             m_pZSocketTask,
             AfxGetThread( ),
             m_pZSocketTask->m_pWinThread,
             m_pZSocketTask->m_sock );
   TraceLineS( szMsg, "" );

   zLONG lCnt = 0;
#endif

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );
   ZXRA_Item *pXRA = m_pHead;
   m_pHead = 0;
   ZXRA_Item *pXRA_Next;

   while ( pXRA )
   {
#ifdef DEBUG_ALL
      lCnt++;
#endif
      pXRA_Next = pXRA->m_pNext;
      pXRA->m_pNext = 0;
      delete( pXRA );
      pXRA = pXRA_Next;
   }

#ifdef DEBUG_ALL
   TraceLineI( "Delete ZXRA_List", lCnt );
#endif

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

// Return pointer if added ... 0 if already in the list.
ZXRA_Item *
ZXRA_List::Add( zCPCHAR cpcName, ZSubtask *pZSubtask, zVIEW vXRA_Def )
{
   if ( Find( cpcName, pZSubtask, "Add" ) )
      return( 0 );

   ZXRA_Item *pXRA;

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   if ( m_pHead == 0 )
   {
      m_pHead = new ZXRA_Item( cpcName, vXRA_Def );
      pXRA = m_pHead;
   }
   else
   {
      pXRA = m_pHead;
      while ( pXRA->m_pNext )
         pXRA = pXRA->m_pNext;

      pXRA->m_pNext = new ZXRA_Item( cpcName, vXRA_Def );
      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
   return( pXRA );
}

// Return pointer if found ... 0 if not found in the list.
ZXRA_Item *
ZXRA_List::Find( zCPCHAR cpcName, ZSubtask *pZSubtask, zCPCHAR cpcLabel )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );
   ZXRA_Item *pXRA;

#ifdef DEBUG_ALL

   zCHAR szMsg[ 256 ];

   pXRA = m_pHead;
   sprintf_s( szMsg, zsizeof( szMsg ), "ZXRA_List find: %s Label: %s", cpcName, cpcLabel );
   TraceLineS( szMsg, "" );

   while ( pXRA )
   {
      sprintf_s( szMsg, zsizeof( szMsg ),
                "   XRA: %ld   pZSubtask: %ld   Def: %ld"
                "   Name: %s   RemoteWndTag: %s",
                pXRA, pXRA->m_pZSubtask, pXRA->m_vXRA_Def, pXRA->m_csName,
                (pXRA->m_pZSubtask && pXRA->m_pZSubtask->m_pzsRemoteWndTag) ?
                             *(pXRA->m_pZSubtask->m_pzsRemoteWndTag) : "?" );
      TraceLineS( szMsg, "" );
      pXRA = pXRA->m_pNext;
   }

#endif

   if ( pZSubtask )
   {
      pXRA = m_pHead;
      while ( pXRA )
      {
         if ( pZSubtask == pXRA->m_pZSubtask &&
              zstrcmp( cpcName, pXRA->m_csName ) == 0 )
         {
#ifdef DEBUG_ALL
            sprintf_s( szMsg, zsizeof( szMsg ), "=> XRA: %ld pZSubtask: %ld Def: %ld Name: %s",
                      pXRA, pXRA->m_pZSubtask, pXRA->m_vXRA_Def, pXRA->m_csName );
            TraceLineS( szMsg, "" );
#endif
            break;
         }

         pXRA = pXRA->m_pNext;
      }
   }
   else
      pXRA = 0;

   if ( pXRA == 0 )
   {
      pXRA = m_pHead;

      while ( pXRA )
      {
         if ( zstrcmp( cpcName, pXRA->m_csName ) == 0 )
         {
#ifdef DEBUG_ALL
            sprintf_s( szMsg, zsizeof( szMsg ),
                      "=> XRA: %ld   pZSubtask: %ld   Def: %ld   Name: %s",
                      pXRA, pXRA->m_pZSubtask,
                      pXRA->m_vXRA_Def, pXRA->m_csName );
            TraceLineS( szMsg, "" );
#endif
            if ( pZSubtask && pXRA->m_pZSubtask != pZSubtask )
               pXRA->m_pZSubtask = pZSubtask;

            break;
         }

         pXRA = pXRA->m_pNext;
      }
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
   return( pXRA );
}

// Return pointer if found ... 0 if not found in the list.
ZXRA_Item *
ZXRA_List::Remove( zCPCHAR cpcName )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   ZXRA_Item *pXRA = m_pHead;
   ZXRA_Item *pXRA_Prev = 0;
   while ( pXRA )
   {
      if ( zstrcmp( cpcName, pXRA->m_csName ) == 0 )
      {
         if ( pXRA_Prev )
            pXRA_Prev->m_pNext = pXRA->m_pNext;
         else
            m_pHead = pXRA->m_pNext;

         pXRA->m_pNext = 0;
         break;
      }

      pXRA_Prev = pXRA;
      pXRA = pXRA->m_pNext;
   }

   if ( pXRA == m_pHead )
      m_pHead = 0;

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
   return( pXRA );
}

// Return pointer if found ... 0 if not found in the list.
ZXRA_Item *
ZXRA_List::Remove( ZXRA_Item *pXRA_Remove )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   ZXRA_Item *pXRA = m_pHead;
   ZXRA_Item *pXRA_Prev = 0;
   while ( pXRA )
   {
      if ( pXRA == pXRA_Remove )
      {
         if ( pXRA_Prev )
            pXRA_Prev->m_pNext = pXRA->m_pNext;
         else
            m_pHead = pXRA->m_pNext;

         pXRA->m_pNext = 0;
         break;
      }

      pXRA_Prev = pXRA;
      pXRA = pXRA->m_pNext;
   }

   if ( pXRA == m_pHead )
      m_pHead = 0;

#ifdef DEBUG_ALL

   if ( pXRA )
      TraceLineS( "ZXRA_List remove: ", pXRA->m_csName );
   else
      TraceLineS( "ZXRA_List remove: ", "Not Found" );

#endif

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
   return( pXRA );
}

// Delete all XRA's that have a null pointer to m_pZSubtask.
void
ZXRA_List::DeleteEmpty( )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   ZXRA_Item *pXRA = m_pHead;
   ZXRA_Item *pXRA_Prev = 0;
   while ( pXRA )
   {
      if ( pXRA->m_pZSubtask == 0 )
      {
         if ( pXRA_Prev )
            pXRA_Prev->m_pNext = pXRA->m_pNext;
         else
            m_pHead = pXRA->m_pNext;

         if ( pXRA == m_pHead )
            m_pHead = 0;

      // TraceLineS( "ZXRA_List DeleteEmpty: ", pXRA->m_csName );
         delete( pXRA );
         pXRA = m_pHead;
         pXRA_Prev = 0;
         continue;
      }

      pXRA_Prev = pXRA;
      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

void
ZXRA_List::ResetSubtasks( )
{
   ZXRA_Item *pXRA = m_pHead;
   zSHORT    nViewCnt = 0;

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   while ( pXRA )
   {
      if ( pXRA->m_pZSubtask )
         pXRA->m_pZSubtask->m_ulSubtaskFlags &= ~zSUBTASK_MAPPED_SINCE_APPLYSTATE;

      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

zSHORT
ZXRA_List::CountViews( zVIEW *pvOI )
{
   ZXRA_Item *pXRA = m_pHead;
   zSHORT    nViewCnt = 0;

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   while ( pXRA )
   {
      if ( pXRA->m_bDefSent == FALSE )
      {
         if ( pvOI )
         {
            pvOI[ nViewCnt ] = pXRA->m_vXRA_Def;
#ifdef DEBUG_ALL
            TraceLineS( "Sending Definition for: ", pXRA->m_csName );
            DisplayObjectInstance( pXRA->m_vXRA_Def, 0, 0 );
#endif
         }

         nViewCnt++;
      }

      if ( pXRA->m_vXRA_Delta )
      {
         if ( pvOI )
         {
            zCHAR szY[ 2 ];

            GetStringFromAttribute( szY, zsizeof( szY ), pXRA->m_vXRA_Delta,
                                    "Wnd", "Closed" );
            if ( szY[ 0 ] == 0 )
               GetStringFromAttribute( szY, zsizeof( szY ), pXRA->m_vXRA_Delta,
                                       "Wnd", "Deleted" );

            if ( szY[ 0 ] == 0 )
               SetAttributeFromString( pXRA->m_vXRA_Delta, "Wnd",
                                       "Opened", "Y" );

            pvOI[ nViewCnt ] = pXRA->m_vXRA_Delta;
#ifdef DEBUG_ALL
            TraceLineS( "Sending Delta for: ", pXRA->m_csName );
            DisplayObjectInstance( pXRA->m_vXRA_Delta, 0, 0 );
#endif
         }

         nViewCnt++;
      }

      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
   return( nViewCnt );
}

void
ZXRA_List::ListViews( )
{
   ZXRA_Item *pXRA = m_pHead;

   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   TraceLineS( "ZXRA_List::ListViews", "" );
   while ( pXRA )
   {
      TraceLineS( " - ", pXRA->m_csName );
      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

void
ZXRA_List::DropObjectInstances( )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   ZXRA_Item *pXRA = m_pHead;

   while ( pXRA )
   {
      pXRA->m_bDefSent = TRUE;
      if ( pXRA->m_vXRA_Def )
         SetAttributeFromString( pXRA->m_vXRA_Def, "App", "Definition", "" );

      mDropOI_Init( pXRA->m_vXRA_Delta );
      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

void
ZXRA_List::SetDeltas( zBOOL bFromXRA )
{
   // Protect this with a semaphore!!!
   SysMutexLock( m_pZSocketTask->vSubtask, "ZDa", 0, 0 );

   ZXRA_Item *pXRA = m_pHead;

   while ( pXRA )
   {
      if ( bFromXRA )
         pXRA->m_vXRA_Delta = pXRA->m_vXRA;
      else
         pXRA->m_vXRA_Delta = 0;

      pXRA = pXRA->m_pNext;
   }

   SysMutexUnlock( m_pZSocketTask->vSubtask, "ZDa", 0 );
}

#endif   // #ifdef zREMOTE_SERVER

/////////////////////////////////////////////////////////////////////////////
// class ZEventList : simple single-linked list of names
//
ZParmItem::ZParmItem( zLONG lID, CString csValue ) :
           m_csValue( csValue )
{
// TraceLineS( "Add ParmItem: ", m_csName );
   m_lID = lID;
// m_pNextParm = 0;
}

ZParmItem::~ZParmItem( )
{
#if 0
// TraceLineS( "Delete ParmItem: ", m_csName );
   ZParmItem *pNextParm;
   ZParmItem *pPrevParm = this;

   while ( pPrevParm == this )
   {
      pNextParm = m_pNextParm;
      while ( pNextParm && pNextParm->m_pNextParm )
      {
         pPrevParm = pNextParm;
         pNextParm = pNextParm->m_pNextParm;
         if ( m_pNextParm == this )
         {
            ::MessageBox( 0, "Recursive dtor", "ZParmItem", MB_OK );
            pNextParm = 0;    // get out of following if
            pPrevParm = 0;    // get out of outer loop
            break;            // get out of inner loop
         }
      }

      if ( pNextParm )
      {
         pPrevParm->m_pNextParm = 0;
         delete( pNextParm );
         pPrevParm = this;
      }
      else
         break;
   }

// TraceLineS( "ZParmItem dtor: ", m_csValue );
#endif
}

ZEventItem::ZEventItem( DISPID dispid )
{
// TraceLineS( "Add EventItem: ", m_csName );
   m_dispid = dispid;
// m_pNextEvent = 0;
// m_pHeadParm = 0;
}

ZEventItem::~ZEventItem( )
{
// TraceLineS( "Delete EventItem: ", m_csName );
// mDeleteInit( m_pNextEvent );
// mDeleteInit( m_pHeadParm );
   while ( !m_ParmList.IsEmpty() )
   {
      ZParmItem *pItem = (ZParmItem *) m_ParmList.RemoveHead();
      delete( pItem );
   }
}

// Return pointer if added ... 0 if already in the list.
ZParmItem *
ZEventItem::AddParm( zLONG lId, CString csValue )
{
#if 0
   ZParmItem *pItem = new ZParmItem( lId, csValue );
   m_lParmCnt++;

   if ( m_pHeadParm )
   {
      ZParmItem *pNextItem = m_pHeadParm;
      while ( pNextItem->m_pNextParm )
      {
         pNextItem = pNextItem->m_pNextParm;
      }

      pNextItem->m_pNextParm = pItem;
   }
   else
   {
      m_pHeadParm = pItem;
   }
#else
   ZParmItem *pItem = 0;
   if ( FindParm( lId ) == 0 )
   {
      ZParmItem *pItem = new ZParmItem( lId, csValue );
      if ( pItem )
         m_ParmList.AddTail( pItem );
   }
#endif

   return( pItem );
}

// Return pointer if found ... 0 if not found in the list.
ZParmItem *
ZEventItem::FindParm( zLONG lID )
{
#if 0

   ZParmItem *pItem = m_pHeadParm;
   while ( pItem )
   {
      if ( lID == pItem->m_lID )
         return( pItem );

      pItem = pItem->m_pNextParm;
   }

#else

   POSITION pos = GetFirstParmPosition();
   ZParmItem *pItem = 0;

   while ( pos )
   {
      pItem = (ZParmItem *)GetNextParm( pos );
      if ( lID == pItem->GetID() )
      {
         return( pItem );
      }
   }
#endif

   return( 0 );
}

ZEventList::ZEventList( )
{
// m_pHeadEvent = 0;
}

ZEventList::~ZEventList( )
{
// TraceLineS( "Delete EventList", "" );
// mDeleteInit( m_pHeadEvent );

   ZEventItem *pItem;
   while ( IsEmpty() == FALSE )
   {
      pItem = (ZEventItem *) RemoveHead();
      delete( pItem );
   }
}

// Return pointer if added ... 0 if already in the list.
ZEventItem *
ZEventList::AddEvent( DISPID dispid )
{
#if 0
   if ( FindEvent( dispid ) )
      return( 0 );

   ZEventItem *pItem = m_pHeadEvent;
   m_pHeadEvent = new ZEventItem( dispid );
   m_pHeadEvent->m_pNextEvent = pItem;

#else

   ZEventItem *pItem = 0;

   if ( FindEvent( dispid ) == FALSE )
   {
      pItem = new ZEventItem( dispid );
      AddTail( pItem );
   }
#endif

   return( pItem );
}

// Return TRUE if removed ... FALSE if not in the list.
zBOOL
ZEventList::DeleteEvent( DISPID dispid )
{
#if 0

   ZEventItem *pItem = m_pHeadEvent;
   ZEventItem *pItemPrev = 0;
   while ( pItem )
   {
      if ( pItem->m_dispid == dispid )
      {
         if ( pItemPrev )
            pItemPrev->m_pNextEvent = pItem->m_pNextEvent;
         else
            m_pHeadEvent = pItem->m_pNextEvent;

         pItem->m_pNextEvent = 0;
         delete( pItem );
         return( TRUE );
      }
   }

#else

   POSITION pos = GetHeadPosition( );
   ZEventItem *pItem = 0;

   while ( pos )
   {
      pItem = (ZEventItem *) GetAt( pos );
      if ( pItem->GetID() == dispid )
      {
         RemoveAt( pos );
         delete( pItem );
         return( TRUE );
      }
      else
      {
         GetNext( pos );
      }
   }
#endif

   return( FALSE );
}

// Return pointer if found ... 0 if not found in the list.
ZEventItem *
ZEventList::FindEvent( DISPID dispid )
{
#if 0

   ZEventItem *pItem = m_pHeadEvent;
   while ( pItem )
   {
      if ( dispid == pItem->m_dispid )
         return( pItem );

      pItem = pItem->m_pNextEvent;
   }

#else

   ZEventItem *pItem;
   POSITION pos = GetHeadPosition( );

   while ( pos )
   {
      pItem = (ZEventItem *) GetNext( pos );
      if ( pItem->GetID( ) == dispid )
      {
         return( pItem );
      }
   }
#endif

   return( 0 );
}

// #define zDEBUG_MEM

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// NOTE: If the allocate of memory fails, the memory pointer passed
//       is nulled and the handle returned is zero.
//
//       When an application requests lBytes of memory, additional
//       memory is allocated to handle the request so that the Driver
//       can track the application's memory usage (and inform the
//       application if memory is overwritten or not freed).  For
//       each application memory allocation request, 4 guard-byte
//       pointers, a length (long), and a pointer to next are allocated
//       in addition to the application requested memory (which is
//       rounded up to the nearest 4-byte boundary).
//
//       So the memory layout for an application requested memory
//       allocation is as follows:
//
//       |||||||| |||||||| |||||||| ............ ||||||||
//         next    length   guard    app memory   guard
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
DrAllocTaskMemory( zCOREMEM Memory, zLONG lBytes, zLONG lGuard )
{
   SysMutexLock( g_vRegister, "ZDMem", 0, 0 );

#ifdef zDEBUG_MEM
   static long lId = 1;

   if ( lGuard == 0 )
      lGuard = lId++;
#else
   ASSERT( lGuard );
#endif

   if ( lBytes % 4 )
      lBytes += 4 - (lBytes % 4);

   zLONG lRealBytes = lBytes + sizeof( zPCHAR ) + (3 * sizeof( zLONG ));

   zPCHAR pchMem = new char[ lRealBytes ];
   if ( pchMem )
   {
      zPCHAR pvMem = pchMem;
      zmemset( pvMem, 0, lRealBytes );

      *((zPCHAR *) pvMem) = g_MemHead;    // next memory block
      (zPCHAR) pvMem += sizeof( zPVOID );
      *((zPLONG) pvMem) = lBytes;         // length of this memory block
      (zPCHAR) pvMem += sizeof( zPVOID );
      *((zPLONG) pvMem) = lGuard;         // first guard byte set
      (zPCHAR) pvMem += sizeof( zPVOID ) + lBytes;
      *((zPLONG) pvMem) = lGuard;         // second guard byte set

      // Now point to the application's data.
      pchMem += sizeof( zPCHAR ) + (2 * sizeof( zLONG ));
      *Memory = pchMem;
      g_MemHead = pchMem;

#ifdef zDEBUG_MEM
      if ( lGuard == 0xc8c8c8c8 )
      {
         TraceLineX( "DrAllocTaskMemory: ", (zLONG) pchMem );
      }
      else
      {
         zCHAR szMsg[ 128 ];
         sprintf_s( szMsg, zsizeof( szMsg ), "DrAllocTaskMemory: 0x%lx %d",
                   (zLONG) pchMem, lGuard );
         TraceLineS( szMsg, "" );
      }
#endif

      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( 0 );
   }
   else
   {
      *Memory = 0;
      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( zCALL_ERROR );
   }
}

zOPER_EXPORT zSHORT OPERATION
DrReAllocTaskMemory( zCOREMEM Memory, zLONG lBytes )
{
   SysMutexLock( g_vRegister, "ZDMem", 0, 0 );
   zPCHAR pchMem = *Memory - (sizeof( zPCHAR ) + sizeof( zLONG ));
   zLONG lOldBytes = *((zPLONG) pchMem);
   pchMem += sizeof( zLONG );
   zLONG lGuard = *((zPLONG) pchMem);
   if ( lOldBytes >= lBytes )
   {
      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( 0 );
   }

   zPCHAR pchMemoryNew;

   if ( DrAllocTaskMemory( (zCOREMEM) &pchMemoryNew, lBytes, lGuard ) == 0 )
   {
#ifdef zDEBUG_MEM
      if ( lGuard != 0xc8c8c8c8 )
         TraceLineI( "DrReAllocTaskMemory: ", lGuard );
#endif
      zmemcpy( pchMemoryNew, *Memory, lOldBytes );
      DrFreeTaskMemory( *Memory );
      *Memory = pchMemoryNew;
      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( 0 );
   }

   SysMutexUnlock( g_vRegister, "ZDMem", 0 );
   return( zCALL_ERROR );
}

zOPER_EXPORT zSHORT OPERATION
DrFreeTaskMemory( zPVOID Address )
{
   SysMutexLock( g_vRegister, "ZDMem", 0, 0 );

   // Check the guard bytes.
   zPCHAR pchMem = (zPCHAR) Address -
                            (sizeof( zPCHAR ) + (2 * sizeof( zLONG )));
   zLONG  lGuard = 0xc8c8c8c8;

#ifndef zDEBUG_MEM
   // If the guard bytes are screwed up ... no sense going on.
   if ( *((zPLONG) (pchMem + (2 * sizeof( zLONG )))) != lGuard )
   {
      TraceLineX( "DrFreeTaskMemory Cannot Free application memory "
                    "(possible underflow) - LEAK will result: ", (zLONG) Address );
      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( zCALL_ERROR );
   }
#endif

   zPCHAR pchMemCurr = pchMem;
   zPCHAR pchMemNext = 0;

   zLONG lBytes = 1;
   for ( zSHORT k = 0; k < 4; k++ )
   {
      if ( k == 0 )
         pchMemNext = *((zPCHAR *) pchMemCurr);
      else
      if ( k == 1 )
         lBytes = *((zPLONG) pchMemCurr);
      else
      {
         if ( *((zPLONG) pchMemCurr) != lGuard )
         {
            if ( k < 3 )
            {
               lGuard = *((zPLONG) pchMemCurr);
               goto NEXT_CHECK_label;
            }
            else
            if ( *((zPLONG) pchMemCurr) == 0xc8c8c8c8 )
            {
               TraceLineX( "(Drvr) Corrupted application memory underflow: ",
                           (zLONG) Address );
            }
            else
            if ( lGuard == 0xc8c8c8c8 )
            {
               TraceLineX( "(Drvr) Corrupted application memory overflow: ",
                           (zLONG) Address );
            }
            else
            {
               TraceLineX( "(Drvr) Corrupted application memory: ",
                           (zLONG) Address );
            }

            lBytes = 0;
            break;  // we have encountered an error
         }
      }

NEXT_CHECK_label:
      (zPCHAR) pchMemCurr += sizeof( zPVOID );
      if ( k == 2 )
         pchMemCurr += lBytes;
   }

   delete [] pchMem;

   if ( lBytes <= 1 )
   {
      TraceLineX( "DrFreeTaskMemory Error freeing:  ", (zLONG) Address );
      if ( lBytes == 0 )
         g_MemHead = 0;

      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( zCALL_ERROR );
   }
   else
   {
      if ( (zPCHAR) Address == g_MemHead )
      {
         g_MemHead = pchMemNext;
      }
      else
      {
         pchMem = g_MemHead;
         while ( pchMem )
         {
            if ( *((zPCHAR *) (pchMem - (sizeof( zPCHAR ) +
                              (2 * sizeof( zLONG ))))) == (zPCHAR) Address )
            {
               *((zPCHAR *) (pchMem - (sizeof( zPCHAR ) +
                                       (2 * sizeof( zLONG ))))) = pchMemNext;
               break;
            }
            else
            {
               pchMem = *((zPCHAR *) (pchMem - (sizeof( zPCHAR ) +
                                                   (2 * sizeof( zLONG )))));
            }
         }
      }

#ifdef zDEBUG_MEM
      if ( lGuard == 0xc8c8c8c8 )
         TraceLineX( "DrFreeTaskMemory  :  ", (zLONG) Address );
      else
      {
         zCHAR szMsg[ 128 ];
         sprintf_s( szMsg, zsizeof( szMsg ), "DrFreeTaskMemory: 0x%lx %d",
                   (zLONG) Address, lGuard );
         TraceLineS( szMsg, "" );
      }
#endif

      SysMutexUnlock( g_vRegister, "ZDMem", 0 );
      return( 0 );
   }
}

zOPER_EXPORT zPVOID OPERATION
DrLockTaskMemory( zPVOID Address )
{
   return( Address );
}

zOPER_EXPORT zPVOID OPERATION
DrUnlockTaskMemory( zPVOID Address )
{
   return( Address );
}

extern "C"
{
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Bootstrap functions for tzlodopr
//
#if 0

zSHORT OPERATION
oTZERSASO_SA_RelinkDelete( zVIEW vSAtobeRelinked0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZTENVRO_TE_RelinkDelete( zVIEW vTEtobeRelinked0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZZOLODO_LOD_RelinkDelete( zVIEW vLODtobeRelinked0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZWDLGSO_DialogRelinkDelete( zVIEW vDialogtobeRelinked0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZRPSRCO_ReportRelinkDelete( zVIEW vReportDef0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZADCSDO_DS_RelinkDelete( zVIEW vDialogSpec0 )
{
   return( 0 );
}

zSHORT OPERATION
ConvertIntegerToString( zLONG lnIntegerValue0,
                        zPCHAR sszStringValue1,
                        zLONG lnLength2 )
{
   return( 0 );
}

zLONG OPERATION
GetStringLength( zPCHAR sszStringValue0 )
{
   return( 0 );
}

zSHORT OPERATION
oTZDMSRCO_ReplaceOrAddDomToXDM( zVIEW vDomainGrp0,
                                zSHORT nCommitFlag0or11 )
{
   return( 0 );
}

zSHORT OPERATION
MergeGlobalPrototypes( zVIEW vvMeta0,
                       zPCHAR sszMetaName1,
                       zPCHAR sszRootEntityName2,
                       zVIEW vvTaskLPLR3 )
{
   return( 0 );
}

zSHORT OPERATION
zgGetZeidonToolsDir( zPCHAR szFullDir,
                     zSHORT nDirType )
{
   return( 0 );
}

zSHORT OPERATION
oTZZOXODO_SaveXOD( zVIEW vTZZOLODO )
{
   return( 0 );
}

zSHORT OPERATION
oTZ__PRFO_GetViewToProfile( zVIEW  vSubtask,
                            zPVIEW pvProfile,
                            zPCHAR pchToolID,
                            zSHORT nCurrentOrReloadFlag )
{
   return( 0 );
}

// TZEDFRMD
zSHORT OPERATION
CleanupLibrary( zVIEW vSubtask )
{
   return( 0 );
}

// TZOBRWAD
zSHORT OPERATION
StageSingleTask( zVIEW vSubtask, zVIEW vViewToStage )
{
   return( 0 );
}

// TZCTL
class TZControlBar;

zLONG  OPERATION
PainterCall( zSHORT nMessage,
             zVIEW  vTZPNTROO,
             TZControlBar *pControlBar,
             zVIEW  vDialog,
             zPCHAR szParm,
             zLONG  lMaxLth )
{
   return( 0 );
}
#endif

//
// End of: Bootstrap functions for tzlodopr
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
}
