#include <binmorph/mapped_document.h>

namespace binmorph
{

void MappedDocument::add(FieldValue value)
{
  values_.push_back(std::move(value));
}

const std::vector<FieldValue> &MappedDocument::values() const noexcept
{
  return values_;
}

const FieldValue *MappedDocument::find(std::string_view path) const noexcept
{
  for (const auto &value : values_)
  {
    if (value.path == path)
      return &value;
  }
  return nullptr;
}

} // namespace binmorph
