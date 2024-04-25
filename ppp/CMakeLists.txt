cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libppp DESCRIPTION "Pandas Plus Plus Library" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(libppp INTERFACE)

target_include_directories(libppp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
