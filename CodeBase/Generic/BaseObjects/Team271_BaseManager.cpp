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

static Team271BaseManager BaseManagerInstance_;

Team271BaseManager& Team271BaseManager::Instance( void )
{
   return BaseManagerInstance_;
}

void Team271BaseManager::InstanceCreateMessages( const float argTimeStamp )
{
   BaseManagerInstance_.CreateMessages( argTimeStamp );
}

void Team271BaseManager::InstanceDecodeMessages( multipart_t& argMessage )
{
   BaseManagerInstance_.DecodeMessages( argMessage );
}

Team271BaseManager::Team271BaseManager( void )
      : Team271NetworkObject( "BaseManager",
                              t271_base_manager_data_index::T271_DATA_COUNT,
                              T271_NETWORK_OBJECT_NETWORK_ID_BASE_MANAGER ),
        ObjCount_( 0 )

{
   for( uint8_t i = 0; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
   {
      Objs_[i] = nullptr;
   }
}

Team271BaseManager::~Team271BaseManager( void )
{
   /*
    * Delete Children
    */
   for( uint8_t i = ( T271_BASE_MANAGER_MAX_OBJECTS - 1 ); i > 0; i-- )
   {
      if( Objs_[i] != nullptr )
      {
         delete Objs_[i];
         Objs_[i] = nullptr;
      }
   }
}

/*
 *
 * Getters
 *
 */
Team271Base* Team271BaseManager::GetByName( const string& argName ) const
{
   Team271Base* tmpReturn = nullptr;

   for( uint8_t i = T271_BASE_MANAGER_START_INDEX; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
   {
      if( Objs_[i] != nullptr )
      {
         if( Objs_[i]->GetName() == argName )
         {
            printf( "GetByName:: %u :: %s :: %s\r\n",
                    i,
                    argName.c_str(),
                    Objs_[i]->GetPath().c_str() );

            tmpReturn = Objs_[i];

            break;
         }
      }
   }

   return tmpReturn;
}

Team271Base* Team271BaseManager::GetByPath( const string& argName ) const
{
   Team271Base* tmpReturn = nullptr;

   for( uint8_t i = T271_BASE_MANAGER_START_INDEX; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
   {
      if( Objs_[i] != nullptr )
      {
         if( Objs_[i]->GetPath() == argName )
         {
            printf( "GetByFullName:: %u :: %s :: %s\r\n",
                    i,
                    argName.c_str(),
                    Objs_[i]->GetPath().c_str() );

            tmpReturn = Objs_[i];

            break;
         }
      }
   }

   return tmpReturn;
}

Team271Base* Team271BaseManager::GetByPath( const string& argName,
                                            const string& argClassName ) const
{
   Team271Base* tmpReturn = nullptr;

   for( uint8_t i = T271_BASE_MANAGER_START_INDEX; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
   {
      if( Objs_[i] != nullptr )
      {
         if( Objs_[i]->GetPath( argClassName ) == argName )
         {
            printf( "GetByFullName:: %u :: %s :: %s\r\n",
                    i,
                    argName.c_str(),
                    Objs_[i]->GetPath().c_str() );

            tmpReturn = Objs_[i];

            break;
         }
      }
   }

   return tmpReturn;
}

/*
 *
 * Child Functions
 *
 */
Team271Base* Team271BaseManager::Add( Team271Base* const argObj )
{
   Team271Base* tmpObj = nullptr;

   if( argObj != nullptr )
   {
      /*
       * Initialize the Object
       */
      argObj->Init();

      /*
       * Try to add the Object
       */
      if( GetByPath( argObj->GetPath( "Team271Base" ),
                     "Team271Base" ) == nullptr )
      {
         for( uint8_t i = T271_BASE_MANAGER_START_INDEX; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
         {
            if( Objs_[i] == nullptr )
            {
               Objs_[i] = argObj;

               Objs_[i]->SetNetworkID( i );

               tmpObj = Objs_[i];
/*
               printf( "Add:: %u :: %s\r\n",
                       i,
                       Objs_[i]->GetPath().c_str() );
*/

               break;
            }
         }
      }
      else
      {
         printf( "Add:: Error Non Unique Path :: %s\r\n",
                 argObj->GetPath().c_str() );
      }
   }

   return tmpObj;
}

void Team271BaseManager::CreateMessages( const float argTimeStamp )
{
   for( uint8_t i = T271_BASE_MANAGER_START_INDEX; i < T271_BASE_MANAGER_MAX_OBJECTS; i++ )
   {
      if( Objs_[i] != nullptr )
      {
         multipart_t tmpMyData;

         Objs_[i]->CreateMessage( argTimeStamp,
                                  tmpMyData );

#if defined( T271_COMP_ROBOT ) || defined( T271_PRACTICE_ROBOT )
         tmpMyData.send( RobotPub_.GetSocket() );
#endif
      }
   }
}

void Team271BaseManager::DecodeMessages( multipart_t& argMessage )
{
   if( argMessage.size() > 0 )
   {
      uint16_t tmpNetworkID = argMessage.peektyp<uint8_t>( 0 );

      if( tmpNetworkID < T271_BASE_MANAGER_MAX_OBJECTS )
      {
         if( Objs_[tmpNetworkID] != nullptr )
         {
            Objs_[tmpNetworkID]->DecodeMessage( argMessage );
         }
      }
   }
}
