/////////////////////////////////////////////////////////////////////////////
// Project ZDrApp
//
// This is a part of the Zeidon Dynamic Rendering of GUI applications package.
// Copyright © 1995 - 2016 QuinSoft, Inc.
// All Rights Reserved.
//
// SUBSYSTEM:    zdrapp.dll - ZDr Application
// FILE:         zdrcomm.cpp
// AUTHOR:
//
// OVERVIEW
// ========
// Source file for implementation of Common dialogs.
//

//
// CHANGE LOG - most recent first order
//
// 200y.mm.dd    xxx
//    Note ...

#include "zstdafx.h"
#include "resource.h"

#define ZDRAPP_CLASS AFX_EXT_CLASS
#include "ZDr.h"
#include "zdrgbl.h"
#include "ZDrCmprs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/* compress.c -- target dependent utility functions for the compression library
 * Copyright (C) 1995-1996 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in compress.h
 */

/* $Id: zutil.c,v 1.8 1995/05/03 17:27:12 jloup Exp $ */

//struct dinternal_state     {int dummy;}; /* for buggy compilers */

#ifndef STDC
extern void exit OF((int));
#endif

zCPCHAR compress_version = COMPRESS_VERSION;

zCPCHAR z_errmsg[10] =
{
"need dictionary",     /* Z_NEED_DICT       2  */
"stream end",          /* Z_STREAM_END      1  */
"",                    /* Z_OK              0  */
"file error",          /* Z_ERRNO         (-1) */
"stream error",        /* Z_STREAM_ERROR  (-2) */
"data error",          /* Z_DATA_ERROR    (-3) */
"insufficient memory", /* Z_MEM_ERROR     (-4) */
"buffer error",        /* Z_BUF_ERROR     (-5) */
"incompatible version",/* Z_VERSION_ERROR (-6) */
""};


void
z_error(char *m)
{
    fprintf(stderr, "%s\n", m);
    exit(1);
}

#ifndef HAVE_MEMCPY

void
zmemcpy(Bytef *dest, Bytef *source, uInt len)
{
    if (len == 0) return;
    do {
        *dest++ = *source++; /* ??? to be unrolled */
    } while (--len != 0);
}

void
zmemzero(Bytef *dest, uInt len)
{
    if (len == 0) return;
    do {
        *dest++ = 0;  /* ??? to be unrolled */
    } while (--len != 0);
}
#endif

#if defined(M_I86) && !(defined(__WATCOMC__) && defined(__386__))
/* Microsoft C */

#  define MY_ZCALLOC

#if (!defined(_MSC_VER) || (_MSC_VER < 600))
#  define _halloc  halloc
#  define _hfree   hfree
#endif

voidpf
zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    return _halloc((zLONG)items, size);
}

void
zcfree(voidpf opaque, voidpf ptr)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    _hfree(ptr);
}

#endif /* MSC */


#ifndef MY_ZCALLOC /* Any system without a special alloc function */

#ifndef STDC
extern voidp  calloc OF((uInt items, uInt size));
extern void   free   OF((voidpf ptr));
#endif

voidpf
zcalloc(voidpf opaque, unsigned items, unsigned size)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    return (voidpf)calloc(items, size);
}

void
zcfree(voidpf opaque, voidpf ptr)
{
    if (opaque) opaque = 0; /* to make compiler happy */
    free(ptr);
}

#endif /* MY_ZCALLOC */

/* adler32.c -- compute the Adler-32 checksum of a data stream
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/* $Id: adler32.c,v 1.6 1995/05/03 17:27:08 jloup Exp $ */

//#include "zlib.h"

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf)  {s1 += *buf++; s2 += s1;}
#define DO2(buf)  DO1(buf); DO1(buf);
#define DO4(buf)  DO2(buf); DO2(buf);
#define DO8(buf)  DO4(buf); DO4(buf);
#define DO16(buf) DO8(buf); DO8(buf);

/* ======================================================================= */
uLong adler32(uLong adler, const Bytef *buf, uInt len)
{
    unsigned long s1 = adler & 0xffff;
    unsigned long s2 = (adler >> 16) & 0xffff;
    int k;

    if (buf == 0) return 1L;

    while (len > 0) {
        k = len < NMAX ? len : NMAX;
        len -= k;
        while (k >= 16) {
            DO16(buf);
            k -= 16;
        }
        if (k != 0) do {
            DO1(buf);
        } while (--k);
        s1 %= BASE;
        s2 %= BASE;
    }
    return (s2 << 16) | s1;
}

////////////////////////////////// DEFLATE.CPP //////////////////////////////

/* deflate.c -- compress data using the deflation algorithm
 * Copyright (C) 1995-1996 Jean-loup Gailly.
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

/*
 *  ALGORITHM
 *
 *      The "deflation" process depends on being able to identify portions
 *      of the input text which are identical to earlier input (within a
 *      sliding window trailing behind the input currently being processed).
 *
 *      The most straightforward technique turns out to be the fastest for
 *      most input files: try all possible matches and select the longest.
 *      The key feature of this algorithm is that insertions into the string
 *      dictionary are very simple and thus fast, and deletions are avoided
 *      completely. Insertions are performed at each input character, whereas
 *      string matches are performed only when the previous match ends. So it
 *      is preferable to spend more time in matches to allow very fast string
 *      insertions and avoid deletions. The matching algorithm for small
 *      strings is inspired from that of Rabin & Karp. A brute force approach
 *      is used to find longer strings when a small match has been found.
 *      A similar algorithm is used in comic (by Jan-Mark Wams) and freeze
 *      (by Leonid Broukhis).
 *      A previous version of this file used a more sophisticated algorithm
 *      (by Fiala and Greene) which is guaranteed to run in linear amortized
 *      time, but has a larger average cost, uses more memory and is patented.
 *      However the F&G algorithm may be faster for some highly redundant
 *      files if the parameter max_chain_length (described below) is too large.
 *
 *  ACKNOWLEDGEMENTS
 *
 *      The idea of lazy evaluation of matches is due to Jan-Mark Wams, and
 *      I found it in 'freeze' written by Leonid Broukhis.
 *      Thanks to many people for bug reports and testing.
 *
 *  REFERENCES
 *
 *      Deutsch, L.P.,"'Deflate' Compressed Data Format Specification".
 *      Available in ftp.uu.net:/pub/archiving/zip/doc/deflate-1.1.doc
 *
 *      A description of the Rabin and Karp algorithm is given in the book
 *         "Algorithms" by R. Sedgewick, Addison-Wesley, p252.
 *
 *      Fiala,E.R., and Greene,D.H.
 *         Data Compression with Finite Windows, Comm.ACM, 32,4 (1989) 490-595
 *
 */

/* $Id: deflate.c,v 1.8 1995/05/03 17:27:08 jloup Exp $ */

//#include "zlib.h"

char deflate_copyright[] = " deflate 1.0 Copyright 1995-1996 Jean-loup Gailly ";
/*
  If you use the zlib library in a product, an acknowledgment is welcome
  in the documentation of your product. If for some reason you cannot
  include such an acknowledgment, I would appreciate that you keep this
  copyright string in the executable of your product.
 */

/* ===========================================================================
 *  Function prototypes.
 */
local void fill_window    OF((deflate_state *s));
local int  deflate_stored OF((deflate_state *s, int flush));
local int  deflate_fast   OF((deflate_state *s, int flush));
local int  deflate_slow   OF((deflate_state *s, int flush));
local void lm_init        OF((deflate_state *s));
local int longest_match   OF((deflate_state *s, IPos cur_match));
local void putShortMSB    OF((deflate_state *s, uInt b));
local void flush_pending  OF((z_stream *strm));
local int read_buf        OF((z_stream *strm, charf *buf, unsigned size));
#ifdef ASMV
      void match_init OF((void)); /* asm code initialization */
#endif

#ifdef DEBUG
local  void check_match OF((deflate_state *s, IPos start, IPos match,
                            int length));
#endif

/* ===========================================================================
 * Local data
 */

#define NIL 0
/* Tail of hash chains */

#ifndef TOO_FAR
#  define TOO_FAR 4096
#endif
/* Matches of length 3 are discarded if their distance exceeds TOO_FAR */

#define MIN_LOOKAHEAD (MAX_MATCH+MIN_MATCH+1)
/* Minimum amount of lookahead, except at the end of the input file.
 * See deflate.c for comments about the MIN_MATCH+1.
 */

typedef int (*compress_func) OF((deflate_state *s, int flush));
/* Compressing function */

/* Values for max_lazy_match, good_match and max_chain_length, depending on
 * the desired pack level (0..9). The values given below have been tuned to
 * exclude worst case performance for pathological files. Better values may be
 * found for specific files.
 */
typedef struct config_s {
   ush good_length; /* reduce lazy search above this match length */
   ush max_lazy;    /* do not perform lazy search above this match length */
   ush nice_length; /* quit search above this match length */
   ush max_chain;
   compress_func func;
} config;

local config configuration_table[10] = {
/*      good lazy nice chain */
/* 0 */ {0,    0,  0,    0, deflate_stored},  /* store only */
/* 1 */ {4,    4,  8,    4, deflate_fast}, /* maximum speed, no lazy matches */
/* 2 */ {4,    5, 16,    8, deflate_fast},
/* 3 */ {4,    6, 32,   32, deflate_fast},

/* 4 */ {4,    4, 16,   16, deflate_slow},  /* lazy matches */
/* 5 */ {8,   16, 32,   32, deflate_slow},
/* 6 */ {8,   16, 128, 128, deflate_slow},
/* 7 */ {8,   32, 128, 256, deflate_slow},
/* 8 */ {32, 128, 258, 1024, deflate_slow},
/* 9 */ {32, 258, 258, 4096, deflate_slow}}; /* maximum compression */

/* Note: the deflate() code requires max_lazy >= MIN_MATCH and max_chain >= 4
 * For deflate_fast() (levels <= 3) good is ignored and lazy has a different
 * meaning.
 */

#define EQUAL 0
/* result of memcmp for equal strings */

//struct static_tree_desc_s {int dummy;}; /* for buggy compilers */

/* ===========================================================================
 * Update a hash value with the given input byte
 * IN  assertion: all calls to to UPDATE_HASH are made with consecutive
 *    input characters, so that a running hash key can be computed from the
 *    previous key instead of complete recalculation each time.
 */
#define UPDATE_HASH(s,h,c) (h = (((h)<<s->d.hash_shift) ^ (c)) & s->d.hash_mask)


/* ===========================================================================
 * Insert string str in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return
 * the previous length of the hash chain.
 * IN  assertion: all calls to to INSERT_STRING are made with consecutive
 *    input characters and the first MIN_MATCH bytes of str are valid
 *    (except for the last MIN_MATCH-1 bytes of the input file).
 */
#define INSERT_STRING(s, str, match_head) \
   (UPDATE_HASH(s, s->d.ins_h, s->d.window[(str) + (MIN_MATCH-1)]), \
    s->d.prev[(str) & s->d.w_mask] = match_head = s->d.head[s->d.ins_h], \
    s->d.head[s->d.ins_h] = (str))

/* ===========================================================================
 * Initialize the hash table (avoiding 64K overflow for 16 bit systems).
 * prev[] will be initialized on the fly.
 */
#define CLEAR_HASH(s) \
    s->d.head[s->d.hash_size-1] = NIL; \
    zmemzero((charf *)s->d.head, (unsigned)(s->d.hash_size-1)*sizeof(*s->d.head));

/* ========================================================================= */
int deflateInit_(z_stream *strm, int level,
                 zCPCHAR version, int stream_size)
{
    return deflateInit2_(strm, level, Z_DEFLATED, MAX_WBITS, DEF_MEM_LEVEL,
          Z_DEFAULT_STRATEGY, version, stream_size);
    /* To do: ignore strm->next_in if we use it as window */
}

/* ========================================================================= */
int deflateInit2_(z_stream *strm,
                  int  level,
                  int  method,
                  int  windowBits,
                  int  memLevel,
                  int  strategy,
                  zCPCHAR version,
                  int stream_size)
{
    deflate_state *s;
    int noheader = 0;

    if (version == 0 || version[0] != COMPRESS_VERSION[0] ||
        stream_size != sizeof(z_stream)) {
      return Z_VERSION_ERROR;
    }
    if (strm == 0) return Z_STREAM_ERROR;

    strm->msg = 0;
    if (strm->zalloc == 0) {
      strm->zalloc = zcalloc;
      strm->opaque = (voidpf)0;
    }
    if (strm->zfree == 0) strm->zfree = zcfree;

    if (level == Z_DEFAULT_COMPRESSION) level = 6;

    if (windowBits < 0) { /* undocumented feature: suppress zlib header */
        noheader = 1;
        windowBits = -windowBits;
    }
    if (memLevel < 1 || memLevel > MAX_MEM_LEVEL || method != Z_DEFLATED ||
        windowBits < 8 || windowBits > 15 || level < 0 || level > 9 ||
        strategy < 0 || strategy > Z_HUFFMAN_ONLY) {
        return Z_STREAM_ERROR;
    }
    s = (deflate_state *) ZALLOC(strm, 1, sizeof(deflate_state));
    if (s == 0) return Z_MEM_ERROR;
    strm->state = (struct internal_state FAR *)s;
    s->d.strm = strm;

    s->d.noheader = noheader;
    s->d.w_bits = windowBits;
    s->d.w_size = 1 << s->d.w_bits;
    s->d.w_mask = s->d.w_size - 1;

    s->d.hash_bits = memLevel + 7;
    s->d.hash_size = 1 << s->d.hash_bits;
    s->d.hash_mask = s->d.hash_size - 1;
    s->d.hash_shift =  ((s->d.hash_bits+MIN_MATCH-1)/MIN_MATCH);

    s->d.window = (Bytef *) ZALLOC(strm, s->d.w_size, 2*sizeof(Byte));
    s->d.prev   = (Posf *)  ZALLOC(strm, s->d.w_size, sizeof(Pos));
    s->d.head   = (Posf *)  ZALLOC(strm, s->d.hash_size, sizeof(Pos));

    s->d.lit_bufsize = 1 << (memLevel + 6); /* 16K elements by default */

    s->d.pending_buf = (uchf *) ZALLOC(strm, s->d.lit_bufsize, 2*sizeof(ush));

    if (s->d.window == 0 || s->d.prev == 0 || s->d.head == 0 ||
        s->d.pending_buf == 0) {
        strm->msg = ERR_MSG(Z_MEM_ERROR);
        deflateEnd (strm);
        return Z_MEM_ERROR;
    }
    s->d.l_buf = (uchf *) &(s->d.pending_buf[s->d.lit_bufsize]);
    s->d.d_buf = (ushf *) &(s->d.pending_buf[2*s->d.lit_bufsize]);
    /* We overlay pending_buf and d_buf+l_buf. This works since the average
     * output size for (length,distance) codes is <= 32 bits (worst case
     * is 15+15+13=zTAG_LTH). d_buf is put last in case zsizeof(short)>2.
     */

    s->d.level = level;
    s->d.strategy = strategy;
    s->d.method = (Byte)method;

    return deflateReset(strm);
}

/* ========================================================================= */
int deflateSetDictionary(z_stream *strm,
                         const Bytef *dictionary,
                         uInt  dictLength)
{
    deflate_state *s;
    uInt length = dictLength;
    uInt n;
    IPos hash_head;

    if (strm == 0 || strm->state == 0 || dictionary == 0 ||
        strm->state->d.status != INIT_STATE) return Z_STREAM_ERROR;

    s = strm->state;
    strm->adler = adler32(strm->adler, dictionary, dictLength);

    if (length < MIN_MATCH) return Z_OK;
    if (length > MAX_DIST(s)) {
   length = MAX_DIST(s);
   dictionary += dictLength - length;
    }
    zmemcpy((charf *)s->d.window, dictionary, length);
    s->d.strstart = length;
    s->d.block_start = (zLONG)length;

    /* Insert all strings in the hash table (except for the last two bytes).
     * s->d.lookahead stays null, so s->d.ins_h will be recomputed at the next
     * call of fill_window.
     */
    s->d.ins_h = s->d.window[0];
    UPDATE_HASH(s, s->d.ins_h, s->d.window[1]);
    for (n = 0; n <= length - MIN_MATCH; n++) {
   INSERT_STRING(s, n, hash_head);
    }
    return Z_OK;
}

/* ========================================================================= */
int deflateReset(z_stream *strm)
{
    deflate_state *s;

    if (strm == 0 || strm->state == 0 ||
        strm->zalloc == 0 || strm->zfree == 0) return Z_STREAM_ERROR;

    strm->total_in = strm->total_out = 0;
    strm->msg = 0; /* use zfree if we ever allocate msg dynamically */
    strm->data_type = Z_UNKNOWN;

    s = (deflate_state *)strm->state;
    s->d.pending = 0;
    s->d.pending_out = s->d.pending_buf;

    if (s->d.noheader < 0) {
        s->d.noheader = 0; /* was set to -1 by deflate(..., Z_FINISH); */
    }
    s->d.status = s->d.noheader ? BUSY_STATE : INIT_STATE;
    strm->adler = 1;
    s->d.last_flush = Z_NO_FLUSH;

    _tr_init(s);
    lm_init(s);

    return Z_OK;
}

/* ========================================================================= */
int deflateParams(z_stream *strm, int level, int strategy)
{
    deflate_state *s;
    compress_func func;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;
    s = strm->state;

    if (level == Z_DEFAULT_COMPRESSION) {
   level = 6;
    }
    if (level < 0 || level > 9 || strategy < 0 || strategy > Z_HUFFMAN_ONLY) {
   return Z_STREAM_ERROR;
    }
    func = configuration_table[s->d.level].func;

    if (func != configuration_table[level].func
   && strm->state->d.lookahead != 0) {

   /* Flush the last buffer: */
   (void)(*func)(strm->state, Z_PARTIAL_FLUSH);
    }
    if (s->d.level != level) {
   s->d.level = level;
   s->d.max_lazy_match   = configuration_table[level].max_lazy;
   s->d.good_match       = configuration_table[level].good_length;
   s->d.nice_match       = configuration_table[level].nice_length;
   s->d.max_chain_length = configuration_table[level].max_chain;
    }
    s->d.strategy = strategy;
    return Z_OK;
}

/* =========================================================================
 * Put a short in the pending buffer. The 16-bit value is put in MSB order.
 * IN assertion: the stream state is correct and there is enough room in
 * pending_buf.
 */
local void putShortMSB(deflate_state *s, uInt b)
{
    put_byte(s, (Byte)(b >> 8));
    put_byte(s, (Byte)(b & 0xff));
}

/* =========================================================================
 * Flush as much pending output as possible. All deflate() output goes
 * through this function so some applications may wish to modify it
 * to avoid allocating a large strm->next_out buffer and copying into it.
 * (See also read_buf()).
 */
local void flush_pending(z_stream *strm)
{
    unsigned len = strm->state->d.pending;

    if (len > strm->avail_out) len = strm->avail_out;
    if (len == 0) return;

    zmemcpy(strm->next_out, strm->state->d.pending_out, len);
    strm->next_out  += len;
    strm->state->d.pending_out  += len;
    strm->total_out += len;
    strm->avail_out  -= len;
    strm->state->d.pending -= len;
    if (strm->state->d.pending == 0) {
        strm->state->d.pending_out = strm->state->d.pending_buf;
    }
}

