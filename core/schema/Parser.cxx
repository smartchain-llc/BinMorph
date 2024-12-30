#include <schema/Parser.h>
#include <iostream>
using namespace bm;

void Parser::ParseTo(Schema& schema, const nlohmann::json& json){
    for(const auto&[key, value] : json.items()){
        schema << LayoutAttribute{key, value};
    }
}
Schema Parser::Parse(const nlohmann::json& json){
    Schema ret;
    Parser::ParseTo(ret, json);
    return ret;
}
Schema bm::create_schema(const nlohmann::json& inputJSON){
    Schema ret;
    Parser::ParseTo(ret, inputJSON);
    return std::move(ret);
}