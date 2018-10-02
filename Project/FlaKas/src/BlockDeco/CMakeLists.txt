cmake_minimum_required(VERSION 2.8.12)

project(BlockDeco)

find_package(BlockDecoBTF REQUIRED)
find_package(Eso REQUIRED)
find_package(Eigen3 REQUIRED)


####################
### core library ###
####################


add_library(BlockDeco INTERFACE )

add_library(BlockDeco::BlockDeco ALIAS BlockDeco)


target_include_directories(BlockDeco
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
     $<INSTALL_INTERFACE:include>)


target_link_libraries(BlockDeco
    INTERFACE BlockDecoBTF::BlockDecoBTF
    INTERFACE Eigen3::Eigen)

### documentation and testing
add_subdirectory(doc)

enable_testing()
add_subdirectory(test)


