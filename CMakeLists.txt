cmake_minimum_required(VERSION 3.5.1)
project(fbs_reader VERSION "1.0.0")

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
else()
    set(CMAKE_BUILD_TYPE "Release")
endif (CMAKE_BUILD_TYPE MATCHES "Debug")

message("-- Current Mode: " ${CMAKE_BUILD_TYPE})

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O -w -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -w")

add_definitions(-std=c++11)
add_definitions(-D__LINUX__=1)


#Global include path for all libs.
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(tests)


