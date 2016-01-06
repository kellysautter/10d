/********************************************************************************************/
//
// File:            tgcolser.cpp
// Copyright:       Ton Beller AG 2001
// Autor:           TMV
// Datum:           8. Februar 2001
// describtion:     Implemented helper classes for mfc serialization of Zeidon Grid
//                  properties
//
/* Change log most recent first order:

   2001.2.8   TMV   2000   ActiveX
      created




*/
/*********************************************************************************************/

#include "zstdafx.h"
#include "tdbg6.h"
#include "tgcolser.h"

//////////////////////////////////////////////////////////////////////////////
//
// Implementation of Zeidon property persistence
// Mapping Informations are stored in a blob that is build using
// mfc-Serialization ( very useful because of their versioning feature)
//
//////////////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( ZColumnMapping, CObject,      VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZGridColCtrl,   CObject,      VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColEdit,       ZGridColCtrl, VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColRadioGrp,   ZGridColCtrl, VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColRadioBtn,   ZGridColCtrl, VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColComboBox,   ZGridColCtrl, VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColCheckBox,   ZGridColCtrl, VERSIONABLE_SCHEMA | 1 )
IMPLEMENT_SERIAL( ZColumns,       CObList,      VERSIONABLE_SCHEMA | 1 )



/****************************************************************************
** class name      : ZGridColCtrl                                          **
** base class      : CObject                                               **
** derived classes : ZColEdit,                                             **
**                   ZColRadioGrp                                          **
**                   ZColRadioBtn                                          **
**                   ZColComboBox                                          **
**                   ZColCheckBox                                          **
**                                                                         **
** Description     : Provides basic mapping properties and serialization   **
**                                                                         **
**                                                                         **
****************************************************************************/




/**************************************************************************/
// Function name   : ZGridColCtrl::SetMapping
// Description     : Sets the basic mapping for a ctrl
// Return type     : void
// Argument        : zCPCHAR pszView         pointer to a views name
// Argument        : zCPCHAR pszEntity       pointer to an entityname name
//                                           within the view above
// Argument        : zCPCHAR pszAttribute    pointer to an attributename name
//                                           within the view entity above
// Argument        : zCPCHAR pszContext      pointer to a contextname of the
//                                           attribute above
/**************************************************************************/
void ZGridColCtrl::SetMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext)
{
   if ( m_pMapping )
      delete m_pMapping;
   m_pMapping = new ZColumnMapping( pszView, pszEntity, pszAttribute, pszContext );
}


/**************************************************************************/
// Function name   : ZGridColCtrl::Serialize
// Description     : reads or writes the objects members to an archive
// Return type     : void
// Argument        : CArchive& ar
/**************************************************************************/
void ZGridColCtrl::Serialize(CArchive& ar)
{
   if ( ar.IsStoring() )
   {
      ar << m_pMapping;
   }
   else
   {
      if( m_pMapping )
         delete m_pMapping;
      ar >> m_pMapping;
   }
}


/**************************************************************************/
// Function name   : ZColumnMapping casting operator
// Description     : returns the basic mapping
// Return type     : ZGridColCtrl::operator
/**************************************************************************/
ZGridColCtrl::operator ZColumnMapping*() const
{
   return m_pMapping;
}

ZGridColCtrl::~ZGridColCtrl()
{
   if( m_pMapping )
      delete m_pMapping;
}


/**************************************************************************/
// Function name   : ZGridColCtrl::GetMapViewName
// Description     : returns the name of the view mapped to a control
// Return type     : CString
/**************************************************************************/
CString ZGridColCtrl::GetMapViewName() const
{
   if( m_pMapping )
      return m_pMapping->GetMapViewName();
   else
      return "";
}


/**************************************************************************/
// Function name   : ZGridColCtrl::GetMapEntityName
// Description     : returns the name of an entity mapped to a control
// Return type     : CString
/**************************************************************************/
CString ZGridColCtrl::GetMapEntityName() const
{
   if( m_pMapping )
      return m_pMapping->GetMapEntityName();
   else
      return "";
}

