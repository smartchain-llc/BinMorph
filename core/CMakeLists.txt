add_library(
  binmorph
  src/binary/binary_view.cxx
  src/codec/codec.cxx
  src/diagnostics/diagnostics.cxx
  src/export/exporters.cxx
  src/mapping/execution_plan.cxx
  src/mapping/mapped_document.cxx
  src/mapping/mapping_session.cxx
  src/model/format_model.cxx
  src/plugin/plugin_api.cxx
  src/schema/schema_parser.cxx
  src/schema/schema_registry.cxx
  src/schema/schema_source.cxx
  src/schema/schema_validator.cxx
)

target_compile_features(binmorph PUBLIC cxx_std_23)

target_include_directories(
  binmorph
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${JSON_INCLUDE_DIR}
)

target_link_libraries(
    binmorph
    gcov
)
target_compile_options(
  binmorph
  PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Wpedantic>
)

target_link_options(
    binmorph
    PUBLIC --coverage
)

target_compile_options(
    binmorph
    PUBLIC --coverage -O0 -g
)