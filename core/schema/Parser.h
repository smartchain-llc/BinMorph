#pragma once
#include <schema/json/FieldAttribute.h>
#include <set>

namespace bm{

struct Schema{};
class Parser{
public:
    static std::vector<LayoutAttribute> Parse(nlohmann::json& json);
    static std::set<LayoutAttribute, LayoutOverlapComparator> MapLayouts(nlohmann::json& json);
    static std::vector<LayoutAttribute> Parse(nlohmann::json& json, Schema&);
    static std::unordered_map<std::string, LayoutAttribute> MapLayouts(nlohmann::json& json, Schema&);
};

}