/**************************************************************************/
// Function name   : ZGridColCtrl::GetMapAttributeName
// Description     : returns the name of an attribute mapped to a control
// Return type     : CString
/**************************************************************************/
CString ZGridColCtrl::GetMapAttributeName() const
{
   if( m_pMapping )
      return m_pMapping->GetMapAttributeName();
   else
      return "";
}

/**************************************************************************/
// Function name   : ZGridColCtrl::GetMapContextName
// Description     : returns the name of a context mapped to a control
// Return type     : CString
/**************************************************************************/
CString ZGridColCtrl::GetMapContextName() const
{
   if( m_pMapping )
      return m_pMapping->GetMapContextName();
   else
      return "";
}



/**************************************************************************/
// Function name   : ZGridColCtrl::GetAttributeDisplayLength
// Description     : returns the max. input length of an attribute
// Return type     : zULONG
/**************************************************************************/
zULONG  ZGridColCtrl::GetAttributeDisplayLength()
{
   if( m_pMapping )
      return m_pMapping->GetAttributeDisplayLength();
   else
      return( 0 );
}


/****************************************************************************
** class name      : ZColumnMapping                                        **
** base class      : CObject                                               **
** derived classes : none                                                  **
**                                                                         **
** Description     : Hols basic Mapping information and provides           **
**                   serialization                                         **
**                                                                         **
**                                                                         **
****************************************************************************/
ZColumnMapping::ZColumnMapping()
{
   m_ulDisplayLth = 0;
   m_strVName.Empty();
   m_strEName.Empty();
   m_strAName.Empty();
   m_strContext.Empty();
}

ZColumnMapping::ZColumnMapping(const ZColumnMapping& ColumnMapping)
{
   m_ulDisplayLth = 0;
   m_strVName   = ColumnMapping.GetMapViewName();
   m_strEName   = ColumnMapping.GetMapEntityName();
   m_strAName   = ColumnMapping.GetMapAttributeName();
   m_strContext = ColumnMapping.GetMapContextName();
}

ZColumnMapping::ZColumnMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext)
{
   m_ulDisplayLth = 0;
   SetMapping( pszView, pszEntity, pszAttribute, pszContext);
}

ZColumnMapping::~ZColumnMapping()
{
}


/**************************************************************************/
// Function name   : ZColumnMapping::SetMapping
// Description     :
// Return type     : void
// Argument        : zCPCHAR pszView         pointer to a views name
// Argument        : zCPCHAR pszEntity       pointer to an entityname name
//                                           within the view above
// Argument        : zCPCHAR pszAttribute    pointer to an attributename name
//                                           within the view entity above
// Argument        : zCPCHAR pszContext      pointer to a contextname of the
//                                           attribute above
/**************************************************************************/
void ZColumnMapping::SetMapping( zCPCHAR pszView, zCPCHAR pszEntity,
                                 zCPCHAR pszAttribute, zCPCHAR pszContext)
{
   m_strVName   = pszView;
   m_strEName   = pszEntity;
   m_strAName   = pszAttribute;
   m_strContext = pszContext;
}

void ZColumnMapping::Serialize(CArchive& ar)
{
   CObject::Serialize( ar );

   if( ar.IsStoring() )
   {
         ar << m_strVName;
         ar << m_strEName;
         ar << m_strAName;
         ar << m_strContext;
   }
   else
   {
      UINT uiSchema = ar.GetObjectSchema();
      switch ( uiSchema )
      {
      case 1:
         ar >> m_strVName;
         ar >> m_strEName;
         ar >> m_strAName;
         ar >> m_strContext;
         break;
      default:
//         TraceLineI("Failed to read ZColumnMapping using schema: ", uiSchema );
         break;
      }
   }
}


