cmake_minimum_required(VERSION 3.23)
project(Lab4_Vertorization)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

add_compile_options(-mavx2)

add_executable(Lab4_Vertorization ManualOptimized.cpp)
