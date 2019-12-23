#pragma once

#include <vector>
#include <string>
#include <exception>
#include <iostream>
#include <cstdint>

#include "../Common/Rect.hpp"

namespace Luna::FE
{
  class Renderer
  {

  public:

    Renderer();
    virtual ~Renderer();

    virtual void fill(const Luna::Rect<int64_t> & rect);

  };
}
