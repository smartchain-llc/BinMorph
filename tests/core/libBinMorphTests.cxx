#include <gtest/gtest.h>
#include <TestUtils.h>
#include <schema/Parser.h>
#include <schema/SchemaFile.h>

TEST(LibraryDevAPI, IsSeamless){
    std::filesystem::path inSchemaPath { std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json" };
    bm::SchemaFile schemaFile{ inSchemaPath };
    
}