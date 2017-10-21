#include "../../../Include/Luna/Server/Input.hpp"

using namespace Luna::Server;

//==============================================================================
Input::Input() : fRunning(false) {}

//==============================================================================
Input::~Input() {}

//==============================================================================
bool Input::isRunning() const
{
  return fRunning;
}

//==============================================================================
void Input::start(std::shared_ptr<SessionManager> sm)
{
  // Set the thread as running.
  fRunning = true;

  // Create the monitor thread.
  fMonitorThread = std::make_unique<std::thread>(&Input::monitor, this, sm);
}

//==============================================================================
void Input::stop()
{
  // Stop the thread running.
  fRunning = false;

  // Wait for the thread to join.
  fMonitorThread->join();
}
