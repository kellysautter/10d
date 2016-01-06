/////////////////////////////////////////////////////////////////////////////
// Name:        XML_PARSER.cpp
// Version:     2.0
// Purpose:     Simple MSXML library Wrapper class for MFC/C++ ( source file )
// Author:      André Sébastien  ( maximus@oreka.com )
// Copyright:   ( c ) ANDRE Sébastien
/////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Version 2.0 What's new?
//
//      1. The parsing document starts from document and not from root element
//           - Parse the header document information
//      2. New XML header management
//           - encoding format, stylesheet reference, etc...
//                - SetHeader( "xml", "version", "1.0" )
//                - SetHeader( "xml", "encoding", "UTF-8" ), etc...
//      3. New CData section management
//           - We can now read and add CData section with method like "Add_LastChildCData( LPCTSTR data )"
//      4. Some new methods for more fun
//           - like "GetXML_Document( )"
//      5. Minor design change for improve the class
//           - Some methods have been renamed like "Get_Text( )" to "Get_TextValue( )"
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include <stdafx.h>
#include <atlbase.h>
#include "XML_PARSER.h"

ZXmlParser::ZXmlParser( )       // Constructor
{
   // Init our members
   m_AttribNames.RemoveAll( );
   m_AttribValues.RemoveAll( );
   m_Tag.Empty( );
   m_Name.Empty( );
   m_ChainTag.Empty( );
   m_TextField.Empty( );

   // Init MSXML members
   m_CurrentNode = NULL;
   m_plDomDocument = NULL;
   m_pDocRoot = NULL;

   m_attrib_index = -1;

   // -- Errors Init --
   m_ok.Empty( );
   m_ok = "Success";
   m_LastError = m_ok;
}

ZXmlParser::~ZXmlParser( )
{
   // Free ressource
   ResetXML_Document( );
}

CString&
ZXmlParser::GetLastError( )
{
   return( m_LastError );
}

bool
ZXmlParser::InitMSXML( )
{
   m_LastError = m_ok;

   // -- Init OLE Object Comunication for MSXML ( only once time )--
   static bool ole_initialized = false;
   if ( !ole_initialized )
   {
      ::AfxOleInit( );
      ole_initialized = true;
   }

   // -- Instantiate an XML document --
   HRESULT hr = m_plDomDocument.CreateInstance( CLSID_DOMDocument );
   if ( FAILED( hr ) )
   {
      _com_error er( hr );
      m_LastError = er.ErrorMessage( );
      return( false );
   }

   return( true );
}

bool
ZXmlParser::LoadXML_Document( LPCTSTR strFileName )
{
   m_LastError = m_ok;

   // Reset Document
   ResetXML_Document( );

   // Convert xml file name string to something COM can handle (BSTR)
   CString csFileName = strFileName;
   _bstr_t bstrFileName;
   bstrFileName = csFileName.AllocSysString( );

   // Call the IXMLDOMDocumentPtr's load function to load the XML document
   variant_t vResult;
   vResult = m_plDomDocument->load( bstrFileName );
   if ( ((bool) vResult) == TRUE ) // success!
   {
      // Now that the document is loaded, we need to initialize the root pointer
      m_pDocRoot = m_plDomDocument->documentElement;

      // Now we can Parse this document !!
      ParseObjects( m_plDomDocument );

      GoToRoot( );

      return( true );
   }
   else
   {
      // XML document is not loaded, return error
      m_LastError = "XML Document FAILED to load: ";
      m_LastError += strFileName;
      return( false );
   }
}

void
ZXmlParser::LoadXML_FromBuffer( LPCTSTR source )
{
   // Reset the document
   ResetXML_Document( );

   // Load from Buffer
   m_plDomDocument->loadXML( source );
   m_pDocRoot = m_plDomDocument->documentElement;

   // Start the Parsing
   ParseObjects( m_plDomDocument );

   // Move to the root node
   GoToRoot( );
}

void
ZXmlParser::GetXML( CString& buffer )
{
   if ( m_CurrentNode == m_pDocRoot )
      GetXML_Document( buffer );
   else
   {
      BSTR  b_xml;

      m_CurrentNode->get_xml( &b_xml );
      buffer = b_xml;
   }
}

void
ZXmlParser::GetXML_Document( CString& buffer )
{
   BSTR b_xml;

   m_plDomDocument->get_xml( &b_xml );
   buffer = b_xml;
}

void
ZXmlParser::ParseXML_Document( )
{
   // Nothing to do, this method will be redefined in derived class
}

void
ZXmlParser::ParseObjects( IXMLDOMNodePtr pObject )
{
   // Child node
   IXMLDOMNodePtr pChild;

   // Grab Informations from the pObject node
   GrabNodeInformation( pObject );

   // Update "m_CurrentNode"
   m_CurrentNode = pObject;

   // Call User Parse Handling for let him what he want.
   ParseXML_Document( );

   if ( IsMSXML_Node( pObject ) != NODE_ELEMENT )
     return;

   // add the ChainTag
   ChainTagAdd( m_Tag );

   for ( pChild = pObject->firstChild;  NULL != pChild;  pChild = pChild->nextSibling )
   {
      // Parse Child nodes
      ParseObjects( pChild );
   }

   // Remove Current Tag from ChainTag
   ChainTagRemove( 1 );
}

