#ifndef SFMLDOTS_TIMEDDOT_T_HPP
#define SFMLDOTS_TIMEDDOT_T_HPP

#include <cstdint>

#include <SFML/Graphics/CircleShape.hpp>

namespace rj
{
  struct TimedDot_t
  {
    // starts off at 0 and then builds up to each frame
    int32_t millisecondsElapsed { 0 };
    sf::CircleShape dot;
    float velocity { 0.f };
    sf::Color initialColor; // to be used as a reference for fading
    bool isRemovable { false };
  };
}

#endif //SFMLDOTS_TIMEDDOT_T_HPP
