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

#if defined( T271_LADDER_ENABLE )

//#define LADDER_LIMIT_DOWN         (3600-3100)
//#define LADDER_LIMIT_UP            (25500-3100)

#define LADDER_LIMIT_DOWN         (3600)
#define LADDER_LIMIT_UP            (25500)

#define LADDER_LIMIT_DELTA         (LADDER_LIMIT_UP-LADDER_LIMIT_DOWN)

//#define LADDER_LIMIT_DOWN         (6000)
//#define LADDER_LIMIT_UP            (10000)

Team271Ladder::Team271Ladder( void )
      : Team271Base( nullptr,
                     "Ladder",
                     t271_ladder_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}

Team271Ladder::~Team271Ladder( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Ladder::GetClassName( void ) const
{
   return "Team271Ladder";
}

float Team271Ladder::GetSetpoint( void ) const
{
   return Read<float>( t271_ladder_data_index::T271_LADDER_DATA_VALUE );
}

bool Team271Ladder::GetClimbMode( void ) const
{
   return Read<uint8_t>( t271_ladder_data_index::T271_LADDER_DATA_CLIMB_MODE );
}

float Team271Ladder::GetOperatorLadderAxisY( void ) const
{
   /*
    * Get Ladder Input
    */
#if defined(T271_INPUT_ENABLE)
   return Team271Input::Instance().GetAxis( eOperatorLeft,
                                            eOperatorLeftInputAxisLadderY ) * -1.0;
#else
   return 0;
#endif
}

/*
 *
 * Setters
 *
 */
void Team271Ladder::SetSetpoint( const float argSetpoint )
{
   Write( t271_ladder_data_index::T271_LADDER_DATA_VALUE,
          argSetpoint );
}

void Team271Ladder::SetClimbMode( const bool argClimbMode )
{
   Write( t271_ladder_data_index::T271_LADDER_DATA_CLIMB_MODE,
          argClimbMode );
}

/*
 *
 * Base
 *
 */
void Team271Ladder::Init( void )
{
   Team271Motor* tmpMasterMotor = nullptr;
   Team271Motor* tmpSlaveMotor = nullptr;

   /*
    * Create Ladder Transmission
    *
    * Add it to the Child List
    */
   _TransLadder = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                            "Ladder" ) );
   //Children_[LADDER_CHILD_INDEX_TRANS] = _TransLadder;

   if( _TransLadder != nullptr )
   {
      /*
       * Add the motors
       */
      tmpMasterMotor = _TransLadder->AddMotor( "Ladder 3",
                                               TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                               TALON_SRX_LADDER_THREE_MOTOR,
                                               nullptr );
      tmpMasterMotor->SetInvertedOutput( TEAM271_INVERTED_STATE_NOT_INVERTED );
      tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );

      tmpMasterMotor->SetOpenLoopRamp( 0.125 );
      /*tmpMasterMotor->SetClosedLoopRamp( 0.25 );*/

      tmpMasterMotor->SetPeakFwdVoltage( 0.1 );
      tmpMasterMotor->SetPeakRevVoltage( -0.8 );

      tmpMasterMotor->SetStatusFrameQuadEncoder( 20 );
      tmpMasterMotor->SetStatusFramePulseWidthMeas( 20 );

      tmpSlaveMotor = _TransLadder->AddMotor( "Ladder 1",
                                              TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                              TALON_SRX_LADDER_ONE_MOTOR,
                                              tmpMasterMotor );
      tmpSlaveMotor = _TransLadder->AddMotor( "Ladder 2",
                                              TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                              TALON_SRX_LADDER_TWO_MOTOR,
                                              tmpMasterMotor );
      tmpSlaveMotor = _TransLadder->AddMotor( "Ladder 4",
                                              TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                              TALON_SRX_LADDER_FOUR_MOTOR,
                                              tmpMasterMotor );

      /*
       * Create the Shifter - Speed
       */
      _ShifterSpeed = _TransLadder->SetShifter( TEAM271_SHIFTER_INDEX_1,
                                                new Team271Shifter_LadderSpeed( _TransLadder ) );

      /*
       * Create the Shifter - Break
       */
      _ShifterBreak = _TransLadder->SetShifter( TEAM271_SHIFTER_INDEX_2,
                                                new Team271Shifter_LadderBrake( _TransLadder ) );

      //_TransLadder->GetMasterTalon()->ConfigReverseSoftLimitThreshold( LADDER_LIMIT_DOWN, 10 );
      //_TransLadder->GetMasterTalon()->ConfigReverseSoftLimitEnable( true, 10 );

      //_TransLadder->GetMasterTalon()->ConfigForwardSoftLimitThreshold( LADDER_LIMIT_UP, 10 );
      //_TransLadder->GetMasterTalon()->ConfigForwardSoftLimitEnable( true, 10 );

      /*
       * Setup Sensors
       */
      _Encoder = new Team271SensorSRX( _TransLadder,
                                       "Team271Ladder - Encoder",
                                       _TransLadder->GetMotor(),
                                       TEAM271_SENSOR_TYPE_MAG_ABS,
                                       TEAM271_INVERTED_STATE_NOT_INVERTED,
                                       TEAM271_SENSOR_SETTINGS_NONE,
                                       0 );

      _TransLadder->SetSensor( TEAM271_SENSOR_INDEX_ENCODER,
                               _Encoder );

      _TransLadder->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
   }
}

