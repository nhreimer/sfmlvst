#ifndef SFMLDOTS_RECEIVERDISTRIBUTOR_HPP
#define SFMLDOTS_RECEIVERDISTRIBUTOR_HPP

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"

#include "source/ui/view/viz/dots/DotsWindowEventReceiver.hpp"
#include "source/ui/view/viz/dots/DotsMenuWindowEventReceiver.hpp"
#include "source/ui/view/viz/dots/DotsMenuWindowEventReceiver.hpp"
#include "source/ui/view/viz/dots/DotsData_t.hpp"

#include <SFML/Graphics/RenderWindow.hpp>

namespace rj
{
  /***
   * Distributes events out to different receivers and manages the clear & display
   * SFML calls
   */
  class ReceiverDistributor : public IWindowEventReceiver
  {
  public:

    // TODO: this should be the default and then press a button to switch visualization engine
    // TODO: and its menu
    ReceiverDistributor()
      : m_ptrVisualizer( std::make_unique< DotsWindowEventReceiver >( m_data ) ),
        m_ptrVisMenu( std::make_unique< DotsMenuWindowEventReceiver >( m_data ) )
    {}

    void onInit( sf::RenderWindow & window ) override
    {
      m_ptrVisMenu->onInit( window );
      m_ptrVisualizer->onInit( window );
    }

    void onShutdown( sf::RenderWindow & window ) override
    {
      POLY_DEBUG( "window shutdown event received." );
      m_ptrVisMenu->onShutdown( window );
      m_ptrVisualizer->onShutdown( window );
    }

    void beginEventProcessing() override
    {
      m_ptrVisMenu->beginEventProcessing();
      m_ptrVisualizer->beginEventProcessing();
    }

    void endEventProcessing() override
    {
      m_ptrVisMenu->endEventProcessing();
      m_ptrVisualizer->endEventProcessing();
    }

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override
    {
      m_ptrVisMenu->processEvent( window, event );
      m_ptrVisualizer->processEvent( window, event );
    }

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {
      m_ptrVisMenu->update( window, time );
      m_ptrVisualizer->update( window, time );
    }

    void draw( sf::RenderWindow &window ) override
    {
      window.clear( sf::Color( 32, 32, 32 ) );
      m_ptrVisMenu->draw( window );
      m_ptrVisualizer->draw( window );
      window.display();
    }

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override
    {
      m_ptrVisualizer->receiveVstEvent( event );
    }

    void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) override
    {
      m_ptrVisualizer->receiveVstEvent( ptrMsg );
    }

    bool sendVstEvent( const Steinberg::Vst::Event &event,
                       const std::string & eventTag ) override
    {
      return m_ptrVisualizer->sendVstEvent( event, eventTag );
    }

    void onKeyDown( Steinberg::char16 key,
                    Steinberg::int16 keyCode,
                    Steinberg::int16 modifiers ) override
    {
      m_ptrVisualizer->onKeyDown( key, keyCode, modifiers );
    }

    void onKeyUp( Steinberg::char16 key,
                  Steinberg::int16 keyCode,
                  Steinberg::int16 modifiers ) override
    {
      m_ptrVisualizer->onKeyUp( key, keyCode, modifiers );
    }

  private:

    DotsData_t m_data;
    std::unique_ptr< IWindowEventReceiver > m_ptrVisualizer;
    std::unique_ptr< IWindowEventReceiver > m_ptrVisMenu;

  };
}

#endif //SFMLDOTS_RECEIVERDISTRIBUTOR_HPP
