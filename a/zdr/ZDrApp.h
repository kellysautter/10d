/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrapp.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definition for ZDrApp (CWinApp).
// Class definition for ZSubtask (each corresponds with a primary window)
// Class definition for ZMapAct (each corresponds to a control)
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//
// 2001.09.24    DKS   Bianca
//    Altered timing of NoteBook Ctrl Mapping.
//
// 2001.05.15    BL    WEB
//    Added SetUpdateFlagForAttribute
//
// 2001.05.03    DKS   WEB
//    Phase II of Delta Data.  One more phase left to implement.
//
// 2001.04.19    DKS   Z10
//    Change to use MapRect units as base for conversion to dialog units.
//    Added new operations.
//
// 2001.04.11    DKS   RemoteServer
//    Set up for Delta Data project.
//
// 2001.04.10    TMV   Z2000
//   Overload method OnMouseActivate for ZDrView
//
// 2000.08.07    RG    Z2000
//   Changed declaration of fontsize from zSHORT to zLONG to remove
//   compiler warnings in Zeidon Tools
//
// 1999.11.23    TMV   Z10    ActiveX
//   Fixing Memory leaks of CBitmaps.
//   implement ZEventList as derived from CObList
//             ZEventItem as derived from CObject
//             ZParamItem as derived from CObject
//   to avoid multiple list implementations and to fix an endless loop
//   when destroying list ZParamItems
//   Implement ambient properties
//
// 1999.04.19    RS
//   Commentlines required for Rose.
//
// 1999.04.16    RS
//    includes required for Rose
//
// 1998.09.02    DKS
//    Removed ZDRCMDLN.H (became part of zdrapp.h)
//
// 1998.08.26    RG - 1998.08.31  DKS
//    Definition of class ZCommandLineInfo for 16 and 32 bit. If it is only
//    defined for 16 bit, we get a link error: unresolved external ... we
//    are compiling and linking without the class in 32-bit (DKS).
//
// 1998.07.23    TMV
//    Make changes so that a 16-Bit Runtime can be built from MFC 5.0 Sources
//

// Don't remove the following commentlines. They are required for Rose.
//#include <afxtempl.h> // required for Rose
//#include <kzoessaa.h> // required for Rose
//#include <zdrvropr.h> // required for Rose
//#include <kzwdlgxo.hst> // required for Rose

#ifndef __zdrapp_h   // Sentry, use file only if it's not already included
#define __zdrapp_h

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

#pragma warning(disable: 4996) // This function or variable may be unsafe
#pragma warning(disable: 4297) // function assumed not to throw an exception but does

#include "ZDrTwain.h"      // Standard Twain definitions
#include "occimpl.h"       // for OnAmbientProperty methods
#include "resource.h"      // main symbols

// #define DEBUG_LISTWND

/////////////////////////////////////////////////////////////////////////////
// Note: Use AFX_EXT_CLASS to export an entire class.  This avoids
// creating a .DEF file with all the decorated names for the class.
// Creating a .DEF file is more efficient since the names can be
// exported by ordinal.
//
// Here we use that method of exporting, much like MFC does itself, by
// implementing the following two lines and adding the appropriate lines
// to our .DEF file. All the public interfaces are exported in ZDrApp.def.
//
#ifndef ZDR_CLASS
#define ZDR_CLASS
#endif

#ifndef ZDRAPP_CLASS
   #define ZDRAPP_CLASS _declspec(dllimport)
#endif

extern "C" void WINAPI
ZeidonInitZDRAPP( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,
                  zLONG lMapDlgCharX, zLONG lMapDlgCharY );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//
//    Zeidon primary window
//    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//    ³      CWnd       ³
//    ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ
//    ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿            ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//    ³    CFrameWnd    ³            ³     ZSubtask    ³
//    ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ            ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ
//             ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ <---------> ÄÙ
//                   ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
//                   ³     ZDrFrame    ³
//                   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//
//
//    Zeidon controls (e.g.  Edit control)
//    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//    ³      CWnd       ³
//    ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ
//    ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿            ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
//    ³      CEdit      ³            ³     ZMapAct     ³
//    ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ            ÀÄÄÄÄÄÄÄÄÂÄÄÄÄÄÄÄÄÙ
//             ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÂÄ <---------> ÄÙ
//                   ÚÄÄÄÄÄÄÄÄÁÄÄÄÄÄÄÄÄ¿
//                   ³      ZEdit      ³
//                   ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// #define DEBUG_ALL

/////////////////////////////////////////////////////////////////////////////
// This class is basically just an array of 1024 strings,
// the names of each WM_ message. Constructor initializes it.
//
class ZDRAPP_CLASS ZWndMsgMap
{
   static LPCTSTR Names[];             // array of WM_ message names
public:
   ZWndMsgMap( );                      // constructor initializes them
   CString GetMsgName( UINT uMsg );    // get name of message
};

/////////////////////////////////////////////////////////////////////////////
// Classes defined in other executables
//
class ZDR_CLASS ZDrApp;
class ZDRAPP_CLASS ZCommandLineInfo;

/////////////////////////////////////////////////////////////////////////////
// Classes defined in this header file
//
class ZDR_CLASS ZDxFrame;

class ZDRAPP_CLASS ZTask;
class ZDRAPP_CLASS ZSubtask;
class ZDRAPP_CLASS ZDrFrame;
class ZDRAPP_CLASS ZMapAct;
class ZDRAPP_CLASS ZOption;
class ZDRAPP_CLASS ZSocketTask;
class ZDRAPP_CLASS ZPageItem;
class ZDRAPP_CLASS ZDrDoc;
class ZDRAPP_CLASS ZDrView;
class ZDRAPP_CLASS ZPrintout;
class ZDRAPP_CLASS ZTwain;
class ZDRAPP_CLASS ZMultiIdxSet;
class ZDRAPP_CLASS ZDrMemory;

#define zLOUSHORT( l )         ((zUSHORT)(zULONG) ( l ))
#define zHIUSHORT( l )         ((zUSHORT)((((zULONG)( l )) >> 16) & 0xFFFF))
#define zMAKELONG( low, high ) ((zULONG)(((zUSHORT)( low )) | (((zULONG)((zUSHORT)( high ))) << 16)))

#define zAPP_STATE_INIT                   (zUSHORT) 0x0000
#define zAPP_STATE_TERMINATED             (zUSHORT) 0x0001
#define zAPP_STATE_NT                     (zUSHORT) 0x0002
#define zAPP_STATE_INITIALIZED            (zUSHORT) 0x0008
#define zAPP_STATE_PRINT                  (zUSHORT) 0x1000

#define zAUTO_ENABLE                      (char) 16
#define zAUTO_DISABLE                     (char) -16

//
// ID Ranges
// ---------
// By convention, the ID value of 0 is not used.  Windows implementation
// limitations restrict true resource IDs to be less than or equal to 0x7FFF.
// MFC's internal framework implementations reserve several ranges:
// 0xE000->0xEFFF and 0x7000->0x7FFF.  Several Windows system commands use
// the range of 0xF000 -> 0xFFFF.  Control IDs of 1->7 are reserved by IDOK,
// IDCANCEL, and so on.  The range of 0x8000->0xFFFF for strings is reserved
// for menu prompts for commands.
//
// Please refer to Technical Note 20 for more details on the use of ID
// ranges in MFC.
//
// MFC standard commands fall in the range 0xE000 to 0xEFFF. Please do
// not rely on the specific values of these IDs since they are subject
// to change in future versions of the library.
//
// Your application should define its commands in the range 0x8000 to 0xDFFF.
//
#define zZEIDON_DRIVER_MSG                                   0x4000
#define zZEIDON_COMMUNICATE_OE              (zZEIDON_DRIVER_MSG + 1)
#define zZEIDON_POSTED_CLOSE                (zZEIDON_DRIVER_MSG + 2)
#define zZEIDON_PROCESS_EVENT               (zZEIDON_DRIVER_MSG + 3)
#define zZEIDON_PROCESS_ACTION              (zZEIDON_DRIVER_MSG + 4)
#define zZEIDON_PROCESS_HOTKEY              (zZEIDON_DRIVER_MSG + 5)
#define zZEIDON_RESUME_FROM_PAINTER         (zZEIDON_DRIVER_MSG + 6)
#define zZEIDON_BEGIN_DRAG                  (zZEIDON_DRIVER_MSG + 7)
#define zZEIDON_QUERY_DROP                  (zZEIDON_DRIVER_MSG + 8)
#define zZEIDON_DO_DROP                     (zZEIDON_DRIVER_MSG + 9)
#define zZEIDON_PROCESS_TAB                 (zZEIDON_DRIVER_MSG + 10)
#define zZEIDON_PRINT_PREVIEW_PREV          (zZEIDON_DRIVER_MSG + 11)
#define zZEIDON_PRINT_PREVIEW_NEXT          (zZEIDON_DRIVER_MSG + 12)
#define zZEIDON_PRINT_PREVIEW_ONEUP         (zZEIDON_DRIVER_MSG + 13)
#define zZEIDON_PRINT_PREVIEW_TWOUP         (zZEIDON_DRIVER_MSG + 14)
#define zZEIDON_PRINT_PREVIEW_DONE          (zZEIDON_DRIVER_MSG + 15)
#define zZEIDON_PRINT_PREVIEW_CURR          (zZEIDON_DRIVER_MSG + 16)
#define zZEIDON_THREAD_INIT                 (zZEIDON_DRIVER_MSG + 17)
#define zZEIDON_THREAD_READY                (zZEIDON_DRIVER_MSG + 18)
#define zZEIDON_THREAD_COMPLETE             (zZEIDON_DRIVER_MSG + 19)
#define zZEIDON_THREAD_TERM                 (zZEIDON_DRIVER_MSG + 20)
#define zZEIDON_CAPTURE_MOUSE               (zZEIDON_DRIVER_MSG + 21)
#define zZEIDON_GET_TOOLTIP_TEXT            (zZEIDON_DRIVER_MSG + 22)
#define zZEIDON_FIND_DIALOG_WINDOW          (zZEIDON_DRIVER_MSG + 23)
#define zZEIDON_SET_WINDOW_STATE            (zZEIDON_DRIVER_MSG + 24)
#define zZEIDON_SET_WAB                     (zZEIDON_DRIVER_MSG + 25)
#define zZEIDON_POSTED_ACTIVATE             (zZEIDON_DRIVER_MSG + 26)
#define zZEIDON_POSTED_INVALIDATE           (zZEIDON_DRIVER_MSG + 27)
#define zZEIDON_REMOTE_SOCKET               (zZEIDON_DRIVER_MSG + 40)
#define zZEIDON_REMOTE_GETWND               (zZEIDON_DRIVER_MSG + 41)
#define zZEIDON_REMOTE_PRINT_REPORT         (zZEIDON_DRIVER_MSG + 42)
#define zZEIDON_POSTED_POPUP_MENU           (zZEIDON_DRIVER_MSG + 43)
#define zZEIDON_TOOLBAR_DROPDOWN            (zZEIDON_DRIVER_MSG + 44)
#define zZEIDON_TOOLBAR_SET_SELECT          (zZEIDON_DRIVER_MSG + 45)
#define zZEIDON_POSTED_SPLITTER_CLOSE       (zZEIDON_DRIVER_MSG + 46)
#define zZEIDON_OE_SHAREMEM                 (zZEIDON_DRIVER_MSG + 47)
#define zZEIDON_POSTED_MARK_DELETE          (zZEIDON_DRIVER_MSG + 48)
#define zZEIDON_CTRL_MSG                    (zZEIDON_DRIVER_MSG + 0x08FF)
#define zZEIDON_LAST_RESERVED               (zZEIDON_DRIVER_MSG + 0x4FFF)
#define zZEIDON_APPLICATION_MSG              0x5000
#define zZEIDON_LAST_APP_RESERVED            0x5FFF

#define zPOINTER_ARRAY_ERROR  -1

// 224 command id's should be enough
#define zZEIDON_FIRST_TOOLBAR_CMD     AFX_IDW_CONTROLBAR_FIRST + 32
#define zZEIDON_LAST_TOOLBAR_CMD      AFX_IDW_CONTROLBAR_LAST

// 16 ToolBar type controls should be enough
#define zZEIDON_FIRST_TOOLBAR         AFX_IDW_DOCKBAR_TOP + 32
#define zZEIDON_LAST_TOOLBAR          zZEIDON_FIRST_TOOLBAR + 15

#define zZEIDON_TOOLTIP_TEXTLTH          80
#define zNO_APPLICATION_EVENT       9999999

// Returned when no more code is to be executed after handling an event
// (in Wrapper - DLL for ActiveX).
#define zACTIVEX_EVENT_COMPLETED                0x00000000

// The opposite of zACTIVEX_EVENT_COMPLETED ... we must execute user
// provided code.
#define zACTIVEX_EVENT_CONTINUE                 ~zACTIVEX_EVENT_COMPLETED

// WIDTHBYTES performs DWORD-aligning of DIB scanlines.  The "bits"
// parameter is the bit count for the scanline (biWidth * biBitCount),
// and this macro returns the number of DWORD-aligned bytes needed
// to hold those bits.
// Macro to round off the given value to the closest byte.

#define WIDTHBYTES( bits ) (((bits) + 31) / 32 * 4)

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZMultiIdxSet.
//
// In the course of building a tree or sorted list, it will be necessary
// to compare whatever objects are to be stored in it.  Since it is not
// known at this stage what these will be, we define a type for a pointer
// to a user supplied function.  A similar typedef is required for the
// user supplied function which determines what is done when a tree node
// is visited inorder.  Finally, a function to copy nodes (called from the
// copy ctor) can be specified.
typedef zSHORT (* fnCOMPARE)( const void *, const void * );
typedef void   (* fnVISIT_INORDER)( void *, WPARAM, LPARAM );
typedef void * (* fnCOPY)( void * );

#define zTAG_LTH  33
#define zINCR_NODE_CNT  64

enum ZMultiIdxSet_errors
{
    zMIXSET_INVALID_PTR = -10,
    zMIXSET_DUP,
    zMIXSET_NOTFND,
    zMIXSET_OK = 0
};

class ZDRAPP_CLASS ZOrderIndex
{
friend class ZMultiIdxSet;
private:
   ZOrderIndex( fnCOMPARE pfnCompare,
                fnCOMPARE pfnSeek );
   virtual ~ZOrderIndex( );

   fnCOMPARE m_pfnCompare;
   fnCOMPARE m_pfnSeek;
   ZOrderIndex *m_pNextOrderIndex;
   zPLONG    m_plIdx;          // pointer to integer ordering index array
   zLONG     m_lIdxCnt;        // ordering index array size
   zBOOL     m_bDirty;
};

class ZDRAPP_CLASS ZMultiIdxSet
{

public:
   // There is only one constructor taking five arguments:
   // 1. Must point to the function which is to be used
   //    to compare the objects to be stored in the list.
   // 2. Must point to the function which is to be used
   //    to compare the objects while seeking through the list.
   // 3. Initial maximum number of nodes to be supported
   // 4. Pointer to the function used to control the cleanup behavior
   //    of the list:
   //    - 0      ==> only the node list and index are deleted.
   //    - otherwise ==> the destructor will call this function for each
   //                    of the attached data objects in the list and then
   //                    delete the node list.
   // 5. Pointer to the function called to copy attached data objects.
   //
   ZMultiIdxSet( fnCOMPARE pfnCompare = 0,
                 fnCOMPARE pfnSeek = 0,
                 zLONG     lMaxNodeCnt = 0,
                 fnVISIT_INORDER pfnPurge = 0,
                 fnCOPY    pfnCopy = 0 );

   ZMultiIdxSet( const ZMultiIdxSet& rmix ); // copy ctor
   virtual ~ZMultiIdxSet( );

   zLONG   GetNodeIndex( zCPVOID pvNode );
   zPVOID  GetAt( zLONG  lPos );
   zLONG   InsertNode( zPVOID pvNode,
                       zPVOID pvOrderNode = 0,
                       zCHAR chPos = 0 );
   POSITION AddHead( zPVOID pNode );
   POSITION AddTail( zPVOID pNode );
   POSITION FindIndex( zLONG lPos );
   POSITION GetHeadPosition( );
   zPVOID  GetNext( POSITION& pos );
   zPVOID  SeekNode( zCPVOID pvData );

   zLONG   RemoveNode( zCPVOID pvNode );
   void    ResetContent( );
   zSHORT  GetError( );

   zBOOL   AddOrderIndex( zSHORT nId,
                          fnCOMPARE pfnCompare,
                          fnCOMPARE pfnSeek );
   zBOOL   UseOrderIndex( zSHORT nId );  // specify which index to use
   zBOOL   RemoveOrderIndex( zSHORT nId );

   void    VisitInorder( fnVISIT_INORDER pfnVisit,
                         zSHORT nSortedOrder = 1,
                         WPARAM wParam = 0,
                         LPARAM lParam = 0 );
   void    SetCopyFunction( fnCOPY pfnCopyNode );
   inline  zLONG GetCount( ) const { return( m_lNodeCnt ); }
   inline  zLONG GetMaxCount( ) const { return( m_lMaxNodes ); }

   zPVOID    m_pvNodeErr;       // first node in error during visit inorder
   zPVOID    *m_pvNode;         // pointer to array of data objects pointers

protected:
   zBOOL   RebuildOrderIndex( ZOrderIndex *pOrderIndex );

   zLONG     m_lNodeCnt;        // current number of objects in the list
   zLONG     m_lMaxNodes;       // maximum number of objects in the list
   fnVISIT_INORDER m_pfnVisit;
   fnVISIT_INORDER m_pfnPurge;  // if set, delete nodes when deleting class
   fnCOPY    m_pfnCopy;         // if set, call from copy ctor to copy nodes
   ZOrderIndex *m_pHeadIndex;   // head (last) of list of order indexes
   ZOrderIndex *m_pCurrIndex;   // order index in use
   zSHORT    m_nIdxCnt;         // number of indexes defined for this list
   zSHORT    m_nErrorCode;      // last error code
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#if 1  // until I figure out templates better ... just hardcode list classes

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZPtrArray.
//

class ZDRAPP_CLASS ZPtrArray   // pointer array class
{
public:
   ZPtrArray( zSHORT nInitialPointers = 16,
              zSHORT nExpansionIncrement = 8,
              ZSubtask **pBufferArray = 0 );
   virtual ~ZPtrArray( );

   zLONG  Insert( ZSubtask *pZSubtask ); // returns index where inserted (-1 if error)
   zLONG  Find( ZSubtask *pZSubtask );   // returns index where located (-1 if error)
   zLONG  Detach( ZSubtask *pZSubtask ); // returns index where found (-1 if error)
   zLONG  GetSize( );           // returns current count of pointers in array
   ZSubtask * operator []( zLONG lIdx ) const;   // returns zero on error

private:
   zLONG  m_lCnt;             // current count of pointers in the array
   zLONG  m_lMax;             // maximum pointers allocated in the array
   zSHORT m_nIncr;            // array allocation expansion increment
   zBOOL  m_bLocalBuffer;     // array buffer allocated within this class
   ZSubtask **m_pArrayBuffer; // buffer containing array of pointers
};

#else

template <class T> class ZDRAPP_CLASS ZPtrArray   // pointer array class
{
public:
   ZPtrArray( zSHORT nInitialPointers = 16,
              zSHORT nExpansionIncrement = 8,
              T *pBufferArray = 0 );
   virtual ~ZPtrArray( );

   zSHORT Insert( T pT );     // returns index where inserted (-1 if error)
   zSHORT Find( T pT );       // returns index where located (-1 if error)
   zSHORT Detach( T pT );     // returns index where found (-1 if error)
   zSHORT GetSize( );         // returns current count of pointers in array
   T operator []( zSHORT nIdx ) const;   // returns zero on error

private:
   zSHORT m_nCnt;             // current count of pointers in the array
   zSHORT m_nMax;             // maximum pointers allocated in the array
   zSHORT m_nIncr;            // array allocation expansion increment
   zBOOL  m_bLocalBuffer;     // array buffer allocated within this class
   T      *m_pArrayBuffer;    // buffer containing array of pointers
};

#endif

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZNumberList (simple single-linked list).
//
class ZDRAPP_CLASS ZNumberItem
{
public:
   ZNumberItem( zLONG lNumber, zLONG lFlag = 0 );
   ~ZNumberItem( );

public:
   ZNumberItem *m_pNextNumber;
   zLONG       m_lNumber;
   zLONG       m_lFlag;
};

class ZDRAPP_CLASS ZNumberList
{
public:
   ZNumberList( zBOOL bUnique = TRUE );
   ~ZNumberList( );

