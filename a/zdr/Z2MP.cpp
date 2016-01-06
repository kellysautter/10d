// Z2MP.cpp : Defines the initialization routines for the DLL.
//

#include "zstdafx.h"
#include "mpna81.h"
#include "Z2MP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
// Note!
//
//    If this DLL is dynamically linked against the MFC
//    DLLs, any functions exported from this DLL which
//    call into MFC must have the AFX_MANAGE_STATE macro
//    added at the very beginning of the function.
//
//    For example:
//
//    extern "C" BOOL PASCAL EXPORT ExportedFunction( )
//    {
//       AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
//       // normal function body here
//    }
//
//    It is very important that this macro appear in each
//    function, prior to any calls into MFC.  This means that
//    it must appear as the first statement within the
//    function, even before any object variable declarations
//    as their constructors may generate calls into the MFC
//    DLL.
//
//    Please see MFC Technical Notes zTAG_LTH and 58 for additional
//    details.
//

/////////////////////////////////////////////////////////////////////////////
// ZMapPointApp

BEGIN_MESSAGE_MAP(ZMapPointApp, CWinApp)
   //{{AFX_MSG_MAP(ZMapPointApp)
      // NOTE - the ClassWizard will add and remove mapping macros here.
      //    DO NOT EDIT what you see in these blocks of generated code!
   //}}AFX_MSG_MAP
END_MESSAGE_MAP( )

/////////////////////////////////////////////////////////////////////////////
// ZMapPointApp construction

ZMapPointApp::ZMapPointApp( )
{
   // TODO: add construction code here,
   // Place all significant initialization in InitInstance
   m_nFailedOLE = 0;
   m_nMapPointOnHold = 0;
   m_pMapPointApp = 0;
}

ZMapPointApp::~ZMapPointApp( )
{
   // TODO: add construction code here,
   // Place all significant initialization in InitInstance
   mDeleteInit( m_pMapPointApp );
}

void
ZMapPointApp::OnAppExit( )
{
   TraceLineS( "ZMapPointApp::OnAppExit", "" );
}

/////////////////////////////////////////////////////////////////////////////
// The one and only ZMapPointApp object

ZMapPointApp theApp;

/////////////////////////////////////////////////////////////////////////////
// ZMapPointApp initialization

BOOL
ZMapPointApp::InitInstance( )
{
   // Register all OLE server (factories) as running.  This enables the
   // OLE libraries to create objects from other applications.
   OleInitialize( 0 );

   return( TRUE );
}

SHORT
ZMapPointApp::GetMapPointState( )
{
   if ( m_pMapPointApp )
      return( 1 );
   else
      if ( m_nMapPointOnHold == 1 )
         return( -1 );
      else
         return( 0 );
}

void
ZMapPointApp::QuitMapPointApplication( )
{
   if ( m_pMapPointApp )
   {
   // COleVariant SaveChanges, OriginalFormat, RouteDocument;
   // SaveChanges = VARIANT_FALSE;

      m_pMapPointApp->Quit( ); // SaveChanges, OriginalFormat, RouteDocument );
      delete( m_pMapPointApp );
      m_pMapPointApp = 0;
   }
}

void
ZMapPointApp::QuitMapPointApplication( int nHold )
{
   if ( m_pMapPointApp )
   {
   // COleVariant SaveChanges, OriginalFormat, RouteDocument;
   // SaveChanges = VARIANT_FALSE;

      m_pMapPointApp->Quit( ); // SaveChanges, OriginalFormat, RouteDocument );
      delete( m_pMapPointApp );
      m_pMapPointApp = 0;
      m_nMapPointOnHold = nHold;
   }
}

void
ZMapPointApp::SetHoldState( int nHold )
{
   m_nMapPointOnHold = nHold;
}

