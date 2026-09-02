add_subdirectory(api)
add_subdirectory(inc)
add_subdirectory(src)

if(BUILD_MOCKS)
    add_subdirectory(mock)
endif()

if(BUILD_TESTS)
    add_subdirectory(tst)
endif()
