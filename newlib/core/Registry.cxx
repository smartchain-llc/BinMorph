#include "../bm.h"
#include "Registry.h"
#include <filesystem>
#include "Schema/Schema.h"

schema_id_t __register_impl(const char* json){
    schema_id_t ret;
    Schema s = from_literal(json);
}


#ifdef __cplusplus
extern "C" {
#endif
schema_id_t register_schema(const char* json){
    return __register_impl(json);
}
schema_id_t register_schema_file(const char* filePath){
    return {};
}
// void set_schema_data_file(schema_id_t* schema, const char* filePath){
//     __set_schema_data_file(schema, filePath);
// }
#ifdef __cplusplus
}
#endif