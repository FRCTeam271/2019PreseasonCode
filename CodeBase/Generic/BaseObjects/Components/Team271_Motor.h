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

#ifndef TEAM271_MOTOR_H_
#define TEAM271_MOTOR_H_

#if defined( T271_MOTOR_ENABLE )

typedef enum : uint8_t
{
   TEAM271_MOTOR_CONTROLLER_TYPE_NONE,

   TEAM271_MOTOR_CONTROLLER_TYPE_TALON_SRX,
   TEAM271_MOTOR_CONTROLLER_TYPE_VICTOR_SPX,
   TEAM271_MOTOR_CONTROLLER_TYPE_SPARK,
   TEAM271_MOTOR_CONTROLLER_TYPE_SPARK_MAX,

   TEAM271_MOTOR_CONTROLLER_TYPE_COUNT
}TEAM271_MOTOR_CONTROLLER_TYPE;

typedef enum : uint8_t
{
   TEAM271_NEUTRAL_MODE_COAST,
   TEAM271_NEUTRAL_MODE_BRAKE,

   TEAM271_NEUTRAL_MODE_COUNT
}TEAM271_NEUTRAL_MODE;

typedef enum : uint8_t
{
   TEAM271_CONTROL_MODE_DISABLED,

   TEAM271_CONTROL_MODE_PERCENT_OUTPUT,
   TEAM271_CONTROL_MODE_POSITION,
   TEAM271_CONTROL_MODE_VELOCITY,
   TEAM271_CONTROL_MODE_CURRENT,
   TEAM271_CONTROL_MODE_FOLLOWER,
   TEAM271_CONTROL_MODE_MOTION_MAGIC,

   TEAM271_CONTROL_MODE_COUNT
}TEAM271_CONTROL_MODE;

/*
 *
 * Shared Network Data
 *
 */
class t271_motor_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Controller Type
       */
      T271_MOTOR_DATA_CONTROLLER_TYPE = t271_base_data_index::T271_DATA_COUNT,

      /*
       * IDs
       */
      T271_MOTOR_DATA_ID,
      T271_MOTOR_DATA_ID_MASTER,

      /*
       * Output Values
       */
      T271_MOTOR_DATA_VALUE_PREV,
      T271_MOTOR_DATA_VALUE_CURRENT,
      T271_MOTOR_DATA_VALUE_NEXT,
      T271_MOTOR_DATA_VALUE_NEUTRAL,

      T271_MOTOR_DATA_ARB_FF_PREV,
      T271_MOTOR_DATA_ARB_FF_CURRENT,
      T271_MOTOR_DATA_ARB_FF_NEXT,

      /*
       * Control Mode
       */
      T271_MOTOR_DATA_CONTROL_MODE,

      /*
       * Motion Magic
       */
      T271_MOTOR_DATA_MM_VEL,
      T271_MOTOR_DATA_MM_ACCEL,

      /*
       * Status Frame Rates
       */
      T271_MOTOR_DATA_STATUS_FRAME_RATE_GENERAL,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_FEEDBACK,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_QUAD_ENCODER,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_ANALOG_TEMP_VBAT,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_PULSE_WIDTH_MEAS,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_BASE_PIDF,
      T271_MOTOR_DATA_STATUS_FRAME_RATE_MOTION_MAGIC,

      /*
       * Status Frame Rates As Slave
       */
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_GENERAL,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_FEEDBACK,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_QUAD_ENCODER,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_ANALOG_TEMP_VBAT,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_PULSE_WIDTH_MEAS,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_BASE_PIDF,
      T271_MOTOR_DATA_SLAVE_STATUS_FRAME_RATE_MOTION_MAGIC,

      /*
       * Output Settings
       */
      T271_MOTOR_DATA_INVERTED_OUTPUT,
      T271_MOTOR_DATA_NEUTRAL_MODE,

      /*
       * Current Limiting
       */
      T271_MOTOR_DATA_CURRENT_LIMIT_ENABLE,
      T271_MOTOR_DATA_PEAK_CURRENT_LIMIT,
      T271_MOTOR_DATA_PEAK_CURRENT_DURATION,
      T271_MOTOR_DATA_CONTINUOUS_CURRENT_LIMIT,

      /*
       * Output Shaping
       */
      /* Seconds */
      T271_MOTOR_DATA_OPEN_LOOP_RAMP,
      T271_MOTOR_DATA_CLOSED_LOOP_RAMP,

      /* Percent */
      T271_MOTOR_DATA_PEAK_VOLT_FWD,
      T271_MOTOR_DATA_PEAK_VOLT_REV,

      /* Percent */
      T271_MOTOR_DATA_NOMINAL_VOLT_FWD,
      T271_MOTOR_DATA_NOMINAL_VOLT_REV,

      /* Percent */
      T271_MOTOR_DATA_NEUTRAL_DEADBAND,

      T271_MOTOR_DATA_ENABLE_VOLT_COMP,

      T271_DATA_COUNT,
   }t271_motor_data_index_t;
};


class Team271Motor : public Team271Base
{
protected:
   /*
    *
    * Variables
    *
    */
   const Team271Motor* MasterMotor_ = nullptr;

   /*
    * Motor Controllers
    */
#if defined(T271_CTRE)
   BaseMotorController* MotorController_ = nullptr;
#endif
   Team271PWM* MotorControllerPWM_ = nullptr;

public:

