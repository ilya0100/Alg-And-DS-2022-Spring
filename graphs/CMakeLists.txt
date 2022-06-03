cmake_minimum_required(VERSION 3.14)
project(GraphsTests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(
    ${PROJECT_NAME}
    tests/test.cpp
    src/ListGraph.cpp
    src/MatrixGraph.cpp
    src/ArcGraph.cpp
    src/SetGraph.cpp
)

target_link_libraries(${PROJECT_NAME} GTest::GTest gtest_main Threads::Threads)
