/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 2007 - 2008 Arksoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrparse.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of the Palette Message Handling class.
// The code is derived from code written by Paul DiLascia.

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...
//

#include "zstdafx.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "ZDrParse.h"
#include "zdrgbl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//
// To turn on tracing for palette messages, you need to define _TRACEPAL
// here or in your make file

// #define _TRACEPAL
// Parsing HTML in Microsoft C#
// By Jeff Heaton
//
// Most data on the Web is stored in the Hypertext Markup Language (HTML) format.
// There are many times that you might want to parse HTML in your C# application.
// However, the .NET framework does not provide an easy way to parse HTML.
// Evidence of this is the numerous questions posted by C# programmers looking
// for an easy way to parse HTML.
//
// The Microsoft .NET framework includes extensive support for Extensible Markup
// Language (XML). However, although XML and HTML look very similar, they are not
// very compatible. Consider the following major differences between XML and HTML:
//
// XML requires end tags.
// All XML attribute values must be fully quoted with either single or double quotes.
// XML tags must be properly nested.
// XML tag names are case sensitive.
// XML does not allow duplicate attributes.
// Empty attributes are not allowed in XML.
//
// Let's look at one of these examples in code to illustrate the difference. In
// XML, every beginning tag must have an ending tag. The following HTML would cause
// problems for an XML parser.
//
// <p>This is line 1<br>
// This is line 2</p>
//
// This is just one of many differences. Of course, you can require HTML to be
// written in such a way that it is compatible with XML. The preceding HTML
// could be rewritten as in the following example.
//
// <p>This is line 1<br/>
// This is line 2</p>
//
// Both an XML parser and any modern browser could understand this. Unfortunately,
// this is not a viable solution because you do not control the sources of HTML.
// You will want your program to be able to process HTML from any source.
//
// The Solution
// Because of this, I found it necessary to write my own HTML parser. In this
// article, I will show you how my HTML parser was constructed, and how you can
// use this parser with your own applications. I will begin by showing you the
// main components that make up the HTML parser. I will conclude this article
// by showing a simple example that uses the HTML parser.
//
// The HTML parser consists of the following four classes:
//
// ZHTML_Attribute - The attribute class is used to hold an individual attribute
// inside an HTML tag.
// ZHTML_TagAttributeList - The attribute list holds an individual HTML tag and all
// of its attributes.
// ZHTML_Parse - Holds general text parsing routines.
// ZHTML_ParseHTML - The main class that you will interface with; the ZHTML_ParseHTML
// class is fed the HTML that you would like to parse.
//
// I will now show you how each of these classes functions, and how you will
// use them. I will begin with the ZHTML_Attribute class.
//
// The ZHTML_Attribute Class
// The ZHTML_Attribute class is used to store individual HTML attributes. The source
// code for the ZHTML_Attribute class can be seen in Listing 1. The following HTML
// tag demonstrates attributes:
//
// <img src="picture.gif" alt="Some Picture">
// The above HTML tag has two attributes named "src" and "alt". The values
// of these two attributes are "picture.gif" and "Some Picture", respectively.
//
// The ZHTML_Attribute class consists of three properties named "m_csName", "m_csValue",
// and "m_chDelimiter". The "m_csName" property stores the name of the attribute. The
// "m_csValue" property stores the value held by the property. And finally, the
// "m_chDelimiter" property holds the character that was used to delimit the value,
// if any. This property will either hold a quote ("), an apostrophe ('), or
// nothing at all, depending on what was used to delineate the value.
//
// The ZHTML_TagAttributeList Class
// An HTML tag often consists of several attributes. The "ZHTML_TagAttributeList"
// class is used to hold a list of these attributes. The "ZHTML_TagAttributeList"
// class is shown in Listing 2. The "ZHTML_TagAttributeList" class consists of a
// name and a collection of attributes. The "ZHTML_TagAttributeList" name, stored
// in a property called "name", holds the name of the tag. When tags are
// returned to you from the parser, they will be in the form of "ZHTML_TagAttributeList"
// objects.
//
// The ZHTML_TagAttributeList class makes use of the C# indices. You can access
// individual attributes both by numeric and string indicies. For example,
// if an attribute "src" were stored in the "ZHTML_TagAttributeList" object "theTag",
// you could access the "src" attribute in the following ways:
//
// theTag[ 0 ]    // assuming "src" was the first attribute
// theTag[ "src" ]
//
// Both of these methods could be used to access the attributes of the tag.
//
// The ZHTML_Parse and ZHTML_ParseHTML Classes
// If you only want to use the classes to parse HTML, you need not be concerned with
// the "ZHTML_Parse" class. The "ZHTML_Parse" class is used internally by the HTML
// parser to provide low-level support for attribute-value based files, such as
// HTML, SGML, XML, or even HTTP headers. The source code for the "ZHTML_Parse" class
// is shown in Listing 3. I will not cover the "ZHTML_Parse" class at length in this
// article. If you are interested in the ZHTML_Parse class, all of its methods are
// commented.
//
// The "ZHTML_ParseHTML" class subclasses the "ZHTML_Parse" class. The "ZHTML_ParseHTML"
// class provides the HTML-specific code needed to make the parser work with HTML.
// The ZHTML_ParseHTML class is shown in Listing 4. The "ZHTML_ParseHTML" class will
// be your primary interface to the HTML parser. Actually, using the HTML parser
// is covered in the next section. The methods that you will use primarily are
// listed here.
//
//     public char ZHTML_Parse( )
//     public ZHTML_TagAttributeList GetTag( )
//
// The ZHTML_Parse() method is called to retrieve the next character in the HTML
// file that you are parsing. If the next character is part of a tag, the value of
// zero is returned. When you see that ZHTML_Parse( ) has returned zero, you know
// that you must process an HTML tag. You can access the tag by calling the GetTag
// method. The GetTag method will return an ArrayList object that contains the tag
// and all of its attributes.
//
// Using the HTML Parser
// Now, I will show you an example of how to use the HTML parser. The example
// program will prompt you for a URL and then show you all of the hyperlinks
// contained within the HTML file located at the URL you specified. This example
// will only work on URLs that lead to HTML data; it will not work with images
// or other binary data. This example is shown in Listing 5.
//
// To see the program work enter any URL address, such as http://www.developer.com.
// The program will then display all of the hyperlinks contained on the homepage
// of Developer.COM.
//
// The loop that processes the page is shown here.
//
// ZHTML_ParseHTML parse = new ZHTML_ParseHTML( );
// parse.Source = page;
// while ( !parse.Eof( ) )
// {
//   char ch = parse.ZHTML_Parse( );
//   if ( ch == 0 )
//   {
//     ZHTML_TagAttributeList tag = parse.GetTag( );
//     if ( tag[ "href" ] != null )
//       System.Console.WriteLine( "Found link: " + tag[ "href" ].Value );
//   }
// }
//
// As you can see, a ZHTML_ParseHTML object is instantiated, and the object's Source
// property is set to the HTML page to be parsed. A loop continues so long as
// the end of the pages has not been reached. We ignore regular characters and
// look for each tag, noted by when ch is equal to zero. For each tag, we check
// to see if it has an HREF attribute. If an HREF attribute is present, the
// link is displayed.
//
// Conclusions
// As you can see, these classes provide an easy-to-use framework for HTML
// parsing that you can reuse in any Microsoft .NET application. The example
// program provided uses the parser only to display links. However, this parser
// has been used in a variety of complex HTML parsing applications.
//
// This source code may be used freely under the Limited GNU Public License (LGPL).
// Written by Jeff Heaton (http://www.jeffheaton.com)

/////////////////////////////////////////////////////////////////////////////
// The Attribute Class
/////////////////////////////////////////////////////////////////////////////

// Comparison function required to order objects added to the list.  In this
// case it just compares the names embedded in the ZHTML_Attribute type.
zSHORT
fnCompareAttributes( const ZHTML_Attribute *pAttribute1,
                     const ZHTML_Attribute *pAttribute2 )
{
   return( pAttribute1->m_csName.Compare( pAttribute2->m_csName ) );
}

// Comparison function required to seek for an object in the list.  In this
// case it just compares to the name embedded in the ZHTML_Attribute type.
zSHORT
fnSeekAttribute( const CString *pzsTag,
                 const ZHTML_Attribute *pAttribute )
{
   return( pzsTag->Compare( pAttribute->m_csName ) );
}

// Function to delete the attribute data.
void
fnDeleteAttribute( ZHTML_Attribute *pAttribute,
                   WPARAM wParam,
                   LPARAM lParam )
{
   mDeleteInit( pAttribute );
}

// Function to copy the attribute data.
void *
fnCopyAttribute( ZHTML_Attribute *pAttribute )
{
   return( new ZHTML_Attribute( pAttribute->GetName( ),
                                pAttribute->GetValue( ),
                                pAttribute->GetDelimiter( ) ) );
}

// ZHTML_Attribute holds one attribute, as is normally stored in an
// HTML or XML file. This includes a name, value and delimiter.
ZHTML_Attribute::ZHTML_Attribute( )
{
   m_csName  = "";
   m_csValue = "";
   m_chDelimiter = (char) 0;
   m_t = 0;  // debugging only
}

// Construct a new ZHTML_Attribute.  The name, delimiter, and value properties can be specified here.
// Parameters:
//   csName - name of this attribute.
//   csValue - value of this attribute.
//   chDelimiter - delimiter character for the value.
ZHTML_Attribute::ZHTML_Attribute( CString& csName, CString& csValue, char chDelimiter )
{
   csName.MakeLower( );
   m_csName  = csName;
   m_csValue = csValue;
   m_chDelimiter = chDelimiter;
}

ZHTML_Attribute::~ZHTML_Attribute( )
{
// TraceLineI( "ZHTML_Attribute::dtor for: ", m_t );
}

#if 0
// #region ICloneable Members
// public virtual object Clone( )
// {
//    return( new ZHTML_Attribute( m_csName, m_csValue, m_chDelimiter ) );
// }
// #endregion

// Make an exact copy of this object.
// Returns a new object that is a clone of the specified object.
ZHTML_TagAttributeList::Clone( )
{
   ZHTML_TagAttributeList rtn = new ZHTML_TagAttributeList( );

   for ( zLONG k = 0; k < m_list.Count; k++ )
      rtn.Add( (ZHTML_Attribute) this[ k ].Clone( ) );

   return( rtn );
}
#endif

/////////////////////////////////////////////////////////////////////////////
// The AttributeList Class
/////////////////////////////////////////////////////////////////////////////

// Create a new, empty, attribute list.
ZHTML_TagAttributeList::ZHTML_TagAttributeList( ) :
                        ZMultiIdxSet( (fnCOMPARE) fnCompareAttributes,
                                      (fnCOMPARE) fnSeekAttribute,
                                      8,  // initially holds eight attributes
                                      (fnVISIT_INORDER) fnDeleteAttribute,
                                      (fnCOPY) fnCopyAttribute )
{
   m_csTag = "";
   m_bTag = FALSE;
   m_nTagType = 0;
}

// Delete attribute list.
ZHTML_TagAttributeList::~ZHTML_TagAttributeList( )
{
   ClearInit( );
}

