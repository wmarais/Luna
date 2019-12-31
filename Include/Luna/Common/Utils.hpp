#pragma once

#include <string>


#include "../Config.hpp"
#include "Debug/Exception.hpp"

namespace Luna
{
  /*************************************************************************//**
   * An exception occurs if std::string is initialised with a NULL pointer
   *  which can happen when dealing with C strings. This function aliviate the
   *  the problem by initialising the std::string as an empty string ("") if a
   *  a NULL pointer is passed for str.
   *
   ****************************************************************************/
  inline static std::string toString(const char * str)
  {
    return std::string(str ? str : "");
  }
}
