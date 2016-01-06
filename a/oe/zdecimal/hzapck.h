/*
** MODULE NAME: hzapck.h
**----------------------------------------------------------------------
**   Include file for using the module hzapck.c
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

#ifndef __HZAPCK_H__4
#define __HZAPCK_H__

#include "hnmaca.h"
#include "decari.h"


/* ErrorCodes */

#define ERROR_NO_PACKED 1            /* Stelle in gepackter Zahl > '9' */
#define ERROR_NO_SPACE  2            /* Zahl passt nicht ins Ziel */
#define ERROR_SIG       3            /* negatives Vorzeichen und unsigned */
#define ERROR_NO_SIG    4            /* Vorzeichen ungueltig */

#define ERROR_MYSTIFY   999          /* ??? */

/* Epsilon fuer Rundung double -> gepackt */

#define EPSILON_CUT           0.005
#define EPSILON_LIGHT_ROUND   0.1
#define EPSILON_MEDIUM_ROUND  0.25
#define EPSILON_ROUND         0.5

#define EPSILON EPSILON_ROUND

#define FB3_MAX    16777215          /* so etwas fehlt in limits.h */

/* Types */

/* gepackt */

/* gepackte Zahl (mit Vorzeichen) */

typedef struct s_pck
{
   unsigned char  *packed;         /* Adresse der gepackten Zahl */
   unsigned short  stz;            /* Stellenanzahl */
   unsigned short  nk;             /* Anzahl der Nachkommastellen */
} T_PCK ;

/* ungepackte Zahl */

typedef struct s_unpck
{
   unsigned char  *unpacked;       /* Adresse der ungepackten Zahl */
   unsigned short  stz;            /* Stellenanzahl */
   unsigned short  nk;             /* Anzahl der Nachkommastellen */
} T_UNPCK ;

/* gepackte Zahl (ohne Vorzeichen) */

typedef struct s_pckov
{
   unsigned char  *packedov;       /* Adresse der gepackten Zahl */
   unsigned short  stz;            /* Stellenanzahl */
   unsigned short  nk;             /* Anzahl der Nachkommastellen */
} T_PCKOV;

/* gepackte Zahl (ohne Vorzeichen) */

typedef struct s_fux
{
   char           *str;            /* Adresse, wo der Fuchs wohnt */
   unsigned short  stz;            /* Stellenanzahl ggf. mit Komma und
                                      Vorzeichen */
   unsigned short  nk;             /* Anzahl der Nachkommastellen */
} T_FUX;

/* Prototypes */



short pck_to_lzahl        (const T_PCK     *pck,
                           long            *lzahl,
                           unsigned short   nk,
                           long             max);

short pck_from_lzahl      (T_PCK           *pck,
                           long             lzahl,
                           unsigned short   nk);

short pck_to_long         (const T_PCK     *pck,
                           long            *lo,
                           unsigned short   nk);

short pck_from_long       (T_PCK           *pck,
                           long             lo,
                           unsigned short   nk);

short pck_to_short        (const T_PCK     *pck,
                           short           *sh,
                           unsigned short   nk);

short pck_from_short      (T_PCK           *pck,
                           short            sh,
                           unsigned short   nk);

short pck_to_char         (const T_PCK     *pck,
                           char            *ch,
                           unsigned short   nk);

short pck_from_char       (T_PCK           *pck,
                           char             ch,
                           unsigned short   nk);

short pck_to_uzahl        (const T_PCK     *pck,
                           unsigned long   *uzahl,
                           unsigned short   nk,
                           unsigned long    max);

short pck_from_uzahl      (T_PCK           *pck,
                           unsigned long    uzahl,
                           unsigned short   nk);

short pck_to_ulong        (const T_PCK     *pck,
                           unsigned long   *ulo,
                           unsigned short   nk);

short pck_from_ulong      (T_PCK           *pck,
                           unsigned long    ulo,
                           unsigned short   nk);

short pck_to_ushort       (const T_PCK     *pck,
                           unsigned short  *ush,
                           unsigned short   nk);

short pck_from_ushort     (T_PCK           *pck,
                           unsigned short   ush,
                           unsigned short   nk);

short pck_to_uchar        (const T_PCK     *pck,
                           unsigned char   *uch,
                           unsigned short   nk);

short pck_from_uchar      (T_PCK           *pck,
                           unsigned char    uch,
                           unsigned short   nk);

short pck_to_fb3          (const T_PCK     *pck,
                           unsigned char   *fb3,
                           unsigned short   nk);

short pck_from_fb3        (T_PCK           *pck,
                           unsigned char   *fb3,
                           unsigned short   nk);

short pck_to_double       (const T_PCK     *pck,
                           double          *doub);

short  pck_from_double    (T_PCK           *pck,
                           double           doub,
                           double           cut_or_round);

