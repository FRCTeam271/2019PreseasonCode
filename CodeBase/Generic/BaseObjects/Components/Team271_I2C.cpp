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

#if defined(T271_I2C_ENABLE)

static uint8_t i2COnboardObjCount { 0 };
static uint8_t i2CMXPObjCount { 0 };

Team271I2C::Team271I2C( const TEAM271_I2C_PORT argPort,
                        const uint32_t argDeviceAddress )
      : Port_( argPort ),
        DeviceAddress_( argDeviceAddress )
{
   LOG_TRACE( "%s - Constructor Start",
              GetName().c_str() );

   int32_t tmpStatus = 0;

   if( Port_ == TEAM271_I2C_PORT_ONBOARD )
   {
      if( i2COnboardObjCount == 0 )
      {
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
         OnBoardHandle_ = open( "/dev/i2c-2",
                                O_RDWR );
#endif

         if( OnBoardHandle_ < 0 )
         {
            LOG_TRACE( "%s - Failed to open onboard i2c bus: %s",
                       GetName().c_str(),
                       strerror( errno ) );
         }
         else
         {
            ++i2COnboardObjCount;
         }
      }
   }
   else if( Port_ == TEAM271_I2C_PORT_MXP )
   {
      if( i2CMXPObjCount == 0 )
      {
#if defined( T271_IO_ENABLE )
         if( ( Team271IO::getInstance().InitDIOChannel( 24,
                                                        false ) ) == 0 )
         {
            if( ( Team271IO::getInstance().InitDIOChannel( 25,
                                                           false ) ) == 0 )
            {
               Team271IO::getInstance().WriteEnableMXPSpecialFunction( Team271IO::getInstance().ReadEnableMXPSpecialFunction( &tmpStatus ) | 0xC000,
                                                                       &tmpStatus );

#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
               MXPHandle_ = open( "/dev/i2c-1",
                                  O_RDWR );
#endif

               if( MXPHandle_ < 0 )
               {
                  LOG_TRACE( "%s - Failed to open MXP i2c bus: %s",
                             GetName().c_str(),
                             strerror( errno ) );
               }
               else
               {
                  ++i2CMXPObjCount;
               }
            }
            else
            {
               Team271IO::getInstance().FreeDIOChannel( 24 );
            }
         }
#endif
      }
   }

   LOG_TRACE( "%s - Constructor End",
              GetName().c_str() );
}

Team271I2C::~Team271I2C( void )
{
   if( Port_ == TEAM271_I2C_PORT_ONBOARD )
   {
      if( OnBoardHandle_ >= 0 )
      {
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
         close( OnBoardHandle_ );
#endif

         i2COnboardObjCount = 0;
      }
   }
   else if( Port_ == TEAM271_I2C_PORT_MXP )
   {
      if( MXPHandle_ >= 0 )
      {
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
         close( MXPHandle_ );
#endif
      }

#if defined( T271_IO_ENABLE )
      Team271IO::getInstance().FreeDIOChannel( 24 );
      Team271IO::getInstance().FreeDIOChannel( 25 );
#endif

      i2CMXPObjCount = 0;
   }
}

void Team271I2C::Transaction( const uint8_t* const argTXBuf,
                              const uint32_t argTXCount,
                              uint8_t* const argRXBuf,
                              const uint32_t argRXCount ) const
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   struct i2c_msg msgs[2];
   msgs[0].addr = DeviceAddress_;
   msgs[0].flags = 0;
   msgs[0].len = argTXCount;
   msgs[0].buf = const_cast<uint8_t*>( argTXBuf );
   msgs[1].addr = DeviceAddress_;
   msgs[1].flags = I2C_M_RD;
   msgs[1].len = argRXCount;
   msgs[1].buf = argRXBuf;

   struct i2c_rdwr_ioctl_data rdwr;
   rdwr.msgs = msgs;
   rdwr.nmsgs = 2;

   if( Port_ == TEAM271_I2C_PORT_ONBOARD )
   {
      ioctl( OnBoardHandle_,
             I2C_RDWR,
             &rdwr );
   }
   else if( Port_ == TEAM271_I2C_PORT_MXP )
   {
      ioctl( MXPHandle_,
             I2C_RDWR,
             &rdwr );
   }
#endif
}

void Team271I2C::Read( const uint8_t argRegister,
                       uint8_t* const argBuf,
                       const uint32_t argCount ) const
{
   Transaction( &argRegister,
                1U,
                argBuf,
                argCount );
}

void Team271I2C::Read( uint8_t* const argBuf,
                       const uint32_t argCount ) const
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   if( argBuf == nullptr )
   {

   }
   else if( argCount == 0U )
   {

   }
   else
   {

      struct i2c_msg msg;
      msg.addr = DeviceAddress_;
      msg.flags = I2C_M_RD;
      msg.len = argCount;
      msg.buf = argBuf;

      struct i2c_rdwr_ioctl_data rdwr;
      rdwr.msgs = &msg;
      rdwr.nmsgs = 1;

      if( Port_ == TEAM271_I2C_PORT_ONBOARD )
      {
         ioctl( OnBoardHandle_,
                I2C_RDWR,
                &rdwr );
      }
      else if( Port_ == TEAM271_I2C_PORT_MXP )
      {
         ioctl( MXPHandle_,
                I2C_RDWR,
                &rdwr );
      }
   }
#endif
}

void Team271I2C::Write( uint8_t* const argBuf,
                        const uint32_t argCount ) const
{
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   if( argBuf == nullptr )
   {

   }
   else if( argCount == 0U )
   {

   }
   else
   {
      struct i2c_msg msg;
      msg.addr = DeviceAddress_;
      msg.flags = 0;
      msg.len = argCount;
      msg.buf = const_cast<uint8_t*>( argBuf );

      struct i2c_rdwr_ioctl_data rdwr;
      rdwr.msgs = &msg;
      rdwr.nmsgs = 1;

      if( Port_ == TEAM271_I2C_PORT_ONBOARD )
      {
         ioctl( OnBoardHandle_,
                I2C_RDWR,
                &rdwr );
      }
      else if( Port_ == TEAM271_I2C_PORT_MXP )
      {
         ioctl( MXPHandle_,
                I2C_RDWR,
                &rdwr );
      }
   }
#endif
}

void Team271I2C::Write( const uint8_t argRegister,
                        const uint8_t argData ) const
{
   uint8_t tmpBuf[2] = { argRegister,
                         argData };

   Write( tmpBuf,
          2U );
}

#endif

