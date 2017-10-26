//##############################################################################
//
// Provide the ability to scan for connected input devices and screens as well
// as to monitor for hotplug events related to input devices and screens being
// plugged in and unplugged.
//
// Usage Note: If you scan for connected input and display devices then start
// monitoring for hotplugged devices, it is possible that some devices may not
// be detected. I.e. if the user plugged the device in right between the scan
// finishing and the hotplug monitor starting. To avoid this, start the hotplug
// monitoring first, then scan for devices. The Input and Display managers are
// cabpable of dealing with duplicate device adds and as such no problems will
// arise from the hotplug monitoring adding / remove a device while the device
// scanner tries to add / remvove a device. This overlap however means that
// devices will not be missed. When a device is unplugged, its dev node entry is
// also removed. Thus if the hotplug system remove the device before the scanner
// tries to add it back in, the addition will fail (non fatal) and the device
// will not be monitored either. So ... all good?
//##############################################################################
#ifndef LUNA_SERVER_UDEV_HPP
#define LUNA_SERVER_UDEV_HPP

#include "../../../Include/Luna/Common.hpp"

namespace Luna
{
  namespace Server
  {
    class InputManager;
    class DisplayManager;

    class UDEV
    {
    public:
      //------------------------------------------------------------------------
      // The list of input device types.
      //------------------------------------------------------------------------
      enum InputDeviceTypes : int
      {
        kKeyboardInputDevice = 0,
        kKeyInputDevice,
        kMouseInputDevice,
        kTouchPadInputDevice,
        kTouchScreenInputDevice,
        kTabletInputDevice,
        kJoystickInputDevice,
        kAccelerometerInputDevice,
        kInputDevicesCount,
        kUnknownInputDevice
      };

      //------------------------------------------------------------------------
      // The hotplug action.
      //------------------------------------------------------------------------
      enum DeviceActions : int
      {
        kAddDevice = 0,
        kRemoveDevice,
        kDeviceActionsCount,
        kUnknownDeviceAction
      };

      //------------------------------------------------------------------------
      // Default constructor. Only intialises the member variables, does not
      // start any monitoring function.
      //------------------------------------------------------------------------
      UDEV();

      //------------------------------------------------------------------------
      // Stop any monitoring to shut down cleanly.
      //------------------------------------------------------------------------
      ~UDEV();

      //------------------------------------------------------------------------
      // Start monitoring for hot plugged devices.
      //------------------------------------------------------------------------
      void startMonitor(std::shared_ptr<InputManager> im,
                        std::shared_ptr<DisplayManager> dm);

      //------------------------------------------------------------------------
      // Stop monitoring for hot plugged devices.
      //------------------------------------------------------------------------
      void stopMonitor();

      //------------------------------------------------------------------------
      // Scan for all the attached input devices and add the detected ones to
      // the input manager.
      //------------------------------------------------------------------------
      void scan(InputManager * im, DisplayManager * dm);

    private:

      //------------------------------------------------------------------------
      // The sub systems of interest.
      //------------------------------------------------------------------------
      enum SubSystems : int
      {
        kInputSubSystem = 0,
        kDRMSubSystem,
        kSubSystemsCount,
        kUnknownSubSystem
      };

      //------------------------------------------------------------------------
      // The list of input device ID's that can be tested to determine the type
      // of input devices thats been hot plugged.
      //------------------------------------------------------------------------
      static const char * InputDeviceTypeProps[kInputDevicesCount];

      //------------------------------------------------------------------------
      // The list of action strings of interest to this project.
      //------------------------------------------------------------------------
      static const char * kDeviceActionStrs[kDeviceActionsCount];

      //------------------------------------------------------------------------
      // The list of sub system names. This is not a complete list, only the
      // ones of interest to this project.
      //------------------------------------------------------------------------
      static const char * kSubSystemNames[kSubSystemsCount];

      //------------------------------------------------------------------------
      // The boolean flag indicating the status of the monitoring thread.
      //------------------------------------------------------------------------
      std::atomic_bool fMonitoring;

      //------------------------------------------------------------------------
      // The thread to monitor for hotplug events.
      //------------------------------------------------------------------------
      std::unique_ptr<std::thread> fMonitorThread;

      //------------------------------------------------------------------------
      // The entry point for the monitor thread.
      //------------------------------------------------------------------------
      void monitor(std::shared_ptr<InputManager> im,
                   std::shared_ptr<DisplayManager> dm);

      //------------------------------------------------------------------------
      // Determine the input type that was hot plugged.
      //------------------------------------------------------------------------
      InputDeviceTypes getInputType(struct udev_device * dev) const;

      //------------------------------------------------------------------------
      // Determine the sub system that raised the hotplug event.
      //------------------------------------------------------------------------
      SubSystems getSubSystem(struct udev_device * dev) const;

      //------------------------------------------------------------------------
      // Determine if the device was plugged in or unplugged.
      //------------------------------------------------------------------------
      DeviceActions getDeviceAction(struct udev_device * dev) const;

      //------------------------------------------------------------------------
      // Scan for particular devices.
      //------------------------------------------------------------------------
      std::vector<std::string> scanDevices(const char * subSystem,
                                           const char * property);
    };
  }
}


#endif // LUNA_SERVER_UDEV_HPP
