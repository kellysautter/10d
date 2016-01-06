//./ ADD NAME=TranslateToUnderscoreCase
/////////////////////////////////////////////////////////////////////////////
//
//   ENTRY:    TranslateToUnderscoreCase
//
//   PROTOTYPE:
//        void OPERATION
//        TranslateToUnderscoreCase( zPCHAR pchSrc,
//                                   zPCHAR pchTarget,
//                                   zSHORT targetLth )
//
//   PURPOSE:  Converts a string from Camel Casing to underscores.  E.g.:
//
//                  ThisIsCamelCase to this_is_camel_case
//                  ThisIsHTTPCase to this_is_http_case
//                  ThisIsHTTP to this_is_http
//                  ThisIsATest to this_is_a_test
//                  nocamel_case to nocamel_case
//                  noLeadingUpper to no_leading_upper
//
//   RETURNS: 0 or zCALL_ERROR
//
//////////////////////////////////////////////////////////////////////////////
zSHORT OPERATION
TranslateToUnderscoreCase( zPCHAR pchSrc, zPCHAR pchTarget, zSHORT targetLth )
{
  zPCHAR targetEnd = pchTarget + targetLth - 1; // Point to last char in target buffer.
  zPCHAR original = pchSrc;
  zPCHAR p;

  // Defensive programming...
  *targetEnd = 0;

  if ( zSTRLEN( pchSrc ) >= targetLth )
    return zCALL_ERROR;

  p = pchTarget;

  // Copy first char to target.
  *p++ = *pchSrc++;

  while ( *pchSrc != 0 )
    {
      // Do we have enough space to add two more chars?
      if ( p + 2 > targetEnd )
   return -16;

      if ( *pchSrc >= 'A' && *pchSrc <= 'Z' )
   {
     // We have a capital letter.  Is previous letter upper?
     if ( *(p-1) < 'A' || *(p-1) > 'Z' )
       // No, so copy _.
       *p++ = '_';
     else
       // Previous letter is also upper.  Insert _ if next char is lower.
       if ( ( *(pchSrc+1) < 'A' || *(pchSrc+1) > 'Z' ) && *(pchSrc+1) != 0 )
         *p++ = '_';
   }

      *p++ = *pchSrc++;
    }

  *p++ = 0;

  // Now convert to all lower.
  SysTranslateString( pchTarget, 'L' );

  return 0;
}
