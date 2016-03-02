#include <jni.h>  // Java Native Interface

#define  KZSYSSVC_INCL
#include "kzoengaa.h"
#include "kzoetdaa.h"
#include "kzoep0aa.h"
#include "kzoemiaa.h"

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// To change a signature, we need to modify the operation in this file
// as well as the corresponding signature and operation in zView.java.
// As we can see in the definition of CallDialogOperation below, each
// string is defined by Ljava_lang_String_2 and the integer is defined
// by the I.
//
// Class:     zeidon_zView
// Method:    CallDialogOperation( vSubtask, szDialog, szOperation )
//       This just sets cursor position by index.
// Signature: (Ljava/lang/String;Ljava/lang/String;I;)S
//
//JNIEXPORT jshort JNICALL
//Java_zeidon_zView_niCallDialogOperation__Ljava_lang_String_2Ljava_lang_String_2I
//               ( JNIEnv   *jEnv,
//                 jobject  this,
//                 jstring  jstrDialog,
//                 jstring  jstrOperation,
//                 jint     jiControl )
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

// KJS 05/15/09 - Originally we were using the JNU_GetStringNat... and the ReleaseStringUTFC...
// but when we tried to move to our new Windows 2008 server, tomcat kept crashing on the
// RELEASESTRING and I found data that said you should only do the ReleaseString... when first
// doing a GetString...  Now we are using free() with the JNU_GetS...
//#define GETSTRING( js )         (js == 0 ? 0 : (*jEnv)->GetStringUTFChars( jEnv, js, NULL ))
//#define RELEASESTRING( js, s )  if ( js ) (*jEnv)->ReleaseStringUTFChars( jEnv, js, s )
#define GETSTRING( js )         (js == 0 ? 0 : JNU_GetStringNativeChars(jEnv, js ))
#define RELEASESTRING( js, s )  if ( js ) free( (char *)s )

typedef zSHORT (POPERATION zFARPROC_DYNOBJ)( zVIEW );
zPVOID OPERATION
GetOperationDynamicCallAddress( zVIEW       vSubtask,
                                LPLPLIBRARY lpLibrary,
                                zCPCHAR     cpcDLL_Name,
                                zCPCHAR     cpcOperationName,
                                zCPCHAR     cpcErrorMessagePrefix );
zOPER_EXPORT zSHORT OPERATION
GetWebRedirection( zVIEW   vSubtask,
                   zLONG   lWindowBehavior,
                   zPCHAR  pchDlgTag,
                   zPCHAR  pchWndTag );
zOPER_EXPORT zSHORT OPERATION
SetWebRedirection( zVIEW   vSubtask,
                   zLONG   lWindowBehavior,
                   zCPCHAR cpcDlgTag,
                   zCPCHAR cpcWndTag );
zLONG  OPERATION
PrintReportToHTML( zVIEW   vSubtask,
                   zVIEW   vSourceOI,
                   zCPCHAR cpcSubobjectEntityName,
                   zCPCHAR cpcReportDefName,
                   zPCHAR  pchReportName,
                   zLONG   lFlags );

static jmethodID MID_String_getBytes;

zPCHAR
JNU_GetStringNativeChars( JNIEnv *jEnv, jstring jStr )
{
    jbyteArray byteArray = 0;
    jthrowable throwableException;
    zPCHAR pchResult = 0;
    jclass stringClass = (*jEnv)->FindClass( jEnv, "java/lang/String" );
    MID_String_getBytes = (*jEnv)->GetMethodID( jEnv, stringClass, "getBytes", "()[B" );

    if ( (*jEnv)->EnsureLocalCapacity( jEnv, 2 ) < 0 )
    {
       return 0; // out of memory error
    }

    byteArray = (*jEnv)->CallObjectMethod( jEnv, jStr, MID_String_getBytes );
    throwableException = (*jEnv)->ExceptionOccurred( jEnv );
    if ( !throwableException )
    {
        jint jLth = (*jEnv)->GetArrayLength( jEnv, byteArray );
        pchResult = (zPCHAR) malloc( jLth + 1 );
        if ( pchResult == 0 )
        {
            (*jEnv)->DeleteLocalRef( jEnv, byteArray );
            return 0;
        }

        (*jEnv)->GetByteArrayRegion( jEnv, byteArray, 0, jLth, (jbyte *) pchResult );
        pchResult[ jLth ] = 0; // NULL-terminate
    }
    else
    {
        (*jEnv)->DeleteLocalRef(jEnv, throwableException);
    }

    (*jEnv)->DeleteLocalRef( jEnv, byteArray );
    (*jEnv)->DeleteLocalRef( jEnv, stringClass );
    return pchResult;
}

// zVIEW g_vRegSubtask = 0;
zLONG    g_lInstance = 0;
zLONG    g_hWnd      = 0;
zCHAR    g_szClassName[ ] = "KZOE_Tomcat";
zUSHORT  g_wClientMessage = 0;
zSHORT   g_nTraceAction = 0;
#define IDM_GETACTIONTRACE 204
#define IDM_GETWEBTRACE    214

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

zVIEW
fnGetView( JNIEnv   *jEnv,
           jobject  this )
{
   static jclass   jcView = 0;
   static jfieldID jFieldId = 0;

   if ( this == 0 )
      return( 0 );

   // This doesn't work.  I'll try something different later.
// if ( jcView == 0 )
   {
      jcView   = (*jEnv)->GetObjectClass( jEnv, this );
      jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   }

   return( (zVIEW) (*jEnv)->GetIntField( jEnv, this, jFieldId ) );
}

void
fnSetView( JNIEnv   *jEnv,
           jobject  this,
           zVIEW    vView )
{
   static jclass   jcView = 0;
   static jfieldID jFieldId = 0;

// if ( jcView == 0 )  This doesn't work.  I'll try something different later.
   {
      jcView   = (*jEnv)->GetObjectClass( jEnv, this );
      jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   }

   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vView );

   return;
}

// Constructs a Java string from a C array using the String(byte [])
// constructor, which uses default local encoding.
jstring
fnNewJavaString( JNIEnv *jEnv, const char *str )
{
   static jstring   classString    = 0;
   static jmethodID String_init_ID = 0;

   jstring    jstrResult;
   jbyteArray hab = 0;
   int        iLth;

// if ( classString == 0 )  This doesn't work.  I'll try something different later.
   {
      classString = (*jEnv)->FindClass( jEnv, "java/lang/String" );
      String_init_ID = (*jEnv)->GetMethodID(jEnv, classString, "<init>", "([B)V");
   }

   iLth = zstrlen( str );
   hab = (*jEnv)->NewByteArray( jEnv, iLth );
   if ( hab )
   {
      (*jEnv)->SetByteArrayRegion( jEnv, hab, 0, iLth, (jbyte *) str );
      jstrResult = (*jEnv)->NewObject( jEnv, classString, String_init_ID, hab );
      (*jEnv)->DeleteLocalRef( jEnv, hab );
      return( jstrResult );
   }

   return( 0 );
}

jobject
fnNewJavaInteger( JNIEnv *jEnv, long lValue )
{
   static jclass    classInteger    = 0;
   static jmethodID Integer_init_ID = 0;

   jobject result;

   // This doesn't work.  I'll try something different later.
// if ( classInteger == 0 )
   {
      classInteger = (*jEnv)->FindClass( jEnv, "java/lang/Integer" );
      Integer_init_ID = (*jEnv)->GetMethodID( jEnv, classInteger,
                                             "<init>", "(I)V" );
   }

   result = (*jEnv)->NewObject( jEnv, classInteger, Integer_init_ID,
                               (jint) lValue );
   return( result );
}

/////////////////////////////////////////////////////////////////////////////
//
//  General methods.
//
/////////////////////////////////////////////////////////////////////////////

