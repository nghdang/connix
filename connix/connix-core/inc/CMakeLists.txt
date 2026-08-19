file(GLOB_RECURSE CONNIX_CORE_PRIVATE_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/Communication/**/*.hpp
)

target_sources(connix-core PRIVATE ${CONNIX_CORE_PRIVATE_HEADERS})
target_include_directories(connix-core PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
