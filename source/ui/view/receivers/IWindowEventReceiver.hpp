#ifndef POLYEMU_IWINDOWDRIVER_HPP
#define POLYEMU_IWINDOWDRIVER_HPP

#include <string>

#include "pluginterfaces/base/ftypes.h"

// FORWARD DECLARATIONS
namespace Steinberg::Vst
{
  struct Event;
  class IMessage;
}

namespace sf
{
  class RenderWindow;
  class Event;
  class Time;
}

namespace rj
{
  struct IWindowEventReceiver
  {
    virtual ~IWindowEventReceiver() = default;

    ////////////////////////////////////////////////////////////////////////////
    ///                             STARTUP/TEARDOWN
    ////////////////////////////////////////////////////////////////////////////

    virtual void onInit( sf::RenderWindow &window ) = 0;
    virtual void onShutdown( sf::RenderWindow &window ) = 0;

    ////////////////////////////////////////////////////////////////////////////
    ///                             PRE/POST-LOOP PROCESSING
    ////////////////////////////////////////////////////////////////////////////

    virtual void beginEventProcessing() = 0;
    virtual void endEventProcessing() = 0;

    ////////////////////////////////////////////////////////////////////////////
    ///                             WINDOW LOOP
    ////////////////////////////////////////////////////////////////////////////

    virtual void processEvent( sf::RenderWindow&, const sf::Event& ) = 0;
    virtual void update( sf::RenderWindow&, const sf::Time& ) = 0;
    virtual void draw( sf::RenderWindow& ) = 0;

    ////////////////////////////////////////////////////////////////////////////
    ///                             VST EVENTS
    ////////////////////////////////////////////////////////////////////////////

    /***
     * WARNING! this gets called from a different thread than the UI thread.
     * This is a convenience for method for simple messages where the controller
     * or EventReceiver extracts the Event from the IMessage container
     * @param event VST Event Message
     */
    virtual void receiveVstEvent( const Steinberg::Vst::Event &event ) = 0;

    /***
     * WARNING! this gets called from a different thread than the UI thread.
     * @param event pointer to Vst::IMessage, which is the envelope for Vst::Event
     */
    virtual void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) = 0;

    /***
     * sends a VST Event Message to the processor
     * @param event
     * @param eventTag name chosen for the event (should be consistent)
     * @return true if successful, otherwise false
     */
    virtual bool sendVstEvent( const Steinberg::Vst::Event &event,
                               const std::string & eventTag ) = 0;

    virtual void onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) = 0;
    virtual void onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) = 0;
  };
}

#endif//POLYEMU_IWINDOWDRIVER_HPP
