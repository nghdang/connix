file(GLOB_RECURSE CONNIX_CORE_PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/**/*.hpp
)

target_sources(connix-core PUBLIC ${CONNIX_CORE_PRIVATE_HEADERS})
target_include_directories(connix-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
