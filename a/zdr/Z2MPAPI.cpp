#include "zstdafx.h"
#include "mpna81.h"
#include "Z2MP.h"

#pragma warning(disable: 4996) // This function or variable may be unsafe

zOPER_EXPORT zSHORT OPERATION
MapPointIsActive( )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   if ( pApp )
   {
      TRY
      {
         nRC = pApp->GetMapPointState( );
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   // Return 0 for Inactive and 1 for Active.
   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointStart( )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );

   SHORT nRC = 0;
   zCPCHAR cpcMapName = 0;
   ZMapPointApp *pApp = 0;
   _Application *pMP = 0;

   TRY
   {
      pApp = (ZMapPointApp *) ::AfxGetApp( );

      if ( pApp )
      {
         zLONG lState = pApp->GetMapPointState( );
         pMP = pApp->GetMapPointApplication( );

         if ( pMP == 0 )
         {
            return( zCALL_ERROR );
         }

         // to cause an exception if MapPoint is not really alive
         BOOL bVisible = pMP->GetVisible( );
         if ( lState )  // already active
            return( 0 );

         if ( pMP == 0 )
            nRC = zCALL_ERROR;

         CString csMapName;
         HKEY hKey;
         zCHAR szMapPointPath[ 256 ];
         DWORD dwBufLen = zsizeof( szMapPointPath );

         if ( RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\"
                               "App Paths\\MapPoint.exe",
                            0, KEY_QUERY_VALUE, &hKey ) == ERROR_SUCCESS )
         {
            if ( RegQueryValueEx( hKey, "Path", NULL, NULL,
                                  (LPBYTE) szMapPointPath,
                                  &dwBufLen ) == ERROR_SUCCESS )
            {
               csMapName = szMapPointPath;
            }

            RegCloseKey( hKey );
         }

         if ( cpcMapName && *cpcMapName )
            csMapName += cpcMapName;
         else
            csMapName += "Templates\\New North American Map.ptt";

      // pMP->NewMap( "E:\\Program Files\\Microsoft MapPoint\\Templates\\"
      //                 "New North American Map.ptt" );
         pMP->NewMap( csMapName );

#if 0
         // This is for a MapControl.
         VARIANT a;
         a.vt = VT_I4;
         a.lVal = 1; // geoMapNorthAmerica!
      // a.lVal = 2; // geoMapEurope!

         pMP->NewMap( a );
#endif
      }
   }
   CATCH_ALL( e )
   {
      pApp->m_pMapPointApp = 0;
   // ::MessageBox( 0, "MapPoint was inadvertantly closed\n"
   //                    "Please try again", "Zeidon MapPoint", MB_OK );
      nRC = zCALL_ERROR;
   }
   END_CATCH_ALL

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointStartFromHold( )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         pApp->SetHoldState( 0 );
         _Application *pMP = pApp->GetMapPointApplication( );
         if ( pMP == 0 )
            nRC = zCALL_ERROR;
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointStop( )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         pApp->QuitMapPointApplication( 0 );
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointStopAndHold( )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         pApp->QuitMapPointApplication( 1 );
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointShowApplication( BOOL bShow )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         _Application *pMP = pApp->GetMapPointApplication( );
         if ( pMP == 0 )
            nRC = zCALL_ERROR;
         else
            pMP->SetVisible( bShow );
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

// geoAllResultsValid 0 - Used only for results obtained from the FindNearby
// and ObjectsFromPoint methods, where all returned results match the method
// criteria.
// geoAmbiguousResults 2 - At least the first two results are good matches,
// but it is not clear which one was intended.
// geoFirstResultGood 1 - The first dResult is a good match to the method
// criteria.
// geoNoGoodResult 3 - None of the results is a good match to the method
// criteria, but the results are the best available.
// geoNoResults 4 - No results.

