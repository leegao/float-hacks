cmake_minimum_required(VERSION 3.6)
project(fast_inverse_cube_root)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")

set(SOURCE_FILES inverse_cuberoot.cpp inverse_cuberoot.h floathacks/common.h floathacks/pow.h floathacks/consts/pow.h)

add_executable(fast_inverse_cube_root ${SOURCE_FILES})