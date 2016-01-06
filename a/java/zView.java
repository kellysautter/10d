package zeidon;

import java.io.*;
import java.util.*;
import java.net.*;
import java.rmi.*;

// The valueBound() method when the object is stored in the session; the
// valueUnbound() method when the object is removed from the session or
// when the session times-out or becomes invalid.
import javax.servlet.http.*; // for HttpSessionBindingListener

import javax.servlet.jsp.JspWriter;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.FactoryConfigurationError;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.w3c.dom.Document;
import org.w3c.dom.DOMException;

// For write operation
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerException;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamSource;
import javax.xml.transform.stream.StreamResult;

// For ServletContextListener
//import javax.servlet.*;
//import javax.servlet.http.*;

public class zView implements Serializable, HttpSessionBindingListener //, ServletContextListener, HttpSessionListener
{
   public  int     m_lView;   // View pointer as an int.

   // True if this view returned from SfCreateSubtask
   public  boolean m_bIsSubtaskView;

   // True if this view returned from RegisterZeidonApp
   public  boolean m_bIsMainView;

   // True if user wants views automatically dropped when Java object goes away.
   public  boolean m_bAutoDrop;

   // Session Id
   public  String  m_strSessionId;

   // ====================================================================
   //
   // NATIVE METHODS - These methods duplicate most of the straight methods
   //
   // ====================================================================
   //
   // General Methods.
   //
   private native short niRegisterZeidonApplication( String strSessionId, String strAppName );
   private native short niUnregisterZeidonApplication( String strSessionId );

   private native short niSetDefaultViewForActiveTask( String strSessionId, int iControl );
   private native short niDisplayObjectInstance( String strSessionId );
   private native short niDisplayEntityInstance( String strSessionId, String strEntityName );
   private native short niGarbageCollectViews( String strSessionId );
   private native int   niGetIntegerFromView( String strSessionId );

   private native short niActivateEmpty( String strSessionId, String strLOD_Name,
                                         zView  vSubtask,
                                         int    iControl );
   private native short niSfActivateSysEmptyOI( String strSessionId, String strLOD_Name,
                                                zView  vSubtask,
                                                int    iControl );
   private native short niActivate( String strSessionId, String strLOD_Name,
                                    zView  vSubtask,
                                    int    iControl );
   private native short niActivate( String strSessionId, String strLOD_Name,
                                    zView  vSubtask,
                                    zView  vQualification,
                                    int    iControl );
   private native short niActivateFromFile( String strSessionId, String strLOD_Name,
                                            zView  vSubtask,
                                            String strFileName,
                                            int    iControl );
   public  native short Commit( String strSessionId );
   public  native short CommitOI_ToXML_File( String strSessionId, String strFileName,
                                             int    iFlags );
   public  native short CommitSubobjectToXML_File( String strSessionId, String strFileName,
                                                   String strSubobjectRoot,
                                                   int    iFlags );
   public  native Vector GetTableListForAttribute( String strSessionId, String strEntityName,
                                                   String strAttrName,
                                                   String strContextName );
   private native short niGetView( String strSessionId, String  strName );
   private native short niGetView( String strSessionId, String  strName,
                                   zView   vSubtask );
   private native short niGetView( String strSessionId, String  strName,
                                   zView   vSubtask,
                                   short   nControl );
   private native short niSetName( String strSessionId, String  strName );
   private native short niSetName( String strSessionId, String  strName,
                                   zView   vSubtask );
   private native short niSetName( String strSessionId, String  strName,
                                   zView   vSubtask,
                                   short   nControl );
   private native short niCreateViewFromView( String strSessionId, zView vSrcView );
   private native short niDropName( String strSessionId, String  strName );
   private native short niDropView( String strSessionId );
   private native short niDropObjectInstance( String strSessionId );

   //
   // Static methods.
   //
   private static native void niTraceLine( String str1, String str2 );
   private static native void niTraceLine( String str1, int i );
   private static native void niTraceLine( String str1, zView v );

   //
   // OI methods.
   //
   private native short niSetOIFromBlob( String strSessionId, 
                                          String strTargetLODName,
                                          zView  vSubtask,
                                          zView  vSourceView,
                                          String strSourceEntityName,
                                          String strSourceAttributeName,
                                          int nNbr  );

   private native short niSetBlobFromOI( String strSessionId, String strTargetEntityName,
                                         String strTargetAttributeName,
                                         zView  vSourceView,
                                         int nNbr  );

