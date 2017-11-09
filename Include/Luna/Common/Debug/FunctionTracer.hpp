#ifndef LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
#define LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP

#ifdef LUNA_DEBUG

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
#else
#define LUNA_TRACE_FUNCTION()
#endif

#endif // LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
