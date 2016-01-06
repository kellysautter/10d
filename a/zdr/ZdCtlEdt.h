/////////////////////////////////////////////////////////////////////////////
// Project ZdCtl
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2007 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdctl.dll - ZDr Application
// FILE:         zdctledt.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdctledt_h__  // Sentry, use file only if it's not already included
#define __zdctledt_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

extern "C"
{
AFX_EXT_API
CWnd * OPERATION
Edit( ZSubtask *pZSubtask,
      CWnd     *pWndParent,
      ZMapAct  *pzmaComposite,
      zVIEW    vDialog,
      zSHORT   nOffsetX,
      zSHORT   nOffsetY,
      zKZWDLGXO_Ctrl_DEF *pCtrlDef );
}

// Zeidon ZEdit standard control
class ZDCTL_CLASS ZEdit : public CEdit, public ZMapAct
{
   DECLARE_DYNAMIC( ZEdit )
public:
   // ctor
   ZEdit( ZSubtask *pZSubtask,
          CWnd     *pWndParent,
          ZMapAct  *pzmaComposite,
          zVIEW    vDialog,
          zSHORT   nOffsetX,
          zSHORT   nOffsetY,
          zKZWDLGXO_Ctrl_DEF *pCtrlDef );

   // dtor
   virtual ~ZEdit( );

   virtual void   CreateZ( );
   virtual zSHORT MapFromOI( WPARAM wFlag = 0 ); // transfer data (OI to ctrl)
   virtual zSHORT MapToOI( zLONG lFlag = 0 );
   virtual zLONG  SetZCtrlState( zLONG lStatusType, zULONG ulValue );
   virtual zSHORT SetCtrlMapping( zCPCHAR cpcViewName,
                                  zCPCHAR cpcEntityName,
                                  zCPCHAR cpcAttributeName,
                                  zCPCHAR cpcContextName,
                                  zLONG   lColumn = -1 );
   virtual zBOOL  HandleQuickKey( zACCEL_DEF ad );

   virtual zLONG  SelectAll( );
   zSHORT SelectText( zSHORT nTextStartPos, zSHORT nTextEndPos );
   zSHORT LimitTextLength( zLONG lTextLth );
// zSHORT SetRange( zDECIMAL dxMin, zDECIMAL dxMax );
   void   CWndSetWindowText( zCPCHAR cpcText );

   virtual BOOL DestroyWindow( );

protected:
   // Child id notifications handled at the child.
   void ENChange( );
   void ENErrspace( );
   void ENHscroll( );
   void ENMaxtext( );
   void ENSetFocus( );
   void ENKillFocus( );
   void ENUpdate( );
   void ENVscroll( );

   afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnMouseMove( UINT uModKeys, CPoint pt );
   afx_msg void OnNcPaint( );
   afx_msg void OnPaint( );

