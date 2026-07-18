add_executable(connix-cli)

target_link_libraries(connix-cli PRIVATE connix-core)

add_subdirectory(src)
