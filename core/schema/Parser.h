#pragma once
#include <schema/json/FieldAttribute.h> // remove when Schema works
#include <schema/Schema.h>
#include <set>

namespace bm{
namespace traits{
    template<typename T> concept IsJSONType = std::convertible_to<T, nlohmann::json>;
    template<typename T> concept InputJSONProvider = IsJSONType<T> || requires(T t){
        { t.getJSON() } -> std::convertible_to<nlohmann::json>;
    };

}


class Parser{
public:
    static void ParseTo(Schema& schema, const nlohmann::json& json);
    static Schema Parse(const nlohmann::json& json);
};
Schema create_schema(const nlohmann::json& inputJSON);
template<traits::InputJSONProvider T> 
Schema create_schema_t(T input){
    bm::Schema ret;
    if constexpr (traits::IsJSONType<T>) // SchemaFile already contains Schema member, no need to Parse
        bm::Parser::ParseTo(ret, input);
    else
        bm::Parser::ParseTo(ret, input.getJSON());
    return std::move(ret);
}
/*
    Undefined Reference Issue:
    template function create_schema_t is defined in Parser.cxx, but it is declared in the header Parser.h. 
    Since templates are instantiated at compile time, the compiler needs to see the full definition of the template 
    function wherever it is used.

    FIX:
        1. Move Definition from .cxx to .h
        2. Explicit Template Instantiation in .cxx
            - `template bm::Schema create_schema_t<nlohmann::json>(nlohmann::json input)`
            This approach works only if you know all the types for which the template will be instantiated.
*/
}