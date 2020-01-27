#pragma once

#include <string>

namespace Luna
{
  class FilePath
  {
    std::string fPathStr;

  public:
    FilePath(const std::string &path) : fPathStr(path) {}
    std::string toString() const { return fPathStr; }
  };
} // namespace Luna
