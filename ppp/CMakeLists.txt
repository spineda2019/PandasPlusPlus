cmake_minimum_required(VERSION 3.19)

project(libppp DESCRIPTION "Pandas Plus Plus Library")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PPP_SOURCES
    "CCSVInterface.cpp"
    "CCSVInterface.h"
    "Dataframe.h"
    "Exceptions.h"
    )

add_library(libppp ${MODULE_BUILD_TYPE} ${PPP_SOURCES})
