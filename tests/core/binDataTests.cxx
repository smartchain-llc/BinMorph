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

