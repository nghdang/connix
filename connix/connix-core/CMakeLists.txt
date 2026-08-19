add_library(connix-core STATIC)

add_subdirectory(api)
add_subdirectory(inc)
add_subdirectory(src)

install(TARGETS connix-core DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(BUILD_MOCKS OR BUILD_TESTS)
    find_package(GTest REQUIRED)
endif()

if(BUILD_MOCKS)
    add_library(connix-core-mocks SHARED)

    target_link_libraries(connix-core-mocks PUBLIC connix-core gtest::gtest)

    add_subdirectory(mock)

    install(TARGETS connix-core-mocks DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if(BUILD_TESTS)
    add_subdirectory(tst)
endif()
