cmake_minimum_required(VERSION 3.22)

project("tcpstat")

include_directories(include)

add_definitions(-std=c++17)

find_package(nlohmann_json 3.2.0 REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAXE_CXX_FLAGS} -g -Wall")

set(sources
   src/Collector.cpp
   src/JsonConversion.cpp
   src/TrafficData.cpp
   src/TrafficReader.cpp
   src/TrafficStorage.cpp
   src/TrafficWritter.cpp
   src/Utils.cpp
   )

add_executable(tcpstat main.cpp ${sources})

target_link_libraries(tcpstat PRIVATE nlohmann_json::nlohmann_json)


