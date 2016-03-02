/*
// MODULE NAME:  TZSIOPRS
//
// DESCRIPTION:  Functions for handling Siron-Katalogs
//
// AUTHOR:  ??
// DATE:    ??.??.????
*/

/*
CHANGE LOG
2000.11.07  RG   Z2000
    New function: zwTZSIOPRS_CreateDataModel
2000.02.04  RG   VSAM
    New Operation: SearchSpecialCharacters
1999.10.29  DKS  Cosmetic.
1999.10.28  RG  Z9J, Z2000    VSAM
   Changed loading of local catalogs to DISAM
1999.07.22 - 1999.08.23 RG  Z9J, Z2000    VSAM
   New Operations for the new VSAM Handling
From here old version
97/03/07    RG
   Changed the parameter 'Zeidon System' in the call
   of SfCreateSubtask in function GetWorkObjPathName
   to 'Zeidon_Tools'
97/08/13    RG
   Changed function GetSironWorkObjPathName
   Inserted the call of getenv function instead of
   GetApplDirectoryFromView to get the pathname of ZEIDON environment
   variable. The *.wsk files will be stored there.
*/


#define KZSYSSVC_INCL
#include "KZOENGAA.H"
#include "tzlodopr.h"
#include "tz__oprs.h"
#include "tzlodopr.hg"
#include "ZeidonOp.H"

///---------------------------------------//
 //   Siron Header
 //---------------------------------------*/
#include <string.h>
#include <stdlib.h>

#include "hzcmsgci.h" // the interface to the Siron Reports
#include "kzlddll.h"

/*----------------------------------------------------------------------
// .prototypes
//----------------------------------------------------------------------
*/

// function prototypes
static zSHORT fnLoadHzvaffz( zVIEW vSubtask );
static zSHORT GetLocalCatalogs( zVIEW vSubtask, zVIEW vAktView );
static zSHORT GetAnzahlCatalogFiles( zVIEW vSubtask, zVIEW vAktView );
static zSHORT GetCatalogs( zVIEW vSubtask, zVIEW vAktView );

// internal prototypes for Siron calls
short (PDLL_ENTRY pfnOpenEnv)();           // hzvaOpenEnv
short (PDLL_ENTRY pfnFreeEnv)();           // hzvaFreeEnv
short (PDLL_ENTRY pfnGetAnzahlServerNames)(); // hzvaGetAllServerNames
short (PDLL_ENTRY pfnGetAllServerNames)(); // hzvaGetAllServerNames
short (PDLL_ENTRY pfnGetLocalCatalogs)();  // hzvaGetAllLocalCatalogs
short (PDLL_ENTRY pfnGetServerCatalogs)();  // hzvaGetAllServerCatalogs
short (PDLL_ENTRY pfnGetAnzahlCatalogFiles)();  // hzvaGetAnzahlCatalogFiles
short (PDLL_ENTRY pfnGetAllCatalogFiles)();  // hzvaGetAnzahlCatalogFiles
short (PDLL_ENTRY pfnReadCatalog)();  // hzvaReadCatalog
short (PDLL_ENTRY pfnFreeCatalogStructs)();  // hzvaFreeCatalogStructs
short (PDLL_ENTRY pfnWriteWSK)();  // hzvaWriteWSK
short (PDLL_ENTRY pfnGetPathname)();  // hzvaGetPathname
short (PDLL_ENTRY pfnReadServerCatalog)(); // hzvsReadServerCatalogs
short (PDLL_ENTRY pfnFreeListMemory)(); // hzvsReadServerCatalogs

/*----------------------------------------------------------------------
// .globals
//----------------------------------------------------------------------
*/
static LPLIBRARY g_hLib;           // HZVAFFZ.DLL Laden

///-------------------------------------------------------------------//
 //                                                                   //
 //  .typedefs                                                        //
 //                                                                   //
 //-------------------------------------------------------------------*/

