file(GLOB_RECURSE CONNIX_CORE_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Product/ConnixCore/Logging/*.hpp
)

target_sources(connix-core PUBLIC ${CONNIX_CORE_PUBLIC_HEADERS})
target_include_directories(connix-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Product
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
