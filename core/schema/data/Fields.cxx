#include <schema/data/Fields.h>
#include <iostream>
using namespace bm::data;
using namespace bm::json;

Field::Field(const FieldAttribute &data) : _m_name{std::move(data.name)},
                                           _m_length{std::move(data.length)}
{
    _m_endian = GetEndian(data);
}
