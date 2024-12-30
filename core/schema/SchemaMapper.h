#pragma once
#include <schema/Schema.h>
#include <bm/traits.h>
namespace bm
{

    template <typename MapImpl>
    class _SchemaMapper
    {
    public:
        _SchemaMapper() {}
        _SchemaMapper(MapImpl &&mapper) : _m_mapper{mapper} {}
        template <typename S, typename D>
            requires SchemaProvider<S> && DataProvider<D>
        void map(const S &schema, D &dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }
        template <typename S, typename D>
            requires SchemaProvider<S> && DataProvider<D>
        void map(const S &schema, D &&dataProvider)
        {
            _m_mapper.map(schema, dataProvider);
        }

        MapImpl::ResultsType results() const noexcept { return _m_mapper.results(); }

    private:
        MapImpl _m_mapper;
    };

    class SchemaMapper
    {
    public:
        template<typename T>
        struct Results{
            Results(const T& r): results{std::move(r)}{}
            T results;
        };
        template <typename MapImpl, typename S, typename D>
            requires bm::SchemaProvider<S> && bm::DataProvider<D>
        SchemaMapper(MapImpl &&mapper, const S &sp, D &dp)
        {
            mapper.map(sp, dp);
        }
        template <typename MapImpl, typename S, typename D>
            requires bm::SchemaProvider<S> && bm::DataProvider<D>
        static Results<typename MapImpl::ResultsType> map(const S &sp, D &dp)
        {
            return { map_data_against_schema<MapImpl>(sp, dp) };
        }
    };

    template <typename MapImpl, typename S, typename D>
        requires bm::SchemaProvider<S> && bm::DataProvider<D>
    void map_data_against_schema(MapImpl &&mapper, const S &sp, D &dp)
    {
        mapper.map(sp, dp);
    }
    template <typename MapImpl, typename S, typename D>
        requires bm::SchemaProvider<S> && bm::DataProvider<D>

    MapImpl::ResultsType map_data_against_schema(const S &sp, const D &dp)
    {
        MapImpl _mapper;
        _mapper.map(sp, dp);
        return _mapper.results();
    }
    template <typename MapImpl, typename S, typename D>
        requires bm::SchemaProvider<S> && bm::DataProvider<D>

    MapImpl::ResultsType map_data_against_schema(const S &sp, D &&dp)
    {
        MapImpl _mapper;
        _mapper.map(sp, dp);
        return _mapper.results();
    }
}