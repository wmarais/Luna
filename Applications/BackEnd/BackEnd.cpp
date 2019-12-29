#include "BackEnd.hpp"

using namespace Luna;
using namespace Luna::BE;


const std::string BackEnd::kLogFilePath = "/var/log/luna/backend.log";
const std::string BackEnd::kSettingFilePath = "/etc/luna/backend.conf";

/******************************************************************************/
BackEnd::BackEnd()
{

}

/******************************************************************************/
BackEnd::~BackEnd()
{

}

/******************************************************************************/
void BackEnd::startInput()
{
  // Create a new IO manager.
  //fIOManager = std::unique_ptr<IOManager>(fSettings);




}

/******************************************************************************/
void BackEnd::stopInput()
{

}

/******************************************************************************/
void BackEnd::run(int argc, char * argv[])
{
  LUNA_UNUSED_PARAM(argc);
  LUNA_UNUSED_PARAM(argv);

  // Parse the settings file.
  LUNA_LOG_INFO("Parsing settings: " << kSettingFilePath);
  fSettings = std::make_shared<Settings>(kSettingFilePath);


  // Create the input manager.
  fInputManager = std::make_shared<InputManager>(fSettings.get());

  // Create the display manager.
  fDisplayManager = std::make_shared<DisplayManager>(fSettings);

  // Create the udev interface.
  fUDEV = std::make_unique<UDEV>();

  // Check if the hotplug monitor should be running.
  if(fSettings->getValue("hotplug", true))
  {
    // Start monitoring for hotplug events on UDEV.
    fUDEV->startMonitor(fInputManager, fDisplayManager);
  }

  // Scan for the devices allready plugged in.
  fUDEV->scan(fInputManager.get(), fDisplayManager.get());

  // Perform a render test.
  fDisplayManager->renderTest();


  /*// Create the session manager.
  LUNA_LOG_INFO("Creating the session manager.");
  std::shared_ptr<SessionManager> sm = std::make_shared<SessionManager>();

  // Starting the session manager.
  LUNA_LOG_INFO("Starting the session manager.");
  sm->start();

  // Starting the io manager.
  LUNA_LOG_INFO("Creating the IO Manager.");
  std::unique_ptr<InputManager> ioman =
      std::make_unique<InputManager>(fSettings.get());*/


  // Create the display manager.
}
