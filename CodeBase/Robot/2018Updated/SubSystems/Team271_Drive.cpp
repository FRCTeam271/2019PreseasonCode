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

#if defined(T271_DRIVE_ENABLE)

Team271Drive::Team271Drive( void )
      : Team271Base( nullptr,
                     "Drive",
                     t271_drive_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
         GetName() );

   LOG_TRACE( "%s - Constructor End",
         GetName() );
}

Team271Drive::~Team271Drive( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Drive::GetClassName( void ) const
{
   return "Team271Drive";
}

float Team271Drive::GetHeading( void ) const
{
   return 0;
}

float Team271Drive::GetDriverPosition( void ) const
{
   float tmpReturn = 0;

   if( _DriverEncoder != nullptr )
   {
      tmpReturn = _DriverEncoder->GetPosition( DRIVE_ENCODER_CONST );
   }

   return tmpReturn;
}

float Team271Drive::GetDriverRate( void ) const
{
   float tmpReturn = 0;

   if( _DriverEncoder != nullptr )
   {
      tmpReturn = _DriverEncoder->GetRate( DRIVE_ENCODER_CONST ) * 10.0;
   }

   return tmpReturn;
}

float Team271Drive::GetDriverOutputVoltage( void ) const
{
   float tmpReturn = 0;

   if( _TransDriver != nullptr )
   {
      tmpReturn = _TransDriver->GetOutputVoltage();
   }

   return tmpReturn;
}

float Team271Drive::GetDriverOutputCurrent( void ) const
{
   float tmpReturn = 0;

   if( _TransDriver != nullptr )
   {
      tmpReturn = _TransDriver->GetOutputCurrent();
   }

   return tmpReturn;
}

float Team271Drive::GetPassengerPosition( void ) const
{
   float tmpReturn = 0;

   if( _PassengerEncoder != nullptr )
   {
      tmpReturn = _PassengerEncoder->GetPosition( DRIVE_ENCODER_CONST );
   }

   return tmpReturn;
}

float Team271Drive::GetPassengerRate( void ) const
{
   float tmpReturn = 0;

   if( _PassengerEncoder != nullptr )
   {
      tmpReturn = _PassengerEncoder->GetRate( DRIVE_ENCODER_CONST ) * 10.0;
   }

   return tmpReturn;
}

float Team271Drive::GetPassengerOutputVoltage( void ) const
{
   float tmpReturn = 0;

   if( _TransPassenger != nullptr )
   {
      tmpReturn = _TransPassenger->GetOutputVoltage();
   }

   return tmpReturn;
}

float Team271Drive::GetPassengerOutputCurrent( void ) const
{
   float tmpReturn = 0;

   if( _TransPassenger != nullptr )
   {
      tmpReturn = _TransPassenger->GetOutputCurrent();
   }

   return tmpReturn;
}

float Team271Drive::GetDriverAxisDriverX( void ) const
{
   /*
    * Get Drive Input
    */
#if defined( T271_INPUT_ENABLE )
   return Team271Input::Instance().GetAxis( eDriverLeft,
                                            eDriverLeftAxisDriveDriverX );
#else
   return 0;
#endif
}

float Team271Drive::GetDriverAxisDriverY( void ) const
{
   /*
    * Get Drive Input
    */
#if defined( T271_INPUT_ENABLE )
   return Team271Input::Instance().GetAxis( eDriverLeft,
                                            eDriverLeftAxisDriveDriverY ) * -1.0;
#else
   return 0;
#endif
}

float Team271Drive::GetDriverAxisPassengerX( void ) const
{
   /*
    * Get Drive Input
    */
#if defined( T271_INPUT_ENABLE )
   return Team271Input::Instance().GetAxis( eDriverLeft,
                                            eDriverLeftAxisDrivePassnegerX );
#else
   return 0;
#endif
}

float Team271Drive::GetDriverAxisPassengerY( void ) const
{
   /*
    * Get Drive Input
    */
#if defined( T271_INPUT_ENABLE )
   return Team271Input::Instance().GetAxis( eDriverLeft,
                                            eDriverLeftAxisDrivePassnegerY ) * -1.0;
#else
   return 0;
#endif
}

Team271IMU* Team271Drive::GetIMU( void )
{
   return IMU_;
}

/*
 *
 * Setters
 *
 */
void Team271Drive::SetDriveMode( const uint32_t argDriveMode )
{
   Write( t271_drive_data_index::T271_DRIVE_DATA_MODE,
          argDriveMode );
}

void Team271Drive::ShiftGears( const eDriveGear argGear )
{
   if( _TransDriver != nullptr )
   {
      _TransDriver->ShiftGears( TEAM271_SHIFTER_INDEX_1,
                                (eGear)argGear );
   }

   if( _TransPassenger != nullptr )
   {
      _TransPassenger->ShiftGears( TEAM271_SHIFTER_INDEX_1,
                                   (eGear)argGear );
   }
}

void Team271Drive::SetSetpoint( const float argDriverSetpoint,
                                const float argPassSetpoint )
{
   Write( t271_drive_data_index::T271_DRIVE_DATA_DRIVER_VALUE,
          argDriverSetpoint );

   Write( t271_drive_data_index::T271_DRIVE_DATA_PASS_VALUE,
          argPassSetpoint );
}

void Team271Drive::SetNeutral( void )
{
   float tmpDriverSetpoint = 0;
   float tmpPassSetpoint = 0;

   if( _TransDriver != nullptr )
   {
      tmpDriverSetpoint = _TransDriver->GetNeutralValue();
   }

   if( _TransPassenger != nullptr )
   {
      tmpPassSetpoint = _TransDriver->GetNeutralValue();
   }

   SetSetpoint( tmpDriverSetpoint,
                tmpPassSetpoint );
}

void Team271Drive::Init( void )
{
   Team271Motor* tmpMasterMotor = nullptr;
   Team271Motor* tmpSlaveMotor = nullptr;

   /*
    * Create Driver Side Transmission
    *
    * Add it to the Child List
    */
   _TransDriver = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                            "Driver Side" ) );
   //Children_[DRIVE_CHILD_INDEX_TRANS_DRIVER] = _TransDriver;

   if( _TransDriver != nullptr )
   {
      /*
       * Add the motors
       */
      tmpMasterMotor = _TransDriver->AddMotor( "Driver Front",
                                               TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                               TALON_SRX_FRONT_DRIVER_MOTOR );

      tmpMasterMotor->SetInvertedOutput( TEAM271_INVERTED_STATE_INVERTED );
      /*tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );*/

      /*tmpMasterMotor->SetOpenLoopRamp( 0.125 );*/
      /*tmpMasterMotor->SetClosedLoopRamp( 0.25 );*/
      tmpMasterMotor->SetVoltageCompensation( true );
      /*tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_COAST );*/
      tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );

      tmpMasterMotor->SetOpenLoopRamp( 0.05f );
      tmpMasterMotor->SetClosedLoopRamp( 0.00f );
      tmpMasterMotor->SetNeutralDeadband( 0.01f );
      tmpMasterMotor->SetMotionMagicVel( 6000 );
      tmpMasterMotor->SetMotionMagicAccel( 3000 );

      tmpSlaveMotor = _TransDriver->AddMotor( "Driver Middle",
                                              TEAM271_MOTOR_CONTROLLER_TYPE_VICTOR_SPX,
                                              TALON_SRX_MIDDLE_DRIVER_MOTOR,
                                              tmpMasterMotor );

      tmpSlaveMotor = _TransDriver->AddMotor( "Driver Rear",
                                              TEAM271_MOTOR_CONTROLLER_TYPE_VICTOR_SPX,
                                              TALON_SRX_REAR_DRIVER_MOTOR,
                                              tmpMasterMotor );

      /*
       * Create the Shifter
       */
      _DriveShifter = _TransDriver->SetShifter( TEAM271_SHIFTER_INDEX_1,
                                                new Team271Shifter_Drive( _TransDriver ) );

      /*
       * Setup Driver Side Sensors
       */
      _DriverEncoder = new Team271SensorSRX( _TransDriver,
                                             "Driver Side Enc",
                                             _TransDriver->GetMotor(),
                                             TEAM271_SENSOR_TYPE_MAG_REL,
                                             TEAM271_INVERTED_STATE_NOT_INVERTED,
                                             TEAM271_SENSOR_SETTINGS_NONE,
                                             0 );

      _DriverEncoder->SetVelMeas( T271_VelocityMeasPeriod::T271_Period_20Ms,
                                  16 );

      /*
       _DriverEncoder->SetVelMeas( T271_VelocityMeasPeriod::T271_Period_100Ms,
       64 );
       */

      _TransDriver->SetSensor( TEAM271_SENSOR_INDEX_ENCODER,
                               _DriverEncoder );

      _TransDriver->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
   }

   /*
    * Create Passenger Side Transmission
    *
    * Add it to the Child List
    */
   _TransPassenger = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                               "Passenger Side" ) );
   //Children_[DRIVE_CHILD_INDEX_TRANS_PASSENGER] = _TransPassenger;

   if( _TransPassenger != nullptr )
   {
      /*
       * Add the motors
       */
      tmpMasterMotor = _TransPassenger->AddMotor( "Passenger Front",
                                                  TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                                  TALON_SRX_FRONT_PASSENGER_MOTOR );

      tmpMasterMotor->SetInvertedOutput( TEAM271_INVERTED_STATE_NOT_INVERTED );
      /*tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );*/
      /*tmpMasterMotor->SetOpenLoopRamp( 0.125 );*/
      /*tmpMasterMotor->SetClosedLoopRamp( 0.25 );*/
      tmpMasterMotor->SetVoltageCompensation( true );
      /*tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_COAST );*/
      tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );
      tmpMasterMotor->SetOpenLoopRamp( 0.05f );
      tmpMasterMotor->SetClosedLoopRamp( 0.00f );
      tmpMasterMotor->SetNeutralDeadband( 0.01f );
      tmpMasterMotor->SetMotionMagicVel( 6000 );
      tmpMasterMotor->SetMotionMagicAccel( 3000 );

      tmpSlaveMotor = _TransPassenger->AddMotor( "Passenger Middle",
                                                 TEAM271_MOTOR_CONTROLLER_TYPE_VICTOR_SPX,
                                                 TALON_SRX_MIDDLE_PASSENGER_MOTOR,
                                                 tmpMasterMotor );

      tmpSlaveMotor = _TransPassenger->AddMotor( "Passenger Rear",
                                                 TEAM271_MOTOR_CONTROLLER_TYPE_VICTOR_SPX,
                                                 TALON_SRX_REAR_PASSENGER_MOTOR,
                                                 tmpMasterMotor );

      /*
       * Set the Shifters
       */
      _TransPassenger->SetShifter( TEAM271_SHIFTER_INDEX_1,
                                   new Team271Shifter_Drive( _TransPassenger,
                                                             0 ) );

      /*
       * Setup Passenger Side Sensors
       */
      _PassengerEncoder = new Team271SensorSRX( _TransPassenger,
                                                "Passenger Side Enc",
                                                _TransPassenger->GetMotor(),
                                                TEAM271_SENSOR_TYPE_MAG_REL,
                                                TEAM271_INVERTED_STATE_NOT_INVERTED,
                                                TEAM271_SENSOR_SETTINGS_NONE,
                                                0 );

      _PassengerEncoder->SetVelMeas( T271_VelocityMeasPeriod::T271_Period_20Ms,
                                     16 );
      /*
       _PassengerEncoder->SetVelMeas( T271_VelocityMeasPeriod::T271_Period_100Ms,
       64 );
       */

      _TransPassenger->SetSensor( TEAM271_SENSOR_INDEX_ENCODER,
                                  _PassengerEncoder );

      _TransPassenger->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
   }

   /*
    * Setup IMU
    */
   IMU_ = Team271BaseManager::InstanceAdd( new Team271EM7180( this ) );

   //CreateTable();
}

