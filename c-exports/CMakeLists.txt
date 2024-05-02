cmake_minimum_required(VERSION 3.19)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(libcexports DESCRIPTION "C Bindings for ppp" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CEXPORT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/CInterface.cpp")

add_compile_definitions(C_EXPORTS)

add_library(libcexports SHARED ${CEXPORT_SOURCES})
target_link_libraries(libcexports PRIVATE libppp)
target_include_directories(libcexports PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")
