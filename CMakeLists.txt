cmake_minimum_required(VERSION 3.17)
project(cis541)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++14")

include_directories(.)
include_directories(exmaples)
include_directories(hw2)
include_directories(Test)

set(SOURCE_FILES
        hw2/stopwatch.cpp
        )

add_executable(cis541
        ${SOURCE_FILES})
