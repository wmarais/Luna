//#pragma once

//#include <string>
//#include <map>
//#include <vector>
//#include <limits>
//#include <memory>
//#include <cstring>
//#include <memory.h>
//#include <tuple>
//#include <mutex>

//#include "Debug/Exception.hpp"
//#include "CircularQueue.hpp"
//#include "Serialiser.hpp"
//#include "Network/Socket.hpp"

//namespace Luna
//{
//  class RPC
//  {
//    static const uint16_t kStatBitError = 0x8000;
//    static const uint16_t kStatBitSync  = 0x0001;

//    static const size_t kQueueLen = 1024;

//    /*#####################################################################*//**
//     * A class that wraps the strong type call handler. This allows it to be
//     * placed into a single container, while retain strong type safety.
//     *########################################################################*/
//    struct Handler
//    {
//      /*********************************************************************//**
//       * Provide a virtual constructor so inherited object will destruct
//       * correctly.
//       ************************************************************************/
//      virtual ~Handler() = 0;

//      /*********************************************************************//**
//       * Execute the handler function with the supplied call data packet. This
//       * function is implemented by the FunctionHandler and
//       * MemberFunctionHandler classes to unpack the buffer, populate the
//       * arguments and execute the associated functions.
//       *
//       * @param buffer  The buffer that contains the call information.
//       * @param result  The packed result of the handler call.
//       ************************************************************************/
//      virtual void execute(const std::vector<uint8_t> & buffer,
//                           std::vector<uint8_t> & result) = 0;
//    };

//    /*#####################################################################*//**
//     * A class that can be used to invoke member functions of an object. This
//     * allows classes / objects to be used as handlers.
//     *########################################################################*/
//    template <typename R, typename O, typename ... T>
//    struct MemberFunctionHandler : public Handler
//    {
//      /** The object that is used for execution. */
//      O fObj;

//      /** The pointer to the function. */
//      R (*fFuncPtr)(T...args);

//      /** The ID of the call as read from the buffer. */
//      uint32_t fID;

//      /** The status field as read from the buffer. */
//      uint32_t fStat;

//      /** The name of the call as extracted from the buffer. */
//      std::string fName;

//      /** The arguments of the function. */
//      std::tuple<T...> fArgs;

//      /* Result of the function call. */
//      R fResult;

//      /*********************************************************************//**
//       * Create the new member function handler.
//       *
//       * @param funcPtr The pointer to the member function.
//       * @param obj     The pointer to the object.
//       ************************************************************************/
//      MemberFunctionHandler(R (*funcPtr)(T...args), O obj) : fObj(obj),
//        fFuncPtr(funcPtr) {}

//      /*********************************************************************//**
//       * Execute the member function handler and pack the result of the
//       * execution in to result.
//       ************************************************************************/
//      void execute(const std::vector<uint8_t> & buffer,
//                   std::vector<uint8_t> & result)
//      {
//        /* Deserialise the fields. */
//        size_t offset = 0;
//        offset += Deserialise(buffer, offset, fID);
//        offset += Deserialise(buffer, offset, fStat);
//        offset += Deserialise(buffer, offset, fName);

//        /* Unpack the parameters. */
//        loadParams(buffer, offset, fArgs, std::index_sequence_for<T...>());

//        /* Execute the member function. */
//        fResult = std::apply(fFuncPtr, fObj, fArgs);

//        /* Calculate the serialised size of the result data. */
//        result.resize(SerialisedSize(fID),
//                      SerialisedSize(fStat),
//                      SerialisedSize(fResult));

//        /* Pack the results. */
//        offset = 0;
//        offset += Serialise(result, offset, fID);
//        offset += Serialise(result, offset, fStat);
//        offset += Serialise(result, offset, fResult);
//      }

//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      template<std::size_t ... I>
//      void unpackArgs(const std::vector<uint8_t> & buffer, size_t offset,
//                   std::tuple<T...> & tuple, std::index_sequence<I...>)
//      {
//        /* Deserialise the args. */
//        deserialise(buffer, offset, std::get<I>(tuple)...);
//      }

//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      template <typename A0, typename ... An>
//      void unpack(const std::vector<uint8_t> & buffer, size_t offset,
//                       A0 & arg, An & ... args)
//      {
//        /* Deserialise the argument. */
//        offset += Deserialise(buffer, offset, arg);

