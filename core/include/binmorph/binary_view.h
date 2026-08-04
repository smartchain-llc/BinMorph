#pragma once

#include "types.h"

#include <filesystem>
#include <span>
#include <string>

namespace binmorph
{

class BinaryView
{
public:
  static BinaryView from_bytes(Bytes bytes, std::string source_name = "memory");
  static BinaryView from_file(const std::filesystem::path &path);

  [[nodiscard]] std::span<const Byte> bytes() const noexcept;
  [[nodiscard]] std::span<const Byte> slice(std::size_t offset, std::size_t length) const;
  [[nodiscard]] std::size_t size() const noexcept;
  [[nodiscard]] const std::string &source_name() const noexcept;

private:
  BinaryView(Bytes bytes, std::string source_name);

  Bytes bytes_;
  std::string source_name_;
};

class BinaryCursor
{
public:
  explicit BinaryCursor(const BinaryView &view);

  [[nodiscard]] std::size_t offset() const noexcept;
  [[nodiscard]] std::size_t remaining() const noexcept;
  [[nodiscard]] std::span<const Byte> read(std::size_t length);
  void seek(std::size_t offset);

private:
  const BinaryView *view_;
  std::size_t offset_{0};
};

} // namespace binmorph
