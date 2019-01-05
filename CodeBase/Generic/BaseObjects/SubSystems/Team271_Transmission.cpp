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

#if defined(T271_TRANSMISSION_ENABLE)

Team271Transmission::Team271Transmission( Team271Base* const argParent,
                                          const string& argTransmissionName )
      : Team271Base( argParent,
                     argTransmissionName,
                     t271_transmission_data_index::T271_DATA_COUNT )
{
   uint32_t i = 0;

   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   /*
    * Initialize Motor Controller Structures
    */
   for( uint32_t i = 0; i < TEAM271_MOTOR_INDEX_COUNT; i++ )
   {
      MotorControllers_[i] = nullptr;
   }

   /*
    * Setup Sensor Array
    */
   for( i = 0; i < TEAM271_SENSOR_INDEX_COUNT; i++ )
   {
      Sensors_[i] = nullptr;
   }

   /*
    * Initialize the Shifter Array
    */
   for( uint32_t i = 0; i < TEAM271_SHIFTER_INDEX_COUNT; i++ )
   {
      Shifters_[i] = nullptr;
   }

   /*
    * Store Parameters
    */
   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_MODIFIER,
          1.0f );

   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT_MODIFIER,
          1.0f );

   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_CONVERSION_POSITION,
          1.0f );

   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_CONVERSION_RATE,
          1.0f );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271Transmission::~Team271Transmission( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Transmission::GetClassName( void ) const
{
   return "Team271Transmission";
}

Team271Motor* Team271Transmission::GetMotor( const uint32_t argMotorIndex ) const
{
   Team271Motor* tmpMotor = nullptr;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpMotor = MotorControllers_[argMotorIndex];
   }

   return tmpMotor;
}

#if defined(T271_CTRE)
TalonSRX* Team271Transmission::GetTalon( const uint32_t argMotorIndex ) const
{
   TalonSRX* tmpMotor = nullptr;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpMotor = MotorControllers_[argMotorIndex]->GetTalon();
   }

   return tmpMotor;
}

VictorSPX* Team271Transmission::GetVictor( const uint32_t argMotorIndex ) const
{
   VictorSPX* tmpMotor = nullptr;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpMotor = MotorControllers_[argMotorIndex]->GetVictor();
   }

   return tmpMotor;
}
#endif

float Team271Transmission::GetBusVoltage( const uint32_t argMotorIndex ) const
{
   float tmpVoltage = 0;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpVoltage = MotorControllers_[argMotorIndex]->GetBusVoltage();
   }

   return tmpVoltage;
}

float Team271Transmission::GetOutputVoltage( const uint32_t argMotorIndex ) const
{
   float tmpVoltage = 0;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpVoltage = MotorControllers_[argMotorIndex]->GetOutputVoltage();
   }

   return tmpVoltage;
}

float Team271Transmission::GetOutputCurrent( const uint32_t argMotorIndex ) const
{
   float tmpCurrent = 0;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpCurrent = MotorControllers_[argMotorIndex]->GetOutputCurrent();
   }

   return tmpCurrent;
}

