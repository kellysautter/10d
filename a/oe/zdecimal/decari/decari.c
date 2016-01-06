/*
** decari.c
**
**  special decimal arithmetic
**
**  Description
**
**  The decimal is represented by N = DECARI_N bytes.
**  The first is the exponent, the rest is used for the mantissa.
**  The base of the system is 100.
**  The first byte (exponent) is defined as follows:
**  0x80 = sign bit, on means: the whole number is negative.
**  0x7F = exponent, 0x40 is the base.
**  The n-th byte of the mantissa is the coefficient according to 100**-n
**  (n >= 0) .
**
**  Examples, let N=6
**  0x410102030405 =
**     (1*100**0 +2*100**-1 +3*100**-2 +4*100**-3 +5*100**-4) *100**1 =
**     102.030405
**  0x3E63584D4237 =
**     (99*100**0 +88*100**-1 +77*100**-2 +66*100**-3 +55*100**-4) *100**-2 =
**     0.00009988776655
**  0xC30C0D0E0F10 =
**    -(12*100**0 +13*100**-1 +14*100**-2 +15*100**-3 +16*100**-4) *100**+3 =
**    -121314.1516
**
**  Advantage
**  o  we have float with all benefits for the user
**  o  we have a base-10 compatible system, therefore no rounding errors
**
*/

/*
   Change Log

1999.10.12  HH All
   corrected rounding bug (0.009 rounded to 2 digits was 1.0 ).

1999.06.09  LW All Aktiviere Siron
   eliminated too long line + superfluous line feeds.
   New Format+Sequence of Log-Entries.

08.05.1998-30.5.98  HH
   created new

20.07.1998  HH
   fixed a bug : last digit was trashed in division if mantissa
   of numerator was of maximum length.

21.07.1998  HH
   fixed a bug in addition: carry was lost for first mantissa byte.

10.08.1998  HH
   fixed comparison bug 0.0 != 0.0

11.08.1998  HH
   I N T E R F A C E  C H A N G E
   changed decari_to_string, so that nPrecision = 0 now truncates
   the fractional part of the number.

18.08.1998  HH
   in decari_to_string format 0.0 according to nPrecision.

24.08.1998  CS
   new functions decari_to_packed, packed_to_decari, hzadec

11.09.1998  HH
   fixed "last digit" bug in addition.

18.02.1999  AV/CS
   fixed error in decari_to_packed, a 'const' parameter was changed

22.02.1999  HH
   fixed bug in rounding (round( 1.11, 2 ) was 1.2)

03.01.1999  AV
   introduced const keyword
   added common constants
   added decari_sign()
   added decari_negate()
   added decari_abs()
   added decari_ipow10()

02.03.1999  HH
   fixed bug in rounding (round( 0.5 ) was 0)
   added new function decari_int.

05.03.1999  HH
   improve last rounding fix.

10.03.1999  CS
   return code for exponent ge maximum in decari_to_packed

17.05.1999  HH
   minor improvements (performance).

24.06.1999  HH
   a bug (subtraction) had been introduced by the previous change.

27.07.1999  CF
    maximum in decari_to_packed changed, as mantissa was increased.

*/

#include <string.h>
#include <stdio.h>

#if 0
#include <assert.h>
#endif

#include "decari.h"

static int compare_mantissa( const tDec *, const tDec * );
static int add_internal( tDec *pd1, tDec *pd2, tDec *pd3 );
static int sub_internal( tDec *pd1, tDec *pd2, tDec *pd3 );
static int round( unsigned char *c, unsigned short m, unsigned char cTest );
static int normalize( tDec *pd1);

#define WHITESPACE " \t"
#define NUMERIC "1234567890"

#define DECARI_ISZERO( pd ) \
        ( memcmp( (pd)->mant, decari_0.mant, DECARI_M ) == 0 )

/* ================================================================= */

/* decari_add
 * ==========
 * add d1+d2 result = d3
 *
 *  return : 1 exponent overflow
 *          -1 exponent underflow
 *          -3 Nan Input
 *           0 ok
 */
int
decari_add( const tDec *pd1, const tDec *pd2, tDec *pd3 )
{
   int sign1, sign2, iexp1, iexp2;
   tDec a1,a2,a3;
   tDec *pa1, *pa2;
   int k;

   /* if one of the input numbers is Nan, return Nan
    */
   if ( decari_isnan( pd1 ) || decari_isnan( pd2 ) )
   {
      memset( pd3, 0xff, DECARI_N );
      return( -3 );
   }


   memcpy (&a1, pd1, sizeof (tDec));
   memcpy (&a2, pd2, sizeof (tDec));
   sign1 = (a1.expo & 0x80) ? 1 : 0;
   a1.expo = a1.expo & 0x7F; /* clear sign bit */
   sign2 = (a2.expo & 0x80) ? 1 : 0;
   a2.expo = a2.expo & 0x7F; /* clear sign bit */
   iexp1 = (int) a1.expo;
   iexp2 = (int) a2.expo;
   if ( sign1 == sign2 )
   {
      /* add */
      if ( decari_compare( &a1, &a2 ) == 1 )
      {
         /* a1 > a2 */
         pa1 = &a1;
         pa2 = &a2;
      }
      else
      {
         /* a1 < a2 */
         pa1 = &a2;
         pa2 = &a1;
         k = iexp1;
         iexp1 = iexp2;
         iexp2 = k;
      }

      k = add_internal( pa1, pa2, &a3 );
   }
   else
   {
      /* subtract */
      if ( decari_compare( &a1, &a2 ) == 1 )
      {
         /* a1 > a2 */
         pa1 = &a1;
         pa2 = &a2;
      }
      else
      {
         /* a1 < a2 */
         pa1 = &a2;
         pa2 = &a1;
         k = iexp1;
         iexp1 = iexp2;
         iexp2 = k;
         k = sign1;
         sign1 = sign2;
         sign2 = k;
      }

      k = sub_internal( pa1, pa2, &a3 );
   }

   if ( DECARI_ISZERO( (&a3) ) )
   {
      /* result is zero
       */
      memset( pd3, 0, DECARI_N );
      return( 0 );
   }

   iexp1 = iexp1 +k;

   if ( iexp1 > 0x7F )
   {
      /* Exponent overflow
       * move 'NaN' to return area
       */
      memset ( pd3, 0xFF, DECARI_N);
      return( 1 );
   }

   k = normalize( &a3 );
   iexp1 = iexp1 - k;
   if ( iexp1 < 0 )
   {
      /* Exponent underflow
       */
      memset ( pd3, 0x00, DECARI_N);
      return( -1 );
   }

   memcpy( pd3->mant, a3.mant, DECARI_M );
   pd3->expo = (unsigned char) iexp1;
   if ( sign1 )
      pd3->expo |= 0x80;

   return( 0 );
}

/* decari_sub
 * ==========
 * subtract d1 -d2,  result = d3
 *
 *  return : 1 exponent overflow
 *          -1 exponent underflow
 *          -3 NaN Input
 *           0 ok
 */
int
decari_sub( const tDec *pd1, const tDec *pd2, tDec *pd3 )
{
   tDec a2;
   memcpy( &a2, pd2, sizeof( tDec ) );

   /* * toggle sign of 2. operand */
   a2.expo = a2.expo ^ 0x80;
   return( decari_add( pd1, &a2, pd3 ) );
}

