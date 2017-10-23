#include "../../../../Include/Luna/Server/Input/Joystick.hpp"

using namespace Luna::Server;

//==============================================================================
Joystick::Joystick(std::string path)
{
  // Open the controller.
}

//==============================================================================
Joystick::~Joystick()
{
  // Close the controller.
}

//==============================================================================
void Joystick::monitor(std::shared_ptr<SessionManager> sm)
{
  // Keep looping until stop() is called.
  while(isRunning())
  {
    // Check if the input changed and generate events.

    // Yield the thread to give other task time to work.
    std::this_thread::yield();
  }
}