float Team271Transmission::GetPosition( const uint32_t argSensorIndex ) const
{
   float tmpReturnValue = 0;

   if( argSensorIndex >= TEAM271_SENSOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sensor Index: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else if( Sensors_[argSensorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Sensor Doesn't Exist: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else
   {
      tmpReturnValue = Sensors_[argSensorIndex]->GetPosition( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_CONVERSION_POSITION ) );
   }

   return tmpReturnValue;
}

float Team271Transmission::GetVelocity( const uint32_t argSensorIndex ) const
{
   float tmpReturnValue = 0;

   if( argSensorIndex >= TEAM271_SENSOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sensor Index: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else if( Sensors_[argSensorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Sensor Doesn't exist: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else
   {
      tmpReturnValue = Sensors_[argSensorIndex]->GetRate( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_CONVERSION_RATE ) );
   }

   return tmpReturnValue;
}

float Team271Transmission::GetNextValue( void ) const
{
   return Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_NEXT_VALUE );
}

float Team271Transmission::GetCurrentValue( const uint32_t argMotorIndex ) const
{
   float tmpValue = 0;

   if( argMotorIndex >= TEAM271_MOTOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Motor Index: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else if( MotorControllers_[argMotorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Motor Doesn't Exist: %d",
                 GetName().c_str(),
                 argMotorIndex );
   }
   else
   {
      tmpValue = MotorControllers_[argMotorIndex]->GetValueCurrent();
   }

   return tmpValue;
}

float Team271Transmission::GetNeutralValue( void ) const
{
   float tmpValue = 0;

   if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] == nullptr )
   {
      LOG_ERROR( "%s - Master Motor Doesn't Exist",
                 GetName().c_str() );
   }
   else
   {
      tmpValue = MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->GetValueNeutral();
   }

   return tmpValue;
}

uint32_t Team271Transmission::IsShifting( void ) const
{
   if( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT ) > 0 )
   {
      return kIsShifting;
   }

   return 0;
}

/*
 *
 * Setters
 *
 */
Team271Shifter* Team271Transmission::SetShifter( const TEAM271_SHIFTER_INDEX argShifterIndex,
                                                 Team271Shifter* const argShifter )
{
   Team271Shifter* tmpShifter = nullptr;

   LOG_TRACE( "%s - %d - Set Shifter",
         GetName().c_str(),
         argShifterIndex );

   if( argShifter == nullptr )
   {
      LOG_ERROR( "%s - %d - Invalid Shifter",
                 GetName().c_str(),
                 argShifterIndex );
   }
   else if( argShifterIndex >= TEAM271_SHIFTER_INDEX_COUNT )
   {
      LOG_ERROR( "%s - %d - %s - Invalid Shifter Index",
                 GetName().c_str(),
                 argShifterIndex,
                 argShifter->GetName() );
   }
   else if( Shifters_[argShifterIndex] != nullptr )
   {
      LOG_ERROR( "%s - %d - %s - Shifter Already Exists",
                 GetName().c_str(),
                 argShifterIndex,
                 argShifter->GetName() );
   }
   else
   {
      Shifters_[argShifterIndex] = Team271BaseManager::InstanceAdd( argShifter );

      tmpShifter = argShifter;
   }

   return tmpShifter;
}

Team271Sensor* Team271Transmission::SetSensor( const uint32_t argSensorIndex,
                                               Team271Sensor* const argSensor )
{
   Team271Sensor* tmpSensor = nullptr;

   if( argSensorIndex >= TEAM271_SENSOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sensor Index: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else if( Sensors_[argSensorIndex] != nullptr )
   {
      LOG_ERROR( "%s - %d - %s - Sensor Already Exists",
                 GetName().c_str(),
                 argSensorIndex,
                 argSensor->GetName() );
   }
   else
   {
      Sensors_[argSensorIndex] = Team271BaseManager::InstanceAdd( argSensor );

      tmpSensor = argSensor;
   }

   return tmpSensor;
}

void Team271Transmission::SetPosition( const float argPosition,
                                       const uint32_t argSensorIndex )
{
   if( argSensorIndex >= TEAM271_SENSOR_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sensor Index: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else if( Sensors_[argSensorIndex] == nullptr )
   {
      LOG_ERROR( "%s - Sensor doesn't exist: %d",
                 GetName().c_str(),
                 argSensorIndex );
   }
   else
   {
      Sensors_[argSensorIndex]->SetPosition( argPosition );
   }
}

void Team271Transmission::SetControlMode( const TEAM271_CONTROL_MODE argSetControlMode )
{
   /*
    * Only allow a set if we aren't in the middle of a shift, unless we set the force flag
    */
   if( IsShifting() == kIsShifting )
   {
      /*
       DEBUG_PRINT_FUNC( GetName().c_str(),
       T271_FUNC_NAME,
       "Waiting for a shift to end" );
       */

      //LOG_WARN( "%s - Waiting for a shift to end",
      //GetName().c_str() );
   }

   /*
    * Check if there is a valid Master Controller
    */
   else if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] == nullptr )
   {
      LOG_ERROR( "%s - No Master Motor Controller",
                 GetName().c_str() );
   }

   /*
    * Apply Changes
    */
   else
   {
      MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->SetControlMode( argSetControlMode );
   }
}

/*
 *
 * Robot
 *
 */
void Team271Transmission::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   /*
    * Check if we need to restore the speed
    */
   if( TimeWaitLast_ != 0 )
   {
      if( Team271Robot::CompareTime( TimeWaitLast_,
                                     Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT ) ) == 1 )
      {
         TimeWaitLast_ = 0;

         Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT,
                0.0f );
         Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT_MODIFIER,
                1.0f );
      }
   }
}

