cmake_minimum_required(VERSION 3.26)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LIBNAME pppdotnet)


project(${LIBNAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DOTNET_LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/FMatrix.cpp")
add_library(${LIBNAME} SHARED ${DOTNET_LIB_SOURCES})
target_link_libraries(${LIBNAME} PRIVATE libppp)
target_include_directories(${LIBNAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
set_target_properties(${LIBNAME} PROPERTIES COMMON_LANGUAGE_RUNTIME "")
target_compile_options(${LIBNAME} BEFORE PUBLIC "/clr")

