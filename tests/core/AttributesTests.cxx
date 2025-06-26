#include <gtest/gtest.h>
#include <schema/attributes/FieldAttributes.h>

TEST(FieldAttribute, CanBeConstructed){
    auto json = nlohmann::json::parse(R"(
{
    "name": "p1.header","length": 8, "endian":"big"
}
)");
    FieldAttribute fa { json };
    ASSERT_EQ(fa.endian, "big");
}

TEST(FieldAttribute, CanConstructContainerFromJSONArray){
    auto json = nlohmann::json::parse(R"(
{
    "fields":[
        {"name": "header","length": 8, "endian":"big"},
        {"name": "data","length": 4, "endian":"big"}
    ]
}
)");

    const auto fields = field_attributes(json["fields"]);
    ASSERT_EQ(fields.size(), 2);
    ASSERT_EQ(fields[1].length, 4); 
}

