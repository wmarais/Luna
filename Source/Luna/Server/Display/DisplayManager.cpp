#include "../../../../Include/Luna/Server.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

//==============================================================================
DisplayManager::DisplayManager(std::shared_ptr<Settings> settings) :
  fSettings(settings) {}

//==============================================================================
void DisplayManager::changed(const std::string & devNode,
                                  UDEV::DeviceActions action)
{
  // Try to find the video card in the map of video cards.
  auto iter = fVideoCards.find(devNode);

  // Check if the device is already in the list.
  if(iter != fVideoCards.end())
  {
    // Check if the device has been removed.
    if(action == UDEV::kRemoveDevice)
    {
      // Remove the device from the list.
      fVideoCards.erase(iter);
    }
    // Check if the device changed.
    else if(action == UDEV::kChangeDevice)
    {
      // Reconfigure the displays.
      iter->second->configure(fSettings.get());
    }
  }
  else
  {
    // Check if the device has been added or changed.
    if(action == UDEV::kAddDevice || action == UDEV::kChangeDevice)
    {
      try
      {
        // Create the Video Card object.
        std::unique_ptr<VideoCard> videoCard =
            std::make_unique<VideoCard>(devNode);

        // Configure the video card.
        videoCard->configure(fSettings.get());

        // Add the video card to the map.
        fVideoCards[devNode] = std::move(videoCard);
      }
      catch(const std::exception & ex)
      {
        LUNA_LOG_EXCEPTION("Video card (" << devNode << ") ignored because " <<
                           "of exception.", ex);
      }
    }
  }
}
