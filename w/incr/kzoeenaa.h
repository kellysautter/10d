#ifndef KZOEENAA_DEF
#define KZOEENAA_DEF

/*
CHANGE LOG
2002.01.10  HH
  Added zxtob.

2000.10.20  SR  Z2000
  Add define zMAX_FILESPEC_LTH and zMAX_EXTENSION_LTH.
  Change size of zMAX_FILENAME_LTH, zSHORT_FILENAME_LTH and zBASE_FILENAME_LTH for
  third case.

2000.03.08  HH
   remove "__WIN32__ warning"

1999.11.29  SR  Z2000  Compile
   Remove compile directive __WIN32__ in makefile zdecimal.mak
   because it is defined twice. Remove "TB_NT" in this file
   is not possible while several other libraries are based on it.

1999.11.29  SR  Z2000  Comments
   Changed order of CHANGE LOG's so that it corresponds to other modules

1998.10.14  DGC
   Removed zOS_NAME.

1998.08.28  DGC
   Changed zxtol() to map to strtoul() instead of strtol().

09.06.1998  RG
   Several changes from DKS

11.05.1998  RG
   Change from 16.04.1998 is needed for Siron compilation

20.04.1998  RG
   Removed the last change because of link-problems in win3.1
   The above change must only be done for release 9J

16.04.1998  RG
   Added definition of __WIN32__ if TB_NT is set

1998.01.28  DGC
   Added zBASE_FILENAME_LTH for all OS's.

1998.01.22  DGC
   Changed back to 'double'.
   Tried to clean up a little.

1998.01.14  DGC
   Changed definition of zDECIMAL from 'double' to 'long double'.

   Changed zatof to convert to a long double (zDECIMAL).

07.10.1997  HH
   Added definition zdecimalpoint

??.??.1997 ??
   lots of changes

97/05/12    DGC
   Added zBYTE type.

97/05/01    DGC
   Added Win32 code for NT.

*/

//
// Object services environment specific header for windows 3.x
//

#define LOADDS

// Uncomment following to use 'long double' instead of 'double' for Win 3.x
#define zLONG_DOUBLE

// SR: remove second definition in makefile zdecimal.mak: " /D "__WIN32__" ".
//     Cannot remove "TB_NT" in this file. It is necessary when building libraries
#ifndef __WIN32__
#define __WIN32__
#endif

#include <string.h>
#include <stdlib.h>
#include <tchar.h>

// win NT
#define zPASCAL            __stdcall

#ifdef _MSC_VER
    #define zOPER_EXPORT       __declspec (dllexport)  // AFX_API_EXPORT
    #define OPERATION          __stdcall
#else
    #define zOPER_EXPORT
    #define OPERATION          __declspec (dllexport) __stdcall
#endif

#define POPERATION         __stdcall  *
#define LOCALOPER          zPASCAL

#define VARYOPER        __declspec (dllexport) __stdcall // Variable arg list.

typedef struct DecimalStruct
{
    char data[ 40 ];

} zDECIMAL;

typedef struct DecimalStruct zDOUBLE;

#define zEXPORT

#define zINLINE            _inline

typedef int (POPERATION zPOPER)();

typedef unsigned char       zBOOL;
typedef unsigned char       zBYTE;
typedef char                zCHAR;
typedef long                zLONG;
typedef short               zSHORT;
typedef void                zVOID;

typedef const unsigned char zCBOOL;
typedef const unsigned char zCBYTE;
typedef const char          zCCHAR;
typedef const long          zCLONG;
typedef const short         zCSHORT;

typedef unsigned char       zUCHAR;
typedef unsigned long       zULONG;
typedef unsigned short      zUSHORT;

typedef const unsigned char  zCUCHAR;
typedef const unsigned long  zCULONG;
typedef const unsigned short zCUSHORT;

typedef zBOOL *             zPBOOL;
typedef zBOOL *             zPBYTE;
typedef zCHAR *             zPCHAR;
typedef zPCHAR *            zPPCHAR;
typedef const zPCHAR *      zCPPCHAR;
typedef const zCHAR *       zCPCHAR;
typedef zDOUBLE *           zPDOUBLE;
typedef zLONG *             zPLONG;
typedef zSHORT *            zPSHORT;
typedef zVOID *             zPVOID;
typedef zPVOID *            zPPVOID;
typedef const zVOID *       zCPVOID;
typedef zUCHAR *            zPUCHAR;
typedef zULONG *            zPULONG;
typedef zUSHORT *           zPUSHORT;

