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

Team271Robot::Team271Robot( void )
      : Team271Base( nullptr,
                     "Robot",
                     t271_robot_data_index::T271_DATA_COUNT )
{
   memset( &ControlWord_,
           0,
           sizeof( ControlWord_ ) );

   memset( &MatchInfo_,
           0,
           sizeof( MatchInfo_ ) );

   memset( &CANStatus_,
           0,
           sizeof( CANStatus_ ) );
}

Team271Robot::~Team271Robot( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Robot::GetClassName( void ) const
{
   return "Team271Robot";
}

/*
 *
 * Robot
 *
 */
void Team271Robot::RobotInit( const bool argIsParent )
{
   /*
    * Enable the Compressor
    */
#if defined( T271_PCM_ENABLE )
   Team271PCM::getInstance( 0 ).SetCompressorEnable( true );
#endif

   UpdateMatchInfo();
   UpdateControlWord();
}

void Team271Robot::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Base Call
    */
   Team271Base::PreRobotPeriodic();

   /*
    * Update the Control Word
    */
   if( Team271Robot::CompareTime( TimeLastControlWordUpdate_,
                                  0.050f ) == 1 )
   {
      UpdateControlWord();
   }

   /*
    * Update the CAN Status
    */
#if defined( T271_FRC_NETCOM_ENABLE )
   int32_t tmpStatus = 0;

   FRC_NetworkCommunication_CANSessionMux_getCANStatus( &CANStatus_.percentBusUtilization,
                                                        &CANStatus_.busOffCount,
                                                        &CANStatus_.txFullCount,
                                                        &CANStatus_.receiveErrorCount,
                                                        &CANStatus_.transmitErrorCount,
                                                        &tmpStatus );

#endif

   Write( t271_robot_data_index::T271_ROBOT_DATA_CAN_UTILIZATION,
          CANStatus_.percentBusUtilization );

   Write( t271_robot_data_index::T271_ROBOT_DATA_CAN_BUS_OFF_COUNT,
          CANStatus_.busOffCount );

   Write( t271_robot_data_index::T271_ROBOT_DATA_CAN_TX_FULL_COUNT,
          CANStatus_.txFullCount );

   Write( t271_robot_data_index::T271_ROBOT_DATA_CAN_RX_ERROR_COUNT,
          CANStatus_.receiveErrorCount );

   Write( t271_robot_data_index::T271_ROBOT_DATA_CAN_TX_ERROR_COUNT,
          CANStatus_.transmitErrorCount );

#if defined( T271_PDP_ENABLE )
   Team271PDP::getInstance().PreRobotPeriodic();
#endif

#if defined( T271_PCM_ENABLE )
   Team271PCM::getInstance( 0 ).PreRobotPeriodic();
#endif

   /*
    * Update Shared Data
    */
   Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_RIO,
          GetVinVoltage() );

   Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_PDP_BUS,
          Team271PDP::getInstance().GetBusVoltage() );

   Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_PCM_BUS,
          Team271PCM::getInstance( 0 ).GetBusVoltage() );

   /*
    *
    * Voltage Warnings
    * - Check if the System Voltage is Critical
    * - Check if the System Voltage is approaching a Critical Level
    *
    */
   if( Read<float>( t271_robot_data_index::T271_ROBOT_DATA_VOLT_PDP_BUS ) <= VOLTAGE_CRITICAL )
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_CRIT,
             1U );

      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_WARN,
             1U );
   }
   else if( Read<float>( t271_robot_data_index::T271_ROBOT_DATA_VOLT_PDP_BUS ) <= VOLTAGE_WARNING )
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_CRIT,
             0U );

      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_WARN,
             1U );
   }

   else
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_CRIT,
             0U );

      Write( t271_robot_data_index::T271_ROBOT_DATA_VOLT_WARN,
             0U );
   }

   /*
    * Compressor Warnings
    */
