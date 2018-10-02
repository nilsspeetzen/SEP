cmake_minimum_required(VERSION 3.5)

find_package(GTest REQUIRED)


file(GLOB BlockDeco_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/../include/BlockDeco/*.hpp
)


add_executable(testBlockDeco ${BlockDeco_HEADERS}
    EigenHybridNonlinearSolver.test.cpp
    AlgebraicEsoSolver.test.cpp
    AlgebraicEsoBlockSolver.test.cpp
    )

target_link_libraries(testBlockDeco BlockDeco::BlockDeco GTest::GTest GTest::Main
Eso::Eso)
