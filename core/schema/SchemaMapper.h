#pragma once
#include <schema/Schema.h>
#include <concepts>
#include <type_traits>
namespace bm
{
    template <typename T>
    concept BytePointer = std::same_as<T, char *> || std::same_as<uint8_t *, T>;
    template <typename T>
    concept DataProvider = requires(T t) {
        { t.length() } noexcept -> std::same_as<std::size_t>;
        requires(requires(char *src, std::size_t len) { t.readTo(src, len); }) ||
                    (requires(uint8_t *src, std::size_t len) { t.readTo(src, len); });
    };
    template <typename T>
    concept SchemaProvider = requires {
        requires(
            std::derived_from<Schema, T> || std::convertible_to<Schema, T> ||
            requires(T t){{ t.schema() } noexcept -> std::same_as<Schema>;}
        );
    };
    template <typename T, typename SP, typename DP>
    concept SchemaMappable = requires {
        requires(SchemaProvider<SP> && DataProvider<DP>);
        requires(
            requires(T t, SP &sp, DP &dp) { {t.map(sp, dp)} -> std::same_as<void>; });
    };

    template <typename MapImpl>
    class SchemaMapper
    {
    public:
        SchemaMapper() {}
        SchemaMapper(MapImpl &&mapper) : _m_mapper{mapper} {}
        template <typename S, typename D> requires SchemaProvider<S> && DataProvider<D>
        void map(const S &schema, D& dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }
        template <typename S, typename D> requires SchemaProvider<S> && DataProvider<D>
        void map(const S &schema, D&& dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }

        MapImpl::ResultsType results() const noexcept { return _m_mapper.results(); }

    private:
        MapImpl _m_mapper;
    };
}