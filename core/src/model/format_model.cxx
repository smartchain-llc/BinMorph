#include <binmorph/format_model.h>

namespace binmorph
{

std::string_view to_string(Endian endian) noexcept
{
  switch (endian)
  {
  case Endian::Little:
    return "little";
  case Endian::Big:
    return "big";
  case Endian::Native:
    return "native";
  }
  return "native";
}

std::string_view to_string(NodeKind kind) noexcept
{
  switch (kind)
  {
  case NodeKind::Container:
    return "container";
  case NodeKind::Field:
    return "field";
  case NodeKind::Choice:
    return "choice";
  case NodeKind::Repeat:
    return "repeat";
  }
  return "container";
}

std::optional<Endian> parse_endian(std::string_view value) noexcept
{
  if (value == "little" || value == "le")
    return Endian::Little;
  if (value == "big" || value == "be")
    return Endian::Big;
  if (value == "native")
    return Endian::Native;
  return std::nullopt;
}

bool LayoutNode::is_field() const noexcept
{
  return kind == NodeKind::Field;
}

FormatModel::FormatModel(std::string schema_id) : schema_id_{std::move(schema_id)}
{
}

const std::string &FormatModel::schema_id() const noexcept
{
  return schema_id_;
}

void FormatModel::set_schema_id(std::string schema_id)
{
  schema_id_ = std::move(schema_id);
}

const std::string &FormatModel::version() const noexcept
{
  return version_;
}

void FormatModel::set_version(std::string version)
{
  version_ = std::move(version);
}

void FormatModel::add_root(LayoutNode node)
{
  roots_.push_back(std::move(node));
}

const std::vector<LayoutNode> &FormatModel::roots() const noexcept
{
  return roots_;
}

bool FormatModel::empty() const noexcept
{
  return roots_.empty();
}

} // namespace binmorph
