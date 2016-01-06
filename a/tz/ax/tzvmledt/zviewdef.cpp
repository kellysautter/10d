#include "zstdafx.h"
#include "zviewdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


ZLod::ZLod( LPCSTR lpszName, LPCSTR lpcstrDescribtion )
{
   if( lpszName ){
      m_strName = lpszName;
   }
   else{
      m_strName.Empty();
   }
   if( lpcstrDescribtion ){
      m_strDescribtion = lpcstrDescribtion;
   }
   else{
      m_strDescribtion.Empty();
   }
}

ZLod::~ZLod()
{
   POSITION pos = m_mapEntities.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZENTITY lpEntity;
      m_mapEntities.GetNextAssoc( pos, strKey, (CObject*&)lpEntity );
      delete lpEntity;
   }
   m_mapEntities.RemoveAll( );
}

ZEntity::ZEntity( LPCSTR lpszName, LPCSTR lpcstrDescribtion )
{
   if( lpszName ){
      m_strName = lpszName;
   }
   else{
      m_strName.Empty();
   }
   if( lpcstrDescribtion ){
      m_strDescribtion = lpcstrDescribtion;
   }
   else{
      m_strDescribtion.Empty();
   }
}

ZEntity::~ZEntity( )
{
   POSITION pos = m_mapAttributes.GetStartPosition( );
   while (pos != NULL)
   {
      CString strKey;
      LPZATTRIBUTE lpAttribute;
      m_mapAttributes.GetNextAssoc( pos, strKey, (CObject*&)lpAttribute );
      delete lpAttribute;
   }
   m_mapAttributes.RemoveAll( );
}



ZAttribute::ZAttribute( LPCSTR lpszName, LPCSTR lpcstrDescribtion )
{
   if( lpszName ){
      m_strName = lpszName;
   }
   else{
      m_strName.Empty();
   }
   if( lpcstrDescribtion ){
      m_strDescribtion = lpcstrDescribtion;
   }
   else{
      m_strDescribtion.Empty();
   }
}

ZAttribute::~ZAttribute( )
{
}
