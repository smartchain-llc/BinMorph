#pragma once
#include <json.hpp>
#include <type_traits>

namespace bm{
    
// class Endianess {
// public:
//     static Endianess Little(){ return Endianess(Type::LITTLE); }
//     static Endianess Big(){ return Endianess(Type::BIG); }
// public:
//     inline operator std::string() const noexcept{ 
//         return _m_type == Type::BIG ? "big" : "little"; 
//     }
// private:
//     enum Type{ LITTLE, BIG };
//     Endianess(Type type): _m_type{std::move(type)}{}
    
//     Type _m_type;
// };
struct Endianess {
    
};

class Field{
public:
    Field(const nlohmann::json& json);

private:
    std::string _m_name;
    std::size_t _m_length;
};

}