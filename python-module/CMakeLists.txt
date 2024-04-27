project(pyppp DESCRIPTION "(Yet another) Matrix math module for python")

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(Python3 COMPONENTS Development Interpreter)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found! Ensure it is installed")
else()
    message("Python3 include dirs found @ ${Python_INCLUDE_DIRS}")
endif()
    

set(PY_SOURCES
    "src/pppmodule.c")

add_library(pyppp SHARED ${PY_SOURCES})

target_include_directories(pyppp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include" ${Python3_INCLUDE_DIRS})
