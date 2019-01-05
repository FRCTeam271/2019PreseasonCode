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

#ifndef TEAM271_AUTO_ACTION_DRIVE_H_
#define TEAM271_AUTO_ACTION_DRIVE_H_

#if defined(T271_AUTO_ENABLE)

typedef enum
{
   eAutoActionDriveTypeNone = 0,
   eAutoActionDriveTypeContinous,
   eAutoActionDriveTypeStop,
}eAutoActionDriveType;

class Team271AutoActionDrive : public Team271AutoAction
{
protected:
   /*
    * Drive Output
    */
#if defined(T271_DRIVE_ENABLE)
   eDriveGear DriveGear_ = DRIVE_DEFAULT_AUTO_GEAR;
#endif
   eAutoActionDriveType DriveType_ = eAutoActionDriveTypeNone;

   float Driver_ = 0.0;
   float Passenger_ = 0.0;
   float DriverB_ = 0.0;
   float PassengerB_ = 0.0;

   /*
    * Drive Gyro Values
    */
   float GyroSet_ = 0;

   float GyroTol_ = 0;

   float GyroMin_ = 0;
   float GyroMax_ = 0;

   /*
    * Drive Encoder Values
    */
   float EncMin_ = 0;
   float EncMax_ = 0;

   float DriverEncSet_ = 0;
   float PassEncSet_ = 0;

   /*
    * Drive PathFinder
    */
#if defined(T271_ENABLE_PATHFINDER)
   int32_t Length_ = 0;

   Waypoint* Points_ = nullptr;
   Segment* Trajectory_ = nullptr;
   Segment* LeftTrajectory_ = nullptr;
   Segment* RightTrajectory_ = nullptr;

   EncoderFollower FollowerDriver_;
   EncoderFollower FollowerPassenger_;

   EncoderConfig ConfigDriver_;
   EncoderConfig ConfigPassenger_;
#endif

public:
   /*
    * Functions
    */
   Team271AutoActionDrive( const eAutoActionType argAction,
                           const float argActionLength = 0,
                           const float argDriver = 0,
                           const float argPassenger = 0,
                           const float argActionTimeout = 0 );

   ~Team271AutoActionDrive( void );

   void Reset( void ) override;

   void Start( void ) override;

   eAutoActionState Process( const float argStartTime,
                             const float argElapsedTime ) override;

   void End( void ) override;

   void SetDriveType( const eAutoActionDriveType argDriveType );

#if defined(T271_DRIVE_ENABLE)
   void SetGear( const eDriveGear argDriveGear );
#endif
};

#endif

#endif