/* ================================================================= */

/* decari_mult
 * ===========
 * multiply d1 * d2,  result = d3
 *
 *  return : 1 exponent overflow
 *          -1 exponent underflow
 *          -3 NaN Input
 *           0 ok
 */
int
decari_mult( const tDec *pd1, const tDec *pd2, tDec *pd3 )
{
   tDec a3;
   int sign1, sign2, iexp1, iexp2, iexp3, k;
   short i,j;
   unsigned char a[ DECARI_M ];
   unsigned char b[ DECARI_M ];
   unsigned long q[ 2 * DECARI_M - 1 ];
   unsigned long la=0;

   /* if one of the input numbers is Nan, return Nan */
   if ( decari_isnan( pd1 ) || decari_isnan( pd2 ) )
   {
      memset( pd3, 0xff, DECARI_N );
      return( -3 );
   }

   sign1 = (pd1->expo & 0x80) ? 1 : 0;
   iexp1 = (int) (pd1->expo & 0x7F);  /* clear sign bit */
   iexp1 = iexp1 - 0x40;
   sign2 = (pd2->expo & 0x80) ? 1 : 0;
   iexp2 = (int) (pd2->expo & 0x7F);  /* clear sign bit */
   iexp2 = iexp2 - 0x40;


   memcpy( a, pd1->mant, DECARI_M );
   memcpy( b, pd2->mant, DECARI_M );

   for ( i = 0; i < 2 * DECARI_M - 1; i++ )
      q[i] =0;

   /* Multiply */
   for ( i = 0; i < DECARI_M; i++ )
   {
      for ( j = 0; j < DECARI_M; j++ )
      {
         k = i + j;
         q[ k ] = q[ k ] + (long) a[ i ] * (long) b[ j ];
      }
   }

   /* Normalize */
   for ( i = 2 * DECARI_M - 2; i >= 0; i-- )
   {
      la = 0;
      if ( q[ i ] < 100 )
         continue;

      la = q[ i ];
      q[ i ] = la % 100;
      la = la / 100;
      if ( i > 0 )
         q[ i - 1 ] += la;
   }

   iexp3 = iexp1 + iexp2;
   if ( la > 0 )
     iexp3 += 1;

   if ( iexp3 > 0x3F )
   {
     /* Exponent overflow
        move 'NaN' to return area */
     memset ( pd3, 0xFF, DECARI_N);
     return( 1 );
   }

   if ( iexp3 < -0x40 )
   {
     /* Exponent underflow, normally no problem
        move x00 to return area */
     memset ( pd3, 0x00, DECARI_N);
     return( -1 );
   }

   a3.expo = (unsigned char) (iexp3 + 0x40);
   if ( la > 0 )
   {
#if 0
      assert( la < 100 );
#endif
      a3.mant[ 0 ] = (unsigned char) la;
      for ( i = 1; i < DECARI_M; i++ )
      {
        a3.mant[ i ] = (unsigned char) q[ i - 1 ];
      }

      j = (unsigned short) q[ DECARI_M -1 ];
   }
   else
   {
      for ( i = 0; i < DECARI_M; i++ )
      {
        a3.mant[ i ] = (unsigned char) q[ i ];
      }

      j = (unsigned short) q[ DECARI_M ];
   }

   /* round */
   if ( j >= 50 )
   {
      k = round( a3.mant, DECARI_M -1, 99 ); /* force rounding */
   }

   if ( k == 1 )
   {
      /* we just rounded 0.9999999.. up to 1.000... */
      if ( a3.expo == 0x3F )
      {
         /* hard, but Overflow
          * move 'NaN' to return area
          */
         memset( pd3, 0xFF, DECARI_N);
         return( 1 );
      }
      else
      {
         a3.expo ++;
         a3.mant[ 0 ] = 1;
      }
   }

   if ( sign1 != sign2 )
   {
      /* result is negative */
       a3.expo |= 0x80;
   }

   memcpy( pd3, &a3, DECARI_N );
   return( 0 );
}

/* ================================================================= */

/* decari_div
 * ==========
 * divide d1 / d2,  result = d3
 *
 *  return : 1 exponent overflow
 *          -1 exponent underflow
 *          -2 zero divide
 *          -3 NaN Input
 *           0 ok
 */
int decari_div( const tDec *pd1, const tDec *pd2, tDec *pd3 )
{
   int sign1, sign2, iexp1, iexp2, iexp3;
   short i, j;
   unsigned char a[ DECARI_M + 1 ];
   unsigned char b[ DECARI_M ];
   unsigned char c[ DECARI_M + 2 ];
   unsigned long q[ DECARI_M + 1 ];
   long p[ DECARI_M+1 ];
   unsigned long la, lb, lc;

   /* if one of the input numbers is Nan, return Nan
    */
   if ( decari_isnan( pd1 ) || decari_isnan( pd2 ) )
   {
      memset( pd3, 0xff, DECARI_N );
      return( -3 );
   }

   /* test for zero divide
    * if yes, return NaN
    */
   if ( DECARI_ISZERO( pd2 ) )
   {
      memset( pd3, 0xff, DECARI_N );
      return( -2 );
   }

   sign1 = (pd1->expo & 0x80) ? 1 : 0;
   iexp1 = (int) (pd1->expo & 0x7F);  /* clear sign bit */
   iexp1 = iexp1 - 0x40;
   sign2 = (pd2->expo & 0x80) ? 1 : 0;
   iexp2 = (int) (pd2->expo & 0x7F);  /* clear sign bit */
   iexp2 = iexp2 - 0x40;


   memcpy (a+1, pd1->mant, DECARI_M);
   memcpy (b, pd2->mant, DECARI_M);
   a[ 0 ] = 0x00;


  /* Divide
   */
   for( i=0; i < DECARI_M + 2; i++ )
   {
     /* Determine c[i]
      *   c[i] = a[0]*100 +a[1]*100**0 + a[2]*100**-1 ...
      *        / b[0]*100**0 +a[b]*100**-1 + b[2]*100**-2 ...
      *   First guess :
      *   c[i] = a[0]*100**3 +a[1]*100**2+a[2]*100+a[3] +1
      *        / b[0]*100**2 +b[1]*100*+b[2]
      *   the "+1" is to make sure, that the guess result is not
      *   less then the actual result.
      */
      la = a[0]*100*100*100 +a[1]*100*100 +a[2]*100 +a[3]+1;
      lb = b[0]*100*100 +b[1]*100 +b[2];
      lc = la / lb;

      if ( lc >= 100 )
         lc = 99;

      for( ;; )
      {
        if ( lc == 0 )
        {
           /* shift and continue outer loop
            */
            c[ i ] = 0;
            for( j=0; j < DECARI_M; j++ )
            {
               a[ j ] = a[ j+1 ];
            }
            a[ DECARI_M ] = 0;
            break;
         }

         c[ i ] = (unsigned char) lc;
         /* Multiply back
          */
         la = 0;
         for( j= DECARI_M; j >0; j-- )
         {
            q[ j ] = lc * b[ j-1 ] +la;
            if ( q[ j ] >= 100 )
            {
               la = q[ j ] / 100;
#if 0
               assert( la < 100 );
#endif
               q[ j ] = q[ j ] % 100;
            }
            else
            {
               la = 0;
            }
         }
         q[ 0 ] = la;

         /* Subtract q from a.  Remainder must be < b. */
         la = 0;
         for( j= DECARI_M; j >= 0; j-- )
         {
            p[ j ] = (long) a[ j ] - q[ j ];
            if ( p[ j ] < 0 )
            {
               p [j] = p[ j ] + 100;
               if ( j > 0 )
                  q[ j-1 ] = q[ j-1 ] + 1;
               else
                  la = 1;
            }
         }

         if ( la != 0 )
         {
            /* our guess was too high, try again with 1 smaller. */
            lc --;
            continue;
         }
         else
         {
            /* shift and continue */
#if 0
            assert( p[ 0 ] == 0 );
#endif
            for( j = 0; j < DECARI_M; j++ )
            {
               a[ j ] = (unsigned char) p[ j+1 ];
            }

            a[ DECARI_M ] = 0;
            break;
         }
      }
   }

   /* round if neccessary */
   if ( c[ 0 ] == 0 )
   {
      if ( c[ DECARI_M +1 ] >= 50 )
      {
         if ( round( c, DECARI_M, c[ DECARI_M+1 ] ) == 1 )
            c[ 0 ] = 1;
      }
   }

   if ( c[ 0 ] == 0 )
   {
      --iexp1;
      for( j=0; j <= DECARI_M; j++ )
      {
         c[ j ] = c[ j+1 ];
      }
      c[ DECARI_M +1 ] = 0;
   }

   /* round (again) if neccessary
    */
   if ( round( c, DECARI_M-1, c[ DECARI_M ] ) == 1 )
   {
      ++iexp1;
      c[ 0 ] = 1;
   }

   iexp3 = iexp1 - iexp2;

   if ( iexp3 > 0x3F )
   {
     /* Exponent overflow
        move 'NaN' to return area
     */
     memset( pd3, 0xFF, DECARI_N );
     return( 1 );
   }

   if ( iexp3 < -0x40 )
   {
     /* Exponent underflow, normally no problem
        move x00 to return area
     */
     memset ( pd3, 0x00, DECARI_N);
     return( -1 );
   }

   (*pd3).expo = (unsigned char) (iexp3 + 0x40);
   for ( i = 0; i < DECARI_M; i++ )
   {
     pd3->mant[ i ] = c[ i ];
   }

   if ( sign1 != sign2 )
   {
      /* result is negative */
      pd3->expo |= 0x80;
   }

   return( 0 );
}

