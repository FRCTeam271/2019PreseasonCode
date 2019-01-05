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

#if defined(T271_AUTO_ENABLE)
/*
 *
 * Auto 2
 * Center Switch - Fast High Gear
 * 3 Cube
 *
 */
void Team271_Auto2( void )
{
   const uint32_t tmpAutoModeNum = 2U;

   Team271AutoMode* tmpAutoMode = nullptr;
   AutoActionList* tmpAutoActions = nullptr;
   Team271AutoAction* tmpAutoAction = nullptr;
   Team271AutoActionDrive* tmpAutoActionDrive = nullptr;
   Team271AutoActionDriveGame* tmpAutoActionDriveGame = nullptr;
   Team271AutoActionLadder* tmpAutoActionLadder = nullptr;

   if( Auto_ != nullptr )
   {
      /*
       * Get the Auto Mode
       */
      tmpAutoMode = Auto_->GetAutoMode( tmpAutoModeNum );

      if( tmpAutoMode != nullptr )
      {
         /*
          *
          *
          * Cube 1 Start
          *
          *
          */

         /*
          * Drive Forward, Lift Ladder, Lower Arm
          */
         tmpAutoActions = tmpAutoMode->GetAutoActions( 0 );

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        0.35,
                                                                                                                     /* Driver */      1.0,
                                                                                                                     /* Passenger */   1.0 ) );

#if defined(T271_DRIVE_ENABLE)
            tmpAutoActionDrive->SetGear( DRIVE_HIGH );
#endif

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderTime,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   -0.5,
                                                                                                                        /* Arm */      1.0 ) );
         }

         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.5,
                                                                                                                                 /* Driver Right */      -0.5,
                                                                                                                                 /* Passenger Right */   0.5,

                                                                                                                                 /* Time Left */         0.5,
                                                                                                                                 /* Driver Left */       -0.5,
                                                                                                                                 /* Passenger Left */    0.5 ) );
         }

         /*
          * Drive Forward
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.55,
                                                                                                                                 /* Driver Right */      1.0,
                                                                                                                                 /* Passenger Right */   1.0,

                                                                                                                                 /* Time Left */         0.42,
                                                                                                                                 /* Driver Left */       1.0,
                                                                                                                                 /* Passenger Left */    1.0 ) );
         }

         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.5,
                                                                                                                                 /* Driver Right */      0.5,
                                                                                                                                 /* Passenger Right */   -0.5,

                                                                                                                                 /* Time Left */         0.45,
                                                                                                                                 /* Driver Left */       -0.5,
                                                                                                                                 /* Passenger Left */    0.5 ) );
         }

         /*
          * Drive Forward and Lower Arm
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        0.5,
                                                                                                                     /* Driver */      0.5,
                                                                                                                     /* Passenger */   0.5 ) );

            tmpAutoActionDrive->SetDriveType( eAutoActionDriveTypeStop );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderTime,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      1.0 ) );
         }

         /*
          * Spit the Cube
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderOuttake,
                                                                                                                        /* Time */     0.4,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0 ) );
         }

         /*
          *
          *
          * Cube 2 Start
          *
          *
          */

         /*
          * Drive Backwards, Lower Ladder, Lower Arm
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.0,
                                                                                                                     /* Driver */      -0.6,
                                                                                                                     /* Passenger */   -0.6 ) );

            tmpAutoActionDrive->SetDriveType( eAutoActionDriveTypeStop );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderTime,
                                                                                                                        /* Time */     1.0,
                                                                                                                        /* Ladder */   0.3,
                                                                                                                        /* Arm */      0.0 ) );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderTime,
                                                                                                                        /* Time */     1.75,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      1.0 ) );
         }

         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          * Turn 90 degrees or so towards the Cube Pile for Cube 2
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.25,
                                                                                                                                 /* Driver Right */      -0.6,
                                                                                                                                 /* Passenger Right */   0.6,

                                                                                                                                 /* Time Left */         0.25,
                                                                                                                                 /* Driver Left */       0.6,
                                                                                                                                 /* Passenger Left */    -0.6 ) );
         }

         /*
          * Drive Forward then Drive Backwards while Intaking
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderIntake,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0 ) );

            tmpAutoAction = tmpAutoActions->AddAutoAction( new Team271AutoAction( eAutoActionTypeNone,
                                                                                  /* Time */     2.4 ) );

            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.2,
                                                                                                                     /* Driver */      0.5,
                                                                                                                     /* Passenger */   0.5 ) );

            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.0,
                                                                                                                     /* Driver */      -0.6,
                                                                                                                     /* Passenger */   -0.6 ) );
         }

         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          * Turn back towards the Switch for Cube 2
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            /*
             * Invert the Drive time
             */
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.3,
                                                                                                                                 /* Driver Right */      0.6,
                                                                                                                                 /* Passenger Right */   -0.6,

                                                                                                                                 /* Time Left */         0.3,
                                                                                                                                 /* Driver Left */       -0.6,
                                                                                                                                 /* Passenger Left */    0.6 ) );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderIntake,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0     ) );
         }

         /*
          * Drive Forward and Raise Ladder
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        0.0,
                                                                                                                     /* Driver */      0.4,
                                                                                                                     /* Passenger */   0.4 ) );

            tmpAutoActionDrive->SetDriveType( eAutoActionDriveTypeStop );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderElevationTime,
                                                                                                                        /* Time */     1.5,
                                                                                                                        /* Ladder */   -1.0,
                                                                                                                        /* Arm */      0.0 ) );
         }

         /*
          * Spit the Second Cube
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderOuttake,
                                                                                                                        /* Time */     0.4,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0 ) );
         }

         /*
          *
          *
          * Cube 3 Start
          *
          *
          */

         /*
          * Drive Backward and Lower Ladder
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        0.8,
                                                                                                                     /* Driver */      -0.6,
                                                                                                                     /* Passenger */   -0.6 ) );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderElevationTime,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   1.0,
                                                                                                                        /* Arm */      0.0 ) );
         }

         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          * Turn 90 degrees or so towards the Cube Pile for Cube 2
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            /*
             * Invert the Drive time
             */
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.25,
                                                                                                                                 /* Driver Right */      -0.5,
                                                                                                                                 /* Passenger Right */   0.5,

                                                                                                                                 /* Time Left */         0.25,
                                                                                                                                 /* Driver Left */       0.5,
                                                                                                                                 /* Passenger Left */    -0.5 ) );
         }

         /*
          * Acquire the Third Cube
          *
          * Suck In
          * Drive Forward
          * Drive Backward
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderIntake,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0 ) );

            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.5,
                                                                                                                     /* Driver */      0.5,
                                                                                                                     /* Passenger */   0.5 ) );

            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.1,
                                                                                                                     /* Driver */      -0.6,
                                                                                                                     /* Passenger */   -0.6 ) );
         }


         /*
          * Turn Right by default
          * If the switch is on the left these will be inverted
          * Turn back towards the Switch for Cube 3
          *
          * Suck in the Cube
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            /*
             * Invert the Drive time
             */
            tmpAutoActionDriveGame = (Team271AutoActionDriveGame*)tmpAutoActions->AddAutoAction( new Team271AutoActionDriveGame( /* Time Right */        0.25,
                                                                                                                                 /* Driver Right */      0.5,
                                                                                                                                 /* Passenger Right */   -0.5,

                                                                                                                                 /* Time Left */         0.25,
                                                                                                                                 /* Driver Left */       -0.5,
                                                                                                                                 /* Passenger Left */    0.5 ) );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderIntake,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0     ) );
         }

         /*
          * Drive Forward and Raise Ladder
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionDrive = (Team271AutoActionDrive*)tmpAutoActions->AddAutoAction( new Team271AutoActionDrive( eAutoActionTypeDriveTime,
                                                                                                                     /* Time */        1.0,
                                                                                                                     /* Driver */      0.6,
                                                                                                                     /* Passenger */   0.6 ) );

            tmpAutoActionDrive->SetDriveType( eAutoActionDriveTypeStop );

            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeLadderElevationTime,
                                                                                                                        /* Time */     0.0,
                                                                                                                        /* Ladder */   -1.0,
                                                                                                                        /* Arm */      0.0 ) );
         }

         /*
          * Spit the Third Cube
          */
         tmpAutoActions = tmpAutoMode->GetNextAutoActions();

         if( tmpAutoActions != nullptr )
         {
            tmpAutoActionLadder = (Team271AutoActionLadder*)tmpAutoActions->AddAutoAction( new Team271AutoActionLadder( eAutoActionTypeHerderOuttake,
                                                                                                                        /* Time */     10.0,
                                                                                                                        /* Ladder */   0.0,
                                                                                                                        /* Arm */      0.0 ) );
         }
      }
   }
}
#endif
