#pragma once

#include <map>

#include "Display.hpp"

namespace Luna::BE
{

  class VideoCard
  {
    int fFD;

    //------------------------------------------------------------------------
    // The map of connectors and associated displays connected to this video
    // card. The key value is the video card connector and the value is the
    // associated display information.
    //------------------------------------------------------------------------
    std::map<int, std::unique_ptr<Display>> fDisplays;

  public:
    VideoCard(const std::string & devNode);
    ~VideoCard();

    //------------------------------------------------------------------------
    // Detect all the active connectors and configure the modes for all the
    // attached displays. Can be called at any time
    //------------------------------------------------------------------------
    void configure(const Settings * settings);

    void setModes();

    void renderTest();
  };
}
