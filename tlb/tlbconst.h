#ifndef _TLB_TRANSLATION_TABLES
#define _TLB_TRANSLATION_TABLES

#if !defined DISPID_AUTOSIZE
#include <OLECTL.H>
#endif


#define DISPATCH_MAX_PARAM 64
#define CRLF _T("\r\n")
#define TAB  _T("   ")


/////////////////////////////////////////////////////////////////////
//
// constants for generating C/C++ code
//
//
// formatstring for default include in cpp and c file;
LPCSTR lpszDEFAULT_INCLUDE =  "\r\n\
#define zGLOBAL_DATA\r\n\
#include \"%s\"// include Zeidon generated Header\r\n\r\n\
#include \"Zdr.h\"\r\n\
#include \"Zdctl.h\"\r\n\
\r\n\
#ifdef _DEBUG\r\n\
#define new DEBUG_NEW\r\n \
#undef THIS_FILE\r\n  \
static char THIS_FILE[] = __FILE__;\r\n\
#endif\r\n";

// standard include for DLL Extention
LPCSTR lpszMFC_EXTENSION = "\r\n\
#include \"afxdllx.h\"    // standard MFC Extension DLL routines\r\n\r\n\
static AFX_EXTENSION_MODULE NEAR extensionDLL = { 0, 0 };\r\n";

// standard MFC includes
LPCSTR lpszDEFAULT_MFC_INCLUDE = "#define VC_EXTRALEAN\r\n\
#include <afxwin.h>         // MFC core and standard components\r\n\
#include <afxext.h>         // MFC extensions (including VB)\r\n\
#include <afxpriv.h>\r\n\
#include <afxpriv2.h>\r\n\
#include <afxole.h>         // MFC OLE classes\r\n\
#include <afxodlgs.h>       // MFC OLE dialog classes\r\n\
#include <afxdisp.h>        // MFC OLE automation classes\r\n\
#include <vfw.h>\r\n";


LPCSTR lpszDllMain = "\r\n\
extern \"C\" int APIENTRY\r\n\
DllMain( HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved )\r\n\
{\r\n\
   UNREFERENCED_PARAMETER( lpReserved );\r\n\
\r\n\
   if ( dwReason == DLL_PROCESS_ATTACH )\r\n\
   {\r\n\
      g_hInstanceDLL = hInstance;\r\n\
      if ( AfxInitExtensionModule( extensionDLL, hInstance ) == 0 )\r\n\
         return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( dwReason == DLL_PROCESS_DETACH )\r\n\
   {\r\n\
      AfxTermExtensionModule( extensionDLL );\r\n\
   }\r\n\
   return( 1 );   // ok\r\n\
}\r\n";

LPCSTR lpszZeidonInitRoutine = "\r\n\
extern \"C\" void WINAPI\r\n\
ZeidonInit%s( zVIEW vSubtask, zLONG lDlgUnitCharX, zLONG lDlgUnitCharY,\r\n\
              zLONG lMapDlgCharX, zLONG lMapDlgCharY )\r\n\
{\r\n\
   new CDynLinkLibrary( extensionDLL );\r\n\
}\r\n";


LPCSTR lpszZeidonScriptInit = "\r\n\
extern \"C\" zOPER_EXPORT zSHORT OPERATION\r\n\
%sScript( ZMapAct *pzma, zLONG lFlags, zLONG lRequest )\r\n\
{\r\n\
   #define ZCLASS_NAME %s \r\n\
   zSHORT nRC = 0;\r\n\
\r\n\
   if ( pzma == NULL ) // return immediate\r\n\
      return (0);\r\n\
\r\n\
   if ( lRequest == zACTIVEX_INITIALIZE )\r\n\
   {\r\n\
      // Default Zeidon Implementation\r\n\
      // Never change this code unless you want to crash!!!!!\r\n\
      IDispatch *pIDispatch = pzma->GetDispatchInterface();\r\n\
      if ( pIDispatch )\r\n\
      {\r\n\
         ZCLASS_NAME *pWrapperInstance = new ZCLASS_NAME( pIDispatch );\r\n\
         if ( pWrapperInstance )\r\n\
         {\r\n\
            pzma->SetWrapperInstance( pWrapperInstance );\r\n\
            nRC = 0;\r\n\
         }\r\n\
      \r\n\
         if ( nRC == 0 )\r\n\
         // to do:\r\n\
         // add additional initialization code here\r\n\
            return( 0 );\r\n\
      }\r\n\
   \r\n\
   }\r\n\
   else";

