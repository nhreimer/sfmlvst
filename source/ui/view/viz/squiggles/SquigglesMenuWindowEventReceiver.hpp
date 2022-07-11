#ifndef SFMLDOTS_SQUIGGLESMENUWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_SQUIGGLESMENUWINDOWEVENTRECEIVER_HPP

namespace rj
{
  class SquigglesMenuWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit SquigglesMenuWindowEventReceiver( SquigglesData_t & data )
    : m_data( data )
    {}

    void onInit( sf::RenderWindow &window ) override
    {
      ImGui::SFML::Init( window );
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
    }

    void draw( sf::RenderWindow &window ) override
    {
      drawMenu();
      ImGui::SFML::Render( window );
    }

  private:

    void drawMenu()
    {
      ImGui::Begin(
      "Squiggles Menu",
      nullptr,
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove );
      {
        // we are fixed to the top left corner
        ImGui::SetWindowPos( { 0, 0 } );

        ImVec4 color = m_data.startColor;

        if ( ImGui::ColorPicker4( "StartColor##1",
                                  ( float* )&color,
                                  ImGuiColorEditFlags_AlphaBar,
                                  nullptr ) )
        {
          m_data.startColor = color;
        }

        ImGui::Separator();

        int32_t timeoutInMS = m_data.timeoutInMS;
        if ( ImGui::SliderInt( "Timeout##1", &timeoutInMS, 0, 10000 ) )
        {
          m_data.timeoutInMS = timeoutInMS;
        }

        float spread = m_data.spreadMultiplier;
        if ( ImGui::SliderFloat( "Spread##1", &spread, 0.f, 5.f ) )
        {
          m_data.spreadMultiplier = spread;
        }

        float radius = m_data.radius;
        if ( ImGui::SliderFloat( "Radius##1", &radius, 0.f, 100.f ) )
        {
          m_data.radius = radius;
        }
      }
      ImGui::End();
    }

  private:

    SquigglesData_t &m_data;

  };
}

#endif //SFMLDOTS_SQUIGGLESMENUWINDOWEVENTRECEIVER_HPP