bool
ZXmlParser::IsTag( LPCTSTR aTag )
{
   return( GetCurrentTag( ) == aTag );
}

bool
ZXmlParser::IsTextNode( )
{
   return( IsTag( "<#TEXT#>" ) );
}

bool
ZXmlParser::IsCDataSection( )
{
   return( IsTag( "<#CDATA#>" ) );
}

CString&
ZXmlParser::GetCurrentTag( )
{
   return( m_Tag );
}

CString&
ZXmlParser::GetCurrentName( )
{
   return( m_Name );
}

bool
ZXmlParser::IsRoot( )
{
   return( m_ChainTag.IsEmpty( ) != 0 );
}

bool
ZXmlParser::IsChildOf( LPCTSTR parent_chain )
{
   int pos = m_ChainTag.Find( parent_chain );
   int len = strlen( parent_chain );

   if ( pos != -1 )
   {
      // look if it's the end of this chainTag
      return(  m_ChainTag.GetLength( ) == pos + len );
   }

   return( false );
}

CString&
ZXmlParser::GetTextValue( )
{
   // Now you can handle the text value on the real node directly if you want

   if ( IsMSXML_Node( m_CurrentNode ) == NODE_TEXT || IsMSXML_Node( m_CurrentNode ) == NODE_CDATA_SECTION )
      return( m_TextField );

   // We must find his text value
   CString TextValue;

   // Find if a CHILD TEXT NODE exist or not
   if ( m_CurrentNode == NULL )
   {
      if ( m_pDocRoot )
         m_CurrentNode = m_pDocRoot;
      else
      {
         // We can't set the text
         m_LastError = "ZXmlParser::Get_TextValue( ) Can't set text on NULL node";
         m_TextField = "";
         return( m_TextField );
      }
   }

   // Find it now
   if ( m_CurrentNode->hasChildNodes( ) )
   {
      IXMLDOMNodePtr pChild;
      for( pChild = m_CurrentNode->firstChild; pChild; pChild = pChild->nextSibling )
      {
         // Find it's a NODE TEXT
         if ( IsMSXML_Node( pChild ) == NODE_TEXT )
         {
            // Take information from this Text Node
            GrabNodeInformation( pChild );
            TextValue = m_TextField;
            GrabNodeInformation( m_CurrentNode ); // it remove m_TextField
            m_TextField = TextValue;
            return( m_TextField );
         }
      }
   }

   m_TextField = "";
   return( m_TextField );
}

HRESULT
ZXmlParser::IsMSXML_Node( IXMLDOMNodePtr pChild )
{
   if ( pChild == NULL )
      return( NULL );

   // I handle only few node type other are ignored and are considered as NODE_ELEMENT
   //
   // Handled Node type by this wrapper:
   //      - NODE_ELEMENT
   //      - NODE_TEXT
   //      - NODE_CDATA_SECTION
   //      - NODE_PROCESSING_INSTRUCTION

   if ( pChild->nodeType == NODE_TEXT )
      return( NODE_TEXT );

   if ( pChild->nodeType == NODE_CDATA_SECTION )
      return( NODE_CDATA_SECTION );

   if ( pChild->nodeType == NODE_PROCESSING_INSTRUCTION )
      return( NODE_PROCESSING_INSTRUCTION );

   return( NODE_ELEMENT );
}

void
ZXmlParser::GrabNodeInformation( IXMLDOMNodePtr pChild )
{
   if ( IsMSXML_Node( pChild ) == NODE_ELEMENT )
   {
      // It's a node

      // Tag Update
      m_Tag.Format( "<%s>", (const char *)( pChild->nodeName ) );
      m_Name = (const char *)( pChild->nodeName );

      // TextField no significant value
      m_TextField.Empty( );

      // Update Attribute List
      m_AttribNames.RemoveAll( );
      m_AttribValues.RemoveAll( );

      IXMLDOMNamedNodeMapPtr pAttribs = pChild->Getattributes( );
      if ( pAttribs )
      {
         long nAttriCnt = pAttribs->Getlength( );

         // Iterate over the attributes
         for( int i=0; i<nAttriCnt; ++i )
         {
            IXMLDOMNodePtr pAttrib = pAttribs->Getitem( i );
            if ( pAttrib )
            {
               CString cs = ( const char * ) pAttrib->GetnodeName( );
               m_AttribNames.Add( cs );

               _variant_t vVal = pAttrib->GetnodeValue (  );
               cs = ( const char * ) _bstr_t( vVal );
               m_AttribValues.Add(  cs );
            }
         }
      }
   }
   else
   if ( IsMSXML_Node( pChild ) == NODE_TEXT )
   {
      // Tag is #TEXT#
      m_Tag.Empty( );
      m_Tag = "<#TEXT#>";
      m_Name.Empty( );
      m_Name = "#TEXT#";

      // TextField Update
      m_TextField = (const char *)( pChild->text );

      // Update Attribute List have no means
      m_AttribNames.RemoveAll( );
      m_AttribValues.RemoveAll( );
   }
   else
   if ( IsMSXML_Node( pChild ) == NODE_CDATA_SECTION )
   {
      // Tag is #CDATA#
      m_Tag.Empty( );
      m_Tag = "<#CDATA#>";
      m_Name.Empty( );
      m_Name = "#CDATA#";

      // TextField Update
      m_TextField = (const char *)( pChild->text );

      // Update Attribute List have no means
      m_AttribNames.RemoveAll( );
      m_AttribValues.RemoveAll( );
   }
   else
   if ( IsMSXML_Node( pChild ) == NODE_PROCESSING_INSTRUCTION )
   {
      // Do nothing here
   }

   return;
}

