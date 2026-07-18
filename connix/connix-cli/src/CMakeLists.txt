file(GLOB_RECURSE CONNIX_CLI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Bootstrap/*.cpp
)

target_sources(connix-cli PRIVATE ${CONNIX_CLI_SOURCES})
