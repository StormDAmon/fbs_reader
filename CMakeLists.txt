cmake_minimum_required(VERSION 3.5.1)
# 编译器
set(CMAKE_CXX_STANDARD 11)
#-C编译器
SET(CMAKE_C_COMPILER                "gcc")
SET(CMAKE_C_FLAGS                   "-Wall -std=c99 -fPIC")
SET(CMAKE_C_FLAGS_DEBUG             "$ENV{CXXFLAGS} -O0 -w -g -ggdb -Wno-pragma-once-outside-header")
SET(CMAKE_C_FLAGS_MINSIZEREL        "-Os -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE           "$ENV{CXXFLAGS} -w -O4 -Wno-pragma-once-outside-header")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")
#-C艹编译器
SET(CMAKE_CXX_COMPILER              "g++")
SET(CMAKE_CXX_FLAGS                 "-Wall -fPIC")
SET(CMAKE_CXX_FLAGS_DEBUG           "$ENV{CXXFLAGS} -O0 -w -g -ggdb -Wno-pragma-once-outside-header")
SET(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE         "$ENV{CXXFLAGS} -w -O4 -Wno-pragma-once-outside-header")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
add_definitions(-std=c++11)
add_definitions(-D__LINUX__=1)
add_definitions(-DBOOST_UTILS=1)
add_definitions(-DBOOST_MODE=1)

# 工程
project(fbs_reader VERSION "1.0.0")
message("-- System: " ${CMAKE_SYSTEM_NAME})
if (CMAKE_BUILD_TYPE MATCHES "Release")
else ()
    add_definitions(-DDEBUG)
    set (CMAKE_BUILD_TYPE "Debug")
endif ()
message("-- Current Mode: " ${CMAKE_BUILD_TYPE})




include_directories(${CMAKE_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory(src)
add_subdirectory(tests)


