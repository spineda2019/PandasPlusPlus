cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(PandasPlusPlus CXX)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

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
