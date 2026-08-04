#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace binmorph
{

using Byte = std::uint8_t;
using Bytes = std::vector<Byte>;

enum class Endian
{
  Little,
  Big,
  Native
};

enum class NodeKind
{
  Container,
  Field,
  Choice,
  Repeat
};

std::string_view to_string(Endian endian) noexcept;
std::string_view to_string(NodeKind kind) noexcept;
std::optional<Endian> parse_endian(std::string_view value) noexcept;

} // namespace binmorph
