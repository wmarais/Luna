#include <unistd.h>
#include "../../../Include/Luna/BE/Window.hpp"
#include "../../../Include/Luna/Common/Serialiser.hpp"

using namespace Luna;
using namespace Luna::BE;

/******************************************************************************/
Window::Window(std::unique_ptr<UnixSocketClient> & socket) :
  fSocket(std::move(socket))
{
  /* Allocate enough bufferspace to hold an event. */
  fEventBuffer.resize(sizeof(Event));
}

/******************************************************************************/
void Window::sendEvent(const Event & event)
{
  /* Serialise the event. */
  Serialise(event, fEventBuffer);

  /* Keep looping until the event is send. */
  while(!fSocket->writeBlockSwap(fEventBuffer))
  {
    sleep(0);
  }
}
