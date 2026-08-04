#include <binmorph/schema_parser.h>

#include <exception>

namespace binmorph
{
namespace
{

template <typename Json>
LayoutNode parse_node(std::string name,
                      std::string path,
                      const Json &json,
                      Diagnostics &diagnostics)
{
  LayoutNode node;
  node.name = std::move(name);
  node.path = std::move(path);

  if (json.is_array())
  {
    node.kind = NodeKind::Repeat;
    for (std::size_t i = 0; i < json.size(); ++i)
    {
      const auto child_name = node.name + "[" + std::to_string(i) + "]";
      const auto child_path = node.path + "[" + std::to_string(i) + "]";
      node.children.push_back(parse_node(child_name, child_path, json.at(i), diagnostics));
    }
    return node;
  }

  if (!json.is_object())
  {
    diagnostics.error(node.path, 0, "layout node must be an object or array");
    return node;
  }

  if (json.contains("len"))
  {
    node.kind = NodeKind::Field;
    if (json["len"].is_number_unsigned())
      node.length = json["len"].template get<std::size_t>();
    else
      diagnostics.error(node.path, 0, "field len must be an unsigned integer");

    if (json.contains("endian"))
    {
      if (!json["endian"].is_string())
      {
        diagnostics.error(node.path, 0, "field endian must be a string");
      }
      else if (auto endian = parse_endian(json["endian"].template get<std::string>()))
      {
        node.endian = *endian;
      }
      else
      {
        diagnostics.error(node.path, 0, "unknown endian value");
      }
    }

    return node;
  }

  node.kind = NodeKind::Container;
  for (const auto &[key, value] : json.items())
  {
    const auto child_path = node.path.empty() ? key : node.path + "." + key;
    node.children.push_back(parse_node(key, child_path, value, diagnostics));
  }
  return node;
}

template <typename Json>
FormatModel parse_model(const Json &json, std::string schema_id, Diagnostics &diagnostics)
{
  FormatModel model{std::move(schema_id)};
  if (!json.is_object())
  {
    diagnostics.error(model.schema_id(), 0, "schema root must be a JSON object");
    return model;
  }

  if (json.contains("$id") && json["$id"].is_string())
    model.set_schema_id(json["$id"].template get<std::string>());
  if (json.contains("version") && json["version"].is_string())
    model.set_version(json["version"].template get<std::string>());

  for (const auto &[key, value] : json.items())
  {
    if (!key.empty() && key[0] == '$')
      continue;
    if (key == "version")
      continue;
    model.add_root(parse_node(key, key, value, diagnostics));
  }

  return model;
}

} // namespace

FormatModel SchemaParser::parse(const SchemaSource &source, Diagnostics &diagnostics) const
{
  try
  {
    return parse_model(nlohmann::ordered_json::parse(source.text()), source.uri(), diagnostics);
  }
  catch (const std::exception &ex)
  {
    diagnostics.error(source.uri(), 0, ex.what());
    return FormatModel{source.uri()};
  }
}

FormatModel SchemaParser::parse_json(const nlohmann::json &json,
                                     std::string schema_id,
                                     Diagnostics &diagnostics) const
{
  return parse_model(json, std::move(schema_id), diagnostics);
}

} // namespace binmorph
