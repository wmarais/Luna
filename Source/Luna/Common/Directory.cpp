#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

#include "../../../Include/Luna/Config.hpp"
#include "../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../Include/Luna/Common/Directory.hpp"
#include "../../../Include/Luna/Common/User.hpp"

using namespace Luna;

/******************************************************************************/
Directory::Directory(const std::string & path) : fHandle(nullptr)
{
  fHandle = opendir(path.c_str());
}

/******************************************************************************/
Directory::~Directory()
{
  if(fHandle)
  {
    closedir(fHandle);
  }
}

/******************************************************************************/
bool Directory::exists(const std::string &path)
{
  /* Try to open the directory. */
  DIR * dir = opendir(path.c_str());

  if(dir)
  {
    /* The dir is valid and was was opened. */
    closedir(dir);

    /* Return true to indicate it all went well. */
    return true;
  }
  else if(errno == ENOENT)
  {
    /* Not a directory. */
    return false;
  }

  /* Return false to indicate that it was not a directory, or it was not
   * accessible. */
  return false;
}

/******************************************************************************/
void Directory::mkdir(const std::string & path, uint32_t mode)
{
  /* Create the directory. */
  if(::mkdir(path.c_str(), mode) == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to create directory because: " <<
                             strerror(errno));
  }
}

/******************************************************************************/
std::string Directory::homeDirStr()
{
  /* Check if the HOME environment variable is set. */
  const char * home = getenv("HOME");

  /* If not, try and get it from the user password object. */
  if(home == nullptr)
  {
    /* Get the user's home directory. */
    home = getpwuid(getuid())->pw_dir;
  }

  /* If the home directory does not exist, then throw and exception. */
  if(home == nullptr)
  {
    LUNA_THROW_RUNTIME_ERROR("Unable to determine user's home directory.");
  }

  /* Return the path to use. */
  return std::string(home);
}

/******************************************************************************/
std::string Directory::sockPath()
{
 return std::string(LUNA_CONF_RUN_PATH) + "/" + User::whoami() + "-sock";
}

/******************************************************************************/
std::string Directory::fbPath()
{
  return std::string(LUNA_CONF_RUN_PATH) + "/" + User::whoami() + "-fb";
}



