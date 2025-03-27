cmake_minimum_required(VERSION 3.2)
project(newlib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(JSON_INCLUDE_DIR build/include)

configure_file(thirdparty/json.hpp include/json.hpp)

file(GLOB_RECURSE LIBSRC "core/*.cxx")

add_library(
    bm
    SHARED
    ${LIBSRC}
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
  # option(Boost_INCLUDE_DIR "Path to Boost include" /usr/local/boost)
  # message( "== [BinMorph] Boost Root: ${Boost_INCLUDE_DIR}")
  # find_package(Boost REQUIRED)
  # add_compile_definitions(CROW_USE_BOOST)
  add_subdirectory(tests/core)
endif()