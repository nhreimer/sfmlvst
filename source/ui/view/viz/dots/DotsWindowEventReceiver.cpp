#include "source/ui/view/viz/dots/DotsWindowEventReceiver.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "pluginterfaces/vst/ivstevents.h"

#include "source/ui/view/viz/helpers/ColorHelper.hpp"
#include "source/ui/view/viz/helpers/MathHelper.hpp"
#include "source/ui/view/viz/helpers/MidiHelper.hpp"

#include "source/log/PolyLogger.hpp"

////////////////////////////////////////////////////////////////////////////
///                             PUBLIC
////////////////////////////////////////////////////////////////////////////
/* PUBLIC CTOR */
rj::DotsWindowEventReceiver::DotsWindowEventReceiver( rj::DotsData_t &data )
  : m_data( data )
{}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::DotsWindowEventReceiver::onInit( sf::RenderWindow &window )
{
  WindowEventReceiver::onInit( window );
  m_data.center = getView().getCenter();
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::DotsWindowEventReceiver::onShutdown( sf::RenderWindow &window )
{
  m_isShuttingDown = true;
  POLY_DEBUG( "shutting down DOTS visualizer" );
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::DotsWindowEventReceiver::update( sf::RenderWindow &window, const sf::Time &time )
{
  std::scoped_lock lock( m_mutex );

  for ( auto it = m_midiDots.begin(); it != m_midiDots.end(); )
  {
    if ( !it->second.isRemovable && !it->second.isKeyOn )
    {
      auto & dotData = it->second;
      dotData.millisecondsElapsed += time.asMilliseconds();

      auto percentage = static_cast< float >( dotData.millisecondsElapsed ) /
                        static_cast< float >( m_data.timeoutInMS );

      // mark the dot for removal. it will be removed on the next call to update
      if ( percentage >= 1.f )
      {
        dotData.isRemovable = true;
        dotData.dot.setFillColor( m_data.endColor );
      }
      else
      {
        dotData.dot.setFillColor(
        ColorHelper::getNextColor( dotData.initialColor, m_data.endColor, percentage ) );
      }
    }
    ++it;
  }
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::DotsWindowEventReceiver::draw( sf::RenderWindow &window )
{
  std::scoped_lock lock( m_mutex );
  for ( const auto & pair : m_midiDots )
  {
    if ( !pair.second.isRemovable )
      window.draw( pair.second.dot );
  }
}

////////////////////////////////////////////////////////////////////////////
///                             PROTECTED
////////////////////////////////////////////////////////////////////////////
/* PROTECTED */
void rj::DotsWindowEventReceiver::processVstMidiOnEvent( const Steinberg::Vst::Event &event )
{
  // don't process requests while the system is shutting down
  if ( m_isShuttingDown )
    return;

  std::scoped_lock lock( m_mutex );

  // add a new record
  if ( m_midiDots.find( event.noteOn.pitch ) == m_midiDots.end() )
    m_midiDots.insert( { event.noteOn.pitch, TimedDot_t() } );

  // reset record
  auto & dotData = m_midiDots.find( event.noteOn.pitch )->second;

  dotData.isKeyOn = true;         // hold it down until a midi off note is received
  dotData.isRemovable = false;    // no longer removable if it was before

  // initialize the dot
  dotData.dot.setRadius( m_data.radius );
  dotData.initialColor = ColorHelper::getColorPercentage( m_data.startColor, event.noteOn.velocity );
  dotData.dot.setFillColor( dotData.initialColor );

  // convert the pitch to its note and octave
  auto noteTuple = MidiHelper::getMidiNote( event.noteOn.pitch );

  auto noteNumber = std::get< 0 >( noteTuple );
  auto noteOctave = std::get< 1 >( noteTuple );

  // get the angle the note and octave belong to
  auto position = MathHelper::getAnglePosition( noteNumber,
                                                12,
                                                static_cast< float >( noteOctave ) * m_data.spreadMultiplier,
                                                static_cast< float >( noteOctave ) * m_data.spreadMultiplier );

  position += m_data.center;

  // stick it in a circle
  dotData.dot.setPosition( position );

  // the intensity of the dot is based on its velocity
  dotData.velocity = event.noteOn.velocity;
}

////////////////////////////////////////////////////////////////////////////
/* PROTECTED */
void rj::DotsWindowEventReceiver::processVstMidiOffEvent( const Steinberg::Vst::Event &event )
{
  std::scoped_lock lock( m_mutex );

  auto it = m_midiDots.find( event.noteOff.pitch );

  // don't start processing events
  if ( it != m_midiDots.end() )
    it->second.isKeyOn = false;
}

