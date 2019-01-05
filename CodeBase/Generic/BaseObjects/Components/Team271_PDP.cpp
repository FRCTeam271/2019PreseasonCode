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

#if defined(T271_PDP_ENABLE)

static Team271PDP PDPInstance_( 0 );

Team271PDP& Team271PDP::getInstance( void )
{
   return PDPInstance_;
}

Team271PDP::Team271PDP( const uint32_t argCANID )
      : Team271Base( nullptr,
                     "PDP",
                     t271_pdp_data_index::T271_DATA_COUNT ),
        CANID_( argCANID )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   /*
    * Setup Control TX
    */
   PDPControl_.ArbID_ = CreateCANID( kControl1 );

   /*
    * Setup Status1 RX
    */
   memset( &PDPStatus1_,
           0,
           sizeof( PDPStatus1_ ) );

   PDPStatus1_.ArbID_ = CreateCANID( kStatus1 );

   /*
    * Setup Status2 RX
    */
   memset( &PDPStatus2_,
           0,
           sizeof( PDPStatus2_ ) );

   PDPStatus2_.ArbID_ = CreateCANID( kStatus2 );

   /*
    * Setup Status3 RX
    */
   memset( &PDPStatus3_,
           0,
           sizeof( PDPStatus3_ ) );

   PDPStatus3_.ArbID_ = CreateCANID( kStatus3 );

   /*
    * Setup StatusEnergy RX
    */
   memset( &PDPStatusEnergy_,
           0,
           sizeof( PDPStatusEnergy_ ) );

   PDPStatusEnergy_.ArbID_ = CreateCANID( kStatusEnergy );

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271PDP::~Team271PDP( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271PDP::GetClassName( void ) const
{
   return "Team271PDP";
}

float Team271PDP::GetBusTemp( void ) const
{
   return Read<float>( t271_pdp_data_index::T271_PDP_DATA_BUS_TEMPERATURE );
}

float Team271PDP::GetBusVoltage( void ) const
{
   return Read<float>( t271_pdp_data_index::T271_PDP_DATA_BUS_VOLTAGE );
}

float Team271PDP::GetTotalCurrent( void ) const
{
   return Read<float>( t271_pdp_data_index::T271_PDP_DATA_TOTAL_CURRENT );
}

float Team271PDP::GetTotalPower( void ) const
{
   return Read<float>( t271_pdp_data_index::T271_PDP_DATA_TOTAL_POWER );
}

float Team271PDP::GetTotalEnergy( void ) const
{
   return Read<float>( t271_pdp_data_index::T271_PDP_DATA_TOTAL_ENERGY );
}

float Team271PDP::GetCurrent( const uint32_t argCahnnel ) const
{
   float tmpReturnValue = 0;

   switch( argCahnnel )
   {
      case 0:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan1_h8 ) << 2 ) | PDPStatus1_.Data_.bits.chan1_l2;
         break;
      case 1:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan2_h6 ) << 4 ) | PDPStatus1_.Data_.bits.chan2_l4;
         break;
      case 2:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan3_h4 ) << 6 ) | PDPStatus1_.Data_.bits.chan3_l6;
         break;
      case 3:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan4_h2 ) << 8 ) | PDPStatus1_.Data_.bits.chan4_l8;
         break;
      case 4:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan5_h8 ) << 2 ) | PDPStatus1_.Data_.bits.chan5_l2;
         break;
      case 5:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus1_.Data_.bits.chan6_h6 ) << 4 ) | PDPStatus1_.Data_.bits.chan6_l4;
         break;
      case 6:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan7_h8 ) << 2 ) | PDPStatus2_.Data_.bits.chan7_l2;
         break;
      case 7:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan8_h6 ) << 4 ) | PDPStatus2_.Data_.bits.chan8_l4;
         break;
      case 8:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan9_h4 ) << 6 ) | PDPStatus2_.Data_.bits.chan9_l6;
         break;
      case 9:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan10_h2 ) << 8 ) | PDPStatus2_.Data_.bits.chan10_l8;
         break;
      case 10:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan11_h8 ) << 2 ) | PDPStatus2_.Data_.bits.chan11_l2;
         break;
      case 11:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus2_.Data_.bits.chan12_h6 ) << 4 ) | PDPStatus2_.Data_.bits.chan12_l4;
         break;
      case 12:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus3_.Data_.bits.chan13_h8 ) << 2 ) | PDPStatus3_.Data_.bits.chan13_l2;
         break;
      case 13:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus3_.Data_.bits.chan14_h6 ) << 4 ) | PDPStatus3_.Data_.bits.chan14_l4;
         break;
      case 14:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus3_.Data_.bits.chan15_h4 ) << 6 ) | PDPStatus3_.Data_.bits.chan15_l6;
         break;
      case 15:
         tmpReturnValue = ( static_cast<uint32_t>( PDPStatus3_.Data_.bits.chan16_h2 ) << 8 ) | PDPStatus3_.Data_.bits.chan16_l8;
         break;
   }

   /*
    * Convert to Amps
    * 7.3 fixed point value in Amps
    */
   return tmpReturnValue * 0.125f;
}

/*
 *
 * Robot
 *
 */
