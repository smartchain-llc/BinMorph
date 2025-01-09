#pragma once
#include <set>
#include <types/traits.h>
#include <schema/FieldAttribute.h>

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
        const std::size_t calculatedSize() const noexcept
        {
            std::size_t ret{0};
            for (const auto &layout : layouts)
                ret += layout.byteLength();
            return ret;
        }

    private:
        std::set<LayoutAttribute, LayoutOverlapComparator> layouts = {};
    };

    namespace traits
    {
        /**
         * @brief Concept to satifies for types that contain, or are implicitly, `Schema`s.
         * @details Should contain a `schema()` member function returning the generated `Schema`
         */
        template <typename T>
        concept SchemaProvider = requires {
            requires(
                std::derived_from<Schema, T> || std::convertible_to<Schema, T> ||
                requires(T t) {{ t.schema() } noexcept -> std::same_as<Schema>; });
        };
        template <typename T, typename SP, typename DP>
        concept SchemaMappable = requires {
            requires(SchemaProvider<SP> && DataProvider<DP>);
            requires(
                requires(T t, SP &sp, DP &dp) { {t.map(sp, dp)} -> std::same_as<void>; });
        };
    }
}