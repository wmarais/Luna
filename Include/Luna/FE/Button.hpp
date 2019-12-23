#pragma once

#include "Object.hpp"

namespace Luna::FE
{
  class Button : public Object
  {
  public:

    Button();
    virtual ~Button() override;

    void repaint(Renderer & renderer,
                const Rect<int64_t> & area) override;

  };
}
