#include <process.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <dos.h>
#include <io.h>
#include <errno.h>
#include <time.h>
#include <time.h>
#include <ctype.h>
#include <stat.h>

#ifdef __WIN32__
   #include <windows.h>
#endif

#ifndef TRUE
   #define TRUE  1
   #define FALSE 0
#endif

void PrintHelp( void )
{
   printf( "\n" );
   printf( "QSPAWN.EXE - Used to execute a DOS command, store the standard\n" );
   printf( "             output to a file and then print the output file.\n" );
   printf( "\n" );
   printf( "QSPAWN [!][+]output-file cmd [arg1 arg2...]\n" );
   printf( "\n" );
   printf( "  If '!' is specified in front of the output file name,\n" );
   printf( "  then use stderr instead of stdout.\n" );
   printf( "\n" );
   printf( "  If '+' is specified in front of the output file name,\n" );
   printf( "  then the output of the command is APPENDED to the output file.\n" );
   printf( "\n" );
   return;
}

void fnPrintFile( char * pszFileName )
{
   char szCmd[ 300 ];

   sprintf( szCmd, "type %s", pszFileName );
   system( szCmd );
}

void main ( int argc, char *argv[], char *envp[] )
{
   int    i;
   int    nRC = 0;
   char   szCmd[ 10000 ];
   char * pszFileName;
#ifdef __WIN32__
   int    iStreamType;
   HANDLE hFile;
   HANDLE hConsole;
   SECURITY_ATTRIBUTES sa;
#else
   FILE * stream;
   FILE * stdstream;
#endif

   if ( argc < 3 )
   {
      PrintHelp ();
      exit( 0 );
   }

   szCmd[ 0 ] = 0;
   for ( i = 2; i < argc; i++ )
   {
      strcat( szCmd, " " );
      strcat( szCmd, argv[ i ] );
      printf( "%s ", argv[ i ] );

      // If first char of the argument is '@' then it's an inline file.
      // Print the file.
      if ( argv[ i ][ 0 ] == '@' )
      {
         printf( "\n" );
         fnPrintFile( &argv[ i ][ 1 ] );
      }
   }
   printf( "\n" );

   pszFileName = argv[ 1 ];

#ifdef __WIN32__

   if ( *pszFileName == '!' )
   {
      pszFileName++;
      strcat( szCmd, " 2>" );
   }
   else
      strcat( szCmd, " >" );

   if ( *pszFileName == '+' )
   {
      pszFileName++;
      strcat( szCmd, "> " );
   }
   else
      strcat( szCmd, " " );

   strcat( szCmd, pszFileName );

   nRC = system( &szCmd[ 1 ] );

   if ( nRC == -1 )
      printf( "spawn error: %ld\n", (long) errno );

#else
   if ( *pszFileName == '!' )
   {
      pszFileName++;
      stdstream = stderr;
   }
   else
      stdstream = stdout;

   if ( *pszFileName == '+' )
   {
      pszFileName++;
      stream = freopen( pszFileName, "a", stdstream );
      fseek( stream, 0, SEEK_END );
   }
   else
      stream = freopen( pszFileName, "w", stdstream );

   if ( stream == NULL )
   {
      printf( "Couldn't reopen stdout as '%1'\n", argv[ 1 ] );
      exit( 1 );
   }

   nRC = _spawnvp( _P_WAIT, argv[ 2 ], &argv[ 2 ] );

   fclose( stream );
   stream = freopen( "CON", "w", stdstream );

#endif

   fnPrintFile( pszFileName );

   if ( nRC )
   {
      printf( "\n"
              "QSPAWN returning error running command: %s\n"
              "\n", argv[ 2 ] );
   }

   exit( nRC );
}
