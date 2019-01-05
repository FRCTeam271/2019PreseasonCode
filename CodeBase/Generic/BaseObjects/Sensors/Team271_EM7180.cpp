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

#if defined(T271_EM7180_ENABLE)

Team271EM7180::Team271EM7180( Team271Base* const argParent )
      : Team271IMU( argParent,
                    "EM7180",
                    t271_sensor_em7180_data_index::T271_DATA_COUNT,
                    TEAM271_I2C_PORT_MXP,
                    ADDRESS )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   Write( t271_sensor_em7180_data_index::T271_SENSOR_EM7180_DATA_STATE,
          TEAM271_IMU_EM7180_STATE_RESET );

   /*
    * Create Thread to Run the IMU State Machine
    */
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   ThreadIMU_ = new std::thread( std::bind( &Team271EM7180::UpdateIMU,
                                            this ) );
#endif

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271EM7180::~Team271EM7180( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271EM7180::GetClassName( void ) const
{
   return "Team271EM7180";
}

/*
 *
 * Robot
 *
 */
void Team271EM7180::PreRobotPeriodic( const bool argIsParent )
{
   Write( t271_sensor_data_index::T271_SENSOR_DATA_CONNECTED,
          ThreadSensorConnected_.load() );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_INITIALIZED,
          ThreadSensorInitialized_.load() );

   Write( t271_sensor_data_index::T271_SENSOR_DATA_ERROR,
          ThreadSensorError_.load() );

   ThreadDeclination_.store( Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_DECLINATION ) );

   ThreadYawZeroOffset_.store( Read<float>( t271_sensor_imu_data_index::T271_SENSOR_IMU_DATA_YAW_ZERO_OFFSET ) );

   /*
    * Base Call
    */
   Team271IMU::PreRobotPeriodic();
}

/*
 *
 * Sensor Functions
 *
 */
void Team271EM7180::UpdateRates( const float argConversion )
{
   /*
    * Grab the latest values from the IMU Thread
    */
   Write( t271_sensor_em7180_data_index::T271_SENSOR_EM7180_DATA_STATE,
          EM7180State_.load() );

   SetQW( ThreadQW_ );
   SetQX( ThreadQX_ );
   SetQY( ThreadQY_ );
   SetQZ( ThreadQZ_ );

   SetRoll( ThreadRoll_ );
   SetPitch( ThreadPitch_ );
   SetYaw( ThreadYaw_ );

   SetAccelX( ThreadAX_ );
   SetAccelY( ThreadAY_ );
   SetAccelZ( ThreadAZ_ );

   SetGyroX( ThreadGX_ );
   SetGyroY( ThreadGY_ );
   SetGyroZ( ThreadGZ_ );
}

/*
 *
 * IMU Functions
 *
 */
