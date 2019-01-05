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

void MainCleanup( void )
{
#if !defined( T271_DASHBOARD ) && !defined( T271_SIM )
   //Stop_InputThread();
   //Stop_NetworkThread();

   Stop_FileThread();
#endif

#if defined( T271_FRC_NETCOM_ENABLE )
   // Unregister our new data condition variable.
   setNewDataSem( nullptr );
#endif
}

void MainInit( void )
{
   /*
    *
    * Banner & Version Info
    *
    */
   cout << endl;
   cout << "     ****************************************************" << endl;
   cout << "     *****  Team 271 - 2018 Robot Code " << _Version << "  *****" << endl;
   cout << "     *******  Build Date: " << _Date << "  " << _Time << "  *********" << endl;
   cout << "     ****************************************************" << endl;
   cout << endl;

#if defined( T271_DASHBOARD )
   LOG_NOTICE( "Main",
         "Running Dashboard Build" );
#elif defined( T271_SIM )
   LOG_NOTICE( "Main",
         "Running Simulation Build" );
#elif defined( T271_COMP_ROBOT )
   LOG_NOTICE( "Main",
         "Running Competition Build" );
#elif defined( T271_PRACTICE_ROBOT )
   LOG_NOTICE( "Main",
               "Running Practice Build" );
#endif

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                               eMatchStateRobotInit );

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV,
                               eMatchStateRobotInit );

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                               kPeriodMainLoop );

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MIN_SLEEP_TIME,
                               DBL_MAX );

   /*
    *
    * Clear out Subsystems
    *
    */
   DEBUG_PRINT( "Main",
                "Clear out Subsystems" );

   for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      Team271SubSystems_[i] = nullptr;
   }

   /*
    *
    * Create Threads
    *
    */
   DEBUG_PRINT( "Main",
                "Create Threads" );

#if !defined( T271_DASHBOARD ) && !defined( T271_SIM )
   Create_FileThread();
#endif

   /*
    *
    * Create Subsystems
    *
    */
   DEBUG_PRINT( "Main",
                "Create Subsystems" );

   /*
    * Create Robot Subsystem
    */
   Robot_ = Team271BaseManager::InstanceAdd( new Team271Robot() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_ROBOT] = Robot_;

   /*
    * Create Vision Subsystem
    */
#if defined( T271_VISION_ENABLE )
   /*
    _Vision = new Team271Vision();
    Team271SubSystems_[ROBOT_SUBSYSTEM_VISION] = _Vision;
    */
   cs::UsbCamera tmpCamera = CameraServer::GetInstance()->StartAutomaticCapture();
   tmpCamera.SetResolution( 160, 120 );
   tmpCamera.SetFPS( 60 );
   tmpCamera.SetBrightness( 10 );
   tmpCamera.SetWhiteBalanceManual( 10 );
   tmpCamera.SetExposureManual( 10 );
#endif

   /*
    * Create Drive Subsystem
    */
#if defined( T271_DRIVE_ENABLE )
   Drive_ = Team271BaseManager::InstanceAdd( new Team271Drive() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_DRIVE] = Drive_;
#endif

   /*
    * Create Ladder Subsystem
    */
#if defined( T271_LADDER_ENABLE )
   Ladder_ = Team271BaseManager::InstanceAdd( new Team271Ladder() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_LADDER] = Ladder_;
#endif

   /*
    * Create Arm Subsystem
    */
#if defined( T271_ARM_ENABLE )
   Arm_ = Team271BaseManager::InstanceAdd( new Team271Arm() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_ARM] = Arm_;
#endif

   /*
    * Create Herder Subsystem
    */
#if defined( T271_HERDER_ENABLE )
   Herder_ = Team271BaseManager::InstanceAdd( new Team271Herder() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_HERDER] = Herder_;
#endif

   /*
    * Create Auto Subsystem
    */
#if defined( T271_AUTO_ENABLE )
   Auto_ = Team271BaseManager::InstanceAdd( new Team271Auto() );
   Team271SubSystems_[ROBOT_SUBSYSTEM_AUTO] = Auto_;
