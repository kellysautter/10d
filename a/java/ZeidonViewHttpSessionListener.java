package zeidon;

import java.io.*;
import java.util.*;

// The valueBound() method when the object is stored in the session; the
// valueUnbound() method when the object is removed from the session or
// when the session times-out or becomes invalid.
import javax.servlet.http.*; // for HttpSessionBindingListener

public class ZeidonViewHttpSessionListener implements HttpSessionBindingListener //, ServletContextListener, HttpSessionListener
{
   private zView vZeidonApp = null;

   public ZeidonViewHttpSessionListener()
   {
   }

   public ZeidonViewHttpSessionListener( zView vZeidonApp )
   {
      setZeidonApp( vZeidonApp );
   }

   public void setZeidonApp( zView vZeidonApp )
   {
      this.vZeidonApp = vZeidonApp;
   }

   // This is called to notify the listener that it is being bound into an SSLSession.
   public void valueBound( HttpSessionBindingEvent event )
   {
   }

   // This is called to notify the listener that it is being unbound from an SSLSession.
   public void valueUnbound( HttpSessionBindingEvent event )
   {
      if ( vZeidonApp != null )
      {
         synchronized( vZeidonApp )
         {
            if ( vZeidonApp.IsActiveApplication() )
            {
               vZeidonApp.UnregisterZeidonApplication( null );
            }
         }
      }
   }
}