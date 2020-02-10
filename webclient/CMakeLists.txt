cmake_minimum_required(VERSION 3.5)

project(webclient)

set(CMAKE_BUILD_TYPE Release)
 
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(spdlog INTERFACE ../)
 
add_executable(webclient main.cpp)
