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
// template<typename T> requires bm::traits::InputJSONProvider<T>
// bm::Schema create_schema_t(T input){
//     bm::Schema ret;
//     bm::Parser::ParseTo(ret, input.getJSON());
//     return std::move(ret);
// }