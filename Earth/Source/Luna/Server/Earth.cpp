#include "../../../Include/Luna/Server/Earth.hpp"

using namespace Luna::Common;
using namespace Luna::Server;


const std::string Earth::kLogFilePath = "/var/log/luna/earth.log";
const std::string Earth::kSettingFilePath = "/etc/luna/earth.conf";

//==============================================================================
Earth::Earth()
{

}

//==============================================================================
Earth::~Earth()
{

}

//==============================================================================
void Earth::startInput()
{
  // Create a new IO manager.
  //fIOManager = std::unique_ptr<IOManager>(fSettings);




}

//==============================================================================
void Earth::stopInput()
{

}

//==============================================================================
void Earth::run(int argc, char * argv[])
{
  // Create the log.
  Log::create();

  // Parse the settings file.
  LUNA_LOG_INFO("Parsing settings: " << kSettingFilePath);
  fSettings = std::make_unique<Settings>(kSettingFilePath);

  /*// Create the session manager.
  LUNA_LOG_INFO("Creating the session manager.");
  std::shared_ptr<SessionManager> sm = std::make_shared<SessionManager>();

  // Starting the session manager.
  LUNA_LOG_INFO("Starting the session manager.");
  sm->start();*/

  // Starting the io manager.
  LUNA_LOG_INFO("Creating the IO Manager.");
  std::unique_ptr<InputManager> ioman =
      std::make_unique<InputManager>(fSettings.get());



  // Destroy the log.
  Log::destroy();
}

/*
 *   try
  {
    IOManager ioManager;

    // Create a keyboard input.
    std::unique_ptr<Input> keyboard =
        std::make_unique<Keyboard>(kLUNA_EARTH_CONFIG_KEYBOARD);

    // Create a mouse input.
    std::unique_ptr<Input> mouse =
        std::make_unique<Mouse>(kLUNA_EARTH_CONFIG_MOUSE);

    // Start monitoring the keyboard.
    keyboard->start(sm);

    // Start monitoring the mouse.
    mouse->start(sm);













    // Stop monitoring the mouse.
    mouse->stop();

    // Stop monitoring the keyboard.
    keyboard->stop();
  }
  catch(const char * ex)
  {
    std::cout << "Exception: " << ex << std::endl;
  }
    */
