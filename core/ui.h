#pragma once
#include "schema/FieldAttribute.h"

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    size_t length;
    const char* name;
    const char* endian;
} bm_FieldAtt;

void bm_fa_create(const char*, bm_FieldAtt*);

const char* to_json(const char*);

#ifdef __cplusplus
}
#endif