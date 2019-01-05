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

#if defined(T271_IO_ENABLE)

static Team271IO IOInstance_;

Team271IO& Team271IO::getInstance( void )
{
   IOInstance_.Setup();

   return IOInstance_;
}

Team271IO::Team271IO( void )
{

}

Team271IO::~Team271IO( void )
{

}

void Team271IO::Setup( void )
{
   if( IsSetup_ == 0 )
   {
      IsSetup_ = 1;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
      int32_t tmpStatus = 0;

      digitalSystem.reset( hal::tDIO::create( &tmpStatus ) );
#endif

      Relay_Setup();

      PWM_Setup();

      SPI_Setup();
   }
}

int32_t Team271IO::InitDIOChannel( const uint32_t argChannel,
                                   int32_t argInput )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)

   if( argChannel < kNumDigitalChannels )
   {
      DigitalPort& port = DigitalChannels_[argChannel];

      port.channel = static_cast<uint8_t>( argChannel );

      hal::tDIO::tOutputEnable outputEnable = digitalSystem->readOutputEnable( &tmpStatus );

      if( port.channel >= ( kNumDigitalHeaders + kNumDigitalMXPChannels ) )
      {
         //if( !hal::getPortHandleSPIEnable( portHandle ) )
         //{
         // if this flag is not set, we actually want DIO.
         uint32_t bitToSet = 1u << remapSPIChannel( port.channel );

         uint16_t specialFunctions = spiSystem->readEnableDIO( &tmpStatus );
         // Set the field to enable SPI DIO
         spiSystem->writeEnableDIO( specialFunctions | bitToSet,
                                    &tmpStatus );

         if( argInput )
         {
            outputEnable.SPIPort = outputEnable.SPIPort & ( ~bitToSet );  // clear the field for read
         }
         else
         {
            outputEnable.SPIPort = outputEnable.SPIPort | bitToSet;  // set the bits for write
         }
         //}
      }
      else if( port.channel < kNumDigitalHeaders )
      {
         uint32_t bitToSet = 1u << port.channel;
         if( argInput )
         {
            outputEnable.Headers = outputEnable.Headers & ( ~bitToSet );  // clear the bit for read
         }
         else
         {
            outputEnable.Headers = outputEnable.Headers | bitToSet;  // set the bit for write
         }
      }
      else
      {
         uint32_t bitToSet = 1u << remapMXPChannel( port.channel );

         uint16_t specialFunctions = digitalSystem->readEnableMXPSpecialFunction( &tmpStatus );
         digitalSystem->writeEnableMXPSpecialFunction( specialFunctions & ~bitToSet,
                                                       &tmpStatus );

         if( argInput )
         {
            outputEnable.MXP = outputEnable.MXP & ( ~bitToSet );  // clear the bit for read
         }
         else
         {
            outputEnable.MXP = outputEnable.MXP | bitToSet;  // set the bit for write
         }
      }

      digitalSystem->writeOutputEnable( outputEnable,
                                        &tmpStatus );
   }
#endif

   return tmpStatus;
}

int32_t Team271IO::FreeDIOChannel( const uint32_t argChannel )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   DigitalPort& port = DigitalChannels_[argChannel];

   if( port.channel >= ( kNumDigitalHeaders + kNumDigitalMXPChannels ) )
   {
      // Unset the SPI flag
      int32_t bitToUnset = 1 << remapSPIChannel( port.channel );
      uint16_t specialFunctions = spiSystem->readEnableDIO( &tmpStatus );
      spiSystem->writeEnableDIO( specialFunctions & ~bitToUnset,
                                 &tmpStatus );
   }
   else if( port.channel >= kNumDigitalHeaders )
   {
      // Unset the MXP flag
      uint32_t bitToUnset = 1u << remapMXPChannel( port.channel );

      uint16_t specialFunctions = digitalSystem->readEnableMXPSpecialFunction( &tmpStatus );
      digitalSystem->writeEnableMXPSpecialFunction( specialFunctions | bitToUnset,
                                                    &tmpStatus );
   }
#endif

   return tmpStatus;
}

uint32_t Team271IO::ReadEnableMXPSpecialFunction( tRioStatusCode* const argStatus )
{
   uint32_t tmpReturnValue = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   int32_t tmpStatus = 0;

   tmpReturnValue = digitalSystem->readEnableMXPSpecialFunction( &tmpStatus );
#endif

   return tmpReturnValue;
}

