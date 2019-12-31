#include <cstring>
#include "../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../Include/Luna/Common/Event.hpp"

using namespace Luna;

/******************************************************************************/
Event::Event() : fType(kInvalidEvent)
{

}

/******************************************************************************/
Event::Event(const Event & event)
{

}

/******************************************************************************/
Event & Event::operator = (const Event & event)
{
  /* Copy the common bits. */
  fType = event.fType;

  /* Just copy the bits that matter. Not sure this is necisarily much faster. */
  switch(event.fType)
  {
    case kFBChangedEvent:
      memcpy(&(fPayload.fFBConfig), &(event.fPayload.fFBConfig),
             sizeof(event.fPayload.fFBConfig));
    break;
  }

  return *this;
}

/******************************************************************************/
void Event::buildFBConfigEvent(Event & event, uint32_t id, const Rect & rect)
{
  event.fType = kFBChangedEvent;
  event.fPayload.fFBConfig.fID = id;
  event.fPayload.fFBConfig.fX = rect.x();
  event.fPayload.fFBConfig.fY = rect.y();
  event.fPayload.fFBConfig.fWidth = rect.width();
  event.fPayload.fFBConfig.fHeight = rect.height();
}

/******************************************************************************/
void Event::serialise(std::vector<uint8_t> & buffer, size_t offset)
{
  /* Calculate thge amount of space required. */
  auto spaceReq = (offset  + sizeof(Event)) - buffer.size();

  /* Resize the buffer if required. */
  if(spaceReq > 0)
  {
    buffer.resize(spaceReq);
  }

  /* Serialise the event into the buffer. */
  memcpy(buffer.data() + offset, this, sizeof(Event));
}

/******************************************************************************/
void Event::deserialise(const std::vector<uint8_t> & buffer, size_t offset)
{
  /* Check if there is enough data in the buffer. */
  if(buffer.size() - offset < sizeof(Event))
  {
    LUNA_THROW_RUNTIME_ERROR("Not enough space to deserialise event.");
  }

  /* Deserialise the event. */
  memcpy(this, buffer.data() + offset, sizeof(Event));
}