void
ZXmlParser::ChainTagAdd( CString& val )
{
   // Add a tag to the ChainTag
   if ( !m_ChainTag.IsEmpty( ) )
      m_ChainTag += val;
   else
      m_ChainTag = val;
}

void
ZXmlParser::ChainTagRemove( int number )
{
   // Remove the n tag to the ChainTag
   for( int bcl = 0; bcl < number; bcl ++ )
   {
      int pos = m_ChainTag.ReverseFind( '<' );
      if ( pos == -1 )
         m_ChainTag.Empty( );
      else
         m_ChainTag = m_ChainTag.Left( pos );
   }
}

int
ZXmlParser::GetAttributeCount( )
{
   return( m_AttribNames.GetSize( ) );
}

CString&
ZXmlParser::GetAttributeName( int index )
{
   m_LastError = "ZXmlParser::GetAttributeName( int ) failed";
   if ( index < 0 || index > GetAttributeCount( ) )
   {
      m_tmp.Empty( );
      return( m_tmp );
   }

   m_LastError = m_ok;
   return( m_AttribNames[ index ] );
}

CString&
ZXmlParser::GetAttributeValue( int index )
{
   m_LastError = "ZXmlParser::GetAttributeValue( int ) failed";
   if ( index < 0 || index > GetAttributeCount( ) )
   {
      m_tmp.Empty( );
      return( m_tmp );
   }

   m_LastError = m_ok;
   return( m_AttribValues[ m_attrib_index ] );
}

bool
ZXmlParser::IsValidAttribute( LPCTSTR Name )
{
   // Create the CString Name Object
   CString sName = Name;

   // Clear attribute index
   m_attrib_index = -1;

   int bcl;
   for( bcl = 0; bcl < GetAttributeCount( ) ; bcl++ )
   {
      // Check if the name is equal
      if ( m_AttribNames[ bcl ] == sName )
      {
         // set index fot let user to retrieve value with "GetAttributeValue( )" method
         m_attrib_index = bcl;
         return( true );
      }
   }

   return( false );
}

CString&
ZXmlParser::GetAttributeValue( )
{
   // Assume Success
   m_LastError = m_ok;

   if ( m_attrib_index != -1 )
      return( m_AttribValues[ m_attrib_index ] );

   // We can't retrieve a Attribute values
   m_LastError = "ZXmlParser::GetAttributeValue( )  : Can't Retrieve an Attribute";
   return( m_LastError );
}

bool
ZXmlParser::_add_lastchild( IXMLDOMNodePtr newNode )
{
   // Attach the Node to the document
   if ( m_CurrentNode )
   {
      if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
         return( false );

      ChainTagAdd(  GetCurrentTag( ) );
      newNode = m_CurrentNode->appendChild( newNode );
   }
   else
   if ( m_pDocRoot )
   {
      if ( IsMSXML_Node( m_pDocRoot ) != NODE_ELEMENT )
         return( false );

      CString cs = (const char *) m_pDocRoot->nodeName;
      ChainTagAdd(  cs );
      newNode = m_pDocRoot->appendChild( newNode );
   }
   else
   {
      m_ChainTag.Empty( );
      m_pDocRoot = newNode;
      m_plDomDocument->documentElement = m_pDocRoot;
   }

   // Update Current Node (cast operation)
   m_CurrentNode = newNode;

   // Update information for this Node
   GrabNodeInformation( m_CurrentNode );

   return( true );
}

bool
ZXmlParser::_add_firstchild( IXMLDOMNodePtr newNode )
{
   // Create Reference Node for the Insertion
   _variant_t NodeRef = (IUnknown *) m_CurrentNode->firstChild;

   // Attach the Node to the document
   if ( m_CurrentNode )
   {
      if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
         return( false );

      ChainTagAdd(  GetCurrentTag( ) );
      newNode = m_CurrentNode->insertBefore( newNode, NodeRef );
   }
   else
   if ( m_pDocRoot )
   {
      if ( IsMSXML_Node( m_pDocRoot ) != NODE_ELEMENT )
         return( false );

      CString cs = (const char *) m_pDocRoot->nodeName;
      ChainTagAdd( cs );
      newNode = m_pDocRoot->insertBefore( newNode, NodeRef );
   }
   else
   {
      m_ChainTag.Empty( );
      m_pDocRoot = newNode;
      m_plDomDocument->documentElement = m_pDocRoot;
   }

   // Update Current Node (cast operation)
   m_CurrentNode = newNode;

   // Update information for this Node
   GrabNodeInformation( m_CurrentNode );

   return( true );
}