void Team271EM7180::UpdateIMU( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   uint8_t tmpSentralStatus = 0;

   if( pthread_setname_np( pthread_self(),
                           "T271UpdateIMU" ) )
   {
      std::cerr << "Failed to set IMU Thread Name : " << std::strerror( errno ) << std::endl;
   }
/*
   ThreadIMU_SCHParams_.sched_priority = 30;
   if( pthread_setschedparam( pthread_self(),
                              SCHED_FIFO,
                              &ThreadIMU_SCHParams_ ) )*/
   //ThreadIMU_SCHParams_.sched_priority = 30;
   if( pthread_setschedparam( pthread_self(),
                              SCHED_IDLE,
                              &ThreadIMU_SCHParams_ ) )
   {
      std::cerr << "Failed to set IMU Thread scheduling : " << std::strerror( errno ) << std::endl;
   }

   while( ThreadIMU_Run_.load() )
   {
      /*
       *
       * Update IMU
       *
       */

      /*
       *
       * TEAM271_IMU_STATE_RESET
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_RESET )
      {
         /*
          * Request Reset
          */
         writeRegister( ResetRequest,
                        0x01 );

         /*
          * Store the Start Time
          */
         UpdateTimeStart_ = Team271Robot::GetTime();

         /*
          * Go to the next State
          */
         EM7180State_.store( TEAM271_IMU_EM7180_STATE_DETECT_EEPROM );
      }

      /*
       *
       * TEAM271_IMU_STATE_DETECT_EEPROM
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_DETECT_EEPROM )
      {
         /*
          * Check Loop Rate for EEPROM Reset and Detect
          */
         if( Team271Robot::CompareTime( UpdateTimeStart_,
                                        1.000f ) == 1 )
         {
            /*
             * Check if we've detected the EEPROM
             */
            tmpSentralStatus = getSentralStatus();

            if( ( tmpSentralStatus & TEAM271_IMU_STATUS_EEPROM_DETECTED ) != 0 )
            {
               /*
                * EEPROM is Detected
                *
                * This means that we have a good Connection to the Sensor
                */
               ThreadSensorConnected_.store( 1 );
               ThreadSensorInitialized_.store( 0 );
               ThreadSensorError_.store( 0 );

               errorStatus = 0;

               /*
                * Check if the EEPROM Upload is Done
                */
               if( ( tmpSentralStatus & TEAM271_IMU_STATUS_EEPROM_UPLOAD_DONE ) != 0 )
               {
                  /*
                   * EEPROM Upload is Done, was there an Error?
                   */
                  if( ( tmpSentralStatus & TEAM271_IMU_STATUS_EEPROM_UPLOAD_ERROR ) != 0 )
                  {
                     ThreadSensorError_.store( 1 );

                     errorStatus = 0xA0;

                     LOG_ERROR( "%s - CheckStatus - EEPROM CRC incorrect!",
                                GetName().c_str() );

                     /*
                      * Try to Reset the IMU
                      */
                     EM7180State_.store( TEAM271_IMU_EM7180_STATE_RESET );
                  }

                  /*
                   * EEPROM Upload Successful
                   */
                  else
                  {
                     /*
                      * Set SENtral in initialized state to configure registers
                      */
                     setRunDisable();

                     /*
                      * Go to the next State
                      */
                     EM7180State_.store( TEAM271_IMU_EM7180_STATE_INITIALIZE_REGISTERS );
                  }
               }
            }
            else if( ( tmpSentralStatus & TEAM271_IMU_STATUS_EEPROM_NO_EEPROM ) != 0 )
            {
               ThreadSensorConnected_.store( 1 );
               ThreadSensorInitialized_.store( 0 );
               ThreadSensorError_.store( 1 );

               errorStatus = 0xA0;

               LOG_ERROR( "%s - CheckStatus - No EEPROM detected!",
                          GetName().c_str() );

               /*
                * Try to Reset the IMU
                */
               EM7180State_.store( TEAM271_IMU_EM7180_STATE_RESET );
            }
            else
            {
               ThreadSensorConnected_.store( 0 );
               ThreadSensorInitialized_.store( 0 );
               ThreadSensorError_.store( 0 );

               errorStatus = 0;
            }
         }
      }

      /*
       *
       * TEAM271_IMU_STATE_CANCEL_PASSTHROUGH
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_CANCEL_PASSTHROUGH )
      {
         /*
          * Cancel pass-through mode
          */
         writeRegister( PassThruControl,
                        0x00 );

         /*
          * Store the Start Time
          */
         UpdateTimeStart_ = Team271Robot::GetTime();

         /*
          * Go to the next State
          */
         EM7180State_.store( TEAM271_IMU_EM7180_STATE_CANCEL_PASSTHROUGH_WAIT );
      }

      /*
       *
       * TEAM271_IMU_STATE_CANCEL_PASSTHROUGH_WAIT
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_CANCEL_PASSTHROUGH_WAIT )
      {
         if( Team271Robot::CompareTime( UpdateTimeStart_,
                                        0.005f ) == 1 )
         {
            /*
             * Check the Status
             */
            if( ( readRegister( PassThruStatus ) & 0x01 ) != 0 )
            {
               /*
                * Go to the next State
                */
               EM7180State_.store( TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM );
            }
         }
      }

      /*
       *
       * TEAM271_IMU_STATE_RESTART_ALGORITHM
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM )
      {
         /*
          * Re-start algorithm
          */
         writeRegister( AlgorithmControl,
                        0x00 );

         /*
          * Store the Start Time
          */
         UpdateTimeStart_ = Team271Robot::GetTime();

         /*
          * Go to the next State
          */
         EM7180State_.store( TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM_WAIT );
      }

      /*
       *
       * TEAM271_IMU_STATE_RESTART_ALGORITHM_WAIT
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_RESTART_ALGORITHM_WAIT )
      {
         if( Team271Robot::CompareTime( UpdateTimeStart_,
                                        0.005f ) == 1 )
         {
            /*
             * Store the Start Time
             */
            UpdateTimeStart_ = Team271Robot::GetTime();

            /*
             * Check the Status
             */
            if( ( readRegister( AlgorithmStatus ) & 0x01 ) != 0 )
            {
               /*
                * Go to the next State
                */
               EM7180State_.store( TEAM271_IMU_EM7180_STATE_INITIALIZE_REGISTERS );
            }
         }
      }

      /*
       *
       * TEAM271_IMU_STATE_INITIALIZE_REGISTERS
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_INITIALIZE_REGISTERS )
      {
         // Setup LPF bandwidth (BEFORE setting ODR's)
         setAccelLpfBandwidth( 0x03 ); // 41Hz
         setGyroLpfBandwidth( 0x03 );  // 41Hz

         // Set accel/gyro/mage desired ODR rates
         setQRateDivisor( Q_RATE_DIVISOR - 1 );
         setMagRate( MAG_RATE_HZ );
         setAccelRate( ACCEL_RATE_HZ / 10 );
         setGyroRate( GYRO_RATE_HZ / 10 );
         setBaroRate( 0x80 | BARO_RATE_HZ ); // 0x80 = enable bit

         // Configure operating modeA
         algorithmControlReset(); // read scale sensor data

         // Enable interrupt to host upon certain events:
         // quaternions updated (0x04), an error occurs (0x02), or the SENtral needs to be reset(0x01)
         enableEvents( 0x07 );

         // Enable EM7180 run mode
         setRunEnable();    // set SENtral in normal run mode

         /*
          * Store the Start Time
          */
         UpdateTimeStart_ = Team271Robot::GetTime();

         /*
          * Go to the next State
          */
         EM7180State_.store( TEAM271_IMU_EM7180_STATE_DISABLE_STILLNESS );
      }

      /*
       *
       * TEAM271_IMU_STATE_DISABLE_STILLNESS
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_DISABLE_STILLNESS )
      {
         /*
          * Check Loop Rate for Stillness Mode Disable
          */
         if( Team271Robot::CompareTime( UpdateTimeStart_,
                                        0.100f ) == 1 )
         {
            /*
             * Disable stillness mode
             */
            setIntegerParam( 0x49,
                             0x00 );

            ThreadSensorInitialized_.store( 1 );

            /*
             * Go to Normal Operation
             */
            EM7180State_.store( TEAM271_IMU_EM7180_STATE_NORMAL );
         }
      }

      /*
       *
       * TEAM271_IMU_STATE_NORMAL
       *
       */
      if( EM7180State_.load() == TEAM271_IMU_EM7180_STATE_NORMAL )
      {
         /*
          * Check Event Status
          *
          * Way to check data ready by checkEventStatusing rather than interrupt
          * Reading clears the register
          */
         _eventStatus = getEventStatus();

         /*
          * Check for Error
          */
         if( gotError() == true )
         {
            LOG_ERROR( "%s - UpdateIMU - Error: %s",
                       GetName().c_str(),
                       getErrorString() );

            ThreadSensorConnected_.store( 0 );
            ThreadSensorInitialized_.store( 0 );
            ThreadSensorError_.store( 1 );

            EM7180State_.store( TEAM271_IMU_EM7180_STATE_RESET );
         }

         /*
          * No Error
          */
         else
         {
            /*
             * Define output variables from updated quaternion---these are Tait-Bryan
             * angles, commonly used in aircraft orientation.  In this coordinate
             * system, the positive z-axis is down toward Earth.  Yaw is the angle
             * between Sensor x-axis and Earth magnetic North (or true North if
             * corrected for local declination, looking down on the sensor positive
             * yaw is counterclockwise.  Pitch is angle between sensor x-axis and
             * Earth ground plane, toward the Earth is positive, up toward the sky is
             * negative.  Roll is angle between sensor y-axis and Earth ground plane,
             * y-axis up is positive roll.  These arise from the definition of the
             * homogeneous rotation matrix constructed from q.  Tait-Bryan
             * angles as well as Euler angles are non-commutative; that is, the get
             * the correct orientation the rotations must be applied in the correct
             * order which for this configuration is yaw, pitch, and then roll.  For
             * more see http://en.wikipedia.org/wiki/Conversion_between_q_and_Euler_angles
             * which has additional links.
             */
            if( gotQuaternion() == true )
            {
               float tmpQW = 0;
               float tmpQX = 0;
               float tmpQY = 0;
               float tmpQZ = 0;

               Team271EM7180::readQuaternion( tmpQW,
                                              tmpQX,
                                              tmpQY,
                                              tmpQZ );

               ThreadQW_ = tmpQW;
               ThreadQX_ = tmpQX;
               ThreadQY_ = tmpQY;
               ThreadQZ_ = tmpQZ;

               /*
                *
                * Yaw
                *
                */
               float tmpYaw = 0;

               tmpYaw = atan2( 2.0f * ( ThreadQX_ * ThreadQY_ + ThreadQW_ * ThreadQZ_ ),
                               ThreadQW_ * ThreadQW_ + ThreadQX_ * ThreadQX_ - ThreadQY_ * ThreadQY_ - ThreadQZ_ * ThreadQZ_ );

               tmpYaw *= 180.0f / M_PI;

               tmpYaw += ThreadDeclination_.load();

               /*
                * Ensure Yaw stays between 0 and 360
                */
               if( tmpYaw < 0 )
               {
                  tmpYaw += 360.0f;
               }

               tmpYaw -= ThreadYawZeroOffset_.load();


               ThreadYaw_.store( tmpYaw );

               /*
                *
                * Pitch
                *
                */
               float tmpPitch = 0;

               tmpPitch = -asin( 2.0f * ( ThreadQX_ * ThreadQZ_ - ThreadQW_ * ThreadQY_ ) );

               tmpPitch *= 180.0f / M_PI;

               ThreadPitch_.store( tmpPitch );

               /*
                *
                * Roll
                *
                */
               float tmpRoll = 0;

               tmpRoll = atan2( 2.0f * ( ThreadQW_ * ThreadQX_ + ThreadQY_ * ThreadQZ_ ),
                                ThreadQW_ * ThreadQW_ - ThreadQX_ * ThreadQX_ - ThreadQY_ * ThreadQY_ + ThreadQZ_ * ThreadQZ_ );

               tmpRoll *= 180.0f / M_PI;

               ThreadRoll_.store( tmpRoll );
            }

            double tmpX = 0;
            double tmpY = 0;
            double tmpZ = 0;

            if( gotAccelerometer() == true )
            {
               readAccelerometer( tmpX,
                                  tmpY,
                                  tmpZ );

               ThreadAX_.store( tmpX );
               ThreadAY_.store( tmpY );
               ThreadAZ_.store( tmpZ );
            }

            if( gotGyrometer() == true )
            {
               readGyrometer( tmpX,
                              tmpY,
                              tmpZ );

               ThreadGX_.store( tmpX );
               ThreadGY_.store( tmpY );
               ThreadGZ_.store( tmpZ );
            }
         }
      }

      /*
       *
       * Sleep
       *
       */
      std::this_thread::sleep_for( std::chrono::milliseconds( (int64_t)( kIMUUpdatePeriodThread ) ) );
   }
