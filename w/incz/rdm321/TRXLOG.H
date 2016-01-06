/*----------------------------------------------------------------------
   trxlog.h -- memory cache overflow and transaction log file control

   Copyright (c) 1984-1992, Raima Corporation, All Rights Reserved
----------------------------------------------------------------------*/

/* ********************** EDIT HISTORY *******************************

 SCR    DATE    INI                   DESCRIPTION
----- --------- --- -----------------------------------------------------
      08-Aug-89 WLW Added ADD_LOGFILE and DEL_LOGFILE constants

*/

#ifndef TRXLOG_H
#define TRXLOG_H

/*
==========================================================================
   The following constants control the functioning of the cache overflow
   and transaction logging processes

   BUI             The number of bits in an unsigned int
   IX_PAGESIZE     The size (in bytes) of an index page
   IX_EPP          The number of entries that will fit on an index page
   RI_BITMAP_SIZE  The size of the index ri_bitmap (in unsigned int units)
   IX_SIZE         The number of index pages needed to control the db pages
   OADDR_OF_IXP    Calculates the overflow file address of an index page #

==========================================================================
*/

/* (BITS(unsigned int)) */
#define BUI 16

/* ((((256*sizeof(F_ADDR))+D_BLKSZ-1) / D_BLKSZ)*D_BLKSZ) */
#define IX_PAGESIZE 1024

/* (IX_PAGESIZE / sizeof(F_ADDR)) */
#define IX_EPP 256

#define RI_BITMAP_SIZE(pcnt) ((int)((IX_SIZE(pcnt)+(BUI-1)) / BUI))

#define IX_SIZE(pcnt) ((long)( ((pcnt) + (IX_EPP-1)) / IX_EPP))

/* Next define the base file offsets for entries in the overflow file */

#define BM_BASE( file ) ( root_ix[file].base )
#define IX_BASE(file, pcnt) ((long)(BM_BASE(file) + (RI_BITMAP_SIZE(pcnt)*sizeof(unsigned int))))
#define PZ_BASE(file, pcnt) ((long)(IX_BASE(file, pcnt) + (IX_SIZE(pcnt)*IX_PAGESIZE)))

/*
==========================================================================
*/

/* The following typedef'ed structure defines a single entry in the
   root index data.  */

typedef struct RI_ENTRY_S
{
   long pg_cnt;                        /* Number of pages currently in
                                        * file */
   F_ADDR base;                        /* Base of data stored in overflow */
   BOOLEAN pz_modified;                /* Was page zero written to
                                        * overflow? */
   INT_P Ri_bitmap;                       /* Used index page ri_bitmap */
}  RI_ENTRY;

#define ri_bitmap Ri_bitmap.ptr
#define RI_ENTRY_IOSIZE (sizeof(RI_ENTRY)-sizeof(INT_P)+sizeof(DB_SHORT *))

/*
==========================================================================
*/

/* page zero table entry */
#define PGZEROSZ (2*sizeof(F_ADDR)+sizeof(DB_ULONG))
typedef struct PGZERO_S
{
   F_ADDR pz_dchain;                   /* delete chain pointer */
   F_ADDR pz_next;                     /* next available record number */
   DB_ULONG pz_timestamp;              /* file's timestamp value */
   BOOLEAN pz_modified;                /* TRUE if page zero has been
                                        * modified */
}  PGZERO;

/* binary search lookup table entry */

/* External declarations */
extern int trlog_flag;                 /* Transaction logging enabled flag */
extern int trcommit;                   /* Transaction commit in progress
                                        * flag
                                        */
extern TAFFILE tafbuf;                 /* Current TAF file after a
                                        * taf_access() call
                                        */
/* When the TAF file contains TAFLIMIT entries, it is permissible to
   access it if you are going to remove an entry, but not if you want
   to add an entry.  The following definitions are used as the parameter
   to taf_access().
*/
#define ADD_LOGFILE 1
#define DEL_LOGFILE 0

#endif
/* vpp @win.vpp TRXLOG.h (Thu May 21 10:46:38 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

