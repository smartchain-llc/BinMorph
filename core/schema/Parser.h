#pragma once
#include <json.hpp>
namespace bm{

struct Object{
    Object(const std::string& key, const nlohmann::json& value):
        m_key{std::move(key)},
        m_value{value}
        {}
    std::string m_key;
    nlohmann::json m_value;
};
struct Value{};

class SchemaParser{
public:
    static void Print(const nlohmann::json& json){
        for (const auto&[key, value] : json.items())
            std::cout << "Key: " << key << "\tValue:\n\t" << value <<std::endl;
    }
    static void ValueTypes(const nlohmann::json& json){
        for (const auto& [key,value] : json.items())
            std::cout << "Key: " << key << " type: " << value.type_name() << "\n\tvalue: " << value << std::endl;
    } 
    static void ParseAnd(const nlohmann::json& json, std::function<void(Object&)> cb){
        for (const auto&[key, value] : json.items()){
            Object o {key, value};
            cb(o);
        }

    }
    static inline int index = 1;
    static std::vector<Object> Get(const nlohmann::json& json){
        std::vector<Object> ret;
        for(const auto[key, value] : json.items())
            ret.emplace_back(Object{key, value});
        return ret;
    }
    template<typename T> T ParseAs(const nlohmann::json& json){

    }
};

}