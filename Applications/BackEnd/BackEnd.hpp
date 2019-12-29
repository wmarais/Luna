#pragma once

#include "../../Include/Luna/BackEnd.hpp"
#include "../../Include/Luna/Common.hpp"

namespace Luna::BE
{
  class BackEnd
  {
    std::shared_ptr<InputManager> fInputManager;
    std::shared_ptr<DisplayManager> fDisplayManager;

    std::unique_ptr<BE::UDEV> fUDEV;


    std::shared_ptr<Settings> fSettings;

    std::unique_ptr<InputManager> fIOManager;


    static const std::string kLogFilePath;
    static const std::string kSettingFilePath;

    // The value to assume of the detect.input.mice is not set.
    static const bool kDefaultScanForMice = true;

    void startInput();
    void stopInput();

  public:
    BackEnd();
    ~BackEnd();

    void run(int argc, char * argv[]);
  };
}
