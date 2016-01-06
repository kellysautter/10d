BEGIN {
   bPrint = 0
}

index( toupper( FILENAME ), ".MAP" ) == 0 \
{
   print $0
}

index( toupper( FILENAME ), ".MAP" ) != 0 \
{
   if ( $1 == "" )
      next;

   if ( bPrint == 0 )
   {
      if ( $1 == "Address" && $2 == "Export" && $3 == "Alias" )
      {
         bPrint = 1;
      }

      next;

   }
   else
   {
      if ( $1 == "Address" && $2 == "Publics" )
      {
         bPrint = 0;
         next;
      }
   }

   if ( toupper( $2 ) != "___EXPORTEDSTUB" )
      print toupper( $2 ) "=" $2
}
