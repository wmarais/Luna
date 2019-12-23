#pragma once

#include "Object.hpp"

namespace Luna::FE
{
  class Window : public Object
  {
  public:
    Window();
    virtual ~Window() override;

    virtual void render() override;

  };
}
