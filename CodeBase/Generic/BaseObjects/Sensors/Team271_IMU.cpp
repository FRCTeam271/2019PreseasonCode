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

#if defined(T271_IMU_ENABLE)

#if defined( T271_I2C_ENABLE )
Team271IMU::Team271IMU( Team271Base* const argParent,
                        const string& argName,
                        const uint8_t argMessageCount,
                        TEAM271_I2C_PORT argI2CPort,
                        uint32_t argI2CAddress )
      : Team271Sensor( argParent,
                       argName,
                       argMessageCount,
                       TEAM271_SENSOR_TYPE_IMU )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_STATE,
          TEAM271_IMU_STATE_RESET );

   /*
    * Declination at Bay Shore, NY is 13 degrees 14 minutes on 2018-11-23
    */
   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_DECLINATION,
          13.14 );
   /*
    * Create I2C
    */
   I2C_ = new Team271I2C( argI2CPort,
                          argI2CAddress );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}
#endif

#if defined( T271_SPI_ENABLE )
Team271IMU::Team271IMU( const string argName,
      TEAM271_SPI_PORT argSPIPort )
: Team271Sensor( string( "Team271IMU-" ) + argName,
      TEAM271_SENSOR_TYPE_IMU )
{
   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );

   SPI_ = new Team271SPI( argSPIPort );

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}
#endif

Team271IMU::~Team271IMU( void )
{
   ThreadIMU_Run_.store( 0,
                         std::memory_order_relaxed );

   if( ThreadIMU_ != nullptr )
   {
      if( ThreadIMU_->joinable() )
      {
         ThreadIMU_->join();
      }

      delete ThreadIMU_;
   }

   ThreadIMU_ = nullptr;

#if defined( T271_I2C_ENABLE )
   if( I2C_ != nullptr )
   {
      delete I2C_;
   }

   I2C_ = nullptr;
#endif

#if defined( T271_SPI_ENABLE )
   if( SPI_ != nullptr )
   {
      delete SPI_;
   }

   SPI_ = nullptr;
#endif
}

/*
 *
 * Getters
 *
 */
const string Team271IMU::GetClassName( void ) const
{
   return "Team271IMU";
}

float Team271IMU::GetQW( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QW );
}

float Team271IMU::GetQX( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QX );
}

float Team271IMU::GetQY( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QY );
}

float Team271IMU::GetQZ( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QZ );
}

float Team271IMU::GetRoll( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_DATA_POSITION_X );
}

float Team271IMU::GetPitch( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_DATA_POSITION_Y );
}

float Team271IMU::GetYaw( void ) const
{
   return Read<float>( t271_sensor_imu_data_index::T271_SENSOR_DATA_POSITION_Z );
}

Team271Sensor* Team271IMU::GetAccel( void ) const
{
   return Accel_;
}

float Team271IMU::GetAccelX( void ) const
{
   if( Accel_ != nullptr )
   {
      return Accel_->GetRateX();
   }

   return 0;
}

float Team271IMU::GetAccelY( void ) const
{
   if( Accel_ != nullptr )
   {
      return Accel_->GetRateY();
   }

   return 0;
}

float Team271IMU::GetAccelZ( void ) const
{
   if( Accel_ != nullptr )
   {
      return Accel_->GetRateZ();
   }

   return 0;
}

Team271Sensor* Team271IMU::GetGyro( void ) const
{
   return Gyro_;
}

float Team271IMU::GetGyroX( void ) const
{
   if( Gyro_ != nullptr )
   {
      return Gyro_->GetRateX();
   }

   return 0;
}

float Team271IMU::GetGyroY( void ) const
{
   if( Gyro_ != nullptr )
   {
      return Gyro_->GetRateY();
   }

   return 0;
}

float Team271IMU::GetGyroZ( void ) const
{
   if( Gyro_ != nullptr )
   {
      return Gyro_->GetRateZ();
   }

   return 0;
}

/*
 *
 * Setters
 *
 */
void Team271IMU::SetQW( const float argValue )
{
   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QW,
          argValue );
}

void Team271IMU::SetQX( const float argValue )
{
   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QX,
          argValue );
}

void Team271IMU::SetQY( const float argValue )
{
   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QY,
          argValue );
}

void Team271IMU::SetQZ( const float argValue )
{
   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_QZ,
          argValue );
}

void Team271IMU::SetRoll( const float argValue )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_X,
          argValue );
}

void Team271IMU::SetPitch( const float argValue )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Y,
          argValue );
}

void Team271IMU::SetYaw( const float argValue )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_POSITION_Z,
          argValue );
}

void Team271IMU::SetAccelX( const float argValue )
{
   if( Accel_ != nullptr )
   {
      Accel_->SetRateX( argValue );
   }
}

void Team271IMU::SetAccelY( const float argValue )
{
   if( Accel_ != nullptr )
   {
      Accel_->SetRateY( argValue );
   }
}

void Team271IMU::SetAccelZ( const float argValue )
{
   if( Accel_ != nullptr )
   {
      Accel_->SetRateZ( argValue );
   }
}

void Team271IMU::SetGyroX( const float argValue )
{
   if( Gyro_ != nullptr )
   {
      Gyro_->SetRateX( argValue );
   }
}

void Team271IMU::SetGyroY( const float argValue )
{
   if( Gyro_ != nullptr )
   {
      Gyro_->SetRateY( argValue );
   }
}

void Team271IMU::SetGyroZ( const float argValue )
{
   if( Gyro_ != nullptr )
   {
      Gyro_->SetRateZ( argValue );
   }
}

/*
 *
 * Base
 *
 */
void Team271IMU::Init( void )
{
   /*
    * Create Accel
    */
   Accel_ = Team271BaseManager::InstanceAdd( new Team271Sensor( this,
                                                                GetName() + string( " - Accel" ),
                                                                t271_sensor_data_index::T271_DATA_COUNT,
                                                                TEAM271_SENSOR_TYPE_ACCEL ) );

   /*
    * Create Gyro
    */
   Gyro_ = Team271BaseManager::InstanceAdd( new Team271Sensor( this,
                                                               GetName() + string( " - Gyro" ),
                                                               t271_sensor_data_index::T271_DATA_COUNT,
                                                               TEAM271_SENSOR_TYPE_GYRO ) );

   /*
    * Create Thread to Run the IMU State Machine
    */
   /*
    ThreadIMU_ = new std::thread( std::bind( &Team271IMU::UpdateIMU,
    this ) );
    */
}

/*
 *
 * Robot
 *
 */
void Team271IMU::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Sensor::RobotInit();

   Write( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_STATE,
          TEAM271_IMU_STATE_RESET );
}

/*
 *
 * IMU Functions
 *
 */
void Team271IMU::UpdateIMU( void )
{

}

/*
 *
 * Gyro Functions
 *
 */
void Team271IMU::GyroCal( void )
{

}

void Team271IMU::GyroInit( void )
{

}

void Team271IMU::GyroReset( void )
{

}

#endif
