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

#ifndef TEAM271_PDP_H_
#define TEAM271_PDP_H_

#if defined( T271_PDP_ENABLE )

/* encoder/decoders */
union PdpStatus1
{
   uint8_t data[8];
   struct Bits
   {
      unsigned chan1_h8 :8;
      unsigned chan2_h6 :6;
      unsigned chan1_l2 :2;
      unsigned chan3_h4 :4;
      unsigned chan2_l4 :4;
      unsigned chan4_h2 :2;
      unsigned chan3_l6 :6;
      unsigned chan4_l8 :8;
      unsigned chan5_h8 :8;
      unsigned chan6_h6 :6;
      unsigned chan5_l2 :2;
      unsigned reserved4 :4;
      unsigned chan6_l4 :4;
   } bits;
};

union PdpStatus2
{
   uint8_t data[8];
   struct Bits
   {
      unsigned chan7_h8 :8;
      unsigned chan8_h6 :6;
      unsigned chan7_l2 :2;
      unsigned chan9_h4 :4;
      unsigned chan8_l4 :4;
      unsigned chan10_h2 :2;
      unsigned chan9_l6 :6;
      unsigned chan10_l8 :8;
      unsigned chan11_h8 :8;
      unsigned chan12_h6 :6;
      unsigned chan11_l2 :2;
      unsigned reserved4 :4;
      unsigned chan12_l4 :4;
   } bits;
};

union PdpStatus3
{
   uint8_t data[8];
   struct Bits
   {
      unsigned chan13_h8 :8;
      unsigned chan14_h6 :6;
      unsigned chan13_l2 :2;
      unsigned chan15_h4 :4;
      unsigned chan14_l4 :4;
      unsigned chan16_h2 :2;
      unsigned chan15_l6 :6;
      unsigned chan16_l8 :8;
      unsigned internalResBattery_mOhms :8;
      unsigned busVoltage :8;
      unsigned temp :8;
   } bits;
};

union PdpStatusEnergy
{
   uint8_t data[8];
   struct Bits
   {
      unsigned TmeasMs_likelywillbe20ms_ :8;
      unsigned TotalCurrent_125mAperunit_h8 :8;
      unsigned Power_125mWperunit_h4 :4;
      unsigned TotalCurrent_125mAperunit_l4 :4;
      unsigned Power_125mWperunit_m8 :8;
      unsigned Energy_125mWPerUnitXTmeas_h4 :4;
      unsigned Power_125mWperunit_l4 :4;
      unsigned Energy_125mWPerUnitXTmeas_mh8 :8;
      unsigned Energy_125mWPerUnitXTmeas_ml8 :8;
      unsigned Energy_125mWPerUnitXTmeas_l8 :8;
   } bits;
};

typedef struct
{
   uint32_t ArbID_ = 0;
} T271_PDP_TXControl;

typedef struct
{
   uint32_t ArbID_ = 0;

   PdpStatus1 Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PDP_RXStatus1;

typedef struct
{
   uint32_t ArbID_ = 0;

   PdpStatus2 Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PDP_RXStatus2;

typedef struct
{
   uint32_t ArbID_ = 0;

   PdpStatus3 Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PDP_RXStatus3;

typedef struct
{
   uint32_t ArbID_ = 0;

   PdpStatusEnergy Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PDP_RXStatusEnergy;

class t271_pdp_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_PDP_DATA_BUS_VOLTAGE = t271_base_data_index::T271_DATA_COUNT,
      T271_PDP_DATA_BUS_TEMPERATURE,
      T271_PDP_DATA_TOTAL_CURRENT,
      T271_PDP_DATA_TOTAL_POWER,
      T271_PDP_DATA_TOTAL_ENERGY,

      T271_DATA_COUNT,
   }t271_pdp_data_index_t;
};

class Team271PDP : public Team271Base
{
protected:
   /*
    *
    * Constants
    *
    */
   static const int32_t kFullMessageIDMask = 0x1fffffff;
   static constexpr T271_CANManufacturer kManufacturer = T271_CANManufacturer::T271_CAN_Man_kCTRE;
   static constexpr T271_CANDeviceType kDeviceType = T271_CANDeviceType::T271_CAN_Dev_kPowerDistribution;

   static constexpr int32_t kStatus1 = 0x50;
   static constexpr int32_t kStatus2 = 0x51;
   static constexpr int32_t kStatus3 = 0x52;
   static constexpr int32_t kStatusEnergy = 0x5D;

   static constexpr int32_t kControl1 = 0x70;

   static constexpr float kCANUpdateRate = 0.100f;
   float TimeCANUpdateLast_ = 0;

   uint32_t CANID_ = 0;

   T271_PDP_TXControl PDPControl_;

   T271_PDP_RXStatus1 PDPStatus1_;
   T271_PDP_RXStatus2 PDPStatus2_;
   T271_PDP_RXStatus3 PDPStatus3_;
   T271_PDP_RXStatusEnergy PDPStatusEnergy_;

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271PDP( const uint32_t argCANID );

   virtual ~Team271PDP( void );

   static Team271PDP& getInstance( void );

   Team271PDP( Team271PDP const& ) = delete;
   void operator=( Team271PDP const& ) = delete;

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   float GetBusVoltage( void ) const;

   float GetBusTemp( void ) const;

   float GetTotalCurrent( void ) const;

   float GetTotalPower( void ) const;

   float GetTotalEnergy( void ) const;

   float GetCurrent( const uint32_t argCahnnel ) const;

   /*
    *
    * Setters
    *
    */

   /*
    *
    * Robot
    *
    */
   void PreRobotPeriodic( void );

   /*
    *
    * Robot State
    *
    */
   void ClearStickyFaults( void );

   /*
    *
    * PDP Functions
    *
    */
   int32_t CreateCANID( const int32_t argApiID ) const;

   void ClearTotalEnegery( void );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