void Team271IO::WriteEnableMXPSpecialFunction( const uint16_t argValue,
                                               tRioStatusCode* const argStatus )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   digitalSystem->writeEnableMXPSpecialFunction( argValue,
                                                 &tmpStatus );
#endif

   *argStatus = tmpStatus;
}

/*
 *
 * PWM
 *
 */
int32_t Team271IO::PWM_Setup( void )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   pwmSystem.reset( hal::tPWM::create( &tmpStatus ) );

   if( pwmSystem != nullptr )
   {
      /*
       * Make sure that the 9403 IONode has had a chance to initialize before continuing
       */
      while( pwmSystem->readLoopTiming( &tmpStatus ) == 0 )
      {
         std::this_thread::yield();
      }

      if( pwmSystem->readLoopTiming( &tmpStatus ) != kExpectedLoopTiming )
      {
         LOG_ERROR( "%s - Loop Timing Error",
                    "Team271IO" );
      }

      // Calculate the length, in ms, of one DIO loop
      const double loopTime = pwmSystem->readLoopTiming( &tmpStatus ) / ( kSystemClockTicksPerMicrosecond * 1e3 );

      pwmSystem->writeConfig_Period( static_cast<uint16_t>( kDefaultPwmPeriod / loopTime + .5 ),
                                     &tmpStatus );

      uint16_t minHigh = static_cast<uint16_t>( ( kDefaultPwmCenter - kDefaultPwmStepsDown * loopTime ) / loopTime + .5 );

      pwmSystem->writeConfig_MinHigh( minHigh,
                                      &tmpStatus );

      // Ensure that PWM output values are set to OFF
      for( uint8_t pwmIndex = 0; pwmIndex < kNumPWMChannels; pwmIndex++ )
      {
         // Copy of SetPWM
         if( pwmIndex < hal::tPWM::kNumHdrRegisters )
         {
            pwmSystem->writeHdr( pwmIndex,
                                 kPwmDisabled,
                                 &tmpStatus );
         }
         else
         {
            pwmSystem->writeMXP( pwmIndex - hal::tPWM::kNumHdrRegisters,
                                 kPwmDisabled,
                                 &tmpStatus );
         }

         // Copy of SetPWMPeriodScale, set to 4x by default.
         if( pwmIndex < hal::tPWM::kNumPeriodScaleHdrElements )
         {
            pwmSystem->writePeriodScaleHdr( pwmIndex,
                                            3,
                                            &tmpStatus );
         }
         else
         {
            pwmSystem->writePeriodScaleMXP( pwmIndex - hal::tPWM::kNumPeriodScaleHdrElements,
                                            3,
                                            &tmpStatus );
         }
      }
   }
#endif

   return tmpStatus;
}

void Team271IO::PWM_SetEliminateDeadband( const uint32_t argChannel,
                                          const int32_t argEliminateDeadband )
{
   DigitalChannels_[argChannel].eliminateDeadband = argEliminateDeadband;
}

uint32_t Team271IO::PWM_ReadLoopTiming( tRioStatusCode* const argStatus )
{
   uint32_t tmpReturnValue = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   tmpReturnValue = pwmSystem->readLoopTiming( argStatus );
#endif

   return tmpReturnValue;
}

void Team271IO::PWM_WriteHdr( const uint32_t argRegIdx,
                              const uint32_t argValue,
                              tRioStatusCode* const argStatus )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   pwmSystem->writeHdr( argRegIdx,
                        argValue,
                        argStatus );
#endif
}

