#pragma once
#include <json.hpp>


namespace bm::json{

    struct FieldAttribute
    {
        std::size_t length;
        std::string name;
        std::string endian;
    };

    void from_json(const nlohmann::json &json, FieldAttribute &field)
    {
        json["name"].get_to(field.name);
        json["length"].get_to(field.length);
        json["endian"].get_to(field.endian);
    }
}