bool
ZXmlParser::_add_before( IXMLDOMNodePtr newNode )
{
   if ( IsRoot( ) )
   {
      m_LastError = "ZXmlParser::_add_before( )   : Can't add node at same level that the root node";
      return( false );
   }

   // Create Reference Node for the Insertion
   IXMLDOMNodePtr pParent;
   m_CurrentNode->get_parentNode( &pParent );
   _variant_t NodeRef = (IUnknown *) m_CurrentNode;


   // Attach the Node to the document
   if ( m_CurrentNode )
          newNode = pParent->insertBefore( newNode, NodeRef );
   else
   if ( m_pDocRoot )
      newNode = m_pDocRoot->insertBefore( newNode, NodeRef );
   else
   {
      m_pDocRoot = newNode;
      m_plDomDocument->documentElement = m_pDocRoot;
   }

   // Update Current Node (cast operation)
   m_CurrentNode = newNode;

   // Update information for this Node
   GrabNodeInformation( m_CurrentNode );

   return( true );
}

bool
ZXmlParser::_add_after( IXMLDOMNodePtr newNode )
{
   // If m_CurrentNode->NextSibling == NULL then must call AddLastChildNode on Parent Node
   // Because we can't use InsertBefore on a NULL Reference ;o )
   //
   // We are sure that a Parent node exist because before we control that we aren't not on root node.
   if ( m_CurrentNode->nextSibling == NULL )
   {
      // Get Parent Node
      IXMLDOMNodePtr pParent;
      m_CurrentNode->get_parentNode( &pParent );

      // Set Parent node as Current Node
      m_CurrentNode = pParent;
      GrabNodeInformation( pParent );

      // Add Node as Last Child Node
      return( AddLastChildNode( m_Name ) );
   }

   // Create Reference Node for the Insertion
   IXMLDOMNodePtr pParent;
   m_CurrentNode->get_parentNode( &pParent );
   _variant_t NodeRef = (IUnknown *) m_CurrentNode->nextSibling;

   // Attach the Node to the document
   if ( m_CurrentNode )
      newNode = pParent->insertBefore( newNode, NodeRef );
   else
   if ( m_pDocRoot )
      newNode = m_pDocRoot->insertBefore( newNode, NodeRef );
   else
   {
      m_pDocRoot = newNode;
      m_plDomDocument->documentElement = m_pDocRoot;
   }

   // Update Current Node (cast operation)
   m_CurrentNode = newNode;

   // Update information for this Node
   GrabNodeInformation( m_CurrentNode );

   return( true );
}

bool
ZXmlParser::AddLastChildCData( LPCTSTR data )
{
   // Nodes members
   IXMLDOMNodePtr newNode;

   CComVariant vType( MSXML2::NODE_CDATA_SECTION );
   newNode = m_plDomDocument->createNode( vType, "", "" );

   // Attach the Node to the document
   bool Result = _add_lastchild( newNode );
   SetTextValue( data );

   return( Result );
}

bool
ZXmlParser::AddFirstChildCData( LPCTSTR data )
{
   // Nodes members
   IXMLDOMNodePtr newNode;

   // If no child then use Add_LastChildCData or CurrentElement node not set yet
   //
   if ( m_CurrentNode )
   {
      if ( !m_CurrentNode->hasChildNodes( ) )
         return( AddLastChildCData( data ) );
   }
   else
      return( AddLastChildCData( data ) );

   // Create the NODE
   CComVariant vType( MSXML2::NODE_CDATA_SECTION );
   newNode = m_plDomDocument->createNode( vType, "", "" );

   bool Result = _add_firstchild( newNode );
   SetTextValue( data );

   return( Result );
}

bool
ZXmlParser::AddCDataBefore( LPCTSTR data )
{
   // Nodes members
   // IXMLDOMElementPtr m_CurrentNode;        /* Global Member */
   IXMLDOMNodePtr newNode;

   // Can't use this function on the root node
   if ( IsRoot( ) )
     return( false );

   // Create the NODE
   CComVariant vType( MSXML2::NODE_CDATA_SECTION );
   newNode = m_plDomDocument->createNode( vType, "", "" );

   bool Result = _add_before( newNode );
   SetTextValue( data );

   return( Result );
}

bool
ZXmlParser::AddCDataAfter( LPCTSTR data )
{
   // Nodes members
   IXMLDOMNodePtr newNode;

   // Can't use this function on the root node
   if ( IsRoot( ) )
   {
      m_LastError = "ZXmlParser::Add_CDataAfter( LPCTSTR )   : Can't add node at same level that the root node";
      return( false );
   }

   // Create the NODE
   CComVariant vType( MSXML2::NODE_CDATA_SECTION );
   newNode = m_plDomDocument->createNode( vType, "", "" );

   bool Result = _add_after( newNode );
   SetTextValue( data );

   return( Result );
}

bool
ZXmlParser::AddLastChildNode( LPCTSTR Name )
{
   // Nodes members
   IXMLDOMNodePtr newNode;

   // Create NODE TYPE
   CComVariant vType( MSXML2::NODE_ELEMENT );

   // Create the NODE
   newNode = m_plDomDocument->createNode( vType, Name, "" );

   // Attach the Node to the document
   return( _add_lastchild( newNode ) );
}

