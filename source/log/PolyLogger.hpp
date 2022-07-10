#ifndef POLYSECT_POLYLOGGER_HPP
#define POLYSECT_POLYLOGGER_HPP

#ifndef STANDALONE
//#define STANDALONE
#endif

#ifndef DEBUG_LOG
#define DEBUG_LOG
#endif

#if !defined(__PRETTY_FUNCTION__) && !defined(__GNUC__)
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
//#define SPDLOG_FUNCTION __PRETTY_FUNCTION__

#include <spdlog/spdlog.h>
#include <spdlog/logger.h>

#ifndef PLOG_LOGGER
#define PLOG_LOGGER

#ifdef DEBUG_LOG

#define POLY_TRACE( ... ) SPDLOG_LOGGER_TRACE( rj::SLog::log, __VA_ARGS__ )
#define POLY_DEBUG( ... ) SPDLOG_LOGGER_DEBUG( rj::SLog::log, __VA_ARGS__ )
#define POLY_INFO( ... ) SPDLOG_LOGGER_INFO( rj::SLog::log, __VA_ARGS__ )
#define POLY_WARN( ... ) SPDLOG_LOGGER_WARN( rj::SLog::log, __VA_ARGS__ )
#define POLY_ERROR( ... ) SPDLOG_LOGGER_ERROR( rj::SLog::log, __VA_ARGS__ )
#define POLY_CRITICAL( ... ) SPDLOG_LOGGER_CRITICAL( rj::SLog::log, __VA_ARGS__ )

#else

#define POLY_TRACE( ... )
#define POLY_DEBUG( ... )
#define POLY_INFO( ... )
#define POLY_WARN( ... )
#define POLY_ERROR( ... )
#define POLY_CRITICAL( ... )

#endif

#endif

namespace rj
{
  struct SLog
  {
    inline static std::shared_ptr< spdlog::logger > log;
    static bool initializeLogger();
  };

}
#endif // POLYSECT_POLYLOGGER_HPP