//        /* Unpack the remaining arguments. */
//        unpack(buffer, offset, args...);
//      }

//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      void unpack (const std::vector<uint8_t> &, size_t) {}
//    };

//    /*#####################################################################*//**
//     *
//     *########################################################################*/
//    template <typename R, typename ... T>
//    struct FunctionHandler : public Handler
//    {
//      /* The pointer to the function. */
//      R (*fFuncPtr)(T...args);

//      /** The ID of the call as read from the buffer. */
//      uint32_t fID;

//      /** The status field as read from the buffer. */
//      uint32_t fStat;

//      /** The name of the call as extracted from the buffer. */
//      std::string fName;

//      /** The arguments of the function. */
//      std::tuple<T...> fArgs;

//      /* Result of the function call. */
//      R fResult;

//      /*********************************************************************//**
//       * @brief FunctionHandler
//       ************************************************************************/
//      FunctionHandler(R (*funcPtr)(T...args)) : fFuncPtr(funcPtr) {}

//      /*********************************************************************//**
//       * @brief execute
//       * @param buffer
//       * @param result
//       ************************************************************************/
//      void execute(const std::vector<uint8_t> & buffer,
//                   std::vector<uint8_t> & result) override
//      {
//        /* Deserialise the fields. */
//        size_t offset = 0;
//        offset += Deserialise(buffer, offset, fID);
//        offset += Deserialise(buffer, offset, fStat);
//        offset += Deserialise(buffer, offset, fName);

//        /* Unpack the parameters. */
//        loadParams(buffer, offset, fArgs, std::index_sequence_for<T...>());

//        /* Execute the member function. */
//        fResult = std::apply(fFuncPtr, fArgs);

//        /* Calculate the serialised size of the result data. */
//        result.resize(SerialisedSize(fID),
//                      SerialisedSize(fStat),
//                      SerialisedSize(fResult));

//        /* Pack the results. */
//        offset = 0;
//        offset += Serialise(result, offset, fID);
//        offset += Serialise(result, offset, fStat);
//        offset += Serialise(result, offset, fResult);
//      }

//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      template<std::size_t ... I>
//      void unpackArgs(const std::vector<uint8_t> & buffer, size_t offset,
//                   std::tuple<T...> & tuple, std::index_sequence<I...>)
//      {
//        /* Deserialise the args. */
//        deserialise(buffer, offset, std::get<I>(tuple)...);
//      }

//      /*********************************************************************//**
//       *
//       ************************************************************************/
//      template <typename A0, typename ... An>
//      void unpack(const std::vector<uint8_t> & buffer, size_t offset,
//                       A0 & arg, An & ... args)
//      {
//        /* Deserialise the argument. */
//        arg = *(reinterpret_cast<const A0*>(buffer.data() + offset));
//        offset += sizeof(A0);

//        /* Deserialise the remaining. */
//        deserialise(buffer, offset, args...);
//      }

//      /*********************************************************************//**
//       * @brief unpack
//       ************************************************************************/
//      void unpack(const std::vector<uint8_t> &, size_t) {}
//    };

//    /*#####################################################################*//**
//     *
//     *########################################################################*/
//    struct Local
//    {
//      /** The list of synchronouse function calls. These calls will aways be
//       * executed in the order they were submitted. */
//      CircularQueue<std::vector<uint8_t>, kQueueLen> fSyncCalls;

//      /** The list of asynchronouse function calls. These calls will be executed
//       * in any order relative to all other function calls. */
//      CircularQueue<std::vector<uint8_t>, kQueueLen> fAsyncCalls;

//      /** The list of result received from the remote side. */
//      std::map<uint32_t, std::vector<uint8_t>> fResults;
//    }
//    fLocal;

//    /*#####################################################################*//**
//     *
//     *########################################################################*/
//    struct Remote
//    {
//      /** The list of function handlers. */
//      std::map<const std::string, std::unique_ptr<Handler>> fHandlers;

//      /** The list of synchronouse function calls. These calls will aways be
//       * executed in the order they were submitted. */
//      CircularQueue<std::vector<uint8_t>, kQueueLen> fSyncCalls;

//      /** The list of asynchronouse function calls. These calls will be executed
//       * in any order relative to all other function calls. */
//      CircularQueue<std::vector<uint8_t>, kQueueLen> fAsyncCalls;

//      /** The list of results to be transmitted back to the Local side. */
//      std::map<uint32_t, std::vector<uint8_t>> fResults;
//    }
//    fRemote;