// geoCountryCanada          39   Canada
// geoCountryUnitedStates   244   United States
zOPER_EXPORT zSHORT OPERATION
MapPointValidateAddress( zPCHAR pchStreet, zPCHAR pchCity, zPCHAR pchOtherCity,
                         zPCHAR pchRegion, zPCHAR pchPostalCode,
                         zCPCHAR cpcStreetAddress, zLONG lFlag )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );

   *pchStreet = 0;
   *pchCity = 0;
   *pchOtherCity = 0;
   *pchRegion = 0;
   *pchPostalCode = 0;
   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         _Application *pMP = pApp->GetMapPointApplication( );
         if ( pMP == 0 )
         {
            nRC = zCALL_ERROR;
         }
         else
         {
            _Map Map = pMP->GetActiveMap( );
         // if ( pMap == 0 )
         //    nRC = zCALL_ERROR;
         // else
            {
            // Item    item;
               zLONG   lCnt;
               VARIANT a;
               a.vt = VT_I4;
               a.lVal = 244; // geoMapNorthAmerica!
            // a.lVal = 1; // geoMapNorthAmerica!
            // a.lVal = 2; // geoMapEurope!

               StreetAddress SA = Map.ParseStreetAddress( cpcStreetAddress );
               strcpy_s( pchStreet, 256, SA.GetStreet( ) );
               strcpy_s( pchCity, 256, SA.GetCity( ) );
               strcpy_s( pchOtherCity, 256, SA.GetOtherCity( ) );
               strcpy_s( pchRegion, 256, SA.GetRegion( ) );
               strcpy_s( pchPostalCode, 256, SA.GetPostalCode( ) );
               FindResults FR = Map.FindAddressResults( pchStreet, pchCity,
                                                        pchOtherCity,
                                                        pchRegion,
                                                        pchPostalCode, a );
               TraceLineI( "FindAddressResults Quality: ", FR.GetResultsQuality( ) );
               lCnt = FR.GetCount( );
               TraceLineI( "FindAddressResults Count: ", lCnt );
#if 0
               for ( int k = 1; k <= lCnt; k++ )
               {
                  a.lVal = k;
                  Location L = FR.GetItem( &a );
                  long lType = L.GetType( );
                  long lX = Map.LocationToX( L );
                  long lY = Map.LocationToY( L );
                  TraceLine( "Location Name: %s  Type: %d  X: %d  Y: %d",
                             L.GetName( ), lType, lX, lY );

                  Map.AddPushpin( L, L.GetName( ) );
                  PlaceCategory PC = L.GetPlaceCategory( );
                  TraceLineS( "The Place Category is: ", PC.GetName( ) );
               }
#endif
            }
         }
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

