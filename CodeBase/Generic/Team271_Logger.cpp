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

#if defined(T271_LOGGER_ENABLE)

static Team271Logger LoggerInstance_( (LogLevel_t)( LL_DEBUG | LL_NOTICE | LL_WARNING | LL_ERROR ),
                                      (LogLevel_t)( LL_DEBUG | LL_NOTICE | LL_WARNING | LL_ERROR ) );

Team271Logger& Team271Logger::getInstance( void )
{
   return LoggerInstance_;
}

Team271Logger::Team271Logger( const LogLevel_t argEnabledLogLevels,
                              const LogLevel_t argEnabledLogLevelsPrint )
      : EnabledLevels_( argEnabledLogLevels ),
        EnabledLevelsPrint_( argEnabledLogLevelsPrint )
{
   /*
    * Initialize the Logger
    */
   _TraceFile = CreateFile( "/home/lvuser/log",
                            "Team271Logger_Trace",
                            _TraceFilePath );

   LogTrace2( "\r\n\r\nTrace Logger Initialized\r\n\r\n" );
}

Team271Logger::~Team271Logger( void )
{
   CloseFile( _TraceFile );
   CloseFile( _MatchFile );
}

/*
 *
 * Autonomous
 *
 */
void Team271Logger::AutonomousInit( void )
{
   /*
    * Log the Game Specific Message
    */
   if( Robot_ != nullptr )
   {
      LogMatch( LL_TRACE,
                "AutonomousInit: Game Specific Message: %s\r\n",
                Robot_->GetGameSpecificMessage() );
   }
   else
   {
      LogMatch( LL_ERROR,
                "AutonomousInit: Game Specific Message: No Driver Station\r\n" );
   }
}

/*
 *
 * Logger Functions
 *
 */

string Team271Logger::CreateMatchFilePath( void ) const
{
   string tmpReturnString = "/home/lvuser/log";

   if( Robot_ != nullptr )
   {
      if( Robot_->IsDSAttached() == true )
      {
         Robot_->UpdateMatchInfo();

         /*
          * Get Event and Match Data
          */
         const string tmpEventName = Robot_->GetEventName();
         const match_type_t tmpMatchType = Robot_->GetMatchType();
         const uint32_t tmpMatchNum = Robot_->GetMatchNumber();
         const uint32_t tmpReplayNum = Robot_->GetReplayNumber();

         /*
          * Create the Match File Path
          */
         char tmpFilePath[_LOG_PATH_LEN] = "";

         /*
          * Append Event Name
          */
         if( tmpEventName != "" )
         {
            sprintf( tmpFilePath,
                     "%s/%s",
                     tmpFilePath,
                     tmpEventName.c_str() );
         }

         /*
          * Append Match Type
          */
         if( tmpMatchType != kNone )
         {
            if( tmpMatchType == kPractice )
            {
               sprintf( tmpFilePath,
                        "%s/Practice",
                        tmpFilePath );
            }
            else if( tmpMatchType == kQualification )
            {
               sprintf( tmpFilePath,
                        "%s/Practice",
                        tmpFilePath );
            }
            else if( tmpMatchType == kElimination )
            {
               sprintf( tmpFilePath,
                        "%s/Elim",
                        tmpFilePath );
            }
         }

         /*
          * Append the Match Number
          */
         if( tmpMatchNum > 0 )
         {
            sprintf( tmpFilePath,
                     "%s/%d",
                     tmpFilePath,
                     tmpMatchNum );
         }

         /*
          * Append the Replay Number
          */
         if( tmpReplayNum > 0 )
         {
            sprintf( tmpFilePath,
                     "%s/%d",
                     tmpFilePath,
                     tmpReplayNum );
         }

         tmpReturnString += tmpFilePath;
      }
   }

   return tmpReturnString;
}

