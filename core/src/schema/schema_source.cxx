#include <binmorph/schema_source.h>

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace binmorph
{

SchemaSource SchemaSource::from_string(std::string text, std::string uri)
{
  return SchemaSource(std::move(uri), std::move(text));
}

SchemaSource SchemaSource::from_file(const std::filesystem::path &path)
{
  std::ifstream input{path};
  if (!input)
    throw std::runtime_error("failed to open schema file: " + path.string());

  std::ostringstream text;
  text << input.rdbuf();
  return SchemaSource(path.string(), text.str());
}

const std::string &SchemaSource::uri() const noexcept
{
  return uri_;
}

const std::string &SchemaSource::text() const noexcept
{
  return text_;
}

SchemaSource::SchemaSource(std::string uri, std::string text)
    : uri_{std::move(uri)}, text_{std::move(text)}
{
}

} // namespace binmorph
