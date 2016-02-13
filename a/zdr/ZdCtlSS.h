/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctlss.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctlss_h__  // Sentry, use file only if it's not already included
#define __zdctlss_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

#define m_pzsScope  m_pzsAName // AN was stolen for the scoping entity name

#define zSS_HIDEROWHEADER              zCONTROL_SPECIFIC_1
#define zSS_HIDEGRIDLINES              zCONTROL_SPECIFIC_2
#define zSS_HIDEROWSELECTIONCOL        zCONTROL_SPECIFIC_3
#define zSS_NEWGRID                    zCONTROLX_SPECIFIC_1
#define zSS_SORTEDHEADERS              zCONTROLX_SPECIFIC_2
#define zSS_REMEMBERSORT               zCONTROLX_SPECIFIC_3
#define zSS_SORT_DISPLAY               zCONTROLX_SPECIFIC_4
#define zSS_SHOWVERTICALGRIDLINES      zCONTROLX_SPECIFIC_5
#define zSS_MULTISELECT                zCONTROLX_SPECIFIC_6
#define zSS_SCOPE_OI                   zCONTROLX_SPECIFIC_7
#define zSS_RECURSIVE_EVENT            zCONTROL_CENTERJUSTIFY

// Define case for the string data type

#define SS_CASE_LCASE          0
#define SS_CASE_NOCASE         1
#define SS_CASE_UCASE          2

// Text Styles

#define SS_TEXT_LEFT           0x001
#define SS_TEXT_RIGHT          0x002
#define SS_TEXT_CENTER         0x004
#define SS_TEXT_VCENTER        0x008
#define SS_TEXT_TOP            0x010
#define SS_TEXT_BOTTOM         0x020
#define SS_TEXT_WORDWRAP       0x040
#define SS_TEXT_SHADOW         0x080
#define SS_TEXT_PREFIX         0x100
#define SS_TEXT_SHADOWIN       0x200

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Grid( ZSubtask *pZSubtask,
      CWnd     *pWndParent,
      ZMapAct  *pzmaComposite,
      zVIEW    vDialog,
      zSHORT   nOffsetX,
      zSHORT   nOffsetY,
      zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CellRange.h: header file
//
// MFC Grid Control - interface for the ZCellRange class.
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.20+
//
/////////////////////////////////////////////////////////////////////////////

UINT
GetMouseScrollLines( );

// The code contained in this file is based on the original
// WorldCom Grid control written by Joe Willcoxson,
//      mailto:chinajoe@aol.com
//      http://users.aol.com/chinajoe

class ZCellRange
{
public:

   ZCellRange( long lMinRow = -1, int nMinCol = -1,
               long lMaxRow = -1, int nMaxCol = -1 )
   {
      Set( lMinRow, nMinCol, lMaxRow, nMaxCol );
   }

   void Set( long lMinRow = -1, int nMinCol = -1,
             long lMaxRow = -1, int nMaxCol = -1 );

   int  IsValid( ) const;
   int  InRange( long lRow, int nCol ) const;
   int  InRange( const ZCellId& cellID ) const;
   int  Count( ) { return( (m_lMaxRow - m_lMinRow + 1) *
                                             (m_nMaxCol - m_nMinCol + 1) ); }

   ZCellId  GetTopLeft( ) const;
   ZCellRange  Intersect( const ZCellRange& rhs ) const;

   long GetMinRow( ) const { return m_lMinRow;}
   void SetMinRow( long lMinRow ) { m_lMinRow = lMinRow;}

   int  GetMinCol( ) const { return m_nMinCol; }
   void SetMinCol( int nMinCol ) { m_nMinCol = nMinCol; }

   long GetMaxRow( ) const { return m_lMaxRow; }
   void SetMaxRow( long lMaxRow ) { m_lMaxRow = lMaxRow; }

   int  GetMaxCol( ) const { return m_nMaxCol; }
   void SetMaxCol( int nMaxCol ) { m_nMaxCol = nMaxCol;}

   int GetRowSpan( ) const { return m_lMaxRow - m_lMinRow + 1; }
   int GetColSpan( ) const { return m_nMaxCol - m_nMinCol + 1; }

   void operator=(const ZCellRange& rhs);
   int  operator==(const ZCellRange& rhs);
   int  operator!=(const ZCellRange& rhs);

protected:
   long m_lMinRow;
   int  m_nMinCol;
   long m_lMaxRow;
   int  m_nMaxCol;
};

inline void ZCellRange::Set( long lMinRow, int nMinCol,
                             long lMaxRow, int nMaxCol )
{
   m_lMinRow = lMinRow;
   m_nMinCol = nMinCol;
   m_lMaxRow = lMaxRow;
   m_nMaxCol = nMaxCol;
}

inline void ZCellRange::operator=(const ZCellRange& rhs)
{
    if ( this != &rhs )
       Set( rhs.m_lMinRow, rhs.m_nMinCol, rhs.m_lMaxRow, rhs.m_nMaxCol );
}

inline int ZCellRange::operator==(const ZCellRange& rhs)
{
   return( (m_lMinRow == rhs.m_lMinRow) && (m_nMinCol == rhs.m_nMinCol) &&
           (m_lMaxRow == rhs.m_lMaxRow) && (m_nMaxCol == rhs.m_nMaxCol) );
}

inline int ZCellRange::operator!=(const ZCellRange& rhs)
{
   return( !operator==(rhs) );
}

inline int ZCellRange::IsValid( ) const
{
   return( m_lMinRow >= 0 && m_nMinCol >= 0 &&
           m_lMaxRow >= 0 && m_nMaxCol >= 0 &&
           m_lMinRow <= m_lMaxRow && m_nMinCol <= m_nMaxCol );
}

inline int ZCellRange::InRange( long lRow, int nCol ) const
{
   return( lRow >= m_lMinRow && lRow <= m_lMaxRow &&
           nCol >= m_nMinCol && nCol <= m_nMaxCol );
}

inline int ZCellRange::InRange( const ZCellId& cellID ) const
{
   return( InRange( cellID.m_lRow, cellID.m_nCol ) );
}

inline ZCellId ZCellRange::GetTopLeft( ) const
{
   return( ZCellId( m_lMinRow, m_nMinCol ) );
}

inline ZCellRange ZCellRange::Intersect( const ZCellRange& rhs ) const
{
   return( ZCellRange( max( m_lMinRow,rhs.m_lMinRow ),
                       max( m_nMinCol,rhs.m_nMinCol ),
                       min( m_lMaxRow,rhs.m_lMaxRow ),
                       min( m_nMaxCol,rhs.m_nMaxCol ) ) );
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase.h : header file
//
// MFC Grid Control - Grid cell base class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.22+
//
/////////////////////////////////////////////////////////////////////////////

class ZGrid;

// Cell states
#define GVIS_FOCUSED            0x00000001
#define GVIS_SELECTED           0x00000002
#define GVIS_DROPHILITED        0x00000004
#define GVIS_READONLY           0x00000008
#define GVIS_FIXED              0x00000010
#define GVIS_FIXEDROW           0x00000020
#define GVIS_FIXEDCOL           0x00000040
#define GVIS_MODIFIED           0x00000080
#define GVIS_VIRTUALTEXT        0x00000100

// Cell data mask
#define GVIF_TEXT               LVIF_TEXT
#define GVIF_IMAGE              LVIF_IMAGE
#define GVIF_PARAM              LVIF_PARAM
#define GVIF_STATE              LVIF_STATE
#define GVIF_BKCLR              (GVIF_STATE << 1)
#define GVIF_FGCLR              (GVIF_STATE << 2)
#define GVIF_FORMAT             (GVIF_STATE << 3)
#define GVIF_FONT               (GVIF_STATE << 4)
#define GVIF_MARGIN             (GVIF_STATE << 5)
#define GVIF_ALL                (GVIF_TEXT | GVIF_IMAGE | GVIF_PARAM |  \
                                 GVIF_STATE | GVIF_BKCLR | GVIF_FGCLR | \
                                 GVIF_FORMAT | GVIF_FONT | GVIF_MARGIN)

// Used for Get/SetItem calls.
typedef struct _GV_ITEM
{
   long     lRow;        // row of item
   int      nCol;        // column of item
   UINT     mask;        // mask for use in getting/setting cell data
   DWORD    dwState;     // cell state (focus/hilighted etc.)
   DWORD    dwFormat;    // format of cell
   int      nImage;      // index of the list view item’s icon
   COLORREF clrBk;       // background color (or CLR_DEFAULT)
   COLORREF clrFg;       // forground color (or CLR_DEFAULT)
   LPARAM   lItemExtra;  // 32-bit value to associate with item
   LOGFONT  lfFont;      // cell font
   UINT     nMargin;     // internal cell margin
   CString  csText;      // text in cell

} GV_ITEM;

// Each cell contains one of these. Fields "row" and "column" are not stored
// since we will usually have access to them in other ways, and they are an
// extra 8 bytes per cell that is probably unnecessary.
class ZGridCellBase : public CObject
{
   friend class ZGrid;
   DECLARE_DYNAMIC( ZGridCellBase )

// Construction/Destruction
public:
   ZGridCellBase( );
   virtual ~ZGridCellBase( );

// Attributes
public:
   virtual void SetText( zCPCHAR cpcText, long lRow, int nCol ) = 0;
   virtual void SetData( LPARAM lCellData )      = 0;
   virtual void SetImage( int nImage )           = 0;
   virtual void SetState( DWORD dwState )  { m_dwState = dwState; }
   virtual void SetFormat( DWORD dwFormat )      = 0;
   virtual void SetTextClr( COLORREF clr )       = 0;
   virtual void SetBackClr( COLORREF clr )       = 0;
   virtual void SetFont( const LOGFONT *plf )    = 0;
   virtual void SetMargin( UINT uMargin )        = 0;
   virtual void SetGrid( ZGrid *pGrid )          = 0;
   virtual void SetCoords( long lRow, int nCol ) = 0;
   virtual zBOOL SetEditing( zBOOL bOn )           = 0;

   virtual void GetText( CString& csText, long lRow, int nCol ) const = 0;
   virtual LPARAM GetData( long lRow, int nCol ) const = 0;

   // May override TitleTip return.
   virtual void GetTipText( CString& csTipText, long lRow, int nCol ) const
   {
      GetText( csTipText, lRow, nCol );
   }

   virtual int        GetImage( )      const    = 0;
   virtual DWORD      GetState( )      const { return( m_dwState ); }
   virtual zBOOL      IsEditing( )     const { return( m_bEditing ); }
   virtual DWORD      GetFormat( )     const    = 0;
   virtual COLORREF   GetTextClr( )    const    = 0;
   virtual COLORREF   GetBackClr( )    const    = 0;
   virtual LOGFONT  * GetFont( )       const    = 0;
   virtual CFont    * GetFontObject( ) const    = 0;
   virtual ZGrid    * GetGrid( )       const    = 0;
   virtual CWnd     * GetEditWnd( )    const    = 0;
   virtual UINT       GetMargin( )     const    = 0;

   virtual ZGridCellBase * GetDefaultCell( ) const;

   virtual zBOOL IsDefaultFont( )      const    = 0;
   virtual zBOOL IsFocused( )          const { return( (m_dwState & GVIS_FOCUSED) ? TRUE : FALSE ); }
   virtual zBOOL IsFixed( )            const { return( (m_dwState & GVIS_FIXED) ? TRUE : FALSE ); }
   virtual zBOOL IsFixedCol( )         const { return( (m_dwState & GVIS_FIXEDCOL) ? TRUE : FALSE ); }
   virtual zBOOL IsFixedRow( )         const { return( (m_dwState & GVIS_FIXEDROW) ? TRUE : FALSE ); }
   virtual zBOOL IsSelected( )         const { return( (m_dwState & GVIS_SELECTED) ? TRUE : FALSE ); }
   virtual zBOOL IsReadOnly( )         const { return( (m_dwState & GVIS_READONLY) ? TRUE : FALSE ); }
   virtual zBOOL IsModified( )         const { return( (m_dwState & GVIS_MODIFIED) ? TRUE : FALSE ); }
   virtual zBOOL IsDropHighlighted( )  const { return( (m_dwState & GVIS_DROPHILITED) ? TRUE : FALSE ); }
   virtual zBOOL IsVirtualText( )      const { return( (m_dwState & GVIS_VIRTUALTEXT) ? TRUE : FALSE ); }
   virtual BOOL WantKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags ) { return( FALSE ); }

// Operators
public:
   virtual void operator=(const ZGridCellBase& cell);

// Operations
public:
   virtual void Reset( );

   virtual BOOL Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd = TRUE, BOOL bDrawText = TRUE );

   // i/o:  i=dims of cell rect; o=dims of text rect
   virtual zBOOL GetTextRect( CRect& rect, zBOOL bFull = FALSE );

   // May override for buttons, etc.
   virtual zBOOL GetTipTextRect( long lRow, int nCol, CRect& rect )
   {
      return( GetTextRect( rect ) );
   }

   virtual CSize GetTextExtent( zCPCHAR cpc, CDC *pDC = 0 );
   virtual CSize GetCellExtent( CDC *pDC, long lRow, int nCol );

    // Editing.
   virtual BOOL Edit( long lRow, int nCol, CRect rect, CPoint pt,
                      UINT uId, UINT uChar )
   {
      ASSERT( FALSE );  // force this to be overridden
      return( FALSE );
   }

   virtual zBOOL ValidateEdit( zCPCHAR cpc );
   virtual void EndEdit( BOOL bDestroy = TRUE ) {}

   // EFW - Added to print cells properly
   virtual zBOOL PrintCell( CDC *pDC, long lRow, int nCol, CRect rect );

   // Add additional protected grid members required of cells.
   LRESULT SendMessageToParent( long lRow, int nCol, int nMessage );

   virtual void OnEndEdit( );

