#pragma once

#include <schema/data/Endian.h>
namespace bm{


    struct Reader
    {
        static void read(const LayoutAttribute &layout, uint8_t *src, uint8_t *dest)
        {
            auto readOffset = layout.startOffset();

            for (const auto &field : layout.fields)
            {
                GetEndian(field)->read(&src[readOffset], &dest[readOffset], field.length);
                readOffset += field.length;
            }
        }
    };
}