/*
//---------------------------------------------------------------------
//
// .Funktionsname:   >>>GetWorkObjPathName
// .Beschreibung:
//     Build Pathname for Siron Catalog Object
//
// .Descriptor:
// .Parameter:
//    Datatype,  Name,     (I/O/U),   Description
//    PCHAR    pszPathName   (U)      Pointer to Pathname variable
//    LONG     lPathName     (I)      max length of Pathname variable
//
// .Funktionswert:
//   0 ==> OK
//   -1 ==> Error
//
//---------------------------------------------------------------------
*/
zOPER_EXPORT zSHORT OPERATION
GetSironWorkObjPathName( zPCHAR pszPathName, zLONG lPathName )
{
  if ( pszPathName == NULL )
       return( -1 );

  if ( lPathName < 1 )
       return( -1 );

  pfnGetPathname( pszPathName );
  TraceLineS( "GetSironWorkObjPathName return string: ", pszPathName );

  return( 0 );
}

/*
//---------------------------------------------------------------------
//
// .Funktionsname:   >>>CreateSironWorkObjectList
// .Beschreibung:
//     Build a temporary object containing a list of Siron import work object
//     entries.  An entry in the list will be created from each existing work
//     object file in the directory passed by using the Window interface:
//     Getxxxxxxx
//     The resulting list object will be named "TZSIIMLO"
//
// .Descriptor:
// .Parameter:
//    PCHAR    pszPathName   (U)      Pointer to Directory path
//
// .Funktionswert:
//    Number of entries in list object.
//
//---------------------------------------------------------------------
*/

