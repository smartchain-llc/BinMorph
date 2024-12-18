#include <gtest/gtest.h>
#include <TestUtils.h>
// #include <schema/Parser.h>
// #include <schema/SchemaFile.h>
#include <bm/schema.h>
#include <bm/input.h>
TEST(LibraryDevAPI, IsSeamless)
{
    std::filesystem::path inSchemaPath{std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json"};
    bm::SchemaFile schemaFile{inSchemaPath};
    bm::SchemaMapper<bm::ToJSONMapper> mapper;

    const auto binFilePath = std::filesystem::path(TEST_DATA_PATH) / "256.bin";
    bm::InputFile bin { binFilePath };
    // mapper.map(schemaFile.get(), )
}