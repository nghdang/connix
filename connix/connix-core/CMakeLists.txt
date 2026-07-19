add_library(connix-core STATIC)

add_subdirectory(api)
add_subdirectory(src)

install(TARGETS connix-core DESTINATION ${CMAKE_INSTALL_LIBDIR})