// Add the specified attribute to the list of attributes.
// Parm: pAttribute - An attribute to add to this ZHTML_TagAttributeList.
zSHORT
ZHTML_TagAttributeList::AddAttribute( ZHTML_Attribute *pAttribute )
{
   if ( InsertNode( pAttribute ) == 0 )
      return( m_nErrorCode );
   else
      return( 0 );
}

// Clear all attributes from this ZHTML_TagAttributeList and return it to an empty state.
void
ZHTML_TagAttributeList::ClearInit( )
{
   ResetContent( );
   m_csTag.Empty( );
   m_bTag = FALSE;
   m_nTagType = 0;
}

// Returns true if this ZHTML_TagAttributeList is empty, with no attributes.
// Returns true if this ZHTML_TagAttributeList is empty, false otherwise.
zBOOL
ZHTML_TagAttributeList::IsEmpty( )
{
   return( GetCount( ) <= 0 );
}

// If there is already an attribute with the specified name, it will have its
// value changed to match the specified value. If there is no ZHTML_Attribute
// with the specified name, one will be created. This method is case-insensitive.
// Parm: csName - Name of the ZHTML_Attribute to edit or create.  Case-insensitive.
// Parm: csValue - Value to be held in this attribute.
// Parm: chDelimiter - Delimiter to surround this attribute.
// Parm: lFlag - 0 - Set/Replace current value of this attribute.
//               1 - Append value to current value of this attribute.
//               2 - Append value to current, resetting delimiter to encompass entire value
void
ZHTML_TagAttributeList::SetAttributeProperties( CString& csName, CString& csValue, char chDelimiter, zLONG lFlag )
{
   if ( csName.IsEmpty( ) == false )
   {
      csName.MakeLower( );

   // if ( csValue.IsEmpty( ) )
   //    csValue = "";

      ZHTML_Attribute *pAttribute = (ZHTML_Attribute *) SeekNode( &csName );

      if ( pAttribute )
      {
         if ( lFlag == 0 )
         {
            pAttribute->SetValue( csValue );
         }
         else
         {
            CString cs = pAttribute->GetValue( );
            if ( lFlag & 2 )
            {
               if ( cs.IsEmpty( ) == FALSE )
                  cs += " ";
            }

            cs += csValue;
            pAttribute->SetValue( cs );
         }
      }
      else
      {
         pAttribute = new ZHTML_Attribute( csName, csValue, chDelimiter );
         AddAttribute( pAttribute );
      }
   }
}

// Reconstruct the HTML tag.
CString
ZHTML_TagAttributeList::BuildTag( zBOOL bTagOnly )
{
   ZHTML_Attribute *pAttribute;
   CString csBuffer;
   zLONG   k;

   if ( m_bTag )
      csBuffer = "<";

   if ( bTagOnly == FALSE || m_bTag )
      csBuffer += m_csTag;

   if ( bTagOnly == FALSE )
   {
      for ( k = 0; pAttribute = (ZHTML_Attribute *) GetItem( k ); k++ ) // has attributes
      {
         csBuffer += " ";
         if ( pAttribute->GetValue( ) == "" )
         {
            if ( pAttribute->GetDelimiter( ) != 0 )
               csBuffer += pAttribute->GetDelimiter( );

            csBuffer += pAttribute->GetName( );
            if ( pAttribute->GetDelimiter( ) != 0 )
               csBuffer += pAttribute->GetDelimiter( );
         }
         else
         {
            csBuffer += pAttribute->GetName( );
            if ( pAttribute->GetValue( ) != "" )
            {
               csBuffer += "=";
               if ( pAttribute->GetDelimiter( ) != 0 )
                  csBuffer += pAttribute->GetDelimiter( );

               csBuffer += pAttribute->GetValue( );
               if ( pAttribute->GetDelimiter( ) != 0 )
                  csBuffer += pAttribute->GetDelimiter( );
            }
         }
      }
   }

   if ( m_bTag )
      csBuffer += ">";

   return( csBuffer );
}

// How many attributes are in this ZHTML_TagAttributeList?
//zLONG
//ZHTML_TagAttributeList::GetCount( )
//{
//   return( m_pList ? m_pList->GetCount( ) : 0 );
//}

// A list of the attributes in this ZHTML_TagAttributeList.
//ZMultiIdxSet *
//ZHTML_TagAttributeList::GetList( )
//{
//   return( m_pList );
//}

// Access the individual attributes.
ZHTML_Attribute *
ZHTML_TagAttributeList::GetItem( zLONG lIndex )
{
   if ( lIndex >= 0 && lIndex < GetCount( ) )
      return( (ZHTML_Attribute *) GetAt( lIndex + 1 ) );
   else
      return( 0 );
}

// Access the individual attributes by name.
ZHTML_Attribute *
ZHTML_TagAttributeList::GetItem( CString& csName )
{
   ZHTML_Attribute *pAttribute = (ZHTML_Attribute *) SeekNode( &csName );
   return( pAttribute );
}

/////////////////////////////////////////////////////////////////////////////
// The Parse Class
/////////////////////////////////////////////////////////////////////////////

static CString g_csWhiteSpace = "\t\n\r ";

ZHTML_Parse::ZHTML_Parse( )
{
   m_pTagAttributeList = 0;
   m_lIdx = 0;
   m_chPrev = 0;
}

ZHTML_Parse::~ZHTML_Parse( )
{
   mDeleteInit( m_pTagAttributeList ); // it should have been deleted by external code
}

zBOOL
ZHTML_Parse::IsWhiteSpace( char ch )
{
   return( g_csWhiteSpace.Find( ch ) != -1 );
}

// Advance the index past any whitespace.
void
ZHTML_Parse::EatWhiteSpace( )
{
   char ch;

   while ( Eof( ) == false )
   {
      ch = GetCurrentChar( );
      if ( IsWhiteSpace( ch ) == false )
         return;

      m_chPrev = ch;
      m_lIdx++;
   }
}

// Find the next character past any whitespace.  It does not
// reset the current index (m_lIdx).
char
ZHTML_Parse::FindNextNonWhiteSpace( )
{
   zLONG lIdx = m_lIdx;
   char  chPrev = m_chPrev;
   char  ch;

   EatWhiteSpace( );
   if ( Eof( ) == false )
      ch = m_csSource.GetAt( m_lIdx );
   else
      ch = 0;

   // Reset index and previous character.
   m_lIdx = lIdx;
   m_chPrev = chPrev;

   return( ch );
}

// Parse the attribute name.
void
ZHTML_Parse::ParseAttributeName( )
{
   char ch;

   EatWhiteSpace( );

   // Get attribute name.
   while ( Eof( ) == false )
   {
      ch = GetCurrentChar( );
      if ( IsWhiteSpace( ch ) || (ch == '=') || (ch == '>') )
      {
         break;
      }

      m_csParseName += ch;
      m_chPrev = ch;
      m_lIdx++;
   }

   if ( ch == '>' && m_chPrev == '/' )
      m_pTagAttributeList->m_nTagType |= 2;  // close

   EatWhiteSpace( );
}

// Parse the attribute value.
void
ZHTML_Parse::ParseAttributeValue( )
{
   char ch;

   if ( m_chParseDelimiter != 0 )
      return;

   ch = GetCurrentChar( );
   if ( ch == '=' )
   {
      m_chPrev = ch;
      m_lIdx++;
      EatWhiteSpace( );
      ch = GetCurrentChar( );
      if ( (ch == '\'') || (ch == '\"') )
      {
         m_chParseDelimiter = ch;
         m_chPrev = ch;
         m_lIdx++;
         while ( (ch = GetCurrentChar( )) != m_chParseDelimiter )
         {
            m_csParseValue += ch;
            m_chPrev = ch;
            m_lIdx++;
         }

         m_chPrev = ch;
         m_lIdx++;
      }
      else
      {
         while ( Eof( ) == false &&
                 IsWhiteSpace( (ch = GetCurrentChar( )) ) == false &&
                 (ch != '>') )
         {
            m_csParseValue += ch;
            m_lIdx++;
         }
      }

      EatWhiteSpace( );
   }
}

// static int t = 0;

// Add a parsed attribute to the collection.  If the attribute already exists in the collection,
// the new attribute will supersede the previous attribute.
void
ZHTML_Parse::AddAttribute( )
{
   zSHORT nRC;

// t++;
// if ( t == 29 )
//    TraceLineI( "AddAttribute: ", t );

   ZHTML_Attribute *pAttribute = new ZHTML_Attribute( m_csParseName, m_csParseValue, m_chParseDelimiter );
   nRC = m_pTagAttributeList->AddAttribute( pAttribute );
   if ( nRC != 0 )
   {
      if ( nRC == zMIXSET_DUP )
      {
         TraceLine( "AddAttribute (DUP): 0x%08x  ParseName: %s   ParseValue: %s   Delimiter: %c",
                    pAttribute, (zCPCHAR) m_csParseName, (zCPCHAR) m_csParseValue, m_chParseDelimiter );
         ZHTML_Attribute *pA = (ZHTML_Attribute *) m_pTagAttributeList->SeekNode( &m_csParseName );
         pA->SetValue( pAttribute->GetValue( ) );
         delete pAttribute;
         return;
      }

      // We have an unknown error.
      TraceLine( "ZHTML_Parse::AddAttribute unknown error: %d   Attribute: 0x%08x   ParseName: %s   ParseValue: %s   Delimiter: %c",
                 nRC, pAttribute, (zCPCHAR) m_csParseName, (zCPCHAR) m_csParseValue, m_chParseDelimiter );
      delete pAttribute;
   }
}

// Get a few characters ahead of the current character.
// Parm: name nPeek - How many characters to peek ahead for.
// Returns - The character that was retrieved.
char
ZHTML_Parse::GetCurrentChar( zLONG lPeek )
{
   char ch;

   if ( (m_lIdx + lPeek) < m_lSrcLth )
     ch = m_csSource.GetAt( m_lIdx + lPeek );
   else
     ch = 0;

  return( ch );
}

// Obtain the next character and advance the index by one.
// Returns - The next character.
char
ZHTML_Parse::AdvanceCurrentChar( )
{
   char ch = GetCurrentChar( 0 );
   m_chPrev = ch;
   m_lIdx++;
   return( ch );
}

/////////////////////////////////////////////////////////////////////////////
// The ParseHTML Class
/////////////////////////////////////////////////////////////////////////////

ZHTML_ParseHTML::ZHTML_ParseHTML( ) :
                 ZHTML_Parse( )
{
   m_lFlags = 0;
}

ZHTML_ParseHTML::~ZHTML_ParseHTML( )
{
}

