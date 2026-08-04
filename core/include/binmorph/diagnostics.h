#pragma once

#include "types.h"

#include <string>
#include <vector>

namespace binmorph
{

enum class Severity
{
  Info,
  Warning,
  Error
};

struct Diagnostic
{
  Severity severity{Severity::Info};
  std::string path;
  std::size_t offset{0};
  std::string message;
};

class Diagnostics
{
public:
  void info(std::string path, std::size_t offset, std::string message);
  void warning(std::string path, std::size_t offset, std::string message);
  void error(std::string path, std::size_t offset, std::string message);

  [[nodiscard]] bool has_errors() const noexcept;
  [[nodiscard]] const std::vector<Diagnostic> &entries() const noexcept;

private:
  std::vector<Diagnostic> entries_;
};

} // namespace binmorph
