add_executable(connix-cli)

target_link_libraries(connix-cli PRIVATE connix-core)

add_subdirectory(src)

install(TARGETS connix-cli DESTINATION ${CMAKE_INSTALL_BINDIR})
