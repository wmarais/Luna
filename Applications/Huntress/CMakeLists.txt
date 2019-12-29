
set(luna_huntress_SOURCES
  Main.cpp
  Huntress.cpp
  Huntress.hpp
)

add_executable(luna_huntress ${luna_huntress_SOURCES})
target_link_libraries(luna_huntress ${LUNA_LIBRARIES})
