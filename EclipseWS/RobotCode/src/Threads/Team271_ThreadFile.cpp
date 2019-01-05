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

std::thread* ThreadFile_ = nullptr;
sched_param ThreadFile_SCHParams_;

std::atomic<uint8_t> ThreadFile_Run_ = { 1U };

void Create_FileThread( void )
{
   /*
    * Create Logger Subsystem
    */

   DEBUG_PRINT( "Create_FileThread",
                "Create Thread" );

   ThreadFile_ = new std::thread( &FileThread );
}

void Stop_FileThread( void )
{
   ThreadFile_Run_.store( 0U,
                          std::memory_order_relaxed );

   if( ThreadFile_ != nullptr )
   {
      if( ThreadFile_->joinable() )
      {
         ThreadFile_->join();
      }

      delete ThreadFile_;
   }

   ThreadFile_ = nullptr;
}

void FileThread( void )
{
   /*
    *
    * Setup Thread Settings
    *
    */
   if( pthread_setname_np( pthread_self(),
                           "T271FileThread" ) )
   {
      DEBUG_PRINT( "FileThread",
                   "Failed to set File Thread Name : ",
                   std::strerror( errno ) );
   }

   ThreadFile_SCHParams_.sched_priority = 0;
   if( pthread_setschedparam( pthread_self(),
                              SCHED_IDLE,
                              &ThreadFile_SCHParams_ ) )
   {
      DEBUG_PRINT( "FileThread",
                   "Failed to set File Thread Scheduling : ",
                   std::strerror( errno ) );
   }

#if defined( T271_LOGGER_ENABLE )
   double tmpSystemVoltage = 0;

   /*
    *
    * Flush the logger at the end of the entire Robot Loop
    *
    */
   while( ThreadFile_Run_.load( std::memory_order_relaxed ) )
   {
      /*
       * Log The Match Data
       */
      Team271Logger::getInstance().LogMatchHeader();

      /*
       * Voltage Warnings
       */
#if defined( T271_PDP_ENABLE )
      /*
       * Get the System Voltage
       */
      tmpSystemVoltage = Team271PDP::getInstance().GetBusVoltage();

      /*
       * Check if the System Voltage is Critical
       */
      if( tmpSystemVoltage <= VOLTAGE_CRITICAL )
      {
         /*
          * Log the System Voltage and Flush the Log files to make sure the status gets saved to a file
          */
         Team271Logger::getInstance().LogRobotStats();
      }

      /*
       * Check if the System Voltage is approaching a Critical Level
       */
      else if( tmpSystemVoltage <= VOLTAGE_WARNING )
      {
         /*
          * Log the System Voltage and Flush the Log files to make sure the status gets saved to a file
          */
         Team271Logger::getInstance().LogRobotStats();
      }
#endif

      /*
       * Compressor Warnings
       */
#if defined( T271_PCM_ENABLE )
      if( Team271PCM::getInstance( 0 ).GetCompressorFault_TooHighCurrent() == true )
      {
         /*
          * Log the Compressor Fault and Flush the Log files to make sure the status gets saved to a file
          */
         Team271Logger::getInstance().LogRobotStats();
      }

      if( Team271PCM::getInstance( 0 ).GetCompressorFault_Shorted() == true )
      {
         /*
          * Log the Compressor Fault and Flush the Log files to make sure the status gets saved to a file
          */
         Team271Logger::getInstance().LogRobotStats();
      }

      if( Team271PCM::getInstance( 0 ).GetCompressorFault_NotConnected() == true )
      {
         /*
          * Log the Compressor Fault and Flush the Log files to make sure the status gets saved to a file
          */
         Team271Logger::getInstance().LogRobotStats();
      }
#endif

      /*
       * Flush Log Files
       */
      Team271Logger::getInstance().FlushFiles();

      /*
       *
       * Sleep
       *
       */
      std::this_thread::sleep_for( std::chrono::milliseconds( kFlushFilesUpdatePeriodThread ) );
   }
#endif
}
