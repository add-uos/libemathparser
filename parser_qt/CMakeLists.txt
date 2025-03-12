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

if (WIN32)
    add_library(eparserqt STATIC ${SOURCES})
elseif (UNIX)
    add_library(eparserqt SHARED ${SOURCES})
endif()

target_link_libraries(eparserqt
    PUBLIC
        Qt5::Core
)

install(TARGETS eparserqt DESTINATION /usr/lib)
file(GLOB HEADER_FILES "*.h")
install(FILES ${HEADER_FILES} DESTINATION /usr/inclue/libemathparser/)
