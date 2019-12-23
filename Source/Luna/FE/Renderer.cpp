#include "../../../Include/Luna/FE/Renderer.hpp"

#include <png++/png.hpp>

using namespace Luna;
using namespace Luna::FE;

/******************************************************************************/
Renderer::Renderer(int64_t width, int64_t height) : fWidth(width),
  fHeight(height)
{
  fPixels.resize(static_cast<size_t>(fWidth * fHeight));
}

/******************************************************************************/
Renderer::~Renderer()
{

}

/******************************************************************************/
Rect Renderer::rect() const
{
  return Rect(0, 0, fWidth, fHeight);
}

/******************************************************************************/
void Renderer::fill(const Rect & rect, const Colour & colour)
{
  for(auto y = rect.y0(); y < rect.y1(); y++)
  {
    for(auto x = rect.x0(); x < rect.x1(); x++)
      fPixels[static_cast<size_t>(y * fWidth + x)] = colour;
  }
}

/******************************************************************************/
void Renderer::savePNG(const std::string & fileName)
{
  /* Create the image. */
  png::image<png::rgba_pixel> image(static_cast<png::uint_32>(fWidth),
                                    static_cast<png::uint_32>(fHeight));

  /* Load the iamge data. Converting from BGRA to RGBA. */
  for(size_t y = 0; y < static_cast<size_t>(fHeight); y++)
  {
    for(size_t x = 0; x < static_cast<size_t>(fWidth); x++)
    {
      Colour pixel = fPixels[y * static_cast<size_t>(fWidth) + x];

      image[y][x] = png::rgba_pixel(pixel.red(), pixel.green(), pixel.blue(),
                                    pixel.alpha());
    }
  }

  /* Write the image now. */
  image.write(fileName);
}