bool
ZXmlParser::AddNodeBefore( LPCTSTR Name )
{
   // Nodes members
   // IXMLDOMElementPtr m_CurrentNode;        /* Global Member */
   IXMLDOMNodePtr newNode;

   // Can't use this function on the root node
   if ( IsRoot( ) )
      return( false );

   // Create NODE TYPE
   CComVariant vType( MSXML2::NODE_ELEMENT );

   // Create the NODE
   newNode = m_plDomDocument->createNode( vType, Name, "" );

   return( _add_before( newNode ) );
}

bool
ZXmlParser::AddNodeAfter( LPCTSTR Name )
{
   // Nodes members
   // IXMLDOMElementPtr m_CurrentNode;        /* Global Member */
   IXMLDOMNodePtr newNode;

   // Can't use this function on the root node
   if ( IsRoot( ) )
   {
      m_LastError = "ZXmlParser::AddNodeAfter( LPCTSTR )   : Can't add node at same level that the root node";
      return( false );
   }

   // Create NODE TYPE
   CComVariant vType( MSXML2::NODE_ELEMENT );

   // Create the NODE
   newNode = m_plDomDocument->createNode( vType, Name, "" );

   return( _add_after( newNode ) );
}

bool
ZXmlParser::AddFirstChildNode( LPCTSTR Name )
{
   // Nodes members
   // IXMLDOMElementPtr m_CurrentNode;        /* Global Member */
   //
   IXMLDOMNodePtr newNode;
   //IXMLDOMNodeListPtr ChildList;

   // If no child then use AddLastChildNode or CurrentElement node not set yet
   if ( m_CurrentNode )
   {
      if ( !m_CurrentNode->hasChildNodes( ) )
         return( AddLastChildNode( Name ) );
   }
   else
      return( AddLastChildNode( Name ) );

   // Create NODE TYPE
   CComVariant vType( MSXML2::NODE_ELEMENT );

   // Create the NODE
   newNode = m_plDomDocument->createNode( vType, Name, "" );

   return( _add_firstchild( newNode ) );
}

bool
ZXmlParser::SetAttribute( LPCTSTR AttribName, LPCTSTR AttribValue )
{
   // Nodes members
   IXMLDOMElementPtr CurrentElementNode = m_CurrentNode;

   // Create Attribute variable
   CComVariant sAttribute( AttribValue );

   // Set the new attribute
   if ( m_CurrentNode )
   {
      CurrentElementNode->setAttribute(  AttribName, sAttribute );
      m_CurrentNode = CurrentElementNode;
      GrabNodeInformation( m_CurrentNode );
   }
   else
   {
      m_pDocRoot->setAttribute( AttribName, sAttribute );
      GrabNodeInformation( m_pDocRoot );
   }

   return( true );
}

bool
ZXmlParser::RemoveAttribute( LPCTSTR AttribName )
{
   if ( m_CurrentNode == NULL )
   {
      if ( m_pDocRoot )
         m_CurrentNode = m_pDocRoot;
      else
      {
         m_LastError = "ZXmlParser::Remove_Attribute( LPCTSTR )   : Can't remove attribute on a NULL Node";
         return( false );
      }
   }

   IXMLDOMElementPtr CurrentElementNode = m_CurrentNode;
   CurrentElementNode->removeAttribute( AttribName );
   m_CurrentNode = CurrentElementNode;

   return( true );
}

bool
ZXmlParser::SetTextValue( LPCTSTR TextValue )
{
   // Nodes members
   // IXMLDOMElementPtr m_CurrentNode;        /* Global Member */
   IXMLDOMNodePtr newNode;

   // Find if a CHILD TEXT NODE exist or not
   if ( m_CurrentNode == NULL )
   {
      if ( m_pDocRoot )
         m_CurrentNode = m_pDocRoot;
      else
      {
         m_LastError = "ZXmlParser::SetTextValue( LPCTSTR )   : Can't set a Text on a NULL Node";
         return( false );
      }
   }

   if ( IsMSXML_Node( m_CurrentNode ) == NODE_CDATA_SECTION )
   {
     CComVariant sValue( TextValue );
     m_CurrentNode->put_nodeValue( sValue );

     return( true );
   }

   // Rq: a NODE_CDATA_SECTION can't have any childs
   if ( m_CurrentNode->hasChildNodes( ) )
   {
      IXMLDOMNodePtr pChild;
      for( pChild = m_CurrentNode->firstChild; pChild; pChild = pChild->nextSibling )
      {
         // Find it's a NODE TEXT
         if ( IsMSXML_Node( pChild ) == NODE_TEXT )
         {
            // A Text Node is found, Replace it now!!
            CComVariant sValue( TextValue );
            pChild->put_nodeValue( sValue );

            return( true );
         }
      }
   }

   // No previous Text was defined before, we can add it.
   if ( IsMSXML_Node( m_CurrentNode ) == NODE_ELEMENT )
   {
      // Create NODE TEXT type
      CComVariant vType( MSXML2::NODE_TEXT );

      // Create the node
      newNode = m_plDomDocument->createTextNode( TextValue );

      // Attach the Node to the document
      newNode = m_CurrentNode->appendChild( newNode );
   }

   return( true );
}

