#pragma once

#include "schema/SchemaValidator.h"
#include "io/output/Mapper.h"
namespace bm{

template<std::convertible_to<std::string> J, traits::is_schema_validator V = DefaultSchemaValidator> 
Schema create_schema(J json){
    if( std::is_same_v<J, std::string> ){
        return V::validate(nlohmann::json::parse( json ));
    }
    return V::validate(nlohmann::json::parse( std::string(json) ));
}


}