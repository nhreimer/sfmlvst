#ifndef SFMLDOTS_DOTSMENUWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_DOTSMENUWINDOWEVENTRECEIVER_HPP

#include <imgui.h>
#include <imgui-SFML.h>

#include "source/ui/view/receivers/ImGuiMenuEventReceiver.hpp"
#include "source/ui/view/viz/dots/DotsData_t.hpp"

namespace rj
{
  class DotsMenuWindowEventReceiver : public ImGuiMenuEventReceiver
  {
  public:
    explicit DotsMenuWindowEventReceiver( DotsData_t & data )
      : m_data( data )
    {}

  protected:

    void drawMenu() override
    {
      ImGui::Begin(
      "Dots Menu",
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
        if ( ImGui::SliderInt( "Timeout##1", &timeoutInMS, 20, 10000 ) )
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

        ImGui::Separator();
        ImGui::Text( "FPS: %d. (%.2f)", getFPS(), getFPSDelta() );
      }
      ImGui::End();
    }

  private:

    DotsData_t& m_data;
  };
}

#endif