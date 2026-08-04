#pragma once

#include "types.h"

#include <optional>
#include <string>
#include <vector>

namespace binmorph
{

struct LayoutNode
{
  std::string name;
  std::string path;
  NodeKind kind{NodeKind::Container};
  std::optional<std::size_t> length;
  Endian endian{Endian::Native};
  std::vector<LayoutNode> children;

  [[nodiscard]] bool is_field() const noexcept;
};

class FormatModel
{
public:
  FormatModel() = default;
  explicit FormatModel(std::string schema_id);

  [[nodiscard]] const std::string &schema_id() const noexcept;
  void set_schema_id(std::string schema_id);

  [[nodiscard]] const std::string &version() const noexcept;
  void set_version(std::string version);

  void add_root(LayoutNode node);
  [[nodiscard]] const std::vector<LayoutNode> &roots() const noexcept;
  [[nodiscard]] bool empty() const noexcept;

private:
  std::string schema_id_{"anonymous"};
  std::string version_{"0"};
  std::vector<LayoutNode> roots_;
};

} // namespace binmorph
