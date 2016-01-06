int PASCAL
zNetSend( LPSTR   pszNetworkName,
          LPVOID * ppHandle,
          LPVOID * ppvConnPtr,
          LPSTR   pszBuffer,
          UINT    uLth,
          char    cDataType );
int PASCAL
zNetReceive( LPSTR   pszNetworkName,
             LPVOID * ppHandle,
             LPVOID * ppvConnPtr,
             LPSTR * ppszReturnBuffer,
             UINT    uLth,
             char    cDataType );
int PASCAL
zNetListen( LPSTR   pszNetworkName,
            LPVOID * ppHandle );
int PASCAL
zNetStopListen( LPSTR   pszNetworkName,
                LPVOID * ppHandle );
int PASCAL
zNetStart( LPSTR     pszNetworkName,
           LPVOID *   ppHandle,
           HINSTANCE hInstance,
           HWND      hwndTrace );
int PASCAL
zNetClose( LPSTR     pszNetworkName,
           LPVOID *  ppHandle,
           HINSTANCE hInstance );
int PASCAL
zNetOpenConnection( LPSTR   pszNetworkName,
                    LPVOID * ppHandle,
                    LPVOID * ppvConnPtr );
int PASCAL
zNetCloseConnection( LPSTR   pszNetworkName,
                     LPVOID * ppHandle,
                     LPVOID * ppvConnPtr );
