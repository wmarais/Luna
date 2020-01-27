#include <sstream>

#include "../../../../Include/Luna/Common/Debug/FunctionTracer.hpp"
#include "../../../../Include/Luna/Common/Debug/Log.hpp"

using namespace Luna;

//==============================================================================
FunctionTracer::FunctionTracer(std::string file, std::string func, int line)
{
  // Find the last seperator position.
  size_t i = file.rfind('/', file.length());

  // Check if it was found.
  if(i != std::string::npos)
  {
    file = file.substr(i + 1, file.length() - i);
  }

  // Create the entry message.
  std::stringstream ss;
  ss << "TRACE | " << file << " | " << func << " | " << line << " | ENTER.";

  // Write a log message.
  Luna::Log::ptr()->write(Log::Levels::Debug, ss.str());

  // Create the exit message.
  ss.str(std::string());
  ss << "TRACE | " << file << " | " << func << " | " << line << " | EXIT.";

  fExitMsg = ss.str();
}

//==============================================================================
FunctionTracer::~FunctionTracer()
{
  // Write a log message.
  Luna::Log::ptr()->write(Log::Levels::Debug, fExitMsg);
}
