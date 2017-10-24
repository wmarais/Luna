#include "../../../Include/Luna/Server/UDEV.hpp"
#include "../../../Include/Luna/Server/Input/InputManager.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

//==============================================================================
const char * UDEV::InputDeviceTypeProps[UDEV::kInputDevicesCount] =
{
  "ID_INPUT_KEYBOARD",
  "ID_INPUT_KEY",
  "ID_INPUT_MOUSE",
  "ID_INPUT_TOUCHPAD",
  "ID_INPUT_TOUCHSCREEN",
  "ID_INPUT_TABLET",
  "ID_INPUT_JOYSTICK",
  "ID_INPUT_ACCELEROMETER"
};

//==============================================================================
const char * UDEV::kDeviceActionStrs[UDEV::kDeviceActionsCount]
{
  "add",
  "remove"
};

//==============================================================================
const char * UDEV::kSubSystemNames[UDEV::kSubSystemsCount]
{
  "input",
  "drm"
};

//==============================================================================
UDEV::UDEV() : fMonitoring(false) {}

//==============================================================================
UDEV::~UDEV()
{
  // Stop the monitor thread.
  stopMonitor();
}

//==============================================================================
void UDEV::startMonitor(std::shared_ptr<InputManager> im)
{
  // Check if a monitoring thread was created.
  if(fMonitorThread)
  {
    // Check if it is running.
    if(fMonitoring)
    {
      // Nothing to do.
      return;
    }

    // Wait for the thread to finish.
    fMonitorThread->join();
  }

  // Set the start flag.
  fMonitoring = true;

  // Start the monitor.
  fMonitorThread = std::make_unique<std::thread>(&UDEV::monitor, this,
                                                 im);
}

//==============================================================================
void UDEV::stopMonitor()
{
  // Check if the thread has been started.
  if(fMonitoring)
  {
    // Tell the thread to stop.
    fMonitoring = false;

    // Wait for the thread to finish.
    fMonitorThread->join();
  }
}

//==============================================================================
std::vector<std::tuple<std::string, UDEV::InputDeviceTypes>>
  UDEV::scanInputDevices()
{

}

//==============================================================================
UDEV::InputDeviceTypes UDEV::getInputType(struct udev_device * dev) const
{
  // The input device that was detected.
  InputDeviceTypes inputDevice = kUnknownInputDevice;

  // Loop through all the device type properties.
  for(int i = 0; inputDevice == kUnknownInputDevice && i < kInputDevicesCount;
      i++)
  {
    // Get the property value.
    std::string value =
        toString(udev_device_get_property_value(dev, InputDeviceTypeProps[i]));

    // Check if the property is set.
    if(value == "1")
    {
      inputDevice = static_cast<InputDeviceTypes>(i);
    }
  }

  // Return the input device.
  return inputDevice;
}

//==============================================================================
UDEV::SubSystems UDEV::getSubSystem(struct udev_device * dev) const
{
  // The sub system that will be returned.
  SubSystems subSys = kUnknownSubSystem;

  // Get the sub system name.
  std::string subSysName = toString(udev_device_get_subsystem(dev));

  // Check which sub systems.
  for(int i = 0; subSys == kUnknownSubSystem && i < kSubSystemsCount; i++)
  {
    // Check if the sub system name matches.
    if(subSysName == kSubSystemNames[i])
    {
      subSys = static_cast<SubSystems>(i);
    }
  }

  // Return the sub system.
  return subSys;
}

//==============================================================================
UDEV::DeviceActions UDEV::getDeviceAction(struct udev_device * dev) const
{
  // The device action that will be returned.
  DeviceActions action = kUnknownDeviceAction;

  // Get the action.
  std::string actionStr = toString(udev_device_get_action(dev));

  for(int i = 0; action == kUnknownDeviceAction && i < kDeviceActionsCount; i++)
  {
    // Check if the action matches.
    if(actionStr == kDeviceActionStrs[i])
    {
      // Set the corresponding action.
      action = static_cast<DeviceActions>(i);
    }
  }

  // Return the action.
  return action;
}

