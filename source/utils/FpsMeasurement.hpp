#ifndef SFMLDOTS_FPSMEASUREMENT_HPP
#define SFMLDOTS_FPSMEASUREMENT_HPP

#include "source/utils/TimeMeasurement.hpp"

namespace rj
{
  class FpsMeasurement
  {
  public:

    FpsMeasurement()
    {
      m_time.reset();
      m_delta = 0.0;
    }

    void addFrame()
    {
      ++m_frames;
    }

    void reset()
    {
      m_frames = 0;
      m_delta = 0.0;
      m_time.reset();
    }

    [[nodiscard]]
    uint32_t getFPS()
    {
      if ( ( m_time.getMilliseconds() + m_delta ) >= 1000 )
      {
        m_delta = m_time.reset() - 1000;
        m_lastFpsValue = m_frames;
        m_frames = 0;
      }

      return m_lastFpsValue;
    }

    [[nodiscard]]
    double getDelta() const
    {
      return m_delta;
    }

  private:

    TimeMeasurement m_time;
    uint32_t m_frames { 0 };
    uint32_t m_lastFpsValue { 0 };
    double m_delta { 0.0 };
  };

}

#endif //SFMLDOTS_FPSMEASUREMENT_HPP