void Team271Transmission::PostRobotPeriodic( const bool argIsParent )
{
   /*
    * Check if there is a valid Master Controller
    */
   if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] == nullptr )
   {

   }

   /*
    * Check to make sure the Master isn't a follower
    */
   else if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->GetControlMode() == TEAM271_CONTROL_MODE_FOLLOWER )
   {

   }

   /*
    * Apply Changes
    */
   else
   {
      if( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT ) > 0 )
      {
         if( TimeWaitLast_ == 0 )
         {
            TimeWaitLast_ = Team271Robot::GetTime();
         }

         MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->SetNext( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_NEXT_VALUE ) * fabs( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT_MODIFIER ) ),
                                                                 Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_ARB_FF_NEXT ) );
      }
      else
      {
         MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->SetNext( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_NEXT_VALUE ) * fabs( Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_MODIFIER ) ),
                                                                 Read<float>( t271_transmission_data_index::T271_TRANSMISSION_DATA_ARB_FF_NEXT ) );
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
void Team271Transmission::DisabledInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::DisabledInit();

   /*
    * Stop the transmission
    */
   Stop();
}

/*
 *
 * Autonomous
 *
 */
void Team271Transmission::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Stop the transmission
    */
   Stop();
}

/*
 *
 * Teleop
 *
 */
void Team271Transmission::TeleopInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TeleopInit();

   /*
    * Stop the transmission
    */
   Stop();
}

/*
 *
 * Test
 *
 */
void Team271Transmission::TestInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::TestInit();

   /*
    * Stop the transmission
    */
   Stop();
}

/*
 *
 * Transmission Functions
 *
 */
Team271Motor* Team271Transmission::AddMotor( const string& argMotorName,
                                             const TEAM271_MOTOR_CONTROLLER_TYPE argControllerType,
                                             const uint32_t argControlID,
                                             Team271Motor* const argMasterMotor )
{
   Team271Motor* tmpMotor = nullptr;

   if( argMasterMotor == nullptr )
   {
      tmpMotor = new Team271Motor( this,
                                   argMotorName,
                                   argControllerType,
                                   argControlID );
   }
   else
   {
      tmpMotor = new Team271Motor( argMasterMotor,
                                   argMotorName,
                                   argControllerType,
                                   argControlID,
                                   argMasterMotor );
   }

   Team271BaseManager::InstanceAdd( tmpMotor );

   return AddMotor( tmpMotor );
}

