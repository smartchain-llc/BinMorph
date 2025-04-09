#pragma once

#include "schema/SchemaValidator.h"
#include "io/output/Mapper.h"
#include "io/input/InputFile.h"
#include <fstream>
namespace bm{

template<traits::is_schema_validator V = DefaultSchemaValidator, typename J> 
Schema create_schema(J json){
    if( std::convertible_to<J, std::string> ){
        return V::validate(nlohmann::json::parse( json ));
    }
    // return V::validate(nlohmann::json::parse( sJson ));
}
template<const char*, traits::is_schema_validator V = DefaultSchemaValidator>
Schema create_schema(const char* json){
 return V::validate(nlohmann::json::parse( std::string(json) ));
}
template<traits::is_schema_validator V = DefaultSchemaValidator>
Schema schema_from_file(const file::File& sFile){
    FILE* f = fopen(sFile.path().c_str(), "r");
    return V::validate(nlohmann::json::parse(f));
}

}

