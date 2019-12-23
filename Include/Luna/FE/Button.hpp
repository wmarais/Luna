#pragma once

#include "Object.hpp"

namespace Luna::FE
{
  class Button : public Object
  {
  public:

    Button();
    virtual ~Button() override;

    void render() override;

  };
}
