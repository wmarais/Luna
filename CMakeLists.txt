cmake_minimum_required (VERSION 3.2)
project (Luna)


set(LunaCommon_HEADERS
  Include/Luna/Common.hpp

  Include/Luna/Common/Event.hpp
  Include/Luna/Common/GeneralHeaders.hpp
  Include/Luna/Common/SystemHeaders.hpp
)

set(LunaCommon_SOURCES
  Source/Luna/Common/Event.cpp
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

  Include/Luna/Server/Input.hpp
  Include/Luna/Server/Request.hpp
  Include/Luna/Server/Session.hpp
  Include/Luna/Server/SessionManager.hpp

  Include/Luna/Server/Input/Controller.hpp
  Include/Luna/Server/Input/Keyboard.hpp
  Include/Luna/Server/Input/Mouse.hpp
)

set(LunaServer_SOURCES
  Source/Luna/Server/Input.cpp
  Source/Luna/Server/Request.cpp
  Source/Luna/Server/Session.cpp
  Source/Luna/Server/SessionManager.cpp

  Source/Luna/Server/Input/Controller.cpp
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

)

set(LunaEarth_SOURCES
  Earth/Source/Main.cpp
)

add_executable(LunaEarth ${LunaEarth_HEADERS} ${LunaEarth_SOURCES})