zOPER_EXPORT zSHORT OPERATION
CreateSironWorkObjectList( zPCHAR pchDirectoryPath, zVIEW vSubtask )
{
   zVIEW  vImportList;
   zSHORT nRC;
   HWND   hWndList;
   zSHORT i, nCount;
   zCHAR  szMetaName[ 13 ];
   HWND   hWnd;
   HINSTANCE hInst;

   nRC = GetViewByName( &vImportList, "TZSIIMLO", NULL, zLEVEL_TASK );
   if ( nRC < 0 )
   {
      // Initialize empty list object.
      ActivateEmptyObjectInstance( &vImportList, "TZSIIMLO", vSubtask, zSINGLE );
      CreateEntity( vImportList, "Root", zPOS_AFTER );
      SetNameForView( vImportList, "TZSIIMLO",NULL, zLEVEL_TASK );
   }

   // Using the Windows interface, create an invisible window which
   // contains the list of all files in the directory pased of type *.wsk.
   // The *.wsk is passed to Windows to delineate the files listed.

   strcat_s( pchDirectoryPath, zMAX_FILENAME_LTH + 1, "*.WSK" );

   GetWindowHandle( (zPULONG) &hWnd, 0, vSubtask, 0 );
   hInst = (HINSTANCE) GetApplicationInstance( vSubtask );

   hWndList = CreateWindow( "ListBox", NULL, WS_CHILD | LBS_SORT, 1, 1, 1, 1,
                            hWnd, (HMENU) 101, (HANDLE) hInst, NULL );

   SendMessage( hWndList, WM_SETREDRAW, FALSE, 0L );
   SendMessage( hWndList, LB_DIR, 0x0000, (LONG)(LPSTR) pchDirectoryPath );
   nCount = (int) SendMessage( hWndList, LB_GETCOUNT, 0, 0L );

   if ( nCount <= 0 )
   {
      // If there doesn't exist a WSK file disable the buttons OK and Delete
      SetCtrlState(vSubtask, "ImportWSK", zCONTROL_STATUS_ENABLED, FALSE);
      SetCtrlState(vSubtask, "DeleteWSK", zCONTROL_STATUS_ENABLED, FALSE);

      return( 0 );
   }

   for ( i = 0; i < nCount; i++ )
   {
      SendMessage( hWndList, LB_GETTEXT, i, (LONG)(LPSTR) szMetaName );
      TraceLineS( "//// List Meta Name: ", szMetaName );
      /*
       * remove 4 Bytes at the end of the string (.wsk)
       */
      *(szMetaName + zstrlen( szMetaName ) -4 ) = 0;

      // Create the List entity
      CreateEntity( vImportList, "KatalogFile", zPOS_AFTER );
      SetAttributeFromString( vImportList, "KatalogFile", "Name", szMetaName );
   }

   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GetAllServerNames
//
zOPER_EXPORT zSHORT OPERATION
GetAllServerNames( zVIEW vSubtask, zVIEW vAktView )
{
   zSHORT nRet;
   void   *pEnv = NULL;
   zCHAR  *pszServerList = NULL;
   long   lAnzServer;
   long   lHMem;
   long   lLv;
   unsigned long  lLocalLength;
   zCHAR  szLocalServer[MAX_COMPUTERNAME_LENGTH+1];

   // If the serverlist still exists, return
   nRet = CheckExistenceOfEntity( vAktView, "TZSIIMWK");
   if ( nRet == zCURSOR_SET || nRet == zCALL_ERROR )
      return( nRet );

   // Load hzvaffz.dll and get the function pointers
   if ( g_hLib != NULL )
   {
      nRet = fnLoadHzvaffz (vSubtask);
      if (nRet != 0 )
         return( nRet );
   }

   nRet = pfnOpenEnv( &pEnv );
   if ( nRet != 0 )
       return( nRet );

   nRet = pfnGetAnzahlServerNames (pEnv, &lAnzServer);
   if ( nRet != 0 )
   {
      pfnFreeEnv (pEnv);
      return( nRet );
   }

   lAnzServer += 1;   // The local servername is not in the list

   // Get Memory for the whole serverlist
   // max. length of a servername shoud be 9 characters
   lHMem = SysAllocMemory( &pszServerList, lAnzServer * 9, 0, zCOREMEM_ALLOC, 0 );
   if ( lHMem == 0 )
   {
      pfnFreeEnv( pEnv );
      return( 551 );
   }

   // Now get the Local Server Name
   lLocalLength = MAX_COMPUTERNAME_LENGTH + 1;
   memset(szLocalServer, 0, MAX_COMPUTERNAME_LENGTH + 1);
   if (GetComputerName( szLocalServer, &lLocalLength ) == 0 )
   {
      pfnFreeEnv( pEnv );
      return( -3 );
   }

   // Put the local Server name into the list
   memset( pszServerList, 0, lAnzServer * 9 );
   memcpy( pszServerList, szLocalServer, 9 );

   // Get all Servernames
   nRet = pfnGetAllServerNames( pEnv, pszServerList+9, lAnzServer - 1 );
   pfnFreeEnv( pEnv );
   if ( nRet != 0 )
   {
      SysFreeMemory( lHMem );
      return( nRet );
   }

   // Set all Servernames into the WorkLOD
   for ( lLv = 0; lLv < lAnzServer; lLv++ )
   {
      CreateEntity( vAktView, "TZSIIMWK", zPOS_AFTER );
      SetAttributeFromString( vAktView, "TZSIIMWK",
                              "wkServerName", pszServerList );
      pszServerList += 9;
   }

   SysFreeMemory( lHMem );
   return( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: GetAllCatalogNames
//
zOPER_EXPORT zSHORT  OPERATION
GetAllCatalogNames( zVIEW vSubtask, zVIEW vvAktView )
{
   zCHAR szLocalServer[ MAX_COMPUTERNAME_LENGTH + 1 ];
   zLONG lLocalLength;
   zCHAR szActualServer[ 9 ];

   // First we have to see, if the catalogs should be loaded locally or from
   // the server
   lLocalLength = MAX_COMPUTERNAME_LENGTH + 1;
   memset(szLocalServer, 0, MAX_COMPUTERNAME_LENGTH + 1);
   if (GetComputerName( szLocalServer, &lLocalLength ) == 0 )
      return( -3);

   GetStringFromAttribute( szActualServer, zsizeof( szActualServer ), vvAktView, "TZSIIMWK", "wkServerName");
   if ( zstrcmp( szActualServer, szLocalServer ) == 0 )
      return( GetLocalCatalogs( vSubtask, vvAktView ) );
   else
      return( GetCatalogs( vSubtask, vvAktView ) );
} // GetAllCatalogNames


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: FreeHZVAFFZ
//
zOPER_EXPORT zSHORT OPERATION
FreeHZVAFFZ( zVIEW vSubtask )
{
   if (g_hLib != NULL)
      SysFreeLibrary( vSubtask, g_hLib );

   return( 0 );
} // FreeHZVAFFZ

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: BuildWSK
//
zOPER_EXPORT zSHORT OPERATION
BuildWSK( zVIEW vSubtask, zVIEW vAktView, zPCHAR szServerName,
          zPCHAR szCatFile, zPCHAR szCatalogName, zPCHAR pszFilename )
{
   void  *ptCatalog = NULL;
   zSHORT nRC;

   zCHAR szLocalServer[ MAX_COMPUTERNAME_LENGTH + 1 ];
   zLONG lLocalLength;
   zCHAR szActualServer[ 9 ];

   // First we have to see, if the catalogs should be loaded locally or from
   // the server
   lLocalLength = MAX_COMPUTERNAME_LENGTH + 1;
   memset( szLocalServer, 0, MAX_COMPUTERNAME_LENGTH + 1 );
   if ( GetComputerName( szLocalServer, &lLocalLength ) == 0 )
      return( -3 );

   GetStringFromAttribute( szActualServer, zsizeof( szActualServer ), vAktView, "TZSIIMWK", "wkServerName");
   if ( zstrcmp( szActualServer, szLocalServer ) == 0 )
   {
      // First we have to read the actual catalogdata
      nRC = pfnReadCatalog( szCatFile, szCatalogName, &ptCatalog );
      if ( nRC )
      {
         pfnFreeCatalogStructs( ptCatalog );
         return( nRC );
      }
   }
   else
   {
       nRC = pfnReadServerCatalog (szActualServer, szCatFile, szCatalogName,
                                   &ptCatalog, "", "");
       if ( nRC )
       {
           // Speicher freigeben
           pfnFreeCatalogStructs (ptCatalog);
           return( nRC );
       }
   }

   // Next Step: Write the WSK-File
   nRC = pfnWriteWSK (ptCatalog, vSubtask, pszFilename);

   pfnFreeCatalogStructs (ptCatalog);
   return( nRC );
} // BuildWSK


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: LoadSironDLLs
//
zSHORT /* GLOBAL */  OPERATION
LoadSironDLLs( zVIEW vSubtask )
{
   return( fnLoadHzvaffz (vSubtask) );
} // LoadSironDLLs


////////////////////////////////////////////////////////////////////////////////////////////////////////
// Local Operations
////////////////////////////////////////////////////////////////////////////////////////////////////////
// Load HZVAFFZ.DLL
static zSHORT fnLoadHzvaffz ( zVIEW vSubtask )
{
  // Load the DLL (must be in the path)

  g_hLib = SysLoadLibraryWithErrFlag( vSubtask, "HZVAFFZ", 0 );
  if ( g_hLib == 0 )
    return( 500 );

  pfnOpenEnv = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaOpenEnv");
  if (pfnOpenEnv == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 501 );
  }

  pfnFreeEnv = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaFreeEnv");
  if (pfnFreeEnv == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 502 );
  }

  pfnGetAllServerNames = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetAllServerNames" );
  if (pfnGetAllServerNames == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 503 );
  }

  pfnGetAnzahlServerNames = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetAnzahlServerNames" );
  if (pfnGetAnzahlServerNames == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 504 );
  }

  pfnGetAnzahlCatalogFiles = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetAnzahlCatalogFiles" );
  if (pfnGetAnzahlCatalogFiles == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 505 );
  }

  pfnGetAllCatalogFiles = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetAllCatalogFiles" );
  if (pfnGetAllCatalogFiles == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 506 );
  }

  pfnGetLocalCatalogs = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetLocalCatalogs" );
  if (pfnGetLocalCatalogs == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 513 );
  }

  pfnReadCatalog = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaReadCatalog" );
  if (pfnReadCatalog == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 514 );
  }

  pfnFreeCatalogStructs = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaFreeCatalogStructs" );
  if (pfnFreeCatalogStructs == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 515 );
  }

  pfnWriteWSK = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaWriteWSK" );
  if (pfnWriteWSK == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 516 );
  }

  pfnGetPathname = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetPathname" );
  if (pfnGetPathname == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 517 );
  }

  pfnGetServerCatalogs = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaGetServerCatalogs" );
  if (pfnGetServerCatalogs == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 520 );
  }

  pfnReadServerCatalog = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaReadServerCatalog" );
  if (pfnReadServerCatalog == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 521 );
  }

  pfnFreeListMemory = (DLL_ENTRY_S) SysGetProc( g_hLib, "hzvaFreeListMemory" );
  if (pfnFreeListMemory == NULL)
  {
      SysFreeLibrary( vSubtask, g_hLib );
      return( 522 );
  }

  return( 0 );
}

