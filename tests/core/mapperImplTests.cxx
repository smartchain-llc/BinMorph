#include <gtest/gtest.h>

#include <bm.h>

#include <TestUtils.h>

struct CharBuffToStringMapper
{
    using ResultsType = std::string;

    ResultsType map(bm::Schema s, char *d, const std::size_t &len)
    {
        std::stringstream ss;
        for (auto i = 0; i < len; i++)
            ss << "0x" << std::hex << (short)d[i] << " ";
        return {ss.str()};
    }
};
TEST(ConcreteMapperImpl, AbidesByProviderConstraints)
{
    char dataToMap[32];
    auto schema = bm::create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    }
}
)");

    const auto results = bm::Map<CharBuffToStringMapper>(schema, dataToMap, 32);
    ASSERT_EQ(typeid(results).name(), typeid(CharBuffToStringMapper::ResultsType).name());

    static_assert(bm::traits::is_mappable<bm::ToJSONMapper, bm::Schema, char *, std::size_t>);
    const auto json_results = bm::Map<bm::ToJSONMapper>(schema, dataToMap, 32);
    std::cout << json_results << std::endl;
}