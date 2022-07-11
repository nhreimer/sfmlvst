#ifndef SFMLDOTS_SQUIGGLESWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_SQUIGGLESWINDOWEVENTRECEIVER_HPP

namespace rj
{
  class SquigglesWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit SquigglesWindowEventReceiver( SquigglesData_t & data )
      : m_data( data )
    {}

  private:

    SquigglesData_t &m_data;

  };
}

#endif //SFMLDOTS_SQUIGGLESWINDOWEVENTRECEIVER_HPP
