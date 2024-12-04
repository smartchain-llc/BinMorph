#pragma once
#include <type_traits>
#include <schema/Specification.h>

namespace bm{

template<typename S> struct is_specification : std::is_base_of<Specification, std::remove_cvref_t<S>>{
    // using SpecificationType = decltype(std::declval<S>());
    using SpecificationType = std::remove_cvref_t<S>;
};

template<typename S> using SpecificationType = std::enable_if_t<
    is_specification<S>::value,
    typename is_specification<S>::SpecificationType
>;

}