bool
ZXmlParser::SaveXML_Document( LPCTSTR strFileName )
{
   // Save the XML document
   m_plDomDocument->save( strFileName );

   return( true );
}

void
ZXmlParser::ResetXML_Document( )
{
   // Init
   InitMSXML( );
   m_plDomDocument->loadXML( "" );
   m_pDocRoot = m_plDomDocument->documentElement;
   m_CurrentNode = NULL;

   // Init our members
   m_AttribNames.RemoveAll( );
   m_AttribValues.RemoveAll( );
   m_Tag.Empty( );
   m_Name.Empty( );
   m_ChainTag.Empty( );
   m_TextField.Empty( );
   m_attrib_index = -1;
}

void
ZXmlParser::GoToRoot( )
{
  m_attrib_index = -1;
  m_CurrentNode = m_pDocRoot;
  m_ChainTag.Empty( );
}

bool
ZXmlParser::GoToChild( )
{
   // Child node
   IXMLDOMNodePtr pChild;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
      return( false );

   pChild = m_CurrentNode->firstChild;
   if ( pChild == NULL )
      return( false );

   // Grab Information from Child node
   m_attrib_index = -1;
   ChainTagAdd( GetCurrentTag( ) );
   GrabNodeInformation( pChild );

   m_CurrentNode = pChild;
   return( true );
}

bool
ZXmlParser::GoToParent( )
{
   // Parent node
   IXMLDOMNodePtr pParent = NULL;

   if ( m_CurrentNode == m_pDocRoot )
      return( false );

   m_CurrentNode->get_parentNode( &pParent );
   m_CurrentNode = pParent;

   m_attrib_index = -1;
   ChainTagRemove( 1 );
   GrabNodeInformation( m_CurrentNode );
   return( true );
}

bool
ZXmlParser::GoForward( )
{
   // Sibling node
   IXMLDOMNodePtr pSibling = NULL;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT && IsMSXML_Node( m_CurrentNode ) != NODE_CDATA_SECTION )
      return( false );

   pSibling = m_CurrentNode->nextSibling;
   if ( pSibling == NULL )
      return( false );

   m_CurrentNode = pSibling;

   // Grab Information from Sibling node
   GrabNodeInformation( m_CurrentNode );
   return( true );
}

bool
ZXmlParser::GoBackward( )
{
   // Sibling node
   IXMLDOMNodePtr pSibling;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT && IsMSXML_Node( m_CurrentNode ) != NODE_CDATA_SECTION )
      return( false );

   pSibling = m_CurrentNode->previousSibling;
   if ( pSibling == NULL )
      return( false );

   m_CurrentNode = pSibling;

   // Grab Information from Sibling node
   m_attrib_index = -1;
   GrabNodeInformation( pSibling );
   return( true );
}

bool
ZXmlParser::GoToChild( LPCTSTR NodeName )
{
   // Child node
   IXMLDOMNodePtr pChild;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
      return( false );

   ChainTagAdd( GetCurrentTag( ) );

   for ( pChild = m_CurrentNode->firstChild;  NULL != pChild;  pChild = pChild->nextSibling )
   {
      // Grab Information from Child node
      GrabNodeInformation( pChild );

      if ( GetCurrentName( ) == NodeName )
      {
         // Update new Position
         m_attrib_index = -1;
         m_CurrentNode = pChild;
         return( true );
      }
   }

   // Node Not Found (restore Node information)
   ChainTagRemove( 1 );
   GrabNodeInformation( m_CurrentNode );
   return( false );
}

bool
ZXmlParser::GoToDescendant( LPCTSTR NodeName )
{
   // Child node
   IXMLDOMNodePtr pChild;

   // Current Node before the call method
   IXMLDOMElementPtr pCurrent = m_CurrentNode;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
      return( false );

   ChainTagAdd( GetCurrentTag( ) );

   for ( pChild = m_CurrentNode->firstChild;  NULL != pChild;  pChild = pChild->nextSibling )
   {
      // Grab Information from Child node
      GrabNodeInformation( pChild );

      if ( GetCurrentName( ) == NodeName )
      {
         // Update new Position
         m_attrib_index = -1;
         m_CurrentNode = pChild;
         return( true );
      }

      // Try to go into Childs of this Child
      m_CurrentNode = pChild;
      if (  GoToDescendant( NodeName ) )
      {
         // We find the approriate node
         // All is set, we can return
         return( true );
      }

      // Restore Current node
      m_CurrentNode = pCurrent;
   }

   // Node Not Found (restore Node information)
   ChainTagRemove( 1 );
   GrabNodeInformation( m_CurrentNode );
   return( false );
}