/*
 *
 * Robot
 *
 */
void Team271Ladder::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::RobotInit();

   /*
    * Initialize Shifter
    */
   ShiftGears( LADDER_BRAKE_OFF );
   ShiftGears( LADDER_DEFAULT_GEAR );

   /*
    * Zero Position
    */
   if( _TransLadder != nullptr )
   {
      SetSetpoint( _TransLadder->GetNeutralValue() );
   }
}

void Team271Ladder::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   ModePrev_ = Read<uint8_t>( t271_ladder_data_index::T271_LADDER_DATA_MODE );
}

void Team271Ladder::PostRobotPeriodic( const bool argIsParent )
{
   uint32_t tmpCanMove = 0;

   //const int32_t tmpLowerLimit = LADDER_LIMIT_DOWN;
   //const int32_t tmpUpperLimit = LADDER_LIMIT_UP;

   const int32_t tmpLowerLimit = 0;
   const int32_t tmpUpperLimit = LADDER_LIMIT_DELTA;

   /*
    * Ladder
    */
   if( _TransLadder != nullptr )
   {
      /*
       * Check if the Brake is Engaged
       * If it is stop the Ladder
       */
      if( _ShifterBreak != nullptr )
      {
         if( _ShifterBreak->GetGear() == (eGear)LADDER_BRAKE_ON )
         {
            Write( t271_ladder_data_index::T271_LADDER_DATA_VALUE,
                   0 );
         }
      }

      /*
       * Apply the Ladder Value
       */
      if( _Encoder != nullptr )
      {
         if( ( _Encoder->GetPosition() > tmpLowerLimit ) && ( Read<float>( t271_ladder_data_index::T271_LADDER_DATA_VALUE ) > 0 ) )
         {
            tmpCanMove = 1;
         }
         else if( ( _Encoder->GetPosition() < tmpUpperLimit ) && ( Read<float>( t271_ladder_data_index::T271_LADDER_DATA_VALUE ) < 0 ) )
         {
            tmpCanMove = 1;
         }
      }
      else
      {
         /*
          * Fail Safe Always Move
          */
         tmpCanMove = 1;
      }

#if defined(T271_INPUT_ENABLE)
      if( Team271Input::InstanceGetDSOState() == TEAM271_DSO_STATE_ACTIVE )
      {
         tmpCanMove = 1;
      }
#endif

      if( tmpCanMove == 0 )
      {
         SetSetpoint( _TransLadder->GetNeutralValue() );
      }

      _TransLadder->SetOutput( GetSetpoint() );
   }

   /*
    * Base Call
    */
   Team271Base::PostRobotPeriodic();
}

/*
 *
 * Disabled
 *
 */
void Team271Ladder::DisabledInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::DisabledInit();

   /*
    * Stop Driving
    */
   if( _TransLadder != nullptr )
   {
      SetSetpoint( _TransLadder->GetNeutralValue() );
   }
}

void Team271Ladder::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Stop Driving
    */
   if( _TransLadder != nullptr )
   {
      SetSetpoint( _TransLadder->GetNeutralValue() );
   }

   /*
    * Switch to Default Auto Gear
    */
   ShiftGears( LADDER_BRAKE_OFF );
   ShiftGears( LADDER_DEFAULT_AUTO_GEAR );
}

void Team271Ladder::TeleopInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TeleopInit();

   /*
    * Stop Driving
    */
   if( _TransLadder != nullptr )
   {
      SetSetpoint( _TransLadder->GetNeutralValue() );
   }

   /*
    * Switch to Default Teleop Gear
    */
   ShiftGears( LADDER_BRAKE_OFF );
   ShiftGears( LADDER_DEFAULT_TELEOP_GEAR );
}

void Team271Ladder::TeleopPeriodic( const bool argIsParent )
{
   ModePrev_ = Read<uint8_t>( t271_ladder_data_index::T271_LADDER_DATA_MODE );

   /*
    * Set Drive Values
    */
   /*
    if( _DriveMode == 0 )
    {
    }
    else if( _DriveMode == 3 )
    {
    bool tmpReset = false;

    if( _DriveModePrev != _DriveMode )
    {
    tmpReset = true;
    }

    float tmpLadder = 0;
    float tmpArm = 0;
    CalculateEncShift( _EncSetPoint, _EncSetPoint, 0.05, &tmpLadder, &tmpArm, 0, 0.5, tmpReset );

    tmpLadderYAdj = tmpLadder;
    tmpArmYAdj = tmpArm;
    }
    */

   SetSetpoint( _InputLadderValue.load( std::memory_order_relaxed ) );
}

