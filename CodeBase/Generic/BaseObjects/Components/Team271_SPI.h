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

#ifndef TEAM271_SPI_H_
#define TEAM271_SPI_H_

#if defined( T271_SPI_ENABLE )

typedef enum
{
   TEAM271_SPI_PORT_ONBOARD_CS0,
   TEAM271_SPI_PORT_ONBOARD_CS1,
   TEAM271_SPI_PORT_ONBOARD_CS2,
   TEAM271_SPI_PORT_ONBOARD_CS3,
   TEAM271_SPI_PORT_MXP,

   TEAM271_SPI_PORT_COUNT
}TEAM271_SPI_PORT;

#if !defined(T271_DASHBOARD)
class Team271SPI
{
protected:
   /*
    *
    * Variables
    *
    */
   TEAM271_SPI_PORT Port_ = TEAM271_SPI_PORT_ONBOARD_CS0;

   int32_t Handle_ = -1;

public:

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271SPI( const TEAM271_SPI_PORT argPort );

   virtual ~Team271SPI( void );

   /*
    *
    * Output Functions
    *
    */
   void Transaction( const uint8_t* const argTXBuf,
                     const uint32_t argTXCount,
                     uint8_t* const argRXBuf,
                     const uint32_t argRXCount );

   void Read( const uint8_t argRegister,
              uint8_t* const argBuf,
              const uint32_t argCount );

   void Read( uint8_t* const argBuf,
              const uint32_t argCount );

   void Write( uint8_t* const argBuf,
               const uint32_t argCount );

   void Write( const uint8_t argRegister,
               const uint8_t argData );
};
#endif

#endif

#endif /* TEAM271_MOTOR_H_ */