/* ========================================================================= */
int deflate(z_stream *strm, int flush)
{
    int old_flush; /* value of flush param for previous deflate call */
    deflate_state *s;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;

    s = strm->state;

    if (strm->next_out == 0 ||
        (strm->next_in == 0 && strm->avail_in != 0) ||
        (s->d.status == FINISH_STATE && flush != Z_FINISH))
    {
        ERR_RETURN(strm, Z_STREAM_ERROR);
    }
    if (strm->avail_out == 0) ERR_RETURN(strm, Z_BUF_ERROR);

    s->d.strm = strm; /* just in case */
    old_flush = s->d.last_flush;
    s->d.last_flush = flush;

    /* Write the zlib header */
    if (s->d.status == INIT_STATE) {

        uInt header = (Z_DEFLATED + ((s->d.w_bits-8)<<4)) << 8;
        uInt level_flags = (s->d.level-1) >> 1;

        if (level_flags > 3) level_flags = 3;
        header |= (level_flags << 6);
   if (s->d.strstart != 0) header |= PRESET_DICT;
        header += 31 - (header % 31);

        s->d.status = BUSY_STATE;
        putShortMSB(s, header);

   /* Save the adler32 of the preset dictionary: */
   if (s->d.strstart != 0) {
       putShortMSB(s, (uInt)(strm->adler >> 16));
       putShortMSB(s, (uInt)(strm->adler & 0xffff));
       strm->adler = 1L;
   }
    }

    /* Flush as much pending output as possible */
    if (s->d.pending != 0) {
        flush_pending(strm);
        if (strm->avail_out == 0) return Z_OK;

    /* Make sure there is something to do and avoid duplicate consecutive
     * flushes. For repeated and useless calls with Z_FINISH, we keep
     * returning Z_STREAM_END instead of Z_BUFF_ERROR.
     */
    } else if (strm->avail_in == 0 && flush <= old_flush &&
          flush != Z_FINISH) {
        ERR_RETURN(strm, Z_BUF_ERROR);
    }

    /* User must not provide more input after the first FINISH: */
    if (s->d.status == FINISH_STATE && strm->avail_in != 0) {
        ERR_RETURN(strm, Z_BUF_ERROR);
    }

    /* Start a new block or continue the current one.
     */
    if (strm->avail_in != 0 || s->d.lookahead != 0 ||
        (flush != Z_NO_FLUSH && s->d.status != FINISH_STATE)) {
        int quit;

        if (flush == Z_FINISH) {
            s->d.status = FINISH_STATE;
        }
   quit = (*(configuration_table[s->d.level].func))(s, flush);

        if (quit || strm->avail_out == 0) return Z_OK;
        /* If flush != Z_NO_FLUSH && avail_out == 0, the next call
         * of deflate should use the same flush parameter to make sure
         * that the flush is complete. So we don't have to output an
         * empty block here, this will be done at next call. This also
         * ensures that for a very small output buffer, we emit at most
         * one empty block.
         */
        if (flush != Z_NO_FLUSH && flush != Z_FINISH) {
            if (flush == Z_PARTIAL_FLUSH) {
                _tr_align(s);
            } else { /* FULL_FLUSH or SYNC_FLUSH */
                _tr_stored_block(s, (char*)0, 0L, 0);
                /* For a full flush, this empty block will be recognized
                 * as a special marker by inflate_sync().
                 */
                if (flush == Z_FULL_FLUSH) {
                    CLEAR_HASH(s);             /* forget history */
                }
            }
            flush_pending(strm);
            if (strm->avail_out == 0) return Z_OK;
        }
    }
    Assert(strm->avail_out > 0, "bug2");

    if (flush != Z_FINISH) return Z_OK;
    if (s->d.noheader) return Z_STREAM_END;

    /* Write the zlib trailer (adler32) */
    putShortMSB(s, (uInt)(strm->adler >> 16));
    putShortMSB(s, (uInt)(strm->adler & 0xffff));
    flush_pending(strm);
    /* If avail_out is zero, the application will call deflate again
     * to flush the rest.
     */
    s->d.noheader = -1; /* write the trailer only once! */
    return s->d.pending != 0 ? Z_OK : Z_STREAM_END;
}

/* ========================================================================= */
int deflateEnd(z_stream *strm)
{
    int status;

    if (strm == 0 || strm->state == 0) return Z_STREAM_ERROR;

    /* Deallocate in reverse order of allocations: */
    TRY_FREE(strm, strm->state->d.pending_buf);
    TRY_FREE(strm, strm->state->d.head);
    TRY_FREE(strm, strm->state->d.prev);
    TRY_FREE(strm, strm->state->d.window);

    status = strm->state->d.status;
    ZFREE(strm, strm->state);
    strm->state = 0;

    return status == BUSY_STATE ? Z_DATA_ERROR : Z_OK;
}

/* ========================================================================= */
int deflateCopy(z_stream *dest, z_stream *source)
{
    if (source == 0 || dest == 0 || source->state == 0) {
        return Z_STREAM_ERROR;
    }
    *dest = *source;
    return Z_STREAM_ERROR; /* to be implemented */
#if 0
    dest->state = (struct dinternal_state FAR *)
        (*dest->zalloc)(1, zsizeof(deflate_state));
    if (dest->state == 0) return Z_MEM_ERROR;

    *(dest->state) = *(source->state);
    return Z_OK;
#endif
}

/* ===========================================================================
 * Read a new buffer from the current input stream, update the adler32
 * and total number of bytes read.  All deflate() input goes through
 * this function so some applications may wish to modify it to avoid
 * allocating a large strm->next_in buffer and copying from it.
 * (See also flush_pending()).
 */
local int read_buf(z_stream *strm, charf *buf, unsigned size)
{
    unsigned len = strm->avail_in;

    if (len > size) len = size;
    if (len == 0) return( 0 );

    strm->avail_in  -= len;

    if (!strm->state->d.noheader) {
        strm->adler = adler32(strm->adler, strm->next_in, len);
    }
    zmemcpy(buf, strm->next_in, len);
    strm->next_in  += len;
    strm->total_in += len;

    return (int)len;
}

/* ===========================================================================
 * Initialize the "longest match" routines for a new zlib stream
 */
local void lm_init(deflate_state *s)
{
    s->d.window_size = (ulg)2L*s->d.w_size;

    CLEAR_HASH(s);

    /* Set the default configuration parameters:
     */
    s->d.max_lazy_match   = configuration_table[s->d.level].max_lazy;
    s->d.good_match       = configuration_table[s->d.level].good_length;
    s->d.nice_match       = configuration_table[s->d.level].nice_length;
    s->d.max_chain_length = configuration_table[s->d.level].max_chain;

    s->d.strstart = 0;
    s->d.block_start = 0L;
    s->d.lookahead = 0;
    s->d.match_length = s->d.prev_length = MIN_MATCH-1;
    s->d.match_available = 0;
    s->d.ins_h = 0;
#ifdef ASMV
    match_init(); /* initialize the asm code */
#endif
}

/* ===========================================================================
 * Set match_start to the longest match starting at the given string and
 * return its length. Matches shorter or equal to prev_length are discarded,
 * in which case the result is equal to prev_length and match_start is
 * garbage.
 * IN assertions: cur_match is the head of the hash chain for the current
 *   string (strstart) and its distance is <= MAX_DIST, and prev_length >= 1
 * OUT assertion: the match length is not greater than s->d.lookahead.
 */
#ifndef ASMV
/* For 80x86 and 680x0, an optimized version will be provided in match.asm or
 * match.S. The code will be functionally equivalent.
 */
local int longest_match(deflate_state *s,
                        IPos cur_match) /* current match */
{
    unsigned chain_length = s->d.max_chain_length;/* max hash chain length */
    register Bytef *scan = s->d.window + s->d.strstart; /* current string */
    register Bytef *match;                       /* matched string */
    register int len;                           /* length of current match */
    int best_len = s->d.prev_length;              /* best match length so far */
    int nice_match = s->d.nice_match;             /* stop if match long enough */
    IPos limit = s->d.strstart > (IPos)MAX_DIST(s) ?
        s->d.strstart - (IPos)MAX_DIST(s) : NIL;
    /* Stop when cur_match becomes <= limit. To simplify the code,
     * we prevent matches with the string of window index 0.
     */
    Posf *prev = s->d.prev;
    uInt wmask = s->d.w_mask;

#ifdef UNALIGNED_OK
    /* Compare two bytes at a time. Note: this is not always beneficial.
     * Try with and without -DUNALIGNED_OK to check.
     */
    register Bytef *strend = s->d.window + s->d.strstart + MAX_MATCH - 1;
    register ush scan_start = *(ushf*)scan;
    register ush scan_end   = *(ushf*)(scan+best_len-1);
#else
    register Bytef *strend = s->d.window + s->d.strstart + MAX_MATCH;
    register Byte scan_end1  = scan[best_len-1];
    register Byte scan_end   = scan[best_len];
#endif

    /* The code is optimized for HASH_BITS >= 8 and MAX_MATCH-2 multiple of 16.
     * It is easy to get rid of this optimization if necessary.
     */
    Assert(s->d.hash_bits >= 8 && MAX_MATCH == 258, "Code too clever");

    /* Do not waste too much time if we already have a good match: */
    if (s->d.prev_length >= s->d.good_match) {
        chain_length >>= 2;
    }
    /* Do not look for matches beyond the end of the input. This is necessary
     * to make deflate deterministic.
     */
    if (nice_match > (int) s->d.lookahead) nice_match = (int) s->d.lookahead;

    Assert((ulg)s->d.strstart <= s->d.window_size-MIN_LOOKAHEAD, "need lookahead");

    do {
        Assert(cur_match < s->d.strstart, "no future");
        match = s->d.window + cur_match;

        /* Skip to next match if the match length cannot increase
         * or if the match length is less than 2:
         */
#if (defined(UNALIGNED_OK) && MAX_MATCH == 258)
        /* This code assumes zsizeof(unsigned short) == 2. Do not use
         * UNALIGNED_OK if your compiler uses a different size.
         */
        if (*(ushf*)(match+best_len-1) != scan_end ||
            *(ushf*)match != scan_start) continue;

        /* It is not necessary to compare scan[2] and match[2] since they are
         * always equal when the other bytes match, given that the hash keys
         * are equal and that HASH_BITS >= 8. Compare 2 bytes at a time at
         * strstart+3, +5, ... up to strstart+257. We check for insufficient
         * lookahead only every 4th comparison; the 128th check will be made
         * at strstart+257. If MAX_MATCH-2 is not a multiple of 8, it is
         * necessary to put more guard bytes at the end of the window, or
         * to check more often for insufficient lookahead.
         */
        Assert(scan[2] == match[2], "scan[2]?");
        scan++, match++;
        do {
        } while (*(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 *(ushf*)(scan+=2) == *(ushf*)(match+=2) &&
                 scan < strend);
        /* The funny "do {}" generates better code on most compilers */

        /* Here, scan <= window+strstart+257 */
        Assert(scan <= s->d.window+(unsigned)(s->d.window_size-1), "wild scan");
        if (*scan == *match) scan++;

        len = (MAX_MATCH - 1) - (int)(strend-scan);
        scan = strend - (MAX_MATCH-1);

#else /* UNALIGNED_OK */

        if (match[best_len]   != scan_end  ||
            match[best_len-1] != scan_end1 ||
            *match            != *scan     ||
            *++match          != scan[1])      continue;

        /* The check at best_len-1 can be removed because it will be made
         * again later. (This heuristic is not always a win.)
         * It is not necessary to compare scan[2] and match[2] since they
         * are always equal when the other bytes match, given that
         * the hash keys are equal and that HASH_BITS >= 8.
         */
        scan += 2, match++;
        Assert(*scan == *match, "match[2]?");

        /* We check for insufficient lookahead only every 8th comparison;
         * the 256th check will be made at strstart+258.
         */
        do {
        } while (*++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 *++scan == *++match && *++scan == *++match &&
                 scan < strend);

        Assert(scan <= s->d.window+(unsigned)(s->d.window_size-1), "wild scan");

        len = MAX_MATCH - (int)(strend - scan);
        scan = strend - MAX_MATCH;

#endif /* UNALIGNED_OK */

        if (len > best_len) {
            s->d.match_start = cur_match;
            best_len = len;
            if (len >= nice_match) break;
#ifdef UNALIGNED_OK
            scan_end = *(ushf*)(scan+best_len-1);
#else
            scan_end1  = scan[best_len-1];
            scan_end   = scan[best_len];
#endif
        }
    } while ((cur_match = prev[cur_match & wmask]) > limit
             && --chain_length != 0);

    if (best_len <= (int) s->d.lookahead) return best_len;
    return s->d.lookahead;
}
#endif /* ASMV */

#ifdef DEBUG
/* ===========================================================================
 * Check that the match at match_start is indeed a match.
 */
local void check_match(deflate_state *s, IPos start, IPos match, int length)
{
    /* check that the match is indeed a match */
    if (zmemcmp((charf *)s->d.window + match,
                (charf *)s->d.window + start, length) != EQUAL) {
        fprintf(stderr, " start %u, match %u, length %d\n",
      start, match, length);
        do {
       fprintf(stderr, "%c%c", s->d.window[match++], s->d.window[start++]);
   } while (--length != 0);
        z_error("invalid match");
    }
    if (verbose > 1) {
        fprintf(stderr,"\\[%d,%d]", start-match, length);
        do { putc(s->d.window[start++], stderr); } while (--length != 0);
    }
}
#else
#  define check_match(s, start, match, length)
#endif

/* ===========================================================================
 * Fill the window when the lookahead becomes insufficient.
 * Updates strstart and lookahead.
 *
 * IN assertion: lookahead < MIN_LOOKAHEAD
 * OUT assertions: strstart <= window_size-MIN_LOOKAHEAD
 *    At least one byte has been read, or avail_in == 0; reads are
 *    performed for at least two bytes (required for the zip translate_eol
 *    option -- not supported here).
 */
local void fill_window(deflate_state *s)
{
    register unsigned n, m;
    register Posf *p;
    unsigned more;    /* Amount of free space at the end of the window. */
    uInt wsize = s->d.w_size;

    do {
        more = (unsigned)(s->d.window_size -(ulg)s->d.lookahead -(ulg)s->d.strstart);

        /* Deal with !@#$% 64K limit: */
        if (more == 0 && s->d.strstart == 0 && s->d.lookahead == 0) {
            more = wsize;

        } else if (more == (unsigned)(-1)) {
            /* Very unlikely, but possible on 16 bit machine if strstart == 0
             * and lookahead == 1 (input done one byte at time)
             */
            more--;

        /* If the window is almost full and there is insufficient lookahead,
         * move the upper half to the lower one to make room in the upper half.
         */
        } else if (s->d.strstart >= wsize+MAX_DIST(s)) {

            zmemcpy((charf *)s->d.window, (charf *)s->d.window+wsize,
                   (unsigned)wsize);
            s->d.match_start -= wsize;
            s->d.strstart    -= wsize; /* we now have strstart >= MAX_DIST */

            s->d.block_start -= (zLONG) wsize;

            /* Slide the hash table (could be avoided with 32 bit values
               at the expense of memory usage):
             */
            n = s->d.hash_size;
            p = &s->d.head[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
            } while (--n);

            n = wsize;
            p = &s->d.prev[n];
            do {
                m = *--p;
                *p = (Pos)(m >= wsize ? m-wsize : NIL);
                /* If n is not on any hash chain, prev[n] is garbage but
                 * its value will never be used.
                 */
            } while (--n);

            more += wsize;
        }
        if (s->d.strm->avail_in == 0) return;

        /* If there was no sliding:
         *    strstart <= WSIZE+MAX_DIST-1 && lookahead <= MIN_LOOKAHEAD - 1 &&
         *    more == window_size - lookahead - strstart
         * => more >= window_size - (MIN_LOOKAHEAD-1 + WSIZE + MAX_DIST-1)
         * => more >= window_size - 2*WSIZE + 2
         * In the BIG_MEM or MMAP case (not yet supported),
         *   window_size == input_size + MIN_LOOKAHEAD  &&
         *   strstart + s->d.lookahead <= input_size => more >= MIN_LOOKAHEAD.
         * Otherwise, window_size == 2*WSIZE so more >= 2.
         * If there was sliding, more >= WSIZE. So in all cases, more >= 2.
         */
        Assert(more >= 2, "more < 2");

        n = read_buf(s->d.strm, (charf *)s->d.window + s->d.strstart + s->d.lookahead,
                     more);
        s->d.lookahead += n;

        /* Initialize the hash value now that we have some input: */
        if (s->d.lookahead >= MIN_MATCH) {
            s->d.ins_h = s->d.window[s->d.strstart];
            UPDATE_HASH(s, s->d.ins_h, s->d.window[s->d.strstart+1]);
#if MIN_MATCH != 3
            Call UPDATE_HASH() MIN_MATCH-3 more times
#endif
        }
        /* If the whole input has less than MIN_MATCH bytes, ins_h is garbage,
         * but this is not important since only literal bytes will be emitted.
         */

    } while (s->d.lookahead < MIN_LOOKAHEAD && s->d.strm->avail_in != 0);
}

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: strstart is set to the end of the current match.
 */
#define FLUSH_BLOCK_ONLY(s, eof) { \
   _tr_flush_block(s, (s->d.block_start >= 0L ? \
                   (charf *)&s->d.window[(unsigned)s->d.block_start] : \
                   (charf *)0), \
      (ulg)((zLONG)s->d.strstart - s->d.block_start), \
      (eof)); \
   s->d.block_start = s->d.strstart; \
   flush_pending(s->d.strm); \
   Tracev((stderr,"[FLUSH]")); \
}

/* Same but force premature exit if necessary. */
#define FLUSH_BLOCK(s, eof) { \
   FLUSH_BLOCK_ONLY(s, eof); \
   if (s->d.strm->avail_out == 0) return( 1 ); \
}

/* ===========================================================================
 * Copy without compression as much as possible from the input stream, return
 * true if processing was terminated prematurely (no more input or output
 * space).  This function does not insert new strings in the dictionary
 * since uncompressible data is probably not useful. This function is used
 * only for the level=0 compression option.
 * NOTE: this function should be optimized to avoid extra copying.
 */
