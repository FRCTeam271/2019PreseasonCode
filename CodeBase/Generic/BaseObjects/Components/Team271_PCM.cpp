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

#if defined(T271_PCM_ENABLE)

static Team271PCM PCMInstance0_( "PCM0",
                                 0 );

static Team271PCM PCMInstance1_( "PCM1",
                                 1 );

Team271PCM& Team271PCM::getInstance( const uint32_t argID )
{
   if( argID == 1 )
   {
      PCMInstance1_.Setup();

      return PCMInstance1_;
   }

   PCMInstance0_.Setup();

   return PCMInstance0_;
}

Team271PCM::Team271PCM( const string& argName,
                        const int32_t argModuleNumber )
      : Team271Base( nullptr,
                     argName ),
        ModuleID_( argModuleNumber )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   /*
    * Setup Control TX
    */
   memset( &TXControl_,
           0,
           sizeof( TXControl_ ) );

   TXControl_.ArbID_ = CONTROL_1 | ModuleID_;

   /*
    * Setup Status RX
    */
   memset( &RXStatus_,
           0,
           sizeof( RXStatus_ ) );

   RXStatus_.ArbID_ = STATUS_1 | ModuleID_;

   /*
    * Setup Faults RX
    */
   memset( &RXFaults_,
           0,
           sizeof( RXFaults_ ) );

   RXFaults_.ArbID_ = STATUS_SOL_FAULTS | ModuleID_;

   /*
    * Setup Debug RX
    */
   memset( &RXDebug_,
           0,
           sizeof( RXDebug_ ) );

   RXDebug_.ArbID_ = STATUS_DEBUG | ModuleID_;

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271PCM::~Team271PCM( void )
{
#if defined(T271_PCM_CAN_ENABLE)
   if( ( ModuleID_ >= 0 ) && ( ModuleID_ >= kNumPCMModules ) )
   {
#if defined( T271_FRC_NETCOM_ENABLE )
      int32_t tmpStatus = 0;
      FRC_NetworkCommunication_CANSessionMux_sendMessage( TXControl_.ArbID_,
                                                          (uint8_t*)&TXControl_.Data_,
                                                          8,
                                                          0,
                                                          &tmpStatus );
#endif
   }
#endif
}

/*
 *
 * Getters
 *
 */
const string Team271PCM::GetClassName( void ) const
{
   return "Team271PCM";
}

float Team271PCM::GetBusVoltage( void ) const
{
   return RXStatus_.Data_.battVoltage * 0.05f + 4.0f; /* 50mV per unit plus 4V. */
}

float Team271PCM::GetSolenoidRailVoltage( void ) const
{
   uint32_t raw = RXStatus_.Data_.solenoidVoltageTop8;

   raw <<= 2;
   raw |= RXStatus_.Data_.solenoidVoltageBtm2;

   return (float)raw * 0.03125f; /* 5.5 fixed pt value in Volts */
}

bool Team271PCM::GetPressureSwitch( void ) const
{
   return RXStatus_.Data_.pressureSwitchEn;
}

bool Team271PCM::GetCompressorEnable( void ) const
{
   return RXStatus_.Data_.compressorOn;
}

float Team271PCM::GetCompressorCurrent( void ) const
{
   uint32_t raw = RXStatus_.Data_.compressorCurrentTop6;

   raw <<= 4;
   raw |= RXStatus_.Data_.solenoidVoltageBtm2;

   return (float)raw * 0.03125f; /* 5.5 fixed pt value in Amps */
}

bool Team271PCM::GetCompressorFault_TooHighCurrent( void ) const
{
   return RXStatus_.Data_.faultCompCurrentTooHigh;
}

bool Team271PCM::GetCompressorFault_Shorted( void ) const
{
   return RXStatus_.Data_.Fault_dItooHigh;
}

bool Team271PCM::GetCompressorFault_NotConnected( void ) const
{
   return RXFaults_.Data_.Fault_CompNoCurrent;
}

/*
 *
 * Setters
 *
 */
void Team271PCM::SetCompressorEnable( const bool argEnable )
{
   ClosedLoopEnable_.store( argEnable );
}

