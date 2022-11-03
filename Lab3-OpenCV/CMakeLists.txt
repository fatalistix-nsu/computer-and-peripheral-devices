cmake_minimum_required(VERSION 3.23)
project(Lab3-OpenCV)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Lab3-OpenCV main.cpp)

find_package( OpenCV REQUIRED )
#find_package(  REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
target_link_libraries( Lab3-OpenCV ${OpenCV_LIBS} )