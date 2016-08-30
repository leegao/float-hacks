cmake_minimum_required(VERSION 3.6)
project(fast_inverse_cube_root)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES inverse_cuberoot.c inverse_cuberoot.h)

add_executable(fast_inverse_cube_root ${SOURCE_FILES})