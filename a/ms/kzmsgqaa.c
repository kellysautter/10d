#define KZSYSSVC_INCL
#include <kzoengaa.h>
#include "ZDRVROPR.H"

/*

   CHANGE LOG

2002.03.11   TMV
   change fnMQ_MessageSendServer and fnMQ_MessageSend to have a timestamp
   at the beginning of an error message which is useful in case of an engine
   to engine communication or a remote server situation.

2001.01.22   DGC
   R53324
   Change MQ_MessagePresent() to call MQ_MessageSend() to show the message
   instead of doing its own thing.  This should fix the error where messages
   where not being translated.

2001.01.22   HH
   improve overwriting of messages from land.msg so that any special info
   following \n would be kept.

2000.12.07   DGC
   Removed re-entrant check--this is performed by Core now.

2000.04.20   DGC
   Change re-entrant messages so they just trace a line.

2000.02.22   DGC
   Bumped up buffer length for szMsgText.

1998.02.12   DonC
   Modified MQ_MessagePresent to only present a single "Data validation error"
   message, even if multiple exist in the message object. Also switched all
   SQL Error messages to use OperatorPrompt instead of OperatorSend. (QS 153)

1998.01.16
   Modified MQ_MessagePrompt to also override messages from LAND.MSG.

1998.01.15   DonC
   Modified Domain Error message to use Title passed in.

1997.08.10   DonC
   Modified fnMQ_MessageSend to provide additional message when aborting
   system.

1997.12.17   DGC
   Changed MessageSend to fnMessageSendServer and fnMessageSend.
*/

#if 0
   The following are the states set by the driver.  The default message processing does use them to differentiate processing.

#define zMSGQ_Action                           1L
#define zMSGQ_Tabbing                          2L
#define zMSGQ_MapToOI                          3L
#define zMSGQ_DialogOperation                  4L

   The following are the states set by the whomever.  They may be relevant to some
   applications, but the default message processing does not take advantage of them.

#define  zMSGQ_OBJECT_CONSTRAINT_WARNING  100
#define  zMSGQ_DOMAIN_ERROR               200
#define  zMSGQ_OBJECT_CONSTRAINT_ERROR    300
#define  zMSGQ_REQUIRED_DATA_ITEM_ERROR   400
#define  zMSGQ_SYSTEM_ERROR              1000

#endif


static zVOID
AddSpecialInfo( zPCHAR pchMsgText, zCPCHAR szOrigText, zULONG nLth );

#if 0    // unused
zLONG
fnGetCurrentDriverState( zVIEW vMsgQ )
{
   zPCHAR lpStack;   // State Stack

   GetAddrForAttribute( (zCOREMEM) &lpStack, vMsgQ, "Task", "Status" );
   if ( lpStack == 0 )
      return( 0 );

   return( *((zPLONG) lpStack) );
}
#endif

// This function used by Zeidon Servers to create message object entities.

zSHORT LOCALOPER
fnMQ_MessageSendServer( zVIEW    vMsgQ,
                        zVIEW    vSubtask,
                        zCPCHAR  cpcMsgId,
                        zCPCHAR  cpcTitle,
                        zCPCHAR  cpcMsgText,
                        zLONG    lMsgType,
                        zSHORT   bBeep )
{
   zCHAR szTSMessage[ 5000 ]; // message with leading timestamp
   zCHAR szTimestamp[ 20 ];
   zCHAR szDateTime [ 32 ];

   zmemset( szTimestamp, 0, 17 );
   zmemset( szTSMessage, 0, 5000 );

   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   UfFormatDateTime( szDateTime, zsizeof( szDateTime ), szTimestamp, "YYYY-mm-DD-HH.MI.SS.999" );

   // Setup timestamped message buffer.
   strcpy_s( szTSMessage, zsizeof( szTSMessage ), szDateTime );
   strcat_s( szTSMessage, zsizeof( szTSMessage ), "\n" );

   if ( CreateEntity( vMsgQ, "QMsg", zPOS_LAST ) == 0 )
   {
      SetAttributeFromString( vMsgQ, "QMsg", "Id",  cpcMsgId );
      SetAttributeFromString( vMsgQ, "QMsg", "Text", cpcMsgText );
      SetAttributeFromString( vMsgQ, "QMsg", "Title", cpcTitle );
      SetAttributeFromInteger( vMsgQ, "QMsg", "Type", lMsgType );
      SetAttributeFromInteger( vMsgQ, "QMsg", "Beep", bBeep);
   }

   strcat_s( szTSMessage, zsizeof( szTSMessage ), cpcMsgText );

   // Also copy to TraceFile.
   SysMessageBox( vSubtask, cpcTitle, szTSMessage, 1 );
   return( 0 );
}

