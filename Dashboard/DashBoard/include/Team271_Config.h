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

#ifndef TEAM271_CONFIG_H_
#define TEAM271_CONFIG_H_

//#define T271_CAN_ENABLE
//#define T271_FRC_NETCOM_ENABLE

//#define T271_SIM
#define T271_DASHBOARD
//#define T271_COMP_ROBOT
//#define T271_PRACTICE_ROBOT

#if defined( T271_DASHBOARD )
#  define T271_ZMQ_IO_THREADS      (3U)
#  define T271_ZMQ_PUBSYNC_ADDR    "tcp://*:5802"
#  define T271_ZMQ_PUB_ADDR        "tcp://*:5803"

#  define T271_ZMQ_SUBSYNC_ADDR    "tcp://10.2.17.2:5800"
#  define T271_ZMQ_SUB_ADDR        "tcp://10.2.17.2:5801"

//#  define T271_ZMQ_SUBSYNC_ADDR    "tcp://127.0.0.1:5800"
//#  define T271_ZMQ_SUB_ADDR        "tcp://127.0.0.1:5801"
#elif defined( T271_SIM )
#  define T271_ZMQ_IO_THREADS      (3U)

#  define T271_ZMQ_PUBSYNC_ADDR    "tcp://*:5800"
#  define T271_ZMQ_PUB_ADDR        "tcp://*:5801"

#  define T271_ZMQ_SUBSYNC_ADDR    "tcp://127.0.0.1:5802"
#  define T271_ZMQ_SUB_ADDR        "tcp://127.0.0.1:5803"
#else
#  define T271_ZMQ_IO_THREADS      (1U)

#  define T271_ZMQ_PUBSYNC_ADDR    "tcp://*:5800"
#  define T271_ZMQ_PUB_ADDR        "tcp://*:5801"

#  define T271_ZMQ_SUBSYNC_ADDR    "tcp://10.2.17.5:5802"
#  define T271_ZMQ_SUB_ADDR        "tcp://10.2.17.5:5803"
#endif

#define ENABLE_DEBUG_PRINTING
#if defined( ENABLE_DEBUG_PRINTING )
//#  define ENABLE_DEBUG_PRINTING_TRACE
#endif

#define T271_LOGGER_ENABLE

#if defined( T271_LOGGER_ENABLE )
#  define T271_LOGGER_ENABLE_DEBUG
//#  define T271_LOGGER_ENABLE_TRACE
#  define T271_LOGGER_ENABLE_NOTICE
#  define T271_LOGGER_ENABLE_WARN
#  define T271_LOGGER_ENABLE_ERROR
#endif

#define T271_NETWORK_DATA

#define T271_INPUT_ENABLE

#if defined( T271_INPUT_ENABLE )
#  define INPUT_ENABLE_DSO
#endif

#define T271_IO_ENABLE

#define T271_I2C_ENABLE
//#define T271_SPI_ENABLE

#define T271_PDP_ENABLE
#if defined( T271_PDP_ENABLE )
#  define T271_PDP_CAN_ENABLE
#endif

#define T271_PCM_ENABLE
#if defined( T271_PCM_ENABLE )
#  define T271_PCM_CAN_ENABLE
#endif

#define T271_SOLENOID_ENABLE

#define T271_PWM_ENABLE
#define T271_MOTOR_ENABLE
#define T271_SENSOR_ENABLE

#if defined( T271_SENSOR_ENABLE )
/*#define ENABLE_VISION*/
#  define T271_IMU_ENABLE

#  if defined( T271_IMU_ENABLE )
#     define T271_EM7180_ENABLE
/*#      define T271_NAVX_ENABLE*/
#  endif
#endif

#define T271_SHIFTER_ENABLE
#define T271_TRANSMISSION_ENABLE

#define T271_DRIVE_ENABLE
#define T271_LADDER_ENABLE
#define T271_ARM_ENABLE
#define T271_HERDER_ENABLE

#define T271_AUTO_ENABLE

//#define T271_ENABLE_PATHFINDER

#endif /* TEAM271_CONFIG_H_ */
