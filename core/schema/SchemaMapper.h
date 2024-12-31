#pragma once
#include <schema/Schema.h>
#include <bm/traits.h>
#include <schema/mappers/Results.h>
namespace bm
{

    template <typename Mapper>
    class _SchemaMapper
    {
    public:
        _SchemaMapper() {}
        _SchemaMapper(Mapper &&mapper) : _m_mapper{mapper} {}
        template <typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        void map(const Schema_t &schema, DataProvider &dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }
        template <typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        void map(const Schema_t &schema, DataProvider &&dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }

        Mapper::ResultsType results() const noexcept { return _m_mapper.results(); }

    private:
        Mapper _m_mapper;
    };

    class SchemaMapper
    {
    public:
        template <typename Mapper, typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        SchemaMapper(Mapper &&mapper, const Schema_t &sp, DataProvider &dp)
        {
            mapper.map(sp, dp);
        }
        template <typename Mapper, typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        static Results<typename Mapper::ResultsType> map(const Schema_t &sp, DataProvider &dp)
        {
            return { map_data_against_schema<Mapper>(sp, dp) };
        }
    };

    template <typename Mapper, typename Schema_t, typename DataProvider>
        requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
    void map_data_against_schema(Mapper &&mapper, const Schema_t &sp, DataProvider &dp)
    {
        mapper.map(sp, dp);
    }
    template <typename Mapper, typename Schema_t, typename DataProvider>
        requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>

    Mapper::ResultsType map_data_against_schema(const Schema_t &sp, const DataProvider &dp)
    {
        Mapper _mapper;
        _mapper.map(sp, dp);
        return _mapper.results();
    }
    template <typename Mapper, typename Schema_t, typename DataProvider>
        requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>

    Mapper::ResultsType map_data_against_schema(const Schema_t &sp, DataProvider &&dp)
    {
        Mapper _mapper;
        _mapper.map(sp, dp);
        return _mapper.results();
    }
}