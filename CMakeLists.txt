cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(PandasPlusPlus CXX)

option(BUILD_STATIC "Build static libraries rather than shared" OFF)

if(${BUILD_STATIC})
    message("Building libraries as static libs")
    set(MODULE_BUILD_TYPE STATIC)
else()
  message("Building libraries as shared libs")
  set(MODULE_BUILD_TYPE SHARED)
endif()

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(ppp)
add_subdirectory(test)