#endif

   /*
    *
    * Initialize Subsystems
    *
    */
   DEBUG_PRINT( "Main",
                "Initialize Subsystems" );

   for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->RobotInit();
      }
   }

   /*
    *
    * Tell the DS that the robot is ready to be enabled
    *
    */
   DEBUG_PRINT( "Main",
                "Tell the DS that the robot is ready to be enabled" );

#if defined( T271_FRC_NETCOM_ENABLE )
   FRC_NetworkCommunication_observeUserProgramStarting();
#endif

   DEBUG_PRINT( "Main",
                "Start the Main Robot Loop" );
}

float InputLast_ = 0;

void MainLoop( void )
{
   float tmpTimeLoopStart = 0;

   /*
    *
    * Store Previous Match State
    *
    */
   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV,
                               GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE ) );

   /*
    *
    * Pre Robot Periodic
    * Handle Sub Systems
    *
    */
   tmpTimeLoopStart = Team271Robot::GetTime();

   for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->PreRobotPeriodic();
      }
   }

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_PRE,
                               Team271Robot::GetTimeDelta( tmpTimeLoopStart ) );

   /*
    *
    * Get Input
    *
    */
   tmpTimeLoopStart = Team271Robot::GetTime();

   /*
    * 1) Store how long it's been since the last time we've run our Loop
    * 2) Cache the start time of this loop iteration
    */
   /*
    if( Team271Robot::CompareTime( InputLast_,
    0.020 ) == 1 )
    */

   if( Team271Input::Instance().GetHasNewData() == 1 )
   {
      /*
       * Grab the latest Input
       */
      Team271Input::Instance().UpdateInput();

      /*
       * Update all SubSystem Inputs
       */
      for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
      {
         if( Team271SubSystems_[i] != nullptr )
         {
            Team271SubSystems_[i]->UpdateInput();
         }
      }
   }

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_INPUT,
                               Team271Robot::GetTimeDelta( tmpTimeLoopStart ) );

   /*
    *
    * Call the appropriate function depending upon the current robot mode
    *
    */
   tmpTimeLoopStart = Team271Robot::GetTime();

   if( Robot_ != nullptr )
   {
      /*
       *
       * Check Match State - Disconnected
       *
       */
      if( Robot_->IsDSAttached() == false )
      {
         /*
          * Check Match State
          */
         if( GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV ) != eMatchStateDisconnected )
         {
            DEBUG_PRINT( "Main",
                         "Disconnected Init" );

            /*
             * Set Match State
             * Set the period for the thread
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateDisconnectedInit );

            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                                        kPeriodDisconnected );

            /*
             * Set Match State
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateDisconnected );
         }
      }
      else if( Robot_->IsDisabled() == true )
      {
         /*
          * Check Match State - Disabled
          */
         if( GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV ) != eMatchStateDisabled )
         {
            DEBUG_PRINT( "Main",
                         "Disabled Init" );
            /*
             * Set Match State
             * Set the period for the thread
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateDisabledInit );

            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                                        kPeriodDisabled );

            /*
             * Handle Sub Systems
             * Run State Initialization
             */
            for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
            {
               if( Team271SubSystems_[i] != nullptr )
               {
                  Team271SubSystems_[i]->DisabledInit();
               }
            }

#if defined( T271_FRC_NETCOM_ENABLE )
            FRC_NetworkCommunication_observeUserProgramDisabled();
#endif

            /*
             * Set Match State
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateDisabled );
         }

         /*
          * Handle Sub Systems
          */
         for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
         {
            if( Team271SubSystems_[i] != nullptr )
            {
               Team271SubSystems_[i]->DisabledPeriodic();
            }
         }
      }

      /*
       *
       * Check Match State - Auto
       *
       */
      else if( Robot_->IsAutonomous() == true )
      {
         /*
          * Check Match State
          */
         if( GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV ) != eMatchStateAuto )
         {
            DEBUG_PRINT( "Main",
                         "Autonomous Init" );

            /*
             * Set Match State
             * Set the period for the thread
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateAutoInit );

            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                                        kPeriodAuto );

            /*
             * Log Auto Data
             */
#if defined( T271_LOGGER_ENABLE )
            Team271Logger::getInstance().AutonomousInit();
#endif
            /*
             * Handle Sub Systems
             * Run State Initialization
             */
            for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
            {
               if( Team271SubSystems_[i] != nullptr )
               {
                  Team271SubSystems_[i]->AutonomousInit();
               }
            }

#if defined( T271_FRC_NETCOM_ENABLE )
            FRC_NetworkCommunication_observeUserProgramAutonomous();
#endif

            /*
             * Set Match State
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateAuto );
         }

         /*
          * Handle Sub Systems
          */
         for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
         {
            if( Team271SubSystems_[i] != nullptr )
            {
               Team271SubSystems_[i]->AutonomousPeriodic();
            }
         }
      }

      /*
       *
       * Check Match State - Teleop
       *
       */
      else if( Robot_->IsTeleop() == true )
      {
         /*
          * Check Match State
          */
         if( GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV ) != eMatchStateTeleop )
         {
            DEBUG_PRINT( "Main",
                         "Teleop Init" );

            /*
             * Set Match State
             * Set the period for the thread
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateTeleopInit );

            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                                        kPeriodTeleop );

            /*
             * Handle Sub Systems
             * Run State Initialization
             */
            for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
            {
               if( Team271SubSystems_[i] != nullptr )
               {
                  Team271SubSystems_[i]->TeleopInit();
               }
            }

#if defined( T271_FRC_NETCOM_ENABLE )
            FRC_NetworkCommunication_observeUserProgramTeleop();
#endif

            /*
             * Set Match State
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateTeleop );
         }

         /*
          * Handle Sub Systems
          */
         for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
         {
            if( Team271SubSystems_[i] != nullptr )
            {
               Team271SubSystems_[i]->TeleopPeriodic();
            }
         }
      }

      /*
       *
       * Check Match State - Test
       *
       */
      else if( Robot_->IsTest() == true )
      {
         /*
          * Check Match State
          */
         if( GlobalNetworkObject_.Read<eMatchState>( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE_PREV ) != eMatchStateTest )
         {
            DEBUG_PRINT( "Main",
                         "Test Init" );

            /*
             * Set Match State
             * Set the period for the thread
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateTestInit );

            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD,
                                        kPeriodTest );

            /*
             * Handle Sub Systems
             * Run State Initialization
             */
            for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
            {
               if( Team271SubSystems_[i] != nullptr )
               {
                  Team271SubSystems_[i]->TestInit();
               }
            }

#if defined( T271_FRC_NETCOM_ENABLE )
            FRC_NetworkCommunication_observeUserProgramTest();
#endif

            /*
             * Set Match State
             */
            GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MATCH_STATE,
                                        eMatchStateTest );
         }

         /*
          * Handle Sub Systems
          */
         for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
         {
            if( Team271SubSystems_[i] != nullptr )
            {
               Team271SubSystems_[i]->TestPeriodic();
            }
         }
      }
   }

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_LOOP,
                               Team271Robot::GetTimeDelta( tmpTimeLoopStart ) );

   /*
    *
    * Post Robot Periodic
    * Handle Sub Systems
    *
    */
   tmpTimeLoopStart = Team271Robot::GetTime();

   for( uint8_t i = 0; i < ROBOT_SUBSYSTEM_MAX; i++ )
   {
      if( Team271SubSystems_[i] != nullptr )
      {
         Team271SubSystems_[i]->PostRobotPeriodic();
      }
   }

   GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_POST,
                               Team271Robot::GetTimeDelta( tmpTimeLoopStart ) );
}

/*
 zmq_pollitem_t items[] = { { ZMQRecv_,
 0,
 ZMQ_POLLIN,
 0 },
 { ZMQSend_,
 0,
 ZMQ_POLLIN,
 0 } };
 zmq_poll( items,
 2,
 -1 );
 if( items[0].revents & ZMQ_POLLIN )
 {
 }
 if( items[1].revents & ZMQ_POLLIN )
 {
 }
 */
