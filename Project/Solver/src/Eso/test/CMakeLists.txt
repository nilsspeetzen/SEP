cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)

file(GLOB Eso_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/Eso/*.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/Eso/Models/*.hpp
)

add_executable(testEso ${Eso_HEADERS}
    FirstOrderEso.test.cpp
)

target_link_libraries(testEso Eso::Eso GTest::GTest GTest::Main)