/**************************************************************************/
// Function name   : ZColumnMapping::InitMappingAttributeLength
// Description     : Retrieves the max. input length of an attribute
// Return type     : void
// Argument        : zVIEW vApp hosting current entity and attribute
/**************************************************************************/
void ZColumnMapping::InitMappingAttributeLength( zVIEW vApp )
{
   if( m_strVName.IsEmpty() || m_strEName.IsEmpty() || m_strAName.IsEmpty() )
      return;

   ::GetAttributeDisplayLength( &m_ulDisplayLth, vApp,
                           m_strEName,
                           m_strAName,
                           m_strContext.IsEmpty() ? 0 : (zCPCHAR)m_strContext );

}


/**************************************************************************/
// Function name   : ZColumnMapping::GetAttributeDisplayLength
// Description     : returns the max. input length of an attribute
//                   there must be a call to InitMappingAttributeLength
//                   prior to call this function
// Return type     : zULONG
/**************************************************************************/
zULONG ZColumnMapping::GetAttributeDisplayLength()
{
   return m_ulDisplayLth;
}

/////////////////////////////////////////////////////////////////////////

/****************************************************************************
** class name      : ZColEdit                                              **
** base class      : ZGridColCtrl                                          **
** derived classes : none                                                  **
**                                                                         **
**                                                                         **
** Description     : just like ZGridColCtrl but for documentation purpose  **
**                   and for future use                                    **
**                                                                         **
****************************************************************************/


ZColEdit::ZColEdit()
{
}

ZColEdit::~ZColEdit()
{
}

