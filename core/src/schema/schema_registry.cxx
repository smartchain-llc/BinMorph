#include <binmorph/schema_registry.h>

namespace binmorph
{

void SchemaRegistry::register_model(FormatModel model)
{
  models_.insert_or_assign(model.schema_id(), std::move(model));
}

const FormatModel *SchemaRegistry::resolve(const std::string &schema_id) const noexcept
{
  const auto found = models_.find(schema_id);
  if (found == models_.end())
    return nullptr;
  return &found->second;
}

std::vector<std::string> SchemaRegistry::schema_ids() const
{
  std::vector<std::string> ids;
  ids.reserve(models_.size());
  for (const auto &[id, _] : models_)
    ids.push_back(id);
  return ids;
}

} // namespace binmorph
