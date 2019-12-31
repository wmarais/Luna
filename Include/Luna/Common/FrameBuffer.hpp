#pragma once

#include <string>
#include "Colour.hpp"
#include "Rect.hpp"
namespace Luna
{
  class FrameBuffer
  {
    /** The handle to the shared memory. */
    int fHandle;

    /** The width of the frame buffer. */
    uint32_t fWidth;

    /** The height of the frame buffer. */
    uint32_t fHeight;

    /** The pointer to the pixel map. */
    Colour * fPixels;

    /** The ID of the frame buffer. */
    uint32_t fID;

    /** The area of the frame buffer. */
    Rect fRect;

    FrameBuffer() = delete;

  public:
    FrameBuffer(uint32_t width, uint32_t height, const std::string & path);
    ~FrameBuffer();

    void resize(uint32_t width, uint32_t height);

    uint32_t id() const
    {
      return fID;
    }

    const Rect & rect() const
    {
      return fRect;
    }

    const Colour * pixels() const
    {
      return fPixels;
    }
  };
}
