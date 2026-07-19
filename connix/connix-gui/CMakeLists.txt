add_executable(connix-gui)

target_link_libraries(connix-gui PRIVATE connix-core)

add_subdirectory(src)

install(TARGETS connix-gui DESTINATION ${CMAKE_INSTALL_BINDIR})
