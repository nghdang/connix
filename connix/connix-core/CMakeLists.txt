add_library(connix-core STATIC)

find_package(nlohmann_json REQUIRED)
find_package(nlohmann_json_schema_validator REQUIRED)
target_link_libraries(connix-core PUBLIC nlohmann_json::nlohmann_json nlohmann_json_schema_validator)

install(TARGETS connix-core DESTINATION ${CMAKE_INSTALL_LIBDIR})

add_library(connix-core-mocks SHARED)

target_link_libraries(connix-core-mocks PUBLIC connix-core gtest::gtest GTest::gmock)

install(TARGETS connix-core-mocks DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(BUILD_TESTS)
    function(create_unit_test src_file)
        get_filename_component(src_file_we ${src_file} NAME_WE)
        set(TEST_NAME ${src_file_we})

        # ${ARGV} contains list of all function arguments
        add_executable(${TEST_NAME} ${ARGV})

        target_link_libraries(${TEST_NAME} PRIVATE connix-core connix-core-mocks gtest::gtest GTest::gmock)
        target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../../api/public
            ${CMAKE_CURRENT_SOURCE_DIR}/../../api/internal
            ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/public
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/internal
        )
        target_compile_definitions(${TEST_NAME} PRIVATE CONNIX_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

        add_test(${TEST_NAME} ${TEST_NAME} --gtest_output=json:${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.json)
        # we do not allow any unit test to run for more than 30 seconds
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 30)

        add_dependencies(connix-core-unit-tests-build ${TEST_NAME})
    endfunction()

    function(create_integration_test src_file)
        get_filename_component(src_file_we ${src_file} NAME_WE)
        set(TEST_NAME ${src_file_we})

        # ${ARGV} contains list of all function arguments
        add_executable(${TEST_NAME} ${ARGV})

        target_link_libraries(${TEST_NAME} PRIVATE connix-core connix-core-mocks gtest::gtest GTest::gmock)
        target_include_directories(${TEST_NAME} PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../../api/public
            ${CMAKE_CURRENT_SOURCE_DIR}/../../api/internal
            ${CMAKE_CURRENT_SOURCE_DIR}/../../inc
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/public
            ${CMAKE_CURRENT_SOURCE_DIR}/../../gmock/internal
        )
        target_compile_definitions(${TEST_NAME} PRIVATE CONNIX_PROJECT_ROOT="${CMAKE_SOURCE_DIR}")

        add_test(${TEST_NAME} ${TEST_NAME} --gtest_output=json:${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}.json)
        # we do not allow any integration test to run for more than 60 seconds
        set_tests_properties(${TEST_NAME} PROPERTIES TIMEOUT 60)

        add_dependencies(connix-core-integration-tests-build ${TEST_NAME})
    endfunction()

    add_custom_target(connix-core-unit-tests-build)
    add_custom_target(connix-core-unit-tests-run
        COMMAND ${CMAKE_CTEST_COMMAND} -j1 --tests-regex "UnitTest" --verbose
        USES_TERMINAL
    )
    add_dependencies(connix-core-unit-tests-run connix-core-unit-tests-build)

    add_custom_target(connix-core-integration-tests-build)
    add_custom_target(connix-core-integration-tests-run
        COMMAND ${CMAKE_CTEST_COMMAND} -j1 --tests-regex "IntegrationTest" --verbose
        USES_TERMINAL
    )
    add_dependencies(connix-core-integration-tests-run connix-core-integration-tests-build)
endif()

add_subdirectory("domain")
add_subdirectory("application")
add_subdirectory("infrastructure")