#endif
}

/*
 *
 * EM7180 Functions
 *
 */
bool Team271EM7180::gotError( void ) const
{
   bool tmpReturn = false;

   if( _eventStatus & 0x02 )
   {
      tmpReturn = true;
   }

   return tmpReturn;
}

bool Team271EM7180::gotQuaternion( void ) const
{
   return _eventStatus & 0x04;
}

bool Team271EM7180::gotMagnetometer( void ) const
{
   return _eventStatus & 0x08;
}

bool Team271EM7180::gotAccelerometer( void ) const
{
   return _eventStatus & 0x10;
}

bool Team271EM7180::gotGyrometer( void ) const
{
   return _eventStatus & 0x20;
}

bool Team271EM7180::gotBarometer( void ) const
{
   return _eventStatus & 0x40;
}

void Team271EM7180::readThreeAxis( uint8_t regx,
                                   double& x,
                                   double& y,
                                   double& z,
                                   double scale )
{
   int16_t xx = 0;
   int16_t yy = 0;
   int16_t zz = 0;

   readThreeAxis( regx,
                  xx,
                  yy,
                  zz );

   x = xx * scale;
   y = yy * scale;
   z = zz * scale;
}

void Team271EM7180::readAccelerometer( double& ax,
                                       double& ay,
                                       double& az )
{
   readThreeAxis( Team271EM7180::AX,
                  ax,
                  ay,
                  az,
                  0.000488 );
}