protected:
   virtual void OnMouseEnter( );
   virtual void OnMouseOver( );
   virtual void OnMouseLeave( );
   virtual void OnClick( CPoint ptCellRelative );
   virtual void OnClickDown( CPoint ptCellRelative );
   virtual void OnRClick( CPoint ptCellRelative );
   virtual void OnDblClick( CPoint ptCellRelative );
   virtual BOOL OnSetCursor( );

protected:
   CString  m_csText;       // Cell text (or binary data if you wish ...)
   LPARAM   m_lCellExtra;   // 32-bit value to associate with item
   DWORD    m_dwState;      // Cell state (selected/focus/disabled etc.)
   zBOOL    m_bEditing;     // Cell being edited?
};

/////////////////////////////////////////////////////////////////////////////
// GridCell.h : header file
//
// MFC Grid Control - Grid cell class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.20+
//
/////////////////////////////////////////////////////////////////////////////

// Each cell contains one of these. Fields "row" and "column" are not stored
// since we will usually have access to them in other ways, and they are an
// extra 8 bytes per cell that is probably unnecessary.

class ZGridCell : public ZGridCellBase
{
   friend class ZGrid;
   DECLARE_DYNCREATE( ZGridCell )

// Construction/Destruction
public:
   ZGridCell( );
   virtual ~ZGridCell( );

// Attributes
public:
   void operator=(const ZGridCell& cell);

   virtual void  SetText( zCPCHAR cpcText, long lRow, int nCol );
   virtual void  SetImage( int nImage )           { m_nImage = nImage; }
   virtual void  SetData( LPARAM lCellData );
   virtual void  SetGrid( ZGrid *pGrid )          { m_pGrid = pGrid; }
// virtual void  SetState( const DWORD dwState ); - use base class version
   virtual void  SetFormat( DWORD dwFormat )      { m_dwFormat = dwFormat; }
   virtual void  SetTextClr( COLORREF clr )       { m_clrFgClr = clr; }
   virtual void  SetBackClr( COLORREF clr )       { m_clrBkClr = clr; }
   virtual void  SetFont( const LOGFONT *plf );
   virtual void  SetMargin( UINT uMargin )        { m_uMargin = uMargin; }
   virtual CWnd * GetEditWnd( ) const             { return( m_pEditWnd ); }
   virtual void  SetCoords( long lRow, int nCol ) {} // don't need to know
                                                     // the row and column
                                                     // for base implementation

   virtual zBOOL  SetEditing( zBOOL bOn );
   virtual void   GetText( CString& csText, long lRow, int nCol ) const;
   virtual LPARAM GetData( long lRow, int nCol ) const;
   virtual int    GetImage( ) const       { return( m_nImage ); }
   virtual ZGrid * GetGrid( ) const       { return( m_pGrid ); }
// virtual DWORD  GetState( ) const - use base class
   virtual DWORD  GetFormat( ) const;

   // TODO: change to use default cell
   virtual COLORREF GetTextClr( ) const   { return( m_clrFgClr ); }
   virtual COLORREF GetBackClr( ) const   { return( m_clrBkClr ); }
   virtual LOGFONT * GetFont( ) const;
   virtual CFont * GetFontObject( ) const;
   virtual UINT    GetMargin( ) const;

   virtual zBOOL   IsDefaultFont( ) const { return( m_plfFont == 0 ); }
   virtual void    Reset( );

// editing cells
public:
   virtual BOOL Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
protected:
   virtual void OnEndEdit( );

protected:
   int        m_nImage;      // Index of the list view item’s icon
   DWORD      m_dwFormat;
   COLORREF   m_clrFgClr;
   COLORREF   m_clrBkClr;
   LOGFONT    *m_plfFont;
   UINT       m_uMargin;

   ZGrid      *m_pGrid;      // Parent grid control
   CWnd       *m_pEditWnd;
};

// This class is for storing grid default values. It's a little heavy weight,
// so don't use it in bulk.
class ZGridDefaultCell : public ZGridCell
{
   DECLARE_DYNCREATE( ZGridDefaultCell )

// Construction/Destruction
public:
   ZGridDefaultCell( );
   virtual ~ZGridDefaultCell( );

public:
   virtual DWORD GetStyle( ) const;
   virtual void  SetStyle( DWORD dwStyle );
   virtual int   GetWidth( ) const;
   virtual int   GetHeight( ) const;
   virtual void  SetWidth( int nWidth );
   virtual void  SetHeight( int nHeight );

   // Disable these properties
   virtual void     SetData( LPARAM lParam )         { ASSERT( FALSE ); }
   virtual void     SetState( DWORD dwState )        { ASSERT( FALSE ); }
   virtual DWORD    GetState( ) const;
   virtual void     SetCoords( long lRow, int nCol ) { ASSERT( FALSE ); }
   virtual void     SetFont( const LOGFONT *plf );
   virtual LOGFONT * GetFont( ) const;
   virtual CFont  * GetFontObject( ) const;

protected:
   CSize m_sizeDefault;    // Default Size
   CFont m_fontCached;     // Cached font
   DWORD m_dwStyle;        // Cell Style - unused
};

/////////////////////////////////////////////////////////////////////////////
// GridCellCheck.h : header file
//
// MFC Grid Control - Grid combo cell class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.22+
//
//////////////////////////////////////////////////////////////////////

class ZGridCellCheck : public ZGridCell
{
   friend class ZGrid;
   DECLARE_DYNCREATE( ZGridCellCheck )

public:
   ZGridCellCheck( );

public:
   virtual BOOL Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
   zBOOL SetCheck( zSHORT nChecked, long lRow, int nCol );
   zBOOL GetCheck( long lRow, int nCol );

// Operations
   virtual CSize GetCellExtent( CDC *pDC, long lRow, int nCol );
   virtual void OnClick( CPoint ptCellRelative );
   virtual void OnClickDown( CPoint ptCellRelative );

   // i/o:  i=dims of cell rect; o=dims of text rect
   virtual zBOOL GetTextRect( CRect& rect, zBOOL bFull = FALSE );
   virtual BOOL WantKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );

protected:
   CRect GetCheckPlacement( int nCol );
   virtual BOOL Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd = TRUE, BOOL bDrawText = TRUE );

protected:
   zBOOL m_bChecked;
   CRect m_rect;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// GridCellCombo.h : header file
//
// MFC Grid Control - Grid combo cell class header file
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.10
//
//////////////////////////////////////////////////////////////////////

class ZGridCellCombo : public ZGridCell
{
   friend class ZGrid;
   DECLARE_DYNCREATE( ZGridCellCombo )

public:
   ZGridCellCombo( );

// editing cells
public:
   virtual BOOL Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
   virtual CWnd * GetEditWnd( ) const;

// Operations
public:
   virtual CSize GetCellExtent( CDC *pDC, long lRow, int nCol );

// ZGridCellCombo specific calls
public:
   void  SetOptions( CStringArray& ar );
   void  SetStyle( DWORD dwStyle )     { m_dwStyle = dwStyle; }
   DWORD GetStyle( )                   { return( m_dwStyle ); }

protected:
   virtual BOOL Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd = TRUE, BOOL bDrawText = TRUE );

   CStringArray m_Strings;
   DWORD        m_dwStyle;
};

