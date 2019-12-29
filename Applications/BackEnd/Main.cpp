#include <signal.h>
#include "BackEnd.hpp"

using namespace Luna;
using namespace Luna::BE;

volatile sig_atomic_t fExecuting = 1;

/******************************************************************************/
void gLUNA_BackEndTerminate(int sig)
{
  LUNA_UNUSED_PARAM(sig);
  fExecuting = 0;
}

/******************************************************************************/
int main(int argc, char * argv[])
{
  /* The exit status. */
  int status = EXIT_SUCCESS;

  /* Listen to for Ctrl + C signals. */
  signal(SIGINT, gLUNA_BackEndTerminate);

  try
  {
    LUNA_LOG_INFO("Starting LunaEarth ...");

    /* Create the earth object. */
    Luna::BE::BackEnd be;;

    /* Run the server. */
    be.run(argc, argv);

    /* Wait for execution to finish. */
    while(fExecuting)
    {
      std::this_thread::yield();
    }

    LUNA_LOG_INFO("Stopping LunaEarth ...");
  }
  catch(std::exception & ex)
  {
    LUNA_LOG_EXCEPTION(ex);

    /* Indicate a failure exit state. */
    status = EXIT_FAILURE;
  }

  /* Return the exit code. */
  return status;
}