zSHORT LOCALOPER
fnMQ_MessageSend( zVIEW    vMsgQ,
                  zVIEW    vSubtask,
                  zCPCHAR  cpcMsgId,
                  zCPCHAR  cpcTitle,
                  zCPCHAR  cpcMsgText,
                  zLONG    lMsgType,
                  zSHORT   bBeep )
{
   zVIEW  vMsg;
   zLONG  hFile;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szMsgText[ 5000 ];
   zCHAR  szMsgTitle[ 200 ];
   zSHORT nMsgTypeFlag;
   zSHORT nRC;

   zCHAR szTSMessage[ 5000 ]; // message with leading timestamp
   zCHAR szTimestamp[ 20 ];
   zCHAR szDateTime[ 32 ];

   zmemset( szTimestamp, 0, zsizeof( szTimestamp ) );
   zmemset( szTSMessage, 0, zsizeof( szTSMessage ) );

   SysGetDateTime( szTimestamp, zsizeof( szTimestamp ) );
   UfFormatDateTime( szDateTime, zsizeof( szDateTime ), szTimestamp, "YYYY-mm-DD-HH.MI.SS.999" );

   // setup timestamped messagebuffer
   strcpy_s( szTSMessage, zsizeof( szTSMessage ), szDateTime );
   strcat_s( szTSMessage, zsizeof( szTSMessage ), "\n" );

   // This message processing routine has two major sections.
   // 1. A message object named LAND.MSG is activated, if it exists, for overriding messages with German equivalents.
   //    If the object is found and there is a match on Id, the new message attributes are used.
   // 2. OperatorPrompt is used to send the message, since we can control the icon on the message box with that operation.

   nMsgTypeFlag = 0;  // Initialize the flag to not overridden.

   if ( GetViewByName( &vMsg, "KZ_MessageText", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nRC = SetCursorFirstEntityByString( vMsg, "OverrideMsg", "Id", cpcMsgId, 0 );
      if ( nRC >= zCURSOR_SET )
         nMsgTypeFlag = 1;
   }
   else
   {
      SysGetLocalDirectory( szFileName );
      strcat_s( szFileName, zsizeof( szFileName ), "LAND.MSG" );
      hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
      if ( hFile >= 0 )
      {
         ActivateOI_FromFile( &vMsg, "KZMSGQOO", vSubtask, szFileName, zSINGLE );
         SetNameForView( vMsg, "KZ_MessageText", vSubtask, zLEVEL_TASK );
         nRC = SetCursorFirstEntityByString( vMsg, "OverrideMsg", "Id", cpcMsgId, 0 );
         if ( nRC >= zCURSOR_SET )
            nMsgTypeFlag = 1;
      }
   }

   if ( nMsgTypeFlag == 1 )   // An override message was found.
   {
      GetStringFromAttribute( szMsgText, zsizeof( szMsgText ), vMsg, "OverrideMsg", "Text" );
      // If the original message contains '\n', then we want to add everything after the \n.
      // This is done, because special information is added (Domain Messages), separated by \n.
      // We do not want to lose this information.
      AddSpecialInfo( szMsgText, cpcMsgText, zsizeof( szMsgText ) );
      GetStringFromAttribute( szMsgTitle, zsizeof( szMsgTitle ), vMsg, "OverrideMsg", "Title" );
   }
   else                       // No override message was found.
   {
      strncpy_s( szMsgText, zsizeof( szMsgText ), cpcMsgText, zsizeof( szMsgText ) - 1 );
      strncpy_s( szMsgTitle, zsizeof( szMsgTitle ), cpcTitle, zsizeof( szMsgTitle ) - 1 );
   }

   strcat_s( szTSMessage, zsizeof( szTSMessage ), szMsgText );
   TraceLineS( "", szTSMessage );

#ifdef NO_DRIVER
   SysMessageBox( szMsgTitle, szMsgText, 1 );
   return( zRESPONSE_OK );
#else
   switch( lMsgType )
   {
      case zMSGQ_DOMAIN_ERROR:
         return( OperatorPrompt( vSubtask, "Domain Validation Error", szMsgText, FALSE, 0, 0, zICON_STOP ) );

      case zMSGQ_OBJECT_CONSTRAINT_ERROR:
         return( OperatorPrompt( vSubtask, szMsgTitle, szMsgText, FALSE, 0, 0, zICON_STOP  ) );

      case zMSGQ_MODAL_ERROR:
         return( OperatorPrompt( vSubtask, szMsgTitle, szMsgText, FALSE, 0, 0, zICON_ERROR ) );

      case zMSGQ_OBJECT_CONSTRAINT_WARNING:
         return( OperatorPrompt( vSubtask, szMsgTitle, szMsgText, FALSE, 0, 0, zICON_EXCLAMATION ) );

      case zMSGQ_OBJECT_CONSTRAINT_INFORMATION:
         return( OperatorPrompt( vSubtask, szMsgTitle, szMsgText, FALSE, 0, 0, zICON_INFORMATION ) );
      default:
         if ( lMsgType > 500 )  // Messages over 500 should only be system errors.
         {
         // strcat_s( szMsgText, zsizeof( szMsgText ), "\n\n                 Cancel will abort task." );
            nRC = OperatorPrompt( vSubtask, szMsgTitle,
                                  szMsgText, FALSE, 0, 0,
                               // zBUTTONS_OKCANCEL, zRESPONSE_OK,
                                  zICON_EXCLAMATION );
            if ( nRC == zRESPONSE_CANCEL )
            {
               // This message should not be necessary since we are calling
               // AbortTask which causes an exception ... which in turn
               // causes the stack to be unwound properly ... and causes
               // the Driver task to be terminated.
            // nRC = OperatorPrompt( vSubtask, szMsgTitle,
            //                       "You must close all Zeidon tasks and the Object Engine after aborting.",
            //                       FALSE, 0, 0, zICON_INFORMATION );
            // AbortTask( "Task terminated" );
            }

            return( nRC );
         }
         else
         {
            return( OperatorPrompt( vSubtask, szMsgTitle, szMsgText, FALSE, 0, 0, zICON_EXCLAMATION ) );
         }
   }
#endif
}

zSHORT OPERATION
MQ_MessageSend( zVIEW    vMsgQ,
                zVIEW    vSubtask,
                zCPCHAR  cpcMsgId,
                zCPCHAR  cpcTitle,
                zCPCHAR  cpcMsgText,
                zLONG    lMsgType,
                zSHORT   bBeep )
{
   zSHORT nRC;

   if ( SfCurrentTaskIsServer( vSubtask ) )
      nRC = fnMQ_MessageSendServer( vMsgQ, vSubtask, cpcMsgId, cpcTitle, cpcMsgText, lMsgType, bBeep );
   else
      nRC = fnMQ_MessageSend( vMsgQ, vSubtask, cpcMsgId, cpcTitle, cpcMsgText, lMsgType, bBeep );

   return( nRC );
}

zSHORT OPERATION
MQ_MessagePrompt( zVIEW    vMsgQ,
                  zVIEW    vSubtask,
                  zCPCHAR  cpcMsgId,
                  zCPCHAR  cpcTitle,
                  zCPCHAR  cpcMsgText,
                  zSHORT   bBeep,
                  zSHORT   nButtons,
                  zSHORT   nDefaultButton,
                  zSHORT   nIcon )
{
   zVIEW  vMsg;
   zLONG  hFile;
   zCHAR  szFileName[ zMAX_FILENAME_LTH + 1 ];
   zCHAR  szMsgText[ 5000 ];
   zCHAR  szMsgTitle[ 200 ];
   zSHORT nMsgTypeFlag;
   zSHORT nRC;

   // This message processing routine has two major sections.
   // 1. A message object named LAND.MSG is activated, if it exists, for overriding messages with German equivalents.
   //    If the object is found and there is a match on Id, the new message attributes are used.
   // 2. OperatorPrompt is used to send the message, since we can control the icon on the message box with that operation.
   //
   nMsgTypeFlag = 0;  // Initialize the flag to not overridden.

   if ( GetViewByName( &vMsg, "KZ_MessageText", vSubtask, zLEVEL_TASK ) >= 0 )
   {
      nRC = SetCursorFirstEntityByString( vMsg, "OverrideMsg", "Id", cpcMsgId, 0 );
      if ( nRC >= zCURSOR_SET )
         nMsgTypeFlag = 1;
   }
   else
   {
      SysGetLocalDirectory( szFileName );
      strcat_s( szFileName, zsizeof( szFileName ), "LAND.MSG" );
      hFile = SysOpenFile( vSubtask, szFileName, COREFILE_READ );
      if ( hFile >= 0 )
      {
         ActivateOI_FromFile( &vMsg, "KZMSGQOO", vSubtask, szFileName, zSINGLE );
         SetNameForView( vMsg, "KZ_MessageText", vSubtask, zLEVEL_TASK );
         nRC = SetCursorFirstEntityByString( vMsg, "OverrideMsg", "Id", cpcMsgId, 0 );
         if ( nRC >= zCURSOR_SET )
            nMsgTypeFlag = 1;
      }
   }

   if ( nMsgTypeFlag == 1 )   // An override message was found.
   {
      GetStringFromAttribute( szMsgText, zsizeof( szMsgText ), vMsg, "OverrideMsg", "Text" );
      AddSpecialInfo( szMsgText, cpcMsgText, zsizeof( szMsgText ) );
      GetStringFromAttribute( szMsgTitle, zsizeof( szMsgTitle ), vMsg, "OverrideMsg", "Title" );
   }
   else                       // No override message was found.
   {
      strncpy_s( szMsgText, zsizeof( szMsgText ), cpcMsgText, zsizeof( szMsgText ) - 1 );
      strncpy_s( szMsgTitle, zsizeof( szMsgTitle ), cpcTitle, zsizeof( szMsgTitle ) - 1 );
   }

   TraceLine( "MessagePrompt: %s %s %s", cpcMsgId, szMsgTitle, szMsgText );

#ifdef NO_DRIVER
   SysMessageBox( "Zeidon Message Error", "Can't get prompt without driver", 1 );
   nRC = zCALL_ERROR;
#else
   nRC = OperatorPrompt( vSubtask, szMsgTitle, szMsgText, bBeep, nButtons, nDefaultButton, nIcon );
#endif

   return( nRC );
}

zSHORT OPERATION
MQ_MessagePromptForInput( zVIEW    vMsgQ,
                          zVIEW    vSubtask,
                          zCPCHAR  cpcMsgId,
                          zCPCHAR  cpcTitle,
                          zCPCHAR  cpcMsgText,
                          zSHORT   bBeep,
                          zPCHAR   lpchReturnBuffer,
                          zUSHORT  usBufferLth )
{
#ifdef NO_DRIVER
   SysMessageBox( "Zeidon Message Error", "Can't get prompt without driver", 1 );
   return( zCALL_ERROR );
#else
   return( OperatorPromptForInput( vSubtask, cpcTitle, cpcMsgText, bBeep, lpchReturnBuffer, usBufferLth ) );
#endif
}

zSHORT OPERATION
MQ_MessagePresent( zVIEW    vMsgQ,
                   zVIEW    vSubtask,
                   zLONG    lEventType )
{
   zCPCHAR cpcId;
   zCPCHAR cpcTitle;
   zCPCHAR cpcMsgText;
   zLONG   lState;
   zLONG   lMsgType;
   zSHORT  nRC;

   CreateViewFromViewForTask( &vMsgQ, vMsgQ, vSubtask );
   for( nRC = SetCursorLastEntity( vMsgQ, "QMsg", 0 );
        nRC >= zCURSOR_SET;
        nRC = DeleteEntity( vMsgQ, "QMsg", zREPOS_NEXT ) )
   {
      GetAddrForAttribute( (zCOREMEM) &cpcId, vMsgQ, "QMsg", "Id" );
      GetAddrForAttribute( (zCOREMEM) &cpcTitle, vMsgQ, "QMsg", "Title" );
      GetAddrForAttribute( (zCOREMEM) &cpcMsgText, vMsgQ, "QMsg", "Text" );
      GetIntegerFromAttribute( (zPLONG) &lMsgType, vMsgQ, "QMsg", "Type" );
      GetIntegerFromAttribute( (zPLONG) &lState, vMsgQ, "QMsg", "Status" );

      // We'll just use MessageSend to display the message.  NOTE that at the moment we don't too anything with lState.
      MQ_MessageSend( vMsgQ, vSubtask, cpcId, cpcTitle, cpcMsgText, lMsgType, TRUE );
   }

   DropView( vMsgQ );
   return( 0 );
}

// if the original message contains '\n', then we assume that everything following was added as special information,
// which must not be lost when a message is overwritten from land.msg.
static zVOID
AddSpecialInfo( zPCHAR pchMsgText, zCPCHAR szOrigText, zULONG ulMaxLth )
{
   zLONG k;
   zLONG lOrigLth = zstrlen( szOrigText );

   for ( k = 0; k < lOrigLth; k++ )
   { 
      if ( szOrigText[ k ] == '\n' )
      {
         strncat_s( pchMsgText, ulMaxLth, szOrigText + k, ulMaxLth - (zstrlen( pchMsgText ) + 1) );
         break;
      }
   }
}
