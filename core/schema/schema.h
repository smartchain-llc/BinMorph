#pragma once
#include <filesystem>
#include <fstream>
#include <schema/json/FieldAttribute.h>

namespace bm{

class Schema{
public:
    Schema(const nlohmann::json& json){
        std::vector<nlohmann::json> fields;
        for(const auto&[key, value] : json.items()){
            value.template get_to(fields);
        }
        for(auto& fieldAtt : fields)
            _m_fields.emplace_back(std::move(FieldAttribute{fieldAtt}));
    }

public:
    std::vector<FieldAttribute> __fields() const { return _m_fields; }
private:
    std::vector<FieldAttribute> _m_fields;
};

// void from_json(const nlohmann::json& json, Schema& schema){
//     const auto fields = json.get<std::vector<nlohmann::json>>();
    // for(const auto&[key, value] : json.items()){
    //     schema << value;
    // }
// }

}