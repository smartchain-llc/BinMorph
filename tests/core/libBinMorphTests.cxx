#include <gtest/gtest.h>
#include <TestUtils.h>
// #include <schema/Parser.h>
// #include <schema/SchemaFile.h>
#include <bm/schema.h>
#include <bm/input.h>
TEST(LibraryDevAPI, IsSeamless){
    std::filesystem::path inSchemaPath { std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json" };
    bm::SchemaFile schemaFile{ inSchemaPath };
    
    auto schema = bm::Parser::Parse(schemaFile);
    bm::SchemaMapper mapper { bm::ToJSONMapper{} };
    // mapper.map(schema, )
}