// Get all local Catalogs
static zSHORT GetLocalCatalogs( zVIEW vSubtask, zVIEW vAktView )
{
    zSHORT nRet;

    // If the list of catalogfiles still exists, return
    nRet = CheckExistenceOfEntity( vAktView, "CatFiles");
    if (nRet == zCURSOR_SET || nRet == zCALL_ERROR)
       return( nRet );

    nRet = GetAnzahlCatalogFiles ( vSubtask, vAktView );
    if (nRet != 0 )
       return( nRet );

    return( GetCatalogs (vSubtask, vAktView));
}

static zSHORT GetAnzahlCatalogFiles( zVIEW vSubtask, zVIEW vAktView)
{
    zSHORT nRet;
    zLONG  lHMem;
    zLONG  lAnzCatalogFiles;
    zLONG  lLv;
    zCHAR  *pszCatalogFileList = NULL;
    void   *pEnv = NULL;

    // First we have to get the number of catalogfiles from the environment file
    // SIRENV
    if (g_hLib == NULL)
    {
       nRet = fnLoadHzvaffz (vSubtask);
       if ( nRet != 0 )
         return( nRet );
    }

    // Open the environment
    nRet = pfnOpenEnv( &pEnv );
    if ( nRet != 0 )
       return( nRet );

    nRet = pfnGetAnzahlCatalogFiles (pEnv, &lAnzCatalogFiles);
    if ( nRet != 0 )
    {
       pfnFreeEnv( pEnv );
       return( nRet );
    }

    // Get Memory for the whole cataloglist
    // max. length of a catalogname shoud be 9 characters
    lHMem = SysAllocMemory( &pszCatalogFileList, lAnzCatalogFiles * 9, 0, zCOREMEM_ALLOC, 0 );
    if ( lHMem == 0 )
    {
       pfnFreeEnv( pEnv );
       return( 552);
    }

    memset(pszCatalogFileList, 0, lAnzCatalogFiles * 9);
    // Get all Catalogfilenames from the Environmentfile
    nRet = pfnGetAllCatalogFiles (pEnv, pszCatalogFileList, lAnzCatalogFiles);
    pfnFreeEnv( pEnv );
    if ( nRet != 0 )
    {
       SysFreeMemory (lHMem);
       return( nRet );
    }

    // Set all Catalogfilenames into the WorkLOD
    for ( lLv = 0; lLv < lAnzCatalogFiles; lLv++ )
    {
        CreateEntity( vAktView, "CatFiles", zPOS_AFTER );
        SetAttributeFromString(vAktView, "CatFiles", "wkCatFiles", pszCatalogFileList);
        SetAttributeFromString( vAktView, "CatFiles", "wkEinschraenkung", "");

        pszCatalogFileList += 9;
    }

    // Free all Memory
    SysFreeMemory( lHMem );
    return( 0 );
}

