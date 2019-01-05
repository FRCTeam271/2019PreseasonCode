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

#if defined( T271_INPUT_ENABLE )

static Team271Input InputInstance_;

std::atomic<uint8_t> InputUpdate_ = { 0U };

#if defined( T271_FRC_NETCOM_ENABLE )
constexpr int32_t refNumber = 42;

static void newDataOccur( uint32_t refNum )
{
   if( refNum != refNumber )
   {
      return;
   }

   InputUpdate_.store( 1 );
}
#endif

Team271Input& Team271Input::Instance( void )
{
   return InputInstance_;
}

TEAM271_DSO_STATE Team271Input::InstanceGetDSOState( void )
{
   return InputInstance_.GetDSOState();
}

double Team271Input::InstanceGetAxis( const eInputIndex newInputIndex,
                                      const uint32_t newAxis )
{
   return InputInstance_.GetAxis( newInputIndex,
                                  newAxis );
}

double Team271Input::InstanceGetAxisPrev( const eInputIndex newInputIndex,
                                          const uint32_t newAxis )
{
   return InputInstance_.GetAxisPrev( newInputIndex,
                                      newAxis );
}

int32_t Team271Input::InstanceGetPOVState( const eInputIndex newInputIndex,
                                           const uint32_t newPOV )
{
   return InputInstance_.GetPOVState( newInputIndex,
                                      newPOV );
}

int32_t Team271Input::InstanceGetPOVStatePrev( const eInputIndex newInputIndex,
                                               const uint32_t newPOV )
{
   return InputInstance_.GetPOVStatePrev( newInputIndex,
                                          newPOV );
}

bool Team271Input::InstanceGetButtonState( const eInputIndex newInputIndex,
                                           const uint32_t newButton )
{
   return InputInstance_.GetButtonState( newInputIndex,
                                         newButton );
}

bool Team271Input::InstanceGetButtonStatePrev( const eInputIndex newInputIndex,
                                               const uint32_t newButton )
{
   return InputInstance_.GetButtonStatePrev( newInputIndex,
                                             newButton );
}

Team271Input::Team271Input( void )
      : Team271NetworkObject( "Input",
                              t271_input_data_index::T271_DATA_COUNT,
                              T271_NETWORK_OBJECT_NETWORK_ID_INPUT )
{
   LOG_TRACE( "%s - Constructor Start",
         "Team271Input" );

   /*
    *
    * Clear Arrays
    * - Axes
    * - POVs
    * - Buttons
    * - Descriptors
    */
   memset( _Axes,
           0,
           sizeof( _Axes ) );
   memset( _AxesPrev,
           0,
           sizeof( _AxesPrev ) );

   memset( _Povs,
           0,
           sizeof( _Povs ) );
   memset( _PovsPrev,
           0,
           sizeof( _PovsPrev ) );

   memset( _Buttons,
           0,
           sizeof( _Buttons ) );
   memset( _ButtonsPrev,
           0,
           sizeof( _ButtonsPrev ) );

   for( uint8_t i = 0; i < eInputIndexMax; i++ )
   {
      _Desc[i].isXbox = 0;
      _Desc[i].type = -1;
      _Desc[i].name[0] = '\0';
   }

#if defined( T271_FRC_NETCOM_ENABLE )
   // Set up the occur function internally with NetComm
   NetCommRPCProxy_SetOccurFuncPointer( newDataOccur );

   // Set up our occur reference number
   setNewDataOccurRef( refNumber );
#endif
}

Team271Input::~Team271Input( void )
{
#if defined( T271_FRC_NETCOM_ENABLE )
   /*
    * Unregister our new data condition variable.
    */
   setNewDataSem( nullptr );
#endif
}

/*
 *
 * Getters
 *
 */
uint32_t Team271Input::GetHasNewData( void ) const
{
   return InputUpdate_.load();
}

bool Team271Input::IsUserButtonAction( void ) const
{
   return Read<bool>( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON_ACTION );
}

TEAM271_DSO_STATE Team271Input::GetDSOState( void ) const
{
#if defined( INPUT_ENABLE_DSO )
   return Read<TEAM271_DSO_STATE>( t271_input_data_index::T271_INPUT_DATA_DSO_STATE );
#else
   return TEAM271_DSO_STATE_INACTIVE;
#endif
}

double Team271Input::GetAxis( const eInputIndex newInputIndex,
                              const uint32_t newAxis ) const
{
   if( newInputIndex < eInputIndexMax && newAxis < kMaxJoystickAxes )
   {
      return _Axes[newInputIndex].axes[newAxis];
   }

   return 0;
}

double Team271Input::GetAxisPrev( const eInputIndex newInputIndex,
                                  const uint32_t newAxis ) const
{
   if( newInputIndex < eInputIndexMax && newAxis < kMaxJoystickAxes )
   {
      return _AxesPrev[newInputIndex].axes[newAxis];
   }

   return 0;
}

int32_t Team271Input::GetPOVState( const eInputIndex newInputIndex,
                                   const uint32_t newPOV ) const
{
   if( newInputIndex < eInputIndexMax && newPOV < kMaxJoystickPOVs )
   {
      return _Povs[newInputIndex].povs[newPOV];
   }

   return 0;
}

int32_t Team271Input::GetPOVStatePrev( const eInputIndex newInputIndex,
                                       const uint32_t newPOV ) const
{
   if( newInputIndex < eInputIndexMax && newPOV < kMaxJoystickPOVs )
   {
      return _PovsPrev[newInputIndex].povs[newPOV];
   }

   return 0;
}