/* ================================================================= */

/* decari_compare
 * =============
 * compare d1 versus d2
 *
 *  return : 1 d1 > d2
 *          -1 d1 < d2
 *           0 d1 == d2
 *           2 one or two comparands are NaN
 */
int
decari_compare( const tDec *pd1, const tDec *pd2 )
{
   int sign1, sign2, iexp1, iexp2;
   int k;

   /* Important:
    * a NaN  n e v e r  compares
    */
   /*
    * The following statement was replaced to get the compare
    *  as fast as possible.
    *  As both numbers should not be garbage,
    *  testing the first mantissa byte should be sufficient
   if ( decari_isnan( pd1 ) || decari_isnan( pd2 ) )
   */
   if ( pd1->mant[0] >= 100 || pd2->mant[0] >= 100 )
      return( 2 );

   sign1 = (pd1->expo & 0x80) ? 1 : 0;
   iexp1 = (int) (pd1->expo & 0x7F);  /* clear sign bit */
   sign2 = (pd2->expo & 0x80) ? 1 : 0;
   iexp2 = (int) (pd2->expo & 0x7F);  /* clear sign bit */

   /* If one comparand is zero, then the sign of
    *  the other comparand defines the result
    */
   if ( DECARI_ISZERO( pd1 ) )
   {
      if ( DECARI_ISZERO( pd2 ) )
         return( 0 );
      else
      if ( sign2 )
         return( 1 ); /* d2 < 0, so 0 > d2 */
      else
         return( -1 );
   }
   else
   if ( DECARI_ISZERO( pd2 ) )
   {
      if ( sign1 )
         return( -1 ); /* d1 < 0 */
      else
         return( 1 );
   }

   if ( sign1 == 0 && sign2 == 1 )
      return( 1 ); /* d1 >0, d2 <0 */

   if ( sign1 == 1 && sign2 == 0 )
      return( -1 ); /* d1 <0, d2 >0 */

   if ( sign1 )
   {
      /* both negative */
      if ( iexp1 > iexp2 )
      {
         /* d1 is absolute bigger, therefore smaller */
         return( -1 );
      }
      else
      if ( iexp1 < iexp2 )
      {
         /* d1 is absolute smaller, therefore bigger */
         return( 1 );
      }

      /* sign and exponent are same */
      k = compare_mantissa( pd1, pd2 );
      if ( k == 0 )
         return( 0 );
      else
      if ( k == 1 )
         return( -1 );
      else
         return( 1 );
   }
   else
   {
      /* both positive */
      if ( iexp1 > iexp2 )
      {
         return( 1 );
      }
      else
      if ( iexp1 < iexp2 )
      {
         return( -1 );
      }

      /* sign and exponent are same */
      k = compare_mantissa( pd1, pd2 );
      if ( k == 0 )
         return( 0 );
      else
      if ( k == 1 )
         return( 1 );
      else
         return( -1 );
   }
}

/* ================================================================= */

/* decari_from_string
 * ==================
 * scan decimal from string
 *
 * valid input is
 * [+-]9999.8888E[+-]110
 *
 *  return : 1 exponent overflow
 *          -1 exponent underflow
 *           0 ok
 *           3 invalid String, NaN
 */