_Application *
ZMapPointApp::GetMapPointApplication( )
{
   TRY
   {
      if ( m_nFailedOLE > 1 )
         return( m_pMapPointApp );

      if ( m_nMapPointOnHold == 1 )
         return( m_pMapPointApp );

      if ( m_pMapPointApp == 0 )
      {
         if ( m_nMapPointOnHold == 1 )
            return( m_pMapPointApp );

         IDispatch *pDisp; // Main IDispatch pointer.
         CLSID clsid; // Holds CLSID of server after CLSIDFromProgID.
         HRESULT hr; // General error/result holder.

         // Get CLSID for MapPoint.Application from registry.
         hr = CLSIDFromProgID( L"MapPoint.Application", &clsid );
         if ( FAILED( hr ) )
         {
            return( 0 );
         // AfxThrowOleException( hr );
         }
         // Start MapPoint and get its IDispatch pointer.
         //CoInitialize( 0 );
         hr = CoCreateInstance( clsid, 0, CLSCTX_LOCAL_SERVER,
                                IID_IDispatch, (void **) &pDisp);
         if ( FAILED( hr ) )
         {
            AfxThrowOleException( hr );
         }

         m_pMapPointApp = new _Application( pDisp );
      // Documents docs( m_pMapPointApp->GetDocuments( ) );
      // docs.Add( );
      }
   }
   CATCH( COleException, e )
   {
      if ( m_pMapPointApp )
      {
         delete m_pMapPointApp;
         m_pMapPointApp = 0;
      }

      m_nFailedOLE = m_nFailedOLE++;
      e->ReportError( );
      e->Delete( );
   }
   END_CATCH

   return( m_pMapPointApp );
}

#if 0
Function SpellTest(ByVal sMapPoint As String) As Boolean

  Dim sSuggestion As String, nSuggestions As Integer

  Dim aDoc As MapPoint.Document
  Dim oSSs As MapPoint.SpellingSuggestions
  Dim oSS As MapPoint.SpellingSuggestion

  'Debug.Print "MapPoint.CheckSpelling(" & sMapPoint & ") = " & CStr(MapPoint.CheckSpelling(sMapPoint))
  If MapPoint.CheckSpelling(sMapPoint, , False) Then
     SpellTest = True
     Debug.Print sMapPoint & " is OK."
  Else
     SpellTest = False
     Debug.Print sMapPoint & " is misspelled."
     Set aDoc = Documents.Add
     Set oSSs = GetSpellingSuggestions(sMapPoint, , False)
     For Each oSS In oSSs
        Debug.Print "  Suggested MapPoint: " & oSS.Name
     Next
     nSuggestions = oSSs.Count
     If nSuggestions = 0 Then
        sSuggestion = ""
        Debug.Print "  No Suggestions"
     Else
        'sSuggestion = oSSs.Item(1).Name
        Debug.Print "  " & nSuggestions & " Suggestions"
     End If
     Set oSSs = Nothing
     Set aDoc = Nothing
  End If
End Function
#endif

#if 0
BOOL ZMapPointApp::SpellTest(LPCTSTR lpszMapPoint)
{
   BOOL bReturn = FALSE;
   _Application *pApp = GetMapPointApplication( );
   if ( pApp == 0 )
      return( bReturn );

   TRY
   {
      COleVariant CustomDictionary, IgnoreUppercase;
      IgnoreUppercase = VARIANT_FALSE;
      if ( m_pSpellSug )
         delete( m_pSpellSug );

      m_pSpellSug = new SpellingSuggestions( pApp->GetSpellingSuggestions( lpszMapPoint,
                                                                           CustomDictionary,
                                                                           IgnoreUppercase ) );

      if ( m_pSpellSug->GetCount( ) > 0 )
         bReturn = FALSE;
      else
         bReturn = TRUE;
   }
   CATCH( COleException, e )
   {
      if ( m_pMapPointApp )
      {
         delete( m_pMapPointApp );
         m_pMapPointApp = 0;
      }

      e->ReportError( );
      e->Delete( );
   }
   END_CATCH

   return( bReturn );
}

LONG ZMapPointApp::EnumSpellingSuggestion( )
{
   if ( m_pSpellSug )
      return( m_pSpellSug->GetCount( ) );

   return( 0 );
}

SHORT ZMapPointApp::GetSpellingSuggestion( LONG lIndex, LPSTR lpszBuffer, SHORT nBuffLth )
{
   SHORT nReturn = -16;
   if ( m_pSpellSug == 0 )
      return( -16 );

   if ( m_pSpellSug->GetCount( ) < lIndex )
      return( -8 );

   TRY
   {
      memset( lpszBuffer, 0, nBuffLth );
      SpellingSuggestion ss( m_pSpellSug->Item( lIndex ) );
      CString strHelp = ss.GetName( );
      strncpy( lpszBuffer, strHelp, nBuffLth - 1 );
      nReturn = strHelp.GetLength( );
   }
   CATCH( COleException, e )
   {
      if ( m_pMapPointApp )
      {
         delete m_pMapPointApp;
         m_pMapPointApp = 0;
      }

      e->ReportError( );
      e->Delete( );
   }
   END_CATCH

   return( nReturn );
}
#endif
