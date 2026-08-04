cmake_minimum_required(VERSION 3.20)
project(BinMorph)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

configure_file(thirdparty/json.hpp include/json.hpp)
set(JSON_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
add_subdirectory(core)

option(BUILD_EXAMPLES "Build BinMorph examples" ON)
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

option(BUILD_TESTS "Build BinMorph tests" ON)
if(BUILD_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 --coverage")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 --coverage")
  enable_testing()
  message("== [BinMorph] Building Tests")
  add_subdirectory(tests)

  find_program(LCOV_EXECUTABLE lcov)
  if(LCOV_EXECUTABLE)
    add_custom_target(
      coverage_reset
      COMMAND ${LCOV_EXECUTABLE}
        --quiet
        --zerocounters
        --directory ${CMAKE_BINARY_DIR}
      COMMENT "Resetting coverage counters"
      VERBATIM
    )

    add_custom_target(
      coverage
      COMMAND ${LCOV_EXECUTABLE}
        --quiet
        --capture
        --directory ${CMAKE_BINARY_DIR}
        --base-directory ${CMAKE_SOURCE_DIR}
        --include "${CMAKE_SOURCE_DIR}/core/*"
        --output-file ${CMAKE_BINARY_DIR}/coverage.info
      COMMENT "Generating LCOV coverage report at ${CMAKE_BINARY_DIR}/coverage.info"
      VERBATIM
    )
  else()
    add_custom_target(
      coverage_reset
      COMMAND ${CMAKE_COMMAND} -E echo "lcov was not found; install lcov to reset coverage counters."
      COMMAND ${CMAKE_COMMAND} -E false
      VERBATIM
    )

    add_custom_target(
      coverage
      COMMAND ${CMAKE_COMMAND} -E echo "lcov was not found; install lcov to generate ${CMAKE_BINARY_DIR}/coverage.info."
      COMMAND ${CMAKE_COMMAND} -E false
      VERBATIM
    )
  endif()
endif()
