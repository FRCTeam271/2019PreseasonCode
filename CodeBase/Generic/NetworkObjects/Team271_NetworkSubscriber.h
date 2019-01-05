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

#ifndef TEAM271_NETWORKSUBSCRIBER_H_
#define TEAM271_NETWORKSUBSCRIBER_H_

class Team271NetworkSubscriber
{
protected:
   string Name_ = "";
   string AddrSync_ = "";
   string Addr_ = "";
   string SubPattern_ = "";

   uint32_t SyncSNDHWM_ = 100;
   socket_t Sync_;
   uint8_t SyncConnected_ = 0;
   uint8_t SyncConnectedPrev_ = 0;

   float TimeLastSync_ = 0;

   uint32_t SNDHWM_ = 100;
   socket_t Socket_;
   uint8_t Connected_ = 0;
   uint8_t ConnectedPrev_ = 0;

   uint32_t NetworkMessageLoopCount_ = 0;

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271NetworkSubscriber( const string& argName,
                             const string& argAddrSync,
                             const string& argAddr,
                             const string& argSubPattern = "" );

   virtual ~Team271NetworkSubscriber( void );

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
    * Subscriber
    *
    */
   uint32_t SubLoop( void );

   virtual uint32_t Sub( void );
};

#endif /* TEAM271_NETWORKSUBSCRIBER_H_ */
