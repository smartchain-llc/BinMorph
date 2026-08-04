#pragma once

#include "format_model.h"

#include <map>
#include <optional>
#include <string>

namespace binmorph
{

class SchemaRegistry
{
public:
  void register_model(FormatModel model);

  [[nodiscard]] const FormatModel *resolve(const std::string &schema_id) const noexcept;
  [[nodiscard]] std::vector<std::string> schema_ids() const;

private:
  std::map<std::string, FormatModel> models_;
};

} // namespace binmorph
