#pragma once

#include "diagnostics.h"
#include "format_model.h"

#include <set>

namespace binmorph
{

class SchemaValidator
{
public:
  [[nodiscard]] bool validate(const FormatModel &model, Diagnostics &diagnostics) const;

private:
  void validate_node(const LayoutNode &node,
                     Diagnostics &diagnostics,
                     std::set<std::string> &paths) const;
};

} // namespace binmorph
