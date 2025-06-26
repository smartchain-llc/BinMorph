#include "FieldAttributes.h"
#include <iostream>

FieldAttribute::FieldAttribute(const nlohmann::json& json){
    json["name"].get_to(name);
    json["length"].get_to(length);
    json["endian"].get_to(endian);
}

void from_json(const nlohmann::json &json, FieldAttribute &att)
{
    json["name"].get_to(att.name);
    json["length"].get_to(att.length);
    json["endian"].get_to(att.endian);
}
bool containsRequired(const nlohmann::json &json)
{
    if (!json.contains("name"))
    {
        return false;
    }
    if (!json.contains("length"))
    {
        return false;
    }
    if (!json.contains("endian"))
    {
        return false;
    }
    return true;
}

std::vector<FieldAttribute> field_attributes(const nlohmann::json& json){
    std::vector<FieldAttribute> ret;

    for(const auto& field : json){
        ret.emplace_back(field);
    }
    return ret;
}

std::ostream& operator<<(std::ostream& ostream, const FieldAttribute& field){
    
}