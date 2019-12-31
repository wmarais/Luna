#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <cstring>
#include <tuple>

namespace Luna
{
  class RPC
  {
    struct FunctionBase
    {
      virtual ~FunctionBase() {}
      virtual void execute(const std::vector<uint8_t> & buffer,
                           std::vector<uint8_t> & result) = 0;
    };

    template <typename R, typename ... T>
    struct Function : public FunctionBase
    {
      /* The pointer to the function. */
      R (*fFuncPtr)(T...args);

      /* The result of the function. */
      R fResult;

      /* The arguments of the function. */
      std::tuple<T...> fArgs;

      /* The buffer containing the serialised data. */
      std::vector<uint8_t> fArgBuffer;

      /************************************************************************/
      Function(R (*funcPtr)(T...args)) : fFuncPtr(funcPtr) {}

      /************************************************************************/
      template <typename A0, typename ... AN>
      void initArgs(std::vector<uint8_t> & buffer, size_t offset, A0 & cur,
                    AN & ... next)
      {
        /* Extract the value of the parameter. */
        cur = *(reinterpret_cast<A0*>(buffer.data()  + offset));

        /* Increment the offset. */
        offset += sizeof(A0);

        /* Initialise the remaining parameters. */
        initArgs(buffer, next...);
      }

      /************************************************************************/
      void deserialise(const std::vector<uint8_t> & buffer, size_t offset)
      {

      }

      /************************************************************************/
      template <typename A0, typename ... An>
      void deserialise(const std::vector<uint8_t> & buffer, size_t offset,
                       A0 & arg, An & ... args)
      {
        /* Deserialise the argument. */
        arg = *(reinterpret_cast<const A0*>(buffer.data() + offset));
        offset += sizeof(A0);

        /* Deserialise the remaining. */
        deserialise(buffer, offset, args...);
      }

      /************************************************************************/
      template<std::size_t ... I>
      void loadParams(const std::vector<uint8_t> & buffer, size_t offset,
                   std::tuple<T...> & tuple, std::index_sequence<I...>)
      {
        /* Deserialise the args. */
        deserialise(buffer, offset, std::get<I>(tuple)...);
      }


      /************************************************************************/
      void execute(const std::vector<uint8_t> & buffer,
                   std::vector<uint8_t> & result) override
      {
        /* Calculate the offset where the parameters start. */
        size_t offset = strlen(reinterpret_cast<const char*>(buffer.data()))
            + sizeof(char);

        /* Deserialise the parameters. */
        loadParams(buffer, offset, fArgs, std::index_sequence_for<T...>());

        /* Excute the function. */
        R temp = std::apply(fFuncPtr, fArgs);

        /* Write the result to the buffer. */
        result.resize(sizeof(temp));
        *(reinterpret_cast<R*>(result.data())) = temp;

      }
    };

    std::map<const std::string, std::unique_ptr<FunctionBase>> fHandlers;



    std::vector<std::vector<uint8_t>> fCalls;

    std::vector<std::vector<uint8_t>> fResults;


    void executeNext()
    {
      /* Handle all the calls. */
      for(auto call : fCalls)
      {
        /* Get the name of the call. */
        std::string name(reinterpret_cast<const char*>(call.data()));

        /* Get the handler required. */
        auto iter = fHandlers.find(name);

        /* Check if it was found. */
        if(iter != fHandlers.end())
        {
          /* The result of the function. */
          std::vector<uint8_t> result;

          /* Execute the handler. */
          iter->second->execute(call, result);

          /* Transmit the results. */
          fResults.push_back(result);
        }
      }
    }




  public:

    /**************************************************************************/
    template <typename R, typename ... T>
    void registerHandler(const std::string & name, R (*funcPtr)(T ... args))
    {
      fHandlers[name] = std::make_unique<Function<R,T...>>(funcPtr);
    }

    /***********************************************************************//**
     * Calculate the amount of memory required to serialise the function
     * parameters.
     **************************************************************************/
    size_t sizeOfParams() { return 0; }

    template <typename T, typename ... N>
    size_t sizeOfParams(T cur, N ... next)
    {
      return sizeof(cur) + sizeOfParams(next...);
    }

    /**************************************************************************/
    void serialise(std::vector<uint8_t> & buff, size_t offset) {}

    template <typename T, typename ... A>
    void serialise(std::vector<uint8_t> & buff, size_t offset, T arg, A...args)
    {
      /* Write the current value into the buffer. */
      *(reinterpret_cast<T*>(buff.data() + offset)) = arg;

      /* Serialise the remaining params. */
      serialise(buff, offset + sizeof(T), args...);
    }

    /**************************************************************************/
    template <typename R, typename ... T>
    int32_t invokeStart(const std::string & name, T ... args)
    {
      /* Create the new call. */
      std::vector<uint8_t> call;

      /* Calculate the size of the buffer. First the length of the function with
       * a single null terminator. */
      size_t buffSize = name.length() + sizeof(char) + sizeOfParams(args...);

      /* Resize the vector. */
      call.resize(buffSize);

      /* Write the name of the function. */
      memcpy(call.data(), name.data(), name.length());
      call[name.length()] = 0;

      /* Write the parameters. */
      serialise(call, name.length() + sizeof(char), args...);

      /* Put it in the call queue. */
      fCalls.push_back(call);
    }

    /**************************************************************************/
    template <typename R>
    std::optional<R> invokeFinished(int32_t id)
    {
      while(fResults.size() == 0)
      {
      }

      return *reinterpret_cast<const R*>(fResults.front().data());
    }


    /**************************************************************************/
    void update()
    {
      executeNext();
    }
  };
}

