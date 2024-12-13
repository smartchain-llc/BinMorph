#pragma once

#include <schema/Schema.h>
#include <schema/data/Endian.h>
#include <schema/data/BinData.h>

namespace bm {

struct Reader {
    static void read(const LayoutAttribute& layout, uint8_t* src, uint8_t* dest){
        auto readOffset = layout.startOffset();

        for(const auto& field : layout.fields){
            GetEndian(field)->read(&src[readOffset], &dest[readOffset], field.length);
            readOffset += field.length;
        }
    }
};

class SchemaDataMapper{
public:
    SchemaDataMapper(const Schema& schema, uint8_t* dataProvider){
        auto dataLen = schema.calculatedSize();
        BinData<1024> buffer;
        
        for(const auto& part : schema){
            Reader::read(part, dataProvider, buffer.data);
        }

        for(const auto& part : schema){
            _m_results = nlohmann::json::object();
            _m_results[part.id] = nlohmann::json::object();
            _m_results[part.id]["fields"] = nlohmann::json::array();
            
            unsigned short fIndex { 0 };
            std::size_t fieldStartOffset { 0 };
            std::size_t fieldEndOffset { 0 };
            std::cout << _m_results << std::endl;
            
            for(const auto& field : part.fields){
                fieldEndOffset = field.length + fieldStartOffset;
                _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                // Pushing entire `part` length of bytes instead of field speicifc
                // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                _m_results[part.id]["fields"][fIndex]["data"] = nlohmann::json::binary(
                    buffer.to_vector(fieldStartOffset, fieldEndOffset)
                );
                fIndex++;
                fieldStartOffset += field.length;
                std::cout << _m_results << std::endl;
            }
        }
        std::cout << _m_results << std::endl;
    }
    const nlohmann::json& results() const noexcept { return _m_results; }
    // template<typename Output>
    // void results_to(Output& output) const noexcept {
    //     for(const auto& j : _m_results)
    //         j << _m_results;
    // }
private:
    Schema _m_schema;
    nlohmann::json _m_results;
};

}