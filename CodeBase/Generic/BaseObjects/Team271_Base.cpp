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

Team271Base::Team271Base( Team271Base* const argParent,
                          const string& argName,
                          const uint8_t argMessageCount )
      : Team271NetworkObject( argName,
                              argMessageCount )


{
#if defined( ENABLE_DEBUG_PRINTING_TRACE )
   if( Parent_ == nullptr )
   {
      DEBUG_PRINT( GetClassName().c_str(),
                   "Constructor" );
   }
   else
   {
      DEBUG_PRINT( GetClassName().c_str(),
                   "Constructor - Parent: %s",
                   Parent_->GetName() );
   }
#endif

   /*
    * Clear Out Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      Children_[i] = nullptr;
   }

   if( argParent != nullptr )
   {
      argParent->ChildAdd( this );
   }
}

Team271Base::~Team271Base( void )
{
#if 0
   /*
    * Delete Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         delete Children_[i];
      }

      Children_[i] = nullptr;
   }
#endif
}

/*
 *
 * Getters
 *
 */
const string Team271Base::GetClassName( void ) const
{
   return "Team271Base";
}

Team271Base* Team271Base::GetParent( void ) const
{
   return Parent_;
}

const string Team271Base::GetPath( void ) const
{
   return GetPath( GetClassName() );
}

const string Team271Base::GetPath( const string& argClassName ) const
{
   string tmpPath = "(" + GetClassName() + ")" + GetName();
   const Team271Base* tmpOwner = GetParent();

   while( tmpOwner != nullptr )
   {
      tmpPath.insert( 0,
                      "(" + tmpOwner->GetClassName() + ")" + tmpOwner->GetName() + "/" );

      tmpOwner = tmpOwner->GetParent();
   }

   return tmpPath;
}

uint32_t Team271Base::GetDirty( void ) const
{
   /*
    * Base Call
    */
   uint8_t tmpDirty = Team271NetworkObject::GetDirty();

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( (Children_[i] != nullptr) && (Children_[i]->GetDirty() == 1) )
      {
         tmpDirty = 1U;
         break;
      }
   }

   return tmpDirty;
}

uint32_t Team271Base::GetDirtyString( const uint32_t argIndex ) const
{
   /*
    * Base Call
    */
   uint8_t tmpDirty = Team271NetworkObject::GetDirtyString( argIndex );

   /*
    * Update Children
    */
/*
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( (Children_[i] != nullptr) && (Children_[i]->GetDirtyString( argIndex ) == 1) )
      {
         tmpDirty = 1U;
         break;
      }
   }
*/
   return tmpDirty;
}

uint32_t Team271Base::GetDirty( const uint32_t argIndex ) const
{
   /*
    * Base Call
    */
   uint8_t tmpDirty = Team271NetworkObject::GetDirty( argIndex );

   /*
    * Update Children
    */
/*
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( (Children_[i] != nullptr) && (Children_[i]->GetDirty( argIndex ) == 1) )
      {
         tmpDirty = 1U;
         break;
      }
   }
*/
   return tmpDirty;
}

/*
 *
 * Setters
 *
 */
void Team271Base::SetParent( Team271Base* const argParent )
{
   Parent_ = argParent;
}

void Team271Base::SetDirty( void )
{
   /*
    * Base Call
    */
   Team271NetworkObject::SetDirty();

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->SetDirty();
      }
   }
}

void Team271Base::ClearDirty( void )
{
   /*
    * Base Call
    */
   Team271NetworkObject::ClearDirty();

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->ClearDirty();
      }
   }
}

/*
 *
 * Base
 *
 */
void Team271Base::Init( void )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->Init();
      }
   }
}

/*
 *
 * Robot
 *
 */
void Team271Base::RobotInit( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_TRACE( "%s - RobotInit",
            GetName().c_str() );

      LOG_MATCH_TRACE( "%s - RobotInit",
            GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->RobotInit();
      }
   }
}

void Team271Base::PreRobotPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->PreRobotPeriodic();
      }
   }
}

void Team271Base::PostRobotPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->PostRobotPeriodic();
      }
   }
}

/*
 *
 * Disabled
 *
 */
void Team271Base::DisabledInit( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_TRACE( "%s - DisabledInit",
            GetName().c_str() );

      LOG_MATCH_TRACE( "%s - DisabledInit",
            GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->DisabledInit();
      }
   }
}

void Team271Base::DisabledPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->DisabledPeriodic();
      }
   }
}

/*
 *
 * Autonomous
 *
 */
void Team271Base::AutonomousInit( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_TRACE( "%s - AutonomousInit",
            GetName().c_str() );

      LOG_MATCH_TRACE( "%s - AutonomousInit",
            GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->AutonomousInit();
      }
   }
}

