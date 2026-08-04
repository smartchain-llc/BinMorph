#include <binmorph/binary_view.h>

#include <fstream>
#include <iterator>
#include <stdexcept>

namespace binmorph
{

BinaryView BinaryView::from_bytes(Bytes bytes, std::string source_name)
{
  return BinaryView(std::move(bytes), std::move(source_name));
}

BinaryView BinaryView::from_file(const std::filesystem::path &path)
{
  std::ifstream input{path, std::ios::binary};
  if (!input)
    throw std::runtime_error("failed to open binary file: " + path.string());

  Bytes bytes{std::istreambuf_iterator<char>{input}, std::istreambuf_iterator<char>{}};
  return BinaryView(std::move(bytes), path.string());
}

std::span<const Byte> BinaryView::bytes() const noexcept
{
  return bytes_;
}

std::span<const Byte> BinaryView::slice(std::size_t offset, std::size_t length) const
{
  if (offset > bytes_.size() || length > bytes_.size() - offset)
    throw std::out_of_range("binary slice exceeds input length");
  return std::span<const Byte>{bytes_.data() + offset, length};
}

std::size_t BinaryView::size() const noexcept
{
  return bytes_.size();
}

const std::string &BinaryView::source_name() const noexcept
{
  return source_name_;
}

BinaryView::BinaryView(Bytes bytes, std::string source_name)
    : bytes_{std::move(bytes)}, source_name_{std::move(source_name)}
{
}

BinaryCursor::BinaryCursor(const BinaryView &view) : view_{&view}
{
}

std::size_t BinaryCursor::offset() const noexcept
{
  return offset_;
}

std::size_t BinaryCursor::remaining() const noexcept
{
  return view_->size() - offset_;
}

std::span<const Byte> BinaryCursor::read(std::size_t length)
{
  const auto start = offset_;
  auto result = view_->slice(start, length);
  offset_ += length;
  return result;
}

void BinaryCursor::seek(std::size_t offset)
{
  if (offset > view_->size())
    throw std::out_of_range("binary cursor seek exceeds input length");
  offset_ = offset;
}

} // namespace binmorph
