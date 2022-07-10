//------------------------------------------------------------------------
// Copyright(c) 2022 nhr.
//------------------------------------------------------------------------

#pragma once

#include <string>

#include "public.sdk/source/vst/vstaudioeffect.h"

namespace Steinberg::Vst { struct Event; }

namespace rj
{

//------------------------------------------------------------------------
//  SfmlDotsProcessor
//------------------------------------------------------------------------
class SfmlDotsProcessor : public Steinberg::Vst::AudioEffect
{
public:
	SfmlDotsProcessor ();
	~SfmlDotsProcessor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{
    return dynamic_cast< Steinberg::Vst::IAudioProcessor * >( new SfmlDotsProcessor );
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:

private:

  /***
   * sends a midi note event (either on or off)
   * @param event midi event
   * @param eventTypeId event id type
   */
  void sendMidiNoteEventMessage( const Steinberg::Vst::Event & event,
                                 const std::string & eventTypeId,
                                 const std::string & attributeId ) const;

};

//------------------------------------------------------------------------
} // namespace rj
