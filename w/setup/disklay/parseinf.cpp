/////////////////////////////////////////////////////////////////////////////
//  PARSEINF.CPP - written by Doug Sorensen    - create date 03/01/89
//                 (C)  Quinsoft Inc., 1989    - last update 03/01/89
//
//  This program reads and writes a
//
//  Input Parms:
//     1. filespec.inf - filespec of the .inf file (including drive and dir)
//     2. Directory for the compressed files
//     3. /B  - build bat files for each disk
//     4. /R  - report size for each disk
//
//
//  Compile using cl -F 2000 parseinf.cpp
//
/////////////////////////////////////////////////////////////////////////////
#define PGM_VERSION "1.1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys\stat.h>
#include <ctype.h>
#include <dos.h>
#include <ostream.h>

typedef enum BOOL { FALSE, TRUE };

#define  READFILE      "r"    /*  read only                              */
#define  WRITEFILE     "w"    /*  write only (overwrite existing file)   */
#define  BUFSIZE      1024
#define  DIRSIZE       256

/* a simpleton single-link list for files */
class CSListFile
{
public:
   CSListFile( const char *pchName,
               long lFileSize );
   ~CSListFile( void );

   long m_lFileSize;
   char *m_pchName;
   short m_nLth;
   CSListFile *m_pNext;
};


CSListFile::CSListFile( const char *pchName,
                        long lFileSize )
{
   m_lFileSize = lFileSize;
   m_nLth = strlen( pchName );
   m_pchName = new char[ m_nLth + 1 ];
   strcpy( m_pchName, pchName );
   m_pNext = 0;
}

CSListFile::~CSListFile( void )
{
   delete [] m_pchName;
}


/* a simpleton single-link list for drives */
class CSListDrive
{
public:
   CSListDrive( const char *pchName,
                unsigned short usDriveNbr );
   ~CSListDrive( void );

   unsigned short m_usDriveNbr;
   unsigned short m_usCnt;
   char *m_pchName;
   short m_nLth;
   CSListDrive *m_pNext;

   CSListFile *m_pslFilesHead;
   CSListFile *m_pslFilesCurr;
};


CSListDrive::CSListDrive( const char *pchName,
                          unsigned short usDriveNbr )
{
   m_usCnt = 0;
   m_usDriveNbr = usDriveNbr;
   m_nLth = strlen( pchName );
   m_pchName = new char[ m_nLth + 1 ];
   strcpy( m_pchName, pchName );
   m_pNext = 0;

   m_pslFilesHead = 0;
   m_pslFilesCurr = 0;
}

CSListDrive::~CSListDrive( void )
{
   delete [] m_pchName;
   for ( m_pslFilesCurr = m_pslFilesHead;
         m_pslFilesCurr;
         m_pslFilesCurr = m_pslFilesCurr->m_pNext )
   {
      delete( m_pslFilesCurr );
   }
}


void
fnProcessINF( const char *cpcFileName );
void
fnPI_CmdHelp( );


struct _stat statBuffer;
long  lTotalEntries = 0;
long  lTotalSize = 0;
BOOL  bBatFiles = FALSE;
BOOL  bReport = FALSE;
char  szCompressDir[ 256 ] = "";
short nCompressLth = 0;

short
main( short argc,
      char *argv[ ] )
{
   short nInf = 0;
   short nDir = 0;
   short k;

   for ( k = 1; k < argc; k++ )
   {
      if ( argv[ k ][ 0 ] == '/' || argv[ k ][ 0 ] == '-' )
      {
         if ( argv[ k ][ 1 ] == 'B' || argv[ k ][ 1 ] == 'b' )
         {
            bBatFiles = TRUE;
         }
         else
         if ( argv[ k ][ 1 ] == 'R' || argv[ k ][ 1 ] == 'r' )
         {
            bReport = TRUE;
         }
         else
         {
            cout << "Invalid switch - " << argv[ k ]
                 << "\n****** Processing Aborted ******"
                 << endl;
            return( 1 );
         }
      }
      else
      {
         if ( nInf == 0 )
         {
            nInf = k;
         }
         else
         if ( nDir == 0 )
         {
            nDir = k;
         }
         else
         {
            cout << "Too many arguments - " << argv[ k ]
                 << "\n****** Processing Aborted ******"
                 << endl;
            nInf = 0;     // force help
            break;
         }
      }
   }

   if ( nInf == 0 )
   {
      fnPI_CmdHelp( );
      return( 1 );
   }

   if ( nDir )
   {
      strcpy( szCompressDir, argv[ nDir ] );
      nCompressLth = strlen( szCompressDir );
      if ( szCompressDir[ nCompressLth - 1 ] != '\\' )
      {
         szCompressDir[ nCompressLth++ ] = '\\';
         szCompressDir[ nCompressLth ] = 0;
      }

//    cerr << "Directory = " << szCompressDir << endl;
   }


   fnProcessINF( argv[ nInf ] );

   if ( bReport )
   {
      cout << "\n\tTotal files = " << lTotalEntries
           << "\n\tTotal size  = " << lTotalSize << endl;
   }

   return( 0 );
}