void Team271Drive::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::RobotInit();

   /*
    * Initialize Shifter
    */
   ShiftGears( DRIVE_DEFAULT_GEAR );

   /*
    * Stop Driving
    */
   SetNeutral();
}

void Team271Drive::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   /*
    * Store Previous Drive Mode
    */
   ModePrev_ = Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE );

   /*
    * Store the Previous Input Values
    */
   _InputDriverValuePrev = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_DRIVER_VALUE );
   _InputPassValuePrev = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_PASS_VALUE );
}

void Team271Drive::PostRobotPeriodic( const bool argIsParent )
{
   float tmpDriver = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_DRIVER_VALUE );
   float tmpPassenger = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_PASS_VALUE );

   /*const float batteryVoltage = frc::DriverStation::GetInstance().GetBatteryVoltage();*/

   /*
    * Driver Side
    */
   if( _TransDriver != nullptr )
   {
      /*
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) != SharedDataDrive_Continuous_.ModePrev_ )
       {
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 0 )
       {
       _TransDriver->Set( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
       }
       else if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 3 )
       {
       _TransDriver->Set( TEAM271_CONTROL_MODE_POSITION );
       }
       }

       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 3 )
       {
       tmpDriver = (tmpDriver / WHEEL_CIRCUMFRENCE) * 4096.0;
       }
       */
      /*
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 1 )
       {
       tmpDriver = m_drivetrain.ControllerLeftVoltage() / batteryVoltage;
       }
       */
      float Vintercept = ( 1.2 / 12 ) * 1023;
      float Kv = 0.626736178;
      float Ka = 0.18713812;

      if( tmpDriver < 0 )
      {
         Vintercept *= -1.0;
         Kv *= -1.0;
         Ka *= -1.0;
      }

      float tmpDriverF = Kv * 100 + Ka * 1.0 + Vintercept;

      if( ( tmpDriver > -0.03 && tmpDriver < 0.03 ) || Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) != 6 )
      {
         tmpDriverF = 0;
      }
      tmpDriverF = 0;

      if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 6 )
      {
         tmpDriver /= 0.1;
         tmpDriver /= DRIVE_ENCODER_CONST;
      }

      _TransDriver->SetOutput( tmpDriver,
                               1.0f,
                               tmpDriverF );
   }

   /*
    * Passenger Side
    */
   if( _TransPassenger != nullptr )
   {
      /*
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) != SharedDataDrive_Continuous_.ModePrev_ )
       {
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 0 )
       {
       _TransPassenger->Set( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
       }
       else if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 3 )
       {
       _TransPassenger->Set( TEAM271_CONTROL_MODE_POSITION );
       }
       }

       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 3 )
       {
       tmpPassenger = (tmpPassenger / WHEEL_CIRCUMFRENCE) * 4096.0;
       }
       */
      /*
       if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 1 )
       {
       tmpDriver = m_drivetrain.ControllerRightVoltage() / batteryVoltage;
       }
       */
      float Vintercept = ( 1.2 / 12 ) * 1023;
      float Kv = 0.626736178;
      float Ka = 0.18713812;

      if( tmpPassenger < 0 )
      {
         Vintercept *= -1.0;
         Kv *= -1.0;
         Ka *= -1.0;
      }

      float tmpPassengerF = Kv * 100 + Ka * 1.0 + Vintercept;

      if( ( tmpPassenger > -0.03 && tmpPassenger < 0.03 ) || Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) != 6 )
      {
         tmpPassengerF = 0;
      }
      tmpPassengerF = 0;

      if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 6 )
      {
         tmpPassenger /= 0.1;
         tmpPassenger /= DRIVE_ENCODER_CONST;
      }

      _TransPassenger->SetOutput( tmpPassenger,
                                  1.0f,
                                  tmpPassengerF );
   }

   /*
    if( _ClimbMode == 0 )
    {

    }
    else
    {
    _DriverValue = newPassenger * -1.0f;
    _PassengerValue = newDriver * -1.0f;
    }
    */

   /*
    * Base Call
    */
   Team271Base::PostRobotPeriodic();
}

