#pragma once
#include <schema/Schema.h>
namespace bm{

template<typename MapImpl> class SchemaMapper{
public:
    void map(const Schema &schema, uint8_t *dataProvider) {
        _m_mapper.map(schema, dataProvider);
    }
    const MapImpl::ResultsType& results() const noexcept { return _m_mapper.results(); }
private:
    MapImpl _m_mapper;
    MapImpl::ResultsType _m_results;
};

}