   private native short niOrderEntityForView( String strSessionId, 
                                              zView  vView,
                                              String strEntityName,
                                              String strOrderKeys );



   //
   // Entity Methods.
   //
   private native short niCreateEntity( String strSessionId, String strEntityName,
                                        short  nPosition );

   private native short niSetSelectStateOfEntity( String strSessionId, String strEntityName,
                                                  short  nState );

   private native short niSetAllSelectStatesForEntity( String strSessionId, String strEntityName,
                                                       short  nState,
                                                       String strScopingEntityName );

   private native short niGetSelectStateOfEntity( String strSessionId, String strEntityName );

   private native short niCheckExistenceOfEntity( String strSessionId, String strEntityName );

   private native short niDeleteEntity( String strSessionId, String strEntityName, short nPos );

   private native short niIncludeSubobjectFromSubobject( String strSessionId, String strTargetEntityName,
                                                         zView  vSourceView,
                                                         String strSourceEntityName,
                                                         short nPos );

   private native short niExcludeEntity( String strSessionId, String strEntityName,
                                         short nPos );

   //
   // Cursor Methods.
   //

   private native short niSetCursorEntityKey( String strSessionId, String strEntityName,
                                              String strEntityKey );
   private native short niSetCursorScopeOI( String strSessionId, String strEntityName,
                                            int iPos );

   // Sets cursor to position by index.
   private native short niSetCursor( String strSessionId, String strEntityName,
                                     int    iPos,
                                     String strScopingEntity );
   private native short niSetCursorFirst( String strSessionId, String strEntityName );
   private native short niSetCursorFirst( String strSessionId, String strEntityName,
                                          String strScopingEntity );
   private native short niSetCursorFirst( String strSessionId, String strEntityName,
                                          String strAttribName,
                                          String strValue,
                                          String strScopingEntity );
   private native short niSetCursorFirst( String strSessionId, String strEntityName,
                                          String strAttribName,
                                          double dValue,
                                          String strScopingEntity );
   private native short niSetCursorFirst( String strSessionId, String strEntityName,
                                          String strAttribName,
                                          int    iValue,
                                          String strScopingEntity );

   private native short niSetCursorLast( String strSessionId, String strEntityName );
   private native short niSetCursorLast( String strSessionId, String strEntityName,
                                         String strScopingEntity );

   private native short niSetCursorNext( String strSessionId, String strEntityName );
   private native short niSetCursorNext( String strSessionId, String strEntityName,
                                         String strScopingEntity );
   private native short niSetCursorNext( String strSessionId, String strEntityName,
                                         String strAttribName,
                                         String strValue,
                                         String strScopingEntity );
   private native short niSetCursorNext( String strSessionId, String strEntityName,
                                         String strAttribName,
                                         double dValue,
                                         String strScopingEntity );
   private native short niSetCursorNext( String strSessionId, String strEntityName,
                                         String strAttribName,
                                         int    iValue,
                                         String strScopingEntity );

   private native short niSetCursorPrev( String strSessionId, String strEntityName );
   private native short niSetCursorPrev( String strSessionId, String strEntityName,
                                         String strScopingEntity );

   //
   // Attribute methods.
   //
   private native int  niGetEntityKey( String strSessionId, String strEntityName );
   private native int  niGetInteger( String strSessionId, String strEntityName,
                                     String strAttributeName );

   private native String niGetString( String strSessionId, String strEntityName,
                                      String strAttributeName );

   private native String niGetStringFromAttributeByContext( String strSessionId, String strEntityName,
                                                            String strAttributeName,
                                                            String strContextName,
                                                            int    iMaxStringLength );

   private native short niSetAttribute( String strSessionId, String strEntityName,
                                        String strAttributeName,
                                        int    iValue );

   private native short niSetAttribute( String strSessionId, String strEntityName,
                                        String strAttributeName,
                                        String strValue );

   //
   // zencas added methods
   //
   private native short niInitWebSessionObject( String strSessionId,
                                                zView zAnyView,
                                                String strUserID );
   private native short niInitWebPage( String strSessionId,
                                       String strViewName,
                                       String strPageName );
   private native String niGetFocusCtrl( String strSessionId,
                                         String strDialog,
                                         String strWindow );
   private native String niGetBannerName( String strSessionId,
                                          String strDialog );