   /*
    *
    * Constructor/Destructor
    *
    */
   Team271Motor( Team271Base* const argParent,
                 const string& argMotorName,
                 const TEAM271_MOTOR_CONTROLLER_TYPE argControllerType,
                 const uint32_t argControlID,
                 Team271Motor* const argMasterMotor = nullptr );

   virtual ~Team271Motor( void );

   /*
    *
    * Getters
    *
    */
   const string GetClassName( void ) const override;

   const Team271Motor* GetMaster( void ) const;

   uint32_t GetIDControl( void ) const;

   TEAM271_CONTROL_MODE GetControlMode( void ) const;

   /*
    * Output Values
    */
   float GetValuePrev( void ) const;

   float GetValueCurrent( void ) const;

   float GetValueNeutral( void ) const;

   /*
    * Motor Controllers
    */
#if defined(T271_CTRE)
   BaseMotorController* GetBaseMotorController( void ) const;

   TalonSRX* GetTalon( void ) const;

   VictorSPX* GetVictor( void ) const;
#endif

   Team271PWM* GetPWM( void ) const;

   /*
    * Power
    */
   float GetBusVoltage( void ) const;

   float GetOutputVoltage( void ) const;

   float GetOutputCurrent( void ) const;

   /*
    *
    * Setters
    *
    */
   void SetMaster( const Team271Motor* const argMasterMotor );

   void SetControlMode( const TEAM271_CONTROL_MODE argSetControlMode );

   /*
    * The General Status frame has a default period of 10ms, and provides…
    * -Closed Loop Error: the closed-loop target minus actual position/velocity.
    * -Throttle: The current 10bit motor output duty cycle (-1023 full reverse to +1023 full forward).
    * -Forward Limit Switch Pin State
    * -Reverse Limit Switch Pin State
    * -Fault bits
    * -Applied Control Mode
    */
   void SetStatusFrameGeneral( const uint32_t argMilliSecs );

   /*
    * The Feedback Status frame has a default period of 20ms, and provides…
    * -Sensor Position: Position of the selected sensor
    * -Sensor Velocity: Velocity of the selected sensor
    * -Motor Current
    * -Sticky Faults
    * -Brake Neutral State
    * -Motor Control Profile Select
    */
   void SetStatusFrameFeedback( const uint32_t argMilliSecs );

   /*
    * The Quadrature Encoder Status frame has a default period of 100ms.
    * -Encoder Position: Position of the quadrature sensor
    * -Encoder Velocity: Velocity of the selected sensor
    * -Number of rising edges counted on the Index Pin.
    * -Quad A pin state.
    * -Quad B pin state.
    * -Quad Index pin state.
    */
   void SetStatusFrameQuadEncoder( const uint32_t argMilliSecs );

   /*
    * The Analog/Temp/BattV status frame has a default period of 100ms.
    * -Analog Position: Position of the selected sensor
    * -Analog Velocity: Velocity of the selected sensor
    * -Temperature
    * -Battery Voltage
    */
   void SetStatusFrameAnalogTempVbat( const uint32_t argMilliSecs );

   void SetStatusFramePulseWidthMeas( const uint32_t argMilliSecs );

   void SetStatusFrameBasePIDF( const uint32_t argMilliSecs );

   void SetStatusFrameMotionMagic( const uint32_t argMilliSecs );

   void SetInvertedOutput( const TEAM271_INVERTED_STATE argInvertedOutput );

   void SetNeutralMode( const TEAM271_NEUTRAL_MODE argNeutralMode );

   void SetPeakCurrentLimit( const float argCurrentLimit );

   void SetPeakCurrentDuration( const float argCurrentDuration );

   void SetContinuousCurrentLimit( const float argContinuousCurrentLimit );

   void SetEnableCurrentLimit( const bool argEnable );

   void SetContinuousCurrentLimit( const float argCurrentLimit,
         const float argCurrentDuration,
         const float argContinuousCurrentLimit );

   void SetOpenLoopRamp( const float argRamp );

   void SetClosedLoopRamp( const float argRamp );

   void SetPeakFwdVoltage( const float argVoltage );

   void SetPeakRevVoltage( const float argVoltage );

   void SetNominalFwdVoltage( const float argVoltage );

   void SetNominalRevVoltage( const float argVoltage );

   void SetNeutralDeadband( const float argDeadband );

   void SetVoltageCompensation( const bool argEnable );

   void SetMotionMagicVel( const int16_t argVel );

   void SetMotionMagicAccel( const int16_t argAccel );

   void SetNext( const float argNextValue,
         const float argNextArbFF = 0 );

   void SetNextValue( const float argNextValue );

   void SetNextArbFF( const float argNextArbFF );

   void SetNeutralValue( const float argValue );

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
   void PostRobotPeriodic( const bool argIsParent = false ) override;

   /*
    *
    * Robot State
    *
    */
   void ClearStickyFaults( const bool argIsParent = false ) override;

   /*
    *
    * Controller Functions
    *
    */
   void Setup( const Team271Motor* const argSetupFromMotor );

   /*
    *
    * Output Functions
    *
    */
   void Stop( void );

private:
   void SetOutput( const float argSet,
         const float argArgA = 0 );
};

#endif

#endif /* TEAM271_MOTOR_H_ */
