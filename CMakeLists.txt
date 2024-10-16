cmake_minimum_required(VERSION 3.0)
PROJECT("Calculator")

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-Wno-deprecated-declarations -Wall -Werror -Wnon-virtual-dtor -Woverloaded-virtual")
#可执行文件输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#构建的库的输出目录
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin) 

add_executable(Calculator main.cpp)