   private native String niFindOpenFile( String strSessionId );
   private native String niFindErrorFields( String strSessionId );
   private native String niCheckError( String strSessionId,
                                       String strWidgetCurr,
                                       String strErrorList );
   private native String niGetDateDefaultContextFormat( String strSessionId );
   private native short niAddWebControlSubobject( String strSessionId,
                                                  String strControlName,
                                                  String strControlType,
                                                  String strControlContext,
                                                  String strSourceViewName,
                                                  String strSourceEntityName,
                                                  String strSourceAttributeName,
                                                  String strAutoComboBoxValue );

   private native short niSetAttributeFromVariable( String strSessionId, String strEntityName,
                                                    String strAttributeName,
                                                    String strVariable,
                                                    char   cVariableType,
                                                    int    iLength,
                                                    String strContextName,
                                                    int    iFlag );

   private native String niMakeWebFileName( String strSessionId, int iFlags );
   private native String niPrintReportToHTML( String strSessionId, zView  vSourceOI,
                                              String strSubobjectEntityName,
                                              String strReportDefName,
                                              String strReportName,
                                              int    iFlags );
   private native short niCallDialogOperation( String strSessionId,
                                               String strDialog,
                                               String strOperation,
                                               int    iControl );

   // ====================================================================
   //
   // JAVA METHODS
   //
   // ====================================================================

   //
   // Java-only methods (have  no Core equivalent).
   //
   public boolean IsNull( )
   {
      if ( m_lView == 0 )
         return( true );
      else
         return( false );
   }

   // This is called to notify the listener that it is being bound into an SSLSession.
   public void valueBound( HttpSessionBindingEvent event )
   {
   // synchronized( m_lView )
      {
      String strSessionId = event.getSession( ).getId( );
      System.out.println( "Zeidon object: " + event.getName( ) + " is in session: " +
                          strSessionId + " '" + m_strSessionId + "'" ); 
      }
   }

   // This is called to notify the listener that it is being unbound from an SSLSession.
   public void valueUnbound( HttpSessionBindingEvent event )
   {
   // zVIEW vWebSubtask;

   // String strSessionId = event.getSession( ).getId( );

      UnregisterZeidonApplication( m_strSessionId );
      System.out.println( "Zeidon object: " + event.getName( ) + " came out of session: " +
                          m_strSessionId + "  UnregisterZeidonApplication" );
   }

   // This is called when the servlet context is initialized (when the Web application is deployed). 
   // You can initialize servlet context related data here.
//   public void contextInitialized( ServletContextEvent event )
//   {
//      System.out.println( "Zeidon object: Context Initialized" );
//   }
    
   // This is invoked when the Servlet Context (the Web application) is undeployed or 
   // WebLogic Server shuts down.
//   public void contextDestroyed( ServletContextEvent event )
//   {
//      System.out.println( "Zeidon object: Context Destroyed" );
//   }

   // This method is called when an HTTP session is activated.
//   public void sessionCreated( HttpSessionEvent se )
//   {
//      System.out.println( "Zeidon object: Session Created" );
//   }

   // This method is called when an HTTP session is about to be passivated.
//   public void sessionDestroyed( HttpSessionEvent se )
//   {
//      System.out.println( "Zeidon object: Session Destroyed" );
//   }

   //
   // General Methods.
   //
   public short RegisterZeidonApplication( String strSessionId, String strAppName )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      m_bIsMainView = true;

      // We always make the main view an autodrop view.
      m_bAutoDrop = true;

