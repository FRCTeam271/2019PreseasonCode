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

#ifndef TEAM271_LOGGER_H_
#define TEAM271_LOGGER_H_

#define T271_FUNC_NAME   __FUNCTION__

/* buffer size*/
//#define _LOG_BUFFSIZE  (32U*32U*4U)
#define _LOG_BUFFSIZE  (256U)

/* log */
#define _LOG_PATH_LEN  (250U)

typedef enum LOGLEVEL
{
   LL_NONE = 0,
   LL_DEBUG = ( 1 << 0 ),
   LL_TRACE = ( 1 << 1 ),
   LL_NOTICE = ( 1 << 2 ),
   LL_WARNING = ( 1 << 3 ),
   LL_ERROR = ( 1 << 4 ),

   LL_ALL = LL_DEBUG | LL_TRACE | LL_NOTICE | LL_WARNING | LL_ERROR,
} LogLevel_t;

/*
 * Trace
 */
#if !defined(T271_DASHBOARD) && !defined(T271_SIM)
#if defined( T271_LOGGER_ENABLE_ERROR )
#define LOG_ERROR(log_fmt, log_arg...) \
    do{ \
       Team271Logger::getInstance().LogTrace(LL_ERROR,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_ERROR(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_WARN )
#define LOG_WARN(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogTrace(LL_WARNING,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_WARN(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_NOTICE )
#define LOG_NOTICE(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogTrace(LL_NOTICE,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_NOTICE(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_TRACE )
#define LOG_TRACE(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogTrace(LL_TRACE,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_TRACE(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_DEBUG )
#define LOG_DEBUG(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogTrace(LL_DEBUG,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_DEBUG(log_fmt, log_arg...)
#endif

/*
 * Match
 */
#if defined( T271_LOGGER_ENABLE_ERROR )
#define LOG_MATCH_ERROR(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch(LL_ERROR,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH_ERROR(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_WARN )
#define LOG_MATCH_WARN(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch(LL_WARNING,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH_WARN(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_NOTICE )
#define LOG_MATCH_NOTICE(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch(LL_NOTICE,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH_NOTICE(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_TRACE )
#define LOG_MATCH_TRACE(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch(LL_TRACE,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH_TRACE(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE_DEBUG )
#define LOG_MATCH_DEBUG(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch(LL_DEBUG,   "[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH_DEBUG(log_fmt, log_arg...)
#endif

#if defined( T271_LOGGER_ENABLE )
#define LOG_MATCH2(log_fmt, log_arg...) \
    do{ \
      Team271Logger::getInstance().LogMatch2("[%s:%d][%s] " log_fmt "\r\n", \
                __FILE__, __LINE__, T271_FUNC_NAME, ##log_arg); \
    } while (0)
#else
#define LOG_MATCH2(log_fmt, log_arg...)
#endif
#else
#define LOG_ERROR(log_fmt, log_arg, ...)
#define LOG_WARN(log_fmt, log_arg, ...)
#define LOG_NOTICE(log_fmt, log_arg, ...)
#define LOG_TRACE(log_fmt, log_arg, ...)
#define LOG_DEBUG(log_fmt, log_arg, ...)

#define LOG_MATCH_ERROR(log_fmt, log_arg, ...)
#define LOG_MATCH_WARN(log_fmt, log_arg, ...)
#define LOG_MATCH_NOTICE(log_fmt, log_arg, ...)
#define LOG_MATCH_TRACE(log_fmt, log_arg, ...)
#define LOG_MATCH_DEBUG(log_fmt, log_arg, ...)
#define LOG_MATCH2(log_fmt, log_arg, ...)
#endif

#if defined(T271_LOGGER_ENABLE)
class Team271Logger
{
protected:
   /*
    *
    * Variables
    *
    */
   LogLevel_t EnabledLevels_ = LL_NONE;
   LogLevel_t EnabledLevelsPrint_ = LL_NONE;

   char _TraceFilePath[_LOG_PATH_LEN] =
   {  '\0'};
   FILE* _TraceFile = nullptr;
   std::mutex _TraceMutex;

   char _MatchFilePath[_LOG_PATH_LEN] =
   {  '\0'};
   FILE* _MatchFile = nullptr;
   std::mutex _MatchMutex;

   uint8_t LoggedMatchHeader_ = 0;

public:
   Team271Logger( const LogLevel_t argEnabledLogLevels,
         const LogLevel_t argEnabledLogLevelsPrint );

   virtual ~Team271Logger( void );

   static Team271Logger& getInstance( void );

   Team271Logger( Team271Logger const& ) = delete;
   void operator=( Team271Logger const& ) = delete;

   /*
    *
    * Autonomous
    *
    */
   void AutonomousInit( void );

   /*
    *
    * Logger Functions
    *
    */
   void LogMatchHeader( void );

   void LogRobotStats( void );

   string CreateMatchFilePath( void ) const;

   void LogTrace( const LogLevel_t argLevel,
         const char* argLogFmt,
         ... );

   void LogTrace2( const char* argLogFmt,
         ... );

   void LogMatch( const LogLevel_t argLevel,
         const char* argLogFmt,
         ... );

   void LogMatch2( const char* argLogFmt,
         ... );

   void FlushFiles( void );

private:
   FILE* CreateFile( const char* const argDir,
         const char* const argFileName,
         char* argFilePath ) const;

   void CloseFile( FILE* argLogFile ) const;

   const string _logLevelToString( const LogLevel_t argLevel ) const;

   int _premakestr( char* const argBuffer,
         const LogLevel_t argLevel ) const;
};
#endif

#endif /* TEAM271_LOGGER_H_ */
