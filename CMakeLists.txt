cmake_minimum_required(VERSION 2.8)
project(PriorityQueue)


set(GTest_ROOT /usr/src/googletest/googletest)
add_subdirectory(${GTest_ROOT}
        "${CMAKE_CURRENT_BINARY_DIR}/googletest" EXCLUDE_FROM_ALL)
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(. COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "*.cpp")
list(REMOVE_ITEM TEST_FILES "tests.cpp")

add_executable("priority_queue_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("priority_queue_tests" gtest pthread PRIORITY_QUEUE)
add_dependencies(. "priority_queue_tests")
add_test("priority_queue_tests" "./priority_queue_tests")