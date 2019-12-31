#pragma once

#include <vector>
#include <cstdint>
#include <uuid/uuid.h>
#include "Rect.hpp"

namespace Luna
{
  class Event final
  {
    struct FBConfig
    {
      uint32_t fID;
      int64_t fX;
      int64_t fY;
      int64_t fWidth;
      int64_t fHeight;
    };

    /***********************************************************************//**
     * The payload of an event. To safe sapce, a union is used to overlap the
     * memory of the diffirent payload types.
     **************************************************************************/
    union Payload
    {
      /** The keyboard key that was pressed. */
      int16_t fKeyCode;

      /** The value of the slider  axis. */
      int16_t fAxisValue;

      /** The button that was pressed. */
      int16_t kButtonNumber;

      /** Frame buffer configuration. */
      FBConfig fFBConfig;
    };


  public:

    /** The list of all the supported event types. */
    enum Types : uint8_t
    {
      kInvalidEvent           = 0x00,

      /* Mouse Events. */
      kMouseDownEvent         = 0x01,
      kMouseUpEvent           = 0x02,
      kMouseClickEvent        = 0x03,
      kMouseDoubleClickEvent  = 0x04,
      kMouseTripleClickEvent  = 0x05,
      kMouseAxisEvent         = 0x06,

      /* Keyboard Events. */
      kKeyDownEvent           = 0x07,
      kKeyUpEvent             = 0x08,
      kKeyPressedEvent        = 0x09,

      /* Controller Events. */
      kCtrlAxisEvent          = 0x0A,
      kCtrlButtonDownEvent    = 0x0B,
      kCtrlButtonUpEvent      = 0x0C,
      kCtrlButtonPressedEvent = 0x0D,

      /* A Frame Buffer Change event. This event will never be raised during
       * a repaint, so the FE can always safely change the FB configuration. */
      kFBChangedEvent          = 0xF0,
    };

  private:

    /** The type of the event. */
    Types fType;

    /** The timestamp of the event. */
    uint64_t fTimeStamp;

    /** The payload of the event. */
    Payload fPayload;

  public:

    Event();
    Event(const Event & event);

    Event & operator = (const Event & event);



    void serialise(std::vector<uint8_t> & buffer, size_t offset);

    void deserialise(const std::vector<uint8_t> & buffer, size_t offset);


    static void buildFBConfigEvent(Event & event, uint32_t id,
                                   const Rect & rect);
  };
}
