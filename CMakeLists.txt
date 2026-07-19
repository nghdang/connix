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

include(GNUInstallDirs)

add_subdirectory(connix)
