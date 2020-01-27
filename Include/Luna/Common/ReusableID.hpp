#pragma once

#include <memory>
#include <optional>

namespace Luna
{
  /**
   * Produce a limited amount of unique IDs used to map temporary correlate
   * information such as local RPC calls to the remote RPC results.
   *
   * In order to identify which result maps to which RPC upon completion, an ID
   * is required. Since RPCs can be long lived it is possible that the ID number
   * used can wrap around and an active ID may come up for reuse. In order to
   * ensure uniqueness of the ID, the active IDs must be tracked and not
   * reallocated until released. This tracking / management behaviour has the
   * benifit of artificially limiting the maximum number of concurrent RPCs,
   * removing the threat of runaway RPC spawning overwhelming the local and
   * remote RPC endpoints.
   */
  template <typename T> class ReusableID final
  {
    /** The states of each ID. */
    enum States
    {
      /** The ID is currently unsused and can be allocated. */
      kUnused,

      /** The ID is currently in use and not available for allocation. */
      kUsed
    };

    /** The maximum number of concurrently active IDs / pool size. */
    const T kPoolSize;

    /** The next ID that will be allocated if available. */
    T fNextID;

    /** The current usage state of the IDs. */
    std::unique_ptr<States[]> fIDPool;

  public:
    /**
     * Create and initialise the ID pool to mark all IDs as unused / available.
     *
     * @param poolSize  The size of the ID pool, i.e. the maximum number of IDs
     *                  which can ever be concurently allocated.
     */
    ReusableID(T poolSize) : kPoolSize(poolSize), fNextID(0)
    {
      /* Create the memory required to track the states. */
      fIDPool = std::make_unique<States[]>(kPoolSize);

      /* Initialised the default state to all IDs unused. */
      for(T i = 0; i < kPoolSize; i++)
        fIDPool[i] = kUnused;
    }

    /**
     * Get the next available ID in the pool and mark it as as used /
     * unvailable. Since it is expected behaviour that all the IDs may be
     * consumed, an optional<> is returned which will be set to false if no IDs
     * are presently available.
     *
     * @return  An optional object which is either False (if no IDs are
     *          available), or True with the ID set as the value() of the
     *          returned optional object.
     */
    std::optional<T> next()
    {
      /* Save the search start point. */
      auto startID = fNextID;

      /* Keep looping until the next free ID is found. */
      while(fIDPool[fNextID] == kUsed)
      {
        /* Progress to the next ID. */
        ++fNextID;

        /* Check if the ID should be wrapped around. */
        if(fNextID >= kPoolSize)
          fNextID = 0;

        /* Check if we are back to where we started searching. In that case
         * there are no more free IDs. */
        if(fNextID == startID)
          return {};
      }

      /* Mark the active ID as used. */
      fIDPool[fNextID] = kUsed;

      /* Return the ID to use. */
      return fNextID;
    }

    /**
     * Make the specified ID available for reuse. All IDs which has been
     * allocated by next() must be released(), otherwise all IDs will
     * eventually be spent and next() will not return any IDs.
     *
     * @param id  The id to be released back in the available ID pool.
     */
    void release(const T &id)
    {
      /* Mark the specified ID as available. */
      fIDPool[id] = kUnused;
    }
  };
} // namespace Luna
