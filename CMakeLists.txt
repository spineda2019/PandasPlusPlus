cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(PandasPlusPlus DESCRIPTION "Fast Pandas Alt written in C++ for various target langs")

add_subdirectory(ppp)

if(WIN32 AND MSVC AND (BUILD_ALL OR BUILD_DOTNET) AND VS_GEN)
    message(">> PandasPlusPlus: Configuring Dotnet CLR Library")
    add_subdirectory(dotnet)
else()
    message(">> NOTICE: dotnet clr library must be build on windows with msvc, skipping...")
endif()

if(BUILD_ALL OR BUILD_PYTHON_MODULE)
    message(">> PandasPlusPlus: Configuring C Exports & Python Module Library")
    add_subdirectory(c-exports)
    add_subdirectory(python-module)
elseif(BUILD_EXPORTS)
    message(">> PandasPlusPlus: Configuring C Exports Library")
    add_subdirectory(c-exports)
endif()

if(BUILD_ALL OR BUILD_TESTS)
    message(">> PandasPlusPlus: Configuring Test Executable")
    enable_testing()
    add_subdirectory(test)
endif()
