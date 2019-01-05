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

#if defined( T271_AUTO_ENABLE )

Team271AutoActionDrive::Team271AutoActionDrive( const eAutoActionType argAction,
                                                const float argActionLength,
                                                const float argDriver,
                                                const float argPassenger,
                                                const float argActionTimeout )
      : Team271AutoAction( argAction,
                           argActionLength,
                           argActionTimeout )
{
   /*
    *
    * Reset the Action
    *
    */
   Reset();

   /*
    *
    * Set Initial Parameters
    *
    */
#if defined(T271_DRIVE_ENABLE)
   DriveGear_ = DRIVE_NONE;
#endif
   DriveType_ = eAutoActionDriveTypeContinous;

   /*
    * Drive Gyro Values
    */
   GyroSet_ = 0;

   GyroMin_ = 0;
   GyroMax_ = 0;

   /*
    * Drive Encoder Values
    */
   EncMin_ = 0;
   EncMax_ = 0;

   DriverEncSet_ = 0;
   PassEncSet_ = 0;

   /*
    *
    * Handle Drive Parameters
    *
    */
   Driver_ = argDriver;
   Passenger_ = argPassenger;
}

Team271AutoActionDrive::~Team271AutoActionDrive( void )
{
   /*
    *
    * Free Pathfinder Objects
    *
    */
#if defined(T271_ENABLE_PATHFINDER)
   if( RightTrajectory_ != nullptr )
   {
      free( RightTrajectory_ );
   }

   if( LeftTrajectory_ != nullptr )
   {
      free( LeftTrajectory_ );
   }

   if( Trajectory_ != nullptr )
   {
      free( Trajectory_ );
   }

   if( Points_ != nullptr )
   {
      free( Points_ );
   }
#endif
}

void Team271AutoActionDrive::Reset( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Reset();
}

void Team271AutoActionDrive::Start( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Start();

#if defined(T271_DRIVE_ENABLE)
   /*
    *
    * Check if we want this Drive Action to change Gears
    *
    */
   if( DriveGear_ != DRIVE_NONE )
   {
      /*
       * Shift the Transmission
       */
      Drive_->ShiftGears( DriveGear_ );
   }
#endif
}

