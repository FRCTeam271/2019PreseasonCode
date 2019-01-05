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

#ifndef TEAM271_NETWORKOBJECT_H_
#define TEAM271_NETWORKOBJECT_H_

#define T271_NETWORK_OBJECT_NETWORK_ID_BASE_MANAGER   (0U)
#define T271_NETWORK_OBJECT_NETWORK_ID_GLOBALS        (1U)
#define T271_NETWORK_OBJECT_NETWORK_ID_INPUT          (2U)
#define T271_NETWORK_OBJECT_NETWORK_ID_END            (3U)

#define T271_NETWORK_OBJECT_MAX_STRING_DATA_ITEMS   (8U)

#define T271_NETWORK_OBJECT_MAX_DATA_STORES  (4U)
#define T271_NETWORK_OBJECT_MAX_DATA_ITEMS   (16U)

class t271_network_string_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_NETWORK_STRING_DATA_NAME,
      T271_STRING_DATA_COUNT,

      T271_NETWORK_STRING_DATA_END = T271_NETWORK_OBJECT_MAX_STRING_DATA_ITEMS,
   } t271_network_data_index_t;

   const uint32_t Index_ = T271_NETWORK_STRING_DATA_NAME;

   t271_network_string_data_index( const uint32_t argIndex );
};

class t271_network_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_DATA_COUNT,

      T271_NETWORK_DATA_END = (T271_NETWORK_OBJECT_MAX_DATA_STORES * T271_NETWORK_OBJECT_MAX_DATA_ITEMS),
   } t271_network_data_index_t;

   const uint8_t StoreIndex_ = 0U;
   const uint32_t Index_ = T271_DATA_COUNT;

   t271_network_data_index( const uint32_t argIndex );
};

typedef union
{
   uint8_t udata8;
   uint16_t udata16;
   uint32_t udata32;

   int8_t data8;
   int16_t data16;
   int32_t data32;

   float dataf;
   bool datab;
} network_data_union_t;

class Team271NetworkObject
{
protected:
   uint8_t NumNetworkMessages_ = 0U;
   uint8_t NumDataStores_ = 1U;

   uint8_t NetworkID_ = 0U;

   float TimeStampLast_ = 0;

   uint8_t StringDataStoreDirty_ = 0;
   string StringDataStores_[T271_NETWORK_OBJECT_MAX_DATA_ITEMS];

   uint16_t DataStoreDirty_[T271_NETWORK_OBJECT_MAX_DATA_STORES];
   network_data_union_t DataStores_[T271_NETWORK_OBJECT_MAX_DATA_STORES][T271_NETWORK_OBJECT_MAX_DATA_ITEMS];

public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271NetworkObject( const string& argName,
                         const uint8_t argMessageCount = t271_network_data_index::T271_DATA_COUNT,
                         const uint8_t argNetworkID = 0 );

   virtual ~Team271NetworkObject( void );

   /*
    *
    * Getters
    *
    */
   uint32_t GetNetworkID( void ) const;

   virtual const string GetClassName( void ) const;

   const string& GetName( void ) const;

   virtual uint32_t GetDirty( void ) const;

   virtual uint32_t GetDirtyString( const uint32_t argIndex ) const;

   virtual uint32_t GetDirty( const uint32_t argIndex ) const;

   /*
    *
    * Setters
    *
    */
   void SetNetworkID( const uint32_t argID );

   void SetName( const string& argName );

   virtual void SetDirty( void );

   virtual void ClearDirty( void );

   void Set( const uint32_t argIndexA,
             const uint32_t argIndexB );

   void Set( const uint32_t argIndex,
             const Team271NetworkObject& argNetworkObj );

   void Set( const uint32_t argIndex,
             const Team271NetworkObject* const argNetworkObj );

   /*
    *
    * Readers
    *
    */
   const string& ReadString( const uint32_t argIndex ) const;

   template<typename T>
   const T Read( const uint32_t argIndex ) const
   {
      const t271_network_data_index tmpIndex( argIndex );

      return reinterpret_cast<const T&>( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_] );
   }

   /*
    *
    * Writers
    *
    */
   const string& Write( const uint32_t argIndex,
                        const string& argData );

   float Write( const uint32_t argIndex,
                const float argData );

   template<typename T>
   const T& Write( const uint32_t argIndex,
                   const T argData )
   {
      const t271_network_data_index tmpIndex( argIndex );

      if( reinterpret_cast<const T&>( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_] ) != argData )
      {
         reinterpret_cast<T&>( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_] ) = argData;

         DataStoreDirty_[tmpIndex.StoreIndex_] |= (1U << tmpIndex.Index_);
      }

      return reinterpret_cast<const T&>( DataStores_[tmpIndex.StoreIndex_][tmpIndex.Index_] );
   }

   /*
    *
    * Incrementers
    *
    */
   uint8_t IncUInt8( const uint32_t argIndex );

   uint16_t IncUInt16( const uint32_t argIndex );

   uint32_t IncUInt32( const uint32_t argIndex );

   int8_t IncInt8( const uint32_t argIndex );

   int16_t IncInt16( const uint32_t argIndex );

   int32_t IncInt32( const uint32_t argIndex );

   float IncFloat( const uint32_t argIndex );

   /*
    *
    * Decrementers
    *
    */
   uint8_t DecUInt8( const uint32_t argIndex );

   uint16_t DecUInt16( const uint32_t argIndex );

   uint32_t DecUInt32( const uint32_t argIndex );

   int8_t DecInt8( const uint32_t argIndex );

   int16_t DecInt16( const uint32_t argIndex );

   int32_t DecInt32( const uint32_t argIndex );

   float DecFloat( const uint32_t argIndex );

   /*
    *
    * Network
    *
    */
   void CreateMessage( const float argTimeStamp,
                       multipart_t& argMessage );

   void DecodeMessage( multipart_t& argMessage );
};

#endif /* TEAM271_NETWORKOBJECT_H_ */
