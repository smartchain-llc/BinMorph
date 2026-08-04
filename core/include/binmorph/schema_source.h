#pragma once

#include <filesystem>
#include <string>

namespace binmorph
{

class SchemaSource
{
public:
  static SchemaSource from_string(std::string text, std::string uri = "memory://schema");
  static SchemaSource from_file(const std::filesystem::path &path);

  [[nodiscard]] const std::string &uri() const noexcept;
  [[nodiscard]] const std::string &text() const noexcept;

private:
  SchemaSource(std::string uri, std::string text);

  std::string uri_;
  std::string text_;
};

} // namespace binmorph