local int deflate_stored(deflate_state *s, int flush)
{
    for (;;) {
        /* Fill the window as much as possible: */
        if (s->d.lookahead <= 1) {

            Assert(s->d.strstart < s->d.w_size+MAX_DIST(s) ||
         s->d.block_start >= (zLONG)s->d.w_size, "slide too late");

            fill_window(s);
            if (s->d.lookahead == 0 && flush == Z_NO_FLUSH) return( 1 );

            if (s->d.lookahead == 0) break; /* flush the current block */
        }
   Assert(s->d.block_start >= 0L, "block gone");

   s->d.strstart += s->d.lookahead;
   s->d.lookahead = 0;

        /* Stored blocks are limited to 0xffff bytes: */
        if (s->d.strstart == 0 || s->d.strstart > 0xffff) {
       /* strstart == 0 is possible when wraparound on 16-bit machine */
       s->d.lookahead = s->d.strstart - 0xffff;
       s->d.strstart = 0xffff;
   }

   /* Emit a stored block if it is large enough: */
        if (s->d.strstart - (uInt)s->d.block_start >= MAX_DIST(s)) {
            FLUSH_BLOCK(s, 0);
   }
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return( 0 ); /* normal exit */
}

/* ===========================================================================
 * Compress as much as possible from the input stream, return true if
 * processing was terminated prematurely (no more input or output space).
 * This function does not perform lazy evaluation of matches and inserts
 * new strings in the dictionary only for unmatched strings or for short
 * matches. It is used only for the fast compression options.
 */
local int deflate_fast(deflate_state *s, int flush)
{
    IPos hash_head = NIL; /* head of the hash chain */
    int bflush;           /* set if current block must be flushed */

    for (;;) {
        /* Make sure that we always have enough lookahead, except
         * at the end of the input file. We need MAX_MATCH bytes
         * for the next match, plus MIN_MATCH bytes to insert the
         * string following the next match.
         */
        if (s->d.lookahead < MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->d.lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) return( 1 );

            if (s->d.lookahead == 0) break; /* flush the current block */
        }

        /* Insert the string window[strstart .. strstart+2] in the
         * dictionary, and set hash_head to the head of the hash chain:
         */
        if (s->d.lookahead >= MIN_MATCH) {
            INSERT_STRING(s, s->d.strstart, hash_head);
        }

        /* Find the longest match, discarding those <= prev_length.
         * At this point we have always match_length < MIN_MATCH
         */
        if (hash_head != NIL && s->d.strstart - hash_head <= MAX_DIST(s)) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            if (s->d.strategy != Z_HUFFMAN_ONLY) {
                s->d.match_length = longest_match (s, hash_head);
            }
            /* longest_match() sets match_start */
        }
        if (s->d.match_length >= MIN_MATCH) {
            check_match(s, s->d.strstart, s->d.match_start, s->d.match_length);

            bflush = _tr_tally(s, s->d.strstart - s->d.match_start,
                               s->d.match_length - MIN_MATCH);

            s->d.lookahead -= s->d.match_length;

            /* Insert new strings in the hash table only if the match length
             * is not too large. This saves time but degrades compression.
             */
            if (s->d.match_length <= s->d.max_insert_length &&
                s->d.lookahead >= MIN_MATCH) {
                s->d.match_length--; /* string at strstart already in hash table */
                do {
                    s->d.strstart++;
                    INSERT_STRING(s, s->d.strstart, hash_head);
                    /* strstart never exceeds WSIZE-MAX_MATCH, so there are
                     * always MIN_MATCH bytes ahead.
                     */
                } while (--s->d.match_length != 0);
                s->d.strstart++;
            } else {
                s->d.strstart += s->d.match_length;
                s->d.match_length = 0;
                s->d.ins_h = s->d.window[s->d.strstart];
                UPDATE_HASH(s, s->d.ins_h, s->d.window[s->d.strstart+1]);
#if MIN_MATCH != 3
                Call UPDATE_HASH() MIN_MATCH-3 more times
#endif
                /* If lookahead < MIN_MATCH, ins_h is garbage, but it does not
                 * matter since it will be recomputed at next deflate call.
                 */
            }
        } else {
            /* No match, output a literal byte */
            Tracevv((stderr,"%c", s->d.window[s->d.strstart]));
            bflush = _tr_tally (s, 0, s->d.window[s->d.strstart]);
            s->d.lookahead--;
            s->d.strstart++;
        }
        if (bflush) FLUSH_BLOCK(s, 0);
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return( 0 ); /* normal exit */
}

/* ===========================================================================
 * Same as above, but achieves better compression. We use a lazy
 * evaluation for matches: a match is finally adopted only if there is
 * no better match at the next window position.
 */
local int deflate_slow(deflate_state *s, int flush)
{
    IPos hash_head = NIL;    /* head of hash chain */
    int bflush;              /* set if current block must be flushed */

    /* Process the input block. */
    for (;;) {
        /* Make sure that we always have enough lookahead, except
         * at the end of the input file. We need MAX_MATCH bytes
         * for the next match, plus MIN_MATCH bytes to insert the
         * string following the next match.
         */
        if (s->d.lookahead < MIN_LOOKAHEAD) {
            fill_window(s);
            if (s->d.lookahead < MIN_LOOKAHEAD && flush == Z_NO_FLUSH) return( 1 );

            if (s->d.lookahead == 0) break; /* flush the current block */
        }

        /* Insert the string window[strstart .. strstart+2] in the
         * dictionary, and set hash_head to the head of the hash chain:
         */
        if (s->d.lookahead >= MIN_MATCH) {
            INSERT_STRING(s, s->d.strstart, hash_head);
        }

        /* Find the longest match, discarding those <= prev_length.
         */
        s->d.prev_length = s->d.match_length, s->d.prev_match = s->d.match_start;
        s->d.match_length = MIN_MATCH-1;

        if (hash_head != NIL && s->d.prev_length < s->d.max_lazy_match &&
            s->d.strstart - hash_head <= MAX_DIST(s)) {
            /* To simplify the code, we prevent matches with the string
             * of window index 0 (in particular we have to avoid a match
             * of the string with itself at the start of the input file).
             */
            if (s->d.strategy != Z_HUFFMAN_ONLY) {
                s->d.match_length = longest_match (s, hash_head);
            }
            /* longest_match() sets match_start */

            if (s->d.match_length <= 5 && (s->d.strategy == Z_FILTERED ||
                 (s->d.match_length == MIN_MATCH &&
                  s->d.strstart - s->d.match_start > TOO_FAR))) {

                /* If prev_match is also MIN_MATCH, match_start is garbage
                 * but we will ignore the current match anyway.
                 */
                s->d.match_length = MIN_MATCH-1;
            }
        }
        /* If there was a match at the previous step and the current
         * match is not better, output the previous match:
         */
        if (s->d.prev_length >= MIN_MATCH && s->d.match_length <= s->d.prev_length) {
            uInt max_insert = s->d.strstart + s->d.lookahead - MIN_MATCH;
            /* Do not insert strings in hash table beyond this. */

            check_match(s, s->d.strstart-1, s->d.prev_match, s->d.prev_length);

            bflush = _tr_tally(s, s->d.strstart -1 - s->d.prev_match,
                               s->d.prev_length - MIN_MATCH);

            /* Insert in hash table all strings up to the end of the match.
             * strstart-1 and strstart are already inserted. If there is not
             * enough lookahead, the last two strings are not inserted in
             * the hash table.
             */
            s->d.lookahead -= s->d.prev_length-1;
            s->d.prev_length -= 2;
            do {
                if (++s->d.strstart <= max_insert) {
                    INSERT_STRING(s, s->d.strstart, hash_head);
                }
            } while (--s->d.prev_length != 0);
            s->d.match_available = 0;
            s->d.match_length = MIN_MATCH-1;
            s->d.strstart++;

            if (bflush) FLUSH_BLOCK(s, 0);

        } else if (s->d.match_available) {
            /* If there was no match at the previous position, output a
             * single literal. If there was a match but the current match
             * is longer, truncate the previous match to a single literal.
             */
            Tracevv((stderr,"%c", s->d.window[s->d.strstart-1]));
            if (_tr_tally (s, 0, s->d.window[s->d.strstart-1])) {
                FLUSH_BLOCK_ONLY(s, 0);
            }
            s->d.strstart++;
            s->d.lookahead--;
            if (s->d.strm->avail_out == 0) return( 1 );
        } else {
            /* There is no previous match to compare with, wait for
             * the next step to decide.
             */
            s->d.match_available = 1;
            s->d.strstart++;
            s->d.lookahead--;
        }
    }
    Assert (flush != Z_NO_FLUSH, "no flush?");
    if (s->d.match_available) {
        Tracevv((stderr,"%c", s->d.window[s->d.strstart-1]));
        _tr_tally (s, 0, s->d.window[s->d.strstart-1]);
        s->d.match_available = 0;
    }
    FLUSH_BLOCK(s, flush == Z_FINISH);
    return( 0 );
}

////////////////////////////////// INFLATE.CPP //////////////////////////////

/* inflate.c -- zlib interface to inflate modules
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */
//#include "zlib.h"

//struct inflate_blocks_state {int dummy;}; /* for buggy compilers */

int
inflateReset(z_stream *z)
{
  uLong c;

  if (z == 0 || z->state == 0)
    return Z_STREAM_ERROR;
  z->total_in = z->total_out = 0;
  z->msg = 0;
  z->state->i.mode = z->state->i.nowrap ? iinternal_state::BLOCKS :
                                      iinternal_state::METHOD;
  inflate_blocks_reset(z->state->i.blocks, z, &c);
  Trace((stderr, "inflate: reset\n"));
  return Z_OK;
}


int
inflateEnd(z_stream *z)
{
  uLong c;

  if (z == 0 || z->state == 0 || z->zfree == 0)
    return Z_STREAM_ERROR;
  if (z->state->i.blocks != 0)
    inflate_blocks_free(z->state->i.blocks, z, &c);
  ZFREE(z, z->state);
  z->state = 0;
  Trace((stderr, "inflate: end\n"));
  return Z_OK;
}


int inflateInit2_(z_stream *z, int w, zCPCHAR version, int stream_size)
{
  if (version == 0 || version[0] != COMPRESS_VERSION[0] ||
      stream_size != sizeof(z_stream))
      return Z_VERSION_ERROR;

  /* initialize state */
  if (z == 0)
    return Z_STREAM_ERROR;
  z->msg = 0;
  if (z->zalloc == 0)
  {
    z->zalloc = zcalloc;
    z->opaque = (voidpf)0;
  }
  if (z->zfree == 0) z->zfree = zcfree;
  if ((z->state = (struct internal_state FAR *)
       ZALLOC(z,1,sizeof(struct internal_state))) == 0)
    return Z_MEM_ERROR;
  z->state->i.blocks = 0;

  /* handle undocumented nowrap option (no zlib header or check) */
  z->state->i.nowrap = 0;
  if (w < 0)
  {
    w = - w;
    z->state->i.nowrap = 1;
  }

  /* set window size */
  if (w < 8 || w > 15)
  {
    inflateEnd(z);
    return Z_STREAM_ERROR;
  }
  z->state->i.wbits = (uInt)w;

  /* create inflate_blocks state */
  if ((z->state->i.blocks =
      inflate_blocks_new(z, z->state->i.nowrap ? 0 : adler32, (uInt)1 << w))
      == 0)
  {
    inflateEnd(z);
    return Z_MEM_ERROR;
  }
  Trace((stderr, "inflate: allocated\n"));

  /* reset state */
  inflateReset(z);
  return Z_OK;
}


int inflateInit_(z_stream *z, zCPCHAR version, int stream_size)
{
  return inflateInit2_(z, DEF_WBITS, version, stream_size);
}


#define xNEEDBYTE {if(z->avail_in==0)return r;r=Z_OK;}
#define xNEXTBYTE (z->avail_in--,z->total_in++,*z->next_in++)

int inflate(z_stream *z, int f)
{
  int r = f;    /* to avoid warning about unused f */
  uInt b;

  if (z == 0 || z->state == 0 || z->next_in == 0)
    return Z_STREAM_ERROR;
  r = Z_BUF_ERROR;
  while (1) switch (z->state->i.mode)
  {
    case iinternal_state::METHOD:
      xNEEDBYTE
      if (((z->state->i.sub.method = xNEXTBYTE) & 0xf) != Z_DEFLATED)
      {
        z->state->i.mode = iinternal_state::BAD;
        z->msg = (char*)"unknown compression method";
        z->state->i.sub.marker = 5;       /* can't try inflateSync */
        break;
      }
      if ((z->state->i.sub.method >> 4) + 8 > z->state->i.wbits)
      {
        z->state->i.mode = iinternal_state::BAD;
        z->msg = (char*)"invalid window size";
        z->state->i.sub.marker = 5;       /* can't try inflateSync */
        break;
      }
      z->state->i.mode = iinternal_state::FLAG;
    case iinternal_state::FLAG:
      xNEEDBYTE
      b = xNEXTBYTE;
      if (((z->state->i.sub.method << 8) + b) % 31)
      {
        z->state->i.mode = iinternal_state::BAD;
        z->msg = (char*)"incorrect header check";
        z->state->i.sub.marker = 5;       /* can't try inflateSync */
        break;
      }
      Trace((stderr, "inflate: zlib header ok\n"));
      if (!(b & PRESET_DICT))
      {
        z->state->i.mode = iinternal_state::BLOCKS;
   break;
      }
      z->state->i.mode = iinternal_state::DICT4;
    case iinternal_state::DICT4:
      xNEEDBYTE
      z->state->i.sub.check.need = (uLong)xNEXTBYTE << 24;
      z->state->i.mode = iinternal_state::DICT3;
    case iinternal_state::DICT3:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE << 16;
      z->state->i.mode = iinternal_state::DICT2;
    case iinternal_state::DICT2:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE << 8;
      z->state->i.mode = iinternal_state::DICT1;
    case iinternal_state::DICT1:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE;
      z->adler = z->state->i.sub.check.need;
      z->state->i.mode = iinternal_state::DICT0;
      return Z_NEED_DICT;
    case iinternal_state::DICT0:
      z->state->i.mode = iinternal_state::BAD;
      z->msg = (char*)"need dictionary";
      z->state->i.sub.marker = 0;       /* can try inflateSync */
      return Z_STREAM_ERROR;
    case iinternal_state::BLOCKS:
      r = inflate_blocks(z->state->i.blocks, z, r);
      if (r == Z_DATA_ERROR)
      {
        z->state->i.mode = iinternal_state::BAD;
        z->state->i.sub.marker = 0;       /* can try inflateSync */
        break;
      }
      if (r != Z_STREAM_END)
        return r;
      r = Z_OK;
      inflate_blocks_reset(z->state->i.blocks, z, &z->state->i.sub.check.was);
      if (z->state->i.nowrap)
      {
        z->state->i.mode = iinternal_state::DONE;
        break;
      }
      z->state->i.mode = iinternal_state::CHECK4;
    case iinternal_state::CHECK4:
      xNEEDBYTE
      z->state->i.sub.check.need = (uLong)xNEXTBYTE << 24;
      z->state->i.mode = iinternal_state::CHECK3;
    case iinternal_state::CHECK3:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE << 16;
      z->state->i.mode = iinternal_state::CHECK2;
    case iinternal_state::CHECK2:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE << 8;
      z->state->i.mode = iinternal_state::CHECK1;
    case iinternal_state::CHECK1:
      xNEEDBYTE
      z->state->i.sub.check.need += (uLong)xNEXTBYTE;

      if (z->state->i.sub.check.was != z->state->i.sub.check.need)
      {
        z->state->i.mode = iinternal_state::BAD;
        z->msg = (char*)"incorrect data check";
        z->state->i.sub.marker = 5;       /* can't try inflateSync */
        break;
      }
      Trace((stderr, "inflate: zlib check ok\n"));
      z->state->i.mode = iinternal_state::DONE;
    case iinternal_state::DONE:
      return Z_STREAM_END;
    case iinternal_state::BAD:
      return Z_DATA_ERROR;
    default:
      return Z_STREAM_ERROR;
  }
}


int inflateSetDictionary(z_stream *z, const Bytef *dictionary, uInt dictLength)
{
  uInt length = dictLength;

  if (z == 0 || z->state == 0 ||
      z->state->i.mode != iinternal_state::DICT0)
  {
    return Z_STREAM_ERROR;
  }

  if (adler32(1L, dictionary, dictLength) != z->adler) return Z_DATA_ERROR;
  z->adler = 1L;

  if (length >= (uInt)(1<<z->state->i.wbits))
  {
    length = (1<<z->state->i.wbits)-1;
    dictionary += dictLength - length;
  }
  inflate_set_dictionary(z->state->i.blocks, z, dictionary, length);
  z->state->i.mode = iinternal_state::BLOCKS;
  return Z_OK;
}


int inflateSync(z_stream *z)
{
  uInt n;       /* number of bytes to look at */
  Bytef *p;     /* pointer to bytes */
  uInt m;       /* number of marker bytes found in a row */
  uLong r, w;   /* temporaries to save total_in and total_out */

  /* set up */
  if (z == 0 || z->state == 0)
    return Z_STREAM_ERROR;
  if (z->state->i.mode != iinternal_state::BAD)
  {
    z->state->i.mode = iinternal_state::BAD;
    z->state->i.sub.marker = 0;
  }
  if ((n = z->avail_in) == 0)
    return Z_BUF_ERROR;
  p = z->next_in;
  m = z->state->i.sub.marker;

  /* search */
  while (n && m < 4)
  {
    if (*p == (Byte)(m < 2 ? 0 : 0xff))
      m++;
    else if (*p)
      m = 0;
    else
      m = 4 - m;
    p++, n--;
  }

  /* restore */
  z->total_in += p - z->next_in;
  z->next_in = p;
  z->avail_in = n;
  z->state->i.sub.marker = m;

  /* return no joy or set up to restart on a new block */
  if (m != 4)
    return Z_DATA_ERROR;
  r = z->total_in;  w = z->total_out;
  inflateReset(z);
  z->total_in = r;  z->total_out = w;
  z->state->i.mode = iinternal_state::BLOCKS;
  return Z_OK;
}

//===========================================================================


/* inflate_util.c -- data and routines common to blocks and codes
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */


//struct inflate_codes_state {int dummy;}; /* for buggy compilers */

/* And'ing with mask[n] masks the lower n bits */
uInt inflate_mask[17] = {
    0x0000,
    0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f, 0x007f, 0x00ff,
    0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff, 0xffff
};


/* copy as much as possible from the sliding window to the output area */
int
inflate_flush(inflate_blocks_statef *s,
              z_stream *z,
              int r)
{
  uInt n;
  Bytef *p, *q;

  /* local copies of source and destination pointers */
  p = z->next_out;
  q = s->read;

  /* compute number of bytes to copy as far as end of window */
  n = (uInt)((q <= s->write ? s->write : s->end) - q);
  if (n > z->avail_out) n = z->avail_out;
  if (n && r == Z_BUF_ERROR) r = Z_OK;

  /* update counters */
  z->avail_out -= n;
  z->total_out += n;

  /* update check information */
  if (s->checkfn != 0)
    z->adler = s->check = (*s->checkfn)(s->check, q, n);

  /* copy as far as end of window */
  zmemcpy(p, q, n);
  p += n;
  q += n;

  /* see if more to copy at beginning of window */
  if (q == s->end)
  {
    /* wrap pointers */
    q = s->window;
    if (s->write == s->end)
      s->write = s->window;

    /* compute bytes to copy */
    n = (uInt)(s->write - q);
    if (n > z->avail_out) n = z->avail_out;
    if (n && r == Z_BUF_ERROR) r = Z_OK;

    /* update counters */
    z->avail_out -= n;
    z->total_out += n;

    /* update check information */
    if (s->checkfn != 0)
      z->adler = s->check = (*s->checkfn)(s->check, q, n);

    /* copy */
    zmemcpy(p, q, n);
    p += n;
    q += n;
  }

  /* update pointers */
  z->next_out = p;
  s->read = q;

  /* done */
  return r;
}
/* infblock.c -- interpret and process block types to last block
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

//#include "zutil.h"

//struct inflate_codes_state {int dummy;}; /* for buggy compilers */

