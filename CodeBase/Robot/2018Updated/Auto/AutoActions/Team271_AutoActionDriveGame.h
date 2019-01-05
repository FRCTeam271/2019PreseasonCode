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

#ifndef TEAM271_AUTO_ACTION_DRIVE_GAME_H_
#define TEAM271_AUTO_ACTION_DRIVE_GAME_H_

#if defined(T271_AUTO_ENABLE)

class Team271AutoActionDriveGame : public Team271AutoActionDrive
{
protected:
   /*
    * Time
    */
   float ActionLengthRight_ = 0.0;
   float ActionLengthLeft_ = 0.0;

   /*
    * Drive Output
    */
   float DriverLeft_ = 0.0;
   float PassengerLeft_ = 0.0;

public:
   /*
    * Functions
    */
   Team271AutoActionDriveGame( const float argActionLengthRight,
                               const float argDriverRight,
                               const float argPassengerRight,
                               const float argActionLengthLeft,
                               const float argDriverLeft,
                               const float argPassengerLeft,
                               const float argActionTimeout = 0 );

   void Start( void ) override;

   eAutoActionState Process( const float argStartTime,
                             const float argElapsedTime ) override;
};

#endif

#endif
