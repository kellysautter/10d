/* KZLSTRAA.H - quinsoft utility C prototypes */
#ifndef __XZLSTR_H__
#define __XZLSTR_H__

#ifndef __cplusplus   // C++ will use standard libraries

#ifdef OS2_32         // OS/2 will use standard libraries

#else

zSHORT  OPERATION
zstrcmp( zCPCHAR szString1,
         zCPCHAR szString2 );
zSHORT  OPERATION
zstrncmp( zCPCHAR szString1,
          zCPCHAR szString2,
          zSHORT  nLth );
zSHORT  OPERATION
zstrcmpi( zCPCHAR szString1,
          zCPCHAR szString2 );
zSHORT  OPERATION
zstrncmpi( zCPCHAR szString1,
           zCPCHAR szString2,
           zSHORT  nLth );
zSHORT   OPERATION
zstrlen( zCPCHAR szString );
zPCHAR OPERATION
zstrcat( zPCHAR szString1,
         zCPCHAR szString2 );
zPCHAR   OPERATION
zstrcatp( zPCHAR szString1,
          zCPCHAR szString2 );
zSHORT   OPERATION
zstrcatl( zPCHAR szString1,
          zCPCHAR szString2 );
zPCHAR OPERATION
zstrncat( zPCHAR szString1,
          zCPCHAR szString2,
          zSHORT  nLth );
zPCHAR OPERATION
zstrcpy( zPCHAR szString1,
         zCPCHAR szString2 );
zSHORT   OPERATION
zstrcpyl( zPCHAR szString1,
          zCPCHAR szString2 );
zPCHAR OPERATION
zstrncpy( zPCHAR szString1,
          zCPCHAR szString2,
          zSHORT  nLth );
zPCHAR OPERATION
zstrchr( zPCHAR szString,
         char cChar );
zPCHAR OPERATION
zstrtok( zPCHAR szString,
         zPCHAR szDelimiter );
zPCHAR OPERATION
zstrrev( zPCHAR szString );
zSHORT   OPERATION
zstrskips( zPCHAR szString );
zSHORT   OPERATION
zstrskipw( zPCHAR szString );
zSHORT   OPERATION
zstrtrims( zPCHAR szString );
zPCHAR OPERATION
zstrtrimw( zPCHAR szString );
zSHORT   OPERATION
zstrjusts( zPCHAR szString );
zSHORT   OPERATION
zstrjustw( zPCHAR szString );
void  OPERATION
zstrjustr( zPCHAR szString,
           zUSHORT wDisplayLth );
void OPERATION
zstrblank( zPCHAR szString,
           zSHORT  nLth,
           zSHORT bNullLastByte );
zSHORT   OPERATION
zmemcmp( zCPVOID pBuffer1,
         zCPVOID pBuffer2,
         zSHORT  nLth );
zSHORT   OPERATION
zmemcmpi( zCPVOID pBuffer1,
          zCPVOID pBuffer2,
          zSHORT  nLth );
void  OPERATION
zmemset( zPVOID pBuffer,
         zCHAR  cChar,
         zSHORT nLth );
void  OPERATION
zmemcpy( zPVOID  pBuffer1,
         zCPVOID pBuffer2,
         zSHORT  nLth );
void  OPERATION
zmemmov( zPVOID pBuffer1,
         zPVOID pBuffer2,
         zSHORT nLth );
zLONG  OPERATION
zgetdate( void );
zLONG  OPERATION
zgettime( void );
zLONG  OPERATION
zatol( zCPCHAR szAsciiString );
zLONG  OPERATION
zxtol( zCPCHAR szHexAsciiString );
void  OPERATION
zltoa( zLONG lValue,
       zPCHAR szAsciiString );
void  OPERATION
zltoal( zLONG lValue,
        zPCHAR szAsciiString,
        zSHORT  nLth );
zSHORT   OPERATION
zfixpath( zPCHAR szPathString );
zSHORT   OPERATION
ztermpath( zPCHAR szPathString );
zSHORT   OPERATION
zsetfsize( zSHORT  fh,
           zLONG dwSize );
zLONG  OPERATION
zgetfsize( zSHORT  fh );
zSHORT   OPERATION
zsetftime( zSHORT  fh,
           zLONG dwDateTime );
zLONG  OPERATION
zgetftime( zSHORT  fh );

/***  MS Helper functions ****/

zSHORT OPERATION
GetCurrentDrive( zPCHAR );

zSHORT OPERATION
GetCurrentDirectory( zUSHORT, zPCHAR );

zSHORT OPERATION
SetCurrentDrive( zCPCHAR );

zSHORT OPERATION
SetCurrentDirectory( zCPCHAR );

#endif   // #ifndef OS2_32

#endif   // #ifndef __cplusplus

#endif   // #ifndef __XZLSTR_H__