void Team271Ladder::TestInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TestInit();

   /*
    * Stop Driving
    */
   if( _TransLadder != nullptr )
   {
      SetSetpoint( _TransLadder->GetNeutralValue() );
   }
}

/*
 *
 * Robot State
 *
 */
void Team271Ladder::UpdateInput( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::UpdateInput();

   /*
    * Store the Previous Value
    */
   _InputLadderValuePrev.store( _InputLadderValue.load( std::memory_order_relaxed ),
                                std::memory_order_relaxed );

   /*
    * Normal Throttle Drive
    */
   const float tmpLadderY = GetOperatorLadderAxisY();

   /*
    * Deadband and Deadband Scaling
    */
   float tmpLadderYDeadScale = 0;
   float tmpLadderXDeadScale = 0;

   ApplyRadialDeadZone( tmpLadderXDeadScale,
                        tmpLadderYDeadScale,
                        0,
                        tmpLadderY,
                        LADDER_DEADZONE_LOW,
                        LADDER_DEADZONE_HIGH );

   /*
    * Apply a scaling factor for the different Gears
    */
   float tmpScaleFactor = 1.0f;

   if( _ShifterSpeed != nullptr )
   {
      if( _ShifterSpeed->GetGear() == (eGear)LADDER_LOW )
      {
         tmpScaleFactor = LADDER_SCALE_LOW;
      }
      else
      {
         tmpScaleFactor = LADDER_SCALE_HIGH;
      }
   }

   /*
    * Calculate the new Value
    * Cube the Input, scale to Max Value
    */
   _InputLadderValue.store( tmpScaleFactor * ( tmpLadderYDeadScale * tmpLadderYDeadScale * tmpLadderYDeadScale ) + ( LADDER_INPUT_MAX_SCALE - tmpScaleFactor ) * tmpLadderYDeadScale,
                            std::memory_order_relaxed );

#if defined(T271_INPUT_ENABLE)
   /*
    *
    * Shift
    *
    */
   /*
    * Ladder Speed Shifter
    */
   /*
    if( Input_->GetButtonState( eOperatorLeft,
    5 ) != Input_->GetButtonStatePrev( eOperatorLeft,
    5 ) )
    {
    if( Input_->GetButtonState( eOperatorLeft,
    5 ) != 0 )
    {
    ShiftGears( LADDER_LOW );
    }
    }

    if( Input_->GetButtonState( eOperatorLeft,
    6 ) != Input_->GetButtonStatePrev( eOperatorLeft,
    6 ) )
    {
    if( Input_->GetButtonState( eOperatorLeft,
    6 ) != 0 )
    {
    ShiftGears( LADDER_HIGH );
    }
    }
    */

   /*
    * Ladder Break Shifter
    */
   /*
    if( Input_->GetButtonState( eOperatorRight,
    5 ) != Input_->GetButtonStatePrev( eOperatorRight,
    5 ) )
    {
    if( Input_->GetButtonState( eOperatorRight,
    5 ) != 0 )
    {
    ShiftGears( LADDER_BREAK_OFF );
    }
    }

    if( Input_->GetButtonState( eOperatorRight,
    6 ) != Input_->GetButtonStatePrev( eOperatorRight,
    6 ) )
    {
    if( Input_->GetButtonState( eOperatorRight,
    6 ) != 0 )
    {
    ShiftGears( LADDER_BREAK_ON );
    }
    }
    */

   /*
    * Reset Zero
    */
   /*
    if( Input_->GetButtonState( eOperatorRight,
    7 ) != Input_->GetButtonStatePrev( eOperatorRight,
    7 ) )
    {
    if( Input_->GetButtonState( eOperatorRight,
    8 ) != 0 && Input_->GetButtonState( eOperatorRight,
    8 ) != 0 )
    {
    if( _TransLadder != nullptr )
    {
    _TransLadder->SetPosition( 0 );
    }
    }
    }
    */
#endif
}

void Team271Ladder::ShiftGears( const eLadderGear argGear )
{
   if( _TransLadder != nullptr )
   {
      _TransLadder->ShiftGears( TEAM271_SHIFTER_INDEX_1,
                                (eGear)argGear );
   }
}

void Team271Ladder::ShiftGears( const eLadderBrake argGear )
{
   if( _TransLadder != nullptr )
   {
      _TransLadder->ShiftGears( TEAM271_SHIFTER_INDEX_2,
                                (eGear)argGear );
   }
}

#endif
