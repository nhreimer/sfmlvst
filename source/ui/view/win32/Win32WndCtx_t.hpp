#ifndef POLYEMU_WIN32WNDCTX_T_HPP
#define POLYEMU_WIN32WNDCTX_T_HPP

namespace rj
{
  /***
   * Contains information related to and required for window creation.
   * This information gets filled out by the Win32WindowFactory and
   * is consumed by the Win32SfmlWindow.
   * @see rj::Win32WindowFactory
   * @see rj::Win32SfmlWindow
   */
  struct Win32WndCtx_t
  {
    std::string classname;              // WNDCLASS classname. must be unique.
    HWND parentHwnd { nullptr };        // Parent HWND of the child HWND
    HWND childHwnd { nullptr };         // HWND to the child
    HINSTANCE hInstance { nullptr };    // Instance handle
    UINT_PTR timerResult { 0 };         // timer id
  };
}

#endif//POLYEMU_WIN32WNDCTX_T_HPP
