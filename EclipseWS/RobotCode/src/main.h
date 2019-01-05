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

#ifndef MAIN_H_
#define MAIN_H_

extern std::unique_ptr<hal::tGlobal> Global_;
extern std::unique_ptr<hal::tSysWatchdog> Watchdog_;
extern std::unique_ptr<hal::tPower> Power_;

extern "C"
{
void NumericArrayResize( void );
void RTSetCleanupProc( void );
void EDVR_CreateReference( void );
}

#endif