/////////////////////////////////////////////////////////////////////////////
// ZSS_ComboEdit window

#define IDC_COMBOEDIT 1001

class ZSS_ComboEdit : public CEdit
{
// Construction
public:
   ZSS_ComboEdit( );

// Attributes
public:

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZSS_ComboEdit )
   virtual BOOL PreTranslateMessage( MSG* pMsg );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZSS_ComboEdit( );

   // Generated message map functions
protected:
   //{{AFX_MSG( ZSS_ComboEdit )
   afx_msg void OnKillFocus( CWnd* pNewWnd );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// ZInPlaceList window

class ZInPlaceList : public CComboBox
{
    friend class ZSS_ComboEdit;

// Construction
public:
   ZInPlaceList( CWnd *pParent,         // parent
                 CRect& rect,           // dimensions & location
                 DWORD dwStyle,         // window/combobox style
                 UINT uId,              // control ID
                 long lRow,             // row
                 int nCol,              // column
                 COLORREF clrFg,        // foreground color
                 COLORREF clrBk,        // background color
                 CStringArray& Items,   // items in list
                 CString csInitText,    // initial selection
                 UINT uFirstChar );     // first character to pass to control

// Attributes
public:
   ZSS_ComboEdit m_edit; // subclassed edit control

// Operations
public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZInPlaceList )
   protected:
   virtual void PostNcDestroy( );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZInPlaceList( );
   virtual void EndEdit( BOOL bDestroy = TRUE );

protected:
    int GetCorrectDropWidth( );

// Generated message map functions
protected:
   //{{AFX_MSG( ZInPlaceList )
   afx_msg void OnKillFocus( CWnd* pNewWnd );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnDropdown( );
   afx_msg UINT OnGetDlgCode( );
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT uCtlColor );
   //}}AFX_MSG
   //afx_msg void OnSelendOK( );

private:
   int      m_nNbrLines;
   CString  m_csInitText;
   long     m_lRow;
   int      m_nCol;
   UINT     m_nLastChar;
   zBOOL    m_bExitOnArrows;
   COLORREF m_clrFg;
   COLORREF m_clrBk;

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////

// GridCellDateTime.h: interface for the ZGridCellDateTime class.
//
// Provides the implementation for a datetime picker cell type of the
// grid control.
//
// For use with ZGrid v2.22+
//
/////////////////////////////////////////////////////////////////////////////

class ZGridCellDateTime : public ZGridCell
{
   friend class ZGrid;
   DECLARE_DYNCREATE( ZGridCellDateTime )

   CTime m_time;
   DWORD m_dwStyle;

public:
   ZGridCellDateTime( );
   ZGridCellDateTime( DWORD dwStyle );
   virtual ~ZGridCellDateTime( );

  // editing cells
public:
   void Init( DWORD dwStyle );
   virtual BOOL Edit( long lRow, int nCol, CRect rect, CPoint pt,
                      UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
   virtual CWnd * GetEditWnd( ) const;

   CTime * GetTime( ) { return( &m_time ); };
   void  SetTime( CTime time, long lRow, int nCol );
};

class ZInPlaceDateTime : public CDateTimeCtrl
{
// Construction
public:
   ZInPlaceDateTime( CWnd *pParent,         // parent
                     CRect& rect,           // dimensions & location
                     DWORD dwStyle,         // window/combobox style
                     UINT  nID,             // control ID
                     long lRow, int nCol,   // row and column
                     COLORREF clrFg,        // Foreground
                     COLORREF clrBk,        // Background color
                     CTime *pTime,
                     UINT uFirstChar );     // first character to pass to control

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZInPlaceList )
   protected:
   virtual void PostNcDestroy( );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~ZInPlaceDateTime( );
   virtual void EndEdit( BOOL bDestroy = TRUE );

// Generated message map functions
protected:
   //{{AFX_MSG( ZInPlaceList )
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg UINT OnGetDlgCode( );
   //}}AFX_MSG
   //afx_msg void OnSelendOK( );

private:
   CTime     *m_pTime;
   long      m_lRow;
   int       m_nCol;
   UINT      m_nLastChar;
   zBOOL     m_bExitOnArrows;
   COLORREF  m_clrFg;
   COLORREF  m_clrBk;

   DECLARE_MESSAGE_MAP( )
};

// GridCellNumeric.h: interface for the ZGridCellNumeric class.
//
// Written by Andrew Truckle [ajtruckle@wsatkins.co.uk]
//
/////////////////////////////////////////////////////////////////////////////

class ZGridCellNumeric : public ZGridCell
{
   DECLARE_DYNCREATE( ZGridCellNumeric )

public:
   virtual BOOL Edit( long lRow, int nCol, CRect rect, CPoint pt,
                      UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
};

// GridCellZeidon.h: interface for the ZGridCellZeidon class.
//
/////////////////////////////////////////////////////////////////////////////

class ZGridCellZeidon : public ZGridCell
{
   DECLARE_DYNCREATE( ZGridCellZeidon )

public:
   virtual BOOL Edit( long lRow, int nCol, CRect rect, CPoint pt,
                      UINT uId, UINT uChar );
   virtual void EndEdit( BOOL bDestroy = TRUE );
   virtual BOOL Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd = TRUE, BOOL bDrawText = TRUE );
   virtual BOOL WantKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );

   ZMapAct  *m_pzma;

protected:
   CRect   m_rect;
};

// GridURLCell.h: interface for the ZGridURLCell class.
//
/////////////////////////////////////////////////////////////////////////////

typedef struct
{
   zCPCHAR szURLPrefix;
   int     nLth;
} URLStruct;

class ZGridURLCell : public ZGridCell
{
   DECLARE_DYNCREATE( ZGridURLCell )

public:
   ZGridURLCell( );
   virtual ~ZGridURLCell( );

   virtual BOOL Draw( CDC *pDC, long lRow, int nCol, CRect rect,
                      BOOL bEraseBkgnd = TRUE, BOOL bDrawText = FALSE );
   virtual BOOL Edit( long lRow, int nCol, CRect rect,
                      CPoint pt, UINT uId, UINT uChar );
   virtual zCPCHAR GetTipText( ) { return( 0 ); }
   void SetAutoLaunchUrl( zBOOL bLaunch = TRUE ) { m_bLaunchUrl = bLaunch; }
   zBOOL GetAutoLaunchUrl( ) { return( m_bLaunchUrl ); }

protected:
   virtual BOOL OnSetCursor( );
   virtual void OnClick( CPoint ptCellRelative );

   zBOOL HasUrl( CString cs );
   zBOOL OverURL( CPoint& pt, CString& csURL );

protected:
#ifndef _WIN32_WCE
   static HCURSOR g_hLinkCursor;      // Hyperlink mouse cursor
   HCURSOR GetHandCursor( );
#endif
   static URLStruct g_szURIprefixes[ ];

protected:
   COLORREF m_clrUrl;
   zBOOL    m_bLaunchUrl;
   CRect    m_rect;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// GridDropTarget.h : header file
//
// MFC Grid Control - Drag/Drop target implementation
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.10+
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// ZGridDropTarget command target

class ZGridDropTarget : public COleDropTarget
{
public:
   ZGridDropTarget( );
   virtual ~ZGridDropTarget( );

// Attributes
public:
   ZGrid  *m_pGridCtrl;
   zBOOL  m_bRegistered;

// Operations
public:
   zBOOL Register( ZGrid *pGridCtrl );
   virtual void Revoke( );

   BOOL        OnDrop( CWnd *pWnd, COleDataObject *pDataObject,
                       DROPEFFECT dropEffect, CPoint pt );
   DROPEFFECT  OnDragEnter( CWnd *pWnd, COleDataObject *pDataObject,
                            DWORD dwKeyState, CPoint pt );
   void        OnDragLeave( CWnd *pWnd );
   DROPEFFECT  OnDragOver( CWnd *pWnd, COleDataObject *pDataObject,
                           DWORD dwKeyState, CPoint pt );
   DROPEFFECT  OnDragScroll( CWnd *pWnd, DWORD dwKeyState, CPoint pt );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZGridDropTarget )
   //}}AFX_VIRTUAL

// Implementation
protected:

