cmake_minimum_required(VERSION 3.22)

project(connix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT "${CONAN_BUILD}" STREQUAL "ON")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
    include(conan_provider)

    conan_provide_dependency()

    get_property(_conan_generators_folder GLOBAL PROPERTY CONAN_GENERATORS_FOLDER)
    list(PREPEND CMAKE_MODULE_PATH "${_conan_generators_folder}")

    include(conan_toolchain)
endif()

option(BUILD_MOCKS "Option for build mocking files" OFF)
option(BUILD_TESTS "Option for build testing files" OFF)

message(STATUS "CMAKE_SYSROOT: ${CMAKE_SYSROOT}")
if(NOT CMAKE_SYSROOT)
    set(BUILD_MOCKS ON)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(BUILD_TESTS ON)
    endif()
endif()

message(STATUS "BUILD_MOCKS: ${BUILD_MOCKS}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")

include(GNUInstallDirs)

if(BUILD_TESTS)
    include(CTest)
    enable_testing()

    add_compile_options("--coverage")
    add_link_options("--coverage")

    find_program(LCOV_EXEC NAMES lcov)
    message(STATUS "lcov: ${LCOV_EXEC}")

    find_program(GENHTML_EXEC NAMES genhtml)
    message(STATUS "genhtml: ${GENHTML_EXEC}")

    if(LCOV_EXEC AND GENHTML_EXEC)
        add_custom_target(gen-code-coverage
            COMMAND ${LCOV_EXEC} -c -d ${CMAKE_BINARY_DIR} -b ${CMAKE_BINARY_DIR} --branch-coverage --ignore-errors mismatch,mismatch --ignore-errors gcov,gcov -o coverage.info
            COMMAND ${LCOV_EXEC} -r coverage.info "*c++/*" "gmock/*" "gtest/*" "mock/*" "tst/*" "*.conan2/*" -b ${CMAKE_BINARY_DIR} --branch-coverage -o coverage.info
            COMMAND ${GENHTML_EXEC} coverage.info --branch-coverage -o coverage/
            USES_TERMINAL
        )
    endif()
endif()

add_subdirectory(connix)
