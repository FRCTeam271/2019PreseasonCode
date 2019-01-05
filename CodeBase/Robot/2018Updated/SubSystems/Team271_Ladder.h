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

#ifndef TEAM271_LADDER_H_
#define TEAM271_LADDER_H_

#if defined(T271_LADDER_ENABLE)

#define LADDER_DEADZONE_LOW            ( 0.05f )
#define LADDER_DEADZONE_HIGH           ( 0.05f )

#define LADDER_INPUT_MAX_SCALE         ( 1.005f )

#define LADDER_SCALE_LOW               ( 1.0f )
#define LADDER_SCALE_HIGH              ( 1.0f )

#define LADDER_DEFAULT_GEAR            ( LADDER_LOW )
#define LADDER_DEFAULT_TELEOP_GEAR     ( LADDER_LOW )
#define LADDER_DEFAULT_AUTO_GEAR       ( LADDER_LOW  )

//#define LADDER_CHILD_INDEX_TRANS         (0U)

/*
 *
 * Shared Network Data
 *
 */
class t271_ladder_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Mode
       */
      T271_LADDER_DATA_MODE = t271_base_data_index::T271_DATA_COUNT,

      T271_LADDER_DATA_CLIMB_MODE,

      T271_LADDER_DATA_VALUE,

      T271_DATA_COUNT,
   } t271_ladder_data_index_t;
};

class Team271Ladder : public Team271Base
{
protected:
   /*
    * Transmissions
    */
   Team271Transmission* _TransLadder = nullptr;

   /*
    * Drive Shifter
    */
   Team271Shifter* _ShifterSpeed = nullptr;
   Team271Shifter* _ShifterBreak = nullptr;

   /*
    * Sensors
    */
   Team271SensorSRX* _Encoder = nullptr;

   /*
    *
    * Variables
    *
    */
   uint8_t ModePrev_ = 0;

   atomic<float> _InputLadderValue = { 0 };
   atomic<float> _InputLadderValuePrev = { 0 };

public:
   Team271Ladder( void );

   virtual ~Team271Ladder( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   float GetSetpoint( void ) const;

   bool GetClimbMode( void ) const;

   float GetOperatorLadderAxisY( void ) const;

   /*
    *
    * Setters
    *
    */
   void SetSetpoint( const float argSetpoint );

   void SetClimbMode( const bool argClimbMode );

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
   void TeleopPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Test
    *
    */
   void TestInit( const bool argIsParent = false ) override;

   /*
    *
    * Robot State
    *
    */
   void UpdateInput( const bool argIsParent = false ) override;

   /*
    *
    * Ladder
    *
    */
   void ShiftGears( const eLadderGear argGear );
   void ShiftGears( const eLadderBrake argGear );
};

#endif

#endif /* TEAM271_LADDER_H_ */
