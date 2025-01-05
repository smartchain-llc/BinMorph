#include <schema/Parser.h>
#include <iostream>

void bm::Parser::ParseTo(bm::Schema& schema, const nlohmann::json& json){
    for(const auto&[key, value] : json.items()){
        schema << bm::LayoutAttribute{key, value};
    }
}
bm::Schema bm::Parser::Parse(const nlohmann::json& json){ // Create InputJSONProvider trait
    bm::Schema ret;
    bm::Parser::ParseTo(ret, json);
    return ret;
}
bm::Schema bm::create_schema(const nlohmann::json& inputJSON){
    bm::Schema ret;
    bm::Parser::ParseTo(ret, inputJSON);
    return std::move(ret);
}
bm::Schema bm::create_schema(const char* json){
    const auto _json = nlohmann::json::parse(json);
    auto schema = bm::Parser::Parse(_json);
    return schema;
}