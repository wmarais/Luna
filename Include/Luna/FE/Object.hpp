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
    /** The parent of the object. This can not be a shared_ptr otherwise it will
     * be a cyclic reference and a memory leak will be created. */
    Object * fParent;

    /** The children of the object. */
    std::vector<std::shared_ptr<Object>> fChildren;

    /** The bounding rectangle of the object in screen coordinates. */
    Rect<int64_t> fScreenRect;

    /***********************************************************************//**
     * GUI object specific repaint function to be implemented by each GUI
     * object.
     *
     * @param renderer    The renderer to use for painting.
     * @param screenArea  The screenArea that needs to be repainted.
     **************************************************************************/
    virtual void repaint(Renderer & renderer,
                         const Rect<int64_t> & screenArea) = 0;

  public:
    /***********************************************************************//**
     * Create a new GUI object and set it's parent.
     *
     * @param parent  The parent of the object.
     **************************************************************************/
    Object(Object * parent = nullptr);

    /***********************************************************************//**
     * Destroy the GUI object and clear all children's reference to it.
     **************************************************************************/
    virtual ~Object();

    /***********************************************************************//**
     * Add a child oject to this object. This object will become the parent of
     * the child node.
     *
     * @param child The new child to add to this object.
     **************************************************************************/
    void addChild(std::shared_ptr<Object> child);

    /***********************************************************************//**
     * Return the handle to the object's parent.
     *
     * @return  The pointer to the object's parent.
     **************************************************************************/
    Object * getParent();

    /***********************************************************************//**
     * Repaint the object. The object should check if they fall within the
     * repaint area before rendering. There is no point in repainting anything
     * that does not require it.
     *
     * @param renderer  The renderer to use for repainting.
     * @param area      The area that must be repainted in screen coordinates.
     **************************************************************************/
    void render(Renderer & renderer, const Rect<int64_t> & screenArea);
  };
}