zOPER_EXPORT zSHORT OPERATION
MapPointSetPushPin( zCPCHAR cpcStreet, zCPCHAR cpcCity, zCPCHAR cpcOtherCity,
                    zCPCHAR cpcRegion, zCPCHAR cpcPostalCode,
                    zCPCHAR cpcMsg, zSHORT nPushPin, zLONG lFlag )
{
   AFX_MANAGE_STATE( AfxGetStaticModuleState( ) );
   SHORT nRC = 0;
   ZMapPointApp *pApp = (ZMapPointApp *) ::AfxGetApp( );
   if ( pApp )
   {
      TRY
      {
         if ( pApp->GetMapPointState( ) == 0 )  // not active
            return( zCALL_ERROR );

         _Application *pMP = pApp->GetMapPointApplication( );
         if ( pMP == 0 )
         {
            nRC = zCALL_ERROR;
         }
         else
         {
            _Map Map = pMP->GetActiveMap( );
         // if ( pMap == 0 )
         //    nRC = zCALL_ERROR;
         // else
            {
            // Item    item;
               zLONG   lCnt;
               VARIANT a;
               a.vt = VT_I4;
               a.lVal = 244; // geoMapNorthAmerica!
            // a.lVal = 1; // geoMapNorthAmerica!
            // a.lVal = 2; // geoMapEurope!

               FindResults FR = Map.FindAddressResults( cpcStreet, cpcCity,
                                                        cpcOtherCity,
                                                        cpcRegion,
                                                        cpcPostalCode, a );
               TraceLineI( "FindAddressResults Quality: ", FR.GetResultsQuality( ) );
               lCnt = FR.GetCount( );
               TraceLineI( "FindAddressResults Count: ", lCnt );
               for ( int k = 1; k <= lCnt; k++ )
               {
                  a.lVal = k;
                  Location L = FR.GetItem( &a );
                  long lType = L.GetType( );
                  long lX = Map.LocationToX( L );
                  long lY = Map.LocationToY( L );
                  TraceLine( "Location Name: %s  Type: %d  X: %d  Y: %d",
                             L.GetName( ), lType, lX, lY );

                  Pushpin pin;

                  if ( cpcMsg && &cpcMsg )
                     pin = Map.AddPushpin( L, cpcMsg );
                  else
                     pin = Map.AddPushpin( L, L.GetName( ) );

                  pin.SetSymbol( nPushPin );
                  if ( lFlag & 0x00000001 )
                     L.GoTo( );

                  PlaceCategory PC = L.GetPlaceCategory( );
                  TraceLineS( "The Place Category is: ", PC.GetName( ) );
               }
            }
         }
      }
      CATCH_ALL( e )
      {
         nRC = zCALL_ERROR;
      }
      END_CATCH_ALL
   }

   return( nRC );
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// There are many applications, such as navigation and radio frequency
// engineering, where it is necessary to calculate the minimum path dDistance
// and azimuth between two points on the Earth. How you calculate dDistance
// and azimuth depends on how you model the surface of the Earth. The
// simplest intuitive model for the Earth is a sphere; the problem is that
// the Earth is far from a perfect sphere. Typically, the radius of the Earth
// at the equator is taken as 6378 kilometers and 6356 kilometers and the
// North and South poles. The next level of sophistication is to assume an
// average radius for the Earth depending on latitude. Many situations
// require much greater accuracy than is afforded by the average-radius
// model. In these cases, the typical Earth model is an ellipsoid, basically
// a sphere that is flattened at the poles. Both models have symmetry about
// the polar axis as shown in the image below, but the spherical model also
// has symmetry about the center of the Earth, and that symmetry simplifies
// dDistance and azimuth calculations immensely.
//
// This article describes in detail the spherical dDistance and azimuth
// calculations and also provides sample code for three different approaches
// to dDistance calculations: one for a spherical model, and two for ellipsoid
// models.
//
// Here are three algorithms from an article by Andy McGovern for finding the
// dDistance between two points on the Earth. The first is based on the Law
// of Cosines for spherical triangles; the second method is an approximate
// calculation found in the Meeus book (a terrific book for programmers -
// he used a formula developed by Andoyer in a publication from 1950 called
// Annuaire du Bureau des Longitudes), and the third was published by the
// United States Geological Society.
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Some great circle formulae:
// Distance between points:
// The great circle distance d between two points with coordinates
// {lat1,lon1} and {lat2,lon2} is given by:
//
//    d=acos(sin(lat1)*sin(lat2)+cos(lat1)*cos(lat2)*cos(lon1-lon2))
//
// A mathematically equivalent formula, which is less subject to rounding
// error for short distances is:
//
//    d=2*asin(sqrt((sin((lat1-lat2)/2))^2 +
//             cos(lat1)*cos(lat2)*(sin((lon1-lon2)/2))^2))
//
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

#include <math.h>

// some geo constants
namespace GEO
{
   const double PI = 3.14159265359;
   const double TWOPI = 6.28318530718;
   const double DE2RA = 0.01745329252;
   const double RA2DE = 57.2957795129;
   const double ERAD = 6378.135;
   const double ERADM = 6378135.0;
   const double AVG_ERAD = 6371.0;
   const double FLATTENING = 1.0 / 298.26; // Earth flattening (WGS '72)
   const double EPS = 0.000000000005;
   const double KM2MI = 0.621371;
}

//
// Calculating distance for a spherical model is very straightforward by
// using the Law of Cosines for spherical triangles.
//
// dist = (Earth Radius) *
//        arccos( cos( 90 - lat2 ) * cos( 90 - lat1 ) +
//                sin( 90 - lat2 ) * sin( 90 - lat1 ) * cos( lon2 - lon1 ) )
//
// and since cos( t ) = sin( 90 - t )   and   sin( t ) = cos( 90 - t )
//
// double d = sin( dLat1 ) * sin( dLat2 ) +
//            cos( dLat1 ) * cos( dLat2 ) * cos( dLon1 - dLon2 );
// double dDist = GEO::AVG_ERAD * acos( d );
//
// Now let's get the formula to get the latitude a given distance from
// a specified Lat/Lon, just solve for Lat1.
//
// dDist / GEO::AVG_ERAD = acos( d )
//
// cos( dDist / GEO::AVG_ERAD ) = d
//
// cos( dDist / GEO::AVG_ERAD ) =
//            sin( dLat1 ) * sin( dLat2 ) +
//            cos( dLat1 ) * cos( dLat2 ) * cos( dLon1 - dLon2 )
//
// and since Lon1 == Lon2 and Cos( 0 ) = 1
//
// cos( dDist / GEO::AVG_ERAD ) =
//            sin( dLat1 ) * sin( dLat2 ) + cos( dLat1 ) * cos( dLat2 )
//
// now since cos( s + t ) = cos( s ) * cos( t ) - sin( s ) * sin( t )
// and       cos( s - t ) = cos( s ) * cos( t ) + sin( s ) * sin( t )
//
// cos( dDist / GEO::AVG_ERAD ) = cos( dLat1 - dLat2 )
//
// (dDist / GEO::AVG_ERAD) = dLat1 - dLat2
//
// dLat1 = (dDist / GEO::AVG_ERAD) + dLat2
//
double
GetLatitudeAtDistanceFromPoint( double dLat, double dLon, double dDist )
{
   // Distance is assumed to be in kilometers.  Lat and Lon are in degrees.
   // Getting latitude specified distance from given latitude assuming
   // same longitude.
   dLat *= GEO::DE2RA;
   dLon *= GEO::DE2RA;

   double dLatNew = (dDist / GEO::AVG_ERAD) + dLat;
   dLatNew *= GEO::RA2DE;
   return( dLatNew );
}

//
// Calculating distance for a spherical model is very straightforward by
// using the Law of Cosines for spherical triangles.
//
// dist = (Earth Radius) *
//        arccos( cos( 90 - lat2 ) * cos( 90 - lat1 ) +
//                sin( 90 - lat2 ) * sin( 90 - lat1 ) * cos( lon2 - lon1 ) )
//
// and since cos( t ) = sin( 90 - t )   and   sin( t ) = cos( 90 - t )
//
// double d = sin( dLat1 ) * sin( dLat2 ) +
//            cos( dLat1 ) * cos( dLat2 ) * cos( dLon1 - dLon2 );
// double dDist = GEO::AVG_ERAD * acos( d );
//
// Now let's get the formula to get the longitude a given distance from
// a specified Lat/Lon, just solve for Lon1.
//
// dDist / GEO::AVG_ERAD = acos( d )
//
// cos( dDist / GEO::AVG_ERAD ) = d
//
// cos( dDist / GEO::AVG_ERAD ) =
//            sin( dLat1 ) * sin( dLat2 ) +
//            cos( dLat1 ) * cos( dLat2 ) * cos( dLon1 - dLon2 )
//
// and since Lat1 == Lat2
//
// cos( dDist / GEO::AVG_ERAD ) =
//            sin( dLat ) ^ 2 + cos( dLat ) ^ 2 * cos( dLon1 - dLon2 )
//
// cos( dLon1 - dLon2 ) =
//     (cos( dDist / GEO::AVG_ERAD ) - sin( dLat ) ^ 2) / cos( dLat ) ^ 2
//
// dLon1 - dLon2 =
//   acos( (cos( dDist / GEO::AVG_ERAD ) - sin( dLat ) ^ 2) / cos( dLat ) ^ 2 )
//
// dLon1 = dLon2 +
//   acos( (cos( dDist / GEO::AVG_ERAD ) - sin( dLat ) ^ 2) / cos( dLat ) ^ 2 )
//
double
GetLongitudeAtDistanceFromPoint( double dLat, double dLon, double dDist )
{
   // Distance is assumed to be in kilometers.  Lat and Lon are in degrees.
   // Getting longitude specified distance from given longitude assuming
   // same latitude.
   dLat *= GEO::DE2RA;
   dLon *= GEO::DE2RA;

   double dCosD_ER = cos( dDist / GEO::AVG_ERAD );
   double dSinLat = sin( dLat );
   dSinLat *= dSinLat;    // square it
   double dCosLat = cos( dLat );
   dCosLat *= dCosLat;    // square it
   if ( dCosLat == 0.0 )  // ensure it's not zero
      dCosLat = 0.0000000001;

   double dLonNew = dLon + acos( (dCosD_ER - dSinLat) / dCosLat );
   dLonNew *= GEO::RA2DE;
   return( dLonNew );
}

//
// lFlag - 0x80000000 distance in kilometers rather than miles.
//         0x00000001 use best distance calculations (default)
//         0x00000002 use better distance calculations
//         0x00000004 use good distance calculations
//

zOPER_EXPORT zSHORT OPERATION
GetDistanceRectangle( zDECIMAL *pdSouthLatitude,
                      zDECIMAL *pdEastLongitude,
                      zDECIMAL *pdNorthLatitude,
                      zDECIMAL *pdWestLongitude,
                      zDECIMAL dLatitude, zDECIMAL dLongitude,
                      zDECIMAL dDistance, zLONG lFlag )
{
   zCHAR szDecimal[ 32 ];
   ZDecimal d;
   ZDecimal d2;
   d = dDistance;

   double dDist = (double) d;
   if ( (lFlag & 0x80000000) == 0 )
      dDist /= GEO::KM2MI;   // convert to kilometers

   // A degree is approximately 112 kilometers.  So we will do a quick
   // approximation and then do "search until close enough".
   d = dLatitude;
   double dLat1 = d;
   d = dLongitude;
   double dLon1 = d;
   double dLat2 = GetLatitudeAtDistanceFromPoint( dLat1, dLon1, dDist );
   double dLon2 = GetLongitudeAtDistanceFromPoint( dLat1, dLon1, dDist );

   int nTrys = 0;
   double dPrev = dLat2;
   double dCalcDist = ApproxDistance( dLat1, dLon1, dLat2, dLon1, 0x80000000 );
   double dDelta = dDist - dCalcDist;
   double dDeltaMin = dDelta;
   double dMin = dLat2;
   while ( dDelta > 0.01 || dDelta < -0.01 )
   {
      nTrys++;
      dPrev = dLat2;
      dLat2 += dDelta / 112;
      dCalcDist = ApproxDistance( dLat1, dLon1, dLat2, dLon1, 0x80000000 );
      dDelta = dDist - dCalcDist;
      if ( dDelta < dDeltaMin && dDelta > -dDeltaMin )
      {
         dDeltaMin = dDelta;
         dMin = dLat2;
      }

      if ( nTrys > 50 )
      {
         dLat2 = dMin;
         d = dDeltaMin;
         SysConvertDecimalToString( &d, szDecimal, 6 );
         TraceLineS( "LatitudeDelta: ", szDecimal );
         break;
      }
   }

   d = dLat2;
   *pdNorthLatitude = d;
   d2 = dLat1 * 2;
   *pdSouthLatitude = d2 - d;

   nTrys = 0;
   dPrev = dLon2;
   dCalcDist = ApproxDistance( dLat1, dLon1, dLat1, dLon2, 0x80000000 );
   dDelta = dDist - dCalcDist;
   dDeltaMin = dDelta;
   dMin = dLon2;
   while ( dDelta > 0.01 || dDelta < -0.01 )
   {
      nTrys++;
      dPrev = dLon2;
      dLon2 += dDelta / 100;
      dCalcDist = ApproxDistance( dLat1, dLon1, dLat1, dLon2, 0x80000000 );
      dDelta = dDist - dCalcDist;
      if ( dDelta < dDeltaMin && dDelta > -dDeltaMin )
      {
         dDeltaMin = dDelta;
         dMin = dLon2;
      }

      if ( nTrys > 50 )
      {
         dLon2 = dMin;
         d = dDeltaMin;
         SysConvertDecimalToString( &d, szDecimal, 6 );
         TraceLineS( "LongitudeDelta: ", szDecimal );
         break;
      }
   }

   d = dLon2;
   *pdWestLongitude = d;
   d2 = dLon1 * 2;
   *pdEastLongitude = d2 - d;

   if ( (lFlag & 0x80000000) == 0 )
      dDelta *= GEO::KM2MI;   // convert to miles

   return( (zSHORT) (dDelta * 100.0) );
}

zOPER_EXPORT zDECIMAL OPERATION
GetDistance( zDECIMAL dLat1, zDECIMAL dLon1,
             zDECIMAL dLat2, zDECIMAL dLon2, zLONG lFlag )
{
   ZDecimal dL1;
   dL1 = dLat1;
   ZDecimal dL2;
   dL2 = dLon1;
   ZDecimal dL3;
   dL3 = dLat2;
   ZDecimal dL4;
   dL4 = dLon2;
   ZDecimal dRC;

   if ( lFlag & 0x00000004 )
      dRC = GCDistance( dL1, dL2, dL3, dL4, lFlag );
   else
   if ( lFlag & 0x00000002 )
      dRC = EllipsoidDistance( dL1, dL2, dL3, dL4, lFlag );
   else
// if ( lFlag & 0x00000001 )
      dRC = ApproxDistance( dL1, dL2, dL3, dL4, lFlag );

   return( dRC );
}

#if 0
void
OnCalculate()
{
   zCHAR text[ 256 ];

   double dLat1, dLon1, dLat2, dLon2;
   double dDistance, dAZ;
   double dDegrees, dMinutes, dSeconds;

   CString msg = "";

   memset( text, 0, (size_t) 256 );
   m_dLat1deg_edit.GetWindowText( text, 255 );
   dDegrees = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLat1min_edit.GetWindowText( text, 255 );
   dMinutes = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLat1sec_edit.GetWindowText( text, 255 );
   dSeconds = atof( text );
   if ( (dDegrees < -90.0) || (dDegrees > 90.0) )
   {
      msg += "Latitude 1: degrees out of range, must be between (-90, 90)\n";
   }

   if ( (dMinutes < 0) || (dMinutes > 59) )
   {
      msg += "Latitude 1: minutes out of range, must be between (0, 59)\n";
   }

   if ( (dSeconds < 0) || (dSeconds >= 60) )
   {
      msg += "Latitude 1: seconds out of range, must be between (0, 59)\n";
   }

   dLat1 = mConvertDegMinSecToDecimalDeg( dDegrees, dMinutes, dSeconds );
   dLat1 = (m_dLat1_combo.GetCurSel( ) == 0) ? dLat1 : -dLat1;
// dLat1 = 41.90686;

   memset( text, 0, (size_t) 256 );
   m_dLon1deg_edit.GetWindowText( text, 255 );
   dDegrees = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLon1min_edit.GetWindowText( text, 255 );
   dMinutes = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLon1sec_edit.GetWindowText( text, 255 );
   dSeconds = atof( text );
   dLon1 = mConvertDegMinSecToDecimalDeg( dDegrees, dMinutes, dSeconds );
   dLon1 = (m_dLon1_combo.GetCurSel( ) == 0) ? dLon1 : -dLon1;
// dLon1 = 79.04500;

   if ( (dDegrees < -180.0) || (dDegrees > 180.0) )
   {
      msg += "Longitude 1: degrees out of range, must be between (-90, 90)\n";
   }

   if ( (dMinutes < 0) || (dMinutes > 59) )
   {
      msg += "Longitude 1: minutes out of range, must be between (0, 59)\n";
   }

   if ( (dSeconds < 0) || (dSeconds >= 60) )
   {
      msg += "Longitude 1: seconds out of range, must be between (0, 59)\n";
   }

   memset( text, 0, (size_t) 256 );
   m_dLat2deg_edit.GetWindowText( text, 255 );
   dDegrees = atof(text);
   memset(text,0,(size_t)256);
   m_dLat2min_edit.GetWindowText( text, 255 );
   dMinutes = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLat2sec_edit.GetWindowText( text, 255 );
   dSeconds = atof( text );
   dLat2 = mConvertDegMinSecToDecimalDeg( dDegrees, dMinutes, dSeconds );
   dLat2 = (m_dLat2_combo.GetCurSel( ) == 0 ) ? dLat2 : -dLat2;
// dLat2 = 41.83134;

   if ( (dDegrees < -90.0) || (dDegrees > 90.0) )
   {
      msg += "Latitude 2: degrees out of range, must be between (-90, 90)\n";
   }

   if ( (dMinutes < 0) || (dMinutes > 59) )
   {
      msg += "Latitude 2: minutes out of range, must be between (0, 59)\n";
   }

   if ( (dSeconds < 0) || (dSeconds >= 60) )
   {
      msg += "Latitude 2: seconds out of range, must be between (0, 59)\n";
   }

   memset( text, 0, (size_t) 256 );
   m_dLon2deg_edit.GetWindowText( text, 255 );
   dDegrees = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLon2min_edit.GetWindowText( text, 255 );
   dMinutes = atof( text );
   memset( text, 0, (size_t) 256 );
   m_dLon2sec_edit.GetWindowText( text, 255 );
   dSeconds = atof( text );
   dLon2 = mConvertDegMinSecToDecimalDeg( dDegrees, dMinutes, dSeconds );
   dLon2 = (m_dLon2_combo.GetCurSel( ) == 0) ? dLon2 : -dLon2;
// dLon2 = 79.12455;

   if ( (dDegrees < -180.0) || (dDegrees > 180.0) )
   {
      msg += "Longitude 2: degrees out of range, must be between (-90, 90)\n";
   }

   if ( (dMinutes < 0) || (dMinutes > 59) )
   {
      msg += "Longitude 2: minutes out of range, must be between (0, 59)\n";
   }

   if ( (dSeconds < 0) || (dSeconds >= 60) )
   {
      msg += "Longitude 2: seconds out of range, must be between (0, 59)\n";
   }

   if ( msg.GetLength( ) > 0 )
   {
      CString errmsg = "Please correct the following errors:\n\n";
      errmsg += msg;
      AfxMessageBox( (LPCTSTR) errmsg, MB_ICONERROR );
      return;
   }

   dDistance = GCDistance( dLat1, dLon1, dLat2, dLon2 );

   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance );
   m_gc_km_edit.SetWindowText( text );
   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance * GEO::KM2MI );
   m_gc_mi_edit.SetWindowText( text );

   dDistance = ApproxDistance( dLat1, dLon1, dLat2, dLon2 );

   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance );
   m_approx_km_edit.SetWindowText( text );
   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance * GEO::KM2MI );
   m_approx_mi_edit.SetWindowText( text );

   dDistance = EllipsoidDistance( dLat1, dLon1, dLat2, dLon2 );

   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance );
   m_ellipse_km_edit.SetWindowText( text );
   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dDistance * GEO::KM2MI );
   m_ellipse_mi_edit.SetWindowText( text );

   dAZ = GCAzimuth( dLat1, dLon1, dLat2, dLon2 );
   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dAZ );
   m_azdeg_edit.SetWindowText( text );
   memset( text, 0, (size_t) 256 );
   sprintf( text, "%.5f", dAZ * GEO::DE2RA );
   m_azrad_edit.SetWindowText( text );
}
#endif

