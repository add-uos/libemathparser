project(muparser VERSION ${PROJECT_VERSION})

# Add GNUInstallDirs for installation paths
include(GNUInstallDirs)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

find_package(Qt5 COMPONENTS Core  REQUIRED)

if (WIN32)
    add_library(muparser STATIC ${SOURCES})
elseif (UNIX)
    add_library(muparser SHARED ${SOURCES})
endif()

target_link_libraries(muparser
    PUBLIC
        Qt5::Core
)

install(TARGETS muparser DESTINATION /usr/lib)
file(GLOB HEADER_FILES "*.h")
install(FILES ${HEADER_FILES} DESTINATION /usr/inclue/libemathparser/)
