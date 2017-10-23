#include "../../../Include/Luna/Server/IOManager.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

//==============================================================================
IOManager::IOManager(Settings * settings)
  : fUDEV(udev_new(), udev_unref)
{ 
  // Validate the settings parameter.
  assert(settings && "Settings object is null.");

  LUNA_LOG_INFO("Validating udev object.");
  if(!fUDEV)
  {
    LUNA_THOW_RUNTIME_ERROR("Failed to create UDEV device.");
  }

  // TODO - Implement the getArray() function in the settings and initialise
  //        the vectors to the user defined arrays of input devices.
  std::vector<std::string> mice;
  std::vector<std::string> keyboards;
  std::vector<std::string> joysticks;

  // Check if we should try to autodetect all the attached mice.
  if(settings->getValue("detect.input.mice", true))
  {
    LUNA_LOG_INFO("Scanning for mice.");
    mice = scanDevNodes("input", "ID_INPUT_MOUSE");
  }

  // Check if we should try and autodetect all the attached keyboards.
  if(settings->getValue("detect.input.keyboards", true))
  {
    LUNA_LOG_INFO("Scanning for keyboards.");
    keyboards = scanDevNodes("input", "ID_INPUT_KEYBOARD");
  }

  // Check if we should try and autodetect all the attached joysticks.
  if(settings->getValue("detect.input.joysticks", true))
  {
    LUNA_LOG_INFO("Scanning for joysticks.");
    joysticks = scanDevNodes("input", "ID_INPUT_JOYSTICK");
  }

  // Create all the mice.
  for(std::string devNode : mice)
  {
    LUNA_LOG_INFO("Creating Mouse: " << devNode);
    fMice[devNode] = std::make_unique<Mouse>(devNode);
  }

  // Create all the keyboards.
  for(std::string devNode : keyboards)
  {
    LUNA_LOG_INFO("Creating Keyboard: " << devNode);
    fKeyboards[devNode] = std::make_unique<Joystick>(devNode);
  }

  // Create all the joysticks.
  for(std::string devNode : joysticks)
  {
    LUNA_LOG_INFO("Creating Joystick: " << devNode);
    fJoysticks[devNode] = std::make_unique<Joystick>(devNode);
  }
}

//==============================================================================
std::vector<std::string> IOManager::scanDevNodes(const char * subSystem,
                                                 const char * property)
{
  // The list of dev nodes.
  std::vector<std::string> devNodes;

  // Create a list of input devices.
  std::unique_ptr<struct udev_enumerate, decltype(&udev_enumerate_unref)>
      enumerator(udev_enumerate_new(fUDEV.get()), udev_enumerate_unref);

  // Check if the enumerator object is created.
  if(!enumerator)
  {
    LUNA_THOW_RUNTIME_ERROR("Failed to create enumerator object.");
  }

  // Check if a particular sub system must be scanned.
  if(subSystem)
  {
    if(udev_enumerate_add_match_subsystem(enumerator.get(), subSystem) < 0)
    {
      LUNA_THOW_RUNTIME_ERROR("Failed to set enumerate sub system.");
    }
  }

  // Check if a particular property must be matched.
  if(property)
  {
    // Only scan for Mice.
    if(udev_enumerate_add_match_property(enumerator.get(), property, "1") < 0)
    {
      LUNA_THOW_RUNTIME_ERROR("Failed to set match property.");
    }
  }

  // Scan for the devices of interest.
  if(udev_enumerate_scan_devices(enumerator.get()) < 0)
  {
    LUNA_THOW_RUNTIME_ERROR("Failed to scan for devices.");
  }

  // The first entry in the list of devices.
  struct udev_list_entry * firstDevListEntry =
      udev_enumerate_get_list_entry(enumerator.get());

  // Check there is alteast a first entry.
  if(firstDevListEntry)
  {
    // The current device.
    struct udev_list_entry * curDevListEntry = nullptr;

    // Iterate through all the devices.
    udev_list_entry_foreach(curDevListEntry, firstDevListEntry)
    {
      // Get the filename of the /sys entry for the device and create a
      // udev_device object (dev) representing it.
      const char * path = udev_list_entry_get_name(curDevListEntry);

      // Check if the path is valid.
      if(path)
      {
        // Get a handle to the device.
        struct udev_device * device = udev_device_new_from_syspath(fUDEV.get(),
                                                                   path);

        // The dev path for the mouse.
        const char * devNode = udev_device_get_devnode(device);

        // Check the device node / path is good.
        if(devNode)
        {
          // Add it to the list of dev nodes.
          devNodes.push_back(devNode);
        }
      }
    }
  }

  // Return the list of dev nodes detected.
  return devNodes;
}

//==============================================================================
void IOManager::startInputMonitor(std::shared_ptr<SessionManager> sm)
{
  // Start monitoring the mice.
  for(auto mouse = fMice.begin(); mouse != fMice.end(); mouse++)
  {
    LUNA_LOG_INFO("Monitoring Mouse: " << mouse->first);
    mouse->second->start(sm);
  }

  // Start monitoring the keyboard.
  for(auto keyboard = fKeyboards.begin(); keyboard != fKeyboards.end();
      keyboard++)
  {
    LUNA_LOG_INFO("Monitoring Keyboard: " << keyboard->first);
    keyboard->second->start(sm);
  }

}

//==============================================================================
void IOManager::startHotplugMonitor(std::shared_ptr<SessionManager> sm)
{

}
