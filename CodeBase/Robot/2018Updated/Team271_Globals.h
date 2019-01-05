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

#ifndef TEAM271_GLOBALS_H_
#define TEAM271_GLOBALS_H_

/*
 *
 * Subsystem Indexes
 *
 */

#define ROBOT_SUBSYSTEM_ROBOT    (0U)
#define ROBOT_SUBSYSTEM_VISION   (1U)
#define ROBOT_SUBSYSTEM_IMU      (2U)
#define ROBOT_SUBSYSTEM_DRIVE    (3U)
#define ROBOT_SUBSYSTEM_LADDER   (4U)
#define ROBOT_SUBSYSTEM_ARM      (5U)
#define ROBOT_SUBSYSTEM_HERDER   (6U)

#define ROBOT_SUBSYSTEM_AUTO     (7U)

#define ROBOT_SUBSYSTEM_MAX      (ROBOT_SUBSYSTEM_AUTO + 1U)

/*
 * Enumerations
 */
typedef enum : uint8_t
{
   eMatchStateRobotInit,

   eMatchStateDisabledInit,
   eMatchStateDisabled,

   eMatchStateAutoInit,
   eMatchStateAuto,

   eMatchStateTeleopInit,
   eMatchStateTeleop,

   eMatchStateTestInit,
   eMatchStateTest,

   eMatchStateDisconnectedInit,
   eMatchStateDisconnected
} eMatchState;

class t271_global_data_index : public t271_base_data_index
{
public:
   typedef enum : uint8_t
   {
      /*
       * Match State
       */
      T271_GLOBAL_DATA_MATCH_STATE = t271_base_data_index::T271_DATA_COUNT,
      T271_GLOBAL_DATA_MATCH_STATE_PREV,

      /*
       * Loop Timing
       */

      T271_GLOBAL_DATA_TIME_MAIN_DELTA,

      T271_GLOBAL_DATA_TIME_INPUT,

      T271_GLOBAL_DATA_TIME_PRE,
      T271_GLOBAL_DATA_TIME_LOOP,
      T271_GLOBAL_DATA_TIME_POST,

      T271_GLOBAL_DATA_TIME_TOTAL,

      T271_GLOBAL_DATA_LOOP_PREIOD,

      T271_GLOBAL_DATA_SLEEP_TIME,
      T271_GLOBAL_DATA_MIN_SLEEP_TIME,
      T271_GLOBAL_DATA_MAX_SLEEP_TIME,

      T271_GLOBAL_DATA_TIME_INPUT_DELTA,

      T271_GLOBAL_DATA_TIME_LAST_TBE,
      T271_GLOBAL_DATA_TBE_COUNT,

      /*
       * Network Stats
       */
      T271_GLOBAL_DATA_NETWORK_TIME_STAMP,
      T271_GLOBAL_DATA_NETWORK_TIME_STAMP_DELTA,

      T271_DATA_COUNT,
   }t271_global_data_index_t;
};

extern Team271NetworkObject GlobalNetworkObject_;

#if defined( T271_COMP_ROBOT ) || defined( T271_PRACTICE_ROBOT )
extern Team271PublisherRobot RobotPub_;
//extern Team271NetworkSubscriber RobotSub_;
#endif

#if defined( T271_DASHBOARD )
//extern Team271PublisherRobot DashboardPub_;
extern Team271NetworkSubscriber DashboardSub_;
#endif

/*
 *
 * Subsystems
 *
 */
extern Team271Base* Team271SubSystems_[ROBOT_SUBSYSTEM_MAX];

extern Team271Robot* Robot_;

#if defined(ENABLE_VISION)
extern pthread_t VisionThread_;
extern Team271Vision* Vision_;
#endif

#if defined(T271_IMU_ENABLE)
extern Team271IMU* IMU_;
#endif

#if defined(T271_DRIVE_ENABLE)
extern Team271Drive* Drive_;
#endif

#if defined(T271_LADDER_ENABLE)
extern Team271Ladder* Ladder_;
#endif

#if defined(T271_ARM_ENABLE)
extern Team271Arm* Arm_;
#endif

#if defined(T271_HERDER_ENABLE)
extern Team271Herder* Herder_;
#endif

#if defined(T271_AUTO_ENABLE)
extern Team271Auto* Auto_;
#endif

#endif /* TEAM271_GLOBALS_H_ */
