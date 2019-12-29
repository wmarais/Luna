#pragma once

#include <map>
#include <memory>

#include "../UDEV.hpp"


namespace Luna
{
  class Settings;
}

namespace Luna::BE
{
  class VideoCard;

  class DisplayManager
  {
    //------------------------------------------------------------------------
    // The settings of the application. A copy is stored here so that the
    // displays can be reconfigured properly when new displays are attached.
    //------------------------------------------------------------------------
    std::shared_ptr<Settings> fSettings;

    //------------------------------------------------------------------------
    // The list of video cards being monitored.
    //------------------------------------------------------------------------
    std::map<std::string, std::unique_ptr<VideoCard>> fVideoCards;

  public:

    //------------------------------------------------------------------------
    // Store the settings pointer required to reconfigure the display.
    //------------------------------------------------------------------------
    DisplayManager(std::shared_ptr<Settings> settings);

    //------------------------------------------------------------------------
    // Invoked when a video card is added, removed or changed. If the device
    // action is "change" then the video card is rescanned for all the
    // connected display devices. This function is invoked atleast once at
    // the start of the application.
    //------------------------------------------------------------------------
    void changed(const std::string & devNode, UDEV::DeviceActions action);

    //------------------------------------------------------------------------
    // Perform a render test on all the video cards and displays.
    //------------------------------------------------------------------------
    void renderTest();
  };
}