bool
ZXmlParser::GoToParent( LPCTSTR NodeName )
{
   // Parent node
   IXMLDOMNodePtr pParent = NULL;
   IXMLDOMNodePtr oldCurrent = m_CurrentNode;

   if ( m_CurrentNode == m_pDocRoot )
      return( false );

   CString oldChainTag = m_ChainTag;

   m_CurrentNode->get_parentNode( &pParent );

   while ( true )
   {
      m_CurrentNode = pParent;
      ChainTagRemove( 1 );
      GrabNodeInformation( m_CurrentNode );
      if ( GetCurrentName( ) == NodeName )
      {
         m_attrib_index = -1;
         return( true );
      }

      if ( m_CurrentNode == m_pDocRoot )
         break;

      m_CurrentNode->get_parentNode( &pParent );
   }

   // Parent not found
   m_CurrentNode = oldCurrent;
   m_ChainTag = oldChainTag;
   GrabNodeInformation( m_CurrentNode );
   return( false );
}

// Go to a Node attached at the same Node than the Current Node (Forward sens)
bool
ZXmlParser::GoForward( LPCTSTR NodeName )
{
   // Sibling node
   IXMLDOMNodePtr pSibling = NULL;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT && IsMSXML_Node( m_CurrentNode ) != NODE_CDATA_SECTION )
      return( false );

   for ( pSibling= m_CurrentNode;  true ;  pSibling = pSibling->nextSibling )
   {
      if ( pSibling == NULL )
         break;

      // Grab Information from Sibling node
      GrabNodeInformation( pSibling );

      if ( GetCurrentName( ) == NodeName )
      {
         // Update new Position
         m_attrib_index = -1;
         m_CurrentNode = pSibling;
         return( true );
      }
   }

   // Node Not Found (restore Node information)
   GrabNodeInformation( m_CurrentNode );
   return( false );
}

bool
ZXmlParser::GoBackward( LPCTSTR NodeName )
{
   // Sibling node
   IXMLDOMNodePtr pSibling;

   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT && IsMSXML_Node( m_CurrentNode ) != NODE_CDATA_SECTION )
      return( false );

   for ( pSibling = m_CurrentNode;  NULL != pSibling;  pSibling = pSibling->previousSibling )
   {
      // Grab Information from Sibling node
      GrabNodeInformation( pSibling );

      if ( GetCurrentName( ) == NodeName )
      {
         // Update new Position
         m_attrib_index = -1;
         m_CurrentNode = pSibling;
         return( true );
      }
   }

   // Node Not Found (restore Node information)
   GrabNodeInformation( m_CurrentNode );
   return( false );
}

bool
ZXmlParser::Remove( )
{
   // Parent node
   IXMLDOMNodePtr pParent= NULL;

   if ( m_CurrentNode == NULL )
   {
      if ( m_pDocRoot )
         m_CurrentNode = m_pDocRoot;
      else
         return( false );
   }

   if ( m_CurrentNode != m_pDocRoot )
   {
      m_CurrentNode->get_parentNode( &pParent );
      pParent->removeChild( m_CurrentNode );
      m_CurrentNode = pParent;
      GrabNodeInformation( m_CurrentNode );
   }
   else
      ResetXML_Document( );

   m_attrib_index = -1;
   return( true );
}

bool
ZXmlParser::RemoveChild( LPCTSTR NodeName )
{
   // Find the Child
   if ( IsMSXML_Node( m_CurrentNode ) != NODE_ELEMENT )
      return( false );

   if (  GoToChild( NodeName ) )
   {
      int cur_attrib_index = m_attrib_index;
      bool result = Remove( );

      m_attrib_index = cur_attrib_index;
      return( result );
   }
   else
      return( false );
}

