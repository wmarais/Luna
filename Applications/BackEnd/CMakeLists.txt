
set(lunabe_SOURCES
  Main.cpp
  BackEnd.cpp
  BackEnd.hpp
)

add_executable(luna_backend ${lunabe_SOURCES})
target_link_libraries(luna_backend ${LUNA_LIBRARIES})
