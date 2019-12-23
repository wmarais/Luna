#pragma once

#include <memory>
#include <vector>
#include <algorithm>
#include <vulkan/vulkan.h>

namespace Luna::FE
{
  class Object
  {
    /* The parent of the object. */
    std::shared_ptr<Object> fParent;

    /* The children of the object. */
    std::vector<std::shared_ptr<Object>> fChildren;

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

    virtual void render() = 0;




  };
}
