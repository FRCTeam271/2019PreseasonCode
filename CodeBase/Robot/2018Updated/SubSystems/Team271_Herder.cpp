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

#if defined(T271_HERDER_ENABLE)

Team271Herder::Team271Herder( void )
      : Team271Base( nullptr,
                     "Herder",
                     t271_herder_data_index::T271_DATA_COUNT )
{
   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}

Team271Herder::~Team271Herder( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Herder::GetClassName( void ) const
{
   return "Team271Herder";
}

/*
 *
 * Setters
 *
 */
void Team271Herder::SetHerderGrabState( const t271_herder_grab_state_t argState )
{
   Write( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE,
          argState );
}

void Team271Herder::SetHerderWheelState( const t271_herder_wheels_state_t argState )
{
   Write( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE,
          argState );
}

/*
 *
 * Base
 *
 */
void Team271Herder::Init( void )
{
   Team271Motor* tmpMasterMotor = nullptr;
   Team271Motor* tmpSlaveMotor = nullptr;

   /*
    * Create Herder Driver Transmission
    *
    * Add it to the Child List
    */
   _TransHerderDriver = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                                  "Driver Side" ) );
   //Children_[HERDER_CHILD_INDEX_TRANS_DRIVER] = _TransHerderDriver;

   if( _TransHerderDriver != nullptr )
   {
      /*
       * Add the motors
       */
      tmpMasterMotor = _TransHerderDriver->AddMotor( "HerderDriver",
                                                     TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                                     TALON_SRX_HERDER_DRIVER_MOTOR );
      tmpMasterMotor->SetInvertedOutput( TEAM271_INVERTED_STATE_INVERTED );
      tmpMasterMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );
      tmpMasterMotor->SetOpenLoopRamp( 0.125 );
      /*tmpMasterMotor->SetClosedLoopRamp( 0.25 );*/

      tmpMasterMotor->SetPeakFwdVoltage( 0.7 );
      tmpMasterMotor->SetPeakRevVoltage( -0.7 );

      //ChildAdd( _TransHerderDriver );
   }

   /*
    * Create Herder Passenger Transmission
    *
    * Add it to the Child List
    */
   _TransHerderPassenger = Team271BaseManager::InstanceAdd( new Team271Transmission( this,
                                                                                     "Passenger Side" ) );
   //Children_[HERDER_CHILD_INDEX_TRANS_PASSENGER] = _TransHerderPassenger;

   if( _TransHerderPassenger != nullptr )
   {
      /*
       * Add the motors
       */
      tmpSlaveMotor = _TransHerderPassenger->AddMotor( "HerderPassenger",
                                                       TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
                                                       TALON_SRX_HERDER_PASSENGER_MOTOR );
      tmpSlaveMotor->SetNeutralMode( TEAM271_NEUTRAL_MODE_BRAKE );
      tmpSlaveMotor->SetOpenLoopRamp( 0.125 );
      /*tmpSlaveMotor->SetClosedLoopRamp( 0.25 );*/

      tmpSlaveMotor->SetPeakFwdVoltage( 0.7 );
      tmpSlaveMotor->SetPeakRevVoltage( -0.7 );

      //ChildAdd( _TransHerderPassenger );
   }

   /*
    * Setup Action Queue
    */
   //_Actions.clear();
   /*
    * Initialize Herder Position
    */
   //HerderActuatorFront( DEFAULT_HERDER_POSITION_FRONT );
   //HerderWheelsFront( T271_HERDER_WHEELS_OFF );
   _HerderClawClose = Team271BaseManager::InstanceAdd( new Team271Solenoid( this,
                                                                            "Herder Close",
                                                                            PCM_HERDER_GRAB,
                                                                            SOL_HERDER_GRAB_1 ) );

   _HerderClawClose = Team271BaseManager::InstanceAdd( new Team271Solenoid( this,
                                                                            "Herder Open",
                                                                            PCM_HERDER_GRAB,
                                                                            SOL_HERDER_GRAB_2 ) );
}

/*
 *
 * Robot
 *
 */
void Team271Herder::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::RobotInit();

   /*
    * Setup Herder
    */
   Write( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE,
          T271_HERDER_GRAB_CLOSE );

   Write( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE,
          T271_HERDER_WHEELS_OFF );
}

void Team271Herder::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   HerderGrabStatePrev_ = Read<t271_herder_grab_state_t>( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE );
   HerderWheelsStatePrev_ = Read<t271_herder_wheels_state_t>( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE );
}

