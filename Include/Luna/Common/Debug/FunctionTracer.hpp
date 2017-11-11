#ifndef LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
#define LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP

#if !defined(LUNA_DEBUG) || defined(LUNA_DISABLE_TRACE)
#define LUNA_TRACE_FUNCTION()
#else
#define LUNA_TRACE_FUNCTION() Luna::Common::FunctionTracer luna_tracer(\
  __FILE__, __FUNCTION__ , __LINE__)

namespace Luna
{
  namespace Common
  {
    class FunctionTracer
    {
      std::string fExitMsg;

    public:
      FunctionTracer(std::string file, std::string func, int line);
      ~FunctionTracer();
    };
  }
}
#endif // !defined(LUNA_DEBUG) || defined(LUNA_DISABLE_TRACE)
#endif // LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
