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

#if defined(T271_SOLENOID_ENABLE)

Team271Solenoid::Team271Solenoid( Team271Base* const argParent,
                                  const string& argName,
                                  const uint32_t argPCMModuleA,
                                  const uint32_t argPCMChannelA )
      : Team271Base( argParent,
                     argName,
                     t271_solenoid_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   /*
    * Store Parameters
    */
   Write( t271_solenoid_data_index::T271_SOLENOID_DATA_TYPE,
          TEAM271_SOLENOID_TYPE_SINGLE );

   SetSingle( argPCMModuleA,
              argPCMChannelA );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271Solenoid::Team271Solenoid( Team271Base* const argParent,
                                  const string& argName,
                                  const uint32_t argPCMModuleA,
                                  const uint32_t argPCMChannelA,
                                  const uint32_t argPCMModuleB,
                                  const uint32_t argPCMChannelB )
      : Team271Base( argParent,
                     argName,
                     t271_solenoid_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   Write( t271_solenoid_data_index::T271_SOLENOID_DATA_TYPE,
          TEAM271_SOLENOID_TYPE_DOUBLE );

   SetDouble( argPCMModuleA,
              argPCMChannelA,
              argPCMModuleB,
              argPCMChannelB );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271Solenoid::~Team271Solenoid( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Solenoid::GetClassName( void ) const
{
   return "Team271Solenoid";
}

TEAM271_SOLENOID_STATE Team271Solenoid::GetValuePrev( void ) const
{
   return Read<TEAM271_SOLENOID_STATE>( t271_solenoid_data_index::T271_SOLENOID_DATA_VALUE_PREV );
}

TEAM271_SOLENOID_STATE Team271Solenoid::GetValueCurrent( void ) const
{
   return Read<TEAM271_SOLENOID_STATE>( t271_solenoid_data_index::T271_SOLENOID_DATA_VALUE_CURRENT );
}

/*
 *
 * Solenoid Functions
 *
 */
void Team271Solenoid::SetOutput( const TEAM271_SOLENOID_STATE argSet )
{
   /*
    * Store the value
    */
   Write( t271_solenoid_data_index::T271_SOLENOID_DATA_VALUE_CURRENT,
          argSet );
}

/*
 *
 * Setters
 *
 */
void Team271Solenoid::SetSingle( const uint32_t argPCMModuleA,
                                 const uint32_t argChannelA )
{
   if( argPCMModuleA > ( Team271PCM::kMaxInstance - 1 ) )
   {
      LOG_ERROR( "%s - Invalid Module Parameter - Module A: %d",
                 GetName().c_str(),
                 argPCMModuleA );
   }
   else if( argChannelA >= kNumSolenoidChannels )
   {
      LOG_ERROR( "%s - Invalid Channel Parameter - Channel A: %d",
                 GetName().c_str(),
                 argChannelA );
   }
   else
   {
      Write( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A,
             argPCMModuleA );
      Write( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A,
             argChannelA );
   }
}

void Team271Solenoid::SetDouble( const uint32_t argPCMModuleA,
                                 const uint32_t argChannelA,
                                 const uint32_t argPCMModuleB,
                                 const uint32_t argChannelB )
{
   if( Read<TEAM271_SOLENOID_TYPE>( t271_solenoid_data_index::T271_SOLENOID_DATA_TYPE ) != TEAM271_SOLENOID_TYPE_DOUBLE )
   {
      LOG_ERROR( "%s - Invalid Type - Type: %d",
                 GetName().c_str(),
                 Read<TEAM271_SOLENOID_TYPE>( t271_solenoid_data_index::T271_SOLENOID_DATA_TYPE ) );
   }
   else
   {
      SetSingle( argPCMModuleA,
                 argChannelA );

      if( argPCMModuleB > ( Team271PCM::kMaxInstance - 1 ) )
      {
         LOG_ERROR( "%s - Invalid Module Parameter - Channel A: %d",
                    GetName().c_str(),
                    argChannelB );
      }
      else if( argChannelB >= kNumSolenoidChannels )
      {
         LOG_ERROR( "%s - Invalid Channel Parameter - Channel A: %d",
                    GetName().c_str(),
                    argChannelB );
      }
      else
      {
         Write( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_B,
                argPCMModuleB );
         Write( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_B,
                argChannelB );
      }
   }
}

/*
 *
 * Robot
 *
 */
void Team271Solenoid::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   Set( t271_solenoid_data_index::T271_SOLENOID_DATA_VALUE_PREV,
        t271_solenoid_data_index::T271_SOLENOID_DATA_VALUE_CURRENT );
}

void Team271Solenoid::PostRobotPeriodic( const bool argIsParent )
{
   if( GetValueCurrent() != GetValuePrev() )
   {
      if( Read<TEAM271_SOLENOID_TYPE>( t271_solenoid_data_index::T271_SOLENOID_DATA_TYPE ) == TEAM271_SOLENOID_TYPE_SINGLE )
      {
         if( (GetValueCurrent() == TEAM271_SOLENOID_STATE_OFF) || (GetValueCurrent() == TEAM271_SOLENOID_STATE_REV) )
         {

            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),                                                                                          0 );
/*
            LOG_NOTICE( "%s - Off",
                        GetName().c_str() );
*/
         }
         else
         {
            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),
                                                                                                                       1 );
/*
            LOG_NOTICE( "%s - On",
                        GetName().c_str() );
*/
         }
      }
      else
      {
         if( GetValueCurrent() == TEAM271_SOLENOID_STATE_ON )
         {
            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),
                                                                                                                       1 );

            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_B ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_B ),
                                                                                                                       1 );
/*
            LOG_NOTICE( "%s - Double On",
                        GetName().c_str() );
*/
         }
         else if( GetValueCurrent() == TEAM271_SOLENOID_STATE_REV )
         {
            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),
                                                                                                                       0 );

            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_B ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_B ),
                                                                                                                       1 );
/*
            LOG_NOTICE( "%s - Double Rev",
                        GetName().c_str() );
*/
         }
         else if( GetValueCurrent() == TEAM271_SOLENOID_STATE_FWD )
         {
            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),
                                                                                                                       1 );

            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_B ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_B ),
                                                                                                                       0 );
/*
            LOG_NOTICE( "%s - Double Fwd",
                        GetName().c_str() );
*/
         }
         else
         {
            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_A ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_A ),
                                                                                                                       0 );

            Team271PCM::getInstance( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_MODULE_B ) ).Set( Read<uint8_t>( t271_solenoid_data_index::T271_SOLENOID_DATA_PCM_CHANNEL_B ),
                                                                                                                       0 );
/*
            LOG_NOTICE( "%s - Double Off",
                        GetName().c_str() );
*/
         }
      }
   }

   /*
    * Base Call
    */
   Team271Base::PostRobotPeriodic();
}

#endif

