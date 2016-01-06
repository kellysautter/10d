/*
** MODULE NAME: hzapck.c
**----------------------------------------------------------------------
**   conversion of ZDecimal Data type to Siron datatypes
**   the module was created by Clemens Schlechte
**----------------------------------------------------------------------
**
** CHANGE LOG most recent first order
** ====== ===
**
** 1999.23.08  US  ALL
**    existing module put into Source Control System
**
**
*/

#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* #include "hgenva.h" */  /* Wird von hnmaca.h included */
/* #include "hgmaca.H" */  /* Wird von hnmaca.h included */
#include "hnmaca.h"
#include "hsipca.h"
/*#include "hzaty1.h"  */
#include "decari.h"
#include "hzapck.h"
#include "hzapck2.h"

/* Konstanten */


/* 2 hoch 32 = hex 1 0000 0000 bzw. dez 4 294 967 296 */
const tDec decari_2exp32  = { (unsigned char)0x44,
                              { (unsigned char) 42,
                                (unsigned char) 94,
                                (unsigned char) 96,
                                (unsigned char) 72,
                                (unsigned char) 96
                              }
                            };

/* ================================================================== */
/*      Kopieren einer gepackten Zahl (1)                             */
/* ================================================================== */

/* Diese interne Funktion ueberfuehrt eine gepackte Zahl aus einer
   Struktur der Form T_PCK rechtsbuendig in ein maximal langes Feld,
   der Rest ist Null */

void pck_to_pck16 (unsigned char       *packed,
                   const T_PCK         *pck)
{
  short len;

  len = (pck->stz >> 1) + 1;
  memset (packed, 0, PACKED_N);
  memcpy (packed + PACKED_N - len, pck->packed, len);
}

/* ================================================================== */
/*      Kopieren einer gepackten Zahl (2)                             */
/* ================================================================== */

/* Diese interne Funktion ueberfuehrt eine gepackte Zahl aus einem
   maximal langen Feldin ein Feld der Form T_PCK */

short pck_from_pck16 (const unsigned char *packed,
                      T_PCK               *pck)
{
  short len;
  short i;
  short rc;

  rc = 0;
  len = (pck->stz >> 1) + 1;

  memcpy (pck->packed, packed + PACKED_N - len, len);

        /* Ist in den Bytes, die ggf. weggeschnitten werden,
           etwas drin, dann gibt es einen rc */
  for (i = 0; i < PACKED_N - len; i++)
  {
    if (packed [i])
    {
      rc = ERROR_NO_SPACE;
      break;
    }
  }

  return rc;
}

/* ================================================================== */
/*   Konvertierung  gepackt --> long (Kernfunktion)                   */
/* ================================================================== */

/* Funktion nur fuer interne Verwendung */

short pck_to_lzahl
(
  const T_PCK    *pck,     /* i: Pointer auf gepackte Zahl          */
  long           *lzahl,   /* o: long als Ergebnis                  */
  unsigned short  nk,      /* i: Nachkommastellen von lzahl         */
  long            max      /* i: so gross kann lzahl maximal werden */
)
{
  int             pck_hi;
  long            erg;
  long            tmp;
  unsigned short  digit;
  unsigned short  len;
  unsigned char  *packed;
  short           s;

  packed = pck->packed;
  len = pck->stz;
  if ((len % 2) == 0)
    len ++;                               /* Anzahl Ziffern */
  pck_hi = TRUE;                          /* Beginnen mit hi-nibble */
  erg = 0;                                /* Ergebnis der Umwandlung */

  while (len --)
  {
    getdigit ();
    tmp = (max - digit) / 10;
    if (erg <= tmp)
      erg = erg * 10 + digit;
    else
      return ERROR_NO_SPACE;
  }

  getsign ();

  s = (short) nk - (short) pck->nk;

  if (s > 0)
  {
    if (erg > max / (zLONG) arPowP10 (s))
      return ERROR_NO_SPACE;
  }
  erg = (zLONG) (erg * arPowP10 (s));
  if (digit == IPMI)
    erg = -erg;
  *lzahl = erg;

  return( 0 )
}

/* ================================================================== */
/*   Konvertierung  long --> gepackt (Kernfunktion)                   */
/* ================================================================== */

/* Funktion nur fuer interne Verwendung */

short pck_from_lzahl
(
  T_PCK          *pck,     /* o: gepackte Zahl               */
  long            lzahl,   /* i: long Zahl                   */
  unsigned short  nk       /* i: Nachkommastellen von lzahl  */
)
{
  int             pck_hi;
  unsigned char  *packed;
  unsigned short  len;
  short           s;

             /* Schreibzeiger auf letztes Erg-Byte */
  packed = pck->packed + (pck->stz >> 1);
             /* Beginnen Vorzeichen im unteren Nibble */
  pck_hi = FALSE;
             /* Anzahl Ziffern */
  len = pck->stz;
             /* Vorzeichen ablegen */
  if (lzahl < 0)
  {
    lzahl = -lzahl;
    put_digitrev (IPMI);
  }
  else
    put_digitrev (IPPL);

  s = (zLONG) pck->nk - nk;
  if (s <= 0)
    lzahl = lzahl / (zLONG) arPowM10 (s);
  else
  {
    while (s --)
    {
      put_digitrev ((unsigned char) (0));
      len --;
    }
  }
  while (len --)
  {
    put_digitrev ((unsigned char) (lzahl % 10));
    lzahl /= 10;
  }

  if (pck->stz % 2 == 0)  /* high nib des ersten bytes unbenutzt */
    put_digitrev ((unsigned char) (0));

  if (lzahl != 0)
    return ERROR_NO_SPACE;
  return( 0 )
}

/* ================================================================== */
/*              Konvertierung  gepackt --> long                       */
/* ================================================================== */

short pck_to_long      (const T_PCK    *pck,
                        long           *lo,
                        unsigned short  nk)
{
  short rc;

  rc = pck_to_lzahl (pck, lo, nk, LONG_MAX);

  return rc;
}

/* ================================================================== */
/*              Konvertierung  long --> gepackt                       */
/* ================================================================== */

short pck_from_long    (T_PCK          *pck,
                        long            lo,
                        unsigned short  nk)
{
  short rc;

  rc = pck_from_lzahl (pck, lo, nk);

  return rc;
}

/* ================================================================== */
/*              Konvertierung  gepackt --> short                      */
/* ================================================================== */

short pck_to_short     (const T_PCK    *pck,
                        short          *sh,
                        unsigned short  nk)
{
  short  rc;
  long  zahl;

  zahl = (zLONG) *sh;

  rc = pck_to_lzahl (pck, &zahl, nk, (zLONG) SHRT_MAX);

  *sh = (short) zahl;

  return rc;
}

/* ================================================================== */
/*              Konvertierung  short --> gepackt                      */
/* ================================================================== */

