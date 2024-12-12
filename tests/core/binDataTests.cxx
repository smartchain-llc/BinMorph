#include <gtest/gtest.h>
#include <TestUtils.h>
#include <schema/Parser.h>
#include <schema/data/Endian.h>
using namespace bm;
using namespace nlohmann;

static const auto ValidJSON = json::parse(R"(
    {
        "p1":{
            "offset":0,
            "fields":[
                {"name":"h1","length":8,"endian":"big"},
                {"name":"h2","length":8,"endian":"big"}
            ]
        },"p2":{
            "offset":16,
            "fields":[
                {"name":"p2","length":16,"endian":"big"}
            ]
        }
    }
)");
static const auto BinaryData = []()
{
    auto WriteHeader = test_utils::WriteStrategy<16>([](const std::size_t& i, uint8_t& c){
        c = (i % 2 == 0) ? 0xFF : 0x69;
    });
    auto WritePayload = test_utils::WriteStrategy<16>([](const std::size_t& i, uint8_t& c){
        c = i;
    });

    auto genData = test_utils::GenerateBinaryData<32>();
    genData += WriteHeader;
    genData += WritePayload;
    return genData;
};

struct Reader {
    static void read(const LayoutAttribute& layout, uint8_t* src, uint8_t* dest){
        auto readOffset = layout.startOffset();

        for(const auto& field : layout.fields){
            GetEndian(field)->read(&src[readOffset], &dest[readOffset], field.length);
            readOffset += field.length;
        }
    }
};
struct BinData{
    BinData(uint8_t* d, const std::size_t& len): data{memcpy(data, d, len)}{}
    uint8_t* data { nullptr };
};
TEST(BinaryDataFromSchema, CanReadBinaryDataAccordingToSchema)
{
    Schema schema;
    Parser::ParseTo(schema, ValidJSON);

    auto data = BinaryData();
    uint8_t formattedData[32];
    for(const auto& part : schema){
        Reader::read(part, data, formattedData);
    }
    uint64_t* head = (uint64_t*)(formattedData);
    uint64_t expected = 0x69FF69FF69FF69FF;
    ASSERT_EQ(*head, 0x69FF69FF69FF69FF);
}