#pragma once
#include <types/traits.h>
#include "ToJSONMapper.h"
namespace bm{

namespace traits{


template<typename M, typename S, typename ...Args> 
concept is_mappable = is_schema_type<S> && requires(M m, S s, Args... args){
    m.map(s, args...);
} ;

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
}