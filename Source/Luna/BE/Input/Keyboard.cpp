#include <fcntl.h>
#include <linux/input.h>
#include <unistd.h>

#include "../../../../Include/Luna/BE/Input/Keyboard.hpp"
#include "../../../../Include/Luna/Common/Event.hpp"
#include "../../../../Include/Luna/Common/Macros.hpp"

using namespace Luna;
using namespace Luna::BE;

//==============================================================================
Keyboard::Keyboard(const std::string & path) : fPath(path) {}

//==============================================================================
Keyboard::~Keyboard() {}

//==============================================================================
void Keyboard::monitor(std::shared_ptr<SessionManager> sm)
{
  LUNA_UNUSED_PARAM(sm);

  // Open the keyboard.
  int fd;
  struct input_event ie;

  // The latest event.
//  Event event;

  // Open the keyboard file.
  if((fd = open(fPath.c_str(), O_RDONLY)) == -1)
  {
// TODO - THROW SOMETHING BETTER.
    throw "Failed to open keyboard.";
  }

  // Keep looping until stop() is called.
  while(isRunning())
  {
    // Check if the input changed and generate events.
    if(read(fd, &ie, sizeof(struct input_event)) == -1)
    {
// TODO - THROW SOMETHING BETTER.
      throw "Failed to read from keyboard.";
    }

/*    // Record the time stamp.
    event.fTimeStamp = ie.time.tv_usec;

    // Record the keyboard
    event.fData.fKeyCode = ie.code; */

    // Yield the thread to give other task time to work.
    std::this_thread::yield();
  }

  // Close the file if we are no longer reading from it.
  close(fd);
}