void Team271PCM::PreRobotPeriodic( void )
{
#if defined( T271_PCM_CAN_ENABLE )
   if( Team271Robot::CompareTime( TimeCANRXUpdateLast_,
                                  kCANRXUpdateRate ) == 1 )
   {
      int32_t tmpStatus = 0;

      /*
       * Get PCM Status
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &RXStatus_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&RXStatus_.Data_,
                                                             &RXStatus_.Len_,
                                                             &RXStatus_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         RXStatus_.DataLast_ = Team271Robot::GetTimeDelta( RXStatus_.DataLast_ );
      }

      /*
       * Get PCM Solenoid Faults
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &RXFaults_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&RXFaults_.Data_,
                                                             &RXFaults_.Len_,
                                                             &RXFaults_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         RXFaults_.DataLast_ = Team271Robot::GetTimeDelta( RXFaults_.DataLast_ );
      }

      /*
       * Get PCM Debug
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &RXDebug_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&RXDebug_.Data_,
                                                             &RXFaults_.Len_,
                                                             &RXFaults_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         RXDebug_.DataLast_ = Team271Robot::GetTimeDelta( RXDebug_.DataLast_ );
      }
   }
#endif
}

void Team271PCM::PostRobotPeriodic( void )
{
   uint8_t tmpSendControlUpdate = 0;

   /*
    * Update CAN if:
    *    - Control Data has Changed
    *    - Periodically just in case
    */
   if( memcmp( &TXControl_.Data_,
               &TXControl_.DataPrev_,
               sizeof( TXControl_.Data_ ) ) != 0 )
   {
      TimeCANTXUpdateLast_ = Team271Robot::GetTime();

      tmpSendControlUpdate = 1;
   }
   else if( Team271Robot::CompareTime( TimeCANTXUpdateLast_,
                                       kCANTXUpdateRate ) == 1 )
   {
      tmpSendControlUpdate = 1;
   }

   if( tmpSendControlUpdate == 1 )
   {
      /*
       * Update Data
       */
      TXControl_.Data_.solenoidBits = SolenoidBits_.load();
      TXControl_.Data_.closedLoopEnable = ClosedLoopEnable_.load();

      /*
       * Update TX
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      int32_t tmpStatus = 0;
      FRC_NetworkCommunication_CANSessionMux_sendMessage( TXControl_.ArbID_,
                                                          (uint8_t*)&TXControl_.Data_,
                                                          8,
                                                          0,
                                                          &tmpStatus );
#endif

      /*
       * Store the Previous Value Sent
       */
      memcpy( &TXControl_.DataPrev_,
              &TXControl_.Data_,
              sizeof( TXControl_.DataPrev_ ) );
   }
}

/*
 *
 * Robot State
 *
 */
void Team271PCM::ClearStickyFaults( void )
{
#if defined( T271_PCM_CAN_ENABLE )
   int32_t tmpStatus = 0;

   uint8_t pcmSupplemControl[] = { 0,
                                   0,
                                   0,
                                   0x80 }; /* only bit set is ClearStickyFaults */

#if defined( T271_FRC_NETCOM_ENABLE )
   FRC_NetworkCommunication_CANSessionMux_sendMessage( CONTROL_2 | ModuleID_,
                                                       pcmSupplemControl,
                                                       sizeof( pcmSupplemControl ),
                                                       0,
                                                       &tmpStatus );
#endif
#endif
}

/*
 *
 * Ports
 *
 */
void Team271PCM::Setup( void )
{
   if( IsSetup_ == 0 )
   {
      if( ( ModuleID_ >= 0 ) && ( ModuleID_ >= kNumPCMModules ) )
      {
         LOG_ERROR( "%s - Invalid PCM: %d",
                    GetName().c_str(),
                    ModuleID_ );
      }
      else
      {
         IsSetup_ = 1;

#if defined( T271_FRC_NETCOM_ENABLE )
         int32_t tmpStatus = 0;
         FRC_NetworkCommunication_CANSessionMux_sendMessage( TXControl_.ArbID_,
                                                             (uint8_t*)&TXControl_.Data_,
                                                             8,
                                                             kCANPeriod,
                                                             &tmpStatus );
#endif
      }
   }
}

void Team271PCM::Set( const uint32_t argChannelNumber,
                      const uint32_t argValue )
{
   if( argValue == 1 )
   {
      SolenoidBits_.fetch_or( 1ul << argChannelNumber );
      //TXControl_.Data_.solenoidBits |= ( 1ul << ( argChannelNumber ) );
   }
   else
   {
      SolenoidBits_.fetch_and( ~( 1ul << argChannelNumber ) );
      //TXControl_.Data_.solenoidBits &= ~( 1ul << ( argChannelNumber ) );
   }
}

#endif

