/////////////////////////////////////////////////////////////////////////////
//  ALTERINF.CPP - written by Doug Sorensen    - create date 03/01/89
//                 (C)  Quinsoft Inc., 1989    - last update 03/01/89
//
//  This program reads and writes a .inf file to
//
//  Input Parms:
//     1. filespec.inf - filespec of the .inf file (including drive and dir)
//     2. change.dat   - file containing change records
//
//  Outputs:
//     1. rewritten filespec.inf
//
//
//  Compile using cl -F 2000 alterinf.cpp
//
/////////////////////////////////////////////////////////////////////////////
#define PGM_VERSION "1.1"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>
#include <ostream.h>

typedef enum BOOL { FALSE, TRUE };

#define  READFILE      "r"    /*  read only                              */
#define  WRITEFILE     "w"    /*  write only (overwrite existing file)   */
#define  BUFSIZE      1024
#define  DIRSIZE       256


/* a simpleton single-link list for names */
class CSList
{
public:
   CSList( char const *pchName );
   ~CSList( void );

   char *m_pchName;
   short m_nLth;
   CSList *m_pNext;
};


CSList::CSList( char const *pchName )
{
   m_nLth = strlen( pchName );
   m_pchName = new char[ m_nLth + 1 ];
   strcpy( m_pchName, pchName );
   m_pNext = 0;
}

CSList::~CSList( void )
{
   delete [] m_pchName;
}


void
fnProcessINF( const char *cpcFileName,
              const char *cpcChangeData );
void
fnAI_CmdHelp( );

short
main( short argc,
      char *argv[ ] )
{
   short nInf = 0;
   short nChg = 0;
   short k;

   for ( k = 1; k < argc; k++ )
   {
      if ( nInf == 0 )
      {
         nInf = k;
      }
      else
      if ( nChg == 0 )
      {
         nChg = k;
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

   if ( nInf == 0 || nChg == 0 )
   {
      fnAI_CmdHelp( );
      return( 1 );
   }

   fnProcessINF( argv[ nInf ], argv[ nChg ] );
   return( 0 );
}

void
fnProcessINF( const char *cpcFileName,
              const char *cpcChangeData )
{
   FILE   *fpRead;
   FILE   *fpChange;
   FILE   *fpWrite;
   CSList *pslChgHead = 0;
   CSList *pslChgCurr = pslChgHead;
   char   szBuffer[ BUFSIZE ];
   char   szBufferTo[ BUFSIZE ];
   char   szFileTemp[ DIRSIZE ];
   char   szFileBack[ DIRSIZE ];
   char   *pch;
   char   *pch2;
   short  nStatus = 0;
   unsigned short usDriveNbr;
   short  nLth;
   short  k;

   if ( (fpRead = fopen( cpcFileName, READFILE )) == 0 )
   {
      cerr << "Error opening file - " << cpcFileName
           << "\n*** Processing aborted ***" << endl;
      exit( 1 );
   }

   if ( (fpChange = fopen( cpcChangeData, READFILE )) == 0 )
   {
      cerr << "Error opening file - " << cpcChangeData
           << "\n*** Processing aborted ***" << endl;
      exit( 1 );
   }

   strcpy( szFileTemp, cpcFileName );
   pch = strrchr( szFileTemp, '\\' );
   if ( pch )
      pch++;
   else
      pch = szFileTemp;

   pch = strchr( pch, '.' );
   if ( pch )
      strcpy( pch + 1, "bak" );
   else
      strcat( szFileTemp, ".bak" );

   strcpy( szFileBack, szFileTemp );
   pch = strrchr( szFileTemp, '.' );
   strcpy( pch, ".tmp" );

   k = 0;
   while ( fgets( szBuffer, sizeof( szBuffer ), fpChange ) )
   {
      k++;
      nLth = strlen( szBuffer );
      while ( nLth && isspace( szBuffer[ nLth - 1 ] ) )
         nLth--;

      if ( nLth == 0 )
      {
         cerr << "Error processing change file in line " << k
              << "\n*** Processing aborted ***" << endl;
         exit( 1 );
      }

      szBuffer[ nLth ] = 0;
      if ( pslChgHead )
      {
         pslChgCurr->m_pNext = new CSList( szBuffer );
         pslChgCurr = pslChgCurr->m_pNext;
      }
      else
      {
         pslChgHead = new CSList( szBuffer );
         pslChgCurr = pslChgHead;
      }
   }

   if ( (fpWrite = fopen( szFileTemp, WRITEFILE )) == 0 )
   {
      cerr << "Error opening temporary file - " << szFileTemp
           << "\n*** Processing aborted ***" << endl;
      exit( 1 );
   }
// else
//    cerr << "Writing Temp file " << szFileTemp << endl;

   k = 1;
   while ( fgets( szBuffer, sizeof( szBuffer ), fpRead ) )
   {
      for ( pslChgCurr = pslChgHead;
            pslChgCurr;
            pslChgCurr = pslChgCurr->m_pNext )
      {
//       cerr << "Checking " << pslChgCurr->m_pchName << endl;

         if ( (pch = strstr( szBuffer, pslChgCurr->m_pchName )) == 0 )
            pslChgCurr = pslChgCurr->m_pNext;
         else
         {
            *pch = 0;
            pch += pslChgCurr->m_nLth;
            strcpy( szBufferTo, szBuffer );
            pslChgCurr = pslChgCurr->m_pNext;
            strcat( szBufferTo, pslChgCurr->m_pchName );
            strcat( szBufferTo, pch );
            strcpy( szBuffer, szBufferTo );
         }
      }

      fputs( szBuffer, fpWrite );
//    fputs( "\n", fpWrite );
//    cerr << k++ << " - " << szBuffer << endl;
   }

   fclose( fpWrite );
   remove( szFileBack );
// cerr << "Removing " << szFileBack << endl;
   rename( cpcFileName, szFileBack );
// cerr << "Renaming " << cpcFileName << " to " << szFileBack << endl;
   rename( szFileTemp, cpcFileName );
// cerr << "Renaming " << szFileTemp << " to " << cpcFileName << endl;

   cout << "Rewriting inf file - " << cpcFileName << '\n';
}

/***********************************************************
*
*  Help for AI
*
************************************************************/
void
fnAI_CmdHelp( )
{
   cout << "\nALTERINF " << PGM_VERSION << " - Invalid argument list\n";
   cout << "syntax:   ALTERINF  filespec.inf change.dat  [/?]\n\n"
        << "Two positional arguments may be specified\n"
        << " File spec of the .inf file (including drive and directory)\n"
        << " File containing change records\n"
        << "Optional switches\n"
        << " /? - display this information\n";

   cout.flush( );
}

