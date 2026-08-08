file(GLOB_RECURSE CONNIX_CORE_MOCKS
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/Communication/**/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore/Communication/**/*.cpp
)

target_sources(connix-core-mocks PRIVATE ${CONNIX_CORE_MOCKS})
target_include_directories(connix-core-mocks PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../api/
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ConnixCore
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
