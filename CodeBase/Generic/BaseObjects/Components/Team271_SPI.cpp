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

#if defined(T271_SPI_ENABLE)

#if !defined(T271_DASHBOARD)

#if 0
static int32_t m_spiCS0Handle { 0 };
static int32_t m_spiCS1Handle { 0 };
static int32_t m_spiCS2Handle { 0 };
static int32_t m_spiCS3Handle { 0 };
static int32_t m_spiMXPHandle { 0 };

static constexpr int32_t kSpiMaxHandles = 5;

// Indices 0-3 are for onboard CS0-CS2. Index 4 is for MXP.
static std::array<wpi::mutex, kSpiMaxHandles> spiHandleMutexes;
static std::array<wpi::mutex, kSpiMaxHandles> spiApiMutexes;
static std::array<wpi::mutex, kSpiMaxHandles> spiAccumulatorMutexes;

// MXP SPI does not count towards this
static std::atomic<int32_t> spiPortCount { 0 };

static HAL_DigitalHandle digitalHandles[9] { HAL_kInvalidHandle };

static wpi::mutex spiAutoMutex;
static int32_t spiAutoPort = kSpiMaxHandles;
static std::atomic_bool spiAutoRunning { false };
static std::unique_ptr<nFPGA::tDMAManager> spiAutoDMA;

static void CommonSPIPortInit( int32_t* tmpStatus )
{
   // All false cases will set
   if( spiPortCount.fetch_add( 1 ) == 0 )
   {
      // Have not been initialized yet
      hal::initializeDigital( tmpStatus );
      if( *&tmpStatus != 0 )
         return;
      // MISO
      if( ( digitalHandles[3] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 29 ),
                                                       false,
                                                       tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         std::printf( "Failed to allocate DIO 29 (MISO)\n" );
         return;
      }
      // MOSI
      if( ( digitalHandles[4] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 30 ),
                                                       false,
                                                       tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         std::printf( "Failed to allocate DIO 30 (MOSI)\n" );
         HAL_FreeDIOPort( digitalHandles[3] );  // free the first Port_ allocated
         return;
      }
   }
}

static void CommonSPIPortFree( void )
{
   if( spiPortCount.fetch_sub( 1 ) == 1 )
   {
      // Clean up SPI Handles
      HAL_FreeDIOPort( digitalHandles[3] );
      HAL_FreeDIOPort( digitalHandles[4] );
   }
}
#endif

