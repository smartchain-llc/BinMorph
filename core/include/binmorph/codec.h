#pragma once

#include "types.h"

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>

namespace binmorph
{

struct DecodedValue
{
  std::optional<std::uint64_t> unsigned_integer;
  std::string display;
};

using Codec = std::function<DecodedValue(std::span<const Byte>, Endian)>;

class CodecRegistry
{
public:
  CodecRegistry();

  void register_codec(std::string name, Codec codec);
  [[nodiscard]] DecodedValue decode(std::string_view name,
                                    std::span<const Byte> bytes,
                                    Endian endian) const;

private:
  std::map<std::string, Codec, std::less<>> codecs_;
};

DecodedValue decode_unsigned_integer(std::span<const Byte> bytes, Endian endian);

} // namespace binmorph
