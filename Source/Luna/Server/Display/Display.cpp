#include "../../../../Include/Luna/Server/Display/Display.hpp"

using namespace Luna::Common;
using namespace Luna::Server;

std::vector<uint32_t> Display::fUsedCRTCs;

//==============================================================================
Display::Display(std::unique_ptr<drmModeConnector,
  decltype(&drmModeFreeConnector)> & connector) : fConnector(nullptr, nullptr),
  fCRTCID(0), fEncoderID(0)
{
  // Tranfer ownership of the connector to this display object.
  fConnector = std::move(connector);

  // Atleast one mode should be available.
  if(fConnector->count_modes <= 0)
  {
    LUNA_THROW_RUNTIME_ERROR("No modes available for display.");
  }
}

//==============================================================================
void Display::configure(int fd, drmModeRes * res, const Settings * settings)
{

  // Setup a suitable Encoder and CRT Controller.
  setupEncoderAndCRTC(fd, res);

}
//==============================================================================
bool Display::isEncoderAndCRTCValid(int fd)
{
  // The status of the function.
  bool status = false;

  // Check if a valid Encoder is allready attached to the connector.
  if(fEncoderID > 0 && fConnector->encoder_id == fEncoderID)
  {
    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, fEncoderID), drmModeFreeEncoder);

    // Check if the CRT Controller is valid.
    if(encoder && fCRTCID >= 0 && encoder->crtc_id == fCRTCID)
    {
      // The Encoder and CRTC is valid.
      status = true;
    }
  }

  // Return the status of the function.
  return status;
}

//==============================================================================
void Display::setupEncoderAndCRTC(int fd, drmModeRes * res)
{
  // Before trying to find a suitabled Encoder and CRTC, check if the current
  // ones are valid. (This should be true for most reconfigurations.)
  if(isEncoderAndCRTCValid(fd))
  {
    // Nothing left to do, just return.
    return;
  }

  // Check if an ecoder is allready attached to the connector.
  if(fConnector->encoder_id)
  {
    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, fConnector->encoder_id),
                drmModeFreeEncoder);

    // Check if a valid encoder was returned.
    if(encoder)
    {
      // Check if the CRT Controller is allready in use.
      if(std::find(fUsedCRTCs.begin(), fUsedCRTCs.end(), encoder->crtc_id) !=
         fUsedCRTCs.end())
      {
        // The encoder and CRTC is both valid and allready connected. Nothing
        // else left to do.
        return;
      }
    }
  }

  // Not suitable Encoder or CRTC has been found yet. Iterate through all the
  // possible encoders and CRTCs and find one not allready in use.
  for(int i = 0; i < fConnector->count_encoders; i++)
  {
    // Get the handle to the encoder.
    std::unique_ptr<drmModeEncoder, decltype(&drmModeFreeEncoder)>
        encoder(drmModeGetEncoder(fd, fConnector->encoder_id),
                drmModeFreeEncoder);

    // Check if the encoder was retrieved.
    if(!encoder)
    {
      LUNA_LOG_ERROR("Failed to retrieve encoder " << i << " because: " <<
                     strerror(errno));
      continue;
    }

    // Iterate trhough all gloab CRTCs.
    for(int j = 0; j < res->count_crtcs; ++j)
    {
      // Check whether this CRTC works with the encoder. This crazyness brought
      // to you by:
      // https://dvdhrm.wordpress.com/2012/09/13/linux-drm-mode-setting-api/
      if (!(encoder->possible_crtcs & (1 << j)))
      {
        continue;
      }

      // Check if the CRTC is allready in use.
      if(std::find(fUsedCRTCs.begin(), fUsedCRTCs.end(), res->crtcs[j]) ==
         fUsedCRTCs.end())
      {
        // Save the Encoder and CRTC ID.
        fEncoderID = encoder->encoder_id;
        fCRTCID = res->crtcs[j];
      }
    }
  }

  LUNA_THROW_RUNTIME_ERROR("Unable to find a suitable CRTC for the connector.");
}

