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

Team271AutoActionLadder::Team271AutoActionLadder( const eAutoActionType argAction,
                                                  const float argActionLength,
                                                  const float argLadder,
                                                  const float argArm,
                                                  const float argActionTimeout )
      : Team271AutoAction( argAction, argActionLength, argActionTimeout )
{
   /*
    *
    * Reset the Action
    *
    */
   Reset();

   /*
    *
    * Set Initial Parameters
    *
    */
   HerderSetpoint_ = 0;

   /*
    *
    * Handle Ladder Parameters
    *
    */
   LadderSetpoint_ = argLadder;
   ArmSetpoint_ = argArm;
}

void Team271AutoActionLadder::Reset( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Reset();
}

void Team271AutoActionLadder::Start( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Start();

   /*
    * Set Herder Outtake
    */
#if defined(T271_HERDER_ENABLE)
   if( Herder_ != nullptr )
   {
      if( AutoAction_ == eAutoActionTypeHerderOuttake )
      {
         Herder_->SetHerderWheelState( T271_HERDER_WHEELS_OUT_AUTO );
      }

      /*
       * Set Herder Intake
       */
      else if( AutoAction_ == eAutoActionTypeHerderIntake )
      {
         Herder_->SetHerderWheelState( T271_HERDER_WHEELS_IN );
      }
   }
#endif
}

eAutoActionState Team271AutoActionLadder::Process( const float argListStartTime,
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
    * Set Ladder and ARM Elevation
    */
   if( AutoAction_ == eAutoActionTypeLadderTime )
   {
      /*
       * Wait till the Ladder Action is over
       */
   }

   /*
    * Set Ladder Elevation
    */
   else if( AutoAction_ == eAutoActionTypeLadderElevationTime )
   {
      /*
       * Wait till the Ladder Action is over
       */
   }

   /*
    * Set Arm Elevation
    */
   else if( AutoAction_ == eAutoActionTypeHerderElevationTime )
   {
      /*
       * Wait till the Arm Action is over
       */
   }

   /*
    * Set Herder Open
    */
   else if( AutoAction_ == eAutoActionTypeHerderOpen )
   {
#if defined(T271_HERDER_ENABLE)
      if( Herder_ != nullptr )
      {
         Herder_->SetHerderGrabState( T271_HERDER_GRAB_OPEN );
      }
#endif

   }

   /*
    * Set Herder Close
    */
   else if( AutoAction_ == eAutoActionTypeHerderClose )
   {
#if defined(T271_HERDER_ENABLE)
      if( Herder_ != nullptr )
      {
         Herder_->SetHerderGrabState( T271_HERDER_GRAB_CLOSE );
      }
#endif
   }

   /*
    * Set Herder Outtake
    */
   else if( AutoAction_ == eAutoActionTypeHerderOuttake )
   {
      /*
       * Wait till the Herder Action is over
       */
   }

   /*
    * Set Herder Intake
    */
   else if( AutoAction_ == eAutoActionTypeHerderIntake )
   {
      /*
       * Wait till the Herder Action is over
       */
   }

   /*
    * Call the Ladder Class
    */
#if defined(T271_LADDER_ENABLE)
   if( Ladder_ != nullptr )
   {
      Ladder_->SetSetpoint( LadderSetpoint_ );
   }
#endif

   /*
    * Call the Arm Class
    */
#if defined( T271_ARM_ENABLE )
   if( Arm_ != nullptr )
   {
      Arm_->SetSetpoint( ArmSetpoint_ );
   }
#endif

   return AutoActionState_;
}

void Team271AutoActionLadder::End( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::End();

   /*
    * Stop the Ladder
    */
#if defined(T271_LADDER_ENABLE)
   if( Ladder_ != nullptr )
   {
      Ladder_->SetSetpoint( 0 );
   }
#endif

   /*
    * Call the Arm Class
    */
#if defined( T271_ARM_ENABLE )
   if( Arm_ != nullptr )
   {
      Arm_->SetSetpoint( 0 );
   }
#endif

   /*
    * Call the Herder Class
    */
#if defined(T271_HERDER_ENABLE)
   if( Herder_ != nullptr )
   {
      /*
       * Turn the Herder Off
       */
      Herder_->SetHerderWheelState( T271_HERDER_WHEELS_OFF );
   }
#endif
}

#endif
