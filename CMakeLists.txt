cmake_minimum_required(VERSION 3.22)

project(ghnstat VERSION 1.0)

include_directories(include)

add_definitions(-std=c++17)

set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

add_subdirectory(collector)

add_executable(ghnstat main.cpp ${sources})
target_link_libraries(ghnstat PUBLIC collector)


