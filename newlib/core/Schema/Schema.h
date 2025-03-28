#pragma once
#include <set>
#include "Attributes/FieldAttribute.h"

bool areOverlapping(const LayoutAttribute &, const LayoutAttribute &);


class Schema
{
public:
    inline auto begin() const { return layouts.begin(); }
    inline auto end() const { return layouts.end(); }

    void operator<<(const LayoutAttribute &);
    const LayoutAttribute &get(const std::string &id) const;
    const std::size_t calculatedSize() const noexcept;

private:
    std::set<LayoutAttribute, LayoutOverlapComparator> layouts = {};
};

namespace traits
{
    
    template<typename S>
    concept is_schema_type = std::same_as<Schema, S> || std::convertible_to<S, Schema>;
}

Schema&& from_literal(const char*);