cmake_minimum_required(VERSION 3.24)
project(Lab6_Associate)

add_compile_options(-O1)
add_compile_options(-mavx2)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
set(CMAKE_CXX_STANDARD 20)

add_executable(Lab6_Associate main.cpp MulMatrices.h)
