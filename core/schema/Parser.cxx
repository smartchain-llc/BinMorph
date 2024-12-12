#include <schema/Parser.h>
#include <iostream>
using namespace bm;

void Parser::ParseTo(Schema& schema, const nlohmann::json& json){
    for(const auto&[key, value] : json.items()){
        schema << LayoutAttribute{key, value};
    }
}