void
ZHTML_ParseHTML::ParseHTML_Page( CString& csPage, zCPCHAR cpcSearchAttribute )
{
   ZHTML_TagAttributeList *pTAL;
   ZHTML_Attribute *pAttribute;
   CString csSearch;
   CString cs;
   char    ch;

   if ( cpcSearchAttribute && cpcSearchAttribute[ 0 ] )
   {
      csSearch = cpcSearchAttribute;
   }
   else
   {
      cpcSearchAttribute = 0;
      csSearch = "";
   }

   SetSource( csPage );
   while ( Eof( ) == false )
   {
      pTAL = new ZHTML_TagAttributeList( );
      SetAttributeList( pTAL );
      m_TagList.AddTail( pTAL );
      ch = ParseHTML( );
      if ( ch == 0 )
      {
         if ( cpcSearchAttribute &&
              (pAttribute = (ZHTML_Attribute *) pTAL->SeekNode( &csSearch )) != 0 )
         {
            cs = pTAL->BuildTag( FALSE );
            TraceLine( "Found '%s': %s  in tag ====> %s",
                       cpcSearchAttribute, pAttribute->GetValue( ), cs );
         }
      // else
      // {
      //    cs = pTAL->BuildTag( FALSE );
      //    TraceLineS( "No src ==> ", cs );
      // }
      }
   }
}

char
ZHTML_ParseHTML::ParseHTML( )
{
   while ( TRUE )
   {
      if ( GetCurrentChar( ) == '<' )
      {
         char sz[ 2 ];

         Advance( );

         sz[ 0 ] = GetCurrentChar( );
         sz[ 1 ] = 0;
         CharLower( sz );
         if ( ((sz[ 0 ] >= 'a') && (sz[ 0 ] <= 'z')) || (sz[ 0 ] == '!') || (sz[ 0 ] == '/') )
         {
            ParseTag( );
            return( (char) 0 );
         }
         else
            return( AdvanceCurrentChar( ) );
      }
      else
      {
      // return( AdvanceCurrentChar( ) );  // originally this was it

         // If this is not just "whitespace", it must be the "value" of a tag.
         char ch = FindNextNonWhiteSpace( );
         if ( ch == '<' )
         {
            EatWhiteSpace( );
            continue;
         }
         else
         {
            ParseTagValue( );
            return( (char) 0 );
         }
      }
   }
}

void
ZHTML_ParseHTML::ParseTag( )
{
   char ch;
   int  nLth;

// m_pTagAttributeList->ClearInit( );
// m_pTagAttributeList->m_csTag = "";
   m_pTagAttributeList->m_bTag = TRUE;
// m_pTagAttributeList->m_nTagType = 0;

   // Is it a comment?
   if ( (GetCurrentChar( ) == '!') &&
        (GetCurrentChar( 1 ) == '-') &&
        (GetCurrentChar( 2 ) == '-') )
   {
      while ( Eof( ) == false )
      {
         ch = GetCurrentChar( );
         if ( (ch == '-') &&
              (GetCurrentChar( 1 ) == '-') &&
              (GetCurrentChar( 2 ) == '>') )
         {
            break;
         }

         if ( ch != '\r' )
            m_pTagAttributeList->m_csTag += ch;

         Advance( );
      }

      m_pTagAttributeList->m_csTag += "--";
      Advance( );
      Advance( );
      Advance( );
      m_chParseDelimiter = (char) 0;
      return;
   }

   // Find the tag name.
   while ( Eof( ) == false )
   {
      ch = GetCurrentChar( );
      if ( IsWhiteSpace( ch ) || (ch == '>') )
      {
         break;
      }

      if ( (ch == '>' && m_chPrev == '/') || (ch == '/' && m_chPrev == '<') )
         m_pTagAttributeList->m_nTagType |= 2;  // close

      m_pTagAttributeList->m_csTag += ch;
      Advance( );
   }

   EatWhiteSpace( );

   // Get the attributes.
   while ( GetCurrentChar( ) != '>' )
   {
      m_csParseName  = "";
      m_csParseValue = "";
      m_chParseDelimiter = (char) 0;

      ParseAttributeName( );

      if ( GetCurrentChar( ) == '>' )
      {
         if ( m_chPrev == '/' )
            m_pTagAttributeList->m_nTagType |= 2;  // close

         AddAttribute( );
         break;
      }

      // Get the value (if any).
      ParseAttributeValue( );
      AddAttribute( );
   }

   Advance( );

   // Determine type of tag ... open/close.
   nLth = m_pTagAttributeList->m_csTag.GetLength( );

   if ( nLth > 0 )
   {
      if ( m_pTagAttributeList->m_csTag[ 0 ] == '/' )
         m_pTagAttributeList->m_nTagType |= 2;  // close
      else
         m_pTagAttributeList->m_nTagType |= 1;  // open

      if ( m_pTagAttributeList->m_csTag[ nLth - 1 ] == '/' )
         m_pTagAttributeList->m_nTagType |= 2; // close
   }
}

// Parse Tag's value.
void
ZHTML_ParseHTML::ParseTagValue( )
{
   char ch;

   m_pTagAttributeList->ClearInit( );
// m_pTagAttributeList->m_csTag = "";
// m_pTagAttributeList->m_bTag = FALSE;
// m_pTagAttributeList->m_nTagType = 0;

   // Store the value of this tag.
   while ( Eof( ) == false )
   {
      ch = GetCurrentChar( );
      if ( (ch == '<') )
      {
         break;
      }

      m_pTagAttributeList->m_csTag += ch;
      Advance( );
   }
}

#if 1
#define fnCheckStack( a, b, c )
#else
void
fnCheckStack( zPCHAR cpcText, ZNameList& TagStack, zLONG lCnt )
{
   ZNameItem *pNameItem = TagStack.GetHead( );
   zLONG   lLevel = 0;
   CString csSpace = "                                                                                                    ";
   CString cs;

   TraceLine( "%4d - fnCheckStack: %s", lCnt, cpcText );
   while ( pNameItem )
   {
      lLevel++;
      cs = csSpace.Left( lLevel * 3 );
      TraceLineS( cs, pNameItem->m_csName );
      pNameItem = pNameItem->m_pNextName;
   }
}
#endif

// Remove the close (/) character and remove whitespace from the tag.
CString
fnTrimTag( CString& cs )
{
   CString csTag = cs;
   zLONG   lLth = csTag.GetLength( );
   if ( lLth > 0 && (csTag[ 0 ] == '/' || csTag[ lLth - 1 ] == '/') )
   {
      csTag.Remove( '/' );
      csTag.TrimLeft( );
      csTag.TrimRight( );
   }

   return( csTag );
}

zBOOL
fnIsTagInList( zCPCHAR cpcTagName, zPCHAR pchValidHTML_TagList )
{
   CString csTagName;
   CString cs = cpcTagName;
   zPCHAR  pchTag;
   zPCHAR  pchComma;
   zBOOL   bFound = FALSE;

   csTagName = fnTrimTag( cs );
   pchTag = pchValidHTML_TagList;
   while ( pchTag )
   {
      pchComma = zstrchr( pchTag, ',' );
      if ( pchComma )
         *pchComma = 0;

      if ( zstrcmp( csTagName.GetString(), pchTag ) == 0 )
      {
         if ( pchComma )
            *pchComma = ',';

         bFound = TRUE;
         break;
      }

      if ( pchComma )
      {
         *pchComma = ',';
         pchTag = pchComma + 1;
      }
      else
         pchTag = 0;
   }

   return( bFound );
}

// Compare the tags from the attribute list and the tag stack after
// trimming the close (/) tag character.  If they are the same,
// decrement the indentation level.
zLONG
fnCompareTags( ZHTML_TagAttributeList *pTAL, ZNameList& TagStack, zLONG& lLevel )
{
   CString csTag = fnTrimTag( pTAL->m_csTag );
   ZNameItem *pNameItem = TagStack.GetHead( );
   zLONG   lRC = 0;
   if ( pNameItem )
   {
      if ( pNameItem->m_csName.Compare( csTag ) != 0 )
      {
         TraceLine( "Error in Tag Stack: %s ... expected: %s",
                    (zCPCHAR) pNameItem->m_csName, (zCPCHAR) csTag );
      }

      lRC = pNameItem->m_lFlag;
      lLevel--;
      TagStack.DropHead( );  // dropping the one we just got using GetHead
      delete pNameItem;      // deleting the one that would be returned by DropHead
      fnCheckStack( "After Drop", TagStack, -1 );
   }
   else
   {
      TraceLine( "Error in Tag Stack: unmatched close: %s",
                 (zCPCHAR) csTag );
   }

   return( lRC );
}

// Add the specified tag to the list if not already in the list.  Add the
// specified attribute/value to the tag (override if already present).
// Returns number of tags found at level (does not include the one added).
zSHORT
ZHTML_ParseHTML::AddTagAndAttributeToList( CString& csTag, zCPCHAR cpcAttributeName, zCPCHAR cpcAttributeValue,
                                           char chDelimiter, zLONG lFlag )
{
   ZHTML_TagAttributeList *pTAL = 0;
   CString csAttributeName = cpcAttributeName;
   CString csAttributeValue = cpcAttributeValue;
   CString csTagName;
   zSHORT nOpen = 0;
   zSHORT nRC = 0;

   // First run through and locate the specified tag.
   ZPointerItem *pPointerItem = m_TagList.GetHead( );
   while ( pPointerItem )
   {
      pTAL = (ZHTML_TagAttributeList *) pPointerItem->m_pvPointer;

      if ( pTAL->m_bTag )
      {
         csTagName = fnTrimTag( pTAL->m_csTag );
         if ( csTag.Compare( csTagName ) == 0 )
         {
            if ( pTAL->m_nTagType & 1 )  // open
            {
               nOpen++;
               if ( nOpen == 1 )
               {
                  nRC++;
                  if ( cpcAttributeName && cpcAttributeName[ 0 ] )
                  {
                     pTAL->SetAttributeProperties( csAttributeName, csAttributeValue, chDelimiter, lFlag );
                  }
               }
            }

            if ( pTAL->m_nTagType & 2 )  // close
            {
               nOpen--;
            }
         }
      }

      pPointerItem = pPointerItem->m_pNextPointer;
   }

   if ( nRC == 0 )  // did not find one in list ... so add it
   {
      pTAL = new ZHTML_TagAttributeList( );
      pTAL->m_bTag = TRUE;
      pTAL->m_csTag = "/" + csTag;
      pTAL->m_nTagType = 2;  // close
      SetAttributeList( pTAL );
      m_TagList.AddTail( pTAL );  // add tail first

      pTAL = new ZHTML_TagAttributeList( );
      pTAL->m_bTag = TRUE;
      pTAL->m_csTag = csTag;
      pTAL->m_nTagType = 1;  // open
      SetAttributeList( pTAL );
      m_TagList.AddHead( pTAL );  // add head next so it is our working pTAL

      if ( cpcAttributeName && cpcAttributeName[ 0 ] )
      {
         pTAL->SetAttributeProperties( csAttributeName, csAttributeValue, chDelimiter, lFlag );
      }
   }

   return( nRC );
}