int
decari_from_string( tDec *pd1, const char * szDec )
{
   int i, j;
   int iSign = 0, iSignE = 1, iExp = 0;
   const char *pc = szDec;
   const char *pc1;
   char cx[ 256 ];

   pc = pc+strspn(pc,WHITESPACE);   /* Blank-Elimination */
   if ( *pc == '-' )
   {
      iSign = -1;
      pc++;
   }
   else
   if (*pc == '+')
   {
      iSign = 1;
      pc++;
   }

   /* Check if we have a valid number
    *  otherwise return NaN
    */
   if ( *pc == '.' )
      pc1 = pc+1;
   else
      pc1 = pc;

   if ( strspn( pc1, NUMERIC) == 0 )
   {
      memset( pd1, 0xff, DECARI_N);
      return( -3 );
   }

   /* find next char not = '0' */
   i = strspn( pc, "0" );
   pc += i;

   /* Did we find the Comma? */
   if ( *pc == '.' )
   {
      /* Yes, Comma, no significant digits found before
       *  eliminate the following zeroes
       */
      pc++;
      i = strspn( pc, "0" );
      pc += i;
      iExp = iExp -i;
      /* Count the numeric digits following */
      i = strspn( pc, NUMERIC );
      if ( i == 0 )
      {
         /* No digits following '0' result = zero */
         memset( pd1, 0, DECARI_N );
         return( 0 );
      }
      else
      {
         /* Move the digits to buffer */
         memcpy( cx, pc, i );
         cx[ i ] = 0;
         pc += i;
      }
   } else
   {
      /* Preceding digits found.
       * Now search for Comma or delimiter
       *   read over numeric digits
       */
      i = strspn( pc, NUMERIC );
      /* Move the first part of the number */
      if ( i )
      {
         memcpy( cx, pc, i);
         cx[ i ] = 0;
      }
      else
      {
         /* we found digits, which were all = '0'
          *  and no decimal point.
          *  So we have zero result
          */
         memset( pd1, 0x00, DECARI_N);
         return( 0 );
      }
      iExp = i;
      if ( pc[ i ] == '.' )
      {
         pc += i+1;
         i = strspn( pc, NUMERIC );
         if ( i > 0 )
         {
            /* and move the second part too */
            memcpy( cx+iExp, pc, i);
            cx[ iExp +i ] = 0;
            pc += i;
         }
      }
      else
      {
         pc += i;
      }
   }

   if ( *pc == 'E' || *pc == 'e' )
   {
      /* Exponent analysis */
      int iExp2 = 0;
      pc++;
      if ( *pc == '-' )
      {
         iSignE = -1;
         pc++;
      }

      if ( *pc == '+' )
      {
         iSignE = 1;
         pc++;
      }

      i = strspn( pc, NUMERIC ); /* go over numerics */
      if ( i == 0 )
      {
         /* Error, at this place we expect a numerical char */
         memset( pd1, 0xFF, DECARI_N ); /* NaN */
         return( -3 );
      }

      /* Compute Exponent value */
      for( j = 0; j < i; j++ )
      {
         iExp2 = iExp2 * 10;
         /*
          * last nibble = value. ascii and ebcdic as well
          */
         iExp2 = iExp2 + ((unsigned char) pc[ j ] & 0x0F);
      }

      iExp = iExp + iExp2*iSignE;
   }

   /* now our number is
    * (cx[0]*10**-1 + cx[1]*10**-2 +cx[2]*10**-3 + ... )*10**iExp
    * but we need to go to base 100
    * if iExp is even
    * then
    * ((cx[0]*10 + cx[1])*100**-1 + (cx[2]*10+ cx[3])*100**-2 + ... )
    *                                                  *100**(iExp/2)
    * otherwise, if iExp is odd
    * then
    * (cx[0] + cx[1]*10**-1 +cx[2]*10**-2 + ... )*10**(iExp-1)
    * =
    * (cx[0]+ (cx[1]*10+cx[2])*100**-1 +(cx[3]*10+cx[4])*100**-2 +...)
    *                                              *100**((iExp-1)/2)
    */
   if ( iExp % 2 )
   {
      /* Exponent( base 10) is odd. */
      iExp = (iExp -1) / 2;
      memset( pd1->mant, 0, DECARI_M);
      pd1->mant[0] = (unsigned char) cx[ 0 ] & 0x0F;
      for( j=1; j< DECARI_M; j++ )
      {
         unsigned short u;
         if ( cx[ 2*j-1 ] == 0 )
            break;

         u = (unsigned short) ( cx[ 2*j-1 ] & 0x0F );
         u = u * 10;
         if ( cx[ 2*j ] == 0 )
         {
            pd1->mant[ j ] = (unsigned char) u;
            break;
         }

         u = u + (unsigned short) (cx[ 2 * j ] & 0x0F);
         pd1->mant[ j ] = (unsigned char) u;
      }
   }
   else
   {
      /* Exponent (base 10) is even. */
      iExp = (iExp -2) / 2;
      memset( pd1->mant, 0, DECARI_M);
      for( j = 0; j < DECARI_M; j++ )
      {
         unsigned short u;
         if ( cx[ 2 * j ] == 0 )
            break;

         u = (unsigned short) (cx[ 2 * j ] & 0x0F);
         u = u * 10;
         if ( cx[ 2 * j + 1 ] == 0 )
         {
            pd1->mant[ j ] = (unsigned char) u;
            break;
         }

         u = u + (unsigned short) (cx[ 2 * j + 1 ] & 0x0F);
         pd1->mant[ j ] = (unsigned char) u;
      }
   }

   /* Rounding needs to be done here */

   if ( iExp > 63 )
   {
      /* Exponent overflow
       * move 'NaN' to return area
       */
      memset( pd1, 0xFF, DECARI_N );
      return( 1 );
   }

   if ( iExp < -64 )
   {
      /* Exponent underflow
       */
      memset ( pd1, 0x00, DECARI_N );
      return( -1 );
   }

   pd1->expo = (unsigned char) iExp + 0x40;

   if ( iSign < 0 )
      pd1->expo |= 0x80;

   return( 0 );
}

/* ================================================================= */

/* decari_to_string
 * ==================
 * format string from decimal
 *
 * szReturn = char field to be formatted
 *            I M P O R T A N T : has to be long enough!
 *
 * nPrecision = accuracy for the output
 *              digits after comma in F - Format
 *              total digits in E - Format
 *               N O T E  W E L L :
 *               nPrecision does not cause any  r o u n d i n g,
 *               it just  t r u n c a t e s.
 *
 *
 * cEF = 'e' or 'E'
 *      ==> result is [-]1.2345E[-]99 (scientific notation)
 *      otherwise :  [-]12345.6789
 *
 */
