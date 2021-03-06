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

set(CMAKE_CXX_STANDARD 20)
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

  Include/Luna/Common/FileSystem/FilePath.hpp

  Include/Luna/Common/Network/DNS.hpp
  Include/Luna/Common/Network/IPAddress.hpp
  Include/Luna/Common/Network/Socket.hpp
  Include/Luna/Common/Network/SocketAddress.hpp
  Include/Luna/Common/Network/UnixSocketClient.hpp
  Include/Luna/Common/Network/UnixSocketServer.hpp

  Include/Luna/Common/RemoteDesktop/Crypto.hpp

  Include/Luna/Common/CircularQueue.hpp
  Include/Luna/Common/Colour.hpp
  Include/Luna/Common/Directory.hpp
  Include/Luna/Common/Event.hpp
  Include/Luna/Common/FrameBuffer.hpp
  Include/Luna/Common/Macros.hpp
  Include/Luna/Common/Rect.hpp
  Include/Luna/Common/ReusableID.hpp
  Include/Luna/Common/RPC.hpp
  Include/Luna/Common/Serialiser.hpp
  Include/Luna/Common/SharedResource.hpp
  Include/Luna/Common/Settings.hpp
  Include/Luna/Common/Utils.hpp
  Include/Luna/Common/User.hpp
  Include/Luna/Common/SystemHeaders.hpp
)

set(LunaCommon_SOURCES  
  Source/Luna/Common/Debug/Exception.cpp
  Source/Luna/Common/Debug/FunctionTracer.cpp
  Source/Luna/Common/Debug/Log.cpp

  Source/Luna/Common/Network/DNS.cpp
  Source/Luna/Common/Network/IPAddress.cpp
  Source/Luna/Common/Network/Socket.cpp
  Source/Luna/Common/Network/SocketAddress.cpp
  Source/Luna/Common/Network/UnixSocketClient.cpp
  Source/Luna/Common/Network/UnixSocketServer.cpp

  Source/Luna/Common/Event.cpp
  Source/Luna/Common/FrameBuffer.cpp
  Source/Luna/Common/Directory.cpp
  Source/Luna/Common/Serialiser.cpp
  Source/Luna/Common/Settings.cpp
  Source/Luna/Common/Utils.cpp
  Source/Luna/Common/User.cpp
)

add_library(libluna STATIC ${LunaCommon_HEADERS} ${LunaCommon_SOURCES})
target_link_libraries(libluna pthread)

################################################################################
# BACKEND LIBRARY
################################################################################
set(liblunabe_HEADERS
  Include/Luna/BackEnd.hpp

  Include/Luna/BE/Request.hpp
  Include/Luna/BE/Session.hpp
  Include/Luna/BE/SessionManager.hpp
  Include/Luna/BE/UDEV.hpp
  Include/Luna/BE/Window.hpp

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
  Source/Luna/BE/Window.cpp

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

set(LUNA_LIBRARIES liblunabe liblunafe libluna ${Luna_LIBCONFIGPP} ${Luna_LIBUUID}
    ${Luna_LIBPTHREAD} ${Luna_LIBUDEV} ${Luna_LIBDRM} ${PNG_LIBRARIES})


subdirs(Tests/UnixSocket)
subdirs(Tests)

# Build the Luna Back End.
subdirs(Applications/BackEnd)

# Build the Luna File / Directolry Explorer application.
subdirs(Applications/Huntress)

find_package(benchmark)
if(benchmark_FOUND)
    subdirs(Benchmark)
endif()

################################################################################
## Find the Doxygen tool.
#find_package(Doxygen)
#if(DOXYGEN_FOUND)
#    # Set the input and output files.
#    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
#    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

#    # Configure the file.
#    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

#    # Build the documentation.
#    add_custom_target(lunadocs ALL
#        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#        COMMENT "Generating API documentation with Doxygen."
#        VERBATIM)
#else()
#    message("Doxygen need to be installed to generate the API documentation.")
#endif()
