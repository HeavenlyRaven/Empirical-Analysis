cmake_minimum_required(VERSION 3.0)
project(Tarjan)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_BUILD_TYPE MATCHES Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

add_executable(Tarjan tarjan.cpp)
