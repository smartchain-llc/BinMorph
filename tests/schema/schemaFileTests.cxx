#include <gtest/gtest.h>
#include <bm/schema.h>
#include <bm/input.h>
#include <TestUtils.h>
#include <iostream>


TEST(SchemaFile, ValidatesFilePathArg){
    bm::SchemaFile in { std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json"};
}
TEST(SchemaFile, UsesSpecificFileValidatorClass){
    bm::SchemaFile input{std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json"};
    auto schema = input.schema();
    ASSERT_EQ(schema.calculatedSize(), 256);
}