#if defined( T271_PCM_ENABLE )
   if( Team271PCM::getInstance( 0 ).GetCompressorFault_TooHighCurrent() == true )
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_CURRENT_HIGH,
             1U );
   }
   else
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_CURRENT_HIGH,
             0U );
   }

   if( Team271PCM::getInstance( 0 ).GetCompressorFault_Shorted() == true )
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_SHORTED,
             1U );
   }
   else
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_SHORTED,
             0U );
   }

   if( Team271PCM::getInstance( 0 ).GetCompressorFault_NotConnected() == true )
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_NOT_CONNECTED,
             1U );
   }
   else
   {
      Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_FAULT_NOT_CONNECTED,
             0U );
   }

   /*
    * GetPressureSwitchValue = False when the pressure is Low
    */
   Write( t271_robot_data_index::T271_ROBOT_DATA_COMP_LOW_PRESSURE,
          !Team271PCM::getInstance( 0 ).GetPressureSwitch() );
#endif

   //UpdateMatchInfo();
}

void Team271Robot::PostRobotPeriodic( const bool argIsParent )
{
#if defined( T271_PCM_ENABLE )
   Team271PCM::getInstance( 0 ).PostRobotPeriodic();
#endif

   /*
    * Base Call
    */
   Team271Base::PostRobotPeriodic();
}

/*
 *
 * Robot State
 *
 */
void Team271Robot::UpdateInput( const bool argIsParent )
{
#if defined( T271_INPUT_ENABLE )
   /*
    * Check if the User Button on the RIO was pressed
    * This will only trigger once
    */
   if( Team271Input::Instance().IsUserButtonAction() == true )
   {
      DEBUG_PRINT( "Main",
                   "User Button Pressed - Clearing ALL Sticky Faults" );

      ClearStickyFaults();
   }

   /*
    * Check if we should Disable the Compressor
    */
#if defined( T271_PCM_ENABLE )
   if( Team271Input::Instance().GetButtonState( eOperatorRight,
                                                8 ) != Team271Input::Instance().GetButtonStatePrev( eOperatorRight,
                                                                                                    8 ) )
   {
      if( Team271Input::Instance().GetButtonState( eOperatorRight,
                                                   8 ) != 0 )
      {
         if( Team271PCM::getInstance( 0 ).GetCompressorEnable() == false )
         {
            Team271PCM::getInstance( 0 ).SetCompressorEnable( true );
         }
         else
         {
            Team271PCM::getInstance( 0 ).SetCompressorEnable( false );
         }
      }
   }
#endif
#endif
}

void Team271Robot::ClearStickyFaults( const bool argIsParent )
{
#if defined( T271_PDP_ENABLE )
   Team271PDP::getInstance().ClearStickyFaults();
#endif

#if defined( T271_PCM_ENABLE )
   Team271PCM::getInstance( 0 ).ClearStickyFaults();
#endif
}

/*
 *
 * Match Info
 *
 */
void Team271Robot::UpdateControlWord( void )
{
#if defined(T271_FRC_NETCOM_ENABLE)
   std::memset( &ControlWord_,
                0,
                sizeof(T271_ControlWord) );

   FRC_NetworkCommunication_getControlWord( (ControlWord_t*)&ControlWord_ );
#endif

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_ENABLED,
                   ControlWord_.enabled );

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_AUTO,
                   ControlWord_.autonomous );

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_TEST,
                   ControlWord_.test );

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_ESTOP,
                   ControlWord_.eStop );

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_FMS_ATTACHED,
                   ControlWord_.fmsAttached );

   Write<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_DS_ATTACHED,
                   ControlWord_.dsAttached );
}

bool Team271Robot::IsEnabled( void )
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_ENABLED ) && Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_DS_ATTACHED );
}

bool Team271Robot::IsDisabled( void )
{
   return !( Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_ENABLED ) && Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_DS_ATTACHED ) );
}

bool Team271Robot::IsAutonomous( void )
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_AUTO );
}

bool Team271Robot::IsTeleop( void )
{
   return !( Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_AUTO ) || Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_TEST ) );
}

