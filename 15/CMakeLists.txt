cmake_minimum_required(VERSION 3.6)
project(task15)

# added -fopenmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp")

set(CMAKE_CXX_STANDARD 14)

add_executable(task15 main.cpp Graph.cpp Graph.h DSU.cpp DSU.h)