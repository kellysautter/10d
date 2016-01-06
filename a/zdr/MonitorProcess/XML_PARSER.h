/////////////////////////////////////////////////////////////////////////////
// Name:        XML_PARSER.h
// Version:     2.0
// Purpose:     Simple MSXML library Wrapper class for MFC/C++ (header file)
// Author:      André Sébastien  (maximus@oreka.com)
// Copyright:   (c) ANDRE Sébastien
/////////////////////////////////////////////////////////////////////////////

#if !defined(XML_PARSER_H)
#define XML_PARSER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// -- CArray STL --
//
#include <afxtempl.h>

// -- Load MSXML library --
//
#import <msxml3.dll> named_guids
using namespace MSXML2;

class ZXmlParser
{
public:
   ZXmlParser( );                                                 // ctor
   virtual ~ZXmlParser( );                                        // dtor

   // -- ********************************* --
   // -- *** LOADING / SAVING DOCUMENT *** --
   // -- ********************************* --
   bool LoadXML_Document( LPCTSTR strFileName );                  // Load an XML Document from File
   bool SaveXML_Document( LPCTSTR strFileName );                  // Save the Current XML Document into a file
   void LoadXML_FromBuffer( LPCTSTR source );                     // Load an XML Document from Buffer
   void ResetXML_Document( );                                     // Clear the XML Document

   // -- ************************** --
   // -- *** XML REPRESENTATION *** --
   // -- ************************** --
   void GetXML( CString& buffer );                                // Get the XML Representation of the current node
   void GetXML_Document( CString& buffer );                       // Get the XML Representation of the entire document

   // -- ***************************** --
   // -- *** XML Header management *** --
   // -- ***************************** --
   bool SetHeader( LPCTSTR header, LPCTSTR name, LPCTSTR value ); // Set an header property like <? xml version="1.0"?>
   bool GetHeader( LPCTSTR header, LPCTSTR name, CString & res ); // Return the header property like <? xml version="1.0"?>
                                                                  // Rq: For request the version number like the example below
                                                                  //     you must specify header = "xml" and name = "version"
   // -- ********************** --
   // -- *** PARSING METHOD *** --
   // -- ********************** --
   virtual void ParseXML_Document( );                             // ===> YOU MUST IMPLEMENT IT FOR PARSING PROCESS <===

   // -- *********************************
   // -- *** CURRENT NODE TYPE CONTROL ***
   // -- *********************************
   bool IsTag( LPCTSTR aTag );                                    // Return true if it's equal to the Current Tag
   bool IsChildOf( LPCTSTR parent_chain );                        // Test if a chain Tag is the parent of current Tag
   bool IsRoot( );                                                // Test if a Tag is the Root
   bool IsTextNode( );                                            // Return true if it's a TEXT node
   bool IsCDataSection( );                                        // Return true if it's a CDATA section

   // -- *******************************************
   // -- *** CURRENT NODE TYPE ACCESS PROPERTIES ***
   // -- *******************************************
   CString & GetCurrentTag( );                                    // Get the Current Tag value  (with "<>")
   CString & GetCurrentName( );                                   // Get the Current Name value (without "<>")
   CString & GetTextValue( );                                     // Get the Text Value when it's a TEXT or CDATA node
                                                                  // If you make a call of "GetTextValue" of a node
                                                                  // this method will return you it's Text value if it has one
                                                                  // For return the CData section, you must be on the CDATA
                                                                  // section itself
   bool SetTextValue( LPCTSTR TextValue );                        // Set a Text to the Current Node
                                                                  // on a CData section, this method set the data

   // -- ************************* --
   // -- *** ATTRIBUTES ACCESS *** --
   // -- ************************* --
   bool IsValidAttribute( LPCTSTR Name );                         // Return true if current node has the specified attribute defined
   CString & GetAttributeValue( );                                // Return the attribute value selected by "IsValidAttribute( )"

   int GetAttributeCount( );                                      // Return the number of attributes for the current node
   CString & GetAttributeName( int index );                       // Return the attribute name for the nth attribute
   CString & GetAttributeValue( int index );                      // Return the attribute value for the nth attribute

   bool SetAttribute( LPCTSTR AttribName, LPCTSTR AttribValue );  // Set an attribute to the Current Node
   bool RemoveAttribute( LPCTSTR AttribName );                    // Remove an attribute from the Current Node

