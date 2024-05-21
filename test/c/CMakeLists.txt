cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(ctest DESCRIPTION "Test Suite for PandasPlusPlus" LANGUAGES C)

set(CTEST_SOURCES
    "src/main.c")

add_executable(ctest ${CTEST_SOURCES})
target_compile_features(ctest PUBLIC c_std_17)
target_link_libraries(ctest PRIVATE libcexports)
target_include_directories(ctest PRIVATE src/include)
add_test(NAME CTest COMMAND $<TARGET_FILE:ctest>)
