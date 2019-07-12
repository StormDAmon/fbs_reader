cmake_minimum_required(VERSION 3.5.1)
project(fbs_reader VERSION "1.0.0")

set(CMAKE_BUILD_TYPE "Release")
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG)
else()
    set(CMAKE_BUILD_TYPE "Release")
endif (CMAKE_BUILD_TYPE MATCHES "Debug")
message("-- Current Mode: " ${CMAKE_BUILD_TYPE})

# 编译参数
set(CMAKE_CXX_STANDARD 14)
#-C编译器
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_C_COMPILER            "/usr/bin/clang")
endif ()
SET(CMAKE_C_FLAGS                   "-Wall -std=c99")
SET(CMAKE_C_FLAGS_DEBUG             "$ENV{CXXFLAGS} -O0 -w -g -ggdb -fPIC")
SET(CMAKE_C_FLAGS_MINSIZEREL        "-Os -DNDEBUG")
SET(CMAKE_C_FLAGS_RELEASE           "$ENV{CXXFLAGS} -w -fPIC -O4 -DNDEBUG")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO    "-O2 -g")
#-C艹编译器
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_CXX_COMPILER          "/usr/bin/clang++")
endif ()
SET(CMAKE_CXX_COMPILER              "/usr/bin/clang++")
SET(CMAKE_CXX_FLAGS                 "-Wall")
SET(CMAKE_CXX_FLAGS_DEBUG           "$ENV{CXXFLAGS} -O0 -w -g -ggdb -fPIC")
SET(CMAKE_CXX_FLAGS_MINSIZEREL      "-Os -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELEASE         "$ENV{CXXFLAGS} -w -fPIC -O4 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO  "-O2 -g")
if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    SET(CMAKE_AR                    "/usr/bin/llvm-ar")
    SET(CMAKE_LINKER                "/usr/bin/llvm-ld")
    SET(CMAKE_NM                    "/usr/bin/llvm-nm")
    SET(CMAKE_OBJDUMP               "/usr/bin/llvm-objdump")
    SET(CMAKE_RANLIB                "/usr/bin/llvm-ranlib")
endif ()
add_definitions(-std=c++11)
add_definitions(-D__LINUX__=1)
add_definitions(-DBOOST_UTILS=1)
add_definitions(-DBOOST_MODE=1)


SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O -w -g -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -w")
add_definitions(-std=c++17)
add_definitions(-D__LINUX__=1)


#Global include path for all libs.
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(src)
add_subdirectory(tests)