short pck_from_short   (T_PCK          *pck,
                        short           sh,
                        unsigned short  nk)
{
  short rc;
  long  lo;

  lo = (zLONG) sh;
  rc = pck_from_lzahl (pck, lo, nk);

  return rc;
}

/* ================================================================== */
/*              Konvertierung  gepackt --> char                       */
/* ================================================================== */

short pck_to_char   (const T_PCK    *pck,
                        char           *ch,
                        unsigned short  nk)
{
  short  rc;
  long  zahl;

  zahl = (zLONG) *ch;

  rc = pck_to_lzahl (pck, &zahl, nk, (zLONG) SCHAR_MAX);

  *ch = (char) zahl;

  return rc;
}

/* ================================================================== */
/*              Konvertierung  char --> gepackt                       */
/* ================================================================== */

short pck_from_char    (T_PCK          *pck,
                        char            ch,
                        unsigned short  nk)
{
  short rc;
  long  lo;

  lo = (zLONG) ch;
  rc = pck_from_lzahl (pck, lo, nk);

  return rc;
}

/* ================================================================== */
/*   Konvertierung  gepackt --> unsigned long (Kernfunktion)          */
/* ================================================================== */

short pck_to_uzahl     (const T_PCK    *pck,
                        unsigned long  *uzahl,
                        unsigned short  nk,
                        unsigned long   max)
{
  int             pck_hi;
  unsigned long   erg;
  unsigned long   tmp;
  unsigned short  digit;
  unsigned short  len;
  unsigned char  *packed;
  short           s;

  packed = pck->packed;
  len = pck->stz;
  if ((len % 2) == 0)
    len ++;                               /* Anzahl Ziffern */
  pck_hi = TRUE;                          /* Beginnen mit hi-nibble */
  erg = 0;                                /* Ergebnis der Umwandlung */

  while (len --)
  {
    getdigit ();
    tmp = (max - digit) / 10;
    if (erg <= tmp)
      erg = erg * 10 + digit;
    else
      return ERROR_NO_SPACE;
  }

  s = (short) nk - (short) pck->nk;

  if (s > 0)
  {
    if (erg > max / (unsigned long) arPowP10 (s))
      return ERROR_NO_SPACE;
  }

  getsign ();
  if (digit == IPMI)
    return(ERROR_SIG);

  erg = (unsigned long) (erg * arPowP10 (s));
  *uzahl = erg;

  return( 0 )
}

/* ================================================================== */
/*   Konvertierung  unsigned long --> gepackt (Kernfunktion)          */
/* ================================================================== */

short pck_from_uzahl   (T_PCK          *pck,
                        unsigned long   uzahl,
                        unsigned short  nk)
{
  int             pck_hi;
  unsigned char  *packed;
  unsigned short  len;
  short           s;

             /* Schreibzeiger auf letztes Erg-Byte */
  packed = pck->packed + (pck->stz >> 1);
             /* Beginnen Vorzeichen im unteren Nibble */
  pck_hi = FALSE;
             /* Anzahl Ziffern */
  len = pck->stz;

  put_digitrev (IPPL);

  s = (zLONG) pck->nk - nk;
  if (s <= 0)
    uzahl = uzahl / (zLONG) arPowM10 (s);
  else
  {
    while (s --)
    {
      put_digitrev ((unsigned char) (0));
      len --;
    }
  }
  while (len --)
  {
    put_digitrev ((unsigned char) (uzahl % 10));
    uzahl /= 10;
  }

  if (pck->stz % 2 == 0)  /* high nib des ersten bytes unbenutzt */
    put_digitrev ((unsigned char) (0));

  if (uzahl != 0)
    return ERROR_NO_SPACE;
  return( 0 )
}

/* ================================================================== */
/*        Konvertierung  gepackt --> unsigned long (fb4)              */
/* ================================================================== */

short pck_to_ulong     (const T_PCK    *pck,
                        unsigned long  *ulo,
                        unsigned short  nk)
{
  short rc;

  rc = pck_to_uzahl (pck, ulo, nk, ULONG_MAX);

  return rc;
}

/* ================================================================== */
/*        Konvertierung  unsigned long (fb4) --> gepackt              */
/* ================================================================== */

short pck_from_ulong   (T_PCK          *pck,
                        unsigned long   ulo,
                        unsigned short  nk)
{
  short rc;

  rc = pck_from_uzahl (pck, ulo, nk);

  return rc;

}


/* ================================================================== */
/*        Konvertierung  gepackt --> unsigned short (fb2)             */
/* ================================================================== */

short pck_to_ushort    (const T_PCK    *pck,
                        unsigned short *ush,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

  ulo = (unsigned long) *ush;

  rc = pck_to_uzahl (pck, &ulo, nk, USHRT_MAX);

  *ush = (unsigned short) ulo;

  return rc;
}

/* ================================================================== */
/*        Konvertierung  unsigned short (fb2) --> gepackt             */
/* ================================================================== */

short pck_from_ushort  (T_PCK          *pck,
                        unsigned short  ush,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

  ulo = (unsigned short) ush;

  rc = pck_from_uzahl (pck, ulo, nk);

  return rc;

}


/* ================================================================== */
/*        Konvertierung  gepackt --> unsigned char (fb1)              */
/* ================================================================== */

short pck_to_uchar     (const T_PCK    *pck,
                        unsigned char  *uch,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

  ulo = (unsigned long) *uch;

  rc = pck_to_uzahl (pck, &ulo, nk, UCHAR_MAX);

  *uch = (unsigned char) ulo;

  return rc;
}

/* ================================================================== */
/*        Konvertierung  unsigned char (fb1) --> gepackt              */
/* ================================================================== */

short pck_from_uchar   (T_PCK          *pck,
                        unsigned char   uch,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

  ulo = (unsigned char) uch;

  rc = pck_from_uzahl (pck, ulo, nk);

  return rc;

}


/* ================================================================== */
/*        Konvertierung  gepackt --> fb3                              */
/* ================================================================== */

short pck_to_fb3       (const T_PCK    *pck,
                        unsigned char  *fb3,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

  rc = pck_to_uzahl (pck, &ulo, nk, FB3_MAX);

#ifndef TB_SWAP
  fb3 [0] = BYTE_2_OF_LONG (ulo);
  fb3 [1] = BYTE_3_OF_LONG (ulo);
  fb3 [2] = BYTE_4_OF_LONG (ulo);
#else
  fb3 [0] = BYTE_4_OF_LONG (ulo);
  fb3 [1] = BYTE_3_OF_LONG (ulo);
  fb3 [2] = BYTE_2_OF_LONG (ulo);
#endif
  return rc;
}

/* ================================================================== */
/*        Konvertierung  fb3 --> gepackt                              */
/* ================================================================== */