Team271Motor* Team271Transmission::AddMotor( Team271Motor* const argMotorController )
{
   uint32_t i = 0;

   Team271Motor* tmpReturnMotor = nullptr;

   LOG_TRACE( "%s - Add Motor Controller",
         GetName().c_str() );

   if( argMotorController == nullptr )
   {
      LOG_ERROR( "%s - Invalid Motor Controller",
                 GetName().c_str() );
   }
   else
   {
      /*
       * Search for the next free Motor Slot
       */
      for( i = 0; i < TEAM271_MOTOR_INDEX_COUNT; i++ )
      {
         if( MotorControllers_[i] == nullptr )
         {
            /*
             * Motor is free
             *
             * Add the Motor
             */
            MotorControllers_[i] = argMotorController;

            /*
             * Check if this is the Master Motor
             * If not, Force to Follower and Set Master ID
             */
            if( ( i != TEAM271_MOTOR_INDEX_MASTER ) && ( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] != nullptr ) )
            {
               MotorControllers_[i]->SetMaster( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] );
            }

            /*
             * Set Return Motor
             */
            tmpReturnMotor = MotorControllers_[i];

            /*
             * Break out of the loop
             */
            break;
         }
      }

      if( i >= TEAM271_MOTOR_INDEX_COUNT )
      {
         LOG_ERROR( "%s - No Free Motor Slots",
                    GetName().c_str() );
      }
   }

   return tmpReturnMotor;
}

void Team271Transmission::ShiftGears( const TEAM271_SHIFTER_INDEX argShifterIndex,
                                      const eGear argGear )
{
   if( IsShifting() == kIsShifting )
   {
      LOG_WARN( "%s - Waiting for a shift to end: %d",
                GetName().c_str(),
                argShifterIndex );
   }
   else if( argShifterIndex >= TEAM271_SHIFTER_INDEX_COUNT )
   {
      LOG_ERROR( "%s - Invalid Shifter Index: %d",
                 GetName().c_str(),
                 argShifterIndex );
   }
   else if( Shifters_[argShifterIndex] == nullptr )
   {
      LOG_ERROR( "%s - Shifter Doesn't Exist: %d",
                 GetName().c_str(),
                 argShifterIndex );
   }
   else if( Shifters_[argShifterIndex]->GetGear() == argGear )
   {
      LOG_NOTICE( "%s - Already in Gear: %d | %d | %s",
                  GetName().c_str(),
                  argShifterIndex,
                  argGear,
                  Shifters_[argShifterIndex]->GetGearName().c_str() );
   }
   else
   {
      /*
       * Only apply a speed modifier if the Transmission is in Percent Output or Velocity
       */
      if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] != nullptr )
      {
         if( ( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->GetControlMode() == TEAM271_CONTROL_MODE_PERCENT_OUTPUT )
               || ( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->GetControlMode() == TEAM271_CONTROL_MODE_VELOCITY ) )
         {
            if( Shifters_[argShifterIndex]->GetSpeedModifierTime() > 0 )
            {
               /*
                * Store the Time to wait
                * Get the Current Time
                * Calculate the End Time
                */
               Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT,
                      Shifters_[argShifterIndex]->GetSpeedModifierTime() );

               Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_WAIT_MODIFIER,
                      Shifters_[argShifterIndex]->GetSpeedModifier() );
            }
         }
      }

      Shifters_[argShifterIndex]->SetGear( argGear );
   }
}

void Team271Transmission::Stop( void )
{
   /*
    * Check if there is a valid Master Controller
    */
   if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER] == nullptr )
   {
      LOG_ERROR( "%s - No Master Motor Controller",
                 GetName().c_str() );
   }

   /*
    * Check to make sure the Master isn't a follower
    */
   else if( MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->GetControlMode() == TEAM271_CONTROL_MODE_FOLLOWER )
   {
      LOG_WARN( "%s - Trying to Stop a Follower Transmission",
                GetName().c_str() );
   }

   /*
    * Apply Changes
    */
   else
   {
      MotorControllers_[TEAM271_MOTOR_INDEX_MASTER]->Stop();
   }
}

void Team271Transmission::SetOutput( const float argSet,
                                     const float argSetModifier,
                                     const float argFeedFwd )
{
   /*
    * - Store NextValue
    * - Store Modifier
    * - Store Next Arb Feed Fwd
    */
   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_NEXT_VALUE,
          argSet );

   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_ARB_FF_NEXT,
          argFeedFwd );

   Write( t271_transmission_data_index::T271_TRANSMISSION_DATA_MODIFIER,
          argSetModifier );
}

#endif