static zSHORT GetCatalogs( zVIEW vSubtask, zVIEW vAktView )
{
    zSHORT nRet;
    zCHAR szServerName[9];
    zCHAR szCatalogFileName[9];
    zCHAR szEinschraenkung[9];
    zCHAR szCatalogName[9];
    zCHAR *pszCatalogList = NULL;
    long lAnzahlCatalogs;
    long lLv;
    zCHAR szLocalServer[MAX_COMPUTERNAME_LENGTH + 1];
    zLONG lLocalLength;

    // First we have to see, if the catalogs should be loaded locally or from
    // the server
    lLocalLength = MAX_COMPUTERNAME_LENGTH + 1;
    memset(szLocalServer, 0, MAX_COMPUTERNAME_LENGTH + 1);
    if (GetComputerName( szLocalServer, &lLocalLength ) == 0 )
       return( -3);

    // Get the actual Servername and CatalogFileName
    memset( szServerName, 0, 9 );
    GetStringFromAttribute( szServerName, zsizeof( szServerName ), vAktView, "TZSIIMWK", "wkServerName" );

    if ( zstrcmp( szServerName, szLocalServer ) != 0 )
    {
       // For servers we have to see if the list of catalogs is
       // already loaded. If not we have to Create an entity for
       // the catalogfile ADRTAFEL
       // If the list of catalogfiles still exists, return
       nRet = CheckExistenceOfEntity( vAktView, "CatFiles" );
       if ( nRet == zCURSOR_SET || nRet == zCALL_ERROR )
          return( nRet );

       // First we have to Create the Catalogfilename in the WorkLOD
       CreateEntity(vAktView, "CatFiles", zPOS_AFTER);
       SetAttributeFromString( vAktView, "CatFiles",
                               "wkCatFiles", "ADRTAFEL");
       SetAttributeFromString( vAktView, "CatFiles",
                               "wkEinschraenkung", "");
    }

    // If the siron modules are not loaded, load them now
    if ( g_hLib == NULL )
    {
       nRet = fnLoadHzvaffz( vSubtask );
       if ( nRet != 0 )
         return( nRet );
    }

    // Position onto the first catalogfile
    nRet = SetCursorFirstEntity(vAktView, "CatFiles", "");
    while ( nRet == zCURSOR_SET )
    {
       // Set Catalogfilename in structure for getting the cataloglist
       memset(szCatalogFileName, 0, 9);
       GetStringFromAttribute( szCatalogFileName, zsizeof( szCatalogFileName ), vAktView,
                               "CatFiles", "wkCatFiles");
       GetStringFromAttribute( szEinschraenkung, zsizeof( szEinschraenkung ), vAktView,
                               "CatFiles", "wkCatFiles");

       lAnzahlCatalogs = 0;
       if ( zstrcmp( szServerName, szLocalServer ) == 0 )
       {
          // Read all Catalogs from local PC
          nRet = pfnGetLocalCatalogs( szCatalogFileName, szEinschraenkung,
                                      &lAnzahlCatalogs, &pszCatalogList, "", "" );
       }
       else
       {
          // Read all Catalogs from the selected server
          nRet = pfnGetServerCatalogs( szServerName, szCatalogFileName,
                                       szEinschraenkung, &lAnzahlCatalogs,
                                       &pszCatalogList, "", "" );
       }

       if ( nRet != 0 )
       {
          pfnFreeListMemory( &pszCatalogList );
          return( nRet );   // Umsetzen auf 590
       }

       for (lLv = 0; lLv < lAnzahlCatalogs; lLv++)
       {
           memset( szCatalogName, 0, 9 );
           memcpy( szCatalogName, pszCatalogList+(lLv*9), 8);
           CreateEntity (vAktView, "Catalogs", zPOS_AFTER);
           SetAttributeFromString (vAktView, "Catalogs", "wkCatalogName", szCatalogName);
       }

       // Free Memory
       pfnFreeListMemory( &pszCatalogList );

       nRet = SetCursorNextEntity( vAktView, "CatFiles", "" );
    }

    return( 0 );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SetWSKInLod
//
zOPER_EXPORT zSHORT OPERATION
SetWSKInLod( zVIEW vSubtask, zVIEW vAktView, zPCHAR szFilename )
{
  zCHAR *pszFilename;
  zCHAR *pszEndname;

  // Get the Filename to set it into the WorkLod
  pszFilename = zstrrchr( szFilename, '\\' ) + 1;
  pszEndname  = zstrchr( pszFilename, '.' );
  *pszEndname = 0;

  CreateEntity( vAktView, "KatalogFile", zPOS_FIRST );
  SetAttributeFromString( vAktView, "KatalogFile", "Name", pszFilename);

  // To import the file we need the extension
  *pszEndname = '.';

  return( 0 );
} // SetWSKInLod

////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SearchSpecialCharacter
//
//    This Operation searches for special characters in a string excluding '_'
//    If a special character is found, it returns the position in the string
//    otherwise it returns -1
//    The position in a string could never be -1, therefore we return it, if
//    there is no special character found.
//    Parameter sSpecChar returns the special character which was found.
zOPER_EXPORT zSHORT OPERATION
SearchSpecialCharacter( zPCHAR szFieldName,
                        zPCHAR pchSpecChar )
{
   int iLv;

   for ( iLv = 0; iLv < (int) strlen( szFieldName ); iLv++ )
   {
       if ( isalnum( szFieldName[ iLv ] ) == 0 && szFieldName[ iLv ] != '_')
       {
          *pchSpecChar = szFieldName[ iLv ];
          return( iLv );
       }
   }

   return( -1 );
} // SearchSpecialCharacter

/////////////////////////////////////////////////////////////////////////////
//
//    OPERATION: SironCreateDataModel
//
//
/////////////////////////////////////////////////////////////////////////////
zOPER_EXPORT zSHORT /*GLOBAL */  OPERATION
SironCreateDataModel( zVIEW vSubtask,
                      zPVIEW pvERD )
{
   zVIEW  vERD_Window;
   zVIEW  vTaskLPLR;

   GetViewByName( &vTaskLPLR, "TaskLPLR", vSubtask, zLEVEL_TASK );

   ActivateEmptyMetaOI( vSubtask, pvERD, zSOURCE_ERD_META,
                        zSINGLE | zLEVEL_APPLICATION );

   // Build Root Instance
   CreateMetaEntity( vSubtask, *pvERD, "EntpER_Model", zPOS_AFTER );
   SetAttributeFromAttribute( *pvERD, "EntpER_Model", "Name", vTaskLPLR, "LPLR", "Name" );
   SetAttributeFromString( *pvERD, "EntpER_Model", "Desc", "Siron Catalogs DataModel" );
   SetNameForView( *pvERD, "TZEREMDO", vSubtask, zLEVEL_TASK );

   // Data Model exists, enable ERD Control
   GetParentWindow( &vERD_Window, vSubtask );
   SetCtrlState( vERD_Window, "ERD", zCONTROL_STATUS_ENABLED, TRUE );

   // Set window Title with check out state
   SetTitleWithCheckOutState( vERD_Window, "Data Model", "TZEREMDO", *pvERD,
                              "EntpER_Model", zSOURCE_ERD_META );
   return( 0 );
} // SironCreateDataModel
