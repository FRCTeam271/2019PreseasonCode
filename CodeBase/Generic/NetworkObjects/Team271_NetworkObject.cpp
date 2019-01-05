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

t271_network_string_data_index::t271_network_string_data_index( const uint32_t argIndex )
      : Index_( argIndex % T271_NETWORK_OBJECT_MAX_STRING_DATA_ITEMS )
{
   assert( Index_ < T271_NETWORK_STRING_DATA_END );
}

t271_network_data_index::t271_network_data_index( const uint32_t argIndex )
      : StoreIndex_( argIndex / T271_NETWORK_OBJECT_MAX_DATA_ITEMS ),
        Index_( argIndex % T271_NETWORK_OBJECT_MAX_DATA_ITEMS )
{
   assert( StoreIndex_ < T271_NETWORK_OBJECT_MAX_DATA_STORES );

   assert( Index_ < T271_NETWORK_DATA_END );
}

Team271NetworkObject::Team271NetworkObject( const string& argName,
                                            const uint8_t argMessageCount,
                                            const uint8_t argNetworkID )
      : NumNetworkMessages_( argMessageCount ),
        NumDataStores_( ( argMessageCount / T271_NETWORK_OBJECT_MAX_DATA_ITEMS ) + 1U ),
        NetworkID_( argNetworkID )

{
   assert( NumNetworkMessages_ < (T271_NETWORK_OBJECT_MAX_DATA_STORES * T271_NETWORK_OBJECT_MAX_DATA_ITEMS) );

   //cout << "Name: " << argName << " | NumNetworkMessages_: " << to_string(NumNetworkMessages_) << " | NumDataStores_: " << to_string(NumDataStores_) << endl;

   /*
    * Initialize String Data Stores
    */
   memset( StringDataStores_,
           0,
           sizeof( StringDataStores_ ) );

   /*
    * Set Name
    */
   StringDataStores_[0] = argName;

   /*
    * Initialize Data Stores
    */
   memset( DataStoreDirty_,
           0,
           sizeof( DataStoreDirty_ ) );

   memset( DataStores_,
           0,
           sizeof( DataStores_ ) );

   /*
    * Setup Data Markers
    * These will fail t271_network_data_index assert
    */
   DataStores_[0][0].udata32 = 0xA5A5A5A5U;
   DataStoreDirty_[0] |= 1U;
}

Team271NetworkObject::~Team271NetworkObject( void )
{

}

/*
 *
 * Getters
 *
 */
uint32_t Team271NetworkObject::GetNetworkID( void ) const
{
   return NetworkID_;
}

const string Team271NetworkObject::GetClassName( void ) const
{
   return "Team271NetworkObject";
}

const string& Team271NetworkObject::GetName( void ) const
{
   return StringDataStores_[0];
}

uint32_t Team271NetworkObject::GetDirty( void ) const
{
   uint8_t tmpHasDirty = 0;

   if( StringDataStoreDirty_ != 0 )
   {
      tmpHasDirty = 1U;
   }

   for( uint8_t j = 0; j < NumDataStores_; j++ )
   {
      if( DataStoreDirty_[j] != 0 )
      {
         tmpHasDirty = 1U;
         break;
      }
   }

   return tmpHasDirty;
}

uint32_t Team271NetworkObject::GetDirtyString( const uint32_t argIndex ) const
{
   const t271_network_data_index tmpIndex( argIndex );

   uint8_t tmpHasDirty = 0;

    if( (StringDataStoreDirty_ & ( 1U << argIndex )) != 0U )
    {
       tmpHasDirty = 1U;
    }

   return tmpHasDirty;
}

uint32_t Team271NetworkObject::GetDirty( const uint32_t argIndex ) const
{
   const t271_network_data_index tmpIndex( argIndex );

   uint8_t tmpHasDirty = 0;

    if( (DataStoreDirty_[tmpIndex.StoreIndex_] & ( 1U << tmpIndex.Index_ )) != 0U )
    {
       tmpHasDirty = 1U;
    }

   return tmpHasDirty;
}