void Team271EM7180::readGyrometer( double& gx,
                                   double& gy,
                                   double& gz )
{
   readThreeAxis( Team271EM7180::GX,
                  gx,
                  gy,
                  gz,
                  0.153 );
}

void Team271EM7180::readMagnetometer( double& mx,
                                      double& my,
                                      double& mz )
{
   readThreeAxis( Team271EM7180::MX,
                  mx,
                  my,
                  mz,
                  0.305176 );
}

const char* Team271EM7180::getErrorString( void ) const
{
   if( errorStatus & 0x01 )
   {
      return "Magnetometer error";
   }
   if( errorStatus & 0x02 )
   {
      return "Accelerometer error";
   }
   if( errorStatus & 0x04 )
   {
      return "Gyro error";
   }
   if( errorStatus & 0x10 )
   {
      return "Magnetometer ID not recognized";
   }
   if( errorStatus & 0x20 )
   {
      return "Accelerometer ID not recognized";
   }
   if( errorStatus & 0x30 )
   {
      return "Math error";
   }
   if( errorStatus & 0x40 )
   {
      return "Gyro ID not recognized";
   }
   if( errorStatus & 0x80 )
   {
      return "Invalid sample rate";
   }

   // Ad-hoc
   if( errorStatus & 0x90 )
   {
      return "Failed to put SENtral in pass-through mode";
   }
   if( errorStatus & 0xA0 )
   {
      return "Unable to read from SENtral EEPROM";
   }
   if( errorStatus & 0xB0 )
   {
      return "Unable to upload config to SENtral EEPROM";
   }

   return "Unknown error";
}