Team271SPI::Team271SPI( const TEAM271_SPI_PORT argPort )
      : Port_( argPort )
{
   int32_t tmpStatus = 0;

   LOG_TRACE( "%s - Constructor Start",
         GetName().c_str() );
#if 0
   if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS0 )
   {
      CommonSPIPortInit( &tmpStatus );
      if( tmpStatus != 0 )
         return;
      // CS0 is not a DIO Port_, so nothing to allocate
      Handle_ = open( "/dev/spidev0.0",
                      O_RDWR );
      if( Handle_ < 0 )
      {
         std::printf( "Failed to open SPI Port_ %d: %s\n",
                      Port_,
                      std::strerror( errno ) );
         CommonSPIPortFree();
         return;
      }
      HAL_SetSPIHandle( HAL_SPI_kOnboardCS0,
                        Handle_ );
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS1 )
   {
      CommonSPIPortInit( &tmpStatus );
      if( tmpStatus != 0 )
         return;
      // CS1, Allocate
      if( ( digitalHandles[0] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 26 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         std::printf( "Failed to allocate DIO 26 (CS1)\n" );
         CommonSPIPortFree();
         return;
      }
      Handle_ = open( "/dev/spidev0.1",
                      O_RDWR );
      if( Handle_ < 0 )
      {
         std::printf( "Failed to open SPI Port_ %d: %s\n",
                      Port_,
                      std::strerror( errno ) );
         CommonSPIPortFree();
         HAL_FreeDIOPort( digitalHandles[0] );
         return;
      }
      HAL_SetSPIHandle( HAL_SPI_kOnboardCS1,
                        Handle_ );
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS2 )
   {
      CommonSPIPortInit( &tmpStatus );
      if( tmpStatus != 0 )
         return;
      // CS2, Allocate
      if( ( digitalHandles[1] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 27 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         std::printf( "Failed to allocate DIO 27 (CS2)\n" );
         CommonSPIPortFree();
         return;
      }
      Handle_ = open( "/dev/spidev0.2",
                      O_RDWR );
      if( Handle_ < 0 )
      {
         std::printf( "Failed to open SPI Port_ %d: %s\n",
                      Port_,
                      std::strerror( errno ) );
         CommonSPIPortFree();
         HAL_FreeDIOPort( digitalHandles[1] );
         return;
      }
      HAL_SetSPIHandle( HAL_SPI_kOnboardCS2,
                        Handle_ );
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS3 )
   {
      CommonSPIPortInit( &tmpStatus );
      if( tmpStatus != 0 )
         return;
      // CS3, Allocate
      if( ( digitalHandles[2] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 28 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         std::printf( "Failed to allocate DIO 28 (CS3)\n" );
         CommonSPIPortFree();
         return;
      }
      Handle_ = open( "/dev/spidev0.3",
                      O_RDWR );
      if( Handle_ < 0 )
      {
         std::printf( "Failed to open SPI Port_ %d: %s\n",
                      Port_,
                      std::strerror( errno ) );
         CommonSPIPortFree();
         HAL_FreeDIOPort( digitalHandles[2] );
         return;
      }
      HAL_SetSPIHandle( HAL_SPI_kOnboardCS3,
                        Handle_ );
   }
   else if( Port_ == TEAM271_SPI_PORT_MXP )
   {
      hal::initializeDigital( &tmpStatus );
      if( tmpStatus != 0 )
         return;
      if( ( digitalHandles[5] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 14 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         //wpi::outs() << "Failed to allocate DIO 14\n";
         return;
      }
      if( ( digitalHandles[6] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 15 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         //wpi::outs() << "Failed to allocate DIO 15\n";
         HAL_FreeDIOPort( digitalHandles[5] );  // free the first Port_ allocated
         return;
      }
      if( ( digitalHandles[7] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 16 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         //wpi::outs() << "Failed to allocate DIO 16\n";
         HAL_FreeDIOPort( digitalHandles[5] );  // free the first Port_ allocated
         HAL_FreeDIOPort( digitalHandles[6] );  // free the second Port_ allocated
         return;
      }
      if( ( digitalHandles[8] = HAL_InitializeDIOPort( hal::createPortHandleForSPI( 17 ),
                                                       false,
                                                       &tmpStatus ) ) ==
      HAL_kInvalidHandle )
      {
         //wpi::outs() << "Failed to allocate DIO 17\n";
         HAL_FreeDIOPort( digitalHandles[5] );  // free the first Port_ allocated
         HAL_FreeDIOPort( digitalHandles[6] );  // free the second Port_ allocated
         HAL_FreeDIOPort( digitalHandles[7] );  // free the third Port_ allocated
         return;
      }
      hal::digitalSystem->writeEnableMXPSpecialFunction( hal::digitalSystem->readEnableMXPSpecialFunction( &tmpStatus ) | 0x00F0,
                                                         &tmpStatus );
      Handle_ = open( "/dev/spidev1.0",
                      O_RDWR );
      if( Handle_ < 0 )
      {
         std::printf( "Failed to open SPI Port_ %d: %s\n",
                      Port_,
                      std::strerror( errno ) );
         HAL_FreeDIOPort( digitalHandles[5] );  // free the first Port_ allocated
         HAL_FreeDIOPort( digitalHandles[6] );  // free the second Port_ allocated
         HAL_FreeDIOPort( digitalHandles[7] );  // free the third Port_ allocated
         HAL_FreeDIOPort( digitalHandles[8] );  // free the fourth Port_ allocated
         return;
      }
      HAL_SetSPIHandle( HAL_SPI_kMXP,
                        Handle_ );
   }
#endif

   LOG_TRACE( "%s - Constructor End",
         GetName().c_str() );
}

Team271SPI::~Team271SPI( void )
{
#if 0
   int32_t tmpStatus = 0;

   HAL_FreeSPIAuto( Port_,
                    &tmpStatus );

   close( Handle_ );

   if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS0 )
   {
      // Case 0 does not need to do anything
      CommonSPIPortFree();
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS1 )
   {
      CommonSPIPortFree();
      HAL_FreeDIOPort( digitalHandles[0] );
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS2 )
   {
      CommonSPIPortFree();
      HAL_FreeDIOPort( digitalHandles[1] );
   }
   else if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS3 )
   {
      CommonSPIPortFree();
      HAL_FreeDIOPort( digitalHandles[2] );
   }
   else if( Port_ == TEAM271_SPI_PORT_MXP )
   {
      CommonSPIPortFree();
      HAL_FreeDIOPort( digitalHandles[5] );
      HAL_FreeDIOPort( digitalHandles[6] );
      HAL_FreeDIOPort( digitalHandles[7] );
      HAL_FreeDIOPort( digitalHandles[8] );
   }
#endif
}

void Team271SPI::Transaction( const uint8_t* const argTXBuf,
                              const uint32_t argTXCount,
                              uint8_t* const argRXBuf,
                              const uint32_t argRXCount )
{
   if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS0 )
   {
   }
   else if( Port_ == TEAM271_SPI_PORT_MXP )
   {
   }
}

void Team271SPI::Read( const uint8_t argRegister,
                       uint8_t* const argBuf,
                       const uint32_t argCount )
{
   Transaction( &argRegister,
                1U,
                argBuf,
                argCount );
}

void Team271SPI::Read( uint8_t* const argBuf,
                       const uint32_t argCount )
{
   if( argBuf != nullptr )
   {

   }
   else if( argCount < 1U )
   {

   }
   else
   {
      if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS0 )
      {
      }
      else if( Port_ == TEAM271_SPI_PORT_MXP )
      {
      }
   }
}

void Team271SPI::Write( uint8_t* const argBuf,
                        const uint32_t argCount )
{
   if( argBuf != nullptr )
   {

   }
   else if( argCount < 1U )
   {

   }
   else
   {
      if( Port_ == TEAM271_SPI_PORT_ONBOARD_CS0 )
      {
      }
      else if( Port_ == TEAM271_SPI_PORT_MXP )
      {
      }
   }
}

void Team271SPI::Write( const uint8_t argRegister,
                        const uint8_t argData )
{
   uint8_t tmpBuf[2] = { argRegister,
                         argData };

   Write( tmpBuf,
          2U );
}
#endif

#endif

