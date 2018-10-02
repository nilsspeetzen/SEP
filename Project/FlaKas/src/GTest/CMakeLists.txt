cmake_minimum_required(VERSION 3.5)
message("Adding GTest as subproject")
option(BUILD_GTEST "" ON)
option(BUILD_GMOCK "" OFF)
if(MSVC)
   option(gtest_force_shared_crt "" ON)
   message("Building GTest with dynamic runtime libraries")
endif()
add_subdirectory(googletest GTest)
add_library(GTest::GTest ALIAS gtest) ## alias by raha01
add_library(GTest::Main ALIAS gtest_main) ## alias by raha01
