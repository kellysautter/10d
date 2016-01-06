/*
** MODULE NAME: hzapck2.c
**----------------------------------------------------------------------
**   helper module for hzapck.c
**   This module is only necessary when a application using hzapck.c
**   is linked without any siron objects
**   A normal Zeidon server program smust not link this file
**----------------------------------------------------------------------
**
** CHANGE LOG most recent first order
** ====== ===
**
** 1999.09.10  HH  ALL
**    include rest.h -> hzapck2.h
*** 1999.23.08  US  ALL
**    existing module put into Source Control System
**
**
*/

#include "hgenva.h"
#include "hnmaca.h"
#include "hsipca.h"
#include "hzapck2.h"

/* -------------------- aus hzsipc.c ----------------------- */

/*
 * Vorzeichenstelle von gepackten Zahlen, UNPK erzeugt Zeichen
 * aus def_str,
 * PACK versteht Zeichen aus def_str und alt_str.
 */

#ifdef TB_ASCII

/* str[i] entspricht:      0 ...... 90 ......-9 */
static const CHAR def_str[21] =
                    "0123456789pqrstuvwxy";  /* default */
static const CHAR alt_str[21] =
                    "{ABCDEFGHI}JKLMNOPQR";  /* alternativ */

#else

/* str[i] entspricht:      0 ...... 90 ......-9 */
static const CHAR def_str[21] =
                    "0123456789}JKLMNOPQR";  /* default */
static const CHAR alt_str[21] =
                    "{ABCDEFGHI}JKLMNOPQR";  /* alternativ */

#endif


/*
 * SHORT ip_unpk_vorzeichen(UCHAR ch)
 * ermittelt Typ der Vorzeichenstelle einer ungepackten Zahl. Liefert:
 *  + 2 : wenn ch in alt_str[0..9]
 *  + 1 : wenn ch in def_str[0..9]
 *  - 1 : wenn ch in def_str[10..19]
 *  - 2 : wenn ch in alt_str[10..19]
 *    0 : wenn ch keine gueltige Vorzeichenstelle ist
 */

SHORT ENTRY_FUNC ip_unpk_vorzeichen(UCHAR ch)
{
  CHAR *p;

  if ( ch == (UCHAR)0 )
    return( 0 )

  p = strrchr(def_str, ch);
  if (p != NULL)
  {
    if (p - def_str < 10)
      return( 1 )
    return( -1 )
  }

  p = strrchr(alt_str, ch);
  if (p != NULL)
  {
    if (p - alt_str < 10)
      return 2;
    return -2;
  }
  return( 0 )
}

/* pack_tab: Index = Vorzeichen-Byte eine ungepackten Zahl,
liefert Vorzeichen-Byte der gepackten Zahl. */

UCHAR *ip_pack_tab;


/* unpk_tab: Index = Vorzeichen-Byte einer gepackten  Zahl,
liefert Vorzeichen-Byte einer ungepackten Zahl */

UCHAR *ip_unpk_tab;


/* Speicher fuer Tabellen */
static UCHAR unpk_mem[256];
static UCHAR pack_mem[256];


VOID ipc_init(VOID)
{
  static SHORT initialized;
  SHORT i;

#ifndef TB_IBM
  if (initialized)
    return;
#endif

  initialized = TRUE;

  ip_pack_tab = pack_mem;
  ip_unpk_tab = unpk_mem;
  /*
   * default: hi- mit lo-Nibble vertauschen
   */

  for (i = 0; i < 256; i++)
  {
    ip_pack_tab[i] = ((i & 0xf) << 4) | ((i >> 4) & 0xf);
    ip_unpk_tab[i] = ip_pack_tab[i];
  }

#ifndef EBCDIC_ON_ASCII_MACHINE

  for (i = 0; i < 10; i++)
  {
    ip_pack_tab[def_str[i]] = (i << 4) + 0x0c;
    ip_pack_tab[alt_str[i]] = (i << 4) + 0x0c;
  }
  for (i = 0; i < 10; i++)
  {
    ip_pack_tab[def_str[i + 10]] = (i << 4) + 0x0d;
    ip_pack_tab[alt_str[i + 10]] = (i << 4) + 0x0d;
  }


  for (i = 0; i < 10; i++)
  {
    ip_unpk_tab[(i << 4) + 0x0a] = def_str[i];
    ip_unpk_tab[(i << 4) + 0x0c] = def_str[i];
    ip_unpk_tab[(i << 4) + 0x0e] = def_str[i];
    ip_unpk_tab[(i << 4) + 0x0f] = def_str[i];
  }
  for (i = 0; i < 10; i++)
  {
    ip_unpk_tab[(i << 4) + 0x0b] = def_str[i + 10];
    ip_unpk_tab[(i << 4) + 0x0d] = def_str[i + 10];
  }

#endif /* EBCDIC_ON_ASCII_MACHINE */

}


/* ------------- aus hzsipc.c ----------------------------------- */

UCHAR ip_translate_digit[16] =
  /* 0  1  2  3  4  5  6  7  8  9   a     b     c     d     e     f  */
   { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, IPPL, IPMI, IPPL, IPMI, IPPL, IPPL };


/* aus hzaty1.c */

const tNumber arPow10Array[arPow10Entries] =
{
 1e-32, 1e-31,  1e-30,  1e-29,  1e-28,  1e-27,  1e-26,  1e-25,
 1e-24, 1e-23,  1e-22,  1e-21,  1e-20,  1e-19,  1e-18,  1e-17,
 1e-16, 1e-15,  1e-14,  1e-13,  1e-12,  1e-11,  1e-10,  1e-9,
 1e-8,  1e-7,   1e-6,   1e-5,   1e-4,   1e-3,   1e-2,   1e-1,
 1e0,
 1e1,   1e2,    1e3,    1e4,    1e5,    1e6,    1e7,    1e8,
 1e9,   1e10,   1e11,   1e12,   1e13,   1e14,   1e15,   1e16,
 1e17,  1e18,   1e19,   1e20,   1e21,   1e22,   1e23,   1e24,
 1e25,  1e26,   1e27,   1e28,   1e29,   1e30,   1e31,   1e32
};