uint8_t Team271EM7180::getProductId( void ) const
{
   return readRegister( ProductID );
}

uint8_t Team271EM7180::getRevisionId( void ) const
{
   return readRegister( RevisionID );
}

uint16_t Team271EM7180::getRamVersion( void ) const
{
   const uint16_t ram1 = readRegister( RAMVersion1 );
   const uint16_t ram2 = readRegister( RAMVersion2 );

   return ram1 << 8 | ram2;
}

uint16_t Team271EM7180::getRomVersion( void ) const
{
   const uint16_t rom1 = readRegister( ROMVersion1 );
   const uint16_t rom2 = readRegister( ROMVersion2 );

   return rom1 << 8 | rom2;
}

uint8_t Team271EM7180::getSentralStatus( void ) const
{
   return readRegister( SentralStatus );
}

void Team271EM7180::requestReset( void ) const
{
   writeRegister( ResetRequest,
                  0x01 );
}

void Team271EM7180::readThreeAxis( uint8_t xreg,
                                   int16_t& x,
                                   int16_t& y,
                                   int16_t& z )
{
   uint8_t rawData[6];  // x/y/z register data stored here
   readRegisters( xreg,
                  6,
                  &rawData[0] );  // Read the six raw data registers sequentially into data array
   x = (int16_t)( ( (int16_t)rawData[1] << 8 ) | rawData[0] );   // Turn the MSB and LSB into a signed 16-bit value
   y = (int16_t)( ( (int16_t)rawData[3] << 8 ) | rawData[2] );
   z = (int16_t)( ( (int16_t)rawData[5] << 8 ) | rawData[4] );
}

void Team271EM7180::setPassThroughMode( void ) const
{
#if 0
   // First put SENtral in standby mode
   writeRegister(AlgorithmControl, 0x01);
   delay(5);

   // Place SENtral in pass-through mode
   writeRegister(PassThruControl, 0x01);
   while (true)
   {
      if (readRegister(PassThruStatus) & 0x01) break;
      delay(5);
   }
#endif
}

bool Team271EM7180::hasFeature( uint8_t features ) const
{
   return features & readRegister( FeatureFlags );
}

void Team271EM7180::setMasterMode( void ) const
{
#if 0
   // Cancel pass-through mode
   writeRegister(PassThruControl, 0x00);
   while (true)
   {
      if (!(readRegister(PassThruStatus) & 0x01)) break;
      delay(5);
   }

   // Re-start algorithm
   writeRegister(AlgorithmControl, 0x00);
   while (true)
   {
      if (!(readRegister(AlgorithmStatus) & 0x01)) break;
      delay(5);
   }
#endif
}

void Team271EM7180::setRunEnable( void ) const
{
   writeRegister( HostControl,
                  0x01 );
}

void Team271EM7180::setRunDisable( void ) const
{
   writeRegister( HostControl,
                  0x00 );
}

void Team271EM7180::setAccelLpfBandwidth( uint8_t bw ) const
{
   writeRegister( ACC_LPF_BW,
                  bw );
}

void Team271EM7180::setGyroLpfBandwidth( uint8_t bw ) const
{
   writeRegister( GYRO_LPF_BW,
                  bw );
}

void Team271EM7180::setQRateDivisor( uint8_t divisor ) const
{
   writeRegister( QRateDivisor,
                  divisor );
}

void Team271EM7180::setMagRate( uint8_t rate ) const
{
   writeRegister( MagRate,
                  rate );
}

void Team271EM7180::setAccelRate( uint8_t rate ) const
{
   writeRegister( AccelRate,
                  rate );
}

void Team271EM7180::setGyroRate( uint8_t rate ) const
{
   writeRegister( GyroRate,
                  rate );
}

