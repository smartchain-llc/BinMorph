#include "bm.h"

#ifdef __cplusplus
extern "C" {
#endif
schema_id_t register_schema(const char* json){
    return schema_id_t{.id = 0};
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