/*
 *
 * Setters
 *
 */
void Team271NetworkObject::SetNetworkID( const uint32_t argID )
{
   NetworkID_ = argID;
}

void Team271NetworkObject::SetName( const string& argName )
{
   StringDataStores_[0] = argName;

   StringDataStoreDirty_ |= 1U;
}

void Team271NetworkObject::SetDirty( void )
{
   StringDataStoreDirty_ = 0xFF;

   for( uint32_t i = 0; i < NumNetworkMessages_; i++ )
   {
      const t271_network_data_index tmpIndex( i );

      DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );
   }
}

void Team271NetworkObject::ClearDirty( void )
{
   StringDataStoreDirty_ = 0;

   for( uint8_t i = 0; i < NumDataStores_; i++ )
   {
      DataStoreDirty_[i] = 0;
   }
}

void Team271NetworkObject::Set( const uint32_t argIndexA,
                                const uint32_t argIndexB )
{
   const t271_network_data_index tmpIndexA( argIndexA );
   const t271_network_data_index tmpIndexB( argIndexB );

   if( DataStores_[tmpIndexA.StoreIndex_][tmpIndexA.Index_].udata32 != DataStores_[tmpIndexB.StoreIndex_][tmpIndexB.Index_].udata32 )
   {
      DataStores_[tmpIndexA.StoreIndex_][tmpIndexA.Index_].udata32 = DataStores_[tmpIndexB.StoreIndex_][tmpIndexB.Index_].udata32;

      DataStoreDirty_[tmpIndexA.StoreIndex_] |= ( 1U << tmpIndexA.Index_ );
   }
}

void Team271NetworkObject::Set( const uint32_t argIndex,
                                const Team271NetworkObject& argNetworkObj )
{
   const t271_network_data_index tmpIndex( argIndex );

   if( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 != argNetworkObj.DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 )
   {
      DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 = argNetworkObj.DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;

      DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );
   }
}

void Team271NetworkObject::Set( const uint32_t argIndex,
                                const Team271NetworkObject* const argNetworkObj )
{
   const t271_network_data_index tmpIndex( argIndex );

   if( argNetworkObj != nullptr )
   {
      if( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 != argNetworkObj->DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 )
      {
         DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32 = argNetworkObj->DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;

         DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );
      }
   }
}

/*
 *
 * Readers
 *
 */
const string& Team271NetworkObject::ReadString( const uint32_t argIndex ) const
{
   const t271_network_string_data_index tmpIndex( argIndex );

   return StringDataStores_[tmpIndex.Index_];
}

/*
 *
 * Writers
 *
 */
const string& Team271NetworkObject::Write( const uint32_t argIndex,
                                           const string& argData )
{
   const t271_network_string_data_index tmpIndex( argIndex );

   if( StringDataStores_[tmpIndex.Index_] != argData )
   {
      StringDataStores_[tmpIndex.Index_] = argData;

      StringDataStoreDirty_ |= ( 1U << tmpIndex.Index_ );
   }

   return StringDataStores_[tmpIndex.Index_];
}

float Team271NetworkObject::Write( const uint32_t argIndex,
                                   const float argData )
{
   const t271_network_data_index tmpIndex( argIndex );

   if( ( fabs( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf - argData ) >= kTeam271Epsilon ) )
   {
      DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf = argData;

      DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );
   }

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf;
}

/*
 *
 * Incrementers
 *
 */
uint8_t Team271NetworkObject::IncUInt8( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata8;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata8;
}

uint16_t Team271NetworkObject::IncUInt16( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata16;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata16;
}

uint32_t Team271NetworkObject::IncUInt32( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;
}

int8_t Team271NetworkObject::IncInt8( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data8;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data8;
}

int16_t Team271NetworkObject::IncInt16( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data16;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data16;
}

int32_t Team271NetworkObject::IncInt32( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data32;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data32;
}

float Team271NetworkObject::IncFloat( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   ++DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf;
}