// Remove the first occurrence of the tag from the HTML text, and return it.
zSHORT
ZHTML_ParseHTML::RemoveTag( CString& csReturnTag, CString& csPage, CString& csTag )
{
   ZHTML_TagAttributeList *pTAL = 0;
   CString csTagName;
   CString csCurrentTagName;
   zBOOL  bTagFound = FALSE;
   zSHORT nFinishUp = 0;
   zSHORT nOpen = 0;
   zSHORT nRC = 0;

   csReturnTag = "";
   csPage = "";

   // Run through and locate the specified tag.  Then remove the tag from its open to close and
   // everything in between.  If the tag is contained by another tag, remove that tag from open
   // to close instead (which will also remove the specified tag).
   ZPointerItem *pPointerItem = m_TagList.GetHead( );
   while ( pPointerItem )
   {
      pTAL = (ZHTML_TagAttributeList *) pPointerItem->m_pvPointer;
      do  // purist's goto
      {
         if ( nFinishUp == 0 )
         {
            if ( pTAL->m_bTag )
            {
               csTagName = fnTrimTag( pTAL->m_csTag );

               if ( csCurrentTagName.IsEmpty( ) )
                  csCurrentTagName = csTagName;

               if ( csCurrentTagName.Compare( csTagName ) == 0 )
               {
                  if ( pTAL->m_nTagType & 1 )  // open
                  {
                     nOpen++;
                     if ( csTag.Compare( csTagName ) == 0 )
                     {
                        bTagFound = TRUE;
                        if ( nOpen == 1 )
                           nRC++;
                     }
                  }

                  if ( pTAL->m_nTagType & 2 )  // close
                     nOpen--;
               }
               else
               {
                  if ( csTag.Compare( csTagName ) == 0 )
                  {
                     if ( pTAL->m_nTagType & 1 )  // open
                        bTagFound = TRUE;
                  }
               }

               if ( bTagFound && nOpen == 0 )
               {
                  nFinishUp = 1;
                  break;  // exit purist's goto
               }
            }
         }

      } while ( FALSE );  // end purist's goto

      if ( nFinishUp == 2 )
      {
         csPage += pTAL->BuildTag( FALSE );
      }
      else
      {
         csReturnTag += pTAL->BuildTag( FALSE );
         if ( nFinishUp == 1 )
            nFinishUp = 2;
      }

      delete pTAL;
      m_TagList.DropHead( );  // drop the current pPointerItem out of the list
      delete pPointerItem;    // delete the current pPointerItem
      pPointerItem = m_TagList.GetHead( );
   }

   SetAttributeList( 0 );  // the attribute list has been "eaten up"
   return( nRC );
}

// Cleanup the current tag/attribute list and get the next tag/attribute list.
ZHTML_TagAttributeList *
ZHTML_ParseHTML::PopTagList( ZHTML_TagAttributeList *pTAL )
{
   if ( pTAL )
      delete pTAL;  // "eating up" the attribute list

   ZPointerItem *pPointerItem = m_TagList.DropHead( );
   if ( pPointerItem )
   {
      pTAL = (ZHTML_TagAttributeList *) pPointerItem->m_pvPointer;
      delete pPointerItem;
   }
   else
      pTAL = 0;

   return( pTAL );
}

// Add the current tag/attribute list's tag to the output string, cleanup the
// current tag/attribute list, and get the next tag/attribute list.
ZHTML_TagAttributeList *
ZHTML_ParseHTML::GetNextInTagList( CString& cs, ZHTML_TagAttributeList *pTAL, zPCHAR pchValidHTML_TagList )
{
   if ( pTAL )
   {
      if ( pTAL->m_bTag )
      {
         // m_lFlags: 0 - full HTML; 1 - strip invalid tags; 2 - strip properties
         if ( (m_lFlags & 1) == 0 || fnIsTagInList( pTAL->m_csTag, pchValidHTML_TagList ) )
            cs += pTAL->BuildTag( (m_lFlags & 2) ? TRUE : FALSE );
      }
      else
      {
         // m_lFlags: 0 - full HTML; 1 - strip invalid tags; 2 - strip properties
      // if ( (m_lFlags & 2) == 0 )
            cs += pTAL->BuildTag( FALSE );
      }

      delete pTAL;  // "eating up" the attribute list
   }

   ZPointerItem *pPointerItem = m_TagList.DropHead( );
   if ( pPointerItem )
   {
      pTAL = (ZHTML_TagAttributeList *) pPointerItem->m_pvPointer;
      delete pPointerItem;

#if 0  // debugging only
      ZHTML_Attribute *pAttribute;
      zLONG k;
      for ( k = 0; pAttribute = (ZHTML_Attribute *) pTAL->GetItem( k ); k++ ) // has attributes
      {
         if ( pAttribute->m_t == 29 )
            TraceLineS( "Attribute ", "29" );
      }
#endif
   }
   else
      pTAL = 0;

   return( pTAL );
}

// Right now only traces HTML for testing purposes ... could be put to a file or Blob.
void
ZHTML_ParseHTML::ReconstituteHTML( CString& csHTML,
                                   zCPCHAR cpcValidHTML_TagList,
                                   zLONG lFlags ) // 0 - full HTML; 1 - strip invalid tags; 2 - strip properties

{
   ZHTML_TagAttributeList *pTAL = 0;
   ZNameList TagStack( FALSE );  // tag stack (need not be unique)
   zLONG lLth = zstrlen( cpcValidHTML_TagList ) + 1;
   zPCHAR pchValidHTML_TagList = new char[ lLth ];
   CString csSpace = "                                                                                                    ";
   CString cs;
   zLONG   lLevel = 0;
   zLONG   lRC;  // just for debugging ... not used

   TraceLineS( "", "" );
   TraceLineS( "", "" );
   TraceLineS( "Original HTML ==========================================================> ", csHTML );

   m_lFlags = lFlags;
   strcpy_s( pchValidHTML_TagList, lLth, cpcValidHTML_TagList );
   csHTML = "";
   pTAL = GetNextInTagList( csHTML, pTAL, pchValidHTML_TagList );
   while ( pTAL )
   {
      if ( pTAL->m_nTagType & 2 )  // close
         lRC = fnCompareTags( pTAL, TagStack, lLevel );

      if ( pTAL->m_bTag && (pTAL->m_nTagType & 1) &&  // open tag
           ((m_lFlags & 1) == 0 || fnIsTagInList( pTAL->m_csTag, pchValidHTML_TagList )) )
      {
         cs = "";  // "\n" + csSpace.Left( lLevel * 3 );
      }
      else
         cs = "";

      // Determine if we are opening or closing a tag ... and alter level accordingly.
      if ( pTAL->m_bTag && pTAL->m_nTagType )
      {
         if ( pTAL->m_nTagType & 1 )  // open
         {
            TagStack.AddHead( pTAL->m_csTag );
            if ( (pTAL->m_nTagType & 2) == 0 )  // if it's not an open/close combination
               lLevel++;

            fnCheckStack( "After Open", TagStack, -1 );
         }
      }

      pTAL = GetNextInTagList( cs, pTAL, pchValidHTML_TagList );
      if ( pTAL )
      {
         // If it's not a tag, just add it (value of the tag) onto the output string and
         // then immediately try to add on the close of the tag.
         if ( pTAL->m_bTag == FALSE )
         {
            pTAL = GetNextInTagList( cs, pTAL, pchValidHTML_TagList );
            if ( pTAL )
            {
               if ( pTAL->m_bTag && (pTAL->m_nTagType == 2) )  // strictly a close
               {
                  lRC = fnCompareTags( pTAL, TagStack, lLevel );
                  pTAL = GetNextInTagList( cs, pTAL, pchValidHTML_TagList );
                  fnCheckStack( "After Value", TagStack, -1 );
               }
            }
         }
      }

   // TraceLineS( " | ", cs );
      csHTML += cs;
   }

   TraceLineS( "Reconstituted HTML =====================================================> ", csHTML );
   TraceLineS( "", "" );
   SetAttributeList( 0 );  // the attribute list has been "eaten up"
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

void
ZHTML_ParseHTML::CreateTagOI( ZHTML_TagAttributeList *pTAL,
                              zVIEW   vTgt,
                              zLONG   lLevel )
{
   ZHTML_Attribute *pAttribute;
   char    szDelimiter[ 2 ];
   zLONG   k;

   CreateEntity( vTgt, m_csTagEntity, zPOS_AFTER );
   SetAttributeFromString( vTgt, m_csTagEntity,
                           m_csTagName, pTAL->m_csTag );
   SetAttributeFromString( vTgt, m_csTagEntity,
                           m_csTagYN, pTAL->m_bTag ? "Y" : "N" );
   SetAttributeFromInteger( vTgt, m_csTagEntity,
                            m_csTagType, pTAL->m_nTagType );
   SetAttributeFromInteger( vTgt, m_csTagEntity,
                            m_csTagLevel, lLevel );

   szDelimiter[ 1 ] = 0;  // for changing char to string

   for ( k = 0; pAttribute = (ZHTML_Attribute *) pTAL->GetItem( k ); k++ ) // has attributes
   {
      CreateEntity( vTgt, m_csAttributeEntity, zPOS_AFTER );
      SetAttributeFromString( vTgt, m_csAttributeEntity,
                              m_csAttributeName, pAttribute->GetName( ) );
      SetAttributeFromString( vTgt, m_csAttributeEntity,
                              m_csAttributeValue, pAttribute->GetValue( ) );
      szDelimiter[ 0 ] = pAttribute->GetDelimiter( );
      SetAttributeFromString( vTgt, m_csAttributeEntity,
                              m_csAttributeDelimiter, szDelimiter );
   }
}

// Populate Zeidon OI from HTML.
void
ZHTML_ParseHTML::HTML_ToZeidonOI( zVIEW vTgt )
{
   ZHTML_TagAttributeList *pTAL = 0;
   ZHTML_Attribute *pAttribute;
   ZNameList TagStack( FALSE );  // tag stack (need not be unique)
   ZNameItem *pNameItem;
   CString cszid = "zid";
   CString cs;
   zBOOL   bCheck = TRUE; // need a Div entity to start things off
   zLONG   lLevel = 0;
   zLONG   lFlag;
   zLONG   lCnt = 0;

   while ( CheckExistenceOfEntity( vTgt, m_csDivEntity ) == 0 )
      DeleteEntity( vTgt, m_csDivEntity, zREPOS_FIRST );

   TraceLineS( "", "" );
   TraceLineS( "", "" );
   TraceLineS( "HTML to Zeidon OI", "==========================================================" );
   TraceLineS( "HTML to Zeidon OI", "==========================================================" );
   TraceLineS( "", "" );

   pTAL = PopTagList( pTAL );
   while ( pTAL )
   {
      lCnt++;
      pNameItem = 0;

      // Determine if we are opening or closing a tag ... and alter level accordingly.
      if ( pTAL->m_bTag && pTAL->m_nTagType )
      {
         if ( pTAL->m_nTagType & 1 )  // open
         {
            pNameItem = TagStack.AddHead( pTAL->m_csTag );
            lLevel++;
            fnCheckStack( "After Open", TagStack, lCnt );
         }
      }

      pAttribute = (ZHTML_Attribute *) pTAL->SeekNode( &cszid );
      if ( pAttribute )
      {
         bCheck = FALSE;
         if ( pNameItem )
            pNameItem->m_lFlag = 1; // we are on a generated div

         CreateEntity( vTgt, m_csDivEntity, zPOS_AFTER );
         SetAttributeFromString( vTgt, m_csDivEntity,
                                 m_csDivAttribute, pAttribute->GetValue( ) );
      }
      else
      {
         if ( bCheck && CheckExistenceOfEntity( vTgt, m_csDivEntity ) != 0 )
         {
            bCheck = FALSE;
            CreateEntity( vTgt, m_csDivEntity, zPOS_AFTER );
         // SetAttributeFromString( vTgt, m_csDivEntity,
         //                         m_csDivAttribute, ".NoDiv." );
         }

         CreateTagOI( pTAL, vTgt, lLevel );
         if ( pTAL->m_nTagType == 3 ) // open and close
            lLevel--;
      }

      if ( pTAL->m_nTagType == 2 )  // strictly a close
      {
         lFlag = fnCompareTags( pTAL, TagStack, lLevel );
         if ( lFlag == 1 )
         {
            CreateEntity( vTgt, m_csAttributeEntity, zPOS_AFTER );
            SetAttributeFromString( vTgt, m_csAttributeEntity,
                                    m_csAttributeName, "<zid>" );
         }
      }

      pTAL = PopTagList( pTAL );
      if ( pTAL )
      {
         // If it's not a tag, just add it (value of the tag) on to the output
         // string and then immediately try to add on the close of the tag.
         if ( pTAL->m_bTag == FALSE )
         {
            lLevel++;
            CreateTagOI( pTAL, vTgt, lLevel );
            pTAL = PopTagList( pTAL );
            lLevel--;
            if ( pTAL )
            {
               if ( pTAL->m_bTag && (pTAL->m_nTagType == 2) )  // strictly a close
               {
                  CreateTagOI( pTAL, vTgt, lLevel );
                  lFlag = fnCompareTags( pTAL, TagStack, lLevel );
                  if ( lFlag == 1 )
                  {
                     CreateEntity( vTgt, m_csAttributeEntity, zPOS_AFTER );
                     SetAttributeFromString( vTgt, m_csAttributeEntity,
                                             m_csAttributeName, "<zid>" );
                  }

                  pTAL = PopTagList( pTAL );
                  fnCheckStack( "After Value", TagStack, lCnt );
               }
            }
         }
      }
   }

   SetAttributeList( 0 );  // the attribute list has been "eaten up"
}

/////////////////////////////////////////////////////////////////////////////
//
// ParseHTML_TextToZeidonOI
//
//    cpcHTML               raw HTML
//    TgtView               target view into which to parse HTML
//    TgtDivEntity          target "div" entity
//    TgtDivAttribute       target "zid" attribute
//    TgtTagEntity          target HTML tag entity
//    TgtTagName            HTML tag entity name
//    TgtTagValue           HTML tag entity value
//    TgtTagLevel           HTML tag entity indentation level
//    TgtTagYN              Y ==> it is a tag   N ==> not a tag (comment ...)
//    TgtTagType            1 ==> start; 2 ==> end; 3 ==> both; 0 ==> not a tag/indeterminate
//    TgtAttributeEntity    target HTML tag attribute entity
//    TgtAttributeName      HTML tag attribute name
//    TgtAttributeValue     HTML tag attribute value
//    TgtAttributeDelimiter HTML tag attribute delimiter (", ', or null)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ParseHTML_TextToZeidonOI( zCPCHAR cpcHTML,
                          zVIEW   vHTML_TgtView,
                          zCPCHAR cpcHTML_TgtDivEntity,
                          zCPCHAR cpcHTML_TgtDivAttribute,
                          zCPCHAR cpcHTML_TgtTagEntity,
                          zCPCHAR cpcHTML_TgtTagName,
                          zCPCHAR cpcHTML_TgtTagValue,
                          zCPCHAR cpcHTML_TgtTagLevel,
                          zCPCHAR cpcHTML_TgtTagYN,
                          zCPCHAR cpcHTML_TgtTagType,
                          zCPCHAR cpcHTML_TgtAttributeEntity,
                          zCPCHAR cpcHTML_TgtAttributeName,
                          zCPCHAR cpcHTML_TgtAttributeValue,
                          zCPCHAR cpcHTML_TgtAttributeDelimiter )
{
   ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );
   ZNameList TagStack( FALSE );  // tag stack (FALSE ==> need not be unique)
   CString csPage;

   csPage = cpcHTML;
   pParseHTML->SetAttributeList( 0 );
   pParseHTML->ParseHTML_Page( csPage, 0 );

   // HTML to Zeidon OI.

   pParseHTML->m_csDivEntity = cpcHTML_TgtDivEntity;
   pParseHTML->m_csDivAttribute = cpcHTML_TgtDivAttribute;
   pParseHTML->m_csTagEntity = cpcHTML_TgtTagEntity;
   pParseHTML->m_csTagName = cpcHTML_TgtTagName;
   pParseHTML->m_csTagValue = cpcHTML_TgtTagValue;
   pParseHTML->m_csTagLevel = cpcHTML_TgtTagLevel;
   pParseHTML->m_csTagYN = cpcHTML_TgtTagYN;
   pParseHTML->m_csTagType = cpcHTML_TgtTagType;
   pParseHTML->m_csAttributeEntity = cpcHTML_TgtAttributeEntity;
   pParseHTML->m_csAttributeName = cpcHTML_TgtAttributeName;
   pParseHTML->m_csAttributeValue = cpcHTML_TgtAttributeValue;
   pParseHTML->m_csAttributeDelimiter = cpcHTML_TgtAttributeDelimiter;

   pParseHTML->HTML_ToZeidonOI( vHTML_TgtView );
   delete pParseHTML;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// ParseHTML_AttrToZeidonOI
