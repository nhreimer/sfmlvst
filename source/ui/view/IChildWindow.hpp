#ifndef POLYEMU_ICHILDWINDOW_HPP
#define POLYEMU_ICHILDWINDOW_HPP

namespace rj
{
  /***
   * Interface for a child window
   */
  struct IChildWindow
  {
    /***
     * Starts the event loop
     * @return true if successful, otherwise false
     */
    virtual bool start() = 0;

    /***
     * sets the size of the window
     * @param width
     * @param height
     */
    virtual void setSize( int32_t width, int32_t height ) = 0;
    virtual void shutdown() = 0;

    /***
     * gets the size of the window
     * @return
     */
    [[nodiscard]]
    virtual sf::Vector2u getSize() const = 0;

    virtual ~IChildWindow() = default;
  };
}

#endif//POLYEMU_ICHILDWINDOW_HPP
