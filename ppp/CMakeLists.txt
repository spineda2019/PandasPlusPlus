cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libppp DESCRIPTION "Pandas Plus Plus Library" LANGUAGES CXX)

add_library(libppp INTERFACE)

target_compile_features(libppp INTERFACE cxx_std_23)

target_include_directories(libppp INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")
