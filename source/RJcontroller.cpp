//------------------------------------------------------------------------
// Copyright(c) 2022 nhr.
//------------------------------------------------------------------------

#include "RJcontroller.h"
#include "RJcids.h"

#include "source/log/PolyLogger.hpp"

using namespace Steinberg;

namespace rj
{
  //------------------------------------------------------------------------
  // SfmlDotsController Implementation
  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::initialize( FUnknown *context )
  {
    // Here the Plug-in will be instanciated

    rj::SLog::initializeLogger();
    POLY_DEBUG( "initializing controller" );

    //---do not forget to call parent ------
    tresult result = EditControllerEx1::initialize( context );
    if ( result != kResultOk )
      return result;

    // Here you could register some parameters

    return result;
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::terminate()
  {
    // Here the Plug-in will be de-instanciated, last possibility to remove some memory!

    //---do not forget to call parent ------
    return EditControllerEx1::terminate();
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::setComponentState( IBStream *state )
  {
    // Here you get the state of the component (Processor part)
    if ( !state )
      return kResultFalse;

    return kResultOk;
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::setState( IBStream *state )
  {
    // Here you get the state of the controller

    return kResultTrue;
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::getState( IBStream *state )
  {
    // Here you are asked to deliver the state of the controller (if needed)
    // Note: the real state of your plug-in is saved in the processor

    return kResultTrue;
  }

  //------------------------------------------------------------------------
  IPlugView * PLUGIN_API SfmlDotsController::createView( FIDString name )
  {
    IPlugView * result = nullptr;

    // Here the Host wants to open your editor (if you have one)
    if ( FIDStringsEqual( name, Vst::ViewType::kEditor ) )
    {
      POLY_INFO( "creating IPlugView implementation" );
      // we do not own this memory so do NOT free it!
      m_ptrPluginView = new VstPluginViewImpl( { 0, 0, 1280, 760 } );
      result = m_ptrPluginView;
    }

    return result;
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::setParamNormalized( Vst::ParamID tag, Vst::ParamValue value )
  {
    // called by host to update your parameters
    tresult result = EditControllerEx1::setParamNormalized( tag, value );
    return result;
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::getParamStringByValue( Vst::ParamID tag, Vst::ParamValue valueNormalized,
                                                                Vst::String128 string )
  {
    // called by host to get a string for given normalized value of a specific parameter
    // (without having to set the value!)
    return EditControllerEx1::getParamStringByValue( tag, valueNormalized, string );
  }

  //------------------------------------------------------------------------
  tresult PLUGIN_API SfmlDotsController::getParamValueByString( Vst::ParamID tag, Vst::TChar *string,
                                                                Vst::ParamValue &valueNormalized )
  {
    // called by host to get a normalized value from a string representation of a specific parameter
    // (without having to set the value!)
    return EditControllerEx1::getParamValueByString( tag, string, valueNormalized );
  }

  //------------------------------------------------------------------------
  tresult SfmlDotsController::notify( Vst::IMessage * ptrMsg )
  {
    // forward any notifications from the Processor to the IWindowEventReceiver
    // !!! THIS METHOD GETS CALLED ON A DIFFERENT THREAD !!!
    if ( m_ptrPluginView != nullptr )
      m_ptrPluginView->notifyEvent( ptrMsg );

    return ComponentBase::notify( ptrMsg );
  }

} // namespace rj
