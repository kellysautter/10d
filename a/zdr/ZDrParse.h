/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 2007 - 2008 Arksoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrparse.h
// AUTHOR:
//
// OVERVIEW
// ========
// Class definitions for standard Zeidon DR Ctrls.
//

//
// Change log most recent first order:
//
#ifndef __zdrparse_h__  // Sentry, use file only if it's not already included
#define __zdrparse_h__

#ifndef __cplusplus
#error  C++ compilation is required (use a .cpp suffix)
#endif

class ZDRAPP_CLASS ZHTML_Attribute
{
   friend zSHORT
          fnCompareAttributes( const ZHTML_Attribute *pAttribute1,
                               const ZHTML_Attribute *pAttribute2 );
   friend zSHORT
          fnSeekAttribute( const CString *pzsTag,
                           const ZHTML_Attribute *pAttribute );
   friend void
          fnDeleteAttribute( ZHTML_Attribute *pAttribute,
                             WPARAM wParam,
                             LPARAM lParam );
   friend void *
          fnCopyAttribute( ZHTML_Attribute *pAttribute );
public:
   // The default constructor.  Construct a blank attribute.
   ZHTML_Attribute( );
   ZHTML_Attribute( CString& csName, CString& csValue, char chDelimiter = 0 );
   virtual ~ZHTML_Attribute( );

   // The delimiter for this attribute.
   char GetDelimiter( ) { return( m_chDelimiter ); }
   void SetDelimiter( char chDelimiter ) { m_chDelimiter = chDelimiter; }

   // The name for this attribute.
   CString GetName( ) { return( m_csName ); }
   void SetName( CString& csName ) { m_csName = csName; }

   // The value for this attribute.
   CString GetValue( ) { return( m_csValue ); }
   void SetValue( CString& csValue ) { m_csValue = csValue; }
   int     m_t;           // debugging only

private:

   CString m_csName;      // name of this attribute
   CString m_csValue;     // value of this attribute
   char    m_chDelimiter; // delimiter for the value of this attribute (i.e. " or ')
};

class ZDRAPP_CLASS ZHTML_TagAttributeList : public ZMultiIdxSet
{
public:
   // Create a new, empty, attribute list.
   ZHTML_TagAttributeList( );

   // Delete attribute list.
   ~ZHTML_TagAttributeList( );

   // Add the specified attribute to the list of attributes.
   // Parm: pAttribute - Attribute to add to this ZHTML_TagAttributeList.
   zSHORT AddAttribute( ZHTML_Attribute *pAttribute );

   // Clear all attributes from this ZHTML_TagAttributeList and return it to a empty state.
   void ClearInit( );

   // Returns true if this ZHTML_TagAttributeList is empty, with no attributes ... false otherwise.
   zBOOL IsEmpty( );

   // If there is already an attribute with the specified name, it will have its
   // value changed to match the specified value. If there is no ZHTML_Attribute
   // with the specified name, one will be created. This method is case-insensitive.
   // Parm: csName - Name of the ZHTML_Attribute to edit or create.  Case-insensitive.
   // Parm: csValue - Value to be held in this attribute.
   // Parm: chDelimiter - Delimiter for value (usually quote or apostrophe).
   // Parm: lFLag - 0 - replace value; 1 - append value.
   void SetAttributeProperties( CString& csName, CString& csValue, char chDelimiter = 0, zLONG lFlag = 0 );

   // Number of attributes in this ZHTML_TagAttributeList.
// zLONG GetCount( );

// // A list of the attributes in this ZHTML_TagAttributeList.
// ZMultiIdxSet * GetList( );

   // is this used???
// void CreateZeidonTag( );

   // Reconstruct the HTML tag.
   CString BuildTag( zBOOL bTagOnly );

   // Access the individual attributes.
   ZHTML_Attribute * GetItem( zLONG lIndex );

   // Access the individual attributes by name.
   ZHTML_Attribute * GetItem( CString& csIndex );

   CString m_csTag;       // tag to which attributes apply
   zBOOL   m_bTag;
   zSHORT  m_nTagType;    // 1 - open tag; 2 - close tag; 3 - open/close tag

// protected:
//    ZMultiIdxSet *m_pList; // internal vector contains the entire list of attributes
};

// Base class for parsing tag based files, such as HTML, HTTP headers, or XML.
class ZDRAPP_CLASS ZHTML_Parse
{
public:
   ZHTML_Parse( );
   ~ZHTML_Parse( );

   // Determine if the specified character is whitespace or not.
   // Parm: ch - Character to check
   // Returns true if the character is whitespace.
   zBOOL IsWhiteSpace( char ch );

   // Advance the index until past any whitespace.
   void EatWhiteSpace( );

   // Find the next character past any whitespace.  It does not
   // reset the current index (m_lIdx).
   char FindNextNonWhiteSpace( );

   // Determine if the end of the source text has been reached.
   // Returns True if the end of the source text has been reached.
   zBOOL Eof( ) { return( m_lIdx >= m_lSrcLth ); }

   // Parse the attribute name.
   void ParseAttributeName( );