LPCSTR lpszZeidonScriptMapping = "\r\n\
   if ( lRequest == zACTIVEX_TRANSFER_DATA_FROM_OI )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // add code to display OI's data with this control\r\n\
      return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_TRANSFER_DATA_TO_OI )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // add code to update OI's data with control's data\r\n\
      return( 0 );\r\n\
   }\r\n\
   else";

LPCSTR lpszZeidonScriptMisc = "\r\n\
   if ( lRequest == zACTIVEX_GET_ATTRIBUTE_COUNT )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // return number of attributes to map here\r\n\
      return( 0 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_RECEIVE_FOCUS )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // handle get focus here\r\n\
      return( 1 );\r\n\
   }\r\n\
   else\r\n\
   if ( lRequest == zACTIVEX_LOSE_FOCUS )\r\n\
   {\r\n\
      ZCLASS_NAME *pWrapperInstance = (ZCLASS_NAME *)pzma->GetWrapperInstance( );\r\n\
      if ( !pWrapperInstance )\r\n\
         return ( 0 );\r\n\
         \r\n\
      // to do:\r\n\
      // handle lose focus here\r\n\
      return( 1 );\r\n\
   }\r\n\
\r\n\
   return( nRC );\r\n\
}\r\n";



LPCSTR lpszZeidonDefinitions = "\r\n\
#include <kzoegdef.h>\r\n\r\n\
zEXTERN HINSTANCE  g_hInstanceDLL;";


LPCSTR lpszDISPATCH_DRIVER_BASE_CLASS  = " COleDispatchDriver";
LPCSTR lpszCPP_CLASS_DECLARATION       = "class %s : public %s\r\n{\r\n";
LPCSTR lpszCPP_TOKEN_PUBLIC            = "public:\r\n";
LPCSTR lpszPARAMTEMPLATE               = "param%d";

// formatstring for Invokehelper call
LPCSTR lpszINVOKE_HELPER               = "InvokeHelper( %s%s%s%s%s%s );";

// formatstring for GetProperty method
LPCSTR lpszGET_PROP_CPP = "%s %s::Get%s()\r\n{\r\n    %s result;\r\n    \
   GetProperty(0x%x, %s, (void*)&result);\r\n   return result;\r\n}";

// formatstring for GetProperty method
LPCSTR lpszSET_PROP_CPP = "void %s::Set%s(%s propVal)\r\n{\r\n    \
   SetProperty(0x%x, %s, propVal);\r\n}";



