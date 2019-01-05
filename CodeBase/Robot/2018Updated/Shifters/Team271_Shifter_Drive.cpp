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

#if defined( T271_SHIFTER_ENABLE )

Team271Shifter_Drive::Team271Shifter_Drive( Team271Base* const argParent,
                                            const uint32_t argCreateSolenoids )
      : Team271Shifter( argParent,
                        "Drive Shifter",
                        argCreateSolenoids )
{

}

Team271Shifter_Drive::~Team271Shifter_Drive( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271Shifter_Drive::GetClassName( void ) const
{
   return "Team271Shifter_Drive";
}

const string Team271Shifter_Drive::GetGearName( const eGear argGear ) const
{
   string tmpReturnValue = "Invalid";

   if( argGear >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Gear Index",
                 GetName().c_str(),
                 argGear );
   }
   else if( argGear == TEAM271_GEAR_1 )
   {
      tmpReturnValue = "Low";
   }
   else if( argGear == TEAM271_GEAR_2 )
   {
      tmpReturnValue = "High";
   }

   return tmpReturnValue;
}

const bool Team271Shifter_Drive::GetSolState( const eGear argGear,
                                              const uint32_t argIndex ) const
{
   const bool tmpSolState[TEAM271_GEAR_COUNT][TEAM271_GEAR_COUNT] = { { true,
                                                                        false,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        true,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        false,
                                                                        false,
                                                                        false },
                                                                      { false,
                                                                        false,
                                                                        false,
                                                                        false } };

   bool tmpReturn = false;

   if( argGear >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Gear Index",
                 GetName().c_str(),
                 argGear );
   }
   else if( argIndex >= TEAM271_GEAR_COUNT )
   {
      LOG_ERROR( "%s - Invalid Sol Index",
                 GetName().c_str(),
                 argGear );
   }
   else
   {
      tmpReturn = tmpSolState[argGear][argIndex];
   }

   return tmpReturn;
}

/*
 *
 * Base
 *
 */
void Team271Shifter_Drive::Init( void )
{
   if( IsSolCreator_ == 1 )
   {
      /*
       * Low Gear
       */
      SolTransShifter_[DRIVE_LOW] = Team271BaseManager::InstanceAdd( new Team271Solenoid( this,
                                                                                          GetGearName( (eGear)DRIVE_LOW ),
                                                                                          PCM_TRANS_SPEED,
                                                                                          SOL_TRANS_SPEED_1 ) );

      /*
       * High Gear
       */
      SolTransShifter_[DRIVE_HIGH] = Team271BaseManager::InstanceAdd( new Team271Solenoid( this,
                                                                                           GetGearName( (eGear)DRIVE_HIGH ),
                                                                                           PCM_TRANS_SPEED,
                                                                                           SOL_TRANS_SPEED_2 ) );
   }

   SetGear( (eGear)DRIVE_HIGH );
}
#endif
