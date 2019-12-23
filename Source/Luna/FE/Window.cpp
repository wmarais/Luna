#include "../../../Include/Luna/FE/Window.hpp"

using namespace Luna::FE;

/******************************************************************************/
Window::Window()
{

}

/******************************************************************************/
Window::~Window()
{

}

/******************************************************************************/
void Window::render(std::shared_ptr<Renderer> renderer,
                    const Rect<int64_t> & area)
{
  /* Check if there is anything to repaint. */
  //auto subArea = area.intersects(fRect);




}

