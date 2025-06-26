#include <cstddef>
#include <cstdint>
#pragma once
namespace typeGen
{
    struct HeaderPartition
    {
        std::size_t offset{0};
        std::size_t length{16};
        uint8_t magic[8];
        uint8_t version[8];
    };

};
