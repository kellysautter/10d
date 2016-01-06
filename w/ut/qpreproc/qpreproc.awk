BEGIN {
   nIfCount = 0;
   bPrint = 1;
   bPrintIdx[ 0 ] = 1;
   bElIfIdx[ 0 ] = 1;
   bDebug = 0;
}

{
   # Loop through each of the fields, looking for a non-blank field.
   nParmCount = 0;
   for ( i = 1; i <= NF; i++ )
   {               
      if ( $i != "" && $i != " " )
      {
         # Copy each char one at a time.  If we find a '(', start a new parm.)
         nParmCount++;
         szParm[ nParmCount ] = "";

         for ( j = 1; j <= length( $i ); j++ )
         {
            ch = substr( $i, j, 1 );
            if ( ch == "(" || ch == ")"  )
            {
               if ( j < length( $i ) )
               {
                  nParmCount++;
                  szParm[ nParmCount ] = "";
               }
            }
            else
               szParm[ nParmCount ] = szParm[ nParmCount ] ch;
         }
      }
   }

   # If no non-blank fields then print the line and get the next one.
   if ( nParmCount == 0 )
   {
      if ( bPrint )
         print $0;

      next;
   }

   if ( szParm[ 1 ] == "#ifdef" )
   {
      nIfCount++;

      # We're only going to change bPrint if it's currently TRUE.
      if ( bPrint )
      {
         if ( SYMTAB[ szParm[ i ] ] == "" || SYMTAB[ szParm[ i ] ] == 0 )
            bPrint = 0;
      }

      bPrintIdx[ nIfCount ] = bPrint;

      next;
   }

   if ( szParm[ 1 ] == "#if" || szParm[ 1 ] == "#elif" )
   {
      if ( bDebug )
      {
         print "line >> " $0;
         for ( j = 1; j <= nParmCount; j++ )
         {               
            print "field >> szParm[ " j " ] = '" szParm[ j ] "'";
         }
      }

      if ( szParm[ 1 ] == "#if" )
      {
         nIfCount++;
         bElIfIdx[ nIfCount ] = 0;
      }
      else
      {
         # Check current value of bElIfIdx[ nIfCount ].  If it is 1 then
         # the first '#if' or a previous '#elif' at this level was already
         # true so the current one can't be true.
         bPrintIdx[ nIfCount ] = !bElIfIdx[ nIfCount ];
         bPrint = !bElIfIdx[ nIfCount ];

         # Check to see if there are any "parent" #if's that are not printing.
         # This means that the current #if shouldn't print either.
         for ( j = nIfCount - 1; j > 0; j-- )
         {
            if ( !bPrintIdx[ j ] )
            {
               bPrint = 0;
               break;
            }
         }
      }

      if ( bPrint )
      {
         bResult = 1;
         bOper   = 1;      # 1 = AND, 2 = OR.

         # Loop through each of the fields.
         for ( i = 2; i <= nParmCount; i++ )
         {
            if ( szParm[ i ] == "" )
               continue;

            if ( szParm[ i ] == "||" )
            {
               bOper = 2;
               continue;
            }

            if ( szParm[ i ] == "&&" )
            {
               bOper = 1;
               continue;
            }

            if ( szParm[ i ] == "defined" )
            {
               i++;   # Skip to next field.

               if ( bDebug )
                  print ">> Checking def szParm[ " i " ] " szParm[ i ] " = " SYMTAB[ szParm[ i ] ]

               if ( SYMTAB[ szParm[ i ] ] == "" || SYMTAB[ szParm[ i ] ] == 0 )
                  bTemp = 0;
               else
                  bTemp = 1;
            }
            else
            if ( szParm[ i ] == "!defined" )
            {
               i++;   # Skip to next field.

               if ( bDebug )
                  print ">> Checking def szParm[ " i " ] " szParm[ i ] " = " SYMTAB[ szParm[ i ] ]

               if ( SYMTAB[ szParm[ i ] ] == "" || SYMTAB[ szParm[ i ] ] == 0 )
                  bTemp = 1;
               else
                  bTemp = 0;
            }
            else
            {
               if ( bDebug )
                  print ">> Checking szParm[ " i " ] " szParm[ i ] " = " SYMTAB[ szParm[ i ] ]

               if ( SYMTAB[ szParm[ i ] ] == "" || SYMTAB[ szParm[ i ] ] == 0 )
                  bTemp = 0;
               else
                  bTemp = 1;
            }

            if ( bOper == 1 )
               bResult = bResult && bTemp;
            else
               bResult = bResult || bTemp;

         } # for ...

         bPrint = bResult;
         bPrintIdx[ nIfCount ] = bPrint;

         if ( bPrintIdx[ nIfCount ] )
         {
            bElIfIdx[ nIfCount ] = 1;
         }

      } # if ( bPrint )...

      next;

   } # if ( szParm[ 1 ] == "#if" )...

   if ( szParm[ 1 ] == "#else" )
   {
      # Swap current print value.
      bPrintIdx[ nIfCount ] = \
                        ! ( bPrintIdx[ nIfCount ] || bElIfIdx[ nIfCount ] );
      bPrint = bPrintIdx[ nIfCount ];

      # Check to see if there are any "parent" #if's that are not printing.
      # This means that the current #if shouldn't print either.
      for ( j = nIfCount - 1; j > 0; j-- )
      {
         if ( !bPrintIdx[ j ] )
         {
            bPrint = 0;
            break;
         }
      }

      next;
   }

   if ( szParm[ 1 ] == "#endif" )
   {
      if ( nIfCount == 0 )
      {
         print "UNEXPECTED #endif!!";
         exit;
      }

      nIfCount--;
      bPrint = bPrintIdx[ nIfCount ];

      next;
   }

   if ( bPrint )
   {
      print $0;
   }

}
