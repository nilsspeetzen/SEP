cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 14)
project(Solver)



set(as_subproject DcoCppCe BlockDeco Eigen3 GTest
    BlockDecoBTF Yasp Eso)


macro(find_package)
  if(NOT "${ARGV0}" IN_LIST as_subproject)
    _find_package(${ARGV})
  endif()
endmacro()

add_custom_target(doc)
add_dependencies(doc docBlockDeco)


add_subdirectory(GTest)
add_subdirectory(Yasp)
add_subdirectory(DcoCppCe)
add_subdirectory(Eigen3)
add_subdirectory(BlockDecoBTF)
add_subdirectory(BlockDeco)
add_subdirectory(Eso)
add_subdirectory(SingleFlash)

