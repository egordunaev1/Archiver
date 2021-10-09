cmake_minimum_required(VERSION 3.14)
project(PriorityQueue)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")


# Setup testing
enable_testing()
add_executable(
        huffman_trie_test
        huffman_trie_test.cpp
)
target_link_libraries(
        huffman_trie_test
        gtest_main
)
include(GoogleTest)
gtest_discover_tests(huffman_trie_test)