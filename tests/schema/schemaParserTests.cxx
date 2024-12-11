#include <gtest/gtest.h>
#include <schema/Parser.h>

#include <TestUtils.h>

static auto SimpleJSONSchema = test_utils::JSONFrom("simple");
static auto MultiLayoutSchema = test_utils::JSONFrom("multispec");

using namespace bm;
TEST(SchemaParser, CanParseToplevelFields){
    const auto layout = Parser::Parse(SimpleJSONSchema);
    ASSERT_EQ(layout[0].id, "start");
}
TEST(SchemaParser, CanMapLayoutIDToAttributes){
    const auto schema = Parser::MapLayouts(MultiLayoutSchema);
}
TEST(SchemaParser, ErrorsOnOverlappingOffsetAttributes){

}