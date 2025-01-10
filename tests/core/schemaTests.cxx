#include <gtest/gtest.h>
#include <bm.h>
#include <TestUtils.h>

using namespace bm;


TEST(SchemaAttributes, InvalidatesOnMissingRequiredElements)
{
    const auto missing_fields = nlohmann::json::parse(R"(
{
    "p1": {
        "offset": 0
    }
}
)"); // No required fields
    const auto missing_offset = nlohmann::json::parse(R"(
{
    "p1": {
        "fields": [{}]
    }
}
)");
    ASSERT_FALSE(LayoutAttribute::containsProperLayout(missing_fields["p1"]));
    ASSERT_FALSE(LayoutAttribute::containsProperLayout(missing_offset["p1"]));
}

TEST(FieldAttributes, InvalidatesOnMissingRequiredElements)
{
    const auto missing_name = nlohmann::json::parse(R"({"length": 0, "endian":"big"})");
    const auto missing_length = nlohmann::json::parse(R"({"name": "0", "endian":"big"})");
    const auto missing_endian = nlohmann::json::parse(R"({"length": 0, "length": 0})");
    const auto valid = nlohmann::json::parse(R"({"name": "0","length": 0, "endian":"big"})");
    ASSERT_FALSE(FieldAttribute::containsRequired(missing_name));
    ASSERT_FALSE(FieldAttribute::containsRequired(missing_length));
    ASSERT_FALSE(FieldAttribute::containsRequired(missing_endian));
    ASSERT_TRUE(FieldAttribute::containsRequired(valid));
}
TEST(Schema, CanBeInitializedWithRawStringInput)
{
    const auto schema = create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 8, "endian":"big"}]
    }
}
)");
    ASSERT_EQ(schema.get("p1").offset, 0);
}

TEST(Schema, GeneratesLengthOfBytesToMap){
    const auto schema = create_schema(R"(
{
    "p1": {
        "offset": 0,
        "fields":[{"name": "p1.header","length": 16, "endian":"big"}]
    },
    "p2": {
        "offset": 16,
        "fields":[{"name": "p2","length": 32, "endian":"big"}]
    }
}
)");

    constexpr auto p1Size { 16 };
    constexpr auto p2Size { 32 };

    ASSERT_EQ(schema.calculatedSize(), p1Size + p2Size);
}