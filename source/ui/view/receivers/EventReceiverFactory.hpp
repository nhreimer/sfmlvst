#ifndef SFMLDOTS_EVENTRECEIVERFACTORY_HPP
#define SFMLDOTS_EVENTRECEIVERFACTORY_HPP

#include <memory>

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"
#include "source/ui/view/viz/VisualizationEventDistributor.hpp"

#include "source/ui/view/viz/dots/Dots.hpp"
#include "source/ui/view/viz/squiggles/Squiggles.hpp"

#include "source/log/PolyLogger.hpp"

namespace rj
{
  typedef VisualizationEventDistributor< DotsData_t,
                                         DotsWindowEventReceiver,
                                         DotsMenuWindowEventReceiver > DotsEventDistributor;

  typedef VisualizationEventDistributor< SquigglesData_t,
                                         SquigglesWindowEventReceiver,
                                         SquigglesMenuWindowEventReceiver > SquigglesEventDistributor;


  struct EventReceiverFactory
  {
    enum E_EventDistribution
    {
      DEFAULT,
      DOTS,
      SQUIGGLES,
    };

    static std::unique_ptr< IWindowEventReceiver > create( const E_EventDistribution eventDistribution )
    {
      switch ( eventDistribution )
      {
        case SQUIGGLES:
          return std::make_unique< SquigglesEventDistributor >();
        case DOTS:
        case DEFAULT:
        default:
          return std::make_unique< DotsEventDistributor >();
      }
    }
  };

}

#endif //SFMLDOTS_EVENTRECEIVERFACTORY_HPP
