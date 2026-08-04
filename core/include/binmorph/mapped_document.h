#pragma once

#include "codec.h"
#include "types.h"

#include <optional>
#include <span>
#include <string>
#include <vector>

namespace binmorph
{

struct FieldValue
{
  std::string path;
  std::size_t offset{0};
  std::size_t length{0};
  Endian endian{Endian::Native};
  Bytes raw_bytes;
  DecodedValue decoded;
};

class MappedDocument
{
public:
  void add(FieldValue value);

  [[nodiscard]] const std::vector<FieldValue> &values() const noexcept;
  [[nodiscard]] const FieldValue *find(std::string_view path) const noexcept;

private:
  std::vector<FieldValue> values_;
};

} // namespace binmorph