//
//    SrcView               source view to get raw HTML
//    SrcEntity             source entity
//    SrcAttribute          source attribute containing raw HTML
//    TgtView               target view into which to parse HTML
//    TgtDivEntity          target "div" entity
//    TgtDivAttribute       target "zid" attribute
//    TgtTagEntity          target HTML tag entity
//    TgtTagName            HTML tag entity name
//    TgtTagValue           HTML tag entity value
//    TgtTagLevel           HTML tag entity indentation level
//    TgtTagYN              Y ==> it is a tag   N ==> not a tag (comment ...)
//    TgtTagType            1 ==> start; 2 ==> end; 3 ==> both; 0 ==> not a tag/indeterminate
//    TgtAttributeEntity    target HTML tag attribute entity
//    TgtAttributeName      HTML tag attribute name
//    TgtAttributeValue     HTML tag attribute value
//    TgtAttributeDelimiter HTML tag attribute delimiter (", ', or null)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ParseHTML_AttrToZeidonOI( zVIEW   vHTML_SrcView,
                          zCPCHAR cpcHTML_SrcEntity,
                          zCPCHAR cpcHTML_SrcAttribute,
                          zVIEW   vHTML_TgtView,
                          zCPCHAR cpcHTML_TgtDivEntity,
                          zCPCHAR cpcHTML_TgtDivAttribute,
                          zCPCHAR cpcHTML_TgtTagEntity,
                          zCPCHAR cpcHTML_TgtTagName,
                          zCPCHAR cpcHTML_TgtTagValue,
                          zCPCHAR cpcHTML_TgtTagLevel,
                          zCPCHAR cpcHTML_TgtTagYN,
                          zCPCHAR cpcHTML_TgtTagType,
                          zCPCHAR cpcHTML_TgtAttributeEntity,
                          zCPCHAR cpcHTML_TgtAttributeName,
                          zCPCHAR cpcHTML_TgtAttributeValue,
                          zCPCHAR cpcHTML_TgtAttributeDelimiter )
{
   zPCHAR  pchHTML;
   CString csPage;

   GetAddrForAttribute( &pchHTML, vHTML_SrcView, cpcHTML_SrcEntity, cpcHTML_SrcAttribute );
   if ( pchHTML[ 0 ] == 0 )
   {
      TraceLine( "ParseHTML_ToZeidon HTML is empty for Entity.Attribute: %s.%s",
                 cpcHTML_SrcEntity, cpcHTML_SrcAttribute );
   }
   else
   {
      TraceLine( "ParseHTML_ToZeidon HTML looking for 'zid' in HTML from Entity.Attribute: %s.%s Lth: %d ==> %s",
                 cpcHTML_SrcEntity, cpcHTML_SrcAttribute, zstrlen( pchHTML), pchHTML );
   }

   return( ParseHTML_TextToZeidonOI( pchHTML, vHTML_TgtView, cpcHTML_TgtDivEntity, cpcHTML_TgtDivAttribute,
                                     cpcHTML_TgtTagEntity, cpcHTML_TgtTagName, cpcHTML_TgtTagValue,
                                     cpcHTML_TgtTagLevel, cpcHTML_TgtTagYN, cpcHTML_TgtTagType,
                                     cpcHTML_TgtAttributeEntity, cpcHTML_TgtAttributeName, cpcHTML_TgtAttributeValue,
                                     cpcHTML_TgtAttributeDelimiter ) );
}

void
ZHTML_ParseHTML::ConstructTag( zVIEW vSrc, CString& csPage, CString& csLastTag )
{
   CString csSpace = "                                                                                                    ";
   CString csTag;
   CString cs;
   zPCHAR  pchTag;
   zPCHAR  pchValue;
   zPCHAR  pch;
   char    szDelimiter[ 2 ];
   zLONG   lLevel = 0;
   zLONG   lTagType;
   zSHORT  nRC;

   nRC = SetCursorFirstEntity( vSrc, m_csTagEntity, 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetIntegerFromAttribute( &lTagType, vSrc, m_csTagEntity, m_csTagType );
      GetAddrForAttribute( &pch, vSrc, m_csTagEntity, m_csTagYN );
      GetIntegerFromAttribute( &lLevel, vSrc, m_csTagEntity, m_csTagLevel );
      GetAddrForAttribute( &pchTag, vSrc, m_csTagEntity, m_csTagName );
      if ( pch[ 0 ] == 'Y' )
      {
         cs = pchTag;
         csTag = fnTrimTag( cs );
         if ( csTag.Compare( csLastTag ) == 0 )
         {
            csLastTag = "";
         }
         else
         {
            csPage += "\r\n";
            csPage += csSpace.Left( lLevel * 3 );
         }

         csPage += '<';
         csPage += pchTag;

         if ( lTagType & 1 )
         {
            nRC = SetCursorFirstEntity( vSrc, m_csAttributeEntity, 0 );
            while ( nRC == zCURSOR_SET )
            {
               GetStringFromAttribute( szDelimiter, sizeof( szDelimiter ), vSrc, m_csAttributeEntity, m_csAttributeDelimiter );
               GetAddrForAttribute( &pch, vSrc, m_csAttributeEntity, m_csAttributeName );
               GetAddrForAttribute( &pchValue, vSrc, m_csAttributeEntity, m_csAttributeValue );
               csPage += ' ';
               csPage += pch;
               if ( pchValue[ 0 ] )
               {
                  csPage += '=';
                  csPage += szDelimiter;
                  csPage += pchValue;
                  csPage += szDelimiter;
               }

               nRC = SetCursorNextEntity( vSrc, m_csAttributeEntity, 0 );
            }
         }

         csPage += '>';
         csLastTag = pchTag;
      }
      else
      {
         csPage += pchTag;
      }

      nRC = SetCursorNextEntity( vSrc, m_csTagEntity, 0 );
   }
}

