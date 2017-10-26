cmake_minimum_required (VERSION 3.7)
project (Luna)



find_library(Luna_LIBCONFIGPP "config++")
find_library(Luna_LIBUUID "uuid")
find_library(Luna_LIBUDEV "udev")

set(Luna_LIBPTHREAD pthread)

message("Luna_LIBUUID = ${Luna_LIBUUID}\n")

set(LunaCommon_HEADERS
  Include/Luna/Common.hpp

  Include/Luna/Common/Event.hpp
  Include/Luna/Common/Exception.hpp
  Include/Luna/Common/Log.hpp
  Include/Luna/Common/GeneralHeaders.hpp
  Include/Luna/Common/Settings.hpp
  Include/Luna/Common/SystemHeaders.hpp
)

set(LunaCommon_SOURCES
  Source/Luna/Common/Event.cpp
  Source/Luna/Common/Log.cpp
  Source/Luna/Common/Settings.cpp
)

add_library(LunaCommon ${LunaCommon_HEADERS} ${LunaCommon_SOURCES})



set(LunaClient_HEADERS
  Include/Luna/Client.hpp
  Include/Luna/Client/Event.hpp
  Include/Luna/Client/Request.hpp
)

set(LunaClient_SOURCES

)


#add_library(LunaClient ${LunaClient_HEADERS} ${LunaClient_SOURCES})


set(LunaServer_HEADERS
  Include/Luna/Server.hpp


  Include/Luna/Server/Request.hpp
  Include/Luna/Server/Session.hpp
  Include/Luna/Server/SessionManager.hpp
  Include/Luna/Server/UDEV.hpp

  Include/Luna/Server/Display/DisplayManager.hpp

  Include/Luna/Server/Input/InputDevice.hpp
  Include/Luna/Server/Input/InputManager.hpp
  Include/Luna/Server/Input/Joystick.hpp
  Include/Luna/Server/Input/Keyboard.hpp
  Include/Luna/Server/Input/Mouse.hpp
)

set(LunaServer_SOURCES
  Source/Luna/Server/Request.cpp
  Source/Luna/Server/Session.cpp
  Source/Luna/Server/SessionManager.cpp
  Source/Luna/Server/UDEV.cpp

  Source/Luna/Server/Display/DisplayManager.cpp

  Source/Luna/Server/Input/InputDevice.cpp
  Source/Luna/Server/Input/InputManager.cpp
  Source/Luna/Server/Input/Joystick.cpp
  Source/Luna/Server/Input/Keyboard.cpp
  Source/Luna/Server/Input/Mouse.cpp
)

add_library(LunaServer ${LunaServer_HEADERS} ${LunaServer_SOURCES})


set(LunaMoon_HEADERS

)

set(LunaMoon_SOURCES
  Moon/Source/Main.cpp
)

add_executable(LunaMoon ${LunaMoon_HEADERS} ${LunaMoon_SOURCES})

set(LunaEarth_HEADERS
  Earth/Include/Luna/Server/Earth.hpp
)

set(LunaEarth_SOURCES
  Earth/Source/Main.cpp
  Earth/Source/Luna/Server/Earth.cpp
)

add_executable(LunaEarth ${LunaEarth_HEADERS} ${LunaEarth_SOURCES})
target_link_libraries(LunaEarth LunaCommon LunaServer ${Luna_LIBCONFIGPP}
    ${Luna_LIBUUID} ${Luna_LIBPTHREAD} ${Luna_LIBUDEV})



