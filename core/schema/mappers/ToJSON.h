#pragma once
#include <bm/schema.h>
#include <io/Serialize.h>
namespace bm
{
    namespace mappers
    {
        class ToJSONMapper
        {
        public:
            using ResultsType = nlohmann::json;
            template <typename Schema_t, typename DataProvider>
                requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
            void map(const Schema_t &schema, DataProvider &dataProvider)
            {
                auto dataLen = schema.calculatedSize();
                BinData<1024> buffer;
                BinData<1024> provided;
                dataProvider.readTo(provided.data, dataLen);
                for (const auto &part : schema)
                {
                    Reader::read(part, provided.data, buffer.data);
                }
                _m_results = nlohmann::json::object();
                for (const auto &part : schema)
                {
                    _m_results[part.id] = nlohmann::json::object();
                    _m_results[part.id]["fields"] = nlohmann::json::array();

                    unsigned short fIndex{0};
                    std::size_t fieldStartOffset{part.startOffset()};
                    std::size_t fieldEndOffset{0};

                    for (const auto &field : part.fields)
                    {
                        fieldEndOffset = field.length + fieldStartOffset;
                        _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                        _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                        _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                        // Pushing entire `part` length of bytes instead of field speicifc
                        // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                        _m_results[part.id]["fields"][fIndex]["data"] = nlohmann::json::binary(
                            buffer.to_vector(fieldStartOffset, fieldEndOffset));
                        fIndex++;
                        fieldStartOffset += field.length;
                    }
                }
            }
            template <typename Schema_t, typename DataProvider>
                requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
            void map(const Schema_t &schema, DataProvider &dataProvider, ResultsType &output)
            {
                auto dataLen = schema.calculatedSize();
                BinData<1024> buffer;
                BinData<1024> provided;
                dataProvider.readTo(provided.data, dataLen);
                for (const auto &part : schema)
                {
                    Reader::read(part, provided.data, buffer.data);
                }
                _m_results = nlohmann::json::object();
                for (const auto &part : schema)
                {
                    _m_results[part.id] = nlohmann::json::object();
                    _m_results[part.id]["fields"] = nlohmann::json::array();

                    unsigned short fIndex{0};
                    std::size_t fieldStartOffset{part.startOffset()};
                    std::size_t fieldEndOffset{0};

                    for (const auto &field : part.fields)
                    {
                        fieldEndOffset = field.length + fieldStartOffset;
                        _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                        _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                        _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                        // Pushing entire `part` length of bytes instead of field speicifc
                        // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                        _m_results[part.id]["fields"][fIndex]["data"] = nlohmann::json::binary(
                            buffer.to_vector(fieldStartOffset, fieldEndOffset));
                        fIndex++;
                        fieldStartOffset += field.length;
                    }
                }
            }
            const ResultsType &results() const noexcept { return _m_results; }

        private:
            ResultsType _m_results;
        };

        // template <>
        // struct Serializer<ToJSONMapper::ResultsType>
        // {
        //     // Serializer(ToJSONMapper::ResultsType& results);
        //     Serializer(ToJSONMapper::ResultsType &results) : _mref_results{results} {}
        //     void serialize(char *dest) {}
        //     inline std::size_t size() const noexcept { return _mref_results.size(); }

        // private:
        //     ToJSONMapper::ResultsType &_mref_results;
        // };
    }

    class ToJSONMapper
    {
    public:
        using ResultsType = nlohmann::json;
        template <typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        void map(const Schema_t &schema, DataProvider &dataProvider)
        {
            auto dataLen = schema.calculatedSize();
            BinData<1024> buffer;
            BinData<1024> provided;
            dataProvider.readTo(provided.data, dataLen);
            for (const auto &part : schema)
            {
                Reader::read(part, provided.data, buffer.data);
            }
            _m_results = nlohmann::json::object();
            for (const auto &part : schema)
            {
                _m_results[part.id] = nlohmann::json::object();
                _m_results[part.id]["fields"] = nlohmann::json::array();

                unsigned short fIndex{0};
                std::size_t fieldStartOffset{part.startOffset()};
                std::size_t fieldEndOffset{0};

                for (const auto &field : part.fields)
                {
                    fieldEndOffset = field.length + fieldStartOffset;
                    _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                    _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                    _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                    // Pushing entire `part` length of bytes instead of field speicifc
                    // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                    _m_results[part.id]["fields"][fIndex]["data"] = nlohmann::json::binary(
                        buffer.to_vector(fieldStartOffset, fieldEndOffset));
                    fIndex++;
                    fieldStartOffset += field.length;
                }
            }
        }
        template <typename Schema_t, typename DataProvider>
            requires traits::SchemaProvider<Schema_t> && traits::DataProvider<DataProvider>
        void map(const Schema_t &schema, DataProvider &dataProvider, ResultsType &output)
        {
            auto dataLen = schema.calculatedSize();
            BinData<1024> buffer;
            BinData<1024> provided;
            dataProvider.readTo(provided.data, dataLen);
            for (const auto &part : schema)
            {
                Reader::read(part, provided.data, buffer.data);
            }
            _m_results = nlohmann::json::object();
            for (const auto &part : schema)
            {
                _m_results[part.id] = nlohmann::json::object();
                _m_results[part.id]["fields"] = nlohmann::json::array();

                unsigned short fIndex{0};
                std::size_t fieldStartOffset{part.startOffset()};
                std::size_t fieldEndOffset{0};

                for (const auto &field : part.fields)
                {
                    fieldEndOffset = field.length + fieldStartOffset;
                    _m_results[part.id]["fields"][fIndex] = nlohmann::json::object();
                    _m_results[part.id]["fields"][fIndex]["name"] = field.name;
                    _m_results[part.id]["fields"][fIndex]["length"] = field.length;

                    // Pushing entire `part` length of bytes instead of field speicifc
                    // {"header":{"fields":[{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"magic"},{"data":{"bytes":[224,104,105,85,85,85,0,0,112,105,105,85,85,85,0,0],"subtype":null},"length":8,"name":"reserved"}]}}
                    _m_results[part.id]["fields"][fIndex]["data"] = nlohmann::json::binary(
                        buffer.to_vector(fieldStartOffset, fieldEndOffset));
                    fIndex++;
                    fieldStartOffset += field.length;
                }
            }
        }
        const ResultsType &results() const noexcept { return _m_results; }

    private:
        ResultsType _m_results;
    };

    template <>
    struct Serializer<ToJSONMapper::ResultsType>
    {
        // Serializer(ToJSONMapper::ResultsType& results);
        Serializer(ToJSONMapper::ResultsType &results) : _mref_results{results} {}
        void serialize(char *dest) {}
        inline std::size_t size() const noexcept { return _mref_results.size(); }

    private:
        ToJSONMapper::ResultsType &_mref_results;
    };

}