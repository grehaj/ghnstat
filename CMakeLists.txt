cmake_minimum_required(VERSION 3.22)

project("tcpscan")

include_directories(include)

add_definitions(-std=c++17)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

set(sources
   src/Collector.cpp
   src/TrafficData.cpp
   src/TrafficStorage.cpp
   src/Utils.cpp
   )

add_executable(tcpscan main.cpp ${sources})


