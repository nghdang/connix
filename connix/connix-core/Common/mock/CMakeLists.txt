file(GLOB_RECURSE CONNIX_CORE_COMMON_MOCKS
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/**/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/**/*.cpp
)

target_sources(connix-core-mocks PRIVATE ${CONNIX_CORE_COMMON_MOCKS})
target_include_directories(connix-core-mocks PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../api/
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
