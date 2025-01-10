#pragma once
#include "json.hpp"
#include <types/traits.h>

namespace bm
{

    struct ToJSONMapper
    {
        ToJSONMapper() = default;
        using ResultsType = nlohmann::json;

        template <bm::traits::is_schema_type S>
        ResultsType map(S s, char *buffer, const std::size_t &len) // buffer should be vector<char>
        {
            nlohmann::json _m_results = nlohmann::json::object();
            const auto schema = static_cast<Schema>(s);
            for (const auto &part : schema)
            {
                _m_results[part.id] = nlohmann::json::object();
                _m_results[part.id]["fields"] = nlohmann::json::array();

                unsigned short fIndex{0};
                std::size_t fieldStartOffset{part.startOffset()};
                std::size_t fieldEndOffset{0};

                for (const auto &field : part.fields)
                {
                    std::vector<uint8_t> bytes;
                    bytes.reserve(field.length);
                    for (auto i = 0; i < field.length; i++)
                        bytes.push_back(buffer[i]);

                    fieldEndOffset = field.length + fieldStartOffset;
                    _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                    _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                    _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                    // Pushing entire `part` length of bytes instead of field speicifc
                    // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                    _m_results[part.id]["fields"][fIndex]["data"] =
                        nlohmann::json::binary(bytes);
                    fIndex++;
                    fieldStartOffset += field.length;
                }
            }
            return _m_results;
        }
    };

}