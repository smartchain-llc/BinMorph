#include "FieldAttribute.h"
namespace bm{
void from_json(const nlohmann::json &json, FieldAttribute &att)
{

    json["name"].get_to(att.name);
    json["length"].get_to(att.length);
    json["endian"].get_to(att.endian);
}
}