void Team271IO::PWM_SetConfig( const uint32_t argChannel,
                               const double argMax,
                               const double argDeadbandMax,
                               const double argCenter,
                               const double argDeadbandMin,
                               const double argMin,
                               int32_t* argStatus )
{
   // calculate the loop time in milliseconds
   double loopTime = PWM_ReadLoopTiming( argStatus ) / ( kSystemClockTicksPerMicrosecond * 1e3 );

   if( *argStatus == 0 )
   {
      int32_t maxPwm = static_cast<int32_t>( ( argMax - kDefaultPwmCenter ) / loopTime + kDefaultPwmStepsDown - 1 );
      int32_t deadbandMaxPwm = static_cast<int32_t>( ( argDeadbandMax - kDefaultPwmCenter ) / loopTime + kDefaultPwmStepsDown - 1 );
      int32_t centerPwm = static_cast<int32_t>( ( argCenter - kDefaultPwmCenter ) / loopTime + kDefaultPwmStepsDown - 1 );
      int32_t deadbandMinPwm = static_cast<int32_t>( ( argDeadbandMin - kDefaultPwmCenter ) / loopTime + kDefaultPwmStepsDown - 1 );
      int32_t minPwm = static_cast<int32_t>( ( argMin - kDefaultPwmCenter ) / loopTime + kDefaultPwmStepsDown - 1 );

      DigitalChannels_[argChannel].maxPwm = maxPwm;
      DigitalChannels_[argChannel].deadbandMaxPwm = deadbandMaxPwm;
      DigitalChannels_[argChannel].deadbandMinPwm = deadbandMinPwm;
      DigitalChannels_[argChannel].centerPwm = centerPwm;
      DigitalChannels_[argChannel].minPwm = minPwm;
      DigitalChannels_[argChannel].configSet = true;
   }
}

void Team271IO::PWM_SetSpeed( const uint32_t argChannel,
                              const double argSpeed,
                              int32_t* argStatus )
{
   DigitalPort& port = DigitalChannels_[argChannel];

   if( port.configSet != 0 )
   {
      double tmpSpeed = argSpeed;

      if( tmpSpeed < -1.0 )
      {
         tmpSpeed = -1.0;
      }
      else if( tmpSpeed > 1.0 )
      {
         tmpSpeed = 1.0;
      }
      else if( std::isfinite( tmpSpeed ) == false )
      {
         tmpSpeed = 0.0;
      }

      // calculate the desired output pwm value by scaling the speed appropriately
      int32_t rawValue;
      if( tmpSpeed == 0.0 )
      {
         rawValue = GetCenterPwm( &port );
      }
      else if( tmpSpeed > 0.0 )
      {
         rawValue = static_cast<int32_t>( tmpSpeed * static_cast<double>( GetPositiveScaleFactor( &port ) ) + static_cast<double>( GetMinPositivePwm( &port ) ) + 0.5 );
      }
      else
      {
         rawValue = static_cast<int32_t>( tmpSpeed * static_cast<double>( GetNegativeScaleFactor( &port ) ) + static_cast<double>( GetMaxNegativePwm( &port ) ) + 0.5 );
      }

      if( !( ( rawValue >= GetMinNegativePwm( &port ) ) && ( rawValue <= GetMaxPositivePwm( &port ) ) ) || rawValue == kPwmDisabled )
      {
         //*argStatus = HAL_PWM_SCALE_ERROR;
         *argStatus = -1;
      }
      else
      {
         PWM_SetRaw( argChannel,
                     rawValue,
                     argStatus );
      }
   }
   else
   {
      //*argStatus = INCOMPATIBLE_STATE;
      *argStatus = -1;
   }
}

void Team271IO::PWM_SetRaw( const uint32_t argChannel,
                            const int32_t argValue,
                            int32_t* const argStatus )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   if( argChannel < hal::tPWM::kNumHdrRegisters )
   {
      PWM_WriteHdr( argChannel,
                    argValue,
                    argStatus );
   }
   else
   {
      pwmSystem->writeMXP( argChannel - hal::tPWM::kNumHdrRegisters,
                           argValue,
                           argStatus );
   }
#endif
}

void Team271IO::PWM_InitializePort( const uint32_t argChannel,
                                    int32_t* const argStatus )
{
   uint32_t tmpChannel = argChannel;

   if( tmpChannel < kNumPWMChannels )
   {
      DigitalPort& port = DigitalChannels_[tmpChannel];
      uint8_t origChannel = static_cast<uint8_t>( tmpChannel );

      if( origChannel < kNumPWMHeaders )
      {
         tmpChannel += kNumDigitalChannels;  // remap Headers to end of allocations
      }
      else
      {
         tmpChannel = remapMXPPWMChannel( tmpChannel ) + 10;  // remap MXP to proper channel
      }

      port.channel = tmpChannel;

      if( port.channel > hal::tPWM::kNumHdrRegisters - 1 )
      {
         int32_t bitToSet = 1 << remapMXPPWMChannel( port.channel );
         uint16_t specialFunctions = ReadEnableMXPSpecialFunction( argStatus );
         WriteEnableMXPSpecialFunction( specialFunctions | bitToSet,
                                        argStatus );
      }

      // Defaults to allow an always valid config.
      PWM_SetConfig( tmpChannel,
                     2.0,
                     1.501,
                     1.5,
                     1.499,
                     1.0,
                     argStatus );
   }
}