short  pck_to_float       (const T_PCK     *pck,
                           float           *flo);

short  pck_from_float     (T_PCK           *pck,
                           float            flo,
                           float            cut_or_round);

short  pck_compare        (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           short           *result);

short  pck_min            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *min);

short  pck_max            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *max);

short  pck_add            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

short  pck_sub            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

short  pck_mul            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

short  pck_div            (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

short  pck_prozent        (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

short  pck_prozentm       (const T_PCK     *pck1,
                           const T_PCK     *pck2,
                           T_PCK           *pck3);

void   pck_to_pck16       (unsigned char  *packed,
                           const T_PCK    *pck);

short  pck_from_pck16     (const unsigned char  *packed,
                           T_PCK                *pck);

short  pck_to_unpck       (const T_PCK    *pck,
                           T_UNPCK        *unpck);

short  pck_from_unpck     (T_PCK          *pck,
                           const T_UNPCK  *unpck);

short  pck_to_pck         (const T_PCK    *pck1,
                           T_PCK          *pck2);

short  pck_abs            (const T_PCK    *pck1,
                           T_PCK          *pck2);

short  pck_neg            (const T_PCK    *pck1,
                           T_PCK          *pck2);

short  pck_to_pckov       (const T_PCK    *pck,
                           T_PCKOV        *pckov);

short  pck_from_pckov (T_PCK          *pck,
                           const T_PCKOV  *pckov);

short  pck_to_fux         (const T_PCK    *pck,
                           T_FUX          *fux);

short  pck_from_fux       (T_PCK          *pck,
                           const T_FUX    *fux);

short  pck_to_dlong       (const T_PCK     *pck,
                           unsigned char   *dword,
                           unsigned short   nks);

short  pck_from_dlong     (T_PCK                *pck,
                           const unsigned char  *dword,
                           unsigned short       nks);

short decari_to_double    (const tDec *pd,
                           double     *result);

short decari_from_double  (tDec       *pd,
                           double      value);

short decari_to_long      (const tDec *pd,
                           long       *result,
                           short       nks);

short decari_from_long    (tDec       *pd,
                           const long  value,
                           short       nks);

short decari_to_ulong     (const tDec          *pd,
                           unsigned long       *result,
                           short                nks);

short decari_from_ulong   (tDec                *pd,
                           const unsigned long  value,
                           short                nks);

short decari_to_pck       (const tDec    *pd,
                           T_PCK         *pck);

short decari_from_pck     (tDec          *pd,
                           const T_PCK   *pck);

short decari_to_unpck      (const tDec    *pd,
                           T_UNPCK       *data);

short decari_from_unpck    (tDec          *pd,
                           const T_UNPCK *unpck);

short decari_to_pckov       (const tDec    *pd,
                           T_PCKOV       *pckov);

short decari_from_pckov     (tDec          *pd,
                           const T_PCKOV *pckov);


short decari_to_fux       (const tDec    *pd,
                           T_FUX         *fux);

short decari_from_fux     (tDec          *pd,
                           const T_FUX   *fux);

short decari_to_dlong     (const    tDec *pd,
                           unsigned char *data,
                           short          nks);

short decari_from_dlong   (tDec                *pd,
                           const unsigned char *data,
                           short                nks);

short  decari_floor       (tDec                *pd1,
                           tDec                *pd2);

#define decari_from_packed packed_to_decari

#define pck_to_fb1   pck_to_uchar
#define pck_from_fb1 pck_from_uchar
#define pck_to_fb2   pck_to_ushort
#define pck_from_fb2 pck_from_ushort

#define pck_to_fb4   pck_to_ulong
#define pck_from_fb4 pck_from_ulong


/* Macros aus hzsipc.c, teilweise angepasst */

#define getdigit()                              \
  {                                             \
    if (pck_hi)                                 \
      digit = *packed >> 4;                     \
    else                                        \
      digit = *packed ++ & 0x0f;                \
    pck_hi = !pck_hi;                           \
    if (digit > 9)                              \
    {                                           \
       return ERROR_NO_PACKED;                  \
    }                                           \
  }

/*
 * getsign()
 *
 * liefert das Vorzeichen einer gepackten Zahl. Das Vorzeichen
 * ist entweder IPPL oder IPMI.
 */

#define getsign()                              \
      digit = ip_translate_digit[*packed & 0x0f]
/*
 * put_digit_rev()
 *
 * legt die naechste Ziffer in einer gepackten Zahl ab. Die Ziffern
 * werden von rechts nach links eingetragen (Vorzeichen zuerst).
 * Variablen wie get_digit().
 */

#define put_digitrev(digit)                    \
  {                                             \
    if (pck_hi)                                 \
      *packed-- |= ((digit) << 4);              \
    else                                        \
      *packed = (digit);                        \
    pck_hi = !pck_hi;                           \
  }

#endif
