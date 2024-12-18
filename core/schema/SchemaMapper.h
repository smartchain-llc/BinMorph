#pragma once
#include <schema/Schema.h>
#include <concepts>
#include <type_traits>
namespace bm{
template<typename T> concept BytePointer = std::same_as<T, char*> || std::same_as<uint8_t*, T>;
template<typename T> concept DataProvider = requires(T t) {
    { t.length() } noexcept -> std::same_as<std::size_t>;
    requires (requires(char* src, std::size_t len) { t.readTo(src, len); }) ||
             (requires(uint8_t* src, std::size_t len) { t.readTo(src, len); });
};
template<typename T> concept SchemaProvider = requires(T t){
    { t.schema() } noexcept -> std::same_as<Schema>;
};
template<typename MapImpl> class SchemaMapper{
public:
    SchemaMapper(){}
    SchemaMapper(MapImpl&& mapper):_m_mapper{mapper}{}
    template<bm::DataProvider D>
    void map(const Schema &schema, D& dataProvider) {
        _m_mapper.map(schema, dataProvider);
    }

    template<bm::DataProvider D>
    void _map(const Schema& schema, D& dataProvider){
        _m_mapper.map(schema, dataProvider);
    }

    MapImpl::ResultsType results() const noexcept { return _m_mapper.results(); }
private:
    MapImpl _m_mapper;
};

template<typename MapImpl> class _SchemaMapper{
public:
    _SchemaMapper(){}
    _SchemaMapper(MapImpl&& mapper):_m_mapper{mapper}{}
    template<DataProvider D, SchemaProvider S>
    void map(const S &sp, D& dataProvider) {
        _m_mapper.map(sp.schema(), dataProvider);
    }

    template<DataProvider D, SchemaProvider S>
    void _map(const S& sp, D& dataProvider){
        _m_mapper.map(sp.schema(), dataProvider);
    }

    MapImpl::ResultsType results() const noexcept { return _m_mapper.results(); }
private:
    MapImpl _m_mapper;
};
}