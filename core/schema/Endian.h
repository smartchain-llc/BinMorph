#pragma once
#include <memory>

namespace bm{
namespace data{

    
        // _m_read_impl{
        //     [*this](uint8_t* src, uint8_t* dest, const std::size_t& length){
        //         static_cast<const E*>(_m_endian_impl)->read(src, dest, length);
        //     }
        // }
class Endian{
public:
    template<typename E> Endian(const E& type):
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
private:
    const void* _m_endian_impl;
    void(*_m_read_impl)(const void*, uint8_t*, uint8_t*, const std::size_t&);
};

}
}