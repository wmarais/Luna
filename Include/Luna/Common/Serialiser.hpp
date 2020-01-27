#pragma once

#include "Event.hpp"

namespace Luna
{
  size_t SerialisedSize(bool ser = true)
  {
    return 0;
  }

  size_t SerialisedSize(int32_t, bool ser = true)
  {
    return sizeof(int32_t);
  }

  size_t SerialisedSize(uint32_t, bool ser = true)
  {
    return sizeof(uint32_t);
  }





  size_t Serialise(std::vector<uint8_t> & dst, size_t offset, int32_t value,
                   bool isLocal = true)
  {

  }

  size_t Serialise(std::vector<uint8_t> & dst, size_t offset, uint32_t value,
                   bool isLocal = true)
  {

  }

  size_t Deserialise(const std::vector<uint8_t> & src, size_t offset,
                     int32_t & value, bool isLocal = true)
  {

  }

  size_t Deserialise(const std::vector<uint8_t> & src, size_t offset,
                     uint32_t & value, bool isLocal = true)
  {

  }

}
