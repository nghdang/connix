file(GLOB_RECURSE CONNIX_GUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Bootstrap/*.cpp
)

target_sources(connix-gui PRIVATE ${CONNIX_GUI_SOURCES})
