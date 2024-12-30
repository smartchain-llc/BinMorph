#pragma once
#include <cstring>
#include <cstdint>

namespace bm{
struct BufferedData {
    BufferedData(uint8_t* buffer, std::size_t byteLength):
        _buf{buffer},
        _len{byteLength}
    {}
    void readTo(uint8_t* dest, std::size_t len){
        memcpy(dest, _buf, _len);
    }
    std::size_t length() noexcept { return _len; }
    uint8_t* _buf;
    std::size_t _len;
};

// BufferedData create_buffer(uint8_t* input, std::size_t byteLength){
//     return BufferedData(input, )
// }
}