#ifndef LUNA_DISPLAY_VIDEO_CARD_HPP
#define LUNA_DISPLAY_VIDEO_CARD_HPP

#include "../../Common.hpp"
#include "Display.hpp"

namespace Luna
{
  namespace Server
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

      //------------------------------------------------------------------------
      // The thread used for rendering the framebuffer to the screen.
      //------------------------------------------------------------------------
      std::unique_ptr<std::thread> fRenderThread;

      //------------------------------------------------------------------------
      // The flag to keep track of whether the rendering loop is running.
      //------------------------------------------------------------------------
      std::atomic_bool fRendering;

      //------------------------------------------------------------------------
      // Start rendering to the displays.
      //------------------------------------------------------------------------
      void startRendering();

      //------------------------------------------------------------------------
      // Stop rendering to the attached displays.
      //------------------------------------------------------------------------
      void stopRendering();

      //------------------------------------------------------------------------
      // The rendering thread's function.
      //------------------------------------------------------------------------
      void render();

      //------------------------------------------------------------------------
      // Set the the video modes.
      //------------------------------------------------------------------------
      void setModes();

    public:
      VideoCard(const std::string & devNode);
      ~VideoCard();

      //------------------------------------------------------------------------
      // Detect all the active connectors and configure the modes for all the
      // attached displays. Can be called at any time
      //------------------------------------------------------------------------
      void configure(const Luna::Common::Settings * settings);

      void renderTest();
    };
  }
}

#endif // LUNA_DISPLAY_VIDEO_CARD_HPP
