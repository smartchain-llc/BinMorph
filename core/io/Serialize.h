#pragma once
#include <types/traits.h>

namespace bm{


template<typename T>
struct Serializer;

namespace traits{
template<class S>
concept HasSerializeInterface = requires(S s, char* dest){
    requires(
        requires{ s.serialize(dest) ;} &&
        requires{{s.size()} -> std::same_as<std::size_t> ;}
    );
};

template<typename T>
concept Serializable = requires{
    sizeof(Serializer<T>);
};

template<typename T> requires Serializable<T>
char* serialize(const T& t){
    return (char*)t;
}

}}