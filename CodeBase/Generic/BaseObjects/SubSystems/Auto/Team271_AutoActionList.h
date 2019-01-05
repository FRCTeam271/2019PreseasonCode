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

#ifndef TEAM271_AUTO_ACTION_LIST_H_
#define TEAM271_AUTO_ACTION_LIST_H_

#if defined(T271_AUTO_ENABLE)

#define AUTO_MAX_ACTIONS      ( 5 )

typedef enum
{
   eAutoActionListStateNone = 0,
   eAutoActionListStateInit,
   eAutoActionListStateProcessing,
   eAutoActionListStateDone,
   eAutoActionListStateDoneTimeout,
} eAutoActionListState;

class AutoActionList
{
private:
   /*
    * Time
    */
   double StartTime_ = 0;

public:
   /*
    * Action
    */
   eAutoActionListState _AutoActionListState = eAutoActionListStateNone;
   Team271AutoAction* _AutoActions[AUTO_MAX_ACTIONS];

   AutoActionList( void );

   /*
    * Functions
    */
   void Reset( void );

   Team271AutoAction* AddAutoAction( Team271AutoAction* const argAutoAction );

   void Start( void );

   bool IsDone( void ) const;

   eAutoActionListState Process( const float argStartTime,
                                 const float argElapsedTime );
};

#endif

#endif