void
decari_to_string( char *pchReturn, const tDec *pd1, short nPrecision, char cEF )
{
   static char cDigit[] = "0123456789";
   int iSign, iExp;
   short j;
   char cx[ 2 * DECARI_M + 1 ], cy[ 256 ];
   char *pc;

   /* if we have NaN
    * write "***"
    */
   if ( decari_isnan( pd1) )
   {
      strcpy_s( pchReturn, 256, "***" );
      return;
   }

   /* if we have zero
    * write "0.0" or other representation, according to nPrecision
    */
   if ( DECARI_ISZERO( pd1 ) )
   {
      if ( cEF == 'E' || cEF == 'e' || nPrecision < 0 )
      {
         strcpy_s( pchReturn, 256, "0.0" );
      }
      else
      {
         if ( nPrecision == 0 )
            strcpy_s( pchReturn, 256, "0" );
         else
         {
            short k;
            strcpy_s( pchReturn, 256, "0." );
            for ( k=0; k < nPrecision; k++)
               pchReturn[ k+2 ] = '0';

            pchReturn[ nPrecision+2 ] = 0;
         }
      }

      return;
   }


   iSign = (pd1->expo & 0x80) ? 1 : 0;
   iExp = (int) (pd1->expo & 0x7F);  /* clear sign bit */
   iExp = iExp - 0x40;

   /*
    * first create format for scientific notation
    * without exponent and sign
    */
   for( j=0; j < DECARI_M; j++)
   {
      cx[ 2*j ] = cDigit[ pd1->mant[ j ] / 10 ];
      cx[ 2*j+1 ] = cDigit[ pd1->mant[ j ] % 10 ];
   }
   cx[ 2*DECARI_M ] = 0;
   pc = cy;
   if ( cx[ 0 ] == '0' )
   {
      *(pc++) = cx[ 1 ];
      strcpy_s( pc, sizeof( cy ) - 1 - (pc - cy), cx + 2 );
   }
   else
   {
      strcpy_s( pc, sizeof( cy ) - 1 - (pc - cy), cx );
   }

   /* go to base 10 */
   if ( cx[ 0 ] == '0' )
      iExp = iExp * 2;
   else
      iExp = iExp * 2  + 1;

   if ( cEF == 'E' || cEF == 'e' )
   {
      /* scientific notation
       */
      pc = pchReturn;
      if ( iSign )
      {
         *pc = '-';
         pc++;
      }
      /* truncate, if neccessary
       */
      if ( nPrecision > 0 && (short) strlen( cy ) > nPrecision )
      {
         /* in E-Format we use nPrecision
          * as limit to the total number of digits
          * values <= 0 are not valid
          */
         cy[ nPrecision ] = 0;
      }
      *(pc++) = cy[ 0 ];
      *(pc++) = '.';

      /* remove trailing zeros
       */
      for ( j = (short) strlen( cy + 2 ) -1; j > 0; j--)
      {
         if ( cy[ 2+j ] != '0' )
            break;
         cy[ 2+j ] = 0;
      }
      strcpy_s( pc, 256 - (pc - pchReturn), cy + 1 );
      pc = pc + strlen( pc );
      *(pc++) = 'E';
      sprintf_s( pc, 256 - (pc - pchReturn), "%d", iExp );
   } else
   {
      /* not scientific notation
       */
      pc = pchReturn;
      if ( iSign )
      {
         *pc = '-';
         pc++;
      }

      if ( iExp < 0)
      {
         char *pcComma;

         /* insert 0.0...
          */
         *(pc++) = '0';
         *(pc++) = '.';
         pcComma = pc;
         for( j= -1; j > iExp; j-- )
            *(pc++) = '0';

         /* remove trailing zeros */
         for ( j = (short) strlen( cy + 1 ) -1; j >= 0; j--)
         {
            if ( cy[ 1 + j ] != '0' )
               break;

            cy[ 1+j ] = 0;
         }

         strcpy_s( pc, 256 - (pc - pchReturn), cy );
         /* truncate, if neccessary */
         if ( nPrecision == 0)
            pcComma[ -1 ] = 0; /* Overwrite the decimal point */
         else
         if ( nPrecision > 0 && (short) strlen( pcComma ) > nPrecision )
            pcComma[ nPrecision ] = 0;

      }
      else
      if ( iExp < (short) strlen( cy ) )
      {
         char *pcComma;
         memcpy( pc, cy, iExp+1 );
         pc += iExp + 1;
         *(pc++) = '.';
         pcComma = pc;
         if ( iExp == (short) strlen( cy ) -1 )
         {
            *(pc++) = '0';
            *pc = 0;
         }
         else
         {
            /* remove trailing zeros
             */
         // for ( j = (short) strlen( cy + iExp + 1 ) -1; j > 0; j--)
            for ( j = (short) strlen( cy + iExp + 1 ) -1; j > 0 && j >= nPrecision; j-- )  // dks 2009.11.20
            {
               if ( cy[ iExp + 1 + j ] != '0' )
                  break;
               cy[ iExp + 1 + j ] = 0;
            }

            strcpy_s( pc, 256 - (pc - pchReturn), cy+iExp+1 );
         }

         /* truncate, if neccessary */
         if ( nPrecision == 0)
            pcComma[ -1 ] = 0; /* Overwrite the decimal point */
         else
         if ( nPrecision > 0 && (short) strlen( pcComma ) > nPrecision )
            pcComma[ nPrecision ] = 0;
      }
      else
      {
         /* iExp > strlen( cy ) */
         strcpy_s( pc, 256 - (pc - pchReturn), cy );
         iExp -= strlen( cy );
         for( j = 0; j < iExp; j++ )
            *(pc++) = '0';
         *(pc++) = '.';
         *(pc++) = '0';
         *pc = 0;
      }
   }
}

/* ================================================================= */

/* decari_isnan
 *
 *  test for NaN (Not a Number)
 *  this is true, if at least 1 mantissa byte is > 100
 *  return: 1 if input decimal is Nan
 *          0 otherwise
 */
int
decari_isnan( const tDec *pd )
{
   int i;
   for( i = 0; i < DECARI_M; i++ )
   {
      if ( pd->mant[ i ] >= 100 )
         return( 1 );
   }

   return( 0 );
}

/* ================================================================= */

/* decari_round
 *
 *  Round decimal number
 *  Input parameter nScale =
 *     number of decimal digits remaining after comma
 */
void
decari_round( tDec *pd, unsigned short nScale )
{
   int i;
   unsigned char exp;
   int sign, iRound;
   unsigned short nScale2, nRest;

   /* do nothing if input is NaN or zero
    */
   if ( decari_isnan( pd ) )
      return;

   if ( DECARI_ISZERO( pd ) )
      return;

   sign = (pd->expo & 0x80) ? 1 : 0;
   exp = pd->expo;
   exp &= 0x7f; /* clear sign bit */

   /*
    * iRound is the array position in the mantissa,
    *  where rounding starts,
    *  nRest=1 if the rounding is in the last digit of the ronding
    *    byte, otherwise (iRest=0) it is the first digit.
    * examples
    *   1,23/4 = (exp=x40, mant=(01,23,40, 0..)), (iRound,nRest)=(2,0)
    *   0,12/37 = (exp=x39, mant=(12,37, 0..)), (iRound,nRest)=(1,0)
    *   0,00/9  = (exp=x38, mant=(90,0..)), (iRound,nRest)=(0,0)
    *   1,234/5 = (exp=x40, mant=(01,23,45,0..)), (iRound,nRest)=(2,1)
    *   0,123/7 = (exp=x39, mant=(12,37, 0..)), (iRound,nRest)=(1,1)
    *   0,001/9  = (exp=x38, mant=(19,0..)), (iRound,nRest)=(0,1)
    *
    *for exp = 0x40 the comma is after the 1. byte.
    * here rounding starts.
    */
   iRound = (int) exp - 0x40 + 1;

   /* nScale is shift for base 10, we go to base 100
    * nScale2 is shift for base 100,
    * nRest = 1 if one more digit has to be shifted
    */
   nScale2 = nScale / 2;
   iRound += nScale2;
   nRest = nScale - nScale2*2;

   /* The leftmost byte, where rounding can start, is DECARI_M -1 */
   if ( iRound >= DECARI_M)
      return; /* nothing to round */

   if ( iRound < 0 )
   {
      /* zero result */
      memset(pd, 0, DECARI_N);
      return;
   }

   if ( iRound == 0 )
   {
      if ( nRest == 0 )
      {
         /* nRest = 0 means, that we round to the 10**0 position
          *   so, if the 10**-1 position is >= 5 the result will be 1
          *   otherwise = 0.
          */
         if ( pd->mant[ 0 ] >= 50 )
         {
            /* the result mantissa is (1, 0.. ) */
            memset(pd->mant+1, 0, DECARI_M-1);
            pd->mant[ 0 ] = 1;
            ++pd->expo;
            if ( sign )
              pd->expo |= 0x80;
         }
         else
         {
            /* zero result */
            memset(pd, 0, DECARI_N);
         }
      }
      else
      {
         /* nRest = 1 means, that we round to the 10**-1 position
          *  everything depends on mant[ 0 ] byte.
          */
         int k;
         k = pd->mant[ 0 ] +5;
         k = k - (k % 10);
         if ( k >= 100 )
         {
            /* the result mantissa is (1, 0 .. ) */
            pd->mant[ 0 ] = 1;
            ++pd->expo;
            if ( sign )
              pd->expo |= 0x80;
         }
         else
         {
            pd->mant[ 0 ] = (unsigned char) k;
         }
         memset(pd->mant+1, 0, DECARI_M-1);
      }

      return;
   }

   if ( nRest == 0 )
   {
      /* Normal Rounding starting with iRound-1 Byte
       * bytes iComma ... are cleared.
       */
      int k;
      k = round( pd->mant, (unsigned short) (iRound -1),
                 pd->mant[ iRound ] );
      if ( k )
      {
        ++exp;
        pd->mant[ 0 ] = 1;
      }

      for( i=iRound; i< DECARI_M; i++)
         pd->mant[ i ] = 0x00;
   }
   else
   {
      /* Normal Rounding starting with iRound-1 Byte
       * bytes iRound+1 ... are cleared.
       * special treatment for byte iComma.
       */
      int k;
      k = pd->mant[ iRound ] +5;
      k = k - (k%10);
      if ( k >= 100 )
      {
         pd->mant[ iRound ] = 0x00;
         k = round( pd->mant, (unsigned short) (iRound -1), 99 );
         if ( k )
         {
           ++exp;
           pd->mant[ 0 ] = 1;
         }
      }
      else
         pd->mant[ iRound ] = (unsigned char) k;

      for( i=iRound+1; i< DECARI_M; i++)
         pd->mant[ i ] = 0x00;
   }

   pd->expo = exp;
   if ( sign )
      pd->expo |= 0x80;
}

