#include "../../../../Include/Luna/Server/Input/InputManager.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

//==============================================================================
InputManager::InputManager(const Settings * settings)
  /*: fUDEV(udev_new(), udev_unref)*/
{ 
  /*// Validate the settings parameter.
  assert(settings && "Settings object is null.");

  LUNA_LOG_INFO("Validating udev object.");
  if(!fUDEV)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to create UDEV device.");
  }

  // Setup the input devices.
  setupInput(settings);*/
}

//==============================================================================
std::vector<std::string> InputManager::scanDevNodes(const char * subSystem,
                                                 const char * property)
{
  // The list of dev nodes.
  std::vector<std::string> devNodes;

  /*// Create a list of input devices.
  std::unique_ptr<struct udev_enumerate, decltype(&udev_enumerate_unref)>
      enumerator(udev_enumerate_new(fUDEV.get()), udev_enumerate_unref);

  // Check if the enumerator object is created.
  if(!enumerator)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to create enumerator object.");
  }

  // Check if a particular sub system must be scanned.
  if(subSystem)
  {
    if(udev_enumerate_add_match_subsystem(enumerator.get(), subSystem) < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to set enumerate sub system.");
    }
  }

  // Check if a particular property must be matched.
  if(property)
  {
    // Only scan for Mice.
    if(udev_enumerate_add_match_property(enumerator.get(), property, "1") < 0)
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to set match property.");
    }
  }

  // Scan for the devices of interest.
  if(udev_enumerate_scan_devices(enumerator.get()) < 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to scan for devices.");
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
  }*/

  // Return the list of dev nodes detected.
  return devNodes;
}

//==============================================================================
void InputManager::setupInput(const Settings * settings)
{
  // Get the list of predefined inpyut devices.
  std::vector<std::string> mice = settings->getArray("input.mice");
  std::vector<std::string> keyboards = settings->getArray("input.keyboards");
  std::vector<std::string> joysticks = settings->getArray("input.joysticks");

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

    try
    {
      fMice[devNode] = std::make_unique<Mouse>(devNode);
    }
    catch(std::exception & ex)
    {
      // It is not a fatal if the input can not be opened, however we should
      // log it to inform the user.
      LUNA_LOG_ERROR("Failed to open mouse: " << devNode << ", because: " <<
                     ex.what());
    }
  }

  // Create all the keyboards.
  for(std::string devNode : keyboards)
  {
    LUNA_LOG_INFO("Creating Keyboard: " << devNode);

    try
    {
      fKeyboards[devNode] = std::make_unique<Joystick>(devNode);
    }
    catch(std::exception & ex)
    {
      // It is not a fatal if the input can not be opened, however we should
      // log it to inform the user.
      LUNA_LOG_ERROR("Failed to open keyboard: " << devNode << ", because: " <<
                     ex.what());
    }
  }

  // Create all the joysticks.
  for(std::string devNode : joysticks)
  {
    LUNA_LOG_INFO("Creating Joystick: " << devNode);
    try
    {
      fJoysticks[devNode] = std::make_unique<Joystick>(devNode);
    }
    catch(std::exception & ex)
    {
      // It is not a fatal if the input can not be opened, however we should
      // log it to inform the user.
      LUNA_LOG_ERROR("Failed to open joystick: " << devNode << ", because: " <<
                     ex.what());
    }
  }
}

//==============================================================================
void InputManager::startInputMonitor(std::shared_ptr<SessionManager> sm)
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
void InputManager::startHotplugMonitor(std::shared_ptr<SessionManager> sm)
{

}

//==============================================================================
void InputManager::addDevice(std::string devNode)
{

}

//==============================================================================
void InputManager::removeDevice(std::string devNode)
{

}

//==============================================================================
void InputManager::hotplugged(const std::string & devNode,
                              UDEV::InputDeviceTypes devType,
                              UDEV::DeviceActions action)
{
  LUNA_LOG_INFO("Managing device: " << devNode);
}







