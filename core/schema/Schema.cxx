#include "Schema.h"
#include <sstream>
#include <algorithm>

using namespace bm;

bool bm::areOverlapping(const LayoutAttribute &l1, const LayoutAttribute &l2)
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
    layouts.insert(layout);
}

const LayoutAttribute& Schema::get(const std::string& id) const {
    auto found = std::find_if(layouts.begin(), layouts.end(), [&](const LayoutAttribute& attr){
        return attr.id == id;
    });
    return *found;
}