bool Team271Robot::IsTest( void )
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_TEST );
}

bool Team271Robot::IsDSAttached( void )
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_DS_ATTACHED );
}

bool Team271Robot::IsFMSAttached( void )
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_FMS_ATTACHED );
}

void Team271Robot::UpdateMatchInfo( void )
{
#if defined(T271_FRC_NETCOM_ENABLE)
   FRC_NetworkCommunication_getMatchInfo( MatchInfo_.eventName,
                                          reinterpret_cast<MatchType_t*>( &MatchInfo_.matchType ),
                                          &MatchInfo_.matchNumber,
                                          &MatchInfo_.replayNumber,
                                          MatchInfo_.gameSpecificMessage,
                                          &MatchInfo_.gameSpecificMessageSize );
#endif

   *( std::end( MatchInfo_.eventName ) - 1 ) = '\0';
   *( std::end( MatchInfo_.gameSpecificMessage ) - 1 ) = '\0';

   /*
    * Update Match Info
    */
   Write( t271_robot_data_index::T271_ROBOT_DATA_MATCH_TYPE,
          MatchInfo_.matchType );

   Write( t271_robot_data_index::T271_ROBOT_DATA_MATCH_NUM,
          MatchInfo_.matchNumber );

   Write( t271_robot_data_index::T271_ROBOT_DATA_REPLAY_NUM,
          MatchInfo_.replayNumber );

   Write( t271_robot_data_index::T271_ROBOT_DATA_GAME_MESSAGE_SIZE,
          MatchInfo_.gameSpecificMessageSize );

   if( MatchInfo_.eventName != nullptr )
   {
      Write( t271_robot_string_data_index::T271_ROBOT_STRING_DATA_GAME_MESSAGE,
             std::string( reinterpret_cast<const char*>( MatchInfo_.eventName ) ) );
   }

   if( MatchInfo_.gameSpecificMessage != nullptr )
   {
      Write( t271_robot_string_data_index::T271_ROBOT_STRING_DATA_GAME_MESSAGE,
             std::string( reinterpret_cast<const char*>( MatchInfo_.gameSpecificMessage ) ) );
   }
}

string Team271Robot::GetGameSpecificMessage( void ) const
{
   return ReadString( t271_robot_string_data_index::T271_ROBOT_STRING_DATA_GAME_MESSAGE );
}

string Team271Robot::GetEventName( void ) const
{
   return ReadString( t271_robot_string_data_index::T271_ROBOT_STRING_DATA_EVENT_NAME );
}

match_type_t Team271Robot::GetMatchType( void ) const
{
   return Read<match_type_t>( t271_robot_data_index::T271_ROBOT_DATA_MATCH_TYPE );
}

uint32_t Team271Robot::GetMatchNumber( void ) const
{
   return Read<uint16_t>( t271_robot_data_index::T271_ROBOT_DATA_MATCH_NUM );
}

uint32_t Team271Robot::GetReplayNumber( void ) const
{
   return Read<uint8_t>( t271_robot_data_index::T271_ROBOT_DATA_REPLAY_NUM );
}

alliance_t Team271Robot::GetAlliance( void )
{
   alliance_t tmpReturn = kInvalid;

   T271_AllianceStationID tmpAllianceStationID = HAL_AllianceStationID_kInvalid;

#if defined(T271_FRC_NETCOM_ENABLE)
   FRC_NetworkCommunication_getAllianceStation( reinterpret_cast<AllianceStationID_t*>( &tmpAllianceStationID ) );
#endif

   switch( tmpAllianceStationID )
   {
      case HAL_AllianceStationID_kRed1:
      case HAL_AllianceStationID_kRed2:
      case HAL_AllianceStationID_kRed3:
         tmpReturn = kRed;
         break;

      case HAL_AllianceStationID_kBlue1:
      case HAL_AllianceStationID_kBlue2:
      case HAL_AllianceStationID_kBlue3:
         tmpReturn = kBlue;
         break;

      default:
         tmpReturn = kInvalid;
         break;
   }

   return tmpReturn;
}

