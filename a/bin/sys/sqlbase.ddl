DROP   TABLE W_SQLCTCLMN;
DROP   TABLE W_SQLCT_SRC;
DROP   TABLE W_SQLCTINDX;
DROP   TABLE WSQLCTINDXCLMN;
DROP   TABLE W_SQLCTTBL;
DROP   TABLE MM_W_SQLCTCLMN_SRF;

CREATE TABLE W_SQLCTCLMN
       ( NAME                            CHAR( 32 )                  ,
         COLNO                           INTEGER                     ,
         COLTYPE                         CHAR( 8 )                   ,
         LENGTH                          INTEGER                     ,
         SCALE                           INTEGER                     ,
         NULLS                           CHAR( 1 )                   ,
         REMARKS                         CHAR( 254 )                 ,
         z_LABEL                         CHAR( 30 )                  ,
         FK_CREATOR                      CHAR( 32 )        NOT NULL  ,
         FK_NAME                         CHAR( 32 )        NOT NULL  )
   PRIMARY KEY 
       ( NAME                               ,
         COLNO                              )
   FOREIGN KEY TABLEID
       ( FK_CREATOR                         ,
         FK_NAME                            ) REFERENCES W_SQLCTTBL
;
CREATE UNIQUE INDEX WSQLCTCLMNWSQLCTTB
     ON W_SQLCTCLMN
       ( NAME                               ,
         COLNO                              );

CREATE TABLE W_SQLCT_SRC
       ( NAME                            CHAR( 32 )        NOT NULL  ,
         ZKEY                            INTEGER           NOT NULL  ,
         SERVER                          CHAR( 32 )        NOT NULL  ,
         z_DESC                          CHAR( 254 )                 ,
         PARADIGM                        CHAR( 1 )                   ,
         DBMS                            CHAR( 1 )                   ,
         CONECTIONTYPE                   CHAR( 1 )                   )
   PRIMARY KEY 
;

CREATE TABLE W_SQLCTINDX
       ( NAME                            CHAR( 32 )                  ,
         CREATOR                         CHAR( 32 )                  ,
         UNIQUERULE                      CHAR( 1 )                   ,
         COLCOUNT                        INTEGER                     ,
         IXTYPE                          CHAR( 1 )                   ,
         CLUSTERRULE                     CHAR( 1 )                   ,
         IXSIZE                          INTEGER                     ,
         PERCENTFREE                     INTEGER                     ,
         FK_CREATOR                      CHAR( 32 )        NOT NULL  ,
         FK_NAME                         CHAR( 32 )        NOT NULL  )
   PRIMARY KEY 
       ( CREATOR                            ,
         NAME                               )
   FOREIGN KEY TABLEID
       ( FK_CREATOR                         ,
         FK_NAME                            ) REFERENCES W_SQLCTTBL
;
CREATE UNIQUE INDEX W_SQLCTINDXINDEXID
     ON W_SQLCTINDX
       ( CREATOR                            ,
         NAME                               );

CREATE TABLE WSQLCTINDXCLMN
       ( COLSEQ                          INTEGER                     ,
         ORDERING                        CHAR( 1 )                   ,
         FUNCTION                        CHAR( 250 )                 ,
         FK_CREATOR                      CHAR( 32 )        NOT NULL  ,
         FK_NAME                         CHAR( 32 )        NOT NULL  )
   PRIMARY KEY 
   FOREIGN KEY INDEXID
       ( FK_CREATOR                         ,
         FK_NAME                            ) REFERENCES W_SQLCTINDX
;
CREATE UNIQUE INDEX WSQLCTNDXCLMNNDXCX
     ON WSQLCTINDXCLMN

CREATE TABLE W_SQLCTTBL
       ( CREATOR                         CHAR( 32 )                  ,
         NAME                            CHAR( 32 )                  ,
         COLCOUNT                        INTEGER                     ,
         REMARKS                         CHAR( 254 )                 ,
         TYPE                            CHAR( 1 )                   ,
         SNUM                            INTEGER                     ,
         z_LABEL                         CHAR( 30 )                  ,
         PERCENTFREE                     INTEGER                     )
   PRIMARY KEY 
       ( CREATOR                            ,
         NAME                               )
;
CREATE UNIQUE INDEX W_SQLCTTBL_TABLEID
     ON W_SQLCTTBL
       ( CREATOR                            ,
         NAME                               );

CREATE TABLE MM_W_SQLCTCLMN_SRF
       ( FK_NAME                         CHAR( 32 )                  ,
         FK_COLNO                        INTEGER                     )
   PRIMARY KEY 
       ( FK_NAME                            ,
         FK_COLNO                           )
   FOREIGN KEY W_SQLCATTABLE_CONTAINS
       ( FK_NAME                            ,
         FK_COLNO                           ) REFERENCES W_SQLCTCLMN
;
CREATE UNIQUE INDEX MMWSQLCTCLMNSRFDMM
     ON MM_W_SQLCTCLMN_SRF
       ( FK_NAME                            ,
         FK_COLNO                           );

COMMIT;