bool Team271Input::GetButtonState( const eInputIndex newInputIndex,
                                   const uint32_t newButton ) const
{
   if( newInputIndex < eInputIndexMax && newButton < kMaxJoystickButtons )
   {
      return _Buttons[newInputIndex].buttons & 1 << newButton;
   }

   return 0;
}

bool Team271Input::GetButtonStatePrev( const eInputIndex newInputIndex,
                                       const uint32_t newButton ) const
{
   if( newInputIndex < eInputIndexMax && newButton < kMaxJoystickButtons )
   {
      return _ButtonsPrev[newInputIndex].buttons & 1 << newButton;
   }

   return 0;
}

/*
 *
 * Updates
 *
 */
void Team271Input::UpdateInput( void )
{
   /*
    * Update User Button
    * - Clear UserButtonAction
    * - Store Previous Button State
    * - Store Current Button State
    */
   Write( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON_ACTION,
          false );

   _InputUserButtonStatePrev = Read<bool>( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON );

   Write( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON,
          Team271Robot::GetUserButton() );

   /*
    * Check if we should perform User Button Action
    */
   if( Read<bool>( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON ) != _InputUserButtonStatePrev )
   {
      if( Read<bool>( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON ) == false )
      {
         Write( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON_ACTION,
                true );

         LOG_NOTICE( "%s - CheckUserButton - True",
                     "Team271Input" );
      }
      else
      {
         Write( t271_input_data_index::T271_INPUT_DATA_USER_BUTTON_ACTION,
                false );

         LOG_NOTICE( "%s - CheckUserButton - False",
                     "Team271Input" );
      }
   }

   /*
    *
    * Update Double Secret Override
    * - Store Previous State
    * - Update State
    *
    */
   _DSOStatePrev = Read<TEAM271_DSO_STATE>( t271_input_data_index::T271_INPUT_DATA_DSO_STATE );

   if( Read<TEAM271_DSO_STATE>( t271_input_data_index::T271_INPUT_DATA_DSO_STATE ) == TEAM271_DSO_STATE_ENTER )
   {
      LOG_NOTICE( "%s - Entering DSO",
                  "Team271Input" );

      Write( t271_input_data_index::T271_INPUT_DATA_DSO_STATE,
             TEAM271_DSO_STATE_ACTIVE );
   }
   else if( Read<TEAM271_DSO_STATE>( t271_input_data_index::T271_INPUT_DATA_DSO_STATE ) == TEAM271_DSO_STATE_EXIT )
   {
      LOG_NOTICE( "%s - Exiting DSO",
                  "Team271Input" );

      Write( t271_input_data_index::T271_INPUT_DATA_DSO_STATE,
             TEAM271_DSO_STATE_INACTIVE );
   }

   /*
    * Save All Previous States
    */
   memcpy( &_AxesPrev,
           &_Axes,
           sizeof( _AxesPrev ) );

   memcpy( &_PovsPrev,
           &_Povs,
           sizeof( _PovsPrev ) );

   memcpy( &_ButtonsPrev,
           &_Buttons,
           sizeof( _ButtonsPrev ) );

   /*
    * Get All Current Input States
    * - Descriptors
    * - Axis
    * - POV
    * - Button
    */
   static uint8_t myDescUpdate = 0;
   ++myDescUpdate;

   for( uint8_t i = 0; i < eInputIndexMax; i++ )
   {
      if( myDescUpdate > 100 )
      {
         _Desc[i].isXbox = 0;
         _Desc[i].type = std::numeric_limits<uint8_t>::max();
         _Desc[i].name[0] = '\0';
         _Desc[i].axisCount = kMaxJoystickAxes; /* set to the Desc->axisTypes's capacity */
         _Desc[i].buttonCount = 0;
         _Desc[i].povCount = 0;

#if defined( T271_FRC_NETCOM_ENABLE )
         FRC_NetworkCommunication_getJoystickDesc( i,
                                                   &_Desc[i].isXbox,
                                                   &_Desc[i].type,
                                                   reinterpret_cast<char*>( &_Desc[i].name ),
                                                   &_Desc[i].axisCount,
                                                   reinterpret_cast<uint8_t*>( &_Desc[i].axisTypes ),
                                                   &_Desc[i].buttonCount,
                                                   &_Desc[i].povCount );
#endif
      }

      T271_JoystickAxesInt tmpAxesInt;

#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_getJoystickAxes( i,
                                                reinterpret_cast<JoystickAxes_t*>( &tmpAxesInt ),
                                                kMaxJoystickAxes );
#endif

      _Axes[i].count = tmpAxesInt.count;

      /* current scaling is -128 to 127, can easily be patched in the future by
       * changing this function.
       */
      for( int32_t j = 0; j < tmpAxesInt.count; j++ )
      {
         int8_t value = tmpAxesInt.axes[j];
         if( value < 0 )
         {
            _Axes[i].axes[j] = value / 128.0;
         }
         else
         {
            _Axes[i].axes[j] = value / 127.0;
         }
      }

#if defined( T271_FRC_NETCOM_ENABLE )
      FRC_NetworkCommunication_getJoystickPOVs( i,
                                                reinterpret_cast<JoystickPOV_t*>( &_Povs[i] ),
                                                kMaxJoystickPOVs );

      FRC_NetworkCommunication_getJoystickButtons( i,
                                                   &_Buttons[i].buttons,
                                                   &_Buttons[i].count );
#endif
   }

   if( myDescUpdate > 100 )
   {
      myDescUpdate = 0;
   }

   InputUpdate_.store( 0 );
}

#endif