typedef zPDOUBLE            zPDECIMAL;
typedef zPDOUBLE *          zPPDECIMAL;
typedef zLONG               zINTEGER;
typedef zPLONG              zPINTEGER;

#define zdecimalpoint( s )                   /**/

//#error __WIN32__ is defined
// Win 32 defs.
#define zisalnum                             isalnum
#define zaccess                              _access
#define zisascii                             __isascii
#define zisdigit                             isdigit
#define zatol                                atol
#define zstrtol                              strtol
#define zxtol( sz )                          strtoul( (sz), 0, 16 )
#define zltoa( l, sz, size )                 _ltoa_s( (l), (sz), (size), 10 )
#define zltox( l, sz, size )                 _ltoa_s( (l), (sz), (size), 16 )
#define zatob                                zatol
#define zxtob                                zxtol
#define zbtoa                                zltoa
#define zsprintf                             sprintf
#define ztoupper                             toupper
#define zmemcmp                              memcmp
#define zmemicmp                             memicmp
#define zmemcpy                              memcpy
#define zmemset                              memset
#define zstrcat                              strcat
#define zstrchr                              (zPCHAR) strchr
#define zstrcmp( sz1, sz2 )                  ((*((char *) (sz1)) == *((char *) (sz2))) ? strcmp( (char *) (sz1), (char *) (sz2) ) : (*((char *) (sz1)) - *((char *) (sz2))) > 0 ? 1 : -1)
// #define zstrcmp                              strcmp
#define zstrcmpi                             _stricmp
#define zstrcpy                              strcpy
#define zstricmp                             stricmp
#define zstrnicmp                            _strnicmp
#define zstrlen                              strlen
#define zstrncat                             strncat
#define zstrncmp                             strncmp
#define zstrncmpi                            _strnicmp
#define zstrncpy                             strncpy
#define zstrrchr                             (zPCHAR) strrchr
#define zstrrev                              _strrev
#define zstrstr                              strstr
#define zstrtok                              strtok
#define zfullpath                            _fullpath
#define zstrupper                            _strupr
#define zstrlower                            _strlwr
#define zisspace                             isspace

// For Win31/NT/OS2, SysGetTickCount returns milliseconds
#define zTICKS_PER_SECOND 1000

#define zMAKEPOINT  MAKEPOINTS

// Define OS and filename declarations
#define zGETPTR( a )     SysGetPointerFromHandle( (a) )

// Following define the space (in chars) needed to hold file names for the OS.
// This should NO LONGER include the null-terminator.
#define zMAX_FILESPEC_LTH           512   // Max filespecification lth for OS
#define zMAX_FILENAME_LTH           256   // Max file lth for OS
#define zSHORT_FILENAME_LTH         256   // Max file lth w/o dir
#define zBASE_FILENAME_LTH          128   // Max file lth w/o dir or ext.
#define zMAX_EXTENSION_LTH          3     // Max length of file extension

#if 0  // moved to kzoeufaa.c   dks 2015.12.16
#define zstrcpyp( pTgt, size, pSrc ) ((zPCHAR) strcpy_s( pTgt, size, pSrc ) + zstrlen( pTgt ))
#define zstrcatp( pTgt, size, pSrc ) ((zPCHAR) strcat_s( pTgt, size, pSrc ) + zstrlen( pTgt ))
#else
zPCHAR OPERATION
zstrcpyp( zPCHAR pchTgt, zLONG lMaxLth, zCPCHAR cpcSrc );
zPCHAR OPERATION
zstrcatp( zPCHAR pchTgt, zLONG lMaxLth, zCPCHAR cpcSrc );
#endif
#define zstrcpyl( pTgt, size, pSrc ) ((zSHORT) zstrlen( strcpy_s( pTgt, size, pSrc ) ))
#define zstrcatl( pTgt, size, pSrc ) ((zSHORT) zstrlen( strcat_s( pTgt, size, pSrc ) ))

zSHORT OPERATION
SysDescribeZeidonPageTable( zPCHAR pszResults, zLONG lMaxLth );

#endif // #ifndef KZOEENAA_DEF
