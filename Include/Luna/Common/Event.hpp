#ifndef LUNA_SERVER_EVENT_HPP
#define LUNA_SERVER_EVENT_HPP

#include "GeneralHeaders.hpp"

namespace Luna
{
  namespace Common
  {
    struct Event
    {
      // The list of all the supported event types.
      enum Types : uint8_t
      {
        // Mouse Events.
        kMouseDownEvent         = 0x00,
        kMouseUpEvent           = 0x01,
        kMouseClickEvent        = 0x02,
        kMouseDoubleClickEvent  = 0x03,
        kMouseTripleClickEvent  = 0x04,
        kMouseAxisEvent         = 0x05,

        // Keyboard Events.
        kKeyDownEvent           = 0x06,
        kKeyUpEvent             = 0x07,
        kKeyPressedEvent        = 0x08,

        // Controller Events.
        kCtrlAxisEvent          = 0x09,
        kCtrlButtonDownEvent    = 0x0A,
        kCtrlButtonUpEvent      = 0x0B,
        kCtrlButtonPressedEvent = 0x0C,
      };

      // The event type.
      Types fType;

      // The timestamp of the event.
      uint64_t fTimeStamp;

      // The uuid of the event source.
      uuid_t fUUID;

      // The event data.
      union
      {
        // The key that was pressed.
        int16_t fKeyCode;

        // The value of the axis.
        int16_t fAxisValue;

        // The button that was pressed.
        int16_t kButtonNumber;
      }
      fData;
    };

  }
}

#endif // LUNA_SERVER_EVENT_HPP
