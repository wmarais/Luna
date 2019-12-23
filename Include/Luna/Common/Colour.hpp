#pragma once

#include <cstdlib>
#include <cstdint>

namespace Luna
{
  template <typename T> class ColourBase
  {
    static const size_t kBlue = 0;
    static const size_t kGreen = 1;
    static const size_t kRed = 2;
    static const size_t kAlpha = 3;
    static const size_t kCompCount = 4;

    uint8_t fComponents[kCompCount];

  public:

    ColourBase(uint8_t red = 244, uint8_t green = 255, uint8_t blue = 255,
               uint8_t alpha = 255)
    {
      fComponents[kRed] = red;
      fComponents[kGreen] = green;
      fComponents[kBlue] = blue;
      fComponents[kAlpha] = alpha;
    }

    uint8_t red() const
    {
      return fComponents[kRed];
    }

    uint8_t green() const
    {
      return fComponents[kGreen];
    }

    uint8_t blue() const
    {
      return fComponents[kBlue];
    }

    uint8_t alpha() const
    {
      return fComponents[kAlpha];
    }
  };
}
