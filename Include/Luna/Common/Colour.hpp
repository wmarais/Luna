#pragma once

#include <cstdlib>
#include <cstdint>

#include "../Config.hpp"

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

  /** Define the default colour type used by Luna. */
  typedef ColourBase<LUNA_CONF_COLOUR_COMP_TYPE> Colour;

  static const Colour kWhite(255, 255, 255, 255);
  static const Colour kBlack(0, 0, 0, 255);
  static const Colour kBlue(0, 0, 255, 255);
  static const Colour kRed(255, 0, 0, 255);
}
