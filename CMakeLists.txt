cmake_minimum_required (VERSION 3.7)
project (Luna)

################################################################################
# Required Libraries
################################################################################
find_library(Luna_LIBCONFIGPP "config++")
find_library(Luna_LIBUUID "uuid")
find_library(Luna_LIBUDEV "udev")
find_library(Luna_LIBDRM "drm")

set(Luna_LIBPTHREAD pthread)


include_directories("/usr/include/libdrm")

################################################################################
# GCC Specific Compiler Flags
################################################################################
# Code Coverage Checking
set(GCC_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_LINK_FLAGS    "-lgcov")

# Enable exception handling.
set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} -fexceptions")

# Enable stack protection.
set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} -fstack-protector")

# Imroved warning and code checking.
set(GCC_COMPILE_FLAGS "${GCC_COMPILE_FLAGS} \
    -Wall -Wextra -Wzero-as-null-pointer-constant -Wuseless-cast \
    -Wswitch-default -Wswitch-enum -Wunused-parameter -Wunused-result" )

if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  # Set the compile flags that apply to both Debug and Release builds.
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_LINK_FLAGS}")

  # Enable gprof and valgrind for debug builds. Basically ensure that debug
  # symbols are enabled, profile is enabled and no code optimisation is
  # performed. Note there is a bug in GCC V6 that is causing gprof not to work.
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -p -pg -O0")

endif()

################################################################################
# Common code shared between client and server.
################################################################################
set(LunaCommon_HEADERS
  Include/Luna/Common.hpp

  Include/Luna/Common/Event.hpp
  Include/Luna/Common/Exception.hpp
  Include/Luna/Common/Log.hpp
  Include/Luna/Common/GeneralHeaders.hpp
  Include/Luna/Common/SharedResource.hpp
  Include/Luna/Common/Settings.hpp
  Include/Luna/Common/SystemHeaders.hpp
)

set(LunaCommon_SOURCES
  Source/Luna/Common/Event.cpp
  Source/Luna/Common/Log.cpp
  Source/Luna/Common/Settings.cpp
)

add_library(LunaCommon STATIC ${LunaCommon_HEADERS} ${LunaCommon_SOURCES})

################################################################################
# Client Specific Code
################################################################################
set(LunaClient_HEADERS
  Include/Luna/Client.hpp
  Include/Luna/Client/Event.hpp
  Include/Luna/Client/Request.hpp
)

set(LunaClient_SOURCES

)

#add_library(LunaClient STATIC ${LunaClient_HEADERS} ${LunaClient_SOURCES})

################################################################################
# Server Specific Code
################################################################################
set(LunaServer_HEADERS
  Include/Luna/Server.hpp


  Include/Luna/Server/Request.hpp
  Include/Luna/Server/Session.hpp
  Include/Luna/Server/SessionManager.hpp
  Include/Luna/Server/UDEV.hpp

  Include/Luna/Server/Display/Display.hpp
  Include/Luna/Server/Display/DisplayManager.hpp
  Include/Luna/Server/Display/VideoCard.hpp

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

  Source/Luna/Server/Display/Display.cpp
  Source/Luna/Server/Display/DisplayManager.cpp
  Source/Luna/Server/Display/VideoCard.cpp

  Source/Luna/Server/Input/InputDevice.cpp
  Source/Luna/Server/Input/InputManager.cpp
  Source/Luna/Server/Input/Joystick.cpp
  Source/Luna/Server/Input/Keyboard.cpp
  Source/Luna/Server/Input/Mouse.cpp
)

add_library(LunaServer STATIC ${LunaServer_HEADERS} ${LunaServer_SOURCES})


################################################################################
# Session Client
################################################################################
set(LunaMoon_HEADERS

)

set(LunaMoon_SOURCES
  Moon/Source/Main.cpp
)

add_executable(LunaMoon ${LunaMoon_HEADERS} ${LunaMoon_SOURCES})

################################################################################
# Session Server
################################################################################
set(LunaEarth_HEADERS
  Earth/Include/Luna/Server/Earth.hpp
)

set(LunaEarth_SOURCES
  Earth/Source/Main.cpp
  Earth/Source/Luna/Server/Earth.cpp
)

add_executable(LunaEarth ${LunaEarth_HEADERS} ${LunaEarth_SOURCES})
target_link_libraries(LunaEarth LunaCommon LunaServer ${Luna_LIBCONFIGPP}
    ${Luna_LIBUUID} ${Luna_LIBPTHREAD} ${Luna_LIBUDEV} ${Luna_LIBDRM})



