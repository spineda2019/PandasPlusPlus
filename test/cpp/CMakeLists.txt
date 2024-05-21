cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(cpptest DESCRIPTION "Test Suite for PandasPlusPlus" LANGUAGES CXX C)

set(TEST_SOURCES
    "src/main.cpp"
    "src/benchmark.cpp"
    "src/column_tests.cpp"
    "src/matrix_tests.cpp")

add_executable(cpptest ${TEST_SOURCES})

target_compile_features(cpptest PUBLIC cxx_std_23)

if(BENCHMARK)
    message(">> PandasPlusPlus: Configuring with Benchmarking")
    target_compile_definitions(cpptest PRIVATE BENCHMARK)
endif()

target_include_directories(cpptest PRIVATE src/include)
target_link_libraries(cpptest PRIVATE libppp)

add_test(NAME CppTest COMMAND $<TARGET_FILE:cpptest>)

