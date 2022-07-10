#ifndef POLYEMU_WIN32HELPER_HPP
#define POLYEMU_WIN32HELPER_HPP

#include <Windows.h>

#include <string>

#include <SFML/System/Vector2.hpp>
#include <rpc.h>

extern "C"
IMAGE_DOS_HEADER __ImageBase;

namespace rj
{
  inline static const std::string INVALID_UNIQUE_NAME { "" };

  /***
   * Creates a unique name by requesting a Uuid from the OS
   * @return Uuid as a string if successful, otherwise INVALID_UNIQUE_NAME
   * @see INVALID_UNIQUE_NAME
   */
  static std::string createUniqueName()
  {
    UUID uuid;
    RPC_STATUS ret_val = ::UuidCreate( &uuid );

    if ( ret_val == RPC_S_OK )
    {
      char * cszUuid = nullptr;
      ::UuidToStringA( &uuid, ( RPC_CSTR * ) &cszUuid );
      if ( cszUuid != nullptr )
      {
        std::string str( ::_strdup( cszUuid ) );
        ::RpcStringFreeA( ( RPC_CSTR * ) &cszUuid );
        return str;
      }
    }

    return INVALID_UNIQUE_NAME;
  }

  /***
   * Gets the window size from the OS
   * @param hwnd handle to a window
   * @return size of window if successful, otherwise a vector of { 0, 0 }
   */
  static sf::Vector2u getWin32WindowSize( HWND hwnd )
  {
    RECT frame {};
    if ( ::GetWindowRect( hwnd, &frame ) )
    {
      return { ( uint32_t )( frame.right - frame.left ),
               ( uint32_t )( frame.bottom - frame.top ) };
    }

    // hand back an empty vector
    return sf::Vector2u {};
  }
}

#endif//POLYEMU_WIN32HELPER_HPP
