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

AutoActionList::AutoActionList( void )
{
   /*
    *
    * Clear the Action List
    *
    */
   for( uint32_t i = 0; i < AUTO_MAX_ACTIONS; i++ )
   {
      _AutoActions[i] = nullptr;
   }

   /*
    *
    * Reset the Action List
    *
    */
   Reset();
}

void AutoActionList::Reset( void )
{
   /*
    *
    * Reset This List of Auto Actions
    *
    */
   for( uint32_t i = 0; i < AUTO_MAX_ACTIONS; i++ )
   {
      if( _AutoActions[i] != nullptr )
      {
         _AutoActions[i]->Reset();
      }
   }

   /*
    * Time
    */
   StartTime_ = 0;

   /*
    * Action
    */
   _AutoActionListState = eAutoActionListStateNone;
}

Team271AutoAction* AutoActionList::AddAutoAction( Team271AutoAction* const argAutoAction )
{
   Team271AutoAction* tmpReturnAutoAction = nullptr;

   /*
    *
    * Find the next open Action and Store IT
    *
    */
   for( uint32_t i = 0; i < AUTO_MAX_ACTIONS; i++ )
   {
      /*
       * Check if the Action is in use
       */
      if( _AutoActions[i] == nullptr )
      {
         /*
          * Create the Action
          */
         _AutoActions[i] = argAutoAction;

         tmpReturnAutoAction = _AutoActions[i];

         break;
      }
   }

   return tmpReturnAutoAction;
}

void AutoActionList::Start( void )
{
   /*
    *
    * Reset the Action List
    *
    */
   Reset();

   /*
    * Update Init State
    */
   _AutoActionListState = eAutoActionListStateInit;
}

bool AutoActionList::IsDone( void ) const
{
   if( (_AutoActionListState == eAutoActionListStateDone) || (_AutoActionListState == eAutoActionListStateDoneTimeout) )
   {
      return true;
   }

   return false;
}

eAutoActionListState AutoActionList::Process( const float argStartTime,
                                              const float argElapsedTime )
{
   uint32_t tmpIsDone = 1U;
   uint32_t tmpHadTimeout = 0U;
   eAutoActionState tmpState = eAutoActionStateNone;

   /*
    * Set Start Time
    */
   if( _AutoActionListState == eAutoActionListStateInit )
   {
      StartTime_ = Team271Robot::GetTime();
   }

   /*
    * Calculate Times
    */
   const double tmpElapsedTime = Team271Robot::GetTimeDelta( StartTime_ );

   /*
    *
    * Process this List of Auto Actions
    *
    */
   for( uint32_t i = 0; i < AUTO_MAX_ACTIONS; i++ )
   {
      if( _AutoActions[i] != nullptr )
      {
         _AutoActions[i]->Process( StartTime_,
                                   tmpElapsedTime );

         tmpState = _AutoActions[i]->ProcessEnd( StartTime_,
                                                 tmpElapsedTime );

         if( (tmpState == eAutoActionStateProcessing) && (_AutoActions[i]->GetActionLength() > 0) )
         {
            tmpIsDone = 0;
         }
         else if( tmpState == eAutoActionStateDoneTimeout )
         {
            tmpHadTimeout = 1U;
         }
      }
   }

   /*
    * Update Init State
    */
   if( _AutoActionListState == eAutoActionListStateInit )
   {
      _AutoActionListState = eAutoActionListStateProcessing;
   }
   else if( tmpIsDone == 1 )
   {
      /*
       * Check that All Actions called Done
       */
      for( uint32_t i = 0; i < AUTO_MAX_ACTIONS; i++ )
      {
         if( _AutoActions[i] != nullptr )
         {
            if( _AutoActions[i]->GetActionState() == eAutoActionStateProcessing )
            {
               _AutoActions[i]->End();
            }
         }
      }

      if( tmpHadTimeout == 1 )
      {
         _AutoActionListState = eAutoActionListStateDoneTimeout;
      }
      else
      {
         _AutoActionListState = eAutoActionListStateDone;
      }
   }

   return _AutoActionListState;
}

#endif
