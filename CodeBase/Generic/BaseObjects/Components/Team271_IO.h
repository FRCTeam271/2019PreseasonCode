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

#ifndef TEAM271_IO_H_
#define TEAM271_IO_H_

#if defined( T271_IO_ENABLE )

class Team271IO
{
public:
   /*
    *
    * Constants
    *
    */
   static constexpr int32_t kPwmDisabled = 0;

protected:
   struct DigitalPort
   {
      uint8_t channel = 0;
      bool configSet = false;
      bool eliminateDeadband = false;
      int32_t maxPwm = 0;
      int32_t deadbandMaxPwm = 0;
      int32_t centerPwm = 0;
      int32_t deadbandMinPwm = 0;
      int32_t minPwm = 0;
   };

   /*
    *
    * Constants
    *
    */

   /**
    * MXP channels when used as digital output PWM are offset from actual value
    */
   static constexpr int32_t kMXPDigitalPWMOffset = 6;

   static constexpr int32_t kExpectedLoopTiming = 40;

   /**
    * kDefaultPwmPeriod is in ms
    *
    * - 20ms periods (50 Hz) are the "safest" setting in that this works for all
    *   devices
    * - 20ms periods seem to be desirable for Vex Motors
    * - 20ms periods are the specified period for HS-322HD servos, but work
    *   reliably down to 10.0 ms; starting at about 8.5ms, the servo sometimes hums
    *   and get hot; by 5.0ms the hum is nearly continuous
    * - 10ms periods work well for Victor 884
    * - 5ms periods allows higher update rates for Luminary Micro Jaguar speed
    *   controllers. Due to the shipping firmware on the Jaguar, we can't run the
    *   update period less than 5.05 ms.
    *
    * kDefaultPwmPeriod is the 1x period (5.05 ms).  In hardware, the period
    * scaling is implemented as an output squelch to get longer periods for old
    * devices.
    */
   static constexpr double kDefaultPwmPeriod = 5.05;

   /**
    * kDefaultPwmCenter is the PWM range center in ms
    */
   static constexpr double kDefaultPwmCenter = 1.5;

   /**
    * kDefaultPWMStepsDown is the number of PWM steps below the centerpoint
    */
   static constexpr int32_t kDefaultPwmStepsDown = 1000;

   static constexpr int32_t kSystemClockTicksPerMicrosecond = 40;

   /*
    *
    * Variables
    *
    */
#if !defined(T271_DASHBOARD)
   std::unique_ptr<hal::tDIO> digitalSystem;
   std::unique_ptr<hal::tRelay> relaySystem;
   std::unique_ptr<hal::tPWM> pwmSystem;
   std::unique_ptr<hal::tSPI> spiSystem;
#endif

   DigitalPort DigitalChannels_[kNumDigitalChannels + kNumPWMHeaders];

   atomic<uint8_t> IsSetup_ = { 0 };
public:

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271IO( void );

   virtual ~Team271IO( void );

   static Team271IO& getInstance( void );

   Team271IO( Team271IO const& ) = delete;
   void operator=( Team271IO const& ) = delete;

   /*
    *
    * DIO
    *
    */
   void Setup( void );

   int32_t InitDIOChannel( const uint32_t argChannel,
         int32_t argInput );

   int32_t FreeDIOChannel( const uint32_t argChannel );

   uint32_t ReadEnableMXPSpecialFunction( tRioStatusCode* const argStatus );

   void WriteEnableMXPSpecialFunction( const uint16_t argValue,
         tRioStatusCode* const argStatus );

   /*
    *
    * PWM
    *
    */
   int32_t PWM_Setup( void );

   uint32_t PWM_ReadLoopTiming( tRioStatusCode* const argStatus );

   void PWM_WriteHdr( const uint32_t argRegIdx,
         const uint32_t argValue,
         tRioStatusCode* const argStatus );

   void PWM_SetEliminateDeadband( const uint32_t argChannel,
         const int32_t argEliminateDeadband );

   void PWM_SetConfig( const uint32_t argChannel,
         const double argMax,
         const double argDeadbandMax,
         const double argCenter,
         const double argDeadbandMin,
         const double argMin,
         int32_t* argStatus );

   void PWM_SetSpeed( const uint32_t argChannel,
         const double argSpeed,
         int32_t* argStatus );

   void PWM_SetRaw( const uint32_t argChannel,
         const int32_t argValue,
         int32_t* const argStatus );

   void PWM_InitializePort( const uint32_t argChannel,
         int32_t* const argStatus );

   void PWM_FreePort( const uint32_t argChannel,
         int32_t* const argStatus );

   static inline int32_t GetMaxPositivePwm( const DigitalPort* const argPort );

   static inline int32_t GetMinPositivePwm( const DigitalPort* const argPort );

   static inline int32_t GetCenterPwm( const DigitalPort* const argPort );

   static inline int32_t GetMaxNegativePwm( const DigitalPort* const argPort );

   static inline int32_t GetMinNegativePwm( const DigitalPort* const argPort );

   static inline int32_t GetPositiveScaleFactor( const DigitalPort* const argPort );

   static inline int32_t GetNegativeScaleFactor( const DigitalPort* const argPort );

   static inline int32_t GetFullRangeScaleFactor( const DigitalPort* const argPort );

   static int32_t remapMXPChannel( const int32_t argChannel );

   static int32_t remapMXPPWMChannel( const int32_t argChannel );

   static int32_t remapSPIChannel( const int32_t argChannel );

   /*
    *
    * Relay
    *
    */
   int32_t Relay_Setup( void );

   /*
    *
    * SPI
    *
    */
   int32_t SPI_Setup( void );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
