#pragma once
#include <schema/Schema.h>
#include <bm/traits.h>
#include <schema/mappers/Results.h>
namespace bm
{

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
            return {map_data_against_schema<Mapper>(sp, dp)};
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

    Results<typename Mapper::ResultsType> map_data_against_schema(const Schema_t &sp, const DataProvider &dp)
    {
        Mapper _mapper;
        _mapper.map(sp, dp);
        return _mapper.results();
    }
    template <typename Mapper, typename Schema_t, typename DataProvider>
        requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>

    Results<typename Mapper::ResultsType> map_data_against_schema(const Schema_t &sp, DataProvider &&dp)
    {
        Mapper _mapper;
        _mapper.map(static_cast<Schema>(sp), dp);
        return _mapper.results();
    }

    template <typename Mapper, typename Schema_t, typename DataProvider>
        requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
    Results<typename Mapper::ResultsType> get_results_of(const Schema_t &sp, DataProvider &&dp)
    {
        Mapper _mapper;
        _mapper.map(static_cast<Schema>(sp), dp);
        return {_mapper.results()};
    }

    template <typename Mapper, typename SchemaProvider, bm::traits::IsByteType Data>
        requires traits::SchemaProvider<SchemaProvider>
    Results<typename Mapper::ResultsType> get_results_of(const SchemaProvider &sp, Data data)
    {
        Mapper _mapper;
        _mapper.map(static_cast<Schema>(sp), data);
        return {_mapper.results()};
    }
    namespace clean
    {
        template <typename M>
        using mapper_results = decltype(std::declval<M>().results());

        template<typename S>
        concept is_schema_type = std::same_as<bm::Schema, S> || std::convertible_to<S, bm::Schema>;



        template <typename M, typename S, typename D>
        concept is_mappable = is_schema_type<S> && bm::traits::DataProvider<D> && requires(M m, S s, D d) {
            typename M::ResultsType;
            
            { m.map(s, d) };
            { m.results() } noexcept -> std::convertible_to<typename M::ResultsType>;
        };

        template<typename M, is_schema_type S, bm::traits::DataProvider D> requires is_mappable<M, S, D>
        bm::Results<typename M::ResultsType> get_results_of(const S& schema, const D& data){

        }

    }
}
