#include <binmorph/diagnostics.h>

#include <algorithm>

namespace binmorph
{

void Diagnostics::info(std::string path, std::size_t offset, std::string message)
{
  entries_.push_back({Severity::Info, std::move(path), offset, std::move(message)});
}

void Diagnostics::warning(std::string path, std::size_t offset, std::string message)
{
  entries_.push_back({Severity::Warning, std::move(path), offset, std::move(message)});
}

void Diagnostics::error(std::string path, std::size_t offset, std::string message)
{
  entries_.push_back({Severity::Error, std::move(path), offset, std::move(message)});
}

bool Diagnostics::has_errors() const noexcept
{
  return std::ranges::any_of(entries_, [](const Diagnostic &diagnostic)
                             { return diagnostic.severity == Severity::Error; });
}

const std::vector<Diagnostic> &Diagnostics::entries() const noexcept
{
  return entries_;
}

} // namespace binmorph
