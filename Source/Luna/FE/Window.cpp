#include "../../../Include/Luna/FE/Window.hpp"

using namespace Luna;
using namespace Luna::FE;

/******************************************************************************/
Window::Window() : fState(kNormalised)
{
  /* Create the socket connection to the session. */
  fSocket = std::make_unique<UnixSocketClient>(Directory::sockPath());

  /* Open the session's frame buffer. */
}

/******************************************************************************/
Window::~Window()
{

}

/******************************************************************************/
std::optional<Rect> Window::topBorder(const Rect & area) const
{
  /* If the border is 0 pixels wide, then there is not going to be anything to
   * paint. */
  if(fBorderWidth <= 0)
    return {};

  /* Calculate the rectangle that will form the top border. */
  Rect border(fScreenRect.x0(), fScreenRect.y0(), fScreenRect.width(),
              fBorderWidth);

  /* Calculate the intersection between the border and the render area. */
  return area.intersect(border);
}

/******************************************************************************/
std::optional<Rect> Window::bottomBorder(const Rect & area) const
{
  /* If the border is 0 pixels wide, then there is not going to be anything to
   * paint. */
  if(fBorderWidth <= 0)
    return {};

  /* Calculate the area that will for the bottom border. */
  Rect border(fScreenRect.x0(), fScreenRect.y1(), fScreenRect.width(),
              -fBorderWidth);

  /* Calculate the intersection between the border and the render area. */
  return area.intersect(border);
}

/******************************************************************************/
std::optional<Rect> Window::leftBorder(const Rect & area) const
{
  /* If the border is 0 pixels wide, then there is not going to be anything to
   * paint. */
  if(fBorderWidth <= 0)
    return {};

  /* Calculate the area that will for the left border. */
  Rect border(fScreenRect.x0(), fScreenRect.y0(), fBorderWidth,
              fScreenRect.height());

  /* Calculate the intersection between the border and the render area. */
  return area.intersect(border);
}

/******************************************************************************/
std::optional<Rect> Window::rightBorder(const Rect & area) const
{
  /* If the border is 0 pixels wide, then there is not going to be anything to
   * paint. */
  if(fBorderWidth <= 0)
    return {};

  /* Calculate the area that will for the right border. */
  Rect border(fScreenRect.x1(), fScreenRect.y0(), -fBorderWidth,
              fScreenRect.height());

  /* Calculate the intersection between the border and the render area. */
  return area.intersect(border);
}

/******************************************************************************/
void Window::repaint(Renderer & renderer, const Rect & area)
{
  /* Paint the top border. */
  if(auto border = topBorder(area))
    renderer.fill(border.value(), fBorderColour);

  /* Paint the bottom border. */
  if(auto border = bottomBorder(area))
    renderer.fill(border.value(), fBorderColour);

  /* Paint the left border. */
  if(auto border = leftBorder(area))
    renderer.fill(border.value(), fBorderColour);

  /* Paint the right border. */
  if(auto border = rightBorder(area))
    renderer.fill(border.value(), fBorderColour);

  /* Repaint the window header. */
}

/******************************************************************************/
bool Window::isClosed() const
{
  return fState == kClosed;
}

