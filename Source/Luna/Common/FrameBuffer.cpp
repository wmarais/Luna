#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cstring>

#include "../../../Include/Luna/Common/Colour.hpp"
#include "../../../Include/Luna/Common/Debug/Exception.hpp"
#include "../../../Include/Luna/Common/FrameBuffer.hpp"

using namespace Luna;

/******************************************************************************/
FrameBuffer::FrameBuffer(uint32_t width, uint32_t height,
  const std::string & path) : fWidth(width), fHeight(height)
{
  /* Open the handle to the shared memory file. */
  fHandle = open(path.c_str(), O_RDWR);

  /* Check if the file was opened. */
  if(fHandle == -1)
  {
    LUNA_THROW_RUNTIME_ERROR("Unable to open frame buffer (" << path <<
                             "): " << strerror(errno));
  }
}

/******************************************************************************/
FrameBuffer::~FrameBuffer()
{

}

/******************************************************************************/
void FrameBuffer::resize(uint32_t width, uint32_t height)
{
  /* Calculate the required file size. */
  size_t newSize = width * height * sizeof (Colour);

  if(fPixels)
  {
    /* Calculate the old size. */
    size_t oldSize = fWidth * fHeight * sizeof(Colour);

    /* Check if this really need to resized or not. */
    if(oldSize == newSize)
      return;

    /* Set the new file size. */
    ftruncate(fHandle, static_cast<__off_t>(newSize));

    /* Resize the memory mapping. */
    auto ptr = mremap(fPixels, oldSize, newSize, MREMAP_MAYMOVE);

    /* Check if an error occured during the remap. */
    if(ptr == reinterpret_cast<void*>(-1))
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to remap memory because: " <<
                               strerror(errno));
    }

    /* Save the new pixel memory. */
    fPixels = reinterpret_cast<Colour*>(ptr);
  }
  else
  {
    /* Set the new file size. */
    ftruncate(fHandle, static_cast<__off_t>(newSize));

    /* The memory has not been mapped yet, so map it for the first time. */
    auto ptr = mmap(nullptr, newSize, PROT_READ | PROT_WRITE,
                    MAP_PRIVATE | MAP_POPULATE, fHandle, 0);

    /* Try to map the memory. */
    if(ptr == reinterpret_cast<void*>(-1))
    {
      LUNA_THROW_RUNTIME_ERROR("Failed to map memory because: " <<
                               strerror(errno));
    }

    /* Save the pointer to the pixel map. */
    fPixels = reinterpret_cast<Colour*>(ptr);
  }
}
