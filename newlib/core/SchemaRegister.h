#pragma once
#include <cstdlib>
#include <iostream>
#include <filesystem>
#include <concepts>
#include <fstream>
#include "Schema.h"
#include "../schema.h"
// class Schema{
// public:
// template<typename IG>
// Schema(std::filesystem::path& file, IG&& id)
//     {
//         _file = std::move(file);
//         _id = id(*this);
//     }
//     operator schema_id_t(){
//         return {_id};
//     }
//     const std::filesystem::path& filePath() const noexcept { return _file; }
//     inline size_t id() const noexcept { return _id; }
// private:
//     size_t _id = 0;
//     std::filesystem::path _file;
// };
#include <future>
    struct DefaultSchemaValidator
    {
        static Schema validate(const nlohmann::json &json)
        {
            Schema ret;
            for (const auto &[key, value] : json.items())
            {
                if(!LayoutAttribute::containsProperLayout(value))
                    throw "LayoutAttribute is invalid";
                ret << LayoutAttribute{key, value};
            }
            return std::move(ret);
        }
    };
Schema schema_from_file(const std::filesystem::path& sFile){
    std::ifstream _in { sFile };
    
    char buffer[2048];
    _in.read(buffer, 2048);
    return DefaultSchemaValidator::validate(nlohmann::json::parse( buffer ));
}

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
        _paths[RegistryIndex] =std::filesystem::canonical( std::move(filePath));
        _parsing[RegistryIndex] = std::async(schema_from_file, _paths[RegistryIndex]);

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
        const auto s = _parsing[schema->id].get();
        std::cout << "[libbm]: Registered data file <"
            << filePath << "> to Schema file <"
            << _paths[schema->id] << ">\n";
    }
private:
    static inline size_t RegistryIndex = 0;
    static inline std::filesystem::path _paths[10];
    static inline std::future<Schema> _parsing[10];
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