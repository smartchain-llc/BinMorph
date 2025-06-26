cmake_minimum_required(VERSION 3.10)
project(BinMorph)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(thirdparty/json.hpp include/json.hpp)
set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
add_subdirectory(core)

option(BUILD_TESTS "Build the IFR Test Files" ON)
if(${BUILD_TESTS})

  message("== [BinMorph] Building Tests")

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  )

  FetchContent_MakeAvailable(googletest)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(tests)
endif()