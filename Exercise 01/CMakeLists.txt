cmake_minimum_required(VERSION 3.23)
project(Exercise_1)

set(CMAKE_CXX_STANDARD 14)
set(GCC_ENABLE_WARNINGS "-O1 -Wall -Werror -Wextra -Wvla -Wno-unused-result")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_ENABLE_WARNINGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

include_directories(.)

add_executable(main
        main.cpp
        osm.h
        osm.cpp)
