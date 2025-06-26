#pragma once
#include <json.hpp>
#include <vector>

///@brief Contains values from a field item
struct FieldAttribute
{
    std::size_t length;
    std::string name;
    std::string endian;
    
    ///@brief Extracts field item values from an array index of fields
    ///@param json JSON object from a "fields" array index.
    FieldAttribute(const nlohmann::json &json);

};
void from_json(const nlohmann::json &json, FieldAttribute &att);
bool containsRequired(const nlohmann::json &json);
std::vector<FieldAttribute> field_attributes(const nlohmann::json& json);

std::ostream& operator<<(std::ostream&, const FieldAttribute&);