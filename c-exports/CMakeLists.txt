cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libcexports DESCRIPTION "C Bindings for ppp" LANGUAGES C CXX)

set(CEXPORT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/CInterface.cpp")

add_library(libcexports SHARED ${CEXPORT_SOURCES})

target_compile_features(libcexports PRIVATE cxx_std_23)

target_compile_definitions(libcexports PRIVATE C_EXPORTS)

target_link_libraries(libcexports PRIVATE libppp)
target_include_directories(libcexports PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
