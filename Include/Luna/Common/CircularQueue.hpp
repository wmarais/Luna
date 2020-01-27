#pragma once

#include <memory>
#include <optional>

namespace Luna
{
  template <typename T, size_t kMaxSize> class CircularQueue
  {
    std::unique_ptr<T[]> fElements;

    size_t fHead;

    size_t fTail;

    size_t fSize;

  public:
    CircularQueue() : fHead(0), fTail(0), fSize(0)
    {
      fElements = std::make_unique<T[]>(kMaxSize);
    }

    T *elements() { return fElements.get(); }

    /***********************************************************************/ /**
                                                                               * This function returns a reference to the head node in the circular
                                                                               * queue. The user can directly write to this node. This is done to reduce
                                                                               * the number of memory allocations and copies.
                                                                               *
                                                                               * @return  A reference to the head node in the queue or False if there is
                                                                               *          no space left.
                                                                               **************************************************************************/
    std::optional<T *> push()
    {
      /* Check if there is any space left. */
      if(fSize >= kMaxSize)
        return {};

      /* Get the element reference. */
      T *ref = &(fElements[fTail++]);

      /* Check if the tail pointer should be wrapped around. */
      if(fTail >= kMaxSize)
        fTail = 0;

      /* Increment the element count. */
      ++fSize;

      /* Return the reference to the element. */
      return ref;
    }

    /***********************************************************************/ /**
                                                                               * Returns a reference to the head node which contains a valid element or
                                                                               * False if the node is not valid.
                                                                               *
                                                                               * @return  The reference to the element.
                                                                               **************************************************************************/
    std::optional<T *> pop()
    {
      /* Check if there are any elements in the queue. */
      if(fSize <= 0)
        return {};

      /* Get the reference to the element. */
      T *ref = &(fElements[fHead++]);

      /* Check if the head should be wrapped around. */
      if(fHead >= kMaxSize)
        fHead = 0;

      /* Decrement the size. */
      --fSize;

      /* Return the reference. */
      return ref;
    }
  };
} // namespace Luna