   // -- ****************************** --
   // -- *** CDATA SECTION CREATION *** --
   // -- ****************************** --
   bool AddLastChildCData( LPCTSTR data );                        // Add a new CDATA Section at the End of Current Node
   bool AddFirstChildCData( LPCTSTR data );                       // Add a new CDATA Section at the Begining of Current Node
   bool AddCDataBefore( LPCTSTR data );                           // Add a new CDATA Section (same level) before the Current Node
   bool AddCDataAfter( LPCTSTR data );                            // Add a new CDATA Section (same level) after the Current Node
                                                                  // Rq: When Current Node is a CDATA Section,
                                                                  //     use GetTextValue( ) for retrieve DATA
                                                                  //     and SetTextValue( ) for sets DATA
   // -- ********************* --
   // -- *** NODE CREATION *** --
   // -- ******************** --
   bool AddLastChildNode( LPCTSTR Name );                         // Add a new CHILD NODE at the End of Current Node
   bool AddFirstChildNode( LPCTSTR Name );                        // Add a new CHILD NODE at the Begining of Current Node
   bool AddNodeBefore( LPCTSTR Name );                            // Add a new NODE ( same level ) before the Current Node
   bool AddNodeAfter( LPCTSTR Name );                             // Add a new NOFR ( same level ) after the Current Node

   // -- *********************
   // -- *** NODE DELETION ***
   // -- *********************
   bool Remove( );                                                // Remove the Current Node (Move to the Parent Node)
   bool RemoveChild( LPCTSTR NodeName );                          // Remove a Child Node     (No Move);

   // -- ******************* --
   // -- *** NODE MOVING *** --
   // -- ******************* --
   void GoToRoot( );                                              // Go to the Root node
   bool GoToChild( );                                             // Go to the first child of the current node
   bool GoToParent( );                                            // Go to the parent node of the current node
   bool GoForward( );                                             // Go to the next     node of same level as the current node
   bool GoBackward( );                                            // Go to the previous node of same level as the current node

   // -- *************************** --
   // -- *** NODE MOVING by name *** --
   // -- *************************** --
   bool GoToChild( LPCTSTR NodeName );                            // Go to a Direct Child Node
   bool GoToDescendant( LPCTSTR NodeName );                       // Go to a Descendant Node (Child, Little Child, etc...)
   bool GoToParent( LPCTSTR NodeName );                           // Go to a Parent Node
   bool GoForward( LPCTSTR NodeName );                            // Go to a next Node at the same Node than the Current
   bool GoBackward( LPCTSTR NodeName );                           // Go to a previous Node attached at the same Node than the Current

   // -- ************************ --
   // -- *** Error Management *** --
   // -- ************************ --
   CString & GetLastError( );                                     // Get the last error signification

private:
   // Node Value
   CString m_Tag;                                                 // The last Tag (the name with "<...>")
   CString m_Name;                                                // The Name (without "<...>")
   CString m_ChainTag;                                            // The Chain Tag
   CString m_TextField;                                           // The Text Value

   // Init MSXML library
   bool InitMSXML( );

   // XML document pointer
   IXMLDOMDocumentPtr m_plDomDocument;

   // Root node of XML document
   IXMLDOMElementPtr m_pDocRoot;

   // The current Node added
   IXMLDOMNodePtr m_CurrentNode;

   // Error Status String
   CString m_LastError, m_ok, m_tmp;

   // Attrib Values
   CArray<CString,CString &> m_AttribNames;
   CArray<CString,CString &> m_AttribValues;
   int m_attrib_index;

   // Internal Methods, Parse the Tree nodes
   void ParseObjects( IXMLDOMNodePtr pObject );

   // Update "Node Values" & Test type
   void GrabNodeInformation( IXMLDOMNodePtr pChild );             // Grab all eficients informations about a MSXML Node
   HRESULT IsMSXML_Node( IXMLDOMNodePtr pChild );                 // Return NULL, NODE_ELEMENT, NODE_TEXT or NODE_CDATA_SECTION

   // ChainTag methods management
   void ChainTagAdd( CString & val );
   void ChainTagRemove( int number );

   // Internal adding node
   bool _add_lastchild( IXMLDOMNodePtr newNode );
   bool _add_firstchild( IXMLDOMNodePtr newNode );
   bool _add_before( IXMLDOMNodePtr newNode );
   bool _add_after( IXMLDOMNodePtr newNode );
};
#endif
