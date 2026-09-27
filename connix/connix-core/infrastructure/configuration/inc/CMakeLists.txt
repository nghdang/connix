file(GLOB_RECURSE CONNIX_CORE_CONFIGURATION_INC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/**/*.hpp
)

target_sources(connix-core PRIVATE
    ${CONNIX_CORE_CONFIGURATION_INC_HEADERS}
)

target_include_directories(connix-core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