// ***************************
// ** Header XML Management **
// ***************************
bool
ZXmlParser::SetHeader( LPCTSTR header, LPCTSTR name, LPCTSTR value )
{
   m_LastError = m_ok;
   bool bEmptyXml = false;

   CString strHeader = header;
   CString strName = name;
   CString strValue = value;

   BSTR bstr;
   CString cstr, cstr2;

   IXMLDOMNodePtr pChild = NULL;           // Working node
   pChild = m_plDomDocument->firstChild;   // Start at first document child

   if ( pChild == NULL )
      bEmptyXml = true;

   while ( pChild )
   {
      if ( pChild == m_pDocRoot )
      {
         // Root document reach, it's done, the processing node wasn't found
         break;
      }

      if ( pChild->nodeType != NODE_PROCESSING_INSTRUCTION )
      {
         pChild = pChild->nextSibling;    // Go to Next Processing instruction node
         continue;
      }


      pChild->get_baseName( &bstr );
      cstr = bstr;
      if ( cstr == header )
      {
         // Correct header, check the correct property
         pChild->get_text( &bstr );
         cstr = bstr;

         int index = cstr.Find( name, 0 );
         if ( index == -1 )
         {
            // The property doesn't exist on this processing instruction" );

            // Assume correct constraint about "xml" processing instruction
            {
               // must have version="xxx" in first
               // must have standalone="xxx" in last if exist
               cstr2.Empty( );
               int standalone_index = cstr.Find( "standalone", 0 );
               if ( standalone_index != -1 )
               {
                  cstr2 = cstr.Right(  cstr.GetLength( ) - standalone_index + 1 );
                  cstr  = cstr.Left( standalone_index );
               }

               int version_index = cstr.Find( "version", 0 );
               if ( version_index == -1 && strHeader == "xml" )
               {
                  CString strTemp = cstr;
                  cstr = "version=\"1.0\" ";
                  cstr += strTemp;
               }

               if ( strName != "version" )
                  cstr += " " + strName + "=\"" + strValue + "\" " + cstr2;
               else
                  cstr += cstr2;
            }

            // Create the new Processing Instruction node
            HRESULT hr;
            IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
            hr = m_plDomDocument->raw_createProcessingInstruction( _bstr_t( strHeader ), _bstr_t( cstr ), &pIXMLDOMPInstr );

            if ( SUCCEEDED( hr ) )
            {
               // We succes the creation of the processing instruction
               // Replace the node
               m_plDomDocument->replaceChild( pIXMLDOMPInstr, pChild );
            }
            else
            {
               // Arf, i fails the creation, grrr, again
               m_LastError="ZXmlParser::SetHeader( ) : Can't create the new processing instruction node";
               return( false );
            }
            return( true );
         }
         else
         {
            // The processing instruction node exists, must change it's value!!
            int start, end;
            start = cstr.Find( '"', index );
            if ( start == -1 )
            {
               m_LastError = "ZXmlParser::SetHeader( ) : bad value structure";
               return( false );
            }

            end = cstr.Find( '"', start + 1 );
            if ( end == -1 )
            {
               m_LastError = "ZXmlParser::SetHeader( ) : bad value structure";
               return( false );
            }

            cstr2 = cstr.Mid( 0, start + 1 ) + value + cstr.Mid( end, cstr.GetLength( ) - end );

            IXMLDOMNodePtr m_lpNode = NULL;
            IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
            HRESULT hr;

            hr = m_plDomDocument->raw_createProcessingInstruction( _bstr_t( strHeader ), _bstr_t( cstr2 ), &pIXMLDOMPInstr );

            if ( SUCCEEDED( hr ) )
            {
               // We succeeded in the creation of the processing instruction
               // Replace the node
               m_plDomDocument->replaceChild( pIXMLDOMPInstr, pChild );
            }
            else
            {
               m_LastError="ZXmlParser::SetHeader( ) : Can't create the new processing instruction node";
               return( false );
            }

            return( true );
        }
     }

     pChild = pChild->nextSibling;   // next processing instruction node
   }

   // No processing node for our header
   {
      if ( strName != "version" && strHeader == "xml" )
         cstr = "version=\"1.0\" " + strName + "=\"" + strValue + "\"";
      else
         cstr = strName + "=\"" + strValue + "\"";

      IXMLDOMNodePtr m_lpNode = NULL;
      IXMLDOMProcessingInstruction *pIXMLDOMPInstr = NULL;
      HRESULT hr;

      hr = m_plDomDocument->raw_createProcessingInstruction( _bstr_t( strHeader ), _bstr_t( cstr ), &pIXMLDOMPInstr );
      if ( SUCCEEDED( hr ) )
      {
         if ( !bEmptyXml )
         {
            _variant_t NodeRef = (IUnknown *) m_pDocRoot;
            m_lpNode = NULL;
            m_lpNode = m_plDomDocument->insertBefore( pIXMLDOMPInstr, NodeRef );
            if ( m_lpNode == NULL )
               m_LastError = "PARSER_XML::SetHeader( ) : Can't insert Processing node after the root document";

            return( m_lpNode != NULL );
         }
         else
         {
            m_lpNode = NULL;
            m_lpNode = m_plDomDocument->appendChild( pIXMLDOMPInstr );
            if ( m_lpNode == NULL )
               m_LastError = "PARSER_XML::SetHeader( ) : Can't insert Processing node in the empty document";

            return( m_lpNode != NULL );
         }
      }

      m_LastError = "PARSER_XML::SetHeader( ) : Can't create new Processing node";
      return( false );
   }
}

bool
ZXmlParser::GetHeader( LPCTSTR header, LPCTSTR name, CString& res )
{
   m_LastError = m_ok;

   IXMLDOMNodePtr pChild;                  // working node
   res.Empty( );

   pChild = m_plDomDocument;
   pChild = m_plDomDocument->firstChild;   // start at first document child

   if ( pChild == NULL )
   {
      m_LastError = "ZXmlParser::Get_Header( ) : The XML Document is a null pointer";
      return( false );
   }

   while( pChild )
   {
      if ( pChild->nodeType != NODE_PROCESSING_INSTRUCTION )
         break;

      BSTR bstr;
      CString cstr;

      pChild->get_baseName( &bstr );
      cstr = bstr;
      if ( cstr == header )
      {
         // Correct header, check the correct property
         pChild->get_text( &bstr );
         cstr = bstr;

         int index = cstr.Find( name, 0 );
         if ( index == -1 )
            return( false );

         int start, end;
         start = cstr.Find( '"', index );
         if ( start == -1 )
         {
            m_LastError = "ZXmlParser::Get_Header( ) : bad value structure";
            return( false );
         }

         end = cstr.Find( '"', start + 1 );
         if ( end == -1 )
         {
            m_LastError = "ZXmlParser::Get_Header( ) : bad value structure";
            return( false );
         }

         res = cstr.Mid( start + 1, end - start - 1 );
         return( true );
      }

      pChild = pChild->nextSibling;   // next Processing instruction node
   }

   return( false );
}