/* ================================================================= */

/* decari_int
 *
 *  Truncate decimal number to integer part
 *  discard the fractional part
 */
void decari_int( tDec *pd )
{
   int i;
   unsigned char exp;
   int sign, iComma;

   /* do nothing if input is NaN or zero
    */
   if ( decari_isnan( pd ) )
      return;

   if ( DECARI_ISZERO( pd ) )
      return;

   sign = (pd->expo & 0x80) ? 1 : 0;
   exp = pd->expo;
   exp &= 0x7f; /* clear sign bit */

   /* for exp = 0x40 the comma is after the 1. byte.
    * from here everything is truncated.
    */
   iComma = (int) exp - 0x40 + 1;

   if ( iComma >= DECARI_M)
      return; /* no fractional part */

   if ( iComma <= 0 )
   {
      /* zero result */
      memset(pd, 0, DECARI_N);
      return;
   }

   /* all we have to do, is zero out the fractional part
    */
   for ( i = iComma; i < DECARI_M; i++ )
   {
      pd->mant[ i ] = 0x00;
   }
}

/* ================================================================= */

/*
 * round mantissa
 *
 * m = Index, where rounding starts
 * cTest = switch, if cTest > 50 then round
 *
 * return  1 = round result = 1000...
 *         0   otherwise
 */
static int
round( unsigned char *c,
       unsigned short m,
       unsigned char cTest )
{
   unsigned short j;

   if ( cTest < 50 )
      return( 0 );

   c[ m ]++;
   for( j = m; j >=0; j-- )
   {
      if ( c[ j ] >= 100 )
      {
         c[ j ] = c[ j ] -100;
         if ( j > 0 )
            c[ j - 1 ] ++;
         else
            return( 1 );
      }
      else
         break;
   }

   return( 0 );
}

/* ================================================================= */

static int
compare_mantissa( const tDec *pd1, const tDec *pd2 )
{
   unsigned short j;
   for( j = 0; j < DECARI_M; j++ )
   {
      if ( pd1->mant[ j ] > pd2->mant[ j ] )
        return( 1 );

      if ( pd1->mant[ j ] < pd2->mant[ j ] )
        return( -1 );
   }

   return( 0 );
}

/* ================================================================= */

/* add_internal
 *  both arguments positive,
 *  first one is bigger
 *
 * return 0/1 if first byte overflow no/yes
 */
static int add_internal( tDec *pd1, tDec *pd2, tDec *pd3)
{
   unsigned short j;
   unsigned char a1[ DECARI_M + 2 ];
   unsigned char a2[ DECARI_M + 2 ];

   int iShift = pd1->expo - pd2->expo;
   if ( iShift > DECARI_M + 1 )
   {
      /* the 2. argumet is too small, there is
       *  no influence on the result
       */
      memcpy( pd3, pd1, DECARI_N );
      return( 0 );
   }

   memset( a1, 0, sizeof( a1 ) );
   memset( a2, 0, sizeof( a2 ) );

   for( j=0; j < DECARI_M; j++)
   {
      a1[ j+1 ] = pd1->mant[ j ];
   }

   for( j= iShift+1; j < DECARI_M+1; j++)
   {
      a2[ j ] = pd2->mant[ j -iShift -1 ];
   }
   if ( iShift > 0 )
      a2[ DECARI_M+1 ] = pd2->mant[ DECARI_M - iShift ];

   for( j = DECARI_M + 1; j > 0; j-- )
   {
      a1[ j ] = a1[ j ] + a2[ j ];
      if ( a1[ j ] >= 100 )
      {
         a1[ j ] -= 100;
         a1[ j-1 ] += 1;
      }
   }

   if ( a1[ 0 ] == 0 )
   {
      round( a1, DECARI_M, a1[ DECARI_M + 1 ] );
      for( j=0; j < DECARI_M; j++)
      {
         pd3->mant[ j ] = a1[ j + 1 ];
      }

      return( 0 );
   }
   else
   {
      round( a1, DECARI_M-1, a1[ DECARI_M ] );
      for( j = 0; j < DECARI_M; j++ )
      {
         pd3->mant[ j ] =  a1[ j ];
      }

      return( 1 );
   }
}

/* ================================================================= */

/* sub_internal
 *  both arguments positive,
 *  first one is bigger
 *
 *  return 0/-1 if there was borrow in the first byte no/yes
 */
static int
sub_internal( tDec *pd1, tDec *pd2, tDec *pd3)
{
   unsigned short i,j;
   unsigned char a1[ DECARI_M + 2 ];
   unsigned char a2[ DECARI_M + 2 ];
   int iShift = pd1->expo - pd2->expo;
   if ( iShift > DECARI_M + 1 )
   {
      /* the 2. argumet is too small, there is
       *  no influence on the result */
      memcpy( pd3, pd1, DECARI_N );
      return( 0 );
   }

   for( j = 0; j < DECARI_M; j++ )
   {
      a1[ j+1 ] = pd1->mant[ j ];
      a2[ j ] = 0;
   }
   a1[ 0 ] = 0;
   a1[ DECARI_M + 1 ] = 0;
   a2[ DECARI_M ] = 0;
   a2[ DECARI_M + 1 ] = 0;

   for( j = iShift, i = 0; j < DECARI_M + 1 && i < DECARI_M; j++, i++ )
   {
      a2[ j + 1 ] = pd2->mant[ i ];
   }

   /* Subtract */
   for( j = DECARI_M + 1; j > 0; j-- )
   {
      if ( a1[ j ] < a2[ j ] )
      {
         a1[ j ] = a1[ j ]+ 100;
         a1[ j ] = a1[ j ] - a2[ j ];
         a2[ j - 1 ] += 1;
      }
      else
      {
         a1[ j ] = a1[ j ] - a2[ j ];
      }
   }

   round( a1, DECARI_M, a1[ DECARI_M + 1 ] );

   for( j = 0; j < DECARI_M; j++ )
   {
      pd3->mant[ j ] = a1[ j + 1 ];
   }

   if ( a2[ 0 ] == 0 )
      return( 0 );
   else
      return( -1 );
}

