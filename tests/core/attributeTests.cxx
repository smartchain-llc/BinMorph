#include <gtest/gtest.h>
#include <bm.h>
#include <TestUtils.h>
#include <cstdarg>
#include <cstddef>
#include <stdlib.h>
using namespace bm;
// template<unsigned N, typename... T> struct DetermineType{ 
//     using type = 
// }
// template<typename...T> struct TypeOf {
//     template<unsigned N> constexpr auto at(std::tuple<T...>& values) {
//         return std::get<N>(values);
//     }
// };

template<std::size_t I> struct AttributeGet{
    template<typename Attr, typename Callable>
    static void process(Attr& t, Callable&& fn){
        fn(std::get<I>(t));
        AttributeGet<I-1>::process(t, std::forward<Callable>(fn));
    }
};
template<> struct AttributeGet<0>{
    template<typename Attr, typename Callable>
    static void process(Attr& t, Callable&& fn){
        fn(std::get<0>(t));
        // return [](){ return std::get<0>(t);}
    }
};
template<typename...AttributeTypes> struct Specification{

    Specification(std::initializer_list<std::string> ids): 
        _m_values{ std::make_tuple<AttributeTypes...>(AttributeTypes{}...) }{
        uint8_t _id_i{0};
        for(const auto& id : ids)
            strcpy(_m_ids[_id_i++], id.data()); /// TODO: memmove ??
    }

    template<typename Callable>
    void valueOf(std::size_t i, Callable&& cb){
        constexpr std::size_t _al = std::tuple_size_v<decltype(_m_values)>;
        AttributeGet<_al - 1>::process(_m_values, std::forward<Callable>(cb));
    }
    
    template<std::size_t i, typename Callable>
    void setValueAt(Callable&& cb){
        constexpr std::size_t _al = std::tuple_size_v<decltype(_m_values)>;
        cb(std::get<i>(_m_values));
    }

    template<typename Callable>
    void setValueById(const char* Id, Callable&& cb){
        for(auto i = 0; i < _m_ids.size(); i++){
            if(strcmp(_m_ids[i], Id))
                cb(std::get<i>(_m_values));
        }
    }
    std::tuple<AttributeTypes...> _m_values;
    std::array<char[256], sizeof...(AttributeTypes)> _m_ids;
    uint8_t _m_att_count = sizeof...(AttributeTypes);
};

// #define DECLARE(A) __typeof(A) _m A ##;
template<template <typename...> class Attr, typename... Ts> using AttrTypes = Specification<Ts...>;

template<typename T>
struct TestMe{

};

TEST(AttributeSpecification, RecievesIDArgsForEachAttributeType){
    Specification<std::string, std::size_t, double> spec{"name", "length", "value"};
    const char* expected { "length" };
    ASSERT_EQ(strcmp(spec._m_ids[1], expected),0);
    const char* id = "length";
    spec.setValueById(id, [](auto& value){ 
        value = 0x69;
    });
    spec.valueOf(1, [](const auto& value){
        std::cout << value << std::endl;
    });
    // auto len = spec.get(0);
}