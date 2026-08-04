#pragma once

#include "diagnostics.h"
#include "format_model.h"
#include "schema_source.h"

#include <json.hpp>

namespace binmorph
{

class SchemaParser
{
public:
  [[nodiscard]] FormatModel parse(const SchemaSource &source, Diagnostics &diagnostics) const;
  [[nodiscard]] FormatModel parse_json(const nlohmann::json &json,
                                       std::string schema_id,
                                       Diagnostics &diagnostics) const;
};

} // namespace binmorph
