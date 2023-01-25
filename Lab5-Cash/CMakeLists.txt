cmake_minimum_required(VERSION 3.23)
project(Lab5_Cash)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab5_Cash
        Direct.cpp Function__GenerateArray.h)

add_compile_options(-O1)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)