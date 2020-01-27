# Add all the benchmark applications.
add_executable(LunaBM_Log LunaBM_Log.cpp)
add_executable(LunaBM_ReusableID LunaBM_ReusableID.cpp)
add_executable(LunaBM_TCPSocket LunaBM_TCPSocket.cpp)
add_executable(LunaBM_UDPSocket LunaBM_UDPSocket.cpp)
add_executable(LunaBM_UnixSocket LunaBM_UnixSocket.cpp)

# Link against the benchmark library.
target_link_libraries(LunaBM_Log benchmark::benchmark libluna)
target_link_libraries(LunaBM_ReusableID benchmark::benchmark libluna)
target_link_libraries(LunaBM_TCPSocket benchmark::benchmark libluna)
target_link_libraries(LunaBM_UDPSocket benchmark::benchmark libluna)
target_link_libraries(LunaBM_UnixSocket benchmark::benchmark libluna)
