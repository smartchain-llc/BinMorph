#include <binmorph/binmorph.h>

#include <iomanip>
#include <iostream>
#include <string>

namespace
{

  std::string example_schema()
  {
    return R"({
    "$id": "examples.some-data",
    "version": "1",
    "architecture": [
      {
        "magic": { "len": 4, "endian": "little" },
        "header": { "len": 4, "endian": "little" },
        "payload": {
          "metadata": { "len": 8, "endian": "little" }
        }
      }
    ]
  })";
  }

  binmorph::Bytes example_bytes()
  {
    return {
        0x42, 0x4d, 0x4f, 0x52,
        0x01, 0x00, 0x00, 0x00,
        0xde, 0xad, 0xbe, 0xef,
        0x10, 0x20, 0x30, 0x40,
        0x99, 0x88};
  }

  void print_banner(std::string_view title)
  {
    std::cout << "\n== " << title << " ==\n";
  }

  void print_diagnostics(const binmorph::Diagnostics &diagnostics)
  {
    if (diagnostics.entries().empty())
    {
      std::cout << "No diagnostics.\n";
      return;
    }

    for (const auto &diagnostic : diagnostics.entries())
    {
      std::cout << diagnostic.path << " @ " << diagnostic.offset << ": "
                << diagnostic.message << '\n';
    }
  }

} // namespace

int main()
{
  binmorph::Diagnostics schema_diagnostics;
  binmorph::SchemaParser parser;
  binmorph::SchemaValidator validator;

  print_banner("1. Load Schema Source");
  const auto source = binmorph::SchemaSource::from_string(example_schema(), "examples/some-data.json");
  std::cout << "source: " << source.uri() << '\n';

  print_banner("2. Parse And Validate Schema");
  auto model = parser.parse(source, schema_diagnostics);
  const auto valid = validator.validate(model, schema_diagnostics);
  std::cout << "schema id: " << model.schema_id() << '\n';
  std::cout << "version: " << model.version() << '\n';
  std::cout << "valid: " << std::boolalpha << valid << '\n';
  print_diagnostics(schema_diagnostics);

  print_banner("3. Compile Execution Plan");
  binmorph::MappingSession session{model};
  std::size_t next_offset{0};
  for (const auto &step : session.plan().steps())
  {
    std::cout << std::setw(36) << std::left << step.path
              << " offset " << std::setw(2) << next_offset
              << " len " << std::setw(2) << step.length
              << " endian " << binmorph::to_string(step.endian) << '\n';
    next_offset += step.length;
  }

  print_banner("4. Parse Arbitrary Binary Input");
  auto input = binmorph::BinaryView::from_bytes(example_bytes(), "example-buffer");
  auto parsed = session.parse(input);
  for (const auto &value : parsed.document.values())
  {
    std::cout << std::setw(36) << std::left << value.path
              << " raw[";
    for (const auto byte : value.raw_bytes)
    {
      std::cout << " 0x" << std::hex << std::setw(2) << std::setfill('0')
                << static_cast<int>(byte) << std::dec << std::setfill(' ');
    }
    std::cout << " ] decoded=" << value.decoded.display << '\n';
  }

  print_banner("5. Diagnostics");
  print_diagnostics(parsed.diagnostics);

  print_banner("6. Export JSON Projection");
  binmorph::ExporterSuite exporters;
  std::cout << exporters.to_json(parsed.document).dump(2) << '\n';

  print_banner("7. Rebuild Binary From Mapped Document");
  binmorph::Diagnostics build_diagnostics;
  const auto rebuilt = session.build(parsed.document, build_diagnostics);
  std::cout << "rebuilt bytes: " << rebuilt.size() << '\n';
  print_diagnostics(build_diagnostics);

  return schema_diagnostics.has_errors() || parsed.diagnostics.has_errors() ||
                 build_diagnostics.has_errors()
             ? 1
             : 0;
}
