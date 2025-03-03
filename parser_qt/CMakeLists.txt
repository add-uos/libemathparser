project(emathparser-qt VERSION ${PROJECT_VERSION})

# Add GNUInstallDirs for installation paths
include(GNUInstallDirs)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core  REQUIRED)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_library(eparserqt SHARED ${SOURCES})

target_link_libraries(eparserqt
    PUBLIC
        Qt5::Core
)
