#include <gtest/gtest.h>
#include <types/traits.h>
#include <bm.h>
#include <TestUtils.h>
struct DataWrapper
{
    uint8_t d[8] = {0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x8};
    uint8_t *data() { return d; }
};

template <bm::traits::is_schema_type S, typename D>
    requires bm::traits::DataContainer<D> || bm::traits::DataStream<D>
nlohmann::json uses_provider(S s, D &d)
{
    nlohmann::json ret;
    if constexpr (bm::traits::DataContainer<D>)
    {
        auto data = d.data();
        using ByteType = bm::traits::is_byte_container<decltype(data)>::byte_type;
        std::vector<ByteType> v;
        v.reserve(s.calculatedSize());
        for (auto i = 0; i < v.capacity(); i++)
            v.push_back(data[i]);

        ret = nlohmann::json::binary(v);
    }
    else
    {
        std::vector<uint8_t> v(s.calculatedSize());
        d.read((char*)v.data(), s.calculatedSize());
        ret = nlohmann::json::binary(v);
    }

    return ret;
}
auto schema = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    }
}
)");

TEST(DataProvider, AssertsForTypesReturningBytes)
{
    char buf[2];

    static_assert(
        bm::traits::IsByteType<char>);
    static_assert(
        std::is_same_v<uint8_t *, bm::traits::is_byte_container<uint8_t *>::qualified_type>);
    static_assert(
        bm::traits::is_byte_container_v<char[2]>);
    static_assert(
        bm::traits::DataContainer<DataWrapper>);

    DataWrapper data{};
    const auto results = uses_provider(schema, data);
    std::cout << results << std::endl;
}
#include <fstream>
TEST(DataProvider, CanBeUsedWithSTLByteProviders)
{
auto schema128 = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    },
    "data": {
        "offset": 8,
        "fields":[
            {"name": "socket name","length": 64, "endian":"big"},
            {"name": "socket data","length": 32, "endian":"big"}
        ]
    }
}
)");

    auto Write_I = test_utils::WriteStrategy<256>([](const std::size_t& i, uint8_t& c){
        c = i;
    });
    auto data = test_utils::GenerateBinaryData<256>{};
    data += Write_I;
    test_utils::TestFile<256>(std::filesystem::path(TEST_DATA_PATH) / "data.bin", data);

    static_assert(
        bm::traits::DataContainer<std::vector<uint8_t>>);
    static_assert(
        bm::traits::DataStream<std::ifstream>);

    std::ifstream inData{std::filesystem::path(TEST_DATA_PATH) / "data.bin"};
    // inData.read();
    const auto results = uses_provider(schema128, inData);
    std::cout << results << std::endl;
}