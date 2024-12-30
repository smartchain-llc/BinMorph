#pragma once
#include <schema/json/FieldAttribute.h> // remove when Schema works
#include <schema/Schema.h>
#include <set>

namespace bm{

class Parser{
public:
    static void ParseTo(Schema& schema, const nlohmann::json& json);
    static Schema Parse(const nlohmann::json& json);
};
Schema create_schema(const nlohmann::json& inputJSON);
}