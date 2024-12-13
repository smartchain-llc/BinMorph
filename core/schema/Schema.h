#pragma once
#include <set>

#include <schema/json/FieldAttribute.h>

namespace bm
{

    bool areOverlapping(const LayoutAttribute &, const LayoutAttribute &);

    class Schema
    {
    public:
        inline auto begin() const { return layouts.begin(); }
        inline auto end() const { return layouts.end(); }

        void operator<<(const LayoutAttribute &);
        const LayoutAttribute &get(const std::string &id) const;
        const std::size_t calculatedSize() const noexcept{
            std::size_t ret { 0 };
            for(const auto& layout : layouts)
                ret += layout.byteLength();
            return ret;
        }
    private:
        std::set<LayoutAttribute, LayoutOverlapComparator> layouts;
    };

}