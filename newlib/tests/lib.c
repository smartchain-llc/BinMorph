#include "../bm.h"
#include <stdio.h>

int main(){
    const char* json = "{\"1\":{\"offset\":0, \"fields\":{\"name\": \"magic\", \"length\": 8, \"endian\": \"little\"}}}";
    schema_id_t schema = register_schema(json);
    // const char* file = "./256.json";
    // schema_id_t ret = register_schema(file);
    // printf("Register to ID: %d\n",ret.id);
    // set_schema_data_file(&ret, "/tmp/data.bin");
    return 0;
}