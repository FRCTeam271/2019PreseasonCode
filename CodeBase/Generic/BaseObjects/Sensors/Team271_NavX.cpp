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

#if defined(T271_NAVX_ENABLE)

#if !defined(T271_DASHBOARD)

Team271NavX::Team271NavX( void )
      : Team271Sensor( string( "Team271NavX" ),
                       TEAM271_SENSOR_TYPE_IMU )
{
   LOG_TRACE( "%s - Constructor Start",
               _ClassName.c_str() );

   try
   {
      _AHRS = new AHRS( SerialPort::Port::kUSB,
                        AHRS::SerialDataType::kProcessedData,
                        10 );
      //_AHRS = new AHRS( SerialPort::Port::kUSB );
      if( _AHRS != nullptr )
      {
         _AHRS->EnableLogging( true );
      }
   }
   catch( std::exception& ex )
   {
      std::string err_string = "Error instantiating navX MXP:  ";
      err_string += ex.what();

      LOG_ERROR( "%s",
                 err_string.c_str() );

      DriverStation::ReportError( err_string.c_str() );
   }

   LOG_TRACE( "%s - Constructor End",
               _ClassName.c_str() );
}

Team271NavX::~Team271NavX( void )
{

}

/*
 *
 * Robot
 *
 */
void Team271NavX::PreRobotPeriodic( void )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   /*
    * Update Gyro
    */
   if( _AHRS != nullptr )
   {
      _GyroZ = _AHRS->GetAngle();
   }
}

/*
 *
 * Dashboard
 *
 */
void Team271NavX::UpdateNetwork( void )
{
   /*
    * Base Call
    */
   Team271Base::UpdateNetwork();

   if( _AHRS != nullptr )
   {
      SmartDashboard::PutBoolean( "IMU_Connected",
                                  _AHRS->IsConnected() );
      SmartDashboard::PutNumber( "IMU_Yaw",
                                 _AHRS->GetYaw() );
      SmartDashboard::PutNumber( "IMU_Pitch",
                                 _AHRS->GetPitch() );
      SmartDashboard::PutNumber( "IMU_Roll",
                                 _AHRS->GetRoll() );
      SmartDashboard::PutNumber( "IMU_CompassHeading",
                                 _AHRS->GetCompassHeading() );
      SmartDashboard::PutNumber( "IMU_Update_Count",
                                 _AHRS->GetUpdateCount() );
      SmartDashboard::PutNumber( "IMU_Byte_Count",
                                 _AHRS->GetByteCount() );
      SmartDashboard::PutNumber( "IMU_Timestamp",
                                 _AHRS->GetLastSensorTimestamp() );

      /* These functions are compatible w/the WPI Gyro Class */
      SmartDashboard::PutNumber( "IMU_TotalYaw",
                                 _AHRS->GetAngle() );
      SmartDashboard::PutNumber( "IMU_YawRateDPS",
                                 _AHRS->GetRate() );

      SmartDashboard::PutNumber( "IMU_Accel_X",
                                 _AHRS->GetWorldLinearAccelX() );
      SmartDashboard::PutNumber( "IMU_Accel_Y",
                                 _AHRS->GetWorldLinearAccelY() );
      SmartDashboard::PutBoolean( "IMU_IsMoving",
                                  _AHRS->IsMoving() );
      SmartDashboard::PutNumber( "IMU_Temp_C",
                                 _AHRS->GetTempC() );
      SmartDashboard::PutBoolean( "IMU_IsCalibrating",
                                  _AHRS->IsCalibrating() );

      SmartDashboard::PutNumber( "Velocity_X",
                                 _AHRS->GetVelocityX() );
      SmartDashboard::PutNumber( "Velocity_Y",
                                 _AHRS->GetVelocityY() );
      SmartDashboard::PutNumber( "Displacement_X",
                                 _AHRS->GetDisplacementX() );
      SmartDashboard::PutNumber( "Displacement_Y",
                                 _AHRS->GetDisplacementY() );

      /* Display Raw Gyro/Accelerometer/Magnetometer Values                       */
      /* NOTE:  These values are not normally necessary, but are made available   */
      /* for advanced users.  Before using this data, please consider whether     */
      /* the processed data (see above) will suit your needs.                     */

      SmartDashboard::PutNumber( "RawGyro_X",
                                 _AHRS->GetRawGyroX() );
      SmartDashboard::PutNumber( "RawGyro_Y",
                                 _AHRS->GetRawGyroY() );
      SmartDashboard::PutNumber( "RawGyro_Z",
                                 _AHRS->GetRawGyroZ() );
      SmartDashboard::PutNumber( "RawAccel_X",
                                 _AHRS->GetRawAccelX() );
      SmartDashboard::PutNumber( "RawAccel_Y",
                                 _AHRS->GetRawAccelY() );
      SmartDashboard::PutNumber( "RawAccel_Z",
                                 _AHRS->GetRawAccelZ() );
      SmartDashboard::PutNumber( "RawMag_X",
                                 _AHRS->GetRawMagX() );
      SmartDashboard::PutNumber( "RawMag_Y",
                                 _AHRS->GetRawMagY() );
      SmartDashboard::PutNumber( "RawMag_Z",
                                 _AHRS->GetRawMagZ() );
      SmartDashboard::PutNumber( "IMU_Temp_C",
                                 _AHRS->GetTempC() );
      /* Omnimount Yaw Axis Information                                           */
      /* For more info, see http://navx-mxp.kauailabs.com/installation/omnimount  */
      AHRS::BoardYawAxis yaw_axis = _AHRS->GetBoardYawAxis();
      SmartDashboard::PutString( "YawAxisDirection",
                                 yaw_axis.up ? "Up" : "Down" );
      SmartDashboard::PutNumber( "YawAxis",
                                 yaw_axis.board_axis );

      /* Quaternion Data                                                          */
      /* Quaternions are fascinating, and are the most compact representation of  */
      /* orientation data.  All of the Yaw, Pitch and Roll Values can be derived  */
      /* from the Quaternions.  If interested in motion processing, knowledge of  */
      /* Quaternions is highly recommended.                                       */
      SmartDashboard::PutNumber( "QuaternionW",
                                 _AHRS->GetQuaternionW() );
      SmartDashboard::PutNumber( "QuaternionX",
                                 _AHRS->GetQuaternionX() );
      SmartDashboard::PutNumber( "QuaternionY",
                                 _AHRS->GetQuaternionY() );
      SmartDashboard::PutNumber( "QuaternionZ",
                                 _AHRS->GetQuaternionZ() );
   }
}

/*
 *
 * Gyro Functions
 *
 */
void Team271NavX::GyroCal( void )
{
   if( _AHRS != nullptr )
   {

   }
}

void Team271NavX::GyroInit( void )
{
   if( _AHRS != nullptr )
   {

   }
}

void Team271NavX::GyroReset( void )
{
   if( _AHRS != nullptr )
   {
      _AHRS->Reset();

      usleep( 1000 );
   }
}

#endif

#endif
