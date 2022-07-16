#ifndef SFMLDOTS_TIMEDDOT_T_HPP
#define SFMLDOTS_TIMEDDOT_T_HPP

#include <cstdint>

#include <SFML/Graphics/CircleShape.hpp>

#include "source/log/PolyLogger.hpp"

namespace rj
{
  struct TimedDot_t
  {
    // starts off at 0 and then builds up to each frame
    int32_t millisecondsElapsed { 0 };  // the lifespan of the dot so far
    sf::CircleShape dot;                // the sfml shape used
    float velocity { 0.f };             // the velocity of the midi note
    sf::Color initialColor;             // to be used as a reference for fading
    bool isRemovable { false };         // has this dot expired?
    bool isKeyOn { false };             // is the key being held down still?
  };
}

#endif //SFMLDOTS_TIMEDDOT_T_HPP
