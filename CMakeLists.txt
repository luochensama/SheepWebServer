cmake_minimum_required(VERSION 3.10)
project(SheepWebServer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
add_subdirectory(WebServer)