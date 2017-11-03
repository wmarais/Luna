


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
    class Display final
    {
    public:
      //------------------------------------------------------------------------
      // Initialise the member functions.
      //------------------------------------------------------------------------
      Display();

      //------------------------------------------------------------------------
      // Simply allow for function tracing in debug mode.
      //------------------------------------------------------------------------
      ~Display();

      //------------------------------------------------------------------------
      // The ID of the connector that the display is attached too.
      //------------------------------------------------------------------------
      uint32_t connectorID() const;

      //------------------------------------------------------------------------
      // Configure the display. It can be called at any time and can be used
      // to initially configure the display or to adjust the display
      // configuration while running / post initial configuration.
      //------------------------------------------------------------------------
      void configure(int fd, drmModeConnector * conn, drmModeRes * res,
                     const Luna::Common::Settings * settings);

      //------------------------------------------------------------------------
      // Return the physical width of the display area in mm.
      //------------------------------------------------------------------------
      uint32_t physicalWidth() const;

      //------------------------------------------------------------------------
      // Return the physical height fo the display area in mm.
      //------------------------------------------------------------------------
      uint32_t physicalHeight() const;

      //------------------------------------------------------------------------
      // The number of pixels per 1mm along the X axis of the display.
      //------------------------------------------------------------------------
      float dpmmX() const;

      //------------------------------------------------------------------------
      // The number of pixels per 1mm along the Y axis of the display.
      //------------------------------------------------------------------------
      float dpmmY() const;


      void setMode(int fd, drmModeConnector * conn);

    private:
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

      struct FrameBuffer
      {
        int fResult;
        int fFD;
        uint32_t fID;

        FrameBuffer(int fd, DumbBuffer *buffer);
        ~FrameBuffer();
      };

      //------------------------------------------------------------------------
      // There are a limited number of CRT Controllers (CRTCs) and their usage
      // needs to be tracked to ensure they are not accidentaly re-used.
      //------------------------------------------------------------------------
      static std::vector<uint32_t> fUsedCRTCs;

      uint32_t fConnectorID;
      uint32_t fEncoderID;

      uint32_t fCRTCID;

      //------------------------------------------------------------------------
      // The active display mode.
      //------------------------------------------------------------------------
      drmModeModeInfo fActiveMode;

      //------------------------------------------------------------------------
      // The frambuffer that stores the pointer to the front buffer. (This is
      // the buffer that is currently being rendered to the screen.)
      //------------------------------------------------------------------------
      std::unique_ptr<FrameBuffer> fFrontBufferFB;
      std::unique_ptr<DumbBuffer> fFrontBufferDB;

      //------------------------------------------------------------------------
      // The frambuffer that stores the pointer to the back buffer. (This is
      // the buffer that the composer is rendering too.)
      //------------------------------------------------------------------------
      std::unique_ptr<FrameBuffer> fBackBufferFB;
      std::unique_ptr<DumbBuffer> fBackBufferDB;

      //------------------------------------------------------------------------
      // The original CRTC configuration before the mode setting.
      //------------------------------------------------------------------------
      std::unique_ptr<drmModeCrtc, decltype(&drmModeFreeCrtc)> fSavedCRTC;

      //------------------------------------------------------------------------
      // The physical width and the height of the display in mm.
      //------------------------------------------------------------------------
      uint32_t fWidthMM, fHeightMM;

      //------------------------------------------------------------------------
      // Check if there is a currently configured Encoder and CRT Controller.
      //------------------------------------------------------------------------
      bool isEncoderAndCRTCValid(int fd, drmModeConnector *conn);

      //------------------------------------------------------------------------
      // Setup the Encoder and CRT Controller required for rendering to the
      // display.
      //------------------------------------------------------------------------
      void setupEncoderAndCRTC(int fd, drmModeConnector * conn, drmModeRes * res);

      //------------------------------------------------------------------------
      // Create the framebuffer.
      //------------------------------------------------------------------------
      void createBuffer(int fd, const drmModeModeInfo & mode, uint32_t bpp,
                        std::unique_ptr<DumbBuffer> & db,
                        std::unique_ptr<FrameBuffer> & fb);

      //------------------------------------------------------------------------
      // Determine the best Mode to run the display at. If the user explicitly
      // specified it in the settings for the display, then that will be use,
      // otherwise the highest refresh and screen area will be selected.
      //------------------------------------------------------------------------
      drmModeModeInfo getBestMode(drmModeModeInfoPtr modes, uint32_t numModes,
                                  const Common::Settings *settings);

    };
  }
}

#endif // LUNA_SERVER_DISPLAY_DISPLAY_HPP
