cmake_minimum_required(VERSION 3.14)
project(PriorityQueue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")


# Setup testing
enable_testing()

add_executable(
        priority_queue_test
        priority_queue_test.cpp
)
target_link_libraries(
        priority_queue_test
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(priority_queue_test)
