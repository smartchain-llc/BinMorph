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

    private:
        std::set<LayoutAttribute, LayoutOverlapComparator> layouts;
    };

}