//==============================================================================
void Display::createBuffer(int fd, uint32_t width, uint32_t height,
                           uint32_t bpp)
{
  LUNA_TRACE_FUNCTION();

  LUNA_LOG_DEBUG("Creating dumb buffer.");
  fDumbBuffer = std::make_unique<DumbBuffer>(fd, width, height, bpp);

  LUNA_LOG_DEBUG("Creating frame buffer.");
  fFrameBuffer = std::make_unique<FrameBuffer>(fd, fDumbBuffer.get());

  LUNA_LOG_DEBUG("Preparing buffer for memory map.");
  struct drm_mode_map_dumb mreq;
  memset(&mreq, 0, sizeof(mreq));
  mreq.handle = fDumbBuffer->handle();
  if(drmIoctl(fd, DRM_IOCTL_MODE_MAP_DUMB, &mreq) < 0)
  {
    LUNA_THROW_RUNTIME_ERROR("Failed to map dump buffer because: " <<
                             strerror(errno))
  }



}

//==============================================================================
uint32_t Display::widthMM() const
{
  return fWidthMM;
}

//==============================================================================
uint32_t Display::heightMM() const
{
  return fHeightMM;
}

//==============================================================================
 float Display::dpmmX() const
{
  LUNA_TRACE_FUNCTION();

  // TODO - DECIDE WHAT TO DO HERE.
  // The calculated dpmm.
  float dpmm = 0; //static_cast<float>(fActiveMode.fWidthPx);

  // Check for a possible divide-by-zero error.
  if(fWidthMM > 0)
  {
    // Finish the dpmm calculation.
    dpmm /= static_cast<float>(fWidthMM);
  }
  else
  {
    LUNA_LOG_WARN("The display (" << fConnectorID << ") has no valid " <<
                  "physical width information.");
  }

  // Return the dpmm.
  return dpmm;
}

//==============================================================================
float Display::dpmmY() const
{
  LUNA_TRACE_FUNCTION();

  // TODO - DECIDE WHAT TO DO HERE.
  // The calculated dpmm.
  float dpmm = 0; //static_cast<float>(fActiveMode.fHeightPx);

  // Check for a possible divide-by-zero error.
  if(fWidthMM > 0)
  {
    // Finish the dpmm calculation.
    dpmm /= static_cast<float>(fHeightMM);
  }
  else
  {
    LUNA_LOG_WARN("The display (" << fConnectorID << ") has no valid " <<
                  "physical height information.");
  }

  // Return the dpmm.
  return dpmm;
}

//##############################################################################
//                                DUMB BUFFER
//##############################################################################
Display::DumbBuffer::DumbBuffer(int fd, uint32_t width, uint32_t height,
                                uint32_t bpp) : fResult(-1), fFD(fd)
{
  LUNA_TRACE_FUNCTION();

  // Clear the memory.
  memset(&fBuffer, 0, sizeof(fBuffer));

  // Set the desired fields.
  fBuffer.width = width;
  fBuffer.height = height;
  fBuffer.bpp = bpp;

  // Create the buffer.
  fResult = drmIoctl(fFD, DRM_IOCTL_MODE_CREATE_DUMB, &fBuffer);
}

//==============================================================================
Display::DumbBuffer::~DumbBuffer()
{
  LUNA_TRACE_FUNCTION();

  // Check if the buffer was created.
  if(fResult >= 0)
  {
    // Setup the delete buffer.
    struct drm_mode_destroy_dumb deleteBuffer;
    memset(&deleteBuffer, 0, sizeof(deleteBuffer));
    deleteBuffer.handle = fBuffer.handle;

    // Delete the buffer.
    drmIoctl(fFD, DRM_IOCTL_MODE_DESTROY_DUMB, &deleteBuffer);
  }
}

//==============================================================================
uint32_t Display::DumbBuffer::width() const
{
  return fBuffer.width;
}

//==============================================================================
uint32_t Display::DumbBuffer::height() const
{
  return fBuffer.height;
}

//==============================================================================
uint32_t Display::DumbBuffer::bpp() const
{
  return fBuffer.bpp;
}

//==============================================================================
uint32_t Display::DumbBuffer::stride() const
{
  return fBuffer.pitch;
}

//==============================================================================
uint32_t Display::DumbBuffer::handle() const
{
  return fBuffer.handle;
}

//##############################################################################
//                                FRAME BUFFER
//##############################################################################
Display::FrameBuffer::FrameBuffer(int fd, DumbBuffer * buffer) : fFD(fd)
{
  fResult = drmModeAddFB(fFD, buffer->width(), buffer->height(), 24,
              buffer->bpp(), buffer->stride(), buffer->handle(), &fID);

}

//==============================================================================
Display::FrameBuffer::~FrameBuffer()
{
  if(fResult >= 0)
  {
    drmModeRmFB(fFD, fID);
  }
}
