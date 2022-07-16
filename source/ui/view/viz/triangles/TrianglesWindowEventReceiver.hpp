#ifndef SFMLDOTS_TRIANGLESWINDOWEVENTRECEIVER_HPP
#define SFMLDOTS_TRIANGLESWINDOWEVENTRECEIVER_HPP

#include <array>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Clock.hpp>

#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/vst/ivstmessage.h"

#include "source/ui/view/receivers/WindowEventReceiver.hpp"
#include "source/ui/view/viz/helpers/MathHelper.hpp"
#include "source/ui/view/viz/helpers/MidiHelper.hpp"
#include "source/ui/view/viz/helpers/ColorHelper.hpp"

namespace rj
{
  class TrianglesWindowEventReceiver : public WindowEventReceiver
  {
  public:

    explicit TrianglesWindowEventReceiver( TrianglesData_t & data )
      : m_data( data )
    {}

    void onInit( sf::RenderWindow &window ) override
    {
      WindowEventReceiver::onInit( window );
      m_data.center = getView().getCenter();

      // must initialize shapes here because the ctor is not aware of the window size
      for ( int i = 0; i < ARRAY_SIZE; ++i )
        initializeShape( i );
    }

    void onShutdown( sf::RenderWindow &window ) override
    {
      m_isShuttingDown = true;
      POLY_DEBUG( "shutting down triangles visualization" );
    }

    void update( sf::RenderWindow &window, const sf::Time &time ) override
    {
      if ( m_clockRotation.getElapsedTime().asMilliseconds() >= m_data.rotationTimeoutInMS )
      {
        auto & view = getView();
        view.rotate( ROTATION_ANGLE );
        window.setView( view );
        m_clockRotation.restart();
      }
    }

    void draw( sf::RenderWindow &window ) override
    {
      for ( auto & shape : m_shapes )
        window.draw( shape );
    }

  protected:

    void processVstMidiOnEvent( const Steinberg::Vst::Event & event ) override
    {
      if ( m_isShuttingDown )
        return;

      std::scoped_lock lock( m_mutex );

      // convert the pitch to its note and octave
      auto noteTuple = MidiHelper::getMidiNote( event.noteOn.pitch );

      auto noteNumber = std::get< 0 >( noteTuple );
      auto noteOctave = std::get< 1 >( noteTuple );

      m_shapes[ noteNumber ].setFillColor( calculateColor( noteOctave, event.noteOn.velocity ) );
    }

    void processVstMidiOffEvent( const Steinberg::Vst::Event & event ) override
    {
      if ( m_isShuttingDown )
        return;

      std::scoped_lock lock( m_mutex );
      // convert the pitch to its note and octave
      auto noteTuple = MidiHelper::getMidiNote( event.noteOn.pitch );
      auto noteNumber = std::get< 0 >( noteTuple );

      m_shapes[ noteNumber ].setFillColor( m_data.endColor );
    }

  private:

    void initializeShape( int index )
    {
      auto &shape = m_shapes[ index ];

      shape.setPointCount( 3 );

      float angle = CIRCLE_ANGLE * ( float )index * MathHelper::RAD_TO_DEG;

      auto position = sf::Vector2f
      {
        std::cos( CIRCLE_ANGLE * static_cast< float >( index ) ) * MathHelper::RAD_TO_DEG + m_data.center.x,
        std::sin( CIRCLE_ANGLE * static_cast< float >( index ) ) * MathHelper::RAD_TO_DEG + m_data.center.y
      };

      shape.setPosition( position );
      shape.rotate( angle );
      shape.setRadius( m_data.radius );
      shape.setFillColor( m_data.endColor );
    }

    static sf::Color calculateColor( int32_t octave, float velocity )
    {
      auto colorAmount = static_cast< uint8_t >( velocity * 255.f );
      switch ( octave )
      {
        case 0: return { colorAmount, 0, 0 };
        case 1: return { 0, colorAmount, 0 };
        case 2: return { 0, 0, colorAmount };

        case 3: return { colorAmount, ( uint8_t )( ( float )colorAmount / 2.f ), 0 };
        case 4: return { 0, colorAmount, ( uint8_t )( ( float )colorAmount / 2.f ) };
        case 5: return { ( uint8_t )( ( float )colorAmount / 2.f ), 0, colorAmount };

        case 6: return { colorAmount, colorAmount, 0 };
        case 7: return { 0, colorAmount, colorAmount };
        case 8: return { colorAmount, 0, colorAmount };

        default:
          return { colorAmount, colorAmount, colorAmount };
      }
    }

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
