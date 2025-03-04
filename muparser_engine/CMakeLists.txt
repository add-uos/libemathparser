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

install(TARGETS muparser DESTINATION /usr/lib)
file(GLOB HEADER_FILES "*.h")
install(FILES ${HEADER_FILES} DESTINATION /usr/inclue/libemathparser/)