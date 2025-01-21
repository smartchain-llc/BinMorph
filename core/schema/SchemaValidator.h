#pragma once
#include "Schema.h"
#include <types/traits.h>

namespace bm
{

    namespace traits
    {

        /// @brief Schema Validator Concept
        /// Validators responsibility is to read JSON and handle invalid JSON errors.
        template <typename V>
        concept is_schema_validator = std::same_as<V, std::function<Schema(nlohmann::json)>> || requires(V v, nlohmann::json json) {
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

    struct ProceduralOffsetValidator{
        static Schema validate(const nlohmann::json &json){
            Schema ret;
            std::size_t _offset { 0 };
            nlohmann::json _w_offset = {};

            for(const auto&[key, value] : json.items()){
                _w_offset = value;
                _w_offset += {"offset", _offset};
                std::cout << "_w_offset: \n" << _w_offset << std::endl;
                if(!LayoutAttribute::containsProperLayout<ProceduralOffsetLayouts>(_w_offset))
                    throw "[PROFF_V] LayoutAttribute is invalid";
                
                auto _layout = bm::LayoutAttribute{key, _w_offset};
                _offset += _layout.endOffset();
                ret << _layout;
            }
            return std::move(ret);
        }
    };
}