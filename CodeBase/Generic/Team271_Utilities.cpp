/*
 *    FRC Team 271's 2019 Pre Season Code
 *    Copyright (C) 2019  FRC Team 271
 *
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "Team271_Includes.h"

/*
 * Round
 */
inline float Team271_Round( const double argNumber )
{
   if( argNumber < 0 )
   {
      return ceil( argNumber - 0.5 );
   }

   return floor( argNumber + 0.5 );
}

/*
 * 3 Places
 */
float Team271_Round3Places( const double argNumber )
{
   return Team271_Round( argNumber * 1000.0 ) / 1000.0;
}

/*
 * 4 Places
 */
float Team271_Round4Places( const double argNumber )
{
   return Team271_Round( argNumber * 10000.0 ) / 10000.0;
}

long Team271_Map( const long x,
                  const long in_min,
                  const long in_max,
                  const long out_min,
                  const long out_max )
{
   return ( x - in_min ) * ( out_max - out_min ) / ( in_max - in_min ) + out_min;
}

double Team271_Remap( const double newValueToMap,
                      const double newInMin,
                      const double newInMax,
                      const double newOutMin,
                      const double newOutMax )
{
   return ( newValueToMap - newInMin ) * ( newOutMax - newOutMin ) / ( newInMax - newInMin ) + newOutMin;
}

bool Team271_Compare( const double newA,
                      const double newB )
{
   return fabs( newA - newB ) < std::numeric_limits<double>::epsilon();
}

void ApplyRadialDeadZone( float& pOutX,   // out: resulting stick x value
                          float& pOutY,   // out: resulting stick y value
                          const float x,   // in: initial stick x value
                          const float y,   // in: initial stick x value
                          const float deadZoneLow,   // in: distance from zero to ignore
                          const float deadZoneHigh   // in: distance from unit circle to ignore
                          )
{
   const float mag = sqrt( x * x + y * y );

   if( mag > deadZoneLow )
   {
      // scale such that output magnitude is in the range [0.0f, 1.0f]
      const float legalRange = 1.0f - deadZoneHigh - deadZoneLow;
      const float normalizedMag = min( 1.0f,
                                       ( mag - deadZoneLow ) / legalRange );
      const float scale = normalizedMag / mag;
      pOutX = x * scale;
      pOutY = y * scale;
   }
   else
   {
      // stick is in the inner dead zone
      pOutX = 0.0f;
      pOutY = 0.0f;
   }
}

#if defined( ENABLE_DEBUG_PRINTING )

/* buffer size*/
#define _DEBUG_BUFFSIZE  (128U)
void Team271DebugPrint( const char* const argClassName,
                        const char* const argLogFmt,
                        ... )
{
   uint32_t tmpSizeA = 0;

   char tmpFullMsg[_DEBUG_BUFFSIZE] = "";

   tmpSizeA = snprintf( tmpFullMsg,
                        _DEBUG_BUFFSIZE,
                        "**** %s - ",
                        argClassName );

   va_list args;
   va_start( args,
             argLogFmt );

   vsnprintf( &tmpFullMsg[tmpSizeA],
              _DEBUG_BUFFSIZE - tmpSizeA,
              argLogFmt,
              args );

   va_end( args );

   printf( "%s ****\r\n",
           tmpFullMsg );
}

void Team271DebugPrintFunc( const char* const argClassName,
                            const char* const argFuncName,
                            const char* const argLogFmt,
                            ... )
{
   uint32_t tmpSizeA = 0;

   char tmpFullMsg[_DEBUG_BUFFSIZE] = "";

   if( strlen( argLogFmt ) == 0 )
   {
      tmpSizeA = snprintf( tmpFullMsg,
                           _DEBUG_BUFFSIZE,
                           "**** %s - %s",
                           argClassName,
                           argFuncName );
   }
   else
   {
      tmpSizeA = snprintf( tmpFullMsg,
                           _DEBUG_BUFFSIZE,
                           "**** %s - %s - ",
                           argClassName,
                           argFuncName );
   }

   va_list args;
   va_start( args,
             argLogFmt );

   vsnprintf( &tmpFullMsg[tmpSizeA],
              _DEBUG_BUFFSIZE - tmpSizeA,
              argLogFmt,
              args );

   va_end( args );

   printf( "%s ****\r\n",
           tmpFullMsg );
}
#endif
