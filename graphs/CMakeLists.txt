project(GraphsTests)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(include)

add_executable(
    ${PROJECT_NAME}
    tests/test.cpp
    src/ListGraph.cpp
    src/MatrixGraph.cpp
    src/ArcGraph.cpp
)

target_link_libraries(${PROJECT_NAME} GTest::GTest gtest_main Threads::Threads)
