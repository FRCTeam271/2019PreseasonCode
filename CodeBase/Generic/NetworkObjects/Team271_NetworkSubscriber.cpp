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

Team271NetworkSubscriber::Team271NetworkSubscriber( const string& argName,
                                                    const string& argAddrSync,
                                                    const string& argAddr,
                                                    const string& argSubPattern )
      : Name_( argName ),
        AddrSync_( argAddrSync ),
        Addr_( argAddr ),
        SubPattern_( argSubPattern ),
        Sync_( ZMQContext_,
               socket_type::req ),
        Socket_( ZMQContext_,
                 socket_type::sub )

{
   uint32_t tmpLINGER = 0;

   /*
    * Setup ZMQ Sub Sync
    */
   Sync_.setsockopt( ZMQ_SNDHWM,
                        &SyncSNDHWM_,
                        sizeof(SyncSNDHWM_) );

   Sync_.setsockopt( ZMQ_LINGER,
                        &tmpLINGER,
                        sizeof( tmpLINGER ) );

   Sync_.connect( AddrSync_ );

   /*
    * Setup ZMQ Sub
    */
   Socket_.setsockopt( ZMQ_SNDHWM,
                    &SNDHWM_,
                    sizeof(SNDHWM_) );

   Socket_.setsockopt( ZMQ_LINGER,
                    &tmpLINGER,
                    sizeof( tmpLINGER ) );

   Socket_.connect( Addr_ );

   Socket_.setsockopt( ZMQ_SUBSCRIBE,
                       argSubPattern.c_str(),
                       argSubPattern.length() );
}

Team271NetworkSubscriber::~Team271NetworkSubscriber( void )
{
   /*
    * ZMQ Pub
    */
   Socket_.disconnect( Addr_ );

   Socket_.close();

   /*
    * ZMQ Pub Sync
    */
   if( Sync_.connected() == true )
   {
      multipart_t tmpMsgSend;

      tmpMsgSend.addstr( "End" );

      /*
       * Send the Message
       */
      tmpMsgSend.send( Sync_ );
   }

   Sync_.disconnect( AddrSync_ );

   Sync_.close();
}

/*
 *
 * Getters
 *
 */
const string Team271NetworkSubscriber::GetClassName( void ) const
{
   return "Team271NetworkSubscriber";
}

const string& Team271NetworkSubscriber::GetName( void ) const
{
   return Name_;
}

socket_t& Team271NetworkSubscriber::GetSocket( void )
{
   return Socket_;
}

uint32_t Team271NetworkSubscriber::GetConnected( void ) const
{
   return (uint32_t)( ( SyncConnected_ == true ) && ( SyncConnected_ == true ) );
}

uint32_t Team271NetworkSubscriber::GetDirty( void ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

uint32_t Team271NetworkSubscriber::GetDirty( const uint32_t argIndex ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

/*
 *
 * Setters
 *
 */
void Team271NetworkSubscriber::SetName( const string& argName )
{

}

void Team271NetworkSubscriber::SetDirty( void )
{

}

void Team271NetworkSubscriber::ClearDirty( void )
{
   /*
    * Clear Dirty Bits
    */
   GlobalNetworkObject_.ClearDirty();

#if defined( T271_INPUT_ENABLE )
   Team271Input::Instance().ClearDirty();
#endif

   for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->ClearDirty();
      }
   }
}

/*
 *
 * Subscriber
 *
 */
uint32_t Team271NetworkSubscriber::SubLoop( void )
{
   uint32_t tmpPacketCount = 0;
   uint32_t tmpByteCount = 0;

   SyncConnectedPrev_ = SyncConnected_;
   ConnectedPrev_ = Connected_;

   /*
    *
    * Check Sync Port
    *
    */
   if( Sync_.connected() == true )
   {
      if( SyncConnected_ == 0 )
      {
         /*
          * Only Send a Sync Attempt Once Every Second
          */
         if( Team271Robot::CompareTime( TimeLastSync_,
                                        1 ) == 1 )
         {
            multipart_t tmpMsgSend;

            tmpMsgSend.addstr( "Start" );

            /*
             * Send the Message
             */
            tmpMsgSend.send( Sync_ );
         }
      }

      /*
       * Check if we've Received Data
       */
      multipart_t tmpMsgRecv;

      if( tmpMsgRecv.recv( Sync_,
                           ZMQ_DONTWAIT ) == true )
      {
         const string tmpData = tmpMsgRecv.popstr();

         if( tmpData == "Team271" )
         {
            SyncConnected_ = 1;
            Connected_ = 0;

            TimeLastSync_ = Team271Robot::GetTime();
         }
         else
         {
            SyncConnected_ = 0;
            Connected_ = 0;
         }
      }
   }
   else
   {
      SyncConnected_ = 0;
      Connected_ = 0;
   }

   /*
    * Check if we're connected
    */
   if( SyncConnected_ == 1 )
   {
      if( Socket_.connected() == true )
      {
         /*
          * - Store Previous Connected State
          * - Set Connected
          */
         Connected_ = 1;

         /*
          * Check if we've Received Data
          */
         multipart_t tmpMsg;

         while( tmpMsg.recv( Socket_,
                             ZMQ_DONTWAIT ) == true )
         {
            ++tmpPacketCount;

            /*
             * Count the number of Bytes in the Message
             */
            for( uint16_t i = 0; i < tmpMsg.size(); i++ )
            {
               tmpByteCount += tmpMsg.peek( i )->size();
            }

            /*
             * - Read Global Messages
             * - Read Input Messages
             * - Read BaseManager Messages
             */
            GlobalNetworkObject_.DecodeMessage( tmpMsg );

#if defined( T271_INPUT_ENABLE )
            Team271Input::Instance().DecodeMessage( tmpMsg );
#endif

            Team271BaseManager::InstanceDecodeMessages( tmpMsg );

#if defined(T271_DASHBOARD) || defined(T271_SIM)
            //std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
#endif
         }
      }
      else
      {
         /*
          * Set Disconnected
          */
         SyncConnected_ = 0;
         Connected_ = 0;
      }
   }

   if( tmpPacketCount != 0 )
   {
      return tmpByteCount / tmpPacketCount;
   }

   return 0;
}

uint32_t Team271NetworkSubscriber::Sub( void )
{
   return 0;
}
