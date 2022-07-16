#ifndef RJRZMBK_TIMEMEASUREMENT_HPP
#define RJRZMBK_TIMEMEASUREMENT_HPP

#include <chrono>

using namespace std::chrono;

namespace rj
{
  class TimeMeasurement
  {
  public:

    TimeMeasurement() = default;

    double reset()
    {
      auto result = getMilliseconds();
      m_lastMeasurement = high_resolution_clock::now();
      return result;
    }

    [[nodiscard]] inline double getMilliseconds() const
    {
      return duration< double, std::milli >(
             high_resolution_clock::now() -
             m_lastMeasurement ).count();
    }

  private:

    high_resolution_clock::time_point m_lastMeasurement;

  };
}

#endif//RJRZMBK_TIMEMEASUREMENT_HPP
