#include <gtest/gtest.h>
#include <bm.h>
#include <TestUtils.h>
#include <cstdarg>


using namespace bm;

TEST(AttributeSpecification, EmplacesCorrectlyFromList){
    AttributeSpecification<std::string, std::size_t> spec = {{"name",""}, {"length", 60UL}};
    ASSERT_EQ(spec.attribute_count(), 2);
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

TEST(LayoutAttribute, InvalidatesOnMissingRequiredElements)
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
    ASSERT_TRUE(LayoutAttribute::containsProperLayout<ProceduralOffsetLayouts>(missing_offset["p1"]));
}

TEST(LayoutAttribute, CanUtilizeLengthsToGenerateOffsets){

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

TEST(Schema, CanHaveOffsetsGenerated){
    const auto schema = create_schema<bm::ProceduralOffsetValidator>(R"(
{
    "p1": {
        "fields":[{"name": "p1.header","length": 16, "endian":"big"}]
    },
    "p2": {
        "fields":[{"name": "p2","length": 32, "endian":"big"},{"name": "p2.2","length": 8, "endian":"big"}]
    },
    "p3": {
        "fields":[{"name": "p3","length": 32, "endian":"big"}]
    }
}
)");
    const auto p2 = (++schema.begin());
    ASSERT_EQ(p2->startOffset(), 16);

}

TEST(Schema, CanBeDerivedFromJSONFile){
    const auto schemaFile = bm::file::FileFactory::create({TEST_SCHEMAS_PATH, "256.json"});

    auto schema = schema_from_file(schemaFile);
    ASSERT_EQ(schema.calculatedSize(), 256);
}

TEST(Schema, CanBeDerivedFromLargeJSONFile){
    const auto schemaFile = bm::file::FileFactory::create({TEST_SCHEMAS_PATH, "large.json"});
    auto schema = schema_from_file<ProceduralOffsetValidator>(schemaFile);

    ASSERT_EQ(schema.get("bb").id, "bb");
}