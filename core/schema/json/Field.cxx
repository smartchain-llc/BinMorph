#include "Field.h"

Field create_field(){
    Field ret = {
        .length = 16,
        .name = "TestField",
        .endian = "big"
    };
    return ret;
}