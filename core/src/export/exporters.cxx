#include <binmorph/exporters.h>

#include <iomanip>

namespace binmorph
{

nlohmann::json ExporterSuite::to_json(const MappedDocument &document) const
{
  nlohmann::json root = nlohmann::json::array();
  for (const auto &value : document.values())
  {
    nlohmann::json bytes = nlohmann::json::array();
    for (const auto byte : value.raw_bytes)
      bytes.push_back(byte);

    root.push_back({
        {"path", value.path},
        {"offset", value.offset},
        {"length", value.length},
        {"endian", std::string{to_string(value.endian)}},
        {"display", value.decoded.display},
        {"rawBytes", std::move(bytes)},
    });
  }
  return root;
}

void ExporterSuite::write_summary(const MappedDocument &document,
                                  const Diagnostics &diagnostics,
                                  std::ostream &out) const
{
  out << "fields: " << document.values().size() << '\n';
  for (const auto &value : document.values())
  {
    out << value.path << " @ " << value.offset << " len " << value.length
        << " = " << value.decoded.display << '\n';
  }

  out << "diagnostics: " << diagnostics.entries().size() << '\n';
  for (const auto &diagnostic : diagnostics.entries())
    out << diagnostic.path << " @ " << diagnostic.offset << ": " << diagnostic.message << '\n';
}

} // namespace binmorph