void Team271EM7180::setBaroRate( uint8_t rate ) const
{
   writeRegister( BaroRate,
                  rate );
}

void Team271EM7180::algorithmControlRequestParameterTransfer( void ) const
{
   writeRegister( AlgorithmControl,
                  0x80 );
}

void Team271EM7180::algorithmControlReset( void ) const
{
   writeRegister( AlgorithmControl,
                  0x00 );
}

void Team271EM7180::enableEvents( uint8_t mask ) const
{
   writeRegister( EnableEvents,
                  mask );
}

void Team271EM7180::requestParamRead( uint8_t param ) const
{
   writeRegister( ParamRequest,
                  param );
}

uint8_t Team271EM7180::getParamAcknowledge( void ) const
{
   return readRegister( ParamAcknowledge );
}

uint8_t Team271EM7180::readSavedParamByte0( void ) const
{
   return readRegister( SavedParamByte0 );
}

uint8_t Team271EM7180::readSavedParamByte1( void ) const
{
   return readRegister( SavedParamByte1 );
}

uint8_t Team271EM7180::readSavedParamByte2( void ) const
{
   return readRegister( SavedParamByte2 );
}

uint8_t Team271EM7180::readSavedParamByte3( void ) const
{
   return readRegister( SavedParamByte3 );
}

uint8_t Team271EM7180::getRunStatus( void ) const
{
   return readRegister( RunStatus );
}

uint8_t Team271EM7180::getAlgorithmStatus( void ) const
{
   return readRegister( AlgorithmStatus );
}

uint8_t Team271EM7180::getPassThruStatus( void ) const
{
   return readRegister( PassThruStatus );
}

uint8_t Team271EM7180::getEventStatus( void ) const
{
   return readRegister( EventStatus );
}

uint8_t Team271EM7180::getSensorStatus( void ) const
{
   return readRegister( SensorStatus );
}

uint8_t Team271EM7180::getErrorStatus( void ) const
{
   return readRegister( ErrorRegister );
}

void Team271EM7180::setGyroFs( uint16_t gyro_fs ) const
{
   uint8_t bytes[4], STAT;
   bytes[0] = gyro_fs & ( 0xFF );
   bytes[1] = ( gyro_fs >> 8 ) & ( 0xFF );
   bytes[2] = 0x00;
   bytes[3] = 0x00;
   writeRegister( LoadParamByte0,
                  bytes[0] ); //Gyro LSB
   writeRegister( LoadParamByte1,
                  bytes[1] ); //Gyro MSB
   writeRegister( LoadParamByte2,
                  bytes[2] ); //Unused
   writeRegister( LoadParamByte3,
                  bytes[3] ); //Unused
   writeRegister( ParamRequest,
                  0xCB ); //Parameter 75; 0xCB is 75 decimal with the MSB set high to indicate a paramter write processs
   writeRegister( AlgorithmControl,
                  0x80 ); //Request parameter transfer procedure
   STAT = readRegister( ParamAcknowledge ); //Check the parameter acknowledge register and loop until the result matches parameter request byte
   while( !( STAT == 0xCB ) )
   {
      STAT = readRegister( ParamAcknowledge );
   }
   writeRegister( ParamRequest,
                  0x00 ); //Parameter request = 0 to end parameter transfer process
   writeRegister( AlgorithmControl,
                  0x00 ); // Re-start algorithm
}

void Team271EM7180::setMagAccFs( uint16_t mag_fs,
                                 uint16_t acc_fs ) const
{
   uint8_t bytes[4], STAT;
   bytes[0] = mag_fs & ( 0xFF );
   bytes[1] = ( mag_fs >> 8 ) & ( 0xFF );
   bytes[2] = acc_fs & ( 0xFF );
   bytes[3] = ( acc_fs >> 8 ) & ( 0xFF );
   writeRegister( LoadParamByte0,
                  bytes[0] ); //Mag LSB
   writeRegister( LoadParamByte1,
                  bytes[1] ); //Mag MSB
   writeRegister( LoadParamByte2,
                  bytes[2] ); //Acc LSB
   writeRegister( LoadParamByte3,
                  bytes[3] ); //Acc MSB
   writeRegister( ParamRequest,
                  0xCA ); //Parameter 74; 0xCA is 74 decimal with the MSB set high to indicate a paramter write processs
   writeRegister( AlgorithmControl,
                  0x80 ); //Request parameter transfer procedure
   STAT = readRegister( ParamAcknowledge ); //Check the parameter acknowledge register and loop until the result matches parameter request byte
   while( !( STAT == 0xCA ) )
   {
      STAT = readRegister( ParamAcknowledge );
   }
   writeRegister( ParamRequest,
                  0x00 ); //Parameter request = 0 to end parameter transfer process
   writeRegister( AlgorithmControl,
                  0x00 ); // Re-start algorithm
}