/*
 * Class:     zeidon_zView
 * Method:    niRegisterZeidonApplication
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niRegisterZeidonApplication
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrAppName )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcAppName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jfieldID   jFieldId;

   // If g_vRegSubtask is non-zero then we've already registered this task so exit.
// if ( g_vRegSubtask )
//    return( zCALL_ERROR );

   // Get each of the arguments.
   cpcAppName = GETSTRING( jstrAppName );
   cpcSessionId = GETSTRING( jstrSessionId );

// if ( g_hWnd == 0 )
// {
//    g_wClientMessage = SysGetClientMessage( );
//    g_hWnd = SysInitApplicationWindow( g_lInstance, g_szClassName, "KZOE_TomcatTitle" );
// }

   if ( (vSubtask = FindWebSubtask( cpcSessionId )) == 0 )
   {
      nRC = RegisterZeidonApplication( &vSubtask, g_lInstance, g_hWnd,
                                       g_wClientMessage, cpcAppName, cpcSessionId, 1 );
      SfLockView( vSubtask );
   // printf( "Registering Application: %s   Session: %s\n", cpcAppName, cpcSessionId );
      TraceLine( "(java) Registering application %s   Subtask: 0x%08x for SessionId: %s",
                 cpcAppName, vSubtask, cpcSessionId );

      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "RegisterZeidonApplication" );
      TraceLine( "(java) Registering application after SetTransientTaskProcess", "");
   // GarbageCollectViews( vSubtask, 0 );
      g_nTraceAction = (zSHORT) SysSendOE_Message( IDM_GETACTIONTRACE, 0 );
      TraceLine( "(java) Registering application after SysSendOE_Message", "");
   }
   else
   {
      nRC = 1;  // normal successful register returns 0
      TraceLine( "(java) Application already registered %s   Subtask: 0x%08x for SessionId: %s",
                 cpcAppName, vSubtask, cpcSessionId );
   }

   TraceLine( "(java) Before first Releasestring", "");

   // Seems like we should use free when we've used JNI_GetNative....  But if we use GetUTF then we should use
   // releaseUTF.
   // We get a compile warning when doing the following free call.  Not sure what the problem is.
   //free(cpcAppName);
   RELEASESTRING( jstrAppName, cpcAppName );  //???
   TraceLine( "(java) Before second Releasestring", "");
   RELEASESTRING( jstrSessionId, cpcSessionId );
   TraceLine( "(java) after Releasestring", "");

// g_vRegSubtask = vSubtask;
   TraceLine( "(java) Before GetFieldID", "");

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   TraceLine( "(java) Before SetIntField", "");
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vSubtask );

   TraceLine( "(java) Registering application Before Return", "");

   return( nRC );
}

#if 0
/*
 * Class:     zeidon_zView
 * Method:    niRegisterZeidonAppForThread
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niRegisterZeidonAppForThread
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrAppName )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcAppName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jfieldID   jFieldId;

   // If g_vRegSubtask is non-zero then we've already registered this task ... so exit.
// if ( g_vRegSubtask )
//    return( zCALL_ERROR );

   // Get each of the arguments.
   cpcAppName = GETSTRING( jstrAppName );
   cpcSessionId = GETSTRING( jstrSessionId );

   nRC = RegisterZeidonAppForThread( 0, &vSubtask, cpcAppName, cpcSessionId, 1 );
   TraceLineS( "(java) Registering thread application: ", cpcAppName );

   RELEASESTRING( jstrAppName, cpcAppName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

// g_vRegSubtask = vSubtask;

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vSubtask );

   return( nRC );
}
#endif

/*
 * Class:     zeidon_zView
 * Method:    niUnregisterZeidonApplication
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niUnregisterZeidonApplication
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jfieldID   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   zVIEW      vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, this, jFieldId );
   zSHORT     nRC;

   // If the view for the current (this) object is NOT the same as the global
   // subtask pointer then don't worry about it.
// if ( vSubtask != g_vRegSubtask )
//    return( 0 );

// g_vRegSubtask = 0;

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   TraceLine( "(java) Unregistering Subtask: 0x%08x", vSubtask );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "UnregisterZeidonApplication" ) == 0 )
      nRC = UnregisterZeidonApplication( vSubtask );
   else
      nRC = -1;

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetDefaultViewForActiveTask
 * Signature: (Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetDefaultViewForActiveTask
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jint     jiState )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

// SetDefaultViewForActiveTask( vSubtask, jiState ); deprecated

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    niDisplayObjectInstance
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDisplayObjectInstance
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vView;
   zCPCHAR    cpcSessionId;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DisplayObjectInstance" ) == 0 )
   {
      DisplayObjectInstance( vView, 0, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DisplayObjectInstance" );
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );
   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    niDisplayEntityInstance
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDisplayEntityInstance
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DisplayEntityInstance" ) == 0 )
   {
      DisplayEntityInstance( vView, cpcEntityName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DisplayEntityInstance" );
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );
   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    niGarbageCollectViews
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niGarbageCollectViews
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "GarbageCollectViews" ) == 0 )
   {
      GarbageCollectViews( vSubtask, 0 );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "GarbageCollectViews" );
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );
   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetIntegerFromView
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_zeidon_zView_niGetIntegerFromView
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vView;
   zCPCHAR    cpcSessionId;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( (int) vView );
}

/*
 * Class:     zeidon_zView
 * Method:    niActivateEmpty
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niActivateEmpty
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrLodName,
              jobject  jvSubtask,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zVIEW      vView;
   zCPCHAR    cpcLodName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zSHORT     nRC;

   // Get the view for 'this'.
   jFieldId   = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask   = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   // Get each of the arguments.
   cpcLodName = GETSTRING( jstrLodName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "ActivateEmptyObjectInstance" ) == 0 )
   {
      nRC = ActivateEmptyObjectInstance( &vView, cpcLodName, vSubtask, jiControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "ActivateEmptyObjectInstance" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrLodName, cpcLodName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSfActivateSysEmptyOI
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSfActivateSysEmptyOI
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrLodName,
              jobject  jvSubtask,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zVIEW      vView;
   zCPCHAR    cpcLodName = 0;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zSHORT     nRC;

   // Get the view for 'this'.
   jFieldId   = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask   = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   // Get each of the arguments.
   cpcLodName = GETSTRING( jstrLodName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "SfActivateSysEmptyOI" ) == 0 )
   {
      nRC = SfActivateSysEmptyOI( &vView, cpcLodName, vSubtask, jiControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "SfActivateSysEmptyOI" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrLodName, cpcLodName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niActivate
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niActivate__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2I
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrLodName,
              jobject  jvSubtask,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zVIEW      vView;
   zCPCHAR    cpcLodName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask   = fnGetView( jEnv, jvSubtask );

   // Get each of the arguments.
   cpcLodName = GETSTRING( jstrLodName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "ActivateObjectInstance" ) == 0 )
   {
      nRC = ActivateObjectInstance( &vView, cpcLodName, vSubtask, 0, jiControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "ActivateObjectInstance" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrLodName, cpcLodName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   fnSetView( jEnv, this, vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niActivate
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;Lzeidon/zView;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niActivate__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2Lzeidon_zView_2I
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrLodName,
              jobject  jvSubtask,
              jobject  jvQualOI,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zVIEW      vQualOI;
   zVIEW      vView;
   zCPCHAR    cpcLodName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask   = fnGetView( jEnv, jvSubtask );
   vQualOI    = fnGetView( jEnv, jvQualOI );

   // Get each of the arguments.
   cpcLodName = GETSTRING( jstrLodName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "ActivateObjectInstance" ) == 0 )
   {
      nRC = ActivateObjectInstance( &vView, cpcLodName, vSubtask, vQualOI, jiControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "ActivateObjectInstance" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrLodName, cpcLodName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   fnSetView( jEnv, this, vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niActivateFromFile
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niActivateFromFile
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrLodName,
              jobject  jvSubtask,
              jstring  jstrFileName,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zVIEW      vView;
   zCPCHAR    cpcLodName;
   zCPCHAR    cpcFileName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask    = fnGetView( jEnv, jvSubtask );

   // Get each of the arguments.
   cpcLodName  = GETSTRING( jstrLodName );
   cpcFileName = GETSTRING( jstrFileName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "ActivateOI_FromFile" ) == 0 )
   {
      nRC = ActivateOI_FromFile( &vView, cpcLodName, vSubtask, cpcFileName, jiControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "ActivateOI_FromFile" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrLodName, cpcLodName );
   RELEASESTRING( jstrFileName, cpcFileName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   fnSetView( jEnv, this, vView );

   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    Commit
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_Commit
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vView;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "CommitObjectInstance" ) == 0 )
   {
      nRC = CommitObjectInstance( vView );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "CommitObjectInstance" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    CommitOI_ToXML_File
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_CommitOI_1ToXML_1File
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrFileName,
              jint     jlControl )
{
   zVIEW      vView;
   zCPCHAR    cpcFileName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcFileName = GETSTRING( jstrFileName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "CommitOI_ToXML_File" ) == 0 )
   {
      nRC = CommitOI_ToXML_File( vView, cpcFileName, jlControl );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "CommitOI_ToXML_File" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrFileName, cpcFileName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    CommitSubobjectToXML_File
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_CommitSubobjectToXML_1File
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrFileName,
              jstring  jstrSubobjectRoot,
              jint     jlControl )
{
   zVIEW      vView;
   zCPCHAR    cpcFileName;
   zCPCHAR    cpcSubobjectRoot = 0;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcFileName = GETSTRING( jstrFileName );
   cpcSessionId = GETSTRING( jstrSessionId );
   if ( jstrSubobjectRoot )
      cpcSubobjectRoot = GETSTRING( jstrSubobjectRoot );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "CommitSubobjectToXML_File" ) == 0 )
   {
      nRC = CommitSubobjectToXML_File( vView, cpcFileName,
                                       cpcSubobjectRoot, jlControl );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "CommitSubobjectToXML_File" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrFileName, cpcFileName );
   if ( jstrSubobjectRoot )
      RELEASESTRING( jstrSubobjectRoot, cpcSubobjectRoot );

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    GetTableListForAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/util/Vector;
 */
JNIEXPORT jobject JNICALL
Java_zeidon_zView_GetTableListForAttribute
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jstring  jstrContextName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcContextName;
   zCPCHAR    cpcSessionId;
   jclass     classVector;
   jstring    line1;
   jmethodID  Vector_init_ID;
   jmethodID  Vector_add_ID;
   jobject    oVector;
   char       szTableEntry[ 256 ];
   void       *pv;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName  = GETSTRING( jstrEntityName );
   cpcAttribName  = GETSTRING( jstrAttribName );
   cpcContextName = GETSTRING( jstrContextName );
   cpcSessionId = GETSTRING( jstrSessionId );

   // Get the class info for the Vector class.
   classVector = (*jEnv)->FindClass( jEnv, "java/util/Vector" );
   Vector_init_ID = (*jEnv)->GetMethodID( jEnv, classVector, "<init>", "()V");
   Vector_add_ID  = (*jEnv)->GetMethodID( jEnv, classVector, "add",
                                          "(Ljava/lang/Object;)Z" );

   // Create a new vector instance.
   oVector = (*jEnv)->NewObject( jEnv, classVector, Vector_init_ID );

   // Test...try adding a new line.
