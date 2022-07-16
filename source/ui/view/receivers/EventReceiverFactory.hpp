#ifndef SFMLDOTS_EVENTRECEIVERFACTORY_HPP
#define SFMLDOTS_EVENTRECEIVERFACTORY_HPP

#include <memory>

#include "source/ui/view/receivers/IWindowEventReceiver.hpp"
#include "source/ui/view/viz/VisualizationEventDistributor.hpp"

#include "source/ui/view/viz/dots/Dots.hpp"
#include "source/ui/view/viz/triangles/Triangles.hpp"

#include "source/log/PolyLogger.hpp"

namespace rj
{
  typedef VisualizationEventDistributor< DotsData_t,
                                         DotsWindowEventReceiver,
                                         DotsMenuWindowEventReceiver > DotsEventDistributor;

  typedef VisualizationEventDistributor< TrianglesData_t,
                                         TrianglesWindowEventReceiver,
                                         TrianglesMenuWindowEventReceiver > TrianglesEventDistributor;

  struct EventReceiverFactory
  {
    enum E_EventDistribution
    {
      DEFAULT,
      DOTS,
      TRIANGLES,
    };

    static std::unique_ptr< IWindowEventReceiver > create( const E_EventDistribution eventDistribution )
    {
      switch ( eventDistribution )
      {
        case TRIANGLES:
          return std::make_unique< TrianglesEventDistributor >();
        case DOTS:
        case DEFAULT:
        default:
          return std::make_unique< DotsEventDistributor >();
      }
    }
  };

}

#endif //SFMLDOTS_EVENTRECEIVERFACTORY_HPP
