cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(PandasPlusPlus CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_subdirectory(ppp)
if(WIN32 AND MSVC)
    add_subdirectory(dotnet)
else()
    message("NOTICE: dotnet clr library must be build on windows with msvc, skipping...")
endif()
add_subdirectory(c-exports)
add_subdirectory(test)
add_subdirectory(python-module)
