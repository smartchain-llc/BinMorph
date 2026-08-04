#pragma once

#include "format_model.h"

#include <vector>

namespace binmorph
{

struct PlanStep
{
  std::string path;
  std::size_t length{0};
  Endian endian{Endian::Native};
};

class ExecutionPlan
{
public:
  static ExecutionPlan compile(const FormatModel &model);

  [[nodiscard]] const std::vector<PlanStep> &steps() const noexcept;
  [[nodiscard]] std::size_t required_size() const noexcept;

private:
  void add_node(const LayoutNode &node);

  std::vector<PlanStep> steps_;
  std::size_t required_size_{0};
};

} // namespace binmorph