void Team271Base::AutonomousPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->AutonomousPeriodic();
      }
   }
}

/*
 *
 * Teleop
 *
 */
void Team271Base::TeleopInit( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_TRACE( "%s - TeleopInit",
            GetName().c_str() );

      LOG_MATCH_TRACE( "%s - TeleopInit",
            GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->TeleopInit();
      }
   }
}

void Team271Base::TeleopPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->TeleopPeriodic();
      }
   }
}

/*
 *
 * Test
 *
 */
void Team271Base::TestInit( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_TRACE( "%s - TestInit",
            GetName().c_str() );

      LOG_MATCH_TRACE( "%s - TestInit",
            GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->TestInit();
      }
   }
}

void Team271Base::TestPeriodic( const bool argIsParent )
{
   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->TestPeriodic();
      }
   }
}

/*
 *
 * Robot State
 *
 */
void Team271Base::UpdateInput( const bool argIsParent )
{
   TEAM271_DSO_STATE tmpDSOState = TEAM271_DSO_STATE_NONE;

   /*
    *
    * Get the DSO State
    *
    */
#if defined( T271_INPUT_ENABLE )
   tmpDSOState = Team271Input::InstanceGetDSOState();
#endif

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->SetDSO( tmpDSOState );
      }
   }
}

void Team271Base::ClearStickyFaults( const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      LOG_NOTICE( "%s",
                  GetName().c_str() );

      LOG_MATCH_NOTICE( "%s",
                        GetName().c_str() );
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->ClearStickyFaults();
      }
   }
}

void Team271Base::SetDSO( const TEAM271_DSO_STATE argIsDSO,
                          const bool argIsParent )
{
   if( argIsDSO == TEAM271_DSO_STATE_ACTIVE )
   {
      /*
       * Log Data
       */
      if( argIsParent == true )
      {
         LOG_NOTICE( "%s - True",
                     GetName().c_str() );

         LOG_MATCH_NOTICE( "%s - True",
                           GetName().c_str() );
      }
   }
   else
   {
      /*
       * Log Data
       */
      if( argIsParent == true )
      {
         LOG_NOTICE( "%s - False",
                     GetName().c_str() );

         LOG_MATCH_NOTICE( "%s - False",
                           GetName().c_str() );
      }
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->SetDSO( argIsDSO );
      }
   }
}

void Team271Base::PIDControllerState( const uint32_t argEnable,
                                      const bool argIsParent )
{
   /*
    * Log Data
    */
   if( argIsParent == true )
   {
      if( argEnable == TRUE )
      {
         LOG_NOTICE( "%s - Enabled",
                     GetName().c_str() );

         LOG_MATCH_NOTICE( "%s - Enabled",
                           GetName().c_str() );
      }
      else
      {
         LOG_NOTICE( "%s - Disabled",
                     GetName().c_str() );

         LOG_MATCH_NOTICE( "%s - Disabled",
                           GetName().c_str() );
      }
   }

   /*
    * Update Children
    */
   for( uint8_t i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
   {
      if( Children_[i] != nullptr )
      {
         Children_[i]->PIDControllerState( argEnable );
      }
   }
}

/*
 *
 * Child Functions
 *
 */
int32_t Team271Base::ChildAdd( Team271Base* const argChild )
{
   uint8_t i = 0;
   uint8_t tmpIndex = -1;

   if( argChild == nullptr )
   {
      LOG_ERROR( "Invalid Child - Parent: %s",
                 GetName().c_str() );
   }
   else if( argChild->GetParent() != nullptr )
   {
      LOG_ERROR( "Child Already Has Owner - Child: %s | New Parent: %s | Existing Parent: %s",
                 argChild->GetName().c_str(),
                 GetName().c_str(),
                 argChild->GetParent()->GetName().c_str() );
   }
   else
   {
      /*
       * Find Free Child Slot
       */
      for( i = 0; i < T271_BASE_MAX_CHILDREN; i++ )
      {
         /*
          * Check if the Child Node is free
          */
         if( Children_[i] == nullptr )
         {
            /*
             * Add Child
             */
            Children_[i] = argChild;

            IncUInt32( t271_base_data_index::T271_BASE_DATA_CHILDREN );

            /*
             * Set the Child's Owner
             */
            Children_[i]->SetParent( this );

            /*
             * Store Index to Return
             */
            tmpIndex = i;

            /*
             * Break Out of the Loop
             */
            break;
         }
      }

      if( i >= T271_BASE_MAX_CHILDREN )
      {
         /*
          * Log Data
          */
         LOG_ERROR( "Max Children Reached - Parent: %s | Child: %s",
                    GetName().c_str(),
                    argChild->GetName().c_str() );
      }
   }

   return tmpIndex;
}
