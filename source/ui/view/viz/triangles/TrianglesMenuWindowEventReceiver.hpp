#ifndef SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP

#include "source/ui/view/receivers/WindowEventReceiver.hpp"

namespace rj
{
  class TrianglesMenuWindowEventReceiver : public WindowEventReceiver
  {
  public:
    explicit TrianglesMenuWindowEventReceiver( TrianglesData_t & data )
    : m_data( data )
    {}
  private:
    TrianglesData_t & m_data;
  };
}

#endif //SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP
