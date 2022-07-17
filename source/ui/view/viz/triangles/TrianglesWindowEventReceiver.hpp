#ifndef SFMLDOTS_TRIANGLESWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_TRIANGLESWINDOWEVENTRECEIVER_HPP

#include <array>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>

#include "source/ui/view/viz/helpers/MathHelper.hpp"
#include "source/ui/view/receivers/WindowEventReceiver.hpp"

namespace rj
{
  struct TrianglesData_t;

  class TrianglesWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit TrianglesWindowEventReceiver( TrianglesData_t & data );

    void onInit( sf::RenderWindow &window ) override;

    void onShutdown( sf::RenderWindow &window ) override;

    void update( sf::RenderWindow &window, const sf::Time &time ) override;

    void draw( sf::RenderWindow &window ) override;

  protected:

    void processVstMidiOnEvent( const Steinberg::Vst::Event & event ) override;

    void processVstMidiOffEvent( const Steinberg::Vst::Event & event ) override;

  private:

    void initializeShape( int index );

    static sf::Color calculateColor( int32_t octave, float velocity );

  private:

    std::atomic_bool m_isShuttingDown { false };

    // a single degree at a time
    inline static const float ROTATION_ANGLE = MathHelper::TAU * MathHelper::RAD_TO_DEG / 360.f;

    // spacing of the shapes around the circle
    inline static const float CIRCLE_ANGLE = MathHelper::TAU / 12.f;

    inline static const int32_t ARRAY_SIZE = 12;

    sf::Clock m_clockRotation;

    TrianglesData_t & m_data;
    std::array< sf::CircleShape, ARRAY_SIZE > m_shapes;

    std::mutex m_mutex;
  };
}

#endif //SFMLDOTS_TRIANGLESWINDOWEVENTRECEIVER_HPP
