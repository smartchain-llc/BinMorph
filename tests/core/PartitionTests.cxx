#include <gtest/gtest.h>
#include <schema/attributes/Partition.h>
static auto json = nlohmann::json::parse(R"(
{
    "magic":{
        "offset": 0,
        "length": 8,
        "fields": [{}]
        }
}
)");

TEST(Partition, ThrowsIfMissingRequiredAttributes){
    auto badjson = nlohmann::json::parse(R"(
{
    "magic":{
        "fields": [{}]
        }
}
)");
    auto pinit = [&](){
        Partition p {
            badjson
        };
    };
    ASSERT_ANY_THROW(pinit());
}

TEST(Partition, PopulatesAttributes){
    Partition p {json};
    ASSERT_EQ(p.offset, 0);
    ASSERT_EQ(p.key, "magic");
    ASSERT_EQ(p.length, 8);
}