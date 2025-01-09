#pragma once
#include "Schema.h"
#include <types/traits.h>

namespace bm{

namespace traits{

template<typename V> concept is_schema_validator = requires(V v, nlohmann::json json){
    // {V::validate(json)  } noexcept -> std::same_as<std::add_rvalue_reference< Schema> >;
    { V::validate(json)  } -> std::same_as<Schema&&>;
};

template<typename T> using is_json_type = std::is_same<T, nlohmann::json>::value;
template<typename T> using is_json_convertible = std::is_convertible<T, nlohmann::json>::value;


}


struct DefaultSchemaValidator{
    template<std::convertible_to<nlohmann::json> SchemaInput>
    static Schema&& validate(const SchemaInput& json){
        Schema ret;
        if constexpr ( ! std::is_same_v<SchemaInput, nlohmann::json> ){
            
        }
        else {
            
        }
        return std::move(ret);
    }
};

}