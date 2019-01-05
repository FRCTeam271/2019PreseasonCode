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

#ifndef __T271UTILITIES_H__
#define __T271UTILITIES_H__

#define T271_EPSILON       ( 0.0001 )

/*
 * Check if all of the Options are set
 * Returns true if all of the Options are set
 */
#define T271_OPTION_CHECK_SET( a, s )  ( (a & s) == s )

/*
 * Check if any of the Options are set
 * Returns true if any of the Options are set
 */
#define T271_OPTION_CHECK_SET_ANY( a, s )  ( (a & s) != 0 )

/*
 * Check if all of the Options are cleared
 * Returns true if all of the Options are cleared
 */
#define T271_OPTION_CHECK_CLEAR( a, c )  ( ((~a) & c) == c )

/*
 * Check if any of the Options are cleared
 * Returns true if any of the Options are cleared
 */
#define T271_OPTION_CHECK_CLEAR_ANY( a, c )  ( ((~a) & c) != 0 )

/*
 * Check if all of the Set Options are set and all of the Clear Options are cleared
 * Returns true if all of the Set Options are set and all of the Clear Options are cleared
 */
#define T271_OPTION_CHECK_SET_CLEAR( a, s, c )  ( T271_OPTION_CHECK_SET(a, s) && T271_OPTION_CHECK_CLEAR(a, c) )

/*
 * Sets the Options
 */
#define T271_OPTION_SET( a, s )  ( (a) |= (s) )

/*
 * Clears the Options
 */
#define T271_OPTION_CLEAR( a, c )  ( (a) &= ( ~(c) ) )

/*
 * Toggles the Options
 */
#define T271_OPTION_TOGGLE( a, t )  ( (a) ^= (t) )

extern float Team271_Round( const double argNumber );

extern float Team271_Round3Places( const double argNumber );

extern float Team271_Round4Places( const double argNumber );

extern long Team271_Map( const long x,
                         const long in_min,
                         const long in_max,
                         const long out_min,
                         const long out_max );
extern double Team271_Remap( const double newValueToMap,
                             const double newInMin,
                             const double newInMax,
                             const double newOutMin,
                             const double newOutMax );

extern void ApplyRadialDeadZone( float& pOutX, // out: resulting stick x value
                                 float& pOutY, // out: resulting stick y value
                                 const float x,    // in: initial stick x value
                                 const float y,    // in: initial stick x value
                                 const float deadZoneLow, // in: distance from zero to ignore
                                 const float deadZoneHigh // in: distance from unit circle to ignore
                                 );

#if defined( ENABLE_DEBUG_PRINTING )
void Team271DebugPrint( const char* const argClassName,
                        const char* const argLogFmt,
                        ... );

void Team271DebugPrintFunc( const char* const argClassName,
                            const char* const argFuncName,
                            const char* const argLogFmt,
                            ... );

#  if !defined(T271_DASHBOARD) && !defined(T271_SIM)
#     define DEBUG_PRINT(argClassName, argLogFmt, log_arg...) Team271DebugPrint( argClassName, argLogFmt, ##log_arg );
#     define DEBUG_PRINT_FUNC(argClassName, argFuncName, argLogFmt, log_arg...) Team271DebugPrintFunc( argClassName, argFuncName, argLogFmt, ##log_arg );
#  else
#     define DEBUG_PRINT(argClassName, argLogFmt, ...) Team271DebugPrint( argClassName, argLogFmt, __VA_ARGS__ );
#     define DEBUG_PRINT_FUNC(argClassName, argFuncName, argLogFmt, ...) Team271DebugPrintFunc( argClassName, argFuncName, argLogFmt, __VA_ARGS__ );
#  endif
#else
#  if !defined(T271_DASHBOARD) && !defined(T271_SIM)
#     define DEBUG_PRINT(argClassName, argLogFmt, log_arg...)
#     define DEBUG_PRINT_FUNC(argClassName, argFuncName, argLogFmt, log_arg...)
#  else
#     define DEBUG_PRINT(argClassName, argLogFmt, log_arg, ...)
#     define DEBUG_PRINT_FUNC(argClassName, argFuncName, argLogFmt, log_arg, ...)
#  endif
#endif

#endif /* __T271UTILITIES_H__ */
