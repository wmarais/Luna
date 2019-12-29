#include "../../../../Include/Luna/BE/Input/InputDevice.hpp"

using namespace Luna::BE;

//==============================================================================
InputDevice::InputDevice() : fRunning(false) {}

//==============================================================================
InputDevice::~InputDevice() {}

//==============================================================================
bool InputDevice::isRunning() const
{
  return fRunning;
}

//==============================================================================
void InputDevice::start(std::shared_ptr<SessionManager> sm)
{
  // Set the thread as running.
  fRunning = true;

  // Create the monitor thread.
  fMonitorThread = std::make_unique<std::thread>(&InputDevice::monitor,
                                                 this, sm);
}

//==============================================================================
void InputDevice::stop()
{
  // Stop the thread running.
  fRunning = false;

  // Wait for the thread to join.
  fMonitorThread->join();
}
