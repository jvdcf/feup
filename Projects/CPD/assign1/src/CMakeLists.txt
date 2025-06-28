cmake_minimum_required(VERSION 3.22)
project(CPD_T09G13_P1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -Wall -Wextra -Wpedantic -Werror")

set(dir ${CMAKE_CURRENT_SOURCE_DIR})
set(EXECUTABLE_OUTPUT_PATH ${dir}/..)

# Project build
file(GLOB SOURCES ${dir}/*.cpp)
add_executable(CPD1 ${SOURCES})

find_library(PAPI_LIBRARY papi)
target_link_libraries(CPD1 ${PAPI_LIBRARY})
find_package(OpenMP REQUIRED)
target_link_libraries(CPD1 OpenMP::OpenMP_CXX)

