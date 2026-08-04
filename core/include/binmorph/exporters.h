#pragma once

#include "diagnostics.h"
#include "mapped_document.h"

#include <json.hpp>
#include <ostream>

namespace binmorph
{

class ExporterSuite
{
public:
  [[nodiscard]] nlohmann::json to_json(const MappedDocument &document) const;
  void write_summary(const MappedDocument &document,
                     const Diagnostics &diagnostics,
                     std::ostream &out) const;
};

} // namespace binmorph
