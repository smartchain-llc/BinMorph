#include <gtest/gtest.h>
#include <schema/SchemaFile.h>
#include <TestUtils.h>
#include <iostream>
TEST(SchemaFile, ValidatesFilePathArg){
    bm::SchemaFile in { std::filesystem::path(TEST_SCHEMAS_PATH) / "256.json"};
    std::cout << in.JSON() << std::endl;
}