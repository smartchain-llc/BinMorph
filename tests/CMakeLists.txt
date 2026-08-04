find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  )
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()

include(GoogleTest)
include(CTest)

function(add_binmorph_component_test target)
  add_executable(${target} ${ARGN})
  target_link_libraries(${target} PRIVATE binmorph GTest::gtest_main)
  target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  add_test(NAME ${target} COMMAND ${target})
  gtest_discover_tests(${target})
endfunction()

add_binmorph_component_test(binmorph_binary_tests binary/binary_view_tests.cxx)
add_binmorph_component_test(binmorph_codec_tests codec/codec_tests.cxx)
add_binmorph_component_test(binmorph_diagnostics_tests diagnostics/diagnostics_tests.cxx)
add_binmorph_component_test(binmorph_export_tests export/exporters_tests.cxx)
add_binmorph_component_test(binmorph_mapping_tests mapping/mapping_tests.cxx)
add_binmorph_component_test(binmorph_model_tests model/format_model_tests.cxx)
add_binmorph_component_test(binmorph_plugin_tests plugin/plugin_api_tests.cxx)
add_binmorph_component_test(binmorph_schema_tests schema/schema_tests.cxx)
