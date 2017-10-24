#include "../Include/Luna/Server/Earth.hpp"

using namespace Luna::Server;


int main(int argc, char * argv[])
{
  // The exit status.
  int status = EXIT_SUCCESS;

  try
  {
    // Create the earth object.
    Luna::Server::Earth earth;

    // Run the server.
    earth.run(argc, argv);
  }
  catch(std::exception & ex)
  {
    // Indicate a failure exit state.
    status = EXIT_FAILURE;
  }

  // Return the exit code.
  return status;
}
