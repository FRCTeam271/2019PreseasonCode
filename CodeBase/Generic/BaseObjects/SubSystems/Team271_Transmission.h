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

#ifndef TEAM271_TRANSMISSION_H_
#define TEAM271_TRANSMISSION_H_

#if defined( T271_TRANSMISSION_ENABLE )

typedef enum : uint8_t
{
   TEAM271_SHIFTER_INDEX_1,
   TEAM271_SHIFTER_INDEX_2,
   TEAM271_SHIFTER_INDEX_3,

   TEAM271_SHIFTER_INDEX_COUNT
}TEAM271_SHIFTER_INDEX;

typedef enum : uint8_t
{
   TEAM271_MOTOR_INDEX_MASTER,

   TEAM271_MOTOR_INDEX_SLAVE1,
   TEAM271_MOTOR_INDEX_SLAVE2,
   TEAM271_MOTOR_INDEX_SLAVE3,
   TEAM271_MOTOR_INDEX_SLAVE4,
   TEAM271_MOTOR_INDEX_SLAVE5,

   TEAM271_MOTOR_INDEX_COUNT
}TEAM271_MOTOR_INDEX;

typedef enum : uint8_t
{
   TEAM271_SENSOR_INDEX_FWD_LIM,
   TEAM271_SENSOR_INDEX_REV_LIM,

   TEAM271_SENSOR_INDEX_ENCODER,

   TEAM271_SENSOR_INDEX_COUNT
}TEAM271_SENSOR_INDEX;

class t271_transmission_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_TRANSMISSION_DATA_NEXT_VALUE = t271_base_data_index::T271_DATA_COUNT,

      T271_TRANSMISSION_DATA_MODIFIER,

      T271_TRANSMISSION_DATA_ARB_FF_NEXT,

      T271_TRANSMISSION_DATA_WAIT,
      T271_TRANSMISSION_DATA_WAIT_MODIFIER,

      T271_TRANSMISSION_DATA_CONVERSION_POSITION,
      T271_TRANSMISSION_DATA_CONVERSION_RATE,

      T271_DATA_COUNT,
   } t271_transmission_data_index_t;
};

class Team271Transmission : public Team271Base
{
protected:
   /* Motors */
   Team271Motor* MotorControllers_[TEAM271_MOTOR_INDEX_COUNT];

   /* Shifters */
   Team271Shifter* Shifters_[TEAM271_SHIFTER_INDEX_COUNT];

   /* Sensors */
   Team271Sensor* Sensors_[TEAM271_SENSOR_INDEX_COUNT];

   float TimeWaitLast_ = 0;

public:
   /*
    *
    * Constants
    *
    */
   static constexpr uint32_t kForceSet = 0xA5A5A5A5U;

   static constexpr uint32_t kIsShifting = 1U;

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271Transmission( Team271Base* const argParent,
                        const string& argTransmissionName );

   virtual ~Team271Transmission( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   Team271Motor* GetMotor( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

#if defined(T271_CTRE)
   TalonSRX* GetTalon( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

   VictorSPX* GetVictor( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;
#endif

   float GetBusVoltage( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

   float GetOutputVoltage( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

   float GetOutputCurrent( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

   float GetPosition( const uint32_t argSensorIndex = TEAM271_SENSOR_INDEX_ENCODER ) const;

   float GetVelocity( const uint32_t argSensorIndex = TEAM271_SENSOR_INDEX_ENCODER ) const;

   float GetNextValue( void ) const;

   float GetCurrentValue( const uint32_t argMotorIndex = TEAM271_MOTOR_INDEX_MASTER ) const;

   float GetNeutralValue( void ) const;

   uint32_t IsShifting( void ) const;

   /*
    *
    * Setters
    *
    */
   Team271Shifter* SetShifter( const TEAM271_SHIFTER_INDEX argShifterIndex,
                               Team271Shifter* const argShifter );

   Team271Sensor* SetSensor( const uint32_t argSensorIndex,
                             Team271Sensor* const argSensor );

   void SetPosition( const float argPosition,
                     const uint32_t argSensorIndex = TEAM271_SENSOR_INDEX_ENCODER );

   void SetControlMode( const TEAM271_CONTROL_MODE argSetControlMode );

   /*
    *
    * Robot
    *
    */
   void PreRobotPeriodic( const bool argIsParent = false ) override;
   void PostRobotPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Disabled
    *
    */
   void DisabledInit( const bool argIsParent = false ) override;

   /*
    *
    * Autonomous
    *
    */
   void AutonomousInit( const bool argIsParent = false ) override;

   /*
    *
    * Teleop
    *
    */
   void TeleopInit( const bool argIsParent = false ) override;

   /*
    *
    * Test
    *
    */
   void TestInit( const bool argIsParent = false ) override;

   /*
    *
    * Transmission Functions
    *
    */
   Team271Motor* AddMotor( const string& argMotorName,
                           const TEAM271_MOTOR_CONTROLLER_TYPE argControllerType,
                           const uint32_t argControlID = 0,
                           Team271Motor* const argMasterMotor = nullptr );

   Team271Motor* AddMotor( Team271Motor* const argMotorController );

   void ShiftGears( const TEAM271_SHIFTER_INDEX argShifterIndex,
                    const eGear argGear );

   /*
    * Stop the Transmission
    *
    * Set all motors to their Neutral Value
    */
   void Stop( void );

   void SetOutput( const float argSet,
                   const float argSetModifier = 1.0f,
                   const float argFeedFwd = 0 );
};

#endif

#endif /* TEAM271_TRANSMISSION_H_ */