/* Table for deflate from PKZIP's appnote.txt. */
local uInt border[] = { /* Order of the bit length code lengths */
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

/*
   Notes beyond the 1.93a appnote.txt:

   1. Distance pointers never point before the beginning of the output
      stream.
   2. Distance pointers can point back across blocks, up to 32k away.
   3. There is an implied maximum of 7 bits for the bit length table and
      15 bits for the actual data.
   4. If only one code exists, then it is encoded using one bit.  (Zero
      would be more efficient, but perhaps a little confusing.)  If two
      codes exist, they are coded using one bit each (0 and 1).
   5. There is no way of sending zero distance codes--a dummy must be
      sent if there are none.  (History: a pre 2.0 version of PKZIP would
      store blocks with no distance codes, but this was discovered to be
      too harsh a criterion.)  Valid only for 1.93a.  2.04c does allow
      zero distance codes, which is sent as one code of zero bits in
      length.
   6. There are up to 286 literal/length codes.  Code 256 represents the
      end-of-block.  Note however that the static length tree defines
      288 codes just to fill out the Huffman codes.  Codes 286 and 287
      cannot be used though, since there is no length base or extra bits
      defined for them.  Similarily, there are up to 30 distance codes.
      However, static trees define 32 codes (all 5 bits) to fill out the
      Huffman codes, but the last two had better not show up in the data.
   7. Unzip can check dynamic Huffman blocks for complete code sets.
      The exception is that a single code would not be complete (see #4).
   8. The five bits following the block type is really the number of
      literal codes sent minus 257.
   9. Length codes 8,16,16 are interpreted as 13 length codes of 8 bits
      (1+6+6).  Therefore, to output three times the length, you output
      three codes (1+1+1), whereas to output four times the same length,
      you only need two codes (1+3).  Hmm.
  10. In the tree reconstruction algorithm, Code = Code + Increment
      only if BitLength(i) is not zero.  (Pretty obvious.)
  11. Correction: 4 Bits: # of Bit Length codes - 4     (4 - 19)
  12. Note: length code 284 can represent 227-258, but length code 285
      really is 258.  The last length deserves its own, short code
      since it gets used a lot in very redundant files.  The length
      258 is special since 258 - 3 (the min match length) is 255.
  13. The literal/length and distance code bit lengths are read as a
      single stream of lengths.  It is possible (and advantageous) for
      a repeat code (16, 17, or 18) to go across the boundary between
      the two sets of lengths.
 */


void inflate_blocks_reset(inflate_blocks_statef *s, z_stream *z, uLongf *c)
{
  if (s->checkfn != 0)
    *c = s->check;
  if (s->mode == BTREE || s->mode == DTREE)
    ZFREE(z, s->sub.trees.blens);
  if (s->mode == CODES)
  {
    inflate_codes_free(s->sub.decode.codes, z);
    inflate_trees_free(s->sub.decode.td, z);
    inflate_trees_free(s->sub.decode.tl, z);
  }
  s->mode = TYPE;
  s->bitk = 0;
  s->bitb = 0;
  s->read = s->write = s->window;
  if (s->checkfn != 0)
    z->adler = s->check = (*s->checkfn)(0L, 0, 0);
  Trace((stderr, "inflate:   blocks reset\n"));
}


inflate_blocks_statef *inflate_blocks_new(z_stream *z, check_func c, uInt w)
{
  inflate_blocks_statef *s;

  if ((s = (inflate_blocks_statef *)ZALLOC
       (z,1,sizeof(struct inflate_blocks_state))) == 0)
    return s;
  if ((s->window = (Bytef *)ZALLOC(z, 1, w)) == 0)
  {
    ZFREE(z, s);
    return( 0 );
  }
  s->end = s->window + w;
  s->checkfn = c;
  s->mode = TYPE;
  Trace((stderr, "inflate:   blocks allocated\n"));
  inflate_blocks_reset(s, z, &s->check);
  return s;
}


int inflate_blocks(inflate_blocks_statef *s, z_stream *z, int r)
{
  uInt t;               /* temporary storage */
  uLong b;              /* bit buffer */
  uInt k;               /* bits in bit buffer */
  Bytef *p;             /* input data pointer */
  uInt n;               /* bytes available there */
  Bytef *q;             /* output window write pointer */
  uInt m;               /* bytes to end of window or read pointer */

  /* copy input/output information to locals (UPDATE macro restores) */
  LOAD

  /* process input based on current state */
  while (1) switch (s->mode)
  {
    case TYPE:
      NEEDBITS(3)
      t = (uInt)b & 7;
      s->last = t & 1;
      switch (t >> 1)
      {
        case 0:                         /* stored */
          Trace((stderr, "inflate:     stored block%s\n",
                 s->last ? " (last)" : ""));
          DUMPBITS(3)
          t = k & 7;                    /* go to byte boundary */
          DUMPBITS(t)
          s->mode = LENS;               /* get length of stored block */
          break;
        case 1:                         /* fixed */
          Trace((stderr, "inflate:     fixed codes block%s\n",
                 s->last ? " (last)" : ""));
          {
            uInt bl, bd;
            inflate_huft *tl, *td;

            inflate_trees_fixed(&bl, &bd, &tl, &td);
            s->sub.decode.codes = inflate_codes_new(bl, bd, tl, td, z);
            if (s->sub.decode.codes == 0)
            {
              r = Z_MEM_ERROR;
              LEAVE
            }
            s->sub.decode.tl = 0;       /* don't try to free these */
            s->sub.decode.td = 0;
          }
          DUMPBITS(3)
          s->mode = CODES;
          break;
        case 2:                         /* dynamic */
          Trace((stderr, "inflate:     dynamic codes block%s\n",
                 s->last ? " (last)" : ""));
          DUMPBITS(3)
          s->mode = TABLE;
          break;
        case 3:                         /* illegal */
          DUMPBITS(3)
          s->mode = BAD;
          z->msg = (char*)"invalid block type";
          r = Z_DATA_ERROR;
          LEAVE
      }
      break;
    case LENS:
      NEEDBITS(32)
      if ((((~b) >> 16) & 0xffff) != (b & 0xffff))
      {
        s->mode = BAD;
        z->msg = (char*)"invalid stored block lengths";
        r = Z_DATA_ERROR;
        LEAVE
      }
      s->sub.left = (uInt)b & 0xffff;
      b = k = 0;                      /* dump bits */
      Tracev((stderr, "inflate:       stored length %u\n", s->sub.left));
      s->mode = s->sub.left ? STORED : TYPE;
      break;
    case STORED:
      if (n == 0)
        LEAVE
      NEEDOUT
      t = s->sub.left;
      if (t > n) t = n;
      if (t > m) t = m;
      zmemcpy(q, p, t);
      p += t;  n -= t;
      q += t;  m -= t;
      if ((s->sub.left -= t) != 0)
        break;
      Tracev((stderr, "inflate:       stored end, %lu total out\n",
              z->total_out + (q >= s->read ? q - s->read :
              (s->end - s->read) + (q - s->window))));
      s->mode = s->last ? DRY : TYPE;
      break;
    case TABLE:
      NEEDBITS(14)
      s->sub.trees.table = t = (uInt)b & 0x3fff;
#ifndef PKZIP_BUG_WORKAROUND
      if ((t & 0x1f) > 29 || ((t >> 5) & 0x1f) > 29)
      {
        s->mode = BAD;
        z->msg = (char*)"too many length or distance symbols";
        r = Z_DATA_ERROR;
        LEAVE
      }
#endif
      t = 258 + (t & 0x1f) + ((t >> 5) & 0x1f);
      if (t < 19)
        t = 19;
      if ((s->sub.trees.blens = (uIntf*)ZALLOC(z, t, sizeof(uInt))) == 0)
      {
        r = Z_MEM_ERROR;
        LEAVE
      }
      DUMPBITS(14)
      s->sub.trees.index = 0;
      Tracev((stderr, "inflate:       table sizes ok\n"));
      s->mode = BTREE;
    case BTREE:
      while (s->sub.trees.index < 4 + (s->sub.trees.table >> 10))
      {
        NEEDBITS(3)
        s->sub.trees.blens[border[s->sub.trees.index++]] = (uInt)b & 7;
        DUMPBITS(3)
      }
      while (s->sub.trees.index < 19)
        s->sub.trees.blens[border[s->sub.trees.index++]] = 0;
      s->sub.trees.bb = 7;
      t = inflate_trees_bits(s->sub.trees.blens, &s->sub.trees.bb,
                             &s->sub.trees.tb, z);
      if (t != Z_OK)
      {
        r = t;
        if (r == Z_DATA_ERROR)
          s->mode = BAD;
        LEAVE
      }
      s->sub.trees.index = 0;
      Tracev((stderr, "inflate:       bits tree ok\n"));
      s->mode = DTREE;
    case DTREE:
      while (t = s->sub.trees.table,
             s->sub.trees.index < 258 + (t & 0x1f) + ((t >> 5) & 0x1f))
      {
        inflate_huft *h;
        uInt i, j, c;

        t = s->sub.trees.bb;
        NEEDBITS(t)
        h = s->sub.trees.tb + ((uInt)b & inflate_mask[t]);
        t = h->word.what.Bits;
        c = h->more.Base;
        if (c < 16)
        {
          DUMPBITS(t)
          s->sub.trees.blens[s->sub.trees.index++] = c;
        }
        else /* c == 16..18 */
        {
          i = c == 18 ? 7 : c - 14;
          j = c == 18 ? 11 : 3;
          NEEDBITS(t + i)
          DUMPBITS(t)
          j += (uInt)b & inflate_mask[i];
          DUMPBITS(i)
          i = s->sub.trees.index;
          t = s->sub.trees.table;
          if (i + j > 258 + (t & 0x1f) + ((t >> 5) & 0x1f) ||
              (c == 16 && i < 1))
          {
            s->mode = BAD;
            z->msg = (char*)"invalid bit length repeat";
            r = Z_DATA_ERROR;
            LEAVE
          }
          c = c == 16 ? s->sub.trees.blens[i - 1] : 0;
          do {
            s->sub.trees.blens[i++] = c;
          } while (--j);
          s->sub.trees.index = i;
        }
      }
      inflate_trees_free(s->sub.trees.tb, z);
      s->sub.trees.tb = 0;
      {
        uInt bl, bd;
        inflate_huft *tl, *td;
        inflate_codes_statef *c;

        bl = 9;         /* must be <= 9 for lookahead assumptions */
        bd = 6;         /* must be <= 9 for lookahead assumptions */
        t = s->sub.trees.table;
        t = inflate_trees_dynamic(257 + (t & 0x1f), 1 + ((t >> 5) & 0x1f),
                                  s->sub.trees.blens, &bl, &bd, &tl, &td, z);
        if (t != Z_OK)
        {
          if (t == (uInt)Z_DATA_ERROR)
            s->mode = BAD;
          r = t;
          LEAVE
        }
        Tracev((stderr, "inflate:       trees ok\n"));
        if ((c = inflate_codes_new(bl, bd, tl, td, z)) == 0)
        {
          inflate_trees_free(td, z);
          inflate_trees_free(tl, z);
          r = Z_MEM_ERROR;
          LEAVE
        }
        ZFREE(z, s->sub.trees.blens);
        s->sub.decode.codes = c;
        s->sub.decode.tl = tl;
        s->sub.decode.td = td;
      }
      s->mode = CODES;
    case CODES:
      UPDATE
      if ((r = inflate_codes(s, z, r)) != Z_STREAM_END)
        return inflate_flush(s, z, r);
      r = Z_OK;
      inflate_codes_free(s->sub.decode.codes, z);
      inflate_trees_free(s->sub.decode.td, z);
      inflate_trees_free(s->sub.decode.tl, z);
      LOAD
      Tracev((stderr, "inflate:       codes end, %lu total out\n",
              z->total_out + (q >= s->read ? q - s->read :
              (s->end - s->read) + (q - s->window))));
      if (!s->last)
      {
        s->mode = TYPE;
        break;
      }
      if (k > 7)              /* return unused byte, if any */
      {
        Assert(k < 16, "inflate_codes grabbed too many bytes")
        k -= 8;
        n++;
        p--;                    /* can always return one */
      }
      s->mode = DRY;
    case DRY:
      FLUSH
      if (s->read != s->write)
        LEAVE
      s->mode = DONE;
    case DONE:
      r = Z_STREAM_END;
      LEAVE
    case BAD:
      r = Z_DATA_ERROR;
      LEAVE
    default:
      r = Z_STREAM_ERROR;
      LEAVE
  }
}


int inflate_blocks_free(inflate_blocks_statef *s, z_stream *z, uLongf *c)
{
  inflate_blocks_reset(s, z, c);
  ZFREE(z, s->window);
  ZFREE(z, s);
  Trace((stderr, "inflate:   blocks freed\n"));
  return Z_OK;
}


void inflate_set_dictionary(inflate_blocks_statef *s, z_stream *z,
                            const Bytef *d, uInt n)
{
  zmemcpy((charf *)s->window, d, n);
  s->read = s->write = s->window + n;
}
/* infcodes.c -- process literals and length/distance pairs
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

//#include "zutil.h"

/* simplify the use of the inflate_huft type with some defines */
#define base more.Base
#define next more.Next
#define exop word.what.Exop
#define bits word.what.Bits

/* inflate codes private state */
struct inflate_codes_state
{
  /* mode */
  enum
  {        /* waiting for "i:"=input, "o:"=output, "x:"=nothing */
      START,    /* x: set up for LEN */
      LEN,      /* i: get length/literal/eob next */
      LENEXT,   /* i: getting length extra (have base) */
      DIST,     /* i: get distance next */
      DISTEXT,  /* i: getting distance extra */
      COPY,     /* o: copying bytes in window, waiting for space */
      LIT,      /* o: got literal, waiting for output space */
      WASH,     /* o: got eob, possibly still output waiting */
      END,      /* x: got eob and all data flushed */
      BADCODE   /* x: got error */
  }  mode;               /* current inflate_codes mode */

  /* mode dependent information */
  uInt len;
  union {
    struct {
      inflate_huft *tree;       /* pointer into tree */
      uInt need;                /* bits needed */
    } code;             /* if LEN or DIST, where in tree */
    uInt lit;           /* if LIT, literal */
    struct {
      uInt get;                 /* bits to get for extra */
      uInt dist;                /* distance back to copy from */
    } copy;             /* if EXT or COPY, where and how much */
  } sub;                /* submode */

  /* mode independent information */
  Byte lbits;           /* ltree bits decoded per branch */
  Byte dbits;           /* dtree bits decoder per branch */
  inflate_huft *ltree;          /* literal/length/eob tree */
  inflate_huft *dtree;          /* distance tree */
};


inflate_codes_statef *
inflate_codes_new(uInt bl, uInt bd,
                  inflate_huft *tl,
                  inflate_huft *td,
                  z_stream *z)
{
  inflate_codes_statef *c;

  if ((c = (inflate_codes_statef *)
       ZALLOC(z,1,sizeof(struct inflate_codes_state))) != 0)
  {
     c->mode = inflate_codes_state::START;
     c->lbits = (Byte)bl;
     c->dbits = (Byte)bd;
     c->ltree = tl;
     c->dtree = td;
     Tracev((stderr, "inflate:       codes new\n"));
  }
  return c;
}


int inflate_codes(inflate_blocks_statef *s, z_stream *z, int r)
{
  uInt j;               /* temporary storage */
  inflate_huft *t;      /* temporary pointer */
  uInt e;               /* extra bits or operation */
  uLong b;              /* bit buffer */
  uInt k;               /* bits in bit buffer */
  Bytef *p;             /* input data pointer */
  uInt n;               /* bytes available there */
  Bytef *q;             /* output window write pointer */
  uInt m;               /* bytes to end of window or read pointer */
  Bytef *f;             /* pointer to copy strings from */
  inflate_codes_statef *c = s->sub.decode.codes;  /* codes state */

  /* copy input/output information to locals (UPDATE macro restores) */
  LOAD

  /* process input and output based on current state */
  while (1) switch (c->mode)
  {             /* waiting for "i:"=input, "o:"=output, "x:"=nothing */
    case inflate_codes_state::START:         /* x: set up for LEN */
#ifndef SLOW
      if (m >= 258 && n >= 10)
      {
        UPDATE
        r = inflate_fast(c->lbits, c->dbits, c->ltree, c->dtree, s, z);
        LOAD
        if (r != Z_OK)
        {
          c->mode = r == Z_STREAM_END ? inflate_codes_state::WASH :
                                        inflate_codes_state::BADCODE;
          break;
        }
      }
#endif /* !SLOW */
      c->sub.code.need = c->lbits;
      c->sub.code.tree = c->ltree;
      c->mode = inflate_codes_state::LEN;
    case inflate_codes_state::LEN:           /* i: get length/literal/eob next */
      j = c->sub.code.need;
      NEEDBITS(j)
      t = c->sub.code.tree + ((uInt)b & inflate_mask[j]);
      DUMPBITS(t->bits)
      e = (uInt)(t->exop);
      if (e == 0)               /* literal */
      {
        c->sub.lit = t->base;
        Tracevv((stderr, t->base >= 0x20 && t->base < 0x7f ?
                 "inflate:         literal '%c'\n" :
                 "inflate:         literal 0x%02x\n", t->base));
        c->mode = inflate_codes_state::LIT;
        break;
      }
      if (e & 16)               /* length */
      {
        c->sub.copy.get = e & 15;
        c->len = t->base;
        c->mode = inflate_codes_state::LENEXT;
        break;
      }
      if ((e & 64) == 0)        /* next table */
      {
        c->sub.code.need = e;
        c->sub.code.tree = t->next;
        break;
      }
      if (e & 32)               /* end of block */
      {
        Tracevv((stderr, "inflate:         end of block\n"));
        c->mode = inflate_codes_state::WASH;
        break;
      }
      c->mode = inflate_codes_state::BADCODE;        /* invalid code */
      z->msg = (char *) "invalid literal/length code";
      r = Z_DATA_ERROR;
      LEAVE
    case inflate_codes_state::LENEXT:        /* i: getting length extra (have base) */
      j = c->sub.copy.get;
      NEEDBITS(j)
      c->len += (uInt)b & inflate_mask[j];
      DUMPBITS(j)
      c->sub.code.need = c->dbits;
      c->sub.code.tree = c->dtree;
      Tracevv((stderr, "inflate:         length %u\n", c->len));
      c->mode = inflate_codes_state::DIST;
    case inflate_codes_state::DIST:          /* i: get distance next */
      j = c->sub.code.need;
      NEEDBITS(j)
      t = c->sub.code.tree + ((uInt)b & inflate_mask[j]);
      DUMPBITS(t->bits)
      e = (uInt)(t->exop);
      if (e & 16)               /* distance */
      {
        c->sub.copy.get = e & 15;
        c->sub.copy.dist = t->base;
        c->mode = inflate_codes_state::DISTEXT;
        break;
      }
      if ((e & 64) == 0)        /* next table */
      {
        c->sub.code.need = e;
        c->sub.code.tree = t->next;
        break;
      }
      c->mode = inflate_codes_state::BADCODE;  /* invalid code */
      z->msg = (char*)"invalid distance code";
      r = Z_DATA_ERROR;
      LEAVE
    case inflate_codes_state::DISTEXT: /* i: getting distance extra */
      j = c->sub.copy.get;
      NEEDBITS(j)
      c->sub.copy.dist += (uInt)b & inflate_mask[j];
      DUMPBITS(j)
      Tracevv((stderr, "inflate:         distance %u\n", c->sub.copy.dist));
      c->mode = inflate_codes_state::COPY;
    case inflate_codes_state::COPY:   /* o: copying bytes in window, waiting for space */
      f = (uInt)(q - s->window) < c->sub.copy.dist ?
          s->end - (c->sub.copy.dist - (q - s->window)) :
          q - c->sub.copy.dist;
      while (c->len)
      {
        NEEDOUT
        OUTBYTE(*f++)
        if (f == s->end)
          f = s->window;
        c->len--;
      }
      c->mode = inflate_codes_state::START;
      break;
    case inflate_codes_state::LIT:   /* o: got literal, waiting for output space */
      NEEDOUT
      OUTBYTE(c->sub.lit)
      c->mode = inflate_codes_state::START;
      break;
    case inflate_codes_state::WASH:  /* o: got eob, possibly more output */
      FLUSH
      if (s->read != s->write)
        LEAVE
      c->mode = inflate_codes_state::END;
    case inflate_codes_state::END:
      r = Z_STREAM_END;
      LEAVE
    case inflate_codes_state::BADCODE:       /* x: got error */
      r = Z_DATA_ERROR;
      LEAVE
    default:
      r = Z_STREAM_ERROR;
      LEAVE
  }
}


void
inflate_codes_free(inflate_codes_statef *c, z_stream *z)
{
  ZFREE(z, c);
  Tracev((stderr, "inflate:       codes free\n"));
}
/* inffast.c -- process literals and length/distance pairs fast
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

//#include "zutil.h"

//struct inflate_codes_state {int dummy;}; /* for buggy compilers */

/* simplify the use of the inflate_huft type with some defines */
#define base more.Base
#define next more.Next
#define exop word.what.Exop
#define bits word.what.Bits

/* macros for bit input with no checking and for returning unused bytes */
#define GRABBITS(j) {while (k<(j)){b|=((uLong)NEXTBYTE)<<k;k+=8;}}
#define UNGRAB {n+=(c=k>>3);p-=c;k&=7;}

/* Called with number of bytes left to write in window at least 258
   (the maximum string length) and number of input bytes available
   at least ten.  The ten bytes are six bytes for the longest length/
   distance pair plus four bytes for overloading the bit buffer. */

int inflate_fast(uInt bl, uInt bd, inflate_huft *tl,
                 inflate_huft *td, inflate_blocks_statef *s,
                 z_stream *z)
{
  inflate_huft *t;      /* temporary pointer */
  uInt e;               /* extra bits or operation */
  uLong b;              /* bit buffer */
  uInt k;               /* bits in bit buffer */
  Bytef *p;             /* input data pointer */
  uInt n;               /* bytes available there */
  Bytef *q;             /* output window write pointer */
  uInt m;               /* bytes to end of window or read pointer */
  uInt ml;              /* mask for literal/length tree */
  uInt md;              /* mask for distance tree */
  uInt c;               /* bytes to copy */
  uInt d;               /* distance back to copy from */
  Bytef *r;             /* copy source pointer */

  /* load input, output, bit values */
  LOAD

  /* initialize masks */
  ml = inflate_mask[bl];
  md = inflate_mask[bd];

  /* do until not enough input or output space for fast loop */
  do {                          /* assume called with m >= 258 && n >= 10 */
    /* get literal/length code */
    GRABBITS(20)                /* max bits for literal/length code */
    if ((e = (t = tl + ((uInt)b & ml))->exop) == 0)
    {
      DUMPBITS(t->bits)
      Tracevv((stderr, t->base >= 0x20 && t->base < 0x7f ?
                "inflate:         * literal '%c'\n" :
                "inflate:         * literal 0x%02x\n", t->base));
      *q++ = (Byte)t->base;
      m--;
      continue;
    }
    do {
      DUMPBITS(t->bits)
      if (e & 16)
      {
        /* get extra bits for length */
        e &= 15;
        c = t->base + ((uInt)b & inflate_mask[e]);
        DUMPBITS(e)
        Tracevv((stderr, "inflate:         * length %u\n", c));

        /* decode distance base of block to copy */
        GRABBITS(15);           /* max bits for distance code */
        e = (t = td + ((uInt)b & md))->exop;
        do {
          DUMPBITS(t->bits)
          if (e & 16)
          {
            /* get extra bits to add to distance base */
            e &= 15;
            GRABBITS(e)         /* get extra bits (up to 13) */
            d = t->base + ((uInt)b & inflate_mask[e]);
            DUMPBITS(e)
            Tracevv((stderr, "inflate:         * distance %u\n", d));

            /* do the copy */
            m -= c;
            if ((uInt)(q - s->window) >= d)     /* offset before dest */
            {                                   /*  just copy */
              r = q - d;
              *q++ = *r++;  c--;        /* minimum count is three, */
              *q++ = *r++;  c--;        /*  so unroll loop a little */
            }
            else                        /* else offset after destination */
            {
              e = d - (uInt)(q - s->window); /* bytes from offset to end */
              r = s->end - e;           /* pointer to offset */
              if (c > e)                /* if source crosses, */
              {
                c -= e;                 /* copy to end of window */
                do {
                  *q++ = *r++;
                } while (--e);
                r = s->window;          /* copy rest from start of window */
              }
            }
            do {                        /* copy all or what's left */
              *q++ = *r++;
            } while (--c);
            break;
          }
          else if ((e & 64) == 0)
            e = (t = t->next + ((uInt)b & inflate_mask[e]))->exop;
          else
          {
            z->msg = (char*)"invalid distance code";
            UNGRAB
            UPDATE
            return Z_DATA_ERROR;
          }
        } while (1);
        break;
      }
      if ((e & 64) == 0)
      {
        if ((e = (t = t->next + ((uInt)b & inflate_mask[e]))->exop) == 0)
        {
          DUMPBITS(t->bits)
          Tracevv((stderr, t->base >= 0x20 && t->base < 0x7f ?
                    "inflate:         * literal '%c'\n" :
                    "inflate:         * literal 0x%02x\n", t->base));
          *q++ = (Byte)t->base;
          m--;
          break;
        }
      }
      else if (e & 32)
      {
        Tracevv((stderr, "inflate:         * end of block\n"));
        UNGRAB
        UPDATE
        return Z_STREAM_END;
      }
      else
      {
        z->msg = (char*)"invalid literal/length code";
        UNGRAB
        UPDATE
        return Z_DATA_ERROR;
      }
    } while (1);
  } while (m >= 258 && n >= 10);

  /* not enough input or output--restore pointers and return */
  UNGRAB
  UPDATE
  return Z_OK;
}
/* inftrees.c -- generate Huffman trees for efficient decoding
 * Copyright (C) 1995-1996 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h
 */

//#include "zutil.h"

char inflate_copyright[] = " inflate 1.0 Copyright 1995-1996 Mark Adler ";
/*
  If you use the zlib library in a product, an acknowledgment is welcome
  in the documentation of your product. If for some reason you cannot
  include such an acknowledgment, I would appreciate that you keep this
  copyright string in the executable of your product.
 */
// struct dinternal_state {int dummy;}; /* for buggy compilers */

/* simplify the use of the inflate_huft type with some defines */
#define base more.Base
#define next more.Next
#define exop word.what.Exop
#define bits word.what.Bits


local int huft_build OF((
    uIntf *,            /* code lengths in bits */
    uInt,               /* number of codes */
    uInt,               /* number of "simple" codes */
    uIntf *,            /* list of base values for non-simple codes */
    uIntf *,            /* list of extra bits for non-simple codes */
    inflate_huft * FAR*,/* result: starting table */
    uIntf *,            /* maximum lookup bits (returns actual) */
    z_stream *));       /* for zalloc function */

local voidpf falloc OF((
    voidpf,             /* opaque pointer (not used) */
    uInt,               /* number of items */
    uInt));             /* size of item */

/* Tables for deflate from PKZIP's appnote.txt. */
local uInt cplens[31] = { /* Copy lengths for literal codes 257..285 */
        3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258, 0, 0};
        /* actually lengths - 2; also see note #13 above about 258 */
local uInt cplext[31] = { /* Extra bits for literal codes 257..285 */
        0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
        3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0, 192, 192}; /* 192==invalid */
local uInt cpdist[30] = { /* Copy offsets for distance codes 0..29 */
        1, 2, 3, 4, 5, 7, 9, 13, 17, 25, zTAG_LTH, 49, 65, 97, 129, 193,
        257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145,
        8193, 12289, 16385, 24577};
local uInt cpdext[30] = { /* Extra bits for distance codes */
        0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6,
        7, 7, 8, 8, 9, 9, 10, 10, 11, 11,
        12, 12, 13, 13};

/*
   Huffman code decoding is performed using a multi-level table lookup.
   The fastest way to decode is to simply build a lookup table whose
   size is determined by the longest code.  However, the time it takes
   to build this table can also be a factor if the data being decoded
   is not very long.  The most common codes are necessarily the
   shortest codes, so those codes dominate the decoding time, and hence
   the speed.  The idea is you can have a shorter table that decodes the
   shorter, more probable codes, and then point to subsidiary tables for
   the longer codes.  The time it costs to decode the longer codes is
   then traded against the time it takes to make longer tables.

   This results of this trade are in the variables lbits and dbits
   below.  lbits is the number of bits the first level table for literal/
   length codes can decode in one step, and dbits is the same thing for
   the distance codes.  Subsequent tables are also less than or equal to
   those sizes.  These values may be adjusted either when all of the
   codes are shorter than that, in which case the longest code length in
   bits is used, or when the shortest code is *longer* than the requested
   table size, in which case the length of the shortest code in bits is
   used.

   There are two different values for the two tables, since they code a
   different number of possibilities each.  The literal/length table
   codes 286 possible values, or in a flat code, a little over eight
   bits.  The distance table codes 30 possible values, or a little less
   than five bits, flat.  The optimum values for speed end up being
   about one bit more than those, so lbits is 8+1 and dbits is 5+1.
   The optimum values may differ though from machine to machine, and
   possibly even between compilers.  Your mileage may vary.
 */


/* If BMAX needs to be larger than 16, then h and x[] should be uLong. */
#define BMAX 15         /* maximum bit length of any code */
#define N_MAX 288       /* maximum number of codes in any set */

#ifdef DEBUG
  uInt inflate_hufts;
#endif

local int
huft_build(uIntf *b,      // code lengths in bits (all assumed <= BMAX)
           uInt n,        // number of codes (assumed <= N_MAX)
           uInt s,        // number of simple-valued codes (0..s-1)
           uIntf *d,      // list of base values for non-simple codes
           uIntf *e,      // list of extra bits for non-simple codes
           inflate_huft * FAR *t, // result: starting table
           uIntf *m,      // maximum lookup bits, returns actual
           z_stream *zs)  // for zalloc function
/* Given a list of code lengths and a maximum table size, make a set of
   tables to decode that set of codes.  Return Z_OK on success, Z_BUF_ERROR
   if the given code set is incomplete (the tables are still built in this
   case), Z_DATA_ERROR if the input is invalid (all zero length codes or an
   over-subscribed set of lengths), or Z_MEM_ERROR if not enough memory. */
{

  uInt a;                       /* counter for codes of length k */
  uInt c[BMAX+1];               /* bit length count table */
  uInt f;                       /* i repeats in table every f entries */
  int g;                        /* maximum code length */
  int h;                        /* table level */
  register uInt i;              /* counter, current code */
  register uInt j;              /* counter */
  register int k;               /* number of bits in current code */
  int l;                        /* bits per table (returned in m) */
  register uIntf *p;            /* pointer into c[], b[], or v[] */
  inflate_huft *q;              /* points to current table */
  struct inflate_huft_s r;      /* table entry for structure assignment */
  inflate_huft *u[BMAX];        /* table stack */
  uInt v[N_MAX];                /* values in order of bit length */
  register int w;               /* bits before this table == (l * h) */
  uInt x[BMAX+1];               /* bit offsets, then code stack */
  uIntf *xp;                    /* pointer into x */
  int y;                        /* number of dummy codes added */
  uInt z;                       /* number of entries in current table */


  /* Generate counts for each bit length */
  p = c;
#define C0 *p++ = 0;
#define C2 C0 C0 C0 C0
#define C4 C2 C2 C2 C2
  C4                            /* clear c[]--assume BMAX+1 is 16 */
  p = b;  i = n;
  do {
    c[*p++]++;                  /* assume all entries <= BMAX */
  } while (--i);
  if (c[0] == n)                /* null input--all zero length codes */
  {
    *t = (inflate_huft *)0;
    *m = 0;
    return Z_OK;
  }


  /* Find minimum and maximum length, bound *m by those */
  l = *m;
  for (j = 1; j <= BMAX; j++)
    if (c[j])
      break;
  k = j;                        /* minimum code length */
  if ((uInt)l < j)
    l = j;
  for (i = BMAX; i; i--)
    if (c[i])
      break;
  g = i;                        /* maximum code length */
  if ((uInt)l > i)
    l = i;
  *m = l;


  /* Adjust last length count to fill out codes, if needed */
  for (y = 1 << j; j < i; j++, y <<= 1)
    if ((y -= c[j]) < 0)
      return Z_DATA_ERROR;
  if ((y -= c[i]) < 0)
    return Z_DATA_ERROR;
  c[i] += y;


  /* Generate starting offsets into the value table for each length */
  x[1] = j = 0;
  p = c + 1;  xp = x + 2;
  while (--i) {                 /* note that i == g from above */
    *xp++ = (j += *p++);
  }


  /* Make a table of values in order of bit lengths */
  p = b;  i = 0;
  do {
    if ((j = *p++) != 0)
      v[x[j]++] = i;
  } while (++i < n);


  /* Generate the Huffman codes and for each, make the table entries */
  x[0] = i = 0;                 /* first Huffman code is zero */
  p = v;                        /* grab values in bit order */
  h = -1;                       /* no tables yet--level -1 */
  w = -l;                       /* bits decoded == (l * h) */
  u[0] = (inflate_huft *)0;             /* just to keep compilers happy */
  q = (inflate_huft *)0;        /* ditto */
  z = 0;                        /* ditto */

  /* go through the bit lengths (k already is bits in shortest code) */
  for (; k <= g; k++)
  {
    a = c[k];
    while (a--)
    {
      /* here i is the Huffman code of length k bits for value *p */
      /* make tables up to required level */
      while (k > w + l)
      {
        h++;
        w += l;                 /* previous table always l bits */

        /* compute minimum size table less than or equal to l bits */
        z = (z = g - w) > (uInt)l ? l : z;      /* table size upper limit */
        if ((f = 1 << (j = k - w)) > a + 1)     /* try a k-w bit table */
        {                       /* too few codes for k-w bit table */
          f -= a + 1;           /* deduct codes from patterns left */
          xp = c + k;
          if (j < z)
            while (++j < z)     /* try smaller tables up to z bits */
            {
              if ((f <<= 1) <= *++xp)
                break;          /* enough codes to use up j bits */
              f -= *xp;         /* else deduct codes from patterns */
            }
        }
        z = 1 << j;             /* table entries for j-bit table */

        /* allocate and link in new table */
        if ((q = (inflate_huft *)ZALLOC
             (zs,z + 1,sizeof(inflate_huft))) == 0)
        {
          if (h)
            inflate_trees_free(u[0], zs);
          return Z_MEM_ERROR;   /* not enough memory */
        }
#ifdef DEBUG
        inflate_hufts += z + 1;
#endif
        *t = q + 1;             /* link to list for huft_free() */
        *(t = &(q->next)) = 0;
        u[h] = ++q;             /* table starts after link */

        /* connect to last table, if there is one */
        if (h)
        {
          x[h] = i;             /* save pattern for backing up */
          r.bits = (Byte)l;     /* bits to dump before this table */
          r.exop = (Byte)j;     /* bits in this table */
          r.next = q;           /* pointer to this table */
          j = i >> (w - l);     /* (get around Turbo C bug) */
          u[h-1][j] = r;        /* connect to last table */
        }
      }

      /* set up table entry in r */
      r.bits = (Byte)(k - w);
      if (p >= v + n)
        r.exop = 128 + 64;      /* out of values--invalid code */
      else if (*p < s)
      {
        r.exop = (Byte)(*p < 256 ? 0 : 32 + 64);     /* 256 is end-of-block */
        r.base = *p++;          /* simple code is just the value */
      }
      else
      {
        r.exop = (Byte)e[*p - s] + 16 + 64; /* non-simple--look up in lists */
        r.base = d[*p++ - s];
      }

      /* fill code-like entries with r */
      f = 1 << (k - w);
      for (j = i >> w; j < z; j += f)
        q[j] = r;

      /* backwards increment the k-bit code i */
      for (j = 1 << (k - 1); i & j; j >>= 1)
        i ^= j;
      i ^= j;

      /* backup over finished tables */
      while ((i & ((1 << w) - 1)) != x[h])
      {
        h--;                    /* don't need to update q */
        w -= l;
      }
    }
  }


  /* Return Z_BUF_ERROR if we were given an incomplete table */
  return y != 0 && g != 1 ? Z_BUF_ERROR : Z_OK;
}


int
inflate_trees_bits(uIntf *c,      // 19 code lengths/
                   uIntf *bb,     // bits tree desired/actual depth/
                   inflate_huft * FAR *tb, // bits tree result/
                   z_stream *z)   // for zfree function/
{
  int r;

  r = huft_build(c, 19, 19, (uIntf*)0, (uIntf*)0, tb, bb, z);
  if (r == Z_DATA_ERROR)
    z->msg = (char*)"oversubscribed dynamic bit lengths tree";
  else if (r == Z_BUF_ERROR)
  {
    inflate_trees_free(*tb, z);
    z->msg = (char*)"incomplete dynamic bit lengths tree";
    r = Z_DATA_ERROR;
  }
  return r;
}


int inflate_trees_dynamic(uInt nl,     // number of literal/length codes
                          uInt nd,     // number of distance codes
                          uIntf *c,    // that many (total) code lengths
                          uIntf *bl,   // literal desired/actual bit depth
                          uIntf *bd,   // distance desired/actual bit depth
                          inflate_huft * FAR *tl, // literal/length tree result
                          inflate_huft * FAR *td, // distance tree result
                          z_stream *z) // for zfree function
{
  int r;

  /* build literal/length tree */
  if ((r = huft_build(c, nl, 257, cplens, cplext, tl, bl, z)) != Z_OK)
  {
    if (r == Z_DATA_ERROR)
      z->msg = (char*)"oversubscribed literal/length tree";
    else if (r == Z_BUF_ERROR)
    {
      inflate_trees_free(*tl, z);
      z->msg = (char*)"incomplete literal/length tree";
      r = Z_DATA_ERROR;
    }
    return r;
  }

  /* build distance tree */
  if ((r = huft_build(c + nl, nd, 0, cpdist, cpdext, td, bd, z)) != Z_OK)
  {
    if (r == Z_DATA_ERROR)
      z->msg = (char*)"oversubscribed literal/length tree";
    else if (r == Z_BUF_ERROR) {
#ifdef PKZIP_BUG_WORKAROUND
      r = Z_OK;
    }
#else
      inflate_trees_free(*td, z);
      z->msg = (char*)"incomplete literal/length tree";
      r = Z_DATA_ERROR;
    }
    inflate_trees_free(*tl, z);
    return r;
#endif
  }

  /* done */
  return Z_OK;
}


