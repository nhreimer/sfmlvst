#ifndef SFMLDOTS_SQUIGGLEDATA_T_HPP
#define SFMLDOTS_SQUIGGLEDATA_T_HPP

namespace rj
{
  struct SquigglesData_t
  {
    sf::Color startColor { 255, 255, 255 };
    sf::Color endColor { 32, 32, 32 };
    float radius { 15.f };
    int32_t timeoutInMS { 1500 };
    sf::Vector2f center { 640.f, 360.f };
    float spreadMultiplier { 1.f };
  };
}

#endif //SFMLDOTS_SQUIGGLEDATA_T_HPP
