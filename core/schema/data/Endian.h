#pragma once
#include <memory>
#include <string_view>
#include <schema/json/FieldAttribute.h>

namespace bm
{
/**
### Endianess
Determines the least -> most, left -> right, etc.. ordering of a collection of bytes.

### Endianess of: 32-bit `0x0A0B0C0D`
Byte positions in table are: [0 1 2 3]
Byte positions in memory are: [3 2 1 0] 
#### Little Endian
In little-endian ordering, the LSB of the word is at the smallest address, and the MSB is at the largest address. 
Little endian value: `0X0D0C0B0A`
|Address (T:M)|8-bit Value|16-bit Value|
|---|---|----|
|N+0|`0x0D`|`0x0D0C`|
|N+1|`0x0C`|`0x0D0C`|
|N+2|`0x0B`|`0x0B0A`|
|N+3|`0x0A`|`0x0B0A|

#### Big Endian
In big-endian ordering, the most significant byte (MSB) of the word is stored in the smallest address, and the least significant byte (LSB) is stored in the largest address.
Big endian value: `0x0A0B0C0D`
|Address|8-bit Value|16-bit Value|
|---|---|----|
|N|`0x0A`|`0x0A0B`|
|N+1|`0x0B`|`0x0A0B`|
|N+2|`0x0C`|`0x0C0D`|
|N+3|`0x0D`|`0x0C0D`|

*/
        class Endian
        {
        public:
            template <typename E>
            Endian(const E &type) : _m_type{E::Type},
                                    _m_endian_impl{&type},
                                    _m_read_impl{
                                        [](const void *endianImpl, uint8_t *src, uint8_t *dest, const std::size_t &length)
                                        {
                                            static_cast<const E *>(endianImpl)->read(src, dest, length);
                                        }}
            {
            }
            void read(uint8_t *src, uint8_t *dest, const std::size_t &length) const
            {
                _m_read_impl(_m_endian_impl, src, dest, length);
            }
            inline std::string_view type() const noexcept { return _m_type; }

        private:
            std::string _m_type;
            const void *_m_endian_impl;
            void (*_m_read_impl)(const void *, uint8_t *, uint8_t *, const std::size_t &);
        };
    std::shared_ptr<Endian> GetEndian(const FieldAttribute &attribute);
    }
