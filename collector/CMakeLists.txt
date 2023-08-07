cmake_minimum_required(VERSION 3.22)

project("ghnstat")

add_definitions(-std=c++17)


find_package(nlohmann_json 3.2.0 REQUIRED)

add_library(collector STATIC
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Collector.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/CollectorThread.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/JsonConversion.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TrafficData.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TrafficReader.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TrafficStorage.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/TrafficWritter.cpp
   ${CMAKE_CURRENT_SOURCE_DIR}/src/Utils.cpp
)

target_include_directories(collector PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(collector PRIVATE nlohmann_json::nlohmann_json)


