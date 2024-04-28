
cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBNAME pppdotnet)

project(${LIBNAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DOTNET_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/dotnet.cpp")
add_library(${LIBNAME} SHARED ${DOTNET_LIB_SOURCES})
target_link_libraries(${LIBNAME} PRIVATE libppp)
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
add_compile_options("/clr")
