#ifndef SFMLDOTS_WINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_WINDOWEVENTRECEIVER_HPP

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"

namespace rj
{

  /***
   * Convenience class that has empty initializations for ease of
   * implementing only the parts of the interface that are necessary in children
   */
  class WindowEventReceiver : public IWindowEventReceiver
  {
  public:
    ~WindowEventReceiver() override = default;

    void onInit( sf::RenderWindow &window ) override {}
    void onShutdown( sf::RenderWindow &window ) override {}

    void beginEventProcessing() override {}
    void endEventProcessing() override {}

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override {}
    void update( sf::RenderWindow &window, const sf::Time &time ) override {}
    void draw( sf::RenderWindow &window ) override {}

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override {}
    void receiveVstEvent( Steinberg::Vst::IMessage *ptrMsg ) override {}

    bool sendVstEvent( const Steinberg::Vst::Event &event, const std::string &eventTag ) override
    {
      return false;
    }

    void onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}
    void onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}
  };

}

#endif //SFMLDOTS_WINDOWEVENTRECEIVER_HPP