short pck_from_fb3     (T_PCK          *pck,
                        unsigned char  *fb3,
                        unsigned short  nk)
{
  short rc;
  unsigned long ulo;

#ifndef TB_SWAP
  BYTE_1_OF_LONG (ulo) = 0;
  BYTE_2_OF_LONG (ulo) = fb3 [0];
  BYTE_3_OF_LONG (ulo) = fb3 [1];
  BYTE_4_OF_LONG (ulo) = fb3 [2];
#else
  BYTE_1_OF_LONG (ulo) = 0;
  BYTE_2_OF_LONG (ulo) = fb3 [2];
  BYTE_3_OF_LONG (ulo) = fb3 [1];
  BYTE_4_OF_LONG (ulo) = fb3 [0];
#endif
  rc = pck_from_uzahl (pck, ulo, nk);

  return rc;
}

/* ================================================================== */
/*      Konvertierung gepackt --> double                              */
/* ================================================================== */

short  pck_to_double     (const T_PCK  *pck,
                          double       *doub)
{
  int             pck_hi;
  double          erg;
  unsigned short  digit;
  unsigned char  *packed;
  unsigned short  len;

  packed = pck->packed;
  len = pck->stz;

  if ((len % 2) == 0)
    len ++;                               /* Anzahl Ziffern */
  pck_hi = TRUE;                          /* Beginnen mit hi-nibble */
  erg = 0.0;                              /* Ergebnis der Umwandlung */

  while (len --)
  {
    getdigit ();
    erg = erg * 10.0 + (double) digit;
  }
  getsign();
  if (digit == IPMI)
    erg = -erg;

       /* eventuelle Nachkommastellen beruecksichtigen */
  if (pck->nk != 0)
  {
    erg *=  arPowM10 (pck->nk);
  }
  *doub = erg;

  return( 0 )
}

/* ================================================================== */
/*      Konvertierung double --> gepackt                              */
/* ================================================================== */

short  pck_from_double   (T_PCK  *pck,
                          double  doub,
                          double  cut_or_round)
{
  int             pck_hi;
  unsigned short  digit;
  unsigned char  *packed;
  unsigned short  len;

  packed = pck->packed + (pck->stz >> 1);
         /* Beginnen Vorzeichen im unteren Nibble */
  pck_hi = FALSE;
             /* Anzahl Ziffern */
  len = pck->stz;

  if (doub < 0.0)            /* Vorzeichen ablegen */
  {
    doub = -doub;
    put_digitrev (IPMI);
  }
  else
    put_digitrev (IPPL);

       /* eventuelle Nachkommastellen beruecksichtigen */
  if (pck->nk != 0)
  {
    doub *=  arPowP10 (pck->nk);
  }

      /* jetzt noch runden, abschneiden oder irgend etwas dazwischen */
  doub = doub + cut_or_round;
  doub = floor (doub);

  while (len --)
  {
    LDblDigit (digit, &doub);
    put_digitrev ((unsigned char) digit);
  }

  if (pck->stz % 2 == 0)  /* high nib des ersten bytes unbenutzt */
    put_digitrev ((unsigned char) (0));

  return( 0 )
}

/* ================================================================== */
/*      Konvertierung gepackt --> float                               */
/* ================================================================== */

short  pck_to_float      (const T_PCK  *pck,
                          float        *flo)
{
  short rc;
  double doub;

  rc = pck_to_double (pck, &doub);

  *flo = (float) doub;

  return rc;
}

/* ================================================================== */
/*      Konvertierung float --> gepackt                               */
/* ================================================================== */

short pck_from_float     (T_PCK  *pck,
                          float   flo,
                          float   cut_or_round)
{
  short rc;
  double doub;

  doub = (double) flo;

  rc = pck_from_double (pck, doub, cut_or_round);

  return rc;
}

/* ================================================================== */
/*      Vergleich zweier gepackter Zahlen                             */
/* ================================================================== */

short  pck_compare
(
  const T_PCK  *pck1,     /* i: 1. gepackte Zahl            */
  const T_PCK  *pck2,     /* i: 2. gepackte Zahl            */
  short        *result    /* 1 : packed1 > packed2
                             0 : packed1 = packed2
                            -1 : packed1 < packed2          */
)
{
  tDec  decari1;
  tDec  decari2;
  unsigned char  packed1 [PACKED_N];
  unsigned char  packed2 [PACKED_N];
  short rc;

  pck_to_pck16 (packed1, pck1);
  pck_to_pck16 (packed2, pck2);

  rc = packed_to_decari (&decari1, packed1, pck1->nk);
  if (rc)
    return rc;

  rc = packed_to_decari (&decari2, packed2, pck2->nk);
  if (rc)
    return rc;

  /* decari_compare liefert 0, 1, oder -1, je nachdem, wie der
     Vergleich ausfaellt. Der Returnwert 2 fuer Fehler (NaN)
     kann hier nicht mehr auftreten, dieser Fall ist schon
     bei packed_to_decari ausgesondert worden */

  *result = decari_compare (&decari1, &decari2);

  return( 0 )
}

/* ================================================================== */
/*      Minimum zweier gepackter Zahlen                               */
/* ================================================================== */

short  pck_min
(
  const T_PCK   *pck1,   /* i : eine gepackte Zahl                 */
  const T_PCK   *pck2,   /* i : eine zweite gepackte Zahl          */
  T_PCK         *min     /* o : das Minimum dieser beiden          */
)
{
  short result;
  short rc;

  rc = pck_compare (pck1, pck2, &result);
  if (rc)
    return(rc);

  switch (result)
  {
    case 0:  *min = *pck1;   /* p1 = p2 */
             break;

    case 1:  *min = *pck2;   /* p1 > p2 */
             break;

    case -1: *min = *pck1;   /* p1 < p2 */
             break;

    default: /* kann nicht sein, so etwas macht pck_compare nicht */
             return ERROR_MYSTIFY;
  }

  return rc;
}



/* ================================================================== */
/*      Maximum einer gepackten Zahl                                  */
/* ================================================================== */

short  pck_max
(
  const T_PCK   *pck1,      /* eine gepackte Zahl                 */
  const T_PCK   *pck2,      /* eine zweite gepackte Zahl          */
  T_PCK         *max        /* das Maximum diese beiden           */
)
{
  short result;
  short rc;

  rc = pck_compare (pck1, pck2, &result);
  if (rc)
    return(rc);

  switch (result)
  {
    case 0:  *max = *pck1;   /* p1 = p2 */
             break;

    case 1:  *max = *pck1;   /* p1 > p2 */
             break;

    case -1: *max = *pck2;   /* p1 < p2 */
             break;

    default: /* kann nicht sein, so etwas macht pck_compare nicht */
             return ERROR_MYSTIFY;
  }

  return rc;
}

/* ================================================================== */
/*      Addieren zweier gepackter Zahlen                              */
/* ================================================================== */

