#ifndef POLYEMU_CHILDWINDOWFACTORY_HPP
#define POLYEMU_CHILDWINDOWFACTORY_HPP

#ifdef WIN32
#include <windows.h>
#include "win32/Win32SfmlWindow.hpp"
typedef HWND NATIVE_HANDLE;
#endif

#include <memory>

#include <SFML/Window/ContextSettings.hpp>

#include "IChildWindow.hpp"
#include "source/ui/view/receivers/IWindowEventReceiver.hpp"


namespace rj
{

  struct ChildWindowFactory
  {
    static std::unique_ptr< IChildWindow > createChildWindow(
      NATIVE_HANDLE parentHandle,
      IWindowEventReceiver & eventReceiver,
      sf::ContextSettings && contextSettings )
    {
#ifdef WIN32
      return std::make_unique< Win32SfmlWindow >(
        parentHandle,
        eventReceiver,
        std::forward< sf::ContextSettings >( contextSettings ) );
#endif
    }
  };
}

#endif//POLYEMU_CHILDWINDOWFACTORY_HPP