   // Generated message map functions
   //{{AFX_MSG( ZGridDropTarget )
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
// GridCtrl.h : header file
//
// MFC Grid Control - main header
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.20+
//
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Defines - these determine the features (and the final size) of the
// final code.
/////////////////////////////////////////////////////////////////////////////

//#define GRIDCONTROL_NO_TITLETIPS  // do not use titletips for cells with large data
//#define GRIDCONTROL_NO_DRAGDROP   // do not use OLE drag and drop
//#define GRIDCONTROL_NO_CLIPBOARD  // do not use clipboard routines

#ifdef _WIN32_WCE
#   define GRIDCONTROL_NO_TITLETIPS // do not use titletips for cells with large data
#   define GRIDCONTROL_NO_DRAGDROP  // do not use OLE drag and drop
#   define GRIDCONTROL_NO_CLIPBOARD // do not use clipboard routines
#   define GRIDCONTROL_NO_PRINTING  // do not use printing routines
#   ifdef WCE_NO_PRINTING           // older versions of CE had different #def's
#       define _WIN32_WCE_NO_PRINTING
#   endif
#   ifdef WCE_NO_CURSOR
#       define _WIN32_WCE_NO_CURSOR
#   endif
#endif  // _WIN32_WCE

// Use this as the classname when inserting this control as a custom control
// in the MSVC++ dialog editor
#define GRIDCTRL_CLASSNAME    _T( "ZGridCtrl" )  // Window class name
#define IDC_INPLACE_CONTROL   8                  // ID of inplace edit controls

/////////////////////////////////////////////////////////////////////////////
// Helper functions
/////////////////////////////////////////////////////////////////////////////

// Backwards compatibility for pre 2.20 grid versions
#define DDX_GridControl( pDX, nIDC, rControl ) DDX_Control( pDX, nIDC, rControl )

/////////////////////////////////////////////////////////////////////////////
// Structures
/////////////////////////////////////////////////////////////////////////////

// This structure sent to Grid's parent in a WM_NOTIFY message and passed
// to Zeidon events.
typedef struct tagNM_GRIDVIEW
{
   NMHDR hdr;
   zGRID_CELL cell;
} NM_GRIDVIEW;

// This is sent to the Grid from child in-place edit controls.
typedef struct tagGV_DISPINFO
{
   NMHDR   hdr;
   GV_ITEM item;
} GV_DISPINFO;

// This is sent to the Grid from child in-place edit controls.
typedef struct tagGV_CACHEHINT
{
   NMHDR      hdr;
   ZCellRange range;
} GV_CACHEHINT;

// Storage typedef for each row in the grid.
typedef CTypedPtrArray<CObArray, ZGridCellBase *> GRID_ROW;

// For virtual mode callback.
typedef BOOL (CALLBACK * GRIDCALLBACK)( GV_DISPINFO *, LPARAM );

/////////////////////////////////////////////////////////////////////////////
// Defines
/////////////////////////////////////////////////////////////////////////////

// Stealing flags that have no meaning if set as a column control.
#define zMAPACT_GRIDCOL_SETWIDTH  zMAPACT_FULL_CLIENT
#define zMAPACT_GRIDCOL_MINWIDTH  zMAPACT_ABSOLUTE_PIXEL_SIZE
#define zMAPACT_GRIDCOL_SETHEIGHT zMAPACT_CLIENT_TYPE
#define zMAPACT_GRIDCOL_MINHEIGHT zMAPACT_ABSOLUTE_PIXEL_POS

// Notification messages.
#define zGRDN_CLICK                     0x0001
#define zGRDN_DBLCLK                    0x0002
#define zGRDN_BEGINEDIT                 0x0003
#define zGRDN_ENDEDIT                   0x0004
#define zGRDN_COLUMNCLICK               0x0005
#define zGRDN_BEFOREPASTECELL           0x0006
#define zGRDN_AFTERPASTECELL            0x0007
#define zGRDN_CELLSELECTCHANGING        0x0047
#define zGRDN_CELLSELECTED              0x0048
#define zGRDN_ROWSELECTCHANGING         0x0049
#define zGRDN_RBUTTONDOWN               0x0050
#define zGRDN_CHECKBOXCHANGE            0x0051
#define zGRDN_DELETEROWDATA             0x0052

// Grid line/scrollbar selection.
#define GVL_NONE                0L      // Neither
#define GVL_HORZ                1L      // Horizontal line or scrollbar
#define GVL_VERT                2L      // Vertical line or scrollbar
#define GVL_BOTH                3L      // Both

// Autosizing option.
#define GVS_DEFAULT             0
#define GVS_HEADER              1       // Size using column fixed cells data only
#define GVS_DATA                2       // Size using column non-fixed cells data only
#define GVS_BOTH                3       // Size using column fixed and non-fixed

// Cell Searching options.
#define GVNI_FOCUSED            0x0001
#define GVNI_SELECTED           0x0002
#define GVNI_DROPHILITED        0x0004
#define GVNI_READONLY           0x0008
#define GVNI_FIXED              0x0010
#define GVNI_MODIFIED           0x0020

#define GVNI_ABOVE              LVNI_ABOVE
#define GVNI_BELOW              LVNI_BELOW
#define GVNI_TOLEFT             LVNI_TOLEFT
#define GVNI_TORIGHT            LVNI_TORIGHT
#define GVNI_ALL                (LVNI_BELOW | LVNI_TORIGHT | LVNI_TOLEFT)
#define GVNI_AREA               (LVNI_BELOW | LVNI_TORIGHT)

// Hit test values (not yet implemented).
#define GVHT_DATA               0x0000
#define GVHT_TOPLEFT            0x0001
#define GVHT_COLHDR             0x0002
#define GVHT_ROWHDR             0x0004
#define GVHT_COLSIZER           0x0008
#define GVHT_ROWSIZER           0x0010
#define GVHT_LEFT               0x0020
#define GVHT_RIGHT              0x0040
#define GVHT_ABOVE              0x0080
#define GVHT_BELOW              0x0100

// Messages sent to the grid's parent (More will be added in future).
#define GVN_BEGINDRAG           LVN_BEGINDRAG
#define GVN_BEGINEDIT           LVN_BEGINLABELEDIT
#define GVN_BEGINRDRAG          LVN_BEGINRDRAG
#define GVN_COLUMNCLICK         LVN_COLUMNCLICK
#define GVN_DELETEITEM          LVN_DELETEITEM
#define GVN_ENDEDIT             LVN_ENDLABELEDIT
#define GVN_SELCHANGING         LVN_ITEMCHANGING
#define GVN_SELCHANGED          LVN_ITEMCHANGED
#define GVN_GETDISPINFO         LVN_GETDISPINFO
#define GVN_ODCACHEHINT         LVN_ODCACHEHINT
#define GVN_ODSTATECHANGED      LVN_ODSTATECHANGED

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

template<> AFX_INLINE UINT AFXAPI HashKey(LARGE_INTEGER key)
{
   // default identity hash - works for most primitive values
   return( ((UINT)(void *)(DWORD)(key.LowPart + key.HighPart)) >> 4 );
}

template<> AFX_INLINE BOOL AFXAPI
CompareElements( const LARGE_INTEGER* pElement1,
                 const LARGE_INTEGER* pElement2 )
{
   return( pElement1->HighPart == pElement2->HighPart &&
           pElement1->LowPart == pElement2->LowPart );
}

/////////////////////////////////////////////////////////////////////////////
// ZGrid window

// Zeidon ZGrid spread sheet control
class ZDCTL_CLASS ZGrid : public CWnd, public ZMapAct
{
   DECLARE_DYNAMIC( ZGrid )

public:
friend BOOL CALLBACK
GridCallback( GV_DISPINFO *pDispInfo, LPARAM lParam );
friend int CALLBACK
CellTextCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lGrid );
friend int CALLBACK
CellNumericCompare( LPARAM lParam1, LPARAM lParam2, LPARAM lGrid );

   // ctor
   ZGrid( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZGrid( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 );
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual LRESULT RelayEvent( NM_GRIDVIEW& nmgv );
   virtual zSHORT SetRowColText( zCPCHAR cpcText,
                                 zLONG lRow = 0, zSHORT nCol = 0 );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uLth,
                                 zLONG lRow = 0, zSHORT nCol = 0 );
   virtual zLONG  SetZCtrlProperty( zLONG   lPropertyType,
                                    zULONG  ulNumericProperty,
                                    zCPCHAR cpcStringProperty );
   virtual zLONG  GetZCtrlProperty( zLONG   lPropertyType,
                                    zPULONG pulNumericProperty,
                                    zPCHAR  pchStringProperty,
                                    zLONG   lMaxLth );
   virtual zLONG  SetZCtrlState( zLONG  lStatusType, zULONG ulValue );
   virtual zLONG  SelectAll( );
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
   virtual zSHORT PostCreateCtrls( WPARAM wFlag = 0 );
   virtual zLONG  TransferTextFromInplace( zCPCHAR cpcText );

   zSHORT FormatTextForCell( CString& csText, zLONG lRow, zSHORT nCol );
   zSHORT TransferDataFromCellToOI( zCPCHAR cpcText, zLONG lRow, zSHORT nCol );
   zSHORT LockColumn( zSHORT nCol, zBOOL bLock );
   zSHORT GetColumnTitle( zPCHAR pchTitle, zSHORT nCol );
   zBOOL  SetColumnTitle( zPCHAR pchTitle, zSHORT nCol );
   int    GetPreferencesFromIni( );
   zBOOL  InformCompositeOfKillFocus( ZMapAct *pzmaLosingFocus );
   void   CorrelateZeidonSelectToListSelect( );
   void   PostOnClickDown( ZGridCellBase *pCell, CPoint ptClicked );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnSetFocus( CWnd *pWndLostFocus );
   afx_msg void OnKillFocus( CWnd *pWndGetFocus );

