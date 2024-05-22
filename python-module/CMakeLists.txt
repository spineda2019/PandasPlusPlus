project(pyppp DESCRIPTION "(Yet another) Matrix math module for python" LANGUAGES CXX)

find_package(Python3 COMPONENTS Development Interpreter)

if(NOT Python3_FOUND)
    message(FATAL_ERROR "Python3 not found! Ensure it is installed")
else()
    message("Python3 include dirs found @ ${Python3_INCLUDE_DIRS}")
endif()
    

set(PY_SOURCES
    "src/pppmodule.cpp")

add_library(pyppp SHARED ${PY_SOURCES})

set_target_properties(pyppp PROPERTIES OUTPUT_NAME ppp PREFIX "")

target_compile_features(pyppp PUBLIC cxx_std_23)

target_include_directories(pyppp PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include" ${Python3_INCLUDE_DIRS})

target_link_directories(pyppp PUBLIC "${Python3_LIBRARY_DIRS}")
