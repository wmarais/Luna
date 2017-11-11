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
  try
  {
    // Try to find the video card in the map of video cards.
    auto iter = fVideoCards.find(devNode);

    // Check if the device is already in the list.
    if(iter != fVideoCards.end())
    {
      // Delete the device. It seems that some DRM resources are no updated
      // properly and it is best to close the file and re-open it.
      fVideoCards.erase(iter);
    }

    // Check if the action is Add or Changed.
    if(action == UDEV::kAddDevice || action == UDEV::kChangeDevice)
    {
      // Create the Video Card object.
      std::unique_ptr<VideoCard> videoCard =
          std::make_unique<VideoCard>(devNode);

      // Configure the video card.
      videoCard->configure(fSettings.get());

      // Add the video card to the map.
      fVideoCards[devNode] = std::move(videoCard);
    }
  }
  catch(const std::exception & ex)
  {
    LUNA_LOG_EXCEPTION(ex);
    LUNA_LOG_ERROR("Video card (" << devNode << ") ignored.");
  }
}

//==============================================================================
void DisplayManager::renderTest()
{
  for(std::map<std::string, std::unique_ptr<VideoCard>>::iterator iter =
      fVideoCards.begin(); iter != fVideoCards.end(); iter++)
  {
    iter->second->renderTest();
  }
}
