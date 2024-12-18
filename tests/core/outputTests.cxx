#include <gtest/gtest.h>
#include <TestUtils.h>

#include <schema/Parser.h>
#include <schema/SchemaResults.h>

using namespace bm;

struct Output
{
};
struct FileOutput
{
};

template <typename ReportWriter>
struct DataMapper : public SchemaDataMapper
{
    DataMapper(const Schema &schema, uint8_t *dataProvider) : SchemaDataMapper(schema, dataProvider)
    {
    }
};
#include <fstream>
TEST(ResultsOutputAbstraction, HasInterfaceForWritingSchemaResutls)
{
    auto data = test_utils::GenerateBinaryData<256>();
    auto DataWriter = test_utils::WriteStrategy<256>([](const auto &i, uint8_t &c)
                                                     {
        if(i < 16)
            c = (i % 3) ? i : 0x01;
        if(i < (256-16) && i > 16){
            if(i % 4)
                c = 0x04;
            if(i % 6)
                c = 0x69;
            if(i % 9)
                c = 0xFF;
        } });
    data += DataWriter;
    test_utils::CreateAndWrite(std::filesystem::path(TEST_DATA_PATH) / "256.bin", data, 256);

    Schema schema;
    Parser::ParseTo(schema, test_utils::JSONFrom("256"));
    SchemaDataMapper mapper{schema, data};

    FileOutput results;
}
template<typename M> struct Mapper {
    Mapper(){}
    void map(const Schema &schema, uint8_t *dataProvider) {
        _m_mapper.map(schema, dataProvider);
    }
    M _m_mapper;
    M::ResultsType _m_results;
};
struct ToJSON
{
    using ResultsType = nlohmann::json;
    ToJSON() = default;
    void map(const Schema &schema, uint8_t *dataProvider)
    {
        auto dataLen = schema.calculatedSize();
        BinData<1024> buffer;

        for (const auto &part : schema)
        {
            Reader::read(part, dataProvider, buffer.data);
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
    nlohmann::json _m_results;
};

TEST(SchemaResultsService, AnAbstractionToProcessSchemaResults)
{
    auto data = test_utils::GenerateBinaryData<256>();
    auto DataWriter = test_utils::WriteStrategy<256>([](const auto &i, uint8_t &c)
                                                     {
        if(i < 16)
            c = (i % 3) ? i : 0x01;
        if(i < (256-16) && i > 16){
            if(i % 4)
                c = 0x04;
            if(i % 6)
                c = 0x69;
            if(i % 9)
                c = 0xFF;
        } });
    data += DataWriter;
    test_utils::CreateAndWrite(std::filesystem::path(TEST_DATA_PATH) / "256.bin", data, 256);

    Schema schema;
    Parser::ParseTo(schema, test_utils::JSONFrom("256"));
    // SchemaDataMapper mapper{schema, data};
    Mapper<ToJSON> jsonResults;
    jsonResults.map(schema, data);

}