void ZColEdit::Serialize(CArchive& ar)
{
   ZGridColCtrl::Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////


/****************************************************************************
** class name      : ZColCheckBox                                          **
** base class      : ZGridColCtrl                                          **
** derived classes : none                                                  **
**                                                                         **
**                                                                         **
** Description     : just like ZGridColCtrl plus translation of checked    **
**                   and unchecked state                                   **
**                                                                         **
****************************************************************************/
ZColCheckBox::ZColCheckBox()
{
}

ZColCheckBox::~ZColCheckBox()
{
}

void ZColCheckBox::Serialize(CArchive& ar)
{
   ZGridColCtrl::Serialize( ar );

   if( ar.IsStoring() )
   {
      ar << m_strChecked;
      ar << m_strUnchecked;
   }
   else
   {
      UINT uiSchema = ar.GetObjectSchema();
      switch ( uiSchema )
      {
      case 1:
         ar >> m_strChecked;
         ar >> m_strUnchecked;
         break;
      default:
//         TraceLineI("Failed to read ZColCheckBox using schema: ", uiSchema );
         break;
      }
   }
}


/**************************************************************************/
// Function name   : ZColCheckBox::GetCheckboxTranslationValue
// Description     : returns the value that is assigned to an attribute
//                   if a checkbox is checked/unchecked
// Return type     : CString
// Argument        : zBOOL bChecked TRUE if checked / FALSE if unchecked
/**************************************************************************/
CString ZColCheckBox::GetCheckboxTranslationValue( zBOOL bChecked )
{
   if( bChecked )
      return m_strChecked;
   else
      return m_strUnchecked;
}



/**************************************************************************/
// Function name   : ZColCheckBox::SetCheckboxTranslationValues
// Description     : Sets checked/unchecked values that are used to translate
//                   a checked/unchecked Checkbox
// Return type     : void
// Argument        : zCPCHAR pszCheckedValue    Value to be returned if a checkbox
//                                              is checked
// Argument        : zCPCHAR pszUncheckedValue  Value to be returned if a checkbox
//                                              is unchecked
/**************************************************************************/
void ZColCheckBox::SetCheckboxTranslationValues( zCPCHAR pszCheckedValue, zCPCHAR pszUncheckedValue )
{
   m_strChecked   = pszCheckedValue;
   m_strUnchecked = pszUncheckedValue;

   if( m_strUnchecked == m_strChecked )
      TRACE1( "Values are the same for checked and unchecked State of ZColCheckBox. Value is %s:", m_strChecked );
}

/////////////////////////////////////////////////////////////////////////


/****************************************************************************
** class name      : ZColRadioGrp                                          **
** base class      : ZGridColCtrl                                          **
** derived classes : none                                                  **
**                                                                         **
**                                                                         **
** Description     : just like ZGridColCtrl plus acting as a container for **
**                   radio buttons currently it is only used for           **
**                   serialization at designtime                           **
****************************************************************************/
ZColRadioGrp::ZColRadioGrp()
{
}



ZColRadioGrp::~ZColRadioGrp()
{
}

void ZColRadioGrp::Serialize(CArchive& ar)
{
   ZGridColCtrl::Serialize( ar );
   if ( ar.IsStoring() )
   {
      m_lRadioButtons.Serialize( ar );
   }
   else
   {
      UINT uiSchema = ar.GetObjectSchema();
      switch ( uiSchema )
      {
      case 1:
         m_lRadioButtons.Serialize( ar );
         break;
      default:
         break;
      }
   }
}

POSITION ZColRadioGrp::GetFirstRadioButtonPos()
{
   return m_lRadioButtons.GetHeadPosition();
}

ZColRadioBtn * ZColRadioGrp::GetNextRadioButton(POSITION &pos)
{
   return (ZColRadioBtn *)m_lRadioButtons.GetNext( pos );
}


/////////////////////////////////////////////////////////////////////////


/****************************************************************************
** class name      : ZColRadioBtn                                          **
** base class      : CObject                                               **
** derived classes : none                                                  **
**                                                                         **
** Description     : to hold information about the value of a checked      **
**                   button currently it is only used for                  **
**                   serialization at designtime                           **
****************************************************************************/
ZColRadioBtn::ZColRadioBtn()
{
   m_strValue.Empty();
   m_bIsDefault = FALSE;
}

ZColRadioBtn::~ZColRadioBtn()
{
}

void ZColRadioBtn::Serialize(CArchive& ar)
{
   if( ar.IsStoring() )
   {
         ar << m_strValue;
         ar << m_strDisplayValue;
         ar << m_bIsDefault;
   }
   else
   {
      UINT uiSchema = ar.GetObjectSchema();
      switch ( uiSchema )
      {
      case 1:
         ar >> m_strValue;
         ar >> m_strDisplayValue;
         ar >> m_bIsDefault;
         break;
      default:
//         TraceLineI("Failed to read ZColRadioBtn using schema: ", uiSchema );
         break;
      }
   }
}

/****************************************************************************
** class name      : ZColComboBox                                          **
** base class      : ZGridColCtrl                                          **
** derived classes : none                                                  **
**                                                                         **
**                                                                         **
** Description     : just like ZGridColCtrl plus additional mappings for   **
**                   the list portion of a combobox, browse and scope      **
**                   entities to display only requestes data               **
****************************************************************************/


ZColComboBox::ZColComboBox()
{
   m_lFormatType = 0;
   m_pListMapping = NULL;
   m_bListLoaded = FALSE;
}

ZColComboBox::~ZColComboBox()
{
   if ( m_pListMapping )
      delete m_pListMapping;
}

void ZColComboBox::Serialize(CArchive& ar)
{
   ZGridColCtrl::Serialize( ar );

   if( ar.IsStoring() )
   {
      ar << m_lFormatType;
      ar << m_pListMapping;
      ar << m_strBrowseEntity;
      ar << m_strBrowseScopeEntity;
      ar << m_strOnlySelectedView;
   }
   else
   {
      UINT uiSchema = ar.GetObjectSchema();
      switch ( uiSchema )
      {
      case 1:
         ar >> m_lFormatType;
         ar >> m_pListMapping;
         ar >> m_strBrowseEntity;
         ar >> m_strBrowseScopeEntity;
         ar >> m_strOnlySelectedView;
         break;
      default:
         TraceLineI("Failed to read ZColComboBox using schema: ", uiSchema );
         break;
      }
   }
}

zLONG ZColComboBox::GetFormatType()
{
   return m_lFormatType;
}


/**************************************************************************/
// Function name   : ZColComboBox::SetFormatType
// Description     : Sets the formattype of a combobox
// Return type     : zLONG the former formattype
// Argument        : zLONG lFormatType the new formattype to be set
//                   see following defines in zdctl.h
//                        zCOMBOBOX_DOMAIN_LIST
//                        zCOMBOBOX_POSITION_ONLY
//                        zCOMBOBOX_SELECT
//                        zCOMBOBOX_INCLUDE
//                        zCOMBOBOX_FOREIGN_KEY
/**************************************************************************/
zLONG ZColComboBox::SetFormatType( zLONG lFormatType )
{
   zLONG lOldFormatType = m_lFormatType;
   m_lFormatType = lFormatType;
   return lOldFormatType;
}


/**************************************************************************/
// Function name   : ZColComboBox::SetListMapping
// Description     : Sets the mapping for the list portion of a combobox
// Return type     : void
// Argument        : zCPCHAR pszView         pointer to a views name
// Argument        : zCPCHAR pszEntity       pointer to an entityname name
//                                           within the view above
// Argument        : zCPCHAR pszAttribute    pointer to an attributename name
//                                           within the view entity above
// Argument        : zCPCHAR pszContext      pointer to a contextname of the
//                                           attribute above
/**************************************************************************/
void ZColComboBox::SetListMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext)
{
   if ( m_pListMapping )
      delete m_pListMapping;
   m_pListMapping = new ZColumnMapping( pszView, pszEntity, pszAttribute, pszContext );
}


