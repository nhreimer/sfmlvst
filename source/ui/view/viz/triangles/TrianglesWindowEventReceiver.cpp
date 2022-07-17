#include "source/ui/view/viz/triangles/TrianglesWindowEventReceiver.hpp"

#include "source/ui/view/viz/helpers/MidiHelper.hpp"
#include "source/ui/view/viz/helpers/ColorHelper.hpp"

#include "source/ui/view/viz/triangles/TrianglesData_t.hpp"

////////////////////////////////////////////////////////////////////////////
///                             PUBLIC
////////////////////////////////////////////////////////////////////////////
/* PUBLIC CTOR */
rj::TrianglesWindowEventReceiver::TrianglesWindowEventReceiver( TrianglesData_t &data )
  : m_data( data )
{}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::TrianglesWindowEventReceiver::onInit( sf::RenderWindow &window )
{
  WindowEventReceiver::onInit( window );
  m_data.center = getView().getCenter();

  // must initialize shapes here because the ctor is not aware of the window size
  for ( int i = 0; i < ARRAY_SIZE; ++i )
    initializeShape( i );
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::TrianglesWindowEventReceiver::onShutdown( sf::RenderWindow &window )
{
  m_isShuttingDown = true;
  POLY_DEBUG( "shutting down triangles visualization" );
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::TrianglesWindowEventReceiver::update( sf::RenderWindow &window, const sf::Time &time )
{
  if ( m_clockRotation.getElapsedTime().asMilliseconds() >= m_data.rotationTimeoutInMS )
  {
    auto & view = getView();
    view.rotate( ROTATION_ANGLE );
    window.setView( view );
    m_clockRotation.restart();
  }
}

void rj::TrianglesWindowEventReceiver::draw( sf::RenderWindow &window )
{
  for ( auto & shape : m_shapes )
    window.draw( shape );
}

////////////////////////////////////////////////////////////////////////////
///                             PROTECTED
////////////////////////////////////////////////////////////////////////////
/* PROTECTED */
void rj::TrianglesWindowEventReceiver::processVstMidiOnEvent( const Steinberg::Vst::Event &event )
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

////////////////////////////////////////////////////////////////////////////
/* PROTECTED */
void rj::TrianglesWindowEventReceiver::processVstMidiOffEvent( const Steinberg::Vst::Event &event )
{
  if ( m_isShuttingDown )
    return;

  std::scoped_lock lock( m_mutex );
  // convert the pitch to its note and octave
  auto noteTuple = MidiHelper::getMidiNote( event.noteOn.pitch );
  auto noteNumber = std::get< 0 >( noteTuple );

  m_shapes[ noteNumber ].setFillColor( m_data.endColor );
}

////////////////////////////////////////////////////////////////////////////
///                             PRIVATE
////////////////////////////////////////////////////////////////////////////
/* PRIVATE */
void rj::TrianglesWindowEventReceiver::initializeShape( int index )
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

/* STATIC PRIVATE */
sf::Color rj::TrianglesWindowEventReceiver::calculateColor( int32_t octave, float velocity )
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
