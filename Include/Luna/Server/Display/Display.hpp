#ifndef LUNA_SERVER_DISPLAY_DISPLAY_HPP
#define LUNA_SERVER_DISPLAY_DISPLAY_HPP

// TODO - Verify if crtc_id can be equal to 0? How do you valide it?
// TODO - Verify if a encoder_id can be equal to 0? How do you validate it?
// TODO - Verify if Encoder and CRTC are always paired.

#include "../../Common.hpp"
namespace Luna
{
  namespace Server
  {
    class Display
    {
    public:
      Display(std::unique_ptr<drmModeConnector,
              decltype(&drmModeFreeConnector)> & connector);

      //------------------------------------------------------------------------
      // Configure the display. It can be called at any time and can be used
      // to initially configure the display or to adjust the display
      // configuration while running / post initial configuration.
      //------------------------------------------------------------------------
      void configure(int fd, drmModeRes * res,
                     const Luna::Common::Settings * settings);

      //------------------------------------------------------------------------
      // Return the width of the display in mm.
      //------------------------------------------------------------------------
      uint32_t widthMM() const;

      //------------------------------------------------------------------------
      // Return the height fo the display in mm.
      //------------------------------------------------------------------------
      uint32_t heightMM() const;

      //------------------------------------------------------------------------
      // The number of pixels per 1mm along the X axis of the display.
      //------------------------------------------------------------------------
      float dpmmX() const;

      //------------------------------------------------------------------------
      // The number of pixels per 1mm along the Y axis of the display.
      //------------------------------------------------------------------------
      float dpmmY() const;

    private:

      //------------------------------------------------------------------------
      // There are a limited number of CRT Controllers (CRTCs) and their usage
      // needs to be tracked to ensure they are not accidentaly re-used.
      //------------------------------------------------------------------------
      static std::vector<uint32_t> fUsedCRTCs;

      class DumbBuffer
      {
        struct drm_mode_create_dumb fBuffer;
        int fResult;
        int fFD;
      public:
        DumbBuffer(int fd, uint32_t width, uint32_t height, uint32_t bpp);
        ~DumbBuffer();

        uint32_t width() const;
        uint32_t height() const;
        uint32_t bpp() const;
        uint32_t stride() const;
        uint32_t handle() const;
      };

      class FrameBuffer
      {
        int fResult;
        int fFD;
        uint32_t fID;
      public:
        FrameBuffer(int fd, DumbBuffer *buffer);
        ~FrameBuffer();
      };

      uint32_t fConnectorID;
      uint32_t fEncoderID;

      uint32_t fCRTCID;

      //------------------------------------------------------------------------
      // The dumb buffer associated with the display.
      //------------------------------------------------------------------------
      std::unique_ptr<DumbBuffer> fDumbBuffer;

      //------------------------------------------------------------------------
      // The frambuffer associated with the dumb buffer.
      //------------------------------------------------------------------------
      std::unique_ptr<FrameBuffer> fFrameBuffer;

      //------------------------------------------------------------------------
      // The width and the height of the display in mm.
      //------------------------------------------------------------------------
      uint32_t fWidthMM, fHeightMM;

      //------------------------------------------------------------------------
      // The physical connector that this display is attached too.
      //------------------------------------------------------------------------
      std::unique_ptr<drmModeConnector, decltype(&drmModeFreeConnector)>
        fConnector;

      //------------------------------------------------------------------------
      // Check if there is a currently configured Encoder and CRT Controller.
      //------------------------------------------------------------------------
      bool isEncoderAndCRTCValid(int fd);

      //------------------------------------------------------------------------
      // Setup the Encoder and CRT Controller required for rendering to the
      // display.
      //------------------------------------------------------------------------
      void setupEncoderAndCRTC(int fd, drmModeRes * res);

      //------------------------------------------------------------------------
      // Create the framebuffer.
      //------------------------------------------------------------------------
      void createBuffer(int fd, uint32_t width, uint32_t height, uint32_t bpp);

    };
  }
}

#endif // LUNA_SERVER_DISPLAY_DISPLAY_HPP
