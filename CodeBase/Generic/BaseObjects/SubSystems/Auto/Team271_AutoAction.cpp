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

Team271AutoAction::Team271AutoAction( const eAutoActionType argAction,
                                      const float argActionLength,
                                      const float argActionTimeout )
      : AutoAction_( argAction ),
        ActionLength_( argActionLength ),
        Timeout_( argActionTimeout )

{
   /*
    * Reset the Action
    */
   Reset();
}

Team271AutoAction::~Team271AutoAction( void )
{

}

void Team271AutoAction::Reset( void )
{
   /*
    * Time
    */
   StartTime_ = 0;
   ElapsedTime_ = 0;

   /*
    * Action
    */
   AutoActionState_ = eAutoActionStateNone;
}

/*
 * This Function gets called once before we start Processing
 */
void Team271AutoAction::Start( void )
{
   /*
    * Set Start Time
    */
   StartTime_ = Team271Robot::GetTime();
}

float Team271AutoAction::GetActionLength( void ) const
{
   return ActionLength_;
}

eAutoActionState Team271AutoAction::GetActionState( void ) const
{
   return AutoActionState_;
}

eAutoActionState Team271AutoAction::Process( const float argListStartTime,
                                             const float argListElapsedTime )
{
   /*
    * Check if this is the Start of the Action
    */
   if( AutoActionState_ == eAutoActionStateNone )
   {
      /*
       * Set the State to Processing
       */
      AutoActionState_ = eAutoActionStateProcessing;

      /*
       * Call the Start function once
       */
      Start();
   }

   /*
    * Calculate Times
    */
   ElapsedTime_ = Team271Robot::GetTimeDelta( StartTime_ );

   /*
    * Check Action Length
    */
   if( ( ElapsedTime_ >= ActionLength_ ) && ( fabs( ActionLength_ - 0 ) < kTeam271Epsilon ) )
   {
      /*
       * Action was time based and it's completed successfully
       */
      AutoActionState_ = eAutoActionStateDone;
   }

   /*
    * Check Timeout
    */
   else if( ( ElapsedTime_ > Timeout_ ) && ( fabs( Timeout_ - 0 ) < kTeam271Epsilon ) )
   {
      /*
       * Action Timed out
       */
      AutoActionState_ = eAutoActionStateDoneTimeout;
   }

   /*
    *
    * Process Action
    *
    */

   /*
    * Do Nothing/Delay
    */
   if( ( AutoAction_ == eAutoActionTypeNone ) || ( AutoAction_ == eAutoActionTypeDelay ) )
   {
      /*
       * Do Nothing Until we Reach the Action Length/Timeout
       */
   }

   return AutoActionState_;
}

eAutoActionState Team271AutoAction::ProcessEnd( const float argStartTime,
                                                const float argElapsedTime )
{
   /*
    * Check if the Action is Done
    */
   if( ( AutoActionState_ == eAutoActionStateDone ) || ( AutoActionState_ == eAutoActionStateDoneTimeout ) )
   {
      /*
       * Action is done, we need to call the End function once
       */
      End();
   }

   return AutoActionState_;
}

/*
 * This Function gets called once before after we are done Processing
 */
void Team271AutoAction::End( void )
{

}

#endif
