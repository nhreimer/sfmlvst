#ifndef SFMLDOTS_IMGUIMENUEVENTRECEIVER_HPP
#define SFMLDOTS_IMGUIMENUEVENTRECEIVER_HPP

#include "source/utils/FpsMeasurement.hpp"

namespace rj
{
  class ImGuiMenuEventReceiver : public IWindowEventReceiver
  {
  public:
    void onInit( sf::RenderWindow &window ) override
    {
      ImGui::SFML::Init( window );
      m_fps.reset();
    }

    void onShutdown( sf::RenderWindow &window ) override
    {
      ImGui::SFML::Shutdown( window );
    }

    void processEvent( sf::RenderWindow &window, const sf::Event &event ) override
    {
      ImGui::SFML::ProcessEvent( window, event );
    }

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {
      ImGui::SFML::Update( window, time );
      m_fps.addFrame();
    }

    void draw( sf::RenderWindow &window ) override
    {
      drawMenu();
      ImGui::SFML::Render( window );
    }

    void beginEventProcessing() override {}
    void endEventProcessing() override {}

    void receiveVstEvent( const Steinberg::Vst::Event &event ) override {}
    void receiveVstEvent( Steinberg::Vst::IMessage *ptrMsg ) override {}

    bool sendVstEvent( const Steinberg::Vst::Event &event, const std::string &eventTag ) override
    {
      return false;
    }

    void onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}
    void onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override {}

  protected:

    virtual void drawMenu() = 0;
    uint32_t getFPS() { return m_fps.getFPS(); }
    double getFPSDelta() const { return m_fps.getDelta(); }
    void resetFPS() { m_fps.reset(); }

  private:

    FpsMeasurement m_fps;

  };
}

#endif //SFMLDOTS_IMGUIMENUEVENTRECEIVER_HPP