void Team271PDP::PreRobotPeriodic( void )
{
   if( Team271Robot::CompareTime( TimeCANUpdateLast_,
                                  kCANUpdateRate ) == 1 )
   {
#if defined(T271_PDP_CAN_ENABLE)
      int32_t tmpStatus = 0;

      /*
       *
       * Status 1
       *
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &PDPStatus1_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&PDPStatus1_.Data_,
                                                             &PDPStatus1_.Len_,
                                                             &PDPStatus1_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         PDPStatus1_.DataLast_ = Team271Robot::GetTimeDelta( PDPStatus1_.DataLast_ );
      }

      /*
       *
       * Status 2
       *
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &PDPStatus2_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&PDPStatus2_.Data_,
                                                             &PDPStatus2_.Len_,
                                                             &PDPStatus2_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         PDPStatus2_.DataLast_ = Team271Robot::GetTimeDelta( PDPStatus2_.DataLast_ );
      }

      /*
       *
       * Status 3
       *
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &PDPStatus3_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&PDPStatus3_.Data_,
                                                             &PDPStatus3_.Len_,
                                                             &PDPStatus3_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         PDPStatus3_.DataLast_ = Team271Robot::GetTimeDelta( PDPStatus3_.DataLast_ );

         /* 50mV per unit plus 4V. */
         Write( t271_pdp_data_index::T271_PDP_DATA_BUS_VOLTAGE,
                (float)PDPStatus3_.Data_.bits.busVoltage * 0.05f + 4.0f );

         Write( t271_pdp_data_index::T271_PDP_DATA_BUS_TEMPERATURE,
                (float)PDPStatus3_.Data_.bits.temp * 1.03250836957542f - 67.8564500484966f );
      }

      /*
       *
       * Status Energy
       *
       */
#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_CANSessionMux_receiveMessage( &PDPStatusEnergy_.ArbID_,
                                                             kFullMessageIDMask,
                                                             (uint8_t*)&PDPStatusEnergy_.Data_,
                                                             &PDPStatusEnergy_.Len_,
                                                             &PDPStatusEnergy_.TimeStamp_,
                                                             &tmpStatus );
#endif

      if( tmpStatus == 0 )
      {
         PDPStatusEnergy_.DataLast_ = Team271Robot::GetTimeDelta( PDPStatusEnergy_.DataLast_ );

         /*
          * Total Current
          */
         uint32_t raw = PDPStatusEnergy_.Data_.bits.TotalCurrent_125mAperunit_h8;
         raw <<= 4;
         raw |= PDPStatusEnergy_.Data_.bits.TotalCurrent_125mAperunit_l4;

         /* 7.3 fixed pt value in Amps */
         Write( t271_pdp_data_index::T271_PDP_DATA_TOTAL_CURRENT,
                0.125f * raw );

         /*
          * Total Power
          */
         raw = PDPStatusEnergy_.Data_.bits.Power_125mWperunit_h4;
         raw <<= 8;
         raw |= PDPStatusEnergy_.Data_.bits.Power_125mWperunit_m8;
         raw <<= 4;
         raw |= PDPStatusEnergy_.Data_.bits.Power_125mWperunit_l4;

         /* 7.3 fixed pt value in Watts */
         Write( t271_pdp_data_index::T271_PDP_DATA_TOTAL_POWER,
                0.125f * raw );

         /*
          * Total Energy
          */
         raw = PDPStatusEnergy_.Data_.bits.Energy_125mWPerUnitXTmeas_h4;
         raw <<= 8;
         raw |= PDPStatusEnergy_.Data_.bits.Energy_125mWPerUnitXTmeas_mh8;
         raw <<= 8;
         raw |= PDPStatusEnergy_.Data_.bits.Energy_125mWPerUnitXTmeas_ml8;
         raw <<= 8;
         raw |= PDPStatusEnergy_.Data_.bits.Energy_125mWPerUnitXTmeas_l8;

         float energyJoules = 0.125f * raw; /* mW integrated every TmeasMs */
         energyJoules *= 0.001f; /* convert from mW to W */
         energyJoules *= PDPStatusEnergy_.Data_.bits.TmeasMs_likelywillbe20ms_; /* multiplied by TmeasMs = joules */

         Write( t271_pdp_data_index::T271_PDP_DATA_TOTAL_ENERGY,
                0.125f * raw );
      }
#endif
   }
}

/*
 *
 * Robot State
 *
 */
void Team271PDP::ClearStickyFaults( void )
{
#if defined(T271_PDP_CAN_ENABLE)
   int32_t tmpStatus = 0;

   uint8_t pdpControl[] = { 0x80 }; /* only bit set is ClearStickyFaults */

#if defined( T271_FRC_NETCOM_ENABLE )
   FRC_NetworkCommunication_CANSessionMux_sendMessage( PDPControl_.ArbID_,
                                                       pdpControl,
                                                       1,
                                                       T271_CAN_SEND_PERIOD_NO_REPEAT,
                                                       &tmpStatus );
#endif
#endif
}

/*
 *
 * PDP Functions
 *
 */
int32_t Team271PDP::CreateCANID( const int32_t argApiID ) const
{
   int32_t tmpID = ( static_cast<int32_t>( kDeviceType ) & 0x1F ) << 24;

   tmpID |= ( static_cast<int32_t>( kManufacturer ) & 0xFF ) << 16;
   tmpID |= ( argApiID & 0x3FF ) << 6;
   tmpID |= ( CANID_ & 0x3F );

   return tmpID;
}

void Team271PDP::ClearTotalEnegery( void )
{
#if defined(T271_PDP_CAN_ENABLE)
   int32_t tmpStatus = 0;

   uint8_t pdpControl[] = { 0x40 }; /* only bit set is ResetEnergy */

#if defined( T271_FRC_NETCOM_ENABLE )
   FRC_NetworkCommunication_CANSessionMux_sendMessage( PDPControl_.ArbID_,
                                                       pdpControl,
                                                       1,
                                                       T271_CAN_SEND_PERIOD_NO_REPEAT,
                                                       &tmpStatus );
#endif
#endif
}

#endif