/* ================================================================= */

/* normalize
 *
 *  return number of leading zeroes, which were normalized away
 */
static int
normalize( tDec *pd1 )
{
   short j;
   int i = 0;

   /* count leading zeroes */
   for( j = 0; j < DECARI_M; j++ )
   {
      if ( pd1->mant[ j ] == 0 )
         i++;
      else
         break;
   }

   if ( i == 0 )
      return( 0 );

   if ( i < DECARI_M )
   {
      for( j = 0; j < DECARI_M; j++ )
      {
         if ( j < DECARI_M - i )
            pd1->mant[ j ] = pd1->mant[ j + i ];
         else
            pd1->mant[ j ] = 0;
      }
   }

   return( i );
}

/* ================================================================= */

/* Entry for HOST */

void
hzadec( short *p_opco, tAll *pAll )
{
  switch ( *p_opco )
  {
    case OC_PACKED_TO_DECARI:

            pAll->retco = packed_to_decari (pAll->pd1,
                                            pAll->Packed,
                                            pAll->nPrecision);
            return;

    case OC_DECARI_TO_PACKED:

            pAll->retco = decari_to_packed (pAll->pd1,
                                            pAll->Packed,
                                            pAll->nPrecision);
            return;

    case OC_DECARI_FROM_STRING:

            pAll->retco = (short) decari_from_string (pAll->pd1,
                                                      pAll->szDec);
            return;

    case OC_DECARI_TO_STRING:

            decari_to_string  (pAll->szDec, pAll->pd1,
                               pAll->nPrecision, pAll->cEF);
            pAll->retco = 0;
            return;

/* Pruefen */

    case OC_DECARI_COMPARE:

            pAll->retco = decari_compare (pAll->pd1, pAll->pd2);
            return;

    case OC_DECARI_ISNAN:

            pAll->retco = (short) decari_isnan (pAll->pd1);
            return;

/* Rechnen */

    case OC_DECARI_ADD:

           pAll->retco = (short) decari_add (pAll->pd1,
                                             pAll->pd2,
                                             pAll->pd3);
           return;

    case OC_DECARI_SUB:

           pAll->retco = (short) decari_sub (pAll->pd1,
                                             pAll->pd2,
                                             pAll->pd3);
           return;

    case OC_DECARI_MULT:

           pAll->retco = (short) decari_mult (pAll->pd1,
                                              pAll->pd2,
                                              pAll->pd3);
           return;

    case OC_DECARI_DIV:

           pAll->retco = (short) decari_div (pAll->pd1,
                                             pAll->pd2,
                                             pAll->pd3);
           return;

    case OC_DECARI_ROUND:

           decari_round (pAll->pd1, pAll->nScale);
           pAll->retco = 0;
           return;

    default : pAll->retco = RC_OPCO;
              return;
  }
}


/* -----------------------------------------------------------------
Bei der Konvertierung von gepackten Zahlen in das decari-Format
und zurueck, ist entscheidend, ob die Anzahl der Nachkommastellen
gerade oder ungerade ist, wie man an folgenden Beispielen sieht.

Nachkommastellen 3 (ungerade)

123.456 = ( 1*100**0 + 23*100**-1 + 45*100**-2+60*100**-3) * 100 ** 1
ist gepackt:
Byte  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
     00|00|00|00|00|00|00|00|00|00|00|00|01|23|45|6C|

und im decari-Format
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
41|01|17|2D|3C|00|00|00|00|00|00|00|00|00|00|00|

Das heisst, bei gepackt nach decari kann innerhalb der einzelnen Bytes
zugegriffen werden

Nachkommastellen 2 (gerade)

123.45 = ( 1*100**0 + 23*100**-1 + 45*100**-2) * 100** 1
ist gepackt:
Byte  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
     00|00|00|00|00|00|00|00|00|00|00|00|00|12|34|5C|

und im decari-Format
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
41|01|17|2D|00|00|00|00|00|00|00|00|00|00|00|00|

Das heisst, bei gepackt nach decari kann nicht innerhalb
der einzelnen Bytes zugegriffen werden, sondern es geht über
Bytegrenzen hinaus.

Maximal moegliche Stellenzahl

Nachkommastellen ungerade
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
00|00|00|00|00|00|00|00|00|00|00|00|12|23|45|6C| gepackt
   15 14 13 12 11 10 9  8  7  6  5  1  2  3  4   decari Mantissen-Byte

Nachkommastellen gerade
 1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
00|00|00|00|00|00|00|00|00|00|00|00|01|23|45|6C| gepackt
 15 14 13 12 11 10 9  8  7  6  5  4  1  2  3    decari Mantissen-Byte

D.H. bei ungerader Nachkommazahl koennen maximal 29 stellen,
bei gerader Nachkommazahl maximal 30 Stellen verarbeitet werden.

---------------------------------------------------------------*/


