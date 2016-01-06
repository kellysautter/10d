
typedef union
{
   int pos;
   STRTOK tstr;
   NUMTOK tnum;
}  YYSTYPE;
extern YYSTYPE yylval;

# define T_FUNCTION 257
# define T_IDENT 258
# define T_STRING 259
# define T_ALL 260
# define T_AND 261
# define T_ARE 262
# define T_AS 263
# define T_ASCENDING 264
# define T_ASCII 265
# define T_BETWEEN 266
# define T_BREAK 267
# define T_BY 268
# define T_CALCULATE 269
# define T_CENTER 270
# define T_CHAR 271
# define T_COLUMN 272
# define T_CREATE 273
# define T_CURRENCY 274
# define T_DATE 275
# define T_DECIMAL 276
# define T_DB_ADDR 277
# define T_DBADDR 278
# define T_DEFINE 279
# define T_DESCENDING 280
# define T_DESCRIBE 281
# define T_DIF 282
# define T_DISPLAY 283
# define T_DISTINCT 284
# define T_DOUBLE 285
# define T_ECHO 286
# define T_END 287
# define T_EOS 288
# define T_EQ 289
# define T_FIELD 290
# define T_FILE 291
# define T_FIXED 292
# define T_FLOAT 293
# define T_FLOATW 294
# define T_FOREIGN 295
# define T_FROM 296
# define T_GE 297
# define T_GROUP 298
# define T_GT 299
# define T_HAVING 300
# define T_HELP 301
# define T_IF 302
# define T_IN 303
# define T_INT 304
# define T_INTW 305
# define T_INTO 306
# define T_IS 307
# define T_KEY 308
# define T_LE 309
# define T_LEFT 310
# define T_LENGTH 311
# define T_LIKE 312
# define T_LINK 313
# define T_LOCKCOUNT 314
# define T_LONG 315
# define T_LONGW 316
# define T_LT 317
# define T_MACRO 318
# define T_MEMBER 319
# define T_MONEY 320
# define T_NE 321
# define T_NOT 322
# define T_NULL 323
# define T_NUMERIC 324
# define T_OCCURRENCE 325
# define T_OF 326
# define T_OFF 327
# define T_ON 328
# define T_OR 329
# define T_ORDER 330
# define T_OTHER 331
# define T_OUTPUT 332
# define T_OWNER 333
# define T_PAGE 334
# define T_PATH 335
# define T_PRINTER 336
# define T_PROMPT 337
# define T_RELATION 338
# define T_REPORT 339
# define T_RIGHT 340
# define T_RUN 341
# define T_SELECT 342
# define T_SET 343
# define T_SEPARATOR 344
# define T_SHORT 345
# define T_SHORTW 346
# define T_SHOW 347
# define T_SKIP 348
# define T_SUBSTR 349
# define T_SYSDATE 350
# define T_TERMINAL 351
# define T_THRU 352
# define T_TIME 353
# define T_TITLE 354
# define T_TO 355
# define T_TODAY 356
# define T_TRANSLATE 357
# define T_UNIQUE 358
# define T_USING 359
# define T_VALUES 360
# define T_VIEW 361
# define T_WDOUBLE 362
# define T_WHERE 363
# define T_WIDTH 364
# define T_XOR 365
/* vpp @win.vpp YTAB.H (Thu May 21 10:47:52 1992)
 *
 *     -dMSDOS -nUNIX -nRISC -nSUN -nSCO -dDB_SHARE -dWINDOWS -nVANILLA_BSD
 *     -nVMS -nVMS_SHARE -nOS2 -nOS2_MT -nGENERAL -dFAR_ALLOC -nARCHIVING
 *     -dDB_DLL -nBSD -dMEMLOCK -nIS_UNIX_REALLY -dREOPEN_FILES -dMULTI_TASK
 *     -nONE_DB -nNO_COUNTRY -nNO_FLOAT -nNO_TIMESTAMP -nSINGLE_USER -nNO_TRANS
 *     -nVer330 -d_WINDOWS -nQNX -nNEEDED -nUSED -nMACROS_ALLOWED -nOLD_WAY
 *     -nTRUENAME -nLAT
 */

