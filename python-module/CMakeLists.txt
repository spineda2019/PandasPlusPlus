project(pyppp DESCRIPTION "(Yet another) Matrix math module for python" LANGUAGES C)

find_package(Python3 COMPONENTS Development Interpreter)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found! Ensure it is installed")
else()
    message("Python3 include dirs found @ ${Python3_INCLUDE_DIRS}")
endif()
    

set(PY_SOURCES
    "src/pppmodule.c")

add_library(pyppp SHARED ${PY_SOURCES})

target_compile_features(pyppp PUBLIC c_std_17)

target_include_directories(pyppp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include" ${Python3_INCLUDE_DIRS})

target_link_directories(pyppp PUBLIC "${Python3_LIBRARY_DIRS}")
