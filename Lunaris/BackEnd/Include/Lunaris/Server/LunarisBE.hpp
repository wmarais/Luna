#pragma once

#include "../../../../Include/Luna/Common.hpp"
#include "../../../../Include/Luna/BackEnd.hpp"

namespace Lunaris
{
  class BackEnd
  {
    std::shared_ptr<Luna::BE::InputManager> fInputManager;
    std::shared_ptr<Luna::BE::DisplayManager> fDisplayManager;

    std::unique_ptr<Luna::BE::UDEV> fUDEV;


    std::shared_ptr<Luna::Settings> fSettings;

    std::unique_ptr<Luna::BE::InputManager> fIOManager;


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

