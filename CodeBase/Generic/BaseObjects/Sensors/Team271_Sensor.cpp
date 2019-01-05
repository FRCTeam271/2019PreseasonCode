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

#if defined(T271_SENSOR_ENABLE)

Team271Sensor::Team271Sensor( Team271Base* const argParent,
                              const string& argSensorName,
                              const uint8_t argMessageCount,
                              const TEAM271_SENSOR_TYPE argSensorType,
                              const TEAM271_INVERTED_STATE argSensorInverted,
                              const TEAM271_SENSOR_SETTINGS argSensorSettings,
                              const float argArgumentA )
      : Team271Base( argParent,
                     argSensorName,
                     argMessageCount )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_TYPE,
          argSensorType );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_SETTINGS,
          argSensorSettings );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_INVERTED,
          argSensorInverted );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_ARGUMENT_A,
          argArgumentA );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_FWD,
          TEAM271_SENSOR_LIMIT_SW_STATE_OPEN );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_REV,
          TEAM271_SENSOR_LIMIT_SW_STATE_OPEN );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271Sensor::~Team271Sensor( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Sensor::GetClassName( void ) const
{
   return "Team271Sensor";
}

/* Position */
float Team271Sensor::GetPosition( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_POSITION );
}

float Team271Sensor::GetPositionX( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_X );
}

float Team271Sensor::GetPositionY( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Y );
}

float Team271Sensor::GetPositionZ( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Z );
}

/* Rate */
float Team271Sensor::GetRate( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_RATE );
}

float Team271Sensor::GetRateX( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_RATE_X );
}

float Team271Sensor::GetRateY( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_RATE_Y );
}

float Team271Sensor::GetRateZ( const float argConversion ) const
{
   return Read<float>( t271_sensor_data_index::T271_SENSOR_DATA_RATE_Z );
}

/* Switches */
TEAM271_SENSOR_LIMIT_SW_STATE Team271Sensor::GetLimitSWFwd( void ) const
{
   return Read<TEAM271_SENSOR_LIMIT_SW_STATE>( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_FWD );
}

TEAM271_SENSOR_LIMIT_SW_STATE Team271Sensor::GetLimitSWRev( void ) const
{
   return Read<TEAM271_SENSOR_LIMIT_SW_STATE>( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_REV );
}

/*
 *
 * Setters
 *
 */
void Team271Sensor::SetPosition( const float argPosition )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION,
          argPosition );
}

void Team271Sensor::SetPositionX( const float argPosition )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_X,
          argPosition );
}

void Team271Sensor::SetPositionY( const float argPosition )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Y,
          argPosition );
}

void Team271Sensor::SetPositionZ( const float argPosition )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Z,
          argPosition );
}

void Team271Sensor::SetRate( const float argRate )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_RATE,
          argRate );
}

void Team271Sensor::SetRateX( const float argRate )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_RATE_X,
          argRate );
}

void Team271Sensor::SetRateY( const float argRate )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_RATE_Y,
          argRate );
}

void Team271Sensor::SetRateZ( const float argRate )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_RATE_Z,
          argRate );
}

void Team271Sensor::SetLimitFwd( const TEAM271_SENSOR_LIMIT_SW_STATE argState )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_FWD,
          argState );
}

void Team271Sensor::SetLimitRev( const TEAM271_SENSOR_LIMIT_SW_STATE argState )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_LIMIT_REV,
          argState );
}

/*
 *
 * Robot
 *
 */
void Team271Sensor::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   /*
    * Store Previous Connected Status
    */
   Set( t271_sensor_data_index::T271_SENSOR_DATA_CONNECTED_PREV,
        t271_sensor_data_index::T271_SENSOR_DATA_CONNECTED );

   if( Team271Robot::CompareTime( TimeSensorSwitchUpdateLast_,
                                  kSensorSwitchUpdateRate ) == 1 )
   {
      UpdateSwitches();
   }

   if( Team271Robot::CompareTime( TimeSensorPositionUpdateLast_,
                                  kSensorPositionUpdateRate ) == 1 )
   {
      UpdatePositions();
   }

   if( Team271Robot::CompareTime( TimeSensorRateUpdateLast_,
                                  kSensorRateUpdateRate ) == 1 )
   {
      UpdateRates();
   }
}

/*
 *
 * Sensor Functions
 *
 */
void Team271Sensor::UpdateSwitches( void )
{

}

void Team271Sensor::UpdatePositions( const float argConversion )
{

}

void Team271Sensor::UpdateRates( const float argConversion )
{

}

#endif

