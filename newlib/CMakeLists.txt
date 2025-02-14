cmake_minimum_required(VERSION 3.2)
project(newlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(
    bm
    SHARED
    core/SchemaRegister.cxx
)
target_link_directories(
    bm
    PUBLIC .
)
target_compile_definitions(
    bm
    PUBLIC fPIC
    PUBLIC shared
)