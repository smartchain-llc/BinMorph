#pragma once
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <concepts>
#include "../schema.h"

class Schema{
public:
template<typename IG>
Schema(std::filesystem::path& file, IG&& id)
    {
        _file = std::move(file);
        _id = id(*this);
    }
    operator schema_id_t(){
        return {_id};
    }
    const std::filesystem::path& filePath() const noexcept { return _file; }
    inline size_t id() const noexcept { return _id; }
private:
    size_t _id = 0;
    std::filesystem::path _file;
};

template<typename V> concept is_file_validator = requires(V v, const char* p){
    { v(p) } -> std::same_as<bool>;
};
class SchemaRegistry{
public:
    template<is_file_validator V>
    static inline size_t Register(const char* filePath){
        auto isValid = V{}(filePath);
        if(!isValid) {
            std::cerr << "[libbm]: File "
                << filePath 
                << " is not valid" 
                << std::endl;
            exit(1);
        }
        _paths[RegistryIndex] = std::move(filePath);
        std::cout << "[libbm]: Added file to registry => "
            << _paths[RegistryIndex]
            << std::endl;
        return RegistryIndex++;
    }

    template<is_file_validator V>
    static inline void RegisterDataFile(schema_id_t* schema, const char* filePath){
        auto isValid = V{}(filePath);
        if(!isValid) {
            std::cerr << "[libbm]: File "
                << filePath 
                << " is not valid" 
                << std::endl;
            exit(1);
        }
        _bin_paths[schema->id] = filePath;
        std::cout << "[libbm]: Registered data file <"
            << filePath << "> to Schema file <"
            << _paths[schema->id] << ">\n";
    }
private:
    static inline size_t RegistryIndex = 0;
    static inline std::filesystem::path _paths[10];
    static inline std::filesystem::path _bin_paths[10];
};

struct IdGenerator{
    size_t operator()(Schema&);
};

struct SchemaHash{
    size_t operator()(Schema&);
};

struct InputFileValidator{
    bool operator()(const char*);
};