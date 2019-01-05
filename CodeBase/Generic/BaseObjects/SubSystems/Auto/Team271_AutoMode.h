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

#ifndef TEAM271_AUTO_MODE_H_
#define TEAM271_AUTO_MODE_H_

#if defined(T271_AUTO_ENABLE)

#define AUTO_MAX_LIST_ACTIONS   ( 30 )

typedef enum
{
   eAutoModeStateNone = 0,
   eAutoModeStateInit,
   eAutoModeStateProcessing,
   eAutoModeStateDone,
   eAutoModeStateDoneTimeout,
} eAutoModeState;

class Team271AutoMode : public Team271Base
{
private:
   /*
    * Variables
    */
   uint32_t _AutoMode = 0;

   eAutoModeState _AutoModeState = eAutoModeStateNone;

   uint32_t _AddAutoListAction = 0;
   uint32_t _CurrentAutoListAction = 0;
   AutoActionList _AutoActionLists[AUTO_MAX_LIST_ACTIONS];

   /*
    * Time
    */
   float StartTime_ = 0;

public:
   Team271AutoMode( void );

   /*
    *
    * Autonomous
    *
    */
   virtual void AutonomousInit( void );
   void AutonomousPeriodic( void );

   /*
    * Auto Mode
    */
   void Reset( void );

   virtual void CreateAuto( void );

   void AddAutoActions( const uint32_t argAutoMode );

   AutoActionList* GetAutoActions( const uint32_t argAutoMode );
   AutoActionList* GetNextAutoActions( void );

   eAutoModeState Process( void );
};

#endif

#endif /* TEAM271_AUTO_MODE_H_ */
