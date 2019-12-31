#pragma once

#include "Event.hpp"

namespace Luna
{
  void Serialise(const Event & event, std::vector<uint8_t> & buffer,
                 size_t offset = 0);

  void Deserialise(Event & event, std::vector<uint8_t> & buffer,
                   size_t offset = 0);
}
