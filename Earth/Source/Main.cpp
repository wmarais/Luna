#include "../Include/Luna/Server/Earth.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

volatile sig_atomic_t fExecuting = 1;

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

void gLUNA_EarthTerminate(int sig)
{
  LUNA_UNUSED_PARAM(sig);
  fExecuting = 0;
}

int main(int argc, char * argv[])
{

  std::cout << STRINGIZE(LUNA_LOG_DEBUG) << std::endl;
  std::cout << STRINGIZE(LUNA_LOG_MSG) << std::endl;

  // The exit status.
  int status = EXIT_SUCCESS;

  // Listen to for Ctrl + C signals.
  signal(SIGINT, gLUNA_EarthTerminate);

  try
  {
    LUNA_LOG_INFO("Starting LunaEarth ...");

    // Create the earth object.
    Luna::Server::Earth earth;

    // Run the server.
    earth.run(argc, argv);

    // Wait for execution to finish.
    while(fExecuting)
    {
      std::this_thread::yield();
    }

    LUNA_LOG_INFO("Stopping LunaEarth ...");
  }
  catch(std::exception & ex)
  {
    LUNA_LOG_EXCEPTION(ex);

    // Indicate a failure exit state.
    status = EXIT_FAILURE;
  }

  // Return the exit code.
  return status;
}
