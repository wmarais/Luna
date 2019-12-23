#ifndef LUNA_SERVER_EARTH_HPP
#define LUNA_SERVER_EARTH_HPP

#include "../../../../Include/Luna/Common.hpp"
#include "../../../../Include/Luna/Server.hpp"

namespace Luna
{
  namespace Server
  {
    class Earth
    {
      std::shared_ptr<Luna::Server::InputManager> fInputManager;
      std::shared_ptr<Luna::Server::DisplayManager> fDisplayManager;

      std::unique_ptr<Luna::Server::UDEV> fUDEV;


      std::shared_ptr<Luna::Common::Settings> fSettings;

      std::unique_ptr<InputManager> fIOManager;


      static const std::string kLogFilePath;
      static const std::string kSettingFilePath;

      // The value to assume of the detect.input.mice is not set.
      static const bool kDefaultScanForMice = true;

      void startInput();
      void stopInput();

    public:
      Earth();
      ~Earth();

      void run(int argc, char * argv[]);
    };
  }
}


#endif // LUNA_SERVER_EARTH_HPP
