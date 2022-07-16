#ifndef SFMLDOTS_WINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_WINDOWEVENTRECEIVER_HPP

#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmessage.h"
#include "pluginterfaces/base/funknown.h"

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"

#include "source/utils/Noncopyable.hpp"
#include "source/log/PolyLogger.hpp"

namespace rj
{

  /***
   * Convenience class that handles some of the things that all the visualization
   * components need
   */
  class WindowEventReceiver : public IWindowEventReceiver,
                              private Noncopyable
  {
  public:
    ~WindowEventReceiver() override = default;

    void onInit( sf::RenderWindow &window ) override
    {
      auto center = getCenter( window.getSize() );
      POLY_DEBUG( "center is: {}, {}", center.x, center.y );
      m_view.setCenter( center );
      window.setView( m_view );
    }

    void onShutdown( sf::RenderWindow &window ) override {}

    void beginEventProcessing() override {}
    void endEventProcessing() override {}

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override {}
    void update( sf::RenderWindow &window, const sf::Time &time ) override {}
    void draw( sf::RenderWindow &window ) override {}

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override {}

    /***
     * Handles converting IMessages and then breaking them down into
     * @param ptrMsg
     */
    void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) override
    {
      if ( Steinberg::FIDStringsEqual( ptrMsg->getMessageID(), "midiEvent" ) )
      {
        const void * ptrData = nullptr;
        Steinberg::uint32 msgSize = 0;

        if ( ptrMsg->getAttributes()->getBinary( "midiOn", ptrData, msgSize ) != Steinberg::kResultFalse &&
             msgSize > 0 )
        {
          auto event = *( ( Steinberg::Vst::Event * )ptrData );
          processVstMidiOnEvent( event );
        }
        else if ( ptrMsg->getAttributes()->getBinary( "midiOff", ptrData, msgSize ) != Steinberg::kResultFalse &&
                  msgSize > 0 )
        {
          auto event = *( ( Steinberg::Vst::Event * )ptrData );
          processVstMidiOffEvent( event );
        }
      }
    }

    bool sendVstEvent( const Steinberg::Vst::Event &event, const std::string &eventTag ) override
    {
      return false;
    }

    void onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}
    void onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}

  protected:

    sf::View & getView() { return m_view; }

    virtual void processVstMidiOnEvent( const Steinberg::Vst::Event & event ) {}
    virtual void processVstMidiOffEvent( const Steinberg::Vst::Event & event ) {}

  private:

    static inline sf::Vector2f getCenter( const sf::Vector2u & size )
    {
      return sf::Vector2f { static_cast< float >( size.x ) / 2.f,
                            static_cast< float >( size.y ) / 2.f };
    }

  private:

    sf::View m_view;
  };

}

#endif //SFMLDOTS_WINDOWEVENTRECEIVER_HPP
