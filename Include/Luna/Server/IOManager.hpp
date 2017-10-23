#ifndef LUNA_SERVER_IO_MANAGER_HPP
#define LUNA_SERVER_IO_MANAGER_HPP

#include "Input/Mouse.hpp"
#include "Input/Keyboard.hpp"
#include "Input/Joystick.hpp"

namespace Luna
{
  namespace Server
  {
    class IOManager
    {
      //------------------------------------------------------------------------
      // The udev object.
      //------------------------------------------------------------------------
      std::unique_ptr<struct udev, decltype(&udev_unref)> fUDEV;

      //------------------------------------------------------------------------
      // The mice that are monitored. The key is a string to the dev node for
      // the mouse.
      //------------------------------------------------------------------------
      std::map<std::string, std::unique_ptr<Input>> fMice;

      //------------------------------------------------------------------------
      // The keyboards that are monitored. The key is a string to the dev node
      // for the keyboards.
      //------------------------------------------------------------------------
      std::map<std::string, std::unique_ptr<Input>> fKeyboards;

      //------------------------------------------------------------------------
      // The joysticks that are monitored. The key is a string to the dev node
      // for the joystick.
      //------------------------------------------------------------------------
      std::map<std::string, std::unique_ptr<Input>> fJoysticks;

      //------------------------------------------------------------------------
      // Scan for the devnodes of the specified subsystems with the particular
      // property.
      //
      // Sub Systems
      // -----------
      // The complete list of subsystems can be found by executing the following
      // command in the terminal:
      //
      //    ls /sys/class/
      //
      // However at this time, the only ones of interest to us are:
      //
      //    "input" - Used to detect Mice, Keyboards etc. The specific imput
      //              can be limited by specifying one of the properties in
      //
      //    "drm"   - Used to detect the rendering hardware.
      //
      // Input Properties
      // ----------------
      // I'm not entirely sure how you get the properties, but between google
      // and "udevadm info --name=/dev/dri/card0 --query=property". For "input"
      // the following properties are useful:
      //
      //    ID_INPUT_KEYBOARD
      //    ID_INPUT_KEY
      //    ID_INPUT_MOUSE
      //    ID_INPUT_TOUCHPAD
      //    ID_INPUT_TOUCHSCREEN
      //    ID_INPUT_TABLET
      //    ID_INPUT_JOYSTICK
      //    ID_INPUT_ACCELEROMETER
      //------------------------------------------------------------------------
      std::vector<std::string> scanDevNodes(const char * subSystem,
                                            const char * property);

      //------------------------------------------------------------------------
      // Delete the unused constructors as well as the copy and assignment
      // operators.
      //------------------------------------------------------------------------
      IOManager() = delete;
      IOManager(const IOManager&) = delete;
      IOManager & operator = (const IOManager &);

    public:
      //------------------------------------------------------------------------
      // Initialise the IO Manager and create all the input objects to monitor
      // all the required inputs.
      //------------------------------------------------------------------------
      IOManager(Luna::Common::Settings * settings);

      //------------------------------------------------------------------------
      // Start monitoring the input devices for input events.
      //------------------------------------------------------------------------
      void startInputMonitor(std::shared_ptr<SessionManager> sm);

      //------------------------------------------------------------------------
      // Monitor for hotplugged input devices and screens.
      //------------------------------------------------------------------------
      void startHotplugMonitor(std::shared_ptr<SessionManager> sm);
    };
  }
}

#endif // LUNA_SERVER_IO_MANAGER_HPP