void Team271Logger::LogMatchHeader( void )
{
   if( Robot_ != nullptr )
   {
      if( ( Robot_->IsDSAttached() == true ) && ( LoggedMatchHeader_ == 0 ) )
      {
         Robot_->UpdateMatchInfo();

         /*
          * Get Event and Match Data
          */
         const string tmpEventName = Robot_->GetEventName();
         const match_type_t tmpMatchType = Robot_->GetMatchType();
         const uint32_t tmpMatchNum = Robot_->GetMatchNumber();
         const uint32_t tmpReplayNum = Robot_->GetReplayNumber();
         const alliance_t tmpAlliance = Robot_->GetAlliance();
         const uint32_t tmpAllianceLoc = Robot_->GetLocation();
         const string tmpGameMessage = Robot_->GetGameSpecificMessage();

         /*
          * Create/Open the Match File
          */
         _MatchFile = CreateFile( CreateMatchFilePath().c_str(),
                                  "Team271Logger_Match",
                                  _MatchFilePath );

         LogMatch2( "\r\n\r\nMatch Logger Initialized\r\n\r\n" );

         /*
          * Log the Event Name
          */
         if( tmpEventName != "" )
         {
            LogMatch2( "Event Name: %s\r\n",
                       tmpEventName.c_str() );
         }
         else
         {
            LogMatch2( "Event Name: None\r\n" );
         }

         /*
          * Log the Match Type
          */
         if( tmpMatchType == kNone )
         {
            LogMatch2( "Match Type: None\r\n" );
         }
         else if( tmpMatchType == kPractice )
         {
            LogMatch2( "Match Type: Practice\r\n" );
         }
         else if( tmpMatchType == kQualification )
         {
            LogMatch2( "Match Type: Qualification\r\n" );
         }
         else if( tmpMatchType == kElimination )
         {
            LogMatch2( "Match Type: Elimination\r\n" );
         }
         else
         {
            LogMatch2( "Match Type: Error\r\n" );
         }

         /*
          * Log the Match Number
          */
         LogMatch2( "Match Number: %d\r\n",
                    tmpMatchNum );

         /*
          * Log the Replay Number
          */
         LogMatch2( "Replay Number: %d\r\n",
                    tmpReplayNum );

         /*
          * Log the Alliance
          */
         if( tmpAlliance == kRed )
         {
            LogMatch2( "Alliance: Red\r\n" );
         }
         else if( tmpAlliance == kBlue )
         {
            LogMatch2( "Alliance: Blue\r\n" );
         }
         else if( tmpAlliance == kInvalid )
         {
            LogMatch2( "Alliance: Invalid\r\n" );
         }

         /*
          * Log the Alliance Location
          */
         LogMatch2( "Alliance Location: %d\r\n",
                    tmpAllianceLoc );

         /*
          * Log the Game Specific Message
          */
         LogMatch2( "Game Specific Message: %s\r\n",
                    tmpGameMessage.c_str() );

         DEBUG_PRINT_FUNC( "Team271Logger",
                           T271_FUNC_NAME,
                           "Match Data Logged" );

         LogTrace( LL_NOTICE,
                   "Match Data Logged\r\n" );

         /*
          * Only Log the Data Once
          */
         LoggedMatchHeader_ = 1;
      }
   }
}

void Team271Logger::LogRobotStats( void )
{
   LogMatch( LL_TRACE,
             "****Robot Stats ****\r\n" );

#if defined(T271_PDP_ENABLE)
   Team271PDP::getInstance().PreRobotPeriodic();

   LogMatch2( "Battery Voltage: %f\r\n",
              Team271PDP::getInstance().GetBusVoltage() );
#endif

   LogMatch2( "Is Browned Out: %d\r\n",
              Team271Robot::IsBrownedOut() );
   LogMatch2( "Fault Count 3.3V: %d | 5V: %d | 5V Under: %d | 6V: %d\r\n",
              Team271Robot::GetFaultCount3V3(),
              Team271Robot::GetFaultCount5V(),
              Team271Robot::GetFaultCount5V_UnderVoltage(),
              Team271Robot::GetFaultCount6V() );

   if( Robot_ != nullptr )
   {
      const can_status_t* const tmpCANStatus = Robot_->GetCANStatus();
      if( tmpCANStatus != nullptr )
      {
         LogMatch2( "CAN Status: Utilization: %f | Tx Error: %d | Rx Error: %d\r\n",
                    tmpCANStatus->percentBusUtilization,
                    tmpCANStatus->transmitErrorCount,
                    tmpCANStatus->receiveErrorCount );
      }
   }
}

void Team271Logger::LogTrace( const LogLevel_t argLevel,
                              const char* argLogFmt,
                              ... )
{
   uint32_t tmpSize = 0;
   uint32_t tmpPreStrLen = 0;

   char tmpBuffer[_LOG_BUFFSIZE] = "";
   char* star = tmpBuffer;

   va_list args;

   if( ( T271_OPTION_CHECK_SET_ANY( EnabledLevels_, argLevel ) != 0 ) || ( T271_OPTION_CHECK_SET_ANY( EnabledLevelsPrint_, argLevel ) != 0 ) )
   {
      tmpPreStrLen = _premakestr( star,
                                  argLevel );

      star += tmpPreStrLen;

      va_start( args,
                argLogFmt );

      tmpSize = vsnprintf( star,
                           _LOG_BUFFSIZE - tmpPreStrLen,
                           argLogFmt,
                           args );
      va_end( args );
   }

#if defined( ENABLE_DEBUG_PRINTING )
   if( T271_OPTION_CHECK_SET_ANY( EnabledLevels_, argLevel ) != 0 )
   {
      //tmpBuffer[_LOG_BUFFSIZE-1] = 0;
      printf( tmpBuffer );
   }
#endif

   if( T271_OPTION_CHECK_SET_ANY( EnabledLevels_, argLevel ) != 0 )
   {
      _TraceMutex.lock();

      if( _TraceFile != nullptr )
      {
         if( fwrite( tmpBuffer,
                     (size_t)(tmpPreStrLen + tmpSize),
                     1,
                     _TraceFile ) != 1 ) //only write 1 item
         {
            fprintf( stderr,
                     "\r\nTeam271Logger: Failed to write to logfile. errno:%s    message:%s\r\n",
                     strerror( errno ),
                     tmpBuffer );
         }
      }

      _TraceMutex.unlock();
   }
}

