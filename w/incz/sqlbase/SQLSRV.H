/* COPYRIGHT (C) GUPTA CORPORATION 1984-1997 */
/*
INTERFACE TO
  SQL SRV
*/
/*
REVISION HISTORY
  12/04/91 GTI release 5.0.0
  04/01/92 GTI release 5.0.2
  11/02/92 GTI release 5.1.1
  05/03/93 GTI release 5.2.0
  05/19/94 GTI release 6.0.0
  04/12/95 GTI release 6.0.1
  08/02/95 GTI release 6.1.0
  12/20/95 GTI release 6.1.1
  11/14/96 GTI release 6.5.0
  10/20/97 GTI release 7.0.0
DESCRIPTION
  This file contains structure   definitions  and  defined  constants  used  to
  interface with a SQLBASE SERVER.
*/

#ifndef SQLSRV
#define SQLSRV

#ifndef SQL
#include "sql.h"
#endif   /* !SQL */

#ifdef __cplusplus
extern "C" {      /* Assume C declarations for C++        */
#endif   /* __cplusplus */

#ifndef SIZEOF
#define SIZEOF(x) (int)sizeof(x)
#endif

/* server remote procedure call open file flags              */
#define   SQLORDONLY 0x0000      /* open for reading only       */
#define   SQLOWRONLY 0x0001      /* open for writing only       */
#define   SQLORDWR   0x0002      /* open for reading and writing     */
#define   SQLOAPPEND 0x0008      /* writes done at eof          */
#define   SQLOCREAT  0x0100      /* create and open file        */
#define   SQLOTRUNC  0x0200      /* open and truncate        */
#define   SQLOEXCL   0x0400      /* open if file doesn't exist     */
#define   SQLODIRCREA   0x0800      /* create directory if doesn't exist */
#define   SQLOTEXT   0x4000      /* file mode is text        */
#define   SQLOBINARY 0x8000      /* file mode is binary         */

/* server information flags */
#define   SQLGPWD   0x01      /* send password         */
#define   SQLGCUR   0x02      /* cursor information          */
#define   SQLGDBS   0x04      /* database information        */
#define   SQLGCFG   0x08      /* configuration information      */
#define   SQLGSTT   0x10      /* statistics            */
#define   SQLGPRC   0x20      /* process information         */

/* message header */
struct hdrdefx
{
     unsigned short hdrlen;      /* message length (including hdr)   */
     unsigned short hdrrsv;      /* reserved           */
};
typedef struct hdrdefx hdrdef;
#define HDRSIZ SIZEOF(hdrdef)

/* message section header */
struct mshdefx
{
     unsigned short mshflg;      /* section data type        */
     unsigned short mshten;      /* total # of entries available     */
     unsigned short mshnen;      /* # of entries contained in msg    */
     unsigned short mshlen;      /* # of data bytes in msg section   */
               /* (including this header)     */
};
typedef struct mshdefx mshdef;
#define MSHSIZ SIZEOF(mshdef)

/* cursor information */
struct curdefx
{
   unsigned long  currow;     /* number of rows        */
   unsigned short curibl;     /* input buffer          */
   unsigned short curobl;     /* output message buffer length     */
   unsigned short curspr;     /* stat counter, physical reads     */
   unsigned short curspw;     /* stat counter, physical writes    */
   unsigned short cursvr;     /* stat counter, virtual reads       */
   unsigned short cursvw;     /* stat counter, virtual writes     */
   unsigned char  curtyp;     /* command type          */
   unsigned char  curpnm;     /* process number        */
   unsigned char  curiso[3];  /* locking isolation        */
   unsigned char  curunb[19]; /* user name buffer         */
   unsigned char  curdbn[9];  /* database name         */
   unsigned char  currsv[3];  /* reserved           */
};
typedef struct curdefx curdef;
#define CURSIZ SIZEOF(curdef)

/* database information */
struct dbsdefx
{
     unsigned long  dbsbfs;      /* before image file size      */
     unsigned long  dbsbwp;      /* before image write page     */
     unsigned long  dbsdfs;      /* database file size          */
     unsigned long  dbsfrp;      /* 1st reader page in circular log  */
     unsigned long  dbsfup;      /* 1st updater page in circular log */
     unsigned long  dbslpa;      /* last page allocated         */
     unsigned long  dbslpm;      /* log page maximum         */
     unsigned long  dbslpt;      /* log page threshold          */
     unsigned long  dbslpw;      /* last page written        */
     unsigned long  dbsltp;      /* last temporary page accessed     */
     unsigned long  dbsltw;      /* last temporary page written       */
     unsigned char  dbsuse;      /* use count             */
     unsigned char  dbsnat;      /* number of active transactions    */
     unsigned char  dbsntr;      /* number of transactions      */
     unsigned char  dbsfnm[9];      /* database file name          */
};
typedef struct dbsdefx dbsdef;
#define DBSSIZ SIZEOF(dbsdef)

/* configuration information */
struct cfgdefx
{
     unsigned long  cfgwsa;      /* work space allocation size     */
     unsigned long  cfgwsl;      /* work space limit         */
     unsigned short cfgnlk;      /* number of locks          */
     unsigned short cfgnpg;      /* number of pages          */
     unsigned short cfgcnc;      /* connect count         */
     unsigned char  cfgsvn[9];      /* server name           */
     unsigned char  cfgrsv;      /* reserved           */
};
typedef   struct    cfgdefx cfgdef;
#define CFGSIZ SIZEOF(cfgdef)

/* statistics */
struct sttdefx
{
     unsigned long  sttspr;      /* physical reads        */
     unsigned long  sttspw;      /* physical writes          */
     unsigned long  sttsvr;      /* virtual reads         */
     unsigned long  sttsvw;      /* virtual writes        */
     unsigned long  sttnps;      /* number of process switches     */
};
typedef struct sttdefx sttdef;
#define STTSIZ SIZEOF(sttdef)

/* process information */
struct prcdefx
{
     unsigned short prccol;      /* current output message length    */
     unsigned short prcibl;      /* input message buffer length       */
     unsigned short prcinl;      /* input length          */
     unsigned short prcobl;      /* output message buffer length     */
     unsigned short prcoul;      /* output length         */
     unsigned char  prcpnm;      /* process number        */
     unsigned char  prcact;      /* active flag           */
};
typedef struct prcdefx prcdef;
#define PRCSIZ SIZEOF(prcdef)

#ifdef __cplusplus
}
#endif   /* __cplusplus */

#endif   /* !SQLSRV */
