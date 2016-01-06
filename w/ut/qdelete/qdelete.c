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
   printf( "QSPAWN.EXE - Deletes all files listed in command line or file.\n" );
   printf( "\n" );
   printf( "QDELETE file-list | @filename.lst\n" );
   printf( "\n" );
   return;
}

void main ( int argc, char *argv[], char *envp[] )
{
   int    i;
   char   szFile[ 300 ];
   char * pszFileName;
   FILE * f;

   if ( argc < 2 || argv[ 1 ] [ 1 ] == '?' )
   {
      PrintHelp ();
      exit( 0 );
   }

   for ( i = 1; i <= argc; i++ )
   {
      pszFileName = argv[ i ];

      if ( *pszFileName == '@' )
      {
         // Filename begins with a '@' so that means instead of deleting the
         // file we will delete all files listed in the file.
         pszFileName++;

         f = fopen( pszFileName, "r" );
         if ( f == 0 )
         {
            printf( "Error opening file %s\n", pszFileName );
            exit( 1 );
         }

         while ( fgets( szFile, sizeof( szFile ), f ) )
         {
            if ( remove( szFile ) == 0 )
               printf( "File %s deleted.\n", szFile );
         }

         fclose( f );
      }
      else
      {
         if ( remove( pszFileName ) == 0 )
            printf( "File %s deleted.\n", pszFileName );
      }
   }

   exit( 0 );
}
