#pragma once
#include <cstdint>

namespace bm{


/**
 ### BinData
 Stores "in-memory" block of data
 #### Example
 ```
uint64_t expected = 0x69FF69FF69FF69FF;
 ```
Is stored in memory as: `0xFF 69 FF 69 FF 69 FF 69` (little-endian)
*/
template<std::size_t Length>
struct BinData{
    BinData(){}
    BinData(uint8_t* d)
    {
        memcpy(data, d, Length);
    }
    BinData(std::initializer_list<uint8_t> bytes){
        for(auto i = 0; i < Length; i++)
            data[i] = *(bytes.begin()+i);
    }
    template<std::size_t PartLength> BinData<PartLength>parition(std::size_t from) const noexcept{
        BinData<PartLength> ret;
        const auto partLen = PartLength+from;
        if(partLen > Length)
            throw "partition length would exceed bin data capacity";
        std::size_t retIndex { 0 };
        for(auto i = from; i < partLen; i++)
            ret.data[retIndex++] = data[i];
        return ret;
    }
    // std::vector<uint8_t> to_vector() const noexcept {
    //     std::vector<uint8_t> ret;
    //     uint8_t* data;
    //     for(auto i = 0; i < Length; i++)
    //         ret.push_back(*(data + i));
    // }
    std::vector<uint8_t> to_vector(const std::size_t& from, const std::size_t& to) const noexcept {
        std::vector<uint8_t> ret;
        for(auto i = from; i < to && i < Length; i++)
            ret.push_back(data[i]);
        return ret;
    }


    uint8_t data[Length];
};
template<std::size_t Length>
bool BinaryCompare(const BinData<Length>& input, const BinData<Length>& output){
    for(auto i = 0; i < Length; i++)
        if(input.data[i] != output.data[i])
            return false;
    return true;
}

}