void Team271Drive::DisabledInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::DisabledInit();

   /*
    * Stop Driving
    */
   SetNeutral();
}

void Team271Drive::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Stop Driving
    */
   SetNeutral();

   /*
    * Switch to Default Auto Gear
    */
   /*ShiftGears( DRIVE_DEFAULT_AUTO_GEAR );*/
#if defined(T271_AUTO_ENABLE)
   /*ShiftGears( _Auto->GetDriveGear() );*/
#endif
}

void Team271Drive::TeleopInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TeleopInit();

   /*
    * Stop Driving
    */
   SetNeutral();

   /*
    * Switch to Default Teleop Gear
    */
   /*ShiftGears( DRIVE_DEFAULT_TELEOP_GEAR );*/
}

void Team271Drive::TeleopPeriodic( const bool argIsParent )
{
   /*
    * Set Drive Values
    */
   if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 0 )
   {
      /*m_drivetrain.Disable();*/
      /*
       m_thread.Stop();
       */

      if( ModePrev_ != Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) )
      {
         _TransDriver->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
         _TransPassenger->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
      }

      SetSetpoint( Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_DRIVER_VALUE ),
                   Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_PASS_VALUE ) );
   }
   else if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 1 )
   {
      static float myInitalHeading = 0;

      if( ModePrev_ != Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) )
      {
         _TransDriver->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
         _TransPassenger->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );

         myInitalHeading = GetIMU()->GetYaw();
      }

      float tmpDriverValue = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_DRIVER_VALUE );
      float tmpPassValue = Read<float>( t271_drive_data_index::T271_DRIVE_DATA_INPUT_PASS_VALUE );

      SetSetpoint( tmpDriverValue,
                   tmpPassValue );
   }
   else if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 6 )
   {
      if( ModePrev_ != Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) )
      {
         _TransDriver->SetControlMode( TEAM271_CONTROL_MODE_VELOCITY );
         _TransPassenger->SetControlMode( TEAM271_CONTROL_MODE_VELOCITY );
      }
   }
   else if( Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) == 7 )
   {
      if( ModePrev_ != Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) )
      {
         _TransDriver->SetControlMode( TEAM271_CONTROL_MODE_MOTION_MAGIC );
         _TransPassenger->SetControlMode( TEAM271_CONTROL_MODE_MOTION_MAGIC );
      }
   }
}

