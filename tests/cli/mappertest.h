#pragma once

#include <bm.h>
#include <types/traits.h>


/**
 * Function template cannot be partially specialized
 */
// template<typename S>
// void bmmap<bm::ToJSONMapper, S>(S s, char* dest, int i){

// }

template<typename M> struct Mapper: M {

};

template<typename M> concept is_mapper = requires{
    typename Mapper<M>;
};

struct CharBuffToStringMapper
{
    using ResultsType = std::string;

    ResultsType map(bm::Schema s, char *d, const std::size_t &len)
    {
        std::stringstream ss;
        for (auto i = 0; i < len; i++)
            ss << "0x" << std::hex << (short)d[i] << " ";
        return {ss.str()};
    }
};

template<typename M> struct Map : std::enable_if_t<
    true,
    std::true_type
>{
    template<typename S>
    void operator()(S s){}  
};

template<> struct Map<bm::ToJSONMapper> : std::true_type {
    template<typename S>
    void operator()(S s, char* dest, int l){}
};

template<typename M, typename S> std::enable_if_t<
    bm::traits::is_schema_type<S>,
    void
>
bmmap(S s){

}