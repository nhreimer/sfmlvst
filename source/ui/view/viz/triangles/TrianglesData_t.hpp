#ifndef SFMLDOTS_TRIANGLESDATA_T_HPP
#define SFMLDOTS_TRIANGLESDATA_T_HPP

#include <SFML/System/Vector2.hpp>

namespace rj
{
  struct TrianglesData_t
  {
    float radius { 20.f }; // radius/size of the triangle
    int32_t rotationTimeoutInMS { 50 }; // how many MS to wait before rotating
    int32_t colorTimeoutInMS { 1500 };
    sf::Color endColor { 48, 48, 48 };
    sf::Vector2f center;
  };
}

#endif //SFMLDOTS_TRIANGLESDATA_T_HPP
