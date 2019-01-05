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

#ifndef TEAM271_PWM_H_
#define TEAM271_PWM_H_

#if defined( T271_PWM_ENABLE )

class t271_pwm_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      T271_PWM_DATA_CHANNEL = t271_base_data_index::T271_DATA_COUNT,
      T271_PWM_DATA_HANDLE,
      T271_PWM_DATA_INVERTED,

      T271_DATA_COUNT,
   } t271_pwm_data_index_t;
};

class Team271PWM : public Team271Base
{
public:
   /*
    *
    * Constructor/Destructor
    *
    */
   Team271PWM( Team271Base* const argParent,
               const uint32_t argChannel );

   virtual ~Team271PWM( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   /*
    *
    * Setters
    *
    */
   void SetInverted( const TEAM271_INVERTED_STATE argInverted );

   /*
    *
    * Output Functions
    *
    */
   void Set( const double argSet );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
