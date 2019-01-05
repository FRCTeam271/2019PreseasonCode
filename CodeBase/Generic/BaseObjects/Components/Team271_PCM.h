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

#ifndef TEAM271_PCM_H_
#define TEAM271_PCM_H_

#if defined( T271_PCM_ENABLE )

#define STATUS_1           0x9041400
#define STATUS_SOL_FAULTS  0x9041440
#define STATUS_DEBUG       0x9041480

#define CONTROL_1          0x09041C00  /* PCM_Control */
#define CONTROL_2          0x09041C40  /* PCM_SupplemControl */
#define CONTROL_3          0x09041C80  /* PcmControlSetOneShotDur_t */

/* encoder/decoders */
typedef struct _PcmStatus_t
{
   /* Byte 0 */
   unsigned SolenoidBits :8;
   /* Byte 1 */
   unsigned compressorOn :1;
   unsigned stickyFaultFuseTripped :1;
   unsigned stickyFaultCompCurrentTooHigh :1;
   unsigned faultFuseTripped :1;
   unsigned faultCompCurrentTooHigh :1;
   unsigned faultHardwareFailure :1;
   unsigned isCloseloopEnabled :1;
   unsigned pressureSwitchEn :1;
   /* Byte 2*/
   unsigned battVoltage :8;
   /* Byte 3 */
   unsigned solenoidVoltageTop8 :8;
   /* Byte 4 */
   unsigned compressorCurrentTop6 :6;
   unsigned solenoidVoltageBtm2 :2;
   /* Byte 5 */
   unsigned StickyFault_dItooHigh :1;
   unsigned Fault_dItooHigh :1;
   unsigned moduleEnabled :1;
   unsigned closedLoopOutput :1;
   unsigned compressorCurrentBtm4 :4;
   /* Byte 6 */
   unsigned tokenSeedTop8 :8;
   /* Byte 7 */
   unsigned tokenSeedBtm8 :8;
} PcmStatus_t;

typedef struct _PcmControl_t
{
   /* Byte 0 */
   unsigned tokenTop8 :8;
   /* Byte 1 */
   unsigned tokenBtm8 :8;
   /* Byte 2 */
   unsigned solenoidBits :8;
   /* Byte 3*/
   unsigned reserved :4;
   unsigned closeLoopOutput :1;
   unsigned compressorOn :1;
   unsigned closedLoopEnable :1;
   unsigned clearStickyFaults :1;
   /* Byte 4 */
   unsigned OneShotField_h8 :8;
   /* Byte 5 */
   unsigned OneShotField_l8 :8;

   unsigned Res1 :8;
   unsigned Res2 :8;
   unsigned Res3 :8;
} PcmControl_t;

typedef struct _PcmControlSetOneShotDur_t
{
   uint8_t sol10MsPerUnit[8];
} PcmControlSetOneShotDur_t;

typedef struct _PcmStatusFault_t
{
   /* Byte 0 */
   unsigned SolenoidBlacklist :8;
   /* Byte 1 */
   unsigned reserved_bit0 :1;
   unsigned reserved_bit1 :1;
   unsigned reserved_bit2 :1;
   unsigned reserved_bit3 :1;
   unsigned StickyFault_CompNoCurrent :1;
   unsigned Fault_CompNoCurrent :1;
   unsigned StickyFault_SolenoidJumper :1;
   unsigned Fault_SolenoidJumper :1;

   unsigned Res1 :8;
   unsigned Res2 :8;
   unsigned Res3 :8;
   unsigned Res4 :8;
   unsigned Res5 :8;
   unsigned Res6 :8;
} PcmStatusFault_t;

typedef struct _PcmDebug_t
{
   unsigned tokFailsTop8 :8;
   unsigned tokFailsBtm8 :8;
   unsigned lastFailedTokTop8 :8;
   unsigned lastFailedTokBtm8 :8;
   unsigned tokSuccessTop8 :8;
   unsigned tokSuccessBtm8 :8;

   unsigned Res1 :8;
   unsigned Res2 :8;
} PcmDebug_t;

typedef struct
{
   uint32_t ArbID_;
   PcmControl_t Data_;
   PcmControl_t DataPrev_;
} T271_PCM_TXControl;

typedef struct
{
   uint32_t ArbID_ = 0;

   PcmStatus_t Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PCM_RXStatus;

typedef struct
{
   uint32_t ArbID_ = 0;

   PcmStatusFault_t Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PCM_RXFault;

typedef struct
{
   uint32_t ArbID_ = 0;

   PcmDebug_t Data_;
   uint8_t Len_ = 0;
   uint32_t TimeStamp_ = 0;

   float DataLast_ = 0;
} T271_PCM_RXDebug;

class Team271PCM : public Team271Base
{
protected:
   /*
    *
    * Constants
    *
    */
   static const int32_t kFullMessageIDMask = 0x1fffffff;
   static const int32_t kCANPeriod = 20;

   static constexpr float kCANTXUpdateRate = 0.100;
   static constexpr float kCANRXUpdateRate = 0.010;

   float TimeCANTXUpdateLast_ = 0;
   float TimeCANRXUpdateLast_ = 0;

   int8_t ModuleID_ = -1;

   T271_PCM_TXControl TXControl_;

   T271_PCM_RXStatus RXStatus_;
   T271_PCM_RXFault RXFaults_;
   T271_PCM_RXDebug RXDebug_;

   atomic<uint8_t> SolenoidBits_ = { 0 };
   atomic<uint8_t> ClosedLoopEnable_ = { 0 };

   atomic<uint8_t> IsSetup_ = { 0 };

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271PCM( const string& argName,
               const int32_t argModuleNumber );

   virtual ~Team271PCM( void );

   /*
    *
    * Instance
    *
    */
   static constexpr uint8_t kMaxInstance = 2;

   static Team271PCM& getInstance( const uint32_t argID );

   Team271PCM( Team271PCM const& ) = delete;
   void operator=( Team271PCM const& ) = delete;

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   float GetBusVoltage( void ) const;

   float GetSolenoidRailVoltage( void ) const;

   bool GetPressureSwitch( void ) const;

   bool GetCompressorEnable( void ) const;

   float GetCompressorCurrent( void ) const;

   bool GetCompressorFault_TooHighCurrent( void ) const;

   bool GetCompressorFault_Shorted( void ) const;

   bool GetCompressorFault_NotConnected( void ) const;

   /*
    *
    * Setters
    *
    */
   void SetCompressorEnable( const bool argEnable );

   void PreRobotPeriodic( void );
   void PostRobotPeriodic( void );

   /*
    *
    * Robot State
    *
    */
   void ClearStickyFaults( void );

   /*
    *
    * PCM
    *
    */
   void Setup( void );

   void Set( const uint32_t argChannelNumber,
             const uint32_t argValue );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
