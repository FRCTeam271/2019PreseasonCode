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

#if defined(T271_PWM_ENABLE)

Team271PWM::Team271PWM( Team271Base* const argParent,
                        const uint32_t argChannel )
      : Team271Base( argParent,
                     to_string( argChannel ),
                     t271_pwm_data_index::T271_DATA_COUNT )
{
   int32_t tmpStatus = 0;

   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );

   /*
    * Store Parameters
    */
   Write( t271_pwm_data_index::T271_PWM_DATA_CHANNEL,
          argChannel );

   if( argChannel < kNumPWMChannels )
   {
      Write( t271_pwm_data_index::T271_PWM_DATA_HANDLE,
             0 );

      LOG_ERROR( "%s - Couldn't Open Channel %d",
                 GetName().c_str(),
                 argChannel );
   }
   else
   {
      Write( t271_pwm_data_index::T271_PWM_DATA_HANDLE,
             argChannel );

#if defined( T271_IO_ENABLE )
      Team271IO::getInstance().PWM_InitializePort( argChannel,
                                                   &tmpStatus );

      Team271IO::getInstance().PWM_SetRaw( argChannel,
                                           Team271IO::kPwmDisabled,
                                           &tmpStatus );

      Team271IO::getInstance().PWM_SetEliminateDeadband( argChannel,
                                                         false );
#endif
   }

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}

Team271PWM::~Team271PWM( void )
{
#if defined( T271_IO_ENABLE )
   int32_t tmpStatus = 0;

   if( Read<uint32_t>( t271_pwm_data_index::T271_PWM_DATA_HANDLE ) != 0 )
   {
      Team271IO::getInstance().PWM_SetRaw( Read<uint32_t>( t271_pwm_data_index::T271_PWM_DATA_HANDLE ),
                                           Team271IO::kPwmDisabled,
                                           &tmpStatus );

      Team271IO::getInstance().PWM_FreePort( Read<uint32_t>( t271_pwm_data_index::T271_PWM_DATA_HANDLE ),
                                             &tmpStatus );
   }
#endif
}

/*
 *
 * Getters
 *
 */
const string Team271PWM::GetClassName( void ) const
{
   return "Team271PWM";
}

/*
 *
 * Setters
 *
 */
void Team271PWM::SetInverted( const TEAM271_INVERTED_STATE argInverted )
{
   Write( t271_pwm_data_index::T271_PWM_DATA_INVERTED,
          argInverted );
}

/*
 *
 * Output Functions
 *
 */
void Team271PWM::Set( const double argSet )
{
#if defined( T271_IO_ENABLE )
   int32_t tmpStatus = 0;
   double tmpSet = argSet;

   if( Read<TEAM271_INVERTED_STATE>( t271_pwm_data_index::T271_PWM_DATA_INVERTED ) == TEAM271_INVERTED_STATE_INVERTED )
   {
      tmpSet *= -1.0;
   }

   if( Read<uint32_t>( t271_pwm_data_index::T271_PWM_DATA_HANDLE ) != 0 )
   {
      Team271IO::getInstance().PWM_SetSpeed( Read<uint32_t>( t271_pwm_data_index::T271_PWM_DATA_HANDLE ),
                                             tmpSet,
                                             &tmpStatus );
   }
#endif
}

#endif

