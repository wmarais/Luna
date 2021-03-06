//==============================================================================
// DESIGN NOTES
// ------------
// This is the generic input interface that all input classes derive from. The
// monitor() function must be implemented for each input. The function act as
// the entry point for the MonitorThread and the fRunning flag indicate if the
// the input should be monitored or not. The typical monitor() function
// implementation will look like:
//
//  void Controller::monitor()
//  {
//    // Keep looping until stop() is called.
//    while(isRunning())
//    {
//      // Check if the input changed and generate events.
//
//      // Yield the thread to not waste CPU cycles.
//      std::this_thread::yield();
//    }
//  }
//
//==============================================================================
#pragma once

#include <memory>
#include <thread>
#include <atomic>

namespace Luna::BE
{
  class SessionManager;

  class InputDevice
  {
    // Tracks if the monitoring thread should be running or not.
    std::atomic_bool fRunning;

    // Thread for running the monitor() function asynchronously.
    std::unique_ptr<std::thread> fMonitorThread;

  protected:

    // The entry point for the thread. This is custom implemented for each
    // input type.
    virtual void monitor(std::shared_ptr<SessionManager> sm) = 0;

  public:

    // Default constructor.
    InputDevice();

    // Destructor.
    virtual ~InputDevice();

    // Return true if the input is being monitored, else false.
    bool isRunning() const;

    // Start the monitoring thread.
    void start(std::shared_ptr<SessionManager> sm);

    // Stop the monitoring thread.
    void stop();
  };
}
