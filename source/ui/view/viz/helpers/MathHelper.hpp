#ifndef SFMLDOTS_MATHHELPER_HPP
#define SFMLDOTS_MATHHELPER_HPP

#include <cstdint>
#include <cmath>

#include <SFML/System/Vector2.hpp>

namespace rj
{
  struct MathHelper
  {

    inline static const float PI         =  3.141592653f;
    inline static const float TAU        =  6.283185307f;
    inline static const float RAD_TO_DEG = 57.295779513f;

    /***
     * Calculates the angle around a circle according to its partition number
     * e.g., 12 notes form a circle and we want the 3rd note, so it would be
     * theta = 360 / 12 * 3, x = cos( theta ) * xDistance, y = sin( theta ) * yDistance
     * Additional offsets can be provided in order to spread out the positions
     * @param partitions the total number of segments around a circle
     * @param partitionIndex the index around the circle needed
     * @param xDistance distance from center (1 = default)
     * @param yDistance distance from center (1 = default)
     * @return x and y in degrees
     */
    template< typename T >
    static inline sf::Vector2f getAnglePosition( T partitions,
                                                 T partitionIndex,
                                                 float xDistance = 1.f,
                                                 float yDistance = 1.f )
    {
      auto theta = TAU / static_cast< float >( partitions ) * static_cast< float >( partitionIndex );

      return sf::Vector2f { std::cos( theta ) * RAD_TO_DEG * xDistance,
                            std::sin( theta ) * RAD_TO_DEG * yDistance };
    }
  };
}

#endif //SFMLDOTS_MATHHELPER_HPP
