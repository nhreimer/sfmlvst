#ifndef SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP

#include <unordered_map>
#include <mutex>

#include <SFML/Graphics/View.hpp>

#include "source/ui/view/receivers/WindowEventReceiver.hpp"

#include "source/ui/view/viz/dots/DotsData_t.hpp"
#include "source/ui/view/viz/dots/TimedDot_t.hpp"

namespace rj
{

  class DotsWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit DotsWindowEventReceiver( DotsData_t & data );

    void onInit( sf::RenderWindow &window ) override;
    void onShutdown( sf::RenderWindow &window ) override;

    void update( sf::RenderWindow &window, const sf::Time &time ) override;

    void draw( sf::RenderWindow &window ) override;

  protected:

    void processVstMidiOnEvent( const Steinberg::Vst::Event & event ) override;
    void processVstMidiOffEvent( const Steinberg::Vst::Event & event ) override;

  private:

    // used to determine when the system is flagged for shutting down
    // VST events will no longer be processed
    std::atomic_bool m_isShuttingDown { false };

    DotsData_t &m_data;

    // maintains the active dots. dots marked as isRemovable will be purged.
    // it is a resource shared by two threads, so it is protected by a mutex
    std::unordered_map< int16_t, TimedDot_t > m_midiDots;
    std::mutex m_mutex;

  };

}

#endif //SFMLDOTS_DOTSWINDOWEVENTRECEIVER_HPP
