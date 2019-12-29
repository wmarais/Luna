cmake_minimum_required (VERSION 3.7)
project (Luna)

################################################################################
# Required Libraries
################################################################################
find_library(Luna_LIBCONFIGPP "config++")
find_library(Luna_LIBUUID "uuid")
find_library(Luna_LIBUDEV "udev")
find_library(Luna_LIBDRM "drm")

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIRS})

set(Luna_LIBPTHREAD pthread)


include_directories("/usr/include/libdrm")

################################################################################
# GCC Specific Compiler Flags
################################################################################
# Code Coverage Checking
set(GCC_COMPILE_FLAGS "-fprofile-arcs -ftest-coverage")
set(GCC_LINK_FLAGS    "-lgcov")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
  Include/Luna/Config.hpp

  Include/Luna/Common/Debug/Exception.hpp
  Include/Luna/Common/Debug/FunctionTracer.hpp
  Include/Luna/Common/Debug/Log.hpp

  Include/Luna/Common/Network/UnixSocketClient.hpp
  Include/Luna/Common/Network/UnixSocketServer.hpp

  Include/Luna/Common/CircularQueue.hpp
  Include/Luna/Common/Colour.hpp
  Include/Luna/Common/Event.hpp
  Include/Luna/Common/Macros.hpp
  Include/Luna/Common/Rect.hpp
  Include/Luna/Common/SharedResource.hpp
  Include/Luna/Common/Settings.hpp
  Include/Luna/Common/String.hpp
)

set(LunaCommon_SOURCES  
  Source/Luna/Common/Debug/Exception.cpp
  Source/Luna/Common/Debug/FunctionTracer.cpp
  Source/Luna/Common/Debug/Log.cpp

  Source/Luna/Common/Network/UnixSocketClient.cpp
  Source/Luna/Common/Network/UnixSocketServer.cpp

  Source/Luna/Common/Event.cpp
  Source/Luna/Common/Settings.cpp
)

add_library(libluna STATIC ${LunaCommon_HEADERS} ${LunaCommon_SOURCES})

################################################################################
# BACKEND LIBRARY
################################################################################
set(liblunabe_HEADERS
  Include/Luna/BackEnd.hpp

  Include/Luna/BE/Request.hpp
  Include/Luna/BE/Session.hpp
  Include/Luna/BE/SessionManager.hpp
  Include/Luna/BE/UDEV.hpp

  Include/Luna/BE/Display/Display.hpp
  Include/Luna/BE/Display/DisplayManager.hpp
  Include/Luna/BE/Display/VideoCard.hpp

  Include/Luna/BE/Input/InputDevice.hpp
  Include/Luna/BE/Input/InputManager.hpp
  Include/Luna/BE/Input/Joystick.hpp
  Include/Luna/BE/Input/Keyboard.hpp
  Include/Luna/BE/Input/Mouse.hpp
)

set(liblunabe_SOURCES
  Source/Luna/BE/Request.cpp
  Source/Luna/BE/Session.cpp
  Source/Luna/BE/SessionManager.cpp
  Source/Luna/BE/UDEV.cpp

  Source/Luna/BE/Display/Display.cpp
  Source/Luna/BE/Display/DisplayManager.cpp
  Source/Luna/BE/Display/VideoCard.cpp

  Source/Luna/BE/Input/InputDevice.cpp
  Source/Luna/BE/Input/InputManager.cpp
  Source/Luna/BE/Input/Joystick.cpp
  Source/Luna/BE/Input/Keyboard.cpp
  Source/Luna/BE/Input/Mouse.cpp
)

add_library(liblunabe STATIC ${liblunabe_HEADERS} ${liblunabe_SOURCES})

################################################################################
# FRONTEND LIBRARY
################################################################################
set(liblunafe_HEADERS
  Include/Luna/FrontEnd.hpp
  Include/Luna/FE/Button.hpp
  Include/Luna/FE/Event.hpp
  Include/Luna/FE/GridLayout.hpp
  Include/Luna/FE/HorizontalLayout.hpp
  Include/Luna/FE/Label.hpp
  Include/Luna/FE/Object.hpp
  Include/Luna/FE/Renderer.hpp
  Include/Luna/FE/RendererVulkan.hpp
  Include/Luna/FE/Request.hpp
  Include/Luna/FE/TextBox.hpp
  Include/Luna/FE/VerticalLayout.hpp
  Include/Luna/FE/Window.hpp
)

set(liblunafe_SOURCES
  Source/Luna/FE/Button.cpp
  Source/Luna/FE/GridLayout.cpp
  Source/Luna/FE/HorizontalLayout.cpp
  Source/Luna/FE/Label.cpp
  Source/Luna/FE/Object.cpp
  Source/Luna/FE/Renderer.cpp
  Source/Luna/FE/RendererVulkan.cpp
  Source/Luna/FE/TextBox.cpp
  Source/Luna/FE/VerticalLayout.cpp
  Source/Luna/FE/Window.cpp
)

add_library(liblunafe STATIC ${liblunafe_HEADERS} ${liblunafe_SOURCES})

################################################################################
# Session Client
################################################################################
#set(LunaFE_HEADERS

#)

#set(LunaFE_SOURCES
#  FrontEnd/Source/Main.cpp
#)

#add_executable(LunaFE ${LunaFE_HEADERS} ${LunaFE_SOURCES})
#target_link_libraries(LunaFE liblunafe libluna ${PNG_LIBRARIES})

################################################################################
# Session Server
################################################################################
#set(LunaBE_HEADERS
#  BackEnd/Include/Luna/Server/Earth.hpp
#)

#set(LunaBE_SOURCES
#  BackEnd/Source/Main.cpp
#  BackEnd/Source/Luna/Server/Earth.cpp
#)

#add_executable(LunaBE ${LunaBE_HEADERS} ${LunaBE_SOURCES})

#target_link_libraries(LunaBE liblunabe libluna ${Luna_LIBCONFIGPP}
#    ${Luna_LIBUUID} ${Luna_LIBPTHREAD} ${Luna_LIBUDEV} ${Luna_LIBDRM})


subdirs(Tests/UnixSocket)

subdirs(Applications/BackEnd)
subdirs(Applications/Huntress)

