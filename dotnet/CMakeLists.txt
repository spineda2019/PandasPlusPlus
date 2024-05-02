cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBNAME pppdotnet)

project(${LIBNAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMMON_LANGUAGE_RUNTIME "netcore")
add_compile_options("/clr")

set(DOTNET_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/FMatrix.cpp")
add_library(${LIBNAME} SHARED ${DOTNET_LIB_SOURCES})
target_link_libraries(${LIBNAME} PRIVATE libppp)
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