// Reconstruct HTML from Zeidon OI.
CString
ZHTML_ParseHTML::ZeidonOI_ToHTML( zVIEW vSrc, CString& csPage )
{
   ZHTML_TagAttributeList *pTAL = 0;
   ZNameList TagStack( FALSE );  // tag stack (need not be unique)
   CString csTag;
   CString csLastTag = "div";
   zPCHAR  pch;
   zBOOL   bCheck = TRUE;
   zSHORT  nRC;

   TraceLineS( "", "" );
   TraceLineS( "", "" );
   TraceLineS( "Zeidon OI to HTML", "==========================================================" );
   TraceLineS( "Zeidon OI to HTML", "==========================================================" );
   TraceLineS( "", "" );

   nRC = SetCursorFirstEntity( vSrc, m_csDivEntity, 0 );
   while ( nRC == zCURSOR_SET )
   {
      GetAddrForAttribute( &pch, vSrc, m_csDivEntity, m_csDivAttribute );

      if ( csLastTag.Compare( "div" ) != 0 )
         csPage += "\r\n";

      csPage += "<div zid=\"";
      csPage += pch;
      csPage += "\" style=\"border:0px; margin:0px;\">";

      ConstructTag( vSrc, csPage, csLastTag );

   // csPage += "\r\n</div>";
      nRC = SetCursorNextEntity( vSrc, m_csDivEntity, 0 );
   }

   return( csPage );
}

/////////////////////////////////////////////////////////////////////////////
//
// ConstructHTML_TextFromZeidonOI
//
//    TgtView             target view for HTML
//    TgtEntity           target entity for HTML
//    TgtAttribute        target attribute for HTML
//    SrcView             source view into which to parse HTML
//    SrcDivEntity        source "div" entity
//    SrcDivAttribute     source "zid" attribute
//    SrcTagEntity        source HTML tag entity
//    SrcTagName          HTML tag entity name
//    SrcTagValue         HTML tag entity value
//    SrcTagLevel         HTML tag entity indentation level
//    SrcTagYN            Y ==> it is a tag   N ==> not a tag (comment ...)
//    SrcTagType          1 ==> start; 2 ==> end; 3 ==> both; 0 ==> not a tag/indeterminate
//    SrcAttributeEntity  source HTML tag attribute entity
//    SrcAttributeName    HTML tag attribute name
//    SrcAttributeValue   HTML tag attribute value
//    SrcAttributeDelimiter HTML tag attribute delimiter (", ', or null)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ConstructHTML_TextFromZeidonOI( zPCHAR  pchHTML_Text,
                                zULONG  ulMaxLth,
                                zVIEW   vHTML_SrcView,
                                zCPCHAR cpcHTML_SrcDivEntity,
                                zCPCHAR cpcHTML_SrcDivAttribute,
                                zCPCHAR cpcHTML_SrcTagEntity,
                                zCPCHAR cpcHTML_SrcTagName,
                                zCPCHAR cpcHTML_SrcTagValue,
                                zCPCHAR cpcHTML_SrcTagLevel,
                                zCPCHAR cpcHTML_SrcTagYN,
                                zCPCHAR cpcHTML_SrcTagType,
                                zCPCHAR cpcHTML_SrcAttributeEntity,
                                zCPCHAR cpcHTML_SrcAttributeName,
                                zCPCHAR cpcHTML_SrcAttributeValue,
                                zCPCHAR cpcHTML_SrcAttributeDelimiter )
{
   ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );
   ZNameList TagStack( FALSE );  // tag stack (need not be unique)
   CString csPage;
   zSHORT  nRC;

   // Zeidon OI to HTML.

   pParseHTML->m_csDivEntity = cpcHTML_SrcDivEntity;
   pParseHTML->m_csDivAttribute = cpcHTML_SrcDivAttribute;
   pParseHTML->m_csTagEntity = cpcHTML_SrcTagEntity;
   pParseHTML->m_csTagName = cpcHTML_SrcTagName;
   pParseHTML->m_csTagValue = cpcHTML_SrcTagValue;
   pParseHTML->m_csTagLevel = cpcHTML_SrcTagLevel;
   pParseHTML->m_csTagYN = cpcHTML_SrcTagYN;
   pParseHTML->m_csTagType = cpcHTML_SrcTagType;
   pParseHTML->m_csAttributeEntity = cpcHTML_SrcAttributeEntity;
   pParseHTML->m_csAttributeName = cpcHTML_SrcAttributeName;
   pParseHTML->m_csAttributeValue = cpcHTML_SrcAttributeValue;
   pParseHTML->m_csAttributeDelimiter = cpcHTML_SrcAttributeDelimiter;

   pParseHTML->ZeidonOI_ToHTML( vHTML_SrcView, csPage );

   strncpy_s( pchHTML_Text, ulMaxLth, csPage, ulMaxLth - 1 );
   if ( ulMaxLth < (zULONG) csPage.GetLength( ) )
      nRC = 1;
   else
      nRC = 0;

   // Remove these lines prior to deployment!!!
   TraceLineS( "ConstructHTML_FromZeidonOI: ", csPage );

   zLONG lFileHTML = SysOpenFile( vHTML_SrcView, "c:\\temp\\test.html", COREFILE_WRITE );
   SysWriteFile( vHTML_SrcView, lFileHTML, csPage, csPage.GetLength( ) );
   SysCloseFile( vHTML_SrcView, lFileHTML, 0 );
   // End of: Remove these lines prior to deployment!!!

   delete pParseHTML;
   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// ConstructHTML_AttrFromZeidonOI
//
//    TgtView             target view for HTML
//    TgtEntity           target entity for HTML
//    TgtAttribute        target attribute for HTML
//    SrcView             source view into which to parse HTML
//    SrcDivEntity        source "div" entity
//    SrcDivAttribute     source "zid" attribute
//    SrcTagEntity        source HTML tag entity
//    SrcTagName          HTML tag entity name
//    SrcTagValue         HTML tag entity value
//    SrcTagLevel         HTML tag entity indentation level
//    SrcTagYN            Y ==> it is a tag   N ==> not a tag (comment ...)
//    SrcTagType          1 ==> start; 2 ==> end; 3 ==> both; 0 ==> not a tag/indeterminate
//    SrcAttributeEntity  source HTML tag attribute entity
//    SrcAttributeName    HTML tag attribute name
//    SrcAttributeValue   HTML tag attribute value
//    SrcAttributeDelimiter HTML tag attribute delimiter (", ', or null)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ConstructHTML_AttrFromZeidonOI( zVIEW   vHTML_TgtView,
                                zCPCHAR cpcHTML_TgtEntity,
                                zCPCHAR cpcHTML_TgtAttribute,
                                zVIEW   vHTML_SrcView,
                                zCPCHAR cpcHTML_SrcDivEntity,
                                zCPCHAR cpcHTML_SrcDivAttribute,
                                zCPCHAR cpcHTML_SrcTagEntity,
                                zCPCHAR cpcHTML_SrcTagName,
                                zCPCHAR cpcHTML_SrcTagValue,
                                zCPCHAR cpcHTML_SrcTagLevel,
                                zCPCHAR cpcHTML_SrcTagYN,
                                zCPCHAR cpcHTML_SrcTagType,
                                zCPCHAR cpcHTML_SrcAttributeEntity,
                                zCPCHAR cpcHTML_SrcAttributeName,
                                zCPCHAR cpcHTML_SrcAttributeValue,
                                zCPCHAR cpcHTML_SrcAttributeDelimiter )
{
   ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );
   ZNameList TagStack( FALSE );  // tag stack (need not be unique)
   CString csPage;
   zPCHAR  pchHTML_Text;
   zULONG  ulLth;
   zSHORT  nRC;

   GetAttributeLength( &ulLth, vHTML_TgtView, cpcHTML_TgtEntity, cpcHTML_TgtAttribute );
   pchHTML_Text = new char[ ulLth + 1 ];
   nRC = ConstructHTML_TextFromZeidonOI( pchHTML_Text, ulLth,
                                         vHTML_SrcView,
                                         cpcHTML_SrcDivEntity,
                                         cpcHTML_SrcDivAttribute,
                                         cpcHTML_SrcTagEntity,
                                         cpcHTML_SrcTagName,
                                         cpcHTML_SrcTagValue,
                                         cpcHTML_SrcTagLevel,
                                         cpcHTML_SrcTagYN,
                                         cpcHTML_SrcTagType,
                                         cpcHTML_SrcAttributeEntity,
                                         cpcHTML_SrcAttributeName,
                                         cpcHTML_SrcAttributeValue,
                                         cpcHTML_SrcAttributeDelimiter );

   SetAttributeFromString( vHTML_TgtView, cpcHTML_TgtEntity, cpcHTML_TgtAttribute, pchHTML_Text );

   delete [] pchHTML_Text;
   delete pParseHTML;
   return( nRC );
}

// Load the next "attribute" from the line (read from the CSV "file").
zPCHAR
fnGetNextAttribute( zPCHAR pchLine, CString& csAttribute, zLONG lLineNbr )
{
   zPCHAR pchComma;
   zPCHAR pchQuote;
   zPCHAR pchEnd;
   zCHAR  chDelimiter;

   if ( pchLine == 0 )
   {
      csAttribute = "";
   }
   else
   if ( *pchLine == '"' || *pchLine == '\'' )
   {
      chDelimiter = *pchLine;
      pchLine++;
      pchQuote = strchr( pchLine, chDelimiter );
      pchComma = pchQuote + 1;
      if ( pchQuote == 0 || (*pchComma && *pchComma != ',') )
      {
         // Complain if an error happened.
         TraceLine( "CSV Quote/Comma Error at line %d parsing CSV value: %s", lLineNbr, pchLine );
         return( 0 );
      }

      *pchQuote = 0;
      if ( *pchComma )
         pchEnd = pchComma + 1;
      else
         pchEnd = 0;  // no more left to parse

      csAttribute = pchLine;
   }
   else
   {
      pchComma = strchr( pchLine, ',' );
      if ( pchComma )
      {
         *pchComma = 0;
         pchEnd = pchComma + 1;
      }
      else
         pchEnd = 0;  // no more left to parse

      csAttribute = pchLine;
   }

   return( pchEnd );  // if zero, nothing left to parse
}