eAutoActionState Team271AutoActionDrive::Process( const float argListStartTime,
                                                  const float argListElapsedTime )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::Process( argListStartTime,
                               argListElapsedTime );

   /*
    *
    * Process Action
    *
    */

   /*
    * Drive for a Specific Time
    */
   if( AutoAction_ == eAutoActionTypeDriveTime )
   {
      /*
       * Wait till the Drive Action is over
       */
   }

   /*
    * Drive for a Specific Time Base on the Game Data
    */
   else if( AutoAction_ == eAutoActionTypeDriveTimeGame )
   {
      /*
       * Wait till the Drive Action is over
       */
   }

   /*
    * Turn Towards The Correct Switch Plate
    */
   else if( AutoAction_ == eAutoActionTypeDriveTurnGame )
   {
      /*
       * Invert the Drive time
       */
      if( Auto_ != nullptr )
      {
         if( Auto_->IsLeft() )
         {
            Driver_ = Driver_ * -1.0;
            Passenger_ = Passenger_ * 1.0;
         }
         else
         {
            Driver_ = Driver_ * 1.0;
            Passenger_ = Passenger_ * -1.0;
         }
      }
   }

   /*
    * Drive for a Distance using the Encoders
    */
   else if( AutoAction_ == eAutoActionTypeDriveEncoder )
   {
#if 0
      bool tmpReset = false;
      if( AutoActionState_ == eAutoActionStateInit )
      {
         tmpReset = true;
         //Drive_->SetDriveMode( 1 );
      }
      //Drive_->_PassPHigh = 2.4;

      double tmpError = Drive_->CalculateEncShift( DriverEncSet_,
            PassEncSet_,
            0.05,
            &Driver_,
            &Passenger_,
            EncMin_,
            EncMax_,
            tmpReset );
      if( tmpError == 0 )
      {
         Driver_ = 0;
         Passenger_ = 0;

         AutoActionState_ = eAutoActionStateDone;
      }
#endif
   }

   /*
    * Rotate to Angle
    */
   else if( AutoAction_ == eAutoActionTypeDriveGyro )
   {
#if 0
      bool tmpReset = false;
      if( AutoActionState_ == eAutoActionStateInit )
      {
         tmpReset = true;

         //Drive_->gyroPidKpHigh = 0.5;
         //Drive_->gyroPidKiHigh = 0.01;
      }

      double tmpError = Drive_->CalculateGyroShift( GyroSet_,
            GyroTol_,
            &Driver_,
            &Passenger_,
            GyroMin_,
            GyroMax_,
            tmpReset );

      if( tmpError == 0 )
      {
         Driver_ = 0;
         Passenger_ = 0;

         AutoActionState_ = eAutoActionStateDone;
      }
#endif
   }

   /*
    * Rotate to Target
    */
   else if( AutoAction_ == eAutoActionTypeDriveVisionGyro )
   {
#if 0
      bool tmpReset = false;
      if( AutoActionState_ == eAutoActionStateInit )
      {
         tmpReset = true;

         ///Drive_->gyroPidKpHigh = 0.5;
         //Drive_->gyroPidKiHigh = 0.01;
      }

      double tmpError = Drive_->CalculateGyroShift( GyroSet_,
            GyroTol_,
            &Driver_,
            &Passenger_,
            GyroMin_,
            GyroMax_,
            tmpReset );

      if( tmpError == 0 )
      {
         Driver_ = 0;
         Passenger_ = 0;

         AutoActionState_ = eAutoActionStateDone;
      }
#endif
   }

   /*
    * Drive for a Distance using the Encoders at some Heading using the Gyro
    */
   else if( AutoAction_ == eAutoActionTypeDriveEncoderGyro )
   {
#if 0
      bool tmpReset = false;
      if( AutoActionState_ == eAutoActionStateInit )
      {
         tmpReset = true;
      }

      double tmpError = Drive_->CalculateEncShift( DriverEncSet_,
            PassEncSet_,
            0.05,
            &Driver_,
            &Passenger_,
            EncMin_,
            EncMax_,
            tmpReset );

      double tmpDriverGyro = 0;
      double tmpPassengerGyro = 0;
      Drive_->CalculateGyroShift( GyroSet_,
            GyroTol_,
            &tmpDriverGyro,
            &tmpPassengerGyro,
            GyroMin_,
            GyroMax_,
            tmpReset );

      Driver_ += tmpDriverGyro;
      Passenger_ += tmpPassengerGyro;

      if( tmpError == 0 )
      {
         Driver_ = 0;
         Passenger_ = 0;

         AutoActionState_ = eAutoActionStateDone;
      }
#endif
   }

   /*
    * Drive using PathFinder
    */
   else if( AutoAction_ == eAutoActionTypeDrivePathFinder )
   {
#if 0
      if( AutoActionState_ == eAutoActionStateInit )
      {
      }
      // Arg 1: The EncoderConfig
      // Arg 2: The EncoderFollower for this side
      // Arg 3: The Trajectory generated from `pathfinder_modify_tank`
      // Arg 4: The Length of the Trajectory (length used in Segment seg[length];)
      // Arg 5: The current value of your encoder
      double l = pathfinder_follow_encoder( _ConfigDriver, &FollowerDriver_, LeftTrajectory_, Length_, Drive_->GetDriverPosition() );
      double r = pathfinder_follow_encoder( ConfigPassenger_, &_FollowerPassenger, RightTrajectory_, Length_, Drive_->GetPassengerPosition() );

      // -- using l and r from the previous code block -- //
      double gyro_heading = Drive_->GetHeading();
      double desired_heading = r2d( FollowerDriver_.heading);

      double angle_difference = desired_heading - gyro_heading;// Make sure to bound this from -180 to 180, otherwise you will get super large values

      double turn = 0.8 * (-1.0/80.0) * angle_difference;

      Driver_ = l + turn;
      Passenger_ = r - turn;
#endif
   }

   /*
    *
    * Call Drive Class
    *
    */
#if defined(T271_DRIVE_ENABLE)
   Drive_->SetSetpoint( Driver_,
                        Passenger_ );
#endif

   return AutoActionState_;
}

void Team271AutoActionDrive::End( void )
{
   /*
    *
    * Base Call
    *
    */
   Team271AutoAction::End();

   /*
    *
    * Check if we should stop driving at the end of this Auto Action
    *
    */
   if( DriveType_ == eAutoActionDriveTypeStop )
   {
      Driver_ = 0;
      Passenger_ = 0;

#if defined(T271_DRIVE_ENABLE)
      Drive_->SetSetpoint( Driver_,
                           Passenger_ );
#endif
   }
}

void Team271AutoActionDrive::SetDriveType( const eAutoActionDriveType argDriveType )
{
   DriveType_ = argDriveType;
}

#if defined(T271_DRIVE_ENABLE)
void Team271AutoActionDrive::SetGear( const eDriveGear argDriveGear )
{
   DriveGear_ = argDriveGear;
}
#endif
#endif
