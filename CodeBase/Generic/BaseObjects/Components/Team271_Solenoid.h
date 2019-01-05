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

#ifndef TEAM271_SOLENOID_H_
#define TEAM271_SOLENOID_H_

#if defined( T271_SOLENOID_ENABLE )

typedef enum : uint8_t
{
   TEAM271_SOLENOID_TYPE_SINGLE,
   TEAM271_SOLENOID_TYPE_DOUBLE,

   TEAM271_SOLENOID_TYPE_COUNT
}TEAM271_SOLENOID_TYPE;

typedef enum : uint8_t
{
   /*
    * Single
    */
   TEAM271_SOLENOID_STATE_OFF,
   TEAM271_SOLENOID_STATE_ON,

   /*
    * Double
    */
   TEAM271_SOLENOID_STATE_REV,
   TEAM271_SOLENOID_STATE_FWD,

   TEAM271_SOLENOID_STATE_COUNT
}TEAM271_SOLENOID_STATE;

class t271_solenoid_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Controller Type
       */
      T271_SOLENOID_DATA_TYPE = t271_base_data_index::T271_DATA_COUNT,

      /*
       * IDs
       */
      T271_SOLENOID_DATA_PCM_MODULE_A,
      T271_SOLENOID_DATA_PCM_CHANNEL_A,

      T271_SOLENOID_DATA_PCM_MODULE_B,
      T271_SOLENOID_DATA_PCM_CHANNEL_B,

      /*
       * Output Values
       */
      T271_SOLENOID_DATA_VALUE_CURRENT,
      T271_SOLENOID_DATA_VALUE_PREV,

      T271_DATA_COUNT,
   } t271_solenoid_data_index_t;
};

class Team271Solenoid : public Team271Base
{
public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271Solenoid( Team271Base* const argParent,
                    const string& argName,
                    const uint32_t argPCMModuleA,
                    const uint32_t argPCMChannelA );

   Team271Solenoid( Team271Base* const argParent,
                    const string& argName,
                    const uint32_t argPCMModuleA,
                    const uint32_t argPCMChannelA,
                    const uint32_t argPCMModuleB,
                    const uint32_t argPCMChannelB );

   virtual ~Team271Solenoid( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   TEAM271_SOLENOID_STATE GetValuePrev( void ) const;

   TEAM271_SOLENOID_STATE GetValueCurrent( void ) const;

   /*
    *
    * Solenoid Functions
    *
    */
   void SetOutput( const TEAM271_SOLENOID_STATE argSet );

   /*
    *
    * Robot
    *
    */
   void PreRobotPeriodic( const bool argIsParent = false ) override;

   void PostRobotPeriodic( const bool argIsParent = false ) override;

private:
   /*
    *
    * Setters
    *
    */
   void SetSingle( const uint32_t argPCMModule,
                   const uint32_t argChannel );

   void SetDouble( const uint32_t argPCMModuleA,
                   const uint32_t argChannelA,
                   const uint32_t argPCMModuleB,
                   const uint32_t argChannelB );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
