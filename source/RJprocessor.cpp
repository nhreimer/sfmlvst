//------------------------------------------------------------------------
// Copyright(c) 2022 nhr.
//------------------------------------------------------------------------

#include "RJprocessor.h"
#include "RJcids.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "pluginterfaces/vst/ivstevents.h"

#include "log/PolyLogger.hpp"

using namespace Steinberg;

namespace rj
{
//------------------------------------------------------------------------
// SfmlDotsProcessor
//------------------------------------------------------------------------
SfmlDotsProcessor::SfmlDotsProcessor ()
{
  SLog::initializeLogger();

	//--- set the wanted controller for our processor
	setControllerClass (kSfmlDotsControllerUID);
}

//------------------------------------------------------------------------
SfmlDotsProcessor::~SfmlDotsProcessor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instanciated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::terminate ()
{
	// Here the Plug-in will be de-instanciated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    /*if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId ())
                {
				}
			}
		}
	}*/
	
	//--- Here you have to implement your processing

  if ( data.inputEvents != nullptr )
  {
    for ( Steinberg::int32 i = 0; i < data.inputEvents->getEventCount(); ++i )
    {
      Steinberg::Vst::Event event {};
      if ( data.inputEvents->getEvent( i, event ) != kResultFalse &&
           event.type == Steinberg::Vst::Event::kNoteOnEvent )
      {
        sendMidiNoteEventMessage( event, "midiEvent", "midiOn" );
      }
    }
  }

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API SfmlDotsProcessor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------

void SfmlDotsProcessor::sendMidiNoteEventMessage( const Vst::Event & event,
                                                  const std::string & eventTypeId,
                                                  const std::string & attributeId ) const
{
  auto * ptrMsg = allocateMessage();
  if ( ptrMsg != nullptr )
  {
    ptrMsg->setMessageID( eventTypeId.c_str() );

    if ( ptrMsg->getAttributes()->setBinary( attributeId.c_str(),
                                             &event,
                                             sizeof( Vst::Event ) ) == kResultOk )
    {
      // NOTE: the message can fail to send but still actually send
      this->sendMessage( ptrMsg );
//      if ( this->sendMessage( ptrMsg ) != kResultOk )
//        POLY_ERROR( "failed to send message" );
    }
    else
      POLY_ERROR( "failed to set midi data in message" );

    ptrMsg->release();
  }
  else
    POLY_ERROR( "failed to allocate message" );
}

} // namespace rj
