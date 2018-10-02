cmake_minimum_required(VERSION 3.5)
project(SingleFlash)

find_package(BlockDeco REQUIRED)
find_package(Eso REQUIRED)
find_package(Eigen3 REQUIRED)

add_executable(singleFlash main.cpp)
target_link_libraries(singleFlash Eso::Eso BlockDeco::BlockDeco Eigen3::Eigen)
