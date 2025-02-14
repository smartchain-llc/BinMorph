// #include "../bm.h"
#include "../core/SchemaRegister.h"
#include <filesystem>
#include <iostream>

#define REGISTRY_CAPACITY 5

static std::string* __registry_file_paths[REGISTRY_CAPACITY] = { NULL };
static unsigned char __registered_file_paths = 0;

class SchemaRegistry{
public:
    static inline void Register(std::filesystem::path& filePath){
        _paths[0] = std::move(filePath);
        std::cout << "[libbm]: Added file to registry => "
            << _paths[0];
    }
private:
    static std::filesystem::path _paths[10];
};

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

size_t IdGenerator::operator()(Schema& s){
    size_t ret;
    char _firstC = s.filePath().c_str()[0];
    ret = _firstC ^ __registered_file_paths;
    return ret;
}

schema_id_t *register_schema(const char* filePath){
    std::filesystem::path _path { filePath };
    Schema s = Schema( _path, IdGenerator{} );
    schema_id_t* ret = new schema_id_t;

    if(!std::filesystem::exists(_path))
        throw "[libbm]: Cannot register file. Does not exist.";
        
    __registry_file_paths[0] = new std::string(_path.string());
    __registered_file_paths++;

    ret->id = s.id();
    return ret;
}