add_executable(connix-gui)

target_link_libraries(connix-gui PRIVATE connix-core)

add_subdirectory(src)
