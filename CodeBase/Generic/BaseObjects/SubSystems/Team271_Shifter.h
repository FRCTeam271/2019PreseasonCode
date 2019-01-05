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

#ifndef TEAM271_SHIFTER_H_
#define TEAM271_SHIFTER_H_

#if defined( T271_SHIFTER_ENABLE )

typedef enum : uint8_t
{
   TEAM271_GEAR_1,
   TEAM271_GEAR_2,
   TEAM271_GEAR_3,
   TEAM271_GEAR_4,

   TEAM271_GEAR_COUNT
} eGear;

class t271_shifter_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /* Currently Engaged Gear */
      T271_SHIFTER_DATA_GEAR_CURRENT = t271_base_data_index::T271_DATA_COUNT,
      T271_SHIFTER_DATA_GEAR_PREV,

      /* Speed Modifier to apply to the Transmission when switching into this Gear */
      T271_SHIFTER_DATA_SPEED_MODIFIER,

      /* How long in seconds to apply the Speed Modifier */
      T271_SHIFTER_DATA_SPEED_MODIFIER_TIME,

      T271_DATA_COUNT,
   } t271_shifter_data_index_t;
};

class Team271Shifter : public Team271Base
{
protected:
   /* List of all the Solenoids for the Gears */
   Team271Solenoid* SolTransShifter_[TEAM271_GEAR_COUNT];

   const uint8_t IsSolCreator_ = 0;

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271Shifter( Team271Base* const argParent,
                   const string& argName,
                   const uint32_t argCreateSolenoids = 1 );

   virtual ~Team271Shifter( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   eGear GetGear( void ) const;

   const string GetGearName( void ) const;

   virtual const string GetGearName( const eGear argGear ) const;

   float GetSpeedModifier( void ) const;

   float GetSpeedModifierTime( void ) const;

   /*
    *
    * Setters
    *
    */
   void SetGear( const eGear argGear );

   /*
    *
    * Robot
    *
    */
   void PreRobotPeriodic( const bool argIsParent = false ) override;

   void PostRobotPeriodic( const bool argIsParent = false ) override;

public:
   /*
    *
    * Getters
    *
    */
   virtual const bool GetSolState( const eGear argGear,
                                   const uint32_t argIndex ) const;
};

#endif

#endif /* TEAM271_SHIFTER_H_ */