void Team271EM7180::loadParamByte0( uint8_t value ) const
{
   writeRegister( LoadParamByte0,
                  value );
}

void Team271EM7180::loadParamByte1( uint8_t value ) const
{
   writeRegister( LoadParamByte1,
                  value );
}

void Team271EM7180::loadParamByte2( uint8_t value ) const
{
   writeRegister( LoadParamByte2,
                  value );
}

void Team271EM7180::loadParamByte3( uint8_t value ) const
{
   writeRegister( LoadParamByte3,
                  value );
}

void Team271EM7180::writeGp36( uint8_t value ) const
{
   writeRegister( GP36,
                  value );
}

void Team271EM7180::writeGp37( uint8_t value ) const
{
   writeRegister( GP37,
                  value );
}

void Team271EM7180::writeGp38( uint8_t value ) const
{
   writeRegister( GP38,
                  value );
}

void Team271EM7180::writeGp39( uint8_t value ) const
{
   writeRegister( GP39,
                  value );
}

void Team271EM7180::writeGp40( uint8_t value ) const
{
   writeRegister( GP40,
                  value );
}

void Team271EM7180::writeGp50( uint8_t value ) const
{
   writeRegister( GP50,
                  value );
}

void Team271EM7180::writeGp51( uint8_t value ) const
{
   writeRegister( GP51,
                  value );
}

void Team271EM7180::writeGp52( uint8_t value ) const
{
   writeRegister( GP52,
                  value );
}

void Team271EM7180::writeGp53( uint8_t value ) const
{
   writeRegister( GP53,
                  value );
}

void Team271EM7180::writeGp54( uint8_t value ) const
{
   writeRegister( GP54,
                  value );
}

void Team271EM7180::writeGp55( uint8_t value ) const
{
   writeRegister( GP55,
                  value );
}

void Team271EM7180::writeGp56( uint8_t value ) const
{
   writeRegister( GP56,
                  value );
}

void Team271EM7180::readAccelerometer( int16_t& ax,
                                       int16_t& ay,
                                       int16_t& az )
{
   readThreeAxis( AX,
                  ax,
                  ay,
                  az );
}

void Team271EM7180::readGyrometer( int16_t& gx,
                                   int16_t& gy,
                                   int16_t& gz )
{
   readThreeAxis( GX,
                  gx,
                  gy,
                  gz );
}

void Team271EM7180::readBarometer( float& pressure,
                                   float& temperature )
{
   uint8_t rawData[2];  // x/y/z gyro register data stored here

   readRegisters( Baro,
                  2,
                  &rawData[0] );  // Read the two raw data registers sequentially into data array
   int16_t rawPressure = (int16_t)( ( (int16_t)rawData[1] << 8 ) | rawData[0] );   // Turn the MSB and LSB into a signed 16-bit value
   pressure = (float)rawPressure * .01f + 1013.25f; // pressure in millibars

   // get BMP280 temperature
   readRegisters( Temp,
                  2,
                  &rawData[0] );  // Read the two raw data registers sequentially into data array
   int16_t rawTemperature = (int16_t)( ( (int16_t)rawData[1] << 8 ) | rawData[0] );   // Turn the MSB and LSB into a signed 16-bit value

   temperature = (float)rawTemperature * 0.01;  // temperature in degrees C
}

void Team271EM7180::readMagnetometer( int16_t& mx,
                                      int16_t& my,
                                      int16_t& mz )
{
   readThreeAxis( MX,
                  mx,
                  my,
                  mz );
}

void Team271EM7180::readQuaternion( float& qw,
                                    float& qx,
                                    float& qy,
                                    float&qz )
{
   uint8_t rawData[16];  // x/y/z/w quaternion register data stored here (note unusual order!)

   readRegisters( QX,
                  16,
                  &rawData[0] );

   qx = uint32_reg_to_float( &rawData[0] );
   qy = uint32_reg_to_float( &rawData[4] );
   qz = uint32_reg_to_float( &rawData[8] );
   qw = uint32_reg_to_float( &rawData[12] );
}

