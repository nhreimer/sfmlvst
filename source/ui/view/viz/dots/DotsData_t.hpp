#ifndef SFMLDOTS_DOTSDATA_T_HPP
#define SFMLDOTS_DOTSDATA_T_HPP

#include <cstdint>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

namespace rj
{
  struct DotsData_t
  {
    sf::Color startColor { 255, 255, 255 };
    sf::Color endColor { 32, 32, 32 };
    float radius { 15.f };
    int32_t timeoutInMS { 1500 };
    sf::Vector2f center;
    float spreadMultiplier { 1.f };
  };
}

#endif //SFMLDOTS_DOTSDATA_T_HPP
