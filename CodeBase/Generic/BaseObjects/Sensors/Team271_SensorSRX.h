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

#ifndef TEAM271_SENSOR_SRX_H_
#define TEAM271_SENSOR_SRX_H_

#if defined( T271_SENSOR_ENABLE )

typedef enum
{
   T271_Period_1Ms = 1,
   T271_Period_2Ms = 2,
   T271_Period_5Ms = 5,
   T271_Period_10Ms = 10,
   T271_Period_20Ms = 20,
   T271_Period_25Ms = 25,
   T271_Period_50Ms = 50,
   T271_Period_100Ms = 100,
} T271_VelocityMeasPeriod;

class t271_sensor_srx_data_index : public t271_sensor_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Sensor Configuration
       */
      T271_SENSOR_SRX_DATA_VEL_MEASURE_PERIOD = t271_sensor_data_index::T271_DATA_COUNT,

      T271_SENSOR_SRX_DATA_VEL_MEASURE_WINDOW,


      T271_DATA_COUNT,
   } t271_sensor_srx_data_index_t;
};

class Team271SensorSRX : public Team271Sensor
{
protected:
   /*
    *
    * Constants
    *
    */
   static constexpr double kSRXCountsPerRev = 4096.0;
   static constexpr float kSensorCheckRate = 0.050f;

   /*
    *
    * Variables
    *
    */
#if defined(T271_CTRE)
   TalonSRX* TalonSRX_ = nullptr;
   ctre::phoenix::motorcontrol::SensorCollection* SensorCollection_;
#endif

   float TimeSensorStatusLast_ = 0.0f;

public:
   /*
    * Constructor/Destructor
    */
   Team271SensorSRX( Team271Base* const argParent,
                     const string& argSensorName,
                     Team271Motor* const argMotor,
                     const TEAM271_SENSOR_TYPE argSensorType,
                     const TEAM271_INVERTED_STATE argSensorInverted,
                     const TEAM271_SENSOR_SETTINGS argSensorSettings,
                     const float argArgumentA );

   virtual ~Team271SensorSRX( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   /*
    *
    * Setters
    *
    */
   void SetPosition( const float argPosition ) override;

   void SetInverted( const TEAM271_INVERTED_STATE argInverted );

   void SetVelMeasPeriod( const T271_VelocityMeasPeriod argPeriod );

   void SetVelMeasWindow( const int32_t argWindow );

   void SetVelMeas( const T271_VelocityMeasPeriod argPeriod,
                    const uint32_t argWindow );

   /*
    *
    * Robot Init
    *
    */
   void PreRobotPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Disabled
    *
    */
   void DisabledInit( const bool argIsParent = false ) override;

   /*
    *
    * Sensor Functions
    *
    */
private:
   void UpdateSwitches( void ) override;
   void UpdatePositions( const float argConversion = 1.0 ) override;
   void UpdateRates( const float argConversion = 1.0 ) override;
};

#endif

#endif /* TEAM271_SENSOR_SRX_H_ */