void
fnProcessINF( const char *cpcFileName )
{
   FILE    *fpRead;
   FILE    *fpWrite = 0;
   CSListDrive *pslDriveHead = 0;
   CSListDrive *pslDriveCurr;
   CSListFile  *pslFileCurr;
   char   szBuffer[ BUFSIZE ];
   char   szSection[ 128 ];
   char   szFileName[ 14 ];
   char   *pch;
   char   *pch2;
   BOOL   bSMD = FALSE;  // [Source Media Descriptions]
   BOOL   bDFS = FALSE;  // [Default File Settings]
   long   lSectionEntries;
   long   lSectionSize;
   long   lFileSize;
   long   lSectionCompressSize;
   long   lCompressSize;
   short  nStatus = 0;
   unsigned short usMaxDrive = 0;
   unsigned short usDriveNbr;
   short  k;

   if ( (fpRead = fopen( cpcFileName, READFILE )) == 0 )
   {
      cerr << "Error opening file - " << cpcFileName
           << "\n*** Processing aborted ***" << endl;
      exit( 1 );
   }

   while ( fgets( szBuffer, sizeof( szBuffer ), fpRead ) )
   {
      switch( nStatus )
      {
         case 0:
            if ( strstr( szBuffer, "[Source Media Descriptions]" ) )
            {
               nStatus = 1;
            }

            break;

         case 1:
            if ( strstr( szBuffer, "[Default File Settings]" ) )
            {
               nStatus = 2;
            }
            else
            {
               /* process [Source Media Descriptions] in the form:
                  "1", "ZEIDON_1", "setup.exe", "." */
               if ( (pch = strchr( szBuffer, '"' )) != 0 )
               {
                  usDriveNbr = atoi( pch + 1 );
                  if ( usDriveNbr > usMaxDrive )
                     usMaxDrive = usDriveNbr;

                  if ( (pch = strchr( pch, ',' )) != 0 &&
                       (pch = strchr( pch, '"' )) != 0 )
                  {
                     pch++;
                     pch2 = strchr( pch, '"' );
                     if ( pch2 )
                     {
                        *pch2 = 0;
                        if ( pslDriveHead )
                        {
                           pslDriveCurr->m_pNext =
                                    new CSListDrive( pch, usDriveNbr );
                           pslDriveCurr = pslDriveCurr->m_pNext;
                        }
                        else
                        {
                           pslDriveHead = new CSListDrive( pch, usDriveNbr );
                           pslDriveCurr = pslDriveHead;
                        }
                     }
                  }
               }
            }

            break;

         case 2:
            if ( szBuffer[ 0 ] == '[' )
            {
               nStatus = 3;
               pch = strchr( szBuffer, ']' );
               if ( pch )
               {
                  *(pch + 1) = 0;
                  strcpy( szSection, szBuffer );
                  lSectionEntries = 0;
                  lSectionSize = 0;
               }
            }
   //       else
   //          process source default file settings


            break;

         case 3:
         {
            if ( szBuffer[ 0 ] == '[' )
            {
               pch = strchr( szBuffer, ']' );
               if ( pch )
               {
                  if ( bReport && lSectionEntries )
                     cout << "REM Processing section " << szSection <<
                             "  - count = " << lSectionEntries <<
                             "  - size = " << lSectionSize << '\n';

                  *(pch + 1) = 0;
                  strcpy( szSection, szBuffer );
                  lSectionEntries = 0;
                  lSectionSize = 0;
               }
            }
            else
            if ( (pch = strchr( szBuffer, '=' )) != 0 )
            {
               /* process files in the form:
                  "C_Binary" = 5, ctl3d.dll,,,, 1993-09-22,,,,,,,,,, 19568,,,, 1.1.3.0, */

               pch++;  // point to drive number
               usDriveNbr = atoi( pch );
               if ( (pch = strchr( pch, ',' )) != 0 )
               {
                  pch += 2;  // point to file name
                  pch2 = pch;
                  for ( k = 0; k < 14; k++ )
                  {
                     pch2 = strchr( pch2, ',' );
                     if ( pch2 )
                        *pch2++ = 0;
                  }

                  if ( pch2 )
                  {
                     for ( pslDriveCurr = pslDriveHead;
                           pslDriveCurr;
                           pslDriveCurr = pslDriveCurr->m_pNext )
                     {
                        if ( pslDriveCurr->m_usDriveNbr == usDriveNbr )
                           break;
                     }

                     if ( pslDriveCurr )
                     {
                        pslFileCurr = new CSListFile( pch, 0 );
                        pch = pch2 + 1;
                        pch2 = strchr( pch, ',' );
                        *pch2 = 0;
                        pslFileCurr->m_lFileSize = atol( pch );

                        lTotalEntries++;
                        lSectionEntries++;
                        lTotalSize += pslFileCurr->m_lFileSize;
                        lSectionSize += pslFileCurr->m_lFileSize;

                        if ( pslDriveCurr->m_pslFilesHead )
                        {
                           pslDriveCurr->m_pslFilesCurr->m_pNext =
                                                                 pslFileCurr;
                           pslDriveCurr->m_pslFilesCurr =
                                       pslDriveCurr->m_pslFilesCurr->m_pNext;
                        }
                        else
                        {
                           pslDriveCurr->m_pslFilesHead = pslFileCurr;
                           pslDriveCurr->m_pslFilesCurr = pslFileCurr;
                        }
                     }
                  }
               }
            }
         }
      }
   }

   if ( bReport && lSectionEntries )
      cout << "REM Processing section " << szSection
           << "  - count = " << lSectionEntries
           << "  - size = " << lSectionSize << '\n';

   for ( pslDriveCurr = pslDriveHead;
         pslDriveCurr;
         pslDriveCurr = pslDriveCurr->m_pNext )
   {
      if ( bReport )
         cout << "Drive " << pslDriveCurr->m_usDriveNbr << " "
              << pslDriveCurr->m_pchName << '\n';

      if ( bBatFiles )
      {
         char szBatchFile[ 14 ];

         if ( fpWrite )
            fclose( fpWrite );

         strncpy( szBatchFile, pslDriveCurr->m_pchName, 8 );
         szBatchFile[ 8 ] = 0;
         strcat( szBatchFile, ".bat" );
         if ( (fpWrite = fopen( szBatchFile, WRITEFILE )) == 0 )
         {
            cerr << "Error opening file - " << szBatchFile
                 << "\n*** Processing aborted ***" << endl;
            exit( 1 );
         }
         else
            cout << "Writing bat file - " << szBatchFile << '\n';
      }

      lSectionEntries = 0;
      lSectionSize = 0;
      lSectionCompressSize = 0;

      for ( pslFileCurr = pslDriveCurr->m_pslFilesHead;
            pslFileCurr;
            pslFileCurr = pslFileCurr->m_pNext )
      {
         strcpy( szFileName, pslFileCurr->m_pchName );
         pch = strchr( szFileName, '.' );
         if ( pch )
         {
            pch++;
            if ( *pch && *(pch + 1) )
            {
               *(pch + 2) = '_';
               *(pch + 3) = 0;
            }
            else
            if ( *pch )
            {
               *(pch + 1) = '_';
               *(pch + 2) = 0;
            }
            else
               strcat( pch, "_" );
         }
         else
            strcat( szFileName, "._" );

         strcpy( szCompressDir + nCompressLth, szFileName );
         if ( fpWrite )
         {
            strcpy( szBuffer, "XCOPY /v " );
            strcpy( szBuffer + 9, szCompressDir );
            strcat( szBuffer, "   A:\\\n" );
            fputs( szBuffer, fpWrite );
         }

         if ( _stat( szCompressDir, &statBuffer ) == 0 &&
              (statBuffer.st_mode & _S_IFREG) )
         {
            lCompressSize = statBuffer.st_size;
         }
         else
            lCompressSize = 0;

         if ( bReport )
            cout << "File " << pslFileCurr->m_pchName
                 << "  " << pslFileCurr->m_lFileSize
                 << "  compress size " << lCompressSize << '\n';

         lSectionEntries++;
         lSectionSize += pslFileCurr->m_lFileSize;
         lSectionCompressSize += lCompressSize;
      }

      if ( bReport )
         cout << "REM Drive number totals "
              << "  - count = " << lSectionEntries
              << "  - size = " << lSectionSize
              << "  - compress size = " << lSectionCompressSize << '\n';
   }
}

/***********************************************************
*
*  Help for PI
*
************************************************************/
void
fnPI_CmdHelp( )
{
   cout << "\nALTERINF " << PGM_VERSION << " - Invalid argument list\n";
   cout << "syntax:   ALTERINF  filespec.inf compress_dir  [/B /R /?]\n\n"
        << "Two positional arguments may be specified\n"
        << " File spec of the .inf file (including drive and directory)\n"
        << " (Optional) directory for the compressed files\n"
        << "Optional switches\n"
        << " /B - build bat files for each disk\n"
        << " /R - report size for each disk\n"
        << " /? - display this information\n";

   cout << "\ne.g.  parseinf zeidon.inf c:\test -b -r\n";

   cout.flush( );
}
