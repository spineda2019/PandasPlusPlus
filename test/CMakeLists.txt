cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(test DESCRIPTION "Test Suite for PandasPlusPlus" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(TEST_SOURCES
    "src/main.cpp"
    "src/benchmark.cpp"
    "src/column_tests.cpp"
    "src/matrix_tests.cpp")

add_executable(test ${TEST_SOURCES})

if(BENCHMARK)
    message(">> PandasPlusPlus: Configuring with Benchmarking")
    target_compile_definitions(test PRIVATE BENCHMARK)
endif()

target_include_directories(test PRIVATE src/include)
target_link_libraries(test PRIVATE libppp)
