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

#ifndef TEAM271_CONSTANTS_H_
#define TEAM271_CONSTANTS_H_

/*
 *
 * Constants
 *
 */
static constexpr float kRolloverTime = ( 1ll << 32 ) / 1e6;

/*
 * Times are in Seconds
 *
 * kPeriodMainLoop - This is the frequency we want our entire loop to run
 * kSleepPad - This is how much time we "allow" for the rest of the System/OS
 * kLoopTime - This is how much time is left over for our code
 *
 * kPeriodDisabled - Period Override for Disabled
 * kPeriodAuto - Period Override for Auto
 * kPeriodTeleop - Period Override for Teleop
 * kPeriodTest - Period Override for Test
 * kPeriodDisconnected - Period Override for Disconnected
 *
 * kTeam271Epsilon - Minimum Value for floats to differ for them to be "equal"
 *
 * kTeam271Pi - Our Value for PI
 */
static constexpr float kPeriodMainLoop = 0.005f;
static constexpr float kSleepPad = 0.0005f;
static constexpr float kLoopTime = kPeriodMainLoop - kSleepPad;

static constexpr float kPeriodDisabled = kPeriodMainLoop;
static constexpr float kPeriodAuto = kPeriodMainLoop;
static constexpr float kPeriodTeleop = kPeriodMainLoop;
static constexpr float kPeriodTest = kPeriodMainLoop;
static constexpr float kPeriodDisconnected = kPeriodMainLoop;

static constexpr float kTeam271Epsilon = 0.00001f;
//static constexpr float kTeam271Epsilon = std::numeric_limits<float>::epsilon();

static constexpr float kTeam271Pi = 3.14159265358979323846f;

/*
 *
 * Version Information
 *
 */
/* Version Number */
static const string _Version = "V0.02.00";
/* Compile Date */
static const string _Date = __DATE__;
/* Compile Time */
static const string _Time = __TIME__;

/*
 * Status Frame Rates
 */
static constexpr uint32_t _StatusFrameRateTimeout = 50U;

/*
 * Types
 */
typedef enum : uint8_t
{
   TEAM271_INVERTED_STATE_NOT_INVERTED,
   TEAM271_INVERTED_STATE_INVERTED,

   TEAM271_INVERTED_STATE_COUNT
} TEAM271_INVERTED_STATE;

typedef enum : uint8_t
{
   TEAM271_DSO_STATE_NONE,

   TEAM271_DSO_STATE_INACTIVE,
   TEAM271_DSO_STATE_ACTIVE,

   TEAM271_DSO_STATE_ENTER,
   TEAM271_DSO_STATE_EXIT,

   TEAM271_DSO_STATE_COUNT
} TEAM271_DSO_STATE;

/*
 * Enumerations
 */
typedef enum : uint8_t
{
   kRed,
   kBlue,
   kInvalid
} alliance_t;

typedef enum : uint8_t
{
   kNone,
   kPractice,
   kQualification,
   kElimination
} match_type_t;

typedef struct CAN_STATUS
{
   float percentBusUtilization;
   uint32_t busOffCount;
   uint32_t txFullCount;
   uint32_t receiveErrorCount;
   uint32_t transmitErrorCount;
} can_status_t;

constexpr int32_t kNumAccumulators = hal::tAccumulator::kNumSystems;
constexpr int32_t kNumAnalogTriggers = hal::tAnalogTrigger::kNumSystems;
constexpr int32_t kNumAnalogInputs = 8;
constexpr int32_t kNumAnalogOutputs = hal::tAO::kNumMXPRegisters;
constexpr int32_t kNumCounters = hal::tCounter::kNumSystems;
constexpr int32_t kNumDigitalHeaders = 10;
constexpr int32_t kNumDigitalMXPChannels = 16;
constexpr int32_t kNumDigitalSPIPortChannels = 5;
constexpr int32_t kNumPWMHeaders = hal::tPWM::kNumHdrRegisters;
constexpr int32_t kNumDigitalChannels = kNumDigitalHeaders + kNumDigitalMXPChannels + kNumDigitalSPIPortChannels;
constexpr int32_t kNumPWMChannels = hal::tPWM::kNumMXPRegisters + kNumPWMHeaders;
constexpr int32_t kNumDigitalPWMOutputs = hal::tDIO::kNumPWMDutyCycleAElements + hal::tDIO::kNumPWMDutyCycleBElements;
constexpr int32_t kNumEncoders = hal::tEncoder::kNumSystems;
constexpr int32_t kNumInterrupts = hal::tInterrupt::kNumSystems;
constexpr int32_t kNumRelayChannels = 8;
constexpr int32_t kNumRelayHeaders = kNumRelayChannels / 2;
constexpr int32_t kNumPCMModules = 63;
constexpr int32_t kNumSolenoidChannels = 8;
constexpr int32_t kNumPDPModules = 63;
constexpr int32_t kNumPDPChannels = 16;

typedef enum T271_CANDEVTYPE
{
   T271_CAN_Dev_kBroadcast = 0,
   T271_CAN_Dev_kRobotController = 1,
   T271_CAN_Dev_kMotorController = 2,
   T271_CAN_Dev_kRelayController = 3,
   T271_CAN_Dev_kGyroSensor = 4,
   T271_CAN_Dev_kAccelerometer = 5,
   T271_CAN_Dev_kUltrasonicSensor = 6,
   T271_CAN_Dev_kGearToothSensor = 7,
   T271_CAN_Dev_kPowerDistribution = 8,
   T271_CAN_Dev_kPneumatics = 9,
   T271_CAN_Dev_kMiscellaneous = 10,
   T271_CAN_Dev_kFirmwareUpdate = 31
} T271_CANDeviceType;

typedef enum T271_CANMAN
{
   T271_CAN_Man_kBroadcast = 0,
   T271_CAN_Man_kNI = 1,
   T271_CAN_Man_kLM = 2,
   T271_CAN_Man_kDEKA = 3,
   T271_CAN_Man_kCTRE = 4,
   T271_CAN_Man_kMS = 7,
   T271_CAN_Man_kTeamUse = 8,
} T271_CANManufacturer;

#define T271_CAN_SEND_PERIOD_NO_REPEAT 0
#define T271_CAN_SEND_PERIOD_STOP_REPEATING -1

/*
 * Excluding Continuous/Once, No More then 2 Rates are sent at once
 */
typedef enum : uint8_t
{
   /* Not Sent */
   TEAM271_DATA_RATE_NONE        = 0,

   /* Sent on Every Loop */
   TEAM271_DATA_RATE_CONTINUOUS  = ( 1 << 0 ),

   /* These Rates are Every Other Loop Alternating */
   TEAM271_DATA_RATE_HIGH        = ( 1 << 1 ),
   TEAM271_DATA_RATE_HIGH_ALT    = ( 1 << 2 ),

   /* These Rates are Every Third Loop Rotating */
   TEAM271_DATA_RATE_LOW         = ( 1 << 3 ),
   TEAM271_DATA_RATE_LOW_ALT_1   = ( 1 << 4 ),
   TEAM271_DATA_RATE_LOW_ALT_2   = ( 1 << 5 ),

   /* Reserved */
   TEAM271_DATA_RATE_RESERVED    = ( 1 << 6 ),

   /* Sent Once/On Demand */
   TEAM271_DATA_RATE_ONCE        = ( 1 << 7 ),

   TEAM271_DATA_RATE_ALL = 0xFF,
}TEAM271_DATA_RATE;

#endif /* TEAM271_CONSTANTS_H_ */