   ZNumberItem * AddHead( zLONG lNumber, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZNumberItem * AddTail( zLONG lNumber, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZNumberItem * Find( zLONG lNumber );     // return ptr if found ... 0 otw
   ZNumberItem * Remove( zLONG lNumber );   // return ptr if removed ... 0 otw
   ZNumberItem * MoveHead( zLONG lNumber ); // return ptr if found ... 0 otw
   ZNumberItem * DropHead( );               // return ptr if removed ... 0 otw
   ZNumberItem * DropTail( );               // return ptr if removed ... 0 otw
   ZNumberItem * GetHead( ) { return( m_pHeadNumber ); }
   ZNumberItem * GetTail( ) { return( m_pTailNumber ); }

   ZNumberItem *m_pHeadNumber;
   ZNumberItem *m_pTailNumber;
   zBOOL m_bUnique;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZNameList (simple single-linked list).
//
class ZDRAPP_CLASS ZNameItem
{
public:
   ZNameItem( zCPCHAR cpcName, zLONG lFlag = 0 );
   ~ZNameItem( );

public:
   ZNameItem *m_pNextName;
   CString   m_csName;
   zLONG     m_lFlag;
};

class ZDRAPP_CLASS ZNameList
{
public:
   ZNameList( zBOOL bUnique = TRUE );
   ~ZNameList( );

   ZNameItem * AddHead( zCPCHAR cpcName, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZNameItem * AddTail( zCPCHAR cpcName, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZNameItem * Find( zCPCHAR cpcName );     // return ptr if found ... 0 otw
   ZNameItem * Remove( zCPCHAR cpcName );   // return ptr if removed ... 0 otw
   ZNameItem * MoveHead( zCPCHAR cpcName ); // return ptr if found ... 0 otw
   ZNameItem * DropHead( );                 // return ptr if removed ... 0 otw
   ZNameItem * DropTail( );                 // return ptr if removed ... 0 otw
   ZNameItem * GetHead( ) { return( m_pHeadName ); }
   ZNameItem * GetTail( ) { return( m_pTailName ); }

   ZNameItem *m_pHeadName;
   ZNameItem *m_pTailName;
   zBOOL m_bUnique;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZPointerList (simple single-linked list).
//
class ZDRAPP_CLASS ZPointerItem
{
public:
   ZPointerItem( zPVOID pvPointer, zLONG lFlag = 0 );
   ~ZPointerItem( );

public:
   ZPointerItem *m_pNextPointer;
   zPVOID       m_pvPointer;
   zLONG        m_lFlag;
};

class ZDRAPP_CLASS ZPointerList
{
public:
   ZPointerList( zBOOL bUnique = TRUE );
   ~ZPointerList( );

   ZPointerItem * AddHead( zPVOID pvPointer, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZPointerItem * AddTail( zPVOID pvPointer, zLONG lFlag = 0 );  // return ptr if added ... 0 otw
   ZPointerItem * Find( zPVOID pvPointer );     // return ptr if found ... 0 otw
   ZPointerItem * Remove( zPVOID pvPointer );   // return ptr if removed ... 0 otw
   ZPointerItem * MoveHead( zPVOID pvPointer ); // return ptr if found ... 0 otw
   ZPointerItem * DropHead( );                  // return ptr if removed ... 0 otw
   ZPointerItem * DropTail( );                  // return ptr if removed ... 0 otw
   ZPointerItem * GetHead( ) { return( m_pHeadPointer ); }
   ZPointerItem * GetTail( ) { return( m_pTailPointer ); }

   ZPointerItem *m_pHeadPointer;
   ZPointerItem *m_pTailPointer;
   zBOOL m_bUnique;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZPairedList (simple single-linked list).
//
class ZDRAPP_CLASS ZPairedItem
{
public:
   ZPairedItem( zLONG lValue1, zLONG lValue2,
                zLONG lValue3 = 0, zLONG lValue4 = 0 );
   ~ZPairedItem( );

public:
   ZPairedItem *m_pNextItem;
   zLONG       m_lValue1;
   zLONG       m_lValue2;
   zLONG       m_lValue3;
   zLONG       m_lValue4;
};

class ZDRAPP_CLASS ZPairedList
{
public:
   ZPairedList( );
   ~ZPairedList( );

   void Cleanup( );
   void Display( zCPCHAR cpcText );

   ZPairedItem * Add( zLONG lValue1,
                      zLONG lValue2,
                      zLONG lValue3 = 0,
                      zLONG lValue4 = 0 ); // return ptr if added ... 0 otw
   ZPairedItem * Find( zLONG lValue1,      // check Value1 if (lFlag & 1)
                       zLONG lValue2,      // check Value2 if (lFlag & 2)
                       zLONG lValue3 = 0,  // check Value3 if (lFlag & 4)
                       zLONG lValue4 = 0,  // check Value4 if (lFlag & 8)
                       zLONG lFlag = 15 ); // return ptr if found ... 0 otw
   ZPairedItem *
     Remove( ZPairedItem *pPairedItem ); // return ptr if removed ... 0 otw
   ZPairedItem * DropHead( );            // return ptr if found ... 0 otw

   ZPairedItem *m_pHeadItem;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZCorrelatedList (simple single-linked list).
//
class ZDRAPP_CLASS ZCorrelatedItem
{
public:
   ZCorrelatedItem( zLONG lNumber, zCPCHAR cpcValue );
   ZCorrelatedItem( zCPCHAR cpcKey, zCPCHAR cpcValue );
   ~ZCorrelatedItem( );

public:
   ZCorrelatedItem *m_pNextItem;
   zLONG       m_lNumber;
   CString     *m_pcsKey;
   CString     m_csValue;
};

class ZDRAPP_CLASS ZCorrelatedList
{
public:
   ZCorrelatedList( );
   ~ZCorrelatedList( );

   ZCorrelatedItem * Add( zLONG   lNumber,
                          zCPCHAR cpcValue );    // return ptr if added ... 0 otw
   ZCorrelatedItem * Add( zCPCHAR cpcKey,
                          zCPCHAR cpcValue );    // return ptr if added ... 0 otw
   ZCorrelatedItem * Find( zLONG lNumber );      // return ptr if found ... 0 otw
   ZCorrelatedItem * Find( zCPCHAR cpcValue, zBOOL bKey = FALSE );  // return ptr if found ... 0 otw
   ZCorrelatedItem * Remove( zLONG lNumber );    // return ptr if removed ... 0 otw
   ZCorrelatedItem * Remove( zCPCHAR cpcValue, zBOOL bKey = FALSE ); // return ptr if removed ... 0 otw

   ZCorrelatedItem *m_pHeadItem;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZDualCorrelatedList (simple single-linked list).
//
class ZDRAPP_CLASS ZDualCorrelatedItem
{
public:
   ZDualCorrelatedItem( zLONG lKey, zLONG lValue, zCPCHAR cpcKey, zCPCHAR cpcValue );
   ~ZDualCorrelatedItem( );

public:
   ZDualCorrelatedItem *m_pNextItem;
   zLONG       m_lKey;
   zLONG       m_lValue;
   CString     *m_pcsKey;
   CString     *m_pcsValue;
};

class ZDRAPP_CLASS ZDualCorrelatedList
{
public:
   ZDualCorrelatedList( );
   ~ZDualCorrelatedList( );

   void Cleanup( );
   ZDualCorrelatedItem * Add( zLONG   lKey,
                              zLONG   lValue,
                              zCPCHAR cpcKey,
                              zCPCHAR cpcValue );           // return ptr if added ... 0 otw
   ZDualCorrelatedItem * FindKey( zLONG lKey  );            // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindKey( zCPCHAR cpcKey );         // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindKey( zLONG   lKey,
                                  zCPCHAR cpcKey );         // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindValue( zLONG lValue );         // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindValue( zCPCHAR cpcValue );     // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindValue( zLONG   lValue,
                                    zCPCHAR cpcValue );     // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextKey( zLONG lKey );         // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextKey( zCPCHAR cpcKey );     // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextKey( zLONG   lKey,
                                      zCPCHAR cpcKey );     // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextValue( zLONG lValue );     // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextValue( zCPCHAR cpcValue ); // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNextValue( zLONG   lValue,
                                        zCPCHAR cpcValue ); // return ptr if found ... 0 otw
   ZDualCorrelatedItem * RemoveByKey( zLONG lKey );         // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * RemoveByKey( zCPCHAR cpcKey );     // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * RemoveByKey( zLONG   lKey,
                                      zCPCHAR cpcKey );     // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * RemoveByValue( zLONG lValue );     // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * RemoveByValue( zCPCHAR cpcValue ); // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * RemoveByValue( zLONG   lValue,
                                        zCPCHAR cpcValue ); // return ptr if removed ... 0 otw
   ZDualCorrelatedItem * Find( zLONG   lKey,                // check lKey if (lFlag & 1)
                               zLONG   lValue,              // check lValue if (lFlag & 2)
                               zCPCHAR cpcKey,              // check cpcKey if (lFlag & 4)
                               zCPCHAR cpcValue,            // check cpcValue if (lFlag & 8)
                               zLONG   lFlag = 15 );        // return ptr if found ... 0 otw
   ZDualCorrelatedItem * FindNext( zLONG   lKey,            // check lKey if (lFlag & 1)
                                   zLONG   lValue,          // check lValue if (lFlag & 2)
                                   zCPCHAR cpcKey,          // check cpcKey if (lFlag & 4)
                                   zCPCHAR cpcValue,        // check cpcValue if (lFlag & 8)
                                   zLONG   lFlag = 15 );    // return ptr if found ... 0 otw
   ZDualCorrelatedItem * Remove( zLONG   lKey,              // check lKey if (lFlag & 1)
                                 zLONG   lValue,            // check lValue if (lFlag & 2)
                                 zCPCHAR cpcKey,            // check cpcKey if (lFlag & 4)
                                 zCPCHAR cpcValue,          // check cpcValue if (lFlag & 8)
                                 zLONG   lFlag = 15 );      // return ptr if found ... 0 otw

   ZDualCorrelatedItem *m_pHeadItem;
   ZDualCorrelatedItem *m_pCurrItem;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZColorList (simple single-linked list).
//

#define zNUMERIC_COMPARE 0x80000000    // vs. string compare
#define zIGNORE_CASE     0x20000000    // case insensitive string compare
#define zTEST_BOTH       0x40000000    // test long and string values
                                       // (not just string value)
#define zALWAYS_EQUAL    0x00800000    // compare always returns equal
#define zLT              0x01000000    // less than
#define zLE              0x02000000    // less than or equal
#define zEQ              0x00000000    // equal (default)
#define zGT              0x08000000    // greater than
#define zGE              0x04000000    // greater than or equal
#define zNE              0x00400000    // not equal
#define zNZ              0x00200000    // null or zero
#define zNN              0x00100000    // not null/zero
#define zIN              0x00080000    // in (contains)
#define zNI              0x00040000    // not in (does not contain)
#define zSW              0x00020000    // starts with
#define zNS              0x00010000    // does not start with

#define zBOLD            0x00000001    // bold
#define zITALIC          0x00000002    // italic
#define zUNDERLINE       0x00000004    // underline
#define zSTRIKEOUT       0x00000008    // strikeout

class ZDRAPP_CLASS ZColorItem
{
public:
   ZColorItem( zLONG lColumn, zCPCHAR cpcValue,
               zLONG lClrText, zLONG lClrBack, zLONG lFlags );
   ~ZColorItem( );

public:
   ZColorItem *m_pNextItem;
   zLONG       m_lColumn;
   CString     m_csValue;
   zLONG       m_lClrText;
   zLONG       m_lClrBack;
   zLONG       m_lClrSelText;
   zLONG       m_lClrSelBack;
   zLONG       m_lClrFlags;
};

typedef zSHORT (POPERATION zFARPROC_ITEMCOMPARE)( ZColorItem *pColorItem,
                                                  zLONG      lColumn,
                                                  zCPCHAR    cpcValue,
                                                  zBOOL      bTestBoth );

extern "C" zSHORT OPERATION
CompareCell( ZColorItem *pColorItem, zLONG lColumn,
             zCPCHAR cpcValue, zBOOL bTestBoth );

class ZDRAPP_CLASS ZColorList
{
public:
   ZColorList( zFARPROC_ITEMCOMPARE lpfnCompare = 0 );
   ~ZColorList( );

   ZColorItem * Add( zLONG   lColumn,
                     zCPCHAR cpcValue,
                     zLONG   lClrText,
                     zLONG   lClrBack,
                     zLONG   lFlags );      // return ptr if added ... 0 otw
   ZColorItem * Find( zLONG   lColumn,
                      zCPCHAR cpcValue,
                      zBOOL   bTestBoth );  // return ptr if found ... 0 otw
   ZColorItem * FindNext( zLONG   lColumn,
                          zCPCHAR cpcValue,
                          zBOOL   bTestBoth ); // return ptr if found ... 0 otw
   ZColorItem * Remove( zLONG   lColumn,
                        zCPCHAR cpcValue ); // return ptr if removed ... 0 otw

   ZColorItem           *m_pHeadItem;
   ZColorItem           *m_pCurrItem;
   zFARPROC_ITEMCOMPARE m_lpfnCompare;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZXRA_List (simple single-linked list).
//
class ZDRAPP_CLASS ZXRA_Item
{
public:
   ZXRA_Item( zCPCHAR cpcName, zVIEW vXRA );
   ~ZXRA_Item( );

   void   ActivateEmptyDelta( zVIEW vSubtask, zCPCHAR cpcAppTag );

public:
   ZXRA_Item *m_pNext;
   CString   m_csName;
   zBOOL     m_bDefSent;
   zVIEW     m_vXRA_Def;             // XRW Definition for this window
   zVIEW     m_vXRA;                 // Complete XRW subtask view (with data)
   zVIEW     m_vXRA_Delta;           // Outgoing (to LightClient) XRW delta
   ZSubtask  *m_pZSubtask;           // Pointer to related ZSubtask
};

class ZDRAPP_CLASS ZXRA_List
{
public:
   ZXRA_List( ZSocketTask *pZSocketTask );
   ~ZXRA_List( );

   ZXRA_Item * Add( zCPCHAR cpcName,
                    ZSubtask *pZSubtask,
                    zVIEW vXRA_Def );      // return ptr if added ... 0 otw
   ZXRA_Item * Find( zCPCHAR cpcName,
                     ZSubtask *pZSubtask,
                     zCPCHAR cpcLabel );   // return ptr if found ... 0 otw
   ZXRA_Item * Remove( zCPCHAR cpcName );  // return ptr if removed ... 0 otw
   ZXRA_Item * Remove( ZXRA_Item *pXRA );  // return ptr if removed ... 0 otw
   void   DeleteEmpty( );
   void   ResetSubtasks( );
   zSHORT CountViews( zVIEW *pvOI );
   void   ListViews( );
   void   SetDeltas( zBOOL bFromXRA );
   void   DropObjectInstances( );

   ZSocketTask *m_pZSocketTask;
   ZXRA_Item *m_pHead;
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZEventList (simple single-linked list).
//
class ZDRAPP_CLASS ZParmItem : public CObject
{
   friend class ZActiveX;
   friend class ZEventItem;
public:
   ZParmItem( zLONG lID, CString csValue );
   ~ZParmItem( );

   DISPID GetID( ) { return m_lID; }
   CString GetValue( ) { return m_csValue; }

private:

// ZParmItem *m_pNextParm;
   DISPID    m_lID;
   CString   m_csValue;
};

class ZDRAPP_CLASS ZEventItem : public CObject
{
   friend class ZEventList;
public:
   ZEventItem( DISPID dispid );
   ~ZEventItem( );

   ZParmItem * AddParm( zLONG lID, CString csValue ); // TRUE if OK, FALSE otw
   ZParmItem * FindParm( zLONG lID );    // return ptr if found ... 0 otw
   zLONG GetParmCnt( ) { return( m_ParmList.GetCount( ) ); }
   POSITION GetFirstParmPosition( ) { return m_ParmList.GetHeadPosition( ); }
   ZParmItem * GetNextParm( POSITION& pos )
   {
      return (ZParmItem *) m_ParmList.GetNext( pos );
   }

   DISPID GetID( ) { return m_dispid; }

private:
// ZEventItem *m_pNextEvent;
// ZParmItem  *m_pHeadParm;
   CObList    m_ParmList;
   DISPID     m_dispid;
// zLONG      m_lParmCnt;
};

class ZDRAPP_CLASS ZEventList : private CObList
{
public:
   ZEventList( );
   ~ZEventList( );

   ZEventItem * AddEvent( DISPID dispid );  // return ptr if added ... 0 otw
   zBOOL DeleteEvent( DISPID dispid ); // return TRUE if removed ... FALSE otw
   ZEventItem * FindEvent( DISPID dispid ); // return ptr if found ... 0 otw

private:
// ZEventItem *m_pHeadEvent;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Exception handling class
//
class ZDriverException : public CException
{
   // abstract class for dynamic type checking
   DECLARE_DYNAMIC( ZDriverException )

public:
// Constructors
   ZDriverException ( BOOL bAutoDelete = TRUE );   // sets m_bAutoDelete = TRUE
   ZDriverException ( CString strErrorMsg );

// Operations
   virtual BOOL GetErrorMessage(LPTSTR lpszError, UINT nMaxError,
      PUINT pnHelpContext = NULL);
   virtual int ReportError(UINT nType = MB_OK, UINT nMessageID = 0);

public:
   void Delete( );
   virtual ~ZDriverException( );

private:
   CString m_csErrorMessage;
};

/////////////////////////////////////////////////////////////////////////////
// Structure definition for StatusBar (DIL) items
//
typedef struct tagSTATUSBAR_ITEM
{
   zLONG  lSeqNbr;
   zLONG  lOffset;
   zLONG  lWidth;
   zPCHAR pchText;
   zLONG  lMode;

} STATUSBAR_ITEM;

/////////////////////////////////////////////////////////////////////////////
// Structure definition for ZMessage.
//
struct ZMessage
{
   HWND   hWnd;

   union
   {
      HANDLE hParam;
      WPARAM wParam;
   };

   union
   {
      LPARAM  lParam;

      struct tagLP
      {
         long Lo;
         long Hi;  //???
      } LP;
   };
};

/////////////////////////////////////////////////////////////////////////////
// Class definition for ZAttr (style bits and position/size values).
//
class ZDRAPP_CLASS ZAttr
{
public:
   ZAttr( ) { StyleEx = Style = X = Y = W = H = 0; }
   zLONG X;
   zLONG Y;
   zLONG W;
   zLONG H;
   zLONG Style;
   zLONG StyleEx;
};

class ZDRAPP_CLASS ZAmbientProperty : public CObject
{
   DECLARE_SERIAL( ZAmbientProperty );

public:
   ZAmbientProperty( );
   ZAmbientProperty( DISPID dispid, LPCTSTR pszName, const VARIANT& var,
                     int vti, BOOL tStock = FALSE );

   void Enable( BOOL tEnabled );
   DISPID GetID( ) const;
   CString GetName( ) const;
   const COleVariant& GetValue( ) const;
   int  GetVTI( ) const;
   BOOL IsEnabled( ) const;
   BOOL IsStock( ) const;
   void SetValue( const VARIANT& varValue, int vti = -1 );

   void Serialize( CArchive& ar );

#ifdef _DEBUG
   void Dump(CDumpContext& dc) const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump(dc);
      dc << pRc->m_lpszClassName << "\n";
   }
#endif

protected:
   DISPID m_dispid;
   CString m_csName;
   COleVariant m_varValue;
   int  m_vti;
   BOOL m_tEnabled;
   BOOL m_tStock;
};

/////////////////////////////////////////////////////////////////////////////
// Twain wrapper class
//
class ZDRAPP_CLASS ZTwain
{
public:

   ZTwain( CFrameWnd *pZFWnd = 0 );
   virtual ~ZTwain( );

   zBOOL InitTwain( CFrameWnd *pZFWnd );
   void ReleaseTwain( CFrameWnd *pZFWnd );

   // This routine must be implemented by the derived class.
   // After setting the required values in the m_AppId structure,
   // the derived class should call the parent class implementation.
   // Refer Pg: 51 of the Twain Specification version 1.8.
   virtual void  GetIdentity( );
   virtual zBOOL SelectSource( );
   virtual zBOOL OpenSource( TW_IDENTITY *pSource = 0 );
   virtual int   ShouldTransfer( TW_IMAGEINFO& info );

   zBOOL ProcessMessage( MSG *pMsg );

   zBOOL SelectDefaultSource( );
   zBOOL IsValidDriver( ) const;
   zBOOL DSMOpen( ) const;
   zBOOL DSOpen( ) const;
   zBOOL SourceSelected( ) const;
   zBOOL ModalUI( ) const;

   TW_INT16 GetRC( ) const;
   TW_STATUS GetStatus( ) const;

   zBOOL SetImageCount( TW_INT16 nCount = 1 );
   zBOOL Acquire( int nImages = 1 );

protected:
   zBOOL CallTwainProc( pTW_IDENTITY pOrigin, pTW_IDENTITY pDest,
                        TW_UINT32 DG, TW_UINT16 DAT, TW_UINT16 MSG,
                        TW_MEMREF pData );

   void  CloseDSM( );
   void  CloseDS( );

   zBOOL GetCapability( TW_CAPABILITY& twCap, TW_UINT16 cap,
                       TW_UINT16 conType = TWON_DONTCARE16 );
   zBOOL GetCapability( TW_UINT16 cap, TW_UINT32& value );
   zBOOL SetCapability( TW_UINT16 cap, TW_UINT16 value, zBOOL sign = FALSE );
   zBOOL SetCapability( TW_CAPABILITY& twCap );
   zBOOL EnableSource( zBOOL bShowUI = TRUE );

   zBOOL GetImageInfo( TW_IMAGEINFO& info );

   void  TranslateMessage( TW_EVENT& twEvent );
   void  TransferImage( );
   zBOOL EndTransfer( );
   void  CancelTransfer( );
   zBOOL ShouldContinue( );
   zBOOL GetImage( TW_IMAGEINFO& info );

   virtual zBOOL DisableSource( );
   virtual zBOOL CanClose( );
   virtual void  CopyImage( HANDLE hBitmap, TW_IMAGEINFO& info ) = 0;

protected:
   HINSTANCE    m_hTwainDLL;
   DSMENTRYPROC m_pDSMProc;
   TW_IDENTITY  m_AppId;
   TW_IDENTITY  m_Source;
   TW_STATUS    m_Status;
   TW_INT16     m_nRC;
   CFrameWnd    *m_pZFWnd;
   zBOOL m_bSourceSelected;
   zBOOL m_bDSMOpen;
   zBOOL m_bDSOpen;
   zBOOL m_bModalUI;
   int   m_nImageCnt;

public:
   CString *m_pzsTwainFileName;
   zBOOL   m_bSourceEnabled;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZSplitterWnd
//
class ZDRAPP_CLASS ZSplitterWnd : public CSplitterWnd
{
   DECLARE_DYNAMIC( ZSplitterWnd )
public:
   ZSplitterWnd( ZSubtask *pSubtask, zCPCHAR cpcSplitTag,
                 zSHORT nPercentX, zSHORT nPercentY, zLONG lFlag );
   virtual ~ZSplitterWnd( );

   zLONG  AddView( zSHORT nRow, zSHORT nCol,
                   CRuntimeClass *pViewClass,
                   CCreateContext *pContext,
                   CWnd *pNewView = 0 );
   void   ShowView( zLONG lViewId );
   zLONG  GetView( ZSubtask *pZSubtask );
   ZDrView * GetView( zLONG lViewId );
   ZDrView * GetCurrentView( zSHORT nRow, zSHORT nCol, zLONG *plCurrId = 0 );
   zBOOL  SetSubtaskForViewId( zLONG lViewId, ZSubtask *pZSubtask );
   void   SetCurrentView( zSHORT nRow, zSHORT nCol, zLONG lViewId );
   zLONG  HideCurrentView( zSHORT nRow, zSHORT nCol );
   zBOOL  FixupSubtaskForViewId( zLONG lViewId, ZSubtask *pZSubtask );
   ZSubtask * GetSubtaskForPane( zSHORT nRow, zSHORT nCol );
   zBOOL  GetPaneFromViewId( zLONG lViewId, zPSHORT pnRow, zPSHORT pnCol );
//?zBOOL  ReplaceView( CRuntimeClass *pViewClass );
   ZDrView * RemoveView( zLONG lViewId );
   zLONG  GetLastView( );
   ZPairedItem * GetFirstPaneView( );

   void RefreshSplitBars( );
   void SetActivePane( int nRow, int nCol, CWnd *pWnd = 0 );
   void SetHighlight( );

   // Attributes
public:
   static zLONG g_lIdNbr;
   zULONG      m_ulIdNbr;         // unique IdNbr assigned in ctor
   HWND        m_hWndAct;
   zSHORT      m_nRow;
   zSHORT      m_nCol;

   CString     *m_pcsSplitTag;
   ZTask       *m_pZTask;
   ZSubtask    *m_pZSubtask1;     // primary subtask
   ZSubtask    *m_pZSubtask2;     // secondary subtask
   ZDrView     *m_pZView;
   zULONG      m_ulBaseSplitterPane;
   zSHORT      m_nPercentX;
   zSHORT      m_nPercentY;
   zLONG       m_lFlag;
   zSHORT      m_nCols;
   zSHORT      m_nRows;
   CWnd        *m_pParentWnd;
   zBOOL       m_bSplitterView[ 2 ][ 2 ];

// ZPairedList m_mapViewPane;     // map<int, long> m_mapViewPane;
// ZPairedList m_mapIdViews;      // map<int, CWnd *> m_mapIdViews;
   ZPairedList m_mapIdPaneViews;  // ViewId, PaneId, ZDrView *, ZSubtask *
   ZPairedList m_mapCurrentViews; // map<long, int> m_mapCurrentViews;
   zLONG       m_lNextViewId;

protected:
   afx_msg void OnClose( );

   // Implementation
protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage );
   afx_msg void OnSize( UINT uSizeType, int cx, int cy );
   afx_msg void OnDrawSplitter( CDC *pDC, ESplitType nType, const CRect& rectArg );

#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

   DECLARE_MESSAGE_MAP( )
};

class ZDRAPP_CLASS ZParentData;

/////////////////////////////////////////////////////////////////////////////
// ZSplitterView view
//
class ZDRAPP_CLASS ZSplitterView : public CView
{
   DECLARE_DYNCREATE( ZSplitterView )

public:
   ZSplitterView( );
   virtual ~ZSplitterView( );

   static zLONG g_lIdNbr;
   zULONG       m_ulIdNbr;             // unique IdNbr assigned in ctor
   ZSubtask     *m_pZSubtask;
   zBOOL        m_bInitialized;
   zBOOL        m_bInitSize;
   zLONG        m_lFlag;
   ZSplitterWnd *m_pSplitterWnd;
   ZParentData  *m_pParentData;

   // Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZSplitterView )
protected:
public:
   afx_msg void OnInitialUpdate( );
protected:
   virtual void OnDraw( CDC *pDC );    // overridden to draw this view
   //}}AFX_VIRTUAL

   // Implementation
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

   // Generated message map functions
protected:
   //{{AFX_MSG(ZSplitterView)
   afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
   afx_msg void OnSize(UINT nType, int cx, int cy);
   LRESULT OnPostedSplitterClose( WPARAM wParam, LPARAM lParam );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZParentData
//
class ZDRAPP_CLASS ZParentData
{
public:
   ZParentData( ZSplitterView *pParentSplitterView,
                ZSplitterWnd  *pParentSplitterWnd,
                ZSplitterView *pSplitterView,
                zSHORT nRow, zSHORT nCol );
   virtual ~ZParentData( );

   static zLONG g_lIdNbr;
   zULONG        m_ulIdNbr;          // unique IdNbr assigned in ctor
   ZSplitterView *m_pParentSplitterView;
   ZSplitterWnd  *m_pParentSplitterWnd;
   ZSplitterView *m_pSplitterView;
   CWnd          *m_pOrigViewParent;
   zSHORT        m_nRow;
   zSHORT        m_nCol;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zMOUSE_STATUS_OFF             (UINT) 0x0000
#define zMOUSE_SELECT_PENDING         (UINT) 0x0010
#define zMOUSE_CAPTURE                (UINT) 0x1000
#define zMOUSE_CAPTURE_LEFT           (UINT) 0x1001
#define zMOUSE_CAPTURE_RIGHT          (UINT) 0x1002
#define zMOUSE_DRAG                   (UINT) 0x1100
#define zMOUSE_DRAG_LEFT              (UINT) 0x1101
#define zMOUSE_DRAG_RIGHT             (UINT) 0x1102
#define zMOUSE_ASYNC_DISABLE          (UINT) 0x2000

/////////////////////////////////////////////////////////////////////////////
// Structure definition for ZDragDrop.
//
typedef struct ZDragDrop_tag
{
   zCHAR    szSrcDragKey[ 33 ];        // source drag key
   ZMapAct  *pzmaSrc;                  // source drag ctrl
   zVIEW    vSrcSubtask;               // source subtask view
   zCHAR    szSrcDialogName[ zTAG_LTH ];     // source dialog name
   zCHAR    szSrcWindowName[ zTAG_LTH ];     // source window name
   zCHAR    szSrcCtrlName[ zTAG_LTH ];       // source control name
   zVIEW    vData;                     // source view
   zCHAR    szViewName[ zTAG_LTH ];          // source view name
   zCHAR    szEntityName[ zTAG_LTH ];        // source entity name
   zCHAR    szAttributeName[ zTAG_LTH ];     // source attribute name
   zCHAR    fCopyMultiple;             // 0x00 - MOVE;   0x01 - COPY;
                                       // 0x00 - SINGLE; 0x10 - MULTIPLE
   ZMapAct  *pzmaTgt;                  // target drag ctrl
   zVIEW    vTgtSubtask;               // target subtask view
   zCHAR    szTgtDialogName[ zTAG_LTH ];     // target dialog name
   zCHAR    szTgtWindowName[ zTAG_LTH ];     // target window name
   zCHAR    szTgtCtrlName[ zTAG_LTH ];       // target control name
   POINT    ptDrop;                    // location at which the drop occurred

} ZDragDrop;

/////////////////////////////////////////////////////////////////////////////
// Interface of the ZDrFrame class
//
class ZDRAPP_CLASS ZDrFrame : public CFrameWnd
{
// DECLARE_DYNAMIC( ZDrFrame )
   DECLARE_DYNCREATE( ZDrFrame )

public:
   // Can be used to create from serialization.
   ZDrFrame( ZSubtask *pZSubtask = 0 );
   virtual ~ZDrFrame( );

// Overrides
public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
   virtual BOOL DestroyWindow( );
// virtual void OnDraw( CDC *pDC );  // overridden to use memory DC

   BOOL VerifyBarState( LPCTSTR lpszProfileName );
   zBOOL SetSystemMenuItem( zCPCHAR cpcItemText, zCPCHAR cpcAfterItem,
                            zUSHORT uItemId,
                            zCPCHAR cpcDLL_Name, zCPCHAR cpcOperationName );

protected:
   virtual BOOL OnCmdMsg( UINT nID, int nCode, void *pExtra,
                          AFX_CMDHANDLERINFO *pHandlerInfo );
   virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs,
                                CCreateContext *pContext );

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

// Generated message map functions
protected:

   //{{AFX_MSG(ZDrFrame)
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnClose( );
   afx_msg void OnDestroy( );
   afx_msg void OnNcDestroy( );
   afx_msg void OnActivate( UINT nState, CWnd *pWndOther, BOOL bMinimized );
   afx_msg void OnKillFocus( CWnd *pWndLostFocus );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT uMessage );
   afx_msg void OnEnable( BOOL bEnable );
   afx_msg void OnPaint( );
   afx_msg void OnNcMouseMove( UINT hitTest, CPoint pt );
   afx_msg void OnNcLButtonDown( UINT hitTest, CPoint pt );
   afx_msg void OnNcMButtonDown( UINT hitTest, CPoint pt );
   afx_msg void OnNcRButtonDown( UINT hitTest, CPoint pt );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg HANDLE OnQueryDragIcon( );
   afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
   afx_msg void OnGetMinMaxInfo( MINMAXINFO *lpMMI );
   afx_msg void OnSize( UINT sizeType, int cx, int cy );
   afx_msg void OnMove( int x, int y );
   afx_msg void OnSysColorChange( );
   afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
   afx_msg void OnInitMenu( CMenu *pMenu );
   afx_msg void OnInitMenuPopup( CMenu *pPopupMenu, UINT nIndex, BOOL bSysMenu );
   afx_msg BOOL IdleAction( long lIdleCount );
   afx_msg void OnMenuSelect( UINT menuItemId, UINT uFlags, HMENU hMenu );
   afx_msg void OnPaletteChanged( CWnd *pWndPalChg );

   afx_msg void OnUpdateControlBarMenu( CCmdUI *pCmdUI );
   afx_msg BOOL OnBarCheck( UINT nID );
   afx_msg BOOL OnToolTipText( UINT uIdNbr, NMHDR *pNMHDR,
                               LRESULT *plResult );
   afx_msg BOOL OnToolbarDropDown( UINT uIdNbr, NMHDR *pNMHDR,
                                   LRESULT *plResult );
   afx_msg void OnUpdateKeyIndicator( CCmdUI *pCmdUI );
   afx_msg void OnUpdateContextHelp( CCmdUI *pCmdUI );
   afx_msg void OnUpdateRestore( CCmdUI *pCmdUI );
   afx_msg void OnUpdateMinimize( CCmdUI *pCmdUI );
   afx_msg void OnUpdateMaximize( CCmdUI *pCmdUI );

   // user messages
   afx_msg LRESULT OnSetMessageString( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnProcessEvent( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnProcessAction( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnProcessHotkey( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedActivate( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedPopupMenu( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedClose( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedSplitterClose( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnResumeFromPainter( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedMarkDelete( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnFindDialogWindow( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnSetWindowState( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnSetWindowActionBehavior( WPARAM wParam, LPARAM lParam );

   // registered messages
   afx_msg LRESULT OnBeginDrag( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnQueryDrop( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnDoDrop( WPARAM wParam, LPARAM lParam );

   // NOTE - the ClassWizard will add and remove member functions here.
   //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG

// void OnMenuSelection( );   // handling OnUpdateCmdUI instead ... except
//                            // for CUT, PASTE, etc?
// void OnEditCut( );
// void OnEditCopy( );
// void OnEditPaste( );
// void OnEditUndo( );
// void OnEditRedo( );

   zBOOL SetApplicationState( UINT uCmdType );

// Attributes
protected:  // control bar embedded members
   CStatusBar   m_wndStatusBar;
   CToolBar     m_wndToolBar;

public:
   static zLONG g_lIdNbr;
   zULONG       m_ulFrameIdNbr;        // unique IdNbr assigned in ctor
   ZSubtask     *m_pZSubtask;
   ZDrView      *m_pZView;
   ZTwain       *m_pZTwain;
   ZSplitterWnd *m_pSplitterWnd;
   ZSubtask     *m_pZSubtaskPopup;
   zLONG        m_lPaneId;
   zSHORT       m_nMinSizeX;
   zSHORT       m_nMinSizeY;
   zSHORT       m_nMaxSizeX;
   zSHORT       m_nMaxSizeY;
   ZEventItem   *m_pSysMenuEvents;
   zULONG       m_ulFrameFlags;

   DECLARE_MESSAGE_MAP( )
};

#if 0
class ZDRAPP_CLASS ZDrFramexMDI : public CMDIFrameWnd
{
   DECLARE_DYNAMIC( ZDrFramexMDI )
public:
   ZDrFramexMDI( ZSubtask *pZSubtask );
   virtual ~ZDrFramexMDI( );

// Implementation
public:
   ZSubtask *m_pZSubtask;
   ZDrView  *m_pZView;

protected:  // control bar embedded members
   CStatusBar  m_wndStatusBar;
   CToolBar    m_wndToolBar;

// Generated message map functions
protected:
   //{{AFX_MSG(ZDrFramexMDI)
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnPaletteChanged( CWnd *pFocusWnd );
   afx_msg BOOL OnQueryNewPalette( );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};
#else

#if 0    // we do not use MDI at this time
class ZDrFramexMDI : public CMDIChildWnd
{
   DECLARE_DYNCREATE( ZDrFramexMDI )
public:
   ZDrFramexMDI( );

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDrFramexMDI)
   public:
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   protected:
   virtual BOOL OnCreateClient( LPCREATESTRUCT lpcs,
                                CCreateContext *pContext );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZDrFramexMDI( );
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

// Generated message map functions
protected:
   //{{AFX_MSG(ZDrFramexMDI)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
#endif
#endif

#ifdef zREMOTE_SERVER

// There is one of these classes for each Socket ... it essentially
// replaces the ZTask class for remote processing.
class ZDRAPP_CLASS ZSocketTask
{
public:
   // ctor
   ZSocketTask( SOCKET sock, ZTask *pZTask );
   ~ZSocketTask( );

   zSHORT  ApplyState( zBOOL bLocateActionOnly );
   void    LockMutex( zBOOL bLock, zCPCHAR cpcText );
   LRESULT SendZDxFrameMessage( WPARAM wParam );

   SOCKET   m_sock;                 // Socket to send/receive.
   CWinThread *m_pWinThread;
   ZTask    *m_pZTask;              // Pointer to related ZTask
   ZSubtask *m_pZSubtaskTop;        // Main subtask (corresponding to main window)
   zVIEW    m_vRegisteredSubtask;   // Subtask view for thread
   zVIEW    m_vCreatedSubtask;      // Subtask view for thread
   zVIEW    m_vSubtask;             // Subtask view for thread
   zVIEW    m_vXRA;                 // Current XRA definition
   zVIEW    m_vXRA_State;           // Incoming (to RemoteServer) XRW state
   ZXRA_List *m_pCacheWndList;
   zLONG    m_lSocketState;         // Current state of socket thread ...
                                    //    0 - initialization
                                    //    1 - send XRA
                                    //    2 - receive XRA
                                    //    4 - idle
                                    //   -1 - termination
   zLONG    m_lSyncIdx;
   zCHAR    m_szAppTag[ zTAG_LTH ];       // Application/LPLR name
   zCHAR    m_szRemoteWndTag[ 66 ];
   zCHAR    m_szActTag[ 66 ];       // for special events, Tag + event id
   zCHAR    m_chActType;            // WndEvent, Ctrl, Opt, Hotkey (W,C,O,H)
   zLONG    m_lMutexLockCnt;        // track number of times mutex is locked
   ZDxFrame *m_pZDxFrame;
   CString  *m_pzsAppName;          // the ApplicationName used by the
                                    // LightDriver through KZXRA
};

#endif

/////////////////////////////////////////////////////////////////////////////
// Zeidon application task ... to minimize the interface between the .exe
// and this .dll
//
class ZDRAPP_CLASS ZTask : public ZTwain
{
public:
   // ctor
   ZTask( ZDrApp *pZDrApp,    // DR C++ application object
          zVIEW  vSubtask,    // Zeidon subtask for this window
          ZCommandLineInfo *pCmdInfo );

   // alternate ctor
   ZTask( zVIEW  vSubtask );  // Zeidon subtask for this window

   // dtor
   virtual ~ZTask( );

#ifdef zREMOTE_SERVER
   CWinThread *m_pThread;      // Thread which tracks all Subtask threads
#endif

   void InitTask( zVIEW vSubtask );

   // Create a new window within this application.
   ZSubtask *
   NewZWnd( ZSubtask *pZParent,     // Zeidon parent window (subtask)
            ZSubtask *pZOwner,      // Zeidon owner window (subtask)
            zVIEW    vSubtask,      // Zeidon subtask for this window
            zVIEW    vNewSubtask,   // if 0, create new subtask, else use
            zSHORT   nCreateModal,  // create as a modal window
            zCPCHAR  cpcDlgTag,     // Dialog containing the ...
            zCPCHAR  cpcWndTag,     // Window to create (if 0, use dflt)
#ifdef zREMOTE_SERVER
            ZSubtask *pZSubtaskReplace = 0,
            ZSocketTask *pZSocketTask = 0 );

   static zLONG GetSocketTaskCount( );
#else
            ZSubtask *pZSubtaskReplace = 0 );
#endif

   void DeleteZWnd( ZSubtask *pZSubtask );
   void DeleteSubtask( ZSubtask *pZSubtask );
   void AddApplicationDLL( ZSubtask *pZSubtask, zCPCHAR pchDLL_Name );
   void SetImage( HANDLE hBitmap, zUSHORT uBits );
   void CopyImage( HANDLE hBitmap, TW_IMAGEINFO& info );

   void PutSubtaskOnSplitterDiscardPile( ZSubtask *pZSubtask );
   ZSubtask * FindSubtaskOnSplitterDiscardPile( zCPCHAR cpcSplitTag,
                                                zCPCHAR cpcDlgTag,
                                                zCPCHAR cpcWndTag );

   static zLONG g_lIdNbr;
   zULONG       m_ulTaskIdNbr;     // unique IdNbr assigned in ctor
   ZDrApp       *m_pZDrApp;
   zCHAR        m_szInitialDlgName[ 34 ]; // the dialog name by which this
                                          // task is started
   ZSubtask     *m_pZSubtaskTop;   // Main subtask (corresponding to main window)
   CString      *m_pzsHelpFile;    // If help was invoked, this is the Help File name
   ZSubtask     *m_pZSubtaskFocus; // Window to which to set focus (current if 0)
   CWnd         *m_pWndParent;     // If this was started from another application
   zVIEW        m_vInterpSubtask;  // Is the interpretor active for this task
   zVIEW        m_vNewAppSubtask;  // Is the new subtask under a new application?
   zFARPROC_INTERP m_lpfnInterp;   // Interpretor dynamic call operation
   zUSHORT      m_uAppState;       // state of this object
   ZCommandLineInfo *m_pCmdInfo;   // to hold on to the command line
   ZNameList    m_DLL_InitList;    // list of initialized Ctrl DLL's
   ZSubtask     *m_pZSubtaskConversational;  // ZSubtask conv dialog semaphore
   zSHORT       m_nOS_Version;     // Operating system version
   zSHORT       m_nTraceAction;    // turn trace on/off for actions
   zULONG       m_ulNextSplitterPane;
   zBOOL        m_bDeletingSplitter;
   zBOOL        m_bLocked;
   CFrameWnd    *m_pZFWndHidden;   // hidden frame window pointer
   ZNumberItem  *m_pSplitterDiscardHead;
   CString      *m_pzsLastDlgTag;  // Last dialog to have been active
   CString      *m_pzsLastWndTag;  // Last window to have been active
   ZSubtask     *m_pZSubtaskPrint;
   zVIEW        m_vHK;             // Zeidon Hotkey view for this task
   zVIEW        m_vPE;             // Zeidon PE view for this task

   // Drag and Drop cursors.
   HCURSOR  m_hcurDropNot;
   HCURSOR  m_hcurDropOKC1;
   HCURSOR  m_hcurDropOKM1;
   HCURSOR  m_hcurDropOKCM;
   HCURSOR  m_hcurDropOKMM;
   HCURSOR  m_hcurSave;

   ZPairedList  *m_pWndList;
   CString      m_csHoldCaption;
   zLONG        m_lHoverDelay;
   UINT         m_uToolTipTimerId;
};

/////////////////////////////////////////////////////////////////////////////
// Helper class to maintain list of ToolBar controls for tooltip processing
//
class ZDRAPP_CLASS ZBarTip
{
public:
   // ctor
   ZBarTip( HWND hWnd, zLONG lFirstId, zLONG lLastId );

   HWND   m_hWnd;
   zLONG  m_lFirstId;
   zLONG  m_lLastId;
};

class ZDRAPP_CLASS ZAction
{
public:
   ZAction( zLONG lActType = 0, zCPCHAR cpcAction = 0,
            zCPCHAR cpcDlg = 0, zCPCHAR cpcWnd = 0 );
   ~ZAction( );

   zLONG m_lActType;       // type of action requested in code for this window
   zCHAR  m_szAction[ zTAG_LTH ]; // data for requested action (e.g. Y, 1, 2 ...)
   zCHAR  m_szActDlg[ zTAG_LTH ]; // name of dialog for the action type
   zCHAR  m_szActWnd[ zTAG_LTH ]; // name of window for the action type
   ZAction *m_pNextAction;
};

/////////////////////////////////////////////////////////////////////////////
// typedefs
//
typedef AFX_EXT_API CWnd * (POPERATION zFARPROC_CONTROL)( ZSubtask *, CWnd *,
                                                          ZMapAct *, zVIEW,
                                                          zSHORT, zSHORT,
                                                          zKZWDLGXO_Ctrl_DEF * );
typedef zUSHORT (POPERATION zFARPROC_CTRL)( ZSubtask * );
typedef zUSHORT (POPERATION zFARPROC_INIT)( zVIEW, zLONG, zLONG, zLONG, zLONG );
typedef zSHORT (POPERATION zFARPROC_COMPARE)( CArray<zPVOID, zPVOID> *,
                                              zLONG, zLONG, zPVOID );

#define zFD_MAX 1000

/////////////////////////////////////////////////////////////////////////////

#define zSUBTASK_INITIALIZE               0x00000000L // initialization value
#define zSUBTASK_CREATED_MODAL            0x00001000L // window created modal
#define zSUBTASK_CONTROL_DOMAINERR        0x00002000L // domain error for any control
#define zSUBTASK_DIALOG_BOX               0x00004000L // dialog box or primary window
#define zSUBTASK_PROCESSING_ACTIVATE      0x00008000L // prevent recursive activates
#define zSUBTASK_SUBWND_SDI_FRAME         0x00000100L
#define zSUBTASK_SUBWND_DLG_FRAME         0x00000200L
#define zSUBTASK_SUBWND_MDI_FRAME         0x00000400L
#define zSUBTASK_SUBWND_CONVERSATIONAL    0x00000800L
#define zSUBTASK_SUBWND_SPLITTER_PANE     0x00100000L
#define zSUBTASK_SPLITTER_MAIN_SUBTASK    0x00200000L
#define zSUBTASK_INPLACEEDIT              0x00400000L
//efine zSUBTASK_PROCESSING_ACTION        0x01000000L
#define zSUBTASK_SELECT_FIRST_EDIT        0x00010000L // select text in first editbox
                                                      // on entry to window
#define zSUBTASK_SUBWND_CHILD             0x00000010L // child window
#define zSUBTASK_SUBWND_POPUP_PARENT      0x00000020L // popup window with parent
#define zSUBTASK_SUBWND_POPUP_NOPARENT    0x00000040L // popup window with no parent
#define zSUBTASK_NO_MAIN_MENU             0x00000080L // window created with no menu
#define zSUBTASK_SCROLLBARS               0x00000001L // scroll bars
#define zSUBTASK_MAPPING_FROM_OI          0x00000002L // subtask MapFromOI in progress
#define zSUBTASK_MINIMIZED_MODAL          0x00000004L // minimized by Modal subwindow
#define zSUBTASK_APPLICATION_OPERATION    0x00000008L // processing app operation
#define zSUBTASK_MAPPED_SINCE_APPLYSTATE  0x00020000L // subtask mapped since ApplyState
#define zSUBTASK_SINGLE_RELSIZE           0x00040000L // window has only one resizable ctrl
#define zSUBTASK_NO_ERROR_PROMPT          0x00080000L // prevent error message box

#define zSUBTASK_STATE_INIT               0x00000000L
#define zSUBTASK_STATE_IN_CONSTRUCTION    0x00000010L
#define zSUBTASK_STATE_PREBUILD_PRECODE   0x00000011L
#define zSUBTASK_STATE_PREBUILD_POSTCODE  0x00000012L
#define zSUBTASK_STATE_POSTBUILD_PRECODE  0x00000014L
#define zSUBTASK_STATE_POSTBUILD_POSTCODE 0x00000018L
#define zSUBTASK_STATE_WELL_FORMED        0x00000020L
#define zSUBTASK_STATE_MARK_FOR_DELETE    0x00000100L
#define zSUBTASK_STATE_DELETE             0x00000300L
#define zSUBTASK_STATE_DELETED            0x00000700L
#define zSUBTASK_STATE_DELETE_WELL_FORMED 0x00000720L
#define zSUBTASK_STATE_PROVISION_REPLACE  0x00010300L
#define zSUBTASK_STATE_REPLACED           0x00020000L
#define zSUBTASK_STATE_CLEANUP            0x00040700L
#define zSUBTASK_STATE_STOP_RECURSE       0x00080000L
#define zSUBTASK_STATE_EXTERNAL_APP       0x01000000L
#define zSUBTASK_STATE_INTERP_DEBUG       0x02000000L
#define zSUBTASK_STATE_REPLACED_BY_SELF   0x04000000L
#define zSUBTASK_STATE_PREVENT_REFRESH    0x40000000L
#define zSUBTASK_STATE_SPLIT_IN_PROCESS   0x20000000L
#define zSUBTASK_STATE_ERROR              0x10000700L
#define zSUBTASK_STATE_ON_RETURN          0x80000000L

/////////////////////////////////////////////////////////////////////////////
// Zeidon application subtask (incorporates Dialog information) object - to
// permit multiple classes of windows to derive common characteristics
//
class ZDRAPP_CLASS ZSubtask
{
public:
   // ctor
   ZSubtask( ZTask    *pZTask,      // DR C++ task object
             ZSubtask *pZParent,    // Zeidon parent window
             ZSubtask *pZOwner,     // Zeidon owner window
             zVIEW    vSubtask,     // Zeidon subtask for this window
             zVIEW    vNewSubtask,  // if 0, create new subtask, else use
             zSHORT   nCreateModal, // create as a modal window
             zCPCHAR  cpcDlgTag,    // Dialog containing the ...
#ifdef zREMOTE_SERVER
             zCPCHAR  cpcWndTag,    // Window to create (0 ==> dflt)
             ZSocketTask *pZSocketTask,
             zCPCHAR  cpcAppTag = 0 ); //Application / lplr name
#else
             zCPCHAR  cpcWndTag );  // Window to create (0 ==> dflt)
#endif

   // alternate ctor
   ZSubtask( ZTask  *pZTask,        // ZDr C++ task object
             CWnd   *pCWnd );       // Parent window (from existing app)

   // dtor
   virtual ~ZSubtask( );

   void   CreateZ( );
   zLONG  HandleEvent( zLONG lEvent );
   void   CheckMouseMove( UINT uModKeys, CPoint pt );
   void   RestartBuildWindow( zVIEW vAppSubtask );

   void   SetCaptionTitle( zCPCHAR cpcCaption,
                           zCPCHAR cpcTitle,
                           zBOOL   bUpdateWindow = TRUE );
   void   EnableParentWindows( zBOOL bEnable );
   BOOL   EnableWindow( BOOL bEnable );
   void   ResetFocus( ZMapAct *pzma );
   HWND   CreateMDI_Child( zCPCHAR cpcTitle );

   // Create action bar from the Zeidon dialog object instance.
   void   CreateZeidonMenu( zCPCHAR cpcMenuName );
   void   CreateZeidonPopupMenu( zCPCHAR cpcMenuName, POINT pt );
   zBOOL  PostClose( WPARAM wParam );

   // Mapping to the object instance from the window controls.
   zSHORT MapFromOI( WPARAM wFlag = 0, zCPCHAR cpcCtrlTag = 0 );
   zSHORT MapToOI( );
   void   AutoGrayActions( );

   // let the driver know that focus is changing
   zBOOL  InformDriverOfSetFocus( ZMapAct *pzmaReceivingFocus,
                                  zBOOL   bPushButton = FALSE );
   zBOOL  InformDriverOfKillFocus( ZMapAct *pzmaReceivingFocus );
   void   InformDriverOfFlyBy( ZMapAct *pzma, zCPCHAR cpcMsg = 0 );
   zSHORT SetStatusBarText( zCPCHAR pchMessage, zSHORT nItem = 0 );

   zBOOL  IsValidCtrl( ZMapAct *pzma );
   ZMapAct * GetMapActFromHWnd( HWND hWndCtrl );
   void   RefreshSplitters( );
   ZSubtask * GetSplitterMainSubtask( );
   void   RecalcLayout( HWND hWnd );
   ZMapAct * FindFirstLastFocus( zBOOL bSet = TRUE );
   ZMapAct * GetFocusCtrlWithinComposite( ZMapAct *pzmaComposite,
                                          ZMapAct *pzma,
                                          zBOOL bNext = TRUE );
   void   SendParentResize( ZMapAct *pzma, CSize *pSize );
   zLONG  SendCtrlSelectAll( );
   void   SaveBarState( );
   void   DoPlacement( ZTask *pZTask );
   ZMapAct * FindCtrlWithQuickKey( ZMapAct *pzmaComposite, zCHAR chKey );
   void   AddBarTip( HWND hWnd, zLONG lFirstId, zLONG lLastId );
   HWND   LocateBarTip( zLONG lId );
   void   CleanupBarTips( );
   void   CreateDefaultFont( );
   static void   InitializeSubtaskState( ZSubtask *pZSubtask, HWND hWnd = 0 );
   static zLONG  GetSubtaskStateIndex( ZSubtask *pZSubtask );
   static zLONG  GetSubtaskCount( zLONG lId );
   static zULONG GetSubtaskState( ZSubtask *pZSubtask );
   static zULONG SetSubtaskState( ZSubtask *pZSubtask,
                                  zULONG   ulStateRemove,
                                  zULONG   ulStateAdd );
   static void   TerminateSubtaskState( ZSubtask *pZSubtask );

protected:

   zUSHORT fnRecurseSubOptions( zVIEW   vSubMeta,
                                ZMultiIdxSet *pZBinOpt,
                                CMenu   *pMenu,
                                ZOption *pParent,
                                zUSHORT uParentOptID,
                                zUSHORT uOptID,
                                zUSHORT uOptBaseID );

public:
   static zLONG g_lIdNbr;
   zULONG   m_ulSubtaskIdNbr;    // unique IdNbr assigned in ctor
   ZTask    *m_pZTask;           // pointer to C++ task object
   ZSubtask *m_pZParent;         // parent C++ ZSubtask object
   ZSubtask *m_pZOwner;          // owner C++ ZSubtask object
   CFrameWnd *m_pZFWnd;          // Zeidon frame window pointer
   ZDrView  *m_pZView;           // Client window within frame
   HMENU    m_hMenuDefault;      // To retain MFC's default frame menu
   HMENU    m_hMenuDocTemplate;  // To retain MFC's DocTemplate default menu
   ZAttr    Attr;
#ifdef zREMOTE_SERVER
   ZSocketTask *m_pZSocketTask;
   CString  *m_pzsAppTag;        // Zeidon Application name
   CString  *m_pzsRemoteWndTag;  // Zeidon Dialog.Window name
   CString  *m_pzsImageURL;      // Image (resource) URL
#endif
   CString  *m_pzsDlgTag;        // Zeidon dialog name
   CString  *m_pzsWndTag;        // Zeidon dialog window name
   CString  *m_pzsDLL_Name;      // DLL name for subtask
   HINSTANCE m_hInstDLL;         // Instance handle from DLL
   HINSTANCE m_hInstRes;         // Resource instance handle from DLL
   CString  *m_pzsLastTag;       // Last control/option to have caused an action
   CString  *m_pzsLastCtrlTag;   // Last control to have caused an action
   CString  *m_pzsProfileViewName;// view name to be used for automatic profiling
   zPCHAR   m_pchWindowPlacement;// window placement structure
   zVIEW    m_vDialog;           // Dialog subtask view corresponding to this
   zVIEW    m_vMsgQ;             // Zeidon Message view for this subtask
   zULONG   m_ulWndOper;         // Window event Operation(s) bit flag
   zPCHAR   m_pchEnabledAction;  // TRUE ==> control is enabled ... FALSE otw
   ZMapAct  *m_pzmaFirstFocus;   // first control which can accept focus
   ZMapAct  *m_pzmaLastFocus;    // last control which can accept focus
   ZMapAct  *m_pzmaWithFocus;    // last control to have had focus on this window
   ZMapAct  *m_pzmaLastFlyBy;    // last control to inform subtask of "FlyBy"
   zULONG   m_ulFlyByTickCnt;
   ZMapAct  *m_pzmaAccel;        // this control wants first crack at accelerator keys
   ZMapAct  *m_pDfltBtn;         // default pushbutton for this window
   ZMapAct  *m_pCurrDfltBtn;     // current default pushbutton for this window

   ZAction  m_Action;            // current action for this window
   zPVOID   m_pEventParms;       // parameters associated with this event
   zCHAR    m_szActionName[ zTAG_LTH ];// current action during Application operation
   zVIEW    m_vInterpSubtask;    // is the debugger active for this window
   CString  *m_pzsResumeAction;  // name of action to resume when debugger is done
   CMenu    *m_pMenu;            // menu class pointer for this window
   ZMultiIdxSet *m_pZBinMenu;    // pointer to indexed menu list
   zSHORT   m_nDrawMenuBar;      // < 0 ==> do not call DrawMenuBar
   CMenu    *m_pTrackMenu;       // tracking popup menu class pointer for this window
   CString  *m_pzsMenuTag;       // menu bar tag
   CString  *m_pzsPopupTag;      // current popup menu tag
   ZMultiIdxSet *m_pZBinPopup;   // pointer to indexed popup menu list
   ZMultiIdxSet *m_pZMIXCtrl;    // pointer to indexed ctrl list
   CString  *m_pzsCaption;       // Window caption
   CString  *m_pzsTitle;         // Window title
   CStatusBar *m_pDIL;           // pointer to StatusBar (if any) for this window
   zSHORT   m_nAutoDIL;          // the item within the DIL to get automatic messages
   LPLIBRARY m_hLibrary;         // Library from which to run operations
   LPLIBRARY m_hLibRes;          // Library from which to load resources
   zKZWDLGXO_Wnd_DEF *m_pWndDef; // hold window info until the object is well formed
   ZMessage m_ZMsg;              // msg parms when child ctrls are notified
   zLONG    m_lRelTop;           // relative positioning top
   zLONG    m_lRelLeft;          // relative positioning left
   zLONG    m_lRelBottom;        // relative positioning bottom
   zLONG    m_lRelRight;         // relative positioning right
   zLONG    m_lOrigRelSizeX;     // original relative sizing box width
   zLONG    m_lOrigRelSizeY;     // original relative sizing box height
   zLONG    m_lExtentX;          // maximum X extent (for scrolling)
   zLONG    m_lExtentY;          // maximum Y extent (for scrolling)
   COLORREF m_clrText;           // non-zero ==> override text color
   COLORREF m_clrBk;             // non-zero ==> override background color
   CFont    *m_pFontWS;          // non-zero ==> override font [Workstation]
   CFont    *m_pFontCtrl;        // non-zero ==> override control fonts
   zBOOL    m_bUseMapDlgUnits;   // TRUE ==> use DlgUnits from MapDialogRect

   COLORREF m_clrErrText;        // text color for controls in error (dflt red)
   zSHORT   m_nInvisible;        // == 0 ==> window is visible ... < 0 otw
   zSHORT   m_nDisabled;         // == 0 ==> control is enabled ... < 0 otw
   zBOOL    m_bLocked;           // Locked for update using LockWindowUpdate
   zULONG   m_ulSubtaskFlags;    // modal, dialog box, child, popup, ..., flags
   UINT     m_uMouseStatus;
   zBOOL    m_bStopEvents;
   zPCHAR   m_pAccels;
   zSHORT   m_nDIL_Items;
   zLONG    m_lMsgCmdId;
   zCPCHAR  m_cpcCurrOper;       // current operation being processed
   CArray<ZBarTip *, ZBarTip *> m_BarTipList;
   zSHORT   m_nBarTipCnt;
   zULONG   m_ulNextBarTipCmd;

   ZSubtask *m_pZSubtaskGlobal; // global subtask pointer
   ZSubtask *m_pZSubtaskProxy;  // subtask standing in for global subtask
   ZSubtask *m_pZSubtaskParent;
   ZSplitterView *m_pSplitterView;
   CString  m_csSplitTag;
   CString  m_csVName;

   ZPrintout *m_pZPrintout;      // printer object
   zLONG    m_lRelease;          // Major/Minor/Rel/Ver bytes
   zLONG    m_lTID;              // Task or Thread Id
   zVIEW    m_vPE;               // Zeidon PE view for construction only
};

// ZMapAct::m_ulMapActFlags
#define zMAPACT_INITIALIZE          0x00000000 // initialization value
#define zMAPACT_ENTITY_EXISTS       0x00000001
#define zMAPACT_ENTITY_UPDATEABLE   0x00000002
#define zMAPACT_ENABLED             0x00000004 // ctrl enabled
#define zMAPACT_AUTO_DISABLED       0x00000008 // ctrl auto disabled
#define zMAPACT_VISIBLE             0x00000010 // ctrl visible/invisible
#define zMAPACT_AUTO_INVISIBLE      0x00000020 // ctrl auto invisible
#define zMAPACT_FOCUS_ENABLED       0x00000040 // ctrl may receive focus
#define zMAPACT_AUTO_NOFOCUS        0x00000080 // ctrl auto focus disabled
#define zMAPACT_ASKPARENT_FOCUS     0x00000100 // ask parent if focus ok
#define zMAPACT_DRAG_ENABLED        0x00000200 // ctrl enabled as drag src
#define zMAPACT_DROP_ENABLED        0x00000400 // ctrl enabled as drag tgt
#define zMAPACT_PAINT_IN_PROGRESS   0x00000800 // ctrl OnPaint in progress
#define zMAPACT_FULL_CLIENT         0x00010000 // ctrl requests full client
#define zMAPACT_NOAUTOMAP_FROM_OI   0x00020000 // only map on RefreshCtrl
#define zMAPACT_NOAUTOMAP_TO_OI     0x00040000 // only map on MapCtrl
#define zMAPACT_DISABLE_READONLY    0x00080000 // when disabled, set ReadOnly
#define zMAPACT_ABSOLUTE_PIXEL_SIZE 0x00100000 // ctrl size is in pixels
#define zMAPACT_ABSOLUTE_PIXEL_POS  0x00200000 // ctrl position is in pixels
#define zMAPACT_CLIENT_TYPE         0x00400000 // ctrl is of type client
#define zMAPACT_IGNORE_RESIZE       0x00800000 // ignore ParentResize message
#define zMAPACT_CTRL_SPECIFIC1      0x10000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC2      0x20000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC3      0x40000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC4      0x80000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC5      0x01000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC6      0x02000000 // ctrl managed flag
#define zMAPACT_CTRL_SPECIFIC7      0x04000000 // ctrl managed flag
#define zMAPACT_USE_SUBTASK_BRUSH   0x08000000 // using Subtask created brush

// ZMapAct::m_ulMapActFlag2
#define zMAPACT_CREATED_ONCE        0x00000001 // ctrl window created at least once
#define zMAPACT_MAPPED_ONCE         0x00000002 // ctrl MapFromOI has occurred at least once
#define zMAPACT_CREATE_CHILDREN     0x00000100 // create child controls?
#define zMAPACT_CREATED             0x00000200 // ctrl has been created
#define zMAPACT_MAPPING_FROM_OI     0x00001000 // ctrl MapFromOI in progress
#define zMAPACT_MAPPED_FROM_OI      0x00002000 // ctrl MapFromOI has occurred
#define zMAPACT_CREATED1            zMAPACT_CREATED | zMAPACT_CREATED_ONCE
#define zMAPACT_MAPPED_FROM_OI1     zMAPACT_MAPPED_FROM_OI | zMAPACT_MAPPED_ONCE
#define zMAPACT_INPLACE_CTRL        0x00010000 // being used as an "in place" ctrl
#define zMAPACT_INPLACE_VISIBLE     0x00020000 // visible "in place" ctrl
#define zMAPACT_PREFILL             0x00040000 // prefill with ctrl text when null

#define zMAP_DEFAULT       (WPARAM) 0x0000
#define zMAP_FIRST_TIME    (WPARAM) 0x0001
#define zMAP_NO_SORT       (WPARAM) 0x0002
#define zMAP_BY_SORT       (WPARAM) 0x0004

/////////////////////////////////////////////////////////////////////////////
// Zeidon application Object Ctrl mapping/action class
//
class ZDRAPP_CLASS ZMapAct
{
public:
   enum eDRAGDROP_EVENT { eBeginDrag, eQueryDrop, eDoDrop, eDrop };

   // ctor
   ZMapAct( ZSubtask *pZSubtask,       // this child control's parent
            ZMapAct  *pzmaComposite,   // this control's composite
            CWnd     *pWndParent,
            CWnd     *pCtrl,           // Ctrl object pointer
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef,
            zCPCHAR  cpcType );

   // dtor
   virtual ~ZMapAct( );

   // N.B.  a member function must be virtual if it is redefined in a derived
   // class, yet addressed by a pointer (reference) to the base class.

   // If zVALIDATE is specified as the flag, MapToOI will validate the data
   // but will not map to the object instance.
   virtual zSHORT MapToOI( zLONG lFlag = 0 );

   // If zMAP_FIRST_TIME is specified as the flag, this is the first time
   // mapping is being performed.
   virtual zSHORT MapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual zSHORT PostMapFromOI( WPARAM wFlag = zMAP_DEFAULT );
   virtual zSHORT PostCreateCtrls( WPARAM wFlag = 0 );

   // Default processing for a ctrl's mapping from the data OI ... if the
   // View and Entity and Attribute have been specified, SetText using
   // the value returned by GetStringFromAttribute returning 0, otherwise
   // return -1.

   // These must be virtual.
   virtual zBOOL  IsVisibleForMapFromOI( );
   virtual zVIEW  IsVisibleForMapToOI( zCPCHAR cpcViewName = 0 );
   virtual zVIEW  SetMapViewFromView( zVIEW v, zCPCHAR cpcEName, BOOL bSrc );
   virtual zBOOL  IsFocusOKForChild( ZMapAct *pzmaReceivingFocus );
   virtual void   ResetFocus( );
   virtual zBOOL  InformCompositeOfSetFocus( ZMapAct *pzmaReceivingFocus );
   virtual zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );
   virtual ZMapAct * GetRealFocus( );
   virtual zBOOL  DrawZCtrl( CDC *pDC, CRect& rectDraw );
   virtual zBOOL  PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle = "",
                              zLONG lFlags = 0,
                              zSHORT nFontSize = 80,
                              zCPCHAR cpcFontFace = "Arial" );
   virtual zSHORT PrintZPage( zLONG  lPageNbr,
                              zLONG  lPageWidth,
                              zLONG  lPageHeight );
   virtual void   HiliteText( zBOOL bHilite, zLONG lType,
                              zLONG lRange = 0, zBOOL bHiliteItem = TRUE );
   virtual zLONG  GetZCtrlState( zLONG lStatusType );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zLONG  GetZCtrlProperty( zLONG   lPropertyType,
                                    zPULONG pulNumericProperty,
                                    zPCHAR  pchStringProperty,
                                    zLONG   lMaxLth );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );
   virtual zSHORT SetCtrlMapping( zCPCHAR cpcViewName,
                                  zCPCHAR cpcEntityName,
                                  zCPCHAR cpcAttributeName,
                                  zCPCHAR cpcContextName,
                                  zLONG   lColumn = -1 );
   virtual zSHORT GetCtrlMapping( zPCHAR pchViewName,
                                  zPCHAR pchEntityName,
                                  zPCHAR pchAttributeName,
                                  zPCHAR pchContextName,
                                  zLONG  lColumn = -1 );
   virtual zSHORT SetRowColText( zCPCHAR cpcText,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uLth,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   virtual zSHORT SetChildCtrlState( zLONG  lStatusType, zULONG ulValue,
                                     zBOOL  bAtLevel = FALSE );
   virtual void   ParentResize( ZMapAct * pParent, CSize *pSize );
   virtual void   ParentMove( );
   virtual void   PaletteChanged( );
   virtual void   ParentActivated( );
   virtual zBOOL  HandleAccel( zACCEL_DEF& ad );
   virtual zLONG  MessageFromMapAct( zPCHAR pchMessage,
                                     WPARAM wParam, LPARAM lParam );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );
   virtual zSHORT MapChildren( zBOOL  bToOI, zBOOL  bCreateDestroy );
   virtual zBOOL  SetFontOverride( );

   virtual void   CreateZ( ) = 0;
   virtual void   DestroyZ( );
   virtual zLONG  HandleEvent( zLONG lEvent );
   virtual zBOOL  BeginDrag( ZDragDrop *pDragDrop );
   virtual zBOOL  QueryDrop( ZDragDrop *pDragDrop );
   virtual zBOOL  DoDrop( ZDragDrop *pDragDrop );
   virtual zBOOL  Drop( ZDragDrop *pDragDrop );
   virtual zLONG  GetDragDropEventId( eDRAGDROP_EVENT eEventType = eDoDrop );
   virtual zBOOL  IsEventHandled( zLONG lEvent );
   virtual zLONG  SelectAll( );
   virtual zLONG  TransferTextFromInplace( zCPCHAR cpcText );

   zBOOL  TranslateValue( zPCHAR pchValue, zLONG lMaxLth, zBOOL bToDisplay, zSHORT nCol = 0 );
   void   SetTranslationList( zCPCHAR cpcPairedList, zSHORT nCol );

   ZDrView * GetFormView( );
   zBOOL   IsAncestor( ZMapAct *pzma ) const;
   zUSHORT GetMappingAutoGray( zVIEW   *pAppView,
                               zCPCHAR cpcVName,
                               zCPCHAR cpcEName,
                               zCPCHAR cpcAName );
#ifdef zREMOTE_SERVER
   zVIEW   SetupRemoteObject( zBOOL bCreate = TRUE );
#endif

   // Helper methods for controls default event handling
   // like a click in a checkbox.
   zLONG   RegisterEvent( zLONG lEvent );

   COleDispatchDriver * GetWrapperInstance( );
   void SetWrapperInstance( COleDispatchDriver *pWrapperInstance );
   IDispatch * GetDispatchInterface( );

   static zLONG g_lIdNbr;
   zULONG  m_ulMapActIdNbr;    // unique IdNbr assigned in ctor
   zLONG   m_lKeyId;           // the IdNbr ("ZKey") of this control
   ZSubtask *m_pZSubtask;      // pointer to owner window of this child window
   CWnd    *m_pCtrl;           // Ctrl CWnd object address
   HWND    m_hWndChild;        // First child of the control (if it exists)
   ZAttr   Attr;
   ZMapAct *m_pzmaComposite;   // Composite (parent control) to which this control belongs
   CString m_csERemote;        // Remote entity name (subtype) of the ctrl
   CString *m_pzsTag;          // Name of the control ... used for external location
   CString *m_pzsVName;        // View name to application map object
   CString *m_pzsEName;        // Entity name within map object
   CString *m_pzsAName;        // Attribute name within map object
   CString *m_pzsContext;      // Domain context for this attribute
   CString *m_pzsText;         // default text for the control
   zPCHAR  m_pchEventAction;   // Event-Action pairs associated with this control
   zSHORT  m_nEventActionCnt;  // Number of Event-Action pairs
   zPCHAR  m_pchDragDrop;      // Drag & Drop information for this control
   zSHORT  m_nIdNbr;           // Ctrl Identifier
   zULONG  m_lIdx;             // Ctrl Index into the binary search array
   zSHORT  m_nDIL;             // Ctrl Index into the DIL message entity
   zLONG   m_lRelPsX;          // > 0 ==> position is a percent of the parent window size
   zLONG   m_lRelPsY;          // > 0 ==> position is a percent of the parent window size
   zLONG   m_lRelSzX;          // > 0 ==> size is a percent of the parent window size
   zLONG   m_lRelSzY;          // > 0 ==> size is a percent of the parent window size
   zSHORT  m_nRelSizeCntX;     // > 0 ==> # of relative size ctrls left of this ctrl
   zSHORT  m_nRelSizeTotalX;   // > 0 ==> # of relative size ctrls in X direction
   zSHORT  m_nRelSizeCntY;     // > 0 ==> # of relative size ctrls above this ctrl
   zSHORT  m_nRelSizeTotalY;   // > 0 ==> # of relative size ctrls in Y direction
   COLORREF m_clrText;         // non-zero ==> override text color
   COLORREF m_clrBk;           // non-zero ==> override background color
   CBrush  *m_pBrush;          // non-zero ==> override background brush
   CFont   *m_pFont;           // non-zero ==> override font
   zLONG   m_ulCtrlErr;        // control error flag
   zULONG  m_ulMapActFlags;    // visible, enabled, ..., flags
   zULONG  m_ulMapActFlag2;    // created, mapped, ..., flags
   CWnd    *m_pWndParent;
   CWnd    *m_pWndLastFocus;
   HWND    m_hWndFullClient;

   zPCHAR  m_pchPairedList;

   CPoint  m_ptMouseDown;
   CPoint  m_ptMouseUp;
   zCHAR   m_chQuickKey;
   ZCorrelatedList *m_pTranslationList;  // for "ad hoc domains"
   IDispatch *m_pDispatch;
   COleDispatchDriver *m_pWrapperInstance;
   CDWordArray m_aHandledEvents; // which events are handled by Control wrapper
};

#define zPOPUP_MENU_FIRST          1024

/////////////////////////////////////////////////////////////////////////////
// Zeidon application option (localization) object.  Define the data type
// which the tree is to hold.  Then we can declare a binary tree container
// object for this type.
//
class ZDRAPP_CLASS ZOption
{
public:
   // ctor
   ZOption( zVIEW    vSubOption,
            zKZWDLGXO_Opt_DEF *pOptDef,
            zKZWDLGXO_OptMap_DEF *pOptMapDef,
            zUSHORT  uID,
            ZOption  *pParent,
            ZSubtask *pZSubtask );

   ZOption( zUSHORT  uID,
            zCPCHAR  cpcTag,
            zCPCHAR  cpcText,
            zCPCHAR  cpcActionTag,
            zBOOL    bChecked,
            ZOption  *pParent,
            ZSubtask *pZSubtask );

   // dtor
   virtual ~ZOption( );

   zBOOL SetCheckForOption( );
   CMenu * GetMenu( ZSubtask *pZSubtask );
   zBOOL RemoveMenu( ZSubtask *pZSubtask );
   zBOOL InsertMenu( CMenu *pMenu, ZSubtask *pZSubtask );

   ZSubtask *m_pZSubtask;
   HMENU    m_hMenu;            // menu of which this option is a part
   ZOption  *m_pParent;         // non-zero ==> option is part of a sub-menu
   CString  *m_pzsTag;          // Option tag
   CString  *m_pzsText;         // Option text
   CString  *m_pzsVName;        // View name to application map object
   CString  *m_pzsEName;        // Entity name within data object
   CString  *m_pzsAName;        // Attribute name within data object
   CString  *m_pzsContext;      // Domain context for this attribute
   CString  *m_pzsValueOn;      // Checkmark ON value to be mapped
   CString  *m_pzsValueOff;     // Checkmark OFF value to be mapped
   CString  *m_pzsActionTag;    // Action associated with this option
   zBOOL    m_bChecked;         // TRUE ==> option is checked ... FALSE otw
   zBOOL    m_bEnabled;         // TRUE ==> option is enabled ... FALSE otw
   zBOOL    m_bVisible;         // TRUE ==> option is visible ... FALSE otw
   zBOOL    m_bParent;          // TRUE ==> option has sub-menus ... FALSE otw
   zLONG    m_lOptionFlags;     // visible, enabled, ..., flags
   zUSHORT  m_uParentID;        // Parent Option Identifier
   zUSHORT  m_uOptionID;        // Option Command Identifier
   zUSHORT  m_uOptionPos;       // Option Position (relative)
   zUSHORT  m_uSeparator;       // Separator count
   zSHORT   m_nDIL;             // Option Index into the DIL message entity

protected:

private:
};

/////////////////////////////////////////////////////////////////////////////
// Zeidon application option (localization) object
// Define the data type which the tree is to hold.  Then we can declare
// a binary tree container object for this type
class ZDRAPP_CLASS ZMapping
{
public:
   // ctor
   ZMapping( zKZWDLGXO_CtrlMap_DEF CtrlMapDef );
   ZMapping( zCPCHAR cpcTag );

   // dtor
   virtual ~ZMapping( );

   CString  m_csTag;          // Mapping tag
   CString  m_csVName;        // View name to application map object
   CString  m_csEName;        // Entity name within data object
   CString  m_csAName;        // Attribute name within data object
   CString  m_csContext;      // Domain context for this attribute
   zCHAR    m_chType;         // Entity/Scope/Attribute/Multiple attributes
   zULONG   m_ulDisplayLth;
};

/////////////////////////////////////////////////////////////////////////////

typedef struct ZFindDialog_tag
{
   ZSubtask *pZSubtask;
   zPVOID   pvDialog;
   zLONG    lTID;      // Task or Thread Id
   HWND     hWnd;
   zCHAR    DlgN[ zTAG_LTH ];
   zCHAR    WndN[ zTAG_LTH ];
   zULONG   ulSubtaskState;

} ZFindDialog;

#ifdef zREMOTE_SERVER
typedef struct ZFindSocket_tag
{
   ZSocketTask *pZSocketTask;
   zULONG      ulSocketState;

} ZFindSocket;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZDrOleClient class

class ZDRAPP_CLASS ZDrOleClient : public COleClientItem
{
   DECLARE_SERIAL(ZDrOleClient)

// Constructors
public:
   // Note: pContainer is allowed to be NULL to enable IMPLEMENT_SERIALIZE.
   //  IMPLEMENT_SERIALIZE requires the class have a constructor with
   //  zero arguments.  Normally, OLE items are constructed with a
   //  non-NULL document pointer.
   ZDrOleClient( ZDrDoc  *pContainer = 0 );
   ZDrOleClient( ZDrDoc  *pContainer,
                 ZAttr   *pAttr,
                 zBOOL   bFullClient );

// Attributes
public:
   ZDrDoc * GetDocument( );
   ZDrView * GetActiveView( );

   virtual BOOL FinishCreate( SCODE sc );
   BOOL IsInvisibleAtRuntime( ) const;
   void SetParentView( CView *p );
   CView * GetParentView( );
   LPDISPATCH GetObjectDispatch( );
   void SetupTracker( CRectTracker &rect ) const;
   CRect GetRect( ) const;

   ZAttr    *m_pAttr;
   zBOOL    m_bFullClient;
   zBOOL    m_bWindowless;
   DWORD    m_dwMiscStatus;
   IOleInPlaceObjectWindowless *m_pOleInPlaceObjectWindowless;

protected:
   virtual BOOL InterfaceConnect( IUnknown *pObj, REFIID riid,
                                  IUnknown *pIUnknownSink, LPDWORD pdwConn );
   virtual BOOL InterfaceDisconnect( IUnknown *pObj, REFIID riid,
                                     LPDWORD pdwConn );

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDrOleClient)
   public:
   virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
   virtual void OnActivate( );
   virtual void OnActivateUI( );
   protected:
   virtual void OnGetItemPosition(CRect& rPosition);
   virtual void OnDeactivateUI(BOOL bUndoable);
   virtual BOOL OnChangeItemPosition(const CRect& rectPos);
   //}}AFX_VIRTUAL

// Implementation
public:

   void OnDeactivate( );
   void OnDeactivate( BOOL tNoRedraw );

   ~ZDrOleClient( );
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump(CDumpContext& dc) const;
#endif
   virtual void Serialize(CArchive& ar);

protected:
   CDC *m_pDC;

   // Added to support TB ActiveX Editor - 1998/4/18
private:
   COleDispatchDriver *m_pDispatchInstance;
   CView   *m_pParentView;
   IID     m_iidEditorEvents;
   DWORD   m_dwConnKey;
   // Added to support TB ActiveX Editor - 1998/4/18

   // Support additional Interfaces.

   BEGIN_INTERFACE_PART( OleInPlaceSiteWindowless, IOleInPlaceSiteWindowless )
      STDMETHOD( ContextSensitiveHelp )( BOOL tEnterMode );
      STDMETHOD( GetWindow )( HWND* phWnd );

      STDMETHOD( CanInPlaceActivate )( );
      STDMETHOD( DeactivateAndUndo )( );
      STDMETHOD( DiscardUndoState )( );
      STDMETHOD( GetWindowContext )( IOleInPlaceFrame** ppFrame,
                                     IOleInPlaceUIWindow** ppDoc,
                                     LPRECT prectPos, LPRECT prectClip,
                                     LPOLEINPLACEFRAMEINFO pFrameInfo );
      STDMETHOD( OnInPlaceActivate )( );
      STDMETHOD( OnInPlaceDeactivate )( );
      STDMETHOD( OnPosRectChange )( LPCRECT prectPos );
      STDMETHOD( OnUIActivate )( );
      STDMETHOD( OnUIDeactivate )( BOOL tUndoable );
      STDMETHOD( Scroll )( SIZE sizeExtent );

      STDMETHOD( OnInPlaceActivateEx )( BOOL* ptNoRedraw, DWORD dwFlags );
      STDMETHOD( OnInPlaceDeactivateEx )( BOOL tNoRedraw );
      STDMETHOD( RequestUIActivate )( );

      STDMETHOD( AdjustRect )( LPRECT prect );
      STDMETHOD( CanWindowlessActivate )( );
      STDMETHOD( GetCapture )( );
      STDMETHOD( GetDC )( LPCRECT prect, DWORD dwFlags, HDC* phDC );
      STDMETHOD( GetFocus )( );
      STDMETHOD( InvalidateRect )( LPCRECT prect, BOOL tErase );
      STDMETHOD( InvalidateRgn )( HRGN hRgn, BOOL tErase );
      STDMETHOD( OnDefWindowMessage )( UINT nMessage, WPARAM wParam,
                                       LPARAM lParam, LRESULT* plResult );
      STDMETHOD( ReleaseDC )( HDC hDC );
      STDMETHOD( ScrollRect )( int dx, int dy, LPCRECT prectScroll,
                               LPCRECT prectClip );
      STDMETHOD( SetCapture )( BOOL tCapture );
      STDMETHOD( SetFocus )( BOOL tFocus );
   END_INTERFACE_PART( OleInPlaceSiteWindowless );

   DECLARE_INTERFACE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZDrDoc class
//
class ZDRAPP_CLASS ZDrDoc : public COleDocument
{
// DECLARE_DYNAMIC( ZDrDoc )
   DECLARE_DYNCREATE( ZDrDoc )

protected: // create from serialization only
   ZDrDoc( );

public:
   virtual ~ZDrDoc( );

   // Added to support TB ActiveX Editor - 1998/4/18
   void  SetDocTemplateMenu( HMENU hMenu );
   // Added to support TB ActiveX Editor - 1998/4/18

// Attributes
public:
   ZSubtask      *m_pZSubtask;
// ZSplitterData *m_pSplitterData;

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDrDoc)
   public:
   virtual BOOL OnNewDocument( );
   virtual void Serialize(CArchive& ar);
   //}}AFX_VIRTUAL

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
   //{{AFX_MSG(ZDrDoc)
      // NOTE - the ClassWizard will add and remove member functions here.
      //    DO NOT EDIT what you see in these blocks of generated code !
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// CMappedBitmapButton window
//
// Usage -- a replacement for the MFC CBitmapButton
// 1. include an owner-draw button in your dialog
// 2. declare a CMappedBitmapButton member in the CDialog code
// 3. hook in the CMappedBitmapButton using a call to AutoLoad
//
// the bitmap resource specified in AutoLoad must be divisable into
// 4 equally sized images that represent (left to right) the
// up, down, focused and disabled states of the button
//
class ZMappedBitmapButton : public CButton
{
// Construction
public:
   DECLARE_DYNAMIC( ZMappedBitmapButton  )
   ZMappedBitmapButton( );

// Attributes
public:

protected:
   CImageList m_image;
   UINT       m_idResource;

// Operations
public:
   BOOL LoadBitmap( UINT idBitmapResource );
   void SizeToContent();
   BOOL AutoLoad(UINT nID, CWnd* pParent, UINT idBitmapResource);

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZMappedBitmapButton)
   public:
   virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZMappedBitmapButton();

   // Generated message map functions
protected:
   //{{AFX_MSG(CMappedBitmapButton)
   afx_msg void OnSysColorChange();
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZDrView printing

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDRAPP_CLASS ZDrPreviewView : public CPreviewView
{
// DECLARE_DYNAMIC( ZDrPreviewView )
   DECLARE_DYNCREATE( ZDrPreviewView )

public:
// ZDrPreviewView( ZSubtask *pZSubtask );
   virtual ~ZDrPreviewView( );

protected:
   ZDrPreviewView( ); // create from serialization only

// Attributes
   ZSubtask     *m_pZSubtask;

   ZMappedBitmapButton m_print;
   ZMappedBitmapButton m_next;
   ZMappedBitmapButton m_previous;
   ZMappedBitmapButton m_onetwo;
   ZMappedBitmapButton m_zoomIn;
   ZMappedBitmapButton m_zoomOut;

   zBOOL m_bOne;
   zBOOL m_bEnableZooming;
// Operations
public:

// Overrides
   virtual void OnDisplayPageNumber(UINT nPage, UINT nPagesDisplayed);

   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDrPreviewView)
   //}}AFX_VIRTUAL

#ifdef _DEBUG
   virtual void AssertValid() const;
   virtual void Dump(CDumpContext& dc) const;
#endif

   // Generated message map functions
protected:
   //{{AFX_MSG(ZDrPreviewView)
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSize( UINT nType, int cx, int cy );
   afx_msg void OnPreviewPrint( );
   afx_msg void OnUpdateNumPageChange( CCmdUI *pCmdUI );
   afx_msg void OnUpdateZoomIn( CCmdUI *pCmdUI );
   afx_msg void OnUpdateZoomOut( CCmdUI *pCmdUI );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZDrView form view
//
class ZDRAPP_CLASS ZDrView : public CFormView  // CScrollView doesn't work
{
// DECLARE_DYNAMIC( ZDrView )
   DECLARE_DYNCREATE( ZDrView )

public:
   // Can be used to create from serialization.
   ZDrView( ZSubtask *pZSubtask = 0 );
   virtual ~ZDrView( );

// Attributes
public:
   ZDrDoc * GetDocument( );
   // m_pSelection holds the selection to the current ZDrOleClient.
   // For many applications, such a member variable isn't adequate to
   // represent a selection, such as a multiple selection or a selection
   // of objects that are not ZDrOleClient objects.  This selection
   // mechanism is provided just to help you get started.

   // TODO: replace this selection mechanism with one appropriate to your app.

   BOOL Create( LPCTSTR cpcClassName, LPCTSTR cpcWindowName,
                DWORD dwRequestedStyle, const RECT& rect,
                CWnd *pParentWnd, UINT nID, CCreateContext *pContext );
   void Repaint( ZMapAct *pzmaParent = 0 );  // force repaint of controls

// Operations
public:
   HRESULT OnWindowlessDefWindowMessage( UINT nMessage, WPARAM wParam,
                                         LPARAM lParam, LRESULT *plResult );
   HRESULT OnWindowlessReleaseCapture( ZDrOleClient *pItem );
   HRESULT OnWindowlessReleaseFocus( );
   HRESULT OnWindowlessSetCapture( ZDrOleClient *pItem );
   HRESULT OnWindowlessSetFocus( ZDrOleClient *pItem );
   ZDrOleClient * GetItemWithCapture( ) const;
   ZDrOleClient * GetItemWithFocus( ) const;
   ZAmbientProperty * FindAmbientProperty( DISPID dispid );
   void InitAmbientProperties( );
   ZAmbientProperty * FindAmbientProperty( LPCOLESTR pszName );
   void RemoveAmbientProperties( );

   BOOL CheckSplitterFocus( MSG *pMsg );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZDrView)
public:
// void OnToolbarDropDown( NMHDR *lpnmhdr, LRESULT *plResult );

protected:
   virtual BOOL PreCreateWindow( CREATESTRUCT& cs );
   virtual BOOL OnAmbientProperty( COleControlSite *pSite,
                                   DISPID  dispid,
                                   VARIANT *pVariant );
   virtual void OnInitialUpdate( ); // called first time after construct

   virtual BOOL OnPreparePrinting( CPrintInfo *pInfo );
   virtual void OnPrepareDC( CDC *pDC, CPrintInfo *pInfo );
   virtual void OnBeginPrinting( CDC *pDC, CPrintInfo *pInfo );
   virtual void OnPrint( CDC *pDC, CPrintInfo *pInfo );
   virtual void OnDraw( CDC *pDC );  // overridden to draw this view
   virtual void OnEndPrinting( CDC *pDC, CPrintInfo *pInfo );

   virtual BOOL PreTranslateMessage( MSG *pMsg );

   virtual BOOL IsSelected( const CObject *pDocItem ) const;// Container support
   virtual void OnEndPrintPreview( CDC *pDC, CPrintInfo *pInfo,
                                   POINT pt, CPreviewView *pView );
   //}}AFX_VIRTUAL


   // Generated message map functions
   //{{AFX_MSG(ZDrView)
protected:
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg HBRUSH OnCtlColor( CDC *pDC, CWnd *pWnd, UINT nCtlColor );
   afx_msg void OnPaint( );
   afx_msg LRESULT OnCommonFind( WPARAM wParam, LPARAM lParam );
   afx_msg void OnLButtonDown( UINT modKeys, CPoint pt );
   afx_msg void OnLButtonUp( UINT modKeys, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT modKeys, CPoint pt );
   afx_msg void OnMButtonDown( UINT modKeys, CPoint pt );
   afx_msg void OnMButtonUp( UINT modKeys, CPoint pt );
   afx_msg void OnMButtonDblClk( UINT modKeys, CPoint pt );
   afx_msg void OnRButtonDown( UINT modKeys, CPoint pt );
   afx_msg void OnRButtonUp( UINT modKeys, CPoint pt );
   afx_msg void OnRButtonDblClk( UINT modKeys, CPoint pt );
   afx_msg void OnMouseMove( UINT modKeys, CPoint pt );
   afx_msg void OnFilePrintPreview( );
   afx_msg int  OnMouseActivate( CWnd *pDesktopWnd, UINT nHitTest, UINT message );
   afx_msg void OnHScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT nSBCode, UINT uThumbPos, CScrollBar *pScrollBar );
   afx_msg void OnDestroy( );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnSize( UINT sizeType, int cx, int cy );
   afx_msg void OnInsertObject( );
   afx_msg void OnCancelEditCntr( );
   //}}AFX_MSG

// Implementation
public:
#ifdef _DEBUG
   virtual void AssertValid( ) const;
   virtual void Dump( CDumpContext& dc ) const;
#endif

public:
   static zLONG g_lIdNbr;
   zULONG        m_ulViewIdNbr;         // unique IdNbr assigned in ctor
   ZSubtask      *m_pZSubtask;
   ZDrOleClient  *m_pItemWithFocus;
   ZDrOleClient  *m_pItemWithCapture;
   ZDrOleClient  *m_pSelection;
   zLONG         m_lScrollPosX;
   zLONG         m_lPageSizeX;
   zLONG         m_lScrollPosY;
   zLONG         m_lPageSizeY;
   CBrush        *m_pBrushBk;           // background brush override
   CWnd          *m_pOrigParent;

   enum ePaneId { eUndef = 0, eTop = 1, eLeft = 2, eRight = 4, eBottom = 8 };
   ePaneId       m_eQuad;
   ZSplitterView *m_pParentSplitterView;
   ZSplitterWnd  *m_pParentSplitterWnd;
   ZDrView       *m_pPrevView;
   ZDrView       *m_pNextView;

protected:
   static CString m_csClassName;

   // Ambient properties for control interaction at designtime
   CArray<ZAmbientProperty *, ZAmbientProperty *> m_apList;
   CFontHolder *m_pFontHolder;
   FONTDESC    m_descFont;
   zBOOL       m_bTraceAmbientPropertyRequest;

// zBOOL m_bInitialized;
// zBOOL m_bShouldSetXColumn;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDRAPP_CLASS ZFont : public CFont
{
public:
   ZFont( zVIEW   vSubtask,
          zCPCHAR cpcTag,
          zLONG   nPointSize,
          zSHORT  nWeight = FW_NORMAL,
          zSHORT  bItalic = FALSE,
          zSHORT  bUnderline = FALSE,
          zSHORT  bStrikeout = FALSE,
          zLONG   lTextColor = 0,
          zLONG   lTextBkColor = 0,
          zSHORT  nEscapement = 0,
          zCPCHAR cpcFaceName = 0 );
   ZFont( zVIEW vSubtask, zCPCHAR cpcTag, zCPCHAR cpcFaceName,
          int nPointSize, CDC *pDC = 0, zLONG lTextColor = 0 );
   virtual ~ZFont( );

   zSHORT    m_nIdNbr;
   zBOOL     m_bSet;
   ZNameList m_TagList;
   COLORREF  m_clrText;       // non-zero ==> override text color
   COLORREF  m_clrTextBk;     // non-zero ==> override text color

   zLONG     m_nPointSize;
   zSHORT    m_nWeight;
   zSHORT    m_nFlags;        // 1 - italic   2 - underscore  4 - negative
                              // 8 - outline 16 - strikeout  32 - bold
   zSHORT    m_nEscapement;
   CString   m_csFaceName;
   PANOSE    m_Panose;
   BYTE      m_byCharset;     // 0 - ANSI     1 - DEFAULT     2 - SYMBOL ...
   BYTE      m_byPitchFamily; // 0 - nil      1 - roman       2 - swiss
                              // 3 - modern   4 - script      5 - decor
                              // 6 - tech     7 - bidi
};

class ZDRAPP_CLASS ZViewPort
{
public:
   ZViewPort( zCPCHAR cpcCurrentEntityName,
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
              zLONG   lViewPortRight );
   virtual ~ZViewPort( );

   zCHAR   m_szCurrentEntityName[ zTAG_LTH ];
   zLONG   m_lRepeatHorizontal;
   zLONG   m_lRepeatHorizontalColumn;
   zLONG   m_lRepeatHorizontalIndent;
   zLONG   m_lRepeatHorizontalMargin;
   zLONG   m_lRepeatHorizontalExtent;
   zLONG   m_lRepeatHorizontalFlags;
   zLONG   m_lViewPortBottomPrev;
   zLONG   m_lViewPortTop;
   zLONG   m_lViewPortLeft;
   zLONG   m_lViewPortBottom;
   zLONG   m_lViewPortRight;
   ZViewPort *m_pViewPortNext;
};

class ZDRAPP_CLASS ZPrintout
{
public:
   ZPrintout( ZSubtask *pZSubtask,
              zCPCHAR  cpcDLL_Name,
              zCPCHAR  cpcPrintPageFunction,
              zLONG    lPrintFlags,
              zSHORT   bBanding,
              zSHORT   bScale,
              zLONG    bPrompt,
              zCPCHAR  cpcReportDefName,
              zPCHAR   pchReportName,
              zCPCHAR  cpcCSS_FileName );

   ZPrintout( ZTask    *pZTask,
              zCPCHAR  cpcDLL_Name,
              zCPCHAR  cpcPrintPageFunction,
              zLONG    lPrintFlags,
              zSHORT   bBanding,
              zSHORT   bScale,
              zLONG    bPrompt,
              zCPCHAR  cpcReportDefName,
              zPCHAR   pchReportName,
              zCPCHAR  cpcCSS_FileName );

   virtual ~ZPrintout( );

   void   AddFont( zVIEW   vSubtask,
                   zCPCHAR cpcTag,
                   zLONG   nPointSize,
                   zSHORT  nWeight = FW_NORMAL,
                   zSHORT  bItalic = FALSE,
                   zSHORT  bUnderline = FALSE,
                   zSHORT  bStrikeout = FALSE,
                   zLONG   lTextColor = 0,
                   zLONG   lTextBkColor = 0,
                   zSHORT  nEscapement = 0,
                   zCPCHAR cpcFaceName = 0 );
   zBOOL  RemoveFont( ZFont *pFont );
   ZFont  * LocateFont( zCPCHAR cpcTag );
   ZFont  * LocateFont( zLONG nPtSize, zSHORT nWeight,
                        zSHORT nFlags, zSHORT nEscapement,
                        zCPCHAR cpcFaceName );
   zSHORT ListFontsAndColors( zPCHAR pchFontList, zLONG lMaxLth );
   zBOOL  SetPhantomMode( zBOOL bPhantom );
   zBOOL  GetPhantomMode( );
   zLONG  HandlePageNumbering( zPCHAR pchPageNumber,
                               zLONG  lEntityKey,
                               zLONG  lPageNbr,
                               zPLONG plTotalPages,
                               zLONG  lFlags );
   zSHORT AddPagePosition( zLONG lPageNbr, zVIEW vSourceHierOI,
                           zVIEW vReportDef, zLONG lPagePosition,
                           zLONG lRC, zLONG lLevel, zLONG lFlags,
                           zLONG lMultiLineTextPos,
                           zLONG lContinuedGroupFlag,
                           zCPCHAR pchEntityName,
                           zCPCHAR cpcRptGroupSetTag,
                           zCPCHAR cpcRptGroupTag );
   zSHORT GetPagePosition( zLONG  lPageNbr,
                           zVIEW  vSourceHierOI,
                           zVIEW  vReportDef,
                           zPLONG plPagePosition, zPLONG plRC,
                           zPLONG plLevel, zPLONG plFlags,
                           zPLONG plMultiLineTextPos,
                           zPLONG plContinuedGroupFlag,
                           zPCHAR pchEntityName,
                           zPCHAR pchRptGroupSetTag,
                           zPCHAR pchRptGroupTag );
   zBOOL  RemoteFilePrint( zVIEW vSubtask );
   zSHORT PushTabStops( zSHORT nTabStop );
   zSHORT PopTabStops( );
   zLONG  PushViewPort( zCPCHAR cpcCurrentEntityName,
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
                        zLONG   lViewPortRight );
   zLONG  PopViewPort( zPCHAR  pchCurrentEntityName,
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
                       zPLONG  plViewPortRight );
   int    RptDrawText( CDC *pDC, const CString& rcs, LPRECT lpRect, UINT uFormat );
   CPoint RptMoveTo( CDC *pDC, int x, int y );
   BOOL   RptLineTo( CDC *pDC, int x, int y );

   CDC        *m_pDC;
   CPrintInfo *m_pPrintInfo;

   ZFont      *m_pFontCurr;
   CSize      m_sizePage;
   zBOOL      m_bPrintPreview;
   zBOOL      m_bPrintToPDF;
   zBOOL      m_bPrintToCSV;
   zBOOL      m_bCountPages;
   zBOOL      m_bPrintToHTML;
   zBOOL      m_bPrintToCtrl;
   zBOOL      m_bResetPaging;
   zLONG      m_lLastColumnCSV;
// COLORREF   m_clrOrig;      // non-zero ==> override text color
   zSHORT     m_nPixelsPerInchX;
   zSHORT     m_nPixelsPerInchY;
   zSHORT     m_nPixelsOffsetX;
   zSHORT     m_nPixelsOffsetY;

   CWnd       *m_pWindow;
   zSHORT     m_nMapMode;

   int        m_nPrevMode;
   CSize      m_sizeOldVExt;
   CSize      m_sizeOldWExt;
   CRect      m_rectOrg;

   zLONG      m_lHorzSize256ths;
   zLONG      m_lVertSize256ths;
   zLONG      m_lHorzRes;
   zLONG      m_lVertRes;

   ZSubtask   *m_pZSubtask;
   ZTask      *m_pZTask;
   zVIEW      m_vSubtask;
   zPCHAR     m_pchDLL_Name;
   zPCHAR     m_pchPrintPageFunction;
   zSHORT     m_bBanding;
   zLONG      m_bPrompt;
   zSHORT     m_bScale;
   zBOOL      m_bLandscape;
   zBOOL      m_bDropView;
   zBOOL      m_bPhantom;
   zLONG      m_lFileCSV;

   CString    m_csReportFileName;
   CString    m_csReportDefName;
   CString    m_csReportInfo;
   zCHAR      m_szDefaultPrinter[ zMAX_FILESPEC_LTH ];

   ZPageItem  *m_pFirstPage;
   ZNumberList m_FontList;
   zSHORT     m_nFontMax;
   zSHORT     m_nFontCnt;
   zFARPROC_PRINT m_lpfnPrintPageRoutine;
   LPLIBRARY  m_hLibrary;         // Library from which to run operations
   zUSHORT    m_uMaxPageNbr;
   zSHORT     m_nEscapementFactor;
   zLONG      m_lPageCnt;
   zLONG      m_lWordCnt;
   zLONG      m_lCharCnt;
   double     m_dHeight;
   zSHORT     m_nTabStops;
   ZNumberItem *m_pTabStopHead;
   ZViewPort  *m_pViewPortHead;
   ZPairedItem *m_pResetPagingHead;
   ZPairedItem *m_pResetPagingTail;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZSubtaskList dialog
//
class ZSubtaskList : public CDialog
{
// Construction
public:
   ZSubtaskList( zVIEW vSubtask, CWnd *pParent = NULL );   // standard constructor

// Dialog Data
   //{{AFX_DATA(ZSubtaskList)
   enum { IDD = IDD_SUBTASKS };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA


// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZSubtaskList)
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG(ZSubtaskList)
   virtual BOOL OnInitDialog( );
   virtual void OnOK( );
   afx_msg void OnDblclkListSubtasks( NMHDR *pNMHDR, LRESULT *pResult );
   //}}AFX_MSG

   zVIEW m_vSubtask;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZModalDlg dialog
//
class ZModalDlg : public CDialog
{
// Construction
public:
   ZModalDlg( zPCHAR   pchReturnCtrlTag,
              ZSubtask *pZSubtaskParent,
              zCPCHAR  cpcDlgTag,
              zCPCHAR  cpcWndTag,
              zVIEW    vDialog );
   virtual ~ZModalDlg( );

// Dialog Data
   //{{AFX_DATA(ZModalDlg)
   enum { IDD = IDD_MODALDLG };
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(ZModalDlg)
   protected:
   virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
   //}}AFX_VIRTUAL

// Implementation
protected:

   ZSubtask *m_pZSubtask;
   zPCHAR   m_pchReturnCtrlTag;

   afx_msg void OnSysCommand( UINT nID, LPARAM lParam );
   afx_msg LRESULT OnProcessEvent( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnProcessAction( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnProcessHotkey( WPARAM wParam, LPARAM lParam );
   afx_msg LRESULT OnPostedClose( WPARAM wParam, LPARAM lParam );

   // Generated message map functions
   //{{AFX_MSG(ZModalDlg)
   virtual BOOL OnInitDialog( );
   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual void OnOK( );
   virtual void OnCancel( );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// ZDrMemory
//
class ZDRAPP_CLASS ZDrMemory
{
// Construction
public:
   ZDrMemory( zLONG lInitialSize );
   virtual ~ZDrMemory( );

   // Returns: Resultant length of string in memory
   zLONG  AddStringToMemory( zCPCHAR cpcString, zLONG lFlag );

   zPCHAR GetStringFromMemory( ) { return( m_pchMemory ); }
   void   ClearMemory( ) { m_pchMemory[ 0 ] = 0; m_lUsed = 0; }

private:

   zPCHAR m_pchMemory;
   zLONG  m_lMemorySize;
   zLONG  m_lUsed;

};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#define zMAX_BLOB                            128000L

#define zWND_EVENT_ACCELERATOR               0x00000000L
#define zWND_EVENT_PRE_BUILD                 0x00000001L
#define zWND_EVENT_POST_BUILD                0x00000002L
#define zWND_EVENT_ON_ACTIVATE               0x00000004L
#define zWND_EVENT_ON_RETURN                 0x00000008L
#define zWND_EVENT_CHILD_LOSE_FOCUS          0x00000010L
#define zWND_EVENT_MINIMIZE                  0x00000020L
#define zWND_EVENT_MAXIMIZE                  0x00000040L
#define zWND_EVENT_RESTORE                   0x00000080L
#define zWND_EVENT_CLOSE                     0x00000100L
#define zWND_EVENT_ON_REFRESH                0x00000200L
#define zWND_EVENT_NOTIFYCHILD_ACTIVATE      0x00000400L
#define zWND_EVENT_NOTIFYCHILD_PALETTE       0x00000800L
#define zWND_EVENT_ON_PALETTE_CHANGE         0x00001000L
#define zWND_EVENT_ON_SIZE                   0x00002000L
#define zWND_EVENT_MOUSE_MOVE                0x00004000L
#define zWND_EVENT_CAN_CLOSE                 0x00008000L
#define zWND_EVENT_LBUTTON_DOWN              0x00010000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_LBUTTON_UP                0x00020000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_LBUTTON_CLICK             0x00030000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_LBUTTON_DBLCLK            0x00040000L // the flags in
#define zWND_EVENT_LBUTTON_DRAG              0x00050000L // this group
#define zWND_EVENT_LBUTTON_DRAG_DROP         0x00060000L // require special
#define zWND_EVENT_RBUTTON_DOWN              0x00070000L // code to test
#define zWND_EVENT_RBUTTON_UP                0x00080000L // for equality
#define zWND_EVENT_RBUTTON_CLICK             0x00090000L // ... NOT bitwise
#define zWND_EVENT_RBUTTON_DBLCLK            0x000a0000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_RBUTTON_DRAG              0x000b0000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_RBUTTON_DRAG_DROP         0x000c0000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_ON_ACQUIRE                0x000d0000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_ON_MOVE                   0x00100000L
#define zWND_EVENT_ON_SETUP                  0x00200000L
#define zWND_EVENT_ON_CLEANUP                0x00400000L
#define zWND_EVENT_ON_MAPTOOI                0x00800000L
#define zWND_EVENT_FILE_NEW                  0x01000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_FILE_OPEN                 0x02000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_FILE_CLOSE                0x03000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_FILE_CLOSEALL             0x04000000L // the flags in
#define zWND_EVENT_FILE_SAVE                 0x05000000L // this group
#define zWND_EVENT_FILE_SAVEAS               0x06000000L // require special
#define zWND_EVENT_EDIT_UNDO                 0x07000000L // code to test
#define zWND_EVENT_EDIT_REDO                 0x08000000L // for equality
#define zWND_EVENT_EDIT_FIND                 0x09000000L // ... NOT bitwise
#define zWND_EVENT_EDIT_NEXT                 0x0A000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_EDIT_REPLACE              0x0B000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_HELP_ABOUT                0x0C000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_FILE_EXIT                 0x0D000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_MDI_CHILD_SIZE            0x0E000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_MDI_CHILD_SETFOCUS        0x0F000000L // !!!!!!!!!!!!!!!
#define zWND_EVENT_MENU_PRE_DROPDOWN         0x10000000L
#define zWND_EVENT_POPUP_PRE_DROPDOWN        0x20000000L
#define zWND_EVENT_CHILD_RECEIVE_FOCUS       0x40000000L
#define zWND_EVENT_ACTION_AUTOGRAY           0x80000000L

#define zWND_EVENTX_COMMON_FIND              0x00000010L
#define zWND_EVENTX_MBUTTON_DOWN             0x00000040L
#define zWND_EVENTX_MBUTTON_UP               0x00000080L
#define zWND_EVENTX_MBUTTON_CLICK            0x00000100L
#define zWND_EVENTX_MBUTTON_DBLCLK           0x00000200L
#define zWND_EVENTX_MBUTTON_DRAG             0x00000400L
#define zWND_EVENTX_MBUTTON_DRAG_DROP        0x00000800L
#define zWND_EVENTX_FILE_PRINTER_SETUP       0x00100000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_FILE_PRINT_PREVIEW       0x00200000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_FILE_PRINT               0x00300000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_EDIT_CUT                 0x00400000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_EDIT_COPY                0x00500000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_EDIT_PASTE               0x00600000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_EDIT_DELETE              0x00700000L // !!!!!!!!!!!!!!!
#define zWND_EVENTX_EDIT_CLEAR               0x00800000L // !!!!!!!!!!!!!!!
//#define zWND_EVENTX_HANDLE_KEY               0x00001000L

#define zCTRL_EVENT_BEGINDRAG                0x00100000L
#define zCTRL_EVENT_QUERYDROP                0x00100001L
#define zCTRL_EVENT_DODROP                   0x00100002L
#define zCTRL_EVENT_DROP                     0x00100003L

#define zACTION_DIALOG_CODE_ONLY             0x80000000L
#define zACTION_PREVENT_MAP                  0x00000001L
#define zACTION_PREVENT_AUTOVERSION          0x00000002L
#define zACTION_PREVENT_DIALOG_CODE          0x00000004L
#define zACTION_PREVENT_AUTO_ENABLE          0x00000008L

#define zTEMPORAL_SUBOBJECT_ACCEPT           0x00000001L
#define zTEMPORAL_SUBOBJECT_CANCEL           0x00000002L
#define zTEMPORAL_SUBOBJECT_DELETE           0x00000004L
#define zTEMPORAL_SUBOBJECT_EXCLUDE          0x00000008L
#define zTEMPORAL_SUBOBJECT_CREATE_ENTITY    0x00000010L
#define zTEMPORAL_SUBOBJECT_CREATE_TEMPORAL  0x00000020L
#define zTEMPORAL_SUBOBJECT_CREATE_VERSION   0x00000040L
#define zTEMPORAL_SUBOBJECT_NEXT             0x00000080L
#define zTEMPORAL_SUBOBJECT_PREV             0x00000100L
#define zTEMPORAL_SUBOBJECT_STAYWHENNULL     0x00000200L
#define zTEMPORAL_SUBOBJECT_RETURNWHENNULL   0x00000400L
#define zTEMPORAL_SUBOBJECT_VERIFY           0x10000000L

#define zAUTOGRAY_ACTION_FIRST               0x00000001L
#define zAUTOGRAY_ACTION_LAST                0x00000002L
#define zAUTOGRAY_ACTION_NOT_SELECTED        0x00000004L
#define zAUTOGRAY_ACTION_NULL                0x00000008L
#define zAUTOGRAY_ACTION_BROWSE_ONLY         0x00000010L
#define zAUTOGRAY_ACTION_NOT_NULL            0x00000020L
#define zAUTOGRAY_ACTION_READONLY            0x10000000L

#define zMOUSE_STATUS_OFF             (UINT) 0x0000
#define zMOUSE_SELECT_PENDING         (UINT) 0x0010
#define zMOUSE_CAPTURE                (UINT) 0x1000
#define zMOUSE_CAPTURE_LEFT           (UINT) 0x1001
#define zMOUSE_CAPTURE_RIGHT          (UINT) 0x1002
#define zMOUSE_DRAG                   (UINT) 0x1100
#define zMOUSE_DRAG_LEFT              (UINT) 0x1101
#define zMOUSE_DRAG_RIGHT             (UINT) 0x1102
#define zMOUSE_ASYNC_DISABLE          (UINT) 0x2000

////////////////////////////////////////////////////////////////////////////
//
//           ZDRAPP EXPORTED FUNCTION PROTOTYPES
//
////////////////////////////////////////////////////////////////////////////
extern "C"
{
// Process the requested action
zOPER_EXPORT zLONG OPERATION
ProcessActionLoop( ZSubtask *pZSubtask,
                   zCPCHAR  cpcEntityName,
                   zULONG   ulMsgPos,
                   zVIEW    vResumeSubtask,
                   zLONG    lOperCtrl,
                   zBOOL    bEntity,
                   zCPCHAR  cpcEvent );
zOPER_EXPORT zLONG OPERATION
ProcessAction( ZSubtask *pZSubtask,
               zCPCHAR  cpcActionName,
               zVIEW    vResumeSubtask,
               zLONG    lOperCtrl,
               zCPCHAR  cpcEvent );
zOPER_EXPORT zSHORT OPERATION
GetWindowAndCtrl( ZSubtask **pWndReturn,
                  ZMapAct  **pzmaReturn,
                  zVIEW    vSubtask,
                  zCPCHAR  cpcCtrlTag );
zOPER_EXPORT zSHORT OPERATION
GetWindowAndOption( ZSubtask **pWndReturn,
                    ZOption  **pzOptReturn,
                    zVIEW    vSubtask,
                    zCPCHAR  cpcOptTag );
zOPER_EXPORT zSHORT OPERATION
GetWindowAndPopupOption( ZSubtask **pWndReturn,
                         ZOption  **pzOptReturn,
                         zVIEW    vSubtask,
                         zCPCHAR  cpcOptTag );
zOPER_EXPORT zLONG OPERATION
ProcessImmediateEvent( const  ZMapAct *pzma,
                       zLONG  lEvent,
                       zPVOID pEventParms = 0 );
zOPER_EXPORT zBOOL OPERATION
ProcessEvent( const ZMapAct *pzma,
              zLONG lEvent );
zOPER_EXPORT void OPERATION
CreateZeidonCtrls( zVIEW    vSubobject,
                   ZSubtask *pZParent,
                   CWnd     *pWndParent,
                   ZMapAct  *pzmaComposite,
                   zSHORT   nOffsetX,
                   zSHORT   nOffsetY,
                   zBOOL    bSubcontrol );
zOPER_EXPORT zSHORT OPERATION
EnableZWindowsForApplication( ZSubtask *pZSubtask,
                              zBOOL    bEnable );

zOPER_EXPORT zBOOL OPERATION
fnIsAccelerator( ZSubtask *pZSubtask,
                 MSG *pMsg );

zOPER_EXPORT COLORREF OPERATION
fnTranslateColor( zLONG rgb );

zOPER_EXPORT UINT OPERATION
GetTaskbarEdge( CRect rect );
zOPER_EXPORT zBOOL OPERATION
ReadWindowPlacement( zVIEW    vSubtask,
                     zCPCHAR  cpcID,
                     zCPCHAR  cpcDlgTag,
                     zCPCHAR  cpcWndTag,
                     LPWINDOWPLACEMENT pwp );
zOPER_EXPORT void OPERATION
WriteWindowPlacement( zVIEW    vSubtask,
                      zCPCHAR  cpcID,
                      zCPCHAR  cpcDlgTag,
                      zCPCHAR  cpcWndTag,
                      LPWINDOWPLACEMENT pwp );
zOPER_EXPORT void OPERATION
fnGetProfileStr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcStringKey,
                 zPCHAR   pchReturnBuffer,
                 zLONG    lBufferLth );
zOPER_EXPORT void OPERATION
fnSetProfileStr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcStringKey,
                 zCPCHAR  cpcString );
zOPER_EXPORT zLONG OPERATION
fnGetProfileNbr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcIntegerKey );
zOPER_EXPORT void OPERATION
fnSetProfileNbr( ZSubtask *pZSubtask,
                 zCPCHAR  cpcIniID,
                 zCPCHAR  cpcIntegerKey,
                 zLONG    lInteger );

zOPER_EXPORT zBOOL OPERATION
CheckWindowPos( WINDOWPLACEMENT *wp );

zOPER_EXPORT zBOOL OPERATION
ReadToolBarPlacement( ZSubtask *pZSubtask,
                      zCPCHAR  cpcIniID,
                      zCPCHAR  cpcBarID,
                      CControlBarInfo *pCB_Info );
zOPER_EXPORT void OPERATION
WriteToolBarPlacement( ZSubtask *pZSubtask,
                       zCPCHAR  cpcIniID,
                       zCPCHAR  cpcBarID,
                       CControlBarInfo *pCB_Info );

zOPER_EXPORT zSHORT OPERATION
HeapSortArray( CArray<zPVOID, zPVOID> *pva,
               zFARPROC_COMPARE lpfnCompare,
               zLONG   lRecordCnt,
               zPVOID  pvSortData );
zOPER_EXPORT void OPERATION
TraceRect( zCPCHAR cpcMessage,
           RECT&   rect );
zOPER_EXPORT void OPERATION
TracePoint( zCPCHAR cpcMessage,
            POINT&  pt );
zOPER_EXPORT void OPERATION
TraceSize( zCPCHAR cpcMessage,
           SIZE&   size );
zOPER_EXPORT zSHORT OPERATION
zkCancel( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zkExitCloseOK( zVIEW vSubtask );
zOPER_EXPORT zSHORT OPERATION
zkCtrlInfo( zVIEW vSubtask );
zOPER_EXPORT zLONG OPERATION
ListSubtasks( zVIEW vSubtask );
zOPER_EXPORT zLONG OPERATION
GetRegisteredMessage( zCPCHAR cpcMsg );
zOPER_EXPORT zSHORT OPERATION
SaveActiveCtrlToCSV( zVIEW vSubtask );

zEXPORT zPVOID OPERATION
FindDIBBits( zPVOID pbi );
zEXPORT zULONG OPERATION
DIBWidth( zPVOID pDib );
zEXPORT zULONG OPERATION
DIBHeight( zPVOID pDib );
zEXPORT zUSHORT OPERATION
PaletteSize( zPVOID pbi );
zEXPORT zUSHORT OPERATION
DIBNbrColors( zPVOID pbi );
zEXPORT HBITMAP OPERATION
Emboss( HBITMAP hBitmap, HBITMAP hbmBackGnd, HPALETTE hPal, BOOL bRaised = TRUE,
        int xDest = 0, int yDest = 0,
        COLORREF clrHighlight = GetSysColor( COLOR_BTNHIGHLIGHT ),
        COLORREF clrShadow = GetSysColor( COLOR_BTNSHADOW ) );
zEXPORT HBITMAP OPERATION
LoadAnImage( zPCHAR pchFileName );

} // end: extern "C"

////////////////////////////////////////////////////////////////////////////
//
//           ZDRAPP LOCAL FUNCTION PROTOTYPES
//
////////////////////////////////////////////////////////////////////////////
//
void
fnPushMsgQ_State( zVIEW vMsgQ,
                  zLONG lState );
zLONG
fnPopMsgQ_State( zVIEW vMsgQ );
void
fnClearCtrlErr( ZMapAct *pNode,
                WPARAM  wParam,
                LPARAM  lParam );
zBOOL
fnValidTab( ZSubtask *pZSubtask,
            zBOOL bBackward );

HICON
GetIconForWindow( ZSubtask *pZSubtask );
zSHORT
fnSetSuboptionStates( ZSubtask *pZSubtask,
                      ZOption  *pzOpt,
                      zLONG    lOptionType,
                      zULONG   ulValue,
                      zBOOL    bPopup );
zSHORT
fnCompareOptionNodes( const ZOption *pNode1,
                      const ZOption *pNode2 );
zSHORT
fnSeekOptionNode( const CString *pzsTag,
                  const ZOption *pNode );
void
fnEnableDisableOptions( ZOption *pNode,
                        WPARAM wParam,
                        LPARAM lParam );
void
fnDeleteOptionObject( ZOption *pNode,
                      WPARAM wParam,
                      LPARAM lParam );
void
fnOrderedOptions( const ZOption *pNode,
                  WPARAM wParam,
                  LPARAM lParam );
zSHORT
fnCompareMapNodes( const ZMapping *pNode1,
                   const ZMapping *pNode2 );
zSHORT
fnSeekMapNode( const CString *m_pzsTag,
               const ZMapping *pNode );
void
fnOrderedMapping( const ZMapping *pNode,
                  WPARAM wParam,
                  LPARAM lParam );
void
fnDeleteMapObject( ZMapping *pNode,
                   WPARAM wParam,
                   LPARAM lParam );
void
fnDeleteCtrlObject( ZMapAct *pNode,
                    WPARAM  wParam,
                    LPARAM  lParam );
zSHORT
fnCompareCtrlNodes( const ZMapAct *pNode1,
                    const ZMapAct *pNode2 );
zSHORT
fnSeekCtrlNode( const CString *pzsTag,
                const ZMapAct *pNode );
void
fnEnableDisableCtrls( ZMapAct *pNode,
                      WPARAM  wParam,
                      LPARAM  lParam );
void
fnDisableCtrls( ZMapAct *pNode,
                WPARAM  wParam,
                LPARAM  lParam );
void
fnMapCtrlsToOI( ZMapAct *pNode,
                WPARAM  wParam,
                LPARAM  lParam );
void
fnMapCtrlsFromOI( ZMapAct *pNode,
                  WPARAM  wParam,
                  LPARAM  lParam );
void
fnPostMapCtrlsFromOI( ZMapAct *pNode,
                      WPARAM  wParam,
                      LPARAM  lParam );
void
fnPostCreateCtrls( ZMapAct *pNode,
                   WPARAM  wParam,
                   LPARAM  lParam );
void
fnSendCtrlsParentResize( ZMapAct *pNode,
                         WPARAM  wParam,
                         LPARAM  lParam );
void
fnSendCtrlsParentMove( ZMapAct *pNode,
                       WPARAM  wParam,
                       LPARAM  lParam );
void
fnSendCtrlsPaletteChanged( ZMapAct *pNode,
                           WPARAM  wParam,
                           LPARAM  lParam );
void
fnSendCtrlsParentActivated( ZMapAct *pNode,
                            WPARAM  wParam,
                            LPARAM  lParam );
void
fnOrderedCtrls( const ZMapAct *pNode,
                WPARAM wParam,
                LPARAM lParam );
void
fnSetFirstLastFocus( ZMapAct *pNode,
                     WPARAM  wParam,
                     LPARAM  lParam );
void
fnDeleteCtrlObject( ZMapAct *pNode,
                    WPARAM  wParam,
                    LPARAM  lParam );
zLONG
fnProcessDialogOperationCode( ZSubtask *pZSubtask,
                              zCPCHAR  cpcDLL_Name,
                              zCPCHAR  cpcCodeName );
void
fnProcessReturn( ZSubtask *pZSubtask,
                 zKZWDLGXO_Act_DEF ActDef );
void
fnStartDebugger( ZSubtask *pZSubtask,
                 zCPCHAR  pchActionName );
void
fnResumeFromPainter( ZSubtask *pZSubtask );

ZSubtask *
fnGetSubtask( ZSubtask *pZSubtask,
              zVIEW    *vSubtaskReturn,
              zCPCHAR  cpcDlgTag,
              zCPCHAR  cpcWndTag,
              zBOOL    bMatch );
zBOOL
FindGlobalDialogWindow( ZFindDialog *pfdReturn );

void
EnableParentWindows( ZSubtask *pZSubtask,
                     zBOOL    bEnable );

void
InvalidateChildRect( ZMapAct *pzma,
                     zBOOL   bErase );
ZMapAct *
GetSafeCtrlNode( ZMultiIdxSet *pZBinCtrl,
                 zUSHORT      uID );

zBOOL
RegisterZeidonDialog( HINSTANCE hInstance );
zSHORT
ConstructZeidonDialogBox( ZSubtask *pZParent,
                          zPCHAR   lpReturnCtrlTag,
                          zCPCHAR  cpcDlgTag,
                          zCPCHAR  cpcWndTag );
zBOOL
GetPrinterDevice( zPCHAR  pchPrinterName,
                  HGLOBAL *phDevNames,
                  HGLOBAL *phDevMode );

zOPER_EXPORT zSHORT OPERATION
SetZCtrlText( ZMapAct *pzma,
              zCPCHAR cpcText );
zOPER_EXPORT zSHORT OPERATION
GetZCtrlText( ZMapAct *pzma,
              zPCHAR  pcText,
              zUSHORT uTextLth );

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteCtrlParent( ZMapAct *pzma,
                            zVIEW   vXRA,
                            zBOOL   bCreateEntity );
zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteCtrl( ZMapAct *pzma,
                      zVIEW   vXRA,
                      zBOOL   bCreateEntity );
zOPER_EXPORT zSHORT OPERATION
SetRemoteZCtrlAttribute( ZMapAct *pzma,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zCPCHAR cpcValue );
zOPER_EXPORT zSHORT OPERATION
SetUpdateFlagForAttribute( zVIEW   vView,
                           zCPCHAR cpcEntity,
                           zCPCHAR cpcAttribute,
                           zBOOL   bValue );
zOPER_EXPORT zSHORT OPERATION
GetRemoteZCtrlAttribute( ZMapAct *pzma,
                         zCPCHAR cpcEntity,
                         zCPCHAR cpcAttribute,
                         zPCHAR  pchValue,
                         zUSHORT uMaxLth );

zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteOptParent( ZOption *pOpt,
                           zVIEW   vXRA,
                           zBOOL   bCreateEntity );
zOPER_EXPORT zBOOL OPERATION
PositionOnRemoteOpt( ZOption *pOpt,
                     zVIEW   vXRA,
                     zBOOL   bCreateEntity );
zOPER_EXPORT zSHORT OPERATION
SetRemoteZOptAttribute( ZOption *pOpt,
                        zCPCHAR cpcEntity,
                        zCPCHAR cpcAttribute,
                        zCPCHAR cpcValue );
zOPER_EXPORT zSHORT OPERATION
GetRemoteZOptAttribute( ZOption *pOpt,
                        zCPCHAR cpcEntity,
                        zCPCHAR cpcAttribute,
                        zPCHAR  pchValue,
                        zUSHORT uMaxLth );
zOPER_EXPORT zSHORT
fnSendReceiveXML( ZSocketTask *pZSocketTask,
                  zPCHAR pchBuffer );
zOPER_EXPORT void OPERATION
MakeShortString( CDC *pDC, zPCHAR pchReturn, zLONG lMaxReturnLth,
                 zCPCHAR cpcString, zLONG lColLth, zLONG lOffset );

void
ListWindowsAndControls( ZSubtask *pZSubtask,
                        zPCHAR   lpLabel,
                        zLONG    lItem );

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Derived from code written by Paul DiLascia.
//
#ifdef _DEBUG

/////////////////////////////////////////////////////////////////////////////
// Implements TRACEFN macro.
// Don't ever use directly, just use TRACEFN
//

// Goodies to get names of things.
//
extern CString DbgWndName( CWnd *pWnd ); // get name of window
extern CString DbgWndName( UINT uMsg );  // get name of WM_ message

#ifdef REFIID

struct DBGINTERFACENAME
{
   const IID *piid;  // ptr to GUID
   LPCSTR name;      // human-readable name of interface
};

// Change this to whatever interfaces you want to track
// Default is none
//
extern DBGINTERFACENAME *_pDbgInterfaceNames;

extern CString sDbgName( REFIID iid ); // get name of COM interface

#endif // REFIID

#else // Not _DEBUG

#define DbgWndName( x )   CString( )
#define TRACEFN TRACE

#endif

//
// Macro casts to LPCTSTR for use with TRACE/printf/CString::Format
//
#define DbgName( x ) (LPCTSTR) DbgWndName( x )

/////////////////////////////////////////////////////////////////////////////
//
// global functions for ordinary CBitmap too
//
CSize
GetBitmapSize( CBitmap *pBitmap );
zBOOL
DrawBitmap( CDC& dc, CBitmap *pBitmap,
            const CRect *rectTgt = 0, const CRect *rectSrc = 0 );

#define IS_WIN30_DIB( pbi ) ((*(zPULONG)(pbi)) == sizeof( BITMAPINFOHEADER ))
#define RECTWIDTH( pRect )  ((pRect)->right - (pRect)->left)
#define RECTHEIGHT( pRect ) ((pRect)->bottom - (pRect)->top)
#define PALVERSION  0x300

#define HDIB HANDLE

/////////////////////////////////////////////////////////////////////////////
// ZDib implements Device Independent Bitmaps as a form of CBitmap.
//
class ZDRAPP_CLASS ZDib : public CBitmap
{
   DECLARE_DYNAMIC( ZDib )

public:
   ZDib( );
   ~ZDib( );

   CSize GetSize( ) { return( CSize( m_bitmap.bmWidth, m_bitmap.bmHeight ) ); }
   zBOOL Attach( HGDIOBJ hbm );
   zBOOL Load( zCPCHAR cpcPathName );
   zBOOL Load( HINSTANCE hInst, zCPCHAR cpcResourceName );
   zBOOL Load( HINSTANCE hInst, UINT uResId );

   // Universal Draw function can use DrawDib or not.
   zBOOL Draw( CDC& dc, const CRect *rectTgt = 0, const CRect *rectSrc = 0,
               zBOOL bUseDrawDib = TRUE, CPalette *pPalette = 0,
               zBOOL bForeground = FALSE );

   zBOOL DeleteObject( );
   zBOOL CreatePalette( CPalette& pal );
   CPalette * GetPalette( );
   UINT  GetColorTable( RGBQUAD *colorTab, UINT uColors );

   zBOOL    m_bBitmapLoaded;

protected:
   BITMAP   m_bitmap;    // stored for speed
   CPalette m_palette;   // palette
   HDRAWDIB m_hdd;       // for DrawDib
};

class ZDRAPP_CLASS ZDib2
{
public:
   enum eBitmapType
   {
      BMP,
      GIF,
      TIFF
   };

   ZDib2( HANDLE hDib = 0, int uBits = 8 );
   virtual ~ZDib2( );
   ZDib2& operator=( ZDib2& dib );
   zBOOL IsValid( ) { return( m_pVoid && Width( ) && Height( ) ); };
   void UseGamma( float fg, zBOOL bUse = TRUE );
   zBOOL CreateFromHandle( HANDLE hDib, int uBits );
   zBOOL Create( int lWidth, int lHeight, int uBits = 24 );
   zBOOL Create( BITMAPINFOHEADER& bmInfo );
   zBOOL CopyDIB( ZDib2& dib );
   zBOOL OpenDIB( zCPCHAR cpcFileName );
   zBOOL SaveDIB( zCPCHAR cpcFileName, eBitmapType bmType );
   void ReplaceColor( unsigned char oldColor, unsigned char newColor );
   HANDLE GetDIBits( int lStartX = -1, int lStartY = -1,
                     int nCx = -1, int nCy = -1 );
   CBitmap *GetBitmap( CDC& dc );
   CBitmap *GetTempBitmap( CDC& dc );
   int CountColors( );
   int EnumColors( BYTE *colors );
   void InitDIB( COLORREF color );
   void CopyLine( int source, int dest );
   void DestroyDIB( );
   void SetPalette( unsigned char *palette );
   void SetPalette( RGBQUAD *pRGB );
   COLORREF PaletteColor( int index );
   void SetPixel( int x, int y, COLORREF color );
   void SetPixel8( int x, int y, unsigned char color );
   COLORREF GetPixel( int x, int y );
   void GetPixel( UINT x, UINT y, int& pixel );
   void BitBlt( HDC hDest, int nXTgt, int nYTgt,
                int lWidth, int lHeight,
                int nSrcX, int nSrcY );
   void BitBlt( int nXTgt, int nYTgt, int lWidth, int lHeight,
                ZDib2& dibSrc, int nSrcX, int nSrcY, BYTE *colors = 0 );
   void StretchBlt( HDC hDest, int nXTgt, int nYTgt,
                    int nDWidth, int nDHeight,
                    int nSrcX, int nSrcY, int nSWidth, int nSHeight );
   void StretchBlt( int nXTgt, int nYTgt, int nDWidth, int nDHeight,
                    ZDib2& dibSrc, int nSrcX, int nSrcY,
                    int nSWidth, int nSHeight );
   void ExpandBlt( int nXTgt, int nYTgt, int xRatio, int yRatio,
                   ZDib2& dibSrc, int nSrcX, int nSrcY,
                   int nSWidth, int nSHeight );
   void SetFlags( int flag ) { m_nFlags = flag; };
   int Height( ) { return( m_lHeight ); };
   int Width( ) { return( m_lWidth ); };
   unsigned char *GetLinePtr( int lLine );
   inline int GetBitCount( ) { return( m_pInfo->bmiHeader.biBitCount ); };
   zBOOL Make8Bit( ZDib2& dib );
   zBOOL SwitchFromOne( ZDib2& dib );
   zBOOL SwitchFromFour( ZDib2& dib );
   zBOOL SwitchFrom24( ZDib2& dib );
   zBOOL SwitchPalette( ZDib2& dib );
   int ClosestColor( RGBQUAD *pRgb );
   LPBITMAPINFO GetBitmapInfo( ) { return( m_pInfo );};
   static zULONG Distance( RGBQUAD& rgb1, RGBQUAD& rgb2 );
   zBOOL SaveBMP( zCPCHAR cpcFileName );
   zULONG GetDIBSize( );
   int GetPaletteSize( BITMAPINFOHEADER& rbmInfo );
   int GetPaletteSize( );

protected :
   HANDLE DIBHandle( );
   zBOOL OpenBMP( zCPCHAR cpcFileName );
   zBOOL OpenGIF( zCPCHAR cpcFileName );
   zBOOL OpenTIFF( zCPCHAR cpcFileName );
   zBOOL SaveGIF( zCPCHAR cpcFileName );
   zBOOL SaveTIFF( zCPCHAR cpcFileName );
   void CreateGammaCurve( );
   void Expand( int nXTgt, int nYTgt, int xRatio, int yRatio,
                ZDib2& dibSrc, int nSrcX, int nSrcY,
                int nSWidth, int nSHeight );
   unsigned char *m_pBits;
   PBITMAPINFO m_pInfo;
   RGBQUAD *m_pRGB;
   void  *m_pVoid;
   BYTE  **m_pLinePtr;
   int m_lHeight, m_nBytes, m_lWidth, m_nBits;
   int m_nFlags;
   zBOOL m_bUseGamma;
   float m_fGamma;
   float m_fOldGamma;
   unsigned char m_ucGamma[ 256 ];
   RGBQUAD m_rgbQuadCache[ 256 ];
   zCHAR  m_CachePtr[ 256 ];
};

/////////////////////////////////////////////////////////////////////////////
// ZCoolBar encapsulates IE 4.0 common coolbar for MFC.
//
class ZDRAPP_CLASS ZCoolBar : public CControlBar
{
protected:
   DECLARE_DYNAMIC( ZCoolBar )

public:
   ZCoolBar( ZSubtask *pZSubtask );
   virtual ~ZCoolBar( );

   BOOL Create( CWnd *pParentWnd, DWORD dwStyle,
                DWORD dwAfxBarStyle = CBRS_ALIGN_TOP,
                UINT nID = AFX_IDW_TOOLBAR );

   // Message wrappers
   BOOL GetBarInfo( LPREBARINFO lp );
   BOOL SetBarInfo( LPREBARINFO lp );
   BOOL GetBandInfo( int iBand, LPREBARBANDINFO lp );
   BOOL SetBandInfo( int iBand, LPREBARBANDINFO lp );
   BOOL InsertBand( int iWhere, LPREBARBANDINFO lp );
   BOOL DeleteBand( int nWhich );
   int  GetBandCount( );
   int  GetRowCount( );
   int  GetRowHeight( int nWhich );

protected:
   // new virtual functions you must/can override
   virtual BOOL OnCreateBands( ) = 0; // return -1 if failed
   virtual void OnHeightChange( const CRect& rcNew );

   // CControlBar Overrides
   virtual CSize CalcFixedLayout( BOOL bStretch, BOOL bHorz );
   virtual CSize CalcDynamicLayout( int nLength, DWORD nMode );
   virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );

   // message handlers
   DECLARE_MESSAGE_MAP( )
   afx_msg int  OnCreate( LPCREATESTRUCT lpcs );
   afx_msg void OnPaint( );
   afx_msg void OnHeightChange( NMHDR *pNMHDR, LRESULT *pRes );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
};

/////////////////////////////////////////////////////////////////////////////
// Specialized CToolBar fixes display problems in MFC.
//
class ZDRAPP_CLASS ZCoolToolBar : public CToolBar
{
   DECLARE_DYNAMIC( ZCoolToolBar )
public:
   ZCoolToolBar( );
   virtual ~ZCoolToolBar( );
protected:
   virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );

   afx_msg BOOL OnNcCreate( LPCREATESTRUCT lpcs );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );
   afx_msg void OnNcCalcSize( BOOL, NCCALCSIZE_PARAMS * );

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// Programmer-friendly REBARINFO initializes itself
//
class ZDRAPP_CLASS ZRebarInfo : public REBARINFO
{
public:
   ZRebarInfo( );
};

/////////////////////////////////////////////////////////////////////////////
// Programmer-friendly REBARBANDINFO initializes itself
//
class ZDRAPP_CLASS ZRebarBandInfo : public REBARBANDINFO
{
public:
   ZRebarBandInfo( );
};

/////////////////////////////////////////////////////////////////////////////
// "Flat" style tool bar.  Use instead of CToolBar to create a tool bar
// with the flat look.
//
// ZFlatToolBar has overridden load functions that modify the style to
// TBSTYLE_FLAT.  If the toolbar is not loaded from a resource, call
// ModifyStyle( 0, TBSTYLE_FLAT ).
//
class ZDRAPP_CLASS ZFlatOrCoolBarCmdUI : public CCmdUI
{
public: // re-implementations only
   virtual void Enable( BOOL bOn );
   virtual void SetCheck( int nCheck );
   virtual void SetText( LPCTSTR lpszText );
};

class ZDRAPP_CLASS ZFlatToolBar : public CToolBar
{
public:
   BOOL LoadToolBar( zCPCHAR lpszResourceName );
   BOOL LoadToolBar( UINT nIDResource );

protected:
   DECLARE_DYNAMIC( ZFlatToolBar )
   virtual void OnUpdateCmdUI( CFrameWnd *pTarget, BOOL bDisableIfNoHndler );
   DECLARE_MESSAGE_MAP( )
   afx_msg void OnWindowPosChanging( LPWINDOWPOS lpWndPos );
   afx_msg void OnWindowPosChanged( LPWINDOWPOS lpWndPos );
};

/////////////////////////////////////////////////////////////////////////////
//
class ZDRAPP_CLASS ZFontUI
{
protected:
   int      m_nFontPtSizeMin; // min..
   int      m_nFontPtSizeMax; // ..and max font size in points

   // Override to change grow/shrink algorithm
   virtual int GrowFontSize( int ptSize, int dir );

public:
   ZFontUI( );
   virtual ~ZFontUI( );

   // universal function for bigger/smaller/dialog
   zBOOL OnChangeFont( CFont& font, int op,
                       CWnd *pWnd = 0, DWORD dwFlags = 0 );

   void SetMinMaxFontSize( int nMin, int nMax )
      { m_nFontPtSizeMin = nMin; m_nFontPtSizeMax = nMax; }

   // Handy conversion functions
   int   GetFontPointSize( CFont& font, CDC& dc );
   zBOOL SetFontPointSize( CFont& font, CDC& dc, int pts );

   // Functions to read/write font in profile ( registry settings or .ini )
   zBOOL WriteProfileFont( zCPCHAR cpcKey, zCPCHAR cpcVal,
                           CFont& font, CDC *pDC = 0 );
   zBOOL GetProfileFont( zCPCHAR cpcKey, zCPCHAR cpcVal,
                         CFont& font, CDC *pDC = 0 );
};

/////////////////////////////////////////////////////////////////////////////
//
// Generic class to hook messages on behalf of a CWnd.
// Once hooked, all messages go to ZMsgHook::WindowProc before going
// to the window. Specific subclasses can trap messages and do something.
// To use:
//
// Derive a class from ZMsgHook.
//
// Override ZMsgHook::WindowProc to handle messages. Make sure you call
// ZMsgHook::WindowProc if you don't handle the message, or your window will
// never get messages. If you write seperate message handlers, you can call
// Default( ) to pass the message to the window.
//
// Instantiate your derived class somewhere and call HookWindow( pWnd )
// to hook your window, AFTER it has been created.
// To unhook, call HookWindow( 0 ).
//
class ZDRAPP_CLASS ZMsgHook : public CObject
{
protected:
   DECLARE_DYNAMIC( ZMsgHook );
   CWnd        *m_pWndHooked;    // the window hooked
   WNDPROC     m_pOldWndProc;    // ..and original window proc
   ZMsgHook    *m_pNext;         // next in chain of hooks for this window

   // Override this to handle messages in specific handlers
   virtual LRESULT WindowProc( UINT msg, WPARAM wp, LPARAM lp );
   LRESULT Default( );           // call this at the end of handler fns

public:
   ZMsgHook( );
   ~ZMsgHook( );

   // Hook a window. Hook( 0 ) to unhook (automatic on WM_NCDESTROY)
   zBOOL  HookWindow( CWnd *pRealWnd );
   zBOOL  IsHooked( ) { return( m_pWndHooked != 0 ); }

   friend LRESULT CALLBACK HookWndProc( HWND, UINT, WPARAM, LPARAM );
   friend class ZMsgHookMap;
};

/////////////////////////////////////////////////////////////////////////////
//
// Generic palette message handler makes handling palette messages easy.
// To use:
//
// Instaniate a ZPalMsgHandler in your main frame and
// every CWnd class that needs to realize palettes (e.g., your view).
// Call Install to install.
// Call DoRealizePalette( TRUE ) from your view's OnInitialUpdate fn.
//
class ZDRAPP_CLASS ZPalMsgHandler : public ZMsgHook
{
protected:
   CPalette *m_pPalette; // ptr to palette

   DECLARE_DYNAMIC( ZPalMsgHandler );

   // These are similar to, but NOT the same as the equivalent CWnd fns.
   // Rarely, if ever need to override.
   //
   virtual LRESULT WindowProc( UINT msg, WPARAM wp, LPARAM lp );
   virtual void OnPaletteChanged( CWnd *pFocusWnd );
   virtual zBOOL OnQueryNewPalette( );
   virtual void OnSetFocus( CWnd *pOldWnd );

   // Override this if you realize your palette some other way
   // (not by having a ptr to a CPalette).
   //
   virtual int  DoRealizePalette( zBOOL bForeground );

public:
   ZPalMsgHandler( );
   ~ZPalMsgHandler( );

   // Get/Set palette obj
   CPalette *GetPalette( ) { return( m_pPalette ); }
   void SetPalette( CPalette *pPal ) { m_pPalette = pPal; }

   // Call this to install the palette handler
   zBOOL Install( CWnd *pWnd, CPalette *pPal )
   {
      m_pPalette = pPal;
      return( HookWindow( pWnd ) );
   }
};

//
// Any modifications should be done in this class
//

class ZDRAPP_CLASS ZCommandLineInfo : public CCommandLineInfo
{
public:
   ZCommandLineInfo( );
   virtual ~ZCommandLineInfo( );

   //Sample 03: Get functions for params and switches
   void GetParams(CStringArray& params);
   void GetSwitches(CStringArray& switches);

   CCommandLineInfo &operator=(ZCommandLineInfo &ZCmdLI);
   CCommandLineInfo *operator=(ZCommandLineInfo *ZCmdLI);

private:
   //Sample 01: Private Members
   CStringArray m_params;
   CStringArray m_switches;

   //Sample 02: Override for Base class virtual
   void ParseParam(const TCHAR *pszParam, BOOL bFlag, BOOL bLast);
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Purpose: Classes to browse the Registry for installed ActiveX Components
//
// Classes: ZActiveX_CtrlList   - List of installed Components
//          ZActiveX_Ctrl       - Single component
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

class ZDRAPP_CLASS ZActiveX_Ctrl : public CObject
{
   DECLARE_DYNAMIC( ZActiveX_Ctrl )

public:
   ZActiveX_Ctrl( );
   ZActiveX_Ctrl( zCPCHAR cpcCLSID,
                  zCPCHAR cpcProgId,
                  zCPCHAR cpcDescription,
                  zCPCHAR cpcToolbarBitmap,
                  zCPCHAR cpcTypeLib,
                  zLONG   lKey );

   virtual ~ZActiveX_Ctrl( );

   zLONG   GetKey( );
   zCPCHAR GetToolbarBitmapName( );
   zCPCHAR GetProgId ( );
   zCPCHAR GetClassId( );
   zCPCHAR GetDescription( );
   zCPCHAR GetTypeLib( );
   CBitmap * GetControlBitmap( );

   zBOOL SetKey( zLONG lKey );
   zBOOL SetClassId( zCPCHAR cpcCLSID );
   zBOOL SetProgId( zCPCHAR cpcProgId );
   zBOOL SetDescription( zCPCHAR cpcDescription );
   zBOOL SetToolbarBitmap( zCPCHAR cpcToolbarBitmap );
   zBOOL SetTypeLib( zCPCHAR cpcTypeLib );

#ifdef _DEBUG
   virtual void Dump( CDumpContext& dc )
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
      dc << *m_pcsProgId;
   }
#endif

private:
   zLONG   m_lKey;
   CString *m_pcsToolbarBitmap;
   CString *m_pcsDescription;
   CString *m_pcsProgId;
   CString *m_pcsCLSID;
   CString *m_pcsTypeLib;
   CBitmap *m_pBitmap;
};

class ZDRAPP_CLASS ZActiveX_CtrlList : public CObList
{
   DECLARE_DYNAMIC( ZActiveX_CtrlList )

public:
   ZActiveX_CtrlList( zVIEW v ) { m_v = v; m_hKeyCLSIDS = 0; m_hKeyTypeLib = 0; }
   ZActiveX_Ctrl * FindRegControl( CString csCLSID );
   ZActiveX_CtrlList( zCPCHAR cpcMachineName = 0 );
   ~ZActiveX_CtrlList( );

   // Two separate population mechanisms.
   zSHORT PopulateZeidonRegisteredActiveX( );
   zSHORT PopulateZeidonUnregisteredActiveX( );
   zBOOL  CreateItemFromClassId( CString csClassId, zLONG lKey = 0 );

#ifdef _DEBUG
   virtual void Dump( CDumpContext& dc )const
   {
      CRuntimeClass *pRc = GetRuntimeClass( );
      CObject::Dump( dc );
      dc << pRc->m_lpszClassName << "\n";
      dc << "Number of elements " << GetCount( );
   }
#endif

private: // Members
   zVIEW   m_v;
   HKEY    m_hKeyCLSIDS;
   HKEY    m_hKeyTypeLib;

private: //Implementation
   zSHORT DeleteAllEntries( );
   zBOOL  QueryKeyValue( HKEY    hKeyParent,
                         CString csName,
                         CString *pcsBuffer );
   zBOOL  KeyIsControl( HKEY hKeyParent,
                        CString csName );
   zBOOL  FindItemByName( HKEY    hParent,
                          CString csItemName,
                          CString *pcsReturn );
   zBOOL  QueryTypeLibFile( CString csTypeLibClsid,
                            CString *pcsReturn );
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// This is a slightly optimized version of the popular CMemDC class.  For
// the fastest redraw put a bitmap in your Dialog, Control or View header:
//   CBitmap Bitmap;
//
// In the .cpp file:
// #include "MemDC.h"
// Start your OnPaint() with the following two lines:
//
//   CPaintDC dcPaint( this );
//   CMemDC dc(&dcPaint,&Bitmap);
//
// Then draw everything to dc - and thats it!
//
// You don't need the Bitmap - just create the CMemDC with:
//
//   CMemDC dc( &dcPaint );
//
// but it will be a little slower.
//
// Remember the following tips:
//
// if you're filling the background yourself:
// Remember to do it either in OnEraseBkgnd or to not do it at all use:
//   BOOL CMyDlgOrControl::OnEraseBkgnd( CDC *pDC ) { return( TRUE ); }
//
//  If you're filling it black or white use:
//
//   PatBlt( m_rect.left, m_rect.top,
//           m_rect.Width(), m_rect.Height(), BLACKNESS );
//  or:
//    PatBlt( m_rect.left, m_rect.top,
//            m_rect.Width(), m_rect.Height(), WHITENESS );
//
//  to fill with the current dialog background color use:
//
//   dc.FillSolidRect( &m_rect, GetSysColor( COLOR_3DFACE ) ); // Draw in dialogs
//                                                             // background color
//
// To draw using the system dialog font use:
//
//   CGdiObject *OldFont = dc.SelectStockObject( ANSI_VAR_FONT );
//
// then use dc.DrawText(...);
//
// and finish with
//
//   dc.SelectObject( &OldFont );
//
// It is best to use a class to encapsulate the clean-up:
// class CUseDialogFont
// {
//   CGdiObject *OldFont;
//   CDC *pDC;
// public:
//   CUseDialogFont( CDC *pDC )
//   {
//      m_pDC = pDC;
//      OldFont = m_pDC->SelectStockObject( ANSI_VAR_FONT );
//   }
//
//   ~CUseDialogFont( )
//   {
//      pDC->SelectObject( &OldFont );
//   }
// };
//
// Then in your code just use:
//   CUseDialogFont Font( &dc );
//
// That way you can 'return' at any time safe in the knowledge that the
// original font will be selected.
//
/////////////////////////////////////////////////////////////////////////////
class ZDRAPP_CLASS ZMemDC : public CDC
{
public:
   ZMemDC::ZMemDC( CDC *pDC, CBitmap *pBmp = 0 )
   {
      ASSERT_VALID( pDC );
      m_pDC = pDC;
      m_bitmapOld = 0;
      m_bMemDC = !pDC->IsPrinting( );

#ifdef MemDCForView
      if ( m_bMemDC )
      {
         m_pDC->GetClipBox( &m_rect ); // for Views
         m_pDC->LPtoDP( &m_rect );
#else
         m_rect=((CPaintDC *)m_pDC)->m_ps.rcPaint; // for OnPaint
#endif
         CreateCompatibleDC( m_pDC );
         if ( pBmp )
            m_bitmapOld=SelectObject( pBmp ); // user passed bitmap, so use it
         else
         {
            //Create our own bitmap
            m_bitmap.CreateCompatibleBitmap( pDC, m_rect.Width( ),
                                             m_rect.Height( ) );
            m_bitmapOld=SelectObject( &m_bitmap );
         }

#ifdef MemDCForView
         SetMapMode( m_pDC->GetMapMode( ) );
         m_pDC->DPtoLP( &m_rect );
         SetWindowOrg( m_rcClip.left, m_rcClip.top );
      }
      else
      {
         // Make a copy of the relevent parts of the current DC for printing.
         m_bPrinting = pDC->m_bPrinting;
         m_hDC = m_pDC->m_hDC;
         m_hAttribDC = pDC->m_hAttribDC;
      }
#endif
   }

   ~ZMemDC( ) // dtor copies the contents of the mem DC to the original DC
   {
#ifdef MemDCForView
      if ( m_bMemDC )
      {
#endif
         m_pDC->BitBlt( m_rect.left, m_rect.top,
                        m_rect.Width(), m_rect.Height(),
                        this, m_rect.left, m_rect.top, SRCCOPY);
         SelectObject( m_bitmapOld );
#ifdef MemDCForView
      }
      else
         m_hDC = m_hAttribDC = 0;
#endif
   }

   ZMemDC * operator->() { return( this ); } // Allow usage as a pointer
   operator ZMemDC *()   { return( this ); } // Allow usage as a pointer

private:
   CBitmap  m_bitmap;     // Offscreen bitmap
   CBitmap  *m_bitmapOld; // bitmap originally found in CMemDC
   CDC      *m_pDC;       // Saves CDC passed in constructor
   CRect    m_rect;       // Rectangle of drawing area.
   zBOOL    m_bMemDC;     // TRUE if CDC really is a Memory DC.
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
extern "C"
{
#include ".\jpeglib\jpeglib.h"

zOPER_EXPORT void OPERATION
GetVEAPX_Reference( zVIEW vSubtask, zVIEW vAppOI, zCPCHAR cpcVEAPX,
                    zPCHAR pchFileName, zLONG lMaxLth );
zOPER_EXPORT BYTE * OPERATION
LoadBitmapFromJPG( CString csFileName, UINT *pnWidth, UINT *pnHeight );
}

#ifndef zREMOTE_SERVER  // only available under windows

class ZDRAPP_CLASS ZJpegFile
{
public:

   ////////////////////////////////////////////////////////////////
   // these do nothing
   ZJpegFile( );   // creates an empty object
   ~ZJpegFile( );  // destroys nothing


   ////////////////////////////////////////////////////////////////
   // read a JPEG file to an RGB buffer - 3 bytes per pixel
   // returns a ptr to a buffer .
   // caller is responsible for cleanup!!!
   // BYTE *buf = ZJpegFile::ZJpegFileToRGB( .... );
   // delete [] buf;

   static BYTE * JpegFileToRGB( CString fileName,   // path to image
                                UINT *width,        // image width in pixels
                                UINT *height );     // image height

   ////////////////////////////////////////////////////////////////
   // write a JPEG file from a 3-component, 1-byte per component buffer

   static BOOL RGBToJpegFile( CString fileName, // path
                              BYTE *dataBuf,    // RGB buffer
                              UINT width,       // pixels
                              UINT height,      // rows
                              BOOL color,       // TRUE = RGB
                                                // FALSE = Grayscale
                              int quality );    // 0 - 100


   ////////////////////////////////////////////////////////////////
   // fetch width / height of an image

   static BOOL GetJPGDimensions( CString fileName,       // path
                                 UINT *width,            // pixels
                                 UINT *height );

   ////////////////////////////////////////////////////////////////
   // utility functions
   // to do things like DWORD-align, flip, convert to grayscale, etc.
   //

   ////////////////////////////////////////////////////////////////
   // allocates a DWORD-aligned buffer, copies data buffer
   // caller is responsible for delete []'ing the buffer

   static BYTE * MakeDwordAlignedBuf( BYTE *dataBuf,           // input buf
                                      UINT widthPix,           // input pixels
                                      UINT height,             // lines
                                      UINT *uiOutWidthBytes ); // new width bytes


   ////////////////////////////////////////////////////////////////
   // if you have a DWORD aligned buffer, this will copy the
   // RGBs out of it into a new buffer. new width is widthPix * 3 bytes
   // caller is responsible for delete []'ing the buffer

   static BYTE * RGBFromDWORDAligned( BYTE *inBuf,       // input buf
                                      UINT widthPix,     // input size
                                      UINT widthBytes,   // input size
                                      UINT height );

   ////////////////////////////////////////////////////////////////
   // vertically flip a buffer - for BMPs
   // in-place

   // note, this routine works on a buffer of width widthBytes: not a
   // buffer of widthPixels.
   static BOOL VertFlipBuf( BYTE *inbuf,          // input buf
                            UINT widthBytes,       // input width bytes
                            UINT height );         // height

   // NOTE :
   // the following routines do their magic on buffers with a whole number
   // of pixels per data row! these are assumed to be non DWORD-aligned buffers.

   ////////////////////////////////////////////////////////////////
   // convert RGB to grayscale   using luminance calculation
   // in-place

   static BOOL MakeGrayScale( BYTE *buf,       // input buf
                              UINT widthPix,   // width in pixels
                              UINT height );   // height

   ////////////////////////////////////////////////////////////////
   // swap Red and Blue bytes
   // in-place

   static BOOL BGRFromRGB( BYTE *buf,         // input buf
                           UINT widthPix,     // width in pixels
                           UINT height );     // lines

   static void DrawIt( HDC hDC, CRect& rect, BYTE *pBuf,
                       UINT nWidth, UINT nHeight, zBOOL bFit );
};

/////////////////////////////////////////////////////////////////////////////

class ZDRAPP_CLASS ZRememberComboBox : public CComboBox
{
// Construction
public:
   ZRememberComboBox( zSHORT nMaxEntries = 16 );

   void    SetRegKey( CString csRegKey );
   CString GetRegKey( );

   void SetMaxEntries( zSHORT nMaxEntries );
   zSHORT  GetMaxEntries( );

   zBOOL LoadRegEntries( );
   zBOOL LoadRegEntries( CString csRegKey );
   zBOOL SaveRegEntries( );

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CRegComboBox )
   protected:
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZRememberComboBox( );

   virtual BOOL DestroyWindow( );

   // Generated message map functions
protected:
   //{{AFX_MSG(CRegComboBox)
   afx_msg void OnKillFocus( );
   //}}AFX_MSG

private:
   CString m_csRegKey;
   zSHORT  m_nMaxEntries;

   void InsertCurEditText( );

   DECLARE_MESSAGE_MAP( )
};

class ZDRAPP_CLASS ZFindReplaceDialog : public CFindReplaceDialog
{
// Construction
public:
   ZFindReplaceDialog( CWnd *pParent = 0 );   // standard constructor
   zBOOL IsReplace();

// Dialog Data
   //{{AFX_DATA(ZFindReplaceDailog)
   enum { IDD = IDD_ZREPLACEDLG };
   ZRememberComboBox   m_cmbReplaceWith;
   ZRememberComboBox   m_cmbFindWhat;
   CButton   m_btnReplaceAll;
   CButton   m_btnReplace;
   CButton   m_btnIDOK;
   CString   m_csFindWhat;
   CString   m_csReplaceWith;
   //}}AFX_DATA

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZFindReplaceDailog )
   protected:
   virtual void DoDataExchange( CDataExchange *pDX );    // DDX/DDV support
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG( ZFindReplaceDailog )
   afx_msg void OnEditupdateCombofindWhat( );
   afx_msg void OnEditupdateComboReplace( );
   afx_msg void OnSelChangeCombofindWhat( );
   afx_msg void OnSelChangeComboReplace( );
   virtual BOOL OnInitDialog();
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )
};

///////////////////////////////////////////////////////////////////////////////
// ZPinnableDlg.h : Declaration of the ZPinnableDlg class.
//
// Copyright 1999 David Hill - dhill@PrincipiaMathematica.com.
//
///////////////////////////////////////////////////////////////////////////////

// Define message to inform the parent when this dialog is closed.
#define WM_PINUPCLOSE  WM_USER + 10

class ZPinnableDlg : public CDialog
{
public:
   // Construction.
   ZPinnableDlg( UINT nDlgID,         // ID for the dialog template resource
                 CWnd *pParent = NULL );  // Parent window, if any

    // Utility functions.
   BOOL Show( BOOL bPinned = FALSE );
   BOOL Hide();
   BOOL Visible() const;
   void DeactivateAsCancel( BOOL bCancel );

   //{{AFX_DATA(ZPinnableDlg)
      // NOTE: the ClassWizard will add data members here
   //}}AFX_DATA

protected:
   // Generated message map functions
   //{{AFX_MSG(ZPinnableDlg)
   afx_msg void    OnClose( );
   virtual void    OnCancel( );
   virtual void    OnOK( );
   afx_msg LRESULT OnActivate( WPARAM wParam, LPARAM lParam );
   virtual BOOL    OnInitDialog( );
   afx_msg void    OnSysCommand( UINT nID, LPARAM lParam );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP()

   // Protected data members.
   HICON m_hPinned;              // Icon for pinned state.
   HICON m_hUnpinned;            // Icon for unpinned state.
   BOOL  m_bPinned;              // Pinned state flag.
   UINT  m_nTemplateID;          // ID for the dialog's template.
   BOOL  m_bDeactivateAsCancel;  // Treat deactivation as cancel flag.

   // Protected member functions.
   void UpdateIcon( );
};

class AFX_EXT_CLASS CWaitRedraw
{
// Construction/Destruction
public:
   CWaitRedraw( );
   ~CWaitRedraw( );

// Operations
public:
   void Restore( );

protected:
   static int m_nRedrawCount;
};

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
ControlBar( ZSubtask *pZSubtask,
            CWnd     *pWndParent,
            ZMapAct  *pzmaComposite,
            zVIEW    vDialog,
            zSHORT   nOffsetX,
            zSHORT   nOffsetY,
            zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#ifdef ZDRAPP_CLASS_PREDEFINED
   #undef AFX_DATA
   #define AFX_DATA
#endif

#endif   // __zdrapp_h sentry.
