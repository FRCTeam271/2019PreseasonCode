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

/*
 *
 * Globals
 *
 */
Team271NetworkObject GlobalNetworkObject_( "Globals",
                                           t271_global_data_index::T271_DATA_COUNT,
                                           T271_NETWORK_OBJECT_NETWORK_ID_GLOBALS );


#if defined( T271_COMP_ROBOT ) || defined( T271_PRACTICE_ROBOT )
Team271PublisherRobot RobotPub_;
//Team271NetworkSubscriber RobotSub_;
#endif

#if defined( T271_DASHBOARD )
//Team271PublisherRobot DashboardPub_;
Team271NetworkSubscriber DashboardSub_( "DashboardSub",
                                        T271_ZMQ_SUBSYNC_ADDR,
                                        T271_ZMQ_SUB_ADDR,
                                        "" );
#endif

/*
 *
 * Robot Object
 *
 */
Team271Robot* Robot_ = nullptr;

/*
 *
 * Subsystems
 *
 */
Team271Base* Team271SubSystems_[ROBOT_SUBSYSTEM_MAX];

#if defined(ENABLE_VISION)
pthread_t VisionThread_;
Team271Vision* Vision_ = nullptr;
#endif

#if defined(T271_IMU_ENABLE)
Team271IMU* IMU_ = nullptr;
#endif

#if defined(T271_DRIVE_ENABLE)
Team271Drive* Drive_ = nullptr;
#endif

#if defined(T271_LADDER_ENABLE)
Team271Ladder* Ladder_ = nullptr;
#endif

#if defined(T271_ARM_ENABLE)
Team271Arm* Arm_ = nullptr;
#endif

#if defined(T271_HERDER_ENABLE)
Team271Herder* Herder_ = nullptr;
#endif

#if defined(T271_AUTO_ENABLE)
Team271Auto* Auto_ = nullptr;
#endif