/*
 *
 * Robot State
 *
 */
void Team271Drive::UpdateInput( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::UpdateInput();

   /*
    * Normal Throttle Drive
    */
   float tmpDriverY = GetDriverAxisDriverY();
   //const float tmpDriverX   = GetDriverAxisDriverX();
   const float tmpDriverX = 0;
   float tmpPassengerY = GetDriverAxisPassengerY();
   //const float tmpPassengerX = GetDriverAxisPassengerX();
   const float tmpPassengerX = 0;
   /*
    if( fabs(tmpDriverY - tmpPassengerY) < 0.10 )
    {
    tmpPassengerY = tmpDriverY;
    }
    */
   /*
    * Deadband and Deadband Scaling
    */
   float tmpDriverYDeadScale = 0;
   float tmpDriverXDeadScale = 0;
   float tmpPassengerYDeadScale = 0;
   float tmpPassengerXDeadScale = 0;

   ApplyRadialDeadZone( tmpDriverXDeadScale,
                        tmpDriverYDeadScale,
                        tmpDriverX,
                        tmpDriverY,
                        DRIVE_DEADZONE_LOW_DRIVER,
                        DRIVE_DEADZONE_HIGH_DRIVER );
   ApplyRadialDeadZone( tmpPassengerXDeadScale,
                        tmpPassengerYDeadScale,
                        tmpPassengerX,
                        tmpPassengerY,
                        DRIVE_DEADZONE_LOW_PASSENGER,
                        DRIVE_DEADZONE_HIGH_PASSENGER );

   /*
    * Apply a scaling factor for the different Gears
    */
   float tmpScaleFactor = 1.0f;

   if( _DriveShifter != nullptr )
   {
      if( _DriveShifter->GetGear() == (eGear)DRIVE_LOW )
      {
         tmpScaleFactor = DRIVE_SCALE_LOW;
      }
      else
      {
         tmpScaleFactor = DRIVE_SCALE_HIGH;
      }
   }

   /*
    * Calculate the new Value
    * Cube the Input, scale to Max Value
    */
   Write( t271_drive_data_index::T271_DRIVE_DATA_INPUT_DRIVER_VALUE,
          tmpScaleFactor * ( tmpDriverYDeadScale * tmpDriverYDeadScale * tmpDriverYDeadScale ) + ( DRIVE_INPUT_MAX_SCALE - tmpScaleFactor ) * tmpDriverYDeadScale );

   Write( t271_drive_data_index::T271_DRIVE_DATA_INPUT_PASS_VALUE,
          tmpScaleFactor * ( tmpPassengerYDeadScale * tmpPassengerYDeadScale * tmpPassengerYDeadScale ) + ( DRIVE_INPUT_MAX_SCALE - tmpScaleFactor ) * tmpPassengerYDeadScale );

#if defined( T271_INPUT_ENABLE )
   /*
    *
    * Shift
    *
    */
   if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                eDriverLeftButtonDriveLowGear ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft,
                                                                                                                                eDriverLeftButtonDriveLowGear ) )
   {
      if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                   eDriverLeftButtonDriveLowGear ) != 0 )
      {
         ShiftGears( DRIVE_LOW );
      }
   }

   if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                eDriverLeftButtonDriveHighGear ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft,
                                                                                                                                 eDriverLeftButtonDriveHighGear ) )
   {
      if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                   eDriverLeftButtonDriveHighGear ) != 0 )
      {
         ShiftGears( DRIVE_HIGH );
      }
   }

   if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                1 ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft,
                                                                                                    1 ) )
   {
      if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                   1 ) != 0 )
      {
         Write( t271_drive_data_index::T271_DRIVE_DATA_MODE,
                0 );
      }
   }

   if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                2 ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft,
                                                                                                    2 ) )
   {
      if( Team271Input::Instance().GetButtonState( eDriverLeft,
                                                   2 ) != 0 )
      {
         Write( t271_drive_data_index::T271_DRIVE_DATA_MODE,
                1 );
      }
   }
   /*
    if( Team271Input::Instance().GetButtonState( eDriverLeft, 1 ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, 1 ) )
    {
    if( Team271Input::Instance().GetButtonState( eDriverLeft, 1 ) != 0 )
    {
    Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) = 2;
    }
    }

    if( Team271Input::Instance().GetButtonState( eDriverLeft, 4 ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, 4 ) )
    {
    if( Team271Input::Instance().GetButtonState( eDriverLeft, 4 ) != 0 )
    {
    Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) = 1;
    }
    }

    if( Team271Input::Instance().GetButtonState( eDriverLeft, 2 ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, 2 ) )
    {
    if( Team271Input::Instance().GetButtonState( eDriverLeft, 2 ) != 0 )
    {
    Read<uint8_t>( t271_drive_data_index::T271_DRIVE_DATA_MODE ) = 0;
    }
    }
    */
#endif
}

void Team271Drive::StateSpaceUpdate( void )
{
   /*
    m_drivetrain.SetMeasuredStates( _TransDriver->GetPosition(),
    _TransPassenger->GetPosition() );
    */
   // Run controller update
   /*
    auto now = std::chrono::steady_clock::now();
    if( m_lastTime != std::chrono::steady_clock::time_point::min() )
    {
    m_drivetrain.Update( now - m_lastTime );
    }
    else
    {
    m_drivetrain.Update();
    }

    m_lastTime = now;
    */
   /*
    m_drivetrain.Update();
    */
}

#endif
