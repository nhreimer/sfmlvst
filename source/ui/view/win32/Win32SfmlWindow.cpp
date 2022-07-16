#include "Win32SfmlWindow.hpp"
#include <cassert>

#include <SFML/Window/Event.hpp>

#include "source/log/PolyLogger.hpp"
#include "source/ui/view/receivers/IWindowEventReceiver.hpp"

static const float WIN32_FPS = 1000.f / 16.f;

////////////////////////////////////////////////////////////////////////////
///                             PUBLIC
////////////////////////////////////////////////////////////////////////////
/* PUBLIC CTOR */
rj::Win32SfmlWindow::Win32SfmlWindow( HWND parentHwnd,
                                      rj::IWindowEventReceiver & eventReceiver,
                                      sf::ContextSettings && settings )
    : m_eventReceiver( eventReceiver ),
      m_sfmlCtx( settings )
{
  assert( ::IsWindow( parentHwnd ) );
  m_wndCtx.parentHwnd = parentHwnd;
  m_wndCtx.classname = createUniqueName();
  assert( !m_wndCtx.classname.empty() );

  m_wndCtx.hInstance = reinterpret_cast< HINSTANCE >( &__ImageBase );

  if ( registerWin32WndClass() && createWin32Window() )
  {
    sm_wndMap.insert( { m_wndCtx.childHwnd, this } );
    initializeRenderWindow();
  }
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC DTOR */
rj::Win32SfmlWindow::~Win32SfmlWindow()
{
  POLY_DEBUG( "destructor called. destroying window." );
  shutdown();
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::Win32SfmlWindow::shutdown()
{
  // notify the event receiver that the window will close
  m_eventReceiver.onShutdown( m_childRenderWindow );

  if ( m_wndCtx.childHwnd != nullptr )
  {
    // stop the callbacks
    if ( m_wndCtx.timerResult != 0 )
    {
      ::KillTimer( m_wndCtx.childHwnd, m_wndCtx.timerResult );
      m_wndCtx.timerResult = 0;
    }

    // SFML takes care of the following whenever it closes the window
    //        ::UnregisterClass( m_wndData.classname.data(), m_wndData.hInstance );
    //        ::DestroyWindow( m_wndData.childHwnd );

    auto it = sm_wndMap.find( m_wndCtx.childHwnd );
    if ( it != sm_wndMap.end() )
      sm_wndMap.erase( it );
    else // something has gone wrong!
      POLY_ERROR( "unable to properly shut down child HWND because it is not mapped!" );

    // shutdown can get called prior to the destructor, so mark this as invalid
    m_wndCtx.childHwnd = nullptr;
  }
  else
    POLY_DEBUG( "child window has already been shut down" );
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
bool rj::Win32SfmlWindow::start()
{
  bool result = true;

  if ( m_wndCtx.childHwnd != nullptr )
  {
    if ( m_wndCtx.timerResult == 0 )
    {
      // notify event receiver of initialization prior to timer
      m_eventReceiver.onInit( m_childRenderWindow );

      m_timer.restart();

      m_wndCtx.timerResult = ::SetTimer( m_wndCtx.childHwnd,
                                         sm_timerIdCounter,
                                         USER_TIMER_MINIMUM,
                                         processTimerExpiry );

      result = ( m_wndCtx.timerResult != 0 );
      if ( result )
        ++sm_timerIdCounter;
      else
        POLY_ERROR( "failed to start timer. {}", ::GetLastError() );
    }
    else
      POLY_WARN( "timer already exists and is running" );
  }
  else
    POLY_ERROR( "child window does not exist" );

  return result;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::Win32SfmlWindow::setSize( int32_t width, int32_t height )
{
  m_childRenderWindow.setSize( { static_cast< uint32_t >( width ),
                                 static_cast< uint32_t >( height ) } );
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
sf::Vector2u rj::Win32SfmlWindow::getSize() const
{
  return m_childRenderWindow.getSize();
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
float rj::Win32SfmlWindow::getFrameRate() const
{
  return WIN32_FPS;
}

////////////////////////////////////////////////////////////////////////////
///                             PRIVATE
////////////////////////////////////////////////////////////////////////////
/* PRIVATE */
void rj::Win32SfmlWindow::initializeRenderWindow()
{
  m_childRenderWindow.create( m_wndCtx.childHwnd, m_sfmlCtx );

  if ( !::AllowSetForegroundWindow( ASFW_ANY ) )
    POLY_WARN( "failed to allow foreground. interactivity may be limited." );

  m_childRenderWindow.setActive( true );
  m_childRenderWindow.requestFocus();
  m_childRenderWindow.clear();
  m_childRenderWindow.display();
}

////////////////////////////////////////////////////////////////////////////
/* PRIVATE */
void rj::Win32SfmlWindow::processWindowLoop()
{
  if ( m_childRenderWindow.isOpen() )
  {
    // notify that events will begin
    m_eventReceiver.beginEventProcessing();

    sf::Event event {};
    while ( m_childRenderWindow.pollEvent( event ) )
    {
      if ( event.type == sf::Event::Closed )
      {
        // trigger shutdown and exit.
        shutdown();
        return;
      }

      m_eventReceiver.processEvent( m_childRenderWindow, event );
    }

    // notify that event processing has stopped
    m_eventReceiver.endEventProcessing();

    m_eventReceiver.update( m_childRenderWindow, m_timer.restart() );
    m_eventReceiver.draw( m_childRenderWindow );
  }
}

////////////////////////////////////////////////////////////////////////////
/* PRIVATE */
bool rj::Win32SfmlWindow::registerWin32WndClass()
{
  ::WNDCLASS wndClass =
  {
    CS_GLOBALCLASS | CS_DBLCLKS,
    processWndEvent,
    0,
    0,
    m_wndCtx.hInstance,
    nullptr, nullptr, nullptr, nullptr,
    m_wndCtx.classname.data()
  };

  if ( ::RegisterClass( &wndClass ) == 0 )
  {
    POLY_ERROR( "failed to register window class. {}", ::GetLastError() );
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////
/* PRIVATE */
bool rj::Win32SfmlWindow::createWin32Window()
{
  // https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
  // CW_USEDEFAULT = 0 whenever it's a child, which will completely
  // screwn with SFML's scaling. if it returns zero then the program should
  // not continue!
  auto parentWndSize = getWin32WindowSize( m_wndCtx.parentHwnd );
  if ( parentWndSize.x == 0 || parentWndSize.y == 0 )
    POLY_WARN( "parent wnd size not found. scaling issues may occur" );

  m_wndCtx.childHwnd = ::CreateWindowEx(
    WS_EX_NOINHERITLAYOUT,
    m_wndCtx.classname.data(),
    TEXT( "__innerWindowName" ),
    WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
    CW_USEDEFAULT, CW_USEDEFAULT, ( int32_t )parentWndSize.x, ( int32_t )parentWndSize.y,
    m_wndCtx.parentHwnd,
    nullptr,
    m_wndCtx.hInstance,
    nullptr );

  if ( m_wndCtx.childHwnd == nullptr )
  {
    POLY_ERROR( "failed to create child window. {}", ::GetLastError() );
    return false;
  }

  return true;
}

////////////////////////////////////////////////////////////////////////////
///                           PRIVATE STATIC
////////////////////////////////////////////////////////////////////////////
/* PRIVATE STATIC */
sf::Vector2u rj::Win32SfmlWindow::getWin32WindowSize( HWND hwnd )
{
  RECT frame {};
  if ( ::GetWindowRect( hwnd, &frame ) )
  {
    return { ( uint32_t )( frame.right - frame.left ),
             ( uint32_t )( frame.bottom - frame.top ) };
  }

  POLY_ERROR( "unable to retrieve HWND size. {}", ::GetLastError() );

  // hand back an empty vector
  return sf::Vector2u {};
}

////////////////////////////////////////////////////////////////////////////
/* PRIVATE STATIC */
void rj::Win32SfmlWindow::processTimerExpiry( HWND hwnd,
                                              UINT wmTimerMsg,
                                              UINT_PTR timerId,
                                              DWORD currentSysTime )
{
  auto it = sm_wndMap.find( hwnd );
  if ( it != sm_wndMap.end() )
    it->second->processWindowLoop();
  else
  {
    // this means something has gone terribly wrong. events cannot be processed,
    // so kill the timer.
    POLY_CRITICAL( "timer is running but child HWND not found. stopping timer." );
    ::KillTimer( hwnd, timerId );
  }
}

////////////////////////////////////////////////////////////////////////////
/* PRIVATE STATIC */
LRESULT rj::Win32SfmlWindow::processWndEvent( HWND hwnd,
                                              UINT msg,
                                              WPARAM wParam,
                                              LPARAM lParam )
{
  return ::DefWindowProc( hwnd, msg, wParam, lParam );
}
