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

std::unique_ptr<hal::tGlobal> Global_ = nullptr;
std::unique_ptr<hal::tSysWatchdog> Watchdog_ = nullptr;
std::unique_ptr<hal::tPower> Power_ = nullptr;

void KillExistingProgram( void )
{
   const uint32_t tmpTimeoutGraceful = 3000;
   const uint32_t tmpTimeoutForced = 1000;

   std::fstream tmpFS;
   pid_t tmpPID = 0;

   /*
    * By making this both in/out, it won't give us an error if it doesn't exist
    */
   tmpFS.open( "/var/lock/frc.pid",
               std::fstream::in | std::fstream::out );

   if( tmpFS.bad() == false )
   {
      if( !tmpFS.eof() && !tmpFS.fail() )
      {
         tmpFS >> tmpPID;

         /*
          * See if the pid is around, but we don't want to mess with init id=1, or ourselves
          */
         if( ( tmpPID >= 2 ) && ( tmpPID != getpid() ) )
         {
            DEBUG_PRINT( "KillExistingProgram",
                         "Killing previously running FRC program, PID: ",
                         tmpPID );

            /*
             * Check if PID exists
             */
            if( kill( tmpPID,
                      0 ) == 0 )
            {
               /*
                * Try to Kill it gracefully with SIGTERM
                */
               kill( tmpPID,
                     SIGTERM );

               std::this_thread::sleep_for( std::chrono::milliseconds( tmpTimeoutGraceful ) );

               /*
                * Check if it was Killed
                */
               if( kill( tmpPID,
                         0 ) == 0 )
               {
                  DEBUG_PRINT( "KillExistingProgram",
                               "FRC pid %s  did not die within %d ms. Force killing with kill -9",
                               tmpPID,
                               tmpTimeoutGraceful );

                  /*
                   * Force Kill with SIGKILL
                   */
                  const int32_t tmpStatus = kill( tmpPID,
                                                  SIGKILL );
                  if( tmpStatus != 0 )
                  {
                     DEBUG_PRINT( "KillExistingProgram",
                                  "Kill -9 error: %s",
                                  std::strerror( tmpStatus ) );
                  }

                  // Give a bit of time for the kill to take place
                  std::this_thread::sleep_for( std::chrono::milliseconds( tmpTimeoutForced ) );
               }
            }
         }
      }

      tmpFS.close();

      /*
       * Write the new PID
       */
      tmpFS.open( "/var/lock/frc.pid",
                  std::fstream::out | std::fstream::trunc );

      tmpFS.seekp( 0 );
      tmpPID = getpid();
      tmpFS << tmpPID << std::endl;

      tmpFS.close();
   }
}

static void SignalHandler_Kill( int32_t signal_value )
{
   DEBUG_PRINT( "Main",
                "EXIT" );

   /*
    * Cleanup our Program
    */
   MainCleanup();
}

