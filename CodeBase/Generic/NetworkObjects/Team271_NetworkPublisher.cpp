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

Team271NetworkPublisher::Team271NetworkPublisher( const string& argName,
                                                  const string& argAddrPubSync,
                                                  const string& argAddrPub )
      : Name_( argName ),
        AddrPubSync_( argAddrPubSync ),
        AddrPub_( argAddrPub ),
        PubSync_( ZMQContext_,
                  socket_type::rep ),
        Pub_( ZMQContext_,
              socket_type::pub )

{
   uint32_t tmpLINGER = 0;

   /*
    * Setup ZMQ Pub Sync
    */
   PubSync_.setsockopt( ZMQ_SNDHWM,
                        &PubSyncSNDHWM_,
                        sizeof( PubSyncSNDHWM_ ) );

   PubSync_.setsockopt( ZMQ_LINGER,
                        &tmpLINGER,
                        sizeof( tmpLINGER ) );

   PubSync_.bind( AddrPubSync_ );

   /*
    * Setup ZMQ Pub
    */
   Pub_.setsockopt( ZMQ_SNDHWM,
                    &PubSNDHWM_,
                    sizeof( PubSNDHWM_ ) );

   Pub_.setsockopt( ZMQ_LINGER,
                    &tmpLINGER,
                    sizeof( tmpLINGER ) );

   Pub_.bind( AddrPub_ );
}

Team271NetworkPublisher::~Team271NetworkPublisher( void )
{
   /*
    * ZMQ Pub
    */
   Pub_.unbind( AddrPub_ );

   Pub_.close();

   /*
    * ZMQ Pub Sync
    */
   if( PubSync_.connected() == true )
   {
      multipart_t tmpMsgSend;

      tmpMsgSend.addstr( "End" );

      /*
       * Send the Message
       */
      tmpMsgSend.send( PubSync_ );
   }

   PubSync_.unbind( AddrPubSync_ );

   PubSync_.close();
}

/*
 *
 * Getters
 *
 */
const string Team271NetworkPublisher::GetClassName( void ) const
{
   return "Team271NetworkPublisher";
}

const string& Team271NetworkPublisher::GetName( void ) const
{
   return Name_;
}

socket_t& Team271NetworkPublisher::GetSocket( void )
{
   return Pub_;
}

uint32_t Team271NetworkPublisher::GetConnected( void ) const
{
   return (uint32_t)( ( PubSyncConnected_ == true ) && ( PubSyncConnected_ == true ) );
}

uint32_t Team271NetworkPublisher::GetDirty( void ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

uint32_t Team271NetworkPublisher::GetDirty( const uint32_t argIndex ) const
{
   uint8_t tmpHasDirty = 0;

   return tmpHasDirty;
}

/*
 *
 * Setters
 *
 */
void Team271NetworkPublisher::SetName( const string& argName )
{

}

void Team271NetworkPublisher::SetDirty( void )
{

}

void Team271NetworkPublisher::ClearDirty( void )
{

}

/*
 *
 * Publisher
 *
 */
void Team271NetworkPublisher::PubLoop( void )
{
   uint8_t tmpForceDirty = 0;

   PubSyncConnectedPrev_ = PubSyncConnected_;
   PubConnectedPrev_ = PubConnected_;

   /*
    *
    * Check Sync Port
    *
    */
   if( PubSync_.connected() == true )
   {
      /*
       * Check if we've Received Data
       */
      multipart_t tmpMsgRecv;

      if( tmpMsgRecv.recv( PubSync_,
                           ZMQ_DONTWAIT ) == true )
      {
         const string tmpData = tmpMsgRecv.popstr();

         /*
          * Check the Data in the Message
          * If it's ever a Start Sync then Start/Restart the Connection Process
          */
         if( tmpData == "Start" )
         {
            /*
             * Send A Message Back
             */
            multipart_t tmpMsgSend;

            tmpMsgSend.addstr( "Team271" );

            tmpMsgSend.send( PubSync_ );

            PubSyncConnected_ = 1;
            PubConnected_ = 0;

            tmpForceDirty = 1;

            TimeLastPubSync = Team271Robot::GetTime();

            LOG_NOTICE( "%s - Received Start Sync",
                        GetName().c_str() );
         }
         else if( tmpData == "End" )
         {
            multipart_t tmpMsgSend;

            tmpMsgSend.addstr( "Team271" );

            tmpMsgSend.send( PubSync_ );

            LOG_NOTICE( "%s - Received End",
                        GetName().c_str() );

            PubSyncConnected_ = 0;
            PubConnected_ = 0;
         }
         else
         {
            LOG_NOTICE( "%s - Received Start Sync Error",
                        GetName().c_str() );

            PubSyncConnected_ = 0;
            PubConnected_ = 0;
         }
      }
   }

   if( ( PubSyncConnected_ != PubSyncConnectedPrev_ ) && ( PubSyncConnected_ == 0 ) )
   {
      LOG_NOTICE( "%s - Sync Disconnect",
                  GetName().c_str() );

      PubSyncConnected_ = 0;
      PubConnected_ = 0;
   }

   /*
    * Check if we should Resync All Data
    * - Be careful as this is A LOT of data
    */
   if( ( ( PubConnected_ != PubConnectedPrev_ ) && ( PubConnected_ == 0 ) ) || ( tmpForceDirty == 1 ) )
   {
      SetDirty();
   }

   /*
    *
    * If we've received a Sync Start then start Publishing Data
    *
    */
   if( PubSyncConnected_ == 1 )
   {
      if( Pub_.connected() == true )
      {
         /*
          * Set Connected
          */
         PubConnected_ = 1;

         if( PubConnected_ != PubConnectedPrev_ )
         {
            LOG_NOTICE( "%s - Pub Connected",
                        GetName().c_str() );
         }

         /*
          * Publish Data
          */
         Pub();

         /*
          * Update Loop Count
          */
         ++NetworkMessageLoopCount_;
      }
      else
      {
         if( PubConnected_ != PubConnectedPrev_ )
         {
            LOG_NOTICE( "%s - Pub Disconnect",
                        GetName().c_str() );
         }

         PubConnected_ = 0;
      }
   }
   else
   {
      if( PubConnected_ != PubConnectedPrev_ )
      {
         LOG_NOTICE( "%s - Pub Disconnect",
                     GetName().c_str() );
      }

      PubConnected_ = 0;
   }
}

void Team271NetworkPublisher::Pub( void )
{

}