/* build fixed tables only once--keep them here */
local int fixed_built = 0;
#define FIXEDH 530      /* number of hufts used by fixed tables */
local inflate_huft fixed_mem[FIXEDH];
local uInt fixed_bl;
local uInt fixed_bd;
local inflate_huft *fixed_tl;
local inflate_huft *fixed_td;


local voidpf
falloc(voidpf q,  // opaque pointer
       uInt n,    // number of items
       uInt s)    // size of item
{
  Assert(s == sizeof(inflate_huft) && n <= (uInt) *(intf *)q,
         "inflate_trees falloc overflow");
  *(intf *)q -= n+s-s; /* s-s to avoid warning */
  return (voidpf)(fixed_mem + *(intf *)q);
}


int
inflate_trees_fixed(uIntf *bl,     // literal desired/actual bit depth
                    uIntf *bd,     // distance desired/actual bit depth
                    inflate_huft * FAR *tl,  // literal/length tree result
                    inflate_huft * FAR *td)  // distance tree result
{
  /* build fixed tables if not already (multiple overlapped executions ok) */
  if (!fixed_built)
  {
    int k;              /* temporary variable */
    unsigned c[288];    /* length list for huft_build */
    z_stream z;         /* for falloc function */
    int f = FIXEDH;     /* number of hufts left in fixed_mem */

    /* set up fake z_stream for memory routines */
    z.zalloc = falloc;
    z.zfree = 0;
    z.opaque = (voidpf)&f;

    /* literal table */
    for (k = 0; k < 144; k++)
      c[k] = 8;
    for (; k < 256; k++)
      c[k] = 9;
    for (; k < 280; k++)
      c[k] = 7;
    for (; k < 288; k++)
      c[k] = 8;
    fixed_bl = 7;
    huft_build(c, 288, 257, cplens, cplext, &fixed_tl, &fixed_bl, &z);

    /* distance table */
    for (k = 0; k < 30; k++)
      c[k] = 5;
    fixed_bd = 5;
    huft_build(c, 30, 0, cpdist, cpdext, &fixed_td, &fixed_bd, &z);

    /* done */
    Assert(f == 0, "invalid build of fixed tables");
    fixed_built = 1;
  }
  *bl = fixed_bl;
  *bd = fixed_bd;
  *tl = fixed_tl;
  *td = fixed_td;
  return Z_OK;
}