// Translation table for VT_Constants to C-Stye
static TCHAR* BASED_CODE _szVTtoC[] =
{
    _T("void"),             //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
    _T("null"),             //VT_NULL            = 1,   /* [V]        SQL style Null              */
    _T("short"),            //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
    _T("long"),             //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
    _T("float"),           //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
    _T("double"),           //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
    _T("CURRENCY"),         //VT_CY              = 6,   /* [V][T][P]  currency                    */
    _T("DATE"),             //VT_DATE            = 7,   /* [V][T][P]  date                        */
    _T("BSTR"),             //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
    _T("IDispatch*"),       //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
    _T("SCODE"),            //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
    _T("BOOL"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
    _T("VARIANT"),          //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
    _T("IUnknown*"),        //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
    _T("wchar_t"),          //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
    _T(""),                 //                   = 15,
    _T("char"),             //VT_I1              = 16,  /*    [T]     signed char                 */
    _T("unsigned char"),             //VT_UI1             = 17,  /*    [T]     unsigned char               */
    _T("unsigned short"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
    _T("unsigned long"),            //VT_UI4             = 19,  /*    [T]     unsigned short              */
    _T("int64"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
    _T("uint64"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
    _T("int"),              //VT_INT             = 22,  /*    [T]     signed machine int          */
    _T("unsigned int"),             //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
    _T("void"),             //VT_VOID            = 24,  /*    [T]     C style void                */
    _T("HRESULT"),          //VT_HRESULT         = 25,  /*    [T]                                 */
    _T("PTR"),              //VT_PTR             = 26,  /*    [T]     pointer type                */
    _T("SAFEARRAY"),        //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
    _T("CARRAY"),           //VT_CARRAY          = 28,  /*    [T]     C style array               */
    _T("USERDEFINED"),      //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
    _T("LPSTR"),            //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
    _T("LPWSTR"),           //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
    _T(""),                 //                   = 32,
    _T(""),                 //                   = 33,
    _T(""),                 //                   = 34,
    _T(""),                 //                   = 35,
    _T(""),                 //                   = 36,
    _T(""),                 //                   = 37,
    _T(""),                 //                   = 38,
    _T(""),                 //                   = 39,
    _T(""),                 //                   = 40,
    _T(""),                 //                   = 41,
    _T(""),                 //                   = 42,
    _T(""),                 //                   = 43,
    _T(""),                 //                   = 44,
    _T(""),                 //                   = 45,
    _T(""),                 //                   = 46,
    _T(""),                 //                   = 47,
    _T(""),                 //                   = 48,
    _T(""),                 //                   = 49,
    _T(""),                 //                   = 50,
    _T(""),                 //                   = 51,
    _T(""),                 //                   = 52,
    _T(""),                 //                   = 53,
    _T(""),                 //                   = 54,
    _T(""),                 //                   = 55,
    _T(""),                 //                   = 56,
    _T(""),                 //                   = 57,
    _T(""),                 //                   = 58,
    _T(""),                 //                   = 59,
    _T(""),                 //                   = 60,
    _T(""),                 //                   = 61,
    _T(""),                 //                   = 62,
    _T(""),                 //                   = 63,
    _T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
    _T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
    _T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
    _T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
    _T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
    _T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
    _T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
    _T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
    _T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};


// Translation table used for DispatchDriver... Implementation
static TCHAR* BASED_CODE _szVTtoVTDispatchCPP[] =
{
    _T("EMPTY"),         //VT_EMPTY           = 0,   /* [V]   [P]  nothing                     */
    _T("NULL"),          //VT_NULL            = 1,   /* [V]        SQL style Null              */
    _T("I2"),            //VT_I2              = 2,   /* [V][T][P]  2 byte signed int           */
    _T("I4"),            //VT_I4              = 3,   /* [V][T][P]  4 byte signed int           */
    _T("R4"),            //VT_R4              = 4,   /* [V][T][P]  4 byte real                 */
    _T("R8"),            //VT_R8              = 5,   /* [V][T][P]  8 byte real                 */
    _T("CY"),            //VT_CY              = 6,   /* [V][T][P]  currency                    */
    _T("DATE"),          //VT_DATE            = 7,   /* [V][T][P]  date                        */
    _T("BSTR"),          //VT_BSTR            = 8,   /* [V][T][P]  binary string               */
    _T("DISPATCH"),      //VT_DISPATCH        = 9,   /* [V][T]     IDispatch FAR*              */
    _T("ERROR"),         //VT_ERROR           = 10,  /* [V][T]     SCODE                       */
    _T("BOOL"),          //VT_BOOL            = 11,  /* [V][T][P]  True=-1, False=0            */
    _T("VARIANT"),       //VT_VARIANT         = 12,  /* [V][T][P]  VARIANT FAR*                */
    _T("UNKNOWN"),       //VT_UNKNOWN         = 13,  /* [V][T]     IUnknown FAR*               */
    _T("WBSTR"),         //VT_WBSTR           = 14,  /* [V][T]     wide binary string          */
    _T(""),                 //                   = 15,
    _T("I1"),            //VT_I1              = 16,  /*    [T]     signed char                 */
    _T("UI1"),           //VT_UI1             = 17,  /*    [T]     unsigned char               */
    _T("UI2"),           //VT_UI2             = 18,  /*    [T]     unsigned short              */
    _T("UI4"),           //VT_UI4             = 19,  /*    [T]     unsigned short              */
    _T("I8"),            //VT_I8              = 20,  /*    [T][P]  signed 64-bit int           */
    _T("UI8"),           //VT_UI8             = 21,  /*    [T]     unsigned 64-bit int         */
    _T("INT"),           //VT_INT             = 22,  /*    [T]     signed machine int          */
    _T("UINT"),          //VT_UINT            = 23,  /*    [T]     unsigned machine int        */
    _T("VOID"),          //VT_VOID            = 24,  /*    [T]     C style void                */
    _T("HRESULT"),       //VT_HRESULT         = 25,  /*    [T]                                 */
    _T("PTR"),           //VT_PTR             = 26,  /*    [T]     pointer type                */
    _T("SAFEARRAY"),     //VT_SAFEARRAY       = 27,  /*    [T]     (use VT_ARRAY in VARIANT)   */
    _T("CARRAY"),        //VT_CARRAY          = 28,  /*    [T]     C style array               */
    _T("USERDEFINED"),   //VT_USERDEFINED     = 29,  /*    [T]     user defined type         */
    _T("LPSTR"),         //VT_LPSTR           = 30,  /*    [T][P]  null terminated string      */
    _T("LPWSTR"),        //VT_LPWSTR          = 31,  /*    [T][P]  wide null terminated string */
    _T(""),                 //                   = 32,
    _T(""),                 //                   = 33,
    _T(""),                 //                   = 34,
    _T(""),                 //                   = 35,
    _T(""),                 //                   = 36,
    _T(""),                 //                   = 37,
    _T(""),                 //                   = 38,
    _T(""),                 //                   = 39,
    _T(""),                 //                   = 40,
    _T(""),                 //                   = 41,
    _T(""),                 //                   = 42,
    _T(""),                 //                   = 43,
    _T(""),                 //                   = 44,
    _T(""),                 //                   = 45,
    _T(""),                 //                   = 46,
    _T(""),                 //                   = 47,
    _T(""),                 //                   = 48,
    _T(""),                 //                   = 49,
    _T(""),                 //                   = 50,
    _T(""),                 //                   = 51,
    _T(""),                 //                   = 52,
    _T(""),                 //                   = 53,
    _T(""),                 //                   = 54,
    _T(""),                 //                   = 55,
    _T(""),                 //                   = 56,
    _T(""),                 //                   = 57,
    _T(""),                 //                   = 58,
    _T(""),                 //                   = 59,
    _T(""),                 //                   = 60,
    _T(""),                 //                   = 61,
    _T(""),                 //                   = 62,
    _T(""),                 //                   = 63,
    _T("FILETIME"),         //VT_FILETIME        = 64,  /*       [P]  FILETIME                    */
    _T("BLOB"),             //VT_BLOB            = 65,  /*       [P]  Length prefixed bytes       */
    _T("STREAM"),           //VT_STREAM          = 66,  /*       [P]  Name of the stream follows  */
    _T("STORAGE"),          //VT_STORAGE         = 67,  /*       [P]  Name of the storage follows */
    _T("STREAMED_OBJECT"),  //VT_STREAMED_OBJECT = 68,  /*       [P]  Stream contains an object   */
    _T("STORED_OBJECT"),    //VT_STORED_OBJECT   = 69,  /*       [P]  Storage contains an object  */
    _T("BLOB_OBJECT"),      //VT_BLOB_OBJECT     = 70,  /*       [P]  Blob contains an object     */
    _T("CF"),               //VT_CF              = 71,  /*       [P]  Clipboard format            */
    _T("CLSID"),            //VT_CLSID           = 72   /*       [P]  A Class ID                  */
};




//
// the string returned by this function is the name of the C++ variable type
// that is used when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString VTtoCType( VARTYPE vt )
{
    CString str ;
    vt &= ~0xF000 ;
    if (vt <= VT_CLSID)
       str = _szVTtoC[vt]  ;
    else
       str = "invalid Variable Type";

    return str ;
}



//
// the string returned by this function is the name of the return type
// that is used when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString VTtoDispatchType( VARTYPE vt )
{
    CString str ;
    vt &= ~0xF000 ;
    if (vt <= VT_CLSID)
    {
       str = "VT_";
       str += _szVTtoVTDispatchCPP[vt]  ;
    }
    else
       str = "invalid Variable Type";

    return str ;
}


//
// the string returned by this function is the name of the VARIANT type
// that is used in the array of paramater types
// when calling the function COleDispatchDriver::InvokeHelper
// or IDispatch::Invoke
//
CString VTtoVariantType( VARTYPE vt )
{
   CString str ;

   vt &= ~0xF000 ;

   if (vt <= VT_CLSID)
   {
      str += _szVTtoVTDispatchCPP[vt]  ;
   }
   else
      str = "invalid Variable Type";

   return str ;
}

//
// returns a string for a predefined DISPID if id is a predefined id
// usually these predefined have a negative value and reach from -500 to -804
// and are not numbered subsequently (that is the reason for this ugly implementation below)
//
CString PredefinedDispID(DISPID id)
{
   CString strOut;
   switch ( id )
   {
      case  DISPID_AUTOSIZE :
         strOut = _T("DISPID_AUTOSIZE");
         break;
      case  DISPID_BACKCOLOR :
         strOut = _T("DISPID_BACKCOLOR");
         break;
      case  DISPID_BACKSTYLE :
         strOut = _T("DISPID_BACKSTYLE");
         break;
      case  DISPID_BORDERCOLOR :
         strOut = _T("DISPID_BORDERCOLOR");
         break;
      case  DISPID_BORDERSTYLE :
         strOut = _T("DISPID_BORDERSTYLE");
         break;
      case  DISPID_BORDERWIDTH :
         strOut = _T("DISPID_BORDERWIDTH");
         break;
      case  DISPID_DRAWMODE :
         strOut = _T("DISPID_DRAWMODE");
         break;
      case  DISPID_DRAWSTYLE :
         strOut = _T("DISPID_DRAWSTYLE");
         break;
      case  DISPID_DRAWWIDTH :
         strOut = _T("DISPID_DRAWWIDTH");
         break;
      case  DISPID_FILLCOLOR :
          strOut = _T("DISPID_FILLCOLOR");
         break;
      case  DISPID_FILLSTYLE :
          strOut = _T("DISPID_FILLSTYLE");
         break;
      case  DISPID_FONT :
         strOut = _T("DISPID_FONT");
         break;
      case  DISPID_FORECOLOR :
         strOut = _T("DISPID_FORECOLOR");
         break;
      case  DISPID_ENABLED :
         strOut = _T("DISPID_ENABLED");
         break;
      case  DISPID_HWND :
         strOut = _T("DISPID_HWND");
         break;
      case  DISPID_TABSTOP :
         strOut = _T("DISPID_TABSTOP");
         break;
      case  DISPID_TEXT :
         strOut = _T("DISPID_TEXT");
         break;
      case  DISPID_CAPTION :
         strOut = _T("DISPID_CAPTION");
         break;
      case  DISPID_BORDERVISIBLE :
         strOut = _T("DISPID_BORDERVISIBLE");
         break;
      case  DISPID_APPEARANCE :
         strOut = _T("DISPID_APPEARANCE");
         break;
      case  DISPID_MOUSEPOINTER :
         strOut = _T("DISPID_MOUSEPOINTER");
         break;
      case  DISPID_MOUSEICON :
         strOut = _T("DISPID_MOUSEICON");
         break;
      case  DISPID_PICTURE :
         strOut = _T("DISPID_PICTURE");
         break;
      case  DISPID_VALID :
         strOut = _T("DISPID_VALID");
         break;
      case  DISPID_READYSTATE :
         strOut = _T("DISPID_READYSTATE");
         break;
#ifdef DISPID_LISTINDEX
      case  DISPID_LISTINDEX :
         strOut = _T("DISPID_LISTINDEX");
         break;
      case  DISPID_SELECTED :
         strOut = _T("DISPID_SELECTED");
         break;
      case  DISPID_LIST :
         strOut = _T("DISPID_LIST");
         break;
      case  DISPID_COLUMN :
         strOut = _T("DISPID_COLUMN");
         break;
      case  DISPID_LISTCOUNT :
         strOut = _T("DISPID_LISTCOUNT");
         break;
      case  DISPID_MULTISELECT :
         strOut = _T("DISPID_MULTISELECT");
         break;
      case  DISPID_MAXLENGTH :
         strOut = _T("DISPID_MAXLENGTH");
         break;
      case  DISPID_PASSWORDCHAR :
         strOut = _T("DISPID_PASSWORDCHAR");
         break;
      case  DISPID_SCROLLBARS :
         strOut = _T("DISPID_SCROLLBARS");
         break;
      case  DISPID_WORDWRAP :
         strOut = _T("DISPID_WORDWRAP");
         break;
      case  DISPID_MULTILINE :
         strOut = _T("DISPID_MULTILINE");
         break;
      case  DISPID_NUMBEROFROWS :
         strOut = _T("DISPID_NUMBEROFROWS");
         break;
      case  DISPID_NUMBEROFCOLUMNS :
         strOut = _T("DISPID_NUMBEROFCOLUMNS");
         break;
      case  DISPID_DISPLAYSTYLE :
         strOut = _T("DISPID_DISPLAYSTYLE");
         break;
      case  DISPID_GROUPNAME :
         strOut = _T("DISPID_GROUPNAME");
         break;
      case  DISPID_IMEMODE :
         strOut = _T("DISPID_IMEMODE");
         break;
      case  DISPID_ACCELERATOR :
         strOut = _T("DISPID_ACCELERATOR");
         break;
      case  DISPID_ENTERKEYBEHAVIOR :
         strOut = _T("DISPID_ENTERKEYBEHAVIOR");
         break;
      case  DISPID_TABKEYBEHAVIOR :
         strOut = _T("DISPID_TABKEYBEHAVIOR");
         break;
      case  DISPID_SELTEXT :
         strOut = _T("DISPID_SELTEXT");
         break;
      case  DISPID_SELSTART :
         strOut = _T("DISPID_SELSTART");
         break;
      case  DISPID_SELLENGTH :
         strOut = _T("DISPID_SELLENGTH");
         break;
      case  DISPID_REFRESH :
         strOut = _T("DISPID_REFRESH");
         break;
      case  DISPID_DOCLICK :
         strOut = _T("DISPID_DOCLICK");
         break;
      case  DISPID_ABOUTBOX :
         strOut = _T("DISPID_ABOUTBOX");
         break;
      case  DISPID_ADDITEM :
         strOut = _T("DISPID_ADDITEM");
         break;
      case  DISPID_CLEAR :
         strOut = _T("DISPID_CLEAR");
         break;
      case  DISPID_REMOVEITEM :
         strOut = _T("DISPID_REMOVEITEM");
         break;
      case  DISPID_CLICK :
         strOut = _T("DISPID_CLICK");
         break;
      case  DISPID_DBLCLICK :
         strOut = _T("DISPID_DBLCLICK");
         break;
      case  DISPID_KEYDOWN :
         strOut = _T("DISPID_KEYDOWN");
         break;
      case  DISPID_KEYPRESS :
         strOut = _T("DISPID_KEYPRESS");
         break;
      case  DISPID_KEYUP :
         strOut = _T("DISPID_KEYUP");
         break;
      case  DISPID_MOUSEDOWN :
         strOut = _T("DISPID_MOUSEDOWN");
         break;
      case  DISPID_MOUSEMOVE :
         strOut = _T("DISPID_MOUSEMOVE");
         break;
      case  DISPID_MOUSEUP :
         strOut = _T("DISPID_MOUSEUP");
         break;
      case  DISPID_ERROREVENT :
         strOut = _T("DISPID_ERROREVENT");
         break;
      case  DISPID_READYSTATECHANGE :
         strOut = _T("DISPID_READYSTATECHANGE");
         break;
      case  DISPID_CLICK_VALUE :
         strOut = _T("DISPID_CLICK_VALUE");
         break;
      case  DISPID_RIGHTTOLEFT :
         strOut = _T("DISPID_RIGHTTOLEFT");
         break;
      case  DISPID_TOPTOBOTTOM :
         strOut = _T("DISPID_TOPTOBOTTOM");
         break;
      case  DISPID_THIS :
         strOut = _T("DISPID_THIS");
         break;
      case  DISPID_AMBIENT_BACKCOLOR :
         strOut = _T("DISPID_AMBIENT_BACKCOLOR");
         break;
      case  DISPID_AMBIENT_DISPLAYNAME :
         strOut = _T("DISPID_AMBIENT_DISPLAYNAME");
         break;
      case  DISPID_AMBIENT_FONT :
         strOut = _T("DISPID_AMBIENT_FONT");
         break;
      case  DISPID_AMBIENT_FORECOLOR :
         strOut = _T("DISPID_AMBIENT_FORECOLOR");
         break;
      case  DISPID_AMBIENT_LOCALEID :
         strOut = _T("DISPID_AMBIENT_LOCALEID");
         break;
      case  DISPID_AMBIENT_MESSAGEREFLECT :
         strOut = _T("DISPID_AMBIENT_MESSAGEREFLECT");
         break;
      case  DISPID_AMBIENT_SCALEUNITS :
         strOut = _T("DISPID_AMBIENT_SCALEUNITS");
         break;
      case  DISPID_AMBIENT_TEXTALIGN :
         strOut = _T("DISPID_AMBIENT_TEXTALIGN");
         break;
      case  DISPID_AMBIENT_USERMODE :
         strOut = _T("DISPID_AMBIENT_USERMODE");
         break;
      case  DISPID_AMBIENT_UIDEAD :
         strOut = _T("DISPID_AMBIENT_UIDEAD");
         break;
      case  DISPID_AMBIENT_SHOWGRABHANDLES :
         strOut = _T("DISPID_AMBIENT_SHOWGRABHANDLES");
         break;
      case  DISPID_AMBIENT_SHOWHATCHING :
         strOut = _T("DISPID_AMBIENT_SHOWHATCHING");
         break;
      case  DISPID_AMBIENT_DISPLAYASDEFAULT :
         strOut = _T("DISPID_AMBIENT_DISPLAYASDEFAULT");
         break;
      case  DISPID_AMBIENT_SUPPORTSMNEMONICS :
         strOut = _T("DISPID_AMBIENT_SUPPORTSMNEMONICS");
         break;
      case  DISPID_AMBIENT_AUTOCLIP :
         strOut = _T("DISPID_AMBIENT_AUTOCLIP");
         break;
      case  DISPID_AMBIENT_APPEARANCE :
         strOut = _T("DISPID_AMBIENT_APPEARANCE");
         break;
      case  DISPID_AMBIENT_CODEPAGE :
         strOut = _T("DISPID_AMBIENT_CODEPAGE");
         break;
      case  DISPID_AMBIENT_PALETTE :
         strOut = _T("DISPID_AMBIENT_PALETTE");
         break;
      case  DISPID_AMBIENT_CHARSET :
         strOut = _T("DISPID_AMBIENT_CHARSET");
         break;
      case  DISPID_AMBIENT_TRANSFERPRIORITY :
         strOut = _T("DISPID_AMBIENT_TRANSFERPRIORITY");
         break;
      case  DISPID_AMBIENT_RIGHTTOLEFT :
         strOut = _T("DISPID_AMBIENT_RIGHTTOLEFT");
         break;
      case  DISPID_AMBIENT_TOPTOBOTTOM :
         strOut = _T("DISPID_AMBIENT_TOPTOBOTTOM");
         break;
#endif
      case  DISPID_Name :
         strOut = _T("DISPID_Name");
         break;
      case  DISPID_Delete :
         strOut = _T("DISPID_Delete");
         break;
      case  DISPID_Object :
         strOut = _T("DISPID_Object");
         break;
      case  DISPID_Parent :
         strOut = _T("DISPID_Parent");
         break;
      default:
         strOut.Empty();
         break;
   }
   return strOut;
}



#endif
