#pragma once

#include "../../Include/Luna/FrontEnd.hpp"

namespace Luna::FE
{
  class Huntress
  {
    /** The handle to the window. */
    std::unique_ptr<Window> fWindow;

  public:
    Huntress(int argc, char * argv[]);

    void run();
  };
}
