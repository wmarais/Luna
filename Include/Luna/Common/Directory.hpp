#pragma once

#include <string>
#include <memory>
#include <dirent.h>
#include <cstring>

namespace Luna
{
  class Directory
  {
    DIR * fHandle;

  public:

    Directory(const std::string & path = "");
   ~Directory();

    /***********************************************************************//**
     * Check if the supplied directory exist by attempting to open it. If it can
     * be opened, the function will return false. If it exist and cant be opened
     * for what ever reason (i.e. permissions) the function will still return
     * false.
     *
     * @param path  The path to check.
     * @return      True if the path exist and can be opened, else false.
     **************************************************************************/
    static bool exists(const std::string & path);

    /***********************************************************************//**
     * Create a new directory at with the specific permissions.
     *
     * @param path  The path to the directory.
     * @param mode  The permission on the directory.
     **************************************************************************/
    static void mkdir(const std::string & path, uint32_t mode = 0700);

    /***********************************************************************//**
     * Return the path to the current user's home directory.
     *
     * @return  The path to the current user's home directory.
     **************************************************************************/
    static std::string homeDirStr();

    static std::string sockPath();
    static std::string fbPath();
  };
}
