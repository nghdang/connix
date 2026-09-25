add_library(connix-core STATIC)

install(TARGETS connix-core DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(connix-core-mocks SHARED)

target_link_libraries(connix-core-mocks PUBLIC connix-core gtest::gtest GTest::gmock)

install(TARGETS connix-core-mocks DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(BUILD_TESTS)
    function(create_unit_test src_file)
        get_filename_component(src_file_we ${src_file} NAME_WE)
        set(TEST_NAME UnitTest${src_file_we})

        # ${ARGV} contains list of all function arguments
        add_executable(${TEST_NAME} ${ARGV})

        target_link_libraries(${TEST_NAME} PRIVATE connix-core connix-core-mocks gtest::gtest GTest::gmock)
        target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../api
            ${CMAKE_CURRENT_SOURCE_DIR}/../inc
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/api
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/api-internal
        )

        message(STATUS "Add UT: ${TEST_NAME}")
        add_test(${TEST_NAME} ${TEST_NAME} --gtest_output=json:${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.json)
        # we do not allow any unit test to run for more than 30 seconds
        set_tests_properties(${full_test_name} PROPERTIES TIMEOUT 30)

        add_dependencies(connix-core-unit-tests-build ${TEST_NAME})
    endfunction()


    add_custom_target(connix-core-unit-tests-build)
    add_custom_target(connix-core-unit-tests-run
        COMMAND ${CMAKE_CTEST_COMMAND} -j1 --tests-regex "UnitTest" --verbose
        USES_TERMINAL
    )
    add_dependencies(connix-core-unit-tests-run connix-core-unit-tests-build)
endif()

add_subdirectory("domain")
add_subdirectory("application")
add_subdirectory("infrastructure")
