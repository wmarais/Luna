#include "../../../Include/Luna/Common/Settings.hpp"

using namespace Luna::Common;

//==============================================================================
Settings::Settings(const std::string & settingsPath) :
  fSettingsPath(settingsPath)
{
  // Try to parse the file, but don't worry to much if it does not work because
  // we must provide default values for all settings.
  fSettingsFile = std::make_unique<libconfig::Config>();

  try
  {
    // Try to parse the config file.
    fSettingsFile->readFile(fSettingsPath.c_str());
  }
  catch(const libconfig::FileIOException & ex)
  {
// TODO - Print an non-fatal error message here to alert the user.
  }

}

//==============================================================================
Settings::~Settings() {}

//==============================================================================
bool Settings::getValue(const std::string & path, bool def) const
{
  // The return value.
  bool ret = false;

  // Lookup the value and see if it exist.
  if(!fSettingsFile->lookupValue(path, ret))
  {
    // Set the return value to the default value.
    ret = def;
  }

  // Return the value read.
  return ret;
}

//==============================================================================
int64_t Settings::getValue(const std::string & path, int64_t def) const
{
  // The return value.
  long long ret = 0;

  // Lookup the value and see if it exist.
  if(!fSettingsFile->lookupValue(path, ret))
  {
    // Set the return value to the default value.
    ret = def;
  }

  // Return the value read.
  return ret;
}

//==============================================================================
double Settings::getValue(const std::string & path, double def) const
{
  // The return value.
  double ret = 0.0;

  // Lookup the value and see if it exist.
  if(!fSettingsFile->lookupValue(path, ret))
  {
    // Set the return value to the default value.
    ret = def;
  }

  // Return the value read.
  return ret;
}

//==============================================================================
std::string Settings::getValue(const std::string & path,
                               const std::string & def) const
{
  // The return value.
  std::string ret;

  // Lookup the value and see if it exist.
  if(!fSettingsFile->lookupValue(path, ret))
  {
    // Set the return value to the default value.
    ret = def;
  }

  // Return the value read.
  return ret;
}

//==============================================================================
std::vector<std::string> Settings::getArray(const std::string & path) const
{

  return std::vector<std::string>();
}


