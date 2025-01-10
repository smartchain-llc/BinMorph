#pragma once
#include "Schema.h"
#include <types/traits.h>

namespace bm
{

    namespace traits
    {

        template <typename V>
        concept is_schema_validator = requires(V v, nlohmann::json json) {
            { V::validate(json) } -> std::same_as<Schema>;
        };

        template <typename T>
        using is_json_type = std::is_same<T, nlohmann::json>::value;
        template <typename T>
        using is_json_convertible = std::is_convertible<T, nlohmann::json>::value;

    }

    struct DefaultSchemaValidator
    {
        static Schema validate(const nlohmann::json &json)
        {
            Schema ret;
            for (const auto &[key, value] : json.items())
            {
                if(!LayoutAttribute::containsProperLayout(value))
                    throw "LayoutAttribute is invalid";
                ret << bm::LayoutAttribute{key, value};
            }
            return std::move(ret);
        }
    };

}