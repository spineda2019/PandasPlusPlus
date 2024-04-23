cmake_minimum_required(VERSION 3.19)

project(test DESCRIPTION "Test Suite for PandasPlusPlus")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_SOURCES
    "main.cpp")

add_executable(test ${TEST_SOURCES})

target_link_libraries(libppp)
