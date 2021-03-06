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

#ifndef TEAM271_NAVX_H_
#define TEAM271_NAVX_H_

#if defined(T271_NAVX_ENABLE)

class Team271NavX : public Team271Sensor
{
protected:
   /*
    *
    * Variables
    *
    */
   AHRS* _AHRS = nullptr;

   /*
    * Sensor
    */
   float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0, mx = 0, my = 0, mz = 0;

   double _GyroX = 0;
   double _GyroY = 0;
   double _GyroZ = 0;

public:
   /*
    *
    * Constants
    *
    */

   /*
    *
    * Variables
    *
    */

   /*
    * Constructor/Destructor
    */
   Team271NavX( void );
   virtual ~Team271NavX( void );

   /*
    *
    * Robot Init
    *
    */
   void PreRobotPeriodic( void ) override;

   /*
    *
    * Dashboard
    *
    */
   void SendDashboardDataToDriversStation( void ) const override;

   /*
    *
    * Gyro Functions
    *
    */
   void GyroReset( void );
   void GyroCal( void );
   void GyroInit( void );
};

#endif

#endif /* TEAM271_GYRO_H_ */
