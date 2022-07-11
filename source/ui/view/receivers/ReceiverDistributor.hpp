#ifndef SFMLDOTS_RECEIVERDISTRIBUTOR_HPP
#define SFMLDOTS_RECEIVERDISTRIBUTOR_HPP

#include <SFML/Graphics/RenderWindow.hpp>

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"
#include "source/ui/view/receivers/EventReceiverFactory.hpp"

namespace rj
{
  /***
   * The main distributor that switches between the visualizations engines.
   */
  class ReceiverDistributor : public IWindowEventReceiver
  {
  public:

    ReceiverDistributor()
      : m_ptrVisEventDistributor(
          EventReceiverFactory::create( EventReceiverFactory::DEFAULT ) )
    {}

    void onInit( sf::RenderWindow & window ) override
    {
      m_ptrVisEventDistributor->onInit( window );
    }

    void onShutdown( sf::RenderWindow & window ) override
    {
      POLY_DEBUG( "window shutdown event received." );
      m_ptrVisEventDistributor->onShutdown( window );
    }

    void beginEventProcessing() override
    {
      m_ptrVisEventDistributor->beginEventProcessing();
    }

    void endEventProcessing() override
    {
      m_ptrVisEventDistributor->endEventProcessing();
    }

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override
    {
      m_ptrVisEventDistributor->processEvent( window, event );
    }

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {
      m_ptrVisEventDistributor->update( window, time );
    }

    void draw( sf::RenderWindow &window ) override
    {
      window.clear( sf::Color( 32, 32, 32 ) );
      m_ptrVisEventDistributor->draw( window );
      window.display();
    }

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override
    {
      m_ptrVisEventDistributor->receiveVstEvent( event );
    }

    void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) override
    {
      m_ptrVisEventDistributor->receiveVstEvent( ptrMsg );
    }

    bool sendVstEvent( const Steinberg::Vst::Event &event,
                       const std::string & eventTag ) override
    {
      return m_ptrVisEventDistributor->sendVstEvent( event, eventTag );
    }

    void onKeyDown( Steinberg::char16 key,
                    Steinberg::int16 keyCode,
                    Steinberg::int16 modifiers ) override
    {
      m_ptrVisEventDistributor->onKeyDown( key, keyCode, modifiers );
    }

    void onKeyUp( Steinberg::char16 key,
                  Steinberg::int16 keyCode,
                  Steinberg::int16 modifiers ) override
    {
      m_ptrVisEventDistributor->onKeyUp( key, keyCode, modifiers );

      switch ( key )
      {
        case 49: // DOTS: 1 key
          POLY_DEBUG( "switching to DOTS visualizations" );
          m_ptrVisEventDistributor.reset();
          m_ptrVisEventDistributor = EventReceiverFactory::create( EventReceiverFactory::DOTS );
          break;

        case 50: // SQUIGGLES: 2 key
          POLY_DEBUG( "switch to SQUIGGLES visualizations" );
          m_ptrVisEventDistributor.reset();
          m_ptrVisEventDistributor = EventReceiverFactory::create( EventReceiverFactory::SQUIGGLES );
          break;

        default:
          break;
      }
//      POLY_DEBUG( "KEY: {}. CODE: {}", key, keyCode );
    }

  private:

    std::unique_ptr< IWindowEventReceiver > m_ptrVisEventDistributor;

  };
}

#endif //SFMLDOTS_RECEIVERDISTRIBUTOR_HPP