uint32_t Team271Robot::GetLocation( void )
{
   uint32_t tmpReturn = 0;

   T271_AllianceStationID tmpAllianceStationID = HAL_AllianceStationID_kInvalid;

#if defined(T271_FRC_NETCOM_ENABLE)
   FRC_NetworkCommunication_getAllianceStation( reinterpret_cast<AllianceStationID_t*>( &tmpAllianceStationID ) );
#endif

   switch( tmpAllianceStationID )
   {
      case HAL_AllianceStationID_kRed1:
      case HAL_AllianceStationID_kBlue1:
         tmpReturn = 1;
         break;

      case HAL_AllianceStationID_kRed2:
      case HAL_AllianceStationID_kBlue2:
         tmpReturn = 2;
         break;

      case HAL_AllianceStationID_kRed3:
      case HAL_AllianceStationID_kBlue3:
         tmpReturn = 3;
         break;

      default:
         tmpReturn = 0;
         break;
   }

   return tmpReturn;
}

float Team271Robot::GetMatchTime( void )
{
   float tmpTime = 0;

#if defined(T271_FRC_NETCOM_ENABLE)
   /*
    * Approx Time Remaining in Auto or Tele
    */
   FRC_NetworkCommunication_getMatchTime( &tmpTime );
#endif

   return tmpTime;
}

int32_t Team271Robot::GetFPGAVersion( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Global_->readVersion( &tmpStatus );
#else
   return 0;
#endif
}

int64_t Team271Robot::GetFPGARevision( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Global_->readRevision( &tmpStatus );
#else
   return 0;
#endif
}

uint64_t Team271Robot::GetFPGATime( void )
{
   uint64_t tmpTime = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   if( Global_ != nullptr )
   {
      const uint64_t upper1 = Global_->readLocalTimeUpper( &tmpStatus );
      uint32_t lower = Global_->readLocalTime( &tmpStatus );
      const uint64_t upper2 = Global_->readLocalTimeUpper( &tmpStatus );

      if( tmpStatus == 0 )
      {
         if( upper1 != upper2 )
         {
            // Rolled over between the lower call, reread lower
            lower = Global_->readLocalTime( &tmpStatus );
         }

         if( tmpStatus == 0 )
         {
            tmpTime = ( upper2 << 32 ) + lower;
         }
      }
   }
#endif

   return tmpTime;
}

float Team271Robot::GetTime( const float argTimeSecsLast )
{
   float tmpTimeCurrent = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   /*
    * Get the FPGA Time ( Time is in microseconds )
    *    - Convert to Seconds
    */
   tmpTimeCurrent = GetFPGATime() * 1.0e-6f;
   /*
    static auto start_time = std::chrono::high_resolution_clock::now();

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    tmpTimeCurrent = std::chrono::duration_cast<std::chrono::microseconds>(time).count() * 1.0e-6f;
    */
#else
   static auto start_time = std::chrono::high_resolution_clock::now();

   auto end_time = std::chrono::high_resolution_clock::now();
   auto time = end_time - start_time;

   tmpTimeCurrent = std::chrono::duration_cast<std::chrono::microseconds>(time).count() * 1.0e-6f;

   /*
    tmpTimeCurrent =(((now.tv_sec - start.tv_sec) * 1000000) +
    (now.tv_usec - start.tv_usec)) / 1000;
    */
#endif

   if( argTimeSecsLast > tmpTimeCurrent )
   {
      tmpTimeCurrent += kRolloverTime;
   }

   return tmpTimeCurrent;
}

float Team271Robot::GetTimeDelta( const float argTimeSecsLast )
{
   /*
    * Calculate the Delta
    */
   return GetTime( argTimeSecsLast ) - argTimeSecsLast;
}

