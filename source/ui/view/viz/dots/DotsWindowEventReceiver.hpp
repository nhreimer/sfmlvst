#ifndef SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP

#include <unordered_map>
#include <mutex>

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmessage.h"

#include "source/ui/view/receivers/WindowEventReceiver.hpp"

#include "source/ui/view/viz/dots/DotsData_t.hpp"
#include "source/ui/view/viz/dots/TimedDot_t.hpp"

#include "source/log/PolyLogger.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>

namespace rj
{

  class DotsWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit DotsWindowEventReceiver( DotsData_t & data )
      : m_data( data )
    {}

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {}

    void draw( sf::RenderWindow &window ) override
    {}

    /***
     * WARNING: occurs on different thread than the UI thread
     * @param event
     */
    void receiveVstEvent( const Steinberg::Vst::Event &event ) override
    {
//      POLY_INFO( "ID: {}. Pitch: {}. Length: {}. Velocity: {}. Tuning: {}",
//           event.noteOn.noteId,
//           event.noteOn.pitch,
//           event.noteOn.length,
//           event.noteOn.velocity,
//           event.noteOn.tuning );
    }

    /***
     * WARNING: occurs on different thread than the UI thread
     * @param ptrMsg
     */
    void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) override
    {
      if ( Steinberg::FIDStringsEqual( ptrMsg->getMessageID(), "midiEvent" ) )
      {
        const void * ptrData = nullptr;
        Steinberg::uint32 msgSize = 0;
        ptrMsg->getAttributes()->getBinary( "midiOn", ptrData, msgSize );

        if ( msgSize > 0 )
        {
          auto event = *( ( Steinberg::Vst::Event * )ptrData );
          receiveVstEvent( event );
        }
        else
          POLY_WARN( "received empty message" );
      }
    }

  private:

    DotsData_t &m_data;

    std::unordered_map< int16_t, TimedDot_t > m_midiDots;
    std::mutex m_mutex;

  };

}

#endif //SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP
