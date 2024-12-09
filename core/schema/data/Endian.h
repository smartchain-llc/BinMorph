#pragma once
#include <memory>
#include <string_view>
#include <schema/json/FieldAttribute.h>

namespace bm::data{
class Endian{
public:
    template<typename E> Endian(const E& type):
        _m_type{E::Type},
        _m_endian_impl{&type},
        _m_read_impl{
            [](const void* endianImpl, uint8_t* src, uint8_t* dest, const std::size_t& length){
                static_cast<const E*>(endianImpl)->read(src, dest, length);
            }
        }
    {

    }
    void read(uint8_t* src, uint8_t* dest, const std::size_t& length) const {
        _m_read_impl(_m_endian_impl, src, dest, length);
    }
    inline std::string_view type() const noexcept { return _m_type; }
private:
    std::string _m_type;
    const void* _m_endian_impl;
    void(*_m_read_impl)(const void*, uint8_t*, uint8_t*, const std::size_t&);
};

std::shared_ptr<Endian> GetEndian(const json::FieldAttribute& attribute);

}