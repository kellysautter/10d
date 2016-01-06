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
   printf( "QSPAWN [+]output-file cmd [arg1 arg2...]\n" );
   printf( "\n" );
   printf( "  If '+' is specified as the first char in the output file name,\n" );
   printf( "  then the output of the command is APPENDED to the output file.\n" );
   printf( "\n" );
   return;
}

void main ( int argc, char *argv[], char *envp[] )
{
   int    i;
   int    nRC = 0;
   char   szCmd[ 100 ];
   char * pszFileName;
   FILE * stream;

   if ( argc < 3 )
   {
      PrintHelp ();
      exit( 0 );
   }

   for ( i = 2; i < argc; i++ )
      printf( "%s ", argv[ i ] );
   printf( "\n" );

   pszFileName = argv[ 1 ];
   if ( *pszFileName == '+' )
   {
      pszFileName++;
      stream = freopen( pszFileName, "a", stdout );
      fseek( stream, 0, SEEK_END );
   }
   else
      stream = freopen( pszFileName, "w", stdout );

   if ( stream == NULL )
   {
      printf( "Couldn't reopen stdout as '%1'\n", argv[ 1 ] );
      exit( 1 );
   }

   nRC = _spawnvp( _P_WAIT, argv[ 2 ], &argv[ 2 ] );
   fclose( stream );
   stream = freopen( "CON", "w", stdout );

   sprintf( szCmd, "type %s", pszFileName );
   system( szCmd );

   if ( nRC )
   {
      printf( "\n"
              "QSPAWN returning error running command: %s\n"
              "\n", argv[ 2 ] );
   }

   exit( nRC );
}