      return( niRegisterZeidonApplication( strSessionId, strAppName ) );
   }

   public short UnregisterZeidonApplication( String strSessionId )
   {
      return( DropView( strSessionId ) );
   }

   public short SetDefaultViewForActiveTask( String strSessionId, int iControl )
   {
      return( niSetDefaultViewForActiveTask( strSessionId, iControl ) );
   }

   public short DisplayObjectInstance( String strSessionId )
   {
      return( niDisplayObjectInstance( strSessionId ) );
   }

   public short DisplayEntityInstance( String strSessionId, String strEntityName )
   {
      return( niDisplayEntityInstance( strSessionId, strEntityName ) );
   }

   public short GarbageCollectViews( String strSessionId )
   {
      return( niGarbageCollectViews( strSessionId ) );
   }

   public int  GetIntegerFromView( String strSessionId )
   {
      return( niGetIntegerFromView( strSessionId ) );
   }

   public short ActivateEmpty( String strSessionId, String strLOD_Name,
                               zView  vSubtask,
                               int    iControl )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niActivateEmpty( strSessionId, strLOD_Name, vSubtask, iControl ) );
   }

   public short SfActivateSysEmptyOI( String strSessionId, String strLOD_Name,
                                      zView  vSubtask,
                                      int    iControl )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niSfActivateSysEmptyOI( strSessionId, strLOD_Name, vSubtask, iControl ) );
   }

   public short Activate( String strSessionId, String strLOD_Name,
                          zView  vSubtask,
                          int    iControl )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niActivate( strSessionId, strLOD_Name, vSubtask, iControl ) );
   }

   public short Activate( String strSessionId, String strLOD_Name,
                          zView  vSubtask,
                          zView  vQualification,
                          int    iControl )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niActivate( strSessionId, strLOD_Name, vSubtask, vQualification, iControl ) );
   }

   public short ActivateFromFile( String strSessionId, String strLOD_Name,
                                  zView  vSubtask,
                                  String strFileName,
                                  int    iControl )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niActivateFromFile( strSessionId, strLOD_Name, vSubtask, strFileName, iControl ) );
   }

   public short GetView( String strSessionId, String strName )
   {
      return( niGetView( strSessionId, strName ) );
   }

   public short GetView( String strSessionId, String  strName,
                         zView   vSubtask )
   {
      return( niGetView( strSessionId, strName, vSubtask ) );
   }

   public short GetView( String strSessionId, String  strName,
                         zView   vSubtask,
                         short   nControl )
   {
      return( niGetView( strSessionId, strName, vSubtask, nControl ) );
   }

   public short SetName( String strSessionId, String  strName )
   {
      return( niSetName( strSessionId, strName ) );
   }

   public short SetName( String strSessionId, String  strName,
                         zView   vSubtask )
   {
      return( niSetName( strSessionId, strName, vSubtask ) );
   }

   public short SetName( String strSessionId, String  strName,
                         zView   vSubtask,
                         short   nControl )
   {
      return( niSetName( strSessionId, strName, vSubtask, nControl ) );
   }

   public short CreateViewFromView( String strSessionId, zView vSrcView )
   {
      if ( m_lView != 0 && m_bAutoDrop )
         DropView( strSessionId );

      return( niCreateViewFromView( strSessionId, vSrcView ) );
   }

   public short DropName( String strSessionId, String  strName )
   {
      return( niDropName( strSessionId, strName ) );
   }

   public short DropView( String strSessionId )
   {
      short nRC = 0;

      if ( m_lView == 0 )
         return( 0 );

      if ( m_bIsMainView )
         nRC = niUnregisterZeidonApplication( strSessionId );
      else
         nRC = niDropView( strSessionId );

      m_lView          = 0;
      m_bIsMainView    = false;
      m_bIsSubtaskView = false;

      return( nRC );
   }

   public short DropObjectInstance( String strSessionId )
   {
      short nRC = 0;

      if ( m_lView == 0 )
         return( 0 );

      if ( m_bIsMainView )
         nRC = niUnregisterZeidonApplication( strSessionId );
      else
         nRC = niDropObjectInstance( strSessionId );

      m_lView          = 0;
      m_bIsMainView    = false;
      m_bIsSubtaskView = false;

      return( nRC );
   }

   //
   // Static methods.
   //
   public static void TraceLine( String str1, String str2 )
   {
      niTraceLine( str1, str2 );
   }

   public static void TraceLine( String str1, int i )
   {
      niTraceLine( str1, i );
   }

   public static void TraceLine( String str1, zView v )
   {
      niTraceLine( str1, v );
   }

   //
   // OI Methods.
   //
   public short SetOIFromBlob( String strSessionId, 
                                String strTargetLODName,
                                zView  vSubtask,
                                zView  vSourceView,
                                String strSourceEntityName,
                                String strSourceAttributeName,
                                int  nNbr  )
   {
      return( niSetOIFromBlob( strSessionId,  strTargetLODName, vSubtask, vSourceView, 
                                strSourceEntityName, strSourceAttributeName, nNbr ) );
   }

   public short SetBlobFromOI( String strSessionId, String strTargetEntityName,
                               String strTargetAttributeName,
                               zView  vSourceView,
                               int nNbr  )
   {
      return( niSetBlobFromOI( strSessionId, strTargetEntityName, 
                               strTargetAttributeName, vSourceView, nNbr ) );
   }

   public short OrderEntityForView( String strSessionId, 
                                    zView vView,
                                    String strEntityName,
                                    String strOrderKeys )
   {
      return( niOrderEntityForView( strSessionId, vView, 
                                    strEntityName, strOrderKeys ) );
   }

   //
   // Entity Methods.
   //
   public short CreateEntity( String strSessionId, String strEntityName,
                              short  nPosition )
   {
      return( niCreateEntity( strSessionId, strEntityName, nPosition ) );
   }

   public short SetSelectStateOfEntity( String strSessionId, String strEntityName,
                                        short  nState )
   {
      return( niSetSelectStateOfEntity( strSessionId, strEntityName, nState ) );
   }

   public short GetSelectStateOfEntity( String strSessionId, String strEntityName )
   {
      return( niGetSelectStateOfEntity( strSessionId, strEntityName ) );
   }

   public short SetAllSelectStatesForEntity( String strSessionId, String strEntityName,
                                             short  nState,
                                             String strScopingEntityName )
   {
      return( niSetAllSelectStatesForEntity( strSessionId, strEntityName, nState,
                                             strScopingEntityName ) );
   }

   public short CheckExistenceOfEntity( String strSessionId, String strEntityName )
   {
      return( niCheckExistenceOfEntity( strSessionId, strEntityName ) );
   }

   public short DeleteEntity( String strSessionId, String strEntityName, short nPos )
   {
      return( niDeleteEntity( strSessionId, strEntityName, nPos ) );
   }

   public int   GetEntityKey( String strSessionId, String strEntityName )
   {
      return( niGetEntityKey( strSessionId, strEntityName ) );
   }

   public short SetCursorEntityKey( String strSessionId, String strEntityName, String strEntityKey )
   {
      return( niSetCursorEntityKey( strSessionId, strEntityName, strEntityKey ) );
   }

   //
   // Cursor Methods.
   //
   public short SetCursorFirst( String strSessionId, String strEntityName )
   {
      return( niSetCursorFirst( strSessionId, strEntityName ) );
   }

   public short SetCursorScopeOI( String strSessionId, String strEntityName,
                                  int iPos )
   {
      return( niSetCursorScopeOI( strSessionId, strEntityName, iPos ) );
   }

   public short SetCursorFirst( String strSessionId, String strEntityName,
                                String strScopingEntity )
   {
      return( niSetCursorFirst( strSessionId, strEntityName, strScopingEntity ) );
   }

   public short SetCursorFirst( String strSessionId, String strEntityName,
                                String strAttribName,
                                String strValue,
                                String strScopingEntity )
   {
      return( niSetCursorFirst( strSessionId, strEntityName, strAttribName, strValue,
                                strScopingEntity ) );
   }

   public short SetCursorFirst( String strSessionId, String strEntityName,
                                String strAttribName,
                                double dValue,
                                String strScopingEntity )
   {
      return( niSetCursorFirst( strSessionId, strEntityName, strAttribName, dValue,
                                strScopingEntity ) );
   }

   public short SetCursorFirst( String strSessionId, String strEntityName,
                                String strAttribName,
                                int    iValue,
                                String strScopingEntity )
   {
      return( niSetCursorFirst( strSessionId, strEntityName, strAttribName, iValue,
                                strScopingEntity ) );
   }

   public short SetCursorLast( String strSessionId, String strEntityName )
   {
      return( niSetCursorLast( strSessionId, strEntityName ) );
   }

   public short SetCursorLast( String strSessionId, String strEntityName,
                               String strScopingEntity )
   {
      return( niSetCursorLast( strSessionId, strEntityName, strScopingEntity ) );
   }

   public short SetCursorNext( String strSessionId, String strEntityName )
   {
      return( niSetCursorNext( strSessionId, strEntityName ) );
   }

   public short SetCursorNext( String strSessionId, String strEntityName,
                               String strScopingEntity )
   {
      return( niSetCursorNext( strSessionId, strEntityName, strScopingEntity ) );
   }

   public short SetCursorNext( String strSessionId, String strEntityName,
                               String strAttribName,
                               String strValue,
                               String strScopingEntity )
   {
      return( niSetCursorNext( strSessionId, strEntityName, strAttribName,
                               strValue, strScopingEntity ) );
   }

   public short SetCursorNext( String strSessionId, String strEntityName,
                               String strAttribName,
                               double dValue,
                               String strScopingEntity )
   {
      return( niSetCursorNext( strSessionId, strEntityName, strAttribName,
                               dValue, strScopingEntity ) );
   }

   public short SetCursorNext( String strSessionId, String strEntityName,
                               String strAttribName,
                               int    iValue,
                               String strScopingEntity )
   {
      return( niSetCursorNext( strSessionId, strEntityName, strAttribName, iValue,
                               strScopingEntity ) );
   }

   public short SetCursorPrev( String strSessionId, String strEntityName )
   {
      return( niSetCursorPrev( strSessionId, strEntityName ) );
   }

   public short SetCursorPrev( String strSessionId, String strEntityName,
                               String strScopingEntity )
   {
      return( niSetCursorPrev( strSessionId, strEntityName, strScopingEntity ) );
   }

   // Following sets cursor to position by index.
   public short SetCursor( String strSessionId, String strEntityName,
                           int    iPos,
                           String strScopingEntity )
   {
      return( niSetCursor( strSessionId, strEntityName, iPos, strScopingEntity ) );
   }

   public short IncludeSubobjectFromSubobject( String strSessionId, String strTargetEntityName,
                                               zView  vSourceView,
                                               String strSourceEntityName,
                                               short nPos )
   {
      return( niIncludeSubobjectFromSubobject( strSessionId, strTargetEntityName, vSourceView,
                                               strSourceEntityName, nPos ) );
   }

   public short ExcludeEntity( String strSessionId, String strEntityName, short nPos )
   {
      return( niExcludeEntity( strSessionId, strEntityName, nPos ) );
   }

   //
   // Attribute methods.
   //
   public int GetInteger( String strSessionId, String strEntityName,
                          String strAttributeName )
   {
      return( niGetInteger( strSessionId, strEntityName, strAttributeName ) );
   }


   public String GetString( String strSessionId, String strEntityName,
                            String strAttribName )
   {
      return( niGetString( strSessionId, strEntityName, strAttribName ) );
   }

   public String GetStringFromAttributeByContext( String strSessionId, String strEntityName,
                                                  String strAttributeName,
                                                  String strContextName,
                                                  int    iMaxStringLength )
   {
      return( niGetStringFromAttributeByContext( strSessionId, strEntityName,
                                                 strAttributeName,
                                                 strContextName,
                                                 iMaxStringLength ) );
   }


   public short SetAttribute( String strSessionId, String strEntityName,
                              String strAttributeName,
                              int    iValue )
   {
      return( niSetAttribute( strSessionId, strEntityName, strAttributeName, iValue ) );
   }

   public short SetAttribute( String strSessionId, String strEntityName,
                              String strAttributeName,
                              String strValue )
   {
      return( niSetAttribute( strSessionId, strEntityName, strAttributeName, strValue ) );
   }

   public short SetAttributeFromVariable( String strSessionId, String strEntityName,
                                          String strAttributeName,
                                          String strVariable,
                                          char   cVariableType,
                                          int    iLength,
                                          String strContextName,
                                          int    iFlag )
   {
      return( niSetAttributeFromVariable( strSessionId, strEntityName, strAttributeName,
                                          strVariable, cVariableType, iLength,
                                          strContextName, iFlag ) );
   }

   //
   // Utility methods.
   //
   public short CallDialogOperation( String strSessionId, String strDialog,
                                     String strOperation,
                                     int    iControl )
   {
      return( niCallDialogOperation( strSessionId, strDialog, strOperation, iControl ) );
   }

   public String PrintReportToHTML( String strSessionId, zView  vSourceOI,
                                    String strSubobjectEntityName,
                                    String strReportDefName,
                                    String strReportName,
                                    int    iFlags )
   {
      return( niPrintReportToHTML( strSessionId, vSourceOI, strSubobjectEntityName,
                                   strReportDefName, strReportName, iFlags ) );
   }

   public String MakeWebFileName( String strSessionId, int iFlags )
   {
      return( niMakeWebFileName( strSessionId, iFlags ) );
   }

   public short AddWebControlSubobject( String strSessionId, String strControlName,
                                        String strControlType,
                                        String strControlContext,
                                        String strSourceViewName,
                                        String strSourceEntityName,
                                        String strSourceAttributeName,
                                        String strAutoComboBoxValue )
   {
      return( niAddWebControlSubobject( strSessionId, strControlName, strControlType,
                                        strControlContext, strSourceViewName,
                                        strSourceEntityName,
                                        strSourceAttributeName,
                                        strAutoComboBoxValue ) );
   }

   public short InitWebSessionObject( String strSessionId, zView zAnyView, String strUserID )
   {
      return( niInitWebSessionObject( strSessionId, zAnyView, strUserID ) );
   }

   public short InitWebPage( String strSessionId, String strViewName, String strPageName )
   {
      return( niInitWebPage( strSessionId, strViewName, strPageName ) );
   }

   public String GetFocusCtrl( String strSessionId, String strDialog, String strWindow )
   {
      return( niGetFocusCtrl( strSessionId, strDialog, strWindow ) );
   }

   public String GetBannerName( String strSessionId, String strDialog )
   {
      return( niGetBannerName( strSessionId, strDialog ) );
   }

   public String FindOpenFile( String strSessionId )
   {
      return( niFindOpenFile( strSessionId ) );
   }

   public String FindErrorFields( String strSessionId )
   {
      return( niFindErrorFields( strSessionId ) );
   }

   public String CheckError( String strSessionId, String strWidgetCurr, String strErrorList )
   {
      return( niCheckError( strSessionId, strWidgetCurr, strErrorList ) );
   }

   public String GetDateDefaultContextFormat( String strSessionId )
   {
      return( niGetDateDefaultContextFormat( strSessionId ) );
   }

   // ====================================================================
   //
   // Zeidon constants
   //
   // ====================================================================
   public static final short zPOS_FIRST          = 1;
   public static final short zPOS_LAST           = 2;
   public static final short zPOS_NEXT           = 3;
   public static final short zPOS_AFTER          = 3;
   public static final short zPOS_PREV           = 4;
   public static final short zPOS_BEFORE         = 4;

   public static final int   zSINGLE                 = 0x00000000;
   public static final int   zMULTIPLE               = 0x00000100;
   public static final int   zNOI_OKAY               = 0x00000200;
   public static final int   zNOENQUEUE              = 0x00000400;
   public static final int   zREADONLY               = 0x00000800;
   public static final int   zSINGLE_FOR_UPDATE      = 0x00400000;
   public static final int   zIGNORE_ENTITY_ERRORS   = 0x00001000;
   public static final int   zIGNORE_ATTRIB_ERRORS   = 0x00002000;
   public static final int   zACTIVATE_CONTINUE      = 0x00800000;
   public static final int   zACTIVATE_LOCAL         = 0x40000000;
   public static final int   zACTIVATE_SYSTEM        = 0x20000000;
   public static final int   zACTIVATE_ROOTONLY      = 0x10000000;
   public static final int   zACTIVATE_NOCONSTRAINTS = 0x01000000;

   public static final int   zIGNORE_ERRORS              = (zIGNORE_ENTITY_ERRORS | zIGNORE_ATTRIB_ERRORS);
   public static final int   zACTIVATE_ROOTONLY_MULTIPLE = (zACTIVATE_ROOTONLY | zMULTIPLE);
   public static final int   zACTIVATE_ROOTONLY_CONTINUE = (zACTIVATE_ROOTONLY | zACTIVATE_CONTINUE | zMULTIPLE);
   public static final int   zACTIVATE_MULTIPLE_CONTINUE = (zACTIVATE_CONTINUE | zMULTIPLE);

   public static final short zCALL_ERROR                = -16;
   public static final short zCURSOR_NULL               = -3;
   public static final short zCURSOR_UNDEFINED          = -2;
   public static final short zCURSOR_UNCHANGED          = -1;
   public static final short zCURSOR_SET                =  0;
   public static final short zCURSOR_SET_NEWPARENT      =  1;
   public static final short zCURSOR_SET_RECURSIVECHILD =  2;

   public static final short zREPOS_NONE                =  0;
   public static final short zREPOS_FIRST               =  1;
   public static final short zREPOS_LAST                =  2;
   public static final short zREPOS_NEXT                =  3;
   public static final short zREPOS_AFTER               =  3;
   public static final short zREPOS_PREV                =  4;
   public static final short zREPOS_BEFORE              =  4;

   public static final int   zXML_ZEIDONINFO            = 0x00000001;
   public static final int   zXML_INCREFLAGS            = 0x00000002;
   public static final int   zXML_UPDATEDONLY           = 0x00000004;
   public static final int   zXML_ENTITYKEYS            = 0x00000008;
   public static final int   zXML_KEYATTRIBSASIDENT     = 0x00000010;
   public static final int   zXML_ROOTONLY              = 0x00000020;

   public static final short zENTITY_SELECTED           = 1;
   public static final short zENTITY_UNSELECTED         = 0;

   // ====================================================================
   //
   //
   //
   // ====================================================================

   public zView( String strSessionId )
   {
      m_lView          = 0;
      m_bIsSubtaskView = false;
      m_strSessionId   = strSessionId;
   }

   // Creates a view and tries to set it by name.
   public zView( String strSessionId, String strViewName )
   {
      this( strSessionId );  // Calls the base constructor

      // If the view name starts with "//" then the user wants to create a
      // subtask view.
      if ( strViewName.startsWith( "//" ) )
      {
      // RegisterZeidonApplication( strSessionId, strViewName );
         m_bIsSubtaskView = true;
      }
      else
         this.GetView( strSessionId, strViewName );
   }

   public zView( String strSessionId, zView vSrcView )
   {
      this( strSessionId );          // Calls the base constructor
      this.CreateViewFromView( strSessionId, vSrcView );
   }

   public void SetAutoDrop()
   {
      m_bAutoDrop = true;
   }

   public void transform( JspWriter out,
                          String    xsl,
                          String    strSubobjectRoot )
               throws java.io.IOException
   {
      String xmlFile = "c:\\temp\\temp.xml";

      CommitSubobjectToXML_File( m_strSessionId, xmlFile, strSubobjectRoot, 256 + 64 );

      DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();

      try
      {
         File stylesheet = new File( xsl );
         File datafile   = new File( xmlFile );

         DocumentBuilder builder  = factory.newDocumentBuilder();
         Document        document = builder.parse( datafile );

         // Use a Transformer for output
         TransformerFactory tFactory    = TransformerFactory.newInstance();
         StreamSource       stylesource = new StreamSource( stylesheet );
         Transformer        transformer = tFactory.newTransformer( stylesource );

         DOMSource    source = new DOMSource( document );
         StreamResult result = new StreamResult( out );
         transformer.transform( source, result );
      }
      catch ( TransformerConfigurationException tce )
      {
         // Error generated by the parser
         System.out.println( "\n** Transformer Factory error" );
         System.out.println( "   " + tce.getMessage() );

         // Use the contained exception, if any
         Throwable x = tce;
         if ( tce.getException() != null )
             x = tce.getException();

         x.printStackTrace();
      }
      catch ( TransformerException te )
      {
         // Error generated by the parser
         System.out.println( "\n** Transformation error" );
         System.out.println( "   " + te.getMessage() );

         // Use the contained exception, if any
         Throwable x = te;
         if ( te.getException() != null )
             x = te.getException();

         x.printStackTrace();
       }
       catch ( SAXException sxe )
       {
         // Error generated by this application
         // (or a parser-initialization error)
         Exception  x = sxe;
         if ( sxe.getException() != null )
             x = sxe.getException();

         x.printStackTrace();
      }
      catch ( ParserConfigurationException pce )
      {
          // Parser with specified options can't be built
          pce.printStackTrace();
      }
      catch ( IOException ioe )
      {
         // I/O error
         ioe.printStackTrace();
      }

      return;
   }

   public void transform( JspWriter out,
                          String    xsl )
               throws java.io.IOException
   {
      transform( out, xsl, null );
   }

   public String toString( )
   {
      if ( m_lView == 0 )
         return( "<null>" );

      Integer i = new Integer( m_lView );
      return( i.toString() );
   }

   public void finalize( )
   {
      if ( m_bAutoDrop )
         DropView( m_strSessionId );
   }

   static
   {
      // Try loading core locally.  If it fails because of security or because
      // it can't find the library then we'll run Zeidon remotely.
      try
      {
         System.loadLibrary( "kzoejava" );
         System.out.println( "Using local OE" );
      }
      catch ( SecurityException e )
      {
         System.out.println( "Security exception" );
      }
      catch ( UnsatisfiedLinkError e )
      {
         System.out.println( "Link Error" );
      }
   }
}
