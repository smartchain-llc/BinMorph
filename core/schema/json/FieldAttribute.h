#pragma once
#include <json.hpp>


namespace bm{
    struct FieldAttribute
    {
        FieldAttribute(const nlohmann::json &json){
            json["name"].get_to(name);
            json["length"].get_to(length);
            json["endian"].get_to(endian);
        }
        std::size_t length;
        std::string name;
        std::string endian;
    };
    
    // why multiple def here?
    // void from_json(const nlohmann::json& json, FieldAttribute& att){

    // }

}