zOPER_EXPORT double OPERATION
GCDistance( double dLat1, double dLon1,
            double dLat2, double dLon2, zLONG lFlag )
{
   dLat1 *= GEO::DE2RA;
   dLon1 *= GEO::DE2RA;
   dLat2 *= GEO::DE2RA;
   dLon2 *= GEO::DE2RA;

   double d = sin( dLat1 ) * sin( dLat2 ) +
              cos( dLat1 ) * cos( dLat2 ) * cos( dLon1 - dLon2 );
   double dDistance = GEO::AVG_ERAD * acos( d );
   if ( (lFlag & 0x80000000) == 0 )
      dDistance *= GEO::KM2MI;   // convert to miles

   return( dDistance );
}

zOPER_EXPORT double OPERATION
GCAzimuth( double dLat1, double dLon1,
           double dLat2, double dLon2 )
{
   double dResult = 0.0;

   INT32 idLat1 = (INT32) (0.50 + dLat1 * 360000.0);
   INT32 idLat2 = (INT32) (0.50 + dLat2 * 360000.0);
   INT32 idLon1 = (INT32) (0.50 + dLon1 * 360000.0);
   INT32 idLon2 = (INT32) (0.50 + dLon2 * 360000.0);

   dLat1 *= GEO::DE2RA;
   dLon1 *= GEO::DE2RA;
   dLat2 *= GEO::DE2RA;
   dLon2 *= GEO::DE2RA;

   if ( (idLat1 == idLat2) && (idLon1 == idLon2) )
   {
      return( dResult );
   }
   else
   if ( idLat1 == idLat2 )
   {
      if ( idLon1 > idLon2 )
         dResult = 90.0;
      else
         dResult = 270.0;
   }
   else
   if ( idLon1 == idLon2 )
   {
      if ( idLat1 > idLat2 )
         dResult = 180.0;
   }
   else
   {
      double c = acos( sin( dLat2 ) * sin( dLat1 ) +
                         cos( dLat2 ) * cos( dLat1 ) * cos( dLon2 - dLon1 ) );
      double A = asin( cos( dLat2 ) * sin( dLon2 - dLon1 ) / sin( c ) );
      dResult = (A * GEO::RA2DE);

      if ( (idLat2 > idLat1) && (idLon2 > idLon1) )
      {
      }
      else
      if ( (idLat2 < idLat1) && (idLon2 < idLon1) )
      {
         dResult = 180.0 - dResult;
      }
      else
      if ( (idLat2 < idLat1) && (idLon2 > idLon1) )
      {
         dResult = 180.0 - dResult;
      }
      else
      if ( (idLat2 > idLat1) && (idLon2 < idLon1) )
      {
         dResult += 360.0;
      }
   }

   return( dResult );
}