/////////////////////////////////////////////////////////////////////////////
//
// Parse list of entity.attribute=value.context specifications.  If only the
// attribute is specified, no "dot" separators are necessary.  If only
// entity.attribute is specified, the second "dot" separator may be omitted.
// If the default entity is to be used and a context is to be specified as
// well, the specification must be as: .attribute.context
//
/////////////////////////////////////////////////////////////////////////////
void
fnSetEntityAttribList( ZNameList *pEntityStack,
                       ZNameList *pAttribStack,
                       ZNameList *pValueStack,
                       ZNameList *pContextStack,
                       zCPCHAR    cpcDefaultEntity,
                       zPCHAR     pchEA )
{
   zPCHAR pch;
   zPCHAR pchDot1;
   zPCHAR pchDot2;
   zPCHAR pchEqual;
   zPCHAR pchComma;

   // So, the choices are:
   //   entity.attribute.context, ... two dots
   //   .attribute.context, ...       two dots (use default entity)
   //   entity.attribute, ...         one dot  (no context)
   //   attribute, ...                no dots  (use default entity, no context)

   pch = pchEA;
   while ( pch && *pch )
   {
      pchDot1 = zstrchr( pch, '.' );
      pchComma = zstrchr( pch, ',' );
      if ( pchDot1 )
      {
         pchDot2 = zstrchr( pchDot1 + 1, '.' );
         if ( pchComma )
         {
            if ( pchDot1 > pchComma )
            {
               pchDot1 = 0;
               pchDot2 = 0;
            }
            else
            {
               if ( pchDot2 > pchComma )
                  pchDot2 = 0;
            }
         }
      }
      else
      {
         pchDot2 = 0;
      }

      if ( pchDot1 )
         *pchDot1 = 0;

      if ( pchDot2 )
         *pchDot2 = 0;

      if ( pchComma )
         *pchComma = 0;

      if ( pchDot1 )
      {
         pEntityStack->AddTail( pch );
         pchEqual = zstrchr( pchDot1 + 1, '=' );
         if ( pchEqual )
            *pchEqual = 0;

         pAttribStack->AddTail( pchDot1 + 1 );
         if ( pchEqual )
            pValueStack->AddTail( pchEqual + 1, 1 );
         else
            pValueStack->AddTail( "", 0 );

         if ( pchDot2 )
            pContextStack->AddTail( pchDot2 + 1 );
         else
            pContextStack->AddTail( "" );
      }
      else
      {
         pEntityStack->AddTail( cpcDefaultEntity );
         pchEqual = zstrchr( pch, '=' );
         if ( pchEqual )
            *pchEqual = 0;

         pAttribStack->AddTail( pch );
         if ( pchEqual )
            pValueStack->AddTail( pchEqual + 1, 1 );
         else
            pValueStack->AddTail( "", 0 );

         pContextStack->AddTail( "" );
      }

      if ( pchEqual )
         *pchEqual = '=';

      if ( pchDot1 )
         *pchDot1 = '.';

      if ( pchDot2 )
         *pchDot2 = '.';

      if ( pchComma )
      {
         *pchComma = ',';
         pch = pchComma + 1;
      }
      else
         pch = 0;
   }
}

