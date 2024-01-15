cmake_minimum_required(VERSION 3.12.1)
project(proj)

set(CMAKE_CXX_STANDARD 14)

# 设置 OpenSSL 路径
#set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")

# 在项目中查找 OpenSSL
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "OpenSSL not found. Set the path to OpenSSL root folder in the system variable OPENSSL_ROOT_DIR.")
endif()

# 查找项目中的源文件
file(GLOB SRC "src/*.cpp")

# 添加可执行文件
add_executable(proj ${SRC})

# 链接 OpenSSL 库
target_link_libraries(proj ${OPENSSL_LIBRARIES})
