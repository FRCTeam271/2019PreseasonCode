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

#if defined( T271_AUTO_ENABLE )

Team271AutoActionDriveGame::Team271AutoActionDriveGame( const float argActionLengthRight,
                                                        const float argDriverRight,
                                                        const float argPassengerRight,
                                                        const float argActionLengthLeft,
                                                        const float argDriverLeft,
                                                        const float argPassengerLeft,
                                                        const float argActionTimeout )
      : Team271AutoActionDrive( eAutoActionTypeDriveTimeGame,
                                argActionLengthRight,
                                argDriverRight,
                                argPassengerRight,
                                argActionTimeout )
{
   /*
    *
    * Handle Drive Parameters
    *
    */
   ActionLengthRight_ = argActionLengthRight;
   ActionLengthLeft_ = argActionLengthLeft;

   DriverLeft_ = argDriverLeft;
   PassengerLeft_ = argPassengerLeft;
}

void Team271AutoActionDriveGame::Start( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoActionDrive::Start();

   /*
    *
    * Check if we want this Drive Action to change Gears
    *
    */
   if( Auto_ != nullptr )
   {
      if( Auto_->IsLeft() )
      {
         ActionLength_ = ActionLengthLeft_;
      }
      else
      {
         ActionLength_ = ActionLengthRight_;
      }
   }
}

eAutoActionState Team271AutoActionDriveGame::Process( const float argListStartTime,
                                                      const float argListElapsedTime )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Process( argListStartTime,
                               argListElapsedTime );

   /*
    *
    * Process Action
    *
    */

   /*
    *
    * Call Drive Class
    *
    */
#if defined(T271_DRIVE_ENABLE)
   if( (Auto_ != nullptr) && (Drive_ != nullptr) )
   {
      if( Auto_->IsLeft() )
      {
         ActionLength_ = ActionLengthLeft_;

         Drive_->SetSetpoint( DriverLeft_,
                              PassengerLeft_ );
      }
      else
      {
         ActionLength_ = ActionLengthRight_;

         Drive_->SetSetpoint( Driver_,
                              Passenger_ );
      }
   }
#endif

   return AutoActionState_;
}
#endif
