#include "PolyLogger.hpp"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <fstream>
#include <iostream>

bool rj::SLog::initializeLogger()
{
  bool result = true;

  if ( log == nullptr )
  {
    try
    {
#ifdef STANDALONE
      log = spdlog::stdout_color_mt( "clog" );
#else
      log = spdlog::basic_logger_mt(
      "clog",
      R"(D:\projects\VST_SDK\my_plugins\SfmlDots\logs\SfmlDots.log)",
      true );
#endif
      log->set_level( spdlog::level::debug );
      log->flush_on( spdlog::level::debug );
      spdlog::set_pattern("[%L][%t][%H:%M:%S.%e][%!:%#] %v");
    }
    catch ( const spdlog::spdlog_ex& ex )
    {
      result = false;
      std::cerr << ex.what() << std::endl;
//      std::ofstream err;
//      err.open( R"(error.txt)" );
//      err.rdbuf()->pubsetbuf( nullptr, 0 ); // prevent from buffering
//      err << "FAILED TO OPEN LOG FILE: " << ex.what() << "\n";
//      err.flush();
    }
  }

  return result;
}
