#pragma once

#include "Object.hpp"
#include "Renderer.hpp"

namespace Luna::FE
{
  class Window : public Object
  {
    /** The Unix Socket used to communicate with the session. */
    std::unique_ptr<UnixSocketClient> fSocket;


  protected:
    enum States
    {
      kNormalised,
      kMinimised,
      kMaximised,
      kClosed
    };

    std::atomic<States> fState;


    int64_t fBorderWidth;

    Colour fBorderColour;


    std::optional<Rect> topBorder(const Rect & area) const;
    std::optional<Rect> bottomBorder(const Rect& area) const;
    std::optional<Rect> leftBorder(const Rect & area) const;
    std::optional<Rect> rightBorder(const Rect & area)const;

    virtual void repaint(Renderer & renderer, const Rect & area) override;


  public:
    Window();
    virtual ~Window() override;

    bool isClosed() const;
  };
}