   afx_msg void OnGridDblClick( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridClick( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridRClick( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridStartEdit( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridEndEdit( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridStartSelChange( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridEndSelChange( NMHDR *pNotifyStruct, LRESULT *pResult );
   afx_msg void OnGridBeginDrag( NMHDR *pNotifyStruct, LRESULT *pResult );

   afx_msg LRESULT OnPostedClickDown( WPARAM wParam, LPARAM lParam );

   virtual BOOL PreTranslateMessage( MSG *pMsg );
   virtual BOOL DestroyWindow( );

   zBOOL     m_bRejectEditAttempts;
   zBOOL     m_bRejectEditChanges;

   zVIEW     m_vApp;
   zVIEW     m_vAppList;
   CString   m_csViewName;
   CString   m_csViewNameList;
   zSHORT    m_nDisplaySelSet;
   zSHORT    m_nSelectSelSet;
   zCHAR     m_chMultiSelect;
   zCHAR     m_chListOnlyWhenSelected;
   zSHORT    m_nRetryMapFromOI;  // flag to permit retry of MapFromOI
   zLONG     m_lEntityCnt;       // number of entities in list
   zULONG    m_ulEntityKey;
   zULONG    m_ulFlag;
   zULONG    m_ulSubtype;
   zPCHAR    m_pchListInfo;
   zBOOL     m_bSelectAll;
   zBOOL     m_bSelect;

   friend class ZGridCell;
   friend class ZGridCellBase;

// Construction
public:

   BOOL Create( const RECT& rect, CWnd *parent, UINT uId,
                DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE );

///////////////////////////////////////////////////////////////////////////////////
// Attributes
///////////////////////////////////////////////////////////////////////////////////
public:
   long  GetRowCount( ) const                    { return( m_lRows ); }
   int   GetColumnCount( ) const                 { return( m_nCols ); }
   int   GetFixedRowCount( ) const               { return( m_nFixedRows ); }
   int   GetFixedColumnCount( ) const            { return( m_nFixedCols ); }
   zBOOL SetRowCount( long lRows = 10 );
   zBOOL SetColumnCount( int nCols = 10 );
   zBOOL SetFixedRowCount( int nFixedRows = 1 );
   zBOOL SetFixedColumnCount( int nFixedCols = 1 );

   int   GetRowHeight( long lRow ) const;
   zBOOL SetRowHeight( long lRow, int height );
   int   GetColumnWidth( int nCol ) const;
   zBOOL SetColumnWidth( int nCol, int width );

   zBOOL GetCellOrigin( long lRow, int nCol, CPoint& pt );
   zBOOL GetCellOrigin( const ZCellId& cell, CPoint& pt );
   zBOOL GetCellRect( long lRow, int nCol, CRect& rect );
   zBOOL GetCellRect( const ZCellId& cell, CRect& rect );
   void  GetRowBoundary( zLONG lRow, CRect& rect );

   zBOOL GetTextRect( const ZCellId& cell, CRect& rect );
   zBOOL GetTextRect( long lRow, int nCol, CRect& rect );

   ZCellId GetCellFromPt( CPoint pt, zBOOL bAllowFixedCellCheck = TRUE );

   int   GetFixedRowHeight( ) const;
   int   GetFixedColumnWidth( ) const;
   long  GetVirtualWidth( ) const;
   long  GetVirtualHeight( ) const;

   CSize GetTextExtent( long lRow, int nCol, zCPCHAR cpcText );

   // EFW - Get extent of current text in cell
   inline CSize GetCellTextExtent( long lRow, int nCol )
   {
      CString csText;

      GetItemText( csText, lRow, nCol );
      return( GetTextExtent( lRow, nCol, csText ) );
   }

   void     SetGridBkColor( COLORREF clr )   { m_clrGridBk = clr; }
   COLORREF GetGridBkColor( ) const          { return( m_clrGridBk ); }
   void     SetGridLineColor( COLORREF clr ) { m_clrGridLine = clr; }
   COLORREF GetGridLineColor( ) const        { return( m_clrGridLine ); }

   void     SetTitleTipBackClr( COLORREF clr = CLR_DEFAULT ) { m_clrTTipBack = clr; }
   COLORREF GetTitleTipBackClr( )            { return( m_clrTTipBack ); }
   void     SetTitleTipTextClr( COLORREF clr = CLR_DEFAULT ) { m_clrTTipText = clr; }
   COLORREF GetTitleTipTextClr( )            { return( m_clrTTipText ); }

#if 0
   // **************************************************************************** *//
   // These have been deprecated. Use GetDefaultCell and then set the colors
   void     SetTextColor( COLORREF clr )     { m_cellDefault.SetTextClr( clr ); }
   COLORREF GetTextColor( )                  { return m_cellDefault.GetTextClr( ); }
   void     SetTextBkColor( COLORREF clr )   { m_cellDefault.SetBackClr( clr ); }
   COLORREF GetTextBkColor( )                { return m_cellDefault.GetBackClr( ); }
   void     SetFixedTextColor( COLORREF clr ) { m_cellFixedRowDef.SetTextClr( clr );
                                                m_cellFixedColDef.SetTextClr( clr );
                                                m_cellFixedRowColDef.SetTextClr( clr ); }
   COLORREF GetFixedTextColor( ) const       { return m_cellFixedRowDef.GetTextClr( ); }
   void     SetFixedBkColor( COLORREF clr )  { m_cellFixedRowDef.SetBackClr( clr );
                                               m_cellFixedColDef.SetBackClr( clr );
                                               m_cellFixedRowColDef.SetBackClr( clr ); }
   COLORREF GetFixedBkColor( ) const         { return m_cellFixedRowDef.GetBackClr( ); }
   void     SetGridColor( COLORREF clr )     { SetGridLineColor( clr ); }
   COLORREF GetGridColor( )                  { return GetGridLineColor( ); }
   void     SetBkColor( COLORREF clr )       { SetGridBkColor( clr ); }
   COLORREF GetBkColor( )                    { return GetGridBkColor( ); }

   void     SetDefCellMargin( int nMargin )  { m_cellDefault.SetMargin( nMargin );
                                               m_cellFixedRowDef.SetMargin( nMargin );
                                               m_cellFixedColDef.SetMargin( nMargin );
                                               m_cellFixedRowColDef.SetMargin( nMargin ); }
   int      GetDefCellMargin( ) const        { return m_cellDefault.GetMargin( ); }

   int      GetDefCellHeight( ) const        { return m_cellDefault.GetHeight( ); }
   void     SetDefCellHeight( int nHeight )  { m_cellDefault.SetHeight( nHeight );
                                               m_cellFixedRowDef.SetHeight( nHeight );
                                               m_cellFixedColDef.SetHeight( nHeight );
                                               m_cellFixedRowColDef.SetHeight( nHeight ); }
   int      GetDefCellWidth( ) const         { return m_cellDefault.GetWidth( ); }
   void     SetDefCellWidth( int nWidth )    { m_cellDefault.SetWidth( nWidth );
                                               m_cellFixedRowDef.SetWidth( nWidth );
                                               m_cellFixedColDef.SetWidth( nWidth );
                                               m_cellFixedRowColDef.SetWidth( nWidth ); }

    // **************************************************************************** *//
#endif

   long GetSelectedCount( ) const                 { return( m_SelectedCellMap.GetCount( ) ); }

   ZCellId SetFocusCell( ZCellId cell );
   ZCellId SetFocusCell( long lRow, int nCol );
   ZCellId GetFocusCell( ) const                  { return( m_cellIdCurrent ); }

   void  SetVirtualMode( zBOOL bVirtual );
   zBOOL GetVirtualMode( ) const                  { return( m_bVirtualMode ); }
   void  SetCallbackFunc( GRIDCALLBACK pCallback,
                          LPARAM lParam )          { m_pfnCallback = pCallback;
                                                    m_lParam = lParam; }
   GRIDCALLBACK GetCallbackFunc( )                { return( m_pfnCallback ); }


   void SetImageList( CImageList *pList )         { m_pImageList = pList; }
   CImageList * GetImageList( ) const             { return( m_pImageList ); }

   void SetGridLines( int nWhichLines = GVL_BOTH );
   int  GetGridLines( ) const                     { return( m_nGridLines ); }
   void SetEditable( zBOOL bEditable = TRUE )     { m_bEditable = bEditable; }
   zBOOL IsEditable( ) const                      { return( m_bEditable ); }
   void SetListMode( zBOOL bEnableListMode = TRUE );
   zBOOL GetListMode( ) const                     { return( m_bListMode ); }
   void SetSingleRowSelection( zBOOL bSing = TRUE ) { m_bSingleRowSelection = bSing; }
   zBOOL GetSingleRowSelection( )                 { return( m_bSingleRowSelection & m_bListMode ); }
   void SetSingleColSelection( zBOOL bSing = TRUE ) { m_bSingleColSelection = bSing; }
   zBOOL GetSingleColSelection( )                 { return( m_bSingleColSelection ); }
   void EnableSelection( zBOOL bEnable = TRUE )
   {
      ResetSelectedRange( );
      m_bEnableSelection = bEnable;
      ResetSelectedRange( );
   }
   zBOOL IsSelectable( ) const                     { return( m_bEnableSelection ); }
   void SetFixedColumnSelection( zBOOL bSelect )   { m_bFixedColumnSelection = bSelect; }
   zBOOL GetFixedColumnSelection( )                { return( m_bFixedColumnSelection ); }
   void SetFixedRowSelection( zBOOL bSelect )      { m_bFixedRowSelection = bSelect; }
   zBOOL GetFixedRowSelection( )                   { return( m_bFixedRowSelection ); }
   void EnableDragAndDrop( zBOOL bAllow = TRUE )   { m_bAllowDragAndDrop = bAllow; }
   zBOOL GetDragAndDrop( ) const                   { return( m_bAllowDragAndDrop ); }
   void SetRowResize( zBOOL bResize = TRUE )       { m_bAllowRowResize = bResize; }
   zBOOL GetRowResize( ) const                     { return( m_bAllowRowResize ); }
   void SetColumnResize( zBOOL bResize = TRUE )    { m_bAllowColumnResize = bResize; }
   zBOOL GetColumnResize( ) const                  { return( m_bAllowColumnResize ); }
   void SetHeaderSort( zBOOL bSortOnClick = TRUE ) { m_bSortOnClick = bSortOnClick; }
   zBOOL GetHeaderSort( ) const                    { return( m_bSortOnClick ); }
   void SetHandleTabKey( zBOOL bHandleTab = TRUE ) { m_bHandleTabKey = bHandleTab; }
   zBOOL GetHandleTabKey( ) const                  { return( m_bHandleTabKey ); }
   void SetDoubleBuffering( zBOOL bBuffer = TRUE ) { m_bDoubleBuffer = bBuffer; }
   zBOOL GetDoubleBuffering( ) const               { return( m_bDoubleBuffer ); }
   void EnableTitleTips( zBOOL bEnable = TRUE )    { m_bTitleTips = bEnable; }
   zBOOL GetTitleTips( )                           { return( m_bTitleTips ); }
   void SetSortColumn( int nCol );
   int  GetSortColumn( ) const
   {
      return( m_nSortColumn );
   }
   void SetSortAscending( zCHAR chAscendDescend )  { m_chAscendDescend = chAscendDescend; }
   zBOOL GetSortAscending( ) const                 { return( m_chAscendDescend != 'D' ); }
   void SetTrackFocusCell( zBOOL bTrack )          { m_bTrackFocusCell = bTrack; }
   zBOOL GetTrackFocusCell( )                      { return( m_bTrackFocusCell ); }
   void SetFrameFocusCell( zBOOL bFrame )          { m_bFrameFocus = bFrame; }
   zBOOL GetFrameFocusCell( )                      { return( m_bFrameFocus ); }
   void SetAutoSizeStyle( int nStyle = GVS_BOTH )  { m_nAutoSizeColumnStyle = nStyle; }
   int  GetAutoSizeStyle( )                        { return( m_nAutoSizeColumnStyle ); }

   void EnableHiddenColUnhide( zBOOL bEnable = TRUE ) { m_bHiddenColUnhide = bEnable; }
   zBOOL GetHiddenColUnhide( )                     { return( m_bHiddenColUnhide ); }
   void EnableHiddenRowUnhide( zBOOL bEnable = TRUE ) { m_bHiddenRowUnhide = bEnable; }
   zBOOL GetHiddenRowUnhide( )                     { return( m_bHiddenRowUnhide ); }

   void EnableColumnHide( zBOOL bEnable = TRUE )   { m_bAllowColHide = bEnable; }
   zBOOL GetColumnHide( )                          { return( m_bAllowColHide ); }
   void EnableRowHide( zBOOL bEnable = TRUE )      { m_bAllowRowHide = bEnable; }
   zBOOL GetRowHide( )                             { return( m_bAllowRowHide ); }
   zBOOL SetAllCellStates( int nCol, DWORD dwState, int nRemoveSetAdd );

/////////////////////////////////////////////////////////////////////////////
// Default Grid cells.  Use these for setting default values such as colors
// and fonts.
/////////////////////////////////////////////////////////////////////////////
public:
   ZGridCellBase * GetDefaultCell( zBOOL bFixedRow, zBOOL bFixedCol ) const;

/////////////////////////////////////////////////////////////////////////////
// Grid cell Attributes
/////////////////////////////////////////////////////////////////////////////
public:
   ZGridCellBase * GetCell( long lRow, int nCol ) const; // get actual cell!

   void   SetModified( zBOOL bModified = TRUE, long lRow = -1, int nCol = -1 );
   zBOOL  GetModified( long lRow = -1, int nCol = -1 );
   zBOOL  IsCellFixed( long lRow, int nCol );

   zBOOL  SetItem( const GV_ITEM *pItem );
   zBOOL  GetItem( GV_ITEM *pItem );
   zBOOL  SetItemText( long lRow, int nCol, zCPCHAR cpcText );
   // The following was virtual. If you want to override, use
   // ZGridCellBase-derived class's GetText( ) to accomplish same thing.
   void   GetItemText( CString& csText, long lRow, int nCol ) const;

   // EFW - 06/13/99 - Added to support printf-style formatting codes.
   // Also supports use with a string resource ID.
#if !defined( _WIN32_WCE ) || ( _WIN32_WCE >= 210 )
   zBOOL  SetItemTextFmt( long lRow, int nCol, zCPCHAR szFmt, ... );
   zBOOL  SetItemTextFmtID( long lRow, int nCol, UINT uId, ... );
#endif

   zBOOL  SetItemData( long lRow, int nCol, LPARAM lParam );
   LPARAM GetItemData( long lRow, int nCol ) const;
   zBOOL  SetItemImage( long lRow, int nCol, int iImage );
   int    GetItemImage( long lRow, int nCol ) const;
   zBOOL  SetItemState( long lRow, int nCol, UINT state );
   UINT   GetItemState( long lRow, int nCol ) const;
   zBOOL  SetItemFormat( long lRow, int nCol, UINT uFormat );
   UINT   GetItemFormat( long lRow, int nCol ) const;
   zBOOL  SetItemBkColor( long lRow, int nCol, COLORREF clr = CLR_DEFAULT );
   COLORREF GetItemBkColor( long lRow, int nCol ) const;
   zBOOL  SetItemFgColor( long lRow, int nCol, COLORREF clr = CLR_DEFAULT );
   COLORREF GetItemFgColor( long lRow, int nCol ) const;
   zBOOL  SetItemFont( long lRow, int nCol, const LOGFONT *lf );
   const LOGFONT *GetItemFont( long lRow, int nCol );

   zBOOL  IsItemEditing( long lRow, int nCol );

   ZGridCellBase * SetCellType( long lRow, int nCol, CRuntimeClass *pRuntimeClass );
   zBOOL  SetDefaultCellType( CRuntimeClass *pRuntimeClass );

///////////////////////////////////////////////////////////////////////////////////
// Operations
///////////////////////////////////////////////////////////////////////////////////
public:
   int   InsertColumn( zCPCHAR cpcHeading,
                       UINT uFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE,
                       int nColumn = -1 );
   int   InsertRow( zCPCHAR cpcHeading, long lRow = -1 );
   zBOOL DeleteColumn( int nColumn );
   zBOOL DeleteRow( long lRow );
   zBOOL DeleteNonFixedRows( );
   zBOOL DeleteAllItems( );
   void  ClearCells( ZCellRange Selection );

   zBOOL AutoSizeRow( long lRow, zBOOL bResetScroll = TRUE );
   zBOOL AutoSizeColumn( int nCol, UINT uAutoSizeStyle = GVS_DEFAULT,
                         zBOOL bResetScroll = TRUE );
   void  AutoSizeRows( );
   void  AutoSizeColumns( UINT uAutoSizeStyle = GVS_DEFAULT );
   void  AutoSize( UINT uAutoSizeStyle = GVS_DEFAULT );
   void  ExpandColumnsToFit( zBOOL bExpandFixed = TRUE );
   void  ExpandLastColumn( );
   void  ExpandRowsToFit( zBOOL bExpandFixed = TRUE );
   void  ExpandToFit( zBOOL bExpandFixed = TRUE );

   void  Refresh( );
   void  AutoFill( );   // Fill grid with blank cells

   void  EnsureVisible( ZCellId &cell )
   {
      EnsureVisible( cell.m_lRow, cell.m_nCol );
   }
   void  EnsureVisible( long lRow, int nCol );
   zBOOL IsCellVisible( long lRow, int nCol );
   zBOOL IsCellVisible( ZCellId cell );
   zBOOL IsCellEditable( long lRow, int nCol ) const;
   zBOOL IsCellEditable( ZCellId &cell ) const;
   zBOOL IsCellSelected( long lRow, int nCol ) const;
   zBOOL IsCellSelected( ZCellId &cell ) const;

   // SetRedraw stops/starts redraws on things like changing the # rows/columns
   // and autosizing, but not for user-intervention such as resizes
   void  SetRedraw( BOOL bAllowDraw, BOOL bResetScrollBars = FALSE );
   zBOOL RedrawCell( long lRow, int nCol, CDC *pDC = NULL );
   zBOOL RedrawCell( const ZCellId& cell, CDC *pDC = NULL );
   zBOOL RedrawRow( long lRow );
   zBOOL RedrawColumn( int nCol );

#ifndef _WIN32_WCE
   zBOOL Save( zCPCHAR filename, TCHAR chSeparator = _T( ',' ) );
   zBOOL Load( zCPCHAR filename, TCHAR chSeparator = _T( ',' ) );
#endif

///////////////////////////////////////////////////////////////////////////////////
// Cell Ranges.
///////////////////////////////////////////////////////////////////////////////////
public:
   ZCellRange GetCellRange( ) const;
   ZCellRange GetSelectedCellRange( ) const;
   void SetSelectedRange( const ZCellRange& Range, zBOOL bForceRepaint = FALSE,
                          zBOOL bSelectCells = TRUE );
   void SetSelectedRange( long lMinRow, int nMinCol, long lMaxRow, int nMaxCol,
                          zBOOL bForceRepaint = FALSE, zBOOL bSelectCells = TRUE );
   zBOOL IsValid( long lRow, int nCol ) const;
   zBOOL IsValid( const ZCellId& cell ) const;
   zBOOL IsValid( const ZCellRange& range ) const;

///////////////////////////////////////////////////////////////////////////////////
// Clipboard, drag and drop, and cut n' paste operations.
///////////////////////////////////////////////////////////////////////////////////
#ifndef GRIDCONTROL_NO_CLIPBOARD
   virtual void CutSelectedText( );
   virtual COleDataSource * CopyTextFromGrid( );
   virtual void WriteRow( CSharedFile& sf, ZCellRange& Selection,
                          zLONG lMinRow, zLONG lMaxRow, int nMinCol, int nMaxCol );
   virtual zBOOL PasteTextToGrid( ZCellId cell, COleDataObject *pDataObject,
                                  zBOOL bSelectPastedCells = TRUE );
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
public:
   virtual void OnBeginDrag( );
   virtual DROPEFFECT OnDragEnter( COleDataObject *pDataObject, DWORD dwKeyState, CPoint pt );
   virtual DROPEFFECT OnDragOver( COleDataObject *pDataObject, DWORD dwKeyState, CPoint pt );
   virtual void OnDragLeave( );
   virtual BOOL OnDrop( COleDataObject *pDataObject, DROPEFFECT dropEffect, CPoint pt );
#endif

#ifndef GRIDCONTROL_NO_CLIPBOARD
   virtual void OnEditCut( );
   virtual void OnEditCopy( );
   virtual void OnEditPaste( );
#endif
   virtual void OnEditSelectAll( );

///////////////////////////////////////////////////////////////////////////////////
// Misc.
///////////////////////////////////////////////////////////////////////////////////
public:
   ZCellId GetNextItem( ZCellId& cell, int nFlags ) const;

   zBOOL SortItems( int nCol, zBOOL bAscending, LPARAM data = 0 );
   zBOOL SortTextItems( int nCol, zBOOL bAscending, LPARAM data = 0 );
   zBOOL SortItems( PFNLVCOMPARE pfnCompare, int nCol,
                    zBOOL bAscending, LPARAM data = 0 );

   void SetCompareFunction( PFNLVCOMPARE pfnCompare );

   void SetColorAttribute( zCPCHAR cpcAttributeName, zSHORT nCol );
   void SetColorAssociation( zLONG lColumn, zCPCHAR cpcValue,
                             zLONG lClrText, zLONG lClrBack, zLONG lFlags );
   void   RemoveColorAssociations( );
   void   SortByColumn( zSHORT nColumn, zSHORT nAscending );
   zLONG  GetSortOrderForKey( zLONG lKey );
// void   SortByDisplayData( );
   void   SetDisplaySelSet( zSHORT nSelSet );
   void   SetSelectSelSet( zSHORT nSelSet );
   void   SetSelectedPosition( zLONG lDisplayPos );

///////////////////////////////////////////////////////////////////////////////////
// Printing.
///////////////////////////////////////////////////////////////////////////////////
#if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
public:

   virtual zBOOL  PrintZCtrl( zBOOL bTest, zCPCHAR cpcTitle = "",
                              zLONG lFlags = 0,
                              zSHORT nFontSize = 80,
                              zCPCHAR cpcFontFace = "Arial" );
   virtual zSHORT PrintZPage( zLONG  lPageNbr,
                              zLONG  lPageWidth,
                              zLONG  lPageHeight );

   void Print( CPrintDialog *pPrntDialog = 0 );

   // EFW - New printing support functions
   void EnableWysiwygPrinting( zBOOL bEnable = TRUE ) { m_bWysiwygPrinting = bEnable; }
   zBOOL GetWysiwygPrinting( )                        { return( m_bWysiwygPrinting ); }

   void SetShadedPrintOut( zBOOL bEnable = TRUE )     { m_bShadedPrintOut = bEnable; }
   zBOOL GetShadedPrintOut( )                         { return( m_bShadedPrintOut ); }

   // Use -1 to have it keep the existing value
   void SetPrintMarginInfo( int nHeaderHeight, int nFooterHeight,
                            int nLeftMargin, int nRightMargin, int nTopMargin,
                            int nBottomMargin, int nGap );

   void GetPrintMarginInfo( int &nHeaderHeight, int &nFooterHeight,
                            int &nLeftMargin, int &nRightMargin, int &nTopMargin,
                            int &nBottomMargin, int &nGap );

///////////////////////////////////////////////////////////////////////////////////
// Printing overrides for derived classes.
///////////////////////////////////////////////////////////////////////////////////
public:
   virtual void OnBeginPrinting( CDC *pDC, CPrintInfo *pInfo );
   virtual void OnPrint( CDC *pDC, CPrintInfo *pInfo );
   virtual void OnEndPrinting( CDC *pDC, CPrintInfo *pInfo );

#endif // #if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )

// Implementation
public:
   LRESULT SendMessageToParent( long lRow, int nCol, int nMessage );
   LRESULT SendDisplayRequestToParent( GV_DISPINFO *pDisplayInfo ) const;
   LRESULT SendCacheHintToParent( const ZCellRange& range ) const;

protected:
   BOOL  RegisterWindowClass( );
   zBOOL Initialize( );
   void  SetupDefaultCells( );

   zBOOL InvalidateCellRect( const long lRow, const int nCol );
   zBOOL InvalidateCellRect( const ZCellId& cell );
   zBOOL InvalidateCellRect( const ZCellRange& cellRange );
   void  EraseBkgnd( CDC *pDC );

   zBOOL GetCellRangeRect( const ZCellRange& cellRange, CRect& rect );

   zBOOL SetCell( long lRow, int nCol, ZGridCellBase *pCell );

   int   SetMouseMode( int nMode )
   {
      int nOldMode = m_MouseMode;
      m_MouseMode = nMode;
      return( nOldMode );
   }

   int   GetMouseMode( ) const { return( m_MouseMode ); }

   zBOOL MouseOverRowResizeArea( CPoint pt );
   zBOOL MouseOverColumnResizeArea( CPoint pt );

   ZCellId GetTopLeftNonFixedCell( zBOOL bForceRecalculation = FALSE );
   ZCellRange GetUnobstructedNonFixedCellRange( zBOOL bForceRecalc = FALSE );
   ZCellRange GetVisibleNonFixedCellRange( LPRECT pRect = NULL,
                                           zBOOL bForceRecalculation = FALSE );

   zBOOL IsVisibleVScroll( ) { return( (m_nBarState & GVL_VERT) > 0 ); }
   zBOOL IsVisibleHScroll( ) { return( (m_nBarState & GVL_HORZ) > 0 ); }
   void  ResetSelectedRange( );
   void  ResetScrollBars( );
   void  EnableScrollBars( int nBar, zBOOL bEnable = TRUE );
   int   GetScrollPos32( int nBar, BOOL bGetTrackPos = FALSE );
   BOOL  SetScrollPos32( int nBar, int nPos, BOOL bRedraw = TRUE );

   zBOOL SortTextItems( int nCol, zBOOL bAscending, int low, int high );
   zBOOL SortItems( PFNLVCOMPARE pfnCompare, int nCol, zBOOL bAscending,
                   LPARAM data, long lLow, long lHigh );

   CPoint GetPointClicked( long lRow, int nCol, const CPoint& pt );

   void ValidateAndModifyCellContents( long lRow, int nCol, zCPCHAR cpcText );
   void DisplayCellData( zCPCHAR cpcText );

// Overrrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL( ZGrid )
    protected:
    virtual void PreSubclassWindow( );
    //}}AFX_VIRTUAL

protected:
#if !defined( _WIN32_WCE_NO_PRINTING ) && !defined( GRIDCONTROL_NO_PRINTING )
    // Printing
   virtual void PrintFixedRowCells( int nStartColumn, int nStopColumn,
                                    long& lRow, CRect& rect, CDC *pDC,
                                    zBOOL& bFirst );
   virtual void PrintColumnHeadings( CDC *pDC, CPrintInfo *pInfo );
   virtual void PrintHeader( CDC *pDC, CPrintInfo *pInfo );
   virtual void PrintFooter( CDC *pDC, CPrintInfo *pInfo );
   virtual void PrintRowButtons( CDC *pDC, CPrintInfo *pInfo );
#endif

#ifndef GRIDCONTROL_NO_DRAGDROP
   // Drag n' drop.
   virtual CImageList * CreateDragImage( CPoint *pHotSpot ); // no longer necessary
#endif

   // Mouse Clicks.
   virtual void  OnFixedColumnClick( ZCellId& cell );
   virtual void  OnFixedRowClick( ZCellId& cell );

   // Editing.
   virtual void  OnEditCell( long lRow, int nCol, CPoint pt, UINT uChar );
   virtual void  OnEndEditCell( long lRow, int nCol, CString cs );
   virtual zBOOL ValidateEdit( long lRow, int nCol, zCPCHAR cpcText );
   virtual void  EndEditing( BOOL bDestroy = TRUE );

   // Drawing.
   virtual void  OnDraw( CDC *pDC );

   // ZGridCellBase Creation and Cleanup.
   virtual void InitCell( ZGridCellBase *pCell, long lRow, int nCol );
   virtual ZGridCellBase * CreateCell( long lRow, int nCol );
   virtual void DestroyCell( long lRow, int nCol );

// Attributes
protected:
   // General attributes.
   COLORREF    m_clrFixedText;
   COLORREF    m_clrFixedBk;
   COLORREF    m_clrGridBk;
   COLORREF    m_clrGridLine;
   COLORREF    m_clrWindowText;     // System colors
   COLORREF    m_clrWindow;
   COLORREF    m_clr3DFace;
   COLORREF    m_clrShadow;
   COLORREF    m_clrTTipBack;       // Titletip colors - FNA
   COLORREF    m_clrTTipText;

   zBOOL       m_bVirtualMode;
   LPARAM      m_lParam;            // lParam for callback
   GRIDCALLBACK m_pfnCallback;      // The callback function

   int         m_nGridLines;
   zBOOL       m_bEditable;
   zBOOL       m_bModified;
   zBOOL       m_bAllowDragAndDrop;
   zBOOL       m_bListMode;
   zBOOL       m_bSingleRowSelection;
   zBOOL       m_bSingleColSelection;
   zBOOL       m_bAllowDraw;
   zBOOL       m_bEnableSelection;
   zBOOL       m_bFixedRowSelection;
   zBOOL       m_bFixedColumnSelection;
   zBOOL       m_bSortOnClick;
   zBOOL       m_bHandleTabKey;
   zBOOL       m_bDoubleBuffer;
   zBOOL       m_bTitleTips;
   int         m_nBarState;
   zBOOL       m_bWysiwygPrinting;
   zBOOL       m_bHiddenColUnhide;
   zBOOL       m_bHiddenRowUnhide;
   zBOOL       m_bAllowColHide;
   zBOOL       m_bAllowRowHide;
   zBOOL       m_bAutoSizeSkipColHdr;
   zBOOL       m_bTrackFocusCell;
   zBOOL       m_bFrameFocus;
   UINT        m_nAutoSizeColumnStyle;
   zULONG      m_ulKeydownMessageTime;
   zULONG      m_ulDblclkTime;
   zCHAR       m_szKeyFind[ 256 ];
   int         m_nKeydownCnt;

   // Cell size details.
   int         m_nFixedRows;
   int         m_nFixedCols;
   long        m_lRows;
   int         m_nCols;
   int         m_nColsMax;
   CUIntArray  m_arRowHeights;
   CUIntArray  m_arColWidths;
   int         m_nVScrollMax;
   int         m_nHScrollMax;
   int         m_nExtraRowsPerItem;
   int         m_nExtraCol;
   int         m_nExtraOffset;

   // Fonts and images.
   CRuntimeClass    *m_pRtcDefault; // determines kind of Grid Cell created by default
   ZGridDefaultCell m_cellDefault;  // "default" cell. Contains default colors, font etc.
   ZGridDefaultCell m_cellFixedColDef;
   ZGridDefaultCell m_cellFixedRowDef;
   ZGridDefaultCell m_cellFixedRowColDef;
   CFont            m_PrinterFont;  // for the printer
   CImageList       *m_pImageList;

   // Report stuff.
   CRect     m_rectDraw;
   CFont     *m_pFontNormal;
   CFont     *m_pFontBold;
   CFont     *m_pFontItalic;
   CFont     *m_pFontUnderline;
   zSHORT    m_nLinesPerPage;
   zSHORT    m_nMaxPage;
   zSHORT    m_nTextHeight;
   zSHORT    m_nTextHeightBold;
   zLONG     m_lPrintFlags;

public:
   CWnd             *m_pZeidonWnd;
   // Row data.
   CArray<zULONG, zULONG> m_EntityKeyList;

   // Column data.
   zLPLB_COL m_pCol;
   ZCellId     m_cellLeftClickDown;
   ZCellId     m_cellIdCurrent;

   // Cell data.
   CTypedPtrArray<CObArray, GRID_ROW *> m_RowData;

protected:
   // Mouse operations such as cell selection.
   int         m_MouseMode;
   zBOOL       m_bLMouseButtonDown;
   zBOOL       m_bRMouseButtonDown;
   CPoint      m_ptLeftClickDown;
   CPoint      m_ptLastMouse;
   ZCellId     m_cellSelectionStart;
   ZCellId     m_cellHold;
   ZCellId     m_cellIdTopLeft;
   int         m_nTimerID;
   int         m_nTimerInterval;
   int         m_nResizeCaptureRange;
   zBOOL       m_bAllowRowResize;
   zBOOL       m_bAllowColumnResize;
   int         m_nRowsPerWheelNotch;

   CMap<LARGE_INTEGER, LARGE_INTEGER, ZCellId, ZCellId&> m_SelectedCellMap;
   CMap<LARGE_INTEGER, LARGE_INTEGER, ZCellId, ZCellId&> m_PrevSelectedCellMap;

#ifndef GRIDCONTROL_NO_TITLETIPS
   ZTitleTip   m_wndTitleTip;            // title tips for cells
#endif

   // Drag and drop.
   ZCellId     m_cellLastDragOver;
#ifndef GRIDCONTROL_NO_DRAGDROP
   ZGridDropTarget m_DropTarget;       // OLE Drop target for the grid
#endif

   // Printing information.
   CSize       m_CharSize;
   int         m_nPageHeight;
   CSize       m_LogicalPageSize;     // page size in gridctrl units
   CSize       m_PaperSize;           // page size in device units

   // Additional properties to support Wysiwyg printing.
   int         m_nPageWidth;
   int         m_nPrintColumn;
   int         m_nCurrPrintRow;
   int         m_nNbrPages;
   int         m_nPageMultiplier;

   // EFW - Added to support shaded/unshaded printout.  If true, colored
   // cells will print as-is.  If false, all text prints as black on white.
   zBOOL       m_bShadedPrintOut;

   // EFW - Added support for user-definable margins.  Top and bottom are in
   // lines.  Left, right, and gap are in characters (avg width is used).
   int         m_nHeaderHeight;
   int         m_nFooterHeight;
   int         m_nLeftMargin;
   int         m_nRightMargin;
   int         m_nTopMargin;
   int         m_nBottomMargin;
   int         m_nGap;

public:
   // sorting
   int         m_nSortColumn;
   PFNLVCOMPARE m_pfnCompare;

   int         m_nColLastSorted;
   zCHAR       m_chAscendDescend;
   zBOOL       m_bAutoSort;
   zBOOL       m_bRememberSort;
   zBOOL       m_bSortDisplay;
   CString     m_csSortBuffer;
   ZNumberList *m_pSortColumnList;

   ZColorList  *m_pColorList;

protected:
   void SelectAllCells( );
   void SelectColumns( ZCellId currentCell, zBOOL bForceRedraw = FALSE,
                       zBOOL bSelectCells = TRUE );
   void SelectRows( ZCellId currentCell, zBOOL bForceRedraw = FALSE,
                    zBOOL bSelectCells = TRUE );
   void SelectCells( ZCellId currentCell, zBOOL bForceRedraw = FALSE,
                     zBOOL bSelectCells = TRUE );
   void OnSelecting( const ZCellId& currentCell );

   // Generated message map functions
   //{{AFX_MSG( ZGrid )
   afx_msg void OnPaint( );
   afx_msg void OnHScroll( UINT uSBCode, UINT uPos, CScrollBar *pScrollBar );
   afx_msg void OnVScroll( UINT uSBCode, UINT uPos, CScrollBar *pScrollBar );
   afx_msg void OnSize( UINT uType, int cx, int cy );
   afx_msg void OnLButtonUp( UINT uFlags, CPoint pt );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnMouseMove( UINT uFlags, CPoint pt );
   afx_msg void OnTimer( UINT uIDEvent );
   afx_msg UINT OnGetDlgCode( );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnChar( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnLButtonDblClk( UINT uFlags, CPoint pt );
   afx_msg BOOL OnEraseBkgnd( CDC *pDC );
   afx_msg void OnSysKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnUpdateEditSelectAll( CCmdUI *pCmdUI );
   //}}AFX_MSG
#ifndef _WIN32_WCE_NO_CURSOR
   afx_msg BOOL OnSetCursor( CWnd *pWnd, UINT uHitTest, UINT message );
#endif
#ifndef _WIN32_WCE
   afx_msg void OnRButtonDown( UINT uFlags, CPoint pt );
   afx_msg void OnRButtonUp( UINT uFlags, CPoint pt );    // EFW - Added
   afx_msg void OnSysColorChange( );
#endif
#ifndef _WIN32_WCE_NO_CURSOR
   afx_msg void OnCaptureChanged( CWnd *pWnd );
#endif
#ifndef GRIDCONTROL_NO_CLIPBOARD
   afx_msg void OnUpdateEditCopy( CCmdUI *pCmdUI );
   afx_msg void OnUpdateEditCut( CCmdUI *pCmdUI );
   afx_msg void OnUpdateEditPaste( CCmdUI *pCmdUI );
   void OnUpdateEditCopyOrCut( CCmdUI *pCmdUI );
#endif
#if ( _MFC_VER >= 0x0421 ) || ( _WIN32_WCE >= 210 )
   afx_msg void OnSettingChange( UINT uFlags, zCPCHAR lpszSection );
#endif
#if !defined( _WIN32_WCE ) && ( _MFC_VER >= 0x0421 )
   afx_msg BOOL OnMouseWheel( UINT uFlags, short nDelta, CPoint pt );
#endif
   afx_msg LRESULT OnSetFont( WPARAM hFont, LPARAM lParam );
   afx_msg LRESULT OnGetFont( WPARAM hFont, LPARAM lParam );
   afx_msg LRESULT OnImeChar( WPARAM wCharCode, LPARAM lParam );
   afx_msg void OnEndInPlaceEdit( NMHDR *pNMHDR, LRESULT *pResult );

   enum eMouseModes
   {
      MOUSE_NOTHING, MOUSE_SELECT_ALL,
      MOUSE_SELECT_COL, MOUSE_SELECT_ROW,
      MOUSE_SELECT_CELLS, MOUSE_SCROLLING_CELLS,
      MOUSE_OVER_ROW_DIVIDE, MOUSE_SIZING_ROW,
      MOUSE_OVER_COL_DIVIDE, MOUSE_SIZING_COL,
      MOUSE_PREPARE_EDIT,
#ifndef GRIDCONTROL_NO_DRAGDROP
      MOUSE_PREPARE_DRAG, MOUSE_DRAGGING
#endif
   };

   DECLARE_MESSAGE_MAP( )
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// InPlaceEdit.h : header file
//
// MFC Grid Control - inplace editing class
//
// Written by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2002. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is
// not sold for profit without the authors written consent, and
// providing that this notice and the authors name and all copyright
// notices remains intact.
//
// An email letting me know how you are using it would be nice as well.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with ZGrid v2.10+
//
/////////////////////////////////////////////////////////////////////////////

class CInPlaceEdit : public CEdit
{
// Construction
public:
   CInPlaceEdit( ZGrid *pGrid, CRect& rect, DWORD dwStyle, UINT uId,
                 long lRow, int nCol, CString csInitText, UINT uFirstChar );

// Attributes
public:

// Operations
public:
   virtual void EndEdit( BOOL bDestroy = TRUE );

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( CInPlaceEdit )
public:
   virtual BOOL PreTranslateMessage( MSG *pMsg );
protected:
   virtual void PostNcDestroy( );
   //}}AFX_VIRTUAL

// Implementation
public:
   virtual ~CInPlaceEdit( );

// Generated message map functions
protected:
   //{{AFX_MSG( CInPlaceEdit )
   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg void OnKillFocus( CWnd *pNewWnd );
   afx_msg void OnChar( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uChar, UINT uRepCnt, UINT uFlags );
   afx_msg UINT OnGetDlgCode( );
   afx_msg void OnLButtonDown( UINT uFlags, CPoint pt );
   //}}AFX_MSG
   DECLARE_MESSAGE_MAP( )

private:
   ZGrid   *m_pGrid;      // Parent grid control
   long    m_lRow;
   int     m_nCol;
   CString m_csInitText;
   UINT    m_nLastChar;
   zBOOL   m_bExitOnArrows;
   CRect   m_rect;
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CMemDC - memory DC
//
// Author: Keith Rule
// Email:  keithr@europa.com
// Copyright 1996-1997, Keith Rule
//
// You may freely use or modify this code provided this
// Copyright is included in all derived versions.
//
// History - 10/3/97 Fixed scrolling bug.
//                   Added print support.
//           25 feb 98 - fixed minor assertion bug
//
// This class implements a memory Device Context

class CMemDC : public CDC
{
public:

   // constructor sets up the memory DC
   CMemDC( CDC *pDC ) : CDC( )
   {
      ASSERT( pDC );

      m_pDC = pDC;
      m_pOldBitmap = 0;
#ifndef _WIN32_WCE_NO_PRINTING
      m_bMemDC = !pDC->IsPrinting( );
#else
      m_bMemDC = FALSE;
#endif

      if ( m_bMemDC )    // Create a Memory DC
      {
         pDC->GetClipBox( &m_rect );
         CreateCompatibleDC( pDC );
         m_bitmap.CreateCompatibleBitmap( pDC, m_rect.Width( ), m_rect.Height( ) );
         m_pOldBitmap = SelectObject( &m_bitmap );
#ifndef _WIN32_WCE
         SetWindowOrg( m_rect.left, m_rect.top );
#endif
         // EFW - Bug fix - Fill background in case the user has overridden
         // WM_ERASEBKGND.  We end up with garbage otherwise.
         // CJM - moved to fix a bug in the fix.
         FillSolidRect( m_rect, pDC->GetBkColor( ) );
      }
      else        // Make a copy of the relevent parts of the current DC for printing
      {
#if !defined( _WIN32_WCE ) || ((_WIN32_WCE > 201) && !defined( _WIN32_WCE_NO_PRINTING ))
         m_bPrinting = pDC->m_bPrinting;
#endif
         m_hDC       = pDC->m_hDC;
         m_hAttribDC = pDC->m_hAttribDC;
      }
   }

   // Destructor copies the contents of the mem DC to the original DC
   ~CMemDC( )
   {
      if ( m_bMemDC )
      {
         // Copy the offscreen bitmap onto the screen.
         m_pDC->BitBlt( m_rect.left, m_rect.top,
                        m_rect.Width( ), m_rect.Height( ),
                        this, m_rect.left, m_rect.top, SRCCOPY );

         // Swap back the original bitmap.
         SelectObject( m_pOldBitmap );
      }
      else
      {
         // All we need to do is replace the DC with an illegal value,
         // this keeps us from accidently deleting the handles associated with
         // the CDC that was passed to the constructor.
         m_hDC = m_hAttribDC = 0;
      }
   }

   // Allow usage as a pointer
   CMemDC *operator->( ) { return( this ); }

   // Allow usage as a pointer
   operator CMemDC*( ) { return( this ); }

private:
   CBitmap  m_bitmap;      // Offscreen bitmap
   CBitmap  *m_pOldBitmap; // bitmap originally found in CMemDC
   CDC      *m_pDC;        // Saves CDC passed in constructor
   CRect    m_rect;        // Rectangle of drawing area.
   zBOOL    m_bMemDC;      // TRUE if CDC really is a Memory DC.
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#endif   // __zdctlss_h__  sentry.
