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

#ifndef TEAM271_ROBOT_H_
#define TEAM271_ROBOT_H_

typedef struct
{
   uint32_t enabled : 1;
   uint32_t autonomous : 1;
   uint32_t test : 1;
   uint32_t eStop : 1;
   uint32_t fmsAttached : 1;
   uint32_t dsAttached : 1;
   uint32_t control_reserved : 26;
} T271_ControlWord;

typedef enum
{
   HAL_AllianceStationID_kInvalid,

   HAL_AllianceStationID_kRed1,
   HAL_AllianceStationID_kRed2,
   HAL_AllianceStationID_kRed3,

   HAL_AllianceStationID_kBlue1,
   HAL_AllianceStationID_kBlue2,
   HAL_AllianceStationID_kBlue3,
} T271_AllianceStationID;

typedef enum
{
   HAL_kMatchType_none,
   HAL_kMatchType_practice,
   HAL_kMatchType_qualification,
   HAL_kMatchType_elimination,
} T271_MatchType;

typedef struct
{
   char eventName[64];
   T271_MatchType matchType;
   uint16_t matchNumber;
   uint8_t replayNumber;
   uint8_t gameSpecificMessage[64];
   uint16_t gameSpecificMessageSize;
} T271_MatchInfo;

class t271_robot_string_data_index : public t271_network_string_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_ROBOT_STRING_DATA_EVENT_NAME = t271_network_string_data_index::T271_STRING_DATA_COUNT,

      T271_ROBOT_STRING_DATA_GAME_MESSAGE,

      T271_DATA_COUNT,
   } t271_robot_string_data_index_t;
};

class t271_robot_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_ROBOT_DATA_VOLT_RIO = t271_base_data_index::T271_DATA_COUNT,

      T271_ROBOT_DATA_VOLT_PDP_BUS,

      T271_ROBOT_DATA_VOLT_PCM_BUS,

      T271_ROBOT_DATA_VOLT_CRIT,
      T271_ROBOT_DATA_VOLT_WARN,

      T271_ROBOT_DATA_COMP_FAULT_CURRENT_HIGH,
      T271_ROBOT_DATA_COMP_FAULT_SHORTED,
      T271_ROBOT_DATA_COMP_FAULT_NOT_CONNECTED,

      T271_ROBOT_DATA_COMP_LOW_PRESSURE,

      T271_ROBOT_DATA_ENABLED,
      T271_ROBOT_DATA_AUTO,
      T271_ROBOT_DATA_TEST,
      T271_ROBOT_DATA_ESTOP,
      T271_ROBOT_DATA_FMS_ATTACHED,
      T271_ROBOT_DATA_DS_ATTACHED,

      T271_ROBOT_DATA_MATCH_TYPE,
      T271_ROBOT_DATA_MATCH_NUM,
      T271_ROBOT_DATA_REPLAY_NUM,
      T271_ROBOT_DATA_GAME_MESSAGE_SIZE,

      T271_ROBOT_DATA_CAN_UTILIZATION,
      T271_ROBOT_DATA_CAN_BUS_OFF_COUNT,
      T271_ROBOT_DATA_CAN_TX_FULL_COUNT,
      T271_ROBOT_DATA_CAN_RX_ERROR_COUNT,
      T271_ROBOT_DATA_CAN_TX_ERROR_COUNT,

      T271_DATA_COUNT,
   } t271_robot_data_index_t;
};

class Team271Robot : public Team271Base
{
protected:
   /*
    *
    * Variables
    *
    */
   T271_ControlWord ControlWord_;
   float TimeLastControlWordUpdate_ = 0;

   T271_MatchInfo MatchInfo_;
   float TimeLastMatchInfoUpdate_ = 0;

   can_status_t CANStatus_;
   float TimeLastCANStatusUpdate_ = 0;

public:
   Team271Robot( void );

   ~Team271Robot( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   /*
    *
    * Robot
    *
    */
   void RobotInit( const bool argIsParent = false );

   void PreRobotPeriodic( const bool argIsParent = false );

   void PostRobotPeriodic( const bool argIsParent = false );

   /*
    *
    * Robot State
    *
    */
   void UpdateInput( const bool argIsParent = false ) override;

   void ClearStickyFaults( const bool argIsParent = false );

   /*
    *
    * Control Word
    *
    */
   void UpdateControlWord( void );

   bool IsEnabled( void );

   bool IsDisabled( void );

   bool IsAutonomous( void );

   bool IsTeleop( void );

   bool IsTest( void );

   bool IsDSAttached( void );

   bool IsFMSAttached( void );

   /*
    *
    * Match Info
    *
    */
   void UpdateMatchInfo( void );

   string GetGameSpecificMessage( void ) const;

   string GetEventName( void ) const;

   match_type_t GetMatchType( void ) const;

   uint32_t GetMatchNumber( void ) const;

   uint32_t GetReplayNumber( void ) const;

   static alliance_t GetAlliance( void );

   static uint32_t GetLocation( void );

   /*
    *
    * HAL Access
    *
    */
   static float GetMatchTime( void );

   static int32_t GetFPGAVersion( void );

   static int64_t GetFPGARevision( void );

   static uint64_t GetFPGATime( void );

   static float GetTime( const float argTimeSecsLast = 0 );

   static float GetTimeDelta( const float argTimeSecsLast = 0 );

   static uint32_t CompareTime( float& argTimeSecsLast,
                                const float argTimeSecsUpdateRate );

   static bool GetUserButton( void );

   static bool IsSysActive( void );

   static bool IsBrownedOut( void );

   /*
    * Input
    */
   static float GetVinVoltage( void );

   static float GetInputCurrent( void );

   /*
    * 3.3V
    */
   static float GetVoltage3V3( void );

   static float GetCurrent3V3( void );

   static bool GetEnabled3V3( void );

   static int32_t GetFaultCount3V3( void );

   /*
    * 5V
    */
   static float GetVoltage5V( void );

   static float GetCurrent5V( void );

   static bool GetEnabled5V( void );

   static int32_t GetFaultCount5V( void );

   static int32_t GetFaultCount5V_UnderVoltage( void );

   /*
    * 6V
    */
   static float GetVoltage6V( void );

   static float GetCurrent6V( void );

   static bool GetEnabled6V( void );

   static int32_t GetFaultCount6V( void );

   /*
    * CAN
    */
   const can_status_t* GetCANStatus( void );
};

#endif /* TEAM271_ROBOT_H_ */
