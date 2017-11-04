#include "../../../../Include/Luna/Common.hpp"

using namespace Luna::Common;

//==============================================================================
FunctionTracer::FunctionTracer(std::string file, std::string func, int line)
{
  // Find the last seperator position.
  size_t i = file.rfind('/', file.length());

  // Check if it was found.
  if (i != std::string::npos)
  {
    file = file.substr(i+1, file.length() - i);
  }

  // Create the entry message.
  std::stringstream ss;
  ss << "TRACE | " << file << " | " << func << " | " <<
        line << " | ENTER.";

  // Write a log message.
  Luna::Common::Log::ptr()->write(ss.str());

  // Create the exit message.
  ss.str(std::string());
  ss << "TRACE | " << file << " | " << func << " | " <<
        line << " | EXIT.";

  fExitMsg = ss.str();
}

//==============================================================================
FunctionTracer::~FunctionTracer()
{
  // Write a log message.
  Luna::Common::Log::ptr()->write(fExitMsg);
}
