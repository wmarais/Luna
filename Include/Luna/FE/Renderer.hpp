#pragma once

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <cstdint>


#include "../Common.hpp"

namespace Luna::FE
{
  class Renderer
  {
    int64_t fWidth;
    int64_t fHeight;

    std::vector<Colour> fPixels;

  public:

    Renderer(int64_t width = 800, int64_t height = 600);

    virtual ~Renderer();

    Rect rect() const;



    virtual void fill(const Rect & rect, const Colour & colour);

    virtual void savePNG(const std::string & fileName);

  };
}
