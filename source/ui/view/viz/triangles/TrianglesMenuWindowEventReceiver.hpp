#ifndef SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP

#include "source/ui/view/receivers/ImGuiMenuEventReceiver.hpp"

namespace rj
{
  class TrianglesMenuWindowEventReceiver : public ImGuiMenuEventReceiver
  {
  public:
    explicit TrianglesMenuWindowEventReceiver( TrianglesData_t & data )
    : m_data( data )
    {}

  protected:

    void drawMenu() override
    {
      ImGui::Begin(
      "Triangles Menu",
      nullptr,
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove );
      {
        // we are fixed to the top left corner
        ImGui::SetWindowPos( { 0, 0 } );

        ImGui::Separator();

        int32_t timeoutInMS = m_data.rotationTimeoutInMS;
        if ( ImGui::SliderInt( "Rotation Timeout##1", &timeoutInMS, 20, 10000 ) )
        {
          m_data.rotationTimeoutInMS = timeoutInMS;
        }

        ImGui::Separator();
        ImGui::Text( "FPS: %d. (%.2f)", getFPS(), getFPSDelta() );
      }
      ImGui::End();
    }

  private:
    TrianglesData_t & m_data;
  };
}

#endif //SFMLDOTS_TRIANGLESMENUWINDOWEVENTRECEIVER_HPP