/////////////////////////////////////////////////////////////////////////////
//
// ImportCSV_ToZeidonOI
//
//    TgtView             target view
//    CSV FileName        file name of CSV data
//
// The first line of the CSV file is expected to have entity.attribute
// pairs (separated by commas) for each attribute in the CSV.  The first
// line is followed by comma-separated attribute lines.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
ImportCSV_ToZeidonOI( zVIEW   vTgt,
                      zCPCHAR cpcCSV_FileName )
{
   CString csAttribute;
   ZNameList EntityStack( FALSE );  // tag stack (need not be unique)
   ZNameList AttribStack( FALSE );  // tag stack (need not be unique)
   ZNameList ValueStack( FALSE );   // tag stack (need not be unique)
   ZNameList ContextStack( FALSE ); // tag stack (need not be unique)
   ZNameItem *pContextItem;
   ZNameItem *pEntityItem;
   ZNameItem *pValueItem;
   ZNameItem *pAttribItem;
   zPCHAR pchLine;
   zPCHAR pchNext;
   zLONG  hFileCSV;
   zLONG  lLineNbr;
   zSHORT nRC;

   hFileCSV = SysOpenFile( vTgt, cpcCSV_FileName, COREFILE_READ );
   if ( hFileCSV == -1 )
      return( -1 );

   nRC = SysReadLine( vTgt, (zCOREMEM) &pchLine, hFileCSV );
   if ( nRC != 1 )
      return( -2 );  // entity.attribute header line not read successfully

   // Even though we don't use the Context here, set up for it.
   fnSetEntityAttribList( &EntityStack, &AttribStack, &ValueStack, &ContextStack, "", pchLine );
   if ( EntityStack.GetHead( ) == 0 || AttribStack.GetHead( ) == 0 )
      return( -3 );  // entity.attribute header line not well-formed

   lLineNbr = 1;     // account for entity.attribute header line
   nRC = SysReadLine( vTgt, (zCOREMEM) &pchLine, hFileCSV );
   while ( nRC == 1 )
   {
      lLineNbr++;
      if ( pchLine && pchLine[ 0 ] )
      {
         pEntityItem = EntityStack.GetHead( );
         pAttribItem = AttribStack.GetHead( );
         pValueItem = ValueStack.GetHead( );
         pContextItem = ContextStack.GetHead( );
         CreateEntity( vTgt, pEntityItem->m_csName, zPOS_LAST );
         pchNext = pchLine;
         while ( pAttribItem )
         {
            pchNext = fnGetNextAttribute( pchNext, csAttribute, lLineNbr );
            if ( CheckExistenceOfEntity( vTgt, pEntityItem->m_csName ) != 0 )
               CreateEntity( vTgt, pEntityItem->m_csName, zPOS_LAST );

            SetAttributeFromString( vTgt, pEntityItem->m_csName, pAttribItem->m_csName, csAttribute );
            pEntityItem = pEntityItem->m_pNextName;
            pAttribItem = pAttribItem->m_pNextName;
            pValueItem = pValueItem->m_pNextName;
            pContextItem = pContextItem->m_pNextName;
            if ( pchNext == 0 || pchNext[ 0 ] == 0 )
               break;
         }
      }

      nRC = SysReadLine( vTgt, (zCOREMEM) &pchLine, hFileCSV );
   }

   SysCloseFile( vTgt, hFileCSV, 0 );
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// BuildCSV_FromEntityAttribute
//
//    TgtView             target view
//    TgtEntity           target entity
//    TgtAttribute        target attribute (text or blob)
//    SrcView             source view
//    SrcEAC              source entity.attribute.context (list)
//    lFlag               0-<crlf>  1-<p></p>
//
// The SrcEAC parameter contains entity.attribute.context triples (separated
// by commas if more than one) for each attribute to be set in the CSV.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BuildCSV_FromEntityAttribute( zVIEW   vTgt,
                              zCPCHAR cpcTgtEntity,
                              zCPCHAR cpcTgtAttribute,
                              zVIEW   vSrc,
                              zCPCHAR cpcSrcListEntityScope,
                              zCPCHAR cpcSrcEAC,
                              zLONG   lFlag )
{
   ZNameList EntityStack( FALSE );  // tag stack (need not be unique)
   ZNameList AttribStack( FALSE );  // tag stack (need not be unique)
   ZNameList ValueStack( FALSE );   // tag stack (need not be unique)
   ZNameList ContextStack( FALSE ); // tag stack (need not be unique)
   ZNameItem *pEntityItem;
   ZNameItem *pAttribItem;
   ZNameItem *pValueItem;
   ZNameItem *pContextItem;
   zPCHAR pchSrcListEntity;
   zPCHAR pchSrcListScope;
   zPCHAR pch;
   zULONG ulCurrLth;
   zULONG ulMaxLth;
   zSHORT nRC;

   pchSrcListEntity = new char[ zstrlen( cpcSrcListEntityScope ) + 1 ];
   strcpy_s( pchSrcListEntity, zTAG_LTH, cpcSrcListEntityScope );
   pchSrcListScope = zstrchr( pchSrcListEntity, '.' );
   if ( pchSrcListScope )
   {
      *pchSrcListScope = 0;
      pchSrcListScope++;
   }

   zLONG lLth = zstrlen( cpcSrcEAC ) + 1;
   pch = new char[ lLth ];
   strcpy_s( pch, lLth, cpcSrcEAC );
   fnSetEntityAttribList( &EntityStack, &AttribStack, &ValueStack, &ContextStack, pchSrcListEntity, pch );
   delete [] pch;

   if ( EntityStack.GetHead( ) == 0 || AttribStack.GetHead( ) == 0 )
      return( -3 );  // entity.attribute.context parameter not well-formed

   GetAttributeLength( &ulMaxLth, vTgt, cpcTgtEntity, cpcTgtAttribute );
   pch = new char[ ulMaxLth + 16 ];  // extra room for adding <crlf> or <p></p>
   pch[ 0 ] = 0; // initialize to empty

   ulCurrLth = 0;
   nRC = SetCursorFirstEntity( vSrc, pchSrcListEntity, pchSrcListScope );
   while ( nRC >= zCURSOR_SET )
   {
      pEntityItem = EntityStack.GetHead( );
      pAttribItem = AttribStack.GetHead( );
      pValueItem = ValueStack.GetHead( );
      pContextItem = ContextStack.GetHead( );
      while ( pAttribItem )
      {
         if ( lFlag & 0x00000001 )
         {
            pch[ ulCurrLth++ ] = '<';
            pch[ ulCurrLth++ ] = 'p';
            pch[ ulCurrLth++ ] = '>';
         }

         GetStringFromAttributeByContext( pch + ulCurrLth, vSrc,
                                          pEntityItem->m_csName,
                                          pAttribItem->m_csName,
                                          pContextItem->m_csName,
                                          ulMaxLth - ulCurrLth );
         ulCurrLth += zstrlen( pch + ulCurrLth );
         pEntityItem = pEntityItem->m_pNextName;
         pAttribItem = pAttribItem->m_pNextName;
         pValueItem = pValueItem->m_pNextName;
         pContextItem = pContextItem->m_pNextName;
         if ( pAttribItem )
         {
            pch[ ulCurrLth++ ] = ',';
            pch[ ulCurrLth++ ] = ' ';
         }
         else
         {
            if ( lFlag & 0x00000001 )
            {
               pch[ ulCurrLth++ ] = '<';
               pch[ ulCurrLth++ ] = '/';
               pch[ ulCurrLth++ ] = 'p';
               pch[ ulCurrLth++ ] = '>';
            }
            else
            {
               pch[ ulCurrLth++ ] = '\r';
               pch[ ulCurrLth++ ] = '\n';
            }
         }

         pch[ ulCurrLth ] = 0;

         if ( ulCurrLth >= ulMaxLth )
         {
            break;
         }
      }

      if ( ulCurrLth >= ulMaxLth )
         nRC = zCALL_ERROR;
      else
         nRC = SetCursorNextEntity( vSrc, pchSrcListEntity, pchSrcListScope );
   }

   SetAttributeFromString( vTgt, cpcTgtEntity, cpcTgtAttribute, pch );
   delete [] pch;
   delete [] pchSrcListEntity;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// BuildEntityAttributeFromCSV
//
//    SrcView             source view
//    SrcEntity           source entity
//    SrcAttribute        source attribute (text or blob)
//    TgtView             target view
//    TgtEAC              target entity.attribute=value.context (list)
//    lFlag               0-<crlf>  1-<p></p>
//
// The TgtEAC parameter contains entity.attribute=value.context triples (separated
// by commas if more than one) for each attribute to be set from the CSV.  If a
// value is specified, it is a constant value for the given attribute.
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
BuildEntityAttributeFromCSV( zVIEW   vSrc,
                             zCPCHAR cpcSrcEntity,
                             zCPCHAR cpcSrcAttribute,
                             zVIEW   vTgt,
                             zCPCHAR cpcTgtListEntity,
                             zCPCHAR cpcTgtEAC,
                             zLONG   lFlag )
{
   CString csAttribute;
   ZNameList EntityStack( FALSE );  // tag stack (need not be unique)
   ZNameList AttribStack( FALSE );  // tag stack (need not be unique)
   ZNameList ValueStack( FALSE );   // tag stack (need not be unique)
   ZNameList ContextStack( FALSE ); // tag stack (need not be unique)
   ZNameItem *pEntityItem;
   ZNameItem *pAttribItem;
   ZNameItem *pValueItem;
   ZNameItem *pContextItem;
   zPCHAR pch;
   zPCHAR pchLine;  // pointer to move through pch which holds the SrcAttribute containing the CSV ... NO!!!
   zPCHAR pchNext;
   zPCHAR pchNewline;
   zULONG ulCurrLth;
   zULONG ulMaxLth;
   zLONG  lLineNbr;

   zLONG lLth = zstrlen( cpcTgtEAC ) + 1;
   pch = new char[ lLth ];
   strcpy_s( pch, lLth, cpcTgtEAC );
   fnSetEntityAttribList( &EntityStack, &AttribStack, &ValueStack, &ContextStack, cpcTgtListEntity, pch );
   delete [] pch;

   if ( EntityStack.GetHead( ) == 0 || AttribStack.GetHead( ) == 0 )
      return( -3 );  // entity.attribute.context parameter not well-formed

   GetAttributeLength( &ulMaxLth, vSrc, cpcSrcEntity, cpcSrcAttribute );
   pch = new char[ ulMaxLth + 1 ];
   GetStringFromAttribute( pch, ulMaxLth + 1, vSrc, cpcSrcEntity, cpcSrcAttribute );
   ulMaxLth = zstrlen( pch );  // get true length

   lLineNbr = 0;
   ulCurrLth = 0; // / lLineNbr;  // divide by lLineNbr to cause a bomb to let me know when we get here
   pchLine = pch;
   while ( ulCurrLth < ulMaxLth )
   {
      lLineNbr++;
      if ( lFlag & 0x00000001 )
      {
         pchNewline = zstrstr( pchLine + ulCurrLth, "</p>" );
         pchLine += 4;
      }
      else
      {
         pchNewline = zstrstr( pchLine + ulCurrLth, "\r\n" );
      }

      if ( pchNewline )
         *pchNewline = 0;

      pchNext = pchLine + ulCurrLth;
      ulCurrLth += zstrlen( pchLine + ulCurrLth );
      pEntityItem = EntityStack.GetHead( );
      pAttribItem = AttribStack.GetHead( );
      pValueItem = ValueStack.GetHead( );
      pContextItem = ContextStack.GetHead( );
      CreateEntity( vTgt, pEntityItem->m_csName, zPOS_LAST );

      while ( pAttribItem )
      {
         if ( CheckExistenceOfEntity( vTgt, pEntityItem->m_csName ) != 0 )
            CreateEntity( vTgt, pEntityItem->m_csName, zPOS_LAST );

         if ( pValueItem->m_lFlag )
            SetAttributeFromString( vTgt, pEntityItem->m_csName, pAttribItem->m_csName, pValueItem->m_csName );
         else
         {
            pchNext = fnGetNextAttribute( pchNext, csAttribute, lLineNbr );
            SetAttributeFromString( vTgt, pEntityItem->m_csName, pAttribItem->m_csName, csAttribute );
         }

         pEntityItem = pEntityItem->m_pNextName;
         pAttribItem = pAttribItem->m_pNextName;
         pValueItem = pValueItem->m_pNextName;
         pContextItem = pContextItem->m_pNextName;
      }

      if ( pchNewline )
      {
         if ( lFlag & 0x00000001 )
         {
            *pchNewline = '<';
            ulCurrLth +=4;
         }
         else
         {
            *pchNewline = '\r';
            ulCurrLth += 2;
         }
      }
   }

   delete [] pch;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// RemoveFormattingFromHTML_Attr
//
//    vHTML                 HTML View
//    cpcHTML_Entity        HTML entity
//    cpcHTML_Attribute     HTML attribute
//    cpcValidHTML_TagList  Valid HTML tags (comma separated list that will
//                          not be removed)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
RemoveFormattingFromHTML_Attr( zVIEW   vHTML,
                               zCPCHAR cpcHTML_Entity,
                               zCPCHAR cpcHTML_Attribute,
                               zCPCHAR cpcValidHTML_TagList )
{
   zPCHAR pchHTML;
   zLONG  lLth;

   GetAddrForAttribute( &pchHTML, vHTML, cpcHTML_Entity, cpcHTML_Attribute );
   lLth = zstrlen( pchHTML );
   zPCHAR pchUnformattedHTML = new char[ lLth + 1 ];
   strcpy_s( pchUnformattedHTML, lLth + 1, pchHTML );
   RemoveFormattingFromHTML_Text( vHTML, pchUnformattedHTML, lLth, cpcValidHTML_TagList );
   SetAttributeFromString( vHTML, cpcHTML_Entity, cpcHTML_Attribute, pchUnformattedHTML );

   delete [] pchUnformattedHTML;
   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// RemoveFormattingFromHTML_Text
//
//    vTaskView             Task View
//    pchHTML_Text          Text potentially containing HTML tags
//    lMaxLth               Maximum length of HTML text
//    cpcValidHTML_TagList  Valid HTML tags (comma separated list of tags
//                          that will not be removed)
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
RemoveFormattingFromHTML_Text( zVIEW   vTaskView,
                               zPCHAR  pchHTML_Text,
                               zLONG   lMaxLth,
                               zCPCHAR cpcValidHTML_TagList )
{
   CString csPage;
   zLONG   lLth;

   csPage = pchHTML_Text;
   if ( csPage.IsEmpty( ) )
   {
      TraceLine( "RemoveFormattingFromHTML_Text HTML is empty" );
   }
   else
   {
      ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );

      TraceLine( "RemoveFormattingFromHTML_Text length: %d   HTML: %s", csPage.GetLength( ), csPage );
      pParseHTML->ParseHTML_Page( csPage, "" );
      pParseHTML->ReconstituteHTML( csPage, cpcValidHTML_TagList, 3 );
      delete pParseHTML;
   }

   lLth = csPage.GetLength( );
   if ( lLth > lMaxLth )
   {
      TraceLine( "RemoveFormattingFromHTML length: %d  exceeded maximum: %d", lLth, lMaxLth );
      TraceLineS( "RemoveFormattingFromHTML original: ", pchHTML_Text );
      TraceLineS( "RemoveFormattingFromHTML reformat: ", csPage );
   }
   else
      strcpy_s( pchHTML_Text, lMaxLth, csPage );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
//
// AddHTML_TagAttributes
//    If the tag does not already exist in the HTML text, add the tag as
//    the "outermost" tag.  Add/replace attributes to the tag.
//
//    vTaskView              Task View
//    pchHTML_Text           HTML text
//    lMaxLth                Maximum length of HTML text
//    cpcHTML_Tag            HTML tag
//    cpcHTML_AttributeName  Attribute name to set for tag
//    cpcHTML_AttributeValue Value of attribute
//    cpcDelimiter           Delimiter to wrap attribute
//    lFlags                 0 - replace value; 1 - append value
//
// Returns:  -1 - error
//          otw - number of tags found at level (does not include the one added).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
AddHTML_TagAttribute( zVIEW   vTaskView,
                      zPCHAR  pchHTML_Text,
                      zLONG   lMaxLth,
                      zCPCHAR cpcHTML_Tag,
                      zCPCHAR cpcHTML_AttributeName,
                      zCPCHAR cpcHTML_AttributeValue,
                      zCPCHAR cpcDelimiter,
                      zLONG   lFlags )
{
   CString csPage( pchHTML_Text );
   CString csTag( cpcHTML_Tag );
   ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );
   zLONG   lLth;
   zSHORT  nRC;

   TraceLine( "AddHTML_TagAttribute Tag: %s   Attribute Name: %s   Attribute Value: %s   Length: %d   HTML: %s",
              cpcHTML_Tag, cpcHTML_AttributeName, cpcHTML_AttributeValue, csPage.GetLength( ), csPage );
   pParseHTML->ParseHTML_Page( csPage, "" );
   nRC = pParseHTML->AddTagAndAttributeToList( csTag, cpcHTML_AttributeName,
                                               cpcHTML_AttributeValue, cpcDelimiter[ 0 ], lFlags );

   pParseHTML->ReconstituteHTML( csPage, "", 0 ); // Flags 0 ==> full HTML

   delete pParseHTML;

   lLth = csPage.GetLength( );
   if ( lLth > lMaxLth )
   {
      TraceLine( "AddHTML_TagAttribute length: %d  exceeded maximum: %d", lLth, lMaxLth );
      TraceLineS( "AddHTML_TagAttribute original: ", pchHTML_Text );
      TraceLineS( "AddHTML_TagAttribute reformat: ", csPage );
      nRC = -1;
   }
   else
   {
      // Things are good.  Send back the revised HTML.
      strcpy_s( pchHTML_Text, lMaxLth, csPage );
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
// RemoveTagFromHTML
//    Remove the tag from the HTML text, and return it.
//
//    vTaskView              Task View
//    pchReturnTag           Return tag
//    pchHTML_Text           HTML text
//    cpcTag                 HTML tag to return
//
// Returns:  -1 - error
//          otw - number of tags found at level (does not include the one added).
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT OPERATION
RemoveTagFromHTML( zVIEW   vTaskView,
                   zPCHAR  pchReturnTag,
                   zPCHAR  pchHTML_Text,
                   zCPCHAR cpcTag )
{
   CString csPage( pchHTML_Text );
   CString csTag( cpcTag );
   CString csReturnTag;
   ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );

   pchReturnTag[ 0 ] = 0;
   TraceLine( "RemoveTagFromHTML Tag: %s   HTML: %s", cpcTag, csPage );
   pParseHTML->ParseHTML_Page( csPage, "" );
   pParseHTML->RemoveTag( csReturnTag, csPage, csTag );

   delete pParseHTML;

   // Things are good.  Send back the revised HTML.
   strcpy_s( pchReturnTag, zTAG_LTH, csReturnTag );
   zLONG lLth = zstrlen( pchHTML_Text ) + 1;
   strcpy_s( pchHTML_Text, lLth, csPage );

   return( 0 );
}

/////////////////////////////////////////////////////////////////////////////
// The FindLinks Test Operation
/////////////////////////////////////////////////////////////////////////////

zOPER_EXPORT void OPERATION
FindLinks( zVIEW vApp, zCPCHAR cpcEntity, zCPCHAR cpcAttribute )
{
   zPCHAR  pchHTML;
   CString csPage;

   GetAddrForAttribute( &pchHTML, vApp, cpcEntity, cpcAttribute );
   csPage = pchHTML;
   if ( csPage.IsEmpty( ) )
   {
      TraceLine( "FindLinks HTML is empty for Entity.Attribute: %s.%s",
                 cpcEntity, cpcAttribute );
   }
   else
   {
      ZHTML_ParseHTML *pParseHTML = new ZHTML_ParseHTML( );

      TraceLine( "FindLinks HTML looking for 'src' in HTML from Entity.Attribute: %s.%s",
                 cpcEntity, cpcAttribute );
      pParseHTML->ParseHTML_Page( csPage, "src" );
      pParseHTML->ReconstituteHTML( csPage, "", 0 );
      TraceLineS( "FindLinks HTML: ", csPage );
      delete pParseHTML;
   }
}
