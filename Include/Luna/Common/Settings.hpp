#ifndef LUNA_COMMON_SETTINGS_HPP
#define LUNA_COMMON_SETTINGS_HPP

#include "GeneralHeaders.hpp"

namespace Luna
{
  namespace Common
  {
    class Settings
    {
      // The name of the settings file.
      std::string fSettingsPath;

      std::unique_ptr<libconfig::Config> fSettingsFile;


    public:
      Settings(const std::string & settingsPath);

      ~Settings();

      // Try to read the boolean, if the value does not exist then set it to the
      // passed value and return it.
      bool getValue(const std::string & path, bool def);

      int64_t getValue(const std::string & path, int64_t def);

      double getValue(const std::string & path, double def);



      std::string getValue(const std::string & path, const std::string & def);

    };
  }
}


#endif // LUNA_COMMON_SETTINGS_HPP