/**************************************************************************/
// Function name   : ZColComboBox::SetBrowseScopeEntity
// Description     : Set the scoping entity for the listbox portion
//                   it is to limit the number of entries in the combobox
// Return type     : void
// Argument        : zCPCHAR pszEntity
/**************************************************************************/
void ZColComboBox::SetBrowseScopeEntity( zCPCHAR pszEntity)
{
   m_strBrowseScopeEntity = pszEntity;
}

void ZColComboBox::SetBrowseEntity( zCPCHAR pszEntity)
{
   m_strBrowseEntity = pszEntity;
}


/**************************************************************************/
// Function name   : ZColComboBox::SetOnlySelectedView
// Description     : Sets the viewname that is used for displaying only
//                   selected entities
// Return type     : void
// Argument        : zCPCHAR pszView - Viewname
/**************************************************************************/
void ZColComboBox::SetOnlySelectedView( zCPCHAR pszView )
{
   m_strOnlySelectedView = pszView;
}


/**************************************************************************/
// Function name   : ZColComboBox::SetListLoaded
// Description     : Sets a flag to remember whether the list of entries has
//                   already been filled
// Return type     : void
// Argument        : zBOOL bLoaded - TRUE is loaded Otherwise FALSE
/**************************************************************************/
void ZColComboBox::SetListLoaded(zBOOL bLoaded)
{
   m_bListLoaded = bLoaded;
}


/****************************************************************************
** class name      : ZColumns                                              **
** base class      : CObList                                               **
** derived classes : none                                                  **
**                                                                         **
**                                                                         **
** Description     : just a container to hold all Zeidon specific column   **
**                   informations. All entries in this list shoud be       **
**                   derived from from ZGridColCtrl                        **
****************************************************************************/

ZColumns::ZColumns()
{
}

ZColumns::~ZColumns()
{
   while( GetCount() > 0 )
   {
      CObject *p = RemoveHead();
      delete p;
   }
}

void ZColumns::Serialize(CArchive& ar)
{
   CObList::Serialize( ar );

   if (ar.IsStoring())
   {
   }
   else
   {
   }
}


#ifdef _DEBUG

void ZColumns::Dump(CDumpContext& dc) const
{
   CObList::Dump( dc );
   POSITION pos = GetHeadPosition();
   while ( pos )
   {
      CObject* p = (CObject *) GetNext( pos );
      if( p )
         p->Dump( dc );
   }
}

#endif //_DEBUG