//    /** Used to restrict access to the ID counter. */
//    std::mutex fCurIDMutex;

//    /** The counter used to assign an ID to the call. */
//    uint32_t fCurID;

//    /***********************************************************************//**
//     * Get the next ID to use and make sure that it is unique.
//     **************************************************************************/
//    uint32_t getNewID()
//    {
//      /* Lock access to the ID mutex. */
//      std::scoped_lock l(fCurIDMutex);

//      /* Calculate the newID. */
//      auto id = fCurID++;

//      /* Return ID. */
//      return id;
//    }

//    /***********************************************************************//**
//     *
//     **************************************************************************/
//    void handleSyncCalls()
//    {
//      while(auto val = fRemote.fSyncCalls.pop())
//      {
//        /* Get the name of the call. */
//        std::string name;
//      }


//      /* Handle all the calls. */
//      for(auto call : fCalls)
//      {
//        /* Get the name of the call. */
//        std::string name(reinterpret_cast<const char*>(call.data()));

//        /* Get the handler required. */
//        auto iter = fHandlers.find(name);

//        /* Check if it was found. */
//        if(iter != fHandlers.end())
//        {
//          /* The result of the function. */
//          std::vector<uint8_t> result;

//          /* Execute the handler. */
//          iter->second->execute(call, result);

//          /* Transmit the results. */
//          fResults.push_back(result);
//        }
//      }
//    }




//  public:
//    /***********************************************************************//**
//     * @brief RPC
//     * @param serialiser
//     */
//    RPC(std::shared_ptr<Serialiser> serialiser = nullptr);

//    /**************************************************************************/
//    template <typename R, typename ... T>
//    void registerHandler(const std::string & name, R (*funcPtr)(T ... args))
//    {
//      fHandlers[name] = std::make_unique<Function<R,T...>>(funcPtr);
//    }

//    /***********************************************************************//**
//     * Calculate the amount of memory required to serialise the function
//     * parameters.
//     **************************************************************************/
//    size_t sizeOfParams() { return 0; }

//    template <typename T, typename ... N>
//    size_t sizeOfParams(T cur, N ... next)
//    {
//      return sizeof(cur) + sizeOfParams(next...);
//    }

//    /**************************************************************************/
//    void serialise(std::vector<uint8_t> & buff, size_t offset) {}

//    template <typename T, typename ... A>
//    void serialise(std::vector<uint8_t> & buff, size_t offset, T arg, A...args)
//    {
//      /* Write the current value into the buffer. */
//      *(reinterpret_cast<T*>(buff.data() + offset)) = arg;

//      /* Serialise the remaining params. */
//      serialise(buff, offset + sizeof(T), args...);
//    }

//    /**************************************************************************/
//    template <typename R, typename ... T>
//    int32_t invokeStart(const std::string & name, T ... args)
//    {
//      /* Create the new call. */
//      std::vector<uint8_t> call;

//      /* Check the lenght of the call name does not exceed the maximum value
//       * that can be stored in a uint16_t. */
//      if(name.length() > std::numeric_limits<uint16_t>::max())
//      {
//        LUNA_THROW_RUNTIME_ERROR("Call name is too long!");
//      }

//      /* Calculate the size of the buffer. First the length of the function with
//       * a single null terminator. */
//      size_t buffSize = sizeof(uint32_t) +      /* ID Field.            */
//                        sizeof(uint16_t) +      /* Length Field.        */
//                        name.length() +         /* Name Field.          */
//                        sizeOfParams(args...);  /* Parameter Field(s).  */

//      /* Resize the vector. */
//      call.resize(buffSize);

//      /* Write the ID of the call. */
//      uint32_t id =

//      /* Write the name of the function. */
//      memcpy(call.data(), name.data(), name.length());
//      call[name.length()] = 0;

//      /* Write the parameters. */
//      serialise(call, name.length() + sizeof(char), args...);

//      /* Put it in the call queue. */
//      fCalls.push_back(call);
//    }

//    /**************************************************************************/
//    template <typename R>
//    std::optional<R> invokeFinished(int32_t id)
//    {
//      while(fResults.size() == 0)
//      {
//      }

//      return *reinterpret_cast<const R*>(fResults.front().data());
//    }


//    /**************************************************************************/
//    void update()
//    {
//      executeNext();
//    }
//  };
//}

