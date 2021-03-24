# Preamble
cmake_minimum_required (VERSION 3.19.2)
project(ParseFlags)

# Project wide setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_C_FLAGS "-g -Wall -Wextra -Werror")
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror")
set(CMAKE_CXX_EXTENSIONS NO)

add_subdirectory(src)
add_subdirectory(demo EXCLUDE_FROM_ALL)
