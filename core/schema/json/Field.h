#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Field{
    unsigned long length;
    const char* name;
    const char* endian;
};

Field create_field();

#ifdef __cplusplus
}
#endif