zOPER_EXPORT double OPERATION
ApproxDistance( double dLat1, double dLon1,
                double dLat2, double dLon2, zLONG lFlag )
{
   double dDistance;
   dLat1 = GEO::DE2RA * dLat1;
   dLon1 = -GEO::DE2RA * dLon1;
   dLat2 = GEO::DE2RA * dLat2;
   dLon2 = -GEO::DE2RA * dLon2;

   double F = (dLat1 + dLat2) / 2.0;
   double G = (dLat1 - dLat2) / 2.0;
   double L = (dLon1 - dLon2) / 2.0;

   double dSinG = sin( G );
   double dCosL = cos( L );
   double dCosF = cos( F );
   double dSinL = sin( L );
   double dSinF = sin( F );
   double dCosG = cos( G );

   double S = dSinG * dSinG * dCosL * dCosL + dCosF * dCosF * dSinL * dSinL;
   double C = dCosG * dCosG * dCosL * dCosL + dSinF * dSinF * dSinL * dSinL;
   double W = atan2( sqrt( S ), sqrt( C ) );
   double R = sqrt( (S * C) ) / W;
   double H1 = ((3 * R) - 1.0) / (2.0 * C);
   double H2 = ((3 * R) + 1.0) / (2.0 * S);
   double D = 2 * W * GEO::ERAD;
   dDistance = D * (1 + GEO::FLATTENING * H1 * dSinF * dSinF * dCosG * dCosG -
                     GEO::FLATTENING * H2 * dCosF * dCosF * dSinG * dSinG);

   if ( (lFlag & 0x80000000) == 0 )
      dDistance *= GEO::KM2MI;   // convert to miles

   return( dDistance );
}

