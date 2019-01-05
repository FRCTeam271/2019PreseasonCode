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

#ifndef __T271HWDEF_H__
#define __T271HWDEF_H__

#if !defined(TRUE)
#define TRUE      (1)
#endif

#if !defined(FALSE)
#define FALSE     (0)
#endif

/* System voltage "Warning" level in volts */
#define VOLTAGE_WARNING    (8.0)

/* System voltage "Critical" level in volts (BrownOut @ 6.8V) */
#define VOLTAGE_CRITICAL   (7.0)

/***********************************/
/* Hardware Connection Definitions */
/***********************************/

#if defined( T271_COMP_ROBOT )
#else
/*
 * Drive Motors
 */
/* Master: Has Encoder */
#  define TALON_SRX_FRONT_PASSENGER_MOTOR       (1U)
/* Followers */
#  define TALON_SRX_MIDDLE_PASSENGER_MOTOR      (15U)
#  define TALON_SRX_REAR_PASSENGER_MOTOR        (16U)

/* Master: Has Encoder */
#  define TALON_SRX_FRONT_DRIVER_MOTOR          (4U)
/* Followers */
#  define TALON_SRX_MIDDLE_DRIVER_MOTOR         (2U)
#  define TALON_SRX_REAR_DRIVER_MOTOR           (3U)

/*
 * Ladder Motors
 */
/* Master: Has Encoder */
#  define TALON_SRX_LADDER_THREE_MOTOR          (5U)
/* Followers */
#  define TALON_SRX_LADDER_ONE_MOTOR            (6U)
#  define TALON_SRX_LADDER_TWO_MOTOR            (7U)
#  define TALON_SRX_LADDER_FOUR_MOTOR           (8U)

/*
 *  Arm Motors
 */
/* Master: Has Encoder */
#  define TALON_SRX_ARM_ONE_MOTOR               (14U)
/* Followers */
/* #define TALON_SRX_ARM_TWO_MOTOR               (16U)*/

/*
 *  Herder Motors
 */
#  define TALON_SRX_HERDER_DRIVER_MOTOR         (11U)
#  define TALON_SRX_HERDER_PASSENGER_MOTOR      (12U)

/*
 *  Winch Motors
 */
/*#define TALON_SRX_WINCH_MOTOR            (9U)*/

/*
 *  Solenoids
 */
#  define PCM_TRANS_SPEED                 (0U)
#  define SOL_TRANS_SPEED_1               (1U)
#  define SOL_TRANS_SPEED_2               (2U)

#  define PCM_LADDER_SPEED                (0U)
#  define SOL_LADDER_SPEED_1              (0U)
#  define SOL_LADDER_SPEED_2              (3U)

#  define PCM_LADDER_BREAK                (0U)
#  define SOL_LADDER_BREAK_1              (5U)
#  define SOL_LADDER_BREAK_2              (6U)

#  define PCM_HERDER_GRAB                 (0U)
#  define SOL_HERDER_GRAB_1               (4U)
#  define SOL_HERDER_GRAB_2               (7U)

#  define PCM_LIFT                        (0U)
#  define SOL_LIFT_1                      (3U)
#  define SOL_LIFT_2                      (4U)

#endif

#endif /* __T271HWDEF_H__ */
