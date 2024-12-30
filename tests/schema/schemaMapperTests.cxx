#include <gtest/gtest.h>
#include <schema/SchemaMapper.h>
#include <schema/Parser.h>
#include <schema/mappers/ToJSON.h>
#include <schema/data/Reader.h>
#include <TestUtils.h>


struct SLayout : public bm::LayoutAttribute
{
    SLayout(const bm::LayoutAttribute &l, uint8_t *from) : _m_lref{l},
                                                           _m_serialize{
                                                               [&](uint8_t *to)
                                                               {
                                                                   uint8_t data[_m_lref.length];
                                                                   std::size_t putIndex{0};
                                                                   memcpy(&to[putIndex], l.id.data(), l.id.length());
                                                                   putIndex += l.id.length();
                                                                   memcpy(&to[putIndex], &(l.offset), sizeof(std::size_t));
                                                                   putIndex += sizeof(std::size_t);
                                                                   std::size_t fieldStartOffset{l.startOffset()};
                                                                   std::size_t fieldEndOffset{0};
                                                               }}
    {
    }
    void serialize(uint8_t *to)
    {
        _m_serialize(to);
    }
    const bm::LayoutAttribute &_m_lref;
    std::function<void(uint8_t *)> _m_serialize;
};

struct S
{
    S(const bm::LayoutAttribute &l) : _m_lref{l} {}
    void serialize(uint8_t *from, uint8_t *to) noexcept
    {
        std::size_t putIndex{_m_lref.startOffset()};
        memcpy(&to[putIndex], _m_lref.id.data(), _m_lref.id.length());
        putIndex += _m_lref.id.length();
        memcpy(&to[putIndex], &(_m_lref.offset), sizeof(std::size_t));
        putIndex += sizeof(std::size_t);
        std::size_t fieldStartOffset{_m_lref.startOffset()};
        std::size_t fieldEndOffset{0};
        for (const auto &field : _m_lref.fields)
        {
            memcpy(&to[putIndex], &(field.length), sizeof(std::size_t));
            putIndex += sizeof(std::size_t);
            memcpy(&to[putIndex], field.name.data(), field.name.length());

            fieldEndOffset = field.length + fieldStartOffset;
            putIndex += field.name.length();
            memcpy(&to[putIndex], &from[fieldStartOffset], field.length);
            fieldStartOffset += field.length;
            putIndex += field.length;
        }
    }
    const bm::LayoutAttribute &_m_lref;
};
#include <concepts>
template <typename L>
concept Serializable = requires(L l, uint8_t *from, uint8_t *to) {
    { l.serialize(from, to) };
};
template <Serializable L>
void serialize(L &&l, uint8_t *from, uint8_t *to)
{
    l.serialize(from, to);
}

struct MemWriter
{
    static void read(const bm::Schema &schema, uint8_t *from, uint8_t *to)
    {
        std::size_t putIndex{0};
        for (const auto &part : schema)
        {
            serialize(S{part}, from, to);
        }
    }
};
struct FromBuffer {
    FromBuffer(uint8_t* buffer, std::size_t byteLength):
        _buf{buffer},
        _len{byteLength}
    {}
    void readTo(uint8_t* dest, std::size_t len){
        memcpy(dest, _buf, _len);
    }
    std::size_t length() noexcept { return _len; }
    uint8_t* _buf;
    std::size_t _len;
};

struct ToMemMapper
{
    ~ToMemMapper() { delete[] _m_mem; }
    using ResultsType = uint8_t *;
    template<bm::DataProvider D>
    void map(const bm::Schema &schema, D& dataProvider)
    {
        auto dataLen = schema.calculatedSize();
        _m_mem = new uint8_t[(dataLen * 2)];
        uint8_t binData[2048];
        dataProvider.readTo(binData, dataLen);
        MemWriter::read(schema, binData, _m_mem);
    }
    uint8_t *results() const noexcept { return _m_mem; }
    uint8_t *_m_mem{nullptr};
};

TEST(JSONMapper, MapsBinDataAgainstSchemaToJSONResults)
{
    auto [data, inschema] = test_utils::Generate256Bytes();
    bm::Schema schema;
    bm::Parser::ParseTo(schema, inschema);

    bm::_SchemaMapper<bm::ToJSONMapper> mapper; 
    FromBuffer from{data, 256};
    mapper.map(schema, from);

    ASSERT_TRUE(!mapper.results()["header"].is_null());

    bm::_SchemaMapper<ToMemMapper> memMap;
    memMap.map(schema, from);
    auto mem = memMap.results();
    ASSERT_FALSE(mem == nullptr);
}
// struct ResultsOutput{
//     template<typename M>
//     ResultsOutput(const bm::SchemaMapper<M> m){}
// };
struct JSONFileOutput {
    template<typename M>
    JSONFileOutput(const bm::_SchemaMapper<M>& m){
        std::ofstream outFile { std::filesystem::path(TEST_DATA_PATH) / "out", std::ios::trunc };
        const auto results = m.results();
        outFile << results;
        outFile.flush();
    }
};
TEST(MapperResultsOutput, CanTakeAnyMapperResultsAndOutputAsDesired){
    auto [data, inschema] = test_utils::Generate256Bytes();
    bm::Schema schema;
    bm::Parser::ParseTo(schema, inschema);

    bm::_SchemaMapper<ToMemMapper> mapper;
    FromBuffer from{data, 256};
    mapper.map(schema, from);
    JSONFileOutput out { mapper };
    // mapper.map(schema, data);


}
#include <schema/data/BinData.h>
#include <bm/input.h>
struct FileDataProvider : public bm::InputFile<bm::DefaultFileValidator>{
    FileDataProvider(std::filesystem::path filePath): 
        bm::InputFile<bm::DefaultFileValidator>(filePath)
    {
        if(!isValid()) throw "File failed validation";
        _m_in = std::ifstream { filePath };
        _m_byteLength = std::filesystem::file_size(filePath);
    }
    void readTo(uint8_t* dest, std::size_t len) { 
        const auto fileSize = std::filesystem::file_size(filePath());
        _m_in.read((char*)dest, len);
    }
    std::size_t length() const noexcept { return _m_byteLength; }
    std::ifstream _m_in;
    std::size_t _m_byteLength;
};

TEST(DataProviderConcept, CanBeUsedWithSchemaMapper){
    auto [data, inschema] = test_utils::Generate256Bytes();
    bm::Schema schema;
    bm::Parser::ParseTo(schema, inschema);

    bm::_SchemaMapper<ToMemMapper> mapper;
    const auto binFile = std::filesystem::path(TEST_DATA_PATH) / "256.bin";
    FileDataProvider bin {binFile};
    mapper.map(schema, bin);
}
TEST(CleanDP, ShouldBeAbleToOnlyPassProviderType){
    std::filesystem::path inSchemaPath{std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json"};
    bm::SchemaFile schemaFile{inSchemaPath};
    
    bm::_SchemaMapper<bm::ToJSONMapper> mapper;
    mapper.map(schemaFile, FileDataProvider{schemaFile});
    std::cout << mapper.results() << std::endl;
}