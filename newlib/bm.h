#pragma once
#include <cstdlib>

#ifdef __cplusplus
extern "C"{
#endif
struct schema_id_t;

schema_id_t* register_schema(const char*);
void register_schema_file(const char*);
void registry_list_files();

#ifdef __cplusplus
}
#endif