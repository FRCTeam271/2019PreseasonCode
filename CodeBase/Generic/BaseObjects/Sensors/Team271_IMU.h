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

#ifndef TEAM271_IMU_H_
#define TEAM271_IMU_H_

#if defined(T271_IMU_ENABLE)

typedef enum: uint8_t
{
   TEAM271_IMU_STATUS_NONE = 0,

   TEAM271_IMU_STATUS_CONNECTED = ( 1 << 0 ),

   TEAM271_IMU_STATUS_INITIALIZED = ( 1 << 1 ),

   TEAM271_IMU_STATUS_COUNT
}TEAM271_IMU_STATUS;

typedef enum : uint8_t
{
   TEAM271_IMU_STATE_RESET = 0,

   TEAM271_IMU_STATE_INITIALIZE,

   TEAM271_IMU_STATE_CALIBRATE,

   TEAM271_IMU_STATE_NORMAL,

   TEAM271_IMU_STATE_COUNT
}TEAM271_IMU_STATE;

/*
 *
 * Shared Network Data
 *
 */
class t271_sensor_imu_data_index : public t271_sensor_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_SENSOR_IMU_DATA_STATE = t271_sensor_data_index::T271_DATA_COUNT,

      T271_SENSOR_IMU_DATA_QW,
      T271_SENSOR_IMU_DATA_QX,
      T271_SENSOR_IMU_DATA_QY,
      T271_SENSOR_IMU_DATA_QZ,

      T271_SENSOR_IMU_DATA_DECLINATION,
      T271_SENSOR_IMU_DATA_YAW_ZERO_OFFSET,


      T271_DATA_COUNT,
   } t271_sensor_imu_data_index_t;
};

class Team271IMU : public Team271Sensor
{
protected:
   /*
    *
    * Constants
    *
    */
   static constexpr float kIMUUpdatePeriod = 0.002;
   static constexpr int64_t kIMUUpdatePeriodThread = (int64_t)(kIMUUpdatePeriod * 1000.0);

   static constexpr uint8_t MAG_RATE_HZ = 100;

   static constexpr uint16_t ACCEL_RATE_HZ = 100;

   static constexpr uint16_t GYRO_RATE_HZ = 100;

   static constexpr uint8_t BARO_RATE_HZ = 50;

   static constexpr uint8_t Q_RATE_DIVISOR = 3;

   /*
    *
    * Variables
    *
    */
   std::atomic<uint8_t> ThreadIMU_Run_ = { 1U };
   std::thread* ThreadIMU_ = nullptr;
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   sched_param ThreadIMU_SCHParams_;
#endif

#if defined( T271_I2C_ENABLE )
   Team271I2C* I2C_ = nullptr;
#endif

#if defined( T271_SPI_ENABLE )
   Team271SPI* SPI_ = nullptr;
#endif

   float UpdateTimeStart_ = 0;

   Team271Sensor* Gyro_ = nullptr;

   Team271Sensor* Accel_ = nullptr;

public:
   /*
    *
    * Constructor/Destructor
    *
    */
#if defined( T271_I2C_ENABLE )
   Team271IMU( Team271Base* const argParent,
               const string& argName,
               const uint8_t argMessageCount,
               TEAM271_I2C_PORT argI2CPort,
               uint32_t argI2CAddress );
#endif

#if defined( T271_SPI_ENABLE )
   Team271IMU( const string& argName,
               TEAM271_SPI_PORT argSPIPort );
#endif

   virtual ~Team271IMU( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   float GetQW( void ) const;
   float GetQX( void ) const;
   float GetQY( void ) const;
   float GetQZ( void ) const;

   float GetPitch( void ) const;
   float GetRoll( void ) const;
   float GetYaw( void ) const;

   Team271Sensor* GetAccel( void ) const;
   float GetAccelX( void ) const;
   float GetAccelY( void ) const;
   float GetAccelZ( void ) const;

   Team271Sensor* GetGyro( void ) const;
   float GetGyroX( void ) const;
   float GetGyroY( void ) const;
   float GetGyroZ( void ) const;

   /*
    *
    * Setters
    *
    */
   void SetQW( const float argValue );
   void SetQX( const float argValue );
   void SetQY( const float argValue );
   void SetQZ( const float argValue );

   void SetRoll( const float argValue );
   void SetPitch( const float argValue );
   void SetYaw( const float argValue );

   void SetAccelX( const float argValue );
   void SetAccelY( const float argValue );
   void SetAccelZ( const float argValue );

   void SetGyroX( const float argValue );
   void SetGyroY( const float argValue );
   void SetGyroZ( const float argValue );

   /*
    *
    * Base
    *
    */
   void Init( void ) override;

   /*
    *
    * Robot Init
    *
    */
   void RobotInit( const bool argIsParent = false ) override;

   /*
    *
    * IMU Functions
    *
    */
   virtual void UpdateIMU( void );

   /*
    *
    * Gyro Functions
    *
    */
   virtual void GyroReset( void );
   virtual void GyroCal( void );
   virtual void GyroInit( void );
};

#endif

#endif /* TEAM271_GYRO_H_ */
