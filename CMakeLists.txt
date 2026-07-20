cmake_minimum_required(VERSION 3.20)

project(BinMorph VERSION 0.1.0 LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BINMORPH_ENABLE_COVERAGE "Build tests with gcov/lcov compatible coverage instrumentation" OFF)

add_library(binmorph_core
    src/binmorph.cpp
)

target_include_directories(binmorph_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_options(binmorph_core PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

if(BINMORPH_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(binmorph_core PRIVATE -O0 -g --coverage)
    target_link_options(binmorph_core PUBLIC --coverage)
endif()

add_executable(binmorph
    src/main.cpp
)

target_link_libraries(binmorph PRIVATE binmorph_core)

target_compile_options(binmorph PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

enable_testing()

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(binmorph_unit_tests
    tests/binmorph_core_test.cpp
)

target_link_libraries(binmorph_unit_tests PRIVATE
    binmorph_core
    GTest::gtest_main
    GTest::gmock
)

target_compile_definitions(binmorph_unit_tests PRIVATE
    BINMORPH_TEST_RESOURCES="${CMAKE_CURRENT_SOURCE_DIR}/tests/resources"
)

target_compile_options(binmorph_unit_tests PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

if(BINMORPH_ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(binmorph_unit_tests PRIVATE -O0 -g --coverage)
    target_link_options(binmorph_unit_tests PRIVATE --coverage)
endif()

include(GoogleTest)
gtest_discover_tests(binmorph_unit_tests)

add_test(
    NAME cli_contract
    COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_contract_test.sh $<TARGET_FILE:binmorph>
)

add_custom_target(format
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/format.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Format C and C++ source files"
)

add_custom_target(format-check
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-format.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Check C and C++ source formatting"
)

add_custom_target(docs-check
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-doxygen.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generate Doxygen documentation with warnings as errors"
)

add_custom_target(hygiene-check
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/scripts/check-repository-hygiene.sh
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Check repository hygiene"
)
