add_library(connix-core STATIC)

add_subdirectory(api)
add_subdirectory(inc)
add_subdirectory(src)

install(TARGETS connix-core DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(BUILD_MOCKS OR BUILD_TESTS)
    find_package(GTest REQUIRED)
endif()

if(BUILD_TESTS)
    add_subdirectory(tst)
endif()