/*
 *
 * Decrementers
 *
 */
uint8_t Team271NetworkObject::DecUInt8( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata8;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata8;
}

uint16_t Team271NetworkObject::DecUInt16( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata16;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata16;
}

uint32_t Team271NetworkObject::DecUInt32( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].udata32;
}

int8_t Team271NetworkObject::DecInt8( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data8;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data8;
}

int16_t Team271NetworkObject::DecInt16( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data16;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data16;
}

int32_t Team271NetworkObject::DecInt32( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data32;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].data32;
}

float Team271NetworkObject::DecFloat( const uint32_t argIndex )
{
   const t271_network_data_index tmpIndex( argIndex );

   --DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf;

   DataStoreDirty_[tmpIndex.StoreIndex_] |= ( 1U << tmpIndex.Index_ );

   return DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_].dataf;
}

/*
 *
 * Network
 *
 */
void Team271NetworkObject::CreateMessage( const float argTimeStamp,
                                          multipart_t& argMessage )
{
   uint8_t tmpHasDirty = 0;

   TimeStampLast_ = argTimeStamp;

   if( StringDataStoreDirty_ != 0 )
   {
      tmpHasDirty = 1U;
   }

   for( uint8_t j = 0; j < NumDataStores_; j++ )
   {
      if( DataStoreDirty_[j] != 0 )
      {
         tmpHasDirty = 1U;
         break;
      }
   }

   if( tmpHasDirty == 1U )
   {
      argMessage.addtyp( NetworkID_ );

      argMessage.addtyp( TimeStampLast_ );

      argMessage.addtyp( StringDataStoreDirty_ );

      for( uint8_t i = 0; i < T271_NETWORK_OBJECT_MAX_STRING_DATA_ITEMS; i++ )
      {
         if( ( StringDataStoreDirty_ & ( 1U << i ) ) != 0U )
         {
            argMessage.addstr( StringDataStores_[i] );
         }
      }

      //StringDataStoreDirty_ = 0;

      for( uint8_t j = 0; j < NumDataStores_; j++ )
      {
         argMessage.addtyp( DataStoreDirty_[j] );

         for( uint32_t i = 0; i < T271_NETWORK_OBJECT_MAX_DATA_ITEMS; i++ )
         {
            if( ( DataStoreDirty_[j] & ( 1U << i ) ) != 0U )
            {
               argMessage.addtyp( DataStores_[j][i].udata32 );
            }
         }

         //DataStoreDirty_[j] = 0;
      }
   }
}

void Team271NetworkObject::DecodeMessage( multipart_t& argMessage )
{
   if( ( argMessage.size() > 0 ) && ( argMessage.peek( 0 )->size() == sizeof( NetworkID_ ) ) )
   {
      const uint16_t tmpNetworkID = argMessage.peektyp<uint8_t>( 0 );

      if( tmpNetworkID == NetworkID_ )
      {
         argMessage.poptyp<uint8_t>();

         TimeStampLast_ = argMessage.poptyp<float>();

         StringDataStoreDirty_ = argMessage.poptyp<uint8_t>();

         for( uint8_t i = 0; i < T271_NETWORK_OBJECT_MAX_STRING_DATA_ITEMS; i++ )
         {
            if( ( StringDataStoreDirty_ & ( 1U << i ) ) != 0U )
            {
               StringDataStores_[i] = argMessage.popstr();
            }
         }

         //StringDataStoreDirty_ = 0;

         for( uint8_t j = 0; j < NumDataStores_; j++ )
         {
            DataStoreDirty_[j] = argMessage.poptyp<uint16_t>();

            for( uint32_t i = 0; i < T271_NETWORK_OBJECT_MAX_DATA_ITEMS; i++ )
            {
               if( ( DataStoreDirty_[j] & ( 1U << i ) ) != 0U )
               {
                  DataStores_[j][i].udata32 = argMessage.poptyp<uint32_t>();
               }
            }

            //DataStoreDirty_[j] = 0;
         }
      }
   }
}
