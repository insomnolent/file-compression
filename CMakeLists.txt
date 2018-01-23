cmake_minimum_required(VERSION 3.5)
project(CS32_project4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES deltaforce.cpp hashtable.cpp)
add_executable(CS32_project4 ${SOURCE_FILES})