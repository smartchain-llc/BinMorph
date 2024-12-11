#include <schema/Parser.h>

#include <iostream>
using namespace bm;


std::vector<LayoutAttribute> Parser::Parse(nlohmann::json& json){
    std::vector<LayoutAttribute> ret;
    for(const auto& [key, value] : json.items()){
        ret.push_back({key, value});
    }
    return ret;
}
std::set<LayoutAttribute, LayoutOverlapComparator> Parser::MapLayouts(nlohmann::json& json){
    std::set<LayoutAttribute, LayoutOverlapComparator> ret;
    std::size_t lastEndOffset { 0 };

    for(const auto&[key, value] : json.items()){
        LayoutAttribute attribute{ value };
        if(attribute.offset < lastEndOffset)
            std::cerr << "Attribute: " << key << " overlaps previous attribute\n";
        lastEndOffset = attribute.endOffset();

        ret.insert(attribute);
    }
    return ret;
}