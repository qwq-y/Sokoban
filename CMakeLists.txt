cmake_minimum_required(VERSION 3.12.1)
project(proj)

set(CMAKE_CXX_STANDARD 14)

file(GLOB SRC "src/*.cpp")
add_executable(proj ${SRC})
