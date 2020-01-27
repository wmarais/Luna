#ifndef LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
#define LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP

#include <string>

#define LUNA_TRACE_FUNCTION()

//#define LUNA_TRACE_FUNCTION() Luna::FunctionTracer luna_tracer(\
//  __FILE__, __FUNCTION__ , __LINE__)

namespace Luna
{
  class FunctionTracer
  {
    std::string fExitMsg;

  public:
    FunctionTracer(std::string file, std::string func, int line);
    ~FunctionTracer();
  };
}

#endif // LUNA_COMMON_DEBUG_FUNCTION_TRACER_HPP
