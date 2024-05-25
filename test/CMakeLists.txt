cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ppp-test DESCRIPTION "Test Suite for PandasPlusPlus")

add_subdirectory(c)
add_subdirectory(cpp)
add_subdirectory(python)