int main( void )
{
   int32_t tmpStatus = 0;
   float tmpTimeMainLast = 0;

   /*
    * Set stdin and stdout to Line Buffered
    * - This means that stdin and stdout will flush on a newline
    */
   setlinebuf( stdin );
   setlinebuf( stdout );

   /*
    * Random Seed
    */
   srand( time( NULL ) );

   /*
    * This will trigger a SIGTERM if this parents process dies
    */
   prctl( PR_SET_PDEATHSIG,
          SIGTERM );

   cout << endl;

   /*
    * Kill an existing program
    */
   KillExistingProgram();

   /*
    * Setup FRC NetworkCommunication
    */
   FRC_NetworkCommunication_Reserve( nullptr );

   /*
    * image 4; Fixes errors caused by multiple processes. Talk to NI about this
    */
   nFPGA::nRoboRIO_FPGANamespace::g_currentTargetClass = nLoadOut::kTargetClass_RoboRIO;

   /*
    * Setup FRC ChipObject
    * - Global
    * - Watchdog
    * - Power
    */
   Global_.reset( hal::tGlobal::create( &tmpStatus ) );
   Watchdog_.reset( hal::tSysWatchdog::create( &tmpStatus ) );
   Power_.reset( hal::tPower::create( &tmpStatus ) );

   /*
    *
    * Setup This Thread
    * - Set Thread Name
    * - Set Thread Priority
    * - Set SIGINT and SIGTERM handlers
    *
    */
   if( pthread_setname_np( pthread_self(),
                           "T271MainThread" ) )
   {
      DEBUG_PRINT( "Main",
                   "Failed to set Main Thread Name : ",
                   std::strerror( errno ) );
   }

   sched_param ThreadMain_SCHParams_;

   ThreadMain_SCHParams_.sched_priority = 99;
   if( pthread_setschedparam( pthread_self(),
                              SCHED_FIFO,
                              &ThreadMain_SCHParams_ ) )
   {
      DEBUG_PRINT( "Main",
                   "Failed to set Main Thread Scheduling : ",
                   std::strerror( errno ) );
   }

   struct sigaction action;
   memset( &action,
           0,
           sizeof(struct sigaction) );
   action.sa_handler = SignalHandler_Kill;

   sigaction( SIGQUIT,
              &action,
              NULL );
   sigaction( SIGINT,
              &action,
              NULL );
   sigaction( SIGTERM,
              &action,
              NULL );

   /*
    *
    * Save WPILib Version for the Driver Station's FTP
    *
    */
   std::FILE* tmpFile = nullptr;
   tmpFile = std::fopen( "/tmp/frc_versions/FRC_Lib_Version.ini",
                         "w" );

   if( tmpFile != nullptr )
   {
      std::fputs( "C++ ",
                  tmpFile );

      std::fputs( "2018.4.1",
                  tmpFile );

      std::fclose( tmpFile );
   }

   MainInit();

   /*
    * Wait Forever
    */
   while( true )
   {
      /*
       * We've padded our sleep/loop time to get a more consistent execution rate
       *
       * So sit here till it's time to run our loop
       * Since this is a real time thread no one should preempt us
       *
       * 1) Check and Store how long it's been since the last time we've run our Loop
       * 2) Check if it's time to execute our Loop again
       * 2) Cache the start time of this loop iteration
       */
      do
      {
         GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_MAIN_DELTA,
                                     Team271Robot::GetTimeDelta( tmpTimeMainLast ) );
         //}while( ( 0.010 - SharedDataGlobal_Continuous_.TimeMainDelta_ ) > 0 );
      }while( ( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD ) - GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_TIME_MAIN_DELTA ) )
            > 0 );

      tmpTimeMainLast = Team271Robot::GetTime();

      /*
       * - Check if we've received any Data
       * - Run our Loop
       * - Clear Received Dirty Bits
       */
      //NetworkLoop_Robot_Sub();

      MainLoop();

      //NetworkLoop_Robot_Sub_ClearDirty();

      /*
       * - Send out any network data we have to
       * - Clear Dirty Bits
       */
      RobotPub_.PubLoop();
      RobotPub_.ClearDirty();

      /********************************************************************************************
       * WARNING: Any updates after this point won't be sent over the network until the next loop *
       ********************************************************************************************/

      /*
       *
       * Calculate the time we need to Sleep for
       * - Get the Total length of time we've been running for
       *
       */
      GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_TOTAL,
                                  Team271Robot::GetTimeDelta( tmpTimeMainLast ) );

      /*
       * - Get the the needed sleep time (in seconds)
       * - Calculate Min/Max Sleep Times
       */
      //SharedDataGlobal_Continuous_.SleepTime_ = 0.010 - SharedDataGlobal_Continuous_.TimeTotal_ - 0.0005;
      GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME,
                                  GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_LOOP_PREIOD )
                                        - GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_TIME_TOTAL ) - kSleepPad );

      if( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) > GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_MAX_SLEEP_TIME ) )
      {
         GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MAX_SLEEP_TIME,
                                     GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) );
      }

      if( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) < GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_MIN_SLEEP_TIME ) )
      {
         GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_MIN_SLEEP_TIME,
                                     GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) );
      }

      /*
       * Check to make sure we didn't exceed out time budget
       * If we did don't sleep and start execution of the next loop immediately
       */
      if( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) < 0.0f )
      {
         GlobalNetworkObject_.IncUInt32( t271_global_data_index::T271_GLOBAL_DATA_TBE_COUNT );

         GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_TIME_LAST_TBE,
                                     Team271Robot::GetTime() );

         GlobalNetworkObject_.Write( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME,
                                     0.0f );
      }
      else
      {
         /*
          * Sleep
          */
         //std::this_thread::sleep_for( std::chrono::milliseconds( _LoopPeriod ) );
         //std::this_thread::sleep_for( std::chrono::duration<double>( SharedDataGlobal_Continuous_.SleepTime_ ) );
         std::this_thread::sleep_for( std::chrono::milliseconds( (int64_t)( GlobalNetworkObject_.Read<float>( t271_global_data_index::T271_GLOBAL_DATA_SLEEP_TIME ) * 1000.0f ) ) );
         //std::this_thread::sleep_for( std::chrono::milliseconds( 10 ) );
         //std::this_thread::yield();
      }
   }

   /*
    * We SHOULD never get here, but if we do clean everything up
    */
   MainCleanup();

   return -1;
}

void NumericArrayResize( void )
{

}

void RTSetCleanupProc( void )
{

}

void EDVR_CreateReference( void )
{

}
