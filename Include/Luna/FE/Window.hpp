#pragma once

#include "Object.hpp"
#include "Renderer.hpp"

namespace Luna::FE
{
  class Window : public Object
  {
  public:
    Window();
    virtual ~Window() override;

    virtual void repaint(Renderer & renderer,
                        const Rect<int64_t> & area) override;

  };
}
