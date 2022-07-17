#ifndef SFMLDOTS_VISUALIZATIONEVENTDISTRIBUTOR_HPP
#define SFMLDOTS_VISUALIZATIONEVENTDISTRIBUTOR_HPP

#include <type_traits>

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"

namespace rj
{

  /***
   * Distributes events to the visualization engine and the
   * menu associated visualization engine
   * @tparam TDATA the data shared between the menu and engine
   * @tparam TVIZ IWindowEventReceiver that manages the visualizations
   * @tparam TVIZMENU IWindowEventReceiver that manages the visualization menu
   */
  template < typename TDATA,
             typename TVIZ,
             typename TVIZMENU >
  class VisualizationEventDistributor : public IWindowEventReceiver
  {
  public:

    static_assert( std::is_base_of< IWindowEventReceiver, TVIZ >::value );
    static_assert( std::is_base_of< IWindowEventReceiver, TVIZMENU >::value );

    VisualizationEventDistributor()
      : m_eventReceiver( m_data ),
        m_menuEventReceiver( m_data )
    {}


    void onInit( sf::RenderWindow &window ) override
    {
      m_menuEventReceiver.onInit( window );
      m_eventReceiver.onInit( window );
      m_isInit = true;
    }

    void onShutdown( sf::RenderWindow &window ) override
    {
      m_menuEventReceiver.onShutdown( window );
      m_eventReceiver.onShutdown( window );
    }

    void beginEventProcessing() override
    {
      m_menuEventReceiver.beginEventProcessing();
      m_eventReceiver.beginEventProcessing();
    }

    void endEventProcessing() override
    {
      m_menuEventReceiver.endEventProcessing();
      m_eventReceiver.endEventProcessing();
    }

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override
    {
      m_menuEventReceiver.processEvent( window, event );
      m_eventReceiver.processEvent( window, event );
    }

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {
      if ( !m_isInit )
        onInit( window );

      m_menuEventReceiver.update( window, time );
      m_eventReceiver.update( window, time );
    }

    void draw( sf::RenderWindow &window ) override
    {
      m_menuEventReceiver.draw( window );
      m_eventReceiver.draw( window );
    }

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override
    {
      m_menuEventReceiver.receiveVstEvent( event );
      m_eventReceiver.receiveVstEvent( event );
    }

    void receiveVstEvent( Steinberg::Vst::IMessage * ptrMsg ) override
    {
      m_menuEventReceiver.receiveVstEvent( ptrMsg );
      m_eventReceiver.receiveVstEvent( ptrMsg );
    }

    bool sendVstEvent( const Steinberg::Vst::Event &event, const std::string &eventTag ) override
    {
      return m_menuEventReceiver.sendVstEvent( event, eventTag ) &&
             m_eventReceiver.sendVstEvent( event, eventTag );
    }

    void onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override
    {
      m_menuEventReceiver.onKeyDown( key, keyCode, modifiers );
      m_eventReceiver.onKeyDown( key, keyCode, modifiers );
    }

    void onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override
    {
      m_menuEventReceiver.onKeyUp( key, keyCode, modifiers );
      m_eventReceiver.onKeyUp( key, keyCode, modifiers );
    }

  private:

    TDATA m_data;
    TVIZ m_eventReceiver;
    TVIZMENU m_menuEventReceiver;

    bool m_isInit { false };

  };

}

#endif //SFMLDOTS_VISUALIZATIONEVENTDISTRIBUTOR_HPP
