#include <gtest/gtest.h>
#include <bm.h>

#include <fstream>

#include <TestUtils.h>

#include "mappertest.h"

TEST(BinMorphCLITool, DefaultsToAllJSONUsage){
    auto jsonSchemaFilePath = test_utils::JSON_Test_Schemas.at("256");

    const auto jsonSchema = bm::create_schema(jsonSchemaFilePath.string());

    std::ifstream dataFile = { std::filesystem::path(TEST_DATA_PATH) / "256.bin" };   

    const auto outputFilePath = jsonSchemaFilePath.parent_path() / "256.out.json";
    std::ofstream outFile { outputFilePath };

    // bmmap<CharBuffToStringMapper>(jsonSchema);
    // const auto m = Map<bm::ToJSONMapper>{}()
    static_assert(
        is_mapper<int>
    );
    // bm::Map<bm::ToJSONMapper>(jsonSchema, dataFile, )
}