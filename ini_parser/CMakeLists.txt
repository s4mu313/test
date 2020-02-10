cmake_minimum_required(VERSION 3.5)

project(ini_parser)

set(CMAKE_BUILD_TYPE Release)
 
set(CMAKE_CXX_STANDARD 17)

# Include headers
include_directories(include)
 
# Include sources
file(GLOB SOURCES "src/*.cpp")
 
# Generate the shared library from the sources
add_library(ini_parser SHARED ${SOURCES})

target_include_directories(ini_parser INTERFACE include/)

# Copy header to dfu dir
install(DIRECTORY include DESTINATION ini_parser)

# Copy .so to dfu dir
install(TARGETS ini_parser DESTINATION /usr/lib) 
