cmake_minimum_required(VERSION 3.20)

project(BinMorph VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(binmorph
    src/main.cpp
)

target_compile_options(binmorph PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

enable_testing()

add_test(
    NAME cli_contract
    COMMAND sh ${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_contract_test.sh $<TARGET_FILE:binmorph>
)