void Team271IO::PWM_FreePort( const uint32_t argChannel,
                              int32_t* const argStatus )
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   if( argChannel < kNumPWMChannels )
   {
      DigitalPort& port = DigitalChannels_[argChannel];

      if( port.channel > hal::tPWM::kNumHdrRegisters - 1 )
      {
         int32_t bitToUnset = 1 << remapMXPPWMChannel( port.channel );
         uint16_t specialFunctions = digitalSystem->readEnableMXPSpecialFunction( argStatus );
         digitalSystem->writeEnableMXPSpecialFunction( specialFunctions & ~bitToUnset,
                                                       argStatus );
      }
   }
#endif
}

inline int32_t Team271IO::GetMaxPositivePwm( const DigitalPort* const argPort )
{
   return argPort->maxPwm;
}

inline int32_t Team271IO::GetMinPositivePwm( const DigitalPort* const argPort )
{
   if( argPort->eliminateDeadband )
   {
      return argPort->deadbandMaxPwm;
   }
   else
   {
      return argPort->centerPwm + 1;
   }
}

inline int32_t Team271IO::GetCenterPwm( const DigitalPort* const argPort )
{
   return argPort->centerPwm;
}

inline int32_t Team271IO::GetMaxNegativePwm( const DigitalPort* const argPort )
{
   if( argPort->eliminateDeadband )
   {
      return argPort->deadbandMinPwm;
   }
   else
   {
      return argPort->centerPwm - 1;
   }
}

inline int32_t Team271IO::GetMinNegativePwm( const DigitalPort* const argPort )
{
   return argPort->minPwm;
}

inline int32_t Team271IO::GetPositiveScaleFactor( const DigitalPort* const argPort )
{
   return GetMaxPositivePwm( argPort ) - GetMinPositivePwm( argPort );
}  ///< The scale for positive speeds.

inline int32_t Team271IO::GetNegativeScaleFactor( const DigitalPort* const argPort )
{
   return GetMaxNegativePwm( argPort ) - GetMinNegativePwm( argPort );
}  ///< The scale for negative speeds.

inline int32_t Team271IO::GetFullRangeScaleFactor( const DigitalPort* const argPort )
{
   return GetMaxPositivePwm( argPort ) - GetMinNegativePwm( argPort );
}  ///< The scale for positions.

/**
 * Map DIO channel numbers from their physical number (10 to 26) to their
 * position in the bit field.
 */
int32_t Team271IO::remapMXPChannel( const int32_t argChannel )
{
   return argChannel - 10;
}

int32_t Team271IO::remapMXPPWMChannel( const int32_t argChannel )
{
   if( argChannel < 14 )
   {
      return argChannel - 10;  // first block of 4 pwms (MXP 0-3)
   }
   else
   {
      return argChannel - 6;  // block of PWMs after SPI
   }
}

/**
 * Map SPI channel numbers from their physical number (27 to 31) to their
 * position in the bit field.
 */
int32_t Team271IO::remapSPIChannel( const int32_t argChannel )
{
   return argChannel - 26;
}

/*
 *
 * Relay
 *
 */
int32_t Team271IO::Relay_Setup( void )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   // Relay Setup
   relaySystem.reset( hal::tRelay::create( &tmpStatus ) );

   // Turn off all relay outputs.
   relaySystem->writeValue_Forward( 0,
                                    &tmpStatus );
   relaySystem->writeValue_Reverse( 0,
                                    &tmpStatus );
#endif

   return tmpStatus;
}

/*
 *
 * SPI
 *
 */
int32_t Team271IO::SPI_Setup( void )
{
   int32_t tmpStatus = 0;

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   spiSystem.reset( hal::tSPI::create( &tmpStatus ) );
#endif

   return tmpStatus;
}

#endif

