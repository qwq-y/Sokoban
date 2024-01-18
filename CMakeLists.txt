cmake_minimum_required(VERSION 3.12.1)
project(proj)

set(CMAKE_CXX_STANDARD 17)

# 查找项目中的源文件
file(GLOB SRC "src/*.cpp")

# 添加可执行文件
add_executable(proj ${SRC})
