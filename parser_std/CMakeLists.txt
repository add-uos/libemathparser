project(emathparser-std VERSION ${PROJECT_VERSION})

# Add GNUInstallDirs for installation paths
include(GNUInstallDirs)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_library(eparserstd SHARED ${SOURCES})

