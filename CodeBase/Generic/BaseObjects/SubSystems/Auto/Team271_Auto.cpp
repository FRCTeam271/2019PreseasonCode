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

#if defined(T271_AUTO_ENABLE)

Team271Auto::Team271Auto( void )
      : Team271Base( nullptr,
                     string( "Auto" ) )
{
   LOG_TRACE( "%s - Constructor Start",
              "Team271Auto" );

   LOG_TRACE( "%s - Constructor End",
              "Team271Auto");
}

void Team271Auto::RobotInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::RobotInit();

   /*
    * Create Auto Modes
    */
   /*CreateAuto();*/
}

void Team271Auto::AutonomousInit( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousInit();

   /*
    * Select Auto Mode
    */
#if defined( ENABLE_NETWORK_DATA )
   _AutoMode = NTE_Mode_.GetDouble( _AutoMode );
   _AutoModeDelay = NTE_Delay_.GetDouble( _AutoModeDelay );
#endif

   LOG_MATCH2("_AutoMode: %d\r\n",
              SharedAutoData_.Mode_ );

   LOG_MATCH2( "_AutoModeDelay: %d\r\n",
               SharedAutoData_.Delay_ );

   /*
    * Reset Auto Modes
    */
   /*
    _AutoInit = 0;
    _CurrentAutoAction = 0;
    */

   /*
    * Create Auto Modes
    */
   /*CreateAuto();*/
}

double priorAutospeed = 0;
double numberArray[9];

void Team271Auto::AutonomousPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousPeriodic();

   /*
    * Run Auto
    */
   if( SharedAutoData_.Mode_ < AUTO_MAX_MODES )
   {
      /*
       if( _AutoInit == 0 )
       {
       _Drive->ShiftGears( _AutoActions[_AutoMode][0]._DriveGear );

       _AutoInit = 1;
       }

       if( _CurrentAutoAction < AUTO_MAX_ACTIONS )
       {
       const eAutoActionState tmpActionState = _AutoActions[_AutoMode][_CurrentAutoAction].Process();
       if( (tmpActionState == eAutoActionStateDone) || (tmpActionState == eAutoActionStateDoneTimeout) )
       {
       ++_CurrentAutoAction;
       }
       }
       */
   }

#if defined(T271_DRIVE_ENABLE)
#if defined(ENABLE_NETWORK_DATA)
   nt::NetworkTableEntry autoSpeedEntry = _NetworkTables.GetEntry("/robot/autospeed");
   nt::NetworkTableEntry telemetryEntry = _NetworkTables.GetEntry("/robot/telemetry");

   // Retrieve values to send back before telling the motors to do something
   double now = Team271Robot::GetTime();

   double leftPosition = _Drive->GetDriverPosition();
   double leftRate = _Drive->GetDriverRate();

   double rightPosition = _Drive->GetPassengerPosition();
   double rightRate = _Drive->GetPassengerRate();

   double battery = _PDP->GetBusVoltage();

   double leftMotorVolts = _Drive->GetDriverOutputVoltage();
   double rightMotorVolts = _Drive->GetPassengerOutputVoltage();

   // Retrieve the commanded speed from NetworkTables
   double autospeed = autoSpeedEntry.GetDouble(0);
   priorAutospeed = autospeed;

   // command motors to do things
   _Drive->Drive( autospeed,
         autospeed * 0.82 );

   // send telemetry data array back to NT
   numberArray[0] = now;
   numberArray[1] = battery;
   numberArray[2] = autospeed;
   numberArray[3] = leftMotorVolts;
   numberArray[4] = rightMotorVolts;
   numberArray[5] = leftPosition;
   numberArray[6] = rightPosition;
   numberArray[7] = leftRate;
   numberArray[8] = rightRate;

   //nt::ArrayRef<double> numberArrayRef;
   //numberArrayRef =
   telemetryEntry.SetDoubleArray(numberArray);
#endif
#endif
}

void Team271Auto::CreateAuto( void )
{
   /*
    * Reset Auto Modes
    */
   for( uint32_t i = 0; i < AUTO_MAX_MODES; i++ )
   {
      _AutoModes[i].Reset();
   }

   /*
    *
    * Create Auto Modes
    *
    */
   Team271_Auto0();
   Team271_Auto1();
   Team271_Auto2();
   Team271_Auto3();
   Team271_Auto4();
   Team271_Auto5();
   Team271_Auto6();
   Team271_Auto7();
   Team271_Auto8();
   Team271_Auto9();
}

Team271AutoMode* Team271Auto::GetAutoMode( const uint32_t argAutoMode )
{
   return &_AutoModes[argAutoMode];
}

uint32_t Team271Auto::IsLeft( void ) const
{
   uint32_t tmpMakeLeft = 0;

   if( Robot_ != nullptr )
   {
      if( Robot_->GetGameSpecificMessage()[0] == 'L' )
      {
         tmpMakeLeft = 1;
      }
   }
   else
   {
      tmpMakeLeft = 0;
   }

   return tmpMakeLeft;
}
#endif
