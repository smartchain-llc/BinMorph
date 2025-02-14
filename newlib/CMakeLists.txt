cmake_minimum_required(VERSION 3.2)
project(newlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(JSON_INCLUDE_DIR build/include)

configure_file(thirdparty/json.hpp include/json.hpp)
add_library(
    bm
    SHARED
    core/Attributes.cxx
    core/Schema.cxx
    core/SchemaRegister.cxx
)
target_include_directories(
    bm
    PUBLIC core
    PUBLIC ${JSON_INCLUDE_DIR}
)
target_compile_definitions(
    bm
    PUBLIC fPIC
    PUBLIC shared
    PUBLIC Wl
    PUBLIC no-undefined
)
set_target_properties(bm PROPERTIES CXX_STANDARD 20)
add_executable(
    newlibCtest
    tests/lib.c
)
target_link_libraries(newlibCtest bm)