/********************************************************************************************/
//
// File:            tgcolser.h
// Copyright:       TONBELLER AG2001
// Autor:           TMV
// Datum:           8. Februar 2001
// describtion:     Defines helper classes for mfc serialization of Zeidon Grid properties
//
//
// list of classes :
// Name:             describtion:
// ------------------------------------------------------------------------------------
// ZColumnMapping    Holds Mapping Information ( View, Entity, Attribute, Context )
// ZGridColCtrl      Base class for all controltypes representing a column
// ZColEdit          Edit ctrl representation class of a column
// ZColCheckBox      Checkbox ctrl representation class of a column
// ZColComboBox      Combobox ctrl representation class of a column
// ZColRadioGrp      RadioGroup ctrl representation class of a column
//                   holds a list of ZColRadioBtn
// ZColRadioBtn      holds information about the translation a checked Radiobutton
//
/* Change log most recent first order:

   2001.2.8   TMV   2000   ActiveX
      created






*/
/*********************************************************************************************/

#ifndef __TG_COLUMN_SERIALIZABLE__
#define __TG_COLUMN_SERIALIZABLE__

#define VC_EXTRALEAN
#include "zstdafx.h"
#include <kzoegdef.h>

#define ZDCTL_CLASS AFX_EXT_CLASS

#include "..\..\..\zdr\Zdr.h"
#include "..\..\..\zdr\Zdctl.h"


class ZDCTL_CLASS ZColumnMapping : public CObject
{
     DECLARE_SERIAL( ZColumnMapping )
public:
   void InitMappingAttributeLength( zVIEW vApp );

   ZColumnMapping();
   // copy constructor
   ZColumnMapping(const ZColumnMapping& ColumnMapping);
   ZColumnMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);

   virtual ~ZColumnMapping();

   virtual void Serialize(CArchive& ar);

   CString GetMapViewName() const      { return m_strVName; }
   CString GetMapEntityName() const    { return m_strEName; }
   CString GetMapAttributeName() const { return m_strAName; }
   CString GetMapContextName() const   { return m_strContext; }
   zULONG GetAttributeDisplayLength();

   void SetMapViewName( zCPCHAR pszValue )      { m_strVName = pszValue ; }
   void SetMapEntityName( zCPCHAR pszValue )    { m_strEName = pszValue ; }
   void SetMapAttributeName( zCPCHAR pszValue ) { m_strAName = pszValue ; }
   void SetMapContextName( zCPCHAR pszValue )   { m_strContext = pszValue ; }
   void SetMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);


#ifdef _DEBUG

   void Dump(CDumpContext& dc) const
   {
      CObject::Dump( dc );
      dc << m_strVName   << "\n";
      dc << m_strEName   << "\n";
      dc << m_strAName   << "\n";
      dc << m_strContext << "\n";
   }

#endif

private:
   CString m_strVName;        // View name to application map object
   CString m_strEName;        // Entity name within map object
   CString m_strAName;        // Attribute name within map object
   CString m_strContext;      // Domain context for this attribute
   zULONG  m_ulDisplayLth;    // displaylength of attribute used at runtime not at designtime
                              // so it will not be serialized

};

class ZDCTL_CLASS ZGridColCtrl : public CObject
{
     DECLARE_SERIAL( ZGridColCtrl )
public:

   ZGridColCtrl (){ m_pMapping = NULL; }
   virtual ~ZGridColCtrl ();
   virtual void Serialize(CArchive& ar);
   void SetMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);

   operator ZColumnMapping*() const;

   ZColumnMapping* GetColumnMapping()  { return m_pMapping; }
   CString GetMapViewName() const;
   CString GetMapEntityName() const;
   CString GetMapAttributeName() const;
   CString GetMapContextName() const;
   zULONG GetAttributeDisplayLength();


#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      CObject::Dump(dc);
      m_pMapping->Dump( dc );
   }
#endif

private:
   ZColumnMapping *m_pMapping;

};

class ZDCTL_CLASS ZColEdit : public ZGridColCtrl
{
     DECLARE_SERIAL( ZColEdit )
public:

   ZColEdit();
   virtual ~ZColEdit();
   virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      ZGridColCtrl::Dump(dc);
   }
#endif

};

class ZDCTL_CLASS ZColCheckBox : public ZGridColCtrl
{
     DECLARE_SERIAL( ZColCheckBox )
public:

   ZColCheckBox();
   virtual ~ZColCheckBox();

   virtual void Serialize(CArchive& ar);

