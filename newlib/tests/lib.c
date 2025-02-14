#include "../bm.h"
#include <stdio.h>

int main(){
    const char* file = "./lib.c";
    schema_id_t ret = register_schema(file);
    printf("Register to ID: %d\n",ret.id);
    set_schema_data_file(&ret, "/tmp/data.bin");
    return 0;
}