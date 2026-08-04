#include <binmorph/execution_plan.h>

namespace binmorph
{

ExecutionPlan ExecutionPlan::compile(const FormatModel &model)
{
  ExecutionPlan plan;
  for (const auto &root : model.roots())
    plan.add_node(root);
  return plan;
}

const std::vector<PlanStep> &ExecutionPlan::steps() const noexcept
{
  return steps_;
}

std::size_t ExecutionPlan::required_size() const noexcept
{
  return required_size_;
}

void ExecutionPlan::add_node(const LayoutNode &node)
{
  if (node.is_field() && node.length)
  {
    steps_.push_back({node.path, *node.length, node.endian});
    required_size_ += *node.length;
    return;
  }

  for (const auto &child : node.children)
    add_node(child);
}

} // namespace binmorph
