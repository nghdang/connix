add_subdirectory(api)
add_subdirectory(inc)
add_subdirectory(res)
add_subdirectory(src)
add_subdirectory(gmock)

if(BUILD_TESTS)
    add_subdirectory(tst)
endif()