int inflate_trees_free(inflate_huft *t, // table to free
                       z_stream *z)     // for zfree function
/* Free the malloc'ed tables built by huft_build(), which makes a linked
   list of the tables it made, with the links in a dummy first entry of
   each table. */
{
  register inflate_huft *p, *q, *r;

  /* Reverse linked list */
  p = 0;
  q = t;
  while (q != 0)
  {
    r = (q - 1)->next;
    (q - 1)->next = p;
    p = q;
    q = r;
  }
  /* Go through linked list, freeing from the malloced (t[-1]) address. */
  while (p != 0)
  {
    q = (--p)->next;
    ZFREE(z,p);
    p = q;
  }
  return Z_OK;
}


////////////////////////////////// TREES.CPP ////////////////////////////////
////////////////////////////////// TREES.CPP ////////////////////////////////
////////////////////////////////// TREES.CPP ////////////////////////////////
////////////////////////////////// TREES.CPP ////////////////////////////////

// trees.c -- output deflated data using Huffman coding
// Copyright (C) 1995-1996 Jean-loup Gailly
// For conditions of distribution and use, see copyright notice in zlib.h

//  ALGORITHM
//
//      The "deflation" process uses several Huffman trees. The more
//      common source values are represented by shorter bit sequences.
//
//      Each code tree is stored in a compressed form which is itself
// a Huffman encoding of the lengths of all the code strings (in
// ascending order by source values).  The actual code strings are
// reconstructed from the lengths in the inflate process, as described
// in the deflate specification.
//
//  REFERENCES
//
//      Deutsch, L.P.,"'Deflate' Compressed Data Format Specification".
//      Available in ftp.uu.net:/pub/archiving/zip/doc/deflate-1.1.doc
//
//      Storer, James A.
//          Data Compression:  Methods and Theory, pp. 49-50.
//          Computer Science Press, 1988.  ISBN 0-7167-8156-5.
//
//      Sedgewick, R.
//          Algorithms, p290.
//          Addison-Wesley, 1983. ISBN 0-201-06672-6.
//

/* $Id: trees.c,v 1.5 1995/05/03 17:27:12 jloup Exp $ */

//#include "zlib.h"

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

#define MAX_BL_BITS 7
/* Bit length codes must not exceed MAX_BL_BITS bits */

#define END_BLOCK 256
/* end of block literal code */

#define REP_3_6      16
/* repeat previous bit length 3-6 times (2 bits of repeat count) */

#define REPZ_3_10    17
/* repeat a zero length 3-10 times  (3 bits of repeat count) */

#define REPZ_11_138  18
/* repeat a zero length 11-138 times  (7 bits of repeat count) */

local int extra_lbits[LENGTH_CODES] /* extra bits for each length code */
   = {0,0,0,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,0};

local int extra_dbits[D_CODES] /* extra bits for each distance code */
   = {0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11,11,12,12,13,13};

local int extra_blbits[BL_CODES]/* extra bits for each bit length code */
   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,3,7};

local uch bl_order[BL_CODES]
   = {16,17,18,0,8,7,9,6,10,5,11,4,12,3,13,2,14,1,15};
// The lengths of the bit length codes are sent in order of decreasing
// probability, to avoid transmitting the lengths for unused bit length codes.

#define Buf_size (8 * 2*sizeof(char))
// Number of bits used within bi_buf. (bi_buf might be implemented on
// more than 16 bits on some systems.)

/////////////////////////////////////////////////////////////////////////////
// Local data. These are initialized only once.

local ct_data static_ltree[L_CODES+2];
// The static literal tree. Since the bit lengths are imposed, there is no
// need for the L_CODES extra codes used during heap construction. However
// The codes 286 and 287 are needed to build a canonical tree (see _tr_init
// below).

local ct_data static_dtree[D_CODES];
// The static distance tree. (Actually a trivial tree since all codes use
// 5 bits.)

local uch dist_code[512];
// distance codes. The first 256 values correspond to the distances
// 3 .. 258, the last 256 values correspond to the top 8 bits of
// the 15 bit distances.

local uch length_code[MAX_MATCH-MIN_MATCH+1];
// length code for each normalized match length (0 == MIN_MATCH).

local int base_length[LENGTH_CODES];
// First normalized length for each code (0 = MIN_MATCH).

local int base_dist[D_CODES];
// First normalized distance for each code (0 = distance of 1).

struct static_tree_desc_s
{
    ct_data *static_tree;        /* static tree or NULL */
    intf    *extra_bits;         /* extra bits for each code or NULL */
    int     extra_base;          /* base index for extra_bits */
    int     elems;               /* max number of elements in the tree */
    int     max_length;          /* max bit length for the codes */
};

local static_tree_desc  static_l_desc =
{static_ltree, extra_lbits, LITERALS+1, L_CODES, MAX_BITS};

local static_tree_desc  static_d_desc =
{static_dtree, extra_dbits, 0,          D_CODES, MAX_BITS};

local static_tree_desc  static_bl_desc =
{(ct_data *)0, extra_blbits, 0,      BL_CODES, MAX_BL_BITS};

/////////////////////////////////////////////////////////////////////////////
// Local (static) routines in this file.

//local void tr_static_init OF((void));
local void init_block     OF((deflate_state *s));
local void pqdownheap     OF((deflate_state *s, ct_data *tree, int k));
local void gen_bitlen     OF((deflate_state *s, tree_desc *desc));
local void gen_codes      OF((ct_data *tree, int max_code, ushf *bl_count));
local void build_tree     OF((deflate_state *s, tree_desc *desc));
local void scan_tree      OF((deflate_state *s, ct_data *tree, int max_code));
local void send_tree      OF((deflate_state *s, ct_data *tree, int max_code));
local int  build_bl_tree  OF((deflate_state *s));
local void send_all_trees OF((deflate_state *s, int lcodes, int dcodes,
                              int blcodes));
local void compress_block OF((deflate_state *s, ct_data *ltree,
                              ct_data *dtree));
local void set_data_type  OF((deflate_state *s));
local unsigned bi_reverse OF((unsigned uVal, int nLth));
local void bi_windup      OF((deflate_state *s));
local void bi_flush       OF((deflate_state *s));
local void copy_block     OF((deflate_state *s, charf *buf, unsigned len,
                              int header));

#ifndef DEBUG
#  define send_code(s, c, tree) send_bits(s, tree[c].Code, tree[c].Len)
   // Send a code of the given tree. c and tree must not have side effects.

#else // DEBUG
#  define send_code(s, c, tree) \
     { if (verbose>1) fprintf(stderr,"\ncd %3d ",(c)); \
       send_bits(s, tree[c].Code, tree[c].Len); }
#endif

#define d_code(dist) \
   ((dist) < 256 ? dist_code[dist] : dist_code[256+((dist)>>7)])
/* Mapping from a distance to a distance code. dist is the distance - 1 and
 * must not have side effects. dist_code[256] and dist_code[257] are never
 * used.
 */

/* ==========================================================================
 * Output a short LSB first on the stream.
 * IN assertion: there is enough room in pendingBuf.
 */
#define put_short(s, w) { \
    put_byte(s, (uch)((w) & 0xff)); \
    put_byte(s, (uch)((ush)(w) >> 8)); \
}

/* ==========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */
#ifdef DEBUG
//local void send_bits      OF((deflate_state *s, int value, int length));

local void
send_bits(deflate_state *s,
          int value,  // value to send
          int length) // number of bits
{
    Tracevv((stderr," l %2d v %4x ", length, value));
    Assert(length > 0 && length <= 15, "invalid length");
    s->d.bits_sent += (ulg)length;

    /* If not enough room in bi_buf, use (valid) bits from bi_buf and
     * (16 - bi_valid) bits from value, leaving (width - (16-bi_valid))
     * unused bits in value.
     */
    if (s->d.bi_valid > (int)Buf_size - length) {
        s->d.bi_buf |= (value << s->d.bi_valid);
        put_short(s, s->d.bi_buf);
        s->d.bi_buf = (ush)value >> (Buf_size - s->d.bi_valid);
        s->d.bi_valid += length - Buf_size;
    } else {
        s->d.bi_buf |= value << s->d.bi_valid;
        s->d.bi_valid += length;
    }
}
#else // !DEBUG

#define send_bits(s, value, length) \
{ int len = length;\
  if (s->d.bi_valid > (int)Buf_size - len) {\
    int val = value;\
    s->d.bi_buf |= (val << s->d.bi_valid);\
    put_short(s, s->d.bi_buf);\
    s->d.bi_buf = (ush)val >> (Buf_size - s->d.bi_valid);\
    s->d.bi_valid += len - Buf_size;\
  } else {\
    s->d.bi_buf |= (value) << s->d.bi_valid;\
    s->d.bi_valid += len;\
  }\
}
#endif // DEBUG


#define MAX(a,b) (a >= b ? a : b)
// the arguments must not have side effects.

/////////////////////////////////////////////////////////////////////////////
// Initialize the various 'constant' tables. In a multi-threaded environment,
// this function may be called by two threads concurrently, but this is
// harmless since both invocations do exactly the same thing.
//
local void
tr_static_init()
{
    static int static_init_done = 0;
    int n;        /* iterates over tree elements */
    int nBits;    /* bit counter */
    int nLth;     /* length value */
    int nCode;    /* code value */
    int nDist;    /* distance index */
    ush bl_count[MAX_BITS+1];
    /* number of codes at each bit length for an optimal tree */

    if (static_init_done) return;

    /* Initialize the mapping length (0..255) -> length code (0..28) */
    nLth = 0;
    for (nCode = 0; nCode < LENGTH_CODES-1; nCode++) {
        base_length[nCode] = nLth;
        for (n = 0; n < (1<<extra_lbits[nCode]); n++) {
            length_code[nLth++] = (uch)nCode;
        }
    }
    Assert (nLth == 256, "tr_static_init: nLth != 256");
    /* Note that the length 255 (match length 258) can be represented
     * in two different ways: code 284 + 5 bits or code 285, so we
     * overwrite length_code[255] to use the best encoding:
     */
    length_code[nLth-1] = (uch)nCode;

    /* Initialize the mapping dist (0..32K) -> dist code (0..29) */
    nDist = 0;
    for (nCode = 0 ; nCode < 16; nCode++) {
        base_dist[nCode] = nDist;
        for (n = 0; n < (1<<extra_dbits[nCode]); n++) {
            dist_code[nDist++] = (uch)nCode;
        }
    }
    Assert (nDist == 256, "tr_static_init: nDist != 256");
    nDist >>= 7; /* from now on, all distances are divided by 128 */
    for ( ; nCode < D_CODES; nCode++) {
        base_dist[nCode] = nDist << 7;
        for (n = 0; n < (1<<(extra_dbits[nCode]-7)); n++) {
            dist_code[256 + nDist++] = (uch)nCode;
        }
    }
    Assert (nDist == 256, "tr_static_init: 256+nDist != 512");

    /* Construct the codes of the static literal tree */
    for (nBits = 0; nBits <= MAX_BITS; nBits++) bl_count[nBits] = 0;
    n = 0;
    while (n <= 143) static_ltree[n++].Len = 8, bl_count[8]++;
    while (n <= 255) static_ltree[n++].Len = 9, bl_count[9]++;
    while (n <= 279) static_ltree[n++].Len = 7, bl_count[7]++;
    while (n <= 287) static_ltree[n++].Len = 8, bl_count[8]++;
    /* Codes 286 and 287 do not exist, but we must include them in the
     * tree construction to get a canonical Huffman tree (longest code
     * all ones)
     */
    gen_codes((ct_data *)static_ltree, L_CODES+1, bl_count);

    /* The static distance tree is trivial: */
    for (n = 0; n < D_CODES; n++) {
        static_dtree[n].Len = 5;
        static_dtree[n].Code = bi_reverse((unsigned)n, 5);
    }
    static_init_done = 1;
}

/* ===========================================================================
 * Initialize the tree data structures for a new zlib stream.
 */
void
_tr_init(deflate_state *s)
{
    tr_static_init();

    s->d.compressed_len = 0L;

    s->d.l_desc.dyn_tree = s->d.dyn_ltree;
    s->d.l_desc.stat_desc = &static_l_desc;

    s->d.d_desc.dyn_tree = s->d.dyn_dtree;
    s->d.d_desc.stat_desc = &static_d_desc;

    s->d.bl_desc.dyn_tree = s->d.bl_tree;
    s->d.bl_desc.stat_desc = &static_bl_desc;

    s->d.bi_buf = 0;
    s->d.bi_valid = 0;
    s->d.last_eob_len = 8; /* enough lookahead for inflate */
#ifdef DEBUG
    s->d.bits_sent = 0L;
#endif

    /* Initialize the first block of the first file: */
    init_block(s);
}

/* ===========================================================================
 * Initialize a new block.
 */
local void init_block(deflate_state *s)
{
    int n; /* iterates over tree elements */

    /* Initialize the trees. */
    for (n = 0; n < L_CODES;  n++) s->d.dyn_ltree[n].Freq = 0;
    for (n = 0; n < D_CODES;  n++) s->d.dyn_dtree[n].Freq = 0;
    for (n = 0; n < BL_CODES; n++) s->d.bl_tree[n].Freq = 0;

    s->d.dyn_ltree[END_BLOCK].Freq = 1;
    s->d.opt_len = s->d.static_len = 0L;
    s->d.last_lit = s->d.matches = 0;
}

#define SMALLEST 1
/* Index within the heap array of least frequent node in the Huffman tree */


/* ===========================================================================
 * Remove the smallest element from the heap and recreate the heap with
 * one less element. Updates heap and heap_len.
 */
#define pqremove(s, tree, top) \
{\
    top = s->d.heap[SMALLEST]; \
    s->d.heap[SMALLEST] = s->d.heap[s->d.heap_len--]; \
    pqdownheap(s, tree, SMALLEST); \
}

/* ===========================================================================
 * Compares to subtrees, using the tree depth as tie breaker when
 * the subtrees have equal frequency. This minimizes the worst case length.
 */
#define smaller(tree, n, m, depth) \
   (tree[n].Freq < tree[m].Freq || \
   (tree[n].Freq == tree[m].Freq && depth[n] <= depth[m]))

/* ===========================================================================
 * Restore the heap property by moving down the tree starting at node k,
 * exchanging a node with the smallest of its two sons if necessary, stopping
 * when the heap property is re-established (each father smaller than its
 * two sons).
 */
local void
pqdownheap(deflate_state *s,
           ct_data *tree,  // the tree to restore
           int k)          // node to move down
{
    int v = s->d.heap[k];
    int j = k << 1;  /* left son of k */
    while (j <= s->d.heap_len) {
        /* Set j to the smallest of the two sons: */
        if (j < s->d.heap_len &&
            smaller(tree, s->d.heap[j+1], s->d.heap[j], s->d.depth)) {
            j++;
        }
        /* Exit if v is smaller than both sons */
        if (smaller(tree, v, s->d.heap[j], s->d.depth)) break;

        /* Exchange v with the smallest son */
        s->d.heap[k] = s->d.heap[j];  k = j;

        /* And continue down the tree, setting j to the left son of k */
        j <<= 1;
    }
    s->d.heap[k] = v;
}

/* ===========================================================================
 * Compute the optimal bit lengths for a tree and update the total bit length
 * for the current block.
 * IN assertion: the fields freq and dad are set, heap[heap_max] and
 *    above are the tree nodes sorted by increasing frequency.
 * OUT assertions: the field len is set to the optimal bit length, the
 *     array bl_count contains the frequencies for each bit length.
 *     The length opt_len is updated; static_len is also updated if stree is
 *     not null.
 */
