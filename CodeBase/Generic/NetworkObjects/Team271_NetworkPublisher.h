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

#ifndef TEAM271_NETWORKPUBLISHER_H_
#define TEAM271_NETWORKPUBLISHER_H_

class Team271NetworkPublisher
{
protected:
   string Name_ = "";
   string AddrPubSync_ = "";
   string AddrPub_ = "";

   uint32_t PubSyncSNDHWM_ = 100;
   socket_t PubSync_;
   uint8_t PubSyncConnected_ = 0;
   uint8_t PubSyncConnectedPrev_ = 0;

   float TimeLastPubSync = 0;

   uint32_t PubSNDHWM_ = 100;
   socket_t Pub_;
   uint8_t PubConnected_ = 0;
   uint8_t PubConnectedPrev_ = 0;

   uint32_t NetworkMessageLoopCount_ = 0;

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271NetworkPublisher( const string& argName,
                            const string& argAddrPubSync,
                            const string& argAddrPub );

   virtual ~Team271NetworkPublisher( void );

   /*
    *
    * Getters
    *
    */
   virtual const string GetClassName( void ) const;

   const string& GetName( void ) const;

   socket_t& GetSocket( void );

   uint32_t GetConnected( void ) const;

   virtual uint32_t GetDirty( void ) const;

   virtual uint32_t GetDirty( const uint32_t argIndex ) const;

   /*
    *
    * Setters
    *
    */
   void SetName( const string& argName );

   virtual void SetDirty( void );

   virtual void ClearDirty( void );

   /*
    *
    * Publisher
    *
    */
   void PubLoop( void );

   virtual void Pub( void );
};

#endif /* TEAM271_NETWORKPUBLISHER_H_ */
