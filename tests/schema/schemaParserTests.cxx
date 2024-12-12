#include <gtest/gtest.h>
#include <schema/Parser.h>
#include <schema/Schema.h>
#include <TestUtils.h>

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");
static auto MultiLayoutSchema = test_utils::JSONFrom("multispec");

using namespace bm;
TEST(SchemaParser, ParsesJSONToSchemaObject)
{
    Schema s;
    nlohmann::json schema = nlohmann::json::parse(R"(
        {
            "p1":{
                "offset":0,
                "fields":[
                    {"name":"p1","length":16,"endian":"big"}
                ]
            },"p2":{
                "offset":16,
                "fields":[
                    {"name":"p2","length":16,"endian":"big"}
                ]
            }
        }
    )");
    Parser::ParseTo(s, schema);
    ASSERT_NO_THROW(s.get("p1"));
}
TEST(ParsedSchema, SchemaThrowsWhenLayoutsOverlapOffsets)
{
    Schema s;
    nlohmann::json schema = nlohmann::json::parse(R"(
        {
            "p1":{
                "offset":0,
                "fields":[
                    {"name":"p1","length":16,"endian":"big"}
                ]
            },"p2":{
                "offset":5,
                "fields":[
                    {"name":"p2","length":16,"endian":"big"}
                ]
            }
        }
    )");
    ASSERT_ANY_THROW( Parser::ParseTo(s, schema) );
}
TEST(ParsedSchema, CanUtilizeRangedBasedForLoop){
    Schema s;
    nlohmann::json schema = nlohmann::json::parse(R"(
        {
            "p1":{
                "offset":0,
                "fields":[
                    {"name":"p1","length":16,"endian":"big"}
                ]
            },"p2":{
                "offset":16,
                "fields":[
                    {"name":"p2","length":16,"endian":"big"}
                ]
            }
        }
    )");
    Parser::ParseTo(s, schema);
    for(const auto layout : s){
        ASSERT_NO_THROW(layout.byteLength());
    }
}