local void
gen_bitlen(deflate_state *s,
           tree_desc *desc)    // the tree descriptor
{
    ct_data *tree  = desc->dyn_tree;
    int nMaxCode   = desc->max_code;
    ct_data *stree = desc->stat_desc->static_tree;
    intf *nExtra    = desc->stat_desc->extra_bits;
    int nBase       = desc->stat_desc->extra_base;
    int nMaxLth = desc->stat_desc->max_length;
    int h;              /* heap index */
    int n, m;           /* iterate over the tree elements */
    int nBits;           /* bit length */
    int xbits;          /* extra bits */
    ush f;              /* frequency */
    int overflow = 0;   /* number of elements with bit length too large */

    for (nBits = 0; nBits <= MAX_BITS; nBits++) s->d.bl_count[nBits] = 0;

    /* In a first pass, compute the optimal bit lengths (which may
     * overflow in the case of the bit length tree).
     */
    tree[s->d.heap[s->d.heap_max]].Len = 0; /* root of the heap */

    for (h = s->d.heap_max+1; h < HEAP_SIZE; h++) {
        n = s->d.heap[h];
        nBits = tree[tree[n].Dad].Len + 1;
        if (nBits > nMaxLth) nBits = nMaxLth, overflow++;
        tree[n].Len = (ush)nBits;
        /* We overwrite tree[n].Dad which is no longer needed */

        if (n > nMaxCode) continue; /* not a leaf node */

        s->d.bl_count[nBits]++;
        xbits = 0;
        if (n >= nBase) xbits = nExtra[n-nBase];
        f = tree[n].Freq;
        s->d.opt_len += (ulg)f * (nBits + xbits);
        if (stree) s->d.static_len += (ulg)f * (stree[n].Len + xbits);
    }
    if (overflow == 0) return;

    Trace((stderr,"\nbit length overflow\n"));
    /* This happens for example on obj2 and pic of the Calgary corpus */

    /* Find the first bit length which could increase: */
    do {
        nBits = nMaxLth-1;
        while (s->d.bl_count[nBits] == 0) nBits--;
        s->d.bl_count[nBits]--;      /* move one leaf down the tree */
        s->d.bl_count[nBits+1] += 2; /* move one overflow item as its brother */
        s->d.bl_count[nMaxLth]--;
        /* The brother of the overflow item also moves one step up,
         * but this does not affect bl_count[nMaxLth]
         */
        overflow -= 2;
    } while (overflow > 0);

    /* Now recompute all bit lengths, scanning in increasing frequency.
     * h is still equal to HEAP_SIZE. (It is simpler to reconstruct all
     * lengths instead of fixing only the wrong ones. This idea is taken
     * from 'ar' written by Haruhiko Okumura.)
     */
    for (nBits = nMaxLth; nBits != 0; nBits--) {
        n = s->d.bl_count[nBits];
        while (n != 0) {
            m = s->d.heap[--h];
            if (m > nMaxCode) continue;
            if (tree[m].Len != (unsigned) nBits) {
                Trace((stderr,"code %d nBits %d->%d\n", m, tree[m].Len, nBits));
                s->d.opt_len += ((zLONG)nBits - (zLONG)tree[m].Len)
                              *(zLONG)tree[m].Freq;
                tree[m].Len = (ush)nBits;
            }
            n--;
        }
    }
}

/* ===========================================================================
 * Generate the codes for a given tree and bit counts (which need not be
 * optimal).
 * IN assertion: the array bl_count contains the bit length statistics for
 * the given tree and the field len is set for all tree elements.
 * OUT assertion: the field code is set for all tree elements of non
 *     zero code length.
 */
local void
gen_codes(ct_data *tree,    // the tree to decorate
          int nMaxCode,     // largest code with non zero frequency
          ushf *bl_count)   // number of codes at each bit length
{
    ush next_code[MAX_BITS+1]; /* next code value for each bit length */
    ush code = 0;              /* running code value */
    int nBits;                  /* bit index */
    int n;                     /* code index */

    /* The distribution counts are first used to generate the code values
     * without bit reversal.
     */
    for (nBits = 1; nBits <= MAX_BITS; nBits++) {
        next_code[nBits] = code = (code + bl_count[nBits-1]) << 1;
    }
    /* Check that the bit counts in bl_count are consistent. The last code
     * must be all ones.
     */
    Assert (code + bl_count[MAX_BITS]-1 == (1<<MAX_BITS)-1,
            "inconsistent bit counts");
    Tracev((stderr,"\ngen_codes: nMaxCode %d ", nMaxCode));

    for (n = 0;  n <= nMaxCode; n++) {
        int len = tree[n].Len;
        if (len == 0) continue;
        /* Now reverse the bits */
        tree[n].Code = bi_reverse(next_code[len]++, len);

        Tracecv(tree != static_ltree, (stderr,"\nn %3d %c l %2d c %4x (%x) ",
             n, (isgraph(n) ? n : ' '), len, tree[n].Code, next_code[len]-1));
    }
}

/* ===========================================================================
 * Construct one Huffman tree and assigns the code bit strings and lengths.
 * Update the total bit length for the current block.
 * IN assertion: the field freq is set for all tree elements.
 * OUT assertions: the fields len and code are set to the optimal bit length
 *     and corresponding code. The length opt_len is updated; static_len is
 *     also updated if stree is not null. The field max_code is set.
 */
local void
build_tree(deflate_state *s,
           tree_desc *desc) // the tree descriptor
{
    ct_data *tree   = desc->dyn_tree;
    ct_data *stree  = desc->stat_desc->static_tree;
    int elems       = desc->stat_desc->elems;
    int n, m;          /* iterate over heap elements */
    int nMaxCode = -1; /* largest code with non zero frequency */
    int node;          /* new node being created */

    /* Construct the initial heap, with least frequent element in
     * heap[SMALLEST]. The sons of heap[n] are heap[2*n] and heap[2*n+1].
     * heap[0] is not used.
     */
    s->d.heap_len = 0, s->d.heap_max = HEAP_SIZE;

    for (n = 0; n < elems; n++) {
        if (tree[n].Freq != 0) {
            s->d.heap[++(s->d.heap_len)] = nMaxCode = n;
            s->d.depth[n] = 0;
        } else {
            tree[n].Len = 0;
        }
    }

    /* The pkzip format requires that at least one distance code exists,
     * and that at least one bit should be sent even if there is only one
     * possible code. So to avoid special checks later on we force at least
     * two codes of non zero frequency.
     */
    while (s->d.heap_len < 2) {
        node = s->d.heap[++(s->d.heap_len)] = (nMaxCode < 2 ? ++nMaxCode : 0);
        tree[node].Freq = 1;
        s->d.depth[node] = 0;
        s->d.opt_len--; if (stree) s->d.static_len -= stree[node].Len;
        /* node is 0 or 1 so it does not have extra bits */
    }
    desc->max_code = nMaxCode;

    /* The elements heap[heap_len/2+1 .. heap_len] are leaves of the tree,
     * establish sub-heaps of increasing lengths:
     */
    for (n = s->d.heap_len/2; n >= 1; n--) pqdownheap(s, tree, n);

    /* Construct the Huffman tree by repeatedly combining the least two
     * frequent nodes.
     */
    node = elems;              /* next internal node of the tree */
    do {
        pqremove(s, tree, n);  /* n = node of least frequency */
        m = s->d.heap[SMALLEST]; /* m = node of next least frequency */

        s->d.heap[--(s->d.heap_max)] = n; /* keep the nodes sorted by frequency */
        s->d.heap[--(s->d.heap_max)] = m;

        /* Create a new node father of n and m */
        tree[node].Freq = tree[n].Freq + tree[m].Freq;
        s->d.depth[node] = (uch) (MAX(s->d.depth[n], s->d.depth[m]) + 1);
        tree[n].Dad = tree[m].Dad = (ush)node;
#ifdef DUMP_BL_TREE
        if (tree == s->d.bl_tree) {
            fprintf(stderr,"\nnode %d(%d), sons %d(%d) %d(%d)",
                    node, tree[node].Freq, n, tree[n].Freq, m, tree[m].Freq);
        }
#endif
        /* and insert the new node in the heap */
        s->d.heap[SMALLEST] = node++;
        pqdownheap(s, tree, SMALLEST);

    } while (s->d.heap_len >= 2);

    s->d.heap[--(s->d.heap_max)] = s->d.heap[SMALLEST];

    /* At this point, the fields freq and dad are set. We can now
     * generate the bit lengths.
     */
    gen_bitlen(s, (tree_desc *)desc);

    /* The field len is now set, we can generate the bit codes */
    gen_codes ((ct_data *)tree, nMaxCode, s->d.bl_count);
}

/* ===========================================================================
 * Scan a literal or distance tree to determine the frequencies of the codes
 * in the bit length tree.
 */
