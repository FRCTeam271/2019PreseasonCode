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

Team271PublisherRobot::Team271PublisherRobot( void )
      : Team271NetworkPublisher( "RobotPub",
                                 T271_ZMQ_PUBSYNC_ADDR,
                                 T271_ZMQ_PUB_ADDR )

{

}

Team271PublisherRobot::~Team271PublisherRobot( void )
{

}

/*
 *
 * Getters
 *
 */
const string Team271PublisherRobot::GetClassName( void ) const
{
   return "Team271PublisherRobot";
}

uint32_t Team271PublisherRobot::GetDirty( void ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

uint32_t Team271PublisherRobot::GetDirty( const uint32_t argIndex ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

/*
 *
 * Setters
 *
 */
void Team271PublisherRobot::SetDirty( void )
{
   GlobalNetworkObject_.SetDirty();

#if defined( T271_INPUT_ENABLE )
   Team271Input::Instance().SetDirty();
#endif

   for( uint32_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->SetDirty();
      }
   }
}

void Team271PublisherRobot::ClearDirty( void )
{
   GlobalNetworkObject_.ClearDirty();

#if defined( T271_INPUT_ENABLE )
   Team271Input::Instance().ClearDirty();
#endif

   for( uint32_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->ClearDirty();
      }
   }
}

/*
 *
 * Publisher
 *
 */
void Team271PublisherRobot::Pub( void )
{
   multipart_t tmpMsgSend;

   /*
    *
    * - Calculate the Time since our last Network Message
    * - Update the Message Timestamp
    *
    */
   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP_DELTA,
                               Team271Robot::GetTimeDelta( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP ) ) );

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP,
                               Team271Robot::GetTime() );

   /*
    *
    * Create Message
    * - Send Global Messages
    * - Send Input Messages
    * - Send BaseManager Messages
    *
    */
   GlobalNetworkObject_.CreateMessage( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP ),
                                       tmpMsgSend );

   tmpMsgSend.send( Pub_ );

#if defined( T271_INPUT_ENABLE )
   tmpMsgSend.clear();

   Team271Input::Instance().CreateMessage( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP ),
                                           tmpMsgSend );

   tmpMsgSend.send( Pub_ );
#endif

   Team271BaseManager::InstanceCreateMessages( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_NETWORK_TIME_STAMP ) );
}
