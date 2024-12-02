#include <gtest/gtest.h>
#include <schema/SchemaFile.h>
#include <TestUtils.h>
#include <fsUtils.h>

TEST(BinaryDataTestUtils, CreateFilesAppropriately){
    const auto filePath = test_utils::toPath({TEST_DATA_PATH, "simple.bin"});
    test_utils::CreateAndWrite(
        filePath,
        test_utils::data::Simple,
        test_utils::data::Simple_Data_Len
    );
    ASSERT_TRUE(std::filesystem::exists(filePath));
}