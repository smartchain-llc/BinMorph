#pragma once
#include <schema/Schema.h>
namespace bm{

template<typename MapImpl> class SchemaMapper{
public:
    SchemaMapper(){}
    SchemaMapper(MapImpl&& mapper):_m_mapper{mapper}{}
    void map(const Schema &schema, uint8_t *dataProvider) {
        _m_mapper.map(schema, dataProvider);
    }
    MapImpl::ResultsType results() const noexcept { return _m_mapper.results(); }
private:
    MapImpl _m_mapper;
};

}