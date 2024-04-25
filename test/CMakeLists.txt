cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(test DESCRIPTION "Test Suite for PandasPlusPlus" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_SOURCES
    "main.cpp")

add_executable(test ${TEST_SOURCES})

target_link_libraries(test PRIVATE libppp)
