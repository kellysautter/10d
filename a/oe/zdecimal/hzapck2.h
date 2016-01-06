/*
** MODULE NAME: hzapck2.h
**----------------------------------------------------------------------
**   Additional include file needed by hzapck.c, to avoid that parts
**   or all of the 'new siron arithmetic' has to be included and linked
**   to the application using the hzapck module
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

#ifndef __HZAPCK2_H__
#define __HZAPCK2_H__



/*** Aus Siron Headern extrahiert (kopiert) um nicht die Arithmetik
 *** ziehen zu muessen
 ***/

typedef double tNumber;
#define arPow10Entries 65

extern const tNumber arPow10Array[arPow10Entries];

#define arPowP10(a)     arPow10Array[32 + (a)]
#define arPowM10(a)     arPow10Array[32 - (a)]

/*
 * default-Vorzeichen fuer gepackte Arith. in C
 */

#define IPPL 0xc   /* positives Vorzeichen */
#define IPMI 0xd   /* negatives Vorzeichen */



#endif  /* #ifndef __REST_H__ */
