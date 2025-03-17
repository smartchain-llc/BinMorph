// #include "../bm.h"
#include "../core/SchemaRegister.h"
#include <filesystem>
#include <iostream>

#define REGISTRY_CAPACITY 5

static std::string* __registry_file_paths[REGISTRY_CAPACITY];
static unsigned char __registered_file_paths = 0;
static std::filesystem::path __registery_data_file_paths[REGISTRY_CAPACITY];
static schema_id_t __refernces[REGISTRY_CAPACITY];

// size_t SchemaHash::operator()(Schema& schema){ return schema.id() % REGISTRY_CAPACITY; }

void register_schema_file(const char* filePath){
    std::filesystem::path _path { filePath };

    if(!std::filesystem::exists(_path))
        throw "[libbm]: Cannot register file. Does not exist.";
        
    __registry_file_paths[0] = new std::string(_path.string());
    __registered_file_paths++;
}

void registry_list_files(){
    for(char i = 0; i < __registered_file_paths; i++){
        std::cout   << "[libbm]: String Registry<" << i << "> => "
                    << *__registry_file_paths[i]
                    << std::endl;

    }
}

// size_t IdGenerator::operator()(Schema& s){
//     size_t ret;
//     char _firstC = s.filePath().c_str()[0];
//     ret = _firstC ^ __registered_file_paths;
//     return ret;
// }

bool InputFileValidator::operator()(const char* filePath){
    return false;
}
struct TrueFileValidator{
    bool operator()(const char* filePath){ return true; }
};
auto __register_impl(const char* filePath){
    auto index = SchemaRegistry::Register<TrueFileValidator>(filePath);
    __refernces[index] = {.id = index };
    return __refernces[index];
}
void __set_schema_data_file(schema_id_t* schema, const char* filePath){
    SchemaRegistry::RegisterDataFile<TrueFileValidator>(schema, filePath);
}

#ifdef __cplusplus
extern "C" {
#endif
schema_id_t register_schema(const char* filePath){
    return __register_impl(filePath);;
}
void set_schema_data_file(schema_id_t* schema, const char* filePath){
    __set_schema_data_file(schema, filePath);
}
#ifdef __cplusplus
}
#endif