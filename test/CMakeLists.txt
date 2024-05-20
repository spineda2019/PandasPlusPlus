cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ppp-test DESCRIPTION "Test Suite for PandasPlusPlus" LANGUAGES CXX)

set(TEST_SOURCES
    "src/main.cpp"
    "src/benchmark.cpp"
    "src/column_tests.cpp"
    "src/matrix_tests.cpp")

add_executable(ppp-test ${TEST_SOURCES})

target_compile_features(ppp-test PUBLIC cxx_std_23)

if(BENCHMARK)
    message(">> PandasPlusPlus: Configuring with Benchmarking")
    target_compile_definitions(ppp-test PRIVATE BENCHMARK)
endif()

target_include_directories(ppp-test PRIVATE src/include)
target_link_libraries(ppp-test PRIVATE libppp)


add_test(NAME MainTest COMMAND $<TARGET_FILE:ppp-test>)
