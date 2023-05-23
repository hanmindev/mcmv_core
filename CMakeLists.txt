cmake_minimum_required(VERSION 3.25)
project(mcmv_core)

set(CMAKE_CXX_STANDARD 17)

add_executable(mcmv_core src/main.cpp src/math/quaternion.cpp src/math/quaternion.h src/math/vector3.h src/math/vector3.cpp)
