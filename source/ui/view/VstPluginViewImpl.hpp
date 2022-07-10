#ifndef SFMLDOTS_VSTVIEW_HPP
#define SFMLDOTS_VSTVIEW_HPP

#include <memory>

#include <base/source/fobject.h>
#include <pluginterfaces/gui/iplugview.h>

#include "source/ui/view/receivers/ReceiverDistributor.hpp"
#include "source/ui/view/IChildWindow.hpp"

namespace rj
{
  /***
   * This class gets called by the RJcontroller class
   * @see Steinberg::IPlugView for more details
   */
  class VstPluginViewImpl : public Steinberg::FObject,
                            public Steinberg::IPlugView,
                            public Steinberg::IPlugFrame
  {
  public:

    // pass in the UI state to your plugin
    explicit VstPluginViewImpl( Steinberg::ViewRect windowSize );

    ~VstPluginViewImpl() override = default;

    Steinberg::tresult isPlatformTypeSupported( Steinberg::FIDString type ) override;

    /***
     * This loads our own Child Window that we will control and display.
     * @param parent parent window handle
     * @param type
     * @return kResultOk/kResultTrue if successful, otherwise false
     */
    Steinberg::tresult attached( void * parent, Steinberg::FIDString type ) override;
    Steinberg::tresult removed() override;
    Steinberg::tresult onWheel( float distance ) override;
    Steinberg::tresult onKeyDown( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override;
    Steinberg::tresult onKeyUp( Steinberg::char16 key, Steinberg::int16 keyCode, Steinberg::int16 modifiers ) override;
    Steinberg::tresult getSize( Steinberg::ViewRect *size ) override;
    Steinberg::tresult onSize( Steinberg::ViewRect *newSize ) override;
    Steinberg::tresult onFocus( Steinberg::TBool state ) override;
    Steinberg::tresult setFrame( Steinberg::IPlugFrame *frame ) override;
    Steinberg::tresult canResize() override;
    Steinberg::tresult checkSizeConstraint( Steinberg::ViewRect *rect ) override;

    Steinberg::tresult resizeView( IPlugView *view, Steinberg::ViewRect *newSize ) override;

    void notifyEvent( Steinberg::Vst::IMessage * ptrMsg );

    //---Interface------
    OBJ_METHODS (VstPluginViewImpl, FObject)
    DEFINE_INTERFACES
      DEF_INTERFACE (IPlugView)
      DEF_INTERFACE (IPlugFrame)
    END_DEFINE_INTERFACES (FObject)
    REFCOUNT_METHODS (FObject)

  private:

    // used for the initial size only
    const Steinberg::ViewRect m_rect;

    // distributes window-related events out to other IWindowEventReceiver implementations
    rj::ReceiverDistributor m_eventDistributor;

    // SFML RenderWindow that is a child of the parent given in the attach() method
    std::unique_ptr< rj::IChildWindow > m_ptrChildWnd;
  };
}

#endif //SFMLDOTS_VSTVIEW_HPP