void Team271EM7180::setIntegerParam( uint8_t param,
                                     uint32_t param_val ) const
{
   uint8_t bytes[4], STAT;
   bytes[0] = param_val & ( 0xFF );
   bytes[1] = ( param_val >> 8 ) & ( 0xFF );
   bytes[2] = ( param_val >> 16 ) & ( 0xFF );
   bytes[3] = ( param_val >> 24 ) & ( 0xFF );
   param = param | 0x80; //Parameter is the decimal value with the MSB set high to indicate a paramter write processs
   writeRegister( LoadParamByte0,
                  bytes[0] ); //Param LSB
   writeRegister( LoadParamByte1,
                  bytes[1] );
   writeRegister( LoadParamByte2,
                  bytes[2] );
   writeRegister( LoadParamByte3,
                  bytes[3] ); //Param MSB
   writeRegister( ParamRequest,
                  param );
   writeRegister( AlgorithmControl,
                  0x80 ); //Request parameter transfer procedure
   STAT = readRegister( ParamAcknowledge ); //Check the parameter acknowledge register and loop until the result matches parameter request byte
   while( !( STAT == param ) )
   {
      STAT = readRegister( ParamAcknowledge );
   }
   writeRegister( ParamRequest,
                  0x00 ); //Parameter request = 0 to end parameter transfer process
   writeRegister( AlgorithmControl,
                  0x00 ); // Re-start algorithm
}

void Team271EM7180::getFullScaleRanges( uint8_t& accFs,
                                        uint16_t& gyroFs,
                                        uint16_t& magFs )
{
   uint8_t param[4];

   // Read sensor new FS values from parameter space
   writeRegister( ParamRequest,
                  0x4A ); // Request to read  parameter 74
   writeRegister( AlgorithmControl,
                  0x80 ); // Request parameter transfer process
   uint8_t param_xfer = readRegister( ParamAcknowledge );
   while( !( param_xfer == 0x4A ) )
   {
      param_xfer = readRegister( ParamAcknowledge );
   }
   param[0] = readRegister( SavedParamByte0 );
   param[1] = readRegister( SavedParamByte1 );
   param[2] = readRegister( SavedParamByte2 );
   param[3] = readRegister( SavedParamByte3 );
   magFs = ( (uint16_t)( param[1] << 8 ) | param[0] );
   accFs = ( (uint8_t)( param[3] << 8 ) | param[2] );
   writeRegister( ParamRequest,
                  0x4B ); // Request to read  parameter 75
   param_xfer = readRegister( ParamAcknowledge );
   while( !( param_xfer == 0x4B ) )
   {
      param_xfer = readRegister( ParamAcknowledge );
   }
   param[0] = readRegister( SavedParamByte0 );
   param[1] = readRegister( SavedParamByte1 );
   param[2] = readRegister( SavedParamByte2 );
   param[3] = readRegister( SavedParamByte3 );
   gyroFs = ( (uint16_t)( param[1] << 8 ) | param[0] );
   writeRegister( ParamRequest,
                  0x00 ); //End parameter transfer
   writeRegister( AlgorithmControl,
                  0x00 ); // re-enable algorithm
}

uint8_t Team271EM7180::getActualMagRate( void ) const
{
   return readRegister( ActualMagRate );
}

uint16_t Team271EM7180::getActualAccelRate( void ) const
{
   return readRegister( ActualAccelRate );
}

uint16_t Team271EM7180::getActualGyroRate( void ) const
{
   return readRegister( ActualGyroRate );
}

uint8_t Team271EM7180::getActualBaroRate( void ) const
{
   return readRegister( ActualBaroRate );
}

uint8_t Team271EM7180::getActualTempRate( void ) const
{
   return readRegister( ActualTempRate );
}

uint8_t Team271EM7180::readRegister( uint8_t subAddress ) const
{
   uint8_t data;
   readRegisters( subAddress,
                  1U,
                  &data );
   return data;
}

void Team271EM7180::writeRegister( uint8_t subAddress,
                                   uint8_t data ) const
{
   if( I2C_ != nullptr )
   {
      I2C_->Write( subAddress,
                   data );
   }
}

void Team271EM7180::readRegisters( uint8_t subAddress,
                                   uint8_t count,
                                   uint8_t* dest ) const
{
   if( I2C_ != nullptr )
   {
      I2C_->Read( subAddress,
                  dest,
                  count );
   }
}

float Team271EM7180::uint32_reg_to_float( uint8_t *buf )
{
   union
   {
      uint32_t ui32;
      float f;
   } u;

   u.ui32 = ( ( (uint32_t)buf[0] ) + ( ( (uint32_t)buf[1] ) << 8 ) + ( ( (uint32_t)buf[2] ) << 16 ) + ( ( (uint32_t)buf[3] ) << 24 ) );
   return u.f;
}

#endif
