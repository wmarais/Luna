#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "Renderer.hpp"

#include "../Common/Rect.hpp"

namespace Luna::FE
{
  class Object
  {
  protected:
    /* The parent of the object. */
    std::shared_ptr<Object> fParent;

    /* The children of the object. */
    std::vector<std::shared_ptr<Object>> fChildren;

    /* The bounding rectangle of the object. */
    Rect<int64_t> fRect;

  protected:

  public:
    Object();
    virtual ~Object();

    /***********************************************************************//**
     * Provide a mechanism for chaning the parent of a child object. This is the
     * only mechanism available for this purpose and is done so on purpose to
     * ensure that the integrity of hierachy. Any other method of chaning
     * reparenting an object is not supported and should reported as bug.
     *
     * @param parent  The new parent of the child object.
     * @param child   The child object whose parent must be updated.
     **************************************************************************/
    static void setParent(std::shared_ptr<Object> parent,
                          std::shared_ptr<Object> child);

    /***********************************************************************//**
     * Return the handle to the object's parent. The pointer must still be
     * checked for validity before being used.
     *
     * @return  The pointer to the object's parent.
     **************************************************************************/
    std::shared_ptr<Object> getParent();

    /***********************************************************************//**
     * Repaint the object. The object should check if they fall within the
     * repaint area before rendering. There is no point in repainting anything
     * that does not require it.
     *
     * @param renderer  The renderer to use for repainting.
     * @param area      The area that must be repainted. This is the area
     *                  relative to the parent object.
     **************************************************************************/
    virtual void render(std::shared_ptr<Renderer> renderer,
                        const Rect<int64_t> & area) = 0;
  };
}
