#pragma once

#include "schema/SchemaValidator.h"
namespace bm{


// Schema create_schema(const char* json){
//     const auto _json = nlohmann::json::parse(json);
//     Schema schema;
//     for(const auto&[key, value] : _json.items()){
//         schema << bm::LayoutAttribute{key, value};
//     }
//     return schema;
// }

template<std::convertible_to<std::string> J, traits::is_schema_validator V = DefaultSchemaValidator> 
Schema create_schema(J json){
    if( std::is_same_v<J, std::string> ){
        return V::validate(nlohmann::json::parse( json ));
    }
    return V::validate(nlohmann::json::parse( std::string(json) ));
}

}