short  pck_add
(
  const T_PCK  *pck1,     /* i : erster gepackter Summand      */
  const T_PCK  *pck2,     /* i : zweiter gepackter Summand     */
  T_PCK        *pck3      /* o : gepackte Summe (Achtung !!!
                                 die Felder stz und nk der Struktur
                                 sind input, nur packed ist output */
)
{
  tDec  decari1;
  tDec  decari2;
  tDec  decari3;
  unsigned char  packed1 [PACKED_N];
  unsigned char  packed2 [PACKED_N];
  unsigned char  packed3 [PACKED_N];
  short rc;

  pck_to_pck16 (packed1, pck1);
  pck_to_pck16 (packed2, pck2);

  rc = packed_to_decari (&decari1, packed1, pck1->nk);
  if (rc)
    return rc;

  rc = packed_to_decari (&decari2, packed2, pck2->nk);
  if (rc)
    return rc;

  rc = (short) decari_add (&decari1, &decari2, &decari3);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_to_packed (&decari3, packed3, pck3->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (packed3, pck3);

  return rc;
}

/* ================================================================== */
/*      Subtrahieren zweier gepackter Zahlen                          */
/* ================================================================== */

short  pck_sub
(
  const T_PCK  *pck1,     /* i : erster gepackter Subtrahent   */
  const T_PCK  *pck2,     /* i : zweiter gepackter Minuend     */
  T_PCK  *pck3            /* o : gepackte Differenz (Achtung !!!
                                 die Felder stz und nk der Struktur
                                 sind input, nur packed ist output */
)
{
  tDec  decari1;
  tDec  decari2;
  tDec  decari3;
  unsigned char  packed1 [PACKED_N];
  unsigned char  packed2 [PACKED_N];
  unsigned char  packed3 [PACKED_N];
  short rc;

  pck_to_pck16 (packed1, pck1);
  pck_to_pck16 (packed2, pck2);

  rc = packed_to_decari (&decari1, packed1, pck1->nk);
  if (rc)
    return rc;

  rc = packed_to_decari (&decari2, packed2, pck2->nk);
  if (rc)
    return rc;

  rc = (short) decari_sub (&decari1, &decari2, &decari3);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_to_packed (&decari3, packed3, pck3->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (packed3, pck3);

  return rc;
}

/* ================================================================== */
/*      Multiplizieren zweier gepackter Zahlen                        */
/* ================================================================== */

short  pck_mul
(
  const T_PCK  *pck1,     /* i : erster gepackter Faktor      */
  const T_PCK  *pck2,     /* i : zweiter gepackter Faktor     */
  T_PCK  *pck3            /* o : gepacktes Produkt (Achtung !!!
                                 die Felder stz und nk der Struktur
                                 sind input, nur packed ist output */
)
{
  tDec  decari1;
  tDec  decari2;
  tDec  decari3;
  unsigned char  packed1 [PACKED_N];
  unsigned char  packed2 [PACKED_N];
  unsigned char  packed3 [PACKED_N];
  short rc;

  pck_to_pck16 (packed1, pck1);
  pck_to_pck16 (packed2, pck2);

  rc = packed_to_decari (&decari1, packed1, pck1->nk);
  if (rc)
    return rc;

  rc = packed_to_decari (&decari2, packed2, pck2->nk);
  if (rc)
    return rc;

  rc = (short) decari_mult (&decari1, &decari2, &decari3);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_to_packed (&decari3, packed3, pck3->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (packed3, pck3);

  return rc;
}

/* ================================================================== */
/*      Dividieren zweier gepackter Zahlen                            */
/* ================================================================== */

short  pck_div
(
  const T_PCK  *pck1,     /* i : gepackter Divident        */
  const T_PCK  *pck2,     /* i : gepackter Divisor         */
  T_PCK        *pck3      /* o : gepacktes Quotient (Achtung !!!
                                 die Felder stz und nk der Struktur
                                 sind input, nur packed ist output */
)
{
  tDec  decari1;
  tDec  decari2;
  tDec  decari3;
  unsigned char  packed1 [PACKED_N];
  unsigned char  packed2 [PACKED_N];
  unsigned char  packed3 [PACKED_N];
  short rc;

  pck_to_pck16 (packed1, pck1);
  pck_to_pck16 (packed2, pck2);

  rc = packed_to_decari (&decari1, packed1, pck1->nk);
  if (rc)
    return rc;

  rc = packed_to_decari (&decari2, packed2, pck2->nk);
  if (rc)
    return rc;

  rc = (short) decari_div (&decari1, &decari2, &decari3);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_to_packed (&decari3, packed3, pck3->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (packed3, pck3);

  return rc;
}

/* ================================================================== */
/*      Prozent zweier gepackter Zahlen                               */
/* ================================================================== */

short  pck_prozent
(
  const T_PCK  *pck1,         /* i : erste gepackte Zahl p1 */
  const T_PCK  *pck2,         /* i : zweite gepackte Zahl p2 */
  T_PCK        *pck3          /* o : p1 / p2 * 100  (Achtung !!!
                                    die Felder stz und nk der Struktur
                                    sind input, nur packed ist output */
)
{
  short         rc;
  T_PCK         pck;
  unsigned char packed [2];

  packed [0] = 0x10;
  packed [1] = 0x0c;

  pck.packed = packed;
  pck.stz = 3;
  pck.nk = 0;

  rc = pck_mul (pck1, &pck, pck3);
  if (rc)
    return rc;

  rc = pck_div (pck3, pck2, pck3);

  return rc;
}

/* ================================================================== */
/*      Prozent zweier gepackter Zahlen (2)                           */
/* ================================================================== */

short  pck_prozentm
(
  const T_PCK  *pck1,        /* i : erste gepackte Zahl p1 */
  const T_PCK  *pck2,        /* i : zweite gepackte Zahl p2 */
  T_PCK        *pck3         /* o : p1 / p2 * 100 - 100  (Achtung !!!
                                    die Felder stz und nk der Struktur
                                    sind input, nur packed ist output */
)
{
  short         rc;
  T_PCK         pck;
  unsigned char packed [2];

  packed [0] = 0x10;
  packed [1] = 0x0c;

  pck.packed = packed;
  pck.stz = 3;
  pck.nk = 0;

  rc = pck_mul (pck1, &pck, pck3);
  if (rc)
    return(rc);

  rc = pck_div (pck3, pck2, pck3);
  if (rc)
    return(rc);

  rc = pck_sub (pck3, &pck, pck3);

  return rc;
}

/* ------------------------------------------------------------------ */
/*       Konvertierung ungepackt -> gepackt                           */
/* ------------------------------------------------------------------ */


short pck_from_unpck (
  T_PCK         *pck,          /* o: gepackte Zahl (stz ist input) */
  const T_UNPCK *unpck         /* i: ungepackte Zahl */
)
{
  unsigned char *p1;
  unsigned char *p2;
  short          l1;
  short          l2;
  short          len;
  short          rc;
  unsigned char  byte;
  T_PCK          pcktemp;
  unsigned char  packedtemp [PACKED_N];

  pcktemp.packed = packedtemp;
  pcktemp.nk     = unpck->nk;
  pcktemp.stz    = unpck->stz;

  p1 = pcktemp.packed;
  l1 = (pcktemp.stz >> 1) + 1;
  p2 = unpck->unpacked;
  l2 = unpck->stz;

  ipc_init ();

  -- l1;
  -- l2;

  /*
   * Verarbeitung von rechts nach links, p1 und p2 auf das letzte  Byte
   * einstellen.
   */

  p1 += l1;
  p2 += l2;

  /*
   * Vorzeichenstelle verarbeiten
   */

  *p1 -- = ip_pack_tab [(unsigned char)(*p2 --)];

  /*
   * len = Anzahl der Bytes berechnen, die von p2 nach p1 gepackt
   * werden, d.h. 2 * len Bytes werden von p2 gelesen und len Bytes
   * nach p1 gepackt. len = min(l1, l2/2);
   */

  len = l1;
  if ((l2 >> 1) < (short) len)
    len = (l2 >> 1);

  /*
   * Restlaengen berechnen
   */

  l1 -= len;
  l2 -= (len << 1);

  /*
   * Bytes paarig uebertragen
   */

  while (len)
  {
    byte = *p2 -- & 0x0f;
    *p1 -- = byte | ((*p2 -- & 0x0f) << 4);
    -- len;
  }

  /*
   * Rest auffuellen
   */

  if (l1 > 0)      /* falls nicht fertig */
  {
    if (l2 > 0)    /* falls in p2 noch eine Ziffer uebrig ist */
    {
      *p1 -- = *p2 -- & 0x0f;
      -- l1;
    }

    while (l1 -- > 0)  /* Rest mit 0 auffuellen */
      *p1 -- = 0;
  }

  rc = pck_to_pck (&pcktemp, pck);

  return rc;

}


/* ------------------------------------------------------------------ */
/*    Konvertierung gepackt -> ungepackt                              */
/* ------------------------------------------------------------------ */

short pck_to_unpck (
  const T_PCK   *pck,     /* i: gepackte Zahl */
  T_UNPCK       *unpck    /* o: ungepackte Zahl (stz und nk sind input) */
)
{
  unsigned char *p1;
  unsigned char *p2;
  short          l1;
  short          l2;
  short          len;
  short          rc;
  T_PCK          pcktemp;
  unsigned char  packedtemp [PACKED_N];

  pcktemp.packed = packedtemp;
  pcktemp.nk     = unpck->nk;
  pcktemp.stz    = unpck->stz;

  rc = pck_to_pck (pck, &pcktemp);
  if (rc)
    return rc;

  p1 = unpck->unpacked;
  l1 = unpck->stz;
  p2 = pcktemp.packed;
  l2 = (pcktemp.stz >> 1) + 1;

  ipc_init();

  -- l1;  /* Laengen auf Host-Format bringen (um 1 zu kurz) */
  -- l2;

  /*
   * Verarbeitung von rechts nach links, p1 und p2 auf das letzte  Byte
   * einstellen.
   */

  p1 += l1;
  p2 += l2;

  /*
   * Vorzeichenstelle verarbeiten
   */

  *p1 -- = ip_unpk_tab [*p2 --];

  /*
   * len = Anzahl der Bytes berechnen, die von p2 nach p1 geUNPKt
   * werden, d.h. len Bytes werden von p2 gelesen und 2 * len Bytes
   * nach p1 geschrieben. len = min(l1 / 2, l2);
   */

  len = l2;
  if ((l1 >> 1) < (short) len)
    len = (l1 >> 1);

  /*
   * Restlaengen berechnen
   */

  l1 -= (len << 1);
  l2 -= len;

  /*
   * Bytes paarig uebertragen
   */

  while (len --)
  {
    *p1 -- = '0' + (*p2 & 0x0f);
    *p1 -- =  '0' + (*p2 -- >> 4);
  }

  /*
   * evtl eine Ziffer einzeln uebertragen
   */

  if (l1 && l2)
  {
    *p1 -- = '0' + (*p2 & 0x0f);
    -- l1;
  }

  /*
   * Rest mit null auffuellen
   */

  while (l1 -- > 0)
    *p1 -- = '0';

  return( 0 )
}

/* ------------------------------------------------------------------ */
/*    Konvertierung gepackt -> gepackt                                */
/* ------------------------------------------------------------------ */

short pck_to_pck
(
  const T_PCK *pck1, /* i : gepackte Zahl */
  T_PCK       *pck2  /* o : gepackte Zahl ( stz und nk sind input) */
)
{
  unsigned char *res;
  unsigned char *op;
  short          op_len;
  short          res_len;
  short          nk_diff;
  unsigned char *p1;
  unsigned char *p2;

  op = pck1->packed;
  res = pck2->packed;
  op_len  = pck1->stz;
  res_len = pck2->stz;
  nk_diff = pck2->nk - pck1->nk;

  memset (res, 0, (size_t) res_len/2);
 /* Ergebnis mit 0'llen initialisieren */
  *(res + res_len/2) = (*(op + op_len/2) & 0x0f);

  if (nk_diff < 0)        /* op rechts abschneiden */
    op_len  += nk_diff;
  else                    /* res rechts von op mit 0'llen auffuellen */
    res_len -= nk_diff;

  if (nk_diff % 2 == 0)  /* copy by byte */
  {
    *(res + res_len/2) |= *(op + op_len/2) & 0xf0;
    p1 = res + res_len/2;
    p2 = op + op_len/2;
    while (p1 > res && p2 > op)
      *(--p1) = *(--p2);
  }
  else                  /* copy by nibble */
  {
    if (nk_diff < 0)
    {
      p1 = res + res_len/2 - 1;
      p2 = op  + (op_len - 1)/2;
      *(res + res_len/2) |= (*p2 << 4);
    }
    else
    {
      p1 = res + (res_len - 1)/2;
      p2 = op + op_len/2;
    }
    while (p1 >= res && p2 > op)
    {
      *p1 = (*p2 >> 4) | (*(p2-1) << 4);
      --p1; --p2;
    }
    if (p1 >= res && p2 == op)
      *p1 = *op >> 4;
  }
  if (pck2->stz % 2 == 0)  /* high nib des ersten bytes unbenutzt */
    *res &= 0x0f;
  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     gepackt -> abs (gepackt)  - Absolutbetrag -                    */
/* ------------------------------------------------------------------ */

short pck_abs (
  const T_PCK *pck1,    /* i : input    */
  T_PCK       *pck2     /* o : output   */
)
{
  short len;

  pck2->stz = pck1->stz;
  pck2->nk = pck1->nk;
  len = (pck1->stz >> 1) + 1;
  memcpy (pck2->packed, pck1->packed, len);
  pck2->packed [len - 1] &= 0xf0;
  pck2->packed [len - 1] |= IPPL;

  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     gepackt -> - gepackt  - Vorzeichenwechsel  -                   */
/* ------------------------------------------------------------------ */

short pck_neg (
  const T_PCK *pck1,    /* i : input    */
  T_PCK       *pck2     /* o : output   */
)
{
  short len;
  unsigned char x;

  pck2->stz = pck1->stz;
  pck2->nk = pck1->nk;
  len = (pck1->stz >> 1) + 1;
  memcpy (pck2->packed, pck1->packed, len);
  x = ip_translate_digit [pck2->packed [len - 1] & 0x0f];

  if (x == IPPL)
  {
    pck2->packed [len - 1] &= 0xf0;
    pck2->packed [len - 1] |= IPMI;
  }
  else if (x == IPMI)
  {
    pck2->packed [len - 1] &= 0xf0;
    pck2->packed [len - 1] |= IPPL;
  }
  else
    return ERROR_NO_SIG;

  return( 0 )

}

/* ------------------------------------------------------------------ */
/*     gepackt -> gepackt ohne Vorzeichen                             */
/* ------------------------------------------------------------------ */

short pck_to_pckov (
  const T_PCK   *pck,     /* i : gepackte Zahl */
  T_PCKOV       *pckov    /* o : gepackte Zahl ohne Vorzeichen,
                                 stz und nk sind input */
)
{
  short          l1;
  short          l2;
  short          rc;
  unsigned char  hbl;
  unsigned char  hbr;
  T_PCK          pcktemp;
  unsigned char  packedtemp [PACKED_N];

  pcktemp.packed = packedtemp;
  pcktemp.nk     = pckov->nk;
  pcktemp.stz    = pckov->stz;

  rc = pck_to_pck (pck, &pcktemp);
  if (rc)
    return rc;

  l1 = (pcktemp.stz >> 1);
  l2 = (pcktemp.stz >> 1) + (pcktemp.stz % 2) - 1;

  while (l1 >= 0)
  {
    if (l1 > 0)
      hbl = (pcktemp.packed [l1 - 1] & 0x0f) << 4;
    else
      hbl = 0;

    hbr = (pcktemp.packed [l1 ] & 0xf0) >> 4;

    if (l2 >= 0)
      pckov->packedov [l2 --] = hbl | hbr;
    l1 --;
  }

  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     gepackt (ohne Vorzeichen) -> gepackt                           */
/* ------------------------------------------------------------------ */

short pck_from_pckov (
  T_PCK         *pck,    /* o : gepackte Zahl, stz und nk sind input */
  const T_PCKOV *pckov   /* i : gepackte Zahl ohne Vorzeichen */
)
{
  short          l1;
  short          l2;
  short          rc;
  unsigned char  hbl;
  unsigned char  hbr;
  T_PCK          pcktemp;
  unsigned char  packedtemp [PACKED_N];

  pcktemp.packed = packedtemp;
  pcktemp.nk     = pckov->nk;
  pcktemp.stz    = pckov->stz;

  l1 = (pckov->stz >> 1) + (pckov->stz % 2) - 1;
  l2 = (pckov->stz >> 1);

  hbr = IPPL;

  while (l1 >= 0)
  {
    hbl = (pckov->packedov [l1] & 0x0f) << 4;

    pcktemp.packed [l2 --] = hbl | hbr;
    hbr = (pckov->packedov [l1 --] & 0xf0) >> 4;
  }
  if (l2 >= 0)
    pcktemp.packed [l2] = hbr;


  rc = pck_to_pck (&pcktemp, pck);

  return rc;

}

/* ------------------------------------------------------------------ */
/*     gepackt  -> fux                                                */
/* ------------------------------------------------------------------ */

short  pck_to_fux (
  const T_PCK *pck,       /* i : gepackte Zahl */
  T_FUX       *fux        /* o : fux-Zahl */
)
{
  tDec  decari;
  unsigned char  packed [PACKED_N];
                              /* max. 31 Stellen + '.' + '-' + '\0' */
  char           temp [2 * PACKED_N + 2];
  short  rc;
  short  l;
  short  i;
  short  stz;
  char  *p;

           /* + Vorzeichen */
  stz = fux->stz + 1;
           /* + Dezimalpunkt */
  if (fux->nk > 0)
    stz ++;

  pck_to_pck16 (packed, pck);

  rc = packed_to_decari (&decari, packed, pck->nk);
  if (rc)
    return rc;

  decari_to_string( temp, sizeof( temp ), &decari, fux->nk, ' ');

  if (fux->nk > 0)
  {
    p = strchr (temp, '.');
    l = strlen (p);
    for (i = l; i <= fux->nk; i++)
      p [i] = '0';
    p [fux->nk + 1] = '\0';
  }

  l = strlen (temp);
  if (l > stz)
  {
    memcpy (fux->str, temp, stz);
    return ERROR_NO_SPACE;
  }

  memset (fux->str, ' ', stz);
  memcpy (fux->str + stz - l, temp, l);

  return( 0 )
}


/* ------------------------------------------------------------------ */
/*     fux  -> gepackt                                                */
/* ------------------------------------------------------------------ */

short  pck_from_fux (
  T_PCK       *pck,       /* o : gepackte Zahl */
  const T_FUX *fux        /* i : fux-Zahl */
)
{
  short          rc;
  tDec           decari;
  unsigned char  packed [PACKED_N];
                              /* max. 31 Stellen + '.' + '-' + '\0' */
  char           temp [2 * PACKED_N + 2];
  short          stz;

           /* + Vorzeichen */
  stz = fux->stz + 1;
           /* + Dezimalpunkt */
  if (fux->nk > 0)
    stz ++;

  memcpy (temp, fux->str, stz);
  temp [stz] = '\0';
  rc = decari_from_string (&decari, temp);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_to_packed (&decari, packed, pck->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (packed, pck);

  return( 0 )

}


/* ------------------------------------------------------------------ */
/*     decari  -> double                                              */
/* ------------------------------------------------------------------ */

short decari_to_double
(
  const tDec *pd,          /* i : Zahl im decari-Format */
  double     *result       /* o : double                */
)
{
  short  i;
  double s;
  char   exp;

  s = 0.0;

  for (i = DECARI_M - 1; i >= 0; i --)
  {
    s = s * 0.01 + pd->mant [i];
  }

  exp = pd->expo;

  if ((exp & 0x80) == 0x80)
  {
    exp &= 0x7f;
    s = -s;
  }

  if (exp)
  {
    s = s * (double) arPowP10 (2 * (exp - 0x40));
  }
  *result = s;
  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     double -> decari                                               */
/* ------------------------------------------------------------------ */

short decari_from_double
(
  tDec       *pd,          /* o : Zahl im decari Format */
  double      value        /* i : double                */
)
{
  char *pc;
  char  temp [256];

  memset (temp, ' ', 256);
  sprintf (temp, "%22.15e", value);
  if ( (pc = strchr (temp, ',')) != NULL)
    *pc = '.';
  decari_from_string (pd, temp) ;
  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     decari -> long                                                 */
/* ------------------------------------------------------------------ */

short decari_to_long
(
  const tDec *pd,          /* i : Zahl im decari-Format */
  long       *result,      /* o : long                  */
  short       nks          /* i : Nachkommastellen      */
)
{
  short           rc;
  unsigned char   buffer [PACKED_N];
  T_PCK           pck;
  long            lo;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = nks;

  rc = decari_to_packed (pd, buffer, nks);
  if (rc)
    return(rc);

  rc = pck_to_long (&pck, &lo, nks);
  if (rc)
    return rc;

  *result = lo;

  return( 0 )

}

/* ------------------------------------------------------------------ */
/*     long -> decari                                                 */
/* ------------------------------------------------------------------ */

short decari_from_long
(
  tDec       *pd,         /* i : Zahl im decari-Format */
  const long  value,      /* o : long                  */
  short       nks         /* i : Nachkommastellen      */
)
{
  short          rc;
  unsigned char  buffer [PACKED_N];
  T_PCK          pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = nks;

  rc = pck_from_long (&pck, value, nks);
  if (rc)
    return rc;

  rc = packed_to_decari (pd, buffer, nks);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     decari -> unsigned long                                        */
/* ------------------------------------------------------------------ */

short decari_to_ulong
(
  const tDec          *pd,          /* i : Zahl im decari-Format */
  unsigned long       *result,      /* o : unsigned long         */
  short                nks          /* i : Nachkommastellen      */
)
{
  short           rc;
  unsigned char   buffer [PACKED_N];
  T_PCK           pck;
  unsigned long   ulo;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = nks;

  rc = decari_to_packed ((tDec *) pd, buffer, nks);
  if (rc)
    return(rc);

  rc = pck_to_ulong (&pck, &ulo, nks);
  if (rc)
    return rc;

  *result = ulo;

  return( 0 )

}

/* ------------------------------------------------------------------ */
/*     unsigned long -> decari                                        */
/* ------------------------------------------------------------------ */

short decari_from_ulong
(
  tDec                *pd,         /* i : Zahl im decari-Format */
  const unsigned long  value,      /* o : unsigned long         */
  short                nks         /* i : Nachkommastellen      */
)
{
  short          rc;
  unsigned char  buffer [PACKED_N];
  T_PCK          pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = nks;

  rc = pck_from_ulong (&pck, value, nks);
  if (rc)
    return rc;

  rc = packed_to_decari (pd, buffer, nks);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     decari -> packed                                               */
/* ------------------------------------------------------------------ */

short decari_to_pck
(
  const tDec    *pd,     /* i : Zahl im decari-Format */
  T_PCK         *pck     /* o : gepackte Zahl         */
)
{
  short rc;
  unsigned char buffer [PACKED_N];

  rc = decari_to_packed ((tDec *) pd, buffer, pck->nk);
  if (rc)
    return rc;

  rc = pck_from_pck16 (buffer, pck);

  return rc;

}

/* ------------------------------------------------------------------ */
/*     packed -> decari                                               */
/* ------------------------------------------------------------------ */

short decari_from_pck
(
  tDec          *pd,     /* o : Zahl im decari-Format */
  const T_PCK   *pck     /* i : gepackte Zahl         */
)
{
  short         rc;
  unsigned char buffer [PACKED_N];

  pck_to_pck16 (buffer, pck);
  rc = packed_to_decari (pd, buffer, pck->nk);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     decari -> ungepackt                                            */
/* ------------------------------------------------------------------ */

short decari_to_unpck
(
  const tDec    *pd,       /* i : Zahl im decari-Format */
  T_UNPCK       *unpck     /* o : ungepackte Zahl       */
)
{
  short           rc;
  unsigned char   buffer [PACKED_N];
  T_PCK           pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = unpck->nk;

  rc = decari_to_packed ((tDec *) pd, buffer, unpck->nk);
  if (rc)
    return(rc);

  rc = pck_to_unpck (&pck, unpck);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     ungepackt -> decari                                            */
/* ------------------------------------------------------------------ */

short decari_from_unpck
(
  tDec           *pd,     /* o : Zahl im decari-Format */
  const T_UNPCK  *unpck   /* i : ungepackte Zahl       */
)
{
  short          rc;
  unsigned char  buffer [PACKED_N];
  T_PCK          pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = unpck->nk;

  rc = pck_from_unpck (&pck, unpck);
  if (rc)
    return rc;

  rc = packed_to_decari (pd, buffer, pck.nk);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     decari -> gepackt (ohne Vorzeichen)                            */
/* ------------------------------------------------------------------ */

short decari_to_pckov
(
  const tDec    *pd,          /* i : Zahl im decari-Format */
  T_PCKOV       *pckov        /* o : gepackte Zahl ohne Vorzeichen */
)
{
  short           rc;
  unsigned char   buffer [PACKED_N];
  T_PCK           pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = pckov->nk;

  rc = decari_to_packed ((tDec *) pd, buffer, pck.nk);
  if (rc)
    return(rc);

  rc = pck_to_pckov (&pck, pckov);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     gepackt (ohne Vorzeichen) -> decari                            */
/* ------------------------------------------------------------------ */

short decari_from_pckov
(
  tDec          *pd,        /* o : Zahl im Decari-Format */
  const T_PCKOV *pckov      /* i : gepackte Zahl ohne Vorzeichen */
)
{
  short          rc;
  unsigned char  buffer [PACKED_N];
  T_PCK          pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = pckov->nk;

  rc = pck_from_pckov (&pck, pckov);
  if (rc)
    return rc;

  rc = packed_to_decari (pd, buffer, pck.nk);

  return rc;
}


/* ------------------------------------------------------------------ */
/*     decari -> fux                                                  */
/* ------------------------------------------------------------------ */

short decari_to_fux
(
  const tDec    *pd,       /* i : Decari-Zahl */
  T_FUX         *fux       /* o : fux-Zahl    */
)
{
                              /* max. 31 Stellen + '.' + '-' + '\0' */
  char           temp [2 * PACKED_N + 2];
  short  l;
  short  i;
  short  stz;
  char  *p;

           /* + Vorzeichen */
  stz = fux->stz + 1;
           /* + Dezimalpunkt */
  if (fux->nk > 0)
    stz ++;

  decari_to_string( temp, sizeof( temp ), (tDec *)pd, fux->nk, ' ');

  if (fux->nk > 0)
  {
    p = strchr (temp, '.');
    l = strlen (p);
    for (i = l; i <= fux->nk; i++)
      p [i] = '0';
    p [fux->nk + 1] = '\0';
  }

  l = strlen (temp);
  if (l > stz)
  {
    memcpy (fux->str, temp, stz);
    return ERROR_NO_SPACE;
  }

  memset (fux->str, ' ', stz);
  memcpy (fux->str + stz - l, temp, l);

  return( 0 )

}

/* ------------------------------------------------------------------ */
/*     fux -> decari                                                  */
/* ------------------------------------------------------------------ */

short decari_from_fux
(
  tDec          *pd,       /* o : Zahl im Decari-Format */
  const T_FUX   *fux       /* i : fux-Zahl              */
)
{
  short          rc;
  unsigned char  buffer [PACKED_N];
  T_PCK          pck;

  pck.packed = buffer;
  pck.stz = PACKED_N*2-1;
  pck.nk = fux->nk;

  rc = pck_from_fux (&pck, fux);
  if (rc)
    return rc;

  rc = packed_to_decari (pd, buffer, pck.nk);

  return rc;
}

/* ------------------------------------------------------------------ */
/*     decari -> dlong (dword)                                        */
/* ------------------------------------------------------------------ */

short decari_to_dlong
(
  const    tDec *pd,          /* i : Zahl im decari-Format */
  unsigned char *dword,       /* o : dword (zwei longs)    */
  short          nks          /* i : Nachkommastellen      */
)
{
  unsigned long hi;
  unsigned long lo;
  short         neg;
  short         rc;
  tDec          dec_fak;
  tDec          dec_temp1;
  tDec          dec_temp2;
  tDec          dec_temp3;
  tDec          dec_temp4;
  tDec          dec_temp5;
  tDec          dec_temp6;

  long          fak = 1;

        /* Vorkommastellen beruecksichtigen */
  if (nks != 0)
  {
    rc = decari_from_long (&dec_fak, fak, nks);
    if (rc)
      return rc;
    rc = decari_div (pd, &dec_fak, &dec_temp1);
      if ((rc != 0) && (rc != -1))  /* Underflow */
        return rc;
  }
  else
  {
    dec_temp1 = *pd;
  }

         /* zahl < 0, also 1. bit im expo gesetzt */
  if (dec_temp1.expo & 0x80)
  {
         /* zahl = -zahl, also 1. bit im expo ausknipsen */
    dec_temp1.expo &= 0x7f;
    neg = 1;
  }
  else
    neg = 0;

  rc = decari_floor (&dec_temp1, &dec_temp2);
  if (rc)
    return rc;

  rc = decari_div (&dec_temp2, &decari_2exp32, &dec_temp3);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

              /* dec_temp4 ist ganzzahliger Anteil */
  rc = decari_floor (&dec_temp3, &dec_temp4);
  if (rc)
    return rc;
              /* dec_temp5 ist Nachkomma-Anteil */
  rc = decari_sub (&dec_temp3, &dec_temp4, &dec_temp5);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  rc = decari_mult (&dec_temp5, &decari_2exp32, &dec_temp6);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  decari_round (&dec_temp4, 0);
  decari_round (&dec_temp6, 0);

  rc = decari_to_ulong (&dec_temp4, &hi, 0);
  if (rc)
    return rc;

  rc = decari_to_ulong (&dec_temp6, &lo, 0);
  if (rc)
    return rc;

  if (neg)
  {
    hi = ~hi;
    if (lo == 0)
      ++ hi;
    else
      lo = ~lo + 1;
  }

#ifdef TB_SWAP
  *((unsigned long *) dword + 1) = hi;
  *(unsigned long *) dword = lo;
#else
  *(unsigned long *) dword = hi;
  *((unsigned long *) dword + 1) = lo;
#endif

  return( 0 )
}


/* ------------------------------------------------------------------ */
/*     dlong (dword) -> decari                                        */
/* ------------------------------------------------------------------ */

short decari_from_dlong
(
  tDec                *pd,        /* o : Zahl im decari-Format */
  const unsigned char *dword,     /* i : dword (zwei longs)    */
  short                nks        /* i : Nachkommastellen      */
)
{

  long          sign;
  long          hi;
  unsigned long lo;
  short         rc;
  tDec          dec_sign;
  tDec          dec_hi;
  tDec          dec_lo;
  tDec          dec_fak;
  tDec          dec_temp1;
  tDec          dec_temp2;

  long          fak = 1;

#ifdef TB_SWAP
  memcpy (&hi, dword + sizeof (zLONG), sizeof (zLONG));
  memcpy (&lo, dword, sizeof (unsigned long));
#else
  memcpy (&hi, dword, sizeof (zLONG));
  memcpy (&lo, dword + sizeof (unsigned long), sizeof (unsigned long));
#endif

  if (hi < 0)
  {
    hi = ~hi;
    if (lo == 0L)
      ++ hi;
    else
      lo = ~lo + 1;
    sign = -1;
  }
  else
    sign = 1;

  /* alles nach decari */

  rc = decari_from_long (&dec_sign, sign, 0);
  if (rc)
    return rc;
  rc = decari_from_long (&dec_hi, hi, 0);
  if (rc)
    return rc;
  rc = decari_from_ulong (&dec_lo, lo, 0);
  if (rc)
    return rc;
  rc = decari_from_long (&dec_fak, fak, nks);
  if (rc)
    return rc;

  /* hi * 4294967296 */
  rc = decari_mult (&dec_hi, &decari_2exp32, &dec_temp1);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  /* hi * 4294967296 + lo */
  rc = decari_add (&dec_temp1, &dec_lo, &dec_temp2);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  /* Nachkommastellen beruecksichtigen */
  rc = decari_mult (&dec_temp2, &dec_fak, &dec_temp1);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  /* sign beruecksichtigen */
  rc = decari_mult (&dec_sign, &dec_temp1, pd);
  if ((rc != 0) && (rc != -1))  /* Underflow */
    return rc;

  return( 0 )
}

/* ------------------------------------------------------------------ */
/*     packed -> dlong (dword)                                        */
/* ------------------------------------------------------------------ */

short  pck_to_dlong
(
  const T_PCK     *pck,        /* i : gepackte Zahl      */
  unsigned char   *dword,      /* o : dword (zwei longs) */
  unsigned short   nks         /* i : Nachkommastellen   */
)
{
  tDec  decari;
  short rc;

  rc = decari_from_pck (&decari, pck);
  if (rc)
    return rc;

  rc = decari_to_dlong (&decari, dword, nks);

  return(rc);
}

/* ------------------------------------------------------------------ */
/*     dlong (dword) -> packed                                        */
/* ------------------------------------------------------------------ */

short  pck_from_dlong    (T_PCK                 *pck,
                           const unsigned char   *dword,
                           unsigned short         nks)
{
  short rc;
  tDec  decari;

  rc = decari_from_dlong (&decari, dword, nks);
  if (rc)
    return rc;

  rc = decari_to_pck (&decari, pck);
  return rc;
}

/* ------------------------------------------------------------------ */
/*     floor (decari)  --- groesste ganze zahl <= decari              */
/* ------------------------------------------------------------------ */

short  decari_floor
(
  tDec *pd1,               /* i : Zahl im decari-Format */
  tDec *pd2                /* o : floor (input)         */
)
{

  short rc;

  *pd2 = *pd1;

  decari_int (pd2);
  if (decari_compare (pd2, pd1) > 0)
  {
    rc = decari_add (pd2, &decari_m1, pd2);
    if ((rc != 0) && (rc != -1))  /* Underflow */
      return rc;
  }
  return( 0 )
}

