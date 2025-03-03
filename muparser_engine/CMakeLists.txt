project(muparser VERSION ${PROJECT_VERSION})

# Add GNUInstallDirs for installation paths
include(GNUInstallDirs)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

find_package(Qt5 COMPONENTS Core  REQUIRED)

add_library(muparser SHARED ${SOURCES})

target_link_libraries(muparser
    PUBLIC
        Qt5::Core
)
