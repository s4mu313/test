cmake_minimum_required(VERSION 3.5)

project(webserver)

set(CMAKE_BUILD_TYPE Release)
 
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(spdlog INTERFACE ../)

include_directories(/usr/local/ini_parser/include/)

link_directories(/usr/lib/)

find_package (Threads)

add_executable(webserver main.cpp)

target_link_libraries (webserver Threads::Threads)

target_link_libraries(webserver ini_parser)
