cmake_minimum_required(VERSION 3.10)
project(libemathparser VERSION 0.1.0 LANGUAGES CXX)

# Build options
option(BUILD_EXAMPLES "Build example applications" ON)

# C++ standard settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.h"
)

# math parser qt interface
add_subdirectory(parser_qt)

# math parser cxx interface
add_subdirectory(parser_std)

# muparser interface
add_subdirectory(muparser_engine)

# build examples
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
