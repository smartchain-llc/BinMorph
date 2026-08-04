#pragma once

#include <binmorph/diagnostics.h>
#include <binmorph/format_model.h>
#include <binmorph/schema_parser.h>
#include <binmorph/schema_source.h>
#include <binmorph/schema_validator.h>
#include <binmorph/types.h>

#include <stdexcept>

namespace binmorph::tests
{

inline constexpr auto sample_schema_text = R"({
  "$id": "tests.sample",
  "version": "1",
  "architecture": [
    {
      "magic": { "len": 4, "endian": "little" },
      "header": { "len": 4, "endian": "big" },
      "payload": {
        "metadata": { "len": 8, "endian": "little" }
      }
    }
  ]
})";

inline FormatModel parse_valid_model()
{
  Diagnostics diagnostics;
  SchemaParser parser;
  SchemaValidator validator;

  auto model = parser.parse(SchemaSource::from_string(sample_schema_text, "tests/sample.json"),
                            diagnostics);
  if (!validator.validate(model, diagnostics) || diagnostics.has_errors())
    throw std::runtime_error("sample schema should parse and validate");

  return model;
}

inline Bytes sample_bytes()
{
  return {
      0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x02,
      0x10, 0x20, 0x30, 0x40,
      0x50, 0x60, 0x70, 0x80,
  };
}

} // namespace binmorph::tests
