#include <binmorph/codec.h>

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace binmorph
{

CodecRegistry::CodecRegistry()
{
  register_codec("uint", decode_unsigned_integer);
}

void CodecRegistry::register_codec(std::string name, Codec codec)
{
  codecs_.insert_or_assign(std::move(name), std::move(codec));
}

DecodedValue CodecRegistry::decode(std::string_view name,
                                   std::span<const Byte> bytes,
                                   Endian endian) const
{
  const auto found = codecs_.find(name);
  if (found == codecs_.end())
    throw std::runtime_error("unknown codec: " + std::string{name});
  return found->second(bytes, endian);
}

DecodedValue decode_unsigned_integer(std::span<const Byte> bytes, Endian endian)
{
  DecodedValue value;
  if (bytes.size() <= sizeof(std::uint64_t))
  {
    std::uint64_t decoded{0};
    if (endian == Endian::Big)
    {
      for (const auto byte : bytes)
        decoded = (decoded << 8U) | byte;
    }
    else
    {
      for (std::size_t i = 0; i < bytes.size(); ++i)
        decoded |= static_cast<std::uint64_t>(bytes[i]) << (i * 8U);
    }
    value.unsigned_integer = decoded;
    value.display = std::to_string(decoded);
    return value;
  }

  std::ostringstream display;
  display << "0x";
  for (const auto byte : bytes)
    display << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
  value.display = display.str();
  return value;
}

} // namespace binmorph
