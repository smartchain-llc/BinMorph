#pragma once

#include "schema/SchemaValidator.h"
#include "io/output/Mapper.h"
#include "io/input/InputFile.h"
#include <fstream>
namespace bm{

template<traits::is_schema_validator V = DefaultSchemaValidator, typename J> 
Schema create_schema(J json){
    if( std::is_same_v<J, std::string> ){
        return V::validate(nlohmann::json::parse( json ));
    }
    return V::validate(nlohmann::json::parse( std::string(json) ));
}
template<const char*, traits::is_schema_validator V = DefaultSchemaValidator>
Schema create_schema(const char* json){
 return V::validate(nlohmann::json::parse( std::string(json) ));
}

Schema schema_from_file(const InputFile& sFile){
    std::ifstream _in { sFile.path() };
    
    char buffer[2048];
    _in.read(buffer, 2048);
    return create_schema(buffer);
}

}