short packed_to_decari (tDec *td1, const unsigned char *pck,
                               short nks)
{
  unsigned char   vz;
  unsigned char   tmp;
  unsigned char   hb1;
  unsigned char   hb2;
  short           ipacked;
  short           idecari;
  short           ziffern;
  short           x;
  short           last;

  /*
  ** todo: dieser code modifiziert den Parameter temporaer.
  */
  char *Packed = (char *) pck;

  memset( td1, 0x00, sizeof( tDec ) );

  /* Nachkommastellen gueltig */
  if ( nks < 0 )
    return RC_NKS;

  /* Vorzeichen-bit */
  last = PACKED_N - 1;
  vz = 0x00;
  if ((Packed [last] & (unsigned char) 0x0f)
            == (unsigned char) 0x0d)
    vz = 0x80;

  /* Halb-Byte Vorzeichen ausblenden */
  tmp = Packed [last];
  Packed [last] &= 0xf0;

  idecari = 0;
  /* Nachkommastellen ungerade */
  if ((nks % 2) != 0)
  {
    /* erstes Byte darf nicht belegt sein, sonst passt Zahl nicht in
       Decari-Mantisse */
    if (Packed [0])
    {
      Packed [last] = tmp;
      return RC_PACKED_OVERFLOW;
    }
    /* fuehrende Nullen ueberlesen */
    ipacked = 1;
    ziffern = 2 * DECARI_M - 1;
    while ((ipacked < PACKED_N) && (Packed [ipacked] == 0))
    {
      ipacked ++;
      ziffern -= 2;
    }

    /* linkes Halb-Byte kann noch eine fuehrende Null sein */
    if ((Packed [ipacked] & (unsigned char) 0xf0) == 0)
      ziffern --;

    /* jetzt geht's los - jetzt geht's los - jetzt geht's los ! */
    while (ipacked < PACKED_N)
    {
      hb1 = (Packed [ipacked] & 0xf0) >> 4;
      hb2 = Packed [ipacked ++] & 0x0f;
      if ((hb1 > 9) || (hb2 > 9))
      {
        Packed [last] = tmp;
        return( RC_NO_PACKED );
      }

      td1->mant[ idecari++ ] = 10 * hb1 + hb2;
    }
  }
  else
  /* Nachkommastellen gerade */
  {
    /* linkes Halb-Byte des ersten Byte darf nicht belegt sein,
       sonst passt Zahl nicht in Decari-Mantisse */
    if (Packed [0] & (unsigned char) 0xf0)
    {
      Packed [last] = tmp;
      return RC_PACKED_OVERFLOW;
    }

    /* fuehrende Nullen ueberlesen */
    ipacked = 1;
    ziffern = 2 * DECARI_M;
    while (     (ipacked < PACKED_N)
             && ((Packed [ipacked - 1] & (unsigned char) 0x0f) == 0)
             && ((Packed [ipacked]     & (unsigned char) 0xf0) == 0))
    {
      ipacked ++;
      ziffern -= 2;
    }
    /* rechtes Halb-Byte kann noch eine fuehrende Null sein */
    if ((Packed [ipacked - 1] & (unsigned char) 0x0f) == 0)
      ziffern --;

    /* jetzt geht's los - jetzt geht's los - jetzt geht's los ! */
    while (ipacked < PACKED_N)
    {
      hb1 = Packed [ipacked - 1] & 0x0f;
      hb2 = (Packed [ipacked ++] & 0xf0) >> 4;
      if ((hb1 > 9) || (hb2 > 9))
      {
        Packed [last] = tmp;
        return RC_NO_PACKED;
      }
      td1->mant [idecari ++] = 10 * hb1 + hb2;
    }
  }

  /* Exponent */
  /* falls alles Null ist, kommt durch genulltes Vorzeichen-Halbbyte
     hier -1 raus */
  if (ziffern <= 0)
  {
    /* Exponent laut Definition auf 0 setzen */
    td1->expo = 0;
    Packed [last] = tmp;
    return( 0 );
  }

  /* Exponent berechnen */
  x = ziffern - nks;
  if (x >= 0)
    td1->expo = (x + 1) / 2 - 1;
  else
    td1->expo = x / 2 - 1;

  /* Displacement addieren */
  td1->expo += 0x40;
  /* Vorzeichen einstellen */
  td1->expo |= vz;

  Packed [last] = tmp;
  return( 0 );
}

short
decari_to_packed (const tDec *pd, unsigned char *Packed, short nks )
{
  short            idecari1;
  short            idecari2;
  short            ipacked;
  short            exp;
  short            nullen;
  unsigned short   hb1;
  unsigned short   hb2;
  tDec             td1;
  short            maxexpo;

  memset( Packed, 0x00, PACKED_N );
  td1 = *pd;

  /* Nachkommastellen gueltig */
  if ( nks < 0 )
    return( RC_NKS );

  /* Decari runden auf sinnvolle Stellenzahl (wird aus Exponent und
     Nachkomma ermittelt */

  decari_round( &td1, nks );

  idecari1 = DECARI_M - 1;
  ipacked = 1;

  /* Exponent ermitteln */
  exp = (short) ((td1.expo & (unsigned char) 0x7f) - 0x40);
  /* Ueberpruefen, ob die decari-Zahl in das geforderte gepackte
     Format passt */
  maxexpo = (DECARI_M - 1) - (nks / 2);
  if ( (exp > maxexpo) ||
       ((exp == maxexpo) && ((nks % 2) == 1) &&
        ((td1.mant [0] & 0xf0) != 0x00)) )
  {
    return RC_PACKED_OVERFLOW;
  }

  /* maximale Anzahl fuehrender Nullen ermitteln */
  if ( (nks % 2) == 1 )
    nullen = 2 * DECARI_M - 1;
  else
    nullen = 2 * DECARI_M;

  nullen = nullen - nks - 2 - 2 * exp;

  /* Ueberlesen der Nullen in Decari-Mantisse von rechts */
  while ( (idecari1 >= 0) && (td1.mant [idecari1] == 0) && (nullen > 0) )
  {
    idecari1 --;
    ipacked ++;
    nullen -= 2;
  }

  /* jetzt geht's los - jetzt geht's los - jetzt geht's los ! */
  for ( idecari2 = 0; idecari2 <= idecari1; idecari2 ++ )
  {
    /* Pruefung auf Gueltigkeit */
    if ( td1.mant[ idecari2 ] > (unsigned char) 0x63 )
      return( RC_NO_DECARI );

    hb1 = td1.mant[ idecari2 ] / 10;
    hb2 = td1.mant[ idecari2 ] % 10;
    if ( (nks % 2) != 0 )
    {
      /* Nachkommastellen ungerade */
      Packed[ ipacked++ ] = (hb1 << 4) | hb2;
    }
    else
    {
      /* Nachkommastellen gerade */
      Packed[ ipacked - 1 ] |= hb1;
      Packed[ ipacked ]     |= (hb2 << 4);
      ipacked ++;
    }
  }

  /* Vorzeichen einstellen */
  if ((td1.expo & (unsigned char) 0x80) != 0 )
    Packed [PACKED_N - 1] |= 0x0d;
  else
    Packed [PACKED_N - 1] |= 0x0c;

  return( 0 );
}

/* ========================================================= */
/* the following is needed by siron arith */

/*
** common constants
*/

const tDec decari_0    = { (unsigned char)0x00, { (unsigned char)0 }};
const tDec decari_1    = { (unsigned char)0x40, { (unsigned char)1 }};
const tDec decari_10   = { (unsigned char)0x40, { (unsigned char)10 }};
const tDec decari_m1   = { (unsigned char)0xc0, { (unsigned char)1 }};
const tDec decari_inf  = { (unsigned char)0x7f, { (unsigned char)99 }};
const tDec decari_minf = { (unsigned char)0xff, { (unsigned char)99 }};
const tDec decari_999  = { (unsigned char)0x3f, {
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
  (unsigned char)99,
}};



/*
** returns the absolut value of pd
*/
void
decari_abs( tDec *pd )
{
  if ( decari_sign( pd ) < 0 )
    decari_negate( pd );
}

/*
** returns the sign of pd.
**   -1 if pd < 0
**   +1 if pd > 0
**    0 if pd == 0
*/

int
decari_sign( const tDec *pd )
{
  if ( decari_compare( pd, &decari_0 ) == 0 )
    return( 0 );

  if ( (pd->expo & 0x80) != 0 )
    return( -1 );

  return( 1 );
}

/*
** computes pd * 10^exp
*/

int
decari_ipow10(tDec *pd, int exp )
{
  int rc = 0;

  if ( (int) pd->expo + exp/2 <= 255 )
  {
    pd->expo += exp/2;
    if ((exp % 2) != 0) {
      if (exp > 0)
        rc = decari_mult(pd, &decari_10, pd);
      else
        rc = decari_div(pd, &decari_10, pd);
    }

    return( rc );
  }

  return( 1 ); /* exponent overflow */
}
