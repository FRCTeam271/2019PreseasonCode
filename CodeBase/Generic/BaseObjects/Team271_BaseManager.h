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

#ifndef TEAM271_BASE_MANAGER_H_
#define TEAM271_BASE_MANAGER_H_

#define T271_BASE_MANAGER_START_INDEX   (T271_NETWORK_OBJECT_NETWORK_ID_END)
#define T271_BASE_MANAGER_MAX_OBJECTS   (255U - T271_BASE_MANAGER_START_INDEX)

/*
 *
 * Shared Network Data
 *
 */
class t271_base_manager_data_index : public t271_network_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_BASE_MANAGER_DATA_OBJECT_COUNT = t271_network_data_index::T271_DATA_COUNT,

      T271_DATA_COUNT,
   } t271_base_manager_data_index_t;
};

class Team271BaseManager : public Team271NetworkObject
{
protected:
   /*
    *
    * Variables
    *
    */
   uint8_t ObjCount_ = 0;
   Team271Base* Objs_[T271_BASE_MANAGER_MAX_OBJECTS];

public:
   static Team271BaseManager& Instance( void );

   template<typename T>
   static T InstanceAdd( T const argObj )
   {
      return (T)Instance().Add( argObj );
   }

   static void InstanceCreateMessages( const float argTimeStamp );

   static void InstanceDecodeMessages( multipart_t& argMessage );

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271BaseManager( void );

   virtual ~Team271BaseManager( void );

   Team271BaseManager( Team271BaseManager const& ) = delete;
   void operator=( Team271BaseManager const& ) = delete;

   /*
    *
    * Getters
    *
    */
   Team271Base* GetByName( const string& argName ) const;

   Team271Base* GetByPath( const string& argName ) const;

   Team271Base* GetByPath( const string& argName,
                           const string& argClassName ) const;

   /*
    *
    * Child Functions
    *
    */
   Team271Base* Add( Team271Base* const argObj );

   /*
    *
    * Network
    *
    */
   void CreateMessages( const float argTimeStamp );

   void DecodeMessages( multipart_t& argMessage );
};

#endif /* TEAM271_BASE_MANAGER_H_ */