void Team271Herder::PostRobotPeriodic( const bool argIsParent )
{
   /*
    *
    * Handle Herder Open/Close
    *
    */
   if( Read<t271_herder_grab_state_t>( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE ) == T271_HERDER_GRAB_OPEN )
   {
      /*
       * Open
       */
      if( _HerderClawClose != nullptr )
      {
         _HerderClawClose->SetOutput( TEAM271_SOLENOID_STATE_OFF );
      }
      if( _HerderClawOpen != nullptr )
      {
         _HerderClawOpen->SetOutput( TEAM271_SOLENOID_STATE_ON );
      }
   }
   else
   {
      /*
       * Close
       */
      if( _HerderClawClose != nullptr )
      {
         _HerderClawClose->SetOutput( TEAM271_SOLENOID_STATE_OFF );
      }
      if( _HerderClawOpen != nullptr )
      {
         _HerderClawOpen->SetOutput( TEAM271_SOLENOID_STATE_ON );
      }
   }

   /*
    *
    * Handle Herder Wheels
    *
    */
   if( Read<t271_herder_wheels_state_t>( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE ) == T271_HERDER_WHEELS_OFF )
   {
      if( _TransHerderDriver != nullptr )
      {
         _TransHerderDriver->SetOutput( 0 );
      }
      if( _TransHerderPassenger != nullptr )
      {
         _TransHerderPassenger->SetOutput( 0 );
      }
   }
   else if( Read<t271_herder_wheels_state_t>( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE ) == T271_HERDER_WHEELS_IN )
   {
      if( _TransHerderDriver != nullptr )
      {
         _TransHerderDriver->SetOutput( 1 );
      }
      if( _TransHerderPassenger != nullptr )
      {
         _TransHerderPassenger->SetOutput( 1 );
      }
   }
   else if( Read<t271_herder_wheels_state_t>( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE ) == T271_HERDER_WHEELS_OUT )
   {
      if( _TransHerderDriver != nullptr )
      {
         _TransHerderDriver->SetOutput( -1 );
      }
      if( _TransHerderPassenger != nullptr )
      {
         _TransHerderPassenger->SetOutput( -1 );
      }
   }
   else if( Read<t271_herder_wheels_state_t>( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE ) == T271_HERDER_WHEELS_OUT_AUTO )
   {
      if( _TransHerderDriver != nullptr )
      {
         _TransHerderDriver->SetOutput( -0.4 );
      }
      if( _TransHerderPassenger != nullptr )
      {
         _TransHerderPassenger->SetOutput( -0.4 );
      }
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
void Team271Herder::DisabledInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::DisabledInit();

   /*
    * Setup Herder
    */
   Write( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE,
          T271_HERDER_GRAB_CLOSE );

   Write( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE,
          T271_HERDER_WHEELS_OFF );
}

void Team271Herder::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Setup Herder
    */
   Write( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE,
          T271_HERDER_GRAB_CLOSE );

   Write( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE,
          T271_HERDER_WHEELS_OFF );
}

void Team271Herder::TeleopInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TeleopInit();

   /*
    * Setup Herder
    */
   Write( t271_herder_data_index::T271_HERDER_DATA_GRAB_STATE,
          T271_HERDER_GRAB_CLOSE );

   Write( t271_herder_data_index::T271_HERDER_DATA_WHEELS_STATE,
          T271_HERDER_WHEELS_OFF );
}

/*
 *
 * Robot State
 *
 */
void Team271Herder::UpdateInput( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::UpdateInput();

   /*
    * Check if we have new Driver Station Data
    */
#if defined(T271_INPUT_ENABLE)
   if( Team271Input::InstanceGetButtonState( eOperatorLeft,
                                             0 ) != 0 || Team271Input::InstanceGetAxis( eDriverLeft,
                                                                                        3 ) > 0.5 )
   {
      SetHerderWheelState( T271_HERDER_WHEELS_IN );
   }
   else if( Team271Input::InstanceGetButtonState( eOperatorRight,
                                                  0 ) != 0 || Team271Input::InstanceGetAxis( eDriverLeft,
                                                                                             4 ) > 0.5 )
   {
      SetHerderWheelState( T271_HERDER_WHEELS_OUT );
   }
   else
   {
      SetHerderWheelState( T271_HERDER_WHEELS_OFF );
   }
   /*
    if( Team271Input::Instance().GetButtonState( eOperatorLeft, eOperatorLeftButtonHerderIn ) != Team271Input::Instance().GetButtonStatePrev( eOperatorLeft, eOperatorLeftButtonHerderIn ) ||
    Team271Input::Instance().GetButtonState( eOperatorRight, eOperatorLeftButtonHerderIn ) != Team271Input::Instance().GetButtonStatePrev( eOperatorRight, eOperatorLeftButtonHerderIn ) ||
    Team271Input::Instance().GetButtonState( eDriverLeft, eOperatorLeftButtonHerderIn ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, eOperatorLeftButtonHerderIn ) )
    {
    HerderWheels( eHerderWheelsStateIn );
    }
    else if( Team271Input::Instance().GetButtonState( eOperatorLeft, eOperatorLeftButtonHerderOut ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, eOperatorLeftButtonHerderOut ) ||
    Team271Input::Instance().GetButtonState( eOperatorRight, eOperatorRightButtonHerderOut ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, eOperatorRightButtonHerderOut ) ||
    Team271Input::Instance().GetButtonState( eDriverLeft, eOperatorLeftButtonHerderOut ) != Team271Input::Instance().GetButtonStatePrev( eDriverLeft, eOperatorLeftButtonHerderOut ) )
    {
    HerderWheels( eHerderWheelsStateOut );
    }
    else
    {
    HerderWheels( T271_HERDER_WHEELS_OFF );
    }
    */

   if( Team271Input::InstanceGetButtonState( eOperatorLeft,
                                             1 ) != Team271Input::InstanceGetButtonStatePrev( eOperatorLeft,
                                                                                              1 ) )
   {
      if( Team271Input::InstanceGetButtonState( eOperatorLeft,
                                                1 ) != 0 )
      {
         /*
          * Open Herder
          */
         SetHerderGrabState( T271_HERDER_GRAB_OPEN );
      }
   }

   if( Team271Input::InstanceGetButtonState( eOperatorRight,
                                             1 ) != Team271Input::InstanceGetButtonStatePrev( eOperatorRight,
                                                                                              1 ) )
   {
      if( Team271Input::InstanceGetButtonState( eOperatorRight,
                                                1 ) != 0 )
      {
         /*
          * Close Herder
          */
         SetHerderGrabState( T271_HERDER_GRAB_CLOSE );
      }
   }
#endif
}

#endif

