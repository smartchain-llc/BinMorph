#include <binmorph/schema_validator.h>

namespace binmorph
{

bool SchemaValidator::validate(const FormatModel &model, Diagnostics &diagnostics) const
{
  if (model.empty())
    diagnostics.error(model.schema_id(), 0, "schema has no root layout nodes");

  std::set<std::string> paths;
  for (const auto &root : model.roots())
    validate_node(root, diagnostics, paths);

  return !diagnostics.has_errors();
}

void SchemaValidator::validate_node(const LayoutNode &node,
                                    Diagnostics &diagnostics,
                                    std::set<std::string> &paths) const
{
  if (node.path.empty())
    diagnostics.error(node.name, 0, "layout node has empty path");

  if (!paths.insert(node.path).second)
    diagnostics.error(node.path, 0, "duplicate layout path");

  if (node.is_field())
  {
    if (!node.length)
      diagnostics.error(node.path, 0, "field is missing len");
    else if (*node.length == 0)
      diagnostics.error(node.path, 0, "field len must be greater than zero");

    if (!node.children.empty())
      diagnostics.error(node.path, 0, "field nodes cannot have children");
    return;
  }

  if (node.children.empty())
    diagnostics.warning(node.path, 0, "container node has no children");

  for (const auto &child : node.children)
    validate_node(child, diagnostics, paths);
}

} // namespace binmorph
