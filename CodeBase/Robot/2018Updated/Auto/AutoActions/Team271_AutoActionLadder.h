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

#ifndef TEAM271_AUTO_ACTION_LADDER_H_
#define TEAM271_AUTO_ACTION_LADDER_H_

#if defined(T271_AUTO_ENABLE)

class Team271AutoActionLadder : public Team271AutoAction
{
protected:
   /*
    * Ladder
    */
   float LadderSetpoint_ = 0;
   float ArmSetpoint_ = 0;

   /*
    * Herder
    */
   float HerderSetpoint_ = 0;

public:
   /*
    * Functions
    */
   Team271AutoActionLadder( const eAutoActionType argAction,
                            const float argActionLength = 0,
                            const float argLadder = 0,
                            const float argArm = 0,
                            const float argActionTimeout = 0 );

   void Reset( void ) override;

   void Start( void ) override;

   eAutoActionState Process( const float argStartTime,
                             const float argElapsedTime ) override;

   void End( void ) override;
};

#endif

#endif
