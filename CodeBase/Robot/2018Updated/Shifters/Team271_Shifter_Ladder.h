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

#ifndef TEAM271_SHIFTER_LADDER_H_
#define TEAM271_SHIFTER_LADDER_H_

typedef enum : uint8_t
{
   LADDER_LOW = TEAM271_GEAR_1,
   LADDER_HIGH = TEAM271_GEAR_2,
   LADDER_GEAR_MAX
} eLadderGear;

typedef enum : uint8_t
{
   LADDER_BRAKE_OFF = TEAM271_GEAR_1,
   LADDER_BRAKE_ON = TEAM271_GEAR_2,
   LADDER_BRAKE_MAX
} eLadderBrake;

#if defined( T271_SHIFTER_ENABLE )

/*
 * Speed
 */
class Team271Shifter_LadderSpeed : public Team271Shifter
{
public:
   Team271Shifter_LadderSpeed( Team271Base* const argParent );

   virtual ~Team271Shifter_LadderSpeed( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   const string GetGearName( const eGear argGear ) const override;

   /*
    *
    * Base
    *
    */
   void Init( void ) override;

public:
   /*
    *
    * Getters
    *
    */
   const bool GetSolState( const eGear argGear,
                           const uint32_t argIndex ) const override;
};

/*
 * Brake
 */
class Team271Shifter_LadderBrake : public Team271Shifter
{
public:
   Team271Shifter_LadderBrake( Team271Base* const argParent );

   virtual ~Team271Shifter_LadderBrake( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   const string GetGearName( const eGear argGear ) const override;

   /*
    *
    * Base
    *
    */
   void Init( void ) override;

public:
   /*
    *
    * Getters
    *
    */
   const bool GetSolState( const eGear argGear,
                           const uint32_t argIndex ) const override;
};

#endif

#endif /* TEAM271_SHIFTER_LADDER_H_ */
