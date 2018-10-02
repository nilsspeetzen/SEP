cmake_minimum_required(VERSION 2.6)

project(gt1s)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

add_executable(gt1s main.cpp driver.cpp driver.hpp f.hpp)
