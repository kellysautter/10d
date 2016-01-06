/*

    KZHSQLDS.H - Prototype file needed for QuinSoft DB2 static SQL.

*/

#ifdef __cplusplus
extern "C" {
#endif

zSHORT LOCALOPER
SDB2_AllocBuffer( zPPCHAR pvBuffer,
                  long *  plLth,
                  long    lNewLth );

zSHORT LOCALOPER
SDB2_FreeBuffer( zPPVOID plpBuffer,
                 long *  plLth );

zSHORT LOCALOPER
SDB2_SetAttributeFromString( zVIEW        lpView,
                             LPVIEWOD     lpViewOD,
                             LPVIEWENTITY lpViewEntity,
                             zPCHAR       pszAttribName,
                             zPCHAR       pszValue,
                             short        nNullIndicator );

zSHORT LOCALOPER
SDB2_SetAttributeFromInteger( zVIEW        lpView,
                              LPVIEWOD     lpViewOD,
                              LPVIEWENTITY lpViewEntity,
                              zPCHAR       pszAttribName,
                              zLONG        lValue,
                              short        nNullIndicator );

zSHORT LOCALOPER
SDB2_SetAttributeFromBlob( zVIEW        lpView,
                           LPVIEWOD     lpViewOD,
                           LPVIEWENTITY lpViewEntity,
                           zPCHAR       pszAttribName,
                           zPVOID       pszValue,
                           short        sNullIndicator,
                           zULONG       uLth );

zSHORT LOCALOPER
SDB2_GetStringFromAttribute( zPCHAR   pszValue,
                             zVIEW    lpView,
                             LPVIEWOD lpViewOD,
                             zPCHAR   pszEntityName,
                             zPCHAR   pszAttribName );

zSHORT LOCALOPER
SDB2_GetIntegerFromAttribute( zPLONG   plValue,
                              zVIEW    lpView,
                              LPVIEWOD lpViewOD,
                              zPCHAR   pszEntityName,
                              zPCHAR   pszAttribName );

zSHORT LOCALOPER
SDB2_GetStringFromQualification( zPCHAR       pszValue,
                                 LPVIEWENTITY lpViewEntity,
                                 zPCHAR       pszAttribName,
                                 LPQUALENTITY lpQualEntity );

zSHORT LOCALOPER
SDB2_GetIntegerFromQualification( zPLONG       plValue,
                                  LPVIEWENTITY lpViewEntity,
                                  zPCHAR       pszAttribName,
                                  LPQUALENTITY lpQualEntity );

#ifdef __cplusplus
}
#endif