   afx_msg void OnUpdate( );
   afx_msg void OnSysKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnSysKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyDown( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnKeyUp( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnChar( UINT uKey, UINT uRepeatCnt, UINT uFlags );
   afx_msg void OnLButtonUp( UINT uModKeys, CPoint pt );
   afx_msg void OnSize( UINT nType, int cx, int cy );
   afx_msg void OnWindowPosChanged( WINDOWPOS *lpWndPos );
   virtual BOOL PreTranslateMessage( MSG *pMsg );

   zBOOL   IsFormatPicChar( zLONG lPos );
   zLONG   GoToNextEntryChar( zLONG lPos );
   zSHORT  GetReplacementChar( zLONG lPos, zCHAR cWant, char& cGet );

public:
   virtual zSHORT SetRowColText( zCPCHAR cpcText,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   void    SetText( zPCHAR pchText, zLONG lMaxLth );
   virtual zSHORT GetRowColText( zPCHAR pchText, zUSHORT uLth,
                                 zLONG lRow = 0, zLONG lColumn = 0 );
   zLONG   GetText( zPCHAR pchText, zLONG lMaxLth ) const;

   void    SetFormatPic( zCPCHAR cpcFormatPic, zCPCHAR cpcOverride,
                        zCHAR cPlaceHolder = '_' );
   CString GetFormatPic( );
   int     ResetStartChar( CString& csText, int nStart, int nEnd );
   zCHAR   m_chPlaceHolder;

public:
   zULONG  m_ulLastKey;

private:
   CBrush  *m_hBrushSystem;
   zULONG  m_ulLth;
   zCHAR   m_chPic;
   zCHAR   m_chPassword;
   zULONG  m_ulSubtype;

   zPCHAR  m_pchBlobInfo;
   zBOOL   m_bStripSpecial;
   zLONG   m_lSpecialCnt;
   zLONG   m_lFormatPicLth;
   zCPCHAR m_cpcFormatPic;
   zCPCHAR m_cpcOverride;
   CString m_csSaveText;
   zBOOL   m_bLastKeyWasDelete;
   zBOOL   m_bLastKeyWasBackspace;
   zBOOL   m_bFixedFont;

   DECLARE_MESSAGE_MAP( )
};

#if 1
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//           Abstract (Pure Virtual) Class:  ZMFE
//
//                    MFE (Multi-Field Edit) Control
//
//                    Version: 2.0    Date:  04/16/00
//
//
//                Developed By: George Chastain,
//                              Huntsville, Alabama.
//                              e-mail: gcoutpost@hotmail.com
//
// OVERVIEW:
//
// This abstract class provides a base class from which to derive a CEdit
// based control class that provides functionality for what I refer to as
// the MFE Control.  An MFE Control is a single Edit Control that contains
// fields of data that behave as though they were separate controls.  A
// popular example is the new IP Address Control available with the
// Microsoft(R) Visual C++ Version 6.0 product.
//
// The ZMFE class only provides the basic functionality required to provide
// the Multi-Field Edit Control behavior.
//
// This class allows the developer to derive a class that may do the following:
//
// -  Specify as many fields as desired.
// -  Specify a single field separator
// -  Specify a different field separator for each field
// -  Specify field separators of as many characters in length as desired.
// -  Specify which field is selected.
// -  Ask the base class which class is currently selected.
// -  Allow selection of a field by using the left & right cursor control
//    keys (arrow keys) or the Space Bar.
// -  Allow the selection of a field by clicking over the field using the
//    left mouse button.
// -  Update the contents of the fields at any time.
// -  Reformat the field at any time.  That is, the derived class may change
//    the number of fields, the width of the fields and even the field
//    separators.
//
// This class makes no assumptions as to how data WITHIN the fields is to be
// displayed.  Since this class is designed to be a generic, abstract class
// from which specific control classes are to be derived, there is no way of
// determining what data will be presented within the fields.  Nor should this
// class make any assumptions as to how the control is to behave regarding the
// selection of fields.
//
// Therefore, the developer is responsible for ensuring that the data to be
// displayed in each field is correct when passing it to this class's methods
// for presentation to the operator.  This class is designed to take CString
// items for each field to allow the derived class to pass anyinformation to
// the ZMFE methods for display in its fields.  It also accepts CString items
// for the field separators so that any character or string may be used as
// the field separators.
//
// I decided to use the ISO Standard C++ Standard Template Library (STL)
// vector container class for passing field contents and field separators to
// this class.  These just provide neat, self-contained -- well,
// containers -- for storing and manipulating arrays.  Without having to
// worry about the drudgery of coding up the allocation and deallocation of
// conventional arrays.
//
//
//                MFE (Multi-Field Edit) Control Class Hierarchy
//
//                                 CEdit
//                                   |
//                              CGCColorEdit
//                                   |
//                            ZMFE (abstract)
//                                   |
//                         Your Multi-Field Edit Class
//
// SUPPORT CLASSES:
//
// This file also defines some support classes (NOT part of the
// ZMFE class) that you may use to help implement
// numeric fields.  Please refer to the descriptions accompanying
// the declarations at the bottom of this header file.
//
// The ZMFE class is derived from my CGCColorEdit class
// to provide the ability to set the background and text colors and
// the font in use.  Multi-Field Edit Controls look better when using
// them if they are set to use a non-proportional, or fixed pitch, font.
// The CGCColorEdit class provides that capability.
//
//
// HOW TO USE:
//
// 1. Include CGCColorEdit class in your project (if desired).
// 2. Include ZMFE class in your project.
// 3. Derive your new class from this class.
//    Use the support classes as needed to help you implement your derived
//    class.
//
// REVISION HISTORY:
//
// VERSION    DATE     DEVELOPER
//   1.0    02/15/00   Chastain
// Created
//
//   2.0    04/16/00   Chastain
// Added CGCFieldBase class from which I derived all field helper classes.
// Added CGCTextBase class from which I derived CGCTextField and
// CGCStringListField helper classes.  Modified CGCTextField and
// CGCStringListField to take advantage of CGCTextBase.  Added CGCNumBase
// class from which I derived CGCIntegerField and CGCDoubleField.  Changed
// return type of Construct (not to be confused with the constructor) for
// each helper class to be consistently of type BOOL instead of returning the
// new value of the data represented by the field object.  Changed the name
// of the construct method of CGCStringListField from ConstructInputAndSearch
// to Construct for consistency.  Added support functions to check class of
// field object pointer.  Added Reset method override to all support classes.
// Added macros to perform dynamic casting of pointer to field helper class
// objects.  Replaced pointers for member variables in CGCStringListField
// with statically declared variables for simplification.  Now require /GR
// compile option to support RTTI.  Added fixes to CGCDoubleField to help
// ensure that only the specified precision is maintained and that
// "double-precision garbage" isn't kept around.  Added operator overrides
// to field support classes.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#include <vector>
using namespace std;

// A single, convenient location to allow you to change the parent class.  If
// you do not want to use the CGCColorEdit, or use a different parent class,
// just change this define and the parent will change to what you define.
// If you do not want to use the CGCColorEdit class, remember to remove the
// include statement below and change the definition of _PARENT_CLASS to
// "CEdit" or whatever you will use.
//
class CGCColorEdit;
#define _PARENT_CLASS   CGCColorEdit
//#include "GCColorEdit.h"      // So you can set colors, fonts.

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//                             CGCColorEdit
//
//                           Color Edit Control
//
//                     Version 1.2      Date: 04/19/00
//
//                Developed By:  George Chastain
//                               Software Consultant
//                               e-mail: gcoutpost@hotmail.com
//
//
//
// DESCRIPTION:
// Use this class to create a member variable for a CEdit control to
// allow you to dynamically change the background and text colors for the
// CEdit box.  This class also provides the ability to set the background
// of the edit box to a specified pattern.
//
// Refer to the comments associated with the class methods below.
//
//
// Revision History:
//
// VERSION     DEVELOPER      CHANGE
// 1.0         Chastain    Created
// 1.1         Chastain    Removed some old obsolete code from
//                         SetBackgroundColor() and SetTextColor()
//                         that should have been deleted.
//                         Added more comments.
//
// 1.2         Chastain    Added Flat style. (SetFlat() method).
//
//
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// ColorEditClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGCColorEdit window

class CGCColorEdit : public CEdit
{
public:
   CGCColorEdit( );

   // Sets new text color.  Returns previous text color.
   COLORREF SetTextColor( const COLORREF textColor );

   // Sets new background color. Returns previous background color.  If
   // the previous background was a pattern, then the returned color will
   // be black (0).
   COLORREF SetBackgroundColor( const COLORREF bkColor );

   // Sets the background of the edit box to the specified pattern.  The
   // pattern must be a bitmap of 8 pixels high by 8 pixels wide.  If the
   // previous background was a pattern, then the returned color will be
   // black (0).
   COLORREF SetBackgroundPattern( CBitmap *pattern );

   // Returns current text color.
   COLORREF GetTextColor( ) const;

   // Returns the current text color as separate RGB values.
   void GetTextColorRGB( BYTE& Red, BYTE& Green, BYTE& Blue ) const;

   // Returns current background color.
   // If the current background is a pattern, then the returned color will be black (0).
   COLORREF GetBackgroundColor( ) const;

   // Returns current background color as separate RGB values.
   // If the current background is a pattern, then the returned color will be black (0,0,0).
   void GetBackgroundColorRGB( BYTE& Red, BYTE& Green, BYTE& Blue ) const;

   // Changes the edit box to have a static frame style.
   // Extended Style WS_EX_STATICEDGE.
   zBOOL SetStaticFrame( const zBOOL bOn );

   // SetModalFrame() -- Changes the edit box to have a modal frame style.
   // Extended type WS_EX_DLGMODALFRAME.  Modal style overrides static edge style.
   zBOOL SetModalFrame( const zBOOL bOn );

   // SetPointFont() sets the edit control to use the specified font at the specified
   // point size. Examples of how to specify font name: "Comic Sans MS" or "Haettenschweiler".
   // It is up to the developer to create the edit control large-enough to display
   // the largest font required.
   zBOOL SetPointFont( const int PointSize, const zCPCHAR fontname );

   // Sets the edit control to use the specified font.
   // For a description of how to use the input parameter, refer to the MFC documentation
   // for CFont::CreateFontIndirect( );
   zBOOL SetFontIndirect( const LOGFONT *lpLogFont );

   // Sets the edit control to use the specified font.
   // For a description of how to use the input parameter, refer to the MFC documentation
   // for CFont::CreatePointFontIndirect( );
   zBOOL SetPointFontIndirect( const LOGFONT *lpLogFont );

   // Allows you to change the font weight between 400 (normal) and 800 (bold).
   // Returns the previous state of the bold attribute.
   zBOOL SetBold( const zBOOL bOn = TRUE );

   // Provides more control over the font weight that SetBold().
   // The effect dependes on the font in use.
   void SetFontWeight( const int Weight );

   // Sets the text in the edit control to italics or turns it off.
   // Returns the previous state of the italic attribute.
   zBOOL SetItalic( const zBOOL bOn = TRUE );

   // Sets/removes underlining from the text.
   // Returns the previous state of the underline attribute.
   zBOOL SetUnderline( const zBOOL bOn = TRUE );

   // Returns the logical font in use in a LOGFONT structure.
   LOGFONT GetLogFont( ) const;

   zBOOL SetFlat( const zBOOL bFlat );

   // Retrieves the current font name.
   void GetFontName( CString& rcsFontName ) const;

   virtual ~CGCColorEdit( );

   // By defining constants such as these in this manner instead of using
   // #defines outside the class definition, the names will not conflict
   // with other #defines in the clients code.  A client can still reference
   // these if needed as in this example: CGCColorEdit::DEFAULT_BACKGROUND_COLOR.
   enum { DEFAULT_BACKGROUND_COLOR = RGB( 255, 255, 255 ),
          DEFAULT_TEXT_COLOR = RGB( 0, 0, 0 ) };

public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL(CGCColorEdit)
   //}}AFX_VIRTUAL

protected:
   //{{AFX_MSG(CGCColorEdit)
   afx_msg HBRUSH CtlColor( CDC *pDC, UINT nCtlColor );
   afx_msg void OnNcPaint( );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP()
private:
   zBOOL m_bFlat;
   zBOOL m_bModal;             // Indicates if modal frame is set.
   zBOOL m_bStatic;               // Indicates if static frame is set. Modal frame overrides.
   zBOOL m_bBold;              // Indicates bold face.
   zBOOL m_bItalic;               // Indicates Italic.
   zBOOL m_bUnderline;            // Indicates underline.
   CFont m_Font;              // Current client-created font object.

   COLORREF m_CurrentBackColor;  // Current background color.
   COLORREF m_CurrentTextColor;  // Current text color.
   CBrush m_BackBrush;           // Current background brush.
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////


// The following defines allow you to elmininate support classes that you do
// not need so that you can keep your code as compact as possible.  Comment
// out the defines for the Support Classes that you do not need.  For example,
// if you will not be deriving a Multi-Field Edit Control that uses a floating
// point field, you can comment out the define statement for GCMFESUPP_DBL and
// the support class CGCDoubleField will not be compiled into the code.

#define GCMFESUPP_INT         // Defines the CGCIntegerField Support Class.
#define GCMFESUPP_DBL         // Defines the CGCDoubleField Support Class.
#define GCMFESUPP_TXT         // Defines the CGCTextField Support Class.
#define GCMFESUPP_TLS         // Defines the CGCStringListField Support Class.

class ZMFE : public _PARENT_CLASS
{

public:

// Overrides
   // ClassWizard generated virtual function overrides
   //{{AFX_VIRTUAL( ZMFE )
   //}}AFX_VIRTUAL

public:
   ZMFE( );

   // I made the destructor PURE virtual to ensure that ZMFE is
   // an abstract class.  It doesn't make sense to create an object of
   // ZMFE class because it doesn't do anything.  The client
   // is expected to use this class to create his/her own specialized
   // Multi-Field Edit Control.
   virtual ~ZMFE( ) = 0;

   // Used to define the number of fields in the control, as well as the width
   // of each field.  Further, it is used to define the field separators.
   //
   // Parameters:
   //
   // FieldContents   - Contains the initial contents of each field.
   //                   The number of fields for the Multi-Field Edit Control
   //                   is determined by the number of items in this vector.
   //
   //                   The width of each field in the Multi-Field Edit Control
   //                   is determined from the length of each string in the
   //                   FieldContents vector.  The width of the fields are
   //                   fixed at this width until Format( ) is called again.
   //
   // FieldSeparators - Contains the string( s ) to be used for the field
   //                   separators. This vector may contain 1 string of
   //                   length >= 1 character( s ). If only one separator
   //                   is specified, that will be used to separate all fields.
   //                   If more than one is specified then the vector must
   //                   contain one less than the number of items contained
   //                   by the FieldContents vector.  Each separator is used
   //                   in the order specified to separate the fields starting
   //                   from the left side of the control.  If this condition
   //                   is not met, the function fails and returns FALSE.
   //
   // Prefix          - In addition to allowing the developer to specify
   //                   different characters or strings for the field
   //                   separators, you may also elect to have a special
   //                   character or string displayed in the control at the
   //                   beginning of the control, before the the first field.
   //                   This "Prefix", like the field separators, is not
   //                   selectable.  This allows you greater flexibility in
   //                   controlling the content/format of the Multi-Field
   //                   Edit Control.  The default is not to use one.
   //
   // Postfix         - Like the Prefix, you may specify a non-selectible
   //                   character or string to terminate the contents of the
   //                   Multi-Field Edit Control.  Like the Prefix and the
   //                   field separators, the Postfix is not selectable.
   //                   The default is not to use one.
   //
   // The Prefix and Postfix parameters are not typically used in Multi-Field
   // Edit Controls.  However, they are provided to allow you to customize
   // the edit control beyond the use of just the field separators.


   // Used to programmatically select a field in the control.  Returns TRUE
   // if successful.  Parameter FieldNumber specifies a 0-based index
   // identifying the field to select starting from the left side of the
   // control.
   zBOOL SelectField( const zSHORT FieldNumber );

   // Returns TRUE if successful.
   zBOOL Format( const vector<CString> FieldContents,   // Contents of each field
                 const vector<CString> FieldSeparators, // Field separators
                 const CString        Prefix = "",      // Prefix
                 const CString        Postfix = "" );   // Postfix
   // Used to obtain the currently selected field.  Returns a 0-based value
   // starting from the left side of the control.
   zSHORT GetSelectedField( ) const;

   // Used to update the contents of each field.   This does NOT change the
   // width of the fields.  If the derived class calls this method with fields
   // that are longer than those defined by a call to Format( ), the contents
   // of those longer fields will be truncated on the right.  Since the
   // Multi-Field Edit Control is designed for controls that are expected to
   // have unchanging field lengths, the developer should ensure that
   // the strings contained in the parameter passed to the Update( ) methode
   // are of the same length as those specified in the original set passed to
   // Format( ).  Otherwise, the developer should call Format( ) if the width
   // of any field is to change dynamically.
   // Returns TRUE if successful.
   zBOOL Update( const vector<CString>& FieldContents );

   // This override does nothing.  Disabled intentionally.  Calling this
   // override of the parent class's SetWindowText( ) has no effect.  The
   // client must call Format( ) or Update( ) to change the contents of
   // the control.
   // I wish that Microsoft would either make it easy to add new types for
   // custom ddx routines (custom entries into the .CLW file) so that it
   // would be easy to inherit from CEdit privately, or that ISO/ANSI would
   // modify the C++ language definition to allow for the hiding of specific
   // methods inherited from parent classes.  Actually, as long as I'm
   // fantasizing, I want both. :-)
   void SetWindowText( zCPCHAR lcpcString );

protected:

   //{{AFX_MSG( ZMFE )
   afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
   afx_msg void OnSetFocus( CWnd *pOldWnd );
   afx_msg void OnLButtonDown( UINT nFlags, CPoint pt );
   afx_msg void OnNcPaint( );
   afx_msg void OnMouseMove( UINT nFlags, CPoint pt );
   afx_msg void OnLButtonDblClk( UINT nFlags, CPoint pt );
   afx_msg void OnChar( UINT nChar, UINT nRepCnt, UINT nFlags );
   afx_msg void OnRButtonUp( UINT nFlags, CPoint pt );
   afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
   //}}AFX_MSG

   DECLARE_MESSAGE_MAP( )
private:

   zBOOL m_bValid; // Used to determine if the control has been initialized
                   // by a call to Format( ).  The control can not be used
                   // until Format( ) is called.

   // Record type FIELD_SPEC_TYPE is used to store the field starting and
   // ending positions within the edit box.
   typedef struct
   {
      zSHORT start;
      zSHORT stop;

   } FIELD_SPEC_TYPE;


   FIELD_SPEC_TYPE *m_FieldSpecs;      // Array of field start/end positions.

   vector<CString> m_FieldSeparators;  // Internal array of field separators.
   vector<zSHORT>  m_FieldWidths;      // Internal array of field widths.

   vector<CString> m_FieldContents; // The current contents of each field.
                              // When examining the cpp file, you may notice
                              // that it looks as though m_FieldContents
                              // could be eliminated.  The main reason that an
                              // internal copy of this data is maintained is
                              // for the proper function of the OnNcPaint( )
                              // handler.  See the comments in the cpp file
                              // for that handler for further details.

   CString m_Prefix;          // Stores the specified Prefix.
   CString m_Postfix;         // Stores the specified Postfix.


   zSHORT m_NumberOfFields;   // The number of fields within the control.
   zSHORT m_SelectedField;    // Which track is currently selected?

   void FormatAndDisplay( );  // Private method to display the data
                              // within the Multi-Field Edit box.
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//                                  SUPPORT CLASSES
//
// The following support classes are provided to make it easier for you
// to implement a Multi-Field Edit Controls derived from ZMFE.
//
// The class hierarchy for the helper classes is as follows:
//
//                                CGCFieldBase (abstract)
//                                      |
//                  ------------------------------------------
//                  |                                        |
//             CGCNumBase (abstract)                  CGCTextBase (abstract)
//                   |                                       |
//       ------------------------               -----------------------
//       |                      |               |                     |
//  CGCIntegerField      CGCDoubleField   CGCTextField     CGCStringListField
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#ifdef GCMFESUPP_INT       // If we are to compile CGCIntegerField class
   #define GCMFESUPPBASE      //    compile CGCFieldBase too
   #define GCMFESUPPNBASE     //    compile CGCNumBase too.
#endif
#ifdef GCMFESUPP_DBL       // If we are to compile CGCDoubleField class
   #define GCMFESUPPBASE      //    compile CGCFieldBase too.
   #define GCMFESUPPNBASE     //    compile CGCNumBase too.
#endif
#ifdef GCMFESUPP_TXT       // If we are to compile CGCTextField class
   #define GCMFESUPPBASE      //    compile CGCFieldBase too
   #define GCMFESUPPTXTBASE   //    compile CGCTextBase too
#endif
#ifdef GCMFESUPP_TLS       // If we are to compile CGCStringListField class
   #define GCMFESUPPBASE      //    compile CGCFieldBase too
   #define GCMFESUPPTXTBASE   //    compile CGCTextBase too
#endif


#ifdef GCMFESUPPBASE

// Abstract (pure virtual) class used as parent for all field helper
// classes.  This class is provided for convenience for some operations.
//
// This class simply provides a common way in which to manage the different
// types of fields that a Multi-Field Edit Control is likely to have.
// I typically do not use this class but some people may find it useful in
// the future.
class CGCFieldBase
{
public:
   CGCFieldBase( ) {};
   virtual ~CGCFieldBase( ) {};
   virtual zBOOL Construct( const UINT nChar ) = 0 { return( FALSE ); }
   virtual void Reset( ) = 0 {};
};

   // Returns TRUE if ptr ==> to a CGCFieldBase class object.
   zBOOL IsField( CGCFieldBase *ptr );

#endif

#ifdef GCMFESUPPNBASE

// Abstract (pure virtual) base class for the numerical field support
// classes CGCIntegerField and CGCDoubleField.
class CGCNumBase : public CGCFieldBase
{
public:
   CGCNumBase( ) {}
   virtual ~CGCNumBase( ) {}
   // See comments for the declarations of CGCIntegerField and CGCDoubleField
   // for details on the overrides for these pure virtual functions.
   virtual void Increment( ) = 0 {}
   virtual void Decrement( ) = 0 {}
   virtual zBOOL operator> (const double& Val) const = 0 { return( TRUE );}
   virtual zBOOL operator< (const double& Val) const = 0 { return( TRUE );}
   virtual zBOOL operator>=(const double& Val) const = 0 { return( TRUE );}
   virtual zBOOL operator<=(const double& Val) const = 0 { return( TRUE );}
   virtual zBOOL operator==(const double& Val) const = 0 { return( TRUE );}
   virtual double GetValue( ) const = 0;
};

   zBOOL IsNumField( CGCNumBase *ptr );

#endif

#ifdef GCMFESUPP_INT

// CGCIntegerField
//
// You should use this class when implementing an integer field in the class
// that you derive from ZMFE.  It will help process key strokes
// in your OnChar( ) and OnKeyDown( ) overrides.
//
//                               HOW TO USE
//
// In your derived class, create a member variable of class CGCIntegerField.
// Then use the Increment( ) and Decrement( ) methods in your OnKeyDown( )
// handler for the up/down cursor control ( arrow ) keys if you wish to
// handle these keys in your derived class.  When implementing your OnChar( )
// override, call the Construct( ) method, passing it the value of nChar that
// is input to the OnChar( ) handler.  The CGCIntegerField class will handle
// the input appropriately to modify the value associated with the integer
// field.  Call GetValue( ) as needed to obtain the current value represented
// by the CGCIntegerField class object.
//
// You will need to create a CGCIntegerField object for each Integer
// (or long) Field in your Multi-Field Edit Control.

class CGCIntegerField : public CGCNumBase
{
public:
   // Constructor to be used when creating a CGCIntegerField field support
   // object.
   CGCIntegerField( zLONG lMinValue,   // Minimum allowed value for integer field.
                    zLONG lMaxValue,   // Maximum allowed value for integer field.
                    zLONG lInterval ); // Interval to be used by the parameterless
                                       // versions of Increment( ) and Decrement( ).

   CGCIntegerField( );       // Default constructor.  Not intended for
                             // client use.  It is supplied to support
                             // auto constructions when declaring arrays, etc.

   ~CGCIntegerField( ) {};

   // Useful for VK_UP handler in the OnKeyDown( ) override of your derived
   // class.  Increments the integer value by the number specified in the
   // Interval parameter of this class's constructor.
   void Increment( );

   // Increments the integer value by the number specified in the Interval
   // parameter to this method.  The Interval parameter passed to this
   // class's constrcutor is not used in incrementing when calling this
   // method override.  Returns the new value of the integer field object.
   // Value wraps around to MinValue if the integer field object is
   // incremented beyond MaxValue.
   long Increment( const long Interval );

   // Decrements the integer value by the number specified in the Interval
   // parameter to this class's constructor.
   // Useful for VK_DOWN handler in the OnKeyDown( ) override of your derived
   // class.
   void Decrement( );

   // Decrements the integer value by the number specified in the Interval
   // parameter to this method.  The interval specified in this class's
   // constructor is not used in this override.  Useful for VK_DOWN handler
   // in the OnKeyDown( ) override of your derived class.  Returns the new
   // value of the integer field object.  Value wraps around to MaxValue if
   // the integer field object is decremented below MinValue.
   zLONG Decrement( const zLONG Interval );

   // Get the current value for the field represented by this CGCIntegerField
   // object.
   double GetValue( ) const;
   void GetValue( zLONG& Val ) const;

   // Call from your OnChar( ) override in your derived class.  This is used
   // to build an integer value from the input keystrokes.  It returns TRUE
   // if the long integer represented by this class was modified.  Returns
   // FALSE if it was not.  FALSE would be returned if nChar was invalid.
   zBOOL Construct( const UINT nChar );

   // Useful for assigning a long value to your integer field object.
   // Returns TRUE if the value was successfully assigned.
   // Returns FALSE if the value represented by this object was unchanged
   // because Val was out of range as specified by the constructor.
   CGCIntegerField& operator=(const zLONG Val);

   // Checks to see if the current value of this object is greater than the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>(const CGCIntegerField& Val);

   // Checks to see if the current value of this object is greater than
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>(const zLONG Val);
   zBOOL operator>(const double& Val) const;

   // Checks to see if the current value of this object is >= the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>=(const CGCIntegerField& Val);

   // Checks to see if the current value of this object is >=
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>=(const zLONG Val);
   zBOOL operator>=(const double& Val) const;

   // Checks to see if the current value of this object is less than the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<(const CGCIntegerField& Val);

   // Checks to see if the current value of this object is less than
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<(const zLONG Val);
   zBOOL operator<(const double& Val) const;

   // Checks to see if the current value of this object is <= the current
   // value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<=(const CGCIntegerField& Val);

   // Checks to see if the current value of this object is <=
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<=(const long Val );
   zBOOL operator<=(const double& Val) const;

   // Checks to see if the current value of this object is equal to the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator==(const CGCIntegerField& Val);

   // Checks to see if the current value of this object is equal to
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator==(const long Val);
   zBOOL operator==(const double& Val) const;

   // Sets the value represented by this field to a negative value assuming
   // that this field support object supports negative values.  If this
   // object were created such that negative numbers were not allowed
   // (i.e.  minimum range value was greater than or equal to 0) then the
   // value represented by this object is not changed and an Assertion is
   // issued in the debug build.
   CGCIntegerField& operator-( );   // - unary operator.

   // This method resets this object to the minimum value provided
   // to this class's constructor.
   void Reset( ){ m_lValue = m_lMinValue;};

   // Returns this object's minimum and maximum possible values.
   void GetMinMax( double& dMin, double& dMax ) const
   {
      dMin = double( m_lMinValue );
      dMax = double( m_lMaxValue );
   };

private:
   zLONG  m_lInterval;     // Interval used when calling the parameterless versions
                           // of Increment( ) and Decrement( )
   zBOOL  m_bIsNegative;   // Is the value currently a negative?
   zBOOL  m_bAllowNegatives; // Flag indicating whether or not to allow a negative number.
   zLONG  m_lValue;        // The current value of the field.
   CString m_csValue;      // Internal string representation of the field.
   zLONG  m_lMinValue;     // Minimum allowed value for the field.
   zLONG  m_lMaxValue;     // Maximum allowed value for the field.
};

// Returns TRUE if ptr ==> to a CGCIntegerField class object.
// Requires /GR compiler option to support RTTI.
zBOOL IsIntegerField( CGCFieldBase *ptr );

// Casts a ptr to CGCIntegerField * if possible.  Otherwise, expression
// evaluates to NULL.
#define CASTIF( ptr ) dynamic_cast<CGCIntegerField*>( ptr )

#endif

#ifdef GCMFESUPP_DBL

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGCDoubleField
//
// You should use this class when implementing a float/double field in the
// class that you derive from ZMFE.  It will help process key
// strokes in your OnChar( ) and OnKeyDown( ) overrides.
//
//                               HOW TO USE
//
// In your derived class, create a member variable of class CGCDoubleField.
// Then use the Increment( ) and Decrement( ) methods in your OnKeyDown( )
// handler for the up/down cursor control (arrow) keys if you wish to handle
// these keys in your derived class.  When implementing your OnChar( )
// override, call the Construct( ) method, passing it the value of nChar
// that was input to the OnChar( ) handler.  The CGCDoubleField class will
// handle the input appropriately to modify the value associated with the
// float/double field. Call GetValue( ) as needed to obtain the current
// value represented by the CGCDoubleField class object.
//
// You will need to create a CGCDoubleField object for each Floating Point
// Field in your Multi-Field Edit Control.

class CGCDoubleField : public CGCNumBase
{
public:
   // Ctor to be used when creating a CGCDoubleField field support object.
   CGCDoubleField( double MinValue, // Minimum allowed value for float/double field.
               double MaxValue,     // Maximum allowed value for float/double field.
               UINT MaxPrecision,   // Maximum number of digits allowed to right of decimal.
               double Interval );   // Interval to be use by the parameterless versions of
                              // Increment( ) and Decrement( ).


   CGCDoubleField( );         // Default constructor.  Not intended to be
                              // used by the client.  This is provided
                              // to support the creation of arrays of CGCDoubleField
                              // objects if needed.

   ~CGCDoubleField( ) {};

   // Increment the double number by the value specified in the Interval
   // parameter to this class's constructor.
   // Useful for VK_UP handler in the OnKeyDown( ) override of your derived
   // class.
   void Increment( );

   // Increments the double value by the interval specified in the call to
   // this method. The Interval specified in the constructor is not used in
   // this override. Useful for VK_UP handler in the OnKeyDown( ) override
   // of your derived class. Returns the new value of the float/double field
   // object.  Value wraps around to MinValue if the float/double field
   // object is incremented beyond MaxValue.
   double Increment( const double Interval );

   // Decrements the double value by the Interval specified in the Interval
   // parameter to this class's constructor.  Useful for VK_DOWN handler in
   // the OnKeyDown( ) override of your derived class.
   void Decrement( );

   // Decrements the double value by the interval provided to this method.
   // The interval specified to this class's constructor is not used by this
   // override.  Useful for VK_DOWN handler in the OnKeyDown( ) override of
   // your derived class.  Returns the new value of the float/double field
   // object.  Value wraps around to MaxValue if the float/double field
   // object is decremented below MinValue.
   double Decrement( const double Interval );

   // Get the current long value for your float/double field object.
   double GetValue( ) const;
   void GetValue( double& Val ) const;

   // Call from your OnChar( ) override in your derived class.  This is used
   // to build a float/double value from the input keystrokes.  It returns
   // TRUE if the value is changed.  If nChar is not a valid character, FALSE
   // is returned.
   zBOOL Construct( const UINT nChar );

   // Useful for assigning a double value to your float/double field object.
   // Returns TRUE if Val was within the range specified by this class's ctor.
   // Returns FALSE and leaves the double represented by this class unchanged
   // if Val is outside the range.
   CGCDoubleField& operator=(const double Val);  // = operator

   // Sets the value represented by this field to a negative value assuming
   // that this field support object supports negative values.  If this
   // object were created such that negative numbers were not allowed (i.e.
   // minimum range value was greater than or equal to 0) then the value
   // represented by this object is not changed and an Assertion is issued
   // in the debug build.
   //
   CGCDoubleField& operator-( );    // - unary operator.

   // Checks to see if the current value of this object is greater than the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>(const CGCDoubleField& Val);

   // Checks to see if the current value of this object is greater than
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>(const double& Val) const;

   // Checks to see if the current value of this object is >= the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>=(const CGCDoubleField& Val);

   // Checks to see if the current value of this object is >= Val.
   // Returns TRUE if it is, FALSE otherwise.
   zBOOL operator>=(const double& Val) const;

   // Checks to see if the current value of this object is less than the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<(const CGCDoubleField& Val);

   // Checks to see if the current value of this object is less than
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<(const double& Val) const;

   // Checks to see if the current value of this object is <= the current
   // value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<=(const CGCDoubleField& Val);

   // Checks to see if the current value of this object is <=
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator<=(const double& Val) const;

   // Checks to see if the current value of this object is equal to the
   // current value of Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator==(const CGCDoubleField& Val);

   // Checks to see if the current value of this object is equal to
   // Val.  Returns TRUE if it is, FALSE otherwise.
   zBOOL operator==(const double& Val) const;

   // This method resets this object to the minimum value provided
   // to this class's constructor.
   void Reset( ){ m_dValue = m_MinValue; };

   // Returns this object's minimum and maximum possible values.
   void GetMinMax( double& Min, double& Max ) const
   {
      Min = m_MinValue;
      Max = m_MaxValue;
   };

private:
   void RoundNTrunc( );
   double m_dInterval;     // Interval used when calling the parameterless versions
                           // of Increment( ) and Decrement( ).
   zBOOL m_bIsNegative;    // Is the value currently a negative?
   zBOOL m_bAllowNegatives; // Flag indicating whether or not to allow a negative number.
   UINT m_uiMaxPrecision;  // Digits to use to the right of the decimal pt.
   double m_dValue;        // The current value of the field.
   CString m_csValue;      // Internal string representation of the field.
   double m_MinValue;      // Minimum allowed value for the field.
   double m_MaxValue;      // Maximum allowed value for the field.
};

// Returns TRUE if ptr ==> to a CGCDoubleField class object.
// Requires /GR compiler option to support RTTI.
zBOOL IsDoubleField( CGCFieldBase *ptr );

// Casts a ptr to CGCDoubleField * if possible.  Otherwise, expression
// evaluates to NULL.
#define CASTDF( ptr ) dynamic_cast<CGCDoubleField*>( ptr )

#endif

#ifdef GCMFESUPPTXTBASE

// Abstract ( pure virtual ) class used as a parent to the CGCTextField and
// CGCStringListField field helper classes.
class CGCTextBase : public CGCFieldBase
{
public:
   CGCTextBase( ) {};
   ~CGCTextBase( ) {};

   // I do not provide a virtual Construct( ) (not to be confused with a
   // class constructor) here because one is inherited from CGCFieldBase.
   virtual CString GetValue( ) const = 0 { return( "" ); };

   // We do not need to define versions of "+" and "=" that accept
   // const char * because the compiler will convert the strings to
   // CStrings before executing the operator.

   // Checks to see if the current value of this object is lexigraphically
   // greater than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   virtual zBOOL operator>(const CString& Val) = 0 { return( FALSE );};

   // Checks to see if the current value of this object is lexigraphically
   // less than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   virtual zBOOL operator<(const CString& Val) = 0 {return( FALSE );};

   // Checks to see if the current value of this object is lexigraphically
   // equal to Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   virtual zBOOL operator==(const CString& Val) = 0 {return( FALSE );};

   // I did not add the "+" operator at this level because it has different
   // meanings for CGCTextField and CGCStringListField.  I also did not
   // include the "[]" operator at this level for the same reason.
};
#endif

#ifdef GCMFESUPP_TXT

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGCTextField
//
// You should use this class when implementing a text field in your class
// that you derive from ZMFE.  It will help process key strokes
// in your OnChar( ) override.
//                               HOW TO USE
//
// In your derived class, create a member variable of class CGCTextField.
// When implementing your OnChar( ) override, call the Construct( ) method,
// passing it the value of nChar input to the OnChar( ) handler.  The
// CGCTextField class will handle the input appropriately to modify the
// value associated with the string field.  Call GetValue( ) as needed to
// obtain the current value represented by the CGCTextField class object.
//
// You will want to create a CGCTextField object for each Text Field in
// your Multi-Field Edit Control.

class CGCTextField : public CGCTextBase
{
public:

   CGCTextField( UINT MaxCharacters, // Specify maximum number of characters in text field.
                                     // This should be no larger than the number of characters
                                     // that were specified in the ZMFE::Format( )
                                     // call for the field.
                 CString UnallowedCharacters = "" ); // String of characters not allowed.  May be empty.

   zBOOL Construct( const UINT nChar );// Appends nChar to string.
                                       // Starts a new string if the string is already
                                       // MaxCharacters is length.

   CGCTextField& operator+(const CString& Val); // Concatenates Val to the
                                       // existing string, truncating characters
                                       // that exceed MaxCharacters.

   CGCTextField& operator=(const CString& Val);  // Assigns string Val to the
                                       // CGCTextField object.
                                       // Returns TRUE if the value was changed,
                                       // FALSE otherwise.

   CString GetValue( ) const;          // Returns the string value of
                                       // the Text Field represented by
                                       // this CGCTextField object.

   zCHAR operator[](const UINT ui) const; // Returns a ptr to the character
                                         // at position ui or NULL if ui is
                                         // out of range.

   // Checks to see if the current value of this object is lexigraphically
   // greater than the current value of Val.  Returns TRUE if it is, FALSE
   // otherwise.  Makes a non-case-sensitive comparison.
   zBOOL operator>(const CGCTextField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // greater than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator>(const CString& Val);

   // Checks to see if the current value of this object is lexigraphically
   // less than the current value of Val.  Returns TRUE if it is, FALSE
   // otherwise.  Makes a non-case-sensitive comparison.
   zBOOL operator<(const CGCTextField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // less than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator<(const CString& Val);

   // Checks to see if the current value of this object is lexigraphically
   // equal to the current value of Val.  Returns TRUE if it is, FALSE
   // otherwise.  Makes a non-case-sensitive comparison.
   zBOOL operator==(const CGCTextField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // equal to Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator==(const CString& Val);

   // "Nulls" out the string.
   void Reset( );

   // Returns the maximum number of characters allowed in the string.
   UINT GetMaxCharacters( ) { return( m_uiMaxCharacters ); }

   // Returns the current length of the string in the Text Field.
   UINT GetLength( ) { return( m_csValue.GetLength( ) ); }

private:
   CString m_csUnallowed;      // Characters not allowed in the field.
   CString m_csValue;          // Current value of the string field.
   UINT m_uiMaxCharacters;     // Maximum characters allowed in the field.
};

// Returns TRUE if ptr ==> to a CGCTextField class object.
// Requires /GR compiler option to support RTTI.
//
zBOOL IsTextField( CGCFieldBase *ptr );

// Casts a ptr to CGCTextField * if possible.  Otherwise, expression
// evaluates to NULL.
#define CASTTF( ptr ) dynamic_cast<CGCTextField*>( ptr )

#endif

#ifdef GCMFESUPP_TLS

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CGCStringListField
//
// Class CGCStringListField helps you to create a field that may display any
// one of a list of strings that you specify.
//
//                               HOW TO USE
//
// In your derived class, create a member variable of class CGCStringListField.
// Then use the Increment( ) and Decrement( ) methods in your OnKeyDown( )
// handler for the up/down cursor control ( arrow ) keys if you wish to handle
// these keys in your derived class.  When implementing your OnChar( )
// override, call the ConstructInputAndSearch( ) method, passing it the value
// of nChar input to the OnChar( ) handler.  The CGCStringListField class will
// handle the input appropriately to modify the value associated with the
// float/double field.  Call GetValue( ) as needed to obtain the currently
// displayed entry.
//
// Call the "+" operator in your derived class's constructor to build the
// list of values to be displayed in the CGCStringListField field.
//
// You will want to create a CGCStringListField object for each String List
// Field in your Multi-Field Edit Control.

class CGCStringListField : public CGCTextBase
{
public:
   CGCStringListField( );

   CString GetValue( ) const;           // Gets the currently displayed string.

   zBOOL Construct( const UINT nChar ); // Takes operator input, adds it to
                                        // the previous input, and checks to
                                        // see if it evaluates to an entry in
                                        // the list.  If it does, that entry is displayed
                                        // and TRUE is returned.  If it does not, then
                                        // the control does not change and FALSE
                                        // is returned.

   CString operator+(const CString& NewString); // Appends a new entry to the list.
                                          // Note that this class does
                                          // NOT restrict you on the length of
                                          // each string so that you have greater
                                          // flexibility in using the CGCStringListField.
                                          // But remember, if you obtain the
                                          // value of this object by calling GetValue( ) and
                                          // pass this string to the ZMFE::Update( )
                                          // method, the string will be truncated on the right
                                          // to the length specified for the field.

   CGCStringListField& operator=(const CString& Val); // Sets the field to display the
                                          // specified entry.  If the entry
                                          // isn't in the list, the control
                                          // does not change and FALSE is
                                          // returned.

   CString operator[](const UINT ui) const;

   // Checks to see if the current value of this object is lexigraphically
   // greater than the value currently displayed in Val.  Returns TRUE if it
   // is, FALSE otherwise. Makes a non-case-sensitive comparison.
   zBOOL operator>(const CGCStringListField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // greater than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator>(const CString& Val);

   // Checks to see if the current value of this object is lexigraphically
   // less than the value currently displayed in Val.  Returns TRUE if it is,
   // FALSE otherwise. Makes a non-case-sensitive comparison.
   zBOOL operator<(const CGCStringListField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // less than Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator<(const CString& Val);

   // Checks to see if the current value of this object is lexigraphically
   // equal to the value currently displayed in Val.  Returns TRUE if it is,
   // FALSE otherwise.  Makes a non-case-sensitive comparison.
   zBOOL operator==(const CGCStringListField& Val);

   // Checks to see if the current value of this object is lexigraphically
   // equal to Val.  Returns TRUE if it is, FALSE otherwise.
   // Makes a non-case-sensitive comparison.
   zBOOL operator==(const CString& Val);

   CString Increment( );                // Go to the next entry.  Use
                                        // this in your OnKeyDown( ) handler
                                        // for the UP arrow key.

   CString Decrement( );                // Go to the previous entry.  Use
                                        // this in your OnKeyDown( ) handler
                                        // for the DOWN arrow key.

   // This method resets this object to the minimum value provided to this
   // class's constructor.
   void Reset( ) { m_lIndex = 0; }

   // This method is used to delete all entries from the list.
   void Clear( );                       // Clears the object, deleting all
                                        // entries in the list.

   // Returns the number of entries in the String List associated with this
   // String List Field.
   UINT GetNumEntries( ) { return( m_List.size( ) ); }

   // Returns the index of the currently selected/displayed item or -1 if the
   // list is empty.
   zLONG GetIndex( ) { return( m_lIndex ); }

   // Performs a non-case sensitive search for Str in the list.  If it finds
   // it, it returns a 0-based index in the list of the string.  Otherwise,
   // it returns -1 to indicate that Str was not found in the list.
   zLONG GetEntryPos( const CString& Str );

   // Allows for the deletion of an entry based on a specified index into the
   // list. Returns TRUE if the entry was the one currently displayed.  This
   // can be used as a trigger to update the display of the Multi-Field Edit.
   // If the entry with the specified index was not currently displayed, or
   // if the specified Index is invalid, FALSE is returned.
   zBOOL DeleteEntry( const UINT Index );

   ~CGCStringListField( );
private:
   CString m_InputString;                // Used for tracking operator input.
   zLONG   m_lIndex;                     // Currently displayed entry.
   vector<CString> m_List;               // Internal string.
};

// Returns TRUE if ptr ==> to a CGCStringListField class object.
// Requires /GR compiler option to support RTTI.
zBOOL IsStringListField( CGCFieldBase *ptr );

// Casts a ptr to CGCStringListField * if possible.  Otherwise, expression
// evaluates to NULL.
#define CASTLF( ptr ) dynamic_cast<CGCStringListField *>( ptr )

#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately
// before the previous line.

#endif

#endif   // __zdctledt_h__ sentry.
