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

    find_program(GEN_HTML_EXEC NAMES genhtml)
    message(STATUS "genhtml: ${GEN_HTML_EXEC}")

    if(LCOV_EXEC AND GEN_HTML_EXEC)
        set(LCOV_ARG1 "-c")
        list(APPEND LCOV_ARG1 "-d" "${CMAKE_BINARY_DIR}")
        list(APPEND LCOV_ARG1 "-b" "${CMAKE_BINARY_DIR}")
        list(APPEND LCOV_ARG1 "--branch-coverage")
        list(APPEND LCOV_ARG1 "--ignore-errors" "mismatch,mismatch")
        list(APPEND LCOV_ARG1 "--ignore-errors" "gcov,gcov")
        list(APPEND LCOV_ARG1 "-o" "coverage.info")
        message(STATUS "LCOV_ARG1: ${LCOV_ARG1}")

        set(LCOV_ARG2 "-r" "coverage.info")
        list(APPEND LCOV_ARG2 "*c++/*")
        list(APPEND LCOV_ARG2 "gtest/*")
        list(APPEND LCOV_ARG2 "tst/*")
        list(APPEND LCOV_ARG2 "-b" "${CMAKE_BINARY_DIR}")
        list(APPEND LCOV_ARG2 "--branch-coverage")
        list(APPEND LCOV_ARG2 "-o" "coverage.info")
        message(STATUS "LCOV_ARG2: ${LCOV_ARG2}")

        set(GEN_HTML_ARG "coverage.info")
        list(APPEND GEN_HTML_ARG "--branch-coverage")
        list(APPEND GEN_HTML_ARG "-o" "coverage/")
        message(STATUS "GEN_HTML_ARG: ${GEN_HTML_ARG}")

        add_custom_target(gen-code-coverage
            COMMAND ${LCOV_EXEC} ${LCOV_ARG1}
            COMMAND ${LCOV_EXEC} ${LCOV_ARG2}
            COMMAND ${GEN_HTML_EXEC} ${GEN_HTML_ARG}
            USES_TERMINAL
        )
    endif()
endif()

add_subdirectory(connix)
