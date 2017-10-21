#include "../../../../Include/Luna/Server/Input/Controller.hpp"

using namespace Luna::Server;

//==============================================================================
Controller::Controller(std::string path)
{
  // Open the controller.
}

//==============================================================================
Controller::~Controller()
{
  // Close the controller.
}

//==============================================================================
void Controller::monitor(std::shared_ptr<SessionManager> sm)
{
  // Keep looping until stop() is called.
  while(isRunning())
  {
    // Check if the input changed and generate events.

    // Yield the thread to give other task time to work.
    std::this_thread::yield();
  }
}