void Team271Logger::LogTrace2( const char* argLogFmt,
                               ... )
{
   uint32_t _size = 0;
   uint32_t prestrlen = 0;

   char tmpBuffer[_LOG_BUFFSIZE];
   char* star = tmpBuffer;

   va_list args;

   va_start( args,
             argLogFmt );

   _size = vsnprintf( star,
                      _LOG_BUFFSIZE - prestrlen,
                      argLogFmt,
                      args );
   va_end( args );

#if defined( ENABLE_DEBUG_PRINTING )
   printf( tmpBuffer );
#endif

   if( _TraceFile != nullptr )
   {
      _TraceMutex.lock();

      if( fwrite( tmpBuffer,
                  (size_t)(prestrlen + _size),
                  1,
                  _TraceFile ) != 1 ) //only write 1 item
      {
         fprintf( stderr,
                  "\r\nTeam271Logger: Failed to write to logfile. errno:%s    message:%s\r\n",
                  strerror( errno ),
                  tmpBuffer );
      }

      _TraceMutex.unlock();
   }

}

void Team271Logger::LogMatch( const LogLevel_t argLevel,
                              const char* argLogFmt,
                              ... )
{
   uint32_t _size = 0;
   uint32_t prestrlen = 0;

   char tmpBuffer[_LOG_BUFFSIZE] = "";
   char* star = tmpBuffer;

   va_list args;

   if( ( T271_OPTION_CHECK_SET_ANY( EnabledLevels_, argLevel ) != 0 ) || ( T271_OPTION_CHECK_SET_ANY( EnabledLevelsPrint_, argLevel ) != 0 ) )
   {
      prestrlen = _premakestr( star,
                               argLevel );
      star += prestrlen;

      va_start( args,
                argLogFmt );

      _size = vsnprintf( star,
                         _LOG_BUFFSIZE - prestrlen,
                         argLogFmt,
                         args );
      va_end( args );
   }

   if( T271_OPTION_CHECK_SET_ANY( EnabledLevelsPrint_, argLevel ) != 0 )
   {
#if defined( ENABLE_DEBUG_PRINTING )
      printf( tmpBuffer );
#endif
   }

   if( T271_OPTION_CHECK_SET_ANY( EnabledLevels_, argLevel ) != 0 )
   {
      _MatchMutex.lock();

      if( _MatchFile != nullptr )
      {
         if( fwrite( tmpBuffer,
                     (size_t)(prestrlen + _size),
                     1,
                     _MatchFile ) != 1 ) //only write 1 item
         {
            fprintf( stderr,
                     "\r\nTeam271Logger: Failed to write to logfile. errno:%s    message:%s\r\n",
                     strerror( errno ),
                     tmpBuffer );
         }
      }

      _MatchMutex.unlock();
   }

}

void Team271Logger::LogMatch2( const char* argLogFmt,
                               ... )
{
   uint32_t _size = 0;
   uint32_t prestrlen = 0;

   char tmpBuffer[_LOG_BUFFSIZE] =  "";
   char* star = tmpBuffer;

   va_list args;

   va_start( args,
             argLogFmt );

   _size = vsnprintf( star,
                      _LOG_BUFFSIZE - prestrlen,
                      argLogFmt,
                      args );

   va_end( args );

#if defined( ENABLE_DEBUG_PRINTING )
   printf( tmpBuffer );
#endif

   if( _MatchFile != nullptr )
   {
      _MatchMutex.lock();

      if( fwrite( tmpBuffer,
                  (size_t)(prestrlen + _size),
                  1,
                  _MatchFile ) != 1 ) //only write 1 item
      {
         fprintf( stderr,
                  "\r\nTeam271Logger: Failed to write to logfile. errno:%s    message:%s\r\n",
                  strerror( errno ),
                  tmpBuffer );
      }

      _MatchMutex.unlock();
   }
}

