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

#ifndef TEAM271_HERDER_H_
#define TEAM271_HERDER_H_

#if defined(T271_HERDER_ENABLE)

#define HERDER_DEADZONE_ELEVATION   ( 0.08f )

#define HERDER_INPUT_MAX_SCALE      ( 1.005f )

#define HERDER_SCALE                  ( 0.3f )

#define   HERDER_MOTOR_SPEED_IN      ( -1.0f )
#define   HERDER_MOTOR_SPEED_OUT      ( 1.0f )
#define HERDER_MOTOR_SPEED_OFF      ( 0.0f )

//#define HERDER_CHILD_INDEX_TRANS_DRIVER      (0U)
//#define HERDER_CHILD_INDEX_TRANS_PASSENGER   (1U)

typedef enum : uint8_t
{
   eHerderModeManual = 0,
   eHerderModePID,
   eHerderModeMax
} eHerderMode;

enum eHerderState : uint8_t
{
   eHerderStateNone = 0,
   eHerderStateIn,
   eHerderStateOut,
   eHerderStateMax
};

enum eHerderAction : uint8_t
{
   eHerderActionNone = 0,

   eHerderActionIn,
   eHerderActionOut,
   eHerderActionToggle,
   eHerderActionWheelsIn,
   eHerderActionWheelsOut,

   eHerderActionMax
};

enum eHerderActuationState : uint8_t
{
   eHerderActuationStateNone = 0,
   eHerderActuationStateIn,
   eHerderActuationStateOut,
   eHerderActuationStateToggle,
   eHerderActuationStateMax
};

#define DEFAULT_HERDER_POSITION        ( eHerderActuationStateIn )
#define DEFAULT_HERDER_POSITION_AUTO   ( eHerderActuationStateNone )
#define DEFAULT_HERDER_POSITION_TELEOP ( eHerderActuationStateNone )

typedef enum : uint8_t
{
   T271_HERDER_GRAB_OPEN = 0,
   T271_HERDER_GRAB_CLOSE
} t271_herder_grab_state_t;

typedef enum : uint8_t
{
   T271_HERDER_WHEELS_OFF,
   T271_HERDER_WHEELS_IN,
   T271_HERDER_WHEELS_OUT,
   T271_HERDER_WHEELS_OUT_AUTO
} t271_herder_wheels_state_t;

/*
 *
 * Shared Network Data
 *
 */
class t271_herder_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Mode
       */
      T271_HERDER_DATA_WHEELS_STATE = t271_base_data_index::T271_DATA_COUNT,

      T271_HERDER_DATA_GRAB_STATE,

      T271_DATA_COUNT,
   } t271_herder_data_index_t;
};

class Team271Herder : public Team271Base
{
protected:
   Team271Transmission*_TransHerderDriver = nullptr;
   Team271Transmission* _TransHerderPassenger = nullptr;

   t271_herder_grab_state_t HerderGrabStatePrev_ = T271_HERDER_GRAB_OPEN;
   t271_herder_wheels_state_t HerderWheelsStatePrev_ = T271_HERDER_WHEELS_OFF;

   Team271Solenoid* _HerderClawOpen = nullptr;
   Team271Solenoid* _HerderClawClose = nullptr;

public:
   Team271Herder( void );

   virtual ~Team271Herder( void );

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
   void SetHerderGrabState( const t271_herder_grab_state_t argState );

   void SetHerderWheelState( const t271_herder_wheels_state_t argState );

   /*
    *
    * Base
    *
    */
   void Init( void ) override;

   /*
    *
    * Robot
    *
    */
   void RobotInit( const bool argIsParent = false ) override;

   void PreRobotPeriodic( const bool argIsParent = false ) override;

   void PostRobotPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Disabled
    *
    */
   void DisabledInit( const bool argIsParent = false ) override;

   /*
    *
    * Autonomous
    *
    */
   void AutonomousInit( const bool argIsParent = false ) override;

   /*
    *
    * Teleop
    *
    */
   void TeleopInit( const bool argIsParent = false ) override;

   /*
    *
    * Robot State
    *
    */
   void UpdateInput( const bool argIsParent = false ) override;
};

#endif

#endif /* TEAM271_HERDER_H_ */
