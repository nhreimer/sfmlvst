#include "source/ui/view/VstPluginViewImpl.hpp"

#include "source/log/PolyLogger.hpp"

#include "source/ui/view/ChildWindowFactory.hpp"

////////////////////////////////////////////////////////////////////////////
///                             PUBLIC
////////////////////////////////////////////////////////////////////////////
/* PUBLIC CTOR */
rj::VstPluginViewImpl::VstPluginViewImpl( Steinberg::ViewRect windowSize )
  : m_rect( windowSize )
{}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::isPlatformTypeSupported( Steinberg::FIDString type )
{
  // Windows platform
  if ( strcmp( type, Steinberg::kPlatformTypeHWND ) == 0 )
    return Steinberg::kResultTrue;

  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::attached( void * parent,
                                                    Steinberg::FIDString type )
{
  auto result = Steinberg::kResultTrue;

  // free any memory hanging around from previous windows -- WHICH SHOULD NOT OCCUR!
  if ( m_ptrChildWnd != nullptr )
  {
    POLY_WARN( "previous child window was not detached. detaching now." );

    // free the memory from the previous instance. if this occurs then the UI
    // state may be undefined. the memory should be freed in remove()
    m_ptrChildWnd.reset();
  }

  POLY_DEBUG( "attempting to attach" );
  m_ptrChildWnd = rj::ChildWindowFactory::createChildWindow( reinterpret_cast< HWND >( parent ),
                                                             m_eventDistributor,
                                                             sf::ContextSettings { 24, 8, 4 } );

  // ensure our window creation was successful
  if ( m_ptrChildWnd != nullptr )
  {
    // set the initial window size
    m_ptrChildWnd->setSize( m_rect.right, m_rect.bottom );

    if ( !m_ptrChildWnd->start() )
    {
      POLY_CRITICAL( "failed to start child window or event loop. shutting down." );
      m_ptrChildWnd.reset();
      result = Steinberg::kResultFalse;
    }
    else
      POLY_INFO( "successfully started child event loop" );
  }
  else
  {
    POLY_CRITICAL( "failed to create child window" );
    result = Steinberg::kResultFalse;
  }

  return result;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::removed()
{
  // when the destructor is called, the onShutdown event will be trigger
  // so that any state can be saved
  if ( m_ptrChildWnd != nullptr )
  {
    POLY_DEBUG( "initiating window shutdown process" );
    m_ptrChildWnd.reset();
  }

  return Steinberg::kResultTrue;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::onWheel( float distance )
{
  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers )
{
  if ( m_ptrChildWnd != nullptr )
  {
    m_eventDistributor.onKeyDown( key, keyCode, modifiers );
    return Steinberg::kResultTrue;
  }

  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers )
{
  if ( m_ptrChildWnd != nullptr )
  {
    m_eventDistributor.onKeyUp( key, keyCode, modifiers );
    return Steinberg::kResultTrue;
  }

  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::getSize( Steinberg::ViewRect * size )
{
  if ( size != nullptr && m_ptrChildWnd != nullptr )
  {
    auto wndSize = m_ptrChildWnd->getSize();
    size->right = static_cast< int32_t >( wndSize.x );
    size->bottom = static_cast< int32_t >( wndSize.y );

    onSize( size );

    return Steinberg::kResultTrue;
  }

  return Steinberg::kInvalidArgument;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::onSize( Steinberg::ViewRect * newSize )
{
  if ( m_ptrChildWnd != nullptr )
    m_ptrChildWnd->setSize( newSize->getWidth(), newSize->getHeight() );

  return Steinberg::kResultTrue;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::onFocus( Steinberg::TBool state )
{
  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::setFrame( Steinberg::IPlugFrame * frame )
{
  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::canResize()
{
  return Steinberg::kResultTrue;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::checkSizeConstraint( Steinberg::ViewRect * rect )
{
  if ( rect->right < 800 )
    rect->right = 800;

  if ( rect->bottom < 600 )
    rect->bottom = 600;

  return Steinberg::kResultTrue;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
Steinberg::tresult rj::VstPluginViewImpl::resizeView( Steinberg::IPlugView * view,
                                                      Steinberg::ViewRect * newSize )
{
  return Steinberg::kResultFalse;
}

////////////////////////////////////////////////////////////////////////////
/* PUBLIC */
void rj::VstPluginViewImpl::notifyEvent( Steinberg::Vst::IMessage * ptrMsg )
{
  m_eventDistributor.receiveVstEvent( ptrMsg );
}
