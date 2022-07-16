#ifndef POLYEMU_WIN32SFMLWINDOW_HPP
#define POLYEMU_WIN32SFMLWINDOW_HPP

#include <unordered_map>
#include <string>

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Win32Helper.hpp"
#include "Win32WndCtx_t.hpp"
#include "../IChildWindow.hpp"

namespace rj
{

  // forward declarations
  struct IWindowEventReceiver;

  /***
   * This class provides and maintains some Win32 components that are required
   * for driving the SFML Window.
   */
  class Win32SfmlWindow : public IChildWindow
  {

  public:

    Win32SfmlWindow( HWND parentHwnd,
                     IWindowEventReceiver & driver,
                     sf::ContextSettings && settings );

    ~Win32SfmlWindow() override;

    /***
     * shutdowns the window, destroying it
     */
    void shutdown() final;

    /***
     * Starts the event loop that begins making calls to the IWindowEventReceiver
     * @return true if the event loop starts, otherwise false
     */
    bool start() override;

    /***
     * Sets the window width and height
     * @param width
     * @param height
     */
    void setSize( int32_t width, int32_t height ) override;

    /***
     * Gets the window size
     * @return
     */
    sf::Vector2u getSize() const override;

    /***
     * An expected frame rate based on windows' timer, which only allows for
     * 16ms rates
     * @return the expected lowest frame rate possible on windows
     */
    float getFrameRate() const override;

  private:

    /***
     * If the Win32 child HWND is successfully created then this initializes
     * the SFML RenderWindow
     */
    void initializeRenderWindow();

    /***
     * SFML window loop that calls the appropriate driver methods. This method
     * gets called whenever the processTimerExpiry gets called.
     */
    void processWindowLoop();

    /***
     * Win32 class registration
     * @return true if successful, otherwise false
     * @see Win32 RegisterClass
     */
    bool registerWin32WndClass();

    /***
     * Creates the Win32 child class
     * @return true if successful, otherwise false
     * @see Win32 CreateWindow
     */
    bool createWin32Window();

    static sf::Vector2u getWin32WindowSize( HWND hwnd );

    /***
     * Receives the callback whenever our Windows timer expires. This timer makes a callback
     * on the same thread that creates it, so we don't need to worry about context switching
     * or using thread-safe constructs.
     * @param hwnd
     * @param wmTimerMsg
     * @param timerId
     * @param currentSysTime
     */
    static void WINAPI processTimerExpiry( HWND hwnd,
                                           UINT wmTimerMsg,
                                           UINT_PTR timerId,
                                           DWORD currentSysTime );

    /***
     * The event processor for the child HWND. It is not in use but is required
     * to exist.
     * @param hwnd
     * @param msg
     * @param wParam
     * @param lParam
     * @return
     */
    static LRESULT WINAPI processWndEvent( HWND hwnd,
                                           UINT msg,
                                           WPARAM wParam,
                                           LPARAM lParam );

  private:

    Win32WndCtx_t m_wndCtx;                   // holds all the Win32 info about the child window
    IWindowEventReceiver & m_eventReceiver;   // the target
    sf::RenderWindow m_childRenderWindow;     // the child window that is now owned by SFML
    sf::ContextSettings m_sfmlCtx;            // SFML's context settings

    sf::Clock m_timer;                        // SFML timer for providing time between updates

    // required for timer callbacks across multiple instances
    // each child HWND is mapped to Win32SfmlWindow instance
    inline static std::unordered_map< HWND, Win32SfmlWindow * > sm_wndMap {};

    // usually starts at 5 and goes up
    inline static uint32_t sm_timerIdCounter { 5 };
  };
}

#endif//POLYEMU_WIN32SFMLWINDOW_HPP
