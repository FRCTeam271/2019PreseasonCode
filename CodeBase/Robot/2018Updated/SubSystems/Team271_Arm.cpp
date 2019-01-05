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

#if defined( T271_ARM_ENABLE )

#define ARM_LIMIT_DOWN     (315)
#define ARM_LIMIT_UP       (1350)

#define ARM_LIMIT_DELTA    (ARM_LIMIT_UP - ARM_LIMIT_DOWN)

Team271Arm::Team271Arm( void )
      : Team271Base( nullptr,
                     "Arm",
                     t271_arm_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}

Team271Arm::~Team271Arm( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Arm::GetClassName( void ) const
{
   return "Team271Arm";
}

float Team271Arm::GetSetpoint( void ) const
{
   return Read<float>( t271_arm_data_index::T271_ARM_DATA_VALUE );
}

float Team271Arm::GetOperatorArmAxisY( void ) const
{
   /*
    * Get Arm Input
    */
#if defined(T271_INPUT_ENABLE)
   return Team271Input::Instance().GetAxis( eOperatorRight,
                                            eOperatorRightInputAxisArmY ) * -1.0;
#else
   return 0;
#endif
}

/*
 *
 * Setters
 *
 */
void Team271Arm::SetSetpoint( const float argSetpoint )
{
   Write( t271_arm_data_index::T271_ARM_DATA_VALUE,
          argSetpoint );
}

/*
 *
 * Base
 *
 */
void Team271Arm::Init( void )
{
   Team271Motor* tmpMasterMotor = nullptr;

   /*
    * Create Arm Transmission
    *
    * Add it to the Child List
    */
   _TransArm = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                         "Arm" ) );

   //Children_[ARM_CHILD_INDEX_TRANS] = _TransArm;

   if( _TransArm != nullptr )
   {
      /*
       * Add the motors
       */
      tmpMasterMotor = _TransArm->AddMotor( "Arm 1",
                                            TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                            TALON_SRX_ARM_ONE_MOTOR );

      tmpMasterMotor->SetInvertedOutput( TEAM271_INVERTED_STATE_INVERTED );
      tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );

      tmpMasterMotor->SetOpenLoopRamp( 0.25 );
      /*tmpMasterMotor->SetClosedLoopRamp( 0.25 );*/

      tmpMasterMotor->SetPeakFwdVoltage( 0.4 );
      tmpMasterMotor->SetPeakRevVoltage( -0.4 );

      /*
       * Setup Sensors
       */
      _Encoder = new Team271SensorSRX( _TransArm,
                                       "Team271Arm - Encoder",
                                       _TransArm->GetMotor(),
                                       TEAM271_SENSOR_TYPE_MAG_ABS,
                                       TEAM271_INVERTED_STATE_NOT_INVERTED,
                                       TEAM271_SENSOR_SETTINGS_NONE,
                                       0 );

      _TransArm->SetSensor( TEAM271_SENSOR_INDEX_ENCODER,
                            _Encoder );

      _TransArm->SetControlMode( TEAM271_CONTROL_MODE_PERCENT_OUTPUT );
   }
}

/*
 *
 * Robot
 *
 */
void Team271Arm::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::RobotInit();

   /*
    * Zero Position
    */
   if( _TransArm != nullptr )
   {
      SetSetpoint( _TransArm->GetNeutralValue() );
   }
}

void Team271Arm::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   ModePrev_ = Read<uint8_t>( t271_arm_data_index::T271_ARM_DATA_MODE );
}

void Team271Arm::PostRobotPeriodic( const bool argIsParent )
{
   uint32_t tmpCanMove = 0;

   //const int32_t tmpLowerLimit = ARM_LIMIT_DOWN;
   //const int32_t tmpUpperLimit = ARM_LIMIT_UP;

   //const int32_t tmpLowerLimit = 0;
   //const int32_t tmpUpperLimit = ARM_LIMIT_DELTA;

   /*
    * Arm
    */
   if( _TransArm != nullptr )
   {

      /*
       * Apply the Arm Value
       */
//      if( _Encoder != nullptr )
//      {
//         if( _Encoder->GetPosition() > tmpLowerLimit && tmpArm < 0 )
//         {
//            tmpCanMove = 1;
//         }
//         else if( _Encoder->GetPosition() < tmpUpperLimit && tmpArm > 0 )
//         {
//            tmpCanMove = 1;
//         }
//      }
//      else
//      {
//         /*
//          * Fail Safe Always Move
//          */
//         tmpCanMove = 1;
//      }
      tmpCanMove = 1;

#if defined(T271_INPUT_ENABLE)
      if( Team271Input::InstanceGetDSOState() == TEAM271_DSO_STATE_ACTIVE )
      {
         tmpCanMove = 1;
      }
#endif

      if( tmpCanMove == 0 )
      {
         SetSetpoint( _TransArm->GetNeutralValue() );
      }

      _TransArm->SetOutput( GetSetpoint() );
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
void Team271Arm::DisabledInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::DisabledInit();

   /*
    * Stop Driving
    */
   if( _TransArm != nullptr )
   {
      SetSetpoint( _TransArm->GetNeutralValue() );
   }
}

void Team271Arm::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Stop Driving
    */
   if( _TransArm != nullptr )
   {
      SetSetpoint( _TransArm->GetNeutralValue() );
   }
}

void Team271Arm::TeleopInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TeleopInit();

   /*
    * Stop Driving
    */
   if( _TransArm != nullptr )
   {
      SetSetpoint( _TransArm->GetNeutralValue() );
   }
}

void Team271Arm::TeleopPeriodic( const bool argIsParent )
{
   /*
    * Set Drive Values
    */
   SetSetpoint( _InputArmValue.load( std::memory_order_relaxed ) );
}

void Team271Arm::TestInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TestInit();

   /*
    * Stop Driving
    */
   if( _TransArm != nullptr )
   {
      SetSetpoint( _TransArm->GetNeutralValue() );
   }
}

/*
 *
 * Robot State
 *
 */
void Team271Arm::UpdateInput( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::UpdateInput();

   /*
    * Store the Previous Value
    */
   _InputArmValuePrev.store( _InputArmValue.load( std::memory_order_relaxed ),
                             std::memory_order_relaxed );

   /*
    * Normal Throttle Drive
    */
   const float tmpArmY = GetOperatorArmAxisY();

   /*
    * Deadband and Deadband Scaling
    */
   float tmpArmYDeadScale = 0;
   float tmpArmXDeadScale = 0;

   ApplyRadialDeadZone( tmpArmXDeadScale,
                        tmpArmYDeadScale,
                        0,
                        tmpArmY,
                        ARM_DEADZONE_LOW,
                        ARM_DEADZONE_HIGH );

   /*
    * Apply a scaling factor for the different Gears
    */
   float tmpScaleFactor = 1.0f;

   /*
    * Calculate the new Value
    * Cube the Input, scale to Max Value
    */
   _InputArmValue.store( tmpScaleFactor * ( tmpArmYDeadScale * tmpArmYDeadScale * tmpArmYDeadScale ) + ( ARM_INPUT_MAX_SCALE - tmpScaleFactor ) * tmpArmYDeadScale,
                         std::memory_order_relaxed );

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
    if( _TransArm != nullptr )
    {
    _TransArm->SetPosition( 0 );
    }
    }
    }
    */
}

#endif