void Team271Logger::FlushFiles( void )
{
   if( _TraceFile != nullptr )
   {
      _TraceMutex.lock();

      fflush( _TraceFile );

      _TraceMutex.unlock();
   }

   if( _MatchFile != nullptr )
   {
      _MatchMutex.lock();

      fflush( _MatchFile );

      _MatchMutex.unlock();
   }
}

#if defined(T271_DASHBOARD) || defined(T271_SIM)
#include  <io.h>

std::wstring s2ws(const std::string& str)
{
   int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
   std::wstring wstrTo(size_needed, 0);
   MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
   return wstrTo;
}
#endif

FILE* Team271Logger::CreateFile( const char* const argDir,
                                 const char* const argFileName,
                                 char* argFilePath ) const
{
   FILE* tmpFile = nullptr;
   char tmpFilePath[_LOG_PATH_LEN];

   uint32_t tmpDirExists = 1;

   /*
    * Check if the directory Exists
    */
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   if( access( argDir,
               0 ) == -1 )
   {
      /*
       * Directory doesn't exist, create it
       */
      if( mkdir( argDir,
                 S_IREAD | S_IWRITE | S_IEXEC ) < 0 )
      {
         tmpDirExists = 0;
      }
   }
#else
   if( _access( argDir,
               0 ) == -1 )
   {
      /*
       * Directory doesn't exist, create it
       */
      if( CreateDirectory(s2ws(string(argDir)).c_str(), NULL) == ERROR_ALREADY_EXISTS )
      {
         tmpDirExists = 0;
      }
   }
#endif

   /*
    * Only continue if the directory Exists
    */
   if( tmpDirExists == 1 )
   {
      snprintf( tmpFilePath,
                _LOG_PATH_LEN,
                "%s/%s.access",
                argDir,
                argFileName );

      if( strlen( tmpFilePath ) < _LOG_PATH_LEN )
      {
         /*
          * Open the file
          */
         tmpFile = fopen( tmpFilePath,
                          "a" );
         if( tmpFile != nullptr )
         {
            setvbuf( tmpFile,
                     (char *)NULL,
                     _IOLBF,
                     0 );
            fprintf( stderr,
                     "\r\n\r\nTeam271Logger: Log File Created: %s\r\n",
                     tmpFilePath );

            strcpy( argFilePath,
                    tmpFilePath );
         }
         else
         {
            tmpFile = stdout;
            fprintf( stderr,
                     "\r\n\r\nTeam271Logger: Can not open Log File: %s\r\n",
                     tmpFilePath );
         }
      }
      else
      {
         tmpFile = stdout;
         fprintf( stderr,
                  "\r\n\r\nTeam271Logger: Log File Path is too long:%u limit:%u\r\n",
                  (uint32_t)strlen( tmpFilePath ),
                  _LOG_PATH_LEN );
      }
   }
   else
   {
      tmpFile = stdout;
      fprintf( stderr,
               "\r\n\r\nTeam271Logger: Can not Create Folder: %s\r\n",
               argDir );
   }

   return tmpFile;
}

void Team271Logger::CloseFile( FILE* argLogFile ) const
{
   if( argLogFile != nullptr )
   {
      fflush( argLogFile );
      fclose( argLogFile );
      argLogFile = nullptr;
   }
}

const string Team271Logger::_logLevelToString( const LogLevel_t argLevel ) const
{
   string tmpLogString = "UNKNOWN";

   if( argLevel == LL_DEBUG )
   {
      tmpLogString = "DEBUG";
   }
   else if( argLevel == LL_TRACE )
   {
      tmpLogString = "TRACE";
   }
   else if( argLevel == LL_NOTICE )
   {
      tmpLogString = "NOTICE";
   }
   else if( argLevel == LL_WARNING )
   {
      tmpLogString = "WARN";
   }
   else if( argLevel == LL_ERROR )
   {
      tmpLogString = "ERROR";
   }

   return tmpLogString;
}

int Team271Logger::_premakestr( char* const argBuffer,
                                const LogLevel_t argLevel ) const
{
   time_t now;
   now = time( &now );

   struct tm vtm;
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
   localtime_r( &now,
                &vtm );
#else
   localtime_s( &vtm,
         &now);
#endif

   return snprintf( argBuffer,
                    _LOG_BUFFSIZE,
                    "%s: %02d-%02d %02d:%02d:%02d ",
                    _logLevelToString( argLevel ).c_str(),
                    vtm.tm_mon + 1,
                    vtm.tm_mday,
                    vtm.tm_hour,
                    vtm.tm_min,
                    vtm.tm_sec );
}

#endif
