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

#if defined(T271_AUTO_ENABLE)

Team271AutoMode::Team271AutoMode( void )
      : Team271Base( nullptr,
                     "AutoMode" )
{
   LOG_TRACE( "%s - Constructor Start",
              "Team271AutoMode" );

   LOG_TRACE( "%s - Constructor End",
              "Team271AutoMode" );
}

void Team271AutoMode::AutonomousInit( void )
{
   /*
    * Set Start Time
    */
   StartTime_ = Team271Robot::GetTime();
}

void Team271AutoMode::AutonomousPeriodic( void )
{
   /*
    * Base Call
    */
   Team271Base::AutonomousPeriodic();

   /*
    * Calculate Times
    */
   const double tmpElapsedTime = Team271Robot::GetTimeDelta( StartTime_ );

   /*
    * Run Auto
    */
   if( _CurrentAutoListAction < AUTO_MAX_ACTIONS )
   {
      const eAutoActionListState tmpActionState = _AutoActionLists[_CurrentAutoListAction].Process( StartTime_,
                                                                                                    tmpElapsedTime );
      if( (tmpActionState == eAutoActionListStateDone) || (tmpActionState == eAutoActionListStateDoneTimeout) )
      {
         ++_CurrentAutoListAction;
      }
   }
}

void Team271AutoMode::Reset( void )
{
   /*
    * Reset This List of Auto Actions
    */
   for( uint32_t i = 0; i < AUTO_MAX_LIST_ACTIONS; i++ )
   {
      _AutoActionLists[i].Reset();
   }

   /*
    * Time
    */
   StartTime_ = 0;

   /*
    * Action
    */
   _AutoModeState = eAutoModeStateNone;
}

void Team271AutoMode::CreateAuto( void )
{
   /*
    * Reset Auto Modes and Moves
    */
   for( uint32_t i = 0; i < AUTO_MAX_LIST_ACTIONS; i++ )
   {
      _AutoActionLists[i].Reset();
   }
}

AutoActionList* Team271AutoMode::GetAutoActions( const uint32_t argAutoAction )
{
   AutoActionList* tmpReturn = nullptr;

   if( argAutoAction < AUTO_MAX_LIST_ACTIONS )
   {
      tmpReturn = &_AutoActionLists[argAutoAction];
   }

   return tmpReturn;
}

AutoActionList* Team271AutoMode::GetNextAutoActions( void )
{
   ++_AddAutoListAction;

   return GetAutoActions( _AddAutoListAction );
}

#endif
