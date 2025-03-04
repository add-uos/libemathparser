project(emathparser-std VERSION ${PROJECT_VERSION})

# Add GNUInstallDirs for installation paths
include(GNUInstallDirs)

file(GLOB SOURCES
    "*.cpp"
    "*.h"
)

add_library(eparserstd SHARED ${SOURCES})

install(TARGETS eparserstd DESTINATION /usr/lib)
file(GLOB HEADER_FILES "*.h")
install(FILES ${HEADER_FILES} DESTINATION /usr/inclue/libemathparser/)