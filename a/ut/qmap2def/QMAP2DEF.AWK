BEGIN {
   bPrint = 0
}

index( toupper( FILENAME ), ".DFB" ) != 0 \
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

   print toupper( $2 ) "=" $2
}
