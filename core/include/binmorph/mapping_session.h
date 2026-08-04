#pragma once

#include "binary_view.h"
#include "codec.h"
#include "diagnostics.h"
#include "execution_plan.h"
#include "format_model.h"
#include "mapped_document.h"

namespace binmorph
{

struct MappingOptions
{
  bool stop_on_error{true};
};

struct MappingResult
{
  MappedDocument document;
  Diagnostics diagnostics;
};

class MappingSession
{
public:
  explicit MappingSession(FormatModel model);

  void set_options(MappingOptions options) noexcept;
  [[nodiscard]] const FormatModel &model() const noexcept;
  [[nodiscard]] const ExecutionPlan &plan() const noexcept;
  [[nodiscard]] CodecRegistry &codecs() noexcept;

  [[nodiscard]] MappingResult parse(const BinaryView &input) const;
  [[nodiscard]] Bytes build(const MappedDocument &document, Diagnostics &diagnostics) const;

private:
  FormatModel model_;
  ExecutionPlan plan_;
  MappingOptions options_;
  CodecRegistry codecs_;
};

} // namespace binmorph
