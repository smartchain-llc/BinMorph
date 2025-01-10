#include <gtest/gtest.h>
#include <types/traits.h>
#include <bm.h>

struct DataWrapper{
    uint8_t d[8] = {0x0,0x1,0x0,0x0,0x0,0x0,0x8};
    uint8_t* data() { return d; }
};

template <bm::traits::is_schema_type S, bm::traits::DataProvider D>
nlohmann::json uses_provider(S s, D d){
    auto data = d.data();
    using ByteType = bm::traits::is_byte_container<decltype(data)>::byte_type;
    std::vector<ByteType>v;
    v.reserve(s.calculatedSize());
    for(auto i = 0; i < v.capacity(); i++)
        v.push_back(data[i]);

    return nlohmann::json::binary(v);
}


TEST(DataProvider, AssertsForTypesReturningBytes){
    char buf[2];

    static_assert(
        bm::traits::IsByteType<char>
    );
    static_assert(
        std::is_same_v<uint8_t*, bm::traits::is_byte_container<uint8_t*>::qualified_type>
    );
    static_assert(
        bm::traits::is_byte_container_v<char[2]>
    );
    static_assert(
        bm::traits::DataProvider<DataWrapper>
    );
        auto schema = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    }
}
)");

    const auto results = uses_provider(schema, DataWrapper{});
    std::cout << results << std::endl;

}
#include <fstream>
TEST(DataProvider, CanBeUsedWithSTLByteProviders){
    static_assert(
        bm::traits::DataProvider<std::vector<uint8_t>>
    );
    static_assert(
        bm::traits::DataStream<std::ifstream>
    );
    // std::ifstream inData {"data.bin"};
    // inData.read();
}