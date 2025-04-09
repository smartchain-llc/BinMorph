#pragma once
#include <types/traits.h>
#include "ToJSONMapper.h"
namespace bm{

namespace traits{


template<typename M, typename S, typename ...Args> 
concept is_mappable = is_schema_type<S> && requires(M m, S s, Args... args){
    m.map(s, args...);
} ;

template<typename M, typename W, typename S, typename ...Args> concept results_writable = is_mappable<M, S, Args...> && requires(W w, typename M::ResultsType r){
    w.write(r);
};

template<typename M, typename W> concept requires_length = requires(W w, std::size_t l, typename M::ResultsType r){
    w.write(r, l);
};
}
template<typename M, traits::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::is_mappable<M, S, Args...> && std::is_default_constructible_v<M>,
    typename M::ResultsType
> Map(S s, Args... args){
    return M{}.map(s, args...);
}


template<typename M, traits::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::is_mappable<M, S, Args...> && !std::is_default_constructible_v<M>,
    typename M::ResultsType
> Map(M& m, S s, Args... args){
    return m.map(s, args...);
}



template<typename M, typename W, traits::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::results_writable<M, W, S, Args...> || traits::requires_length<M, W>,
    void
> MapAndWrite(S s, Args... args){
    const auto results = M{}.map(s, args...);
    if constexpr (traits::requires_length<M, W>)
        W{}.write(results, s.calculatedSize());
    else
        W{}.write(results);
}

template<typename M, typename W, traits::is_schema_type S, typename...Args>
std::enable_if_t<
    traits::results_writable<M, W, S, Args...> || traits::requires_length<M, W>,
    void
> MapAndWrite(W& w, S s, Args... args){
    const auto results = M{}.map(s, args...);
    if constexpr (traits::requires_length<M, W>)
        w.write(results, s.calculatedSize());
    else
        w.write(results);
}
}