zOPER_EXPORT double OPERATION
EllipsoidDistance( double dLat1, double dLon1,
                   double dLat2, double dLon2, zLONG lFlag )
{
   double  dDistance = 0.0;
   double  dFaz, dBaz;
   double  r = 1.0 - GEO::FLATTENING;
   double  tu1, tu2, cu1, su1, cu2, x, sx, cx, sy, cy, y, sa, c2a, cz, e, c, d;
   double  dCosY1, dCosY2;
   dDistance = 0.0;

   if ( (dLon1 == dLon2) && (dLat1 == dLat2) )
      return( dDistance );

   dLon1 *= GEO::DE2RA;
   dLon2 *= GEO::DE2RA;
   dLat1 *= GEO::DE2RA;
   dLat2 *= GEO::DE2RA;

   dCosY1 = cos( dLat1 );
   dCosY2 = cos( dLat2 );

   if ( dCosY1 == 0.0 )
      dCosY1 = 0.0000000001;

   if ( dCosY2 == 0.0 )
      dCosY2 = 0.0000000001;

   tu1 = r * sin( dLat1 ) / dCosY1;
   tu2 = r * sin( dLat2 ) / dCosY2;
   cu1 = 1.0 / sqrt( tu1 * tu1 + 1.0 );
   su1 = cu1 * tu1;
   cu2 = 1.0 / sqrt( tu2 * tu2 + 1.0 );
   x = dLon2 - dLon1;

   dDistance = cu1 * cu2;
   dBaz = dDistance * tu2;
   dFaz = dBaz * tu1;

   do
   {
      sx = sin( x );
      cx = cos( x );
      tu1 = cu2 * sx;
      tu2 = dBaz - su1 * cu2 * cx;
      sy = sqrt( tu1 * tu1 + tu2 * tu2 );
      cy = dDistance * cx + dFaz;
      y = atan2( sy, cy );
      sa = dDistance * sx / sy;
      c2a = -sa * sa + 1.0;
      cz = dFaz + dFaz;
      if ( c2a > 0.0 )
         cz = -cz / c2a + cy;

      e = cz * cz * 2. - 1.0;
      c = ((-3.0 * c2a + 4.0) * GEO::FLATTENING + 4.0) * c2a * GEO::FLATTENING / 16.0;
      d = x;
      x = ((e * cy * c + cz) * sy * c + y ) * sa;
      x = (1.0 - c) * x * GEO::FLATTENING + dLon2 - dLon1;

   } while ( fabs( d - x ) > GEO::EPS );

   x = sqrt( (1.0 / r / r - 1.0) * c2a + 1.0 ) + 1.0;
   x = (x - 2.0) / x;
   c = 1.0 - x;
   c = (x * x / 4.0 + 1.0) / c;
   d = (0.375 * x * x - 1.0) * x;
   x = e * cy;
   dDistance = 1.0 - e - e;
   dDistance = ((((sy * sy * 4.0 - 3.0) * dDistance * cz * d / 6.0 - x) *
                  d / 4.0 + cz) * sy * d + y) * c * GEO::ERAD * r;

   if ( (lFlag & 0x80000000) == 0 )
      dDistance *= GEO::KM2MI;   // convert to miles

   return( dDistance );
}