   // Parse the attribute value.
   void ParseAttributeValue( );

   // Add a parsed attribute to the collection.
   void AddAttribute( );

   // Get the current character that is being parsed.
   char GetCurrentChar( ) { return( GetCurrentChar( 0 ) ); }

   // Get a few characters ahead of the current character.
   // Parm: nPeek - Number of characters to peek ahead for.
   // Returns - The character that was retrieved.
   char GetCurrentChar( zLONG lPeek );

   // Obtain the next character and advance the index by one.
   // Returns - The next character.
   char AdvanceCurrentChar( );

   // Move the nIndex forward by one.
   void Advance( ) { m_chPrev = GetCurrentChar( 0 ); m_lIdx++; }

   // The last attribute name that was encountered.
   CString GetParseName( ) { return( m_csParseName ); }
   void SetParseName( CString& csName ) { m_csParseName = csName; }

   // The last attribute value that was encountered.
   CString GetParseValue( ) { return( m_csParseValue ); }
   void SetParseValue( CString& csValue ) { m_csParseValue = csValue; }

   // The last attribute delimiter that was encountered.
   char GetParseDelimiter( ) { return( m_chParseDelimiter ); }
// void SetParseDelimiter( CString& csDelimiter ) { m_chParseDelimiter = csDelimiter; }
   void SetParseDelimiter( char chDelimiter ) { m_chParseDelimiter = chDelimiter; }

   // The text that is to be parsed.
   CString GetSource( ) { return( m_csSource ); }
   void SetSource( CString& csSource ) { m_csSource = csSource;
                                         m_lSrcLth = m_csSource.GetLength( ); }

   void SetAttributeList( ZHTML_TagAttributeList *pTagAttributeList ) { m_pTagAttributeList = pTagAttributeList; }

protected:
   ZHTML_TagAttributeList *m_pTagAttributeList;
   CString m_csSource;      // source text that is being parsed
   zLONG   m_lSrcLth;       // length of source text that is being parsed
   zLONG   m_lIdx;          // current position inside of the text that is being parsed
   char    m_chParseDelimiter; // most recently parsed attribute delimiter
   char    m_chPrev;        // character prior to current character
   CString m_csParseName;   // most recently parsed attribute name
   CString m_csParseValue;  // most recently parsed attribute value
};

class ZDRAPP_CLASS ZHTML_ParseHTML : public ZHTML_Parse
{
public:
   ZHTML_ParseHTML( );
   ~ZHTML_ParseHTML( );

   char ParseHTML( );
   void ParseHTML_Page( CString& csPage, zCPCHAR cpcSearchAttribute );
   zSHORT AddTagAndAttributeToList( CString& csTag, zCPCHAR cpcAttributeName, zCPCHAR cpcAttributeValue, char chDelimiter, zLONG lFlag );
   ZHTML_TagAttributeList * GetNextInTagList( CString& cs, ZHTML_TagAttributeList *pTAL, zPCHAR pchValidHTML_TagList );
   zSHORT RemoveTag( CString& csReturnTag, CString& csPage, CString& csTag );
   void ReconstituteHTML( CString& csHTML, zCPCHAR cpcValidHTML_TagList, zLONG lFlags );
   ZHTML_TagAttributeList * PopTagList( ZHTML_TagAttributeList *pTAL );
   void CreateTagOI( ZHTML_TagAttributeList *pTAL, zVIEW   vTgt, zLONG   lLevel );
   void HTML_ToZeidonOI( zVIEW vTgt );
   CString ZeidonOI_ToHTML( zVIEW vSrc, CString& csPage );
   void ConstructTag( zVIEW vSrc, CString& csPage, CString& csLastTag );

   CString m_csDivEntity;          // "div" (zeidon entity)
   CString m_csDivAttribute;       // "zid" (zeidon attribute)
   CString m_csTagEntity;          // HTML tag (zeidon entity)
   CString m_csTagName;            // HTML tag name (zeidon attribute)
   CString m_csTagValue;           // HTML tag value (zeidon attribute)
   CString m_csTagLevel;           // HTML tag indentation level (zeidon attribute)
   CString m_csTagYN;              // Y ==> it is a tag   N ==> not a tag (comment ...) (zeidon attribute)
   CString m_csTagType;            // 1 ==> start; 2 ==> end; 3 ==> both; 0 ==> not a tag/indeterminate (zeidon attribute)
   CString m_csAttributeEntity;    // HTML tag attribute (zeidon entity)
   CString m_csAttributeName;      // HTML tag attribute name (zeidon attribute)
   CString m_csAttributeValue;     // HTML tag attribute value (zeidon attribute)
   CString m_csAttributeDelimiter; // HTML tag attribute delimiter (", ', or null) (zeidon attribute)
   CString m_csSearchAttribute;    // HTML tag attribute to search for while parsing

protected:
   ZPointerList m_TagList;         // list of tags within this HTML
   void ParseTag( );
   void ParseTagValue( );

private:
   zLONG m_lFlags;                 // 0 - full HTML; 1 - strip invalid tags; 2 - strip properties
};

#endif   // __zdrparse_h__ sentry.
