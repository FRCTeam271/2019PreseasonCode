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

#ifndef TEAM271_DRIVE_H_
#define TEAM271_DRIVE_H_

#if defined(T271_DRIVE_ENABLE)

#define DRIVE_DEADZONE_LOW_DRIVER         ( 0.05f )
#define DRIVE_DEADZONE_HIGH_DRIVER         ( 0.05f )

#define DRIVE_DEADZONE_LOW_PASSENGER      ( 0.05f )
#define DRIVE_DEADZONE_HIGH_PASSENGER      ( 0.05f )

#define DRIVE_INPUT_MAX_SCALE               ( 1.005f )

#define DRIVE_SCALE_LOW                     ( 0.7f )
#define DRIVE_SCALE_HIGH                  ( 0.3f )

#define DRIVE_DEFAULT_GEAR                  ( DRIVE_HIGH )
#define DRIVE_DEFAULT_TELEOP_GEAR         ( DRIVE_HIGH )
#define DRIVE_DEFAULT_AUTO_GEAR            ( DRIVE_LOW  )

//#define DRIVE_CHILD_INDEX_TRANS_DRIVER      (0U)
//#define DRIVE_CHILD_INDEX_TRANS_PASSENGER   (1U)

/*
 * Wheel Diameter
 */
constexpr float DRIVE_WHEEL_DIAMETER_IN = 4.0;
constexpr float DRIVE_WHEEL_DIAMETER_FT = DRIVE_WHEEL_DIAMETER_IN / 12.0;
constexpr float DRIVE_WHEEL_DIAMETER_M = DRIVE_WHEEL_DIAMETER_FT * 0.3048;

/*
 * Wheel Radius
 */
constexpr float DRIVE_WHEEL_RADIUS_IN = DRIVE_WHEEL_DIAMETER_IN / 2.0;
constexpr float DRIVE_WHEEL_RADIUS_FT = DRIVE_WHEEL_RADIUS_IN / 12.0;
constexpr float DRIVE_WHEEL_RADIUS_M = DRIVE_WHEEL_RADIUS_FT * 0.3048;

/*
 * Wheel Circumference
 */
constexpr float DRIVE_WHEEL_CIRCUMFERENCE_IN = 2.0 * kTeam271Pi * DRIVE_WHEEL_RADIUS_IN;
constexpr float DRIVE_WHEEL_CIRCUMFERENCE_FT = DRIVE_WHEEL_CIRCUMFERENCE_IN / 12.0;
constexpr float DRIVE_WHEEL_CIRCUMFERENCE_M = DRIVE_WHEEL_CIRCUMFERENCE_FT * 0.3048;

/*
 * Encoder Counts Per Revolution
 */
constexpr float DRIVE_ENCODER_COUNTS_PER_REV = 4096.0;

/*
 * Encoder Counts to Distance
 */
constexpr float DRIVE_ENCODER_TO_DISTANCE_IN = (1.0 / DRIVE_WHEEL_CIRCUMFERENCE_IN) * DRIVE_WHEEL_DIAMETER_IN * kTeam271Pi;
constexpr float DRIVE_ENCODER_TO_DISTANCE_FT = DRIVE_ENCODER_TO_DISTANCE_IN / 12.0;
constexpr float DRIVE_ENCODER_TO_DISTANCE_M = DRIVE_ENCODER_TO_DISTANCE_FT * 0.3048;

constexpr float DRIVE_ENCODER_CONST = (1.0 / (DRIVE_ENCODER_COUNTS_PER_REV)) * DRIVE_WHEEL_DIAMETER_FT * kTeam271Pi;

/*
 *
 * Shared Network Data
 *
 */
class t271_drive_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Mode
       */
      T271_DRIVE_DATA_MODE = t271_base_data_index::T271_DATA_COUNT,

      /*
       * Driver
       */
      T271_DRIVE_DATA_DRIVER_VALUE,
      T271_DRIVE_DATA_INPUT_DRIVER_VALUE,

      /*
       * Passenger
       */
      T271_DRIVE_DATA_PASS_VALUE,
      T271_DRIVE_DATA_INPUT_PASS_VALUE,

      T271_DATA_COUNT,
   } t271_drive_data_index_t;
};

class Team271Drive : public Team271Base
{
protected:
   /*
    * Drive Transmissions
    */
   Team271Transmission* _TransDriver = nullptr;
   Team271Transmission* _TransPassenger = nullptr;

   /*
    * Drive Shifter
    */
   Team271Shifter* _DriveShifter = nullptr;

   /*
    * Drive Sensors
    */
   Team271SensorSRX* _DriverEncoder = nullptr;
   Team271SensorSRX* _PassengerEncoder = nullptr;

   Team271IMU* IMU_ = nullptr;

   /*
    *
    * Variables
    *
    */
   uint8_t ModePrev_ = 0;

   float _InputDriverValuePrev = 0;
   float _InputPassValuePrev = 0;

   //DrivetrainController m_drivetrain;
   //std::chrono::steady_clock::time_point m_lastTime = std::chrono::steady_clock::time_point::min();

   /*frc::Notifier m_thread{&Drivetrain::Iterate, this};*/

public:
   Team271Drive( void );

   virtual ~Team271Drive( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   float GetHeading( void ) const;

   float GetDriverPosition( void ) const;
   float GetDriverRate( void ) const;
   float GetDriverOutputVoltage( void ) const;
   float GetDriverOutputCurrent( void ) const;

   float GetPassengerPosition( void ) const;
   float GetPassengerRate( void ) const;
   float GetPassengerOutputVoltage( void ) const;
   float GetPassengerOutputCurrent( void ) const;

   float GetDriverAxisDriverX( void ) const;
   float GetDriverAxisDriverY( void ) const;

   float GetDriverAxisPassengerX( void ) const;
   float GetDriverAxisPassengerY( void ) const;

   Team271IMU* GetIMU( void );

   /*
    *
    * Setters
    *
    */
   void SetDriveMode( const uint32_t argDriveMode );

   void SetSetpoint( const float argDriverSetpoint,
                     const float argPassSetpoint );

   void SetNeutral( void );

   /*
    *
    * Base
    *
    */
   void Init( void ) override;

   /*
    *
    * Robot
    *
    */
   void RobotInit( const bool argIsParent = false ) override;

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

   void TeleopPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Robot State
    *
    */
   void UpdateInput( const bool argIsParent = false ) override;

   /*
    *
    * Drive
    *
    */
   void ShiftGears( const eDriveGear argGear );

   void StateSpaceUpdate( void );
};

#endif

#endif /* TEAM271_DRIVE_H_ */
