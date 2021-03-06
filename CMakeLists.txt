cmake_minimum_required(VERSION 3.14)
project(Archiver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -O3")
SET(CMAKE_CXX_STANDARD_REQUIRED 11)
SET(CMAKE_BUILD_TYPE Debug)

set(BUILD_TESTS NO)

##############################################################################
# make archiver library

# source files
FILE(
        GLOB_RECURSE
        ARCHIVER_LIBRARY_SRC_FILES
        src/archiver/*.cpp
)

# include files
FILE(
        GLOB_RECURSE
        ARCHIVER_LIBRARY_SRC_FILES
        src/archiver/*.cpp
)

ADD_LIBRARY(
        archiver
        SHARED
        ${ARCHIVER_LIBRARY_SRC_FILES}
        ${ARCHIVER_LIBRARY_SRC_FILES}
)

ADD_EXECUTABLE(
        my_archiver
        main.cpp
)

TARGET_LINK_LIBRARIES(
        my_archiver
        archiver
)

##############################################################################
# Tests

if(${BUILD_TESTS})
    # Download googletest
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(
        GLOB_RECURSE
        ARCHIVER_TESTS_SRC_FILES
        tests/*test.cpp tests/run_tests.cpp
    )

    # Build one file for all tests
    ADD_EXECUTABLE(
            run-tests
            ${ARCHIVER_TESTS_SRC_FILES}
    )

    TARGET_LINK_LIBRARIES(
            run-tests
            archiver
            gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(run-tests)
endif()

##############################################################################