local void
scan_tree(deflate_state *s,
          ct_data *tree,   // the tree to be scanned
          int nMaxCode)    // and its largest code of non zero frequency
{
    int n;                     /* iterates over all tree elements */
    int prevlen = -1;          /* last emitted length */
    int curlen;                /* length of current code */
    int nextlen = tree[0].Len; /* length of next code */
    int count = 0;             /* repeat count of the current code */
    int max_count = 7;         /* max repeat count */
    int min_count = 4;         /* min repeat count */

    if (nextlen == 0) max_count = 138, min_count = 3;
    tree[nMaxCode+1].Len = (ush)0xffff; /* guard */

    for (n = 0; n <= nMaxCode; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            s->d.bl_tree[curlen].Freq += count;
        } else if (curlen != 0) {
            if (curlen != prevlen) s->d.bl_tree[curlen].Freq++;
            s->d.bl_tree[REP_3_6].Freq++;
        } else if (count <= 10) {
            s->d.bl_tree[REPZ_3_10].Freq++;
        } else {
            s->d.bl_tree[REPZ_11_138].Freq++;
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

/* ===========================================================================
 * Send a literal or distance tree in compressed form, using the codes in
 * bl_tree.
 */
local void
send_tree (deflate_state *s,
           ct_data *tree, // the tree to be scanned
           int nMaxCode)  // and its largest code of non zero frequency
{
    int n;                     /* iterates over all tree elements */
    int prevlen = -1;          /* last emitted length */
    int curlen;                /* length of current code */
    int nextlen = tree[0].Len; /* length of next code */
    int count = 0;             /* repeat count of the current code */
    int max_count = 7;         /* max repeat count */
    int min_count = 4;         /* min repeat count */

    /* tree[nMaxCode+1].Len = -1; */  /* guard already set */
    if (nextlen == 0) max_count = 138, min_count = 3;

    for (n = 0; n <= nMaxCode; n++) {
        curlen = nextlen; nextlen = tree[n+1].Len;
        if (++count < max_count && curlen == nextlen) {
            continue;
        } else if (count < min_count) {
            do { send_code(s, curlen, s->d.bl_tree); } while (--count != 0);

        } else if (curlen != 0) {
            if (curlen != prevlen) {
                send_code(s, curlen, s->d.bl_tree); count--;
            }
            Assert(count >= 3 && count <= 6, " 3_6?");
            send_code(s, REP_3_6, s->d.bl_tree); send_bits(s, count-3, 2);

        } else if (count <= 10) {
            send_code(s, REPZ_3_10, s->d.bl_tree); send_bits(s, count-3, 3);

        } else {
            send_code(s, REPZ_11_138, s->d.bl_tree); send_bits(s, count-11, 7);
        }
        count = 0; prevlen = curlen;
        if (nextlen == 0) {
            max_count = 138, min_count = 3;
        } else if (curlen == nextlen) {
            max_count = 6, min_count = 3;
        } else {
            max_count = 7, min_count = 4;
        }
    }
}

/* ===========================================================================
 * Construct the Huffman tree for the bit lengths and return the index in
 * bl_order of the last bit length code to send.
 */
local int build_bl_tree(deflate_state *s)
{
    int max_blindex;  /* index of last bit length code of non zero freq */

    /* Determine the bit length frequencies for literal and distance trees */
    scan_tree(s, (ct_data *)s->d.dyn_ltree, s->d.l_desc.max_code);
    scan_tree(s, (ct_data *)s->d.dyn_dtree, s->d.d_desc.max_code);

    /* Build the bit length tree: */
    build_tree(s, (tree_desc *)(&(s->d.bl_desc)));
    /* opt_len now includes the length of the tree representations, except
     * the lengths of the bit lengths codes and the 5+5+4 bits for the counts.
     */

    /* Determine the number of bit length codes to send. The pkzip format
     * requires that at least 4 bit length codes be sent. (appnote.txt says
     * 3 but the actual value used is 4.)
     */
    for (max_blindex = BL_CODES-1; max_blindex >= 3; max_blindex--) {
        if (s->d.bl_tree[bl_order[max_blindex]].Len != 0) break;
    }
    /* Update opt_len to include the bit length tree and counts */
    s->d.opt_len += 3*(max_blindex+1) + 5+5+4;
    Tracev((stderr, "\ndyn trees: dyn %ld, stat %ld",
            s->d.opt_len, s->d.static_len));

    return max_blindex;
}

/* ===========================================================================
 * Send the header for a block using dynamic Huffman trees: the counts, the
 * lengths of the bit length codes, the literal tree and the distance tree.
 * IN assertion: lcodes >= 257, dcodes >= 1, blcodes >= 4.
 */
local void
send_all_trees(deflate_state *s, int lcodes,
               int dcodes, int blcodes)
{
    int rank;                    /* index in bl_order */

    Assert (lcodes >= 257 && dcodes >= 1 && blcodes >= 4, "not enough codes");
    Assert (lcodes <= L_CODES && dcodes <= D_CODES && blcodes <= BL_CODES,
            "too many codes");
    Tracev((stderr, "\nbl counts: "));
    send_bits(s, lcodes-257, 5); /* not +255 as stated in appnote.txt */
    send_bits(s, dcodes-1,   5);
    send_bits(s, blcodes-4,  4); /* not -3 as stated in appnote.txt */
    for (rank = 0; rank < blcodes; rank++) {
        Tracev((stderr, "\nbl code %2d ", bl_order[rank]));
        send_bits(s, s->d.bl_tree[bl_order[rank]].Len, 3);
    }
    Tracev((stderr, "\nbl tree: sent %ld", s->d.bits_sent));

    send_tree(s, (ct_data *)s->d.dyn_ltree, lcodes-1); /* literal tree */
    Tracev((stderr, "\nlit tree: sent %ld", s->d.bits_sent));

    send_tree(s, (ct_data *)s->d.dyn_dtree, dcodes-1); /* distance tree */
    Tracev((stderr, "\ndist tree: sent %ld", s->d.bits_sent));
}

/* ===========================================================================
 * Send a stored block
 */
void
_tr_stored_block(deflate_state *s,
                 charf *buf,     // input block
                 ulg stored_len, // length of input block
                 int eof)        // true if this is the last block for a file
{
    send_bits(s, (STORED_BLOCK<<1)+eof, 3);  /* send block type */
    s->d.compressed_len = (s->d.compressed_len + 3 + 7) & (ulg)~7L;
    s->d.compressed_len += (stored_len + 4) << 3;

    copy_block(s, buf, (unsigned)stored_len, 1); /* with header */
}

/* ===========================================================================
 * Send one empty static block to give enough lookahead for inflate.
 * This takes 10 bits, of which 7 may remain in the bit buffer.
 * The current inflate code requires 9 bits of lookahead. If the
 * last two codes for the previous block (real code plus EOB) were coded
 * on 5 bits or less, inflate may have only 5+3 bits of lookahead to decode
 * the last real code. In this case we send two empty static blocks instead
 * of one. (There are no problems if the previous block is stored or fixed.)
 * To simplify the code, we assume the worst case of last real code encoded
 * on one bit only.
 */
void
_tr_align(deflate_state *s)
{
    send_bits(s, STATIC_TREES<<1, 3);
    send_code(s, END_BLOCK, static_ltree);
    s->d.compressed_len += 10L; /* 3 for block type, 7 for EOB */
    bi_flush(s);
    /* Of the 10 bits for the empty block, we have already sent
     * (10 - bi_valid) bits. The lookahead for the last real code (before
     * the EOB of the previous block) was thus at least one plus the length
     * of the EOB plus what we have just sent of the empty static block.
     */
    if (1 + s->d.last_eob_len + 10 - s->d.bi_valid < 9) {
        send_bits(s, STATIC_TREES<<1, 3);
        send_code(s, END_BLOCK, static_ltree);
        s->d.compressed_len += 10L;
        bi_flush(s);
    }
    s->d.last_eob_len = 7;
}

/* ===========================================================================
 * Determine the best encoding for the current block: dynamic trees, static
 * trees or store, and output the encoded block to the zip file. This function
 * returns the total compressed length for the file so far.
 */
ulg
_tr_flush_block(deflate_state *s,
                charf *buf,      // input block, or NULL if too old
                ulg stored_len,  // length of input block
                int eof)         // true if this is the last block for a file
{
    ulg opt_lenb, static_lenb; /* opt_len and static_len in bytes */
    int max_blindex = 0;  /* index of last bit length code of non zero freq */

    /* Build the Huffman trees unless a stored block is forced */
    if (s->d.level > 0) {

    /* Check if the file is ascii or binary */
   if (s->d.data_type == Z_UNKNOWN) set_data_type(s);

   /* Construct the literal and distance trees */
   build_tree(s, (tree_desc *)(&(s->d.l_desc)));
   Tracev((stderr, "\nlit data: dyn %ld, stat %ld", s->d.opt_len,
      s->d.static_len));

   build_tree(s, (tree_desc *)(&(s->d.d_desc)));
   Tracev((stderr, "\ndist data: dyn %ld, stat %ld", s->d.opt_len,
      s->d.static_len));
   /* At this point, opt_len and static_len are the total bit lengths of
    * the compressed block data, excluding the tree representations.
    */

   /* Build the bit length tree for the above two trees, and get the index
    * in bl_order of the last bit length code to send.
    */
   max_blindex = build_bl_tree(s);

   /* Determine the best encoding. Compute first the block length in bytes*/
   opt_lenb = (s->d.opt_len+3+7)>>3;
   static_lenb = (s->d.static_len+3+7)>>3;

   Tracev((stderr, "\nopt %lu(%lu) stat %lu(%lu) stored %lu lit %u ",
      opt_lenb, s->d.opt_len, static_lenb, s->d.static_len, stored_len,
      s->d.last_lit));

   if (static_lenb <= opt_lenb) opt_lenb = static_lenb;

    } else {
        Assert(buf != (char*)0, "lost buf");
   opt_lenb = static_lenb = stored_len + 5; /* force a stored block */
    }

    /* If compression failed and this is the first and last block,
     * and if the .zip file can be seeked (to rewrite the local header),
     * the whole file is transformed into a stored file:
     */
#ifdef STORED_FILE_OK
#  ifdef FORCE_STORED_FILE
    if (eof && s->d.compressed_len == 0L) { /* force stored file */
#  else
    if (stored_len <= opt_lenb && eof && s->d.compressed_len==0L && seekable()) {
#  endif
        /* Since LIT_BUFSIZE <= 2*WSIZE, the input data must be there: */
        if (buf == (charf*)0) error ("block vanished");

        copy_block(buf, (unsigned)stored_len, 0); /* without header */
        s->d.compressed_len = stored_len << 3;
        s->d.method = STORED;
    } else
#endif /* STORED_FILE_OK */

#ifdef FORCE_STORED
    if (buf != (char*)0) { /* force stored block */
#else
    if (stored_len+4 <= opt_lenb && buf != (char*)0) {
                       /* 4: two words for the lengths */
#endif
        /* The test buf != NULL is only necessary if LIT_BUFSIZE > WSIZE.
         * Otherwise we can't have processed more than WSIZE input bytes since
         * the last block flush, because compression would have been
         * successful. If LIT_BUFSIZE <= WSIZE, it is never too late to
         * transform a block into a stored block.
         */
        _tr_stored_block(s, buf, stored_len, eof);

#ifdef FORCE_STATIC
    } else if (static_lenb >= 0) { /* force static trees */
#else
    } else if (static_lenb == opt_lenb) {
#endif
        send_bits(s, (STATIC_TREES<<1)+eof, 3);
        compress_block(s, (ct_data *)static_ltree, (ct_data *)static_dtree);
        s->d.compressed_len += 3 + s->d.static_len;
    } else {
        send_bits(s, (DYN_TREES<<1)+eof, 3);
        send_all_trees(s, s->d.l_desc.max_code+1, s->d.d_desc.max_code+1,
                       max_blindex+1);
        compress_block(s, (ct_data *)s->d.dyn_ltree, (ct_data *)s->d.dyn_dtree);
        s->d.compressed_len += 3 + s->d.opt_len;
    }
    Assert (s->d.compressed_len == s->d.bits_sent, "bad compressed size");
    init_block(s);

    if (eof) {
        bi_windup(s);
        s->d.compressed_len += 7;  /* align on byte boundary */
    }
    Tracev((stderr,"\ncomprlen %lu(%lu) ", s->d.compressed_len>>3,
           s->d.compressed_len-7*eof));

    return s->d.compressed_len >> 3;
}

/* ===========================================================================
 * Save the match info and tally the frequency counts. Return true if
 * the current block must be flushed.
 */
int
_tr_tally(deflate_state *s,
          unsigned dist, // distance of matched string
          unsigned lc)   // match length-MIN_MATCH or unmatched char (if dist==0)
{
    s->d.d_buf[s->d.last_lit] = (ush)dist;
    s->d.l_buf[s->d.last_lit++] = (uch)lc;
    if (dist == 0) {
        /* lc is the unmatched char */
        s->d.dyn_ltree[lc].Freq++;
    } else {
        s->d.matches++;
        /* Here, lc is the match length - MIN_MATCH */
        dist--;             /* dist = match distance - 1 */
        Assert((ush)dist < (ush)MAX_DIST(s) &&
               (ush)lc <= (ush)(MAX_MATCH-MIN_MATCH) &&
               (ush)d_code(dist) < (ush)D_CODES,  "_tr_tally: bad match");

        s->d.dyn_ltree[length_code[lc]+LITERALS+1].Freq++;
        s->d.dyn_dtree[d_code(dist)].Freq++;
    }

    /* Try to guess if it is profitable to stop the current block here */
    if (s->d.level > 2 && (s->d.last_lit & 0xfff) == 0) {
        /* Compute an upper bound for the compressed length */
        ulg out_length = (ulg)s->d.last_lit*8L;
        ulg in_length = (ulg)((zLONG)s->d.strstart - s->d.block_start);
        int dcode;
        for (dcode = 0; dcode < D_CODES; dcode++) {
            out_length += (ulg)s->d.dyn_dtree[dcode].Freq *
                (5L+extra_dbits[dcode]);
        }
        out_length >>= 3;
        Tracev((stderr,"\nlast_lit %u, in %ld, out ~%ld(%ld%%) ",
               s->d.last_lit, in_length, out_length,
               100L - out_length*100L/in_length));
        if (s->d.matches < s->d.last_lit/2 && out_length < in_length/2) return( 1 );
    }
    return (s->d.last_lit == s->d.lit_bufsize-1);
    /* We avoid equality with lit_bufsize because of wraparound at 64K
     * on 16 bit machines and because stored blocks are restricted to
     * 64K-1 bytes.
     */
}

/* ===========================================================================
 * Send the block data compressed using the given Huffman trees
 */
local void
compress_block(deflate_state *s,
               ct_data *ltree,  /* literal tree */
               ct_data *dtree)  /* distance tree */
{
    unsigned dist;      /* distance of matched string */
    int lc;             /* match length or unmatched char (if dist == 0) */
    unsigned lx = 0;    /* running index in l_buf */
    unsigned code;      /* the code to send */
    int nExtra;          /* number of extra bits to send */

    if (s->d.last_lit != 0) do {
        dist = s->d.d_buf[lx];
        lc = s->d.l_buf[lx++];
        if (dist == 0) {
            send_code(s, lc, ltree); /* send a literal byte */
            Tracecv(isgraph(lc), (stderr," '%c' ", lc));
        } else {
            /* Here, lc is the match length - MIN_MATCH */
            code = length_code[lc];
            send_code(s, code+LITERALS+1, ltree); /* send the length code */
            nExtra = extra_lbits[code];
            if (nExtra != 0) {
                lc -= base_length[code];
                send_bits(s, lc, nExtra);       /* send the extra length bits */
            }
            dist--; /* dist is now the match distance - 1 */
            code = d_code(dist);
            Assert (code < D_CODES, "bad d_code");

            send_code(s, code, dtree);       /* send the distance code */
            nExtra = extra_dbits[code];
            if (nExtra != 0) {
                dist -= base_dist[code];
                send_bits(s, dist, nExtra);   /* send the extra distance bits */
            }
        } /* literal or match pair ? */

        /* Check that the overlay between pending_buf and d_buf+l_buf is ok: */
        Assert(s->d.pending < (int) (s->d.lit_bufsize + 2*lx), "pendingBuf overflow");

    } while (lx < s->d.last_lit);

    send_code(s, END_BLOCK, ltree);
    s->d.last_eob_len = ltree[END_BLOCK].Len;
}

/* ===========================================================================
 * Set the data type to ASCII or BINARY, using a crude approximation:
 * binary if more than 20% of the bytes are <= 6 or >= 128, ascii otherwise.
 * IN assertion: the fields freq of dyn_ltree are set and the total of all
 * frequencies does not exceed 64K (to fit in an int on 16 bit machines).
 */
local void set_data_type(deflate_state *s)
{
    int n = 0;
    unsigned ascii_freq = 0;
    unsigned bin_freq = 0;
    while (n < 7)        bin_freq += s->d.dyn_ltree[n++].Freq;
    while (n < 128)    ascii_freq += s->d.dyn_ltree[n++].Freq;
    while (n < LITERALS) bin_freq += s->d.dyn_ltree[n++].Freq;
    s->d.data_type = (Byte)(bin_freq > (ascii_freq >> 2) ? Z_BINARY : Z_ASCII);
}

/* ===========================================================================
 * Reverse the first len bits of a code, using straightforward code (a faster
 * method would use a table)
 * IN assertion: 1 <= len <= 15
 */
local unsigned
bi_reverse(unsigned code, /* the value to invert */
           int len)       /* its bit length */
{
    register unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

/* ===========================================================================
 * Flush the bit buffer, keeping at most 7 bits in it.
 */
local void bi_flush(deflate_state *s)
{
    if (s->d.bi_valid == 16) {
        put_short(s, s->d.bi_buf);
        s->d.bi_buf = 0;
        s->d.bi_valid = 0;
    } else if (s->d.bi_valid >= 8) {
        put_byte(s, (Byte)s->d.bi_buf);
        s->d.bi_buf >>= 8;
        s->d.bi_valid -= 8;
    }
}

/* ===========================================================================
 * Flush the bit buffer and align the output on a byte boundary
 */
local void bi_windup(deflate_state *s)
{
    if (s->d.bi_valid > 8) {
        put_short(s, s->d.bi_buf);
    } else if (s->d.bi_valid > 0) {
        put_byte(s, (Byte)s->d.bi_buf);
    }
    s->d.bi_buf = 0;
    s->d.bi_valid = 0;
#ifdef DEBUG
    s->d.bits_sent = (s->d.bits_sent+7) & ~7;
#endif
}

/* ===========================================================================
 * Copy a stored block, storing first the length and its
 * one's complement if requested.
 */
local void
copy_block(deflate_state *s,
           charf    *buf,    // the input data
           unsigned uLth,    // its length
           int      nHeader)  // true if block header must be written
{
    bi_windup(s);          // align on byte boundary
    s->d.last_eob_len = 8; // enough lookahead for inflate

    if (nHeader) {
        put_short(s, (ush)uLth);
        put_short(s, (ush)~uLth);
#ifdef DEBUG
        s->d.bits_sent += 2*16;
#endif
    }
#ifdef DEBUG
    s->d.bits_sent += (ulg)uLth<<3;
#endif
    while (uLth--)
    {
        put_byte(s, *buf++);
    }
}

///////////////////////////// ZLIBENGN.CPP //////////////////////////////////

//
// This file contains the implementation the
// the ZlibEngine class, used to simplify
// compression and decompression of files
// using the Zlib engine.
//
// The ZlibEngine is a Tiny Software (tm) project.
// You may use the code in this project without restriction.
// Contact markn@tiny.com for more information.
//

#if defined( _WINDOWS )
#include <windows.h>
#endif

//#include "Compress.h"

//
// The constructor initializes a couple of members
// of the z_stream class.  See the Zlib documentation
// for details on what those members do
//

ZlibEngine::ZlibEngine()
{
    zalloc = 0;  //z_stream member
    zfree = 0;   //z_stream member
    opaque = 0;  //z_stream member
//
// I initialize these members just for tidiness.
//
    m_fin = 0;
    m_fout = 0;
}

//
// compress() is the public function used to compress
// a single file.  It has to take care of opening the
// input and output files and setting up the buffers for
// Zlib.  It then calls deflate() repeatedly until all
// input and output processing has been done, and finally
// closes the files and cleans up the Zlib structures.
//
int
ZlibEngine::compress( zCPCHAR input,
                      zCPCHAR output,
                      int level )
{
    m_nErr = Z_OK;
    avail_in = 0;
    avail_out = output_length;
    next_out = output_buffer;
    m_AbortFlag = 0;

    m_fin  = fopen( input, "rb" );
    m_fout = fopen( output, "wb" );
    m_lLth = filelength( fileno( m_fin ) );
    deflateInit( this, level );
    for ( ; ; )
    {
        if ( m_AbortFlag )
            break;
        if ( !load_input() )
            break;
        m_nErr = deflate( this, Z_NO_FLUSH );
        flush_output();
        if ( m_nErr != Z_OK )
            break;
        progress( percent() );
    }

    for ( ; ; )
    {
        if ( m_AbortFlag )
            break;
        m_nErr = deflate( this, Z_FINISH );
        if ( !flush_output() )
            break;
        if ( m_nErr != Z_OK )
            break;
    }
    progress( percent() );
    deflateEnd( this );
    if ( m_AbortFlag )
        status( "User Abort" );
    else if ( m_nErr != Z_OK && m_nErr != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        m_nErr = Z_OK;
    }
    fclose( m_fin );
    fclose( m_fout );
    m_fin = 0;
    m_fout = 0;
    if ( m_AbortFlag )
        return Z_USER_ABORT;
    else
        return m_nErr;
}

//
// decompress has to do most of the same chores as compress().
// The only major difference it has is the absence of the level
// parameter.  The level isn't needed when decompressing data
// using the deflate algorithm.
//

int ZlibEngine::decompress( zCPCHAR input,
                            zCPCHAR output )
{
    m_nErr = Z_OK;
    avail_in = 0;
    avail_out = output_length;
    next_out = output_buffer;
    m_AbortFlag = 0;

    m_fin  = fopen( input, "rb" );
    m_fout = fopen( output, "wb" );
    m_lLth = filelength( fileno( m_fin ) );
    inflateInit( this );
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        if ( !load_input() )
            break;
        m_nErr = inflate( this, Z_NO_FLUSH );
        flush_output();
        if ( m_nErr != Z_OK )
            break;
        progress( percent() );
    }
    for ( ; ; ) {
        if ( m_AbortFlag )
            break;
        m_nErr = inflate( this, Z_FINISH );
        if ( !flush_output() )
            break;
        if ( m_nErr != Z_OK )
            break;
    }
    progress( percent() );
    inflateEnd( this );
    if ( m_AbortFlag )
        status( "User Abort" );
    else if ( m_nErr != Z_OK && m_nErr != Z_STREAM_END )
        status( "Zlib Error" );
    else {
        status( "Success" );
        m_nErr = Z_OK;
    }
    if ( m_fin )
        fclose( m_fin );
    m_fin = 0;
    if ( m_fout )
        fclose( m_fout );
    m_fout = 0;
    if ( m_AbortFlag )
        return Z_USER_ABORT;
    else
        return m_nErr;
}
//
//  This function is called so as to provide the progress()
//  virtual function with a reasonable figure to indicate
//  how much processing has been done.  Note that the length
//  member is initialized when the input file is opened.
//
int ZlibEngine::percent()
{
    if ( m_lLth == 0 )
        return 100;
    else if ( m_lLth > 10000000L )
        return ( total_in / ( m_lLth / 100 ) );
    else
        return ( total_in * 100 / m_lLth );
}

//
//  Every time Zlib consumes all of the data in the
//  input buffer, this function gets called to reload.
//  The avail_in member is part of z_stream, and is
//  used to keep track of how much input is available.
//  I churn the Windows message loop to ensure that
//  the process can be aborted by a button press or
//  other Windows event.
//
int ZlibEngine::load_input()
{
#if defined( _WINDOWS )
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
#endif
    if ( avail_in == 0 ) {
        next_in = input_buffer;
        avail_in = fread( input_buffer, 1, input_length, m_fin );
    }
    return avail_in;
}

//
//  Every time Zlib filsl the output buffer with data,
//  this function gets called.  Its job is to write
//  that data out to the output file, then update
//  the z_stream member avail_out to indicate that more
//  space is now available.  I churn the Windows message
//  loop to ensure that the process can be aborted by a
//  button press or other Windows event.
//

int
ZlibEngine::flush_output()
{
#if defined( _WINDOWS )
    MSG msg;
    while ( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
#endif
    unsigned int uCount = output_length - avail_out;
    if ( uCount )
    {
        if ( fwrite( output_buffer, 1, uCount, m_fout ) != uCount )
        {
            m_nErr = Z_ERRNO;
            return( 0 );
        }

        next_out = output_buffer;
        avail_out = output_length;
    }

    return( uCount );
}


//
// This test program uses the ZlibEngine class
// to implement a short and sweet file compression
// program.  See the README file included with this
// source file for project building information.
//
// The ZlibEngine is a Tiny Software (tm) project.
// You may use the code in this project without restriction.
// Contact markn@tiny.com for more information.
//

//#include <stdio.h>
//#include <conio.h>
//#include <limits.h>

//
// The only customization I need to do to the ZlibEngine
// class is to create my own progress function.  The
// progress function will be called periodically as the
// files are compressed and expanded.  If the user presses
// any key during the process an abort is signaled.
//

//#include <time.h>
////#include <sys/types.h>
//#include <sys/timeb.h>
//#include <string.h>

// time - compression - level ... for a .BMP
// 0540 - 3,932,825   - 0
// 1292 - 1,534,154   - 1
// 1302 - 1,528,630   - 2
// 1zTAG_LTH2 - 1,526,980 - 3
// 1692 - 1,518,434   - 4
// 1742 - 1,517,049   - 5
// 1793 - 1,517,144   - 6
// 1783 - 1,517,167   - 7
// 1913 - 1,515,680   - 8
// 1993 - 1,515,621   - 9

class MyZlibEngine : public ZlibEngine
{
public :
   void
   progress( int nPercent )
   {
   // printf( "%3d%%\b\b\b\b", nPercent );
   // if ( kbhit() )
   // {
   //    getch();
   //    m_AbortFlag = 1;
   // }
   }
};

//./ ADD NAME=CompressFile
// Source Module=zdrdlgw.cpp
/////////////////////////////////////////////////////////////////////////////
//
//  DIALOG OPERATION: CompressFile
//                                              08/20/01 Modified: 08/20/01
//
//  PURPOSE:   This routine compresses/decompresses a file.  The source file
//             will be processed (compressed or decompressed) and the results
//             will be stored in the target file.
//
//  PARAMETERS: vSubtask    - The subtask view for the current window
//              cpcSrcFileName - Specified name of the file to be processed.
//              cpcTgtFileName - Name of the file in which to store the
//                            results.  If null or empty, a temporary file
//                            name is generated.
//              pchReturnFileName - Name of the file in which the results
//                            are stored.
//              nMaxFileNameLth -  Maximum length of (return) file name.
//              bCompress   - TRUE - compress   FALSE - decompress file
//
//  RETURNS:    0 - operation (compress or decompress) succeeded
//             -1 - Error locating window
//
/////////////////////////////////////////////////////////////////////////////
//./ END + 7
zOPER_EXPORT zSHORT OPERATION
CompressFile( zVIEW   vSubtask,
              zCPCHAR cpcSrcFileName,
              zCPCHAR cpcTgtFileName,
              zPCHAR  pchReturnFileName,
              zSHORT  nMaxFileNameLth,
              zBOOL   bCompress )
{
   ZSubtask *pZSubtask;

   if ( GetWindowAndCtrl( &pZSubtask, 0, vSubtask, 0 ) == 0 )
   {
      if ( cpcTgtFileName && *cpcTgtFileName )
      {
         strncpy_s( pchReturnFileName, nMaxFileNameLth, cpcTgtFileName, nMaxFileNameLth - 1 );
      }
      else
      {
         zCHAR  szZeidonPath[ zMAX_FILENAME_LTH ];
         zCHAR  szWorkString[ zMAX_FILENAME_LTH ];
         LPAPP pApp;

         szZeidonPath[ 0 ] = 0;
         if ( SfGetApplicationForSubtask( &pApp, pZSubtask->m_vDialog ) == 0 &&
              pApp )
         {
            strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), pApp->szLocalDir );
         }

         if ( szZeidonPath[ 0 ] == 0  )
            strcpy_s( szZeidonPath, zsizeof( szZeidonPath ), "c:\\temp\\" );

         SysAppendcDirSep( szZeidonPath );
         SysConvertEnvironmentString( szWorkString, zsizeof( szWorkString ), szZeidonPath );
         GenerateUniqueFileName( pchReturnFileName, nMaxFileNameLth, szWorkString, "Compress", "out" );
      }

      MyZlibEngine *pZLib = new MyZlibEngine;
      int nRC;

      if ( bCompress )
      {
         nRC = pZLib->compress( cpcSrcFileName, pchReturnFileName, Z_DEFAULT_COMPRESSION );
      }
      else
      {
          nRC = pZLib->decompress( cpcSrcFileName, pchReturnFileName );
      }

      delete( pZLib );
      return( 0 );
   }

   return( -1 );
}