   CString GetCheckboxTranslationValue( zBOOL bChecked = TRUE );
   void SetCheckboxTranslationValues( zCPCHAR pszCheckedValue, zCPCHAR pszUncheckedValue );

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      ZGridColCtrl::Dump(dc);
      dc << "\t" << "Unchecked value is " << m_strUnchecked << "\n";
      dc << "\t" << "checked value is  "<< m_strChecked << "\n";
   }
#endif

private:
   CString m_strChecked, m_strUnchecked;
};


class ZDCTL_CLASS ZColRadioBtn : public CObject
{
     DECLARE_SERIAL( ZColRadioBtn )
public:

   ZColRadioBtn();
   virtual ~ZColRadioBtn();

   virtual void Serialize(CArchive& ar);

   void SetCheckValue( zCPCHAR szValue ){ m_strValue = szValue; }
   void SetDisplayValue( zCPCHAR szValue ){ m_strDisplayValue = szValue; }
   CString GetCheckValue(){ return m_strValue; }
   CString GetDisplayValue(){ return m_strDisplayValue; }
   zBOOL IsDefault( ) { return m_bIsDefault; }
   void SetDefault ( zBOOL bSet ) { m_bIsDefault = bSet; }

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      CObject::Dump(dc);
      dc << "\t" << "Value is " << m_strValue << "\n";
      dc << "\t" << "Default is ";
      if ( m_bIsDefault )
         dc << "TRUE" ;
      else
         dc << "FALSE";
      dc << "\n";
   }
#endif


private:
   CString m_strValue;
   CString m_strDisplayValue;
   zBOOL m_bIsDefault;
};

class ZDCTL_CLASS ZColComboBox : public ZGridColCtrl
{
     DECLARE_SERIAL( ZColComboBox )
public:
   zLONG SetFormatType( zLONG lFormatType );
   zLONG GetFormatType();

   // sets the mapping of the listbox part of a combobox
   void SetListMapping( zCPCHAR pszView, zCPCHAR pszEntity, zCPCHAR pszAttribute, zCPCHAR pszContext);
   void SetBrowseScopeEntity( zCPCHAR pszEntity);
   void SetBrowseEntity( zCPCHAR pszEntity);
   void SetOnlySelectedView( zCPCHAR szEntity );

   // returns the mapping of the listbox part of a combobox
   ZColumnMapping *GetListMapping( ) { return m_pListMapping; }
   CString GetBrowseScopeEntity( ) { return m_strBrowseScopeEntity; }
   CString GetBrowseEntity( ) { return m_strBrowseEntity; }

   ZColComboBox();
   virtual ~ZColComboBox();

   virtual void Serialize(CArchive& ar);

   // methods only used at runtime

   // call SetListLoaded( TRUE ) after the first time all values have been retrieved
     // call SetListLoaded( FALSE ) each time the control is refreshed
     void SetListLoaded( zBOOL bLoaded = TRUE );
     // call IsListLoaded( ) to determine whether a reaload has to occur
     zBOOL IsListLoaded( ){ return m_bListLoaded; }


#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      ZGridColCtrl::Dump(dc);
      dc << "\t" << "Formattype is " << m_lFormatType << "\n";
   }
#endif

private:
   zBOOL m_bListLoaded;
   zLONG m_lFormatType;
   // additional mappings for list and scope
   ZColumnMapping *m_pListMapping;
   CString m_strBrowseScopeEntity;
   CString m_strBrowseEntity;
   CString m_strOnlySelectedView;
};


class ZDCTL_CLASS ZColumns : public CObList
{
     DECLARE_SERIAL( ZColumns )
public:

   ZColumns();
   virtual ~ZColumns();
   POSITION GetFirstColumnPosition(){ return GetHeadPosition(); }
   ZGridColCtrl *GetNextColumn( POSITION &pos ){ return (ZGridColCtrl *)GetNext(pos); }
     virtual void Serialize(CArchive& ar);

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const;
#endif

};

class ZDCTL_CLASS ZColRadioGrp : public ZGridColCtrl
{
     DECLARE_SERIAL( ZColRadioGrp )
public:
   ZColRadioBtn * GetNextRadioButton( POSITION& pos );
   POSITION GetFirstRadioButtonPos();

   ZColRadioGrp();
   virtual ~ZColRadioGrp();

   virtual void Serialize(CArchive& ar);
   void AddButton( ZColRadioBtn *pButton )
   {
      m_lRadioButtons.AddTail( pButton );
   }

#ifdef _DEBUG
   virtual void Dump(CDumpContext& dc) const
   {
      ZGridColCtrl::Dump( dc );
      m_lRadioButtons.Dump( dc );
   }
#endif

private:
   ZColumns m_lRadioButtons;
};


#endif //__TG_COLUMN_SERIALIZABLE__
