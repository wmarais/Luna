#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include "../../../Include/Luna/Common/User.hpp"
#include "../../../Include/Luna/Common/Debug/Exception.hpp"

using namespace Luna;

/******************************************************************************/
std::string User::whoami()
{
  /* Get the current user ID. */
  uid_t uid = geteuid();

  /* Get the password structure for the current UID. */
  struct passwd * pw = getpwuid(uid);

  /* Check if the password structure is valid. */
  if(!pw)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to retrieve password structure for UID: "
                             << uid);
  }

  /* Return the current user name. */
  return std::string(pw->pw_name);
}