// line1 = fnNewJavaString( jEnv, "This is line 1" );
// (*jEnv)->CallBooleanMethod( jEnv, oVector, Vector_add_ID, line1 );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "GetTableListForAttribute" ) == 0 )
   {

      // Call OE to get list of table entries.
      for ( nRC = GetFirstTableEntryForAttribute( szTableEntry,  zsizeof( szTableEntry ), vView, cpcEntityName,
                                                  cpcAttribName, cpcContextName, &pv );
            nRC >= 0;
            nRC = GetNextTableEntryForAttribute( szTableEntry, zsizeof( szTableEntry ), vView, cpcEntityName,
                                                 cpcAttribName, cpcContextName, &pv ) )
      {
         // Convert the string returned from OE to a java string.
         line1 = fnNewJavaString( jEnv, szTableEntry );

         // Add the java string to the vector.
         (*jEnv)->CallBooleanMethod( jEnv, oVector, Vector_add_ID, line1 );
      }

      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetTableListForAttribute" );
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrContextName, cpcContextName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( oVector );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetView
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niGetView__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName )
{
   zVIEW      vReturnView;
   zVIEW      vSubtask;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jfieldID   jFieldId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   zsprintf( szMsg, "GetViewByName V/Task: 0x%08x / 0x%08x  %s",
             vSubtask, SysGetTaskFromView( vSubtask ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      nRC = GetViewByName( &vReturnView, cpcViewName, vSubtask, zLEVEL_TASK );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   }
   else
   {
      vReturnView = 0;
      nRC = -1;
   }

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vReturnView );

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetView
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niGetView__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jobject  jvSubtask )
{
   zVIEW      vSubtask;
   zVIEW      vReturnView;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   zsprintf( szMsg, "GetViewByName V/Task: 0x%08x / 0x%08x  %s",
             vSubtask, SysGetTaskFromView( vSubtask ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      nRC = GetViewByName( &vReturnView, cpcViewName, vSubtask, zLEVEL_TASK );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   }
   else
   {
      vReturnView = 0;
      nRC = -1;
   }

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vReturnView );

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetView
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niGetView__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2S
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jobject  jvSubtask,
              jshort   jnControl )
{
   zVIEW      vSubtask;
   zVIEW      vReturnView;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   zsprintf( szMsg, "GetViewByName V/Task: 0x%08x / 0x%08x  %s",
             vSubtask, SysGetTaskFromView( vSubtask ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      nRC = GetViewByName( &vReturnView, cpcViewName, vSubtask, jnControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   }
   else
   {
      vReturnView = 0;
      nRC = -1;
   }

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vReturnView );

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetName
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetName__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName )
{
   zVIEW      vView;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   zsprintf( szMsg, "SetNameForView V/Task: 0x%08x / 0x%08x  %s",
             vView, SysGetTaskFromView( vView ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, szMsg ) == 0 )
   {
      nRC = SetNameForView( vView, cpcViewName, vView, zLEVEL_TASK );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, szMsg );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetName
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetName__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jobject  jvSubtask )
{
   zVIEW      vView;
   zVIEW      vSubtask;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   zsprintf( szMsg, "SetNameForView V/Task: 0x%08x / 0x%08x  %s",
             vSubtask, SysGetTaskFromView( vSubtask ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      nRC = SetNameForView( vView, cpcViewName, vSubtask, zLEVEL_TASK );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetName
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetName__Ljava_lang_String_2Ljava_lang_String_2Lzeidon_zView_2S
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jobject  jvSubtask,
              jshort   jnControl )
{
   zVIEW      vView;
   zVIEW      vSubtask;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zCHAR      szMsg[ 256 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   zsprintf( szMsg, "SetNameForView V/Task: 0x%08x / 0x%08x  %s",
             vSubtask, SysGetTaskFromView( vSubtask ), cpcViewName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      nRC = SetNameForView( vView, cpcViewName, vSubtask, jnControl );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niCreateViewFromView
 * Signature: (Ljava/lang/String;Lzeidon/zView;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niCreateViewFromView
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jobject  jvSrcView )
{
   zVIEW      vSrcView;
   zVIEW      vView;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   vSrcView = fnGetView( jEnv, jvSrcView );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSrcView, TRUE, "CreateViewFromViewForTask" ) == 0 )
   {
      nRC = CreateViewFromViewForTask( &vView, vSrcView, vSrcView );
      SetTransientTaskProcess( cpcSessionId, vSrcView, FALSE, "CreateViewFromViewForTask" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   // Set the 'this' view.
   fnSetView( jEnv, this, vView );

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niDropName
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDropName
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName )
{
   zVIEW      vView;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DropNameForView" ) == 0 )
   {
      nRC = DropNameForView( vView, cpcViewName, vView, zLEVEL_TASK );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DropNameForView" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niDropView
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDropView
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vView;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DropView" ) == 0 )
   {
      nRC = DropView( vView );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DropView" );
   }
   else
   {
      nRC = -1;
   }

   vView = 0;

   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   fnSetView( jEnv, this, vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niDropObjectInstance
 * Signature: (Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDropObjectInstance
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vView;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DropObjectInstance" ) == 0 )
   {
      nRC = DropObjectInstance( vView );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DropObjectInstance" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );

   vView = 0;

   // Set the return view.
   fnSetView( jEnv, this, vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niTraceLine
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL
Java_zeidon_zView_niTraceLine__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jclass   jcView,
              jstring  jstr1,
              jstring  jstr2 )
{
   zCPCHAR  cpc1;
   zCPCHAR  cpc2;

   // Get each of the arguments.
   cpc1 = GETSTRING( jstr1 );
   cpc2 = GETSTRING( jstr2 );

   TraceLineS( cpc1, cpc2 );

   RELEASESTRING( jstr1, cpc1 );
   RELEASESTRING( jstr2, cpc2 );
}

/*
 * Class:     zeidon_zView
 * Method:    niTraceLine
 * Signature: (Ljava/lang/String;I)V
 */
JNIEXPORT void JNICALL
Java_zeidon_zView_niTraceLine__Ljava_lang_String_2I
            ( JNIEnv   *jEnv,
              jclass   jcView,
              jstring  jstr1,
              jint     jInt1 )
{
   zCPCHAR    cpc1;

   // Get each of the arguments.
   cpc1 = GETSTRING( jstr1 );

   TraceLineI( cpc1, jInt1 );

   RELEASESTRING( jstr1, cpc1 );
}

/*
 * Class:     zeidon_zView
 * Method:    niTraceLine
 * Signature: (Ljava/lang/String;Lzeidon/zView;)V
 */
JNIEXPORT void JNICALL
Java_zeidon_zView_niTraceLine__Ljava_lang_String_2Lzeidon_zView_2
            ( JNIEnv   *jEnv,
              jclass   jcView,
              jstring  jstr1,
              jobject  this )
{
   zCPCHAR    cpc1;
   zVIEW      vView;

   // Get each of the arguments.
   cpc1 = GETSTRING( jstr1 );

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   TraceLine( "%s 0x%08x", cpc1, vView );

   RELEASESTRING( jstr1, cpc1 );

   return;
}

/////////////////////////////////////////////////////////////////////////////
//
//  OI methods.
//
/////////////////////////////////////////////////////////////////////////////

/*
 * Class:     zeidon_zView
 * Method:    niSetOIFromBlob
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;Lzeidon/zView;Ljava/lang/String;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetOIFromBlob
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrTgtLodName,
              jobject  jvSubtask,
              jobject  jvSrcView,
              jstring  jstrSrcEntityName,
              jstring  jstrSrcAttrName,
              jint     jiNbr )
{
   zVIEW      vTgtView;
   zVIEW      vSrcView;
   zVIEW      vSubtask;
   zCHAR      szTgtLodName[ 32 ];
   zCPCHAR    cpcTgtLodName;
   zCPCHAR    cpcSrcEntityName;
   zCPCHAR    cpcSrcAttrName;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask ); // ???
   jfieldID   jFieldId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vTgtView = 0;
   vSrcView = fnGetView( jEnv, jvSrcView );
   vSubtask = fnGetView( jEnv, jvSubtask );

   // Get each of the arguments.
   cpcTgtLodName = GETSTRING( jstrTgtLodName );
   cpcSrcEntityName = GETSTRING( jstrSrcEntityName );
   cpcSrcAttrName = GETSTRING( jstrSrcAttrName );
   cpcSessionId = GETSTRING( jstrSessionId );

   //jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   //vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask /*vSrcView*/, TRUE, "SetOI_FromBlob" ) == 0 )
   {
      // KJS 10/16/08 - SetOI_FromBlob sends the target name back so szTgtLodName should really
      // be a pointer to a string.  Not sure why but c (or is it java) doesn't handle this (from Doug), so since
      // I really don't care what the target lod name is (it's usually zero) I am simply creating a regular
      // string and not expecting to get something back.  Now the code compiles.
      // Look at kzoeataa.c.
      strcpy_s( szTgtLodName, zsizeof( szTgtLodName ), cpcTgtLodName );
      nRC = SetOI_FromBlob( &vTgtView, szTgtLodName,
                            vSubtask, vSrcView, cpcSrcEntityName, cpcSrcAttrName, jiNbr );
      SetTransientTaskProcess( cpcSessionId, vSubtask /*vSrcView*/, FALSE, "SetOI_FromBlob" );
   }
   else
   {
      vTgtView = 0;
      nRC = -1;
   // nRC = zCALL_ERROR; ???
   }

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vTgtView );

   RELEASESTRING( jstrTgtLodName, cpcTgtLodName );
   RELEASESTRING( jstrSrcEntityName, cpcSrcEntityName );
   RELEASESTRING( jstrSrcAttrName, cpcSrcAttrName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetBlobFromOI
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetBlobFromOI
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrTgtEntityName,
              jstring  jstrTgtAttrName,
              jobject  jvSrcView,
              jint     jiNbr )
{
   zVIEW      vTgtView;
   zVIEW      vSrcView;
   zCPCHAR    cpcTgtEntityName;
   zCPCHAR    cpcTgtAttrName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vTgtView = fnGetView( jEnv, this );
   vSrcView = fnGetView( jEnv, jvSrcView );

   // Get each of the arguments.
   cpcTgtEntityName = GETSTRING( jstrTgtEntityName );
   cpcTgtAttrName = GETSTRING( jstrTgtAttrName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSrcView, TRUE, "SetBlobFromOI" ) == 0 )
   {
      nRC = SetBlobFromOI( vTgtView, cpcTgtEntityName,
                           cpcTgtAttrName, vSrcView, jiNbr );
      SetTransientTaskProcess( cpcSessionId, vSrcView, FALSE, "SetBlobFromOI" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrTgtEntityName, cpcTgtEntityName );
   RELEASESTRING( jstrTgtAttrName, cpcTgtAttrName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niOrderEntityForView
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niOrderEntityForView
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jobject  jvView,
              jstring  jstrEntityName,
              jstring  jstrOrderKeys)
{
   zVIEW      vView;
   zVIEW      vSubtask;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcOrderKeys;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jfieldID   jFieldId;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );
   vView = fnGetView( jEnv, jvView );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcOrderKeys = GETSTRING( jstrOrderKeys );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "OrderEntityForView" ) == 0 )
   {
      //nRC = OrderEntityForView( &vView, cpcEntityName, cpcOrderKeys );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "OrderEntityForView" );
   }
   else
   {
      nRC = -1;
   }

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vView );

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrOrderKeys, cpcOrderKeys );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  Entity methods.
//
/////////////////////////////////////////////////////////////////////////////

/*
 * Class:     zeidon_zView
 * Method:    niCreateEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niCreateEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jshort   jsPosition )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "CreateEntity" ) == 0 )
   {
      nRC = CreateEntity( vView, cpcEntityName, jsPosition );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "CreateEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetSelectStateOfEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetSelectStateOfEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jshort   jsState )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetSelectStateOfEntity" ) == 0 )
   {
      nRC = SetSelectStateOfEntity( vView, cpcEntityName, jsState );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetSelectStateOfEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetAllSelectStatesForEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;SLjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetAllSelectStatesForEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jshort   jsState,
              jstring  jstrScopingEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcScopingEntityName = GETSTRING( jstrScopingEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetAllSelectStatesForEntity" ) == 0 )
   {
      nRC = SetAllSelectStatesForEntity( vView, cpcEntityName, jsState, cpcScopingEntityName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetAllSelectStatesForEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntityName, cpcScopingEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetSelectStateOfEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niGetSelectStateOfEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "GetSelectStateOfEntity" ) == 0 )
   {
      nRC = GetSelectStateOfEntity( vView, cpcEntityName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetSelectStateOfEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niCheckExistenceOfEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niCheckExistenceOfEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "CheckExistenceOfEntity" ) == 0 )
   {
      nRC = CheckExistenceOfEntity( vView, cpcEntityName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "CheckExistenceOfEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niDeleteEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niDeleteEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jshort   jsPosition )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "DeleteEntity" ) == 0 )
   {
      if ( CheckExistenceOfEntity( vView, cpcEntityName ) == 0 )
         nRC = DeleteEntity( vView, cpcEntityName, jsPosition );
      else
         nRC = -1;

      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "DeleteEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niIncludeSubobjectFromSubobject
 * Signature: (Ljava/lang/String;Ljava/lang/String;Lzeidon/zView;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niIncludeSubobjectFromSubobject
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrTgtEntityName,
              jobject  jvSrcView,
              jstring  jstrSrcEntityName,
              jshort   jsPosition )
{
   zVIEW      vTgtView;
   zVIEW      vSrcView;
   zCPCHAR    cpcTgtEntityName;
   zCPCHAR    cpcSrcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vTgtView = fnGetView( jEnv, this );
   vSrcView    = fnGetView( jEnv, jvSrcView );

   // Get each of the arguments.
   cpcTgtEntityName = GETSTRING( jstrTgtEntityName );
   cpcSrcEntityName = GETSTRING( jstrSrcEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSrcView, TRUE, "IncludeSubobjectFromSubobject" ) == 0 )
   {
      nRC = IncludeSubobjectFromSubobject( vTgtView, cpcTgtEntityName,
                                           vSrcView, cpcSrcEntityName, jsPosition );
      SetTransientTaskProcess( cpcSessionId, vSrcView, FALSE, "IncludeSubobjectFromSubobject" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrTgtEntityName, cpcTgtEntityName );
   RELEASESTRING( jstrSrcEntityName, cpcSrcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niExcludeEntity
 * Signature: (Ljava/lang/String;Ljava/lang/String;S)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niExcludeEntity
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrTgtEntityName,
              jshort   jsPosition )
{
   zVIEW      vTgtView;
   zCPCHAR    cpcTgtEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vTgtView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcTgtEntityName = GETSTRING( jstrTgtEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vTgtView, TRUE, "ExcludeEntity" ) == 0 )
   {
      nRC = ExcludeEntity( vTgtView, cpcTgtEntityName, jsPosition  );
      SetTransientTaskProcess( cpcSessionId, vTgtView, FALSE, "ExcludeEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrTgtEntityName, cpcTgtEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////==
//
// Cursor methods.
//
/////////////////////////////////////////////////////////////////////////////==

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorEntityKey
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorEntityKey
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrEntityKey )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcEntityKey;
   zCPCHAR    cpcSessionId;
   jint       jiEntityKey;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcEntityKey     = GETSTRING( jstrEntityKey );
   cpcSessionId = GETSTRING( jstrSessionId );

   jiEntityKey = zatol( cpcEntityKey );
   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetEntityCursor" ) == 0 )
   {
      nRC = SetEntityCursor( vView, cpcEntityName, 0,
                             zQUAL_ENTITYKEY | zPOS_FIRST | zQUAL_SCOPE_OI,
                             (zCPVOID) &jiEntityKey,
                             0, 0, 0, 0, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetEntityCursor" );
   }
   else
   {
      nRC = -1;
   }

// TraceLine( "SetCursorEntityKey for Entity: %s   Key: %d   RC: %d",
//            cpcEntityName, jiEntityKey, nRC );
// DisplayEntityInstance( vView, cpcEntityName );

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrEntityKey, cpcEntityKey );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursor
 * Signature: (Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursor
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jint     jiPos,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorRelativeEntity" ) == 0 )
   {
      SetCursorFirstEntity( vView, cpcEntityName, cpcScopingEntity );
      nRC = SetCursorRelativeEntity( vView, cpcEntityName,
                                     (zLONG) jiPos, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorRelativeEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorFirst
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorFirst__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorFirstEntity" ) == 0 )
   {
      nRC = SetCursorFirstEntity( vView, cpcEntityName, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorFirstEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorScopeOI
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL Java_zeidon_zView_niSetCursorScopeOI
            ( JNIEnv  *jEnv,
              jobject this,
              jstring jstrSessionId,
              jstring jstrEntityName,
              jint    jiPos )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorScopeOI" ) == 0 )
   {
      nRC = SetCursorScopeOI( vView, cpcEntityName, (zSHORT) jiPos );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorScopeOI" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorFirst
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorFirst__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorFirstEntity" ) == 0 )
   {
      nRC = SetCursorFirstEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorFirstEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorFirst
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorFirst__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jstring  jstrValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcValue;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcValue         = GETSTRING( jstrValue );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorFirstEntityByString" ) == 0 )
   {
      nRC = SetCursorFirstEntityByString( vView, cpcEntityName, cpcAttribName,
                                          cpcValue, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorFirstEntityByString" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrValue, cpcValue );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorFirst
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DLjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorFirst__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2DLjava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jdouble  jdValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   TraceLineS( "Zeidon Java ",
               "SetCursorFirst by decimal is not implemented yet" );
// SysMessageBox( "Zeidon Java", "We don't handle SetCursorFirst() by decimal yet.", 1 );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorFirstEntity" ) == 0 )
   {
      nRC = SetCursorFirstEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorFirstEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorFirst
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorFirst__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2ILjava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jint     jiValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorFirstEntityByInteger" ) == 0 )
   {
      nRC = SetCursorFirstEntityByInteger( vView, cpcEntityName, cpcAttribName,
                                           (zLONG) jiValue, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorFirstEntityByInteger" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorLast
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorLast__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorLastEntity" ) == 0 )
   {
      nRC = SetCursorLastEntity( vView, cpcEntityName, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorLastEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorLast
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorLast__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorLastEntity" ) == 0 )
   {
      nRC = SetCursorLastEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorLastEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorNext__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorNextEntity" ) == 0 )
   {
      nRC = SetCursorNextEntity( vView, cpcEntityName, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorNextEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorNext__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorNextEntity" ) == 0 )
   {
      nRC = SetCursorNextEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorNextEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorNext__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jstring  jstrValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcValue;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcValue         = GETSTRING( jstrValue );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorNextEntityByString" ) == 0 )
   {
      nRC = SetCursorNextEntityByString( vView, cpcEntityName, cpcAttribName,
                                         cpcValue, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorNextEntityByString" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrValue, cpcValue );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;DLjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorNext__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2DLjava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jdouble  jdValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   TraceLineS( "Zeidon Java ",
               "SetCursorNext by decimal is not implemented yet" );
// SysMessageBox( "Zeidon Java", "We don't handle SetCursorNext() by decimal yet.", 1 );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorNextEntity" ) == 0 )
   {
      nRC = SetCursorNextEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorNextEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorNext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;ILjava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorNext__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2ILjava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jint     jiValue,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttribName    = GETSTRING( jstrAttribName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorNextEntityByInteger" ) == 0 )
   {
      nRC = SetCursorNextEntityByInteger( vView, cpcEntityName, cpcAttribName,
                                          (zLONG) jiValue, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorNextEntityByInteger" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorPrev
 * Signature: (Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorPrev__Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorPrevEntity" ) == 0 )
   {
      nRC = SetCursorPrevEntity( vView, cpcEntityName, 0 );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorPrevEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetCursorPrev
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetCursorPrev__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrScopingEntity )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcScopingEntity;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcScopingEntity = GETSTRING( jstrScopingEntity );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetCursorPrevEntity" ) == 0 )
   {
      nRC = SetCursorPrevEntity( vView, cpcEntityName, cpcScopingEntity );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetCursorPrevEntity" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrScopingEntity, cpcScopingEntity );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//
//  Attribute methods.
//
/////////////////////////////////////////////////////////////////////////////

/*
 * Class:     zeidon_zView
 * Method:    niGetEntityKey
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL
Java_zeidon_zView_niGetEntityKey
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcSessionId;
   jint       lValue;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "GetEntityKey" ) == 0 )
   {
      lValue = GetEntityKey( vView, cpcEntityName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetEntityKey" );
   }
   else
   {
      lValue = 0;
   }

// TraceLine( "GetEntityKey for Entity: %s ===> %d",
//            cpcEntityName, lValue );

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( lValue );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetInteger
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/Integer;
 */
JNIEXPORT jint JNICALL
Java_zeidon_zView_niGetInteger
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttributeName;
   zCPCHAR    cpcSessionId;
   zLONG      lValue;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttributeName = GETSTRING( jstrAttribName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "GetIntegerFromAttribute" ) == 0 )
   {
      nRC = GetIntegerFromAttribute( &lValue, vView,
                                     cpcEntityName, cpcAttributeName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetIntegerFromAttribute" );
   }
   else
   {
      nRC = -1;
   }

   if ( nRC < 0 )
      lValue = 0;

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttributeName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( lValue );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetString
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetString
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttributeName;
   zCPCHAR    cpcSessionId;
   char       szTemp[ 32000 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttributeName = GETSTRING( jstrAttribName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "GetStringFromAttribute" ) == 0 )
   {
      nRC = GetStringFromAttribute( szTemp, zsizeof( szTemp ), vView,
                                    cpcEntityName, cpcAttributeName );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetStringFromAttribute" );
   }
   else
   {
      nRC = -1;
   }

   // If the attribute was not set, set it to null string.
   if ( nRC < 0 )
      szTemp[ 0 ] = 0;

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttributeName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szTemp ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetStringFromAttributeByContext
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetStringFromAttributeByContext
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jstring  jstrContextName,
              jint     jiMaxStringLth )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttributeName;
   zCPCHAR    cpcContextName;
   zCPCHAR    cpcSessionId;
   char       szTemp[ 1024 ];
   zPCHAR     pchTemp = 0;
   jstring    jstrReturn;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttributeName = GETSTRING( jstrAttribName );
   cpcContextName   = GETSTRING( jstrContextName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( jiMaxStringLth >= zsizeof( szTemp ) )
   {
   // strcpy_s( szTemp, zsizeof( szTemp ), "GetStringFromAttributeByContext Entity: " );
   // strcat_s( szTemp, zsizeof( szTemp ), cpcEntityName );
   // strcat_s( szTemp, zsizeof( szTemp ), " Attribute: " );
   // strcat_s( szTemp, zsizeof( szTemp ), cpcAttributeName );
   // strcat_s( szTemp, zsizeof( szTemp ), " Context: " );
   // strcat_s( szTemp, zsizeof( szTemp ), cpcContextName );
   // strcat_s( szTemp, zsizeof( szTemp ), " reallocating ... buffer size (1024) exceeded by maximum size of attribute: " );
   // TraceLineI( szTemp, jiMaxStringLth );
      pchTemp = (zPCHAR) malloc( jiMaxStringLth + 1 );
   }
   else
      pchTemp = szTemp;

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, szTemp ) == 0 )
   {
      nRC = GetStringFromAttributeByContext( pchTemp, vView,
                                             cpcEntityName, cpcAttributeName,
                                             cpcContextName, jiMaxStringLth );
   // TraceLine( "GetStringFromAttributeByContext Entity: %s   Attribute: %s   Context: %s   Lth: %d   Value: %s",
   //            cpcEntityName, cpcAttributeName, cpcContextName, zstrlen( pchTemp ), pchTemp );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "GetStringFromAttributeByContext" );
   }
   else
   {
      nRC = -1;
   }

   // If the attribute was not set, set it to null string.
   if ( nRC < 0 )
      szTemp[ 0 ] = 0;

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttributeName );
   RELEASESTRING( jstrContextName, cpcContextName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   jstrReturn = fnNewJavaString( jEnv, pchTemp );
   if ( pchTemp && pchTemp != szTemp )
      free( pchTemp );

   return( jstrReturn );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetAttribute__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2I
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttributeName,
              jint     jiValue )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttributeName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcAttributeName = GETSTRING( jstrAttributeName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetAttributeFromInteger" ) == 0 )
   {
      nRC = SetAttributeFromInteger( vView, cpcEntityName, cpcAttributeName, jiValue );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetAttributeFromInteger" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttributeName, cpcAttributeName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetAttribute__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttribName,
              jstring  jstrValue )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttribName;
   zCPCHAR    cpcValue;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName = GETSTRING( jstrEntityName );
   cpcAttribName = GETSTRING( jstrAttribName );
   cpcSessionId = GETSTRING( jstrSessionId );
   if ( jstrValue )
      cpcValue = GETSTRING( jstrValue );
   else
      cpcValue = "";

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetAttributeFromString" ) == 0 )
   {
      nRC = SetAttributeFromString( vView, cpcEntityName, cpcAttribName, cpcValue );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetAttributeFromString" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttribName, cpcAttribName );
   if ( jstrValue )
      RELEASESTRING( jstrValue, cpcValue );

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetAttributeFromVariable
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;CILjava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSetAttributeFromVariable
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrEntityName,
              jstring  jstrAttributeName,
              jstring  jstrVariable,
              jchar    jcVariableType,
              jint     jiLth,
              jstring  jstrContextName,
              jint     jiFlag  )
{
   zVIEW      vView;
   zCPCHAR    cpcEntityName;
   zCPCHAR    cpcAttributeName;
   zCPCHAR    cpcVariable;
// zCPCHAR    cpcVariableType;
   zCPCHAR    cpcContextName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcEntityName    = GETSTRING( jstrEntityName );
   cpcAttributeName = GETSTRING( jstrAttributeName );
   cpcVariable      = GETSTRING( jstrVariable );
// cpcVariableType  = GETSTRING( jstrVariableType );
   cpcContextName   = GETSTRING( jstrContextName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vView, TRUE, "SetAttributeFromVariable" ) == 0 )
   {
      nRC = SetAttributeFromVariable( vView,  cpcEntityName, cpcAttributeName,
                                      cpcVariable,
                                      (zCHAR) jcVariableType, jiLth,
                                      cpcContextName, (short) jiFlag );
      SetTransientTaskProcess( cpcSessionId, vView, FALSE, "SetAttributeFromVariable" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrEntityName, cpcEntityName );
   RELEASESTRING( jstrAttributeName, cpcAttributeName );
   RELEASESTRING( jstrVariable, cpcVariable );
// RELEASESTRING( jstrVariableType, cpcVariableType );
   RELEASESTRING( jstrContextName, cpcContextName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
//////////zencas added methods

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
#if 0
/*
 * Class:     zeidon_zView
 * Method:    niCheckReentry
 * Signature: (Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL
Java_zeidon_zView_niCheckReentry
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jint     jiControl )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zLONG      lRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

// SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "CheckReentry" );
   if ( vSubtask )
   {
      if ( jiControl & 0x00000001 )
         lRC = MiGetSetTaskFlag( vSubtask, 1, 7 ); // set TaskInUseByWeb flag ON
      else
      if ( jiControl & 0x00000002 )
         lRC = MiGetSetTaskFlag( vSubtask, 1, 0 ); // get TaskInUseByWeb flag
      else
         lRC = MiGetSetTaskFlag( vSubtask, 1, 5 ); // set TaskInUseByWeb flag OFF

      if ( lRC )
         lRC = 1;
   }
   else
   {
      if ( FindWebTask( cpcSessionId ) == 0 )  // cannot find Zeidon Task!!!
         lRC = -1;
      else
         lRC = 1;  // if the subtask is not set, assume problems so prevent reentry
   }

// SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "CheckReentry" );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   if ( lRC )  // flag set (cannot reenter)
   {
      if ( jiControl & 0x00000001 )
         TraceLineS( "CheckReentry FOUND reentry", "" );

      return( lRC );
   }

// TraceLineS( "CheckReentry NOT reentry", "" );
   return( 0 );
}
#endif

/*
 * Class:     zeidon_zView
 * Method:    niMakeWebFileName
 * Signature: (Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niMakeWebFileName
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jint     jiFlags )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   char       szTemp[ 512 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "SysMakeWebFileName" ) == 0 )
   {
      nRC = SysMakeWebFileName( szTemp, zsizeof( szTemp ), vSubtask, jiFlags );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "SysMakeWebFileName" );
   }
   else
   {
      szTemp[ 0 ] = 0;
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szTemp ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetBannerName
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetBannerName
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrDialog )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zCPCHAR    cpcDialog;
   char       szBannerName[ 512 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );
   cpcDialog = GETSTRING( jstrDialog );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "SysGetWebBannerName" ) == 0 )
   {
      nRC = SysGetWebBannerName( vSubtask, cpcDialog, szBannerName );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "SysGetWebBannerName" );
   }
   else
   {
      szBannerName[ 0 ] = 0;
   }

   RELEASESTRING( jstrDialog, cpcDialog );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szBannerName ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niInitWebSessionObject
 * Signature: (Ljava/lang/String;Lzeidon/zView;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niInitWebSessionObject
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jobject  jvSubtask,
              jstring  jstrUserID )
{
   zVIEW      vSubtask;
   zVIEW      vView;
   zCPCHAR    cpcUserID;
   zCPCHAR    cpcSessionId;
   jclass     jcView = (*jEnv)->GetObjectClass( jEnv, this );
   jclass     jcSubtask = (*jEnv)->GetObjectClass( jEnv, jvSubtask );
   jfieldID   jFieldId;
   zSHORT     nRC;

   jFieldId = (*jEnv)->GetFieldID( jEnv, jcSubtask, "m_lView", "I" );
   vSubtask = (zVIEW) (*jEnv)->GetIntField( jEnv, jvSubtask, jFieldId );

   // Get each of the arguments.
   cpcUserID = GETSTRING( jstrUserID );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "InitWebSessionObject" ) == 0 )
   {
      nRC = InitWebSessionObject( &vView, vSubtask, cpcUserID );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "InitWebSessionObject" );
   }
   else
   {
      vView = 0;
      nRC = -1;
   }

   RELEASESTRING( jstrUserID, cpcUserID );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   // Set the return view.
   jFieldId = (*jEnv)->GetFieldID( jEnv, jcView, "m_lView", "I" );
   (*jEnv)->SetIntField( jEnv, this, jFieldId, (jint) vView );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niInitWebPage
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niInitWebPage
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jstring  jstrPageName )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcPageName;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcViewName = GETSTRING( jstrViewName );
   cpcPageName = GETSTRING( jstrPageName );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "InitWebPage" ) == 0 )
   {
      nRC = InitWebPage( vSubtask, cpcViewName, cpcPageName );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "InitWebPage" );
   }
   else
   {
      nRC = -1;
   }

   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrPageName, cpcPageName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetWebRedirection
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetWebRedirection
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zCHAR      szDlgTag[ 33 ];
   zCHAR      szWndTag[ 33 ];
   zCHAR      szNextJSP[ 80 ];

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   szNextJSP[ 0 ] = 0;
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "GetWebRedirection" ) == 0 )
   {
      GetWebRedirection( vSubtask, 0, szDlgTag, szWndTag );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "GetWebRedirection" );
      if ( szDlgTag[ 0 ] && szWndTag[ 0 ] )
      {
         strcpy_s( szNextJSP, zsizeof( szNextJSP ), szDlgTag );
         strcat_s( szNextJSP, zsizeof( szNextJSP ), szWndTag );
         strcat_s( szNextJSP, zsizeof( szNextJSP ), ".jsp" );
      }
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szNextJSP ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niSetWebRedirection
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niSetWebRedirection
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jint     jiWindowActionBehavior,
              jstring  jstrDlgTag,
              jstring  jstrWndTag )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zCPCHAR    cpcDlgTag;
   zCPCHAR    cpcWndTag;
   zCHAR      szDlgTag[ 33 ];
   zCHAR      szWndTag[ 33 ];
   zCHAR      szNextJSP[ 80 ];
   zVIEW      vKZXMLPGO;
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );
   cpcDlgTag = GETSTRING( jstrDlgTag );
   cpcWndTag = GETSTRING( jstrWndTag );

   szNextJSP[ 0 ] = 0;
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "SetWebRedirection" ) == 0 )
   {
      nRC = SetWebRedirection( vSubtask, jiWindowActionBehavior, cpcDlgTag, cpcWndTag );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "SetWebRedirection" );

      GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
      if ( vKZXMLPGO && CheckExistenceOfEntity( vKZXMLPGO, "NextDialogWindow" ) == 0 )
      {
         GetStringFromAttribute( szDlgTag, zsizeof( szDlgTag ), vKZXMLPGO, "NextDialogWindow", "DialogName" );
         GetStringFromAttribute( szWndTag, zsizeof( szWndTag ), vKZXMLPGO, "NextDialogWindow", "WindowName" );
      }

      if ( szDlgTag[ 0 ] == 0 || szWndTag[ 0 ] == 0 )
      {
         strcpy_s( szDlgTag, zsizeof( szDlgTag ), cpcDlgTag );
         strcpy_s( szWndTag, zsizeof( szWndTag ), cpcWndTag );
      }

      if ( szDlgTag[ 0 ] && szWndTag[ 0 ] )
      {
         strcpy_s( szNextJSP, zsizeof( szNextJSP ), szDlgTag );
         strcat_s( szNextJSP, zsizeof( szNextJSP ), szWndTag );
         strcat_s( szNextJSP, zsizeof( szNextJSP ), ".jsp" );
      }
   }
   else
      nRC = 0;

   RELEASESTRING( jstrDlgTag, cpcDlgTag );
   RELEASESTRING( jstrWndTag, cpcWndTag );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szNextJSP ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niAddWebControlSubobject
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niAddWebControlSubobject
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrControlName,
              jstring  jstrControlType,
              jstring  jstrControlContext,
              jstring  jstrSrcViewName,
              jstring  jstrSrcEntityName,
              jstring  jstrSrcAttributeName,
              jstring  jstrAutoComboBoxValue )
{
   zVIEW      vView;
   zCPCHAR    cpcControlName;
   zCPCHAR    cpcControlType;
   zCPCHAR    cpcControlContext;
   zCPCHAR    cpcSrcViewName;
   zCPCHAR    cpcSrcEntityName;
   zCPCHAR    cpcSrcAttributeName;
   zCPCHAR    cpcAutoComboBoxValue;
   zCPCHAR    cpcSessionId;
   zSHORT     nRC;

   // Get the view for 'this'.
   vView = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcControlName         = GETSTRING( jstrControlName );
   cpcControlType         = GETSTRING( jstrControlType );
   cpcControlContext      = GETSTRING( jstrControlContext );
   cpcSrcViewName         = GETSTRING( jstrSrcViewName );
   cpcSrcEntityName       = GETSTRING( jstrSrcEntityName );
   cpcSrcAttributeName    = GETSTRING( jstrSrcAttributeName );
   cpcAutoComboBoxValue   = GETSTRING( jstrAutoComboBoxValue );
   cpcSessionId = GETSTRING( jstrSessionId );

   nRC = AddWebControlSubobject( vView, cpcControlName, cpcControlType, cpcControlContext, cpcSrcViewName,
                                 cpcSrcEntityName, cpcSrcAttributeName, cpcAutoComboBoxValue );

   RELEASESTRING( jstrControlName, cpcControlName );
   RELEASESTRING( jstrControlType, cpcControlType );
   RELEASESTRING( jstrControlContext, cpcControlContext );
   RELEASESTRING( jstrSrcViewName, cpcSrcViewName );
   RELEASESTRING( jstrSrcEntityName, cpcSrcEntityName );
   RELEASESTRING( jstrSrcAttributeName, cpcSrcAttributeName );
   RELEASESTRING( jstrAutoComboBoxValue, cpcAutoComboBoxValue );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( nRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niPrintReportToHTML
 * Signature: (Ljava/lang/String;Lzeidon/zView;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niPrintReportToHTML
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jobject  jvSourceOI,
              jstring  jstrSubobjectEntityName,
              jstring  jstrReportDefName,
              jstring  jstrReportName,
              jint     jiFlags )
{
   zVIEW      vSubtask;
   zVIEW      vSourceOI;
   zCPCHAR    cpcSubobjectEntityName;
   zCPCHAR    cpcReportDefName;
   zCPCHAR    cpcReportName;
   zCPCHAR    cpcSessionId;
   char       szTemp[ 512 ];
   zLONG      lRC;

   // Get the view for 'this'.
   vSubtask    = fnGetView( jEnv, this );
   vSourceOI   = fnGetView( jEnv, jvSourceOI );

   // Get each of the arguments.
   cpcSubobjectEntityName = GETSTRING( jstrSubobjectEntityName );
   cpcReportDefName = GETSTRING( jstrReportDefName );
   cpcReportName = GETSTRING( jstrReportName );
   cpcSessionId = GETSTRING( jstrSessionId );

   strcpy_s( szTemp, zsizeof( szTemp ), cpcReportName );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "PrintReportToHTML" ) == 0 )
   {
      lRC = PrintReportToHTML( vSubtask, vSourceOI, cpcSubobjectEntityName,
                               cpcReportDefName, szTemp, jiFlags );
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "PrintReportToHTML" );
   }
   else
   {
      szTemp[ 0 ] = 0;
   }

   RELEASESTRING( jstrSubobjectEntityName, cpcSubobjectEntityName );
   RELEASESTRING( jstrReportDefName, cpcReportDefName );
   RELEASESTRING( jstrReportName, cpcReportName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szTemp ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niCallDialogOperation
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niCallDialogOperation
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrDialog,
              jstring  jstrOperation,
              jint     jiControl )
{
   zFARPROC_DYNOBJ lpfnDynRoutine;
   LPLIBRARY  hLibrary;
   zVIEW      vSubtask;
   zCPCHAR    cpcDialogName;
   zCPCHAR    cpcOperationName;
   zCPCHAR    cpcSessionId;
   zCHAR      szMsg[ 512 ];
// zSHORT     k;
   zLONG      lRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcDialogName    = GETSTRING( jstrDialog );
   cpcOperationName = GETSTRING( jstrOperation );
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( g_nTraceAction )
   {
      zsprintf( szMsg, "CallDialogOperation V/Task: 0x%08x / 0x%08x  Dialog.Operation: %s.%s",
                vSubtask, vSubtask->hTask, cpcDialogName, cpcOperationName );
      TraceLineS( szMsg, "" );
   }

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, szMsg ) == 0 )
   {
      if ( jiControl == 1 )  // do not perform operation if there are errors
      {
         zVIEW vMsgQ = fnGetMsgQ( vSubtask, 0, 1 );

         if ( vMsgQ )
         {
            TraceLine( "CallDialogOperation (%s) for Dialog: %s  Error Return",
                       cpcOperationName, cpcDialogName );

            SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "CallDialogOperation" );
            return( 0 );  // errors ==> get out without performing operation
         }
      }

   // k = 0;
   // while ( k < 10000 && (lRC = MiGetSetTaskFlag( vSubtask, 1, 7 )) != 0 ) // set TaskInUseByWeb flag ON
   // {
   //    k++;
   //    SysWait( 10 );
   // }
   //
   // if ( lRC )  // TaskInUseByWeb flag ON
   // {
   //    zsprintf( szMsg, "CallDialogOperation FAILED (reentrancy) V/Task: 0x%08x / 0x%08x  Dialog.Operation: %s.%s",
   //              vSubtask, vSubtask->hTask, cpcDialogName, cpcOperationName );
   //    SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, szMsg );
   //    return( -256 );
   // }

      lRC = -1;
      hLibrary = 0;

      lpfnDynRoutine = (zFARPROC_DYNOBJ)
                GetOperationDynamicCallAddress( vSubtask, &hLibrary,
                                                cpcDialogName, cpcOperationName,
                                                "<java dialog call error>" );
      if ( lpfnDynRoutine )
      {
      // try
      // {
            lRC = (*lpfnDynRoutine)( (zVIEW) vSubtask );
      // }
      // catch( ... )
      // {
      //    TraceLineS( "JS CallDialogOperation Exception", "" );
      // }
      }

   // SetCursorFirstEntity( vSubtask, cpcEntityName, cpcScopingEntity );
   // nRC = SetCursorRelativeEntity( vSubtask, cpcEntityName, (zLONG) jiPos,
   //                                cpcScopingEntity );

      if ( g_nTraceAction )
      {
      // if ( k > 0 )
      // {
      //    zsprintf( szMsg, "CallDialogOperation V/Task: 0x%08x / 0x%08x  WAITED: %d  Dialog.Operation: %s.%s  RC: %d",
      //              vSubtask, vSubtask->hTask, k, cpcDialogName, cpcOperationName, lRC );
      // }
      // else
         {
            zsprintf( szMsg, "CallDialogOperation V/Task: 0x%08x / 0x%08x  Dialog.Operation: %s.%s  RC: %d",
                      vSubtask, vSubtask->hTask, cpcDialogName, cpcOperationName, lRC );
         }

         TraceLineS( szMsg, "" );
      }

   // MiGetSetTaskFlag( vSubtask, 1, 1 ); // set TaskInUseByWeb flag OFF
      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "CallDialogOperation" );
   }
   else
      lRC = -1;

   RELEASESTRING( jstrDialog, cpcDialogName );
   RELEASESTRING( jstrOperation, cpcOperationName );
   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( (zSHORT) lRC );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetFocusCtrl
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetFocusCtrl
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrDialog,
              jstring  jstrWindow )
{
   zVIEW      vSubtask;
   zVIEW      vKZXMLPGO;
   zCPCHAR    cpcSessionId;
   zCPCHAR    cpcDialog;
   zCPCHAR    cpcWindow;
   char       szFocusCtrl[ 256 ];

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );
   cpcDialog = GETSTRING( jstrDialog );
   cpcWindow = GETSTRING( jstrWindow );

   GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
   if ( vKZXMLPGO &&
        SetCursorFirstEntityByString( vKZXMLPGO, "Dialog", "DialogName", cpcDialog, "" ) == 0 &&
        SetCursorFirstEntityByString( vKZXMLPGO, "Window", "WindowName", cpcWindow, "" ) == 0 )
   {
      GetStringFromAttribute( szFocusCtrl, zsizeof( szFocusCtrl ), vKZXMLPGO, "Window", "FocusCtrl" );
   }
   else
   {
      szFocusCtrl[ 0 ] = 0;
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );
   RELEASESTRING( jstrDialog, cpcDialog );
   RELEASESTRING( jstrWindow, cpcWindow );

   return( fnNewJavaString( jEnv, szFocusCtrl ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niFindOpenFile
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niFindOpenFile
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vSubtask;
   zVIEW      vKZXMLPGO;
   zVIEW      vTemp;
   zCPCHAR    cpcSessionId;
   char       szTemp[ 512 ];
   char       szPDFfile[ 512 ];
   char       szFileExt[ 10 ];
   zSHORT     nRC;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   // If the user has run PrintReportToHTML, then the html file name is created using
   // SysMakeWebFileName (uses subtask).  In the jsp code, a view name is created with this file name and
   // this function checks if that view exists.  If it does, then it passes back the file name to open.
   // If the user has run PrintReportToPDF we do something different.  The report name that is passed to
   // PrintReportToPDF is saved to KZXMLPGO ("Session", "PrintFileName").  This is the name of the pdf that
   // gets created.  In this function we check if there is a filename in PrintFileName.  If there is then we
   // pass back this file name (won't be running both an html and pdf together) and reset PrintFileName to null.
   // We assume that if PrintFileName exists, then we won't need to look at SysMakeWebFileName but we still
   // call that code because we want to drop the view for that name (gets created for both html and pdf).
   szPDFfile[ 0 ] = 0;
   GetViewByName( &vKZXMLPGO, "KZXMLPGO", vSubtask, zLEVEL_TASK );
   GetStringFromAttribute( szPDFfile, zsizeof( szPDFfile ), vKZXMLPGO, "Session", "PrintFileName" );
   GetStringFromAttribute( szFileExt, zsizeof( szFileExt ), vKZXMLPGO, "Session", "PrintFileType" );
   if ( szPDFfile[ 0 ] != 0 )
   {
      strcat( szPDFfile, "." );
      strcat( szPDFfile, szFileExt );
   }

   TraceLineS( "Get Open File ===========>> ", szPDFfile );
   SetAttributeFromString( vKZXMLPGO, "Session", "PrintFileName", "" );
   SetAttributeFromString( vKZXMLPGO, "Session", "PrintFileType", "" );

   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "FindOpenFile" ) == 0 )
   {
      nRC = SysMakeWebFileName( szTemp, zsizeof( szTemp ), vSubtask, 0 );
      strcat( szTemp, ".html" );
      nRC = GetViewByName( &vTemp, szTemp, vSubtask, zLEVEL_TASK );
      if ( vTemp )
         nRC = DropNameForView( vTemp, szTemp, vSubtask, zLEVEL_TASK );
      else
         szTemp[ 0 ] = 0;

      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "FindOpenFile" );
   }
   else
   {
      szTemp[ 0 ] = 0;
   }

   // If there had been a filename for a pdf file, then we are going to use
   // that instead of the html name that would be used for generating to html.
   if ( szPDFfile[ 0 ] != 0 )
      strcpy_s( szTemp, zsizeof( szTemp ), szPDFfile );

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szTemp ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niFindErrorFields
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niFindErrorFields
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vSubtask;
   zVIEW      vMsgQ;
   zVIEW      v;
   zCPCHAR    cpcSessionId;
   char       szError[ 8192 ];  // hopefully way big enough
   zSHORT     nLth = 0;
   zSHORT     nRC = 0;
   zSHORT     nRepos = 0; // zREPOS_NONE    0
                          // zREPOS_FIRST   1
                          // zREPOS_LAST    2
                          // zREPOS_NEXT    3
                          // zREPOS_PREV    4

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   szError[ 0 ] = 0;

// TraceLineS( "FindErrorFields for session =======>> ", cpcSessionId );
   if ( SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE, "FindErrorFields" ) == 0 )
   {
      nRC = GetViewByName( &vMsgQ, "__MSGQ", vSubtask, zLEVEL_TASK );
      if ( vMsgQ )
      {
      // TraceLine( "FindErrorFields found __MSGQ", "" );
         CreateViewFromView( &v, vMsgQ );
      // TraceLineI( "FindErrorFields __MSGQ View: ", (zLONG) vSubtask );
         nRC = SetCursorFirstEntityByInteger( v, "Task", "Id", (zLONG) vSubtask, 0 );
      // TraceLineI( "FindErrorFields SetCursorFirst Task RC: ", nRC );
         DisplayObjectInstance( v, 0, 0 );
         if ( nRC == 0 )
         {
            nRC = SetCursorFirstEntity( v, "QMsg", 0 );
         // TraceLineI( "FindErrorFields SetCursorFirst QMsg RC: ", nRC );

            // Get only the first error message for presentation.
            if ( nRC == 0 )
            {
               GetStringFromAttribute( szError + 2, vMsgQ, "QMsg", "Title" );
               if ( szError[ 2 ] == 'D' && szError[ 3 ] == '\t' )
               {
                  strcpy_s( szError, zsizeof( szError ), szError + 2 );
               }
               else
               {
                  szError[ 0 ] = 'Y';
                  szError[ 1 ] = '\t';
               }

               nLth = strlen( szError );
               if ( nLth == 0 || szError[ nLth - 1 ] != '\t' )
                  szError[ nLth++ ] = '\t';

               szError[ nLth++ ] = '\t';  // double tabs to separate error message
               GetStringFromAttribute( szError + nLth, vMsgQ, "QMsg", "Text" );
            }

            // Delete all the error message entities.
            while ( nRC == 0 )
            {
            // DisplayEntityInstance( v, "QMsg" );
               DeleteEntity( v, "QMsg", nRepos );
               nRC = SetCursorFirstEntity( v, "QMsg", 0 );
            // nRC = SetCursorNextEntity( v, "QMsg" );
            // TraceLineI( "FindErrorFields SetCursorNext RC: ", nRC );
            }
         }

         DropView( v );
      // alert( strErrorTitle );
      }

      if ( szError[ 0 ] )
         TraceLineS( "FindErrorFields Error===========>> ", szError );

      SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE, "FindErrorFields" );
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, szError ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niCheckError
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niCheckError
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrWidgetCurr,
              jstring  jstrErrorList )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zCPCHAR    cpcWidgetCurr;
   zCPCHAR    cpcErrorList;
   char       szError[ 256 ];
   zPCHAR     pch;
   zPCHAR     pchTab;
   zSHORT     nLth;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );
   cpcWidgetCurr = GETSTRING( jstrWidgetCurr );
   cpcErrorList = GETSTRING( jstrErrorList );

   szError[ 0 ] = 0;
// TraceLineS( "CheckError looking for control name ====================>> ", cpcWidgetCurr );
// TraceLineS( "                          in ErrorList ===========>> ", cpcErrorList );
   nLth = zstrlen( cpcWidgetCurr );
   pch = strstr( cpcErrorList, cpcWidgetCurr );

   // First see if the name of the control is in the name of the error field.
   while ( pch )
   {
      // If it is then see if there is a tab (value in error is after the tab).
      pchTab = strchr( pch, '\t' );
      if ( pchTab && pchTab - pch == nLth && (pch == cpcErrorList || *(pch - 1) == '\t') )
      {
         pch = pchTab + 1;
         pchTab = strchr( pch, '\t' );
         if ( pchTab )
         {
            strncpy( szError, pch, pchTab - pch );
            szError[ pchTab - pch ] = 0;
         }
         else
            strcpy( szError, pch );

      // TraceLine( "CheckError Found Widget ********>> ", cpcWidgetCurr );
      // TraceLine( "CheckError Found Error Value  ********>> ", szError );
      }

      pch = strstr( pch + nLth, cpcWidgetCurr );
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );
   RELEASESTRING( jstrWidgetCurr, cpcWidgetCurr );
   RELEASESTRING( jstrErrorList, cpcErrorList );

   if ( szError[ 0 ] )
      TraceLineS( "CheckError returning error ====================>> ", szError );

   return( fnNewJavaString( jEnv, szError ) );
}

/*
 * Class:     zeidon_zView
 * Method:    niSaveFeedback
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)S
 */
JNIEXPORT jshort JNICALL
Java_zeidon_zView_niSaveFeedback
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId,
              jstring  jstrViewName,
              jstring  jstrDialogName,
              jstring  jstrWindowName,
              jstring  jstrFeedback )
{
   zVIEW      vSubtask;
   zVIEW      v;
   zCPCHAR    cpcSessionId;
   zCPCHAR    cpcViewName;
   zCPCHAR    cpcDialogName;
   zCPCHAR    cpcWindowName;
   zCPCHAR    cpcFeedback;
   zPCHAR     pchEditString = 0;
   zCHAR      szUserId[ 34 ];

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );
   cpcViewName = GETSTRING( jstrViewName );
   cpcDialogName = GETSTRING( jstrDialogName );
   cpcWindowName = GETSTRING( jstrWindowName );
   cpcFeedback = GETSTRING( jstrFeedback );

   GetViewByName( &v, cpcViewName, vSubtask, zLEVEL_TASK );
   if ( v == 0 )
   {
      // Assume LOD name matches view name.
      ActivateEmptyObjectInstance( &v, cpcViewName, vSubtask, zLEVEL_TASK );
      SetNameForView( v, cpcViewName, vSubtask, zLEVEL_TASK );
   }

   if ( CheckExistenceOfEntity( v, "Feedback" ) == 0 )
   {
      GetStringFromAttribute( szUserId, zsizeof( szUserId ), v, "Feedback", "UserId" );
      if ( szUserId[ 0 ] == 0 )
      {
         SfGetUserIdForTask( vSubtask, szUserId );
         if ( szUserId[ 0 ] )
            SetAttributeFromString( v, "Feedback", "UserId", szUserId );
      }
   }

   if ( cpcFeedback[ 0 ] )
   {
      SfGetUserIdForTask( vSubtask, szUserId );
      CreateEntity( v, "Feedback", zPOS_LAST );
      SetAttributeFromString( v, "Feedback", "Dialog", cpcDialogName );
      SetAttributeFromString( v, "Feedback", "Window", cpcWindowName );
      SetAttributeFromString( v, "Feedback", "Comment", cpcFeedback );
      if ( szUserId[ 0 ] )
      {
         SetAttributeFromString( v, "Feedback", "UserId", szUserId );
         CommitObjectInstance( v );
      }
   }

   RELEASESTRING( jstrSessionId, cpcSessionId );
   RELEASESTRING( jstrViewName, cpcViewName );
   RELEASESTRING( jstrDialogName, cpcDialogName );
   RELEASESTRING( jstrWindowName, cpcWindowName );
   RELEASESTRING( jstrFeedback, cpcFeedback );

   return( 0 );
}

/*
 * Class:     zeidon_zView
 * Method:    niGetDateDefaultContextFormat
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL
Java_zeidon_zView_niGetDateDefaultContextFormat
            ( JNIEnv   *jEnv,
              jobject  this,
              jstring  jstrSessionId )
{
   zVIEW      vSubtask;
   zCPCHAR    cpcSessionId;
   zLPCONTEXT lpContext;
   zPCHAR     pchEditString = 0;

   // Get the view for 'this'.
   vSubtask = fnGetView( jEnv, this );

   // Get each of the arguments.
   cpcSessionId = GETSTRING( jstrSessionId );

   if ( SfGetDefaultContextForDomain( &lpContext, vSubtask, "Date" ) == 1 )
   {
      if ( lpContext->hEditString )
         pchEditString = SysGetPointerFromHandle( lpContext->hEditString );
   }

   if ( pchEditString == 0 || *pchEditString == 0 )
      pchEditString = "MM/DD/YYYY";

   RELEASESTRING( jstrSessionId, cpcSessionId );

   return( fnNewJavaString( jEnv, pchEditString ) );
}

zPVOID OPERATION
GetOperationDynamicCallAddress( zVIEW       vSubtask,
                                LPLPLIBRARY lpLibrary,
                                zCPCHAR     cpcDLL_Name,
                                zCPCHAR     cpcOperationName,
                                zCPCHAR     cpcErrorMessagePrefix )
{
   zPVOID lpfnDynRoutine = 0;

// SetTransientTaskProcess( cpcSessionId, vSubtask, TRUE );   done outside call

// TraceLine( "GetOperationDynamicCallAddress Subtask: 0x%08x   DLL: %s   Operation: %s",
//            vSubtask, cpcDLL_Name, cpcOperationName );
   if ( *lpLibrary == 0 )
      *lpLibrary = SysLoadLibraryWithErrFlag( vSubtask, cpcDLL_Name, 0 );

   if ( *lpLibrary )
   {
      lpfnDynRoutine = SysGetProc( *lpLibrary, cpcOperationName );
      if ( lpfnDynRoutine )
         return( lpfnDynRoutine );
   }

   if ( cpcErrorMessagePrefix && *cpcErrorMessagePrefix )
   {
      zSHORT nLth = zstrlen( cpcErrorMessagePrefix );
      zCHAR  sz[ 256 ];

      strcpy_s( sz, zsizeof( sz ), cpcErrorMessagePrefix );
      if ( *lpLibrary == 0 )
      {
         strcat_s( sz, zsizeof( sz ), " Unable to load\n application DLL - " );
         strcat_s( sz, zsizeof( sz ), cpcDLL_Name );
         strcat_s( sz, zsizeof( sz ), "\n for operation - " );
         strcat_s( sz, zsizeof( sz ), cpcOperationName );
      }
      else
      {
         strcat_s( sz, zsizeof( sz ), " " );
         strcat_s( sz, zsizeof( sz ), cpcOperationName );
         strcat_s( sz, zsizeof( sz ), " Operation Code not found\n in DLL - " );
         strcat_s( sz, zsizeof( sz ), cpcDLL_Name );
      }

      TraceLineS( sz, "" );
      MessageSend( vSubtask, 0, "Zeidon Driver", sz, 0, TRUE );
   }

// SetTransientTaskProcess( cpcSessionId, vSubtask, FALSE );  done outside call
   return( 0 );
}

#if defined( __WIN32__ )

#include <windows.h>

BOOL WINAPI
DllMain( HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved )
{
  switch ( dwReason )
  {
      case DLL_PROCESS_ATTACH:
         g_lInstance = (zLONG) hDLL;
         break;

      case DLL_THREAD_ATTACH:
         break;

      case DLL_THREAD_DETACH:
         break;

      case DLL_PROCESS_DETACH:
         break;
   }

   return( TRUE );

} // DllMain

#endif

#if 0

We all know that it is good practice to "nail down" all user inputs when working
with forms on the Internet. If a user is free to enter "what they like where they
like" then our web site is leaving the doors wide open for attack. It is simple to
limit the length (or number of characters) that can be keyed into a single line
text box. By using the maxlength property we can simply stop the user from
entering more than our predefined limit.

<input name="txtField" type="text" id="txtField" maxlength="20">

But why is this so important? Well, if you are using a web form to insert data
into a database and the user enters 200 characters for a field within your database
that only permits 30 than you are going to get an error when the user submits the form.

Text Area field type does not have a limit on character length, so if you use this
field type in a form you need to limit the text entered using other means. Below
is a JavaScript function that does just that!

<script language="JavaScript" type="text/JavaScript">

// Function to limit text entered into textarea
function limitText(textArea, length)
{
   if (textArea.value.length > length)
   {
      textArea.value = textArea.value.substr(0,length);
   }
}

</script>


This function uses the DOM (Document Object Model) to check the users text input,
when called, to ensure that they don't enter too many characters. All you need
to do now is add the following line of code within the TextArea Tag;


onKeyPress="limitText(this,254);"

The first parameter passed is a reference to the form field, you can use the this
keyword to make life easier (you can also use "document.forms.<formname>.<formitem>"
if you wish). The second parameter is the length you would like to limit to, in
this case 254. A Good tip is to always allow 1 character less than the maximum
limit on your database field as this will always mean that you are within limits.

It is also a good idea to add this function call to the onBlur event. This will
ensure that even if the user tries to paste text into the text Area field
(using right click --> paste) the text pasted will be trimmed as soon as the
users focus moves away from the Text Area Field.

#endif