//==============================================================================
void UDEV::monitor(std::shared_ptr<InputManager> im)
{
  try
  {
    // The udev object.
    std::unique_ptr<struct udev, decltype(&udev_unref)>
        udev(udev_new(), udev_unref);

    // Check if the udev object is valid.
    if(!udev)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to create udev object.");
    }

    // Create the monitor object.
    std::unique_ptr<struct udev_monitor, decltype(&udev_monitor_ref)>
        monitor(udev_monitor_new_from_netlink(udev.get(), "udev"),
                udev_monitor_unref);

    // Check the monitor object is valid.
    if(!monitor)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to create monitor object.");
    }

    // Monitor input devices.
    if(udev_monitor_filter_add_match_subsystem_devtype(monitor.get(),
        kSubSystemNames[kInputSubSystem], nullptr) < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to add filter match for input devices.");
    }

    // Monitor drm devices (screens).
    if(udev_monitor_filter_add_match_subsystem_devtype(monitor.get(),
        kSubSystemNames[kDRMSubSystem], nullptr) < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed add filter match for drm devices.");
    }

    // Start recieving hotplug events.
    if(udev_monitor_enable_receiving(monitor.get()) < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to start monitor.");
    }

    // Get the monitor's fild descriptor.
    int fd = udev_monitor_get_fd(monitor.get());

    // Check if the file descriptor is valid.
    if(fd < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to get file descriptor of monitor.");
    }

    // Keep monitoring until the application exits. For this loop to be
    // nonblocking (resolving shutdown lockups), select() is used to wait for
    // data to available before attempting any receive / read.
    while(fMonitoring)
    {
      // Set up the call to select(). In this case, select() will only operate
      // on a single file descriptor, the one associated with our udev_monitor.
      // Note that the timeval object is set to 0, which will cause select() to
      // not block.
      fd_set fds;
      struct timeval tv;

      FD_ZERO(&fds);
      FD_SET(fd, &fds);
      tv.tv_sec = 0;
      tv.tv_usec = 0;

      // Check if any data is available.
      int ret = select(fd+1, &fds, nullptr, nullptr, &tv);

      // Check if an udev event was recieved.
      if(ret > 0 && FD_ISSET(fd, &fds))
      {
        // Get the new device.
        std::unique_ptr<struct udev_device, decltype(&udev_device_unref)>
            device(udev_monitor_receive_device(monitor.get()),
                   udev_device_unref);

        // Make sure it is valid.
        if(device)
        {
          // Get the devNode path.
          std::string devNode =
              toString(udev_device_get_devnode(device.get()));

          // Get the hotplug action.
          DeviceActions devAction = getDeviceAction(device.get());

          // Check the sub system.
          switch(getSubSystem(device.get()))
          {
            // A device was added or removed.
            case kInputSubSystem:
            {
              // Get the device type.
              InputDeviceTypes devType = getInputType(device.get());

              // Input device was hot plugged.
              im->hotplugged(devNode, devType, devAction);
            }
            break;

            // A screen was added in or removed.
            case kDRMSubSystem:
            {

            }
            break;
          }
        }
      }

      // Yield the thread so something else can use the CPU.
      std::this_thread::yield();
    }
  }
  // We only use std::exception and derivatives there of. So we would expect
  // to only ever them.
  catch(std::exception & ex)
  {
    // Set the flag to indicate the monitor thread is not running.
    fMonitoring = false;

    LUNA_LOG_ERROR("Hotplug monitor thread died because: " << ex.what());
  }
  // This will catch any non std::exception types. This should never be reached
  // but for pedantic reasons, lets check for it anyway.
  catch(...)
  {
    // Set the flag to indicate the monitor thread is not running.
    fMonitoring = false;

    LUNA_LOG_ERROR("Hotplug monitor thread died for unknown reasons.");
  }
}
