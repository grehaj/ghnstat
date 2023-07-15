cmake_minimum_required(VERSION 3.22)

project("scan_detector")

include_directories(include)

add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall -pthread")

set(sources
   src/Collector.cpp
   )

add_executable(scan_detector main.cpp ${sources})


