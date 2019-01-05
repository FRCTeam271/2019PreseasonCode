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

#ifndef TEAM271_AUTO_ACTION_H_
#define TEAM271_AUTO_ACTION_H_

#if defined(T271_AUTO_ENABLE)

#define AUTO_MAX_SUB_ACTIONS      ( 5 )

typedef enum
{
   eAutoActionStateNone = 0,
   eAutoActionStateProcessing,
   eAutoActionStateDone,
   eAutoActionStateDoneTimeout,
}eAutoActionState;

typedef enum
{
   eAutoActionTypeNone = 0,
   eAutoActionTypeDelay,
   eAutoActionTypeLock,

   eAutoActionTypeDriveTime,
   eAutoActionTypeDriveTimeGame,
   eAutoActionTypeDriveTurnGame,
   eAutoActionTypeDriveEncoder,
   eAutoActionTypeDriveGyro,
   eAutoActionTypeDriveEncoderGyro,
   eAutoActionTypeDriveVisionGyro,
   eAutoActionTypeDrivePathFinder,

   eAutoActionTypeLadderTime,

   eAutoActionTypeLadderElevationTime,
   eAutoActionTypeLadderElevationEncoder,

   eAutoActionTypeHerderElevationTime,
   eAutoActionTypeHerderElevationEncoder,

   eAutoActionTypeHerderOpen,
   eAutoActionTypeHerderClose,

   eAutoActionTypeHerderOuttake,
   eAutoActionTypeHerderIntake,
   eAutoActionTypeHerderStop,
}eAutoActionType;

class Team271AutoAction
{
protected:
   /*
    * Action
    */
   eAutoActionType AutoAction_ = eAutoActionTypeNone;
   eAutoActionState AutoActionState_ = eAutoActionStateNone;

   /*
    * SubActions
    */
   uint32_t AutoSubActionsIndex = 0U;
   Team271AutoAction* AutoSubActions_[AUTO_MAX_SUB_ACTIONS];

   /*
    * Time
    */
   float StartTime_ = 0;
   float ElapsedTime_ = 0;

   float ActionLength_ = 0;
   float Timeout_ = 0;

public:
   /*
    * Functions
    */
   Team271AutoAction( const eAutoActionType argAction,
                      const float argActionLength = 0,
                      const float argActionTimeout = 0 );

   virtual ~Team271AutoAction( void );

   virtual void Reset( void );

   float GetActionLength( void ) const;
   eAutoActionState GetActionState( void ) const;

   virtual void Start( void );

   virtual eAutoActionState Process( const float argStartTime,
                                     const float argElapsedTime );

   virtual eAutoActionState ProcessEnd( const float argStartTime,
                                        const float argElapsedTime );

   virtual void End( void );
};

#endif

#endif
