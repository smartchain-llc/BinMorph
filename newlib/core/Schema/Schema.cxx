#include "Schema.h"
#include <sstream>
#include <algorithm>


bool areOverlapping(const LayoutAttribute &l1, const LayoutAttribute &l2)
{
    const auto &[first, second] = l1.startOffset() < l2.startOffset() ? std::tuple<LayoutAttribute, LayoutAttribute>(l1, l2) : std::tuple<LayoutAttribute, LayoutAttribute>(l2, l1);
    return second.startOffset() < first.endOffset() ? true : false;
}

void Schema::operator<<(const LayoutAttribute &layout)
{
    for (const auto &validLayout : layouts)
    {
        if (areOverlapping(validLayout, layout))
        {
            std::ostringstream os;
            os << "Layout (" << layout.id << ") offset [" << layout.offset << "] overlaps Layout: (" << validLayout.id << ") offset [" << validLayout.offset << " - " << validLayout.endOffset() << "]\n";

            // std::cerr << os.str();
            throw os.str();
        }
    }
    layouts.insert(std::move(layout));
}

const LayoutAttribute& Schema::get(const std::string& id) const {
    auto found = std::find_if(layouts.begin(), layouts.end(), [&](const LayoutAttribute& attr){
        return attr.id == id;
    });
    return *found;
}

const std::size_t Schema::calculatedSize() const noexcept
{
    std::size_t ret{0};
    for (const auto &layout : layouts)
        ret += layout.byteLength();
    return ret;
}

Schema&& from_literal(const char* json){
    Schema ret;

    auto parsedJSON = nlohmann::json::parse(json);

    for(auto& [key, value] : parsedJSON.items()){
        ret << LayoutAttribute{value};
    }

    return std::move(ret);
}