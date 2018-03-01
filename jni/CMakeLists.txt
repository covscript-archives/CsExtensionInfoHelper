cmake_minimum_required(VERSION 3.4)
project(cshelper)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS " -fPIC -O3")

if (NOT DEFINED ENV{JAVA_HOME})
    message(FATAL_ERROR "Environment variable required: JAVA_HOME")
endif ()

include_directories(include)

set(JAVA_HOME $ENV{JAVA_HOME})
include_directories(${JAVA_HOME}/include)
IF (WIN32)
    include_directories(${JAVA_HOME}/include/windows)
ELSEIF (APPLE)
    include_directories(${JAVA_HOME}/include/darwin)
ELSEIF (UNIX)
    include_directories(${JAVA_HOME}/include/linux)
ENDIF ()

add_library(cshelper SHARED native.cpp)