uint32_t Team271Robot::CompareTime( float& argTimeSecsLast,
                                    const float argTimeSecsUpdateRate )
{
   uint32_t tmpReturn = 0;

   /*
    * Get the FPGA Time ( Time is in microseconds )
    *    - Convert to Seconds
    */
   const float tmpTimeCurrent = GetTime( argTimeSecsLast );

   /*
    * Check if the Time Wrapped Around
    */
   if( ( tmpTimeCurrent - argTimeSecsLast ) > argTimeSecsUpdateRate )
   {
      argTimeSecsLast = GetTime();

      tmpReturn = 1;
   }

   return tmpReturn;
}

bool Team271Robot::GetUserButton( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Global_->readUserButton( &tmpStatus );
#else
   return 0;
#endif
}

bool Team271Robot::IsSysActive( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Watchdog_->readStatus_SystemActive( &tmpStatus );
#else
   return false;
#endif
}

bool Team271Robot::IsBrownedOut( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return !( Watchdog_->readStatus_PowerAlive( &tmpStatus ) );
#else
   return false;
#endif
}

/*
 *
 * Input
 *
 */
float Team271Robot::GetVinVoltage( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readVinVoltage( &tmpStatus ) / 4.096f * 0.025733f - 0.029f;
#else
   return 0;
#endif
}

float Team271Robot::GetInputCurrent( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readVinCurrent( &tmpStatus ) / 4.096f * 0.017042f - 0.071f;
#else
   return 0;
#endif
}

/*
 *
 * 3.3V
 *
 */
float Team271Robot::GetVoltage3V3( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserVoltage3V3( &tmpStatus ) / 4.096f * 0.004902f - 0.01f;
#else
   return 0;
#endif
}

float Team271Robot::GetCurrent3V3( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserCurrent3V3( &tmpStatus ) / 4.096f * 0.002486f - 0.003f;
#else
   return 0;
#endif
}

bool Team271Robot::GetEnabled3V3( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readStatus_User3V3( &tmpStatus ) == 4;
#else
   return false;
#endif
}

int32_t Team271Robot::GetFaultCount3V3( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return static_cast<int32_t>( Power_->readFaultCounts_OverCurrentFaultCount3V3( &tmpStatus ) );
#else
   return 0;
#endif
}

/*
 *
 * 5V
 *
 */
float Team271Robot::GetVoltage5V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserVoltage5V( &tmpStatus ) / 4.096f * 0.005962f - 0.013f;
#else
   return 0;
#endif
}

float Team271Robot::GetCurrent5V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserCurrent5V( &tmpStatus ) / 4.096f * 0.001996f - 0.002f;
#else
   return 0;
#endif
}

bool Team271Robot::GetEnabled5V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readStatus_User5V( &tmpStatus ) == 4;
#else
   return false;
#endif
}

int32_t Team271Robot::GetFaultCount5V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return static_cast<int32_t>( Power_->readFaultCounts_OverCurrentFaultCount5V( &tmpStatus ) );
#else
   return 0;
#endif
}

int32_t Team271Robot::GetFaultCount5V_UnderVoltage( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return static_cast<int32_t>( Power_->readFaultCounts_UnderVoltageFaultCount5V( &tmpStatus ) );
#else
   return 0;
#endif
}

/*
 *
 * 6V
 *
 */
float Team271Robot::GetVoltage6V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserVoltage6V( &tmpStatus ) / 4.096f * 0.007019f - 0.014f;
#else
   return 0;
#endif
}

float Team271Robot::GetCurrent6V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readUserCurrent6V( &tmpStatus ) / 4.096f * 0.005566f - 0.009f;
#else
   return 0;
#endif
}

bool Team271Robot::GetEnabled6V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return Power_->readStatus_User6V( &tmpStatus ) == 4;
#else
   return false;
#endif
}

int32_t Team271Robot::GetFaultCount6V( void )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   return static_cast<int32_t>( Power_->readFaultCounts_OverCurrentFaultCount6V( &tmpStatus ) );
#else
   return 0;
#endif
}

/*
 *
 * CAN
 *
 */
const can_status_t* Team271Robot::GetCANStatus( void )
{
   return &CANStatus_;
}

