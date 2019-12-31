#include <cstring>
#include "../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../Include/Luna/Common/Event.hpp"

using namespace Luna;

namespace Luna
{
  /****************************************************************************/
  void Serialise(const Event & event, std::vector<uint8_t> & buffer,
                 size_t offset)
  {
    /* Calculate thge amount of space required. */
    auto spaceReq = (offset  + sizeof(event)) - buffer.size();

    /* Resize the buffer if required. */
    if(spaceReq > 0)
    {
      buffer.resize(spaceReq);
    }

    /* Serialise the event into the buffer. */
    memcpy(buffer.data() + offset, &event, sizeof(event));
  }

  /****************************************************************************/
  void Deserialise(Event & event, const std::vector<uint8_t> & buffer,
                   size_t offset)
  {
    /* Check if there is enough data in the buffer. */
    if(buffer.size() - offset < sizeof(event))
    {
      LUNA_THROW_RUNTIME_ERROR("Not enough space to deserialise event.");
    }

    /* Deserialise the event. */
    memcpy(&event, buffer.data() + offset, sizeof(event));
  }
}
