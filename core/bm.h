#pragma once
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C"{
#endif

typedef struct {
    unsigned long id;
} schema_id_t;

schema_id_t register_schema(const char*);
schema_id_t register_schema_file(const char*);
void registry_list_files();
void set_schema_data_file(schema_id_t*,const char*);

#ifdef __cplusplus
}
#endif